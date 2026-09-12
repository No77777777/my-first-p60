/*==============================================================================

FILE:      GPIOInt.c

DESCRIPTION
  This modules implements the API to utilize the Dal GPIO interrupt controller. 
  The definitions in this module are implemented internally and should not be 
  exposed for external client usage directly without using the accompanying DDI
  interface file for this module.

REFERENCES

       Copyright © 2010-2017 Qualcomm Technologies Incorporated.
               All Rights Reserved.
              QUALCOMM Proprietary/GTDR
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt.c#1 $

when        who     what, where, why
--------    ---     ---------------------------------------------------------- 
5/18/2017   hvm     GPIOInt driver refactored.
1/1/2010    aratin  First draft created. 
===========================================================================*/

#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DALGlbCtxt.h"
#include "qurt.h"

#include "DDITimetick.h"
#include "DDIGPIOMgr.h"
#include "DDITlmm.h"
#include "DDIInterruptController.h"
#include "DDIHWIO.h"

#ifdef GPIOINT_USE_MPM
#include "DDImpm.h"
#endif /*GPIOINT_USE_MPM */

#include "busywait.h"

#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "uGPIOInt.h"
#include "uGPIOIntInternal.h"

//Unit Test
#ifdef TEST_GPIOINT
   GPIOIntr_DeviceHandle* g_test_device_handle;
#endif

/*------------------------------------------------------------------------------
 GPIOInt Data Declarations.
------------------------------------------------------------------------------*/ 
/*
 * This is static GPIOInt state data. It can be accessed for debugging 
 * GPIOInterrupts to see what is the current registration state of the GPIO.
 */
static GPIOIntr_CntrlType gGPIOIntData;

/*------------------------------------------------------------------------------
 Forward Declarations for internal functions.
------------------------------------------------------------------------------*/ 

DALResult GPIOIntr_ConfigureGPIOInterrupt  (GPIOIntr_ClientHandle * pCtxt,
                                            uint32 gpio, 
                                            GPIOIntTriggerType eTrigger,
                                            uint32 nFlags);

DALResult GPIOIntr_UnConfigureGPIOInterrupt(GPIOIntr_ClientHandle * pCtxt,
                                            uint32 gpio);

DALResult GPIOIntr_MapGPIOIntTrigger       (uint32  nGPIOTrigger, 
                                            uint32* nIntCTrigger);

DALResult GPIOIntr_GetAttributes           (GPIOIntTriggerType eTrigger,
                                            GpioIntr_Attribute *pAttr, 
                                            uint32 nFlags,
                                            uint32 nGpio);

boolean GPIOIntr_IsSupported               (GPIOIntr_DeviceHandle *hDevice,
							                uint32 nGPIO);

/*------------------------------------------------------------------------------
 Following functions are for DALDriver specific functionality
------------------------------------------------------------------------------*/

DALResult 
GPIOIntr_DriverInit(GPIOIntDrvCtxt *pCtxt)
{
    return DAL_SUCCESS;
}

DALResult 
GPIOIntr_DriverDeInit(GPIOIntDrvCtxt *pCtxt)
{
    return DAL_SUCCESS;
}

/*------------------------------------------------------------------------------
 Following functions are declared in DalDevice Interface. 
------------------------------------------------------------------------------*/

/*==========================================================================

  FUNCTION      GPIOIntr_DeviceInit

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_DeviceInit
(
  GPIOIntr_ClientHandle *pCtxt
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle* hDevice;
  uint32 nDeviceType, nCount;
  GPIOIntFnDescType** apfnSupportedDrivers;
  GPIOIntr_InitDataType GPIOIntr_InitData;

  #ifdef TEST_GPIOINT
    g_test_device_handle = pCtxt->hDevice;
  #endif

  hDevice = pCtxt->hDevice;
  if(hDevice == NULL)
  {
      return DAL_ERROR;
  }

  hDevice->pGPIOIntData = &gGPIOIntData;

  /* Acquire GPIO Manager */
  if( DalGPIOMgr_Attach( &hDevice->hGPIOMgrHandle ) != DAL_SUCCESS ) 
  {
    return DAL_ERROR;
  }

  /* Attach to Dalmpm for MPM calls */
  DAL_StringDeviceAttach("/dev/mpm", &hDevice->hDalMpmHandle);
  
  /*
   * GPIOInt_Init has to be called first before anyone can attach to this 
   * driver. This is to ensure that GPIOInt dependencies of NPA and DALSYS 
   * are met.
   */
  if(gGPIOIntData.GPIOInt_Init != 1)
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_DeviceInit: Client cannot attach before GPIOInt_Init ");
      return DAL_ERROR;
  }
 
  if(!hDevice->bIsInitialized)
  {
      /* Each processor has its own internal synchronization object to access 
       * data that it owns
       */

      qurt_rmutex_init (&hDevice->hLock);

      /*
       * Initialize the GPIOInt HAL
       */
      GPIOINT_LOCK(hDevice);

      if(DAL_SUCCESS != GPIOIntr_GetTargetData(pCtxt))
      {
          GPIOINT_FREE(hDevice);
          return DAL_ERROR;
      }

      /*
       * Setup uLog buffer 
       */ 
      GPIOIntLog_Init(&hDevice->hULog);
      if(hDevice->hULog == NULL)
      {
          GPIOINT_FREE(hDevice);
          return DAL_ERROR;
      }

      /* 
       * Iterate through Init Funcs for all supported GpioInt DeviceType drivers
       */
      apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
      nCount = hDevice->pGPIOIntData->gpioint_priority_list_count;
      GPIOIntr_InitData.pLock = &hDevice->hLock;
      GPIOIntr_InitData.hDalMpmHandle = hDevice->hDalMpmHandle;
      for( nDeviceType = 0; nDeviceType < nCount; nDeviceType++ )
      {
          eResult = (apfnSupportedDrivers[nDeviceType])->GPIOIntFn_Init(&GPIOIntr_InitData);
          if(eResult != DAL_SUCCESS)
          {
              hDevice->bIsInitialized = 0;
              gGPIOIntData.is_initialized = 0;
              GPIOINT_FREE(hDevice);
              return eResult;
          }
      }

      GPIOINT_FREE(hDevice);

      /*
       * flag the driver as initialised. 
       */ 
      hDevice->bIsInitialized = 1;
      gGPIOIntData.is_initialized = 1;
  }

   return DAL_SUCCESS;

} /* END GPIOIntr_DeviceInit */

