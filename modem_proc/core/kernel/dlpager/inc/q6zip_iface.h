#pragma once
/**
   @file q6zip_iface.h 
   @author anandj 
   @brief Q6ZIP interface 
    
    Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
    Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "q6zip_context.h"
#include "q6zip_request.h"
#include "q6zip_types.h"

/**
   @brief Type for callback from Q6ZIP indicating that it has initialized 
   @param [in] initialized TRUE on success, FALSE on failure 
 */
typedef void (*q6zip_initialized_cb_t)(boolean initialized);

/**
   @brief Type for callback from Q6ZIP indicating a zip/unzip operation has 
   completed 
   @param [in] request Request. It is *OWNED* by the callee! Once done with it, 
          callee *MUST* call q6zip_request_free( &request ) to dispose it
   @param [in] response Response associated with the request. 
 */
typedef void (*q6zip_completed_cb_t)(q6zip_request_t * request, q6zip_response_t const * response);

/**
   @brief Interface to initialize Q6ZIP 
   @param [in] ro_dict Pointer to start of dictionary for unzip using RO 
          algorithm
   @param [in] ro_dict_len Length (bytes) of dictionary 
   @param [in] initialized Callback for completion of Q6ZIP initialization
   @param [in] completed Callback for zip/unzip completion 
 */
typedef void (*q6zip_init_fn_t)(
    void *                 ro_dict,
    unsigned int           ro_dict_len,
    q6zip_initialized_cb_t initialized,
    q6zip_completed_cb_t   completed
);

/**
   @brief Interface to get a Q6ZIP request 
   @warning May block if underlying implementation doesnt have requests to hand 
            out
   @return q6zip_request_t * 
 */
typedef q6zip_request_t * (*q6zip_get_request_fn_t)(void);

/**
   @brief Interface to submit a request to Q6ZIP 
   @param [in] request Q6ZIP request. On success, it is owned by callee
   @return q6zip_error_t 
 */
typedef q6zip_error_t (*q6zip_submit_fn_t)(q6zip_request_t ** request);

/**
   @brief Interface to submit a pair of requests to Q6ZIP 
   @param [in] pair Q6ZIP request pair. On success, the requests are owned by 
          callee
   @return q6zip_error_t 
 */
typedef q6zip_error_t (*q6zip_submit_pair_fn_t)(q6zip_request_pair_t * pair);

/**
   @brief Interface to cleanup Q6ZIP 
 */
typedef void (*q6zip_fini_fn_t)(void);

/** @brief Q6ZIP interface for DL pager. Q6ZIP implementations *MUST* implement
           this interface */
typedef struct
{
    q6zip_init_fn_t        init;
    q6zip_get_request_fn_t get_request;
    q6zip_submit_fn_t      submit;
    q6zip_submit_pair_fn_t submit_pair;
    q6zip_fini_fn_t        fini;

} q6zip_iface_t;

