/*=============================================================================

                 err_qdi.c

GENERAL DESCRIPTION
   Implements QDI layer for Err that goes into main image

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

Copyright (c) 2014 - 2019 by Qualcomm Technologies, Inc.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.
 
$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err_qdi.c#5 $ 
$DateTime: 2019/11/11 03:27:16 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/01/15   abh     Added changes for extended smem logging
08/27/14   din     Replaced snprintf with mutex free calls.
07/31/14   din     File created to support Error on User PD.

=============================================================================*/


/*****************************************************************************/
/*                           INCLUDE FILES                                   */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stringl/stringl.h>
#include "err_qdi.h"
#include "pd_mon_restart.h"
#include "tms_servreg.h"
#include "err.h"
#include "tms_utils.h"
#include "tms_utils_msg.h"
#include "err_smem_log.h"
#include "timer.h"
#include "queue.h"
#include "procinfo_qdi.h"

/*****************************************************************************/
/*                         MACRO DECLARATIONS                                */
/*****************************************************************************/

/* Timeout value for pdtimeout timer, Unit in Msecs */
#define ERR_QDI_PDTIMEOUT_TIMER_PERIOD_DEFAULT  2000

#define ERR_QDI_CTXT_INVOKE_SIG 0x1

/*****************************************************************************/
/*                         DATA DECLARATIONS                                 */
/*****************************************************************************/
typedef struct err_qdi_opener_struct 
{
  /* Should be the first member */
  qurt_qdi_obj_t                 qdiobj;
  err_qdi_internal_s             user_pd_data;
  struct err_qdi_opener_struct   *next;
}err_qdi_opener_s;

typedef struct
{
  qurt_mutex_t     mutex;
  qurt_mutex_t     mutex_cb;  /* Mutex to serialize user PD callbacks */
  uint32           pd_callback_timeout_msec;
  uint32           pd_exit_timeout_msec;
  boolean          subsystem_pdr_enabled;
  err_qdi_opener_s *list;
  qurt_qdi_obj_t   opener_obj;
  q_type           queue;
  qurt_anysignal_t sig;
}err_qdi_s;

typedef struct
{
  /* q_link_type - Must be the first element */
  q_link_type        node;
  unsigned int       pid;
  unsigned int       tid;
  qurt_sysevent_error_t sys_err;
}err_qdi_q_data_s;

static err_qdi_s err_qdi_internal;


/* MACRO for locking/unlocking the qmutex*/
#define ERR_QDI_MUTEX_LOCK(x)  \
   qurt_pimutex_lock(x)
  
#define ERR_QDI_MUTEX_TRY_LOCK(x)  \
   qurt_pimutex_try_lock(x)

#define ERR_QDI_MUTEX_UNLOCK(x)  \
   qurt_pimutex_unlock(x)

#define ERR_QDI_MUTEX_INIT(x) \
   qurt_pimutex_init(x)

/*****************************************************************************/
/*                      FUNCTION DECLARATIONS                                */
/*****************************************************************************/
extern void *err_qdi_pdr_records_user_err_info(err_qdi_internal_s * dataobj);
extern void err_qdi_pdr_update_pd_exit_end_time
(
  void * temp_events,
  uint64 pd_exit_end_time
);
extern void err_qdi_pdr_records_init(void);
extern void err_qdi_dalcfg_get_pd_timeout
( 
  uint32 * pd_timeout_exit_msec 
);

int err_qdi_invoke(int client_handle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9);

extern int pd_mon_private_proxy_attach(int client_asid, qurt_qdi_obj_t *obj_ptr);
extern SERVREG_RESULT servreg_send_early_pd_down_ind(uint32 pid);

/*=============================================================================

FUNCTION ERR_QDI_PROCESS_PDTIMEOUT_TIMER_EXPIRY

DESCRIPTION
  Crashes the system in the event of user PD error callbacks taking more than 
  expected time or during PD exit failure

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None.

=============================================================================*/

void err_qdi_process_pdtimeout_timer_expiry( timer_cb_data_type data )
{
  err_qdi_internal_s * dataobj;
  err_qdi_opener_s * clntobj = (err_qdi_opener_s *)(data);
  
  if( clntobj == NULL )
  {
    ERR_FATAL("QDI layer Misbehaved, Shouldn't reach here ", 0,0,0);
    return ;
  }

  dataobj = &(clntobj->user_pd_data);
  
  /* CPU,DDR and VDD unvote for vote made in err_qdi_process_pd_err() */
  tms_utils_unvote_resources();
  
  /* This branch will not return */
  ERR_FATAL_AUX_MSG("USER PD crashed, PD_exit is stuck, check coredump.err.aux_msg",
      dataobj->pfr, ERR_QDI_PFR_LEN);	

} /* err_qdi_process_pdtimeout_timer_expiry */


