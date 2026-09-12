/*=============================================================================

                Timer_Client.c

GENERAL DESCRIPTION
      Timer Client Process Code

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


$Header: //components/rel/core.mpss/10.0/services/time/src/timer_client.c#1 $
$DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when		who     	what, where, why
--------	---		------------------------------------------------------------
08/31/18    abr     Changed the timer_sleep API to use cancellable version of signal_wait API 
                    instead of normal signal_wait
05/20/14   	abh		Added changes to support MultiPD on MPSS
06/20/13   	ab		Add file that provides MultiPD changes
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "qurt_signal2.h"
#include "timer.h"
#include "ats_v.h"
#include "timer_qdi_v.h"
#include "DALSys.h"
#include "rcinit.h"
#include "err.h"
#include "dll_global_def.h"

/*****************************************************************************/
/*                          DATA DECLARATIONS                                */
/*****************************************************************************/

/* Signals which must cause the active timer list to be processed */
#define TIMER_SLEEP_CB_SIG_MASK 0x80000000

/* Current Process idx */
int timer_client_pid;

/* Qdi Timer Client Handle to communicate with Guest os layer */
int timer_client_qdi_handle = -1;

/* Timers declared to be in the NULL group are reparented to this group */
timer_group_type                  timer_null_group;

/* Global Non-deferrable group for various clients use */
/* Note: This group shouldn't be called with disable/enable group functions as
   this will be used by other clients too. */
__attribute__((visibility("default"))) timer_group_type                  timer_non_defer_group;

/* Factor to divide 19.2MHz clock into 32kHz */
#ifndef XO_TO_SCLK_CONV_FACTOR
   #define XO_TO_SCLK_CONV_FACTOR 586
#endif

#define FEATURE_TIMER_CLIENT_DEBUG
#ifdef FEATURE_TIMER_CLIENT_DEBUG

/*These values are set high to avoid the annoying F3 messages
 and an error fatal in case cb processing time is high.
 1 sec = 19200000 ticks.
*/
volatile static uint64 timer_client_processing_time_warning_threshold = 19200;
#ifndef TIMER_CLIENT_ERROR_FATAL_THRESHOLD_ZEROED_OUT
volatile static uint64 timer_client_processing_time_err_fatal_threshold=38400000;
#else
volatile static uint64 timer_client_processing_time_err_fatal_threshold=0;
#endif


typedef struct timer_client_debug_var_1
{
  timer_ptr_type         timer;
  uint32                 notify_type;
  void *                 sig_func_addr;
  uint32                 mask_data;
  time_timetick_type     processing_started;
  time_timetick_type     processing_time;
}timer_client_expired_type;
#define MAX_TIMER_CLIENT_EXPIRED 8
static timer_client_expired_type timer_client_expired[MAX_TIMER_CLIENT_EXPIRED];
static int timer_client_expired_cnt = 0;

#endif /* FEATURE_TIMER_CLIENT_DEBUG */

/*==============================================================================
  LOCAL FUNCTIONS DEFINITION
=============================================================================*/


/*=============================================================================

FUNCTION TIMER_GET_TIMETICK

DESCRIPTION
  This function returns 19.2MHz based XO timetick value.

DEPENDENCIES
  None.

RETURN VALUE
  Returns 19.2MHz timetick value.

SIDE EFFECTS
  None.

=============================================================================*/

static inline time_timetick_type timer_get_timetick( void )
{
  return (time_timetick_type)(qurt_sysclock_get_hw_ticks());
} /* timer_get_timetick */


/*=============================================================================

FUNCTION TIMER_CONV_TO_SCLK_FREQ

DESCRIPTION
  This function returns 32kHz timetick value based on 19MHz timetick value.

DEPENDENCIES
  None.

RETURN VALUE
  Returns 32kHz timetick value.

SIDE EFFECTS
  None.

=============================================================================*/

static inline timetick_type timer_conv_to_sclk_freq(uint64 time_in)
{
  return (timetick_type)(time_in/XO_TO_SCLK_CONV_FACTOR);
} /* timer_conv_to_sclk_freq */



/*=============================================================================

FUNCTION TIMER_CONV_TO_XO_FREQ

DESCRIPTION
  This function returns 19MHz timetick value based on 32kHz timetick value.

DEPENDENCIES
  None.

RETURN VALUE
  Returns 19MHz timetick value.

SIDE EFFECTS
  None.

=============================================================================*/

static inline uint64 timer_conv_to_xo_freq(timetick_type time_in)
{
  return ((uint64)time_in)*((uint64)XO_TO_SCLK_CONV_FACTOR);
} /* timer_conv_to_xo_freq */


/*==============================================================================

                            A P I   D E F I N I T I O N S

=============================================================================*/

/*==============================================================================

                        TIMER GROUP DEFINITIONS

=============================================================================*/
/**
Enables a timer group. Timers in the timer group that expired during the
diabled period have their expiry processing executed.

@param[in] timer_group Timer group to be enabled.

@return
None.

@dependencies
None.

@sideeffects
This function may generate signals to a task, which can cause task switches. \n
This function may queue an asynchronous procedure call.
*/
timer_error_type timer_group_enable
(
  timer_group_ptr                 timer_group
)
{
   if(!timer_group)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_group_enable(timer_client_qdi_handle, timer_group);
}

/**
Disables a timer group. Timers in the timer group continue to count
down, but they do not expire.

@param[in] timer_group Timer group to be disabled.

@return
None.

@dependencies
None.
*/
timer_error_type timer_group_disable
(
  timer_group_ptr                 timer_group
)
{
   if(!timer_group)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_group_disable(timer_client_qdi_handle, timer_group);
}

/*==============================================================================

                        DEFER-UNDEFER DEFINITIONS

=============================================================================*/


