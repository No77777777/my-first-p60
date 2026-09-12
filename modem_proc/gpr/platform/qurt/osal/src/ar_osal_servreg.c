/*
 * Copyright (c) 2020-2021, 2022 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ar_osal_servreg.c
 *
 * This file has implementation of thread related helper functions.
 *
 */

#include "ar_osal_types.h"
#include "ar_osal_servreg.h"
#include "ar_osal_signal2.h"
#include "ar_osal_mutex.h"
#include "ar_osal_error.h"
#include "ar_osal_thread.h"
#include "ar_osal_log.h"
#include <stdlib.h>
#include <string.h>
#include "servreg_locator.h"
#include "servreg_locator_qurt.h"
#include "servreg_monitor.h"
#include "servreg_monitor_qurt.h"
#include "ar_msg.h"
#include "ar_osal_heap.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_string.h"
/*Macro Declarations*/
#define AR_SERVREG_THR_STACK_SIZE 2 * 1024
/*Allowed number of registrations*/
#define SERVICE_NODE_REGISTER_COUNT 16
#define AR_SERVREG_THR_PRIO 182
/*Signal mask for service locatory*/
#define SERVREG_SIG_LOC_AVAILABLE 0x1
#define AR_OSAL_SERVREG_TAG "CASR"
#define LISTENER_WORKER_THREAD_ACK 1
#define WORKER_THR_KILL_SIG 2
/*Unused mask range token*/
#define UNUSED_MASK_TOKEN 3

/*Globals*/
int32_t           g_init_done = 0;
ar_osal_signal2_t listner_worker_thread_ack;

/*Structures and typedefs*/
typedef struct service_node
{
   /* Stores the service handle */
   ar_osal_servreg_t srv_handle;

   /* Service info */
   ar_osal_servreg_entry_type service;

   /* Domain info */
   ar_osal_servreg_entry_type domain;

   /* Stores the cb context data */
   void *cb_context;

   /* callback function */
   ar_osal_servreg_callback cb_func;

   /* Indicates whether service provider or service user */
   bool_t is_service;

   /* Stores the service state */
   ar_osal_service_state_type srv_state;

   /* Identify whether service registered to tms(For listeners only) */
   bool_t is_servreg_registered;

   /* Signal mask to wait */
   uint32_t signal_mask;
} service_node_type;

typedef struct servreg_handle_pool
{
   /* Stores the nodes info */
   service_node_type **node;

   /* No of nodes */
   uint32_t num_nodes;

   /* Indicates if servreg is available from tms/core */
   bool_t is_servreg_available;

   /* Stores the signal for waiting on all the events */
   ar_osal_signal2_t servreg_wait_all_status_signal;

   /* Holds the lock */
   ar_osal_mutex_t lock;

   /*worker thread info serving service nodes*/
   ar_osal_thread_t tid;
} servreg_handle_pool_type;

/* Global service handle to stores all the service and listeners info */
servreg_handle_pool_type servreg_handle_pool;

/****************************************************************************************/
/**********************************  INTERNAL FUNCTIONS *********************************/
/****************************************************************************************/

/*Fetches the free node number*/
static ar_osal_servreg_t get_available_node(uint32_t *node_num)
{

   ar_osal_servreg_t handle = NULL;
   uint32_t          i      = 0;

   if (NULL == node_num)
   {
      return NULL;
   }

   // Get available free node numbers..
   for (i = 0; i < SERVICE_NODE_REGISTER_COUNT; i++)
   {
      if (NULL == servreg_handle_pool.node[i])
      {
         *node_num = i;
         handle    = node_num;
         break;
      }
   }

   return handle;
}

/*Map the service event states according to the service registry datatypes*/
static uint32_t map_service_event_state(uint32_t service_state)
{
   switch (service_state)
   {
      case AR_OSAL_SERVICE_STATE_DOWN:
         return SERVREG_SERVICE_STATE_DOWN;
      case AR_OSAL_SERVICE_STATE_UP:
         return SERVREG_SERVICE_STATE_UP;
      case SERVREG_SERVICE_STATE_DOWN:
         return AR_OSAL_SERVICE_STATE_DOWN;
      case SERVREG_SERVICE_STATE_UP:
         return AR_OSAL_SERVICE_STATE_UP;
      default:
         return SERVREG_SERVICE_STATE_UNINIT;
   }
}

