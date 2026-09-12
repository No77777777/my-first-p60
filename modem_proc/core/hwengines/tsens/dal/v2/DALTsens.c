/*============================================================================
  FILE:         TsensDevice.c

  OVERVIEW:     Implementation of the TSENS device library

  DEPENDENCIES: None

                Copyright (c) 2012-2015, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/tsens/dal/v2/DALTsens.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-02-17  jjo  Auto-gen BSP update.
  2015-02-04  jjo  Add calibration API.
  2014-12-10  jjo  Modify for hardware temperature conversion.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DALTsens.h"
#include "HALtsens.h"
#include "msg.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define TSENS_DEFAULT_STACK_SIZE 0x2000

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
#ifdef FEATURE_TSENS_THRESHOLDS
static uint32 gTsensWorkloopPriority = 0;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint8* Tsens_MapHWIORegion(TsensDevCtxt *pDevCtxt, const char *pszBase)
{
   DALResult status;
   uint8 *puVirtAddr;

   status = DalHWIO_MapRegion(pDevCtxt->phHWIO,
                              pszBase,
                              &puVirtAddr);
   if (status != DAL_SUCCESS)
   {
      return NULL;
   }

   return puVirtAddr;
}

static void Tsens_GetTempInternal(TsensDevCtxt *pDevCtxt, uint32 uController, uint32 uChannel, int32 *pnDeciDegC)
{
   int32 nDeciDegC = 0;
   int32 nDeciDegCTry1;
   int32 nDeciDegCTry2;
   int32 nDeciDegCTry3;
   uint8 *pucTsensTMAddr;
   boolean bValid;

   pucTsensTMAddr = pDevCtxt->paControllers[uController].pucTsensTMAddr;

   bValid = HAL_tsens_tm_GetChannelPrevTemp(pucTsensTMAddr, uChannel, &nDeciDegCTry1);
   if (bValid)
   {
      nDeciDegC = nDeciDegCTry1;
   }
   else
   {
      bValid = HAL_tsens_tm_GetChannelPrevTemp(pucTsensTMAddr, uChannel, &nDeciDegCTry2);
      if (bValid)
      {
         nDeciDegC = nDeciDegCTry2;
      }
      else
      {
         bValid = HAL_tsens_tm_GetChannelPrevTemp(pucTsensTMAddr, uChannel, &nDeciDegCTry3);
         if (bValid)
         {
            nDeciDegC = nDeciDegCTry3;
         }
         else if (nDeciDegCTry1 == nDeciDegCTry2)
         {
            nDeciDegC = nDeciDegCTry1;
         }
         else if (nDeciDegCTry2 == nDeciDegCTry3)
         {
            nDeciDegC = nDeciDegCTry2;
         }
         else
         {
            nDeciDegC = nDeciDegCTry1;
         }
      }
   }

   *pnDeciDegC = (int32)nDeciDegC;

   return;
}

#ifdef FEATURE_TSENS_THRESHOLDS
static int32 minval(int32 a, int32 b)
{
   return (a < b) ? a: b;
}

static int32 maxval(int32 a, int32 b)
{
   return (a > b) ? a: b;
}

static void Tsens_UpdateThresholds(TsensDevCtxt *pDevCtxt, uint32 uController, TsensThresholdType eThreshold)
{
   TsensControllerType *pController = &pDevCtxt->paControllers[uController];
   TsensClientInfoType *pClient;
   HAL_tsens_tm_Threshold eHalThreshold;
   uint32 uClient;
   int32 nAggrThreshold = 0;
   int32 nThreshold;
   boolean bThresholdEnabled;
   uint32 uChannel;
   uint32 uThresholdEnMask = 0;

   switch (eThreshold)
   {
      case TSENS_THRESHOLD_LOWER:
         eHalThreshold = HAL_TSENS_TH_LOWER;
         break;

      case TSENS_THRESHOLD_UPPER:
         eHalThreshold = HAL_TSENS_TH_UPPER;
         break;

      case TSENS_THRESHOLD_CRITICAL:
         eHalThreshold = HAL_TSENS_TH_CRITICAL;
         break;

      default:
         return;
   }

   DALSYS_SyncEnter(pDevCtxt->hIsrSync);
   if ((pController->uThreshCrossedMask & (1 << eThreshold)) == 0)
   {
      HAL_tsens_tm_SetInterruptEn(pController->pucTsensTMAddr, 0, eHalThreshold);
   }
   DALSYS_SyncLeave(pDevCtxt->hIsrSync);

   for (uChannel = 0; uChannel < TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER; uChannel++)
   {
      if ((pController->uChannelEnableMask & (1 << uChannel)) == 0)
      {
         continue;
      }

      bThresholdEnabled = FALSE;

      for (uClient = 0; uClient < TSENS_MAX_NUM_CLIENTS; uClient++)
      {
         pClient = &pDevCtxt->aClients[uClient];

         if (!pClient->bRegistered || !pClient->bEnabled)
         {
            continue;
         }

         if (pClient->paControllers[uController].aThresholds[uChannel][eThreshold].eThresholdState == TSENS_THRESHOLD_STATE_ENABLED)
         {
            nThreshold = pClient->paControllers[uController].aThresholds[uChannel][eThreshold].nThresholdDeciDegC;
            if (!bThresholdEnabled)
            {
               nAggrThreshold = nThreshold;
               bThresholdEnabled = TRUE;
            }
            else if (eThreshold == TSENS_THRESHOLD_UPPER || eThreshold == TSENS_THRESHOLD_CRITICAL)
            {
               nAggrThreshold = minval(nThreshold, nAggrThreshold);
            }
            else
            {
               nAggrThreshold = maxval(nThreshold, nAggrThreshold);
            }
         }
      }

      if (bThresholdEnabled)
      {
         uThresholdEnMask |= 1 << uChannel;
         if (pController->anAggrThresholds[uChannel][eThreshold] != nAggrThreshold)
         {
            pController->anAggrThresholds[uChannel][eThreshold] = nAggrThreshold;
            HAL_tsens_tm_SetThreshold(pController->pucTsensTMAddr, uChannel, eHalThreshold, nAggrThreshold);
         }
      }
   }

   DALSYS_SyncEnter(pDevCtxt->hIsrSync);
   if ((pController->uThreshCrossedMask & (1 << eThreshold)) == 0)
   {
      HAL_tsens_tm_SetInterruptEn(pController->pucTsensTMAddr, uThresholdEnMask, eHalThreshold);
   }
   DALSYS_SyncLeave(pDevCtxt->hIsrSync);

   return;
}

static DALResult Tsens_ThresholdWorkLoop(DALSYSEventHandle hEvent, void *pInDevCtxt)
{
   TsensWorkLoopContext *pWorkLoopCtxt = (TsensWorkLoopContext *)pInDevCtxt;
   TsensDevCtxt *pDevCtxt = pWorkLoopCtxt->pDevCtxt;
   TsensThresholdType eThreshold = pWorkLoopCtxt->eThreshold;
   uint32 uController = pWorkLoopCtxt->uController;
   TsensControllerType *pController = &pDevCtxt->paControllers[pWorkLoopCtxt->uController];
   TsensCallbackPayloadType tsensCallbackPayload;
   TsensThresholdConfigType *pThreshold;
   TsensClientInfoType *pClient;
   int32 nDeciDegC;
   uint32 uSensor;
   uint32 uChannel;
   uint32 uClient;

   DALSYS_SyncEnter(pDevCtxt->hSync);

   for (uChannel = 0; uChannel < TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER; uChannel++)
   {
      if (pController->uChannelEnableMask & (1 << uChannel))
      {
         Tsens_GetTempInternal(pDevCtxt, uController, uChannel, &nDeciDegC);

         uSensor = pController->auSensorIdx[uChannel];

         for (uClient = 0; uClient < TSENS_MAX_NUM_CLIENTS; uClient++)
         {
            pClient = &pDevCtxt->aClients[uClient];

            if (pClient->bRegistered && pClient->bEnabled)
            {
               pThreshold = &pClient->paControllers[uController].aThresholds[uChannel][eThreshold];

               if (pThreshold->eThresholdState == TSENS_THRESHOLD_STATE_ENABLED)
               {
                  if ((eThreshold == TSENS_THRESHOLD_LOWER && nDeciDegC <= pThreshold->nThresholdDeciDegC) ||
                      (eThreshold == TSENS_THRESHOLD_UPPER && nDeciDegC >= pThreshold->nThresholdDeciDegC) ||
                      (eThreshold == TSENS_THRESHOLD_CRITICAL && nDeciDegC >= pThreshold->nThresholdDeciDegC))
                  {
                     pThreshold->eThresholdState = TSENS_THRESHOLD_STATE_TRIGGERED;

                     tsensCallbackPayload.eThreshold = eThreshold;
                     tsensCallbackPayload.uSensor = uSensor;
                     tsensCallbackPayload.tempTriggered.nDeciDegC = nDeciDegC;
                     tsensCallbackPayload.tempTriggered.nDegC = nDeciDegC / 10;

                     DALSYS_SyncLeave(pDevCtxt->hSync);
                     (void)DALSYS_EventCtrlEx(pThreshold->hEvent,
                                              DALSYS_EVENT_CTRL_TRIGGER,
                                              eThreshold,
                                              (void *)&tsensCallbackPayload,
                                              sizeof(tsensCallbackPayload));
                     DALSYS_SyncEnter(pDevCtxt->hSync);
                  }
               }
            }
         }
      }
   }

   DALSYS_SyncEnter(pDevCtxt->hIsrSync);
   pController->uThreshCrossedMask &= ~(1 << eThreshold);
   DALSYS_SyncLeave(pDevCtxt->hIsrSync);

   Tsens_UpdateThresholds(pDevCtxt, uController, eThreshold);

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return DAL_SUCCESS;
}

static void * Tsens_ThresholdIsr(DALISRCtx pCtxt)
{
   TsensInterruptContext *pIsrCtxt = (TsensInterruptContext *)pCtxt;
   TsensDevCtxt *pDevCtxt = pIsrCtxt->pDevCtxt;
   TsensControllerType *pController = &pDevCtxt->paControllers[pIsrCtxt->uController];
   TsensThresholdType eThreshold;
   uint32 uThreshCrossedMask = 0;

   DALSYS_SyncEnter(pDevCtxt->hIsrSync);

   if (pIsrCtxt->eInterrupt == TSENS_INTERRUPT_LOW_HIGH)
   {
      if (HAL_tsens_tm_GetInterruptStatus(pController->pucTsensTMAddr, HAL_TSENS_TH_LOWER) != 0)
      {
         HAL_tsens_tm_SetInterruptEn(pController->pucTsensTMAddr, 0, HAL_TSENS_TH_LOWER);
         HAL_tsens_tm_ClearInterrupt(pController->pucTsensTMAddr, pController->uChannelEnableMask, HAL_TSENS_TH_LOWER);
         pController->uThreshCrossedMask |= 1 << TSENS_THRESHOLD_LOWER;
         uThreshCrossedMask |= 1 << TSENS_THRESHOLD_LOWER;
      }

      if (HAL_tsens_tm_GetInterruptStatus(pController->pucTsensTMAddr, HAL_TSENS_TH_UPPER) != 0)
      {
         HAL_tsens_tm_SetInterruptEn(pController->pucTsensTMAddr, 0, HAL_TSENS_TH_UPPER);
         HAL_tsens_tm_ClearInterrupt(pController->pucTsensTMAddr, pController->uChannelEnableMask, HAL_TSENS_TH_UPPER);
         pController->uThreshCrossedMask |= 1 << TSENS_THRESHOLD_UPPER;
         uThreshCrossedMask |= 1 << TSENS_THRESHOLD_UPPER;
      }
   }
   else
   {
      if (HAL_tsens_tm_GetInterruptStatus(pController->pucTsensTMAddr, HAL_TSENS_TH_CRITICAL) != 0)
      {
         HAL_tsens_tm_SetInterruptEn(pController->pucTsensTMAddr, 0, HAL_TSENS_TH_CRITICAL);
         HAL_tsens_tm_ClearInterrupt(pController->pucTsensTMAddr, pController->uChannelEnableMask, HAL_TSENS_TH_CRITICAL);
         pController->uThreshCrossedMask |= 1 << TSENS_THRESHOLD_CRITICAL;
         uThreshCrossedMask |= 1 << TSENS_THRESHOLD_CRITICAL;
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hIsrSync);

   for (eThreshold = TSENS_THRESHOLD_LOWER; eThreshold < _TSENS_NUM_THRESHOLDS; eThreshold++)
   {
      if (uThreshCrossedMask & (1 << eThreshold))
      {
         (void)DALSYS_EventCtrlEx(pController->aWorkLoopCtxt[eThreshold].hWorkLoopEvent,
                                  DALSYS_EVENT_CTRL_TRIGGER,
                                  0,
                                  NULL,
                                  0);
      }
   }

   return NULL;
}
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
 * Functions specific to the DalTsens interface
 * ----------------------------------------------------------------------*/
