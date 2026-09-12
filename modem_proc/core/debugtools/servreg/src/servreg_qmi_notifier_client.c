/*
#============================================================================
#  Name:
#    servreg_qmi_notifier_client.c
#
#  Description:
#    Service Registry notifier file for root image. This module serves as the end-point
#    of communication via QMI.
#
# Copyright (c) 2015-2019 by Qualcomm Technologies, Inc.  All Rights Reserved.
#============================================================================
*/
#include "stdarg.h"
#include "stdlib.h"
#include <stringl/stringl.h>
#include "comdef.h"             /* Definitions for byte, word, etc.     */
#include "err.h"
#include "tms_utils_msg.h"
#include "tms_utils.h"
#include "qmi_client.h"
#include "qmi_cci_target_ext.h"
#include "rcinit.h"

#include "service_registry_notifier_v01.h"
#include "servreg_internal.h"
#include "servreg_locator.h"
#include "servreg_localdb.h"
#include "servreg_utils.h"
#include "servreg_monitor.h"
#include "servreg_notifier.h"
#include "servreg_qmi_notifier_client.h"
#include "gen_cb_ctxt.h"

#define SERVREG_QMI_NOTIF_CLIENT_REQ_TIMEOUT_MS           1000 
#define SERVREG_NOTIF_EVT_SYNC                            0x00000001        /* Indicates service down syncronization is complete */

#define SERVREG_QMI_REMOTE_ACK_REQ_SIG                    0x0002
#define SERVREG_QMI_REMOTE_QMI_IND_SIG                    0x0004
#define SERVREG_QMI_REMOTE_AVAILABLE_SIG                  0x0008

typedef enum
{
   SERVREG_NOTIF_CLIENT_REGISTER_LISTENER_REQ      = QMI_SERVREG_NOTIF_REGISTER_LISTENER_REQ_V01,
   SERVREG_NOTIF_CLIENT_STATE_UPDATED_IND_ACK_REQ  = QMI_SERVREG_NOTIF_STATE_UPDATED_IND_ACK_REQ_V01,
}servreg_notif_client_msg_req_type;

/* Flag indicating if the QMI client initialization is done */
static SERVREG_SERVICE_STATE servreg_qmi_notifier_client_init_status = SERVREG_SERVICE_STATE_UNINIT;

/* Service Registry QMI Remote Request Queue entries */
q_type servreg_remote_ack_req_q, servreg_remote_qmi_ind_q, servreg_remote_service_q;
qurt_anysignal_t servreg_remote_op_signal;

typedef struct servreg_remote_ack_req_s
{
   /* Link for the next queue */
   q_link_type link;

   SERVREG_NAME service_name;

   SERVREG_MON_HANDLE sr_mon_handle;

   int32_t transaction_id;

} servreg_remote_ack_req_t, *servreg_remote_ack_req_p;

typedef struct servreg_remote_qmi_ind_s
{
   /* Link for the next queue */
   q_link_type link;

   qmi_servreg_notif_state_updated_ind_msg_v01 rcvd_ind_qmi_msg;

} servreg_remote_qmi_ind_t, *servreg_remote_qmi_ind_p;

typedef struct servreg_remote_service_s
{
   /* Link for the next queue */
   q_link_type link;
  
   SERVREG_QMI_NOTIF_CLIENT_HANDLE qmi_notif_handle;
   
   SERVREG_QMI_CONNECT_TYPE connection_type;
   
   char service_name[QMI_SERVREG_NOTIF_NAME_LENGTH_V01];
   
} servreg_remote_service_t, *servreg_remote_service_p;

/* QMI message related and keeping it global because it should be valid for the lifetime of the user_handle */
static qmi_idl_service_object_type servreg_notif_service_object;
static qmi_client_type servreg_qmi_client_notifier;
static qmi_client_os_params os_param;

/* Service Registry Notifier QMI entry table */
struct servreg_qmi_entry_table_s
{
   SERVREG_NAME domain_name;

   uint32_t qmi_instance_id;

   qmi_service_info servreg_notif_server_info;

   SERVREG_QMI_CONNECTION_STATE connection_state;

   qmi_client_type servreg_qmi_notifier_client_handle;

   struct servreg_qmi_entry_table_s* next;
};
typedef struct servreg_qmi_entry_table_s servreg_qmi_entry_table_t, * servreg_qmi_entry_table_p;

/* Service Registry Notifier ROOT node structure. List of SERVREG_MON_HANDLE'S*/
struct servreg_qmi_notif_client_node_s
{
   uint32_t notif_signature;

   SERVREG_MON_HANDLE sr_mon_handle;

   qurt_anysignal_t sync_sig;
   
   uint32_t ssr_down_event; 
   
   struct servreg_qmi_notif_client_node_s* next;
};
typedef struct servreg_qmi_notif_client_node_s servreg_qmi_notif_client_node_t, * servreg_qmi_notif_client_node_p;

/* Internal QMI Client Stucture */
struct servreg_qmi_client_internal_s
{
   qurt_mutex_t                    mutex;                               /* Generic mutex for list protection */
   servreg_qmi_entry_table_p servreg_qmi_table_head;                    /* Head node for qmi table linked list */
   servreg_qmi_notif_client_node_p servreg_qmi_notif_client_list_head;  /* Head node of the servreg notification list */
};
struct servreg_qmi_client_internal_s servreg_qmi_client_internal;

/* Type casts as accessor functions */
#define sr_qmi_notif_client_node2sr_qmi_notif_client_handle(x)        ((SERVREG_QMI_NOTIF_CLIENT_HANDLE)x)
#define sr_qmi_notif_client_handle2sr_qmi_notif_client_node(x)        ((servreg_qmi_notif_client_node_p)x)

/* Static functions in this file */
static SERVREG_RESULT servreg_qmi_connect_to_server(servreg_qmi_entry_table_p sr_qmi_entry);
static void servreg_ssr_down_event_cb(uint32_t sr_qmi_notif_handle_addr);
static SERVREG_RESULT servreg_connect_remote_listener(SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle, uint32_t* sr_curr_remote_state, SERVREG_QMI_CONNECT_TYPE connection_type);
static void servreg_ssr_up_event_cb(uint32_t sr_qmi_notif_handle_addr);
static void servreg_register_ssr_event(SERVREG_NAME service_name, SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle);
static void servreg_deregister_ssr_event(SERVREG_NAME service_name, SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle);
static servreg_qmi_entry_table_p servreg_get_qmi_table_entry(SERVREG_NAME domain_name);
static SERVREG_RESULT servreg_remote_queue_action(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, int32_t sr_transaction_id);
static SERVREG_RESULT sr_ind_process(qmi_servreg_notif_state_updated_ind_msg_v01* indication);

/** =====================================================================
 * Function:
 *     servreg_qmi_notif_client_node_alloc
 *
 * Description:
 *     Gives the first available free and allocated space from the memory
 *
 * Parameters:
 *     None
 *
 * Returns:
 *    servreg_qmi_notif_client_node_p : the first available free and allocated space from the memory
 * =====================================================================  */
static servreg_qmi_notif_client_node_p servreg_qmi_notif_client_node_alloc(void)
{
   servreg_qmi_notif_client_node_p new_node = SERVREG_NULL;
   new_node = (servreg_qmi_notif_client_node_p)calloc(sizeof(servreg_qmi_notif_client_node_t),1);

   if (SERVREG_NULL != new_node)
   {
      new_node->notif_signature = SERVREG_QMI_NOTIF_CLIENT_SIGNATURE;
   }
   else
   {
      ERR_FATAL( "servreg_qmi_notif_client_node_p calloc failed", 0, 0, 0);
   }

   return new_node;
}

