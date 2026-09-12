/*=========================================================================
  Copyright (c) 2015-17, 2019 Qualcomm Technologies Incorporated.
  All rights reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/**
@file heapanalyzer.c
@brief
Implements the heap analyzer functionality to send out information on 
heap usage for each task per heap.
 
INITIALIZATION AND SEQUENCING REQUIREMENTS

*/ 

/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/utils/src/heapanalyzer.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/20/17   nk      Fixed crash while opening heap analyzer window
07/24/15   ps      Add file to support heap analyzer view in QXDM

=============================================================================*/

#include "core_variation.h"
#include "comdef.h"

#include "rcinit.h"
#include "timer.h"
#include "msg.h"
#include "heapanalyzer.h"

#include "memheap.h"
#include <stringl/stringl.h> /* It is mandatory to use memscpy as memcpy is banned */

#include "dog.h"
#include "task.h"
#include "rex.h"
#include "rcevt.h"


#ifndef HEAP_ANALYZER_MAX_HEAPS
#define HEAP_ANALYZER_MAX_HEAPS 1
#endif

#define HEAP_ANALYZER_TIMER_SIG 0x1
rex_timer_type heap_analyzer_timer;
extern uint16 magic_num_index;
extern mem_heap_type *heap_ptr_table[MAX_HEAP_INIT];
extern char memheap_tls_to_tname[MEMHEAP_MAX_THREADS][QURT_THREAD_ATTR_NAME_MAXLEN];

rex_tcb_type *heap_analyzer_tcb_ptr = NULL;

#define PROFILE_TYPE_TOP_TASKS 1
#define PROFILE_TYPE_TASK_IDS 2

ha_profile_type ha_profile;

static uint16 log_timer = HA_DEFAULT_DURATION;
static task_log_info_type top_heap_user_stats[HA_MAX_TASK_PROFILE];

/*===========================================================================

 FUNCTION get_heaps_info_cmd_handler

DESCRIPTION
 This function responsds to the get heap info command with all the heaps (memheap)currently active in the system.
 The response is a list of heap names in the system.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) get_heaps_info_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  uint16 num_heaps;
  int rsp_len;
  get_heaps_info_rsp_type * rsp_ptr = NULL;
  heap_id_name_t *heap_id_name;

  num_heaps = magic_num_index;

  rsp_len = sizeof( get_heaps_info_rsp_type )+ num_heaps*sizeof(heap_id_name_t);

  /* Allocate the same length as the request. */
  rsp_ptr = (get_heaps_info_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           GET_HEAPS_INFO,
                                           rsp_len );

  if (rsp_ptr != NULL && num_heaps < MAX_HEAP_INIT)
  {
     rsp_ptr->num_heaps = num_heaps-1;
     heap_id_name = (heap_id_name_t *)((char *)rsp_ptr + sizeof(get_heaps_info_rsp_type));

     for(int i = 0; i<num_heaps;i++)
     {
        heap_id_name->heap_id = i;
		if(heap_ptr_table[i] == 0)
		{
			continue;
		}
        memscpy(heap_id_name->heap_name, HEAP_NAME_SIZE, heap_ptr_table[i]->heapName, HEAP_NAME_SIZE);
        heap_id_name++;
     }
  }

  (void)pkt_len;

  return (rsp_ptr);

} /* get_heaps_info_cmd_handler */


extern uint16 tls_stats_index_count;

/*===========================================================================

 FUNCTION get_heap_task_names_cmd_handler

DESCRIPTION
 This function responsds to the get task names command in the system using the heap.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) get_heap_task_names_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  int rsp_len;
  uint16 num_tasks;
  task_id_name_t *task_ptr;
  get_heap_task_names_rsp_type * rsp_ptr = NULL;

  num_tasks = (tls_stats_index_count < 16)?tls_stats_index_count : 16;

  rsp_len = sizeof( get_heap_task_names_rsp_type) + (num_tasks * sizeof(task_id_name_t));

  /* Allocate the same length as the request. */
  rsp_ptr = (get_heap_task_names_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           GET_HEAP_TASK_NAMES,
                                           rsp_len );

  if (rsp_ptr != NULL)
  {
     rsp_ptr->num_tasks= num_tasks;
     task_ptr = (task_id_name_t *)(((char *)rsp_ptr) + sizeof(get_heap_task_names_rsp_type));

     for(int i = 0; i<num_tasks;i++)
     {
        task_ptr->task_id = i;
        memscpy(task_ptr->task_name, QURT_THREAD_ATTR_NAME_MAXLEN, memheap_tls_to_tname[i], QURT_THREAD_ATTR_NAME_MAXLEN);
        task_ptr++;
     }
  }

  (void)pkt_len;

  return (rsp_ptr);

} /* get_heaps_info_cmd_handler */


