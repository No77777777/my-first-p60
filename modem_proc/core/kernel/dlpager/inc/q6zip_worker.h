#pragma once
/**
   @file q6zip_worker.h 
   @author anandj 
   @brief Q6ZIP worker is a convenient wrapper on QuRT threads
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "qurt.h"

/**
   @brief Worker entry 
   @param [in] argument Argument provided during q6zip_worker_fork 
 */
typedef void * (*q6zip_worker_entry_fn_t)(void * argument);

/** @brief Datatype for a worker thread */
typedef struct q6zip_worker_s
{
    qurt_thread_t           thread;
    q6zip_worker_entry_fn_t entry;
    unsigned int            priority;
    void *                  argument;
    void *                  stack;

} q6zip_worker_t;

/**
 * @brief Start a worker. It quits once it is done. Until then, cannot mess 
 * with it! 
 * @param [in] worker Worker 
 * @param [in] name Name of worker (for debugability) 
 * @param [in] stack_size Size (bytes) of the worker thread's stack 
 * @param [in] entry Entry into the worker function
 * @param [in] priority Priority of the worker thread 
 * @param [in] argument Argument to worker thread 
 */
extern void q6zip_worker_fork (
    q6zip_worker_t *        worker,
    char const *            name,
    unsigned int            stack_size,
    q6zip_worker_entry_fn_t entry, 
    unsigned int            priority,
    void *                  argument
);

/**
 * @brief Join with a worker
 * @param [in] worker Worker previously forked using q6zip_worker_fork()
 */
extern void q6zip_worker_join (q6zip_worker_t * worker);