DALResult Tsens_DeviceInit(TsensClientCtxt *pCtxt)
{
   const char *pszTsensBspPropName = "TSENS_BSP";
   const TsensControllerCfgType *pControllerCfg;
   const TsensBspType *pBsp;
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   DALSYSPropertyVar propertyVar;
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensClientControllerType *pClientController;
   TsensWorkLoopContext *pWorkLoopCtxt;
   TsensInterruptContext *pIsrCtxt;
   TsensClientInfoType *pClient;
   TsensThresholdType eThreshold;
   TsensInterruptType eInterrupt;
   uint8 *pucTsensTMAddr;
   uint32 uInterruptId;
   uint32 uChannel;
   uint32 uSensor;
   uint32 uClient;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
   TsensControllerType *pController;
   uint32 uController;
   DALResult status;
   uint8 *pucTsensSROTAddr;

   /* Get the BSP */
   status = DALSYS_GetPropertyValue(pDevCtxt->hProp,
                                    pszTsensBspPropName,
                                    0,
                                    &propertyVar);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Device failed to get TSENS_BSP property");
      goto error;
   }

   pBsp = (TsensBspType *)propertyVar.Val.pStruct;
   pDevCtxt->pBsp = pBsp;

   /* Allocate memory for device context */
   status = DALSYS_Malloc((pBsp->uNumControllers * sizeof(TsensControllerType)),
                          (void **)&pDevCtxt->paControllers);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Failed to allocate memory");
      goto error;
   }

