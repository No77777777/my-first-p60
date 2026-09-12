
#ifndef QPA6560_ASM_CONFIG_AG_H
#define QPA6560_ASM_CONFIG_AG_H/*
WARNING: This QPA6560 driver is auto-generated.

Generated using: qasm_2g_autogen.pl 
Generated from-  

	File: QPA6560_MLB_RFFE_Settings.xlsx 
	Released: 11/22/2019
	Author: Xinwei Wang
	Revision: 0.5.1
	Change Note: BC0/BC10 ULPM settigns changed to 2stages (without stage bypass)
	Tab: qpa6560_asm_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

  Copyright (c) 2013-2020 Qualcomm Technologies Incorporated. All Rights Reserved

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


$Header: //components/rel/rfdevice_qpa6560.mpss/1.11/api/qpa6560_asm_config_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 05:00:30 $ 

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

 
#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qasm_2g_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif  


boolean rfdevice_qasm_2g_qpa6560_construct_driver_ag
(
  rfdevice_qasm_2g_settings_type* qasm_settings
);

#ifdef __cplusplus
}
#endif
#endif