/*=============================================================================

FUNCTION ERR_QDI_RELEASE

DESCRIPTION
 Releases the Qdi object that corresponding to a process that got destroyed

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None.

=============================================================================*/
void err_qdi_release(qurt_qdi_obj_t *obj)
{
  err_qdi_opener_s *temp, *clntobj = (err_qdi_opener_s *)obj;
  err_qdi_internal_s * dataobj;

  if( clntobj == NULL )
  {
    ERR_FATAL("QDI layer Misbehaved, Shouldn't reach here ", 0,0,0);
    return ;
  }

  ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex);

  /* CPU,DDR and VDD unvote for PD error handling end, 
     vote made in err_qdi_process_pd_err() */
  tms_utils_unvote_resources();

  dataobj = &(clntobj->user_pd_data);

  /* Clear the pdtimeout timer if set */
  if ( dataobj->pdtimeout_timer_set == TRUE )
  {
    (void)timer_clr_64( &(dataobj->pdtimeout_timer), T_MSEC );
    (void)timer_undef( &(dataobj->pdtimeout_timer) );
    dataobj->pdtimeout_timer_set = FALSE;
  }

  tms_get_timetick(&dataobj->end_time);
 
  /* Update PDR records with PD exit time */
  err_qdi_pdr_update_pd_exit_end_time( dataobj->pdr_record_data, 
                                       dataobj->end_time);

  if ( err_qdi_internal.list == NULL  )
  {
    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
    return;
  }

  if ( err_qdi_internal.list != NULL && err_qdi_internal.list == clntobj )
  {
    err_qdi_internal.list = clntobj->next;
    free(clntobj);
    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
    return;
  }
  
  for (temp = err_qdi_internal.list; NULL != temp; temp = temp->next)
  {
    if ( temp->next == clntobj )
    {
      temp->next = clntobj->next;
      free(clntobj);
      ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
      return;
    }
  }

  ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);

  /* Should not reach here */
  ERR_FATAL("Release Handle not found in the list", 0,0,0);

}/* err_qdi_release */


/*=============================================================================

FUNCTION ERR_QDI_COMMUNICATE_USER_CB

DESCRIPTION
 Communicates user callback addresses with root process err QDI layer.

DEPENDENCIES
  None

RETURN VALUE
 ERR_QDI_SUCCESS - for success
 ERR_QDI_FAIL    - for failure

SIDE EFFECTS
  None

=============================================================================*/
err_qdi_e err_qdi_communicate_user_cb
(
   err_qdi_opener_s   *clntobj,
   err_cb_bucket_t    bucket,
   void               *cb
)
{
  err_qdi_internal_s * dataobj;

  if( clntobj == NULL )
  {
    /* Deliberate */
    return ERR_QDI_NOT_INITIALIZED;
  }

  ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex);
  
  dataobj = &(clntobj->user_pd_data);
  
  /* State should be ERR_QDI_USERPD_INITIALIZED or ERR_QDI_USERPD_ERR_INITIATED */
  if(dataobj->state < ERR_QDI_USERPD_INITIALIZED)
  {
    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
    return ERR_QDI_NOT_INITIALIZED;
  }

  if( bucket < ERRCB_BUCKET_MAX )
  {
    dataobj->user_cb.user_cb[bucket].cb = cb;
  }
  
  ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
  
  return  ERR_QDI_SUCCESS;

} /* err_qdi_communicate_user_cb */

/*=============================================================================

FUNCTION ERR_QDI_COMMUNICATE_TID

DESCRIPTION
 Communicates user process tid to root incase preSTM callback is 
 ever registered in the user process

DEPENDENCIES
  None

RETURN VALUE
 ERR_QDI_SUCCESS - for success
 ERR_QDI_FAIL    - for failure

SIDE EFFECTS
  None

=============================================================================*/
err_qdi_e err_qdi_communicate_tid
(
   err_qdi_opener_s   *clntobj,
   unsigned int       tid,
   unsigned int       fault_pc, 
   unsigned int       sp, 
   unsigned int       badva, 
   unsigned int       cause,
   unsigned int       fp,
   unsigned int       lr,
   unsigned int       ssr
)
{
   err_qdi_internal_s * dataobj;

  if( clntobj == NULL )
  {
    /* Deliberate */
    return ERR_QDI_NOT_INITIALIZED;
  }

  ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex);
  
  dataobj = &(clntobj->user_pd_data);
  
  /* State should be ERR_QDI_USERPD_INITIALIZED or ERR_QDI_USERPD_ERR_INITIATED */
  if(dataobj->state < ERR_QDI_USERPD_INITIALIZED)
  {
    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
    return ERR_QDI_NOT_INITIALIZED;
  }

  dataobj->crash_tid = tid;
  dataobj->crash_fault_pc = fault_pc;
  dataobj->crash_sp    = sp;
  dataobj->crash_badva = badva, 
  dataobj->crash_cause = cause;
  dataobj->crash_fp = fp;
  dataobj->crash_lr = lr;
  dataobj->crash_ssr = ssr;

  ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);

  return  ERR_QDI_SUCCESS;

} /* err_qdi_communicate_tid*/


