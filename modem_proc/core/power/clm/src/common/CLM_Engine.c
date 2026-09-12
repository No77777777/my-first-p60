/*============================================================================
@file CLM_Engine.c

Implements CLM's backend - the component that uses a timer and input
from sources such as the PMU, Qurt etc. to quantify CPU utilization.
It interfaces with the frontend (CLM's client layer) via a callback.

Copyright (c) 2014-2015,2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/clm/src/common/CLM_Engine.c#2 $
============================================================================*/

#include "CLM_Engine.h"
#include "ClockDefs.h"
#include "qurt.h"
#include "qurt_consts.h"
#include "qurt_event.h"
#include "sleep_lpr.h"
#include "timer.h"
#include "npa.h"
#include "npa_resource.h"
#include "msmhwiobase.h"
#include "HALhwio.h"
#include "DDIPlatformInfo.h"
#include <DDIClock.h>
#include "stringl.h"
#include CLM_HWIO_H

/* Macro for reading Clock Gating state */
#define CLM_CGC_OVERRIDE_INM(m)      \
        in_dword_masked(HWIO_MSS_QDSP6SS_CGC_OVERRIDE_ADDR, m)

#define MAX_UINT32 0xffffffff

/* Global context for the CLM Engine */
typedef struct
{
  /* Indicates whether CLM has initialized */
  uint32 initDone;
  
  CoreMutexType *lock;

  uint32 numClients;
  CLM_Client *clientList;

  /* Struct with fields required to compute/store util info */
  CLM_EngineCalculation calc;

  /* CLM timer */
  timer_type timer;
  timer_group_type timerGroup;

  /* Query Handle to the "/clk/cpu" resource */
  npa_query_handle clkQuery;

  /* Number of HW threads on this target */
  uint32 nHWThreads;

  /* Records when the CLM timer was last started - this can change when Q6 CBs are triggered */
  uint64 startSysTick;
  /* Static record of when the CLM timer was last started */
  uint64 tmrStartSysTick;

  /* Indicates whether the CLM timer is started */
  uint32 timerStarted;

  /* Events to track Q6 clk changes */
  npa_event_handle preClkChangeEvent;
  npa_event_handle postClkChangeEvent;

  /* Interface between Engine and Client layers */
  CLM_ClientCB clientCb;

} SCLMctxt;

SCLMctxt clmEngineCtxt;


uint8 gClmDoDebugPrints = 0;


extern void CLM_ResetClientData( CLM_Client *client );

/** 
 * <!-- CLM_EngineIsClockGatingDisabled -->
 *  
 * @brief Returns TRUE if ClockGating is disabled
 */
uint32 CLM_EngineIsClockGatingDisabled( void )
{
  /* In general, we rely on the PMU for load information. But if the PMU is
   * unavailable, we use the qurt_profile API. If Clock gating is disabled, 
   * this API cannot be used.
   */
  clmEngineCtxt.calc.clkGatingDisabled = 
    ( CLM_CGC_OVERRIDE_INM( HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_CLK_EN_BMSK
                            | HWIO_MSS_QDSP6SS_CGC_OVERRIDE_CORE_RCLK_EN_BMSK )
                            != 0 );

  return clmEngineCtxt.calc.clkGatingDisabled;
}

/** 
 * <!-- DcvsQdspPMUbusy -->
 *  
 * @brief Called by QURT to indicate to CLM that the PMU is free or busy.
 * Updates the global clmPmuBusy, used by EngineMeasure.
 *
 * @param isBusy: TRUE when PMU is being used
 */
void DcvsQdspPMUbusy( int isBusy )
{
}

/**
 * @brief CLM_EngineGetClkState
 * 
 * @return : The current state of "/clk/cpu" in KHz
 */
static uint32 CLM_EngineGetClkState( void )
{
  npa_query_type result;
  CORE_VERIFY( 
    NPA_QUERY_SUCCESS == npa_query( clmEngineCtxt.clkQuery,
                                    NPA_QUERY_CURRENT_STATE, &result ) );

  return result.data.state;
}

/** 
 * <!--  CLM_EngineStartTimer -->
 * 
 * @brief Starts the CLM Engine Timer
 * @param Timer period in us
 */
static void CLM_EngineStartTimer( uint32 timeUs )
{
  CLM_LOG(1, "Start CLM timer for %u usec", timeUs);

  timer_set_64( &clmEngineCtxt.timer, timeUs, 0, T_USEC );
  clmEngineCtxt.timerStarted = TRUE;
}

/** 
 * <!-- CLM_EngineFindNextTimerPeriod -->
 *  
 * @brief Finds the next timer period from the measurement window sizes of
 * clients.
 *
 * @param elapsedTimeUs : Time elapsed from when the timer was last set
 *  
 * @return Next timer expiry period
 */
static uint32 CLM_EngineFindNextTimerPeriod(void)
{
  int minPeriodUs = 0x7FFFFFFF;
  CLM_Client *client = clmEngineCtxt.clientList;
  CORE_VERIFY_PTR( client );
  uint64 currTime = qurt_sysclock_get_hw_ticks();
  int32 origPendTm = 0;

  do
  {
    origPendTm = client->pendingTimeUs;
    /* UPDATE - there is logical concurrency in client pending time access here and during client CBs in
       CLM_EngineUpdate() which can lead to CBs not being called */

    ///* first subtract elapsedTime */
    //client->pendingTimeUs -= elapsedTimeUs;

    ///* reset timer period for client if measurement period has expired,
    //   accounting for a Xusec inaccuracy in the timer subsystem */
    ////if ( client->pendingTimeUs <= CLM_TIMER_TOLERANCE )
    ////{
    //  /* UPDATE: client info should only be reset once the client callback has been called - this is already
    //             done in CLM_ResetClientData() */
    //  //client->pendingTimeUs = client->measPeriodUs;
    ////}

    /* adjust pending times to account for extra time spent since CB was called and now, as other client CBs
       that were triggered after this client's CB could have taken significant amount of time */
    client->pendingTimeUs -= TICK_DIFF_TO_USECS(currTime, client->updateTime);

    minPeriodUs = MIN( minPeriodUs, client->pendingTimeUs );

    /* if client update had already gone out in this frame, it's pendingUs should be reset to client period */
    //if (origPendTm == client->measPeriodUs)
    //  client->pendingTimeUs = client->measPeriodUs;

    CLM_LOG(9,  "FindNextTimer (clmEngTmElapsed %u clientTmElapsed %u) (pendTm %d->%d) "
                "(minPer %u) (tmSinceClCBcall %u) (client: \"0x %x %x \") (measPer %u)",
                clmEngineCtxt.calc.timeElapsedUs, client->timeElapsedUs, origPendTm,
                client->pendingTimeUs, minPeriodUs, TICK_DIFF_TO_USECS(currTime, client->updateTime),
                client->name[0], client->name[1], client->measPeriodUs);
    
    client = client->next;

  } while ( client );

  if (minPeriodUs < 0)
    minPeriodUs = 0;

  return minPeriodUs;
}

