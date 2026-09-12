/*============================================================================
  FILE:         DalAdc.c

  OVERVIEW:     Implementation of the ADC DAL

  DEPENDENCIES: None

                Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/dal/src/DalAdc.c#3 $$DateTime: 2021/06/21 03:43:43 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2015-03-27  jjo  Added tolerance API.
  2013-05-22  jjo  Moved the work loop to the DAL ADC layer; one shared
                   device queue; added synchronous read support.
  2013-02-26  jjo  Added TM APIs.
  2012-10-18  jjo  Added a result type for recalibration.
  2012-06-13  jjo  Updated to use DAL device ID string names.
  2012-04-19  jdt  Updated for 8974: Removed remote support. Removed batch APIs.
  2009-03-16  jdt  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DalAdc.h"
#include "DDIAdcDevice.h"
#include "DDITimetick.h"
#include "pm_version_p.h"
/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ADC_DEFAULT_STACK_SIZE 0x2000
#define ADC_LOG_MAX 30

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
DALSYSSyncHandle ghAdcDebugSync;
DALSYS_SYNC_OBJECT(ghAdcDebugSyncObject);

struct
{
   struct
   {
      enum
      {
         ADC_LOG_EVENT_REQUEST_CONVERSION = 1,
         ADC_LOG_EVENT_REQUEST_RECALIBRATION,
         ADC_LOG_EVENT_CONVERSION_COMPLETE,
         ADC_LOG_EVENT_CALIBRATION_COMPLETE
      } eEvent;
      uint64 uTimestamp;
      uint32 uDeviceIdx;
      uint32 uChannelIdx;
      DALSYSEventHandle hClientEvent;
   } aLog[ADC_LOG_MAX];
   DalDeviceHandle *phTimetickDev;
   uint32 uCurIdx;
} gAdcDebug;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
/*
 * Work loop priority - this is a global so it can be easily changed
 * in the debugger
 */
static uint32 gAdcWorkloopPriority = 0;

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
/*======================================================================

  FUNCTION        Adc_GetTimetick

  DESCRIPTION     Gets the timetick

  DEPENDENCIES    None

  PARAMETERS      None

  RETURN VALUE    64-bit timetick

  SIDE EFFECTS    None

======================================================================*/
static uint64
Adc_GetTimetick(void)
{
   DALResult result;
   DalTimetickTime64Type u64Tick;

   result = DalTimetick_GetTimetick64(gAdcDebug.phTimetickDev, &u64Tick);
   if (result != DAL_SUCCESS)
   {
      return 0;
   }

   return (uint64)u64Tick;
}

/*======================================================================

  FUNCTION        Adc_LogConversionRequest

  DESCRIPTION     This function logs a conversion request

  DEPENDENCIES    None

  PARAMETERS      uDeviceIdx  [in] the device index
                  uChannelIdx [in] the channel index

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void
Adc_LogConversionRequest(
   uint32 uDeviceIdx,
   uint32 uChannelIdx
   )
{
   DALSYS_SyncEnter(ghAdcDebugSync);
   gAdcDebug.aLog[gAdcDebug.uCurIdx].eEvent = ADC_LOG_EVENT_REQUEST_CONVERSION;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uTimestamp = Adc_GetTimetick();
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uDeviceIdx = uDeviceIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uChannelIdx = uChannelIdx;

   if (gAdcDebug.uCurIdx < ADC_LOG_MAX - 1)
   {
      gAdcDebug.uCurIdx++;
   }
   else
   {
      gAdcDebug.uCurIdx = 0;
   }
   DALSYS_SyncLeave(ghAdcDebugSync);
}

/*======================================================================

  FUNCTION        Adc_LogRecalibrationRequest

  DESCRIPTION     This function logs a recalibration request

  DEPENDENCIES    None

  PARAMETERS      uDeviceIdx  [in] the device index
                  uChannelIdx [in] the channel index

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void
Adc_LogRecalibrationRequest(
   uint32 uDeviceIdx,
   uint32 uChannelIdx
   )
{
   DALSYS_SyncEnter(ghAdcDebugSync);
   gAdcDebug.aLog[gAdcDebug.uCurIdx].eEvent = ADC_LOG_EVENT_REQUEST_RECALIBRATION;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uTimestamp = Adc_GetTimetick();
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uDeviceIdx = uDeviceIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uChannelIdx = uChannelIdx;

   if (gAdcDebug.uCurIdx < ADC_LOG_MAX - 1)
   {
      gAdcDebug.uCurIdx++;
   }
   else
   {
      gAdcDebug.uCurIdx = 0;
   }
   DALSYS_SyncLeave(ghAdcDebugSync);
}

/*======================================================================

  FUNCTION        Adc_LogConversionComplete

  DESCRIPTION     This function logs when a conversion completes

  DEPENDENCIES    None

  PARAMETERS      uDeviceIdx   [in] the device index
                  uChannelIdx  [in] the channel index
                  hClientEvent [in] the client event

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void
Adc_LogConversionComplete(
   uint32 uDeviceIdx,
   uint32 uChannelIdx,
   const DALSYSEventHandle hClientEvent
   )
{
   DALSYS_SyncEnter(ghAdcDebugSync);
   gAdcDebug.aLog[gAdcDebug.uCurIdx].eEvent = ADC_LOG_EVENT_CONVERSION_COMPLETE;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uTimestamp = Adc_GetTimetick();
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uDeviceIdx = uDeviceIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uChannelIdx = uChannelIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].hClientEvent = hClientEvent;

   if (gAdcDebug.uCurIdx < ADC_LOG_MAX - 1)
   {
      gAdcDebug.uCurIdx++;
   }
   else
   {
      gAdcDebug.uCurIdx = 0;
   }
   DALSYS_SyncLeave(ghAdcDebugSync);
}

/*======================================================================

  FUNCTION        Adc_LogCalibrationComplete

  DESCRIPTION     This function logs when calibration is complete

  DEPENDENCIES    None

  PARAMETERS      uDeviceIdx   [in] the device index
                  uChannelIdx  [in] the channel index
                  hClientEvent [in] the client event

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void
Adc_LogCalibrationComplete(
   uint32 uDeviceIdx,
   uint32 uChannelIdx,
   const DALSYSEventHandle hClientEvent
   )
{
   DALSYS_SyncEnter(ghAdcDebugSync);
   gAdcDebug.aLog[gAdcDebug.uCurIdx].eEvent = ADC_LOG_EVENT_CALIBRATION_COMPLETE;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uTimestamp = Adc_GetTimetick();
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uDeviceIdx = uDeviceIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].uChannelIdx = uChannelIdx;
   gAdcDebug.aLog[gAdcDebug.uCurIdx].hClientEvent = hClientEvent;

   if (gAdcDebug.uCurIdx < ADC_LOG_MAX - 1)
   {
      gAdcDebug.uCurIdx++;
   }
   else
   {
      gAdcDebug.uCurIdx = 0;
   }
   DALSYS_SyncLeave(ghAdcDebugSync);
}

/*======================================================================

  FUNCTION        Adc_AddDeviceQueueRequest

  DESCRIPTION     This function adds a conversion or recalibration
                  request to the device queue.

  DEPENDENCIES    None

  PARAMETERS
      pDevCtxt          [in]  pointer to the ADC device context
      eAdcDeviceRequest [in]  which kind of request to add
      nDeviceIdx        [in]  index to the ADC device
      nChannelIdx       [in]  index to the ADC channel
      hClientEvent      [in]  event to signal when the request is completed
                              set hEvent = NULL to block on the completion
      pAdcRequestStatus [out] pointer to the request status; only set if
                              non-null.

  RETURN VALUE    DAL_SUCCESS if request was added to the queue
                  ADC_DEVICE_QUEUE_FULL if the queue was full

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_AddDeviceQueueRequest(
   AdcClientCtxt *pCtxt,
   AdcDeviceRequestType eAdcDeviceRequest,
   uint32 uDeviceIdx,
   uint32 uChannelIdx,
   const DALSYSEventHandle hClientEvent,
   AdcRequestStatusType *pAdcRequestStatus
   )
{
   AdcDeviceQueueNodeType *pNode;
   DALResult ret = DAL_SUCCESS;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   uint32 uQueueIdx = pDevCtxt->pBsp->paAdcPhysicalDevices[uDeviceIdx].uQueueIdx;
   AdcQueueType *pQueue = &pDevCtxt->paQueues[uQueueIdx];

   DALSYS_SyncEnter(pDevCtxt->hSync);

   /* Get a node from the pool of free nodes */
   pNode = (AdcDeviceQueueNodeType *)DALFW_Q_Get(&pQueue->qFree);
   if (pNode != NULL)
   {
      /*
       * A free node is available, so copy the request data into the free node
       * and move the node from the list of free nodes to the appropriate queue.
       */
      pNode->hClientEvent = hClientEvent;
      pNode->uDeviceIdx = uDeviceIdx;
      pNode->uChannelIdx = uChannelIdx;
      pNode->eAdcDeviceRequest = eAdcDeviceRequest;

      if (DALFW_Q_Check(&pQueue->qPending) != NULL)
      {
         /* Queue the node */
         pNode->adcRequestStatus.eStatus = ADC_REQUEST_STATUS_QUEUED;
         if (pAdcRequestStatus != NULL)
         {
            pAdcRequestStatus->eStatus = ADC_REQUEST_STATUS_QUEUED;
         }
         DALFW_Q_Put(&pQueue->qQueued, (DALFW_Q_LinkType * )pNode);
      }
      else
      {
         /* Place the node in pending */
         pNode->adcRequestStatus.eStatus = ADC_REQUEST_STATUS_PENDING;
         if (pAdcRequestStatus != NULL)
         {
            pAdcRequestStatus->eStatus = ADC_REQUEST_STATUS_PENDING;
         }
         DALFW_Q_Put(&pQueue->qPending, (DALFW_Q_LinkType *)pNode);

         /* Queue the work loop for processing */
         DALSYS_EventCtrlEx(pQueue->hAdcOpEvent,
                            DALSYS_EVENT_CTRL_TRIGGER,
                            0,
                            NULL,
                            0);
      }
   }
   else
   {
      if (pAdcRequestStatus != NULL)
      {
         pAdcRequestStatus->eStatus = ADC_REQUEST_STATUS_ERROR;
      }

      DALSYS_LogEvent(pDevCtxt->DevId,
                      DALSYS_LOGEVENT_INFO,
                      "Adc device request queue is full");
      ret = ADC_ERROR_DEVICE_QUEUE_FULL;
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return ret;
}

