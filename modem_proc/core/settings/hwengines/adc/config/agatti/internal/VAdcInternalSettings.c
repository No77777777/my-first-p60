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

  $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/config/agatti/internal/VAdcInternalSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
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
extern const VAdcChannelConfigType gVAdcChannels_agatti[];
extern const VAdcAveragedChannelConfigType gVAdcAveragedChannels_agatti[];
extern const uint32 guNumVAdcChannels_agatti;
extern const uint32 guNumVAdcAveragedChannels_agatti;
extern const VAdcChannelConfigType gVAdcChannelsPMI455_agatti[];
extern const uint32 guNumVAdcChannelsPMI455_agatti;

/*
 * This table is a mapping of ADC channels to GPIO inputs.
 */
static const uint8 aucAmux1GpioChans[] = {0x12, 0x32, 0x52, 0x72};
static const uint8 aucAmux2GpioChans[] = {0x13, 0x33, 0x53, 0x73};
static const uint8 aucAmux3GpioChans[] = {0x14, 0x34, 0x54, 0x74, 0x94};
static const uint8 aucAmux4GpioChans[] = {0x15, 0x35, 0x55, 0x75, 0x95};

static const uint8 aucAmux1GpioChans_455[] = {0x12, 0x32, 0x52, 0x72, 0x92};
static const uint8 aucAmux2GpioChans_455[] = {0x13, 0x33, 0x53, 0x73, 0x93};
static const uint8 aucAmux3GpioChans_455[] = {0x14, 0x34, 0x54, 0x74, 0x94};
static const uint8 aucAmux4GpioChans_455[] = {0x15, 0x35, 0x55, 0x75};

static const VAdcGpioChannelMappingType vAdcGpioChannelMappings[] =
{
  {
      .paucChannels = aucAmux1GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans),
      .eGpio        = PM_GPIO_3,
   },

   {
      .paucChannels = aucAmux2GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans),
      .eGpio        = PM_GPIO_4,
   },

   {
      .paucChannels = aucAmux3GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans),
      .eGpio        = PM_GPIO_6,
   },

   {
      .paucChannels = aucAmux4GpioChans,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans),
      .eGpio        = PM_GPIO_7,
   },
};

const VAdcBspType VAdcBsp_agatti[] =
{
   {
      .paChannels              = gVAdcChannels_agatti,
      .puNumChannels           = &guNumVAdcChannels_agatti,
      .paAveragedChannels      = gVAdcAveragedChannels_agatti,
      .puNumAveragedChannels   = &guNumVAdcAveragedChannels_agatti,
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
 * PMI455 Settings
 * -------------------------------------------------------------------------*/
static const VAdcGpioChannelMappingType vAdcGpioChannelMappingsPMI455[] =
{
   {
      .paucChannels = aucAmux1GpioChans_455,
      .uNumChannels = ARRAY_LENGTH(aucAmux1GpioChans_455),
      .eGpio        = PM_GPIO_1,
   },

   {
      .paucChannels = aucAmux2GpioChans_455,
      .uNumChannels = ARRAY_LENGTH(aucAmux2GpioChans_455),
      .eGpio        = PM_GPIO_3,
   },

   {
      .paucChannels = aucAmux3GpioChans_455,
      .uNumChannels = ARRAY_LENGTH(aucAmux3GpioChans_455),
      .eGpio        = PM_GPIO_4,
   },

   {
      .paucChannels = aucAmux4GpioChans_455,
      .uNumChannels = ARRAY_LENGTH(aucAmux4GpioChans_455),
      .eGpio        = PM_GPIO_6,
   },
};

const VAdcBspType VAdcBspPMI455_agatti[] =
{
   {
      .paChannels              = gVAdcChannelsPMI455_agatti,
      .puNumChannels           = &guNumVAdcChannelsPMI455_agatti,
      .paAveragedChannels      = NULL,
      .puNumAveragedChannels   = NULL,
      .paGpioChannelMappings   = vAdcGpioChannelMappingsPMI455,
      .uNumGpioChannelMappings = ARRAY_LENGTH(vAdcGpioChannelMappingsPMI455),
      .bUsesInterrupts         = FALSE,
      .uFullScale_code         = 0x70E4,
      .uFullScale_uV           = 1875000,
      .uReadTimeout_us         = 500000,
      .uLDOSettlingTime_us     = 67,
      .uSlaveId                = 2,
      .uPeripheralId           = 0x32,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 1,
   }
};


