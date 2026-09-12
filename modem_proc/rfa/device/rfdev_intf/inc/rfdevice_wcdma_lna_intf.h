#ifndef RFDEVICE_WCDMA_LNA_INTF_H
#define RFDEVICE_WCDMA_LNA_INTF_H
/*!
   @file
   rfdevice_wcdma_lna_intf.h

   @brief


*/

/*===========================================================================

Copyright (c) 2012 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/24/15   sd      FR30081 add support for AsDiv alt gain param in device APIs
12/13/14   vbh    Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#ifdef FEATURE_WCDMA
#include "rfcom.h"
#include "rfdevice_lna_intf.h"
#include "rfdevice_wcdma_type_defs.h"
#include "rf_buffer_intf.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                          Prototypes
 ===========================================================================*/

/* ----------------------------------------------------------------------- */
boolean rfdevice_wcdma_lna_sleep
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing
);
/* ----------------------------------------------------------------------- */

boolean rfdevice_wcdma_lna_wakeup
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing
);
/*----------------------------------------------------------------------------*/

boolean rfdevice_wcdma_lna_get_device_info
(
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type rf_band,
  uint16 *mfg_id,
  uint16 *prd_id,
  uint16 *rev_id
);

/*----------------------------------------------------------------------------*/
boolean rfdevice_wcdma_lna_trigger
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
boolean rfdevice_wcdma_lna_set_band_map
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
boolean rfdevice_wcdma_lna_set_gain
(
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing,
  int8 system_gain,
  boolean asdiv_alt_gain
);

/*----------------------------------------------------------------------------*/
boolean rfdevice_wcdma_lna_get_gain_index
(
  rfm_device_enum_type rfm_device,
  rfcom_wcdma_band_type rf_band,
  int8 system_gain,
  rfdevice_lna_gain_enum_type *gain_state,
  void *device_ptr,
  boolean asdiv_alt_gain
);

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* FEATURE_WCDMA */
#endif /* RFDEVICE_PA_INTF_H */


