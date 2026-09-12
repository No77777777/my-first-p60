/*==============================================================================

FILE:      uGPIOIntKernel.c

DESCRIPTION
  This modules implements the API to utilize the Micro GPIO interrupt controller for the Kernel.
  This Micro GPIO Interrupt Controller module is strictly used in KERNEL or GUEST OS Protection
  Domain and in Micro image.The definitions in this module are implemented internally
  and should not be exposed for external client usage directly without using the
  accompanying uGPIOInt.h in the core/api/systemdrivers interface file for this module.

REFERENCES

Copyright (c) 2014-2015 Qualcomm Technologies, Inc.
        All Rights Reserved.
Qualcomm Technologies, Inc. Confidential and Proprietary.
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/uimage/uGPIOIntKernel.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
1/1/2010   aratin  First draft created. 
===========================================================================*/
#include <atomic_ops.h>
#include <qurt.h>
#include "qurt_int.h"
#include "HALhwio.h"
#include "uGPIOInt.h"
#include "uGPIOIntKernel.h"
#include "uGPIOIntKernelInternal.h"

#ifdef GPIOINT_USE_MPM
#include "mpmint.h"
#endif /*GPIOINT_USE_MPM */

/*------------------------------------------------------------------------------
 GPIOInt Data Declarations.
------------------------------------------------------------------------------*/ 

/*
 * This is static uGPIOIntKernel state data. It can be accessed for debugging 
 * uGPIO Interrupts to see what is the current registration state of the GPIO.
 */
uGPIOIntKernelCntrlType uGPIOIntKernelData;

/*------------------------------------------------------------------------------
 GPIOInt Macro Declarations.
------------------------------------------------------------------------------*/ 
#define UGPIOINT_PRIORITY 4

#define VALID_UGPIO(x) ((x) < MAX_NUMBER_OF_GPIOS)

#define VALID_UINT(x) ((x) < 0xFF)

/*------------------------------------------------------------------------------
 GPIOInt Function Declarations.
------------------------------------------------------------------------------*/ 
static void uGPIOIntKernel_ConfigureMPMInterrupt(uint32 gpio,uGPIOIntTriggerType trigger);

/*==========================================================================

   FUNCTION      uGPIOIntKernel_Init

   DESCRIPTION   See uGPIOIntKernel.h

==========================================================================*/
int32 uGPIOIntKernel_Init(void)
{
  uint32 index;
 
  index = 0;

  /*
   * Initialize the uGPIO state table.
   */
  qurt_mutex_init(&uGPIOIntKernelData.uGPIOIntKernelLock);
  for(index =0;index < MAX_NUMBER_OF_GPIOS; index++)
  {
    uGPIOIntKernelData.state[index].qurt_intr_id = UGPIOINT_NONE;
    uGPIOIntKernelData.state[index].MPMId = UGPIOINT_NONE;  
  }

  return UGPIOINT_SUCCESS;
     
} /* END uGPIOIntKernel_Init */


/*==========================================================================

  FUNCTION      uGPIOIntKernel_ConfigureMPMInterrupt

  DESCRIPTION   This is an internal function to configure MPM interrupt.

==========================================================================*/
static void uGPIOIntKernel_ConfigureMPMInterrupt
(
  uint32              gpio,
  uGPIOIntTriggerType trigger
)
{
  #ifdef GPIOINT_USE_MPM
  mpmint_processor_type    mpm_proc;
  mpmint_detect_type       mao_int_detect;
  mpmint_polarity_type     mao_int_polarity;
  
  mpm_proc = MPMINT_LPASS;
  if (uGPIOIntKernelData.state[gpio].MPMId != UGPIOINT_NONE) 
  {
    
    switch (trigger)
    {

      case UGPIOINT_TRIGGER_HIGH:
        mao_int_detect = MPMINT_LEVEL_DETECT;
        mao_int_polarity = MPMINT_ACTIVE_HIGH;
        break;

      case UGPIOINT_TRIGGER_LOW:
        mao_int_detect = MPMINT_LEVEL_DETECT;
        mao_int_polarity = MPMINT_ACTIVE_LOW;
        break;

      case UGPIOINT_TRIGGER_RISING:
        mao_int_detect = MPMINT_EDGE_DETECT;
        mao_int_polarity = MPMINT_ACTIVE_HIGH;
        break;

      case UGPIOINT_TRIGGER_FALLING:
        mao_int_detect = MPMINT_EDGE_DETECT;
        mao_int_polarity = MPMINT_ACTIVE_LOW;
        break;

      default:
        mao_int_detect = MPMINT_LEVEL_DETECT;
        mao_int_polarity = MPMINT_ACTIVE_HIGH;
        break;
    }
  
    mpmint_config_gpio_wakeup(gpio, mao_int_detect,mao_int_polarity, mpm_proc);
  }
  #endif /*GPIOINT_USE_MPM */
  return;

} /* END uGPIOIntKernel_ConfigureMPMInterrupt */


