/*==============================================================================

FILE:      GPIOInt_DirConn.c

DESCRIPTION
  This modules implements the API to utilize GPIO interrupt controller for a
  specific type of GPIOInt - Direct Connect Interrupts.
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

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt_DirConn.c#1 $

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
#include "DDIGPIOMgr.h"
#include "qurt.h"
#include "ULog.h"
#include "ULogFront.h"

#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "GPIOInt_DirConn.h"
#include "DirConnGpioIntInterface.h"
#include "HALtlmmtile.h"

#ifdef GPIOINT_USE_MPM
#include "DDImpm.h"
#endif /*GPIOINT_USE_MPM */

/*=========================================================================
      Macros
==========================================================================*/


/*=========================================================================
      Data Types
==========================================================================*/

/* 
 * Func Pointer Table for DirConn Gpio interrupts
 */
GPIOIntFnDescType DirConnGpio_Procs = {
  DirConnGpio_Init,
  DirConnGpio_DeInit,
  DirConnGpio_IsSupported,
  DirConnGpio_Config,
  DirConnGpio_Deconfig,
  DirConnGpio_Register,
  DirConnGpio_Deregister,
  DirConnGpio_Enable,
  DirConnGpio_Disable,
  DirConnGpio_SetTrigger,
  DirConnGpio_TriggerInterrupt,
  DirConnGpio_ClearInterrupt,
  DirConnGpio_IsInterruptSet,
  DirConnGpio_IsInterruptPending,
  DirConnGpio_IsInterruptEnabled,
  DirConnGpio_DecrementNonMPMInterrupts
};

const uint32 nDirConnTriggerMap[GPIOINT_TRIGGER_INVALID] = 
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

static DirConnGpioCfgCtxtType DirConnGpio_CfgCtxt = {0};

static DirConnGpioIntConfigMapType* DirConnGpioIntConfigMap;

uint32 DirConnGpioInternal_GetDirConnIntrTrigger( GPIOIntTriggerType eTrigger );

DALResult DirConnGpioInternal_FindIdxByGpio( uint32 nGpio, uint32 *nIdx );

DALResult DirConnGpioInternal_GetDirConnGpio( uint32 nGpio, uint32* pDirConnID );

DALResult DirConnGpioInternal_ReleaseDirConnGpio( uint32 nGpio, uint32* pDirConnID );

void DirConnGpioInternal_LogInit( ULogHandle* hL );

void DirConnGpioInternal_SetDirConnIntr( uint32 nGpio);

void DirConnGpioInternal_SetDirConnIntrPolarity( uint32 nGpio, 
												 GPIOIntTriggerType eTrigger );

void DirConnGpioInternal_DisableDirConnIntr( uint32 nGpio );

extern void DirConnGpio_Init_NPA( void *context, 
                                  unsigned int event_type, 
                                  void *resource_name, 
                                  unsigned int resource_strlen );

/*=========================================================================
      Internal Helper Functions
==========================================================================*/

/*==========================================================================
   FUNCTION      DirConnGpioInternal_AttachToDals
==========================================================================*/

DALResult 
DirConnGpioInternal_AttachToDals
(
  DirConnGpioCfgCtxtType *pCtxt
)
{
  /* Attach to the Main Interrupt Controller DAL */
  if(DAL_SUCCESS != DAL_DeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, 
                                     &pCtxt->hIntrCntrlHandle))
  {
      return DAL_ERROR;
  }

  /* Acquire GPIO Manager */
  if(DAL_SUCCESS != DalGPIOMgr_Attach(&pCtxt->hGPIOMgrHandle))
  {
    return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END DirConnGpioInternal_AttachToDals */

/*==========================================================================
   FUNCTION      DirConnGpioInternal_DetachToDals
==========================================================================*/

DALResult 
DirConnGpioInternal_DetachToDals
(
  DirConnGpioCfgCtxtType *pCtxt
)
{
  /* Detach from the Main Interrupt Controller DAL */
  if(DAL_SUCCESS != DAL_DeviceDetach(pCtxt->hIntrCntrlHandle))
  {
      return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END DirConnGpioInternal_DetachToDals */


/*==========================================================================
   FUNCTION      DirConnGpioInternal_GetTargetData
==========================================================================*/

DALResult 
DirConnGpioInternal_GetTargetData
(
  DirConnGpioCfgCtxtType *pCtxt
)
{
  DALResult eResult = DAL_ERROR;
  char* XOShutdownResource;

  /* 
   * declare property handle variable.
   */
  DALSYSPropertyVar propertyVar;
  DALSYS_PROPERTY_HANDLE_DECLARE(hPropertyHandle);

  /* Get the property handle */
  DALSYS_GetDALPropertyHandleStr("DirConnGpioInt", hPropertyHandle);

  /* determine number of supported Dir Conn Gpio Interrupts */ 
  eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                   "NUMBER_OF_DIRECT_CONNECT_INTERRUPTS", 0, 
                                   &propertyVar); 
  if(eResult == DAL_SUCCESS)
  {
      pCtxt->nTotalGpioCount = propertyVar.Val.dwVal;
  }
  else
  {
      pCtxt->nTotalGpioCount = 0;
  }    

  /* Get pointer to Dir Conn Gpio Interrupt Config Map */ 
  if(DirConnGpio_CfgCtxt.nTotalGpioCount)
  {
      eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                        "DIRECT_CONNECT_CONFIG_MAP", 0, 
                                        &propertyVar); 

      if(eResult == DAL_SUCCESS)
      {
          DirConnGpioIntConfigMap = 
              (DirConnGpioIntConfigMapType *)propertyVar.Val.pStruct;
      }
      else
      {
          GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                         "DirConnGpio_GetTargetData:FAIL. Direct Conn Interrupt Config Map is corrupted");
          return DAL_ERROR;
      }
  }

  eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                    "XO_SHUTDOWN_RSRC", 0, 
                                    &propertyVar); 
  if(eResult == DAL_SUCCESS)
  {
      XOShutdownResource = (char *)propertyVar.Val.pszVal;

      /*
      * Communicate to PDC for power-saving features implemented. 
      */ 
      #ifdef DIRCONNGPIO_USE_NPA
      npa_resource_available_cb(XOShutdownResource, DirConnGpio_Init_NPA, pCtxt);
      #endif
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "DirConnGpio_GetTargetData:FAIL. NPA Resource target data is corrupt");
      return DAL_ERROR;
  }

  return eResult;

} /* END DirConnGpioInternal_GetTargetData */


