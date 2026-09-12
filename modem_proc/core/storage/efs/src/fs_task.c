/***********************************************************************
 * fs_task.c
 *
 * FS Task place holder.
 * Copyright (C) 2009-2014, 2016-2017,2019 QUALCOMM Technologies, Inc.
 *
 * The FS-task is as of now in EFS1 VU, when we finally kill EFS1, we have
 * to move the FS-task from EFS1 to this file.
 *
 ***********************************************************************/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/storage/efs/src/fs_task.c#2 $ $DateTime: 2019/12/17 05:16:47 $ $Author: pwbldsvc $

when         who   what, where, why
----------   ---   ---------------------------------------------------------
2019-11-20   vm    Fix the mutex locking order between fs_lib and rest.
2017-01-11   rp    Add support to extract efs-zip-tar via delayed diag.
2016-04-27   rp    Do EFS golden copy files list restore outside efs-init
2016-04-14   rp    Add EFS golden copy creation from files list.
2016-01-21   mj    Added support for RCINIT Group 7 shutdown notification.
2014-04-26   vm    Use correct FS_MSG macros to save RAM.
2013-03-06   nr    Enable security vulnerabilty message.
2013-04-11   dks   Make FS_HEARTBEAT_TIMER be set runtime based on a file.
2013-03-19   dks   Clean up legacy dog usage, make fs_task not dog monitored.
2013-03-06   nr    Add message to indicate a security vulnerabilty is enabled.
2013-02-28   nr    Wait for the rcinit handshake before initializing EFS.
2013-01-28   nr    Clean up references to tmc task.h.
2012-04-30   nr    Register shutdown callback with RCINIT.
2012-03-08   wek   Rename macros and remove header files to compile for boot.
2012-01-17   wek   Remove unused header files to compile for boot.
2011-11-11   rp    Removed dependency on FEATURE_RCINIT.
2011-06-01   dks   Migrate from TMC to rcinit.
2011-01-12   vr    Support EFS removal based on FEATURE_EFS2.
2010-05-19   rp    Fix bug in starting the FS task's watchdog timer.
2010-04-28   vr    Relocate fs_task from EFS1 to EFS2
2009-11-16   rp    Create

===========================================================================*/


#include "fs_public.h"
#include "fs_config_i.h"

#ifdef FEATURE_EFS2

#ifndef FEATURE_EFS_EFS1_PROVIDE_FS_TASK

#include "fs_task_i.h"
#include "fs_sys_types.h"
#include "fs_err.h"
#include "fs_errno.h"
#include "fs_util.h"
#include "fs_osal.h"
#include "task.h"
#include "fs_sync.h"
#include "fs_ftl.h"
#include "fs_priv_funcs.h"
#include "fs_global_lock.h"
#include "fs_diagd.h"

/* Timer to monitor fs-task since it is not monitored by Watchdog. */
static fs_os_timer_type fs_heartbeat_timer;
static uint32 fs_heartbeat_time_msecs = 0;

#define FS_HEARTBEAT_ENABLE_ITEM "/fs_heartbeat.cfg"

#define FS_TASK_DEFAULT_SECURITY_MESSAGE_TIMER 5000

static void
fs_task_heartbeat (void)
{
  if (fs_heartbeat_time_msecs != 0)
  {
    (void) fs_os_clr_sigs (fs_os_self (), FS_HEARTBEAT_TIMER_SIG);
    (void) fs_os_set_timer (&fs_heartbeat_timer, fs_heartbeat_time_msecs);
  }
}