/*=============================================================================

FUNCTION ERR_QDI_OPEN

DESCRIPTION
 Creates err driver object for the client and initializes required details.

DEPENDENCIES
  None

RETURN VALUE
 QURT_EOK if the driver obj is defined and set correctly.

SIDE EFFECTS
  None

=============================================================================*/
static int err_qdi_open
(
   int                client_handle,
   void               *coredump,
   void               *err_fatal_params,
   void               *stack_addr,
   uint32             stack_size
)
{
   err_qdi_internal_s * dataobj;
   qurt_sysenv_procname_t pn={0};
   int rc = -1;
   err_qdi_opener_s *clntobj = calloc(1, sizeof(err_qdi_opener_s));
   
   if ( clntobj == NULL )
   {
     return -1;
   }

   /* QDI_OS_SYSENV is the standard method for requesting 
    * this type of information */
   /* QDI_HANDLE_LOCAL_CLIENT means that this request originated locally */
   /* QURT_SYSENV_PROCNAME_TYPE is the specific piece of data being requested 
    * -- to fill in a qurt_sysenv_procname_t structure */
   /* &pn is the structure we want filled in */

   if (0 > (rc = qurt_qdi_handle_invoke(client_handle, QDI_OS_SYSENV, 
                QDI_HANDLE_LOCAL_CLIENT, 
          QURT_SYSENV_PROCNAME_TYPE, &pn)) )
   {
      TMS_MSG_HIGH_2( "Cannot obtain client info %d %d", pn.asid, rc);
      free(clntobj);
      return rc;
   }

   ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex);
   
   clntobj->qdiobj.invoke    = err_qdi_invoke;
   clntobj->qdiobj.refcnt    = QDI_REFCNT_INIT;
   clntobj->qdiobj.release   = err_qdi_release;

   dataobj = &(clntobj->user_pd_data);
   
   /* Populate PD name and PID */
   strlcpy(dataobj->pd_name, pn.name, ERR_QDI_DEVICE_NAME_LEN);
   dataobj->pid = pn.asid;
   dataobj->state = ERR_QDI_USERPD_INITIALIZED;
   dataobj->pdr_enabled = TRUE;

   /* Insert at the head of the list */
   clntobj->next = err_qdi_internal.list;
   err_qdi_internal.list= clntobj;

   dataobj->client_handle = client_handle;

   timer_def_osal( &(dataobj->pdtimeout_timer), NULL, 
                   TIMER_FUNC1_CB_TYPE, 
          &err_qdi_process_pdtimeout_timer_expiry, (uint32)clntobj );

   dataobj->coredump = coredump;
   dataobj->err_fatal_params = err_fatal_params;
   dataobj->user_cb.stack_addr = (void*)(stack_addr);
   dataobj->user_cb.stack_size = stack_size;

   ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
   
   return qurt_qdi_handle_create_from_obj_t(client_handle, &clntobj->qdiobj);

} /* err_qdi_open */

/*=============================================================================

FUNCTION ERR_QDI_CHECK_PDR_ENABLED

DESCRIPTION
 Checks if given PD Restart is enabled for given subsystem for given ASID 
 by walking list in err_qdi internal structure
 
DEPENDENCIES
  None

RETURN VALUE
 True if PDR is enabled for both subsystem and PD

SIDE EFFECTS
  None

=============================================================================*/
boolean err_qdi_check_pdr_enabled(int asid)
{
  err_qdi_opener_s *temp;
  err_qdi_internal_s * dataobj;
  boolean ret = FALSE;
  
  if (!err_qdi_internal.subsystem_pdr_enabled || err_qdi_internal.list == NULL)
  {
    return ret;
  }

  /* Walk the list, find matching PID and return pdr_enabled for that PID */
  for (temp = err_qdi_internal.list; NULL != temp; temp = temp->next)
  {
    dataobj = &(temp->user_pd_data);

    if(dataobj->pid == asid)
    {
      ret = dataobj->pdr_enabled;
      break;
    }
  }
  
  return ret;
}


