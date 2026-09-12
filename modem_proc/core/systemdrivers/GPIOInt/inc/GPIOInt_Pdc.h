#ifndef __GPIOInt_Pdc_H
#define __GPIOInt_Pdc_H
/*===========================================================================

        P D C   G P I O   I N T E R R U P T   C O N T R O L L E R

DESCRIPTION
  Header file for a specific type of GPIOInt - PDC Interrupts

REFERENCES

       Copyright (c) 2017 QUALCOMM Technologies Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/inc/GPIOInt_Pdc.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
5/15/2017  hvm     First draft created. 
===========================================================================*/
#include "DalDevice.h"
#include "DDIGPIOInt.h"
#include "GPIOInt.h"

/*=========================================================================

                           CONSTANT DECLARATIONS

===========================================================================*/



/*=========================================================================

                           DATA TYPE DEFINITIONS

===========================================================================*/
typedef struct
{
  uint32                         nGpio;
  GPIOIntTriggerType             eIntrTrigger;
  uint32                         nQurtIntrID;
  uint32                         nTypeFlags;
  uint32                         nUInterrupt;
  uint32                         nPriority;
  GPIOINTISR                     isr;
  GPIOINTISRCtx                  isr_param;
} PdcGpioCfgDataType;


typedef struct
{
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  DalDeviceHandle *              hIntrCntrlHandle;
  uint32                         nTotalGpioCount;
  PdcGpioCfgDataType *           pStateTable; //GPIOINT_TODO: optimizate to LUT-based 
  GPIOIntFnDescType *            pVtbl;
  ULogHandle                     hULog; 
  uint8                          bIsInitialized;
  qurt_mutex_t*                  pLock; 
} PdcGpioCfgCtxtType;

/*=========================================================================

                         FUNCTION DEFINITIONS

===========================================================================*/

DALResult PdcGpio_Init( GPIOIntr_InitDataType *GPIOIntr_InitData );
DALResult PdcGpio_DeInit( void );
DALResult PdcGpio_IsSupported( uint32 nGpio, GpioIntr_Attribute *pAttr );
DALResult PdcGpio_Config( uint32 nGpio, GpioIntr_Attribute *pAttr, GpioIntr_CfgType *pIntrCfgData);
DALResult PdcGpio_Deconfig( uint32 nGpio, GpioIntr_CfgType *pIntrCfgData); 
DALResult PdcGpio_Register(uint32 nGpio, void *pfnIsr, void *pIsrParam );
DALResult PdcGpio_Deregister( uint32 nGpio ); 
DALResult PdcGpio_Enable( uint32 nGpio );
DALResult PdcGpio_Disable( uint32 nGpio );
DALResult PdcGpio_SetTrigger( uint32 nGpio, uint32 nNewTrigger );
DALResult PdcGpio_TriggerInterrupt( uint32 nGpio );
DALResult PdcGpio_ClearInterrupt( uint32 nGpio );
DALResult PdcGpio_IsInterruptSet( uint32* state, uint32 nGpio );
DALResult PdcGpio_IsInterruptPending( uint32* state, uint32 nGpio );
DALResult PdcGpio_IsInterruptEnabled( uint32* state, uint32 nGpio );

#endif /*__GPIOInt_Pdc_H*/
