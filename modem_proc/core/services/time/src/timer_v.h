#ifndef TIMER_V_H
#define TIMER_V_H
/*=============================================================================

 T I M E R  S E R V I C E  I N T E R N A L

GENERAL DESCRIPTION
 Provides internal structures and function used by timer. 

EXTERNALIZED FUNCTIONS

<WRITE THIS PART>

Copyright (c) 2009 - 2017 by QUALCOMM Technologies Incorporated.  All Rights Reserved.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.



=============================================================================*/



/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include "timer.h"
#include "queue.h"
#include "tms_rcinit.h"

#define TIMER_FREQ_VALUE          19200000
#define TIMER_BIN_RANGE           TIMER_FREQ_VALUE/4
/* This range is a result of the following calculation...
In general we can say that the oscillator has a +/- 100% swing in freq
Therefore lower bound on Latency(Ticks/ms) = 2*f/1000 
If 5ms is max then TIMER_SCLK_LATENCY = (2*f/1000)*5 = f/100 
*/
#define TIMER_SCLK_LATENCY        TIMER_FREQ_VALUE/100

#define TIMER_BIN_REMOVED

/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/

#define TIMER_FROM_REMOTE_PROCESS 3

/** @brief Pointer to the timer client structure. */
typedef struct timer_client_struct *timer_client_ptr_type;
/** @brief Pointer to the internal group structure. */
typedef struct timer_group_s *timer_group_t_ptr;
/** @brief Pointer to the timer list structure. */
typedef struct timer_struct_internal *timer_ptr_internal_type;

/** Pointer to timer list structure */
typedef struct timer_list_struct  *timer_list_ptr;
/** Pointer to timer bin structure */
typedef struct timer_bin_struct   *timer_bin_ptr;

/* Remote Process Callback Handler type */
typedef void (*timer_process_cb_handler)
(
  /* Timer client pointer */
  timer_client_ptr_type			clnt_timer,

  /* run time in msecs */  
  time_timetick_type			run_time_ms
);

/*=============================================================================

                           ENUMERATIONS

=============================================================================*/

/** Timer Queue flags with magic number */
typedef enum timer_internal_queue
{
  NODE_IS_FREE = 0x3,
  NODE_IS_NOT_FREE = 0xC 
} timer_internal_q;

/* Enumerated type for Parameters */
typedef enum
{
  TIMER_IS_ACTIVE,
  TIMER_IS_PERIODIC,
  TIMER_EXPIRES_AT_PARAM,
  TIMER_GET_START_64,
  TIMER_GET_GROUP,
  TIMER_GET_EXPIRY_TYPE,
  TIMER_MAX_PARAMS
} timer_param_type;

/* PC mask index */
typedef enum timer_pc_mask_index_e
{
  TIMER_PC_MASK_INDEX = 0x0,
  TIMER_SLAVE1_PC_MASK_INDEX,
  TIMER_SLAVE2_PC_MASK_INDEX,
  TIMER_SLAVE3_PC_MASK_INDEX,
} timer_pc_mask_index_t;

/*=============================================================================

                           STRUCTURES

=============================================================================*/
/** @brief Timer list structure type. Values in this structure are for private 
use by timer.c only. */
typedef struct timer_list_struct
{
  timer_ptr_internal_type         first; /**< List of timers. */

  timer_ptr_internal_type         last;  /**< End of the timer list. */
}
timer_list_type;

/** @brief Timer group structure type. Values in this structure are for private 
use by timer.c only. */
typedef struct timer_group_s
{
  timer_group_ptr                 ext_self;   /**< Points to external timer group 
                                                   user variable */
  timer_list_type                 list;       /**< Timers in the disabled timer 
                                                   group. */
  boolean                         disabled;   /**< Disabled flag for the timer 
                                                   group. */
  boolean                         deferrable; /**< Deferrable flag for the 
                                                   timer group. Default = FALSE. */
  timer_group_t_ptr               next;       /**< Link to the next timer group. */

  uint8                           process_idx;/**< Process index relating this group 
                                                                                 to the associated PD */

  uint8                           group_idx;  /**< Group index to the internal groups */

  uint16                          timer_cnt;  /**< Timers cnt belongs to this group */


} timer_group_t;

