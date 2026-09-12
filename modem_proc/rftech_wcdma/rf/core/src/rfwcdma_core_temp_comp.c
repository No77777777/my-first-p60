/*!
  @file
  rfwcdma_core_temp_comp.c

  @brief
  Provides WCDMA Temperature Compensation functionality

*/

/*===========================================================================

Copyright (c) 2008 -2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:08:47 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_temp_comp.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
18/04/16   ag      Correct the autopin check for temp comp update 
07/04/16   vs      Correct the autopin check for temp comp update
06/06/16   wxl     Check autopin flag in DM for temp comp
02/17/16   ag      Appy pin/pout through temp comp update when WTR autopin support absent
12/17/15   ak      Fix Critical KW Errors
12/16/15   ak      Fix Scheduling Issue in Therm
12/15/15   ag      Skip calling temp comp when autopin is enabled
12/01/15   ag      Fix KW warnings
11/23/15   ag      Update temp comp in progress to false in callback API in SW
11/19/15   ag      Skip calling temp comp update when autopin and fbrx are on
10/10/15   zhh     only schedule FED based therm for WTR4905
06/02/15   gh/zhh  Add support for FED based therm_read
09/18/15   aa      Save reference NV pin value to be added to autopin accumulator
09/17/15   kma     Added auto pin support for Pin cal
07/14/15    aa     Added support for FBRx C1 NV container
04/24/15   vbh     Fixed temp comp init logic
07/04/15   ak      Migrating to new WCDMA state machine
07/04/15   ak      Rx/Tx Split Changes 
03/10/15   dw      Added support for DCHSUPA ref Pin
12/03/14   aa      Added support to normalize FBRx_GAIN_VS_TEMP_VS_FREQ items to cal temperature 
10/29/14   kai     Fix FBRx temp comp sequence issue during init Tx 
09/15/14   vbh     [1] Added device as a parameter to support Tx sharing
                   [2] Removed hardcoding on device 0
03/12/14   zhh    Update XPT Delay structure to support DC-UPA
08/07/14   kai     mutex the entire temp comp in decfg Tx
08/06/14   aro     Support to override TempComp mode of operation
04/14/14   zhh     PinPout NV for DCUPA
07/21/14   JJ      Use therm bins from each tech to get temp comp value
07/02/14   kr/kai  Non-0dB Pin SW implementation to support -ve pin offsets values
07/02/14   kai     Do temp_comp_lock init once
07/01/14   aro     Collision Prevention between TxPlim and TempComp
06/25/14   bn      Therm bin in RFLM logging
06/19/14   aro/ak  Added Support for Prep/Exec Temp Comp Split
06/16/14   vs/kai  Non-0dB Pin SW implementation 
05/09/14   kai     Add rfwcdma_core_check_temp_comp_done API
05/01/14   dw      Added update ET delay and set freq adjust in MDSP async set data command
03/24/14   kai     Add ET delay update during temp comp 
02/24/14   rmb     Fix OFT Compilation Error.
02/21/14   rmb     Add global NV pointers for each of band specific static NVs.
02/14/14   kai     Remove the legacy temp comp
02/13/14   aa      Update fbrx temp comp
01/02/14   dw      Bolt cleanup on legacy mdsp function call 
11/14/13   ka      Added fbrx temp comp
10/22/13   dw      Fix warning
09/10/13   dw      Remove DBDC featurization and code clean up.
07/19/13   ac      DBDC related changes
07/24/13   kai     Added XPT path delay offset vs temp vs freq
07/08/13   sma     Midify "rfwcdma_core_temp_comp_read_therm" to retun raw temp
06/05/13   cd      xPT temp comp - ref therm ADC value from multilin NV must
                   be scaled
05/13/13   cd      Temp Comp - Fixed compiler warning because of feature
05/09/13   rmb     Added support for avoiding K-sensor and temp comp update 
                   while doing TDET read.
05/09/13   cd      Temp Comp - Support legacy and xPT temp comp when xPT is
                   enabled
                   - Run Tx temp comp updates only if driver is in RXTX state
05/06/13   tks     Added support for rsb comp with temp change
05/02/13   cd      Added API to convert raw therm read to scaled value
04/26/13   cd      Temp Comp - support for multi-chan linearizer compensation
04/25/13   rmb     Fix Compiler Error
04/25/13   rmb     Added temperature compensation for QFE TDET in every temp comp update before thremistor read.
04/09/13   cd      Obtain Pin/Pout offsets only for valid PA states
04/04/13   ka      WCDMA state machine updates
03/29/13   cd      XPT Temp Comp, fix Tx wakeup temp comp application
03/20/13   cd      XPT Temp Comp, fix KW error
03/20/13   cd      XPT Temp comp support
03/19/13   dw/kai  [1]Added support for HDET based linearizer compensation 
                   [2]Added support to limit allowable HDET mod range 
02/14/12   ac      feature and tdet code cleanup
02/04/13   ka      Added rfwcdma_core_temp_comp_set_enable
10/15/12   kai     Changed QFE1510 HDET APIs
09/26/12   kai     Changed name of K sensor comp apis to ant common api
08/02/12   kai     Add QFE1510 HDET and K val comp support for W online mode
09/28/12   ac      device interface to pass band to rfc_wcdma_get_device
05/03/12   nv      Fixed KW warning
03/27/12   dw      Fix max power issue that NV limit was not initalized 
                   properly until temp comp kicks in
03/27/12   kai     Add null pointer check for return value of rfwcdma_core_txlin_get_band_data 
03/06/12   gh      16-channel Tx lin_vs_temp feature
11/16/11   dw      Change to update LUT buffer index after LUT for all PA ranges are updated
10/26/11   dw      Support for closed loop AGC in FTM
10/25/11   ac/jn   Fix the check for invalid temp comp thermistor reading.
09/30/11   dw      Nikel temp comp updates
09/06/11   dw      Fix warning 
09/06/11   dw      APT cleanup
08/30/11   dw      Fixed Lint warning
08/23/11   ac      tech specific command dispatch
04/06/11   ad      Port APT changes from MDM9k 
10/19/10   vb      All device functions are now accessed thru device interface functions 
08/19/10   ka      Enable temp comp for voltage scaling 
08/11/10   ka      Update tx temp comp when enabling tx 
08/04/10   ad      Disable applying temp comp during FTM mode
03/16/10   kguo    Merged from scmm_final_merge branch
01/21/10   bn      Lint Fixes
01/08/10   ckl     Removed high level lint error.
11/05/09   sar     Removed deprecated file therm.h.
09/21/09   jhe     temp comp
09/18/09   sar     Updated ADC interface for CMI conformance.
08/13/09   jhe     some temp comp cleaning
03/15/09   ckl     Deleted rfnv.h
03/09/09   ckl     Code cleanup
11/06/08   av      Initial version.

============================================================================*/
#include "rfwcdma_core_temp_comp.h"
#include "DDIAdc.h"
/*! @todo therm.h will be deprecated. Need to revisit later */
#include "comdef.h"
#include "rfwcdma_core_txlin.h"
#include "rfwcdma_core_txplim.h"
#include "rfwcdma_core_rxctl.h"
#include "rfnv_wcdma.h"
#include "rfdevice_wcdma_intf.h"
#include "rfcommon_nv_mm.h"
#include "ftm.h"
#include "pm.h"
#include "rfc_card.h"
#include "rfwcdma_mdsp.h"
#include "rfwcdma_mdsp_async.h"
#include "rfwcdma_mc.h"
#include "rfm_internal.h"
#include "rfcommon_math.h"
#include "rfwcdma_core.h"
#include "rfwcdma_core_util.h"
#include "rfwcdma_core_xpt.h"
#include "rfcommon_fbrx_api.h"
#include "rfwcdma_data.h"
#include "rfcommon_locks.h"
#include "rflm_wcdma_sw_api.h"
#include "rfdevice_hdet_cmn_intf.h"
#include "rfdevice_hdet_wcdma_intf.h"
#include "rf_cmd_interface.h"
#include "rflm_wcdma_msg.h"
#include "rfcommon_autopin_api.h"
#include "rfcommon_core_utils.h"
#include "rflm_api_autopin_hw.h"

/* The temperature compensation uses interpolated tables of size
   NV_TEMP_TABLE_SIZ.  The temperature value from the therm services
   is scaled 0..THERM_SCALE.  The bin size between the entries in the
   table is RFI_TEMP_BIN_SIZ.  Note that this is a scalar, and the
   result of the division can make 1 bin smaller than RFI_TEMP_BIN_SIZ.
*/
/*! todo will use THERM_SCALE below ? */
#define RFWCDMA_CORE_TEMP_BIN_SIZ (RFWCDMA_CORE_THERM_MAX / (NV_TEMP_TABLE_SIZ-1))
#define RFWCDMA_CORE_THERM_MIN 0
#define RFWCDMA_CORE_THERM_MAX 255