/*==========================================================================
   FUNCTION      DirConnGpioInternal_GetDirConnIntrTrigger
==========================================================================*/

uint32 DirConnGpioInternal_GetDirConnIntrTrigger( GPIOIntTriggerType eTrigger )
{
  if ( eTrigger >= GPIOINT_TRIGGER_INVALID )
  {
    return DALINTRCTRL_ENABLE_LEVEL_HIGH_TRIGGER;
  }

  return nDirConnTriggerMap[eTrigger];
} /* END DirConnGpioInternal_GetDirConnIntrTrigger */


/*==========================================================================
   FUNCTION      DirConnGpioInternal_FindIdxByGpio
==========================================================================*/
/**
  Gets the Index of the entry in the State Table for the GPIO given.

  @param[in]  nGpio       The GPIO pin number for the direct connect. 
  @param[out] nIdx        Index of entry in Dir Conn State Table assigned to GPIO. 
  @return
  DAL_SUCCESS -- is returned if entry is found in State Table for the GPIO given.
  DAL_ERROR -- is returned if there is no entry for the GPIO given. 

*/

DALResult DirConnGpioInternal_FindIdxByGpio(uint32 nGpio, uint32 *nIdx)
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
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

} /* END DirConnGpioInternal_FindIdxByGpio */


/* =========================================================================
**  Function : DirConnGpioInternal_GetDirConnGpio
** =========================================================================*/
/**
  Gets a free direct connect GPIO for the GPIO given.

  @param[in]  nGpio       The GPIO pin number to attach to the direct connect. 
  @param[out] pDirConnID  The direct connect index assigned to the GPIO. 
  @return
  DAL_SUCCESS -- is returned if a direct connect was assigned. 
  DAL_ERROR -- is returned if there are no more direct connects. 

*/

DALResult DirConnGpioInternal_GetDirConnGpio(uint32 nGpio, uint32* pDirConnID)
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIter;

   for ( nIter = 0; nIter < pCtxt->nTotalGpioCount; nIter++ )
   {
           if ( pCtxt->pStateTable[nIter].nGpio == GPIOINT_NONE )
       {
           *pDirConnID = pCtxt->pStateTable[nIter].eIntrType;

           /* Assign the GPIO in the local state table */
           pCtxt->pStateTable[nIter].nGpio = nGpio;
           eResult = DAL_SUCCESS;
           break;
       }
   }
   return eResult;

} /* END DirConnGpioInternal_GetDirConnGpio */

/* =========================================================================
**  Function : DirConnGpioInternal_ReleaseDirectConnectGPIO
** =========================================================================*/
/**
  Releases a direct connect GPIO for the GPIO given.  This GPIO will now
  be added back to the free direct connect pool.

  @param[in] nGpio      The GPIO currently assigned to the direct connect. 
  @param[in] nDirConnID The direct connect ID to be released. 
  @return
  DAL_SUCCESS -- is returned if GPIO successfully released from direct connect. 
  DAL_ERROR -- is returned if the GPIO was never in use by direct connect. 
  @dependencies
  Direct connect must be already assigned to this GPIO.
*/