/** =====================================================================
 * Function:
 *     servreg_get_qmi_notif_client_node
 *
 * Description:
 *     Checks if a node already exists with the given name. If it does 
 *     exists it returns a pointer to that node.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by just domain name.
 *
 * Returns:
 *    SERVREG_QMI_NOTIF_CLIENT_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QMI_NOTIF_CLIENT_HANDLE servreg_get_qmi_notif_client_node(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;

   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
   
   sr_qmi_notif_client_node = servreg_qmi_client_internal.servreg_qmi_notif_client_list_head;

   while(SERVREG_NULL != sr_qmi_notif_client_node)
   {
      if(sr_mon_handle == sr_qmi_notif_client_node->sr_mon_handle)
      {
         sr_qmi_notif_client_handle = sr_qmi_notif_client_node2sr_qmi_notif_client_handle(sr_qmi_notif_client_node);
         break;
      }
      else
      {
         sr_qmi_notif_client_node = sr_qmi_notif_client_node->next;
      }
   }

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);

   return sr_qmi_notif_client_handle;
}

/** =====================================================================
 * Function:
 *     servreg_create_qmi_notif_client_node
 *
 * Description:
 *     Creates a srnotif node with the given sr_mon_handle
 *     This function does not check if the srnotif node exists in order 
 *     to avoid duplication of effort. Caller must perform the check.
 *
 * Parameters:
 *     sr_mon_handle : Handle to an existing service state which is mapped by just domain name.
 *
 * Returns:
 *    SERVREG_QMI_NOTIF_CLIENT_HANDLE : handle to the sr notif node
 * =====================================================================  */
SERVREG_QMI_NOTIF_CLIENT_HANDLE servreg_create_qmi_notif_client_node(SERVREG_MON_HANDLE sr_mon_handle)
{
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;  
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;

   sr_qmi_notif_client_node = servreg_qmi_notif_client_node_alloc(); 
   if(SERVREG_NULL == sr_qmi_notif_client_node)
   {
      ERR_FATAL("sr_qmi_notif_client_node alloc failed: %x", sr_mon_handle, 0, 0);
   }
   
   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
 
   /*  Insert to head of list */
   sr_qmi_notif_client_node->next = servreg_qmi_client_internal.servreg_qmi_notif_client_list_head;

   /* Update head */
   servreg_qmi_client_internal.servreg_qmi_notif_client_list_head = sr_qmi_notif_client_node;

   sr_qmi_notif_client_node->sr_mon_handle = sr_mon_handle;

   /* Initialize sync signal */
   qurt_anysignal_init(&(sr_qmi_notif_client_node->sync_sig));

   sr_qmi_notif_client_handle = sr_qmi_notif_client_node2sr_qmi_notif_client_handle(sr_qmi_notif_client_node);

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
    
   return sr_qmi_notif_client_handle;
}

/** =====================================================================
 * Function:
 *     servreg_delete_qmi_notif_client_node
 *
 * Description:
 *     Deletes a sr notif node given the sr notif handle
 *
 * Parameters:
 *     sr_qmi_notif_client_handle  : Handle to the notifier node to be deleted
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_delete_qmi_notif_client_node(SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle)
{
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL, sr_notif_prev = SERVREG_NULL, sr_qmi_notif_client_node_del = SERVREG_NULL;
   servreg_remote_service_p sr_remote_service_entry = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;

   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);

   sr_qmi_notif_client_node = servreg_qmi_client_internal.servreg_qmi_notif_client_list_head;
   sr_qmi_notif_client_node_del = sr_qmi_notif_client_handle2sr_qmi_notif_client_node(sr_qmi_notif_client_handle);
   if(SERVREG_NULL == sr_qmi_notif_client_node_del)
   {
      TMS_MSG_ERROR("sr_qmi_notif_client_handle NULL");
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return SERVREG_INVALID_HANDLE;
   }

   if(SERVREG_QMI_NOTIF_CLIENT_SIGNATURE != sr_qmi_notif_client_node_del->notif_signature)
   {
      TMS_MSG_ERROR("sr_qmi_notif_client_handle has invalid signature");
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return SERVREG_INVALID_HANDLE;
   }

   while(SERVREG_NULL != sr_qmi_notif_client_node)
   {
      if(sr_qmi_notif_client_node == sr_qmi_notif_client_node_del)
      {
         if(SERVREG_NULL == sr_notif_prev)
         {
            servreg_qmi_client_internal.servreg_qmi_notif_client_list_head = sr_qmi_notif_client_node->next;
         }
         else
         {
            sr_notif_prev->next = sr_qmi_notif_client_node->next;
         }

         /* Check queue for pending remote service registrations */
         sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);

         /* Check each queue entry for a QMI service UP */
         while(SERVREG_NULL != sr_remote_service_entry)
         {
            if (sr_qmi_notif_client_node == sr_remote_service_entry->qmi_notif_handle)
            {
              q_delete(&servreg_remote_service_q, &sr_remote_service_entry->link);
              free(sr_remote_service_entry);
              break;
            }

            /* Move to next entry */
            sr_remote_service_entry = (servreg_remote_service_p)q_next(&servreg_remote_service_q, &sr_remote_service_entry->link);
         } /* while */
 
         sr_qmi_notif_client_node->next = SERVREG_NULL;

         /* Free memory */
         free(sr_qmi_notif_client_node);
         ret = SERVREG_SUCCESS;
         break;
      }
      else
      {
         sr_notif_prev = sr_qmi_notif_client_node;
         sr_qmi_notif_client_node = sr_qmi_notif_client_node->next;  
      }
   } /* while() */

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);

   return ret;
}


