/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        Q U E U E    S E R V I C E S

GENERAL DESCRIPTION

  A queue is a simple data structure used for logically storing and re-
  trieving data blocks, in a first in - first out (FIFO) order without
  physically copying them. Software tasks and interrupt handlers can use
  queues for cleanly transferring data blocks to one another.

                        +-------+     +-------+     +-------+
                        | DATA  |     | DATA  |     | DATA  |
                        | BLOCK |     | BLOCK |     | BLOCK |
          +-------+     +-------+     +-------+     +-------+
          | QUEUE |---->| LINK  |---->| LINK  |---->| LINK  |---- |
   |----->+-------+     +-------+     +-------+     +-------+     |
   |      |       |     |       |     |       |     |       |     |
   |      +-------+     |       |     |       |     |       |     |
   |                    +-------+     +-------+     +-------+     |
   |                                                              |
   ---------------------------------------------------------------|

  The Queue Services provide a small set of declarations and functions for
  defining and initializing a queue, defining and initializing a links with-
  in a data block, placing a data block at the tail of a queue, removing a
  data block from the head of a queue, and removing a data block from any
  position in a queue.

  Aside from requiring each data block to contain a link, the Queue Services
  impose no restrictions on the size of structure of data blocks used with
  with queues. This allows software to pass virtually any type of data on
  queues. Notice that a data block may contain multiple links allowing it to
  be placed simultaneously on multiple queues.


EXTERNALIZED FUNCTIONS

  uq_init
    This function initializes a queue. It should be called on behalf of a
    queue prior to using the queue.

  uq_link
    This function initializes a link field. It should be called on behalf
    of a link field prior to using the link filed with the other Queue
    Services.

  uq_get
    This function removes the data block at head of a queue and returns a
    pointer to the data block. If the queue is empty, then a NULL pointer
    is returned.

  uq_put
    This function places a data block at the tail of a queue.

  uq_cnt
    This function returns the number of items currently on a queue.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Prior to use, a queue must be initialized by calling uq_init. Similarly,
  a link must be initialized prior to use by calling uq_link.

Copyright (c) 2015 by Qualcomm Technologies Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                              Edit History

$PVCSPath: O:/src/asw/COMMON/vcs/queue.c_v   1.4   02 Oct 2001 10:46:56   rajeevg  $
$Header: //components/rel/core.mpss/10.0/services/utils/src/uqueue.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/05/15   ps      Add additional APIs required in micro image CR 865036
04/21/15   ps      Added queue for micro image

===========================================================================*/

/* ------------------------------------------------------------------------
** Includes
** ------------------------------------------------------------------------ */

#include "core_variation.h"
#include "uqueue.h"
#ifndef FEATURE_WINCE
#include "amssassert.h"
#endif

#ifdef CUST_H
#endif

/* NOTUSED */
#define NOTUSED(i) if(i){}


#ifdef FEATURE_QUBE
#include "qube.h"
#include "err.h"
#endif /* FEATURE_QUBE */


/* ==========================================================================
FUNCTION Q_LOCK_DELETE
DESCRIPTION
   Deletes the mutex for the given queue
DEPENDENCIES
   None.
RETURN VALUE
   None.
SIDE EFFECTS
   None.
============================================================================= */
static void uq_lock_delete( uq_type *q_ptr )
{
   qurt_rmutex_destroy(&(q_ptr->mutex));
   return;
}  /* END uq_lock_delete */


/* ==========================================================================
FUNCTION Q_LOCK
DESCRIPTION
   Locks the mutex for the given queue
DEPENDENCIES
   None.
RETURN VALUE
   None.
SIDE EFFECTS
   None.
============================================================================= */
static void uq_lock( uq_type *q_ptr )
{
  #ifdef FEATURE_QUBE
   ASSERT( q_ptr != NULL );
   qurt_rmutex_lock(&q_ptr->mutex);
  #endif //USE_MUTEX
  return;
} /* END uq_lock */