/*==========================================================================

  FUNCTION      GPIOIntr_DeviceDeInit

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_DeviceDeInit(GPIOIntr_ClientHandle *pCtxt)
{
   DALResult eResult = DAL_ERROR;
   GPIOIntr_DeviceHandle* hDevice;
   uint32 nDeviceType, nCount;
   GPIOIntFnDescType** apfnSupportedDrivers;

   hDevice = pCtxt->hDevice;

   GPIOINT_LOCK(hDevice);

   /* Iterate through the DeInit Funcs for all supported GpioInt DeviceType drivers */
   apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
   nCount = hDevice->pGPIOIntData->gpioint_priority_list_count;
   for( nDeviceType = 0; nDeviceType < nCount; nDeviceType++ )
   {
       eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_DeInit();
       if(eResult != DAL_SUCCESS)
       {
           /* Do Nothing, rather move on with rest of the driver tear-down */
       }
   }

   GPIOINT_FREE(hDevice);

  /*
   * flag the driver as de-initialised. 
   */ 
   hDevice->bIsInitialized = 0;
   gGPIOIntData.is_initialized = 0;

   return DAL_SUCCESS;

} /* END GPIOIntr_DeviceDeInit */

/*==========================================================================

  FUNCTION      GPIOIntr_GetTargetData

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_GetTargetData
(
  GPIOIntr_ClientHandle *pCtxt
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  GPIOInt_TargetDataType TargetData;
  uint32 gpio;

  hDevice = pCtxt->hDevice;

  /* 
   * Structure which holds properties for dal.
   */
  DALSYSPropertyVar system_property_var;  

  /* 
   * declare property handle variable.
   */
  DALSYS_PROPERTY_HANDLE_DECLARE(interrupt_property); 
  
  /* Get the property handle */
  DALSYS_GetDALPropertyHandle(DALDEVICEID_GPIOINT, interrupt_property);

  /* determine Subsystem Processor */
  eResult = DALSYS_GetPropertyValue(interrupt_property, 
                                    "PROCESSOR", 0, 
                                    &system_property_var); 
  if(eResult == DAL_SUCCESS)
  {
      hDevice->pGPIOIntData->processor = 
          (GPIOIntProcessorType)system_property_var.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_GetTargetData: Target Data is corrupt");
      return DAL_ERROR;
  }

  TargetData.eProcessor = hDevice->pGPIOIntData->processor;
  
  eResult = DALSYS_GetPropertyValue(interrupt_property, 
                                    "GPIOINT_PHYSICAL_ADDRESS", 0,
                                    &system_property_var); 
  if(eResult == DAL_SUCCESS)
  {
      hDevice->pGPIOIntData->gpioint_physical_address = 
          (uint32 *)system_property_var.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_GetTargetData: could not get a base address for GPIOInt");
      return DAL_ERROR;
  } 

  TargetData.nGPIOIntBase     = hDevice->pGPIOIntData->gpioint_virtual_address;
  TargetData.nGPIOIntBasePhys = hDevice->pGPIOIntData->gpioint_physical_address;

  eResult = DALSYS_GetPropertyValue(interrupt_property, 
                                    "TOTAL_COUNT_OF_GPIOS", 0, 
                                    &system_property_var); 
  if(eResult == DAL_SUCCESS)
  {
      hDevice->pGPIOIntData->total_gpio_number = 
                                    system_property_var.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_GetTargetData: Total Count of Gpios is corrupt");
      return DAL_ERROR;
  }
    
  /* determine number of supported DeviceType drivers */    
  eResult = DALSYS_GetPropertyValue(interrupt_property, 
                                    "NUMBER_OF_SUPPORTED_DRIVERS", 0, 
                                    &system_property_var); 
  if(eResult == DAL_SUCCESS)
  {
      hDevice->pGPIOIntData->gpioint_priority_list_count = 
                                    system_property_var.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_GetTargetData: Count of priority_list_gpioint_drivers is corrupt");
      return DAL_ERROR;
  }  
    
  /* 
   * Get a handle to func pointer table for supported DeviceType drivers
   */
  eResult = DALSYS_GetPropertyValue(interrupt_property, 
                                    "PRIORITY_LIST_OF_SUPPORTED_DRIVERS", 0, 
                                    &system_property_var); 
  if(eResult == DAL_SUCCESS)
  {
      hDevice->pGPIOIntData->gpioint_priority_list = 
          (GPIOIntFnDescType**)system_property_var.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, hDevice->hULog, 0, 
                     "GPIOIntr_GetTargetData: priority_list_gpioint_drivers is corrupt");
      return DAL_ERROR;
  }

  /* 
   * Check if UGPIOINT_DRIVER is supported
   */
  eResult = DALSYS_GetPropertyValue(interrupt_property,
                                    "UGPIOINT_DRIVER", 0,
                                    &system_property_var);
  if(eResult == DAL_SUCCESS)
  {
     hDevice->pGPIOIntData->gpioint_uimage = 1;
     uGPIOInt_Init();
  }
  else
  {
    hDevice->pGPIOIntData->gpioint_uimage = 0;
  }
 
  /*
   *  Initialize state table for all GPIOs as DEFAULT.
   */ 
  for(gpio = 0; gpio < hDevice->pGPIOIntData->total_gpio_number; gpio++)
  {
      hDevice->pGPIOIntData->state[gpio].nDeviceType  = GPIOINT_NO_DEVICETYPE;
      if(DAL_SUCCESS != DALSYS_Malloc((sizeof(GpioIntr_CfgType)),
                                      (void **)&(hDevice->pGPIOIntData->state[gpio].pIntrCfg)))
      {
          return DAL_ERROR;
      }
  }

  return eResult;

} /* END GPIOIntr_GetTargetData */


