#ifndef RFDEVICE_WCDMA_XSW_INTF_H
#define RFDEVICE_WCDMA_XSW_INTF_H
/*!
   @file
   rfdevice_wcdma_xsw_intf.h

   @brief


*/

/*===========================================================================

Copyright (c) 2012 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/02/18   nik     Adding QAT3522 XSW support for WCDMA

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#ifdef FEATURE_WCDMA
#include "rfcom.h"
#include "rfdevice_wcdma_type_defs.h"
#include "rf_buffer_intf.h"
#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                          Prototypes
 ===========================================================================*/

/* ----------------------------------------------------------------------- */
boolean rfdevice_wcdma_xsw_sleep
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing,
  uint32 asdiv_position
);
/* ----------------------------------------------------------------------- */

boolean rfdevice_wcdma_xsw_wakeup
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type rf_band,
  rf_buffer_intf *script,
  rf_device_execution_type execution_type,
  int16 script_timing,
  uint32 asdiv_position
);

#ifdef __cplusplus
}
#endif
#endif /* FEATURE_WCDMA */
#endif