/*===========================================================================

 FUNCTION set_heap_profile_top_tasks_cmd_handler

DESCRIPTION
 This function sets the heap analyzer profile for a particular heap. The profile type is the top N tasks to be profiled.
 specified by the user.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) set_heap_profile_top_tasks_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  int rsp_len;
  set_heap_profile_top_tasks_rsp_type * rsp_ptr = NULL;

  set_heap_profile_top_tasks_req_type *req =(set_heap_profile_top_tasks_req_type *) req_pkt;

  rsp_len = sizeof( set_heap_profile_top_tasks_rsp_type);

  /* Avoid buffer over read. */
  if (pkt_len < sizeof(set_heap_profile_top_tasks_req_type))
  {
    return (diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len));
  }
  
  /* Allocate the same length as the request. */
  rsp_ptr = (set_heap_profile_top_tasks_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           SET_HEAP_PROFILE_TOP_TASKS,
                                           rsp_len );

  if (rsp_ptr != NULL)
  {
     if((req->heap_id > magic_num_index) ||  (req->heap_id > MAX_HEAP_INIT))
     {
       rsp_ptr->response = 1;
     }else
     {
       ha_profile.heap_id = req->heap_id;
       ha_profile.is_enabled = TRUE;
       ha_profile.profile_type = PROFILE_TYPE_TOP_TASKS;
       ha_profile.num_tasks = req->num_top_tasks;
       rex_set_timer( &heap_analyzer_timer, log_timer );
       rsp_ptr->response = 0; //SUCCESS
     }
  }

  return (rsp_ptr);

} /* set_heap_profile_top_tasks_cmd_handler */


/*===========================================================================

 FUNCTION set_heap_profile_tasks_cmd_handler

DESCRIPTION
  This function sets the heap analyzer profile for a particular heap. The profile type is the list of tasks in the commad to be 
 profiled.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) set_heap_profile_tasks_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  int rsp_len;
  set_heap_profile_tasks_rsp_type * rsp_ptr = NULL;

  set_heap_profile_tasks_req_type *req =(set_heap_profile_tasks_req_type *) req_pkt;

  rsp_len = sizeof( set_heap_profile_tasks_rsp_type);

  /* Avoid buffer over read. */
  if (pkt_len < sizeof(set_heap_profile_tasks_req_type))
  {
    return (diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len));
  }
  

  /* Allocate the same length as the request. */
  rsp_ptr = (set_heap_profile_tasks_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           SET_HEAP_PROFILE_TASKS,
                                           rsp_len );

  if (rsp_ptr != NULL)
  {
     if(req->num_tasks > HA_MAX_TASK_PROFILE)
     {
        rsp_ptr->response = 1; //ERROR
     }else
     {
        ha_profile.heap_id = req->heap_id;
        ha_profile.is_enabled = TRUE;
        ha_profile.profile_type = PROFILE_TYPE_TASK_IDS;
        ha_profile.num_tasks = req->num_tasks;
        for(int i=0; (i<req->num_tasks && i< HA_MAX_TASK_PROFILE); i++)
        {
           ha_profile.task_ids[i]=req->task_ids[i];
           rex_set_timer( &heap_analyzer_timer, log_timer );
        }
        rsp_ptr->response = 0; //SUCCESS
     }
  }

  return (rsp_ptr);

} /* set_heap_profile_tasks_cmd_handler */

/*===========================================================================

 FUNCTION set_profile_time_cmd_handler

DESCRIPTION
 This function sets the timer value of the interval at which heap analyzer should send out profiling data. .
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) set_profile_time_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  int rsp_len;
  set_heap_profile_time_rsp_type * rsp_ptr = NULL;

  set_heap_profile_time_req_type *req =(set_heap_profile_time_req_type *) req_pkt;

  rsp_len = sizeof( set_heap_profile_time_rsp_type);

  /* Avoid buffer over read. */
  if (pkt_len < sizeof(set_heap_profile_time_req_type))
  {
    return (diagpkt_err_rsp(DIAG_BAD_LEN_F, req_pkt, pkt_len));
  }

  /* Allocate the same length as the request. */
  rsp_ptr = (set_heap_profile_time_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           SET_PROFILE_TIME,
                                           rsp_len );

  if (rsp_ptr != NULL)
  {
     if(req->log_collect_time > HA_MIN_LOG_COLLECT_TIMER)
     {
       log_timer = req->log_collect_time;
       rsp_ptr->response = 0;
     }else
     {
        rsp_ptr->response = 1;
     }
  }

  return (rsp_ptr);

} /* set_heap_profile_tasks_cmd_handler */



