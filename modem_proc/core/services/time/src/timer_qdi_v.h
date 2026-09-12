/*=============================================================================

                Timer_Qdi_v.h  --  Header File

GENERAL DESCRIPTION
      Timer QDI Layer Header File

EXTERNAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2009 - 2018
      by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.


$Header: //components/rel/core.mpss/10.0/services/time/src/timer_qdi_v.h#1 $ 
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
03/28/17   abh     Added changes to use queue instead of pipes.
05/20/14   abh     Added changes to support MultiPD on MPSS
06/20/13   ab      Added header file
=============================================================================*/
#ifndef TIMER_QDI_V_H
#define TIMER_QDI_V_H

#include "qurt_qdi.h"
#include "qurt_qdi_driver.h"
#include "queue.h"
#include "timer.h"

#define TIMER_ERR_NONE                       0x0
#define TIMER_ERR_QDI_CPY_FAIL               0x1
#define TIMER_ERR_CANCEL_WAIT                0x2

#define TIMER_DRIVER_NAME              "/dev/timers"

typedef enum timer_qdi_methods
{
TIMER_QDI_GROUP_SET_DEFERRABLE = (0+QDI_PRIVATE),
TIMER_QDI_GROUP_DELETE,

TIMER_QDI_GROUP_ENABLE,
TIMER_QDI_GROUP_DISABLE,

TIMER_QDI_GET_TIME_TILL_FIRST_NON_DEFERRABLE_TIMER,
TIMER_QDI_GET_FIRST_NON_DEFERRABLE_TIMER_EXPIRY,

TIMER_QDI_DEFER_MATCH_INTERRUPT,
TIMER_QDI_DEFER_MATCH_INTERRUPT_64,
TIMER_QDI_UNDEFER_MATCH_INTERRUPT,

TIMER_QDI_DEF,
TIMER_QDI_DEF2,
TIMER_QDI_DEF_OSAL,
TIMER_QDI_DEF_PRIORITY,
TIMER_QDI_DEF2_PRIORITY,
TIMER_QDI_DEF_OSAL_PRIORITY,
TIMER_QDI_REG,
TIMER_QDI_SET,
TIMER_QDI_SET_64,
TIMER_QDI_SET_ABSOLUTE,
TIMER_QDI_GET,
TIMER_QDI_GET_64,
TIMER_QDI_GET_PARAMS,
TIMER_QDI_SET_PARAMS,
TIMER_QDI_IS_ACTIVE,
TIMER_QDI_EXPIRES_AT_64,
TIMER_QDI_EXPIRES_AT,
TIMER_QDI_GET_START,
TIMER_QDI_GET_START_64,
TIMER_QDI_CLR,
TIMER_QDI_CLR_64,
TIMER_QDI_UNDEF,
TIMER_QDI_PAUSE,
TIMER_QDI_RESUME,
TIMER_QDI_CLR_TASK_TIMERS,
TIMER_QDI_CLR_OSAL_THREAD_TIMERS,
TIMER_QDI_GET_SCLK_TILL_EXPIRY,
TIMER_QDI_GET_SCLK_TILL_EXPIRY_64,
TIMER_QDI_SET_WAKEUP,
TIMER_QDI_SET_WAKEUP_64,
TIMER_QDI_RESTART,
TIMER_QDI_STOP,
TIMER_QDI_GET_EXPIRY_TYPE,
TIMER_QDI_GET_GROUP,
TIMER_QDI_IS_PERIODIC,
TIMER_QDI_REGISTER_FOR_NON_DEFER_EXPIRY_CHANGE,

/* Call back method */
TIMER_QDI_GET_CB_INFO,
TIMER_QDI_MARK_INTERDEPENDENCY_BY_TID,
TIMER_QDI_MARK_INTERDEPENDENCY_BY_NAME,

} timer_qdi_method_type;

#define TIMER_QDI_ATS_LAST_METHOD TIMER_QDI_GET_CB_INFO

/* Signal mask for signalling data enque status */
#define TIMER_REMOTE_EXPIRY_ENQUEUED  0x00000001

typedef struct timer_qdi_opener_type {
  qurt_qdi_obj_t  qdiobj;
  uint32          process_idx;
  qurt_pipe_t    *pipe;
  q_type          timer_qdi_q;
  qurt_anysignal_t timer_qdi_sig;
}timer_qdi_opener;

