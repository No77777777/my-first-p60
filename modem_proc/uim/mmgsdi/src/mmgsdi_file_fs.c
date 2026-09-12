/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   F I L E   F S   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the processing of all functions related to
  SIM file access fallback to File System

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2017, 2019, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_file_fs.c#3 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
04/01/21   rps     Add support for GSM FPLMN in fs fallback
09/09/21   pvb     Delete attributes from card for File fallback supported file
09/24/19   sg      Fallback to system/cache for EF INCONSISTENT error scenario
09/16/19   sg      Fallback to file system/cache for missing files
03/14/17   nc      Better checks for return values 
01/16/17   shr     Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "intconv.h"
#include "uim_msg.h"
#include "mmgsdi_efs.h"
#include "mmgsdiutil.h"
#include "mmgsdi_nv.h"
#include "mmgsdi_card_init.h"
#include "mmgsdi_file_fs.h"
#include "mmgsdi_file.h"
#include "mmgsdi_file_fallback.h"
#include "mmgsdicache.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Number of file-associate file pairs supported per SIM file
   in the File System */
#define MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED      10

/* Max. file length of an associate file */
#define MMGSDI_FILE_FS_MAX_ASSC_FILE_LEN        10

/* Max. file path length associated with fallback to FS */
#define MMGSDI_FILE_FS_MAX_FILE_PATH_LEN        256

/* Size of the FS Info Table */
#define MMGSDI_FILE_FS_INFO_TABLE_SIZE          \
  (sizeof(mmgsdi_file_fs_info_table)/sizeof(mmgsdi_file_fs_info_table[0]))

/* File paths for SIM file fallback to File System */
static char *mmgsdi_fs_mmgsdi_dir = "/mmgsdi";
static char *mmgsdi_fs_efs_dir    = "/mmgsdi/efs";

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_FS_FILE_ASSC_FILE_INFO_TYPE

   DESCRIPTION:
     The structure contains info on a file-associate file pair
     stored in the header of a SIM fallback file in the file system
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST{
  /* Length of associate file in assc_file_buf */
  uint8 assc_file_len;
  /* Buffer storing the asscociate file value */
  uint8 assc_file_buf[MMGSDI_FILE_FS_MAX_ASSC_FILE_LEN];
  /* Length of the file in the SIM fallback file body */
  uint8 file_len;
} mmgsdi_file_fs_file_assc_file_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_FS_FILE_HEADER_TYPE

   DESCRIPTION:
     The structure maps to the header of a SIM fallback file in
     the file system
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST{
  /* Index to update in file_assc_file_info when adding
     new entry */
  uint8                                   
    next_entry_to_update;
  /* Info on File-Associate file pairs stored in the File System */
  mmgsdi_file_fs_file_assc_file_info_type 
    file_assc_file_info[MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED];
} mmgsdi_file_fs_file_header_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_FS_FILE_SYS_INFO_TYPE

   DESCRIPTION:
     The structure contains info on the SIM fallback file in
     the file system
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST{
  /* Should the File System file cache be written back into FS */
  boolean is_write_needed;
  /* Has the file system been read and file info cached */
  boolean  is_fs_read;
  /* No. of file-associate file pairs in the FS file */
  uint8    no_of_entries;
  /* Length of the file in the FS */
  uint32   fs_file_len;
  /* Cached copy of the file in the FS */
  uint8   *fs_file_buf_ptr;
} mmgsdi_file_fs_file_sys_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      MMGSDI_FILE_FS_INFO_TABLE_TYPE

   DESCRIPTION:
     The structure contains info on the SIM files for which fallback
     to File System is supported
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  /* File for which fallback to FS is supported */
  mmgsdi_file_enum_type              file;
  /* App type corresponding to the file */
  mmgsdi_app_enum_type               app_type;
  /* Service number corresponding to the file */
  mmgsdi_service_enum_type           srv;
  /* Associate file used to uniquely tie the file
     to a SIM card */
  mmgsdi_file_enum_type              associate_file;
  /* Type of file */
  mmgsdi_file_structure_enum_type    file_type;
 /* Length of the associate file */
  uint8                              associate_file_length;
  /* Type of the associate file */
  mmgsdi_file_structure_enum_type    associate_file_type;
  /* Name of the fallback file in File System */
  const char*                        file_name_in_fs;
  /* Info on the fallback file in the File System */
  mmgsdi_file_fs_file_sys_info_type *file_info_ptr;
} mmgsdi_file_fs_info_table_type;

/* USIM EF-MWIS: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type mwis_file_info;

/* USIM EF-FPLMN: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type fplmn_file_info;

/* USIM EF-KEYS: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type keys_file_info;

/* USIM EF-KEYSPS: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type keysps_file_info;

/* GSM EF-KC: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type kc_file_info;

/* GSM EF-FPLMN: Info on fallback file in File System */
static mmgsdi_file_fs_file_sys_info_type gsm_fplmn_file_info;

/* ----------------------------------------------------------------------------
                          IMPORTANT NOTE
----------------------------------------------------------------------------
When adding a new file to the MMGSDI File FS Info table
a. Add an entry to the mmgsdi_file_fs_info_table and mmgsdi_file_info_table
b. Create a fallback file info variable similar to mwis_file_info
   and link this in mmgsdi_file_fs_info_table_type.file_info_ptr
-------------------------------------------------------------------------------*/
static const mmgsdi_file_fs_info_table_type mmgsdi_file_fs_info_table[] =
{
  /* File,                   App type,             Service, Associate File 
     File Type,       Asscociate File Length,     Asscociate File Type,
     File Name in File System, Info on fallback file in File System */

    {MMGSDI_USIM_MWIS, MMGSDI_APP_USIM, MMGSDI_USIM_SRV_MWI, MMGSDI_USIM_IMSI,
     MMGSDI_LINEAR_FIXED_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE,
     "mwis", &mwis_file_info},

    {MMGSDI_USIM_FPLMN, MMGSDI_APP_USIM, MMGSDI_NONE, MMGSDI_USIM_IMSI,
     MMGSDI_TRANSPARENT_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE,
     "fplmn", &fplmn_file_info},

    {MMGSDI_USIM_KEYS, MMGSDI_APP_USIM, MMGSDI_NONE, MMGSDI_USIM_IMSI,
     MMGSDI_TRANSPARENT_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE,
     "keys", &keys_file_info},

    {MMGSDI_USIM_KEYSPS, MMGSDI_APP_USIM, MMGSDI_NONE, MMGSDI_USIM_IMSI,
     MMGSDI_TRANSPARENT_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE,
     "keysps", &keysps_file_info},

    {MMGSDI_GSM_KC, MMGSDI_APP_SIM, MMGSDI_NONE, MMGSDI_GSM_IMSI,
     MMGSDI_TRANSPARENT_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE, 
     "gsmkc", &kc_file_info},

    {MMGSDI_GSM_FPLMN, MMGSDI_APP_SIM, MMGSDI_NONE, MMGSDI_GSM_IMSI,
     MMGSDI_TRANSPARENT_FILE, MMGSDI_IMSI_LEN, MMGSDI_TRANSPARENT_FILE,
     "gsmfplmn", &gsm_fplmn_file_info},
};


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_GET_FILE_PATH_FROM_INDEX

DESCRIPTION
  This function will return the path in the file system w.r.t.
  storage of the SIM file contents on fallback.

