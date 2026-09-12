
#ifndef QFE1035_PHYSICAL_DEVICE_AG_H
#define QFE1035_PHYSICAL_DEVICE_AG_H
/*!
   @file
   qfe1035_physical_device_ag.h

   @brief
   qfe1035 physical device driver

*/

/*===========================================================================

Copyright (c) 2013-2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qfe1035.mpss/1.22/api/qfe1035_physical_device_ag.h#1 $ 

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

#include "rfdevice_qasm.h"
#include "rfdevice_qcoupler.h"
#include "rfdevice_qxsw.h"
#include "rfdevice_asm_xsw_intf.h"


#include "rfdevice_physical_device.h"
#include "qfe1035_asm_config_main_ag.h"
#include "qfe1035_xsw_config_main_ag.h"
#include "qfe1035_coupler_config_main_ag.h"

class qfe1035_physical_device : public rfdevice_physical_device, public rfdevice_asm_xsw_intf
{
public:

  qfe1035_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  virtual bool get_asm_xsw_config(rfc_logical_device_info_type *logical_device_cfg, uint8 *xsw_mode);

  virtual bool save_asm_xsw_config(rfc_logical_device_info_type *logical_device_cfg, uint8 xsw_mode);

  virtual bool set_xsw_port_config(rfc_logical_device_info_type *logical_device_cfg, asm_xsw_port_config_info_type *xsw_port_info);

  virtual bool get_xsw_port_config(rfc_logical_device_info_type *logical_device_cfg, asm_xsw_port_config_info_type *xsw_port_info);

  qfe1035_physical_device* qfe1035_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* Flag to track cross switch position */
  uint8 cross_switch_mode;

  /* Cross Switch port info */
  asm_xsw_port_config_info_type cross_switch_port_config;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm* qfe1035_asm_obj_ptr;

  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qfe1035_coupler_obj_ptr;

  /* XSW */
  void create_xsw_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qxsw* qfe1035_xsw_obj_ptr;

};
#endif