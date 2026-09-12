/*============================================================================
  FILE:         masterstats_lpr.c

  OVERVIEW:     This file provides the LPR definition for the CXO clock
                low-power modes.

  DEPENDENCIES: None

                Copyright (c) 2016 - 2021 Qualcomm Technologies Inc.
                All Rights Reserved.
                Qualcomm Technologies Confidential and Proprietary
================================================================================
$Header: //components/rel/core.mpss/10.2/power/sleep2.0/src/master_stats/masterstats_lpr.c#1 $
$DateTime: 2022/01/04 12:00:45 $
==============================================================================*/
#include "sleep_lpr.h"
#include "sleep_target.h"
#include "CoreVerify.h"
#include "sleep_masterstats.h"
#include "sleep_statsi.h"
#include "rsc.h"

/*==============================================================================
                               INTERNAL FUNCTIONS
 =============================================================================*/
/**
 * masterStatsLPR_enter
 *
 * @brief Function that gets called when Master is voting for cxo shutdown
 *        during a Sleep cycle.
 *
 * @param wakeup_tick: Absolute time tick (in 19.2 Mhz) when Master is
 *                     expected to wake up.
 */
void masterStatsLPR_enter(uint64 wakeup_tick)
{
  /* Updating the CX collapse mode Entry timestamp in SMEM memory location */
  sleepMasterstats_updatePowerdownStats(CoreTimetick_Get64());
  return;
}

/**
 * masterstatsLPR_initialize
 *
 * @brief Initialize the master stats LPR
 */
void masterstatsLPR_initialize(void)
{
  /* Initialize the SMEM memory to store cx collapse stats */
  sleepMasterstats_init();
  return;
}

/*==============================================================================
                             CXO Profiling Function
 =============================================================================*/
 /**
 * masterStatsLPR_ProfileExit
 *
 * @brief Profile function to Update Cx collapse mode wakeup stats to shared memory
 */
uint64 masterStatsLPR_ProfileExit(sleepStats_timestamps *timeStamps,
                                  sleep_lprm            *self,
                                  boolean               runAsCacheable)
{
  rsc_profile_unit  *rscUpend          = &timeStamps->rsc_child[STATS_RSC_PROFILE_UP_END];
  uint64            lprModeLatency     = self->mode_statistics.last_exit_end - self->mode_statistics.last_exit_start;
  boolean           validLpmCycle      = FALSE;

  rsc_profile_unit  *rscUpstart  = &timeStamps->rsc_child[STATS_RSC_PROFILE_UP_START];
  if(rscUpstart->valid)
  {
    validLpmCycle = TRUE;
  }

  /*Updating the CX collapse wakeup mode stats using the child end timestamp */
  sleepMasterstats_updateWakeupStats(rscUpend->timeStamp, validLpmCycle);

  return lprModeLatency;
}
