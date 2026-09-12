/*========================================================================

This file contains osal signal2 utilities.

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Edit History

$Header:

when       who     what, where, why
--------   ---     -------------------------------------------------------
05/22/2018 rv	   Created the file.
06/06/2018 rv    Changed errors codes to AR osal.

========================================================================== */

/* =======================================================================
INCLUDE FILES FOR MODULE
========================================================================== */
#include "qurt.h"
#include "ar_osal_signal2.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include "ar_osal_heap.h"
#include <stdlib.h>

int32_t ar_osal_signal2_init(ar_osal_signal2_t signal2)
{
   return AR_EUNSUPPORTED;
}

int32_t ar_osal_signal2_deinit(ar_osal_signal2_t signal2)
{
   return AR_EUNSUPPORTED;
}

size_t ar_osal_signal2_get_size()
{
   return AR_EUNSUPPORTED;
}

int32_t ar_osal_signal2_create(ar_osal_signal2_t *signal2)
{
   /*check input argument*/
   if (NULL == signal2)
   {
      return AR_EBADPARAM;
   }

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;
   /*Allocate memory for the singal v2 object */
   qurt_signal2_t *temp = (qurt_signal2_t *)ar_heap_malloc(sizeof(qurt_signal2_t), &heap_info);
   if (NULL == temp)
   {
      return AR_EFAILED;
   }
   qurt_signal2_init(temp);
   /*Return the qurt object handle*/
   *signal2 = temp;
   return AR_EOK;
}

int32_t ar_osal_signal2_destroy(ar_osal_signal2_t signal2)
{
   /*Destroy the qurt signal */
   qurt_signal2_destroy((qurt_signal2_t *)signal2);

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   /*Deallocate the object memory */
   ar_heap_free((void *)signal2, &heap_info);

   return AR_EOK;
}

uint32_t ar_osal_signal2_wait_any(ar_osal_signal2_t signal2, uint32_t signal2_mask)
{
   /*Wait on any signal mask */
   uint32_t status = (uint32_t)qurt_signal2_wait_any((qurt_signal2_t *)signal2, signal2_mask);

   return (status & signal2_mask);
}

uint32_t ar_osal_signal2_wait_all(ar_osal_signal2_t signal2, uint32_t signal2_mask)
{
   /* Wait for all signal in the signal mask */
   uint32_t status = qurt_signal2_wait_all((qurt_signal2_t *)signal2, signal2_mask);
   return status;
}

int32_t ar_osal_signal2_set(ar_osal_signal2_t signal2, uint32_t signal2_mask)
{
   qurt_signal2_set((qurt_signal2_t *)signal2, signal2_mask);
   return AR_EOK;
}

uint32_t ar_osal_signal2_get(ar_osal_signal2_t signal2)
{
   return qurt_signal2_get((qurt_signal2_t *)signal2);
}

int32_t ar_osal_signal2_clear(ar_osal_signal2_t signal2, uint32_t signal2_mask)
{
   qurt_signal2_clear((qurt_signal2_t *)signal2, signal2_mask);
   return AR_EOK;
}
