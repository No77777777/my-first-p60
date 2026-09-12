#ifndef __VADCTMLOG_H__
#define __VADCTMLOG_H__
/*============================================================================
  @file VAdcTMLog.h

  Logging for the VAdcTM


                Copyright (c) 2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/VAdcTMLog.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DDIAdcDevice.h"
#include "DALFramework.h"
#include "VAdcTMHal.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#ifdef ADC_LOG_DISABLED

#define VAdcTM_LogThresholdSet(v,w,x,y,z)
#define VAdcTM_LogThresholdCleared(x,y,z)
#define VAdcTM_LogThresholdTriggered(w,x,y,z)
#define VAdcTM_LogInterrupt(x)
#define VAdcTM_LogPullUpSwitched(x,y,z)
#define VAdcTM_LogError(x,y,z)
#define VAdcTM_LogDumpRegisters(x,y)

#else

#define VADCTM_LOG_MAX 30
#define VADCTM_REG_DUMP_MAX 1

#endif

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
#ifndef ADC_LOG_DISABLED

typedef struct
{
   enum
   {
      VADCTM_LOG_EVENT_THRESHOLD_SET = 0,
      VADCTM_LOG_EVENT_THRESHOLD_CLEARED,
      VADCTM_LOG_EVENT_INTERRUPT,
      VADCTM_LOG_EVENT_THRESHOLD_TRIGGERED,
      VADCTM_LOG_EVENT_PULLUP_SWITCHED,
      VADCTM_LOG_EVENT_ERROR
   } eEvent;
   uint64 uTimestamp;
   uint32 uMeasIdx;
   AdcDeviceTMThresholdType eThreshold;
   int32 nThresholdDesired;
   int32 nThresholdSet;
   int32 nTriggered;
   const char *pszErrorMsg;
} VAdcTMRegDataLogEntryType;

typedef struct
{
   VAdcTMRegDataLogEntryType aLog[VADCTM_LOG_MAX];
   VAdcTMHalRegDumpType aRegDump[VADCTM_REG_DUMP_MAX];
   DalDeviceHandle *phTimetickDev;
   DALSYSSyncHandle hSync;
   DALSYS_SYNC_OBJECT(syncObject);
   uint32 uCurIdx;
   uint32 uRegDumpCurIdx;
   DALDEVICEID deviceId;
} VAdcTMDebugInfoType;

#else

typedef void *VAdcTMDebugInfoType;

static __inline DALResult
VAdcTM_LogInit(VAdcTMDebugInfoType *pVAdcTMDebugInfo, DALDEVICEID deviceId)
{
   return DAL_SUCCESS;
}

#endif

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
#ifndef ADC_LOG_DISABLED

DALResult VAdcTM_LogInit(VAdcTMDebugInfoType *pVAdcTMDebugInfo, DALDEVICEID deviceId);
void VAdcTM_LogThresholdSet(VAdcTMDebugInfoType *pVAdcTMDebugInfo, uint32 uMeasIdx, AdcDeviceTMThresholdType eThreshold, int32 nThresholdDesired, int32 nThresholdSet);
void VAdcTM_LogThresholdCleared(VAdcTMDebugInfoType *pVAdcTMDebugInfo, uint32 uMeasIdx, AdcDeviceTMThresholdType eThreshold);
void VAdcTM_LogThresholdTriggered(VAdcTMDebugInfoType *pVAdcTMDebugInfo, uint32 uMeasIdx, AdcDeviceTMThresholdType eThreshold, int32 nTriggered);
void VAdcTM_LogInterrupt(VAdcTMDebugInfoType *pVAdcTMDebugInfo);
void VAdcTM_LogPullUpSwitched(VAdcTMDebugInfoType *pVAdcTMDebugInfo, uint32 uMeasIdx, uint32 uPullUp);
void VAdcTM_LogError(VAdcTMDebugInfoType *pVAdcTMDebugInfo, const char *pszErrorMsg, DALBOOL bFatalError);
void VAdcTM_LogDumpRegisters(VAdcTMDebugInfoType *pVAdcTMDebugInfo, VAdcTMHalInterfaceType *piVAdcTMHal);

#endif

#endif /* #ifndef __VADCTMLOG_H__ */

