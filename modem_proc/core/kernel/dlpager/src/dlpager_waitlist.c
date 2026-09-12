/*==========================================================================
 * FILE:         dlpager_waitlist.c
 *
 * SERVICES:     DL PAGER WAITLIST
 *
 * DESCRIPTION:  This file provides functioanlity to manage waitlists.
 *               There are two waitlists, one for pending rwbuffers and another for pending tasks.
 *               The waitlists share a common node structure, though it's not a requirement.
 *               Multiple tasks may pend on a page; only one rwbuffer may pend on a page.
 *               The same page may exist simultaenously in both waitlists. It's treated independently in each case.
 *               Two freelists are needed to manage static memory: one for the generic nodes in either waitlist
 *               and another for the pending tasks (which may be linked) in the task waitlist
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.

EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      First version
===========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qurt.h>
#include <dlpager.h>
#include <dlpager_waitlist.h>
#include <dlpager_params.h>
//#include <lockless_queue.h>
//#include <queue.h>
#include <dlpager_queue.h>



typedef struct{
  dlpager_queue_link_type link;
  qurt_thread_t  task_id;
  //unsigned int   tasklog_idx; 
}dlpager_waitlist_task_t;

typedef dlpager_iovec_t dlpager_waitlist_rwbuf_t;

typedef struct{
  dlpager_queue_link_type  link;
  unsigned int page_va;
  dlpager_event_t event; 
  union{
    dlpager_queue_type                    tasks;
    dlpager_waitlist_rwbuf_t  rwbuf;
  }payload_u;
}dlpager_waitlist_node_t;

typedef struct{
  unsigned int    page_va;
  dlpager_event_t event;
}page_va_and_event_t;


// General mutex for waitlist operations.
qurt_mutex_t dlpager_waitlist_mutex;

// Waitlists
struct{
  dlpager_queue_type tasks;  // Holds dlpager_waitlist_node_t which has payload_u of tasks
  dlpager_queue_type rwbufs; // Holds dlpager_waitlist_node_t which has payload_u of rwbuf
}dlpager_waitlist;

// Freelists
struct{
  dlpager_queue_type nodes; // Holds dlpager_waitlist_node_t
  dlpager_queue_type tasks; // Holds dlpager_waitlist_task_t
}dlpager_freelist;


// Statically allocated memory for nodes and tasks
dlpager_waitlist_node_t dlpager_waitlist_node_pool[MAX_PENDING_PAGES];
dlpager_waitlist_task_t dlpager_waitlist_task_pool[MAX_PENDING_TASKS];



/**
 * dlpager_queue_compare_page_va_and_event
 *   Compare function for dlpager_queue_linear_search()
 *   Returns 1 if both page_va and event match those at a given node; 0 otherwise.
 */
int dlpager_queue_compare_page_va_and_event(void *compare_node, void *compare_page_va_and_event)
{
  dlpager_waitlist_node_t *node = (dlpager_waitlist_node_t *) compare_node;
  page_va_and_event_t *page_va_and_event = (page_va_and_event_t *) compare_page_va_and_event;
  if(node->page_va == page_va_and_event->page_va /*&& node->event == page_va_and_event->event*/){
    return 1;
  }
  return 0;
}


/**
 * dlpager_waitlist_init
 *   Must be called before dlpager_waitlist can be used.
 */ 
int dlpager_waitlist_init(void)
{        
  int i;
  qurt_mutex_init(&dlpager_waitlist_mutex);
  dlpager_queue_init(&dlpager_waitlist.tasks);
  dlpager_queue_init(&dlpager_waitlist.rwbufs);
  dlpager_queue_init(&dlpager_freelist.nodes);
  dlpager_queue_init(&dlpager_freelist.tasks);
  memset(dlpager_waitlist_node_pool, 0, sizeof(dlpager_waitlist_node_t) * MAX_PENDING_PAGES);
  memset(dlpager_waitlist_task_pool, 0, sizeof(dlpager_waitlist_task_t) * MAX_PENDING_TASKS);
  for(i = 0; i < MAX_PENDING_PAGES; ++i){
    dlpager_queue_put(&dlpager_freelist.nodes, &dlpager_waitlist_node_pool[i].link);
  }
  for(i = 0; i < MAX_PENDING_TASKS; ++i){
    dlpager_queue_put(&dlpager_freelist.tasks, &dlpager_waitlist_task_pool[i].link);
  }
  return 0;
}