DALResult DirConnGpioInternal_ReleaseDirConnGpio(uint32 nGpio, uint32* pDirConnID)
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIter;

   for ( nIter = 0; nIter < pCtxt->nTotalGpioCount; nIter++ )
   {
       if ( (pCtxt->pStateTable[nIter].nGpio == nGpio) && 
            (pCtxt->pStateTable[nIter].eIntrType == *pDirConnID) )
       {
           /* Release the GPIO in the local state table */
           pCtxt->pStateTable[nIter].nGpio = GPIOINT_NONE;
           eResult = DAL_SUCCESS;
           break;
       }
   }
   return eResult;

} /* END DirConnGpioInternal_ReleaseDirConnGpio */

/* =========================================================================
**  Function : DirConnGpioInternal_LogInit
** =========================================================================*/
/**
  Initialize the uLog required for DirConnGpioInt.

  @return  None
*/

void DirConnGpioInternal_LogInit(ULogHandle* hL)
{
   char sztGPIOIntHeader[] = "DirConnGpioInt Log";

   ULogFront_RealTimeInit(hL,
                          "DirConnGpioInt",
                          4096,
                          ULOG_MEMORY_LOCAL,
                          ULOG_LOCK_OS);

   if ( hL != NULL )
   {
     ULogCore_HeaderSet(*hL, sztGPIOIntHeader);
   }
} /* END DirConnGpioInternal_LogInit */

/* =========================================================================
**  Function : DirConnGpioInternal_SetDirConnIntr
** =========================================================================*/
void DirConnGpioInternal_SetDirConnIntr
( 
  uint32 nGpio
)
{
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  uint32 nIdx;
  DALResult eResult;
  
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  
  /* 
   * We need to Set the Direct connect CFG register Gpio select bits
   */

  HAL_tlmmtile_SetDirConn ( pCtxt->pStateTable[nIdx].nDirConnID, HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_SEL, nGpio, pCtxt->pStateTable[nIdx].nDirectConnectBaseType);
  
  /* 
   * We need to enable the direct connect 
   * interrupt from the Summary CFG register 
   */ 
  
  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_DIRCONNEN, 1 );

} /* END DirConnGpioInternal_SetDirConnIntr */


/* =========================================================================
**  Function : DirConnGpioInternal_SetDirConnIntrPolarity
** =========================================================================*/
void DirConnGpioInternal_SetDirConnIntrPolarity
( 
  uint32 nGpio,
  GPIOIntTriggerType eTrigger 
)
{
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  uint32 nIdx, nVal;
  DALResult eResult;
  
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);

  if(GPIOINT_IS_DIRCONN(pCtxt->pStateTable[nIdx].nDirConnID))
  {
    /*
     * Configure the polarity
     */
    if ( eTrigger == GPIOINT_TRIGGER_HIGH ||
         eTrigger == GPIOINT_TRIGGER_RISING )
    {
      nVal = 1;
    }
    else
    {
      nVal = 0;
    }

    HAL_tlmmtile_SetDirConn ( pCtxt->pStateTable[nIdx].nDirConnID, HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_POL, nVal , pCtxt->pStateTable[nIdx].nDirectConnectBaseType);
  }

} /* END DirConnGpioInternal_SetDirConnIntrPolarity */


/* =========================================================================
**  Function : DirConnGpioInternal_DisableDirConnIntr
** =========================================================================*/
void DirConnGpioInternal_DisableDirConnIntr
(
  uint32 nGpio
)
{ 
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  uint32 nIdx;
  
  DALResult eResult;
  
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  
  if(GPIOINT_IS_DIRCONN(pCtxt->pStateTable[nIdx].nDirConnID))
  {
    /* 
     * We need to clear the Direct connect register Gpio select bits
     */
    HAL_tlmmtile_SetDirConn ( pCtxt->pStateTable[nIdx].nDirConnID,  HAL_TLMMTILE_FIELD_DIRCONNINTRCFG_SEL, 0 , pCtxt->pStateTable[nIdx].nDirectConnectBaseType);
  }

  /* 
   * We need to clear the direct connect 
   * interrupt from the Summary register 
   */ 
  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_DIRCONNEN, 0 );

  /* 
   * We need to disable the GPIO interrupt
   */ 
  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_EN, 0 );

} /* END DirConnGpioInternal_DisableDirConnIntr */

/*=============================================================================
  FUNCTION      DirConnGpioInternal_ReconfigureDirConnGpio
=============================================================================*/
static
void DirConnGpioInternal_ReconfigureDirConnGpio(uint32 nGpio,GPIOIntTriggerType eTrigger)
{ 
  DALResult eResult = DAL_ERROR;
  uint32 nIdx;
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  
  /* Determine Direct Connect Index assigned to nGpio */
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  if( DAL_SUCCESS != eResult )
  {
    GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                   "DirConnGpioInternal_ReconfigureDirConnGpio:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                   nGpio);
  }
  
  /* Program nGpio DirConn params in HW */
  DirConnGpioInternal_SetDirConnIntrPolarity(nGpio, eTrigger);
  pCtxt->pStateTable[nIdx].eIntrTrigger = eTrigger;
  
}

