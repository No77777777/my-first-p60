/*============================================================================
@file CLM_Engine.h

Internal data structures and type declarations for CLM

Copyright (c) 2014-2015 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/power/clm/inc/CLM_Engine.h#2 $
============================================================================*/
#ifndef _CLM_ENGINE_H_
#define _CLM_ENGINE_H_

#include "CLM.h"
#include "CLM_Internal.h"
#include CLM_TARGET_H

#if CLM_DCVS_DEBUG == 0
  #define CLM_LOG(NUM_ARGS, ...)
#else
  #define MSG_0 MSG
  #define CLM_LOG(NUM_ARGS, ...) \
  do { \
       if (gClmDoDebugPrints & 0x1) \
       { \
         MSG_##NUM_ARGS(MSG_SSID_DFLT, MSG_LEGACY_HIGH, __VA_ARGS__);\
       } \
     } while(0)
#endif

typedef struct CLM_Client
{
  /* client input */
  uint32 highThresholdPct;
  uint32 lowThresholdPct;
  uint32 nThreadsForHigh;
  uint32 nThreadsForLow;
  uint32 measPeriodUs;
  CLM_ClientOutputType type;
  uint32 attributes;
  const char *name;
  CLM_CallbackFuncPtr cbFunc;
  void *cbData;

  /* basic */
  int32 timeElapsedUs;
  uint32 busyPctCurClk;
  uint32 nonAdjBusyPctCurClk;
  uint32 busyPctMaxClk;
  /* extended */
  uint64 budgetCycles;
  uint64 busyCycles;
  uint64 nonAdjBusyCycles; /* busy cycles without adjustment for parallelism */
  uint64 totalPackets;
  uint64 pCPPx1000;
  uint64 *idleCycles;
  /* pmu */
  uint32 busyPctOneThread;
  uint32 *cyclesNThreadsBusy;
  
  uint64 updateTime; //Time when client callback was triggered
  int32 pendingTimeUs;
  char lastKnownThreshold; /* 1 == High; 0 == Low */
  
  /* Points to client_type-specific data structure passed into client cb */
  void *loadInfo;
  
  struct CLM_Client *next;
} CLM_Client;


typedef struct
{
  uint32 curClkKhz;
  uint32 maxClkKhz;

  uint64 sumBudgetCycles;

  uint64 busyCycles;
  uint64 nonAdjBusyCycles; /* busy cycles without adjustment for parallelism */
  uint64 totalPackets;
  uint64 pCPPx1000;
  
  uint64 prevBusyCyclesQurt;
  uint64 *idleCyclesQurt;
  uint64 *prevIdleCyclesQurt;

  uint32 *cyclesNThreadsBusy;
  uint32 *prevCyclesNThreadsBusy;
  uint32 *packetCount;
  uint32 *prevPacketCount;

  /* Time elapsed in usec */
  uint32 timeElapsedUs;

  /* period in usec that the timer is set for */
  uint32 curTimerPeriod;


  /* TRUE, if Clock gating is disabled */
  uint32 clkGatingDisabled; 

  /* Timetick of last PC exit */
  uint64 lastLPRexitTS;
} CLM_EngineCalculation;

typedef void (*CLM_ClientCB)( CLM_EngineCalculation *, CLM_Client *client );

/* CLM ULog */
extern ULogHandle	clmLog;

extern uint8 gClmDoDebugPrints;

/**
 * <!-- CLM_EngineRegisterClient -->
 *  
 * @brief Register a CLM client with the CLM engine
 *
 * @param client: The client
 */
void CLM_EngineRegisterClient( CLM_Client *client );

/**
 * <!-- CLM_EngineUnregisterClient -->
 *  
 * Unregister a client from CLM
 *
 * @param client : The client
 */
void CLM_EngineUnregisterClient( CLM_Client *client );

/**
 * <!-- CLM_EngineCreate -->
 *  
 * @brief Creates an instance of the CLM Engine
 * @param cb: Callback function used to communicate between Engine and Client
 * layers
 */
void CLM_EngineCreate( CLM_ClientCB cb );

/**
 * <!-- CLM_EngineGetClients -->
 * @return Pointer to the list of clients
 */
CLM_Client* CLM_EngineGetClients( void );

/**
 * <!-- CLM_EngineGetNumHWThreads -->
 * @return The number of HW threads on the target
 */
uint32 CLM_EngineGetNumHWThreads( void );

/**
 * <!-- CLM_EngineGetLastLPRExitTime -->
 * @brief Returns the time of the last LPR exit 
 *
 * @return Timetick of last PC exit
 */
uint64 CLM_EngineGetLastLPRExitTime( void ); 


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
void CLM_SetClientMeasurementPeriod( CLM_HandleType client, 
                                     uint32 measPeriodUs );

#endif /* _CLM_ENGINE_H_ */
