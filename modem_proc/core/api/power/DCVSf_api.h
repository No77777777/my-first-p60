#ifndef DCVSF_API_H
#define DCVSF_API_H

/*============================================================================
  @file dcvsf_api.h

  DCVSf APIs

  Publishes APIs supported by fast DCVS algorithm.

  Copyright (c) 2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/api/power/DCVSf_api.h#1 $ */
#include <CLM.h>

/* Controls for disabling various paths to trigger fast polling mode */
#define DISABLE_FP_MODE_ENTER      0x1  /* Disables entering FP mode entirely */
#define DISABLE_MDM_DANGER_TRIGGER 0x2  /* Disable handling of DCVS_SYS_EVT_MDM_DANGER in DCVS_processSysEvent */
#define DISABLE_SYSEVENT_TRIGGER   0x4  /* Disable handling of DCVS_SYS_EVT_TRIG_DCVS in DCVS_processSysEvent */
#define DISABLE_HIGH_UTIL_TRIGGER  0x8  /* Disable triggering fast mode when utilization exceeds set threshold */
#define DISABLE_LOW_TH_ADJUSTMENT  0x10 /* Disable adjusting low threshold dynamically */
#define DISABLE_STATS_TRIGGER      0x20 /* Disable reacting to statistics triggers */
#define DISABLE_FP_MODE_MASK (DISABLE_FP_MODE_ENTER     | DISABLE_MDM_DANGER_TRIGGER | DISABLE_SYSEVENT_TRIGGER | \
                              DISABLE_HIGH_UTIL_TRIGGER | DISABLE_LOW_TH_ADJUSTMENT  | DISABLE_STATS_TRIGGER)
/* Debug bits to control various DCVSf logical paths */
#define DBG_Q6_MCVS_LIMIT1      0x1000
#define DBG_Q6_MCVS_LIMIT2      0x2000
#define DBG_Q6_BOOST_OFF        0x4000
#define DBG_BIMC_BOOST_OFF      0x8000
#define DBG_MCVS_FP_TRIG        0x10000
#define DBG_FP_LOW_TH_TRIG      0x20000
#define DBG_Q6MCVS_MIN_PERF_LVL 0x40000
#define DBG_EXTRA_VERBOSE_LOGS  0x80000
#define DBG_UT_TH_PRIO_MID      0x100000
#define DBG_UT_TH_PRIO_HIGH     0x200000
#define DBG_CRASH_ON_VOTE_DELAY 0x400000
#define DBG_CRASH_TMR_EXTEND    0x800000
/* SL = soft-landing, GD = gradual descent */
#define DBG_DISABLE_SL_GD       0x1000000
#define DBG_CRASH_ON_ASYNC_CALL 0x2000000

#define NSEC_PER_TICK 52
#define XO_MHz        19.2

//DCVS state polling periods
/* load measurement interval in default mode */
#define DEFAULT_POLLING_INTERVAL 50 //msecs

#define DCVS_LEVEL_UP 2
#define DCVS_LEVEL_DOWN 1
#define DCVS_LEVEL_NO_CHG 0

//bit sets possible for gDCVSdebugPrints
#define DBG_VERB_LOGS_ALWAYS 0x1
#define DBG_VERB_LOGS_LVL_CHG 0x2
#define DBG_PRINT_HW_INFO 0x4
#define DBG_TRIG_STROBE 0x8
#define DBG_NO_LAT_ADJ_ALL_STATES 0x10
#define DBG_DO_LAT_ADJ 0x20
#define DEFAULT_DBG_MASK (DBG_NO_LAT_ADJ_ALL_STATES | DBG_DO_LAT_ADJ)

#if CLM_DCVS_DEBUG == 0
  #define DCVS_LOG(NUM_ARGS, ...)
#else
  #include "msg.h"
  #define MSG_0 MSG
  #define DCVS_LOG(NUM_ARGS, ...) \
  do { \
       if ((gDCVSdebugPrints & DBG_VERB_LOGS_ALWAYS) || \
           ((gDCVSdebugPrints & DBG_VERB_LOGS_LVL_CHG) && (dcvsLevelChanged != DCVS_LEVEL_NO_CHG))) \
       { \
         MSG_##NUM_ARGS(MSG_SSID_DFLT, MSG_LEGACY_HIGH, __VA_ARGS__);\
       } \
     } while(0)
#endif

#define TRIG_PWR_STROBE(param) \
  if (gDCVSdebugPrints & DBG_TRIG_STROBE) \
  { \
    uint8 fncalled = 0; \
    uint32 ret = 0; \
    CORE_CALL_WEAK_FN(fncalled, L1EM_TrigPwrStrobe, &ret, (param)); \
    if (fncalled == 0) \
    { \
      DCVS_LOG(0, "Power strobes will not work as L1EM_TrigPwrStrobe is not defined in this build!"); \
    } \
  }

