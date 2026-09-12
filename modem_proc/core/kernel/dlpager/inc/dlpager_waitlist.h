
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               DL PAGER TOP LEVEL HEADER FILE

GENERAL DESCRIPTION

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      Header file for APIs related to waitlists 
===========================================================================*/

#ifndef  DLPAGER_WAITLIST_H
#define  DLPAGER_WAITLIST_H

#include <qurt.h>
#include <dlpager_types.h>
#include <q6zip_params.h>


typedef struct{
  unsigned int task_id;
  unsigned int tasklog_idx;
}dlpager_task_idx_pair_t;


int dlpager_waitlist_init(void);

void dlpager_waitlist_add_task(unsigned int page_va, dlpager_event_t event, qurt_thread_t task_id);
unsigned int dlpager_waitlist_try_add_task(unsigned int page_va, dlpager_event_t event, unsigned int task_id);
unsigned int dlpager_waitlist_get_task(unsigned int page_va, dlpager_event_t event, dlpager_task_idx_pair_t *task_idx_pair);

void dlpager_waitlist_add_rwbuffer(unsigned int page_va, dlpager_iovec_t rwbuffer);
dlpager_iovec_t dlpager_waitlist_get_rwbuffer(unsigned int page_va);
void dlpager_waitlist_set_rwbuffer_len(unsigned int page_va, unsigned int len);



#endif
