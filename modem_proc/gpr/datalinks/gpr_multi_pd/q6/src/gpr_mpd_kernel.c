/*========================================================================
*/ /** @file gpr_mpd_kernel.c
This file contains the implementations of gpr mpd kernel

Copyright (c) 2018-2020,2022 Qualcomm Technologies, Inc. and/or its subsidiaries.
All Rights Reserved.  
Qualcomm Technologies Proprietary and Confidential.
*/ /*====================================================================== */

/*========================================================================
Edit History

when       who    what, where, why
--------  -----     -------------------------------------------------------
04/11/18   np      Created file.
========================================================================== */

/******************************************************************************
 * Includes                                                                    *
 *****************************************************************************/
#include "qurt.h"
#include "qurt_qdi_driver.h"
#include "gpr_api_inline.h"
#include "gpr_list.h"
#include "gpr_mpd.h"
#include "gpr_mpd_kernel_i.h"
#include "ar_osal_mutex.h"
#include "ar_osal_mem_op.h"
#include "ar_osal_heap.h"
#include "ar_msg.h"
#include "servreg_locator_qurt.h"
#include "servreg_monitor_qurt.h"
#include "ar_osal_servreg.h"

#ifndef SIM
#define PDR_FEATURE 1
#endif

/*****************************************************************************
 * Defines                                                                   *
 ****************************************************************************/
/* Max. no. of uGPRs allowed */
#define GPR_MPD_MAX_UGPRS 10

/* Total number of packets */
#define GPR_MPD_NUM_PACKETS 5 * GPR_MPD_MAX_UGPRS

/*****************************************************************************
 * Data Structures                                                           *
 ****************************************************************************/
ar_osal_service_state_type upd_status;
ar_osal_servreg_t          ar_servreg_handle;

typedef struct gpr_mpd_packet_item_s
{
   gpr_list_node_t link;
   gpr_packet_t *  packet;
} gpr_mpd_packet_item_t;

struct gpr_mpd_ugpr_item_s;

typedef struct gpr_mpd_ugpr_s
{
   /* QDI framework related fields */
   qurt_qdi_obj_t qdiobj;
   int            client_handle; /* Client handle for QDI heap reference */

   /* Driver defined fields */
   struct gpr_mpd_ugpr_item_s *nodeptr;     /* Ptr to its container item */
   qurt_signal_t *             rx_signal;   /* To signal uGPR about incoming rx packet */
   uint8_t                     domain_id;   /* Assuming only one domain id per uGPR */
   gpr_list_t                  rx_packet_q; /* Rx packet queue for the uGPR */
} gpr_mpd_ugpr_t;

typedef struct gpr_mpd_ugpr_item_s
{
   gpr_list_node_t link;
   gpr_mpd_ugpr_t  ugpr;
} gpr_mpd_ugpr_item_t;

typedef struct gpr_mpd_qdi_opener_s
{
   qurt_qdi_obj_t qdiobj;
} gpr_mpd_qdi_opener;

/*****************************************************************************
 * Declarations                                                              *
 ****************************************************************************/

static int gpr_mpd_qdi_invocation(int             client_handle,
                                  qurt_qdi_obj_t *obj,
                                  int             method,
                                  qurt_qdi_arg_t  a1,
                                  qurt_qdi_arg_t  a2,
                                  qurt_qdi_arg_t  a3,
                                  qurt_qdi_arg_t  a4,
                                  qurt_qdi_arg_t  a5,
                                  qurt_qdi_arg_t  a6,
                                  qurt_qdi_arg_t  a7,
                                  qurt_qdi_arg_t  a8,
                                  qurt_qdi_arg_t  a9);

/* Function table for kernel mpd dl */
static ipc_to_gpr_vtbl_t mpd_kernel_vtbl = { gpr_mpd_send, NULL };

static void gpr_mpd_qdi_release(qurt_qdi_obj_t *qdiobj);
static uint32_t gpr_mpd_allocate_ugpr(gpr_mpd_ugpr_t **ppugpr);
static uint32_t gpr_mpd_free_ugpr(gpr_mpd_ugpr_t *pugpr);
static uint32_t gpr_mpd_free_packet_item(gpr_mpd_packet_item_t *ppacket_item);
static uint32_t gpr_mpd_allocate_packet_item(gpr_mpd_packet_item_t **pppacket_item);

/*****************************************************************************
 * Variables                                                                 *
 ****************************************************************************/

/* Data Definitions */
static gpr_mpd_qdi_opener gpr_mpd_opener = { { gpr_mpd_qdi_invocation, QDI_REFCNT_PERM, NULL } };

