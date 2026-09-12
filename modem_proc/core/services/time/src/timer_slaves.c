/*=============================================================================
  Copyright (c) 2003-2017 QUALCOMM Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/**
@file timer_slaves.c
@brief
Implements timer slave tasks

Traditionally, timer task has been used to call the calllback 
functions associated with the timer. This has often led to 
abusive callback holding timer task at ransom, affecting timer 
accuracy. By implementing these slave tasks, the callbacks can 
be offloaded to these slave tasks, and hence timer task is free 
to services other expiring timers. 

In this file, there is support for three slave tasks, which can 
be enabled by defining FEATURE_TIMER_SLAVE1, 
FEATURE_TIMER_SLAVE2 and FEATURE_TIMER_SLAVE3 respectively. 
These defines can be made in <product>.builds file.

INITIALIZATION AND SEQUENCING REQUIREMENTS

timer_slave_cmd_queue_init() should be called before timer_init()

The slave tasks should be started before the timer task in tmc. 
The reason is that there might be expired timers needed to be serviced
when timer task starts. This would require that timer slave task(s) 
are already started to process these timer callbacks.* 
*/ 

/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/time/src/timer_slaves.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
04/02/2012 rks     Removed False Dependency to TMC.H in CoreBSP-Services
04/12/11   gparikh File created.

=============================================================================*/

#include "timer_slaves.h"
#include "ats_v.h"
#include "err.h"
#include "rex.h"
#include "rcevt.h"
#include "rcinit.h"
#include "rcinit_rex.h"
#include "DDITimetick.h"
#include "assert.h"
#include "osal.h"


/*defines here*/
#define MAX_TASK_PRIORITY 255

#define MAGIC_INITIALIZER 0xDEADBEEF
#define MAGIC_INITIALIZER_64 0xDEADBEEFD00DFEEDuLL
/*end defines*/

//#define TIMER_SLAVE_PRINT_HEALTH_MSG
#define TIMER_SLAVE_DEBUG_MSG_TH 50

#define TIMER_DEPENDENT_TASKS_BIT_LOCATION 8
#define TIMER_TASKS_ARRAY_INDEX 32

/*static variables here*/

static DalDeviceHandle *hTimerSlavesHandle = NULL;

/*Define timer cmd queue*/
timer_slave_cmd_type timer_slaves_cmd_buf[MAX_SLAVE_TASKS][MAX_TIMER_SLAVE_CMD_ELEMENTS];

/*to store the commands in queue*/
q_type timer_slaves_cmd_q[MAX_SLAVE_TASKS];          

/* Queue for storing free command buffers */
q_type timer_slaves_cmd_free_q[MAX_SLAVE_TASKS];     

#define KEEP_UNCACHED __attribute__((section (".keep_uncached")))

uint64 timer_slave_processing_time_warning_threshold KEEP_UNCACHED = (8*TIMER_FREQ_VALUE)/32768 ; /* Keeping same ratio as 32kHz case */

#ifndef TIMER_ERROR_FATAL_THRESHOLD_ZEROED_OUT 
uint64 timer_slave_processing_time_err_fatal_threshold KEEP_UNCACHED =2*TIMER_FREQ_VALUE;
#else
uint64 timer_slave_processing_time_err_fatal_threshold KEEP_UNCACHED =0;
#endif

static rex_tcb_type* timer_slave1_tcb_ptr=NULL;
static rex_tcb_type* timer_slave2_tcb_ptr=NULL;
static rex_tcb_type* timer_slave3_tcb_ptr=NULL;


/* Each bit in this array represents the tasks priority.
   If two or more timers are dependent on each we can set
   corresponding bit in this array to mark dependency  
*/
static uint8 timer_dependency[TIMER_TASKS_ARRAY_INDEX] = {0};
/*end static variables*/

/*function prototypes here*/

extern void timer_reset_check_pc_mask_if_set
(
  unsigned int index,
  unsigned int decrease_count
);

/*end function prototypes*/

