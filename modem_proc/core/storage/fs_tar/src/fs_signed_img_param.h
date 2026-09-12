/***********************************************************************
 * fs_signed_img_param.h
 *
 * Parameters needed to verify signed image.
 * Copyright (C) 2011,2013,2018 QUALCOMM Technologies, Inc.
 *
 * When the file system image is signed, there are some parameters
 * needed to verify the authenticity of the image. This file implements
 * the methods for retrieving these parameters.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/fs_tar/src/fs_signed_img_param.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-03-05   rp    Move SW_KEY hash from EFS-module to security-module.
2013-01-29   nr    Migrate to new signed image authentication apis.
2011-06-18   wek   Create. Add secure TAR signing/verification.

===========================================================================*/

#ifndef __FS_SIGNED_IMG_PARAM_H__
#define __FS_SIGNED_IMG_PARAM_H__

#include "fs_config.h"
#include "comdef.h"
#include "secboot_util.h"

/* Gets the Software ID used to sign the file system image.
 *
 * The software ID is defined by the macro FS_SIGNED_IMG_SOFTWARE_ID, it
 * is 8 bytes long. Similarly to the root certificate hash there are three
 * ways to change this value:
 *   - Define a new value for the macro FS_SIGNED_IMG_SOFTWARE_ID.
 *   - Modify the value of the macro in fs_signed_img_param.c
 *   - Modify the implementation of the function
 *     fs_sign_get_software_version_id ().
 *
 * @param sw_version[out]   Software version ID configured to authenticate
 *                          the file system image.
 */
void fs_sign_get_software_version_id (uint64 *sw_version);

#endif /* not __FS_SIGNED_IMG_PARAM_H__ */
