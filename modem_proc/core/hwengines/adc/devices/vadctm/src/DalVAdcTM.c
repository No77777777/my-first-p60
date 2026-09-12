/*============================================================================
  FILE:         DalVAdcTM.c

  OVERVIEW:     Implementation of a Physical ADC device DAL for the
                PMIC VADCTM peripheral.

  DEPENDENCIES: None

                Copyright (c) 2009-2016, 2020 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/DalVAdcTM.c#4 $$DateTime: 2020/02/13 04:15:11 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-03-20  KS   Updated for Talos
  2018-02-06  jjo  Enhance RUMI check.
  2016-03-23  jjo  Add HW supported checks.
  2015-08-12  jjo  Updated for VADC refresh.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DalVAdcTM.h"
#include "DDIPlatformInfo.h"
#include "DDIAdc.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define VADCTM_SPMI_READ_NUM_RETRIES 5
#define VADCTM_DEFAULT_STACK_SIZE 0x2000

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/
static uint32 gVAdcTMWorkloopPriority = 0;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
boolean gVAdcTMOverrideRumiCheck = FALSE;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
/*======================================================================

  FUNCTION        VAdcTM_ReadBytes

  DESCRIPTION     This function reads from the VAdcTM peripheral

  DEPENDENCIES    None

  PARAMETERS
      pCtxt            [in]  pointer to the HAL interface context
      uOffset          [in]  the starting address
      pucData          [out] the bytes read
      uLen             [in]  the number of bytes to read

  RETURN VALUE    VADCTM_HAL_SUCCESS or an error code

  SIDE EFFECTS    None

======================================================================*/
static VAdcTMHalResultType
VAdcTM_ReadBytes(
   void *pCtxt,
   uint32 uOffset,
   uint8 *pucData,
   uint32 uLen
   )
{
   VAdcTMDevCtxt *pDevCtxt = (VAdcTMDevCtxt *)pCtxt;
   uint32 uBytesRead;
   DALResult result;
   uint32 i;

   for (i = 0; i < VADCTM_SPMI_READ_NUM_RETRIES; i++)
   {
      result = AdcSpmiRead(&pDevCtxt->spmiCtxt,
                           uOffset,
                           pucData,
                           uLen,
                           &uBytesRead);
      if (result != DAL_SUCCESS || uBytesRead != uLen)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - SPMI read failed, try again", FALSE);
         continue;
      }

      return VADCTM_HAL_SUCCESS;
   }

   VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - SPMI read failed repeatedly", TRUE);
   return VADCTM_HAL_ERROR;
}

/*======================================================================

  FUNCTION        VAdcTM_WriteBytes

  DESCRIPTION     This function writes to the VAdcTM peripheral

  DEPENDENCIES    None

  PARAMETERS
      pCtxt            [in] pointer to the HAL interface context
      uOffset          [in] the starting address
      pucData          [in] the bytes to write
      uLen             [in] the number of bytes to write

  RETURN VALUE    VADCTM_HAL_SUCCESS or an error code

  SIDE EFFECTS    None

======================================================================*/
static VAdcTMHalResultType
VAdcTM_WriteBytes(
   void *pCtxt,
   uint32 uOffset,
   uint8 *pucData,
   uint32 uLen
   )
{
   VAdcTMDevCtxt *pDevCtxt = (VAdcTMDevCtxt *)pCtxt;
   DALResult result;

   result = AdcSpmiWrite(&pDevCtxt->spmiCtxt,
                         uOffset,
                         pucData,
                         uLen);
   if (result != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - SPMI write failed", TRUE);
      return VADCTM_HAL_ERROR;
   }

   return VADCTM_HAL_SUCCESS;
}

/*======================================================================

  FUNCTION        VAdcTM_GetPullUp

  DESCRIPTION     Gets the pull up for a meas.

  DEPENDENCIES    None

  PARAMETERS
     pDevCtxt        [in]
     uMeasIdx        [in]

  RETURN VALUE    Pull up value

  SIDE EFFECTS    None

======================================================================*/
static uint32
VAdcTM_GetPullUp(
   VAdcTMDevCtxt *pDevCtxt,
   uint32 uMeasIdx
   )
{
   const VAdcTMMeasConfigType *pMeasConfig;

   pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeasIdx];

   if (pMeasConfig->pPullUpConfig != NULL)
   {
      switch (pDevCtxt->aThresholds[uMeasIdx].ePullUpState)
      {
         case VADCTM_PULLUP_STATE_LOW:
            return pMeasConfig->pPullUpConfig->uLowPullUp;
         case VADCTM_PULLUP_STATE_HIGH:
            return pMeasConfig->pPullUpConfig->uHighPullUp;
         default:
            return pMeasConfig->uPullUp;
      }
   }
   else
   {
      return pMeasConfig->uPullUp;
   }
}

