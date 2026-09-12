/*
#============================================================================
#  Name:
#    servreg_monitor.c 
#
#  Description:
#    Service Registry Monitor(mon) feature. 
#
# Copyright (c) 2015-2019 by Qualcomm Technologies Incorporated. All Rights Reserved.
#============================================================================
*/
#include "stdarg.h"
#include "stdlib.h"
#include <stringl/stringl.h>
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "err.h"
#include "queue.h"
#include "timer.h"

#include "servreg_internal.h"
#include "servreg_locator.h"
#include "servreg_localdb.h"
#include "servreg_utils.h"
#include "servreg_monitor.h"
#include "servreg_notifier.h"
#include "servreg_qdi.h"
#include "gen_cb_ctxt.h"

#include "tms_utils_msg.h"
#include "tms_utils.h"

#define SERVREG_MON_ACK_TIMEOUT_COUNT         10
#define SERVREG_MON_ACK_TIME_CHK_SEC          1
#define SERVREG_MAX_TXN_ID                    100

/* Type casts as accessor functions */
#define sr_mon_node2sr_mon_handle(x)        ((SERVREG_MON_HANDLE)x)
#define sr_mon_handle2sr_mon_node(x)        ((servreg_mon_node_p)x)

/* Internal Monitor Structure */
struct servreg_mon_internal_s
{
   servreg_mon_node_p servreg_mon_node_hashtab[SERVREG_MON_NODE_HASHTABLE_BUCKETS];
   timer_type servreg_mon_ack_timer;
   timer_group_type servreg_group;
   qurt_mutex_t mutex; /* Generic mutex for hashtab protection */
};
struct servreg_mon_internal_s servreg_mon_internal;

/* Static Functions defined in this file */
static SERVREG_MON_HANDLE servreg_create_mon_node(SERVREG_NAME sr_name, RCESN_HANDLE rc_handle);
static void servreg_service_queue(SERVREG_MON_HANDLE sr_mon_handle);
static int servreg_compare_state(void* item_ptr, void* compare_val);
static void servreg_generic_task_cb(timer_cb_data_type unused);

/* Hash Functions */
static servreg_mon_node_p servreg_mon_node_hashtab_get(SERVREG_NAME const name, servreg_hash_t hash);
static void servreg_mon_node_hashtab_put(servreg_mon_node_p sr_mon_node, servreg_hash_t hash);
static void servreg_mon_node_hashtab_delete(servreg_mon_node_p sr_mon_node_del, servreg_hash_t hash);

void servreg_mon_ack_timer_cb(void* unused);

/** =====================================================================
 * Function:
 *     servreg_mon_node_alloc
 *
 * Description:
 *     Allocates the servreg monitor node
 *
 * Parameters:
 *     None
 *
 * Returns:
 *    Dynamically allocated servreg monitor node
 * =====================================================================  */
static servreg_mon_node_p servreg_mon_node_alloc(void)
{
   servreg_mon_node_p new_node = SERVREG_NULL;

   new_node = (servreg_mon_node_p) calloc(sizeof(servreg_mon_node_t), 1);

   if (SERVREG_NULL != new_node)
   {
      new_node->mon_signature = SERVREG_MON_SIGNATURE;
   }
   else
   {
      ERR_FATAL( "servreg_mon_node_p calloc failed", 0, 0, 0);
   }

   return new_node;
}

/** =====================================================================
 * Function:
 *     servreg_mon_node_free
 *
 * Description:
 *     It will free memories which were allocated in servreg_create_mon_node() 
 *
 * Parameters:
 *     Servreg monitor node, for which we need to free memories.
 *
 * Returns:
 *    None
 * =====================================================================  */
static void servreg_mon_node_free(servreg_mon_node_p sr_mon_node)
{
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;

   if(SERVREG_NULL != sr_mon_node)
   {
      if (SERVREG_NULL != sr_mon_node->service_name)
      {
         free(sr_mon_node->service_name);
         sr_mon_node->service_name = SERVREG_NULL;
      }

      sr_q_entry = (servreg_mon_queue_p)q_get(sr_mon_node->state_q);
      while(SERVREG_NULL != sr_q_entry)
      {
         free(sr_q_entry);
         sr_q_entry = (servreg_mon_queue_p)q_get(sr_mon_node->state_q);
      }

      q_destroy(sr_mon_node->state_q);
      free(sr_mon_node->state_q);
      sr_mon_node->state_q = SERVREG_NULL;

      qurt_pimutex_destroy(&(sr_mon_node->node_mutex));

      free(sr_mon_node);
      sr_mon_node = SERVREG_NULL;
   }
}

/** =====================================================================
 * Function:
 *     servreg_mon_node_hashtab_get
 *
 * Description:
 *     Given the hash value, get the corresponding sr monitor node
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" name
 *     hash : hash value which is based on the service name
 *
 * Returns:
 *     servreg_mon_node_p : sr monitor node that corresponds to the hash value
 * =====================================================================  */
static servreg_mon_node_p servreg_mon_node_hashtab_get(SERVREG_NAME const name, servreg_hash_t hash)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_mon_internal.mutex));

   sr_mon_node = servreg_mon_internal.servreg_mon_node_hashtab[hash];

   while(SERVREG_NULL != sr_mon_node)
   {
      qurt_pimutex_lock(&(sr_mon_node->node_mutex));

      /* If they dont have the same address */
      if(sr_mon_node->service_name != name)
      {
         int len1 = servreg_nmelen(sr_mon_node->service_name);
         int len2 = servreg_nmelen(name);

         if(len1 == len2 && 0 == servreg_nmecmp(sr_mon_node->service_name, name, len1))
         {
            //TMS_MSG_HIGH_1("handle = 0x%x ", sr_mon_handle);
            qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
            break;
         }
      }
      else
      {
         //TMS_MSG_HIGH_1("handle = 0x%x", sr_mon_handle);
         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
         break;
      }
      qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      sr_mon_node = sr_mon_node->next;
   }

   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

   return sr_mon_node;
}

/** =====================================================================
 * Function:
 *     servreg_mon_node_hashtab_put
 *
 * Description:
 *     Put the sr monitor node to the hash table with the given hash value
 *
 * Parameters:
 *     sr_mon_node : sr monitor node to be put into the hash table
 *     hash        : hash value associated with the sr montior node
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_mon_node_hashtab_put(servreg_mon_node_p sr_mon_node, servreg_hash_t hash)
{
   qurt_pimutex_lock(&(servreg_mon_internal.mutex));

   /*  Insert to head of list for that hash value */
   sr_mon_node->next = servreg_mon_internal.servreg_mon_node_hashtab[hash];

   /* Update head for that hash value */
   servreg_mon_internal.servreg_mon_node_hashtab[hash] = sr_mon_node;

   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));
}

/** =====================================================================
 * Function:
 *     servreg_mon_node_hashtab_delete
 *
 * Description:
 *     Delete the sr monitor node from the hash table with the given hash value
 *
 * Parameters:
 *     sr_mon_node : sr monitor node to be put into the hash table
 *     hash        : hash value associated with the sr montior node
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_mon_node_hashtab_delete(servreg_mon_node_p sr_mon_node_del, servreg_hash_t hash)
{
   servreg_mon_node_p sr_mon_node_curr = SERVREG_NULL, sr_mon_node_prev = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_mon_internal.mutex));

   /* Get the head of the list */
   sr_mon_node_curr = servreg_mon_internal.servreg_mon_node_hashtab[hash];

   while(SERVREG_NULL != sr_mon_node_curr)
   {
      if(sr_mon_node_curr == sr_mon_node_del)
      {
         if(SERVREG_NULL == sr_mon_node_prev)
         {
            /* Delete the first node */
            servreg_mon_internal.servreg_mon_node_hashtab[hash] = sr_mon_node_curr->next;
         }
         else
         {
            sr_mon_node_prev->next = sr_mon_node_curr->next;
         }

         sr_mon_node_curr->next = SERVREG_NULL;

         break; /* exit while() loop */
      }

      sr_mon_node_prev = sr_mon_node_curr;
      sr_mon_node_curr = sr_mon_node_curr->next;
   }

   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

   return;
}