/**
Delete the timer group created by timer_group_init or timer_group_set_deferrable 

@param[in] pgroup    Timer group to be delete.
@param[in] condition Boolean value: \n
                     TRUE -- Deferrable. \n
                     FALSE -- Non-Deferrable.
@return
None.

@dependencies
None.
*/
DLL_API_GLOBAL timer_error_type timer_group_delete
(
  timer_group_ptr pgroup
)
{
   if(!pgroup)
      return TE_INVALID_PARAMETERS;

   if ( (pgroup == &timer_null_group) ||
		(pgroup == &timer_non_defer_group)
	  )
      return TE_INVALID_PARAMETERS;

   return timer_invoke_group_delete(timer_client_qdi_handle, pgroup);
}

/**
Sets the timer group deferrable status.

@note1hang Timer groups default to being non-deferrable.

@param[in] pgroup    Timer group to be undeferred.
@param[in] condition Boolean value: \n
                     TRUE -- Deferrable. \n
                     FALSE -- Otherwise.
@return
None.

@dependencies
None.
*/
DLL_API_GLOBAL timer_error_type timer_group_set_deferrable
(
  timer_group_ptr pgroup,
  boolean condition
)
{
   if(!pgroup)
      return TE_INVALID_PARAMETERS;

   if ( (pgroup == &timer_null_group  && condition == FALSE) ||
		(pgroup == &timer_non_defer_group && condition == TRUE)
	  )
      return TE_INVALID_PARAMETERS;

   return timer_invoke_group_set_deferrable(timer_client_qdi_handle, pgroup, condition);
}

/**
Gets the first non-deferrable timer's expiry value.

@return
Returns the absolute expiry of first occuring non-deferrable timer.

@dependencies
None.
*/
time_timetick_type timer_get_first_non_deferrable_timer_expiry
(
  timer_non_defer_data_ptr timer_data_ptr 
)
{

  time_timetick_type ret_val = 0;
  timer_invoke_get_first_non_deferrable_timer_expiry(timer_client_qdi_handle,&ret_val, timer_data_ptr);
  return ret_val;
} /* timer_get_first_non_deferrable_timer_expiry */


/**
Gets time to the next occurring non-deferrable timer expiry value.

@return
Number of ticks in 32.768KHz domain until the next timer expiry in a
 timer group which is non-deferrable.

@dependencies
Must be called with interrupts locked.
*/
timetick_type timer_get_time_till_first_non_deferrable_timer( void )
{
  return timer_conv_to_sclk_freq(timer_get_time_till_first_non_deferrable_timer_64());
}

/**
Gets time to the next occurring non-deferrable timer expiry value.

@return
Number of ticks in 19MHz domain until the next timer expiry in a
 timer group which is non-deferrable.

@dependencies
Must be called with interrupts locked.
*/
time_timetick_type timer_get_time_till_first_non_deferrable_timer_64( void )
{
   time_timetick_type tt_ret = 0;
   time_timetick_type first_non_def_expiry = 0;
   time_timetick_type ticks_now = 0;
   first_non_def_expiry = timer_get_first_non_deferrable_timer_expiry(NULL);
   ticks_now = timer_get_timetick();
   /* If future_expiry is in past */
   if (first_non_def_expiry < ticks_now )
   {
     tt_ret = 0;
   }
   else
   {
     /* Return the difference between now and the above future expiry */
     tt_ret = (first_non_def_expiry - ticks_now);
   }
   return tt_ret;
}



/**
Sets timer match interrupt to the next occurring non-deferrable timer expiry
value. Complementary function is
timer_undefer_match_interrupt().

@return
timetick_type -- Number of ticks in 19MHz domain
until the next timer expiry in a timer group that is
non-deferrable.

@dependencies
This function must be called with interrupts locked.


@sideeffects
Adjusts the match value and prevents any subsequent timer calls from
updating the match value until timer_undefer_match_interrupt() is called.
*/
timetick_type timer_defer_match_interrupt( void )
{
  return timer_conv_to_sclk_freq(timer_defer_match_interrupt_64());
}

/**
Sets timer match interrupt to the next occurring non-deferrable timer expiry
value. Complementary function is
timer_undefer_match_interrupt().

@return
time_timetick_type -- Number of ticks in 19MHz domain
until the next timer expiry in a timer group that is
non-deferrable.

@dependencies
This function must be called with interrupts locked.


@sideeffects
Adjusts the match value and prevents any subsequent timer calls from
updating the match value until timer_undefer_match_interrupt() is called.
*/
time_timetick_type timer_defer_match_interrupt_64( void )
{
   time_timetick_type ticks = 0;
   timer_invoke_defer_match_interrupt_64(timer_client_qdi_handle, &ticks);
   return ticks;
}

/**
Sets the timer ISR to fire at the earliest expiring timer in the active list.
This function is meant to serve as the complementary function to
timer_defer_match_interrupt().

@dependencies
This function must be called with interrupts locked.

@sideeffects
This function adjusts the match value.
*/
void timer_undefer_match_interrupt( void )
{
   timer_invoke_undefer_match_interrupt(timer_client_qdi_handle);
}

/*==============================================================================

                     TIMER INTERNAL FUNCTION DEFINITIONS

=============================================================================*/
/**
Defines and initializes a timer.

Please refer timer.h
*/
DLL_API_GLOBAL timer_error_type timer_def
(
  timer_ptr_type                  timer,
  timer_group_ptr                 group,
  rex_tcb_type                    *tcb,
  rex_sigs_type                   sigs,
  timer_t1_cb_type                func,
  timer_cb_data_type              data
)
{
   timer_error_type  status = TE_SUCCESS;


   status = timer_def_priority(timer,
   	                           group,
   	                           tcb,
   	                           sigs,
   	                           func,
   	                           data,
   	                           TIMER_PRIORITY_1);

   if (status == TE_INVALID_PARAMETERS)
  	{
  	ERR_FATAL ("Invalid parameters passed timer=0x%x; tcb=0x%x; func=0x%x",
		        timer,tcb,func);
    return status;
  	}
  else
  	return status;
}

