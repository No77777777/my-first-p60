/*==============================================================================

FILE:      GPIOInt_Summary.c

DESCRIPTION
  This modules implements the API to utilize GPIO interrupt controller for a
  specific type of GPIOInt - Summary Interrupts.
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

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt_Summary.c#1 $

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
#include "DDIGPIOInt.h"
#include "qurt.h"
#include "busywait.h"
#include "DDIGPIOMgr.h"
#include "ULog.h"
#include "ULogFront.h"

#include "GPIOInt.h"
#include "GPIOIntLog.h"
#include "GPIOInt_Summary.h"

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
 * Func Pointer Table for Summary Gpio interrupts
 */
GPIOIntFnDescType SummaryGpio_Procs = {
  SummaryGpio_Init,
  SummaryGpio_DeInit,
  SummaryGpio_IsSupported,
  SummaryGpio_Config,
  SummaryGpio_Deconfig,
  SummaryGpio_Register,
  SummaryGpio_Deregister,
  SummaryGpio_Enable,
  SummaryGpio_Disable,
  SummaryGpio_SetTrigger,
  SummaryGpio_TriggerInterrupt,
  SummaryGpio_ClearInterrupt,
  SummaryGpio_IsInterruptSet,
  SummaryGpio_IsInterruptPending,
  SummaryGpio_IsInterruptEnabled,
  SummaryGpio_DecrementNonMPMInterrupts
};

/*=========================================================================
      Forward references
==========================================================================*/

static SummaryGpioCfgCtxtType SummaryGpio_CfgCtxt = {0};

DALResult SummaryGpioInternal_FindIdxByGpio( uint32 nGpio, uint32 *nIdx );

DALResult SummaryGpioInternal_AssignIdxToGpio( uint32 nGpio, uint32* nIdx );

void SummaryGpioInternal_LogInit( ULogHandle* hL );

void SummaryGpioInternal_SetTargetProcessor (GPIOIntProcessorType eProcId, uint32 nGpio);

void SummaryGpioInternal_GetProcessorMask(GPIOIntProcessorType eProcId, uint32* pMask);

void SummaryGpioInternal_GetProcessorId(uint32 nMask, GPIOIntProcessorType* peProcId);

extern void SummaryGpio_Init_NPA( void *context, 
                                  unsigned int event_type, 
                                  void *resource_name, 
                                  unsigned int resource_strlen );

/*=========================================================================
      Internal Helper Functions
==========================================================================*/

/*==========================================================================
   FUNCTION      SummaryGpioInternal_AttachToDals
==========================================================================*/

DALResult 
SummaryGpioInternal_AttachToDals
(
  SummaryGpioCfgCtxtType *pCtxt
)
{
  /* Attach to the Main Interrupt Controller DAL*/
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

} /* END SummaryGpioInternal_AttachToDals */

/*==========================================================================
   FUNCTION      SummaryGpioInternal_DetachToDals
==========================================================================*/

DALResult 
SummaryGpioInternal_DetachToDals
(
  SummaryGpioCfgCtxtType *pCtxt
)
{
  /* Attach to the Main Interrupt Controller DAL*/
  if(DAL_SUCCESS != DAL_DeviceDetach(pCtxt->hIntrCntrlHandle))
  {
      return DAL_ERROR;
  }

  return DAL_SUCCESS;

} /* END SummaryGpioInternal_DetachToDals */


/*==========================================================================
   FUNCTION      SummaryGpioInternal_GetTargetData
==========================================================================*/

