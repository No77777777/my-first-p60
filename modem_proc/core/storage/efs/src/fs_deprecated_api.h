/***********************************************************************
 * fs_deprecated_api.h
 *
 * Deprecated APIs Header for EFS2.
 *
 * This file documents the deprecated API calls out of EFS.
 *
 * The comment text found in this file is intended to be a thorough
 * and complete reference, so please report any inaccuracies or
 * confusing text as bugs.
 *
 * Copyright (C) 2018, QUALCOMM Technologies, Inc.
 *
 ***********************************************************************/

 /*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs/src/fs_deprecated_api.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when          who     what, where, why
--------      ---     ------------------------------------------------------
2018-02-05    eo     Created.

===========================================================================*/


#ifndef __FS_DEPRECATED_API_H__
#define __FS_DEPRECATED_API_H__

#include "fs_sys_types.h"
#include "fs_fcntl.h"
#include "fs_lib.h"

/** Identifies the major version number of this API file. */
#define VER_FS_DEPRECATED_API_H_MAJOR     1
/** Identifies the minor version number of this API file. */
#define VER_FS_DEPRECATED_API_H_MINOR     1

/**
 * Create a new "special" file.
 *
 * Note that EFS assigns NO SIGNIFICANCE to these files.  It simply
 * records their existence and makes them visible during directory
 * enumeration, with the special mode bits intact.  The actual device
 * driver handling must be performed by code outside the filesystem.
 * For example, special files can not be used for read/write
 * filesystem operations.  Also, EFS does not perform the FIFO service
 * traditionally associated with \c S_IFIFO filenames.
 *
 * @param path
 *    The file path to be created.
 *
 * @param mode
 *    'mode' specifies the usual read/write/execute permission bits of the new
 *    file, as well as the type of node to be created.
 *    Please see the definitions of "Posix permission bits" in fs_sys_types.h.
 *    One of either \c S_IFBLK or \c S_IFCHR is required to be set in the
 *    mode for the creation of Block or Character device files,
 *    respectively. These device files require a Device Number, which
 *    will be stored with the file entry for use by the operating
 *    system.
 *    Special files of type FIFO and Socket are also possible by using
 *    the \c S_IFIFO or \c S_IFSOCK mode.  In these cases, the \c dev parameter
 *    is ignored, as Socket and FIFO files have no associated device
 *    number.
 *
 * @param dev
 *    This parameter specifies the 16-bit Device number to be stored
 *    with a new device node.  Traditionally, each of the Major and
 *    Minor numbers are 8-bits, with the Major number in the MSB.
 *    Since EFS assigns no meaning to this value, the actual
 *    interpretation depends on the operating system's interpretation
 *    of device nodes.  This value should be zero for files other than
 *    \c S_IFBLK or \c S_IFCHR.
 *
 * @return 0 for success, or -1 for an error.
 **********************************************************************/
int efs_mknod (const char *path, int mode, fs_devspecial_t dev);

/**
 * Create a new "FIFO" special file (sometimes called a named pipe).
 *
 * This is a simplified case of efs_mknod() that is limited to
 * creating files of type \c S_IFIFO.
 *
 * Note that EFS assigns NO SIGNIFICANCE to these files; it simply
 * records their existence and makes them visible during directory
 * enumeration.  The actual FIFO handling must be performed by handler
 * code above the filesystem.  EFS does not internally perform the
 * FIFO service traditionally associated with these files.
 *
 * @param path
 *    The file path to be created as a FIFO.
 *
 * @param mode
 *    'mode' specifies the permission bits of the new file.\n
 *    \c S_IFIFO will be assumed, and is optional in the mode argument.
 *    Please see the definitions of "Posix permission bits" in
 *    \link fs_sys_types.h \endlink
 *
 * @return 0 for success, or -1 for an error.
 **********************************************************************/
int efs_mkfifo (const char *path, int mode);

/**
 * @par Name:
 *  efs_set_reservation - Set the reservation value for a given group.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_set_reservation (const char *path, uint32 groupid,
                               fs_size_t size);
 *  @endcode
 *
 * @par Description:
 *  A Reservation is a space reserved prior to use in order to guarantee
 *  availability for use later. This API can be used to reserve such space.
 *  As files are written to the file system and to the group which reserved
 *  the space, blocks will be reduced from the reservation available value.
 *  If the reservation available value is reduced to 0, EFS will then
 *  attempt to continue the write using available space from the free pool.
 *  When a file is deleted from the directory, the size of that file will be
 *  returned to the reservation available as long as the reservation is not
 *  currently exceeded.
 *  Any meta-data for inodes or database leaf nodes created under the
 *  directory will be charged to the general pool first; if no space is
 *  available, then to the reservation, therefore any reservation size will
 *  be an approximation of the space reserved. So it is a wise idea to
 *  reserve a few more blocks than necessary to ensure the proper
 *  reservation of space.
 *  A sample usage scenario of Reservation would be that an app which wants
 *  to download an  image wants to ensure space for the image before it
 *  starts the download.
 *
 * @param path
 *  The path where to set aside the reservation space.
 *
 * @param groupid
 *  This is the ID value for groups of files. A group ID allows files and
 *  directories to be grouped together for purpose of the reservation and
 *  quota system. The concept of groups is to create a ruleset to be applied
 *  to various number of files and/or directories. Every directory and
 *  regular file inherits the GID of its parent.
 *
 * @param size
 *  The size parameter should be passed in as the number of bytes. Be aware
 *  however that this function will convert the size to the number of pages as
 *  that is the smallest unit of storage in EFS. Therefore if the byte size
 *  is not divisible by the page size then an additional page will be set
 *  aside for the reservation.
 *
 * @return
 *  Returns 0 on success or -1 on error. Upon error refer efs_errno.
 *
 * @par Errors:
 *  Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EPERM - The group id parameter value is out of the the allowed
 *              number of groups\n
 *    ::ENOSPC - Unable to reserve the space desired\n
 *    ::EINVAL - Invalid parameter.\n
 * @sa
 *   efs_set_quota, efs_chown
 *
 **********************************************************************/
