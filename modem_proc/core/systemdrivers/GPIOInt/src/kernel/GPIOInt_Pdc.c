/*==============================================================================

FILE:      GPIOInt_Pdc.c

DESCRIPTION
  This modules implements the API to utilize GPIO interrupt controller for a
  specific type of GPIOInt - PDC Interrupts.
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

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt_Pdc.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/15/2017  hvm     First draft created. 
===========================================================================*/

/*=========================================================================
      Include Files
==========================================================================*/

#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "DALGlbCtxt.h"
#include "DDIInterruptController.h"
#include "qurt.h"
#include "ULog.h"
#include "ULogFront.h"

#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "GPIOInt_Pdc.h"
#include "pdcGpio.h"
#include "HALtlmmtile.h"

/*=========================================================================
      Macros
==========================================================================*/


/*=========================================================================
      Data Types
==========================================================================*/

/* 
 * Func Pointer Table for Pdc Gpio interrupts
 */

GPIOIntFnDescType PdcGpio_Procs = {
  PdcGpio_Init,
  PdcGpio_DeInit,
  PdcGpio_IsSupported,
  PdcGpio_Config,
  PdcGpio_Deconfig,
  PdcGpio_Register,
  PdcGpio_Deregister,
  PdcGpio_Enable,
  PdcGpio_Disable,
  PdcGpio_SetTrigger,
  PdcGpio_TriggerInterrupt,
  PdcGpio_ClearInterrupt,
  PdcGpio_IsInterruptSet,
  PdcGpio_IsInterruptPending,
  PdcGpio_IsInterruptEnabled
};

const uint32 nPdcTriggerMap[GPIOINT_TRIGGER_INVALID] = 
{
  DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER, /*GPIOINT_TRIGGER_HIGH*/
  DALINTRCTRL_ENABLE_LEVEL_LOW_TRIGGER, /*GPIOINT_TRIGGER_LOW*/
  DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER, /*GPIOINT_TRIGGER_RISING*/
  DALINTRCTRL_ENABLE_FALLING_EDGE_TRIGGER, /*GPIOINT_TRIGGER_FALLING*/
  DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER /*GPIOINT_TRIGGER_DUAL_EDGE*/
};

/*=========================================================================
      Forward references
==========================================================================*/

static PdcGpioCfgCtxtType PdcGpio_CfgCtxt = {0};

pdc_trigger_type PdcGpioInternal_GetPdcIntrTrigger( GPIOIntTriggerType eTrigger );

uint32           PdcGpioInternal_GetIntCTrigger   ( GPIOIntTriggerType eTrigger );

DALResult PdcGpioInternal_FindIdxByGpio( uint32 nGpio, uint32 *nIdx );

DALResult PdcGpioInternal_GetFreeIdxForGpio( uint32 nGpio, uint32 *nIdx );

void PdcGpioInternal_LogInit( ULogHandle* hL );

/*=========================================================================
      Internal Helper Functions
==========================================================================*/

/*==========================================================================
   FUNCTION      PdcGpioInternal_AttachToDals
==========================================================================*/

