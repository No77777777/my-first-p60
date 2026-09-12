/*===========================================================================

FILE:      GPIOCoreRtr.c

DESCRIPTION:
  This file contains the implementation of the GPIO Core driver QDI Router.
  This file is not used in environments that do not support the QURT QDI 
  layer.

PUBLIC CLASSES:
  GPIO_QDIInvoke
  GPIO_QDIOpen
  GPIO_QDIInvoke

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/src/qdi/GPIOCoreRoot.c#1 $

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
#include "qurt_island.h"

#include <stdlib.h>

/*=======================================================================================================

                               INTERNAL MACROS AND DATA STRUCTURES

  =====================================================================================================*/


  
/*
 * QDI Driver object type.
 */
typedef struct
{
  qurt_qdi_obj_t qdiobj;

}GPIOQDIDrvType;



/*=======================================================================================================

                                   INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/

extern int GPIO_QDIExtInvokeFunc
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
  

int GPIO_QDICoreInvokeFunc(int hClientHandle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9);
                          

/*
 * Invocation function for routing QDI requests to the associated GPIO APIs.
 */
static int GPIO_QDIInvoke(int hClientHandle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9);

/*
 * Release function for QDI objects.
 */
void GPIO_QDIRelease(qurt_qdi_obj_t *qdiobj);

void GPIO_QDIRelease
(
  qurt_qdi_obj_t *qdiobj
)
{
  //qdiobj is statically declared.  Do not use free.
  //free(qdiobj);
}

GPIOQDIDrvType GPIOQDIDrv;
const GPIOQDIDrvType GPIOQDIOpener = 
{
  {
    GPIO_QDIInvoke,
    QDI_REFCNT_INIT,
    GPIO_QDIRelease
  }
};

/* ============================================================================
**  Function : GPIO_QDIOpen
** ============================================================================
*/
/**
  Opens a QDI object.

  @param[in]  hClientHandle  - Handle to the requesting client.
  @param[in]  objptr         - Pointer to the object.
  @param[in]  devname        - The string name of the device.
  @param[in]  mode           - Unused.

  @return
  0  - success
  -1 - failure

  @dependencies
  None.

*/
int GPIO_QDIOpen
(
  int hClientHandle,
  qurt_qdi_obj_t *objptr,
  const char *devname,
  int mode
)
{
  /*
   * Initialize the QDI object function pointers and create the handle.
   */
  GPIOQDIDrv.qdiobj.invoke = GPIO_QDIInvoke;
  GPIOQDIDrv.qdiobj.refcnt = QDI_REFCNT_INIT;
  GPIOQDIDrv.qdiobj.release = GPIO_QDIRelease;

  return qurt_qdi_handle_create_from_obj_t(hClientHandle,&GPIOQDIDrv.qdiobj);

} /* GPIO_QDIOpen */



/* ============================================================================
**  Function : GPIO_QDIInvoke
** ============================================================================
*/
/**
  Main receiving API for QDI requests from user PD.  Based on whether execution
  is in uImage or if uImage is supported, this API will reroute to  either the 
  core invocation or the extended invocation functions.  The extended invocation
  function will reroute to the core if the requested API is not found.

  @param[in]  hClientHandle         - Handle to the requesting client.
  @param[in]  method                - The requested command.
  @param[in] ...

  @return
  0  - success
  -1 - failure

  @dependencies
  None.

*/
static int GPIO_QDIInvoke(int hClientHandle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9)
{
  if ( 0x0 == qurt_island_get_status() )
  {
    return GPIO_QDIExtInvokeFunc(hClientHandle, pobj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
  return GPIO_QDICoreInvokeFunc(hClientHandle, pobj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9);
  
} /* GPIO_QDIInvoke */



/* ============================================================================
**  Function : GPIO_QDICoreInvokeFunc
** ============================================================================
*/
/**
  This is the QDI invocation of all the core APIs in the system.
  There is an equivalent "Ext" or extended version of this implementation
  that invokes extended API functionality or re-routes back to this one
  if APIs are contained here.

  @param[in]  hClientHandle         - Handle to the requesting client.
  @param[in]  method                - The requested command.
  @param[in] ...

  @return
  0  - success
  -1 - failure

  @dependencies
  None.

*/

int GPIO_QDICoreInvokeFunc(int hClientHandle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9)
{
  boolean               bReturn = FALSE;
  GPIOResult            eRes = GPIO_ERROR;
  GPIOClientHandleType  CHandle;
  GPIOConfigType       *pConfig = NULL;  

  switch ( method )
  {
    case QDI_OPEN:
      return GPIO_QDIOpen(hClientHandle, (void *)pobj, a1.ptr, a2.num);

    case GPIO_CMD_ATTACH:
      eRes = GPIO_Attach(a1.num, (GPIOClientHandleType*)a2.ptr);
      break;
      
    case GPIO_CMD_REGISTER_PIN_EX:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_RegisterPinExplicit(CHandle, (uint32)a2.num, (GPIOAccessType)a3.num, (GPIOKeyType*)a4.ptr);
      break;
    
    case GPIO_CMD_CONFIG_PIN:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      pConfig = (GPIOConfigType*)a3.ptr;
      eRes = GPIO_ConfigPin(CHandle, (GPIOKeyType)a2.num, *pConfig);
      break;
      
    case GPIO_CMD_READ_PIN:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_ReadPin(CHandle, (GPIOKeyType)a2.num, (GPIOValueType*)a3.ptr);
      break;
      
    case GPIO_CMD_WRITE_PIN:
      CHandle.ClientId = hClientHandle;
      CHandle.DeviceId = a1.num;
      eRes = GPIO_WritePin(CHandle, (GPIOKeyType)a2.num, (GPIOValueType)a3.num);
      break;

    case QDI_ISLAND_CHECK_ALLOC:
      return 1;

    default:
      return qurt_qdi_method_default(hClientHandle, pobj, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }

  if ( bReturn == TRUE || eRes == GPIO_SUCCESS )
  {
    return 0;
  }

  return -1;

} /* End GPIO_QDIInvoke */


boolean GPIO_RouterInit(void)
{
  if (qurt_qdi_register_devname(GPIO_DRV_NAME, &GPIOQDIOpener) == 0)
  {
    return TRUE;
  }
  return FALSE;
}

