#ifndef QAT5515_PHYSICAL_DEVICE_H
#define QAT5515_PHYSICAL_DEVICE_H

/*=============================================================================

Copyright (c) 2019 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/rfdevice/qat/main/1.0/qat5515/protected/qat5515_physical_device.cpp#1 : andrewy : 2017/07/24 17:27:35

when       who    what, where, why
--------   ---   ---------------------------------------------------------------
10/18/19   dh    Initial file

=============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rfdevice_antenna_tuner.h"
#include "rfdevice_qtuner.h"
#include "rfc_common.h"

#include "rfdevice_physical_device.h"
#include "qat5515_tuner_config_ag.h"


class qat5515_physical_device : public rfdevice_physical_device
{
public:

  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);

  qat5515_physical_device* qat5515_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  qat5515_physical_device(rfc_phy_device_info_type* cfg_req,
  	                      boolean ext_cmd = TRUE );

  ~qat5515_physical_device();

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);

private:
  /* Device config */
  rfc_device_cfg_info_type* cfg;
  boolean extended_cmd;

  /* QTUNER */
  void create_tuner_object( rfc_logical_device_info_type *logical_device_info ); 

  rfdevice_antenna_tuner* qat5515_qtuner_obj_ptr;
  
};
#endif