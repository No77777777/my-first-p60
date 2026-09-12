#ifndef QSW8820B_PHYSICAL_DEVICE_H
#define QSW8820B_PHYSICAL_DEVICE_H
/*!
   @file
   qsw8820b_physical_device.h

   @brief
   qsw8820b physical device driver

*/

/*===========================================================================

Copyright (c) 2018 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qsw8820.mpss/1.11/api/qsw8820b_physical_device.h#1 $ 

when       who    what, where, why
--------   ---    ---------------------------------------------------------------
10/29/18   vp     Added physical tracker support
09/27/18   vp     Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"
#include "rfdevice_qcoupler.h"

#include "rfdevice_physical_device.h"
#include "qsw8820b_asm_config_main_ag.h"
#include "qsw8820b_coupler_config_main_ag.h"

/* Bitmask for bits reserved for logical PAs */
#define QSW8820B_ASM_VOTE_MASK 0xFF
/* Number of bits to reserve for logical PAs */
#define QSW8820B_ASM_VOTE_RESERVED_BITS 8

class qsw8820b_physical_device : public rfdevice_physical_device
{
public:

  qsw8820b_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

   virtual bool vote_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);


  qsw8820b_physical_device* qsw8820b_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* Flag to track cross switch position */
  uint8 cross_switch_mode;

  /* 
    One bit per logical component to track the sleep/wakeup votes 
    Wakeup vote sets the bit, Sleep vote clears the bit
  */
  uint64 sleep_wakeup_vote;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm_2g* qsw8820b_asm_obj_ptr;

  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qsw8820b_coupler_obj_ptr;
};
#endif