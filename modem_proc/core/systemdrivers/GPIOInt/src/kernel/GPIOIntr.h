#ifndef __GPIOIntr_H
#define __GPIOIntr_H
/*===========================================================================

       G P I O   I N T E R R U P T   C O N T R O L L E R    

DESCRIPTION
  This modules contains enumeration definitions to support the GPIO 
  interrupt controller. The definitions in this module are used internally 
  in the GPIO interrupt controller and are not to be exposed to the clients.

REFERENCES

       Copyright © 2010-2017 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOIntr.h#1 $

when        who     what, where, why
--------    ---     ---------------------------------------------------------- 
5/18/2017   hvm     GPIOInt driver refactored. 
===========================================================================*/
#include "DDIGPIOInt.h"
#include "ULog.h"
#include "ULogFront.h"
#include "GPIOIntInterface.h"
#include "qurt_rmutex.h"

/*------------------------------------------------------------------------------
 GPIO interrupt controller constant declarations.
------------------------------------------------------------------------------*/

/*
 * Size of the GPIO log
 */
#define MAX_INTERRUPT_LOG_ENTRIES                100

/*
 * As an Upper limit we keep the maximum number of GPIOs as 200.
 * This number needs to be updated if the GPIO number exceeds 200.
 */
#define MAX_NUMBER_OF_GPIOS                      200

/* 
 * Default Init values for State Table members
 */
#define GPIOINT_NO_QURT_ID                    0x7FFF
#define GPIOINT_NO_DEVICETYPE                 0xFFFF


/*
 * GPIOINT_NONE
 *
 * Value returned by GetPending to indicate no more interrupts pending.
 */
#define GPIOINT_NONE                          0xFFFF


/* 
 * Macro to determine if GpioInterrupt is a Direct Connect Type
 */
#define GPIOINT_IS_DIRCONN(interrupt) (interrupt < GPIOINT_SUMMARY)


/*
 * Macro lock the driver mutex.
 */
#define GPIOINT_LOCK(devCtxt) qurt_rmutex_lock(&devCtxt->hLock)


/*
 * Macro try-lock the driver mutex.
 */
#define GPIOINT_TRYLOCK(devCtxt) qurt_rmutex_try_lock(&devCtxt->hLock)


/*
 * Macro to free the driver mutex.
 */
#define GPIOINT_FREE(devCtxt) qurt_rmutex_unlock(&devCtxt->hLock)

/*=========================================================================

                           DATA TYPE DEFINITIONS

===========================================================================*/

typedef struct GPIOIntr_ClientHandle GPIOIntr_ClientHandle;
typedef struct GPIOIntr_DeviceHandle GPIOIntr_DeviceHandle;

/*
 * GPIOIntdata_type
 * Structure storing the details for a Gpio interrupt
 */
typedef struct
{
  GPIOINTISR                     isr;
  GPIOINTISRCtx                  isr_param;
  uint16                         gpio_intr_flags;
  uint32                         nDeviceType;
  uint32                         mpm_intr_id;
  GpioIntr_CfgType               *pIntrCfg;
} GPIOIntr_DataType;


/*
 * GPIOIntlog_entry_type
 *
 * Log entry type.
 *
 *  gpio:         The gpio number
 *  timestamp:    The timestamp in sclks.
 */
typedef struct
{
  uint32                         gpio;
  uint64                         timestamp;
} GPIOIntr_LogEntryType;

/*
 * GPIOIntlog_type
 *
 * Log type container.
 */
typedef struct
{
  uint32                         index;
  GPIOIntr_LogEntryType          entries[MAX_INTERRUPT_LOG_ENTRIES];
} GPIOIntr_LogType;

/*
 * GPIOIntIsrDataType
 *
 * work loop thread data to be passed to.
 */
typedef struct
{
  uint32                         gpio;
  GPIOIntr_DeviceHandle          *device;
} GPIOIntr_IsrDataType;

/*
 * GPIOIntCntrlType
 *
 * Container for all local data.
 *
 * initialized: Indicates if the driver has been started or not.  Needed
 *              mostly because some compilers complain about empty structs.
 * table:       Table of registered GPIO_INT handler functions.
 * wakeup_isr:  ISR to invoke when a monitored GPIO interrupt triggers.
 * log:         Log storage.
 */
