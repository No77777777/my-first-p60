/*
#============================================================================
#  Name:
#    servreg_qdi_notifier_user.c
#
#  Description:
#    Service Registry notifier file for user image. This module serves as the end-point
#    of communication via qdi.
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
#include "servreg_notifier.h"
#include "servreg_qdi_notifier_user.h"
#include "servreg_qdi_client.h"    /* Include for user PD only */

#include "qurt_printf.h"

#define SERVREG_NOTIF_SIGNAL_MASK           0x1

/* Service Registry Notifier node structure. List of SERVREG_QDI_NOTIF_HANDLE'S */
struct servreg_qdi_notif_node_s
{
   uint32_t notif_signature;
   SERVREG_MON_HANDLE sr_mon_handle;
   SERVREG_REMOTE_HANDLE sr_remote_handle;
   SERVREG_SERVICE_STATE curr_state;
   struct servreg_qdi_notif_node_s* next;
};
typedef struct servreg_qdi_notif_node_s servreg_qdi_notif_node_t, * servreg_qdi_notif_node_p;

extern int32_t servnotif_qdi_client_handle;

/* Internal QDI User Stucture */
struct servreg_qdi_user_internal_s
{
   int32_t servreg_qdi_client_handle;

   /* User registrants in root */
   servreg_qdi_notif_node_p servreg_qdi_notif_user_list_head;      /* Head node of the servreg notification list */

   /* Root registrants in user*/
   servreg_qdi_notif_node_p servreg_qdi_notif_register_list_head;  /* Head node of the servreg registration list */
   qurt_anysignal_t servreg_notif_signal;
   qurt_mutex_t mutex;
};
struct servreg_qdi_user_internal_s servreg_qdi_user_internal;

/* Type casts as accessor functions */
#define sr_qdi_notif_node2sr_qdi_notif_handle(x)        ((SERVREG_QDI_NOTIF_HANDLE)x)
#define sr_qdi_notif_handle2sr_qdi_notif_node(x)        ((servreg_qdi_notif_node_p)x)

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
 *     Checks if a notifier node already exists with the sr monitor handle. If it does 
 *     exists it returns a pointer to that notif node.
 *
 * Parameters:
 *     sr_type       : root client registing in the user pd or user client registering in the root pd
 *     sr_mon_handle : Handle to an existing service state which is mapped by domain + service 
 *                     or just domain name
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the notifier node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_get_qdi_notif_node(SERVREG_QDI_TYPE sr_type, SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   if(sr_type == SERVREG_USER_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_user_list_head;
   }
   else if(sr_type == SERVREG_ROOT_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_register_list_head;
   }

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

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));

   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_create_qdi_notif_node
 *
 * Description:
 *     Creates a sr notif node with the given sr_mon_handle and sr_remote_handle
 *
 * Parameters:
 *     sr_type          : root client registing in the user pd or user client registering in the root pd
 *     sr_mon_handle    : Handle to an existing service state which is mapped by domain + service 
 *                        or just domain name
 *     sr_remote_handle : Remote (root PD) notifier node address
 *
 * Returns:
 *    SERVREG_QDI_NOTIF_HANDLE : handle to the notifier node
 * =====================================================================  */
