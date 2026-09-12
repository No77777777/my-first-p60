/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
       D A T A   S E R V I C E S   A P I  T I M E R

GENERAL DESCRIPTION
  This is the source file for managing the PS timer API. The implementation
  utilizes the Platform specific timer APIs.

  PLEASE NOTE THAT THE FUNCTIONS CAN BE CALLED ONLY FROM A SINGLE TASK
  CONTEXT, CURRENTLY IT IS ONLY PS TASK CONTEXT.


  TERMINOLOGY USED:
    API Timers: The timers that are given to the application.

  STEPS FOR USING THE TIMERS:
    To use the timers do the following steps:
        1. Obtain a timer by calling ps_timer_alloc(). If the timer is
           available in the free pool, then a timer handle is returned, else
           it returns PS_TIMER_FAILURE. Pass the callback function that
           should be called on the expiry of the timer. After the timer
           expires, the callback function is called from the PS task
           context.

        2. After getting a valid timer handle, start the timer by calling
           ps_timer_start(). The time should be given in milli seconds.

        3. If you would like to cancel a particular timer, then use the
           function ps_timer_cancel(). Note that the function does not
           free it, the application still owns the handle.

        4. If you are done with using the timer, call ps_timer_free and
           it will return the timer to the free pool.

        5. If you would like to check if a particular timer is running, then
           use the function, ps_timer_is_running(). This returns TRUE if the
           timer is running.


  State Transitions of the Timer:

        (Starting State)------(function to be called)--------(Ending State)
        FREE -------------------ps_timer_alloc-------------------> STOP

        STOP -------------------ps_timer_start-------------------> RUN
                                with non-zero timer value

        RUN --------------------ps_timer_start ------------------> RUN
                                with non-zero timer value

        RUN --------------------ps_timer_cancel------------------> STOP

        STOP--------------------ps_timer_free--------------------> FREE




RELATIONSHIP BETWEEN PS TASK AND THE TIMER API:
    1. Timer Initializations:
          The API timers are initialized by calling ps_timer_init in
          the PS task.
    2. PS TASK SHOULD BE INITIALIZED BEFORE ANY APPLICATION CALLS THIS TIMER API.
          This is required as PS must also initialize the ps timer array.

RELATIONSHIP BETWEEN PS TASK AND TIMER SERVICES:
   1. The PS task registers the client's timer with REX timer services. When
      the timer expires PS task's timer handler callback will determine if
      the client's callback should be called. If it determines the time
      initially set by the client was longer than the max supported REX
      timer length it will restart another timer for the client automatically.
   2. PS task receives an Asynchronous Procedure Call (APC) from timer services
      in the PS context once the timer has expired.
   3. The maximum supported REX timer length is determined by calling the slow
      clock API. This function determines the number of slow clocks based on
      the current slow clock frequency. PS uses a value of 0xFD000000 of the
      maximum value. This prevents an overflow from occuring when the clock
      frequency changes. Drift of .1% can result in 86 secs over 24 hours if
      the mobile does not have access to network time. Using this percentage
      helps to reduce that drift.


INITIALIZATION AND SEQUENCING REQUIREMENTS
  ps_timer_init() MUST be called before any of the other functions can
  be called and have predictable behaviour.

EXTERNALIZED FUNCTIONS
  Initialization functions:
    1) ps_timer_init(void): Initialize the api timers.

  API Functions to be called by applications using Timer:
    1) ps_timer_alloc():   Function to allocate a timer from free pool.
    2) ps_timer_start():      Start the timer using milliseconds
    3) ps_timer_cancel():     Cancel the timer
    4) ps_timer_free() :      Free the timer
    5) ps_timer_is_running:   Returns if the timer is running or not.


Copyright (c) 2000-2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/pstimer.c_v   1.2   19 Feb 2003 16:17:42   jeffd  $
  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/pstimer.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
10/12/14    ad     ps_timer implementation to mux app timers into 1 sys timer
11/30/11    su     Correcting documentation
09/20/11    bvd    Added free floating changes to pstimer
10/18/11    ss     Reducing highly recurring F3.
03/31/11    ss     Avoid posting PS command while timer free or cancel is in
                   progress.
02/17/11    ss     Cleaning up of Globals for thread safety in Q6 Free
                   Floating environment.
02/14/11    ss     Do not crash in case the timer callback is called after
                   the timer has been freed.
02/09/10    sa     Adding two more timer states START and CANCEL to avoid
                   race condition between timer_expiry and timer_cancel.
01/12/10    hm     Seperate rex timer functionality into another module.
08/16/09    pp     Use timer critical sections for mutual exclusion instead
                   of global PS critical section.
                   Using REX_DIS[EN]ABLE_PREMPTION construct to resolve
                   race condition between timer_expiry and timer_cancel.
11/28/08    pp     Use of PS_ENTER_CRIT_SECTION/UNLOCK macros for better task(s)
                   /interrupt(s) syncronization.
10/24/08    pp     Fixed compiler warning observed in OFFTARGET build.
10/01/08    kcr    Incorrect use of REX_DISABLE_PREMPTION(). Scoping rules corrected.
07/27/06    mct    Fixed window between timer expiration and timer callback
                   execution.
06/14/06    msr    Removed critical section in ps_timer_handler_cb()
06/01/06    mct    Removed critical section around client callback.
05/10/06    mct    Now using a random cookie value for timers.
05/02/06    mct    L4 APCs occur in DPC context. Added cmd to ensure all
                   timer client callbacks occur in PS context.
04/22/06    mct    Reworked entire PS timer library to use new timer.h API.
                   Removed all old pstimer tick functionality. Implemented
                   callback mechanism to support extended timers through
                   REX timers.
02/22/06    msr    Using single critical section
01/31/06    rt     Modified TASKLOCK's.
10/21/05    rt     Added TASKLOCK's.
05/18/05    ssh    Fixed lint errors.
03/21/05    ssh    Changes pursuant to the new typedef ps_timer_handle_type
09/10/04    ks/msr added the value of ps_ms_elapsed_cntr to time_left in
                   ps_timer_start() to avoid early expiration. Moved the
                   start of the tick timer to later in the function.
06/16/04    mvl    changed the timers to use an int64 rather than an int32 to
                   support IPv6 prefix lifetimes.
04/30/04    usb    Assert if we exceed max limit on num required timers.
06/11/03    jd     Added ps_timer_remaining to check time left on timer
02/19/03    jd     Save timer # for MSG_ display in timer handler callback
10/08/02    aku    Removed FEATURE_DATA_MM featurization.
08/02/02    usb    Merged in changes for multimode ps support under
                   FEATURE_DATA_MM
06/06/02    jd     Keep track of elapsed ms instead of ticks, since timer
                   might expire much later than one tick (50ms) as seen in
                   the case of dormancy/mobile ip reregistration.
11/12/01    dwp    Add "|| FEATURE_DATA_PS" to whole module.
08/29/01    jd     timer handler was not calling timer callback until 1+
                   ticks after timer expired. fixed. Added rev. header field.
08/08/01    jd     modified ps_timer_handler to handle multiple timers
                   expiring on the same PS_TIMER_TICK_SIG
06/07/01    jd     Modified the time argument to ps_start_timer() to a sint31
                   so that longer times can be specified.
02/06/01    snn    Modified the code to use the clock services instead of
                   maintaining the timer. This ensures that the timer is
                   more accurate. (specially since PS task has very low
                   priority).
11/27/00   na      Minor cleanup.
11/11/00   snn     Added new callback functions for PS task interface.
                   Removed errno in the functions because it is never used.
                   Modified ps_timer_tick_init for better decoupling of PS
                     task and timer api module.
8/20/00    snn/na  Created this module.
===========================================================================*/

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                          INCLUDE FILES FOR MODULE

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "comdef.h"
#include "customer.h"

#include "ps_svc.h"
#include "ps_crit_sect.h"
#include "pstimer.h"
#include "pstimer_config.h"
#include "ps_platform_timer.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                        LOCAL DECLARATIONS FOR MODULE

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*---------------------------------------------------------------------------
  Added a critical section for ps timers as there is no need to block other
  critical sections while performing timer functions.
---------------------------------------------------------------------------*/
static ps_crit_sect_type ps_timer_crit_section;