/* ==========================================================================
FUNCTION Q_UNLOCK
DESCRIPTION
   Frees the mutex for the given queue
DEPENDENCIES
   None.
RETURN VALUE
   None.
SIDE EFFECTS
   None.
============================================================================= */
static void uq_unlock( uq_type *q_ptr )
{
  #ifdef FEATURE_QUBE
   ASSERT( q_ptr != NULL );
   qurt_rmutex_unlock(&q_ptr->mutex);
  #endif //USE_MUTEX
  return;
} /* END q_unlock */


/*==========================================================================
FUNCTION UQ_DESTROY

DESCRIPTION
  This function destroys a specified queue. It should be called if you
  do not require this queue anymore.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Elements in the queue will  not be accessible through this queue 
  anymore. It is user's responsibility to deallocate the memory allocated 
  for the queue and its elements to avoid leaks
===========================================================================*/
void uq_destroy(
   uq_type  *q_ptr  /* Ptr to queue to be destroyed. */
)
{
   ASSERT( q_ptr != NULL );

   uq_lock( q_ptr );
   q_ptr->link.next_ptr = NULL;
   q_ptr->link.prev_ptr = NULL;

   #ifndef FEATURE_Q_NO_SELF_QPTR
      q_ptr->link.self_ptr = NULL;
      q_ptr->link.q_ptr    = NULL;
   #endif

   q_ptr->cnt = 0;
   uq_unlock( q_ptr );

   uq_lock_delete( q_ptr );

   return;
} /* END q_destroy */


/*===========================================================================
FUNCTION UQ_LINK

DESCRIPTION
  This function initializes a specified link. It should be called for each
  link prior to using the link with the other Queue Services.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the initialized link.

SIDE EFFECTS
  The specified link is initialized for use with the Queue Services.
===========================================================================*/
uq_link_type* uq_link(
   void         *item_ptr,  /* Ptr to item or variable containing link. */
   uq_link_type  *link_ptr   /* Ptr to link field within variable. */
)
{
   ASSERT( link_ptr != NULL );
   link_ptr->next_ptr = NULL;

   #ifndef FEATURE_Q_SINGLE_LINK
      link_ptr->prev_ptr = NULL;
   #endif

   #ifndef FEATURE_Q_NO_SELF_QPTR
      link_ptr->self_ptr = item_ptr;
      link_ptr->q_ptr    = NULL;
   #endif

   NOTUSED( item_ptr ); //below macro is empty... remove this line when it's not
   Q_XCEPT_Q_LINK( item_ptr, link_ptr );

   return link_ptr;
} /* END uq_link */

#ifndef MSM5000_IRAM_FWD
/*===========================================================================
FUNCTION UQ_PUT

DESCRIPTION
  This function enqueues an item onto a specified queue using a specified
  link.

DEPENDENCIES
  The specified queue should have been previously initialized via a call
  to q_init. The specified link field of the item should have been prev-
  iously initialized via a call to q_init_link.

RETURN VALUE
  None.

SIDE EFFECTS
  The specified item is placed at the tail of the specified queue.
===========================================================================*/
void uq_put(
   uq_type       *q_ptr,    /* Ptr to queue. */
   uq_link_type  *link_ptr  /* Ptr to item link to use for queueing. */
)
{
   uq_lock( q_ptr );

   ASSERT( q_ptr != NULL );
   ASSERT( link_ptr != NULL );
   link_ptr->next_ptr = (uq_link_type *)&q_ptr->link;

   #ifndef FEATURE_Q_NO_SELF_QPTR
      link_ptr->q_ptr    = q_ptr;
   #endif

   #ifndef FEATURE_Q_SINGLE_LINK
      link_ptr->prev_ptr = q_ptr->link.prev_ptr;

   #endif

   if(q_ptr->link.prev_ptr != NULL)
   {
    q_ptr->link.prev_ptr->next_ptr = link_ptr;
   }
   q_ptr->link.prev_ptr           = link_ptr;
   q_ptr->cnt++;

   Q_XCEPT_Q_PUT( q_ptr, link_ptr );

   uq_unlock( q_ptr );
   return;
} /* END q_put */
#endif /*MSM5000_IRAM_FWD*/

