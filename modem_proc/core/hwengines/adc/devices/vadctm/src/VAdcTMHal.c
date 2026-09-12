/*============================================================================
  FILE:         VAdcTMHal.c

  OVERVIEW:     Implementation of device HAL for VADCTM

  DEPENDENCIES: None

                Copyright (c) 2012, 2014-2015, 2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/VAdcTMHal.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-03-20  KS   Updated for Talos
  2015-08-12  jjo  Updated for VADC refresh.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcTMHal.h"
#include "VAdcTMHalPmio.h"
#include "pm_pmio_p.h"


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

#ifdef ADC_USE_PM_4_REGS
#undef PMIO_STATUS2_ADDR
#undef PMIO_DATA_HOLD_CTL_ADDR
#undef PMIO_MEAS_INTERVAL_CTL_ADDR
#undef PMIO_MEAS_INTERVAL_CTL2_ADDR
#define PMIO_STATUS2_ADDR(x)            ((x) + 0x00000008)
#define PMIO_DATA_HOLD_CTL_ADDR(x)      ((x) + 0x0000003f)
#define PMIO_MEAS_INTERVAL_CTL_ADDR(x)  ((x) + 0x00000050)
#define PMIO_MEAS_INTERVAL_CTL2_ADDR(x) ((x) + 0x00000051)
#endif

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
static const uint8 gaucRegDumpAddresses[VADCTM_HAL_NUM_REGISTERS_TO_DUMP] =
{
   PMIO_ADDRX(0, REVISION1),
   PMIO_ADDRX(0, REVISION2),
   PMIO_ADDRX(0, REVISION3),
   PMIO_ADDRX(0, REVISION4),
   PMIO_ADDRX(0, PERPH_TYPE),
   PMIO_ADDRX(0, PERPH_SUBTYPE),
   PMIO_ADDRX(0, STATUS2),
   PMIO_ADDRX(0, STATUS_LOW),
   PMIO_ADDRX(0, STATUS_HIGH),
   PMIO_ADDRX(0, INT_RT_STS),
   PMIO_ADDRX(0, INT_SET_TYPE),
   PMIO_ADDRX(0, INT_POLARITY_HIGH),
   PMIO_ADDRX(0, INT_POLARITY_LOW),
   PMIO_ADDRX(0, INT_EN_SET),
   PMIO_ADDRX(0, INT_EN_CLR),
   PMIO_ADDRX(0, INT_LATCHED_STS),
   PMIO_ADDRX(0, INT_PENDING_STS),
   PMIO_ADDRX(0, INT_MID_SEL),
   PMIO_ADDRX(0, INT_PRIORITY),
   PMIO_ADDRX(0, MEAS_INTERVAL_CTL),
   PMIO_ADDRX(0, MEAS_INTERVAL_CTL2),
   PMIO_ADDRX(0, ADC_DIG_PARAM),
   PMIO_ADDRX(0, FAST_AVG_CTL),
   PMIO_ADDRX(0, EN_CTL1),
   PMIO_ADDRX(0, DATA_HOLD_CTL),
   PMIO_ADDRX(0, BTM_ADC_CH_SEL_CTL_0),
   PMIO_ADDRX(0, BTM_LOW_THR0_0),
   PMIO_ADDRX(0, BTM_LOW_THR1_0),
   PMIO_ADDRX(0, BTM_HIGH_THR0_0),
   PMIO_ADDRX(0, BTM_HIGH_THR1_0),
   PMIO_ADDRX(0, BTM_MEAS_INTERVAL_CTL_0),
   PMIO_ADDRX(0, BTM_CTL_0),
   PMIO_ADDRX(0, BTM_EN_0),
   PMIO_ADDRX(0, BTM_ADC_CH_SEL_CTL_1),
   PMIO_ADDRX(0, BTM_LOW_THR0_1),
   PMIO_ADDRX(0, BTM_LOW_THR1_1),
   PMIO_ADDRX(0, BTM_HIGH_THR0_1),
   PMIO_ADDRX(0, BTM_HIGH_THR1_1),
   PMIO_ADDRX(0, BTM_MEAS_INTERVAL_CTL_1),
   PMIO_ADDRX(0, BTM_CTL_1),
   PMIO_ADDRX(0, BTM_EN_1),
   PMIO_ADDRX(0, BTM_DATA0_0),
   PMIO_ADDRX(0, BTM_DATA1_0),
   PMIO_ADDRX(0, BTM_DATA0_1),
   PMIO_ADDRX(0, BTM_DATA1_1),
};

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
VAdcTMHalResultType VAdcTMHalGetRevisionInfo(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMRevisionInfoType *pRevisionInfo)
{
   VAdcTMHalResultType result;
   uint8 aucData[6];

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      PMIO_ADDRX(0, REVISION1),
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   pRevisionInfo->usDigitalRev = VADCTM_REVISION(aucData[1], aucData[0]);
   pRevisionInfo->usAnalogRev = VADCTM_REVISION(aucData[3], aucData[2]);
   pRevisionInfo->ucType = aucData[4];
   pRevisionInfo->ucSubType = aucData[5];

   return result;
}

VAdcTMHalResultType VAdcTMHalGetStatus(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMStatusType *pStatus)
{
   VAdcTMHalResultType result;
   uint8 aucData[2];

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      PMIO_ADDRX(0, STATUS_LOW),
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   pStatus->uLowThresholdStatus = aucData[0];
   pStatus->uHighThresholdStatus = aucData[1];

   return VADCTM_HAL_SUCCESS;
}

VAdcTMHalResultType VAdcTMHalConfigInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask, VAdcTMInterruptConfigType eConfig)
{
   VAdcTMHalResultType result;
   uint8 aucData[3];

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      PMIO_ADDRX(0, INT_SET_TYPE),
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   switch (eConfig)
   {
      case VADCTM_INTERRUPT_CONFIG_LEVEL_HIGH:
         aucData[0] &= ~ucIntrMask;
         aucData[1] |= ucIntrMask;
         aucData[2] &= ~ucIntrMask;
         break;
      case VADCTM_INTERRUPT_CONFIG_LEVEL_LOW:
         aucData[0] &= ~ucIntrMask;
         aucData[1] &= ~ucIntrMask;
         aucData[2] |= ucIntrMask;
         break;
      case VADCTM_INTERRUPT_CONFIG_RISING_EDGE:
         aucData[0] |= ucIntrMask;
         aucData[1] |= ucIntrMask;
         aucData[2] &= ~ucIntrMask;
         break;
      case VADCTM_INTERRUPT_CONFIG_FALLING_EDGE:
         aucData[0] |= ucIntrMask;
         aucData[1] &= ~ucIntrMask;
         aucData[2] |= ucIntrMask;
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, INT_SET_TYPE),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalGetPendingInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 *pucIntrMask)
{
   VAdcTMHalResultType result;
   uint8 aucData[1];

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      PMIO_ADDRX(0, INT_LATCHED_STS),
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   *pucIntrMask = aucData[0];

   return VADCTM_HAL_SUCCESS;
}

VAdcTMHalResultType VAdcTMHalClearInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask)
{
   uint8 aucData[1];

   aucData[0] = ucIntrMask;

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, INT_LATCHED_CLR),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalEnableInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask)
{
   uint8 aucData[1];

   aucData[0] = ucIntrMask;

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, INT_EN_SET),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetInterruptMid(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uIntrMid)
{
   uint8 aucData[1];

   aucData[0] = PMIO_FVAL(INT_MID_SEL, INT_MID_SEL, (uint8)uIntrMid);

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, INT_MID_SEL),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetDataHold(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMDataHoldType eDataHold)
{
   uint8 aucData[1];

   if (eDataHold == VADCTM_FREE_RUNNING)
   {
      aucData[0] = PMIO_FVAL(DATA_HOLD_CTL, HOLD, 0);
   }
   else
   {
      aucData[0] = PMIO_FVAL(DATA_HOLD_CTL, HOLD, 1);
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, DATA_HOLD_CTL),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetConversionParameters(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMConversionParametersType *pParams)
{
   uint8 aucData[2];

   aucData[0] = PMIO_FVAL(ADC_DIG_PARAM, DEC_RATIO_SEL, (uint8)pParams->eDecimationRatio);

   aucData[1] = PMIO_FVAL(FAST_AVG_CTL, FAST_AVG_EN, PMIO_VAL(FAST_AVG_CTL, FAST_AVG_EN, FAST_AVG_ENABLED)) |
                PMIO_FVAL(FAST_AVG_CTL, FAST_AVG_SAMPLES, (uint8)pParams->eAverageMode);

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, ADC_DIG_PARAM),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetEnable(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMEnableType eEnable)
{
   uint8 aucData[1];

   if (eEnable == VADCTM_DISABLE)
   {
      aucData[0] = PMIO_FVAL(EN_CTL1, ADC_EN, 0);
   }
   else
   {
      aucData[0] = PMIO_FVAL(EN_CTL1, ADC_EN, 1);
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, EN_CTL1),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalRequestConversion(VAdcTMHalInterfaceType *piVAdcTMHal)
{
   uint8 aucData[1];

   aucData[0] = PMIO_FVAL(CONV_REQ, REQ, 1);

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, CONV_REQ),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetIntervalCtl(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMMeasIntervalCtlType *pMeasIntervalCtl)
{
   uint8 aucData[2];

   aucData[0] = PMIO_FVAL(MEAS_INTERVAL_CTL, MEAS_INTERVAL_TIME1, (uint8)pMeasIntervalCtl->eMeasIntervalTime1);

   aucData[1] = PMIO_FVAL(MEAS_INTERVAL_CTL2, MEAS_INTERVAL_TIME2, (uint8)pMeasIntervalCtl->eMeasIntervalTime2) |
                PMIO_FVAL(MEAS_INTERVAL_CTL2, MEAS_INTERVAL_TIME3, (uint8)pMeasIntervalCtl->eMeasIntervalTime3);

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     PMIO_ADDRX(0, MEAS_INTERVAL_CTL),
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetChannel(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uChannel)
{
   uint8 aucData[1];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_ADC_CH_SEL_CTL_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_ADC_CH_SEL_CTL_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   aucData[0] = (uint8)uChannel;

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetMeasParameters(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMMeasParametersType *pParams)
{
   uint8 aucData[2];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_MEAS_INTERVAL_CTL_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_MEAS_INTERVAL_CTL_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   aucData[0] = (uint8)pParams->eMeasIntervalTimeSelect;

   /* OK to use M0 only here - PMIO doesn't support indexed registers */
   aucData[1] = PMIO_FVAL(BTM_CTL_0, CAL_VAL, PMIO_VAL(BTM_CTL_0, CAL_VAL, TIMER_CAL)) |
                PMIO_FVAL(BTM_CTL_0, CAL_SEL, (uint8)pParams->eCalMethod) |
                PMIO_FVAL(BTM_CTL_0, HW_SETTLE_DELAY, (uint8)pParams->eSettlingDelay);

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetLowThresholdCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uCode)
{
   uint8 aucData[2];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_LOW_THR0_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_LOW_THR0_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   aucData[0] = uCode & 0xff;
   aucData[1] = (uCode >> 8) & 0xff;

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetHighThresholdCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uCode)
{
   uint8 aucData[2];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_HIGH_THR0_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_HIGH_THR0_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   aucData[0] = uCode & 0xff;
   aucData[1] = (uCode >> 8) & 0xff;

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetMeasEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable)
{
   VAdcTMHalResultType result;
   uint8 aucData[1];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_EN_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_EN_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      uReg,
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   if (eEnable == VADCTM_DISABLE)
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] &= ~PMIO_FMSK(BTM_EN_0, MEAS_EN);
   }
   else
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] |= PMIO_FVAL(BTM_EN_0, MEAS_EN, 0x1);
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetMeasLowThrEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable)
{
   VAdcTMHalResultType result;
   uint8 aucData[1];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_EN_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_EN_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      uReg,
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   if (eEnable == VADCTM_DISABLE)
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] &= ~PMIO_FMSK(BTM_EN_0, LOW_THR_INT_EN);
   }
   else
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] |= PMIO_FVAL(BTM_EN_0, LOW_THR_INT_EN, 0x1);
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalSetMeasHighThrEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable)
{
   VAdcTMHalResultType result;
   uint8 aucData[1];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_EN_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_EN_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      uReg,
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   if (eEnable == VADCTM_DISABLE)
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] &= ~PMIO_FMSK(BTM_EN_0, HIGH_THR_INT_EN);
   }
   else
   {
      /* OK to use M0 only here - PMIO doesn't support indexed registers */
      aucData[0] |= PMIO_FVAL(BTM_EN_0, HIGH_THR_INT_EN, 0x1);
   }

   return piVAdcTMHal->pfnWriteBytes(piVAdcTMHal->pCtxt,
                                     uReg,
                                     aucData,
                                     ARRAY_LENGTH(aucData));
}

