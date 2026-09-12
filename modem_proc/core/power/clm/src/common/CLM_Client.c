/*============================================================================
@file CLM_Client.c

Implements CLM's frontend - the client layer

Copyright (c) 2014-2015 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/clm/src/common/CLM_Client.c#2 $
============================================================================*/

#include "CLM.h"
#include "CLM_Engine.h"
#include "CoreVerify.h"
#include "CoreMutex.h"
#include "ULogFront.h"
#include "CLM_Internal.h"

#define MAX_UINT32 0xffffffff
#define CPU_MAXED_OUT_PCT 90

#define DEFAULT_CALC    0x1
#define PARALLELISM_ADJ 0x2

/* Defined here; for use by both this layer and the engine */
ULogHandle clmLog;

/** 
 * <!-- CLM_UpdateClient --> 
 * 
 * @brief Populates the client's loadInfo structure from engineCalc and
 * invokes the client's registered callback with this info.
 * 
 * @param client : pointer to the client
 * @param engineCalc : Load data from Engine
 * @param updateReason : One of CLM_UpdateReasonEnumType
 * 
 * @return None 
 */
static void CLM_UpdateClient( CLM_Client *client,
                              CLM_EngineCalculation *engineCalc,
                              CLM_UpdateReasonEnumType updateReason )
{
  uint32 i;
  uint32 nThreads = CLM_EngineGetNumHWThreads();

  if ( client->type == CLM_CLIENT_BASIC_CPUUTIL )
  {
    CLM_LoadInfoBasicStructType *loadInfo = client->loadInfo;

    loadInfo->updateReason = updateReason;
    loadInfo->currentClkKhz = engineCalc->curClkKhz;

    loadInfo->utilPctAtCurrClk = client->busyPctCurClk;
	loadInfo->nonAdjUtilPctAtCurrClk = client->nonAdjBusyPctCurClk;
    loadInfo->utilPctAtMaxClk = client->busyPctMaxClk;
    loadInfo->timeElapsedUs = client->timeElapsedUs;
    
    ULOG_RT_PRINTF_9( clmLog, 
      "Client Update (client 0x%x) (clientPendTm %u) (clientUpdateTm 0x%x) (utilPctAtCurrClk: adj %u nonAdj %u) (utilPctAtMaxClk: %u) "
      "(busyCycles %u) (nonAdjBusyCycls %u) (updateReason: %u)",
      client->name[0] << 24 | client->name[1] << 16 | client->name[2] << 8 | client->name[3],
      client->pendingTimeUs, (uint32)client->updateTime, loadInfo->utilPctAtCurrClk, loadInfo->nonAdjUtilPctAtCurrClk,
      loadInfo->utilPctAtMaxClk, client->busyCycles, client->nonAdjBusyCycles, loadInfo->updateReason );
    CLM_LOG(9,
      "Client Update (client 0x%x) (clientPendTm %u) (clientUpdateTm 0x%x) (utilPctAtCurrClk: adj %u nonAdj %u) (utilPctAtMaxClk: %u) "
      "(busyCycles %u) (nonAdjBusyCycls %u) (updateReason: %u)",
      client->name[0] << 24 | client->name[1] << 16 | client->name[2] << 8 | client->name[3],
      client->pendingTimeUs, (uint32)client->updateTime, loadInfo->utilPctAtCurrClk, loadInfo->nonAdjUtilPctAtCurrClk,
      loadInfo->utilPctAtMaxClk, client->busyCycles, client->nonAdjBusyCycles, loadInfo->updateReason );
  }
  else if ( client->type == CLM_CLIENT_EXTENDED_CPUUTIL )
  {
    CLM_LoadInfoExtendedStructType *loadInfo = client->loadInfo;

    loadInfo->updateReason = updateReason;
    loadInfo->currentClkKhz = engineCalc->curClkKhz;
    loadInfo->utilPctAtCurrClk = client->busyPctCurClk;
	loadInfo->nonAdjUtilPctAtCurrClk = client->nonAdjBusyPctCurClk;
    loadInfo->utilPctAtMaxClk = client->busyPctMaxClk;
    loadInfo->timeElapsedUs = client->timeElapsedUs;
    loadInfo->timeElapsedSinceCB = (uint32)((int32)client->measPeriodUs - client->pendingTimeUs);
    loadInfo->budgetCycles = client->budgetCycles;
    loadInfo->busyCycles = client->busyCycles;
	loadInfo->nonAdjBusyCycles = client->nonAdjBusyCycles;
    loadInfo->totalPackets = client->totalPackets;
    loadInfo->measPeriodUs = client->measPeriodUs;
    loadInfo->pCPPx1000 = client->pCPPx1000;
    
    for ( i = 0; i < nThreads; i++ )
    {
      loadInfo->cyclesNThreadsBusy[i] = client->cyclesNThreadsBusy[i];
    }
    
    ULOG_RT_PRINTF_9(clmLog, 
        "Client Update (client 0x%x) (utilPct: adj %u nonAdj %u) (busyCycles: adj %u nonAdj %u) "
        "(budgetCycles : %u) (pCPPx1000: %u) (clientUpdateTm 0x%x) (tmElapsedSinceCB %u) ",
        client->name[0] << 24 | client->name[1] << 16 | client->name[2] << 8 | client->name[3],
        loadInfo->utilPctAtCurrClk, loadInfo->nonAdjUtilPctAtCurrClk,
        (uint32)client->busyCycles, client->nonAdjBusyCycles,
        (uint32)client->budgetCycles, loadInfo->pCPPx1000, (uint32)client->updateTime, loadInfo->timeElapsedSinceCB);
      CLM_LOG(9,
        "Client Update (client 0x%x) (utilPct: adj %u nonAdj %u) (busyCycles: adj %u nonAdj %u) "
        "(budgetCycles : %u) (pCPPx1000: %u) (clientUpdateTm 0x%x) (tmElapsedSinceCB %u) ",
        client->name[0] << 24 | client->name[1] << 16 | client->name[2] << 8 | client->name[3],
        loadInfo->utilPctAtCurrClk, loadInfo->nonAdjUtilPctAtCurrClk,
        (uint32)client->busyCycles, client->nonAdjBusyCycles,
        (uint32)client->budgetCycles, loadInfo->pCPPx1000, (uint32)client->updateTime, loadInfo->timeElapsedSinceCB);
  }
  else if ( client->type == CLM_CLIENT_LOAD_INFO )
  {
    CLM_LoadInfoStructType *loadInfo = client->loadInfo;

    loadInfo->timeElapsedUs = client->timeElapsedUs;
    loadInfo->updateReason = updateReason;

      for ( i = 0; i < nThreads; i++ )
      {
      loadInfo->cyclesNThreadsBusy[i] = client->cyclesNThreadsBusy[i];
      }

  if (nThreads == 4)
  {
      ULOG_RT_PRINTF_4( clmLog, "\tClient Update "
        "(1Tbusy: %u) (2Tbusy: %u) (3Tbusy: %u) (4Tbusy: %u) ",
        loadInfo->cyclesNThreadsBusy[0], loadInfo->cyclesNThreadsBusy[1],
        loadInfo->cyclesNThreadsBusy[2], loadInfo->cyclesNThreadsBusy[3]);
      CLM_LOG(4,  "\tClient Update "
        "(1Tbusy: %u) (2Tbusy: %u) (3Tbusy: %u) (4Tbusy: %u) ",
        loadInfo->cyclesNThreadsBusy[0], loadInfo->cyclesNThreadsBusy[1],
        loadInfo->cyclesNThreadsBusy[2], loadInfo->cyclesNThreadsBusy[3]);
    }
/*
  else if (nThreads == 6)
      {
    ULOG_RT_PRINTF_6( clmLog, "\tClient Update "
        "(1Tbusy: %u) (2Tbusy: %u) (3Tbusy: %u) (4Tbusy: %u) (5Tbusy: %u) (6Tbusy: %u)",
      loadInfo->cyclesNThreadsBusy[0], loadInfo->cyclesNThreadsBusy[1],
        loadInfo->cyclesNThreadsBusy[2], loadInfo->cyclesNThreadsBusy[3],
        loadInfo->cyclesNThreadsBusy[4], loadInfo->cyclesNThreadsBusy[5]);
      CLM_LOG(6,  "\tClient Update "
        "(1Tbusy: %u) (2Tbusy: %u) (3Tbusy: %u) (4Tbusy: %u) (5Tbusy: %u) (6Tbusy: %u)",
      loadInfo->cyclesNThreadsBusy[0], loadInfo->cyclesNThreadsBusy[1],
        loadInfo->cyclesNThreadsBusy[2], loadInfo->cyclesNThreadsBusy[3],
        loadInfo->cyclesNThreadsBusy[4], loadInfo->cyclesNThreadsBusy[5]);
    }
*/
  }

  /* this trigger is not from regular CLM timer CB so don't invoke client timer based CBs */
  if (!(updateReason & CLM_UPDATE_REASON_ASYNC_UPDATE))
  {
    client->cbFunc( client->loadInfo, client->cbData );
  }
}

