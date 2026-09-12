/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               N V   R E A D / W R I T E


GENERAL DESCRIPTION
   This file contains the NV read write functions.

EXTERNALIZED FUNCTIONS
   onex_nv_read_write
   onex_nv_efs_init

  Copyright (c) 2005 - 2017 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/diag/src/onex_nv_rd_wt.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/17/17   ppr     Mainline GPP2_SUPPORT_ON_SEC_SUB feature
10/30/17   ppr     Remove Feature flag ONEX_TA_30_BRINGUP.
08/31/17   ppr     Sub specific 1x EFS Item support chgs
08/31/17   ppr     New API to read EFS item as per specific sub 
03/31/15   srk     Use subscription id in EFS operations.
08/21/13   agh     Checked for NULL file_name in onex_nv_efs_init
08/21/13   fg      initial revision
===========================================================================*/


/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/
#include "fs_public.h"
#include "onex_nv_rd_wt.h"
#include "mc_i.h"
#include "mc_v.h"
#include "mcfg_fs.h"
#include "auth_i.h"
#include "srch_v.h"
#include "rex.h"
#include "msg.h"
#include "m1x_diag.h"
#include <memory.h>
#include "stringl.h"

#include "nv.h"
#include "mccdma_i.h"

/*===========================================================================

FUNCTION onex_config_nv_cmd_buf

DESCRIPTION
  This function writes NV command buffer.

DEPENDENCIES
  UNKNOWN

RETURN VALUE
  false:    if the read, right fails.
  true:     if the operation succeeded

SIDE EFFECTS
  UNKNOWN
===========================================================================*/
LOCAL boolean onex_config_nv_cmd_buf
(
  NV_OPTION     nv_op,     /* the parameters to configure nv command buffer */
  nv_cmd_type  *nv_cmd_buf /* command buffer */
)
{

  /*-----------------------------------------------------------------------*/

  if(nv_op & NV_WRT) /* it is a write */
  {
    nv_op ^= NV_WRT; /* erase the read/write bit(set to 0) */
    nv_cmd_buf->cmd = NV_WRITE_F;
  }
  else /* it is a read */
  {
    nv_cmd_buf->cmd = NV_READ_F;
  }

  switch(nv_op)
  {
  case NV_MC:
  case NV_NONSIG:
  case NV_MCCAP:
    nv_cmd_buf->sigs = MC_NV_CMD_SIG;
    break;
  case NV_AUTH:
    nv_cmd_buf->sigs = AUTH_NV_CMD_SIG;
    break;
  case NV_SRCH:
    nv_cmd_buf->sigs = SRCH_NV_CMD_SIG;
    break;
  default:
    /* nv_option error */
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
      "Invalid NV option %d",
      (int) nv_op);
    return FALSE;
  }

  return TRUE;

} /* onex_config_nv_cmd_buf */

/*===========================================================================

FUNCTION onex_nv_read_write

DESCRIPTION
  This function read/write NV.

DEPENDENCIES
  The NV task has been started and is running.

RETURN VALUE
  false:    if the read, right fails.
  true:     if the operation succeeded

SIDE EFFECTS

===========================================================================*/
boolean onex_nv_read_write
(
  nv_items_enum_type  item_code,                   /* Item to access */
  nv_item_type        *item_ptr,                   /* where to put the item */
  NV_OPTION           nv_op,                       /* option for cmd buffer*/
  rex_sigs_type       (*wait_func)(rex_sigs_type), /* the wait function*/
  nv_cmd_type         *nv_cmd_buf                  /* command buffer*/
)
{
  nv_cmd_ext_type nvi_ext;
  /*-----------------------------------------------------------------------*/

  if(!onex_config_nv_cmd_buf(nv_op, nv_cmd_buf))
  {
    return FALSE;
  }

  nv_cmd_buf->item = item_code;
  nv_cmd_buf->data_ptr = item_ptr;
  nv_cmd_buf->tcb_ptr = rex_self();
  nv_cmd_buf->done_q_ptr = NULL;

  /* Clear signal, issue the command, and wait for the response. */

  (void) rex_clr_sigs(nv_cmd_buf->tcb_ptr, nv_cmd_buf->sigs);
  nvi_ext.nvcmd = nv_cmd_buf;
  nvi_ext.context = mcc_get_asid();
  
  nv_cmd_ext(&nvi_ext);

  (void) (*wait_func)(nv_cmd_buf->sigs);

  if ((nv_cmd_buf->status != NV_DONE_S) &&
      (nv_cmd_buf->status != NV_NOTACTIVE_S))
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }

} /* onex_nv_read_write */