typedef struct timer_slave_debug_var
{
  timer_ptr_type timer_expired;
  time_timetick_type processing_started;
  time_timetick_type processing_time;
  uint32 fn_address;
  uint8  call_back_type;
}timer_expired_slave_type;

/*the arrays are intentionally defined separately for fine control
 over each array size.*/
#ifdef FEATURE_TIMER_SLAVE1
#define MAX_TIMER_EXPIRED_SLAVE1 60
static timer_expired_slave_type timers_expired_slave1[MAX_TIMER_EXPIRED_SLAVE1];
static uint32 timer_expired_cnt_slave1=0;
static uint32 timer_expired_cnt_global_slave1=0;
#endif

#ifdef FEATURE_TIMER_SLAVE2
#define MAX_TIMER_EXPIRED_SLAVE2 60
static timer_expired_slave_type timers_expired_slave2[MAX_TIMER_EXPIRED_SLAVE2];
static uint32 timer_expired_cnt_slave2=0;
static uint32 timer_expired_cnt_global_slave2=0;
#endif

#ifdef FEATURE_TIMER_SLAVE3
#define MAX_TIMER_EXPIRED_SLAVE3 60
static timer_expired_slave_type timers_expired_slave3[MAX_TIMER_EXPIRED_SLAVE3];
static uint32 timer_expired_cnt_slave3=0;
static uint32 timer_expired_cnt_global_slave3=0;
#endif


/*end debug variables*/


