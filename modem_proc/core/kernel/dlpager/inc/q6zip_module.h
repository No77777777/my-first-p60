#pragma once
/**
   @file q6zip_module.h 
   @author anandj 
   @brief Main header for Q6ZIP module/feature
    
   Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/**
 * @brief Initialize Q6ZIP 
 * @warning *MUST* be invoked before using Q6ZIP 
 */
extern void q6zip_module_init (void);

/**
 * @brief Cleanup Q6ZIP
 */
extern void q6zip_module_fini (void);