/*======================================================================

  FUNCTION        VAdcTM_GetAdcCode

  DESCRIPTION     This function gets the raw ADC code.

  DEPENDENCIES    VADC

  PARAMETERS
     pDevCtxt        [in]
     uMeasIdx        [in]
     eThreshold      [in]
     nPhysical       [in]
     puCode           [out]
     pnCodeToPhysical [out]

  RETURN VALUE    DAL_SUCCESS or an error code

  SIDE EFFECTS    None

======================================================================*/
static DALResult
VAdcTM_GetAdcCode(
   VAdcTMDevCtxt *pDevCtxt,
   uint32 uMeasIdx,
   AdcDeviceTMThresholdType eThreshold,
   int32 nPhysical,
   uint32 *puCode,
   int32 *pnCodeToPhysical
   )
{
   const VAdcTMMeasConfigType *pMeasConfig;
   DALBOOL bPhysicalInverseToCode;
   AdcDeviceResultType vAdcResult;
   uint32 uIteration;
   uint32 uCodeIncrement;
   uint32 uPullUp;
   uint32 uCode;

   /*
    * The ADC code needs to result in a *pnCodeToPhysical that is:
    * - bPhysicalInverseToCode == FALSE
    *   -- ADC_DEVICE_TM_THRESHOLD_LOWER: CodeToPhysical(*puCode) <= nPhysical
    *   -- ADC_DEVICE_TM_THRESHOLD_HIGHER: CodeToPhysical(*puCode) >= nPhysical
    * - bPhysicalInverseToCode == TRUE
    *   -- ADC_DEVICE_TM_THRESHOLD_LOWER: CodeToPhysical(*puCode) >= nPhysical
    *   -- ADC_DEVICE_TM_THRESHOLD_HIGHER: CodeToPhysical(*puCode) <= nPhysical
    *
    * First, calculate a code that is close. Next, increment / decrement the code
    * until the physical value meets the above criteria.
    */
   pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeasIdx];
   bPhysicalInverseToCode = pDevCtxt->aThresholds[uMeasIdx].bPhysicalInverseToCode;
   uPullUp = VAdcTM_GetPullUp(pDevCtxt, uMeasIdx);

   /* Scale the physical value to code */
   VAdcProcessConversionResultInverse(&pMeasConfig->scalingFactor,
                                      pMeasConfig->eScalingMethod,
                                      pMeasConfig->pIntTable,
                                      uPullUp,
                                      pDevCtxt->pBsp->uFullScale_code,
                                      pDevCtxt->pBsp->uFullScale_uV,
                                      nPhysical,
                                      &vAdcResult);

   if (vAdcResult.eStatus != ADC_DEVICE_RESULT_VALID)
   {
      /* Result cannot be scaled */
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed calculating inverse", FALSE);

      return DAL_ERROR;
   }

   uCode = vAdcResult.uCode;
   uCodeIncrement = 0;
   *puCode = uCode;

   for (uIteration = 0; uIteration < 15; uIteration++)
   {
      /* Scale the code back to physcial to get the physical value being monitored */
      VAdcProcessConversionResult(&pMeasConfig->scalingFactor,
                                  pMeasConfig->eScalingMethod,
                                  pMeasConfig->pIntTable,
                                  uPullUp,
                                  pDevCtxt->pBsp->uFullScale_code,
                                  pDevCtxt->pBsp->uFullScale_uV,
                                  *puCode,
                                  &vAdcResult);

      if (vAdcResult.eStatus != ADC_DEVICE_RESULT_VALID)
      {
         /* Result cannot be scaled */
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed calculating physical from code", FALSE);

         return DAL_ERROR;
      }

      *pnCodeToPhysical = vAdcResult.nPhysical;

      if ((!bPhysicalInverseToCode && eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER  && *pnCodeToPhysical <= nPhysical) ||
          (!bPhysicalInverseToCode && eThreshold == ADC_DEVICE_TM_THRESHOLD_HIGHER && *pnCodeToPhysical >= nPhysical) ||
          (bPhysicalInverseToCode  && eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER  && *pnCodeToPhysical >= nPhysical) ||
          (bPhysicalInverseToCode  && eThreshold == ADC_DEVICE_TM_THRESHOLD_HIGHER && *pnCodeToPhysical <= nPhysical))
      {
         /* Physical value is OK and does not need adjusted */
         return DAL_SUCCESS;
      }
      else
      {
         /* Physical value needs adjusted - use 2^N adjustment because 1 code is only 97 uV
            and thresholds won't converge without a lot of iterations */
         if (uCodeIncrement == 0)
         {
            uCodeIncrement = 1;
         }
         else
         {
            uCodeIncrement *= 2;
         }

         if (eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER)
         {
            /* Decrement code */
            *puCode = uCode - uCodeIncrement;
         }
         else
         {
            /* Increment code */
            *puCode = uCode + uCodeIncrement;
         }
      }
   }

   if ((!bPhysicalInverseToCode && eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER  && *pnCodeToPhysical <= nPhysical) ||
       (!bPhysicalInverseToCode && eThreshold == ADC_DEVICE_TM_THRESHOLD_HIGHER && *pnCodeToPhysical >= nPhysical) ||
       (bPhysicalInverseToCode  && eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER  && *pnCodeToPhysical >= nPhysical) ||
       (bPhysicalInverseToCode  && eThreshold == ADC_DEVICE_TM_THRESHOLD_HIGHER && *pnCodeToPhysical <= nPhysical))
   {
      /* Physical value is OK */
      return DAL_SUCCESS;
   }

   /* Acceptable physical value not found */
   VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed calculating physical", FALSE);

   return DAL_ERROR;
}