DALResult 
GPIOIntr_PowerEvent(GPIOIntr_ClientHandle *pCtxt, DalPowerCmd PowerCmd, 
                    DalPowerDomain PowerDomain)
{
    return DAL_SUCCESS;
}

DALResult 
GPIOIntr_Open(GPIOIntr_ClientHandle *pCtxt, uint32 dwaccessMode )
{
    return DAL_SUCCESS;
}

DALResult 
GPIOIntr_Close(GPIOIntr_ClientHandle *pCtxt)
{
    return DAL_SUCCESS;
}

DALResult 
GPIOIntr_Info(GPIOIntr_ClientHandle *pCtxt,DalDeviceInfo *pdeviceInfo, uint32 dwSize)
{
    return DAL_SUCCESS;
}

/*------------------------------------------------------------------------------
 Following functions are extended in DalGPIOInt Interface. 
------------------------------------------------------------------------------*/
/*==========================================================================

  FUNCTION      GPIOIntr_SetTrigger

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_SetTrigger
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  GPIOIntTriggerType  eTrigger
) 
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
      GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, hDevice->hULog, 1, 
                     "GPIOIntr_SetTrigger: GPIO %d is invalid", gpio);
      return DAL_ERROR;
  }

  /*
   * Validate the given trigger type
   */
  if (eTrigger >= GPIOINT_TRIGGER_INVALID)
  {
     GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, hDevice->hULog, 2,
                    "GPIOIntr_SetTrigger: Invalid trigger type %d passed for GPIO %d", 
                    eTrigger, gpio);
     return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);

  /* 
   * Determine DeviceType for this Gpio Interrupt
   */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, hDevice->hULog, 1, 
                    "GPIOIntr_SetTrigger: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }
  
  hDevice->pGPIOIntData->state[gpio].pIntrCfg->eIntrTrigger = eTrigger;
   
  /* 
   * Set trigger type
   */
  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_SetTrigger(gpio, eTrigger);
  if(eResult != DAL_SUCCESS)
  {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, hDevice->hULog, 1, 
                     "GPIOIntr_SetTrigger: Failed to set trigger for GPIO %d", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }
 
  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_SetTrigger */


/*==========================================================================

  FUNCTION      GPIOIntr_RegisterIsr

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_RegisterIsr
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio, 
  GPIOIntTriggerType eTrigger, 
  GPIOINTISR isr,
  GPIOINTISRCtx param,
  uint32 nFlags
) 
{
  DALResult eResult;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
      GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                     "GPIOIntr_RegisterIsr: GPIO %d is not supported", gpio);
    return DAL_ERROR;
  }
  
  /*
   * Validate the given trigger type
   */
  if (eTrigger >= GPIOINT_TRIGGER_INVALID)
  {
      GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 2,
                     "GPIOIntr_RegisterIsr: Invalid trigger type %d passed for GPIO %d", 
                    eTrigger, gpio);
    return DAL_ERROR;
  }
  
  GPIOINT_LOCK(hDevice);
  if(((hDevice->pGPIOIntData->state[gpio].isr != NULL) &&
      (hDevice->pGPIOIntData->state[gpio].isr != isr)))
  {
     /*
      * Restore interrupts and return an error.
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: GPIO %d is already assigned", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  if(DalGPIOMgr_LockGPIO(hDevice->hGPIOMgrHandle, gpio, qurt_process_get_id()) != DAL_SUCCESS)
  {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: GPIOMgr failed to lock GPIO %d", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  /* 
   * Configure Gpio interrupt
   */
  eResult = GPIOIntr_ConfigureGPIOInterrupt(pCtxt, gpio, eTrigger, nFlags);
  if(eResult != DAL_SUCCESS)
  {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: failed to configure GPIO %d", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  hDevice->pGPIOIntData->state[gpio].isr       = isr;
  hDevice->pGPIOIntData->state[gpio].isr_param = param; 

  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  /* 
   * Register ISR for Gpio interrupt
   */
  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Register(gpio, isr, (void *)param);
  if(eResult != DAL_SUCCESS)
  {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: failed to register ISR for GPIO %d", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  hDevice->pGPIOIntData->state[gpio].gpio_intr_flags |= GPIOINTF_REGISTERED;

  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_RegisterIsr */


/*==========================================================================

  FUNCTION      GPIOIntr_DeregisterIsr

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_DeregisterIsr
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio, 
  GPIOINTISR isr
) 
{
  DALResult eResult;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
      GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                     "GPIOIntr_DeregisterIsr: GPIO %d is invalid", gpio);
      return DAL_ERROR;
  }
  
  /*
   * Disable interrupts by accessing the processor's internal sync variable
   */
  GPIOINT_LOCK(hDevice);

  /*
   * Check that the ISR is correct
   */
  if (hDevice->pGPIOIntData->state[gpio].isr != isr)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_DeregisterIsr: GPIO %d has a different ISR assigned", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
      GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                     "GPIOIntr_DeregisterIsr: DeviceType for GPIO %d is invalid", gpio);
      GPIOINT_FREE(hDevice);
      return DAL_ERROR;
  }

  /* 
   * Deregister ISR for Gpio interrupt
   */
  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Deregister(gpio);
  if(eResult != DAL_SUCCESS)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1,
                    "GPIOIntr_DeregisterISR: failed to deregister ISR for GPIO %d", gpio);
     //GPIOINT_FREE(hDevice);
     //return DAL_ERROR;
  }

  hDevice->pGPIOIntData->state[gpio].gpio_intr_flags &= ~GPIOINTF_REGISTERED;

  /* 
   * Unconfigure Gpio interrupt
   */
  eResult = GPIOIntr_UnConfigureGPIOInterrupt(pCtxt, gpio);
  if(eResult != DAL_SUCCESS)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_DeregisterISR: failed to unconfigure GPIO %d", gpio);
     //GPIOINT_FREE(hDevice);
     //return DAL_ERROR;
  }

  if ( DalGPIOMgr_ReleaseGPIO( hDevice->hGPIOMgrHandle, gpio ) != DAL_SUCCESS )
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1,  
                    "GPIOIntr_DeregisterIsr: GPIOMgr failed to release GPIO %d", gpio);
     //GPIOINT_FREE(hDevice);
     //return DAL_ERROR;
  }

  /*
   * Clear out the handler and remove the event.
   */
  hDevice->pGPIOIntData->state[gpio].isr          = NULL;
  hDevice->pGPIOIntData->state[gpio].isr_param    = 0;
  hDevice->pGPIOIntData->state[gpio].nDeviceType  = GPIOINT_NO_DEVICETYPE;

  /*
   * Restore interrupts
   */
  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_DeregisterIsr */


