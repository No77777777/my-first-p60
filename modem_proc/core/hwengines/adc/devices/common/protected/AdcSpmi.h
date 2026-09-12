#ifndef __ADC_SPMI_H__
#define __ADC_SPMI_H__
/*============================================================================
  @file AdcSpmi.h

  Function and data structure declarations for ADC SPMI wrapper


                Copyright (c) 2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/common/protected/AdcSpmi.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DALFramework.h"
#include "DDISpmi.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define ADC_SPMI_ADDR(perph, reg) ((((perph) << 8) & 0xff00) | ((reg) & 0xff))

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef struct
{
   DalDeviceHandle *phSpmiDev;
   uint32 uSlaveId;
   uint32 uPeripheralId;
   Spmi_AccessPriority ePriority;
} AdcSpmiCtxtType;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
static __inline DALResult
AdcSpmiInit(AdcSpmiCtxtType *pCtxt, Spmi_AccessPriority ePriority, uint32 uSlaveId, uint32 uPeripheralId)
{
   DALResult result;

   pCtxt->uSlaveId = uSlaveId;
   pCtxt->uPeripheralId = uPeripheralId;
   pCtxt->ePriority = ePriority;

   result = DAL_SpmiDeviceAttach("DALDEVICEID_SPMI_DEVICE", &pCtxt->phSpmiDev);
   if (result != DAL_SUCCESS)
   {
      return result;
   }

   return DalDevice_Open(pCtxt->phSpmiDev,
                         DAL_OPEN_SHARED | DAL_OPEN_READ | DAL_OPEN_WRITE);
}

static __inline DALResult
AdcSpmiDeinit(AdcSpmiCtxtType *pCtxt)
{
   return DAL_SUCCESS;
}

static __inline DALResult
AdcSpmiRead(AdcSpmiCtxtType *pCtxt, uint32 uReg, uint8 *pucData, uint32 uLen, uint32 *puBytesRead)
{
   return DalSpmi_ReadLong(pCtxt->phSpmiDev,
                           pCtxt->uSlaveId,
                           pCtxt->ePriority,
                           ADC_SPMI_ADDR(pCtxt->uPeripheralId, uReg),
                           pucData,
                           uLen,
                           puBytesRead);
}

static __inline DALResult
AdcSpmiWrite(AdcSpmiCtxtType *pCtxt, uint32 uReg, uint8 *pucData, uint32 uLen)
{
   return DalSpmi_WriteLong(pCtxt->phSpmiDev,
                            pCtxt->uSlaveId,
                            pCtxt->ePriority,
                            ADC_SPMI_ADDR(pCtxt->uPeripheralId, uReg),
                            pucData,
                            uLen);
}

static __inline DALResult
AdcSpmiRegisterInterrupt(AdcSpmiCtxtType *pCtxt, uint8 ucMask, const SpmiIsr isr, const void *pIsrCtx)
{
   return DalSpmi_RegisterIsr(pCtxt->phSpmiDev,
                              ((pCtxt->uSlaveId << 8) & 0xF00) | (pCtxt->uPeripheralId & 0xFF),
                              ucMask,
                              isr,
                              pIsrCtx);
}

#endif /* #ifndef __ADC_SPMI_H__ */

