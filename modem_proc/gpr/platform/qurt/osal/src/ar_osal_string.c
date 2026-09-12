
/**
 *=============================================================================
 * \file ar_osal_string.c

  Copyright (c) 2019-2020 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  when       who     what, where, why
  --------   ---     -------------------------------------------------------
  11/11/2019  np    Created the file.
 *============================================================================= */

#include "ar_osal_string.h"
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include <stringl/stringl.h>
#include <string.h>

/**
 * \brief ar_strlen
 *        Get the length of the string.
 * \param[in] str: NULL terminated string buffer pointer.
 * \param[in] size: String buffer size in bytes.
 * \return
 *  Number of characters in the string, not including the terminating null
 *  character. If there is no null terminator in the buffer, provided
 *  string buffer size is returned to indicate the error.
 */
size_t ar_strlen(const char_t *str, size_t size)
{
   return strnlen(str, size);
}

/**
 * \brief ar_strcpy
 *        Copies character from source to destination string including null terminating character.
 * \param[in_out] str_dest: Destination string buffer.
 * \param[in] str_dest_size: Destination buffer size, includes null terminating character
 * \param[in] str_src: Source string to copy from.
 * \param[in] cpy_size: Source string buffer size includes null character
 * \return
 *  0 -- Success
 *  Nonzero -- Failure
 * note: Always null terminator is appended to the destination and string
 * truncation will occur if destination size is smaller than copy size.
 */
int32_t ar_strcpy(char_t *str_dest, size_t str_dest_size, const char_t *str_src, size_t cpy_size)
{
   int32_t cStatus     = AR_EOK;
   int32_t size_copied = 0;

   /* Take min of both sizes*/
   if (str_dest_size < cpy_size)
   {
      cpy_size = str_dest_size;

      // Return error if entire size specified cannot be copied
      cStatus = AR_EFAILED;
   }

   /* returns size it copies, at most cpy_size-1 should be copied*/
   size_copied = strlcpy(str_dest, str_src, cpy_size);

   if (size_copied != (cpy_size - 1))
   {
      cStatus |= AR_EFAILED;
   }

   return cStatus;
}

/**
 * \brief ar_strcmp
 *        Compare characters in two given strings.
 * \param[in] str1: Null terminated string 1
 * \param[in] str2: Null terminated string 2
 * \param[in] num: number of characters to compare.
 * \return
 *  < 0   str1 < str2
 *    0   str1 == str2
 *  > 0   str1 > str2
 */
int32_t ar_strcmp(const char_t *str1, const char_t *str2, size_t num)
{
   return strncmp(str1, str2, num);
}

/**
* \brief ar_strcat
*        Append character from source to destination string.
* \param[in_out] str_dest: Destination string buffer.
* \param[in] str_dest_size: Size in characters.
* \param[in] str_src: Source string to copy from.
* \param[in] apnd_size: Size in characters to append from source string.
* \return
*  0 -- Success
*  Nonzero -- Failure
* note: Always null terminator is appended to the destination after concatenation and string
* truncation will occur if destination size is smaller or equal to source.
*/
int32_t ar_strcat(char_t *str_dest, size_t str_dest_size, const char_t *str_src, size_t apnd_size)
{
   int32_t cStatus = AR_EOK;
   size_t  DestLen = ar_strlen(str_dest, str_dest_size);
   size_t  FreeLen = str_dest_size - DestLen;

   /* check the free space for append */

   if (FreeLen <= apnd_size)
   {
      cStatus = strlcat(str_dest, str_src, str_dest_size);
      if (str_dest_size == cStatus)
      {
         cStatus = AR_EOK;
      }
      else
      {
         cStatus = AR_EFAILED;
      }
   }
   else
   {
      cStatus = strlcpy(str_dest + DestLen, str_src, apnd_size);
   }

   return cStatus;
}

/**
 * \brief ar_strstr
 *        Search string in string.
 * \param[in] str: Null terminated string
 * \param[in] str_search: Null terminated string to search for.
 * \return
 *         Pointer to the first occurrence of str_search in str.
 *         NULL- if not found.
 */
char_t *ar_strstr(const char_t *str, const char_t *str_search)
{
   return strstr(str, str_search);
}