/*======================================================================

  FUNCTION        VAdcTM_CheckAndSwitchPullUp

  DESCRIPTION     Checks if the pull up needs switched and performs
                  the switch.

  DEPENDENCIES    None

  PARAMETERS
     pDevCtxt        [in]
     uMeasIdx        [in]
     nAggrPhysical   [in]

  RETURN VALUE    DAL_SUCCESS or an error code

  SIDE EFFECTS    None

======================================================================*/
static DALResult
VAdcTM_CheckAndSwitchPullUp(
   VAdcTMDevCtxt *pDevCtxt,
   uint32 uMeasIdx,
   int32 nAggrPhysical
   )
{
   const VAdcTMMeasConfigType *pMeasConfig;
   VAdcTMThresholdRequestType *pThresholdRequest;
   VAdcTMMeasurementType *pMeasurement;
   DALBOOL bSwitched = FALSE;
   DALBOOL bThresholdArmed = FALSE;
   DALResult retResult = DAL_SUCCESS;
   VAdcTMHalResultType status;
   uint32 uChannel = 0;
   DALResult result;
   uint32 uThresh;

   pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeasIdx];
   pMeasurement = &pDevCtxt->aThresholds[uMeasIdx];

   if (pMeasurement->ePullUpState == VADCTM_PULLUP_STATE_MED)
   {
      if (nAggrPhysical > pMeasConfig->pPullUpConfig->nMedToHigh)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_HIGH;
         uChannel = pMeasConfig->pPullUpConfig->uHighChannel;
         bSwitched = TRUE;
      }
      else if (nAggrPhysical < pMeasConfig->pPullUpConfig->nMedToLow)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_LOW;
         uChannel = pMeasConfig->pPullUpConfig->uLowChannel;
         bSwitched = TRUE;
      }
   }
   else if (pMeasurement->ePullUpState == VADCTM_PULLUP_STATE_LOW)
   {
      if (nAggrPhysical > pMeasConfig->pPullUpConfig->nMedToHigh)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_HIGH;
         uChannel = pMeasConfig->pPullUpConfig->uHighChannel;
         bSwitched = TRUE;
      }
      else if (nAggrPhysical > pMeasConfig->pPullUpConfig->nLowToMed)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_MED;
         uChannel = pMeasConfig->uAdcHardwareChannel;
         bSwitched = TRUE;
      }
   }
   else
   {
      if (nAggrPhysical < pMeasConfig->pPullUpConfig->nMedToLow)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_LOW;
         uChannel = pMeasConfig->pPullUpConfig->uLowChannel;
         bSwitched = TRUE;
      }
      else if (nAggrPhysical < pMeasConfig->pPullUpConfig->nHighToMed)
      {
         pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_MED;
         uChannel = pMeasConfig->uAdcHardwareChannel;
         bSwitched = TRUE;
      }
   }

   if (bSwitched)
   {
      status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                      uMeasIdx,
                                      VADCTM_DISABLE);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Set meas enable failed", FALSE);
         return DAL_ERROR;
      }

      status = VAdcTMHalSetChannel(&pDevCtxt->iVAdcTMHalInterface,
                                   uMeasIdx,
                                   uChannel);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set channel", TRUE);
         return DAL_ERROR;
      }

      for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
      {
         pThresholdRequest = &pMeasurement->aClientThresholds[uThresh];
         if (pThresholdRequest->eThresholdState == VADCTM_THRESHOLD_STATE_THRESHOLD)
         {
            bThresholdArmed = TRUE;

            /* Update the threshold code */
            result = VAdcTM_GetAdcCode(pDevCtxt,
                                       uMeasIdx,
                                       (AdcDeviceTMThresholdType)uThresh,
                                       pThresholdRequest->nPhysicalDesired,
                                       &pThresholdRequest->uCode,
                                       &pThresholdRequest->nPhysicalMonitored);
            if (result != DAL_SUCCESS)
            {
               retResult = result;
            }

            if (((AdcDeviceTMThresholdType)uThresh) == ADC_DEVICE_TM_THRESHOLD_LOWER)
            {
               status = VAdcTMHalSetLowThresholdCode(&pDevCtxt->iVAdcTMHalInterface,
                                                     uMeasIdx,
                                                     pThresholdRequest->uCode);
               if (status != VADCTM_HAL_SUCCESS)
               {
                  VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh code", FALSE);
                  retResult = result;
               }
            }
            else
            {
               status = VAdcTMHalSetHighThresholdCode(&pDevCtxt->iVAdcTMHalInterface,
                                                      uMeasIdx,
                                                      pThresholdRequest->uCode);
               if (status != VADCTM_HAL_SUCCESS)
               {
                  VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh code", FALSE);
                  retResult = result;
               }
            }
         }
      }

      if (pMeasConfig->bAlwaysOn || bThresholdArmed)
      {
         status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                         uMeasIdx,
                                         VADCTM_ENABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Set meas enable failed", FALSE);
            return DAL_ERROR;
         }
      }

      VAdcTM_LogPullUpSwitched(&pDevCtxt->debug,
                               uMeasIdx,
                               (uint32)pMeasurement->ePullUpState);
   }

   return retResult;
}