/*==========================================================================

  FUNCTION      GPIOIntr_RegisterEvent

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_RegisterEvent
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio,
  GPIOIntTriggerType eTrigger, 
  DALSYSEventHandle event
) 
{
   /* NOT SUPPORTED */
   return DAL_ERROR;

} /* GPIOIntr_RegisterEvent */

/*==========================================================================

  FUNCTION      GPIOIntr_DeRegisterEvent

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_DeRegisterEvent
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio,
  DALSYSEventHandle event
) 
{
    /* NOT SUPPORTED */
    return DAL_ERROR;

} /* END GPIOIntr_DeRegisterEvent */


/*==========================================================================

  FUNCTION      GPIOIntr_IsInterruptEnabled

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_IsInterruptEnabled
(
  GPIOIntr_ClientHandle *pCtxt, 
  uint32  gpio, 
  uint32 * state
) 
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptEnabled: GPIO %d is invalid", gpio);
     return DAL_ERROR;
  }

  if ( state == NULL )
  {
    return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptEnabled: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     *state = 0;
     return DAL_SUCCESS;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_IsInterruptEnabled(state, gpio);

  GPIOINT_FREE(hDevice);

  return eResult;

} /* GPIOIntr_IsInterruptEnabled */


/*==========================================================================

  FUNCTION      GPIOIntr_IsInterruptRegistered

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_IsInterruptRegistered
(
  GPIOIntr_ClientHandle *pCtxt, 
  uint32  gpio, 
  uint32 * state
) 
{
  GPIOIntr_DeviceHandle *hDevice;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptRegistered: GPIO %d is invalid", gpio);
     return DAL_ERROR;
  }

  if ( state == NULL )
  {
    return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);

  if(hDevice->pGPIOIntData->state[gpio].gpio_intr_flags & GPIOINTF_REGISTERED)
  {
     *state = 1;
  }
  else
  {
     *state = 0;
  }

  GPIOINT_FREE(hDevice);

  return DAL_SUCCESS;

} /* GPIOIntr_IsInterruptRegistered */


/*==========================================================================

  FUNCTION      GPIOIntr_IsInterruptPending

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_IsInterruptPending
( 
  GPIOIntr_ClientHandle *pCtxt, 
  uint32  gpio, 
  uint32 * state 
) 
{  
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptPending: GPIO %d is invalid", gpio);
     return DAL_ERROR;
  }

  if ( state == NULL )
  {
    return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);

  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptPending: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_IsInterruptPending(state, gpio);

  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_IsInterruptPending */


/*==========================================================================

  FUNCTION      GPIOIntr_IsInterruptSet

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_IsInterruptSet
( 
  GPIOIntr_ClientHandle *pCtxt, 
  uint32  gpio, 
  uint32 * state 
) 
{  
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptSet: GPIO %d is invalid", gpio);
     return DAL_ERROR;
  }
  
  if ( state == NULL )
  {
    return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_IsInterruptSet: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     *state = 0;
     return DAL_SUCCESS;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_IsInterruptSet(state, gpio);

  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_IsInterruptSet */


