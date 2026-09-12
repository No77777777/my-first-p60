/*==============================================================================

FILE:      uGPIOIntUser.c

DESCRIPTION
  This modules implements the API to utilize the Micro GPIO interrupt controller.
  This Micro GPIO Interrupt Controller module is strictly used in USER Protection
  Domain and in Micro image.The definitions in this module are implemented internally
  and should not be exposed for external client usage directly without using the
  accompanying uGPIOInt.h in the core/api/systemdrivers interface file for this module.

REFERENCES

       Copyright (c) 2014-2015 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/user/uimage/uGPIOIntUser.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
1/1/2010   aratin  First draft created. 
===========================================================================*/

// #include "uTimetick.h" so far now not sure about the logging plan.

#include <atomic_ops.h>
#include <qurt.h>
#include "qurt_int.h"
#include "qurt_event.h"
#include "qurt_anysignal.h"
#include "stringl.h"
#include "uGPIOIntUser.h"
#include "DDIGPIOMgr.h"
#include "uGPIOIntQDI.h"
#include "HALtlmmtile.h"
#include "uGPIOIntKernel.h"

/*------------------------------------------------------------------------------
 GPIOInt Data Declarations.
------------------------------------------------------------------------------*/ 

/*
 * This is static uGPIOInt state data. It can be accessed for debugging 
 * uGPIO Interrupts to see what is the current registration state of the GPIO.
 */
extern uGPIOIntCntrlType uGPIOIntData;

/*------------------------------------------------------------------------------
 GPIOInt Macro Declarations.
------------------------------------------------------------------------------*/ 
#define UGPIOINT_PRIORITY 4

#define VALID_UGPIO(x) ((x) < MAX_NUMBER_OF_GPIOS)
#define VALID_UGPIO_IDX(x) (uGPIOIntData.aGPIOLUT[x] != UGPIOINT_NONE)
#define UGPIOINT_IDX(x) (uGPIOIntData.aGPIOLUT[x])

/* 
 * For each interrupt handler thread a Default IST stack size is declared.
 */
#define GPIOINT_IST_STACK_SIZE  2048

/*------------------------------------------------------------------------------
Following functions are internal functions.
------------------------------------------------------------------------------*/

extern void uGPIOInt_ISTMain(void * ISTParam);
static int32 uGPIOInt_ConfigureIST(void);


/*==========================================================================

  FUNCTION      uGPIOInt_SetupFlags

==========================================================================*/

void
uGPIOInt_SetupFlags(uint32 nGPIO, uint32 nFlags, uGPIOIntTriggerType trigger)
{
  if ( nFlags & UGPIOINTF_DSR )
  {
    uGPIOIntData.state[UGPIOINT_IDX(nGPIO)].gpio_intr_flags |= UGPIOINTF_DSR;
  }
}

/*==========================================================================

  FUNCTION      uGPIOInt_ClearFlags

==========================================================================*/

void
uGPIOInt_ClearFlags(uint32 nIndex )
{
  if ( uGPIOIntData.state[nIndex].gpio_intr_flags & UGPIOINTF_DSR )
  {
    uGPIOIntData.state[nIndex].gpio_intr_flags &= ~UGPIOINTF_DSR;
  }
}

/*==========================================================================

  FUNCTION      uGPIOInt_AssignInterruptIndex

  DESCRIPTION   See uGPIOIntUser.h

==========================================================================*/

int32
uGPIOInt_AssignInterruptIndex(uint32 nGPIO)
{
  uint32 index;

  if (uGPIOIntData.aGPIOLUT[nGPIO] == UGPIOINT_NONE) 
  {
    for(index =0;index < MAX_NUMBER_OF_UGPIOS; index++)
    {
      if (uGPIOIntData.state[index].nGPIO == UGPIOINT_NONE) 
      {
        uGPIOIntData.state[index].nGPIO = (uint8)nGPIO;
        uGPIOIntData.aGPIOLUT[nGPIO] = (uint8)index;
        break;
      }   
    }
    if (index >= MAX_NUMBER_OF_UGPIOS) 
    {
      return UGPIOINT_ERROR;
    }
    else
    {
      return UGPIOINT_SUCCESS;
    }
  }
  else
  {
    return UGPIOINT_SUCCESS;
  }
 
}

