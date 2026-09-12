/*============================================================================
  FILE:         AdcTest.c

  OVERVIEW:     Exercises ADC.

  DEPENDENCIES: None

                Copyright (c) 2009, 2014-2016, 2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/test/src/AdcTest.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2016-04-27  jjo  Add wrapper API.
  2015-03-27  jjo  Added tolerance test.
  2015-01-12  jjo  Add TM tests; output to QXDM.
  2014-07-07  jjo  Add channel name support.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DDIAdc.h"
#include "adc.h"
#include "DDITimetick.h"
#include "msg.h"
#include "timer.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

#define ADC_TEST_DEFAULT_STACK_SIZE 0x2000
#define ADC_MAX_QUEUE_SIZE 8

#define LVL_INFO  (MSG_LEGACY_ERROR)
#define LVL_ERROR (MSG_LEGACY_FATAL)

enum
{
   ADC_TEST_EVENT_DEFAULT = 0,
   ADC_TEST_EVENT_TIMEOUT,
   _ADC_TEST_NUM_EVENTS
};

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
typedef enum
{
   ADC_TEST_TM_CALLBACK = 0,
   ADC_TEST_TM_SIGNAL
} AdcTestTMEventType;

typedef struct
{
   DalDeviceHandle *phAdcDev;
   AdcResultType *pAdcResult;
   DALSYSEventHandle hCallbackEvent;
   DALSYS_EVENT_OBJECT(callbackEventObject);
   DALSYSEventHandle hSignalEvent;
   DALSYS_EVENT_OBJECT(signalEventObject);
   DALSYSSyncHandle hSync;
   DALSYS_SYNC_OBJECT(syncObject);
} AdcBlockingReadType;

typedef struct
{
   DalDeviceHandle *phAdcDev;
   AdcRecalibrationResultType *pRecalResult;
   DALSYSEventHandle hCallbackEvent;
   DALSYS_EVENT_OBJECT(callbackEventObject);
   DALSYSEventHandle hSignalEvent;
   DALSYS_EVENT_OBJECT(signalEventObject);
   DALSYSSyncHandle hSync;
   DALSYS_SYNC_OBJECT(syncObject);
} AdcBlockingRecalType;

typedef struct
{
   DalDeviceHandle *phAdcDev;
   DALSYSEventHandle hCallbackEvent;
   DALSYS_EVENT_OBJECT(callbackEventObject);
   DALSYSEventHandle ahEvent[_ADC_TEST_NUM_EVENTS];
   DALSYSSyncHandle hSync;
   DALSYS_SYNC_OBJECT(syncObject);
   AdcInputPropertiesType adcInputProps;
   AdcRequestParametersType adcParams;
   uint32 uNumConvQueued;
   uint32 uNumConvCompleted;
} AdcQueueTestCtxtType;

typedef struct
{
   const char *pszChannelName;
   DALBOOL bPresent;
   AdcInputPropertiesType channel;
   AdcResultType readResult;
   AdcRecalibrationResultType recalResult;
   adc_result_type wrapperResult;
} AdcTestResultType;

typedef struct
{
   const char *pszTMChannel;
   int32 nPhysicalDelta;
   uint32 uWaitTime_us;
   int32 nMinVal;
   int32 nMaxVal;
   int32 nStep;
   int32 nTolerance;
} AdcTestTMChannelType;

typedef struct
{
   DALBOOL bPresent;
   AdcInputPropertiesType adcInputProps;
   AdcTMInputPropertiesType adcTMInputProps;
   AdcTMRangeType adcTMRange;
   int32 nCurrentPhysical;
} AdcTestTMMeasurementType;

typedef struct
{
   AdcTestTMMeasurementType *paMeasurements;
   DalDeviceHandle *phAdcDev;
   DALSYSEventHandle ahEvent[_ADC_TEST_NUM_EVENTS];
   DALSYSEventHandle hCallbackEvent;
   uint32 uNumTMChannels;
} AdcTMTestCtxtType;

typedef struct
{
   DalDeviceHandle *phAdcDev;
   DALSYSEventHandle hCallbackEvent;
} AdcToleranceTestCtxtType;

typedef struct
{
   AdcTestResultType *paResults;
   DalDeviceHandle *phAdcDev;
   AdcBlockingReadType *pAdcBlockingRead;
   AdcBlockingRecalType *pAdcBlockingRecal;
   AdcQueueTestCtxtType *pAdcQueueTestCtxt;
   AdcTMTestCtxtType *pTMTestCtxt;
} AdcTestCtxtType;

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/
static const char *aChannels[] =
{
   ADC_INPUT_VPH_PWR,
   ADC_INPUT_XO_THERM,
   ADC_INPUT_XO_THERM_GPS,
   ADC_INPUT_XO_THERM_GPS_LOW,
   ADC_INPUT_XO_THERM_GPS_MED,
   ADC_INPUT_XO_THERM_GPS_HIGH,
   ADC_INPUT_PMIC_TEMP1,
   ADC_INPUT_PA_THERM,
   ADC_INPUT_PA_THERM1,
   ADC_INPUT_SYS_THERM1,
   ADC_INPUT_SYS_THERM2,
   ADC_INPUT_SYS_THERM3,
   ADC_INPUT_VBATT,
   ADC_INPUT_VBATT_GSM,
   ADC_INPUT_PMIC_HARDWARE_ID,
   ADC_INPUT_PLATFORM_ID,
   ADC_INPUT_BATT_ID,
   ADC_INPUT_BATT_THERM,
   ADC_INPUT_CHG_TEMP,
   ADC_INPUT_VCHG,
   ADC_INPUT_VCOIN,
   ADC_INPUT_USB_IN,
   ADC_INPUT_DC_IN,
};

static const AdcTestTMChannelType aTMChannels[] =
{
   {
      ADC_INPUT_PMIC_TEMP1,
      5000,
      500000,
      -40000,
      125000,
      1,
      1000
   },

   {
      ADC_INPUT_XO_THERM,
      5000,
      500000,
      -40960,
      128000,
      1,
      1024
   },

   {
      ADC_INPUT_VPH_PWR,
      500,
      500000,
      0,
      5400,
      1,
      10
   },

   {
      ADC_INPUT_PA_THERM,
      5,
      500000,
      -40,
      125,
      1,
      1
   },
};

static AdcTestResultType gaResults[ARRAY_LENGTH(aChannels)];
static AdcBlockingReadType gAdcBlockingRead;
static AdcBlockingRecalType gAdcBlockingRecal;
static AdcQueueTestCtxtType gAdcQueueTestCtxt;
static AdcTestTMMeasurementType gaMeasurements[ARRAY_LENGTH(aTMChannels)];
static AdcTMTestCtxtType gAdcTMTestCtxt =
{
   gaMeasurements,
};
static AdcToleranceTestCtxtType gAdcToleranceTestCtxt;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
uint32 guAdcTestNumErrors = 0;

AdcTestCtxtType gAdcTestCtxt =
{
   gaResults,
};

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static int32
minval(
   int32 a,
   int32 b
   )
{
   return (a < b) ? a: b;
}

static int32
maxval(
   int32 a,
   int32 b
   )
{
   return (a > b) ? a: b;
}

static void
AdcTest_Sleep(uint32 uDelay_us)
{
   // This will be a deferrable sleep
   timer_sleep(uDelay_us, T_USEC, FALSE);
}

static uint64
AdcTest_GetTimestampUs(void)
{
   static DalDeviceHandle *phTimetickDev = NULL;
   DalTimetickTime64Type u64Tick;
   DalTimetickTime64Type u64MicroSeconds;
   DALResult result;

   if (phTimetickDev == NULL)
   {
      result = DalTimetick_Attach("SystemTimer", &phTimetickDev);
      if (result != DAL_SUCCESS)
      {
         return 0;
      }
   }

   result = DalTimetick_GetTimetick64(phTimetickDev, &u64Tick);
   if (result != DAL_SUCCESS)
   {
      return 0;
   }

   result = DalTimetick_CvtFromTimetick64(phTimetickDev,
                                          u64Tick,
                                          T_USEC,
                                          &u64MicroSeconds);
   if (result != DAL_SUCCESS)
   {
      return 0;
   }

   return (uint64)u64MicroSeconds;
}

static void*
AdcBlockingReadCompleteCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 uPayloadSize
   )
{
   AdcBlockingReadType *pAdcBlockingRead = pCtxt;
   AdcResultType *pAdcResult = pPayload;

   *pAdcBlockingRead->pAdcResult = *pAdcResult;

   DALSYS_EventCtrl(pAdcBlockingRead->hSignalEvent, DALSYS_EVENT_CTRL_TRIGGER);

   return NULL;
}

static void*
AdcBlockingRecalCompleteCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 uPayloadSize
   )
{
   AdcBlockingRecalType *pAdcBlockingRecal = pCtxt;
   AdcRecalibrationResultType *pRecalResult = pPayload;

   *pAdcBlockingRecal->pRecalResult = *pRecalResult;

   DALSYS_EventCtrl(pAdcBlockingRecal->hSignalEvent, DALSYS_EVENT_CTRL_TRIGGER);

   return NULL;
}

static void*
AdcQueueTestCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 nPayloadSize
   )
{
   AdcQueueTestCtxtType *pAdcQueueTestCtxt = pCtxt;
   AdcResultType *pAdcResult = pPayload;

   DALSYS_SyncEnter(pAdcQueueTestCtxt->hSync);

   /* Sanity check VPH_PWR */
   if (pAdcResult->eStatus != ADC_RESULT_VALID || pAdcResult->nPhysical < 3500 || pAdcResult->nPhysical > 4500 )
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: queue test received invalid VPH_PWR, physical = %i",
                    pAdcResult->nPhysical);
      guAdcTestNumErrors++;
      DALSYS_SyncLeave(pAdcQueueTestCtxt->hSync);
      return NULL;
   }

   MSG_SPRINTF_1(MSG_SSID_ADC,
                 LVL_INFO,
                 "AdcTest: queue test received valid VPH_PWR, physical = %i",
                 pAdcResult->nPhysical);

   pAdcQueueTestCtxt->uNumConvCompleted++;

   if (pAdcQueueTestCtxt->uNumConvCompleted == pAdcQueueTestCtxt->uNumConvQueued)
   {
      DALSYS_EventCtrl(pAdcQueueTestCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT], DALSYS_EVENT_CTRL_TRIGGER);
   }

   DALSYS_SyncLeave(pAdcQueueTestCtxt->hSync);

   return NULL;
}