#ifdef FEATURE_TSENS_THRESHOLDS
   for (uClient = 0; uClient < TSENS_MAX_NUM_CLIENTS; uClient++)
   {
      status = DALSYS_Malloc((pBsp->uNumControllers * sizeof(TsensClientControllerType)),
                             (void **)&pDevCtxt->aClients[uClient].paControllers);
      if (status != DAL_SUCCESS)
      {
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Tsens_DeviceInit : Failed to allocate memory");
         goto error;
      }
   }

   /* Initialize synchronization objects */
   status = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                              &pDevCtxt->hSync,
                              &pDevCtxt->syncObject);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Could not create device lock\n");
      goto error;
   }

   status = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE_INTERRUPT,
                              &pDevCtxt->hIsrSync,
                              &pDevCtxt->isrSyncObject);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Could not create ISR lock\n");
      goto error;
   }

   /* Create work loop */
   status = DALSYS_RegisterWorkLoopEx("TSENS",
                                      TSENS_DEFAULT_STACK_SIZE,
                                      gTsensWorkloopPriority,
                                      pDevCtxt->pBsp->uNumControllers * _TSENS_NUM_THRESHOLDS,
                                      &pDevCtxt->hTsensWorkLoop,
                                      NULL);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : failed to create work loop");
      goto error;
   }
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

   /* Map physical to virtual addresses */
   status = DAL_DeviceAttach(DALDEVICEID_HWIO, &pDevCtxt->phHWIO);
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Failed to attach to HWIO with status=%i\n", status);
      goto error;
   }

   for (uController = 0; uController < pBsp->uNumControllers; uController++)
   {
      pControllerCfg = &pBsp->paControllerCfgs[uController];
      pController = &pDevCtxt->paControllers[uController];

      pController->pucTsensSROTAddr = Tsens_MapHWIORegion(pDevCtxt,
                                                          pControllerCfg->pszSROTModule);
      if (pController->pucTsensSROTAddr == NULL)
      {
         status = DAL_ERROR;
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Tsens_DeviceInit : Failed to map SROT region");
         goto error;
      }

      pController->pucTsensTMAddr = Tsens_MapHWIORegion(pDevCtxt,
                                                        pControllerCfg->pszTMModule);
      if (pController->pucTsensSROTAddr == NULL)
      {
         status = DAL_ERROR;
         DALSYS_LogEvent(pDevCtxt->DevId, DALSYS_LOGEVENT_FATAL_ERROR,
                         "Tsens_DeviceInit : Failed to map TM region");
         goto error;
      }
   }

