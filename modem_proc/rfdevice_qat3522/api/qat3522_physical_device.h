
#ifndef QAT3522_PHYSICAL_DEVICE_H
#define QAT3522_PHYSICAL_DEVICE_H
/*!
   @file
   qat3522_physical_device.h

   @brief
   qat3522 physical device driver

*/

/*===========================================================================

Copyright (c) 2013-2018 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qat3522.mpss/1.11/api/qat3522_physical_device.h#1 $ 
 
when       who   what, where, why
--------   ---   ---------------------------------------------------------------
01/26/16   dbc   Initial Version
============================================================================*/ 
 
/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "rfdevice_qxsw.h"
#include "rfdevice_asm_xsw_intf.h"
#include "rfdevice_physical_device.h"
#include "qat3522_xsw_config_main_ag.h"


class qat3522_physical_device : public rfdevice_physical_device
{
public:
  qat3522_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  qat3522_physical_device* qat3522_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

private:

  /* XSW */
  void create_xsw_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qxsw* qat3522_xsw_obj_ptr;


};

#endif /* QAT3522_PHYSICAL_DEVICE_H */