/**
Defines and initializes a timer with
an option to set priority timer.

Please refer timer.h
*/
timer_error_type timer_def_priority
(
  timer_ptr_type                  timer,
  timer_group_ptr                 group,
  rex_tcb_type                    *tcb,
  rex_sigs_type                   sigs,
  timer_t1_cb_type                func,
  timer_cb_data_type              data,
  timer_priority_enum_type        timer_prio
)
{
   if(timer_client_qdi_handle == -1)
   {
      ERR_FATAL("Timer system not initialized", 0, 0, 0);
      return TE_TIMER_MODULE_NOT_INITIALIZED;
   }

   if(timer == NULL || timer_prio > TIMER_MAX_PRIORITY)
      return TE_INVALID_PARAMETERS;


   /* Reparent timers in the NULL group to be in the timer_null_group */
   if ( group == NULL )
   {
	 group = &timer_null_group;
   }

   return timer_invoke_def_priority(timer_client_qdi_handle,
   	                                timer,
   	                                group,
   	                                tcb,
   	                                sigs,
   	                                func,
   	                                data,
   	                                timer_prio);
}

/**
Defines and initializes a timer. Signal and function notifications are
exclusive. If both are provided at the same time, error will be returned.

Please refer timer.h
*/
DLL_API_GLOBAL timer_error_type timer_def_osal
(
  timer_ptr_type				timer,
  timer_group_ptr 				group,
  timer_notify_type				notify_type,
  time_osal_notify_obj_ptr		sigs_func_addr,
  time_osal_notify_data			sigs_mask_data
)
{
  return timer_def_osal_priority( timer,
  	                              group,
  	                              notify_type,
  	                              sigs_func_addr,
  	                              sigs_mask_data,
  	                              TIMER_PRIORITY_1);
}

/**
Defines and initializes a timer. Signal and function notifications are
exclusive. If both are provided at the same time, error will be returned.

Please refer timer.h
*/
timer_error_type timer_def_osal_priority
(
  timer_ptr_type				  timer,
  timer_group_ptr 			      group,
  timer_notify_type			      notify_type,
  time_osal_notify_obj_ptr	      sigs_func_addr,
  time_osal_notify_data		      sigs_mask_data,
  timer_priority_enum_type        timer_prio
)
{
   if(timer_client_qdi_handle == -1)
   {
      ERR_FATAL("Timer system not initialized", 0, 0, 0);
      return TE_TIMER_MODULE_NOT_INITIALIZED;
   }

   if(timer == NULL  || timer_prio > TIMER_MAX_PRIORITY)
      return TE_INVALID_PARAMETERS;

   /* Reparent timers in the NULL group to be in the timer_null_group */
   if ( group == NULL )
   {
	 group = &timer_null_group;
   }

   //group being NULL is allowed for timer_def, timer_def2
   return timer_invoke_def_osal_priority(timer_client_qdi_handle,
   	                                     timer,
       	                                 group,
   	                                     notify_type,
   	                                     sigs_func_addr,
   	                                     sigs_mask_data,
   	                                     timer_prio);
}

/**
Defines a clock callback-style timer. When the timer expires, the callback
function with the arguments time_ms and data is called from the interrupt context.
Timers may also be staticly declared via the TIMER_DEF2() macro.

Please refer timer.h for more details
*/
DLL_API_GLOBAL timer_error_type timer_def2
(
  timer_ptr_type                  timer,
  timer_group_ptr                 group
)
{
   timer_error_type  status = TE_SUCCESS;

   status = timer_def2_priority(timer, group, TIMER_PRIORITY_1);

   if (status == TE_INVALID_PARAMETERS)
  	{
  	ERR_FATAL ("Invalid parameters passed timer=0x%x; group=0x%x",
		        timer,group,0);
    return status;
  	}
  else
  	return status;

}

/**
Defines a clock callback-style timer. When the timer expires, the callback
function with the arguments time_ms and data is called from the interrupt context.
Timers may also be staticly declared via the TIMER_DEF2() macro.

Please refer timer.h for more details
*/
timer_error_type timer_def2_priority
(
  timer_ptr_type                  timer,
  timer_group_ptr                 group,
  timer_priority_enum_type        timer_prio
)
{
   if(timer_client_qdi_handle == -1)
   {
      ATS_ERR("Timer system not initialized");
      return TE_TIMER_MODULE_NOT_INITIALIZED;
   }

   if(timer == NULL  || timer_prio > TIMER_MAX_PRIORITY)
      return TE_INVALID_PARAMETERS;

   /* Reparent timers in the NULL group to be in the timer_null_group */
   if ( group == NULL )
   {
	 group = &timer_null_group;
   }

   //group being NULL is allowed for timer_def, timer_def2
   return timer_invoke_def2_priority(timer_client_qdi_handle, timer, group, timer_prio);
}

/**
Initializes a clock callback style timer.

Please refer timer.h for more details
*/
DLL_API_GLOBAL timer_error_type timer_reg
(
  timer_ptr_type                  timer,
  timer_t2_cb_type                func,
  timer_cb_data_type              data,
  timetick_type                   time_ms,
  timetick_type                   reload_ms
)
{
   if(timer == NULL)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_reg(timer_client_qdi_handle, timer, func, data, time_ms, reload_ms);
}

