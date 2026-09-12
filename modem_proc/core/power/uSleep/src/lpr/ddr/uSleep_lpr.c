/*==============================================================================
  FILE:         uSleep_lpr.c

  OVERVIEW:     This file contains the low power modes related support functions
                contained in normal mode 

  DEPENDENCIES: Object file generated from source is marked as island section
  
                Copyright (c) 2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/lpr/ddr/uSleep_lpr.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "CoreVerify.h"
#include "uSleep_lpri.h"
#include "uSleep_ddr_log.h"
#include "uSleep_target.h"

/*==============================================================================
                              EXTERNAL VARIABLES
 =============================================================================*/
extern const uint32 g_uSleepModeTableSize;

/*==============================================================================
                          EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/* 
 * uSleepLPR_init
 */
void uSleepLPR_init(void)
{
  uint32      nLPR;
  uSleep_LPR  *LPRPtr;

  uSleepDDRLog_printf(USLEEP_DDR_LOG_LEVEL_TRANSITION, 0, "Init uSleep LPR");

  CORE_VERIFY(USLEEP_NUMBER_LOW_POWER_MODES == g_uSleepModeTableSize);

  /* Run the default init first, if it exists */
  LPRPtr = &g_uSleepDefaultLPR;
  if(NULL != LPRPtr->init_func)
  {
    /* Call default LPR's init function */
    LPRPtr->init_func();
  }

  /* Run through individual power modes init functions */
  for(nLPR=0; nLPR < USLEEP_NUMBER_LOW_POWER_MODES; nLPR++)
  {
    LPRPtr = &g_uSleepLPR[nLPR];
    CORE_VERIFY_PTR(LPRPtr);

    if(NULL != LPRPtr->init_func)
    {
      /* Call each LPR's init function */
      LPRPtr->init_func();
    }

    /* Initialize the working latency values */
    memcpy(&LPRPtr->working_latency, &LPRPtr->seed_latency, sizeof(uSleep_lpr_latency));
  }

  return;
}