/** 
 * <!-- CLM_EngineClkChangeAction -->
 */
void CLM_EngineClkChangeAction( uint32 prevClkKhz, uint32 newClkKhz )
{
  const char *eventLogStr = 
    ( newClkKhz > prevClkKhz ) ? "EnginePreClockChangeEvent" :
                                 "EnginePostClockChangeEvent";

  //uint32 evtChgType = ( newClkKhz > prevClkKhz ) ? 0x72e : 0x7081;

  //Locking here can deadlock between NPA resource updates in CLM_EngineTimerCallback and clock change
  //actions in here. Is it really needed to protect access to CLM context here ?
  //Core_MutexLock( clmEngineCtxt.lock );
    
  clmEngineCtxt.calc.curClkKhz = newClkKhz;

  /* If clients are registered and this callback arrived while the timer
     was still running... */
  if ( clmEngineCtxt.clientList && clmEngineCtxt.timerStarted == 1 )
  {
    uint64 endSysTick = qurt_sysclock_get_hw_ticks();
    uint64 deltaSysTick = endSysTick - clmEngineCtxt.startSysTick;
    uint64 elapsedUs = deltaSysTick * 1000000 / TICKS_PER_SEC;

    clmEngineCtxt.calc.timeElapsedUs += elapsedUs;

    clmEngineCtxt.calc.sumBudgetCycles += (deltaSysTick * prevClkKhz * 1000);

    /* Next sub-window starts */
    clmEngineCtxt.startSysTick = endSysTick;
  
    ULOG_RT_PRINTF_9( clmLog, "%s (prevClkKHz: %d) (newClkKhz: %d) "
                      "(totalTimeElapsedUs: %llu) "
                      "(sumBudgetCycles: %llu) "
                      "(startSysTick: 0x%llx)",
                      eventLogStr, prevClkKhz, clmEngineCtxt.calc.curClkKhz,
                      ULOG64_LOWWORD(clmEngineCtxt.calc.timeElapsedUs),
                      ULOG64_HIGHWORD(clmEngineCtxt.calc.timeElapsedUs),
                      ULOG64_LOWWORD(clmEngineCtxt.calc.sumBudgetCycles),
                      ULOG64_HIGHWORD(clmEngineCtxt.calc.sumBudgetCycles),
                      ULOG64_LOWWORD(clmEngineCtxt.startSysTick),
                      ULOG64_HIGHWORD(clmEngineCtxt.startSysTick) );

    /* Q6 can somehow change once sleep starts which could trigger this F3 in sleep context */ 
    //CLM_LOG(9,  "0x%x (prevClkKHz: %d) (newClkKhz: %d) "
    //                  "(totalTimeElapsedUs: 0x%x%08x) "
    //                  "(sumBudgetCycles: 0x%x%08x) "
    //                  "(startSysTick: 0x%x%08x)",
    //                  evtChgType, prevClkKhz, clmEngineCtxt.calc.curClkKhz,
    //                  ULOG64_HIGHWORD(clmEngineCtxt.calc.timeElapsedUs),
    //                  ULOG64_LOWWORD(clmEngineCtxt.calc.timeElapsedUs),
    //                  ULOG64_HIGHWORD(clmEngineCtxt.calc.sumBudgetCycles),
    //                  ULOG64_LOWWORD(clmEngineCtxt.calc.sumBudgetCycles),
    //                  ULOG64_HIGHWORD(clmEngineCtxt.startSysTick),
    //                  ULOG64_LOWWORD(clmEngineCtxt.startSysTick));
  }
  else
  {
    ULOG_RT_PRINTF_3( clmLog, "%s (prevClkKHz: %d) (newClkKhz: %d)",
                      eventLogStr, prevClkKhz, clmEngineCtxt.calc.curClkKhz );
    //CLM_LOG(3,  "0x%x (prevClkKHz: %d) (newClkKhz: %d)",
    //                  evtChgType, prevClkKhz, clmEngineCtxt.calc.curClkKhz );
  }

  //Core_MutexUnlock( clmEngineCtxt.lock );
}

/** 
 * <!-- CLM_EnginePreClkChangeCallback -->
 *  
 * @brief NPA event callback invoked before a change in state of
 * /clk/cpu. This callback will be invoked synchronous to the clkChange.
 * If the clock is ramping up, we record an endSysTick, update budgetCycles
 * and mark the start of a new window here. This means that we are budgeting
 * at the new higher clk sooner than the clock is actually at that level,
 * but this prevents issues where busyCycles end up > budgetCycles, which
 * would happen if we budgeted in the postChange callback.
 * When we are ramping down, we do the reverse, record, update and mark
 * a start in the postChange callback.
 *
 * @param user_data Ignored
 * @param cb_event_type Event type
 * @param cb_data Event data containing details of the state change
 * @param size size of Event data
 */