DALResult 
SummaryGpioInternal_GetTargetData
(
  SummaryGpioCfgCtxtType *pCtxt
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
  DALSYS_GetDALPropertyHandleStr("SummaryGpioInt", hPropertyHandle);

  /* determine target processor */ 
  eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                    "PROCESSOR", 0, 
                                    &propertyVar); 
  if(eResult == DAL_SUCCESS)
  {
      pCtxt->eProcessor = 
          (GPIOIntProcessorType)propertyVar.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "SummaryGpio_GetTargetData:FAIL. ProcessorType target data is corrupt");
      return DAL_ERROR;
  }

  /* Get Summary Gpio Interrupt ID */ 
  eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                    "SUMMARY_INTR_ID", 0, 
                                    &propertyVar); 
  if(eResult == DAL_SUCCESS)
  {
      pCtxt->nSummaryIntrID = propertyVar.Val.dwVal;
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "SummaryGpio_GetTargetData:FAIL. summary intr id is corrupt");
      return DAL_ERROR;
  }

  /* determine number of supported Summary Gpio Interrupts */ 
  eResult = DALSYS_GetPropertyValue(hPropertyHandle, 
                                   "NUMBER_OF_SUMMARY_INTERRUPTS", 0, 
                                   &propertyVar); 
  if(eResult == DAL_SUCCESS)
  {
      pCtxt->nTotalGpioCount = propertyVar.Val.dwVal;
  }
  else
  {
      pCtxt->nTotalGpioCount = 0;
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "SummaryGpio_GetTargetData:FAIL. Total Summary Gpio Count is corrupt");
      return DAL_ERROR;
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
      #ifdef SUMMARYGPIO_USE_NPA
      npa_resource_available_cb(XOShutdownResource, SummaryGpio_Init_NPA, pCtxt);
      #endif
  }
  else
  {
      GPIOIntLog_Out(GPIOINTLOG_INIT, pCtxt->hULog, 0, 
                     "SummaryGpio_GetTargetData:FAIL. NPA Resource target data is corrupt");
      return DAL_ERROR;
  }

  return eResult;

} /* END SummaryGpioInternal_GetTargetData */


/*==========================================================================
   FUNCTION      SummaryGpioInternal_FindIdxByGpio
==========================================================================*/
/**
  Gets the Index of the entry in the State Table for the GPIO given.

  @param[in]  nGpio       The GPIO pin number for the Summary. 
  @param[out] nIdx        Index of entry in Summary State Table assigned to GPIO. 
  @return
  DAL_SUCCESS -- is returned if entry is found in State Table for the GPIO given.
  DAL_ERROR -- is returned if there is no entry for the GPIO given. 

*/

DALResult SummaryGpioInternal_FindIdxByGpio(uint32 nGpio, uint32 *nIdx)
{
   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
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

} /* END SummaryGpioInternal_FindIdxByGpio */

/*==========================================================================
   FUNCTION      SummaryGpioInternal_UpdateStateTable
==========================================================================*/

/**
  Assigns a Free Index in the State Table for the GPIO given.

  @param[in]  nGpio   The GPIO pin number to assign in state table.
  @param[out] nIdx    Index of entry in Summary State Table assigned to GPIO.
  @return
  DAL_SUCCESS -- is returned if a free index is found.
  DAL_ERROR -- is returned if there are no more free indices in State Table.

*/

DALResult SummaryGpioInternal_AssignIdxToGpio(uint32 nGpio, uint32* nIdx)
{
   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIter;

   /* Assign Next Free Index in State Table to nGpio */
   for ( nIter = 0; nIter < pCtxt->nTotalGpioCount; nIter++ )
   {
       if ( pCtxt->pStateTable[nIter].nGpio == GPIOINT_NONE ||
            pCtxt->pStateTable[nIter].nGpio == nGpio )
       {
           pCtxt->pStateTable[nIter].nGpio = nGpio;
           *nIdx = nIter;
           eResult = DAL_SUCCESS;
           break;
       }
   }

   return eResult;
} /* END SummaryGpioInternal_AssignIdxToGpio */

/* =========================================================================
**  Function : SummaryGpioInternal_SetTargetProcessor
** =========================================================================*/