/**
 * dlpager_waitlist_add_task
 *   Look for the page_va-event pair in the list. 
 *   If it already exists, append the task to that node.
 *   Otherwise, create a new node for this page_va-event, and add the pending task to it.
 */ 
void dlpager_waitlist_add_task( unsigned int page_va, dlpager_event_t event, qurt_thread_t task_id )
{
  dlpager_waitlist_node_t *node;
  page_va_and_event_t page_va_and_event = { .page_va = page_va, .event = event };

  qurt_mutex_lock(&dlpager_waitlist_mutex);

  node = (dlpager_waitlist_node_t *) dlpager_queue_linear_search(&dlpager_waitlist.tasks, dlpager_queue_compare_page_va_and_event, &page_va_and_event);
  if(!node){
    node = (dlpager_waitlist_node_t *) dlpager_queue_get(&dlpager_freelist.nodes);
    ASSERT(node != NULL);
    node->page_va = page_va;
    node->event = event;
    dlpager_queue_init(&node->payload_u.tasks);
    dlpager_queue_put(&dlpager_waitlist.tasks, &node->link);
  }  
  //grab task from its free list and append
  dlpager_waitlist_task_t *task = (dlpager_waitlist_task_t *) dlpager_queue_get(&dlpager_freelist.tasks);
  ASSERT(task != NULL);
  task->task_id = task_id;
  //task->tasklog_idx = tasklog_idx;

  //append task to list
  dlpager_queue_put(&node->payload_u.tasks, &task->link);

  qurt_mutex_unlock(&dlpager_waitlist_mutex);
}



/**
 * dlpager_waitlist_try_add_task
 *   Attempt to append to the waitlist.
 *   Return 0 if the node with the page_va and event does not exist.
 *   Append and return 1 if it does.
 */
unsigned int dlpager_waitlist_try_add_task(unsigned int page_va, dlpager_event_t event, unsigned int task_id)
{
  dlpager_waitlist_node_t *node;
  page_va_and_event_t page_va_and_event = { .page_va = page_va, .event = event };
  int was_appended = 0;

  qurt_mutex_lock(&dlpager_waitlist_mutex);

  node = (dlpager_waitlist_node_t *) dlpager_queue_linear_search(&dlpager_waitlist.tasks, dlpager_queue_compare_page_va_and_event, &page_va_and_event);
  if(node){
    dlpager_waitlist_task_t *task = (dlpager_waitlist_task_t *) dlpager_queue_get(&dlpager_freelist.tasks);
    ASSERT(task != NULL);
    task->task_id = task_id;
    //task->tasklog_idx = -1;
    dlpager_queue_put(&node->payload_u.tasks, &task->link);
    was_appended = 1;
  }

  qurt_mutex_unlock(&dlpager_waitlist_mutex);

  return was_appended;
}



/**
 * dlpager_waitlist_get_task
 *   Gets a pending task for the given page_va-event pair.
 *   If the page_va-event pair does not exist, return a pair of {-1, -1}
 *
 *   out (param): task_idx_pair -- a task_id-tasklog_idx pair for the provided
 *                page_va-event pair. If the page_va-event pair does not exist,
 *                returns a pair of {-1, -1}.
 *   out: num_pending_tasks -- number of tasks still pending for this page_va-event pair.
 */