/*---------------------------------------------------------------------------
  States of the timer
  FREE:  Timer is available in the pool and can be used. This should be
         initialized to zero, because we use memset to zero initalize the
         ps_timer_list and so, FREE should equal zero.
  STOP:  Timer has been allocated but not running (stopped/never started)
  RUN:   Timer has been started.
---------------------------------------------------------------------------*/
typedef enum
{
  PS_TIMER_FREE   = 0,
  PS_TIMER_STOP   = 1,
  PS_TIMER_RUN    = 2
} ps_timer_state_type;

/*---------------------------------------------------------------------------
  The timer structure has the following fields:
    timer_callback_fcn:
                The function pointer points to the function that will be
                executed when the timer expires.  If this is NULL, no
                callback is called when the timer expires.  It will be
                intialized to NULL during initialization and freeing the
                timer.

    timer_callback_param:
                This contains the parameter to be passed to the callback
                function.

    timer:      This is the rex timer structure that contains the timer
                allocation information, callback, time, etc.

    timer_val:  The time set by the client for the timer.

    timer_state: This holds the state of the timer. If the timer is defined
                but not being used, it is in PS_TIMER_STOP state. If it is
                not yet defined, it is in PS_TIMER_FREE state. If the timer is
                started it is in PS_TIMER_RUN state. If it is cancelled
                it is in PS_TIMER_STOP state.
---------------------------------------------------------------------------*/

typedef struct time_list
{
  boolean              cback_pending;          /* Is there a timer callback pending*/
  uint16               index;                  /* Index of timer array */
  uint64               node_expiry_time;             /* delta time w.r.t to previous
                                                timer node, for first node
                                                it will be same as 
                                                time requested             */
  uint64               time_requested;        /* this is requested time in micro-seconds, */

  struct time_list    *next_ptr;                 /* ptr to the next node       */
  timer_cb_type       *timer_callback_fcn;   /* callback function pointer  */
  void                *timer_callback_param; /* callback function parameter*/
  ps_timer_state_type  timer_state;          /* contains timers state      */
} ps_timer_entry_type;

/*---------------------------------------------------------------------------
  This list contains all the API timers.
---------------------------------------------------------------------------*/
static ps_timer_entry_type *ps_timer_list[PS_TIMER_MAX];

/*---------------------------------------------------------------------------
  This list contains all the API timers.Frequently 
  start/stop ps timers will use this list
---------------------------------------------------------------------------*/
static ps_timer_entry_type *ps_timer_list_ext[PS_TIMER_MAX_EXT];

/*---------------------------------------------------------------------------
  Head pointer to running timer list.
---------------------------------------------------------------------------*/

static ps_timer_entry_type *ps_timer_list_head_ptr = NULL;


/*---------------------------------------------------------------------------
  Handler associated to Rex timer.
---------------------------------------------------------------------------*/

timer_type                  ps_tick_timer_handle;

/*---------------------------------------------------------------------------
  Handler associated to Rex timer. Frequently start/stop ps timers will use this handle.
---------------------------------------------------------------------------*/
timer_type                  ps_tick_timer_handle_ext;

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                         LOCAL FUNCTION DECLARATIONS

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

ps_timer_error_type ps_timeri_start
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len
);

ps_timer_error_type ps_timeri_start_ext
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len
);

int64 ps_timeri_remaining
(
  ps_timer_handle_type timer_handle
);

/*===========================================================================

FUNCTION PS_TIMER_TICK_CB()

DESCRIPTION
  This function will be called by the timer services. The function does the following tasks:
  send a command to PS task to trigger timer call back
       
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_tick_timer_cb
( 
  int32               microseconds,
  timer_cb_data_type  data
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_2("ps_tick_timer_cb():Rex timer cb time %ld cb data %ld", microseconds, data);

  /*----------------------------------------------------------------------
   Send a cmd to PS task to process the client callbacks in PS context.
   Protected within timer critical section to avoid posting command while
   timer cancel or timer free is in progress.
  ----------------------------------------------------------------------*/
  ps_send_cmd (PS_TIMER_CALLBACK_CMD, (void*)data);
  
} /* ps_tick_timer_cb() */

/*===========================================================================

FUNCTION PS_TIMER_TICK_CB_EXT()

DESCRIPTION
  This function will be called by the timer services. The function does the following tasks:
  send a command to PS task to trigger timer call back
       
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_tick_timer_cb_ext
( 
  int32               microseconds,
  timer_cb_data_type  data
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_2("ps_tick_timer_cb_ext():Rex timer cb time %ld cb data %ld", microseconds, data);

  /*----------------------------------------------------------------------
   Send a cmd to PS task to process the client callbacks in PS context.
   Protected within timer critical section to avoid posting command while
   timer cancel or timer free is in progress.
  ----------------------------------------------------------------------*/
  ps_send_cmd (PS_TIMER_CALLBACK_EXT_CMD, (void*)data);
  
} /* ps_tick_timer_cb() */


/*===========================================================================

FUNCTION PS_TIMERI_TICK_STOP()

DESCRIPTION
  
  This function called by the pstimer module to stop a tick Timer. 
  This function calls timer_clr to deregister the call back. 
    
  This function has been made a callback function because, this ensures
  that pstimer module is decoupled from ps task. (one reason is if we
  decide to move the tick_timer to any other task, the changes should
  not effect the pstimer module).
  
DEPENDENCIES
  Tick Timer should have been initialized before calling this function.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ps_timeri_tick_stop( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_platform_timer_clr (&ps_tick_timer_handle);

} /* ps_timeri_tick_stop_cb() */ 

/*===========================================================================

FUNCTION PS_TIMERI_TICK_START()

DESCRIPTION
  This is a callback function called by the pstimer module to start a tick
  timer. This function calls the clk_reg, to register the call back structure
  and also reset the ticks_elapsed_cntr to 0 here. Reseting the counter here
  ensures that every time we register the clock callback, the counter 
  restarts.
  
  This function has been made a callback function because, this ensures
  that pstimer module is decoupled from ps task. (one reason is if we
  decide to move the tick_timer to any other task, the changes should
  not effect the pstimer module).
  

DEPENDENCIES
  ps_tick_timer_handle should have been initialized before calling this 
  function. The initialization should be done by calling clk_def().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ps_timeri_tick_start(uint64 ps_tick_interval)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_2("ps_timeri_tick_start():Rex Timer start %ld, Timer list Head ptr  0x%x",
                           ps_tick_interval, ps_timer_list_head_ptr);

  ps_platform_timer_set(&ps_tick_timer_handle, 
                         ps_tick_interval, 
                         ps_timer_list_head_ptr);

} /* ps_timeri_tick_start() */

