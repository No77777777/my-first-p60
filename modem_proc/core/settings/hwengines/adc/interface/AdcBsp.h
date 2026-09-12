#ifndef __ADC_BSP_H__
#define __ADC_BSP_H__
/*============================================================================
  @file AdcBsp.h

  Function and data structure declarations for ADC's BSP


               Copyright (c) 2017 Qualcomm Technologies, Inc.
               All Rights Reserved.
               Qualcomm Technologies Confidential and Proprietary.
============================================================================*/
/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "com_dtypes.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef struct
{
   const char *pszWorkLoopName;
   uint32 uQueueSize;
} AdcQueueConfigType;

typedef struct
{
   const char *pszDevName;
   uint32 uQueueIdx;
} AdcPhysicalDeviceType;

typedef struct
{
   const AdcPhysicalDeviceType *paAdcPhysicalDevices;
   uint32 uNumPhysicalDevices;
   const AdcQueueConfigType *paAdcQueueConfigs;
   uint32 uNumQueues;
} AdcBspType;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/

#endif /* #ifndef __ADC_BSP_H__ */

