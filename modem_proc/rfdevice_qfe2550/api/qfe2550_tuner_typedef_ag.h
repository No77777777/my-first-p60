
#ifndef QFE2550_TUNER_TYPEDEF_AG_H
#define QFE2550_TUNER_TYPEDEF_AG_H
/*
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
#ifdef __cplusplus
extern "C" {
#endif  

/* Device Identifiers */
#define QFE2550_TUNER_MANUFACTURER_ID 0x217
#define QFE2550_TUNER_PRODUCT_ID 0x4
#define QFE2550_TUNER_CHIP_REV 
#define QFE2550_TUNER_CHIP_REV_ADDR 
#define QFE2550_TUNER_INSTANCE 0

/* Tuner common settings */
#define QFE2550_TUNER_PRECONFIG_SCRIPT_SIZE 11
#define QFE2550_TUNER_INIT_SCRIPT_SIZE 3
#define QFE2550_TUNER_DISABLE_SCRIPT_SIZE 3
#define QFE2550_TUNER_TUNE_CODE_SCRIPT_SIZE 3

/* Tuner Device specific settings */
#define QFE2550_TUNER_TUNE_CODE_SIZE 2
#define QFE2550_ALGO_MAP 5

#ifdef __cplusplus
}
#endif
#endif