/*======================================================================

  FUNCTION        VAdcTM_ServiceThresholdEventsInWorkLoop

  DESCRIPTION
      This function services threshold events in a workloop.

  DEPENDENCIES    None

  PARAMETERS
      hEvent     [in]
      pInDevCtxt [in]

  RETURN VALUE    Infinite loop

  SIDE EFFECTS    Completes the client notification

======================================================================*/
static DALResult
VAdcTM_ServiceThresholdEventsInWorkLoop(
   DALSYSEventHandle hEvent,
   void *pInDevCtxt
   )
{
   VAdcTMDevCtxt *pDevCtxt = pInDevCtxt;
   const VAdcTMMeasConfigType *pMeasConfig;
   VAdcTMMeasurementType *pMeasurement;
   VAdcTMHalResultType status;
   VAdcTMThresholdRequestType *pThresholdRequest;
   AdcDeviceResultType vAdcResult;
   uint32 uMeas, uThresh;
   VAdcTMStatusType convStatus;
   uint32 *puCode;
   uint32 uThresholdMask;
   DALBOOL bTriggered;
   AdcTMCallbackPayloadType adcTMCallbackPayload;
   uint32 uPullUp;

   DALSYS_EventCtrl(pDevCtxt->hSignalEvent, DALSYS_EVENT_CTRL_ACCQUIRE_OWNERSHIP);

   for ( ; ; )
   {
      /* Wait on the interrupt event */
      (void)DALSYS_EventWait(pDevCtxt->hSignalEvent);

      DALSYS_SyncEnter(pDevCtxt->hSync);

      DALSYS_EventCtrl(pDevCtxt->hSignalEvent, DALSYS_EVENT_CTRL_RESET);

      status = VAdcTMHalClearInterrupts(&pDevCtxt->iVAdcTMHalInterface,
                                        VADCTM_INT_MASK_THR);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to clear interrupt", FALSE);
      }

      status = VAdcTMHalSetDataHold(&pDevCtxt->iVAdcTMHalInterface, VADCTM_HOLD_DATA);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to hold data", FALSE);
         (void)VAdcTMHalSetDataHold(&pDevCtxt->iVAdcTMHalInterface, VADCTM_FREE_RUNNING);
         goto updateThreshReleaseLockCont;
      }

      status = VAdcTMHalGetStatus(&pDevCtxt->iVAdcTMHalInterface, &convStatus);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to get conv result", FALSE);
         (void)VAdcTMHalSetDataHold(&pDevCtxt->iVAdcTMHalInterface, VADCTM_FREE_RUNNING);
         goto updateThreshReleaseLockCont;
      }

      /* Read each threshold code */
      for (uMeas = 0; uMeas < *pDevCtxt->pBsp->puNumMeas; uMeas++)
      {
         pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeas];
         puCode = &pDevCtxt->aThresholds[uMeas].uLatestCode;

         if ((convStatus.uLowThresholdStatus | convStatus.uHighThresholdStatus) & (1 << uMeas))
         {
            status = VAdcTMHalGetConversionCode(&pDevCtxt->iVAdcTMHalInterface,
                                                uMeas,
                                                puCode);
            if (status != VADCTM_HAL_SUCCESS)
            {
               VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to get conv code", FALSE);
               goto updateThreshReleaseLockCont;
            }

            if (*puCode == VADCTM_INVALID_ADC_CODE)
            {
               /* If code is invalid then clear the status bits for this meas */
               convStatus.uLowThresholdStatus &= ~(1 << uMeas);
               convStatus.uHighThresholdStatus &= ~(1 << uMeas);
            }
            else if (((int16)(*puCode)) < 0)
            {
               /* Code is 16-bit signed and could read negative when close to zero */
               *puCode = 0;
            }
         }
      }

      status = VAdcTMHalSetDataHold(&pDevCtxt->iVAdcTMHalInterface, VADCTM_FREE_RUNNING);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set data running", FALSE);
         (void)VAdcTMHalSetDataHold(&pDevCtxt->iVAdcTMHalInterface, VADCTM_FREE_RUNNING);
         goto updateThreshReleaseLockCont;
      }

      /* Determine which thresholds are crossed */
      for (uMeas = 0; uMeas < *pDevCtxt->pBsp->puNumMeas; uMeas++)
      {
         pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeas];
         pMeasurement = &pDevCtxt->aThresholds[uMeas];
         uPullUp = VAdcTM_GetPullUp(pDevCtxt, uMeas);

         if ((convStatus.uLowThresholdStatus | convStatus.uHighThresholdStatus) & (1 << uMeas))
         {
            VAdcProcessConversionResult(&pMeasConfig->scalingFactor,
                                        pMeasConfig->eScalingMethod,
                                        pMeasConfig->pIntTable,
                                        uPullUp,
                                        pDevCtxt->pBsp->uFullScale_code,
                                        pDevCtxt->pBsp->uFullScale_uV,
                                        pMeasurement->uLatestCode,
                                        &vAdcResult);

            if (vAdcResult.eStatus != ADC_DEVICE_RESULT_VALID)
            {
               VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to process result", FALSE);
               continue;
            }

            pMeasurement->nCurrentValue = vAdcResult.nPhysical;
         }
         else
         {
            continue;
         }

         for (uThresh = 0; uThresh < ADC_DEVICE_TM_NUM_THRESHOLDS; uThresh++)
         {
            switch ((AdcDeviceTMThresholdType)uThresh)
            {
               case ADC_DEVICE_TM_THRESHOLD_LOWER:
                  uThresholdMask = convStatus.uLowThresholdStatus;
                  break;
               case ADC_DEVICE_TM_THRESHOLD_HIGHER:
                  uThresholdMask = convStatus.uHighThresholdStatus;
                  break;
               default:
                  uThresholdMask = 0;
            }

            if (uThresholdMask & (1 << uMeas))
            {
               pThresholdRequest = &pMeasurement->aClientThresholds[uThresh];

               if (pThresholdRequest->eThresholdState == VADCTM_THRESHOLD_STATE_THRESHOLD)
               {
                  bTriggered = FALSE;

                  switch ((AdcDeviceTMThresholdType)uThresh)
                  {
                     case ADC_DEVICE_TM_THRESHOLD_LOWER:

                        if (vAdcResult.uCode <= pThresholdRequest->uCode)
                        {
                           bTriggered = TRUE;

                           pDevCtxt->uLowMeasEnMask &= ~(1 << uMeas);

                           if (!pDevCtxt->aThresholds[uMeas].bPhysicalInverseToCode)
                           {
                              adcTMCallbackPayload.eThresholdTriggered = ADC_TM_THRESHOLD_LOWER;
                           }
                           else
                           {
                              adcTMCallbackPayload.eThresholdTriggered = ADC_TM_THRESHOLD_HIGHER;
                           }

                           status = VAdcTMHalSetMeasLowThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                                                 uMeas,
                                                                 VADCTM_DISABLE);
                           if (status != VADCTM_HAL_SUCCESS)
                           {
                              VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh enable", FALSE);
                           }
                        }

                        break;

                     case ADC_DEVICE_TM_THRESHOLD_HIGHER:

                        if (vAdcResult.uCode >= pThresholdRequest->uCode)
                        {
                           bTriggered = TRUE;

                           pDevCtxt->uHighMeasEnMask &= ~(1 << uMeas);

                           if (!pDevCtxt->aThresholds[uMeas].bPhysicalInverseToCode)
                           {
                              adcTMCallbackPayload.eThresholdTriggered = ADC_TM_THRESHOLD_HIGHER;
                           }
                           else
                           {
                              adcTMCallbackPayload.eThresholdTriggered = ADC_TM_THRESHOLD_LOWER;
                           }

                           status = VAdcTMHalSetMeasHighThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                                                  uMeas,
                                                                  VADCTM_DISABLE);
                           if (status != VADCTM_HAL_SUCCESS)
                           {
                              VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh enable", FALSE);
                           }
                        }

                        break;

                     default:
                        bTriggered = FALSE;
                  }

                  if (bTriggered)
                  {
                     if (!pMeasConfig->bAlwaysOn)
                     {
                        if (((pDevCtxt->uLowMeasEnMask | pDevCtxt->uHighMeasEnMask) & (1 << uMeas)) == 0)
                        {
                           status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                                           uMeas,
                                                           VADCTM_DISABLE);
                           if (status != VADCTM_HAL_SUCCESS)
                           {
                              VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Set meas enable failed", FALSE);
                           }
                        }
                     }

                     VAdcTM_LogThresholdTriggered(&pDevCtxt->debug,
                                                  uMeas,
                                                  (AdcDeviceTMThresholdType)uThresh,
                                                  vAdcResult.nPhysical);

                     pThresholdRequest->eThresholdState = VADCTM_THRESHOLD_STATE_TRIGGERED;

                     DALSYS_SyncLeave(pDevCtxt->hSync);

                     /* Set the payload */
                     adcTMCallbackPayload.adcTMInputProps.uDeviceIdx = pDevCtxt->uDeviceIndex;
                     adcTMCallbackPayload.adcTMInputProps.uChannelIdx = uMeas;
                     adcTMCallbackPayload.nPhysicalTriggered = vAdcResult.nPhysical;

                     DALSYS_EventCtrlEx(pThresholdRequest->hEvent,
                                        DALSYS_EVENT_CTRL_TRIGGER,
                                        NULL,
                                        (void *)&adcTMCallbackPayload,
                                        sizeof(adcTMCallbackPayload));

                     DALSYS_SyncEnter(pDevCtxt->hSync);
                  }
               }
            }
         }
      }

updateThreshReleaseLockCont:
      DALSYS_SyncLeave(pDevCtxt->hSync);
   }
}