#define RFWDCMA_FREQ_BIN_RESOLUTION 256

rfwcdma_core_temp_comp_value_type rfwcdma_core_temp_comp_value =
{
  FALSE,
  2560,
  110
};

/* Therm Bin array */
uint16 therm_bin_array[NV_TEMP_TABLE_SIZ];
uint16 temp_comp_per_dev =0; 

static boolean rfwcdma_core_temp_comp_is_enabled = TRUE;
static rf_lock_data_type rfwcdma_core_temp_comp_lock_data;
static boolean rfwcdma_code_temp_comp_through_fed = TRUE;

extern rfwcdma_mc_state_type rfwcdma_mc_state;
extern rfwcdma_core_txlin_record_type rfwcdma_core_current_txlin_data;
extern int16 pout_comp_auto_pin_ref[RFCOM_TXLIN_NUM_LINEARIZERS];
extern int16 pin_comp_auto_pin_ref[RFCOM_TXLIN_NUM_LINEARIZERS];
extern boolean is_autopin_supported;

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Convert the Raw Therm Read value to a scaled therm read value between 0
  and 255, based on Therm min and Therm max NV values
 
  @param raw_therm_read
  Current raw therm read
 
  @return
  Return the scaled therm read value between 0 - 255
*/
uint8
rfwcdma_core_temp_comp_get_scaled_therm_read( uint16 raw_therm_read )
{
  uint16 therm_scaled_value = 0;
  uint16 therm_raw_range;

  /* Sanity check */
  if ( raw_therm_read >= 0x0FFF ) 
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "rfwcdma_core_temp_comp_get_scaled_therm_read: "
           "Invalid Raw Therm Value (%d) Discarded", 
           raw_therm_read );

    /* wrong raw reading, return last scaled value */
    return( rfwcdma_core_temp_comp_value.last_scaled_temp_value );
  }
  else if ( raw_therm_read > rfcommon_nv_mm_tbl.rf_enh_therm_max )
  {
    raw_therm_read = (uint16)rfcommon_nv_mm_tbl.rf_enh_therm_max;
  }
  else if ( raw_therm_read < rfcommon_nv_mm_tbl.rf_enh_therm_min )
  {
    raw_therm_read = (uint16)rfcommon_nv_mm_tbl.rf_enh_therm_min;
  }

  /* Scale to 0-255 range */
  therm_raw_range = (uint16)(rfcommon_nv_mm_tbl.rf_enh_therm_max - 
                             rfcommon_nv_mm_tbl.rf_enh_therm_min);

  if( therm_raw_range != 0 )
  {
    therm_scaled_value = 
       (uint16)( ((raw_therm_read - rfcommon_nv_mm_tbl.rf_enh_therm_min) * 
                  RFWCDMA_CORE_THERM_MAX) / 
                 therm_raw_range );

    /* if it exceeds the max then return the max */
    therm_scaled_value = MIN( therm_scaled_value, RFWCDMA_CORE_THERM_MAX );
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, 
          "rfwcdma_core_temp_comp_get_scaled_therm_read: "
          "Raw Therm Range is zero", 0);
    therm_scaled_value = rfwcdma_core_temp_comp_value.last_scaled_temp_value;
  }

  return (uint8)therm_scaled_value;
}/* rfwcdma_core_temp_comp_get_scaled_therm_read */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Convert the scaled Therm Read value to a raw therm read value between 
 
  @param scaled therm read
  
  @return
  Return the raw therm read value 
*/

uint16
rfwcdma_core_temp_comp_get_raw_therm_read( uint8 scaled_therm )
{
  uint16 raw_therm       = 0; 
  uint16 raw_therm_range = 0;

  raw_therm_range = (uint16)(rfcommon_nv_mm_tbl.rf_enh_therm_max - 
                             rfcommon_nv_mm_tbl.rf_enh_therm_min);

  /* scaled_therm is type uint8 in range [0,255] */
  if (raw_therm_range != 0)
  {
    raw_therm = ((scaled_therm * raw_therm_range)/RFWCDMA_CORE_THERM_MAX) 
                + rfcommon_nv_mm_tbl.rf_enh_therm_min; 
  }
  else
  {
    raw_therm = 0; 
  }

  return raw_therm; 
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Obtain the Pin and Pout compensation values from the common linearizer
  compensation interface
 
  @details
  This API provides functionality to compute the current Pin and Pout
  temperature vs. frequency compensation offsets, based on data from
  static NV items LIN_VS_TEMP_VS_FREQ_PIN/POUT_I. For any error scenarios
  and for invalid PA states, the offsets are set to zero.

  @param scaled_temperature_read
  This is the current therm read, scaled to 0-255
 
  @param pin_comp_offset
  Array of int16 Pin values, populated as output from this function
 
  @param pout_comp_offset
  Array of int16 Pout values, populated as output from this function
 
*/
static
void rfwcdma_core_temp_comp_vs_freq_get_comp(
                                      uint8 scaled_temperature_read,
                                      int16 pin_comp_offset[],
                                      int16 pout_comp_offset[] )
{
  /* Multilinearizer NV data retrieval */
  tx_band_cal_type *tx_multilin_nv_cal_ptr = NULL;

  tx_lin_vs_temp_vs_freq_comp_param_type temp_vs_freq_params;
  uint8 pa_state;
  uint32 ref_lin_freq;  
  rfm_device_enum_type rfm_tx_device = RFM_MAX_WAN_DEVICES;


  /* Pointer to the WCDMA Txlin data */
  rfwcdma_core_txlin_record_type *curr_txlin_data;
    /*Update the rfm_device for tx*/
  rfm_tx_device = rfwcdma_data_get_tx_device();
  if(rfm_tx_device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_temp_comp_vs_freq_get_comp: Invalid tx device %d",
                        rfm_tx_device);
    return;
  }
	
  rfcom_wcdma_band_type band = rfwcdma_data_get_curr_band(rfm_tx_device);

  /* Skip temp comp offset computation in cal state and in Rx only state */
  if ( (IS_FTM_IN_TEST_MODE()&&(rfm_get_calibration_state() == TRUE)) )
  {
    for (pa_state = 0; pa_state < RFCOM_TXLIN_NUM_LINEARIZERS; pa_state++)
    {
      /* Compute the Power compensation value */
      pin_comp_offset[pa_state] = 0;
      pout_comp_offset[pa_state] = 0;
    }
    return;  
  }

  /* Get current Tx linearizer data */
  curr_txlin_data = rfwcdma_core_get_current_txlin_data();

  /* For the current band get the pointer to complete linearizer band cal 
  data */
  tx_multilin_nv_cal_ptr = 
                 rfwcdma_core_txlin_get_band_data(band);

  /* Null pointer check */
  if (curr_txlin_data == NULL || tx_multilin_nv_cal_ptr == NULL)
  {
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
             "rfwcdma_core_temp_comp_vs_freq_get_comp:"
             "Invalid WCDMA Tx Lin data", 0);
      for (pa_state = 0; pa_state < RFCOM_TXLIN_NUM_LINEARIZERS; pa_state++)
      {
        /* Compute the Power compensation value */
        pin_comp_offset[pa_state] = 0;
        pout_comp_offset[pa_state] = 0;
      }
      return;
  }

  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_vs_freq_get_comp: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_vs_freq_get_comp: Not a valid band %d",
               band);
    return;
  }

  /* Initialize input data for the function to compute offset */
  temp_vs_freq_params.freq_params.tx_freq = 
        rfwcdma_core_get_freq_from_ul_arfcn(rfwcdma_data_get_curr_chan(rfm_tx_device, RFCOM_SINGLE_CARRIER));
  temp_vs_freq_params.freq_params.waveform_type = TX_LINEARIZER_VOICE;

  temp_vs_freq_params.temp_params.temp_read = (uint16)scaled_temperature_read;
  temp_vs_freq_params.temp_params.ref_lin_temp_adc = 
        rfwcdma_core_temp_comp_get_scaled_therm_read( 
                                   tx_multilin_nv_cal_ptr->ref_lin_temp_adc );

  temp_vs_freq_params.tx_multilin_nv_cal_ptr = 
                                    (tx_band_cal_type*)tx_multilin_nv_cal_ptr;

  for ( pa_state = 0; 
        pa_state < rfwcdma_core_current_txlin_data.num_valid_pa_state; 
        pa_state++ )
  {
    /* Provide current PA state */
    temp_vs_freq_params.pa_state = pa_state;
    /* Find ref linearizer frequency per PA state */
    rfcommon_core_txlin_get_ref_linearizer_frequency( 
                             (tx_band_cal_type*)tx_multilin_nv_cal_ptr, 
                             temp_vs_freq_params.freq_params.tx_freq,
                             pa_state,
                             TX_LINEARIZER_VOICE,
                             &ref_lin_freq );
    temp_vs_freq_params.freq_params.ref_lin_tx_freq = ref_lin_freq;
    temp_vs_freq_params.ref_xpt_pin_offset = 0;
    if( curr_txlin_data->current_lin_tbl[pa_state].dpd_type == TX_LINEARIZER_ET_DPD_DATA )
    {
      if (rfwcdma_data_get_num_carriers(rfm_tx_device) == RFCOM_DUAL_CARRIER )
      {
        temp_vs_freq_params.ref_xpt_pin_offset = 
         (int32)rfwcdma_core_xpt_get_cfg_param( band, XPT_CFG_WCDMA_DC_REF_ET_PIN_OFFSET  );
      }
      else
      {
     temp_vs_freq_params.ref_xpt_pin_offset = 
       (int32)rfwcdma_core_xpt_get_cfg_param( band, XPT_CFG_CMN_REF_ET_PIN_OFFSET );
    }
    }
    else if( curr_txlin_data->current_lin_tbl[pa_state].dpd_type == TX_LINEARIZER_EPT_DPD_V2_DATA )
    {
      if (rfwcdma_data_get_num_carriers(rfm_tx_device) == RFCOM_DUAL_CARRIER )
      {
        temp_vs_freq_params.ref_xpt_pin_offset = 
         (int32)rfwcdma_core_xpt_get_cfg_param( band, XPT_CFG_WCDMA_DC_REF_EPT_PIN_OFFSET  );
      }
      else
      {
     temp_vs_freq_params.ref_xpt_pin_offset = 
       (int32)rfwcdma_core_xpt_get_cfg_param( band, XPT_CFG_CMN_REF_EPT_PIN_OFFSET );
      };
    }

    temp_vs_freq_params.auto_pin_enable = (rfcommon_autopin_is_enabled_flag_check(rfwcdma_data_get_rflm_buf_idx(rfm_tx_device)) && rflm_autopin_sanity_check(rfwcdma_data_get_rflm_buf_idx(rfm_tx_device))
		                           && is_autopin_supported);

    /* Get compensation values */
    if((RFCOM_DUAL_CARRIER == rfwcdma_data_get_num_carriers(rfm_tx_device))&& 
       (TRUE == rfnv_wcdma_tbl_ptr[band]->dc_lin_temp_comp_data.has_pin_pout_temp_comp_nv))
    {
      rfcommon_core_txlin_get_temp_compensation( 
         curr_txlin_data->temp_comp_algo,
         &pin_comp_offset[pa_state],
         &pout_comp_offset[pa_state],
         &temp_vs_freq_params,
         &(rfnv_wcdma_tbl_ptr[band]->dc_lin_temp_comp_data));
    }
    else
    {
      rfcommon_core_txlin_get_temp_compensation( 
         curr_txlin_data->temp_comp_algo,
         &pin_comp_offset[pa_state],
         &pout_comp_offset[pa_state],
         &temp_vs_freq_params,
         &(rfnv_wcdma_tbl_ptr[band]->lin_temp_comp_data));
    }
      pin_comp_auto_pin_ref[pa_state] = pin_comp_offset[pa_state];
      pout_comp_auto_pin_ref[pa_state] = pout_comp_offset[pa_state];
  }
  /* Initialize the compensation values for the invalid PA states to zero */
  for ( ; pa_state < RFCOM_TXLIN_NUM_LINEARIZERS; pa_state++ )
  {
    pout_comp_offset[pa_state] = 0;
    pin_comp_offset[pa_state] = 0;
    /*MSG_1( MSG_SSID_RF, MSG_LEGACY_MED,
           "rfwcdma_core_temp_comp_vs_freq_get_comp: Invalid PA state %d, "
           "setting Pin and Pout offsets to zero", 
           pa_state );*/
  }
}/* rfwcdma_core_temp_comp_vs_freq_get_comp */

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function reads temperature from temperature sensor then validates the
  temperature value.

  @return uint8 Temperature value

  @todo changlee Confirm which resolution will be used, 8bit or 10bit.

