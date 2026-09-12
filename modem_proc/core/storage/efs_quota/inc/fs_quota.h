/***********************************************************************
 * fs_quota.h
 *
 * EFS Quota limit setup module.
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 * This module sets up quota limit in EFS for the configured groups.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs_quota/inc/fs_quota.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2016-04-25   rp    Incorporate code review comments for EFS quota change.
2016-04-25   rp    Create

===========================================================================*/

#ifndef __EFS_QUOTA_H__
#define __EFS_QUOTA_H__

#include "customer.h"
#include "comdef.h"
#include "fs_sys_types.h"

#ifndef EFS_QUOTA_SIZE_FOR_GPS_IN_KB
  #define EFS_QUOTA_SIZE_FOR_GPS_IN_KB 0
#endif

#ifndef EFS_QUOTA_SIZE_FOR_NV_IN_KB
  #define EFS_QUOTA_SIZE_FOR_NV_IN_KB 0
#endif

#ifndef EFS_QUOTA_SIZE_FOR_RF_IN_KB
  #define EFS_QUOTA_SIZE_FOR_RF_IN_KB 0
#endif

#ifndef EFS_QUOTA_SIZE_FOR_MCFG_IN_KB
  #define EFS_QUOTA_SIZE_FOR_MCFG_IN_KB 0
#endif

#ifndef EFS_QUOTA_SIZE_FOR_SFS_IN_KB
  #define EFS_QUOTA_SIZE_FOR_SFS_IN_KB 0
#endif

#ifdef FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS

void efs_quota_init_for_clients (void);
int efs_quota_is_enabled_for_clients (void);

#ifdef FS_UNIT_TEST
void sim_efs_quota_set_quota_size (uint32 gid, uint32 quota_size_in_kb);
#endif

#else /* FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS */

static inline void efs_quota_init_for_clients (void)
{
}
static inline int efs_quota_is_enabled_for_clients (void)
{
	return 0;
}

#ifdef FS_UNIT_TEST
static inline void
sim_efs_quota_set_quota_size (uint32 gid, uint32 quota_size_in_kb)
{
  (void) gid; (void) quota_size_in_kb;
}
#endif

#endif /* FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS */

#endif /*  __EFS_QUOTA_H__ */