void CLM_EnginePreClkChangeCallback( void *userData,  unsigned int eventType,
                                     void *eventData, unsigned int size )
{
  /* This callback is invoked before the clock is changed */
  npa_prepost_change_data *changeData = (npa_prepost_change_data *)eventData;
  uint32 prevClkKhz = clmEngineCtxt.calc.curClkKhz;
  uint32 newClkKhz = changeData->to_state;
  
  if ( newClkKhz > prevClkKhz )
  {
    CLM_EngineClkChangeAction( prevClkKhz, newClkKhz );
  }
}

/** 
 * <!-- CLM_EnginePostClkChangeCallback -->
 *  
 * @brief NPA event callback invoked after a change in state of
 * /clk/cpu. This callback will be invoked synchronous to the clkChange.
 * If the clock is ramping down, we record an endSysTick, update budgetCycles
 * and mark the start of a new window here.
 *
 * @param user_data Ignored
 * @param cb_event_type Event type
 * @param cb_data Event data containing details of the state change
 * @param size size of Event data
 */
void CLM_EnginePostClkChangeCallback( void *userData,  unsigned int eventType,
                                      void *eventData, unsigned int size )
{
  /* This callback is invoked after the clock is changed */
  npa_prepost_change_data *changeData = (npa_prepost_change_data *)eventData;
  uint32 prevClkKhz = clmEngineCtxt.calc.curClkKhz;
  uint32 newClkKhz = changeData->to_state;
  
  if ( newClkKhz < prevClkKhz )
  {
    CLM_EngineClkChangeAction( prevClkKhz, newClkKhz );
  }
  else if ( newClkKhz > prevClkKhz )
  { /* This handles the rare case where the Q6clk changed to a higher
     * value than the aggregated request. In the pre-change callback, we only
     * see the aggregated request as to_state. In the post-change, we see
     * the actual new state as to_state. Correct curClkKhz if != to_state.
     */
    clmEngineCtxt.calc.curClkKhz = newClkKhz;
    //ULOG_RT_PRINTF_1( 
    //  clmLog, "EnginePostClockChangeEvent (correctedCurClkKhz: %d)",
    //  clmEngineCtxt.calc.curClkKhz );
  }
}

/** 
 * <!-- CLM_EngineMeasure --> 
 *  
 * @brief Core routine that collects PMU/other data and profiles load on
 * the system
 */
