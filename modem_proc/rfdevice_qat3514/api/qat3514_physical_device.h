#ifndef QAT3514_PHYSICAL_DEVICE_H
#define QAT3514_PHYSICAL_DEVICE_H

/*===========================================================================

Copyright (c) 2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_qat3514.mpss/1.11/api/qat3514_physical_device.h#1 $
  $DateTime: 2021/03/09 04:02:43 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   --------------------------------------------------------------- 
02/19/16   hm    Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rfdevice_antenna_tuner.h"
#include "rfdevice_qtuner.h"
#include "rfc_common.h"

#include "rfdevice_physical_device.h"
#include "qat3514_tuner_config_ag.h"


class qat3514_physical_device : public rfdevice_physical_device
{
public:

  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);

  qat3514_physical_device* qat3514_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  qat3514_physical_device(rfc_phy_device_info_type* cfg_req,
  	                      boolean ext_cmd = TRUE );

  ~qat3514_physical_device();

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);

private:
  /* Device config */
  rfc_device_cfg_info_type* cfg;
  boolean extended_cmd;

  /* QTUNER */
  void create_tuner_object( rfc_logical_device_info_type *logical_device_info ); 

  rfdevice_antenna_tuner* qat3514_qtuner_obj_ptr;
  
};
#endif