/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        ERR EXCEPTION TASK MODULE

GENERAL DESCRIPTION
  This module contains the task definition for err exception handler, when not running in main().

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2007 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_exception_task.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/18/14   din     Added changes for ADSP island mode.
===========================================================================*/
#include "DALSys.h"
#include "erri.h"
#include "tms_utils.h"
#include "qurt.h"

void err_spawn_exception_handler(void)
{
   char tname[13];

   static unsigned int task_spawn_flag = FALSE;

   if (qurt_atomic_compare_and_set(&task_spawn_flag, FALSE, TRUE) )
   {
     if ( sizeof(tname) <= tms_utils_fmt(tname, 12, "err_ex_pd_%d", qurt_getpid()) )
       err_emergency_error_recovery();
    (void)tms_utils_launch_qurt_task(tname, TMS_TASK_STACK_4K, TMS_TASK_PRIO_EXCEPTION,  err_exception_handler, NULL, TRUE);
   }
}


void bootstrap_exception_handler(void)
{
   err_init();
   err_qdi_init();

   err_spawn_exception_handler();

   err_stm_thrd_init();
}