/*===========================================================================

 FUNCTION stop_profiling_cmd_handler

DESCRIPTION
 This function stops the heap analyzer profiling.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None


===========================================================================*/
PACK(void *) stop_profiling_cmd_handler
(
  PACK(void *) req_pkt,
  uint16 pkt_len
)
{
  int rsp_len;
  stop_heap_profile_rsp_type * rsp_ptr = NULL;


  rsp_len = sizeof( stop_heap_profile_rsp_type);

  /* Allocate the same length as the request. */
  rsp_ptr = (stop_heap_profile_rsp_type *)diagpkt_subsys_alloc
                                         ( DIAG_SUBSYS_CORE_SERVICES,
                                           STOP_PROFILE,
                                           rsp_len );

  if (rsp_ptr != NULL)
  {
     rsp_ptr->response = 0;
     rex_clr_timer(&heap_analyzer_timer);
  }

  (void)pkt_len;

  return (rsp_ptr);

} /* stop_profiling_cmd_handler */

/* Diag command response table for heap analyzer */
static const diagpkt_user_table_entry_type heap_subsys_diag_tbl[] =
{
  {GET_HEAPS_INFO, GET_HEAPS_INFO, get_heaps_info_cmd_handler},
  {GET_HEAP_TASK_NAMES, GET_HEAP_TASK_NAMES, get_heap_task_names_cmd_handler},
  {SET_HEAP_PROFILE_TASKS,SET_HEAP_PROFILE_TASKS, set_heap_profile_tasks_cmd_handler},
  {SET_HEAP_PROFILE_TOP_TASKS,SET_HEAP_PROFILE_TOP_TASKS, set_heap_profile_top_tasks_cmd_handler},
  {SET_PROFILE_TIME,SET_PROFILE_TIME,set_profile_time_cmd_handler },
  {STOP_PROFILE, STOP_PROFILE, stop_profiling_cmd_handler}
};


/*========================================================
FUNCTION collect_send_heap_generic_logs()

DESCRIPTION
  Send out periodic heap information

=========================================================*/
void collect_send_heap_generic_logs(ha_profile_type *ha_profile)
{
   mem_heap_type *heap_ptr;
   heap_log_info_type * log_ptr;

   if(NULL == ha_profile)
   {
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile is NULL");
      return;
   }
   if((ha_profile->heap_id > magic_num_index) || (ha_profile->heap_id >= MAX_HEAP_INIT)) 
   {
      MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile->heap_id is out of bounds", ha_profile->heap_id);
      return;
   }

   heap_ptr = heap_ptr_table[ha_profile->heap_id];
   if(NULL == heap_ptr)
   { 
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"heap_ptr is NULL");
      return;
   }
   
   MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"Calling log_alloc with size = %d",sizeof(heap_log_info_type));

   log_ptr = (heap_log_info_type *) log_alloc (LOG_HEAP_ANALYZER_HEAP_INFO_LOG_C,
       sizeof(heap_log_info_type));
   if (log_ptr != NULL) 
   {
      log_ptr->version = HEAP_ANALYZER_HEAP_INFO_LOG_VERSION;
      log_ptr->heap_id = ha_profile->heap_id;
      log_ptr->total_blocks = heap_ptr->total_blocks;
      log_ptr->total_bytes = heap_ptr->total_bytes;
      log_ptr->used_bytes = heap_ptr->used_bytes;
      log_ptr->heap_used_bytes = heap_ptr->heap_used_bytes;
      log_ptr->max_used = heap_ptr->max_used;
      log_ptr->max_request = heap_ptr->max_request;
      log_ptr->number_of_sections = heap_ptr->number_of_sections;
      
      log_ptr->largest_free_block_size = (uint32) findLargestBlockSize(heap_ptr);
      if(heap_ptr->heap_used_bytes != 0)
      {
         log_ptr->fragmentation_factor = 100 - (100* log_ptr->largest_free_block_size)/(heap_ptr->total_bytes - heap_ptr->heap_used_bytes);
      }
      log_commit(log_ptr);
   }
   else
   {
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"log_ptr is NULL ");
   }

}