/**
Sets an inactive timer to expire after a given period of time, or changes an
active timer to expire after a given period of time. Optionally, specifies
the timer to repeatly expire with a given period.  If the timer
is to be set in units of ticks, it will be done in the 19MHz
domain.

Please refer timer.h for more details
*/
timer_error_type timer_set
(
  timer_ptr_type                  timer,
  timetick_type                   time,
  timetick_type                   reload,
  timer_unit_type                 unit
)
{
  /* If the unit type is in legacy ticks, we will convert to new timetick */
  if( unit == T_TICK || unit == T_NONE )
  {
    return timer_set_64(timer,
  			   timer_conv_to_xo_freq(time),
  			   timer_conv_to_xo_freq(reload),
  			   unit);
  }
  else
  {
    return timer_set_64(timer,
  			   time,
  			   reload,
  			   unit);
  }
}

/**
Sets an inactive timer to expire after a given period of time, or changes an
active timer to expire after a given period of time. Optionally, specifies
the timer to repeatly expire with a given period.  If the timer
is to be set in units of ticks, it will be done in the 19MHz
domain.

Please refer timer.h for more details
*/
DLL_API_GLOBAL timer_error_type timer_set_64
(
  timer_ptr_type                  timer,
  time_timetick_type              time,
  time_timetick_type              reload,
  timer_unit_type                 unit
)
{
   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_set_64", timer, time, reload);
      return TE_INVALID_PARAMETERS;
   }

   return timer_invoke_set_64(timer_client_qdi_handle, timer, time, reload, unit);
}

/**
*
*Sets timer to expire at absolute ticks (instead of relative expiry)

Please refer timer.h for more details
*/
DLL_API_GLOBAL timer_error_type timer_set_absolute
(
  /* Timer to set */
  timer_ptr_type                  timer,

  /* Time (in XOs) until first timer expiry */
  time_timetick_type              time

)
{
   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_set_absolute", timer, time, 0);
      return TE_INVALID_PARAMETERS;
   }

   return timer_invoke_set_absolute(timer_client_qdi_handle, timer, time);
}

/**
Get number of ticks in 32kHz domain before timer expires

Please refer timer.h for more details
*/
DLL_API_GLOBAL timetick_type timer_get
(
  timer_ptr_type                  timer,
  timer_unit_type                 unit
)
{
  if( unit == T_TICK || unit == T_NONE )
  {
    return timer_conv_to_sclk_freq(timer_get_64(timer,unit));
  }
  else
  {
    return (timetick_type)timer_get_64(timer,unit);
  }
}



/**
Gets the number of units before the timer expires. If the unit
is ticks, it will return value in 19MHz domain.

Please refer timer.h for more details
*/
time_timetick_type timer_get_64
(
  timer_ptr_type                  timer,
  timer_unit_type                 unit
)
{
   time_timetick_type rem_ticks = 0;

   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_set_absolute", timer, 0, 0);
      return 0;
   }

   timer_invoke_get_64(timer_client_qdi_handle, timer, unit, &rem_ticks);
   return rem_ticks;
}

/**
Updates the passed structure as per the timer parameters.

Please refer timer.h for more details
*/
timer_error_type timer_get_params
(
  timer_ptr_type                    timer,
  timer_attrib_struct_type *        timer_attrib_struct_ptr
)
{
  /* return error if the passed timer pointer is NULL */
  if (timer == NULL || timer_attrib_struct_ptr == NULL)
	  return TE_INVALID_PARAMETERS;

   return timer_invoke_get_params(timer_client_qdi_handle, timer, timer_attrib_struct_ptr);
}

/**
Updates the passed structure as per the timer parameters.

Please refer timer.h for more details
*/
timer_error_type timer_set_params
(
  timer_ptr_type                    timer,
  timer_attrib_set_type             to_set,
  timer_attrib_struct_type *        timer_attrib_struct_ptr
)
{
	if (timer == NULL || timer_attrib_struct_ptr == NULL)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_set_params(timer_client_qdi_handle, timer, to_set, timer_attrib_struct_ptr);
}

/**
Queries whether a timer is active. A timer that has been set to expire at a
certain time is considered to be active even if the group it is a member of is
disabled. A timer that is explicitly paused is not active until it has been
resumed.

Please refer timer.h for more details
*/
boolean timer_is_active
(
  timer_ptr_type  timer
)
{
   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_is_active", timer, 0, 0);
      return FALSE;
   }

   return timer_invoke_is_active(timer_client_qdi_handle, timer);
}

/**
Queries whether a timer is periodic or not. Also returns the timer reload
period in the passed argument.

Please refer timer.h for more details
*/
timer_error_type timer_is_periodic
(
  timer_ptr_type      timer,
  boolean            *is_periodic,
  time_timetick_type *period
)
{
   if(timer == NULL || is_periodic == NULL)
      return TE_INVALID_PARAMETERS;

   *is_periodic = FALSE;
   if(period)
      *period = 0;

   return timer_invoke_is_periodic(timer_client_qdi_handle, timer, is_periodic, period);
}


/**
Returns the SCLK counter value when the timer expires.

Please refer timer.h for more details
*/
timetick_type timer_expires_at
(
  timer_ptr_type  timer
)
{
  return timer_conv_to_sclk_freq(timer_expires_at_64(timer));
}

/**
Returns the XO counter value when the timer expires.

Please refer timer.h for more details
*/
time_timetick_type timer_expires_at_64
(
  timer_ptr_type  timer
)
{
   time_timetick_type ticks = 0;

   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_expires_at_64", timer, 0, 0);
      return 0;
   }

   timer_invoke_expires_at_64(timer_client_qdi_handle, timer, &ticks);
   return ticks;
}

/**
Returns the slow-clock counter value in the 32KHz domain when
the timer started.

Please refer timer.h for more details
*/
timetick_type timer_get_start
(
  timer_ptr_type  timer
)
{
  return timer_conv_to_sclk_freq(timer_get_start_64(timer));
}

