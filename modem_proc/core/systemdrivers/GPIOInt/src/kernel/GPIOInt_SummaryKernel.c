/*==============================================================================

FILE:      GPIOInt_SummaryKernel.c

DESCRIPTION
  This modules implements the API to utilize the Dal GPIO interrupt controller. 
  The definitions in this module are implemented internally and should not be 
  exposed for external client usage directly without using the accompanying DDI
  interface file for this module.

REFERENCES

       Copyright © 2015 Qualcomm Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt_SummaryKernel.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
3/18/2015  cpaulo  First draft created. 
===========================================================================*/
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DDITimetick.h"
#include "DDIGPIOMgr.h"
#include "DDITlmm.h"
#include "busywait.h"
#include "DDIInterruptController.h"
#include "DALGlbCtxt.h"
#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "GPIOInt_Summary.h"
#include "DDIHWIO.h"
#include "qurt.h"
#include "GPIOIntQDI.h"


/*==========================================================================

  FUNCTION      GPIOIntr_Isr

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

extern DALResult SummaryGpioInternal_FindIdxByGpio(uint32 nGpio, uint32 *nIdx);

void 
GPIOIntr_Isr
(
  void *pCtxt
)
{
  uint32  nIdx, nGpio, pending;
  uint32  retry_count = 0;
  uint32 nPD = 0;
  GPIOIntTriggerType eTrigger = GPIOINT_TRIGGER_HIGH;
  SummaryGpioCfgCtxtType *device = (SummaryGpioCfgCtxtType *)pCtxt;
  boolean bIsEdgeTriggered = TRUE;

  (void)qurt_rmutex_lock(device->pLock);
  /*
   *The idea is to	- loop over the known gpio interrupts and if none are pending, 
   *check the summary interrupt.  Repeat this 10 times and if the summary line continues to 
   *be asserted and we cannot find any pending GPIO interrupts then abort.
   */
  do
  {
   /*
    * We would only poll the GPIO INTR Status Regs for which an Handler is registered.          
    */      
    for(nGpio = 0; nGpio < MAX_NUMBER_OF_GPIOS; nGpio++)
    {
      nPD = 0;
      nIdx = 0;

      /* If GPIO found to be in-use by GpioMgr, get corresponding PD for GPIO */
      if ( DAL_SUCCESS != DalGPIOMgr_GetGPIOCurrentPD( device->hGPIOMgrHandle, nGpio, &nPD ) )
      {
          continue;
      }

      /* If GPIO found and in-use by RootPD, determine index in internal state table of RootPD */
      if( nPD == 0 ) 
      {
          if ( DAL_SUCCESS != SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx) )
          {
              continue;
          }
      }

      /* Check if GPIO is registered in RootPD or if nPD is not RootPD */
      if( device->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED ||
          nPD != 0 )
      {
        if(SummaryGpioInternal_IsPending(nGpio) == TRUE)
        {
          if (nPD == 0) 
          {
            /*
             * Run the ISR
             */
            GPIOIntr_RunIsr(device, nIdx);
          }
          else
          {

            /* 
             * Get the trigger type to determine when to clear the interrupt
             */
            SummaryGpioInternal_GetTrigger( nGpio, &eTrigger );
      	  
            if( eTrigger == GPIOINT_TRIGGER_HIGH ||
                eTrigger == GPIOINT_TRIGGER_LOW )
            {
               bIsEdgeTriggered = FALSE;
            }

            /*
              * Clear the interrupt before if it's edge triggered
              */
            if( bIsEdgeTriggered )
            {
              SummaryGpioInternal_Clear(nGpio);
            }

           /*
             * Send the GPIO Interrupt to the correct PD
             */
            GPIOIntQDI_SetSummarySignal(nPD, nGpio);

            /*
             * Wait for Ack from User PD
             */
            GPIOIntQDI_WaitOnAckSummarySignal(nPD);

            /*
              * Clear the interrupt after if it's level triggered
              */
            if( !bIsEdgeTriggered )
            {
              SummaryGpioInternal_Clear(nGpio);
            }

         }

         /* 
          * Get the summary INTR pending status to check if we have serviced all the pending ones.
          */
          DalInterruptController_IsInterruptPending(device->hIntrCntrlHandle, 
                (DALInterruptID)device->nSummaryIntrID,&pending);

          if(!pending)
          {
          /* Summary INTR is not pending, all the pending GPIOINTRs are serviced
           * No need to check the remaining GPIO INTR status Registers.
           */
            (void)qurt_rmutex_unlock(device->pLock);
            return;
          }

         /*
          * we need to loop over 10 times consecutively with summary INTR status pending.
          * Reset retry_count = 0 if a valid GPIOINTR is triggered in the middle.   		  
          */		   
          retry_count  = 0;		   
        } 	  
      }
    }

    /* 
    * Get the summary INTR pending status to check if the summary line is still asserted.
    */
    DalInterruptController_IsInterruptPending(device->hIntrCntrlHandle, 
                           (DALInterruptID)device->nSummaryIntrID,&pending);

  }while((pending) && (++retry_count < GPIOINT_SUMMARY_INTR_NO_OF_TRIALS));

  if(retry_count >= GPIOINT_SUMMARY_INTR_NO_OF_TRIALS)
  {
    device->gpioint_unhandled++;		
    /*
     *The summary line continues to be asserted and we could not find any pending GPIO interrupts.		
     *ERROR_FATAL if a GPIOINT is not found in 10 retrials.
     */
      GPIOIntLog_Out(GPIOINTLOG_SUMMARYISR, device->hULog, 0, 
          "GPIOIntr_Isr: GPIO IRQ is wrongly targeted to LPASS QDSP by another Subsystem"); 
      //GPIOINT_TODO: Call Error Fatal!
  }

  (void)qurt_rmutex_unlock(device->pLock);

} /* END GPIOIntr_isr */


