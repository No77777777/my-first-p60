/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides error related functionality for user PD.

Copyright (c) 2015 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_root.c#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/15   din     Adapted from err.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "qurt.h"
#include "qurt_restricted.h"
#include "erri.h"
#include <stdlib.h>
#include <stringl.h>
#include "err_decompress.h"
#include "tms_utils_msg.h"
#include "err_smem_log.h"

/*===========================================================================

                      Prototypes for internal functions

===========================================================================*/
void err_fatal_handler( void ) ;
void err_fatal_jettison_core (unsigned int line, const char *file_name,
  const char *format, uint32 param1, uint32 param2, uint32 param3);


/*===========================================================================

                              Function definitions

===========================================================================*/

/*=========================================================================

FUNCTION err_user_qdi_communicate

DESCRIPTION
  Registers CB with Root

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successfull
  FALSE if failed

SIDE EFFECTS
  None

===========================================================================*/

void err_user_qdi_communicate(err_cb_bucket_t bucket)
{
}

/*===========================================================================

FUNCTION       err_mem_cache_clean

DESCRIPTION
  Flushes D-cache for root PD only

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void err_mem_cache_clean(void)
{
  qurt_mem_cache_clean(0,0, QURT_MEM_CACHE_FLUSH_ALL, QURT_MEM_DCACHE);

} /* err_mem_cache_clean*/

/*===========================================================================

FUNCTION       err_fatal_post_exception_processing

DESCRIPTION
  This is called from exception handler after error fatal raises an 
  exception

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void err_fatal_post_exception_processing(void)
{
  /* Copy trace log from island mode data structure */
  coredump.err.crumb_trail_bmsk.bitmap |= err_fatal_params.crumb_trail_bmsk.bitmap;
  
  /* Copy register content */
  memscpy( coredump.arch.regs.array, SIZEOF_ALLOCATED_COREDUMP_REG,
             err_fatal_params.array,  SIZEOF_ARCH_COREDUMP_REGISTERS * sizeof(uint32) );

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_CACHCE_CLEAN_PRE);
  
  /* Clean Cache */
  err_mem_cache_clean();

  err_execute_external_cb(ERRCB_BUCKET_POST_STM, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_POST_STM_PRE);

  /* Clean Cache */
  err_mem_cache_clean();

  err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_POST_STM);

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_COREDUMP_UPDATED_PRE);

  /* Decompress and f3 trace */
  err_target_update_coredump_and_f3_trace(&err_fatal_params);

} /* err_fatal_post_exception_processing */

/*===========================================================================

FUNCTION       err_enter_stm_mode

DESCRIPTION
  Halt other HW threads for Root process or goto into sw freeze for user process

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void err_enter_stm_mode(void)
{
   ERR_FATAL_ENTER_SINGLE_THREADED_MODE();
}


/*===========================================================================

FUNCTION ERR_FATAL_CORE_DUMP
DESCRIPTION
  Logs fatal error information, including a core dump.

  NOTE: There is no return from this function.
============================================================================*/
void err_fatal_core_dump (
  unsigned int line,      /* From __LINE__ */
  const char   *file_name, /* From __FILE__ */
  const char   *format   /* format string */
)
{
  err_fatal_lock();
  err_fatal_jettison_core(line, file_name, format, 0, 0, 0);
}

/*===========================================================================

FUNCTION ERR_GET_TCB_NAME
DESCRIPTION
  Returns TCB NAME of the faulting thread.
  Should be used only after err_update_coredump_tid_and_is_exception()

  NOTE: There is no return from this function.
============================================================================*/

char * err_get_tcb_name (void)
{
  return coredump.err.tcb_name;
}


/*===========================================================================

FUNCTION ERR_FATAL_JETTISON_CORE
DESCRIPTION
  Logs fatal error information, including a core dump.
  Not to be called directly by outside code -- for that, use the function
  err_fatal_core_dump().

  NOTE: There is no return from this function.
============================================================================*/
#define ERR_FATAL_EXCEPTION_REENTRANCY "ERR_FATAL_EXCEPTION_REENTRANCY"

void err_fatal_jettison_core (
  unsigned int line,       /* From __LINE__ */
  const char   *file_name, /* From __FILE__ */
  const char   *format,    /* format string */
  uint32 param1,
  uint32 param2,
  uint32 param3
)
{

  /* NOTE: register information should already be saved prior to
   * calling this function.
   */

  /* Store line number */
  coredump.err.linenum = line;

  /* Copy file name */
  if(file_name != 0)
  {
    (void) strlcpy((char *)coredump.err.filename,
                       (char *)file_name,
                       ERR_LOG_MAX_FILE_LEN);
  }

  /* Copy message string */
  if(format != 0)
  {
    (void) strlcpy((char *)coredump.err.message,
                       (char *)format,
                       ERR_LOG_MAX_MSG_LEN);
  }

  coredump.err.param[0]=param1;
  coredump.err.param[1]=param2;
  coredump.err.param[2]=param3;

  if (err_fatal_params.aux_msg[0] != 0)
  {
    (void) strlcpy((char *)coredump.err.aux_msg,
                       (char *)err_fatal_params.aux_msg,
                       ERR_LOG_MAX_MSG_LEN);
  }

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_ERR_FATAL_JETTISION_CORE_POST);

  /* Update the coredump pid */
  qurt_thread_context_get_pid( coredump.err.tid, 
		               &coredump.err.pd_id ); 

  /* Check if an user exceptions and ERR_FATAL arrived simultaneously */
  if ( err_fatal_params.crumb_trail_bmsk.bitmap != 0 && 
        coredump.err.is_exception == TRUE)
  {
    /* Record secondary failure to coredump */
    strlcpy(coredump.err.int_msg, ERR_FATAL_EXCEPTION_REENTRANCY, 
	    sizeof(ERR_FATAL_EXCEPTION_REENTRANCY));   
  } 

  /* Call ERR_FATAL handler (no return) */
  err_fatal_handler();

}