#ifndef MSM5000_IRAM_FWD

/*===========================================================================
FUNCTION UQ_GET

DESCRIPTION
  This function removes an item from the head of a specified queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  A pointer to the dequeued item. If the specified queue is empty, then
  NULL is returned.

SIDE EFFECTS
  The head item, if any, is removed from the specified queue.
===========================================================================*/
void* uq_get(
  uq_type  *q_ptr  /* Ptr to queue. */
)
{
   uq_link_type  *link_ptr;

   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_link_type  *ret_ptr = NULL;
   #endif

   ASSERT( q_ptr != NULL );
   uq_lock( q_ptr );

   /* Get ptr to 1st queue item.
   */
   link_ptr = q_ptr->link.next_ptr;

   /* Can only get an item if the queue is non empty
   */
   if( q_ptr->cnt > 0 )
   {
      q_ptr->link.next_ptr = link_ptr->next_ptr;

      #ifdef FEATURE_Q_SINGLE_LINK
         if (link_ptr->next_ptr == (uq_link_type *)q_ptr)
         {
            q_ptr->link.prev_ptr = (uq_link_type *)(&q_ptr->link);
         }
      #else
	 if(link_ptr->next_ptr != NULL)
	 {	
	    link_ptr->next_ptr->prev_ptr = &q_ptr->link;
	 }
      #endif

      q_ptr->cnt--;

      /* Mark item as no longer in a queue.
      */
      #ifdef FEATURE_Q_NO_SELF_QPTR
         link_ptr->next_ptr = NULL;
         ret_ptr = link_ptr;
      #else
         link_ptr->q_ptr = NULL;
      #endif
   }

   Q_XCEPT_Q_GET( q_ptr );

   uq_unlock( q_ptr );

   #ifdef FEATURE_Q_NO_SELF_QPTR
      return (void *)ret_ptr;
   #else
      return link_ptr->self_ptr;
   #endif
} /* END q_get */
#endif /*MSM5000_IRAM_FWD*/


/*===========================================================================
FUNCTION UQ_NEXT

DESCRIPTION
  This function returns a pointer to the next item on the queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to uq_init.

RETURN VALUE
  A pointer to the next item on the queue. If the end of the queue is reached,
  then NULL is returned.

SIDE EFFECTS
  None.
===========================================================================*/
void* uq_next(
   uq_type       *q_ptr,
   uq_link_type  *q_link_ptr
)
{
   void       *q_temp_ptr = NULL;
   ASSERT( q_link_ptr != NULL );
   uq_lock( q_ptr );
   Q_XCEPT_Q_NEXT( q_ptr, q_link_ptr );
   if( (void *) q_link_ptr->next_ptr != (void *) q_ptr )
   {
     q_temp_ptr = q_link_ptr->next_ptr;
   }
   uq_unlock( q_ptr );
   return q_temp_ptr;
} /* END q_next */


/*===========================================================================
FUNCTION UQ_CHECK

DESCRIPTION
  This function returns a pointer to the data block at the head of the queue.
  The data block is not removed from the queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to uq_init.

RETURN VALUE
  A pointer to the queue item. If the specified queue is empty, then
  NULL is returned.

SIDE EFFECTS
  None
===========================================================================*/
void* uq_check(
  uq_type  *q_ptr
)
{
   uq_link_type  *link_ptr;

   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_link_type  *ret_ptr = NULL;
   #endif

   ASSERT( q_ptr != NULL );
   uq_lock( q_ptr );

   link_ptr = q_ptr->link.next_ptr;

   #ifdef FEATURE_Q_NO_SELF_QPTR
      if( q_ptr->cnt > 0 )
      {
         ret_ptr = link_ptr;
      }
   #endif

   Q_XCEPT_Q_CHECK( q_ptr );

   uq_unlock( q_ptr );

   #ifdef FEATURE_Q_NO_SELF_QPTR
      return (void *)ret_ptr;
   #else
      return link_ptr->self_ptr;
   #endif
} /* END uq_check */