/*======================================================================

  FUNCTION        Adc_ReadChannel

  DESCRIPTION     Reads an ADC channel.

  DEPENDENCIES    None

  PARAMETERS      pDevCtxt     [in]  device context
                  uDeviceIdx   [in]  device index
                  uChannelIdx  [in]  channel index
                  hClientEvent [in]  event to complete
                  pAdcResult   [out] conversion result

  RETURN VALUE    DAL_SUCCESS or an error.

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_ReadChannel(
   AdcDevCtxt *pDevCtxt,
   uint32 uDeviceIdx,
   uint32 uChannelIdx,
   const DALSYSEventHandle hClientEvent,
   AdcResultType *pAdcResult
   )
{
   DALResult result;
   AdcDeviceResultType adcDeviceReadResult;
   AdcDeviceDataType *pDeviceData;

   DALSYS_memset(pAdcResult, 0, sizeof(AdcResultType));
   pAdcResult->eStatus = ADC_RESULT_INVALID;
   pAdcResult->nDeviceIdx = uDeviceIdx;
   pAdcResult->nChannelIdx = uChannelIdx;

   pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

   DalDevice_Open(pDeviceData->phDevice, DAL_OPEN_EXCLUSIVE);

   result = DalAdcDevice_ReadChannel(pDeviceData->phDevice,
                                     uChannelIdx,
                                     &adcDeviceReadResult);

   DalDevice_Close(pDeviceData->phDevice);

   if (result != DAL_SUCCESS)
   {
      return result;
   }

   if (adcDeviceReadResult.eStatus == ADC_DEVICE_RESULT_VALID)
   {
      pAdcResult->eStatus = ADC_RESULT_VALID;
   }
   else if (adcDeviceReadResult.eStatus == ADC_DEVICE_RESULT_TIMEOUT)
   {
      pAdcResult->eStatus = ADC_RESULT_TIMEOUT;
   }
   else if (adcDeviceReadResult.eStatus == ADC_DEVICE_RESULT_FIFO_NOT_EMPTY)
   {
      pAdcResult->eStatus = ADC_RESULT_FIFO_NOT_EMPTY;
   }
   else if (adcDeviceReadResult.eStatus == ADC_DEVICE_RESULT_STALE)
   {
      pAdcResult->eStatus = ADC_RESULT_STALE;
   }
   else
   {
      pAdcResult->eStatus = ADC_RESULT_INVALID;
   }

   pAdcResult->nPhysical = adcDeviceReadResult.nPhysical;
   pAdcResult->nPercent = adcDeviceReadResult.uPercent;
   pAdcResult->nMicrovolts = adcDeviceReadResult.uMicrovolts;
   pAdcResult->nCode = adcDeviceReadResult.uCode;

   Adc_LogConversionComplete(uDeviceIdx, uChannelIdx, hClientEvent);

   return DAL_SUCCESS;
}

/*======================================================================

  FUNCTION        Adc_RecalibrateChannel

  DESCRIPTION     Recalibrates an ADC channel.

  DEPENDENCIES    None

  PARAMETERS      pDevCtxt          [in]  device context
                  uDeviceIdx        [in]  device index
                  uChannelIdx       [in]  channel index
                  hClientEvent      [in]  event to complete
                  pAdcRecalResult   [out] recalibration result

  RETURN VALUE    DAL_SUCCESS or an error.

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_RecalibrateChannel(
   AdcDevCtxt *pDevCtxt,
   uint32 uDeviceIdx,
   uint32 uChannelIdx,
   const DALSYSEventHandle hClientEvent,
   AdcRecalibrationResultType *pAdcRecalResult
   )
{
   DALResult result;
   AdcDeviceRecalibrateResultType adcDeviceRecalResult;
   AdcDeviceDataType *pDeviceData;

   DALSYS_memset(pAdcRecalResult, 0, sizeof(AdcRecalibrationResultType));
   pAdcRecalResult->eStatus = ADC_RESULT_INVALID;
   pAdcRecalResult->uDeviceIdx = uDeviceIdx;
   pAdcRecalResult->uChannelIdx = uChannelIdx;

   pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

   DalDevice_Open(pDeviceData->phDevice, DAL_OPEN_EXCLUSIVE);

   result = DalAdcDevice_RecalibrateChannel(pDeviceData->phDevice,
                                            uChannelIdx,
                                            &adcDeviceRecalResult);

   DalDevice_Close(pDeviceData->phDevice);

   if (result != DAL_SUCCESS)
   {
      return result;
   }

   if (adcDeviceRecalResult.eStatus == ADC_DEVICE_RESULT_VALID)
   {
      pAdcRecalResult->eStatus = ADC_RESULT_VALID;
   }
   else
   {
      pAdcRecalResult->eStatus = ADC_RESULT_INVALID;
   }

   pAdcRecalResult->nPhysical1_uV = adcDeviceRecalResult.nPhysical1_uV;
   pAdcRecalResult->nPhysical2_uV = adcDeviceRecalResult.nPhysical2_uV;
   pAdcRecalResult->uCode1 = adcDeviceRecalResult.uCode1;
   pAdcRecalResult->uCode2 = adcDeviceRecalResult.uCode2;

   Adc_LogCalibrationComplete(uDeviceIdx, uChannelIdx, hClientEvent);

   return DAL_SUCCESS;
}

/*======================================================================

  FUNCTION        Adc_PerformOperationInWorkLoop

  DESCRIPTION     This function processes the request that is pending.

  DEPENDENCIES    None

  PARAMETERS      hEvent     [in]
                  pInDevCtxt [in] pointer to the ADC device context

  RETURN VALUE    DAL_SUCCESS or an error.

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_PerformOperationInWorkLoop(
   DALSYSEventHandle hEvent,
   void *pInDevCtxt
   )
{
   AdcQueueType *pQueue = pInDevCtxt;
   DALResult result;
   AdcDevCtxt *pDevCtxt = pQueue->pAdcDevCtxt;
   AdcDeviceQueueNodeType *pNode;
   AdcResultType adcResult;
   AdcRecalibrationResultType adcRecalResult;
   void *pPayload;
   uint32 uPayloadSize;

   /* Get the pending request to be serviced */
   pNode = (AdcDeviceQueueNodeType *)DALFW_Q_Check(&pQueue->qPending);
   if (pNode == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC work loop called with an queue empty");
      return DAL_ERROR;
   }

   do
   {
      /* Acquire the queue lock inside the loop so it is released after
         each ADC operation. Otherwise, if the work loop becomes non-empty
         it may starve the blocking reads trying to acquire the lock */
      DALSYS_SyncEnter(pQueue->hSync);

      switch (pNode->eAdcDeviceRequest)
      {
         case ADC_REQUEST_CONVERSION:

            result = Adc_ReadChannel(pDevCtxt,
                                     pNode->uDeviceIdx,
                                     pNode->uChannelIdx,
                                     pNode->hClientEvent,
                                     &adcResult);
            if (result != DAL_SUCCESS)
            {
               adcResult.eStatus = ADC_RESULT_INVALID;
            }

            pPayload = &adcResult;
            uPayloadSize = sizeof(adcResult);

            break;

         case ADC_REQUEST_CHANNEL_RECALIBRATION:

            result = Adc_RecalibrateChannel(pDevCtxt,
                                            pNode->uDeviceIdx,
                                            pNode->uChannelIdx,
                                            pNode->hClientEvent,
                                            &adcRecalResult);
            if (result != DAL_SUCCESS)
            {
               adcRecalResult.eStatus = ADC_RESULT_INVALID;
            }

            pPayload = &adcRecalResult;
            uPayloadSize = sizeof(adcRecalResult);

            break;

         default:
            pPayload = NULL;
            uPayloadSize = 0;

            break;
      }

      DALSYS_SyncLeave(pQueue->hSync);

      if (pNode->hClientEvent != NULL)
      {
         /* notify the client that the operation is complete */
         /* note: clients can use NULL for recalibration hence the check */
         (void)DALSYS_EventCtrlEx(pNode->hClientEvent,
                                  DALSYS_EVENT_CTRL_TRIGGER,
                                  0,
                                  pPayload,
                                  uPayloadSize);
      }

      DALSYS_SyncEnter(pDevCtxt->hSync);

      /* Remove node from pending queue and place it back into the free list */
      pNode->adcRequestStatus.eStatus = ADC_REQUEST_STATUS_UNKNOWN;
      DALFW_Q_Delete(&pQueue->qPending, (DALFW_Q_LinkType *)pNode);
      DALFW_Q_Put(&pQueue->qFree, (DALFW_Q_LinkType *)pNode);

      /* Ready to process the next node */
      pNode = (AdcDeviceQueueNodeType *)DALFW_Q_Check(&pQueue->qQueued);
      if (pNode != NULL)
      {
         /* Delete the node from queued */
         DALFW_Q_Delete(&pQueue->qQueued, (DALFW_Q_LinkType*)pNode);

         /* Place the node in pending */
         DALFW_Q_Put(&pQueue->qPending, (DALFW_Q_LinkType *)pNode);

         pNode->adcRequestStatus.eStatus = ADC_REQUEST_STATUS_PENDING;
      }

      DALSYS_SyncLeave(pDevCtxt->hSync);
   } while (pNode != NULL);

   return DAL_SUCCESS;
}