/*===========================================================================

FUNCTION  onex_efs_read

DESCRIPTION
  This function reads the requested EFS item.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If EFS read is Success
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean onex_efs_read
(
  const char *path,
  void       *data_ptr,
  uint32     length
)
{
  boolean fs_read_status = FALSE;
  mcfg_fs_status_e_type mcfg_status;
  sys_modem_as_id_e_type onex_asid = mcc_get_asid();
      /* 1x ASID variable */
  mcfg_fs_sub_id_e_type mcfg_sub_id = MCFG_FS_SUBID_0;
      /* MCFG specific SUB_ID variable */
  /*-----------------------------------------------------------------------*/

  /* Update MCFG specific SUB_ID if 1x is on SUB2 
   * otherwise pass MCFG_FS_SUBID_0 */
  if ( onex_asid == SYS_MODEM_AS_ID_2 )
  {
    mcfg_sub_id = MCFG_FS_SUBID_1;
  }
  mcfg_status = mcfg_fs_read( path, 
                              data_ptr, 
                              length,
                              MCFG_FS_TYPE_EFS, 
                              mcfg_sub_id );

  if ( mcfg_status == MCFG_FS_STATUS_OK )
  {
    fs_read_status = TRUE;
  }
  else
  {
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
         "FAIL!!! Hit MCFG READ ERROR STATUS:%d with MCFG_Sub_Id:%d",
                  mcfg_status,
                  mcfg_sub_id );
  }

  return fs_read_status;

} /* onex_efs_read */

/*===========================================================================

FUNCTION  onex_efs_read_per_sub

DESCRIPTION
  This function reads the requested EFS item specific to SUBID.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If EFS read is Success
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean onex_efs_read_per_sub
(
  const char *path,
  void       *data_ptr,
  uint32     length,
  mcfg_fs_sub_id_e_type sub_id
)
{
  boolean                  fs_read_status = FALSE;
  mcfg_fs_status_e_type    mcfg_status;

  /*-----------------------------------------------------------------------*/

  mcfg_status = mcfg_fs_read( path, data_ptr, length,
                              MCFG_FS_TYPE_EFS, sub_id );

  if ( mcfg_status == MCFG_FS_STATUS_OK )
  {
    fs_read_status = TRUE;
  }
  else
  {
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
      "Hit MCFG EFS READ ERROR STATUS:%d, sub_id:%d",
      mcfg_status,
      sub_id );
  }

  return fs_read_status;
} /* onex_efs_read_per_sub() */

/*===========================================================================

FUNCTION  onex_efs_write

DESCRIPTION
  This function writes the requested EFS item.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:  If EFS write is Success
  FALSE: Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean onex_efs_write
(
  const char *path,
  void       *data_ptr,
  uint32     length,
  int        oflag,
  int        mode
)
{
  boolean fs_write_status = FALSE;
  mcfg_fs_status_e_type mcfg_status;
  sys_modem_as_id_e_type onex_asid = mcc_get_asid();
      /* 1x ASID variable */
  mcfg_fs_sub_id_e_type mcfg_sub_id = MCFG_FS_SUBID_0;
      /* MCFG specific SUB_ID variable */
  /*-----------------------------------------------------------------------*/
  /* Update MCFG specific SUB_ID if 1x is on SUB2 
   * otherwise pass MCFG_FS_SUBID_0 */
  if ( onex_asid == SYS_MODEM_AS_ID_2 )
  {
    mcfg_sub_id = MCFG_FS_SUBID_1;
  }
  mcfg_status = mcfg_fs_write( path, 
                               data_ptr, 
                               length, 
                               oflag, 
                               mode,
                               MCFG_FS_TYPE_EFS, 
                               mcfg_sub_id );

  if ( mcfg_status == MCFG_FS_STATUS_OK )
  {
    fs_write_status = TRUE;
  }
  else
  {
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
      "FAIL!!! Hit MCFG WRITE ERROR STATUS:%d with MCFG Sub_Id:%d",
                  mcfg_status,
                  mcfg_sub_id );
  }

  return fs_write_status;

} /* onex_efs_write */

