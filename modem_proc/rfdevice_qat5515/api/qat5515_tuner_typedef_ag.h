
#ifndef QAT5515_TUNER_TYPEDEF_AG_H
#define QAT5515_TUNER_TYPEDEF_AG_H
/*
WARNING: This QAT5515 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QAT5515_RFFE_Settings.xlsx 
	Released: 
	Author: 
	Revision: 
	Change Note: 
	Tab: qat5515_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QAT5515 TUNER.

Copyright (c) 2017-2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/qtuner/main/1.11/etc/qtuner_autogen.pl#2 qat5515./main/1.0/etc/qtuner_autogen.pl#1 : dnatesan : 2016/03/01 23:59:33 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "comdef.h"
#ifdef __cplusplus
extern "C" {
#endif  

/* Device Identifiers */ 
#define QAT5515_TUNER_MANUFACTURER_ID 0x217
#define QAT5515_TUNER_MANUFACTURER_ID1 0x217
#define QAT5515_TUNER_PRODUCT_ID 0x689
#define QAT5515_TUNER_PRODUCT_ID1 0x68A
#define QAT5515_TUNER_CHIP_REV 
#define QAT5515_TUNER_CHIP_REV_ADDR 
#define QAT5515_TUNER_INSTANCE 0

/* Tuner common settings */
#define QAT5515_TUNER_PRECONFIG_SCRIPT_SIZE 2
#define QAT5515_TUNER_INIT_SCRIPT_SIZE 1
#define QAT5515_TUNER_DISABLE_SCRIPT_SIZE 1
#define QAT5515_TUNER_TUNE_CODE_SCRIPT_SIZE 1

/* Tuner Device specific settings */
#define QAT5515_TUNER_TUNE_CODE_SIZE 1
#define QAT5515_ALGO_MAP 5

#ifdef __cplusplus
}
#endif
#endif