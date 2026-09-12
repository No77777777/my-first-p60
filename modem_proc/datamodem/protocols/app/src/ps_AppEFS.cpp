/*===========================================================================

                   P S _ A P P E F S . C P P

DESCRIPTION
 The Data Services APP Utility file. Contains API functions for applications
 like File Transfer Protocol ( FTP )

EXTERNALIZED FUNCTIONS


Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath: $
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_AppEFS.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module
===========================================================================*/
#include "comdef.h"
#include "target.h"
#include "customer.h"
#include "msg.h"

#include <stringl/stringl.h>
#include "ps_AppEFS.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "ds_Utils_DebugMsg.h"
#include "fs_public.h"
#include "data_msg.h"
#include "ps_system_heap.h"
#include "ps_app_logging_util.h"

#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_APP;
/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

===========================================================================*/
#define PS_APP_MAX_EFS_FILENAME 255

/*===========================================================================
FUNCTION      PS_APP_EFS_FILE_OPEN

DESCRIPTION   Opens file

PARAMETERS 

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
boolean ps_app_efs_file_open
( 
  int *file_handle,
  boolean create_file,
  const char * filename,
  const char * directory_name
)
{
  char *file_name = NULL; 
  uint16 file_name_len = 0, dir_name_len = 0, filepath_len = 0;
  int handle = -1;

  /*-------------------------------------------------------------------------
    Check input parameters,file handle and file name cannot be NULL
    directory_name can be null
  -------------------------------------------------------------------------*/
	if ( NULL ==  file_handle || NULL == filename )
	{
      PS_APP_ERR_LOG_2( file_handle, filename );
      return FALSE;
	}

  file_name_len = strlen(filename);

  /*-------------------------------------------------------------------------
    Set filepath len one greater for adding null terminator
  -------------------------------------------------------------------------*/
  filepath_len = file_name_len+1;

  /*-------------------------------------------------------------------------
    If directory name is not null then append it for efs_open call
  -------------------------------------------------------------------------*/
  if ( NULL != directory_name)
  {
    dir_name_len = strlen(directory_name);
    filepath_len += dir_name_len;
  }

  if ( filepath_len <= 0 || 
       filepath_len >  PS_APP_MAX_EFS_FILENAME )
  {
    PS_APP_ERR_LOG_1( filepath_len );
    return FALSE;
  }

  file_name = (char *) ps_system_heap_mem_alloc(filepath_len);

  if ( NULL == file_name )
  {
    PS_APP_ERR_LOG_0( );
    return FALSE;
  }

  file_name[0] = '\0';
  if (dir_name_len > 0 )
  {
    strlcat( file_name, directory_name, dir_name_len + 1 );
  }

  strlcat( file_name, filename, filepath_len );

  PS_APP_SPRINTF_DEBUG_1( "FTP efs_open file %s", 
                      file_name );

  if ( FALSE == create_file )
  {
    handle = efs_open(file_name,O_RDONLY);
    PS_APP_DEBUG_1( "FTP efs openned read only file, handle %i", handle);
  }
  else
  {
    handle = efs_open(file_name,O_RDWR|O_TRUNC|O_CREAT,0777);
    PS_APP_DEBUG_1( "FTP efs created read/write, trunc file, handle %i", handle);
  }

  PS_SYSTEM_HEAP_MEM_FREE(file_name);

  *file_handle = handle;
  if ( handle < 0 )
  {
    PS_APP_ERR_LOG_1(*file_handle);
    return FALSE;
  }

  return TRUE;
} /* ps_ftp_efs_file_open() */

/**-------------------------------------------------------------------------- 
*   @fn AppEFS()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
AppEFS::AppEFS():
file_handle(-1)
{
}

/**-------------------------------------------------------------------------- 
*   @fn  ~AppEFS()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
AppEFS::~AppEFS()
{
}

/**-------------------------------------------------------------------------- 
*   @fn OpenFile()
*   @brief Opens EFS File
---------------------------------------------------------------------------*/
int AppEFS::OpenFile
( 
   const char* filename, 
   const char* file_dir, 
   uint32* file_size,
   boolean create_file
)
{
  int efs_file_handle = -1;

  if ( FALSE == ps_app_efs_file_open(&efs_file_handle, 
                                     create_file, filename,
                                     file_dir) ) 
  {
    return -1;
  }

  if ( NULL != file_size )
  {
    /* Seek the file size */
    *file_size = efs_lseek(efs_file_handle,0,SEEK_END);

    PS_APP_DEBUG_1("File size is %i", *file_size);

    /* Seek to the beginning for the file reads */ 
    efs_lseek(efs_file_handle, 0, SEEK_SET);
  }

  file_handle = efs_file_handle;

  return efs_file_handle;
}

/**-------------------------------------------------------------------------- 
*   @fn OpenFile()
*   @brief Opens EFS File
---------------------------------------------------------------------------*/
int AppEFS::WritetoFile
( 
  const uint8* write_content, 
  uint32 content_len,
  boolean append
)
{
  if ( TRUE == append )
  {
    //FN debug todo, add capability to append if needed
    PS_APP_DEBUG_1("Appending to file not currently supported %i", append);
  }

  return efs_write(file_handle, (void*)write_content, content_len);
}