static void*
AdcTestTMThresholdCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 nPayloadSize
   )
{
   AdcTMTestCtxtType *pTMTestCtxt = pCtxt;
   AdcTMCallbackPayloadType *pAdcTMPayload = pPayload;

   if (pAdcTMPayload->eThresholdTriggered == ADC_TM_THRESHOLD_LOWER)
   {
      MSG_SPRINTF_3(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: Lower threshold callback received: dev idx = %u meas idx = %u current physical = %i",
                    pAdcTMPayload->adcTMInputProps.uDeviceIdx,
                    pAdcTMPayload->adcTMInputProps.uChannelIdx,
                    pAdcTMPayload->nPhysicalTriggered);
   }
   else
   {
      MSG_SPRINTF_3(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: Higher threshold callback received: dev idx = %u meas idx = %u current physical = %i",
                    pAdcTMPayload->adcTMInputProps.uDeviceIdx,
                    pAdcTMPayload->adcTMInputProps.uChannelIdx,
                    pAdcTMPayload->nPhysicalTriggered);
   }

   DALSYS_EventCtrl(pTMTestCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT], DALSYS_EVENT_CTRL_TRIGGER);

   return NULL;
}

static void*
AdcTestToleranceThresholdCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 nPayloadSize
   )
{
   AdcTMCallbackPayloadType *pAdcTMPayload = pPayload;

   if (pAdcTMPayload->eThresholdTriggered == ADC_TM_THRESHOLD_LOWER)
   {
      MSG_SPRINTF_3(MSG_SSID_ADC,
                    LVL_ERROR,  /* To stand out in logs */
                    "AdcTest: Tolerance (lower) threshold callback received: dev idx = %u meas idx = %u current physical = %i",
                    pAdcTMPayload->adcTMInputProps.uDeviceIdx,
                    pAdcTMPayload->adcTMInputProps.uChannelIdx,
                    pAdcTMPayload->nPhysicalTriggered);
   }
   else
   {
      MSG_SPRINTF_3(MSG_SSID_ADC,
                    LVL_ERROR,  /* To stand out in logs */
                    "AdcTest: Tolerance (upper) threshold callback received: dev idx = %u meas idx = %u current physical = %i",
                    pAdcTMPayload->adcTMInputProps.uDeviceIdx,
                    pAdcTMPayload->adcTMInputProps.uChannelIdx,
                    pAdcTMPayload->nPhysicalTriggered);
   }

   return NULL;
}

static DALResult
AdcBlockingRead(
   AdcBlockingReadType *pAdcBlockingRead,
   AdcInputPropertiesType *pAdcInputProps,
   AdcResultType *pAdcResult
   )
{
   AdcRequestParametersType adcParams;
   DALResult result = DAL_ERROR;

   DALSYS_SyncEnter(pAdcBlockingRead->hSync);
   adcParams.hEvent = pAdcBlockingRead->hCallbackEvent;
   adcParams.nDeviceIdx = pAdcInputProps->nDeviceIdx;
   adcParams.nChannelIdx = pAdcInputProps->nChannelIdx;
   pAdcBlockingRead->pAdcResult = pAdcResult;
   DALSYS_EventCtrl(pAdcBlockingRead->hSignalEvent, DALSYS_EVENT_CTRL_RESET);
   result = DalAdc_RequestConversion(pAdcBlockingRead->phAdcDev, &adcParams, NULL);
   if (result != DAL_SUCCESS)
   {
      DALSYS_SyncLeave(pAdcBlockingRead->hSync);
      return result;
   }
   (void)DALSYS_EventWait(pAdcBlockingRead->hSignalEvent);
   DALSYS_SyncLeave(pAdcBlockingRead->hSync);

   return result;
}