*/
uint8 rfwcdma_core_temp_comp_read_therm(rfm_device_enum_type device)
{
  rfwcdma_mdsp_set_data_param_type therm_data = { 0 };
  uint16 therm_scaled_value = 0;
  uint16 therm_raw_value = 0;
  uint16 therm_raw_range = 0;
  rfwcdma_temp_comp_data_type *tc_data = NULL;
  boolean status = TRUE;

  /* Get temp_comp data */
  tc_data = rfwcdma_data_get_temp_comp(device);

  /* Sanity */
  if (tc_data == NULL)
  {
     MSG( MSG_SSID_RF, MSG_LEGACY_ERROR,
          "rfwcdma_core_temp_comp_read_therm: NULL data pointer");
     return (uint8)therm_scaled_value;
  }

  if (rfwcdma_core_temp_comp_is_enabled)
  {
    /* If therm_enable is done via FED then just do read and disable */
    if ((tc_data->therm_enable_triggered_by_fed) && (!rfm_get_calibration_state()))
    {
      status = rfdevice_wcdma_therm_read_and_disable( device,
                                                      rfwcdma_data_get_curr_band(device),
                                                      &therm_raw_value
                                                      );
    }
    /* Otherwise do enable and read */
    else
    {
      status &= rfdevice_wcdma_tx_cmd_dispatch(device,
                                               rfwcdma_data_get_curr_band(device),
                                               RFDEVICE_GET_THERMISTER_VALUE, &therm_raw_value);
    }

    /* sanity check */
    if ( therm_raw_value >= 0x0FFF ) 
    {
      MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH,"Invalid Raw Therm Value (%d) Discarded", therm_raw_value);
      /* wrong raw reading, return last scaled value */
      return(rfwcdma_core_temp_comp_value.last_scaled_temp_value);
    }
    else if (therm_raw_value > rfcommon_nv_mm_tbl.rf_enh_therm_max)
    {
      therm_raw_value = (uint16)rfcommon_nv_mm_tbl.rf_enh_therm_max;
    }
    else if (therm_raw_value < rfcommon_nv_mm_tbl.rf_enh_therm_min)
    {
      therm_raw_value = (uint16)rfcommon_nv_mm_tbl.rf_enh_therm_min;
    }

    /* save for logging etc */
    rfwcdma_core_temp_comp_value.last_raw_temp_value = therm_raw_value;

    /* scale to 0-255 range */
    therm_raw_range = (uint16)(rfcommon_nv_mm_tbl.rf_enh_therm_max - rfcommon_nv_mm_tbl.rf_enh_therm_min);
    if(therm_raw_range != 0)
    {
      therm_scaled_value = (uint16)(((therm_raw_value - rfcommon_nv_mm_tbl.rf_enh_therm_min) * RFWCDMA_CORE_THERM_MAX) / therm_raw_range);
      
      /* if it exceeds the max then return the max */
      therm_scaled_value = MIN(therm_scaled_value, RFWCDMA_CORE_THERM_MAX);
    }
    else
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Raw Therm Range is NULL!", 0);
    }

    rfwcdma_core_temp_comp_value.last_scaled_temp_value = (uint8)therm_scaled_value;

    /* Update FED DM with updated therm values */
    therm_data.data.set_therm.therm_bin = rfwcdma_core_temp_comp_value.last_scaled_temp_value / RFWCDMA_CORE_TEMP_BIN_SIZ;
    therm_data.data.set_therm.therm_rem = rfwcdma_core_temp_comp_value.last_scaled_temp_value % RFWCDMA_CORE_TEMP_BIN_SIZ;
    therm_data.set_data_cmd = RFWCDMA_MDSP_SET_THERM;
    therm_data.rflm_handle = (rflm_dm_handle_id_t)rfwcdma_data_get_rflm_buf_idx(device);
    rfwcdma_mdsp_async_set_data(&therm_data);
  }

  /* DBG MSG */
  MSG_4(MSG_SSID_RF, MSG_LEGACY_HIGH,
        "THERM_READ: device %d, FED Triggered %d, Raw therm val %d, scaled therm %d",
        device,
        tc_data->therm_enable_triggered_by_fed,
        therm_raw_value,
        (uint8)therm_scaled_value
        );

  /* Reset temp comp FED flag */
  tc_data->therm_enable_triggered_by_fed = FALSE;

  return (uint8)therm_scaled_value;
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function calculates temperature compensation value from temp_table[]
  by linear interpolation.

  @param temp_table[] Temperature compensation table
  @param temp_index Temperature table index
  @param temp_rem Remainder value

  @return int16 Interpolated temperature compensation value

