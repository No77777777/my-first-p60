/*==============================================================================
  FILE:         uimage.c

  OVERVIEW:     This file provides uimage (software based) transition functions

  DEPENDENCIES: Functions are located in normal mode memory section
  
                Copyright (c) 2016-2019 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/transition/uimage.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "CoreVerify.h"
#include "uimage_trans.h"
#include "uSleep_transi.h"
#include "uSleep_os.h"
#include "uSleep_trans.h"
#include "uSleep_log.h"
#include "uSleep_util.h"
#include "icbarb.h"
#include "uimage_rpm.h"
#include "uSleepi.h"

/*==============================================================================
                         INTERNAL GLOBAL VARIABLES
 =============================================================================*/
uint32 g_uImageBIMCFreq = 0;
uint32 g_uImageSNOCFreq = 0;

/* Variable to control DDR on/off when entering island mode */
static volatile boolean g_uSleepKeepDDROn = FALSE;

/*==============================================================================
                            GLOBAL VARIABLES
 =============================================================================*/
uSleep_transition_data g_uSleepCmdSet;

/*==============================================================================
                              GLOBAL EXIT FUNCTIONS
 =============================================================================*/
/*
 * uSleep_prepareForExit
 * 
 * Performed in uSleep_exit context before any kernel exit calls
 */
void uSleep_prepareForExit(uSleep_transition_mode mode)
{
  uImage_rpm_wait_mode_t rpmMode;

  if(UIMAGE_EXIT == mode)
  {
    /* If this is a normal exit request, enable the exit mode low power mode to
     * be entered if the system goes idle during the transition period */
   // uSleepTarget_configureIslandTransitionPowerMode(TRUE);

    /* Do not wait for RPM to turn on DDR which allows us to sleep during
     * this time.
     * Note that until RPM implements this optimization, request a busy wait. */
    rpmMode = UIMAGE_RPM_BUSY_WAIT_MODE;
  }
  else
    {
    /* In fast exit mode, we must always wait for DDR on as we do not want
     * to introduce any additional possibilities of errors in the code. */
    rpmMode = UIMAGE_RPM_BUSY_WAIT_MODE; 
    }

  if(FALSE == g_uSleepKeepDDROn)
  {
    /* Restore bus/ddr frequency to pre-uImage entry */
    uImage_rpm_set_clk_values(g_uImageSNOCFreq, g_uImageBIMCFreq, rpmMode);
  }

  else
  {
    /* DDR was requested to stay on for debug, notify that it's on */
  uSleepTrans_notifyComplete(FALSE);
  }

 g_uImageBIMCFreq  = 0;
 g_uImageSNOCFreq  = 0;

  return;
}

/*==============================================================================
                              GLOBAL ENTER FUNCTIONS
 =============================================================================*/
/*
 * uSleep_completeEntry
 */
void uSleep_completeEntry(void)
{
  return;
}

/*
 * uSleep_continueEntry
 */
void uSleep_continueEntry(void)
{
  uint32 bimcValue = 0;

#ifdef ISLAND_BIMC_SLOW_CLOCK
  bimcValue = 1;  /* Keep bus on, but at slowest possible rate */
#endif

  if(FALSE == g_uSleepKeepDDROn)
  {
		
	    uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 0, 
                   " uSleep_continueEntry send votes");	
    /* Turn DDR off and do not wait since island mode can operate even while DDR
     * is still on */
    uImage_rpm_set_clk_values(1, bimcValue, UIMAGE_RPM_NO_WAIT_MODE);
	
    }
  else
    {
    /* If we are not actually turning off DDR, then we need to notify 
     * ourselves that the transition is complete. */ 
    uSleepTrans_notifyComplete(TRUE);
  }

  return;
}

/*
 * uSleep_fatalErrorHandler
 */
void uSleep_fatalErrorHandler(uSleep_internal_state state)
{
  uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 1, 
                   "USLEEP FATAL ERROR CALLED (state: %d)",
                   state);

  switch(state)
  {
    /* At beginning stage of entry -
     *
     * Nothing to do as the normal exit request was made before (or while in) call
     * to kernel entry */
    case uSLEEP_INTERNAL_STATE_INIT_ENTRY:
    case uSLEEP_INTERNAL_STATE_PREPARE_ENTRY:
    {
      return;
    }

    /* Island entry transition is in progress -
     *
     * This stage is after initial kernel entry, but before final transition is complete.
     * Sleep is (somewhere) in the process of HW entry.  Try executing the exit sequence
     * and continue */
    case uSLEEP_INTERNAL_STATE_CONT_ENTRY:
    {
      /* Initiate the exit */
      uSleepTrans_islandControl(UIMAGE_EXIT_FAST);
      break;
    }

    /* Island entry transition is finalizing -
     *
     * This stage is after kernel and HW entry has completed and sleep is performing the final
     * stages of software entry */
    case uSLEEP_INTERNAL_STATE_CMPLT_ENTRY:
      uSleepTrans_islandControl(UIMAGE_EXIT_FAST);
    break;

    /* Standard exit has initially been requested -
     *
     * This stage is before kernel or HW exit has been triggered so just start the fatal
     * exit process */
    case uSLEEP_INTERNAL_STATE_INIT_EXIT:
      uSleepTrans_islandControl(UIMAGE_EXIT_FAST);
    break;

    /* Initial entry stage is in progress
     *
     * This stage is during the kernel stage 1 entry and would mean a failure most likely in the
     * island manager entry call itself.
     */
    case uSLEEP_INTERNAL_STATE_STAGE1_ENTRY:
    {
      /* Set the fatal exit state so when the transition notification is called it will perform
       * the correct behavior */
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_FATAL_EXIT);
	  
	  /* Continue the exit process */
      uSleepTrans_notifyComplete(FALSE);
      return;
      break;
    }

    /* Standard exit is in progress -
     *
     * This stage is before kernel exit call, but most likely in the exitPrepare call, where HW would be
     * re-enabled.
     * Try to complete the exit anwyway by calling the notifycomplete function
     */
    case uSLEEP_INTERNAL_STATE_STAGE1_EXIT:
    {
      /* Set the fatal exit state so when the transition notification is called it will perform
       * the correct behavior */
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_FATAL_EXIT);

      /* Continue the exit process */
      uSleepTrans_notifyComplete(FALSE);

      break;
    }

    /* Standard exit is in progress -
     *
     * This stage is after sleep stage 1 exit so HW / DDR is enabled but before kernel
     * exit stage is called */
    case uSLEEP_INTERNAL_STATE_STAGE2_EXIT:
    {
      /* Attempt to continue the exit process in the fatal error case */
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_FATAL_EXIT);
      uSleepTrans_notifyComplete(FALSE);
      break;
    }

    /* In island operational mode -
     *
     * Simply start the fatal error exit process */
    case uSLEEP_INTERNAL_STATE_ACTIVE:
    {
      /* In island operational mode, just call the island control API with the fatal flag */
      uSleepTrans_islandControl(UIMAGE_EXIT_FAST);
      break;
    }

    /* Invalid or recursive state - function should not have been called */
    default:
    {
      uSleepLog_QDSSPrintf(USLEEP_LOG_LEVEL_TRANSITION, 
                           USLEEP_CRIT_ERR_NUM_ARGS,
                           USLEEP_CRIT_ERR_STR, 
                           USLEEP_CRIT_ERR,
                           state);

    }
  }

  return;
}

