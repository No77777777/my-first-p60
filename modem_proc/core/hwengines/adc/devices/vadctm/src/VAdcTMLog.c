#ifndef ADC_LOG_DISABLED
/*============================================================================
  FILE:         VAdcTMLog.c

  OVERVIEW:     Logging for the VAdcTM

  DEPENDENCIES: None

                Copyright (c) 2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/VAdcTMLog.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2015-09-28  jjo  Initial version.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcTMLog.h"
#include "DDITimetick.h"
#include "msg.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint64
VAdcTM_GetTimetick(const VAdcTMDebugInfoType *pVAdcTMDebugInfo)
{
   DALResult result;
   DalTimetickTime64Type u64Tick;

   result = DalTimetick_GetTimetick64(pVAdcTMDebugInfo->phTimetickDev, &u64Tick);
   if (result != DAL_SUCCESS)
   {
      return 0;
   }

   return (uint64)u64Tick;
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
void
VAdcTM_LogThresholdSet(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   uint32 uMeasIdx,
   AdcDeviceTMThresholdType eThreshold,
   int32 nThresholdDesired,
   int32 nThresholdSet
   )
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_THRESHOLD_SET;
   pVAdcTMDebugInfo->aLog[uCurIdx].uMeasIdx = uMeasIdx;
   pVAdcTMDebugInfo->aLog[uCurIdx].eThreshold = eThreshold;
   pVAdcTMDebugInfo->aLog[uCurIdx].nThresholdDesired = nThresholdDesired;
   pVAdcTMDebugInfo->aLog[uCurIdx].nThresholdSet = nThresholdSet;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = NULL;

   MSG_4(MSG_SSID_ADC,
         MSG_LEGACY_HIGH,
         "VADCTM thresh set: meas %u, thresh %u, desired %i, set %i",
         uMeasIdx,
         (uint32)eThreshold,
         nThresholdDesired,
         nThresholdSet);
}

void
VAdcTM_LogThresholdCleared(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   uint32 uMeasIdx,
   AdcDeviceTMThresholdType eThreshold
   )
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_THRESHOLD_CLEARED;
   pVAdcTMDebugInfo->aLog[uCurIdx].uMeasIdx = uMeasIdx;
   pVAdcTMDebugInfo->aLog[uCurIdx].eThreshold = eThreshold;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = NULL;

   MSG_1(MSG_SSID_ADC,
         MSG_LEGACY_MED,
         "VADCTM thresh cleared: meas %u",
         uMeasIdx);
}

void
VAdcTM_LogThresholdTriggered(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   uint32 uMeasIdx,
   AdcDeviceTMThresholdType eThreshold,
   int32 nTriggered
   )
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_THRESHOLD_TRIGGERED;
   pVAdcTMDebugInfo->aLog[uCurIdx].uMeasIdx = uMeasIdx;
   pVAdcTMDebugInfo->aLog[uCurIdx].eThreshold = eThreshold;
   pVAdcTMDebugInfo->aLog[uCurIdx].nTriggered = nTriggered;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = NULL;

   MSG_3(MSG_SSID_ADC,
         MSG_LEGACY_HIGH,
         "VADCTM thresh set: meas %u, thresh %u, triggered %i",
         uMeasIdx,
         (uint32)eThreshold,
         nTriggered);
}

void
VAdcTM_LogInterrupt(VAdcTMDebugInfoType *pVAdcTMDebugInfo)
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_INTERRUPT;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = NULL;

   MSG(MSG_SSID_ADC, MSG_LEGACY_MED, "VADCTM Threshold Interrupt");
}

void
VAdcTM_LogPullUpSwitched(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   uint32 uMeasIdx,
   uint32 uPullUp)
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_PULLUP_SWITCHED;
   pVAdcTMDebugInfo->aLog[uCurIdx].uMeasIdx = uMeasIdx;
   pVAdcTMDebugInfo->aLog[uCurIdx].nTriggered = (int32)uPullUp;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = NULL;

   MSG_2(MSG_SSID_ADC,
         MSG_LEGACY_HIGH,
         "VADCTM pull up switched for meas %u to %u",
         uMeasIdx,
         uPullUp);
}

void
VAdcTM_LogError(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   const char *pszErrorMsg,
   DALBOOL bFatalError
   )
{
   uint32 uCurIdx;

   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uCurIdx;
   if (uCurIdx < VADCTM_LOG_MAX - 1)
   {
      pVAdcTMDebugInfo->uCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);

   pVAdcTMDebugInfo->aLog[uCurIdx].uTimestamp = VAdcTM_GetTimetick(pVAdcTMDebugInfo);
   pVAdcTMDebugInfo->aLog[uCurIdx].eEvent = VADCTM_LOG_EVENT_ERROR;
   pVAdcTMDebugInfo->aLog[uCurIdx].pszErrorMsg = pszErrorMsg;

   if (bFatalError == TRUE)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC, MSG_LEGACY_FATAL, "%s", pszErrorMsg);

      DALSYS_LogEvent(pVAdcTMDebugInfo->deviceId,
                      DALSYS_LOGEVENT_FATAL_ERROR,
                      pszErrorMsg);
   }
   else
   {
      MSG_SPRINTF_1(MSG_SSID_ADC, MSG_LEGACY_ERROR, "%s", pszErrorMsg);

      DALSYS_LogEvent(pVAdcTMDebugInfo->deviceId,
                      DALSYS_LOGEVENT_ERROR,
                      pszErrorMsg);
   }
}

void
VAdcTM_LogDumpRegisters(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   VAdcTMHalInterfaceType *piVAdcTMHal
   )
{
   VAdcTMHalRegDumpType *pVAdcTMRegDump;
   uint32 uCurIdx;
   uint32 uReg;

#if 0
   DALSYS_SyncEnter(pVAdcTMDebugInfo->hSync);

   uCurIdx = pVAdcTMDebugInfo->uRegDumpCurIdx;
   if (uCurIdx < VADCTM_REG_DUMP_MAX - 1)
   {
      pVAdcTMDebugInfo->uRegDumpCurIdx++;
   }
   else
   {
      pVAdcTMDebugInfo->uRegDumpCurIdx = 0;
   }

   DALSYS_SyncLeave(pVAdcTMDebugInfo->hSync);
#else
   uCurIdx = 0;
#endif

   pVAdcTMRegDump = &pVAdcTMDebugInfo->aRegDump[uCurIdx];

   (void)VAdcTMHalDumpRegisters(piVAdcTMHal, pVAdcTMRegDump);

   MSG(MSG_SSID_ADC, MSG_LEGACY_ERROR, "VADCTM register dump");

   for (uReg = 0; uReg < VADCTM_HAL_NUM_REGISTERS_TO_DUMP; uReg++)
   {
      MSG_2(MSG_SSID_ADC,
            MSG_LEGACY_ERROR,
            "Offset = %u, Contents = %u",
            pVAdcTMRegDump->aVAdcTMReg[uReg].ucOffset,
            pVAdcTMRegDump->aVAdcTMReg[uReg].ucVal);
   }
}

DALResult
VAdcTM_LogInit(
   VAdcTMDebugInfoType *pVAdcTMDebugInfo,
   DALDEVICEID deviceId
   )
{
   DALResult result;
   pVAdcTMDebugInfo->uCurIdx = 0;
   pVAdcTMDebugInfo->uRegDumpCurIdx = 0;
   pVAdcTMDebugInfo->deviceId = deviceId;

   result = DalTimetick_Attach("SystemTimer", &pVAdcTMDebugInfo->phTimetickDev);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(deviceId,
                      DALSYS_LOGEVENT_FATAL_ERROR,
                      "VADCTM: Failed to attach to Timetick device");
      return result;
   }

   /* Interrupts can happen asynchronously so a lock is required */
   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE_INTERRUPT,
                              &pVAdcTMDebugInfo->hSync,
                              &pVAdcTMDebugInfo->syncObject);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(deviceId,
                      DALSYS_LOGEVENT_FATAL_ERROR,
                      "VADCTM: Failed to create log sync");
      return result;
   }

   return DAL_SUCCESS;
}

#endif /* #ifndef ADC_LOG_DISABLED */

