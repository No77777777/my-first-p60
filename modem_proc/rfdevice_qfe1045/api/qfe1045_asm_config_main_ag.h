
#ifndef QFE1045_ASM_CONFIG_MAIN_AG_H
#define QFE1045_ASM_CONFIG_MAIN_AG_H
/*
WARNING: This QFE1045_HB_V50 driver is auto-generated.

Generated using: qasm_autogen.pl 
Generated from-  

	File: QFE1045_RFFE_Settings.xlsm 
	Released: 5/20/2015
	Author: Ryan Spring
	Revision: 12.67
	Change Note: Deleted CPL setting in MB/HB ASM v40/v50 in Enable TX row.  This fixes THOR ULCA issue
	Tab: qfe1045_hb_v50_asm_settings

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

$Header: //components/rel/rfdevice_qfe1045.mpss/1.22/api/qfe1045_asm_config_main_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 04:01:47 $ 

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

    
#include "rf_rffe_common.h"
#include "rfdevice_qasm_typedef.h"
#include "qfe1045_mb_v00_asm_config_ag.h" 
#include "qfe1045_hb_v00_asm_config_ag.h" 
#include "qfe1045_mb_v40_asm_config_ag.h" 
#include "qfe1045_mb_v50_asm_config_ag.h" 
#include "qfe1045_hb_v40_asm_config_ag.h" 
#include "qfe1045_hb_v50_asm_config_ag.h" 

#ifdef __cplusplus
extern "C" {
#endif  

boolean rfdevice_qasm_qfe1045_validate_n_create_cfg_ag
( 
  rfc_phy_device_info_type* cfg,  
  rfdevice_id_enum_type logical_rf_device_id ,
  uint8 chip_rev,
  rfdevice_qasm_settings_type* qpa_settings,
  rfdevice_qasm_func_tbl_type* pa_fn_ptrs
);

#ifdef __cplusplus
}
#endif
#endif