DALResult 
PdcGpioInternal_AttachToDals
(
  PdcGpioCfgCtxtType *pCtxt
)
{
  /* Attach to the Main DAL Interrupt Controller */
  if(DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, 
                                     &pCtxt->hIntrCntrlHandle))
  {
      return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END PdcGpioInternal_AttachToDals */

/*==========================================================================
   FUNCTION      PdcGpioInternal_DetachToDals
==========================================================================*/

DALResult 
PdcGpioInternal_DetachToDals
(
  PdcGpioCfgCtxtType *pCtxt
)
{
  /* Attach to the Main DAL Interrupt Controller */
  if(DAL_SUCCESS != DAL_DeviceDetach(pCtxt->hIntrCntrlHandle))
  {
      return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END PdcGpioInternal_DetachToDals */


/*==========================================================================
   FUNCTION      PdcGpioInternal_GetTargetData
==========================================================================*/

DALResult 
PdcGpioInternal_GetTargetData
(
  PdcGpioCfgCtxtType *pCtxt
)
{
  /* 
   * declare property handle variable.
   */
  DALSYS_PROPERTY_HANDLE_DECLARE(hPropertyHandle);
  DALSYS_GetDALPropertyHandleStr("PdcGpioInt", hPropertyHandle);

  /* determine number of supported Pdc Gpio Interrupts from PDC driver*/ 
  pCtxt->nTotalGpioCount = pdcGpio_getNumberSupported();
  if(pCtxt->nTotalGpioCount == 0)
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "PdcGpio_GetTargetData:FAIL.Total wakeup capable Gpio is corrupt");
      return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END PdcGpioInternal_GetTargetData */


/*==========================================================================
   FUNCTION      PdcGpioInternal_GetPdcIntrTrigger
==========================================================================*/

pdc_trigger_type PdcGpioInternal_GetPdcIntrTrigger( GPIOIntTriggerType eTrigger )
{
  pdc_trigger_type nPdcIntrTrigger;

  switch (eTrigger)
  {

    case GPIOINT_TRIGGER_DUAL_EDGE:
         nPdcIntrTrigger = TRIGGER_DUAL_EDGE;
         break;

    case GPIOINT_TRIGGER_HIGH:
         nPdcIntrTrigger = TRIGGER_LEVEL_HIGH;
         break;

    case GPIOINT_TRIGGER_LOW:
         nPdcIntrTrigger = TRIGGER_LEVEL_LOW;
         break;

    case GPIOINT_TRIGGER_RISING:
         nPdcIntrTrigger = TRIGGER_RISING_EDGE;
         break;

    case GPIOINT_TRIGGER_FALLING:
         nPdcIntrTrigger = TRIGGER_FALLING_EDGE;
         break;

    default:
         nPdcIntrTrigger = TRIGGER_LEVEL_HIGH;
         break;
  }

  return nPdcIntrTrigger;
} /* END PdcGpioInternal_GetPdcIntrTrigger */

/*==========================================================================
   FUNCTION      PdcGpioInternal_GetIntCTrigger
==========================================================================*/

uint32 PdcGpioInternal_GetIntCTrigger(GPIOIntTriggerType  eTrigger)
{
  if ( eTrigger >= GPIOINT_TRIGGER_INVALID )
  {
    return DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER;
  }
  
  return nPdcTriggerMap[eTrigger];
} /* END PdcGpioInternal_GetIntCTrigger */

/*==========================================================================
   FUNCTION      PdcGpioInternal_FindIdxByGpio
==========================================================================*/

/**
  Gets the Index of the entry in the State Table for the GPIO given.

  @param[in]  nGpio       The GPIO pin number for PDC GpioInt.
  @param[out] nIdx        Index of entry in Pdc State Table
        assigned to GPIO.
  @return
  DAL_SUCCESS -- is returned if entry is found in State Table for the GPIO given.
  DAL_ERROR -- is returned if there is no entry for the GPIO given. 

*/

DALResult PdcGpioInternal_FindIdxByGpio(uint32 nGpio, uint32 *nIdx)
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIter;

   for ( nIter = 0; nIter < pCtxt->nTotalGpioCount; nIter++ )
   {
       if ( pCtxt->pStateTable[nIter].nGpio == nGpio )
       {
           *nIdx = nIter;
           eResult = DAL_SUCCESS;
           break;
       }
   }

   return eResult;
} /* END PdcGpioInternal_FindIdxByGpio */


/*==========================================================================
   FUNCTION      PdcGpioInternal_GetFreeIdxForGpio
==========================================================================*/

/**
  Gets a Free Index in the State Table for the GPIO given.

  @param[in]  nGpio       The GPIO pin number to update in state table. 
  @return
  DAL_SUCCESS -- is returned if a free index is found.
  DAL_ERROR -- is returned if there are no more free indices in State Table.

*/

DALResult PdcGpioInternal_GetFreeIdxForGpio(uint32 nGpio, uint32 *nIdx)
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIter;

   for ( nIter = 0; nIter < pCtxt->nTotalGpioCount; nIter++ )
   {
       if ( pCtxt->pStateTable[nIter].nGpio == GPIOINT_NONE ||
            pCtxt->pStateTable[nIter].nGpio == nGpio )
       {
           *nIdx = nIter;
           eResult = DAL_SUCCESS;
           break;
       }
   }

   return eResult;
} /* END PdcGpioInternal_GetFreeIdxForGpio */

/* =========================================================================
**  Function : PdcGpioInternal_LogInit
** =========================================================================*/
/**
  Initialize the uLog required for PdcGpioInt.

  @return  None
*/

void PdcGpioInternal_LogInit(ULogHandle* hL)
{
   char sztGPIOIntHeader[] = "PdcGpioInt Log";

   ULogFront_RealTimeInit(hL,
                          "PdcGpioInt",
                          4096,
                          ULOG_MEMORY_LOCAL,
                          ULOG_LOCK_OS);

   if ( hL != NULL )
   {
     ULogCore_HeaderSet(*hL, sztGPIOIntHeader);
   }
} /* END PdcGpioInternal_LogInit */

/*=============================================================================
  FUNCTION      PdcGpioInternal_ReconfigureDualEdgePdcInterrupt
=============================================================================*/
static
void PdcGpioInternal_ReconfigurePDCGpio(uint32 nGpio, boolean bEnable, GPIOIntTriggerType eTrigger)
{
   pdc_trigger_type nPdcIntrTrigger;
   uint32 nIdx;
   pdc_trigger_config  sConfig;
   uint32 nPdcIntrID, nAltIntrId = GPIOINT_NONE;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   DALResult eResult;
   
   eResult = PdcGpioInternal_GetFreeIdxForGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
     GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                    "PdcGpioInternal_ReconfigurePDCGpio:FAIL. GetFreeIdxForGpio() Failed for GPIO %d", 
                    nGpio);
   }
   
   
   nPdcIntrTrigger = PdcGpioInternal_GetPdcIntrTrigger(eTrigger);
   
   
   sConfig.drv_num = PDC_DRV0;
   sConfig.trig_type = nPdcIntrTrigger;

   if( PDC_SUCCESS == pdcGpio_config(nGpio, &sConfig, &nPdcIntrID, &nAltIntrId) ) 
   {
      pCtxt->pStateTable[nIdx].nGpio = nGpio;
      pCtxt->pStateTable[nIdx].nQurtIntrID = nPdcIntrID;
      pCtxt->pStateTable[nIdx].eIntrTrigger = eTrigger;
      GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 3, 
                     "PdcGpioInternal_ReconfigurePDCGpio:SUCCESS. GPIO %d with Trigger %d configured as PDC GPIO Interrupt with Interrupt Number %d", 
                     nGpio, (uint32)eTrigger, nPdcIntrID);
	  if(bEnable)
	  {
	    pdcGpio_enable(nGpio);
	  }
   }
}