/*======================================================================

  FUNCTION        VAdcTM_InterruptCb

  DESCRIPTION
      This function is the callback to process the threshold interrupt.

  DEPENDENCIES    None

  PARAMETERS
      pArg            [in]
      uIntrStatusMask [in]

  RETURN VALUE    None

  SIDE EFFECTS    None

======================================================================*/
static void*
VAdcTM_InterruptCb(
   void *pArg,
   uint32 uIntrStatusMask
   )
{
   VAdcTMDevCtxt *pDevCtxt = pArg;

   VAdcTM_LogInterrupt(&pDevCtxt->debug);

   DALSYS_EventCtrl(pDevCtxt->hSignalEvent, DALSYS_EVENT_CTRL_TRIGGER);

   return NULL;
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * The following functions are for DALDriver specific functionality
 * -------------------------------------------------------------------------*/
DALResult
VAdcTM_DriverInit(VAdcTMDrvCtxt *pCtxt)
{
   return DAL_SUCCESS;
}

DALResult
VAdcTM_DriverDeInit(VAdcTMDrvCtxt *pCtxt)
{
   return DAL_SUCCESS;
}

/*----------------------------------------------------------------------------
 * The following functions are declared in DalDevice Interface.
 * -------------------------------------------------------------------------*/
DALResult
VAdcTM_DeviceInit(VAdcTMClientCtxt *pCtxt)
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;
   VAdcTMInterruptConfigType eInterruptConfig;
   VAdcTMConversionParametersType convParams;
   VAdcTMMeasIntervalCtlType measIntervalCtl;
   const VAdcTMMeasConfigType *pMeasConfig;
   VAdcTMMeasurementType *pMeasurement;
   VAdcTMMeasParametersType measParams;
   AdcDeviceResultType minResult;
   AdcDeviceResultType maxResult;
   VAdcTMHalResultType status;
   DALSYSPropertyVar propVar;
   VAdcTMEnableType eEnable;
   uint32 uMeasIdx;
   DALResult ret;
#ifndef VADC_UNIT_TEST
   const VAdcTMGpioChannelMappingType *pGpioChannelMapping;
   uint32 uGpioMappingIdx;
   uint32 uGpioChannel;
   DALBOOL bGpioFound;
   pm_err_flag_type pmResult;
#endif

   ret = VAdcTM_LogInit(&pDevCtxt->debug, pDevCtxt->DevId);
   if (ret != DAL_SUCCESS)
   {
      return ret;
   }

   if (DALPLATFORMINFO_TYPE_RUMI == DalPlatformInfo_Platform() &&
       !gVAdcTMOverrideRumiCheck)
   {
      pm_model_type pmic_model = pm_get_pmic_model(0);

      if (pmic_model == PMIC_IS_UNKNOWN || pmic_model == PMIC_IS_INVALID)
      {
         /* PMIC is not supported on this RUMI */
         pDevCtxt->bHardwareSupported = FALSE;
         return DAL_SUCCESS;
      }
   }

   pDevCtxt->bHardwareSupported = TRUE;

   /* Read the DAL properties - hProp populated in device attach */
   ret = DALSYS_GetPropertyValue(pDevCtxt->hProp, "VADCTM_BSP", 0, &propVar);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to get BSP", TRUE);
      return ret;
   }

   pDevCtxt->pBsp = (VAdcTMBspType *)propVar.Val.pStruct;
   
   /*Check whether TM module is supported on the PMIC
     Not supported for Cocos PMIC
   */
   pm_model_type pmic_model = pm_get_pmic_model(pDevCtxt->pBsp->ucPmicDevice);
   if (pmic_model == PMIC_IS_UNKNOWN || pmic_model == PMIC_IS_INVALID || pmic_model == PMIC_IS_PM2250)
   {
      /* TM module is not supported on this PMIC or PMIC is INVALID*/
      pDevCtxt->bHardwareSupported = FALSE;
      return DAL_SUCCESS;
   }

#ifndef VADC_UNIT_TEST
   /* Get the PMIC device info */
   pmResult = pm_get_pmic_info(pDevCtxt->pBsp->ucPmicDevice, &pDevCtxt->pmicDeviceInfo);
   if (pmResult != PM_ERR_FLAG__SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to get PMIC device info", TRUE);
      return DAL_ERROR;
   }
