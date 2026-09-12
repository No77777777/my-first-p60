#ifndef __VADCTMBSP_H__
#define __VADCTMBSP_H__
/*============================================================================
  @file VAdcTMBsp.h

  Function and data structure declarations for VADCTM DAL


                Copyright (c) 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/interface/VAdcTMBsp.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "AdcScalingBsp.h"
#include "SpmiTypes.h"
#include "pm_version.h"
#include "pm_gpio.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define VADCTM_REVISION(major, minor) (((major) << 8) | (minor))

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef enum
{
   VADCTM_DECIMATION_RATIO_256 = 0,
   VADCTM_DECIMATION_RATIO_512,
   VADCTM_DECIMATION_RATIO_1024
} VAdcTMDecimationRatioType;

typedef enum
{
   VADCTM_AVERAGE_1_SAMPLE = 0,
   VADCTM_AVERAGE_2_SAMPLES,
   VADCTM_AVERAGE_4_SAMPLES,
   VADCTM_AVERAGE_8_SAMPLES,
   VADCTM_AVERAGE_16_SAMPLES
} VAdcTMAverageModeType;

typedef enum
{
   VADCTM_MEAS_INTERVAL_TIME1_0_MS = 0,
   VADCTM_MEAS_INTERVAL_TIME1_1P0_MS,
   VADCTM_MEAS_INTERVAL_TIME1_2P0_MS,
   VADCTM_MEAS_INTERVAL_TIME1_3P9_MS,
   VADCTM_MEAS_INTERVAL_TIME1_7P8_MS,
   VADCTM_MEAS_INTERVAL_TIME1_15P6_MS,
   VADCTM_MEAS_INTERVAL_TIME1_31P3_MS,
   VADCTM_MEAS_INTERVAL_TIME1_62P5_MS,
   VADCTM_MEAS_INTERVAL_TIME1_125_MS,
   VADCTM_MEAS_INTERVAL_TIME1_250_MS,
   VADCTM_MEAS_INTERVAL_TIME1_500_MS,
   VADCTM_MEAS_INTERVAL_TIME1_1000_MS,
   VADCTM_MEAS_INTERVAL_TIME1_2000_MS,
   VADCTM_MEAS_INTERVAL_TIME1_4000_MS,
   VADCTM_MEAS_INTERVAL_TIME1_8000_MS,
   VADCTM_MEAS_INTERVAL_TIME1_16000_MS
} VAdcTMMeasIntervalTime1Type;

typedef enum
{
   VADCTM_MEAS_INTERVAL_TIME2_0_MS = 0,
   VADCTM_MEAS_INTERVAL_TIME2_100_MS,
   VADCTM_MEAS_INTERVAL_TIME2_200_MS,
   VADCTM_MEAS_INTERVAL_TIME2_300_MS,
   VADCTM_MEAS_INTERVAL_TIME2_400_MS,
   VADCTM_MEAS_INTERVAL_TIME2_500_MS,
   VADCTM_MEAS_INTERVAL_TIME2_600_MS,
   VADCTM_MEAS_INTERVAL_TIME2_700_MS,
   VADCTM_MEAS_INTERVAL_TIME2_800_MS,
   VADCTM_MEAS_INTERVAL_TIME2_900_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1000_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1100_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1200_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1300_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1400_MS,
   VADCTM_MEAS_INTERVAL_TIME2_1500_MS,
} VAdcTMMeasIntervalTime2Type;

typedef enum
{
   VADCTM_MEAS_INTERVAL_TIME3_0_S = 0,
   VADCTM_MEAS_INTERVAL_TIME3_1_S,
   VADCTM_MEAS_INTERVAL_TIME3_2_S,
   VADCTM_MEAS_INTERVAL_TIME3_3_S,
   VADCTM_MEAS_INTERVAL_TIME3_4_S,
   VADCTM_MEAS_INTERVAL_TIME3_5_S,
   VADCTM_MEAS_INTERVAL_TIME3_6_S,
   VADCTM_MEAS_INTERVAL_TIME3_7_S,
   VADCTM_MEAS_INTERVAL_TIME3_8_S,
   VADCTM_MEAS_INTERVAL_TIME3_9_S,
   VADCTM_MEAS_INTERVAL_TIME3_10_S,
   VADCTM_MEAS_INTERVAL_TIME3_11_S,
   VADCTM_MEAS_INTERVAL_TIME3_12_S,
   VADCTM_MEAS_INTERVAL_TIME3_13_S,
   VADCTM_MEAS_INTERVAL_TIME3_14_S,
   VADCTM_MEAS_INTERVAL_TIME3_15_S
} VAdcTMMeasIntervalTime3Type;

typedef enum
{
   VADCTM_MEAS_INTERVAL_TIME1 = 0,
   VADCTM_MEAS_INTERVAL_TIME2,
   VADCTM_MEAS_INTERVAL_TIME3
} VAdcTMMeasIntervalTimeSelectType;

typedef enum
{
   VADCTM_CAL_METHOD_NO_CAL = 0,
   VADCTM_CAL_METHOD_RATIOMETRIC,
   VADCTM_CAL_METHOD_ABSOLUTE
} VAdcTMCalMethodType;

typedef enum
{
   VADCTM_SETTLING_DELAY_0_US = 0,
   VADCTM_SETTLING_DELAY_100_US,
   VADCTM_SETTLING_DELAY_200_US,
   VADCTM_SETTLING_DELAY_300_US,
   VADCTM_SETTLING_DELAY_400_US,
   VADCTM_SETTLING_DELAY_500_US,
   VADCTM_SETTLING_DELAY_600_US,
   VADCTM_SETTLING_DELAY_700_US,
   VADCTM_SETTLING_DELAY_800_US,
   VADCTM_SETTLING_DELAY_900_US,
   VADCTM_SETTLING_DELAY_1_MS,
   VADCTM_SETTLING_DELAY_2_MS,
   VADCTM_SETTLING_DELAY_4_MS,
   VADCTM_SETTLING_DELAY_6_MS,
   VADCTM_SETTLING_DELAY_8_MS,
   VADCTM_SETTLING_DELAY_10_MS
} VAdcTMSettlingDelay;

typedef struct
{
   uint32 uLowChannel;
   uint32 uLowPullUp;
   uint32 uHighChannel;
   uint32 uHighPullUp;
   int32 nMedToLow;
   int32 nLowToMed;
   int32 nHighToMed;
   int32 nMedToHigh;
} VAdcTMPullUpConfigType;

typedef struct
{
   const char *pszName;
   const VAdcTMPullUpConfigType *pPullUpConfig;
   uint32 uAdcHardwareChannel;
   VAdcTMSettlingDelay eSettlingDelay;
   VAdcTMMeasIntervalTimeSelectType eMeasIntervalTimeSelect;
   VAdcTMCalMethodType eCalMethod;
   VAdcChannelScalingFactor scalingFactor;
   VAdcScalingMethodType eScalingMethod;
   uint32 uPullUp;
   const AdcIntTableType *pIntTable;
   int32 nPhysicalMin;
   int32 nPhysicalMax;
   boolean bAlwaysOn;
} VAdcTMMeasConfigType;

typedef struct
{
   const uint8 *paucChannels;
   uint32 uNumChannels;
   pm_gpio_perph_index eGpio;
} VAdcTMGpioChannelMappingType;

typedef struct
{
   VAdcTMAverageModeType eAverageMode;
   VAdcTMDecimationRatioType eDecimationRatio;
   VAdcTMMeasIntervalTime1Type eMeasIntervalTime1;
   VAdcTMMeasIntervalTime2Type eMeasIntervalTime2;
   VAdcTMMeasIntervalTime3Type eMeasIntervalTime3;
} VAdcTMCommonConfigType;

typedef struct
{
   const VAdcTMMeasConfigType *paMeasConfig;
   const uint32 *puNumMeas;
   const VAdcTMCommonConfigType *pCommonConfig;
   const VAdcTMGpioChannelMappingType *paGpioChannelMappings;
   uint32 uNumGpioChannelMappings;
   uint32 uFullScale_code;
   uint32 uFullScale_uV;
   uint32 uSlaveId;
   uint32 uPeripheralId;
   uint32 uMasterID;
   SpmiBus_AccessPriorityType eAccessPriority;
   uint8 ucPmicDevice;
} VAdcTMBspType;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/

#endif /* #ifndef __VADCTMBSP_H__ */

