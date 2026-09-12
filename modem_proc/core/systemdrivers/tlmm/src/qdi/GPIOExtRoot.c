/*===========================================================================

FILE:      GPIO.c

DESCRIPTION:
This file contains the hardware abstraction for the micro GPIO driver.
This file provides the public class implementation of the GPIO driver.

PUBLIC CLASSES:
  GPIO_Init
  GPIO_Attach
  GPIO_GetGpioKey
  GPIO_ConfigPin
  GPIO_IsPinInUse
  GPIO_ReadPin
  GPIO_DriveOutput

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/qdi/GPIOExtRoot.c#1 $

  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

#include "GPIO.h"
#include "GPIODiag.h"
#include "GPIOTypes.h"
#include "GPIOExt.h"

#include "qurt.h"
#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt_futex.h"
#include "qurt_atomic_ops.h"

/*=======================================================================================================

                               INTERNAL MACROS AND DATA STRUCTURES

  =====================================================================================================*/

extern int GPIO_QDICoreInvokeFunc
(
  int             hClientHandle,
  qurt_qdi_obj_t *pobj,
  int             method,
  qurt_qdi_arg_t  a1,
  qurt_qdi_arg_t  a2,
  qurt_qdi_arg_t  a3,
  qurt_qdi_arg_t  a4,
  qurt_qdi_arg_t  a5,
  qurt_qdi_arg_t  a6,
  qurt_qdi_arg_t  a7,
  qurt_qdi_arg_t  a8,
  qurt_qdi_arg_t  a9
);

/*=======================================================================================================

                                   INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/


/* ============================================================================
**  Function : GPIO_QDIInvoke
** ============================================================================
*/
/**
  Main receiving API for QDI requests from user PD.

  @param[in]  hClientHandle         - Handle to the requesting client.
  @param[in]  method                - The requested command.
  @param[in] ...

  @return
  0  - success
  -1 - failure

  @dependencies
  None.

*/
int GPIO_QDIExtInvokeFunc
(
  int             hClientHandle,
  qurt_qdi_obj_t *pobj,
  int             method,
  qurt_qdi_arg_t  a1,
  qurt_qdi_arg_t  a2,
  qurt_qdi_arg_t  a3,
  qurt_qdi_arg_t  a4,
  qurt_qdi_arg_t  a5,
  qurt_qdi_arg_t  a6,
  qurt_qdi_arg_t  a7,
  qurt_qdi_arg_t  a8,
  qurt_qdi_arg_t  a9
)
{
  boolean               bReturn = FALSE;
  GPIOResult            eRes = GPIO_ERROR;
  GPIOClientHandleType  CHandle;
  GPIOConfigType       *pConfig = NULL;

  switch ( method )
  {
    case GPIO_CMD_REGISTER_PIN:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_RegisterPin(CHandle, (const char*)a2.ptr, /*(uint32)a3.num, */(GPIOAccessType)a4.num, (GPIOConfigType*)a5.ptr, (GPIOKeyType*)a6.ptr);
      break;

    case GPIO_CMD_CONFIG_PIN_INACTIVE:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_ConfigPinInactive(CHandle, (GPIOKeyType)a2.num);
      break;

    case GPIO_CMD_IS_PIN_ACTIVE:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      bReturn = GPIO_IsPinActive(CHandle, (GPIOKeyType)a2.num);
      break;

    case GPIO_CMD_GET_PIN_CONFIG:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_GetPinConfig(CHandle, a2.num, a3.ptr);
      break;

    case GPIO_CMD_GET_PIN_OUTPUT:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      bReturn = GPIO_GetOutput(CHandle, (GPIOKeyType)a2.num);
      break;

    case GPIO_CMD_GET_INACTIVE_CONFIG:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_GetInactiveConfig(CHandle, (GPIOKeyType)a2.num, (GPIOConfigType*)a3.ptr, (GPIOValueType*)a4.ptr);
      break;

    case GPIO_CMD_SET_INACTIVE_CONFIG:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      pConfig = (GPIOConfigType*)a3.ptr;
      eRes = GPIO_SetInactiveConfig(CHandle, (GPIOKeyType)a2.num, *pConfig, (GPIOValueType)a4.num);
      break;

    case QDI_ISLAND_CHECK_ALLOC:
      return 0;

    default:
      return GPIO_QDICoreInvokeFunc(hClientHandle, pobj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }

  if ( bReturn == TRUE || eRes == GPIO_SUCCESS )
  {
    return 0;
  }

  return -1;

} /* End GPIO_QDIExtInfokeFunc */

