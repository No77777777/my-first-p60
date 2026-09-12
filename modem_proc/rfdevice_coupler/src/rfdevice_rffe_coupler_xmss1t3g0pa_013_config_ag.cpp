/*
WARNING: This XMSS1T3G0PA_013 driver is auto-generated.

Generated using: coupler_autogen.pl 
Generated from:
	File: rfdevice_coupler.xlsm 
	Released: 8/4/2016
	Author: Add Open Settings
	Revision: v1.0.2
	Change Note: pdutta
	Tab: cpl_xmss1t3g0pa_013
*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

  Copyright (c) 2015 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

$Header:
=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfcom.h"
#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_config_ag.h"
#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_settings_ag.h"
#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_typedef_ag.h"

boolean rfdevice_rffe_coupler_xmss1t3g0pa_013_construct_driver_ag
(
  rfdevice_rffe_coupler_settings_type* coupler_settings,
  rfdevice_rffe_coupler_func_tbl_type* coupler_fn_ptrs
)
{
  boolean status = TRUE;
  /*==========================================================================*/
  /* PORT SETTINGS */
  /*==========================================================================*/
  coupler_settings->num_ports = XMSS1T3G0PA_013_COUPLER_PORT_NUM;

  /*==========================================================================*/
  /* COMMON SETTINGS */
  /*==========================================================================*/

  coupler_settings->common_settings_ag.coupler_trigger.reg_tbl = (const uint16 **)&xmss1t3g0pa_013_coupler_trigger_reg_ptr; 
  coupler_settings->common_settings_ag.coupler_trigger.data_tbl= (const int16 **) &xmss1t3g0pa_013_coupler_trigger_data_ptr; 
  coupler_settings->common_settings_ag.coupler_trigger_script_size = XMSS1T3G0PA_013_COUPLER_TRIGGER_SCRIPT_SIZE;
      
  coupler_settings->common_settings_ag.coupler_init.reg_tbl = NULL; 
  coupler_settings->common_settings_ag.coupler_init.data_tbl= NULL; 
  coupler_settings->common_settings_ag.coupler_init_script_size = 0;
      
  coupler_settings->common_settings_ag.coupler_wakeup.reg_tbl = NULL; 
  coupler_settings->common_settings_ag.coupler_wakeup.data_tbl= NULL; 
  coupler_settings->common_settings_ag.coupler_wakeup_script_size = 0;
      
  coupler_settings->common_settings_ag.coupler_sleep.reg_tbl = NULL; 
  coupler_settings->common_settings_ag.coupler_sleep.data_tbl= NULL; 
  coupler_settings->common_settings_ag.coupler_sleep_script_size = 0;
      
  /*==========================================================================*/
  /* RF COMMUNICATION BUS MAPPING */
  /*==========================================================================*/
  coupler_settings->coupler_rfcom_bus_cfg.extended_cmd = FALSE;
  coupler_settings->coupler_rfcom_bus_cfg.settings.half_rate = FALSE;
  coupler_settings->coupler_rfcom_bus_cfg.settings.rd_delay = FALSE;
  coupler_settings->coupler_rfcom_bus_cfg.settings.rffe_force_ext = FALSE;

  
  /*==========================================================================*/
  /* FUNCTION POINTERS MAPPING */
  /*==========================================================================*/
  
  coupler_fn_ptrs->get_config_data      = rfdevice_rffe_coupler_xmss1t3g0pa_013_get_config_data_ag;
  coupler_fn_ptrs->get_gain_data        = rfdevice_rffe_coupler_xmss1t3g0pa_013_get_gain_data_ag;
  coupler_fn_ptrs->get_filter_data      = rfdevice_rffe_coupler_xmss1t3g0pa_013_get_filter_data_ag;
  coupler_fn_ptrs->get_output_type_data = rfdevice_rffe_coupler_xmss1t3g0pa_013_get_output_type_data_ag;

  return status;  

}

boolean rfdevice_rffe_coupler_xmss1t3g0pa_013_get_config_data_ag
(
  rfdevice_rffe_coupler_ag_settings_type* port_settings, 
  rfdevice_rffe_coupler_cfg_type coupler_cfg
)
{
  boolean ret_val = FALSE;

  if (NULL==port_settings)
  {
    return ret_val;
  }
  
  if(coupler_cfg.direction == RFDEVICE_COUPLER_DIRECTION_FWD)
  {
    port_settings->reg_tbl     = (const uint16 **)xmss1t3g0pa_013_coupler_fwd_reg_list_ag; 
    port_settings->data_tbl    = (const int16 **)xmss1t3g0pa_013_coupler_fwd_data_list_ag; 
    port_settings->script_size = XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE;
    ret_val = TRUE;
  }
       
  if(coupler_cfg.direction == RFDEVICE_COUPLER_DIRECTION_REV)
  {
    port_settings->reg_tbl     = (const uint16 **)xmss1t3g0pa_013_coupler_rev_reg_list_ag; 
    port_settings->data_tbl    = (const int16 **)xmss1t3g0pa_013_coupler_rev_data_list_ag; 
    port_settings->script_size = XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE;
    ret_val = TRUE;
  }
       
  if(coupler_cfg.direction == RFDEVICE_COUPLER_DIRECTION_OPEN)
  {
    port_settings->reg_tbl     = (const uint16 **)xmss1t3g0pa_013_coupler_open_reg_list_ag; 
    port_settings->data_tbl    = (const int16 **)xmss1t3g0pa_013_coupler_open_data_list_ag; 
    port_settings->script_size = XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE;
    ret_val = TRUE;
  }
       
  return ret_val;
}

boolean rfdevice_rffe_coupler_xmss1t3g0pa_013_get_gain_data_ag
(
  rfdevice_rffe_coupler_ag_settings_type* port_settings, 
  rfdevice_rffe_coupler_cfg_type coupler_cfg
)
{
  boolean ret_val = FALSE;

  if (NULL==port_settings)
  {
    return ret_val;
  }
  
  return ret_val;
}

boolean rfdevice_rffe_coupler_xmss1t3g0pa_013_get_filter_data_ag
(
  rfdevice_rffe_coupler_ag_settings_type* port_settings, 
  rfdevice_rffe_coupler_cfg_type coupler_cfg
)
{
  boolean ret_val = FALSE;

  if (NULL==port_settings)
  {
    return ret_val;
  }
  
  return ret_val;
}

boolean rfdevice_rffe_coupler_xmss1t3g0pa_013_get_output_type_data_ag
(
  rfdevice_rffe_coupler_ag_settings_type* port_settings, 
  rfdevice_rffe_coupler_cfg_type coupler_cfg
)
{
  boolean ret_val = FALSE;

  if (NULL==port_settings)
  {
    return ret_val;
  }
  
  return ret_val;
}