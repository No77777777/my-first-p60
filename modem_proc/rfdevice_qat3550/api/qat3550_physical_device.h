#ifndef QAT3550_PHYSICAL_DEVICE_H
#define QAT3550_PHYSICAL_DEVICE_H

/*===========================================================================

Copyright (c) 2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_qat3550.mpss/1.11/api/qat3550_physical_device.h#1 $
  $DateTime: 2021/03/09 03:39:34 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   --------------------------------------------------------------- 
02/17/16   ndb   Remove the chip level dependency in the common qtuner driver
02/02/16   hm    Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rfdevice_antenna_tuner.h"
#include "rfdevice_qtuner.h"
#include "rfc_common.h"

#include "rfdevice_physical_device.h"
#include "qat3550_tuner_config_ag.h"


class qat3550_physical_device : public rfdevice_physical_device
{
public:

  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);

  qat3550_physical_device* qat3550_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  qat3550_physical_device(rfc_phy_device_info_type* cfg_req,
  	                      boolean ext_cmd = TRUE );

  ~qat3550_physical_device();

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);

private:
  /* Device config */
  rfc_device_cfg_info_type* cfg;
  boolean extended_cmd;

  /* QTUNER */
  void create_tuner_object( rfc_logical_device_info_type *logical_device_info ); 

  rfdevice_antenna_tuner* qat3550_qtuner_obj_ptr;
  
};
#endif