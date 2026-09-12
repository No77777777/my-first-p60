#ifndef ERRI_H
#define ERRI_H

/*===========================================================================

                    Error Handling Service Internal Header File

Description

Copyright (c) 2009 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/erri.h#2 $

===========================================================================*/

#include "comdef.h"
#include "err.h"
#include "errlog.h"
#include "err_types.h"

#include "err_hw_qdsp6.h"

#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "qurt.h"
#include "qurt_atomic_ops.h"

#include "err_qurt.h"
#include "tms_dll_api.h"

/* Following values should be accomodated only in a byte. 
 * Cannot be greater than 255  */

typedef enum
{
  ERR_TYPE_ERR_FATAL = 0,
  ERR_TYPE_NON_RECOVERABLE_EXCEPTION = 1,
  ERR_TYPE_MAX = 255
} err_type_e;


#ifndef TMS_UTILS_BUF_SZ_ZERO   
#define TMS_UTILS_BUF_SZ_ZERO       0
#endif

#define ERR_GET_PID_TO_UPPER_8_BITS             ( (qurt_getpid()<<8) & 0xFF00 )
#define ERR_RAISE_EXCEPTION_ARG(x)              ( QURT_ECODE_UPPER_ERR_SERVICES | ERR_GET_PID_TO_UPPER_8_BITS | (x) )
#define ERR_GET_ERR_TYPE_FROM_EXCEPTION_ARG(x)  (0xFF & x )
#define ERR_GET_PID_FROM_EXCEPTION_ARG(x)       ( (0xFF00 & x ) >> 8 )

#define ERR_TEST_FOR_ERR_FATAL(x) \
        ( (QURT_ECODE_UPPER_ERR_SERVICES | ERR_TYPE_ERR_FATAL) == (0x00FF00FF & x) )

/* Temporary Definition .. modify it after kernel change */
#ifndef QURT_ECODE_UPPER_ERR_SERVICES
#define QURT_ECODE_UPPER_ERR_SERVICES QURT_ECORE_UPPER_ERR_SERVICES
#endif 

#define ERR_FLUSH_ADDR(ADDR) \
  do { \
     asm volatile ("dccleana(%0)" : : "r" ((qurt_addr_t )( &ADDR )));\
   } while (0)

#define ERR_SET_AND_FLUSH_PTR(PTR, VAL) \
  do { \
     PTR = (void*)VAL; \
     asm volatile ("dccleana(%0)" : : "r" ((qurt_addr_t )( &PTR )));\
   } while (0)


boolean err_log_store ( word , const char* , boolean );
void err_pause_usec(uint32 usec);

void err_exception_handler(void * );
void err_spawn_exception_handler(void);
void err_init(void);
void err_stm_thrd_init(void);
void err_qdi_init(void);
void err_initialize_coredump(void);

void err_fatal_jettison_core (
  unsigned int line,       /* From __LINE__ */
  const char   *file_name, /* From __FILE__ */
  const char   *format,    /* format string */
  uint32 param1,
  uint32 param2,
  uint32 param3
);

void err_fatal_lock_init_check(void);
void err_fatal_lock( void );
void err_fatal_try_lock(void);
void err_fatal_unlock(void);
char* err_get_tcb_name(void);
void err_enter_stm_mode(void);
void err_fatal_post_exception_processing(void);
void err_emergency_error_recovery( void );
void err_mem_cache_clean(void);
void err_update_coredump_tid_and_is_exception(uint32 tid, boolean is_exception);
void err_raise_to_kernel(void);
void err_update_reentrancy_flag(void);

void jettison_core( void );
void err_halt_execution( void ) DLL_API_NORETURN;

void err_update_image_versioning_info (void);

void err_execute_external_cb_pre_STM(uint32);
void err_execute_external_cb_post_STM_helper(void);
void err_execute_external_cb_normal_helper(void);
void err_execute_external_cb_late_helper(void);
void err_execute_external_cb_last_helper(void);
void err_user_qdi_communicate(err_cb_bucket_t bucket);
void err_execute_external_cb(err_cb_bucket_t bucket, uint32 data, uint32 crumb_bits);

void err_target_update_coredump_and_f3_trace( err_fatal_params_type * params );

void err_qdi_init(void);
void err_qdi_invoke_all_user_process_cbs(err_cb_bucket_t);
void err_qdi_process_exception_in_separate_context
(
  unsigned int pid,
  unsigned int tid,
  qurt_sysevent_error_t * sys_err 
);

void ubootstrap_exception_handler(void);

void dog_force_bite(void);
void dog_force_kick(void);
void sys_m_register_fatal_notification(void);
void err_stm_thrd_signal_and_wait(void);

#endif /* ERRI_H */
