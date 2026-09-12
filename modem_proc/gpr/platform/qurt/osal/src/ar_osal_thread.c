/*========================================================================

*/ /** @file ar_osal_thread.cpp
This file contains utilities for threads.

Copyright�(c)�2018-2020, 2021�Qualcomm�Technologies,�Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*====================================================================== */

/*========================================================================
Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.june_29_build/platform/qurt/osal/src/ar_osal_thread.c#1
$

when       who        what, where, why
--------   ---        -------------------------------------------------------
04/19/18   sranumul   created file with converged posal thread API.

========================================================================== */

/* =======================================================================
INCLUDE FILES FOR MODULE
========================================================================== */
#include "qurt.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_thread.h"
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include "ar_osal_heap.h"
#include <stdlib.h>

/* =======================================================================
**                          Local Definitions
** ======================================================================= */

/** Internal thread argument structyre
 **/
typedef struct
{
   qurt_thread_t tid;
   uint32_t *    arg;
   void (*pfStartRoutine)(void *);
   void *stack_ptr;
} _thread_args_t;

static void ar_osal_thread_util_stub(void *arg);

/* =======================================================================
**                          Function Definitions
** ======================================================================= */

int32_t ar_osal_thread_attr_init(ar_osal_thread_attr_t *input_attr_ptr)
{
   /* Initialize to the default attributes for the thread*/
   input_attr_ptr->thread_name = NULL;
   input_attr_ptr->priority    = QURT_THREAD_ATTR_PRIORITY_DEFAULT;
   input_attr_ptr->stack_size  = 0;

   return AR_EOK;
}

int32_t ar_osal_thread_create(ar_osal_thread_t *           ar_osal_thread_ptr,
                              ar_osal_thread_attr_t *      thread_attr_ptr,
                              ar_osal_thread_start_routine osal_thread_start,
                              void *                       osal_thread_param)
{
   qurt_thread_attr_t qurt_attr;
   int32_t            result;

   if (NULL == ar_osal_thread_ptr)
   {
      return AR_EBADPARAM;
   }

   /* Check if there are thread attributes set */
   if (0 == thread_attr_ptr->stack_size || NULL == thread_attr_ptr->thread_name || thread_attr_ptr->priority < 0)
   {
      // AR_MSG(DBG_ERROR_PRIO, "Invalid input thread attributes");
      return AR_EBADPARAM;
   }

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   /*Allocate memory for the stack pointer */
   void *stack_ptr = ar_heap_malloc(thread_attr_ptr->stack_size, &heap_info);
   if (NULL == stack_ptr)
   {
      return AR_EFAILED;
   }
   ar_mem_set(stack_ptr, 0, thread_attr_ptr->stack_size);

   /*Allocate memory for the object*/
   _thread_args_t *thrd_obj_ptr = (_thread_args_t *)ar_heap_malloc(sizeof(_thread_args_t), &heap_info);
   if (NULL == thrd_obj_ptr)
   {
      ar_heap_free(stack_ptr, &heap_info);
      *ar_osal_thread_ptr = NULL;
      return AR_EFAILED;
   }
   /*Assign return object */
   *ar_osal_thread_ptr = (ar_osal_thread_t)thrd_obj_ptr;

   /*Store cache pointer in the thread object */
   thrd_obj_ptr->stack_ptr = stack_ptr;

   /*Initialize qurt thread attributes */
   qurt_thread_attr_init(&qurt_attr);
   qurt_thread_attr_set_stack_size(&qurt_attr, thread_attr_ptr->stack_size);
   qurt_thread_attr_set_stack_addr(&qurt_attr, thrd_obj_ptr->stack_ptr);
   qurt_thread_attr_set_priority(&qurt_attr, (unsigned short)thread_attr_ptr->priority);
   qurt_thread_attr_set_bus_priority(&qurt_attr, QURT_THREAD_BUS_PRIO_ENABLED);
   qurt_thread_attr_set_name(&qurt_attr, (char *)thread_attr_ptr->thread_name);

   /*Create a local thread variable*/
   thrd_obj_ptr->arg            = (uint32_t *)osal_thread_param;
   thrd_obj_ptr->pfStartRoutine = osal_thread_start;

   result = qurt_thread_create(&thrd_obj_ptr->tid, &qurt_attr, ar_osal_thread_util_stub, (void *)thrd_obj_ptr);
   if (QURT_EOK != result)
   {
      return AR_EFAILED;
   }

   return AR_EOK;
}

static void ar_osal_thread_util_stub(void *arg)
{

   _thread_args_t *args_ptr = (_thread_args_t *)arg;

   /*Call user thread routine */
   args_ptr->pfStartRoutine(args_ptr->arg);

   qurt_thread_exit(0);

   return;
}

int64_t ar_osal_thread_get_id()
{
   return (uint64_t)qurt_thread_get_id();
}

int32_t ar_osal_thread_join_destroy(ar_osal_thread_t thread)
{
   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag                      = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes              = AR_HEAP_ALIGN_8_BYTES;
   int             osal_thread_status = 0;
   _thread_args_t *args_ptr           = (_thread_args_t *)thread;

   (void)qurt_thread_join((qurt_thread_t)args_ptr->tid, &osal_thread_status);

   /*Deallocate stack pointer */
   ar_heap_free(args_ptr->stack_ptr, &heap_info);

   /*free thread object pointer*/
   ar_heap_free(args_ptr, &heap_info);

   return AR_EOK;
}

int32_t ar_osal_thread_get_priority(ar_osal_thread_t thread, int32_t *get_priority)
{
   if (NULL == thread)
   {
      return AR_EFAILED;
   }
   _thread_args_t *args_ptr = (_thread_args_t *)thread;

   *get_priority = 255 - qurt_thread_get_priority(args_ptr->tid);
   return AR_EOK;
}

int32_t ar_osal_thread_set_priority(ar_osal_thread_t thread, int32_t set_priority)
{
   if (NULL == thread)
   {
      return AR_EOK;
   }

   _thread_args_t *args_ptr = (_thread_args_t *)thread;

   qurt_thread_set_priority(args_ptr->tid, 255 - set_priority);

   return AR_EOK;
}

int32_t ar_osal_thread_self_get_priority(int32_t *get_priority)
{
   *get_priority = 255 - qurt_thread_get_priority(qurt_thread_get_id());
   return AR_EOK;
}

int32_t ar_osal_thread_self_set_priority(int32_t set_priority)
{
   qurt_thread_set_priority(qurt_thread_get_id(), 255 - set_priority);

   return AR_EOK;
}
