#ifndef RFDEVICE_ANTENNA_SWITCH_INTF_H
#define RFDEVICE_ANTENNA_SWITCH_INTF_H
/*!
   @file
   rfdevice_antenna_switch_intf.h

   @brief
   

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfa.mpss/4.11/device/rfdev_intf/inc/rfdevice_antenna_switch_intf.h#1 $

when       who   what, where, why
--------   ---   -------------------------------------------------------------------
10/10/15   dbc   Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfm_device_types.h"
#include "rfm_mode_types.h"

#ifdef __cplusplus
extern "C" {
#endif



/*----------------------------------------------------------------------------*/

boolean rfdevice_antenna_switch_set_pos 
(  
  rfm_device_enum_type rfm_device,
  uint32               ant_pos, 
  rfcom_mode_enum_type enum_type,
  uint32               rf_band
);


#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_ANTENNA_SWITCH_INTF_H */


