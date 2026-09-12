/*! 
  @file
  rf_cdma_ept.c
 
  @brief
  This file defines the CDMA interfaces to be used for Envelope Power Tracking. 
 
  @addtogroup RF_CDMA_XPT
  @{
*/

/*==============================================================================

  Copyright (c) 2013 -  Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc..

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_cdma.mpss/3.11/common/rf/src/rf_cdma_ept.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
11/26/14   wwl     Clean up tx resource manager code
11/12/14   spa     Change tx_nv_path to be per band per logical device
10/08/14   pk      Fix for RevB ACPR Performance issue (avoid DPD WTR setting)
01/08/14   cd      Remove deallocation of TxLM buffer for ET TxLM handle
12/26/13   sar/hdz Remove et_txlm_hanlde from Bolt c2k code, and use one single 
                   TxLm handle allocation.
03/04/14   zhw     KW error fix
03/03/14   zhw     Fix incorrect ET Path Delay vs Freq search algorithm
08/26/13   shb     Updates for new C++ transceiver device interface
08/08/13   Saul    XPT. KW Fixes.
08/01/13   Saul    XPT. ET delay vs temp FTM char support
07/22/13   APU     Removed et_path_delay_legacy from XPT NV structure.
07/19/13   Saul    XPT. ET delay vs freq vs temp support.
07/17/13   jmf     [xPT] New Delay Table separate from DPD data module
06/27/13   APU     ET changes for gettong path delay vs freq.
06/21/13   jmf     [xPT] ET delay implementation for Calv4
05/28/13   JJ      Clean un-necessary cdma sys_band to rf_band_type conversion
05/13/13   Saul    XPT. Support EPT_ET_CFG.
03/26/13   aro     Renamed interface to indicate xpt_resource
03/26/13   aro     MC Callflow illustration
03/25/13   Saul    CDMA. Abstract TxLM XPT mode used in MC in HAL layer.
03/05/13   aro     Enhanced XPT override command and migrated to FTM common
                   dispatch
03/01/13   Saul    XPT cfg blocking for first response during cal and second
                   response when not in cal.
03/01/13   aro     Temporary Fix for Tx Sleep stuck in Cal mode
02/27/13   aro     Mainlined debug flag for Tx resource manager
02/27/13   aro     Interface to determine XPT mode of operation based on MSM
                   and NV
02/25/13   aro     Updated XPT configure interface to pass XPT mode from
                   calling function
02/22/13   aro     Migrate XPT functions to common EPT module
02/22/13   aro     Migrated the tx resource manager callback function to
                   common CDMA EPT module
02/21/13   aro     Initial version

==============================================================================*/

#include "comdef.h"
#ifdef FEATURE_CDMA1X
#include "rf_cdma_ept.h"
#include "rfcommon_msg.h"
#include "rf_1x_ept.h"
#include "rf_hdr_ept.h"
#include "rfcommon_msm.h"
#include "rfcommon_data.h"
#include "rfm_internal.h"
#include "rf_cdma_mc.h"
#include "rf_cdma_utils_freq.h"

/*============================================================================*/
/*!
  @name XPT CDMA Items

  @brief
  This section contains XPT functions
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get XPT Config Param from NV

  @details
  This function will query the XPT Config Param from NV

  @param device

  @param xpt_cfg_index
 
  @return
  Value of the requested XPT Config param
*/
uint32 
rf_cdma_xpt_get_cfg_param
(
  rfm_device_enum_type device, 
  uint8 xpt_cfg_index 
)
{
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  rfm_cdma_band_class_type band;
  const rf_cdma_tx_static_type *tx_static_nv_cal_ptr; 
  uint32 xpt_cfg_val;
  
  /* Get Read Device Status */
  dev_status_r = rf_cdma_get_device_status( device ); 
  
  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device ); 
  
   /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_cdma_xpt_get_cfg_param:" 
              "NULL data for device %d",device );
    return 0;
  }
 
  /* Get current band */
  band = dev_status_r->curr_band;
  
  tx_static_nv_cal_ptr = 
      rf_cdma_nv_get_tx_static(logical_dev->tx_nv_path[band], band );
  
  if ( tx_static_nv_cal_ptr == NULL )
  {
    RF_MSG_1 ( RF_ERROR, "rf_cdma_xpt_get_cfg_param: Failed to get valid static "
               "NV pointer for device %d", device);
    return 0;
  }
 
  /* Getting the XPT config param value corresponding to index */
  xpt_cfg_val = tx_static_nv_cal_ptr->ept_cfg_params[xpt_cfg_index];
 
  return xpt_cfg_val;
} /* rf_cdma_xpt_get_cfg_param */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query if XPT is enabled
 
  @details
  This function will check the XPT NV to determine if XPT is enabled or not
 
  @param device
  RFM device for which XPT enable is to be checked
 
  @return
  Flag indicating if XPT is enabled or not.
