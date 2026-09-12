/*==========================================================================
 * FILE:         dlpager_stats.c
 *
 * SERVICES:     DL PAGER STATISTICS
 *
 * DESCRIPTION:  This file provides functionality to generate statistics
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      First version
===========================================================================*/
#include <dlpager_stats.h>
#include <atomic_ops.h>

#define NUM_THREAD_ID_BITS      9    // XXX Assumption: 9 bits for thread id
#define MAX_THREADS             (1 << NUM_THREAD_ID_BITS)
#define THREAD_ID_MASK          (MAX_THREADS - 1)    

#define HIST_FAULTRESUME_SIZE  100     // 100-bin hisogram. General case shouldn't exceed idx of 100
#define HIST_LRU_STEPS_SIZE     25      // 25-bucket histogram. General case shouldn't exceed idx of 25

// Number of bits to right shift fault-resume times.
// A little profiling shows the difference tends to be
// between 20k and 35k. With a 100-bin histogram, that
// means we can safely chop off about 3 zeros
#define FAULTRESUME_SHIFT_BITS 10      

#define TIMER_ENTRIES_BITS  13
#define TIMER_ENTRIES_MASK  ((1<<TIMER_ENTRIES_BITS)-1)
#define TIMER_ENTRIES_MAX   (1<<TIMER_ENTRIES_BITS) /**< No. of log entries */


// General stats declaration
// Always record.
struct dlpager_stats_s dlpager_stats;

#if DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING
// temporary buffer for capturing starting pcycle times for faulting threads
static unsigned long long faultresume_pcycle_buf[MAX_THREADS];        

// histogram for collecting time from fault to resume on a per-task basis
static unsigned long dlpager_stats_hist_faultresume_time[HIST_FAULTRESUME_SIZE];
#endif /* DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING */


#if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
typedef enum
{
    AWAITING_IPA_REQUEST,
    AWAITING_IPA_COMPLETE,
    AWAITING_RESUME,
    COMPLETE
} dlpager_stats_tofromipa_state_e;

typedef struct
{
    qurt_thread_t                     task_id;
    dlpager_stats_tofromipa_state_e   state;
    unsigned long long                fault_to_ipa_time;
    unsigned long long                ipa_to_resume_time;
} dlpager_stats_tofromipa_entry_t;

/*
 *   Used for profiling two times:
 *     1. From initial fault to ipa request
 *     2. From completion of ipa request to resume
 *   Not intended for full-time use.
 *   TODO: Use asm to speed up sysclock calls.
 *         See qurt_timer_hw_asm.S for hw_timer_curr_timetick()
 */
static dlpager_stats_tofromipa_entry_t dlpager_stats_tofromipa[TIMER_ENTRIES_MAX];

// temporary storage that associates any currently faulting task_id
// with an index into dlpager_stats_tofromipa[]
static unsigned int task_to_idx[MAX_THREADS];

// index into dlpager_stats_tofromipa[].
// starts at -1 (unsigned) but rolls over to 0 on first increment.
static atomic_word_t dlpager_stats_tofromipa_idx = ATOMIC_INIT(-1);
#endif /* DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING */



#if DLPAGER_STATS_ENABLE_LRU_HIST
static unsigned long long dlpager_stats_hist_lru_steps[HIST_LRU_STEPS_SIZE];
#endif



#if DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING
inline void dlpager_stats_faultresumetimer_start(qurt_thread_t tid)
{
    tid &= THREAD_ID_MASK;
    faultresume_pcycle_buf[tid] = qurt_get_core_pcycles();
}


inline void dlpager_stats_faultresumetimer_stop(qurt_thread_t tid)
{
    unsigned long long end_pcycles;
    unsigned long long faultresume_time;

    tid &= THREAD_ID_MASK;

    end_pcycles = qurt_get_core_pcycles();
    faultresume_time = end_pcycles - faultresume_pcycle_buf[tid];
    faultresume_time = faultresume_time >> FAULTRESUME_SHIFT_BITS;

    if( faultresume_time < HIST_FAULTRESUME_SIZE )
    {
        dlpager_stats_hist_faultresume_time[faultresume_time]++;
    }
    else
    {
    dlpager_stats_hist_faultresume_time[HIST_FAULTRESUME_SIZE-1]++;
    }
}
#endif /* DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING */

#if DLPAGER_STATS_ENABLE_LRU_HIST
inline void dlpager_stats_lrusteps_store(int num_steps)
{
    if( num_steps < HIST_LRU_STEPS_SIZE )
    {
        dlpager_stats_hist_lru_steps[num_steps]++;
    }
    else
    {
        dlpager_stats_hist_lru_steps[HIST_LRU_STEPS_SIZE-1]++;
    }
}
#endif


#if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
/*
 * Start timer for profiling time from task fault to ipa request.
 */
inline void dlpager_stats_fault_to_ipa_start(qurt_thread_t task_id)
{
    unsigned int idx = atomic_inc_return(&dlpager_stats_tofromipa_idx) & TIMER_ENTRIES_MASK;
    task_id &= THREAD_ID_MASK;

    // save the current idx associated with this task_id
    task_to_idx[task_id] = idx;

    dlpager_stats_tofromipa[idx].task_id = task_id;
    dlpager_stats_tofromipa[idx].state = AWAITING_IPA_REQUEST;

    // temporarily store starting time for fault-to-ipa
    dlpager_stats_tofromipa[idx].fault_to_ipa_time = qurt_sysclock_get_hw_ticks();
}

/*
 * Stop timer for profiling time from task fault to ipa request.
 */
inline void dlpager_stats_fault_to_ipa_stop(qurt_thread_t task_id)
{
    task_id &= THREAD_ID_MASK;
    unsigned int idx = task_to_idx[task_id];
    dlpager_stats_tofromipa[idx].state = AWAITING_IPA_COMPLETE;
    dlpager_stats_tofromipa[idx].fault_to_ipa_time = qurt_sysclock_get_hw_ticks() - dlpager_stats_tofromipa[idx].fault_to_ipa_time;
}

/*
 * Start timer for profiling time from IPA request completion to task resumption.
 */
inline void dlpager_stats_ipa_to_resume_start(qurt_thread_t task_id)
{
    task_id &= THREAD_ID_MASK;
    unsigned int idx = task_to_idx[task_id];

    dlpager_stats_tofromipa[idx].state = AWAITING_RESUME;

    // temporarily store starting time for ipa-to-resume
    dlpager_stats_tofromipa[idx].ipa_to_resume_time = qurt_sysclock_get_hw_ticks();
}

/*
 * Stop timer for profiling time from IPA request completion to task resumption.
 */
inline void dlpager_stats_ipa_to_resume_stop(qurt_thread_t task_id)
{
    task_id &= THREAD_ID_MASK;
    unsigned int idx = task_to_idx[task_id];
    if(dlpager_stats_tofromipa[idx].state == AWAITING_RESUME)
    {
        dlpager_stats_tofromipa[idx].state = COMPLETE;
        dlpager_stats_tofromipa[idx].ipa_to_resume_time =
            qurt_sysclock_get_hw_ticks() - dlpager_stats_tofromipa[idx].ipa_to_resume_time;
    }
}
#endif
