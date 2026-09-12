#ifndef QSW2630A_PHYSICAL_DEVICE_H
#define QSW2630A_PHYSICAL_DEVICE_H
/*!
   @file
   qsw2630a_physical_device.h

   @brief
   qsw2630a physical device driver

*/

/*===========================================================================

Copyright (c) 2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qsw2630.mpss/1.11/api/qsw2630a_physical_device.h#1 $ 

when       who    what, where, why
--------   ---    ---------------------------------------------------------------
12/15/16   hzh    Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"
#include "rfdevice_qcoupler.h"

#include "rfdevice_physical_device.h"
#include "qsw2630a_asm_config_main_ag.h"
#include "qsw2630a_coupler_config_main_ag.h"

class qsw2630a_physical_device : public rfdevice_physical_device
{
public:

  qsw2630a_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  qsw2630a_physical_device* qsw2630a_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* Flag to track cross switch position */
  uint8 cross_switch_mode;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm_2g* qsw2630a_asm_obj_ptr;

  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qsw2630a_coupler_obj_ptr;
};
#endif