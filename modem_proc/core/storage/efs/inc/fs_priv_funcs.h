/***********************************************************************
 * fs_priv_funcs.h
 *
 * EFS Private Functions
 * Copyright (C) 2009-2013, 2015-2016,2018-2019 QUALCOMM Technologies, Inc.
 *
 * EFS APIs that are not public and called only internally by other EFS APIs.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs/inc/fs_priv_funcs.h#2 $ $DateTime: 2019/12/17 05:16:47 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-11-25   vm    Fix the mutex locking order between fs_lib and rest.
2019-11-24   vm    Add compressed FSG zip-tar support.
2018-03-08   eo    Cleanup non public apis from EFS public headers
2016-12-19   rp    Cleanup in recursive iteration of files during zip-tar
2016-09-21   np    Add support to extract all non secure EFS-files as zip-tar
2016-06-01   rp    Avoid delete and recreate of EFS private directories.
2016-05-23   rp    Support OEMs to create their own EFS-golden-copy.
2016-04-27   rp    Do EFS golden copy files list restore outside efs-init
2016-04-14   rp    Add EFS golden copy creation from files list.
2016-01-21   mj    Added support for RCINIT Group 7 shutdown notification.
2015-25-05   dks   Add function to read efs items using bigger sized buffer.
2015-02-05   vm    Avoid reading sfs folder while extracting EFS to TAR.
2013-06-19   wek   Add function to do a complete iteration of efs scrub.
2013-06-19   dks   Added NAND Flash scrub support.
2012-12-14   wek   Implement modified efs_utime for item files.
2012-10-18   wek   Make efs_raw_put/get wrappers around efs_put/get.
2012-04-30   nr    Register shutdown callback with RCINIT.
2011-10-11   nr    Added Zipped Tar file support.
2011-07-05   wek   Add file system image creation APIs.
2011-07-01   wek   Create a TAR file from a given directory.
2010-11-19   vr    Moved from efs/src to efs/inc since fs_tar needs this.
2010-10-28   rp    Add efs_auto_mkdir().
2010-09-01   wek   Remove compile time calculations based on page size.
2010-08-10   dks   Moved efs_iter define here for access from benchmark.
2010-02-06   yog   Validate the input param for NULL check.
2010-05-06   wek   Implement golden copy for remote storage page manager.
2010-02-08   rp    CMI compliance, remove #ifdef from API files.
2009-11-04   dks   path arg included in get_fs_data and get_device_info
2009-05-18   rp    Create

===========================================================================*/

#ifndef __FS_PRIV_FUNCS_H__
#define __FS_PRIV_FUNCS_H__

#include "fs_config_i.h"
#include "fs_sys_types.h"


/*************************************************************************
 * FUNCTION      efs_get_fs_data
 *
 * DESCRIPTION
 *               EFS2 has the ability to initialize from a bootstrap image,
 *               called a factory image.  This image contains a header, some
 *               map pages, and then the data clusters from the filesystem in
 *               cluster order. This function returns this factory image one
 *               page at a time. The image_info_ptr should be initialized by
 *               setting info_cluster_sent to zero. Each successive call to
 *               this function will return the next page of factory data in
 *               page_buffer. These pages should be placed in consecutive good
 *               blocks of a NAND flash, and will cause a filesystem to be
 *               built up from that image.
 *
 * INPUT         path             - file path
 *               image_info_ptr   - stores sequence number of cluster map pages
 *                                  & data pages. Specifies if more data is to
 *                                  be sent or not
 *              page_buffer       - buffer for the pages read
 *
 * DEPENDENCIES  None
 *
 * RETURN VALUE  If successful, returns 0.
 *               Otherwise, returns -1, and sets errno to indicate the error.
 *
 * SIDE EFFECTS   None
 **********************************************************************/
int efs_get_fs_data (const char * path,
                     struct fs_factimage_read_info * image_info_ptr,
                     byte * page_buffer);

/***********************************************************************
 * FUNCTION      efs_get_device_info
 *
 * DESCRIPTION   This function will get the attributes of the flash device
 *
 * INPUT         path     - file path
 *               dev_attr - pointer to device information
 *
 * DEPENDENCIES  None
 *
 * RETURN VALUE  If successful efs_get_device_info will return zero else -1.
 *
 * SIDE EFFECTS  None
 ***********************************************************************/
int efs_get_device_info (const char * path, struct fs_device_attr * dev_attr);


/***********************************************************************
 * FUNCTION      efs_wait_for_xact
 *
 * DESCRIPTION   This function waits for a transaction to finish.
 *
 ************************************************************************/
void efs_wait_for_xact (void);

/***********************************************************************
 * FUNCTION      fs_is_path_valid
 *
 * DESCRIPTION   Validates the supplied path for NULL and empty string.
 *
 * INPUT         path - path to be validated.
 *
 ************************************************************************/
int fs_is_path_valid (const char *path);