/*===========================================================================
FUNCTION PS_TIMERI_REMOVE()

DESCRIPTION
  This function removes a timer from the  list and return tick value if removed node
  is first node in the list

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Timer list is modified.
===========================================================================*/
static boolean ps_timeri_remove
(
  ps_timer_entry_type *timer,
  uint64              *ps_timer_tick_value
)
{
   boolean                tick_timer_start = FALSE;
   int64                  remaining_time   = 0;
   ps_timer_entry_type   *curr_timer       = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == timer)
  {
    return FALSE;
  }

  LOG_PSTIMER_MSG_INFO1_2("ps_timeri_remove(): Timer list Head ptr 0x%x, Timer 0x%x",
    ps_timer_list_head_ptr, timer);
  
  curr_timer = ps_timer_list_head_ptr;
  
  /*-------------------------------------------------------------------------
  If given timer is first timer in the list then remove it from the list and take below action
  
    > If there is no next node then expected action is to stop current timer only
    > if there is next node then add remain time into next node and start new timer
   -------------------------------------------------------------------------*/

  if (timer == curr_timer)
  {
    if (NULL == timer->next_ptr)
    {
      *ps_timer_tick_value = 0;
      
    }
    else
    {
      remaining_time = ps_platform_timer_get( &ps_tick_timer_handle );

      LOG_PSTIMER_MSG_INFO1_3("ps_timeri_remove(): remaining time %ld, "
        "curr node expire %ld, next node expire %ld", 
         remaining_time, timer->node_expiry_time,
         timer->next_ptr->node_expiry_time);
      
      timer->next_ptr->node_expiry_time += remaining_time;

      *ps_timer_tick_value =  timer->next_ptr->node_expiry_time;

    }
    /*-------------------------------------------------------------------------
    Update the head pointer to point next node in the list
     -------------------------------------------------------------------------*/
    ps_timer_list_head_ptr = timer->next_ptr;
     
    tick_timer_start  = TRUE;

    LOG_PSTIMER_MSG_INFO1_2("ps_timeri_remove():start rex timer %d Timer list Head ptr 0x%x", 
      tick_timer_start, ps_timer_list_head_ptr);
  
  }
  else
  { 
    /*-------------------------------------------------------------------------
    If Given timer is last node in the list then no need to update next expiry timer
       -------------------------------------------------------------------------*/
    if (NULL != timer->next_ptr)
    {
      timer->next_ptr->node_expiry_time += timer->node_expiry_time;
    }
    
    /*-------------------------------------------------------------------------
    Traverse to the list and update the link
       -------------------------------------------------------------------------*/

    while ( curr_timer != NULL && curr_timer->next_ptr != timer)
    {
      LOG_PSTIMER_MSG_INFO1_1("ps_timeri_remove(): Timer Node Ptr  0x%x", curr_timer);
      curr_timer = curr_timer->next_ptr;
    }
    
    LOG_PSTIMER_MSG_INFO1_1("ps_timeri_remove():  Timer  0x%x", curr_timer);
    
    if (NULL !=  curr_timer && NULL != curr_timer->next_ptr)
    {
      curr_timer->next_ptr = curr_timer->next_ptr->next_ptr;
    }
    
  }
  /*-------------------------------------------------------------------------
    Make sure this timer is not attached to anything.
  -------------------------------------------------------------------------*/
  timer->next_ptr = NULL;
  
  /*-------------------------------------------------------------------------
    Update state of the timer
  -------------------------------------------------------------------------*/
  timer->timer_state      = PS_TIMER_STOP;
  timer->node_expiry_time = 0;
  timer->time_requested   = 0;

  LOG_PSTIMER_MSG_INFO1_3("ps_timeri_remove(): time %ld Rex timer start %ld, Timer list Head Ptr 0x%x", 
    *ps_timer_tick_value, tick_timer_start, ps_timer_list_head_ptr);
   
  return tick_timer_start;

} /* ps_timeri_remove() */


/*===========================================================================
FUNCTION PS_TIMERI_INSERT()

DESCRIPTION
  This function inserts new timer into the timer list. Function will return tick value and flag to start
  new tick value

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successful else FALSE

SIDE EFFECTS
  Timer list is modified.
===========================================================================*/
static boolean ps_timeri_insert
(
  ps_timer_entry_type  *timer,
  uint64               *ps_timer_tick_value
)
{
  ps_timer_entry_type   *curr_timer       = NULL;
  ps_timer_entry_type   *prev_timer       = NULL;
  boolean               tick_timer_start = FALSE;
  uint64                remaining_time   = 0;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == timer) || (NULL == ps_timer_tick_value))
  {
    return tick_timer_start;
  }
  
  /*-------------------------------------------------------------------------
  Set NULL into new node links and update later based on position in the link list
  -------------------------------------------------------------------------*/
  timer->next_ptr = NULL;
  
  curr_timer  = ps_timer_list_head_ptr;
  
  LOG_PSTIMER_MSG_INFO1_2("ps_timeri_insert():Timer Ptr 0x%x,Timer list Head ptr 0x%x",
    timer, ps_timer_list_head_ptr);
  
  if ( NULL == curr_timer )
  {
    /*-------------------------------------------------------------------------
    Update the tick value and set start flag to start new timer. Update Head pointer
    -------------------------------------------------------------------------*/
    *ps_timer_tick_value   = timer->time_requested;
    
    tick_timer_start       = TRUE;

    ps_timer_list_head_ptr = timer;
  }
  else
  {
    /*-------------------------------------------------------------------------
    Clients can start the timer within CB function before we start next timer.
    Hence check the timer state before calculating remaining time 
    Temproray changes and should be revert back after FR.
    -------------------------------------------------------------------------*/ 
    if (TRUE == ps_platform_timer_is_active(&ps_tick_timer_handle))
    {
      remaining_time = ps_platform_timer_get ( &ps_tick_timer_handle );
      LOG_PSTIMER_MSG_INFO1_1("ps_timeri_insert(): Remaining Time(Timer Active) %ld ", remaining_time);
    }
    else
    {
       remaining_time = curr_timer->node_expiry_time;
       LOG_PSTIMER_MSG_INFO1_1("ps_timeri_insert(): Remaining Time(Timer not Active) %ld ", remaining_time);
    }
    
    /*-------------------------------------------------------------------------
    If remaining time is greater then upcoming node then stop current timer and instart back
    into list with diff of remain time a. Start timer again with new node time value
    -------------------------------------------------------------------------*/
    if (timer->time_requested < remaining_time)
    {
        LOG_PSTIMER_MSG_INFO1_5("ps_timeri_insert():Before curr node expiry time %ld,"
          " remaining time %ld, time requested %ld "
          " Timer Requested time %ld, Timer expire timer %ld",
            curr_timer->node_expiry_time, remaining_time,
            curr_timer->time_requested, timer->time_requested,
            timer->node_expiry_time);

      *ps_timer_tick_value = timer->time_requested;
     
      tick_timer_start     = TRUE;
      /* -------------------------------------------------------------------------
           * Update current running node expiry time and insert new node into linked list
          -------------------------------------------------------------------------*/
      curr_timer->node_expiry_time = (remaining_time - timer->time_requested);

      LOG_PSTIMER_MSG_INFO1_1("ps_timeri_insert():After curr node expiry time %ld",
                               curr_timer->node_expiry_time);
      
      timer->next_ptr = curr_timer;
      /*-------------------------------------------------------------------------
      update the Head pointer to point new node 
      -------------------------------------------------------------------------*/
      ps_timer_list_head_ptr = timer;
      
    }
    else
    {
      /*-------------------------------------------------------------------------
      Run a loop till either we found a correct place to instert the node or list become empty
      -------------------------------------------------------------------------*/
      while (NULL != curr_timer)
      {
        /*-----------------------------------------------------------------------
        ONLY insert if the time left on the next node is greater than - thus if
        it is equal it will be inserted AFTER - this maintains FIFO ordering on
        dispatching timer callbacks.
        Note: For first node we should check with remain time  There could be case where new timer
        could greater then remain time and less then node expire time
        -----------------------------------------------------------------------*/
         
        if (timer->node_expiry_time <( (curr_timer == ps_timer_list_head_ptr) 
                           ? remaining_time : curr_timer->node_expiry_time))
        {
        
          LOG_PSTIMER_MSG_INFO1_5("ps_timeri_insert(): Pre Curr timer 0x%x, node expiry %ld,"
            " time requested %ld, Timer requested %ld,  expiry %ld",
              curr_timer, curr_timer->node_expiry_time, curr_timer->time_requested,
              timer->time_requested, timer->node_expiry_time);

          /*-------------------------------------------------------------------
          Break when ever there is a node with time greater than incoming  timer node
          update  node expire value based on new node requested time
           --------------------------------------------------------------------*/

          curr_timer->node_expiry_time = curr_timer->node_expiry_time - timer->node_expiry_time;
           
          LOG_PSTIMER_MSG_INFO1_1("ps_timeri_insert():Post curr node expire 0x%x", curr_timer->node_expiry_time);
          break;
        }
        else
        {

          LOG_PSTIMER_MSG_INFO1_5("ps_timeri_insert(): Pre Curr timer 0x%x, node expiry %ld,"
            " time requested %ld, Timer requested %ld,  expiry %ld",
              curr_timer, curr_timer->node_expiry_time, curr_timer->time_requested,
              timer->time_requested, timer->node_expiry_time);
          /*-----------------------------------------------------------------------
          for the first node  we need to compare  with the remaining_time and for rest of the nodes, 
          we need to compare with node_expiry_time in the node
           -----------------------------------------------------------------------*/
          if (curr_timer == ps_timer_list_head_ptr)
          {
            timer->node_expiry_time -= remaining_time;
          }
          else
          {
            timer->node_expiry_time -= curr_timer->node_expiry_time;
          }

          LOG_PSTIMER_MSG_INFO1_1("ps_timeri_insert():Post curr node expire 0x%x",
            curr_timer->node_expiry_time);
          
          prev_timer = curr_timer;
          
          curr_timer = curr_timer->next_ptr;
          
        }
      } 
      /*-------------------------------------------------------------------------
      we are now pointing to the node after which we want to insert the timer
      so insert it.
      -------------------------------------------------------------------------*/
      if (NULL != prev_timer)
      {

        timer->next_ptr      = prev_timer->next_ptr;      
        prev_timer->next_ptr = timer;
      }
      
    }
  }
  /*-------------------------------------------------------------------------
    Update state of the timer
  -------------------------------------------------------------------------*/
  timer->timer_state = PS_TIMER_RUN;

  LOG_PSTIMER_MSG_INFO1_6("ps_timeri_insert(): Timer value %ld Rex timer start %ld Timer node expire %ld"
    " Timer list Head Ptr 0x%x Timer 0x%x Next Timer 0x%x", 
      *ps_timer_tick_value, tick_timer_start, timer->node_expiry_time,
      ps_timer_list_head_ptr, timer,  timer->next_ptr);

    
  return tick_timer_start;

} /* ps_timeri_insert() */
/*===========================================================================
FUNCTION PS_TIMER_INDEX_TO_HANLDE()

DESCRIPTION
  This function converts the index passed into the client handler of the timer.
  Checks if the handle passed is valid or not. If it is not valid, it returns
  FALSE; If it is valid, it returns TRUE.

DEPENDENCIES
  This should be called only after a valid timer handle is obtained by
  calling ps_timer_alloc_timer

PARAMETERS
  index:           index to array of timers.

RETURN VALUE

  FALSE: if timer index is not valid
  TRUE:  If passed index is valid.

SIDE EFFECTS
  None
===========================================================================*/
static boolean ps_timer_index_to_handle
(
  uint32                 index,
  ps_timer_handle_type  *timer_handle
)
{

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 if (NULL == timer_handle || index >= PS_TIMER_MAX)
 {
   return FALSE;
 }
 
   /*-------------------------------------------------------------------------
    Array index would be from 0 to MAX -1 
  -------------------------------------------------------------------------*/

  *timer_handle = index + 1;

  return TRUE;
}/*ps_timer_index_to_handle*/

