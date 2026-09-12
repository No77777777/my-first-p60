/*==============================================================================
  FILE:         uSleep_trans.c

  OVERVIEW:     This file provides uSleep framework for island transition

  DEPENDENCIES: Object file generated from source is marked as island section
  
                Copyright (c) 2016-2018 Qualcomm Technologies, Inc. (QTI).
                All Rights Reserved.
                Qualcomm Confidential and Proprietary.
                Export of this technology or software is regulated by the U.S. Government.
                Diversion contrary to U.S. law prohibited.
================================================================================
$Header: //components/rel/core.mpss/10.0/power/uSleep/src/transition/uSleep_trans.c#1 $
$DateTime: 2019/04/24 00:03:26 $
==============================================================================*/
#include "comdef.h"
#include "CoreVerify.h"
#include "uCoreTime.h"
#include "uSleep.h"
#include "uSleepi.h"
#include "uSleep_transi.h"
#include "uSleep_timer.h"
#include "uSleep_target.h"
#include "uSleep_util.h"
#include "uSleep_lpr.h"
#include "uSleep_os.h"
#include "uSleep_os_trans.h"
#include "uSleep_log.h"
#include "uInterruptController.h"
#include "island_mgr.h"
#include "timer.h"
#include "uATS.h"
#include "uSleep_trans.h"

/*==============================================================================
                              INTERNAL MACROS
 =============================================================================*/
/* Internal signal values used to coordinate transition routines */
#define USLEEP_CLIENT_EXIT_SIGNAL     0x01

/*==============================================================================
                         EXTERNAL FUNCTION REFERENCES
 =============================================================================*/
void uSleep_prepareForEntry(void);  /* Island entry preperation - Called in normal mode */ 
void uSleep_continueEntry(void);    /* Island entry continuation - Called after Kernel entry */
void uSleep_completeEntry(void);    /* Island entry completion - Called after full entry complete */

void uSleep_prepareForExit(uSleep_transition_mode mode);  /* Island exit preperation - Called in island mode*/
void uSleep_completeExit(uSleep_transition_mode mode);

/*==============================================================================
                             GLOBAL VARIABLES
 =============================================================================*/
/* Signal used to block caller when uSleep exit is requested */
qurt_signal_t g_uSleepExitSignal;

/* Signal used to block a call to exit while in the process of entering island */
qurt_signal_t g_uSleepEnterSignal;

/* Original thread priority before elevating to highest during transitions */
int g_uSleepOrigThreadPriority;

/*==============================================================================
                       INTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/** 
 * uSleepTrans_internalSignalControl
 * 
 * @brief Controls the internal entry and exit signals to ensure the transition
 *        flow completes properly
 * 
 * @param control:  Enum type for the desired operation on the given signal
 * @param signal:   Initilized entry or exit signal
 */
static void uSleepTrans_internalSignalControl(uSleep_trans_signal_control control,
                                              qurt_signal_t               *signal)
{
  switch(control)
  {
    case USLEEP_TRANS_SIGNAL_CLEAR:
    {
      qurt_signal_clear(signal, USLEEP_CLIENT_EXIT_SIGNAL);
    }
    break;

    case USLEEP_TRANS_SIGNAL_SET:
    {
      qurt_signal_set(signal, USLEEP_CLIENT_EXIT_SIGNAL);
    }
    break;

    case USLEEP_TRANS_SIGNAL_WAIT:
    {
      qurt_signal_wait(signal, USLEEP_CLIENT_EXIT_SIGNAL, QURT_SIGNAL_ATTR_WAIT_ALL);
    }
    break;

    default:
    {
      CORE_LOG_VERIFY(0, uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 1,
                                          "Invalid signal (%d)",
                                          (uint32)control));
    }
  }

  return;
}

/** 
 * uSleepTrans_performNormalModeTransition
 * 
 * @brief uSleep function that begins the kernel island exit requirements
 */
