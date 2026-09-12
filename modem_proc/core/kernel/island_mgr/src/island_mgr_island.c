/*======================================================================
                        island_mgr.c 

GENERAL DESCRIPTION
  Sensors uImage Manager functions

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2014  by Qualcomm Technologies, Inc.  All Rights Reserved.
 ======================================================================*/
#include <qurt.h>
#include <island_mgr.h>

qurt_island_t g_island_spec_id;
int g_island_mgr_inited = 0;

int island_mgr_island_enter (void)
{
    int qurt_rc=ISLAND_MGR_EFAILED;

    if (g_island_mgr_inited)
    {
        qurt_rc = qurt_island_enter(g_island_spec_id);
    }
    return qurt_rc;
}

int island_mgr_island_exit (unsigned short stage)
{
    int qurt_rc;

    qurt_rc = qurt_island_exit(stage);
    return qurt_rc;
}

void island_mgr_island_cancel(void)
{
   qurt_island_cancel();
}