*/
int16 rfwcdma_core_temp_comp_calc_comp_val(
                            int16 temp_table[], uint8 temp_index, uint8 temp_rem
                            )
{
  int16 delta;


  /* If the index is not the last entry, find the delta between
     table entries, scale it by the remainder divided by the
     temperature bin size, to get the delta between the Ith entry
     and the correct value.
  */
  if ( temp_index < (NV_TEMP_TABLE_SIZ-1) )
  {
    /* Calculate the difference between table entries */
    delta  = temp_table[ temp_index + 1 ] - temp_table[ temp_index ];

    /* Scale the difference by the remainder and divide
       it by the bin size, to get the correct difference
       between the Ith entry and the correct value.
       The result is rounded to reduce error to 1/2 LSB.
    */
    delta *= temp_rem;
    delta += RFWCDMA_CORE_TEMP_BIN_SIZ/2;
    delta /= RFWCDMA_CORE_TEMP_BIN_SIZ;

    /* return the interpolated value */
    return( temp_table[ temp_index ] + delta );
  }
  else
  {
    /* return max value */
    return( temp_table[ NV_TEMP_TABLE_SIZ-1 ] );
  }
}


/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  Clear or set temperature compensation enable flag

  @param flag Enable or Disable Temperature compensation

*/
void rfwcdma_core_temp_comp_enable(boolean flag, rfcom_device_enum_type device)
{
  rfwcdma_temp_comp_data_type *tc_data;
  rfwcdma_core_temp_comp_value.temp_comp_enable_flag = flag;

  if(TRUE == flag)
  {
    tc_data = rfwcdma_data_get_temp_comp(device);
    if (rfwcdma_get_temp_comp_lock_data() != NULL && tc_data != NULL)
    {
      tc_data->crit_section = rfwcdma_get_temp_comp_lock_data();
    }
  }
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  Performs RF voltage scaling temperature compensation

  @param uint8 therm_bin, uint8 therm_remainder

*/
void rfwcdma_core_temp_comp_rf_voltage_update(uint8 temp_index, uint8 temp_rem)
{
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function updates temperature dependent RF parameters.

  @param temp_index Temperature table index
  @param temp_rem Remainder value

*/
void rfwcdma_core_temp_comp_calc_data(rfm_device_enum_type device,
                                      uint8 temp_index, 
                                      uint8 temp_rem,
                                      uint8 scaled_temperature_read)
{
  uint8 i;
  int16  vga_gain_offset_vs_temp_val, c1_vga_gain_offset_vs_temp_val;
  int16  tx_lim_vs_temp_val, utran_tx_lim_vs_temp_offset_val, hdet_vs_temp_val,hdet_mod_range[2],hdet_mod_base=0;
  rfwcdma_core_txagc_comp_type update_type[RFCOM_TXLIN_NUM_LINEARIZERS];
  int16 pout_comp_offset[RFCOM_TXLIN_NUM_LINEARIZERS];
  int16 pin_comp_offset[RFCOM_TXLIN_NUM_LINEARIZERS];

  
  rfcom_wcdma_band_type band = rfwcdma_data_get_curr_band(device);
  /*Band updation to strictly happen based on device. Modify code flow if band information is not available at this time.*/
  
 
  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_calc_data: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_calc_data: Not a valid band %d",
               band);
     return;
  }


  /* find a new temperature compensation value for each NV table */
  vga_gain_offset_vs_temp_val =
  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->vga_gain_offset_vs_temp,
                          temp_index,
                          temp_rem );

  c1_vga_gain_offset_vs_temp_val =
  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->c1_vga_gain_offset_vs_temp,
                          temp_index,
                          temp_rem );

  rfwcdma_core_rxctl_temp_val_update(vga_gain_offset_vs_temp_val, c1_vga_gain_offset_vs_temp_val);

  /* Tx related temperature compensation, only perform if device is in RXTX state */
  if (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_TX)
  {
    /* Obtain the linearizer Pin and Pout vs temp and freq compensation values */
    rfwcdma_core_temp_comp_vs_freq_get_comp(scaled_temperature_read,
                                            pin_comp_offset,
                                            pout_comp_offset);

    /* Update AGC comp type before doing linearizer update */
  
    for (i=0; i < RFCOM_TXLIN_NUM_LINEARIZERS; i++)
    {
      if (rfwcdma_core_txlin_get_comp_type(i)==TX_AGC_TEMP_COMP)
      {
        update_type[i] = TX_AGC_TEMP_COMP;
      }
      else
      {
        update_type[i] = TX_AGC_COMP_INVALID;

        /* If HDET Comp is unabled, skip temp comp linearizer update for
        this PA state, but remember the static temp comp value as
        the base to calculate allowable HDET based Tx error correction range */
        if (rfwcdma_core_txlin_get_comp_type(i)==TX_AGC_HDET_COMP)
        {
          hdet_mod_base = pin_comp_offset[i];
        }
      }
    }
    

   #ifdef FEATURE_RF_HAS_WTR2965
   if(!(rfcommon_autopin_is_enabled_flag_check(rfwcdma_data_get_rflm_buf_idx(device)) && rflm_autopin_sanity_check(rfwcdma_data_get_rflm_buf_idx(device))
        && is_autopin_supported))
   {
    rfwcdma_core_txlin_hdet_temp_comp_update(device,
                                             pin_comp_offset, 
                                             pout_comp_offset, 
                                             update_type);
   }
   #endif
   #ifdef FEATURE_RF_HAS_WTR3925
   if(!(rfcommon_autopin_is_enabled_flag_check(rfwcdma_data_get_rflm_buf_idx(device)) && rflm_autopin_sanity_check(rfwcdma_data_get_rflm_buf_idx(device))
     &&(rfcommon_fbrx_mc_is_fbrx_enabled(RFLM_TECH_WCDMA)) ) )
   {
    rfwcdma_core_txlin_hdet_temp_comp_update(device,
                                             pin_comp_offset, 
                                             pout_comp_offset, 
                                             update_type);
   }
   #endif
   

    tx_lim_vs_temp_val =
    rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->tx_lim_vs_temp,
                            temp_index,
                            temp_rem );
   
    utran_tx_lim_vs_temp_offset_val =
    rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->utran_tx_lim_vs_temp_offset,
                            temp_index,
                            temp_rem );
   
    hdet_vs_temp_val = 
    rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_vs_temp,
                            temp_index,
                            temp_rem );
   
    hdet_mod_range[0] =  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_mod_range[0],
                                                               temp_index,
                                                               temp_rem );
   
    hdet_mod_range[1] =   rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_mod_range[1],
                                                                temp_index,
                                                                temp_rem );

    rfwcdma_core_txplim_temp_val_update(tx_lim_vs_temp_val, 
                                        utran_tx_lim_vs_temp_offset_val, 
                                        hdet_vs_temp_val,
                                        hdet_mod_base,
                                        hdet_mod_range);
  }/* if (rfwcdma_data_get_rf_state(rfm_tx_device) == RFWCDMA_STATE_TX) */

  rfwcdma_core_temp_comp_rf_voltage_update(temp_index, temp_rem);

}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function read thermistor to get the temperature and call
  a function to interpolate a new temperature compensation value
  for each temperature compensation NV table.