static DALResult
AdcBlockingRecal(
   AdcBlockingRecalType *pAdcBlockingRecal,
   AdcInputPropertiesType *pAdcInputProps,
   AdcRecalibrationResultType *pRecalResult
   )
{
   AdcRequestParametersType adcParams;
   DALResult result = DAL_ERROR;

   DALSYS_SyncEnter(pAdcBlockingRecal->hSync);
   adcParams.hEvent = pAdcBlockingRecal->hCallbackEvent;
   adcParams.nDeviceIdx = pAdcInputProps->nDeviceIdx;
   adcParams.nChannelIdx = pAdcInputProps->nChannelIdx;
   pAdcBlockingRecal->pRecalResult = pRecalResult;
   DALSYS_EventCtrl(pAdcBlockingRecal->hSignalEvent, DALSYS_EVENT_CTRL_RESET);
   result = DalAdc_RequestRecalibration(pAdcBlockingRecal->phAdcDev, &adcParams);
   if (result != DAL_SUCCESS)
   {
      DALSYS_SyncLeave(pAdcBlockingRecal->hSync);
      return result;
   }
   (void)DALSYS_EventWait(pAdcBlockingRecal->hSignalEvent);
   DALSYS_SyncLeave(pAdcBlockingRecal->hSync);

   return result;
}

/*======================================================================

  FUNCTION        AdcTestTMThreshold

  DESCRIPTION     Sets a threshold and blocks until completed or
                  times out.

  DEPENDENCIES    None

  PARAMETERS      pCtxt [in]
                  eTMEvent [in]
                  uWaitTime_us [in]
                  pAdcTMParams [in]
                  eThreshold [in]
                  pnThresholdDesired [in]

  RETURN VALUE    DAL_SUCCESS on success
                  DAL_TIMEOUT if times out
                  DAL_ERROR or other on error

  SIDE EFFECTS    None

======================================================================*/
static DALResult
AdcTestTMThreshold(
   AdcTMTestCtxtType *pCtxt,
   AdcTestTMEventType eTMEvent,
   uint32 uWaitTime_us,
   AdcTMInputPropertiesType *pAdcTMInputProps,
   AdcTMThresholdType eThreshold,
   int32 *pnThresholdDesired
   )
{
   DALResult result;
   AdcTMRequestParametersType adcTMParams;
   int32 nThresholdSet;
   uint32 uEventIdx;

   /*
    * Reset the events
    */

   /* Threshold crossed signal */
   result = DALSYS_EventCtrl(pCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT], DALSYS_EVENT_CTRL_RESET);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Timeout */
   result = DALSYS_EventCtrl(pCtxt->ahEvent[ADC_TEST_EVENT_TIMEOUT], DALSYS_EVENT_CTRL_RESET);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Select the event to send */
   adcTMParams.adcTMInputProps = *pAdcTMInputProps;

   switch (eTMEvent)
   {
      case ADC_TEST_TM_CALLBACK:
         adcTMParams.hEvent = pCtxt->hCallbackEvent;
         break;
      case ADC_TEST_TM_SIGNAL:
         adcTMParams.hEvent = pCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT];
         break;
      default:
         return DAL_ERROR;
   }

   /* Set the threshold */
   result = DalAdc_TMSetThreshold(pCtxt->phAdcDev, &adcTMParams, eThreshold, pnThresholdDesired, &nThresholdSet);
   if (result != DAL_SUCCESS)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: DalAdc_TMSetThreshold failed, return code %i",
                    result);
      return result;
   }

   if (pnThresholdDesired != NULL)
   {
      MSG_SPRINTF_2(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: threshold desired = %i, threshold set = %i",
                    *pnThresholdDesired,
                    nThresholdSet);
   }

   /* Wait for the threshold to be crossed, or time out */
   result = DALSYS_EventMultipleWait(pCtxt->ahEvent,
                                     _ADC_TEST_NUM_EVENTS,
                                     uWaitTime_us,
                                     &uEventIdx);

   return result;
}

static DALResult
AdcTestTMTolerance(
   AdcTMTestCtxtType *pCtxt,
   AdcTestTMEventType eTMEvent,
   uint32 uWaitTime_us,
   AdcTMInputPropertiesType *pAdcTMInputProps,
   int32 nPhysicalCurrent,
   int32 *pnLowTolerance,
   int32 *pnHighTolerance
   )
{
   DALResult result;
   AdcTMRequestParametersType adcTMParams;
   uint32 uEventIdx;

   /*
    * Reset the events
    */

   /* Threshold crossed signal */
   result = DALSYS_EventCtrl(pCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT], DALSYS_EVENT_CTRL_RESET);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Timeout */
   result = DALSYS_EventCtrl(pCtxt->ahEvent[ADC_TEST_EVENT_TIMEOUT], DALSYS_EVENT_CTRL_RESET);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Select the event to send */
   adcTMParams.adcTMInputProps = *pAdcTMInputProps;

   switch (eTMEvent)
   {
      case ADC_TEST_TM_CALLBACK:
         adcTMParams.hEvent = pCtxt->hCallbackEvent;
         break;
      case ADC_TEST_TM_SIGNAL:
         adcTMParams.hEvent = pCtxt->ahEvent[ADC_TEST_EVENT_DEFAULT];
         break;
      default:
         return DAL_ERROR;
   }

   /* Set the tolerance */
   result = DalAdc_TMSetTolerance(pCtxt->phAdcDev, &adcTMParams, nPhysicalCurrent, pnLowTolerance, pnHighTolerance);
   if (result != DAL_SUCCESS)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: DalAdc_TMSetTolerance failed, return code %i",
                    result);
      return result;
   }

   /* Wait for the threshold to be crossed, or time out */
   result = DALSYS_EventMultipleWait(pCtxt->ahEvent,
                                     _ADC_TEST_NUM_EVENTS,
                                     uWaitTime_us,
                                     &uEventIdx);

   return result;
}

static DALResult
AdcBlockingReadInit(
   DalDeviceHandle *phAdcDev,
   AdcBlockingReadType **ppAdcBlockingRead
   )
{
   DALResult result;

   gAdcBlockingRead.phAdcDev = phAdcDev;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &gAdcBlockingRead.hCallbackEvent,
                               &gAdcBlockingRead.callbackEventObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SetupCallbackEvent(gAdcBlockingRead.hCallbackEvent,
                                      AdcBlockingReadCompleteCb,
                                      &gAdcBlockingRead);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                               &gAdcBlockingRead.hSignalEvent,
                               &gAdcBlockingRead.signalEventObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &gAdcBlockingRead.hSync,
                              &gAdcBlockingRead.syncObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   *ppAdcBlockingRead = &gAdcBlockingRead;

   return DAL_SUCCESS;
}