static void CLM_EngineMeasure(SCLMctxt * clmCtxt)
{
  uint32 gCycles[clmCtxt->nHWThreads];
  uint32 gCommits[clmCtxt->nHWThreads];
  uint64 endSysTick;
  uint64 sum = 0;
  uint64 deltaSysTick;
  uint64 idleCyclesQurt[clmCtxt->nHWThreads];
  uint64 elapsedUs;
  uint32 *prevCyclesNThreadsBusy = clmCtxt->calc.prevCyclesNThreadsBusy;
  uint32 *cyclesNThreadsBusy = clmCtxt->calc.cyclesNThreadsBusy;
  uint32 *packetCount = clmCtxt->calc.packetCount;
  uint32 *prevPacketCount = clmCtxt->calc.prevPacketCount;
  uint8  i;


  Core_MutexLock( clmCtxt->lock );

  clmCtxt->timerStarted = 0;

  clmCtxt->calc.totalPackets = 0;

  /* GCYCLES and GCOMMITS registers are used to replace PMU counters:
   * They return the busy cycles and packet counts for
   * 1 HW Thread busy ~ 6 HW Threads busy, respectively.
   * Sum them up to get the overall busy cycles and packet counts
   */
  for ( i = 0; i < clmCtxt->nHWThreads; i++ )
  {
    /* Read gcycles and gcommit registers */
  gCycles[i] = qurt_get_hthread_pcycles(i+1);
  gCommits[i] = qurt_get_hthread_commits(i+1);
    /* Calculate delta and sum */
  cyclesNThreadsBusy[i] = gCycles[i] - prevCyclesNThreadsBusy[i];
  sum += cyclesNThreadsBusy[i];
  packetCount[i] = gCommits[i] - prevPacketCount[i];
  clmCtxt->calc.totalPackets += packetCount[i];
  }

    /* Mark end of the budget/measurement window */
    endSysTick = qurt_sysclock_get_hw_ticks();

  if(clmCtxt->nHWThreads < 4) /* sanity check if number of HW threads is lower than 4; should not happen on this target*/
      {
    CLM_LOG(0, "ERROR: Number of HW Threads is lower than 4!");
      }
      else
      {
    //ULOG_RT_PRINTF_9( clmLog,
        //"Engine Measure (1Tbusy: %u->%u) (2Tbusy: %u->%u) (3Tbusy: %u->%u) (4Tbusy: %u->%u) (clmMsmtStTick %u)",
        //prevCyclesNThreadsBusy[0], gCycles[0], prevCyclesNThreadsBusy[1], gCycles[1],
        //prevCyclesNThreadsBusy[2], gCycles[2], prevCyclesNThreadsBusy[3], gCycles[3],
        //(uint32)clmCtxt->startSysTick);

    CLM_LOG(9,
      "Engine Measure for busy cycles (1Tbusy: %u->%u) (2Tbusy: %u->%u) (3Tbusy: %u->%u) (4Tbusy: %u->%u) (clmMsmtStTick %u)",
      prevCyclesNThreadsBusy[0], gCycles[0], prevCyclesNThreadsBusy[1], gCycles[1],
      prevCyclesNThreadsBusy[2], gCycles[2], prevCyclesNThreadsBusy[3], gCycles[3],
      (uint32)clmCtxt->startSysTick);
      //ULOG_RT_PRINTF_6( clmLog, "\tCycles with (1hwt: %u) (2hwt: %u) "
	  //                    "(3hwt: %u) (4hwt: %u) busy. (sum: %llu)",
      //                  cyclesNThreadsBusy[0], cyclesNThreadsBusy[1],
      //                  cyclesNThreadsBusy[2], cyclesNThreadsBusy[3],
      //                  ULOG64_LOWWORD(sum), ULOG64_HIGHWORD(sum) );
  CLM_LOG(6,  "\tCycles with (1Tbusy: %u) (2Tbusy: %u) "
                        "(3Tbusy: %u) (4Tbusy: %u) (sum: 0x%x%08x)",
                        cyclesNThreadsBusy[0], cyclesNThreadsBusy[1],
                        cyclesNThreadsBusy[2], cyclesNThreadsBusy[3],
                        ULOG64_HIGHWORD(sum), ULOG64_LOWWORD(sum));
  CLM_LOG(8,
      "Engine Measure for packet count (1Tbusy: %u->%u) (2Tbusy: %u->%u) (3Tbusy: %u->%u) (4Tbusy: %u->%u)",
      prevPacketCount[0], gCommits[0], prevPacketCount[1], gCommits[1],
      prevPacketCount[2], gCommits[2], prevPacketCount[3], gCommits[3]);
  CLM_LOG(5,  "\tPacket Counts with (1Tbusy: %u) (2Tbusy: %u) "
                        "(3Tbusy: %u) (4Tbusy: %u) (totalPackets: %u)",
                        packetCount[0], packetCount[1],
                        packetCount[2], packetCount[3],
                        ULOG64_LOWWORD(clmCtxt->calc.totalPackets));


  /* Addtional debug messages for 6 HW threads. [FIXME] ULOG?*/
  if(clmCtxt->nHWThreads > 4)
    {
      CLM_LOG(8,
      "Engine Measure with 6HWT: busy cycles (5Tbusy: %u->%u) (6Tbusy: %u->%u) "
      "packet count (5Tbusy: %u->%u) (6Tbusy: %u->%u)",
      prevCyclesNThreadsBusy[4], gCycles[4], prevCyclesNThreadsBusy[5], gCycles[5],
      prevPacketCount[4], gCommits[4], prevPacketCount[5], gCommits[5]);

      }
    }

    /* Busy Cycles */
    clmCtxt->calc.busyCycles += sum;

    /* Idle Cycles for threshold updates.
       TODO: Confirm this data is valid irrespective of clkGatingState */
    qurt_profile_get_idle_pcycles( idleCyclesQurt );
    for ( i = 0; i < clmCtxt->nHWThreads; i++ )
    {
    clmCtxt->calc.idleCyclesQurt[i] =
        idleCyclesQurt[i] - clmCtxt->calc.prevIdleCyclesQurt[i];
    }


  /* Compute budget Cycles */
  deltaSysTick = endSysTick - clmCtxt->startSysTick;
  elapsedUs = deltaSysTick * 1000000 / TICKS_PER_SEC;

  clmCtxt->calc.timeElapsedUs += (uint32)elapsedUs;

  clmCtxt->calc.sumBudgetCycles +=
    deltaSysTick * clmCtxt->calc.curClkKhz * 1000;

  clmCtxt->calc.nonAdjBusyCycles = clmCtxt->calc.busyCycles;

  //ULOG_RT_PRINTF_8(clmLog,
  //                  "(endSysTick: 0x%x) (totalTimeElapsedUs: %u) "
  //                  "(sumBudgetCycles: %llu) (busyCycles: %u) (nonAdjBusyCycles: %u) "
  //                  "(totalPackets: %u) (curClkKhz: %u)",
  //                  ULOG64_LOWWORD(endSysTick),
  //                  clmCtxt->calc.timeElapsedUs,
  //                  ULOG64_LOWWORD(clmCtxt->calc.sumBudgetCycles),
  //                  ULOG64_HIGHWORD(clmCtxt->calc.sumBudgetCycles),
  //                  ULOG64_LOWWORD(clmCtxt->calc.busyCycles),
  //                  ULOG64_LOWWORD(clmCtxt->calc.nonAdjBusyCycles),
  //                  ULOG64_LOWWORD(clmCtxt->calc.totalPackets),
  //                  clmCtxt->calc.curClkKhz);


  CLM_LOG(8,
           "(endSysTick: 0x%x) (totalTimeElapsedUs: %u) "
           "(sumBudgetCycles: 0x%x%08x) (busyCycles: %u) (nonAdjBusyCycles: %u) "
           "(totalPackets: %u) (curClkKhz: %u)",
           ULOG64_LOWWORD(endSysTick),
           clmCtxt->calc.timeElapsedUs,
           ULOG64_HIGHWORD(clmCtxt->calc.sumBudgetCycles),
           ULOG64_LOWWORD(clmCtxt->calc.sumBudgetCycles),
           ULOG64_LOWWORD(clmCtxt->calc.busyCycles),
           ULOG64_LOWWORD(clmCtxt->calc.nonAdjBusyCycles),
           ULOG64_LOWWORD(clmCtxt->calc.totalPackets),
           clmCtxt->calc.curClkKhz);

  Core_MutexUnlock( clmCtxt->lock );

}


/**
 * <!--  CLM_EngineInitData -->
 *
 * @brief Initialize PMU data variables before the first measurement
 */
static void CLM_EngineInitData( void )
{
  int i;
  uint64 idleCyclesQurt[clmEngineCtxt.nHWThreads];

  /* Current Busy Cycles count from Qurt */
  clmEngineCtxt.calc.prevBusyCyclesQurt = qurt_get_core_pcycles();

  /* Current Idle Cycles count from Qurt */
  qurt_profile_get_idle_pcycles( idleCyclesQurt );

  for ( i = 0; i < clmEngineCtxt.nHWThreads; i++ )
  {
    clmEngineCtxt.calc.prevIdleCyclesQurt[i] = idleCyclesQurt[i];
    clmEngineCtxt.calc.prevCyclesNThreadsBusy[i] = qurt_get_hthread_pcycles(i+1);
  clmEngineCtxt.calc.prevPacketCount[i] = qurt_get_hthread_commits(i+1);
  }

  ULOG_RT_PRINTF_2( clmLog,
    "CLM Engine Data Initialized (prevBusyCyclesQurt: 0x%x%08x)",
    ULOG64_LOWWORD(clmEngineCtxt.calc.prevBusyCyclesQurt),
    ULOG64_HIGHWORD(clmEngineCtxt.calc.prevBusyCyclesQurt));
  CLM_LOG(2,
    "CLM Engine Data Initialized (prevBusyCyclesQurt: 0x%x%08x) ",
	  ULOG64_HIGHWORD(clmEngineCtxt.calc.prevBusyCyclesQurt),
    ULOG64_LOWWORD(clmEngineCtxt.calc.prevBusyCyclesQurt));
}