void PdcGpioInternal_ReconfigureDualEdgePdcInterrupt(uint32 nGpio)
{
  PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
  uint32 nInVal1 = 0, nInVal2 = (uint32)-1,nIdx;
  DALResult eResult;
  
  eResult = PdcGpioInternal_GetFreeIdxForGpio(nGpio, &nIdx);
  if( DAL_SUCCESS != eResult )
  {
    GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                   "PdcGpioInternal_ReconfigureDualEdgePdcInterrupt:FAIL. GetFreeIdxForGpio() Failed for GPIO %d", 
                    nGpio);
  }
 
  if ( pCtxt->pStateTable[nIdx].eIntrTrigger == GPIOINT_TRIGGER_DUAL_EDGE )
  {
    while ( nInVal1 != nInVal2 )
    {
      
      nInVal1 = HAL_tlmmtile_Get(nGpio, HAL_TLMMTILE_REG_INOUT, HAL_TLMMTILE_FIELD_INOUT_IN);

      if ( nInVal1 == 0 )
      {
        /*Input is low , configure rising edge */
        PdcGpioInternal_ReconfigurePDCGpio(nGpio, FALSE, GPIOINT_TRIGGER_RISING);
      }
      else 
      {
        /*Input is high , configure falling edge */
        PdcGpioInternal_ReconfigurePDCGpio(nGpio, FALSE, GPIOINT_TRIGGER_FALLING);
      }

      
      nInVal2 = HAL_tlmmtile_Get(nGpio, HAL_TLMMTILE_REG_INOUT, HAL_TLMMTILE_FIELD_INOUT_IN);
    }
  }
}

