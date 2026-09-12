/*========================================================================
ar_osal_signal1.cpp

This file contains signals utilities.

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.june_29_build/platform/qurt/osal/src/ar_osal_signal.c#1
$

when       who     what, where, why
--------   ---     -------------------------------------------------------
05/21/2018  rv    Created the file.
========================================================================== */

/* =======================================================================
INCLUDE FILES FOR MODULE
========================================================================== */
#include "qurt.h"
#include "ar_osal_signal.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include "ar_osal_heap.h"
#include <stdlib.h>

/*--------------------------------------------------------------*/
/* Macro definitions                                            */
/* -------------------------------------------------------------*/
#define AR_OSAL_SINGLE_SIGNAL_MASK 0x1

/* =======================================================================
**                          Function Definitions
** ======================================================================= */

int32_t ar_osal_signal_init(ar_osal_signal_t signal)
{
   return AR_EUNSUPPORTED;
}

int32_t ar_osal_signal_deinit(ar_osal_signal_t signal)
{
   return AR_EUNSUPPORTED;
}

size_t ar_osal_signal_get_size()
{
   return 0;
}

int32_t ar_osal_signal_create(ar_osal_signal_t *signal)
{
   if (NULL == signal)
   {
      return AR_EBADPARAM;
   }

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;
   /* Allocate memory for the object */
   qurt_signal_t *temp = (qurt_signal_t *)ar_heap_malloc(sizeof(qurt_signal_t), &heap_info);
   if (NULL == temp)
   {
      *signal = NULL;
      return AR_EFAILED;
   }

   /*Initialize the qurt signal */
   qurt_signal_init((qurt_signal_t *)temp);
   *signal = temp;
   return AR_EOK;
}

int32_t ar_osal_signal_destroy(ar_osal_signal_t signal)
{
   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;
   /*Destroy the signal and de allocate the memory */
   qurt_signal_destroy((qurt_signal_t *)signal);
   ar_heap_free((void *)signal, &heap_info);
   return AR_EOK;
}

int32_t ar_osal_signal_set(ar_osal_signal_t signal)
{
   /*This is a single singal usage so use only the LSB bit set/clear the signal*/
   qurt_signal_set((qurt_signal_t *)signal, AR_OSAL_SINGLE_SIGNAL_MASK);
   return AR_EOK;
}

int32_t ar_osal_signal_clear(ar_osal_signal_t signal)
{
   /*This is a single singal usage so use only the LSB bit set/clear the signal*/
   qurt_signal_clear((qurt_signal_t *)signal, AR_OSAL_SINGLE_SIGNAL_MASK);
   return AR_EOK;
}

int32_t ar_osal_signal_wait(ar_osal_signal_t signal)
{
   /*This is a single singal usage so use only the LSB bit set/clear the signal*/
   qurt_signal_wait_all((qurt_signal_t *)signal, AR_OSAL_SINGLE_SIGNAL_MASK);
   return AR_EOK;
}

int32_t ar_osal_signal_timedwait(ar_osal_signal_t signal, int64_t timeout_in_nsec)
{
   /*There is no timed wait in qurt, so this does a standard wait for now */
   qurt_signal_wait_all((qurt_signal_t *)signal, AR_OSAL_SINGLE_SIGNAL_MASK);
   return AR_EOK;
}