*/
boolean
rf_cdma_xpt_is_enabled
(
  rfm_device_enum_type device
)
{
  boolean is_xpt_enabled = FALSE;
  xpt_cfg_type nv_xpt_mode = 0;

  nv_xpt_mode = (xpt_cfg_type) rf_cdma_xpt_get_cfg_param(device, XPT_CFG);

  if ( ( nv_xpt_mode == EPT_CFG ) ||
       ( nv_xpt_mode == ET_CFG ) || 
       ( nv_xpt_mode == EPT_ET_CFG ) )
  {
    is_xpt_enabled = TRUE;
  }
  else
  {
    is_xpt_enabled = FALSE;
  }

  return is_xpt_enabled;

} /* rf_cdma_xpt_is_enabled */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Query the supported XPT mode of operation based on MSM and NV
 
  @details
  This function will query the NV and MSM to define the supported mode of
  operation.
 
  @param device
  RFM device for which the mode of operation is to be determined
 
  @return
  Supported mode of XPT operation
*/
xpt_cfg_type
rf_cdma_xpt_get_operation_mode
(
  rfm_device_enum_type device
)
{
  xpt_cfg_type msm_xpt_mode; /* XPT mode  */
  xpt_cfg_type nv_xpt_mode; /* NV XPT Mode */
  xpt_cfg_type xpt_mode; /* return XPT Mode */
  xpt_cfg_type xpt_ovr_mode = XPT_CFG_TYPE_MAX; /* return XPT Mode */
  xpt_cfg_type xpt_mode_nv_msm; /* return XPT Mode */
  boolean ovr_en;

  /* Get XPT Mode from NV */
  nv_xpt_mode = (xpt_cfg_type) rf_cdma_xpt_get_cfg_param(device, XPT_CFG);

  /* Get XPT mode supported by MSM */
  msm_xpt_mode = rfcommon_msm_get_supported_xpt_mode();


  /*
                 |-------|-------|-------|
                 |          XPT Mode     |
                 |-------|-------|-------|
                 |  APT  |  EPT  |  ET   |
  |------|-------||-------|-------|-------|
  |------|-------||-------|-------|-------|
  |      |  APT  ||  APT  |  APT  |  APT  |
  | XPT  |-------||-------|-------|-------|
  | Mode |  EPT  ||  APT  |  EPT  |  EPT  |
  |      |-------||-------|-------|-------|
  |      |  ET   ||  APT  |  EPT  |  ET   |
  |------|-------||-------|-------|-------|

  */

  /*---- Determine the XPT mode based on NV and MSM support ------------*/

  /* Init with msm mode */
  xpt_mode_nv_msm = msm_xpt_mode;

  /* Take the lowest mode */
  if (nv_xpt_mode < msm_xpt_mode)
  {
    xpt_mode_nv_msm = nv_xpt_mode;
  }

  if ( xpt_mode_nv_msm >= XPT_CFG_TYPE_MAX )
  {
    RF_MSG_3( RF_ERROR, "rf_cdma_xpt_get_operation_mode: Dev %d - Invalid"
              "XPT mode: NV %d MSM %d", device, nv_xpt_mode, msm_xpt_mode);
    xpt_mode_nv_msm = XPT_CFG_TYPE_MAX;
  } /* if ! ( nv_xpt_mode == msm_xpt_mode ) */

  xpt_mode = xpt_mode_nv_msm;

  /*---- Determine the XPT mode based Override mode ------------*/

  ovr_en = rfcommon_is_xpt_override( device, &xpt_ovr_mode );

  if ( ovr_en == TRUE )
  {

    RF_MSG_2( RF_HIGH, "rf_cdma_xpt_get_operation_mode: Dev %d - In XPT "
              "override mode %d", device, xpt_ovr_mode );

    /* Take the lowest mode */
    if ( xpt_ovr_mode < xpt_mode_nv_msm )
    {
      xpt_mode = xpt_ovr_mode;
    } 
    
    if ( xpt_mode >= XPT_CFG_TYPE_MAX )
    {
      RF_MSG_3( RF_ERROR, "rf_cdma_xpt_get_operation_mode: Dev %d - Invalid"
                "XPT mode: NVMSM %d OVR %d", device, xpt_mode_nv_msm, 
                xpt_ovr_mode);
      xpt_mode = XPT_CFG_TYPE_MAX;
    } /* if ! ( xpt_mode_nv_msm == xpt_ovr_mode ) */
  } /* if ( ovr_en == TRUE ) */

  return xpt_mode;

} /* rf_cdma_xpt_get_operation_mode */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Overwrite ET Path Delay in NV table

  @details
  This function will overwrite ET path delay in NV table, in order to enable 
  use of FW-computed ET path delay

  @param device

  @param delay_value
  ET delay value to overwrite in NV table for given channel

  @param chan
  Channel to overwrite ET delay value in NV table
 
  @return
  Boolean TRUE OR FALSE for success / fail