/*======================================================================

  FUNCTION        Adc_TMGetClientIdx

  DESCRIPTION     Gets the client index from the client ID. If the
                  client is not registered then this function will
                  attempt to register the client and assign an index.

  DEPENDENCIES    Callers need to have the device lock acquired.

  PARAMETERS
     pDevCtxt    [in]  device context
     uClientId   [in]  client ID
     puClientIdx [out] client index

  RETURN VALUE
     DAL_SUCCESS: success
     ADC_ERROR_OUT_OF_TM_CLIENTS: out of client handles

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_TMGetClientIdx(
   AdcDevCtxt *pDevCtxt,
   uint32 uClientId,
   uint32 *puClientIdx)
{
   AdcMeasType *pMeas;
   uint32 uClientIdx;
   uint32 uMeas;
   uint32 uThresh;
   uint32 uDeviceIdx;

   /* First check if the client is registered */
   for (uClientIdx = 0; uClientIdx < ADC_MAX_NUM_CLIENTS; uClientIdx++)
   {
      if (pDevCtxt->aClients[uClientIdx].bRegistered == TRUE &&
          pDevCtxt->aClients[uClientIdx].uClientId == uClientId)
      {
         /* The client has been found */
         *puClientIdx = uClientIdx;

         return DAL_SUCCESS;
      }
   }

   /* Client needs to be registered */
   for (uClientIdx = 0; uClientIdx < ADC_MAX_NUM_CLIENTS; uClientIdx++)
   {
      if (pDevCtxt->aClients[uClientIdx].bRegistered == FALSE)
      {
         /* Register the client here */
         pDevCtxt->aClients[uClientIdx].bRegistered = TRUE;
         pDevCtxt->aClients[uClientIdx].uClientId = uClientId;
         pDevCtxt->aClients[uClientIdx].bEnabled = TRUE;

         for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
         {
            for (uMeas = 0; uMeas < pDevCtxt->paDeviceData[uDeviceIdx].uNumMeas; uMeas++)
            {
               pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uMeas];

               for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
               {
                  pMeas->aClientThresholds[uClientIdx][uThresh].eThresholdState = ADC_THRESHOLD_STATE_DISABLED;
               }
            }
         }

         *puClientIdx = uClientIdx;

         return DAL_SUCCESS;
      }
   }

   return ADC_ERROR_OUT_OF_TM_CLIENTS;
}

/*======================================================================

  FUNCTION        Adc_TMAggregateAndRearmThresh

  DESCRIPTION     Aggregates thresholds for a particular measurement
                  threshold and notifies the device of the new
                  threshold.

  DEPENDENCIES    Callers need to have the device lock acquired.

  PARAMETERS
     pDevCtxt   [in] device context
     uDeviceIdx [in] which device
     uMeasIdx   [in] which meas
     eThreshold [in] which threshold
     pnThresholdSet [out] threshold value that was set

  RETURN VALUE
     DAL_SUCCESS: success

  SIDE EFFECTS    None

======================================================================*/
static DALResult
Adc_TMAggregateAndRearmThresh(
   AdcDevCtxt *pDevCtxt,
   uint32 uDeviceIdx,
   uint32 uMeasIdx,
   AdcTMThresholdType eThreshold,
   int32 *pnThresholdSet
   )
{
   AdcDeviceTMThresholdType eDevThreshold;
   AdcThresholdConfigType *pThreshold;
   AdcMeasType *pMeas;
   DALBOOL bReArm = FALSE;
   uint32 uClientIdx;
   int32 nAggrThreshold = 0;
   int32 *pnAggrThreshold;
   DALResult result;

   pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uMeasIdx];

   for (uClientIdx = 0; uClientIdx < ADC_MAX_NUM_CLIENTS; uClientIdx++)
   {
      if (pDevCtxt->aClients[uClientIdx].bRegistered == TRUE &&
          pDevCtxt->aClients[uClientIdx].bEnabled == TRUE)
      {
         pThreshold = &pMeas->aClientThresholds[uClientIdx][eThreshold];

         if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD ||
             pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
         {
            if (bReArm == FALSE)
            {
               nAggrThreshold = pThreshold->nThreshold;
               bReArm = TRUE;
            }
            else
            {
               if (eThreshold == ADC_TM_THRESHOLD_LOWER)
               {
                  if (pThreshold->nThreshold > nAggrThreshold)
                  {
                     nAggrThreshold = pThreshold->nThreshold;
                  }
               }
               else
               {
                  if (pThreshold->nThreshold < nAggrThreshold)
                  {
                     nAggrThreshold = pThreshold->nThreshold;
                  }
               }
            }
         }
      }
   }

   if (bReArm == TRUE)
   {
      pnAggrThreshold = &nAggrThreshold;
   }
   else
   {
      pnAggrThreshold = NULL;
   }

   if (eThreshold == ADC_TM_THRESHOLD_LOWER)
   {
      eDevThreshold = ADC_DEVICE_TM_THRESHOLD_LOWER;
   }
   else
   {
      eDevThreshold = ADC_DEVICE_TM_THRESHOLD_HIGHER;
   }

   result = DalAdcDevice_TMSetThreshold(pDevCtxt->paDeviceData[uDeviceIdx].phDevice,
                                        uMeasIdx,
                                        pDevCtxt->hThresholdCallback,
                                        eDevThreshold,
                                        pnAggrThreshold,
                                        pnThresholdSet);

   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_ERROR,
                      "Failed to set a threshold");
   }

   switch (result)
   {
      case ADC_DEVICE_ERROR_OUT_OF_TM_CLIENTS:
         result = ADC_ERROR_OUT_OF_TM_CLIENTS;
         break;
      case ADC_DEVICE_ERROR_TM_NOT_SUPPORTED:
         result = ADC_ERROR_TM_NOT_SUPPORTED;
         break;
      case ADC_DEVICE_ERROR_TM_THRESHOLD_OUT_OF_RANGE:
         result = ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
         break;
      case ADC_DEVICE_ERROR_TM_INVALID_MEAS_IDX:
         result = ADC_ERROR_INVALID_CHANNEL_IDX;
         break;
      case ADC_DEVICE_ERROR_TM_BUSY:
         result = ADC_ERROR_TM_BUSY;
         break;
      default:
         break;
   }

   return result;
}

