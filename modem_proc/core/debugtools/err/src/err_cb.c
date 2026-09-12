/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides error reporting services for both fatal and
  non-fatal errors.  This module is not a task, but rather a set of
  callable procedures which run in the context of the calling task.

Copyright (c) 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_cb.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/15   din     Adapted from err.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "qurt.h"
#include "erri.h"
#include "tms_utils.h"

/*===========================================================================

                 Defines and variable declarations for module

===========================================================================*/

/* Macros for CB Mutex type - LOCK will initialize if not initialized */
#define ERR_CB_MUTEX_LOCK(x)         if(qurt_atomic_compare_and_set(&err_cb_internal.mutex_init, FALSE, TRUE)) {   \
                                       ERR_MUTEX_INIT(x);                    \
                                     }                                       \
                                     ERR_MUTEX_LOCK(x);

#define ERR_CB_MUTEX_UNLOCK          ERR_MUTEX_UNLOCK


typedef struct {
  ERR_MUTEX_TYPE mutex; /* CB register/deregister protection */
  unsigned int   mutex_init;
} err_cb_internal_s;

static err_cb_internal_s err_cb_internal;


/*===========================================================================

                              Function definitions

===========================================================================*/

/*===========================================================================

FUNCTION err_execute_external_cb_pre_STM

DESCRIPTION
 Calls err pre STM cb

DEPENDENCIES

RETURN VALUE
  No return.

SIDE EFFECTS

===========================================================================*/
void err_execute_external_cb_pre_STM(uint32 tid)
{
  err_execute_external_cb(ERRCB_BUCKET_PRE_STM, tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_PRE_STM_PRE);
} /* err_execute_external_cb_pre_STM */

/*===========================================================================

FUNCTION err_execute_external_cb_post_STM_helper

DESCRIPTION
 Calls err next to STM cb

DEPENDENCIES

RETURN VALUE
  No return.

SIDE EFFECTS

===========================================================================*/
void err_execute_external_cb_post_STM_helper(void)
{
  err_execute_external_cb(ERRCB_BUCKET_POST_STM, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_POST_STM_PRE);
}

/*===========================================================================

FUNCTION       err_execute_external_cb_normal_helper

DESCRIPTION
  Executes external callbacks of normal bucket

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void err_execute_external_cb_normal_helper(void)
{
  err_execute_external_cb(ERRCB_BUCKET_NORMAL, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_NORMAL_PRE);
}

/*===========================================================================

FUNCTION       err_execute_external_cb_late_helper

DESCRIPTION
  Executes external callbacks of late bucket

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void err_execute_external_cb_late_helper(void)
{
  err_execute_external_cb(ERRCB_BUCKET_LATE, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CBS_LATE_PRE);
}

/*===========================================================================

FUNCTION       err_execute_external_cb_last_helper

DESCRIPTION
  Executes external callback of last bucket

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void err_execute_external_cb_last_helper(void)
{
  err_execute_external_cb(ERRCB_BUCKET_LAST, coredump.err.tid, ERR_CRUMB_TRAIL_BMSK_EXTERNEL_CB_LAST_PRE);
}


/** err_cb_enable
 * This will allow clients to register for callback to be invoke in path
 * of error handling post-crash.
 *
 * @param cb_info err_cb_info_t structure having required info.
 *
 * @return ERRCB_E_SUCCESS in case of successful registration else
 *         err_cb_error_t error codes.
 */
DLL_API_GLOBAL err_cb_error_t err_cb_enable(err_cb_info_t * cb_info)
{
  int i = 0;

  /* input validation */
  if(cb_info->err_cb.cb == NULL && cb_info->err_cb.cb_u32 == NULL)
    return ERRCB_E_INVALID_PARAM;

  /* only normal order allowed for now */
  if(cb_info->order != ERRCB_ORDER_NORMAL)
    return ERRCB_E_NOT_ALLOWED;

  ERR_CB_MUTEX_LOCK(&err_cb_internal.mutex);

  /* only one cb is allowed in this bucket */
  if(cb_info->bucket == ERRCB_BUCKET_LAST)
  {
    for (i=0; i<ERR_CB_MAX; i++)
    {
      if(coredump.err.err_cb[i].info.err_cb.cb != NULL)
      {
        if (coredump.err.err_cb[i].info.bucket == ERRCB_BUCKET_LAST)
        {
          ERR_CB_MUTEX_UNLOCK(&err_cb_internal.mutex);
          return ERRCB_E_NOT_ALLOWED;
        }
      }
    }
  }

  /* register callback */
  for (i=0; i<ERR_CB_MAX; i++)
  {
    if(coredump.err.err_cb[i].info.err_cb.cb == NULL)
    {
      coredump.err.err_cb[i].info.type = cb_info->type;
      /* err_cb is a union of cb and cb_u32 and are of the same size */
      coredump.err.err_cb[i].info.err_cb.cb = cb_info->err_cb.cb;
      coredump.err.err_cb[i].info.bucket = cb_info->bucket;
      coredump.err.err_cb[i].info.order = cb_info->order;
      ERR_CB_MUTEX_UNLOCK(&err_cb_internal.mutex);
      err_user_qdi_communicate(cb_info->bucket);
      return ERRCB_E_SUCCESS;
    }
  }

  ERR_CB_MUTEX_UNLOCK(&err_cb_internal.mutex);
  return ERRCB_E_MAX_LIMIT_REACHED;
}

/** err_cb_disable
 * This will allow clients to de-register callback, which was register via 
 * err_cb_enable.
 *
 * @param cb_info err_cb_info_t structure having required info.
 *
 * @return ERRCB_E_SUCCESS in case of successful de-registration else
 *         err_cb_error_t error codes.
 */