/*=============================================================================

FUNCTION ERR_QDI_INVOKE_USER_PROCESS_CB_HELPER

DESCRIPTION
  Callbacks belonging to user PDs are invoked

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
 None

=============================================================================*/
static void  err_qdi_invoke_user_process_cb_helper(err_qdi_internal_s * dataobj, err_cb_bucket_t bucket)
{
  /* Array boundary check*/
  if ( 0 > bucket || ERRCB_BUCKET_MAX <= bucket )
    return;

  /* Return early, if this callback was already attempted */
  if (dataobj->user_cb.user_cb[bucket].start_tick != 0 )
    return;

  /* Log start tick */
  tms_get_timetick(&(dataobj->user_cb.user_cb[bucket].start_tick));
  
  /* Clean cache to preserve the state of system before the callback */
  err_mem_cache_clean();

  if ( NULL == dataobj->user_cb.user_cb[bucket].cb || NULL == dataobj->user_cb.stack_addr || 0 == dataobj->user_cb.stack_size )
  {
    tms_get_timetick(&(dataobj->user_cb.user_cb[bucket].end_tick));
    return;
  }

  qurt_qdi_handle_invoke(dataobj->client_handle, QDI_CLIENT_HANDLE_CALLBACK,
                         dataobj->user_cb.user_cb[bucket].cb,
                         dataobj->user_cb.stack_addr,
                         dataobj->user_cb.stack_size);

  /* Log end tick */
  tms_get_timetick(&(dataobj->user_cb.user_cb[bucket].end_tick));

  /* Clean cache to preserve the state of system after the callback */
  err_mem_cache_clean();
}

/*=============================================================================

FUNCTION ERR_QDI_INVOKE_ALL_USER_PROCESS_CBS

DESCRIPTION
  Callbacks belonging to user PDs are invoked

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
 None

=============================================================================*/
void  err_qdi_invoke_all_user_process_cbs(err_cb_bucket_t bucket)
{
  err_qdi_opener_s *temp;
  err_qdi_internal_s * dataobj;
  
  err_mem_cache_clean();
  
  if ( err_qdi_internal.list == NULL  )
  {
     return;
  }

  for (temp = err_qdi_internal.list; NULL != temp; temp = temp->next)
  {
    dataobj = &(temp->user_pd_data);

    /* State should be ERR_QDI_USERPD_INITIALIZED or ERR_QDI_USERPD_ERR_INITIATED */
    if(dataobj->state < ERR_QDI_USERPD_INITIALIZED)
    {
      continue;
    }
    
    /* Execute callbacks only if global ERR QDI mutex can be acquired.
     * P.S. Thread spawned by QDI_CLIENT_HANDLE_CALLBACK is one time
     * global resource */
    if ( 0 == ERR_QDI_MUTEX_TRY_LOCK(&err_qdi_internal.mutex_cb) )
    {
      err_qdi_invoke_user_process_cb_helper(dataobj,bucket);
      ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex_cb);
    }
   
  }
}

/*=============================================================================

FUNCTION ERR_QDI_INVOKE_USER_PROCESS_CBS

DESCRIPTION
  Callbacks belonging to user PDs are invoked

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
 None

=============================================================================*/
void err_qdi_invoke_user_process_cbs(uint32 pid)
{
  err_qdi_opener_s *temp;
  err_qdi_internal_s * dataobj;
  
  err_mem_cache_clean();
  
  if ( err_qdi_internal.list == NULL  )
  {
     return;
  }

  for (temp = err_qdi_internal.list; NULL != temp; temp = temp->next)
  {
    dataobj = &(temp->user_pd_data);

    if ( dataobj->pid != pid )
    {
      continue;
    }

    /* State should be ERR_QDI_USERPD_INITIALIZED or ERR_QDI_USERPD_ERR_INITIATED */
    if(dataobj->state < ERR_QDI_USERPD_INITIALIZED)
    {
      continue;
    }
    
    /* Get the global ERR QDI mutex_cb before executing user process callbacks. This will serialize
     * user callbacks for user process. P.S. Thread spawned by QDI_CLIENT_HANDLE_CALLBACK is one 
     * time global resource */
    ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex_cb);

    err_qdi_invoke_user_process_cb_helper(dataobj,ERRCB_BUCKET_POST_STM);
    err_qdi_invoke_user_process_cb_helper(dataobj,ERRCB_BUCKET_NORMAL);
    err_qdi_invoke_user_process_cb_helper(dataobj,ERRCB_BUCKET_LATE);
    err_qdi_invoke_user_process_cb_helper(dataobj,ERRCB_BUCKET_LAST);

    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex_cb);

    return;
  }
}