/**
 * <!-- CLM_EngineGetMaxFreqKhz -->
 *
 * @brief Returns the max frequency supported on the target in KHz or
 * MAX_UINT32, if the information could not be queried.
 */
static uint32 CLM_EngineGetMaxFreqKhz( void )
{
  DalDeviceHandle       *hDALHandle;
  ClockIdType            nClockId;
  ClockFreqPlanType     *paFreqPlan = NULL;
  uint32                 nSize = 0;

  DALResult eResult = DAL_ClockDeviceAttach(DALDEVICEID_CLOCK, &hDALHandle);
  CORE_VERIFY(eResult == DAL_SUCCESS);

  eResult = DalClock_GetClockId(hDALHandle, CLK_Q6_NAME,  &nClockId);
  CORE_VERIFY(eResult == DAL_SUCCESS);

  eResult = DalClock_GetFrequencyPlan(hDALHandle, nClockId, &paFreqPlan, &nSize);
  CORE_VERIFY(eResult == DAL_SUCCESS);

  /* During pre-sil (RUMI) preparation it is possible for Dal<> APIs to be stubbed out in which
   * case there will be no error code returned, for those cases handle bad values differently */
  if (nSize == 0)
  {
    ULOG_RT_PRINTF_0(clmLog, "No support available yet to get max Q6 freq!");
    DAL_DeviceDetach(hDALHandle);

    return MAX_UINT32;
  }

  ULOG_RT_PRINTF_1(clmLog, "Max Q6 freq %u", paFreqPlan[nSize - 1].nFreqHz);

  return (paFreqPlan[nSize - 1].nFreqHz / 1000);
}

/** 
 * <!--  CLM_EnteringLowPowerMode -->
 *  
 * @brief Used as a placeholder
 */
void CLM_EnteringLowPowerMode( uint64_t wakeupTick )
{
}

/** 
 * <!--  CLM_ExitingLowPowerMode -->
 *  
 * @brief Record timestamp of last exit from CLM LPR
 */
void CLM_ExitingLowPowerMode( void )
{
  clmEngineCtxt.calc.lastLPRexitTS = qurt_sysclock_get_hw_ticks();
}

/** 
 * <!-- CLM_RegisterLPR -->
 *  
 * @brief Registers the CLM LPR with Sleep
 */
static void CLM_RegisterLPR( void *context, unsigned int event_type, 
                             void *data, unsigned int data_size )
{
  sleepLPR_registerNodelessLPR("CLM", SLEEP_ALL_LPRMS);
}


/** 
 * <!--  CLM_EngineStart -->
 * 
 * @brief We have atleast one client. Init PMU data and start
 * the timer. This callback function is setup after the first client
 * is registered and triggered after CLM_EngineInit is complete.
 */
void CLM_EngineStart( void *context, unsigned int event_type, 
                      void *data,    unsigned int data_size )
{
  Core_MutexLock( clmEngineCtxt.lock );

  CLM_EngineInitData();

  clmEngineCtxt.calc.curTimerPeriod = CLM_EngineFindNextTimerPeriod();

  /* Start the timer */
  CLM_EngineStartTimer( clmEngineCtxt.calc.curTimerPeriod );

  /* Mark the start of timer window */
  clmEngineCtxt.startSysTick = qurt_sysclock_get_hw_ticks();
  clmEngineCtxt.tmrStartSysTick = clmEngineCtxt.startSysTick;

  //ULOG_RT_PRINTF_3( clmLog, "Timer Set "
  //                  "(startSysTick: 0x%llx) (periodUs: %d)",
  //                  ULOG64_LOWWORD(clmEngineCtxt.startSysTick),
  //                  ULOG64_HIGHWORD(clmEngineCtxt.startSysTick),
  //                  clmEngineCtxt.calc.curTimerPeriod );
  CLM_LOG(3,  "Timer Set "
                    "(startSysTick: 0x%x%08x) (periodUs: %d)",
                    ULOG64_HIGHWORD(clmEngineCtxt.startSysTick),
                    ULOG64_LOWWORD(clmEngineCtxt.startSysTick),
                    clmEngineCtxt.calc.curTimerPeriod );
  Core_MutexUnlock( clmEngineCtxt.lock );
}


/** 
 * <!-- CLM_EngineRegisterClient -->
 *  
 *  @brief Registers a CLM_Client with the Engine
 *
 *  @param newClient : Client to register
 */
void CLM_EngineRegisterClient( CLM_Client *newClient )
{
  CORE_VERIFY_PTR( newClient );

  Core_MutexLock( clmEngineCtxt.lock );

  if ( clmEngineCtxt.clientList == NULL )
  {
    clmEngineCtxt.clientList = newClient;
  }
  else
  {
    CLM_Client *client = clmEngineCtxt.clientList;

    while ( client->next )
    {
      client = client->next;
    }
    client->next = newClient;
  }

  if ( clmEngineCtxt.timerStarted )
  {
    /* A timer is already running; we need to align this client's window
       with the next timer expiry */

    /* Add remaining time on the timer to the pendingTimeUs on the client */
    newClient->pendingTimeUs = 
      newClient->measPeriodUs + timer_get( &clmEngineCtxt.timer, T_USEC );
  }
  else
  {
    newClient->pendingTimeUs = newClient->measPeriodUs;

    npa_resource_available_cb( "/clm/enabled", CLM_EngineStart, NULL );
  }

  Core_MutexUnlock( clmEngineCtxt.lock );
}

/** 
 * <!--  CLM_EngineUnregisterClient -->
 * 
 * @brief Unregisters the given client from the Engine
 * @param delClient : Client to unregister
 */
