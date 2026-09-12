/*!
   @file
   rfc_common_data.cpp

   @brief
   This file contains implementation the rfc_common_data class, which will be used by RFC to
   retrieve the rfc common related data.

*/

/*===========================================================================

Copyright (c) 2011-2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rf/common/rf/rfc/src/rfc_common_data.cpp#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
10/26/17   gk      Compilation fixes for RFA for migration of 6.x LLVM to 8.x
11/06/15   hbl     [Ported]Change IRAT alt path lookup implementation
07/14/15   Saul    Access rfc device properties using ptr.
06/02/15   zhw     Minor fixes
06/01/15   zhw     Added support for RFC preferred device query
04/30/15   Saul    CRAT Updates
09/29/14   Saul    New alt path selection APIs.
08/08/14   tks     Modified get alt path table function param list 
07/14/14   vrb     Support for Alternate Path Selection Table
06/16/14   vv      Added functions to get physical and logical device cfg from RFC
04/23/14   tks     Support for dynamic path selection
03/17/14   kg      Port from Dime
02/11/14   sr      Re-design asdiv_tuner to support multiple RF configurations
08/26/13   jr      Added FTM_GET_EFS_VERIFICATION_DATA
05/21/13   sd      Add new api to get cmn device properties
11/26/12   sr      Added flag to capture the rfc data init status.
09/24/12   sr      Made changes to remove hand-coded RFC files.
07/18/12   sr      Changes to create devices through device factory.
07/09/12   sr      changes to use the rfc_data object instead of rfc_ag function ptr table.
05/09/12   sr      Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rfc_common_data.h"

rfc_common_data *rfc_common_data::rfc_common_data_ptr = (rfc_common_data *)NULL;

rfc_common_data * rfc_common_data::get_instance(void)
{
  return rfc_common_data_ptr;
}

// Constructor
rfc_common_data::rfc_common_data(rf_hw_type rf_hw)
  :m_rf_hw(rf_hw)
{
}

// Destructor
rfc_common_data::~rfc_common_data()
{
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the rfc data from efs file to local memory.

  @details
  This function returns the rfc data from efs file to local memory.

  @param
  rfc_info_table : rfc_info_table ptr which is read from EFS file.

  @retval
  num_rfc_signals: number of rf signals.
*/
uint32 rfc_common_data::sig_info_table_get(rfc_signal_info_type **rfc_info_table)
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC Signals info data NOT Available (hw_id = %d)", m_rf_hw);

  return 0;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the rf-card logical device configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @details
  This function returns the rf-card logical device configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @retval
  NULL ptr for the device config info.
*/
rfc_logical_device_info_type* rfc_common_data::get_logical_device_cfg( void )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC Logical device config info data NOT Available (hw_id = %d)", m_rf_hw);

  return (rfc_logical_device_info_type *)NULL;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the rf-card physical device configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @details
  This function returns the rf-card physical device configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @retval
  NULL ptr for the device config info.
*/
rfc_phy_device_info_type* rfc_common_data::get_phy_device_cfg( void )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC  Physical device config info data NOT Available (hw_id = %d)", m_rf_hw);

  return (rfc_phy_device_info_type *)NULL;

}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the rf card common data. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @details
  This function returns the rf-card complete data. This should be implemented in the
  rf-card specific data file. This base-class function returns NULL.

  @retval
  NULL ptr for the device config info.
*/
boolean rfc_common_data::get_rfcard_data( void )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "get_rfcard_data is available to collect (hw_id = %d)", m_rf_hw);

  return FALSE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the logical path configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @details
  This function returns the logical path configuration info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @retval
  FALSE to indicate requested data is NOT available.
*/
boolean rfc_common_data::get_logical_path_config( rfm_devices_configuration_type* dev_cfg )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC Logical Path Config info data NOT Available (hw_id = %d)", m_rf_hw);

  return FALSE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the logical device properties info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @retval
  FALSE to indicate requested data is NOT available.