/***********************************************************************
 * FUNCTION      fs_get_shifts
 *
 * DESCRIPTION   It computes the number of shifts needed to get input.
 *               Input is assumed to be power of 2, if not it round down
 *               input to the nearest power of 2. If input is 8, the
 *               return value is 3. This function can also be interpreted
 *               as returning the floor of log_base2 of value.
 *
 * INPUT         value - Number to calcule log.
 *
 ************************************************************************/
uint32 fs_get_shifts (uint32 value);

/* This is the prototype of the callback function which wil be called
   by efs_iter() function defined belown for each entry that it finds in
   the current directory. Path is the entry we found, and stat_buf contains
   the file information for the entry, and param is any parameter that is
   passed by the function calling efs_iter which needs to be passed down to
   the callback. */

typedef int (*EFS_ITER_CALLBACK)(char *path, struct fs_stat *stat_buf,
                                 const void *param);
/***********************************************************************
 * FUNCTION      efs_iter
 *
 * DESCRIPTION   Iterates through all the entries of any given directory
 *                recursively and invokes the callback function supplied.
 *
 * INPUT         path              - The directory whose entries we have
 *                                   to iterate over recursively.
 *               want_dirs         - Pass 1, if you want the callback to
 *                                    happen for sub-directories too
 *               efs_iter_callback - The callback function. Please refer
 *                                   to the callback function prototype
 *                                   for more information. If the callback
 *                                   function's return value is not 0 then
 *                                   an error is assumed and the iteration
 *                                   proceeds with the next entry.
 *               param             - This supplied param will be passed to
 *                                   the callback function.
 * RETURN VALUE  -ve if error, with number equal to the -ve count of the
 *                callback invocation that returned error.
                  0 if we have iterated through all entries without error.
 *
 * SIDE EFFECTS  None
 *
 ************************************************************************/
int efs_iter (const char *path, int want_dirs,
              EFS_ITER_CALLBACK efs_iter_callback, const void *param);


/***********************************************************************
 * FUNCTION      efs_auto_mkdir
 *
 * DESCRIPTION   Creates intermediate missing directories in the supplied path
 *
 * INPUT         path   - Path whose intermediate missing directories need to
 *                        be created.
 *
 * RETURN VALUE  -ve if error, 0 if all missing directories are created.
 *
 ************************************************************************/
int efs_auto_mkdir (const char * path);


/**
 * struct efs_dir_iter_data_type
 *
 * Holds the information about the recursive iterators.
 *
 **********************************************************************/
struct fs_dir_iter_debug_info
{
  uint32 black_list_entries_count;
  uint32 white_list_entries_count;

  uint32 total_files_count;
  uint32 added_files_count;
  uint32 skipped_files_count;
};

struct fs_dir_iter_file_check_node
{
  const char *path;
  uint32 path_len;
  struct fs_dir_iter_file_check_node *next;
};

struct fs_dir_iter_file_check_info
{
  int enforce_list;
  const char *file_name;
  uint32 file_size;
  char *file_buf;
  struct fs_dir_iter_file_check_node *list_head;
  uint32 list_items_count;
};

struct efs_dir_iter_data_type
{
  uint8 in_use;     /* Indicates if iterator is in use. */
  uint8 first_done; /* On the first time of returning read we return the
                     * same path given to opendir_recursive. */
  int32 err_count;  /* Number of errors encountered while iterating. */
  uint32 push_count;/* Number of directories pushed into the simulated
                       directory stack */
  EFSDIR *dir_data; /* Current directory iterator from efs_opendir.  */
  char *parent_dir; /* Holds the directory name for the current EFSDIR */
  char *curr_path;  /* Holds the path to the current entry. */
  int allow_device_specific_files; /* Allow device specific files? */
  int allow_secure_files; /* Allow secure files? */

  struct fs_dir_iter_file_check_info white_list;
  struct fs_dir_iter_file_check_info black_list;

  struct fs_dir_iter_debug_info dbg_info;
};

extern struct efs_dir_iter_data_type efs_dir_iter_data[1];

/***********************************************************************
 * FUNCTION     efs_opendir_recursive
 *
 * DESCRIPTION  Opens an iterator to recursively scan all entries in a
 *              given directory.
 *
 * INPUT        dirname  - Path to the directory to scan recursively.
 *
 * INPUT        allow_device_specic_files  - Allow device specific files?
 *                          1 --> Ignore device specific files
 *                          0 --> Don't ignore device specific files
 *
 * INPUT        allow_secure_files  - Allow secure files?
 *                          1 --> Allow secure files
 *                          0 --> Don't allow secure files
 *
 * RETURN VALUE  On success a valid handle. On error returns NULL, setting
 *               efs_errno accordingly.
 *
 * SIDE EFFECTS  The path_buff lock is aquired and not released.
 *
 ************************************************************************/
struct efs_dir_iter_data_type *efs_opendir_recursive (const char *dirname,
                    int allow_device_specic_files, int allow_secure_files);

/***********************************************************************
 * FUNCTION     efs_readdir_recursive
 *
 * DESCRIPTION  Returns the full path of the next entry. The first entry
 *              returned is the path given in efs_opendir_recursive ().
 *
 * INPUT        rec_dirp  - Recursive directory pointer given by
 *                          efs_opendir_recursive ()
 *
 *
 * RETURN VALUE  On success returns a valid path. Returns NULL when all
 *               available entries were retured or on error. On error
 *               efs_errno will be set accordingly.
 *
 * SIDE EFFECTS  None.
 *
 ************************************************************************/
