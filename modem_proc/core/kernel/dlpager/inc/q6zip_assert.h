#pragma once
/**
   @file q6zip_assert.h 
   @author anandj 
   @brief Assert support for Q6ZIP modules
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#if defined( Q6ZIP_ENABLE_ASSERTS )
#include "assert.h"
#define Q6ZIP_ASSERT( cond ) ASSERT( cond )
#else
#define Q6ZIP_ASSERT( cond )
#endif
