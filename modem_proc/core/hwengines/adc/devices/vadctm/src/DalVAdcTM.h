#ifndef __DALVADCTM_H__
#define __DALVADCTM_H__
/*============================================================================
  @file DalVAdcTM.h

  Function and data structure declarations for VADCTM DAL


                Copyright (c) 2008-2015, 2017 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/adc/devices/vadctm/src/DalVAdcTM.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DDIAdcDevice.h"
#include "VAdcTMBsp.h"
#include "VAdcTMHal.h"
#include "VAdcTMLog.h"
#include "VAdcCommon.h"
#include "AdcScalingUtil.h"
#include "AdcSpmi.h"
#include "pm_version_p.h"
#include "pm_gpio_p.h"


/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define VADCTM_MAX_NUM_MEAS 8

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef enum
{
   VADCTM_PULLUP_STATE_LOW = 0,
   VADCTM_PULLUP_STATE_MED,
   VADCTM_PULLUP_STATE_HIGH
} VAdcTMPullUpStateType;

typedef enum
{
   VADCTM_THRESHOLD_STATE_DISABLED = 0,
   VADCTM_THRESHOLD_STATE_THRESHOLD,
   VADCTM_THRESHOLD_STATE_TRIGGERED
} VAdcTMThresholdStateType;

typedef struct
{
   DALSYSEventHandle hEvent;
   VAdcTMThresholdStateType eThresholdState;
   int32 nPhysicalDesired;
   uint32 uCode;
   int32 nPhysicalMonitored;
} VAdcTMThresholdRequestType;

typedef struct
{
   VAdcTMThresholdRequestType aClientThresholds[ADC_DEVICE_TM_NUM_THRESHOLDS];
   uint32 uLatestCode;
   int32 nCurrentValue;
   VAdcTMPullUpStateType ePullUpState;
   DALBOOL bPhysicalInverseToCode;
} VAdcTMMeasurementType;

/*
 * Declaring a "VAdcTM" Driver, device and client context
 */
typedef struct VAdcTMDrvCtxt VAdcTMDrvCtxt;
typedef struct VAdcTMDevCtxt VAdcTMDevCtxt;
typedef struct VAdcTMClientCtxt VAdcTMClientCtxt;

/*
 * Declaring a private "VAdcTM" Vtable
 */
typedef struct VAdcTMDALVtbl VAdcTMDALVtbl;
struct VAdcTMDALVtbl
{
  int (*VAdcTM_DriverInit)(VAdcTMDrvCtxt *);
  int (*VAdcTM_DriverDeInit)(VAdcTMDrvCtxt *);
};

struct VAdcTMDevCtxt
{
   // Base Members
   uint32 dwRefs;
   DALDEVICEID DevId;
   uint32 dwDevCtxtRefIdx;
   VAdcTMDrvCtxt *pVAdcTMDrvCtxt;
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   uint32 Reserved[16];
   // VAdcTM Dev state can be added by developers here
   VAdcTMDebugInfoType debug;                               /* debug structure */
   VAdcTMMeasurementType aThresholds[VADCTM_MAX_NUM_MEAS];  /* threshold array */
   AdcSpmiCtxtType spmiCtxt;                                /* SPMI wrapper context */
   VAdcTMHalInterfaceType iVAdcTMHalInterface;              /* HAL interface */
   VAdcTMRevisionInfoType revisionInfo;                     /* revision info */
   pm_device_info_type pmicDeviceInfo;                      /* PMIC device info */
   const VAdcTMBspType *pBsp;                               /* pointer to the BSP */
   DALSYSSyncHandle hSync;                                  /* synchronization object */
   DALSYS_SYNC_OBJECT(syncObject);                          /* synchronization object */
   DALSYSWorkLoopHandle hThresholdWorkLoop;                 /* work loop handle */
   DALSYSEventHandle hWorkLoopEvent;                        /* event to trigger work loop */
   DALSYSEventHandle hSignalEvent;                          /* signal set by the interrupt */
   DALSYS_EVENT_OBJECT(signalEventObject);                  /* signal set by the interrupt */
   uint32 uDeviceIndex;                                     /* device index */
   uint32 uLowMeasEnMask;                                   /* mask of which low meas are enabled */
   uint32 uHighMeasEnMask;                                  /* mask of which high meas are enabled */
   boolean bHardwareSupported;                              /* flag to indicate if the hardware is supported */
};

struct VAdcTMDrvCtxt
{
   // Base Members
   VAdcTMDALVtbl VAdcTMDALVtbl;
   uint32 dwNumDev;
   uint32 dwSizeDevCtxt;
   uint32 bInit;
   uint32 dwRefs;
   VAdcTMDevCtxt VAdcTMDevCtxt[VADCTM_MAX_NUM_DEVICES];
   // VAdcTM Drv state can be added by developers here
};

/*
 * Declaring a "VAdcTM" Client Context
 */
struct VAdcTMClientCtxt
{
   // Base Members
   uint32 dwRefs;
   uint32 dwAccessMode;
   void *pPortCtxt;
   VAdcTMDevCtxt *pVAdcTMDevCtxt;
   DalAdcDeviceHandle DalAdcDeviceHandle;
   // VAdc Client state can be added by developers here
};

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
/* Functions specific to DAL */
DALResult VAdcTM_DalAdcDevice_Attach(const char *, DALDEVICEID, DalDeviceHandle **);
DALResult VAdcTM_DriverInit(VAdcTMDrvCtxt *);
DALResult VAdcTM_DriverDeInit(VAdcTMDrvCtxt *);
DALResult VAdcTM_DeviceInit(VAdcTMClientCtxt *);
DALResult VAdcTM_DeviceDeInit(VAdcTMClientCtxt *);

/* Functions specific to DalAdcDevice interface */
DALResult VAdcTM_SetDeviceIndex(VAdcTMClientCtxt *, uint32);
DALResult VAdcTM_GetDeviceProperties(VAdcTMClientCtxt *, AdcDeviceDevicePropertiesType *);
DALResult VAdcTM_GetChannel(VAdcTMClientCtxt *, const char *, uint32 *);
DALResult VAdcTM_ReadChannel(VAdcTMClientCtxt *, uint32, AdcDeviceResultType *);
DALResult VAdcTM_RecalibrateChannel(VAdcTMClientCtxt *, uint32, AdcDeviceRecalibrateResultType *);
DALResult VAdcTM_GetInputProperties(VAdcTMClientCtxt *, const char *, uint32 *);
DALResult VAdcTM_GetRange(VAdcTMClientCtxt *, uint32, AdcDeviceTMRangeType *);
DALResult VAdcTM_SetThreshold(VAdcTMClientCtxt *pCtxt, uint32, const DALSYSEventHandle, AdcDeviceTMThresholdType, const int32 *, int32 *);

#endif /* #ifndef __DALVADC_H__ */