/*======================================================================

  FUNCTION        Adc_TMThresholdCb

  DESCRIPTION     DAL callback for threshold events.

  DEPENDENCIES    None.

  PARAMETERS
     pCtxt        [in]  callback context = device context
     dwParam      [in]  unused
     pPayload     [in]  pointer to AdcTMCallbackPayloadType
     nPayloadSize [in]  size of AdcTMCallbackPayloadType

  RETURN VALUE
     NULL

  SIDE EFFECTS    None

======================================================================*/
static void*
Adc_TMThresholdCb(
   void *pCtxt,
   uint32 dwParam,
   void *pPayload,
   uint32 nPayloadSize
   )
{
   AdcDevCtxt *pDevCtxt = pCtxt;
   AdcTMCallbackPayloadType *pAdcTMPayload = pPayload;
   uint32 uDeviceIdx;
   uint32 uMeasIdx;
   uint32 uClientIdx;
   AdcMeasType *pMeas;
   AdcThresholdConfigType *pThreshold;
   DALBOOL bTrigger;
   AdcTMThresholdType eThreshold;
   DALResult result;
   DALBOOL bUpdateLower = FALSE;
   DALBOOL bUpdateHigher = FALSE;
   int32 nTolThresh;
   int32 nThresholdSet;

   uDeviceIdx = pAdcTMPayload->adcTMInputProps.uDeviceIdx;
   uMeasIdx = pAdcTMPayload->adcTMInputProps.uChannelIdx;
   eThreshold = pAdcTMPayload->eThresholdTriggered;
   pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uMeasIdx];

   if (eThreshold == ADC_TM_THRESHOLD_LOWER)
   {
      bUpdateLower = TRUE;
   }
   else
   {
      bUpdateHigher = TRUE;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   for (uClientIdx = 0; uClientIdx < ADC_MAX_NUM_CLIENTS; uClientIdx++)
   {
      bTrigger = FALSE;

      if (pDevCtxt->aClients[uClientIdx].bRegistered == TRUE &&
          pDevCtxt->aClients[uClientIdx].bEnabled == TRUE)
      {
         pThreshold = &pMeas->aClientThresholds[uClientIdx][eThreshold];

         if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD ||
             pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
         {
            if (eThreshold == ADC_TM_THRESHOLD_LOWER)
            {
               if (pAdcTMPayload->nPhysicalTriggered <= pThreshold->nThreshold)
               {
                  bTrigger = TRUE;
               }
            }
            else
            {
               if (pAdcTMPayload->nPhysicalTriggered >= pThreshold->nThreshold)
               {
                  bTrigger = TRUE;
               }
            }
         }

         if (bTrigger == TRUE)
         {
            if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD)
            {
               pThreshold->eThresholdState = ADC_THRESHOLD_STATE_TRIGGERED;
            }
            else
            {
               /* Update tolerances */
               pThreshold = &pMeas->aClientThresholds[uClientIdx][ADC_TM_THRESHOLD_LOWER];
               if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
               {
                  nTolThresh = pAdcTMPayload->nPhysicalTriggered - pThreshold->nTolerance;

                  if (nTolThresh < pMeas->range.nPhysicalMin)
                  {
                     nTolThresh = pMeas->range.nPhysicalMin;
                  }
                  else if (nTolThresh > pMeas->range.nPhysicalMax)
                  {
                     nTolThresh = pMeas->range.nPhysicalMax;
                  }

                  pThreshold->nThreshold = nTolThresh;
                  pThreshold->nCurrentValue = pAdcTMPayload->nPhysicalTriggered;
                  bUpdateLower = TRUE;
               }

               pThreshold = &pMeas->aClientThresholds[uClientIdx][ADC_TM_THRESHOLD_HIGHER];
               if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
               {
                  nTolThresh = pAdcTMPayload->nPhysicalTriggered + pThreshold->nTolerance;

                  if (nTolThresh < pMeas->range.nPhysicalMin)
                  {
                     nTolThresh = pMeas->range.nPhysicalMin;
                  }
                  else if (nTolThresh > pMeas->range.nPhysicalMax)
                  {
                     nTolThresh = pMeas->range.nPhysicalMax;
                  }

                  pThreshold->nThreshold = nTolThresh;
                  pThreshold->nCurrentValue = pAdcTMPayload->nPhysicalTriggered;
                  bUpdateHigher = TRUE;
               }
            }

            /* Release lock */
            DALSYS_SyncLeave(pDevCtxt->hSync);

            /* Trigger the event */
            pThreshold = &pMeas->aClientThresholds[uClientIdx][eThreshold];
            DALSYS_EventCtrlEx(pThreshold->hEvent,
                               DALSYS_EVENT_CTRL_TRIGGER,
                               NULL,
                               (void *)pAdcTMPayload,
                               sizeof(*pAdcTMPayload));

            /* Acquire lock */
            DALSYS_SyncEnter(pDevCtxt->hSync);
         }
      }
   }

   if (bUpdateLower == TRUE)
   {
      result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                             uDeviceIdx,
                                             uMeasIdx,
                                             ADC_TM_THRESHOLD_LOWER,
                                             &nThresholdSet);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_ERROR,
                         "Failed to rearm thresholds");
      }
   }

   if (bUpdateHigher == TRUE)
   {
      result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                             uDeviceIdx,
                                             uMeasIdx,
                                             ADC_TM_THRESHOLD_HIGHER,
                                             &nThresholdSet);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_ERROR,
                         "Failed to rearm thresholds");
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return NULL;
}

/*------------------------------------------------------------------------------
Following functions are for DALDriver specific functionality
------------------------------------------------------------------------------*/
DALResult
Adc_DriverInit(AdcDrvCtxt *pCtxt)
{
   uint32 uDevice;

   for (uDevice = 0; uDevice < ADC_MAX_NUM_DEVICES; uDevice++)
   {
      pCtxt->AdcDevCtxt[uDevice].bInit = FALSE;
   }

   return DAL_SUCCESS;
}

DALResult
Adc_DriverDeInit(AdcDrvCtxt *pCtxt)
{
   return DAL_SUCCESS;
}