/*==========================================================================

  FUNCTION      uGPIOIntKernel_SetMPMTrigger

  DESCRIPTION   See uGPIOIntKernel.h

==========================================================================*/
int32 uGPIOIntKernel_SetMPMTrigger
(
  uint32              gpio,
  uGPIOIntTriggerType trigger
)
{
  if (!VALID_UGPIO(gpio))
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  
  if (uGPIOIntKernelData.state[gpio].MPMId != UGPIOINT_NONE) 
  {
    uGPIOIntKernel_ConfigureMPMInterrupt(gpio,trigger);
  }
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 

  return UGPIOINT_SUCCESS;

}/* END uGPIOIntKernel_SetMPMTrigger */



/*==========================================================================

  FUNCTION      uGPIOIntKernel_UnConfigureMPMInterrupt

  DESCRIPTION   This is an internal function to disable MPM interrupt.

==========================================================================*/
void uGPIOIntKernel_UnConfigureMPMInterrupt
(
  uint32 gpio
)
{
  if (uGPIOIntKernelData.state[gpio].MPMId != UGPIOINT_NONE) 
  {
    #ifdef GPIOINT_USE_MPM
    mpmint_disable_gpio_wakeup(gpio);
    #endif /*GPIOINT_USE_MPM */
  }
  return;

} /* END uGPIOIntKernel_UnConfigureMPMInterrupt */


int32 uGPIOIntKernel_MapMPMInterrupt
(
  uint32 gpio,
  uint32 mpm_id
)
{
  if ( !VALID_UGPIO(gpio) || !VALID_UINT(mpm_id) )
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  uGPIOIntKernelData.state[gpio].MPMId = (uint8)mpm_id;
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 

  return UGPIOINT_SUCCESS;

}/* END uGPIOIntKernel_MapMPMInterrupt */

/*===========================================================================

FUNCTION    uGPIOIntKernel_SetGPIOIntConfig

DESCRIPTION
  This api sets the interrupt configuration in the Kernel.

DEPENDENCIES
  None.

RETURN VALUE
  Failure in setting the GPIO interrupt config returns -1 (UGPIOINT_QDI_ERROR)
  Success in setting the GPIO interrupt config returns 0 (UGPIOINT_QDI_SUCCESS)
 
SIDE EFFECTS
  None

===========================================================================*/
int32 uGPIOIntKernel_SetGPIOIntConfig
(
  uint32 gpio, 
  uGPIOIntTriggerType trigger,
  uint32 qurt_intr_id
)
{
  if ( !VALID_UGPIO(gpio) || !VALID_UINT(qurt_intr_id) )
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  uGPIOIntKernelData.state[gpio].qurt_intr_id = (uint8)qurt_intr_id;
  uGPIOIntKernel_ConfigureMPMInterrupt(gpio, (uGPIOIntTriggerType)trigger);
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 
  return UGPIOINT_SUCCESS;

} /* END  uGPIOIntKernel_SetGPIOIntConfig */


/*===========================================================================

FUNCTION    uGPIOIntKernel_ClearGPIOIntConfig

DESCRIPTION
  This api clears the interrupt configuration in the Kernel GPIOInt DAL

DEPENDENCIES
  None.

RETURN VALUE
  Failure in setting the trigger returns -1 (UGPIOINT_QDI_ERROR)
  Success in setting the interrupt trigger returns 0 (UGPIOINT_QDI_SUCCESS)
 
SIDE EFFECTS
  None

===========================================================================*/
int32 uGPIOIntKernel_ClearGPIOIntConfig
(
  uint32 gpio
)
{
  
  if (!VALID_UGPIO(gpio))
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  uGPIOIntKernelData.state[gpio].qurt_intr_id = UGPIOINT_NONE;
  uGPIOIntKernel_UnConfigureMPMInterrupt(gpio);
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 
  return UGPIOINT_SUCCESS;

} /* END  uGPIOIntKernel_ClearGPIOIntConfig */


/*==========================================================================

  FUNCTION      uGPIOIntKernel_SetDirectConnectGPIOMapping

  DESCRIPTION   This is an internal function to disable GPIO direct connect
                mapping from the Kernel GPIOInt Dal.

==========================================================================*/
int32 uGPIOIntKernel_SetDirectConnectGPIOMapping
(
  uint32 gpio,
  uint32 qurt_intr_id
)
{
  
  if ( !VALID_UGPIO(gpio) || !VALID_UINT(qurt_intr_id) )
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  uGPIOIntKernelData.state[gpio].qurt_intr_id = (uint8)qurt_intr_id;
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 
  return UGPIOINT_SUCCESS;

} /* END  uGPIOIntKernel_SetDirectConnectGPIOMapping */


/*==========================================================================

  FUNCTION      uGPIOIntKernel_ClearDirectConnectGPIOMapping

  DESCRIPTION   This is an internal function to disable GPIO direct connect
                mapping from the Kernel GPIOInt Dal.

==========================================================================*/
int32 uGPIOIntKernel_ClearDirectConnectGPIOMapping
(
  uint32 gpio
)
{
  
  if (!VALID_UGPIO(gpio))
  {
    return UGPIOINT_ERROR;
  }
  qurt_mutex_lock(&uGPIOIntKernelData.uGPIOIntKernelLock);
  uGPIOIntKernelData.state[gpio].qurt_intr_id = UGPIOINT_NONE;
  qurt_mutex_unlock(&uGPIOIntKernelData.uGPIOIntKernelLock); 
  return UGPIOINT_SUCCESS;

} /* END  uGPIOIntKernel_ClearDirectConnectGPIOMapping */