#endif

   /* Attach to to the SPMI driver */
   ret = AdcSpmiInit(&pDevCtxt->spmiCtxt,
                     pDevCtxt->pBsp->eAccessPriority,
                     pDevCtxt->pBsp->uSlaveId,
                     pDevCtxt->pBsp->uPeripheralId);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to attach to SPMI", TRUE);
      return ret;
   }

   /* Initialize the HAL interface */
   pDevCtxt->iVAdcTMHalInterface.pCtxt = (void *)pDevCtxt;
   pDevCtxt->iVAdcTMHalInterface.pfnWriteBytes = VAdcTM_WriteBytes;
   pDevCtxt->iVAdcTMHalInterface.pfnReadBytes = VAdcTM_ReadBytes;

   status = VAdcTMHalGetRevisionInfo(&pDevCtxt->iVAdcTMHalInterface, &pDevCtxt->revisionInfo);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to get revision info", TRUE);
      return DAL_ERROR;
   }

   /* Initialize synchronization object */
   ret = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE,
                           &pDevCtxt->hSync,
                           &pDevCtxt->syncObject);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to create the sync object", TRUE);
      return ret;
   }

   /* Create a work loop to wait on threshold events */
   ret = DALSYS_RegisterWorkLoopEx("VADCTM_WORK_LOOP",
                                   VADCTM_DEFAULT_STACK_SIZE,
                                   gVAdcTMWorkloopPriority,
                                   1,
                                   &pDevCtxt->hThresholdWorkLoop,
                                   NULL);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to create workloop", TRUE);
      return ret;
   }

   /* Create a workloop event */
   ret = DALSYS_EventCreate(DALSYS_EVENT_ATTR_WORKLOOP_EVENT,
                            &pDevCtxt->hWorkLoopEvent,
                            NULL);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to create workloop event", TRUE);
      return ret;
   }

   /* Add the event to the workloop */
   ret = DALSYS_AddEventToWorkLoop(pDevCtxt->hThresholdWorkLoop,
                                   VAdcTM_ServiceThresholdEventsInWorkLoop,
                                   pDevCtxt,
                                   pDevCtxt->hWorkLoopEvent,
                                   NULL);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to add event to workloop", TRUE);
      return ret;
   }

   /* Create the event used to signal the threshold work loop */
   ret = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                            &pDevCtxt->hSignalEvent,
                            &pDevCtxt->signalEventObject);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to create signal", TRUE);
      return ret;
   }
   DALSYS_EventCtrl(pDevCtxt->hSignalEvent, DALSYS_EVENT_CTRL_RESET);

   /* Configure interrupts */
   ret = AdcSpmiRegisterInterrupt(&pDevCtxt->spmiCtxt,
                                  VADCTM_INT_MASK_THR,
                                  VAdcTM_InterruptCb,
                                  pDevCtxt);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to register ISR", TRUE);
      return ret;
   }

   /* Configure peripheral interrupts */
   eInterruptConfig = VADCTM_INTERRUPT_CONFIG_RISING_EDGE;

   status = VAdcTMHalConfigInterrupts(&pDevCtxt->iVAdcTMHalInterface,
                                      VADCTM_INT_MASK_THR,
                                      eInterruptConfig);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to config interrupts", TRUE);
      return DAL_ERROR;
   }

   status = VAdcTMHalSetInterruptMid(&pDevCtxt->iVAdcTMHalInterface,
                                     pDevCtxt->pBsp->uMasterID);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed setting MID", TRUE);
      return DAL_ERROR;
   }

   status = VAdcTMHalClearInterrupts(&pDevCtxt->iVAdcTMHalInterface,
                                     VADCTM_INT_MASK_THR);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to clear interrupts", TRUE);
      return DAL_ERROR;
   }

   status = VAdcTMHalEnableInterrupts(&pDevCtxt->iVAdcTMHalInterface,
                                      VADCTM_INT_MASK_THR);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to enable interrupts", TRUE);
      return DAL_ERROR;
   }

   /* Set conversion parameters */
   convParams.eDecimationRatio = pDevCtxt->pBsp->pCommonConfig->eDecimationRatio;
   convParams.eAverageMode = pDevCtxt->pBsp->pCommonConfig->eAverageMode;

   status = VAdcTMHalSetConversionParameters(&pDevCtxt->iVAdcTMHalInterface, &convParams);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed setting conv params", TRUE);
      return DAL_ERROR;
   }

   /* Set measurement intervals */
   measIntervalCtl.eMeasIntervalTime1 = pDevCtxt->pBsp->pCommonConfig->eMeasIntervalTime1;
   measIntervalCtl.eMeasIntervalTime2 = pDevCtxt->pBsp->pCommonConfig->eMeasIntervalTime2;
   measIntervalCtl.eMeasIntervalTime3 = pDevCtxt->pBsp->pCommonConfig->eMeasIntervalTime3;

   status = VAdcTMHalSetIntervalCtl(&pDevCtxt->iVAdcTMHalInterface, &measIntervalCtl);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed setting meas ctrl", TRUE);
      return DAL_ERROR;
   }

   pDevCtxt->uLowMeasEnMask = 0;
   pDevCtxt->uHighMeasEnMask = 0;

   /* Configure each of the measurements */
   for (uMeasIdx = 0; uMeasIdx < *pDevCtxt->pBsp->puNumMeas; uMeasIdx++)
   {
      pMeasurement = &pDevCtxt->aThresholds[uMeasIdx];
      pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeasIdx];

      pMeasurement->ePullUpState = VADCTM_PULLUP_STATE_MED;

      /* Determine range and if code is inverse to physical value */
      VAdcProcessConversionResultInverse(&pMeasConfig->scalingFactor,
                                         pMeasConfig->eScalingMethod,
                                         pMeasConfig->pIntTable,
                                         pMeasConfig->uPullUp,  /* During init pull up is medium by default */
                                         pDevCtxt->pBsp->uFullScale_code,
                                         pDevCtxt->pBsp->uFullScale_uV,
                                         pMeasConfig->nPhysicalMin,
                                         &minResult);

      if (minResult.eStatus != ADC_DEVICE_RESULT_VALID)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to scale min threshold", TRUE);
         return DAL_ERROR;
      }

      VAdcProcessConversionResultInverse(&pMeasConfig->scalingFactor,
                                         pMeasConfig->eScalingMethod,
                                         pMeasConfig->pIntTable,
                                         pMeasConfig->uPullUp,  /* During init pull up is medium by default */
                                         pDevCtxt->pBsp->uFullScale_code,
                                         pDevCtxt->pBsp->uFullScale_uV,
                                         pMeasConfig->nPhysicalMax,
                                         &maxResult);

      if (maxResult.eStatus != ADC_DEVICE_RESULT_VALID)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to scale max threshold", TRUE);
         return DAL_ERROR;
      }

      if (minResult.uCode > maxResult.uCode)
      {
         pMeasurement->bPhysicalInverseToCode = TRUE;
      }
      else
      {
         pMeasurement->bPhysicalInverseToCode = FALSE;
      }

      /* Set all thresholds to disabled */
      pMeasurement->aClientThresholds[ADC_DEVICE_TM_THRESHOLD_LOWER].eThresholdState =
         VADCTM_THRESHOLD_STATE_DISABLED;
      pMeasurement->aClientThresholds[ADC_DEVICE_TM_THRESHOLD_HIGHER].eThresholdState =
         VADCTM_THRESHOLD_STATE_DISABLED;

      /* Configure peripheral measurement settings */
      status = VAdcTMHalSetChannel(&pDevCtxt->iVAdcTMHalInterface,
                                   uMeasIdx,
                                   pMeasConfig->uAdcHardwareChannel);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set channel", TRUE);
         return DAL_ERROR;
      }

      measParams.eCalMethod = pMeasConfig->eCalMethod;
      measParams.eMeasIntervalTimeSelect = pMeasConfig->eMeasIntervalTimeSelect;
      measParams.eSettlingDelay = pMeasConfig->eSettlingDelay;
      status = VAdcTMHalSetMeasParameters(&pDevCtxt->iVAdcTMHalInterface,
                                          uMeasIdx,
                                          &measParams);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set measurement params", TRUE);
         return DAL_ERROR;
      }

      status = VAdcTMHalSetMeasLowThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                            uMeasIdx,
                                            VADCTM_DISABLE);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh enable", TRUE);
         return DAL_ERROR;
      }

      status = VAdcTMHalSetMeasHighThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                             uMeasIdx,
                                             VADCTM_DISABLE);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh enable", TRUE);
         return DAL_ERROR;
      }

      if (pMeasConfig->bAlwaysOn)
      {
         eEnable = VADCTM_ENABLE;
      }
      else
      {
         eEnable = VADCTM_DISABLE;
      }

      status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                      uMeasIdx,
                                      eEnable);
      if (status != VADCTM_HAL_SUCCESS)
      {
         VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set measurement enable", TRUE);
         return DAL_ERROR;
      }
   }