/*=============================================================================

FUNCTION ERR_QDI_INVOKE_USER_PROCESS_CBS_PER_BUCKET

DESCRIPTION
  Specific bucket callbacks belonging to user PDs are invoked

DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
 None

=============================================================================*/
void err_qdi_invoke_user_process_cbs_per_bucket(err_cb_bucket_t bucket, uint32 pid)
{
  err_qdi_opener_s *temp;
  err_qdi_internal_s * dataobj;
  
  err_mem_cache_clean();
  
  if ( err_qdi_internal.list == NULL  )
  {
     return;
  }

  for (temp = err_qdi_internal.list; NULL != temp; temp = temp->next)
  {
    dataobj = &(temp->user_pd_data);

    if ( dataobj->pid != pid )
    {
      continue;
    }

    /* State should be ERR_QDI_USERPD_INITIALIZED or ERR_QDI_USERPD_ERR_INITIATED */
    if(dataobj->state < ERR_QDI_USERPD_INITIALIZED)
    {
      continue;
    }
    
    /* Get the global ERR QDI mutex_cb before executing user process callbacks. This will serialize
     * user callbacks for user process. P.S. Thread spawned by QDI_CLIENT_HANDLE_CALLBACK is one 
     * time global resource */
    ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex_cb);

    err_qdi_invoke_user_process_cb_helper(dataobj,bucket);

    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex_cb);

    return;
  }
}

/*=============================================================================

FUNCTION ERR_QDI_INVOKE

DESCRIPTION
 This function contains the switch which maps to all methods

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

=============================================================================*/
int err_qdi_invoke(int client_handle,
                          qurt_qdi_obj_t *pobj,
                          int method,
                          qurt_qdi_arg_t a1,
                          qurt_qdi_arg_t a2,
                          qurt_qdi_arg_t a3,
                          qurt_qdi_arg_t a4,
                          qurt_qdi_arg_t a5,
                          qurt_qdi_arg_t a6,
                          qurt_qdi_arg_t a7,
                          qurt_qdi_arg_t a8,
                          qurt_qdi_arg_t a9)
{
   /* Cannot be used for QDI_OPEN */
   err_qdi_opener_s* clntobj = (err_qdi_opener_s*)pobj;
   
   switch(method)
   {
      case QDI_OPEN: 
         return err_qdi_open(client_handle, a2.ptr, a3.ptr, a4.ptr, a5.num);
      
      case ERR_QDI_COMMUNICATE_USER_CB:
         return err_qdi_communicate_user_cb(clntobj, a1.num, a2.ptr );
      
      case ERR_QDI_COMMUNICATE_TID:
         return err_qdi_communicate_tid(clntobj, a1.num, a2.num, a3.num, a4.num, a5.num, a6.num, a7.num, a8.num);

      case QDI_CLOSE:
         //err_qdi_release(pobj);
         // No break;

      default:
         return qurt_qdi_method_default(client_handle, pobj, method,
                                     a1, a2, a3, a4, a5, a6, a7, a8, a9);
   }
} /* err_qdi_invoke */