PARAMETERS
  path_buf:     Buffer to hold the path
  path_buf_len: Size of the path buffer
  index:        Index indicating the EF for which path is needed

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_get_file_path_from_index(
  char   *path_buf_ptr,
  uint32  path_buf_len,
  uint8   index
)
{
  const char *efs_file_name = NULL;
  uint32      path_len      = 0;
  uint32      file_len      = 0;

  /* Check input data */
  if(path_buf_ptr == NULL ||
     path_buf_len == 0 ||
     index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Get the name of file in FS */
  efs_file_name = mmgsdi_file_fs_info_table[index].file_name_in_fs;

  if(efs_file_name == NULL)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  path_len = strlen(mmgsdi_fs_efs_dir);
  file_len = strlen(efs_file_name);

  /* If the enrite path cannot be fit in the buffer,
     return error */
  if(path_len + file_len + 2 > path_buf_len)
  {
    return MMGSDI_ERROR;
  }

  /* Copy the folder path of the file in FS */
  mmgsdi_memscpy(path_buf_ptr, path_buf_len, mmgsdi_fs_efs_dir, path_len);

  /* Copy the file name into the path */
  path_buf_ptr[path_len++] = '/';
  mmgsdi_memscpy(&path_buf_ptr[path_len], 
                 (path_buf_len - path_len),
                 efs_file_name,
                 file_len);

  path_buf_ptr[path_len + file_len] = '\0';
  
  return MMGSDI_SUCCESS;
}/* mmgsdi_file_fs_get_file_path_from_index */


/*==========================================================================
FUNCTION MMGSDI_FILE_FS_PARSE_AND_VALIDATE_FILE

DESCRIPTION
  Parses the file read from the file system and validates it.
  If validation fails, the cached file data is cleared and
  file in the file system is considered corrupted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
static void mmgsdi_file_fs_parse_and_validate_file(
  uint8    index
)
{
  uint32                           header_size           = sizeof(mmgsdi_file_fs_file_header_type);
  uint32                           body_size             = 0;
  boolean                          file_invalid          = FALSE;
  uint8                            header_index          = 0;
  boolean                          zero_len_header_found = FALSE;
  uint8                            no_of_entries         = 0;
  mmgsdi_file_fs_file_header_type *fs_file_header_ptr    = NULL;
  uint8                            file_length           = 0;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return;
  }

  /* Ensure File is already read and cached prior to parsing */
  if(!mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read ||
     (mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr == NULL) ||
     (mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len == 0))
  {
    return;
  }

  /* Get file default length from mmgsdi_file_info_table */
  if (mmgsdi_file_fallback_get_file_info(mmgsdi_file_fs_info_table[index].file,
                                         &file_length,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL) != MMGSDI_SUCCESS)
  {
    return;
  }

  /* Get a pointer to the file metadata */
  fs_file_header_ptr =
    (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;

  do
  {
    /* File needs to atleast contain a header */
    if(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len <= header_size)
    {
      file_invalid = TRUE;
      break;
    }
    /* Next entry to update needs to be in valid range */
    if(fs_file_header_ptr->next_entry_to_update >= MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED)
    {
      file_invalid = TRUE;
      break;
    }

    /* Validate each entry in the header */
    for(header_index = 0; header_index < MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED; header_index++)
    {
      /* If header entry is present... */
      if(fs_file_header_ptr->file_assc_file_info[header_index].assc_file_len > 0)
      {
        /* Associate file length should not exceed max. */
        if(fs_file_header_ptr->file_assc_file_info[header_index].assc_file_len > MMGSDI_FILE_FS_MAX_ASSC_FILE_LEN)
        {
          file_invalid = TRUE;
          break;
        }
        else
        {
          /* If there is a valid header entry after an empty entry - this is invalid */
          if(zero_len_header_found)
          {
            file_invalid = TRUE;
            break;
          }
          /* File length should not exceed max. */
          if(fs_file_header_ptr->file_assc_file_info[header_index].file_len != file_length)
          {
            file_invalid = TRUE;
            break;
          }          
          no_of_entries++;          
        }
      }
      /* Empty header entry found */
      else
      {
        zero_len_header_found = TRUE;
      }
    }
  
    /* Calculate size of file body */
    body_size = mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len - header_size;

    /* Validate body size */
    if(body_size != file_length * no_of_entries)
    {
      file_invalid = TRUE;
      break;
    }
    mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries = no_of_entries;
  } while(0);

  if(file_invalid)
  {
    /* Since the file in FS is invalid, the next read attempt will cause
       default EF value to be returned from hard-coded value in code and
       the next write attempt to the EF will create a new file overwriting
       the corrupted file */
    UIM_MSG_ERR_1("In mmgsdi_file_fs_parse_and_validate_file, index: 0x%x, parsed file is invalid",
                  index);
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr);
    mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len = 0;
  }
} /* mmgsdi_file_fs_parse_and_validate_file */


/*==========================================================================
FUNCTION MMGSDI_FILE_FS_READ_FILE

DESCRIPTION
  Reads the file from the file system, caches it and parses the same

DEPENDENCIES
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_read_file(
  uint8    index
)
{
  char                    *path_buf_ptr  = NULL;
  mmgsdi_return_enum_type  mmgsdi_status = MMGSDI_SUCCESS;
  uint32                   read_size     = 0;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* If file is already read from FS, return */
  if(mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read)
  {
    return MMGSDI_SUCCESS;
  }

  mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read = TRUE;

  /* Allocate memory for file path */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(path_buf_ptr,
                                     MMGSDI_FILE_FS_MAX_FILE_PATH_LEN);
  if (path_buf_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  do
  {
    /* Get file path for access in the file system */
    mmgsdi_status = mmgsdi_file_fs_get_file_path_from_index(
                      path_buf_ptr,
                      MMGSDI_FILE_FS_MAX_FILE_PATH_LEN,
                      index);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Get file size */
    if(MMGSDI_SUCCESS !=
           gsdi_fs_get_file_size((const char *)path_buf_ptr,
                                 &read_size,
                                 UIM_COMMON_EFS_DEVICE))
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Allocate memory to cache the file */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr,
                                       read_size);
    if (mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_ERROR;
      break;
    }

    /* Read the file from file system */
    if(MMGSDI_SUCCESS != gsdi_efs_read_file(
                           (const char *)path_buf_ptr,
                           &read_size,
                           (uint8 *)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr,
                           UIM_COMMON_EFS_DEVICE))
    {
      MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr);
      mmgsdi_status = MMGSDI_ERROR;
    }
  }while(0);

  MMGSDIUTIL_TMC_MEM_FREE(path_buf_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  /* Update file size in cache */
  mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len = read_size;

  /* Parse and validate file */
  mmgsdi_file_fs_parse_and_validate_file(index);

  return MMGSDI_SUCCESS;
}/* mmgsdi_file_fs_read_file */


/*==========================================================================
FUNCTION MMGSDI_FILE_FS_WRITE_FILE

DESCRIPTION
  Function to write a file from cache into the file system

DEPENDENCIES
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
==========================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_write_file(
  uint8    index
)
{
  char                    *path_buf_ptr         = NULL;
  mmgsdi_return_enum_type  mmgsdi_status        = MMGSDI_SUCCESS;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* If write to FS is not needed, return */
  if(mmgsdi_file_fs_info_table[index].file_info_ptr->is_write_needed == FALSE)
  {
    return MMGSDI_SUCCESS;
  }

  mmgsdi_file_fs_info_table[index].file_info_ptr->is_write_needed = FALSE;

  /* Allocate memory for file path */
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(path_buf_ptr,
                                     MMGSDI_FILE_FS_MAX_FILE_PATH_LEN);
  if (path_buf_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }  

  do
  {
    /* Get file path for access in the file system */
    mmgsdi_status = mmgsdi_file_fs_get_file_path_from_index(
                      path_buf_ptr,
                      MMGSDI_FILE_FS_MAX_FILE_PATH_LEN,
                      index);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Check if MMGSDI dir exists, else create it */
    if(MMGSDI_SUCCESS != gsdi_efs_name_test((const char *)mmgsdi_fs_mmgsdi_dir,
                                            UIM_COMMON_EFS_DEVICE))
    {
      if(MMGSDI_SUCCESS != gsdi_efs_mkdir((const char *)mmgsdi_fs_mmgsdi_dir))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }

    /* Check if EFS dir exists, else create it */
    if(MMGSDI_SUCCESS != gsdi_efs_name_test((const char *)mmgsdi_fs_efs_dir,
                                            UIM_COMMON_EFS_DEVICE))
    {
      if(MMGSDI_SUCCESS != gsdi_efs_mkdir((const char *)mmgsdi_fs_efs_dir))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }

    /* Check if file dir exists, else create it */
    if(MMGSDI_SUCCESS != gsdi_efs_name_test((const char *)path_buf_ptr,
                                            UIM_COMMON_EFS_DEVICE))
    {
      if(MMGSDI_SUCCESS != gsdi_efs_create_file((const char *)path_buf_ptr,
                                                UIM_COMMON_EFS_DEVICE))
      {
        mmgsdi_status = MMGSDI_ERROR;
        break;
      }
    }

    /* Write file */
    if(MMGSDI_SUCCESS == gsdi_efs_write_file(
                         (const char *)path_buf_ptr,
                          mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len,
                          (uint8 *)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr,
                          UIM_COMMON_EFS_DEVICE))
    {
      mmgsdi_status = MMGSDI_SUCCESS;
      UIM_MSG_HIGH_1("mmgsdi_file_fs_write_file, for index: 0x%x", index);
    }
    else
    {
      mmgsdi_status = MMGSDI_ERROR;
    }
  }while(0);

  MMGSDIUTIL_TMC_MEM_FREE(path_buf_ptr);
  return mmgsdi_status;
}/* mmgsdi_file_fs_write_file */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_READ_ASSC_FILE

