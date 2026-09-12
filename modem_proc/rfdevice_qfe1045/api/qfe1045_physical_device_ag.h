
#ifndef QFE1045_PHYSICAL_DEVICE_AG_H
#define QFE1045_PHYSICAL_DEVICE_AG_H
/*!
   @file
   qfe1045_physical_device_ag.h

   @brief
   qfe1045 physical device driver

*/

/*===========================================================================

Copyright (c) 2013-2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_qfe1045.mpss/1.22/api/qfe1045_physical_device_ag.h#1 $ 
 
when       who   what, where, why
--------   ---   ---------------------------------------------------------------
10/02/14   dbc   Implement rfdevice_asm_xsw_intf interface for tracking XSW position
08/20/14   vv    Added support to decouple qcoupler factory to chip specific drivers  
08/14/14   dbc   Added support for getting/setting XSW position 
08/07/14   vv    Added support to decouple qasm/qxsw factory to chip specific drivers  
06/19/14   vv    Added support for physical device based sleep/wakeup voting
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
#include "qfe1045_asm_config_main_ag.h"
#include "qfe1045_xsw_config_main_ag.h"
#include "qfe1045_coupler_config_main_ag.h"


class qfe1045_physical_device : public rfdevice_physical_device, public rfdevice_asm_xsw_intf
{
public:
  qfe1045_physical_device(rfc_phy_device_info_type* cfg);

  virtual bool load_self_cal(const char* str);

  virtual bool perform_self_cal(const char* str);
    
  virtual rfdevice_logical_component* get_component(rfc_logical_device_info_type *logical_device_cfg);
  
  virtual bool validate_self_cal_efs(void);

  virtual bool vote_sleep_wakeup(rfc_logical_device_info_type *logical_device_cfg, bool sleep_wakeup);

  virtual bool get_asm_xsw_config(rfc_logical_device_info_type *logical_device_cfg, uint8 *xsw_mode);

  virtual bool save_asm_xsw_config(rfc_logical_device_info_type *logical_device_cfg, uint8 xsw_mode);

  virtual bool set_xsw_port_config(rfc_logical_device_info_type *logical_device_cfg, asm_xsw_port_config_info_type *xsw_port_info);

  virtual bool get_xsw_port_config(rfc_logical_device_info_type *logical_device_cfg, asm_xsw_port_config_info_type *xsw_port_info);

  qfe1045_physical_device* qfe1045_physical_device_p;

  rfc_phy_device_info_type* phy_device_cfg;

  uint8 chip_rev;

/* 
  One bit per logical component to track the sleep/wakeup votes 
  Sleep vote resets the bit
  Wakeup vote sets the bit
*/
  uint64 sleep_wakeup_vote;

  /* variables to track the MB and HB cross switch position */
  uint8 qfe1045_mb_xsw_mode;
  uint8 qfe1045_hb_xsw_mode;

  /* Cross Switch port info */
  asm_xsw_port_config_info_type cross_switch_port_config_mb;
  asm_xsw_port_config_info_type cross_switch_port_config_hb;

private:

  /* ASM */
  void create_asm_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qasm* qfe1045_mb_asm_obj_ptr;

  rfdevice_qasm* qfe1045_hb_asm_obj_ptr;


  /* COUPLER */
  void create_coupler_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qcoupler* qfe1045_mb_coupler_obj_ptr;

  rfdevice_qcoupler* qfe1045_hb_coupler_obj_ptr;


  /* XSW */
  void create_xsw_object(rfc_logical_device_info_type *logical_device_cfg);

  rfdevice_qxsw* qfe1045_mb_xsw_obj_ptr;

  rfdevice_qxsw* qfe1045_hb_xsw_obj_ptr;



};

/*enum listing the logical components withtin the chip*/
typedef enum
{
  QFE1045_MB_ASM=0,
  QFE1045_HB_ASM,
  QFE1045_MB_COUPLER,
  QFE1045_HB_COUPLER,
  QFE1045_MB_XSW,
  QFE1045_HB_XSW,
  QFE1045_LOGICAL_INVALID,
}qfe1045_logical;


#endif
