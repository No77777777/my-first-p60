/*
===========================================================================

FILE:   SpiDeviceOsSvc.c

DESCRIPTION:
    This file contains the implementation for the QUP OS Services 
    Interface: IQupSpiDevice 

===========================================================================

        Edit History

$Header: //components/rel/core.mpss/10.0/buses/spi/src/device/SpiDeviceOsSvc.c#1 $

When     Who    What, where, why
-------- ---    -----------------------------------------------------------
04/20/12 ddk    Added updates to enable logging.
04/19/12 ddk    Added changes for avoiding symbol match with I2C library.
03/27/12 DK     Added Physical memory request functions.
09/26/11 LK     Created

===========================================================================
        Copyright c 2011-2014 Qualcomm Technologies Incorporated.
            All Rights Reserved.
            Qualcomm Proprietary/GTDR

===========================================================================
*/

#include "SpiDeviceOsSvc.h"
#include "DALStdDef.h"
#include "DALDeviceId.h"
#include "DALSys.h"
#include "DDIInterruptController.h"
#include "SpiLog.h"

#define QUPBUSOSSVC_NUM_DEVICES_MAX       12
extern int32  aQupDalDevIds[];
extern uint32 IBUSPLAT_GetQupNum(void *QupHandle);

typedef enum SpiDeviceOs_HandleType
{
   SpiDeviceOs_EventHandleType,
   SpiDeviceOs_CritSecHandleType,
} SpiDeviceOs_HandleType;

typedef struct SpiDeviceOs_EventHandle
{
   SpiDeviceOs_HandleType hType;
   DALSYSEventHandle hQupEvents[2];
   DALSYSEventObj aQupEventObj[2];
} SpiDeviceOs_EventHandle;

typedef struct SpiDeviceOs_SyncHandle
{
   SpiDeviceOs_HandleType hType;
   DALSYSSyncHandle hDeviceSynchronization;
   DALSYS_SYNC_OBJECT(DeviceSyncObject);
} SpiDeviceOs_SyncHandle;

typedef struct SpiDeviceOs_Handle
{
   SpiDeviceOs_HandleType hType;
} SpiDeviceOs_Handle;

typedef struct SpiDeviceOs_IrqHandle
{
   ISR_HOOK IsrHook;
   IST_HOOK IstHook;
   void *pIsrData;
   void *pIstData;
   uint32 uInterruptId;
   uint32 uIrqTriggerCfg;
} SpiDeviceOs_IrqHandle;

static struct SpiDeviceOs_InterruptController
{
   SpiDeviceOs_IrqHandle *aIrqHandles[QUPBUSOSSVC_NUM_DEVICES_MAX];
   DalDeviceHandle *hInterruptCtrl;
} qupIrqCtrl;