#ifndef VADC_UNIT_TEST
   /* Map static GPIOs */
   for (uMeasIdx = 0; uMeasIdx < *pDevCtxt->pBsp->puNumMeas; uMeasIdx++)
   {
      pMeasConfig = &pDevCtxt->pBsp->paMeasConfig[uMeasIdx];
      bGpioFound = FALSE;

      for (uGpioMappingIdx = 0; uGpioMappingIdx < pDevCtxt->pBsp->uNumGpioChannelMappings; uGpioMappingIdx++)
      {
         pGpioChannelMapping = &pDevCtxt->pBsp->paGpioChannelMappings[uGpioMappingIdx];

         for (uGpioChannel = 0; uGpioChannel < pGpioChannelMapping->uNumChannels; uGpioChannel++)
         {
            if (pMeasConfig->uAdcHardwareChannel == pGpioChannelMapping->paucChannels[uGpioChannel])
            {
               bGpioFound = TRUE;
               break;
            }
         }

         if (bGpioFound)
         {
            pmResult = pm_gpio_set_ext_pin_config(pDevCtxt->pBsp->ucPmicDevice,
                                                  pGpioChannelMapping->eGpio,
                                                  PM_GPIO_EXT_PIN_DISABLE);
            if (pmResult != PM_ERR_FLAG__SUCCESS)
            {
               VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to configure static GPIO", TRUE);
               return DAL_ERROR;
            }

            break;
         }
      }
   }
#endif

   status = VAdcTMHalSetEnable(&pDevCtxt->iVAdcTMHalInterface,
                               VADCTM_ENABLE);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Enable failed", TRUE);
      return DAL_ERROR;
   }

   /* Request conversion strobe - FSM will stay idle until a meas is enabled */
   status = VAdcTMHalRequestConversion(&pDevCtxt->iVAdcTMHalInterface);
   if (status != VADCTM_HAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Req conv failed", TRUE);
      return DAL_ERROR;
   }

   /* Start the work loop that processes threshold events */
   ret = DALSYS_EventCtrlEx(pDevCtxt->hWorkLoopEvent,
                            DALSYS_EVENT_CTRL_TRIGGER,
                            0,
                            NULL,
                            0);
   if (ret != DAL_SUCCESS)
   {
      VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - failed to trigger workloop", TRUE);
      return ret;
   }

   return DAL_SUCCESS;
}

DALResult
VAdcTM_DeviceDeInit(VAdcTMClientCtxt *pCtxt)
{
   /* ADC is marked as static - deinit is never called */
   return DAL_SUCCESS;
}

/*----------------------------------------------------------------------------
 * The following functions are extended in DalAdcDevice Interface.
 *
 * These functions are documented in DDIAdcDevice.h
 * -------------------------------------------------------------------------*/
DALResult
VAdcTM_SetDeviceIndex(
   VAdcTMClientCtxt *pCtxt,
   uint32 uDeviceIndex
   )
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;

   pDevCtxt->uDeviceIndex = uDeviceIndex;

   return DAL_SUCCESS;
}

DALResult
VAdcTM_GetDeviceProperties(
   VAdcTMClientCtxt *pCtxt,
   AdcDeviceDevicePropertiesType *pAdcDeviceProp
   )
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;

   if (pDevCtxt->bHardwareSupported)
   {
      pAdcDeviceProp->uNumMeas = *pDevCtxt->pBsp->puNumMeas;
   }
   else
   {
      pAdcDeviceProp->uNumMeas = 0;
   }

   pAdcDeviceProp->uNumChannels = 0;

   return DAL_SUCCESS;
}

DALResult
VAdcTM_GetChannel(
   VAdcTMClientCtxt *pCtxt,
   const char *pszChannelName,
   uint32 *puChannelIdx
   )
{
   return DAL_ERROR;
}

DALResult
VAdcTM_ReadChannel(
   VAdcTMClientCtxt *pCtxt,
   uint32 uChannelIdx,
   AdcDeviceResultType *pAdcDeviceResult
   )
{
   return DAL_ERROR;
}

DALResult
VAdcTM_RecalibrateChannel(
   VAdcTMClientCtxt *pCtxt,
   uint32 uChannelIdx,
   AdcDeviceRecalibrateResultType *pAdcDeviceRecalibrateResult
   )
{
   return DAL_ERROR;
}

DALResult
VAdcTM_GetInputProperties(
   VAdcTMClientCtxt *pCtxt,
   const char *pInputName,
   uint32 *puMeasIdx
   )
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;
   uint32 uNumMeas;
   uint32 uMeas;
   const char *pszCurrentMeasName;

   if (!pDevCtxt->bHardwareSupported)
   {
      return DAL_ERROR;
   }

   uNumMeas = *pDevCtxt->pBsp->puNumMeas;

   for (uMeas = 0; uMeas < uNumMeas; uMeas++)
   {
      pszCurrentMeasName = pDevCtxt->pBsp->paMeasConfig[uMeas].pszName;

      if (strcmp(pInputName, pszCurrentMeasName) == 0)
      {
         *puMeasIdx = uMeas;
         return DAL_SUCCESS;
      }
   }

   return DAL_ERROR;
}

DALResult
VAdcTM_GetRange(
   VAdcTMClientCtxt *pCtxt,
   uint32 uMeasIdx,
   AdcDeviceTMRangeType *pAdcDeviceTMRange
   )
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;

   if (!pDevCtxt->bHardwareSupported)
   {
      return DAL_ERROR;
   }

   if (uMeasIdx >= *pDevCtxt->pBsp->puNumMeas)
   {
      return ADC_DEVICE_ERROR_TM_INVALID_MEAS_IDX;
   }

   pAdcDeviceTMRange->nPhysicalMin = pDevCtxt->pBsp->paMeasConfig[uMeasIdx].nPhysicalMin;
   pAdcDeviceTMRange->nPhysicalMax = pDevCtxt->pBsp->paMeasConfig[uMeasIdx].nPhysicalMax;

   return DAL_SUCCESS;
}