typedef struct
{
  /* GPIOInt Dev state can be added by developers here */

  /* Flag to Initialize GPIOInt_Init is called first 
   * before anything else can attach 
   */
  uint8                          GPIOInt_Init;

  /* interrupt_state Table of registered GPIO_INT handler functions */
  GPIOIntr_DataType              state[MAX_NUMBER_OF_GPIOS];

  /* Interrupt Log storage.*/
  GPIOIntr_LogType               log;

  /* total number of GPIOs present on the target */
  uint32                         total_gpio_number;

  /* 
   * This keeps track of the gpios that are configured as 
   * direct connect interrupts.
   */
  GPIOIntProcessorType           processor;
 
  /*
   * fake trigger flag for gpios that are triggered in software.
   */
  uint32*                        gpioint_physical_address;
  uint32*                        gpioint_virtual_address;

  /* Flag to indicate usage of uimage GpioInt driver 
   */
  uint8                          gpioint_uimage;

  /* Flag to indicate if main GpioInt driver is initialized
   */
  uint8                          is_initialized;

  /* total number of GPIO Intr Drivers present on the target */
  uint32                         gpioint_priority_list_count;

  /* Priority List for Supported Types of GpioIntr Drivers. */
  GPIOIntFnDescType              **gpioint_priority_list;

} GPIOIntr_CntrlType;


/*
 * GPIOInt_TargetDataType
 *
 * Target data for the driver
 */ 
typedef struct
{
  GPIOIntProcessorType eProcessor;
  uint32*  nGPIOIntBase;
  uint32*  nGPIOIntBasePhys;
  uint32   nTargetProcVersion;
} GPIOInt_TargetDataType;


/*=========================================================================

                           DATA DEFINITIONS

===========================================================================*/

struct GPIOIntr_ClientHandle
{
  uint32                         nGpioInUse;
  GPIOIntr_DeviceHandle *        hDevice;
};

struct GPIOIntr_DeviceHandle
{
  uint8                          bIsInitialized;
  qurt_mutex_t                   hLock;
  DalDeviceHandle *              hGPIOMgrHandle;
  DalDeviceHandle *              hDalMpmHandle;
  ULogHandle                     hULog;
  GPIOIntr_CntrlType *           pGPIOIntData;
};

/*=============================================================================

                    Internal Functions for GPIOInt
 
=============================================================================*/

/*===========================================================================

FUNCTION      GPIOIntr_GetTargetData

  DESCRIPTION  
  This is an internal function that sets up the target processor data that each 
  instance of interrupt controller needs.

  PARAMETERS    
  GPIOIntClientCtxt * :The client context pointer of the GPIOInt DAL.

  DEPENDENCIES  None.

  RETURN VALUE  
  DALResult.
  DAL_SUCCESS :
  is returned if the GPIOInt target specific data was successfully
  extracted from the parsed xml file.
  DAL_ERROR :
  DAL_ERROR is returned if the xml data was corrupt for any reason.
  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_GetTargetData
(
  GPIOIntr_ClientHandle * pCtxt
);

/*=============================================================================

  FUNCTION      GPIOIntr_Isr

  DESCRIPTION   
  This is the SUMMARY interrupt isr routine that is called by the main interrupt 
  controller whenever any SUMMARY gpio interrupt is fired.

  PARAMETERS    
  void * -  Data Context to be passed to the ISR as an argument.

  DEPENDENCIES  None.

  RETURN VALUE  None.

  SIDE EFFECTS  None.

=============================================================================*/

void 
GPIOIntr_Isr
(
  void *pCtxt
);

/*=============================================================================

  FUNCTION      GPIOIntr_MapGPIOIntTriggerToFlag

  DESCRIPTION   
  This api maps GPIOInt Trigger to a GPIOInt Flag
 
  PARAMETERS    
  uint32  nGPIOTrigger : The GPIOInt Trigger.
  uint32* nGpioIntFlag : [Out] GPIOInt Flag.

  DEPENDENCIES  None.

  RETURN VALUE  DAL_SUCCESS : Mapped successfully.
                DAL_ERROR   : Otherwise.

  SIDE EFFECTS  None.


=============================================================================*/

DALResult
GPIOIntr_MapGPIOIntTriggerToFlag
(
  uint32  nGPIOTrigger,
  uint32* nGpioIntFlag
);