static DALResult
AdcBlockingRecalInit(
   DalDeviceHandle *phAdcDev,
   AdcBlockingRecalType **ppAdcBlockingRecal
   )
{
   DALResult result;

   gAdcBlockingRecal.phAdcDev = phAdcDev;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &gAdcBlockingRecal.hCallbackEvent,
                               &gAdcBlockingRecal.callbackEventObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SetupCallbackEvent(gAdcBlockingRecal.hCallbackEvent,
                                      AdcBlockingRecalCompleteCb,
                                      &gAdcBlockingRecal);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                               &gAdcBlockingRecal.hSignalEvent,
                               &gAdcBlockingRecal.signalEventObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &gAdcBlockingRecal.hSync,
                              &gAdcBlockingRecal.syncObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   *ppAdcBlockingRecal = &gAdcBlockingRecal;

   return DAL_SUCCESS;
}


static DALResult
AdcQueueTestInit(
   DalDeviceHandle *phAdcDev,
   AdcQueueTestCtxtType **ppAdcQueueTestCtxt
   )
{
   DALResult result;

   gAdcQueueTestCtxt.phAdcDev = phAdcDev;

   /* Create a callback event */
   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &gAdcQueueTestCtxt.hCallbackEvent,
                               &gAdcQueueTestCtxt.callbackEventObject);

   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SetupCallbackEvent(gAdcQueueTestCtxt.hCallbackEvent,
                                      AdcQueueTestCb,
                                      &gAdcQueueTestCtxt);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Create a signal event with timeout */
   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                            &gAdcQueueTestCtxt.ahEvent[ADC_TEST_EVENT_DEFAULT],
                            NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT,
                               &gAdcQueueTestCtxt.ahEvent[ADC_TEST_EVENT_TIMEOUT],
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Create a sync object */
   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &gAdcQueueTestCtxt.hSync,
                              &gAdcQueueTestCtxt.syncObject);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Get the VPH_PWR channel */
   result = DalAdc_GetAdcInputProperties(gAdcQueueTestCtxt.phAdcDev,
                                         ADC_INPUT_VPH_PWR,
                                         sizeof(ADC_INPUT_VPH_PWR),
                                         &gAdcQueueTestCtxt.adcInputProps);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   gAdcQueueTestCtxt.adcParams.nDeviceIdx = gAdcQueueTestCtxt.adcInputProps.nDeviceIdx;
   gAdcQueueTestCtxt.adcParams.nChannelIdx = gAdcQueueTestCtxt.adcInputProps.nChannelIdx;
   gAdcQueueTestCtxt.adcParams.hEvent = gAdcQueueTestCtxt.hCallbackEvent;

   *ppAdcQueueTestCtxt = &gAdcQueueTestCtxt;

   return DAL_SUCCESS;
}

static DALResult
AdcTMTestInit(
   DalDeviceHandle *phAdcDev,
   AdcTMTestCtxtType **ppAdcTMTestCtxt
   )
{
   AdcTestTMMeasurementType *pMeasurement;
   DALResult result;
   uint32 uMeas;
   uint32 uChannelNameLength;

   gAdcTMTestCtxt.phAdcDev = phAdcDev;
   gAdcTMTestCtxt.uNumTMChannels = 0;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                               &gAdcTMTestCtxt.ahEvent[ADC_TEST_EVENT_DEFAULT],
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT,
                               &gAdcTMTestCtxt.ahEvent[ADC_TEST_EVENT_TIMEOUT],
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &gAdcTMTestCtxt.hCallbackEvent,
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SetupCallbackEvent(gAdcTMTestCtxt.hCallbackEvent,
                                      AdcTestTMThresholdCb,
                                      &gAdcTMTestCtxt);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Set up tolerance test */
   result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &gAdcToleranceTestCtxt.phAdcDev);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &gAdcToleranceTestCtxt.hCallbackEvent,
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   result = DALSYS_SetupCallbackEvent(gAdcToleranceTestCtxt.hCallbackEvent,
                                      AdcTestToleranceThresholdCb,
                                      &gAdcTMTestCtxt);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   /* Get channel indexes for each meas */
   for (uMeas = 0; uMeas < ARRAY_LENGTH(aTMChannels); uMeas++)
   {
      uChannelNameLength = strlen(aTMChannels[uMeas].pszTMChannel) + 1;
      pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];

      /* Get the TM measurement index */
      result = DalAdc_TMGetInputProperties(gAdcTMTestCtxt.phAdcDev,
                                           aTMChannels[uMeas].pszTMChannel,
                                           uChannelNameLength,
                                           &pMeasurement->adcTMInputProps);

      if (result != DAL_SUCCESS)
      {
         pMeasurement->bPresent = FALSE;
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: no %s VADC TM input found",
                       aTMChannels[uMeas].pszTMChannel);
         continue;
      }

      pMeasurement->bPresent = TRUE;
      gAdcTMTestCtxt.uNumTMChannels++;

      /* Get the ADC read index */
      result = DalAdc_GetAdcInputProperties(gAdcTMTestCtxt.phAdcDev,
                                            aTMChannels[uMeas].pszTMChannel,
                                            uChannelNameLength,
                                            &pMeasurement->adcInputProps);
      if (result != DAL_SUCCESS)
      {
         return result;
      }

      /* Get the range */
      result = DalAdc_TMGetRange(gAdcTMTestCtxt.phAdcDev,
                                 &pMeasurement->adcTMInputProps,
                                 &pMeasurement->adcTMRange);

      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: range failed for %s",
                       aTMChannels[uMeas].pszTMChannel);
         guAdcTestNumErrors++;
         return result;
      }

      MSG_SPRINTF_3(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: %s has a range of %i to %i (physical units)",
                    aTMChannels[uMeas].pszTMChannel,
                    pMeasurement->adcTMRange.nPhysicalMin,
                    pMeasurement->adcTMRange.nPhysicalMax);
   }

   *ppAdcTMTestCtxt = &gAdcTMTestCtxt;

   return DAL_SUCCESS;
}