void PdcGpioInternal_DualEdgeIsr(void *param)
{
  GPIOINTISR pfnIsr;
  GPIOINTISRCtx nParam;
  uint32 nGpio = (uint32)param ,nIdx;
  DALResult eResult;
  PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
  
  (void)qurt_rmutex_lock(pCtxt->pLock);
   
  eResult = PdcGpioInternal_GetFreeIdxForGpio(nGpio, &nIdx);
  if( DAL_SUCCESS != eResult )
  {
    GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                   "PdcGpioInternal_DualEdgeIsr:FAIL. GetFreeIdxForGpio() Failed for GPIO %d", 
                    nGpio);
  }
   
  PdcGpioInternal_ReconfigureDualEdgePdcInterrupt(nGpio);
  pfnIsr = pCtxt->pStateTable[nIdx].isr;
  nParam = pCtxt->pStateTable[nIdx].isr_param;
  (void)qurt_rmutex_unlock(pCtxt->pLock);

  if( pfnIsr != NULL )
  {
    pfnIsr(nParam);
  }
}
/*=========================================================================
      Functions
==========================================================================*/

/*==========================================================================

  FUNCTION      PdcGpio_Init

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Init(GPIOIntr_InitDataType *GPIOIntr_InitData)
{

   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   /*
    * Setup uLog buffer 
    */ 
   PdcGpioInternal_LogInit(&pCtxt->hULog);
   if(pCtxt->hULog == NULL)
   {
       return DAL_ERROR;
   }

   if(!pCtxt->bIsInitialized)
   {
      /*
       * Attachs to the DALs which this DeviceType driver depends on.
       */
      if(DAL_SUCCESS != PdcGpioInternal_AttachToDals(pCtxt))
      {
          return DAL_ERROR;
      }

      /*
       * Get Interrupt Config Data from PDC GpioInt DAL Props
       */
      if(DAL_SUCCESS != PdcGpioInternal_GetTargetData(pCtxt))
      {
          return DAL_ERROR;
      }

      /*
       * Allocate memory for the Pdc GPIOInt State Table
       */
      if(DAL_SUCCESS != DALSYS_Malloc((pCtxt->nTotalGpioCount * sizeof(PdcGpioCfgDataType)),
                                      (void **)&(pCtxt->pStateTable)))
      {
          return DAL_ERROR;
      }

      /*
       * Map the correct Func Pointer Table 
       */
      pCtxt->pVtbl = &PdcGpio_Procs;

      /* Setup local State Table */
      for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
      {
          pCtxt->pStateTable[nIdx].nGpio = GPIOINT_NONE;
          pCtxt->pStateTable[nIdx].nQurtIntrID = GPIOINT_NO_QURT_ID;
      }
      
      /*
       * Store the lock passed in the init 
       */
      pCtxt->pLock = GPIOIntr_InitData->pLock;
	  
      /*
       * Flag the mini-driver as initialised. 
       */ 
      pCtxt->bIsInitialized = 1;

      eResult = DAL_SUCCESS;
   }

   return eResult;
} /* END PdcGpio_Init */

/*==========================================================================

  FUNCTION      PdcGpio_DeInit

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_DeInit( void )
{
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   /* 
    * Deregister all ISRs for all local registered GPIOs
    */
   for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
   {
       if( (pCtxt->pStateTable[nIdx].nGpio != GPIOINT_NONE) &&
           (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED) )
       {
           if(DAL_SUCCESS != PdcGpio_Deregister(pCtxt->pStateTable[nIdx].nGpio))
           {
               /* Do Nothing, rather move on with rest of the driver tear-down */
           }
       }
   }

   /* Detach for attached DAL drivers */
   PdcGpioInternal_DetachToDals(pCtxt);

   /* Free up any memory allocated */
   DALSYS_Free(&(pCtxt->pStateTable));

   /*
	* Flag the mini-driver as uninitialised. 
	*/ 
   pCtxt->hULog = NULL;
   pCtxt->bIsInitialized = 0;

   /* Free up any resource obtained */

   return DAL_SUCCESS;
} /* END PdcGpio_DeInit */

/*==========================================================================

  FUNCTION      PdcGpio_IsSupported

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr )
{
    if( TRUE == pdcGpio_isSupported(nGpio) ) 
    {
        return DAL_SUCCESS;
    }
   return DAL_ERROR;
} /* END PdcGpio_IsSupported */