/** 
 * <!-- CLM_CheckBusyHighThreshold --> 
 *  
 * @brief Checks if utilization of atleast nThreadsForHigh HWTs
 * is above the client-specified highThreshold for the given window
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 * 
 * @return TRUE, if utilization of atleast nThreadsForHigh HWTs
 * is above highThreshold
 */
static uint32 CLM_CheckBusyHighThreshold( CLM_Client *client,
                                          CLM_EngineCalculation *engineCalc )
{
  int i;
  uint64 busyCyclesThread;
  uint32 busyPctThread;
  uint32 nThreadsBusyHigh = 0;
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  for ( i = 0; i < nHWThreads; i++ )
  {
    if ( client->budgetCycles / nHWThreads > client->idleCycles[i] )
    {
      busyCyclesThread = 
        ( client->budgetCycles / nHWThreads ) - client->idleCycles[i];
    }
    else
    {
      busyCyclesThread = 0;
    }

    busyPctThread = busyCyclesThread * nHWThreads * 100 / client->budgetCycles;

    /* Scale to max clock */
    busyPctThread = 
      busyPctThread * engineCalc->curClkKhz / engineCalc->maxClkKhz;

    if ( busyPctThread > client->highThresholdPct )
    {
      nThreadsBusyHigh++;
    }
  }

  return ( nThreadsBusyHigh >= client->nThreadsForHigh ) ? TRUE : FALSE;
}