void AdcTestBlockingRead(void)
{
   AdcTestResultType *pResult;
   DALResult result;
   uint32 uChannel;
   uint64 uStartTime_us;
   uint64 uEndTime_us;
   adc_status_type status;

   for (uChannel = 0; uChannel < ARRAY_LENGTH(aChannels); uChannel++)
   {
      pResult = &gAdcTestCtxt.paResults[uChannel];
      if (pResult->bPresent == TRUE)
      {
         /* First test reads that use ADC's workloop */
         uStartTime_us = AdcTest_GetTimestampUs();

         result = AdcBlockingRead(gAdcTestCtxt.pAdcBlockingRead,
                                  &pResult->channel,
                                  &pResult->readResult);

         uEndTime_us = AdcTest_GetTimestampUs();

         if (result != DAL_SUCCESS || pResult->readResult.eStatus == ADC_RESULT_INVALID)
         {
            MSG_SPRINTF_1(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: failed to read %s (queued method)",
                          pResult->pszChannelName);
            pResult->readResult.eStatus = ADC_RESULT_INVALID;
            guAdcTestNumErrors++;
         }
         else
         {
            if (pResult->readResult.nDeviceIdx != pResult->channel.nDeviceIdx ||
                pResult->readResult.nChannelIdx != pResult->channel.nChannelIdx)
            {
               MSG_SPRINTF_5(MSG_SSID_ADC,
                             LVL_ERROR,
                             "AdcTest: invalid read result for %s (dev %u chan %u) returned dev %u chan %u (queued method)",
                             pResult->pszChannelName,
                             pResult->channel.nDeviceIdx,
                             pResult->channel.nChannelIdx,
                             pResult->readResult.nDeviceIdx,
                             pResult->readResult.nChannelIdx);
               guAdcTestNumErrors++;
            }

            MSG_SPRINTF_7(MSG_SSID_ADC,
                          LVL_INFO,
                          "AdcTest: %s read result: uReadTime_us %u, eStatus %i, nPhysical %i, uPercent %u, uMicrovolts %u, uCode %u (queued method)",
                          pResult->pszChannelName,
                          (uint32)(uEndTime_us - uStartTime_us),
                          pResult->readResult.eStatus,
                          pResult->readResult.nPhysical,
                          pResult->readResult.nPercent,
                          pResult->readResult.nMicrovolts,
                          pResult->readResult.nCode);
         }

         /* Test ADC read function */
         uStartTime_us = AdcTest_GetTimestampUs();

         result = DalAdc_Read(gAdcTestCtxt.phAdcDev,
                              &pResult->channel,
                              &pResult->readResult);

         uEndTime_us = AdcTest_GetTimestampUs();

         if (result != DAL_SUCCESS || pResult->readResult.eStatus == ADC_RESULT_INVALID)
         {
            MSG_SPRINTF_1(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: failed to read %s (blocking method)",
                          pResult->pszChannelName);
            pResult->readResult.eStatus = ADC_RESULT_INVALID;
            guAdcTestNumErrors++;
         }
         else
         {
            if (pResult->readResult.nDeviceIdx != pResult->channel.nDeviceIdx ||
                pResult->readResult.nChannelIdx != pResult->channel.nChannelIdx)
            {
               MSG_SPRINTF_5(MSG_SSID_ADC,
                             LVL_ERROR,
                             "AdcTest: invalid read result for %s (dev %u chan %u) returned dev %u chan %u (blocking method)",
                             pResult->pszChannelName,
                             pResult->channel.nDeviceIdx,
                             pResult->channel.nChannelIdx,
                             pResult->readResult.nDeviceIdx,
                             pResult->readResult.nChannelIdx);
               guAdcTestNumErrors++;
            }

            MSG_SPRINTF_7(MSG_SSID_ADC,
                          LVL_INFO,
                          "AdcTest: %s read result: uReadTime_us %u, eStatus %i, nPhysical %i, uPercent %u, uMicrovolts %u, uCode %u (blocking method)",
                          pResult->pszChannelName,
                          (uint32)(uEndTime_us - uStartTime_us),
                          pResult->readResult.eStatus,
                          pResult->readResult.nPhysical,
                          pResult->readResult.nPercent,
                          pResult->readResult.nMicrovolts,
                          pResult->readResult.nCode);
         }

         /* Test ADC wrapper */
         uStartTime_us = AdcTest_GetTimestampUs();

         status = adc_read(pResult->pszChannelName,
                           &pResult->wrapperResult);

         uEndTime_us = AdcTest_GetTimestampUs();

         if (status != ADC_STATUS_SUCCESS)
         {
            MSG_SPRINTF_1(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: failed to read %s (wrapper method)",
                          pResult->pszChannelName);
            guAdcTestNumErrors++;
         }
         else
         {
            MSG_SPRINTF_5(MSG_SSID_ADC,
                          LVL_INFO,
                          "AdcTest: %s read result: uReadTime_us %u, nPhysical %i, uMicrovolts %u, uCode %u (wrapper method)",
                          pResult->pszChannelName,
                          (uint32)(uEndTime_us - uStartTime_us),
                          pResult->wrapperResult.physical,
                          pResult->wrapperResult.microvolts,
                          pResult->wrapperResult.code);
         }
      }
   }

   return;
}

void AdcTestBlockingRecal(void)
{
   AdcTestResultType *pResult;
   DALResult result;
   uint32 uChannel;
   uint64 uStartTime_us;
   uint64 uEndTime_us;

   for (uChannel = 0; uChannel < ARRAY_LENGTH(aChannels); uChannel++)
   {
      pResult = &gAdcTestCtxt.paResults[uChannel];
      if (pResult->bPresent == TRUE)
      {
         uStartTime_us = AdcTest_GetTimestampUs();

         result = AdcBlockingRecal(gAdcTestCtxt.pAdcBlockingRecal,
                                   &pResult->channel,
                                   &pResult->recalResult);

         uEndTime_us = AdcTest_GetTimestampUs();

         if (result != DAL_SUCCESS || pResult->recalResult.eStatus == ADC_RESULT_INVALID)
         {
            MSG_SPRINTF_1(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: failed to recal %s",
                          pResult->pszChannelName);
            pResult->recalResult.eStatus = ADC_RESULT_INVALID;
            guAdcTestNumErrors++;
         }
         else
         {
            if (pResult->recalResult.uDeviceIdx != pResult->channel.nDeviceIdx ||
                pResult->recalResult.uChannelIdx != pResult->channel.nChannelIdx)
            {
               MSG_SPRINTF_5(MSG_SSID_ADC,
                             LVL_ERROR,
                             "AdcTest: invalid recal result for %s (dev %u chan %u) returned (dev %u chan %u)",
                             pResult->pszChannelName,
                             pResult->channel.nDeviceIdx,
                             pResult->channel.nChannelIdx,
                             pResult->recalResult.uDeviceIdx,
                             pResult->recalResult.uChannelIdx);
               guAdcTestNumErrors++;
            }

            MSG_SPRINTF_7(MSG_SSID_ADC,
                          LVL_INFO,
                          "AdcTest: %s recal result: uReadTime_us %u, eStatus %i, nPhysical1_uV %i, nPhysical1_uV %i, uCode1 %u, uCode2 %u",
                          pResult->pszChannelName,
                          (uint32)(uEndTime_us - uStartTime_us),
                          pResult->recalResult.eStatus,
                          pResult->recalResult.nPhysical1_uV,
                          pResult->recalResult.nPhysical2_uV,
                          pResult->recalResult.uCode1,
                          pResult->recalResult.uCode2);
         }
      }
   }

   return;
}

