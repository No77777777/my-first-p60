/**
 *=============================================================================
 * \file ar_osal_heap.c
 * Copyright (c) 2019-2021 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.

  when       who     what, where, why
  --------   ---     -------------------------------------------------------
  11/11/2019  np    Created the file.
 *============================================================================= */
#include "ar_osal_heap.h"
#include "ar_osal_error.h"
#include "ar_osal_mem_op.h"
#include "qurt.h"
#include <stdlib.h>
#include "posal.h"

/**
 * \brief ar_heap_init
 *        initialize heap memory interface.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_heap_init(void)
{
   return AR_EOK;
}

/**
 * \brief ar_heap_deinit.
 *       De-initialize heap memory interface.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_heap_deinit(void)
{
   return AR_EOK;
}

/**
 * \brief Allocates heap memory.
 *
 * \param[in] bytes: number of bytes to allocate heap memory.
 * \param[in] heap_info: pointer of type: ar_heap_info.
 *
 * \return
 *  Nonzero -- Success: pointer to the allocated heap memory
 *  NULL -- Failure
 *
 */
void *ar_heap_malloc(size_t bytes, par_heap_info heap_info)
{
   void * pBuff     = NULL;
   size_t alignment = 0;

   if (NULL == heap_info)
   {
      return pBuff;
   }

   if (heap_info->heap_id > AR_HEAP_ID_11)
   {
      return NULL;
   }

   // get posal heap index from ar heap index
   POSAL_HEAP_ID posal_heap_id = POSAL_HEAP_DEFAULT;
   switch (heap_info->heap_id)
   {
      case AR_HEAP_ID_DEFAULT: /** default heap */
      {
         posal_heap_id = POSAL_HEAP_DEFAULT;
         break;
      }
      case AR_HEAP_ID_1: /** Low power memory heap*/
      {
         posal_heap_id = spf_mem_island_heap_id;
         break;
      }
      default: // invalid ar heap id
      {
         return NULL;
      }
   }

   switch (heap_info->align_bytes)
   {
      case AR_HEAP_ALIGN_4_BYTES: /** 4-byte boundary */
         alignment = 4;
         break;
      case AR_HEAP_ALIGN_DEFAULT: /** default alignment */
      case AR_HEAP_ALIGN_8_BYTES: /** 8-byte boundary */
         alignment = 8;
         break;
      case AR_HEAP_ALIGN_16_BYTES: /** 16-byte boundary */
         alignment = 16;
         break;
      default:
         alignment = 8;
         break;
   }

   /* allocate enough for requested bytes + alignment wasteage + 1 word for storing offset
    * (which will be just before the aligned ptr) */
   char *ptr = (char *)posal_memory_aligned_malloc(bytes, alignment, posal_heap_id);
   if (ptr == NULL)
   {
      return (NULL);
   }

   return (ptr);
}

/**
 * \brief Allocates heap memory and initialize with 0.
 *
 * \param[in] bytes: number of bytes to allocate heap memory.
 * \param[in] heap_info: pointer of type: ar_heap_info.
 *
 * \return
 *  Nonzero -- Success: pointer to the allocated heap memory
 *  NULL -- Failure
 *
 */
void *ar_heap_calloc(size_t bytes, par_heap_info heap_info)
{
   void *pBuf = NULL;
   pBuf       = ar_heap_malloc(bytes, heap_info);
   if (NULL != pBuf)
   {
      ar_mem_set(pBuf, 0, bytes);
   }
   return pBuf;
}

/**
 * \brief Frees heap memory.
 *
 * \param[in] heap_ptr: pointer to heap memory obtained from ar_heap_alloc().
 *
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
void ar_heap_free(void *heap_ptr, par_heap_info heap_info)
{
   if ((NULL != heap_ptr) && (NULL != heap_info))
   {
      posal_memory_aligned_free(heap_ptr);
   }
   return;
}