static uint32 uSleepTrans_performNormalModeTransition(void)
{
  /* Hardware should have exited island at this point, continue the software
   * exit process */
  CORE_VERIFY(uSleep_getStateInternal() == uSLEEP_INTERNAL_STATE_STAGE2_EXIT);
  
  /* Set sleep to highest priority to ensure entry is completed */
  qurt_thread_set_priority(qurt_thread_get_id(), 1);

  /* Stop the uATS threshold timer to prevent any activity */
  uATS_stopThreasholdTimer();

  uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 0, "Kernel stage 1 exit");

  /* Call the kernel transition function to continue the process of exiting island 
   * mode.
   * 
   * When this returns the kernel will allow DDR access, global interrupts will be
   * disabled (special STM mode) and the calling thread (sleep in this case) will be
   * set to the highest priority. */ 
  if(QURT_EOK != island_mgr_island_exit(QURT_ISLAND_EXIT_STAGE1))
  {
    /* Exit is broke, and we can't continue. */
    uSleepOS_haltOnError();
  }
  
  /* Complete the target specific requirements to continue the exit process */
  uSleep_completeExit(UIMAGE_EXIT);

  /* Finish the transition now that DDR is fully accessible */
  uSleepTrans_completeTransitionToNormalMode(FALSE);

  return 0; 
}

/*==============================================================================
                           EXTERNAL FUNCTION DEFINITIONS
 =============================================================================*/
/*
 * uSleepTrans_islandControl
 */
void uSleepTrans_islandControl(uSleep_transition_mode mode)
{
  int status;
  uSleep_transition_profiling *transProfileData;
  if(UIMAGE_ENTER == mode)
  {
    transProfileData = uSleep_getProfilingDataPtr();

    /* Reset the entry signal so we can block any possible exit call until uSleep
     * has fully entered */
    uSleepTrans_enterSignalControl(USLEEP_TRANS_SIGNAL_CLEAR);

    /* The entry is divided into seperate calls to allow the functions to be located in
     * different memory sections, as well as allow any target specific requirements to be handled
     * seperaratly.
     * 
     * The prepare function will perform any initial setup required to enter uImage mode
     * while still in normal operational mode. */
    uSleep_prepareForEntry();

    /* Update state before kernel call */
    uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_STAGE1_ENTRY);

    /* Set sleep to highest priority to ensure entry is completed */
    qurt_thread_set_priority(qurt_thread_get_id(), 1);

    uSleepDDRLog_printf(USLEEP_DDR_LOG_LEVEL_TRANSITION,0, "Begin island mgr entry");

    /* Mark the start of island entered.
     * Interrups are freed in the island mgr call, so this is as close as we can get to
     * that time */
    transProfileData->start_of_island_operation = uCoreTimetick_Get64();

    /* Invoke the kernel island mangager entry function
     * This will update TLBs and perform other OS specific island entry requirements */
        status = island_mgr_island_enter();
        uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "island_mgr_island_enter");
    /* Note that if an exit has been requested during the above call, the start time is
     * recorded in the uSleep_exit call as it is the only function that can cause island mgr
     * to return a cancel status */
    if(ISLAND_MGR_ECANCEL == status) 
    {
      uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "Kernel island entry canceled");
      transProfileData->early_exits_cnt[EARLY_EXIT_CANCEL]++; 

      /* Current state should be an initial exit request */
      CORE_VERIFY(uSleep_getStateInternal() == uSLEEP_INTERNAL_STATE_INIT_EXIT);

      /* Kernel stage 1 exit has automatically been completed, continue from there */
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_STAGE2_EXIT);

      /* Sync with any exit call */
      uSleepTrans_enterSignalControl(USLEEP_TRANS_SIGNAL_SET);

      /* Short cut exit process by directly calling the continue function */
      uSleepTrans_completeTransitionToNormalMode(TRUE);
    }
    else if(ISLAND_MGR_EOK == status)    
     {
      /* Set the entry continuation state.
       * Check for race conditions with an exit call before setting though by only updateing if
       * we are still in stage 1 */
       uSleep_checkAndSetStateInternal(uSLEEP_INTERNAL_STATE_STAGE1_ENTRY,
                                      uSLEEP_INTERNAL_STATE_CONT_ENTRY);

       uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "before uSLEEP_INTERNAL_STATE_CONT_ENTRY check");
      /* If island manager returned ok, double check the uSleep state to ensure an exit reqeust
       * was not caught in the API call (very small window of oportunity for this) */
      if(uSleep_getStateInternal() != uSLEEP_INTERNAL_STATE_CONT_ENTRY)
      {
        uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "Caught exit request while entering island");

        transProfileData->early_exits_cnt[EARLY_EXIT_OK_STG1]++;

        /* Current state should be an initial exit request */
        CORE_VERIFY(uSleep_getStateInternal() == uSLEEP_INTERNAL_STATE_INIT_EXIT);

        uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_STAGE1_EXIT);

        /* Sync with any exit call */
        uSleepTrans_enterSignalControl(USLEEP_TRANS_SIGNAL_SET);

        /* We haven't called continue/complete entry yet, so skip to the notification exit call
         * to continue with the exit process */
        uSleepTrans_notifyComplete(FALSE);
      }
      else
      {
    /* Perform any further target specific requirements after kernel entry is complete */
    uSleep_continueEntry();
    uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "done uSleep_continueEntry ");
  }
    }
    else
    {
      CORE_LOG_VERIFY(0, uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,1, "Kernel island entry failure (Status: %d)", status));
    }
  }
  else
  {
    /* Set the initial exit state and perform any pre-exit requirements */
    if(UIMAGE_EXIT_FAST == mode)
    {
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_FATAL_EXIT);
    }
    else
    {
      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_STAGE1_EXIT);
    }

    /* Prepare for exiting uImage */
    uSleep_prepareForExit(mode);
  }
 
  return;
}

