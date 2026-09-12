/**
   @file memory_stats.c
   @author pgoswami
   @brief memory stats reporting for timeline issues 

   Copyright (c) 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
   */

#include "err.h"
#include "qurt.h"
#include "qurt_tlb.h"
#include "ULogFront.h"
#include "memheap.h"
#include "dsm.h"

/* Generic profile struct which could be used 
   to profile any modem_mem* APIs */

#ifdef FEATURE_HEAP_ALLOC_PROFILE

#ifndef MODEM_ALLOC_LOG_MAX
#define MODEM_ALLOC_LOG_MAX 1024
#endif

typedef struct modem_mem_profile {
	/* Delta before and after the operation */
	uint32 time_delta;
	/* caller pointer */
	void * caller_ptr;
	/* Thread ID */
	unsigned long  task_id;
} modem_mem_profile_t;


typedef struct perf_modem_mem_profile {
	/* Delta before and after the operation */
	uint32 avg_time_delta;
	uint32 max_time_delta;
	uint32 min_time_delta;
	uint32 max_time_microseconds;
	uint32 min_time_microseconds;
	uint32 avg_time_microseconds;
	/* caller pointer */
	void * caller_ptr;
	/* Thread ID */
	unsigned long  task_id;
} perf_modem_mem_profile;

perf_modem_mem_profile perf_memory_stats;
extern modem_mem_profile_t modem_alloc_log[MODEM_ALLOC_LOG_MAX];

#endif/* FEATURE_HEAP_ALLOC_PROFILE */

extern mem_heap_type modem_mem_heap;
extern memheap_task_stats_type modem_heap_task_stats[MEMHEAP_MAX_THREADS];

char memheap_tls_to_tname[MEMHEAP_MAX_THREADS][QURT_THREAD_ATTR_NAME_MAXLEN];

#ifdef FEATURE_SPLIT_MODEM_HEAP
extern mem_heap_type modem_internal_mem_heap;
memheap_task_stats_type modem_internal_heap_task_stats[MEMHEAP_MAX_THREADS];
#endif

/* DSM memory pools */
extern dsm_pool_mgmt_table_type dsm_ds_small_item_pool;
extern dsm_pool_mgmt_table_type dsm_ds_large_item_pool;
extern dsm_pool_mgmt_table_type dsm_dup_item_pool;
extern dsm_pool_mgmt_table_type dsm_ds_sl_item_pool;

/** @brief ULog handle for this module */
static ULogHandle ulogh;