/** 
@ingroup Timer Slaves Functions 
         
@brief 
timer slave task entry point. This function is called from tmc.

@return
None.

@param[in]                                                               
 tcb pointer
 slave task number

@param[out]
 not used.   
 
@dependencies  
 
@sideeffects 

*/
void timer_slave_task_common
(
  rex_tcb_type                 * slave_tcb_ptr,
  timer_slave_task_enum_type     slave_task_num,
  uint32                         pc_mask_index
)
{
  /* Signals returned from rex wait procedure */
  rex_sigs_type                   sigs;

  /*cmd pointer*/
  timer_slave_cmd_type *cmd_ptr = NULL;

  /*to logs when the cb processing starts*/
  time_timetick_type cb_processing_started = 0;

  /*to log when the cb processing ends*/
  time_timetick_type cb_processing_ended = 0;

  /*to log the processing time*/
  time_timetick_type cb_processing_time = 0;

  /*fn address*/
  uint32  fn_address_of_timer = 0;

  /*for debug messages*/
  uint32 print_health_msg_slave = 0;

  uint32 timer_after_pc_local_count =0;

  /* local variables for debug logs */
  timer_expired_slave_type * timers_expired_slave;
  uint32 *timer_expired_cnt;
  uint32 *timer_expired_cnt_global;

  if (slave_task_num == TIMER_SLAVE_TASK1)
  {
    timers_expired_slave = &timers_expired_slave1[0];
	timer_expired_cnt = &timer_expired_cnt_slave1;
	timer_expired_cnt_global = &timer_expired_cnt_global_slave1;
  }
  else if (slave_task_num == TIMER_SLAVE_TASK2)
  {
    timers_expired_slave = &timers_expired_slave2[0];
	timer_expired_cnt = &timer_expired_cnt_slave2;
	timer_expired_cnt_global = &timer_expired_cnt_global_slave2;
  }
  else if (slave_task_num == TIMER_SLAVE_TASK3)
  {
    timers_expired_slave = &timers_expired_slave3[0];
	timer_expired_cnt = &timer_expired_cnt_slave3;
	timer_expired_cnt_global = &timer_expired_cnt_global_slave3;
  }

  /* Signal the task in order to prime the timer processing mechanism */
  (void) rex_set_sigs( slave_tcb_ptr, TIMER_SLAVE_CMD_Q_SIG );

  for(;;)
  {

    /* Wait for some task signal */
    sigs = rex_wait( TIMER_SLAVE_CMD_Q_SIG );

    if ( sigs & TIMER_SLAVE_CMD_Q_SIG )
    {
      /* Clear the timer expiry signal */
      (void) rex_clr_sigs( slave_tcb_ptr,TIMER_SLAVE_CMD_Q_SIG );
      
      timer_after_pc_local_count=0;
      /*loop through the command queue and process all the commands*/
      while((cmd_ptr = (timer_slave_cmd_type *) q_get(&timer_slaves_cmd_q[slave_task_num])) != NULL)
      {

        timers_expired_slave[*timer_expired_cnt].timer_expired = cmd_ptr->timer_slave_operation.timer;
        timers_expired_slave[*timer_expired_cnt].call_back_type = 0;
        timers_expired_slave[*timer_expired_cnt].fn_address = 0;
        timers_expired_slave[*timer_expired_cnt].processing_time = MAGIC_INITIALIZER_64;

        /*log the time when processing started*/
        DalTimetick_GetTimetick64(hTimerSlavesHandle, &cb_processing_started);

        /*Log the time to indicate the start of processing of this timer*/
        timers_expired_slave[*timer_expired_cnt].processing_started = cb_processing_started;
    
        switch (cmd_ptr->call_back_type) 
        {
          /*TODO: need to fix this*/
          case TIMER_SLAVE_SET_SIG:
             /*as of now setting the signal through slave task is not supported. 
              It is done by timer task. This is just a place holder*/            
          break;
          case TIMER_SLAVE_CB_TYPE1:
            if(cmd_ptr->timer_slave_operation.func_cb1)
            {
              timers_expired_slave[*timer_expired_cnt].call_back_type = 
              (timers_expired_slave[*timer_expired_cnt].call_back_type | 0x2);
              timers_expired_slave[*timer_expired_cnt].fn_address = (uint32)cmd_ptr->timer_slave_operation.func_cb1;
              
              fn_address_of_timer = (uint32)cmd_ptr->timer_slave_operation.func_cb1;
             
              cmd_ptr->timer_slave_operation.func_cb1(cmd_ptr->timer_slave_operation.data);
            }
                       
            break;
          case TIMER_SLAVE_CB_TYPE2:
            if(cmd_ptr->timer_slave_operation.func_cb2)
            {
               timers_expired_slave[*timer_expired_cnt].call_back_type = 
               (timers_expired_slave[*timer_expired_cnt].call_back_type | 0x4); 
               timers_expired_slave[*timer_expired_cnt].fn_address = (uint32)cmd_ptr->timer_slave_operation.func_cb2;
               
               fn_address_of_timer = (uint32)cmd_ptr->timer_slave_operation.func_cb2;

               cmd_ptr->timer_slave_operation.func_cb2(cmd_ptr->timer_slave_operation.run_time,cmd_ptr->timer_slave_operation.data);
            }
          break;

          default:
            ATS_MSG("timer_slave1_debug: command type not supported");
          break;
        }/*end switch*/

        /*return the memory to free command queue for reuse*/
        q_put( &timer_slaves_cmd_free_q[slave_task_num], &cmd_ptr->link );

        /*log the cb ended time*/
        DalTimetick_GetTimetick64(hTimerSlavesHandle, &cb_processing_ended);
 
        /*to take care of the wrap around case*/
        if(cb_processing_ended >= cb_processing_started)
        {
          cb_processing_time = cb_processing_ended - cb_processing_started;       
        }
        else /*if it is a wrap around case*/
        {  
          cb_processing_time = 0xFFFFFFFFFFFFFFuLL - 
                        (cb_processing_started - cb_processing_ended)+1;
        }

        timers_expired_slave[*timer_expired_cnt].processing_time = cb_processing_time;
        *timer_expired_cnt = ((*timer_expired_cnt)+1)%MAX_TIMER_EXPIRED_SLAVE1;
        (*timer_expired_cnt_global)++;

        #ifdef TIMER_SLAVE_PRINT_HEALTH_MSG
        if(!((*timer_expired_cnt_global)%TIMER_SLAVE_DEBUG_MSG_TH))
        {
          print_health_msg_slave=1;
        }
        else
        {
          print_health_msg_slave =0;
        }
        #endif

        /*generate a warningif the processing time > warning threshold*/
        if((cb_processing_time > timer_slave_processing_time_warning_threshold)||print_health_msg_slave)
        {
          ATS_MSG_ERROR("Processing of callback fn = 0x%x took 0x%x ticks, threshold = 0x%x ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_slave_processing_time_warning_threshold);          
                    
        }

        /*generate a warningif the processing time > warning threshold*/
        if(timer_slave_processing_time_err_fatal_threshold != 0 
           && cb_processing_time > timer_slave_processing_time_err_fatal_threshold)
        {
          ERR_FATAL("Processing of callback fn = 0x%x took 0x%x ticks, threshold = 0x%x ticks",
               fn_address_of_timer,
               cb_processing_time,
               timer_slave_processing_time_warning_threshold);          
                    
        }
        timer_after_pc_local_count++;
      }/*end while*/
	  
      // DID we see this happened after powercollapse
      timer_reset_check_pc_mask_if_set(pc_mask_index,timer_after_pc_local_count);
    }/*end TIMER_SLAVE1_CMD_Q*/    

  }/*end for*/

} /* timer_slave_task_common */

/** 
@ingroup Timer Slaves Functions 
         
@brief 
timer slave task 1 entry point. This function is called from tmc.

@return
None.

@param[in]                                                               
 None  

@param[out]
 dword - Not used   
 
@dependencies  
Should be called before timer_task
 
@sideeffects 

*/
void timer_slave1_task
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  dword                           ignored    /*lint -esym(715,ignored) */
)
{
#ifdef FEATURE_TIMER_SLAVE1

  /* initiate the handshake with RCINIT */
  rcinit_handshake_startup();

  /* call the common task function */
  timer_slave_task_common(timer_slave1_tcb_ptr, TIMER_SLAVE_TASK1, TIMER_SLAVE1_PC_MASK_INDEX);

#endif /* FEATURE_TIMER_SLAVE1 */
} /* timer_slave1_task */