/* 
 * uSleepTrans_performIslandModeTransition
 */
uint32 uSleepTrans_performIslandModeTransition(void)
{
  /* Function should entered once, only from the island entry request stage */
  CORE_VERIFY(uSleep_getStateInternal() == uSLEEP_INTERNAL_STATE_INIT_ENTRY);

  /* Set initial entry state  */
  uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_PREPARE_ENTRY);

  /* Start the island entry process */
  uSleepTrans_islandControl(UIMAGE_ENTER);

  return 0;
}

/*
 * uSleepTrans_notifyComplete
 */
void uSleepTrans_notifyComplete(uint8 entry)
{
  uSleep_transition_profiling *transProfileData;
  uSleep_internal_state       state = uSleep_getStateInternal();

  if(TRUE == entry)
  {
    transProfileData = uSleep_getProfilingDataPtr();

    if(state != uSLEEP_INTERNAL_STATE_INIT_EXIT)
    {
      /* Ensure we are in entry continuation state, waiting for completion notification */
      CORE_VERIFY(uSLEEP_INTERNAL_STATE_CONT_ENTRY == state);

      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_CMPLT_ENTRY);

    /* Complete target specific requirments to enter uImage mode */
    uSleep_completeEntry();

    /* Island entry is complete, set task pointer to main uSleep function */
    uSleepOS_setIdleFunctionPtr(uSleepOS_performLPM);

    uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 0, "Island entry done");

      CORE_LOG_VERIFY(((uCoreTimetick_Get64() - transProfileData->entry_start_time) < US_TO_TICKS(50000)),
                      uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "FATAL: LARGE ISLAND ENTRY LATENCY DETECTED"));

    /* Record the uSleep entry transition time */
    sleepStats_updateValue(&transProfileData->entry_stats,
                           uCoreTimetick_Get64() - transProfileData->entry_start_time);

    /* Set island fully entered state */
    uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_ACTIVE);

      /* Release any exit requests waiting on critial entry completion */
    uSleepTrans_enterSignalControl(USLEEP_TRANS_SIGNAL_SET);

    /* Restore original thread priority */
    qurt_thread_set_priority(qurt_thread_get_id(), g_uSleepOrigThreadPriority);

    /* Tell uATS to init itself on island entry */
    uATS_SetSignal(USLEEP_SIGNAL_INIT_COMPLETE);

    uSleepTrans_updateLifetimeStats(transProfileData, TRUE);
  }
  else
  {
      uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "Island entry caught exit request");

      transProfileData->early_exits_cnt[EARLY_EXIT_OK_STG2]++;

      /* Restore original thread priority in exit requested state */
      qurt_thread_set_priority(qurt_thread_get_id(), g_uSleepOrigThreadPriority);

      /* Recursive call here, but should be ok as we are talking the exit path now from the enter
       * which will just fall through when done */
      uSleepTrans_islandControl(UIMAGE_EXIT);

      /* Sync with any exit call */
      uSleepTrans_enterSignalControl(USLEEP_TRANS_SIGNAL_SET);
    }
  }
  else
  {
    if(uSLEEP_INTERNAL_STATE_FATAL_EXIT == state)
    {
      uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION,0, "uSleep fatal exit called.");

      /* In cases of fatal error, exit island immediatly after island has
       * finished it's transition. */
      uSleepOS_setIdleFunctionPtr(uSleepOS_performError);

      /* Exit immediately */
      if(QURT_EOK != island_mgr_island_exit(QURT_ISLAND_EXIT_STAGE1) ||
         QURT_EOK != island_mgr_island_exit(QURT_ISLAND_EXIT_STAGE2))
      {
        /* Fatal error exit is broke, nothing more we can do here. */
        uSleepOS_haltOnError();
      }
    }
    else
    {
      CORE_VERIFY(uSLEEP_INTERNAL_STATE_STAGE1_EXIT == state);

      uSleepLog_printf(USLEEP_LOG_LEVEL_TRANSITION, 0, "Sleep exit stage 1 complete");

      uSleep_setStateInternal(uSLEEP_INTERNAL_STATE_STAGE2_EXIT);

      /* Island exit prep is done, finish the non-error exit process */
      uSleepOS_setIdleFunctionPtr(uSleepTrans_performNormalModeTransition);

      /* Force the sleep thread out of any call to the kernel wait for idle API so the exit can 
       * continue in the sleep context which will run the function set above. */
      qurt_power_cancel_wait_for_idle();
    }
  }

  return;
}

/*
 * uSleepTrans_exitSignalControl
 */
void uSleepTrans_exitSignalControl(uSleep_trans_signal_control control)
{
  uSleepTrans_internalSignalControl(control, &g_uSleepExitSignal);
  return;
}

/*
 * uSleepTrans_enterSignalControl
 */
void uSleepTrans_enterSignalControl(uSleep_trans_signal_control control)
{
  uSleepTrans_internalSignalControl(control, &g_uSleepEnterSignal);
  return;
}

/*
 * uSleepTrans_enterSignalControl
 */
void uSleepTrans_updateLifetimeStats(uSleep_transition_profiling  *transProfileData,
                                     boolean                      enter)
{
  lifetime_stats      *lifetime;
  sleepStats_generic  *stats;

  if(TRUE == enter)
  {
    lifetime  = &transProfileData->lifetime_enter;
    stats     = &transProfileData->entry_stats;
  }
  else
  {
    lifetime  = &transProfileData->lifetime_exit;
    stats     = &transProfileData->exit_stats;
  }

  if(stats->min < lifetime->min)
  {
    lifetime->min = stats->min;
  }

  if(stats->max > lifetime->max)
  {
    lifetime->max = stats->max;
  }

  return;
}