static void dump_dsm_memory_stats(void)
{
	int i =0;
	dsm_mem_event_cb_entry *mem_event_cb_ptr;
	ULOG_RT_PRINTF_0(ulogh,"============DSM Memory Stats ============ ");
	ULOG_RT_PRINTF_0(ulogh,"\n\n\n");
	ULOG_RT_PRINTF_0(ulogh,"==Super large pool stats ==\n");



	ULOG_RT_PRINTF_4( ulogh, "alloc cnt --> %u   ,free cnt  --> %u ,min_free_count -->  %u ,alloc_fail -->%u",dsm_ds_sl_item_pool.alloc_count,
			dsm_ds_sl_item_pool.free_count,dsm_ds_sl_item_pool.stats_min_free_count,
			dsm_ds_sl_item_pool.pool_stats->stats_num_out_of_memory);		

	i=0;
	while(dsm_ds_sl_item_pool.mem_event_cb_list[i].level !=  DSM_MEM_LEVEL_INVALID)
	{

		mem_event_cb_ptr = &dsm_ds_sl_item_pool.mem_event_cb_list[i];
		ULOG_RT_PRINTF_3( ulogh, "memory level--> %u , threshold --> %u ,cb_count --> %u",mem_event_cb_ptr->level,
				mem_event_cb_ptr->avail_item_count,mem_event_cb_ptr->cb_count);
		i++;
	}
	ULOG_RT_PRINTF_0(ulogh,"\n\n\n"); 	
	ULOG_RT_PRINTF_0(ulogh,"==Large pool stats ==\n");



	ULOG_RT_PRINTF_4( ulogh, "alloc cnt --> %u, free cnt --> %u , min_free_count --> %u ,alloc_fail --> %u",dsm_ds_large_item_pool.alloc_count,
			dsm_ds_large_item_pool.free_count,dsm_ds_large_item_pool.stats_min_free_count,
			dsm_ds_large_item_pool.pool_stats->stats_num_out_of_memory); 	   

	i=0;
	while(dsm_ds_large_item_pool.mem_event_cb_list[i].level !=  DSM_MEM_LEVEL_INVALID)
	{

		mem_event_cb_ptr = &dsm_ds_large_item_pool.mem_event_cb_list[i];   
		ULOG_RT_PRINTF_3( ulogh, "memory level--> %u , threshold --> %u ,cb_count --> %u",mem_event_cb_ptr->level,
				mem_event_cb_ptr->avail_item_count,mem_event_cb_ptr->cb_count);
		i++;
	}
	ULOG_RT_PRINTF_0(ulogh,"\n\n");
	ULOG_RT_PRINTF_0(ulogh,"==Small pool stats ==\n");

	ULOG_RT_PRINTF_4( ulogh, "alloc cnt --> %u ,free cnt --> %u ,min_free_count -->%u ,alloc_fail --> %u",dsm_ds_small_item_pool.alloc_count,
			dsm_ds_small_item_pool.free_count,dsm_ds_small_item_pool.stats_min_free_count,
			dsm_ds_small_item_pool.pool_stats->stats_num_out_of_memory);	   

	i=0;
	while(dsm_ds_small_item_pool.mem_event_cb_list[i].level !=	DSM_MEM_LEVEL_INVALID)
	{

		mem_event_cb_ptr = &dsm_ds_small_item_pool.mem_event_cb_list[i];	
		ULOG_RT_PRINTF_3( ulogh, "memory level--> %u , threshold --> %u ,cb_count --> %u",mem_event_cb_ptr->level,
				mem_event_cb_ptr->avail_item_count,mem_event_cb_ptr->cb_count);
		i++;
	}

	ULOG_RT_PRINTF_0(ulogh,"\n\n");
	ULOG_RT_PRINTF_0(ulogh,"==Dup pool stats ==\n");



	ULOG_RT_PRINTF_4( ulogh, "alloc cnt --> %u , free cnt --> %u, min_free_count --> %u, alloc_fail --> %u",dsm_dup_item_pool.alloc_count,
			dsm_dup_item_pool.free_count,dsm_dup_item_pool.stats_min_free_count,
			dsm_dup_item_pool.pool_stats->stats_num_out_of_memory);	   

	i=0;
	while(dsm_dup_item_pool.mem_event_cb_list[i].level !=	DSM_MEM_LEVEL_INVALID)
	{

		mem_event_cb_ptr = &dsm_dup_item_pool.mem_event_cb_list[i];	
		ULOG_RT_PRINTF_3( ulogh, "memory level--> %u , threshold --> %u ,cb_count --> %u",mem_event_cb_ptr->level,
				mem_event_cb_ptr->avail_item_count,mem_event_cb_ptr->cb_count);
		i++;
	}


}