/*=============================================================================

                                  External APIs

 ============================================================================*/

/*=============================================================================

  FUNCTION      GPIOIntr_ClearInterrupt

  DESCRIPTION   This function clears the summary interrupt without 
                deregistering the interrupt handler.
   
  PARAMETERS    
  GPIOIntClientCtxt * pCtxt : Pointer to the driver client context .
  uint32 gpio :               GPIO pin number.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult : this indicates if the dal attach was a failure.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_ClearInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
);

/*=============================================================================
  
  FUNCTION      GPIOIntr_SetTrigger

  DESCRIPTION   This is an function sets the trigger of a given GPIO
                interrupt.

  PARAMETERS     
  GPIOIntClientCtxt * :        The client context pointer of the interrupt 
                               controller DAL.
  uint32 gpio :                Number of the interrupt.
  GPIOIntTriggerType eTrigger: Type of interrupt trigger.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_SetTrigger
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  GPIOIntTriggerType  eTrigger
);

/*=============================================================================

  FUNCTION      GPIOIntr_RegisterIsr

  DESCRIPTION   This function registers the isr routine to a given GPIO 
  interrupt.Whenever an interrupt is fired, the isr routine registered by the 
  user is called.

  PARAMETERS     
  GPIOIntClientCtxt * :        The client context pointer of the interrupt 
                               controller DAL.
  uint32  gpio :               GPIO pin number of the interrupt.
  GPIOIntTriggerType eTrigger: Interrupt trigger specified by the client for 
                               GPIO signal transitions.
  GPIOINTISR  isr :            The Interrupt Service routine for the callback 
                               function pointer.
  GPIOINTISRCtx param :        The uint32 parameter given to the isr routine 
                               when called.

  DEPENDENCIES  None.

  DAL_ERROR :
  Returns an Error if the GPIOInt driver was not able to register the interrupt
  service routine (ISR). This can happen if the GPIO interrupt already has a 
  prior event handle or a callback function interrupt Service routine registered 
  to it which is different from the current one in attempt at event registration. 
  An error is also returned if the GPIO pin is out of bounds or not supported.
  DAL_SUCCESS :
  Returns a success if the ISR registration was a success.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_RegisterIsr
(
  GPIOIntr_ClientHandle * pCtxt,
  uint32 gpio,
  GPIOIntTriggerType eTrigger,
  GPIOINTISR isr,
  GPIOINTISRCtx param,
  uint32 nFlags
);

/*===========================================================================

  FUNCTION      GPIOIntr_DeregisterIsr

  DESCRIPTION   This function deregisters the isr routine to a given GPIO 
                interrupt.Whenever an interrupt is fired the isr routine 
                registered by the user is called.

  PARAMETERS     
  GPIOIntClientCtxt * :    The client context pointer of the interrupt controller 
                           DAL.
  uint32 gpio         :    Interrupt number.
  GPIOINTISR  isr :        The Interrupt Service routine for the callback 
                           function pointer.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_DeregisterIsr
( GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  GPIOINTISR isr
);

/*=============================================================================

  FUNCTION      GPIOIntr_RegisterEvent

  DESCRIPTION   This function registers the client callback event to a given 
  GPIO interrupt.Whenever an interrupt is fired, the event registered by the
  user for that GPIO interrupt is called.

  PARAMETERS     
  GPIOIntClientCtxt * :        The client context pointer of the interrupt 
                               controller DAL.
  uint32  gpio :               GPIO pin number of the interrupt.
  GPIOIntTriggerType eTrigger: Interrupt trigger specified by the client for 
                               GPIO signal transitions.
  DALSYSEventHandle event :    Client callback event that needs to be triggered
                               when the interrupt fires.
                

  DEPENDENCIES  None.

  RETURN VALUE  
  DALResult.  : 
  DAL_ERROR Returns an Error if the GPIOInt driver was not able to register the 
  client event. This can happen if the GPIO interrupt already has a prior event 
  handle or a callback function interrupt Service routine registered to it which 
  is different from the current one in attempt at event registration. 
  An error is also returned if the
  GPIO pin is out of bounds or not supported.
  DAL_SUCCESS :
  Returns a success if the client event registration was a success.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_RegisterEvent
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio,
  GPIOIntTriggerType eTrigger, 
  DALSYSEventHandle event
) ;

/*=============================================================================

  FUNCTION      GPIOIntr_DeRegisterEvent

  DESCRIPTION   This function deregisters the client callback event to a given
  GPIO interrupt. The interrupt is disabled.

  PARAMETERS     
  GPIOIntClientCtxt * :     The client context pointer of the interrupt 
                            controller DAL.
  uint32  gpio :            GPIO pin number of the interrupt.
  DALSYSEventHandle event : Client callback event that needs to be triggered 
                            when the interrupt fires.

  DEPENDENCIES  None.

  RETURN VALUE  
  DALResult.  : 
  DAL_ERROR Returns an Error if the GPIOInt driver was not able to deregister 
  the client event. This can happen if the GPIO interrupt already has a prior 
  event handle or a callback function interrupt Service routine registered to it 
  which is different from the current one in attempt at event deregistration. 
  An error is also returned if the GPIO pin is out of bounds or not supported.
  DAL_SUCCESS :
  Returns a success if the client event deregistration was a success.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_DeRegisterEvent
(
  GPIOIntr_ClientHandle * pCtxt,
  uint32 gpio,
  DALSYSEventHandle event
);

/*=============================================================================

  FUNCTION      GPIOIntr_IsInterruptEnabled

  DESCRIPTION   This function tells the user if the given GPIO interrupt is 
                enabled or not. For Direct connect interrupts. It tells the 
                user if the main level 1 interruptto which the secondary GPIO 
                interrupt line is hooked up to is enabled.

  PARAMETERS    GPIOIntClientCtxt *:
                the client context pointer of the interrupt controller DAL.
                uint32 gpio
                uint32 * state that is given back to the user that 
                indicates whether the given interrupt is enabled.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_IsInterruptEnabled
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  uint32 * state
);

/*=============================================================================

  FUNCTION      GPIOIntr_IsInterruptRegistered

  DESCRIPTION   This function tells the user if the given GPIO interrupt is 
                registered or not. 

  PARAMETERS    GPIOIntClientCtxt *:
                the client context pointer of the interrupt controller DAL.
                uint32 gpio
                uint32 * state that is given back to the user that 
                indicates whether the given interrupt is enabled.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_IsInterruptRegistered
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  uint32 * state
);

/*=============================================================================

  FUNCTION      GPIOIntr_IsInterruptPending

  DESCRIPTION   This function tells the user if the given gpio interrupt is 
                pending or not.

  PARAMETERS    GPIOIntClientCtxt * :
                the client context pointer of the interrupt controller DAL.
                uint32 gpio
                uint32 * state that is given back to the user that 
                indicates whether the given interrupt is pending.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_IsInterruptPending
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  uint32 * state 
);

/*=============================================================================

  FUNCTION      GPIOIntr_IsInterruptSet

  DESCRIPTION   This function tells the user if the given gpio interrupt is 
                fired or not.This is regardless of it being enabled.

  PARAMETERS    GPIOIntClientCtxt * :
                the client context pointer of the interrupt controller DAL.
                uint32 gpio
                uint32 * state that is given back to the user that 
                indicates whether the given interrupt is pending.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_IsInterruptSet
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  uint32 * state 
);

/*=============================================================================
  
  FUNCTION      GPIOIntr_Save

  DESCRIPTION   
  This function is the default power saving function that saves any volatile data 
  or state during power collapse.

  PARAMETERS    
  GPIOIntClientCtxt * :        The client context pointer of the interrupt 
                               controller DAL.
  GPIOIntPowerCollapseType :   Type of power collapse the chip is in.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.
=============================================================================*/

