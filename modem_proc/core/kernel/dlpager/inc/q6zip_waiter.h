#pragma once
/**
   @file q6zip_waiter.h 
   @author anandj 
   @brief Q6ZIP waiter is a convenient synchronization object
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "comdef.h"
#include "qurt.h"
#include <pthread.h>

typedef struct
{
    union
    {
        struct
        {
            pthread_cond_t  cond;
            pthread_mutex_t mutex;

        } conditional;

        struct
        {
            qurt_signal_t object;

        } signal;

    } impl;

    boolean         woken;

} q6zip_waiter_t;

extern void q6zip_waiter_init (q6zip_waiter_t * waiter);
extern void q6zip_waiter_wait (q6zip_waiter_t * waiter);
extern void q6zip_waiter_wake (q6zip_waiter_t * waiter);