/** =====================================================================
 * Function:
 *     servreg_notif_sync_cb
 *
 * Description:
 *     Callback function invoked when servreg sync complete event that was registered for is received
 *
 * Parameters:
 *     sr_qmi_notif_handle_addr : address of the notifier handle
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_notif_sync_cb(SERVREG_CB_PARAM sr_qmi_notif_handle_addr, SERVREG_CB_PARAM DLL_API_UNUSED ignored)
{
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;

   sr_qmi_notif_client_handle = (SERVREG_QMI_NOTIF_CLIENT_HANDLE)sr_qmi_notif_handle_addr;
   sr_qmi_notif_client_node = sr_qmi_notif_client_handle2sr_qmi_notif_client_node(sr_qmi_notif_client_handle);
   
   if(SERVREG_NULL != sr_qmi_notif_client_node)
   {
      qurt_anysignal_set(&(sr_qmi_notif_client_node->sync_sig), SERVREG_NOTIF_EVT_SYNC);
   }
}

/** =====================================================================
 * Function:
 *     servreg_ssr_down_event_cb
 *
 * Description:
 *     Callback function invoked when ssr event that was registered for is received
 *
 * Parameters:
 *     ssr_name_addr      : address of the name of the SSR event registered
 *     sr_qmi_notif_handle_addr : address of the notifier handle
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_ssr_down_event_cb(uint32_t sr_qmi_notif_handle_addr)
{
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   servreg_remote_service_p sr_remote_service_entry = SERVREG_NULL;
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   SERVREG_NAME service_name = SERVREG_NULL;

   sr_qmi_notif_client_handle = (SERVREG_QMI_NOTIF_CLIENT_HANDLE)sr_qmi_notif_handle_addr;
   sr_qmi_notif_client_node = sr_qmi_notif_client_handle2sr_qmi_notif_client_node(sr_qmi_notif_client_handle);

   if(SERVREG_NULL == sr_qmi_notif_client_node)
   {
      TMS_MSG_HIGH("sr_qmi_notif_client_handle NULL");
      return;
   }
   
   /* Indicate down event occurred for this node */
   sr_qmi_notif_client_node->ssr_down_event = 1;

   /* Update the current remote state and remote transaction_id */
   servreg_set_transaction_id(sr_qmi_notif_client_node->sr_mon_handle, SR_INITIAL_TRANSACTION_ID);

   /* For SSR Down event, transaction_id = 0 */
   ret = servreg_set_state(sr_qmi_notif_client_node->sr_mon_handle, SERVREG_SERVICE_STATE_DOWN);

   if(SERVREG_SUCCESS == ret)
   {
      TMS_MSG_HIGH("Service DOWN due to SSR");

      /* Register sync complete callback */
      if(SERVREG_SUCCESS > servreg_register_sync_notif_cb(sr_qmi_notif_client_node->sr_mon_handle, SERVREG_SERVICE_STATE_DOWN, servreg_notif_sync_cb, (SERVREG_CB_PARAM)sr_qmi_notif_handle_addr, (SERVREG_CB_PARAM)0))
      {
         ERR_FATAL("Could not register servreg sync notification", 0, 0, 0);
      }

      /* Ensure sync is pending before waiting */
      if (0 < servreg_get_pending_acks(sr_qmi_notif_client_node->sr_mon_handle, SERVREG_SERVICE_STATE_DOWN))
      {
         qurt_anysignal_wait(&(sr_qmi_notif_client_node->sync_sig), SERVREG_NOTIF_EVT_SYNC);
         qurt_anysignal_clear(&(sr_qmi_notif_client_node->sync_sig), SERVREG_NOTIF_EVT_SYNC);
      }

      /* Sync complete -- internal clean up */
      servreg_deregister_sync_notif_cb(sr_qmi_notif_client_node->sr_mon_handle, SERVREG_SERVICE_STATE_DOWN, servreg_notif_sync_cb, (SERVREG_CB_PARAM)sr_qmi_notif_handle_addr, (SERVREG_CB_PARAM)0);
   }
   else
   {
      ERR_FATAL("Set state failed for 0x%x",sr_qmi_notif_client_node,0,0);
      return;
   }
   
   service_name = servreg_get_service_name(sr_qmi_notif_client_node->sr_mon_handle);
   if(SERVREG_NULL == service_name)
   {
      ERR_FATAL("servreg_ssr_down_event_cb() servreg_get_service_name failed", 0, 0, 0);
      return;
   }

   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);

   /* check if we have entry already for this service */
   sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);
   while(SERVREG_NULL != sr_remote_service_entry)
   {
      if (sr_qmi_notif_client_handle == sr_remote_service_entry->qmi_notif_handle)
      {
         TMS_MSG_HIGH_2("sr_qmi_notif_client_handle already exist 0x%x , 0x%x",sr_qmi_notif_client_handle, sr_remote_service_entry);
         break;
      }
      /* Move to next entry */
      sr_remote_service_entry = (servreg_remote_service_p)q_next(&servreg_remote_service_q, &sr_remote_service_entry->link);
   } /* while */

   if(SERVREG_NULL == sr_remote_service_entry)/*No enrty found*/
   {
      /* populate entry in remote service waiting for connection queue to wait for QMI notification */
      sr_remote_service_entry = (servreg_remote_service_p)calloc(sizeof(servreg_remote_service_t), 1);
      if(SERVREG_NULL == sr_remote_service_entry)
      {
         ERR_FATAL("calloc failed", 0, 0, 0);
      }

      sr_remote_service_entry->qmi_notif_handle = sr_qmi_notif_client_handle; 
      sr_remote_service_entry->connection_type = SR_QMI_RETRY_SSR;
      strlcpy(sr_remote_service_entry->service_name, service_name, QMI_SERVREG_NOTIF_NAME_LENGTH_V01); // service name

      /* Put the sr_remote_service_entry into the queue and remove after notif */
      q_put(&servreg_remote_service_q, q_link(sr_remote_service_entry, &sr_remote_service_entry->link));
   }

   /* Release the QMI handle */
   sr_qmi_entry = servreg_get_qmi_table_entry(service_name);
   if(SERVREG_NULL == sr_qmi_entry)
   {
      ERR_FATAL("servreg_ssr_down_event_cb() servreg_get_qmi_table_entry failed", 0, 0, 0);
      return;
   }

   if(SERVREG_NULL != sr_qmi_entry->servreg_qmi_notifier_client_handle)
   {
      SERVREG_TRACE_2(RELEASE_QMI_CONN, sr_qmi_notif_client_node->sr_mon_handle, sr_qmi_entry->servreg_qmi_notifier_client_handle);
      if(QMI_NO_ERR != qmi_client_release(sr_qmi_entry->servreg_qmi_notifier_client_handle))
      {
        ERR_FATAL("servreg_ssr_down_event_cb() qmi_client_release failed", 0, 0, 0);
      }
      sr_qmi_entry->servreg_qmi_notifier_client_handle = SERVREG_NULL;
      sr_qmi_entry->connection_state = SERVREG_QMI_DISCONNECTED;
   }
   
   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
}

