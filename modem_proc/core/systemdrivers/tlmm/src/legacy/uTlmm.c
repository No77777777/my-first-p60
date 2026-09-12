/*===========================================================================

             M A I N   D A L   T L M M   D R I V E R   F I L E

DESCRIPTION

  Contains the majority of functionality for the uTlmm TLMM driver.  The
  API in this module MUST always remain target independent.


===========================================================================
             Copyright (c) 2015 QUALCOMM Technologies Incorporated.
                    All Rights Reserved.
                  QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/legacy/uTlmm.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/09/09   dcf     Fix for immediateConfigs in restoring GPIOs from sleep.
02/23/09   an      Mainlining FEATURE_MANAGE_GPIOS_FOR_SLEEP and 
                   FEATURE_APPS_GPIOS_LOW_POWER by having internal flags 
                   in TLMM.
01/08/09   dcf     General clean up, removal of bare enumerated types and 
                   addition of uniform headers and comments.
===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "GPIOTypes.h"
#include "GPIO.h"
#include "GPIOCore.h"
#include "uTlmm.h"

/*==========================================================================

                     LOCAL TYPE DEFINES FOR THIS MODULE

==========================================================================*/

#define GPIO_TOTAL_PIN_SLOTS  32


GPIOClientHandleType UTLMMHandle = {GPIO_DEVICE_SSC_LPI};
boolean inited = FALSE;

boolean IsInited = FALSE;

uint32 GPIO_LowPowerConfigs[GPIO_TOTAL_PIN_SLOTS];

/*==========================================================================

                     LOCAL DATA FOR THIS MODULE

==========================================================================*/

uint32 uTlmm_ConvertToLogical(uint32 nDrive)
{
  switch (nDrive)
  {
    case 0: return 200;
    case 1: return 400;
    case 2: return 600;
    case 3: return 800;
    case 4: return 1000;
    case 5: return 1200;
    case 6: return 1400;
    default: return 1600;
  }
  return 0;
}


static void uTlmm_MakeConfig(uint32 nConfig, GPIOConfigType* pCfg)
{
  pCfg->func = (uint32)UTLMM_GPIO_FUNCTION(nConfig);
  pCfg->dir = (uint32)UTLMM_GPIO_DIRECTION(nConfig);
  pCfg->pull = (uint32)UTLMM_GPIO_PULL(nConfig);
  pCfg->drive = (uint32)uTlmm_ConvertToLogical(UTLMM_GPIO_DRIVE(nConfig));  
}


/*==========================================================================

  FUNCTION      uTlmm_ConfigGpio

  DESCRIPTION   See uTlmm.h

==========================================================================*/ 