/*=============================================================================

FUNCTION ERR_QDI_PROCESS_PD_ERROR

DESCRIPTION
 Processes PD Error

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void err_qdi_process_pd_err
(
  unsigned int pid,
  unsigned int tid,
  qurt_sysevent_error_t * sys_err
)
{
  err_qdi_opener_s *clntobj = NULL;
  err_qdi_internal_s * dataobj = NULL;
  char exception_msg[ERR_LOG_MAX_MSG_LEN];
  qurt_thread_attr_t attr;
  qurt_process_attr_t p_attr;

  ERR_QDI_MUTEX_LOCK(&err_qdi_internal.mutex);
  
  /* Check if the process has registered with the root 
   * -------------------------------------------------
   * If yes, Update Coredump ( Check if it is ERR_FATAL or NOT )
   * Check if PDR is enabled,
   * If yes, Do PD exit
   * If No, Do an ERR_FATAL in root
   * -------------------------------------------------
   * If No, Check if process is NON system critical or untrusted
   * if Yes, Do PD Exit
   * If No, Do an ERR_FATAL in root
   * -------------------------------------------------
   */

  qurt_process_attr_get(pid,&p_attr);

  /* This API will return QURT_EPRIVILEGE for untrusted PD and SecurePD */
  if(QURT_EOK != qurt_thread_attr_get(tid, &attr))
  {
    strlcpy(attr.name, "", QURT_THREAD_ATTR_NAME_MAXLEN);
  }

  if ( sizeof(exception_msg) <= tms_utils_fmt(exception_msg, ERR_LOG_MAX_MSG_LEN,
       "EX:%s:0x%x:%s:0x%x:PC=0x%x", p_attr.name, pid, attr.name, tid, sys_err->fault_pc))
  {
    /* Record failure to coredump */
    strlcpy(coredump.err.int_msg, "Failed to copy exception_msg", 
    sizeof("Failed to copy exception_msg"));
  }

  for (clntobj = err_qdi_internal.list; NULL != clntobj; clntobj = clntobj->next)
  {
    dataobj = &(clntobj->user_pd_data);
    if ( dataobj->pid == pid )
    {
      break;
    }
  }

  /* If not registered */
  if ( clntobj == NULL )
  {
    TMS_MSG_SPRINTF_HIGH_1( "User Process crashed: %s", exception_msg);
    TMS_MSG_HIGH_1( "flags = 0x%x", p_attr.flags);

    ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
    /* If PD is NON System Critical or untrusted then kill PD only */
    if (( p_attr.flags & QURT_PROCESS_NON_SYSTEM_CRITICAL ) || ( p_attr.flags & QURT_PROCESS_UNTRUSTED ))
    {
      /* Kill The PD */
      /* Need to pass any non-zero number, if QuRT should call PD dump callback so passing tid */
      qurt_qdi_handle_invoke(QDI_HANDLE_GENERIC, QDI_OS_PROCESS_KILL, pid, tid);
    }
    else /* If PD is  System Critical and trusted then crash Root PD  */
    {
      ERR_FATAL_AUX_MSG("System critical USER Process crashed, coredump.err.aux_msg has more details", 
                exception_msg, ERR_LOG_MAX_MSG_LEN);
    }
  }
  else //Registered
  {
    if(dataobj->state == ERR_QDI_USERPD_ERR_PD_EXIT_STARTED)
    {
      TMS_MSG_HIGH( "User Process exception handling is in progress already");
      ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);
      return ;
    }

    dataobj->state = ERR_QDI_USERPD_ERR_INITIATED;
    tms_get_timetick(&(dataobj->start_time));

    if ( dataobj->crash_tid != 0 )
    {
      tid = dataobj->crash_tid;
      sys_err->fault_pc = dataobj->crash_fault_pc;
      sys_err->sp = dataobj->crash_sp;
      sys_err->badva = dataobj->crash_badva;
      sys_err->cause = dataobj->crash_cause;
      sys_err->fp = dataobj->crash_fp;
      sys_err->lr = dataobj->crash_lr;
      sys_err->ssr = dataobj->crash_ssr;

      /* Get tcb name from tid */
      /* This API will return QURT_EPRIVILEGE for untrusted PD and SecurePD */
      if(QURT_EOK != qurt_thread_attr_get(tid, &attr))
      {
        strlcpy(attr.name, "", QURT_THREAD_ATTR_NAME_MAXLEN);
      }

      memset(dataobj->pfr, 0, ERR_LOG_MAX_FILE_LEN);

      if ( sizeof(exception_msg) <= tms_utils_fmt(exception_msg, ERR_LOG_MAX_MSG_LEN,
       "EX:%s:0x%x:%s:0x%x:PC=0x%x", p_attr.name, pid, attr.name, tid, sys_err->fault_pc))
      {
        /* Record failure to coredump */
        strlcpy(coredump.err.int_msg, "Failed to copy exception_msg", 
        sizeof("Failed to copy exception_msg"));
      }
    }

    strlcpy(dataobj->pfr, exception_msg, ERR_LOG_MAX_MSG_LEN);

    /* Check */
    if ( dataobj->coredump != NULL && dataobj->err_fatal_params != NULL )
    {
      qurt_qdi_copy_to_user( dataobj->client_handle, dataobj->coredump->err.tcb_name, attr.name, QURT_THREAD_ATTR_NAME_MAXLEN);
      qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->err.tid, &tid, (sizeof(unsigned int)));
  
      /* IF ERR_FATAL */
      if ( dataobj->err_fatal_params->msg_const_ptr != NULL )
      {
          int written;

          memset(dataobj->pfr, 0, ERR_LOG_MAX_FILE_LEN);

          /* Copy Filename from user Process */
          qurt_qdi_copy_from_user( dataobj->client_handle, exception_msg, dataobj->err_fatal_params->msg_const_ptr->fname, ERR_LOG_MAX_FILE_LEN);

          written = tms_utils_fmt( dataobj->pfr, ERR_LOG_MAX_MSG_LEN,
                                   "EF:%s:0x%x:%s:0x%x:%s:%d:", 
                                   p_attr.name, pid, attr.name, tid, 
                                   exception_msg, 
                                   dataobj->err_fatal_params->msg_const_ptr->line );

          if (written > 0 && written < ERR_LOG_MAX_MSG_LEN )
          {
             /* First Copy ERR_FATAL string from aux_msg */
            qurt_qdi_copy_from_user( dataobj->client_handle, exception_msg, dataobj->err_fatal_params->aux_msg, ERR_LOG_MAX_MSG_LEN);

            if(exception_msg[0] == 0)
            {
               /* Since ERR_FATAL string from aux_msg is NULL , Copy it from normal msg */
               qurt_qdi_copy_from_user( dataobj->client_handle, exception_msg, dataobj->err_fatal_params->msg_const_ptr->fmt, ERR_LOG_MAX_MSG_LEN);
            }

            /* Append err fatal message */
            /*-1 since tms_utils_fmt() API returns written length with NULL
             * character increment and overwrite previous null-term*/
     
            written += tms_utils_fmt_nostring(dataobj->pfr + written - 1,
                         (ERR_LOG_MAX_MSG_LEN- (written-1)),
                         exception_msg,
                         dataobj->err_fatal_params->param1,
                         dataobj->err_fatal_params->param2,
                         dataobj->err_fatal_params->param3);
          }

          /* Update Coredump */
          qurt_qdi_copy_from_user( dataobj->client_handle, dataobj->coredump->err.filename, dataobj->err_fatal_params->msg_const_ptr->fname, ERR_LOG_MAX_FILE_LEN);
          qurt_qdi_copy_from_user( dataobj->client_handle, dataobj->coredump->err.message, dataobj->err_fatal_params->msg_const_ptr->fmt, ERR_LOG_MAX_MSG_LEN);
          qurt_qdi_copy_from_user( dataobj->client_handle, dataobj->coredump->err.aux_msg, dataobj->err_fatal_params->aux_msg, ERR_LOG_MAX_MSG_LEN);
          qurt_qdi_copy_from_user( dataobj->client_handle, &dataobj->coredump->err.linenum, &dataobj->err_fatal_params->msg_const_ptr->line, sizeof(uint16));
          qurt_qdi_copy_from_user( dataobj->client_handle, &dataobj->coredump->err.param[0], &dataobj->err_fatal_params->param1, sizeof(uint32));
          qurt_qdi_copy_from_user( dataobj->client_handle, &dataobj->coredump->err.param[1], &dataobj->err_fatal_params->param2, sizeof(uint32));
          qurt_qdi_copy_from_user( dataobj->client_handle, &dataobj->coredump->err.param[2], &dataobj->err_fatal_params->param3, sizeof(uint32));
          qurt_qdi_copy_from_user( dataobj->client_handle, dataobj->coredump->arch.regs.array, dataobj->err_fatal_params->array, SIZEOF_ARCH_COREDUMP_REGISTERS *sizeof(uint32));
       }
       else
       {
          /* Update coredump for exceptions */
          qurt_qdi_copy_to_user( dataobj->client_handle, dataobj->coredump->err.message,  dataobj->pfr, ERR_LOG_MAX_FILE_LEN);
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.pc, &sys_err->fault_pc, sizeof(unsigned int));
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.sp, &sys_err->sp, sizeof(unsigned int));
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.badva, &sys_err->badva, sizeof(unsigned int));
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.ssr, &sys_err->ssr, sizeof(unsigned int));
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.fp, &sys_err->fp, sizeof(unsigned int));
          qurt_qdi_copy_to_user( dataobj->client_handle, &dataobj->coredump->arch.regs.name.lr, &sys_err->lr, sizeof(unsigned int));
       }
    }

    TMS_MSG_SPRINTF_HIGH_1( "User Process crashed: %s", dataobj->pfr);
  
    /* Send PD failure reason to APSS*/
    servreg_loc_send_pfr(p_attr.name, strlen(p_attr.name), dataobj->pfr, strlen( dataobj->pfr));

    /* PDR Enabled */
    if ( ( err_qdi_internal.subsystem_pdr_enabled && dataobj->pdr_enabled ) )
    {
      /* Set the PDExit Timeout timer */
      if ( err_qdi_internal.pd_exit_timeout_msec != 0 )
      {
        /* Set the timer to cover timeout cases for PD exit pdtimeouts */
        timer_set_64( &(dataobj->pdtimeout_timer),
        err_qdi_internal.pd_exit_timeout_msec, 0, T_MSEC );
        dataobj->pdtimeout_timer_set = TRUE;
      }

      /* Adding late process exit notification to clear the pdtimeout timer. */
      if ( -1 == pd_mon_private_proxy_attach((int)dataobj->pid, &clntobj->qdiobj))
      {
        ERR_FATAL("Registration of Late Notification for PD exit timeout timer failed",
                   0, 0, 0);
      }

      /* Record PDR info */
      dataobj->pdr_record_data = err_qdi_pdr_records_user_err_info(dataobj);

      /* Vote for CPU,DDR and VDD during PD error handling start. This will unvote
      in err_qdi_release() or in case PD error handling stuck:
      unvote in err_qdi_process_pdtimeout_timer_expiry(). Cannot be used for PDs not registered 
      with Err QDI*/
      tms_utils_vote_resources();

      dataobj->state = ERR_QDI_USERPD_ERR_PD_EXIT_STARTED;
      ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);

      /* To provide early indication of PD down*/
      (void)servreg_send_early_pd_down_ind(dataobj->pid);

      /* Kill The PD */
      /* Need to pass any non-zero number, if QuRT should call PD dump callback so passing tid */
      qurt_qdi_handle_invoke(QDI_HANDLE_GENERIC, QDI_OS_PROCESS_KILL, pid, tid);
    }
    else /* PDR Disabled */
    {
      dataobj->state = ERR_QDI_USERPD_ERR_HANDLING_COMPLETED;
      ERR_QDI_MUTEX_UNLOCK(&err_qdi_internal.mutex);

      /* Log Crash into Smem and don't allow for overwrites, Note : It is only done if PDR is disabled */
      err_smem_log_crash(tid, dataobj->pfr, sys_err, TRUE, 0, 0);

      ERR_FATAL_AUX_MSG("PDR Disabled, USER Process crashed, coredump.err.aux_msg has more details", 
                         dataobj->pfr, ERR_LOG_MAX_MSG_LEN);
    }
  }

} /* err_qdi_process_pd_err */

