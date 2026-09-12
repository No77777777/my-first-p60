/*========================================================================

*/ /** @file csmd.c
This file contains the implementations of the functions/APIs exposed by the CSMD
Layer to the GPR. The functions ultimately invoke the Xport APIs to complete any 
transactions with the shared memory.

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*====================================================================== */

/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\src\csmd.c


when       who    what, where, why
--------  -----     -------------------------------------------------------
03/19/18    akr      Created file.

========================================================================== */
/*==========================================================================
Include files
========================================================================== */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gpr_csmd_i.h"
#include "csmd_api.h"
#include "internal_csmd.h"
#include "csmd_bufq.h"
#include "ar_msg.h"
/*==========================================================================
Globals
========================================================================== */
/*Object of the Global CSMD Context Structure*/
extern csmd_ctx_t csmd_global_ctx;
/*==========================================================================
Function Definitions
========================================================================== */
/**
@brief API to register the Host proc with the IPC Manager
       with its ID, client ID and the channel number it wishes to open

@param[in] *link_id: ptr to csmd link id information

@param[in] channel_id: Channel number it wishes to communicate on

@return ar_result

*/
uint32_t csmd_init_and_register(uint32_t src_domain, uint32_t channel_id)
{
   csmd_cmd_reg_dereg_host_t link_notif;
   /* Check for remote_ss validity */
   if (NULL != gpr_csmd_port_data[channel_id].remote_ss)
   {
      csmd_global_ctx.link_notif_params[channel_id].remote_ss = gpr_csmd_port_data[channel_id].remote_ss;
      // to find the proc ID of the destination for IPC Mgr Reg based on domain IDs
      switch (channel_id)
      {
         case GPR_IDS_DOMAIN_ID_MODEM_V:
         {
            link_notif.dst_proc_id = CSMD_ID_MPSS_PROC;
            break;
         }
         case GPR_IDS_DOMAIN_ID_ADSP_V:
         {
            link_notif.dst_proc_id = CSMD_ID_LPASS_PROC;
            break;
         }
         case GPR_IDS_DOMAIN_ID_APPS_V:
         {
            link_notif.dst_proc_id = CSMD_ID_APSS_PROC;
            break;
         }
         default:
         {
            return AR_EUNSUPPORTED;
         }
      }
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid domain_id %lu", channel_id);
      csmd_global_ctx.link_notif_params[channel_id].remote_ss = NULL;
      return AR_EBADPARAM;
   }
   // write to the shared memory to handover to cosim to complete the write and register with IPC manager
   // make it a standard packet to send to cosim/winsock
   csmd_pkt_t to_send;
   to_send.size       = sizeof(csmd_cmd_reg_dereg_host_t);
   to_send.channel_id = channel_id;
   to_send.opcode     = CSMD_CMD_REG_HOST;

   if (GPR_IDS_DOMAIN_ID_ADSP_V == src_domain)
   {
      link_notif.src_proc_id = CSMD_ID_LPASS_PROC;
   }
   else if (GPR_IDS_DOMAIN_ID_APPS_V == src_domain)
   {
      link_notif.src_proc_id = CSMD_ID_APSS_PROC;
   }
   else if (GPR_IDS_DOMAIN_ID_MODEM_V == src_domain)
   {
      link_notif.src_proc_id = CSMD_ID_MPSS_PROC;
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "csmd register: Host domain not defined");
      return AR_EBADPARAM;
   }

   AR_MSG(DBG_HIGH_PRIO, "CSMD register: Sending Reg Pkt on channel/domain_id %lu", channel_id);

   // Invoking the xport API to send
   return xport_send((void *)&to_send, sizeof(csmd_pkt_t), (void *)&link_notif, to_send.size);
}