uint32 IBUSOS_CreateEvent(IBUSOS_EVENT_HANDLE *phEvt)
{
   uint32 res;
   SpiDeviceOs_EventHandle *pEvt;
   DALResult dalRes;
   enum IBUSOS_CreatEventState
   {
      IBUSOS_CreatEventState_Reset,
      IBUSOS_CreatEventState_Malloced,
      IBUSOS_CreatEventState_1stEvt,
      IBUSOS_CreatEventState_2ndEvt,
   }crEvtState;

   crEvtState = IBUSOS_CreatEventState_Reset;
   do
   {
      res = IBUSOS_Malloc((void **)&pEvt, sizeof(SpiDeviceOs_EventHandle));
      if (res != SPIDEVICE_RES_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Malloc failure\n");
         break;
      }
      IBUSOS_Memset((void *)pEvt, 0, sizeof(SpiDeviceOs_EventHandle));

      crEvtState = IBUSOS_CreatEventState_Malloced;
      /* Create a timeout object used to timeout operations.               */
      dalRes = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT,
                                  &pEvt->hQupEvents[0],
                                  &pEvt->aQupEventObj[0]);
      if (dalRes != DAL_SUCCESS)
      {
         res = IBUSOS_ERROR_EVT_CREATE_FAILED;
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Dal failure err 0x%x\n",
                       (unsigned int)dalRes);
         break;
      }
      crEvtState = IBUSOS_CreatEventState_1stEvt;

      /* Create a wait object on which the client will wait.              */
      dalRes = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT,
                                  &pEvt->hQupEvents[1],
                                  &pEvt->aQupEventObj[1]);
      if (dalRes != DAL_SUCCESS)
      {
         SPIDEVICE_LOG(ERROR, "IBUSOS_CreateEvent: Dal failure err 0x%x\n",
                       (unsigned int)dalRes);
         res = IBUSOS_ERROR_EVT_CREATE_FAILED;
         break;
      }
      pEvt->hType = SpiDeviceOs_EventHandleType;
      crEvtState = IBUSOS_CreatEventState_2ndEvt;
   }
   while (0);

   if (res != SPIDEVICE_RES_SUCCESS)
   {
      switch (crEvtState)
      {
      case IBUSOS_CreatEventState_1stEvt:
         DALSYS_DestroyObject(&pEvt->aQupEventObj[0]);
      case IBUSOS_CreatEventState_Malloced:
         IBUSOS_Free(pEvt);
         break;
         // unreachable added to avoid compiler warnings
      case IBUSOS_CreatEventState_Reset:
      case IBUSOS_CreatEventState_2ndEvt:
         break;
      };
   } else
   {
      *phEvt = (IBUSOS_EVENT_HANDLE)pEvt;
   }

   return res;
}

uint32 IBUSOS_DestroyEvent(SpiDeviceOs_EventHandle *pEvt)
{
   return IBUSOS_Free(pEvt);
}

uint32 IBUSOS_SetEvent(IBUSOS_EVENT_HANDLE hEvent)
{
   DALResult dalRes;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvent;

   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[1], DALSYS_EVENT_CTRL_TRIGGER);
   if (dalRes != DAL_SUCCESS)
   {
      return IBUSOS_ERROR_EVT_CTRL_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_ClearEvent(IBUSOS_EVENT_HANDLE hEvt)
{
   DALResult dalRes;
   uint32 res = SPIDEVICE_RES_SUCCESS;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvt;

   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[0], DALSYS_EVENT_CTRL_RESET);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_EVT_CLEAR_FAILED;
   }
   dalRes = DALSYS_EventCtrl(pEvt->hQupEvents[1], DALSYS_EVENT_CTRL_RESET);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_EVT_CLEAR_FAILED;
   }

   return res;
}

