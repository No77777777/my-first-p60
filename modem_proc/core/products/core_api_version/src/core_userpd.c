/*======================================================================
                        core_userpd.c 

GENERAL DESCRIPTION
 This file is to provide profiling measurement and other experiments on User PD
EXTERNALIZED FUNCTIONS
  none

INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2016  by Qualcomm Technologies, Inc.  All Rights Reserved.
 ======================================================================*/
/*======================================================================
 *
 *                       EDIT HISTORY FOR FILE
 *
 *   This section contains comments describing changes made to the
 *   module. Notice that changes are listed in reverse chronological
 *   order.
 *
 *  
 *
 *
 * when         who     what, where, why
 * ----------   ---     ------------------------------------------------
 * 10/13/2016   tkuo    created. 
 ======================================================================*/

#include "qurt_atomic_ops.h"
#include "qurt.h"
#include "qurt_printf.h"
extern void core_api_version_query_func(char*);

void coreapi_report_init(void)
{

char *core_api_str_ptr = NULL;
#ifdef USERPD_PROFILE_DEBUG
    unsigned long curr_time_in_micro_sec = 0;
	curr_time_in_micro_sec = QURT_TIMER_TIMETICK_TO_US(qurt_timer_get_ticks()); 
	qurt_printf("TIME Ends from USER PD:%lu\n",curr_time_in_micro_sec);
#endif /*USERPD_PROFILE_DEBUG */
	core_api_version_query_func(core_api_str_ptr);
return;
}