/** 
@ingroup Timer Slaves Functions 
         
@brief 
timer slave task 2 entry point. This function is called from tmc.

@return
None.

@param[in]                                                               
 dword - Not used  

@param[out]
 None   
 
@dependencies  
Should be called before timer_task
 
@sideeffects 

*/
void timer_slave2_task
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  dword                           ignored    /*lint -esym(725,ignored) */
)
{
#ifdef FEATURE_TIMER_SLAVE2

  /* initiate the handshake with RCINIT */
  rcinit_handshake_startup();

  /* call the common task function */
  timer_slave_task_common(timer_slave2_tcb_ptr, TIMER_SLAVE_TASK2, TIMER_SLAVE2_PC_MASK_INDEX);

#endif /* FEATURE_TIMER_SLAVE2*/

} /* timer_slave2_task */

/** 
@ingroup Timer Slaves Functions 
         
@brief 
timer slave task 3 entry point. This function is called from tmc.

@return
None.

@param[in]                                                               
 dword - Not used  

@param[out]
 None   
 
@dependencies  
Should be called before timer_task
 
@sideeffects 

*/
void timer_slave3_task
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  dword                           ignored    /*lint -esym(735,ignored) */
)
{

#ifdef FEATURE_TIMER_SLAVE3

  /* initiate the handshake with RCINIT */
  rcinit_handshake_startup();

  /* call the common task function */
  timer_slave_task_common(timer_slave3_tcb_ptr, TIMER_SLAVE_TASK3, TIMER_SLAVE3_PC_MASK_INDEX);

#endif /*FEATURE_TIMER_SLAVE3*/
} /* timer_slave3_task */