/* Utility to identify whether the node is present in the service pool or not */
static int32_t is_node_linked(ar_osal_servreg_t node, uint32_t *node_number)
{
   bool_t is_node_available = AR_ENOTEXIST;

   for (uint32_t i = 0; i <= SERVICE_NODE_REGISTER_COUNT; i++)
   {
      if (node == servreg_handle_pool.node[i])
      {
         is_node_available = AR_EOK;
         *node_number      = i;
         break;
      }
   }
   return is_node_available;
}

/* Internal function to register as listener */
uint32_t ar_osal_register_listener_internal(ar_osal_servreg_t listener_info)
{
   uint32_t           status = AR_EOK;
   ar_osal_servreg_t  handle;
   service_node_type *node;
   uint32_t           service_status = 0;

   if (NULL == listener_info)
   {
      return AR_EBADPARAM;
   }

   node   = (service_node_type *)listener_info;
   handle = servreg_alloc_monitor_handle(node->domain.name, node->service.name);
   
   #ifndef SIM
    MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Domain name %s ", node->domain.name);
    MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"service name %s ", node->service.name);
   #endif
   
   if (NULL == handle)
   {
      AR_MSG(DBG_HIGH_PRIO, "Invalid domain(%s) and/or service (%s)", node->domain.name, node->service.name);
      #ifndef SIM
        MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Failed Domain name %s ", node->domain.name);
        MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Failed service name %s ", node->service.name);
      #endif
      return AR_ENOTEXIST;
   }
   node->srv_handle            = handle;
   node->is_servreg_registered = TRUE;

   AR_MSG(DBG_HIGH_PRIO, "Domain (%s) and service(%s) are valid", node->domain.name, node->service.name);
   status = servreg_register_listener_qurt(node->srv_handle,
                                           servreg_handle_pool.servreg_wait_all_status_signal,
                                           node->signal_mask);
   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "Listener registration failed with status %d", status);
      status = servreg_free_monitor_handle(handle);
      if (AR_EOK != status)
      {
         AR_MSG(DBG_HIGH_PRIO, "Failed to free monitor handle");
      }
      return status;
   }
   else
   {
      service_status  = servreg_get_service_curr_state(node->srv_handle);
      node->srv_state = (ar_osal_service_state_type)map_service_event_state(service_status);
      AR_MSG(DBG_HIGH_PRIO, "rcved state as (%d) - (%d)", node->srv_state, service_status);
   }

   return status;
}

/* Service locator available callback registration*/
/* Whenever service locatory from core/tms is available, this callback is triggered*/
void servreg_locator_available(uint32_t data, uint32_t ignore)
{
   /* Signal task to start pdr registration */
   AR_MSG(DBG_HIGH_PRIO, "Service locatory call back received");

   ar_osal_mutex_lock(servreg_handle_pool.lock);

   // Triggering callback more than once, should not be impacted. Below check should take care on this
   servreg_handle_pool.is_servreg_available = TRUE;

   if (TRUE == servreg_handle_pool.is_servreg_available) // check is to ensure setting the signal only once.
   {
      AR_MSG(DBG_HIGH_PRIO, "Service locatory is available");

      // set the signal when servreg is available, so any events on this signal will receive the availability
      ar_osal_signal2_set(servreg_handle_pool.servreg_wait_all_status_signal, SERVREG_SIG_LOC_AVAILABLE);
   }

   ar_osal_mutex_unlock(servreg_handle_pool.lock);
}