*/
boolean 
rf_cdma_xpt_override_et_delay_value
(
  rfm_device_enum_type device,
  int32 delay_value,
  uint32 chan
)
{
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  const rf_1x_config_type* rf_1x_config_ptr;
  rfm_cdma_band_class_type band;
  rf_cmda_et_nv_items_type* et_info ;
  
  /* Get Read Device Status */
  dev_status_r = rf_cdma_get_device_status( device ); 
  
  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device ); 
  
   /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_cdma_xpt_override_et_delay_value:" 
              "NULL data for device %d",device );
    return FALSE;
  }
 
  /* Get current band */
  band = dev_status_r->curr_band;

  rf_1x_config_ptr = rf_cdma_nv_get_1x_data( logical_dev->tx_nv_path[band], 
                                             band );
  if (rf_1x_config_ptr == NULL)
  {
    RF_MSG_2(RF_ERROR,"rf_cdma_xpt_override_et_delay_value: rf_cdma_nv_get_1x_data() return NULL for Device %d, Band %d", 
                    logical_dev->tx_nv_path[band],
                    band);
    return FALSE;
  }  

  et_info = &(rf_1x_config_ptr->cdma_common_ptr->tx_cal.et_path_delay_info) ;

  RF_MSG_1(RF_MED, 
           "rf_cdma_xpt_override_et_delay_value: delay Updating value to %d" , delay_value);
  et_info->number_of_valid_entries = 1;
  et_info->et_path_delay_vs_freq[0] = delay_value;
  et_info->upper_bound_channel[0] = chan;
  et_info->upper_bound_freq_KHz[0] = rf_cdma_get_tx_carrier_freq ( band , chan ) ;
  return TRUE;
} /* rf_cdma_xpt_override_et_delay_value */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get path delay that is freq specific from NV table.
  The frequency list must be in ASECENDING ORDER.

  @details
  This function will get path delay that is freq specific from NV table.
  Number of valide ET Delay vs Freq NVs are determined by
  number_of_valid_entries from NV. The freq list must be in ASECENDING ORDER.
  Within the number of valid entries, each Delay entry represents the
  ET Delay value applicable for all frequencies LOWER than OR EQUAL to the
  correponding Freq entry, and HIGHER than the previous Freq entry,
  ***EXCEPT THE LAST VALID DELAY ENTRY.***
  *NOTE* The last Delay entry is applicable to not just the above description,
  but also all frequencies beyond the last Freq entry.
  This is specifically designed as a workaround from SW to handle MISUSE CASE
  where users failed to meet the recommendation that the last valid freq/delay
  entry should be the upper boundary channel for a given band.
 

  @param *tx_device
  Pointer to TX device
 
  @param tx_cal
  The calibration structure that holds the freq comp ET path delay bins

  @param band
  The band the phone os transmitting on.
  
  @param channel 
  The chhanel the phone is transmitting on.

  @param enable_temp_update
  Indicator to allow due-to-temperature update to delay. If enabled, the update
  will occur only if relevant nv data is valid/present
 
  @param temp_read_valid
  Indicates whether the temperature reading is valid. 
 
  @param temp_sensor_read
  Teperature sensor ADC reading.

  @return delay value
  ET path delay for a given band-chan.