/*=============================================================================
  FUNCTION      DirConnGpioInternal_ReconfigureDirConnInterrupt
=============================================================================*/
void DirConnGpioInternal_ReconfigureDualEdgeDirConnInterrupt(uint32 nGpio)
{
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  uint32 nInVal1 = 0, nInVal2 = (uint32)-1,nIdx;
  DALResult eResult;
  
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  if( DAL_SUCCESS != eResult )
  {
    GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                   "DirConnGpioInternal_ReconfigureDualEdgeDirConnInterrupt:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                    nGpio);
  }
 
  if ( pCtxt->pStateTable[nIdx].eIntrTrigger == GPIOINT_TRIGGER_DUAL_EDGE )
  {
    while ( nInVal1 != nInVal2 )
    {
      
      nInVal1 = HAL_tlmmtile_Get(nGpio, HAL_TLMMTILE_REG_INOUT, HAL_TLMMTILE_FIELD_INOUT_IN);

      if ( nInVal1 == 0 )
      {
        /* Input is low , configure rising edge */
        DirConnGpioInternal_ReconfigureDirConnGpio(nGpio, GPIOINT_TRIGGER_RISING);
      }
      else 
      {
        /* Input is high , configure falling edge */ 
        DirConnGpioInternal_ReconfigureDirConnGpio(nGpio, GPIOINT_TRIGGER_FALLING);
      }

      
      nInVal2 = HAL_tlmmtile_Get(nGpio, HAL_TLMMTILE_REG_INOUT, HAL_TLMMTILE_FIELD_INOUT_IN);
    }
  }
}

/*=============================================================================
  FUNCTION      DirConnGpioInternal_DualEdgeIsr
=============================================================================*/
void DirConnGpioInternal_DualEdgeIsr(void *param)
{
  GPIOINTISR pfnIsr;
  GPIOINTISRCtx nParam;
  uint32 nGpio = (uint32)param ,nIdx;
  DALResult eResult;
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  
  (void)qurt_rmutex_lock(pCtxt->pLock);
   
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  if( DAL_SUCCESS != eResult )
  {
    GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                    "DirConnGpio_DualEdgeIsr:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                    nGpio);
  }
   
  DirConnGpioInternal_ReconfigureDualEdgeDirConnInterrupt(nGpio);
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

  FUNCTION      DirConnGpio_Init

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Init( GPIOIntr_InitDataType *GPIOIntr_InitData )
{

   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   /*
    * Setup uLog buffer 
    */ 
   DirConnGpioInternal_LogInit(&pCtxt->hULog);
   if(pCtxt->hULog == NULL)
   {
       return DAL_ERROR;
   }

   if(!pCtxt->bIsInitialized)
   {
        /*
         * Attachs to the DALs which DirConn GpioInt DeviceType depends on.
         */
        if(DAL_SUCCESS != DirConnGpioInternal_AttachToDals(pCtxt))
        {
            return DAL_ERROR;
        }

        /*
         * Get Interrupt Config Data from DirConn DAL Props
         */
        if(DAL_SUCCESS != DirConnGpioInternal_GetTargetData(pCtxt))
        {
            return DAL_ERROR;
        }

        /*
         * Allocate memory for DirConn GPIOInt State Table
         */
        if(DAL_SUCCESS != DALSYS_Malloc((pCtxt->nTotalGpioCount * sizeof(DirConnGpioCfgDataType)),
                                        (void **)&(pCtxt->pStateTable)))
        {
            return DAL_ERROR;
        }

        /*
         * Map the correct Func Pointer Table for the DeviceType
         */
        pCtxt->pVtbl = &DirConnGpio_Procs;

        /* 
         * Setup local State Table based on DirConn Config Map
         */
        for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
        {
           pCtxt->pStateTable[nIdx].nGpio = DirConnGpioIntConfigMap[nIdx].gpio;
           pCtxt->pStateTable[nIdx].eIntrType = DirConnGpioIntConfigMap[nIdx].interrupt;
           pCtxt->pStateTable[nIdx].nQurtIntrID = DirConnGpioIntConfigMap[nIdx].interrupt_id;
           pCtxt->pStateTable[nIdx].nUInterrupt = 0;
           pCtxt->pStateTable[nIdx].nPriority = GPIOINT_DEFAULT_PRIORITY;

           if( DirConnGpioIntConfigMap[nIdx].flags & GPIOINTF_EXTERNAL_CONFIG ) 
           {
               pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_EXTERNAL_CONFIG;
           }
           if( DirConnGpioIntConfigMap[nIdx].flags & GPIOINTF_DIRECTINT_CONFIG ) 
           {
               pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_DIRECTINT_CONFIG;
           }
        }

        /*
         * Initialize HAL_tlmmtile  
         */
        HAL_tlmmtile_Init();
       
	/*
         * Store the lock passed in the init 
         */
        pCtxt->pLock = GPIOIntr_InitData->pLock;

        #ifdef GPIOINT_USE_MPM
        /*
         * Store the MPM devicehandle passed in the init 
         */
        pCtxt->hDalMpmHandle = GPIOIntr_InitData->hDalMpmHandle;
        #endif

	/*
         * Flag the mini-driver as initialised. 
         */ 
        pCtxt->bIsInitialized = 1;

        eResult = DAL_SUCCESS;
   }

   return eResult;
} /* END DirConnGpio_Init */

/*==========================================================================

  FUNCTION      DirConnGpio_DeInit

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_DeInit( void )
{
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   /* 
    * Deregister all ISRs for registered GPIO interrupts
    */
   for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
   {
       if( (pCtxt->pStateTable[nIdx].nGpio != GPIOINT_NONE) &&
           (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED) )
       {
           if(DAL_SUCCESS != DirConnGpio_Deregister(pCtxt->pStateTable[nIdx].nGpio))
           {
               /* Do Nothing, rather move on with rest of the driver tear-down */
           }
       }
   }

   /* Detach for attached DAL drivers */
   DirConnGpioInternal_DetachToDals(pCtxt);

   /* Free up any memory allocated */
   DALSYS_Free(&(pCtxt->pStateTable));

   /*
	* Flag the mini-driver as uninitialised. 
	*/ 
   pCtxt->hULog = NULL;
   pCtxt->bIsInitialized = 0;

   /* Free up any resource obtained */

   return DAL_SUCCESS;
}/* END DirConnGpio_DeInit */

