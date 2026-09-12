
#ifndef QPA6560_HB_PHYSICAL_DEVICE_AG_H
#define QPA6560_HB_PHYSICAL_DEVICE_AG_H
/*!
   @file
   qpa6560_hb_physical_device_ag.h

   @brief
   qpa6560_hb physical device driver

*/

/*===========================================================================

Copyright (c) 2017 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qpa6560.mpss/1.11/api/qpa6560_hb_physical_device.h#1 $ 

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
10/05/17   dbc   Initial Version
============================================================================*/ 

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfdevice_qasm_2g.h"
#include "rfdevice_qpa_4g.h"

#include "rfdevice_physical_device.h"
#include "qpa6560_hb_pa_config_main_ag.h"
#include "qpa6560_hb_asm_config_main_ag.h"

/* Bitmask for bits reserved for logical PAs */
#define QPA6560_HB_PA_VOTE_MASK 0xFF
/* Number of bits to reserve for logical PAs */
#define QPA6560_HB_PA_VOTE_RESERVED_BITS 8

class qpa6560_hb_physical_device : public rfdevice_physical_device
{
public:
  qpa6560_hb_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  virtual bool supports_efs_dat(void);

  virtual bool has_common_tab();

  virtual bool vote_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);

  virtual bool vote_common_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);

  qpa6560_hb_physical_device* qpa6560_hb_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

  /* 
    One bit per logical component to track the sleep/wakeup votes 
    Wakeup vote sets the bit, Sleep vote clears the bit
  */
  uint64 sleep_wakeup_vote;

private:

  /* PA */
  void create_pa_object(rfc_logical_device_info_type *logical_device_cfg);
  rfdevice_qpa_4g* qpa6560_hb_pa_obj_ptr;

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);
  rfdevice_qasm_2g* qpa6560_hb_asm_obj_ptr;
  


};
#endif
