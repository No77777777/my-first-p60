/*==============================================================================
  FILE:         uSleep_target.c

  OVERVIEW:     This file provides uSleep target specfic functions while in
                normal operational mode

  DEPENDENCIES: Object file generated from source is marked as island section
  
                Copyright (c) 2016-2017 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/target/ddr/uSleep_target.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "CoreVerify.h"
#include "uSleep.h"
#include "uSleep_target.h"
#include "uSleep_os.h"
#include "uSleep_hwio.h"
#include "sleep_os.h"
#include "rsc.h"
#include "q6_lpm_config.h"

/*==============================================================================
                               GLOBAL VARIABLES
 =============================================================================*/
#ifdef USLEEP_SUPPORT_TRANSITIONS
/* If TRUE, allows island debugging by keeping the JTAG port active. */
static volatile boolean g_usleepEnableIslandDebug = FALSE;

/* Structure to hold the original main sleep interrupt functions */
struct
{
  sleep_PMI_handler_ptr_type      pmi;
  sleep_profile_handler_ptr_type  profile;
}g_sleepIntFunctions = {NULL, NULL};
#endif

/*==============================================================================
                       EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
#ifdef USLEEP_SUPPORT_TRANSITIONS
/*
 * uSleepTarget_enableIslandDebug
 */
void uSleepTarget_enableIslandDebug(boolean enable)
{
  static boolean allowed = FALSE;

  if((TRUE == g_usleepEnableIslandDebug) && (TRUE == enable))
  {
    /* Enable island debug */
    allowed = TRUE;
  }
  else if(TRUE == allowed)
  {
    /* Disable island debug */
    allowed = FALSE;
  }

  return;
}

/*
 * uSleepTarget_setInterruptHandlers
 */
void uSleepTarget_setInterruptHandlers(void)
{
  /* Save the original function pointers */
  g_sleepIntFunctions.pmi     = sleepOS_getPMIFunctionPTR();
  g_sleepIntFunctions.profile = sleepOS_getProfileFunctionPTR();

  /* Use main sleep's API to set the interrupt function pointers to the uSleep implmentations */
  sleepOS_setPMIFunctionPTR(uSleepOS_PMIPerformer);
  sleepOS_setProfileFunctionPTR(NULL);

  return;
}

/*
 * uSleepTarget_restoreInterruptHandlers
 */
void uSleepTarget_restoreInterruptHandlers(void)
{
  sleepOS_setPMIFunctionPTR(g_sleepIntFunctions.pmi);
  sleepOS_setProfileFunctionPTR(g_sleepIntFunctions.profile);

  return;
}

#endif /* USLEEP_SUPPORT_TRANSITIONS */

/*
 * uSleepTarget_initialize
 */
void uSleepTarget_initialize(void)
{
  /* No target specific setup required currently as it all taken care of in the main sleep routines */
  return;
}

/* 
 * uSleepTarget_configBasePowerMode
 */
void uSleepTarget_configBasePowerMode(target_power_mode mode)
{
  /* Set the initial default low power mode setup.  Flag param doesn't matter
   * since it will be selected based on the real mode chosen, just need to
   * setup the base default mode here. */
#ifdef USLEEP_IMAGE_SLPI
  q6LPMConfig_setupModeConfig(RSC_MODE_APCR_PLL_LPM_NO_SS, FALSE);
#else
  q6LPMConfig_setupModeConfig(RSC_MODE_APCR_PLL_LPM, FALSE);
#endif

  /* Disable handshake by default */
  uSleepTarget_setRpmHandshake(FALSE);

  /* Always defer the non-wakeup timer frames */
  USLEEP_HWIO_OUT(L2VIC_WAKEUP_EN, WAKEUP_EN_QTMR_FRAME_2);

#ifdef USLEEP_SUPPORT_V66_1_1
  USLEEP_HWIO_OUTF(RSCCTL_EN_STATERET, PROF_IRQ, 0x0); /* Profling interrupt not currently used */
#endif

  return;
}

/* 
 * uSleepTarget_restoreBasePowerMode
 */
void uSleepTarget_restoreBasePowerMode(void)
{
  q6LPMConfig_setupBase();
}