/** Timer cache structure type */
typedef struct
{
  /** Original set duration */
  time_timetick_type                   duration;

  /** Converted set duration */
  time_timetick_type                   duration_sclk;

  /** Original reload increment */
  time_timetick_type                   reload;

  /** Converted reload increment */
  time_timetick_type                   reload_sclk;

  /** Time units */
  timer_unit_type                      unit;
}
timer_cache_type;

/* Try to keep less than 32 bits in total */
typedef struct timer_properties_struct
{
  /* Nibble (4bits) wise: 7th: None or free 6th-5th: Group Idx, 4th: slave task idx, 3rd: Timer state,
     2nd: process idx, 1st-3rd bit: remote_process 1st: 2-1 bits: cb type, 1st 0th bit: expiry_type
     0th: Node state, being used or not */
  unsigned int 					group_idx:8;		/* 8 bits for group index: Limits to 0-255 groups */
  unsigned int 					slave_task_idx:4;	/* 4 bits for slave task num: 0-15 */
  unsigned int			 		timer_state:4;		/* 4 bits for timer_state: 0-15 */ /* Currently not being used, later if required can use */
  unsigned int 					process_idx:4;		/* 4 bits for process index: 0-15 */
  unsigned int 					remote_process:2;	/* 2 bits: Indicates whether timer is from remote process */
  unsigned int 					callback_type:4;	/* 4 bits*/ /* values None: 0, Sigs:1, func1:2, func2:3 */
  unsigned int 					expiry_type:1;		/* 1 bit: Relative(0)/Absolute(1) */
  unsigned int 					timer_priority:1;	/* 1 bit : Values is either '0' (PRIORITY_0) or '1' (PRIORITY_1) only */
  unsigned int 					node_state:4;		/* 4 bits: Values are NODE_IS_FREE(0x3), NODE_IS_NOT_FREE(0xC) only */
}timer_properties_type;


/** @brief Timer Client Structure type. Contains client details like
     Group, Signals, callback functions, process details etc
     Values in this structure are for private use by timer.c only.*/
typedef struct timer_client_struct
{
  timer_state_enum_type           timer_state; 
  
  time_timetick_type              timer_state_at; 

  /**< Address of client timer */
  timer_ptr_type                  timer_client_ptr;

  /**< Handle to the internal timer. */
  int32                           timer_int_handle;

  /** Thread ID of the client defining the timer */
  time_osal_thread_id			  thread_id;

  /* TCB pointer for signalling */
  rex_tcb_type 				     *tcb;

  /**< Signal(s) to set in the task when the timer expires. */
  rex_sigs_type			          sigs;	

   union
   {
      /**< Asynchronous Procedure Call (APC) function to call when the timer 
         expires. */
      timer_t1_cb_type                func1; 
      /**< Clock callback style function to call when the timer expires. */
      timer_t2_cb_type                func2; 
  }cbfn;

  /**< Data for the APC function when the timer expires. */
  timer_cb_data_type			  data;

  timer_properties_type              info;

  /* Callstack info for the timer_define call while this timer client structure is getting filled */
  void *                             callstack_info[4];
}timer_client_type;



/** Internal Timer struture referenced by external timer structure through handle */
typedef struct timer_struct_internal
{
  /* Link for the next queue */
  q_link_type                     link;
  
  /* Index of timer_buffer array */
  int32                           index;
  
  /* Backward pointer to client timer */
  timer_client_ptr_type           timer_ext;
  
  /** The list the timer is on.  "list" will contain:
       "&active_timers" if the timer is active;
       "&timer->group->list" if the timer is active, but in a disabled group;
       "NULL" if the timer is not active. */
  timer_list_ptr                  list;

  /** Slow clock tick count timer expires at (list != NULL),
     or remaining ticks until expiry if timer is paused (list == NULL) */
  time_timetick_type              expiry;

  /** Reload offset when timer expires (0 = no reload at expiry) */
  time_timetick_type              reload;

  /** Slow clock tick count value when timer was set (started) */
  time_timetick_type              start;

  /** Durations and reload times, original units and converted to sclks */
  timer_cache_type                cache;

  /** Pointer to the next timer in the list (list != NULL) */
  timer_ptr_internal_type         next;

  /** Pointer to the previous timer in the list (list != NULL) */
  timer_ptr_internal_type         prev;

  /** Pointer to the bin for this timer */
  timer_bin_ptr                   bin;

  timer_properties_type           info;

  /** Thread ID of the client using the timer */
  time_osal_thread_id			  thread_id;
  
}timer_internal_type;


