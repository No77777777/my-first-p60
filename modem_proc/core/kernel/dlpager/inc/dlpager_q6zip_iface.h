/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who          what, where, why
--------   -------      ----------------------------------------------------------
12/09/14   rc,rr,cp     Header file for interfacing with q6zip module
===========================================================================*/

#include <dlpager_types.h>

void dlpager_q6zip_schedule_decompression(unsigned int fault_page_va, unsigned int thread_id);

void dlpager_q6zip_schedule_compression(unsigned int src_page_va, dlpager_iovec_t rw_iovec, dlpager_priority_t prio);

int dlpager_q6zip_init(void);

/**
 * @brief Check to see if there are any outstanding Q6Zip operations
 * 
 * @return boolean TRUE if outstanding Q6Zip operations, FALSE otherwise
 */
extern boolean dlpager_q6zip_has_outstanding_operations (void);
