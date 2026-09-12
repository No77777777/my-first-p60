#ifndef RFDEVICE_RFFE_COUPLER_XMSS1T3G0PA_013_TYPEDEF_AG_H
#define RFDEVICE_RFFE_COUPLER_XMSS1T3G0PA_013_TYPEDEF_AG_H
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

#include "comdef.h"
#ifdef __cplusplus
extern "C" {
#endif   

/* Device Identifiers */
#define XMSS1T3G0PA_013_COUPLER_MANUFACTURER_ID 0x020C
#define XMSS1T3G0PA_013_COUPLER_PRODUCT_ID 0x83

#define XMSS1T3G0PA_013_COUPLER_CHIP_REV 
#define XMSS1T3G0PA_013_COUPLER_CHIP_REV_ADDR 
#define XMSS1T3G0PA_013_COUPLER_PRODUCT_REVISION 0
#define XMSS1T3G0PA_013_COUPLER_INSTANCE 
#define RFDEVICE_COUPLER_REG_INVALID -1

/* Port specific settings */
#define XMSS1T3G0PA_013_COUPLER_FWD_SCRIPT_SIZE 1
#define XMSS1T3G0PA_013_COUPLER_REV_SCRIPT_SIZE 1
#define XMSS1T3G0PA_013_COUPLER_OPEN_SCRIPT_SIZE 1
#define XMSS1T3G0PA_013_COUPLER_GAIN_0_SCRIPT_SIZE 0
#define XMSS1T3G0PA_013_COUPLER_GAIN_1_SCRIPT_SIZE 
#define XMSS1T3G0PA_013_COUPLER_GAIN_2_SCRIPT_SIZE 
#define XMSS1T3G0PA_013_COUPLER_GAIN_3_SCRIPT_SIZE 
#define XMSS1T3G0PA_013_COUPLER_FILTER_SCRIPT_SIZE 0
#define XMSS1T3G0PA_013_COUPLER_OUTPUT_TYPE_0_SCRIPT_SIZE 0
#define XMSS1T3G0PA_013_COUPLER_OUTPUT_TYPE_1_SCRIPT_SIZE 

/* Common settings */
#define XMSS1T3G0PA_013_COUPLER_TRIGGER_SCRIPT_SIZE 1
#define XMSS1T3G0PA_013_COUPLER_INIT_SCRIPT_SIZE 0
#define XMSS1T3G0PA_013_COUPLER_WAKEUP_SCRIPT_SIZE 0
#define XMSS1T3G0PA_013_COUPLER_SLEEP_SCRIPT_SIZE 0

typedef enum
{
  XMSS1T3G0PA_013_COUPLER_PORT_0,
  XMSS1T3G0PA_013_COUPLER_PORT_1,
  XMSS1T3G0PA_013_COUPLER_PORT_2,
  XMSS1T3G0PA_013_COUPLER_PORT_NUM,
  XMSS1T3G0PA_013_COUPLER_PORT_INVALID,
}xmss1t3g0pa_013_coupler_port_enum_type;

#ifdef __cplusplus
}
#endif
#endif