/*==========================================================================

  FUNCTION      DirConnGpio_IsSupported

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr )
{
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;
   uint32 nDirConnID, nDirectConnectBaseType;

   /* Verify attribute */
   if ( pAttr == NULL )
   {
     return DAL_ERROR;
   }

   /* DirConn Gpio Interrupts do not support Dual-Edge Trigger Type 
   if( pAttr->eTrigger == GPIOINT_TRIGGER_DUAL_EDGE ) 
   {
     return DAL_ERROR;
   }*/

   for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
   {
     /* Check if new GPIO is already assigned in DirConn config map */
     if( pCtxt->pStateTable[nIdx].nGpio == nGpio && pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG)
     {
       return DAL_SUCCESS; 
     }
   }
   
   for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
   {
     /* Check if new GPIO is already assigned in DirConn config map */
     if( pCtxt->pStateTable[nIdx].nGpio == GPIOINT_NONE ||
         pCtxt->pStateTable[nIdx].nGpio == nGpio )
     {
       break; 
     }
   }

   /* Check if DirConn GPIO is available for this PD in DirConn config map within GpioMgr */
   if( DAL_SUCCESS == DalGPIOMgr_GetDirectConnectGPIO(pCtxt->hGPIOMgrHandle, nGpio, &nDirConnID, &nDirectConnectBaseType) )
   {
     pCtxt->pStateTable[nIdx].nGpio = nGpio;
     pCtxt->pStateTable[nIdx].nDirConnID = nDirConnID;
     pCtxt->pStateTable[nIdx].nDirectConnectBaseType = nDirectConnectBaseType;
     pCtxt->pStateTable[nIdx].eIntrType = (GPIOIntInterruptType)nDirConnID;
     return DAL_SUCCESS;
   }
   else
   {
     GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                    "DirConnGpio_IsSupported: No free Direct Connect Index found for GPIO %d", 
                     nGpio);
   }

   return DAL_ERROR;
} /* END DirConnGpio_IsSupported */