/*===========================================================================
FUNCTION PS_TIMER_HANDLE_TO_INDEX()

DESCRIPTION
  This function converts the handler passed into the  index of the timer.
  Checks if the handle passed is valid or not. If it is not valid, it returns
  FALSE; If it is valid, it returns TRUE.

DEPENDENCIES
  This should be called only after a valid timer handle is obtained by
  calling ps_timer_alloc_timer

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE

  FALSE: If timer handle is not valid
  TRUE:  If passed handle is valid.

SIDE EFFECTS
  None
===========================================================================*/
static boolean ps_timer_handle_to_index
(
  ps_timer_handle_type  timer_handle,
  uint32                *index
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == index || 0 == timer_handle || PS_TIMER_MAX < timer_handle )
  {
    LOG_PSTIMER_MSG_INFO1_1("ps_timeri_handle_to_timer(): Invalid handler 0x%x",
                    timer_handle);
    return FALSE;
  }
  
   /*-------------------------------------------------------------------------
    handles would be from 1 to MAX.
  -------------------------------------------------------------------------*/
  
  *index = timer_handle - 1;

  return TRUE;
}/*ps_timer_handle_to_index*/

/*===========================================================================
FUNCTION PS_TIMERI_HANDLE_TO_TIMER()

DESCRIPTION
  This function converts the handle passed into the address of the timer.
  Checks if the handle passed is valid or not. If it is not valid, it returns
  FALSE; If it is valid, it returns TRUE.

DEPENDENCIES
  This should be called only after a valid timer handle is obtained by
  calling ps_timer_alloc_timer

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE

  NULL: if timer handle is not valid
  address:  If passed handle is valid.

SIDE EFFECTS
  None
===========================================================================*/
static ps_timer_entry_type *ps_timeri_handle_to_timer
(
  ps_timer_handle_type  timer_handle
)
{
  uint32                   index    = 0;
  ps_timer_entry_type     *ps_timer = NULL;  /* Ptr to the ps_timer entry  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Confirm the timer handle is valid and derive the timer index.
  -------------------------------------------------------------------------*/
  if (TRUE == ps_timer_handle_to_index (timer_handle, &index))
  {
    ps_timer = ps_timer_list[index] ;
  }

  /*-------------------------------------------------------------------------
    Caller will check the NULL pointer and will handle gracefully.
  -------------------------------------------------------------------------*/

  return ps_timer;

} /* ps_timeri_handle_to_timer */


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                       REGIONAL DEFINITIONS FOR MODULE

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*===========================================================================
FUNCTION PS_TIMERI_DISPATCH_CLIENT_CB()

DESCRIPTION
  This function is called in PS context to execute the client timer callbacks.

PARAMETERS
  cmd:               The PS_TIMER_CALLBACK_CMD called from ps_task().
  user_data_ptr:     The assigned ps_timer handle.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void ps_timeri_dispatch_client_cb 
(
  ps_cmd_enum_type   cmd,
  void              *user_data_ptr
)
{
  boolean              tick_timer_restart   = FALSE;
  uint16               timers_count         = 0;
  uint16               num_timer_expired    = 0;
  ps_timer_entry_type *ps_timer             = NULL;
  uint64               elapsed_ms           = 0;
  uint64               ps_timer_tick_value  = 0;
  uint16               ps_timer_array_indexes[PS_TIMER_MAX];
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_ASSERT (cmd == PS_TIMER_CALLBACK_CMD);
  
  memset(&ps_timer_array_indexes, 0x0, sizeof(ps_timer_array_indexes));
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  
  /*-------------------------------------------------------------------------
    Avoid Race condition between removing first node and call back processing.
    e.g. client cancel the timer at same time timer fired. tick_call_back was called
    in time task context while timer removal happen in PS context 
  -------------------------------------------------------------------------*/
  if (NULL == ps_timer_list_head_ptr)
  {
    PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
    return;
  }
  /*-------------------------------------------------------------------------
  Check if first node get removed from the list while call back was panding to process
  -------------------------------------------------------------------------*/
  if((ps_timer_entry_type *)user_data_ptr != ps_timer_list_head_ptr)
  {       
    if(0 == ps_timer_list_head_ptr->node_expiry_time)
    {
      elapsed_ms = 0;
    }
    else
    {
      PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
      return;
    }
  }
  else
  {
    elapsed_ms = ps_timer_list_head_ptr->node_expiry_time;
  }
  
  /*-------------------------------------------------------------------------
    Reduce the timer. The value by which we reduce the timer is given by
    elapsed_ms. It's ok for node_expiry_time to be negative. remove timer will
    subtract the extra time from the next timer in the list.
  -------------------------------------------------------------------------*/
  LOG_PSTIMER_MSG_INFO1_3("ps_timeri_dispatch_client_cb(): Node expiry_time: %ld requestd: %ld"
                  "elapsed_ms: %ld", ps_timer_list_head_ptr->node_expiry_time,
                   ps_timer_list_head_ptr->time_requested, elapsed_ms);

  ps_timer_list_head_ptr->node_expiry_time -= elapsed_ms;

  /*-----------------------------------------------------------------------
    After reducing the timer, if the timer has not expired, then return back, because,
    we have nothing else to do.e.g  First timer got removed from the list while 
    tick_timer_cb is in task queue to process.In that case we should ignore and 
    wait for next timer to expire
  -----------------------------------------------------------------------*/
  while ( (ps_timer_list_head_ptr != NULL) && 
          (ps_timer_list_head_ptr->node_expiry_time <= 0) &&
          (num_timer_expired < PS_TIMER_MAX)) 
  {

      /*-----------------------------------------------------------------------
        Store index into local array. Local array will be used to validate and invoke clients CB 
      -----------------------------------------------------------------------*/
    ps_timer_array_indexes[num_timer_expired++] = 
      ps_timer_list_head_ptr->index;

    ps_timer_list_head_ptr->cback_pending = TRUE;
      /*-----------------------------------------------------------------------
        Remove the timers from the list.  ps_timer_list is a doubly linked 
        list and ps_timeri_remove will modify the timers on either side of 
        the one being removed to point at each other.

        NOTE: We should remove the timer before calling the call back function 
        because, the callback function can start the same timer, and we should
        not be removing it.
      -----------------------------------------------------------------------*/
    tick_timer_restart = ps_timeri_remove (ps_timer_list_head_ptr, &ps_timer_tick_value);

    /*-----------------------------------------------------------------------
        Restart the timer for non zero tick value only.
        e.g first few timer can have same requested time and will expire at same time
       -----------------------------------------------------------------------*/
       
    if (TRUE == tick_timer_restart && 0!= ps_timer_tick_value )
    {
      break;
    }
  }
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);  
  
  /*-----------------------------------------------------------------------
    Before calling a callback confirm that the callback is not NULL. Then, 
    dispatch all the timers that are ready, by calling their callbacks. Do 
    this until all the timers with delta values equal to zero as head of 
    the list.
  -----------------------------------------------------------------------*/
  while (timers_count < num_timer_expired)
  {
    ps_timer = ps_timer_list[ps_timer_array_indexes[timers_count]];
    
    if ( NULL != ps_timer && TRUE == ps_timer->cback_pending)
    {
      ps_timer->cback_pending = FALSE;
      
      ps_timer->timer_callback_fcn(ps_timer->timer_callback_param);
      
      LOG_PSTIMER_MSG_INFO1_0("ps_timeri_dispatch_client_cb(): call back called ");
    }
    timers_count++;
  }
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  
  /*-----------------------------------------------------------------------
     Restart next timer.
  -----------------------------------------------------------------------*/
  if ( (TRUE == tick_timer_restart) && 
       (0    != ps_timer_tick_value) && 
       (NULL != ps_timer_list_head_ptr) )
  {
    /*-----------------------------------------------------------------------
      Note :: Clients can insert timer within call back hence reevaluate the first node 
              in the list and start the timer. (CR Number: 806371)   
    -----------------------------------------------------------------------*/
    ps_timer_tick_value = ps_timer_list_head_ptr->node_expiry_time;

    if(FALSE == ps_platform_timer_is_active(&ps_tick_timer_handle))
    {
      ps_timeri_tick_start ( ps_timer_tick_value);
    }
  }
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
    
  return;
} /* ps_timeri_dispatch_client_cb() */