/*
Worker thread for listener notifications
Waits on service locatory and registry pending nodes once the signal is received
Waits forever on the listener events, serves the callbacks once the event is received
*/
void servreg_listener_workerthread(void *param)
{
   /*ackowledge to the thread creator that thread is created*/
   ar_osal_signal2_create(&servreg_handle_pool.servreg_wait_all_status_signal);
   ar_osal_signal2_set(listner_worker_thread_ack, LISTENER_WORKER_THREAD_ACK);

   uint32_t status = AR_EOK, service_state = 0, rcved_signal, each_node = 0; // better to have an event with unint
   ar_osal_servreg_state_notify_payload_type notify_data;
   service_node_type *                       my_node        = NULL;
   uint32_t                                  transaction_id = 0;

   /*wait infinitely*/
   for (;;)
   {
      AR_MSG(DBG_HIGH_PRIO, "Thread created and waiting for events");
      /*Wait for the signal events, all the signals received from the tms or landed here*/
      rcved_signal = (uint32_t)ar_osal_signal2_wait_any(servreg_handle_pool.servreg_wait_all_status_signal, 0xFFFFFFFF);
      AR_MSG(DBG_HIGH_PRIO, "Signal rcved for %d", rcved_signal);

      /*lock*/
      ar_osal_mutex_lock(servreg_handle_pool.lock);

      /* Waits on servregistry locatory availability*/
      /* Signal is received when service locatory is available from corebsp*/
      /* Once the signal is set, process all the unregistered nodes which are queued at the time of
       * ar_osal_service_register*/
      if (SERVREG_SIG_LOC_AVAILABLE & rcved_signal)
      {
         AR_MSG(DBG_HIGH_PRIO, "Service locatory signal rcved");
         ar_osal_signal2_clear(servreg_handle_pool.servreg_wait_all_status_signal, SERVREG_SIG_LOC_AVAILABLE);
         servreg_handle_pool.is_servreg_available = TRUE;

         /*Search for unregistered nodes and register with corebsp*/
         for (each_node = 0; each_node < SERVICE_NODE_REGISTER_COUNT; each_node++)
         {
            my_node = servreg_handle_pool.node[each_node];
            if (NULL == my_node)
            {
               continue;
            }

            /*Only Listeners are allowed/should to register*/
            if ((FALSE == my_node->is_service) && (FALSE == my_node->is_servreg_registered))
            {
               #ifndef SIM
                  MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Service name in thread %s ", my_node->service.name);
                  MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Domain name in thread  %s ", my_node->service.name);
               #endif
               
               status = ar_osal_register_listener_internal(my_node);
               if (AR_EOK != status)
               {
                  AR_MSG(DBG_HIGH_PRIO,
                         "ar_osal_register_listener_internal failed for node(%d) and status (%d)",
                         each_node,
                         status);
                  #ifndef SIM
                    MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Failed Service name in thread %s ", my_node->service.name);
                    MSG_SPRINTF_1(MSG_SSID_QDSP6,MSG_LEGACY_ERROR,"Failed Domain name in thread  %s ", my_node->service.name);
                  #endif
                  
                  /*When the locatory is not available, there is no way to intimate to caller on the sucess or failure*/
                  /*Intimate the failure using the cb func registered*/
                  ar_mem_cpy(&notify_data.service.name,
                             sizeof(my_node->service.name),
                             &my_node->service.name,
                             sizeof(my_node->service.name));
                  ar_mem_cpy(&notify_data.domain.name,
                             sizeof(my_node->domain.name),
                             &my_node->domain.name,
                             sizeof(my_node->domain.name));
                  notify_data.service_state = -1; /*Find better error code*/
                  my_node->cb_func(my_node->srv_handle, NULL, my_node->cb_context, &notify_data, sizeof(notify_data));
               }
            }
         }
      }

      /* Waits for listeners events*/
      for (each_node = 0; each_node < SERVICE_NODE_REGISTER_COUNT; each_node++)
      {
         my_node = servreg_handle_pool.node[each_node];
         if (NULL == my_node)
         {
            continue;
         }

         if (my_node->signal_mask & rcved_signal)
         {
            ar_osal_signal2_clear(servreg_handle_pool.servreg_wait_all_status_signal, my_node->signal_mask);

            /*Fetch the state of the service*/
            service_state = servreg_get_service_curr_state(my_node->srv_handle);
            AR_MSG(DBG_HIGH_PRIO, "Node (%d) got the event state (%d)", each_node, service_state);
            /* Making data ready to trigger call back */
            ar_mem_cpy(&notify_data.service.name,
                       sizeof(my_node->service.name),
                       &my_node->service.name,
                       sizeof(my_node->service.name));
            ar_mem_cpy(&notify_data.domain.name,
                       sizeof(my_node->domain.name),
                       &my_node->domain.name,
                       sizeof(my_node->domain.name));
            notify_data.service_state = (ar_osal_service_state_type)map_service_event_state(service_state);
            my_node->srv_state        = notify_data.service_state;
            /* Callback notified*/
            my_node->cb_func(my_node->srv_handle, NULL, my_node->cb_context, &notify_data, sizeof(notify_data));

            /* Get txn id and set the ack */
            transaction_id = servreg_get_transaction_id(my_node->srv_handle);
            status         = servreg_set_ack(my_node->srv_handle, transaction_id);
            AR_MSG(DBG_HIGH_PRIO, "setting ack is fine (%d) - %d", status, transaction_id);
            if (AR_EOK != status)
            {
               AR_MSG(DBG_HIGH_PRIO, "Sending ack failed with status(%d) for the node (%d)", status, each_node);
            }
         }
      }

      /*unlock*/
      ar_osal_mutex_unlock(servreg_handle_pool.lock);

      /* Kill the thread */
      if (WORKER_THR_KILL_SIG & rcved_signal)
      {
         ar_osal_signal2_destroy(listner_worker_thread_ack);
         break;
      }
   }

   /*Destory the signals*/
   ar_osal_signal2_destroy(servreg_handle_pool.servreg_wait_all_status_signal);
   AR_MSG(DBG_HIGH_PRIO, "Worker thread exiting with status %d", status);
   return;
}

