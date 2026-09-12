/*========================================================================
*/ /** @file gpr_mpd_user.c
This file contains the implementations of the functions/APIs used by user mpd

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
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
#include "gpr_mpd.h"
#include "gpr_mpd_user_i.h"
#include "gpr_mpd_qurt_i.h"
#include "gpr_api_inline.h"
#include "ar_msg.h"
#ifdef FEATURE_CENTRALIZED_THREAD_PRIORITY
#include "rcinit.h"
#endif
#include "qurt.h"
#include "qurt_qdi_driver.h"

/*****************************************************************************
 * Defines                                                                   *
 ****************************************************************************/
#define GPR_KDRV_NAME ("qc_gpr")
#define GPR_QDI_THREAD_PRI_ORDER (50)
#define GPR_QDI_THREAD_STACK_SIZE 8192
#define GPR_QDI_SIG_EXIT 0x00000002
#define GPR_QDI_SIG_DATA_RX GPR_MPD_SIG_RX_PACKET // 0x0000001

#define EC_QURT(call, op)                                                                                              \
   do                                                                                                                  \
   {                                                                                                                   \
      int32_t qurt_rc = 0;                                                                                             \
      if (!((qurt_rc = call)op 0))                                                                                     \
      {                                                                                                                \
         AR_MSG(DBG_ERROR_PRIO, "failed with error %d", qurt_rc);                                                      \
         goto error;                                                                                                   \
      }                                                                                                                \
   } while (0)
#define EC_GTEZ(call) EC_QURT(call, >=) // call >= 0
#define EC_GTZ(call) EC_QURT(call, >)   // call > 0
#define EC_EQZ(call) EC_QURT(call, ==)  // call == 0

/*****************************************************************************
 * Data Structures                                                           *
 ****************************************************************************/
typedef struct gpr_mpd_user_ctxt_t
{
   uint32_t       qdi_handle;
   qurt_signal_t *qdi_signal_handle;

   gpr_mpd_cb_fn_t rx_cb;
   struct
   {
      uint8_t            stack[GPR_QDI_THREAD_STACK_SIZE];
      qurt_thread_attr_t attr;
      qurt_thread_t      tid;
      void *             param;
      uint32_t           result;
   } listener_thread;

} gpr_mpd_user_ctxt_t;

gpr_mpd_user_ctxt_t gpr_mpd_user_ctxt;

/*****************************************************************************
 * Declarations                                                              *
 ****************************************************************************/
/* Function table for user mpd dl*/
static ipc_to_gpr_vtbl_t mpd_user_vtbl = {
   gpr_mpd_send,
   NULL,
};

static bool_t mpd_user_init_done = FALSE;
/*****************************************************************************
 * Implementations                                                           *
 ****************************************************************************/
/*@brief MPD user init
  @param[in] domain_id          Domain id of destination
  @param[in] gpr_to_ipc_vtbl_t  Pointer to function table sent from GPR to mpd user layers
  @param[in] ipc_to_gpr_vtbl_t  Double pointer to function table to be populated by mpd user layer

  @return
  #AR_EOK when successful.
*/
GPR_INTERNAL uint32_t ipc_dl_mpd_user_init(uint32_t                 src_domain_id,
                                           uint32_t                 dest_domain_id,
                                           const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                           ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl)
{
   uint32_t rc = AR_EOK;

   *pp_ipc_to_gpr_vtbl = &mpd_user_vtbl;

   if (!mpd_user_init_done)
   {
      rc = gpr_mpd_init();
      gpr_mpd_set_rx_cb(gpr_ipc_mpd_cb);

      if (AR_EOK == rc)
      {
         mpd_user_init_done = TRUE;
      }
   }
   return rc;
}

/*@brief MPD user de-init
  @param[in] domain_id  Domain id

  @return
  #AR_EOK when successful.
*/
GPR_INTERNAL uint32_t ipc_dl_mpd_user_deinit(uint32_t src_domain_id, uint32_t dest_domain_id)
{
   uint32_t rc = AR_EOK;
   if (mpd_user_init_done)
   {
      rc = gpr_mpd_deinit(dest_domain_id);
      if (AR_EOK == rc)
      {
         mpd_user_init_done = FALSE;
      }
   }
   return rc;
}