/*======================================================================

  FUNCTION        AdcQueueTest

  DESCRIPTION     Exercises the queue. Fills up the queue and lets it
                  drain. Channel is VPH_PWR and the range is checked.

  DEPENDENCIES    None

  PARAMETERS      None

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
void AdcQueueTest(void)
{
   AdcQueueTestCtxtType *pAdcQueueTestCtxt = gAdcTestCtxt.pAdcQueueTestCtxt;
   DALResult result;
   uint32 uAdcRequest;
   AdcRequestStatusType adcReqStatus;
   uint32 uEventIdx;

   pAdcQueueTestCtxt->uNumConvCompleted = 0;
   pAdcQueueTestCtxt->uNumConvQueued = 0;

   DALSYS_SyncEnter(pAdcQueueTestCtxt->hSync);

   for (uAdcRequest = 0; uAdcRequest < ADC_MAX_QUEUE_SIZE; uAdcRequest++)
   {
      result = DalAdc_RequestConversion(pAdcQueueTestCtxt->phAdcDev,
                                        &pAdcQueueTestCtxt->adcParams,
                                        &adcReqStatus);

      if (result == ADC_ERROR_DEVICE_QUEUE_FULL)
      {
         pAdcQueueTestCtxt->uNumConvQueued = uAdcRequest;
         break;
      }
      else if (uAdcRequest == (ADC_MAX_QUEUE_SIZE - 1))
      {
         pAdcQueueTestCtxt->uNumConvQueued = uAdcRequest + 1;
      }
   }

   DALSYS_SyncLeave(pAdcQueueTestCtxt->hSync);

   MSG_SPRINTF_1(MSG_SSID_ADC,
                 LVL_INFO,
                 "AdcTest: queue test queued %u readings",
                 pAdcQueueTestCtxt->uNumConvQueued);

   if (pAdcQueueTestCtxt->uNumConvQueued == 0)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: queue test failed to fill queue");
      guAdcTestNumErrors++;
      return;
   }

   /* Wait for the queue to empty or time out after 1 second */
   result = DALSYS_EventMultipleWait(pAdcQueueTestCtxt->ahEvent,
                                     _ADC_TEST_NUM_EVENTS,
                                     1000000,
                                     &uEventIdx);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: queue test failed");
      guAdcTestNumErrors++;
   }
   else
   {
      MSG(MSG_SSID_ADC, LVL_INFO, "AdcTest: queue test passed");
   }

   return;
}