*/
void rfwcdma_core_temp_comp_update(rfm_device_enum_type device)
{
  uint8 temperature;
  uint8 temp_index;
  uint8 temp_rem;
  int32 xpt_delay_value = 0;
  rfcommon_fbrx_error_type fbrx_temp_comp_result;
  rfwcdma_mdsp_set_data_param_type update_et_delay_param = {0};
  uint16 *tx_therm_bins = NULL;
  /* Multilinearizer NV data retrieval */
  tx_band_cal_type *tx_multilin_nv_cal_ptr = NULL;

  /*Temp compensation should not happen during RF Calibration(FTM state)*/
  if ( (rfwcdma_core_temp_comp_value.temp_comp_enable_flag == TRUE) && ((!IS_FTM_IN_TEST_MODE())||(rfm_get_calibration_state() == FALSE)) )
  {
     /* Make sure mdsp has not started a conversion and if yes, do TDET and temp comp compensation  */
     if ( ( rfwcdma_mdsp_read_hdet_started_flag() == 0 ) || (IS_FTM_IN_TEST_MODE()&&(rfm_get_calibration_state() == TRUE)))
     {
        /* Setting Disable Flag from ARM to prevent MDSP doing HDET trigger*/
        rfwcdma_mdsp_set_arm_to_mdsp_hdet_disable_flag(TRUE);

        if ( ( rfwcdma_mdsp_read_hdet_started_flag() == 0 ) || (IS_FTM_IN_TEST_MODE()&&(rfm_get_calibration_state() == TRUE)))
        {
           /*--------------------------------------------------------------------------*/
           /* Do HDET temp comp before therm read. For Tuner HDET, an immediate 
           write is required so that registers are adjusted internally */
           /*--------------------------------------------------------------------------*/

           (void)rfdevice_hdet_wcdma_measure_temp_comp(device,
                                                       rfwcdma_data_get_curr_band(device),
                                                       RFDEVICE_EXECUTE_IMMEDIATE,
                                                       NULL);
           /* read thermistor */
           temperature = rfwcdma_core_temp_comp_read_therm(device);

           /* calculate temperature compensation table segment index */
           temp_index = temperature / RFWCDMA_CORE_TEMP_BIN_SIZ;

           /* calculate segment remainder */
           temp_rem   = temperature % RFWCDMA_CORE_TEMP_BIN_SIZ;

           MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH,"RF WCDMA Temp Comp temperature read:%d, temp_index:%d, temp_rem:%d", temperature, temp_index, temp_rem );

           rf_common_enter_critical_section(rfwcdma_core_temp_comp_get_lock_data());

           if ((rfwcdma_data_get_rf_state(device) != RFWCDMA_STATE_TX))
           {
             MSG( MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_core_temp_comp_update: temp comp skip since not in RxTx state" );
           }
           else
           {
             /* update temp comp value from NV */
             rfwcdma_core_temp_comp_calc_data(device, temp_index, temp_rem,
                                              temperature);

             rfwcdma_core_rxctl_update_rsb_coeff(temperature);

             if (rfwcdma_core_xpt_is_enabled(rfwcdma_data_get_curr_band(device)))
             {
               /* Update the XPT path delay value */
               xpt_delay_value = rfwcdma_core_xpt_path_delay_comp(device,
                                                                  rfwcdma_data_get_curr_band(device),
                                                                  rfwcdma_data_get_curr_chan(device, RFCOM_SINGLE_CARRIER),
                                                                  FALSE,
                                                                  0,
                                                                  rfwcdma_data_get_num_carriers(device));

               /* Update ET path delay to MDSP/DM*/
               update_et_delay_param.rflm_handle = rfwcdma_data_get_rflm_buf_idx(device);
               update_et_delay_param.set_data_cmd = RFWCDMA_MDSP_UPDATE_ET_DELAY;
               update_et_delay_param.data.gen_dw = (uint32)xpt_delay_value;

               rfwcdma_mdsp_async_set_data(&update_et_delay_param);

             }

             /* Query therm bins from tx temp comp NV */
             tx_therm_bins = rfnv_wcdma_tbl_ptr[rfwcdma_data_get_curr_band(device)]->lin_temp_comp_data.tx_therm_bins;
             /* Update fbrx temp comp with queried therm bins */  
             if (tx_therm_bins != NULL)
             {  
               /* For the current band get the pointer to complete linearizer band cal data */
               tx_multilin_nv_cal_ptr = rfwcdma_core_txlin_get_band_data(rfwcdma_data_get_curr_band(device));
               if (tx_multilin_nv_cal_ptr != NULL)
               { 
                            
                 fbrx_temp_comp_result = rfcommon_fbrx_mc_update_temp_comp(
                                    rfwcdma_data_get_rflm_buf_idx(device),
                                    temperature,
                                    rfwcdma_core_temp_comp_get_scaled_therm_read(tx_multilin_nv_cal_ptr->ref_lin_temp_adc ),
                                    tx_therm_bins,
                                    RFCOMMON_FBRX_NV_CONTAINER_C0);

                 if (fbrx_temp_comp_result != RFCOMMON_FBRX_SUCCESS)
                 {
                   MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed:%d", fbrx_temp_comp_result);
                 }
               }
               else
               {
                   MSG( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed: tx_multilin_nv_cal_ptr is NULL!");
               }
             }
             else
             {
                MSG( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed: Therm bins is NULL!");
             }
           }
           
           rf_common_leave_critical_section(rfwcdma_core_temp_comp_get_lock_data());
        }

        /* Setting Disable Flag from ARM to prevent MDSP doing HDET trigger*/
        rfwcdma_mdsp_set_arm_to_mdsp_hdet_disable_flag(FALSE);
     }

     else
     {
        MSG( MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_core_temp_comp_update:"
             "No Temp Comp or HDET Comp performed as HDET Read is active"
             "to avoid collision between HDET and Therm Read ");
     }
  }
  /* Do VBatt based Max Power Backoff */
  rfwcdma_core_txplim_do_vbatt_based_backoff();

} /* rfwcdma_core_temp_comp_update() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Temp Comp update Handler function
 
  @details
  This function will be called by RF APPS task dispatcher when temp comp
  command is posted to the command Q beloging to RF APPS task. This function
  will perform Linearizer  able update
*/
void 
rfwcdma_temp_comp_update_handler
(
  rfm_device_enum_type device 
)
{
  rf_time_type tempcomp_time;      /* prep_wakeup Time */
  rf_time_tick_type tempcomp_tick;      /* prep_wakeup Time */
  rfwcdma_temp_comp_data_type *tc_data;
  //m_device_enum_type device = RFM_DEVICE_0;
  rf_path_enum_type path =RF_PATH_0 ;
  uint8 temperature;
  uint8 temp_index;
  uint8 temp_rem;
  boolean proceed_with_update = FALSE;
  rfm_cb_event_enum_type tc_event = RFM_WCDMA_TEMP_COMP_PREP_IGNORED;
  rfwcdma_mdsp_get_data_param_type get_param;
  rfwcdma_mdsp_set_data_param_type set_param;
  boolean api_status = TRUE;
  boolean tx_on_status = FALSE;
  rfdevice_id_enum_type wtr_id;

  /* Start Profiling of API */
  tempcomp_tick = rf_time_get_tick();

  /* Throw F3 Message */
  RF_MSG_1( RF_HIGH, "rfwcdma_temp_comp_update_handler : START on device %d", device );

  //device = rfwcdma_mc_state.primary_rx_device;  

  /* Get Temp Comp Data */
  tc_data = rfwcdma_data_get_temp_comp(device);
  
  if ( tc_data == NULL )
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_temp_comp_update_handler: NULL Temp Comp Data - "
                       "Device %d", device );
    return;
  } /* if ( tc_data == NULL ) */

  /* Get TX_ON status */
  get_param.get_data_cmd = RFWCDMA_MDSP_GET_TX_ON_STATUS;
  get_param.rflm_handle = rfwcdma_data_get_rflm_buf_idx( device); //rfwcdma_mc_state.txlm_buf_idx;
  get_param.data.gen_b = 0;
  api_status = rfwcdma_mdsp_async_get_tx_data(&get_param);
  tx_on_status = (boolean)get_param.data.gen_b;


  /* DBG MSG */
  RF_MSG_5( RF_HIGH,
            "rfwcdma_temp_comp_update_handler: proceed_with_update %d tx_on_status %d tc_data->crit_section %d device %d rf_state %d ",
            proceed_with_update,
            tx_on_status,
            tc_data->crit_section,
            device,
            rfwcdma_data_get_rf_state(device)
          );

  if (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_TX)
  {

    /*Temp compensation should not happen during RF Calibration(FTM state)*/
    if ( (rfwcdma_core_temp_comp_value.temp_comp_enable_flag == TRUE) && 
         ((!IS_FTM_IN_TEST_MODE())||(rfm_get_calibration_state() == FALSE)) )
    {
      wtr_id = rfcommon_core_get_wtr_id_from_device(device, 
                                                      rfwcdma_data_get_curr_band(device), 
                                                      RFCOM_WCDMA_MODE);
      if((WTR4905 == wtr_id) || (WTR4905V100 == wtr_id) || 
         (WTR2955 == wtr_id) || (WTR2955V110== wtr_id) || 
         (WTR2965 == wtr_id))
      {
        /* Arm FED for THERM_ENABLE */
        set_param.rflm_handle = rfwcdma_data_get_rflm_buf_idx( device); //rfwcdma_mc_state.txlm_buf_idx;
        set_param.set_data_cmd = RFWCDMA_MDSP_SET_RFLM_THERM_ENABLE_STATE;
        set_param.data.gen_b = 1;
        api_status &= rfwcdma_mdsp_async_set_data(&set_param);

        /* No temp_comp operations to be done and no need to send response to L1 */
        proceed_with_update = FALSE;

        tc_event = RFM_WCDMA_TEMP_COMP_PREP_COMPLETE;

        /* Indicate that therm_enable will be done via FED */
        tc_data->therm_enable_triggered_by_fed = TRUE;

        RF_MSG_1( RF_HIGH, "rfwcdma_temp_comp_update_handler: Triggered FED for THERM_ENABLE wtr_id %d", wtr_id);;
      }
      else
      {
        if ( rflm_wcdma_try_lock_meter_trigger(rfwcdma_data_get_rflm_buf_idx(device)) == TRUE )
        {
           /* Do HDET temp comp before therm read. For Tuner HDET, an immediate 
           write is required so that registers are adjusted internally */
          rf_common_enter_critical_section(rfwcdma_get_temp_comp_lock_data());      
        
          (void)rfdevice_hdet_wcdma_measure_temp_comp(device,
                                                       rfwcdma_data_get_curr_band(device),
                                                       RFDEVICE_EXECUTE_IMMEDIATE,
                                                       NULL);
       
          rf_common_leave_critical_section(rfwcdma_get_temp_comp_lock_data());

          /* read thermistor */
          temperature = rfwcdma_core_temp_comp_read_therm(device);

          /* Store the temperature */
          tc_data->last_temperature = temperature;

          /* calculate temperature compensation table segment index */
          temp_index = temperature / RFWCDMA_CORE_TEMP_BIN_SIZ;

          /* calculate segment remainder */
          temp_rem   = temperature % RFWCDMA_CORE_TEMP_BIN_SIZ;

          RF_MSG_3( RF_HIGH, "rfwcdma_temp_comp_update_handler : read:%d, "
                    "temp_index:%d, temp_rem:%d", temperature, temp_index, 
                    temp_rem );

          /* update temp comp value from NV */
          rfwcdma_core_temp_comp_calc_data(device, temp_index, temp_rem,
                                           temperature);

          tc_event = RFM_WCDMA_TEMP_COMP_PREP_COMPLETE;
             
        } /*if ( rflm_wcdma_try_lock_meter_trigger(rfwcdma_data_get_rflm_buf_idx(device)) == TRUE )*/
        else
        {
          RF_MSG_1( RF_HIGH, "rfwcdma_temp_comp_update_handler : Skipping"
                    "TempComp Update : Power Meter Resource in Use - Handle %d",
                    rfwcdma_data_get_rflm_buf_idx(device) );
        } /* if ! ( rflm_wcdma_try_lock_meter_trigger(rfwcdma_mc_state.txlm_buf_idx) == TRUE ) */
        rflm_wcdma_unlock_meter_trigger(rfwcdma_data_get_rflm_buf_idx(device));
      }
    } /* if ! ( (rfwcdma_core_temp_comp_value.temp_comp_enable_flag == TRUE) && 
         ((!IS_FTM_IN_TEST_MODE())||(rfm_get_calibration_state() == FALSE)) ) */
  } /* if ((rfwcdma_get_rf_state(RFM_DEVICE_0) == RFWCDMA_MC_STATE_RX) ||
      (rfwcdma_get_rf_state(RFM_DEVICE_0) == RFWCDMA_MC_STATE_RXTX)) */
  else
  {
    RF_MSG_1( RF_HIGH, "rfwcdma_temp_comp_update_handler: Skipping "
              "RadioState %d", rfwcdma_data_get_rf_state(device));
  } /* if ! ((rfwcdma_get_rf_state(RFM_DEVICE_0) == RFWCDMA_MC_STATE_RX) ||
      (rfwcdma_get_rf_state(RFM_DEVICE_0) == RFWCDMA_MC_STATE_RXTX)) */

  tc_data->tc_state = TEMP_COMP_PREP_COMPLETED;
  tc_data->prep_complete_time = rf_time_get_tick();

  /* Execute Callback */
  if ( tc_data->callback != NULL )
  {
    tc_data->callback( tc_event, 
                       tc_data->userdata );
  } /* if ( callback != NULL ) */

  /* get time spent in API */
  tempcomp_time = rf_time_get_elapsed( tempcomp_tick, RF_USEC );

  RF_MSG_1( RF_HIGH, "rfwcdma_temp_comp_update_handler : TimeTaken %d us",
            tempcomp_time );

} /* rfwcdma_temp_comp_update_handler */

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function read thermistor to get the temperature and returns
  the Tx linearizer temp comp offset 