void SummaryGpioInternal_SetTargetProcessor 
(
 GPIOIntProcessorType eProcId, 
 uint32 nGpio
)
{  
  /*
   * 0x7 is default value for None
   */
  uint32 nTarget = 0x7;

  SummaryGpioInternal_GetProcessorMask(eProcId, &nTarget);

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_TARGET, nTarget );
  
} /* END SummaryGpioInternal_SetTargetProcessor  */


/* =========================================================================
**  Function : SummaryGpioInternal_GetProcessorMask
** =========================================================================*/

void SummaryGpioInternal_GetProcessorMask
(
 GPIOIntProcessorType eProcId, 
 uint32* pMask
)
{
  switch (eProcId) 
  {
    case GPIOINT_DEVICE_SPS :
      *pMask = 0x0;
      break;
    case GPIOINT_DEVICE_LPA_DSP: 
      *pMask = (0x1);
      break;
    case GPIOINT_DEVICE_RPM: 
      *pMask = (0x2);
      break;
    case GPIOINT_DEVICE_MODEM: 
      *pMask = (0x4);
      break;
    case GPIOINT_DEVICE_APPS: 
      *pMask = (0x5);
      break;
    case GPIOINT_DEVICE_TURING: 
      *pMask = (0x6);
      break;
    default:
     *pMask = (0x7); 
     break;
  }
  return;

} /* SummaryGpioInternal_GetProcessorMask */


/* =========================================================================
**  Function : SummaryGpioInternal_GetProcessorId
** =========================================================================*/

void SummaryGpioInternal_GetProcessorId
(
 uint32 nMask, 
 GPIOIntProcessorType* peProcId
)
{
  switch (nMask) 
  {
    case 0x0 : 
      *peProcId = GPIOINT_DEVICE_SPS;
      break;
    case 0x1 : 
      *peProcId = GPIOINT_DEVICE_LPA_DSP; 
      break;
    case 0x2 : 
      *peProcId = GPIOINT_DEVICE_RPM;
      break;
    case 0x4 : 
      *peProcId = GPIOINT_DEVICE_MODEM;
      break;
    case 0x5 : 
      *peProcId = GPIOINT_DEVICE_APPS;
      break;
    case 0x6 : 
      *peProcId = GPIOINT_DEVICE_TURING;
      break;
    default :
      *peProcId = GPIOINT_DEVICE_NONE; 
      break;
  }
  
  return;

} /* SummaryGpioInternal_GetProcessorId */



/* =========================================================================
**  Function : SummaryGpioInternal_LogInit
** =========================================================================*/
/**
  Initialize the uLog required for SummaryGpioInt.

  @return  None
*/

void SummaryGpioInternal_LogInit(ULogHandle* hL)
{
   char sztGPIOIntHeader[] = "SummaryGpioInt Log";

   ULogFront_RealTimeInit(hL,
                          "SummaryGpioInt",
                          4096,
                          ULOG_MEMORY_LOCAL,
                          ULOG_LOCK_OS);

   if ( hL != NULL )
   {
     ULogCore_HeaderSet(*hL, sztGPIOIntHeader);
   }
} /* END SummaryGpioInternal_LogInit */

/* =========================================================================
**  Function : SummaryGpioInternal_SetTrigger
** =========================================================================*/
void SummaryGpioInternal_SetTrigger
(
  uint32                  nGpio,
  GPIOIntTriggerType      eTrigger
)
{
  uint32 nPolarity = 1;
  uint32 nDetect   = 0;

  switch(eTrigger)
  {
    case GPIOINT_TRIGGER_HIGH :
      nDetect = 0; //Level
      break;
    case GPIOINT_TRIGGER_LOW :
      nPolarity = 0;
      nDetect = 0; //Level
      break;
    case GPIOINT_TRIGGER_RISING :
      nDetect = 1; //Pos Edge
      break;
    case GPIOINT_TRIGGER_FALLING :
      nDetect = 2; //Neg Edge
      break;
    case GPIOINT_TRIGGER_DUAL_EDGE :
      nDetect = 3; //Dual Edge
      break;
    default:
      break;
  }

  /*
   * Set both detect and polarity based on trigger type
   */
  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_DET, nDetect );
  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_POL, nPolarity );

} /* END SummaryGpioInternal_SetTrigger */