const char *efs_readdir_recursive (struct efs_dir_iter_data_type *rec_dirp);

/***********************************************************************
 * FUNCTION     efs_closedir_recursive
 *
 * DESCRIPTION  Close an iterator opened to recursively scan all entries in a
 *              given directory.
 *
 * INPUT        rec_dirp  - Recursive directory pointer given by
 *                          efs_opendir_recursive ()
 *
 * RETURN VALUE  Returns the number of errors encountered while iterating.
 *               On error -1 is returned and efs_errno is set accordingly.
 *
 * SIDE EFFECTS  The path_buff lock is released.
 *
 ************************************************************************/
int efs_closedir_recursive (struct efs_dir_iter_data_type *rec_dirp);


/* All supported File System image type File Formats */
enum fs_filesystem_image_type
{
  FS_FILESYSTEM_IMAGE_TYPE_TAR      = 0,  /* TAR format.         */
  FS_FILESYSTEM_IMAGE_TYPE_ZIP_TAR  = 1,  /* ZIPPED TAR format. */
};

/****************************************************************************
 * FUNCTION      efs_filesystem_image_open
 *
 * DESCRIPTION   This function prepare for the dump of the File System
 *               image in the format requested.
 *
 * INPUT         path       - Path to indicate which File System.
 *               image_type - In what format the File System image is needed?
 *
 * INPUT        allow_device_specic_files  - Allow device specific files?
 *                          1 --> Ignore device specific files
 *                          0 --> Don't ignore device specific files
 *
 * INPUT        allow_secure_files  - Allow secure files?
 *                          1 --> Allow secure files
 *                          0 --> Don't allow secure files
 *
 * RETURN VALUE  -ve if error, 0 on success.
 *
 ***************************************************************************/
int efs_filesystem_image_open (const char *path,
                               enum fs_filesystem_image_type image_type,
                               int allow_device_specific_files,
                               int allow_secure_files);

/****************************************************************************
 * FUNCTION      efs_filesystem_image_read
 *
 * DESCRIPTION   This function is used to read the next page of the file
 *               system image in the requested format.
 *
 * INPUT         image_handle - Handle obtained from above OPEN function.
 *               buff         - Buffer to store the read bytes.
 *               buff_size    - How big is the above buffer?
 *
 * RETURN VALUE  -ve if error, 0 on success.
 *
 ***************************************************************************/
int efs_filesystem_image_read (int image_handle, uint8 *buff,
                               uint32 buff_size);

/****************************************************************************
 * FUNCTION      efs_filesystem_image_close
 *
 * DESCRIPTION   This function is used to indicate the file system to end the
 *               output of the file system image reading process.
 *
 * INPUT         image_handle - Handle obtained from above OPEN function.
 *
 * RETURN VALUE  -ve if error, 0 on success.
 *
 ***************************************************************************/
int efs_filesystem_image_close (int image_handle);


/****************************************************************************
 * FUNCTION      fs_private_borrow_ram
 *
 * DESCRIPTION   This function is used to borrow ram from the lower layers.
 *               Data is clobbered.
 *
 * OUTPUT         ram_addr     - Address of the pointer to overwrite with start
 *                               address of the borrowed ram.
 *                ram_size     - The size of the borrowed ram.
 *
 * RETURN VALUE  -ve if error, 0 on success.
 *
 ***************************************************************************/
int efs_private_borrow_ram (void **ram_addr, uint32 *ram_size);


/****************************************************************************
 * FUNCTION      efs_rcinit_shutdown_callback
 *
 * DESCRIPTION   This function is registered as a callback with RCINIT. In this
 *               function we handle the cleanup activity required during a
 *               graceful shutdown or a reboot.
 *
 ***************************************************************************/
 void efs_rcinit_shutdown_callback (void);

 /****************************************************************************
 * FUNCTION      efs_rcinit_group7_shutdown_callback
 *
 * DESCRIPTION   This function is registered as a callback with RCINIT Group 7.
 *               In this function we set a flag to fail all following efs sync.
 *               Final efs_sync during shutdown would be carried out.
 ***************************************************************************/
 void efs_rcinit_group7_shutdown_callback (void);

/****************************************************************************
 * FUNCTION      efs_pure_put
 *
 * DESCRIPTION   Stores a value into an item file. The resultant file is
 *               either the old file's data or the new file's data.
 * *
 * INPUT         path   - The path of the item file.
 *               data   - The start address of the item file data.
 *               length - Length of the data buffer.
 *               oflag  - A bitwise OR of one of the following:
 *                        O_CREATE: Create the item file if it does not exist.
 *                        O_EXCL: Fail if the item already exists.
 *                        Other flags will have no effect.
 *               mode   - Permission bits of the item file being modified.
 *
 * RETURN VALUE  Returns a zero if the item written is successful, or -1 on
 *               error. The efs_errno is set accordingly.
 *
 * ERRORS      On error, the efs_errno will be set to one of the following:
 *             ENOENT - a directory component of 'path' does not exist.
 *             EEXIST - The entry already exists and O_EXCL was specified, or
 *                      The entry exists and is not already an item-file.
 *             ELOOP  - A symlink named in 'path' contains a loop.
 *             ENAMETOOLONG - The 'path' specified, or the data is too long.
 *             EINVAL - Invalid parameter.
 *
 ***************************************************************************/
