/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     S M E M    L O G G I N G

GENERAL DESCRIPTION
  This module provides extended smem logging in case of fatal and 
  non-fatal errors.  This module is not a task, but rather a set of
  procedures which run in the context of the calling task.

Copyright (c) 2015-2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_smem_log.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/15   abh     File created for error extended smem logging

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "erri.h"
#include "tms_utils.h"
#include "smem.h"
#include "err_smem_log.h"
#include "procinfo_qdi.h"
#include "qurt_restricted.h"

struct err_smem_log_s
{
  char * buf_ptr;
  const char * engg_timestampstring;
  const char * qcom_timestampstring;
  boolean disable_further_writes;
};

struct err_smem_log_s err_smem_log_internal;

/* Code under ERR_F3_TRACE_TO_SMEM flag is deprecated from internal CRMs 
   since A-family targets, however code is available in case if any 
   customer is using this.

   Code under this macro is used to dump error related information in below 
   SMEM_ERR_CRASH_LOG SMEM item of size ERR_DATA_MAX_SIZE.
*/
#if defined(ERR_F3_TRACE_TO_SMEM)
  static char *err_smem_log_buf;
  static err_data_type err_data;
  static void err_store_info(void);
#endif /* ERR_F3_TRACE_TO_SMEM */

static void err_info_to_smem_buffer(void);

static void err_smem_write_buf
( 
  char ** buf,
  unsigned int * buf_size,
  const char * str,
  ...
)
{
  uint32 written =0;
  va_list args;
  
  if ( buf_size <= 0 )
    return;
 
  va_start(args, str);
  
  written = tms_utils_fmt_va_list(*buf, *buf_size, str, args);

  va_end(args); 
	
  if(written > TMS_UTILS_BUF_SZ_ZERO)
  {
    *buf_size -= (written-1);
    *buf += (written-1);
  }
}

/*===========================================================================

FUNCTION err_smem_log_crash

DESCRIPTION
  Logs error information in shared memory buffer to be read by HLOS kernel
   
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS

===========================================================================*/

void err_smem_log_crash
(
  uint32 tid,
  char * pfr,
  qurt_sysevent_error_t * sys_err,
  boolean disable_further_writes,
  unsigned int stack_base,
  unsigned int stack_end
)
{
  unsigned int * sp, stack_size =0, i=0;
  char * buf = err_smem_log_internal.buf_ptr;
  unsigned int buf_size = ERR_SMEM_BUFFER_SIZE;

  if ( err_smem_log_internal.disable_further_writes == TRUE )
    return;

  if ( disable_further_writes == TRUE )
     err_smem_log_internal.disable_further_writes = TRUE;
   
  if ( buf == NULL || buf_size == 0)
    return;

  if ( stack_base == 0 )
  {
    /* qurt_thread_attr_get() returns stack addr from ugp of the root even for user process*/
    /* These functions will not be called for root PD crash and may not be safe for root crash */
    qurt_thread_context_get_stack_base(tid, (void *)&stack_base);
    qurt_thread_context_get_stack_size(tid, (void *)&stack_size);
    stack_end  = stack_base + stack_size; 
  }

  /* Write Version info */
  err_smem_write_buf( &buf, &buf_size, "err_smem_ver.%u.%u:", 2,1);

  /* Write Build Info and crash reason */
  err_smem_write_buf( &buf, &buf_size, "%s:%s:%s:%s:%s:%s:Failure:%s:",
           coredump.image.qc_image_version_string, 
           coredump.image.image_variant_string,
           coredump.image.oem_image_version_string,
           coredump.image.oem_image_uuid_string,
           err_smem_log_internal.engg_timestampstring, 
           err_smem_log_internal.qcom_timestampstring,
           pfr);

  /* Write Registers */
  err_smem_write_buf( &buf, &buf_size, "Registers:SP=0x%08x:FP=0x%08x:PC=0x%08x:SSR=0x%08x:BADVA=0x%08x:LR=0x%08x:",
                             sys_err->sp, sys_err->fp, sys_err->fault_pc,
                             sys_err->ssr, sys_err->badva, sys_err->lr ); 

  sp = (unsigned int *)sys_err->sp; 

  err_smem_write_buf( &buf, &buf_size, "StackDump from:0x%08x to: 0x%08x:", sp, stack_end);

  /* Write Stack Dump */
  if((sp > (unsigned int*)stack_base) && (sp < (unsigned int*)stack_end))
  {
    for(i=0; sp<(unsigned int*)stack_end; i++)
    {
      err_smem_write_buf( &buf, &buf_size, "%08x",(unsigned int)*sp);
      sp++;
    }
  }

  err_smem_write_buf( &buf, &buf_size, "\0");
  
  /* Set the remaining buffer as NULL */
  memset( buf, 0, buf_size);
}

