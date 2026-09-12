
#ifndef QFE2082FC_PHYSICAL_DEVICE_AG_H
#define QFE2082FC_PHYSICAL_DEVICE_AG_H
/*!
   @file
   qfe2082fc_physical_device_ag.h

   @brief
   qfe2082fc physical device driver

*/

/*===========================================================================

Copyright (c) 2013-2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qfe2082fc.mpss/1.22/api/qfe2082fc_physical_device_ag.h#1 $ 

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
10/02/14   dbc   Implement rfdevice_asm_xsw_intf interface for tracking XSW position
08/20/14   vv    Added support to decouple qcoupler factory to chip specific drivers 
08/14/14   dbc   Add support for tracking XSW position
08/07/14   vv    Added support to decouple qasm/qxsw factory to chip specific drivers
04/23/14   vv    Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"
#include "rfdevice_qcoupler.h"

#include "rfdevice_physical_device.h"
#include "qfe2082fc_asm_config_main_ag.h"
#include "qfe2082fc_coupler_config_main_ag.h"

class qfe2082fc_physical_device : public rfdevice_physical_device
{
public:

  qfe2082fc_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  qfe2082fc_physical_device* qfe2082fc_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* Flag to track cross switch position */
  uint8 cross_switch_mode;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm_2g* qfe2082fc_asm_obj_ptr;

  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qfe2082fc_coupler_obj_ptr;
};
#endif