void CLM_EngineUnregisterClient( CLM_Client *delClient )
{
  int found = FALSE;
  CLM_Client *client;

  CORE_VERIFY_PTR( delClient );

  Core_MutexLock( clmEngineCtxt.lock );

  client = clmEngineCtxt.clientList;
  CORE_VERIFY_PTR( client );

  if ( client == delClient )
  {
    /* Unregister client at head of list */
    clmEngineCtxt.clientList = client->next;
    found = TRUE;
  }
  else
  {
    while ( client )
    {
      if ( client->next == delClient )
      {
        client->next = client->next->next;
        found = TRUE;
      }
      client = client->next;
    }
  }

  CORE_VERIFY( found );

  ULOG_RT_PRINTF_1( clmLog, "Client \"%s\" unregistered", delClient->name );

  CLM_LOG(4,  "Client \"0x%x %x %x %x\" unregistered", delClient->name[0], delClient->name[1],
                                              delClient->name[2], delClient->name[3] );

  /* Free up the memory */
  Core_Free( delClient->idleCycles );
  Core_Free( delClient->loadInfo );
  Core_Free( delClient );

  if ( clmEngineCtxt.clientList == NULL )
  {
    /* Stop the active timer */
    timer_clr( &clmEngineCtxt.timer, T_USEC );
    clmEngineCtxt.timerStarted = 0;
  }

  Core_MutexUnlock( clmEngineCtxt.lock );
}

/** 
 * <!-- CLM_adjustTimer --> 
 * @brief Registers DCVS as client with new polling period 
 * @param newPeriod: time in msec
 * @param clientName: client to be registered
 * @param pClmClient: handle to client
 * @param cb: Callback function to receive updated data
 * @param context: Data that was passed in by client during 
 * registration 
 *  
 * @return client handle 
*/
CLM_HandleType CLM_adjustTimer(uint32 newPeriod, const char * clientName, CLM_HandleType pClmClient, 
                               CLM_CallbackFuncPtr cb, void * context)
{
  CORE_VERIFY( newPeriod != 0 );

  Core_MutexLock( clmEngineCtxt.lock );

  /* Set the new measurement period - if CLM CB timer is already running reset it with new timings - no need to bcast
   * current usage immediately as caller should handle any response required for loading uptill the point of this call.
   * If timer is not running, then this is getting called inside the client's CLM cb and CLM_EngineTimerCallback
   * will automatically proceed to calculating next timer value to set after all CBs are done */
  pClmClient->pendingTimeUs = 0;
  CLM_SetClientMeasurementPeriod(pClmClient, (newPeriod) * 1000);

  if (clmEngineCtxt.timerStarted == 1)
  {
    uint32 remTimeUsec = 0;

    pClmClient->updateTime = qurt_sysclock_get_hw_ticks();
    //client has been updated with load info uptill this point, reset client load info
    CLM_ResetClientData(pClmClient);

    //reset CLM timer to start earlier of remaining time or the current client's new meas period
    //NOTE: do not use CLM_EngineFindNextTimerPeriod as it affects pending times for all clients - it can only
    //be used safely when update was evaluated for all clients and not like here when only one client is of concern
    remTimeUsec = timer_clr_64( &clmEngineCtxt.timer, T_USEC );
    timer_set_64( &clmEngineCtxt.timer, MIN(remTimeUsec, pClmClient->measPeriodUs), 0, T_USEC );

    CLM_LOG(4, "set new timer to %u = min(%u, %u), tick 0x%x",
               MIN(remTimeUsec, pClmClient->measPeriodUs), remTimeUsec, pClmClient->measPeriodUs,
               (uint32)qurt_sysclock_get_hw_ticks());
  }

  Core_MutexUnlock( clmEngineCtxt.lock );

  //return new handle back to client
  return pClmClient;
}

/**
 * Resets CLM context data
 */
static void resetCLMcontext(SCLMctxt * clmCtxt)
{
  int i = 0;
  /* Reset some of the data */
  clmCtxt->calc.sumBudgetCycles = 0;
  clmCtxt->calc.timeElapsedUs = 0;
  clmCtxt->calc.busyCycles = 0;
  clmCtxt->calc.nonAdjBusyCycles = 0;
  clmCtxt->calc.totalPackets = 0;

  for ( i = 0; i < clmCtxt->nHWThreads; i++ )
  {
    clmCtxt->calc.cyclesNThreadsBusy[i] = 0;
  clmCtxt->calc.packetCount[i] = 0;
  clmCtxt->calc.prevCyclesNThreadsBusy[i] = qurt_get_hthread_pcycles(i+1);
  clmCtxt->calc.prevPacketCount[i] = qurt_get_hthread_commits(i+1);
  }

  if ( !CLM_EngineIsClockGatingDisabled() )
  {
    uint64 idleCyclesQurt[clmCtxt->nHWThreads];
    
    /* Gather Idle Cycles to compare against (for threshold based updates) at
       timer expiry */
    qurt_profile_get_idle_pcycles( idleCyclesQurt );
    for ( i = 0; i < clmCtxt->nHWThreads; i++ )
    {
      clmCtxt->calc.prevIdleCyclesQurt[i] = idleCyclesQurt[i];
    }

    /* Gather Busy Cycles to compare against (if PMU is unavailable) at
       timer expiry */
    clmCtxt->calc.prevBusyCyclesQurt = qurt_get_core_pcycles();
  }
  else
  {
    /* TODO: Do something about prevIdleCyclesQurt here as well? */
    clmCtxt->calc.prevBusyCyclesQurt = 0xFFFFFFFFFFFFFFFF;
  }

  //ULOG_RT_PRINTF_9( clmLog,
  //                  "Timer Set (startSysTick: 0x%x) (periodUs: %d) (prev1Tbusy: %u) (prev2Tbusy: %u) (prev3Tbusy: %u) (prev4Tbusy: %u) "
  //                  "(prevBusyCyclesQurt: %llu) (clmPmuBusy %u)",
  //                  ULOG64_LOWWORD(clmCtxt->startSysTick),
  //                  clmCtxt->calc.curTimerPeriod,
  //                  prevCyclesNThreadsBusy[0], prevCyclesNThreadsBusy[1],
  //                  prevCyclesNThreadsBusy[2], prevCyclesNThreadsBusy[3],
  //                  ULOG64_LOWWORD(clmCtxt->calc.prevBusyCyclesQurt),
  //                  ULOG64_HIGHWORD(clmCtxt->calc.prevBusyCyclesQurt),
  //                  clmPmuBusy);
  CLM_LOG(8,
          "resetCLMcontext: Timer Set (startSysTick: 0x%x) (periodUs: %d) (prev1Tbusy: %u) (prev2Tbusy: %u) (prev3Tbusy: %u) (prev4Tbusy: %u) "
          "(prevBusyCyclesQurt: 0x%x%08x)",
          ULOG64_LOWWORD(clmCtxt->startSysTick),
          clmCtxt->calc.curTimerPeriod,
          clmCtxt->calc.prevCyclesNThreadsBusy[0], clmCtxt->calc.prevCyclesNThreadsBusy[1],
          clmCtxt->calc.prevCyclesNThreadsBusy[2], clmCtxt->calc.prevCyclesNThreadsBusy[3],
          ULOG64_HIGHWORD(clmCtxt->calc.prevBusyCyclesQurt),
          ULOG64_LOWWORD(clmCtxt->calc.prevBusyCyclesQurt));
}