/** 
@ingroup Timer Slaves Functions 
         
@brief 
Initializes timer slave commmand queues

@return
None.

@param[in]                                                               
 None  

@param[out]
 None   
 
@dependencies  
Should be called before timer slave tasks are started
 
@sideeffects 

*/
void timer_slave_cmd_queue_init
(
  void
)
{
  int  tasks,queue_elements;

  for(tasks=0; tasks < MAX_SLAVE_TASKS; tasks++)
  {
    /*init the timer queue*/
    (void) q_init(&timer_slaves_cmd_q[tasks]);
    (void) q_init(&timer_slaves_cmd_free_q[tasks]);
  }

  /*loop through the slave tasks*/
  for(tasks = 0; tasks < MAX_SLAVE_TASKS; tasks++)
  {
    /*initialize the queue elements and place all the queue elements
     in free cmd queue*/
    for( queue_elements = 0; queue_elements < MAX_TIMER_SLAVE_CMD_ELEMENTS; queue_elements++ )
    {
      (void)q_link( &timer_slaves_cmd_buf[tasks][queue_elements], 
                    &timer_slaves_cmd_buf[tasks][queue_elements].link );
      q_put( &timer_slaves_cmd_free_q[tasks], 
             &timer_slaves_cmd_buf[tasks][queue_elements].link );
    }

  }

  /*dummy assignments, so that the compiler does not optimize out
   these variables*/
  //timer_slave_processing_time_warning_threshold = timer_slave_processing_time_warning_threshold;
  //timer_slave_processing_time_err_fatal_threshold=timer_slave_processing_time_err_fatal_threshold;


}

/** 
@ingroup Timer Slaves Functions 
         
@brief 
Allocates memory from the command buffer of the specified
slave task

@return
None.

@param[in]                                                               
timer_slave_task_enum_type - slave task from which the memory is 
                             to be obtained

@param[out]
timer_slave_cmd_type* - pointer to cmd buffer memory
 
@dependencies  
None
 
@sideeffects 

*/
timer_slave_cmd_type  *timer_slave_get_cmd_buf
( 
  timer_slave_task_enum_type time_slave_task_num 
)
{
  timer_slave_cmd_type    *cmd_ptr=NULL;                           /* Pointer to command */
  static uint32           timer_slave_q_cnt=0;
  static uint32           timer_slave_free_q_cnt=0;
  static uint32           timer_slave_num_queried=0xFF;

  /*check for bounds on time_slave_task_num */
  if(time_slave_task_num >= MAX_SLAVE_TASKS)
  {
    ATS_MSG_2("time_slave_task_num = %d, max value should be less than = %d"
              , time_slave_task_num, MAX_SLAVE_TASKS);
    return NULL;
  }

  /* Get a command buffer from the free command queue. */
  if(NULL == (cmd_ptr = q_get( &timer_slaves_cmd_free_q[time_slave_task_num] )) )
  {
    /*-----------------------------------------------------------------------
        No free command buffers available, something is seriously wrong.
    -----------------------------------------------------------------------*/
    timer_slave_num_queried=time_slave_task_num;
    timer_slave_q_cnt =  q_cnt(&timer_slaves_cmd_q[time_slave_task_num]);
    timer_slave_free_q_cnt = q_cnt(&timer_slaves_cmd_free_q[time_slave_task_num]);
    
    ERR_FATAL("No items on time slave task free cmd q on %d,Q_ele=%d,Free_Q=%d", 
              timer_slave_num_queried, 
              timer_slave_q_cnt, 
              timer_slave_free_q_cnt);
  }  
 
  return( cmd_ptr );

} /* timer_slave_get_cmd_buf */