/** =====================================================================
 * Function:
 *     servreg_connect_remote_listener
 *
 * Description:
 *     Connects client as a remote listener.
 *     QMI used as the communication medium for registering the listener
 *     with the remote service registry framework
 *
 * Parameters:
 *     SERVREG_QMI_NOTIF_CLIENT_HANDLE : Handle of the notifier client node
 *     uint32_t*                       : Pointer for state 
 *     SERVREG_QMI_CONNECT_TYPE        : Connection type enum
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
static SERVREG_RESULT servreg_connect_remote_listener(SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle, uint32_t* sr_curr_remote_state, SERVREG_QMI_CONNECT_TYPE connection_type)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_RESULT qmi_conn_result = SERVREG_FAILURE;
   qmi_client_error_type qmi_client_return;
   qmi_servreg_notif_register_listener_req_msg_v01 servreg_notif_req;
   qmi_servreg_notif_register_listener_resp_msg_v01 servreg_notif_resp;
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;
   SERVREG_NAME service_name = SERVREG_NULL;
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   servreg_remote_service_p sr_remote_service_entry = SERVREG_NULL;
   qmi_client_type servreg_qmi_notifier_client_handle;
   SERVREG_BOOL is_valid = SERVREG_FALSE;

   sr_qmi_notif_client_node = sr_qmi_notif_client_handle2sr_qmi_notif_client_node(sr_qmi_notif_client_handle);

   /* Reconnect an existing notifier handle only */
   if(SERVREG_NULL == sr_qmi_notif_client_node || SERVREG_NULL == sr_qmi_notif_client_node->sr_mon_handle)
   {
      TMS_MSG_HIGH("sr_qmi_notif_client_handle or sr_mon_handle NULL");
      return ret;
   }
   
   service_name = servreg_get_service_name(sr_qmi_notif_client_node->sr_mon_handle);
   if(SERVREG_NULL == service_name)
   {
      TMS_MSG_ERROR("Service name NULL");
      return ret;
   }
   
   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
   
   sr_qmi_entry = servreg_get_qmi_table_entry(service_name);
   if(SERVREG_NULL == sr_qmi_entry)
   {
      TMS_MSG_ERROR("sr_qmi_entry NULL");
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return ret;
   }

   /* Establish a qmi connection with the correct handle during registration of remote service only */
   if(SERVREG_NULL == sr_qmi_entry->servreg_qmi_notifier_client_handle)
   {
       qmi_conn_result = servreg_qmi_connect_to_server(sr_qmi_entry); 
   }
   else
   {
       qmi_conn_result = SERVREG_SUCCESS; //since qmi connection is already present  
   }
   
   servreg_qmi_notifier_client_handle = sr_qmi_entry->servreg_qmi_notifier_client_handle;

   if (SERVREG_IN_PROGRESS != qmi_conn_result && SERVREG_SUCCESS != qmi_conn_result)
   {
      TMS_MSG_ERROR("Cannot connect to QMI server");
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return qmi_conn_result;
   }
   
   // PIL has not spawned SUBSYS 
   if (SERVREG_IN_PROGRESS == qmi_conn_result)
   {
      if (SR_QMI_REGULAR != connection_type)
      {
        SERVREG_TRACE_2(RETRY_QMI_CONN, sr_qmi_notif_client_node->sr_mon_handle, connection_type);
        qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
        return qmi_conn_result;
      }

      /* check if we have entry already for this service */
      sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);
      while(SERVREG_NULL != sr_remote_service_entry)
      {
         if (sr_qmi_notif_client_handle == sr_remote_service_entry->qmi_notif_handle)
         {
            TMS_MSG_HIGH_2("sr_qmi_notif_client_handle already exist 0x%x , 0x%x",sr_qmi_notif_client_handle, sr_remote_service_entry);
            break;
         }
         /* Move to next entry */
         sr_remote_service_entry = (servreg_remote_service_p)q_next(&servreg_remote_service_q, &sr_remote_service_entry->link);
      } /* while */

      if(SERVREG_NULL == sr_remote_service_entry)/*No enrty found*/
      {
         sr_remote_service_entry = (servreg_remote_service_p)calloc(sizeof(servreg_remote_service_t), 1);
         if(SERVREG_NULL == sr_remote_service_entry)
         {
            ERR_FATAL("calloc failed", 0, 0, 0);
         }

         sr_remote_service_entry->qmi_notif_handle = sr_qmi_notif_client_handle; 
         sr_remote_service_entry->connection_type = connection_type;
         strlcpy(sr_remote_service_entry->service_name, service_name, QMI_SERVREG_NOTIF_NAME_LENGTH_V01); // service name

         /* Put the sr_remote_service_entry into the queue and remove after notif */
         q_put(&servreg_remote_service_q, q_link(sr_remote_service_entry, &sr_remote_service_entry->link));
      }

      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return qmi_conn_result;
   }

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);

   /* Enable registration */
   servreg_notif_req.enable = SERVREG_TRUE;
   strlcpy(servreg_notif_req.service_name, service_name, QMI_SERVREG_NOTIF_NAME_LENGTH_V01);

   SERVREG_TIME_TRACE_2(REMOTE_REG_QMI_SEND, sr_qmi_notif_client_node->sr_mon_handle, servreg_qmi_notifier_client_handle);
   SERVREG_TRACE_TIME();
   
   /* QMI message to send request to register as remote listener */
   qmi_client_return = qmi_client_send_msg_sync(servreg_qmi_notifier_client_handle,  /* user_handle */
                                  SERVREG_NOTIF_CLIENT_REGISTER_LISTENER_REQ,               /* msg_id */
                                  (void*)&servreg_notif_req,                         /* *req_c_table */
                                  sizeof(servreg_notif_req),                         /* req_c_table_len*/
                                  (void*)&servreg_notif_resp,                        /* *resp_c_table*/
                                  sizeof(servreg_notif_resp),                        /* resp_c_table_len*/
                                  SERVREG_QMI_NOTIF_CLIENT_REQ_TIMEOUT_MS                   /* timeout_msecs */
                                  );
   if(QMI_NO_ERR != qmi_client_return)
   {
      TMS_MSG_ERROR_1("QMI send error: %d", qmi_client_return);
      SERVREG_TIME_TRACE_2(RESULT, qmi_client_return, 0);
      SERVREG_TRACE_TIME();
      return ret;
   }

   if(QMI_RESULT_SUCCESS_V01 != servreg_notif_resp.resp.result)
   {
      TMS_MSG_ERROR_1("QMI error, Response: %d", servreg_notif_resp.resp.result);
      SERVREG_TIME_TRACE_2(RESULT, qmi_client_return, 0);
      SERVREG_TRACE_TIME();
      return ret;
   }
   
   SERVREG_TIME_TRACE_2(RESULT, qmi_client_return, 0);
   SERVREG_TRACE_TIME();
   
   /* Get the initial state of the remote handle and set that state in the local handle also */
   *sr_curr_remote_state = servreg_notif_resp.curr_state;

   /* Register with SSR to get notified if that subsystem does down */   
   if (SR_QMI_REGULAR == connection_type)
   {
     SERVREG_TRACE_2(REG_SSR_E,service_name,sr_qmi_notif_client_handle);
     servreg_register_ssr_event(service_name, sr_qmi_notif_client_handle);
   }
   else if (SR_QMI_RETRY_REG == connection_type)
   {
     SERVREG_TRACE_2(RETRY_SSR_E,service_name,sr_qmi_notif_client_handle);
     servreg_register_ssr_event(service_name, sr_qmi_notif_client_handle);

     if(SERVREG_SUCCESS == servreg_is_valid_state_change(sr_qmi_notif_client_node->sr_mon_handle,
        servreg_notif_resp.curr_state, &is_valid) && SERVREG_TRUE == is_valid)
     {
       /* Decrease the transaction_id by one as servreg_set_state will increase it by one */
       servreg_set_transaction_id(sr_qmi_notif_client_node->sr_mon_handle, SR_INITIAL_TRANSACTION_ID); 
       servreg_set_state(sr_qmi_notif_client_node->sr_mon_handle, servreg_notif_resp.curr_state);
     }
   }
   else if (SR_QMI_RETRY_SSR == connection_type) /* SSR occurred */
   {
     if(SERVREG_SUCCESS == servreg_is_valid_state_change(sr_qmi_notif_client_node->sr_mon_handle,
        servreg_notif_resp.curr_state, &is_valid) && SERVREG_TRUE == is_valid)
     {
       /* Update the current remote state and remote transaction_id */
       /* Remote transaction id is set to -1 at this point because the subsystem just came back up after SSR */
       servreg_set_transaction_id(sr_qmi_notif_client_node->sr_mon_handle, SR_INITIAL_TRANSACTION_ID);
       servreg_set_state(sr_qmi_notif_client_node->sr_mon_handle, servreg_notif_resp.curr_state);
     }
   }

   ret = SERVREG_SUCCESS;
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_ssr_up_event_cb
 *
 * Description:
 *     Callback function invoked when ssr event that was registered for is received
 *
 * Parameters:
 *     ssr_name_addr      : address of the name of the SSR event registered
 *     sr_qmi_notif_handle_addr : address of the notifier handle
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_ssr_up_event_cb(uint32_t sr_qmi_notif_handle_addr)
{
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;
   servreg_qmi_notif_client_node_p sr_qmi_notif_client_node = SERVREG_NULL;
   servreg_remote_service_p sr_remote_service_entry = SERVREG_NULL;
   uint32_t do_ssr_up_event = 0;

   sr_qmi_notif_client_handle = (SERVREG_QMI_NOTIF_CLIENT_HANDLE)sr_qmi_notif_handle_addr;
   sr_qmi_notif_client_node = sr_qmi_notif_client_handle2sr_qmi_notif_client_node(sr_qmi_notif_client_handle);

   if(SERVREG_NULL == sr_qmi_notif_client_node)
   {
      TMS_MSG_HIGH("sr_qmi_notif_client_node NULL");
      return;
   }

   SERVREG_TRACE_2(SSR_UP_EVENT, sr_qmi_notif_client_node, 0);
   
   /* Return if UP event fired without DOWN event */
   if(!sr_qmi_notif_client_node->ssr_down_event)
   {
     return;
   }
   
   /* Reset flag */
   sr_qmi_notif_client_node->ssr_down_event = 0; 

   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);

   /* Check queue if SSR remote service registrations exist */
   sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);
   
   while(SERVREG_NULL != sr_remote_service_entry)
   {
      if (sr_qmi_notif_client_node == sr_remote_service_entry->qmi_notif_handle)
      {
         do_ssr_up_event = 1;
         break;
      }
      
      /* Move to next entry */
      sr_remote_service_entry = (servreg_remote_service_p)q_next(&servreg_remote_service_q, &sr_remote_service_entry->link);
   } /* while */

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);

   /* Check if queue has the node and signal only if DOWN event occurred */
   if(do_ssr_up_event)
   {
      /* Process new queue entry due to SSR */
      SERVREG_TRACE_0(SSR_UP_PROCESS);
      qurt_anysignal_set(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_AVAILABLE_SIG);
   }
}

/** =====================================================================
 * Function:
 *     servreg_register_ssr_event
 *
 * Description:
 *     Registers for the SSR event based on the service_name
 *     SSR event registration will happen if the remote service is in root-pd or in user-pd.
 *     eg: PD-Down notification will be sent out even when SSR happens. 
 *
 * Parameters:
 *     service_name : string which has the "soc/domain/subdomain" info
 *     sr_qmi_notif_client_handle : Opaque handle to existing event
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_register_ssr_event(SERVREG_NAME service_name, SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle)
{
   SERVREG_NAME ssr_down_name = SERVREG_NULL, ssr_up_name = SERVREG_NULL;

   if(SERVREG_NULL == service_name)
   {
      TMS_MSG_HIGH("User PD need not register for SSR");
      return;
   }
   
   ssr_down_name = servreg_create_ssr_name(service_name, SERVREG_SERVICE_STATE_DOWN);
   if(SERVREG_NULL == ssr_down_name)
   {
      TMS_MSG_ERROR("ssr_down_name is NULL");
      return;
   }

   ssr_up_name = servreg_create_ssr_name(service_name, SERVREG_SERVICE_STATE_UP);
   if(SERVREG_NULL == ssr_up_name)
   {
      TMS_MSG_ERROR("ssr_up_name is NULL");
      free(ssr_down_name);
      return;
   }
   
   /* Every remote service will be registered for SSR events */
   
   /* SSR - Down Event */
   if(RCECB_NULL == rcecb_register_parm1_name(ssr_down_name, (RCECB_CALLBACK_FNSIG_P1)servreg_ssr_down_event_cb, (RCECB_PARM)sr_qmi_notif_client_handle))
   {
      TMS_MSG_ERROR("Could not register with SSR DOWN");
   }

   /* SSR - Up Event */
   if(RCECB_NULL == rcecb_register_parm1_name(ssr_up_name, (RCECB_CALLBACK_FNSIG_P1)servreg_ssr_up_event_cb, (RCECB_PARM)sr_qmi_notif_client_handle))
   {
      TMS_MSG_ERROR("Could not register with SSR UP");
   }

   /* Free the space allocated for ssr_down_name and ssr_up_name as rcecb internally does a strlcpy of the name */
   free(ssr_down_name);
   free(ssr_up_name);
}

