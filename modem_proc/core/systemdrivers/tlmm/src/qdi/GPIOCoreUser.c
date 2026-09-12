/*===========================================================================

FILE:      GPIOCoreUser.c

DESCRIPTION:
This file provides the public class implementation of the GPIO driver.

PUBLIC CLASSES:
  GPIO_Init
  GPIO_Attach
  GPIO_RegisterPinExplicit
  GPIO_ConfigPin
  GPIO_ReadPin
  GPIO_WritePin

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/qdi/GPIOCoreUser.c#1 $

  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

#include "GPIO.h"
#include "GPIOTypes.h"
#include "GPIO.h"

#include "qurt.h"
#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt_futex.h"
#include "qurt_atomic_ops.h"

#include <stdlib.h>


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

GPIOResult GPIO_Attach
(
  GPIODeviceIdType      eDevice,
  GPIOClientHandleType *pHandle
)
{
  if ( pHandle != NULL )
  {
    pHandle->ClientId = qurt_qdi_open(GPIO_DRV_NAME);
    if ( pHandle->ClientId >= 0 )
    {
      if ( qurt_qdi_handle_invoke(pHandle->ClientId,
                                  GPIO_CMD_ATTACH,
                                  eDevice,
                                  pHandle) >= 0 )
      {
        return GPIO_SUCCESS;
      }
    }
  }
  return GPIO_ERROR;

} /* GPIO_Init */



/*=======================================================================================================

  FUNCTION      GPIO_RegisterPinExplicit

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_RegisterPinExplicit
(
  GPIOClientHandleType  Handle,
  uint32                nGpioNumber,
  GPIOAccessType        AccessType,
  GPIOKeyType          *GPIOKey
)
{
  if ( GPIOKey != NULL )
  {
    if ( qurt_qdi_handle_invoke(Handle.ClientId,
                                GPIO_CMD_REGISTER_PIN_EX,
                                Handle.DeviceId,
                                nGpioNumber,
                                AccessType,
                                GPIOKey) >= 0 )
    {
      return GPIO_SUCCESS;
    }
  }
  return GPIO_ERROR;

} /* GPIO_RegisterPinExplicit */



/*=======================================================================================================

  FUNCTION      GPIO_ConfigPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_ConfigPin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOConfigType       Config
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_CONFIG_PIN,
                              Handle.DeviceId,
                              GPIOKey,
                              &Config) >= 0 )
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* END GPIO_ConfigPin */



/*=======================================================================================================

  FUNCTION      GPIO_ReadPin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_ReadPin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOValueType       *pValue
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_READ_PIN,
                              Handle.DeviceId,
                              GPIOKey,
                              pValue) >= 0 )
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;  

} /* GPIO_ReadPin */



/*=======================================================================================================

  FUNCTION      GPIO_WritePin

  DESCRIPTION   See GPIO.h

  =====================================================================================================*/

GPIOResult GPIO_WritePin
(
  GPIOClientHandleType Handle,
  GPIOKeyType          GPIOKey,
  GPIOValueType        eValue
)
{
  if ( qurt_qdi_handle_invoke(Handle.ClientId,
                              GPIO_CMD_WRITE_PIN,
                              Handle.DeviceId,
                              GPIOKey,
                              eValue) >= 0 )
  {
    return GPIO_SUCCESS;
  }
  return GPIO_ERROR;

} /* GPIO_WritePin */


