
#ifndef QFE1035_XSW_CONFIG_MAIN_AG_H
#define QFE1035_XSW_CONFIG_MAIN_AG_H
/*
WARNING: This QFE1035_V50 driver is auto-generated.

Generated using: qxsw_autogen.pl 
Generated from-  

	File: QFE1035_RFFE_Settings.xlsm 
	Released: 11/12/2014
	Author: Ryan Spring
	Revision: 10.36
	Change Note: Added XSW/ASDIV feature on v40 ASM/XSW
	Tab: qfe1035_v50_xsw_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

  Copyright (c) 2013-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
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

$Header: //components/rel/rfdevice_qfe1035.mpss/1.22/api/qfe1035_xsw_config_main_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 04:04:23 $

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

    
#include "rf_rffe_common.h"
#include "rfdevice_qxsw_typedef.h"
#include "qfe1035_v40_xsw_config_ag.h" 
#include "qfe1035_v50_xsw_config_ag.h" 

#ifdef __cplusplus
extern "C" {
#endif  

boolean rfdevice_qxsw_qfe1035_validate_n_create_cfg_ag
( 
  rfc_phy_device_info_type* cfg,   
  rfdevice_id_enum_type logical_rf_device_id ,  
  uint8 chip_rev, 
  rfdevice_qxsw_settings_type* qxsw_settings,
  rfdevice_qxsw_func_tbl_type* xsw_fn_ptrs
);

#ifdef __cplusplus
}
#endif
#endif