int efs_pure_put (const char *path, void *data, fs_size_t length,
                  int oflag, int mode);

/****************************************************************************
 * FUNCTION      efs_pure_get
 *
 * DESCRIPTION   Retrieves the contents of an item file.
 * *
 * INPUT         path   - The path of the item file.
 *               data   - The start address of the item file data.
 *               length - Length of the data buffer.
 *
 * RETURN VALUE  Returns the number of bytes copied in the buffer, -1 for an
 *               error. The efs_errno is set accordingly.
 *
 * ERRORS      On error, the efs_errno will be set to one of the following:
 *             ENOENT - The item or a component of the path does not exist.
 *             ELOOP  - A symlink named in 'path' contains a loop.
 *             ENAMETOOLONG - The 'path' specified is too long.
 *             ENOTITM - The named file is not of type S_IFITM.
 *             FS_ERANGE - The item file is larger than the specified buffer.
 *             EINVAL - Invalid parameter.
 *
 ***************************************************************************/
int efs_pure_get (const char *path, void *data, fs_size_t length);


/****************************************************************************
 * FUNCTION      efs_utime_item
 *
 * DESCRIPTION   Special implmentation of efs_utime for item files 2.
 *               For other types of entries (regular files and directories)
 *               it will call the same implementation of efs_utime.
 *
 * INPUT         path  - The path of the entry.
 *               times - Pointer to the structure that holds the new time of
 *                       the entry. For item files only modtime is used.
 *
 * RETURN VALUE  Returns 0 on success or -1 on error.
 *
 * ERRORS   On error, the efs_errno will be set to one of the following:
 *          ENOENT - The entry or a component of 'path' does not exist.
 *          ELOOP  - A symlink named in 'path' contains a loop.
 *          ENAMETOOLONG - The 'path' specified is too long.
 *          EINVAL - Invalid parameter.
 *          EPERM  - Trying to set the time for an item file 1.
 *
 ***************************************************************************/
int efs_utime_item (const char *path, const struct fs_utimbuf *times);


/****************************************************************************
 * FUNCTION      efs_scrub
 *
 * DESCRIPTION   Scrubbing is the operation of moving all valid pages to new
 *               erased blocks and erasing written blocks. This operation is
 *               only supported on NAND Filesytem. Flash parts only support
 *               fixed number of reads from a page without being erased and
 *               re-programmed, after which the page may no longer retain
 *               its data. The normal GC operation only moves blocks that
 *               contain garbage pages. This keeps the GC operation effecient.
 *               However, this restriction means that if there are some flash
 *               blocks that contain data that never changes, but are read many
 *               times, then those blocks can lose data after a certain large
 *               number of reads. For modems that might need to last very long,
 *               a scrub operation can be done at regular intervals which will
 *               move the read-only blocks also into new erased blocks and
 *               force an erase on the current block. This API need not be used
 *               for normal cell-phone type lifetime of 3-5 years.
 *
 * INPUT         path     A valid path of a file or directory on the mounted
 *                        file system that needs to be scrubbed.
 *
 * RETURN VALUE  Returns  0 on completion, 1 on partial completion, and -1 if
 *                        scrubbing is not supported for the filesystem.
 *
 * ERRORS   On error, the efs_errno will be set to one of the following:
 *          ENOENT - The entry or a component of 'path' does not exist.
 *          EINVAL - Invalid parameter.
 *          EPERM  - Trying to scrub a filesystem that is not NAND-Flash type.
 *
 ***************************************************************************/
int efs_scrub (const char *path);

/****************************************************************************
 * FUNCTION      efs_full_scrub
 *
 * DESCRIPTION   The function efs_scrub needs to be called repeatedly to
 *               scrub the whole file system. This helper function will
 *               take care of calling efs_scrub until it returns that it is
 *               done or returns error.
 *
 * INPUT         path     A valid path of a file or directory on the mounted
 *                        file system that needs to be scrubbed.
 *
 * RETURN VALUE  Returns  0 on completion, -1 on error.
 *
 * ERRORS   On error, the efs_errno will be set to one of the following:
 *          ENOENT - The entry or a component of 'path' does not exist.
 *          EINVAL - Invalid parameter.
 *          EPERM  - Trying to scrub a filesystem that is not NAND-Flash type.
 *
 ***************************************************************************/
int efs_full_scrub (const char *path);

