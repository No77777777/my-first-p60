#pragma once
/**
   @file q6zip_request.h 
   @author anandj 
   @brief Q6ZIP request abstract datatype and the functions that operate on it
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "q6zip_context_opaque.h"
#include "q6zip_types.h"
#include "qurt_types.h"

/** @brief Q6ZIP request */
typedef struct
{
    /** @brief Algorithm to use */
    q6zip_algo_t         algo;

    /** @brief Hint on the algorithm */
    q6zip_algo_hint_t    hint;

    /** @brief Priority associated with this request. Only honored if the
        underlying implementation supports priorities. You may assign fixed
        priority constants as well - @see q6zip_fixed_priority_t */
    q6zip_priority_t     priority;

    /** @brief Source iovec */
    q6zip_iovec_t        src;

    /** @brief Destination iovec */
    q6zip_iovec_t        dst;

    /** @brief User specific context */
    q6zip_user_context_t user;

} q6zip_request_t;

/** @brief Abstract datatype for a pair of requests with strict ordering
    constraint. The first request *SHALL* be serviced before the second */
typedef struct
{
    q6zip_request_t * first;
    q6zip_request_t * second;

} q6zip_request_pair_t;

/**
 * @brief Allocate a Q6ZIP request
 * @return q6zip_request_t* 
 */
extern q6zip_request_t * q6zip_request_alloc (void);

/**
 * @brief Get the Q6ZIP context associated with this request
 * @param [in] request Q6ZIP request
 * 
 * @return q6zip_context_t const* 
 * @warning const-ness is intentional, *DONOT* attempt to cast it away
 */
extern q6zip_context_t const * q6zip_request_get_context (q6zip_request_t * request);

/**
   @brief De-allocate a Q6ZIP request 
   @param [in] request Double pointer to a request to be freed
   @warning request *MUST* have been allocated via q6zip_request_alloc()! 
 */
extern void q6zip_request_free (q6zip_request_t ** request);
