/*============================================================================
@file CLM.h

CLM API. CLM provides clients with CPU utilization information on a periodic or
threshold basis.

Copyright (c) 2013-2015 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR

$Header: //components/rel/core.mpss/10.0/api/power/CLM.h#2 $
============================================================================*/
#ifndef _CLM_H_
#define _CLM_H_

#include "DALStdDef.h"

/* CLM Client names are restricted to this length (including the '\0') */
#define CLM_MAX_NAME_LENGTH 16
#define NSEC_IN_TICK 52

/* The max possible number of HW threads on the system */
#define MAX_HW_THREADS 4

#define CLM_DCVS_DEBUG 0 /* set to 0 in production builds to save extra logging F3 memory */

/**
 * CLM Client Attributes
 */
#define CLM_ATTRIBUTE_DEFAULT 0x0

#define CORE_LIKELY(x)   __builtin_expect((x),1)
#define CORE_UNLIKELY(x) __builtin_expect((x),0)

#define TICK_DIFF_TO_NSECS(end, start) ((end) > (start) ? (uint32)((((end) - (start)) * NSEC_IN_TICK)) : 1)
//do not use NSEC macro for USEC conversion as it will overflow 32 bits in just over 4secs
#define TICK_DIFF_TO_USECS(end, start) ((end) > (start) ? (uint32)((((end) - (start)) / 192) * 10) : 1)

#define CORE_CALL_WEAK_FN(calledflag, fn, ...) \
  void (* volatile def_weak_fptr)(uint32 *ret, ...); \
  (calledflag) = 0; \
  def_weak_fptr = (fn); \
  \
  if (def_weak_fptr != NULL) \
  { \
    (calledflag) = 1; \
    (* def_weak_fptr)(__VA_ARGS__); \
  }

/**
 * CLM Client handle
 */
typedef struct CLM_Client* CLM_HandleType;

/**
 * List of reasons why the client is being provided a util update
 */
typedef enum
{
  CLM_UPDATE_REASON_NONE = 0,

  /* Periodic update at the end of the client specified measurement window */
  CLM_UPDATE_REASON_PERIODIC = 0x1,

  /* CPU utilization is below client specified threshold */
  CLM_UPDATE_REASON_LOW_THRESHOLD = 0x2,

  /* CPU utilization is at or above client specified threshold */
  CLM_UPDATE_REASON_HIGH_THRESHOLD = 0x4,

  /* Unscheduled load info update to a client */
  CLM_UPDATE_REASON_ASYNC_UPDATE = 0x8,

  /* INTERNAL: Forces enum to 32-bits */
  CLM_UPDATE_REASON_DUMMY_FORCE_INT32 = 0x7FFFffff

} CLM_UpdateReasonEnumType;

/**
 * List of client types supported by CLM
 */
typedef enum
{
  /* Use this client to access information of CPU/DDR util registers 
     There registers are GCYCLE/GCOMMIT on Hexagon V67 */  
  CLM_CLIENT_LOAD_INFO = 0,

  /* Use this client if basic CPU load information is desired.
     See CLM_LoadInfoBasicStruct for the list of fields returned */
  CLM_CLIENT_BASIC_CPUUTIL,

  /* Use this client if extended CPU utilization info is desired.
     See CLM_LoadInfoExtendedStruct for the list of fields returned */
  CLM_CLIENT_EXTENDED_CPUUTIL

} CLM_ClientOutputType;


/**
 * Container for Basic CPU load data
 */
typedef struct CLM_LoadInfoBasicStruct
{
  /* Reason why the update is being provided */
  CLM_UpdateReasonEnumType updateReason;

  /* Number of usecs over which this information was computed */
  uint32 timeElapsedUs;

  /* The current clock rate of the CPU in kHz */
  uint32 currentClkKhz;

  /* CPU utilization percentage at the clock rate when the window ended */
  uint32 utilPctAtCurrClk;
  uint32 nonAdjUtilPctAtCurrClk;

  /* CPU utilization percentage scaled to the maximum clock rate supported by 
     the CPU */
  uint32 utilPctAtMaxClk;

} CLM_LoadInfoBasicStructType;


/**
 * Container for PMU Data
 */
typedef struct CLM_LoadInfoStructType
{
  /* Reason why the update is being provided */
  CLM_UpdateReasonEnumType updateReason;

  /* Number of usecs over which this information was computed */
  uint32 timeElapsedUs;
  

  /* Array with the number of cycles during which 1-nHWThreads were busy.
   * [0] is the number of cycles where only 1 HWT was busy, [1] enumerates
   * cycles where 2 HWTs were busy etc.
   */
  uint32 cyclesNThreadsBusy[MAX_HW_THREADS];

} CLM_LoadInfoStructType;

/**
 * Container for extended CPU utilization data
 */