/*-----------------------------------------------------------------------------
  Timer Bin structure type
    Represents a single bin used for fast insertion into the timer list
-----------------------------------------------------------------------------*/
#ifdef TIMER_BIN_REMOVED
typedef struct timer_bin_struct
{
  /* First timer in the bin */
  timer_ptr_internal_type         first;

  /* Last timer in the bin */
  timer_ptr_internal_type         last;

  /* Earliest expiry time in the bin */
  time_timetick_type                   lower;

  /* Latest expiry time in the bin */
  time_timetick_type                   upper;

  /* Number of timers in the bin */
  int                             count;

  /* Next bin pointer */
  timer_bin_ptr                   next_bin;

  /* Previous bin pointer */
  timer_bin_ptr                   prev_bin;
}
timer_bin_type;


/*-----------------------------------------------------------------------------
  Timer bin list type
-----------------------------------------------------------------------------*/

typedef struct timer_bin_list_struct
{
  /* First timer bin in list */
  timer_bin_ptr                   first;

  /* Last timer bin in list */
  timer_bin_ptr                   last;

  /* Number of timer bins */
  int                             count; 
}
timer_bin_list_type, *timer_bin_list_ptr;
#endif

/*-----------------------------------------------------------------------------
  Private timer data ...
-----------------------------------------------------------------------------*/

typedef struct
{
  /* Faux timer group, used to hold list of active timers, as well as a list
     of timer groups.
     Active timers will never "belong" to this group, ie timer->group will
     never be &timers.active, since this would cause them to forget their
     true timer group. */
  timer_group_t                active;

  /*to hold p0 active timers*/
  timer_group_t                p0_active;

  /* Slow clock timestamp of last write to SLEEP_XTAL_TIMETICK_MATCH */
  time_timetick_type                   set_time;

  /* Slow clock timestamp of last attempt to write SLEEP_XTAL_TIMETICK_MATCH */
  time_timetick_type                   last_set_time;

  /* Next timer expiration value */
  time_timetick_type                   set_value;

  /* Last timer expiration value */
  time_timetick_type					set_value_last;

  /* Last value written to SLEEP_XTAL_TIMETICK_MATCH */
  time_timetick_type                   match_value;

  /* Slow clock timestamp when timer_isr is called */
  time_timetick_type                   isr_time;

  /* Slow clock timestamp when timer_isr is called */
  time_timetick_type                   prev_learned_isr_time;

  /* Flag to indicate if timers_process_active_timers() is executing */
  boolean                         processing;

  /* Flag to indicate if timer_isr can be updated */
  boolean                         do_reprogram_isr;

  /* Active timers count */
  uint32                          active_timers_count;
  
  /* Pause timers count */
  uint32                          paused_timers_count;  

#ifdef TIMER_BIN_REMOVED
  /* Active timer bin list */
  timer_bin_list_type             bins;

  /* Free timer bin list */
  timer_bin_list_type             bins_free;
#endif

  /* Pointer to the first non-deferrable timer */
  timer_ptr_internal_type         first_non_deferrable;

  /* DAL Device Handle */
  DalDeviceHandle*                dal_int_handle;

  /* Backoff time */
  uint32						  backoff;

}
timers_type;


/*=============================================================================

                           FUNCTION PROTOTYPES

=============================================================================*/