void
fs_task (dword parm)
{
  fs_os_sigs_type task_sigs;
  uint32 timer_value;
  int res;

  (void) parm;

  /* Register the shutdown function */
  FS_TASK_REGISTER_SHUTDOWN (RCINIT_GROUP_0, efs_rcinit_shutdown_callback);

  /* Register for rcinit imminent shutdown in group 7*/
  FS_TASK_REGISTER_SHUTDOWN(RCINIT_GROUP_7, \
                                efs_rcinit_group7_shutdown_callback);

  /* Init and grab the global lock to make sure calls to EFS api are blocked
   * till we are done with EFS initialization
   */
  efs_initialize_locks ();

  /* Wait for start signal and let rcinit know its safe to continue to next
   * group */
  FS_TASK_PRIV_START ();

  FS_MSG_MED_0 ("FS task started");

  if (efs_initialize() != 0)
  {
    FS_ERR_FATAL ("EFS initialize failed", 0, 0, 0);
  }

  /* Define and start heartbeat timer */
  fs_heartbeat_time_msecs = 0;
  res = efs_read_file_helper (FS_HEARTBEAT_ENABLE_ITEM,
        &timer_value, sizeof (timer_value));
  if (res != 0)
  {
    timer_value = 0;
  }

#ifdef FS_BUILD_HAS_SECURITY_VULNERABILITY_ENABLED
/* If there is a security vulnerability then ensure that there is a warning
 *  message */
  if (timer_value == 0)
  {
    timer_value = FS_TASK_DEFAULT_SECURITY_MESSAGE_TIMER;
  }
#endif

  if (timer_value > 0)
  {
    fs_heartbeat_time_msecs = timer_value;
    fs_os_def_timer (&fs_heartbeat_timer,
                     fs_os_self (), FS_HEARTBEAT_TIMER_SIG);
    fs_task_heartbeat ();
  }

  /* Now it is safe to give up global lock. */
  efs_initialize_release_locks ();

  /* Loop forever handling commands */
  for (;;)
  {
    task_sigs = fs_os_wait(FS_HEARTBEAT_TIMER_SIG  | /* Task heartbeat timer */
                           FS_EFS_SYNC_SIG         | /* Sync EFS          */
                           FS_EFS_GOLDEN_COPY_SIG  | /* FSG create        */
                           FS_EFS_GOLDEN_COPY_RESTORE_SIG | /* FSG restore*/
                           FS_FTL_SYNC_SIG | /* Sync FTL cache    */
                           FS_DIAGD_SIG);

    if ((task_sigs & FS_HEARTBEAT_TIMER_SIG) != 0)
    {
      #ifndef FS_BUILD_HAS_SECURITY_VULNERABILITY_ENABLED
        FS_MSG_MED_1 ("%d, FS_HEARTBEAT_TIMER_SIG received [timer msec]",
                     fs_heartbeat_time_msecs);
      #else
        FS_MSG_ERROR_0 (
               "WARNING: EFS SECURITY VULNERABILITY HAS BEEN ENABLED!!");
      #endif
      fs_task_heartbeat ();
    }

    if ((task_sigs & FS_EFS_SYNC_SIG) != 0)
    {
      FS_MSG_MED_0 ("EFS sync timer sig received");
      (void) fs_os_clr_sigs (fs_os_self (), FS_EFS_SYNC_SIG);
      fs_sync_timer_signal_received ();
    }

    if ((task_sigs & FS_EFS_GOLDEN_COPY_SIG) != 0)
    {
      FS_MSG_MED_0 ("EFS golden copy sig received");
      (void) fs_os_clr_sigs (fs_os_self (), FS_EFS_GOLDEN_COPY_SIG);
      efs_golden_copy_signal_received ();
    }

    if ((task_sigs & FS_EFS_GOLDEN_COPY_RESTORE_SIG) != 0)
    {
      FS_MSG_MED_0 ("EFS golden copy sig received");
      (void) fs_os_clr_sigs (fs_os_self (), FS_EFS_GOLDEN_COPY_RESTORE_SIG);
      efs_golden_copy_restore_signal_received ();
    }

    if ((task_sigs & FS_FTL_SYNC_SIG) != 0)
    {
      FS_MSG_MED_0 ("FTL sync sig received");
      (void) fs_os_clr_sigs (fs_os_self (), FS_FTL_SYNC_SIG);
      fs_ftl_sync_timer_signal_received ();
    }

    if ((task_sigs & FS_DIAGD_SIG) != 0)
    {
      FS_MSG_MED_0 ("Diagd sig received");
      (void) fs_os_clr_sigs (fs_os_self (), FS_DIAGD_SIG);
      fs_diagd_signal_received ();
    }

  }

} /* fs_task */

#endif /* FEATURE_EFS_EFS1_PROVIDE_FS_TASK */

#endif /* FEATURE_EFS2 */