/*===========================================================================
FUNCTION PS_TIMERI_DISPATCH_CLIENT_CB_EXT()

DESCRIPTION
  This function is called in PS context to execute the client timer callbacks.

PARAMETERS
  cmd:               The PS_TIMER_CALLBACK_EXT_CMD called from ps_task().
  user_data_ptr:     The assigned ps_timer handle.

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void ps_timeri_dispatch_client_cb_ext 
(
  ps_cmd_enum_type   cmd,
  void              *user_data_ptr
)
{
  uint16      index = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_UTILS_ASSERT (cmd == PS_TIMER_CALLBACK_EXT_CMD);
  
  if (NULL == ps_timer_list_ext[index])
  {
    return;
  }
  
  ps_timer_list_ext[index]->timer_state =  PS_TIMER_STOP;
  ps_timer_list_ext[index]->timer_callback_fcn(ps_timer_list_ext[index]->timer_callback_param);

}/* ps_timeri_dispatch_client_cb_ext */

/*===========================================================================
FUNCTION PS_TIMER_INIT()

DESCRIPTION
  This function initializes the timer data structures.
    timer_callback_fcn:   Initialize to NULL
    timer_callback_param: Initialize to NULL
    timer_state:          Initialize to PS_TIMER_FREE

  the max time that timers can support without significant drift.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the ps_timer_list structure contents.

===========================================================================*/
void ps_timer_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initialize the ps_timer_crit_section
  -------------------------------------------------------------------------*/
  PS_INIT_CRIT_SECTION(&ps_timer_crit_section);

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  /*-------------------------------------------------------------------------
    Initialize the timer fields to default values.

    All the pointers are initialized to NULL.
    The timer state should be made as FREE, since it can be used.
    The Time value is made as zero.
  -------------------------------------------------------------------------*/
  memset( ps_timer_list, 0x0, sizeof(ps_timer_list));
  memset(ps_timer_list_ext, 0x0, sizeof(ps_timer_list_ext));

  memset( &ps_tick_timer_handle, 0x0, sizeof(ps_tick_timer_handle));
  memset( &ps_tick_timer_handle_ext, 0x0, sizeof(ps_tick_timer_handle_ext));

  /*-------------------------------------------------------------------------
    Initialize the timer used by timer service.
  -------------------------------------------------------------------------*/  
  ps_platform_timer_def ((void *)&ps_tick_timer_handle,
                         (void *)ps_tick_timer_cb,
                          NULL);
  
  /*-------------------------------------------------------------------------
    Initialize the timer used by timer service.
    Frequently start/stop ps timers will use this handle.
  -------------------------------------------------------------------------*/  
  ps_platform_timer_def ((void *)&ps_tick_timer_handle_ext,
                         (void *)ps_tick_timer_cb_ext,
                          NULL);
  
  (void) ps_set_cmd_handler(PS_TIMER_CALLBACK_CMD, ps_timeri_dispatch_client_cb);
  (void) ps_set_cmd_handler(PS_TIMER_CALLBACK_EXT_CMD, ps_timeri_dispatch_client_cb_ext);

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

} /* ps_timer_init() */

/*===========================================================================
FUNCTION PS_TIMER_DEINIT()

DESCRIPTION
  This function clean-up the resource associated with timer data structures.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Changes the ps_timer_list structure contents.

===========================================================================*/
void ps_timer_deinit
(
  void
)
{
  uint32               loop         = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_platform_timer_free(&ps_tick_timer_handle);

  ps_platform_timer_free(&ps_tick_timer_handle_ext);
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  
  for (loop = 0; loop < PS_TIMER_MAX; loop++ )
  {
    if (NULL != ps_timer_list[loop])
    {
       PS_SYSTEM_HEAP_MEM_FREE (ps_timer_list[loop]);
    }
  }
  for (loop = 0; loop < PS_TIMER_MAX_EXT; loop++ )
  {
    if (NULL != ps_timer_list_ext[loop])
    {
       PS_SYSTEM_HEAP_MEM_FREE (ps_timer_list_ext[loop]);
    }
  }
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
  
  PS_DESTROY_CRIT_SECTION(&ps_timer_crit_section);

}

/*===========================================================================
FUNCTION PS_TIMER_ALLOC

DESCRIPTION
  This function allocates a timer. Before any task uses the timer, it should
  first acquire a timer by calling alloc_timer. This function returns a timer
  handle to the caller. This timer handle can be used in the future to access
  this timer (until it is freed).

  Every timer may have a callback associated with it.  So, it takes the
  function callback as an argument.  The callback function must have the
  following signature: void fname(void *);  If NULL is specified as the
  callback parameter, Error will  return.

DEPENDENCIES
  After the timer is used, call FREE_TIMER. This will return the timer back
  to the free pool.

PARAMETERS
  callback  :The callback function to call when the timer expires.
  cb_param  :The parameter to pass back in the client callback.

RETURN VALUE
   Success: A non-zero handle
   Failure: PS_TIMER_INVALID_HANDLE

SIDE EFFECTS
  None
===========================================================================*/
ps_timer_handle_type ps_timer_alloc
(
  void (* callback) (void *),
  void *cb_param
)
{
  ps_timer_handle_type timer_handle = PS_TIMER_INVALID_HANDLE;
  ps_timer_entry_type *ps_timer     = NULL;
  uint32               loop         = 0;

  /*-------------------------------------------------------------------------
    index is used to allocate the timer handle. It stores the value of the
    of the index allocated previously. Hence, its made as static. This allows
    application to allocate different indices every time a new timer is
    requested. Even if a timer is allocated and freed alternatively, we
    ensure that different handles are returned.
  -------------------------------------------------------------------------*/
  static uint16  index  = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(NULL == callback)
  {
    LOG_MSG_ERROR_0("ps_timer_start(): NULL timeout call bak");
    return PS_TIMER_INVALID_HANDLE;
  }
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  /*-------------------------------------------------------------------------
    Find out if there are timers available.

    If there are no free timers available then return PS_TIMER_FAILURE

    Note that we use loop and index to obtain the timer handle. This is
    useful because, it ensures that we always allocate different timer
    handles when more than one free timer exists.
  -------------------------------------------------------------------------*/
  for (loop = 0; loop < PS_TIMER_MAX; loop++ )
  {
    /*-----------------------------------------------------------------------
      If the index reaches its maximum, then wrap around to make it
      initial value.
    -----------------------------------------------------------------------*/
    
    if (PS_TIMER_MAX == index )
    {
      index = 0;
    }

    /*---------------------------------------------------------------------
      Find out if any of the timers are in free pool. If yes, then allocate new timer block
    ---------------------------------------------------------------------*/
    if (index < PS_TIMER_MAX && NULL == ps_timer_list[index])
    {

      PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(ps_timer, sizeof(ps_timer_entry_type),
                                   ps_timer_entry_type*);
      if (NULL != ps_timer)
      {
        ps_timer->timer_state          = PS_TIMER_STOP;
        ps_timer->node_expiry_time     = 0;
        ps_timer->time_requested       = 0;
        ps_timer->timer_callback_fcn   = callback;
        ps_timer->timer_callback_param = cb_param;
        ps_timer->next_ptr                 = NULL;
        ps_timer->index                = index;
        ps_timer->cback_pending        = FALSE;
      
        /*---------------------------------------------------------------------
          Store new allocated timer into global timer list
        ---------------------------------------------------------------------*/
        ps_timer_list[index] =  ps_timer;
      
        (void)ps_timer_index_to_handle (index, &timer_handle);
       
        DS_UTILS_ASSERT( timer_handle != PS_TIMER_INVALID_HANDLE);

        index++;
      }
      
      break;
    }
    else
    {
      /*---------------------------------------------------------------------
      Move to next index in the timer alloc list
      ---------------------------------------------------------------------*/
      index++;
    }
    
  }

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  LOG_PSTIMER_MSG_INFO1_3(" ps_timer_alloc(): Allocated timer handle 0x%x,"
                          " CB Register 0x%x, Timer 0x%x",
                            timer_handle, callback, ps_timer);
  return timer_handle;

} /* ps_timer_alloc() */

