/*============================================================================
  FILE:         VAdcTMInternalSettings.c

  OVERVIEW:     Internal settings for VADC TM.

  DEPENDENCIES: None

                Copyright (c) 2015-2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/settings/hwengines/adc/config/670/internal/VAdcTMInternalSettings.c#1 $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-20  KS   Updated for SDM670.
  2016-08-12  jjo  Split BSP.
  2015-12-03  jjo  Initial revision.

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
extern const VAdcTMMeasConfigType gVAdcTMMeasurements[];
extern const uint32 guNumVAdcTMMeas;
extern const VAdcTMCommonConfigType gVAdcTMCommonConfig;

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
const VAdcTMBspType VAdcTMBsp[] =
{
   {
      .paMeasConfig            = gVAdcTMMeasurements,
      .puNumMeas               = &guNumVAdcTMMeas,
      .pCommonConfig           = &gVAdcTMCommonConfig,
      .paGpioChannelMappings   = NULL,
      .uNumGpioChannelMappings = 0,
      .uFullScale_code         = 0x4000,
      .uFullScale_uV           = 1875000,
      .uSlaveId                = 0,
      .uPeripheralId           = 0x38,
      .uMasterID               = 0,
      .eAccessPriority         = SPMI_BUS_ACCESS_PRIORITY_LOW,
      .ucPmicDevice            = 0,
   }
};