/*======================================================================

  FUNCTION        AdcTMTest

  DESCRIPTION     Tests the TM feature
                   - Thresholds at limits
                   - Thresholds at current physical +/- delta

  DEPENDENCIES    None

  PARAMETERS      None

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void AdcTMTest(void)
{
   const AdcTestTMChannelType *pTMChannel;
   AdcTestTMMeasurementType *pMeasurement;
   AdcTMRequestParametersType adcTMParams;
   AdcResultType adcResult;
   DALResult result;
   uint32 uMeas;
   int32 nThresholdDesired;
   uint32 uEventIdx;
   int32 nMinVal;
   int32 nMaxVal;
   int32 nThreshold;
   int32 nMaxDelta;
   int32 nThresholdSet;
   int32 nDelta;
   int32 nTolerance;

   if (gAdcTMTestCtxt.uNumTMChannels == 0)
   {
      return;
   }

   /*
    * Test thresholds for each meas - use callbacks vs events
    */
   for (uMeas = 0; uMeas < ARRAY_LENGTH(aTMChannels); uMeas++)
   {
      pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];
      if (pMeasurement->bPresent == FALSE)
      {
         continue;
      }

      pTMChannel = &aTMChannels[uMeas];

      /* Read the current value */
      result = DalAdc_Read(gAdcTestCtxt.phAdcDev,
                           &pMeasurement->adcInputProps,
                           &adcResult);

      if (result == DAL_SUCCESS && adcResult.eStatus == ADC_RESULT_VALID)
      {
         pMeasurement->nCurrentPhysical = adcResult.nPhysical;
      }
      else
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: failed to read channel %s",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }

      /*
       * Thresholds based on current value +/- delta
       */

      /* Higher threshold - times out */
      nThresholdDesired = pMeasurement->nCurrentPhysical + pTMChannel->nPhysicalDelta;
      nThresholdDesired = minval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMax);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_HIGHER,
                                  &nThresholdDesired);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s did not time out for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully timed out for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Higher threshold - triggers */
      nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
      nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_HIGHER,
                                  &nThresholdDesired);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Higher threshold - triggers (pass 2) */
      nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
      nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_HIGHER,
                                  &nThresholdDesired);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for high threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Lower threshold - times out */
      nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
      nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_LOWER,
                                  &nThresholdDesired);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s did not time out for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully timed out for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Lower threshold - triggers */
      nThresholdDesired = pMeasurement->nCurrentPhysical + pTMChannel->nPhysicalDelta;
      nThresholdDesired = minval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMax);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_LOWER,
                                  &nThresholdDesired);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Lower threshold - triggers (pass 2) */
      nThresholdDesired = pMeasurement->nCurrentPhysical + pTMChannel->nPhysicalDelta;
      nThresholdDesired = minval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMax);
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_LOWER,
                                  &nThresholdDesired);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for low threshold of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Cancel thresholds */
      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_LOWER,
                                  NULL);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to cancel threshold",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: successfully cleared lower threshold for channel %s",
                       pTMChannel->pszTMChannel);
      }

      result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  ADC_TM_THRESHOLD_HIGHER,
                                  NULL);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to cancel threshold",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: successfully cleared higher threshold for channel %s",
                       pTMChannel->pszTMChannel);
      }

      /* Test lower tolerance */
      nTolerance = 1;
      nThresholdDesired = pMeasurement->nCurrentPhysical + pTMChannel->nPhysicalDelta;
      nThresholdDesired = minval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMax);
      result = AdcTestTMTolerance(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  nThresholdDesired,
                                  &nTolerance,
                                  NULL);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for low tolerance with mock current physical of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for low tolerance with mock current physical of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Clear tolerances */
      result = AdcTestTMTolerance(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  nThresholdDesired,
                                  NULL,
                                  NULL);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to cancel tolerance",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: successfully cleared tolerance for channel %s",
                       pTMChannel->pszTMChannel);
      }

      /* Test upper tolerance */
      nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
      nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);
      result = AdcTestTMTolerance(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  nThresholdDesired,
                                  NULL,
                                  &nTolerance);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s timed out for high tolerance with mock current physical of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: channel %s successfully triggered for high tolerance with mock current physical of %i",
                       pTMChannel->pszTMChannel,
                       nThresholdDesired);
      }

      /* Clear tolerances */
      result = AdcTestTMTolerance(&gAdcTMTestCtxt,
                                  ADC_TEST_TM_CALLBACK,
                                  pTMChannel->uWaitTime_us,
                                  &pMeasurement->adcTMInputProps,
                                  nThresholdDesired,
                                  NULL,
                                  NULL);
      if (result != DAL_ERROR_TIMEOUT)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to cancel tolerance",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }
      else
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_INFO,
                       "AdcTest: successfully cleared tolerance for channel %s",
                       pTMChannel->pszTMChannel);
      }
   }

   /*
    * Test disable client, set threshold, enable client
    */

   /* Test disabling thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, FALSE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   /* Set the threshold (it will reset the event) */
   uMeas = 0;
   pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];
   pTMChannel = &aTMChannels[uMeas];
   nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
   nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);

   result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                               ADC_TEST_TM_SIGNAL,
                               pTMChannel->uWaitTime_us,
                               &pMeasurement->adcTMInputProps,
                               ADC_TM_THRESHOLD_HIGHER,
                               &nThresholdDesired);
   if (result != DAL_ERROR_TIMEOUT)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: channel %s triggered while client is disabled",
                    pTMChannel->pszTMChannel);
      guAdcTestNumErrors++;
   }

   /* Test re-enabling thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, TRUE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   /* Now the event should be set since thresholds were re-enabled */
   result = DALSYS_EventMultipleWait(gAdcTMTestCtxt.ahEvent,
                                     _ADC_TEST_NUM_EVENTS,
                                     pTMChannel->uWaitTime_us,
                                     &uEventIdx);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to receive threshold after re-enabling thresholds");
      guAdcTestNumErrors++;
   }
   else
   {
      MSG(MSG_SSID_ADC, LVL_INFO, "AdcTest: passed disable - set - enable TM test");
   }

   /*
    * Test clearing thresholds
    */

   /* Disable thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, FALSE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   /* Set a valid threshold */
   uMeas = 0;
   pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];
   pTMChannel = &aTMChannels[uMeas];
   nThresholdDesired = pMeasurement->nCurrentPhysical - pTMChannel->nPhysicalDelta;
   nThresholdDesired = maxval(nThresholdDesired, pMeasurement->adcTMRange.nPhysicalMin);

   result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                               ADC_TEST_TM_SIGNAL,
                               pTMChannel->uWaitTime_us,
                               &pMeasurement->adcTMInputProps,
                               ADC_TM_THRESHOLD_HIGHER,
                               &nThresholdDesired);
   if (result != DAL_ERROR_TIMEOUT)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: channel %s triggered while client is disabled",
                    pTMChannel->pszTMChannel);
      guAdcTestNumErrors++;
   }

   /* Clear the threshold */
   result = AdcTestTMThreshold(&gAdcTMTestCtxt,
                               ADC_TEST_TM_SIGNAL,
                               pTMChannel->uWaitTime_us,
                               &pMeasurement->adcTMInputProps,
                               ADC_TM_THRESHOLD_HIGHER,
                               NULL);
   if (result != DAL_ERROR_TIMEOUT)
   {
      MSG_SPRINTF_1(MSG_SSID_ADC,
                    LVL_ERROR,
                    "AdcTest: channel %s triggered while client is disabled",
                    pTMChannel->pszTMChannel);
      guAdcTestNumErrors++;
   }

   /* Re-enable thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, TRUE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   /* Should time out */
   result = DALSYS_EventMultipleWait(gAdcTMTestCtxt.ahEvent,
                                     _ADC_TEST_NUM_EVENTS,
                                     pTMChannel->uWaitTime_us,
                                     &uEventIdx);
   if (result != DAL_ERROR_TIMEOUT)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed clearing threshold TM test");
      guAdcTestNumErrors++;
   }
   else
   {
      MSG(MSG_SSID_ADC, LVL_INFO, "AdcTest: passed clearing threshold TM test");
   }

   /*
    * Verify thresholds can be set over the entire range
    */

   /* Disable thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, FALSE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   for (uMeas = 0; uMeas < ARRAY_LENGTH(aTMChannels); uMeas++)
   {
      pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];
      if (pMeasurement->bPresent == FALSE)
      {
         continue;
      }

      pTMChannel = &aTMChannels[uMeas];

      adcTMParams.adcTMInputProps = pMeasurement->adcTMInputProps;
      adcTMParams.hEvent = gAdcTMTestCtxt.hCallbackEvent;

      nMinVal = maxval(pMeasurement->adcTMRange.nPhysicalMin, pTMChannel->nMinVal);
      nMaxVal = minval(pMeasurement->adcTMRange.nPhysicalMax, pTMChannel->nMaxVal);
      nThreshold = nMinVal;
      nMaxDelta = 0;

      for ( ; ; )
      {
         /* Set a low threshold */
         result = DalAdc_TMSetThreshold(gAdcTMTestCtxt.phAdcDev,
                                        &adcTMParams,
                                        ADC_TM_THRESHOLD_LOWER,
                                        &nThreshold,
                                        &nThresholdSet);
         if (result != DAL_SUCCESS)
         {
            MSG_SPRINTF_2(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: channel %s failed to set lower threshold of %i",
                          pTMChannel->pszTMChannel,
                          nThreshold);
            guAdcTestNumErrors++;
         }
         else
         {
            nDelta = nThresholdSet - nThreshold;
            if (nDelta < 0)
            {
               nDelta *= -1;
            }
            nMaxDelta = maxval(nMaxDelta, nDelta);

            if (nThresholdSet > nThreshold)
            {
               MSG_SPRINTF_3(MSG_SSID_ADC,
                             LVL_ERROR,
                             "AdcTest: channel %s desired lower threshold of %i but invalid %i threshold set",
                             pTMChannel->pszTMChannel,
                             nThreshold,
                             nThresholdSet);
               guAdcTestNumErrors++;
            }
         }

         /* Set a high threshold */
         result = DalAdc_TMSetThreshold(gAdcTMTestCtxt.phAdcDev,
                                        &adcTMParams,
                                        ADC_TM_THRESHOLD_HIGHER,
                                        &nThreshold,
                                        &nThresholdSet);
         if (result != DAL_SUCCESS)
         {
            MSG_SPRINTF_2(MSG_SSID_ADC,
                          LVL_ERROR,
                          "AdcTest: channel %s failed to set higher threshold of %i",
                          pTMChannel->pszTMChannel,
                          nThreshold);
            guAdcTestNumErrors++;
         }
         else
         {
            nDelta = nThresholdSet - nThreshold;
            if (nDelta < 0)
            {
               nDelta *= -1;
            }
            nMaxDelta = maxval(nMaxDelta, nDelta);

            if (nThresholdSet < nThreshold)
            {
               MSG_SPRINTF_3(MSG_SSID_ADC,
                             LVL_ERROR,
                             "AdcTest: channel %s desired higher threshold of %i but invalid %i threshold set",
                             pTMChannel->pszTMChannel,
                             nThreshold,
                             nThresholdSet);
               guAdcTestNumErrors++;
            }
         }

         if (nThreshold == nMaxVal)
         {
            break;
         }

         nThreshold += pTMChannel->nStep;
         if (nThreshold > nMaxVal)
         {
            nThreshold = nMaxVal;
         }
      }

      MSG_SPRINTF_2(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: channel %s had a max threshold delta of %i",
                    pTMChannel->pszTMChannel,
                    nMaxDelta);

      /* Clear the thresholds */
      result = DalAdc_TMSetThreshold(gAdcTMTestCtxt.phAdcDev,
                                     &adcTMParams,
                                     ADC_TM_THRESHOLD_LOWER,
                                     NULL,
                                     &nThresholdSet);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to clear lower threshold",
                       pTMChannel->pszTMChannel,
                       nThreshold);
         guAdcTestNumErrors++;
      }

      result = DalAdc_TMSetThreshold(gAdcTMTestCtxt.phAdcDev,
                                     &adcTMParams,
                                     ADC_TM_THRESHOLD_HIGHER,
                                     NULL,
                                     &nThresholdSet);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_2(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: channel %s failed to clear higher threshold",
                       pTMChannel->pszTMChannel,
                       nThreshold);
         guAdcTestNumErrors++;
      }

      /* Brief sleep to allow other threads to run */
      AdcTest_Sleep(1000);
   }

   /* Re-enable thresholds */
   result = DalAdc_TMSetEnableThresholds(gAdcTMTestCtxt.phAdcDev, TRUE);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: DalAdc_TMSetEnableThresholds failed");
      guAdcTestNumErrors++;
   }

   return;
}