/*------------------------------------------------------------------------------
Following functions are declared in DalDevice Interface.
------------------------------------------------------------------------------*/
DALResult
Adc_DeviceInit(AdcClientCtxt *pCtxt)
{
   uint32 uDeviceIdx;
   uint32 uNodeIdx, uQueueSize;
   uint32 uQueueIdx;
   DALResult result = DAL_SUCCESS;
   DALSYSPropertyVar propVar;
   AdcDeviceDevicePropertiesType adcDeviceProp;
   AdcDeviceDataType *pDeviceData;
   AdcQueueType *pQueue;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   uint32 uClient;
   uint32 uMeas;
   uint32 uThresh;
   AdcMeasType *pMeas;
   AdcDeviceTMRangeType adcTMDeviceRange;
   uint8 pmic_index = 0;

   /* Initialize device context */
   pDevCtxt->hSync = NULL;
   pDevCtxt->paDeviceData = NULL;
   pDevCtxt->paQueues = NULL;

   /* Prevent ADC from unloading */
   DALFW_MarkDeviceStatic((DALDevCtxt *)pDevCtxt);

   /* Initialize the log */
   gAdcDebug.uCurIdx = 0;

   result = DalTimetick_Attach("SystemTimer", &gAdcDebug.phTimetickDev);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "VADC: Failed to attach to Timetick device");
      return result;
   }

   /* Read the DAL properties */
   result = DALSYS_GetDALPropertyHandle(pDevCtxt->DevId, pDevCtxt->hProp);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Could not open ADC DAL properties");
      return result;
   }

   if( (pm_is_pmic_supported(PMIC_IS_PMI632, &pmic_index) == PM_ERR_FLAG_SUCCESS) ||
       (pm_is_pmic_supported(PMIC_IS_TIOMAN, &pmic_index) == PM_ERR_FLAG_SUCCESS)   )
   
   {
      result = DALSYS_GetPropertyValue(pDevCtxt->hProp, "ADC_BSP", 0, &propVar);
   }
   else 
   {
      result = DALSYS_GetPropertyValue(pDevCtxt->hProp, "ADC_BSP_NOPMI", 0, &propVar);
   }
  
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "BSP not found in ADC DAL properties");
      return result;
   }

   pDevCtxt->pBsp = (AdcBspType *)propVar.Val.pStruct;

   /* Initialize synchronization objects */
   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &ghAdcDebugSync,
                              &ghAdcDebugSyncObject);

   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not create log synchronization object");
      return result;
   }

   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &pDevCtxt->hSync,
                              &pDevCtxt->syncObject);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not create device synchronization object");
      return result;
   }

   /* Allocate memory for the queues */
   result = DALSYS_Malloc((sizeof(AdcQueueType) * pDevCtxt->pBsp->uNumQueues),
                          (void **)&pDevCtxt->paQueues);

   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not allocate memory for device queues");
      return result;
   }

   DALSYS_memset(pDevCtxt->paQueues,
                 0,
                 (sizeof(AdcQueueType) * pDevCtxt->pBsp->uNumQueues));

   /* Initialize the queues */
   for (uQueueIdx = 0; uQueueIdx < pDevCtxt->pBsp->uNumQueues; uQueueIdx++)
   {
      pQueue = &pDevCtxt->paQueues[uQueueIdx];

      /* initialize the queue */
      pQueue->pAdcDevCtxt = pDevCtxt;
      (void)DALFW_Q_Init(&pQueue->qFree);
      (void)DALFW_Q_Init(&pQueue->qQueued);
      (void)DALFW_Q_Init(&pQueue->qPending);

      /* allocate the queue nodes */
      uQueueSize = pDevCtxt->pBsp->paAdcQueueConfigs[uQueueIdx].uQueueSize;
      pQueue->uQueueSize = uQueueSize;

      result = DALSYS_Malloc((sizeof(AdcDeviceQueueNodeType) * uQueueSize),
                             (void **)&pQueue->paNodes);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Adc could not allocate memory for device queue nodes");
         return result;
      }

      for (uNodeIdx = 0; uNodeIdx < uQueueSize; uNodeIdx++)
      {
         /* Initialize node data */
         DALSYS_memset(&pQueue->paNodes[uNodeIdx],
                       0,
                       sizeof(AdcDeviceQueueNodeType));

         pQueue->paNodes[uNodeIdx].adcRequestStatus.eStatus = ADC_REQUEST_STATUS_UNKNOWN;

         DALFW_Q_Put(&pQueue->qFree,
                     (DALFW_Q_LinkType *)&pQueue->paNodes[uNodeIdx]);
      }

      /* Create the work-loop */
      result = DALSYS_RegisterWorkLoopEx(pDevCtxt->pBsp->paAdcQueueConfigs[uQueueIdx].pszWorkLoopName,
                                         ADC_DEFAULT_STACK_SIZE,
                                         gAdcWorkloopPriority,
                                         10,
                                         &pQueue->hAdcOpWorkLoop,
                                         NULL);
      if (result != DAL_SUCCESS)
      {
         return result;
      }

      /* Create a workloop event for performing ADC functions */
      result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT,
                                  &pQueue->hAdcOpEvent,
                                  NULL);
      if (result != DAL_SUCCESS)
      {
         return result;
      }

      /* Add the event to the workloop */
      result = DALSYS_AddEventToWorkLoop(pQueue->hAdcOpWorkLoop,
                                         Adc_PerformOperationInWorkLoop,
                                         pQueue,
                                         pQueue->hAdcOpEvent,
                                         NULL);
      if (result != DAL_SUCCESS)
      {
         return result;
      }

      /* Create a synchronization event to be used between work loop and blocking reads */
      result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                                 &pQueue->hSync,
                                 &pQueue->syncObject);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Adc could not create work loop synchronization object");
         return result;
      }
   }

   /* Allocate memory for the device data */
   result = DALSYS_Malloc(sizeof(AdcDeviceDataType) * pDevCtxt->pBsp->uNumPhysicalDevices,
                          (void **)&pDevCtxt->paDeviceData);

   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not allocate memory for device data");
      return result;
   }

   /* Set up the threshold clients */
   for (uClient = 0; uClient < ADC_MAX_NUM_CLIENTS; uClient++)
   {
      pDevCtxt->aClients[uClient].bRegistered = FALSE;
   }

   /* Create a callback event to process threshold completions */
   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CALLBACK_EVENT,
                               &pDevCtxt->hThresholdCallback,
                               &pDevCtxt->thresholdCbOjb);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not create callback event");
      return result;
   }

   result = DALSYS_SetupCallbackEvent(pDevCtxt->hThresholdCallback,
                                      Adc_TMThresholdCb,
                                      pDevCtxt);
   if (result != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Adc could not setup callback");
      return result;
   }

   /*
    * Initialize the devices
    */
   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      /* Get the pointer to the device data corresponding to this device */
      pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

      /* Initialize all of device data to 0 */
      DALSYS_memset(pDeviceData, 0, sizeof(AdcDeviceDataType));

      /* Flag to indicate device hardware resouraces are OFF. */
      pDeviceData->bDeviceIsOpen = FALSE;

      /*
       * Attach to the ADC device. The attach can fail when the device is
       * not physically present. In this case, we just try and attach to the
       * next available device.
       */
      result = DAL_AdcDeviceDeviceAttach(pDevCtxt->pBsp->paAdcPhysicalDevices[uDeviceIdx].pszDevName,
                                         &pDeviceData->phDevice);

      if (result == DAL_SUCCESS && pDeviceData->phDevice != NULL)
      {
         pDeviceData->deviceStatus = ADC_DEVICE_AVAILABLE;
      }
      else
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_ERROR,
                         "Adc failed attaching to a physical device");

         pDeviceData->deviceStatus = ADC_DEVICE_NOT_AVAILABLE;
      }

      if (pDeviceData->phDevice == NULL)
      {
         continue;
      }

      result = DalAdcDevice_SetDeviceIndex(pDeviceData->phDevice,
                                           uDeviceIdx);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Error setting device index");
         return result;
      }

      /* Get the number of channels for each device */
      result = DalAdcDevice_GetDeviceProperties(pDeviceData->phDevice,
                                                &adcDeviceProp);
      if (result != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Error obtaining the number of ADC device channels");
         return result;
      }

      pDeviceData->uNumChannels = adcDeviceProp.uNumChannels;
      pDeviceData->uNumMeas = adcDeviceProp.uNumMeas;

      /* Configure TM measurements */
      if (pDeviceData->uNumMeas == 0)
      {
         pDeviceData->paMeas = NULL;
      }
      else
      {
         result = DALSYS_Malloc(sizeof(AdcMeasType) * pDeviceData->uNumMeas,
                                (void **)&pDeviceData->paMeas);
         if (result != DAL_SUCCESS)
         {
            DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                            "Adc could not allocate memory for meas");
            return result;
         }

         DALSYS_memset(pDeviceData->paMeas, 0, sizeof(AdcMeasType) * pDeviceData->uNumMeas);

         for (uMeas = 0; uMeas < pDeviceData->uNumMeas; uMeas++)
         {
            pMeas = &pDeviceData->paMeas[uMeas];

            result = DalAdcDevice_TMGetRange(pDeviceData->phDevice, uMeas, &adcTMDeviceRange);
            if (result != DAL_SUCCESS)
            {
               DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                               "Failed to get TM range");
               return result;
            }

            pMeas->range.nPhysicalMin = adcTMDeviceRange.nPhysicalMin;
            pMeas->range.nPhysicalMax = adcTMDeviceRange.nPhysicalMax;

            for (uClient = 0; uClient < ADC_MAX_NUM_CLIENTS; uClient++)
            {
               for (uThresh = 0; uThresh < _ADC_TM_NUM_THRESHOLDS; uThresh++)
               {
                  pMeas->aClientThresholds[uClient][uThresh].eThresholdState = ADC_THRESHOLD_STATE_DISABLED;
               }
            }
         }
      }
   }

   if (result == DAL_SUCCESS)
   {
      pDevCtxt->bInit = TRUE;
   }

   return result;
}