/*==========================================================================

  FUNCTION      GPIOIntr_Save (No implementation perceived as of now.)

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_Save
( 
  GPIOIntr_ClientHandle * pCtxt
)
{
   return DAL_ERROR;
}

/*==========================================================================

  FUNCTION      GPIOIntr_Restore (No implementation perceived as of now.)

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult GPIOIntr_Restore
( 
  GPIOIntr_ClientHandle * pCtxt
)
{
   return DAL_SUCCESS;
}


/*==========================================================================

  FUNCTION      GPIOIntr_DisableInterrupt

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_DisableInterrupt
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   *  Validate given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_ENABLE, hDevice->hULog, 1, 
                    "GPIOIntr_DisableInterrupt: GPIO %d is not supported", gpio);
     return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
      /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_ENABLE, hDevice->hULog, 1, 
                    "GPIOIntr_DisableInterrupt: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Disable(gpio);

  GPIOINT_FREE(hDevice);

  return DAL_SUCCESS;
  
} /* END GPIOIntr_DisableInterrupt */


/*==========================================================================

  FUNCTION      GPIOIntr_DisableGPIOInterrupt

  DESCRIPTION   See GPIOInt.h

==========================================================================*/
DALResult 
GPIOIntr_DisableGPIOInterrupt
( 
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
   return GPIOIntr_DisableInterrupt(pCtxt, gpio);

} /* END GPIOIntr_DisableGPIOInterrupt */