/** 
@ingroup Timer Slaves Functions 
         
@brief 
Post commands on appropriate slave task

@return
None.

@param[in]                                                               
* timer_slave_post_cmd_params_type* - ptr to command
*                                     structure 

@param[out]
timer_slave_operation_status_type - status of the operation
 
@dependencies  
None
 
@sideeffects 

*/
timer_slave_operation_status_type timer_slave_post_cmd
(  
  timer_slave_post_cmd_params_type* slave_cmd_ptr
)
{
  rex_tcb_type* timer_slave_tcb_ptr = NULL;
  timer_slave_cmd_type* timer_slave_cmd_ptr = NULL;
  timer_slave_operation_status_type status = TIMER_SLAVE_RESULT_PASS;
  uint8 timer_slave_task_num;

  if(NULL == slave_cmd_ptr)
  {
    return TIMER_SLAVE_RESULT_FAIL;
  }

  timer_slave_task_num = slave_cmd_ptr->timer_slave_task_num;

  /*check for bounds on time_slave_task_num */
  if(timer_slave_task_num >= MAX_SLAVE_TASKS)
  {
    ATS_MSG_2("time_slave_task_num = %d, max value should be less than = %d"
              , timer_slave_task_num, MAX_SLAVE_TASKS);
    status= TIMER_SLAVE_RESULT_FAIL;
  }

  /*get the cmd q buffer from the appropriate queue*/
  timer_slave_cmd_ptr = timer_slave_get_cmd_buf((timer_slave_task_enum_type)timer_slave_task_num);
  
  if(NULL == timer_slave_cmd_ptr)
  {
    ERR_FATAL("timer_slave_task_cmd_ptr == NULL",0,0,0);
    status= TIMER_SLAVE_RESULT_FAIL;
  }

  /*fill in the data for the cmd*/
  timer_slave_cmd_ptr->timer_slave_operation.timer = slave_cmd_ptr->timer_ext;
  timer_slave_cmd_ptr->call_back_type = slave_cmd_ptr->cb_type;
  timer_slave_cmd_ptr->timer_slave_operation.tcb = slave_cmd_ptr->rex_tcb_ptr;
  timer_slave_cmd_ptr->timer_slave_operation.sigs = slave_cmd_ptr->signal_to_be_set;
  timer_slave_cmd_ptr->timer_slave_operation.func_cb1 = slave_cmd_ptr->cb1;
  timer_slave_cmd_ptr->timer_slave_operation.func_cb2 = slave_cmd_ptr->cb2;
  timer_slave_cmd_ptr->timer_slave_operation.run_time = slave_cmd_ptr->run_time;
  timer_slave_cmd_ptr->timer_slave_operation.data = slave_cmd_ptr->data;

  /*link the cmd*/
  q_link(timer_slave_cmd_ptr, &timer_slave_cmd_ptr->link);

  /*populate the tcb with correct tcb pointer*/
  if(TIMER_SLAVE_TASK1 == timer_slave_task_num)
  {
    #ifdef FEATURE_TIMER_SLAVE1
    timer_slave_tcb_ptr = timer_slave1_tcb_ptr;
    #endif
  }
  else if(TIMER_SLAVE_TASK2 == timer_slave_task_num)
  {
    #ifdef FEATURE_TIMER_SLAVE2
    timer_slave_tcb_ptr = timer_slave2_tcb_ptr;
    #endif
  }
  else if(TIMER_SLAVE_TASK3 == timer_slave_task_num)
  {
    #ifdef FEATURE_TIMER_SLAVE3
    timer_slave_tcb_ptr = timer_slave3_tcb_ptr;
    #endif
  }
  else
  {
    status = TIMER_SLAVE_RESULT_FAIL;
  }

  if(timer_slave_tcb_ptr)
  {    
    /*put the message on queue and signal the task*/
    q_put(&timer_slaves_cmd_q[timer_slave_task_num] , &timer_slave_cmd_ptr->link);

    /*set the signal on the queue*/
    (void) rex_set_sigs(timer_slave_tcb_ptr, TIMER_SLAVE_CMD_Q_SIG);    
  }
    
  return status;
}


boolean timer_slave_set_dependency_bit (int task_priority)
{
   int index_of_array,index_for_bit;
   
   index_of_array = task_priority / TIMER_DEPENDENT_TASKS_BIT_LOCATION;
   index_for_bit  = task_priority % TIMER_DEPENDENT_TASKS_BIT_LOCATION;
   
   ATS_MSG_2("timer_slave_set_dependency_bit %d %d",index_of_array,index_for_bit);
   if(index_of_array >= TIMER_TASKS_ARRAY_INDEX)
	   return FALSE;
   
   /* setting dependency bit in the global varibale 
      based on task priority */
   timer_dependency[index_of_array] |= 
            1 << (TIMER_DEPENDENT_TASKS_BIT_LOCATION - index_for_bit); 
	
	return TRUE;
}

