/*==================================================================================================

FILE: tal_os.c

DESCRIPTION: This module defines the reference implementation of the OS services for the
             Target Abstraction Layer.  It uses the DALSYS APIs.

                       Copyright (c) 2014 Qualcomm Technologies Incorporated
                                        All Rights Reserved
                                     Qualcomm Proprietary/GTDR

==================================================================================================*/
/*==================================================================================================

$Header: //components/rel/core.mpss/10.0/wiredconnectivity/uart/uart_blsp/tal/src/tal_os.c#1 $

==================================================================================================*/
/*==================================================================================================
                                            DESCRIPTION
====================================================================================================

GLOBAL FUNCTIONS:
   tal_os_event_create
   tal_os_event_destroy
   tal_os_event_reset
   tal_os_event_set
   tal_os_event_wait
   tal_os_free
   tal_os_get_property
   tal_os_log
   tal_os_malloc
   tal_os_memcpy
   tal_os_phys_alloc
   tal_os_phys_dealloc
   tal_os_phys_map
   tal_os_phys_unmap
   tal_os_sleep_ms
   tal_os_sync_create
   tal_os_sync_destroy
   tal_os_sync_enter
   tal_os_sync_exit

==================================================================================================*/
/*==================================================================================================
                                           INCLUDE FILES
==================================================================================================*/

#include "DALSys.h"
#include "tal.h"
#include <stdarg.h>
#include "stringl/stringl.h"
#include "uart_device_props.h"
#include "DALDeviceId.h"

/*==================================================================================================
                                     LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
//static DALResult tal_os_dummy_attach(const char *arg, DALDEVICEID device_id, DalDeviceHandle **phandle);

/*==================================================================================================
                                       LOCAL/GLOBAL VARIABLES
==================================================================================================*/

static DALDEVICEID device_array[] = { DALDEVICEID_UARTLG_DEVICE_1,
                                      DALDEVICEID_UARTLG_DEVICE_2,
                                      DALDEVICEID_UARTLG_DEVICE_3,
                                      DALDEVICEID_UARTDM_DEVICE_1,
                                      DALDEVICEID_UARTDM_DEVICE_2,
                                      DALDEVICEID_UARTGSBI_DEVICE_0,
                                      DALDEVICEID_UARTGSBI_DEVICE_1,
                                      DALDEVICEID_UARTGSBI_DEVICE_2,
                                      DALDEVICEID_UARTGSBI_DEVICE_3,
                                      DALDEVICEID_UARTGSBI_DEVICE_4,
                                      DALDEVICEID_UARTGSBI_DEVICE_5,
                                      DALDEVICEID_UARTGSBI_DEVICE_6,
                                      DALDEVICEID_UARTGSBI_DEVICE_7,
                                      DALDEVICEID_UARTGSBI_DEVICE_8,
                                      DALDEVICEID_UARTGSBI_DEVICE_9,
                                      DALDEVICEID_UARTGSBI_DEVICE_10,
                                      DALDEVICEID_UARTGSBI_DEVICE_11,
                                      DALDEVICEID_UARTGSBI_DEVICE_12,
                                      DALDEVICEID_UARTBAM_DEVICE_1,
                                      DALDEVICEID_UARTBAM_DEVICE_2,
                                      DALDEVICEID_UARTBAM_DEVICE_3,
                                      DALDEVICEID_UARTBAM_DEVICE_4,
                                      DALDEVICEID_UARTBAM_DEVICE_5,
                                      DALDEVICEID_UARTBAM_DEVICE_6,
                                      DALDEVICEID_UARTBAM_DEVICE_7,
                                      DALDEVICEID_UARTBAM_DEVICE_8,
                                      DALDEVICEID_UARTBAM_DEVICE_9,
                                      DALDEVICEID_UARTBAM_DEVICE_10,
                                      DALDEVICEID_UARTBAM_DEVICE_11,
                                      DALDEVICEID_UARTBAM_DEVICE_12,
                                      DALDEVICEID_UARTCXM,
                                    };

#define NUM_DEVICES (sizeof(device_array)/sizeof(device_array[0]))

// This global variable is exposed to the DAL framework.  It specifies the devices
// whose attach requests should be routed to this driver.

const DALREG_DriverInfo
DALUart_DriverInfo = { NULL, NUM_DEVICES, device_array };

/*==================================================================================================
                                          LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                          GLOBAL FUNCTIONS
==================================================================================================*/
/*==================================================================================================

FUNCTION: tal_os_event_create

==================================================================================================*/
void *tal_os_event_create(void)
{
   DALSYSEventHandle event_handle;
   DALResult result;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_CLIENT_DEFAULT, &event_handle, NULL);
   if (result != DAL_SUCCESS) { return(NULL); }

   return(event_handle);
}