DALResult GPIOIntr_Save
( 
  GPIOIntr_ClientHandle * pCtxt
);

/*=============================================================================

  FUNCTION      GPIOIntr_Restore

  DESCRIPTION   This function is the default power saving function that 
                restores any saved data or state during power collapse upon 
                coming out of power collapse. As of now there is no percieved 
                implementation of this function in use.

  PARAMETERS    GPIOIntClientCtxt * :
                the client context pointer of the interrupt controller DAL.
                GPIOIntPowerCollapseType type of power collapse the 
                chip is in.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_Restore
( 
  GPIOIntr_ClientHandle * pCtxt
);

/*=============================================================================
  
  FUNCTION      GPIOIntr_DisableInterrupt

  DESCRIPTION   This is an internal function that is used to disable the 
                SUMMARY and DIRECT CONNECT interrupts.

  PARAMETERS    GPIOIntClientCtxt * :
                the client context pointer of the interrupt controller DAL.
                uint32 gpio number to which the interrupt is assigned.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_DisableInterrupt
( 
  GPIOIntr_ClientHandle * pCtxt,
  uint32 gpio
);

/*=============================================================================

  FUNCTION      GPIOIntr_EnableInterrupt

  DESCRIPTION   
  This is an internal function used to enable the registered interrupt.

  PARAMETERS    
  GPIOIntClientCtxt * pCtxt : 
  uint32 gpio :              

  DEPENDENCIES  None.

  RETURN VALUE  maoint_processor_type.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_EnableInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
);

/*=============================================================================
  
  FUNCTION      GPIOIntr_DisableGPIOInterrupt

  DESCRIPTION   This is an internal function that is used to disable the 
                SUMMARY and DIRECT CONNECT interrupts.
                This to be used by QDI layer only.

  PARAMETERS    GPIOIntClientCtxt * :
                the client context pointer of the interrupt controller DAL.
                uint32 gpio number to which the interrupt is assigned.

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_DisableGPIOInterrupt
( 
  GPIOIntr_ClientHandle * pCtxt,
  uint32 gpio
);

/*=============================================================================

  FUNCTION      GPIOIntr_EnableGPIOInterrupt

  DESCRIPTION   
  This is an internal function used to enable the registered interrupt.
  This to be used by QDI layer only.

  PARAMETERS    
  GPIOIntClientCtxt * pCtxt : 
  uint32 gpio :              

  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult GPIOIntr_EnableGPIOInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
);

/*=============================================================================

  FUNCTION      GPIOIntr_TriggerInterrupt

  DESCRIPTION   
  This function is used to trigger an interrupt without the GPIO interrupt 
  being actually set in hardware.

  PARAMETERS    
  GPIOIntClientCtxt * pCtxt : Client Context of the GPIOInt Driver.
  uint32 gpio               : GPIO pin whose interrupt needs to be triggered.
  DEPENDENCIES  None.

  RETURN VALUE  DALResult.

  SIDE EFFECTS  None.

=============================================================================*/