/*=========================================================================

FUNCTION err_update_crumb_trail_mask

DESCRIPTION
  Updates Crumb trail mask

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void err_update_crumb_trail_mask(uint32 bits)
{
  ERR_CRUMB_TRAIL_BMSK(bits);

} /* err_update_crumb_trail_mask*/

/*===========================================================================

FUNCTION ERR_UPDATE_COREDUMP_TID_AND_IS_EXCEPTION

DESCRIPTION
 Updates tid information to coredump only the tid is not updated earlier.

DEPENDENCIES

RETURN VALUE
  No return.

SIDE EFFECTS
  None.

===========================================================================*/

void err_update_coredump_tid_and_is_exception(uint32 tid, boolean is_exception)
{
  qurt_thread_attr_t *attr = NULL;
  uint32 stack_base = 0;  /* lower start address of the stack */
  uint32 stack_end = 0;   /* higher adress, i.e. range of the stack */
	  
  if ( coredump.err.tid == 0 )
  {
    coredump.err.tid = tid;
  }
  coredump.err.is_exception = is_exception;

  /* Get tcb name from tid name. It uses k0lock which is internal to kernel. 
   * But that doesn’t preempt the caller task in any way */
  attr = qurt_system_stm_thread_attr_get(coredump.err.tid);

  if ( attr != NULL )
  {
    strlcpy(coredump.err.tcb_name, attr->name, QURT_THREAD_ATTR_NAME_MAXLEN );
    stack_base = (uint32)attr->stack_addr;      /* base address of the stack */
    stack_end = stack_base + attr->stack_size;  /* Range of the stack */
  }

  coredump.err.stack_error = STACK_NO_ERROR;

  /* Only check for stack_error if we got the stack addresses */
  if ( stack_base!= 0 && stack_end != 0 )
  {
    /* Check for stack overflow */
    if ( (coredump.arch.regs.name.fp < stack_base) ||
    	   (coredump.arch.regs.name.sp < stack_base) )
    {
      /* stack overflow detected */
  	  coredump.err.stack_error |= STACK_OVERFLOW;
    }

    /* Check for stack underflow */
    if ( (coredump.arch.regs.name.fp > stack_end) ||
    	   (coredump.arch.regs.name.sp > stack_end) )
    {
      /* stack underflow detected */
  	  coredump.err.stack_error |= STACK_UNDERFLOW;
    }
  }
  else
  {
    coredump.err.stack_error |= STACK_ERR_NO_INFO;
  }

} /* err_update_coredump_tid_and_is_exception */

/*===========================================================================

FUNCTION ERROR_FATAL_HANDLER

DESCRIPTION
  This function is invoked from err_fatal_jettison_core. When using JTAG,
  default breakpoint for ERR_FATAL should be placed at this function.
  Will log error to SMEM, kill the PA, and copy the coredump data into
  the err_data structure in unintialized memory.


DEPENDENCIES

RETURN VALUE
  No return.

SIDE EFFECTS
  **************************************************************
  ************ THERE IS NO RETURN FROM THIS FUNCTION ***********
  **************************************************************

===========================================================================*/
void err_fatal_handler ( void )
{
  static uint32 err_count=0;

  /* Clean Cache */
  err_mem_cache_clean();

  err_count++;

  if((err_count>1))
  {
    err_initialize_coredump();
  
    /* May not return */
    err_emergency_error_recovery();
  }

  err_execute_external_cb(ERRCB_BUCKET_NORMAL, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_NORMAL_PRE);

  /* Clean Cache */
  err_mem_cache_clean();

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_USER_PROCESS_CBS_PRE);
  
  err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_NORMAL);

  /* Clean Cache */
  err_mem_cache_clean();

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_USER_PROCESS_CBS_POST);

  err_execute_external_cb(ERRCB_BUCKET_LATE, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_LATE_PRE);

  /* Clean Cache */
  err_mem_cache_clean();

  err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_LATE);

  err_execute_external_cb(ERRCB_BUCKET_LAST, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CB_LAST_PRE);

  /* Clean Cache */
  err_mem_cache_clean();

  err_qdi_invoke_all_user_process_cbs(ERRCB_BUCKET_LAST);
  
  coredump.err.err_cb_current = err_raise_to_kernel;

   /* Clean Cache */
  err_mem_cache_clean();

  err_raise_to_kernel();

} /* err_fatal_handler */