VAdcTMHalResultType VAdcTMHalGetConversionCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 *puCode)
{
   VAdcTMHalResultType result;
   uint8 aucData[2];
   uint32 uReg;

   switch (uMeasIdx)
   {
      case 0:
         uReg = PMIO_ADDRX(0, BTM_DATA0_0);
         break;
      case 1:
         uReg = PMIO_ADDRX(0, BTM_DATA0_1);
         break;
      default:
         return VADCTM_HAL_ERROR;
   }

   result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                      uReg,
                                      aucData,
                                      ARRAY_LENGTH(aucData));
   if (result != VADCTM_HAL_SUCCESS)
   {
      return result;
   }

   *puCode = (aucData[1] << 8) | aucData[0];

   return VADCTM_HAL_SUCCESS;
}

VAdcTMHalResultType VAdcTMHalDumpRegisters(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMHalRegDumpType *pVAdcTMRegDump)
{
   VAdcTMHalResultType result;
   uint8 aucData[1];
   uint32 uOffset;
   uint32 uReg;

   for (uReg = 0; uReg < VADCTM_HAL_NUM_REGISTERS_TO_DUMP; uReg++)
   {
      uOffset = gaucRegDumpAddresses[uReg];

      result = piVAdcTMHal->pfnReadBytes(piVAdcTMHal->pCtxt,
                                         uOffset,
                                         aucData,
                                         ARRAY_LENGTH(aucData));
      if (result != VADCTM_HAL_SUCCESS)
      {
         return result;
      }

      pVAdcTMRegDump->aVAdcTMReg[uReg].ucOffset = (uint8)uOffset;
      pVAdcTMRegDump->aVAdcTMReg[uReg].ucVal = aucData[0];
   }

   return VADCTM_HAL_SUCCESS;
}