/****************************************************************************
 * FUNCTION      efs_read_full_file
 *
 * DESCRIPTION   The function allows reading the entire file in one shot like
 *               efs_get without needing a file descriptor if the the length
 *               of the file is smaller than supplied buffer length and if the
 *               max transaction size is bigger the file size.
 *
 * INPUT         path     A valid path of a file that needs to be read.
 *
 * RETURN VALUE  Returns  Bytes read on success and -1 on error
 *
 * ERRORS   On error, the efs_errno will be set to one of the following:
 *          ENOENT - The entry or a component of 'path' does not exist.
 *          EINVAL - Invalid parameter.
 *          FS_ERANGE - Max transaction size smaller than file size or buffer
 *                      smaller than file size.
 *
 ***************************************************************************/
int efs_read_full_file (const char *path, void *data, fs_size_t length);

/* All supported EFS golden copy types */
enum fs_golden_copy_type
{
  FS_GOLDEN_COPY_TYPE_FULL_SNAPSHOT       = 1,
  FS_GOLDEN_COPY_TYPE_FILES_LIST          = 2,
  FS_GOLDEN_COPY_TYPE_FILES_LIST_OEM_1    = 3,
  FS_GOLDEN_COPY_TYPE_FILES_LIST_OEM_2    = 4,
  FS_GOLDEN_COPY_TYPE_ZIP_TAR             = 5,
  FS_GOLDEN_COPY_TYPE_MAX                 = 6
};

/* All EFS golden copy partitions on flash */
enum fs_golden_copy_partition_type
{
  FS_GOLDEN_COPY_PARTITION_TYPE_DEFAULT = 0,
  FS_GOLDEN_COPY_PARTITION_TYPE_OEM_1   = 1,
  FS_GOLDEN_COPY_PARTITION_TYPE_OEM_2   = 2,
  FS_GOLDEN_COPY_PARTITION_TYPE_MAX     = 3,
};

/* EFS golden-copy files-list */
enum fs_golden_copy_files_list_type
{
  FS_GOLDEN_COPY_FILES_LIST_DEFAULT = 0,
  FS_GOLDEN_COPY_FILES_LIST_OEM_1   = 1,
  FS_GOLDEN_COPY_FILES_LIST_OEM_2   = 2,
  FS_GOLDEN_COPY_FILES_LIST_MAX     = 3,
};

/***********************************************************************
 * FUNCTION      efs_golden_copy_set_oem_files_list
 *
 * DESCRIPTION   Sets the files-list to be used while making the
 *               EFS-golden-copy based on files list. This API is not
 *               supported on all Page-Managers and currently is supported
 *               only for Remote-Storage Page-Manager.
 *
 * INPUT         fs_golden_copy_files_list_type - Which files-list to set?
 *               FS_GOLDEN_COPY_FILES_LIST_DEFAULT - Creates EFS-golden-copy
 *               comprising only selected files in EFS supplied in the
 *               efs_golden_copy_set_files_list API
 *
 *               FS_GOLDEN_COPY_FILES_LIST_OEM_1 -
 *               FS_GOLDEN_COPY_FILES_LIST_OEM_2 - Creates EFS-golden-copy
 *               comprising only selected files in EFS present in the OEM-list
 ************************************************************************/
int efs_golden_copy_set_oem_files_list (
    enum fs_golden_copy_files_list_type type, uint32 files_count,
    const char **files_list);


/***********************************************************************
 * FUNCTION      efs_make_golden_copy
 *
 * DESCRIPTION   Takes the most recent copy of EFS and back it up into
 *               permanent storage. It can be used for recovery.
 *
 * INPUT         path - path to mountpoint to backup.
 *
 ************************************************************************/
int efs_make_golden_copy (const char *path,
        enum fs_golden_copy_type golden_copy_type);

/***********************************************************************
 * FUNCTION      efs_make_golden_copy_no_wait
 *
 * DESCRIPTION   Sends signal to FS-task to create EFS-golden copy and
 *               will return immediately.
 *
 * INPUT         path - path to mountpoint to backup.
 *
 * INPUT         golden_copy_type - Type of golden-copy to create.
 *               FS_GOLDEN_COPY_TYPE_FULL_SNAPSHOT - Takes the most recent
 *               copy of EFS and back it up into permanent storage.
 *               It can be used for recovery.
 *
 *               FS_GOLDEN_COPY_TYPE_FILES_LIST - Creates EFS-golden-copy
 *               comprising only selected files in EFS supplied in the
 *               efs_golden_copy_set_files_list API
 ************************************************************************/
int efs_make_golden_copy_no_wait (const char *path,
        enum fs_golden_copy_type golden_copy_type);

/* EFS Golden copy creation status */
enum fs_golden_copy_status
{
  FS_GOLDEN_COPY_STATUS_IN_PROGRESS       = 1,
  FS_GOLDEN_COPY_STATUS_COMPLETE          = 2,
};

/***********************************************************************
 * FUNCTION      efs_golden_copy_no_wait_get_status
 *
 * DESCRIPTION   Gets the status of the EFS-golden-copy creation.
 *
 * INPUT         path - path to mountpoint to backup.
 *
 * INPUT         status - status of the efs-golden-copy creation will be
 *               returned in this variable.
 ************************************************************************/
