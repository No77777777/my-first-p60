/*========================================================================
@file ar_osal_mutex.cpp
This file contains utilities for using mutex functionalities.

  Copyright (c) 2019-2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */

/*========================================================================
Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.june_29_build/platform/qurt/osal/src/ar_osal_mutex.c#1
$

when       who     what, where, why
--------   ---     -------------------------------------------------------
02/04/10   mwc      Created file.
========================================================================== */
#include "ar_osal_mutex.h"
#include "qurt.h"
#include "qurt_pimutex.h"
#include "ar_osal_types.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_error.h"
#include "ar_osal_heap.h"
#include <stdlib.h>

int32_t ar_osal_mutex_init(ar_osal_mutex_t mutex)
{
   return AR_EUNSUPPORTED;
}

int32_t ar_osal_mutex_deinit(ar_osal_mutex_t mutex)
{
   return AR_EUNSUPPORTED;
}

size_t ar_osal_mutex_get_size()
{
   return 0;
}

int32_t ar_osal_mutex_create(ar_osal_mutex_t *mutex)
{
   if (NULL == mutex)
   {
      // MSG(MSG_SSID_QDSP6, DBG_FATAL_PRIO, "Invalid input argument");
      return AR_EBADPARAM;
   }

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   // TODO: heap id must be passed as argument, currently passing island by default.
   // if island is not supported by the chipset ar_heap_malloc() allocates from default heap.
   heap_info.heap_id     = AR_HEAP_ID_1;
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;
   qurt_mutex_t *temp    = (qurt_mutex_t *)ar_heap_malloc(sizeof(qurt_mutex_t), &heap_info);
   if (NULL == temp)
   {
      *mutex = NULL;
      // MSG(MSG_SSID_QDSP6, DBG_FATAL_PRIO, "Failed to allocate memory for mutex.");
      return AR_EFAILED;
   }

   // MSG(MSG_SSID_QDSP6, DBG_HIGH_PRIO, "qurt_pimutex_init");
   qurt_pimutex_init(temp);

   *mutex = (ar_osal_mutex_t)temp;
   // The desired attributes are the default attributes.

   return AR_EOK;
}

int32_t ar_osal_mutex_destroy(ar_osal_mutex_t mutex)
{
   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.heap_id     = AR_HEAP_ID_1;
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;
   qurt_mutex_t *temp    = (qurt_mutex_t *)mutex;
   qurt_pimutex_destroy(temp);
   ar_heap_free((void *)temp, &heap_info);
   return AR_EOK;
}

int32_t ar_osal_mutex_try_lock(ar_osal_mutex_t mutex)
{
   qurt_pimutex_try_lock((qurt_mutex_t *)mutex);
   return AR_EOK;
}

/** @} */ /* end_addtogroup osal_mutex */