/*==========================================================================

   FUNCTION      uGPIOInt_SetQURTTriggerConfig

  DESCRIPTION   This function sets the trigger for the interrupt vector
                using the QURT OS apis.

  PARAMETERS    
  uint32 nInterrupt  : The interrupt vector number.
  uint32 nTrigger : The interrupt trigger type.

  DEPENDENCIES  None.

  RETURN VALUE  None.

  SIDE EFFECTS  None.

==========================================================================*/
static int32 uGPIOInt_SetQURTTriggerConfig
(
  
  uint32 nInterruptVector,
  uGPIOIntTriggerType eTrigger,
  uint32 gpio
)
{
  uint32 nDetectType,nPolarity;

  switch (eTrigger)
  {
    case UGPIOINT_TRIGGER_FALLING:
      nDetectType = 1;
      nPolarity = 1;
      break;
    case UGPIOINT_TRIGGER_RISING:
    case UGPIOINT_TRIGGER_DUAL_EDGE:
      nDetectType = 1;
      nPolarity = 0;
      break;
    case UGPIOINT_TRIGGER_HIGH:
      nDetectType = 0;
      nPolarity = 0;
      break;
    case UGPIOINT_TRIGGER_LOW:
      nDetectType = 0;
      nPolarity = 1;
      break;
    default:
      return UGPIOINT_ERROR;    
  }

  if (QURT_EOK != qurt_interrupt_set_config(nInterruptVector, nDetectType, nPolarity))
  {
    /* Throw an error. */
    return UGPIOINT_ERROR;
  } 

  return UGPIOINT_SUCCESS;

} /* SetQURTTriggerConfig */


/* ===========================================================================
**  UGPIOInt_SetDirConnInterrupt
**
** ======================================================================== */
void uGPIOInt_SetDirConnIntr
(
  uint32                   nGPIO,
  uint32                   nDirConnIntrIdx,
  uint32                   nDirectConnectBaseType
)
{
  
  HAL_tlmmtile_SetDirConn(nDirConnIntrIdx, HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_SEL, nGPIO, nDirectConnectBaseType );
  
  HAL_tlmmtile_Set( nGPIO, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_DIRCONNEN, 1 );
}

/* ===========================================================================
**  UGPIOInt_DisableDirConnIntr
**
** ======================================================================== */
void uGPIOInt_DisableDirConnIntr
(
  uint32    nGPIO,  
  uint32    nDirConnIntrIdx,
  uint32    nDirectConnectBaseType
)
{
  HAL_tlmmtile_SetDirConn(nDirConnIntrIdx, HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_SEL, 0, nDirectConnectBaseType );
  
  HAL_tlmmtile_Set( nGPIO, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_DIRCONNEN, 0 );
  
  HAL_tlmmtile_Set( nGPIO, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_EN, 0 );
}

/* ===========================================================================
**  UGPIOInt_SetDirConnIntrPolarity
**
** ======================================================================== */
static void uGPIOInt_SetDirConnIntrPolarity
(
  uGPIOIntTriggerType   eTrigger,
  uint32                nGPIO,
  uint32                nDirConnIntrIdx,
  uint32                nDirectConnectBaseType
)
{
  uint32 nVal;
  
  if (eTrigger == UGPIOINT_TRIGGER_HIGH ||
      eTrigger == UGPIOINT_TRIGGER_RISING )
  {
    nVal = 1;
  }
  else
  {
    nVal = 0;
  }
  
  HAL_tlmmtile_SetDirConn(nDirConnIntrIdx, HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_POL, nVal, nDirectConnectBaseType );
 
} /* END UGPIOInt_SetDirConnPolarity */

