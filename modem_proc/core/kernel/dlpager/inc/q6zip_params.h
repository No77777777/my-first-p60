#pragma once
/**
   @file q6zip_params.h 
   @author anandj 
   @brief Q6ZIP tunable parameters 
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#if !defined( Q6ZIP_PARAM_IPA_MAX_DEF_PRIO_REQS )
/** @brief Maximum outstanding default priority requests to IPA.
    @warning Its value *MUST* be a power-of-two number */
#define Q6ZIP_PARAM_IPA_MAX_DEF_PRIO_REQS 2
#endif

#if !defined( Q6ZIP_PARAM_IPA_MAX_HI_PRIO_REQS )
/** @brief Maximum outstanding high priority requests to IPA
    @warning Its value *MUST* be a power-of-two number */
#define Q6ZIP_PARAM_IPA_MAX_HI_PRIO_REQS  2
#endif

/** @brief Number of Q6ZIP context to account for SW slack.
    @warning This number needs to be tuned on target */
#define Q6ZIP_PARAM_CONTEXT_NUM_ITEMS_FOR_SW_SLACK 4

/** @brief Maximum number of Q6ZIP contexts. One context item is associated with
    one request and response to/from IPA ZIP. After response, a context may also
    be held by DL pager for any post-processing */
#define Q6ZIP_PARAM_CONTEXT_MAX_ITEMS     \
    ( Q6ZIP_PARAM_IPA_MAX_DEF_PRIO_REQS + \
      Q6ZIP_PARAM_IPA_MAX_HI_PRIO_REQS  + \
      Q6ZIP_PARAM_CONTEXT_NUM_ITEMS_FOR_SW_SLACK )


/** @brief Stack size of a Q6ZIP_SW worker thread */
#define Q6ZIP_PARAM_SW_WORKER_STACK_SIZE (4 * 1024)

/** @brief Task priority of SW worker that services high priority requests */
#define Q6ZIP_PARAM_SW_WORKER_HI_TASK_PRIORITY 100

/** @brief Task priority of SW worker that services low priority requests */
#define Q6ZIP_PARAM_SW_WORKER_LO_TASK_PRIORITY 150

/** @brief Stack size of Q6ZIP_IPA async initialization worker thread */
#define Q6ZIP_PARAM_IPA_INIT_WORKER_STACK_SIZE (4 * 1024)

/** @brief Priority of Q6ZIP_IPA async initialization worker thread */
#define Q6ZIP_PARAM_IPA_INIT_WORKER_PRIORITY 106

#if !defined( Q6ZIP_PARAM_IPA_REGISTRATION_TIMEOUT )
/** @brief Maximum time (microseconds) allowed for IPA registration to
           complete. 0 means no timeout (wait indefinitely) */
#define Q6ZIP_PARAM_IPA_REGISTRATION_TIMEOUT 0
#endif

/** @brief Maximum time (microseconds) allowed for IPA to load dictionary */
#define Q6ZIP_PARAM_IPA_DICT_LOAD_TIMEOUT    250000

/** @brief Maximum time (microseconds) for IPA error to propagate to APSS */
#define Q6ZIP_PARAM_IPA_ERROR_PROPAGATION_DURATION 100000