/* =========================================================================
**  Function : SummaryGpioInternal_GetTrigger
** =========================================================================*/
void SummaryGpioInternal_GetTrigger
(
  uint32                  nGpio,
  GPIOIntTriggerType      *peTrigger
)
{
  uint32 nDetectVal, nPolarityVal;

  nDetectVal  =   HAL_tlmmtile_Get( nGpio, 
                                    HAL_TLMMTILE_REG_INTRCFG, 
                                    HAL_TLMMTILE_FIELD_INTRCFG_DET );

  nPolarityVal  = HAL_tlmmtile_Get( nGpio, 
                                    HAL_TLMMTILE_REG_INTRCFG, 
                                    HAL_TLMMTILE_FIELD_INTRCFG_POL );

  switch(nDetectVal)
  {
	case 0 :
	   *peTrigger =
	   (nPolarityVal ? GPIOINT_TRIGGER_HIGH : GPIOINT_TRIGGER_LOW);
	   break;
	case 1 :
	   *peTrigger = GPIOINT_TRIGGER_RISING; 
	   break;
	case 2 :
	   *peTrigger = GPIOINT_TRIGGER_FALLING;
	   break;
	case 3 :
	   *peTrigger = GPIOINT_TRIGGER_DUAL_EDGE;
	   break;
	default:
	   break;
  }

} /* END SummaryGpioInternal_GetTrigger */


/* =========================================================================
**  Function : SummaryGpioInternal_EnableRawStatus
** =========================================================================*/
void SummaryGpioInternal_EnableRawStatus(uint32 nGpio)
{

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_RAW, 1 );

} /* END SummaryGpioInternal_EnableRawStatus */


/* =========================================================================
**  Function : SummaryGpioInternal_Clear
** =========================================================================*/
void SummaryGpioInternal_Clear(uint32 nGpio)
{

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRSTATUS, HAL_TLMMTILE_FIELD_INTRSTATUS_STATUS, 0 );

} /* END SummaryGpioInternal_Clear */


/* =========================================================================
**  Function : SummaryGpioInternal_Enable
** =========================================================================*/
void SummaryGpioInternal_Enable(uint32 nGpio)
{

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_EN, 1 );

} /* END SummaryGpioInternal_Enable */


/* =========================================================================
**  Function : SummaryGpioInternal_Disable
** =========================================================================*/
void SummaryGpioInternal_Disable(uint32 nGpio)
{

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRCFG, HAL_TLMMTILE_FIELD_INTRCFG_EN, 0 );

} /* END SummaryGpioInternal_Disable */

/* =========================================================================
**  Function : SummaryGpioInternal_TriggerInterrupt
** =========================================================================*/
void SummaryGpioInternal_TriggerInterrupt(uint32 nGpio)
{

  HAL_tlmmtile_Set( nGpio, HAL_TLMMTILE_REG_INTRSTATUS, HAL_TLMMTILE_FIELD_INTRSTATUS_STATUS, 1 );

} /* END SummaryGpioInternal_TriggerInterrupt */

/* =========================================================================
**  Function : SummaryGpioInternal_IsSet
** =========================================================================*/
boolean SummaryGpioInternal_IsSet(uint32 nGpio)
{
  uint32 nStatus;
  boolean bSet = FALSE;

  nStatus  = HAL_tlmmtile_Get( nGpio, 
                               HAL_TLMMTILE_REG_INTRSTATUS, 
                               HAL_TLMMTILE_FIELD_INTRSTATUS_STATUS );
 
  if( nStatus != 0 )
  {  
     bSet = TRUE;
  }

  /*
   * Return if we are set.
   */
  return bSet;

} /* END SummaryGpioInternal_IsSet */