/****************************************************************************************/
/*************************           PUBLIC API's 			      ***********************/
/****************************************************************************************/

/**
* \brief ar_osal_servreg_init
*        Initialize servreg interface.
*........Note:This API has to be called before any other API in this interface.
*........Should be called at least once and is expected to be serialized if called
*        multiple times.
* \return
*  0 -- Success
*  Nonzero -- Failure
*/

int32_t ar_osal_servreg_init(void)
{
   int32_t  status     = AR_EOK;
   uint32_t signal_ack = 0;
   char     name[16];
   strlcpy(name, "AR_SERVREG_TH", sizeof(name));
   ar_osal_thread_attr_t attr;
   ar_osal_thread_attr_init(&attr);

   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_init in progress");

   /*Ensure ar_osal_init is called only once*/
   if (g_init_done)
   {
      return status;
   }
   g_init_done = 1;

   /*Create the array pool to store the service nodes info*/
   servreg_handle_pool.node =
      (service_node_type **)ar_heap_malloc(SERVICE_NODE_REGISTER_COUNT * sizeof(service_node_type *), &heap_info);
   // servreg_handle_pool.node = (service_node_type**)malloc(SERVICE_NODE_REGISTER_COUNT * sizeof(service_node_type*));
   ar_mem_set(servreg_handle_pool.node, 0, (SERVICE_NODE_REGISTER_COUNT * sizeof(service_node_type *)));

   if (NULL == servreg_handle_pool.node)
   {
      status = AR_ENOMEMORY;
      AR_MSG(DBG_HIGH_PRIO, "malloc failure %ld", status);
      return status;
   }

   servreg_handle_pool.num_nodes = 0;
   status                        = ar_osal_mutex_create(&servreg_handle_pool.lock);
   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "Mutex creation failed with status: %ld", status);
      return status;
   }

   // Update the thread details.
   attr.thread_name = name;
   attr.stack_size  = AR_SERVREG_THR_STACK_SIZE;
   attr.priority    = AR_SERVREG_THR_PRIO;

   // Create the signals needed
   ar_osal_signal2_create(&listner_worker_thread_ack);
   servreg_handle_pool.is_servreg_available = FALSE;

   /*Creation of worker thread to serve the events*/
   status = ar_osal_thread_create(&servreg_handle_pool.tid, &attr, servreg_listener_workerthread, NULL);

   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "servreg worker thread creation failed: (%ld)", status);
      return status;
   }

   /*Ensuring the thread is created, block till the thread is created and signalled */
   signal_ack = (uint32_t)ar_osal_signal2_wait_any(listner_worker_thread_ack, LISTENER_WORKER_THREAD_ACK);
   
   if (LISTENER_WORKER_THREAD_ACK & signal_ack)
   {
       AR_MSG(DBG_HIGH_PRIO, "listener thread created sucessfully, clearing the signal");
       ar_osal_signal2_clear(listner_worker_thread_ack, LISTENER_WORKER_THREAD_ACK);
   }
   
   /* Register for service locatory availability
      When service locatory is available cb is triggered*/
   status = servreg_register_servloc_availability_cb(servreg_locator_available, NULL, NULL);

   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "Failed to register for locator availability (%ld)", status);
      return status;
   }

   /* Service locatory won't get invoked, if already service locatory is available
   To over come this, we will check the status of the locatory immediately*/
   if (SERVREG_SERVICE_STATE_UP == servreg_get_servloc_availability())
   {
      AR_MSG(DBG_HIGH_PRIO, "Service locatory is available");
      servreg_locator_available(0, 0);
   }

   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_init completd with status(0x%lx)", status);
   return status;
}