/** 
@ingroup Timer Slaves Functions 
         
@brief 
Checking whether a timer is dependent on other timer.

@return
True  If the timer is dependent on other timer
False If the timer is not dependent on other timer

@param[in]                                                               
Priority of task which is setting this timer.
 
@dependencies  
None
 
@sideeffects 
None
*/
static boolean is_timer_dependent(rex_priority_type task_priority)
 {
  
    int index_of_array,index_for_bit;
	
    index_of_array = task_priority / TIMER_DEPENDENT_TASKS_BIT_LOCATION;
    index_for_bit  = task_priority % TIMER_DEPENDENT_TASKS_BIT_LOCATION;

   if(index_of_array >= TIMER_TASKS_ARRAY_INDEX)
	   return FALSE;
   
    /* Checking whether dependency bit is set
       or not in global variable */
   if(timer_dependency[index_of_array] & 
          1 << (TIMER_DEPENDENT_TASKS_BIT_LOCATION - index_for_bit))
     return TRUE;
   else 
	return FALSE;
}

/** 
@ingroup Timer Slaves Functions 
         
@brief 
Assigns slave task to the timer during timer_def

@return
slave task id assigned to the caller

@param[in]                                                               
None

@param[out]
None
 
@dependencies  
FEATURE_TIMER_SLAVE1 should be defined
 
@sideeffects 

*/
uint8 timer_assign_slave_task_to_timer ( void )
{ 

  boolean ret ;
  rex_priority_type task_priority = osal_thread_get_pri();
  
  /*checking whether this current task's timer is dependent 
  on other timer or not*/
  ret = is_timer_dependent(task_priority);
  
  if(ret == TRUE)
	  return TIMER_SLAVE_TASK1;
  else
	  return task_priority % MAX_SLAVE_TASKS ;

}
/** 

@ingroup Timer Slaves Functions

@brief 

Initializes slave tasks 

@return

None

@param[in]                                                               
None

@param[out]
None

@dependencies  

FEATURE_TIMER_SLAVE<X> should be defined to
start the timer_slave<x> task

@sideeffects 
Should be started before timer task
*/
void timer_slave_task1_init
(
  void
)
{
  RCINIT_INFO handle = rcinit_lookup("tmr_slave1");
  timer_slave1_tcb_ptr = rcinit_lookup_rextask("tmr_slave1");

  if (RCINIT_NULL != handle)
  {
    rcinit_initfn_spawn_task(handle, timer_slave1_task);
  }
}

void timer_slave_task2_init
(
  void
)
{
  RCINIT_INFO handle = rcinit_lookup("tmr_slave2");
  timer_slave2_tcb_ptr = rcinit_lookup_rextask("tmr_slave2");

  if (RCINIT_NULL != handle)
  {
    rcinit_initfn_spawn_task(handle, timer_slave2_task);
  }
}

void timer_slave_task3_init
(
  void
)
{
  RCINIT_INFO handle = rcinit_lookup("tmr_slave3");
  timer_slave3_tcb_ptr = rcinit_lookup_rextask("tmr_slave3");

  if (RCINIT_NULL != handle)
  {
    rcinit_initfn_spawn_task(handle, timer_slave3_task);
  }
}

/*=============================================================================

FUNCTION TIMER_SLAVES_GET_TIMETICK_HANDLE

DESCRIPTION
 This function is called from time_init to pass timetick handle to timer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void timer_slaves_get_timetick_handle(void **hTimerHandlePtr)
{
  DalTimetick_Attach("SystemTimer", &hTimerSlavesHandle);
  ASSERT(hTimerSlavesHandle != NULL);
  if(hTimerHandlePtr != NULL)
  {
    *hTimerHandlePtr = hTimerSlavesHandle;
  }
}

