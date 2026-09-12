#ifndef __DALGPIOInt_H
#define __DALGPIOInt_H
/*===========================================================================

  D A L   G P I O   I N T E R R U P T   C O N T R O L L E R    

DESCRIPTION
  This modules contains enumeration definitions to support the Dal GPIO 
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

$Header: //components/rel/core.mpss/10.0/systemdrivers/GPIOInt/src/kernel/GPIOInt.h#1 $

when        who     what, where, why
--------    ---     ---------------------------------------------------------- 
5/18/2017   hvm     GPIOInt driver refactored. 
1/1/2010    aratin  First draft created. 
===========================================================================*/
#include "DALFramework.h"
#include "DALGlbCtxt.h"
#include "GPIOIntr.h"

/*------------------------------------------------------------------------------
Declaring a "GPIOInt" Driver,Device and Client Context
------------------------------------------------------------------------------*/

typedef struct GPIOIntDrvCtxt    GPIOIntDrvCtxt;
typedef struct GPIOIntDevCtxt    GPIOIntDevCtxt;
typedef struct GPIOIntClientCtxt GPIOIntClientCtxt;

/*=========================================================================

                           DATA TYPE DEFINITIONS

===========================================================================*/

/*=========================================================================

                           DATA DEFINITIONS

===========================================================================*/

/*------------------------------------------------------------------------------
Declaring a private "GPIOInt" Vtable
------------------------------------------------------------------------------*/

typedef struct GPIOIntDALVtbl GPIOIntDALVtbl;
struct GPIOIntDALVtbl
{
	int (*GPIOIntr_DriverInit)(GPIOIntDrvCtxt *);
	int (*GPIOIntr_DriverDeInit)(GPIOIntDrvCtxt *);
};

struct GPIOIntDevCtxt
{
  //Base Members
  uint32                         dwRefs;
  DALDEVICEID                    DevId;
  uint32                         dwDevCtxtRefIdx;
  GPIOIntDrvCtxt                 *pGPIOIntDrvCtxt;
  DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
  uint32                         Reserved[16];
};

struct GPIOIntDrvCtxt
{
  //Base Members
  GPIOIntDALVtbl                 GPIOIntDALVtbl;
  uint32                         dwNumDev;
  uint32                         dwSizeDevCtxt;
  uint32                         bInit;
  uint32                         dwRefs;
  GPIOIntDevCtxt                 GPIOIntDevCtxt[2];
  //GPIOInt Drv state can be added by developers here
};

/*------------------------------------------------------------------------------
Declaring a "GPIOInt" Client Context
------------------------------------------------------------------------------*/
struct GPIOIntClientCtxt
{
  //Base Members
  uint32                         dwRefs;
  uint32                         dwAccessMode;
  void                           *pPortCtxt;
  GPIOIntDevCtxt                 *pGPIOIntDevCtxt;
  GPIOIntHandle                  GPIOIntHandle; 
  //GPIOInt Client state can be added by developers here
  GPIOIntr_ClientHandle          *hGPIOIntr;
};

DALResult GPIOIntr_DriverInit(GPIOIntDrvCtxt *);
DALResult GPIOIntr_DriverDeInit(GPIOIntDrvCtxt *);
DALResult GPIOIntr_DeviceInit(GPIOIntr_ClientHandle *);
DALResult GPIOIntr_DeviceDeInit(GPIOIntr_ClientHandle *);
DALResult GPIOIntr_Reset(GPIOIntr_ClientHandle *);
DALResult GPIOIntr_PowerEvent(GPIOIntr_ClientHandle *,DalPowerCmd, DalPowerDomain);
DALResult GPIOIntr_Open(GPIOIntr_ClientHandle *,uint32);
DALResult GPIOIntr_Close(GPIOIntr_ClientHandle *);
DALResult GPIOIntr_Info(GPIOIntr_ClientHandle *, DalDeviceInfo *, uint32);
DALResult GPIOIntr_InheritObjects(GPIOIntr_ClientHandle *,DALInheritSrcPram *,DALInheritDestPram *);
DALResult GPIOIntr_MapMPMInterrupt(GPIOIntr_ClientHandle * pCtxt,uint32 nGPIO,uint32 mpm_interrupt_id);
#ifdef GPIOINT_USE_MPM
DALResult GPIOIntr_ConfigMPMWakeup(DalDeviceHandle  *hDevice,uint32 gpio,int8 bEnable);
#endif

#endif /*___DALGPIOInt_H*/