DLL_API_GLOBAL err_cb_error_t err_cb_disable(err_cb_info_t * cb_info)
{
  int i = 0;
  
  if(cb_info->err_cb.cb == NULL && cb_info->err_cb.cb_u32 == NULL)
    return ERRCB_E_INVALID_PARAM;

  ERR_CB_MUTEX_LOCK(&err_cb_internal.mutex);

  for (i=0; i<ERR_CB_MAX; i++)
  {
    if(coredump.err.err_cb[i].info.err_cb.cb == cb_info->err_cb.cb)
    {
      coredump.err.err_cb[i].info.type = 0;
      coredump.err.err_cb[i].info.err_cb.cb = NULL;
      coredump.err.err_cb[i].info.bucket = 0;
      coredump.err.err_cb[i].info.order = 0;
      ERR_CB_MUTEX_UNLOCK(&err_cb_internal.mutex);      
      return ERRCB_E_SUCCESS;
    }
  }

  ERR_CB_MUTEX_UNLOCK(&err_cb_internal.mutex);
  return ERRCB_E_NOT_FOUND;
}

/*=========================================================================

FUNCTION err_crash_cb_register

DESCRIPTION
  Registers a function (ptr type err_cb_ptr) to be called after an ERR_FATAL
  Function should NOT rely on any messaging, task switching (or system calls
  that may invoke task switching), interrupts, etc.

  !!!These functions MUST NOT call ERR_FATAL/ASSERT under ANY circumstances!!!

DEPENDENCIES
  None

RETURN VALUE
  TRUE if function added to table successfully
  FALSE if function not added.

SIDE EFFECTS
  None

===========================================================================*/
DLL_API_GLOBAL boolean err_crash_cb_register(err_cb_ptr cb)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  cb_info.type = ERRCB_TYPE_VOID;
  cb_info.err_cb.cb = cb;
  cb_info.bucket = ERRCB_BUCKET_NORMAL;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS == rv)
    return TRUE;
  else
    return FALSE;
}


/*=========================================================================

FUNCTION err_crash_cb_reg_pre_STM

DESCRIPTION
  Registers a function (ptr type err_cb_ptr) to be called immediately before
  STM API call 
  Function should NOT rely on any messaging, task switching (or system calls
  that may invoke task switching), interrupts, etc.
 
  !!!These functions MUST NOT call ERR_FATAL under ANY circumstances!!!

DEPENDENCIES
  None
 
RETURN VALUE
  TRUE if function added to table successfully
  FALSE if function not added.

SIDE EFFECTS
  Only one registration of such API is supported so if its used by more than one
  clients than it will overwrite the old registered callback.
  
===========================================================================*/
DLL_API_GLOBAL boolean err_crash_cb_reg_pre_STM(err_cb_ptr_u32 cb)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  cb_info.type = ERRCB_TYPE_UINT32;
  cb_info.err_cb.cb_u32 = cb;
  cb_info.bucket = ERRCB_BUCKET_PRE_STM;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS == rv)
    return TRUE;
  else
     return FALSE;
} /* err_crash_cb_reg_pre_STM */

/*=========================================================================

FUNCTION err_crash_cb_dereg_pre_STM

DESCRIPTION
 Deregisters a function from the error callback table.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if removed
  FALSE if function is not found in table

SIDE EFFECTS
  None

===========================================================================*/
DLL_API_GLOBAL boolean err_crash_cb_dereg_pre_STM(err_cb_ptr_u32 cb)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  cb_info.type = ERRCB_TYPE_UINT32;
  cb_info.err_cb.cb_u32 = cb;
  cb_info.bucket = ERRCB_BUCKET_PRE_STM;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_disable(&cb_info);
  if(ERRCB_E_SUCCESS == rv)
    return TRUE;
  else
    return FALSE;
} /* err_crash_cb_dereg_pre_STM */

/*=========================================================================

FUNCTION err_crash_cb_reg_next_to_STM

DESCRIPTION
  Registers a function (ptr type err_cb_ptr) to be called immediately after
  STM API call 
  Function should NOT rely on any messaging, task switching (or system calls
  that may invoke task switching), interrupts, etc.
 
  !!!These functions MUST NOT call ERR_FATAL under ANY circumstances!!!

DEPENDENCIES
  None
 
RETURN VALUE
  TRUE if function added to table successfully
  FALSE if function not added.

SIDE EFFECTS
  Only one registration of such API is supported so if its used by more than one
  clients than it will overwrite the old registered callback,
  this API was provided only for special case handling to stop ULT Audio Core
  in DPM PL
===========================================================================*/
DLL_API_GLOBAL boolean err_crash_cb_reg_next_to_STM(err_cb_ptr cb)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  cb_info.type = ERRCB_TYPE_VOID;
  cb_info.err_cb.cb = cb;
  cb_info.bucket = ERRCB_BUCKET_POST_STM;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS == rv)
    return TRUE;
  else
     return FALSE;
}

/*===========================================================================
FUNCTION err_crash_cb_postflush_register
DESCRIPTION
  Register for callback function. The Callback function will be called after
  external functions are already called.
  It will be upto the callback to resolve its cache issues. 
============================================================================*/
DLL_API_GLOBAL boolean err_crash_cb_postflush_register (err_cb_ptr cb)
{
  err_cb_info_t cb_info;
  err_cb_error_t rv = ERRCB_E_FAILURE;

  cb_info.type = ERRCB_TYPE_VOID;
  cb_info.err_cb.cb = cb;
  cb_info.bucket = ERRCB_BUCKET_LATE;
  cb_info.order = ERRCB_ORDER_NORMAL;

  rv = err_cb_enable(&cb_info);
  if(ERRCB_E_SUCCESS == rv)
    return TRUE;
  else
    return FALSE;

} /* err_crash_cb_postflush_register*/