/**
Returns the slow-clock counter value in the 19MHz domain when
the timer started.

Please refer timer.h for more details
*/
time_timetick_type timer_get_start_64
(
  timer_ptr_type  timer
)
{
   time_timetick_type ticks = 0;

   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_get_start_64", timer, 0, 0);
      return 0;
   }

   timer_invoke_get_start_64(timer_client_qdi_handle, timer, &ticks);
   return ticks;
}

/**
Gets group handle

Please refer timer.h for more details
*/
timer_error_type timer_get_group
(
  timer_ptr_type  timer,
  timer_group_type *group
)
{
   if(timer == NULL || group == NULL)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_get_group(timer_client_qdi_handle, timer, group);
}

/**
Gets timer expiry type handle/

Please refer timer.h for more details
*/
timer_error_type timer_get_expiry_type
(
  timer_ptr_type  timer,
  timer_expiry_type *expiry_type
)
{
   if(timer == NULL || expiry_type == NULL)
      return TE_INVALID_PARAMETERS;

   *expiry_type = TIMER_RELATIVE_EXPIRY;

   return timer_invoke_get_expiry_type(timer_client_qdi_handle, timer, expiry_type);
}

/**
Stops an active timer.  If units are in ticks, then it will
return the value in the 19MHz domain.

Please refer timer.h for more details
*/
DLL_API_GLOBAL timetick_type timer_clr
(
  timer_ptr_type                  timer,
  timer_unit_type                 unit
)
{
  if( unit == T_TICK || unit == T_NONE )
  {
    return timer_conv_to_sclk_freq(timer_clr_64(timer,unit));
  }
  else
  {
    return (timetick_type)timer_clr_64(timer,unit);
  }

}




/**
Stops an active timer.  If units are in ticks, then it will
return the value in the 19MHz domain.

Please refer timer.h for more details
*/
DLL_API_GLOBAL time_timetick_type timer_clr_64
(
  timer_ptr_type                  timer,
  timer_unit_type                 unit
)
{
   time_timetick_type	rem_time = 0;

   if (TE_SUCCESS != timer_stop(timer,unit,&rem_time))
    return 0;
   else
    return rem_time;
}

/**
This api is same as timer_clr, except that it has error type as return value.
Stops an active timer.  If units are in ticks, then it will
return the value in the 19MHz domain.

Please refer timer.h for more details
*/
DLL_API_GLOBAL timer_error_type timer_stop
(
  timer_ptr_type                  timer,
  timer_unit_type                 unit,
  time_timetick_type              *rem_time
)
{
   if(timer == NULL)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_stop(timer_client_qdi_handle, timer, unit, rem_time);
}

/**
Removes timer from active list and deallocates the timer memory

Please refer timer.h for more details
*/

DLL_API_GLOBAL timer_error_type timer_undef
(
  /* Timer to stop */
  timer_ptr_type                  timer
)
{
   if(timer == NULL)
      return TE_INVALID_PARAMETERS;

   return timer_invoke_undef(timer_client_qdi_handle, timer);
}


/**
Pauses an active timer.

Please refer timer.h for more details
*/
timer_error_type timer_pause
(
  timer_ptr_type                  timer
)
{
   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_pause", timer, 0, 0);
      return TE_INVALID_PARAMETERS;
   }

   return timer_invoke_pause(timer_client_qdi_handle, timer);
}


/**
Resumes a previously paused timer. When a timer is paused 5 seconds before its
expiry and timer_resume( ) is called 30 seconds later, the timer expires
5 seconds after timer_resume( ) was called.

Please refer timer.h for more details
*/
timer_error_type timer_resume
(
  timer_ptr_type                  timer
)
{
   if(timer == NULL)
   {
      ERR_FATAL("timer 0x%x is NULL for timer_resume", timer, 0, 0);
      return TE_INVALID_PARAMETERS;
   }

   return timer_invoke_resume(timer_client_qdi_handle, timer);
}


/**
Stops active timers belonging to the given task.

Please refer timer.h for more details
*/
void timer_clr_osal_thread_timers
(
  time_osal_thread_id                    thread_id
)
{
   timer_invoke_clr_osal_thread_timers(timer_client_qdi_handle, thread_id);
}

/**
Stops active timers belonging to the given task.

Please refer timer.h for more details
*/
void timer_clr_task_timers
(
  rex_tcb_type                    *tcb
)
{
	if(tcb == NULL)
	   return;

   timer_invoke_clr_task_timers(timer_client_qdi_handle, tcb);
}


/**
Returns the number of slow clocks in 32KHz domain until the
expiry of the next timer to expire.

Please refer timer.h for more details
*/
timetick_type timer_get_sclk_till_expiry( void )
{
  return timer_conv_to_sclk_freq(timer_get_sclk_till_expiry_64());
}

/**
Returns the number of slow clocks in 19MHz domain until the
expiry of the next timer to expire.

Please refer timer.h for more details
*/
time_timetick_type timer_get_sclk_till_expiry_64( void )
{
   time_timetick_type ticks = 0;
   timer_invoke_get_sclk_till_expiry_64(timer_client_qdi_handle, &ticks);
   return ticks;
}

/**
Sets the slow-clock timer interrupt to expire in advance of the next expiring
timer expiry point to allow for TCXO to turn on.

Please refer timer.h for more details
*/
timer_error_type timer_set_wakeup
(
  timetick_type                   wakeup_sclks
)
{
  return timer_set_wakeup_64(timer_conv_to_xo_freq(wakeup_sclks));
}


