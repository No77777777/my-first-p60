/*!
  @file
  global_defs.h

  @brief
  Global definitions and includes

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/inc/common/global_defs.h#1 $
==============================================================================*/

#ifndef GLOBAL_DEFS_H_
#define GLOBAL_DEFS_H_

/**
 * @brief   The maximum value for uint16_t. 
*/
#define UC_UINT16_MAX  0xFFFF


/**
 * @brief   Disable the feature to profile CPU (turning on 
 *          reduces code size)
*/
//#define UC_FEATURE_CPU_PROFILING_DISABLED

/**
 * @brief   Disable the feature to profile CPU usage in IRQ handlers that are invoked while we are in wfi (turning on 
 *          reduces code size)
*/
#define UC_FEATURE_CPU_PROFILING_IRQ_HDLR_DISABLED

/**
 * @brief   Enable PC Profiling
*/
#ifdef UC_FEATURE_CPU_PROFILING_IRQ_HDLR_DISABLED
#define UC_FEATURE_PC_PROFILING
#endif

/**
 * @brief   Enable debug logging so logging info is sent to CPU
*/
#define UC_ENABLE_DEBUG_LOGGING

/**
 * @brief   Collect timestamps about various events
*/
#define UC_FEATURE_COLLECT_TIMESTAMPS

#define UC_FIRMWARE_VERSION 0x01

/*==============================================================================

                                GLOBAL INCLUDE FILES

==============================================================================*/
#include "target_defs.h"
#include "stdint.h"
#include "comdef.h"
#include "interrupts.h"
//#include "inline_asm.h"
//#include "services.h"
#include "hal.h"
#include "msmhwioreg.h"
#include "HALhwio_ipa.h"


#endif /* GLOBAL_DEFS_H_ */
