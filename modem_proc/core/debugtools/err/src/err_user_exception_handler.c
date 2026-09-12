/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        M A I N   M O D U L E

GENERAL DESCRIPTION
  This module contains the AMSS exception handler

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2014 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_user_exception_handler.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/15   abh     Added changes for extended smem logging
08/27/14   din     Replaced snprintf with mutex free calls.
07/31/14   din     File created to support Error on User PD.
===========================================================================*/

#include <stdio.h>
#include "comdef.h"
#include "qurt_event.h"
#include "qurt_restricted.h"
#include "erri.h"
#include "err_qdi_client.h"
#include "tms_utils.h"

/*===========================================================================
FUNCTION err_exception_handler

DESCRIPTION
  Handle IPC from QURT Kernel when exceptions occur.

===========================================================================*/
void err_exception_handler
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  void *                           ignored    /*lint -esym(715,ignored) */
)
{
  unsigned int     tid;
  qurt_sysevent_error_t sys_err;

  for (;;)
  {
    /* Register self as Exception_Handler. */
    tid = qurt_exception_wait2(&sys_err);

    /* Try acquiring err_fatal_mutex*/
    err_fatal_try_lock();

    /* Communicate tid to root if user expcetion handler is launched, this is to 
     * make sure that root processes the exception from original thread */
    err_qdi_client_communicate_tid(tid, sys_err.fault_pc, sys_err.sp, sys_err.badva, sys_err.cause, sys_err.fp, sys_err.lr, sys_err.ssr);

    /* Executes Pre STM Callback */
    err_execute_external_cb_pre_STM(tid);

    /* Give it back to QuRT to hand it over to root process exception handler */
    qurt_exception_raise_nonfatal(1);
    
  }
} /* end of err_exception_handler */