static ar_osal_mutex_t       gpr_mpd_packets_lock;
static gpr_mpd_packet_item_t gpr_mpd_packets[GPR_MPD_NUM_PACKETS];
static gpr_list_t            gpr_mpd_free_packet_q;

static gpr_mpd_ugpr_item_t gpr_mpd_ugprs[GPR_MPD_MAX_UGPRS];
static gpr_list_t          gpr_mpd_used_ugpr_q;
static gpr_list_t          gpr_mpd_free_ugpr_q;

static bool_t kernel_init_done = FALSE;
/*****************************************************************************
 * Implementations                                                           *
 ****************************************************************************/
/*@brief MPD kernel init
  @param[in] domain_id          Domain id of destination
  @param[in] gpr_to_ipc_vtbl_t  Pointer to function table sent from GPR to mpd user layers
  @param[in] ipc_to_gpr_vtbl_t  Double pointer to function table to be populated by mpd user layer

  @return
  #AR_EOK when successful.
*/
GPR_INTERNAL uint32_t ipc_dl_mpd_kernel_init(uint32_t                 src_domain_id,
                                             uint32_t                 dest_domain_id,
                                             const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                             ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl)
{
   uint32_t rc = AR_EOK;

   *pp_ipc_to_gpr_vtbl = &mpd_kernel_vtbl;
   if (!kernel_init_done)
   {
      rc = gpr_mpd_init();
      if (AR_EOK == rc)
      {
         kernel_init_done = TRUE;
      }
   }
   return rc;
}

/*@brief MPD kernel de-init
  @param[in] domain_id  Domain id

  @return
  #AR_EOK when successful.
*/
GPR_INTERNAL uint32_t ipc_dl_mpd_kernel_deinit(uint32_t src_domain_id, uint32_t dest_domain_id)
{
   uint32_t rc = AR_EOK;
   if (kernel_init_done)
   {
      rc = gpr_mpd_deinit(dest_domain_id);
      if (AR_EOK == rc)
      {
         kernel_init_done = FALSE;
      }
   }
   return rc;
}

static void gpr_mpd_isr_lock_fn(void)
{
   ar_osal_mutex_lock(gpr_mpd_packets_lock);
}

static void gpr_mpd_isr_unlock_fn(void)
{
   ar_osal_mutex_unlock(gpr_mpd_packets_lock);
}

static uint32_t gpr_ugpr_free_pkts(gpr_mpd_ugpr_t *pugpr)
{
   int32_t                ret = AR_EOK;
   gpr_packet_t *         packet;
   gpr_mpd_packet_item_t *ppacket_item;

   while (pugpr->rx_packet_q.size > 0)
   {
      AR_MSG(DBG_HIGH_PRIO, "Clearing packets queue_size=%lu", pugpr->rx_packet_q.size);
      gpr_list_remove_head(&pugpr->rx_packet_q, (gpr_list_node_t **)&ppacket_item);
      packet = ppacket_item->packet;

      /* Inform sender about packet delivery failure*/
      ret = __gpr_cmd_end_command(packet, AR_ENOTEXIST);
      if (AR_EOK != ret)
      {
         // remove this as accessing a pointer after a pointer is freed is not safe..
         AR_MSG(DBG_ERROR_PRIO, "Failed to send END CMD to dst=0x%lu ret=0x%lu", packet->dst_port, ret);
      }
      ppacket_item->packet = NULL;
      gpr_mpd_free_packet_item(ppacket_item);
   }
   return ret;
}