/**
Sets the slow-clock timer interrupt to expire in advance of the next expiring
timer expiry point to allow for TCXO to turn on.

Please refer timer.h for more details
*/
timer_error_type timer_set_wakeup_64
(
  time_timetick_type                   wakeup_sclks
)
{
   return timer_invoke_set_wakeup_64(timer_client_qdi_handle, wakeup_sclks);
}

/**
Readjusts the timetick counter values to synchronize with the base timetick
counter when waking up from a power collapse. The match value must be adjusted
after a power collapse and this function performs that adjustment. This
function is called after waking up.

Please refer timer.h for more details
*/
void timer_restart(void)
{
   timer_invoke_restart(timer_client_qdi_handle);
}


/*=============================================================================

FUNCTION TIMER_SLEEP_CB

DESCRIPTION
 Signals the task waiting on the task_sleep_timer by timer_sleep function.
 Data will be signal object for Qurt Kernel while task pointer on Rex kernel

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void timer_sleep_cb
(
timer_cb_data_type      data
)
{

#ifdef FEATURE_TIME_QDSP6

   /* Signal the task in order to resume the sleeping task */
   (void) qurt_anysignal_set((qurt_anysignal_t*)data, TIMER_SLEEP_CB_SIG_MASK);

#else

  /* Use rex_resume_task in case of non-q6 kernel */
   rex_resume_task((rex_tcb_type*)data);

#endif /*FEATURE_TIME_QDSP6*/

   return;
}

/**
 Used by the timer client which are dependent on each
 other.

@param[in] tid1 Thread id of first task
@param[in] tid2 Thread id of second task

@return
TE_SUCCESS  If the operation completed successfully
TE_FAIL     If the operation fails

@dependencies  
 This function should only be called from the clients task context 
 as well as slave task context also.

@sideeffects
None.
*/
timer_error_type timer_mark_interdependence_by_tid
(
   qurt_thread_t        tid1,
   qurt_thread_t        tid2
)
{
	return timer_invoke_mark_interdependence_by_tid(timer_client_qdi_handle , tid1 , tid2);
}

/**
 Used by the timer client which are dependent on each
 other.

@param[in] thread1_name Thread name of first task
@param[in] thread2_name Thread name of second task

@return
TE_SUCCESS  If the operation completed successfully
TE_FAIL     If the operation fails

@dependencies  
 This function should only be called from the clients task context 
 as well as slave task context also.

@sideeffects
None.
*/
timer_error_type timer_mark_interdependence_by_name
(
   char       *thread1_name,
   char       *thread2_name
)
{
	return timer_invoke_mark_interdependence_by_name(timer_client_qdi_handle , 
	      thread1_name ,strlen(thread1_name), thread2_name, strlen(thread2_name));
}
/*=============================================================================

FUNCTION TIMER_SLEEP

DESCRIPTION
 Suspends the client task to get suspended for the specified time.
 If non_deferrable option is True, task will surely resumed at specified time
 If non_deferrable option is False, and if Proc goes into sleep at the time of
 specified timeout, task will resume only after Proc wakesup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
DLL_API_GLOBAL void timer_sleep
(
  time_timetick_type	timeout,
  timer_unit_type		unit,
  boolean				non_deferrable
)
{
#ifdef FEATURE_TIME_QDSP6

  /* Signal object for timer callback notification */
  qurt_anysignal_t        timer_sleep_cb_sig_obj;
  unsigned int            dummy;

#else /*FEATURE_TIME_QDSP6*/

  /* rex tcb pointer to be used in case of rex kernel & logging */
  rex_tcb_type            *client_tcb = NULL;

#endif /*FEATURE_TIME_QDSP6*/

  /* Timer for the client to handle the sleeping */
  timer_type		      task_sleep_timer;

  /* Data to send to the timer callback */
  timer_cb_data_type      cb_data = 0;

  /* memset the external timer memory to zero */
  memset(&task_sleep_timer, 0, sizeof(timer_type));

#ifdef FEATURE_TIME_QDSP6

  /* Initialize the qurt signal object */
  qurt_anysignal_init (&timer_sleep_cb_sig_obj);
  cb_data=(timer_cb_data_type)&timer_sleep_cb_sig_obj;

#else /*FEATURE_TIME_QDSP6*/

  /* Get the tcb pointer to be used for the rex_resume/suspend calls & logging */
  client_tcb=rex_self();

  /* Copy rex tcb pointer in the variable to be passed as timer callback data */
  cb_data=(timer_cb_data_type)client_tcb;

#endif /*FEATURE_TIME_QDSP6*/

  if (non_deferrable != TRUE)
    {
	  /* Create a deferrable timer to wake up the calling task */
	  timer_def( &task_sleep_timer,
	             NULL, /* Null Timer Group */
				 NULL,
				 (rex_sigs_type)0,
				 timer_sleep_cb,
				 cb_data
			   );
	}
  else
    {
	  /* Create a non-deferrable timer for the clinet */
	  timer_def( &task_sleep_timer,
	             &timer_non_defer_group,
				 NULL,
				 (rex_sigs_type)0,
				 timer_sleep_cb,
				 cb_data
			   );
	}

  /* Set the timer to expire after the specified duration */
    (void)timer_set_64(&task_sleep_timer,
                 timeout,
                 0,
                 unit);

#ifdef FEATURE_TIME_QDSP6
  /* Wait on the Qurt signal object */
  qurt_signal_wait_cancellable(&timer_sleep_cb_sig_obj, TIMER_SLEEP_CB_SIG_MASK, QURT_SIGNAL_ATTR_WAIT_ANY, &dummy);

  /* Clear the signal */
  qurt_anysignal_clear(&timer_sleep_cb_sig_obj, TIMER_SLEEP_CB_SIG_MASK);

  /* Destroy the qurt signal object */
  qurt_anysignal_destroy(&timer_sleep_cb_sig_obj);

