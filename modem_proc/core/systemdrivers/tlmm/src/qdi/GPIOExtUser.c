/*===========================================================================

FILE:      GPIOExtUser.c

DESCRIPTION:
  This file provides the extension API functions to the GPIO core driver.
  It is meant to be included in environments that do not have excessive
  memory constraints.

PUBLIC CLASSES:
  GPIODrv_Init
  GPIODrv_Attach
  GPIODrv_RegisterPin
  GPIODrv_GetPinConfig
  GPIODrv_IsPinActive
  GPIODrv_ConfigPinInactive
  GPIO_GetOutput
  GPIODrv_GetInactiveConfig
  GPIODrv_SetInactiveConfig

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/qdi/GPIOExtUser.c#1 $

  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

#include "GPIO.h"
#include "GPIOTypes.h"
#include "GPIOCore.h"

#include "qurt.h"
#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt_futex.h"
#include "qurt_atomic_ops.h"


/*=======================================================================================================

                               INTERNAL MACROS AND DATA STRUCTURES

  =====================================================================================================*/



/*=======================================================================================================

                                   INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/


/*=======================================================================================================

  FUNCTION      GPIO_Attach

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIODrv_Attach
(
  GPIODeviceIdType      eDevice,
  GPIOClientHandleType *pHandle
)
{
  if ( pHandle != NULL )
  {
    pHandle->DeviceId = eDevice;
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

}



/*=======================================================================================================

  FUNCTION      GPIODrv_RegisterPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIODrv_RegisterPin
(
  GPIOClientHandleType  Handle,
  const char           *pszSignal,
  GPIOAccessType        AccessType,
  GPIOConfigType       *pConfig,
  GPIOKeyType          *pGPIOKey
)
{
  uint32 nStrLength = 0;

  if ( pszSignal != NULL )
  {
    nStrLength = strlen(pszSignal);

    if ( (pGPIOKey != NULL && pConfig != NULL) || nStrLength > GPIO_MAX_STRING_LENGTH )
    {
      nStrLength = strlen(pszSignal);
      if ( qurt_qdi_handle_invoke((int)Handle.ClientId,
                                  GPIO_CMD_REGISTER_PIN,
                                  Handle.DeviceId,
                                  pszSignal,
                                  nStrLength,
                                  AccessType,
                                  pConfig,
                                  pGPIOKey) >= 0 )
      {
        return GPIO_SUCCESS;
      }
    }
  }
  return GPIO_ERROR;

} /* GPIODrv_RegisterPin */



/*=======================================================================================================

  FUNCTION      GPIODrv_ConfigPinInactive

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIODrv_ConfigPinInactive
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_CONFIG_PIN_INACTIVE,
                              Handle.DeviceId,
                              GPIOKey) >= 0 )
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* END GPIO_ConfigPin */



/*=======================================================================================================

  FUNCTION      GPIODrv_IsPinActive

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

boolean GPIODrv_IsPinActive
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_IS_PIN_ACTIVE,
                              Handle.DeviceId,
                              GPIOKey) >= 0)
  {
    return TRUE;
  }
  return FALSE;

} /* END GPIODrv_IsPinActive */



/*=======================================================================================================

  FUNCTION      GPIO_GetPinConfig

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIODrv_GetPinConfig
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey,
  GPIOConfigType       *pCfg
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_GET_PIN_CONFIG,
                              Handle.DeviceId,
                              GPIOKey,
                              pCfg) >= 0 )
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* END GPIODrv_GetPinConfig */



/*=======================================================================================================

  FUNCTION      GPIO_GetOutput

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOValueType GPIODrv_GetOutput
(
  GPIOClientHandleType  Handle,
  GPIOKeyType           GPIOKey
)
{
  GPIOValueType GPIOValue = GPIO_LOW;
  qurt_qdi_handle_invoke(Handle.ClientId,
                         GPIO_CMD_GET_PIN_OUTPUT,
                         Handle.DeviceId,
                         GPIOKey,
                         &GPIOValue);
  return GPIOValue;

} /* END GPIO_GetOutput */



/*=======================================================================================================

  FUNCTION      GPIODrv_GetInactiveConfig

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOResult GPIODrv_GetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType      *pConfig,
  GPIOValueType       *pValue
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_GET_INACTIVE_CONFIG,
                              Handle.DeviceId,
                              GPIOKey, pConfig, pValue) >= 0)
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIODrv_GetInactiveConfig */



/*=======================================================================================================

  FUNCTION      GPIODrv_SetInactiveConfig

  DESCRIPTION   See GPIOExt.h

  =====================================================================================================*/

GPIOResult GPIODrv_SetInactiveConfig
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType       Config,
  GPIOValueType        Value
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_SET_INACTIVE_CONFIG,
                              Handle.DeviceId,
                              GPIOKey, &Config, Value) >= 0)
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIODrv_GetInactiveConfig */


/*=======================================================================================================

  FUNCTION      GPIO_GetPinNumber

  DESCRIPTION   See GPIODiag.h

  =====================================================================================================*/

uint32 GPIO_GetPinNumber( GPIOKeyType nGpioId )
{
  GPIOPinInfoType PinInfo;
  PinInfo.All = nGpioId;
  return ((uint32)PinInfo.Info.Pin);
  
} /* GPIO_GetPinNumber */

