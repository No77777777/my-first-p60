

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   DL PAGER TOP LEVEL HEADER FILE

   GENERAL DESCRIPTION

   Copyright (c) 2016 Qualcomm Technologies Incorporated.
   All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      Header file for APIs related to collecting statistics
===========================================================================*/
#pragma once
#include <qurt.h>
#include <dlpager_types.h>

#define DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING 0 // Enable/disable timing from thread fault to resume
#define DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING   0 // Enable/disable timing from fault to ipa and ipa to resume
#define DLPAGER_STATS_ENABLE_LRU_HIST           0 // Enable/disable lru histogram

struct dlpager_stats_s
{
  unsigned int rox_decompressions;
  unsigned int rw_compressions;
  unsigned int rw_decompressions;
  unsigned int bss_allocations;
  unsigned int soft_cleans;
  unsigned int soft_cleans_failed;
  unsigned int hard_cleans;
  unsigned int transitions[MAX_STATES][MAX_STATES];
  unsigned int page_counts[MAX_STATES];
  unsigned char *recompress_count;
  unsigned int lru_pages_fe;
  unsigned int lru_pages_be;
};

//Always used and not wrapped with any #if.
extern struct dlpager_stats_s dlpager_stats;

#if DLPAGER_STATS_ENABLE_FAULTRESUME_TIMING
void dlpager_stats_faultresumetimer_start(qurt_thread_t);
void dlpager_stats_faultresumetimer_stop(qurt_thread_t);
#endif

#if DLPAGER_STATS_ENABLE_LRU_HIST
void dlpager_stats_lrusteps_store(int);
#endif

#if DLPAGER_STATS_ENABLE_TOFROMIPA_TIMING
void dlpager_stats_fault_to_ipa_start(qurt_thread_t task_id);
void dlpager_stats_fault_to_ipa_stop(qurt_thread_t task_id);
void dlpager_stats_ipa_to_resume_start(qurt_thread_t task_id);
void dlpager_stats_ipa_to_resume_stop(qurt_thread_t task_id);
#endif
