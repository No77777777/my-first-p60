#ifndef RFDEVICE_COUPLER_COMMON_H
#define RFDEVICE_COUPLER_COMMON_H
/*!
   @file
   rfdevice_coupler_common.h

   @brief
   This file contains definition & prototypes for generic 3rd party Coupler devices

*/
/*==============================================================================

  Copyright (c) 2014-2015 Qualcomm Technologies Incorporated. All Rights Reserved

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
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_coupler.mpss/1.22/api/rfdevice_coupler_common.h#1 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
07/14/15   px      Initial version

==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#include "rf_buffer_intf.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_physical_third_party.h"


/*API to create 3rd party Coupler with physical device interface support.
Once created successfully it registers itself as a child of the physical device object passed to it*/
rfdevice_logical_component* create_gen_coupler_object
(
  rfdevice_physical_device      *rfdevice_physical_third_party_p,
  rfc_logical_device_info_type  *logical_device_cfg
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*RFDEVICE_COUPLER_COMMON_H*/