*/
void rfwcdma_core_get_txlin_temp_comp(rfm_device_enum_type device,
                                      int16 pin_comp_val[],
                                      int16 pout_comp_val[])
{
  uint8 temperature;
  rfcommon_fbrx_error_type fbrx_temp_comp_result;
  uint16 *tx_therm_bins = NULL;
  /* Multilinearizer NV data retrieval */
  tx_band_cal_type *tx_multilin_nv_cal_ptr = NULL;

  /* read thermistor */
  temperature = rfwcdma_core_temp_comp_read_therm(device);

  /* Obtain the linearizer Pin and Pout vs temp and freq compensation values */
  rfwcdma_core_temp_comp_vs_freq_get_comp(temperature,
                                          pin_comp_val,
                                          pout_comp_val);

  /* Query therm bins from tx temp comp NV */
  tx_therm_bins = rfnv_wcdma_tbl_ptr[rfwcdma_data_get_curr_band(device)]->lin_temp_comp_data.tx_therm_bins;
  if (tx_therm_bins != NULL)
  {
    /* For the current band get the pointer to complete linearizer band cal data */
    tx_multilin_nv_cal_ptr = rfwcdma_core_txlin_get_band_data(rfwcdma_data_get_curr_band(device));
    if (tx_multilin_nv_cal_ptr != NULL)
    { 
    /* Update fbrx temp comp with queried therm bins */ 
      fbrx_temp_comp_result = rfcommon_fbrx_mc_update_temp_comp(
                         rfwcdma_data_get_rflm_buf_idx(device),
                         temperature,
                         rfwcdma_core_temp_comp_get_scaled_therm_read(tx_multilin_nv_cal_ptr->ref_lin_temp_adc ),
                         tx_therm_bins,
                         RFCOMMON_FBRX_NV_CONTAINER_C0 );

      if (fbrx_temp_comp_result != RFCOMMON_FBRX_SUCCESS)
      {
        MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed:%d", fbrx_temp_comp_result);
      }
    }
    else
    {
        MSG( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed: tx_multilin_nv_cal_ptr is NULL!");
    }
  }
  else
  {
     MSG( MSG_SSID_RF, MSG_LEGACY_ERROR,"FBRx Temp Comp failed: Therm bins is NULL!");
  }
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function gets and adds the comp_vs_freq_vs_temp values to the input
  temp_comp array.


  @param txlin_temp_comp[] array for adding comp_vs_freq_vs_temp vals into
  @param temp_index Temperature table index
  @param temp_rem Remainder value

*/
void rfwcdma_core_add_comp_vs_freq_vs_temp(int16 txlin_temp_comp[], uint8 temp_index, uint8 temp_rem) 
{
  uint8 i, j;
  int16 freq_comp_indicator, freq_interp;                      
  uint8 freq_index;
  int16 comp_vs_freq_vs_temp[NV_TEMP_TABLE_SIZ];
  int16 comp_vs_freq_vs_temp_n, comp_vs_freq_vs_temp_n_plus_1;
  
  rfm_device_enum_type rfm_tx_device = RFM_MAX_WAN_DEVICES;

  tx_band_cal_type *tx_band_cal_data_ptr = NULL;
    /*Update the rfm_device for tx*/
  rfm_tx_device = rfwcdma_data_get_tx_device();
  rfcom_wcdma_band_type band = rfwcdma_data_get_curr_band(rfm_tx_device);

  /* This array is used for finding the freq comp bin and interp factor*/
  const int16 rfwcdma_core_freq_comp_indicator_array[] =
  {
    0*RFWDCMA_FREQ_BIN_RESOLUTION,  1*RFWDCMA_FREQ_BIN_RESOLUTION,
    2*RFWDCMA_FREQ_BIN_RESOLUTION,  3*RFWDCMA_FREQ_BIN_RESOLUTION,
    4*RFWDCMA_FREQ_BIN_RESOLUTION,  5*RFWDCMA_FREQ_BIN_RESOLUTION,
    6*RFWDCMA_FREQ_BIN_RESOLUTION,  7*RFWDCMA_FREQ_BIN_RESOLUTION,
    8*RFWDCMA_FREQ_BIN_RESOLUTION,  9*RFWDCMA_FREQ_BIN_RESOLUTION,
    10*RFWDCMA_FREQ_BIN_RESOLUTION, 11*RFWDCMA_FREQ_BIN_RESOLUTION,
    12*RFWDCMA_FREQ_BIN_RESOLUTION, 13*RFWDCMA_FREQ_BIN_RESOLUTION,
    14*RFWDCMA_FREQ_BIN_RESOLUTION, 15*RFWDCMA_FREQ_BIN_RESOLUTION
  };

  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_calc_data: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_comp_calc_data: Not a valid band %d",
               band);
    return;
  }

  /* Check if Tx band is valid */
  if(band == RFCOM_BAND_INVALID)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,"Tx Band not initialed yet.", 0);
    return;
  }

  /* Get current band linearizer cal data */
  tx_band_cal_data_ptr = rfwcdma_core_txlin_get_band_data(band);
  if(tx_band_cal_data_ptr == NULL)
  {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid WCDMA band data", 0);
      return;
  }

  /* Check if there are multiple linearizers */
  if( ( tx_band_cal_data_ptr->tx_cal_chan_size <= 0 ) ||  ( tx_band_cal_data_ptr->tx_cal_chan_size > RFCOM_TXLIN_NUM_CHANNELS ) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid Cal Chan Size!", 0);
    return;
  }

  /* Get freq_comp bin and multiplier */
  freq_comp_indicator = rf_lininterp_u32_s16(tx_band_cal_data_ptr->tx_cal_freq,
                                             rfwcdma_core_freq_comp_indicator_array,
                                             tx_band_cal_data_ptr->tx_cal_chan_size,
                                             rfwcdma_core_get_freq_from_ul_arfcn(rfwcdma_data_get_curr_chan(rfm_tx_device,RFCOM_SINGLE_CARRIER))
                                             );

  /* Calculate freq_comp bin index and interp factor */
  freq_interp = freq_comp_indicator % RFWDCMA_FREQ_BIN_RESOLUTION;
  freq_index  = (uint8)((freq_comp_indicator-freq_interp)/RFWDCMA_FREQ_BIN_RESOLUTION);

  for(i=0; i<RFCOM_TXLIN_NUM_LINEARIZERS; i++)
  {
    /* calculate array of deltas of comp_vs_freq_vs_temp with interp */
    for(j = 0; j < NV_TEMP_TABLE_SIZ; j++)
    {
      /* Get the temp_comp arrays for the freq bins */
      if((RFCOM_DUAL_CARRIER == rfwcdma_data_get_num_carriers(rfm_tx_device)) && 
          (TRUE == rfnv_wcdma_tbl_ptr[band]->dc_lin_temp_comp_data.has_pin_pout_temp_comp_nv))
      {
        comp_vs_freq_vs_temp_n = (int16)rfnv_wcdma_tbl_ptr[band]->dc_pa_static.tx_lin_pin[j][i][freq_index];
        comp_vs_freq_vs_temp_n_plus_1 = (int16)rfnv_wcdma_tbl_ptr[band]->dc_pa_static.tx_lin_pin[j][i][freq_index+1];
      }
      else
      {
        comp_vs_freq_vs_temp_n = (int16)rfnv_wcdma_tbl_ptr[band]->pa_static.tx_lin_pin[j][i][freq_index];
        comp_vs_freq_vs_temp_n_plus_1 = (int16)rfnv_wcdma_tbl_ptr[band]->pa_static.tx_lin_pin[j][i][freq_index+1];
      }

      /* Linear interpolation of two temp_comp arrays between two freq_bins */
      comp_vs_freq_vs_temp[j] = (int16)(comp_vs_freq_vs_temp_n +        \
                                        ((comp_vs_freq_vs_temp_n_plus_1 - comp_vs_freq_vs_temp_n)*freq_interp)/RFWDCMA_FREQ_BIN_RESOLUTION);
    }

    /* Add delta to temp_comp */
    txlin_temp_comp[i] += rfwcdma_core_temp_comp_calc_comp_val( comp_vs_freq_vs_temp, 
                                                                temp_index,
                                                                temp_rem );
  }
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function initialize the Tx max power NV limit (lim vs temp)
*/
void rfwcdma_core_temp_txplim_init(rfcom_wcdma_band_type band)
{
  uint8 temp_index, temp_rem;
  int16  tx_lim_vs_temp_val, utran_tx_lim_vs_temp_offset_val, hdet_vs_temp_val, hdet_mod_range[2];

  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_txplim_init: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_temp_txplim_init: Not a valid band %d",
               band);
    return;
  }

  /* calculate temperature compensation table segment index */
  temp_index = rfwcdma_core_temp_comp_value.last_scaled_temp_value / RFWCDMA_CORE_TEMP_BIN_SIZ;

  /* calculate segment remainder */
  temp_rem   = rfwcdma_core_temp_comp_value.last_scaled_temp_value % RFWCDMA_CORE_TEMP_BIN_SIZ;

  /* Update NV limit*/
  tx_lim_vs_temp_val =
  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->tx_lim_vs_temp,
                          temp_index,
                          temp_rem );

  utran_tx_lim_vs_temp_offset_val =
  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->utran_tx_lim_vs_temp_offset,
                          temp_index,
                          temp_rem );

  hdet_vs_temp_val = 
  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_vs_temp,
                          temp_index,
                          temp_rem );

  hdet_mod_range[0] =  rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_mod_range[0],
                                                             temp_index,
                                                             temp_rem );

  hdet_mod_range[1] =   rfwcdma_core_temp_comp_calc_comp_val( rfnv_wcdma_tbl_ptr[band]->hdet_mod_range[1],
                                                              temp_index,
                                                              temp_rem );

  /* Since during Tx init, static temp comp will be applied on HDET PA staet, the base for HDET mod range should be 0 */
  rfwcdma_core_txplim_temp_val_update(tx_lim_vs_temp_val, 
                                      utran_tx_lim_vs_temp_offset_val, 
                                      hdet_vs_temp_val,
                                      0,
                                      hdet_mod_range);
}

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function sets the rfwcdma_core_temp_comp_is_enabled flag.
  Intended for use by off-target to skip temp comp.
