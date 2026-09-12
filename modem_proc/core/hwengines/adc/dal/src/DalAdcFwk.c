/*============================================================================
  FILE:         DALAdcFwk.c

  OVERVIEW:     Implementation of the ADC DAL

  DEPENDENCIES: None

                Copyright (c) 2009-2012, 2015, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/dal/src/DalAdcFwk.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-06  jjo  DAL framework updates.
  2015-03-27  jjo  Added tolerance API.
  2015-01-08  jjo  Added TM APIs.
  2012-04-19  jdt  Updated for 8974.
  2010-06-20  prb  klock work fix in Adc_DalAdc_Attach.
  2009-03-16  jdt  Initial revision.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DALFramework.h"
#include "DALSys.h"
#include "DDIAdc.h"
#include "DalAdc.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
AdcDrvCtxt gAdcDrvCtxt =
{
   .AdcDALVtbl    = {Adc_DriverInit, Adc_DriverDeInit},
   .dwNumDev      = ADC_MAX_NUM_DEVICES,
   .dwSizeDevCtxt = sizeof(AdcDevCtxt),
   .bInit         = 0,
   .dwRefs        = 0,
   .AdcDevCtxt    = {{0}}
};

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
static DALBOOL init_done = FALSE;

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint32
Adc_DalAdc_AddRef(DalAdcHandle *h)
{
   return DALFW_AddRef((DALClientCtxt *)(h->pClientCtxt));
}

/*------------------------------------------------------------------------------
Following functions are defined in DalDevice DAL Interface.
------------------------------------------------------------------------------*/
static uint32
Adc_DalAdc_Detach(uint32 uClientHandle, DalDeviceHandle *h)
{
   uint32 dwref;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return 0;
   }

   /* Unregister the client */
   (void)Adc_DeregisterClient(h->pClientCtxt);

   dwref = DALFW_Release((DALClientCtxt *)(h->pClientCtxt));
   if (0 == dwref)
   {
      /* Release the client context */
      DALSYS_Free(h->pClientCtxt);
   }
   return dwref;
}

static DALResult
Adc_DalAdc_Init(uint32 uClientHandle, DalDeviceHandle *h)
{
   DALResult result;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   if (init_done == TRUE)
   {
      return DAL_SUCCESS;
   }

   result = Adc_DeviceInit(h->pClientCtxt);
   if (result == DAL_SUCCESS)
   {
      init_done = TRUE;
   }

   return result;
}

static DALResult
Adc_DalAdc_DeInit(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_DeviceDeInit(h->pClientCtxt);
}

static DALResult
Adc_DalAdc_PowerEvent(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   DalPowerCmd PowerCmd,
   DalPowerDomain PowerDomain
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_PowerEvent(h->pClientCtxt, PowerCmd, PowerDomain);
}

static DALResult
Adc_DalAdc_Open(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 mode
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_Open(h->pClientCtxt, mode);
}

static DALResult
Adc_DalAdc_Close(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_Close(h->pClientCtxt);
}

static DALResult
Adc_DalAdc_Info(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   DalDeviceInfo *info,
   uint32 infoSize
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   info->Version = DALADC_INTERFACE_VERSION;
   return Adc_Info(h->pClientCtxt,info,infoSize);
}

static DALResult
Adc_DalAdc_SysRequest(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   DalSysReq ReqIdx,
   const void *SrcBuf,
   uint32 SrcBufLen,
   void *DestBuf,
   uint32 DestBufLen,
   uint32 *DestBufLenReq
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return DAL_ERROR;
}

/*------------------------------------------------------------------------------
Following functions are extended in DalAdc Interface.
------------------------------------------------------------------------------*/
static DALResult
Adc_DalAdc_GetAdcInputProperties(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const char *pInputName,
   uint32 nInputNameSize,
   AdcInputPropertiesType *pAdcInputProps
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_GetAdcInputProperties(((DalAdcHandle * )h)->pClientCtxt,
                                    pInputName,
                                    nInputNameSize,
                                    pAdcInputProps);
}

static DALResult
Adc_DalAdc_Read(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcInputPropertiesType *pAdcInputProps,
   AdcResultType *pAdcResult
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_Read(((DalAdcHandle *)h)->pClientCtxt,
                   pAdcInputProps,
                   pAdcResult);
}

static DALResult
Adc_DalAdc_RequestConversion(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcRequestParametersType *pAdcParams,
   AdcRequestStatusType *pAdcRequestStatus
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_RequestConversion(((DalAdcHandle *)h)->pClientCtxt,
                                pAdcParams,
                                pAdcRequestStatus);
}

static DALResult
Adc_DalAdc_RequestRecalibration(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcRequestParametersType *pAdcParams
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_RequestRecalibration(((DalAdcHandle *)h)->pClientCtxt, pAdcParams);
}

static DALResult
Adc_DalAdc_TMGetInputProperties(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const char *pInputName,
   uint32 nInputNameLen,
   AdcTMInputPropertiesType *pAdcTMInputProps
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_TMGetInputProperties(((DalAdcHandle *)h)->pClientCtxt,
                                   pInputName,
                                   nInputNameLen,
                                   pAdcTMInputProps);
}