DALResult
Adc_DeviceDeInit(AdcClientCtxt *pCtxt)
{
   uint32 uDeviceIdx;
   uint32 uQueueIdx;
   DALResult tmpResult;
   DALResult retResult = DAL_SUCCESS;
   AdcQueueType *pQueue;
   AdcDevCtxt * pDevCtxt = pCtxt->pAdcDevCtxt;

   /* Detach from the ADC devices */
   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      if (pDevCtxt->paDeviceData[uDeviceIdx].phDevice != NULL)
      {
         tmpResult = DAL_DeviceDetach(pDevCtxt->paDeviceData[uDeviceIdx].phDevice);
         if (tmpResult != DAL_SUCCESS)
         {
            retResult = DAL_ERROR;
         }
      }
   }

   /* Free device data array */
   if (pDevCtxt->paDeviceData != NULL)
   {
      tmpResult = DALSYS_Free(pDevCtxt->paDeviceData);
      if (tmpResult != DAL_SUCCESS)
      {
         retResult = DAL_ERROR;
      }
      pDevCtxt->paDeviceData = NULL;
   }

   /* Destroy synchronization objects */
   if (pDevCtxt->hSync != NULL)
   {
      tmpResult = DALSYS_DestroyObject(pDevCtxt->hSync);
      if (tmpResult != DAL_SUCCESS)
      {
         retResult = DAL_ERROR;
      }
      pDevCtxt->hSync = NULL;
   }

   if (ghAdcDebugSync != NULL)
   {
      tmpResult = DALSYS_DestroyObject(ghAdcDebugSync);
      if (tmpResult != DAL_SUCCESS)
      {
         retResult = DAL_ERROR;
      }
      ghAdcDebugSync = NULL;
   }

   /* Free the queues */
   if (pDevCtxt->paQueues != NULL)
   {
      for (uQueueIdx = 0; uQueueIdx < pDevCtxt->pBsp->uNumQueues; uQueueIdx++)
      {
         pQueue = &pDevCtxt->paQueues[uQueueIdx];

         if (pQueue->hSync != NULL)
         {
            tmpResult = DALSYS_DestroyObject(pQueue->hSync);
            if (tmpResult != DAL_SUCCESS)
            {
               retResult = DAL_ERROR;
            }
            pQueue->hSync = NULL;
         }

         if (pQueue->paNodes != NULL)
         {
            tmpResult = DALSYS_Free(pQueue->paNodes);
            if (tmpResult != DAL_SUCCESS)
            {
               retResult = DAL_ERROR;
            }
            pQueue->paNodes = NULL;
         }

         if (pQueue->hAdcOpEvent != NULL)
         {
            if (pQueue->hAdcOpWorkLoop != NULL)
            {
               tmpResult = DALSYS_DeleteEventFromWorkLoop(pQueue->hAdcOpWorkLoop,
                                                          pQueue->hAdcOpEvent);
               if (tmpResult != DAL_SUCCESS)
               {
                  retResult = DAL_ERROR;
               }
            }

            tmpResult = DALSYS_DestroyObject(pQueue->hAdcOpEvent);
            if (tmpResult != DAL_SUCCESS)
            {
               retResult = DAL_ERROR;
            }
            pQueue->hAdcOpEvent = NULL;
         }
      }

      tmpResult = DALSYS_Free(pDevCtxt->paQueues);
      if (tmpResult != DAL_SUCCESS)
      {
         retResult = DAL_ERROR;
      }
      pDevCtxt->paQueues = NULL;
   }

   return retResult;
}

DALResult
Adc_PowerEvent(
   AdcClientCtxt *pCtxt,
   DalPowerCmd PowerCmd,
   DalPowerDomain PowerDomain
   )
{
   return DAL_SUCCESS;
}

DALResult
Adc_Open(
   AdcClientCtxt *pCtxt,
   uint32 dwaccessMode
   )
{
   return DAL_SUCCESS;
}

DALResult
Adc_Close(AdcClientCtxt *pCtxt)
{
   return DAL_SUCCESS;
}

DALResult
Adc_Info(
   AdcClientCtxt *pCtxt,
   DalDeviceInfo *pdeviceInfo,
   uint32 dwSize
   )
{
   return DAL_SUCCESS;
}

/*------------------------------------------------------------------------------
Following functions are extended in DalAdc Interface.

These functions are documented in DDIAdc.h
------------------------------------------------------------------------------*/

DALResult
Adc_GetAdcInputProperties(
   AdcClientCtxt *pCtxt,
   const char *pInputName,
   uint32 nInputNameSize,
   AdcInputPropertiesType *pAdcInputProps
   )
{
   uint32 uDeviceIdx, uChannelIdx;
   AdcDeviceDataType *pDeviceData;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;

   if (pAdcInputProps == NULL)
   {
      return DAL_ERROR;
   }

   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      /* Get the pointer to the device data corresponding to this device index */
      pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

      if (pDeviceData->deviceStatus == ADC_DEVICE_NOT_AVAILABLE || pDeviceData->phDevice == NULL)
      {
         continue;
      }

      if (DalAdcDevice_GetInputProperties(pDeviceData->phDevice, pInputName, &uChannelIdx) == DAL_SUCCESS)
      {
         pAdcInputProps->nDeviceIdx  = uDeviceIdx;
         pAdcInputProps->nChannelIdx = uChannelIdx;
         return DAL_SUCCESS;
      }
   }

   return DAL_ERROR;
}

DALResult
Adc_Read(
   AdcClientCtxt *pCtxt,
   const AdcInputPropertiesType *pAdcInputProps,
   AdcResultType *pAdcResult
   )
{
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   DALResult result;
   uint32 uQueueIdx;
   AdcQueueType *pQueue;

   if (pAdcInputProps == NULL || pAdcResult == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc read requested with a NULL pointer parameter");
      return ADC_ERROR_NULL_POINTER;
   }
   else if (pAdcInputProps->nDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc read requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pDevCtxt->paDeviceData[pAdcInputProps->nDeviceIdx].deviceStatus ==
            ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pAdcInputProps->nChannelIdx >=
            pDevCtxt->paDeviceData[pAdcInputProps->nDeviceIdx].uNumChannels)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc read requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   Adc_LogConversionRequest(pAdcInputProps->nDeviceIdx, pAdcInputProps->nChannelIdx);

   uQueueIdx = pDevCtxt->pBsp->paAdcPhysicalDevices[pAdcInputProps->nDeviceIdx].uQueueIdx;
   pQueue = &pDevCtxt->paQueues[uQueueIdx];

   /* Acquiring this lock ensures this request doesn't conflict with the work loop */
   DALSYS_SyncEnter(pQueue->hSync);

   result = Adc_ReadChannel(pDevCtxt,
                            pAdcInputProps->nDeviceIdx,
                            pAdcInputProps->nChannelIdx,
                            NULL,
                            pAdcResult);

   DALSYS_SyncLeave(pQueue->hSync);

   return result;
}

DALResult
Adc_RequestConversion(
   AdcClientCtxt *pCtxt,
   const AdcRequestParametersType *pAdcParams,
   AdcRequestStatusType *pAdcRequestStatus
   )
{
   DALResult ret;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;

   /* Initialize request status to error */
   if (pAdcRequestStatus != NULL)
   {
      pAdcRequestStatus->eStatus = ADC_REQUEST_STATUS_ERROR;
   }

   if (pAdcParams == NULL || pAdcParams->hEvent == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc conversion requested with a NULL pointer parameter");
      return ADC_ERROR_NULL_POINTER;
   }
   else if (pAdcParams->nDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc conversion requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pDevCtxt->paDeviceData[pAdcParams->nDeviceIdx].deviceStatus ==
            ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pAdcParams->nChannelIdx >=
            pDevCtxt->paDeviceData[pAdcParams->nDeviceIdx].uNumChannels)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc conversion requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   /* Log the ADC conversion request */
   Adc_LogConversionRequest(pAdcParams->nDeviceIdx, pAdcParams->nChannelIdx);

   ret = Adc_AddDeviceQueueRequest(pCtxt,
                                   ADC_REQUEST_CONVERSION,
                                   pAdcParams->nDeviceIdx,
                                   pAdcParams->nChannelIdx,
                                   pAdcParams->hEvent,
                                   pAdcRequestStatus);

   return ret;
}

