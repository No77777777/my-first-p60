/***********************************************************************
 * fs_quota.c
 *
 * EFS Quota limit setup module.
 * Copyright (C) 2016 QUALCOMM Technologies, Inc.
 *
 * This module sets up the quota limit in EFS for the configured groups.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs_quota/src/fs_quota.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2018-03-08   eo    Cleanup non public apis from EFS public headers
2016-04-25   rp    Incorporate code review comments for EFS quota change.
2016-04-25   rp    Create

===========================================================================*/

#include "fs_config_i.h"
#include "fs_quota.h"

#ifdef FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS

#include "comdef.h"
#include "fs_public.h"
#include "fs_sys_types.h"
#include "fs_priv_funcs.h"
#include "fs_deprecated_api.h"
#include "fs_errno.h"
#include "assert.h"

#include <string.h>


#ifdef FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS_ENABLE_ASSERT
  #define EFS_QUOTA_ASSERT(x)  ASSERT(x)
#else
  #define EFS_QUOTA_ASSERT(x)
#endif


/***********************************************************************
 * Store each groups quota limit.
 **********************************************************************/
struct efs_quota_group_size_entry
{
  enum efs_group_id gid;
  uint32 quota_size_in_kb;
};
static struct efs_quota_group_size_entry efs_quota_group_size_list[] =
{
  {EFS_GROUP_ID_GPS,    EFS_QUOTA_SIZE_FOR_GPS_IN_KB},
  {EFS_GROUP_ID_RF,     EFS_QUOTA_SIZE_FOR_RF_IN_KB},
  {EFS_GROUP_ID_MCFG,   EFS_QUOTA_SIZE_FOR_MCFG_IN_KB},
  {EFS_GROUP_ID_SFS,    EFS_QUOTA_SIZE_FOR_SFS_IN_KB},
  {EFS_GROUP_ID_NV,     EFS_QUOTA_SIZE_FOR_NV_IN_KB},
};

/***********************************************************************
 * Complete list of all the parent directories that belong to all the quota
 * groups.
 * There can be more than one directory for any one group.
***********************************************************************/
struct efs_quota_group_path_entry
{
  enum efs_group_id gid;
  char *path;
};
static struct efs_quota_group_path_entry efs_quota_group_path_list[] =
{
  { EFS_GROUP_ID_GPS,   "/GNSS/" },
  { EFS_GROUP_ID_NV,    "/nv/" },
  { EFS_GROUP_ID_RF,    "/nv/item_files/rfnv/" },
  { EFS_GROUP_ID_RF,    "/rf/" },
  { EFS_GROUP_ID_MCFG,  "/mcfg/" },
  { EFS_GROUP_ID_SFS,   "/sfs/" },
};

/***********************************************************************
 * Debug info to capture errors during quota setup.
***********************************************************************/
struct efs_quota_debug_info_type
{
  uint32 set_quota_error_count;
  uint32 chown_error_count;
  uint32 mkdir_error_count;
  uint32 quota_exceed_error_count;
};
static struct efs_quota_debug_info_type efs_quota_debug_info;

#ifdef FS_UNIT_TEST
void
sim_efs_quota_set_quota_size (uint32 gid, uint32 quota_size_in_kb)
{
  struct efs_quota_group_size_entry *entry;
  uint32 i, count;

  count = (sizeof (efs_quota_group_size_list) /
           sizeof (efs_quota_group_size_list[0]));

  for ( i = 0; i < count; ++i )
  {
    entry = &efs_quota_group_size_list[i];
    ASSERT (entry->gid < EFS_GROUP_ID_MAX);
    if (entry->gid == gid)
    {
      entry->quota_size_in_kb = quota_size_in_kb;
      break;
    }
  }
}
#endif /* FS_UNIT_TEST */


int efs_quota_is_enabled_for_clients (void)
{
  return 1;
}

static void efs_quota_set_quota_on_groups (void)
{
  struct efs_quota_group_size_entry *entry;
  struct fs_group_info ginfo;
  uint32 i, count, quota_size_in_bytes;
  int result;

  count = (sizeof (efs_quota_group_size_list) /
           sizeof (efs_quota_group_size_list[0]));

  for ( i = 0; i < count; ++i )
  {
    entry = &efs_quota_group_size_list[i];
    ASSERT (entry->gid < EFS_GROUP_ID_MAX);

    if (entry->quota_size_in_kb == 0)
      continue;

    quota_size_in_bytes = entry->quota_size_in_kb * 1024;

    memset (&ginfo, 0, sizeof (ginfo));
    result = efs_get_group_info ("/", entry->gid, &ginfo);
    if (result == 0)
    {
      if (ginfo.quota_size == quota_size_in_bytes)
        continue;
    }

    result = efs_set_quota ("/", entry->gid, quota_size_in_bytes);
    EFS_QUOTA_ASSERT (result == 0);
    if (result != 0)
      ++efs_quota_debug_info.set_quota_error_count;

    memset (&ginfo, 0, sizeof (ginfo));
    result = efs_get_group_info ("/", entry->gid, &ginfo);
    EFS_QUOTA_ASSERT (result == 0);
    EFS_QUOTA_ASSERT (ginfo.quota_size == quota_size_in_bytes);
    EFS_QUOTA_ASSERT (ginfo.space_used <= quota_size_in_bytes);
    if (ginfo.space_used > quota_size_in_bytes)
      ++efs_quota_debug_info.quota_exceed_error_count;
  }
}

static void efs_quota_do_chown_on_dir_list (void)
{
  struct efs_quota_group_path_entry *entry;
  struct fs_stat sbuf;
  uint32 i, count;
  int result;

  count = (sizeof (efs_quota_group_path_list) /
           sizeof (efs_quota_group_path_list[0]));

  for ( i = 0; i < count; ++i )
  {
    entry = &efs_quota_group_path_list[i];
    ASSERT (entry->gid < EFS_GROUP_ID_MAX);
    ASSERT (entry->path != NULL);

    result = efs_stat (entry->path, &sbuf);
    if (result == 0)
    {
      if (sbuf.st_gid == entry->gid)
        continue;
    }

    if ((result < 0) && (efs_errno == ENOENT))
    {
      result = efs_auto_mkdir (entry->path);
      EFS_QUOTA_ASSERT (result == 0);
      if (result != 0)
        ++efs_quota_debug_info.mkdir_error_count;
    }

    result = efs_chown_recursive (entry->path, -1, entry->gid);
    EFS_QUOTA_ASSERT (result == 0);
    if (result != 0)
      ++efs_quota_debug_info.chown_error_count;
  }
}

void efs_quota_init_for_clients (void)
{
  memset (&efs_quota_debug_info, 0, sizeof (efs_quota_debug_info));
  efs_quota_set_quota_on_groups ();
  efs_quota_do_chown_on_dir_list ();
}

#endif /* FEATURE_EFS_QUOTA_SETUP_FOR_CLIENTS */

