
/*
 * dlpager_qdprofile.c
 *   Used for profiling two times:
 *     1. From initial fault to ipa request
 *     2. From completion of ipa request to resume
 *  
 *   Not intended for full-time use.
 *   TODO: Use asm to speed up sysclock calls.
 *         See qurt_timer_hw_asm.S for hw_timer_curr_timetick()
 */

#include <dlpager_qdprofile.h>
#include <atomic_ops.h>

#define NUM_THREAD_ID_BITS      9    // XXX Assumption: 9 bits for thread id
#define MAX_THREADS             (1 << NUM_THREAD_ID_BITS)
#define THREAD_ID_MASK          (MAX_THREADS - 1)    

#define DLPAGER_QDPROFILE_ENTRIES_BITS  13
#define DLPAGER_QDPROFILE_ENTRIES_MASK  ((1<<DLPAGER_QDPROFILE_ENTRIES_BITS)-1)
#define DLPAGER_QDPROFILE_ENTRIES_MAX   (1<<DLPAGER_QDPROFILE_ENTRIES_BITS) /**< No. of log entries */

typedef enum{
        AWAITING_IPA_REQUEST,
        AWAITING_IPA_COMPLETE,
        AWAITING_RESUME,
        COMPLETE
}dlpager_qdprofile_state_e;

typedef struct{
        qurt_thread_t   task_id;
        unsigned int    fault_to_ipa_time;  // FIXME type
        unsigned int    ipa_to_resume_time; // FIXME type
        dlpager_qdprofile_state_e state;
}dlpager_qdprofile_entry_t;

#if DLPAGER_QDPROFILE
// temporary storage that associates any currently faulting task_id with a log_idx
unsigned int dlpager_qdprofile_task_to_log[MAX_THREADS];

// the log
dlpager_qdprofile_entry_t dlpager_qdprofile[DLPAGER_QDPROFILE_ENTRIES_MAX];

// index into the log. starts at -1 (unsigned) but rolls over to 0 on first increment.
atomic_word_t dlpager_qdprofile_log_idx = ATOMIC_INIT(-1);
#endif




/*
 * Start timer for profiling time from task fault to ipa request.
 */
inline
void dlpager_qdprofile_fault_to_ipa_start(qurt_thread_t task_id)
{
#if DLPAGER_QDPROFILE
        unsigned int log_idx = atomic_inc_return(&dlpager_qdprofile_log_idx) & DLPAGER_QDPROFILE_ENTRIES_MASK;
        task_id &= THREAD_ID_MASK;

        // save the current log_idx associated with this task_id
        dlpager_qdprofile_task_to_log[task_id] = log_idx;

        dlpager_qdprofile[log_idx].task_id = task_id;
        dlpager_qdprofile[log_idx].state = AWAITING_IPA_REQUEST;

        // temporarily store starting time for fault-to-ipa
        dlpager_qdprofile[log_idx].fault_to_ipa_time = qurt_sysclock_get_hw_ticks();
#endif
}

/*
 * Stop timer for profiling time from task fault to ipa request.
 */
inline
void dlpager_qdprofile_fault_to_ipa_stop(qurt_thread_t task_id)
{
#if DLPAGER_QDPROFILE
        task_id &= THREAD_ID_MASK;
        unsigned int log_idx = dlpager_qdprofile_task_to_log[task_id];
        dlpager_qdprofile[log_idx].state = AWAITING_IPA_COMPLETE;
        dlpager_qdprofile[log_idx].fault_to_ipa_time = qurt_sysclock_get_hw_ticks() - dlpager_qdprofile[log_idx].fault_to_ipa_time;
#endif
}

/*
 * Start timer for profiling time from IPA request completion to task resumption.
 */
inline
void dlpager_qdprofile_ipa_to_resume_start(qurt_thread_t task_id)
{
#if DLPAGER_QDPROFILE
        task_id &= THREAD_ID_MASK;
        unsigned int log_idx = dlpager_qdprofile_task_to_log[task_id];

        dlpager_qdprofile[log_idx].state = AWAITING_RESUME;

        // temporarily store starting time for ipa-to-resume
        dlpager_qdprofile[log_idx].ipa_to_resume_time = qurt_sysclock_get_hw_ticks();
#endif
}

/*
 * Stop timer for profiling time from IPA request completion to task resumption.
 */
inline
void dlpager_qdprofile_ipa_to_resume_stop(qurt_thread_t task_id)
{
#if DLPAGER_QDPROFILE
        task_id &= THREAD_ID_MASK;
        unsigned int log_idx = dlpager_qdprofile_task_to_log[task_id];
        if(dlpager_qdprofile[log_idx].state == AWAITING_RESUME){
                dlpager_qdprofile[log_idx].state = COMPLETE;
                dlpager_qdprofile[log_idx].ipa_to_resume_time =
                        qurt_sysclock_get_hw_ticks() - dlpager_qdprofile[log_idx].ipa_to_resume_time;
        }

#endif
}