/** =====================================================================
 * Function:
 *     servreg_create_mon_node
 *
 * Description:
 *     Creates a mon node with the given sr_name and rcesn handle
 *
 * Parameters:
 *    sr_name : domain+service or just domain name
 *    rc_handle : rcesn handle
 *
 * Returns:
 *    SERVREG_MON_HANDLE : handle to the mon node
 * =====================================================================  */
static SERVREG_MON_HANDLE servreg_create_mon_node(SERVREG_NAME sr_name, RCESN_HANDLE rc_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   SERVREG_RESULT result = SERVREG_FAILURE;
   servreg_relationship_type relation = RELATIONSHIP_NOT_ESTABLISHED;

   qurt_pimutex_lock(&(servreg_mon_internal.mutex));

   /* Check if the MON node exists */
   sr_mon_handle = servreg_get_sr_mon_handle(sr_name);

   if(SERVREG_NULL == sr_mon_handle)
   {
      /* Allocate a new mon node */
      sr_mon_node = servreg_mon_node_alloc();

      SERVREG_TRACE_2(NODE_CREATE, sr_mon_node, 1);

      if(SERVREG_NULL != sr_mon_node && RCESN_NULL != rc_handle)
      {        
         qurt_pimutex_init(&(sr_mon_node->node_mutex));
         qurt_pimutex_lock(&(sr_mon_node->node_mutex));
         
         /* In Servreg Local Db servreg_concat() allocates memory for sr_name */
         sr_mon_node->service_name = sr_name;
         sr_mon_node->rcesn_handle = rc_handle;
         if(rcesn_setstatecurr_handle(sr_mon_node->rcesn_handle, SERVREG_SERVICE_STATE_UNINIT)){;}
         sr_mon_node->handle_count = 1;
         /* If there is no indication given after node creation and ACK is received
            then servreg_set_ack() should return failure. To achieve that, initializing with SR_INITIAL_TRANSACTION_ID(-1)
            It will also prevent "multiple ACKs were given for one indication"(Only if there is no EDOWN transaction happened in that sr_mon_node,
            Once EDOWN transaction happened , it will not be able to prevent this case)*/
         sr_mon_node->e_d_transaction_id = SR_INITIAL_TRANSACTION_ID;

         result = servreg_calculate_relationship(sr_mon_node->service_name, &relation);
         if (SERVREG_SUCCESS == result)
           sr_mon_node->relationship = relation;

         sr_mon_node->state_q = (q_type *)calloc(sizeof(q_type), 1);

         if(SERVREG_NULL != sr_mon_node->state_q)
         {
            /* Initialize the queue */
            memset((q_type *)sr_mon_node->state_q, 0, sizeof(q_type));
            q_init(sr_mon_node->state_q);
         }
         else
         {
            ERR_FATAL("Calloc failed", 0, 0, 0);
         }

         sr_mon_node->next = SERVREG_NULL;

         servreg_mon_node_hashtab_put(sr_mon_node, servreg_nmehash(sr_mon_node->service_name, (sizeof(servreg_mon_internal.servreg_mon_node_hashtab) / sizeof(servreg_mon_node_p))));

         if(rcesn_signal_handle(rc_handle, SERVREG_SERVICE_STATE_UNINIT)){;}

         sr_mon_handle = sr_mon_node2sr_mon_handle(sr_mon_node);

         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      }
      else
      {
         /* Free sr_name since entry failed */
         free(sr_name);
         SERVREG_TRACE_0(FAIL);
         TMS_MSG_ERROR("sr_mon_node NULL");
      }
   }
   else
   {
      /* Free sr_name since its a duplicate entry */
      free(sr_name);
      sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

      qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      sr_mon_node->handle_count = sr_mon_node->handle_count + 1;
      qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
         
      SERVREG_TRACE_2(NODE_CREATE_DUP, sr_mon_node, sr_mon_node->handle_count);
   }

   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));
   return sr_mon_handle;
}

/** =====================================================================
 * Function:
 *     servreg_service_queue
 *
 * Description:
 *     Function services the state queue for the given monitor handle.
 *     It will set the next pending state of the service.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_service_queue(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;

   //make get count and signal atomic so the number of listeners doesn't change in between
   //todo: we should consider a mutex within each node to give finer granularity
   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("Invalid sr_mon_node");
      return;
   }

   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
   sr_q_entry = (servreg_mon_queue_p)q_check(sr_mon_node->state_q);

   //ack_pending of 0 indicates unserviced since we remove it from Q otherwise
   while (SERVREG_NULL != sr_q_entry && 0 == sr_q_entry->ack_pending_count)
   {
      sr_q_entry->ack_pending_count = rcesn_getregistrants_handle(sr_mon_node->rcesn_handle);

      SERVREG_TRACE_2(SERVICE_Q, sr_mon_node, sr_q_entry->ack_pending_count);

      sr_mon_node->transaction_id = sr_mon_node->transaction_id + 1;
      if(SERVREG_MAX_TXN_ID < sr_mon_node->transaction_id)
      {
         sr_mon_node->transaction_id = (sr_mon_node->transaction_id)%SERVREG_MAX_TXN_ID;
      }
      /* Always signal for rcesn to get correct state */
      if(rcesn_signal_handle(sr_mon_node->rcesn_handle, sr_q_entry->curr_state)){;}

      /* Signal listeners that the state of the service has changed for the first entry in the queue */
      if (sr_q_entry->ack_pending_count > 0)
      {
         /* There are listeners, so resume ACK check timer (if inactive) and wait for acknowledgement */
         if(FALSE == timer_is_active(&servreg_mon_internal.servreg_mon_ack_timer))
         {
            if(TE_SUCCESS != timer_set_64(&servreg_mon_internal.servreg_mon_ack_timer,
                                   SERVREG_MON_ACK_TIME_CHK_SEC,
                                   0,
                                   T_SEC))
            {
               ERR_FATAL("Could not set ACK check timer", 0, 0, 0);
            }
            SERVREG_TIME_TRACE_0(ACK_TIMER_RESUME);
            SERVREG_TRACE_TIME();
         }
         /* else */
         /* timer already active */
	       break;        
      }
      else
      {
         /* Continue processing the remainder of the queue */
	       /* Remove from the queue since no listeners attached to it */
         sr_q_entry = (servreg_mon_queue_p)q_get(sr_mon_node->state_q);
         free(sr_q_entry);

         /* Get the next entry in the queue and loop */
         sr_q_entry = (servreg_mon_queue_p)q_check(sr_mon_node->state_q);
      }
      
   }

   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
   
   return;
}

/** =====================================================================
 * Function:
 *     servreg_compare_state
 *
 * Description:
 *     Callback function given to the queue API's
 *     Function compares the state of q_entry state and the one passes in as a parameter
 *
 * Parameters:
 *     item_ptr : pointer to the item in the queue
 *     compare_val : service state value to be compared against
 *
 * Returns:
 *     1 : if state matches
 *     0 : if state does not match
 * =====================================================================  */
static int servreg_compare_state(void* item_ptr, void* compare_val)
{
   int ret = 0;

   if(((servreg_mon_queue_p)item_ptr)->curr_state == *((uint32_t *)compare_val))
   {
      ret = 1;
   }
   else
   {
      ret = 0;
   }

   return ret;
}