uint32 IBUSOS_WaitEvent(IBUSOS_EVENT_HANDLE hEvent, uint32 dwMilliseconds)
{
   DALResult dalRes;
   SpiDeviceOs_EventHandle *pEvt = (SpiDeviceOs_EventHandle *)hEvent;

   dalRes = DALSYS_EventWait(pEvt->hQupEvents[1]);

   if ((DAL_SUCCESS != dalRes))
   {
      return IBUSOS_ERROR_EVT_WAIT_FAILED;
   }
#if 0
   /* Wait on event or timeout afer dwMilliseconds */
   dalRes = DALSYS_EventMultipleWait(pEvt->hQupEvents, 2,
                                     dwMilliseconds*1000, &uEvtIdx);
   if(((DAL_SUCCESS != dalRes) && (DAL_ERROR_TIMEOUT != dalRes) )
      || (uEvtIdx > 1))
   {
      return IBUSOS_ERROR_EVT_WAIT_FAILED;
   }
   if((DAL_ERROR_TIMEOUT == dalRes) || (0 == uEvtIdx))  //Timeout event.
   {
      return IBUSOS_RESULT_ERROR_TIMEOUT;
   }
#endif
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_DestroyCriticalSection(SpiDeviceOs_SyncHandle *pCritSec)
{
   uint32 res = SPIDEVICE_RES_SUCCESS;
   DALResult dalRes;

   dalRes = DALSYS_DestroyObject(pCritSec->hDeviceSynchronization);
   if (dalRes != DAL_SUCCESS)
   {
      res = IBUSOS_ERROR_SYNC_DESTROY_FAILED;
   }
   if (SPIDEVICE_RES_SUCCESS == res)
   {
      res = IBUSOS_Free(pCritSec);
   } else
   {
      IBUSOS_Free(pCritSec);
   }

   return res;
}

uint32 IBUSOS_CloseHandle(IBUSOS_EVENT_HANDLE hEvt)
{
   SpiDeviceOs_Handle *hOsEvt = (SpiDeviceOs_Handle *)hEvt;

   if (SpiDeviceOs_EventHandleType == hOsEvt->hType)
   {
      return IBUSOS_DestroyEvent((SpiDeviceOs_EventHandle *)hEvt);
   } else if (SpiDeviceOs_CritSecHandleType == hOsEvt->hType)
   {
      return IBUSOS_DestroyCriticalSection((SpiDeviceOs_SyncHandle *)hEvt);
   } else
   {
      return IBUSOS_ERROR_INVALID_HANDLE_TYPE;
   }
}

uint32 IBUSOS_Malloc(void **pBuffer, uint32 size)
{
   DALResult dalRes;
   dalRes = DALSYS_Malloc(size, pBuffer);
   if (DAL_SUCCESS != dalRes)
   {
      return IBUSOS_ERROR_MEM_MALLOC_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_Memset(void *buffer, int32 val, uint32 size)
{
   DALSYS_memset(buffer, val, size);
   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_Free(void *pBuffer)
{
   if (DAL_SUCCESS != DALSYS_Free(pBuffer))
   {
      return IBUSOS_ERROR_MEM_FREE_FAILED;
   }
   return SPIDEVICE_RES_SUCCESS;
}

static void* IBUSOS_Isr (void *pCtxt)
{
   SpiDeviceOs_IrqHandle *pIrqHandle = (SpiDeviceOs_IrqHandle *)pCtxt;

   if (NULL == pIrqHandle)
   {
      return NULL;
   }
   pIrqHandle->IsrHook(pIrqHandle->pIsrData);
   pIrqHandle->IstHook(pIrqHandle->pIstData);
   DalInterruptController_InterruptDone(qupIrqCtrl.hInterruptCtrl, pIrqHandle->uInterruptId);
   return NULL;
}

uint32
IBUSOS_RegisterISTHooks(void *QupHandle, ISR_HOOK IsrHook, void *pIsrData, IST_HOOK IstHook, void *pIstData)
{
   DALResult dalRes;
   DALSYS_PROPERTY_HANDLE_DECLARE(hProp);
   DALSYSPropertyVar PropVar;
   SpiDeviceOs_IrqHandle *pIrqHandle;
   uint32 res = SPIDEVICE_RES_SUCCESS;
   uint32 uInterruptId;
   uint32 uIrqTriggerCfg;
   uint32 uDevId = IBUSPLAT_GetQupNum(QupHandle);

   if (NULL == qupIrqCtrl.hInterruptCtrl)
   {
      dalRes =
         DAL_InterruptControllerDeviceAttach(DALDEVICEID_INTERRUPTCONTROLLER, &qupIrqCtrl.hInterruptCtrl);
      if ((DAL_SUCCESS != dalRes) || (NULL == qupIrqCtrl.hInterruptCtrl))
      {
         return IBUSOS_ERROR_REGISTER_IST_HOOKS;
      }
   }

   if (NULL == qupIrqCtrl.aIrqHandles[uDevId])
   {
      if (DAL_SUCCESS != DALSYS_GetDALPropertyHandle(aQupDalDevIds[uDevId], hProp))
      {
         return IBUSOS_ERROR_REGISTER_IST_HOOKS;
      }
      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_IRQ_ID", 0, &PropVar))
      {
         return IBUSOS_ERROR_REGISTER_IST_HOOKS;
      }
      uInterruptId = (uint32)PropVar.Val.dwVal;
      if (DAL_SUCCESS != DALSYS_GetPropertyValue(hProp, "CHIP_IRQ_TRIGGER_CFG", 0, &PropVar))
      {
         return IBUSOS_ERROR_REGISTER_IST_HOOKS;
      }
      uIrqTriggerCfg = PropVar.Val.dwVal;

      res = IBUSOS_Malloc((void **)&pIrqHandle, sizeof(SpiDeviceOs_IrqHandle));
      if (SPIDEVICE_RES_SUCCESS != res)
      {
         return res;
      }
      IBUSOS_Memset((void *)pIrqHandle, 0, sizeof(SpiDeviceOs_IrqHandle));
      pIrqHandle->uInterruptId = uInterruptId;
      pIrqHandle->uIrqTriggerCfg = uIrqTriggerCfg;
      qupIrqCtrl.aIrqHandles[uDevId] = pIrqHandle;
   } else
   {
      pIrqHandle = qupIrqCtrl.aIrqHandles[uDevId];
   }
   pIrqHandle->IsrHook = IsrHook;
   pIrqHandle->IstHook = IstHook;
   pIrqHandle->pIsrData = pIsrData;
   pIrqHandle->pIstData = pIstData;
   dalRes = DalInterruptController_RegisterISR(
      qupIrqCtrl.hInterruptCtrl,
      pIrqHandle->uInterruptId,
      IBUSOS_Isr,
      (DALISRCtx)pIrqHandle,
      pIrqHandle->uIrqTriggerCfg);
   if (dalRes != DAL_SUCCESS)
   {
      // the context (pIrqHandle) never gets deallocated it is part of the data
      return IBUSOS_ERROR_REGISTER_IST_HOOKS;
   }

   return SPIDEVICE_RES_SUCCESS;
}

uint32 IBUSOS_UnRegisterISTHooks(void *QupHandle)
{
   return SPIDEVICE_RES_SUCCESS;
}

void IBUSOS_BusyWait(uint32 uTimeMicrosec)
{
   DALSYS_BusyWait(uTimeMicrosec);
}

uint32 IBUSOS_PhysMemAlloc (uint32 uLen, void **phMem, void **ppVirtAddr, void **ppPhysAddr)
{
   uint32 result = SPIDEVICE_RES_SUCCESS;
   int32 res;
   DALSYSMemInfo MemInfo;

   do
   {
      if ((NULL == ppPhysAddr) || (NULL == ppVirtAddr))
      {
         result = IBUSOS_ERROR_NULL_PTR;
         break;
      }

      res = DALSYS_MemRegionAlloc(DALSYS_MEM_PROPS_UNCACHED | DALSYS_MEM_PROPS_PHYS_CONT,
         DALSYS_MEM_ADDR_NOT_SPECIFIED,DALSYS_MEM_ADDR_NOT_SPECIFIED, uLen, phMem, NULL);

      if (res != DAL_SUCCESS)
      {
         result = IBUSOS_ERROR_MEM_MALLOC_FAILED;
         break;
      }

      res = DALSYS_MemInfo(*phMem, &MemInfo);

      if (res != DAL_SUCCESS)
      {
         result = IBUSOS_ERROR_MEM_MALLOC_FAILED;
         break;
      }

      *ppVirtAddr = (void *)MemInfo.VirtualAddr;
      *ppPhysAddr = (void *)MemInfo.PhysicalAddr;
   }
   while (0);
   return result;
}

uint32 IBUSOS_PhysMemFree(void *phMem)
{
   uint32 result = SPIDEVICE_RES_SUCCESS;

   do
   {
      if (NULL == phMem)
      {
         result = IBUSOS_ERROR_NULL_PTR;
         break;
      }

      if (DAL_SUCCESS != DALSYS_DestroyObject(phMem))
      {
         result = IBUSOS_ERROR_MEM_FREE_FAILED;
         break;
      }
   }
   while (0);
   return result;
}

void IBUSOS_Memcpy(void *dst, void *src, uint32 size)
{
   memcpy(dst, src, size);
}