/** =====================================================================
 * Function:
 *     servreg_deregister_ssr_event
 *
 * Description:
 *     deregisters for the SSR event based on the service_name
 *
 * Parameters:
 *     service_name : string which has the "soc/domain/subdomain" info
 *     sr_qmi_notif_client_handle : Opaque handle to existing event
 *
 * Returns:
 *     None
 * =====================================================================  */
static void servreg_deregister_ssr_event(SERVREG_NAME service_name, SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle)
{
   SERVREG_NAME ssr_down_name = SERVREG_NULL, ssr_up_name = SERVREG_NULL;

   if(SERVREG_NULL == service_name)
   {
      TMS_MSG_HIGH("User PD need not deregister for SSR");
      return;
   }
   
   ssr_down_name = servreg_create_ssr_name(service_name, SERVREG_SERVICE_STATE_DOWN);
   if(SERVREG_NULL == ssr_down_name)
   {
      TMS_MSG_ERROR("ssr_down_name is NULL");
      return;
   }

   ssr_up_name = servreg_create_ssr_name(service_name, SERVREG_SERVICE_STATE_UP);
   if(SERVREG_NULL == ssr_up_name)
   {
      TMS_MSG_ERROR("ssr_up_name is NULL");
      free(ssr_down_name);
      return;
   }
   
   /* Every remote service will be deregistered for SSR events */
   
   /* SSR - Down Event */
   if(RCECB_NULL == rcecb_unregister_parm1_name(ssr_down_name, (RCECB_CALLBACK_FNSIG_P1)servreg_ssr_down_event_cb, (RCECB_PARM)sr_qmi_notif_client_handle))
   {
      TMS_MSG_ERROR("Could not deregister with SSR DOWN");
   }

   /* SSR - Up Event */
   if(RCECB_NULL == rcecb_unregister_parm1_name(ssr_up_name, (RCECB_CALLBACK_FNSIG_P1)servreg_ssr_up_event_cb, (RCECB_PARM)sr_qmi_notif_client_handle))
   {
      TMS_MSG_ERROR("Could not deregister with SSR UP");
   }

   /* Free the space allocated for ssr_down_name and ssr_up_name as rcecb internally does a strlcpy of the name */
   free(ssr_down_name);
   free(ssr_up_name);
}

/** =====================================================================
 * Function:
 *     servreg_get_qmi_table_entry
 *
 * Description:
 *     Checks if the qmi_instance_id field is already stored for the given domain name
 *
 * Parameters:
 *     domain_name : string which has the "soc/domain/subdomain" info
 *
 * Returns:
 *     Pointer to the qmi entry
 * =====================================================================  */
static servreg_qmi_entry_table_p servreg_get_qmi_table_entry(SERVREG_NAME domain_name)
{
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   SERVREG_NAME pd_name = SERVREG_NULL;

   pd_name = servreg_utils_get_string(domain_name,SOC+DOMAIN+SUBDOMAIN);
   if(SERVREG_NULL == pd_name)
   {
      return SERVREG_NULL;
   }
   
   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
   sr_qmi_entry = servreg_qmi_client_internal.servreg_qmi_table_head;
   
   while(SERVREG_NULL != sr_qmi_entry)
   {
      if(strcmp(pd_name, sr_qmi_entry->domain_name) == 0)
      {
         break;
      }
      else
      {
         sr_qmi_entry = sr_qmi_entry->next;
      }
   }
   
   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
   free(pd_name);
   return sr_qmi_entry;
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
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   uint32_t len = 0;
   SERVREG_NAME local_subdomain_name = SERVREG_NULL;

   local_subdomain_name = servreg_get_local_subdomain_scope();

   if(strcmp(local_subdomain_name, "root_pd") != 0)
   {
      TMS_MSG_ERROR("No need to record qmi_instance_id in user PD");
      return;
   }
   
   /* Record already exists */
   sr_qmi_entry = servreg_get_qmi_table_entry(domain_name);
   if(SERVREG_NULL != sr_qmi_entry)
   {
      return;
   }
   
   sr_qmi_entry = (servreg_qmi_entry_table_p)calloc(sizeof(servreg_qmi_entry_table_t), 1);
   if(SERVREG_NULL == sr_qmi_entry)
   {
      ERR_FATAL( "In servreg_create_qmi_table_entry() alloc failed", 0, 0, 0);
   }  
   
   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);

   /*  Insert to head of list */
   sr_qmi_entry->next = servreg_qmi_client_internal.servreg_qmi_table_head;
   /* Update head */
   servreg_qmi_client_internal.servreg_qmi_table_head = sr_qmi_entry;

   len = servreg_nmelen(domain_name) + 1;
   sr_qmi_entry->domain_name = (SERVREG_NAME)calloc(sizeof(char), len);
   if(SERVREG_NULL == sr_qmi_entry->domain_name)
   {
      TMS_MSG_SPRINTF_HIGH_2("Cannot create QMI table entry for %s, ID: %ld", domain_name, qmi_instance_id);
      ERR_FATAL( "In servreg_create_qmi_table_entry() calloc failed", 0, 0, 0);
   }
   
   strlcpy(sr_qmi_entry->domain_name, domain_name, len);
   sr_qmi_entry->qmi_instance_id = qmi_instance_id;
   sr_qmi_entry->servreg_qmi_notifier_client_handle = SERVREG_NULL;
   sr_qmi_entry->connection_state = SERVREG_QMI_CREATED;
   
   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
   
   return;
}

