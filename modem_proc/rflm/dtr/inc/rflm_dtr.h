#ifndef DTR_H
#define DTR_H

/*!
   @file
   dtr.h

   @brief
   

*/

/*===========================================================================

Copyright (c) 2010 - 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:37:22 $ $Author: pwbldsvc $
$Header: //components/rel/rflm.mpss/2.11/dtr/inc/rflm_dtr.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
11/18/13   cvd     Initial version.

============================================================================*/ 

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#include "rflm.h"
#include "rflm_diag.h"
#include "atomic_ops.h"



#ifdef __cplusplus
extern "C" {
#endif

#define RFLM_DTR_MAX_READER_THREADS 3

#define DTR_DIAG_TRACE_MSG_LOW(fmt, ... ) \
  {RFLM_DIAG_MSG_LOW( fmt, ## __VA_ARGS__); RFLM_DIAG_TRACE( LOW, RFLM_DIAG_MSG_SSID_NONE, fmt, ## __VA_ARGS__);}

#define DTR_DIAG_TRACE_MSG_MED(fmt, ... ) \
  {RFLM_DIAG_MSG_MED( fmt, ## __VA_ARGS__); RFLM_DIAG_TRACE( MED, RFLM_DIAG_MSG_SSID_NONE, fmt, ## __VA_ARGS__);}

#define DTR_DIAG_TRACE_MSG_HIGH(fmt, ... ) \
  {RFLM_DIAG_MSG_HIGH( fmt, ## __VA_ARGS__); RFLM_DIAG_TRACE( HIGH, RFLM_DIAG_MSG_SSID_NONE, fmt, ## __VA_ARGS__);}

#define DTR_DIAG_TRACE_MSG_ERROR(fmt, ... ) \
  {RFLM_DIAG_MSG_ERROR( fmt, ## __VA_ARGS__); RFLM_DIAG_TRACE( ERROR, RFLM_DIAG_MSG_SSID_NONE, fmt, ## __VA_ARGS__);}


typedef enum
{
  DTR_VERBOSE_LEVEL_0,
  DTR_VERBOSE_LEVEL_1,
  DTR_VERBOSE_LEVEL_2,
  DTR_VERBOSE_LEVEL_3
} dtr_verbose_level;

#define RFLM_DTR_LOCK_ACTIVITY_LOG_SIZE 1024

typedef enum {
  RFLM_DTR_LOCK,
  RFLM_DTR_UNLOCK,
  RFLM_WRITE_LOCK,
  RFLM_READ_LOCK,
} rflm_dtr_lock_action;

typedef struct {
  uint32 handle;
  rflm_dtr_lock_action action;
  uint32 time_stmr;
  void* ret_addr;
  uint32 tid;
  rflm_dtr_lock_action action2;
} rflm_dtr_lock_activity_t;

typedef struct {
  rflm_dtr_lock_activity_t events[RFLM_DTR_LOCK_ACTIVITY_LOG_SIZE];
  atomic_word_t index;
} rflm_dtr_lock_log_t;

extern rflm_dtr_lock_log_t rflm_dtr_lock_log;

/* Lock execution log. Events buffer must be POW2 for
   atomic address wraparound handling to work. */
COMPILE_ASSERT(IS_POW2(RFLM_DTR_LOCK_ACTIVITY_LOG_SIZE));
#define RFLM_DTR_LOCK_LOG(_handle, _action, _time_stmr, _level, _action2) \
  {\
  uint32 i = atomic_inc_return(&rflm_dtr_lock_log.index);\
  atomic_and(&rflm_dtr_lock_log.index, (RFLM_DTR_LOCK_ACTIVITY_LOG_SIZE-1));\
  i=i&(RFLM_DTR_LOCK_ACTIVITY_LOG_SIZE-1);\
  rflm_dtr_lock_activity_t* ptr = &rflm_dtr_lock_log.events[i];\
  ptr->handle = _handle;\
  ptr->action = _action;\
  ptr->time_stmr = _time_stmr;\
  ptr->ret_addr = __builtin_return_address(_level);\
  ptr->action2 = _action2;\
  ptr->tid = qurt_thread_get_id();\
  } 

/*===========================================================================
                           FUNCTION DEFINITIONS
===========================================================================*/
void dtr_print_memory_chunk(uint8* addr, uint32 size);

#ifdef __cplusplus
}
#endif

#endif /* DTR_H */