/**
* \brief ar_osal_servreg_deinit
*        Uninitialize servreg interface.
*........Should be called in pair with ar_osal_servreg_init() and
*........should be a serialized call.
* \return
*  0 -- Success
*  Nonzero -- Failure
*/

int32_t ar_osal_servreg_deinit(void)
{
   uint32_t           status = AR_EOK, i = 0;
   service_node_type *node = NULL;
   ar_heap_info       heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   g_init_done = 0;

   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_deinit in progress");

   /* Deregister/free handles from the TMS/corebsp*/
   for (i = 0; i < SERVICE_NODE_REGISTER_COUNT; i++)
   {
      node = servreg_handle_pool.node[i];
      if (NULL != node)
      {
         status = ar_osal_servreg_deregister(node->srv_handle);
         if (status != AR_EOK)
         {
            AR_MSG(DBG_HIGH_PRIO, "Deregister for the node (%ld) failed with status", i, status);
         }
      }
   }

   /*Kill the thread worker thread*/
   ar_osal_signal2_set(servreg_handle_pool.servreg_wait_all_status_signal, WORKER_THR_KILL_SIG);
   ar_osal_thread_join_destroy(servreg_handle_pool.tid);

   /*Free all the service nodes info*/
   ar_heap_free(servreg_handle_pool.node, &heap_info);
   ar_osal_mutex_destroy(servreg_handle_pool.lock);

   AR_MSG(DBG_HIGH_PRIO, "ar osal deinit exited with status %ld", status);
   return AR_EOK;
}

/**
* \brief ar_osal_servreg_get_domainlist
*        Client to call this API to get a list of domains(msm/domain/subdomain) on which
*        a given service(provider/service) is supported.
*
* \param[in]  service: service(provider/service) for which domain(s) list is required.
* \param[in out] domain_list: service supported in domain(s), client to provide
*.                            payload buffer pointer.
* \param[in out] num_domains: Client to provide the num_domains to get the domain list.
*.                            Input NULL domain_list to get the number of domains
*                             for the given service if available in num_domains.
*
* \return
*  0 -- Success
*  Nonzero -- Failure
*  AR_ENOMEMORY- Failed due to insufficient memory, client to call the API again
*                  with required size as returned in num_domains.
*.
*/