/*==========================================================================

  FUNCTION      GPIOIntr_LogEvent

  DESCRIPTION   See GPIOInt.h

==========================================================================*/
//GPIOINT_TODO: move to GpioInt Driver Fwk ?

void 
GPIOIntr_LogEvent
(
  SummaryGpioCfgCtxtType *device,
  uint32 gpio
)
{
#if 0
#ifdef GPIOINT_KERNELPDLOG
  GPIOIntCntrlType *gpioint_cntrl = device->gpioint_cntrl;
  uint32 index = gpioint_cntrl->log.index;
  gpioint_cntrl->log.entries[index].gpio = gpio;
  DalTimetick_GetTimetick64(device->timetick_handle, &gpioint_cntrl->log.entries[index].timestamp);

  if (++gpioint_cntrl->log.index >= MAX_INTERRUPT_LOG_ENTRIES)
  {
    gpioint_cntrl->log.index = 0;
  }
#endif
#endif
} /* END GPIOIntr_log_event */


/*==========================================================================

  FUNCTION      GPIOIntr_RunIsr

  DESCRIPTION   See GPIOInt.h

==========================================================================*/

void 
GPIOIntr_RunIsr
(
  SummaryGpioCfgCtxtType *device,
  uint32 nIter
)
{ 
  boolean bIsEdgeTriggered = TRUE;
  GPIOIntTriggerType eTrigger = GPIOINT_TRIGGER_HIGH;
  uint32 nGpio = device->pStateTable[nIter].nGpio;
  GPIOINTISR local_isr = NULL;
  GPIOINTISRCtx local_isr_param;
  
  /* 
   * Get the trigger type to determine when to clear the interrupt
   */
  SummaryGpioInternal_GetTrigger( nGpio, &eTrigger );

  if( eTrigger == GPIOINT_TRIGGER_HIGH ||
      eTrigger == GPIOINT_TRIGGER_LOW )
  {
    bIsEdgeTriggered = FALSE;
  }

  /*
   * Log the GPIO that fired
   */
  GPIOIntr_LogEvent(device, nGpio);

  /*
   * Clear the interrupt before if it's edge triggered
   */
  if( bIsEdgeTriggered )
  {
    SummaryGpioInternal_Clear(nGpio);
  }
  
  local_isr = device->pStateTable[nIter].isr;
  local_isr_param = device->pStateTable[nIter].isr_param;
  
  (void)qurt_rmutex_unlock(device->pLock);

  /*
   * Get the ISR and run it
   */
  if(local_isr)
  {
   local_isr(local_isr_param);
  }
  
  (void)qurt_rmutex_lock(device->pLock);

  /*
   * Clear the gpio interrupt if it is not owned by the apps processor.
   * During power collapse, the apps interrupt will be preserved to trigger
   * an apps interrupt once gpios are switched back to the apps processor.
   * The apps GPIO monitoring ISR (GPIOIntr_monitor_apps_isr) has disabled
   * the interrupt at this point. 
   * Clear the interrupt after the ISR is called if it's level triggered. 
   */
  if( !bIsEdgeTriggered )
  {
    SummaryGpioInternal_Clear(nGpio);
  }

} /* END GPIOIntr_run_isr */

