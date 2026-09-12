#ifndef RFDEVICE_LTE_LNA_INTF_H
#define RFDEVICE_LTE_LNA_INTF_H
/*!
   @file
   rfdevice_lte_lna_intf.h

   @brief

*/

/*===========================================================================

Copyright (c) 2012 - 15 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/rfa.mpss/4.11/device/rfdev_intf/inc/rfdevice_lte_lna_intf.h#1 $

when         who   what, where, why
--------   ---   -------------------------------------------------------------------
09/25/15   sb      FR30081:Apply gain offsets for ASDIV switch position change
07/13/15   vkm     Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfcom.h"
#include "rflte_msg.h"
#include "rfdevice_lna_intf.h"
#ifdef FEATURE_LTE
#include "rfdevice_lte_type_defs.h"
#endif /*FEATURE_LTE*/
#ifdef __cplusplus
extern "C" {
#endif
#ifdef FEATURE_LTE


  /*===========================================================================
                           Data Definitions
  ===========================================================================*/

  /*===========================================================================
                           Prototypes
  ===========================================================================*/
  /* ----------------------------------------------------------------------- */
  /* function prototypes */

boolean rfdevice_lte_lna_wakeup   
(
  rfcom_lte_band_type rf_band,
  rfm_device_enum_type rfm_device,
  rfdevice_lte_script_data_type* script_data_ptr,
  int16 script_timing
);

boolean rfdevice_lte_lna_sleep
(
  rfcom_lte_band_type rf_band,
  rfm_device_enum_type rfm_device,
  rfdevice_lte_script_data_type* script_data_ptr,
  int16 script_timing
);
#if 0
boolean rfdevice_lte_lna_trigger   
(
  rfcom_lte_band_type rf_band,
  rfm_device_enum_type rfm_device,
  rfdevice_lte_script_data_type* script_data_ptr,
  int16 script_timing
);
#endif
boolean rfdevice_lte_get_lna_gain_index 
(
  rfcom_lte_band_type rf_band,
  rfm_device_enum_type rfm_device,
  int8 system_gain, 
  rfdevice_lna_gain_enum_type *lna_gain,
  uint32 sw_pos 
);


boolean rfdevice_lte_set_lna_gain   
(
  rfcom_lte_band_type rf_band,
  rfm_device_enum_type rfm_device,
  rfdevice_lna_gain_enum_type lna_gain,
  rfdevice_lte_script_data_type* script_data_ptr,
  int16 script_timing
);
#endif /*FEATURE_LTE*/

#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_PA_INTF_H */


