#ifndef __VADCTMHAL_H__
#define __VADCTMHAL_H__
/*============================================================================
  @file VAdcTMHal.h

  Function and data structure declarations for VADCTM HAL


                Copyright (c) 2012, 2015, 2017-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/VAdcTMHal.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "VAdcTMBsp.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define VADCTM_INVALID_ADC_CODE 0x8000

#define VADCTM_INT_MASK_THR 0x1

#define VADCTM_HAL_NUM_REGISTERS_TO_DUMP 45

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef enum
{
   VADCTM_HAL_SUCCESS = 0,
   VADCTM_HAL_ERROR
} VAdcTMHalResultType;

typedef struct
{
   uint16 usDigitalRev;
   uint16 usAnalogRev;
   uint8 ucType;
   uint8 ucSubType;
} VAdcTMRevisionInfoType;

typedef struct
{
   uint32 uLowThresholdStatus;
   uint32 uHighThresholdStatus;
} VAdcTMStatusType;

typedef enum
{
   VADCTM_INTERRUPT_CONFIG_LEVEL_HIGH = 0,
   VADCTM_INTERRUPT_CONFIG_LEVEL_LOW,
   VADCTM_INTERRUPT_CONFIG_RISING_EDGE,
   VADCTM_INTERRUPT_CONFIG_FALLING_EDGE
} VAdcTMInterruptConfigType;

typedef enum
{
   VADCTM_FREE_RUNNING = 0,
   VADCTM_HOLD_DATA
} VAdcTMDataHoldType;

typedef struct
{
   VAdcTMDecimationRatioType eDecimationRatio;
   VAdcTMAverageModeType eAverageMode;
} VAdcTMConversionParametersType;

typedef enum
{
   VADCTM_DISABLE = 0,
   VADCTM_ENABLE
} VAdcTMEnableType;

typedef struct
{
   VAdcTMMeasIntervalTime1Type eMeasIntervalTime1;
   VAdcTMMeasIntervalTime2Type eMeasIntervalTime2;
   VAdcTMMeasIntervalTime3Type eMeasIntervalTime3;
} VAdcTMMeasIntervalCtlType;

typedef struct
{
   uint32 uChannel;
   VAdcTMMeasIntervalTimeSelectType eMeasIntervalTimeSelect;
   VAdcTMCalMethodType eCalMethod;
   VAdcTMSettlingDelay eSettlingDelay;
} VAdcTMMeasParametersType;

typedef struct
{
   uint8 ucOffset;
   uint8 ucVal;
} VAdcTMHalRegType;

typedef struct
{
   VAdcTMHalRegType aVAdcTMReg[VADCTM_HAL_NUM_REGISTERS_TO_DUMP];
} VAdcTMHalRegDumpType;

typedef struct
{
   void *pCtxt;
   VAdcTMHalResultType (*pfnWriteBytes)(void *pCtxt, uint32 uRegisterAddress, uint8 *pucData, uint32 uDataLen);
   VAdcTMHalResultType (*pfnReadBytes)(void *pCtxt, uint32 uRegisterAddress, uint8 *pucData, uint32 uDataLen);
} VAdcTMHalInterfaceType;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
VAdcTMHalResultType VAdcTMHalGetRevisionInfo(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMRevisionInfoType *pRevisionInfo);
VAdcTMHalResultType VAdcTMHalGetStatus(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMStatusType *pStatus);
VAdcTMHalResultType VAdcTMHalConfigInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask, VAdcTMInterruptConfigType eConfig);
VAdcTMHalResultType VAdcTMHalGetPendingInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 *pucIntrMask);
VAdcTMHalResultType VAdcTMHalClearInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask);
VAdcTMHalResultType VAdcTMHalEnableInterrupts(VAdcTMHalInterfaceType *piVAdcTMHal, uint8 ucIntrMask);
VAdcTMHalResultType VAdcTMHalSetInterruptMid(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uIntrMid);
VAdcTMHalResultType VAdcTMHalSetDataHold(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMDataHoldType eDataHold);
VAdcTMHalResultType VAdcTMHalSetConversionParameters(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMConversionParametersType *pParams);
VAdcTMHalResultType VAdcTMHalSetEnable(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMEnableType eEnable);
VAdcTMHalResultType VAdcTMHalRequestConversion(VAdcTMHalInterfaceType *piVAdcTMHal);
VAdcTMHalResultType VAdcTMHalSetIntervalCtl(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMMeasIntervalCtlType *pMeasIntervalCtl);
VAdcTMHalResultType VAdcTMHalSetChannel(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uChannel);
VAdcTMHalResultType VAdcTMHalSetMeasParameters(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMMeasParametersType *pParams);
VAdcTMHalResultType VAdcTMHalSetLowThresholdCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uCode);
VAdcTMHalResultType VAdcTMHalSetHighThresholdCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 uCode);
VAdcTMHalResultType VAdcTMHalSetMeasEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable);
VAdcTMHalResultType VAdcTMHalSetMeasLowThrEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable);
VAdcTMHalResultType VAdcTMHalSetMeasHighThrEnable(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, VAdcTMEnableType eEnable);
VAdcTMHalResultType VAdcTMHalGetConversionCode(VAdcTMHalInterfaceType *piVAdcTMHal, uint32 uMeasIdx, uint32 *puCode);
VAdcTMHalResultType VAdcTMHalDumpRegisters(VAdcTMHalInterfaceType *piVAdcTMHal, VAdcTMHalRegDumpType *pVAdcTMRegDump);

#endif /* #ifndef __VADCTMHAL_H__ */

