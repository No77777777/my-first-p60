
#ifndef QPA8688_ASM_CONFIG_MAIN_AG_H
#define QPA8688_ASM_CONFIG_MAIN_AG_H
/*
WARNING: This QPA8688 driver is auto-generated.

Generated using: qasm_2g_autogen.pl 
Generated from-  

	File: QPA8688_RFFE_Settings.xlsx 
	Released: 4/6/2018
	Author: jose
	Revision: 1.02
	Change Note: Added extended PID
	Tab: qpa8688_asm_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

  Copyright (c) 2013-2017 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.


$Header: //components/rel/rfdevice_qpa8688.mpss/1.11/api/qpa8688_asm_config_main_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 03:51:31 $ 

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

    
#include "rf_rffe_common.h"
#include "rfdevice_qasm_2g_typedef.h"
#include "qpa8688_asm_config_ag.h" 

#ifdef __cplusplus
extern "C" {
#endif  

boolean rfdevice_qasm_2g_qpa8688_validate_n_create_cfg_ag
( 
  rfc_phy_device_info_type* cfg,  
  rfdevice_id_enum_type logical_rf_device_id ,
  uint8 chip_rev,
  rfdevice_qasm_2g_settings_type* qpa_settings
);

#ifdef __cplusplus
}
#endif
#endif