#ifndef RFDEVICE_LNA_INTF_H
#define RFDEVICE_LNA_INTF_H
/*!
   @file
   rfdevice_lna_intf.h

   @brief

*/

/*===========================================================================

  Copyright (c) 2014-2015 Qualcomm Technologies Inc. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Inc.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/rfa.mpss/4.11/device/rfdev_intf/inc/rfdevice_lna_intf.h#1 $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/02/15   px    Added support for alternate gain map
12/08/14   vv    Initial version
==============================================================================*/

/*==============================================================================
                           INCLUDE FILES
==============================================================================*/
#include "rfcom.h"
#include "rfdevice_cmn_intf.h"
#include "rfdevice_cmn_type_defs.h"
#include "rfdevice_lna.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   rfm_device_enum_type rfm_device;
   rfcom_mode_enum_type mode;
   rfcom_band_type_u band;
   rfdevice_lna_trigger_enum_type  trigger_type;
   rfdevice_lna_gain_enum_type lna_gain;
   int8 system_gain;
}rfdevice_lna_config_type;


boolean rfdevice_lna_set_band_map
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  int32 *data_ptr, 
  uint8 size
);

boolean rfdevice_lna_wakeup   
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  void *buff_obj_ptr,
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean rfdevice_lna_sleep   
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  void *buff_obj_ptr,
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean rfdevice_lna_trigger   
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  void *buff_obj_ptr,
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean rfdevice_lna_set_gain   
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  void *buff_obj_ptr,
  rf_device_execution_type dev_action,
  int16 script_timing
);


boolean rfdevice_get_lna_gain_index 
(
  void *device_ptr,
  rfdevice_lna_config_type *lna_cfg_p,
  rfdevice_lna_gain_enum_type *lna_gain,
  boolean use_alt_gain
);

#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_LNA_INTF_H */


