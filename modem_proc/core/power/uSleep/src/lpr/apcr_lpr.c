/*==============================================================================
  FILE:         apcr_lpr.c

  OVERVIEW:     This file provides uSleep APCR LPR functions

  DEPENDENCIES: Object file generated from source is marked as island section
  
                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/lpr/apcr_lpr.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "qurt.h"
#include "qurt_restricted.h"
#include "uSleep.h"
#include "uSleep_target.h"

/*==============================================================================
                       EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
#if defined(USLEEP_SUPPORT_UNCACHED_APCR)
/* 
 * uSleepLPR_apcrEnter 
 */
void uSleepLPR_apcrEnter(uint64 wakeupTime, boolean reEntrant)
{
  /* APCR mode with PMI interrupt since this is uncached */
  uSleepTarget_configPowerMode(TARGET_POWER_MODE_APCR_PLL_LPM, TRUE);

  /* Program the local wakeup time */
  uSleepTarget_programWakeupTimer(wakeupTime);

  /* Enter all wait */
  qurt_power_apcr_enter();

  return;
}

#else

/* 
 * uSleepLPR_cachedApcrEnter
 */
void uSleepLPR_cachedApcrEnter(uint64 wakeupTime, boolean reEntrant)
{
  if(TRUE != reEntrant)
  {
    /* In APCR cached mode, we do not need to run an exit function until
     * we change to an un-cached mode so disable the PMI interrupt. */
    uSleepTarget_configPowerMode(TARGET_POWER_MODE_APCR_PLL_LPM, FALSE);
  }

  /* Since timers are always deferred, always program the wakeup time */
  uSleepTarget_programWakeupTimer(wakeupTime);

  return;
}

#endif /* USLEEP_SUPPORT_UNCACHED_APCR */