#ifdef FEATURE_TSENS_THRESHOLDS
   /* Get the min and max temperatures */
   pDevCtxt->nMinTempDeciDegC = HAL_tsens_GetMinTemp();
   pDevCtxt->nMaxTempDeciDegC = HAL_tsens_GetMaxTemp();

   /* Initialize clients */
   for (uClient = 0; uClient < TSENS_MAX_NUM_CLIENTS; uClient++)
   {
      pClient = &pDevCtxt->aClients[uClient];
      pClient->bEnabled = TRUE;
      pClient->bRegistered = FALSE;

      for (uController = 0; uController < pBsp->uNumControllers; uController++)
      {
         pClientController = &pClient->paControllers[uController];
         for (uChannel = 0; uChannel < TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER; uChannel++)
         {
            for (eThreshold = TSENS_THRESHOLD_LOWER; eThreshold < _TSENS_NUM_THRESHOLDS; eThreshold++)
            {
               pClientController->aThresholds[uChannel][eThreshold].eThresholdState = TSENS_THRESHOLD_STATE_DISABLED;
            }
         }
      }
   }

   /* Store sensor index to channel index mapping */
   for (uSensor = 0; uSensor < pBsp->uNumSensors; uSensor++)
   {
      uController = pBsp->paSensorCfgs[uSensor].ucController;
      uChannel = pBsp->paSensorCfgs[uSensor].ucChannel;
      pDevCtxt->paControllers[uController].auSensorIdx[uChannel] = uSensor;
   }

   /* Obtain a handle to the interrupt controller */
   status = DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER,
                             &(((DALDevCtxt *)pDevCtxt)->hDALInterrupt));
   if (status != DAL_SUCCESS)
   {
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                       "Tsens_DeviceInit : Device failed to attach to Interrupt Controller");

      goto error;
   }

   status = DalDevice_Open(((DALDevCtxt *)pDevCtxt)->hDALInterrupt, DAL_OPEN_SHARED);
   if (status != DAL_SUCCESS)
   {
      DAL_DeviceDetach(((DALDevCtxt *)pDevCtxt)->hDALInterrupt);

      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                      "Tsens_DeviceInit : Device failed to open Interrupt Controller");
      goto error;
   }
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

   /* Initialize controllers */
   for (uController = 0; uController < pBsp->uNumControllers; uController++)
   {
      pController = &pDevCtxt->paControllers[uController];
      pucTsensSROTAddr = pController->pucTsensSROTAddr;
#ifdef FEATURE_TSENS_THRESHOLDS
      pucTsensTMAddr = pController->pucTsensTMAddr;
      pControllerCfg = &pBsp->paControllerCfgs[uController];
      pController->uThreshCrossedMask = 0;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

      /* Get the enabled channels */
      pController->uChannelEnableMask = HAL_tsens_srot_GetEnabledChannels(pucTsensSROTAddr);

#ifdef FEATURE_TSENS_THRESHOLDS
      /* Initialize the thresholds to off */
      HAL_tsens_tm_SetInterruptEn(pucTsensTMAddr, 0, HAL_TSENS_TH_LOWER);
      HAL_tsens_tm_SetInterruptEn(pucTsensTMAddr, 0, HAL_TSENS_TH_UPPER);
      HAL_tsens_tm_SetInterruptEn(pucTsensTMAddr, 0, HAL_TSENS_TH_CRITICAL);

      /* Initialize threshold values */
      for (uChannel = 0; uChannel < TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER; uChannel++)
      {
         if (pController->uChannelEnableMask & (1 << uChannel))
         {
            HAL_tsens_tm_SetThreshold(pucTsensTMAddr, uChannel, HAL_TSENS_TH_LOWER, pDevCtxt->nMinTempDeciDegC);
            HAL_tsens_tm_SetThreshold(pucTsensTMAddr, uChannel, HAL_TSENS_TH_UPPER, pDevCtxt->nMaxTempDeciDegC);
            HAL_tsens_tm_SetThreshold(pucTsensTMAddr, uChannel, HAL_TSENS_TH_CRITICAL, pDevCtxt->nMaxTempDeciDegC);

            pController->anAggrThresholds[uChannel][TSENS_THRESHOLD_LOWER] = pDevCtxt->nMinTempDeciDegC;
            pController->anAggrThresholds[uChannel][TSENS_THRESHOLD_UPPER] = pDevCtxt->nMaxTempDeciDegC;
            pController->anAggrThresholds[uChannel][TSENS_THRESHOLD_CRITICAL] = pDevCtxt->nMaxTempDeciDegC;
         }
      }

      for (eThreshold = TSENS_THRESHOLD_LOWER; eThreshold < _TSENS_NUM_THRESHOLDS; eThreshold++)
      {
         pWorkLoopCtxt = &pController->aWorkLoopCtxt[eThreshold];
         pWorkLoopCtxt->pDevCtxt = pDevCtxt;
         pWorkLoopCtxt->uController = uController;
         pWorkLoopCtxt->eThreshold = eThreshold;

         status = DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT,
                                     &pWorkLoopCtxt->hWorkLoopEvent,
                                     NULL);
         if (status != DAL_SUCCESS)
         {
            DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                            "Tsens_DeviceInit : failed to create work loop event");
            goto error;
         }

         status = DALSYS_AddEventToWorkLoop(pDevCtxt->hTsensWorkLoop,
                                            Tsens_ThresholdWorkLoop,
                                            pWorkLoopCtxt,
                                            pWorkLoopCtxt->hWorkLoopEvent,
                                            NULL);
         if (status != DAL_SUCCESS)
         {
            DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                            "Tsens_DeviceInit : failed to create work loop event");
            return status;
         }
      }

      /* Register the interrupts */
      for (eInterrupt = TSENS_INTERRUPT_LOW_HIGH; eInterrupt < _TSENS_NUM_INTERRUPTS; eInterrupt++)
      {
         pIsrCtxt = &pController->aIntCtxt[eInterrupt];
         pIsrCtxt->pDevCtxt = pDevCtxt;
         pIsrCtxt->uController = uController;
         pIsrCtxt->eInterrupt = eInterrupt;

         if (eInterrupt == TSENS_INTERRUPT_LOW_HIGH)
         {
            uInterruptId = pControllerCfg->uUpperLowerInt;
         }
         else
         {
            uInterruptId = pControllerCfg->uCriticalInt;
         }

         status = DalInterruptController_RegisterISR(((DALDevCtxt *)pDevCtxt)->hDALInterrupt,
                                                     uInterruptId,
                                                     Tsens_ThresholdIsr,
                                                     pIsrCtxt,
                                                     DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER);
         if (status != DAL_SUCCESS)
         {
            DAL_DeviceDetach(((DALDevCtxt *)pDevCtxt)->hDALInterrupt);
            DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_FATAL_ERROR,
                            "Tsens_DeviceInit : Device failed to register ISR");
            goto error;
         }
      }

      /* Enable interrupts */
      HAL_tsens_tm_EnableUpperLowerInterrupt(pucTsensTMAddr);
      HAL_tsens_tm_EnableCriticalInterrupt(pucTsensTMAddr);
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
   }