DALResult
VAdcTM_SetThreshold(
   VAdcTMClientCtxt *pCtxt,
   uint32 uMeasIdx,
   const DALSYSEventHandle hEvent,
   AdcDeviceTMThresholdType eThreshold,
   const int32 *pnThresholdDesired,
   int32 *pnThresholdSet
   )
{
   VAdcTMDevCtxt *pDevCtxt = pCtxt->pVAdcTMDevCtxt;
   VAdcTMThresholdRequestType *pTempThresholdRequest;
   VAdcTMThresholdRequestType *pThresholdRequest;
   VAdcTMMeasurementType *pMeasurement;
   int32 nAggrPhysical;
   DALResult result = DAL_SUCCESS;
   VAdcTMHalResultType status;
   uint32 uCode = 0;

   if (!pDevCtxt->bHardwareSupported)
   {
      return DAL_ERROR;
   }

   DALSYS_SyncEnter(pDevCtxt->hSync);

   pMeasurement = &pDevCtxt->aThresholds[uMeasIdx];

   /* Adjust for channels whose physical values vary inversely with code */
   if (pMeasurement->bPhysicalInverseToCode)
   {
      /* Flip the threshold type */
      if (eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER)
      {
         eThreshold = ADC_DEVICE_TM_THRESHOLD_HIGHER;
      }
      else
      {
         eThreshold = ADC_DEVICE_TM_THRESHOLD_LOWER;
      }
   }

   pThresholdRequest = &pMeasurement->aClientThresholds[eThreshold];

   if (pnThresholdDesired != NULL)
   {
      if (*pnThresholdDesired < pDevCtxt->pBsp->paMeasConfig[uMeasIdx].nPhysicalMin ||
          *pnThresholdDesired > pDevCtxt->pBsp->paMeasConfig[uMeasIdx].nPhysicalMax)
      {
         result = ADC_DEVICE_ERROR_TM_THRESHOLD_OUT_OF_RANGE;
         goto relLockReturn;
      }

      /* Check whether the pull up needs switched */
      if (pDevCtxt->pBsp->paMeasConfig[uMeasIdx].pPullUpConfig != NULL)
      {
         if (eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER)
         {
            pTempThresholdRequest = &pMeasurement->aClientThresholds[ADC_DEVICE_TM_THRESHOLD_HIGHER];
            if (pTempThresholdRequest->eThresholdState == VADCTM_THRESHOLD_STATE_THRESHOLD)
            {
               nAggrPhysical = (*pnThresholdDesired + pTempThresholdRequest->nPhysicalDesired) / 2;
            }
            else
            {
               nAggrPhysical = *pnThresholdDesired;
            }
         }
         else
         {
            pTempThresholdRequest = &pMeasurement->aClientThresholds[ADC_DEVICE_TM_THRESHOLD_LOWER];
            if (pTempThresholdRequest->eThresholdState == VADCTM_THRESHOLD_STATE_THRESHOLD)
            {
               nAggrPhysical = (*pnThresholdDesired + pTempThresholdRequest->nPhysicalDesired) / 2;
            }
            else
            {
               nAggrPhysical = *pnThresholdDesired;
            }
         }

         result = VAdcTM_CheckAndSwitchPullUp(pDevCtxt,
                                              uMeasIdx,
                                              nAggrPhysical);
         if (result != DAL_SUCCESS)
         {
            goto relLockReturn;
         }
      }

      result = VAdcTM_GetAdcCode(pDevCtxt,
                                 uMeasIdx,
                                 eThreshold,
                                 *pnThresholdDesired,
                                 &uCode,
                                 pnThresholdSet);
      if (result != DAL_SUCCESS)
      {
         goto relLockReturn;
      }
   }

   /* Set the threshold */
   if (pnThresholdDesired == NULL)
   {
      /* Cancel the threshold */
      pThresholdRequest->eThresholdState = VADCTM_THRESHOLD_STATE_DISABLED;

      VAdcTM_LogThresholdCleared(&pDevCtxt->debug,
                                 uMeasIdx,
                                 eThreshold);

      if (eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER)
      {
         pDevCtxt->uLowMeasEnMask &= ~(1 << uMeasIdx);

         status = VAdcTMHalSetMeasLowThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                               uMeasIdx,
                                               VADCTM_DISABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Set meas enable failed", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }
      }
      else
      {
         pDevCtxt->uHighMeasEnMask &= ~(1 << uMeasIdx);

         status = VAdcTMHalSetMeasHighThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                                uMeasIdx,
                                                VADCTM_DISABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh enable", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }
      }

      if (!pDevCtxt->pBsp->paMeasConfig[uMeasIdx].bAlwaysOn)
      {
         if (((pDevCtxt->uLowMeasEnMask | pDevCtxt->uHighMeasEnMask) & (1 << uMeasIdx)) == 0)
         {
            status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                            uMeasIdx,
                                            VADCTM_DISABLE);
            if (status != VADCTM_HAL_SUCCESS)
            {
               VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh enable", FALSE);
               result = DAL_ERROR;
               goto relLockReturn;
            }
         }
      }
   }
   else
   {
      /* Set the new threshold */
      pThresholdRequest->eThresholdState = VADCTM_THRESHOLD_STATE_THRESHOLD;
      pThresholdRequest->hEvent = hEvent;
      pThresholdRequest->nPhysicalDesired = *pnThresholdDesired;
      pThresholdRequest->nPhysicalMonitored = *pnThresholdSet;
      pThresholdRequest->uCode = uCode;

      VAdcTM_LogThresholdSet(&pDevCtxt->debug,
                             uMeasIdx,
                             eThreshold,
                             *pnThresholdDesired,
                             *pnThresholdSet);

      if (eThreshold == ADC_DEVICE_TM_THRESHOLD_LOWER)
      {
         pDevCtxt->uLowMeasEnMask |= 1 << uMeasIdx;

         status = VAdcTMHalSetLowThresholdCode(&pDevCtxt->iVAdcTMHalInterface,
                                               uMeasIdx,
                                               uCode);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh code", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }

         status = VAdcTMHalSetMeasLowThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                               uMeasIdx,
                                               VADCTM_ENABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set low thresh enable", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }
      }
      else
      {
         pDevCtxt->uHighMeasEnMask |= 1 << uMeasIdx;

         status = VAdcTMHalSetHighThresholdCode(&pDevCtxt->iVAdcTMHalInterface,
                                                uMeasIdx,
                                                uCode);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh code", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }

         status = VAdcTMHalSetMeasHighThrEnable(&pDevCtxt->iVAdcTMHalInterface,
                                                uMeasIdx,
                                                VADCTM_ENABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Failed to set high thresh enable", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }
      }

      if (!pDevCtxt->pBsp->paMeasConfig[uMeasIdx].bAlwaysOn)
      {
         status = VAdcTMHalSetMeasEnable(&pDevCtxt->iVAdcTMHalInterface,
                                         uMeasIdx,
                                         VADCTM_ENABLE);
         if (status != VADCTM_HAL_SUCCESS)
         {
            VAdcTM_LogError(&pDevCtxt->debug, "VAdcTM - Set meas enable failed", FALSE);
            result = DAL_ERROR;
            goto relLockReturn;
         }
      }
   }

relLockReturn:
   DALSYS_SyncLeave(pDevCtxt->hSync);

   return result;
}

