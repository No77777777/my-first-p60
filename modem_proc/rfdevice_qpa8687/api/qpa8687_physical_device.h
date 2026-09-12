#ifndef QPA8687_PHYSICAL_DEVICE_AG_H
#define QPA8687_PHYSICAL_DEVICE_AG_H
/*!
   @file
   qpa8687_physical_device_ag.h

   @brief
   qpa8687 physical device driver

*/

/*===========================================================================

Copyright (c) 2013-2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qpa8687.mpss/1.11/api/qpa8687_physical_device.h#1 $ 

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
05/09/16   dbc   Enable EFS PA Support
02/24/16   bz    Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "rfdevice_qpa_4g.h"

#include "rfdevice_physical_device.h"
#include "qpa8687_pa_config_main_ag.h"

class qpa8687_physical_device : public rfdevice_physical_device
{
public:
  qpa8687_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  virtual bool supports_efs_dat(void);

  qpa8687_physical_device* qpa8687_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

private:

  /* PA */
  void create_pa_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qpa_4g* qpa8687_pa_obj_ptr;

};
#endif