unsigned int dlpager_waitlist_get_task(unsigned int page_va, dlpager_event_t event, dlpager_task_idx_pair_t *task_idx_pair)
{
  dlpager_waitlist_node_t *node = 0;
  int num_pending_tasks = 0;
  task_idx_pair->task_id = 0xFFFFFFFF;
  //task_idx_pair->tasklog_idx = 0xFFFFFFFF;

  qurt_mutex_lock(&dlpager_waitlist_mutex);

  page_va_and_event_t page_va_and_event = { .page_va = page_va, .event = event };
  node = dlpager_queue_linear_search(&dlpager_waitlist.tasks, dlpager_queue_compare_page_va_and_event, &page_va_and_event);
  if(node){
    // remove a task (FIFO ordering)
    dlpager_waitlist_task_t *task = (dlpager_waitlist_task_t *) dlpager_queue_get(&node->payload_u.tasks);
    ASSERT(task != NULL);
    task_idx_pair->task_id = task->task_id;
    //task_idx_pair->tasklog_idx = task->tasklog_idx;
    dlpager_queue_put(&dlpager_freelist.tasks, &task->link);

    // if the task queue is now empty, then free the node
    num_pending_tasks = dlpager_queue_cnt(&node->payload_u.tasks);
    if(num_pending_tasks == 0){
      dlpager_queue_delete(&dlpager_waitlist.tasks, &node->link);
      dlpager_queue_put(&dlpager_freelist.nodes, &node->link);
    }
  }
  qurt_mutex_unlock(&dlpager_waitlist_mutex);

  return num_pending_tasks;
}


/**
 * dlpager_waitlist_allocate_rwbuffer
 *   Append a new node to the waitlist for rwbuffers.
 *   This associates the given rwbuf to the given page_va.
 */
void dlpager_waitlist_add_rwbuffer(unsigned int page_va, dlpager_waitlist_rwbuf_t rwbuffer)
{
  qurt_mutex_lock(&dlpager_waitlist_mutex);

  // new node
  dlpager_waitlist_node_t *node = (dlpager_waitlist_node_t *) dlpager_queue_get(&dlpager_freelist.nodes);
  ASSERT(node != NULL);
  node->page_va = page_va;
  node->event = 0; // FIXME: hard-code an event for decompression complete here? .. it's the same for all rwbufs
  node->payload_u.rwbuf = rwbuffer;

  // add node to rwbuf waitlist
  dlpager_queue_put(&dlpager_waitlist.rwbufs, &node->link);

  qurt_mutex_unlock(&dlpager_waitlist_mutex);
}


/**
 * dlpager_waitlist_get_rwbuffer
 *   Find the rwbuffer associated with the given page_va, and remove it from its waitlist.
 *   Return the rwbuf to the user.
 */
dlpager_waitlist_rwbuf_t dlpager_waitlist_get_rwbuffer(unsigned int page_va)
{
  dlpager_waitlist_rwbuf_t rwbuf;
  dlpager_waitlist_node_t *node;

  qurt_mutex_lock(&dlpager_waitlist_mutex);

  // FIXME: hard-code an event for decompression complete here? .. it's the same for all rwbufs
  page_va_and_event_t page_va_and_event = { .page_va = page_va, .event = 0 };
  node = (dlpager_waitlist_node_t *) dlpager_queue_linear_search(&dlpager_waitlist.rwbufs, dlpager_queue_compare_page_va_and_event, &page_va_and_event);
  ASSERT(node);
  rwbuf = node->payload_u.rwbuf;

  dlpager_queue_delete(&dlpager_waitlist.rwbufs, &node->link);
  dlpager_queue_put(&dlpager_freelist.nodes, &node->link);

  qurt_mutex_unlock(&dlpager_waitlist_mutex);

  return rwbuf;
}



/**
 * dlpager_waitlist_set_rwbuffer_len
 *   Set the length of the rwbuffer associated with the given page_va.
 */
void dlpager_waitlist_set_rwbuffer_len(unsigned int page_va, unsigned int len)
{
  dlpager_waitlist_node_t *node;

  qurt_mutex_lock(&dlpager_waitlist_mutex);
  // FIXME: hard-code an event for decompression complete here? .. it's the same for all rwbufs
  page_va_and_event_t page_va_and_event = { .page_va = page_va, .event = 0 };
  node = (dlpager_waitlist_node_t *) dlpager_queue_linear_search(&dlpager_waitlist.rwbufs, dlpager_queue_compare_page_va_and_event, &page_va_and_event);
  ASSERT(node);
  node->payload_u.rwbuf.len = len;
  qurt_mutex_unlock(&dlpager_waitlist_mutex);
}