DESCRIPTION
  This function will read the associate file corresponding to file
  that has fallback support to FS

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_read_assc_file(
  mmgsdi_session_id_type    session_id,  
  mmgsdi_slot_id_enum_type  slot,
  uint8                     index,
  mmgsdi_data_type         *data_ptr
)
{
  mmgsdi_file_enum_type associate_file;

  /* Check input data */
  if(data_ptr == NULL ||
     index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  associate_file = mmgsdi_file_fs_info_table[index].associate_file;

  if(mmgsdi_file_fs_info_table[index].associate_file_type ==
       MMGSDI_TRANSPARENT_FILE)
  {
    return mmgsdi_card_init_cache_binary(
             session_id,
             mmgsdi_file_fs_info_table[index].app_type,
             slot,
             mmgsdi_file_fs_info_table[index].associate_file,
             0,
             data_ptr);
  }
  /* Associate file is used to uniquely tie the file with a
     card - hence for sake of simlicity, only record 1 of
     Linear fixed file is supported for Associate file */
  else if(mmgsdi_file_fs_info_table[index].associate_file_type ==
            MMGSDI_LINEAR_FIXED_FILE)
  {
    return mmgsdi_card_init_cache_record(
             session_id,
             mmgsdi_file_fs_info_table[index].app_type,
             slot,
             mmgsdi_file_fs_info_table[index].associate_file,
             1,
             data_ptr);
  }
  /* Only transparent and linear fixed files are suported for
     associate files for now */
  return MMGSDI_ERROR;
} /* mmgsdi_file_fs_read_assc_file */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_GET_ASSC_FILE_INDEX

DESCRIPTION
  This function searches through the File Header to see if there is a
  matching Associate file entry in the File to the File Data passed in
  and return the header index in case of successful match

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_get_assc_file_index(
  uint8             index,
  mmgsdi_data_type *data_ptr,
  uint8            *assc_file_index_ptr
)
{
  uint8                            file_index         = 0;
  mmgsdi_file_fs_file_header_type *fs_file_header_ptr = NULL;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE ||
     assc_file_index_ptr == NULL ||
     data_ptr == NULL ||
     data_ptr->data_ptr == NULL ||
     data_ptr->data_len == 0)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Check if file is read from FS and cached */
  if(!mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read ||
     mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* Get pointer to file header */
  fs_file_header_ptr =
    (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;

  /* Search for associate file match */
  for (file_index = 0; file_index < MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED;
       file_index++)
  {
    if(fs_file_header_ptr->file_assc_file_info[file_index].assc_file_len == 0)
    {
      continue;
    }
    if(!memcmp(data_ptr->data_ptr,
               fs_file_header_ptr->file_assc_file_info[file_index].assc_file_buf,
               fs_file_header_ptr->file_assc_file_info[file_index].assc_file_len))
    {
      *assc_file_index_ptr = file_index;
      return MMGSDI_SUCCESS;
    }
  }
  return MMGSDI_ERROR;
} /* mmgsdi_file_fs_get_assc_file_index */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_ADD_ASSC_FILE

DESCRIPTION
  This function shall add an Associate file into the File System
  Cache and return the associate file index

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_add_assc_file(
  uint8             index,
  mmgsdi_data_type *data_ptr,
  uint8            *assc_file_index_ptr
)
{
  uint32                           header_size        = sizeof(mmgsdi_file_fs_file_header_type);
  uint32                           file_size          = 0;
  uint8                           *fs_file_buf_ptr    = NULL;
  mmgsdi_file_fs_file_header_type *fs_file_header_ptr = NULL;
  uint32                           offset             = 0;
  uint8                            file_length        = 0;
  const uint8*                     default_values_ptr = NULL;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE ||
     assc_file_index_ptr == NULL ||
     data_ptr == NULL ||
     data_ptr->data_ptr == NULL ||
     data_ptr->data_len == 0)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Check if file is read from FS */
  if(!mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read)
  {
    return MMGSDI_ERROR;
  }

  /* Length of one file entry in the file body,
     Get file default length and values from mmgsdi_file_info_table */
  if (mmgsdi_file_fallback_get_file_info(mmgsdi_file_fs_info_table[index].file,
                                         &file_length,
                                         &default_values_ptr,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL)!= MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  /* If file is already read and cached, get a pointer to the cached file header */
  if(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr != NULL)
  {
    fs_file_header_ptr =
      (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;
  }

  /* File is not cached, need to create a file cache */
  if(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr == NULL)
  {
    /* File size for 1 file entry - since file is being created */
    file_size = header_size + file_length;
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr,
                                       file_size);
    if (mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }
    
    /* Update file info */
    fs_file_header_ptr =
      (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;    
    mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len = file_size;
    mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries = 1;
    *assc_file_index_ptr = 0;
    fs_file_header_ptr->next_entry_to_update = 1;
  }
  /* File is already full, so need to overwrite oldest entry */
  else if(mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries == MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED)
  {
    fs_file_header_ptr =
      (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;
    
    *assc_file_index_ptr = fs_file_header_ptr->next_entry_to_update;
    fs_file_header_ptr->next_entry_to_update++;
    if(fs_file_header_ptr->next_entry_to_update ==
       MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED)
    {
      fs_file_header_ptr->next_entry_to_update = 0;
    }
  }
  /* File is not full, need to add a new entry to the file */
  else if(mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries < MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED)
  {
    /* Calculate new file size adding 1 new entry */
    file_size = header_size + (file_length * (mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries + 1));
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(fs_file_buf_ptr,
                                       file_size);
    if (fs_file_buf_ptr == NULL)
    {
      return MMGSDI_ERROR;
    }
    /* Copy old file contents cache to the new cache */
    mmgsdi_memscpy(fs_file_buf_ptr,
                   file_size,
                   mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr,
                   mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len);
    MMGSDIUTIL_TMC_MEM_FREE(mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr);

    /* Update file info */
    mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr = fs_file_buf_ptr;
    mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_len = file_size;  
    fs_file_header_ptr =
      (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr; 

    /* Retrieve Index to update */
    *assc_file_index_ptr = mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries;

    /* Increment file entires */
    mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries++;

    /* Update file header info */
    fs_file_header_ptr->next_entry_to_update =
      mmgsdi_file_fs_info_table[index].file_info_ptr->no_of_entries;
    
    if(fs_file_header_ptr->next_entry_to_update ==
       MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED)
    {
      fs_file_header_ptr->next_entry_to_update = 0;
    }    
  }
  else
  {
    return MMGSDI_ERROR;
  }

  /* Copy associate file info to file header index */
  mmgsdi_memscpy(fs_file_header_ptr->file_assc_file_info[*assc_file_index_ptr].assc_file_buf,
                 sizeof(fs_file_header_ptr->file_assc_file_info[*assc_file_index_ptr].assc_file_buf),
                 data_ptr->data_ptr,
                 data_ptr->data_len);
  fs_file_header_ptr->file_assc_file_info[*assc_file_index_ptr].assc_file_len = (uint8)data_ptr->data_len;
  fs_file_header_ptr->file_assc_file_info[*assc_file_index_ptr].file_len = file_length;
  
  offset = sizeof(mmgsdi_file_fs_file_header_type) + 
             (*assc_file_index_ptr * file_length);

  /* Copy file data from the Default file data */
  mmgsdi_memscpy(&mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr[offset],
                 file_length,
                 default_values_ptr,
                 file_length);
  
  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_add_assc_file */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_ADD_FILE

DESCRIPTION
  This function shall add file data into the File System
  Cache corresponding to the associate file index passed

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_add_file(
  uint8                   index,
  const mmgsdi_data_type *data_ptr,
  uint8                   assc_file_index,
  uint32                  file_offset
)
{
  uint32                           offset             = 0;
  uint32                           len                = 0;
  mmgsdi_file_fs_file_header_type *fs_file_header_ptr = NULL;
  uint8                            file_length        = 0;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE ||
     assc_file_index >= MMGSDI_FILE_FS_NUM_PAIRS_SUPPORTED ||
     data_ptr == NULL ||
     data_ptr->data_ptr == NULL ||
     data_ptr->data_len == 0)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Check if file is read from FS */
  if(!mmgsdi_file_fs_info_table[index].file_info_ptr->is_fs_read)
  {
    return MMGSDI_ERROR;
  }

  fs_file_header_ptr =
    (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;

  /* Max. length of file */
  len = fs_file_header_ptr->file_assc_file_info[assc_file_index].file_len;

  /* If data to copy exceeds max. length, return */
  if(file_offset + data_ptr->data_len > len)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Calculate offset into the file system cache,
     Get file default length and values from mmgsdi_file_info_table */
  if (mmgsdi_file_fallback_get_file_info(mmgsdi_file_fs_info_table[index].file,
                                         &file_length,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL) != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  offset = sizeof(mmgsdi_file_fs_file_header_type) +
           (assc_file_index * file_length) +
           file_offset;

  /* Copy file data into File system cache */
  mmgsdi_memscpy(&mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr[offset],
                 len,
                 data_ptr->data_ptr,
                 data_ptr->data_len);

  mmgsdi_file_fs_info_table[index].file_info_ptr->is_write_needed = TRUE;

  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_add_file */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_GET_FS_INFO_TABLE_INDEX

DESCRIPTION
  This function retrieves the index in the mmgsdi_file_fs_info_table
  corresponding to the file requested

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fs_get_fs_info_table_index(
  mmgsdi_session_id_type    session_id,
  const mmgsdi_access_type *access_ptr,
  uint8                    *index_ptr
)
{
  uint8                 table_index = 0;
  mmgsdi_file_enum_type file        = MMGSDI_MAX_FILE_ENUM;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_2(access_ptr,
                              index_ptr);

  if(access_ptr->access_method == MMGSDI_BY_PATH_ACCESS)
  {
    file = mmgsdi_util_convert_path_to_enum(session_id,
                                            access_ptr->file.path_type);
  }
  else if(access_ptr->access_method == MMGSDI_EF_ENUM_ACCESS)
  {
    file = access_ptr->file.file_enum;
  }

  for(table_index = 0; table_index < MMGSDI_FILE_FS_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_fs_info_table[table_index].file == file)
    {
      *index_ptr = table_index;
      return MMGSDI_SUCCESS;
    }
  }
  return MMGSDI_NOT_FOUND;
} /* mmgsdi_file_fs_get_fs_info_table_index */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_VALIDATE_AND_GET_ACCESS_PARAMS

DESCRIPTION
  This function shall validate the access parameters for
  Read or Write access and return the File Offset and File
  Length to use

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_file_fs_validate_and_get_access_params(
  mmgsdi_session_id_type                   session_id,
  mmgsdi_cmd_enum_type                     request_type,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr,
  uint8                                    index,
  uint32                                  *file_offset,
  uint32                                  *file_len,
  boolean                                 *use_actual_data_ptr
)
{
  mmgsdi_int_app_info_type                      *app_info_ptr        = NULL;
  mmgsdi_file_attributes_type                    file_attr;
  mmgsdi_return_enum_type                        mmgsdi_status       = MMGSDI_SUCCESS;
  mmgsdi_rec_num_type                            num_of_rec          = 0;
  mmgsdi_len_type                                rec_len             = 0;
  uint8                                          full_file_length    = 0;
  mmgsdi_file_info_table_entry_categ_enum_type   table_entry_categ   = MMGSDI_ENTRY_CATEG_CACHE_FALLBACK;
  mmgsdi_get_file_attr_data_type                 raw_attr_data;

  memset(&file_attr, 0x00, sizeof(file_attr));
  memset(&raw_attr_data, 0x00, sizeof(raw_attr_data));

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_3(extra_param_ptr,
                              file_offset,
                              file_len);

  /* Check input data */
  if(request_type != MMGSDI_READ_REQ &&
     request_type != MMGSDI_WRITE_REQ)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Check index */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Get file default length and values from mmgsdi_file_info_table */
  if (mmgsdi_file_fallback_get_file_info(mmgsdi_file_fs_info_table[index].file,
                                         &full_file_length,
                                         NULL,
                                         &table_entry_categ,
                                         NULL,
                                         &raw_attr_data.data_len,
                                         &raw_attr_data.data_ptr) != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }
  
  /* If file fallback category does not allow fallback to FS or
     if fallback to FS is disabled, return */
  if ((table_entry_categ != MMGSDI_ENTRY_CATEG_FS_FALLBACK && 
       table_entry_categ != MMGSDI_ENTRY_CATEG_FS_OR_CACHE_FALLBACK) ||
	  (mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) != 
       MMGSDI_FEATURE_ENABLED))
  {
    MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
    return MMGSDI_ERROR;
  }

  *file_offset = 0;
  *file_len = 0;

  file_attr.file_type = mmgsdi_file_fs_info_table[index].file_type;

  /* Only 1 record is supported for record based files */
  if (file_attr.file_type == MMGSDI_LINEAR_FIXED_FILE || 
      file_attr.file_type == MMGSDI_CYCLIC_FILE)
  {
    rec_len = full_file_length;
    num_of_rec = 1;
  }

  if (MMGSDI_SUCCESS != mmgsdi_util_get_session_app_info(session_id,
                                                         NULL,
                                                         NULL,
                                                         &app_info_ptr,
                                                         NULL,
                                                         NULL,
                                                         NULL) || app_info_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* For some buggy CARD scenario,CARD returns success response for Select,
     But CARD returns Fail response for Read and Write.
     While supporting file fallback mechanism for the file,
     MMGSDI should delete atributes of the file which MMGSDI got from card previously */
  (void)mmgsdi_cache_delete_attr_selective_items(&(app_info_ptr->attr_cache_tbl),
                                                 NULL,
                                                 0,
                                                 &extra_param_ptr->get_file_attr_data.access.file.file_enum,
                                                 1);

  /* read the cache , if the cache is present then check the attr ,
     if the cache is not present then write the default attr */
  mmgsdi_status = mmgsdi_cache_check_and_write_default_attr(session_id,
                                                            extra_param_ptr->get_file_attr_data.access,
                                                            mmgsdi_file_fs_info_table[index].file,
                                                            file_attr.file_type,
                                                            raw_attr_data);

  MMGSDIUTIL_TMC_MEM_FREE(raw_attr_data.data_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS) 
  {
    return mmgsdi_status;
  }

  /* Get app. info to obtain pin status */
  if (MMGSDI_SUCCESS == mmgsdi_util_get_session_app_info(session_id,
                                         NULL,
                                         NULL,
                                         &app_info_ptr,
                                         NULL,
                                         NULL,
                                         NULL))
  {
    if (app_info_ptr != NULL)
    {
      /* Check to see if the security attributes allow to read/write file */
      mmgsdi_status = mmgsdi_util_check_file_attr(session_id,
                                                   app_info_ptr,
                                                   &extra_param_ptr->get_file_attr_data.access,
                                                   request_type,
                                                   MMGSDI_DECODE_ACTUAL_ATTRIBUTES);

      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        return mmgsdi_status;
      }
    }
  }

  switch(mmgsdi_file_fs_info_table[index].file_type)
  {
    case MMGSDI_TRANSPARENT_FILE:
      if(request_type == MMGSDI_READ_REQ)
      {
        /* Read length cannot exceed file length */
        if(extra_param_ptr->read_data.data_len + extra_param_ptr->read_data.offset > full_file_length)
        {
           return MMGSDI_ERROR;
        }
        *file_offset = extra_param_ptr->read_data.offset;
        /* Read entire file from offset if 0 length is requested */
        if(extra_param_ptr->read_data.data_len == 0)
        {
          *file_len = full_file_length - *file_offset;
        }        
        else
        {
          *file_len = extra_param_ptr->read_data.data_len;
        }
      }
      else if(request_type == MMGSDI_WRITE_REQ)
      {
        /* Write length cannot exceed file length */
        if(extra_param_ptr->write_data.data.data_len + extra_param_ptr->write_data.offset > full_file_length)
        {
           /* For FPLMN if MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN it is possible that client might write
              extended FPLMN size to cache, on which here we will have to check the actual data which 
              was written to card */
           if(mmgsdi_file_fs_info_table[index].file == MMGSDI_GSM_FPLMN  &&
              mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SUPPORT_EXTENDED_FPLMN) == MMGSDI_FEATURE_ENABLED &&
              use_actual_data_ptr != NULL)
           {
             if(extra_param_ptr->write_data.actual_data.data_len != 0 &&
                (extra_param_ptr->write_data.actual_data.data_len + extra_param_ptr->write_data.offset <= full_file_length))
             {
               *file_offset = extra_param_ptr->write_data.offset;
               *file_len = extra_param_ptr->write_data.actual_data.data_len;
               *use_actual_data_ptr = TRUE;
               break;
             }
           }

           return MMGSDI_ERROR;
        }
        /* Write length cannot be 0 */
        if(extra_param_ptr->write_data.data.data_len == 0)
        {
          return MMGSDI_ERROR;
        }        
        *file_offset = extra_param_ptr->write_data.offset;
        *file_len = extra_param_ptr->write_data.data.data_len;
      }
      break;
    case MMGSDI_LINEAR_FIXED_FILE:
    case MMGSDI_CYCLIC_FILE:
      if(request_type == MMGSDI_READ_REQ)
      {
        /* Only record 1 supported for record based files */
        if(extra_param_ptr->read_data.rec_num != 1)
        {
          return MMGSDI_ERROR;
        }
        /* Read entire record from FS */
        *file_len = full_file_length;
      }
      else if(request_type == MMGSDI_WRITE_REQ)
      {
        /* Only record 1 supported for record based files */
        if(extra_param_ptr->write_data.rec_num != 1)
        {
          return MMGSDI_ERROR;
        }
        /* Entire record needs to be written into */
        if(extra_param_ptr->write_data.data.data_len != full_file_length)
        {
           return MMGSDI_ERROR;
        }
        *file_len = extra_param_ptr->write_data.data.data_len;        
      }      
      break;
    default:
      return MMGSDI_ERROR;
  }
  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_validate_and_get_access_params */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_CHECK_AND_GET_FILE_DATA_FROM_FS

DESCRIPTION
  This function shall retrieve file data from file system if there
  is a valid entry present, if not would return default data

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fs_check_and_get_file_data_from_fs(
  const mmgsdi_request_header_type *req_header_ptr,
  uint8                             index,
  mmgsdi_data_type                 *data_ptr,
  uint32                            req_file_offset,
  uint32                            req_file_len
)
{
  mmgsdi_return_enum_type          mmgsdi_status      = MMGSDI_SUCCESS;
  mmgsdi_data_type                 assc_data          = {0};
  uint8                            assc_file_index    = 0;
  uint32                           offset             = 0;
  mmgsdi_file_fs_file_header_type *fs_file_header_ptr = NULL;
  uint8                            file_length        = 0;
  const uint8*                     default_values_ptr = NULL;

  /* Check input data */
  if(index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE ||
     req_header_ptr == NULL ||
     data_ptr == NULL)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* Get file default length and values from mmgsdi_file_info_table */
  mmgsdi_status = mmgsdi_file_fallback_get_file_info(mmgsdi_file_fs_info_table[index].file,
                                                     &file_length,
                                                     &default_values_ptr,
                                                     NULL,
                                                     NULL,
                                                     NULL,
                                                     NULL);
  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  do
  {
    /* Read and parse the file in FS */
    mmgsdi_status = mmgsdi_file_fs_read_file(index);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Read associate file value from card */
    mmgsdi_status = mmgsdi_file_fs_read_assc_file(req_header_ptr->session_id,
                                                  req_header_ptr->slot_id,
                                                  index,
                                                  &assc_data);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Get index of the associate file in the file header */
    mmgsdi_status = mmgsdi_file_fs_get_assc_file_index(index,
                                                       &assc_data,
                                                       &assc_file_index);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    fs_file_header_ptr =
      (mmgsdi_file_fs_file_header_type*)mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr;

    /* If data to copy exceeds max. length, return */
    if(req_file_offset + req_file_len > 
         fs_file_header_ptr->file_assc_file_info[assc_file_index].file_len)
    {
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
    }

    /* Calculate offset into the file system cache */
    offset = sizeof(mmgsdi_file_fs_file_header_type) + 
             (assc_file_index * file_length) +
             req_file_offset;

    /* Allocate memory for the reading data */
    MMGSDIUTIL_TMC_MEM_FREE(data_ptr->data_ptr);    
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(data_ptr->data_ptr, req_file_len);

    if(data_ptr->data_ptr == NULL)
    {
      mmgsdi_status = MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
      break;
    }

   /* Copy file data from the File system cache */
    data_ptr->data_len = req_file_len;
    mmgsdi_memscpy(data_ptr->data_ptr,
                   data_ptr->data_len,
                   &mmgsdi_file_fs_info_table[index].file_info_ptr->fs_file_buf_ptr[offset],
                   req_file_len);
  }while(0);

  MMGSDIUTIL_TMC_MEM_FREE(assc_data.data_ptr);

  /* If File data not found in File System and there are no input
     param errors/heap exhaustion issues, return default data
     from File FS Info table*/
  if((mmgsdi_status != MMGSDI_SUCCESS) &&
     (mmgsdi_status != MMGSDI_INCORRECT_PARAMS) &&
     (mmgsdi_status != MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED))
  {
    /* If data to copy is valid */
    if(req_file_offset + req_file_len <= file_length)
    {
      /* Memory allocation */
      MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(data_ptr->data_ptr, req_file_len);
      if (data_ptr->data_ptr)
      {
        /* Copy file data from the Default file data */
         data_ptr->data_len = req_file_len;
         mmgsdi_memscpy(data_ptr->data_ptr,
                        data_ptr->data_len,
                        &default_values_ptr[req_file_offset],
                        req_file_len);
         mmgsdi_status = MMGSDI_SUCCESS;
      }
    }  
  }
  
  return mmgsdi_status;  
} /* mmgsdi_file_fs_check_and_get_file_data_from_fs */


/*=============================================================================
FUNCTION MMGSDI_FILE_FS_CHECK_AND_WRITE_FILE_DATA_TO_FS

DESCRIPTION
  This function shall write file data into the File System
  corresponding to the file index passed

DEPENDENCIES
  None

LIMITATIONS
  NONE

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  NONE
=============================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fs_check_and_write_file_data_to_fs (
  const mmgsdi_request_header_type        *req_header_ptr,
  uint8                                    index,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr
)
{
  mmgsdi_return_enum_type mmgsdi_status     = MMGSDI_SUCCESS;
  mmgsdi_data_type        assc_data;
  uint8                   assc_file_index   = 0;
  uint32                  file_offset       = 0;
  uint32                  file_len          = 0;
  boolean                 write_actual_data = FALSE;

  memset(&assc_data, 0x00, sizeof(assc_data));

  /* Irrespective of successful read from FS or not, we need to proceed
     to attempt write into FS - hence return value ignored */
  (void)mmgsdi_file_fs_read_file(index);

  do
  {
    /* Validate access params */
    mmgsdi_status = mmgsdi_file_fs_validate_and_get_access_params(
                      req_header_ptr->session_id,
                      MMGSDI_WRITE_REQ,
                      extra_param_ptr,
                      index,
                      &file_offset,
                      &file_len,
                      &write_actual_data);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Read associate file value from card */
    mmgsdi_status = mmgsdi_file_fs_read_assc_file(req_header_ptr->session_id,
                                                  req_header_ptr->slot_id,
                                                  index,
                                                  &assc_data);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      break;
    }

    /* Get index of the associate file in the file header */
    mmgsdi_status = mmgsdi_file_fs_get_assc_file_index(index,
                                                       &assc_data,
                                                       &assc_file_index);

    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
      /* If associate file not present in file system,
         add the same */
      mmgsdi_status = mmgsdi_file_fs_add_assc_file(index,
                                                   &assc_data,
                                                   &assc_file_index);

      if(mmgsdi_status != MMGSDI_SUCCESS)
      {
        break;
      }    
    }

    /* Add the file value to File System */
    mmgsdi_status = mmgsdi_file_fs_add_file(index,
                                            write_actual_data ? &extra_param_ptr->write_data.actual_data : &extra_param_ptr->write_data.data,
                                            assc_file_index,
                                            file_offset);
    
    if(mmgsdi_status != MMGSDI_SUCCESS)
    {
     break;
    }   

    /* Write File cache back to file system */
    mmgsdi_status = mmgsdi_file_fs_write_file(index);
  }while(0);

  MMGSDIUTIL_TMC_MEM_FREE(assc_data.data_ptr);
  
  return mmgsdi_status;
} /* mmgsdi_file_fs_check_and_write_file_data_to_fs */


/*===========================================================================
FUNCTION MMGSDI_FILE_FS_CHECK_AND_UPDATE_SERVICE_TABLE_VAL

DESCRIPTION
  This function will parse the UIM response to Read request
  on a Service Table file and update the Read value if the
  Files corresponding to the service table entries are supported
  w.r.t. file system fallback

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
static void  mmgsdi_file_fs_check_and_update_service_table_val (
  mmgsdi_uim_report_rsp_type              *mod_uim_rsp_ptr,
  const mmgsdi_request_header_type        *req_header_ptr,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr
)
{
  uint32                    table_index   = 0;
  uint32                    base_srv      = 0;
  mmgsdi_service_enum_type  max_srv       = MMGSDI_SRV_MAX;
  uint32                    service       = 0;
  uint32                    offset        = 0x00;
  uint8                     mask          = 0x01;
  uint8                     shift         = 0x01;
  mmgsdi_data_type          srv_data      = {0};
  mmgsdi_return_enum_type   mmgsdi_status = MMGSDI_SUCCESS;
  mmgsdi_int_app_info_type *sel_app_ptr   = NULL;


  /* Check input pointers */
  MMGSDIUTIL_RETURN_WITHOUT_VALUE_IF_NULL_3(mod_uim_rsp_ptr,
                                            req_header_ptr,
                                            extra_param_ptr);

  /* Check input params */
  if(extra_param_ptr->read_data.access.access_method != MMGSDI_EF_ENUM_ACCESS ||
     mod_uim_rsp_ptr == NULL ||
     mod_uim_rsp_ptr->rsp_data.data_len == 0)
  {
    return;
  }

  /* Get base service for Service files */
  switch(extra_param_ptr->read_data.access.file.file_enum)
  {
    case MMGSDI_USIM_UST:
      base_srv = MMGSDI_USIM_SRV;
      break;
    case MMGSDI_CSIM_CST:
      base_srv = MMGSDI_CSIM_SRV;
      break;
    /* Other files - nothing to do */
    default:
      break;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(
                        req_header_ptr->session_id,
                        NULL,
                        NULL,
                        &sel_app_ptr,
                        NULL,
                        NULL,
                        NULL);
  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (sel_app_ptr == NULL))
  {
    return;
  }

  /* Copy the original UST/CST value in the cache */
  MMGSDIUTIL_TMC_MEM_FREE(sel_app_ptr->srv_table_orig_cache.data_ptr);
  
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(
    sel_app_ptr->srv_table_orig_cache.data_ptr,
    mod_uim_rsp_ptr->rsp_data.data_len);

  if(sel_app_ptr->srv_table_orig_cache.data_ptr == NULL)
  {
    return;
  }

  sel_app_ptr->srv_table_orig_cache.data_len =
    mod_uim_rsp_ptr->rsp_data.data_len;
 
  mmgsdi_memscpy(sel_app_ptr->srv_table_orig_cache.data_ptr,
                 sel_app_ptr->srv_table_orig_cache.data_len,
                 mod_uim_rsp_ptr->rsp_data.data_ptr,
                 mod_uim_rsp_ptr->rsp_data.data_len);

  /* Find out the max. UST/CST service in the FS info table */
  max_srv = base_srv;
  
  for(table_index = 0; table_index < MMGSDI_FILE_FS_INFO_TABLE_SIZE; table_index++)
  {
    /* Check if USIM/CSIM Service */
    if((mmgsdi_file_fs_info_table[table_index].srv & 0xFF00) == base_srv)
    {      
      if(mmgsdi_file_fs_info_table[table_index].srv > max_srv)
      {
        max_srv = mmgsdi_file_fs_info_table[table_index].srv;
      }      
    }
  }

  service = (uint32)max_srv - base_srv;
  offset = service / 8;

  /* If Service offset is greater than current UST/CST length,
     increase UST/CST length */
  if(offset >= mod_uim_rsp_ptr->rsp_data.data_len)
  {
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(srv_data.data_ptr, (offset + 1));

    if(srv_data.data_ptr == NULL)
    {
      return;
    }

   /* Copy current UST/CST data into new buffer */
    srv_data.data_len = offset + 1;
    mmgsdi_memscpy(srv_data.data_ptr,
                   srv_data.data_len,
                   mod_uim_rsp_ptr->rsp_data.data_ptr,
                   mod_uim_rsp_ptr->rsp_data.data_len);

    MMGSDIUTIL_TMC_MEM_FREE(mod_uim_rsp_ptr->rsp_data.data_ptr);

    /* Point Rsp data to new buffer */
    mod_uim_rsp_ptr->rsp_data.data_ptr = srv_data.data_ptr;
    mod_uim_rsp_ptr->rsp_data.data_len = srv_data.data_len;
  } 

  /* Update UST/CST data with enablement for files with fallback support */
  for(table_index = 0; table_index < MMGSDI_FILE_FS_INFO_TABLE_SIZE; table_index++)
  {
    /* Check if USIM/CSIM Service */
    if((mmgsdi_file_fs_info_table[table_index].srv & 0xFF00) == base_srv)
    {
      service = (uint32)mmgsdi_file_fs_info_table[table_index].srv - base_srv;
      offset = service / 8;
      shift =  service % 8;
      mask = 0x01 << shift;
      if(offset < mod_uim_rsp_ptr->rsp_data.data_len)
      {
        mod_uim_rsp_ptr->rsp_data.data_ptr[offset] = 
          mod_uim_rsp_ptr->rsp_data.data_ptr[offset] | mask;
      }       
    }
  }  
} /* mmgsdi_file_fs_check_and_update_service_table_val */


/*===========================================================================
FUNCTION MMGSDI_FILE_FS_CHECK_AND_RETRIEVE_FILE_DATA

DESCRIPTION:
  This function will parse the UIM response to Read request
  and provide content from File System if the file being read
  is supported w.r.t. file system fallback

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fs_check_and_retrieve_file_data (
  mmgsdi_uim_report_rsp_type              *mod_uim_rsp_ptr,
  const mmgsdi_request_header_type        *req_header_ptr,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr,  
  mmgsdi_return_enum_type                  report_status
)
{
  uint8                   index         = 0;
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_SUCCESS;
  uint32                  file_offset   = 0;
  uint32                  file_len      = 0;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_3(mod_uim_rsp_ptr,
                              req_header_ptr,
                              extra_param_ptr);                       

  /* Get the index of the file being requested in the
     FS info table */
  mmgsdi_status = mmgsdi_file_fs_get_fs_info_table_index(
                    req_header_ptr->session_id,
                    &extra_param_ptr->read_data.access,
                    &index);              

  /* Return if file is not found in the table */
  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE))
  {
    return report_status;
  }

  /* Validate access params */
  mmgsdi_status = mmgsdi_file_fs_validate_and_get_access_params(
                    req_header_ptr->session_id,
                    MMGSDI_READ_REQ,
                    extra_param_ptr,
                    index,
                    &file_offset,
                    &file_len,
                    NULL);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    /* If PIN status does not allow access to the file, return appropriate error code */
    if (mmgsdi_status == MMGSDI_ACCESS_DENIED)
    {
      return mmgsdi_status;
    }

    return report_status;
  }

  /* Retrieve file data from file system */
  mmgsdi_status = mmgsdi_file_fs_check_and_get_file_data_from_fs(req_header_ptr,
                                                                 index,
                                                                 &mod_uim_rsp_ptr->rsp_data,
                                                                 file_offset,
                                                                 file_len);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return report_status;
  }
  
  mod_uim_rsp_ptr->cmd_transacted = TRUE;
  mod_uim_rsp_ptr->is_uim_success = TRUE;
  mod_uim_rsp_ptr->mmgsdi_error_status = MMGSDI_SUCCESS;
  mod_uim_rsp_ptr->status_word.sw1 = GSDI_SIM_SW1_NORM_END;
  mod_uim_rsp_ptr->status_word.sw2 = GSDI_SIM_SW2_NORM_END;

  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_check_and_retrieve_file_data */


