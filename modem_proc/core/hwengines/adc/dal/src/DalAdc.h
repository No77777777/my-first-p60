#ifndef __DALADC_H__
#define __DALADC_H__
/*============================================================================
  @file DalAdc.h

  Function and data structure declarations for ADC DAL


               Copyright (c) 2008-2013, 2015, 2017 Qualcomm Technologies, Inc.
               All Rights Reserved.
               Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.mpss/10.0/hwengines/adc/dal/src/DalAdc.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DDIAdc.h"
#include "AdcBsp.h"
#include "DALFramework.h"
#include "DALQueue.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define ADC_MAX_NUM_CLIENTS 3

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
Declaring an "Adc" Driver, Device and Client Context
------------------------------------------------------------------------------*/
typedef struct AdcDrvCtxt AdcDrvCtxt;
typedef struct AdcDevCtxt AdcDevCtxt;
typedef struct AdcClientCtxt AdcClientCtxt;
typedef struct AdcDeviceDataType AdcDeviceDataType;

/*------------------------------------------------------------------------------
Declaring a private "Adc" Vtable
------------------------------------------------------------------------------*/
typedef struct AdcDALVtbl AdcDALVtbl;
struct AdcDALVtbl
{
  int (*Adc_DriverInit)(AdcDrvCtxt *);
  int (*Adc_DriverDeInit)(AdcDrvCtxt *);
};

typedef enum
{
   ADC_REQUEST_CONVERSION,
   ADC_REQUEST_CHANNEL_RECALIBRATION
} AdcDeviceRequestType;

typedef struct
{
   DALFW_Q_LinkType link;
   AdcDeviceRequestType eAdcDeviceRequest;
   DALSYSEventHandle hClientEvent;
   uint32 uDeviceIdx;
   uint32 uChannelIdx;
   AdcRequestStatusType adcRequestStatus;
} AdcDeviceQueueNodeType;

typedef enum
{
  ADC_DEVICE_NOT_AVAILABLE,
  ADC_DEVICE_AVAILABLE
} AdcDeviceStatusType;

typedef struct
{
   AdcDevCtxt *pAdcDevCtxt;               /* ADC device context */
   AdcDeviceQueueNodeType *paNodes;       /* nodes */
   DALFW_Q_Type qFree;                    /* nodes that are free */
   DALFW_Q_Type qQueued;                  /* nodes that are waiting to be processed */
   DALFW_Q_Type qPending;                 /* queue for the node that is being processed */
   DALSYSWorkLoopHandle hAdcOpWorkLoop;   /* work loop used to perform ADC operations */
   DALSYSEventHandle hAdcOpEvent;         /* work loop event to perform ADC operations */
   DALSYSSyncHandle hSync;                /* synchronization object handle */
   DALSYS_SYNC_OBJECT(syncObject);        /* syn object */
   uint32 uQueueSize;                     /* queue size */
} AdcQueueType;

typedef enum
{
   ADC_THRESHOLD_STATE_DISABLED = 0,
   ADC_THRESHOLD_STATE_THRESHOLD,
   ADC_THRESHOLD_STATE_TOLERANCE,
   ADC_THRESHOLD_STATE_TRIGGERED
} AdcThresholdStateType;

typedef struct
{
   DALSYSEventHandle hEvent;
   int32 nThreshold;
   int32 nTolerance;
   int32 nCurrentValue;
   AdcThresholdStateType eThresholdState;
} AdcThresholdConfigType;

typedef struct
{
   AdcThresholdConfigType aClientThresholds[ADC_MAX_NUM_CLIENTS][_ADC_TM_NUM_THRESHOLDS];
   AdcTMRangeType range;
} AdcMeasType;

struct AdcDeviceDataType
{
   DalDeviceHandle *phDevice;         /* DAL device handle */
   AdcMeasType *paMeas;               /* array of measurement thresholds */
   uint32 uNumChannels;               /* number of channels supported by the ADC device */
   uint32 uNumMeas;                   /* number of TM measurements supported by the ADC device */
   AdcDeviceStatusType deviceStatus;  /* device status */
   DALBOOL bDeviceIsOpen;             /* whether the device is open */
};

