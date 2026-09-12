
#ifndef QAT5655_TUNER_CONFIG_AG_H
#define QAT5655_TUNER_CONFIG_AG_H/*
WARNING: This QAT5655 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QAT5655_RFFE_Settings.xlsx 
	Released: 
	Author: 
	Revision: 
	Change Note: 
	Tab: qat5655_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QAT5655 TUNER.

Copyright (c) 2017-2024 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/qtuner/main/1.11/etc/qtuner_autogen.pl#2 qat5655./main/1.0/etc/qtuner_autogen.pl#1 : dnatesan : 2016/03/01 23:59:33 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qtuner_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif  
boolean rfdevice_qtuner_qat5655_validate_cfg_ag
(
  rfdevice_id_enum_type rf_device_id,
  uint32 product_id,
  uint32 manufacturer_id,
  rfdevice_type_enum_type rf_device_type,
  rfdevice_comm_proto_enum_type rf_device_comm_protocol
);

boolean rfdevice_qtuner_qat5655_construct_driver_ag
(
  rfdevice_qtuner_settings_type* qtuner_settings,
  rfdevice_qtuner_func_tbl_type* qtuner_fn_ptrs,
  rfdevice_qtuner_params_type * qtuner_params
);

#ifdef __cplusplus
}
#endif
#endif