#define RCM_GPIO_TRIG(evt, dur) \
  if (gDCVSdebugPrints & DBG_TRIG_STROBE) \
  { \
    TRIG_PWR_STROBE(evt); \
    DALSYS_BusyWait(dur); \
    TRIG_PWR_STROBE(evt); \
  }

/* trigger RCM trig 0 (RCM_TRIGGER_1) for different sys events - use back to back events to convey more
 * differentiation - event 0 is also used in MCPM but should be safe to overload as these strobes are
 * orthogonal to other mcpm/L1 functionality */
#define TRIG_MDM_DNGR_ENTER_STROBE(evt) \
  if (gDCVSdebugPrints & DBG_TRIG_STROBE) \
  { \
    if ((evt) == MDM_DANGER_LVL_1) \
    { \
      RCM_GPIO_TRIG(1, 10); \
    } \
    if ((evt) == MDM_DANGER_LVL_2) \
    { \
      RCM_GPIO_TRIG(1, 10); \
      DALSYS_BusyWait(10); \
      RCM_GPIO_TRIG(1, 10); \
    } \
  }

/* trigger RCM trig 0 (RCM_TRIGGER_1) for general purpose enter and exit markers */
#define TRIG_GP_ENTER_STROBE TRIG_PWR_STROBE(0);
#define TRIG_GP_EXIT_STROBE  TRIG_PWR_STROBE(0);

/* trigger RCM trig 1 (RCM_TRIGGER_2) for general purpose enter and exit markers */
#define TRIG_GP_ENTER_STROBE1 TRIG_PWR_STROBE(1);
#define TRIG_GP_EXIT_STROBE1  TRIG_PWR_STROBE(1);

#define SIGNAL_DCVS_UT(param) \
  do { \
       uint8 fncalled = 0; \
       uint32 ret = 0; \
       CORE_CALL_WEAK_FN(fncalled, L1EM_CLMtimerCbBcast, &ret, (param)); \
       /* UPDATE: no longer needed - UT uses it's own timer for CLM synchronization and callbacks */ \
       /*if (fncalled == 0) */ \
       /*{ */ \
       /*  DCVS_LOG(0, "DCVS UT will not work as L1EM_CLMtimerCbBcast is not defined in this build!"); */ \
       /*} */ \
  } while (0)

/* when dcvs is inactive/disabled, there are no periodic updates from clm -> dcvs */
#define DCVS_INACTIVE_PERIOD 0

//maximum length of time fast mode stays on
#define AUTO_SELECT_FP_DURATION 0
//exit FP mode when all FP events have turned off
#define EXIT_FP_ON_EVENT_OFF    0x7fffffff

//number of spikes in specific duration to track
#define MAX_SPIKE_COUNT 2

#define DCVS_INVALID_VAL 0xffffffff
#define DCVS_UNINIT_VAL 0xfffffffe

extern uint8 gDCVSdebugPrints;
extern uint8 dcvsLevelChanged;

extern void L1EM_CLMtimerCbBcast(uint32 * ret, ...) __attribute__((weak));

/* Codes to return to callers to DCVS_processSysEvent */
typedef enum
{
  /* Client does not need to take any action */
  DCVS_NO_ACTION = 0,
  /* Client should reschedule call to '' API in their context to allow DCVS
   * to enter fast-polling mode */
  DCVS_RESCHED_CB = 1
} eDCVSAction;

/* Enum for return values */
typedef enum
{
  DCVS_BYPASS_INT_VOTE = 0,
  DCVS_DO_NOT_BYPASS,
  DCVS_ALG_ERROR
} eDCVSReturnType;

/* Different event value types accepted by DCVS_processSysEvent */
typedef enum
{
  /* Values for modem danger levels available in HW */
  MDM_DANGER_NONE  = 0,
  MDM_DANGER_LVL_1 = 1,
  MDM_DANGER_LVL_2 = 2,
  
  /* Values for generic fast poll enter/exit triggers */
  FAST_POLL_STOP   = 0,
  FAST_POLL_START  = 1,

  DCVSF_EVENT_VAL_MAX = 0xffffffff
} eDCVSfEventValTypes;

