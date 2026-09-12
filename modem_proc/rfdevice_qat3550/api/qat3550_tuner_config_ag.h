
#ifndef QAT3550_TUNER_CONFIG_AG_H
#define QAT3550_TUNER_CONFIG_AG_H/*
WARNING: This QAT3550 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QAT3550_RFFE_Settings.xlsx 
	Released: 9/7/2016
	Author: Dwinslow
	Revision: v0.9
	Change Note: Change for QAT3550 V200 including BBM bit. See third tab of this spreadsheet for mapping of tunecode override to registers
	Tab: qat3550_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QAT3550 TUNER.

Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/qtuner/main/1.11/etc/qtuner_autogen.pl#2 qat3550./main/1.0/etc/qtuner_autogen.pl#1 : dnatesan : 2016/03/01 23:59:33 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qtuner_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif  
boolean rfdevice_qtuner_qat3550_validate_cfg_ag
(
  rfdevice_id_enum_type rf_device_id,
  uint32 product_id,
  uint32 manufacturer_id,
  rfdevice_type_enum_type rf_device_type,
  rfdevice_comm_proto_enum_type rf_device_comm_protocol
);

boolean rfdevice_qtuner_qat3550_construct_driver_ag
(
  rfdevice_qtuner_settings_type* qtuner_settings,
  rfdevice_qtuner_func_tbl_type* qtuner_fn_ptrs,
  rfdevice_qtuner_params_type * qtuner_params
);

#ifdef __cplusplus
}
#endif
#endif