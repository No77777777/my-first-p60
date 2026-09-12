#ifndef QFE1550_PHYSICAL_DEVICE_AG_H
#define QFE1550_PHYSICAL_DEVICE_AG_H
/*
Generated using: qfe_physical_device_autogen.pl 
Generated from-  

	File: QFE1550_RFFE_Settings.xlsx 
	Released: 
	Author: 
	Revision: 
	Change Note: 

*/

/*=============================================================================

          RF DEVICE QFE1550 PHYSICAL DEVICE

GENERAL DESCRIPTION
  This file captures the configuration of QFE1550 CHIP.

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_qfe1550.mpss/1.22/api/qfe1550_physical_device.h#1 $
  $DateTime: 2021/03/09 04:15:38 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   --------------------------------------------------------------- 
02/17/16   ndb   Remove the chip level dependency in the common qtuner driver
08/13/15   sr    Initial File
============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_antenna_tuner.h"
#include "rfdevice_qtuner.h"
#include "rfc_common.h"

#include "rfdevice_physical_device.h"
#include "qfe1550_tuner_config_ag.h"


class qfe1550_physical_device : public rfdevice_physical_device
{
public:

  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);

  qfe1550_physical_device* qfe1550_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  qfe1550_physical_device(rfc_phy_device_info_type* cfg_req,
  	                  boolean ext_cmd = TRUE );

  ~qfe1550_physical_device();

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);

private:
  /* Device config */
  rfc_device_cfg_info_type* cfg;
  boolean extended_cmd;

  /* QTUNER */
  void create_tuner_object( rfc_logical_device_info_type *logical_device_info ); 

  rfdevice_antenna_tuner* qfe1550_qtuner_obj_ptr;
  #define QFE1550_QTUNER_INSTANCE 0

};
#endif