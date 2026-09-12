/*============================================================================
  FILE:         VAdcTMSettings.c

  OVERVIEW:     Settings for VADC TM.

  DEPENDENCIES: None

                Copyright (c) 2016-2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/config/670/VAdcTMSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-20  KS   Updated for SDM670.
  2016-08-12  jjo  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcTMBsp.h"
#include "AdcInputs.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
extern const AdcIntTableType gVAdcXoThermTable;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
static const VAdcTMPullUpConfigType vAdcTMXOThermPullUpConfig =
{
   .uLowChannel  = 0x6c,
   .uLowPullUp   = 400000,
   .uHighChannel = 0x2c,
   .uHighPullUp  = 30000,
   .nMedToLow    = 5120,
   .nLowToMed    = 15360,
   .nHighToMed   = 32768,
   .nMedToHigh   = 43008,
};

const VAdcTMMeasConfigType gVAdcTMMeasurements[] =
{
   /* XO_THERM */
   {
      .pszName                   = ADC_INPUT_XO_THERM,
      .pPullUpConfig             = &vAdcTMXOThermPullUpConfig,
      .uAdcHardwareChannel       = 0x4c,
      .eSettlingDelay            = VADCTM_SETTLING_DELAY_4_MS,
      .eMeasIntervalTimeSelect   = VADCTM_MEAS_INTERVAL_TIME2,
      .bAlwaysOn                 = FALSE,
      .eCalMethod                = VADCTM_CAL_METHOD_RATIOMETRIC,
      .scalingFactor             = {1, 1},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_THERMISTOR,
      .uPullUp                   = 100000,
      .pIntTable                 = &gVAdcXoThermTable,
      .nPhysicalMin              = -40960,
      .nPhysicalMax              = 130048,
   },

   /* PMIC_TEMP1 */
   {
      .pszName                   = ADC_INPUT_PMIC_TEMP1,
      .pPullUpConfig             = NULL,
      .uAdcHardwareChannel       = 0x6,
      .eSettlingDelay            = VADCTM_SETTLING_DELAY_0_US,
      .eMeasIntervalTimeSelect   = VADCTM_MEAS_INTERVAL_TIME2,
      .bAlwaysOn                 = FALSE,
      .eCalMethod                = VADCTM_CAL_METHOD_ABSOLUTE,
      .scalingFactor             = {1, 1},  /* {num, den} */
      .eScalingMethod            = VADC_SCALE_PMIC_SENSOR_TO_MILLIDEGREES,
      .uPullUp                   = 0,
      .pIntTable                 = NULL,
      .nPhysicalMin              = -50000,
      .nPhysicalMax              = 150000,
   },
};

const VAdcTMCommonConfigType gVAdcTMCommonConfig =
{
   .eAverageMode            = VADCTM_AVERAGE_4_SAMPLES,
   .eDecimationRatio        = VADCTM_DECIMATION_RATIO_1024,
   .eMeasIntervalTime1      = VADCTM_MEAS_INTERVAL_TIME1_1000_MS,
   .eMeasIntervalTime2      = VADCTM_MEAS_INTERVAL_TIME2_100_MS,
   .eMeasIntervalTime3      = VADCTM_MEAS_INTERVAL_TIME3_5_S,
};

const uint32 guNumVAdcTMMeas = ARRAY_LENGTH(gVAdcTMMeasurements);