/*==========================================================================

  FUNCTION      uGPIOInt_ConfigureDirectConnectInterrupt

  DESCRIPTION   This function configures the given GPIO pin to the
                direct connect line on the main interrupt controller

==========================================================================*/
int32 
uGPIOInt_ConfigureDirectConnectInterrupt
(
  uint32 gpio,
  uGPIOIntTriggerType trigger,
  uint32 nDirConnID,
  uint32 nDirectConnectBaseType
) 
{
  uGPIOInt_SetDirConnIntr(gpio, nDirConnID, nDirectConnectBaseType);
  uGPIOInt_SetDirConnIntrPolarity(trigger , gpio, nDirConnID, nDirectConnectBaseType);

  return uGPIOInt_SetQURTTriggerConfig((uint32)uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id,trigger,gpio);

} /* END uGPIOInt_ConfigureDirectConnectInterrupt */

/*==========================================================================

  FUNCTION      uGPIOInt_UnConfigureDirectConnectInterrupt

  DESCRIPTION   This function configures the given GPIO pin to the
                direct connect line on the main interrupt controller

==========================================================================*/
int32 
uGPIOInt_UnConfigureDirectConnectInterrupt
(
  uint32 gpio
) 
{
  uint32 nDirConnID;
  GPIOMgrDirectConnectBaseType nDirectConnectBaseType;
  
  nDirConnID = uGPIOIntData.state[UGPIOINT_IDX(gpio)].nDirConnIntrIdx;
  nDirectConnectBaseType = uGPIOIntData.state[UGPIOINT_IDX(gpio)].nDirectConnectBaseType;
  
  uGPIOInt_DisableDirConnIntr(gpio, nDirConnID, (uint32)nDirectConnectBaseType); 
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].nGPIO = UGPIOINT_NONE; 
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id = UGPIOINT_INT_NONE;
  uGPIOIntData.aGPIOLUT[gpio] = UGPIOINT_NONE; 
  
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].gpio_intr_flags &= ~UGPIOINTF_REGISTERED;
  
  return UGPIOINT_SUCCESS;

} /* END uGPIOInt_UnConfigureDirectConnectInterrupt */


/*==========================================================================

   FUNCTION      uGPIOInt_Init

   DESCRIPTION   See uGPIOIntUser.h

==========================================================================*/
int32 uGPIOInt_Init(void)
{
  uint32 index;
  int32 nResult;
  
  if(uGPIOIntData.uGPIOInt_Init != 1)
  {
    /* 
     * We get the mapping of all supported direct connect interrupts from DALGPIOInt.                                                                            .
     * This is done at power up.                                                                                             .
     */
    qurt_mutex_init(&uGPIOIntData.uGPIOIntLock);
    
    /*
     * Initialize the uGPIO LUT table. this the full size of 200 GPIOs
     */
    for(index =0;index < MAX_NUMBER_OF_GPIOS; index++)
    {
      uGPIOIntData.aGPIOLUT[index] = UGPIOINT_NONE;  
    }

    /*
     * Initialize the uGPIO state table.
     * The interrupt configuration is fixed for the 10 configurations possible in hw for 10 direct connects.
     */
    for(index =0;index < MAX_NUMBER_OF_UGPIOS; index++)
    {
      uGPIOIntData.state[index].qurt_intr_id = UGPIOINT_INT_NONE;
      uGPIOIntData.state[index].gpio_intr_flags = 0;
      uGPIOIntData.state[index].nGPIO = UGPIOINT_NONE;  
      uGPIOIntData.state[index].nInterruptMask = (1<< index);
      uGPIOIntData.nGPIOIntMask |= uGPIOIntData.state[index].nInterruptMask;
    }
        
    uGPIOIntData.nThreadID = 0;
    uGPIOIntData.nGPIOIntRegistrationMask = 0;

    /*
     * Spawn the IST here in init so it can wait for registration commands when the client registers.
     */
    nResult =  uGPIOInt_ConfigureIST();

    /* 
     * Acquire GPIO Manager
     */
    if( DalGPIOMgr_Attach( &uGPIOIntData.hGPIOMgr ) != DAL_SUCCESS )
    {
      return UGPIOINT_ERROR;
    }

    if (nResult == UGPIOINT_SUCCESS) 
    {
      uGPIOIntData.uGPIOInt_Init = 1;
    }
	
    return nResult;
  }

  return UGPIOINT_SUCCESS;

} /* END uGPIOInt_Init */