#ifdef SIM
int32_t ar_osal_servreg_get_domainlist(ar_osal_servreg_entry_type *service,
                                       ar_osal_servreg_entry_type *domain_list,
                                       uint32_t *                  num_domains)
{
   uint32_t          status = AR_EOK;

   if (NULL == service || NULL == num_domains)
   {
      status = AR_EBADPARAM;
      return status;
   }

   /*Return number of domains for the given service.*/
   if (NULL == domain_list)
   {
      *num_domains = 2;
   }
   else
   {
      ar_strcpy(domain_list[0].name, AR_OSAL_SERVREG_NAME_LENGTH_MAX, "msm/adsp/audio_pd", sizeof("msm/adsp/audio_pd"));

      ar_strcpy(domain_list[1].name,
                AR_OSAL_SERVREG_NAME_LENGTH_MAX,
                "msm/modem/audio_pd",
                sizeof("msm/modem/audio_pd"));
   }
   return status;
}
#else
int32_t ar_osal_servreg_get_domainlist(ar_osal_servreg_entry_type *service,
                                       ar_osal_servreg_entry_type *domain_list,
                                       uint32_t *                  num_domains)
{
   uint32_t          status = AR_EOK, i = 0;
   char_t *          domain_name;
   SERVREG_DL_HANDLE servreg_handle;

   if (NULL == service || NULL == num_domains)
   {
      status = AR_EBADPARAM;
      return status;
   }

   /*Return number of domains for the given service.*/
   if (NULL == domain_list)
   {
      servreg_handle = servreg_alloc_domainlist_handle();
      if (NULL == servreg_handle)
      {
         status = AR_EFAILED;
         AR_MSG(DBG_HIGH_PRIO, "alloc domain list handle failed");
         goto bailout;
      }
      /* get domain list now with the registered handle*/
      status = servreg_get_domainlist(service->name, servreg_handle);
      if (AR_EOK == status)
      {
         *num_domains = servreg_get_num_entries(servreg_handle);
         AR_MSG(DBG_HIGH_PRIO, "ar_get_domainlist queried domains : %d", *num_domains);
      }
      status = servreg_free_domainlist_handle(servreg_handle);
      if (AR_EOK != status)
      {
         status = AR_EFAILED;
         AR_MSG(DBG_HIGH_PRIO, "ar_get_domainlist freeing failed for service(%s) status(%d))", service->name, status);
      }
      goto bailout;
   }
   else
   { /* when domain_list is not empty, fill up the domains to buffer */

      servreg_handle = servreg_alloc_domainlist_handle();
      if (NULL == servreg_handle)
      {
         status = AR_EFAILED;
         goto bailout;
      }
      status = servreg_get_domainlist(service->name, servreg_handle);
      if (AR_EOK == status)
      {
         for (i = 0; i < servreg_get_num_entries(servreg_handle); i++)
         {
            status = servreg_get_entry(servreg_handle, i + 1, &domain_name);
            AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_get_domainlist getting entry for entry:%d", i + 1);
            if ((AR_EOK != status) || (NULL == domain_name))
            {
               status = AR_EFAILED;
               break;
            }
            ar_mem_cpy(domain_list[i].name, strlen(domain_name), domain_name, strlen(domain_name));
         }
      }

      if (AR_EOK != servreg_free_domainlist_handle(servreg_handle))
      {
         status = AR_EFAILED;
         goto bailout;
      }
      goto bailout;
   }
bailout:
   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_get_domainlist exit with status(%d) ", status, 0, 0);
   return status;
}
#endif
/**
* \brief ar_osal_servreg_register
*        Service client(s) to register for the domain service state change notifications.
*
* \param[in opt]  cb_func: callback function pointer to get notifications on.
*.                         This is parameter is optional to Service provider registration.
* \param[in opt]  cb_context: callback function payload/context provided by client.
*.                         This is parameter is optional to Service provider registration.
* \param[in]  domain: domain of the service(msm/domain/subdomain) for which the
*.            state change notifications to be provided.
* \param[in]  service: service(provider/service) for which the
*.            state change notifications to be provided.
* \return
*  servreg_handle on success.
*  null on failure.
*/
ar_osal_servreg_t ar_osal_servreg_register(ar_osal_client_type         client_type,
                                           ar_osal_servreg_callback    cb_func,
                                           void *                      cb_context,
                                           ar_osal_servreg_entry_type *domain,
                                           ar_osal_servreg_entry_type *service)
{
   uint32_t           status = AR_EOK, num_nodes_mask = 0, get_node_num = 0;
   char_t *           domain_name;
   ar_osal_servreg_t  servreg_handle = NULL;
   service_node_type *new_node       = NULL;
   ar_heap_info       heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   if (NULL == service)
   {
      status = AR_EBADPARAM;
      goto bailout;
   }

   /*Service owner registration*/
   if (AR_OSAL_CLIENT_SERVICE_PROVIDER == client_type)
   {
      AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_register as a service provider");
      /*Create a new node for every service provider*/
      // new_node = malloc(sizeof(service_node_type));//move to ar malloc
      new_node = ar_heap_malloc(sizeof(service_node_type), &heap_info);
      if (NULL == new_node)
      {
         status = AR_ENOMEMORY;
         goto bailout;
      }
      ar_mem_set(new_node, 0, sizeof(service_node_type));

      /*lock to ensure thread safety*/
      ar_osal_mutex_lock(servreg_handle_pool.lock);

      /*Store the new node in the service pool, identify the node slot*/
      if (NULL == get_available_node(&get_node_num))
      {
         AR_MSG(DBG_HIGH_PRIO, "No space for new registrations");
         ar_osal_mutex_unlock(servreg_handle_pool.lock);
         ar_heap_free(new_node, &heap_info);
         goto bailout;
      }
      servreg_handle_pool.node[get_node_num] = new_node;
      ar_osal_mutex_unlock(servreg_handle_pool.lock);

      /*Store the user provided info in the new node*/
      new_node->is_service = TRUE;
      new_node->srv_state  = AR_OSAL_SERVICE_STATE_DOWN; // Should not it be UNINT
      ar_mem_cpy(&new_node->service.name, sizeof(service->name), service->name, sizeof(service->name));
      if (NULL == domain) /* If domain is NULL means register to default domain.*/
      {
         domain_name = servreg_get_local_domain();
         ar_mem_cpy(&new_node->domain.name, strlen(domain_name), domain_name, strlen(domain_name));
         AR_MSG(DBG_HIGH_PRIO, "Register to local domain : %s", domain_name);
         AR_MSG(DBG_HIGH_PRIO, "Node is registered at : %d", get_node_num);
      }
      else
      {
         ar_mem_cpy(&new_node->domain.name, sizeof(domain->name), domain->name, sizeof(domain->name));
         AR_MSG(DBG_HIGH_PRIO, "Domain provided by user: %s", domain->name);
      }

      /*allocate a monitor handle*/
      new_node->srv_handle = servreg_alloc_monitor_handle(new_node->domain.name, new_node->service.name);

      // Set register handle to high if allocation is successful.
      // In case of any previous setup failures, registry will not happen.
      if (NULL == new_node->srv_handle)
      {
         AR_MSG(DBG_HIGH_PRIO, "monitor allocation failed for service %s: %ld", service->name, status);
         status = AR_EFAILED;
         goto bailout;
      }

      servreg_handle = new_node;
   }

   if (AR_OSAL_CLIENT_LISTENER == client_type)
   {
      /* Listener registration*/
      AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_register as listener");

      /*create a node and fill the details*/
      // new_node = malloc(sizeof(service_node_type));
      new_node = ar_heap_malloc(sizeof(service_node_type), &heap_info);
      if (NULL == new_node)
      {
         status = AR_ENOMEMORY;
         goto bailout;
      }
      ar_mem_set(new_node, 0, sizeof(service_node_type));

      /*lock for thread safety*/
      ar_osal_mutex_lock(servreg_handle_pool.lock);
      if (NULL == get_available_node(&get_node_num))
      {
         AR_MSG(DBG_HIGH_PRIO, "No space for new registrations");
         ar_osal_mutex_unlock(servreg_handle_pool.lock);
         ar_heap_free(new_node, &heap_info);
         goto bailout;
      }
      num_nodes_mask = get_node_num;

      /*Store the user provided info in the new node*/
      new_node->is_service = FALSE;
      new_node->srv_state  = AR_OSAL_SERVICE_STATE_DOWN; /*Should change to UNINT*/
      ar_mem_cpy(&new_node->service.name, sizeof(service->name), service->name, sizeof(service->name));
      ar_mem_cpy(&new_node->domain.name, sizeof(domain->name), domain->name, sizeof(domain->name));
      new_node->signal_mask           = (1 << (UNUSED_MASK_TOKEN + num_nodes_mask));
      new_node->is_servreg_registered = FALSE;
      new_node->cb_func               = cb_func;

      if (NULL != cb_context)
      {
         new_node->cb_context = cb_context;
      }

      servreg_handle_pool.node[num_nodes_mask] = new_node;
      ar_osal_mutex_unlock(servreg_handle_pool.lock);

      /*If servreg is available, we will register directly directly with corebsp tms */
      /*If service is not avaialble, we will queue the node to the thread*/
      /*Thread will register the queued nodes with corebsp tms once the signal is avaialble*/
      AR_MSG(DBG_HIGH_PRIO, "service checking now");
      if (TRUE == servreg_handle_pool.is_servreg_available)
      {
         AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_register register as listener signal available");
         /* Register with corebsp tms */
         status = ar_osal_register_listener_internal(new_node);
         if (AR_EOK != status)
         {
            AR_MSG(DBG_HIGH_PRIO, "Registering to corebsp tms failed with status %d", status);
            ar_heap_free(new_node, &heap_info);
            goto bailout;
         }
      }

      servreg_handle = new_node;
   }

bailout:
   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_register for exit with status %d", status);
   return servreg_handle;
}

