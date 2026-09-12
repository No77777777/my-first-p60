/*============================================================================
  FILE:         VAdcInternalSettings.c

  OVERVIEW:     Internal settings for VADC.

  DEPENDENCIES: None

                Copyright (c) 2010-2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/config/7150/internal/VAdcInternalSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-09-07  AY   Updated settings for Moorea.
  2017-06-20  KS   Updated for SDM670.
  2016-08-12  jjo  Split BSP.
  2015-12-03  jjo  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "VAdcBsp.h"
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
extern const VAdcChannelConfigType gVAdcChannels[];
extern const VAdcAveragedChannelConfigType gVAdcAveragedChannels[];
extern const uint32 guNumVAdcChannels;
extern const uint32 guNumVAdcAveragedChannels;
extern const VAdcChannelConfigType gVAdcChannelsPM855A[];
extern const uint32 guNumVAdcChannelsPM855A;

/*
 * This table is a mapping of ADC channels to GPIO inputs.
 */
static const uint8 aucAmux1GpioChans[] = {0x12, 0x32, 0x52, 0x72, 0x92};
static const uint8 aucAmux2GpioChans[] = {0x13, 0x33, 0x53, 0x73};
static const uint8 aucAmux3GpioChans[] = {0x14, 0x34, 0x54, 0x74};
static const uint8 aucAmux4GpioChans[] = {0x15, 0x35, 0x55, 0x75};

static const uint8 aucAmux1GpioChans_855A[] = {0x12, 0x32, 0x52, 0x72};
static const uint8 aucAmux3GpioChans_855A[] = {0x14, 0x34, 0x54, 0x74, 0x94};
static const uint8 aucAmux4GpioChans_855A[] = {0x15, 0x35, 0x55, 0x75, 0x95};

static const VAdcGpioChannelMappingType vAdcGpioChannelMappings[] =
{
  {
      .paucChannels = aucAmux1GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans),
      .eGpio        = PM_GPIO_1,
   },

   {
      .paucChannels = aucAmux2GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans),
      .eGpio        = PM_GPIO_8,
   },

   {
      .paucChannels = aucAmux3GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans),
      .eGpio        = PM_GPIO_9,
   },

   {
      .paucChannels = aucAmux4GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans),
      .eGpio        = PM_GPIO_10,
   },
};

const VAdcBspType VAdcBsp[] =
{
   {
      .paChannels              = gVAdcChannels,
      .puNumChannels           = &guNumVAdcChannels,
      .paAveragedChannels      = gVAdcAveragedChannels,
      .puNumAveragedChannels   = &guNumVAdcAveragedChannels,
      .paGpioChannelMappings   = vAdcGpioChannelMappings,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappings),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 0,
      .uPeripheralId           = 0x32,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 0,
   }
};


/*----------------------------------------------------------------------------
 * PM855A Settings
 * -------------------------------------------------------------------------*/
static const VAdcGpioChannelMappingType vAdcGpioChannelMappingsPM855A[] =
{
   {
      .paucChannels = aucAmux1GpioChans_855A,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans_855A),
      .eGpio        = PM_GPIO_5,
   },

   {
      .paucChannels = aucAmux2GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans),
      .eGpio        = PM_GPIO_6,
   },

   {
      .paucChannels = aucAmux3GpioChans_855A,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans_855A),
      .eGpio        = PM_GPIO_7,
   },

   {
      .paucChannels = aucAmux4GpioChans_855A,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans_855A),
      .eGpio        = PM_GPIO_10,
   },
};

const VAdcBspType VAdcBspPM855A[] =
{
   {
      .paChannels              = gVAdcChannelsPM855A,
      .puNumChannels           = &guNumVAdcChannelsPM855A,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappingsPM855A,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappingsPM855A),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 4,
      .uPeripheralId           = 0x32,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 2,
   }
};


