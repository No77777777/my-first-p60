
#ifndef QFE2082FC_V200_ASM_CONFIG_AG_H
#define QFE2082FC_V200_ASM_CONFIG_AG_H/*
WARNING: This QFE2082FC_V200 driver is auto-generated.

Generated using: qasm_2g_autogen.pl 
Generated from-  

	File: QFE2082FC_RFFE_Settings.xlsm 
	Released: 8/28/2015
	Author: Xinwei Wang
	Revision: 8
	Change Note: Correct v200 B7 (TRX12) seeting from 0x0B ==> 0xD3
	Tab: qfe2082fc_v200_asm_settings

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

$Header: //components/rel/rfdevice_qfe2082fc.mpss/1.22/api/qfe2082fc_v200_asm_config_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 03:42:36 $ 

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


boolean rfdevice_qasm_2g_qfe2082fc_v200_construct_driver_ag
(
  rfdevice_qasm_2g_settings_type* qasm_settings
);

#ifdef __cplusplus
}
#endif
#endif