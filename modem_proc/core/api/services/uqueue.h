#ifndef UQUEUE_H
#define UQUEUE_H
/**
  @file queue.h
  @brief This file contains types and declarations associated with the Queue
  Services.

*/
/*===========================================================================
NOTE: The @brief description and any detailed descriptions above do not appear 
      in the PDF. 

      The Utility_Services_API_mainpage.dox file contains all file/group 
      descriptions that are in the output PDF generated using Doxygen and 
      Latex. To edit or update any of the file/group text in the PDF, edit 
      the Utility_Services_API_mainpage.dox file or contact Tech Pubs.

      The above description for this file is part of the "utils_queue" 
      group description in the Utility_Services_API_mainpage.dox file. 
===========================================================================*/
/*===========================================================================

            Q U E U E    S E R V I C E S    H E A D E R    F I L E

DESCRIPTION
 This file contains types and declarations associated with the Queue
 Services for micro image

Copyright (c) 2015 Qualcomm Technologies Incorporated.
All rights reserved.
Qualcomm Confidential and Proprietary. 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath: O:/src/asw/COMMON/vcs/queue.h_v   1.3   16 May 2002 15:43:00   rajeevg  $
$Header: //components/rel/core.mpss/10.0/api/services/uqueue.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/05/15   ps      Add additional APIs required in micro image - CR 865036
04/21/15   ps      Added file for micro image support.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "coreapi_variation.h"
#include "comdef.h"
#if (defined (FEATURE_QUBE))
#include "qube.h"
#endif

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/** @addtogroup utils_queue
@{ */


/** @brief Used as a link field with variables, which allows the variables 
  to be moved on and off the queues. This is the principal structure of Queue
  Utility Services. Note that Users must not directly access the fields of 
  a link; only Queue Utility Services is to access the fields.

  This structure is also used in the definition of the queues themselves.
*/
typedef struct uq_link_struct
{
  struct uq_link_struct  *next_ptr;
  /**< Pointer to the next link in the list. If NULL, there 
  is no next link. */
  struct uq_link_struct  *prev_ptr;
  /**< Pointer to the previous link in the list. If NULL, there 
  is no previous link. */
} uq_link_type;

  /*These are HTORPCMETA comments, Do not delete this.*/
  /*~ FIELD uq_link_struct.next_ptr VOID */
  /*~ FIELD uq_link_struct.prev_ptr VOID */

/** Used instead of uq_link_type when the queue items are linked in 
  a single link list. This avoids the overhead of traversing 
  the entire link list when queueing at the end of the link list. The 
  uq_link_struct structure is to be accessed only by Queue Utility Services.
*/
typedef uq_link_type uq_head_link_type;


/** @brief Used by Queue Utility Services to represent a queue. Note that 
  users must not directly access the fields of a queue; only Queue Utility 
  Services is to access the fields.
*/
typedef struct uq_struct
{
  uq_head_link_type  link; /**< Used for linking the items into a queue. */

  int          cnt;       /**< Keeps track of the number of items enqueued. 
                               This field is not necessary for normal 
                               operations and is used for debugging 
                               purposes. */

  /* Using the mutex structure here instead of a pointer to avoid dependency on malloc
     in micro image mode  */
  qurt_mutex_t     mutex;

} uq_type;

/*===========================================================================

                             Macro Definitions

===========================================================================*/

  /** @cond 
  */
  #define Q_XCEPT_Q_INIT( q_ptr )
  #define Q_XCEPT_Q_LINK( q_ptr, link_ptr )
  #define Q_XCEPT_Q_PUT( q_ptr, link_ptr )
  #define Q_XCEPT_Q_GET( q_ptr )
  #define Q_XCEPT_Q_CNT( q_ptr )
  #define Q_XCEPT_Q_CNT( q_ptr )
  #define Q_XCEPT_Q_CHECK( q_ptr )
  #define Q_XCEPT_Q_NEXT( q_ptr, link_ptr )
  #define Q_XCEPT_Q_INSERT( q_ptr, q_insert_ptr, q_item_ptr )
  #define Q_XCEPT_Q_DELETE( q_ptr, q_delete_ptr )

  /** @endcond */