static void AdcToleranceTest(void)
{
   const AdcTestTMChannelType *pTMChannel;
   AdcTestTMMeasurementType *pMeasurement;
   AdcTMRequestParametersType adcTMParams;
   DALResult result;
   uint32 uMeas;
   int32 nDelta;

   if (gAdcTMTestCtxt.uNumTMChannels == 0)
   {
      return;
   }

   for (uMeas = 0; uMeas < ARRAY_LENGTH(aTMChannels); uMeas++)
   {
      pMeasurement = &gAdcTMTestCtxt.paMeasurements[uMeas];
      pTMChannel = &aTMChannels[uMeas];
      if (pMeasurement->bPresent == FALSE || pTMChannel->nTolerance == 0)
      {
         continue;
      }

      nDelta = pTMChannel->nTolerance;
      adcTMParams.adcTMInputProps = pMeasurement->adcTMInputProps;
      adcTMParams.hEvent = gAdcToleranceTestCtxt.hCallbackEvent;

      result = DalAdc_TMSetTolerance(gAdcToleranceTestCtxt.phAdcDev,
                                     &adcTMParams,
                                     0, // TODO: add nPhysicalCurrent
                                     &nDelta,
                                     &nDelta);
      if (result != DAL_SUCCESS)
      {
         MSG_SPRINTF_1(MSG_SSID_ADC,
                       LVL_ERROR,
                       "AdcTest: failed to set tolerance for channel %s",
                       pTMChannel->pszTMChannel);
         guAdcTestNumErrors++;
      }
   }
}

static DALResult
AdcTestWorkLoop(
   DALSYSEventHandle hEvent,
   void *pInDevCtxt
   )
{
   AdcInputPropertiesType channel;
   AdcTestResultType *pResult;
   DALResult result;
   uint32 uChannel;
   uint32 uNumIterations = 0;

   /* Sleep to allow QXDM to connect */
   AdcTest_Sleep(10000000);

   MSG(MSG_SSID_ADC, LVL_INFO, "AdcTest: work loop has started");

   /* Attach to the ADC device */
   result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &gAdcTestCtxt.phAdcDev);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to attach to ADC");
      return result;
   }

   /* Make sure detach works */
   result = DAL_DeviceDetach(gAdcTestCtxt.phAdcDev);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to detach from ADC");
      return result;
   }

   /* Reattach to the ADC device */
   result = DAL_AdcDeviceAttach(DALDEVICEID_ADC, &gAdcTestCtxt.phAdcDev);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to reattach to ADC");
      return result;
   }

   /* Set up blocking reads */
   result = AdcBlockingReadInit(gAdcTestCtxt.phAdcDev, &gAdcTestCtxt.pAdcBlockingRead);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to set up blocking read");
      return result;
   }

   /* Set up blocking recalibration requests */
   result = AdcBlockingRecalInit(gAdcTestCtxt.phAdcDev, &gAdcTestCtxt.pAdcBlockingRecal);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to set up blocking recal");
      return result;
   }

   /* Set up the queue test */
   result = AdcQueueTestInit(gAdcTestCtxt.phAdcDev, &gAdcTestCtxt.pAdcQueueTestCtxt);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to set up queue test");
      return result;
   }

   /* Set up TM test */
   result = AdcTMTestInit(gAdcTestCtxt.phAdcDev, &gAdcTestCtxt.pTMTestCtxt);
   if (result != DAL_SUCCESS)
   {
      MSG(MSG_SSID_ADC, LVL_ERROR, "AdcTest: failed to set up TM test context");
      return result;
   }

   /* Look up the device / channel indexes */
   for (uChannel = 0; uChannel < ARRAY_LENGTH(aChannels); uChannel++)
   {
      pResult = &gAdcTestCtxt.paResults[uChannel];
      pResult->pszChannelName = aChannels[uChannel];
      result = DalAdc_GetAdcInputProperties(gAdcTestCtxt.phAdcDev,
                                            pResult->pszChannelName,
                                            strlen(pResult->pszChannelName) + 1,
                                            &channel);
      if (result != DAL_SUCCESS)
      {
         pResult->bPresent = FALSE;
      }
      else
      {
         pResult->bPresent = TRUE;
         pResult->channel = channel;
      }
   }

   /* Perform the tests in a loop */
   for ( ; ; )
   {
      AdcTestBlockingRead();
      AdcTestBlockingRecal();
      AdcQueueTest();
      AdcTMTest();
      if ((uNumIterations % 10000 == 0 && uNumIterations > 0) ||
          uNumIterations == 5)
      {
         /* Allow some time for tolerances to remain armed */
         AdcToleranceTest();
      }
      uNumIterations++;

      MSG_SPRINTF_2(MSG_SSID_ADC,
                    LVL_INFO,
                    "AdcTest: number of iterations: %u, number of errors so far: %u",
                    uNumIterations,
                    guAdcTestNumErrors);

      AdcTest_Sleep(10000000);
   }
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
int AdcTest_Init(void)
{
   static DALSYSWorkLoopHandle hAdcTestWorkLoop;
   static DALSYSEventHandle hAdcTestEvent;
   DALResult result;

   DALSYS_InitMod(NULL);

   /* Create the work-loop */
   result = DALSYS_RegisterWorkLoopEx("ADC_TEST_WORK_LOOP",
                                      ADC_TEST_DEFAULT_STACK_SIZE,
                                      0,
                                      10,
                                      &hAdcTestWorkLoop,
                                      NULL);
   if (result != DAL_SUCCESS)
   {
      return -1;
   }

   /* Create a workloop event */
   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT,
                               &hAdcTestEvent,
                               NULL);
   if (result != DAL_SUCCESS)
   {
      return -1;
   }

   /* Add the event to the workloop */
   result = DALSYS_AddEventToWorkLoop(hAdcTestWorkLoop,
                                      AdcTestWorkLoop,
                                      NULL,
                                      hAdcTestEvent,
                                      NULL);
   if (result != DAL_SUCCESS)
   {
      return -1;
   }

   result = DALSYS_EventCtrlEx(hAdcTestEvent,
                               DALSYS_EVENT_CTRL_TRIGGER,
                               0,
                               NULL,
                               0);
   if (result != DAL_SUCCESS)
   {
      return -1;
   }

   return 0;
}