/*===========================================================================
FUNCTION MMGSDI_FILE_FS_CHECK_AND_WRITE_FILE_DATA

DESCRIPTION:
  This function will parse the UIM response to Write request
  and write content to File System if the file being written into
  is supported w.r.t. file system fallback

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  MMGSDI_RETURN_ENUM_TYPE

SIDE EFFECTS
  None
===========================================================================*/
static mmgsdi_return_enum_type  mmgsdi_file_fs_check_and_write_file_data (
  mmgsdi_uim_report_rsp_type              *mod_uim_rsp_ptr,
  const mmgsdi_request_header_type        *req_header_ptr,
  const mmgsdi_client_req_extra_info_type *extra_param_ptr,  
  mmgsdi_return_enum_type                  report_status
)
{
  uint8                   index         = 0;
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_SUCCESS;

  /* Check input pointers */
  MMGSDIUTIL_RETURN_IF_NULL_3(mod_uim_rsp_ptr,
                              req_header_ptr,
                              extra_param_ptr);                       

  /* Get the index of the file being requested in the
     FS info table */
  mmgsdi_status = mmgsdi_file_fs_get_fs_info_table_index(
                    req_header_ptr->session_id,
                    &extra_param_ptr->write_data.access,
                    &index);              

  /* Return if file is not found in the table */
  if((mmgsdi_status != MMGSDI_SUCCESS) ||
     (index >= MMGSDI_FILE_FS_INFO_TABLE_SIZE))
  {
    return report_status;
  }

  /* Write file data to file system */
  mmgsdi_status = mmgsdi_file_fs_check_and_write_file_data_to_fs(req_header_ptr,
                                                                 index,
                                                                 extra_param_ptr);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    return report_status;
  }
    
  mod_uim_rsp_ptr->cmd_transacted = TRUE;
  mod_uim_rsp_ptr->is_uim_success = TRUE;
  mod_uim_rsp_ptr->mmgsdi_error_status = MMGSDI_SUCCESS;
  mod_uim_rsp_ptr->status_word.sw1 = GSDI_SIM_SW1_NORM_END;
  mod_uim_rsp_ptr->status_word.sw2 = GSDI_SIM_SW2_NORM_END;

  return MMGSDI_SUCCESS;
} /* mmgsdi_file_fs_check_and_write_file_data */