/**
  Determines if the item passed in is in a queue.

  @param[in] uq_link_ptr Pointer to the item to be tested.

  @return
  TRUE -- Item is already in a queue. \n
  FALSE -- Item is not in a queue.

  @dependencies
  None.
*/
#define UQ_ALREADY_QUEUED( uq_link_ptr ) \
   ((uq_link_ptr)->next_ptr != NULL)

/*===========================================================================

                            Function Declarations

===========================================================================*/
#ifdef __cplusplus
   extern "C" {
#endif


/**
  Initializes a specified queue. This function must be called for each
  queue before using the queue with other Queue Utility Services.

  @param[in] q_ptr Pointer to the queue to initialize.

  @return
  A pointer to the initialized queue.

  @dependencies
  None.
*/
uq_type* uq_init( uq_type *q_ptr );


/**
  Initializes a specified link. This function must be called for each
  link before using the link with other Queue Utility Services.
 
  @param[in] item_ptr Item to be initialized.
  @param[in] link_ptr Link to be initialized.

  @return
  A pointer to the initialized link.

  @dependencies
  None.
*/
uq_link_type* uq_link( void *item_ptr, uq_link_type *link_ptr );


/**
  Enqueues an item to a specified queue using a specific link.
 
  @param[in] q_ptr    Pointer to the item to be enqueued.
  @param[in] link_ptr Pointer to the link where the item is to be placed.

  @return
  None.

  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init(). \n
  The specified link field of the item must have been initialized previously 
  via a call to q_link().

  @sideeffects
  The specified item is placed at the tail of a specific queue.
*/
void  uq_put( uq_type *q_ptr, uq_link_type *link_ptr );


/**
  Removes an item from the head of a specified queue.

  @param[in] q_ptr Pointer to the queue.

  @return
  Returns a pointer to the dequeued item, or NULL if the specified queue
  is empty.

  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
void* uq_get ( uq_type *q_ptr );

/**
  Returns the number of items currently queued on a specified queue.
 
  @param[in] q_ptr Pointer to the queue.

  @return
  The number of items.

  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
int uq_cnt( uq_type *q_ptr );

/**
  Returns a pointer to the data block at the head of the queue. The data 
  block is not removed from the queue.

  @param[in] q_ptr Pointer to the queue.

  @return
  Returns a pointer to the queue item, or NULL if the specified queue
  is empty.
 
  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
void* uq_check (uq_type  *q_ptr);
/**
  Returns a pointer to the next item on the queue.

  @param[in] q_ptr    Pointer to the queue.
  @param[in] link_ptr Pointer to the link on the queue.

  @return
  Returns a pointer to the next item on the queue, or NULL if the end of 
  the queue is reached.
 
  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
void* uq_next ( uq_type *q_ptr, uq_link_type *link_ptr );


/**
  Inserts an item before a specified item on the queue.

  @param[in] q_ptr        Pointer to the queue.
  @param[in] q_insert_ptr Pointer to where the item is to be inserted.
  @param[in] q_item_ptr   Pointer to the item to insert.

  @return
  None.

  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
void uq_insert  ( 
#ifdef FEATURE_Q_NO_SELF_QPTR
		 uq_type *q_ptr, 
#endif
		 uq_link_type *q_insert_ptr, 
		 uq_link_type *q_item_ptr );

/**
  Deletes an item from a specified queue.

  @param[in] q_ptr        Pointer to the queue.
  @param[in] q_delete_ptr Pointer to the item to be deleted.

  @return
  None.

  @dependencies
  The specified queue must have been initialized previously via a call
  to q_init().
*/
void uq_delete  ( uq_type *q_ptr, uq_link_type *q_delete_ptr );

/**
  Destroys a specified queue. This function is to be called if the queue 
  is no longer required.
  
  @param[in] q_ptr Pointer to the queue to be destroyed.

  @return
  None.

  @dependencies
  None.

  @sideeffects
  Elements in the queue will not be accessible. To avoid leaks, it is user's 
  responsibility to deallocate the memory allocated for the queue and its 
  elements.
*/
void uq_destroy(
   uq_type  *q_ptr
);


#ifdef __cplusplus
   }
#endif

/** @} */ /* end_addtogroup utils_queue */
#endif /* UQUEUE_H */