typedef struct timer_cb_info_type {
   timer_ptr_type timer;
   uint32         cb_type;
   uint32         sig_addr;
   uint32         mask_data2;
   uint32         func_addr;
   uint32         data; 
}timer_cb_info;

typedef struct timer_qdi_queue_s {
  q_link_type                        link;      /* Queue field */
  timer_cb_info                      cb_info;   /* CB Info */
}timer_qdi_queue_type;


/*==============================================================================

                        TIMER GROUP INTERNAL INLINE MACROS

=============================================================================*/
static inline int timer_invoke_group_delete(int handle, timer_group_ptr group)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GROUP_DELETE, group);
}

static inline int timer_invoke_group_set_deferrable(int handle, timer_group_ptr group, boolean condition)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GROUP_SET_DEFERRABLE, group, condition);
}

static inline int timer_invoke_group_enable(int handle, timer_group_ptr group)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GROUP_ENABLE, group);
}

static inline int timer_invoke_group_disable(int handle, timer_group_ptr group)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GROUP_DISABLE, group);
}

/*==============================================================================

                            DEFER-UNDEFER INTERNAL INLINE MACROS

=============================================================================*/

static inline int timer_invoke_get_first_non_deferrable_timer_expiry(
                                 int                        handle,
                                 time_timetick_type         *tick,
                                 timer_non_defer_data_ptr   timer_data_ptr)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_FIRST_NON_DEFERRABLE_TIMER_EXPIRY, tick, timer_data_ptr);
}

static inline int timer_invoke_defer_match_interrupt_64(int handle, time_timetick_type *tick)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_DEFER_MATCH_INTERRUPT_64, tick);
}

static inline int timer_invoke_undefer_match_interrupt(int handle)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_UNDEFER_MATCH_INTERRUPT);
}

/*==============================================================================

                            TIMER INTERNAL INLINE MACROS

=============================================================================*/
static inline timer_error_type timer_invoke_def_priority(
                                   int                             handle, 
                                   timer_ptr_type                  timer,
                                   timer_group_ptr                 group, 
                                   rex_tcb_type                    *tcb,
                                   rex_sigs_type                   sigs,
                                   timer_t1_cb_type                func,
                                   timer_cb_data_type              data,
                                   timer_priority_enum_type        timer_prio)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_DEF_PRIORITY, timer, group, tcb, sigs, func, data, timer_prio);
}

static inline timer_error_type timer_invoke_def_osal_priority(
                                   int                          handle, 
                                   timer_ptr_type				timer,
                                   timer_group_ptr 				group,
                                   timer_notify_type			notify_type,
                                   time_osal_notify_obj_ptr		sigs_func_addr,
                                   time_osal_notify_data		sigs_mask_data,
                                   timer_priority_enum_type		timer_prio)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_DEF_OSAL_PRIORITY, timer, group, notify_type, sigs_func_addr, sigs_mask_data, timer_prio);
}

static inline timer_error_type timer_invoke_def2_priority(
                                   int                            handle, 
                                   timer_ptr_type                  timer,   
                                   timer_group_ptr                 group,
                                   timer_priority_enum_type        timer_prio)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_DEF2_PRIORITY, timer, group, timer_prio);
}

static inline timer_error_type timer_invoke_reg(
                                   int                             handle,
                                   timer_ptr_type                  timer,   
                                   timer_t2_cb_type                func,
                                   timer_cb_data_type              data,
                                   timetick_type                   time_ms,
                                   timetick_type                   reload_ms)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_REG, timer, func, data, time_ms, reload_ms);
}

static inline timer_error_type timer_invoke_set_64(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   time_timetick_type              time,
                                   time_timetick_type              reload,
                                   timer_unit_type                 unit)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_SET_64, timer, time>>32, time, reload>>32, reload, unit);
}

static inline timer_error_type timer_invoke_set_absolute(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   time_timetick_type              time)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_SET_ABSOLUTE, timer, time>>32, time);
}

static inline int timer_invoke_get_64(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_unit_type                 unit,
                                   time_timetick_type             *tick)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_64, timer, unit, tick);
}