SERVREG_QDI_NOTIF_HANDLE servreg_create_qdi_notif_node(SERVREG_QDI_TYPE sr_type, SERVREG_MON_HANDLE sr_mon_handle, SERVREG_REMOTE_HANDLE sr_remote_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;  
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));
   /* Check if the srnotif node exists */
   sr_qdi_notif_handle = servreg_get_qdi_notif_node(sr_type, sr_mon_handle);

   if(SERVREG_NULL == sr_qdi_notif_handle)
   {
     sr_qdi_notif_node = servreg_qdi_notif_node_alloc();

     if(SERVREG_NULL != sr_qdi_notif_node)
     {
        if(sr_type == SERVREG_USER_REGISTER)
        {
           /*  Insert to head of list */
           sr_qdi_notif_node->next = servreg_qdi_user_internal.servreg_qdi_notif_user_list_head;
           /* Update head */
           servreg_qdi_user_internal.servreg_qdi_notif_user_list_head = sr_qdi_notif_node;
        }
        else if(sr_type == SERVREG_ROOT_REGISTER)
        {
           /*  Insert to head of list */
           sr_qdi_notif_node->next = servreg_qdi_user_internal.servreg_qdi_notif_register_list_head;
           /* Update head */
           servreg_qdi_user_internal.servreg_qdi_notif_register_list_head = sr_qdi_notif_node;
        }

        sr_qdi_notif_node->sr_mon_handle = sr_mon_handle;
        sr_qdi_notif_node->sr_remote_handle = sr_remote_handle;
        sr_qdi_notif_node->curr_state = servreg_get_service_curr_state(sr_mon_handle);
        SERVREG_TRACE_2(SET_QDI_NODE_STATE, sr_mon_handle, sr_qdi_notif_node->curr_state);
        sr_qdi_notif_handle = sr_qdi_notif_node2sr_qdi_notif_handle(sr_qdi_notif_node);
     }
     else
     {
        TMS_MSG_ERROR("sr_qdi_notif_node alloc failed");
     }
   }

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
   return sr_qdi_notif_handle;
}

/** =====================================================================
 * Function:
 *     servreg_delete_qdi_notif_node
 *
 * Description:
 *     Deletes a sr notif node given the sr notif handle
 *
 * Parameters:
 *     sr_type              : root client registing in the user pd or user client registering in the root pd
 *     sr_qdi_notif_handle  : Handle to the notifier node to be deleted
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_delete_qdi_notif_node(SERVREG_QDI_TYPE sr_type, SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle)
{
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL, sr_notif_prev = SERVREG_NULL, sr_qdi_notif_node_del = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   if(sr_type == SERVREG_USER_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_user_list_head;
   }
   else if(sr_type == SERVREG_ROOT_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_register_list_head;
   }

   sr_qdi_notif_node_del = sr_qdi_notif_handle2sr_qdi_notif_node(sr_qdi_notif_handle);

   if(SERVREG_NULL != sr_qdi_notif_node_del)
   {
      if(SERVREG_QDI_NOTIF_SIGNATURE == sr_qdi_notif_node_del->notif_signature)
      {
         while(SERVREG_NULL != sr_qdi_notif_node)
         {
            if(sr_qdi_notif_node == sr_qdi_notif_node_del)
            {
               if(SERVREG_NULL == sr_notif_prev)
               {
                  if(sr_type == SERVREG_USER_REGISTER)
                  {
                     servreg_qdi_user_internal.servreg_qdi_notif_user_list_head = sr_qdi_notif_node->next;
                  }
                  else if(sr_type == SERVREG_ROOT_REGISTER)
                  {
                     servreg_qdi_user_internal.servreg_qdi_notif_register_list_head = sr_qdi_notif_node->next;
                  }
               }
               else
               {
                  sr_notif_prev->next = sr_qdi_notif_node->next;
               }

               sr_qdi_notif_node->next = SERVREG_NULL;

               /* Reclaim back the notif node memory*/
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
      }
      else
      {
         TMS_MSG_ERROR("sr_qdi_notif_handle has invalid signature");
         ret = SERVREG_INVALID_HANDLE;
      }
   }
   else
   {
      TMS_MSG_ERROR("sr_qdi_notif_handle NULL");
      ret = SERVREG_INVALID_HANDLE;
   }

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_map_remote_handle
 *
 * Description:
 *     Given the remote notif handle i.e root pd notif node address, return
 *     the user pd's corresponding monitor handle
 *
 * Parameters:
 *     sr_type          : root client registing in the user pd or user client registering in the root pd
 *     sr_remote_handle : Remote (root PD) notifier node address
 *
 * Returns:
 *     SERVREG_MON_HANDLE :  Handle to an existing service state which is mapped by domain + service 
 *                           or just domain name
 * =====================================================================  */
