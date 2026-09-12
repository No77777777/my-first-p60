#pragma once
/**
   @file q6zip_request_private.h 
   @author anandj 
   @brief Private/internal use header for operations on q6zip_request_t
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#include "q6zip_context.h"
#include "q6zip_request.h"

static inline q6zip_context_t * q6zip_request_private_get_context (q6zip_request_t * request)
{
    return ( q6zip_context_t * ) ( (qurt_addr_t)request - Q6ZIP_OFFSETOF( q6zip_context_t, request ) );
}