/* Enum for input events */
typedef enum
{
  /* FW event to indicate modem danger level */
  DCVS_SYS_EVT_MDM_DANGER,
  DCVS_SYS_EVT_STATS,
  DCVS_SYS_EVT_FROZEN_VALUE_MAX = DCVS_SYS_EVT_STATS,
  /* set mask for events for which processing isn't deferred to a different context and is processed
   * immediately in the caller's context */
  #define SYS_EVT_TRIG_FP_CHECK_MASK \
  (~((0x1 << DCVS_SYS_EVT_MDM_DANGER) | (0x1 << DCVS_SYS_EVT_STATS)))
  
  /* Event from client indicating DCVS should run its load response algorithm */
  DCVS_SYS_EVT_TRIG_DCVS,
  /* Internal Event indicating DCVS should switch to fast poll due to high util */
  DCVS_SYS_EVT_HIGH_UTIL,
  /* events specific to usecases - there isn't any major memory saving by making a generic
   * per-tech per-client framework, so use the event types for per use case differentiation */
  DCVS_SYS_EVT_G_IDLE_PSCAN, /* specific to geran power scan */
  DCVS_SYS_EVT_L_Q6_BUMP, /* specific to LTE Q6 q6 bump up mcvs/boost replacement */

  /* mask for events that automatically trigger FP mode for a minimum amount of time */
  #define SYS_EVT_MASK_AUTO_FP_DURATION \
  ((0x1 << DCVS_SYS_EVT_HIGH_UTIL) | (0x1 << DCVS_SYS_EVT_TRIG_DCVS))

    //limit soft landing to known cases where clients expect persistent stress until trigger is released
    #define EXT_STRESS_EVT_MASK ((0x1 << DCVS_SYS_EVT_TRIG_DCVS) | (0x1 << DCVS_SYS_EVT_G_IDLE_PSCAN) | \
                                 (0x1 << DCVS_SYS_EVT_L_Q6_BUMP))
								
  /* Event from MCPM layer to indicate MCVS events */
  DCVS_SYS_EVT_MCPM_MCVS,
  /* Event from MCPM layer to indicate different tech states */
  DCVS_SYS_EVT_MCPM_TECH,
  /* Event from MCPM layer to indicate inter-RAT events */
  DCVS_SYS_EVT_MCPM_IRAT,
  /* Event from MCPM indicating a q6/bimc boost triggered */
  DCVS_SYS_EVT_MCPM_BOOST,

  DCVS_SYS_EVT_MAX
} eDCVSInput;

/* Enum for context
 * NOTE: These are bit wise flags and values must be incremented in one-bit shift intervals */
typedef enum
{
  /* triggered from a system event context */
  DCVS_FLAG_SYSEVENT_CONTEXT = 0x1,
  /* triggered from internal utilization measurement timer context */
  DCVS_FLAG_TIMER_CONTEXT    = 0x2
} eDCVSFlag;

/**
 * <!-- DCVS_processSysEvent --> 
 *  
 * @brief Processes event information sent from client and triggers DCVS to recompute load
 *  
 * @param
 *        inputCmd (in) : Pointer to an array of commands to send to DCVS. Minimum 2 uint32 values must be sent -
 *                        first uint32 input is the 'eventType' as defined by eDCVSInput and second uint32 input
 *                        is the value associated for this event. E.g. for MDM danger 2 send -
 *                          inputCmd[0] = 0 (DCVS_SYS_EVT_MDM_DANGER)
 *                          inputCmd[1] = <Danger level>
 *                        NOTE: To allow this api to be externed instead of enforcing clients to include core .h
 *                              files, the event values will be frozen until DCVS_SYS_EVT_FROZEN_VALUE_MAX.
 *
 *                        For inputCmd[0] = DCVS_SYS_EVT_TRIG_DCVS type event, clients can specific minimum duration FP
 *                        mode needs to be maintained by setting the usecs duration in inputCmd[1].
 *                        This trigger is MT safe. If multiple clients call with different min times, the maximum vote
 *                        among all clients is retained and FP mode countdown is started from the point of max vote trigger.
 *                        inputCmd[1] = 0 [Default] is minimum 50 msecs of FP mode time.
 *
 *        inputSz (in)  : Size of data (in bytes) contained in inputCmd pointer.
 *
 *        responsePtr (out): Pointer to variable sized data to send to the caller. Caller must create and manage
 *                           this memory.
 *        responseSz  (out): Size of data (in bytes) that can be stored in responsePtr safely.
 *
 * @return eDCVSAction type code sent to client indicating further action required from the client after this
 *         API returns. Also to remove dependency on .h files, the return code type is uint32. The return
 *         codes values are also frozen and not allowed to change once defined.
 *
 * @reentrancy Handling of DCVS_SYS_EVT_MDM_DANGER event is not reentrant. Caller must ensure protection on their end.
 *             For other events the API is reentrant.
 */
uint32 DCVS_processSysEvent(uint32 * inputCmd, uint32 inputSz, uint32 * responsePtr, uint32 responseSz);

/**
 * <!-- DCVS_evalFastPollSwitch --> 
 *  
 * @brief Evaluates whether DCVS can enter fast polling mode and if so, triggers the fast-DCVS state machine
 *
 * @param 
 *      inContext : DCVS context to use. If caller does not want a local context to be used, send NULL which
 *                  sets the context to be the global DCVS context.
 *      cntrlflag : eDCVSFlag type flags
 */
uint32 DCVS_evalFastPollSwitch(void * inContext, uint32 cntrlflag);

#endif
