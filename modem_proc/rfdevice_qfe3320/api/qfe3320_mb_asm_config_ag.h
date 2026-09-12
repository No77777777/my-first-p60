
#ifndef QFE3320_MB_ASM_CONFIG_AG_H
#define QFE3320_MB_ASM_CONFIG_AG_H/*
WARNING: This QFE3320_MB driver is auto-generated.

Generated using: qasm_autogen.pl 
Generated from-  

	File: QFE3320_RFFE_Settings.xlsx 
	Released: 9/10/2015
	Author: rspring
	Revision: 3.09
	Change Note: Added LTE B10 for LTE-U.  NOT an Official band that QFE3320 supports.  This is for internal LTE-U support only.
	Tab: qfe3320_mb_asm_settings

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

$Header: //components/rel/rfdevice_qfe3320.mpss/1.32/api/qfe3320_mb_asm_config_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 04:41:35 $ 

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

 
#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qasm_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif  


boolean rfdevice_qasm_qfe3320_mb_construct_driver_ag
(
  rfdevice_qasm_settings_type* qasm_settings,
  rfdevice_qasm_func_tbl_type* asm_fn_ptrs
);

#ifdef __cplusplus
}
#endif
#endif