/*===========================================================================
FUNCTION ERR_SMEM_WRITE_TIMESTAMP_STRING

DESCRIPTION
  Writes the timestamp string from the Root PD.

DEPENDENCIES
  Needs to be called only from Root PD.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void err_smem_write_timestamp_string(const char * engg_timestampstring,
                                           const char * qcom_timestampstring )
{
  err_smem_log_internal.engg_timestampstring = engg_timestampstring;
  err_smem_log_internal.qcom_timestampstring = qcom_timestampstring;
  return;
}

/*===========================================================================

FUNCTION err_smem_log_init

DESCRIPTION
  This function initialized the smem log for writing extended smem info

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/
void err_smem_log_init (void)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  int32 smem_rv;
  smem_alloc_params_type smem_params;

  smem_params.remote_host = SMEM_APPS;
  smem_params.smem_type = ERR_LOG_SMEM_ITEM;
  smem_params.size = ERR_SMEM_BUFFER_SIZE;
  smem_params.buffer = NULL;
  smem_params.flags = SMEM_ALLOC_FLAG_NONE;

  smem_rv = smem_alloc_ex(&smem_params);
  if(SMEM_STATUS_SUCCESS != smem_rv)
  {
    TMS_MSG_ERROR_1("SMEM alloc for ERR_LOG_SMEM_ITEM failed, rv=%d", smem_rv);
    goto next;
  }
  else
  {
    /* Initialize the err smem buffer */
    err_smem_log_internal.buf_ptr = smem_params.buffer;
    memset(err_smem_log_internal.buf_ptr, 0, ERR_SMEM_BUFFER_SIZE);
  }

  /* Error info to SMEM buffer callback registration with error handling */
  cb_info.type = ERRCB_TYPE_VOID;
  cb_info.err_cb.cb = err_info_to_smem_buffer;
  cb_info.bucket = ERRCB_BUCKET_NORMAL;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS != rv)
    TMS_MSG_ERROR_1("Registration of Error info to SMEM buffer cb failed, rv = %d", rv);


next:
  /***************************************************************************
            Code under ERR_F3_TRACE_TO_SMEM flag is deprecated!!
  ***************************************************************************/
  #if defined(ERR_F3_TRACE_TO_SMEM)

  err_smem_log_buf = (char *)smem_alloc(SMEM_ERR_CRASH_LOG, ERR_DATA_MAX_SIZE);
  memset(err_smem_log_buf, 0, ERR_DATA_MAX_SIZE);

  /* err_store_info callback registration with error handling */
  cb_info.type = ERRCB_TYPE_VOID;
  cb_info.err_cb.cb = err_store_info;
  cb_info.bucket = ERRCB_BUCKET_NORMAL;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS != rv)
    TMS_MSG_ERROR_1("Registration of err_store_info cb failed, rv = %d", rv);

  #endif // ERR_F3_TRACE_TO_SMEM
  return;
}