/* ==========================================================================
FUNCTION:      MMGSDI_UTIL_IS_FILE_OK_IN_ORIG_SVC_TABLE

DESCRIPTION:
  This function checks if the file access is allowed by the original
  service table value from the SIM card or not

DEPENDENCIES:
  None

LIMITATIONS:
  The function is limited to:
  1. Checking access via MMGSDI_EF_ENUM_ACCESS
  2. Checking access on files for which fallback to FS support is present
  For files not meeting above criteria, function returns success
  indicating access allowed.

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS:          OK to access the file.
  MMGSDI_ACCESS_DENIED:    Access conditions not satisfied for this EF.
  MMGSDI_NOT_SUPPORTED:    It is not OK to access the file (for reasons
                           other than access violation).

SIDE EFFECTS:
  None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_is_file_ok_in_orig_svc_table (
  mmgsdi_session_id_type    session_id,
  mmgsdi_app_enum_type      app_type,
  const mmgsdi_access_type *mmgsdi_access_ptr
)
{
  mmgsdi_return_enum_type   mmgsdi_status = MMGSDI_SUCCESS;
  mmgsdi_service_enum_type  service_type  = MMGSDI_NONE;
  uint32                    service       = 0;
  uint32                    offset        = 0x00;
  uint32                    mask          = 0x01;
  uint32                    shift         = 0x01;
  boolean                   is_gsdi_task  = TRUE;
  mmgsdi_int_app_info_type *app_info_ptr  = NULL;
  boolean                   srv_available = TRUE;
  uint8                     table_index   = 0;
  mmgsdi_task_enum_type     mmgsdi_task   = MMGSDI_TASK_MAX;

  if(mmgsdi_access_ptr == NULL ||
     mmgsdi_access_ptr->access_method != MMGSDI_EF_ENUM_ACCESS)
  {
    /* Allow the command to the card */
    return MMGSDI_SUCCESS;
  }

  /* Get service type for file */
  mmgsdi_status = mmgsdi_file_get_srv_from_file(
    app_type,
    mmgsdi_access_ptr->file.file_enum,
    &service_type);

  if(mmgsdi_status != MMGSDI_SUCCESS)
  {
    /* Can't find in the table mapping, allow the command to go to the
       card */
    return MMGSDI_SUCCESS;
  }

  if(service_type == MMGSDI_NONE)
  {
    /* No service check associated with the file enum, allow the command to
       go to the card */
    return MMGSDI_SUCCESS;
  }

  /* Check if file is suported w.r.t. fallback to FS */
  for(table_index = 0; table_index < MMGSDI_FILE_FS_INFO_TABLE_SIZE; table_index++)
  {
    if(mmgsdi_file_fs_info_table[table_index].file == mmgsdi_access_ptr->file.file_enum)
    {
      break;
    }
  }

  /* Allow command to go to card as file not supported w.r.t. fallback to FS */
  if(table_index == MMGSDI_FILE_FS_INFO_TABLE_SIZE)
  {
    return MMGSDI_SUCCESS;
  }

  switch(app_type)
  {
    case MMGSDI_APP_USIM:
      if(((uint32)service_type & 0xFF00) != MMGSDI_USIM_SRV)
      {
        /* Invalid service type for USIM, allow command to go to card */
        return MMGSDI_SUCCESS;
      }
      service = (uint32)service_type - MMGSDI_USIM_SRV;
      offset = service / 8;
      shift =  service % 8;

      mask = mask << shift;      
      break;

    /* Expand to other app types if new files added to
       mmgsdi_file_fs_info_table_type which are non-USIM apps */
    default:
      return MMGSDI_SUCCESS;
  }

  /* Check if function is invoked in GSDI task context */
  mmgsdi_task = mmgsdi_util_get_current_task();
  if(!MMGSDIUTIL_IS_MMGSDI_TASK(mmgsdi_task))
  {
    is_gsdi_task = FALSE;
  }

  /* Protect read access to client ID reg. table/App. table (only
     required in non-task context) */
  if(!is_gsdi_task)
  {
    MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;
  }

  /* Get app. info */
  (void)mmgsdi_util_get_session_app_info(session_id,
                                         NULL,
                                         NULL,
                                         &app_info_ptr,
                                         NULL,
                                         NULL,
                                         NULL);
  
  if ((mmgsdi_status == MMGSDI_SUCCESS) && (app_info_ptr != NULL))
  {
    /* Check Orig. Srv. Table cache if service is enabled */   
    if((offset < app_info_ptr->srv_table_orig_cache.data_len) &&
       (app_info_ptr->srv_table_orig_cache.data_ptr != NULL))
    {
      if (((app_info_ptr->srv_table_orig_cache.data_ptr[offset]) & mask) == mask)
      {
        srv_available = TRUE;
      }
      else
      {
        srv_available = FALSE;
      }
    }
  }
  
  if(!is_gsdi_task)
  {
    MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;
  }      
  
  if(srv_available)
  {
    return MMGSDI_SUCCESS;
  }
  
  return MMGSDI_ACCESS_DENIED;
} /* mmgsdi_file_fs_is_file_ok_in_orig_svc_table */