DALResult
Adc_RequestRecalibration(
   AdcClientCtxt *pCtxt,
   const AdcRequestParametersType *pAdcParams
   )
{
   DALResult ret;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;

   if (pAdcParams == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc recalibration requested with a NULL pointer for pAdcParams");
      return ADC_ERROR_NULL_POINTER;
   }
   else if (pAdcParams->nDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc calibration requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pDevCtxt->paDeviceData[pAdcParams->nDeviceIdx].deviceStatus ==
            ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC Device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }
   else if (pAdcParams->nChannelIdx >=
            pDevCtxt->paDeviceData[pAdcParams->nDeviceIdx].uNumChannels)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Adc conversion requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   /* Log the ADC conversion request */
   Adc_LogRecalibrationRequest(pAdcParams->nDeviceIdx, pAdcParams->nChannelIdx);

   ret = Adc_AddDeviceQueueRequest(pCtxt,
                                   ADC_REQUEST_CHANNEL_RECALIBRATION,
                                   pAdcParams->nDeviceIdx,
                                   pAdcParams->nChannelIdx,
                                   pAdcParams->hEvent,
                                   NULL);

   return ret;
}

DALResult
Adc_TMGetInputProperties(
   AdcClientCtxt *pCtxt,
   const char *pInputName,
   uint32 nInputNameSize,
   AdcTMInputPropertiesType *pAdcTMInputProps
   )
{
   uint32 uDeviceIdx, uMeasIdx;
   AdcDeviceDataType *pDeviceData;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;

   if (pAdcTMInputProps == NULL || pInputName == NULL)
   {
      return ADC_ERROR_NULL_POINTER;
   }

   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      /* Get the pointer to the device data corresponding to this device index */
      pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

      if (pDeviceData->deviceStatus == ADC_DEVICE_NOT_AVAILABLE || pDeviceData->phDevice == NULL)
      {
         continue;
      }

      if (DalAdcDevice_TMGetInputProperties(pDeviceData->phDevice, pInputName, &uMeasIdx) == DAL_SUCCESS)
      {
         pAdcTMInputProps->uDeviceIdx  = uDeviceIdx;
         pAdcTMInputProps->uChannelIdx = uMeasIdx;
         return DAL_SUCCESS;
      }
   }

   return DAL_ERROR;
}

