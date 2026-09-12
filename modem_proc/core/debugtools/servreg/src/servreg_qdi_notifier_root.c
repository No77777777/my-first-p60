/*
#============================================================================
#  Name:
#    servreg_qdi_notifier_root.c
#
#  Description:
#    Service Registry notifier file for root image. This module serves as the end-point
#    of communication via glink and qdi
#
# Copyright (c) 2015-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdarg.h"
#include "stdlib.h"
#include <stringl/stringl.h>
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "err.h"
#include "tms_utils_msg.h"
#include "tms_utils.h"

#include "servreg_internal.h"
#include "servreg_locator.h"
#include "servreg_localdb.h"
#include "servreg_utils.h"
#include "servreg_monitor.h"
#include "servreg_monitor_qurt.h"
#include "servreg_notifier.h"
#include "servreg_qdi_notifier_root.h"
#include "servreg_qdi.h"         /* Include for root PD only */
#include "servreg_qmi_notifier_client.h"
#include "servreg_qmi_notifier_server.h"

/* Service Registry Notifier ROOT node structure. List of SERVREG_MON_HANDLE'S*/
struct servreg_qdi_notif_node_s
{
   uint32_t notif_signature;
   SERVREG_MON_HANDLE sr_mon_handle;
   SERVREG_SERVICE_STATE curr_state;
   int32_t transaction_id;
   SERVREG_BOOL ack_set;
   struct servreg_qdi_notif_node_s* next;
};
typedef struct servreg_qdi_notif_node_s servreg_qdi_notif_node_t, * servreg_qdi_notif_node_p;

/* Type casts as accessor functions */
#define sr_qdi_notif_node2sr_qdi_notif_handle(x)        ((SERVREG_QDI_NOTIF_HANDLE)x)
#define sr_qdi_notif_handle2sr_qdi_notif_node(x)        ((servreg_qdi_notif_node_p)x)

struct servreg_qdi_notif_node_internal_s
{
   qurt_mutex_t mutex;
};
struct servreg_qdi_notif_node_internal_s servreg_qdi_notif_node_internal;

/* Static functions in this file */
static servreg_qdi_notif_node_p servreg_qdi_notif_node_alloc(void);


/** =====================================================================
 * Function:
 *     servreg_qdi_notif_node_alloc
 *
 * Description:
 *     Allocates QDI notif node
 *
 * Parameters:
 *     None
 *
 * Returns:
 *    Dynamically allocated QDI notif node
 * =====================================================================  */
static servreg_qdi_notif_node_p servreg_qdi_notif_node_alloc(void)
{
   servreg_qdi_notif_node_p new_node = SERVREG_NULL;

   new_node = (servreg_qdi_notif_node_p)calloc(sizeof(servreg_qdi_notif_node_t), 1);

   if (SERVREG_NULL != new_node)
   {
      new_node->notif_signature = SERVREG_QDI_NOTIF_SIGNATURE;
      new_node->curr_state = SERVREG_SERVICE_STATE_UNINIT;
   }
   else
   {
      ERR_FATAL( "servreg_qdi_notif_node_p calloc failed", 0, 0, 0);
   }

   return new_node;
}