GPR_INTERNAL uint32_t gpr_mpd_init(void)
{
   uint32_t ret = AR_EOK, rc = AR_EOK;
   uint32_t index;

   /* Initializing the pre-allocated pool of packets  */
   ret = ar_osal_mutex_create((ar_osal_mutex_t *)&gpr_mpd_packets_lock);
   if (ret)
   {
      AR_MSG(DBG_ERROR_PRIO, "failed during mutex create");
   }

   gpr_list_init_v2(&gpr_mpd_free_packet_q, gpr_mpd_isr_lock_fn, gpr_mpd_isr_unlock_fn);

   for (index = 0; index < GPR_MPD_NUM_PACKETS; index++)
   {
      rc = gpr_list_add_tail(&gpr_mpd_free_packet_q, &gpr_mpd_packets[index].link);
      if (AR_EOK != rc)
      {
         AR_MSG(DBG_ERROR_PRIO,
                "GPR mpd: Adding nodes to mpd free packet queue failed, mpd_init failed rc %d",
                rc);
		 return rc;
      }
   }

   /*  Initializing the pre-allocated array of gpr_mpd_ugpr_t objects */
   gpr_list_init_v2(&gpr_mpd_used_ugpr_q, gpr_mpd_isr_lock_fn, gpr_mpd_isr_unlock_fn);
   gpr_list_init_v2(&gpr_mpd_free_ugpr_q, gpr_mpd_isr_lock_fn, gpr_mpd_isr_unlock_fn);

   for (index = 0; index < GPR_MPD_MAX_UGPRS; index++)
   {
      gpr_list_init_v2(&gpr_mpd_ugprs[index].ugpr.rx_packet_q, gpr_mpd_isr_lock_fn, gpr_mpd_isr_unlock_fn);
      rc = gpr_list_add_tail(&gpr_mpd_free_ugpr_q, &gpr_mpd_ugprs[index].link);
      if (AR_EOK != rc)
      {
         AR_MSG(DBG_ERROR_PRIO,
                "GPR mpd: Adding nodes to free ugpr queue failed, mpd_init failed rc %d",
                rc);
	     return rc;
      }
   }

   /* Registering GPR MPD DeviceName with QuRT */
   if (qurt_qdi_devname_register(GPR_MPD_DEVICE_NAME, &gpr_mpd_opener.qdiobj))
   {
      AR_MSG(DBG_ERROR_PRIO, "Failed to register GPR MPD Device Name");
      ret = AR_EFAILED;
   }

#ifdef PDR_FEATURE
   uint32_t ret_pdr = AR_EOK;
   ret_pdr          = gpr_pdr_init();

   if (ret_pdr != AR_EOK)
   {
      AR_MSG(DBG_ERROR_PRIO, "PD Restart mode failed, disabling pd restart mode..!");
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "PD Restart mode sucessfully enabled..!");
   }
   ret = ret_pdr;
#endif
   return ret;
}

GPR_INTERNAL uint32_t gpr_mpd_deinit(uint32_t domain_id)
{
   uint32_t ret = AR_EOK;

/* TODO: 1. Do the required clean-up as kGPR is going down */
#ifdef PDR_FEATURE
   ret = gpr_pdr_deinit();
#endif
   return ret;
}

/*
 * Send a packet to the dst address specified in the packet. In case
 * of UGPR the packet is sent to Kernel. In-case of KGPR the packet gets
 * sent to the correct destination based on dst ID.
 *
 * Assumption: Ownership of the memory allocated for packet is transferred to
 *             GPR MPD.
 *
 * @param packet gpr packet to send
*/
GPR_INTERNAL uint32_t gpr_mpd_send(uint32_t domain_id, void *packet_, uint32_t packet_len)
{
   /*
    * Based on the book keeping information maintained by kGPR determine
    * which uGPR hosts this service and send the message.
    *
    * Although uGPR informs kGPR on each service registration. Routing of
    * messages would be made solely based on the domain id.
    *
    * Per design, each uGPR shall be assigned one or more domain ids. But, no
    * two uGPRs shall have the same domain id.
    *
   */
   gpr_packet_t *         packet = (gpr_packet_t *)packet_;
   uint16_t               dst_domain_id;
   gpr_list_node_t *      cur_node, *dummy_node;
   gpr_mpd_ugpr_item_t *  cur_ugpr_item;
   gpr_mpd_packet_item_t *ppacket_item;
   int32_t                ret = 0;
   uint32_t               rc  = AR_EOK;

   if (NULL == packet)
   {
      AR_MSG(DBG_ERROR_PRIO, "NULL packet");
      return AR_EBADPARAM;
   }

   dst_domain_id = packet->dst_domain_id;

#ifdef GPR_DEBUG_MSG
   AR_MSG(DBG_HIGH_PRIO,
          "GPR mpd send: packet dst domain: %d, dst port %d opcode 0x%lx token 0x%lx",
          packet->dst_domain_id,
          packet->dst_port,
          packet->opcode,
          packet->token);
#endif

   /* Find if there exists an uGPR with a matching domain ID*/
   dummy_node = &gpr_mpd_used_ugpr_q.dummy;
   cur_node   = dummy_node->next;
   while (cur_node != dummy_node)
   {
      cur_ugpr_item = (gpr_mpd_ugpr_item_t *)cur_node;
      if (dst_domain_id == cur_ugpr_item->ugpr.domain_id)
         break;
      cur_node = cur_node->next;
   }
   if (cur_node == dummy_node)
   {
      /* Looks like UPD has not done registration with kGPR
       * or it is crashed. Therefore, it is required to inform
       * sender about packet delivery failure.*/
      AR_MSG(DBG_HIGH_PRIO, "Destination service=0x%04lX is not available", packet->dst_port);
      ret = __gpr_cmd_end_command(packet, AR_EFAILED /*AR_ENOSERVICE*/);
      if (ret != AR_EOK)
      {
         AR_MSG(DBG_ERROR_PRIO, "Failed to send end command to ret=0x%lX", ret);
      }
      return ret;
   }

#ifdef PDR_FEATURE
   if (upd_status != AR_OSAL_SERVICE_STATE_UP)
   {
      // Invoke the gpr_pd_restart_call here
      // If this block is invoked then we are freeing ugpr obj as well so until pd is up
      // no packets will reach here and will continue to return
      ret = __gpr_cmd_end_command(packet, AR_ENOTEXIST);
      AR_MSG(DBG_HIGH_PRIO, "Sent end command response from pd down state ret=0x%lX   Audio pd is not yet up", ret);
      if (ret != AR_EOK)
      {
         AR_MSG(DBG_ERROR_PRIO, "Failed to send end command to ret=0x%lX", ret);
      }
      return AR_ENOTREADY; /* Command is already freed so sending other than EOK will result in double free as command is
                        already freed */
   }

#endif

   /* Queue the packet to this uGPR and issue a signal to the same.*/
   ret = gpr_mpd_allocate_packet_item(&ppacket_item);
   if (AR_EOK != ret)
   {
      AR_MSG(DBG_ERROR_PRIO, "Unable to allocate memory to received packet ret=%ld", ret);
      return AR_ENOMEMORY;
   }

   ppacket_item->packet = packet;

   /* Adding to the uGPR Rx packet queue*/
   rc = gpr_list_add_tail(&cur_ugpr_item->ugpr.rx_packet_q, (gpr_list_node_t *)ppacket_item);
   if (AR_EOK != rc)
   {
      AR_MSG(DBG_ERROR_PRIO, "GPR mpd: Adding packet to ugpr queue from kgpr failed rc %d", rc);
   }
#ifdef GPR_DEBUG_MSG
   else
   {
      AR_MSG(DBG_HIGH_PRIO,
             "GPR kernel mpd: Add packet with token 0x%lx to ugpr queue, set rx signal %lx",
             ppacket_item->packet->token,
             cur_ugpr_item->ugpr.rx_signal);
   }
#endif

   /* Signal uGPR */
   qurt_signal_set(cur_ugpr_item->ugpr.rx_signal, GPR_MPD_SIG_RX_PACKET);
   return AR_EOK;
}

