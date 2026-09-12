/** vi: tw=128 ts=3 sw=3 et
@file pd_dump_fs_al.c
@brief This file contains Abstraction layer for writing user process dumps into file systems.
*/
/*=============================================================================
NOTE: The @brief description above does not appear in the PDF.
The tms_mainpage.dox file contains the group/module descriptions that
are displayed in the output PDF generated using Doxygen and LaTeX. To
edit or update any of the group/module text in the PDF, edit the
tms_mainpage.dox file or contact Tech Pubs.
===============================================================================*/
/*=============================================================================
Copyright (c) 2018-2019,2022 by Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Confidential and Proprietary
=============================================================================*/
/*=============================================================================
Edit History
$Header: //components/rel/core.mpss/10.0/debugtools/pd_mon/src/pd_dump_fs_al.c#3 $
$DateTime: 2022/06/22 22:40:36 $
$Change: 38006310 $
$Author: pwbldsvc $
===============================================================================*/

#include "stdlib.h"
#include "stdio.h"
#include "stringl/stringl.h"

#include "qurt.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "rfs_api.h"

#ifdef PD_DUMP_FS_SHELL_USE_FAPI
const char* filename_prefix_string = "/data/vendor/rfsa/";
#else
const char* filename_prefix_string = "/ramdumps/";
#endif

#define FILENAME_SIZE 128
  
int pd_dump_fs_al_open_file_for_read
(
  const char * filename,
  int *file_desc
)
{
  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  FILE * fd;
  #else
  int fd;
  #endif 
  
  char * fname = calloc(FILENAME_SIZE, 1);

  if ( fname == NULL )
    return -1;

  (void)tms_utils_fmt(fname, FILENAME_SIZE, "%s%s", filename_prefix_string, filename);

  TMS_MSG_SPRINTF_HIGH_1("PD dump fname open %s ", fname);

  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  fd = fopen(fname, "r");
  if ( fd == NULL )
  {
    TMS_MSG_ERROR("File open failed");
    free(fname);
    return -1;
  }
  #else
  fd = rfs_open(fname, RFS_O_RDONLY, 0755);
  if ( fd < 0 )
  {
    TMS_MSG_ERROR_1("RFS-File open failed %d ", fd);
    free(fname);
    return -1;
  }
  #endif

  free(fname);
  *file_desc = (int)fd;
  return 1; /*Any positive number to represent success*/
}

int pd_dump_fs_al_open_file_for_create_write
(
  const char * filename,
  int *file_desc
)
{
  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  FILE * fd;
  #else
  int fd;
  #endif 
  
  char * fname = calloc(FILENAME_SIZE, 1);

  if ( fname == NULL )
    return -1;

  (void)tms_utils_fmt(fname, FILENAME_SIZE, "%s%s", filename_prefix_string, filename);

  TMS_MSG_SPRINTF_HIGH_1("PD dump fname open %s ", fname);

  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  fd = fopen(fname, "w+");
  if ( fd == NULL )
  {
    TMS_MSG_ERROR("File open failed");
    free(fname);
    return -1;
  }
  #else
  fd = rfs_open(fname, RFS_O_WRONLY | RFS_O_CREAT | RFS_O_TRUNC, 755);
  if ( fd < 0 )
  {
    TMS_MSG_ERROR_1("RFS-File open failed %d ", fd);
    free(fname);
    return -1;
  }
  #endif

  free(fname);
  *file_desc = (int)fd;
  return 1; /*Any positive number to represent success*/
}

/** pd_dump_fs_al_delete
 *
 * @param filename : pd dump file name
 *
 * @return 1 in case of successful operation else
 *         -1.
 */
int pd_dump_fs_al_delete
(
  const char * filename
)
{
  int ret;

  char * fname = calloc(FILENAME_SIZE, 1);

  if ( fname == NULL )
    return -1;

  (void)tms_utils_fmt(fname, FILENAME_SIZE, "%s%s", filename_prefix_string, filename);

  TMS_MSG_SPRINTF_HIGH_1("PD dump fname delete %s ", fname);

  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  ret = remove(fname);
  if ( 0 != ret )
  {
    TMS_MSG_ERROR("File delete failed");
    free(fname);
    return -1;
  }
  #else
  ret = rfs_unlink(fname);
  if ( 0 != ret )
  {
    TMS_MSG_ERROR_1("RFS-File delete failed %d ", ret);
    free(fname);
    return -1;
  }
  #endif

  free(fname);
  return 1; /*Any positive number to represent success*/
}

unsigned int pd_dump_fs_al_write
(
  unsigned int fd,
  const byte * buf,
  unsigned int nbyte
)
{
  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  return (unsigned int)fwrite(buf, 1, nbyte, (FILE *)fd);
  #else
  return rfs_write(fd, buf, nbyte);
  #endif
}

unsigned int pd_dump_fs_al_read
(
  unsigned int fd,
  void * buf,
  unsigned int nbyte
)
{
  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  return (unsigned int)fread(buf, 1, nbyte, (FILE *)fd);
  #else
  return rfs_read(fd, buf, nbyte);
  #endif
}

unsigned int pd_dump_fs_al_close
(
  unsigned int fd
)
{
  #ifdef PD_DUMP_FS_SHELL_USE_FAPI
  if ( fd == -1 )
     return 0;
  return (unsigned int)fclose((FILE *)fd);
  #else
  return rfs_close(fd);
  #endif
}