ps_timer_handle_type ps_timer_alloc_ext
(
  void (* callback) (void *),
  void *cb_param
)
{
  ps_timer_handle_type timer_handle = PS_TIMER_INVALID_HANDLE;
  ps_timer_entry_type *ps_timer     = NULL;
  static uint16        index        = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if(NULL == callback)
  {
    LOG_MSG_ERROR_0("ps_timer_alloc_ext(): NULL timeout call bak");
    return PS_TIMER_INVALID_HANDLE;
  }
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  if(PS_TIMER_MAX_EXT == index)
  {
    PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
    return PS_TIMER_INVALID_HANDLE;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(ps_timer, sizeof(ps_timer_entry_type),
                               ps_timer_entry_type*);
  if (NULL != ps_timer)
  {
    ps_timer->timer_state          = PS_TIMER_STOP;
    ps_timer->node_expiry_time     = 0;
    ps_timer->time_requested       = 0;
    ps_timer->timer_callback_fcn   = callback;
    ps_timer->timer_callback_param = cb_param;
    ps_timer->next_ptr             = NULL;
    ps_timer->index                = index;
    ps_timer->cback_pending        = FALSE;
  
    /*---------------------------------------------------------------------
      Store new allocated timer into global timer list
    ---------------------------------------------------------------------*/
    ps_timer_list_ext[index] =  ps_timer;
  
    (void)ps_timer_index_to_handle (index, &timer_handle);
   
    DS_UTILS_ASSERT( timer_handle != PS_TIMER_INVALID_HANDLE);
  
    index++;
  }

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
  
  LOG_PSTIMER_MSG_INFO1_3(" ps_timer_alloc_ext(): Allocated timer handle 0x%x,"
                          " CB Register 0x%x, Timer 0x%x",
                            timer_handle, callback, ps_timer);
  return timer_handle;
}/*ps_timer_alloc_ext*/

/*===========================================================================
FUNCTION PS_TIMER_START()

DESCRIPTION
  This function takes a timer handle, the time in milli-seconds for the timer.

DEPENDENCIES
  If this function is called for the first time, then we start the timer
  tick. Since, timer tick is initialized in PS task context, the PS task
  should already be initialized before calling this function. One simple way
  to confirm this is, to check the priority of the PS task and the task
  calling this function. If PS Task priority is higher than calling task,
  then we are safe.

PARAMETERS
  handle:           The assigned ps_timer handle.
  timer_len:        The length of the timer(in ms) to set.

RETURN VALUE
  PS_TIMER_FAILURE: if there is an error setting the timer.
  PS_TIMER_SUCCESS: if the setting is success.

SIDE EFFECTS
  If the incoming timer request is less than the first timer in the list,
  then a timer tick is re-started with value of MIN(incoming timer value, 
  remaining time in the timer node 1)
  If there is already a timer running with the same handle then it 
  will be stopped and re-started
===========================================================================*/
ps_timer_error_type ps_timer_start
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len 
)
{
  ps_timer_error_type   ret_val;
  uint64                time_in_micro_second = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_2("ps_timer_start(): Timer Handle 0x%x, timeout %ld ms",
                           timer_handle, timer_len);
  
  /*-----------------------------------------------------------------------
     PS timer support  MAX 0xFFFFFFFFFFFFF ms (4503599627370495 ms == 1250999896 Hours )
   First three byte reserved for millisecond to microsecond conversion
  -----------------------------------------------------------------------*/
  if (0 == timer_len || (timer_len & 0xFFF0000000000000))
  {
    LOG_MSG_ERROR_2("ps_timer_start(): Invalid timeout %ld, Timer 0x%x",
                        timer_len, timer_handle);
    return PS_TIMER_FAILURE;
  }
  
  /*-----------------------------------------------------------------------
  Convert millisecond time into micro second. PS Timer internal APIs work/support microsecond.
  -----------------------------------------------------------------------*/
  time_in_micro_second = timer_len * 1000;
  
  ret_val = ps_timeri_start(timer_handle, time_in_micro_second);
    
  return ret_val;
}

ps_timer_error_type ps_timer_start_ext
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len 
)
{
  ps_timer_error_type   ret_val;
  uint64                time_in_micro_second = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  LOG_PSTIMER_MSG_INFO1_2("ps_timer_start_ext(): Timer Handle 0x%x, timeout %ld ms",
                           timer_handle, timer_len);
  
  /*-----------------------------------------------------------------------
    PS timer support  MAX 0xFFFFFFFFFFFFF ms (4503599627370495 ms == 1250999896 Hours )
  First three byte reserved for millisecond to microsecond conversion
 -----------------------------------------------------------------------*/
  if (0 == timer_len || (timer_len & 0xFFF0000000000000))
  {
    LOG_MSG_ERROR_2("ps_timer_start_ext(): Invalid timeout %ld, Timer 0x%x",
                        timer_len, timer_handle);
    return PS_TIMER_FAILURE;
  }
  
  /*-----------------------------------------------------------------------
 Convert millisecond time into micro second. PS Timer internal APIs work/support microsecond.
 -----------------------------------------------------------------------*/
  time_in_micro_second = timer_len * 1000;
  
  ret_val = ps_timeri_start_ext(timer_handle, time_in_micro_second);
    
  return ret_val;

}/* ps_timer_start_ext */