int efs_set_reservation (const char *path, uint32 groupid, fs_size_t size);

/**
 * @par Name:
 *  efs_set_quota - Set the quota value for a given group.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_set_quota (const char *path, uint32 groupid, fs_size_t size);
 *  @endcode
 *
 * @par Description:
 *  A Quota is a limit set to prevent overuse of the file system by an
 *  assigned group. Quotas are a self policing mechanism, used to ensure
 *  that a group or application doesn't inadvertently hog the file system or
 *  wants to limit itself based on the available space for performance
 *  issues. This API can be used to set such a quota restriction for a group
 *
 * @param path
 *  The path on which to set the quota restriction.
 *
 * @param groupid
 *  This is the ID value for groups of files. A group ID allows files and
 *  directories to be clumped together for purpose of the reservation and
 *  quota system. The concept of groups is to create a ruleset to be applied
 *  to a various number of files and/or directories. Every directory and
 *  regular file inherits the GID of its parent. groupid 0 is reserved and
 *  cannot be used, EFS will return a permission denied error.
 *
 * @param size
 *  The size parameter should be passed in as the number of bytes. Be aware
 *  however that this function will convert the size to the number of pages as
 *  that is the smallest unit to be stored in EFS. Therefore if the byte size
 *  is not divisible by the EFS Page size, then an additional page will be set
 *  aside for the quota.
 *
 * @return
 *  Returns 0 on success or -1 on error. Upon error refer efs_errno.
 *
 * @par Errors:
 *  Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EPERM - The group id parameter value is out of the the allowed
 *              number of groups\n
 *    ::EINVAL - Invalid parameter.\n
 * @sa
 *  efs_set_reservation, efs_chown
 *
 **********************************************************************/
int efs_set_quota (const char *path, uint32 groupid, fs_size_t size);

/**
 * @par Name:
 *  efs_get_group_info - Get the quota and reservation information for the
 *  given group id.
 *
 * @par Synopsis:
 *  @code
 *    #include "fs_public.h"
 *    #include "fs_sys_types.h"
 *    int efs_get_group_info (const char *path, uint32 groupid,
 *                            struct fs_group_info *ginfo);
 *  @endcode
 *
 * @par Description:
 *  This function will return the group information of a given group id. The
 *  information will be stored in a group_info structure. Please refer to
 *  the fs_group_info structure for details on the meanings of the fields.
 *
 * @param path
 *  The path whose group info to retrieve.
 *
 * @param groupid
 *  This is the ID value for groups of files. A group ID allows files and
 *  directories to be clumped together for purpose of the reservation and
 *  quota system. The concept of groups is to create a ruleset to be applied
 *  to a various number of files and/or directories. Every directory and
 *  regular file inherits the GID of its parent. groupid 0 is reserved and
 *  cannot be used, EFS will return a permission denied error.
 *
 * @param ginfo
 *  The return value, on success this struct will be populated with how much
 *  was the Quota and Reservation values set aside for this groupid and of
 *  which how much has been consumed till now.
 *
 * @return
 *  Returns 0 on success or -1 on error. Upon error refer efs_errno.
 *
 * @par Errors:
 *   Upon error, the efs_errno will be set to one of the following:\n
 *    ::ENOENT - a directory component of 'path' does not exist.\n
 *    ::ELOOP  - A symlink named in 'path' contains a loop.\n
 *    ::ENAMETOOLONG - The 'path' specified, or the data is too long.\n
 *    ::EPERM  - The group id parameter value is out of the the allowed
 *                number of groups\n
 *    ::EINVAL - Invalid parameter.\n
 * @sa
 *  efs_set_reservation, efs_set_quota, efs_chown
 *
 **********************************************************************/
int efs_get_group_info (const char *path, uint32 groupid,
    struct fs_group_info *ginfo);

/**
 * efs_read_only_initialize
 *
 * Initialize EFS on NAND filesystem on read only mode. This is expected to
 * be called only in boot to reduce the code space associated with writes.
 *
 * This API expect that is is called only once during the init process.
 *
 * Returns 0 on success, otherwise returns an error code (does not set
 * errno).
 **********************************************************************/
int efs_read_only_initialize (void);

#endif /* End of __FS_DEPRECATED_API_H__ */