typedef struct CLM_LoadInfoExtendedStructType
{
  /* Reason why the update is being provided */
  CLM_UpdateReasonEnumType updateReason;

  /* Number of usecs over which this information was computed */
  uint32 timeElapsedUs;

  /* Time elapsed since client CB was last triggered (usecs) */
  uint32 timeElapsedSinceCB;

  /* Measurement period of the client in usecs */
  uint32 measPeriodUs;

  /* The current clock rate of the CPU in kHz */
  uint32 currentClkKhz;

  /* CPU utilization percentage at the clock rate when the window ended */
  uint32 utilPctAtCurrClk;
  uint32 nonAdjUtilPctAtCurrClk;

  uint64 totalPackets;

  /* CPU utilization percentage scaled to the maximum clock rate supported by 
     the CPU */
  uint32 utilPctAtMaxClk;

  /* Number of processor cycles that were available for use during the 
     measurement window; includes cycles we may've spent power-collapsed */
  uint64 budgetCycles;

  /* Number of 'adjusted' (IMT) processor cycles that were used to execute instructions 
     during the measurement window */
  uint64 busyCycles;
  
  /* Number of 'non-adjusted' (per-packet CPP) processor cycles that were used to execute instructions 
     during the measurement window */
  uint64 nonAdjBusyCycles;

  /* Average Cycles per packet for current measurement window */
  uint32 pCPPx1000;

  /* Array with the number of cycles during which 1-nHWThreads were busy.
   * [0] is the number of cycles where only 1 HWT was busy, [1] enumerates
   * cycles where 2 HWTs were busy etc.
   */
  uint32 cyclesNThreadsBusy[MAX_HW_THREADS];

} CLM_LoadInfoExtendedStructType;


/**
 * Template of callback function (provided by client) that is invoked when
 * the measurement window for the client elapses or when CPU Utilization at 
 * MAX Clock exceeds/drops below the specified high/low bus thresholds. 
 *
 * @param loadInfo : pointer to CPU Load info. This data structure is 
 * allocated and owned by CLM and is only valid in context of this callback.
 * Clients must make their own copy if data is needed outside or after this
 * context. loadInfo should be typecast to one of the CLM_LoadInfoXXX
 * types, based on the client type.
 *  
 * @param cbData : Data (opaque to CLM) that was passed in by the client
 * during registration.
 */
typedef void (*CLM_CallbackFuncPtr)( void *loadInfo, void *cbData );


/**
 * Configuration data provided by Threshold clients during registration
 */
typedef struct CLM_ThresholdClientRegStruct
{
  /* If non-zero, the client will be notified when _scaled_ CPU utilization at
     MAX clock is >= this value. Must be >= lowThresholdPct */
  uint32 highThresholdPct;

  /* If non-zero, the client will be notified when _scaled_ CPU utilization at
     MAX clock has fallen below this value. Must be <= highThresholdPct */
  uint32 lowThresholdPct;

  /* Num of threads used in computing load to compare against highThreshold */
  uint32 nThreadsForHigh;

  /* Num of threads used in computing load to compare against lowThreshold */  
  uint32 nThreadsForLow;

} CLM_ThresholdClientRegStruct;

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
                            void                  *cbData );

/**
 * <!-- CLM_UnregisterClient -->
 * 
 * @brief Unregister a client from CLM
 * 
 * @param handle : Handle to client
 */
void CLM_UnregisterClient( CLM_HandleType handle );


/** 
 * <!-- CLM_adjustTimer --> 
 * 
 * @brief Registers client with new timer period 
 *  
 * @param newPeriod: Timer expiry in ms 
 *  
 * @param clientName: Name of client 
 *  
 * @param pClmClient: Pointer to existing client handle 
 *  
 * @param cb: Callback function to communicate with client and 
 *          provide utilization stats
 *  
 * @param context: client context 
 *  
 * @return Client handle registered with new period
 */
CLM_HandleType CLM_adjustTimer (uint32 newPeriod, const char * clientName, CLM_HandleType pClmClient,
                                CLM_CallbackFuncPtr cb, void * context);

/** 
 * <!-- CLM_computeLoadSinceLastMsmt --> 
 *  
 * @brief Calculates utilization based on PMU stats
 *  
 * @param updateClient: pointer to client to send unscheduled update to
 *
 * @return 0 on success, 1 on error
 */
uint8 CLM_computeLoadSinceLastMsmt(CLM_HandleType updateClient);

/** 
 * <!-- CLM_GetLoadInfoPtr --> 
 *  
 * @brief Returns the loadInfo pointer for input client
 *  
 * @param client: pointer to client
 *
 * @return loadInfo
 */
void * CLM_GetLoadInfoPtr(CLM_HandleType client);

/** 
 * <!-- CLM_GetLoadInfoPtr --> 
 *  
 * @brief Returns the time when client was updated by CLM
 *  
 * @return update time
 */
uint64 CLM_GetClientUpdateTime(CLM_HandleType client);

/** 
 * <!-- CLM_EngineGetStartTick --> 
 *  
 * @brief Returns the time when CLM load measurement time period started
 *  
 * @return start time
 */
uint64 CLM_EngineGetStartTick( void );

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
                               CLM_CallbackFuncPtr    cbFunc,
                               void                  *cbData );


#endif /* _CLM_H_ */