/*===========================================================================
FUNCTION PS_TIMERI_START()

DESCRIPTION
  This function takes a timer handle, the time in micro-seconds for the timer.

DEPENDENCIES
  If this function is called for the first time, then we start the timer
  tick. Since, timer tick is initialized in PS task context, the PS task
  should already be initialized before calling this function. One simple way
  to confirm this is, to check the priority of the PS task and the task
  calling this function. If PS Task priority is higher than calling task,
  then we are safe.

PARAMETERS
  handle:           The assigned ps_timer handle.
  timer_len:        The length of the timer(in micro second) to set.

RETURN VALUE
  PS_TIMER_FAILURE: if there is an error setting the timer.
  PS_TIMER_SUCCESS: if the setting is success.

SIDE EFFECTS
  If the incoming timer request is less than the first timer in the list,
  then a timer tick is re-started with value of MIN(incoming timer value, 
  remaining time in the timer node 1)
  If there is already a timer running with the same handle then it 
  will be stopped and re-started
===========================================================================*/
ps_timer_error_type ps_timeri_start
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len
)
{
  ps_timer_entry_type    *ps_timer;
  ps_timer_error_type     ret_val;
  boolean                 tick_timer_start = FALSE;
  uint64                  ps_timer_tick_value = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Check if the handle passed is valid and obtain the valid timer address.
    -----------------------------------------------------------------------*/
    if ((ps_timer = ps_timeri_handle_to_timer(timer_handle)) == NULL)
    {
      LOG_MSG_ERROR_1("ps_timeri_start(): Couldn't find timer 0x%x ", timer_handle);
      ret_val = PS_TIMER_FAILURE;
      break;
    }
    
    LOG_PSTIMER_MSG_INFO1_3("ps_timeri_start(): Timer Ptr 0x%x Handle 0x%x, timeout %ld micSec ", 
      ps_timer, timer_handle, timer_len);

    /*-------------------------------------------------------------------------
    If the timer is in the list, take it off the list.
    Note that the timer is on the list only if it is in RUN. If it is STOP,or FREE it is not in the list.
    -------------------------------------------------------------------------*/
    if (PS_TIMER_RUN == ps_timer->timer_state)
    {
      tick_timer_start = ps_timeri_remove (ps_timer, &ps_timer_tick_value);

      if (TRUE == tick_timer_start)
      {
        ps_timeri_tick_stop ();
        
        if (0 != ps_timer_tick_value)
        {
          ps_timeri_tick_start (ps_timer_tick_value);
        }
      }
      
      LOG_PSTIMER_MSG_INFO1_2("ps_timeri_start(): Rex timer start %d time value %ld",
        tick_timer_start, ps_timer_tick_value);
    }
    /*-----------------------------------------------------------------------
      Ensure that if there was already a callback pending, but applications
      are restarting the same timer anyway, we cancel the pending callback.
      This can potentially happen if applications start and stop wihtin call back.
    -----------------------------------------------------------------------*/
    if (ps_timer->cback_pending == TRUE)
    {
      LOG_PSTIMER_MSG_INFO1_1("ps_timeri_start(): Timer 0x%x, removing old callback "
                      "that hasn't been processed yet", timer_handle);
      ps_timer->cback_pending = FALSE;
    }    
     /*---------------------------------------------------------------------
     Initally set same value in requested  and node expiry time. Node expiry time would
     be adjusted while inserting the timer into list
   ---------------------------------------------------------------------*/
     ps_timer->time_requested   = timer_len;

     ps_timer->node_expiry_time = timer_len;
     
    /*-------------------------------------------------------------------------
     Check if this first timer in the list. If yes, then set the variable 
     tick_timer_start to TRUE which causes the tick timer to start below.
   -------------------------------------------------------------------------*/
     tick_timer_start = ps_timeri_insert (ps_timer, &ps_timer_tick_value);
   
    /*-------------------------------------------------------------------------
      If this is the first timer in the list, then tick_timer_start will be
      TRUE. If so, then start the system timer. 
    -------------------------------------------------------------------------*/
    if (TRUE == tick_timer_start )
    {
      ps_timeri_tick_start (ps_timer_tick_value);
    }

    ret_val = PS_TIMER_SUCCESS;

  } while( 0);

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  return ret_val;
} /* ps_timeri_start() */

ps_timer_error_type ps_timeri_start_ext
(
  ps_timer_handle_type  timer_handle,
  uint64                timer_len
)
{
  ps_timer_entry_type    *ps_timer = NULL;
  uint16                  index    =  timer_handle - 1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( index >= PS_TIMER_MAX_EXT )
  {
    return PS_TIMER_FAILURE;
  }

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  do
  {
    ps_timer = ps_timer_list_ext[index] ;

    LOG_PSTIMER_MSG_INFO1_3("ps_timeri_start_ext(): Timer Ptr 0x%x Handle 0x%x, timeout %ld micSec ", 
    ps_timer, timer_handle, timer_len);

    if ( NULL != ps_timer)
    {
      if (PS_TIMER_RUN == ps_timer->timer_state)
      {
        ps_timer_cancel_ext(  timer_handle );
      }
      /*---------------------------------------------------------------------
      Initally set same value in requested 
    ---------------------------------------------------------------------*/
      ps_timer->time_requested   = timer_len;
    
      ps_platform_timer_set(&ps_tick_timer_handle_ext, 
                             timer_len, 
                             NULL);
  
      ps_timer->timer_state =  PS_TIMER_RUN;
    }
  }while(0);

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  return PS_TIMER_SUCCESS;
}/* ps_timeri_start_ext */

/*===========================================================================
FUNCTION PS_TIMER_CANCEL()

DESCRIPTION
  Removes the Timer from the active list of timers.

DEPENDENCIES
  None

PARAMETERS
  handle:          The assigned ps_timer handle.

RETURN VALUE
 PS_TIMER_FAILURE: if there is an error cancelling the timer.
 PS_TIMER_SUCCESS: if the cancelling is success.

SIDE EFFECTS

===========================================================================*/
ps_timer_error_type ps_timer_cancel
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_error_type     ret_val = PS_TIMER_SUCCESS;
  boolean                 tick_timer_start = FALSE;
  uint64                  ps_timer_tick_value = 0;
  ps_timer_entry_type     *ps_timer;
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_1("ps_timer_cancel(): Timer handle 0x%x", timer_handle);

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Check if the handle passed is valid and obtain the valid timer address.
    -----------------------------------------------------------------------*/
    if ((ps_timer = ps_timeri_handle_to_timer(timer_handle)) == NULL)
    {
      LOG_MSG_ERROR_1("ps_timer_cancel(): Couldn't find timer 0x%x",
                      timer_handle);
      ret_val = PS_TIMER_FAILURE;
      break;
    }
    
     LOG_PSTIMER_MSG_INFO1_1("ps_timer_cancel(): ps_timer 0x%x", ps_timer);
     
    /*-----------------------------------------------------------------------
    if timer is running then first stop it and start the next timer in the list
    -----------------------------------------------------------------------*/
    if (PS_TIMER_RUN == ps_timer->timer_state)
    {
      tick_timer_start = ps_timeri_remove (ps_timer, &ps_timer_tick_value);
      
      LOG_PSTIMER_MSG_INFO1_2("ps_timer_cancel(): Rex timer start %d  Time value %ld",
        tick_timer_start, ps_timer_tick_value);

      if (TRUE == tick_timer_start)
      {
        ps_timeri_tick_stop ();
        
        if (0 != ps_timer_tick_value)
        {
          ps_timeri_tick_start (ps_timer_tick_value);
        }
      }
    }
    /*-----------------------------------------------------------------------
      Ensure that if there was already a callback pending, but applications
      are restarting the same timer anyway, we cancel the pending callback.
      This can potentially happen if applications start and stop wihtin call back.
    -----------------------------------------------------------------------*/
    if (ps_timer->cback_pending == TRUE)
    {
      LOG_PSTIMER_MSG_INFO1_1("ps_timer_cancel(): Timer 0x%x, removing old callback "
                      "that hasn't been processed yet", timer_handle);
      ps_timer->cback_pending = FALSE;
    }
    
  } while (0);
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  return ret_val;
} /* ps_timer_cancel() */

ps_timer_error_type ps_timer_cancel_ext
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_entry_type    *ps_timer = NULL;
  ps_timer_error_type     ret_val = PS_TIMER_SUCCESS;
  uint16                  index   =  timer_handle - 1;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_1("ps_timer_cancel_ext(): Timer handle 0x%x", timer_handle);
  
  if( index >= PS_TIMER_MAX_EXT )
  {
    return PS_TIMER_FAILURE;
  }

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  
  ps_timer = ps_timer_list_ext[index] ;

  ps_platform_timer_clr (&ps_tick_timer_handle_ext);

  if( NULL != ps_timer)
  {
    ps_timer->timer_state =  PS_TIMER_STOP;
  } 
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
  
  return ret_val;

}

