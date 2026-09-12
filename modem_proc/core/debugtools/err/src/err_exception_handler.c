/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        M A I N   M O D U L E

GENERAL DESCRIPTION
  This module contains the AMSS exception handler

EXTERNALIZED FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2007-2020 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_exception_handler.c#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/27/16   bbh     Get process name from procinfo server
04/01/15   abh     Added changes for extended smem logging
08/27/14   din     Replaced snprintf with mutex free calls.
07/31/14   din     Made some changes to support User PD.
07/02/14   din     Accomodate Qurt argument change to complete ERR_FATAL call flow.
04/18/14   din     Added changes for ADSP island mode.
04/03/14   psu     Update diag macros using optimized versions.
09/22/08   tbg     Merged in changes to support FEATURE_QUARTZ_20
07/25/07   tbg     Initial verion (pulled code from mobile.c)
===========================================================================*/

#include <stdio.h>
#include "comdef.h"
#include "erri.h"
#include "qurt.h"
#include "qurt_restricted.h"
#include "qurt/err_qurt.h"
#include "qurt_event.h"
#include "tms_utils.h"
#include "procinfo_qdi.h"


char              exception_msg[ERR_LOG_MAX_MSG_LEN];

/*===========================================================================
FUNCTION err_has_err_fatal_occured

DESCRIPTION
  Checks if exception is raised because of error fatal

===========================================================================*/

static boolean err_has_err_fatal_occured
(
  unsigned int cause
)
{
   if ( QURT_EXCEPT_EXIT == (cause & 0xFF))
   {
     if(NULL == err_fatal_params.msg_const_ptr)
     {
       /*NULL indicates that qurt_exception_raise_nonfatal() was called by some module 
         directly so need to treat that as exception*/
       return FALSE;
     }
     return TRUE;
   }

   return FALSE;

} /* err_has_err_fatal_occured */


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
  union arch_coredump_union *p_regs=&coredump.arch.regs;
  char pname[ERR_PDNAME_MAX_LEN] = {0};
  unsigned int err_thread_pid =0, pid =0;
  boolean is_exception = FALSE;

  sys_m_register_fatal_notification();

  #ifdef UERR_ISLAND_MODE
  ubootstrap_exception_handler();
  #endif /* UERR_ISLAND_MODE */

  for (;;)
  {
    /* Register self as Exception_Handler. */
    tid = qurt_exception_wait2(&sys_err);

    /* Try acquiring err_fatal_mutex*/
    err_fatal_try_lock();

    dog_force_kick();

    if (-1==tid)
    {
      (void) tms_utils_fmt(coredump.err.int_msg, sizeof(coredump.err.int_msg),
        "Failed to register with qurt_reg_error_handler: tid=%x", tid);

      /* Failed to register with qurt_reg_error_handler */
      err_emergency_error_recovery();
    }

    /* Get the err_thread_pid */
    err_thread_pid = sys_err.pid;
    /* Get the current executing thread PID */
    pid = qurt_process_get_id();

    /* If the error is seen from a user process */
    if ( err_thread_pid != pid )
    {
      err_qdi_process_exception_in_separate_context(err_thread_pid, tid, &sys_err );
      err_fatal_unlock();
    }
    else
    {
      /* There are various ways to reach here 
      * 1. Err-Fatal\ Exception in Root process. 
      * 2. Non recoverable exception raised by island exception handler. 
      * 3. Error fatal in uImage. 
      * 4. uImage exception redirected to fatal exception handler from island exception handler
      * 5. User Process ERR_FATAL handled through QDI layer
      * 6. User Process exception -> user process exception handler -> QDI layer -> ERR_FATAL
      * 7. Unhandled exceptions from user process
      * */
 
      /* Executes Pre STM Callback */
      err_execute_external_cb_pre_STM(tid);
      err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_PRE_STM);

      /* Move STM threads to READY\RUN state */
      err_stm_thrd_signal_and_wait();

      /* Halt other HW threads for Root process or goto into sw freeze for user process*/
      err_enter_stm_mode();

      dog_force_kick();

      /* Update the registers */
      p_regs->name.pc    = sys_err.fault_pc;
      p_regs->name.sp    = sys_err.sp;
      p_regs->name.badva = sys_err.badva;
      p_regs->name.ssr   = sys_err.ssr;
      p_regs->name.fp    = sys_err.fp;
      p_regs->name.lr    = sys_err.lr;

      if ( FALSE == err_has_err_fatal_occured(sys_err.cause) )
      {
        is_exception = TRUE;
      }

        /* Update tid and is_exception to coredump */
      err_update_coredump_tid_and_is_exception(tid, is_exception);

      if ( is_exception )
      {
        ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_EXCEPTION_PRE);

        /* Clean cache */
	err_mem_cache_clean();

	err_execute_external_cb(ERRCB_BUCKET_POST_STM, tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_POST_STM_PRE);

	/* Clean cache */
	err_mem_cache_clean();

        err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_POST_STM);

        if (PROCINFO_QDI_FAILURE == procinfo_get_name_from_pid(PROCINFO_OEM_NAME_TYPE, err_thread_pid, pname, sizeof(pname)))
        {
          /* If PID not found: pname will revert to qurt provided name */
          procinfo_get_name_from_pid(PROCINFO_INTERNAL_NAME_TYPE, err_thread_pid, pname, sizeof(pname));
        }

	/* Exception message */
        if ( sizeof(exception_msg) <= tms_utils_fmt(exception_msg, sizeof(exception_msg),
          "EX:%s:0x%x:%s:0x%x:PC=0x%x", pname, err_thread_pid, err_get_tcb_name(), tid, sys_err.fault_pc))
        {
          /* Record failure to coredump */
          strlcpy(coredump.err.int_msg, "Failed to copy exception_msg", 
            sizeof("Failed to copy exception_msg"));
        }
    
        /* Perform higher level error logging - no return */
        err_fatal_jettison_core ( err_thread_pid, NULL, exception_msg, 0, 0, 0);
      }
      else 
      {
        err_fatal_post_exception_processing();
      }
    }
  }
} /* end of err_exception_handler */
