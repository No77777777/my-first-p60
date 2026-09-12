/*============================================================================
  @file wlan_core_init.h

  Copyright (c) 2016 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

============================================================================*/
#ifndef _WLAN_CORE_INIT_
#define _WLAN_CORE_INIT_

typedef enum cnss_core_result_enum
{
   CNSS_CORE_FAILURE = -1,
   CNSS_CORE_SUCCESS,
}cnss_core_eResult;

/* =========================================================================
**  Function : cnss_core_init
** =========================================================================*/
/**
  This function invoke initialization for ABT Timeout Driver

  @dependencies
  Needs to make sure following is done before calling this function:
    1. WCAON enabled 
    2. Power rails ON
    3. GDSCR enabled
    4. GCC WCSS clocks to be ON
*/
cnss_core_eResult cnss_core_init(void);

/* =========================================================================
**  Function : cnss_core_deinit
** =========================================================================*/
/**
  This function performs de-Initialization sequence for cnss blocks.
  
  @dependencies
  Needs to make sure following is done before calling this function:
    1. WCAON enabled 
    2. Power rails ON
    3. GDSCR enabled
    4. GCC WCSS clocks to be ON
*/
cnss_core_eResult cnss_core_deinit(void);

#endif /* _WLAN_CORE_INIT_ */