DALResult
GPIOIntrSummaryInternal_Init(SummaryGpioCfgCtxtType *device)
{
  DALResult eResult;
  uint32 gpio;
  uint32 req_size;
  uint32 gpioint_bitmask;  
  uint32 gpioint_glbctxt_idx;
  GPIOIntGlbCtxtType *pgpioint_glb_ctxt;    

  /* Initialize QDI server */
  GPIOIntQDI_Init();

  device->gpioint_glb_ctxt = NULL;
  //GPIOINT_TODO: review GlobalCtxt: is it a wrapper over SMEM that all Q6s access same shared memory?
  eResult = DALGLBCTXT_FindCtxt("GPIOINTLPASS", (void **)&device->gpioint_glb_ctxt);

  /*
   * If GPIOInt DAL global context does not exist, we need to allocate it.
   * If it already exists i.e. after SSR, there is no need to allocate. 
   */
  if(device->gpioint_glb_ctxt == NULL)
  {
   /*
    * Allocate the DAL global context.                                                 
    * Each bit in gpioint_reg_state[] represent a GPIO interrupt.
    * req_size is number bytes required to save the registration status of all the gpios.
    * req_size = ((gpio_number / 32) + 1) Words.
    * req_size = (((gpio_number / 32) + 1) * 4) Bytes.
    */
    req_size = (((device->nTotalGpioCount / 32) + 1) * 4);

    eResult     = DALGLBCTXT_AllocCtxt("GPIOINTLPASS",sizeof(GPIOIntGlbCtxtType) + req_size,
                      DALGLBCTXT_LOCK_TYPE_SPINLOCK, (void **)&device->gpioint_glb_ctxt);

    if (eResult != DAL_SUCCESS)
    {
      GPIOIntLog_Out(GPIOINTLOG_INIT, device->hULog, 0, 
                    "GPIOIntrSummaryInternal_Init: not able to allocate DAL global context memory"); 

      return DAL_ERROR;
    }

    /*
     * Clear the DAL global context excluding its Header.
     */	 
    memset((void *)device->gpioint_glb_ctxt->gpioint_reg_state, 0x0, req_size);
  }
  else
  { 
    /*  Get the GPIOInt registration status from DAL global context,
     *  If an interrupt is enabled and target processor is current processor,
     *  then disable it and clear the pending status.   
     */ 
    pgpioint_glb_ctxt   = device->gpioint_glb_ctxt;	

    for(gpio = 0; gpio < device->nTotalGpioCount; gpio++)
    {
      /* 
       * 32 is the size of data type of gpioint_reg_state[] in bits.
       * Each bit in gpioint_reg_state[] represents a GPIO interrupt.
       * We need ((gpio_number / 32) + 1) words to store the registration 
       * status of gpio_number GPIOs.
       */	
      gpioint_glbctxt_idx = gpio / 32;
      gpioint_bitmask     = 0x1 << (gpio % 32);

      if(pgpioint_glb_ctxt->gpioint_reg_state[gpioint_glbctxt_idx] & gpioint_bitmask)
      {
        pgpioint_glb_ctxt->gpioint_reg_state[gpioint_glbctxt_idx] &= ~(gpioint_bitmask);

        if(SummaryGpioInternal_IsEnabled(gpio))
        {
          SummaryGpioInternal_Clear(gpio);
          SummaryGpioInternal_Disable(gpio);
        }
      }
    }
  }

  eResult = DalInterruptController_RegisterISR(device->hIntrCntrlHandle, 
                                               (DALInterruptID)device->nSummaryIntrID,
                                               (DALISR)GPIOIntr_Isr, 
                                               (DALISRCtx)device, 
                                               DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER);


  if ( eResult != DAL_SUCCESS )
  {
    return DAL_ERROR;
  }

  return eResult;
}


DALResult
GPIOIntrSummaryInternal_UnConfigureGPIOInterrupt( SummaryGpioCfgCtxtType *device, uint32 gpio )
{
  uint32 gpioint_bitmask;  
  uint32 gpioint_glbctxt_idx;
  GPIOIntGlbCtxtType *pgpioint_glb_ctxt;    

 /* 
  * Clear the registration status of summary interrupt in DAL global context.
  */   
 /* 
  * 32 is the size of data type of gpioint_reg_state[] in bits.
  * Each bit in gpioint_reg_state[] represents a GPIO interrupt.
  * We need ((gpio_number / 32) + 1) words to store the registration 
  * status of gpio_number GPIOs.
  */
  gpioint_glbctxt_idx = gpio / 32;	
  gpioint_bitmask     = 0x1 << (gpio % 32);			
  pgpioint_glb_ctxt   = device->gpioint_glb_ctxt;	 

  pgpioint_glb_ctxt->gpioint_reg_state[gpioint_glbctxt_idx] &= ~(gpioint_bitmask);

  return DAL_SUCCESS;
}

DALResult
GPIOIntrSummaryInternal_ConfigureGPIOInterrupt( SummaryGpioCfgCtxtType *device, uint32 gpio )
{
  uint32 gpioint_bitmask;  
  uint32 gpioint_glbctxt_idx;
  GPIOIntGlbCtxtType *pgpioint_glb_ctxt;    

  /* 
   * Update the registration status of summary interrupt in DAL global context.
   */  

  /* 
   * 32 is the size of data type of gpioint_reg_state[] in bits.
   * Each bit in gpioint_reg_state[] represents a GPIO interrupt.
   * We need ((gpio_number / 32) + 1) words to store the registration 
   * status of gpio_number GPIOs.
   */
  gpioint_glbctxt_idx = gpio / 32;	
  gpioint_bitmask     = 0x1 << (gpio % 32);			
  pgpioint_glb_ctxt   = device->gpioint_glb_ctxt;	 

  pgpioint_glb_ctxt->gpioint_reg_state[gpioint_glbctxt_idx] |= gpioint_bitmask;	
  

  return DAL_SUCCESS;
}

