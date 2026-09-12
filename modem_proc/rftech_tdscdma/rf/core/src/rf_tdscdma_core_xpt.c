/*! 
 @file rf_tdscdma_core_xpt.c 
 
 @brief 
  This file has a collection of WCDMA XPT utility functions.
 
*/ 
  
/*==============================================================================

  Copyright (c) 2013 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Technologies Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/
  
/*==============================================================================  
                           EDIT HISTORY FOR FILE 
  
  This section contains comments describing changes made to this file. 
  Notice that changes are listed in reverse chronological order. 

  $Header: //components/rel/rftech_tdscdma.mpss/3.11/rf/core/src/rf_tdscdma_core_xpt.c#1 $
  
When       who     what, where, why 
------------------------------------------------------------------------------- 
08/21/13   jhe     initial version
  
============================================================================*/

#include "rf_tdscdma_core_xpt.h"
#include "rfnv_tdscdma.h"
#include "rfcommon_core_xpt.h"
#include "rf_tdscdma_core_util.h"
#include "rf_tdscdma_core_temp_comp.h"
#include "rfcommon_core_xpt.h"
#include "rfcommon_core_txlin_types.h"
#include "rf_tdscdma_core_txlin.h"
#include "rfm.h"




/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get XPT Config Param from NV

  @details
  This function will query the XPT Config Param from NV

  @param band

  @param xpt_cfg_index
 
  @return
  Value of the requested XPT Config param
*/
uint32 rf_tdscdma_core_xpt_get_cfg_param
(
  rfcom_tdscdma_band_type band,
  uint8 xpt_cfg_index
)
{
  rfnv_tdscdma_tx_tbl_type *tx_nv_tbl = NULL;
  uint32 xpt_cfg_val = 0;

  tx_nv_tbl = rf_tdscdma_core_util_init_tx_nv_table(band);

  if ( tx_nv_tbl != NULL )
  {
    /* Getting the XPT config param value corresponding to the index */
    xpt_cfg_val = tx_nv_tbl->ept_dpd_cfg_params[xpt_cfg_index];
  }
  else
  {
    MSG_ERROR( "rf_tdscdma_core_xpt_get_cfg_param: Failed to get valid static NV pointer for band %d", band, 0, 0 );
    xpt_cfg_val = 0;
  }
 
  return xpt_cfg_val;
} /* rf_tdscdma_core_xpt_get_cfg_param */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query if XPT is enabled
 
  @details
  This function will check the XPT NV to determine if XPT is enabled or not
 
  @param band
  TDSCDMA band for which XPT enable is to be checked
 
  @return
  Flag indicating if XPT is enabled or not.
*/
boolean rf_tdscdma_core_xpt_is_enabled
(
  rfcom_tdscdma_band_type band
)
{
  boolean is_xpt_enabled = FALSE;
  xpt_cfg_type nv_xpt_mode = 0;

  nv_xpt_mode = (xpt_cfg_type) rf_tdscdma_core_xpt_get_cfg_param(band, XPT_CFG);

  if ( ( nv_xpt_mode == EPT_CFG ) ||
       ( nv_xpt_mode == EPT_ET_CFG ) ||
       ( nv_xpt_mode == ET_CFG ) )
  {
    is_xpt_enabled = TRUE;
  }
  else
  {
    is_xpt_enabled = FALSE;
  }

  return is_xpt_enabled;

} /* rf_tdscdma_core_xpt_is_enabled */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query if ET is enabled
 
  @details
  This function will check the XPT NV to determine if ET is enabled or not
 
  @param band
  TDSCDMA band for which ET enable is to be checked
 
  @return
  Flag indicating if ET is enabled or not.
*/
boolean rf_tdscdma_core_xpt_is_et_enabled
(
  rfcom_tdscdma_band_type band
)
{
  boolean is_et_enabled = FALSE;
  xpt_cfg_type nv_xpt_mode = 0;

  nv_xpt_mode = (xpt_cfg_type) rf_tdscdma_core_xpt_get_cfg_param(band, XPT_CFG);

  if ( ( nv_xpt_mode == EPT_ET_CFG ) ||
       ( nv_xpt_mode == ET_CFG ) )
  {
    is_et_enabled = TRUE;
  }
  else
  {
    is_et_enabled = FALSE;
  }

  return is_et_enabled;

} /* rf_tdscdma_core_xpt_is_et_enabled */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Compute the XPT path delay value.
 
  @details
  This function will return the xpt path delay value.
 
  @param band
  TDSCDMA operational band.

  @param rf_chan_tx
  TDSCDMA Tx RF channel.
 
  @param force_delay_temp_comp
  Flag to force apply delay temp comp. 
 
  @param therm_scaled_value
  Current scaled therm value. 
 
  @return
  XPT path delay value.