static void gpr_qdi_thread_entry(void *param)
{
   uint32_t      signal_mask = 0;
   gpr_packet_t  packet_header;
   uint32_t      packet_size = 0, bytes_read = 0;
   gpr_packet_t *new_packet = NULL;
   uint32_t      rc         = AR_EOK;

   /* If there are any error scenarios handle them gracefully, log them and
    * continue. DO NOT EXIT.
   */
   while (1)
   {
#ifdef GPR_DEBUG_MSG
      AR_MSG(DBG_HIGH_PRIO, "gpr user pd: wait on signal mask %lx", signal_mask);
#endif

   /* Get the signal object from Kernel, this will be signaled when
      ever kernel needs to send a message to us */
      signal_mask = __gpr_mpd_cmd_wait_rx_signal(gpr_mpd_user_ctxt.qdi_handle);

      if (signal_mask & GPR_QDI_SIG_EXIT)
      {
         /* exit requested */
         break;
      }

      if (signal_mask & GPR_QDI_SIG_DATA_RX)
      {
         while (1)
         {
            if (AR_EOK != (rc = __gpr_mpd_cmd_peek_rx_packet(gpr_mpd_user_ctxt.qdi_handle, &packet_header)))
            {
               /* Done reading packets */
               break;
            }

            packet_size = GPR_PKT_GET_PACKET_BYTE_SIZE(packet_header.header);

            /* SUPER_HACK: Allocate memory from destination service's packet pool */
            if (AR_EOK != (rc = __gpr_cmd_alloc(packet_size, &new_packet)))
            {
               AR_MSG(DBG_ERROR_PRIO, "gpr user pd: Packet allocation failed ret=0x%x", rc);
               break;
            }

            if (AR_EOK != (rc = __gpr_mpd_cmd_read_rx_packet(gpr_mpd_user_ctxt.qdi_handle,
                                                               (uint8_t *)new_packet,
                                                               packet_size,
                                                               &bytes_read)))
            {
               __gpr_cmd_free(new_packet);
               AR_MSG(DBG_ERROR_PRIO, "gpr user pd: Packet read failed ret=0x%x", rc);
               break;
            }
#ifdef GPR_DEBUG_MSG
            else
            {
               AR_MSG(DBG_HIGH_PRIO,
                      "gpr user pd: Packet read success, dst domain %ld dst port %lx token 0x%lx",
                      new_packet->dst_domain_id,
                      new_packet->dst_port,
                      new_packet->token);
            }
#endif

            /* invoke clients call-back */
            if (AR_EOK != (rc = gpr_mpd_user_ctxt.rx_cb(0, (void *)new_packet, sizeof(gpr_packet_t))))
            {
               AR_MSG(DBG_HIGH_PRIO, "gpr user pd: Callback to uGPR IPC failed ret=0x%x", rc);
               break;
            }
            new_packet = NULL;
         }
      }
   }
   return;
}

/*@brief Receives packet from datalink layer and sends it to gpr
  @param[in] domain_id  Index to retrieve remote ssid, corresponding channel name and datalink type
  @param[in] buf          Buffer received
  @param[in] length       Size of buffer

  @return
  #AR_EOK when successful.
*/
GPR_INTERNAL uint32_t gpr_ipc_mpd_cb(uint32_t event_id, void *buf, uint32_t length)
{
   int32_t       rc = AR_EOK;
   gpr_packet_t *new_packet;

   if (!event_id)
   {
      new_packet = ((gpr_packet_t *)buf);
      rc = __gpr_cmd_async_send(new_packet);
   }
   else
   {
      rc = AR_EFAILED;
      AR_MSG(DBG_ERROR_PRIO, "ADSP gpr_mpd_cb failed, rc = %ld", rc);
   }

   return rc;
}