/*===========================================================================

  FUNCTION      uGPIOInt_ConfigureIST

  DESCRIPTION   This function spawns a single IST thread for handling
                each GPIO interrupt.

  PARAMETERS
  Parameters : None. 

  DEPENDENCIES  None.

  RETURN VALUE  DALResult
  UGPIOINT_SUCCESS : If the IST was spawned successfully.
  UGPIOINT_ERROR : If the IST Spawning encountered an error.

  SIDE EFFECTS  None.


==========================================================================*/
static int32 uGPIOInt_ConfigureIST(void)
{
  int nResult;
  unsigned int StackSize;
  unsigned char * StackAddr;
 
  nResult = 0;

  /*
   * Set the IST name to UGPIOIST
   */
  snprintf(uGPIOIntData.aISTName, 
          (sizeof(unsigned char)*UGPIOIST_TASKNAME_SIZE), "UGPIOIST");

  /*
   *  Initialize any thread attribute objects needed by the interrupt service thread.
   */
  qurt_thread_attr_init (&uGPIOIntData.ThreadAttr);

  /*
   *  Initialize any thread attribute and signal objects needed by
   *  the interrupt service thread.
   */
  qurt_anysignal_init(&uGPIOIntData.ISTSignal);

  /*
   * 64 bit alignment of the stack ptr.
   */
  StackAddr = (unsigned char *)
              (((unsigned long)uGPIOIntData.aISTStack -1) & (~0x7)) + 0x8;

  StackSize = (unsigned int)( UGPIOINT_IST_STACK_SIZE - 
              ((unsigned long)StackAddr - 
              (unsigned long)uGPIOIntData.aISTStack) );

  /* 
   * To ensure that the entire stack (start to end) is 64-bit aligned
   * so the last odd bytes are dropped off.
   */
  StackSize &= ~(0x7);

  
  qurt_thread_attr_set_stack_size(&uGPIOIntData.ThreadAttr, 
                                  StackSize);
  qurt_thread_attr_set_stack_addr (&uGPIOIntData.ThreadAttr, 
                                   (void *)StackAddr);
  qurt_thread_attr_set_priority (&uGPIOIntData.ThreadAttr, 
                                (unsigned short)UGPIOINT_PRIORITY);

  qurt_thread_attr_set_tcb_partition(&uGPIOIntData.ThreadAttr,1); // This task should reside in TCM Memory for UGPIOInt Delivery.

  qurt_thread_attr_set_name(&uGPIOIntData.ThreadAttr, 
                            uGPIOIntData.aISTName);
   
  nResult = qurt_thread_create((qurt_thread_t *)&uGPIOIntData.nThreadID, 
                               &uGPIOIntData.ThreadAttr,
                               uGPIOInt_ISTMain,NULL);
   
  if (nResult == QURT_EFATAL) 
  {
    return UGPIOINT_ERROR;
  }

  return UGPIOINT_SUCCESS;

} /* END uGPIOInt_ConfigureIST */



