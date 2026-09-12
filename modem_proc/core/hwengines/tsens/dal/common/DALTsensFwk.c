/*============================================================================
  FILE:         DALTsensFwk.c

  OVERVIEW:     Implementation of the TSENS DAL Framework

  DEPENDENCIES: None

                Copyright (c) 2010-2012, 2014-2015, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.


  $Header: //components/rel/core.mpss/10.0/hwengines/tsens/dal/common/DALTsensFwk.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-06  jjo  DAL framework updates.
  2015-01-29  jjo  Clean up.
  2014-07-02  jjo  String device support.
  2014-04-08  jjo  Clean up.
  2012-03-13  jdt  New version of TSENS DAL.
  2010-10-05  jdt  Implement TSENS calibration on MDM9K.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DALFramework.h"
#include "DALSys.h"
#include "DDITsens.h"
#include "DALTsens.h"

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
TsensDrvCtxt gTsensDrvCtxt =
{
   .TsensDALVtbl  = {Tsens_DriverInit, Tsens_DriverDeInit},
   .dwNumDev      = TSENS_MAX_NUM_DEVICES,
   .dwSizeDevCtxt = sizeof(TsensDevCtxt),
   .bInit         = 0,
   .dwRefs        = 0,
   .TsensDevCtxt  = {{0}}
};

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint32
Tsens_DalTsens_AddRef(DalTsensHandle *h)
{
   return DALFW_AddRef((DALClientCtxt *)(h->pClientCtxt));
}

/*------------------------------------------------------------------------------
Following functions are defined in DalDevice DAL Interface.
------------------------------------------------------------------------------*/
static uint32
Tsens_DalTsens_Detach(uint32 uClientHandle, DalDeviceHandle *h)
{
   uint32 dwref;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return 0;
   }

   /* Unregister the client */
   (void)Tsens_DeregisterClient(h->pClientCtxt);

   dwref = DALFW_Release((DALClientCtxt *)(h->pClientCtxt));
   if (0 == dwref)
   {
      /* Release the client context */
      DALSYS_Free(h->pClientCtxt);
   }
   return dwref;
}

static DALResult
Tsens_DalTsens_Init(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_DeviceInit(h->pClientCtxt);
}

static DALResult
Tsens_DalTsens_DeInit(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_DeviceDeInit(h->pClientCtxt);
}

static DALResult
Tsens_DalTsens_PowerEvent(
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

   return DAL_SUCCESS;
}

static DALResult
Tsens_DalTsens_Open(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 mode
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return DAL_SUCCESS;
}

static DALResult
Tsens_DalTsens_Close(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return DAL_SUCCESS;
}

static DALResult
Tsens_DalTsens_Info(
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

   info->Version = DALTSENS_INTERFACE_VERSION;
   return DAL_SUCCESS;
}

static DALResult
Tsens_DalTsens_SysRequest(
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
Following functions are extended in DalTsens Interface.
------------------------------------------------------------------------------*/
static DALResult
Tsens_DalTsens_GetTemp(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uSensor,
   TsensTempType *pTemp
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_GetTemp(((DalTsensHandle *)h)->pClientCtxt, uSensor, pTemp);
}

static DALResult
Tsens_DalTsens_GetTempRange(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uSensor,
   TsensTempRangeType *pTsensTempRange
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_GetTempRange(((DalTsensHandle *)h)->pClientCtxt, uSensor, pTsensTempRange);
}

static DALResult
Tsens_DalTsens_SetThreshold(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uSensor,
   TsensThresholdType eThreshold,
   int32 nDegC,
   DALSYSEventHandle hEvent
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_SetThreshold(((DalTsensHandle *)h)->pClientCtxt, uSensor, eThreshold, nDegC, hEvent);
}

static DALResult
Tsens_DalTsens_SetEnableThresholds(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   DALBOOL bEnableThresholds
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_SetEnableThresholds(((DalTsensHandle *)h)->pClientCtxt, bEnableThresholds);
}

static DALResult
Tsens_DalTsens_GetNumSensors(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 *pnNumSensors
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_GetNumSensors(((DalTsensHandle *)h)->pClientCtxt, pnNumSensors);
}

static DALResult
Tsens_DalTsens_CheckCalibration(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uSensor
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return Tsens_CheckCalibration(((DalTsensHandle *)h)->pClientCtxt, uSensor);
}

static void
Tsens_InitInterface(TsensClientCtxt *pClientCtxt)
{
   static const DalTsens vtbl =
   {
      {
         Tsens_DalTsens_Attach,
         Tsens_DalTsens_Detach,
         Tsens_DalTsens_Init,
         Tsens_DalTsens_DeInit,
         Tsens_DalTsens_Open,
         Tsens_DalTsens_Close,
         Tsens_DalTsens_Info,
         Tsens_DalTsens_PowerEvent,
         Tsens_DalTsens_SysRequest
      },
      Tsens_DalTsens_GetTemp,
      Tsens_DalTsens_GetTempRange,
      Tsens_DalTsens_SetThreshold,
      Tsens_DalTsens_SetEnableThresholds,
      Tsens_DalTsens_GetNumSensors,
      Tsens_DalTsens_CheckCalibration
   };
   /*--------------------------------------------------------------------------
   Depending upon client type setup the vtables (entry points)
   --------------------------------------------------------------------------*/
   pClientCtxt->DalTsensHandle.dwDalHandleId = DALDEVICE_INTERFACE_HANDLE_ID;
   pClientCtxt->DalTsensHandle.pVtbl = &vtbl;
   pClientCtxt->DalTsensHandle.pClientCtxt = pClientCtxt;
   pClientCtxt->DalTsensHandle.dwVtblen = sizeof(vtbl) / sizeof( void (*)(void));
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
DALResult
Tsens_DalTsens_Attach(
   const char *pszArg,
   DALDEVICEID DeviceId,
   DalDeviceHandle **phDalDevice
   )
{
   DALResult nErr;
   TsensClientCtxt *pClientCtxt = NULL;

   /* Allocate memory for the client context */
   nErr = DALSYS_Malloc(sizeof(TsensClientCtxt), (void **)&pClientCtxt);
   if (DAL_SUCCESS != nErr || NULL == pClientCtxt)
   {
      return DAL_ERROR;
   }

   DALSYS_memset(pClientCtxt, 0, sizeof(TsensClientCtxt));

   if (DeviceId != 0)
   {
      nErr = DALFW_AttachToDevice(DeviceId,
                                  (DALDrvCtxt *)&gTsensDrvCtxt,
                                  (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the device ID is available */
      nErr = DALSYS_GetDALPropertyHandle(DeviceId, pClientCtxt->pTsensDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }
   else
   {
      nErr = DALFW_AttachToStringDevice(pszArg,
                                        (DALDrvCtxt *)&gTsensDrvCtxt,
                                        (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the string device name is available */
      nErr = DALSYS_GetDALPropertyHandleStr(pszArg, pClientCtxt->pTsensDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }

   Tsens_InitInterface(pClientCtxt);
   (void)Tsens_DalTsens_AddRef(&(pClientCtxt->DalTsensHandle));
   nErr = Tsens_RegisterClient(pClientCtxt);
   if (nErr != DAL_SUCCESS)
   {
      goto error;
   }

   *phDalDevice = (DalDeviceHandle *)&(pClientCtxt->DalTsensHandle);

   return DAL_SUCCESS;

error:
   DALSYS_Free(pClientCtxt);

   return nErr;
}

