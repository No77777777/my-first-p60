#ifndef __GPIOInt_Summary_H
#define __GPIOInt_Summary_H
/*===========================================================================

  S U M M A R Y   G P I O   I N T E R R U P T   C O N T R O L L E R

DESCRIPTION
  Header file for a specific type of GPIOInt - Summary Interrupts

REFERENCES

       Copyright (c) 2017 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/inc/GPIOInt_Summary.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/15/2017  hvm     First draft created. 
===========================================================================*/
#include "DalDevice.h"
#include "DDIGPIOInt.h"
#include "GPIOInt.h"
#include "HALtlmmtile.h"

#ifdef SUMMARYGPIO_USE_NPA
#include "npa.h"
#endif

/*=========================================================================

                           CONSTANT DECLARATIONS

===========================================================================*/

/* 
 * Summary INTR Retry value used to ERROR_FATAL in case if GPIO is not found
 */
#define GPIOINT_SUMMARY_INTR_NO_OF_TRIALS     10

/*=========================================================================

                           DATA TYPE DEFINITIONS

===========================================================================*/

/* 
 * GPIOIntGlbCtxtType defines the main data structure in DAL global context 
 * for saving interrupt regeneration status of each GPIO Interrupt. 
 * Each bit in gpioint_reg_state[ ] array represent the registration 
 * status of a GPIO interrupt. 
 */ 
 
typedef struct
{
  DALGLB_HEADER                  hdr;  /* Shared memory header*/  
  uint32                         gpioint_reg_state[1];
} GPIOIntGlbCtxtType;

typedef struct
{
  uint32                         nGpio;
  GPIOIntTriggerType             eIntrTrigger;
  uint32                         nTypeFlags;
  GPIOINTISR                     isr;
  GPIOINTISRCtx                  isr_param;
} SummaryGpioCfgDataType;


typedef struct
{
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  DalDeviceHandle *              hIntrCntrlHandle;
  DalDeviceHandle *              hGPIOMgrHandle;
  uint32                         nTotalGpioCount;
  SummaryGpioCfgDataType *       pStateTable;
  GPIOIntFnDescType *            pVtbl;
  ULogHandle                     hULog; 
  uint8                          bIsInitialized;
  GPIOIntProcessorType           eProcessor;
  uint32                         nSummaryIntrID;
  GPIOIntGlbCtxtType             *gpioint_glb_ctxt; //GPIOINT_TODO: remove from GpioInt Driver Fwk
  uint32                         gpioint_unhandled;
  int                            gpioint_qdi;
  #ifdef SUMMARYGPIO_USE_NPA
  npa_client_handle              hNpaClient;
  uint32                         non_mpm_interrupts;
  #endif /* SUMMARYGPIO_USE_NPA */
  qurt_mutex_t*                  pLock; 
  #ifdef GPIOINT_USE_MPM
  DalDeviceHandle *              hDalMpmHandle;
  #endif /* GPIOINT_USE_MPM */
} SummaryGpioCfgCtxtType;

/*=========================================================================

                    INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

/* =========================================================================
**  Function : SummaryGpioInternal_SetTrigger
** =========================================================================*/
void SummaryGpioInternal_SetTrigger(uint32 nGpio, GPIOIntTriggerType eTrigger);

/* =========================================================================
**  Function : SummaryGpioInternal_GetTrigger
** =========================================================================*/
void SummaryGpioInternal_GetTrigger(uint32 nGpio, GPIOIntTriggerType *peTrigger);