/*==========================================================================

  FUNCTION      DirConnGpio_Config

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Config( uint32 nGpio, 
                              GpioIntr_Attribute *pAttr, 
                              GpioIntr_CfgType *pIntrCfgData )
{
   DALResult eResult = DAL_ERROR;
   uint32 nIdx;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;

   /* Verify attribute */
   if ( pAttr == NULL )
   {
     GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                    "DirConnGpio_Config:FAIL. pAttr == NULL, Failed for GPIO %d", 
                     nGpio);
     return DAL_ERROR;
   }
   
   /* Determine Direct Connect Index assigned to nGpio */
   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Config:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   if(  pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG)
   {
     pIntrCfgData->nInterruptID =  pCtxt->pStateTable[nIdx].nQurtIntrID;
     pCtxt->pStateTable[nIdx].eIntrTrigger = pAttr->eTrigger;
     pCtxt->pStateTable[nIdx].nUInterrupt  = pAttr->nUInterrupt;
     pCtxt->pStateTable[nIdx].nPriority    = pAttr->nPriority;
     pCtxt->pStateTable[nIdx].mpm_interrupt_id = pAttr->mpm_intr_id;
     pCtxt->pStateTable[nIdx].nTypeFlags |= pAttr->nflags;
     pIntrCfgData->nInterruptType  = GPIOINT_SPECIALCONN;
     goto Done;
   }
   /* Determine a Free Direct Connect Index to assign to nGpio */
   //eResult = DirConnGpioInternal_GetDirConnGpio(nGpio, &(pIntrCfgData->nInterruptType));

   /* Check GPIO configurations in the local state table */
   /* 
    * This check is not valid as LPADSP + Sensors combination of direct connect bases are used
    */
   /*
   if( (uint32)(pCtxt->pStateTable[nIdx].eIntrType) != nIdx )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                      "DirConnGpio_Config:FAIL. Direct Connect Index Mismatch for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }
   */
   
   /* Update config data in GPIOInt State Table */
   pIntrCfgData->nInterruptType = pCtxt->pStateTable[nIdx].nDirConnID;
   
   eResult = DalGPIOMgr_GetDirectConnectInterruptID(pCtxt->hGPIOMgrHandle, nGpio, &(pIntrCfgData->nInterruptID));
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Config:FAIL. No free Direct Connect InterruptID found for GPIO %d with Trigger %d", 
                       nGpio, (uint32)pAttr->eTrigger);
       return DAL_ERROR;
   }

   /* Map the GPIOInt trigger type to trigger type for Main DAL Interrupt Controller */
   pCtxt->pStateTable[nIdx].eIntrTrigger = pAttr->eTrigger;
   pCtxt->pStateTable[nIdx].nUInterrupt  = pAttr->nUInterrupt;
   pCtxt->pStateTable[nIdx].nPriority    = pAttr->nPriority;
   pCtxt->pStateTable[nIdx].mpm_interrupt_id = pAttr->mpm_intr_id;
   pCtxt->pStateTable[nIdx].nTypeFlags |= pAttr->nflags;
   
   /* Program nGpio DirConn params in HW */
   DirConnGpioInternal_SetDirConnIntr(nGpio);
   DirConnGpioInternal_SetDirConnIntrPolarity(nGpio, pAttr->eTrigger);

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 4, 
                  "DirConnGpio_Config:SUCCESS. GPIO %d with Trigger %d configured for Direct Connect Index %d and Interrupt Number %d", 
                  nGpio, (uint32)pAttr->eTrigger, pIntrCfgData->nInterruptType, pIntrCfgData->nInterruptID);
  Done:
  #ifdef GPIOINT_USE_MPM
  if(pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_MPMINT)
  {
    pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_WAKEUP;
    GPIOIntr_ConfigMPMWakeup(pCtxt->hDalMpmHandle, nGpio, TRUE);
  }   
  else
  {
    #ifdef DIRCONNGPIO_USE_NPA
    pCtxt->non_mpm_interrupts++;
    DirConnGpio_UpdateNPARequest(pCtxt);
    #endif /*DIRCONNGPIO_USE_NPA */
  }
  #endif /*GPIOINT_USE_MPM */
   return eResult;
}/* END DirConnGpio_Config */

/*==========================================================================

  FUNCTION      DirConnGpio_Deconfig

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Deconfig( uint32 nGpio, 
                                GpioIntr_CfgType *pIntrCfgData)
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   /* Release the Direct Connect Index assigned to this nGpio */
   //eResult = DirConnGpioInternal_ReleaseDirConnGpio (nGpio, &(pIntrCfgData->nInterruptType));

   /* Release the Direct Connect Index assigned to this nGpio */
   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Deconfig:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

  /* Clear direct connect mapping from Direct Connect select field */
   if( (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG) == 0)
   {
     DirConnGpioInternal_DisableDirConnIntr(nGpio);
   }
   /* Release the Direct Connect Index assigned to this nGpio */
   pCtxt->pStateTable[nIdx].nGpio = GPIOINT_NONE;

   if( (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG) == 0)
   {
   /* Release DirConn GPIO within GpioMgr */
     eResult = DalGPIOMgr_ReleaseDirectConnectGPIO(pCtxt->hGPIOMgrHandle, nGpio, pIntrCfgData->nInterruptID);
   }
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Deconfig:FAIL. Failed to release GPIO %d with Direct Connect Index %d", 
                       nGpio, pIntrCfgData->nInterruptType);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 3, 
                  "DirConnGpio_Deconfig:SUCCESS. GPIO %d deconfigured for Direct Connect Index %d and Interrupt Number %d", 
                  nGpio, pIntrCfgData->nInterruptType, pIntrCfgData->nInterruptID);

   /* default DirConn config data in State Table for nGpio */
   pIntrCfgData->nInterruptID    = GPIOINT_NO_QURT_ID;
   pIntrCfgData->nInterruptType  = GPIOINT_DEFAULT;
   
   #ifdef GPIOINT_USE_MPM
  if(pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_MPMINT)
  {
    pCtxt->pStateTable[nIdx].nTypeFlags &= ~GPIOINTF_WAKEUP;
    Dalmpm_DisablePin(pCtxt->hDalMpmHandle,nGpio);
  }   
  else
  {
    #ifdef DIRCONNGPIO_USE_NPA
    pCtxt->non_mpm_interrupts--;
    DirConnGpio_UpdateNPARequest(pCtxt);
    #endif /*DIRCONNGPIO_USE_NPA */
  }
  #endif /*GPIOINT_USE_MPM */
  
   return eResult;
}/* END DirConnGpio_Deconfig */