timer_client_ptr_type timer_get_client_timer
(
   timer_ptr_type    timer,
   timer_ptr_type    timer_actual_addr,
   uint32            process_idx,
   timer_error_type  *status
);

void timer_init_client_timers(void);

timer_client_ptr_type timer_get_new_client_timer(timer_error_type *status, uint32 *handle);

/* ===================================================
              INTERNAL APIS
    =================================================== */

void timer_drv_set_remote_process_cb_handler(timer_process_cb_handler shim_cb_func);

void timer_drv_set_local_process_idx(uint32 local_process_index);

timer_error_type timer_drv_group_disable
(
  /* Timer group to be disabled */
  timer_group_ptr                 group,

  /* For debug purposes only  */
  timer_group_ptr                 group_actual_addr  
);

timer_error_type timer_drv_group_enable
(
  /* Timer group to be enabled */
  timer_group_ptr                 group,

  /* For debug purposes only  */
  timer_group_ptr                 group_actual_addr  
);

timer_error_type timer_drv_group_delete( timer_group_ptr group, 
	                                           timer_group_ptr group_actual_addr, 
	                                           uint32 process_idx );

timer_error_type timer_drv_group_set_deferrable( timer_group_ptr group, 
	                                                  timer_group_ptr group_actual_addr, 
	                                                  boolean condition, 
	                                                  uint32 process_idx );

time_timetick_type timer_drv_get_first_non_deferrable_timer_expiry(timer_non_defer_data_ptr timer_data_ptr);

time_timetick_type timer_drv_defer_match_interrupt_64( void );

void timer_drv_undefer_match_interrupt( void );

void timer_drv_restart(void);

timer_error_type timer_drv_def_osal_priority
(
  /* Timer to define */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Group timer will belong to (NULL = the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,  

  /* Task to signal and/or task to queue APC call for */
  timer_notify_type				  notify_type,

  /* Task signals to set to the tcb task when timer expires */
  time_osal_notify_obj_ptr		  sigs_func_addr,

  /* APC (tcb!=NULL) or callback (tcb==NULL) to queue/call at timer expiry */
  time_osal_notify_data			  sigs_mask_data,

  /* timer priority information */
  timer_priority_enum_type        timer_prio,

  /* Process Index */
  unsigned int                    process_idx
);

timer_error_type timer_drv_def_priority
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,    

  /* Group timer will belong to (NULL=the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,  

  /* Task to signal and/or task to queue APC call for */
  rex_tcb_type                    *tcb,

  /* Task signals to set to the tcb task when timer expires */
  rex_sigs_type                   sigs,

  /* APC (tcb!=NULL) or callback (tcb==NULL) to queue/call at timer expiry */
  timer_t1_cb_type                func,

  /* Arbitrary data for APC or callback */
  timer_cb_data_type              data,

  timer_priority_enum_type        timer_prio,

  /* Process Index */
  uint32							process_idx
);

timer_error_type timer_drv_def2_priority
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,    

  /* Group timer will belong to (NULL=the "NULL" timer group) */
  timer_group_ptr                 group,

  /* Group Actual Addr. For Debug purposes only */
  timer_group_ptr                 group_actual_addr,

  timer_priority_enum_type        timer_prio,

  /* Process Index */
  uint32						  process_idx
);

timer_error_type timer_drv_reg
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,

  /* Function to call at timer expiry */
  timer_t2_cb_type                func,

  /* Arbitrary data for func(time_ms, data) */
  timer_cb_data_type              data,

  /* Time (in ms) until first timer expiry */
  timetick_type                   time_ms,

  /* Period (in ms) between repeated expiries (0 = not periodic) */
  timetick_type                   reload_ms,

  /* Process Index */
  uint32							process_idx
);

timer_error_type timer_drv_set_64
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Time (in units below) until first timer expiry */
  time_timetick_type                   time,

  /* Period (in units) between repeated expiries (0 = not periodic) */
  time_timetick_type                   reload,

  /* Unit to measure "ticks" in. */
  timer_unit_type                      unit,

  /* Process Index */
  uint32                              process_idx
  
);