/*==========================================================================

   FUNCTION      uGPIOInt_RegisterInterrupt

   DESCRIPTION   See uGPIOInt.h

==========================================================================*/
int32
uGPIOInt_RegisterInterrupt( uint32 gpio,uGPIOIntTriggerType trigger,
                            uGPIOINTISR isr,uGPIOINTISRCtx param, uint32 nFlags)
{
uint32 nSignalMask = 0, sigs = 0 , nDirConnID, nDirectConnectBaseType, nInterruptID;
  qurt_anysignal_t sAckSig;
  int nQDIResult;
  
  if (!VALID_UGPIO(gpio))
  {
    return UGPIOINT_ERROR;
  }
  if (trigger > UGPIOINT_TRIGGER_DUAL_EDGE)
  {
    return UGPIOINT_ERROR;
  }

  qurt_mutex_lock(&uGPIOIntData.uGPIOIntLock);

  if ( DalGPIOMgr_LockGPIO( uGPIOIntData.hGPIOMgr, gpio, qurt_process_get_id() ) != DAL_SUCCESS )
  {
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }

  if ( UGPIOINT_SUCCESS != uGPIOInt_AssignInterruptIndex(gpio) )
  {
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }
  
  if ( DalGPIOMgr_GetDirectConnectGPIO( uGPIOIntData.hGPIOMgr, gpio, &nDirConnID, &nDirectConnectBaseType ) != DAL_SUCCESS )
  {
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }
  
  if ( DalGPIOMgr_GetDirectConnectInterruptID( uGPIOIntData.hGPIOMgr, gpio, &nInterruptID ) != DAL_SUCCESS )
  {
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }
  
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id = nInterruptID;
  
  if ( UGPIOINT_SUCCESS != uGPIOInt_ConfigureDirectConnectInterrupt(gpio, trigger, nDirConnID, nDirectConnectBaseType) )
  {
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }
  
  /*
   * Update Direct connect index and base type 
   */
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].nDirConnIntrIdx = (uint8)nDirConnID;
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].nDirectConnectBaseType = (GPIOMgrDirectConnectBaseType)nDirectConnectBaseType;
  
  /*
   * Update static GPIOInt map with Trigger 
   */ 
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].intr_trigger = (uint8)trigger;

  /* Immediately correct the edge detect for dual edge if applicable */
  //uGPIOInt_ReconfigureDualEdgeDirConnInterrupt(gpio);

  /* Configure flags based on what's supported */
  uGPIOInt_SetupFlags(gpio, nFlags, trigger);
  
  if((uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr != NULL) &&
     (uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr != isr))
  {
    /*
     * Restore interrupts and return an error.
     */
    DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
    return UGPIOINT_ERROR;
  }

  uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr = isr;
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr_param = param; 

  /*
   * Update static GPIOInt map with flag 
   */ 
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].gpio_intr_flags |= UGPIOINTF_REGISTERED;

  /*
   * Set registration mask
   */
  uGPIOIntData.nGPIOIntRegistrationMask |= uGPIOIntData.state[UGPIOINT_IDX(gpio)].nInterruptMask;
  nSignalMask = uGPIOIntData.state[UGPIOINT_IDX(gpio)].nInterruptMask;

  /*
   * Create Ack sig to get feedback, and assign it to the gpio entry
   */
  qurt_anysignal_init(&sAckSig);
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].pAckSig = &sAckSig;

  /* 
   * An IST thread is already created.
   * It will only be restarted on a re registration.
   */
  qurt_anysignal_set(&uGPIOIntData.ISTSignal, nSignalMask);
  
  nQDIResult = uGPIOIntKernel_SetGPIOIntConfig((uint32)gpio,(uint32)trigger,
                                        (uint32)uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id);
  
  qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
  
  if (nQDIResult != UGPIOINT_QDI_SUCCESS)
  {
    return UGPIOINT_ERROR;
  }
  
  /*
   * Wait for feedback from the IST, no need to clear the signal from the structure 
   */
  while (nSignalMask != 0)
  {
    sigs = qurt_anysignal_wait(&sAckSig, nSignalMask);
    qurt_anysignal_clear(&sAckSig, sigs);
    nSignalMask &= ~sigs;
  }

  qurt_anysignal_destroy(&sAckSig);

  return UGPIOINT_SUCCESS;

} /* END uGPIOInt_RegisterInterrupt */


