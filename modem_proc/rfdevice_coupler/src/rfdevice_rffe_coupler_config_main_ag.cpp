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

    
#include "rfdevice_rffe_coupler_config_main_ag.h"
#include "rfdevice_rffe_coupler_xmss1t3g0pa_013_typedef_ag.h" 
#ifdef FEATURE_RF_HAS_TP_CARDS
#error code not present
#endif /* FEATURE_RF_HAS_TP_CARDS */

boolean rfdevice_rffe_coupler_factory
( 
  rfc_phy_device_info_type* cfg,   
  rfdevice_rffe_coupler_settings_type* coupler_settings,
  rfdevice_rffe_coupler_func_tbl_type* coupler_fn_ptrs
)
{
  uint32 prd_rev;

   /*Parameter validity check*/
  if ((cfg == NULL) || (coupler_settings == NULL) || (coupler_fn_ptrs == NULL))
  {
    return FALSE;
  }

  prd_rev = cfg->product_rev;

  /*Unmask MSBs of product revision. Used as a bit indicator*/
  prd_rev &= ~RFC_NONFATAL_IF_MISSING_BIT_IND;
  prd_rev &= ~RFC_SKIP_RFFE_DETECT_BIT_IND;

  if (cfg->manufacturer_id == XMSS1T3G0PA_013_COUPLER_MANUFACTURER_ID && cfg->product_id == XMSS1T3G0PA_013_COUPLER_PRODUCT_ID && prd_rev == XMSS1T3G0PA_013_COUPLER_PRODUCT_REVISION)
  {
    return(rfdevice_rffe_coupler_xmss1t3g0pa_013_construct_driver_ag(coupler_settings, coupler_fn_ptrs));
  }		
#ifdef FEATURE_RF_HAS_TP_CARDS
#error code not present
#endif /* FEATURE_RF_HAS_TP_CARDS */

 return FALSE;

}