/** 
 * <!-- CLM_CheckBusyLowThreshold -->
 * 
 * @brief Checks if utilization of atleast nThreadsForLow HWTs
 * is below the client-specified lowThreshold for the given window
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 *
 * @return TRUE, if CPU Utilization of atleast nThreadsForLow HWTs
 * is below lowThreshold 
 */
static uint32 CLM_CheckBusyLowThreshold( CLM_Client *client,
                                         CLM_EngineCalculation *engineCalc )
{
  int i;
  uint64 busyCyclesThread;
  uint32 busyPctThread;
  uint32 nThreadsBusyLow = 0;
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  for ( i = 0; i < nHWThreads; i++ )
  {
    if ( client->budgetCycles / nHWThreads > client->idleCycles[i] )
    {
      busyCyclesThread = 
        ( client->budgetCycles / nHWThreads ) - client->idleCycles[i];
    }
    else
    {
      busyCyclesThread = 0;
    }

    busyPctThread = busyCyclesThread * nHWThreads * 100 / client->budgetCycles;

    /* Scale to max clock */
    busyPctThread = 
      busyPctThread * engineCalc->curClkKhz / engineCalc->maxClkKhz;

    if ( busyPctThread <= client->lowThresholdPct )
    {
      nThreadsBusyLow++;
    }
  }

  return ( nThreadsBusyLow >= client->nThreadsForLow ) ? TRUE : FALSE;
}

/** 
 * <!-- CLM_AdjustForParallelism -->
 *  
 * @brief Adjusts busyCycles count by accounting for parallelism
 * on a multi-HWT system.
 *
 * @param client : The client
 */
static CLM_INLINE void CLM_AdjustForParallelism( CLM_Client *client )
{
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  if (nHWThreads != 4 && nHWThreads != 6)
  {
    ERR_FATAL("Incorrect number of HW threads %u %u %u", nHWThreads, 0, 0);
  }

  /* adjustedBusyCycles = busyCycles - 2/3 * cycles1hwtBusy - 1/3 * cycles2hwtBusy */
  uint64 adjustedBusyCycles = client->busyCycles -
                              ( 2 * client->cyclesNThreadsBusy[0] / 3 ) -
                              ( client->cyclesNThreadsBusy[1] / 3 );

#ifdef CLM_EXTRA_LOGS
  if (nHWThreads == 4)
  {
    //ULOG_RT_PRINTF_8(
    //  clmLog, "\tAdjustForParallelism (busyCycles: %llu) "
    //  "(cycles1hwtBusy: %d) (cycles2hwtBusy: %d) "
    //  "(cycles3hwtBusy: %d) (cycles4hwtBusy: %d) (adjustedBusyCycles: %llu)",
    //  ULOG64_LOWWORD(client->busyCycles), ULOG64_HIGHWORD(client->busyCycles),
    //  client->cyclesNThreadsBusy[0], client->cyclesNThreadsBusy[1],
    //  client->cyclesNThreadsBusy[2], client->cyclesNThreadsBusy[3],
    //  ULOG64_LOWWORD(adjustedBusyCycles), ULOG64_HIGHWORD(adjustedBusyCycles) );

    CLM_LOG(6,
      "\tAdjustForParallelism (busyCycles: %u) "
      "(cycles1hwtBusy: %d) (cycles2hwtBusy: %d) "
      "(cycles3hwtBusy: %d) (cycles4hwtBusy: %d) (adjustedBusyCycles: %u)",
      ULOG64_LOWWORD(client->busyCycles),
      client->cyclesNThreadsBusy[0], client->cyclesNThreadsBusy[1],
      client->cyclesNThreadsBusy[2], client->cyclesNThreadsBusy[3],
      ULOG64_LOWWORD(adjustedBusyCycles));
  }
  else if (nHWThreads == 6)
  {
    CLM_LOG(8,
      "\tAdjustForParallelism (busyCycles: %u) "
      "(cycles1hwtBusy: %d) (cycles2hwtBusy: %d) (cycles3hwtBusy: %d) "
      "(cycles4hwtBusy: %d) (cycles5hwtBusy: %d) (cycles6hwtBusy: %d) "
      "(adjustedBusyCycles: %u)",
      ULOG64_LOWWORD(client->busyCycles),
      client->cyclesNThreadsBusy[0], client->cyclesNThreadsBusy[1],
      client->cyclesNThreadsBusy[2], client->cyclesNThreadsBusy[3],
      client->cyclesNThreadsBusy[4], client->cyclesNThreadsBusy[5],
      ULOG64_LOWWORD(adjustedBusyCycles));
  }
#endif

  client->busyCycles = adjustedBusyCycles;
}

