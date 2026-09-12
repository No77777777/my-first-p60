/***********************************************************************
 * fs_signed_img_param.c
 *
 * Parameters needed to verify signed image.
 * Copyright (C) 2011,2013,2015-2018 QUALCOMM Technologies, Inc.
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

  $Header: //components/rel/core.mpss/10.0/storage/fs_tar/src/fs_signed_img_param.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-03-05   rp    Move SW_KEY hash from EFS-module to security-module.
2017-12-04   as    Change the SW KEY hash again.
2017-04-10   rp    Revert SW KEY hash from sha384 to sha256
2017-01-11   rp    Migrate SW KEY hash from sha256 to sha384
2016-06-02   rp    Migrate SW KEY to new SecTool and allow dummy key.
2015-07-07   mj    Change default SW_ID for EFS TAR signature verification.
2013-01-29   nr    Migrate to new signed image authentication apis.
2011-06-18   wek   Create. Add secure TAR signing/verification.

===========================================================================*/

#include "fs_signed_img_param.h"

#ifndef FS_SIGNED_IMG_SOFTWARE_ID
  #define FS_SIGNED_IMG_SOFTWARE_ID  0x000000000000001B
#endif

void fs_sign_get_software_version_id (uint64 *sw_version)
{
  *sw_version = FS_SIGNED_IMG_SOFTWARE_ID;
}