static int gpr_mpd_qdi_invocation(int             client_handle,
                                  qurt_qdi_obj_t *obj,
                                  int             method,
                                  qurt_qdi_arg_t  a1,
                                  qurt_qdi_arg_t  a2,
                                  qurt_qdi_arg_t  a3,
                                  qurt_qdi_arg_t  a4,
                                  qurt_qdi_arg_t  a5,
                                  qurt_qdi_arg_t  a6,
                                  qurt_qdi_arg_t  a7,
                                  qurt_qdi_arg_t  a8,
                                  qurt_qdi_arg_t  a9)
{
   gpr_mpd_ugpr_t *pugpr = (gpr_mpd_ugpr_t *)obj;
   int             ret   = AR_EOK;
   switch (method)
   {
      case QDI_OPEN:
      {
         ret = gpr_mpd_allocate_ugpr(&pugpr);
         if (AR_EOK != ret)
         {
            AR_MSG(DBG_ERROR_PRIO, "Unable to allocate memory. ret=%d", ret);
            return -1;
         }

         pugpr->qdiobj.invoke  = gpr_mpd_qdi_invocation;
         pugpr->qdiobj.refcnt  = QDI_REFCNT_INIT;
         pugpr->qdiobj.release = gpr_mpd_qdi_release;
         pugpr->client_handle  = client_handle;

         /* Creating signal to inform uGPR on Rx packets*/
         pugpr->rx_signal = (qurt_signal_t *)qurt_qdi_user_malloc(client_handle, sizeof(*pugpr->rx_signal));
         if (NULL == pugpr->rx_signal)
         {
            gpr_mpd_free_ugpr(pugpr);
            AR_MSG(DBG_ERROR_PRIO, "Failed to allocate memory for Rx Signal");
            return -2;
         }
         qurt_signal_init(pugpr->rx_signal);

         /* Create a handle for the object and return*/
         ret = qurt_qdi_handle_create_from_obj_t(client_handle, (qurt_qdi_obj_t *)pugpr);
         if (ret < 0)
         {
            qurt_signal_destroy(pugpr->rx_signal);
            qurt_qdi_user_free(client_handle, pugpr->rx_signal);
            gpr_mpd_free_ugpr(pugpr);
         }
         return ret; /* Returning the handle obtained */
      }

      case GPR_MPD_CMDID_SET_UPD_STATE:
      {
         AR_MSG(DBG_HIGH_PRIO, "Setting the UPD status as UP");
		 if (a2.num < 0)
		 {
			AR_MSG(DBG_ERROR_PRIO, "Invalid domain ID: %d", a2.num);
			return AR_EBADPARAM;
         }
         pugpr->domain_id = a2.num;
         upd_status       = AR_OSAL_SERVICE_STATE_UP;
         return ret;
      }

      case GPR_MPD_CMDID_WAIT_RX_SIGNAL:
      {
         unsigned int      signal_mask = 0;

         AR_MSG(DBG_HIGH_PRIO, "GPR_MPD_CMDID_WAIT_RX_SIGNAL   refcnt=0x%X", pugpr->qdiobj.refcnt);
         ret = qurt_signal_wait_cancellable(pugpr->rx_signal,
                          GPR_MPD_SIG_EXIT | GPR_MPD_SIG_RX_PACKET,
                          QURT_SIGNAL_ATTR_WAIT_ANY, &signal_mask);
         if(ret != QURT_EOK) {
            return ret;
         }
         if(signal_mask & GPR_MPD_SIG_RX_PACKET) {
            qurt_signal_clear(pugpr->rx_signal, GPR_MPD_SIG_RX_PACKET);
         }

        return signal_mask;
         }
	  case GPR_MPD_CMDID_KILL_RX_SIGNAL:
         {
		  qurt_signal_set(pugpr->rx_signal, GPR_MPD_SIG_EXIT);
         return ret;
      }
      case GPR_MPD_CMDID_ASYNC_SEND:
      {
         /* Check if the destination belongs to one of the uGPRs.
          * If so, try to send it to the uGPR. Otherwise, give it to kGPR core */
         gpr_packet_t *packet_copy;
         uint32_t      size;

         if ( a2.num < 0 || a2.num > GPR_MAX_ALLOC_PKT_SIZE )
		 {
			AR_MSG(DBG_ERROR_PRIO, "Invalid packet size: %d", a2.num);
			return AR_EBADPARAM;
         }



         size = a2.num;

        
         ret = __gpr_cmd_alloc(size, &packet_copy);
         if (AR_EOK != ret)
         {
            AR_MSG(DBG_ERROR_PRIO, "Failed to allocate GPR packet. ret=%d", ret);
           return ret;
         }

         ret = qurt_qdi_copy_from_user(client_handle, packet_copy, a1.ptr, size);
         if (0 != ret)
         {
              AR_MSG(DBG_ERROR_PRIO, "cmd send failed ret=%d ", ret);
              return ret;
         }

#ifdef GPR_DEBUG_MSG
            AR_MSG(DBG_HIGH_PRIO,
                   "GPR mpd user async send: packet dst domain: %d, dst port %d",
                   packet_copy->dst_domain_id,
                   packet_copy->dst_port);
#endif

          ret = __gpr_cmd_async_send(packet_copy);
          if (ret)
          {
               __gpr_cmd_free(packet_copy);
          }

         return ret;
      }
      case GPR_MPD_CMDID_PEEK_RX_PACKET:
      {
         gpr_mpd_packet_item_t *packet_item;

         if (pugpr->rx_packet_q.dummy.next == &pugpr->rx_packet_q.dummy)
         {
            return AR_ENOTEXIST;
         }

         packet_item = (gpr_mpd_packet_item_t *)pugpr->rx_packet_q.dummy.next;
         ret = qurt_qdi_copy_to_user(client_handle, a1.ptr, packet_item->packet, sizeof(gpr_packet_t));
         if (0 != ret)
         {
              AR_MSG(DBG_ERROR_PRIO, "packet peek failed ret=%d ", ret);
         }
                
         return ret;
      }
      case GPR_MPD_CMDID_READ_RX_PACKET:
      {
         gpr_packet_t *         packet;
         uint32_t               packet_size;
         gpr_mpd_packet_item_t *ppacket_item;

         if (pugpr->rx_packet_q.dummy.next == &pugpr->rx_packet_q.dummy)
         {
            return AR_ENOTEXIST;
         }

                  
         gpr_list_remove_head(&pugpr->rx_packet_q, (gpr_list_node_t **)&ppacket_item);
         packet      = ppacket_item->packet;
         packet_size = GPR_PKT_GET_PACKET_BYTE_SIZE(packet->header);

         ret = qurt_qdi_copy_to_user(client_handle, a1.ptr, packet, packet_size);
         if (0 != ret)
         {
            AR_MSG(DBG_ERROR_PRIO, "read rx failed ret=%d ", ret);
            return ret;
         }

         __gpr_cmd_free(packet);
         ppacket_item->packet = NULL;
         gpr_mpd_free_packet_item(ppacket_item);
         a3.num = packet_size;
   
         return ret;
      }
      default:
      {
         AR_MSG(DBG_HIGH_PRIO, "Received unknown method=0x%X", method);
         return qurt_qdi_method_default(client_handle, obj, method, a1, a2, a3, a4, a5, a6, a7, a8, a9);
      }
   }
}