/** 
 * <!-- CLM_ComputeUtil -->
 *  
 * @brief Computes utilization based on data in client window
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 */
static CLM_INLINE void CLM_ComputeUtil( CLM_Client *client,
                                        CLM_EngineCalculation *engineCalc, uint32 controlFlag )
{
  if (controlFlag & DEFAULT_CALC)
  {
    //cpp & non-adjusted utilization only need to be calculated once during the default call
    client->pCPPx1000 = (client->totalPackets != 0 ? client->nonAdjBusyCycles * 1000 / client->totalPackets : 0);
    client->nonAdjBusyPctCurClk = client->nonAdjBusyCycles * 100 / client->budgetCycles;
    client->busyPctOneThread = (uint64)client->cyclesNThreadsBusy[0] * 100 / client->budgetCycles;

    if ( client->busyPctOneThread > 100 )
    {
      client->busyPctOneThread = 99;
    }

    client->busyPctMaxClk = client->busyPctCurClk * engineCalc->curClkKhz / engineCalc->maxClkKhz;
  }

  client->busyPctCurClk = client->busyCycles * 100 / client->budgetCycles;

  if ( client->busyPctCurClk > 100 )
  {
    client->busyPctCurClk = 99;
  }

  CLM_LOG(9,  
    "Compute Util (busyCycles: nonAdj 0x%08x adj 0x%08x) (budgetCycles : 0x%08x) "
    "(cycles1hwtBusy : %u) (busyPctCurClk: nonAdj %u adj %u) (busyPctMaxClk: %u) "
    "(busyPctOneThread: %u) (pCPPx1000: %u)",
    (uint32)client->nonAdjBusyCycles, (uint32)(client->busyCycles), (uint32)(client->budgetCycles), 
    client->cyclesNThreadsBusy[0], client->nonAdjBusyPctCurClk, client->busyPctCurClk,
    client->busyPctMaxClk, client->busyPctOneThread, client->pCPPx1000);
}


/** 
 * <!-- CLM_ResetClientData -->
 *  
 * @brief Resets client measurement data in prep for next window.
 *
 * @param client : The client
 */
void CLM_ResetClientData( CLM_Client *client )
{
  int i;
  uint32 nThreads = CLM_EngineGetNumHWThreads();

  client->budgetCycles = 0;
  client->busyCycles = 0;
  client->nonAdjBusyCycles = 0;
  client->totalPackets = 0;
  client->timeElapsedUs = 0;
  client->pCPPx1000 = 0;
  client->pendingTimeUs = client->measPeriodUs;

  for ( i = 0; i < nThreads; i++ )
  {
    client->idleCycles[i] = 0;
    client->cyclesNThreadsBusy[i] = 0;
  }
}

/** 
 * <!-- CLM_UpdateClientData -->
 *  
 * @brief Updates client-specific load data based on engine measurements
 * at every timer expiry.
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 */
void CLM_UpdateClientData( CLM_Client *client,
                           CLM_EngineCalculation *engineCalc )