/*==========================================================================

  FUNCTION      GPIOIntr_EnableInterrupt

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_EnableInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   *  Validate given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_ENABLE, hDevice->hULog, 1, 
                    "GPIOIntr_EnableInterrupt: GPIO %d is not supported", gpio);
     return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);

  /*
   * If this interrupt is not Registered and not configured externally via SetDirectConnectMapping , then return an error.
   */
  if(!(hDevice->pGPIOIntData->state[gpio].gpio_intr_flags & GPIOINTF_REGISTERED)
     && !(hDevice->pGPIOIntData->state[gpio].gpio_intr_flags & GPIOINTF_EXTERNAL_CONFIG)  )
  {
     /*
      * This GPIO is not registered for a handler. We should not enable it.
      */
     GPIOIntLog_Out(GPIOINTLOG_ENABLE, hDevice->hULog, 1, 
                    "GPIOIntr_EnableInterrupt: GPIO %d does not have a client ", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_ENABLE, hDevice->hULog, 1, 
                    "GPIOIntr_EnableInterrupt : DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Enable(gpio);

  GPIOINT_FREE(hDevice);

  return DAL_SUCCESS;

} /* END GPIOIntr_EnableInterrupt */


/*==========================================================================

  FUNCTION      GPIOIntr_EnableGPIOInterrupt

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_EnableGPIOInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
   return GPIOIntr_EnableInterrupt(pCtxt, gpio);

} /* END GPIOIntr_EnableGPIOInterrupt */


/*==========================================================================

   FUNCTION      GPIOIntr_TriggerInterrupt

   DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_TriggerInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32  gpio
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   * Validate the given gpio interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_TRIGGER, hDevice->hULog, 1, 
                    "GPIOIntr_TriggerInterrupt: GPIO %d is not supported", gpio);
     return DAL_ERROR;
  }

  /*
   * Disable interrupts
   */
  GPIOINT_LOCK(hDevice);

  if( hDevice->pGPIOIntData->state[gpio].isr == NULL )
  {
     GPIOIntLog_Out(GPIOINTLOG_TRIGGER, hDevice->hULog, 1, 
                    "GPIOIntr_TriggerInterrupt: GPIO %d does not have a client in DAL GPIOInt", gpio);
     if(hDevice->pGPIOIntData->gpioint_uimage != 0)
     {
       if(UGPIOINT_SUCCESS == uGPIOInt_TriggerInterrupt(gpio))
       {
         GPIOINT_FREE(hDevice);
         return DAL_SUCCESS;
       }
       else
       {
         GPIOIntLog_Out(GPIOINTLOG_TRIGGER, hDevice->hULog, 1, 
                        "GPIOIntr_TriggerInterrupt: GPIO %d does not have a client in uGPIOInt", gpio);
       }
     }
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_TRIGGER, hDevice->hULog, 1, 
                    "GPIOIntr_TriggerInterrupt: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_TriggerInterrupt(gpio);

  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_TriggerInterrupt */


/*==========================================================================

   FUNCTION      GPIOIntr_ClearInterrupt

   DESCRIPTION   See GPIOInt.h

==========================================================================*/

DALResult 
GPIOIntr_ClearInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /*
   *  Validate the given GPIO interrupt
   */
  if (!GPIOIntr_IsSupported(hDevice, gpio))
  {
     GPIOIntLog_Out(GPIOINTLOG_CLEAR, hDevice->hULog, 1, 
                    "GPIOIntr_ClearInterrupt: GPIO %d is not supported", gpio);
     return DAL_ERROR;
  }

  GPIOINT_LOCK(hDevice);
  
  /* Determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_CLEAR, hDevice->hULog, 1, 
                    "GPIOIntr_ClearInterrupt: DeviceType for GPIO %d is invalid", gpio);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
  }

  eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_ClearInterrupt(gpio);

  GPIOINT_FREE(hDevice);

  return eResult;

} /* END GPIOIntr_ClearInterrupt */


/*==========================================================================

   FUNCTION      GPIOInt_Init

   DESCRIPTION   See DDIGPIOInt.h

==========================================================================*/

#ifdef TEST_GPIOINT
  typedef struct {
      uint32 gpio;
      uint32 trig_type;
  }IsrParamType;
  IsrParamType gIsrParam;

  #define GPIO_COUNT 2
  #define GPIO_TRIGGER_COUNT 3

  static volatile uint32 cpnWaitTimeUs = 5000;
  static uint32 cpnTestGpio[GPIO_COUNT] = {123, 4};
  static GPIOIntTriggerType cpeTrigger[GPIO_COUNT][GPIO_TRIGGER_COUNT] = 
                                              {{GPIOINT_TRIGGER_RISING, GPIOINT_TRIGGER_FALLING, GPIOINT_TRIGGER_DUAL_EDGE},
                                               {GPIOINT_TRIGGER_RISING, GPIOINT_TRIGGER_FALLING, GPIOINT_TRIGGER_DUAL_EDGE}};

  static void GPIOInt_TestIsr(void* pData)
  {
     static uint32 count = 0;
     IsrParamType *pParam = (IsrParamType *)pData;

     count++;
     GPIOIntLog_Out(GPIOINTLOG_SUMMARYISR, g_test_device_handle->hULog, 3, 
                    "** GPIOInt_TestIsr: TestIsr Called for Overall Count %d, Gpio:%d, TriggerType:%d **",
                    count, pParam->gpio, pParam->trig_type);
  }
#endif /*TEST_GPIOINT*/


void GPIOInt_Init(void)
{
  DalDeviceHandle *  gpioint_handle;

  gGPIOIntData.GPIOInt_Init = TRUE;
  /* 
   * attach to the gpioint dal.
   */
  DAL_DeviceAttach(DALDEVICEID_GPIOINT, &gpioint_handle);

  //Unit Test
  #ifdef TEST_GPIOINT
     uint32 gpio_iter, gpio_trig_iter;
     for(gpio_iter = 0; gpio_iter < GPIO_COUNT; gpio_iter++)
     {
        for(gpio_trig_iter = 0; gpio_trig_iter < GPIO_TRIGGER_COUNT; gpio_trig_iter++)
        {
           gIsrParam.gpio = cpnTestGpio[gpio_iter];
           gIsrParam.trig_type = gpio_trig_iter;

           GPIOInt_RegisterIsr(gpioint_handle, cpnTestGpio[gpio_iter], cpeTrigger[gpio_iter][gpio_trig_iter], (GPIOINTISR)GPIOInt_TestIsr, (GPIOINTISRCtx)&gIsrParam);
             
           busywait(cpnWaitTimeUs); 

           GPIOInt_TriggerInterrupt(gpioint_handle, cpnTestGpio[gpio_iter]);

           busywait(cpnWaitTimeUs);

           GPIOInt_DeregisterIsr(gpioint_handle, cpnTestGpio[gpio_iter], (GPIOINTISR)GPIOInt_TestIsr);
        }
     }
  #endif /*TEST_GPIOINT*/

} /* END GPIOInt_Init */

/*=========================================================================
            Internal GPIOInt Functions
==========================================================================*/

/*========================================================================== 
 
  FUNCTION      GPIOIntr_ConfigureGPIOInterrupt
 
==========================================================================*/

DALResult 
GPIOIntr_ConfigureGPIOInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio,
  GPIOIntTriggerType eTrigger,
  uint32 nFlags
)
{
   DALResult eResult = DAL_ERROR;

   GPIOIntr_DeviceHandle * hDevice;
   uint32 nDeviceType, nCount;
   GpioIntr_Attribute aAttr;
   GPIOIntFnDescType** apfnSupportedDrivers;

   hDevice = pCtxt->hDevice;

   /* Populate attribute struct */
   GPIOIntr_GetAttributes(eTrigger, &aAttr, nFlags, gpio);

  /* determine best match for Gpio from the Priority List of GpioInt DeviceTypes */
   apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
   nCount = hDevice->pGPIOIntData->gpioint_priority_list_count;
   for(nDeviceType = 0; nDeviceType < nCount; nDeviceType++)
   {
       if( DAL_SUCCESS == apfnSupportedDrivers[nDeviceType]->GPIOIntFn_IsSupported(gpio, &aAttr) )
       {
           hDevice->pGPIOIntData->state[gpio].pIntrCfg->nInterruptType = GPIOINT_DEFAULT;
           hDevice->pGPIOIntData->state[gpio].pIntrCfg->nInterruptID   = GPIOINT_NO_QURT_ID;
           hDevice->pGPIOIntData->state[gpio].pIntrCfg->eIntrTrigger =  GPIOINT_TRIGGER_INVALID;
           eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Config(gpio, 
                                                                        &aAttr, 
                                                                        hDevice->pGPIOIntData->state[gpio].pIntrCfg);
           if( eResult == DAL_SUCCESS )
           {
              /* capture the GPIOInt DeviceType */
              hDevice->pGPIOIntData->state[gpio].nDeviceType = nDeviceType;
              hDevice->pGPIOIntData->state[gpio].pIntrCfg->eIntrTrigger = eTrigger;
              break;
           }
       }
   }

   if( (eResult != DAL_SUCCESS) && (nDeviceType >= nCount) )
   {
     return eResult;
   }

   return DAL_SUCCESS;

} /* END GPIOIntr_ConfigureGPIOInterrupt */

/*========================================================================== 
 
  FUNCTION      GPIOIntr_UnConfigureGPIOInterrupt
 
==========================================================================*/

DALResult 
GPIOIntr_UnConfigureGPIOInterrupt
(
  GPIOIntr_ClientHandle * pCtxt, 
  uint32 gpio
)
{
  GPIOIntr_DeviceHandle * hDevice;
  uint32 nDeviceType;
  GPIOIntFnDescType** apfnSupportedDrivers;

  hDevice = pCtxt->hDevice;

  /* determine DeviceType for this Gpio interrupt */
  apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
  nDeviceType = hDevice->pGPIOIntData->state[gpio].nDeviceType;
  if (GPIOINT_NO_DEVICETYPE == nDeviceType)
  {
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_UnConfigureGPIOInterrupt: DeviceType for GPIO %d is invalid", gpio);
     return DAL_ERROR;
  }

  if( DAL_SUCCESS != apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Deconfig(gpio, 
                                                                           hDevice->pGPIOIntData->state[gpio].pIntrCfg) )
  {
     return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END GPIOIntr_UnConfigureGPIOInterrupt */

/*==========================================================================

   FUNCTION      GPIOIntr_MapGPIOIntTrigger

==========================================================================*/

DALResult
GPIOIntr_MapGPIOIntTrigger
(
  uint32  nGPIOTrigger,
  uint32* nIntCTrigger
)
{
  if( nIntCTrigger == NULL )
  {
    return DAL_ERROR;
  }

  switch(nGPIOTrigger) 
  {
    case GPIOINT_TRIGGER_HIGH:
      *nIntCTrigger = DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER;
      break;
	  
    case GPIOINT_TRIGGER_LOW:
      *nIntCTrigger = DALINTRCTRL_ENABLE_LEVEL_LOW_TRIGGER;
      break;
	  
    case GPIOINT_TRIGGER_FALLING:
      *nIntCTrigger = DALINTRCTRL_ENABLE_FALLING_EDGE_TRIGGER;
      break;
	  
    case GPIOINT_TRIGGER_DUAL_EDGE:
    case GPIOINT_TRIGGER_RISING:
    default:
      *nIntCTrigger = DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER;
      break;
  }
  
  return DAL_SUCCESS;
}

/*==========================================================================

   FUNCTION      GPIOIntr_MapGPIOIntTriggerToFlag

==========================================================================*/

DALResult
GPIOIntr_GetAttributes
(
  GPIOIntTriggerType eTrigger,
  GpioIntr_Attribute *pAttr,
  uint32 nFlags,
  uint32 nGpio
)
{
  if( pAttr == NULL )
  {
    return DAL_ERROR;
  }

  pAttr->eTrigger = eTrigger;

  if ( GPIOINT_PRIORITY_ISSET(nFlags) )
  {
    pAttr->nPriority = GPIOINT_GET_PRIORITY(nFlags);
  }
  else
  {
    /* Default Priority */
    pAttr->nPriority = GPIOINT_DEFAULT_PRIORITY;
  }

  if ( nFlags & GPIOINT_ISLAND_MODE )
  {
    pAttr->nUInterrupt = 1;
  }
  else
  {
    pAttr->nUInterrupt = 0;
  }

  pAttr->nflags = gGPIOIntData.state[nGpio].gpio_intr_flags;
  
  pAttr->mpm_intr_id = gGPIOIntData.state[nGpio].mpm_intr_id;
  
  return DAL_SUCCESS;
}


/* ===========================================================================
**  
**  FUNCTION      GPIOIntr_IsSupported
**
** ======================================================================== */

boolean GPIOIntr_IsSupported
(
  GPIOIntr_DeviceHandle *hDevice,
  uint32 nGPIO
)
{
  /*
   * Return if this interrupt is supported or not
   */
  return (nGPIO < hDevice->pGPIOIntData->total_gpio_number);

} /* END GPIOIntr_IsSupported */


/*==========================================================================

  FUNCTION      GPIOIntr_ConfigMPMWakeup

  DESCRIPTION   See GPIOInt.h

==========================================================================*/
 #ifdef GPIOINT_USE_MPM
DALResult
GPIOIntr_ConfigMPMWakeup
(
  DalDeviceHandle  *hDevice,
  uint32 gpio,
  int8 bEnable
)
{
  GPIOIntTriggerType  trigger;
  mpm_pin_config_type      sConfig;
  
  sConfig.pin_type = MPM_GPIO; 
  
  trigger = gGPIOIntData.state[gpio].pIntrCfg->eIntrTrigger;
  
  if ((gGPIOIntData.state[gpio].gpio_intr_flags & GPIOINTF_MPMINT) == 0)
  {
    return DAL_ERROR;
  }
  
   /*
   * Determine the DALmpm interrupt configuration
   */
  switch (trigger)
  {

    case GPIOINT_TRIGGER_DUAL_EDGE:
      sConfig.trig_config = MPM_TRIGGER_DUAL_EDGE;
      break;

    case GPIOINT_TRIGGER_HIGH:
      sConfig.trig_config = MPM_TRIGGER_LEVEL_HIGH;
      break;

    case GPIOINT_TRIGGER_LOW:
      sConfig.trig_config = MPM_TRIGGER_LEVEL_LOW;
      break;

    case GPIOINT_TRIGGER_RISING:
      sConfig.trig_config = MPM_TRIGGER_RISING_EDGE;
      break;

    case GPIOINT_TRIGGER_FALLING:
      sConfig.trig_config = MPM_TRIGGER_FALLING_EDGE;
      break;

    default:
      sConfig.trig_config = MPM_TRIGGER_LEVEL_HIGH;
      break;
  }

  /*
   * Inform DALmpm that the GPIO interrupt is enabled
   */
  if( bEnable )
  {
    Dalmpm_ConfigAndEnablePin( hDevice, 
                               gGPIOIntData.state[gpio].mpm_intr_id, 
                               &sConfig ); 
  }
  else
  {
    Dalmpm_ConfigPin( hDevice, 
                      gGPIOIntData.state[gpio].mpm_intr_id, 
                      &sConfig ); 
  }
  
  return DAL_SUCCESS;
  
} /* END GPIOIntr_ConfigMPMWakeup */
#endif

/* ===========================================================================
**  
**  FUNCTION      GPIOIntr_IsSupported
**
** ======================================================================== */

DALResult GPIOIntr_MapMPMInterrupt
(
  GPIOIntr_ClientHandle * pCtxt,
  uint32 nGPIO,
  uint32 mpm_interrupt_id
)
{
  GPIOIntr_DeviceHandle *hDevice;
  
  hDevice = pCtxt->hDevice;
  
  if (GPIOIntr_IsSupported(hDevice,nGPIO) == FALSE)
  {
    return DAL_ERROR;
  }
  else
  {
    GPIOINT_LOCK(hDevice);
    hDevice->pGPIOIntData->state[nGPIO].mpm_intr_id = mpm_interrupt_id;                      
    hDevice->pGPIOIntData->state[nGPIO].gpio_intr_flags  |= GPIOINTF_MPMINT;     
    if (hDevice->pGPIOIntData->state[nGPIO].gpio_intr_flags  & GPIOINTF_REGISTERED)
    {
      #ifdef GPIOINT_USE_MPM
      GPIOIntr_ConfigMPMWakeup(hDevice->hDalMpmHandle,nGPIO,TRUE);
      #ifdef GPIOINT_USE_NPA
      uint32 nDeviceType;
      GPIOIntFnDescType** apfnSupportedDrivers;
      nDeviceType = hDevice->pGPIOIntData->state[nGPIO].nDeviceType;
      if (GPIOINT_NO_DEVICETYPE != nDeviceType)
      {
        apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
        apfnSupportedDrivers[nDeviceType]->GPIOIntFn_DecrementNonMPMInterrupts(nGPIO);
      }
      #endif /* GPIOINT_USE_NPA*/
      #endif /* GPIOINT_USE_MPM */
    }
    GPIOINT_FREE(hDevice);
  }
  
  if(hDevice->pGPIOIntData->gpioint_uimage != 0)
  {
    uGPIOInt_MapMPMInterrupt(nGPIO, mpm_interrupt_id);
  }
  
   
  return DAL_SUCCESS;

} /* END GPIOIntr_MapMPMInterrupt */

/* ===========================================================================
**  
**  FUNCTION      GPIOIntr_SetDirectConnectGPIOMapping
**
** ======================================================================== */

DALResult GPIOIntr_SetDirectConnectGPIOMapping
(
  GPIOIntr_ClientHandle * pCtxt,
  uint32 nGPIO,
  uint32 direct_connect_line
)
{
  DALResult eResult = DAL_ERROR;
  GPIOIntr_DeviceHandle *hDevice;
  uint32 nDeviceType, nCount;
  GPIOIntFnDescType** apfnSupportedDrivers;
  GpioIntr_Attribute aAttr;
  
  hDevice = pCtxt->hDevice;
  
  if (GPIOIntr_IsSupported(hDevice,nGPIO) == FALSE)
  {
    return DAL_ERROR;
  }
  else
  {
    GPIOINT_LOCK(hDevice);
    /* Populate attribute struct */
   GPIOIntr_GetAttributes(GPIOINT_TRIGGER_HIGH, &aAttr, 0, nGPIO);

  /* determine best match for Gpio from the Priority List of GpioInt DeviceTypes */
   apfnSupportedDrivers = hDevice->pGPIOIntData->gpioint_priority_list;
   nCount = hDevice->pGPIOIntData->gpioint_priority_list_count;
   
   if(DalGPIOMgr_LockGPIO(hDevice->hGPIOMgrHandle, nGPIO, qurt_process_get_id()) != DAL_SUCCESS)
   {
     /*
      * Restore interrupts and return an error. 
      */
     GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_RegisterIsr: GPIOMgr failed to lock GPIO %d", nGPIO);
     GPIOINT_FREE(hDevice);
     return DAL_ERROR;
   }
   
   for(nDeviceType = 0; nDeviceType < nCount; nDeviceType++)
   {
       if( DAL_SUCCESS == apfnSupportedDrivers[nDeviceType]->GPIOIntFn_IsSupported(nGPIO, &aAttr) )
       {
           hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->nInterruptType = GPIOINT_DEFAULT;
           hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->nInterruptID   = GPIOINT_NO_QURT_ID;
           hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->eIntrTrigger =  GPIOINT_TRIGGER_INVALID;
           eResult = apfnSupportedDrivers[nDeviceType]->GPIOIntFn_Config(nGPIO, 
                                                                        &aAttr, 
                                                                        hDevice->pGPIOIntData->state[nGPIO].pIntrCfg);
          
           if(hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->nInterruptID != direct_connect_line)
           {
             GPIOIntLog_Out(GPIOINTLOG_REG, hDevice->hULog, 1, 
                    "GPIOIntr_SetDirectConnectGPIOMapping: Mapped  GPIO %d to different interrupt ID", nGPIO, hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->nInterruptID);
             return DAL_ERROR;
           }
           if( eResult == DAL_SUCCESS )
           {
              /* capture the GPIOInt DeviceType */
              hDevice->pGPIOIntData->state[nGPIO].nDeviceType = nDeviceType;
              hDevice->pGPIOIntData->state[nGPIO].pIntrCfg->eIntrTrigger = GPIOINT_TRIGGER_HIGH;
              hDevice->pGPIOIntData->state[nGPIO].gpio_intr_flags |= GPIOINTF_EXTERNAL_CONFIG;
              break;
           }
       }
   }
    GPIOINT_FREE(hDevice);
  }
 
   
  return DAL_SUCCESS;

} /* END GPIOIntr_SetDirectConnectGPIOMapping */