/** =====================================================================
 * Function:
 *     servreg_get_qdi_notif_node
 *
 * Description:
 *     Checks if a SRM node already exists with the given name. If it does 
 *     exists it returns a pointer to that srm node.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by just domain name.
 *     servreg_notif_pd_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_node(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_QDI_NOTIF_HANDLE * servreg_notif_pd_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));

   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_pd_list_head);

   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      if(sr_mon_handle == sr_qdi_notif_node->sr_mon_handle)
      {
         sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
         break;
      }
      else
      {
         sr_qdi_notif_node = sr_qdi_notif_node->next;
      }
   }

   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));

   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_create_qdi_notif_node
 *
 * Description:
 *     Creates a srnotif node with the given sr_mon_handle
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by just domain name.
 *     servreg_notif_pd_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_create_qdi_notif_node(SERVREG_MON_HANDLE sr_mon_handle, SERVREG_QDI_NOTIF_HANDLE * servreg_notif_pd_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;  
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   /* Check if the srnotif node exists */
   sr_qdi_notif_handle = servreg_get_qdi_notif_node(sr_mon_handle, servreg_notif_pd_list_head);

   if(SERVREG_NULL == sr_qdi_notif_handle)
   {
      sr_qdi_notif_node = servreg_qdi_notif_node_alloc();

      if(SERVREG_NULL != sr_qdi_notif_node)
      {
         /*  Insert to head of list */
         sr_qdi_notif_node->next = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_pd_list_head);

         /* Update head */
         *servreg_notif_pd_list_head = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);

         sr_qdi_notif_node->sr_mon_handle = sr_mon_handle;
         sr_qdi_notif_node->curr_state = servreg_get_service_curr_state(sr_mon_handle);
         sr_qdi_notif_node->transaction_id = servreg_get_transaction_id(sr_mon_handle);
         sr_qdi_notif_node->ack_set = SERVREG_FALSE;
         sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
      }
      else
      {
         ERR_FATAL( "SERVREG_NOTIF: sr_qdi_notif_node calloc failed", 0, 0, 0);
      }
   }

   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_delete_qdi_notif_node
 *
 * Description:
 *     Deletes a sr notif node given the sr notif handle
 *     servreg_notif_pd_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Parameters:
 *     sr_qdi_notif_handle  : Handle to the notifier node to be deleted
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_delete_qdi_notif_node(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle, SERVREG_QDI_NOTIF_HANDLE *  servreg_notif_pd_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL, sr_notif_prev = SERVREG_NULL, sr_qdi_notif_node_del = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));

   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_pd_list_head);
   sr_qdi_notif_node_del = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);

   if(SERVREG_NULL == sr_qdi_notif_node_del)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle NULL");
      ret = SERVREG_INVALID_HANDLE;
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node_del->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      ret = SERVREG_INVALID_HANDLE;
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      if(sr_qdi_notif_node == sr_qdi_notif_node_del)
      {
         if(SERVREG_NULL == sr_notif_prev)
         {
            *servreg_notif_pd_list_head = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node->next);
         }
         else
         {
            sr_notif_prev->next = sr_qdi_notif_node->next;
         }

         sr_qdi_notif_node->next = SERVREG_NULL;
         free(sr_qdi_notif_node);
         sr_qdi_notif_node = SERVREG_NULL;
         ret = SERVREG_SUCCESS;
         break;
      }
      else
      {
         sr_notif_prev = sr_qdi_notif_node;
         sr_qdi_notif_node = sr_qdi_notif_node->next;  
      }
   } /* while() */

   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_delete_qdi_notif_list
 *
 * Description:
 *     Deletes a sr notif node given the sr notif head pointer and also de-registers and de-allocates the listeners attached
 *
 * Parameters:
 *     servreg_notif_list_head : Head pointer of the notifier handle list for the user PD.
 *     signal                  : qurt signal associated to that pd
 *     mask                   : qurt mask associated to that pd
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_delete_qdi_notif_list(SERVREG_QDI_NOTIF_HANDLE * servreg_notif_list_head, qurt_anysignal_t* signal, uint32_t mask)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL, sr_notif_next = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_SUCCESS;

   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_list_head);

   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
      sr_notif_next = sr_qdi_notif_node->next;
      sr_mon_handle = sr_qdi_notif_node->sr_mon_handle;
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
         
      ret = servreg_deregister_listener_qurt(sr_mon_handle, signal, mask);

      if(SERVREG_SUCCESS != ret)
      {
         TMS_MSG_ERROR("Deregister listener failed");
      }

      ret = servreg_free_monitor_handle(sr_mon_handle);

      if(SERVREG_SUCCESS != ret)
      {
         TMS_MSG_ERROR("Could not free monitor handle");
      }

      free(sr_qdi_notif_node);
      sr_qdi_notif_node = sr_notif_next;
   } /* while() */

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_qdi_notif_node_state_change
 *
 * Description:
 *     Check which notif node state has changed and return that notif node's handle
 *
 * Parameters:
 *     servreg_notif_pd_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_node_state_change(SERVREG_QDI_NOTIF_HANDLE * servreg_notif_pd_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_SERVICE_STATE new_curr_state;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;

   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_pd_list_head);

   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
      sr_mon_handle = sr_qdi_notif_node->sr_mon_handle;
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));    
      
      new_curr_state = servreg_get_service_curr_state(sr_mon_handle);
      if(sr_qdi_notif_node->curr_state != new_curr_state)
      {
         /* Return the notif node whose state has changed */
         sr_qdi_notif_node->curr_state = new_curr_state;
         sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
         /* exit while() loop */
         break; 
      }
      sr_qdi_notif_node = sr_qdi_notif_node->next;
   }
   
   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_get_qdi_notif_node_ack_set
 *
 * Description:
 *     Check which notif node whose ack was set
 *
 * Parameters:
 *     servreg_notif_register_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_node_ack_set(SERVREG_QDI_NOTIF_HANDLE * servreg_notif_register_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));

   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_register_list_head);

   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      if(sr_qdi_notif_node->ack_set == SERVREG_TRUE)
      {
         /* After finding the node, set the ack to false to process the next ack */
         sr_qdi_notif_node->ack_set = SERVREG_FALSE;

         sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
         /* exit while() loop */
         break; 
      }
      sr_qdi_notif_node = sr_qdi_notif_node->next;
   }
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));

   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_get_qdi_notif_register_node
 *
 * Description:
 *     Given the pointer to the head pointer return the notif node
 *
 * Parameters:
 *     servreg_notif_register_list_head : Head pointer of the notifier handle list for the user PD.
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_register_node(SERVREG_QDI_NOTIF_HANDLE * servreg_notif_register_list_head)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(*servreg_notif_register_list_head);

   if(SERVREG_NULL != sr_qdi_notif_node)
   {
      sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
   }
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_notif_get_curr_state
 *
 * Description:
 *     Get the state of the notifier node
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    Check enum SERVREG_SERVICE_STATE for different possible service states
 * =====================================================================  */