{
  int i;
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  client->budgetCycles += engineCalc->sumBudgetCycles / TICKS_PER_SEC;
  client->busyCycles += engineCalc->busyCycles;
  client->nonAdjBusyCycles += engineCalc->nonAdjBusyCycles;
  client->timeElapsedUs += engineCalc->timeElapsedUs;

    client->totalPackets += engineCalc->totalPackets;


  for ( i = 0; i < nHWThreads; i++ )
  {
    client->idleCycles[i] += engineCalc->idleCyclesQurt[i];
    client->cyclesNThreadsBusy[i] += engineCalc->cyclesNThreadsBusy[i];
  }
}

static int32 getTimeTolerance(CLM_Client *client, CLM_UpdateReasonEnumType updateReason)
{
  int32 timerTolerance = (client->measPeriodUs <= 1000 /* for faster clients, update quickly */ ?
                          100  : CLM_TIMER_TOLERANCE);

  /* for async clients always update client's stats as it is an immediate calculation request */
  if (updateReason & CLM_UPDATE_REASON_ASYNC_UPDATE)
    timerTolerance = 0x7FFFFFFF; // MAX_INT32 to force pending time to always be less than this

  return timerTolerance;
} 
/** 
 * <!-- CLM_UpdatePeriodicClient -->
 *  
 * @brief Update periodic clients whose measurement windows have expired
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 */
static void CLM_UpdatePeriodicClient( CLM_Client *client,
                                      CLM_EngineCalculation *engineCalc,
                                      CLM_UpdateReasonEnumType updateReason)
{
  int32 tmrtolerance = getTimeTolerance(client, updateReason);

  CLM_LOG(6, "CLM_UpdatePeriodicClient: client: \"0x%x%x%x%x\", timerTolerance %d, doCB? %u",
             client->name[0], client->name[1], client->name[2], client->name[3],
             tmrtolerance, (client->pendingTimeUs <= tmrtolerance));

  /* do NOT use timeElapsedUs as that only accounts for time CLM accumulated for PMU stats measurements without the
   * CB callbacks and other CLM overheads - the pending time is correctly adjusted for actual total time expired since
   * last time this client callback was called */
  if (client->pendingTimeUs <= tmrtolerance)
  {
    CLM_ComputeUtil( client, engineCalc, DEFAULT_CALC );

    /* We apply a correction factor to Util if the following are TRUE:
     * 1) The PMU was the source of measurement data
     * 2) The 1HWT Busy/Util percentage is <= SINGLE_THREAD_MAXED_OUT_PCT
     *
     * x2) The overall Busy/Util percentage is <= CPU_MAXED_OUT_PCT
     *    Update 10/8: This condition will be removed to keep all ranges of data uniform - instead clients can look at
     *                 nonAdjUtilPctAtCurrClk and nonAdjBusyCycles data provided by CLM

     *
     * Update_8_28_2018: CLM clients expect the util to be in uniform units - whether pCPP or iCPP -
     * so permanently removing iCPP conversion. The iCPP conversion can be helpful when some clock
     * cycles on a stalled processor are used by another HW thread but there is no HW mechanism to
     * measure this, so while we previously used to guesstimate the CPP by downconverting to IMT CPP,
     * with increased usage of DCVS to provide more accurate perf level response for stability purpose,
     * all util calculations will be restricted to pCPP which helps stability better as in different crash logs.
     */
    if (/* optimize async updates */
        (!(updateReason & CLM_UPDATE_REASON_ASYNC_UPDATE)) &&
        (/*client->busyPctCurClk < CPU_MAXED_OUT_PCT && */
         client->busyPctOneThread < ONE_THREAD_MAXED_OUT_PCT)
       )
    {
      CLM_AdjustForParallelism( client );
      CLM_ComputeUtil( client, engineCalc, PARALLELISM_ADJ );
    }
    else
    {
      CLM_LOG(2,  "\tUtilization NOT adjusted "
                                "(busyPctCurClk: %d) "
                                "(busyPctOneThread: %d)",
                                client->busyPctCurClk,
                                client->busyPctOneThread );
    }

    CLM_UpdateClient( client, engineCalc, CLM_UPDATE_REASON_PERIODIC | updateReason );

    /* if this is an unscheduled update, don't reset data till scheduled CB occurs */
    if (!(updateReason & CLM_UPDATE_REASON_ASYNC_UPDATE))
    {
      CLM_ResetClientData( client );
    }
  }
}

/** 
 * <!-- CLM_UpdateThresholdClient -->
 *  
 * @brief Updates threshold clients if utilization exceeds or falls below
 * given thresholds.
 *
 * @param client : The client
 * @param engineCalc : Load data from Engine
 */