int efs_golden_copy_no_wait_get_status (const char *path,
                      enum fs_golden_copy_status *status);

/***********************************************************************
 * FUNCTION      efs_golden_copy_signal_received
 *
 * DESCRIPTION   Called by FS-task when it receives signal to create
 *               EFS-golden-copy.
 ************************************************************************/
int efs_golden_copy_signal_received (void);

/***********************************************************************
 * FUNCTION      efs_golden_copy_restore_signal_received
 *
 * DESCRIPTION   Called by FS-task when it receives signal to create
 *               EFS-golden-copy.
 ************************************************************************/
int efs_golden_copy_restore_signal_received (void);

/***********************************************************************
 * FUNCTION      efs_rcinit_group_3_func
 *
 * DESCRIPTION   Called by rcinit.
 ************************************************************************/
void efs_rcinit_group_3_func (void);

/***********************************************************************
 * FUNCTION     efs_make_golden_copy_zip_tar
 *
 * DESCRIPTION  Takes a backup of the EFS-file as zipped-tar image.
 *
 * INPUT         path - path to mountpoint to backup.
 *
 * RETURN VALUE  Returns 0 on success and negative number on error.
 ************************************************************************/
int efs_make_golden_copy_zip_tar (const char *path);

/***********************************************************************
 * FUNCTION      efs_is_dir_exists
 *
 * DESCRIPTION   Checks if the given directory exists or not.
 *
 * INPUT         path - directory path to check for existence
 *
 * RETURN VALUE  Returns 1 if the directory exists 0 otherwise
 ************************************************************************/
int efs_is_dir_exists (const char *path);

/**
 * @par Name:
 *  efs_shred - Erases the (old) obsolete data residing
 *  in the device.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_shred ();
 *  @endcode
 *
 * @par Description:
 *  When a file is erased or truncated some of its data content may still
 *  reside in the physical device. The function 'efs_shred' provides a way to
 *  erase obsolete data residing in the device. This function returns
 *  1 if the shredding job is not complete. It is designed to do partial
 *  shred on each call so that the task doesn't consume huge amount of
 *  CPU time to complete shredding the whole device, thus causing watchdog
 *  error if the caller task doesn't get chance to report to dog within its
 *  configured time. After shredding of all the obsolete data as of when
 *  efs_shred called first, it would return 0. If a file modification call is
 *  issued after the first efs_shred call, data becoming obsolete by that
 *  call may reside in the device. Shred is only supported for the NAND
 *  devices only. Shred is an expensive operation and it is intended to be
 *  used when refurbishing the device in the service center or factory.
 *  Shred does not erase obsolete file or directory names.
 *
 * @param path
 *  The path is a valid path of a file or directory on the mounted file system
 *  that needs to be shredded.
 *
 * @b Example
 *
 * @code
 *    int result;
 *    while (1)
 *    {
 *      result = efs_shred ("/");
 *      if (result == 0)
 *        break; // Shred is complete.
 *      else if (result == -1)
 *        // Shred is not a valid command.
 *        break;
 *      // When shred returns a positive number, partial shred is done.
 *      // We should sleep to prevent watchdog expiration and issue another
 *      // shred call until efs_shred returns 0 or shred is complete.
 *      sleep ();
 *    }
 * @endcode
 *
 * @return
 *  Returns 0 on completion and a positive number on partial
 *  completion. It returns -1 if shred cannot be done.
 *
 **********************************************************************/
int
efs_shred (const char *path);

/**
 * @par Name:
 *   efs_mount - Mount a file system at a given directory.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    int  efs_mount (const char *special, const char *dir,
 *                    const char *fstype, int flags, const void *data);
 *  @endcode
 *
 * @par Description:
 *  Mount a file system at a given directory.  Unlike Posix, the name (fstype)
 *  given is a parsed string describing the source of the file system.
 *
 * @param special
 *  Currently not used.
 *
 * @param dir
 *  Directory where to mount the file system.
 *
 * @param fstype
 *  File system type, e.g. EFS2, nodev, extfs (for FAT).
 *
 * @param flags
 *  Currently not used, should be zero. Future purpose flags.
 *
 * @param data
 *  Pointer to where the file system data resides.
 *
 * @return
 *  Returns 0 for success, -1 on error.
 *
 * @par Errors:
 *   Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EMFILE - Mount Table Full.\n
 *    ::ENODEV - Invalid file system type.\n
 *    ::EINVAL - Invalid parameter.\n
 *
 * @sa
 *  efs_mount, efs_remount
 *
 */
int efs_mount (const char *special, const char *dir,
    const char *fstype, int flags, const void *data);

/**
 * @par Name:
 *   efs_umount - Remove a mount file system at a given directory.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    int efs_umount (const char *target);
 *  @endcode
 *
 * @par Description:
 *  Unmounts a previously mounted file system. Unlike Posix, the umount is
 *  performed, even if the file system is busy. All open handles will result
 *  in ENODEV being returned for any access attempt.
 *
 * @param target
 *  Path where the file system is mounted.
 *
 * @return
 *  Returns 0 for success, -1 on error.
 *
 * @par Errors:
 *   Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EINVAL - Not a valid mount point.\n
 *
 * @sa
 *  efs_umount, efs_remount
 *
 */