/*========================================================
FUNCTION findTopNUsers()

DESCRIPTION
  Find top num_tasks users for the heap

=========================================================*/
int findTopNUsers(mem_heap_type *heap_ptr, unsigned int num_tasks)
{
   memheap_task_stats_type *stats;
   task_log_info_type *min;
   unsigned int i;

   if(NULL == heap_ptr) 
      return -1;

   min = &top_heap_user_stats[0];

   unsigned int count = 0;
   /* Put first top tasks in the array */
   for(i=0; i< tls_stats_index_count; i++)
   {
      if(0 != heap_ptr->heapStatistics[i].current_usage)
      {
         top_heap_user_stats[count].current_usage = heap_ptr->heapStatistics[i].current_usage;
         top_heap_user_stats[count].max_usage = heap_ptr->heapStatistics[i].max_usage;
         top_heap_user_stats[count].task_id = i;

         if(min->current_usage > heap_ptr->heapStatistics[i].current_usage)
            min = &top_heap_user_stats[count];
         count++;
      }

      if(count >= num_tasks || count >= tls_stats_index_count)
      {
         i++;
         break;
      }
   }

   /* Compare the already added tasks with the other task statistics and update them */
   for(; i< tls_stats_index_count; i++)
   {
      stats = NULL;
      stats=&heap_ptr->heapStatistics[i];
      if(stats->current_usage == 0) 
         continue;

      if(stats->current_usage > min->current_usage)
      /* Replace the minimum and find a new minimum*/
      {
         min->current_usage = heap_ptr->heapStatistics[i].current_usage;
         min->max_usage = heap_ptr->heapStatistics[i].max_usage;
         min->task_id = i;

         min = &top_heap_user_stats[0];
         /* find a new minimum in the top tasks array */
         for(int k=1; k<num_tasks; k++)
         {
            if(min->current_usage > top_heap_user_stats[k].current_usage)
            min = &top_heap_user_stats[k];
         }
      }
   }
   return 0;
}


/*========================================================
FUNCTION handle_log_collect_sig()

DESCRIPTION
  Find the top N users of heap and send out log packets. 

=========================================================*/
void collect_send_top_tasks_usage(ha_profile_type *ha_profile)
{
   task_log_info_type *log_ptr;
   mem_heap_type *heap_ptr;


   if(NULL == ha_profile)
   {
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile is NULL");
      return;
   }
   if((ha_profile->heap_id > magic_num_index) || (ha_profile->heap_id >= MAX_HEAP_INIT)) 
   {
      MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile->heap_id is out of bounds", ha_profile->heap_id);
      return;
   }

   heap_ptr = heap_ptr_table[ha_profile->heap_id];
   if((NULL == heap_ptr) || (heap_ptr->heapStatistics == NULL))
   { 
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"heap_ptr or heapStatistics is NULL");
      return;
   }

   (void)findTopNUsers(heap_ptr, ha_profile->num_tasks);
   MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"Num top tasks = %d",ha_profile->num_tasks);
   MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"log status LOG_HEAP_ANALYZER_TASK_INFO_LOG_C= %d",log_status(LOG_HEAP_ANALYZER_TASK_INFO_LOG_C));
   for(int i=0; i< ha_profile->num_tasks; i++)
   {
     
     log_ptr = (task_log_info_type *) log_alloc (LOG_HEAP_ANALYZER_TASK_INFO_LOG_C, sizeof(task_log_info_type));
     MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"Calling log alloc with size = %d",sizeof(task_log_info_type));
     if (log_ptr != NULL) 
     {
        log_ptr->version = HEAP_ANALYZER_TASK_INFO_LOG_VERSION;
        log_ptr->current_usage = top_heap_user_stats[i].current_usage;
        log_ptr->max_usage = top_heap_user_stats[i].max_usage;
        log_ptr->task_id  = top_heap_user_stats[i].task_id;
        log_ptr->heap_id = ha_profile->heap_id;
        /* Setting this to zero for now update this when quota related changes are ready */
		log_ptr->quota  = 0; 
        log_commit(log_ptr);
     }
     else
     {
        MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"log_alloc returned NULL");
        break;
     }
   }

}