/** =====================================================================
 * Function:
 *     servreg_register_remote_listener
 *
 * Description:
 *     Register client as a remote listener because the service is not a local service.
 *     QMI or sockets used as the communication medium for registering the listener
 *     with the remote service registry framework
 *
 *     For every service, remote registration is done only once and one proxy listener in 
 *     the root PD of the other QMI end point will be registered that represents all 
 *     the listeners in the clients root PD for that service.
 *
 * Parameters:
 *     service_name             : Service name that the client is interested in 
 *     sr_mon_handle            : Opaque handle to existing event
 *     sr_curr_remote_state     : State of the service received from the QMI message
 *     sr_remote_transaction_id : Transaction id of the service received from the QMI message
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_register_remote_listener(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, uint32_t * sr_curr_remote_state, int32_t * sr_remote_transaction_id)
{
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;

   /* Check if the srnotif node exists */
   sr_qmi_notif_client_handle = servreg_get_qmi_notif_client_node(sr_mon_handle);

   /* Create a new notif node and register a new proxy listener for that service in root-pd of the other subsystem via QMI only if the srnotif node does not exist */
   if(SERVREG_NULL != sr_qmi_notif_client_handle)
   {
      TMS_MSG_HIGH("Proxy client already registered via QMI");
      ret = SERVREG_SUCCESS;
      return ret;
   }
   
   /* Create a Service Notifier node to list the sr_mon_handle to get remote notifications from root pd */
   sr_qmi_notif_client_handle = servreg_create_qmi_notif_client_node(sr_mon_handle);
   if(SERVREG_NULL == sr_qmi_notif_client_handle)
   {
      TMS_MSG_ERROR("Could not register as a remote listener in the notifier"); //Failed to create notif node
      return ret;
   }
   
   /* servreg_connect_remote_listener() will give updated state */
   /* IDL does not support transaction id, do not update it */
   ret = servreg_connect_remote_listener(sr_qmi_notif_client_handle, sr_curr_remote_state, SR_QMI_REGULAR);
   if (SERVREG_IN_PROGRESS != ret && SERVREG_SUCCESS != ret)
   {
      /* Delete the Service Notifier node */
      if(SERVREG_SUCCESS != servreg_delete_qmi_notif_client_node(sr_qmi_notif_client_handle))
      {
         TMS_MSG_ERROR("Delete QMI notif node failed");
      }
   }
   else if(SERVREG_IN_PROGRESS == ret)
   {
      /* Treat this as success as we offload it and process post QMI signal */
      /* Register with SSR in task post QMI signal */
      ret = SERVREG_SUCCESS;
   }
   
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_deregister_remote_listener
 *
 * Description:
 *     De-register the remote proxy listener via QMI only if there are no more local listeners 
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
   qmi_client_error_type qmi_client_return;
   qmi_servreg_notif_register_listener_req_msg_v01 servreg_notif_req;
   qmi_servreg_notif_register_listener_resp_msg_v01 servreg_notif_resp;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;
   qmi_client_type servreg_qmi_notifier_client_handle;
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   
   /* Delete the notif node and de-register the proxy listener for that service in root-pd of the other subsystem via QMI */
   sr_qmi_notif_client_handle = servreg_get_qmi_notif_client_node(sr_mon_handle);

   /* Proceed if the srnotif node already exist */
   if(SERVREG_NULL == sr_qmi_notif_client_handle)
   {
      TMS_MSG_ERROR("sr_qmi_notif_client_handle NULL");
      return ret;
   }

   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
   
   sr_qmi_entry = servreg_get_qmi_table_entry(service_name);
   if(SERVREG_NULL == sr_qmi_entry)
   {
      TMS_MSG_ERROR("sr_qmi_entry NULL");
      
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return ret;
   }
   
   /* Disable registration */
   servreg_notif_req.enable = SERVREG_FALSE;

   servreg_qmi_notifier_client_handle = sr_qmi_entry->servreg_qmi_notifier_client_handle;

   qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
   strlcpy(servreg_notif_req.service_name, service_name, QMI_SERVREG_NOTIF_NAME_LENGTH_V01);

   SERVREG_TIME_TRACE_2(REMOTE_DEREG_QMI_SEND, sr_mon_handle, servreg_qmi_notifier_client_handle);
   SERVREG_TRACE_TIME();
   
   /* QMI message to send request to deregister as remote listener */
   qmi_client_return = qmi_client_send_msg_sync(servreg_qmi_notifier_client_handle,  /* user_handle */
                                        SERVREG_NOTIF_CLIENT_REGISTER_LISTENER_REQ,               /* msg_id */
                                        (void*)&servreg_notif_req,                         /* *req_c_table */
                                        sizeof(servreg_notif_req),                         /* req_c_table_len*/
                                        (void*)&servreg_notif_resp,                        /* *resp_c_table*/
                                        sizeof(servreg_notif_resp),                        /* resp_c_table_len*/
                                        SERVREG_QMI_NOTIF_CLIENT_REQ_TIMEOUT_MS                   /* timeout_msecs */
                                        );
   if(QMI_NO_ERR != qmi_client_return)
   {
      TMS_MSG_ERROR_1("QMI send error: %d", qmi_client_return);
   }
   else
   {
      if(QMI_RESULT_SUCCESS_V01 == servreg_notif_resp.resp.result)
      {
         SERVREG_TRACE_2(DEREG_SSR_E,service_name,sr_qmi_notif_client_handle);
         servreg_deregister_ssr_event(service_name,sr_qmi_notif_client_handle);
         /* Delete the Service Notifier node as there is only one remote registrant for that service per root PD */
         ret = servreg_delete_qmi_notif_client_node(sr_qmi_notif_client_handle);
      }
      else
      {
         TMS_MSG_ERROR_1("QMI error, Response: %d", servreg_notif_resp.resp.result);
      }
   }

   SERVREG_TIME_TRACE_2(RESULT, qmi_client_return, 0);
   SERVREG_TRACE_TIME();
   
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
   /* No QMI message currently in IDL file that supports free_remote_handle() */
   /* Currently we delete the notif node if the number of registrants = 0 */
   return SERVREG_SUCCESS;
}

/** =====================================================================
 * Function:
 *     servreg_set_remote_ack
 *
 * Description:
 *     Set the remote ack count for the remote proxy listener via QMI only if 
 *     all the local acks have been received
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
SERVREG_RESULT servreg_set_remote_ack(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, SERVREG_SERVICE_STATE curr_state, int32_t sr_transaction_id)
{
   servreg_remote_ack_req_p sr_remote_ack_req_entry = SERVREG_NULL;

   if(0 >= sr_transaction_id)
   {
      TMS_MSG_HIGH_1("Sent ACK. transaction_id: %d", sr_transaction_id);
      return SERVREG_SUCCESS;
   }
   
   sr_remote_ack_req_entry = (servreg_remote_ack_req_p)calloc(sizeof(servreg_remote_ack_req_t), 1);
   if(SERVREG_NULL == sr_remote_ack_req_entry)
   {
      ERR_FATAL("servreg_set_remote_ack() calloc failed: %x %x %d", sr_mon_handle, curr_state, sr_transaction_id);
   }
   
   sr_remote_ack_req_entry->service_name = service_name;
   sr_remote_ack_req_entry->sr_mon_handle = sr_mon_handle;
   sr_remote_ack_req_entry->transaction_id = sr_transaction_id;
   
   /* Put the sr_remote_ack_req_entry into the queue and remove it only after you get the ACK back */
   q_put(&servreg_remote_ack_req_q, q_link(sr_remote_ack_req_entry, &sr_remote_ack_req_entry->link));
   
   /* Signal task about new queue entry for remote ack request */
   qurt_anysignal_set(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_ACK_REQ_SIG);
 
   return SERVREG_SUCCESS;
}

/** =====================================================================
 * Function:
 *     servreg_remote_queue_action
 *
 * Description: 
 *     Called by the servreg_remote_ack_req_task to service remote ACK requests
 *     queued up by the client from servreg_set_remote_ack
 *
 * Parameters:
 *     service_name       : Service name that the client is interested in 
 *     SERVREG_MON_HANDLE : Opaque handle to existing event
 *     sr_transaction_id  : Transaction ID
 *
 * Returns:
 *     Refer to the enum SERVREG_RESULT for list of possible results
 * =====================================================================  */
static SERVREG_RESULT servreg_remote_queue_action(SERVREG_NAME service_name, SERVREG_MON_HANDLE sr_mon_handle, int32_t sr_transaction_id)
{
   qmi_client_error_type qmi_client_return;
   qmi_servreg_notif_set_ack_req_msg_v01 servreg_notif_req;
   qmi_servreg_notif_set_ack_resp_msg_v01 servreg_notif_resp;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   SERVREG_QMI_NOTIF_CLIENT_HANDLE sr_qmi_notif_client_handle = SERVREG_NULL;
   servreg_qmi_entry_table_p sr_qmi_entry = SERVREG_NULL;
   qmi_client_type servreg_qmi_notifier_client_handle;

   /* Check if the srnotif node exists */
   sr_qmi_notif_client_handle = servreg_get_qmi_notif_client_node(sr_mon_handle);
   if(SERVREG_NULL == sr_qmi_notif_client_handle)
   {
      TMS_MSG_ERROR("sr_qmi_notif_client_handle NULL");
      return ret;
   }
   
   qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);

   sr_qmi_entry = servreg_get_qmi_table_entry(service_name);
   if(SERVREG_NULL == sr_qmi_entry)
   {
      TMS_MSG_ERROR("sr_qmi_entry NULL");
      qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      return ret;
   }

    servreg_qmi_notifier_client_handle = sr_qmi_entry->servreg_qmi_notifier_client_handle;
    qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
   
    strlcpy(servreg_notif_req.service_name, service_name, QMI_SERVREG_NOTIF_NAME_LENGTH_V01);
    servreg_notif_req.transaction_id = sr_transaction_id;
        
    SERVREG_TIME_TRACE_2(REMOTE_ACK_QMI_SEND, sr_mon_handle, sr_transaction_id);
   SERVREG_TRACE_TIME();

    /* QMI message to the ACK message req */
    qmi_client_return = qmi_client_send_msg_sync(servreg_qmi_notifier_client_handle,  /* user_handle */
                                   SERVREG_NOTIF_CLIENT_STATE_UPDATED_IND_ACK_REQ,           /* msg_id */
                                   (void*)&servreg_notif_req,                         /* *req_c_table */
                                   sizeof(servreg_notif_req),                         /* req_c_table_len*/
                                   (void*)&servreg_notif_resp,                        /* *resp_c_table*/
                                   sizeof(servreg_notif_resp),                        /* resp_c_table_len*/
                                   SERVREG_QMI_NOTIF_CLIENT_REQ_TIMEOUT_MS                   /* timeout_msecs */
                                   );
    if(QMI_NO_ERR != qmi_client_return)
    {
       TMS_MSG_ERROR_1("QMI send error: %d", qmi_client_return);
       SERVREG_TRACE_2(RESULT, qmi_client_return, 0);
    }
    else
    {
       if(QMI_RESULT_SUCCESS_V01 == servreg_notif_resp.resp.result)
       {
          ret = SERVREG_SUCCESS;
          TMS_MSG_HIGH("QMI ACK sent");
          SERVREG_TRACE_0(REMOTE_ACK_QMI_SUCCESS);
       }
       else
       {
          TMS_MSG_ERROR_1("QMI error, Response: %d", servreg_notif_resp.resp.result);
          SERVREG_TRACE_2(RESULT, servreg_notif_resp.resp.result, 0);
       }
    }

   SERVREG_TIME_TRACE_2(RESULT, qmi_client_return, 0);
   SERVREG_TRACE_TIME();
        
   return ret;
}

