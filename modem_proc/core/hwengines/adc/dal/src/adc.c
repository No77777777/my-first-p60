/*============================================================================
  FILE:         adc.c

  OVERVIEW:     Wrapper for DAL ADC.

  DEPENDENCIES: None

                Copyright (c) 2016, 2018 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/dal/src/adc.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2018-03-20  KS   Updated for Talos
  2016-04-27  jjo  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "adc.h"
#include "DDIAdc.h"
#include "string.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/
static DalDeviceHandle *gphAdcDev = NULL;

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
adc_status_type adc_read(const char *input, adc_result_type *adc_result)
{
   AdcInputPropertiesType adcInputProps;
   AdcResultType adcResult;
   DALResult result;

   if (gphAdcDev == NULL)
   {
      return ADC_STATUS_ERROR;
   }

   if (input == NULL || adc_result == NULL)
   {
      return ADC_STATUS_INVALID_PARAM;
   }

   result = DalAdc_GetAdcInputProperties(gphAdcDev,
                                         input,
                                         (uint32)(strlen(input) + 1),
                                         &adcInputProps);
   if (result != DAL_SUCCESS)
   {
      return ADC_STATUS_CHANNEL_NOT_SUPPORTED;
   }

   result = DalAdc_Read(gphAdcDev,
                        &adcInputProps,
                        &adcResult);
   if (result != DAL_SUCCESS || adcResult.eStatus != ADC_RESULT_VALID)
   {
      return ADC_STATUS_ERROR;
   }

   adc_result->physical = adcResult.nPhysical;
   adc_result->microvolts = adcResult.nMicrovolts;
   adc_result->code = adcResult.nCode;

   return ADC_STATUS_SUCCESS;
}

void adc_init(void)
{
   DALSYS_InitMod(NULL);

   (void)DAL_AdcDeviceAttach(DALDEVICEID_ADC, &gphAdcDev);
}

