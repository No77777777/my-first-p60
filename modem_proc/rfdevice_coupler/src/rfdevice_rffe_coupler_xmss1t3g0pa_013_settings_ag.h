
#ifndef RFDEVICE_RFFE_COUPLER_XMSS1T3G0PA_013_SETTINGS_AG_H
#define RFDEVICE_RFFE_COUPLER_XMSS1T3G0PA_013_SETTINGS_AG_H/*
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

#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_typedef_ag.h"
#ifdef __cplusplus
extern "C" {
#endif      
  /*==========================================================================*/
  /* PORT SPECIFIC SETTINGS */
  /*==========================================================================*/

/* fwd */
extern const uint16 *xmss1t3g0pa_013_coupler_fwd_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

extern const int16 *xmss1t3g0pa_013_coupler_fwd_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

/* rev */
extern const uint16 *xmss1t3g0pa_013_coupler_rev_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

extern const int16 *xmss1t3g0pa_013_coupler_rev_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

/* open */
extern const uint16 *xmss1t3g0pa_013_coupler_open_reg_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

extern const int16 *xmss1t3g0pa_013_coupler_open_data_list_ag[XMSS1T3G0PA_013_COUPLER_PORT_NUM];

  /*==========================================================================*/
  /* COMMON SETTINGS */
  /*==========================================================================*/

/* TRIGGER*/
extern const uint16 *xmss1t3g0pa_013_coupler_trigger_reg_ptr;
extern const int16 *xmss1t3g0pa_013_coupler_trigger_data_ptr;

/* INIT */
extern const uint16 *xmss1t3g0pa_013_coupler_init_reg_ptr;
extern const int16 *xmss1t3g0pa_013_coupler_init_data_ptr;

/* WAKEUP*/
extern const uint16 *xmss1t3g0pa_013_coupler_wakeup_reg_ptr;
extern const int16 *xmss1t3g0pa_013_coupler_wakeup_data_ptr;

/* SLEEP*/
extern const uint16 *xmss1t3g0pa_013_coupler_sleep_reg_ptr;
extern const int16 *xmss1t3g0pa_013_coupler_sleep_data_ptr;


#ifdef __cplusplus
}
#endif
#endif