/*==========================================================================

  FUNCTION      PdcGpio_Config

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Config( uint32 nGpio, 
                          GpioIntr_Attribute *pAttr, 
                          GpioIntr_CfgType *pIntrCfgData )
{
   DALResult eResult = DAL_ERROR;
   pdc_trigger_type nPdcIntrTrigger;
   uint32 nIntCTrigger, nIdx;
   uint32 nPdcIntrID, nAltIntrId = GPIOINT_NONE;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   pdc_trigger_config  sConfig;

   /* Verify attribute */
   if ( pAttr == NULL )
   {
     GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                    "PdcGpio_Config:FAIL. pAttr is NULL, Failed for GPIO %d", 
                    nGpio);
     return DAL_ERROR;
   }

   eResult = PdcGpioInternal_GetFreeIdxForGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_Config:FAIL. GetFreeIdxForGpio() Failed for GPIO %d", 
                      nGpio);
       return DAL_ERROR;
   }

   /* Determine the PDC interrupt trigger configuration */
   nPdcIntrTrigger = PdcGpioInternal_GetPdcIntrTrigger(pAttr->eTrigger);

   /* Determine Main DAL Interrupt Controller Trigger configuration */
   //GPIOINT_TODO: All locations and calls to DAL APIs - Review for trigger type "HAL_gpioint_TriggerType"
   nIntCTrigger = PdcGpioInternal_GetIntCTrigger(pAttr->eTrigger);

   /* Configure PDC GPIO interrupt */
   sConfig.drv_num = PDC_DRV0;
   sConfig.trig_type = nPdcIntrTrigger;

   //GPIOINT_TODO: Review use for AltIntrId with Chris P. for uGpioInt driver
   if( PDC_SUCCESS == pdcGpio_config(nGpio, &sConfig, &nPdcIntrID, &nAltIntrId) ) 
   {
      pCtxt->pStateTable[nIdx].nGpio = nGpio;
      pCtxt->pStateTable[nIdx].nQurtIntrID = nPdcIntrID;
      pCtxt->pStateTable[nIdx].eIntrTrigger = pAttr->eTrigger;
      pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_WAKEUP;
      pCtxt->pStateTable[nIdx].nPriority = pAttr->nPriority;
      pCtxt->pStateTable[nIdx].nUInterrupt = pAttr->nUInterrupt;
      GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 3, 
                     "PdcGpio_Config:SUCCESS. GPIO %d with Trigger %d configured as PDC GPIO Interrupt with Interrupt Number %d", 
                     nGpio, (uint32)pAttr->eTrigger, nPdcIntrID);
	  pdcGpio_enable(nGpio);
      eResult = DAL_SUCCESS;
   }

   /* Update GpioInt Driver Fwk State Table for nGpio */
   pIntrCfgData->nInterruptID = pCtxt->pStateTable[nIdx].nQurtIntrID;
   pIntrCfgData->nInterruptType  = pAttr->eTrigger;

   return eResult;
} /* END PdcGpio_Config */

/*==========================================================================

  FUNCTION      PdcGpio_Deconfig

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Deconfig( uint32 nGpio, 
                            GpioIntr_CfgType *pIntrCfgData)
{
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   /* Release the Mux assigned to this nGpio */
   pdcGpio_unconfig(nGpio);

   /* Update State Table to Default Values */
   if( DAL_SUCCESS != PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx) )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_Deconfig:FAIL. FindIdxForGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   pCtxt->pStateTable[nIdx].nGpio = GPIOINT_NONE;
   pCtxt->pStateTable[nIdx].nQurtIntrID = GPIOINT_NO_QURT_ID;
   pCtxt->pStateTable[nIdx].eIntrTrigger = GPIOINT_TRIGGER_HIGH;
   pCtxt->pStateTable[nIdx].nTypeFlags = 0x0;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "PdcGpio_Deconfig:DONE for GPIO %d",
                  nGpio);

   /* default PDC Conn config data in State Table for nGpio */
   pIntrCfgData->nInterruptID    = GPIOINT_NO_QURT_ID;
   pIntrCfgData->nInterruptType  = GPIOINT_DEFAULT;

   return DAL_SUCCESS;
} /* END PdcGpio_Deconfig */