static void CLM_UpdateThresholdClient( CLM_Client *client,
                                       CLM_EngineCalculation *engineCalc )
{

  if (client->pendingTimeUs <= getTimeTolerance(client, CLM_UPDATE_REASON_NONE))
  {
    if ( CLM_CheckBusyHighThreshold( client, engineCalc ) )
    {
      CLM_ComputeUtil( client, engineCalc, DEFAULT_CALC );

      /* Inform client only when state transitions from low or middle to high */
      if ( client->lastKnownThreshold == CLM_LOW_THRESHOLD_LEVEL ||
           client->lastKnownThreshold == CLM_UNKNOWN_THRESHOLD_LEVEL )
      {
        client->lastKnownThreshold = CLM_HIGH_THRESHOLD_LEVEL;
        CLM_UpdateClient( client, engineCalc, 
                          CLM_UPDATE_REASON_HIGH_THRESHOLD );
      }
    }
    else if ( CLM_CheckBusyLowThreshold( client, engineCalc ) )
    {
      CLM_ComputeUtil( client, engineCalc, DEFAULT_CALC );

      /* Inform client only when state transitions from high or middle to low */
      if ( client->lastKnownThreshold == CLM_HIGH_THRESHOLD_LEVEL ||
           client->lastKnownThreshold == CLM_UNKNOWN_THRESHOLD_LEVEL )
      {
        client->lastKnownThreshold = CLM_LOW_THRESHOLD_LEVEL;
        CLM_UpdateClient( client, engineCalc, 
                          CLM_UPDATE_REASON_LOW_THRESHOLD );
      }
    }

    CLM_ResetClientData( client );
  }
}


/** 
 * <!--  CLM_EngineUpdate --> 
 *  
 * @brief Callback function invoked by the CLM Engine whenever the timer
 * expires. This routine updates load data for all clients and then
 * updates specific clients with this information, if their notification
 * conditions are true.
 *
 * @param engineCalc : Load data from Engine
 *        inClient   : Specific client that requires the data update. If inClient is non-NULL
 *                     it is considered to be an async update (i.e. not from CLM timer CB context)
 */
void CLM_EngineUpdate( CLM_EngineCalculation *engineCalc, CLM_Client * inClient)
{
  CLM_Client *client = (inClient == NULL ? CLM_EngineGetClients(): inClient);
  CORE_VERIFY_PTR( client );
  CLM_UpdateReasonEnumType updateReason = (inClient == NULL ? CLM_UPDATE_REASON_NONE :
                                                              CLM_UPDATE_REASON_ASYNC_UPDATE);

  uint64 currTime = 0;
  int32 origpendtm = 0;
  uint64 startSysTick = CLM_EngineGetStartTick();
  uint32 usecsSinceStart = 0;
  
  do
  {
        currTime = qurt_sysclock_get_hw_ticks();
    usecsSinceStart = (TICK_DIFF_TO_USECS(currTime, startSysTick));
    origpendtm = client->pendingTimeUs;
    /* account for time already elapsed since timer was last set */
    client->pendingTimeUs = origpendtm - usecsSinceStart;
    client->updateTime = currTime;

    //ULOG_RT_PRINTF_5(clmLog, "Engine Update (client: \"%s\"), clientPendTm %d->%d, usecsSinceTmrStart %u, clientUpdateTm 0x%x",
    //                 client->name, origpendtm, client->pendingTimeUs,
    //                 usecsSinceStart, (uint32)client->updateTime);
    CLM_LOG(8, "Engine Update (client: \"0x %x %x %x %x\"), clientPendTm %d->%d, usecsSinceTmrStart %u, clientUpdateTm 0x%x",
               client->name[0], client->name[1], client->name[2], client->name[3],
               origpendtm, client->pendingTimeUs,
               usecsSinceStart, (uint32)client->updateTime);
    CLM_UpdateClientData( client, engineCalc );

    if ( !client->lowThresholdPct && !client->highThresholdPct )
    {
      CLM_UpdatePeriodicClient( client, engineCalc, updateReason);
    }
    else
    {
      CLM_UpdateThresholdClient( client, engineCalc );
    }
    client = (inClient == NULL ? client->next : NULL);
  } while ( client );
}

/**
 * <!-- CLM_Initialize -->
 * 
 * @brief Initialize CLM; this function is invoked by RC_INIT
 */
void CLM_Initialize( void )
{
  CORE_DAL_VERIFY( 
    ULogFront_RealTimeInit( &clmLog, "CLMLog", CLM_LOG_BUFSIZE,
                            ULOG_MEMORY_LOCAL, ULOG_LOCK_OS ) );

  /* Create an instance of CLM Engine, passing in the function
     that will be the interface between the 2 layers */
  CLM_EngineCreate( CLM_EngineUpdate );
}


/*----------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------*/