/*==================================================================================================

FUNCTION: tal_os_event_destroy

==================================================================================================*/
void tal_os_event_destroy(void *event_handle)
{
   DALSYS_DestroyObject(event_handle);
}

/*==================================================================================================

FUNCTION: tal_os_event_reset

==================================================================================================*/
void tal_os_event_reset(void *event_handle)
{
   DALSYS_EventCtrl(event_handle, DALSYS_EVENT_CTRL_RESET);
}

/*==================================================================================================

FUNCTION: tal_os_event_set

==================================================================================================*/
void tal_os_event_set(void *event_handle)
{
   DALSYS_EventCtrl(event_handle, DALSYS_EVENT_CTRL_TRIGGER);
}

/*==================================================================================================

FUNCTION: tal_os_event_wait

==================================================================================================*/
TAL_RESULT tal_os_event_wait(void *event_handle, uint32 timeout_ms)
{
   DALSYSEventHandle timer_event = NULL;
   DALSYSEventHandle event_array[2];
   uint32 timeout_us;
   uint32 triggered_event;
   DALResult result;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT, &timer_event, NULL);
   if (result != DAL_SUCCESS) { return(TAL_ERROR); }

   event_array[0] = event_handle;
   event_array[1] = timer_event;

   if (timeout_ms == 0xFFFFFFFF) { timeout_us = DALSYS_EVENT_TIMEOUT_INFINITE; }
   else                          { timeout_us = timeout_ms * 1000; }

   result = DALSYS_EventMultipleWait(event_array, 2, timeout_us, &triggered_event);
   DALSYS_DestroyObject(timer_event);

   return (result == DAL_SUCCESS)       ? (TAL_SUCCESS) :
          (result == DAL_ERROR_TIMEOUT) ? (TAL_TIMEOUT) :
                                          (TAL_ERROR);
}

/*==================================================================================================

FUNCTION: tal_os_free

==================================================================================================*/
void tal_os_free(void *ptr)
{
   DALSYS_Free(ptr);
}

/*==================================================================================================

FUNCTION: tal_os_get_property

==================================================================================================*/
TAL_RESULT tal_os_get_property(void *device_id, const char *name, uint32 *value)
{
   DALSYS_PROPERTY_HANDLE_DECLARE(handle);
   DALSYSPropertyVar property_variable;
   DALResult result;

   result = DALSYS_GetDALPropertyHandle((DALDEVICEID)device_id, handle);
   if (result != DAL_SUCCESS) { return(TAL_ERROR); }

   result = DALSYS_GetPropertyValue(handle, name, 0, &property_variable);
   if (result != DAL_SUCCESS) { return(TAL_ERROR); }

   if (property_variable.dwType != DALSYS_PROP_TYPE_UINT32) { return(TAL_ERROR); }
   if (property_variable.dwLen  != sizeof(uint32))          { return(TAL_ERROR); }

   *value = property_variable.Val.dwVal;
   return(TAL_SUCCESS);
}

/*==================================================================================================

FUNCTION: tal_os_log

==================================================================================================*/
void tal_os_log(void *device_id, TAL_OS_FLAG flag, const char *fmt, ...)
{
   va_list ap;
   char msg[512];
   uint32 type;

   // This implementation should work in any environment where DAL is present.  However,
   // it is inefficient because it performs the formatting twice (once here and again in
   // DALSYS_LogEvent).  Also the formatting here is done whether logs are enabled or not.
   // Vivek plans to replace this with a more efficient implementation.

   switch (flag)
   {
      case ERROR:   type = DALSYS_LOGEVENT_ERROR;   break;
      case WARNING: type = DALSYS_LOGEVENT_WARNING; break;
      case INFO:    type = DALSYS_LOGEVENT_INFO;    break;
      default:      type = DALSYS_LOGEVENT_INFO;    break;
   }

   va_start(ap, fmt);
   vsnprintf(msg, sizeof(msg), fmt, ap);
   va_end(ap);

   DALSYS_LogEvent((DALDEVICEID)device_id, type, "%s", msg);
}

/*==================================================================================================

FUNCTION: tal_os_malloc

==================================================================================================*/
void *tal_os_malloc(uint32 size)
{
   void *ptr;

   return (DALSYS_Malloc(size, &ptr) == DAL_SUCCESS) ? (ptr) : (NULL);
}

/*==================================================================================================

FUNCTION: tal_os_memcpy

==================================================================================================*/
void tal_os_memcpy(void *dst, const void *src, uint32 len)
{
   memscpy(dst, len, src, len);
}

