/**
 *=============================================================================
 * \file ar_osal_mem_op_island.cpp

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  when       who     what, where, why
  --------   ---     -------------------------------------------------------
  11/11/2019  np    Created the file.
 *============================================================================= */

#include "ar_osal_mem_op.h"
#include "ar_osal_error.h"
#include "ar_osal_types.h"
#include <stringl/stringl.h>

/**
 * \brief ar_mem_cpy
 *        copies bytes between buffers.
 * \param[in_out] dest: destinatiopn buffer to copy data.
 * \param[in] dest_size: destination buffer size.
 * \param[in] src: source buffer pointer to copy data from.
 * \param[in] size: bytes to copy from source buffer.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_mem_cpy(_Inout_ void *dest, _In_ size_t dest_size, _In_ const void *src, _In_ size_t size)
{
   int32_t cStatus = AR_EOK;
   if (NULL == dest || NULL == src || 0 == dest_size || 0 == size || dest_size < size)
   {
      cStatus = AR_EBADPARAM;
      goto end;
   }
   memscpy(dest, dest_size, src, size);
end:
   return cStatus;
}

/**
 * \brief ar_mem_set
 *        set buffer to specified value.
 * \param[in_out] dest: destinatiopn buffer to set the specified value.
 * \param[in] c: specified value to set, of integer type.
 * \param[in] size: buffer bytes to set the value, must be multiple of specified value "c" data size.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 */
int32_t ar_mem_set(_Inout_ void *dest, _In_ int32_t c, _In_ size_t size)
{
   int32_t cStatus = AR_EOK;
   if (NULL == dest || 0 == size)
   {
      cStatus = AR_EBADPARAM;
      goto end;
   }
   memset(dest, c, size);
end:
   return cStatus;
}
