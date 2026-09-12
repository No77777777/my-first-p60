/*============================================================================
  FILE:         DalVAdcTMFwk.c

  OVERVIEW:     VADCTM DAL framework file.

  DEPENDENCIES: None

                Copyright (c) 2015, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

  $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/DalVAdcTMFwk.c#1 $$DateTime: 2019/04/24 00:03:26 $$Author: pwbldsvc $

  when        who  what, where, why
  ----------  ---  -----------------------------------------------------------
  2017-06-06  jjo  DAL framework updates.
  2015-09-14  jjo  Initial version.

============================================================================*/
/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "DALFramework.h"
#include "DALSys.h"
#include "DDIAdcDevice.h"
#include "DalVAdcTM.h"
#include "DALSys.h"

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
VAdcTMDrvCtxt gVAdcTMDrvCtxt =
{
   .VAdcTMDALVtbl = {VAdcTM_DriverInit, VAdcTM_DriverDeInit},
   .dwNumDev      = VADCTM_MAX_NUM_DEVICES,
   .dwSizeDevCtxt = sizeof(VAdcTMDevCtxt),
   .bInit         = 0,
   .dwRefs        = 0,
   .VAdcTMDevCtxt = {{0}}
};

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
static uint32
VAdcTM_DalAdcDevice_AddRef(DalAdcDeviceHandle *h)
{
   return DALFW_AddRef((DALClientCtxt *)(h->pClientCtxt));
}

/*------------------------------------------------------------------------------
Following functions are defined in DalDevice DAL Interface.
------------------------------------------------------------------------------*/
static uint32
VAdcTM_DalAdcDevice_Detach(uint32 uClientHandle, DalDeviceHandle *h)
{
   uint32 dwref;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return 0;
   }

   dwref = DALFW_Release((DALClientCtxt *)(h->pClientCtxt));

   return dwref;
}

static DALResult
VAdcTM_DalAdcDevice_Init(uint32 uClientHandle, DalDeviceHandle *h)
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_DeviceInit(pCtxt);
}

static DALResult
VAdcTM_DalAdcDevice_DeInit(uint32 uClientHandle, DalDeviceHandle *h)
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_DeviceDeInit(pCtxt);
}