/*==========================================================================

  FUNCTION      PdcGpio_Register

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Register( uint32 nGpio, void *pfnIsr, void *pIsrParam)
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                      "PdcGpio_Register:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   /* Register ISR for Pdc Gpio Interrupt with Main DAL Interrupt Controller */
   /* Mapping accounts for Dual edge registering for rising edge first */
  

   if (pCtxt->pStateTable[nIdx].eIntrTrigger == GPIOINT_TRIGGER_DUAL_EDGE)
   {
     PdcGpioInternal_ReconfigureDualEdgePdcInterrupt(nGpio);
     /* Register ISR for Pdc Gpio Interrupt with Main DAL Interrupt Controller */
     if( DAL_SUCCESS != DalInterruptController_RegisterISR(pCtxt->hIntrCntrlHandle, 
                                                            pCtxt->pStateTable[nIdx].nQurtIntrID,
                                                           (DALISR)PdcGpioInternal_DualEdgeIsr,
                                                           (DALISRCtx)nGpio,
                                                           DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER))
     {
         GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                        "PdcGpio_Register:FAIL. Failed for GPIO %d", 
                         nGpio);
         return DAL_ERROR;
     }
   }
   else
   {
     if( DAL_SUCCESS != DalInterruptController_RegisterISR(pCtxt->hIntrCntrlHandle, 
                                                          pCtxt->pStateTable[nIdx].nQurtIntrID,
                                                          pfnIsr,
                                                          pIsrParam,
                                                          DALINTRCTRL_ENABLE_CONFIG_EX(
                                                            PdcGpioInternal_GetIntCTrigger(pCtxt->pStateTable[nIdx].eIntrTrigger),
                                                            pCtxt->pStateTable[nIdx].nPriority,
                                                            pCtxt->pStateTable[nIdx].nUInterrupt)))
     {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_Register:FAIL. Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
     }
   }
   pCtxt->pStateTable[nIdx].isr         = pfnIsr;
   pCtxt->pStateTable[nIdx].isr_param   = (GPIOINTISRCtx)pIsrParam;
   pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_REGISTERED;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "PdcGpio_Register:SUCCESS. GPIO %d registered",
                  nGpio);

   return eResult;
} /* END PdcGpio_Register */

/*==========================================================================

  FUNCTION      PdcGpio_Deregister

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Deregister( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                      "PdcGpio_Deregister:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   /* The Main DAL interrupt controller handles the PDC Interrupts */
   eResult = DalInterruptController_Unregister(pCtxt->hIntrCntrlHandle,
                                               pCtxt->pStateTable[nIdx].nQurtIntrID);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_Deregister:FAIL. Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }
   pCtxt->pStateTable[nGpio].isr         = NULL;
   pCtxt->pStateTable[nGpio].isr_param   = NULL;
   pCtxt->pStateTable[nIdx].nTypeFlags &= ~GPIOINTF_REGISTERED;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "PdcGpio_Deregister:SUCCESS. GPIO %d deregistered",
                  nGpio);

   return eResult;
} /* END PdcGpio_Deregister */

/*==========================================================================

  FUNCTION      PdcGpio_Enable

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Enable( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                      "PdcGpio_Enable:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "PdcGpio_Enable: Enabling PDC GPIO %d Interrupt", 
                  nGpio );

   eResult = DalInterruptController_InterruptEnable(pCtxt->hIntrCntrlHandle,
                                                    pCtxt->pStateTable[nIdx].nQurtIntrID);
  
   return eResult;

} /* END PdcGpio_Enable */

/*==========================================================================

  FUNCTION      PdcGpio_Disable

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_Disable( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 2, 
                      "PdcGpio_Disable:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "PdcGpio_Disable: Disabling PDC GPIO %d", nGpio);


   eResult = DalInterruptController_InterruptDisable(pCtxt->hIntrCntrlHandle,
                                                  pCtxt->pStateTable[nIdx].nQurtIntrID);
   
   return eResult;

} /* END PdcGpio_Disable */