/** =====================================================================
 * Function:
 *     servreg_notif_client_ind_cb
 *
 * Description: 
 *     Called by the QCCI infrastructure when an INDICATION message
 *     is received by the client
 *
 * Parameters:
 *     client_handle : Handle used by the infrastructure to 
 *                     identify different clients.
 *     msg_id        : Message ID
 *     ind_buf       : Pointer to the raw/un-decoded indication
 *     ind_buf_len   : Length of the indication
 *     ind_cb_data   : User-data
 *
 * Returns:
 *             None
 * =====================================================================  */
static void servreg_notif_client_ind_cb(qmi_client_type client_handle, uint32_t msg_id, void *ind_buf, uint32_t ind_buf_len, void *ind_cb_data)
{
   void *ind_msg = SERVREG_NULL;
   qmi_client_error_type qmi_err;
   uint32_t decoded_size;

   SERVREG_TRACE_0(QMI_IND_CB);
 
   qmi_err = qmi_idl_get_message_c_struct_len(servreg_notif_service_object, QMI_IDL_INDICATION, msg_id, &decoded_size);

   if(QMI_NO_ERR != qmi_err)
   {
      TMS_MSG_ERROR_1("QMI get error: %d", qmi_err);
      return;
   }

   ind_msg = calloc(decoded_size, 1);
   if(!ind_msg) 
   {
      TMS_MSG_ERROR("Indication buff allocation failed");
      return;
   }

   qmi_err = qmi_client_message_decode(client_handle, QMI_IDL_INDICATION, msg_id, ind_buf, ind_buf_len, ind_msg, decoded_size);
   if (qmi_err != QMI_NO_ERR)
   {
      TMS_MSG_ERROR_1("QMI decode error: %d", qmi_err);
      free(ind_msg);
      return;
   }
 
   /* Insert queue entry for remote_req_task */
   servreg_remote_qmi_ind_p sr_remote_qmi_ind_entry = SERVREG_NULL;
   
   sr_remote_qmi_ind_entry = (servreg_remote_qmi_ind_p)calloc(sizeof(servreg_remote_qmi_ind_t), 1);
   if(SERVREG_NULL == sr_remote_qmi_ind_entry)
   {
      TMS_MSG_ERROR("sr_remote_qmi_ind_entry calloc failed");
      free(ind_msg);
      return;
   }

   /* copy decoded_size bites of ind_msg to sr_remote_qmi_ind_entry->rcvd_ind_qmi_msg */
   memscpy((void*)&sr_remote_qmi_ind_entry->rcvd_ind_qmi_msg, sizeof(sr_remote_qmi_ind_entry->rcvd_ind_qmi_msg), ind_msg, decoded_size);
 
   /* Put the sr_remote_qmi_ind_entry into the queue and remove it after processing */
   q_put(&servreg_remote_qmi_ind_q, q_link(sr_remote_qmi_ind_entry, &sr_remote_qmi_ind_entry->link));
  
   /* Send signal to remote_req_task for decoupled SR internal processing */
   qurt_anysignal_set(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_QMI_IND_SIG); 
 
   free(ind_msg);
}

/** =====================================================================
 * Function:
 *     servreg_qmi_connect_to_server
 *
 * Description:
 *     Initializes the service qmi_notifier QMI client
 *
 * Parameters:
 *     servreg_qmi_entry_table_p sr_qmi_entry
 *
 * Returns:
 *     SERVREG_SUCCESS, SERVREG_FAILURE
 * =====================================================================  */
static SERVREG_RESULT servreg_qmi_connect_to_server(servreg_qmi_entry_table_p sr_qmi_entry)
{
   qmi_client_error_type qmi_client_return;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   qmi_service_info servreg_notif_server_info;
   qmi_client_type servreg_qmi_notifier_client_handle;

   if(SERVREG_NULL == sr_qmi_entry)
   {
      TMS_MSG_ERROR("QMI Instance ID not found");
      ret = SERVREG_INVALID_HANDLE;
      return ret;
   }
   
   qmi_client_return = qmi_client_get_service_instance(servreg_notif_service_object, sr_qmi_entry->qmi_instance_id, &servreg_notif_server_info);
   if(QMI_NO_ERR != qmi_client_return)
   {
      TMS_MSG_ERROR_2("Wait for QMI server at instance ID %d: %d", sr_qmi_entry->qmi_instance_id, qmi_client_return);
      //qmi_client_release(servreg_qmi_client_notifier);
      sr_qmi_entry->connection_state = SERVREG_QMI_PENDING;
      ret = SERVREG_IN_PROGRESS;
      return ret;
   }
              
   qmi_client_return = qmi_client_init(&servreg_notif_server_info, servreg_notif_service_object, (qmi_client_ind_cb)servreg_notif_client_ind_cb, NULL, NULL, &servreg_qmi_notifier_client_handle);
   if(QMI_NO_ERR != qmi_client_return)
   {
      TMS_MSG_ERROR_1("QMI client init failed: %d", qmi_client_return);
      //qmi_client_release(servreg_qmi_client_notifier);
      return ret;
   }
   
   /* Record server connection specific info */
   sr_qmi_entry->servreg_notif_server_info = servreg_notif_server_info;
   sr_qmi_entry->servreg_qmi_notifier_client_handle = servreg_qmi_notifier_client_handle;
   sr_qmi_entry->connection_state = SERVREG_QMI_CONNECTED;

   TMS_MSG_HIGH_2("QMI client init: Instance ID: %d, handle: %d", sr_qmi_entry->qmi_instance_id, servreg_qmi_notifier_client_handle);
   SERVREG_TRACE_2(INSTANCE_UP, sr_qmi_entry->qmi_instance_id, servreg_qmi_notifier_client_handle);
   ret = SERVREG_SUCCESS;
   
   return ret;
}

/** =====================================================================
 * Function:
 *     sr_ind_process
 *
 * Description: 
 *     Offloaded to handle QCCI infrastructure INDICATION service
 *     registry side processing
 *
 * Parameters:
 *     qmi_servreg_notif_state_updated_ind_msg_v01*
 *
 * Returns:
 *     SERVREG_RESULT
 * =====================================================================  */