static void gpr_mpd_qdi_release(qurt_qdi_obj_t *qdiobj)
{
   AR_MSG(DBG_HIGH_PRIO, "gpr_mpd_qdi_release");
   
   upd_status = AR_OSAL_SERVICE_STATE_DOWN;
   
   ar_osal_mutex_lock(gpr_mpd_packets_lock);
   if (NULL != qdiobj)
   {
      AR_MSG(DBG_HIGH_PRIO, "gpr_mpd_qdi_release_not_null");
      gpr_mpd_ugpr_t *pugpr = (gpr_mpd_ugpr_t *)qdiobj;

      if (NULL != pugpr)
      {
         AR_MSG(DBG_HIGH_PRIO, "refcnt=0x%X, client_handle=0x%X", pugpr->qdiobj.refcnt, pugpr->client_handle);

         /* Destroying and Free Rx signal*/
         AR_MSG(DBG_HIGH_PRIO, "Destroying RX signal");
         qurt_signal_destroy(pugpr->rx_signal);

         AR_MSG(DBG_HIGH_PRIO, "Freeing RX signal handler");
         qurt_qdi_user_free(pugpr->client_handle, pugpr->rx_signal);

         /* Clear Rx packet queue if any*/
         AR_MSG(DBG_HIGH_PRIO, "Total pending packets queue_size=%ld", pugpr->rx_packet_q.size);
         gpr_ugpr_free_pkts(pugpr);

         /* Object cleanup*/
         pugpr->client_handle  = 0;
         pugpr->domain_id      = 0;
         pugpr->qdiobj.invoke  = NULL;
         pugpr->qdiobj.refcnt  = 0;
         pugpr->qdiobj.release = NULL;
         pugpr->rx_signal      = NULL;

         AR_MSG(DBG_HIGH_PRIO, "Freeing uGPR object");

         /* Free UGPR Object*/
         gpr_mpd_free_ugpr(pugpr);
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO, "NULL parameters");
      }
   }
   ar_osal_mutex_unlock(gpr_mpd_packets_lock); // Release the lock before returning
   return;
}

