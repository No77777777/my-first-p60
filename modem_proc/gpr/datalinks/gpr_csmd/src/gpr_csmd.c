/*========================================================================

*/ /** @file gpr_csmd.c
This file contains the implementations of the functions/APIs exposed to the GPR
Layer. The functions ultimately invoke the CSMD APIs to complete any 
transactions with the shared memory.

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*====================================================================== */

/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\src\gpr_csmd.c


when       who    what, where, why
--------  -----     -------------------------------------------------------
03/19/18   akr      Created file.

========================================================================== */

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include "gpr_csmd_i.h"
#include "gpr_csmd.h"
#include "ar_msg.h"
#include "csmd_api.h"
#include "internal_csmd.h"
#include "csmd_bufq.h"
/******************************************************************************
 * Globals                                                               *
 *****************************************************************************/
/*Object of the Global CSMD Context Structure*/
csmd_ctx_t csmd_global_ctx;

/* csmd ipc datalink function table*/
static ipc_to_gpr_vtbl_t csmd_vtbl = {
   gpr_csmd_tx,
   gpr_csmd_rx_done,
};

bool_t is_first_dl_init   = TRUE;
bool_t is_first_dl_deinit = TRUE;
/*==========================================================================
Function Definitions
========================================================================== */

/*@brief CSMD datalink init
  @param[in] domain_id          Domain id of destination
  @param[in] gpr_to_ipc_vtbl_t  Pointer to function table sent from GPR to IPC datalink layers
  @param[in] ipc_to_gpr_vtbl_t  Double pointer to function table to be populated by IPC datalink layer

  @return
  #AR_EOK when successful.
*/
uint32_t ipc_dl_csmd_init(uint32_t                 src_domain_id,
                             uint32_t                 dest_domain_id,
                             const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                             ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl)
{
   // assign the  GPR IPC vtbl with the pre-defined function pointers for send and rx_done
   *pp_ipc_to_gpr_vtbl = &csmd_vtbl;
   // one-time initializations
   if (is_first_dl_init)
   {
      // ar_mem_set the global ctx struct to 0 first
      ar_mem_set(&csmd_global_ctx, 0, sizeof(csmd_ctx_t));
      is_first_dl_init = FALSE;

      // Init gpr_csmd_port_pool state
      for (uint32_t index = 0; index < GPR_CSMD_MAX_PORTS; index++)
      {
         csmd_global_ctx.state[index] = GPR_CSMD_DISCONNECTED;
      }
      // initialize the transport layer between the host and IPC Cosim
      if (AR_EOK == xport_init())
      {
         csmd_global_ctx.global_ipc_cb_table.ls_cb         = gpr_ipc_csmd_link_up_cb;
         csmd_global_ctx.global_ipc_cb_table.rx_cb         = (void *)p_gpr_to_ipc_vtbl->receive;
         csmd_global_ctx.global_ipc_cb_table.tx_done_cb    = (void *)p_gpr_to_ipc_vtbl->send_done;
         csmd_global_ctx.global_ipc_cb_table.link_close_cb = gpr_csmd_close;
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO, "Couldn't init the Transport");
         return AR_ENORESOURCE;
      }
   }

   return csmd_init_and_register(src_domain_id, dest_domain_id);
}

/*@brief CSMD datalink de-init
  @param[in] domain_id  Domain id

  @return
  #AR_EOK when successful.
*/
uint32_t ipc_dl_csmd_deinit(uint32_t src_domain_id, uint32_t dest_domain_id)
{
   AR_MSG(DBG_HIGH_PRIO, "gpr_csmd_deinit() Inside DEINIT");

   // one-time de-initializations
   if (is_first_dl_deinit)
   {
      is_first_dl_deinit = FALSE;
      //deregister all the registered procs in one go
      for(uint32_t i = 0; i< GPR_CSMD_MAX_PORTS; i++)
      {
        if(csmd_global_ctx.open_cb_done[i])
        {
          if (AR_EOK != csmd_deinit_and_deregister(src_domain_id, i))
          {
             AR_MSG(DBG_ERROR_PRIO, "Failed to deinit and deregister CSMD");
             return AR_EFAILED;
          }
        }
      }
      // destroys the signal and thread
      if (AR_EOK != xport_deinit())
      {
         AR_MSG(DBG_ERROR_PRIO, "Failed to deinit and deregister CSMD");
         return AR_EFAILED;
      }
      csmd_global_ctx.global_ipc_cb_table.ls_cb         = NULL;
      csmd_global_ctx.global_ipc_cb_table.rx_cb         = NULL;
      csmd_global_ctx.global_ipc_cb_table.tx_done_cb    = NULL;
      csmd_global_ctx.global_ipc_cb_table.link_close_cb = NULL;
   }
   return AR_EOK;
}

/*
@brief Calls csmd api to transmit the provided buffer over csmd.

 @param[in]    domain_id   port index associated with the logical channel

 @param[in]   *buf         Pointer to the data buffer to be transmitted

 @param[in]   size          Size of buffer

 @return       GPR error code

 @sideeffects  Causes remote host to wake-up and process rx pkt
*/
uint32_t gpr_csmd_tx(uint32_t domain_id, void *buf, uint32_t size)
{
   uint32_t csmd_rc;

   AR_MSG(DBG_HIGH_PRIO, "IN gpr_csmd_tx(): port index =%lu, size = %lu", domain_id, size);

   while (FALSE == csmd_global_ctx.open_cb_done[domain_id])
   {
   } // accept no commands until the link is open

   if (GPR_CSMD_CONNECTED != csmd_global_ctx.state[domain_id])
   {
      AR_MSG(DBG_ERROR_PRIO, "gpr_csmd_tx(): port not connected, state=%lx", csmd_global_ctx.state[domain_id]);
      return AR_ENOTREADY;
   }

   if (AR_EOK != (csmd_rc = csmd_tx(domain_id, buf, size)))
   {
      AR_MSG(DBG_ERROR_PRIO, "csmd_tx() failed with rc = %lu", csmd_rc);
      return AR_EFAILED;
   }

   csmd_global_ctx.global_ipc_cb_table.tx_done_cb(buf, size);

   return AR_EOK;
}

/*
@brief Called when RX is done for every buffer via a callback

@param[in]    domain_id   index of the gpr_csmd port

 @param[in]   *ptr         Pointer to the intent used

 @return       GPR error code

 @sideeffects  none
*/
uint32_t gpr_csmd_rx_done(uint32_t domain_id, void *buf)
{
   if (GPR_CSMD_CONNECTED != csmd_global_ctx.state[domain_id])
   {
      return AR_ENOTREADY;
   }
   // make it ready to be reused the intents
   return return_intent(buf);
}
