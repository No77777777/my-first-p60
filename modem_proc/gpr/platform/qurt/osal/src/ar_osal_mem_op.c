/**
 *=============================================================================
 * \file ar_osal_mem_op.cpp

  Copyright (c) 2019-2021 Qualcomm Technologies, Inc.
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
 * \brief ar_mem_move
 *        copies bytes from source area to destination area.
 * \param[in_out] dest: destinatiopn buffer to copy data.
 * \param[in] dest_size: destination buffer size.
 * \param[in] src: source buffer pointer to copy data from.
 * \param[in] size: bytes to copy from source buffer.
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 * note: If some regions of the source area and the destination overlap, ar_mem_move
 * ensures that the original source bytes in the overlapping region are copied before
 * being overwritten.
 */
int32_t ar_mem_move(_Inout_ void *dest, _In_ size_t dest_size, _In_ const void *src, _In_ size_t size)
{
   int32_t cStatus = AR_EOK;
   if (NULL == dest || NULL == src || 0 == dest_size || 0 == size || dest_size < size)
   {
      cStatus = AR_EBADPARAM;
      goto end;
   }
   memsmove(dest, dest_size, src, size);
end:
   return cStatus;
}

/**
 * \brief ar_mem_cmp
 *        compare two buffers.
 * \param[in] buff1: first buffer.
 * \param[in] buff2: second buffer.
 * \param[in] size: bytes to compare.
 * \return
 *  < 0   buff1 < buff2
 *    0   buff1 == buff2
 *  > 0   buff1 > buff2
 */
int32_t ar_mem_cmp(_In_ const void *buff1, _In_ const void *buff2, _In_ size_t size)
{
   int32_t cStatus = AR_EOK;
   if (NULL == buff1 || NULL == buff2)
   {
      cStatus = AR_EBADPARAM;
      goto end;
   }
   memcmp(buff1, buff2, size);
end:
   return cStatus;
}
