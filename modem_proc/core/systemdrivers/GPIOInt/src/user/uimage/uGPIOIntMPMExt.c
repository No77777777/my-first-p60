/*==============================================================================

  G P I O   I N T E R R U P T   C O N T R O L L E R  Q D I   W R A P P E R  

DESCRIPTION
  This modules contains the Qurt Driver Invocation (QDI) layer that interfaces
  with the driver in the kernel on behalf of the user clients in the user domain.

REFERENCES

       Copyright (c) 2012 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
 
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/user/uimage/uGPIOIntMPMExt.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
1/1/2010   aratin  First draft created. 
===========================================================================*/
#include "uGPIOIntQDI.h"
#include "uGPIOInt.h"
#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt.h"
#include "qurt_futex.h"
#include "qurt_atomic_ops.h"
#include "stdlib.h"
#include "err.h"
#include "uGPIOIntKernel.h"

int ugpioint_qdi,ugpioint_qdiinit;

/*===========================================================================
FUNCTION uGPIOIntQDI_Init
 
DESCRIPTION 
  This function is called in RCInit . Its dependent on GPIOInt DAL in Guest OS.
  Only when the Guest OS uGPIOInt_Init is called then can this function be called.
  It registers the GPIOInt QDI wrapper with QDI Framework.
===========================================================================*/
void uGPIOIntMPMExt_Init(void)
{
  if(ugpioint_qdiinit == 0)
  {
    ugpioint_qdi = qurt_qdi_open(uGPIOIntQdiName);
    ugpioint_qdiinit = 1;
  }
  return;
}


int32 uGPIOIntKernel_MapMPMInterrupt
(
  uint32 nGPIO,
  uint32 mpmid
)
{
   uGPIOIntMPMExt_Init();
   if(ugpioint_qdi >0)
   {
      return qurt_qdi_handle_invoke(ugpioint_qdi, 
                                    UGPIOINT_QDI_MAP_UGPIOINT,
                                    (uint32)nGPIO,
                                    (uint32)mpmid);
   }
   else
   {
      return UGPIOINT_ERROR;
   }
}


int32 uGPIOIntKernel_SetGPIOIntConfig
(
  uint32 gpio, 
  uGPIOIntTriggerType trigger,
  uint32 qurt_intr_id
)
{
   uGPIOIntMPMExt_Init();
   if(ugpioint_qdi >0)
   {
      return qurt_qdi_handle_invoke(ugpioint_qdi, 
                            UGPIOINT_QDI_SET_GPIO_CONFIG,
                            (uint32)gpio, (uint32) trigger,
                            (uint32)qurt_intr_id);
   }
   else
   {
      return UGPIOINT_ERROR;
   }
}

int32 uGPIOIntKernel_ClearGPIOIntConfig
(
  uint32 nGPIO
)
{
   uGPIOIntMPMExt_Init();
   if(ugpioint_qdi >0)
   {
      return qurt_qdi_handle_invoke(ugpioint_qdi, 
                            UGPIOINT_QDI_CLEAR_GPIO_CONFIG,
                            (uint32)nGPIO);
   }
   else
   {
      return UGPIOINT_ERROR;
   }
}