#ifdef FEATURE_TSENS_THRESHOLDS
   /* The first client needs registered in Init because for the first call to
    * Tsens_RegisterClient the ctxt is not yet initialized */
   pDevCtxt->aClients[0].bRegistered = TRUE;
   pCtxt->uClientId = 0;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

   pDevCtxt->eDeviceState = TSENS_DEVICE_STATE_READY;

   return DAL_SUCCESS;

error:
   pDevCtxt->eDeviceState = TSENS_DEVICE_STATE_ERROR;
   return status;
}

DALResult Tsens_DeviceDeInit(TsensClientCtxt *pCtxt)
{
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensControllerType *pController;
   uint32 uController;
#endif

   pDevCtxt->eDeviceState = TSENS_DEVICE_STATE_INIT;

#ifdef FEATURE_TSENS_THRESHOLDS
   for (uController = 0; uController < pDevCtxt->pBsp->uNumControllers; uController++)
   {
      pController = &pDevCtxt->paControllers[uController];

      HAL_tsens_tm_DisableUpperLowerInterrupt(pController->pucTsensTMAddr);
      HAL_tsens_tm_DisableCriticalInterrupt(pController->pucTsensTMAddr);
   }
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */

   return DAL_SUCCESS;
}

DALResult Tsens_GetNumSensors(TsensClientCtxt *pCtxt, uint32 *puNumSensors)
{
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   const TsensBspType *pBsp = pDevCtxt->pBsp;

   if (puNumSensors == NULL)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   *puNumSensors = pBsp->uNumSensors;

   return DAL_SUCCESS;
}