#else /*FEATURE_TIME_QDSP6*/
   /* For rex kernels */
   	/* Keep in a loop & susend the task until the timer is actually expired */
	do
	{
	   rex_suspend_task(client_tcb);
	}
	while((uint32)timer_get(&task_sleep_timer,T_MSEC))!=0);

#endif /*FEATURE_TIME_QDSP6*/

  /* Un-define the local timer */
  timer_undef(&task_sleep_timer);

  return;
}

/*=============================================================================
FUNCTION TIMER_REGISTER_FOR_NON_DEFER_EXPIRY_CHANGE

DESCRIPTION
Registers for first non-deferrable timer expiry change notification.
Only signal type notifications are allowed, for others, error will be
returned.

DEPENDENCIES
  Input objects should be initialized by client

RETURN VALUE
TE_INVALID_PARAMETERS   If passed sig obj addr is NULL or invalid notify type
TE_SUCCESS :            If the operation completed successfully

SIDE EFFECTS
  None
=============================================================================*/
timer_error_type timer_register_for_non_defer_expiry_change
(
  timer_notify_type			notify_type,
  time_osal_notify_obj_ptr 	notify_obj,
  time_osal_notify_data 	notify_mask
)
{
   return timer_invoke_register_for_non_defer_expiry_change(timer_client_qdi_handle,
   	                                                        notify_type,
   	                                                        notify_obj,
   	                                                        notify_mask);
}

/*==============================================================================

                     CALLBACK IMPLEMENTATION

=============================================================================*/
/*===========================================================================
TimerCallbackHandler
    Gets callback details and
    either calls callback functions
    or sets masks to signals
===========================================================================*/
void TimerCallbackHandler(void *parameter)
{
   int result;
   timer_cb_info cb_details;
   timer_t1_cb_type cb_func1;
   timer_t2_cb_type cb_func2;
#ifdef FEATURE_TIMER_CLIENT_DEBUG
   unsigned long long cb_processing_started, cb_processing_time;
#endif /* FEATURE_TIMER_CLIENT_DEBUG */

   while(1)
   {
      result = timer_invoke_get_cb_info(timer_client_qdi_handle, &cb_details);

      /* If there was a cancel signal or copy fail, try again */
      if( result == TIMER_ERR_CANCEL_WAIT ||
          result == TIMER_ERR_QDI_CPY_FAIL )
      {
         //ATS_ERR_1("timer qdi server returned %d result", result);
         continue;
      }

      //printf("TimerCallbackHandler: 0x%x in handle: %d \n", (unsigned int)cb_details.sig_func_addr, timer_client_qdi_handle);

#ifdef FEATURE_TIMER_CLIENT_DEBUG
      cb_processing_started = timer_get_timetick();
#endif /* FEATURE_TIMER_CLIENT_DEBUG */

      if(cb_details.cb_type == TIMER_NATIVE_OS_SIGNAL_TYPE)
      {
        qurt_anysignal_set((qurt_anysignal_t *)cb_details.sig_addr, cb_details.mask_data2);
      }
      else if(cb_details.cb_type == TIMER_NATIVE_OS_SIGNAL2_TYPE)
      {
        qurt_signal2_set((qurt_signal2_t *)cb_details.sig_addr, cb_details.mask_data2);
      }
	  else if(cb_details.cb_type == TIMER_REX_TCB_SIGNAL_TYPE ||
	  	      cb_details.cb_type == TIMER_SIGS_FUNC_DUAL_NOTIFY )
      {
        #ifdef FEATURE_TIMER_REX_IS_AVAILABLE
          rex_set_sigs((rex_tcb_type *)cb_details.sig_addr, cb_details.mask_data2);
        #else
          ERR_FATAL("Signal type cannot be rex type. Cb type: %d, sigs_obj: 0x%x, sigs: 0x%x", cb_details.cb_type, cb_details.sig_addr, cb_details.mask_data2);
        #endif /* FEATURE_TIMER_REX_IS_AVAILABLE */
      }
      else if(cb_details.cb_type == TIMER_DALSYS_EVENT_SIGNAL_TYPE)
      {
        timer_dalsys_sig_set(cb_details.sig_addr);
      }

	  if(cb_details.cb_type == TIMER_FUNC1_CB_TYPE ||
         cb_details.cb_type == TIMER_SIGS_FUNC_DUAL_NOTIFY )
      {
        cb_func1 = (timer_t1_cb_type) cb_details.func_addr;
        cb_func1(cb_details.data);
      }
      else if(cb_details.cb_type == TIMER_FUNC2_CB_TYPE)
      {
         cb_func2 = (timer_t2_cb_type) cb_details.func_addr;
         cb_func2(cb_details.mask_data2, cb_details.data);
      }

#ifdef FEATURE_TIMER_CLIENT_DEBUG
      cb_processing_time = timer_get_timetick() - cb_processing_started;

      timer_client_expired[timer_client_expired_cnt].timer              = cb_details.timer;
      timer_client_expired[timer_client_expired_cnt].notify_type        = cb_details.cb_type;
	  if (cb_details.cb_type == TIMER_FUNC1_CB_TYPE ||
	  	  cb_details.cb_type == TIMER_FUNC2_CB_TYPE )
	  {
        timer_client_expired[timer_client_expired_cnt].sig_func_addr	= (void *)cb_details.func_addr;
        timer_client_expired[timer_client_expired_cnt].mask_data		= cb_details.data;
	  }
      else
	  {
	    timer_client_expired[timer_client_expired_cnt].sig_func_addr	= (void *)cb_details.sig_addr;
	    timer_client_expired[timer_client_expired_cnt].mask_data		= cb_details.mask_data2;
	  }


      timer_client_expired[timer_client_expired_cnt].processing_started = cb_processing_started;
      timer_client_expired[timer_client_expired_cnt].processing_time    = cb_processing_time;
      timer_client_expired_cnt = (timer_client_expired_cnt + 1) % MAX_TIMER_CLIENT_EXPIRED;

      if(cb_processing_time > timer_client_processing_time_warning_threshold)
      {
         ATS_ERR_3("callback fn = 0x%x took %d ticks, process = %d",
               cb_details.sig_addr,
               cb_processing_time,
               timer_client_pid);
      }


      if(timer_client_processing_time_err_fatal_threshold != 0 &&
         cb_processing_time > timer_client_processing_time_err_fatal_threshold)
      {
         ERR_FATAL("(timer_cb_info *)=0x%x; notification took %d ticks, process = %d",
               &cb_details,
               cb_processing_time,
               timer_client_pid);
      }
#endif /* FEATURE_TIMER_CLIENT_DEBUG */
   }
} /* TimerCallbackHandler */