SERVREG_SERVICE_STATE servreg_notif_get_curr_state(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_SERVICE_STATE ret = SERVREG_SERVICE_STATE_UNINIT;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   ret = sr_qdi_notif_node->curr_state;
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notif_get_mon_handle
 *
 * Description:
 *     Get the monitor handle of the notifier node
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    Check enum SERVREG_SERVICE_STATE for different possible service states
 * =====================================================================  */
SERVREG_MON_HANDLE servreg_notif_get_mon_handle(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_MON_HANDLE ret = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   ret = sr_qdi_notif_node->sr_mon_handle;
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notif_get_transaction_id
 *
 * Description:
 *     Get the transaction id of the notifier node
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    transaction id Or SR_INVALID_TRANSACTION_ID (-2) in case of failure
 * =====================================================================  */
int32_t servreg_notif_get_transaction_id(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   int32_t ret = SR_INVALID_TRANSACTION_ID; 

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   ret = sr_qdi_notif_node->transaction_id;
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notif_update_handle
 *
 * Description:
 *     Update the notif handle with the state and transaction id from the user pd
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    Check enum SERVREG_SERVICE_STATE for different possible service states
 * =====================================================================  */
SERVREG_RESULT servreg_notif_update_handle(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle, SERVREG_SERVICE_STATE state, int32_t transaction_id)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   SERVREG_BOOL is_valid = SERVREG_FALSE;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }

   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }

   sr_qdi_notif_node->curr_state = state;
   sr_qdi_notif_node->transaction_id = transaction_id;
   sr_mon_handle = sr_qdi_notif_node->sr_mon_handle;

   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   /* Below calls will attempt to acquire another mutex, release the one held */
   
   /* Update the actual state of the user service in root */
   if(0 != transaction_id && SERVREG_SUCCESS == servreg_is_valid_state_change(sr_mon_handle, state, &is_valid) && SERVREG_TRUE == is_valid)
   {
      SERVREG_TRACE_2(QDI_UPDATE_STATE, sr_mon_handle, state);
      SERVREG_TRACE_2(QDI_UPDATE_TXID, sr_mon_handle, transaction_id);
      servreg_set_transaction_id(sr_mon_handle, (transaction_id - 1));
      servreg_set_state(sr_mon_handle, state);
   }
   else
   {
      SERVREG_TRACE_2(QDI_UPDATE_TXID, sr_mon_handle, transaction_id);
      servreg_set_transaction_id(sr_mon_handle, transaction_id);
   }

   ret = SERVREG_SUCCESS;

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notif_set_user_ack
 *
 * Description:
 *     Set the ack for the notif handle
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    Check enum SERVREG_SERVICE_STATE for different possible service states
 * =====================================================================  */
SERVREG_RESULT servreg_notif_set_user_ack(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   sr_qdi_notif_node->ack_set = SERVREG_TRUE;
   SERVREG_TRACE_2(SET_USER_ACK, sr_qdi_notif_node, sr_qdi_notif_node->ack_set);
   ret = SERVREG_SUCCESS;
   
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_get_qdi_notif_next_node
 *
 * Description:
 *     Get the next notifier node
 *
 * Parameters:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the sr notif node
 *
 * Returns:
 *    Check enum SERVREG_SERVICE_STATE for different possible service states
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_next_node(SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{  
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_QDI_NOTIF_HANDLE ret = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_notif_node_internal.mutex));
   sr_qdi_notif_node = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);
   if(SERVREG_NULL == sr_qdi_notif_node)
   {
      TMS_MSG_ERROR("sr_qdi_notif_node NULL");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   if(SERVREG_QDI_NOTIF_SIGNATURE != sr_qdi_notif_node->notif_signature)
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
      qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
      return ret;
   }
   
   ret = sr_qdi_notif_node->next;
   qurt_pimutex_unlock(&(servreg_qdi_notif_node_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notifier_init
 *
 * Description:
 *     Initialization function for the service registry notifier module
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_notifier_init(void)
{
   qurt_pimutex_init(&(servreg_qdi_notif_node_internal.mutex));
   servreg_qmi_notif_server_init();
   //servreg_qmi_notif_client_init();
   return;
}