GPR_INTERNAL uint32_t gpr_mpd_init(void)
{
   void *         pStack = NULL, *pStackAligned = NULL;
   unsigned short prio             = 0;
   uint32_t       alignedStackSize = 0;

   ar_mem_set((void *)&gpr_mpd_user_ctxt, 0, sizeof(gpr_mpd_user_ctxt_t));

   /* open the QDI handle to kernel */
   gpr_mpd_user_ctxt.qdi_handle = -1;
   EC_GTEZ(gpr_mpd_user_ctxt.qdi_handle = qurt_qdi_open(GPR_KDRV_NAME));

#ifdef FEATURE_CENTRALIZED_THREAD_PRIORITY
   { /* Lookup task info */
      RCINIT_INFO info_handle = NULL;
      RCINIT_PRIO rcinit_prio = 0;

      info_handle = rcinit_lookup("GPR_QDI_USR");

      if (!info_handle)
      {
         ERR_FATAL("GPR_QDI_USR task info not found", 0, 0, 0);
      }
      else
      {
         rcinit_prio      = rcinit_lookup_prio_info(info_handle);
         alignedStackSize = (uint32_t)rcinit_lookup_stksz_info(info_handle);
         if ((rcinit_prio > 255) || (alignedStackSize == 0))
         {
            ERR_FATAL("Invalid Priority:%d or Stack Size: %d", rcinit_prio, alignedStackSize, 0);
         }

         /*
          * Normalizing priority to gpr_thread_priority (0=Highest and 255=Lowest)
          * as priorities obtained from rcinit (rcinit_task_prio.csv) follow the
          * opposite i.e. 0=Lowest and 255=Highest.
         */
         prio = 255 - (unsigned short)rcinit_prio;
      }
   }
#else
   prio             = GPR_QDI_THREAD_PRI_ORDER;
   alignedStackSize = GPR_QDI_THREAD_STACK_SIZE;
#endif

   /* Calculate aligned stack pointer */
   pStack        = (void *)(&gpr_mpd_user_ctxt.listener_thread.stack);
   pStackAligned = (void *)((uint32_t)((uint32_t)pStack + 127) & (~127));

   /* Calculate aligned stack size. */
   alignedStackSize = (unsigned int)(alignedStackSize - ((uint32_t)pStackAligned - (uint32_t)pStack));
   alignedStackSize -= alignedStackSize % 128;

   qurt_thread_attr_init(&gpr_mpd_user_ctxt.listener_thread.attr);
   qurt_thread_attr_set_stack_size(&gpr_mpd_user_ctxt.listener_thread.attr, alignedStackSize);
   qurt_thread_attr_set_stack_addr(&gpr_mpd_user_ctxt.listener_thread.attr, pStackAligned);
   qurt_thread_attr_set_priority(&gpr_mpd_user_ctxt.listener_thread.attr, prio);
   qurt_thread_attr_set_name(&gpr_mpd_user_ctxt.listener_thread.attr, "GPR_QDI_USR");

   EC_EQZ(qurt_thread_create(&gpr_mpd_user_ctxt.listener_thread.tid,
                             &gpr_mpd_user_ctxt.listener_thread.attr,
                             gpr_qdi_thread_entry,
                             &gpr_mpd_user_ctxt));

   return AR_EOK;

error:
   if (gpr_mpd_user_ctxt.qdi_handle != -1)
   {
      qurt_qdi_close(gpr_mpd_user_ctxt.qdi_handle);
   }
   if (gpr_mpd_user_ctxt.listener_thread.tid != 0)
   {
      int status = 0;
	  
      __gpr_mpd_cmd_kill_rx_signal(gpr_mpd_user_ctxt.qdi_handle);
      qurt_thread_join(gpr_mpd_user_ctxt.listener_thread.tid, &status);
   }
   return AR_EFAILED;
}

GPR_INTERNAL uint32_t gpr_mpd_deinit(uint32_t domain_id)
{
   int status = 0;
   /* signal QDI thread to exit */
   __gpr_mpd_cmd_kill_rx_signal(gpr_mpd_user_ctxt.qdi_handle);
   qurt_thread_join(gpr_mpd_user_ctxt.listener_thread.tid, &status);
   if (status != QURT_EOK && status != QURT_ENOTHREAD)
   {
      goto error;
   }

   if (gpr_mpd_user_ctxt.qdi_handle != -1)
   {
      EC_EQZ(qurt_qdi_close(gpr_mpd_user_ctxt.qdi_handle));
   }
   return AR_EOK;

error:
   return AR_EFAILED;
}

/*
  Send a packet to the kernel.

  @param packet gpr packet to send
*/
GPR_INTERNAL uint32_t gpr_mpd_send(uint32_t domain_id, void *packet_, uint32_t packet_len)
{
   uint32_t      size;
   gpr_packet_t *packet = (gpr_packet_t *)packet_;
   if (packet)
   {

#ifdef GPR_DEBUG_MSG
	   AR_MSG(DBG_HIGH_PRIO,
	                       "gpr user pd: Send Packet dst domain %ld dst port %lx opcode %lx token 0x%lx",
						   packet->dst_domain_id,
						   packet->dst_port,
						   packet->opcode,
						   packet->token);
#endif

      size = GPR_PKT_GET_PACKET_BYTE_SIZE(packet->header);
      EC_EQZ(__gpr_mpd_cmd_async_send(gpr_mpd_user_ctxt.qdi_handle, (uint8_t *)packet, size));
      __gpr_cmd_free(packet);
   }
   else
   {
      return AR_EBADPARAM;
   }

   return AR_EOK;
error:
   return AR_EFAILED;
}

/*
  Register callback with mpd service. This call-back will get invoked
  for every received message.

  @param pfn_rx_cb callback function
*/
GPR_INTERNAL uint32_t gpr_mpd_set_rx_cb(gpr_mpd_cb_fn_t pfn_rx_cb)
{
   uint32_t domain_id;
   gpr_mpd_user_ctxt.rx_cb = pfn_rx_cb;
   __gpr_cmd_get_host_domain_id(&domain_id);
   
   EC_EQZ( __gpr_mpd_cmd_set_upd_state(gpr_mpd_user_ctxt.qdi_handle, 1, domain_id ));
   
   return AR_EOK;
error:
   return AR_EFAILED;
   
}