/*===========================================================================

FUNCTION onex_nv_efs_init

DESCRIPTION
  This function inits efs (for mc, srch and mux only).

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void onex_nv_efs_init
(
  const char * conf_file,    /* conf file name                */
  int file_max,              /* Maximum number of file names  */
  const efs_item_file_type *efs_item_files, /* Array of pathnames of the EFS
                                             * item files*/
  mcfg_fs_sub_id_e_type mcfg_sub_id         /* MCFG Subscription Id */
)
{
  struct fs_stat conf_file_det;           /* Get the File System Attributes          */
  mcfg_fs_status_e_type file_stat_result; /* Tells if the file is present or not     */
  uint16 bytes_to_write = 0;              /* # of bytes to write + '\n' per nv item  */
  boolean error = FALSE;                  /* To catch efs errors                     */
  int item = 0;

  /*-----------------------------------------------------------------------*/

  /* Check to see if the file already exists and has valid information */
  file_stat_result = mcfg_fs_stat( conf_file, 
                                   &conf_file_det,
                                   MCFG_FS_TYPE_EFS, 
                                   mcfg_sub_id );

  /* Calculate the length of the NV item path that needs to be written
  into onex_mux.conf file */
  for ( item = 0; item < file_max; item++ )
  {
    /* # of bytes in the NV item path + '\n' */
    if (efs_item_files[item].path  != NULL)
    {
      bytes_to_write += strlen( efs_item_files[item].path ) + 1;
    }
    else
    {
      M1X_MSG_SSID( DIAG, LEGACY_ERROR,
        "File name returned as NULL for item no: %d",
        item );
    }
  } /*for ( item = 0; item < (int) MUX_EFS_ITEM_FILE_MAX; item++ )*/

  /* Open a conf file and write only if
  1. The Conf file doesn't exist or
  2. The # of bytes in the file doesn't match with the
  # of bytes we need to write in the file.
  (Since conf files are incremental, the bytes can't ever match the
  size of what we want to write but contain wrong information) */
  if ( (file_stat_result == MCFG_FS_STATUS_OK) && (bytes_to_write == conf_file_det.st_size) )
  {
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
      "File already Exist!!" );
  }/*if ((file_stat_result == MCFG_FS_STATUS_OK) && (bytes_to_write == conf_file_det.st_size))*/
  else
  {
    /* To get a handle for the file opened */
    int file_handle;

    /* Special Flags.
       MCFG_FS_O_CREAT   : Create the item if it does not exist
       MCFG_FS_O_WRONLY  : To specify that file will be used for writing only
       MCFG_FS_O_TRUNC   : Truncate the file if it exists
       MCFG_FS_O_AUTODIR : Auto create parent directories
    */
    int oflag = MCFG_FS_O_CREAT | MCFG_FS_O_WRONLY | MCFG_FS_O_TRUNC | MCFG_FS_O_AUTODIR;

    int mode = MCFG_FS_ALLPERMS;

    /* open/create new conf file. */
    file_handle = mcfg_fopen( conf_file, oflag, 
                              mode,
                              MCFG_FS_TYPE_EFS, 
                              mcfg_sub_id );

    if ( file_handle == MCFG_FS_ERROR )
    {
      /* Open error */
      M1X_MSG_SSID( DIAG, LEGACY_ERROR,
        "Could not open config file for writing" );

      error = TRUE;
    } /* file_handle == -1 */
    else
    {
      /* Hold the write buffer for mcfg_fwrite */
      char* file_write_buffer;

      /* Size of buffer */
      uint32 buffer_size = 0;

      M1X_MSG_SSID( DIAG, LEGACY_MED,
        "Config file opened for writing" );

      /* Allocate memory for the file write used to store paths of all the
         NV items that needs to be written into .conf file */
      buffer_size = bytes_to_write;
      file_write_buffer =
      (char *) modem_mem_alloc( bytes_to_write, MODEM_MEM_CLIENT_1X );

      if( file_write_buffer == NULL )
      {
        error = TRUE;

        M1X_MSG_SSID( DIAG, LEGACY_ERROR,
          "Skipping mcfg_fwrite, error allocating memory" );
      } /* if( file_write_buffer == NULL )*/
      else
      {
        /* # of bytes actually written to the file */
        int bytes_written = 0;

        /* Single file name of the efs item files */
        char* file_name = NULL;

        /* The size of the file name */
        uint32 file_name_size = 0;

        /* Return value of memscpy function */
        size_t copy_size = 0;

        /* Paths for all the looped files will be added in .conf */
        for ( item = 0; item < file_max; item++ )
        {
          /* Append the NV item path to the write buffer */
          file_name = efs_item_files[item].path;

          if (file_name != NULL)
          {
            file_name_size = (uint32)strlen(file_name); /* Skip '\0' */

            if( (bytes_written + file_name_size + 1) <= buffer_size )
            {
              copy_size = memscpy( (file_write_buffer + bytes_written),
                                   (buffer_size - bytes_written),
                                   file_name, file_name_size );

              if( copy_size != file_name_size )
              {
                 M1X_MSG_SSID( DIAG, LEGACY_ERROR,
                   "Truncated memscpy, bytes_copied:%d,src_size:%d,dest_size:%d",
                   copy_size,
                   file_name_size,
                   (buffer_size - bytes_written) );
              }

              /* Write newline character at the end of every file */
              file_write_buffer[bytes_written + file_name_size] = '\n';
              bytes_written += file_name_size + 1;

            } /*if( (bytes_written + file_name_size + 1) <= buffer_size )*/
            else
            {
              M1X_MSG_SSID( DIAG, LEGACY_ERROR,
                "bytes_to_write:%d > buffer_size:%d",
                (bytes_written+file_name_size+1),
                buffer_size );

              error = TRUE;
              break;
            }/*if else ( (bytes_written + file_name_size + 1) <= buffer_size )*/
          }
          else
          {
             M1X_MSG_SSID( DIAG, LEGACY_ERROR,
               "File name returned as NULL for item no:%d",
               item);
          }
        }/* for (item = 0 ; item < (int)MC_OWNED_EFS_ITEM_FILE_MAX; item++)*/

       /* Write the efs item path to the config file */
        bytes_written = mcfg_fwrite( file_handle,
                                     file_write_buffer,
                                     bytes_to_write,
                                     MCFG_FS_TYPE_EFS );

        if ( bytes_written < bytes_to_write )
        {
          M1X_MSG_SSID( DIAG, LEGACY_ERROR,
            "Truncated EFS write error: %d < %u bytes",
            bytes_written,
            bytes_to_write );

          if ( bytes_written < 0 )
          {
            M1X_MSG_SSID( DIAG, LEGACY_ERROR,
              "Error in writing name of item files %d to config file",
              item );

            error = TRUE;
          }
        }/*if ( bytes_written < bytes_to_write )*/

        /* Free the write buffer */
        modem_mem_free( file_write_buffer, MODEM_MEM_CLIENT_1X );

      }/* if else ( file_write_buffer == NULL )*/

      /* close file */
      M1X_MSG_SSID( DIAG, LEGACY_MED,
        "Closing config file" );

      if ( mcfg_fclose( file_handle, MCFG_FS_TYPE_EFS ) == MCFG_FS_ERROR )
      {
        /* Error while closing */
        M1X_MSG_SSID( DIAG, LEGACY_ERROR,
          "Error closing config file" );

        error = TRUE;
      }/*if ( mcfg_fclose( file_handle ) == MCFG_FS_ERROR )*/
    }/*if else (file_handle == MCFG_FS_ERROR)*/
  }/*if else((file_stat_result==0)&&(bytes_to_write==conf_file_det.st_size))*/

  if( error )
  {
    M1X_MSG_SSID( DIAG, LEGACY_ERROR,
      "efs_errno %d while generating EFS conf file via mcfg",
      efs_errno );
  }

} /* onex_nv_efs_init */