static SERVREG_RESULT sr_ind_process(qmi_servreg_notif_state_updated_ind_msg_v01* indication)
{
   SERVREG_MON_HANDLE sr_mon_handle = SERVREG_NULL;
   SERVREG_RESULT ret = SERVREG_FAILURE;
   int32_t transaction_id = 0;
      
   /* Get the existing sr_mon_handle */
   sr_mon_handle = servreg_get_sr_mon_handle(indication->service_name);
   
   /* If we receive an unexpected indication, server starts ack check timer and crashes 
      after 10 seconds of not getting ACK, so crash now instead */
   if(SERVREG_NULL == sr_mon_handle)
   {
      char * temp = calloc(sizeof (char), TMS_UTILS_BUFFER_SIZE);
      if ( temp != NULL )
      {
          (void)tms_utils_fmt(temp, TMS_UTILS_BUFFER_SIZE, "Unexpected indication: %s: 0x%x", indication->service_name,indication->curr_state);
          ERR_FATAL_AUX_MSG("Unexpected indication received. Check coredump.err.aux_msg", temp, TMS_UTILS_BUFFER_SIZE);
      }
      else
      {
         ERR_FATAL("Unexpected indication received.", 0, 0, 0);
      }
   }
   
   /* Decrease the transaction_id by one as servreg_set_state will increase it by one */
   transaction_id = indication->transaction_id - 1;
   servreg_set_transaction_id(sr_mon_handle, transaction_id);
   SERVREG_TRACE_2(QMI_IND_PROCESS, sr_mon_handle, transaction_id);
   
   ret = servreg_set_state(sr_mon_handle, (SERVREG_SERVICE_STATE)indication->curr_state);
   if(SERVREG_SUCCESS != ret)
   {
      ERR_FATAL( "ind msg servreg_set_state() error", 0, 0, 0);
   }
   
   return ret;
}

/** =====================================================================
 * Task:
 *     sr_remote_req_task
 *
 * Description:
 *     Task used for processing queued up remote ACK requests
 *
 * Parameters:
 *     None
 *
 * Returns:
 *     None
 * =====================================================================  */
void sr_remote_req_task( void* unused_param )
{
   uint32_t q_mask;
   
   /* Task forever loop */
   for (;;)
   {
      q_mask = qurt_anysignal_wait(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_ACK_REQ_SIG | SERVREG_QMI_REMOTE_QMI_IND_SIG | SERVREG_QMI_REMOTE_AVAILABLE_SIG);

      /* Handle outgoing acks */
      if (q_mask & SERVREG_QMI_REMOTE_ACK_REQ_SIG)
      {
         qurt_anysignal_clear(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_ACK_REQ_SIG);
                  
         servreg_remote_ack_req_p sr_remote_ack_req_entry = SERVREG_NULL;
         sr_remote_ack_req_entry = (servreg_remote_ack_req_p)q_get(&servreg_remote_ack_req_q);

         while(SERVREG_NULL != sr_remote_ack_req_entry)
         {
            SERVREG_TRACE_2(REMOTE_ACK_Q_GET,sr_remote_ack_req_entry->sr_mon_handle, 0);  
            if (SERVREG_SUCCESS != servreg_remote_queue_action(sr_remote_ack_req_entry->service_name, sr_remote_ack_req_entry->sr_mon_handle, sr_remote_ack_req_entry->transaction_id))
            {
               TMS_MSG_ERROR("servreg_remote_queue_action Failure");
            }
            
            free(sr_remote_ack_req_entry);
            sr_remote_ack_req_entry = (servreg_remote_ack_req_p)q_get(&servreg_remote_ack_req_q);
         }
      }
      
      /* Handle incoming state change notifications from remote services */
      if (q_mask & SERVREG_QMI_REMOTE_QMI_IND_SIG)
      {
         qurt_anysignal_clear(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_QMI_IND_SIG);
         
         SERVREG_TRACE_0(REMOTE_QMI_IND_SIG);
         
         servreg_remote_qmi_ind_p sr_remote_qmi_ind_entry = SERVREG_NULL;
         sr_remote_qmi_ind_entry = (servreg_remote_qmi_ind_p)q_get(&servreg_remote_qmi_ind_q);
         while(SERVREG_NULL != sr_remote_qmi_ind_entry)
         {
            if (SERVREG_SUCCESS != sr_ind_process(&sr_remote_qmi_ind_entry->rcvd_ind_qmi_msg))
            {
               TMS_MSG_ERROR("sr_ind_process Failure");
            }
            
            free(sr_remote_qmi_ind_entry);
            sr_remote_qmi_ind_entry = (servreg_remote_qmi_ind_p)q_get(&servreg_remote_qmi_ind_q);
         }
      }

      /* Handle all remote registrations */
      if (q_mask & SERVREG_QMI_REMOTE_AVAILABLE_SIG)
      {
         qurt_anysignal_clear(&servreg_remote_op_signal, SERVREG_QMI_REMOTE_AVAILABLE_SIG);

         SERVREG_TRACE_0(QMI_INSTANCE_SIG);

         qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
         /* Check queue for remote service registrations */
         servreg_remote_service_p sr_remote_service_entry = SERVREG_NULL;
         sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);
         if (SERVREG_NULL == sr_remote_service_entry)
            SERVREG_TRACE_0(EMPTY_Q_NOTIF);
        
         /* Check each queue entry for a QMI service UP */
         while(SERVREG_NULL != sr_remote_service_entry)
         {
            SERVREG_RESULT ret = SERVREG_FAILURE;
            uint32_t sr_curr_remote_state = SERVREG_SERVICE_STATE_UNINIT;
         
            if (SR_QMI_REGULAR == sr_remote_service_entry->connection_type)
            {
              sr_remote_service_entry->connection_type = SR_QMI_RETRY_REG;
            }
            qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
            ret = servreg_connect_remote_listener(sr_remote_service_entry->qmi_notif_handle, &sr_curr_remote_state, sr_remote_service_entry->connection_type);
            qurt_pimutex_lock(&servreg_qmi_client_internal.mutex);
            if(SERVREG_SUCCESS != ret)
            {
              /* Go to next entry in queue if this instance is not yet UP or a failure occurred */
              sr_remote_service_entry = (servreg_remote_service_p)q_next(&servreg_remote_service_q, &sr_remote_service_entry->link);
              continue;
            }
        
            /* Remove queue entry after successful completion of QMI transactions */
            q_delete(&servreg_remote_service_q, &sr_remote_service_entry->link);
            
            /* Move to next entry */
            free(sr_remote_service_entry);
            sr_remote_service_entry = (servreg_remote_service_p)q_check(&servreg_remote_service_q);
          } /* while */
          qurt_pimutex_unlock(&servreg_qmi_client_internal.mutex);
      } /* if SERVREG_QMI_REMOTE_AVAILABLE_SIG */

    } /* for(;;) */
}

/** =====================================================================
 * Function:
 *     servreg_qmi_notif_client_init
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
void servreg_qmi_notif_client_init(void)
{
   qmi_client_error_type qmi_client_return; 
   
   servreg_qmi_client_internal.servreg_qmi_table_head = SERVREG_NULL;
   servreg_qmi_client_internal.servreg_qmi_notif_client_list_head = SERVREG_NULL;

   qurt_pimutex_init(&servreg_qmi_client_internal.mutex);

   /* Initialize the remote requests queue and signal */
   (void) q_init(&servreg_remote_ack_req_q);
   (void) q_init(&servreg_remote_qmi_ind_q);
   (void) q_init(&servreg_remote_service_q);
   qurt_anysignal_init(&servreg_remote_op_signal);

   if(SERVREG_SERVICE_STATE_UP == servreg_qmi_notifier_client_init_status)
   {
      return;
   }

   servreg_notif_service_object = servreg_notif_get_service_object_v01();
   if (SERVREG_NULL == servreg_notif_service_object)
   {
      TMS_MSG_ERROR("servreg_notif_service_object NULL");
      return;
   }
   
   memset(&os_param, 0, sizeof(qmi_client_os_params));
   os_param.ext_signal = &servreg_remote_op_signal; 
   os_param.sig = SERVREG_QMI_REMOTE_AVAILABLE_SIG;
   
   /* Initialize the qmi client notifier */
   qmi_client_return = qmi_client_notifier_init(servreg_notif_service_object, &os_param, &servreg_qmi_client_notifier);

   if(QMI_NO_ERR != qmi_client_return)
   {
      TMS_MSG_ERROR_1("qmi_client_notifier_init failed: %d",qmi_client_return);
      return;
   }
   
   servreg_qmi_notifier_client_init_status = SERVREG_SERVICE_STATE_UP;

   (void)tms_utils_launch_qurt_task(SERVREG_REMOTE_REQ_TASK, TMS_TASK_STACK_3K, TMS_TASK_PRIO_DEFAULT, sr_remote_req_task, NULL, TRUE);
}