/* =========================================================================
**  Function : SummaryGpioInternal_IsPending
** =========================================================================*/
boolean SummaryGpioInternal_IsPending(uint32 nGpio)
{
  uint32 nStatus;
  boolean bPending = FALSE;

  nStatus  = HAL_tlmmtile_Get( nGpio, 
                               HAL_TLMMTILE_REG_INTRSTATUS, 
                               HAL_TLMMTILE_FIELD_INTRSTATUS_STATUS );
 
  if( nStatus != 0 )
  {
   if(SummaryGpioInternal_IsEnabled(nGpio))
   {    
     bPending = TRUE;
   }
  }

  /*
   * Return if we are set
   */
  return bPending;

} /* END SummaryGpioInternal_IsPending */


/* =========================================================================
**  Function : SummaryGpioInternal_IsEnabled
** =========================================================================*/
boolean SummaryGpioInternal_IsEnabled(uint32 nGpio)
{
  GPIOIntProcessorType eTarget;
  boolean bEnabled = FALSE;

  uint32 nEnabled = HAL_tlmmtile_Get( nGpio, 
                                      HAL_TLMMTILE_REG_INTRCFG, 
                                      HAL_TLMMTILE_FIELD_INTRCFG_EN );;

  uint32 nVal  = HAL_tlmmtile_Get( nGpio, 
                                   HAL_TLMMTILE_REG_INTRCFG, 
                                   HAL_TLMMTILE_FIELD_INTRCFG_TARGET );

  SummaryGpioInternal_GetProcessorId(nVal, &eTarget);

  if((eTarget  == SummaryGpio_CfgCtxt.eProcessor) && (nEnabled == 1))
  {
    bEnabled = TRUE;
  }

  return bEnabled;

} /* END SummaryGpioInternal_IsEnabled */


/*=========================================================================
      Functions
==========================================================================*/

/*==========================================================================

  FUNCTION      SummaryGpio_Init

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Init( GPIOIntr_InitDataType *GPIOIntr_InitData )
{

   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   /*
    * Setup uLog buffer 
    */ 
   SummaryGpioInternal_LogInit(&pCtxt->hULog);
   if(pCtxt->hULog == NULL)
   {
       return DAL_ERROR;
   }

   if(!pCtxt->bIsInitialized)
   {
        /*
         * Attachs to the DALs which this Gpio Int DeviceType depends on.
         */
        if(DAL_SUCCESS != SummaryGpioInternal_AttachToDals(pCtxt))
        {
            return DAL_ERROR;
        }

         /*
          * Get Interrupt Config Data from Summary Gpio Int DAL Props
          */
         if(DAL_SUCCESS != SummaryGpioInternal_GetTargetData(pCtxt))
         {
             return DAL_ERROR;
         }

         /*
          * Allocate memory for the Summary GPIOInt State Table
          */
         if(DAL_SUCCESS != DALSYS_Malloc((pCtxt->nTotalGpioCount * sizeof(SummaryGpioCfgDataType)),
                                         (void **)&(pCtxt->pStateTable)))
         {
             return DAL_ERROR;
         }

         /*
          * Map the correct Func Pointer Table
          */
         pCtxt->pVtbl = &SummaryGpio_Procs;

         /* Setup local State Table */
         for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
         {
             pCtxt->pStateTable[nIdx].nGpio = GPIOINT_NONE;
         }

         /*
          * Call internal PD-specific Summary Init funcs
          */
         GPIOIntrSummaryInternal_Init(&SummaryGpio_CfgCtxt);

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
} /* END SummaryGpio_Init */

