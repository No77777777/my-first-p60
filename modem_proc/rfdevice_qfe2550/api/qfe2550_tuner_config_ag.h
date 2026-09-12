
#ifndef QFE2550_TUNER_CONFIG_AG_H
#define QFE2550_TUNER_CONFIG_AG_H/*
WARNING: This QFE2550 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QFE2550_RFFE_Settings.xlsx 
	Released: 2/19/2016
	Author: ycchiou
	Revision: v1.0
	Change Note: Add "tuner_get_tunecode" and "tuner_get_nvg_status"
	Tab: qfe2550_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QFE2550 TUNER.

Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/QFE2550/main/1.0/etc/qtuner_autogen.pl#1 : c_hmaity : 2016/02/04 23:34:38 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qtuner_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif  
boolean rfdevice_qtuner_qfe2550_validate_cfg_ag
(
  rfdevice_id_enum_type rf_device_id,
  uint32 product_id,
  uint32 manufacturer_id,
  rfdevice_type_enum_type rf_device_type,
  rfdevice_comm_proto_enum_type rf_device_comm_protocol
);

boolean rfdevice_qtuner_qfe2550_construct_driver_ag
(
  rfdevice_qtuner_settings_type* qtuner_settings,
  rfdevice_qtuner_func_tbl_type* qtuner_fn_ptrs,
  rfdevice_qtuner_params_type * qtuner_params
);

#ifdef __cplusplus
}
#endif
#endif