void collect_send_task_usage(ha_profile_type *ha_profile)
{
   task_log_info_type *log_ptr;
   mem_heap_type *heap_ptr;

   if(NULL == ha_profile)
   {
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile is NULL");
      return;
   }
   if((ha_profile->heap_id > magic_num_index) || (ha_profile->heap_id >= MAX_HEAP_INIT)) 
   {
      MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"ha_profile->heap_id is out of bounds", ha_profile->heap_id);
      return;
   }

   heap_ptr = heap_ptr_table[ha_profile->heap_id];
   if((NULL == heap_ptr) || (heap_ptr->heapStatistics == NULL))
   { 
      MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"heap_ptr or heapStatistics is NULL");
      return;
   }

   MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"log status LOG_HEAP_ANALYZER_TASK_INFO_LOG_C= %d",log_status(LOG_HEAP_ANALYZER_TASK_INFO_LOG_C));
   for(int i=0; i< ha_profile->num_tasks; i++)
   {
     if(ha_profile->task_ids[i] < MEMHEAP_MAX_THREADS)
     {
        log_ptr = (task_log_info_type *) log_alloc (LOG_HEAP_ANALYZER_TASK_INFO_LOG_C, sizeof(task_log_info_type));
        MSG_1(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"Calling log alloc with size = %d",sizeof(task_log_info_type));
        if (log_ptr != NULL) 
        {
           log_ptr->version = HEAP_ANALYZER_TASK_INFO_LOG_VERSION;
           log_ptr->current_usage = heap_ptr->heapStatistics[ha_profile->task_ids[i]].current_usage;
           log_ptr->max_usage = heap_ptr->heapStatistics[ha_profile->task_ids[i]].max_usage;
           log_ptr->task_id  = ha_profile->task_ids[i];
           log_ptr->heap_id = ha_profile->heap_id;
           log_commit(log_ptr);
        }
        else
        {
           MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"log_alloc returned NULL");
           break;
        }
      }else
      {
         MSG(MSG_SSID_MEMHEAP,MSG_LEGACY_ERROR,"task id index out of bounds");
      }

   }
}

/*========================================================
FUNCTION handle_log_collect_sig()

DESCRIPTION
  Handle the log generate timer signal 

DEPENDENCIES
  None

PARAMETERS
 

RETURN VALUE
  None

SIDE EFFECTS
  None
=========================================================*/
void handle_log_collect_sig( void)
{
   MSG_HIGH("Collect logs",0,0,0);
   collect_send_heap_generic_logs(&ha_profile);

   switch (ha_profile.profile_type)
   {
      case PROFILE_TYPE_TOP_TASKS: 
        collect_send_top_tasks_usage(&ha_profile);
        break;

      case PROFILE_TYPE_TASK_IDS: 
        collect_send_task_usage(&ha_profile);
        break;
   }

   //Set the timer again
   rex_set_timer( &heap_analyzer_timer, log_timer );
}



/*========================================================
FUNCTION heap_analyzer_init()

DESCRIPTION
  Register diag command table

DEPENDENCIES
  None

PARAMETERS
 

RETURN VALUE
  None

SIDE EFFECTS
  None
=========================================================*/
void heap_analyzer_init( void)
{
  DIAGPKT_DISPATCH_TABLE_REGISTER (DIAG_SUBSYS_CORE_SERVICES, heap_subsys_diag_tbl);
}




/*===========================================================================
FUNCTION heap_analyzer_task_main()

DESCRIPTION
  HEAP_ANALYZER task entry point

DEPENDENCIES
  None

PARAMETERS
 

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/   
void heap_analyzer_task
(
  /* Unused parameter - required to match rex_task_func_type prototype */
  dword                           ignored    /*lint -esym(715,ignored) */
)
{

    /* Signals returned from rex wait procedure */
    rex_sigs_type sigs;

    rcinit_handshake_startup();

    heap_analyzer_tcb_ptr = rex_self();
    
    /* Initialize timers */
    rex_def_timer( &heap_analyzer_timer, heap_analyzer_tcb_ptr, HEAP_ANALYZER_TIMER_SIG );
    heap_analyzer_init();
    
    for (;;)
    {
      sigs = rex_wait
             (
               HEAP_ANALYZER_TIMER_SIG
             );
      (void) rex_clr_sigs( heap_analyzer_tcb_ptr, sigs );

      /* Collect and send logs */
      if( sigs & HEAP_ANALYZER_TIMER_SIG )
      {
        handle_log_collect_sig();
        MSG_HIGH("Received the timer signal",0,0,0);
      }
    }
    
  } /* heap_analyzer_task */