DALResult Tsens_CheckCalibration(TsensClientCtxt *pCtxt, uint32 uSensor)
{
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   const TsensBspType *pBsp = pDevCtxt->pBsp;
   uint32 uController;
   uint32 uChannel;

   if (uSensor >= pBsp->uNumSensors)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   uController = pBsp->paSensorCfgs[uSensor].ucController;
   uChannel = pBsp->paSensorCfgs[uSensor].ucChannel;

   if (((1 << uChannel) & pDevCtxt->paControllers[uController].uChannelEnableMask) == 0)
   {
      return TSENS_ERROR_DEAD_SENSOR;
   }

   return DAL_SUCCESS;
}

DALResult Tsens_GetTemp(TsensClientCtxt *pCtxt, uint32 uSensor, TsensTempType *pTemp)
{
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   const TsensBspType *pBsp = pDevCtxt->pBsp;
   uint32 uController;
   uint32 uChannel;
   int32 nDeciDegC;

   if (uSensor >= pBsp->uNumSensors)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if (pTemp == NULL)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   uController = pBsp->paSensorCfgs[uSensor].ucController;
   uChannel = pBsp->paSensorCfgs[uSensor].ucChannel;

   if (((1 << uChannel) & pDevCtxt->paControllers[uController].uChannelEnableMask) == 0)
   {
      return TSENS_ERROR_DEAD_SENSOR;
   }

   Tsens_GetTempInternal(pDevCtxt, uController, uChannel, &nDeciDegC);

   pTemp->nDeciDegC = nDeciDegC;
   pTemp->nDegC = nDeciDegC / 10;

   MSG_2(MSG_SSID_ADC,
         MSG_LEGACY_HIGH,
         "TSENS: Sensor = %u, DeciDegC = %i",
         uSensor,
         nDeciDegC);

   return DAL_SUCCESS;
}