/*===========================================================================
FUNCTION UQ_DELETE

DESCRIPTION
  This function removes an item from a specified queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to uq_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Input item is delete from the queue.
===========================================================================*/
void uq_delete(
   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_type       *q_ptr,         /* Ptr to the Queue */
   #endif
      uq_link_type  *q_delete_ptr   /* Ptr to link of item to delete */
)
{
   #ifdef FEATURE_Q_SINGLE_LINK
      uq_link_type *link_ptr;
      uq_type *real_q_ptr;
      int qcount;
   #endif



   ASSERT( q_ptr        != NULL );
   ASSERT( q_delete_ptr != NULL );
   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_lock( q_ptr );
   #else
      uq_lock( q_delete_ptr->q_ptr );
   #endif

   #ifdef FEATURE_Q_SINGLE_LINK
      #ifdef FEATURE_Q_NO_SELF_QPTR
         real_q_ptr = q_ptr;
      #else
         real_q_ptr = q_delete_ptr->q_ptr;
      #endif

      ASSERT( real_q_ptr != NULL );
      for( qcount = q_ptr->cnt,
           link_ptr           =  (uq_link_type *) real_q_ptr;
           link_ptr->next_ptr != q_delete_ptr && qcount > 0;
           link_ptr           =  link_ptr->next_ptr, qcount--);

      ASSERT( link_ptr != NULL );
      if(qcount > 0)
      {
        link_ptr->next_ptr = q_delete_ptr->next_ptr;

        if(link_ptr->next_ptr == (uq_link_type *) real_q_ptr)
        {
          real_q_ptr->link.prev_ptr = link_ptr;
        }
   #else
	if(q_delete_ptr->prev_ptr != NULL)
	{
          q_delete_ptr->prev_ptr->next_ptr = q_delete_ptr->next_ptr;
	}
	if(q_delete_ptr->next_ptr != NULL)
	{
	  q_delete_ptr->next_ptr->prev_ptr = q_delete_ptr->prev_ptr;
	}
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
        q_ptr->cnt--;
        q_delete_ptr->next_ptr = NULL;
   #else
        q_delete_ptr->q_ptr->cnt--;
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
        Q_XCEPT_Q_DELETE( q_ptr, q_delete_ptr );
   #else
        Q_XCEPT_Q_DELETE( q_delete_ptr );
   #endif
   #ifdef FEATURE_Q_SINGLE_LINK
      }
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_unlock( q_ptr );
   #else
      uq_unlock( q_delete_ptr->q_ptr );
      /* make null after unlock */
      q_delete_ptr->q_ptr = NULL;
   #endif
   return;
} /* END uq_delete */