/*===========================================================================
DESCRIPTION
   Creates worker thread that waits inside Guest OS for callback details.
   Once the thread gets callback details it will either signal or call the
   callback depending on the callback type.
===========================================================================*/
#define TIMER_PRI 20   /* Todo: Talk to Qurt team, what should be priority of this callback thread.. can it be 10? */
#define TIMER_CLIENT_STACK_SIZE 4096
qurt_thread_t      timer_worker_thread_id;

int CreateWorkerThread(unsigned int process_idx)
{
   int ret_value = 0;
   qurt_thread_attr_t tattr;
   unsigned int stackbase;
   char thread_name[20];
   RCINIT_INFO info_handle = NULL;
   RCINIT_PRIO prio = 0;
   unsigned long stksz = 0;

   snprintf(thread_name, sizeof(thread_name), "TIMER_CLIENT_%u", process_idx);

   /* Look up thread info:
      rcinit_task_prio.csv must be updated as new clients are added */
   info_handle = rcinit_lookup(thread_name);

   if (!info_handle)
   {
      ERR_FATAL("TIMER_CLIENT_%u task info not found",process_idx , 0, 0);
   }
   else
   {
     prio = rcinit_lookup_prio_info(info_handle);
     stksz = rcinit_lookup_stksz_info(info_handle);
     if ((prio > 255) || (stksz == 0))
     {
       ERR_FATAL("Invalid Priority:%d or Stack Size: %d",prio, stksz, 0);
     }

     stackbase = (unsigned int)qurt_malloc(stksz);
     qurt_thread_attr_init (&tattr);
     qurt_thread_attr_set_stack_size (&tattr, (stksz -8));
     qurt_thread_attr_set_stack_addr (&tattr, (void*)((stackbase + 7) & (~7)) );
     qurt_thread_attr_set_priority (&tattr, 255-prio);
     qurt_thread_attr_set_name(&tattr, thread_name);
     ret_value =  qurt_thread_create(&timer_worker_thread_id, &tattr, TimerCallbackHandler, NULL);
  }

   return ret_value;
}  /* CreateWorkerThread() */


#if 0
/*===========================================================================
DESCRIPTION
   Creates Test Thread for testing Qurt timer apis and their delays
===========================================================================*/
#define TIMER_TEST_PRI 100
//unsigned long long timer_test_client_stack[4096];
#define TIMER_CLIENT_TEST_STACK_SIZE 4096
qurt_thread_t      timer_test_id;

/* Defined in timer_client_test.c */
extern void timer_client_test
(
  void *parameter
);

int CreateTestThread(void)
{
   int ret_value = 0;
   qurt_thread_attr_t tattr;

   unsigned int stackbase;

   stackbase = (unsigned int)qurt_malloc(TIMER_CLIENT_TEST_STACK_SIZE);
   qurt_thread_attr_init (&tattr);
   qurt_thread_attr_set_stack_size (&tattr, (TIMER_CLIENT_TEST_STACK_SIZE -8));
   qurt_thread_attr_set_stack_addr (&tattr, (void*)((stackbase +7) &(~7)) );
   qurt_thread_attr_set_priority (&tattr, TIMER_TEST_PRI-1);
   qurt_thread_attr_set_name(&tattr,"TIMER_TEST_CLIENT");
   ret_value =  qurt_thread_create(&timer_test_id, &tattr, timer_client_test, NULL);

   return ret_value;
}  /* CreateTestThread() */
#endif /* #if 0 */

/*===========================================================================

FUNCTION    TIMER_CLIENT_INIT

DESCRIPTION
  Initialize the Timer Client service.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
DLL_API_GLOBAL void timer_client_init(void)
{
   //unsigned int ret;

   timer_client_pid = qurt_getpid();

   timer_client_qdi_handle = qurt_qdi_open(TIMER_DRIVER_NAME);
   if(timer_client_qdi_handle < 0)
   {
      printf("timer_client_init: qdi_open failed");
      ERR_FATAL("timer_client_init :qdi_open failed\n", 0, 0, 0);
      return;
   }

   /* 0 is considered as Guest OS Process ID */
   if(timer_client_pid != 0)
   {
      if(CreateWorkerThread(timer_client_pid) != 0)
      {
          printf("timer_client_init : CreateWorkerThread failed\n");
          ERR_FATAL("timer_client_init : CreateWorkerThread failed\n", 0, 0, 0);
          return;
      }
   }

  /* For testing only */
#if 0
   if(CreateTestThread() != 0)
   {
       printf("qurt_timer_client_init : CreateTestThread failed\n");
       return;
   }
#endif /* #if 0 */

  /* Mark the inernal timer groups as deferrable / non-deferrable */
  timer_group_set_deferrable( &timer_null_group,TRUE );
  timer_group_set_deferrable( &timer_non_defer_group ,FALSE );

   return;
}  /* timer_client_init () */