SERVREG_MON_HANDLE servreg_map_remote_handle(SERVREG_QDI_TYPE sr_type, SERVREG_REMOTE_HANDLE sr_remote_handle)
{
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   if(sr_type == SERVREG_USER_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_user_list_head;
   }
   else if(sr_type == SERVREG_ROOT_REGISTER)
   {
      sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_register_list_head;
   }

   while(SERVREG_NULL != sr_qdi_notif_node)
   {
      if(sr_remote_handle == sr_qdi_notif_node->sr_remote_handle)
      {
         sr_mon_handle = sr_qdi_notif_node->sr_mon_handle;
         break;
      }
      else
      {
         sr_qdi_notif_node = sr_qdi_notif_node->next;
      }
   }

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));

   return sr_mon_handle;
}

/** =====================================================================
 * Task:
 *     servreg_notif_worker_task
 *
 * Description:
 *     This is the service register notifier worker task which keeps track of state
 *     changes in the root PD's notifier nodes and reports back to the user
 *     PD notifier nodes
 *     This task also waits for any root pd clients that want to register witht for
 *     user pd service
 *
 * Parameters:
 *     None
 * 
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_notif_worker_task(void *argv DLL_API_UNUSED)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_REMOTE_HANDLE sr_remote_handle;
   uint32_t new_curr_state;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   int32_t transaction_id = 0;
   uint32_t mask = 0;
   SERVREG_NAME sr_name = SERVREG_NULL;
   SERVREG_BOOL is_valid = SERVREG_FALSE;

   do
   {
      /* Blocking wait within qdi driver */
      ret = servreg_qdi_invoke_worker_wait(&mask);

      if (SERVREG_SUCCESS == ret)
      {
         if(mask & SERVREG_QDI_REG_USER_MASK_VALUE)
         {
            servreg_qdi_invoke_get_remote_handle(&sr_remote_handle, &new_curr_state, &transaction_id);
            sr_mon_handle = servreg_map_remote_handle(SERVREG_USER_REGISTER, sr_remote_handle);
            if(SERVREG_NULL != sr_mon_handle)
            {
               /* Get the transaction_id and decrease the value by one as servreg_set_state increases it by one */
               if(SERVREG_SUCCESS == servreg_is_valid_state_change(sr_mon_handle, new_curr_state, &is_valid) && SERVREG_TRUE == is_valid)
               {
                  transaction_id = transaction_id - 1;
                  servreg_set_transaction_id(sr_mon_handle, transaction_id);
                  servreg_set_state(sr_mon_handle, new_curr_state);
               }
               else
               {
                  servreg_set_transaction_id(sr_mon_handle, transaction_id);
               }
            }
         }

         if(mask & SERVREG_QDI_REG_ROOT_MASK_VALUE)
         {
            /* Get the root client info that wants to register in user pd*/
            sr_name = (SERVREG_NAME)calloc(sizeof(char), QDI_SERVREG_NAME_LENGTH_V01);
            if ( sr_name == SERVREG_NULL )
            {
              TMS_MSG_ERROR("Calloc failed");
              continue;
            }
            servreg_qdi_invoke_register_remote_handle(&sr_remote_handle, sr_name);
            sr_mon_handle = servreg_alloc_monitor_handle(sr_name, SERVREG_NULL);
            if(SERVREG_NULL != sr_mon_handle)
            {
               servreg_register_listener_qurt(sr_mon_handle, &servreg_qdi_user_internal.servreg_notif_signal, SERVREG_NOTIF_SIGNAL_MASK);
               if(SERVREG_NULL == servreg_create_qdi_notif_node(SERVREG_ROOT_REGISTER, sr_mon_handle, sr_remote_handle))
               {
                  TMS_MSG_ERROR("Could not register root client in user pd");
               }
               new_curr_state = servreg_get_service_curr_state(sr_mon_handle);
               transaction_id = servreg_get_transaction_id(sr_mon_handle);
               servreg_qdi_invoke_update_remote_handle(new_curr_state, transaction_id, sr_remote_handle);
            }
            free(sr_name);
         }

         if(mask & SERVREG_QDI_DEREG_ROOT_MASK_VALUE)
         {
            /* Get the root client info that wants to de-register in user pd*/
            sr_name = (SERVREG_NAME)calloc(sizeof(char), QDI_SERVREG_NAME_LENGTH_V01);
            if ( sr_name == SERVREG_NULL )
            {
              TMS_MSG_ERROR("Calloc failed");
              continue;
            }
            servreg_qdi_invoke_deregister_remote_handle(sr_name);
            sr_mon_handle = servreg_get_sr_mon_handle(sr_name);
            if(SERVREG_NULL != sr_mon_handle)
            {
               servreg_deregister_listener_qurt(sr_mon_handle, &servreg_qdi_user_internal.servreg_notif_signal, SERVREG_NOTIF_SIGNAL_MASK);
               if(SERVREG_SUCCESS != servreg_delete_qdi_notif_node(SERVREG_ROOT_REGISTER, servreg_get_qdi_notif_node(SERVREG_ROOT_REGISTER,sr_mon_handle)))
                  TMS_MSG_ERROR("Could not delete root client in user pd");
            }
            free(sr_name);
         }

         if(mask & SERVREG_QDI_FREE_MON_HANDLE_MASK_VALUE)
         {
            /* Get the root client info that wants to free monitor handle in user pd*/
            sr_name = (SERVREG_NAME)calloc(sizeof(char), QDI_SERVREG_NAME_LENGTH_V01);
            if ( sr_name == SERVREG_NULL )
            {
              TMS_MSG_ERROR("Calloc failed");
              continue;
            }
            servreg_qdi_invoke_free_monitor_handle(sr_name);
            sr_mon_handle = servreg_get_sr_mon_handle(sr_name);
            if(SERVREG_NULL != sr_mon_handle)
            {
               if(SERVREG_SUCCESS != servreg_free_monitor_handle(sr_mon_handle))
                  TMS_MSG_ERROR("Could not free monitor handle in user pd");
            }
            free(sr_name);
         }

         if(mask & SERVREG_QDI_ACK_ROOT_MASK_VALUE)
         {
            /* Get the root client info that wants to ack in user pd*/
            sr_name = (SERVREG_NAME)calloc(sizeof(char), QDI_SERVREG_NAME_LENGTH_V01);
            if(SERVREG_NULL != sr_name)
            {
               servreg_qdi_invoke_get_ack_handle(sr_name, &transaction_id);
               sr_mon_handle = servreg_get_sr_mon_handle(sr_name);
               if(SERVREG_NULL != sr_mon_handle)
               {
                  servreg_set_ack(sr_mon_handle, transaction_id);
                  qurt_printf("\n servreg_notif_worker_task(): received ack from root \n"); //TODO: Why is this here?
               }
               free(sr_name);
            }
         }
      }
   } while (1);
}