/*===========================================================================
FUNCTION UQ_INSERT

DESCRIPTION
  This function inserts an item before a specified item on a queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to uq_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Input item is inserted before input item.
===========================================================================*/
void uq_insert(
   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_type    *q_ptr,          /* Ptr to the queue */
   #endif
   uq_link_type  *q_insert_ptr,   /* Ptr to link of item to insert */
   uq_link_type  *q_item_ptr      /* Ptr to link item to insert before */
)
{
   #ifdef FEATURE_Q_SINGLE_LINK
      uq_link_type  *link_ptr;
   #endif

   ASSERT( q_ptr        != NULL );
   ASSERT( q_insert_ptr != NULL );
   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_lock( q_ptr );
   #else
      uq_lock( q_item_ptr->q_ptr );
   #endif

   q_insert_ptr->next_ptr = q_item_ptr;

   #ifdef FEATURE_Q_SINGLE_LINK
      /* Start at beginning of queue and find the item that will be before the
      ** new item
      */
      #ifdef FEATURE_Q_NO_SELF_QPTR
         link_ptr = (uq_link_type *) q_ptr;
      #else
         link_ptr = (uq_link_type *) q_item_ptr->q_ptr;
      #endif

      ASSERT( link_ptr != NULL );
      while (link_ptr->next_ptr != q_item_ptr)
      {
         link_ptr = link_ptr->next_ptr;
         ASSERT( link_ptr != NULL );
      }
      link_ptr->next_ptr = q_insert_ptr;

   #else
      q_insert_ptr->prev_ptr = q_item_ptr->prev_ptr;
      if(q_item_ptr->prev_ptr != NULL)
      {
         q_item_ptr->prev_ptr->next_ptr = q_insert_ptr;
      }
      q_item_ptr->prev_ptr = q_insert_ptr;
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
      q_ptr->cnt++;
   #else
      q_insert_ptr->q_ptr = q_item_ptr->q_ptr;
      q_item_ptr->q_ptr->cnt++;
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
      Q_XCEPT_Q_INSERT( q_ptr, q_insert_ptr, q_item_ptr );
   #else
      Q_XCEPT_Q_INSERT( q_insert_ptr, q_item_ptr );
   #endif

   #ifdef FEATURE_Q_NO_SELF_QPTR
      uq_unlock( q_ptr );
   #else
      uq_unlock( q_item_ptr->q_ptr );
   #endif
   return;
} /* END uq_insert */


/*===========================================================================
FUNCTION UQ_CNT

DESCRIPTION
  This function returns the number of items currently queued on a specified
  queue.

DEPENDENCIES
  The specified queue should have been initialized previously via a call
  to q_init.

RETURN VALUE
  The number of items currently queued on the specified queue.

SIDE EFFECTS
  None.
===========================================================================*/
int uq_cnt(
  uq_type  *q_ptr
)
{
   ASSERT( q_ptr != NULL );
   Q_XCEPT_Q_CNT( q_ptr );
   return q_ptr->cnt;
} /* END q_cnt */


/* ==========================================================================
FUNCTION UQ_LOCK_INIT
DESCRIPTION
   Initializes the mutex for the given queue. It is expected that this call 
   happens only once for every queue. If calling multiple times for a given 
   queue, ensure that the mutex is deleted/freed (uq_lock_delete), prior to it.
DEPENDENCIES
   None.
RETURN VALUE
   None.
SIDE EFFECTS
   None.
============================================================================= */
static void uq_lock_init( uq_type *q_ptr )
{
   ASSERT( q_ptr != NULL );
   memset(&(q_ptr->mutex),0,sizeof(qurt_mutex_t));
   qurt_rmutex_init ( (qurt_mutex_t *)&(q_ptr->mutex) );
   return;
} /* END uq_lock_init */



/*==========================================================================
FUNCTION UQ_INIT

DESCRIPTION
  This function initializes a specified queue. It should be called for each
  queue prior to using the queue with the other Queue Services.

DEPENDENCIES
  None.

RETURN VALUE
  A pointer to the initialized queue.

SIDE EFFECTS
  The specified queue is initialized for use with Queue Services.
===========================================================================*/
uq_type* uq_init(
   uq_type  *q_ptr  /* Ptr to queue to be initialized. */
)
{
   ASSERT( q_ptr != NULL );
   q_ptr->link.next_ptr = (uq_link_type *)(&q_ptr->link);  /* Points to q link. */
   q_ptr->link.prev_ptr = (uq_link_type *)(&q_ptr->link);  /* Points to q link. */

   #ifndef FEATURE_Q_NO_SELF_QPTR
      q_ptr->link.self_ptr = NULL;
      q_ptr->link.q_ptr    = NULL;
   #endif

   q_ptr->cnt = 0;

   uq_lock_init( q_ptr );

   Q_XCEPT_Q_INIT( q_ptr );

   return q_ptr;
} /* END uq_init */