/**
 * <!-- CLM_EngineTimerCallback -->
 * 
 * @brief CLM's timer callback
 */
void CLM_EngineTimerCallback( timer_cb_data_type data )
{
  uint64 startTick = qurt_sysclock_get_hw_ticks();

  /* Gather data, do some of the arithmetic */
  CLM_EngineMeasure(&clmEngineCtxt);

  Core_MutexLock( clmEngineCtxt.lock );

  /* Call into the client layer */
  clmEngineCtxt.clientCb( &clmEngineCtxt.calc, NULL );

  /* The callbacks may take several msecs to execute during which time load may have already accrued
   * Call clients to recompute their actions before resetting stats
   * UPDATE: This adds noticeable amount to util as CBs use MIPS as well resulting in excessive bumps
   */

  /* Compute next timer period */
  clmEngineCtxt.calc.curTimerPeriod = CLM_EngineFindNextTimerPeriod();

  startTick = qurt_sysclock_get_hw_ticks();
  /* Start the timer */
  CLM_EngineStartTimer( clmEngineCtxt.calc.curTimerPeriod );
  
  clmEngineCtxt.timerStarted = 1;

  /* Mark the start of this timer window */
  clmEngineCtxt.startSysTick = qurt_sysclock_get_hw_ticks();
  clmEngineCtxt.tmrStartSysTick = clmEngineCtxt.startSysTick;
  
  resetCLMcontext(&clmEngineCtxt);
 
  Core_MutexUnlock( clmEngineCtxt.lock );
}


/** 
 * <!-- CLM_EngineInit -->
 *
 * @brief Callback function triggered on availability of dependencies.
 * We initialize CLM here.
 */
void CLM_EngineInit( void *context, unsigned int event_type,
                     void *data, unsigned int data_size )
{
  qurt_sysenv_max_hthreads_t hwtQurt;
  uint32 nHWThreads;
  void *p;
  uint32 size;

  Core_MutexLock( clmEngineCtxt.lock );

  /* Query the number of HW threads on this target */
  qurt_sysenv_get_max_hw_threads( &hwtQurt );
  nHWThreads = clmEngineCtxt.nHWThreads = hwtQurt.max_hthreads;

  /* Create a query handle to /clk/cpu */
  clmEngineCtxt.clkQuery = npa_create_query_handle( "/clk/cpu" );

  clmEngineCtxt.calc.maxClkKhz = CLM_EngineGetMaxFreqKhz();
  if (clmEngineCtxt.calc.maxClkKhz == MAX_UINT32)
  {
    /* The /clk/cpu resource was either unavailable or stubbed. This is okay
       if we are executing in Virtio or RUMI */
    CORE_VERIFY( DalPlatformInfo_Platform() == DALPLATFORMINFO_TYPE_RUMI ||
                 DalPlatformInfo_Platform() == DALPLATFORMINFO_TYPE_VIRTIO );

    /* Set cur and maxClk to a possible "safe" value. Util will be incorrect */
    clmEngineCtxt.calc.maxClkKhz = 115000;
    clmEngineCtxt.calc.curClkKhz = 115000;
  }
  else
  {
    clmEngineCtxt.calc.curClkKhz = CLM_EngineGetClkState();
  }

  /* Allocate memory for
   * idleCyclesQurt[], prevIdleCyclesQurt[],
   * cyclesNThreadsBusy[], prevCyclesNThreadsBusy[],
   * packetCount[], prevPacketCount[] */
  size = nHWThreads *
         ( 2*sizeof(uint64) + 4*sizeof(uint32));
  p = Core_Malloc( size );
  memset( p, 0, size );

  clmEngineCtxt.calc.idleCyclesQurt = p;
  clmEngineCtxt.calc.prevIdleCyclesQurt =
    clmEngineCtxt.calc.idleCyclesQurt + nHWThreads;
  clmEngineCtxt.calc.cyclesNThreadsBusy =
    (uint32 *)(clmEngineCtxt.calc.prevIdleCyclesQurt + nHWThreads);
  clmEngineCtxt.calc.prevCyclesNThreadsBusy =
    clmEngineCtxt.calc.cyclesNThreadsBusy + nHWThreads;
  clmEngineCtxt.calc.packetCount =
    clmEngineCtxt.calc.prevCyclesNThreadsBusy + nHWThreads;
  clmEngineCtxt.calc.prevPacketCount =
    clmEngineCtxt.calc.packetCount + nHWThreads;

  /* Create the timer */
  timer_def( &clmEngineCtxt.timer, &clmEngineCtxt.timerGroup,
             NULL, 0, CLM_EngineTimerCallback, 0 );

  /* Set the timer as deferrable */
  timer_group_set_deferrable( &clmEngineCtxt.timerGroup, TRUE );

  /* Register for the clk change events */
  CORE_VERIFY_PTR( clmEngineCtxt.postClkChangeEvent =
    npa_create_custom_event( "/clk/cpu", "CLM_ClkPostChangeEvent",
                             NPA_TRIGGER_POST_CHANGE_EVENT,
                             NULL, CLM_EnginePostClkChangeCallback, NULL ) );

  CORE_VERIFY_PTR( clmEngineCtxt.preClkChangeEvent =
    npa_create_custom_event( "/clk/cpu", "CLM_ClkPreChangeEvent",
                             NPA_TRIGGER_PRE_CHANGE_EVENT,
                             NULL, CLM_EnginePreClkChangeCallback, NULL ) );

  Core_MutexUnlock( clmEngineCtxt.lock );

  npa_define_marker( "/clm/enabled" );
}

