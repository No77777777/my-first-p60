/*==============================================================================
  FILE:         wakeup_lpr.c

  OVERVIEW:     This file provides the LPR definition for programming 
                the wakeup time for low-power modes.

  DEPENDENCIES: None

                Copyright (c) 2016-2019 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/lpr/wakeup_lpr.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "sleep_log.h"
#include "sleep_target.h"
#include "vmpm_utils.h"
#include "synthLPRM.h"
#include "CoreTime.h"

/*==============================================================================
                             GLOBAL VARIABLES
 =============================================================================*/
/**
 * @brief Time at which Sleep timer was programmed to wake up the master 
 *        during standalone low power modes.
 */
uint64 g_expectedWakeupTick = 0;

/*==============================================================================
                              INTERNAL FUNCTIONS
 =============================================================================*/
/**
 * cpuWakeupLPR_standaloneEnter
 *
 * @brief Enter function for standalone wakeup time programming
 */
void cpuWakeupLPR_standaloneEnter(uint64 wakeupTick)
{	

  /* Program the wakeup time in the local timer.
   * Different timer resolutions may result in a slightly updated
   * programmed match value, so save the actual value for later use. */
    g_expectedWakeupTick = sleepTarget_enableSleepTimer(wakeupTick);
	sleepStats_putLprTimeData(g_expectedWakeupTick,
                            SLEEP_STATS_TIME_REQ_LOCAL_WAKE_TYPE);

  return;
}


/**
 * cpuWakeupLPR_standaloneExit
 *
 * @brief Enter function for standalone wakeup time programming
 */
void cpuWakeupLPR_standaloneExit(void)
{
  if(FALSE == synthLPRM_isConfiguringModeFullyCacheable())
  {
    /* Disable Sleep QTimer Frame */
    sleepTarget_disableSleepTimer();
  }

  return;
}

/** cpuWakeupLPR_RPMEnter
 *
 * @brief Enter function for RPM assisted wakeup time programming
 */
void cpuWakeupLPR_RPMEnter(uint64 wakeupTick)
{

  /* Send the wakeup time to RPM */
  vmpm_SetWakeupTimetick(wakeupTick);

  sleepStats_putLprTimeData(wakeupTick, 
                            SLEEP_STATS_TIME_REQ_LOCAL_WAKE_TYPE);
  
  sleepLog_printf(SLEEP_LOG_LEVEL_INFO, (1*2),
                  " Program RPM wakeup (Request: 0x%llx)",
                  ULOG64_DATA(wakeupTick));

  return;
}
