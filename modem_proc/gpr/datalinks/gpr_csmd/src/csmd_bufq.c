/*========================================================================

*/ /** @file csmd_bufq.c
This file contains the implementations of the Intent Queuing Mechanism exposed to the CSMD
Layer. 

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*======================================================================
                                                                                                    */

/*Amith:
Algorithm: We initially queue n number of intents all stored in one node
           each in a linked list. This would be the freeQ. The used Q would
           be empty at this stage.
           We track the front and rear of both queues to optimize for time
           and we only maintain the same n number of nodes till the end.
           When data comes in, the CSMD layer will pick the intent from the
           head of the freeQ and load the data and fwd to GPR. The head pointer
           is naturally incremented. The node that was used will "move" to the
           usedQ (added to the rear end)
           When client is done consuming the buffer, it is returned - we will
           traverse the usedQ to see where the intent came from, memset it and
           "delete/move" that node from the usedQ and add it to the rear of the
           freeQ.
           All memory is actually freed only when free intents is called from
           csmd_deinit
*/
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\src\csmd_bufq.c

when       who        what, where, why
--------  -----     -------------------------------------------------------
06/07/18   akr         Created file.

========================================================================== */
/*==========================================================================
Include files
========================================================================== */
#include "ar_msg.h"
#include "csmd_bufq.h"
#include "internal_csmd.h"
/*==========================================================================
Globals
========================================================================== */
q_node_t *used_rear  = NULL;
q_node_t *used_front = NULL;
q_node_t *free_rear  = NULL;
q_node_t *free_front = NULL;
/*==========================================================================
Function Definitions
========================================================================== */
/**
@brief Utility to create the Linked List Queue, malloc intents and store in Free List

@param[in] num_intents: number of intents/nodes to queue

@param[in] intent_size: size of each intent to malloc

@return result
*/
uint32_t queue_intents(uint32_t num_intents, uint32_t intent_size)
{
   // should only be called once when the first channel opens when both queues are empty
   if ((NULL != free_rear) && (NULL != used_rear))
   {
      return AR_EALREADY;
   }
   if (num_intents <= 0)
   {
      return AR_EBADPARAM;
   }

   // allocate first node and first intent
   free_front = (q_node_t *)malloc(sizeof(q_node_t));
   if (NULL == free_front)
   {
      return AR_ENORESOURCE;
   }
   // allocate intent
   free_front->intent_ptr = malloc(intent_size);
   if (NULL == free_front->intent_ptr)
   {
      free((void *)free_front);
      return AR_ENORESOURCE;
   }
   memset(free_front->intent_ptr, 0, intent_size);
   free_front->next = NULL;
   free_rear        = free_front;

   for (uint32_t i = 1; i < num_intents; i++)
   {
      // allocate Q_node
      free_rear->next = (q_node_t *)malloc(sizeof(q_node_t));
      if (NULL == free_rear->next)
      {
         return AR_ENORESOURCE;
      }
      free_rear = free_rear->next;
      // allocate intent
      free_rear->intent_ptr = malloc(intent_size);
      if (NULL == free_rear->intent_ptr)
      {
         free((void *)free_rear);
         return AR_ENORESOURCE;
      }
   }
   free_rear->next = NULL;
   return AR_EOK;
}
/**
@brief Utility called when an intent needs to be used to copy data and forward
      we load data into the intent from the freeQ and move that node to usedQ

@param[in] *intent_to_use: variable that we populate with the intent to be used

@return result
*/
uint32_t use_intent(void **intent_to_use_pptr)
{
   // we always use from the freeQ
   if (NULL == free_front)
   {
      return AR_EFAILED;
   }
   // we use the intents from the front of the free queue
   *intent_to_use_pptr = free_front->intent_ptr;

   AR_MSG(DBG_HIGH_PRIO, "using intent %lx from node %p", *intent_to_use_pptr, (void *)free_front);

   if (NULL == *intent_to_use_pptr)
   {
      return AR_EUNEXPECTED;
   }
   // now we need to move this to the usedQ
   if (NULL == used_rear) // first use
   {
      used_rear  = free_front;
      used_front = used_rear;
   }
   else
   {
      used_rear->next = free_front;
      used_rear       = used_rear->next;
   }
   // move the free_front ahead
   free_front      = free_front->next;
   used_rear->next = NULL;
   if (NULL == free_front)
   {
      // if all nodes are used up
      free_rear = NULL;
   }
   return AR_EOK;
}

/**
@brief Utility called when a used intent needs to be returned to the freeQ

@param[in] *ptr: intent that is being returned

@return result
*/
// when client returns the intent with rx_done
uint32_t return_intent(void *ptr)
{
   // we need to search just the usedQ
   if (NULL == used_front)
   {
      // not possible to return something that doesn't belong here
      return AR_EBADPARAM;
   }

   AR_MSG(DBG_HIGH_PRIO, "Returning intent %p", (void *)ptr);

   q_node_t *temp = used_front;
   q_node_t *prev = NULL;
   // need to take care of the used_rear
   // changes only when the only element or the last element is returned.
   if (ptr == temp->intent_ptr)
   {
      // need to remove the first node
      used_front = used_front->next;
      if (NULL == used_front)
      {
         // we're out of nodes here
         used_rear = NULL;
      }
      memset(ptr, 0, GPR_CSMD_1024_BYTES_BUFFER);
      if (NULL != free_rear)
      {
         free_rear->next = temp;
         free_rear       = free_rear->next;
      }
      else
      {
         free_front = temp;
         free_rear  = free_front;
      }
      free_rear->next = NULL;
      return AR_EOK;
   }

   while ((NULL != temp) && (ptr != temp->intent_ptr))
   {
      prev = temp;
      temp = temp->next;
   }
   if (NULL == temp)
   {
      // not present in the queue
      return AR_EBADPARAM;
   }

   prev->next = temp->next; // cut off from usedQ

   if (NULL == prev->next)
   {
      used_rear = prev; // removed the last node from the list
   }
   // then we can memset the intent and return it to the freeQ
   memset(ptr, 0, GPR_CSMD_1024_BYTES_BUFFER);

   if (NULL != free_rear)
   {
      free_rear->next = temp;
      free_rear       = free_rear->next;
   }
   else
   {
      free_front = temp;
      free_rear  = free_front;
   }
   free_rear->next = NULL;
   return AR_EOK;
}

/**
@brief Utility called when all the intents and the List(Q) memories need
      to be freed back to the heap

@param[in] none

@return result
*/
uint32_t free_intents()
{
   q_node_t *temp_1    = free_front;
   q_node_t *temp_2    = used_front;
   q_node_t *next_node = NULL;
   if ((NULL == temp_1) && (NULL == temp_2))
   {
      return AR_EALREADY;
   }

   if (NULL != temp_2)
   {
      AR_MSG(DBG_HIGH_PRIO, "Freeing UsedQ");
      // everything is in usedQ
      while (NULL != temp_2)
      {
         free((void *)temp_2->intent_ptr);
         next_node = temp_2->next;
         free((void *)temp_2);
         temp_2 = next_node;
      }
      used_front = NULL;
   }

   if (NULL != temp_1)
   {
      AR_MSG(DBG_HIGH_PRIO, "Freeing FreeQ");
      // everything is in the freeQ
      while (NULL != temp_1)
      {
         free((void *)temp_1->intent_ptr);
         next_node = temp_1->next;
         free((void *)temp_1);
         temp_1 = next_node;
      }
      free_front = NULL;
   }
   return AR_EOK;
}