/**
* \brief ar_osal_servreg_deregister
*        Service client(s) to deregister for the service state change notifications.
*
* \param[in]  servreg_handle: interface handle returned by ar_osal_servreg_allocate_handle().
*
* \return
*  0 -- Success
*  Nonzero -- Failure
*/

int32_t ar_osal_servreg_deregister(ar_osal_servreg_t servreg_handle)
{
   uint32_t           status = AR_EOK, node_number = 0;
   service_node_type *my_node = NULL;
   my_node                    = (service_node_type *)servreg_handle;
   ar_heap_info heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_deregister processing");

   if (NULL == servreg_handle)
   {
      return AR_EBADPARAM;
   }

   /*lock and remove the my_node*/
   ar_osal_mutex_lock(servreg_handle_pool.lock);
   status = is_node_linked(servreg_handle, &node_number);
   if (AR_EOK != status)
   {
      ar_osal_mutex_unlock(servreg_handle_pool.lock);
      return status;
   }

   /*unlink the node to avoid locking for long time*/
   servreg_handle_pool.node[node_number] = NULL;
   ar_osal_mutex_unlock(servreg_handle_pool.lock);

   /*Set the service state to down */
   if (TRUE == my_node->is_service)
   {
      status =
         servreg_set_state(servreg_handle, (SERVREG_SERVICE_STATE)map_service_event_state(AR_OSAL_SERVICE_STATE_DOWN));
      if (AR_EOK != status)
      {
         AR_MSG(DBG_HIGH_PRIO, "Failed to set the servreg state %ld", status);
      }
   }

   /*Free the monitor handle irrespective of failures*/
   status = servreg_free_monitor_handle(servreg_handle);
   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "Failed to free monitor handle %ld", status);
   }
   ar_heap_free(servreg_handle, &heap_info);

   AR_MSG(DBG_HIGH_PRIO, "ar_osal_servreg_deregister handle(0x%pK) status(0x%ld)", servreg_handle, status);
   return status;
}

