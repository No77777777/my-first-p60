#ifndef __UGPIOIntKernel_H
#define __UGPIOIntKernel_H
/*===========================================================================

  D A L   G P I O   I N T E R R U P T   C O N T R O L L E R    

DESCRIPTION
  This modules contains enumeration definitions to support the Kernel uGPIO 
  interrupt controller. The definitions in this module are used internally 
  in the dal GPIO interrupt controller or the QDI interface and are not to be
  exposed to the clients or the user PD uGPIO driver directly . These Apis
  support the maintainance of GPIOInt state across all protection domains.

REFERENCES

       Copyright (c) 2014-2015 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/inc/uGPIOIntKernel.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
1/1/2010   aratin  First draft created. 
===========================================================================*/
#include "com_dtypes.h"
#include "qurt.h"
#include "uGPIOInt.h"

/*------------------------------------------------------------------------------
Declaring uGPIOInt Driver data
------------------------------------------------------------------------------*/




/*=========================================================================

        FUNCTIONS TO BE USED BY THE QDI LAYER or THE KERNEL DAL.

  FUNCTION      uGPIOIntKernel_SetGPIOIntConfig

  DESCRIPTION   
  This function sets the GPIO interrupt trigger configuration to the mpm interrupt
  driver . It also marks the direct connect line to the main interrupt controller
  to which the GPIO is connected as an interrupt.

  PARAMETERS   
  
  uint32 gpio :                     The GPIO pin whose interrupt state needs to
                                    be cleared.
  uGPIOIntTriggerType trigger : The TLMM trigger configuration of the GPIO interrupt.

  DEPENDENCIES  None.

  RETURN VALUE  :
  uGPIOIntKernel_SUCCESS : this result is obtained when the gpio interrupt trigger is successfully
  communicated to the mpm driver and the GPIO interrupt was mapped to the correct direct line
  to the main interrupt controller.
  uGPIOIntKernel_ERROR : this result is returned when the driver failed to set the interrupt trigger
  to the MPM driver or its configuration was not marked as a direct connect interrupt.
  

  SIDE EFFECTS  None.

=============================================================================*/
int32 uGPIOIntKernel_SetGPIOIntConfig
(
  uint32 gpio, 
  uGPIOIntTriggerType trigger,
  uint32 qurt_intr_id
);

/*=============================================================================

  FUNCTION      uGPIOIntKernel_ClearGPIOIntConfig

  DESCRIPTION   
  This function clears the GPIO interrupt mapping to direct connect line of the
  main interrupt controller and disables the mpm trigger.

  PARAMETERS   
  
  uint32 gpio :                     The GPIO pin whose interrupt state needs to
                                    be cleared.

  DEPENDENCIES  None.

  RETURN VALUE  :
  uGPIOIntKernel_SUCCESS : this result is obtained when the gpio state is successfully
  cleared.
  uGPIOIntKernel_ERROR : this result is returned when the driver failed to clear
  the GPIO state.

  SIDE EFFECTS  None.

=============================================================================*/
int32 uGPIOIntKernel_ClearGPIOIntConfig
(
  uint32 gpio
);


/*=============================================================================

  FUNCTION      uGPIOIntKernel_SetMPMTrigger

  DESCRIPTION   
  This function sends the GPIO interrupt trigger configuration to the mpm interrupt
  driver .

  PARAMETERS   
  
  uint32 gpio :                     The GPIO pin whose interrupt state needs to
                                    be cleared.
  uGPIOIntTriggerType trigger : The TLMM trigger configuration of the GPIO interrupt.

  DEPENDENCIES  None.

  RETURN VALUE  :
  uGPIOIntKernel_SUCCESS : this result is obtained when the gpio interrupt trigger
  is successfully communicated to the mpm driver.
  uGPIOIntKernel_ERROR : this result is returned when the driver failed to set
  the interrupt trigger.
  

  SIDE EFFECTS  None.

=============================================================================*/



/*=============================================================================

  FUNCTION      uGPIOIntKernel_SetDirectConnectGPIOMapping

  DESCRIPTION   
  This function is used by the GPIOInt Dal driver to log the direct connect
  interrupts in DDR for Guest OS. This is so that in uimage the micro
  interrupt controller can trigger these interrupts quickly.

  PARAMETERS   
  
  uint32 gpio :         The GPIO pin whose interrupt state needs to be logged.
  uint32 qurt_intr_id : The QURT direct connect line to which the GPIO interrupt
                        is configured to.

  DEPENDENCIES  None.

  RETURN VALUE  :
  uGPIOIntKernel_SUCCESS : this result is obtained when the gpio interrupt mapping
  is successfully communicated to the uGPIOInt driver.
  uGPIOIntKernel_ERROR : this result is returned when the driver failed to set
  the interrupt mapping for uGPIOInt driver.
  

  SIDE EFFECTS  None.

=============================================================================*/


/*=============================================================================

  FUNCTION      uGPIOIntKernel_ClearDirectConnectGPIOMapping

  DESCRIPTION   
  This function is used by the GPIOInt Dal driver clear the GPIO to main
  direct connect interrupt vector id.

  PARAMETERS   
  
  uint32 gpio :         The GPIO pin whose interrupt state needs to be cleared.
  
  DEPENDENCIES  None.

  RETURN VALUE  :
  uGPIOIntKernel_SUCCESS : this result is obtained when the gpio interrupt mapping
  is successfully cleared from the uGPIOInt driver.
  uGPIOIntKernel_ERROR : this result is returned when the driver failed to clear
  the interrupt mapping for uGPIOInt driver.
  

  SIDE EFFECTS  None.

=============================================================================*/

int32 uGPIOIntKernel_MapMPMInterrupt
(
  uint32 gpio,
  uint32 mpm_id
);





#endif /* __UGPIOIntKernel_H */