/** 
 * <!--  CLM_EngineCreate -->
 *  
 * @brief Set up callbacks to complete init, after dependencies are available.
 *
 * @param clientCb : Function that is to be the interface between the
 * engine and client layers
 */
void CLM_EngineCreate( CLM_ClientCB clientCb )
{
  static const char *basicDeps[] =
  {
    "/clk/cpu",
    "/init/timer"
  };

  clmEngineCtxt.clientCb = clientCb;

  CORE_VERIFY_PTR( 
    clmEngineCtxt.lock = Core_MutexCreate( CORE_MUTEXATTR_DEFAULT ) );

  /* Register to be notified when basic resource dependencies are available */
  npa_resources_available_cb( NPA_ARRAY( basicDeps ), 
                              CLM_EngineInit, NULL );

  /* Register to be notified when the sleep LPR node is available */
  //npa_resource_available_cb( SLEEP_LPR_NODE_NAME, CLM_RegisterLPR, NULL );
}


/**
 * <!-- CLM_EngineGetClients -->
 * @return Pointer to the list of clients
 */
CLM_Client* CLM_EngineGetClients( void )
{
  return clmEngineCtxt.clientList;
}

/**
 * <!-- CLM_EngineGetNumHWThreads -->
 * @return The number of HW threads on the target
 */
uint32 CLM_EngineGetNumHWThreads( void )
{
  return clmEngineCtxt.nHWThreads;
}

uint64 CLM_EngineGetStartTick( void )
{
  return clmEngineCtxt.tmrStartSysTick;
}

/**
 * <!-- CLM_EngineGetLastLPRExitTime -->
 * @brief Returns the time of the last LPR exit 
 *
 * @return Timetick of last PC exit
 */
uint64 CLM_EngineGetLastLPRExitTime( void )
{
  return clmEngineCtxt.calc.lastLPRexitTS;
}

/** 
 * <!-- CLM_computeLoadSinceLastMsmt -->
 * @brief Computes utilization uptil this point and broadcasts stats to the updateClient. 
 * This function does not alter any states in the global clmEngineCtxt. It 
 * simply peeks at the PMU stats since last measurement and 
 * reports utilization up till the point this function is called. 
 *  
 * @param clmClient: pointer to the client that should be updated with new load info
 *
 */
uint8 CLM_computeLoadSinceLastMsmt(CLM_Client * updateClient)
{
  //instantiate local copy of clm stats struct
  SCLMctxt clmInfo;

  memset(&clmInfo, 0, sizeof(clmInfo));

  CORE_VERIFY_PTR(updateClient);

  if (clmEngineCtxt.clientCb == NULL) /* called too early before CLM started */
    return 1;

  CORE_VERIFY_PTR(clmInfo.lock = Core_MutexCreate(CORE_MUTEXATTR_DEFAULT));

  Core_MutexLock(clmEngineCtxt.lock);

  /* set the data in local context to be what global CLM data points to be able to calculate the busy cycles since last
   * CLM timer based measurement */
  clmInfo.nHWThreads = clmEngineCtxt.nHWThreads;
  clmInfo.startSysTick = clmEngineCtxt.startSysTick;
  clmInfo.timerStarted = clmEngineCtxt.timerStarted;
  clmInfo.clientCb = clmEngineCtxt.clientCb;

  memscpy(&clmInfo.calc, sizeof(CLM_EngineCalculation), &clmEngineCtxt.calc, sizeof(CLM_EngineCalculation));

  Core_MutexUnlock(clmEngineCtxt.lock);

  //call EngineMeasure to get utilization data
  CLM_EngineMeasure(&clmInfo);

  /* Call into the client layer */
  clmInfo.clientCb(&clmInfo.calc, updateClient);

  /* Update the client context with new info */

  ////calculate utilization based on stats from engine measure
  //budgetCycles = (clmInfo.calc.sumBudgetCycles)/(TICKS_PER_SEC);
  //busyPctCurClk = (clmInfo.calc.busyCycles * 100)/(budgetCycles);

  ////check if busyPctCurClk is greater than 100 due to uncaught errors, reset to 99 to be safe
  //if (busyPctCurClk > 100) 
  //{
  //  busyPctCurClk = 99;
  //}
  ////log info to CLM Log and ULog
  //ULOG_RT_PRINTF_2( clmLog, "\tcomputeLoadSinceLastMsmt: Utilization Update (utilPctAtCurrClk: %u) (totalTimeElapsed: %u)",
  //                   busyPctCurClk, (clmInfo.calc.timeElapsedUs)); 
  //CLM_LOG(2,"\tcomputeLoadSinceLastMsmt: Utilization Update (utilPctAtCurrClk: %u) (totalTimeElapsed: %u)", 
  //        busyPctCurClk, (clmInfo.calc.timeElapsedUs));

  ////populate new utilization
  //*loadSinceLastMsmt = busyPctCurClk;
  
  Core_MutexDestroy(clmInfo.lock);

  return 0;
}

void * CLM_GetLoadInfoPtr(CLM_HandleType client)
{
  CORE_VERIFY_PTR(client);

  return client->loadInfo;
}

uint64 CLM_GetClientUpdateTime(CLM_HandleType client)
{
  CORE_VERIFY_PTR(client);

  return client->updateTime;
}
