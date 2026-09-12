/**
 * @file:  cpr_image_defs.h
 * @brief:
 *
 * Copyright (c) 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
 *
 * $DateTime: 2019/04/24 00:03:26 $
 * $Header: //components/rel/core.mpss/10.0/power/cpr/image/mpss/inc/cpr_image_defs.h#1 $
 * $Change: 18983508 $
 */
#ifndef CPR_IMAGE_DEFS_H
#define	CPR_IMAGE_DEFS_H

#include "comdef.h"
#include "HALhwio.h"
#include "CoreVerify.h"
#include "msmhwiobase.h"
#include "DDIChipInfo.h"

#define CPR_CHIPINFO_VERSION(major, minor) DALCHIPINFO_VERSION(major, minor)
#define CPR_ASSERT(x) CORE_VERIFY(x)

#define CPR_IMAGE_LOCK_CORE()   cpr_image_mutex_lock()
#define CPR_IMAGE_UNLOCK_CORE() cpr_image_mutex_unlock()

void cpr_image_mutex_lock(void);
void cpr_image_mutex_unlock(void);
#endif