/** =====================================================================
* Function:
*     servreg_mon_ack_timer_cb
*
* Description:
*     Generic callback task driven function to check the state of the
*     ACKs for all the monitor nodes
*
* Parameters:
*     unused
*
* Returns:
*     none
* =====================================================================  */
void servreg_mon_ack_timer_cb(void* unused)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;
   uint32_t i = 0;
   uint32_t ack_check_timer_status = 0;    /* Increment counter if ACK is expected */
   
   qurt_pimutex_lock(&(servreg_mon_internal.mutex));

   /* Run through each mon_node and check for listener ACKs */
   for(i = 0; i < SERVREG_MON_NODE_HASHTABLE_BUCKETS; i++)
   {
      sr_mon_node = servreg_mon_internal.servreg_mon_node_hashtab[i];

      while(SERVREG_NULL != sr_mon_node)
      {
         sr_q_entry = (servreg_mon_queue_p)q_check(sr_mon_node->state_q);

         /* An entry in the queue suggests that the head q_entry state is set and its awaiting for the ACKs */
         if(SERVREG_NULL != sr_q_entry)
         {
            /* Increment to keep timer active */
            ++ack_check_timer_status;

            /* Decrement the timeout value */
            sr_q_entry->timeout = sr_q_entry->timeout - 1;
            
            SERVREG_TRACE_2(ACK_TIMER_CB, sr_mon_node, sr_q_entry->timeout);
            
            /* If listener does not ACK after 10 seconds, bail out with an ERR_FATAL */
            if(sr_q_entry->timeout == 0)
            {
               char * temp = calloc(sizeof (char), TMS_UTILS_BUFFER_SIZE);
               if ( temp != NULL )
               {
                 (void)tms_utils_fmt(temp, TMS_UTILS_BUFFER_SIZE, "Servreg ack timedout:%s:State:0x%x", sr_mon_node->service_name,sr_q_entry->curr_state );
                 ERR_FATAL_AUX_MSG( "Servreg ack timedout. Check coredump.err.aux_msg", temp, TMS_UTILS_BUFFER_SIZE);
               }
               else
               {
                 ERR_FATAL( "Servreg ack timedout:state:0x%x", sr_q_entry->curr_state, 0, 0);
               }
            }
         }

         /* Check next node */
         sr_mon_node = sr_mon_node->next;
      }
   }

   /* ACK check timer will be inactive after expiry and will not be reloaded if queue is empty */
   if (0 == ack_check_timer_status)
   {
      SERVREG_TIME_TRACE_0(ACK_TIMER_STOPPED);
      SERVREG_TRACE_TIME();
   }
   /* Resume ACK check timer as it is inactive after expiry and further ACK checks are needed */
   else
   {
      if(TE_SUCCESS != timer_set_64(&servreg_mon_internal.servreg_mon_ack_timer,
                             SERVREG_MON_ACK_TIME_CHK_SEC,
                             0,
                             T_SEC))
      {
         ERR_FATAL("Could not set ACK check timer", 0, 0, 0);
      }
      SERVREG_TIME_TRACE_2(ACK_TIMER_RELOAD, ack_check_timer_status, 0);
      SERVREG_TRACE_TIME();
   }

   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

   return;
}

/** =====================================================================
* Function:
*     servreg_generic_task_cb
*
* Description:
*     Timer driven function to offload and run from generic callback task 
*     context instead of timer context
*
* Parameters:
*     unused
*
* Returns:
*     none
* =====================================================================  */
static void servreg_generic_task_cb(timer_cb_data_type unused)
{
   gen_cb_ctxt_invoke_cb(servreg_mon_ack_timer_cb, NULL);
}

/** =====================================================================
 * Function:
 *     servreg_get_sr_mon_handle
 *
 * Description:
 *     Checks if a MON node already exists with the given name.
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" name
 *
 * Returns:
 *    SERVREG_MON_HANDLE : handle to the mon node
 * =====================================================================  */
SERVREG_MON_HANDLE servreg_get_sr_mon_handle(SERVREG_NAME const name)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;

   sr_mon_node = servreg_mon_node_hashtab_get(name, servreg_nmehash(name, (sizeof(servreg_mon_internal.servreg_mon_node_hashtab) / sizeof(servreg_mon_node_p))));

   sr_mon_handle = sr_mon_node2sr_mon_handle(sr_mon_node);

   return sr_mon_handle;
}

/** =====================================================================
 * Function:
 *     servreg_set_transaction_id
 *
 * Description:
 *     Set the current transaction_id.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 * =====================================================================  */
void servreg_set_transaction_id(SERVREG_MON_HANDLE sr_mon_handle, int32_t sr_transaction_id)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      ERR_FATAL("Invalid sr_mon_node", 0, 0, 0);
      return;
   }

   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
   
   sr_mon_node->e_d_transaction_id = sr_mon_node->transaction_id =  sr_transaction_id;
   //TMS_MSG_HIGH_1("transaction_id = %d\n ", sr_transaction_id);
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return;
}

/** =====================================================================
 * Function:
 *     servreg_alloc_monitor_handle
 *
 * Description:
 *     Given the domain + service name, this functions returns a handle
 *
 * Parameters:
 *     domain : "soc/domain/subdomain" info
 *     service : "provider/service" info. This field can be SERVREG_NULL also.
 *
 * Returns:
 *     SERVREG_MON_HANDLE : Opaque handle to a service state
 *                          Check for return value and if it is NOT NULL
 * =====================================================================  */
SERVREG_MON_HANDLE servreg_alloc_monitor_handle(SERVREG_NAME domain, SERVREG_NAME service)
{
   SERVREG_NAME servreg_name = SERVREG_NULL, rcesn_name = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   RCESN_HANDLE rcesn_handle = RCESN_NULL;

   if(SERVREG_SUCCESS == servreg_name_check(domain, service))
   {
      servreg_name = servreg_concat(domain, service);
      
      if(SERVREG_NULL != servreg_name)
      {
         /* First create the rcesn node with "sr:xxx" name */
         rcesn_name = servreg_create_rcesn_name(servreg_name);

         if(SERVREG_NULL != rcesn_name)
         {
            rcesn_handle = rcesn_create_name(rcesn_name);

            /* Free the space allocated for rcesn_name as rcesn internally does a strlcpy of the name */
            free(rcesn_name);
         }

         if(RCESN_NULL != rcesn_handle)
         {
            /* Then Create the MON node */
            sr_mon_handle = servreg_create_mon_node(servreg_name, rcesn_handle);

            if(SERVREG_NULL != sr_mon_handle)
            {
               //TMS_MSG_HIGH_1("handle = 0x%x ", sr_mon_handle);
            }
            else
            {
               TMS_MSG_ERROR("sr_mon_handle NULL");
            }
         }
         else
         {
            ERR_FATAL( "In servreg_alloc_monitor_handle() rcesn_handle not created", 0, 0, 0);
         }
      }
      else
      {
         TMS_MSG_ERROR("servreg_name NULL");
      }
   }
   else
   {
      TMS_MSG_ERROR("Invalid domain and/or service name");
   }

   return sr_mon_handle;
}

/** =====================================================================
 * Function:
 *     servreg_free_monitor_handle
 *
 * Description:
 *     Free the monitor handle that is associated to an event mapped by domain+service name
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 *
 * Note : 
 *     If clients are no longer interested in the service AND they have de-registered their listeners
 *     using servreg_deregister_listener(), they can free the handle by calling servreg_free_monitor_handle().
 * =====================================================================  */