/*==========================================================================

  FUNCTION      PdcGpio_SetTrigger

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 1, 
                      "PdcGpio_SetTrigger:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   /* Map the GPIOInt Trigger type to Trigger Type for Main Interrupt Controller */
   pCtxt->pStateTable[nIdx].eIntrTrigger = nNewTrigger;

   /*
    * If the interrupt is a direct connect interrupt we need to set the trigger
    * on the main level 1 interrupt controller and set the polarity on the 
    * secondary GPIOInt interrupt controller. In doing so there might be 
    * spurious interrupts generated hence we need to :
    * 1. Disable the interrupt.
    * 2. Set the trigger.
    * 3. Clear it.
    * 4. Enable it.
    */

   DalInterruptController_InterruptDisable(pCtxt->hIntrCntrlHandle,
                                           pCtxt->pStateTable[nIdx].nQurtIntrID);

   eResult = DalInterruptController_SetTrigger(pCtxt->hIntrCntrlHandle, 
                                               pCtxt->pStateTable[nIdx].nQurtIntrID,
                                               PdcGpioInternal_GetIntCTrigger(pCtxt->pStateTable[nIdx].eIntrTrigger));

   DalInterruptController_InterruptClear(pCtxt->hIntrCntrlHandle, 
                                         pCtxt->pStateTable[nIdx].nQurtIntrID);

   DalInterruptController_InterruptEnable(pCtxt->hIntrCntrlHandle, 
                                          pCtxt->pStateTable[nIdx].nQurtIntrID);

   if ( DAL_SUCCESS != eResult )
   {
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 2, 
                  "PdcGpio_SetTrigger: GPIO %d Set with new Trigger %d", 
                  nGpio, (uint32)nNewTrigger);

   return DAL_SUCCESS;
} /* END PdcGpio_SetTrigger */

/*==========================================================================

  FUNCTION      PdcGpio_TriggerInterrupt

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_TriggerInterrupt( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_TRIGGER, pCtxt->hULog, 2, 
                      "PdcGpio_TriggerInterrupt:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_TRIGGER, pCtxt->hULog, 2, 
                  "PdcGpio_TriggerInterrupt: Triggering PDC GPIO %d with interrupt ID %d", 
                  nGpio, 
                  pCtxt->pStateTable[nIdx].nQurtIntrID);

   /* We don't need to trigger both edges for dual edge */
   return DalInterruptController_InterruptTrigger(pCtxt->hIntrCntrlHandle,
                                                  pCtxt->pStateTable[nIdx].nQurtIntrID);
} /* END PdcGpio_TriggerInterrupt */

/*==========================================================================

  FUNCTION      PdcGpio_ClearInterrupt

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_ClearInterrupt( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_CLEAR, pCtxt->hULog, 2, 
                      "PdcGpio_ClearInterrupt:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_CLEAR, pCtxt->hULog, 2, 
                  "PdcGpio_ClearInterrupt: Clearing PDC GPIO %d with interrupt ID %d", 
                  nGpio, pCtxt->pStateTable[nIdx].nQurtIntrID);

   eResult = DalInterruptController_InterruptClear(pCtxt->hIntrCntrlHandle,
                                                   pCtxt->pStateTable[nIdx].nQurtIntrID);

   return eResult;
} /* END PdcGpio_ClearInterrupt */

/*==========================================================================

  FUNCTION      PdcGpio_IsInterruptSet

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_IsInterruptSet( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DALResult bSet;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_IsInterruptSet:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   bSet =  DalInterruptController_InterruptStatus(pCtxt->hIntrCntrlHandle, 
                                                  pCtxt->pStateTable[nIdx].nQurtIntrID);
   if(bSet == DAL_INTERRUPT_SET)
   {
     *state = 1;
   }
   else
   {
     *state = 0;
   }

   return DAL_SUCCESS;
} /* END PdcGpio_IsInterruptSet */

/*==========================================================================

  FUNCTION      PdcGpio_IsInterruptPending

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_IsInterruptPending( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if(  eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_IsInterruptPending:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   eResult = DalInterruptController_IsInterruptPending(pCtxt->hIntrCntrlHandle, 
                                                       pCtxt->pStateTable[nIdx].nQurtIntrID, 
                                                       state);
   
   return eResult;
} /* END PdcGpio_IsInterruptPending */

/*==========================================================================

  FUNCTION      PdcGpio_IsInterruptEnabled

  DESCRIPTION   See GPIOInt_Pdc.h

==========================================================================*/

DALResult PdcGpio_IsInterruptEnabled( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   PdcGpioCfgCtxtType *pCtxt = &PdcGpio_CfgCtxt;
   uint32 nIdx;

   eResult = PdcGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "PdcGpio_IsInterruptEnabled:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   return DalInterruptController_IsInterruptEnabled(pCtxt->hIntrCntrlHandle, 
                                                    pCtxt->pStateTable[nIdx].nQurtIntrID, 
                                                    state);
} /* END PdcGpio_IsInterruptEnabled */