/*===========================================================================

FUNCTION err_info_to_smem_buffer

DESCRIPTION
  This function logs below listed error information in shared memory buffer 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/

static void err_info_to_smem_buffer (void)
{
  qurt_thread_attr_t *attr = NULL;
  qurt_sysevent_error_t sys_err;
  char temp[ERR_LOG_MAX_MSG_LEN];
  int written=0, stack_base =0, stack_end =0;
  char pname[ERR_PDNAME_MAX_LEN] = {0};
  const char *msg_ptr = coredump.err.message;

  /* Update the registers */
  sys_err.fault_pc = coredump.arch.regs.name.pc;
  sys_err.sp       = coredump.arch.regs.name.sp;
  sys_err.badva    = coredump.arch.regs.name.badva;
  sys_err.ssr      = coredump.arch.regs.name.ssr;
  sys_err.fp       = coredump.arch.regs.name.fp;
  sys_err.lr       = coredump.arch.regs.name.lr;

  /* If aux_msg is populated, use it as error message */
  if ( coredump.err.aux_msg[0] != 0 )
  {
    msg_ptr = coredump.err.aux_msg;
  }

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_SMEM_LOG_PRE);

  memset(temp, 0, ERR_LOG_MAX_FILE_LEN);

  if ( coredump.err.is_exception == TRUE )
  {
     strlcpy(temp, msg_ptr, ERR_LOG_MAX_MSG_LEN);
  }
  else
  {
     if (PROCINFO_QDI_FAILURE == procinfo_get_name_from_pid(PROCINFO_OEM_NAME_TYPE, coredump.err.pd_id, pname, sizeof(pname)))
     {
        /* If PID not found: pname will revert to qurt provided name */
        procinfo_get_name_from_pid(PROCINFO_INTERNAL_NAME_TYPE, coredump.err.pd_id, pname, sizeof(pname));
     }

     /* Err Fatal message */
     written = tms_utils_fmt(temp, ERR_LOG_MAX_MSG_LEN, 
                                   "EF:%s:0x%x:%s:0x%x:%s:%lu:",
                                   pname,
                                   coredump.err.pd_id,
                                   err_get_tcb_name(),
                                   coredump.err.tid,
                                   coredump.err.filename,
                                   coredump.err.linenum);

     if(written < ERR_LOG_MAX_MSG_LEN && written > TMS_UTILS_BUF_SZ_ZERO )
     {
       written--;
       written += tms_utils_fmt_nostring( temp+written,
                                          (ERR_LOG_MAX_MSG_LEN-written) ,
                                          msg_ptr,
                                          coredump.err.param[0],
                                          coredump.err.param[1],
                                          coredump.err.param[2]);
     }
  }

  /* Get tcb name from tid name. It uses k0lock which is internal to kernel. 
   * But that doesn’t preempt the caller task in any way */
  attr = qurt_system_stm_thread_attr_get(coredump.err.tid);

  if ( attr != NULL )
  {
    stack_base = (unsigned int)attr->stack_addr;      /* base address of the stack */
    stack_end = stack_base + attr->stack_size;  /* Range of the stack */
  }
  
  err_smem_log_crash( coredump.err.tid, temp, &sys_err, TRUE, stack_base, stack_end);
}

/***************************************************************************
        Code under ERR_F3_TRACE_TO_SMEM flag is deprecated!!
***************************************************************************/
#if defined(ERR_F3_TRACE_TO_SMEM)

/*===========================================================================

FUNCTION ERR_STORE_INFO

DESCRIPTION
  Writes some crash related information to a designated RAM buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
============================================================================*/
#define ERR_DATA_MAGIC_NUMBER       (uint64)0x5252452041544144ULL
static void err_store_info (void)
{
  char *buf = NULL;

  ERR_CRUMB_TRAIL_BMSK(ERR_CRUMB_TRAIL_BMSK_STORE_INFO_PRE);
  
  /* If magic numbers are present at the front of the buffer, then previous
   * data has not been written out yet. Return without doing anything in
   * this case. -- this is probably obsolete without nzi, but cheap protection
   */
  if(err_data.err_log.flag == ERR_DATA_MAGIC_NUMBER)
  {
    /* Buffer is already full */
    return;
  }
  err_data.err_log.flag = ERR_DATA_MAGIC_NUMBER;

  /* Copy data needed to store NV log */
  err_data.err_log.nv_log.line_num = coredump.err.linenum;
  err_data.err_log.nv_log.fatal = TRUE;
  (void) strlcpy((char *)err_data.err_log.nv_log.file_name,
                 (char *)coredump.err.filename,
                 MIN( sizeof(err_data.err_log.nv_log.file_name), sizeof(coredump.err.filename) ));

  buf = (char *) err_data.err_log.data;

  /* Interrupts are disabled at this point -- need to make sure we don't
   * timeout.  Use force_kick to avoid intlock attempt in ISR (possibly)
   */
  dog_force_kick();
  (void) memset (buf, 0x00, ERR_DATA_MAX_SIZE);
  err_log_init(buf, ERR_DATA_MAX_SIZE);
  err_data.err_log.length = err_generate_log();

  /* If we have ptr to ERR smem log region, copy log buffer */
  if(err_smem_log_buf!=NULL)
  {
    memscpy((void*)err_smem_log_buf, ERR_DATA_MAX_SIZE, (void*)buf, ERR_DATA_MAX_SIZE);
  }
  //actual f3 trace is handled exclusively by diag now (using err_crash_cb_register)

  return;
} /* err_store_info */

#endif // ERR_F3_TRACE_TO_SMEM