/**
 * <!-- CLM_RegisterClient -->
 * 
 * @brief Code common to registering either periodic or threshold clients.
 */
static CLM_HandleType 
CLM_RegisterClient( const char            *clientName,
                    CLM_ClientOutputType   clientType, 
                    uint32                 measPeriodUs,
                    uint32                 attributes,
                    CLM_CallbackFuncPtr		cbFunc,
                    void                  *cbData )
{
  CLM_Client *newClient = NULL;
  uint32 size = 0;
  void *p;
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  /* Verify parameters */
  CORE_VERIFY( clientName );
  CORE_VERIFY( strlen( clientName ) <= CLM_MAX_NAME_LENGTH && strlen( clientName ) >= 4);
  CORE_VERIFY( measPeriodUs != 0 );
  CORE_VERIFY_PTR( cbFunc );

  newClient = (CLM_Client *)Core_Malloc( sizeof(CLM_Client) );
  CORE_VERIFY_PTR( newClient );
  memset( newClient, 0, sizeof(CLM_Client));

  /* Determine the right loadInfo struct to allocate */
  if ( clientType == CLM_CLIENT_BASIC_CPUUTIL )
  {
    size = sizeof(CLM_LoadInfoBasicStructType);
  }
  else if (clientType == CLM_CLIENT_EXTENDED_CPUUTIL)
  {
    size = sizeof(CLM_LoadInfoExtendedStructType);
  }
  else if (clientType == CLM_CLIENT_LOAD_INFO)
  {
    size = sizeof(CLM_LoadInfoStructType);
  }
  else
  {
    /* err_fatal */
    CORE_VERIFY( 0 );
  }

  newClient->loadInfo = Core_Malloc( size );
  CORE_VERIFY_PTR( newClient->loadInfo );
  memset( newClient->loadInfo, 0, size );

  size = nHWThreads * ( sizeof(uint64) + sizeof(uint32) );
  p = Core_Malloc( size );
  CORE_VERIFY_PTR( p );
  memset( p, 0, size );

  newClient->idleCycles = p;
  newClient->cyclesNThreadsBusy = (uint32 *)(newClient->idleCycles + nHWThreads );

  newClient->name = clientName;
  newClient->type = clientType;
  newClient->measPeriodUs = measPeriodUs;
  newClient->attributes = attributes;
  newClient->cbFunc = cbFunc;
  newClient->cbData = cbData;

  /* Initialize threshold to UNKNOWN, irrespective of type of client */
  newClient->lastKnownThreshold = CLM_UNKNOWN_THRESHOLD_LEVEL;

  return newClient;
}


/** 
 * <!-- CLM_RegisterPeriodicClient -->
 *  
 * Clients that need CPU load information periodically must register with
 * this API. 
 *  
 * Periodic clients will only receive callbacks with reason code 
 * CLM_UPDATE_REASON_PERIODIC. 
 *  
 * @param clientName : Name of client; should be limited to CLM_MAX_NAME_LENGTH
 * characters (including the terminating NULL)
 * 
 * @param clientType : Type of client; one of CLM_ClientOutputType.
 *
 * @param measPeriodUs : Measurement period of client 
 *
 * @param cbFunc : Client callback function of type CLM_CallbackFuncPtr 
 * 
 * @param cbData : Data (opaque to CLM) that was passed in by the client
 * during registration.
 * 
 * @return A non-NULL client handle is returned on successful registration.
 */
CLM_HandleType
CLM_RegisterPeriodicClient( const char            *clientName,
                            CLM_ClientOutputType   clientType, 
                            uint32                 measPeriodUs,
                            CLM_CallbackFuncPtr		 cbFunc,
                            void                  *cbData )
{
  CLM_Client *newClient = CLM_RegisterClient( clientName, clientType,
                                              measPeriodUs, 
                                              CLM_ATTRIBUTE_DEFAULT,
                                              cbFunc, cbData );
  CLM_EngineRegisterClient( newClient );
  return newClient;
}

/** 
 * <!-- CLM_RegisterPeriodicClient_Ex -->
 *  
 * A version of the RegisterPeriodicClient API that takes an additional
 * attributes parameter.
 *  
 * Periodic clients will only receive callbacks with reason code 
 * CLM_UPDATE_REASON_PERIODIC. 
 *  
 * @param clientName : Name of client; should be limited to CLM_MAX_NAME_LENGTH
 * characters (including the terminating NULL)
 * 
 * @param clientType : Type of client; one of CLM_ClientOutputType.
 *
 * @param measPeriodUs : Measurement period of client 
 *
 * @param attributes : Attributes to modify CLM behavior for this client
 *
 * @param cbFunc : Client callback function of type CLM_CallbackFuncPtr 
 * 
 * @param cbData : Data (opaque to CLM) that was passed in by the client
 * during registration.
 * 
 * @return A non-NULL client handle is returned on successful registration.
 */