static uint32_t gpr_mpd_allocate_ugpr(gpr_mpd_ugpr_t **ppugpr)
{
   uint32_t             ret = AR_EOK, rc = AR_EOK;
   gpr_mpd_ugpr_item_t *pugpr_item = NULL;

   if (NULL == ppugpr)
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid Parameter");
      return AR_EBADPARAM;
   }

   ret = gpr_list_remove_head(&gpr_mpd_free_ugpr_q, (gpr_list_node_t **)&pugpr_item);
   if (AR_EOK == ret)
   {
      rc = gpr_list_add_tail(&gpr_mpd_used_ugpr_q, &pugpr_item->link);
      if (AR_EOK != rc)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR mpd kernel: Adding node to used ugpr queue failed rc %d", rc);
      }

      pugpr_item->ugpr.nodeptr = (struct gpr_mpd_ugpr_item_s *)pugpr_item;
      *ppugpr                  = &pugpr_item->ugpr;
   }
   return ret;
}

static uint32_t gpr_mpd_free_ugpr(gpr_mpd_ugpr_t *pugpr)
{
   uint32_t             ret = AR_EOK, rc = AR_EOK;
   gpr_mpd_ugpr_item_t *pugpr_item = NULL;

   if (NULL == pugpr)
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid Parameter");
      return AR_EBADPARAM;
   }

   pugpr_item = (gpr_mpd_ugpr_item_t *)pugpr->nodeptr;

   ret = gpr_list_delete(&gpr_mpd_used_ugpr_q, (gpr_list_node_t *)pugpr_item);
   if (AR_EOK == ret)
   {
      rc = gpr_list_add_tail(&gpr_mpd_free_ugpr_q, &pugpr_item->link);
      if (AR_EOK != rc)
      {
         AR_MSG(DBG_ERROR_PRIO,
                "GPR mpd upd: Freeing packet failed, packet node is corrupted or has already been freed rc %d",
                rc);
      }
   }
#ifdef GPR_DEBUG_MSG
   AR_MSG(DBG_HIGH_PRIO,
          "gpr_mpd_free_ugpr used_list_size=%lu    free_list_size=%lu",
          gpr_mpd_used_ugpr_q.size,
          gpr_mpd_free_ugpr_q.size);