/*===========================================================================
FUNCTION PS_TIMER_FREE()

DESCRIPTION
  Function will free timer and it's associated memory. Function will stop the timer if it is running
  and then will free timer block allocated to given handler.

DEPENDENCIES
  Note that the application cannot free a timer twice.

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE
  PS_TIMER_SUCCESS: If free'd.
  PS_TIMER_FAILURE: If cannot free

SIDE EFFECTS
  None
===========================================================================*/
ps_timer_error_type ps_timer_free
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_entry_type    *ps_timer;
  ps_timer_error_type     ret_val   = PS_TIMER_SUCCESS;
    uint32                index    = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_1("ps_timer_free(): Timer handle 0x%x", timer_handle);

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Check if the handle passed is valid and obtain the pointer to the timer.
    -----------------------------------------------------------------------*/
    if ((ps_timer = ps_timeri_handle_to_timer(timer_handle)) == NULL)
    {
      LOG_MSG_ERROR_1("ps_timer_free(): Couldn't find timer 0x%x",
                      timer_handle);
      ret_val = PS_TIMER_FAILURE;
      break;
    }

    LOG_PSTIMER_MSG_INFO1_1("ps_timer_free(): ps_timer 0x%x", ps_timer);
    
    /*-----------------------------------------------------------------------
      Cancel the timer.
    -----------------------------------------------------------------------*/
   if (PS_TIMER_SUCCESS == ps_timer_cancel(timer_handle))
   {
      /*-----------------------------------------------------------------------
      Make sure this timer is not attached to anything and update its state.
      -----------------------------------------------------------------------*/
      PS_SYSTEM_HEAP_MEM_FREE (ps_timer);
    
      (void)ps_timer_handle_to_index (timer_handle, &index);
      
      ps_timer_list[index] = NULL;
    
   }
   else
   {
     ret_val = PS_TIMER_FAILURE;
   }
  } while (0);
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  return ret_val;
} /* ps_timer_free() */

ps_timer_error_type ps_timer_free_ext
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_entry_type    *ps_timer = NULL;
  ps_timer_error_type     ret_val = PS_TIMER_SUCCESS;
  uint16                  index   =  timer_handle - 1;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  LOG_PSTIMER_MSG_INFO1_1("ps_timer_free_ext(): Timer handle 0x%x", timer_handle);
  
  if( index >= PS_TIMER_MAX_EXT )
  {
    return PS_TIMER_FAILURE;
  }
  
  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);
  
  ps_timer = ps_timer_list_ext[index] ;
  
  ps_platform_timer_clr (&ps_tick_timer_handle_ext);

  PS_SYSTEM_HEAP_MEM_FREE (ps_timer);

  ps_timer_list_ext[index] = NULL;
  
  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
  
  return ret_val;
}/* ps_timer_free_ext*/

/*===========================================================================
FUNCTION PS_TIMER_IS_RUNNING()

DESCRIPTION
  This function can be used to check if a timer is running.

  Note that, even if a non valid handle is passed, this function returns
  FALSE, because that timer is not running.
  An example of where this function can be used is, in TCP delayed ack
  timers, always check if the timer is running before starting the timer
  again.

DEPENDENCIES
  None

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE
  TRUE:  If the Timer is running
  FALSE: If the Timer is not Running

SIDE EFFECTS
  None
===========================================================================*/
boolean ps_timer_is_running
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_entry_type *ps_timer;
  boolean              is_timer_running;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_PSTIMER_MSG_INFO1_1("ps_timer_is_running(): Timer handle 0x%x", timer_handle);

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Check if the handle passed is valid and obtain the pointer to the timer.
    -----------------------------------------------------------------------*/
    if ((ps_timer = ps_timeri_handle_to_timer(timer_handle)) == NULL)
    {
    
      LOG_MSG_ERROR_1("ps_timer_is_running(): Couldn't find timer 0x%x",
                      timer_handle);
      is_timer_running = FALSE;
      break;
    }

    if (ps_timer->timer_state == PS_TIMER_RUN)
    {
      is_timer_running = TRUE;
    }               
    else
    {
      is_timer_running = FALSE;
    } 

    LOG_PSTIMER_MSG_INFO1_2("ps_timer_is_running(): Timer 0x%x is_running %ld",
                    timer_handle, is_timer_running);
    
  } while (0);

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);
  
  return is_timer_running;

} /* ps_timer_is_running() */

/*===========================================================================
FUNCTION PS_TIMER_REMAINING()

DESCRIPTION
  This function is used to check the time left on a running timer.

DEPENDENCIES
  None

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE
  If timer is running, time left (in ms)
  If timer is stopped, 0
  If timer does not exist, -1

SIDE EFFECTS
  None
===========================================================================*/
int64 ps_timer_remaining
(
  ps_timer_handle_type timer_handle
)
{
  int64  remaining_microsecond_time   = 0;
  int64  remaining_millisecond_time   = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_PSTIMER_MSG_INFO1_1("ps_timer_remaining(): Timer 0x%x", timer_handle);
  
  /*-----------------------------------------------------------------------
  Get remaining time in  microsecond.
  -----------------------------------------------------------------------*/
  remaining_microsecond_time = ps_timeri_remaining(timer_handle);

  /*-----------------------------------------------------------------------
  Convert microsecond  time into millisecond. PS Timer internal APIs work/support microsecond.
  If an error has occurred, no conversion is needed, directly pass the error back.
  -----------------------------------------------------------------------*/
  if( remaining_microsecond_time == -1 )
  {
    remaining_millisecond_time = remaining_microsecond_time;
  }
  else
  {
    remaining_millisecond_time = remaining_microsecond_time/1000;
  }

  LOG_PSTIMER_MSG_INFO1_2("ps_timer_remaining(): Time(micro) %ld, Time(milli) %ld",
    remaining_microsecond_time, remaining_millisecond_time);

  
  return remaining_millisecond_time;

}/*ps_timer_remaining*/

/*===========================================================================
FUNCTION PS_TIMERI_REMAINING()

DESCRIPTION
  This function is used to check the time left on a running timer.

DEPENDENCIES
  None

PARAMETERS
  handle:           The assigned ps_timer handle.

RETURN VALUE
  If timer is running, time left (in micro second)
  If timer is stopped, 0
  If timer does not exist, -1

SIDE EFFECTS
  None
===========================================================================*/
int64 ps_timeri_remaining
(
  ps_timer_handle_type timer_handle
)
{
  ps_timer_entry_type     *ps_timer   = NULL;
  ps_timer_entry_type     *curr_timer = NULL;
  int64                    node_expiry_time = -1;
  int64                    remaining_time   = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_ENTER_CRIT_SECTION(&ps_timer_crit_section);

  do
  {
    /*-----------------------------------------------------------------------
      Check if the handle passed is valid and obtain the pointer to the timer.
    -----------------------------------------------------------------------*/
    if ((ps_timer = ps_timeri_handle_to_timer(timer_handle)) == NULL)
    {
    
      LOG_MSG_ERROR_1("ps_timeri_remaining(): Couldn't find timer 0x%x",
                      timer_handle);
      break;
    }
    
    curr_timer  = ps_timer_list_head_ptr;
    
    /*-----------------------------------------------------------------------
      Check the timer state to determine how much time is left.
    -----------------------------------------------------------------------*/
    if ((NULL != curr_timer) && (ps_timer->timer_state == PS_TIMER_RUN))
    {
      
      remaining_time = ps_platform_timer_get( &ps_tick_timer_handle );
      
      if (curr_timer == ps_timer)
      {
        node_expiry_time = remaining_time;
      }
      else
      {
        curr_timer = curr_timer->next_ptr;
        
        while(  (NULL != curr_timer) && (curr_timer != ps_timer))
        {

          LOG_PSTIMER_MSG_INFO1_2("ps_timeri_remaining(): Time(ms) %ld, curr node expire %ld",
            remaining_time/1000, curr_timer->node_expiry_time);

          remaining_time += curr_timer->node_expiry_time;
          curr_timer = curr_timer->next_ptr;
        }

        if ( curr_timer == ps_timer)
        {
          node_expiry_time = remaining_time + curr_timer->node_expiry_time;
  
          LOG_PSTIMER_MSG_INFO1_2("ps_timeri_remaining(): Time(ms) %ld, curr node expire %ld",
            remaining_time/1000, curr_timer->node_expiry_time);
        }
      }
    }
    else
    {
      /*-----------------------------------------------------------------------
        If the timer is not running then return 0 time remaining.
      -----------------------------------------------------------------------*/
      node_expiry_time = 0;
    }    
  } while (0);

  PS_LEAVE_CRIT_SECTION(&ps_timer_crit_section);

  return node_expiry_time;
} /* ps_timeri_remaining() */

#ifdef TEST_FRAMEWORK
  #error code not present
#endif /*TEST_FRAMEWORK*/