timer_error_type timer_drv_set_absolute
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,

  /* Time (in XOs) until first timer expiry */
  time_timetick_type              time,

  /* process index */
  unsigned int					process_idx
);

time_timetick_type timer_drv_get_64
(
  /* Timer to get */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,  

  /* Unit to measure "ticks before timer expires" in. */
  timer_unit_type                 unit,

  /* process index */
  uint32                          process_idx
);

timer_error_type timer_drv_get_params
(
  timer_ptr_type                    timer,
  timer_ptr_type                    timer_actual_addr,  
  timer_attrib_struct_type *        timer_attrib_struct_ptr,
  uint32                            process_idx
);

timer_error_type timer_drv_set_params
(
  timer_ptr_type                    timer,
  timer_ptr_type                    timer_actual_addr,
  timer_attrib_set_type             to_set,
  timer_attrib_struct_type *        timer_attrib_struct_ptr,
  uint32                            process_idx
);

timer_error_type timer_drv_undef
(
  timer_ptr_type                  timer,
  timer_ptr_type                  timer_actual_addr,
  uint32                          process_idx
);

timer_error_type timer_drv_delete_process_timers(uint32 process_idx);
timer_error_type timer_drv_delete_process_groups(uint32 process_idx);

timer_error_type timer_drv_stop
(
  timer_ptr_type				   timer,
  timer_ptr_type                   timer_actual_addr,
  timer_unit_type	               unit,
  time_timetick_type			   *rem_time,
  uint32                           process_idx
);

timer_error_type timer_drv_pause
(
  /* Timer to pause */
  timer_ptr_type                  timer,
  timer_ptr_type                  timer_actual_addr,
  uint32                          process_idx  
);

timer_error_type timer_drv_resume
(
  /* Timer to resume */
  timer_ptr_type                  timer,

  /* Timer's actual address in its own Process Domain, for logging purposes only */
  timer_ptr_type                  timer_actual_addr,   

  /* Process index */
  uint32                          process_idx
);

timer_error_type timer_drv_clr_osal_thread_timers
(
  /* Timers belonging to this task will be cleared (stopped). */
  time_osal_thread_id                    thread_id
);

timer_error_type timer_drv_clr_task_timers
(
  /* Timers belonging to this task will be cleared (stopped). */
  rex_tcb_type                    *tcb
);

time_timetick_type timer_drv_get_sclk_till_expiry_64( void );

timer_error_type timer_drv_set_wakeup_64
(
  /* Wakeup the CPU in the given number of sclk's. */
  time_timetick_type                   wakeup_sclks
);

timer_error_type timer_drv_register_for_non_defer_expiry_change
(
  timer_notify_type 		notify_type,
  time_osal_notify_obj_ptr 	notify_obj,
  time_osal_notify_data 	notify_mask,
  uint32 					process_idx
);

boolean timer_drv_is_active
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,
  uint32          process_idx
);

timer_error_type timer_drv_is_periodic_timer
(
  timer_ptr_type      timer,
  timer_ptr_type      timer_actual_addr,
  boolean            *is_periodic,
  time_timetick_type *period,
  uint32              process_idx
);

time_timetick_type timer_drv_expires_at_64
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,
  uint32          process_idx
);

time_timetick_type timer_drv_get_start_64
(
  timer_ptr_type  timer,
  timer_ptr_type  timer_actual_addr,
  uint32          process_idx
);

timer_error_type timer_drv_get_group
(
  timer_ptr_type    timer,
  timer_ptr_type    timer_actual_addr,
  timer_group_type *group,
  uint32            process_idx
);

timer_error_type timer_drv_get_expiry_type
(
  timer_ptr_type     timer,
  timer_ptr_type     timer_actual_addr,
  timer_expiry_type *expiry_type,
  uint32             process_idx
);

timer_error_type timer_drv_mark_interdependence_by_tid
(
   qurt_thread_t        tid1,
   qurt_thread_t        tid2
);

timer_error_type timer_drv_mark_interdependence_by_name
( 
   char      *thread1_name ,
   char      *thread2_name
);
#endif /* TIMER_V_H */