#endif
   return ret;
}

static uint32_t gpr_mpd_allocate_packet_item(gpr_mpd_packet_item_t **pppacket_item)
{
   int                    ret          = AR_EOK;
   gpr_mpd_packet_item_t *ppacket_item = NULL;
   if (NULL == pppacket_item)
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid Parameter");
      return AR_EBADPARAM;
   }

   ret = gpr_list_remove_head(&gpr_mpd_free_packet_q, (gpr_list_node_t **)&ppacket_item);
   if (AR_EOK == ret)
   {
      *pppacket_item = ppacket_item;
   }
#ifdef GPR_DEBUG_MSG
   AR_MSG(DBG_LOW_PRIO, "gpr_mpd_allocate_packet_item free_pkt_list_size = %lu", gpr_mpd_free_packet_q.size);
#endif
   return ret;
}

static uint32_t gpr_mpd_free_packet_item(gpr_mpd_packet_item_t *ppacket_item)
{
   int ret = AR_EOK;
   if (NULL == ppacket_item)
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid Parameter");
      return AR_EBADPARAM;
   }

   ret = gpr_list_add_tail(&gpr_mpd_free_packet_q, &ppacket_item->link);
   if (AR_EOK != ret)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR mpd kernel: Freeing packet failed, packet node is corrupted or has already been freed ret %d",
             ret);
   }

#ifdef GPR_DEBUG_MSG
   AR_MSG(DBG_LOW_PRIO, "gpr_mpd_free_packet_item free_pkt_list_size = %lu", gpr_mpd_free_packet_q.size);
#endif
   return ret;
}

/* GPR PDR Feature api's */
#ifdef PDR_FEATURE

static void gpr_servreg_state_change_callback(ar_osal_servreg_t             servreg_handle,
                                              ar_osal_servreg_cb_event_type event_id,
                                              void *                        cb_context,
                                              void *                        payload,
                                              uint32_t                      payload_size)
{
   ar_osal_servreg_state_notify_payload_type *serv_ntfy_pld = (ar_osal_servreg_state_notify_payload_type *)payload;

   AR_MSG(DBG_HIGH_PRIO, "servreg call back rcved with state %d", serv_ntfy_pld->service_state);
   upd_status = serv_ntfy_pld->service_state;
   return;
}

GPR_INTERNAL uint32_t gpr_pdr_init(void)
{
   uint32_t                   result;
   bool_t                     found_domain = FALSE;
   ar_osal_servreg_entry_type domain;
   ar_osal_servreg_entry_type service;
   char                       service_name_listen[16] = "avs/audio";         /*service to listen*/
   char                       domain_name_listen[20]  = "msm/adsp/audio_pd"; // domain to register
   ar_heap_info               heap_info;
   ar_mem_set((void *)&heap_info, 0, sizeof(ar_heap_info));
   heap_info.tag         = AR_HEAP_TAG_DEFAULT;
   heap_info.align_bytes = AR_HEAP_ALIGN_8_BYTES;

   /*copy service and domain to ar service and domain*/
   ar_mem_cpy(&service.name, sizeof(service_name_listen), service_name_listen, sizeof(service_name_listen));
   ar_mem_cpy(&domain.name, sizeof(domain_name_listen), domain_name_listen, sizeof(domain_name_listen));

   /*Init ar_osal_servreg before using any ar_osal_servreg api's*/
   result = ar_osal_servreg_init();
   if (result != AR_EOK)
   {
      AR_MSG(DBG_ERROR_PRIO, "ar_osal_servreg init failed %d", result);
      return result;
   }

#if 0 // Enable once the issues are fixed
	result = ar_osal_servreg_get_domainlist(&service, NULL, &domain_list_size);
	if (result != AR_EOK)
	{
		AR_MSG(DBG_ERROR_PRIO,"failed to get domains with ar_osal state notifier %d", result);
		return result;
	}

	result = ar_osal_servreg_get_domainlist(&service, NULL, &domain_list_size);
	if (result != AR_EOK)
	{
		AR_MSG(DBG_ERROR_PRIO,"failed to get domains with ar_osal state notifier %d", result);
		return result;
	}

	AR_MSG(DBG_HIGH_PRIO,"ar_osal_servreg_register rcved size %d (%d) ", domain_list_size, result);

	domain_list_t = ar_heap_malloc(domain_list_size *sizeof(ar_osal_servreg_entry_type), &heap_info);
	if (domain_list_t == NULL)
	{
		AR_MSG(DBG_HIGH_PRIO,"Unable to malloc");
		result = AR_ENOMEMORY;
		ar_heap_free(domain_list_t,&heap_info);
		return result;
	}

	result = ar_osal_servreg_get_domainlist(&service, domain_list_t,&domain_list_size);
	if (result) 
	{
			AR_MSG(DBG_ERROR_PRIO,"failed to get domains with ar_osal state notifier %d", result);
			result = AR_EFAILED;
			ar_heap_free(domain_list_t,&heap_info);
			return result;
	}
		
	/* scan through domain list and try to fetch needed domain */
	for (i = 0; i < domain_list_size; i++) 
	{
		if(strncmp(domain_list_t[i].name, "mdm/modem/audio_pd", strlen("mdm/modem/audio_pd") ) == 0)
		{
				found_domain = TRUE;
				break;
		}

		//use case for audio running as a UserPD in ADSP/MSM
		if(strncmp(domain_list_t[i].name, "msm/adsp/audio_pd", strlen("msm/adsp/audio_pd") ) == 0)
		{
			found_domain = TRUE;
			break;
		}

		//use case for audio running in rootPD in MDM
		if(strncmp(domain_list_t[i].name, "mdm/modem/root_pd", strlen("mdm/modem/root_pd") ) == 0)
		{
			found_domain = TRUE;
			break;
		}

		
	}
#endif

   found_domain = TRUE; // Remove this once issues are fixed for get domain list
   if (found_domain == TRUE)
   {
      // Register for the service needed to listen along with its domain.
      ar_servreg_handle =
         ar_osal_servreg_register(AR_OSAL_CLIENT_LISTENER, gpr_servreg_state_change_callback, NULL, &domain, &service);
      if (ar_servreg_handle == NULL)
      {
         result = AR_EFAILED;
         AR_MSG(DBG_ERROR_PRIO, "ar_osal_servreg register failed %d", result);
      }
   }

   // free(domain_list_t);
   return result;
}

