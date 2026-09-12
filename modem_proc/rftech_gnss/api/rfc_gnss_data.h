#ifndef RFC_GNSS_DATA_H
#define RFC_GNSS_DATA_H
/*!
  @file
  rfc_gnss_data.h

  @brief
  This file contains the class definition for the rfc_gnss_data, which provides the rfc related data
  retrival functionality to GNSS.

*/

/*===========================================================================

Copyright (c) 2011-24 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gnss.mpss/3.11/api/rfc_gnss_data.h#2 $

when       who     what, where, why
-------------------------------------------------------------------------------
02/27/24   ssk     Added new API to get both WGR and WTR IDs 
10/18/16   jr      Changes to get Rf device ID for NavRF
08/26/13   jr      Added FTM_GET_EFS_VERIFICATION_DATA
03/12/13   vrb     Adding support for getting band split configuration per RFC
07/09/12   sr      changes to use the rfc_data object instead of rfc_ag function ptr table.
05/09/12   sr      init version
============================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif
#include "rfdevice_chip_ids.h"
#ifdef __cplusplus
} /* extern "C" */
#endif


#ifdef __cplusplus
#include "rfa.h"
#include "rfc_common.h"

/* Definition of rfc_gnss_data class */
class rfc_gnss_data : public rfa
{
public:

  static rfc_gnss_data * get_instance();

  // GNSS rfc data
  virtual boolean sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr );
  virtual boolean devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr );
  virtual boolean band_split_cfg_data_get( rfc_cfg_params_type *cfg, rfc_band_split_info_type **ptr );
  virtual boolean get_rfcard_data( void );


  // Destructor
  virtual ~rfc_gnss_data();

protected:

  // constructor
  rfc_gnss_data();

  static rfc_gnss_data *rfc_gnss_data_ptr;

private:

};
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif

/*!
  @brief
  This function returns the rfc_device info data for GNSS tech
  parameters.

  @details
  This function returns the rfc_device info data for GNSS tech
  parameters.

  @param
  'device_id' a.

  @retval
  TRUE - if the rfc_data is available or FALSE if error.
*/
boolean rfc_gnss_get_devices_info_data(rfdevice_id_enum_type *device_id);

/*!
  @brief
  This function returns both SDR and WGR device IDs for GNSS tech. If there is no WGR support then RFDEVICE_INVALID is returned
  parameters

  @details
  This function returns both SDR and WGR device IDs for GNSS tech. If there is no WGR support then RFDEVICE_INVALID is returned
  parameters

  @param
  'device_id_1' : device id. 
  'device_id_2' : device id.

  @retval
  TRUE - if the rfc_data is available or FALSE if error.
*/
boolean rfc_gnss_get_devices_info_data_v2(rfdevice_id_enum_type *device_id_1, rfdevice_id_enum_type *device_id_2);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RFC_GNSS_DATA_H */