*/
void rfwcdma_core_temp_comp_set_enable(boolean enable)
{
  rfwcdma_core_temp_comp_is_enabled = enable;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Updates the current temp comp algorithm to be used (refer to
  rfcommon_core_txlin_get_temp_comp_algo()) and updates it to the WCDMA Txlin 
  data structure

  @param band
  Band for which the temp comp algorithm must be updated

  @return
  TRUE if the API is successful, else FALSE

*/
boolean
rfwcdma_core_temp_comp_update_algo
(
  rfcom_wcdma_band_type band
)
{
  /* Pointer to the WCDMA Txlin data */
  rfwcdma_core_txlin_record_type *curr_txlin_data;
  tx_band_cal_type *tx_band_cal_data_ptr = NULL;
  rfnv_wcdma_nv_tbl_type * nv_tbl;

  /* Get current Tx linearizer data */
  curr_txlin_data = rfwcdma_core_get_current_txlin_data();

  if ( curr_txlin_data == NULL )
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_core_temp_comp_update_algo: "
                                        "Null pointer access to TxLin data" );
    return FALSE; 
  }

  /* Check for calibration state */
  if ( rfm_get_calibration_state() == TRUE )
  {
    curr_txlin_data->temp_comp_algo = RFCOM_TXLIN_TEMP_COMP_ALGO_INVALID;

    return TRUE;
  }

  /* Get NV table for EPT/DPD config params */
  nv_tbl = rfwcdma_core_init_nv_table( band );

  if(nv_tbl == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_core_temp_comp_update_algo: "
                                        "Null pointer access to NV data" );
    return FALSE; 
  }

  /*For the current band get the pointer to complete linearizer band cal data */
  tx_band_cal_data_ptr = rfwcdma_core_txlin_get_band_data( band );

  /* NULL check */
  if ( tx_band_cal_data_ptr == NULL )
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_core_temp_comp_update_algo: "
                                        "Null pointer access to cal data" );
    return FALSE;
  }

  /* Obtain the current temp comp algorithm and store it to device 
  status */
  curr_txlin_data->temp_comp_algo = 
    rfcommon_core_txlin_get_temp_comp_algo( 
       (tx_multi_lin_rev_type)tx_band_cal_data_ptr->nv_rev_type,
       (rfcom_txlin_temp_comp_nv_data_type*)&nv_tbl->lin_temp_comp_data );

  MSG_2( MSG_SSID_RF, MSG_LEGACY_MED, "rfwcdma_core_temp_comp_update_algo: "
                                      "Updated algorithm for band %d to %d", 
         band, curr_txlin_data->temp_comp_algo );

  return TRUE;

}/* rfwcdma_core_temp_comp_update_algo */

