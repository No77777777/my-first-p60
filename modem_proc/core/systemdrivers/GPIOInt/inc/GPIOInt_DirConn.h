#ifndef __GPIOInt_DirConn_H
#define __GPIOInt_DirConn_H
/*===========================================================================

  D I R E C T   C O N N   G P I O   I N T E R R U P T   C O N T R O L L E R

DESCRIPTION
  Header file for a specific type of GPIOInt - Direct Connect Interrupts

REFERENCES

       Copyright (c) 2017 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/inc/GPIOInt_DirConn.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/15/2017  hvm     First draft created. 
===========================================================================*/
#include "DalDevice.h"
#include "DDIGPIOInt.h"
#include "GPIOInt.h"
#include "HALtlmmtile.h"
  
#ifdef DIRCONNGPIO_USE_NPA
#include "npa.h"
#endif

/*=========================================================================

                           CONSTANT DECLARATIONS

===========================================================================*/



/*=========================================================================

                           DATA TYPE DEFINITIONS

===========================================================================*/
typedef struct
{
  uint32                         nGpio;
  GPIOIntInterruptType           eIntrType;
  GPIOIntTriggerType             eIntrTrigger;
  uint32                         nQurtIntrID;
  uint32                         nTypeFlags;
  uint32                         nUInterrupt;
  uint32                         nPriority;
  uint32                         mpm_interrupt_id;
  GPIOINTISR                     isr;
  GPIOINTISRCtx                  isr_param;
  uint32                         nDirConnID;
  uint32                         nDirectConnectBaseType;
} DirConnGpioCfgDataType;


typedef struct
{
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  DalDeviceHandle *              hIntrCntrlHandle;
  DalDeviceHandle *              hGPIOMgrHandle;
  uint32                         nTotalGpioCount;
  DirConnGpioCfgDataType *       pStateTable;
  GPIOIntFnDescType *            pVtbl;
  ULogHandle                     hULog; 
  uint8                          bIsInitialized;
  #ifdef DIRCONNGPIO_USE_NPA
  npa_client_handle              hNpaClient;
  uint32                         non_mpm_interrupts;
  #endif /* DIRCONNGPIO_USE_NPA */
  qurt_mutex_t*                  pLock; 
  #ifdef GPIOINT_USE_MPM
  DalDeviceHandle *              hDalMpmHandle;
  #endif /* GPIOINT_USE_MPM */
} DirConnGpioCfgCtxtType;

/*=========================================================================

                         FUNCTION DEFINITIONS

===========================================================================*/

DALResult DirConnGpio_Init( GPIOIntr_InitDataType *GPIOIntr_InitData );
DALResult DirConnGpio_DeInit( void );
DALResult DirConnGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr );
DALResult DirConnGpio_Config( uint32 nGpio, GpioIntr_Attribute *pAttr, GpioIntr_CfgType *pIntrCfgData);
DALResult DirConnGpio_Deconfig( uint32 nGpio, GpioIntr_CfgType *pIntrCfgData); 
DALResult DirConnGpio_Register(uint32 nGpio, void *pfnIsr, void *pIsrParam );
DALResult DirConnGpio_Deregister( uint32 nGpio ); 
DALResult DirConnGpio_Enable( uint32 nGpio );
DALResult DirConnGpio_Disable( uint32 nGpio );
DALResult DirConnGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger );
DALResult DirConnGpio_TriggerInterrupt( uint32 nGpio );
DALResult DirConnGpio_ClearInterrupt( uint32 nGpio );
DALResult DirConnGpio_IsInterruptSet( uint32* state, uint32 nGpio );
DALResult DirConnGpio_IsInterruptPending( uint32* state, uint32 nGpio );
DALResult DirConnGpio_IsInterruptEnabled( uint32* state, uint32 nGpio );
DALResult DirConnGpio_DecrementNonMPMInterrupts( uint32 nGpio );
#ifdef DIRCONNGPIO_USE_NPA
void DirConnGpio_UpdateNPARequest(void *context);
#endif /*DIRCONNGPIO_USE_NPA*/ 

#endif /*__GPIOInt_DirConn_H*/