/*==========================================================================

   FUNCTION      uGPIOInt_DeregisterInterrupt

   DESCRIPTION   See uGPIOInt.h

==========================================================================*/
int32
uGPIOInt_DeregisterInterrupt(uint32  gpio)
{
  
  int nStatus, nQDIResult = UGPIOINT_SUCCESS;

  /*
   * if the GPIO is not configured by UGPIO module then we will
   * not have any valid index etc. Hence its better to abandon
   * any invalid accesses from  this point.
   */
  if ((!VALID_UGPIO(gpio))||(!VALID_UGPIO_IDX(gpio)))
  {
    return UGPIOINT_ERROR;
  }

  qurt_mutex_lock(&uGPIOIntData.uGPIOIntLock);

  DalGPIOMgr_ReleaseGPIO( uGPIOIntData.hGPIOMgr, gpio );
  
  /* 
   * Deregister IST from QURT so it can stop waiting for an interrupt. 
   * Do this before clearing out the structure to assure that we don't 
   * access bad data. 
   */
  nStatus = qurt_interrupt_deregister(uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id); 

  uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr = NULL;
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].isr_param = 0;
  uGPIOIntData.state[UGPIOINT_IDX(gpio)].intr_trigger = 0;

  uGPIOInt_ClearFlags(UGPIOINT_IDX(gpio));

  /*
   * This will release direct connect gpio
   */
  uGPIOInt_UnConfigureDirectConnectInterrupt(gpio);

  if (QURT_EOK != nStatus) 
  { 
    qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock);
    return UGPIOINT_ERROR;
  }
  
  nQDIResult = uGPIOIntKernel_ClearGPIOIntConfig((uint32)gpio);
  
  /*
   * Restore interrupts
   */
  qurt_mutex_unlock(&uGPIOIntData.uGPIOIntLock); 
  
  if (nQDIResult != UGPIOINT_QDI_SUCCESS) 
  {
    return UGPIOINT_ERROR;
  }
  
  return UGPIOINT_SUCCESS;

} /* END uGPIOInt_DeregisterInterrupt */


/*=============================================================================
  FUNCTION      uGPIOInt_EnableInterruptEx
=============================================================================*/
int32 
uGPIOInt_EnableInterruptEx
(
  
  uint32  gpio,
  boolean bEnable
)
{
  int32 nRetVal;
  if ((!VALID_UGPIO(gpio))||(!VALID_UGPIO_IDX(gpio)))
  {
    return UGPIOINT_ERROR;
  }

  if ( bEnable == TRUE )
  {
    nRetVal = uGPIOInt_EnableInterrupt(gpio);
    if( nRetVal == UGPIOINT_SUCCESS)
    {
       nRetVal = uGPIOIntKernel_SetGPIOIntConfig((uint32)gpio,(uint32)uGPIOIntData.state[UGPIOINT_IDX(gpio)].intr_trigger,
                                        (uint32)uGPIOIntData.state[UGPIOINT_IDX(gpio)].qurt_intr_id);
  
    }
  }
  else
  {
    nRetVal = uGPIOInt_DisableInterrupt(gpio);
    if( nRetVal == UGPIOINT_SUCCESS)
    {
       nRetVal = uGPIOIntKernel_ClearGPIOIntConfig((uint32)gpio);
    }
  }

  return nRetVal;
}

/*==========================================================================

   FUNCTION      uGPIOInt_SetDirectConnectGPIOMapping

   DESCRIPTION   See uGPIOInt.h

==========================================================================*/

int32 uGPIOInt_SetDirectConnectGPIOMapping
(
  uint32 gpio,
  uint32 qurt_intr_id
)
{
  return UGPIOINT_SUCCESS;
}

/*==========================================================================

   FUNCTION      uGPIOInt_ClearDirectConnectGPIOMapping

   DESCRIPTION   See uGPIOInt.h

==========================================================================*/

int32 uGPIOInt_ClearDirectConnectGPIOMapping
(
  uint32 gpio
)
{
  return UGPIOINT_SUCCESS;
}


int32 uGPIOInt_MapMPMInterrupt
(
  uint32 gpio,
  uint32 mpm_id
)
{
  int32 nQDIResult = UGPIOINT_QDI_SUCCESS;
  
  nQDIResult = uGPIOIntKernel_MapMPMInterrupt(gpio, mpm_id);
  
  return nQDIResult;
}