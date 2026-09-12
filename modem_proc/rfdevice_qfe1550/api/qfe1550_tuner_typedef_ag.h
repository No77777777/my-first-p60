
#ifndef QFE1550_TUNER_TYPEDEF_AG_H
#define QFE1550_TUNER_TYPEDEF_AG_H
/*
WARNING: This QFE1550 driver is auto-generated.

Generated using: qtuner_autogen.pl 
Generated from-  

	File: QFE1550_RFFE_Settings.xlsx 
	Released: 4/11/2016
	Author: ycchiou
	Revision: v0.3
	Change Note: Update 0x24 = 0x01 (previously 0x24 = 0x26) on "INI" and remove rl_disable
	Tab: qfe1550_tuner_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QFE1550 TUNER.

Copyright (c) 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfdevice/qtuner/main/1.11/etc/qtuner_autogen.pl#2 qfe1550./main/1.0/etc/qtuner_autogen.pl#1 : dnatesan : 2016/03/01 23:59:33 60============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "comdef.h"
#ifdef __cplusplus
extern "C" {
#endif  

/* Device Identifiers */
#define QFE1550_TUNER_MANUFACTURER_ID 0x217
#define QFE1550_TUNER_PRODUCT_ID 0x02
#define QFE1550_TUNER_CHIP_REV 
#define QFE1550_TUNER_CHIP_REV_ADDR 
#define QFE1550_TUNER_INSTANCE 0

/* Tuner common settings */
#define QFE1550_TUNER_PRECONFIG_SCRIPT_SIZE 3
#define QFE1550_TUNER_INIT_SCRIPT_SIZE 3
#define QFE1550_TUNER_DISABLE_SCRIPT_SIZE 3
#define QFE1550_TUNER_TUNE_CODE_SCRIPT_SIZE 3

/* Tuner Device specific settings */
#define QFE1550_TUNER_TUNE_CODE_SIZE 2
#define QFE1550_ALGO_MAP 5

#ifdef __cplusplus
}
#endif
#endif