/**
Function corresponding err_qdi_internal_ctxt task. It is used to invoke the callbacks
@return
None
*/

void err_qdi_internal_ctxt(void * ignore)
{
  unsigned int    sigs;
  err_qdi_q_data_s * data;
  
  for(;;)
  {
    /* Wait for some task signal */
    sigs = qurt_anysignal_wait( &err_qdi_internal.sig,
                     ERR_QDI_CTXT_INVOKE_SIG);
    
    if ( sigs & ERR_QDI_CTXT_INVOKE_SIG )
    {
      qurt_anysignal_clear( &err_qdi_internal.sig, ERR_QDI_CTXT_INVOKE_SIG );

      /* Process the callback */
      /* Get from the queue */
      data = q_get(&(err_qdi_internal.queue) );

      while ( data != NULL )
      {
        err_qdi_process_pd_err( data->pid, data->tid, &data->sys_err );
        free(data);
        data = q_get(&(err_qdi_internal.queue) );
      }
    }
  }
}

void err_qdi_process_exception_in_separate_context
(
  unsigned int pid,
  unsigned int tid,
  qurt_sysevent_error_t * sys_err 
)
{
  err_qdi_q_data_s * data = calloc(1, sizeof(err_qdi_q_data_s));

  if ( data == NULL )
  {
    return;
  }
  
  data->pid = pid;
  data->tid = tid;
  memsmove(&data->sys_err, sizeof(qurt_sysevent_error_t), sys_err, sizeof(qurt_sysevent_error_t)) ;
    
  /* Insert in the queue */
  (void) q_link( data , &(data->node) );
  q_put(&(err_qdi_internal.queue), 
        &(data->node) );

  (void)qurt_anysignal_set(&err_qdi_internal.sig, ERR_QDI_CTXT_INVOKE_SIG); 
}
/*=============================================================================

FUNCTION ERR_QDI_INIT

DESCRIPTION
 Registers with QDI Framework for ERR QDI
 
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

=============================================================================*/