int efs_umount (const char *target);

/**
 * @par Name:
 *   efs_remount - Relocates where an existing mount point is in the file
 *                 system.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    int efs_remount (const char *oldtarget, const char *newtarget);
 *  @endcode
 *
 * @par Description:
 *  Cannot be used (yet) to remount root, but allows existing mount points
 *  to be moved. Opened handles will continue to work, although pathnames
 *  used to access them will change. This is similar behavior to what would
 *  be seen with directory renaming. Returns EINVAL if oldtarget isn't an
 *  existing active mount. The newtarget directory must exist.
 *
 *  The results are undefined if newtarget is a subdirectory of oldtarget.
 *
 * @param oldtarget
 *  Old path of the file system.
 *
 * @param newtarget
 *  New path for the file system.
 *
 * @return
 *  Returns 0 for success, -1 on error.
 *
 * @par Errors:
 *   Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EINVAL - Not a valid mount point.\n
 *
 * @sa
 *  efs_umount, efs_mount
 *
 */
int efs_remount (const char *oldtarget, const char *newtarget);

/**
 * @par Name:
 *  efs_get_volume_name - Retrieves the volume name for a filesystem.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    int efs_get_volume_name (const char *path, char *label, int len);
 *  @endcode
 *
 * @par Description:
 *  Each filesystem that supports volume names can have a label associated
 *  with it. This API can be used to retrieve this label.
 *
 * @param path
 *  The path where the filesystem is mounted.
 *
 * @param label
 *  This is the buffer where the volume name is returned to the caller. The
 *  caller is required to allocate this buffer.
 *
 * @param len
 *  This is the length parameter for the label buffer that was allocated.
 *  The maximum length is 10. The buffer allocated is not used beyond
 *  this limit.
 *
 * @return
 *  Returns 0 on success or -1 on error. Upon error refer efs_errno.
 *
 * @par Errors:
 *  Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified is too long.\n
 *    ::EINVAL - Volume name not supported. Invalid operation.\n
 *    ::ENODEV - The device does not exist.\n
 *
 * @sa
 *  efs_set_volume_name
 *
 **********************************************************************/
int efs_get_volume_name (const char *path, char *label, int len);

/**
 * @par Name:
 *  efs_set_volume_name - Sets the volume name for a filesystem.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    int efs_set_volume_name (const char *path, char *label);
 *  @endcode
 *
 * @par Description:
 *  Each filesystem that supports volume names can have a label associated
 *  with it. This API can be used to set this label.
 *
 * @param path
 *  The path where the filesystem is mounted.
 *
 * @param label
 *  This is the buffer where the volume name is passed by the caller. The
 *  caller allocates and sets the desired label. Maximium limit of
 *  a permitted label is 10. Any data beyond this limit is automatically
 *  truncated. Only upper-case characters are permitted. Lower case characters
 *  are automatically converted to uppercase.
 *
 * @return
 *  Returns 0 on success or -1 on error. Upon error refer efs_errno.
 *
 * @par Errors:
 *  Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EINVAL - Volume name not supported. Invalid operation.\n
 *    ::ENODEV - The device does not exist.\n
 *
 * @sa
 *  efs_get_volume_name
 *
 **********************************************************************/
int efs_set_volume_name (const char *path, char *label);


/**
 *  md5sum packet structure. It is used to return the hash calculated
 *  in the library function efs_md5sum.
 */
#define MD5_RESULT_SIZE_IN_BYTES 16
struct fs_md5sum {
  unsigned char hash[MD5_RESULT_SIZE_IN_BYTES];
};
typedef struct fs_md5sum *fs_md5sum_t;  /* Not used by EFS */

/**
 * @par Name:
 *   efs_md5sum - Calculate md5 hash for the file-data.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_lib.h"
 *    int efs_md5sum (const char *path, struct fs_md5sum *hash_out_data);
 *  @endcode
 *
 * @par Description:
 *   A hash function maps the input to a output which is usually a
 *   number. hash_output_data is a 128 bit digital fingerprint of
 *   the data inside the file named path. It is very unlikely (but
 *   possible that the md5sum values for two different files to be
 *   the same. RFC 1321 describes the details of the algorithm. The
 *   algorithm starts with 4 32-bit constant values as the hash
 *   result. Each-time 16 bytes of data is chosen and some bitwise
 *   operation between the hash result and 16 byte data is done until
 *   all the data exhaust. Even change of bit in any input byte would
 *   generate a different result.
 *
 * @param path
 *   pathname for the file on whose content the hash function
 *   need to be calculated.
 *
 * @param hash_result
 *   The result for the hash function is computed and put here.
 *
 * @return
 *   Returns 0 on success and -1 on failure.
 *
 */
int
efs_md5sum (const char *path, struct fs_md5sum *hash_result);