/** =====================================================================
 * Task:
 *     servreg_notif_signal_task
 *
 * Description:
 *     This is the service register signal task which keeps track of state
 *     changes in the user PD's notifier nodes and reports back to the root
 *     PD notifier nodes
 *
 * Parameters:
 *     None
 * 
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_notif_signal_task(void *argv DLL_API_UNUSED)
{
   uint32_t q_mask = 0;
   servreg_qdi_notif_node_p sr_qdi_notif_node = SERVREG_NULL;
   SERVREG_SERVICE_STATE new_curr_state = SERVREG_SERVICE_STATE_UNINIT;

   /* Task forever loop */
   for (;;)
   {
      q_mask = qurt_anysignal_wait(&servreg_qdi_user_internal.servreg_notif_signal, SERVREG_NOTIF_SIGNAL_MASK);

      if(q_mask & SERVREG_NOTIF_SIGNAL_MASK)
      {
         qurt_anysignal_clear(&servreg_qdi_user_internal.servreg_notif_signal, SERVREG_NOTIF_SIGNAL_MASK);

         TMS_MSG_HIGH("State change notif received");

         qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

         sr_qdi_notif_node = servreg_qdi_user_internal.servreg_qdi_notif_register_list_head;

         while(SERVREG_NULL != sr_qdi_notif_node)
         {
            new_curr_state = servreg_get_service_curr_state(sr_qdi_notif_node->sr_mon_handle);

            if(sr_qdi_notif_node->curr_state != new_curr_state)
            {
               SERVREG_TRACE_2(QDI_SET_STATE, sr_qdi_notif_node->sr_mon_handle, new_curr_state);
               sr_qdi_notif_node->curr_state = new_curr_state;
               servreg_qdi_invoke_set_state(sr_qdi_notif_node->sr_remote_handle, new_curr_state);
            }
            else
               SERVREG_TRACE_2(QDI_SET_STATE_E, sr_qdi_notif_node->sr_mon_handle, new_curr_state);

            sr_qdi_notif_node = sr_qdi_notif_node->next;
         }

         qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
      }
   }

}

