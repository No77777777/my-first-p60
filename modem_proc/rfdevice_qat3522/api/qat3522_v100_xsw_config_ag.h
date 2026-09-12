
#ifndef QAT3522_V100_XSW_CONFIG_AG_H
#define QAT3522_V100_XSW_CONFIG_AG_H/*
WARNING: This QAT3522_V100 driver is auto-generated.

Generated using: qxsw_autogen.pl 
Generated from-  

	File: QAT3522_RFFE_Settings.xlsm 
	Released: 12/14/2016
	Author: dwinslow
	Revision: v2.5
	Change Note: Added ports for isolation and single port settings to both device revisions
	Tab: qat3522_v100_xsw_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

  Copyright (c) 2013-2018 Qualcomm Technologies, Inc.  All Rights Reserved.
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

$Header: //components/rel/rfdevice_qat3522.mpss/1.11/api/qat3522_v100_xsw_config_ag.h#1 $
$Author: pwbldsvc $
$DateTime: 2021/03/09 04:17:39 $

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

 
#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qxsw_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif  


boolean rfdevice_qxsw_qat3522_v100_construct_driver_ag
(
  rfdevice_qxsw_settings_type* qxsw_settings,
  rfdevice_qxsw_func_tbl_type* xsw_fn_ptrs
);

#ifdef __cplusplus
}
#endif
#endif