/**
 * @par Name:
 *  efs_raw_put - Store a value in an EFS2 'special item' file.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_raw_put (const char *path, void *data, fs_size_t length,
 *                     int oflag, int mode);
 *  @endcode
 *
 * @deprecated
 *  This function will call efs_put() directly, for full description see
 *  efs_put(). This API is deprecated and provided for legacy reasons, new
 *  applications should use efs_put() instead.
 *
 * @param path
 *  The path of the item-file.
 *
 * @param data
 *  The start-address of the data.
 *
 * @param length
 *  The length indicates the size of the item-file and it should not exceed
 *  FS_ITEM_MAX bytes.
 *
 * @param oflag
 *  See efs_put() for complete list of 'oflag' values.
 *
 * @param mode
 *  'mode' will set the initial permission bits of the item file.
 *
 * @return
 *  See efs_put().
 *
 * @par Errors:
 *  See efs_put().
 *
 * @sa
 *  efs_raw_get
 *
 **********************************************************************/
int efs_raw_put (const char *path, void *data, fs_size_t length,
    int oflag, int mode);

/**
 * @par Name:
 *  efs_raw_get - Retrieve an EFS2 'special item' file.  See 'efs_put' for a
 *  description of these files.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_raw_get (const char *path, void *data, fs_size_t length);
 *  @endcode
 *
 * @deprecated
 *  This function will call efs_get() directly, for full description see
 *  efs_get(). This API is deprecated and provided for legacy reasons, new
 *  applications should use efs_get() instead.
 *
 * @param path
 *  The path of the item-file.
 *
 * @param data
 *  The start-address of the data.
 *
 * @param length
 *  The length indicates the size of the buffer 'data' in bytes.
 *
 * @return
 *  See efs_get().
 *
 * @par Errors:
 *  See efs_get().
 *
 * @sa
 *  efs_raw_put
 *
 **********************************************************************/
int efs_raw_get (const char *path, void *data, fs_size_t length);

/**
 * Checks if the file can possibly to be accessed by the user
 * with the given mode.  The mode is a mask of the values R_OK,
 * W_OK, X_OK, and F_OK.  The first three check the appropiate
 * bits of the file.  F_OK can be used to check for the existence
 * of the file.
 * &Deprecate
 *
 * @note
 *    efs_access is not currently implemented.
 *
 * @par Errors:
 *   EINVAL : errno is always set to EINVAL to note that efs_access is not
 *            implemented.\n
 **********************************************************************/
int efs_access (const char *path, int amode);

/**
 * efs_boot_int_romfs
 *
 * Initialize the filesystem and mount romfs as the root filesystem.
 * This should only be called from boot code.
 *
 * Unlike efs_boot_initialize() API, this API does not mount any
 * external FAT partition as "/mmc1".
 *
 * It is okay to call this API multiple times and calling again will
 * immediately return success.
 *
 * Returns 0 on success, otherwise returns an error code (does not set
 * errno).
 **********************************************************************/
int efs_boot_init_romfs (void);

/**
 * efs_initialize_locks
 *
 * Initialize the filesystem lock and grab it to lock the filesystem.
 *
 **********************************************************************/
void efs_initialize_locks (void);

/**
 * efs_initialize_release_locks
 *
 * Unlock the filesystem that was locked during efs_initialize
 *
 **********************************************************************/
void efs_initialize_release_locks (void);


/***********************************************************************
 * FUNCTION      efs_write_file_helper
 *
 * DESCRIPTION   Write the supplied buffer onto the given file.
 *
 * INPUT         path - The path of the file.
 *
 * INPUT         buf - The buffer to write.
 *
 * INPUT         buf_size - Number of bytes to write from buf.
 *
 * RETURN VALUE  Returns 0 if the whole write was done or -ve on error.
 ************************************************************************/
int efs_write_file_helper (const char *path, void *buf, uint32 buf_size);

/***********************************************************************
 * FUNCTION      efs_read_file_helper
 *
 * DESCRIPTION   Read the supplied file onto the given buffer.
 *
 * INPUT         path - The path of the file.
 *
 * INPUT         buf - The buffer to read onto.
 *
 * INPUT         buf_size - Number of bytes to read onto the buf.
 *
 * RETURN VALUE  Returns 0 if the whole read was done or -ve on error.
 ************************************************************************/
int efs_read_file_helper (const char *path, void *buf, uint32 buf_size);

/**
 * efs_initialize
 *
 * Initialize the filesystem and mount the root filesystem.
 *
 * Returns 0 on success, otherwise returns an error code (does not set
 * errno).
 **********************************************************************/
int efs_initialize (void);

/**
 * efs_power_down
 *
 * Place the filesystem in a state where it is safe to remove power.
 **********************************************************************/
int efs_power_down (void);

/**
 * Reset the filesystem and stop the system. The mobile must be
 * rebooted before it can be used again.
 *
 * This function never returns.
 */
void efs_reset (void);

/**
 * Same as efs_reset, except doesn't call FS_ERR_FATAL.  Should only be
 * used from within the FS_ERR_FATAL handler.
 */
void efs_reset_nostop (void);

int efs_image_prepare (const char *path);


#endif /* not __FS_PRIV_FUNCS_H__ */