CLM_HandleType 
CLM_RegisterPeriodicClient_Ex( const char            *clientName,
                               CLM_ClientOutputType   clientType, 
                               uint32                 measPeriodUs,
                               uint32                 attributes,
                               CLM_CallbackFuncPtr		cbFunc,
                               void                  *cbData )
{
  CLM_Client *newClient = CLM_RegisterClient( clientName, clientType,
                                              measPeriodUs, attributes,
                                              cbFunc, cbData );
  CLM_EngineRegisterClient( newClient );
  return newClient;
}


/**
 * <!-- CLM_RegisterThresholdClient -->
 * 
 * Clients that need CPU Load information when a specific threshold is crossed
 * must register using this API.
 *
 * Note that all threshold values will be compared against CPU utilization
 * scaled to MAX clock for the given target.
 *  
 * Clients that have specified a low/high threshold will only receive callbacks 
 * with reason code CLM_UPDATE_REASON_BELOW_LOW_THRESHOLD or 
 * CLM_UPDATE_REASON_BELOW_HIGH_THRESHOLD. 
 *  
 * It is legal to set no thresholds, only a low or a high threshold or to set 
 * both thresholds. If no thresholds are specified, the client is treated as 
 * PERIODIC.
 *
 * @param clientName : Name of client; should be limited to CLM_MAX_NAME_LENGTH
 * characters (including the terminating NULL)
 * 
 * @param clientType : Type of client; one of CLM_ClientOutputType.
 *
 * @param measPeriodUs : Measurement period of client 
 *
 * @param params : Pointer to a CLM_ThresholdClientRegStruct.
 * Will be copied over by CLM during registration.
 *
 * @param attributes : Attributes to modify CLM behavior for this client
 *   
 * @param cbFunc : Client callback function of type CLM_CallbackFuncPtr 
 * 
 * @param cbData : Data (opaque to CLM) that was passed in by the client
 * during registration.
 * 
 * @return A non-NULL client handle is returned on successful registration.
 */
CLM_HandleType 
CLM_RegisterThresholdClient( const char            *clientName,
                             CLM_ClientOutputType   clientType,
                             uint32                 measPeriodUs,
                             CLM_ThresholdClientRegStruct *params,
                             uint32                 attributes,
                             CLM_CallbackFuncPtr    cbFunc,
                             void                  *cbData )
{
  uint32 nHWThreads = CLM_EngineGetNumHWThreads();

  CORE_VERIFY_PTR( params );
  CORE_VERIFY( params->nThreadsForHigh <= nHWThreads );
  CORE_VERIFY( params->nThreadsForLow <= nHWThreads );

  CLM_Client *newClient = CLM_RegisterClient( clientName, clientType,
                                              measPeriodUs, attributes,
                                              cbFunc, cbData );

  newClient->nThreadsForHigh = params->nThreadsForHigh;
  newClient->nThreadsForLow = params->nThreadsForLow;
  newClient->lowThresholdPct = params->lowThresholdPct;
  newClient->highThresholdPct = params->highThresholdPct;

  /* lastKnownThreshold is already initialized to UNKNOWN in RegisterClient */

  CLM_EngineRegisterClient( newClient );
  return newClient;
}

/**
 * <!-- CLM_UnregisterClient -->
 * 
 * @brief Unregister a client from CLM
 * 
 * @param handle : Handle to client
 */
void CLM_UnregisterClient( CLM_HandleType handle )
{
  CLM_EngineUnregisterClient( handle );
}


/**
 * <!-- CLM_GetLastLPRExitTime -->
 * 
 * @brief Returns the timetick recorded at the last CLM LPR exit
 * @return Timetick of last CLM LPR exit
 */
uint64 CLM_GetLastLPRExitTime( void )
{
  return CLM_EngineGetLastLPRExitTime();
}

/**
 * <!-- CLM_SetClientMeasurementPeriod -->
 * 
 * @brief Modify the measurement period of the client. Must be invoked
 * from within the client' callback, not outside.
 *  
 * @param client : Client whose measPeriod is to be modified
 * 
 * @param measPeriodUs : New measurement period in usec
 */
void CLM_SetClientMeasurementPeriod( CLM_HandleType client, uint32 measPeriodUs )
{
  CORE_VERIFY_PTR( client );
  CORE_VERIFY( measPeriodUs != 0 );

  client->measPeriodUs = measPeriodUs;
}
