/*==============================================================================
  FILE:         sleep_target_cmn.c

  OVERVIEW:     This file provides common target-specific functionality for the
                modem Subsystem.

  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/target/sleep_target_cmn.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"

/*==============================================================================
                             FUNCTION DECLARATIONS
 =============================================================================*/
/** 
 * sleepTarget_wakeupHandler
 * 
 * @brief Null handler for the registered idle timer interrupts
 */
void sleepTarget_wakeupHandler(uint32 param)
{
  return;
}

/**
 * sleepTarget_RPMWakeupHandler
 *
 * @brief Null handler for the RPM wakeup (timed trigger) interrupt.
 */
void sleepTarget_RPMWakeupHandler(uint32 param)
{
  return;
}