static void memory_statas_reporting (void)
{
	uint32 temp_current_usage =0;

	int i = 0;
#ifdef FEATURE_HEAP_ALLOC_PROFILE
	for(i=0; i < (sizeof(modem_alloc_log)/sizeof(modem_alloc_log[0]));i++)
	{

		perf_memory_stats.avg_time_delta += modem_alloc_log[i].time_delta;

		if(perf_memory_stats.max_time_delta < modem_alloc_log[i].time_delta)
		{
			perf_memory_stats.max_time_delta = modem_alloc_log[i].time_delta; 
			perf_memory_stats.caller_ptr = modem_alloc_log[i].caller_ptr;
			perf_memory_stats.task_id= modem_alloc_log[i].task_id;
		}
		else
		{
			perf_memory_stats.min_time_delta = modem_alloc_log[i].time_delta; 
		}
	}

	perf_memory_stats.avg_time_delta = perf_memory_stats.avg_time_delta/(sizeof(modem_alloc_log)/sizeof(modem_alloc_log[0]));
	perf_memory_stats.avg_time_microseconds = perf_memory_stats.avg_time_delta/19.2; 
	perf_memory_stats.min_time_microseconds = perf_memory_stats.min_time_delta/19.2;
	perf_memory_stats.max_time_microseconds = perf_memory_stats.max_time_delta/19.2;

	ULOG_RT_PRINTF_4( ulogh, " max xo tick alloc delta --> %u, task id --> %u ,task name --> %s,  caller symbol -->%p",perf_memory_stats.max_time_delta,
			perf_memory_stats.task_id,memheap_tls_to_tname[perf_memory_stats.task_id],perf_memory_stats.caller_ptr);   	

	ULOG_RT_PRINTF_3( ulogh, "xo ticks delta : max alloc --> %u ,avg alloc --> %u, min alloc --> %u ",perf_memory_stats.max_time_delta,
			perf_memory_stats.avg_time_delta,perf_memory_stats.min_time_delta);   	


	ULOG_RT_PRINTF_3( ulogh, "micro seconds : max alloc --> %u ,avg alloc --> %u ,min alloc --> %u \n\n ",perf_memory_stats.max_time_microseconds,
			perf_memory_stats.avg_time_microseconds,perf_memory_stats.min_time_microseconds);   	

#endif

	ULOG_RT_PRINTF_4( ulogh, "modem primary heap: total nodes --> %u ,total bytes --> %u ,heap bytes --> %u max used  --> %u \n",modem_mem_heap.total_blocks,modem_mem_heap.total_bytes,
			modem_mem_heap.heap_used_bytes,modem_mem_heap.max_used);  
    #ifdef FEATURE_SPLIT_MODEM_HEAP
	ULOG_RT_PRINTF_4( ulogh, "modem secondary heap: total nodes --> %u ,total bytes --> %u ,heap bytes --> %u max used  --> %u \n",modem_internal_mem_heap.total_blocks,modem_internal_mem_heap.total_bytes,
			modem_internal_mem_heap.heap_used_bytes,modem_internal_mem_heap.max_used);  
    #endif        

	ULOG_RT_PRINTF_0(ulogh,"============Task wise modem heap allocation details ============ \n");
	ULOG_RT_PRINTF_0(ulogh,"task id , taskname , curreent used memory , max used memory ");

	i = 0;		
	while (i < MEMHEAP_MAX_THREADS)
	{


		if(modem_heap_task_stats[i].max_usage > (0.5*1024*1024))
		{



			ULOG_RT_PRINTF_4( ulogh, "%u,%s ,%uKB ,%uKB",i,memheap_tls_to_tname[i],
					modem_heap_task_stats[i].current_usage/1024,modem_heap_task_stats[i].max_usage/1024);	  

		}
		i++;
	}

	ULOG_RT_PRINTF_0(ulogh,"\n\n");
    
    #ifdef FEATURE_SPLIT_MODEM_HEAP
	ULOG_RT_PRINTF_0(ulogh,"============Task wise modem internal heap allocation details ============ ");
	ULOG_RT_PRINTF_0(ulogh,"task id ,taskname  curreent used memory , max used memory ");

	i = 0;		
	while (i < MEMHEAP_MAX_THREADS)
	{


		if(modem_internal_heap_task_stats[i].max_usage > (0.5*1024*1024))
		{

			ULOG_RT_PRINTF_4( ulogh, "%u,%s ,%uKB ,%uKB",i,memheap_tls_to_tname[i],
					modem_internal_heap_task_stats[i].current_usage/1024,modem_internal_heap_task_stats[i].max_usage/1024);	  

		}
		i++;
	}
    #endif
	ULOG_RT_PRINTF_0(ulogh,"\n\n");
   
	dump_dsm_memory_stats();
}

void perf_memory_stats_rcinit (void)
{

	/* Register for Error Call Back when Modem Crashes and print ULOG */

	err_crash_cb_register(&memory_statas_reporting);
	if(ULogFront_RealTimeInit( &ulogh, "Perf Memory stats", 0x4000, ULOG_MEMORY_LOCAL, ULOG_LOCK_OS ) != 0)
		MSG_ERROR("Error in Creating ULOG Handle for Memory Stats", 0, 0, 0);
}