static DALResult
Adc_DalAdc_TMGetRange(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcTMInputPropertiesType *pAdcTMInputProps,
   AdcTMRangeType *pAdcTMRange
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_TMGetRange(((DalAdcHandle *)h)->pClientCtxt,
                         pAdcTMInputProps,
                         pAdcTMRange);
}

static DALResult
Adc_DalAdc_TMSetThreshold(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcTMRequestParametersType *pAdcTMParams,
   AdcTMThresholdType eThreshold,
   const int32 *pnThresholdDesired,
   int32 *pnThresholdSet
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_TMSetThreshold(((DalAdcHandle *)h)->pClientCtxt,
                             pAdcTMParams,
                             eThreshold,
                             pnThresholdDesired,
                             pnThresholdSet);
}

static DALResult
Adc_DalAdc_TMSetEnableThresholds(uint32 uClientHandle, DalDeviceHandle *h, DALBOOL bEnable)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_TMSetEnableThresholds(((DalAdcHandle *)h)->pClientCtxt, bEnable);
}

static DALResult
Adc_DalAdc_TMSetTolerance(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const AdcTMRequestParametersType *pAdcTMParams,
   int32 nPhysicalCurrent,
   const int32 *pnLowerTolerance,
   const int32 *pnHigherTolerance
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Adc_TMSetTolerance(((DalAdcHandle *)h)->pClientCtxt,
                             pAdcTMParams,
                             nPhysicalCurrent,
                             pnLowerTolerance,
                             pnHigherTolerance);
}

static void
Adc_InitInterface(AdcClientCtxt *pClientCtxt)
{
   static const DalAdc vtbl =
   {
      {
         Adc_DalAdc_Attach,
         Adc_DalAdc_Detach,
         Adc_DalAdc_Init,
         Adc_DalAdc_DeInit,
         Adc_DalAdc_Open,
         Adc_DalAdc_Close,
         Adc_DalAdc_Info,
         Adc_DalAdc_PowerEvent,
         Adc_DalAdc_SysRequest
      },
      Adc_DalAdc_RequestConversion,
      Adc_DalAdc_RequestRecalibration,
      Adc_DalAdc_GetAdcInputProperties,
      Adc_DalAdc_TMGetInputProperties,
      Adc_DalAdc_TMGetRange,
      Adc_DalAdc_TMSetThreshold,
      Adc_DalAdc_TMSetEnableThresholds,
      Adc_DalAdc_Read,
      Adc_DalAdc_TMSetTolerance,
   };
   /*--------------------------------------------------------------------------
   Depending upon client type setup the vtables (entry points)
   --------------------------------------------------------------------------*/
   pClientCtxt->DalAdcHandle.dwDalHandleId = DALDEVICE_INTERFACE_HANDLE_ID;
   pClientCtxt->DalAdcHandle.pVtbl = &vtbl;
   pClientCtxt->DalAdcHandle.pClientCtxt = pClientCtxt;
   pClientCtxt->DalAdcHandle.dwVtblen = sizeof(vtbl) / sizeof( void (*)(void));
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
DALResult
Adc_DalAdc_Attach(
   const char *pszArg,
   DALDEVICEID DeviceId,
   DalDeviceHandle **phDalDevice
   )
{
   DALResult nErr;
   AdcClientCtxt *pClientCtxt = NULL;

   /* Allocate memory for the client context */
   nErr = DALSYS_Malloc(sizeof(AdcClientCtxt), (void **)&pClientCtxt);
   if (DAL_SUCCESS != nErr || NULL == pClientCtxt)
   {
      return DAL_ERROR;
   }

   DALSYS_memset(pClientCtxt, 0, sizeof(AdcClientCtxt));

   if (DeviceId != 0)
   {
      nErr = DALFW_AttachToDevice(DeviceId,
                                  (DALDrvCtxt *)&gAdcDrvCtxt,
                                  (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the device ID is available */
      nErr = DALSYS_GetDALPropertyHandle(DeviceId, pClientCtxt->pAdcDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }
   else
   {
      nErr = DALFW_AttachToStringDevice(pszArg,
                                        (DALDrvCtxt *)&gAdcDrvCtxt,
                                        (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the string device name is available */
      nErr = DALSYS_GetDALPropertyHandleStr(pszArg, pClientCtxt->pAdcDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }

   Adc_InitInterface(pClientCtxt);
   (void)Adc_DalAdc_AddRef(&(pClientCtxt->DalAdcHandle));
   nErr = Adc_RegisterClient(pClientCtxt);
   if (nErr != DAL_SUCCESS)
   {
      goto error;
   }

   *phDalDevice = (DalDeviceHandle *)&(pClientCtxt->DalAdcHandle);

   return DAL_SUCCESS;

error:
   DALSYS_Free(pClientCtxt);

   return nErr;
}

