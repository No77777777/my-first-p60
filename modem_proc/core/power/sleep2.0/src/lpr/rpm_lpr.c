/*==============================================================================
  FILE:         rpmh_lpr.c

  OVERVIEW:     This file provides the LPR definition for the RPMh
                low-power modes.

  DEPENDENCIES: None

                Copyright (c) 2016-2018 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/sleep2.0/src/lpr/rpm_lpr.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "rpm.h"
#include "rpmclient.h"
#include "sleep_log.h"
#include "sleep_target.h"

/*==============================================================================
                              INTERNAL VARIABLES
 =============================================================================*/
static uint32 g_nasResult;
static uint32 g_ssResult;

/*==============================================================================
                              INTERNAL FUNCTIONS
 =============================================================================*/
/**
 * RPMLPR_enter
 *
 * @brief Enter function for RPM synchronization low power mode.
 */
void RPMSyncLPR_enter(uint64 wakeupTick)
{
  /* Indicate that we are doing a deeper power mode */
  sleepTarget_setRPMAssistedMode(TRUE);

  /* Enable child RSC handshaking in non-SLPI images */
  sleepTarget_setRpmHandshake(TRUE);  

  g_ssResult = rpm_force_sync(RPM_SLEEP_SET);

  sleepLog_QDSSPrintf(SLEEP_LOG_LEVEL_INFO, SLEEP_SET_SEND_NUM_ARGS,
                      SLEEP_SET_SEND_STR, SLEEP_SET_SEND);

  g_nasResult = rpm_force_sync(RPM_NEXT_ACTIVE_SET);

  if(g_ssResult || g_nasResult)
  {
    rpm_mask_interrupt(true);
  }

  return;
}

/**
 * RPMhLPR_exit
 *
 * @brief Exit function for RPM synchronization low power mode.
 */
void RPMSyncLPR_exit(void)
{
    /* Ensure churn_queue uses g_nasResult if valid, else g_ssResult */
  rpm_churn_queue(g_nasResult?g_nasResult:g_ssResult);

  if(g_ssResult || g_nasResult)
  {
    rpm_mask_interrupt(false);
  }

sleepTarget_setRPMAssistedMode(FALSE);

  /* Disable RPM handshaking */
  sleepTarget_setRpmHandshake(FALSE);

  return;
}