/* =========================================================================
**  Function : SummaryGpioInternal_EnableRawStatus
** =========================================================================*/
void SummaryGpioInternal_EnableRawStatus(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_Clear
** =========================================================================*/
void SummaryGpioInternal_Clear(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_Enable
** =========================================================================*/
void SummaryGpioInternal_Enable(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_Disable
** =========================================================================*/
void SummaryGpioInternal_Disable(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_TriggerInterrupt
** =========================================================================*/
void SummaryGpioInternal_TriggerInterrupt(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_IsSet
** =========================================================================*/
boolean SummaryGpioInternal_IsSet(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_IsPending
** =========================================================================*/
boolean SummaryGpioInternal_IsPending(uint32 nGpio);

/* =========================================================================
**  Function : SummaryGpioInternal_IsEnabled
** =========================================================================*/
boolean SummaryGpioInternal_IsEnabled(uint32 nGpio);

/*=========================================================================

                         FUNCTION DEFINITIONS

===========================================================================*/

DALResult SummaryGpio_Init( GPIOIntr_InitDataType *GPIOIntr_InitData );
DALResult SummaryGpio_DeInit( void );
DALResult SummaryGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr );
DALResult SummaryGpio_Config( uint32 nGpio, GpioIntr_Attribute *pAttr, GpioIntr_CfgType *pIntrCfgData);
DALResult SummaryGpio_Deconfig( uint32 nGpio, GpioIntr_CfgType *pIntrCfgData); 
DALResult SummaryGpio_Register(uint32 nGpio, void *pfnIsr, void *pIsrParam );
DALResult SummaryGpio_Deregister( uint32 nGpio ); 
DALResult SummaryGpio_Enable( uint32 nGpio );
DALResult SummaryGpio_Disable( uint32 nGpio );
DALResult SummaryGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger );
DALResult SummaryGpio_TriggerInterrupt( uint32 nGpio );
DALResult SummaryGpio_ClearInterrupt( uint32 nGpio );
DALResult SummaryGpio_IsInterruptSet( uint32* state, uint32 nGpio );
DALResult SummaryGpio_IsInterruptPending( uint32* state, uint32 nGpio );
DALResult SummaryGpio_IsInterruptEnabled( uint32* state, uint32 nGpio );
DALResult SummaryGpio_DecrementNonMPMInterrupts( uint32 nGpio );
#ifdef SUMMARYGPIO_USE_NPA
void SummaryGpio_UpdateNPARequest(void *context);
#endif /*SUMMARYGPIO_USE_NPA*/ 

/* PD specific functions */
DALResult GPIOIntrSummaryInternal_Init(SummaryGpioCfgCtxtType *device);
DALResult GPIOIntrSummaryInternal_UnConfigureGPIOInterrupt( SummaryGpioCfgCtxtType *device, uint32 gpio );
DALResult GPIOIntrSummaryInternal_ConfigureGPIOInterrupt( SummaryGpioCfgCtxtType *device, uint32 gpio );

/*=============================================================================

  FUNCTION      GPIOIntr_RunIsr

  DESCRIPTION   
  Run the ISR associated with a GPIO.During power collapse, all apps gpio 
  interrupts are not clear, so the same interrupt will be fired after switched 
  back to apps processor.

  PARAMETERS  
  GPIOIntDevCtxt *device : The Device context pointer of the GPIOInt driver.
  gpio                   : The GPIO pin whose ISR needs to be executed.

  DEPENDENCIES  None.

  RETURN VALUE  None.

  SIDE EFFECTS  None.

=============================================================================*/

void 
GPIOIntr_RunIsr
(
  SummaryGpioCfgCtxtType *device,
  uint32 gpio
);

/*=============================================================================

  FUNCTION      GPIOIntr_LogEvent

  DESCRIPTION   
  This function can be used to log the GPIOInt driver.

  PARAMETERS   
  GPIOIntDevCtxt *device : The Device context pointer of the GPIOInt driver.
  gpio :                   The GPIO pin whose interrupt state needs to
                           be logged.

  DEPENDENCIES  None.

  RETURN VALUE  None.

  SIDE EFFECTS  None.

=============================================================================*/

void 
GPIOIntr_LogEvent
(
  SummaryGpioCfgCtxtType *device, //GPIOINT_TODO: 
  uint32 gpio
);


#endif /*__GPIOInt_Summary_H*/