// De-init sequence used for gpr pdr
GPR_INTERNAL uint32_t gpr_pdr_deinit(void)
{
   uint32_t result = AR_EOK;

   // Deregister from service registry
   result = ar_osal_servreg_deregister(ar_servreg_handle);
   return result;
}

/*  When the UPD restarts, this function is used to clean up the existing UPD packets
At the same time informs the packet owners on the packet delivery failure */
GPR_INTERNAL uint32_t gpr_pd_restart_call(void)
{
   gpr_mpd_ugpr_t *     pugpr;
   uint32_t             ret;
   uint32_t             dst_domain_id;
   gpr_list_node_t *    cur_node, *dummy_node;
   gpr_mpd_ugpr_item_t *cur_ugpr_item;
   __gpr_cmd_get_host_domain_id(&dst_domain_id);

   /* Find if there exists an uGPR with a matching domain ID*/
   /* As the UPD packets are also freed up by qdi release too. So lock it before cleaning up */
   ar_osal_mutex_lock(gpr_mpd_packets_lock);
   dummy_node = &gpr_mpd_used_ugpr_q.dummy;
   cur_node   = dummy_node->next;

   while (cur_node != dummy_node)
   {
      cur_ugpr_item = (gpr_mpd_ugpr_item_t *)cur_node;
      if (dst_domain_id == cur_ugpr_item->ugpr.domain_id)
         break;
      cur_node = cur_node->next;
   }
   // If NO UPD exists, then we assume that cur_node and next node(dummy node) are equal
   if (cur_node == dummy_node)
   {
      ar_osal_mutex_unlock(gpr_mpd_packets_lock); // Make sure to release the lock while returning.
      return AR_EOK;
   }
   // step2: Free up all the rx packet queue for the ugpr(freeing up rx packet is based on domain)
   // This will dynamically pull in the pointer based on the Process domain we need to clean up
   pugpr = &cur_ugpr_item->ugpr;
   qurt_signal_destroy(pugpr->rx_signal); // Destroy the signal so that ugpr is not signalled on any packet arrival
   qurt_qdi_user_free(pugpr->client_handle, pugpr->rx_signal); // Free qdi user

   // Iterate through on rx buffer queue and free it
   AR_MSG(DBG_HIGH_PRIO, "Size of GPR UPD packets before freeing :(%lu)", pugpr->rx_packet_q.size);
   ret = gpr_ugpr_free_pkts(pugpr);
   if (ret != AR_EOK)
   {
      AR_MSG(DBG_ERROR_PRIO, "Error in freeing the uapar resources");
   }
   AR_MSG(DBG_HIGH_PRIO, "Size of GPR UPD packets after freeing: (%lu)", pugpr->rx_packet_q.size);

   ar_osal_mutex_unlock(gpr_mpd_packets_lock); // Release the lock before returning
   return AR_EOK;
}

#endif