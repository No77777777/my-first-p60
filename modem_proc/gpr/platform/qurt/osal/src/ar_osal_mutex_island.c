/*========================================================================
@file ar_osal_mutex_island.cpp
This file contains utilities for using mutex functionalities.

Copyright (c) 2021 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */

/*========================================================================
Edit History

$Header: //components/dev/gpr.common/1.0/sranumul.gpr.common.1.0.june_29_build/platform/qurt/osal/src/ar_osal_mutex.c#1
$

when       who     what, where, why
--------   ---     -------------------------------------------------------
02/04/10   mwc      Created file.
========================================================================== */
#include "ar_osal_mutex.h"
#include "qurt.h"
#include "qurt_pimutex.h"
#include "ar_osal_types.h"
#include "ar_osal_error.h"
#include <stdlib.h>

int32_t ar_osal_mutex_lock(ar_osal_mutex_t mutex)
{
   qurt_pimutex_lock((qurt_mutex_t *)mutex);
   return AR_EOK;
}

int32_t ar_osal_mutex_unlock(ar_osal_mutex_t mutex)
{
   qurt_pimutex_unlock((qurt_mutex_t *)mutex);
   return AR_EOK;
}

/** @} */ /* end_addtogroup osal_mutex */
