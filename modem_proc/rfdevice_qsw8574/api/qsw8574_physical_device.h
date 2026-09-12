#ifndef QSW8574_PHYSICAL_DEVICE_H
#define QSW8574_PHYSICAL_DEVICE_H
/*!
   @file
   qsw8574_physical_device.h

   @brief
   qsw8574 physical device driver

*/

/*===========================================================================

Copyright (c) 2016 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qsw8574.mpss/1.11/api/qsw8574_physical_device.h#1 $ 

when       who    what, where, why
--------   ---    ---------------------------------------------------------------
03/22/16   hzh    Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"

#include "rfdevice_physical_device.h"
#include "qsw8574_asm_config_main_ag.h"

class qsw8574_physical_device : public rfdevice_physical_device
{
public:

  qsw8574_physical_device(rfc_phy_device_info_type* cfg);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
  
  virtual bool validate_self_cal_efs(void);
  
  virtual bool has_common_tab();

  virtual bool vote_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);

  qsw8574_physical_device* qsw8574_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* 
    One bit per logical component to track the sleep/wakeup votes 
    Sleep vote resets the bit
    Wakeup vote sets the bit
  */
  uint64 sleep_wakeup_vote;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm_2g* qsw8574_lb_asm_obj_ptr;
  rfdevice_qasm_2g* qsw8574_mb_asm_obj_ptr;

};

/*enum listing the logical components within the chip*/
typedef enum
{
  QSW8574_LB_ASM=0,
  QSW8574_MB_ASM,
  QSW8574_LOGICAL_INVALID,
}qsw8574_logical;

#endif
