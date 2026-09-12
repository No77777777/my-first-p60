#ifndef QSW2820F_PHYSICAL_DEVICE_H
#define QSW2820F_PHYSICAL_DEVICE_H
/*!
   @file
   qsw2820f_physical_device.h

   @brief
   qsw2820f physical device driver

*/

/*===========================================================================

Copyright (c) 2018 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qsw2820.mpss/1.11/api/qsw2820f_physical_device.h#1 $ 

when       who    what, where, why
--------   ---    ---------------------------------------------------------------
08/01/19   dh      Initial version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"
#include "rfdevice_qcoupler.h"

#include "rfdevice_physical_device.h"
#include "qsw2820f_asm_config_main_ag.h"
#include "qsw2820f_coupler_config_main_ag.h"

/* Bitmask for bits reserved for logical PAs */
#define QSW2820F_ASM_VOTE_MASK 0xFF
/* Number of bits to reserve for logical PAs */
#define QSW2820F_ASM_VOTE_RESERVED_BITS 8

class qsw2820f_physical_device : public rfdevice_physical_device
{
public:

  qsw2820f_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

 virtual bool vote_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);

  qsw2820f_physical_device* qsw2820f_physical_device_p;

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

  rfdevice_qasm_2g* qsw2820f_asm_obj_ptr;

  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qsw2820f_coupler_obj_ptr;
};
#endif