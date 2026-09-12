#ifndef _INTERRUPTCONTROLLERUSER_INTERFACE_H_
#define _INTERRUPTCONTROLLERUSER_INTERFACE_H_
/*==============================================================================

FILE:      InterruptControllerInterface.h

DESCRIPTION: This file defines the interrupt controller data that is used
             in device configuration of the interrupt controller driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A

        Copyright (c) 2012-2017 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/settings/systemdrivers/InterruptController/interface/InterruptControllerUserInterface.h#2 $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------
02/13/17   dcf     Initial version.

===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

/*=========================================================================

                           CONSTANTS

===========================================================================*/

/*=========================================================================

                           CONSTANTS

===========================================================================*/
#define MAX_QURT_IRQ 310
#ifndef DALINTERRUPT_SUPPORTED_INTERRUPTS
  #define DALINTERRUPT_SUPPORTED_INTERRUPTS MAX_QURT_IRQ
#else
  #define DALINTERRUPT_MEMORY_SAVE_MODE
#endif

/*
 * IST THREAD PRIORITY  LIMITS
 *
 * Base IST priority level. This is the lowest priority of an interrupt service thread.
 * Highest priority level. These two limits are checks on the range of the Interrupt 
 * Thread priorities that can be set.
 */
#define IST_THREAD_PRIORITY_BASE    30
#define IST_THREAD_HIGHEST_PRIORITY 1

#define IST_THREAD_SET_FW_PRIORITY(x) ((x) | 0x80)
#define IST_THREAD_GET_FW_PRIORITY(x) ((x) & 0x7f)

 /*
  *  Invalid interrupt value for error checking
  */
#define INVALID_INTERRUPT                0x7FFFFFFF
#define IST_TASKNAME_SIZE 16
/* 
 * For each interrupt handler thread a Default IST stack size is declared.
*/
#define DAL_IST_STACK_SIZE       2048
#define DAL_IST_BASE_STACK_SIZE  1024
#define DAL_IST_STACK_SIZE_4k    4096

/* Type of IST configuration */
#define DAL_IST_CONFIG_DYNAMIC      1

/*
 * Interrupt Controller internal interrupt state flags.
 */
#define INTERRUPTF_MPM           0x01
#define INTERRUPTF_DISABLED      0x02
#define INTERRUPTF_DSR           0x04
#define INTERRUPTF_REGISTERED    0x08
#define INTERRUPTF_RUNNING       0x10
#define INTERRUPTF_PEND_RUN      0x20
#define INTERRUPTF_DEREGISTER    0x40
#define INTERRUPTF_UINTERRUPT    0x80

/*
 * Return signals for registering interrupts
 */
#define INTERRUPT_REGISTER_SUCCESS  0x1
#define INTERRUPT_REGISTER_FAIL     0x2


/*
 * InterruptConfigType
 */
typedef struct
{
  uint32                     nInterruptVector;
  uint32                     nISTStackSize;
  char                      *pInterruptName;
  uint8                      nPriority;
  uint8                      nTrigger;
} InterruptConfigType;


 /*
  * InterruptPlatformDataType
  *
  * pIRQConfigs            - Pointer to an array of interrupt definitions in the BSP.
  * nMaxIRQ                - Maximum number of supported interrupts.
  */
typedef struct
{
  InterruptConfigType       *pIRQConfigs;
  uint32                    nMaxIRQ;
} InterruptPlatformDataType;


#endif /* _INTERRUPTCONTROLLERUSER_INTERFACE_H_ */