*/
int32 rf_tdscdma_core_xpt_path_delay_comp
(
  rfcom_tdscdma_band_type band,
  uint16 rf_chan_tx,
  boolean force_delay_temp_comp,
  uint16 therm_scaled_value
)
{
  int8 i;
  int32 xpt_path_delay_val = 0;
  int16 delay_comp_x10ns = 0;
  rf_tdscdma_nv_xpt_path_delay_type *xpt_delay_tbl = NULL;
  rfnv_tx_delay_offsets_type *xpt_delay_offset_tbl = NULL;
  rfnv_tdscdma_tx_tbl_type *nv_tbl = NULL;
  boolean delay_calibrated = FALSE;
  uint16 delay_calibration_temp_adc = 0;
  uint16 delay_calibration_temp_scaled = 0;
  int16 delay_offset_computed = 0;
  uint8 num_of_channels_enabled_for_delay = 0;
  uint16 therm_bin_array[NV_TEMP_TABLE_SIZ]={0,36,72,108,144,180,216,255};
  tx_band_cal_type* txlin_data = NULL;
  boolean delay_vs_temp_comp_status = FALSE;
  uint32 upper_bound_chan;
  rf_tdscdma_core_temp_comp_value_type therm_data;

  therm_data = rf_tdscdma_core_temp_comp_get_therm_data();

  nv_tbl = rf_tdscdma_core_util_init_tx_nv_table(band);

  if(nv_tbl == NULL)
  {
    RF_MSG_1 ( RF_ERROR, "rf_tdscdma_core_xpt_path_delay_comp: Failed to get valid static "
               "NV pointer for band %d", band);
    return 0;
  }

#if 0
  rfdevice_wcdma_tx_get_et_delay_compensation(device,
                                              band,
                                              &delay_comp_x10ns);
#endif

  /* Getting the XPT path 16channels delay info */
  xpt_delay_tbl = &(nv_tbl->xpt_path_delay);

  xpt_delay_offset_tbl = &(nv_tbl->tx_delay_offsets);
  num_of_channels_enabled_for_delay = xpt_delay_tbl->num_of_active_entries;

  /* Return the ET path delay value if the 16channels delay info not available */
  if (num_of_channels_enabled_for_delay == 0)
  {
    //xpt_path_delay_val = nv_tbl->et_delay_value + delay_comp_x10ns;
    xpt_path_delay_val = 0;
  }
  else
  {
    /* Find the delay value according to channel info */
    for(i=0; i<num_of_channels_enabled_for_delay; i++)
    {
      upper_bound_chan = xpt_delay_tbl->upper_bound_tx_chan[i];

      if(rf_chan_tx <= upper_bound_chan)
      {
        xpt_path_delay_val = (int32)(xpt_delay_tbl->delay_value[i] + delay_comp_x10ns);
        break;
      }
    }
  }

  /*Delay Temp comp should not happen during FTM Calibration*/
  if((force_delay_temp_comp == TRUE) ||
     ((xpt_delay_offset_tbl->channels_enabled != 0) && 
      ((!IS_FTM_IN_TEST_MODE())||(rfm_get_calibration_state() == FALSE))))
  {
    /* Find out if Delay NV was chared or calibrated 
       MSB bit will be turned ON if delay NV is calibrated, else NOT */
    if(num_of_channels_enabled_for_delay & 0x80)
    {
      txlin_data = rf_tdscdma_core_txlin_get_band_data(band);
      if (txlin_data != NULL)
      {
        delay_calibrated = TRUE;
        delay_calibration_temp_adc = txlin_data->ref_lin_temp_adc;
        delay_calibration_temp_scaled = rf_tdscdma_core_temp_comp_scale_therm(delay_calibration_temp_adc,FALSE);
        
        /* Obtain the valid channels for delay  by masking out the MSB bit */
        num_of_channels_enabled_for_delay = (num_of_channels_enabled_for_delay & 0x7F);
      }
      else
      {
        delay_calibrated = FALSE;
        RF_MSG( RF_ERROR,"Null pointer access!");
      }
    }
    else
    {
      delay_calibrated = FALSE;
    }

    /* Find the delay value offset from delay_vs_temp_vs_freq */

    /* There is a NV related therm_bin, but don't use it here, as it might not be
       available when this api is called in ftm mode */
    if (force_delay_temp_comp == TRUE)
    {
      delay_vs_temp_comp_status = 
        rf_common_xpt_delay_offset_computation(xpt_delay_offset_tbl,
                                               (const uint16*) therm_bin_array,
                                               therm_scaled_value,
                                               delay_calibration_temp_scaled,
                                               rf_tdscdma_core_util_get_freq_from_chan_common_api(rf_chan_tx),
                                               delay_calibrated,
                                               &delay_offset_computed,
                                               (rfcommon_core_convert_freq_handler_type)(&rf_tdscdma_core_util_get_freq_from_chan_common_api));
    }
    else
    {
      delay_vs_temp_comp_status = 
        rf_common_xpt_delay_offset_computation(xpt_delay_offset_tbl,
                                               (const uint16*) therm_bin_array,
                                               therm_data.last_scaled_temp_value,
                                               delay_calibration_temp_scaled,
                                               rf_tdscdma_core_util_get_freq_from_chan_common_api(rf_chan_tx),
                                               delay_calibrated,
                                               &delay_offset_computed,
                                               (rfcommon_core_convert_freq_handler_type)(&rf_tdscdma_core_util_get_freq_from_chan_common_api));
    }

    if(delay_vs_temp_comp_status == FALSE)
    {
      delay_offset_computed = 0;
    }

    xpt_path_delay_val += delay_offset_computed;

    RF_MSG_5(RF_HIGH, "XPT delay temp comp: %d cal temp scaled: %d current temp scaled:" 
                      "%d delay_calibrated: %d forced temp comp: %d",
             delay_offset_computed,
             delay_calibration_temp_scaled,
             therm_data.last_scaled_temp_value,
             delay_calibrated,
             force_delay_temp_comp);
  }

  RF_MSG_4(RF_HIGH, "XPT delay value:%d band:%d channel:%d delay comp: %d",
           xpt_path_delay_val,
           band,
           rf_chan_tx,
           delay_comp_x10ns);

  return xpt_path_delay_val;
} /* rf_tdscdma_core_xpt_path_delay_comp */
