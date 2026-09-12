/*============================================================================
  FILE:         VAdcInternalSettings.c

  OVERVIEW:     Internal settings for VADC.

  DEPENDENCIES: None

                Copyright (c) 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/config/24/internal/VAdcInternalSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-03-28  jjo  Initial revision.

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

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
/*
 * This table is a mapping of ADC channels to GPIO inputs.
 */
static const uint8 aucGpio6Channels[]  = {0x12, 0x32, 0x52, 0x72};
static const uint8 aucGpio2Channels[]  = {0x13, 0x33, 0x53, 0x73};
static const uint8 aucGpio10Channels[] = {0x14, 0x34, 0x54, 0x74};
static const uint8 aucGpio4Channels[]  = {0x15, 0x35, 0x55, 0x75};

static const VAdcGpioChannelMappingType vAdcGpioChannelMappings[] =
{
   {
      .paucChannels = aucGpio2Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio2Channels),
      .eGpio        = PM_GPIO_2,
   },

   {
      .paucChannels = aucGpio4Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio4Channels),
      .eGpio        = PM_GPIO_4,
   },

   {
      .paucChannels = aucGpio6Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio6Channels),
      .eGpio        = PM_GPIO_6,
   },

   {
      .paucChannels = aucGpio10Channels,
      .uNumChannels = ARRAY_LENGTH(aucGpio10Channels),
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
      .uFullScale_code         = 0x4000,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 17,
      .uSlaveId                = 0,
      .uPeripheralId           = 0x32,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 0,
      .usMinDigRev             = VADC_REVISION(3, 0),
      .usMinAnaRev             = VADC_REVISION(1, 0),
      .ucPerphType             = 0x8,
   }
};

