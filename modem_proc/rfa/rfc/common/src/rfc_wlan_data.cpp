/*!
   @file
   rfc_wlan_data.cpp

   @brief
   This file contains implementation the rfc_wlan_data class, which will be used by RFC to retrieve
   the WLAN related data.

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:11:02 $ $Author: pwbldsvc $
$Header: //components/rel/rfa.mpss/4.11/rfc/common/src/rfc_wlan_data.cpp#1 $

when       who     what, where, why
-------------------------------------------------------------------------------
05/21/15   fhuo      Initial version.

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rfc_wlan_data.h"

rfc_wlan_data *rfc_wlan_data::rfc_wlan_data_ptr = NULL;

rfc_wlan_data * rfc_wlan_data::get_instance()
{
  return(rfc_wlan_data_ptr);
}

// Constructor
rfc_wlan_data::rfc_wlan_data()
{
}

// Destructor
rfc_wlan_data::~rfc_wlan_data()
{
}

/*!
  @brief
  This function returns the rfc_signals list for wlan tech, based on specified configuration
  parameters.

  @details
  This function returns the rfc_Signals list for wlan tech, based on specified configuration
  parameters.

  @param
  'cfg' : configuration parameters for which rfc_data to be retrived.
  'ptr' : ptr to the rfc_Signals list.

  @retval
  TRUE - if the rfc_data is available or FALSE if error.
*/
boolean rfc_wlan_data::sig_cfg_data_get(rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr)
{
  if (NULL != ptr)
  {
    *ptr = NULL;
  }

  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "WLAN RFC Signals data NOT Available", 0);
  return FALSE;
}


/*----------------------------------------------------------------------------*/

/*!
  @brief
  This function returns the rfc_device info data for wlan tech, based on specified configuration
  parameters.

  @details
  This function returns the rfc_device info data for wlan tech, based on specified configuration
  parameters.

  @param
  'cfg' : configuration parameters for which rfc_device info data to be retrived.
  'ptr' : ptr to the rfc_device info data.

  @retval
  TRUE - if the rfc_data is available or FALSE if error.
*/
boolean rfc_wlan_data::devices_cfg_data_get(rfc_cfg_params_type *cfg, rfc_device_info_type **ptr)
{
  if (NULL != ptr)
  {
    *ptr = NULL;
  }

  MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "WLAN RFC Device info data NOT available", 0);
  return FALSE;
}
