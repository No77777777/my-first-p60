#ifndef __DAL_TSENS_H__
#define __DAL_TSENS_H__
/**
  @file DALTsens.h

  Function and data structure declarations for TSENS DAL.

  This file contains data structures and functions used to configure, control,
  and query temperature sensors.
*/
/*============================================================================
               Copyright (c) 2010-2012, 2014-2015, 2017 Qualcomm Technologies, Inc.
               All Rights Reserved.
               Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/tsens/dal/v2/DALTsens.h#1 $ */

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER 16
#define TSENS_MAX_NUM_CLIENTS 3
#define TSENS_MAX_NUM_DEVICES 1

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DDITsens.h"
#include "TsensBsp.h"
#include "DALFramework.h"
#include "DDIHWIO.h"
#ifdef FEATURE_TSENS_THRESHOLDS
#include "DDIInterruptController.h"
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
/* Declaring a Tsens driver, device and client context */
typedef struct TsensDrvCtxt TsensDrvCtxt;
typedef struct TsensDevCtxt TsensDevCtxt;
typedef struct TsensClientCtxt TsensClientCtxt;

typedef enum
{
   TSENS_DEVICE_STATE_INIT = 0,
   TSENS_DEVICE_STATE_ERROR,
   TSENS_DEVICE_STATE_READY
} TsensDeviceStateType;

#ifdef FEATURE_TSENS_THRESHOLDS
typedef enum
{
   TSENS_THRESHOLD_STATE_DISABLED = 0,
   TSENS_THRESHOLD_STATE_ENABLED,
   TSENS_THRESHOLD_STATE_TRIGGERED
} TsensThresholdStateType;

typedef enum
{
   TSENS_INTERRUPT_LOW_HIGH = 0,
   TSENS_INTERRUPT_CRITICAL,
   _TSENS_NUM_INTERRUPTS
} TsensInterruptType;

typedef struct
{
   TsensDevCtxt *pDevCtxt;
   uint32 uController;
   TsensInterruptType eInterrupt;
} TsensInterruptContext;

typedef struct
{
   TsensDevCtxt *pDevCtxt;
   DALSYSEventHandle hWorkLoopEvent;
   uint32 uController;
   TsensThresholdType eThreshold;
} TsensWorkLoopContext;

typedef struct
{
   DALSYSEventHandle hEvent;
   int32 nThresholdDeciDegC;
   TsensThresholdStateType eThresholdState;
} TsensThresholdConfigType;

typedef struct
{
   TsensThresholdConfigType aThresholds[TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER][_TSENS_NUM_THRESHOLDS];
} TsensClientControllerType;

typedef struct
{
   TsensClientControllerType *paControllers;
   boolean bRegistered;
   boolean bEnabled;
} TsensClientInfoType;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

typedef struct
{
#ifdef FEATURE_TSENS_THRESHOLDS
   int32 anAggrThresholds[TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER][_TSENS_NUM_THRESHOLDS];
   uint32 auSensorIdx[TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER];
   TsensWorkLoopContext aWorkLoopCtxt[_TSENS_NUM_THRESHOLDS];
   TsensInterruptContext aIntCtxt[_TSENS_NUM_INTERRUPTS];
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
   uint8 *pucTsensSROTAddr;
   uint8 *pucTsensTMAddr;
   uint32 uChannelEnableMask;
#ifdef FEATURE_TSENS_THRESHOLDS
   uint32 uThreshCrossedMask;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
} TsensControllerType;

/* Declaring a private Tsens Vtable */
typedef struct TsensDALVtbl TsensDALVtbl;
struct TsensDALVtbl
{
   int (*Tsens_DriverInit)(TsensDrvCtxt *);
   int (*Tsens_DriverDeInit)(TsensDrvCtxt *);
};

/* Tsens device context */
struct TsensDevCtxt
{
   // Base Members
   uint32 dwRefs;
   DALDEVICEID DevId;
   uint32 dwDevCtxtRefIdx;
   TsensDrvCtxt *pTsensDrvCtxt;
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   uint32 Reserved[16];
   // TSENS dev state can be added by developers here
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensClientInfoType aClients[TSENS_MAX_NUM_CLIENTS];
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
   const TsensBspType *pBsp;
   TsensControllerType *paControllers;
   DalDeviceHandle *phHWIO;
#ifdef FEATURE_TSENS_THRESHOLDS
   DALSYSSyncHandle hSync;
   DALSYS_SYNC_OBJECT(syncObject);
   DALSYSSyncHandle hIsrSync;
   DALSYS_SYNC_OBJECT(isrSyncObject);
   DALSYSWorkLoopHandle hTsensWorkLoop;
   int32 nMinTempDeciDegC;
   int32 nMaxTempDeciDegC;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
   TsensDeviceStateType eDeviceState;
};

/* Tsens driver context */
struct TsensDrvCtxt
{
   // Base Members
   TsensDALVtbl TsensDALVtbl;
   uint32 dwNumDev;
   uint32 dwSizeDevCtxt;
   uint32 bInit;
   uint32 dwRefs;
   TsensDevCtxt TsensDevCtxt[TSENS_MAX_NUM_DEVICES];
   // TSENS drv state can be added by developers here
};

/* Tsens client context */
struct TsensClientCtxt
{
   // Base Members
   uint32 dwRefs;
   uint32 dwAccessMode;
   void *pPortCtxt;
   TsensDevCtxt *pTsensDevCtxt;
   DalTsensHandle DalTsensHandle;
   // TSENS client state can be added by developers here
   uint32 uClientId;
};

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
/* Functions specific to the DAL interface */
DALResult Tsens_DalTsens_Attach(const char *, DALDEVICEID, DalDeviceHandle **);
DALResult Tsens_DriverInit(TsensDrvCtxt *);
DALResult Tsens_DriverDeInit(TsensDrvCtxt *);
DALResult Tsens_DeviceInit(TsensClientCtxt *);
DALResult Tsens_DeviceDeInit(TsensClientCtxt *);
DALResult Tsens_RegisterClient(TsensClientCtxt *);
DALResult Tsens_DeregisterClient(TsensClientCtxt *);

/* Functions specific to the DalTsens interface */
DALResult Tsens_GetTemp(TsensClientCtxt *, uint32, TsensTempType *);
DALResult Tsens_GetTempRange(TsensClientCtxt *, uint32, TsensTempRangeType *);
DALResult Tsens_SetThreshold(TsensClientCtxt *, uint32, TsensThresholdType, int32, DALSYSEventHandle);
DALResult Tsens_SetEnableThresholds(TsensClientCtxt *, DALBOOL bThresholdsEnable);
DALResult Tsens_GetNumSensors(TsensClientCtxt *, uint32 *);
DALResult Tsens_CheckCalibration(TsensClientCtxt *, uint32);

#endif