static DALResult
VAdcTM_DalAdcDevice_PowerEvent(
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
VAdcTM_DalAdcDevice_Open(
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
VAdcTM_DalAdcDevice_Close(uint32 uClientHandle, DalDeviceHandle *h)
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return DAL_SUCCESS;
}

static DALResult
VAdcTM_DalAdcDevice_Info(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   DalDeviceInfo* info,
   uint32 infoSize
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   info->Version = DALADCDEVICE_INTERFACE_VERSION;
   return DAL_SUCCESS;
}

static DALResult
VAdcTM_DalAdcDevice_SysRequest(
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
Following functions are extended in DalAdcDevice Interface.
------------------------------------------------------------------------------*/
static DALResult
VAdcTM_DalAdcDevice_SetDeviceIndex(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uDeviceIndex
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_SetDeviceIndex(pCtxt,
                                uDeviceIndex);
}

static DALResult
VAdcTM_DalAdcDevice_GetInputProperties(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   const char *pChannelName,
   uint32 *puChannelIdx
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_GetChannel(pCtxt,
                            pChannelName,
                            puChannelIdx);
}

static DALResult
VAdcTM_DalAdcDevice_GetDeviceProperties(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   AdcDeviceDevicePropertiesType *pAdcDeviceProp
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_GetDeviceProperties(pCtxt, pAdcDeviceProp);
}

static DALResult
VAdcTM_DalAdcDevice_RecalibrateChannel(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uChannelIdx,
   AdcDeviceRecalibrateResultType *pAdcDeviceRecalibrateResult
   )
{
   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return DAL_ERROR;
}

static DALResult
VAdcTM_DalAdcDevice_ReadChannel(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uChannelIdx,
   AdcDeviceResultType *pAdcDeviceReadResult
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_ReadChannel(pCtxt,
                             uChannelIdx,
                             pAdcDeviceReadResult);
}

static DALResult
VAdcTM_DalAdcDevice_TMGetInputProperties(
   uint32 uClientHandle,
   DalDeviceHandle*h,
   const char *pInputName,
   uint32 *puMeasIdx
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_GetInputProperties(pCtxt,
                                    pInputName,
                                    puMeasIdx);
}

static DALResult
VAdcTM_DalAdcDevice_TMGetRange(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uMeasIdx,
   AdcDeviceTMRangeType *pAdcDeviceTMRange
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_GetRange(pCtxt,
                          uMeasIdx,
                          pAdcDeviceTMRange);
}

static DALResult
VAdcTM_DalAdcDevice_TMSetThreshold(
   uint32 uClientHandle,
   DalDeviceHandle *h,
   uint32 uMeasIdx,
   const DALSYSEventHandle hEvent,
   AdcDeviceTMThresholdType eThreshold,
   const int32 *pnThresholdDesired,
   int32 *pnThresholdSet
   )
{
   VAdcTMClientCtxt *pCtxt = h->pClientCtxt;

   if (uClientHandle != DALDEVICE_LOCAL_CLIENTID)
   {
      return DAL_ERROR_DEVICE_ACCESS_DENIED;
   }

   return VAdcTM_SetThreshold(pCtxt,
                              uMeasIdx,
                              hEvent,
                              eThreshold,
                              pnThresholdDesired,
                              pnThresholdSet);
}

static void
VAdcTM_InitInterface(VAdcTMClientCtxt *pclientCtxt)
{
   static const DalAdcDevice vtbl =
   {
      {
         VAdcTM_DalAdcDevice_Attach,
         VAdcTM_DalAdcDevice_Detach,
         VAdcTM_DalAdcDevice_Init,
         VAdcTM_DalAdcDevice_DeInit,
         VAdcTM_DalAdcDevice_Open,
         VAdcTM_DalAdcDevice_Close,
         VAdcTM_DalAdcDevice_Info,
         VAdcTM_DalAdcDevice_PowerEvent,
         VAdcTM_DalAdcDevice_SysRequest
      },
      VAdcTM_DalAdcDevice_SetDeviceIndex,
      VAdcTM_DalAdcDevice_GetInputProperties,
      VAdcTM_DalAdcDevice_GetDeviceProperties,
      VAdcTM_DalAdcDevice_RecalibrateChannel,
      VAdcTM_DalAdcDevice_ReadChannel,
      VAdcTM_DalAdcDevice_TMGetInputProperties,
      VAdcTM_DalAdcDevice_TMGetRange,
      VAdcTM_DalAdcDevice_TMSetThreshold,
   };
   /*--------------------------------------------------------------------------
   Depending upon client type setup the vtables (entry points)
   --------------------------------------------------------------------------*/
   pclientCtxt->DalAdcDeviceHandle.dwDalHandleId = DALDEVICE_INTERFACE_HANDLE_ID;
   pclientCtxt->DalAdcDeviceHandle.pVtbl = &vtbl;
   pclientCtxt->DalAdcDeviceHandle.pClientCtxt = pclientCtxt;
   pclientCtxt->DalAdcDeviceHandle.dwVtblen = sizeof(vtbl) / sizeof( void (*)(void));
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/
DALResult
VAdcTM_DalAdcDevice_Attach(
   const char *pszArg,
   DALDEVICEID DeviceId,
   DalDeviceHandle **phDalDevice
   )
{
   DALResult nErr;
   VAdcTMClientCtxt *pClientCtxt = NULL;

   /* Allocate memory for the client context */
   nErr = DALSYS_Malloc(sizeof(VAdcTMClientCtxt), (void **)&pClientCtxt);
   if (DAL_SUCCESS != nErr || NULL == pClientCtxt)
   {
      return DAL_ERROR;
   }

   DALSYS_memset(pClientCtxt, 0, sizeof(VAdcTMClientCtxt));

   if (DeviceId != 0)
   {
      nErr = DALFW_AttachToDevice(DeviceId,
                                  (DALDrvCtxt *)&gVAdcTMDrvCtxt,
                                  (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the device ID is available */
      nErr = DALSYS_GetDALPropertyHandle(DeviceId, pClientCtxt->pVAdcTMDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }
   else
   {
      nErr = DALFW_AttachToStringDevice(pszArg,
                                        (DALDrvCtxt *)&gVAdcTMDrvCtxt,
                                        (DALClientCtxt *)pClientCtxt);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }

      /* Get a handle to the properties here since the string device name is available */
      nErr = DALSYS_GetDALPropertyHandleStr(pszArg, pClientCtxt->pVAdcTMDevCtxt->hProp);
      if (nErr != DAL_SUCCESS)
      {
         goto error;
      }
   }

   VAdcTM_InitInterface(pClientCtxt);
   (void)VAdcTM_DalAdcDevice_AddRef(&(pClientCtxt->DalAdcDeviceHandle));
   *phDalDevice = (DalDeviceHandle *)&(pClientCtxt->DalAdcDeviceHandle);

   return DAL_SUCCESS;

error:
   DALSYS_Free(pClientCtxt);

   return nErr;
}