*/
int32
rf_cdma_xpt_get_path_delay 
(
   rfdevice_trx_cdma_tx* tx_device,
   rf_cdma_tx_cal_type*     tx_cal ,
   rfm_cdma_band_class_type band ,
   uint32                   channel, 
   boolean enable_temp_update,
   boolean temp_read_valid,
   uint16 temp_sensor_read
)
{
  boolean api_status = TRUE;
  int32 et_path_delay = 0 ;
  int16 delay_offset = 0;
  const rf_cmda_et_nv_items_type* et_info = &(tx_cal->et_path_delay_info) ;
  rfcommon_core_convert_freq_handler_type convert_chan_to_tx_freq = NULL;
  int16 delay_comp_x10ns = 0;
  uint8 num_nv_entries = et_info->number_of_valid_entries ;
  
  if ( (num_nv_entries > 1) && (num_nv_entries <= MAX_FREQ_COMP_ET_PATH_DELAY_BINS) )     
  {
    /* Hunt for the correct freq bin */
    uint8 bin_number = 0; 
    const uint32* chan_into_freq =&(et_info->upper_bound_freq_KHz[0]) ;
    uint32 tx_freq = rf_cdma_get_tx_carrier_freq ( band , channel ) ;
    const uint32* freq_bins = &(et_info->upper_bound_channel[0]) ;
    convert_chan_to_tx_freq = rf_cdma_nv_get_chan_to_tx_freq_handler( band );
  
    /* Search within num_nv_entries entries for a given frequency */
    for ( bin_number = 0 ; bin_number < num_nv_entries; bin_number ++ )
    { 
      /* We know Freq is between bin_number and [bin_number-1] (Or 0)*/
      if ( tx_freq <= chan_into_freq[bin_number] )
      {
        break ;
      }
    }

    /* In case we reach max number of valid entries and still found no freq
    high enough, it means user provided a freqeuency that's higher than 
    what's available in the NV. This is a MISUSE case and SW is designed to
    handle it by returning the next closest thing -- The ET Delay value for
    the highest Freq available */
    if ( bin_number == num_nv_entries  )
    {
      bin_number -= 1;
    }

    if ( bin_number < MAX_FREQ_COMP_ET_PATH_DELAY_BINS )
    {
    et_path_delay = et_info->et_path_delay_vs_freq[bin_number];

    RF_MSG_6 ( RF_MED ,  "rf_cdma_xpt_get_path_delay(): Band %d, Chan %d, "
                         "Bin Index %d, mapped to Chan %d, Freq %d, "
                         "ET Delay: %d" , 
                         band, channel, bin_number, freq_bins[bin_number] , 
                         chan_into_freq[bin_number], et_path_delay ) ;
    }
    else
    {
      et_path_delay = 0;
      RF_MSG_1( RF_ERROR, "rf_cdma_xpt_get_path_delay(): Unexpected boundary"
                          "overflow idx %d, set_et_path_delay to 0.",
                          bin_number );
    }


    /* NV Delay Offset vs Freq vs Temp */
    if ( ( api_status == TRUE ) && 
         ( enable_temp_update == TRUE ) &&
         ( tx_cal->therm_cal_data.is_therm_cal_nv_data_valid == TRUE ) &&
         ( temp_read_valid == TRUE ) &&
         ( tx_cal->et_path_delay_info.et_path_delay_offsets_vs_freq_vs_temp.channels_enabled > 0 ) &&
         ( tx_cal->et_path_delay_info.et_path_delay_offsets_vs_freq_vs_temp.channels_enabled <= RFCOM_TXLIN_NUM_CHANNELS )  )
    {
      if (convert_chan_to_tx_freq != NULL)
      {
        api_status &= rf_common_xpt_delay_offset_computation( 
            &tx_cal->et_path_delay_info.et_path_delay_offsets_vs_freq_vs_temp,
            tx_cal->therm_cal_data.tx_therm_cal,
            temp_sensor_read,
            tx_cal->tx_multi_lin_cal.ref_lin_temp_adc,
            tx_freq,
            et_info->is_delay_cald,
            &delay_offset,
            convert_chan_to_tx_freq );
      }
      else
      {
        RF_MSG( RF_ERROR, "rf_cdma_xpt_get_path_delay: NULL freq handler API. Cannot perform offset computation."
                "Applying NV delay offset = 0" );
        delay_offset = 0;
      }

      if (api_status == FALSE)
      {
        RF_MSG( RF_ERROR, "rf_cdma_xpt_get_path_delay: Common XPT delay offset computation failed. "
                "Applying NV delay offset = 0" );
        delay_offset = 0;
      }

      et_path_delay += delay_offset;
      RF_MSG_2( RF_MED, "rf_cdma_xpt_get_path_delay: temp comp offset %d, new delay %d", delay_offset, et_path_delay );

    }/* NV Delay Offset vs Freq vs Temp */
    else if (enable_temp_update == TRUE)
    {
      RF_MSG_1( RF_HIGH, "rf_cdma_xpt_get_path_delay: Temp comp requested. However, TEMP COMP NOT APPLIED. delay %d", et_path_delay );
    }

    RF_MSG_3 ( RF_MED ,  "rf_cdma_xpt_get_path_delay(): "
             "Channel: %d | Bin: %d | Delay: %d| " ,
             channel , bin_number , et_path_delay ) ;
    /* Round to get an interpolated value
     et_path_delay =
    rf_divide_with_rounding(   
       et_info->et_path_delay_vs_freq[bin_number]  * 
       (valid_tx_freq_in_KHz - freq_bins[bin_number-1])
     + et_info->et_path_delay_vs_freq[bin_number - 1] *
       (freq_bins[bin_number] - valid_tx_freq_in_KHz) ,
       (freq_bins[bin_number] - freq_bins[bin_number - 1]) ); */
  }
  else if ( num_nv_entries == 1 )
  {
   et_path_delay = et_info->et_path_delay_vs_freq[0] ;
   RF_MSG_1( RF_MED, "rf_cdma_xpt_get_path_delay: Using single delay %d", et_path_delay );
  } /* No path delay NVs were found ( num_nv_entries == 0 ) */
  else
  {
    RF_MSG_1 ( RF_ERROR , "rf_cdma_xpt_get_path_delay(): "
                          "Invalid num_nv_entries %d. Setting path delay to 0", 
                           num_nv_entries ) ;
    et_path_delay = 0 ;
    api_status = FALSE ;
    }

  /* Device Delay Compensation */
  if (api_status == TRUE)
  {
    api_status &= rfdevice_cdma_tx_get_et_delay_compensation( tx_device,
                                                              &delay_comp_x10ns );
    if(api_status == FALSE)
    {
       RF_MSG( RF_ERROR, "rf_cdma_xpt_get_path_delay: Device ET delay comp failed. "
               "Applying device delay comp = 0" );
       delay_comp_x10ns = 0;
    }

    et_path_delay += delay_comp_x10ns;
  }

  return et_path_delay ;
} /* rf_cdma_xpt_get_path_delay */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API checks the number of carriers passed to it and returns the status 
  of XPT is enabled for that carrier configuration. 
 
  @details
  This API checks the number of carriers passed to it and returns the status 
  of XPT is enabled for that carrier configuration. 
  It is enough to modify this API in future in XPT support will be enabled for multi 
  carriers.  
 
  @param device
  number of carriers
 
  @return
  Flag indicating if XPT is enabled or not.
*/
boolean
rf_cdma_xpt_is_enabled_for_hdr
(
  uint8 num_carriers
)
{
  boolean is_xpt_enabled_for_hdr;

  if (num_carriers == 0|| num_carriers > RFM_CDMA_CARRIER_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_cdma_xpt_is_enabled_for_hdr(): Invalid Carrier Count:"
                        "%d", num_carriers);
  }	  
  if ( num_carriers == 1)
  {
	is_xpt_enabled_for_hdr = TRUE;
  }
  else
  {
	is_xpt_enabled_for_hdr = FALSE;
  }
	
  return is_xpt_enabled_for_hdr;
} /* rf_cdma_xpt_is_enabled_for_hdr */

#endif /* FEATURE_CDMA1X */

/*! @} */

/*! @} */


