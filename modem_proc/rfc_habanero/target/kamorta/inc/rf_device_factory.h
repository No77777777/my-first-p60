#ifndef RF_DEVICE_FACTORY_H
#define RF_DEVICE_FACTORY_H
/*!
  @file
  rf_device_factory.h

  @brief

*/

/*==============================================================================
Copyright (c) 2011 - 2014 by QUALCOMM Technologies Inc.  All Rights Reserved.

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfc_habanero.mpss/1.11/target/kamorta/inc/rf_device_factory.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
05/27/19   aj      Initial Revision

==============================================================================*/
#include "rfc_common.h"
#include "rfdevice_class.h"
#include "rfdevice_physical_device.h"
#include "rfcommon_atuner_manager_factory.h"

rfdevice_class* 
rf_device_factory_create_tuner_manager
(
  rfdevice_class* device_obj,
   rfcommon_atuner_manager_factory_type manager_type
);

rfdevice_class* 
rf_device_factory_create_device
(
  rfc_device_cfg_info_type* cfg
);

rfdevice_physical_device*
rf_device_factory_create_phys_device
(
   rfc_phy_device_info_type* phy_device_cfg
);

boolean
rf_device_factory_create_gnss_device
( 
  rfc_device_info_type* cfg
);

boolean rf_device_factory_program_device_id
(
   rfc_phy_device_info_type* cfg
);

rfdevice_logical_component* create_gen_device_object(
                                                        rfdevice_physical_device     *rfdevice_physical_third_party_p,
                                                        rfc_logical_device_info_type      *logical_device_cfg
                                                    ); 

#ifdef FEATURE_RF_ASDIV
rfdevice_class* 
rf_device_factory_create_asd_tuner_device 
( 
  void
);
#endif
#endif