/*==========================================================================

  FUNCTION      SummaryGpio_DeInit

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_DeInit( void )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   /* 
    * Deregister all ISRs for all local registered GPIOs
    */
   for( nIdx = 0; nIdx < pCtxt->nTotalGpioCount; nIdx++ )
   {
       if( (pCtxt->pStateTable[nIdx].nGpio != GPIOINT_NONE) &&
           (pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED) )
       {
           if(DAL_SUCCESS != SummaryGpio_Deregister(pCtxt->pStateTable[nIdx].nGpio))
           {
               /* Do Nothing, rather move on with rest of the driver tear-down */
           }
       }
   }

   /* Detach for attached DAL drivers */
   SummaryGpioInternal_DetachToDals(pCtxt);

   /* Free up any memory allocated */
   DALSYS_Free(&(pCtxt->pStateTable));

   /*
	* Flag the mini-driver as uninitialised. 
	*/ 
   pCtxt->hULog = NULL;
   pCtxt->bIsInitialized = 0;

   /* Free up any resource obtained */

   return DAL_SUCCESS;
} /* END SummaryGpio_DeInit */

/*==========================================================================

  FUNCTION      SummaryGpio_IsSupported

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr )
{
   if ( pAttr == NULL )
   {
      return DAL_ERROR;
   }

   if ( pAttr->nPriority != GPIOINT_DEFAULT_PRIORITY ||
        pAttr->nUInterrupt != 0 )
   {
      return DAL_ERROR;
   }

   return DAL_SUCCESS;
}  /* END SummaryGpio_IsSupported */

/*==========================================================================

  FUNCTION      SummaryGpio_Config

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Config( uint32 nGpio, 
                              GpioIntr_Attribute *pAttr, 
                              GpioIntr_CfgType *pIntrCfgData )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   if ( pAttr == NULL )
   {
      GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                     "SummaryGpio_Config:FAIL. GPIO %d failed with pAttr == NULL", 
                     nGpio);
      return DAL_ERROR;
   }

   /* Program nGpio Summary Interrupt params in HW */
   SummaryGpioInternal_SetTargetProcessor(pCtxt->eProcessor, nGpio);

   /* Set SUMMARY interrupt trigger if the gpio is not a DIRECT CONNECT */ 
   SummaryGpioInternal_SetTrigger(nGpio, pAttr->eTrigger);

   /* Update local State Table for nGpio */
   SummaryGpioInternal_AssignIdxToGpio(nGpio, &nIdx);
   pCtxt->pStateTable[nIdx].eIntrTrigger = pAttr->eTrigger;

   /*
    * Call internal PD-specific Summary Config func
    */
   GPIOIntrSummaryInternal_ConfigureGPIOInterrupt(pCtxt, nGpio);

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 3, 
                  "SummaryGpio_Config:SUCCESS. GPIO %d with Trigger %d configured for Summary Interrupt Number %d", 
                  nGpio, (uint32)pAttr->eTrigger, pCtxt->nSummaryIntrID);


   /* Update GpioInt driver Fwk for nGpio */
   pIntrCfgData->nInterruptType = GPIOINT_SUMMARY;
   pIntrCfgData->nInterruptID   = pCtxt->nSummaryIntrID;
   
   #ifdef GPIOINT_USE_MPM
   if(pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_MPMINT)
   {
     pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_WAKEUP;
     GPIOIntr_ConfigMPMWakeup(pCtxt->hDalMpmHandle,nGpio,TRUE);
   }   
   else
   {
     #ifdef SummaryGpio_USE_NPA
     pCtxt->non_mpm_interrupts++;
     SummaryGpio_UpdateNPARequest(pCtxt);
     #endif /*SummaryGpio_USE_NPA */
   }
   #endif /*GPIOINT_USE_MPM */
  
   return DAL_SUCCESS;
} /* END SummaryGpio_Config */