/**
* \brief ar_osal_servreg_set_state
*        Service provider to call this API to register its service states(UP/DOWN).
*        This API to be used only by the service provider(msm/domain/subdomain/provider/service)
*        and not by service client(s).
*
* \param[in]  servreg_handle: interface handle returned by ar_osal_servreg_allocate_handle().
* \param[in]  state: new service state for service registered using ar_osal_servreg_register().
*.
* \return
*  0 -- Success
*  Nonzero -- Failure
*/
int32_t ar_osal_servreg_set_state(ar_osal_servreg_t servreg_handle, ar_osal_service_state_type state)
{
   int32_t            status = AR_EOK;
   uint32_t           node_number;
   service_node_type *my_node = NULL;
   if (NULL == servreg_handle)
   {
      return AR_EBADPARAM;
   }

   ar_osal_mutex_lock(servreg_handle_pool.lock);
   status = is_node_linked(servreg_handle, &node_number);
   if (status != AR_EOK)
   {
      AR_MSG(DBG_HIGH_PRIO, "node does not exist %ld", status);
      ar_osal_mutex_unlock(servreg_handle_pool.lock);
      return status;
   }
   my_node = (service_node_type *)servreg_handle_pool.node[node_number];
   ar_osal_mutex_unlock(servreg_handle_pool.lock);

   /*convert the states according to the service registry api*/
   AR_MSG(DBG_HIGH_PRIO, "Updating the service state %ld", state);
   status = servreg_set_state(my_node->srv_handle, (SERVREG_SERVICE_STATE)map_service_event_state(state));
   if (AR_EOK != status)
   {
      AR_MSG(DBG_HIGH_PRIO, "Failed to set the servreg state %ld", state);
      servreg_free_monitor_handle(servreg_handle);
      status = AR_EFAILED;
   }
   else
   {
      /*Update the node details only if the set state is true.*/
      my_node->srv_state  = state;
      my_node->is_service = TRUE;
   }

   AR_MSG(DBG_HIGH_PRIO,
          "ar_osal_servreg_set_state state(%d) handle(0x%pK) status(0x%ld)",
          state,
          servreg_handle,
          status);
   return status;
}