SERVREG_RESULT servreg_free_monitor_handle(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   servreg_relationship_type relation = RELATIONSHIP_NOT_ESTABLISHED;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("sr_mon_handle is null/has invalid signature");
      ret = SERVREG_INVALID_HANDLE;
      goto common_return;
   }

   ret = servreg_calculate_relationship(sr_mon_node->service_name, &relation);
   if (SERVREG_SUCCESS != ret)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("Not able to establish relationship");
      goto common_return;
   }

   /*Take Global Mutex */
   qurt_pimutex_lock(&(servreg_mon_internal.mutex));
   /*Take Node Mutex*/
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   if(0 == sr_mon_node->handle_count)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("No clients registered to free them");
      ret = SERVREG_FAILURE;
      goto unlock_return;
   }
   
   sr_mon_node->handle_count = sr_mon_node->handle_count - 1;

   if(0 != sr_mon_node->handle_count)
   {
      /* Will free monitor handle if handle_count is 0: All clients have de-registered */
      SERVREG_TRACE_0(SUCCESS);
      ret = SERVREG_SUCCESS;
      goto unlock_return;
   }

   /* handle_count is 0 so All clients have de-registered so free monitor handle in remote/user entities */


   /* Classifying services into following categories:
      1) Local
      2) Remote
      3) User (Local to subsystem) (Applicable only for root PD )
   */
   switch(relation)
   {
      /* Services which are in same PD are considered local for that PD clients*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      {
         SERVREG_TRACE_2(FREE_LOCAL,relation,sr_mon_node);
         goto reclaim;
      }
      /* Services which are in Root PD  are considered remote services for User PD clients.
         Services which are in different subsystem are considered remote services for User PD/Root PD clients.*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT: /*QDI call to root PD */
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT: /*QMI call to other subsystem*/
      {
         SERVREG_TRACE_2(FREE_REMOTE,relation,sr_mon_node);
         if(SERVREG_SUCCESS != servreg_free_remote_handle(sr_mon_node->service_name, sr_mon_handle))
            SERVREG_TRACE_0(FAIL);
         goto reclaim;
      }
      /* Services which are in User PD are considered user services for Root PD clients.*/
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS: /*QDI call to user PD*/
      {
         SERVREG_TRACE_2(FREE_USER,relation,sr_mon_node);
         if(SERVREG_SUCCESS != servreg_free_user_handle(sr_mon_node->service_name, sr_mon_handle))
            SERVREG_TRACE_0(FAIL);
         goto reclaim;
      }
      default :
      {
         SERVREG_TRACE_0(FAIL);
         TMS_MSG_ERROR_1("Relationship %d is not expected",relation);
         ret = SERVREG_FAILURE;
         goto unlock_return;
      }
   }

reclaim:

   /* Delete the entry from the hash table */
   servreg_mon_node_hashtab_delete(sr_mon_node, servreg_nmehash(sr_mon_node->service_name, (sizeof(servreg_mon_internal.servreg_mon_node_hashtab) /  sizeof(servreg_mon_node_p))));

   /*Release node mutex*/
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   /*Release Global Mutex */
   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

   /* Free the node */
   servreg_mon_node_free(sr_mon_node);

   SERVREG_TRACE_0(SUCCESS);
   ret = SERVREG_SUCCESS;

   goto common_return;

unlock_return:
   /*Release Node Mutex */
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
   /*Release Global Mutex */
   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

common_return:
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_set_state
 *
 * Description:
 *     Function sets the state of the service
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : State of the service. See enum SERVREG_SERVICE_STATE for options
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_state(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_BOOL valid_state = SERVREG_FALSE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("Invalid sr_mon_node ");
      return SERVREG_INVALID_HANDLE;
   }

   qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   ret = servreg_is_valid_state_change(sr_mon_handle, state, &valid_state);

   /* Signal only for state change */
   if(SERVREG_TRUE == sr_mon_node->servreg_sync && SERVREG_TRUE == valid_state && SERVREG_SERVICE_STATE_EARLY_DOWN != state)
   {
      SERVREG_TRACE_2(SET_STATE, sr_mon_node, state);

      sr_q_entry = (servreg_mon_queue_p)calloc(sizeof(servreg_mon_queue_t), 1);
      if(SERVREG_NULL == sr_q_entry)
      {
         ERR_FATAL("In servreg_set_state() Calloc failed", 0, 0, 0);
      }

      sr_q_entry->ack_pending_count = 0; //set this when we send notifications
      sr_q_entry->curr_state = state;
      sr_q_entry->timeout = SERVREG_MON_ACK_TIMEOUT_COUNT;
      q_put(sr_mon_node->state_q, q_link(sr_q_entry, &sr_q_entry->link));
      servreg_service_queue(sr_mon_node);
   }
   else if(SERVREG_TRUE == valid_state && SERVREG_SERVICE_STATE_EARLY_DOWN == state)
   {
      SERVREG_TRACE_2(SET_STATE_ED, sr_mon_node, state);
      /* Signal all the listeners that the state of the service has changed */
      if (rcesn_signal_handle(sr_mon_node->rcesn_handle, state)){};
      sr_mon_node->e_d_transaction_id = sr_mon_node->transaction_id = sr_mon_node->transaction_id + 1;
      if(SERVREG_MAX_TXN_ID < sr_mon_node->transaction_id)
      {
         sr_mon_node->e_d_transaction_id = sr_mon_node->transaction_id = (sr_mon_node->transaction_id)%SERVREG_MAX_TXN_ID;
      }
   }
   else if(SERVREG_TRUE == valid_state)
   {
      SERVREG_TRACE_2(SET_STATE_E, sr_mon_node, state);
      /* Signal all the listeners that the state of the service has changed */
      if (rcesn_signal_handle(sr_mon_node->rcesn_handle, state)){};
      sr_mon_node->transaction_id = sr_mon_node->transaction_id + 1;
      if(SERVREG_MAX_TXN_ID < sr_mon_node->transaction_id)
      {
         sr_mon_node->transaction_id = (sr_mon_node->transaction_id)%SERVREG_MAX_TXN_ID;
      }
   }

   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_service_name
 *
 * Description:
 *     Given the monitor handle, the function returns the service name 
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     service_name or SERVREG_NULL
 * =====================================================================  */