boolean uTlmm_ConfigGpio
(  
  uTlmmGpioSignalType   eGpioConfig,
  uTlmmGpioEnableType   eEnableType
)
{
  GPIOConfigType Config = {0};
  GPIOKeyType GPIOKey = 0;
  uint32 PinNumber = (uint32)UTLMM_GPIO_NUMBER(eGpioConfig);
  uint32 nConfig = 0;
  
  if (!IsInited)
  {
    if (GPIO_SUCCESS == GPIO_Attach(GPIO_DEVICE_SSC_LPI, &UTLMMHandle))
    {
      IsInited = TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(UTLMMHandle, PinNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( eEnableType == UTLMM_GPIO_DISABLE )
    {
      if ( PinNumber >= GPIO_TOTAL_PIN_SLOTS )
      {
        return FALSE;
      }
      nConfig = GPIO_LowPowerConfigs[PinNumber];
    }
    else
    {
      nConfig = eGpioConfig;
    }

    uTlmm_MakeConfig(nConfig, &Config);
    uTlmm_GpioOut(eGpioConfig, (uTlmmGpioValueType)UTLMM_GPIO_GET_OUTVAL(nConfig));
    
    if ( GPIO_SUCCESS == GPIO_ConfigPin(UTLMMHandle, GPIOKey, Config) )
    {
      return TRUE;
    }
  }
  return FALSE;

} /* uTlmm_ConfigGpio */


/*==========================================================================

  FUNCTION      uTlmm_GetGpioNumber

  DESCRIPTION   See uTlmm.h

==========================================================================*/ 

boolean  uTlmm_GetGpioNumber
(
  uTlmmGpioSignalType eGpioConfig,
  uint32*            pnGpioNumber
) 
{
  if(pnGpioNumber != NULL)
  {
    *pnGpioNumber = (uint32)UTLMM_GPIO_NUMBER(eGpioConfig);
    return(TRUE);
  }
  return(FALSE);

} /* uTlmm_GetGpioNumber */


/*==========================================================================

  FUNCTION      uTlmm_GpioIn

  DESCRIPTION   See uTlmm.h

==========================================================================*/

boolean  uTlmm_GpioIn
(
  uTlmmGpioSignalType  eGpioConfig,  
  uTlmmGpioValueType*  peValue
) 
{
  GPIOConfigType Config = {0};
  GPIOKeyType GPIOKey = 0;
  uTlmm_MakeConfig(eGpioConfig, &Config);
  uint32 PinNumber = (uint32)UTLMM_GPIO_NUMBER(eGpioConfig);
  GPIOValueType eValue = GPIO_LOW;
  
  if (!IsInited)
  {
    if (GPIO_SUCCESS == GPIO_Attach(GPIO_DEVICE_SSC_LPI, &UTLMMHandle))
    {
      IsInited = TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(UTLMMHandle, PinNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if(peValue != NULL)
    {
      if (GPIO_SUCCESS == GPIO_ReadPin(UTLMMHandle, GPIOKey, &eValue))
      {
        if ( eValue == GPIO_HIGH )
        {
          *peValue = UTLMM_GPIO_HIGH_VALUE;
        }
        else
        {
          *peValue = UTLMM_GPIO_LOW_VALUE;
        }
        return TRUE;
      }
    }
  }

  return(FALSE);

} /* uTlmm_GpioIn */


/*==========================================================================

  FUNCTION      uTlmm_GpioOut

  DESCRIPTION   See uTlmm.h

==========================================================================*/ 

boolean uTlmm_GpioOut
( 
  uTlmmGpioSignalType  eGpioConfig,   
  uTlmmGpioValueType   eValue
) 
{
  boolean bValue = FALSE;
  GPIOConfigType Config = {0};
  GPIOKeyType GPIOKey = 0;
  uint32 PinNumber = (uint32)UTLMM_GPIO_NUMBER(eGpioConfig);
    
  uTlmm_MakeConfig(eGpioConfig, &Config);
  
  if (!IsInited)
  {
    if (GPIO_SUCCESS == GPIO_Attach(GPIO_DEVICE_SSC_LPI, &UTLMMHandle))
    {
      IsInited = TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  if (GPIO_SUCCESS == GPIO_RegisterPinExplicit(UTLMMHandle, PinNumber, GPIO_ACCESS_SHARED, &GPIOKey))
  {
    if ( eValue == UTLMM_GPIO_HIGH_VALUE )
    {
      bValue = TRUE;
    }
    GPIO_WritePin(UTLMMHandle, GPIOKey, bValue);
  }
  return TRUE;

} /* uTlmm_GpioOut */


/*==========================================================================

  FUNCTION      uTlmm_SetInactiveConfig

  DESCRIPTION   See uTlmm.h

==========================================================================*/ 

boolean uTlmm_SetInactiveConfig
(
  uint32              gpio_number, 
  uTlmmGpioSignalType gpio_config
)
{
  if ( gpio_number < GPIO_TOTAL_PIN_SLOTS )
  {
    GPIO_LowPowerConfigs[gpio_number] = gpio_config;
    return TRUE;
  }
  return FALSE;
  
} /* uTlmm_SetInactiveConfig */


/*==========================================================================

  FUNCTION      uTlmm_GetInactiveConfig

  DESCRIPTION   See uTlmm.h

==========================================================================*/

boolean uTlmm_GetInactiveConfig
(
  uint32               gpio_number, 
  uTlmmGpioSignalType *gpio_config
)
{
  if ( (gpio_number < GPIO_TOTAL_PIN_SLOTS) && (gpio_config != NULL) )
  {
    *gpio_config = GPIO_LowPowerConfigs[gpio_number];
    return TRUE;
  }
  return FALSE;
  
} /* uTlmm_GetInactiveConfig */