/**
@brief API to deinit CSMD

@param[in] channel_id: Channel number it wishes to communicate on

@return ar_result

*/
uint32_t csmd_deinit_and_deregister(uint32_t src_domain, uint32_t channel_id)
{
   uint32_t rc;
   // let us deregister this host from the IPC manager
   csmd_pkt_t to_send;
   to_send.size       = sizeof(csmd_cmd_reg_dereg_host_t);
   to_send.channel_id = 0xFFFF; // don't care
   to_send.opcode     = CSMD_CMD_DEREG_HOST;

   csmd_cmd_reg_dereg_host_t payload;
   if (GPR_IDS_DOMAIN_ID_ADSP_V == src_domain)
   {
      payload.src_proc_id = CSMD_ID_LPASS_PROC;
   }
   else if (GPR_IDS_DOMAIN_ID_APPS_V == src_domain)
   {
      payload.src_proc_id = CSMD_ID_APSS_PROC;
   }
   else if (GPR_IDS_DOMAIN_ID_MODEM_V == src_domain)
   {
      payload.src_proc_id = CSMD_ID_MPSS_PROC;
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "csmd register: Host domain not defined");
      return AR_EBADPARAM;
   }
   // Invoking the xport API to send deregister command and payload
   xport_send((void *)&to_send, sizeof(csmd_pkt_t), (void *)&payload, to_send.size);

   // free the intent queues and return
   rc = free_intents();
   if ((AR_EOK == rc) || (AR_EALREADY == rc))
   {
      rc = AR_EOK;
      // marking the channel state as disconnected
      csmd_global_ctx.state[channel_id] = GPR_CSMD_DISCONNECTED;
   }
   return rc;
}
/*
@brief CB called when Client is up and host gets notification

@param[in] *link_info: struct with info about the link

@param[in] chn_id: channel on which host registered

@return none

@sideeffects calls open command
*/
uint32_t gpr_ipc_csmd_link_up_cb(csmd_link_info_type *link_info)
{
   char_t *remote_ss;
   int32_t channel_id, index;

   if (link_info->link_state != CSMD_LINK_STATE_UP)
   {
      AR_MSG(DBG_ERROR_PRIO, "gpr_ipc_csmd_link_state_cb(): link_info->link_state != CSMD_LINK_STATE_UP ");
      return AR_ENOTREADY;
   }
   for (index = 0; index < GPR_CSMD_MAX_PORTS; index++)
   {
      remote_ss = gpr_csmd_port_data[index].remote_ss;
      if ((NULL != remote_ss) && (NULL != link_info->remote_ss))
      {
         uint32_t size_to_cmp =
            (sizeof(link_info->remote_ss) < sizeof(remote_ss)) ? sizeof(link_info->remote_ss) : sizeof(remote_ss);
         if (ar_strcmp(link_info->remote_ss, remote_ss, size_to_cmp) == 0)
         {
            channel_id = gpr_csmd_port_data[index].domain_id;
            if (AR_EOK != gpr_csmd_open(channel_id))
            {
               gpr_csmd_close(channel_id);
               return AR_EFAILED;
            }
         }
      }
   }
   return AR_EOK;
}

/*
@brief         Opens the CSMD channel between host and the remote host

 @param[in]    channel_id: index of the gpr_csmd port

 @return       GPR error code

 @sideeffects  sets port state to disconnecting - wont tx anymore
*/
uint32_t gpr_csmd_open(uint32_t channel_id)
{
   uint32_t rc;
   AR_MSG(DBG_HIGH_PRIO, "GPR Csmd Connected to Remote Client on port %lu", channel_id);

   /* Queue rx intents */
   rc = queue_intents(GPR_CSMD_NUM_OF_1024_BYTES_INTENTS, GPR_CSMD_1024_BYTES_BUFFER);
   if ((AR_EOK == rc) || (AR_EALREADY == rc))
   {
      csmd_global_ctx.state[channel_id] = GPR_CSMD_CONNECTED;
      rc                                = AR_EOK;
   }
   return rc;
}

/*
@brief Closes the CSMD channel between host and the remote host

 @param[in]    channel_id: index of the gpr_csmd port

 @return       GPR error code

 @sideeffects  sets port state to disconnecting - wont tx anymore
*/
uint32_t gpr_csmd_close(uint32_t channel_id)
{
   if (GPR_CSMD_DISCONNECTED == csmd_global_ctx.state[channel_id])
   {
      AR_MSG(DBG_HIGH_PRIO, "Port %lu already closed", channel_id);
      return AR_EOK; // already closed
   }

   csmd_global_ctx.state[channel_id] = GPR_CSMD_DISCONNECTED;

   AR_MSG(DBG_HIGH_PRIO, "GPR_CSMD_DISCONNCTED");
   return AR_EOK;
}

/*
@brief Transmit the provided buffer over csmd.

 @param[in]    channel_id   port index associated with the logical channel

 @param[in]   *data         Pointer to the data buffer to be transmitted

 @param[in]   size          Size of buffer

 @return       ar_result

 @sideeffects  Causes remote host to wake-up and process rx pkt
*/
uint32_t csmd_tx(uint32_t channel_id, const void *data, uint32_t size)
{
   uint32_t status;
   if ((NULL != data) && (0 != size))
   {
      // write to the shared memory to handover to cosim to complete the write and register with IPC manager
      // make it a standard packet to send to cosim
      csmd_pkt_t to_send;
      to_send.opcode     = CSMD_CMD_DATA_PACKET;
      to_send.size       = size;
      to_send.channel_id = channel_id;

      // Invoking the xport API to send
      status = xport_send((void *)&to_send, sizeof(csmd_pkt_t), (void *)data, to_send.size);
   }
   else
   {
      status = AR_EBADPARAM;
   }
   return status;
}
/*
@brief Receive over csmd into our queued intents.

 @param[in]   *buf         Pointer to the data buffer to receive from

 @param[in]   size          Size of buffer

 @return       ar_result

 @sideeffects  Causes host to wake-up and process rx pkt
*/
uint32_t csmd_rx(uint32_t channel_id, void *buf, uint32_t size)
{
   if ((size > GPR_CSMD_1024_BYTES_BUFFER) || (NULL == buf))
   {
      return AR_EBADPARAM;
   }

   return xport_receive(channel_id, buf, size);
}
