/*==========================================================================
 * FILE:         dlpager_rwbuffer.c
 *
 * SERVICES:     DL PAGER READ-WRITE BUFFERS
 *
 * DESCRIPTION:  This file implements functionality to manage read-write buffers used to store compressed RW data 
 *
 * Copyright (c) 2014 Qualcomm Technologies Incorporated.
 * All Rights Reserved. QUALCOMM Proprietary and Confidential.
=============================================================================*/


/*===========================================================================

            EDIT HISTORY FOR MODULE


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/09/14   cp      First version
===========================================================================*/
#include "dlpager_rwbuffer.h"
#include <assert.h>
#include <qurt.h>
#include <dlpager_params.h>

#define NUM_SOFT_RWBUFFERS 2
#define NUM_HARD_RWBUFFERS 4

typedef struct _dlpager_rwbuffer_node_t{
  void                            *rwbuffer;      // pointer to the raw rwbuffer memory
  struct _dlpager_rwbuffer_node_t *next;          // pointer to next node in the free list
}dlpager_rwbuffer_node_t;


typedef struct{
  dlpager_rwbuffer_node_t *head;
  dlpager_rwbuffer_node_t *nodes;
  qurt_mutex_t             mutex;
  char                    *pool;
  int                      num_rwbuffers;
}dlpager_rwbuffer_list_t;

/* @warning 128B alignment is intentional. Q6Zip IPA from AT.1.0 onwards
   requires that destination buffers be aligned to 128B (for DMA and reduction
   of MIPS in IPA uC to handle misalignments). The worstcase size of 4352B for
   each RW buffer automatically guarantees 128B alignment for each */
static char dlpager_rwbuffer_soft_pool[NUM_SOFT_RWBUFFERS][RWBUFFER_SIZE] __attribute__(( aligned( 128 ) ));// statically allocated memory for soft rw buffers
static dlpager_rwbuffer_node_t dlpager_rwbuffer_soft_nodes[NUM_SOFT_RWBUFFERS];        // statically allocated memory for soft free list

static char dlpager_rwbuffer_hard_pool[NUM_HARD_RWBUFFERS][RWBUFFER_SIZE] __attribute__(( aligned( 128 ) ));// statically allocated memory for hard rw buffers
static dlpager_rwbuffer_node_t dlpager_rwbuffer_hard_nodes[NUM_HARD_RWBUFFERS];        // statically allocated memory for hard free list

static qurt_sem_t   dlpager_rwbuffer_hard_sem;

static dlpager_rwbuffer_list_t dlpager_rwbuffer_soft_list;
static dlpager_rwbuffer_list_t dlpager_hard_rwbuffer_list;


static inline
void init_list(dlpager_rwbuffer_list_t *which_list)
{
  int i;
  for(i = 0; i < which_list->num_rwbuffers - 1; ++i){
    (which_list->nodes)[i].rwbuffer = (void *) (which_list->pool + i * RWBUFFER_SIZE);
    (which_list->nodes)[i].next = &((which_list->nodes)[i+1]);
  }
  which_list->nodes[which_list->num_rwbuffers - 1].rwbuffer = (void *) (which_list->pool + (which_list->num_rwbuffers - 1) * RWBUFFER_SIZE);
  which_list->nodes[which_list->num_rwbuffers - 1].next = (void *) 0;
}


void dlpager_rwbuffer_init(void)
{
  dlpager_rwbuffer_soft_list.head  = dlpager_rwbuffer_soft_nodes;
  dlpager_rwbuffer_soft_list.nodes = dlpager_rwbuffer_soft_nodes;
  dlpager_rwbuffer_soft_list.pool  = (char *) dlpager_rwbuffer_soft_pool;
  dlpager_rwbuffer_soft_list.num_rwbuffers = NUM_SOFT_RWBUFFERS;
  qurt_mutex_init(&dlpager_rwbuffer_soft_list.mutex);

  dlpager_hard_rwbuffer_list.head  = dlpager_rwbuffer_hard_nodes;
  dlpager_hard_rwbuffer_list.nodes = dlpager_rwbuffer_hard_nodes;
  dlpager_hard_rwbuffer_list.pool  = (char *) dlpager_rwbuffer_hard_pool;
  dlpager_hard_rwbuffer_list.num_rwbuffers = NUM_HARD_RWBUFFERS;
  qurt_mutex_init(&dlpager_hard_rwbuffer_list.mutex);
  qurt_sem_init_val(&dlpager_rwbuffer_hard_sem, NUM_HARD_RWBUFFERS);

  init_list(&dlpager_rwbuffer_soft_list);
  init_list(&dlpager_hard_rwbuffer_list);
}



void *dlpager_rwbuffer_alloc(dlpager_priority_t priority)
{
  dlpager_rwbuffer_list_t *which_list;
  void *buf = NULL;

  if(priority == LOW_PRIORITY){
    which_list = &dlpager_rwbuffer_soft_list;
  }else{
    which_list = &dlpager_hard_rwbuffer_list;
    // may block
    qurt_sem_down(&dlpager_rwbuffer_hard_sem);
  }

  qurt_mutex_lock(&which_list->mutex);
  // HIGH guarantees a buf
  // LOW gets a buf if one is available
  if(priority == HIGH_PRIORITY || which_list->head != NULL){
    buf = which_list->head->rwbuffer;
    which_list->head = which_list->head->next;
  }
  qurt_mutex_unlock(&which_list->mutex);


  return buf;
}




void dlpager_rwbuffer_free(void *rwbuffer, dlpager_priority_t priority)
{
  dlpager_rwbuffer_list_t *which_list;
  int index;

  if(priority == LOW_PRIORITY){
    which_list = &dlpager_rwbuffer_soft_list;
  }else{
    which_list = &dlpager_hard_rwbuffer_list;
  }

  index = ((unsigned int) rwbuffer - (unsigned int) which_list->pool) / RWBUFFER_SIZE;
  ASSERT(((unsigned int) rwbuffer - (unsigned int) which_list->pool) % RWBUFFER_SIZE == 0);

  qurt_mutex_lock(&which_list->mutex);
  which_list->nodes[index].next = which_list->head;
  which_list->head = &which_list->nodes[index];
  qurt_mutex_unlock(&which_list->mutex);


  if(priority == HIGH_PRIORITY){
    qurt_sem_up(&dlpager_rwbuffer_hard_sem); // indicate hard rwbuffer list has a free node
  }
}