uint16* rfwcdma_core_temp_comp_get_therm_bin_array(void)
{
  return therm_bin_array;
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function initialize one time settings
 
  @details
  This function initialize one time settings
*/
void rfwcdma_core_temp_comp_data_init(void)
{
  int i =0;
  rfwcdma_temp_comp_data_type *tc_data;
  rfm_device_enum_type dev_idx;
  static boolean do_once = TRUE;
  
  /* Make Therm Bin array */
  for (i = 0; i < NV_TEMP_TABLE_SIZ; i++ )
  {
    therm_bin_array[i] = (i*RFWCDMA_CORE_TEMP_BIN_SIZ);
  }
  /* The final element needs to be the Max value supported */
  therm_bin_array[NV_TEMP_TABLE_SIZ-1] = RFWCDMA_CORE_THERM_MAX;
  if (do_once)
  {
    rf_common_init_critical_section( rfwcdma_get_temp_comp_lock_data());
    do_once = FALSE;
  }
    
  for ( dev_idx = RFM_DEVICE_0; 
       dev_idx < RFM_MAX_WAN_DEVICES; 
       dev_idx++ )
  {
    tc_data = rfwcdma_data_get_temp_comp(dev_idx);
    if ( tc_data == NULL )
    {
       RF_MSG_1( RF_ERROR, "rfwcdma_core_temp_comp_data_init: NULL Temp Comp Data - "
                        "Device %d", dev_idx);
       return;
    } /* if ( tc_data == NULL ) */
    if (rfwcdma_get_temp_comp_lock_data() != NULL)
    {
      tc_data->crit_section = rfwcdma_get_temp_comp_lock_data();
      temp_comp_per_dev ++;

    } /* if ( tc_data != NULL ) */
  }
 }

/*------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Override TempComp to disable mode
 
  @details
  This function will be used to override the TempComp mode of operation to disable.
 
  @return
  Flag indication status of TempComp override
*/
boolean
rfwcdma_tempcomp_override_disable
(
   rfm_device_enum_type device
)
{
  rfwcdma_temp_comp_data_type *tc_data;

  /* Get tempcomp data */
  tc_data = rfwcdma_data_get_temp_comp(device);

  if ( tc_data == NULL )
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_tempcomp_override_disable: NULL Temp Comp Data - "
                        "Device %d", device );
    return FALSE;
  } /* if ( tc_data == NULL ) */

  tc_data->override_disable_mode = TRUE;

  RF_MSG_1( RF_HIGH, "rfwcdma_tempcomp_override_disable: Overriden to disable - "
                      "Device %d", device );

  return TRUE;

} /* rfwcdma_tempcomp_override_disable */

/*------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Override TempComp to enable mode
 
  @details
  This function will be used to override the TempComp mode of operation to enable.
 
  @return
  Flag indication status of TempComp override
*/
boolean
rfwcdma_tempcomp_override_enable
(
  rfm_device_enum_type device
)
{
  rfwcdma_temp_comp_data_type *tc_data;

  /* Get tempcomp data */
  tc_data = rfwcdma_data_get_temp_comp(device);

  if ( tc_data == NULL )
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_tempcomp_override_enable: NULL Temp Comp Data - "
                        "Device %d", device );
    return FALSE;
  } /* if ( tc_data == NULL ) */

  tc_data->override_disable_mode = FALSE;

  RF_MSG_1( RF_HIGH, "rfwcdma_tempcomp_override_enable: Overriden to enable - "
                      "Device %d", device );

  return TRUE;

} /* rfwcdma_tempcomp_override_enable */
/*----------------------------------------------------------------------------*/

/*!
  @brief
  Get Lock Data
 
  @details
  This is an accessor function which will return the pointer to the RF
  Lock Data.
 
  @return
  Pointer to RFA Lock Data
*/
rf_lock_data_type*
rfwcdma_core_temp_comp_get_lock_data(void)
{
  return &rfwcdma_core_temp_comp_lock_data;
} /* rfwcdma_core_temp_comp_get_lock_data */

/*------------------------------------------------------------------------------------------------*/
/*!
  @brief
  This function reads the THERM value from WTR
 
  @details
  This function reads the THERM valure from WTR.
  THERM enable and ADC conversion is done in RFLM/FED context and MSG router cmd is sent to trigger
  this API.
  Note that this API can be called only when TxAGC is running.
 
*/
void
rfwcdma_core_temp_comp_read_therm_handler
(
  void* cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data 
)
{
   uint8 temperature;                                                    /* Scaled therm value     */
   uint8 temp_index;                                                     /* therm bin index        */
   uint8 temp_rem;                                                       /* temp remainder         */
   rfm_cb_event_enum_type tc_event = RFM_WCDMA_TEMP_COMP_PREP_IGNORED;   /* event status           */
   rflm_wcdma_do_therm_read_cmd* therm_cmd_payload = NULL;               /* therm cmd payload      */
   rfwcdma_temp_comp_data_type *tc_data = NULL;                          /* temp_comp data         */
   boolean tc_data_error_flag = FALSE;                                   /* TC data error flag     */
   boolean handle_data_error_flag = FALSE;                               /* Handle data error flag */
   rfm_device_enum_type device = RFM_DEVICE_0;                           /* device of interest     */

   rflm_wcdma_dm_buf_type *wcdma_dm_data_ptr = NULL;
   boolean buffer_valid = FALSE;

   /* Get the payload from cmd_ptr */
   therm_cmd_payload = (rflm_wcdma_do_therm_read_cmd*)((rf_cmd_type*)cmd_ptr)->payload;

   device = rfwcdma_data_get_tx_device();

    if(device >= RFM_MAX_WAN_DEVICES)
   {
     RF_MSG_1(RF_ERROR,
              "rfwcdma_core_temp_comp_read_therm_handler: Invalid Tx device %d, returning from here",device);
     return;
   }

   /* @todo: remove hardcoding device */
   /* Get temp_comp data */
   tc_data = rfwcdma_data_get_temp_comp(device);

   /* Handler data check */
   if (therm_cmd_payload == NULL)
   {
      RF_MSG(RF_ERROR,
             "rfwcdma_core_temp_comp_read_therm_handler: NULL payload!");
      handle_data_error_flag = TRUE;
   }

   /* TC data Check */
   if (tc_data == NULL)
   {
      RF_MSG(RF_ERROR,
             "rfwcdma_core_temp_comp_read_therm_handler: NULL TC Data! CB will not be called!");
      tc_data_error_flag = TRUE;
   }

   /* Print timing info */
   if (!handle_data_error_flag)
   {
      RF_MSG_6(RF_HIGH,
               "[WCDMA_THERM][Timing]: setup_complete %d, tx_slot_boundary %d, stable_tx start %d, stable_tx stop %d, therm_enable %d, cb %d",
               therm_cmd_payload->therm_read_setup_completed,
               therm_cmd_payload->slot_boundary_time,
               therm_cmd_payload->stable_tx_start_time,
               therm_cmd_payload->stable_tx_stop_time,
               therm_cmd_payload->event_action_time,
               therm_cmd_payload->cb_stmr_time
              );
   }

        
   buffer_valid = rflm_dm_get_handle_tech_data((rflm_dm_handle_id_t)rfwcdma_data_get_rflm_buf_idx(device), (void *)&wcdma_dm_data_ptr, RFLM_DM_TECH_DATA_GET_PTR);
  
   /* Temp comp operations */
   if ((!tc_data_error_flag) && (!handle_data_error_flag))
   {
      /* read therm */
      temperature = rfwcdma_core_temp_comp_read_therm(device);

     if ((buffer_valid  == TRUE) && (wcdma_dm_data_ptr != NULL))
     {
       wcdma_dm_data_ptr->tx_data.tx_status.temp_comp_in_progress = FALSE;
     }

     /* Store the temperature */
     tc_data->last_temperature = temperature;

     /* calculate temperature compensation table segment index */
     temp_index = temperature / RFWCDMA_CORE_TEMP_BIN_SIZ;

     /* calculate segment remainder */
     temp_rem   = temperature % RFWCDMA_CORE_TEMP_BIN_SIZ;

     /* DBG MSG */
     RF_MSG_3( RF_HIGH,
               "[WCDMA_THERM][Data]: scaled_therm %d, temp_index:%d, temp_rem:%d",
               temperature, temp_index, temp_rem );

     /* update temp comp value from NV */
     rfwcdma_core_temp_comp_calc_data(device, temp_index, temp_rem, temperature);

     /* Set tc_event to complete */
     tc_event = RFM_WCDMA_TEMP_COMP_PREP_COMPLETE;
   }

   else
   {
     if ((buffer_valid  == TRUE) && (wcdma_dm_data_ptr != NULL))
     {
       wcdma_dm_data_ptr->tx_data.tx_status.temp_comp_in_progress = FALSE;
     }
   }

   /* Send response to L1 */
   if (!tc_data_error_flag)
   {
      /* Set TC state to complete */
      tc_data->tc_state = TEMP_COMP_PREP_COMPLETED;

      /* Execute Callback */
      if ( tc_data->callback != NULL )
      {
         tc_data->callback( tc_event, tc_data->userdata );
      }
   }

   rflm_wcdma_unlock_meter_trigger((rflm_dm_handle_id_t)rfwcdma_data_get_rflm_buf_idx(device));
} /* rfwcdma_core_temp_comp_read_therm_handler */