/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_READ_RSP

DESCRIPTION:
  This function will parse the UIM response to a Read request
  and provide content from File System if file system fallback
  is enabled for the file

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type

  MMGSDI_SUCCESS: Read from card was successful or data was read
                  from FS
  report_status:  Incorrect input params/Fallback to FS disabled

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_parse_uim_read_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
)
{
  mmgsdi_int_app_info_type        *app_info_ptr       = NULL;
  mmgsdi_pin_status_enum_type 	   pin1_status        = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
  mmgsdi_return_enum_type          mmgsdi_status      = MMGSDI_SUCCESS;

  /* Check input pointers */
  if((mod_uim_rsp_ptr == NULL) ||
     (req_header_ptr == NULL) ||
     (extra_param_ptr == NULL))
  {
    return report_status;
  }

  /* Check if SIM file fallback to FS is supported */
  if(mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) ==
       MMGSDI_FEATURE_DISABLED)
  {
    return report_status;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(req_header_ptr->session_id,
                                                   NULL,
                                                   NULL,
                                                   &app_info_ptr,
                                                   NULL,
                                                   NULL,
                                                   NULL);

  if (mmgsdi_status == MMGSDI_SUCCESS && app_info_ptr != NULL && app_info_ptr->pin1_ptr != NULL)
  {
    pin1_status = app_info_ptr->pin1_ptr->status;
  }

  /* If the file being read is a service table, check if content needs
     to be updated w.r.t. files that have fallback support */
  if((report_status == MMGSDI_SUCCESS) &&
     (extra_param_ptr->read_data.access.access_method == MMGSDI_EF_ENUM_ACCESS) &&
     (extra_param_ptr->read_data.access.file.file_enum == MMGSDI_USIM_UST))
  {
    mmgsdi_file_fs_check_and_update_service_table_val(mod_uim_rsp_ptr,
                                                      req_header_ptr,
                                                      extra_param_ptr);
  }
  /* If file is not found on card or access is denied even if PIN status is valid for access or scenario is EF-INCONSISTENT error,
     check if content can be retrieved from file system */
  else if(report_status == MMGSDI_NOT_FOUND || (report_status == MMGSDI_ACCESS_DENIED && 
          (pin1_status == MMGSDI_PIN_DISABLED || pin1_status == MMGSDI_PIN_ENABLED_VERIFIED))||
          report_status == MMGSDI_EF_INCONSISTENT)
  {
    report_status = mmgsdi_file_fs_check_and_retrieve_file_data(mod_uim_rsp_ptr,
                                                                req_header_ptr,
                                                                extra_param_ptr,
                                                                report_status);
  }
  
  return report_status;
} /* mmgsdi_file_fs_parse_uim_read_rsp */