DALResult
Adc_TMGetRange(
   AdcClientCtxt *pCtxt,
   const AdcTMInputPropertiesType *pAdcTMInputProps,
   AdcTMRangeType *pAdcTMRange
   )
{
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   AdcDeviceDataType *pDeviceData;
   uint32 uChannelIdx;

   if (pAdcTMInputProps == NULL || pAdcTMRange == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Get range requested with NULL param");
      return ADC_ERROR_NULL_POINTER;
   }

   if (pAdcTMInputProps->uDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Get range requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   pDeviceData = &pDevCtxt->paDeviceData[pAdcTMInputProps->uDeviceIdx];

   if (pDeviceData->deviceStatus == ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   uChannelIdx = pAdcTMInputProps->uChannelIdx;
   if (uChannelIdx >= pDeviceData->uNumMeas)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Get range requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   *pAdcTMRange = pDeviceData->paMeas[uChannelIdx].range;

   return DAL_SUCCESS;
}

DALResult
Adc_TMSetThreshold(
   AdcClientCtxt *pCtxt,
   const AdcTMRequestParametersType *pAdcTMParams,
   AdcTMThresholdType eThreshold,
   const int32 *pnThresholdDesired,
   int32 *pnThresholdSet
   )
{
   DALResult result;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   AdcDeviceDataType *pDeviceData;
   uint32 uDeviceIdx;
   uint32 uChannelIdx;
   uint32 uClientIdx;
   AdcMeasType *pMeas;
   uint32 uThresh;
   AdcThresholdConfigType *pThreshold;
   int32 nThresholdSet;

   if (pAdcTMParams == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set threshold requested with NULL param");
      return ADC_ERROR_NULL_POINTER;
   }

   /* If setting a threshold *pnThresholdSet gets set & we need an event */
   if (pnThresholdDesired != NULL && (pnThresholdSet == NULL || pAdcTMParams->hEvent == NULL))
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set threshold requested with NULL param");
      return ADC_ERROR_NULL_POINTER;
   }

   if (eThreshold != ADC_TM_THRESHOLD_LOWER && eThreshold != ADC_TM_THRESHOLD_HIGHER)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set threshold requested with invalid eThreshold");
      return ADC_ERROR_INVALID_PARAMETER;
   }

   uDeviceIdx = pAdcTMParams->adcTMInputProps.uDeviceIdx;
   if (uDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set threshold requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

   if (pDeviceData->deviceStatus == ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   uChannelIdx = pAdcTMParams->adcTMInputProps.uChannelIdx;
   if (uChannelIdx >= pDeviceData->uNumMeas)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set threshold requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uChannelIdx];

   /* Perform a range check */
   if (pnThresholdDesired != NULL)
   {
      if (*pnThresholdDesired > pMeas->range.nPhysicalMax ||
          *pnThresholdDesired < pMeas->range.nPhysicalMin)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                         "Set threshold requested with threshold out of range");
         return ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
      }
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   result = Adc_TMGetClientIdx(pDevCtxt,
                               pCtxt->uClientId,
                               &uClientIdx);
   if (result != DAL_SUCCESS)
   {
      DALSYS_SyncLeave(pDevCtxt->hSync);
      return result;
   }

   /* Make sure client has not registered a tolerance */
   for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
   {
      if (pMeas->aClientThresholds[uClientIdx][uThresh].eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
      {
         DALSYS_SyncLeave(pDevCtxt->hSync);
         return ADC_ERROR_TM_BUSY;
      }
   }

   /* Update the threshold and then aggregate */
   pThreshold = &pMeas->aClientThresholds[uClientIdx][eThreshold];

   if (pnThresholdDesired == NULL)
   {
      pThreshold->hEvent = NULL;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_DISABLED;
   }
   else
   {
      pThreshold->hEvent = pAdcTMParams->hEvent;
      pThreshold->nThreshold = *pnThresholdDesired;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_THRESHOLD;
   }

   if (pDevCtxt->aClients[uClientIdx].bEnabled == TRUE)
   {
      result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                             uDeviceIdx,
                                             uChannelIdx,
                                             eThreshold,
                                             &nThresholdSet);

      if (result == DAL_SUCCESS && pnThresholdDesired != NULL)
      {
         if (eThreshold == ADC_TM_THRESHOLD_LOWER && *pnThresholdDesired < nThresholdSet)
         {
            /* Another client's threshold was set when aggregated */
            *pnThresholdSet = *pnThresholdDesired;
         }
         else if (eThreshold == ADC_TM_THRESHOLD_HIGHER && *pnThresholdDesired > nThresholdSet)
         {
            /* Another client's threshold was set when aggregated */
            *pnThresholdSet = *pnThresholdDesired;
         }
         else
         {
            *pnThresholdSet = nThresholdSet;
         }
      }
   }
   else if (pnThresholdDesired != NULL)
   {
      *pnThresholdSet = *pnThresholdDesired;
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return result;
}

DALResult
Adc_TMSetEnableThresholds(
   AdcClientCtxt *pCtxt,
   DALBOOL bEnable
   )
{
   DALResult result;
   DALResult rtnResult = DAL_SUCCESS;
   uint32 uDeviceIdx;
   AdcDeviceDataType *pDeviceData;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   uint32 uClientIdx;
   uint32 uMeasIdx;
   uint32 uThresh;
   AdcMeasType *pMeas;
   AdcThresholdConfigType *pThreshold;
   int32 nThresholdSet;

   if (bEnable != TRUE && bEnable != FALSE)
   {
      return ADC_ERROR_INVALID_PARAMETER;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   result = Adc_TMGetClientIdx(pDevCtxt,
                               pCtxt->uClientId,
                               &uClientIdx);
   if (result != DAL_SUCCESS)
   {
      DALSYS_SyncLeave(pDevCtxt->hSync);
      return result;
   }

   if (bEnable == FALSE)
   {
      pDevCtxt->aClients[uClientIdx].bEnabled = FALSE;
   }
   else
   {
      pDevCtxt->aClients[uClientIdx].bEnabled = TRUE;
   }

   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

      if (pDeviceData->paMeas != NULL)
      {
         /* Only need to update thresholds if this client has one set */
         for (uMeasIdx = 0; uMeasIdx < pDeviceData->uNumMeas; uMeasIdx++)
         {
            pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uMeasIdx];

            for (uThresh = 0; uThresh < _ADC_TM_NUM_THRESHOLDS; uThresh++)
            {
               pThreshold = &pMeas->aClientThresholds[uClientIdx][uThresh];

               if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD ||
                   pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
               {
                  result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                                         uDeviceIdx,
                                                         uMeasIdx,
                                                         (AdcTMThresholdType)uThresh,
                                                         &nThresholdSet);
                  if (result != DAL_SUCCESS)
                  {
                     rtnResult = result;
                  }
               }
            }
         }
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return rtnResult;
}

DALResult
Adc_RegisterClient(AdcClientCtxt *pCtxt)
{
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   static uint32 uClientId = 0;

   if (pDevCtxt->bInit == FALSE)
   {
      /* DAL calls RegisterClient before calling DeviceInit, so this is a normal case.
       * As a result, do not try to acquire the lock since it hasn't been created */
      pCtxt->uClientId = uClientId;
      uClientId++;

      return DAL_SUCCESS;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   pCtxt->uClientId = uClientId;
   uClientId++;

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return DAL_SUCCESS;
}

DALResult
Adc_DeregisterClient(AdcClientCtxt *pCtxt)
{
   DALResult result;
   DALResult rtnResult = DAL_SUCCESS;
   uint32 uDeviceIdx;
   AdcDeviceDataType *pDeviceData;
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   uint32 uClientIdx;
   uint32 uMeasIdx;
   uint32 uThresh;
   AdcMeasType *pMeas;
   AdcThresholdConfigType *pThreshold;
   int32 nThresholdSet;

   DALSYS_SyncEnter(pDevCtxt->hSync);

   result = Adc_TMGetClientIdx(pDevCtxt,
                               pCtxt->uClientId,
                               &uClientIdx);
   if (result != DAL_SUCCESS)
   {
      /* Client was not registered */
      DALSYS_SyncLeave(pDevCtxt->hSync);
      return DAL_SUCCESS;
   }

   pDevCtxt->aClients[uClientIdx].bRegistered = FALSE;

   for (uDeviceIdx = 0; uDeviceIdx < pDevCtxt->pBsp->uNumPhysicalDevices; uDeviceIdx++)
   {
      pDeviceData = &pDevCtxt->paDeviceData[uDeviceIdx];

      if (pDeviceData->paMeas != NULL)
      {
         /* Only need to update thresholds if this client has one set */
         for (uMeasIdx = 0; uMeasIdx < pDeviceData->uNumMeas; uMeasIdx++)
         {
            pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uMeasIdx];

            for (uThresh = 0; uThresh < _ADC_TM_NUM_THRESHOLDS; uThresh++)
            {
               pThreshold = &pMeas->aClientThresholds[uClientIdx][uThresh];

               if (pThreshold->eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD ||
                   pThreshold->eThresholdState == ADC_THRESHOLD_STATE_TOLERANCE)
               {
                  result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                                         uDeviceIdx,
                                                         uMeasIdx,
                                                         (AdcTMThresholdType)uThresh,
                                                         &nThresholdSet);
                  if (result != DAL_SUCCESS)
                  {
                     rtnResult = result;
                  }
               }
            }
         }
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return rtnResult;
}

DALResult
Adc_TMSetTolerance(
   AdcClientCtxt *pCtxt,
   const AdcTMRequestParametersType *pAdcTMParams,
   int32 nPhysicalCurrent,
   const int32 *pnLowerTolerance,
   const int32 *pnHigherTolerance
   )
{
   AdcDevCtxt *pDevCtxt = pCtxt->pAdcDevCtxt;
   AdcDeviceDataType *pDeviceData;
   DALResult result;
   uint32 uDeviceIdx;
   uint32 uChannelIdx;
   uint32 uThresh;
   int32 nLowThreshold = 0;
   int32 nHighThreshold = 0;
   uint32 uClientIdx;
   AdcThresholdConfigType *pThreshold;
   AdcMeasType *pMeas;
   int32 nThresholdSet;

   if (pAdcTMParams == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set tolerance requested with NULL param");
      return ADC_ERROR_NULL_POINTER;
   }

   /* If setting a tolerance we need a valid event */
   if ((pnLowerTolerance != NULL || pnHigherTolerance != NULL) && pAdcTMParams->hEvent == NULL)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set tolerance requested with NULL event");
      return ADC_ERROR_NULL_POINTER;
   }

   uDeviceIdx = pAdcTMParams->adcTMInputProps.uDeviceIdx;
   if (uDeviceIdx >= pDevCtxt->pBsp->uNumPhysicalDevices)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set tolerance requested with invalid device index");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   pDeviceData = &pDevCtxt->paDeviceData[pAdcTMParams->adcTMInputProps.uDeviceIdx];

   if (pDeviceData->deviceStatus == ADC_DEVICE_NOT_AVAILABLE)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "ADC device not available.");
      return ADC_ERROR_INVALID_DEVICE_IDX;
   }

   uChannelIdx = pAdcTMParams->adcTMInputProps.uChannelIdx;
   if (uChannelIdx >= pDeviceData->uNumMeas)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                      "Set tolerance requested with invalid channel index");
      return ADC_ERROR_INVALID_CHANNEL_IDX;
   }

   pMeas = &pDevCtxt->paDeviceData[uDeviceIdx].paMeas[uChannelIdx];

   if (pnLowerTolerance != NULL)
   {
      if (*pnLowerTolerance <= 0)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                         "Set tolerance requested with invalid low tolerance");
         return ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
      }
      else
      {
         nLowThreshold = nPhysicalCurrent - *pnLowerTolerance;

         if (nLowThreshold > pMeas->range.nPhysicalMax ||
             nLowThreshold < pMeas->range.nPhysicalMin)
         {
            DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                            "Set tolerance requested with threshold out of range");
            return ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
         }
      }
   }

   if (pnHigherTolerance != NULL)
   {
      if (*pnHigherTolerance <= 0)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                         "Set tolerance requested with invalid high tolerance");
         return ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
      }
      else
      {
         nHighThreshold = nPhysicalCurrent + *pnHigherTolerance;

         if (nHighThreshold > pMeas->range.nPhysicalMax ||
             nHighThreshold < pMeas->range.nPhysicalMin)
         {
            DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_INFO,
                            "Set tolerance requested with threshold out of range");
            return ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
         }
      }
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   result = Adc_TMGetClientIdx(pDevCtxt,
                               pCtxt->uClientId,
                               &uClientIdx);
   if (result != DAL_SUCCESS)
   {
      DALSYS_SyncLeave(pDevCtxt->hSync);
      return result;
   }

   /* Make sure client has not registered a threshold */
   for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
   {
      if (pMeas->aClientThresholds[uClientIdx][uThresh].eThresholdState == ADC_THRESHOLD_STATE_THRESHOLD)
      {
         DALSYS_SyncLeave(pDevCtxt->hSync);
         return ADC_ERROR_TM_BUSY;
      }
   }

   /* Update the thresholds and then aggregate */
   pThreshold = &pMeas->aClientThresholds[uClientIdx][ADC_TM_THRESHOLD_LOWER];
   if (pnLowerTolerance == NULL)
   {
      pThreshold->hEvent = NULL;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_DISABLED;
   }
   else
   {
      pThreshold->hEvent = pAdcTMParams->hEvent;
      pThreshold->nThreshold = nLowThreshold;
      pThreshold->nTolerance = *pnLowerTolerance;
      pThreshold->nCurrentValue = nPhysicalCurrent;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_TOLERANCE;
   }

   pThreshold = &pMeas->aClientThresholds[uClientIdx][ADC_TM_THRESHOLD_HIGHER];
   if (pnHigherTolerance == NULL)
   {
      pThreshold->hEvent = NULL;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_DISABLED;
   }
   else
   {
      pThreshold->hEvent = pAdcTMParams->hEvent;
      pThreshold->nThreshold = nHighThreshold;
      pThreshold->nTolerance = *pnHigherTolerance;
      pThreshold->nCurrentValue = nPhysicalCurrent;
      pThreshold->eThresholdState = ADC_THRESHOLD_STATE_TOLERANCE;
   }

   if (pDevCtxt->aClients[uClientIdx].bEnabled == TRUE)
   {
      for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
      {
         result = Adc_TMAggregateAndRearmThresh(pDevCtxt,
                                                uDeviceIdx,
                                                uChannelIdx,
                                                (AdcTMThresholdType)uThresh,
                                                &nThresholdSet);
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return result;
}