void err_qdi_init 
(
  void
)
{
   /* Initialize the mutex here */
   ERR_QDI_MUTEX_INIT(&err_qdi_internal.mutex);
   ERR_QDI_MUTEX_INIT(&err_qdi_internal.mutex_cb);

   err_qdi_internal.opener_obj.invoke    = err_qdi_invoke;
   err_qdi_internal.opener_obj.refcnt    = QDI_REFCNT_INIT;
   err_qdi_internal.opener_obj.release   = err_qdi_release;
   
   qurt_qdi_register_devname(ERR_DRIVER_NAME, &err_qdi_internal.opener_obj);

   q_init(&err_qdi_internal.queue);
   qurt_anysignal_init(&err_qdi_internal.sig);

   (void)tms_utils_launch_qurt_task("err_qdi_internal", TMS_TASK_STACK_4K, TMS_TASK_PRIO_EXCEPTION + 20,  err_qdi_internal_ctxt, NULL, TRUE); 

} /* err_qdi_init */

void err_qdi_late_init 
(
  void
)
{

   /* Set Subsystem PDR policy */
   pd_mon_pd_restart_config( &err_qdi_internal.subsystem_pdr_enabled );
   
   err_qdi_internal.pd_callback_timeout_msec = ERR_QDI_PDTIMEOUT_TIMER_PERIOD_DEFAULT;
   err_qdi_internal.pd_exit_timeout_msec = ERR_QDI_PDTIMEOUT_TIMER_PERIOD_DEFAULT;
	   
   err_qdi_dalcfg_get_pd_timeout( &err_qdi_internal.pd_exit_timeout_msec );

   err_qdi_pdr_records_init();

} /* err_qdi_late_init */