/** =====================================================================
 * Function:
 *     servreg_register_root_listeners
 *
 * Description:
 *     Register the root clients for the user service if any
 *
 * Parameters:
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
void servreg_register_root_listeners()
{
   SERVREG_REMOTE_HANDLE sr_remote_handle = 0;
   uint32_t new_curr_state = SERVREG_SERVICE_STATE_UNINIT;
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   int32_t transaction_id = 0;
   SERVREG_NAME sr_name = SERVREG_NULL;

   qurt_anysignal_init(&servreg_qdi_user_internal.servreg_notif_signal);

   while(1)
   {
      /* Get the root client info that wants to register in user pd*/
      sr_remote_handle = 0;
      sr_name = (SERVREG_NAME)calloc(sizeof(char), QDI_SERVREG_NAME_LENGTH_V01);
      if(SERVREG_NULL == sr_name)
      {
         return;
      }
      servreg_qdi_invoke_register_root_handles(&sr_remote_handle, sr_name);
      if(sr_remote_handle != 0)
      {
         sr_mon_handle = servreg_alloc_monitor_handle(sr_name, SERVREG_NULL);
         if(SERVREG_NULL != sr_mon_handle)
         {
            servreg_register_listener_qurt(sr_mon_handle, &servreg_qdi_user_internal.servreg_notif_signal, SERVREG_NOTIF_SIGNAL_MASK);
            if(SERVREG_NULL == servreg_create_qdi_notif_node(SERVREG_ROOT_REGISTER, sr_mon_handle, sr_remote_handle))
            {
               TMS_MSG_ERROR("Could not register root client in user PD");
            }
            new_curr_state = servreg_get_service_curr_state(sr_mon_handle);
            transaction_id = servreg_get_transaction_id(sr_mon_handle);
            servreg_qdi_invoke_update_remote_handle(new_curr_state, transaction_id, sr_remote_handle);
         }
         free(sr_name);
      }
      else
      {
         free(sr_name);
         break;
      }
   }
}