/*==========================================================================

  FUNCTION      SummaryGpio_Deconfig

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Deconfig( uint32 nGpio, 
                                GpioIntr_CfgType *pIntrCfgData)
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   /*
    * Call internal PD-specific Summary UnConfig func
    */
   GPIOIntrSummaryInternal_UnConfigureGPIOInterrupt(pCtxt, nGpio);

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                  "SummaryGpio_Deconfig:SUCCESS. GPIO %d deconfigured for Summary Interrupt Number %d", 
                  nGpio, pIntrCfgData->nInterruptID);

   /* Update State Table to Default Values */
   if( DAL_SUCCESS != SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx) )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 2, 
                      "SummaryGpio_Deconfig:FAIL. FindIdxForGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   pCtxt->pStateTable[nIdx].nGpio = GPIOINT_NONE;
   pCtxt->pStateTable[nIdx].eIntrTrigger = GPIOINT_TRIGGER_HIGH;
   pCtxt->pStateTable[nIdx].nTypeFlags = 0x0;
   pCtxt->pStateTable[nIdx].isr = 0x0;
   pCtxt->pStateTable[nIdx].isr_param = 0x0;


   /* Update config data to default in GpioInt Driver State Table for nGpio */
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
     #ifdef SummaryGpio_USE_NPA
     pCtxt->non_mpm_interrupts--;
     DirConnGpio_UpdateNPARequest(pCtxt);
     #endif /*SummaryGpio_USE_NPA */
   }
   #endif /*GPIOINT_USE_MPM */
  
   return DAL_SUCCESS;
} /* END SummaryGpio_DeConfig */

/*==========================================================================

  FUNCTION      SummaryGpio_Register

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Register( uint32 nGpio, void *pfnIsr, void *pIsrParam )
{
   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   eResult = SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                      "SummaryGpio_Register:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   /* Populate state table with the ISR and params */
   pCtxt->pStateTable[nIdx].isr         = pfnIsr;
   pCtxt->pStateTable[nIdx].isr_param   = (GPIOINTISRCtx)pIsrParam;
   pCtxt->pStateTable[nIdx].nTypeFlags |= GPIOINTF_REGISTERED;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "SummaryGpio_Register:SUCCESS. GPIO %d registered",
                  nGpio);

   SummaryGpio_Enable(nGpio);

   return eResult;
} /* END SummaryGpio_Register */

/*==========================================================================

  FUNCTION      SummaryGpio_Deregister

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Deregister( uint32 nGpio )
{
   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   eResult = SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                      "SummaryGpio_Deregister:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   SummaryGpio_Disable(nGpio);

   pCtxt->pStateTable[nGpio].isr         = NULL;
   pCtxt->pStateTable[nGpio].isr_param   = NULL;
   pCtxt->pStateTable[nGpio].nTypeFlags &= ~GPIOINTF_REGISTERED;

   GPIOIntLog_Out(GPIOINTLOG_REG, pCtxt->hULog, 1, 
                  "SummaryGpio_Deregister:SUCCESS. GPIO %d deregistered",
                  nGpio);

   return eResult;
} /* END SummaryGpio_Deregister */