DALResult Tsens_GetTempRange(TsensClientCtxt *pCtxt, uint32 uSensor, TsensTempRangeType *pTempRange)
{
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   uint32 uController;
   uint32 uChannel;

   if (pTempRange == NULL)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if (uSensor >= pDevCtxt->pBsp->uNumSensors)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   uController = pDevCtxt->pBsp->paSensorCfgs[uSensor].ucController;
   uChannel = pDevCtxt->pBsp->paSensorCfgs[uSensor].ucChannel;

   if (((1 << uChannel) & pDevCtxt->paControllers[uController].uChannelEnableMask) == 0)
   {
      return TSENS_ERROR_DEAD_SENSOR;
   }

   pTempRange->nMaxDeciDegC = pDevCtxt->nMaxTempDeciDegC;
   pTempRange->nMinDeciDegC = pDevCtxt->nMinTempDeciDegC;
   pTempRange->nMaxDegC = pTempRange->nMaxDeciDegC / 10;
   pTempRange->nMinDegC = pTempRange->nMinDeciDegC / 10;

   return DAL_SUCCESS;
#else
   return TSENS_ERROR_UNSUPPORTED;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
}

DALResult Tsens_SetThreshold(TsensClientCtxt *pCtxt, uint32 uSensor, TsensThresholdType eThreshold, int32 nDegC, DALSYSEventHandle hEvent)
{
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   uint32 uClient = pCtxt->uClientId;
   TsensThresholdConfigType *pThreshold;
   DALResult status = DAL_SUCCESS;
   int32 nDeciDegC;
   uint32 uController;
   uint32 uChannel;

   if (uClient >= TSENS_MAX_NUM_CLIENTS)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if (uSensor >= pDevCtxt->pBsp->uNumSensors)
   {
      return TSENS_ERROR_UNSUPPORTED;
   }

   if ((eThreshold != TSENS_THRESHOLD_LOWER) &&
       (eThreshold != TSENS_THRESHOLD_UPPER) &&
       (eThreshold != TSENS_THRESHOLD_CRITICAL))
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if (nDegC == TSENS_INVALID_THRESHOLD)
   {
      nDeciDegC = TSENS_INVALID_THRESHOLD;
   }
   else
   {
      nDeciDegC = nDegC * 10;
   }

   if ((nDeciDegC > pDevCtxt->nMaxTempDeciDegC || nDeciDegC < pDevCtxt->nMinTempDeciDegC)
       && nDeciDegC != TSENS_INVALID_THRESHOLD)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if (hEvent == NULL && nDeciDegC != TSENS_INVALID_THRESHOLD)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   uController = pDevCtxt->pBsp->paSensorCfgs[uSensor].ucController;
   uChannel = pDevCtxt->pBsp->paSensorCfgs[uSensor].ucChannel;

   if (((1 << uChannel) & pDevCtxt->paControllers[uController].uChannelEnableMask) == 0)
   {
      return TSENS_ERROR_DEAD_SENSOR;
   }

   pThreshold = &pDevCtxt->aClients[uClient].paControllers[uController].aThresholds[uChannel][eThreshold];

   DALSYS_SyncEnter(pDevCtxt->hSync);

   if (pDevCtxt->aClients[uClient].bRegistered)
   {
      if (nDeciDegC == TSENS_INVALID_THRESHOLD)
      {
         pThreshold->eThresholdState = TSENS_THRESHOLD_STATE_DISABLED;
      }
      else
      {
         pThreshold->eThresholdState = TSENS_THRESHOLD_STATE_ENABLED;
         pThreshold->nThresholdDeciDegC = nDeciDegC;
         pThreshold->hEvent = hEvent;
      }

      Tsens_UpdateThresholds(pDevCtxt, uController, eThreshold);
   }
   else
   {
      status = TSENS_ERROR_INVALID_PARAMETER;
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return status;
#else
   return TSENS_ERROR_UNSUPPORTED;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
}

DALResult Tsens_SetEnableThresholds(TsensClientCtxt *pCtxt, DALBOOL bEnableThresholds)
{
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   DALResult status = DAL_SUCCESS;
   uint32 uClient = pCtxt->uClientId;
   TsensThresholdType eThreshold;
   uint32 uController;

   if (uClient >= TSENS_MAX_NUM_CLIENTS)
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   if ((bEnableThresholds != TRUE) && (bEnableThresholds != FALSE))
   {
      return TSENS_ERROR_INVALID_PARAMETER;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   if (pDevCtxt->aClients[uClient].bRegistered)
   {
      pDevCtxt->aClients[uClient].bEnabled = bEnableThresholds;

      for (uController = 0; uController < pDevCtxt->pBsp->uNumControllers; uController++)
      {
         for (eThreshold = TSENS_THRESHOLD_LOWER; eThreshold < _TSENS_NUM_THRESHOLDS; eThreshold++)
         {
            Tsens_UpdateThresholds(pDevCtxt, uController, eThreshold);
         }
      }
   }
   else
   {
      status = TSENS_ERROR_INVALID_PARAMETER;
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return status;
#else
   return TSENS_ERROR_UNSUPPORTED;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
}

/*-------------------------------------------------------------------------
 * Functions specific to the DAL interface
 * ----------------------------------------------------------------------*/
DALResult
Tsens_DriverInit(TsensDrvCtxt *pCtxt)
{
   uint32 uDevice;

   for (uDevice = 0; uDevice < TSENS_MAX_NUM_DEVICES; uDevice++)
   {
      pCtxt->TsensDevCtxt[uDevice].eDeviceState = TSENS_DEVICE_STATE_INIT;
   }

   return DAL_SUCCESS;
}

DALResult
Tsens_DriverDeInit(TsensDrvCtxt *pCtxt)
{
   return DAL_SUCCESS;
}

DALResult
Tsens_RegisterClient(TsensClientCtxt *pCtxt)
{
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   DALResult status = DAL_ERROR;
   uint32 uClient;

   if (pDevCtxt->eDeviceState == TSENS_DEVICE_STATE_INIT)
   {
      /* The DAL calls RegisterClient before calling DeviceInit, so this is a normal case.
       * As a result, the first client is registered in Tsens_DeviceInit */
      return DAL_SUCCESS;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   for (uClient = 0; uClient < TSENS_MAX_NUM_CLIENTS; uClient++)
   {
      if (!pDevCtxt->aClients[uClient].bRegistered)
      {
         pDevCtxt->aClients[uClient].bRegistered = TRUE;
         pDevCtxt->aClients[uClient].bEnabled = TRUE;
         pCtxt->uClientId = uClient;
         status = DAL_SUCCESS;
         break;
      }
   }

   if (status != DAL_SUCCESS)
   {
      pCtxt->uClientId = TSENS_MAX_NUM_CLIENTS;
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_ERROR,
                      "Tsens_RegisterClient : No free client handles.\n");
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return status;
#else
   return DAL_SUCCESS;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
}

DALResult
Tsens_DeregisterClient(TsensClientCtxt *pCtxt)
{
#ifdef FEATURE_TSENS_THRESHOLDS
   TsensDevCtxt *pDevCtxt = pCtxt->pTsensDevCtxt;
   TsensClientControllerType *pClientController;
   TsensThresholdType eThreshold;
   uint32 uController;
   uint32 uChannel;
   uint32 uClient;

   uClient = pCtxt->uClientId;

   if (uClient >= TSENS_MAX_NUM_CLIENTS)
   {
      DALSYS_LogEvent(DALDEVICEID_TSENS, DALSYS_LOGEVENT_ERROR,
                      "Failed to deregister the client");
      return DAL_ERROR;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   pDevCtxt->aClients[uClient].bRegistered = FALSE;
   pDevCtxt->aClients[uClient].bEnabled = FALSE;

   for (uController = 0; uController < pDevCtxt->pBsp->uNumControllers; uController++)
   {
      pClientController = &pDevCtxt->aClients[uClient].paControllers[uController];
      for (eThreshold = TSENS_THRESHOLD_LOWER; eThreshold < _TSENS_NUM_THRESHOLDS; eThreshold++)
      {
         for (uChannel = 0; uChannel < TSENS_MAX_NUM_CHANNELS_PER_CONTROLLER; uChannel++)
         {
            pClientController->aThresholds[uChannel][eThreshold].eThresholdState = TSENS_THRESHOLD_STATE_DISABLED;
         }

         Tsens_UpdateThresholds(pDevCtxt, uController, eThreshold);
      }
   }

   DALSYS_SyncLeave(pDevCtxt->hSync);

   return DAL_SUCCESS;
#else
   return DAL_SUCCESS;
#endif /* #ifdef FEATURE_TSENS_THRESHOLDS */
}