SERVREG_NAME servreg_get_service_name(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_NAME ret = SERVREG_NULL;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }

   //qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   ret = sr_mon_node->service_name;
   //TMS_MSG_HIGH_1("name = %s ", *ret);

   //qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_set_synchronous
 *
 * Description:
 *     Function sets the service state to be synchronous 
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_synchronous(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      
   sr_mon_node->servreg_sync = SERVREG_TRUE;
   ret = SERVREG_SUCCESS;
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_set_asynchronous
 *
 * Description:
 *     Function sets the service state to be asynchronous 
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_asynchronous(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      
   sr_mon_node->servreg_sync = SERVREG_FALSE;
   ret = SERVREG_SUCCESS;
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_synchronous
 *
 * Description:
 *     Function to get the sync state of the service
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *    TRUE if the sync state is set to TRUE, else FALSE
 * =====================================================================  */
SERVREG_BOOL servreg_get_synchronous(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_BOOL ret = SERVREG_FALSE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      
   ret = sr_mon_node->servreg_sync;
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_pending_acks
 *
 * Description:
 *     Function gets the pending ack count for the service state
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : The service state for which the pending ack is requested
 * Returns:
 *     Ack count
 * =====================================================================  */
uint32_t servreg_get_pending_acks(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;
   uint32_t ret = 0;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

   if(SERVREG_NULL != sr_mon_node)
   {
      if(SERVREG_MON_SIGNATURE == sr_mon_node->mon_signature)
      {
         qurt_pimutex_lock(&(sr_mon_node->node_mutex));
         sr_q_entry = (servreg_mon_queue_p)q_linear_search(sr_mon_node->state_q, servreg_compare_state, &state);

         if(SERVREG_NULL != sr_q_entry)
         {
            ret = sr_q_entry->ack_pending_count;
            TMS_MSG_HIGH_1("Service has pending acks: %d", ret);
         }
         else
         {
            TMS_MSG_HIGH("State does not exist in queue, so ACK:0");
         }
         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      }
      else
      {
         ERR_FATAL("In servreg_get_pending_acks() sr_mon_handle has invalid signature", 0, 0, 0);
      }
   }
   else
   {
      ERR_FATAL("sr_mon_node NULL", 0, 0, 0);
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_register_listener
 *
 * Description:
 *     Register a listener to get event notifications for a service it is interested in
 *
 * Parameters:
 *     sr_mon_handle        : Handle to an existing service state which is mapped by domain + service 
 *                            or just domain name. This is the service that the listener is interested in.
 *     servreg_signal_type  : NHLOS specific notification signal type information
 *     signal               : NHLOS specific notification signal & mask information
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_register_listener(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SIGEX_TYPE servreg_signal_type, SERVREG_SIGEX servreg_signal)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   /* Initialized as SERVREG_SERVICE_STATE_UNINIT */
   uint32_t curr_remote_state = 0x7FFFFFFF;
   int32_t remote_transaction_id = 0;
   RCESN_HANDLE rcesn_h = NULL;
   servreg_relationship_type relation = RELATIONSHIP_NOT_ESTABLISHED;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   SERVREG_TRACE_2(COMMON_REG, sr_mon_node, qurt_thread_get_id());

   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("sr_mon_handle is null/has invalid signature");
      ret = SERVREG_INVALID_HANDLE;
      goto common_return;
   }

   ret = servreg_calculate_relationship(sr_mon_node->service_name, &relation);
   if (SERVREG_SUCCESS != ret)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("Not able to establish relationship");
      goto common_return;
   }

   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
   
   if(servreg_signal_type == SERVREG_SIGEX_TYPE_SIGQURT)
      rcesn_h = SERVREG_RCESN_REGISTER_HANDLE_QURT(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);
   else
      rcesn_h = SERVREG_RCESN_REGISTER_HANDLE_REX(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);

   if (RCESN_NULL == rcesn_h)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("sr_mon_node->rcesn_handle not created");
      ret = SERVREG_FAILURE;
      goto unlock_return;
   }

   sr_mon_node->rcesn_handle = rcesn_h; 
   sr_mon_node->listener_ref_count = sr_mon_node->listener_ref_count + 1;

   /* Classifying services into following categories:
      1) Local
      2) Remote
      3) User (Local to subsystem) (Applicable only for root PD )
   */
   switch(relation)
   {
      /* Services which are in same PD are considered local for that PD clients*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      {
         SERVREG_TRACE_2(REG_LOCAL,relation,sr_mon_node);
         TMS_MSG_HIGH("Client registered for local service");
         ret = SERVREG_SUCCESS;
         goto unlock_return;
      }
      /* Services which are in Root PD  are considered remote services for User PD clients.
         Services which are in different subsystem are considered remote services for User PD/Root PD clients.*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT: /*QDI call to root PD */
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT: /*QMI call to other subsystem*/
      {
         SERVREG_TRACE_2(REG_REMOTE,relation,sr_mon_node);

         /* remote service is automatically treated as synchronous service */
         sr_mon_node->servreg_sync = SERVREG_TRUE;

         /* If there is already a registered remote listener then curr state and transaction id will already have
            the state of the remote service, else it will be updated via the QMI call */
         curr_remote_state = (uint32_t)rcesn_getstatecurr_handle(sr_mon_node->rcesn_handle);
         remote_transaction_id = sr_mon_node->transaction_id;

         /* via QMI and/or QDI */
         ret = servreg_register_remote_listener(sr_mon_node->service_name, sr_mon_handle, &curr_remote_state, &remote_transaction_id);
         if(SERVREG_SUCCESS == ret)
            goto reg_success;
         else
            goto reg_failure;
      }
      /* Services which are in User PD are considered user services for Root PD clients.*/
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS: /*QDI call to user PD*/
      {
         SERVREG_TRACE_2(REG_USER,relation,sr_mon_node);

         /* remote service is automatically treated as synchronous service */
         sr_mon_node->servreg_sync = SERVREG_TRUE;

         /* we want to make only one proxy listener in root PD for one user pd service*/
         if(sr_mon_node->listener_ref_count <= 1)
         {
            SERVREG_TRACE_0(REG_USER_IF);
            ret = servreg_register_user_listener(sr_mon_node->service_name, sr_mon_handle, &curr_remote_state, &remote_transaction_id);
            if(SERVREG_SUCCESS == ret)
               goto reg_success;
            else
               goto reg_failure;
         }
         else /* proxy listener already exist in root PD for this service*/
         {
            SERVREG_TRACE_0(SUCCESS);
            TMS_MSG_HIGH("Client registered for user service");
            ret = SERVREG_SUCCESS;
            goto unlock_return;
         }
      }
      default :
      {
         SERVREG_TRACE_0(FAIL);
         TMS_MSG_ERROR_1("Relationship %d is not expected",relation);
         ret = SERVREG_FAILURE;
         goto unlock_return;
      }
   }

reg_success:
   SERVREG_TRACE_0(SUCCESS);
   /* Update the current remote state and remote transaction_id */
   if(rcesn_setstatecurr_handle(sr_mon_node->rcesn_handle, curr_remote_state)){;}
   sr_mon_node->transaction_id = remote_transaction_id;
   TMS_MSG_HIGH("Client registered for remote/user service");
   goto unlock_return;

reg_failure:
   SERVREG_TRACE_0(FAIL);

   if(servreg_signal_type == SERVREG_SIGEX_TYPE_SIGQURT)
      rcesn_h = SERVREG_RCESN_UNREGISTER_HANDLE_QURT(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);
   else
      rcesn_h = SERVREG_RCESN_UNREGISTER_HANDLE_REX(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);

   if (RCESN_NULL != rcesn_h)
      sr_mon_node->rcesn_handle = rcesn_h;
   
   sr_mon_node->listener_ref_count = sr_mon_node->listener_ref_count - 1;

   if(sr_mon_node->listener_ref_count != 0)
   {
     SERVREG_TRACE_2(COMMON_REG_FAIL, sr_mon_node, sr_mon_node->listener_ref_count);
     TMS_MSG_ERROR_1("listener_ref_count NON ZERO %d", sr_mon_node->listener_ref_count);
   }
   TMS_MSG_ERROR_1("Client registration failed for remote/user service ret: %d", ret);

unlock_return:
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
   
common_return:
   SERVREG_TRACE_2(COMMON_REG_EXIT, sr_mon_node, qurt_thread_get_id());
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_deregister_listener
 *
 * Description:
 *     De-Register a listener to get event notifications for a service
 *
 * Parameters:
 *     sr_mon_handle        : Handle to an existing service state which is mapped by domain + service 
 *                            or just domain name
 *     servreg_signal_type  : NHLOS specific notification signal type information
 *     signal               : NHLOS specific notification signal & mask information
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 *
 * Note : 
 *     If clients are no longer interested in the service AND they have de-registered their listeners
 *     using servreg_deregister_listener(), they can free the handle by calling servreg_free_monitor_handle().
 * =====================================================================  */
SERVREG_RESULT servreg_deregister_listener(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SIGEX_TYPE servreg_signal_type, SERVREG_SIGEX servreg_signal)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   RCESN_HANDLE rcesn_h = NULL;
   servreg_relationship_type relation = RELATIONSHIP_NOT_ESTABLISHED;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   SERVREG_TRACE_2(COMMON_DEREG, sr_mon_node, qurt_thread_get_id());

   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("sr_mon_handle is null/has invalid signature");
      ret = SERVREG_INVALID_HANDLE;
      goto common_return;
   }

   ret = servreg_calculate_relationship(sr_mon_node->service_name, &relation);
   if (SERVREG_SUCCESS != ret)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("Not able to establish relationship");
      goto common_return;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   if(servreg_signal_type == SERVREG_SIGEX_TYPE_SIGQURT)
      rcesn_h = SERVREG_RCESN_UNREGISTER_HANDLE_QURT(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);
   else
      rcesn_h = SERVREG_RCESN_UNREGISTER_HANDLE_REX(sr_mon_node->rcesn_handle, (RCESN_FNSIG_COMPARE)rcesn_compare_ne_prev, servreg_signal);

   if (RCESN_NULL == rcesn_h)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("sr_mon_node->rcesn_handle not created");
      ret = SERVREG_FAILURE;
      goto unlock_return;
   }

   sr_mon_node->rcesn_handle = rcesn_h;
   if(0 == sr_mon_node->listener_ref_count)
   {
      SERVREG_TRACE_0(FAIL);
      TMS_MSG_ERROR("No listeners registered to de-register them");
      ret = SERVREG_FAILURE;
      goto unlock_return;
   }

   sr_mon_node->listener_ref_count = sr_mon_node->listener_ref_count - 1;

   if(0 != sr_mon_node->listener_ref_count)
   {
      SERVREG_TRACE_0(SUCCESS);
      TMS_MSG_HIGH("Client deregistered successfully for remote/user/local service");
      ret = SERVREG_SUCCESS;
      goto unlock_return;
   }

   /*listener_ref_count is 0 so All clients have de-registered for this service so now deregister in remote/user entities also */


   /* Classifying services into following categories:
      1) Local
      2) Remote
      3) User (Local to subsystem) (Applicable only for root PD )
   */
   switch(relation)
   {
      /* Services which are in same PD are considered local for that PD clients*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
      {
         SERVREG_TRACE_2(DEREG_LOCAL,relation,sr_mon_node);
         TMS_MSG_HIGH("Last client deregistered for local service");
         ret = SERVREG_SUCCESS;
         goto unlock_return;
      }
      /* Services which are in Root PD  are considered remote services for User PD clients.
         Services which are in different subsystem are considered remote services for User PD/Root PD clients.*/
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS: /*QDI call to root PD */
      case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT: /*QDI call to root PD */
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT: /*QMI call to other subsystem*/
      {
         SERVREG_TRACE_2(DEREG_REMOTE,relation,sr_mon_node);

         /* remote service was automatically treated as synchronous service during registration so now make it asynchronous */
         sr_mon_node->servreg_sync = SERVREG_FALSE;

         ret = servreg_deregister_remote_listener(sr_mon_node->service_name, sr_mon_handle);
         if(SERVREG_SUCCESS == ret)
            goto dereg_success;
         else
            goto dereg_failure;
      }
      /* Services which are in User PD are considered user services for Root PD clients.*/
      case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS: /*QDI call to user PD*/
      {
         SERVREG_TRACE_2(DEREG_USER,relation,sr_mon_node);

         /* remote service was automatically treated as synchronous service during registration so now make it asynchronous */
         sr_mon_node->servreg_sync = SERVREG_FALSE;

         ret = servreg_deregister_user_listener(sr_mon_node->service_name, sr_mon_handle);
         if(SERVREG_SUCCESS == ret)
            goto dereg_success;
         else
            goto dereg_failure;
      }
      default :
      {
         SERVREG_TRACE_0(FAIL);
         TMS_MSG_ERROR_1("Relationship %d is not expected",relation);
         ret = SERVREG_FAILURE;
         goto unlock_return;
      }
   }