/*==========================================================================

  FUNCTION      DirConnGpio_Register

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Register( uint32 nGpio, void *pfnIsr, void *pIsrParam )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Register:FAIL. FindIdxByGpio() Failed for GPIO %d", nGpio);
       return DAL_ERROR;
   }

   /* Register ISR for DirConn Gpio Interrupt with Main DAL Interrupt Controller */
   /* Mapping accounts for Dual edge registering for rising edge first */
  

   if (pCtxt->pStateTable[nIdx].eIntrTrigger == GPIOINT_TRIGGER_DUAL_EDGE)
   {
     DirConnGpioInternal_ReconfigureDualEdgeDirConnInterrupt(nGpio);
     /* Register ISR for DirConn Gpio Interrupt with Main DAL Interrupt Controller */
     if( DAL_SUCCESS != DalInterruptController_RegisterISR(pCtxt->hIntrCntrlHandle, 
                                                            pCtxt->pStateTable[nIdx].nQurtIntrID,
                                                           (DALISR)DirConnGpioInternal_DualEdgeIsr,
                                                           (DALISRCtx)nGpio,
                                                           DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER))
     {
         GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                        "DirConnGpio_Register:FAIL. Failed for GPIO %d", 
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
                                                            DirConnGpioInternal_GetDirConnIntrTrigger(pCtxt->pStateTable[nIdx].eIntrTrigger),
                                                            pCtxt->pStateTable[nIdx].nPriority,
                                                            pCtxt->pStateTable[nIdx].nUInterrupt)))
     {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Register:FAIL. Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
     }
    
   }
   pCtxt->pStateTable[nIdx].isr         = pfnIsr;
   pCtxt->pStateTable[nIdx].isr_param   = (GPIOINTISRCtx)pIsrParam;
   pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_REGISTERED;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "DirConnGpio_Register:SUCCESS. GPIO %d registered", nGpio);

   return eResult;
}/* END DirConnGpio_Register */

/*==========================================================================

  FUNCTION      DirConnGpio_Deregister

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Deregister( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Deregister:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   /* Main DAL interrupt controller handles the Direct Connect Interrupts */
   eResult = DalInterruptController_Unregister(pCtxt->hIntrCntrlHandle,
                                               pCtxt->pStateTable[nIdx].nQurtIntrID);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_Deregister:FAIL. Failed for GPIO %d", nGpio);
       return DAL_ERROR;
   }
   pCtxt->pStateTable[nGpio].isr         = NULL;
   pCtxt->pStateTable[nGpio].isr_param   = NULL;
   pCtxt->pStateTable[nIdx].nTypeFlags &= ~GPIOINTF_REGISTERED;
   pCtxt->pStateTable[nIdx].eIntrTrigger = GPIOINT_TRIGGER_HIGH;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "DirConnGpio_Deregister:SUCCESS. GPIO %d deregistered", nGpio);

   return eResult;
}/* END DirConnGpio_Deregister */

/*==========================================================================

  FUNCTION      DirConnGpio_Enable

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Enable( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                      "DirConnGpio_Enable:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "DirConnGpio_Enable: Enabling Direct Connect GPIO %d", nGpio );
   
   if( (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG) == 0)
   {
     DirConnGpioInternal_SetDirConnIntr(nGpio);// *************IMPORTANT*****************
   }

   return DalInterruptController_InterruptEnable(pCtxt->hIntrCntrlHandle,
                                                 pCtxt->pStateTable[nIdx].nQurtIntrID);

}/* END DirConnGpio_Enable */

/*==========================================================================

  FUNCTION      DirConnGpio_Disable

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_Disable( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 2, 
                      "DirConnGpio_Disable:FAIL. FindIdxByGpio() Failed for GPIO %d", nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "DirConnGpio_Disable: Disabling Direct Connect GPIO %d", nGpio);
   if( (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG) == 0)
   {
     DirConnGpioInternal_DisableDirConnIntr(nGpio);// *************IMPORTANT*****************
   }
   
   return DalInterruptController_InterruptDisable(pCtxt->hIntrCntrlHandle,
                                                  pCtxt->pStateTable[nIdx].nQurtIntrID);

}/* END DirConnGpio_Disable */