/** =====================================================================
 * Function:
 *     servreg_register_remote_listener
 *
 * Description:
 *     Register client as a remote listener because the service is not a local service.
 *     QDI is used as the communication medium for registering the listener
 *     with the remote service registry framework.
 *
 *     For every service, remote registration is done only once and one proxy listener in 
 *     the root PD will be registered that represents all the listeners in the user PD
 *     for that service.
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_register_remote_listener(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, uint32_t * sr_curr_remote_state, int32_t * sr_remote_transaction_id)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_REMOTE_HANDLE sr_remote_handle = 0;
   uint32_t  sr_remote_state = 0x7fffffff;
   int32_t  sr_transaction_id = 0;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   /* Check if the srnotif node exists */
   sr_qdi_notif_handle = servreg_get_qdi_notif_node(SERVREG_USER_REGISTER, sr_mon_handle);

   /* Create a new notif node and register a new proxy listener in root-pd only if the srnotif node does not exist */
   if(SERVREG_NULL == sr_qdi_notif_handle)
   {
      qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

      ret = servreg_qdi_invoke_register_remote_listener(service_name, &sr_remote_handle, &sr_remote_state, &sr_transaction_id);
      if(ret == SERVREG_SUCCESS)
      {
         /* Create a Service Notifier node to list the sr_mon_handle to get remote notifications from root pd */
         sr_qdi_notif_handle = servreg_create_qdi_notif_node(SERVREG_USER_REGISTER, sr_mon_handle, sr_remote_handle);
         if(SERVREG_NULL != sr_qdi_notif_handle)
         {
            /* Get the initial state of the remote handle and set that state in the local handle also */
            *sr_curr_remote_state = sr_remote_state;
            *sr_remote_transaction_id = sr_transaction_id;
         }
         else
         {
            TMS_MSG_ERROR("Could not register as a remote listener in the notifier pool");
            ret = SERVREG_FAILURE;
         }
      }
      else
      {
         TMS_MSG_ERROR("QDI register for remote listener failed");
      }

      qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
   }
   else
   {
      TMS_MSG_HIGH("Proxy client already registered via QDI");
      ret = SERVREG_SUCCESS;
   }

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_deregister_remote_listener
 *
 * Description:
 *     De-register the remote proxy listener only if there are no more local listeners 
 *     for that service.
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_deregister_remote_listener(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   /* Check if the srnotif node exists */
   sr_qdi_notif_handle = servreg_get_qdi_notif_node(SERVREG_USER_REGISTER, sr_mon_handle);

   /* Delete the notif node and de-register the proxy listener*/
   if(SERVREG_NULL != sr_qdi_notif_handle)
   {
      ret = servreg_qdi_invoke_deregister_remote_listener(service_name);
      if(SERVREG_SUCCESS == ret)
         ret = servreg_delete_qdi_notif_node(SERVREG_USER_REGISTER,sr_qdi_notif_handle);
   }

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_free_remote_handle
 *
 * Description:
 *     Free the handle created when clients register as a remote listener. 
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_free_remote_handle(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_QDI_NOTIF_HANDLE sr_qdi_notif_handle = SERVREG_NULL;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));
   sr_qdi_notif_handle = servreg_get_qdi_notif_node(SERVREG_USER_REGISTER, sr_mon_handle);
   if(SERVREG_NULL != sr_qdi_notif_handle)
   {
      ret = servreg_qdi_invoke_free_remote_handle(service_name);
   }

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_set_remote_ack
 *
 * Description:
 *     Set the remote ack count for the remote proxy listener via QDI only if 
 *     all the local acks have been received
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *     curr_state         : state for which the ACK will be set. Not used in local service registry
 *                          cause the next state is not updated until all the ACK's for the current
 *                          state have received.
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_remote_ack(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE curr_state, int32_t sr_transaction_id)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   ret = servreg_qdi_invoke_set_remote_ack(service_name, sr_transaction_id);

   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));

   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_create_qmi_table_entry
 *
 * Description:
 *     Stores the qmi_instance_id value for that domain_name
 *
 * Parameters:
 *     domain_name : string which has the "soc/domain/subdomain" info
 *     qmi_instance_id : instance id used to establish a qmi connection for remote service updates
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_create_qmi_table_entry(SERVREG_NAME domain_name, uint32 qmi_instance_id)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&(servreg_qdi_user_internal.mutex));

   ret = servreg_qdi_invoke_create_qmi_entry(domain_name, qmi_instance_id);

   if(SERVREG_SUCCESS != ret)
   {
      TMS_MSG_ERROR("Failed to create QMI table entry");
   }
   
   qurt_pimutex_unlock(&(servreg_qdi_user_internal.mutex));
}

/** =====================================================================
 * Function:
 *     servreg_qmi_notif_server_init
 *
 * Description:
 *     Initialization function for the service registry QMI notifier module
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void servreg_qmi_notif_server_init(void)
{
   return;
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
   /* Client QDI driver should be initialized at this point */
   servreg_qdi_user_internal.servreg_qdi_client_handle = servnotif_qdi_client_handle;
   servreg_qdi_user_internal.servreg_qdi_notif_user_list_head = SERVREG_NULL;

   qurt_pimutex_init(&(servreg_qdi_user_internal.mutex));

   /* Launch worker task */
   (void)tms_utils_launch_qurt_task(SERVREG_NOTIF_WORKER_TASK_NAME, TMS_TASK_STACK_2K, TMS_TASK_PRIO_DEFAULT, servreg_notif_worker_task, NULL, TRUE);

   servreg_register_root_listeners();

   /* Launch signal task */
   (void)tms_utils_launch_qurt_task(SERVREG_NOTIF_SIGNAL_TASK_NAME, TMS_TASK_STACK_2K, TMS_TASK_PRIO_DEFAULT, servreg_notif_signal_task, NULL, TRUE);
}