dereg_success:
   SERVREG_TRACE_0(SUCCESS);
   TMS_MSG_HIGH("Last client deregistered for remote/user service");
   goto unlock_return;

dereg_failure:
   SERVREG_TRACE_0(FAIL);
   TMS_MSG_ERROR_1("Last client deregistration failed for remote/user service ret: %d", ret);

unlock_return:
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

common_return:
   SERVREG_TRACE_2(COMMON_DEREG_EXIT, sr_mon_node, qurt_thread_get_id());
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_listener_ref_count
 *
 * Description:
 *     Get the listener reference count for the given montior handle
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Listener ref count
 * =====================================================================  */
uint32_t servreg_get_listener_ref_count(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   uint32_t ret = 0;
   
   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      ERR_FATAL("sr_mon_node NULL/invalid signature", 0, 0, 0);
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      
   ret = sr_mon_node->listener_ref_count;
   //TMS_MSG_HIGH_1("Ref count: %d", ret);
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_service_curr_state
 *
 * Description:
 *     Get the current state of a service that the listener is interested in
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     SERVREG_SERVICE_STATE : Returns the state of the service. 
 *                             Refer to the enum SERVREG_SERVICE_STATE for list of states
 * =====================================================================  */
SERVREG_SERVICE_STATE servreg_get_service_curr_state(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_SERVICE_STATE ret = SERVREG_SERVICE_STATE_UNINIT;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
      
   ret = (SERVREG_SERVICE_STATE)rcesn_getstatecurr_handle(sr_mon_node->rcesn_handle);
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_transaction_id_no_locks
 *
 * Description:
 *     Get the current transaction_id. But not taking mutex.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Current transaction if for that service
 * =====================================================================  */
int32_t servreg_get_transaction_id_no_locks(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   int32_t ret = 0;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      ERR_FATAL("sr_mon_node NULL/invalid signature", 0, 0, 0);
      return ret;
   }

   ret = sr_mon_node->transaction_id;
   //TMS_MSG_HIGH_1("transaction_id = %d\n ", ret);

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_transaction_id
 *
 * Description:
 *     Get the current transaction_id.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Current transaction if for that service
 * =====================================================================  */
int32_t servreg_get_transaction_id(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   int32_t ret = 0;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      ERR_FATAL("sr_mon_node NULL/invalid signature", 0, 0, 0);
   }

   qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   ret = sr_mon_node->transaction_id;
   //TMS_MSG_HIGH_1("transaction_id = %d\n ", ret);

   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_e_d_transaction_id
 *
 * Description:
 *     Get the current Early down transaction_id.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Current Early down transaction id  for that service
 * =====================================================================  */
int32_t servreg_get_e_d_transaction_id(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   int32_t ret = 0;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      ERR_FATAL("sr_mon_node NULL/invalid signature", 0, 0, 0);
   }

   qurt_pimutex_lock(&(sr_mon_node->node_mutex));
   ret = sr_mon_node->e_d_transaction_id;
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_set_ack
 *
 * Description:
 *     Function increases the ack count by one for that service
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_ack(SERVREG_MON_HANDLE sr_mon_handle, int32_t transaction_id)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   servreg_mon_queue_p sr_q_entry = SERVREG_NULL;
   servreg_relationship_type relation = RELATIONSHIP_NOT_ESTABLISHED;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

   if(SERVREG_NULL != sr_mon_node)
   {
      if(SERVREG_MON_SIGNATURE == sr_mon_node->mon_signature)
      {
         ret = servreg_calculate_relationship(sr_mon_node->service_name, &relation);
         qurt_pimutex_lock(&(sr_mon_node->node_mutex));
         if(SERVREG_TRUE == sr_mon_node->servreg_sync && transaction_id == sr_mon_node->transaction_id)
         {
            sr_q_entry = (servreg_mon_queue_p)q_check(sr_mon_node->state_q);

            if(SERVREG_NULL != sr_q_entry)
            {
               sr_q_entry->ack_pending_count = sr_q_entry->ack_pending_count - 1;
               SERVREG_TRACE_2(COMMON_ACK, sr_mon_handle, sr_q_entry->ack_pending_count);

               if(sr_q_entry->ack_pending_count == 0)
               {
                  if(SERVREG_SERVICE_STATE_UP == sr_q_entry->curr_state && RCECB_NULL != sr_mon_node->rcecb_up_handle)
                  {
                     SERVREG_TRACE_2(COMMON_ACK_ALL, sr_mon_handle, SERVREG_SERVICE_STATE_UP);

                     /* Callback function called only after all the ACK's are received */
                     if(rcecb_signal_handle(sr_mon_node->rcecb_up_handle)){;}
                  }
                  else if(SERVREG_SERVICE_STATE_DOWN == sr_q_entry->curr_state && RCECB_NULL != sr_mon_node->rcecb_down_handle)
                  {
                     SERVREG_TRACE_2(COMMON_ACK_ALL, sr_mon_handle, SERVREG_SERVICE_STATE_DOWN);
                     /* Callback function called only after all the ACK's are received */
                     if(rcecb_signal_handle(sr_mon_node->rcecb_down_handle)){;}
                  }

                  if (SERVREG_SUCCESS == ret)
                  {
                     /* Classifying services into following categories:
                        1) Local
                        2) Remote
                        3) User (Local to subsystem) (Applicable only for root PD )
                     */
                     switch(relation)
                     {
                        /* Services which are in same PD are considered local for that PD clients*/
                        case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
                        case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_SAME_PROCESS:
                        {
                            SERVREG_TRACE_2(COMMON_ACK_LOCAL, sr_mon_node, sr_mon_node->transaction_id);
                            ret = SERVREG_SUCCESS;
                            break;
                        }
                        /* Services which are in Root PD  are considered remote services for User PD clients.
                           Services which are in different subsystem are considered remote services for User PD/Root PD clients.*/
                        case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_OUR_PARENT: /*QDI call to root PD */
                        case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_IN_A_SIBLING_PROCESS: /*QDI call to root PD */
                        case WE_ARE_A_CHILD_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERNT_PARENT: /*QDI call to root PD */
                        case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_OF_A_DIFFERENT_PARENT: /*QMI call to other subsystem*/
                        {
                            SERVREG_TRACE_2(COMMON_ACK_REMOTE, sr_mon_node, sr_mon_node->transaction_id);
                            /* via QMI and/or QDI */
                            ret = servreg_set_remote_ack(sr_mon_node->service_name, sr_mon_handle, sr_q_entry->curr_state, sr_mon_node->transaction_id);
                            if(SERVREG_SUCCESS != ret)
                               TMS_MSG_ERROR_2("Set Ack failed for remote service ret: %d, handle: 0x%x", ret,sr_mon_handle);
                            break;
                        }
                        /* Services which are in User PD are considered user services for Root PD clients.*/
                        case WE_ARE_A_PARENT_PROCESS_AND_THIS_IS_A_SERVICE_IN_CHILD_PROCESS: /*QDI call to user PD*/
                        {
                            SERVREG_TRACE_2(COMMON_ACK_USER, sr_mon_node, sr_mon_node->transaction_id);
                            ret = servreg_set_user_ack(sr_mon_node->service_name, sr_mon_handle, sr_q_entry->curr_state, sr_mon_node->transaction_id);
                            if(SERVREG_SUCCESS != ret)
                               TMS_MSG_ERROR_2("Set Ack failed for user service ret: %d, handle: 0x%x", ret,sr_mon_handle);
                            break;
                        }
                        default :
                        {
                           SERVREG_TRACE_0(FAIL);
                           TMS_MSG_ERROR_1("Relationship %d is not expected",relation);
                           ret = SERVREG_FAILURE;
                           break;
                        }
                     }
                  }
                  else
                  {
                     SERVREG_TRACE_0(FAIL);
                     TMS_MSG_ERROR("Not able to establish relationship");
                  }

                  /* Remove from the Queue when all the local ACK's are received */
                  sr_q_entry = (servreg_mon_queue_p)q_get(sr_mon_node->state_q);
                  free(sr_q_entry);

                  /* Process the next entry in the queue */
                  servreg_service_queue(sr_mon_handle);
               }
               else
               {
                  ret = SERVREG_SUCCESS;
                  TMS_MSG_HIGH_1("One ack set, pending acks: %d", sr_q_entry->ack_pending_count);
               }
            }
            else
            {
               if(transaction_id >= sr_mon_node->e_d_transaction_id /* "=" is for handling normal EDOWN ACK.
                  ">" is for Quick state change (If EDOWN ACK is given after EDOWN->DOWN state change so transaction_id
                   will be e_d_transaction_id+1)*/
                  && SR_INITIAL_TRANSACTION_ID != sr_mon_node->e_d_transaction_id
                    /*explanation is in servreg_create_mon_node() for it*/)
               {
                  TMS_MSG_HIGH("Ack for Early down indication , Ignoring");
                  SERVREG_TRACE_2(ED_ACK, sr_mon_node, transaction_id);
                  ret = SERVREG_SUCCESS;
                  /* If Multiple ACK will be given for one indication then also execution may come into this block.
                     Earlier in this situation , Failure was returned but now Success will be returned.*/
               }
               else if(SERVREG_MAX_TXN_ID == sr_mon_node->e_d_transaction_id)
               {
                  TMS_MSG_HIGH("Ack for Early down indication(wrap), Ignoring ");
                  SERVREG_TRACE_2(ED_ACK2, sr_mon_node, transaction_id);
                  ret = SERVREG_SUCCESS;
               }
               else
               {
                  TMS_MSG_ERROR("Sending ACK before state change notification sent out");
                  //ERR_FATAL("SERVREG_MON: in servreg_set_ack() sending ACK before state change notification sent out ", 0, 0, 0);
                  ret = SERVREG_FAILURE;
               }
            }
         }
         else
         {
            if (transaction_id != sr_mon_node->transaction_id)
            {
               TMS_MSG_ERROR("Cannot set ACK: given transaction_id invalid");
            }
            
            ret = SERVREG_SUCCESS;
         }
         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      }
      else
      {
         TMS_MSG_ERROR("sr_mon_handle has invalid signature");
         //ERR_FATAL("SERVREG_MON: in servreg_set_ack() sr_mon_handle has invalid signature ", 0, 0, 0);
         ret = SERVREG_INVALID_HANDLE;
      }
   }
   else
   {
      TMS_MSG_ERROR("sr_mon_node NULL");
      //ERR_FATAL("ERVREG_MON: in servreg_set_ack() sr_mon_node is NULL ", 0, 0, 0);
      ret = SERVREG_INVALID_HANDLE;
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_register_sync_notif_cb
 *
 * Description:
 *     Register with a callback function to get sync notification for that service state
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : The service state for which the client wants to get the sync notification
 *                     Should preferably be SERVREG_SERVICE_STATE_DOWN or SERVREG_SERVICE_STATE_UP
 *     callback      : Callback function registered
 *     cb_p1         : Callback function parameter. Can be set to sr_mon_handle
 *     cb_p2         : Callback function parameter 2. Can be set to the service state.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_register_sync_notif_cb(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state, SERVREG_FNSIG_CALLBACK const callback, SERVREG_CB_PARAM cb_p1, SERVREG_CB_PARAM cb_p2)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_NAME sync_name = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

   if(SERVREG_NULL != sr_mon_node && SERVREG_NULL != callback)
   {
      if(SERVREG_MON_SIGNATURE == sr_mon_node->mon_signature)
      {
         qurt_pimutex_lock(&(sr_mon_node->node_mutex));
         sync_name = servreg_create_sync_name(sr_mon_node->service_name, state);

         if(SERVREG_NULL != sync_name)
         {
            if(SERVREG_SERVICE_STATE_UP == state)
            {
               /* Should return an existing handle if event already created, else crate a new one */
               sr_mon_node->rcecb_up_handle = rcecb_create_name(sync_name);

               if(RCECB_NULL != sr_mon_node->rcecb_up_handle)
               {
                  sr_mon_node->rcecb_up_handle = rcecb_register_parm2_handle(sr_mon_node->rcecb_up_handle, (RCECB_CALLBACK_FNSIG_P2)callback, (RCECB_PARM)cb_p1, (RCECB_PARM)cb_p2);
                  TMS_MSG_HIGH("Client registered for up sync notification");
                  ret = SERVREG_SUCCESS;
               }
               else
               {
                  ERR_FATAL("In servreg_register_sync_notif_cb() rcecb_up_handle NULL", 0, 0, 0);
               }
            }
            else if(SERVREG_SERVICE_STATE_DOWN == state)
            {
               /* Should return an existing handle if event already created, else crate a new one */
               sr_mon_node->rcecb_down_handle = rcecb_create_name(sync_name);

               if(RCECB_NULL != sr_mon_node->rcecb_down_handle)
               {
                  sr_mon_node->rcecb_down_handle = rcecb_register_parm2_handle(sr_mon_node->rcecb_down_handle, (RCECB_CALLBACK_FNSIG_P2)callback, (RCECB_PARM)cb_p1, (RCECB_PARM)cb_p2);
                  TMS_MSG_HIGH("Client registered for down sync notification");
                  ret = SERVREG_SUCCESS;
               }
               else
               {
                  ERR_FATAL("In servreg_register_sync_notif_cb() rcecb_down_handle NULL", 0, 0, 0);
               }
            }
            else
            {
               TMS_MSG_ERROR("Invalid state, should be UP/DOWN");
               ret = SERVREG_FAILURE;
            }

            /* Free the space allocated for sync_name as rcecb internally does a strlcpy of the name */
            free(sync_name);
         }
         else
         {
            TMS_MSG_ERROR("servreg_create_sync_name() ret NULL");
            ret = SERVREG_FAILURE;
         }

         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      }
      else
      {
         TMS_MSG_ERROR("sr_mon_handle has invalid signature");
         ret = SERVREG_INVALID_HANDLE;
      }
   }
   else
   {
      TMS_MSG_ERROR("sr_mon_node/callback NULL");
      ret = SERVREG_INVALID_HANDLE;
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_deregister_sync_notif_cb
 *
 * Description:
 *     De-Register to get sync notification for that service and de-register the callback function
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *     state         : The service state for which the client wants to dereigster to get sync notification
 *                     Should preferably be SERVREG_SERVICE_STATE_DOWN or SERVREG_SERVICE_STATE_UP
 *     callback      : callback function de-registered
 *     cb_p1         : Callback function parameter. Can be set to sr_mon_handle
 *     cb_p2         : Callback function parameter 2. Can be set to the service state.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_deregister_sync_notif_cb(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE state, SERVREG_FNSIG_CALLBACK const callback, SERVREG_CB_PARAM cb_p1, SERVREG_CB_PARAM cb_p2)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);

   if(SERVREG_NULL != sr_mon_node && SERVREG_NULL != callback)
   {
      if(SERVREG_MON_SIGNATURE == sr_mon_node->mon_signature)
      {
         qurt_pimutex_lock(&(sr_mon_node->node_mutex));
         if(SERVREG_SERVICE_STATE_UP == state && RCECB_NULL != sr_mon_node->rcecb_up_handle)
         {
            sr_mon_node->rcecb_up_handle = rcecb_unregister_parm2_handle(sr_mon_node->rcecb_up_handle, (RCECB_CALLBACK_FNSIG_P2)callback, (RCECB_PARM)cb_p1, (RCECB_PARM)cb_p2);
            TMS_MSG_HIGH("Client un-registered for up sync notification");
            ret = SERVREG_SUCCESS;
         }
         else if(SERVREG_SERVICE_STATE_DOWN == state && RCECB_NULL != sr_mon_node->rcecb_down_handle)
         {
            sr_mon_node->rcecb_down_handle = rcecb_unregister_parm2_handle(sr_mon_node->rcecb_down_handle, (RCECB_CALLBACK_FNSIG_P2)callback, (RCECB_PARM)cb_p1, (RCECB_PARM)cb_p2);
            TMS_MSG_HIGH("Client un-registered for down sync notification");
            ret = SERVREG_SUCCESS;
         }
         else
         {
            TMS_MSG_ERROR("Invalid state/client not registerd");
            ret = SERVREG_FAILURE;
         }
         qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
      }
      else
      {
         TMS_MSG_ERROR("sr_mon_handle has invalid signature");
         ret = SERVREG_INVALID_HANDLE;
      }
   }
   else
   {
      TMS_MSG_ERROR("sr_mon_node/callback NULL");
      ret = SERVREG_FAILURE;
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_pd_name
 *
 * Description:
 *     This function returns the pd name i.e format "soc/domain/subdomain".
 *     Memory is allocated for the string returned.
 *
 * Parameters:
 *     name : "soc/domain/subdomain/provider/service" or just "soc/domain/subdomain" info
 *
 * Returns:
 *     	pd name of the format "soc/domain/subdomain"
 * =====================================================================  */
SERVREG_NAME servreg_get_pd_name(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_mon_node_p sr_mon_node = SERVREG_NULL;
   SERVREG_NAME ret = SERVREG_NULL;

   sr_mon_node = sr_mon_handle2sr_mon_node(sr_mon_handle);
   if(SERVREG_NULL == sr_mon_node || SERVREG_MON_SIGNATURE != sr_mon_node->mon_signature)
   {
      TMS_MSG_ERROR("sr_mon_node NULL/invalid signature");
      return ret;
   }
   
   qurt_pimutex_lock(&(sr_mon_node->node_mutex));

   ret = servreg_utils_get_string(sr_mon_node->service_name,SOC+DOMAIN+SUBDOMAIN);
   
   qurt_pimutex_unlock(&(sr_mon_node->node_mutex));
   
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_monitor_init
 *
 * Description:
 *     Initialization function for Service Registry Monitor feature.
 *     Service Registry will register itself as a service and sets its state
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_monitor_init(void)
{
   SERVREG_NAME domain = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL, sr_service_mon_handle = SERVREG_NULL;

   qurt_pimutex_init(&(servreg_mon_internal.mutex));
   qurt_pimutex_lock(&(servreg_mon_internal.mutex));
   secure_memset(&servreg_mon_internal.servreg_mon_node_hashtab, 0, sizeof(servreg_mon_internal.servreg_mon_node_hashtab));
   qurt_pimutex_unlock(&(servreg_mon_internal.mutex));

   domain = servreg_get_local_domain();

   if(SERVREG_NULL != domain)
   {
      /* Register the state of the PD as UP with Service Registry */
      sr_mon_handle = servreg_alloc_monitor_handle(domain, SERVREG_NULL);

      if(SERVREG_NULL != sr_mon_handle)
      {
         /* Always set the pd service itself as synchronous */
         if(SERVREG_SUCCESS != servreg_set_synchronous(sr_mon_handle))
         {
            ERR_FATAL("In servreg_monitor_init() servreg_set_synchronous() failed", 0, 0, 0);
         }

         if(SERVREG_SUCCESS != servreg_set_state(sr_mon_handle, SERVREG_SERVICE_STATE_UP))
         {
            ERR_FATAL("In servreg_monitor_init() servreg_set_state() failed", 0, 0, 0);
         }
      }
      else
      {
         ERR_FATAL("In servreg_monitor_init() sr_mon_handle NULL", 0, 0, 0);
      }

      /* Register the servreg service itself as UP */
      sr_service_mon_handle = servreg_alloc_monitor_handle(domain, SERVREG_SERVICE_NAME);

      if(SERVREG_NULL != sr_service_mon_handle)
      {
         if(SERVREG_SUCCESS != servreg_set_state(sr_service_mon_handle, SERVREG_SERVICE_STATE_UP))
         {
            ERR_FATAL("In servreg_monitor_init() servreg_set_state() failed", 0, 0, 0);
         }
      }
      else
      {
         ERR_FATAL("In servreg_monitor_init() sr_service_mon_handle NULL", 0, 0, 0);
      }
   }

   /* Set timer to check for the ACK state as deferrable timer to avoid unnecessary wakeup of Q6 */
   timer_group_set_deferrable(&servreg_mon_internal.servreg_group, TRUE);

   if(TE_SUCCESS != timer_def_osal(&servreg_mon_internal.servreg_mon_ack_timer,
                                  &servreg_mon_internal.servreg_group,
                                  TIMER_FUNC1_CB_TYPE,
                                  servreg_generic_task_cb, NULL))
   {
       ERR_FATAL("In servreg_monitor_init() Timer def failed", 0, 0, 0);
   }

   if(TE_SUCCESS != timer_set_64(&servreg_mon_internal.servreg_mon_ack_timer,
                                SERVREG_MON_ACK_TIME_CHK_SEC,
                                0,
                                T_SEC))
   {
       ERR_FATAL("In servreg_monitor_init() Timer set failed", 0, 0, 0);
   }

   return;
}