/*==================================================================================================

FUNCTION: tal_os_phys_alloc

==================================================================================================*/
TAL_OS_PHYS *tal_os_phys_alloc(uint32 size)
{
   TAL_OS_PHYS *mem_object;
   DALSYSMemHandle mem_handle;
   DALSYSMemInfo mem_info;
   DALResult result;

   result = DALSYS_Malloc(sizeof(TAL_OS_PHYS), (void **)&mem_object);
   if (result != DAL_SUCCESS) { return(NULL); }

   result = DALSYS_MemRegionAlloc(DALSYS_MEM_PROPS_UNCACHED | DALSYS_MEM_PROPS_PHYS_CONT,
                                  DALSYS_MEM_ADDR_NOT_SPECIFIED,
                                  DALSYS_MEM_ADDR_NOT_SPECIFIED,
                                  size, &mem_handle, NULL);

   if (result != DAL_SUCCESS)
   {
      DALSYS_Free(mem_object);
      return(NULL);
   }

   result = DALSYS_MemInfo(mem_handle, &mem_info);

   if (result != DAL_SUCCESS)
   {
      DALSYS_DestroyObject(mem_handle);
      DALSYS_Free(mem_object);
      return(NULL);
   }

   mem_object->handle    = mem_handle;
   mem_object->phys_addr = (void *)mem_info.PhysicalAddr;
   mem_object->virt_addr = (void *)mem_info.VirtualAddr;
   mem_object->size      = mem_info.dwLen;

   return(mem_object);
}

/*==================================================================================================

FUNCTION: tal_os_phys_dealloc

==================================================================================================*/
void tal_os_phys_dealloc(TAL_OS_PHYS *mem_object)
{
   DALSYS_DestroyObject(mem_object->handle);
   DALSYS_Free(mem_object);
}

/*==================================================================================================

FUNCTION: tal_os_phys_map

==================================================================================================*/
TAL_OS_PHYS *tal_os_phys_map(void *phys_addr, uint32 size)
{
   TAL_OS_PHYS *mem_object;
   DALSYSMemHandle mem_handle;
   DALSYSMemInfo mem_info;
   DALResult result;

   result = DALSYS_Malloc(sizeof(TAL_OS_PHYS), (void **)&mem_object);
   if (result != DAL_SUCCESS) { return(NULL); }

   result = DALSYS_MemRegionAlloc(DALSYS_MEM_PROPS_HWIO,
                                  DALSYS_MEM_ADDR_NOT_SPECIFIED,
                                  (uint32)phys_addr,
                                  size, &mem_handle, NULL);

   if (result != DAL_SUCCESS)
   {
      DALSYS_Free(mem_object);
      return(NULL);
   }

   result = DALSYS_MemInfo(mem_handle, &mem_info);

   if (result != DAL_SUCCESS)
   {
      DALSYS_DestroyObject(mem_handle);
      DALSYS_Free(mem_object);
      return(NULL);
   }

   mem_object->handle    = mem_handle;
   mem_object->phys_addr = (void *)mem_info.PhysicalAddr;
   mem_object->virt_addr = (void *)mem_info.VirtualAddr;
   mem_object->size      = mem_info.dwLen;

   return(mem_object);
}

/*==================================================================================================

FUNCTION: tal_os_phys_unmap

==================================================================================================*/
void tal_os_phys_unmap(TAL_OS_PHYS *mem_object)
{
   DALSYS_DestroyObject(mem_object->handle);
   DALSYS_Free(mem_object);
}

/*==================================================================================================

FUNCTION: tal_os_sleep_ms

==================================================================================================*/
void tal_os_sleep_ms(uint32 duration)
{
   DALSYSEventHandle timer_event;
   uint32 triggered_event;
   DALResult result;

   result = DALSYS_EventCreate(DALSYS_EVENT_ATTR_TIMEOUT_EVENT |
                               DALSYS_EVENT_ATTR_NON_DEFERRED, &timer_event, NULL);
   if (result != DAL_SUCCESS) { return; }

   // multiply duration by 1000 to convert from milliseconds to microseconds
   result = DALSYS_EventMultipleWait(&timer_event, 1, duration * 1000, &triggered_event);
   DALSYS_DestroyObject(timer_event);
}

/*==================================================================================================

FUNCTION: tal_os_sync_create

==================================================================================================*/
void *tal_os_sync_create(void)
{
   DALSYSSyncHandle sync_handle;
   DALResult result;

   result = DALSYS_SyncCreate(DALSYS_SYNC_ATTR_RESOURCE, &sync_handle, NULL);
   if (result != DAL_SUCCESS) { return(NULL); }

   return(sync_handle);
}

/*==================================================================================================

FUNCTION: tal_os_sync_destroy

==================================================================================================*/
void tal_os_sync_destroy(void *sync_handle)
{
   DALSYS_DestroyObject(sync_handle);
}

/*==================================================================================================

FUNCTION: tal_os_sync_enter

==================================================================================================*/
void tal_os_sync_enter(void *sync_handle)
{
   DALSYS_SyncEnter(sync_handle);
}

/*==================================================================================================

FUNCTION: tal_os_sync_exit

==================================================================================================*/
void tal_os_sync_exit(void *sync_handle)
{
   DALSYS_SyncLeave(sync_handle);
}