/*==========================================================================

  FUNCTION      DirConnGpio_SetTrigger

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 1, 
                      "DirConnGpio_SetTrigger:FAIL. FindIdxByGpio() Failed for GPIO %d", nGpio);
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
   if( (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_DIRECTINT_CONFIG) == 0)
   {
   DirConnGpioInternal_SetDirConnIntrPolarity(nGpio, 
                                                (GPIOIntTriggerType)nNewTrigger); // *************IMPORTANT**************
   }

   eResult = DalInterruptController_SetTrigger(pCtxt->hIntrCntrlHandle, 
                                               pCtxt->pStateTable[nIdx].nQurtIntrID,
                                               DirConnGpioInternal_GetDirConnIntrTrigger(pCtxt->pStateTable[nIdx].eIntrTrigger));

   DalInterruptController_InterruptClear(pCtxt->hIntrCntrlHandle,
                                         pCtxt->pStateTable[nIdx].nQurtIntrID);

   DalInterruptController_InterruptEnable(pCtxt->hIntrCntrlHandle, 
                                          pCtxt->pStateTable[nIdx].nQurtIntrID);

   if ( DAL_SUCCESS != eResult )
   {
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 2, 
                  "DirConnGpio_SetTrigger: GPIO %d Set with new direct connect Trigger %d", 
                  nGpio, nNewTrigger);
   
   #ifdef GPIOINT_USE_MPM
   if(pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_MPMINT)
   {
     pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_WAKEUP;
     GPIOIntr_ConfigMPMWakeup(pCtxt->hDalMpmHandle,nGpio,TRUE);
   }   
   else
   {
     #ifdef DIRCONNGPIO_USE_NPA
     pCtxt->non_mpm_interrupts++;
     DirConnGpio_UpdateNPARequest(pCtxt);
     #endif /*DIRCONNGPIO_USE_NPA */
   }
   #endif /*GPIOINT_USE_MPM */
  
   return DAL_SUCCESS;
}/* END DirConnGpio_SetTrigger */

/*==========================================================================

  FUNCTION      DirConnGpio_TriggerInterrupt

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_TriggerInterrupt( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_TRIGGER, pCtxt->hULog, 2, 
                      "DirConnGpio_TriggerInterrupt:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_TRIGGER, pCtxt->hULog, 2, 
                  "DirConnGpio_TriggerInterrupt: Triggering Dir Conn GPIO %d with interrupt ID %d", 
                  nGpio, pCtxt->pStateTable[nIdx].nQurtIntrID);

   return DalInterruptController_InterruptTrigger(pCtxt->hIntrCntrlHandle,
                                                  pCtxt->pStateTable[nIdx].nQurtIntrID);
}/* END DirConnGpio_TriggerInterrupt */

/*==========================================================================

  FUNCTION      DirConnGpio_ClearInterrupt

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_ClearInterrupt( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_CLEAR, pCtxt->hULog, 2, 
                      "DirConnGpio_ClearInterrupt:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   GPIOIntLog_Out(GPIOINTLOG_CLEAR, pCtxt->hULog, 2, 
                  "DirConnGpio_ClearInterrupt: Clearing Dir Conn GPIO %d with interrupt ID %d", 
                  nGpio, pCtxt->pStateTable[nIdx].nQurtIntrID);

   return DalInterruptController_InterruptClear(pCtxt->hIntrCntrlHandle,
                                                pCtxt->pStateTable[nIdx].nQurtIntrID);
}/* END DirConnGpio_ClearInterrupt */

/*==========================================================================

  FUNCTION      DirConnGpio_IsInterruptSet

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_IsInterruptSet( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DALResult bSet;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_IsInterruptSet:FAIL. FindIdxByGpio() Failed for GPIO %d", 
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
}/* END DirConnGpio_IsInterruptSet */

/*==========================================================================

  FUNCTION      DirConnGpio_IsInterruptPending

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_IsInterruptPending( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if(  eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_IsInterruptPending:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   return DalInterruptController_IsInterruptPending(pCtxt->hIntrCntrlHandle, 
                                                    pCtxt->pStateTable[nIdx].nQurtIntrID, 
                                                    state);
}/* END DirConnGpio_IsInterruptPending */

/*==========================================================================

  FUNCTION      DirConnGpio_IsInterruptEnabled

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_IsInterruptEnabled( uint32* state, uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
   uint32 nIdx;

   eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "DirConnGpio_IsInterruptEnabled:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   return DalInterruptController_IsInterruptEnabled(pCtxt->hIntrCntrlHandle, 
                                                    pCtxt->pStateTable[nIdx].nQurtIntrID, 
                                                    state);
} /* END DirConnGpio_IsInterruptEnabled */



/*==========================================================================

  FUNCTION      DirConnGpio_DecrementNonMPMInterrupts

  DESCRIPTION   See GPIOInt_DirConn.h

==========================================================================*/

DALResult DirConnGpio_DecrementNonMPMInterrupts( uint32 nGpio )
{
  DirConnGpioCfgCtxtType *pCtxt = &DirConnGpio_CfgCtxt;
  DALResult eResult = DAL_ERROR;
  uint32 nIdx;
  
  eResult = DirConnGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  
  if( DAL_SUCCESS == eResult )
  {
    if( pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED) 
    {
      #ifdef DIRCONNGPIO_USE_NPA
      pCtxt->non_mpm_interrupts--;
      DirConnGpio_UpdateNPARequest(pCtxt);
      #endif
    }
  }
  return DAL_SUCCESS;
} /* END DirConnGpio_DecrementNonMPMInterrupts */