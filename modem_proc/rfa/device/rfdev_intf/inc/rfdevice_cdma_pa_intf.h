#ifndef RFDEVICE_CDMA_PA_INTF_H
#define RFDEVICE_CDMA_PA_INTF_H
/*!
   @file
   rfdevice_cdma_pa_intf.h

   @brief


*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:

when       who   what, where, why
--------   ---   --------------------------------------------------------------- 
06/02/15   dbc    Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#ifdef FEATURE_CDMA1X


#include "rfcom.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
                         Prototypes
===========================================================================*/
/* ----------------------------------------------------------------------- */
/* function prototypes */
boolean rfdevice_cdma_set_pa_output_cap
(
  rfm_device_enum_type device,
  rfm_cdma_band_class_type band,
  int8 output_cap_offset
);


#ifdef __cplusplus
}
#endif
#endif /* FEATURE_CDMA1X */

#endif /* RFDEVICE_CDMA_PA_INTF_H */


