/**
 *=============================================================================
 * \file ar_osal_file_io.c
 *
 * \brief
 *      Defines public APIs for file IO operations.
 * \cond
 *  Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 * \endcond
 *=============================================================================
 */
#include "ar_osal_file_io.h"
#include "ar_osal_error.h"

#include <stringl/stringl.h>
#include <stdlib.h>

int32_t ar_fopen(_Out_ ar_fhandle *handle, _In_ const char_t *path, _In_ uint32_t access)
{
   int32_t status = AR_EOK;
#ifdef SIM
   FILE *f_ptr = NULL;

   // Check input parameters validity
   if (NULL == handle || NULL == path)
   {
      return AR_EBADPARAM;
   }

   char     opts[4];
   uint32_t i = 0;

   // Get the file permsission mask
   // TODO: Need to check and review the mapping options with Lakshman.
   if (AR_FOPEN_READ_ONLY & access)
   {
      opts[i++] = 'r';
   }

   if (AR_FOPEN_WRITE_ONLY & access)
   {
      opts[i++] = 'w';
   }

   if (AR_FOPEN_APPEND & access)
   {
      opts[i++] = 'a';
   }
   opts[i++] = '\0';

   // Do a file open.
   f_ptr = fopen(path, opts);
   if (NULL == f_ptr)
   {
      return AR_EFAILED;
   }
   // Return the handle
   *handle = (ar_fhandle)f_ptr;
#endif
   return status;
}

size_t ar_fsize(_In_ ar_fhandle handle)
{
   size_t total_size = 0;
#ifdef SIM
   if (handle == NULL)
   {
      return AR_EBADPARAM;
   }

   // get current posisiotn of the file
   FILE * fp          = (FILE *)handle;
   size_t orginal_pos = ftell(fp);

   // move the cursor to the end of the file and find the size of the file.
   fseek(fp, 0, SEEK_END);
   total_size = ftell(fp);

   // Move the cursor back to the original position
   fseek(fp, orginal_pos, SEEK_SET);
#endif
   return total_size;
}

int32_t ar_fseek(_In_ ar_fhandle handle, _In_ size_t offset, _In_ ar_fseek_reference_t ref)
{
#ifdef SIM
   int32_t status = AR_EOK;

   if (handle == NULL)
   {
      return AR_EBADPARAM;
   }

   uint32_t seek_opt = SEEK_CUR;
   switch (ref)
   {
      case AR_FSEEK_BEGIN:
      {
         seek_opt = SEEK_SET;
         break;
      }
      case AR_FSEEK_END:
      {
         seek_opt = SEEK_END;
         break;
      }
      case AR_FSEEK_CURRENT:
      {
         seek_opt = SEEK_CUR;
         break;
      }
      default:
      {
         seek_opt = SEEK_CUR;
         break;
      }
   }

   // Move the file pointer by the offset from the current position.
   status = fseek((FILE *)handle, offset, seek_opt);
   if (AR_FAILED(status))
   {
      return AR_EFAILED;
   }
#endif
   return AR_EOK;
}

int32_t ar_fread(_In_ ar_fhandle handle, _Inout_ void *buf_ptr, _In_ size_t read_size, _Inout_ size_t *bytes_read)
{
#ifdef SIM
   int32_t status = AR_EOK;
   if (handle == NULL || 0 == read_size || NULL == buf_ptr || NULL == bytes_read)
   {
      printf("ERROR in READ param \n");
      return AR_EBADPARAM;
   }
   // file read
   status = fread(buf_ptr, sizeof(char), read_size, (FILE *)handle);
   if (AR_FAILED(status))
   {
      return AR_EFAILED;
   }
#endif
   return AR_EOK;
}

int32_t ar_fwrite(_In_ ar_fhandle handle, _Inout_ void *buf_ptr, _In_ size_t write_size, _Inout_ size_t *bytes_written)
{
#ifdef SIM
   if (handle == NULL || 0 == write_size || NULL == buf_ptr)
   {
      return AR_EBADPARAM;
   }

   // Write the data buffer into the file
   *bytes_written = fwrite(buf_ptr, sizeof(char), write_size, (FILE *)handle);
   if (0 == *bytes_written)
   {
      return AR_EFAILED;
   }
#endif
   return AR_EOK;
}

int32_t ar_fclose(_In_ ar_fhandle handle)
{
#ifdef SIM
   int32_t status = AR_EOK;

   if (handle == NULL)
   {
      return AR_EBADPARAM;
   }

   status = fclose((FILE *)handle);
   if (AR_FAILED(status))
   {
      return AR_EFAILED;
   }
#endif
   return AR_EOK;
}

int32_t ar_fdelete(_In_ const char_t *path)
{
#ifdef SIM
   int32_t status = AR_EOK;

   if (NULL == path)
   {
      return AR_EBADPARAM;
   }

   // Delete the file.
   status = remove(path);
   if (AR_FAILED(status))
   {
      return AR_EFAILED;
   }
#endif
   return AR_EOK;
}