*/
const rfm_devices_configuration_type* rfc_common_data::get_logical_device_properties( void )
{
  RF_MSG_1( RF_ERROR, "RFC Logical Device Properfies info API not implemented (hw_id = %d)", m_rf_hw );

  return NULL;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the common property info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @details
  This function returns the common property info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @retval
  FALSE to indicate requested data is NOT available.
*/
boolean rfc_common_data::get_cmn_properties( rfc_cmn_properties_type **ptr )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC Common Property info data NOT Available (hw_id = %d)", m_rf_hw);

  return FALSE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function set the rfc init failure flag.

  @details
  This function set the rfc init failure flag. The init_failure flag is used to indicate
  un-sucessful unitialization of the rf-card.

  @retval
  None.
*/
void rfc_common_data::set_rfc_init_fail_flag(rfm_mode_enum_type mode)
{
  rfc_init_flag |= (0x1 << mode);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the rfc init failure flag.

  @details
  This function returns the rfc init failure flag. The init_failure flag indicates
  un-sucessful unitialization of the rf-card.

  @retval
  None.
*/
uint32 rfc_common_data::get_rfc_init_fail_flag(void)
{
  return (rfc_init_flag);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to retrieve alt path selection info specific to a rf card
 
  @details
  Function returns a pointer to the alt path selection table as well as the count
  for total no of entries and size per entry. This is used by concurrency
  manager to detect the device mapping and allocate alternate path accordingly 
 
  @param *tbl_size
  pointer to get the table size or total no of entries

  @retval rfc_alt_path_sel_type
  pointer to the path selection table specific to a rf card
 
*/
rfc_alt_path_sel_type*
rfc_common_data::get_alt_path_selection_tbl
(
  uint32 *tbl_size
)
{
  /* Since there is no-valid table, all indexes are zeroed out */
  *tbl_size  = 0; 

  return NULL ; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Function to retrieve IRAT alt path selection info specific to a rf card
 
  @details
  Function returns a pointer to the IRAT specific alternate path selection
  table as well as the count for total no of entries and size per entry.
  This is used by concurrency manager to detect the device mapping and
  allocate alternate path accordingly 
 
  @param *tbl_size
  pointer to get the table size or total no of entries

  @retval rfc_alt_path_sel_type
  pointer to the path selection table specific to a rf card
 
*/
rfc_alt_path_sel_type*
rfc_common_data::get_irat_alt_path_selection_tbl
(
  uint32 *tbl_size,
  uint32 *num_band_groups
)
{
  /* Since there is no-valid table, all indexes are zeroed out */
  *tbl_size  = 0; 
  *num_band_groups  = 0;

  return NULL ; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns ASDiv config info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @details
  This function returns the ASDiv config info. This should be implemented in the
  rf-card specific data file. This base-class function returns FALSE.

  @retval
  FALSE to indicate requested data is NOT available.
*/
boolean rfc_common_data::get_asd_device_info( rfc_asd_cfg_params_type *cfg, rfc_asdiv_config_info_type **ptr )
{
  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RFC ASDiv info data NOT Available (hw_id = %d)", m_rf_hw);

  return FALSE;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred associated rx or tx device for the given device.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.
 
  @param device
  The device for which to obtain preferred associated device
 
  @param rx_tx
  Indicates whether to get the rx or tx preferred assocaited device

  @retval
  Associated device.
*/
rfm_device_enum_type
rfc_common_data::rfc_get_preferred_associated_device
(
  rfm_device_enum_type device,
  rfc_rxtx_enum_type rx_tx
)
{
  boolean api_status = TRUE;
  rfm_device_enum_type preferred_associated_device = RFM_INVALID_DEVICE;
  const rfm_devices_configuration_type* dev_cfg;
  rfc_common_data * cmn_data_ptr = NULL;

  cmn_data_ptr = rfc_common_data::get_instance();
  if (cmn_data_ptr == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "rfc_get_preferred_associated_device - NULL cmn data ptr." );
    return RFM_INVALID_DEVICE;
  }

  /* Get device config info */
  dev_cfg = cmn_data_ptr->get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG_2( RF_ERROR, 
           "rfc_get_preferred_associated_device - Device %d, RxTx %d. NULL ptr.", 
           device, rx_tx );
    return RFM_INVALID_DEVICE;
  }

  /* Array bounds check */
  if (device >= ARR_SIZE(dev_cfg->device_support))
  {
    MSG_3( MSG_SSID_RF_DEBUG, MSG_LEGACY_ERROR, 
           "rfc_get_preferred_associated_device - Device %d, RxTx %d. Device number exceeds num available items %d.", 
           device, rx_tx, ARR_SIZE(dev_cfg->device_support) );
    return RFM_INVALID_DEVICE;
  }

  if (api_status == TRUE)
  {
    /* Get rx or tx associated device */
    switch (rx_tx)
    {
    case RFC_CONFIG_RX:
      preferred_associated_device = dev_cfg->device_support[device].preferred_associated_rx_device;
      break;

    case RFC_CONFIG_TX:
      preferred_associated_device = dev_cfg->device_support[device].preferred_associated_tx_device;
      break;

    default:
      MSG_2( MSG_SSID_RF, MSG_LEGACY_ERROR, 
             "rfc_get_preferred_associated_device - Device %d, RxTx type %d not supported.", 
             device, rx_tx);
      preferred_associated_device = RFM_INVALID_DEVICE;
      break;
    }
  }
  else
  {
    preferred_associated_device = RFM_INVALID_DEVICE;
  }

  return preferred_associated_device;
} /* rfc_get_preferred_associated_device() */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the preferred rx or tx device for the given sys_band.

  @details
  If an invalid input is provided, it returns RFM_INVALID_DEVICE.
 
  @param sys_band
  Sys Band enum for a given tech/band
 
  @param rx_tx
  Indicates whether to get the rx or tx preferred assocaited device

  @retval
  Preferred device 
*/
rfm_device_enum_type
rfc_common_data::rfc_get_preferred_device
(
  sys_band_class_e_type sys_band,
  rfc_rxtx_enum_type rx_tx
)
{
  boolean api_status = TRUE;
  rfm_device_enum_type dev_idx;
  rfm_device_enum_type preferred_device = RFM_INVALID_DEVICE;
  const rfm_devices_configuration_type* dev_cfg;
  rfc_common_data * cmn_data_ptr = NULL;
  uint64 valid_band_found = 0;

  cmn_data_ptr = rfc_common_data::get_instance();
  if (cmn_data_ptr == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "rfc_get_preferred_device - NULL cmn data ptr." );
    return RFM_INVALID_DEVICE;
  }

  rfm_device_capability_type rx_tx_capability = RFM_DEVICE_INVALID_SUPPORT;

  if (api_status == TRUE)
  {
    /* Get rx or tx associated device */
    switch (rx_tx)
    {
    case RFC_CONFIG_RX:
      rx_tx_capability = RFM_DEVICE_RX_SUPPORTED;
      break;

    case RFC_CONFIG_TX:
      rx_tx_capability = RFM_DEVICE_TX_SUPPORTED;
      break;

    default:
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
             "rfc_get_preferred_device - RxTx type %d not supported.", 
             rx_tx);      
      break;
    }
  }

  /* Get device config info */
  dev_cfg = cmn_data_ptr->get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG_2( RF_ERROR, 
           "rfc_get_preferred_device - sys_band %d, RxTx %d. NULL ptr.", 
           sys_band, rx_tx );
    return RFM_INVALID_DEVICE;
  }

  for ( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx = (rfm_device_enum_type)((int)(dev_idx) + 1) )
  {
    if ( rx_tx_capability == dev_cfg->device_support[dev_idx].capability )
    {
      if ((sys_band >= 0) && (sys_band <= 63)) 
      {
        //MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH, 
        //       "rfc_get_preferred_device - dev_cfg->device_support[%d].preferred_bands_supported.mask[0] 0x%08x%08x.", 
        //       dev_idx,
        //       dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[0] >> 32,
        //       dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[0]);      

        valid_band_found = ( (uint64)1 << sys_band ) & dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[0] ;
      }
      else if ( ( sys_band > 63 ) && (sys_band <= 127) )
      {
        //MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH, 
        //       "rfc_get_preferred_device - dev_cfg->device_support[%d].preferred_bands_supported.mask[1] 0x%08x%08x.", 
        //       dev_idx,
        //       dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[1] >> 32,
        //       dev_cf->device_support[dev_idx].preferred_bands_supported.mask[1]);      

        valid_band_found = ( (uint64)1 << (sys_band - 64) ) & dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[1] ;
      }
      else if ( ( sys_band > 127 ) && (sys_band <= 191) )
      {
        //MSG_3( MSG_SSID_RF, MSG_LEGACY_HIGH, 
        //       "rfc_get_preferred_device - dev_cfg->device_support[%d].preferred_bands_supported.mask[2] 0x%08x%08x.", 
        //       dev_idx,
        //       dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[2] >> 32,
        //       dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[2]);      

        valid_band_found = ( (uint64)1 << (sys_band - 128) ) & dev_cfg->device_support[dev_idx].preferred_bands_supported.mask[2] ;
      }

      if ( valid_band_found != 0 )
      {

        //MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
        //       "rfc_get_preferred_device - Matching device found %d.", 
        //       dev_idx);      
        preferred_device = dev_idx;
        break;
      }
      else
      {
        MSG_1( MSG_SSID_RF, MSG_LEGACY_HIGH, 
               "rfc_get_preferred_device - No Matching for dev %d.", 
               dev_idx);      
      }
    }/* if ( rx_tx_capability == dev_cfg->device_support[dev_idx].capability ) */
  }/* for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx ++ ) */  

  return preferred_device;
} /* rfc_get_preferred_device() */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function checks whether a sys_band is listed in the band_supported
  fields in RFC

  @details
  If an invalid input is provided, it returns FALSE.
 
  @param sys_band
  Sys Band enum for a given tech/band
 
  @param device
  RFM Device to check against

  @retval
  TRUE if supported. FALSE if otherwise
*/
boolean 
rfc_common_data::rfc_check_band_supported
(
  sys_band_class_e_type sys_band,
  rfm_device_enum_type device
)
{
  boolean band_supported = FALSE;

  boolean api_status = TRUE;
  const rfm_devices_configuration_type* dev_cfg;
  rfc_common_data * cmn_data_ptr = NULL;
  uint64 valid_band_found = 0;

  cmn_data_ptr = rfc_common_data::get_instance();
  if (cmn_data_ptr == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "rfc_check_band_supported - NULL cmn data ptr." );
    return FALSE;
  }

  /* Get device config info */
  dev_cfg = cmn_data_ptr->get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG_2( RF_ERROR, 
           "rfc_check_band_supported - device %d sys_band %d. NULL ptr.", 
           device, sys_band );
    return FALSE;
  }

  if (device >= ARR_SIZE(dev_cfg->device_support))
  {
    RF_MSG_1( RF_ERROR, "rfc_check_band_supported: Error invalid device %d.", device );
    return FALSE;
  }

  if ( api_status == TRUE)
  {
    if ((sys_band >= 0) && (sys_band <= 63)) 
    {
      valid_band_found = ( (uint64)1 << sys_band ) & dev_cfg->device_support[device].bands_supported.mask[0] ;
    }
    else if ( ( sys_band > 63 ) && (sys_band <= 127) )
    {
      valid_band_found = ( (uint64)1 << (sys_band - 64) ) & dev_cfg->device_support[device].bands_supported.mask[1] ;
    }
    else if ( ( sys_band > 127 ) && (sys_band <= 191) )
    {
      valid_band_found = ( (uint64)1 << (sys_band - 128) ) & dev_cfg->device_support[device].bands_supported.mask[2] ;
    }
    else
    {
      RF_MSG_1( RF_ERROR, "rfc_check_band_supported - Error sys_band index %d not supported. Array bounds exceeded.", sys_band );
      return FALSE;
    }

    if ( valid_band_found != 0 )
    {
      band_supported = TRUE;
    }
  }

  return band_supported;
} /* rfc_check_band_supported() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function remaps the old device represenations in ftm to the new physical device scheme.

  @details
  This API is to be used only in FTM to get the equivalent device mapping from
  the new physical device mappings.
 
  @param source_device_info
  Contains the old device information for which to get equivalent new re-mapping
 
  @param remapped_device_info
  Contains the re-mapped device information

  @retval
  TRUE means provided remapped info should be deemed usable/valid.
  FALSE means an internal API failure. Provided remapped info should be deemed invalid.
*/
boolean
rfc_common_data::rfc_get_remapped_device_info
(
   rfc_cal_device_remap_info_type *source_device_info,
   rfc_cal_device_remap_info_type *remapped_device_info
)
{
  boolean api_status = FALSE;

  /* RF cards that need to actually remap device info will override/implement this API */

  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfc_get_remapped_device_info - API not implemented (hw_id = %d)", m_rf_hw);

  return api_status;
} /* rfc_get_remapped_device_info() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retuns the capability(RX or TX) of the rfm device passed

  @details
  This function retuns the capability(RX or TX) of the rfm device passed
 
  @param device
  Contains the rfm device whose capability needs to be queried

  @retval
  RFM_DEVICE_INVALID_SUPPORT(0x00) for Invalid support(No Rx/Tx) on that device
  RFM_DEVICE_RX_SUPPORTED(0x01) for RX device
  RFM_DEVICE_TX_SUPPORTED(0x02) for TX device
*/
rfm_device_capability_type
rfc_common_data::rfc_get_device_capability
(
  rfm_device_enum_type device
)
{
  boolean api_status = TRUE;
  rfm_device_capability_type device_capability = RFM_DEVICE_INVALID_SUPPORT;
  const rfm_devices_configuration_type* dev_cfg;
  rfc_common_data * cmn_data_ptr = NULL;    
  cmn_data_ptr = rfc_common_data::get_instance();

  if (cmn_data_ptr == NULL)
  {
    MSG( MSG_SSID_RF, MSG_LEGACY_ERROR, 
         "rfc_get_device_capability - NULL cmn data ptr." );
    return RFM_DEVICE_INVALID_SUPPORT;
  }
    
  /* Get device config info */
  dev_cfg = cmn_data_ptr->get_logical_device_properties();
  if (dev_cfg == NULL)
  {
    RF_MSG_1( RF_ERROR, 
           "rfc_get_device_capability - device %d. NULL ptr.", 
           device );
    return RFM_DEVICE_INVALID_SUPPORT;
  }

  /* Array bounds check */
  if (device >= ARR_SIZE(dev_cfg->device_support))
  {
    MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "rfc_get_device_capability - Invalid Device %d",device);
    return RFM_DEVICE_INVALID_SUPPORT;
  }

  if(api_status == TRUE)
  {
    device_capability = (rfm_device_capability_type)dev_cfg->device_support[device].capability;
  }
  return device_capability;
}
