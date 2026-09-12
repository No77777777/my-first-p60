
#ifndef RFDEVICE_RFFE_COUPLER_CONFIG_MAIN_AG_H
#define RFDEVICE_RFFE_COUPLER_CONFIG_MAIN_AG_H
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

    
#include "rf_rffe_common.h"
#include "rfdevice_rffe_coupler_typedef.h"
#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_config_ag.h" 
#ifdef FEATURE_RF_HAS_TP_CARDS
#error code not present
#endif /* FEATURE_RF_HAS_TP_CARDS */

#ifdef __cplusplus
extern "C" {
#endif  

boolean rfdevice_rffe_coupler_factory
( 
  rfc_phy_device_info_type* cfg,  
  rfdevice_rffe_coupler_settings_type* coupler_settings,
  rfdevice_rffe_coupler_func_tbl_type* coupler_fn_ptrs
);

#ifdef __cplusplus
}
#endif
#endif