/*==========================================================================

  FUNCTION      SummaryGpio_Enable

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Enable( uint32 nGpio )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "SummaryGpio_Enable: Enabling GPIO %d for Summary Interrupt", 
                  nGpio );

   SummaryGpioInternal_EnableRawStatus(nGpio);

   /* Wait for the configuration to set in. */ 
   busywait(5);

   /* Clear and enable the interrupt */
   SummaryGpioInternal_Clear(nGpio);

   SummaryGpioInternal_Enable(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_Enable */

/*==========================================================================

  FUNCTION      SummaryGpio_Disable

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_Disable( uint32 nGpio )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;

   GPIOIntLog_Out(GPIOINTLOG_ENABLE, pCtxt->hULog, 1, 
                  "SummaryGpio_Disable: Disabling Summary GPIO %d", nGpio);

   SummaryGpioInternal_Disable(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_Disable */

/*==========================================================================

  FUNCTION      SummaryGpio_SetTrigger

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger )
{
   DALResult eResult = DAL_ERROR;
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
   uint32 nIdx;

   eResult = SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx);
   if( DAL_SUCCESS != eResult )
   {
       GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 1, 
                      "SummaryGpio_SetTrigger:FAIL. FindIdxByGpio() Failed for GPIO %d", 
                       nGpio);
       return DAL_ERROR;
   }

   SummaryGpioInternal_SetTrigger(nGpio,(GPIOIntTriggerType)nNewTrigger);

   GPIOIntLog_Out(GPIOINTLOG_SETTRIGGER, pCtxt->hULog, 2, 
                  "SummaryGpio_SetTrigger: GPIO %d Set with new direct connect Trigger %d", 
                  nGpio, nNewTrigger);

   pCtxt->pStateTable[nGpio].eIntrTrigger = (GPIOIntTriggerType)nNewTrigger;

   return DAL_SUCCESS;
} /* END SummaryGpio_SetTrigger */

/*==========================================================================

  FUNCTION      SummaryGpio_TriggerInterrupt

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_TriggerInterrupt( uint32 nGpio )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;

   GPIOIntLog_Out(GPIOINTLOG_TRIGGER, pCtxt->hULog, 2, 
                  "SummaryGpio_TriggerInterrupt: Triggering GPIO %d with Summary interrupt ID %d", 
                  nGpio, pCtxt->nSummaryIntrID);

   SummaryGpioInternal_TriggerInterrupt(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_TriggerInterrupt */

/*==========================================================================

  FUNCTION      SummaryGpio_ClearInterrupt

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_ClearInterrupt( uint32 nGpio )
{
   SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;

   GPIOIntLog_Out(GPIOINTLOG_CLEAR, pCtxt->hULog, 2, 
                  "SummaryGpio_ClearInterrupt: Clearing GPIO %d with Summary interrupt ID %d", 
                  nGpio, pCtxt->nSummaryIntrID);

   SummaryGpioInternal_Clear(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_ClearInterrupt */

/*==========================================================================

  FUNCTION      SummaryGpio_IsInterruptSet

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_IsInterruptSet( uint32* state, uint32 nGpio )
{
   *state = (uint32)SummaryGpioInternal_IsSet(nGpio);

   return DAL_SUCCESS;
}  /* END SummaryGpio_IsInterruptSet */

/*==========================================================================

  FUNCTION      SummaryGpio_IsInterruptPending

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_IsInterruptPending( uint32* state, uint32 nGpio )
{
   *state = (uint32)SummaryGpioInternal_IsPending(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_IsInterruptPending */

/*==========================================================================

  FUNCTION      SummaryGpio_IsInterruptEnabled

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_IsInterruptEnabled( uint32* state, uint32 nGpio )
{
   *state = (uint32)SummaryGpioInternal_IsEnabled(nGpio);

   return DAL_SUCCESS;
} /* END SummaryGpio_IsInterruptEnabled */


/*==========================================================================

  FUNCTION      SummaryGpio_DecrementNonMPMInterrupts

  DESCRIPTION   See GPIOInt_Summary.h

==========================================================================*/

DALResult SummaryGpio_DecrementNonMPMInterrupts( uint32 nGpio )
{
  SummaryGpioCfgCtxtType *pCtxt = &SummaryGpio_CfgCtxt;
  DALResult eResult = DAL_ERROR;
  uint32 nIdx;
  
  eResult = SummaryGpioInternal_FindIdxByGpio(nGpio, &nIdx);
  if( DAL_SUCCESS == eResult )
  {
    if( pCtxt->pStateTable[nIdx].nTypeFlags & GPIOINTF_REGISTERED) 
    { 
      #ifdef SummaryGpio_USE_NPA
      pCtxt->non_mpm_interrupts--;
      SummaryGpio_UpdateNPARequest(pCtxt);
      #endif
    }
  }
  return DAL_SUCCESS;
} /* END SummaryGpio_DecrementNonMPMInterrupts */