DALResult 
GPIOIntr_TriggerInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio
);

/*=============================================================================

  FUNCTION      GPIOInt_Init
 
=============================================================================*/
 /**
  is used to complete GPIOInt dependencies on other modules in the build.
   
  This function is used for GPIO Interrupt driver initialization. 
  This has to be called first at bootup so that it completes its 
  set of dependencies for NPA and DALSYS. 
  The legacy tramp_gpio name for this API is tramp_gpio_init.

  @param 
  None
  @return
  None
  @dependencies
  None.
*/
void GPIOInt_Init(void);

/*=============================================================================

  FUNCTION      GPIOIntr_SetDirectConnectGPIOMapping
 
=============================================================================*/
 /**
  This API is used to establish direct connect GPIO mapping b/w gpio and direct connect
  interrupt number

  @param 
  None
  @return
  None
  @dependencies
  None.
*/
DALResult GPIOIntr_SetDirectConnectGPIOMapping
(
  GPIOIntr_ClientHandle * pCtxt,
  uint32 nGPIO,
  uint32 direct_connect_line
);

/*=============================================================================

        External APIs specific to DalGPIOInt interface for MDM 9k

=============================================================================

  FUNCTION      GPIOIntr_AttachRemote

  DESCRIPTION   
  This is mdm9k specific function that is used by GPIOINt to attach to its 
  remote component on master and peripheral processors. This attach is 
  primarily used for inter processor communication for relaying interrupts
  fired on the master processor

  PARAMETERS
  GPIOIntClientCtxt*  pCtxt : Client context of the GPIOInt Driver
  uint32 processor :          The remote processor id used to specify which 
                              GPIOINT device to communicate to. 

  DEPENDENCIES  None.

  RETURN VALUE  None.

  SIDE EFFECTS  None.


=============================================================================*/

DALResult GPIOIntr_AttachRemote
(
  GPIOIntr_ClientHandle*  pCtxt,
  uint32 processor
);



#endif /*___GPIOIntr_H*/