static inline int timer_invoke_get_params(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_attrib_struct_type *      timer_attrib_struct_ptr)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_PARAMS, timer, timer_attrib_struct_ptr);
}

static inline int timer_invoke_set_params(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_attrib_set_type           to_set,
                                   timer_attrib_struct_type *      timer_attrib_struct_ptr)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_SET_PARAMS, timer, to_set, timer_attrib_struct_ptr);
}

static inline int timer_invoke_is_active(
                                   int                             handle,
                                   timer_ptr_type                  timer)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_IS_ACTIVE, timer);
}

static inline int timer_invoke_is_periodic(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   boolean                         *is_periodic,
                                   time_timetick_type              *period)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_IS_PERIODIC, timer, is_periodic, period);
}

static inline int timer_invoke_expires_at_64(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   time_timetick_type              *ticks)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_EXPIRES_AT_64, timer, ticks);
}

static inline int timer_invoke_get_start_64(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   time_timetick_type             *tick)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_START_64, timer, tick);
}

static inline int timer_invoke_get_group(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_group_type                *group)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_GROUP, timer, group);
}

static inline int timer_invoke_get_expiry_type(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_expiry_type               *expiry_type)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_EXPIRY_TYPE, timer, expiry_type);
}

static inline int timer_invoke_stop(
                                   int                             handle,
                                   timer_ptr_type                  timer,
                                   timer_unit_type                 unit,
                                   time_timetick_type              *rem_time)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_STOP, timer, unit, rem_time);
}

static inline timer_error_type timer_invoke_undef(
                                   int                             handle,
                                   timer_ptr_type                  timer)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_UNDEF, timer);
}

static inline timer_error_type timer_invoke_pause(
                                   int                             handle,
                                   timer_ptr_type                  timer)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_PAUSE, timer);
}

static inline timer_error_type timer_invoke_resume(
                                   int                             handle,
                                   timer_ptr_type                  timer)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_RESUME, timer);
}

static inline timer_error_type timer_invoke_clr_osal_thread_timers(
                                   int                             handle,
                                   time_osal_thread_id               thread_id)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_CLR_OSAL_THREAD_TIMERS, thread_id);
}

static inline timer_error_type timer_invoke_clr_task_timers(
                                   int                             handle,
                                   rex_tcb_type                    *tcb)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_CLR_TASK_TIMERS, tcb);
}

static inline int timer_invoke_get_sclk_till_expiry_64(
                                   int                             handle,
                                   time_timetick_type             *tick)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_SCLK_TILL_EXPIRY_64, tick);
}

static inline int timer_invoke_set_wakeup_64(
                                   int                             handle,
                                   time_timetick_type              wakeup_sclks)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_SET_WAKEUP_64, wakeup_sclks>>32, wakeup_sclks);
}

static inline int timer_invoke_restart(int                             handle)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_RESTART);
}

static inline int timer_invoke_register_for_non_defer_expiry_change(
	                                   int							handle,
	                                   timer_notify_type			notify_type,
                                       time_osal_notify_obj_ptr 	notify_obj,
                                       time_osal_notify_data 		notify_mask)
{
   return qurt_qdi_handle_invoke(handle, 
   	                             TIMER_QDI_REGISTER_FOR_NON_DEFER_EXPIRY_CHANGE,
   	                             notify_type,
   	                             notify_obj,
   	                             notify_mask);
}

static inline int timer_invoke_get_cb_info(int handle, timer_cb_info *cb_details)
{
   return qurt_qdi_handle_invoke(handle, TIMER_QDI_GET_CB_INFO, cb_details);
}

static inline int timer_invoke_mark_interdependence_by_tid (
												int              handle,
												qurt_thread_t    tid1,
												qurt_thread_t    tid2)
{
	return qurt_qdi_handle_invoke(handle, TIMER_QDI_MARK_INTERDEPENDENCY_BY_TID, tid1, tid2);
}

static inline int timer_invoke_mark_interdependence_by_name (
												int              handle,
												char*            thread1_name,
												int              name_len1,
												char*            thread2_name,
												int              name_len2)
{
	return qurt_qdi_handle_invoke(handle, TIMER_QDI_MARK_INTERDEPENDENCY_BY_NAME, thread1_name, name_len1, thread2_name, name_len2);
}
#endif /* TIMER_QDI_V_H */
