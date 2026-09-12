#pragma once
/**
   @file q6zip_context.h 
   @author anandj 
   @brief Q6ZIP context abstract datatype and the functions that operate on it
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "ipa_api.h"
#include "q6zip_assert.h"
#include "q6zip_context_opaque.h"
#include "q6zip_request.h"
#include "q6zip_types.h"
#include "queue.h"

/**
   @brief Q6ZIP contextual information 
 */
struct q6zip_context_s
{
    /** @brief Link for use with a queue. *MUST* be first member! */
    q_link_type link;

    /** @brief Magic constant to ensure that this context is de-referenced
               correctly */
    unsigned char * magic;

    /** @brief Request */
    q6zip_request_t request;

    /** @brief Response */
    q6zip_response_t response;

    #if 1
    /** @todo anandj As per discussions with CRD folks, this can be removed.
        DL pager will be performing time sampling */
    /**
     * @brief Per-request statistics 
     * @warning These will be filled by the Q6ZIP implementation and *MUST NOT* 
     * be modified by user 
     */
    struct
    {
        /** @brief P-cycle time pair
        q6zip_time_pair_t pcycle;*/

        /** @brief system clock time pair */
        q6zip_time_pair_t sclock;

    } stats;
    #endif

    /** @brief Meta-information for Q6ZIP's internal usage */
    union
    {
        struct
        {
            /** @brief IPA request */
            ipa_zip_cmd_req_type_s req;

            /** @brief IPA queue the request was submitted to */
            unsigned char queue_id;

            /** @brief Sequence number associated with this context. Useful for
                       correlating Q6ZIP logs off-target */
            unsigned char seqno;

        } ipa;

        struct
        {
            /** @brief Sequence number associated with this context. Useful for
                       correlating Q6ZIP logs off-target */
            unsigned char seqno;

        } sw;

    } meta;

    /** @brief Implementation used */
    q6zip_impl_t impl;

};

typedef struct
{
    struct
    {
        q_type free;
    } pool;

    boolean initialized;

} q6zip_context_module_t;

/**
   @brief Q6ZIP context module. Externalized for inlining some operations 
   @warning *DONT* use it directly! 
 */
extern q6zip_context_module_t q6zip_context_module;

/**
 * @brief Initialize Q6ZIP context module 
 *  
 * @warning *MUST* be invoked once before using other functions in this module
 */
extern void q6zip_context_module_init (void);

/**
 * @brief Cleanup Q6ZIP context module
 */
extern void q6zip_context_module_fini (void);

/**
 * @brief Allocate a Q6ZIP context
 * @return q6zip_context_t * on success or NULL on failure
 */
static inline q6zip_context_t * q6zip_context_alloc (void)
{
    q6zip_context_t * context = q_get( &q6zip_context_module.pool.free );

    if ( Q6ZIP_LIKELY( context != NULL ) )
    {
        context->magic -= 1;

        /* Reset priority. This way if DL pager doesnt pick a priority, we are
           not incorrectly using a stale selection */
        context->request.priority = Q6ZIP_FIXED_PRIORITY_IMPL;
    }

    return context;
}

/**
 * @brief De-allocate the Q6ZIP context
 * @param [in] context Double-pointer to the context
 */
static inline void q6zip_context_free (q6zip_context_t ** context)
{
    if ( Q6ZIP_LIKELY( *context != NULL ) )
    {
        /* If this (below) assert fails, it is either a double-free (likely) or
           a bad/dangling pointer (unlikely) */
        Q6ZIP_ASSERT( (q6zip_context_t *)( (*context)->magic ) == *context );
        (*context)->magic += 1;

        q_put( &q6zip_context_module.pool.free, ( q_link_type * )*context );
        *context = NULL;
    }
}