typedef struct
{
   uint32 uClientId;
   DALBOOL bRegistered;
   DALBOOL bEnabled;
} AdcClientInfoType;

struct AdcDevCtxt
{
   // Base Members
   uint32 dwRefs;
   DALDEVICEID DevId;
   uint32 dwDevCtxtRefIdx;
   AdcDrvCtxt *pAdcDrvCtxt;
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   uint32 Reserved[16];
   // Adc Dev state can be added by developers here
   AdcClientInfoType aClients[ADC_MAX_NUM_CLIENTS];  /* tracks client info for thresholds */
   const AdcBspType *pBsp;                /* DAL ADC BSP */
   DALSYSEventHandle hThresholdCallback;  /* event for threshold callback */
   DALSYS_EVENT_OBJECT(thresholdCbOjb);   /* object for threshold callback */
   DALSYSSyncHandle hSync;                /* synchronization object handle */
   DALSYS_SYNC_OBJECT(syncObject);        /* syn object */
   AdcDeviceDataType *paDeviceData;       /* device data array */
   AdcQueueType *paQueues;                /* array of ADC queues */
   DALBOOL bInit;                         /* whether or not the device is initialized */
};

struct AdcDrvCtxt
{
   // Base Members
   AdcDALVtbl AdcDALVtbl;
   uint32 dwNumDev;
   uint32 dwSizeDevCtxt;
   uint32 bInit;
   uint32 dwRefs;
   AdcDevCtxt AdcDevCtxt[ADC_MAX_NUM_DEVICES];
   // Adc Drv state can be added by developers here
};

/*------------------------------------------------------------------------------
Declaring a "Adc" Client Context
------------------------------------------------------------------------------*/
struct AdcClientCtxt
{
   // Base Members
   uint32 dwRefs;
   uint32 dwAccessMode;
   void *pPortCtxt;
   AdcDevCtxt *pAdcDevCtxt;
   DalAdcHandle DalAdcHandle;
   // Adc Client state can be added by developers here
   uint32 uClientId;
};

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
Functions specific to Dal interface
------------------------------------------------------------------------------*/
DALResult Adc_DalAdc_Attach(const char *, DALDEVICEID, DalDeviceHandle **);
DALResult Adc_DriverInit(AdcDrvCtxt *);
DALResult Adc_DriverDeInit(AdcDrvCtxt *);
DALResult Adc_DeviceInit(AdcClientCtxt *);
DALResult Adc_DeviceDeInit(AdcClientCtxt *);
DALResult Adc_PowerEvent(AdcClientCtxt *, DalPowerCmd, DalPowerDomain);
DALResult Adc_Open(AdcClientCtxt *, uint32);
DALResult Adc_Close(AdcClientCtxt *);
DALResult Adc_Info(AdcClientCtxt *,DalDeviceInfo *, uint32);

/*------------------------------------------------------------------------------
Functions specific to DalAdc interface
------------------------------------------------------------------------------*/
DALResult Adc_GetAdcInputProperties(AdcClientCtxt *, const char *, uint32 , AdcInputPropertiesType *);
DALResult Adc_Read(AdcClientCtxt *, const AdcInputPropertiesType *, AdcResultType *);
DALResult Adc_RequestConversion(AdcClientCtxt *, const AdcRequestParametersType *, AdcRequestStatusType *);
DALResult Adc_RequestRecalibration(AdcClientCtxt *, const AdcRequestParametersType *);
DALResult Adc_TMGetInputProperties(AdcClientCtxt *, const char *, uint32 , AdcTMInputPropertiesType *);
DALResult Adc_TMGetRange(AdcClientCtxt *, const AdcTMInputPropertiesType *, AdcTMRangeType *);
DALResult Adc_TMSetThreshold(AdcClientCtxt *, const AdcTMRequestParametersType *, AdcTMThresholdType , const int32 *, int32 *);
DALResult Adc_TMSetTolerance(AdcClientCtxt *, const AdcTMRequestParametersType *, int32, const int32 *, const int32 *);
DALResult Adc_TMSetEnableThresholds(AdcClientCtxt *, DALBOOL);
DALResult Adc_RegisterClient(AdcClientCtxt *);
DALResult Adc_DeregisterClient(AdcClientCtxt *);

#endif /* #ifndef __DALADC_H__ */