/*===========================================================================
FUNCTION:      MMGSDI_FILE_FS_PARSE_UIM_WRITE_RSP

DESCRIPTION:
  This function will parse the UIM response to a Write request
  and provide content from File System if file system fallback
  is enabled for the file

DEPENDENCIES:
  None

LIMITATIONS:
  None

RETURN VALUE:
  mmgsdi_return_enum_type
 
  MMGSDI_SUCCESS: Write to card was successful or data was written
                  into FS
  report_status:  Incorrect input params/Fallback to FS disabled

SIDE EFFECTS:
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_file_fs_parse_uim_write_rsp (
  mmgsdi_uim_report_rsp_type               * mod_uim_rsp_ptr,
  const mmgsdi_request_header_type         * req_header_ptr,
  const mmgsdi_client_req_extra_info_type  * extra_param_ptr,  
  mmgsdi_return_enum_type                    report_status
)
{ 
  mmgsdi_int_app_info_type        *app_info_ptr       = NULL;
  mmgsdi_pin_status_enum_type 	   pin1_status        = MMGSDI_PIN_STATUS_NOT_INITIALIZED;
  mmgsdi_return_enum_type          mmgsdi_status      = MMGSDI_SUCCESS;

  /* Check input pointers */
  if((mod_uim_rsp_ptr == NULL) ||
     (req_header_ptr == NULL) ||
     (extra_param_ptr == NULL))
  {
    return report_status;
  }

  /* Check if SIM file fallback to FS is supported */
  if(mmgsdi_nv_get_device_feature_status(MMGSDI_FEATURE_SIM_FILE_FALLBACK_TO_FS) ==
       MMGSDI_FEATURE_DISABLED)
  {
    return report_status;
  }

  mmgsdi_status = mmgsdi_util_get_session_app_info(req_header_ptr->session_id,
                                                   NULL,
                                                   NULL,
                                                   &app_info_ptr,
                                                   NULL,
                                                   NULL,
                                                   NULL);

  if (mmgsdi_status == MMGSDI_SUCCESS && app_info_ptr != NULL && app_info_ptr->pin1_ptr != NULL)
  {
    pin1_status = app_info_ptr->pin1_ptr->status;
  }
  
  /* If file is not found on card or access is denied even if PIN status is valid for access,
     check if content can be written into the file system */
  if(report_status == MMGSDI_NOT_FOUND || (report_status == MMGSDI_ACCESS_DENIED && 
     (pin1_status == MMGSDI_PIN_DISABLED || pin1_status == MMGSDI_PIN_ENABLED_VERIFIED)))
  {
    report_status = mmgsdi_file_fs_check_and_write_file_data(mod_uim_rsp_ptr,
                                                             req_header_ptr,
                                                             extra_param_ptr,
                                                             report_status);
  }
  
  return report_status;
} /* mmgsdi_file_fs_parse_uim_write_rsp */


#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
