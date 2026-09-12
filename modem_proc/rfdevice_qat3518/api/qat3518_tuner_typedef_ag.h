
#ifndef QAT3518_TUNER_TYPEDEF_AG_H
#define QAT3518_TUNER_TYPEDEF_AG_H
/*
WARNING: This QAT3518 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QAT3518_RFFE_Settings.xlsx 
	Released: 7/24/2017
	Author: dwinslow
	Revision: v 1.2
	Change Note: Added 2nd variant PID (0x585 and 0x586), changed default USID from 9 to 7.
	Tab: qat3518_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QAT3518 TUNER.

Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/qtuner/main/1.11/etc/qtuner_autogen.pl#2 qat3518./main/1.0/etc/qtuner_autogen.pl#1 : dnatesan : 2016/03/01 23:59:33 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "comdef.h"
#ifdef __cplusplus
extern "C" {
#endif  

/* Device Identifiers */ 
#define QAT3518_TUNER_MANUFACTURER_ID 0x217
#define QAT3518_TUNER_MANUFACTURER_ID1 0x217
#define QAT3518_TUNER_PRODUCT_ID 0x585
#define QAT3518_TUNER_PRODUCT_ID1 0x586
#define QAT3518_TUNER_CHIP_REV 
#define QAT3518_TUNER_CHIP_REV_ADDR 
#define QAT3518_TUNER_INSTANCE 0

/* Tuner common settings */
#define QAT3518_TUNER_PRECONFIG_SCRIPT_SIZE 2
#define QAT3518_TUNER_INIT_SCRIPT_SIZE 1
#define QAT3518_TUNER_DISABLE_SCRIPT_SIZE 1
#define QAT3518_TUNER_TUNE_CODE_SCRIPT_SIZE 1

/* Tuner Device specific settings */
#define QAT3518_TUNER_TUNE_CODE_SIZE 1
#define QAT3518_ALGO_MAP 5

#ifdef __cplusplus
}
#endif
#endif