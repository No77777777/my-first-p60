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

#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_settings_ag.h"
  /*==========================================================================*/
  /* PORT AG TABLES */
  /*==========================================================================*/
  
/*===================== FWD ======================================================*/  
const uint16 xmss1t3g0pa_013_coupler_fwd_reg_list_internal_ag[XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE] = 
{
  /* Format: <address>, <default>, <type>*/
  0x00, /* 0x00, CPL_CTL*/
};
const int16 xmss1t3g0pa_013_coupler_fwd_port_0_data_list_ag[XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x01, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_fwd_port_1_data_list_ag[XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x04, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_fwd_port_2_data_list_ag[XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x06, /* 0x00*/
};
/*===================== REV ======================================================*/  
const uint16 xmss1t3g0pa_013_coupler_rev_reg_list_internal_ag[XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE] = 
{
  /* Format: <address>, <default>, <type>*/
  0x00, /* 0x00, CPL_CTL*/
};
const int16 xmss1t3g0pa_013_coupler_rev_port_0_data_list_ag[XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x3, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_rev_port_1_data_list_ag[XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x00, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_rev_port_2_data_list_ag[XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x00, /* 0x00*/
};
/*===================== OPEN ======================================================*/  
const uint16 xmss1t3g0pa_013_coupler_open_reg_list_internal_ag[XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE] = 
{
  /* Format: <address>, <default>, <type>*/
  0x00, /* 0x00, CPL_CTL*/
};
const int16 xmss1t3g0pa_013_coupler_open_port_0_data_list_ag[XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x06, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_open_port_1_data_list_ag[XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x06, /* 0x00*/
};
const int16 xmss1t3g0pa_013_coupler_open_port_2_data_list_ag[XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x06, /* 0x00*/
};
  /*==========================================================================*/
  /* COMMON SETTINGS */
  /*==========================================================================*/
  
/*===================== TRIGGER ======================================================*/  
const uint16 xmss1t3g0pa_013_coupler_trigger_reg_list_ag[XMSS1T3G0PA_013_COUPLER_TRIGGER_SCRIPT_SIZE] = 
{
  /* Format: <address>, <default>, <type>*/
  0x1C, /* 0x00, TRIGGER*/
};
const int16 xmss1t3g0pa_013_coupler_trigger_data_list_ag[XMSS1T3G0PA_013_COUPLER_TRIGGER_SCRIPT_SIZE] = 
{
  /* Format: <value>,  <address>  */
  0x01, /* 0x1C*/
};
const uint16 *xmss1t3g0pa_013_coupler_trigger_reg_ptr = (const uint16 *)xmss1t3g0pa_013_coupler_trigger_reg_list_ag;
const int16 *xmss1t3g0pa_013_coupler_trigger_data_ptr = (const int16 *)xmss1t3g0pa_013_coupler_trigger_data_list_ag;

/*===================== INIT ======================================================*/
const uint16 *xmss1t3g0pa_013_coupler_init_reg_ptr = NULL;
const int16 *xmss1t3g0pa_013_coupler_init_data_ptr = NULL;

/*===================== WAKEUP ======================================================*/
const uint16 *xmss1t3g0pa_013_coupler_wakeup_reg_ptr = NULL;
const int16 *xmss1t3g0pa_013_coupler_wakeup_data_ptr = NULL;

/*===================== SLEEP ======================================================*/
const uint16 *xmss1t3g0pa_013_coupler_sleep_reg_ptr = NULL;
const int16 *xmss1t3g0pa_013_coupler_sleep_data_ptr = NULL;

  /*==========================================================================*/
  /* PORT AGGREGATION */
  /*==========================================================================*/
  
/*===================== FWD_REG_LIST ======================================================*/  
const uint16 *xmss1t3g0pa_013_coupler_fwd_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_fwd_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_fwd_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_fwd_reg_list_internal_ag[0],
};
/*===================== FWD_DATA_LIST ======================================================*/  

const int16 *xmss1t3g0pa_013_coupler_fwd_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_fwd_port_0_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_fwd_port_1_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_fwd_port_2_data_list_ag[0],
};
/*===================== REV_REG_LIST ======================================================*/  
const uint16 *xmss1t3g0pa_013_coupler_rev_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_rev_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_rev_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_rev_reg_list_internal_ag[0],
};
/*===================== REV_DATA_LIST ======================================================*/  

const int16 *xmss1t3g0pa_013_coupler_rev_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_rev_port_0_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_rev_port_1_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_rev_port_2_data_list_ag[0],
};
/*===================== OPEN_REG_LIST ======================================================*/  
const uint16 *xmss1t3g0pa_013_coupler_open_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_open_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_open_reg_list_internal_ag[0],
  &xmss1t3g0pa_013_coupler_open_reg_list_internal_ag[0],
};
/*===================== OPEN_DATA_LIST ======================================================*/  

const int16 *xmss1t3g0pa_013_coupler_open_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM] = 
{
  &xmss1t3g0pa_013_coupler_open_port_0_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_open_port_1_data_list_ag[0],
  &xmss1t3g0pa_013_coupler_open_port_2_data_list_ag[0],
};