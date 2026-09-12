/*========================================================================

*/ /** @file gpr_xport.c
This file contains the implementations of the functions/APIs exposed by the Xport Layer to the csmd
Layer. The functions directly transact with the shared memory.

Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*====================================================================== */

/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\src\csmd_xport.c


when       who      what, where, why
--------  -----     -------------------------------------------------------
03/19/18   akr      Created file.

========================================================================== */
/*==========================================================================
Include files
========================================================================== */
#include <stdlib.h>
#include <string.h>
#include "internal_csmd.h"
#include "gpr_csmd_i.h"
#include "csmd_api.h"
#include "csmd_ob.h"
#include "csmd_bufq.h"
#include "ar_msg.h"
#include <assert.h>
#include "ar_osal_mem_op.h"
#ifdef COSIM_TRANSPORT
#include "qurt.h"
#include "stringl.h"
#include "qurt_fastint.h"
/*--------------------------------------------------------------*/
/* Macro definitions                                            */
/* -------------------------------------------------------------*/
// L2VIC registers for clearing the interrupt
// for more info please check LPASS SWI (IPCAT)
#define LPASS___SHARED_MEM_BASE 0x0071FC18             // defined in cosimCfg
#define A_LPASS___SHARED_MEM_INTERRUPT_STATUS (0x0008) // l2vic interrupt status bit
#define A_LPASS___SHARED_MEM_INTERRUPT_CLR (0x000C)    // signifies clearing the interrupt bit
#define outp32(address, value) (*(((volatile uint32 *)(address))) = ((uint32)(value)))
#define inp32(address) (*((volatile uint32 *)(address)))
#define outp16(address, value) (*(((volatile uint16 *)(address))) = ((uint16)(value)))
#define inp16(address) (*((volatile uint16 *)(address)))

/*==========================================================================
Globals
========================================================================== */
static uint32_t   lpm_client_reg_count = 0;
qurt_mem_region_t lpm_region;
#endif // COSIM_TRANSPORT

/*Object of the Global CSMD Context Structure*/
extern csmd_ctx_t csmd_global_ctx;
/*==========================================================================
Function Definitions
========================================================================== */
/**
@brief Utility function to send the packet to IPC Manager for
       winsock hosts

@param[in] *buf: pointer to the payload to be sent

@param[in] size: size of the payload

@return  none

*/
#ifdef WINSOCK_TRANSPORT
uint32_t send_to_ipc_mgr(void *buf, int size)
{
   uint32_t rc;
   rc = send(csmd_global_ctx.transport_info_t.tx_connect_sock, (char_t *)buf, size, 0);
   if (rc == SOCKET_ERROR)
   {
      AR_MSG(DBG_ERROR_PRIO, "send failed with error: %d\n", WSAGetLastError());
      closesocket(csmd_global_ctx.transport_info_t.tx_connect_sock);
      csmd_global_ctx.transport_info_t.tx_connect_sock = (SOCKET)0;
      WSACleanup();
      return AR_EFAILED;
   }
   return AR_EOK; // success
}

void ob_rx_work_loop(void *dummy)
{
   for (;;)
   {
      uint32_t rcv_size;
      char_t * recvbuf = (char_t *)malloc(CSMD_XPORT_DEFAULT_BUFLEN);
      if(NULL == recvbuf)
      {
         AR_MSG(DBG_ERROR_PRIO, "Malloc Failed in OB Workloop - Breaking Out");
         break;
      }
      rcv_size = recv(csmd_global_ctx.transport_info_t.ob_rx_connect_sock, recvbuf, CSMD_XPORT_DEFAULT_BUFLEN, 0);
      if ((rcv_size > 0) && (rcv_size < 0xFFFFFFFF))
      {
         // AR_MSG(DBG_HIGH_PRIO, "Bytes received: %d\n", rcv_size);
         if (AR_EOK != honour_ob_req(recvbuf, rcv_size))
         {
            AR_MSG(DBG_ERROR_PRIO, "Out of band request handling Failed. Closing Connection");
            free((void *)recvbuf);
            break;

         }
      }
      else if ((0 == rcv_size) || (0xFFFFFFFF == rcv_size))
      {
         AR_MSG(DBG_ERROR_PRIO, "Connection closed");
         free((void *)recvbuf);
         break;
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO, "recv failed with error: %d\n", WSAGetLastError());
      }
      free((void *)recvbuf);
   }
}
/**
@brief Utility function that connects to the IPC manager's Tx/Rx port
      and returns the SOCKET

@param[in] *port_num: port number of the tx/rx port to connect to

@return  SOCKET

*/
SOCKET connenct_to_ipc_manager_port(const char *port_num)
{
   uint32_t         rc     = 0;
   struct addrinfo *result = NULL;
   struct addrinfo *ptr = NULL; /* keeps the addrinfo chain; required to open a new socket */
   struct addrinfo  hints;      /* temporary struct to keep settings needed to open the new socket */
   SOCKET ConnectSocket = INVALID_SOCKET;
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family   = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;
   hints.ai_flags    = AI_PASSIVE;

   rc = apr_socket_get_addrinfo(csmd_global_ctx.transport_info_t.xport_port_address,
                                port_num,
                                &hints,
                                &result);
   if (rc != 0)
   {
      AR_MSG(DBG_ERROR_PRIO, "getaddrinfo failed with error: %d\n", rc);
      WSACleanup();
      return (SOCKET)0;
   }

   // Attempt to connect to an address until one succeeds
   for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
   {

      // Create a SOCKET for connecting to server
      ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if (ConnectSocket == INVALID_SOCKET)
      {
         AR_MSG(DBG_ERROR_PRIO, "socket failed with error: %ld\n", WSAGetLastError());
         WSACleanup();
         return (SOCKET)0;
      }

      // Connect to server.
      rc = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
      if (rc == SOCKET_ERROR)
      {
         closesocket(ConnectSocket);
         ConnectSocket = INVALID_SOCKET;
         continue;
      }
      break;
   }
   freeaddrinfo(result);

   if (ConnectSocket == INVALID_SOCKET)
   {
      AR_MSG(DBG_ERROR_PRIO, "Unable to connect to server!\n");
      WSACleanup();
      return (SOCKET)0;
   }
   return ConnectSocket;
}
#endif // WINSOCK_TRANSPORT
/**
@brief Process function to process the payload that was received
on the  RX Q

@param[in] ptr to the payload

@return  none

*/
uint32_t process_rx_q_payload(void *packet, uint32_t size)
{
   csmd_pkt_t *header      = (csmd_pkt_t *)packet;
   uint32_t opcode     = header->opcode;
   uint32_t    payloadsize = header->size;
   uint32_t channel_id = header->channel_id;

   isr_ctx_t *isr_handle = &csmd_global_ctx.global_ipc_cb_table;

#ifdef CSMD_DEBUG_LOW
   AR_MSG(DBG_HIGH_PRIO,
          "Received payload header: opcode = %lx, size = %lu, channel_id = %lu",
          opcode,
          payloadsize,
          channel_id);
#endif // CSMD_DEBUG_LOW

   switch (opcode)
   {
      case CSMD_EVENT_CLIENT_UP_NOTIFICATION:
      {
         csmd_link_info_type *ls_params_ptr = &(csmd_global_ctx.link_notif_params[channel_id]);
         // call the respective callback
         ls_params_ptr->link_state = CSMD_LINK_STATE_UP;
         return (isr_handle->ls_cb(ls_params_ptr));
         break;
      }
      case CSMD_CMD_DATA_PACKET:
      {
         void *buf;
#ifdef COSIM_TRANSPORT
         buf = (void *)SM_COSIM2DSP_RANGE_START;
#else  // WINSOCK_TRANSPORT
         buf = packet;
#endif // xxTRANSPORT
         if (AR_EOK == csmd_rx(channel_id, buf, payloadsize))
         {
            AR_MSG(DBG_HIGH_PRIO, "Succesfully processed received Buffer on channel_id = %lu", channel_id);
            return AR_EOK;
         }
         else
         {
            AR_MSG(DBG_ERROR_PRIO, "Failed to process received Buffer on channel_id = %lu", channel_id);
            return AR_EFAILED;
         }
         break;
      }
      case CSMD_EVENT_CLIENT_DOWN_NOTIFICATION:
      {
         AR_MSG(DBG_HIGH_PRIO, "Closing CSMD Link on channel_id = %lu", channel_id);
         return isr_handle->link_close_cb(channel_id); // hostid is the port id - gpr_csmd_close
         break;
      }
      default:
      {
         return AR_EUNSUPPORTED;
         break;
      }
   }
}

#ifdef COSIM_TRANSPORT
/**
@brief Interrupt Service Routine that signals the Rx thread

@param[in] none

@return none

*/
void csmd_isr()
{
   csmd_global_ctx.transport_info_t.signal_mask = csmd_global_ctx.transport_info_t.signal_mask | (1 << CH_RX_QUEUE_BIT);
   ar_osal_signal_set(csmd_global_ctx.transport_info_t.p_rx_signal);
   clear_level_interrupt(); // can get more interrupts

   return;
}
#endif // COSIM_TRANSPORT

/**
@brief Workloop that waits on the Channel to receive data from the IPC Manager
Calls Process when it receives a message

@param[in] none

@return  none

*/
void csmd_rx_work_loop(void *dummy)
{
#ifdef WINSOCK_TRANSPORT
   for (;;)
   {
      uint32_t rcv_size;
      char_t * recvbuf = (char_t *)malloc(CSMD_XPORT_DEFAULT_BUFLEN);
      if(NULL == recvbuf)
      {
         AR_MSG(DBG_ERROR_PRIO, "Malloc Failed in RX Workloop - Breaking Out");
         break;
      }
      rcv_size         = recv(csmd_global_ctx.transport_info_t.rx_connect_sock, recvbuf, CSMD_XPORT_DEFAULT_BUFLEN, 0);
      if ((rcv_size >= sizeof(csmd_pkt_t)) && (rcv_size < 0xFFFFFFFF))
      {
         AR_MSG(DBG_HIGH_PRIO, "Bytes received: %d\n", rcv_size);
         if (AR_EOK != process_rx_q_payload((void *)recvbuf, rcv_size))
         {
            AR_MSG(DBG_ERROR_PRIO, "Process Payload Failed - Breaking out of Workloop and will crash");
            break;
         }
         csmd_pkt_t ack;
         ack.opcode     = CSMD_RSP_IBASIC_RESULT;
         ack.size       = 0;      // no payload
         ack.channel_id = 0xFFFF; // dont care
#ifdef CSMD_DEBUG_LOW
         AR_MSG(DBG_HIGH_PRIO, "ACKing the received packet");
#endif // CSMD_DEBUG_LOW
         send_to_ipc_mgr(&ack, sizeof(csmd_pkt_t));
         csmd_pkt_t *header                               = (csmd_pkt_t *)recvbuf;
         csmd_global_ctx.open_cb_done[header->channel_id] = TRUE;
      }

      else if ((rcv_size == 0) || (0xFFFFFFFF))
      {
         AR_MSG(DBG_ERROR_PRIO, "Connection closed\n");
         free((void *)recvbuf);
         break;
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO, "recv failed with error: %d\n", WSAGetLastError());
      }

      free((void *)recvbuf);
   }
#endif
#ifdef COSIM_TRANSPORT
   // Enter forever loop to handle commands and ack
   for (;;)
   {
      // block on any one or more of selected queues to get a msg
      ar_osal_signal_wait(csmd_global_ctx.transport_info_t.p_rx_signal);
      if (csmd_global_ctx.transport_info_t.signal_mask & (1 << CH_RX_QUEUE_BIT))
      {
#ifdef CSMD_DEBUG_LOW
         AR_MSG(DBG_HIGH_PRIO, "Interrupt has woken up the rx Workloop");
#endif // CSMD_DEBUG_LOW         
         // read op-code and size from READ SM
         csmd_pkt_t to_send;

         char_t *ptr_src = (char_t *)SM_COSIM2DSP_RANGE_START;

         ar_mem_cpy((char_t *)&(to_send.opcode), sizeof(to_send.opcode), ptr_src, sizeof(uint32_t));
         
         ptr_src += sizeof(uint32_t);
         ar_mem_cpy((char_t *)&(to_send.size), sizeof(to_send.size), ptr_src, sizeof(uint32_t));
         
         ptr_src += sizeof(uint32_t);
         ar_mem_cpy((char_t *)&(to_send.channel_id), sizeof(to_send.channel_id), ptr_src, sizeof(uint32_t));
   
         if (AR_EOK != process_rx_q_payload((void *)(&to_send), 0xFFFF)) // ffff is a don't care - winsock needs it
         {
            AR_MSG(DBG_ERROR_PRIO, "Process Payload Failed - Breaking out of Workloop and will crash");
            csmd_global_ctx.induced_crash = TRUE;
            break;
         }
         // send a TX ACK only if the processing is a success
         csmd_pkt_t ack;
         ack.opcode                                       = CSMD_RSP_IBASIC_RESULT;
         ack.size                                         = 0; // no payload
         ack.channel_id                                   = 0xFFFF; // dont care
         char_t *ptr_dest                                 = (char_t *)SM_DSP2COSIM_PKT_SEND_ADDR;
         ptr_src                                          = (char_t *)&ack;
         csmd_global_ctx.open_cb_done[to_send.channel_id] = TRUE;
         csmd_global_ctx.transport_info_t.signal_mask =
            csmd_global_ctx.transport_info_t.signal_mask & (~(1 << CH_RX_QUEUE_BIT));
         ar_osal_signal_clear(csmd_global_ctx.transport_info_t.p_rx_signal);
#ifdef CSMD_DEBUG_LOW
         AR_MSG(DBG_HIGH_PRIO, "ACKing the received packet");
#endif // CSMD_DEBUG_LOW
         ar_mem_cpy(ptr_dest, sizeof(csmd_pkt_t), ptr_src, sizeof(csmd_pkt_t));
      }
      if (csmd_global_ctx.transport_info_t.signal_mask & (1 << CH_KILL_SIG_BIT))
      {
         ar_osal_signal_clear(csmd_global_ctx.transport_info_t.p_rx_signal);
         //not an induced crash
         break;
      }
   }
#endif // COSIM_TRANSPORT
   if (csmd_global_ctx.induced_crash)
   {
      assert(0); // crashing so that we can debug any unintended problem
   }
   return;
}
/*
@brief Create and initialize Shared memory between the host and IPC Manager

 @param[in]   none

 @return      ar_result

*/
uint32_t xport_init()
{

#ifdef COSIM_TRANSPORT
   if (AR_EOK != csmd_lpm_sm_register())
   {
      AR_MSG(DBG_ERROR_PRIO, "Xport_init: Couldn't init LPM memory between Q6 and Cosim");
      return AR_ENORESOURCE;
   }
#endif // COSIM_TRANSPORT
   char_t thread_name[16];
   memset(&csmd_global_ctx.transport_info_t, 0, sizeof(xport_global_t));
#ifdef WINSOCK_TRANSPORT

   apr_socket_init();
   csmd_global_ctx.transport_info_t.xport_port_tx      = CSMD_TX_PORT;
   csmd_global_ctx.transport_info_t.xport_port_rx      = CSMD_RX_PORT;
   csmd_global_ctx.transport_info_t.ob_xport_port_tx   = CSMD_OB_TX_PORT;
   csmd_global_ctx.transport_info_t.ob_xport_port_rx   = CSMD_OB_RX_PORT;
   csmd_global_ctx.transport_info_t.xport_port_address = CSMD_COSIM_SERVER_ADDRESS;

   csmd_global_ctx.transport_info_t.tx_connect_sock = connenct_to_ipc_manager_port(csmd_global_ctx.transport_info_t.xport_port_tx);
   if ((SOCKET)0 == csmd_global_ctx.transport_info_t.tx_connect_sock)
   {
      AR_MSG(DBG_ERROR_PRIO, "Tx connect socket is null, failing");
      return AR_EFAILED;
   }

   csmd_global_ctx.transport_info_t.rx_connect_sock = connenct_to_ipc_manager_port(csmd_global_ctx.transport_info_t.xport_port_rx);
   if ((SOCKET)0 == csmd_global_ctx.transport_info_t.rx_connect_sock)
      {
      AR_MSG(DBG_ERROR_PRIO, "Rx connect socket is null, failing");
      return AR_EFAILED;
      }
   //-------------------------------Out of Band connection--------------------------------//

   csmd_global_ctx.transport_info_t.ob_tx_connect_sock = connenct_to_ipc_manager_port(csmd_global_ctx.transport_info_t.ob_xport_port_tx);
      if ((SOCKET)0 == csmd_global_ctx.transport_info_t.ob_tx_connect_sock)
      {
      AR_MSG(DBG_ERROR_PRIO, "OB Tx connect socket is null, failing");
      return AR_EFAILED;
      }

   csmd_global_ctx.transport_info_t.ob_rx_connect_sock = connenct_to_ipc_manager_port(csmd_global_ctx.transport_info_t.ob_xport_port_rx);
      if ((SOCKET)0 == csmd_global_ctx.transport_info_t.ob_rx_connect_sock)
   {
      AR_MSG(DBG_ERROR_PRIO, "OB Rx connect socket is null, failing");
      return AR_EFAILED;
   }

   // Launch a thread for dedicated OB RX
   ar_osal_thread_attr_t attr;

   ar_osal_thread_attr_init(&attr);
   snprintf(thread_name, 16, "OB-RX");
   attr.thread_name = thread_name;
   attr.stack_size  = CSMD_RX_THREAD_STACK_SIZE;
   attr.priority    = 0; // always listening

   if (0 != ar_osal_thread_create(&(csmd_global_ctx.transport_info_t.ob_rcv_tid), &attr, ob_rx_work_loop, NULL))
   {
      AR_MSG(DBG_ERROR_PRIO, "OB-RX thread launch failed");
      return AR_ENORESOURCE;
   }

#endif // WINSOCK_TRANSPORT

// for cosim xport (dsp side) the thread is a one that waits for a signal from the isr
// where as on testapp (client) the thread is the IST in a way that it keeps listening on a socket
// and does callbacks in its own context
#ifdef COSIM_TRANSPORT
   ar_osal_signal_create(&(csmd_global_ctx.transport_info_t.p_rx_signal));
   csmd_global_ctx.transport_info_t.signal_mask = 0;
#endif // COSIM_TRANSPORT

   ar_osal_thread_attr_t attributes;

   ar_osal_thread_attr_init(&attributes);
   snprintf(thread_name, 16, "CSMDRX");
   attributes.thread_name = thread_name;
   attributes.stack_size  = CSMD_RX_THREAD_STACK_SIZE;
   attributes.priority    = 2; // ISR

   if (AR_EOK !=
       ar_osal_thread_create(&(csmd_global_ctx.transport_info_t.rcv_tid), &attributes, csmd_rx_work_loop, NULL))
   {
      csmd_global_ctx.transport_info_t.rcv_tid = 0; //resetting the thread ID
      AR_MSG(DBG_ERROR_PRIO, "CSMDRX thread launch failed");
      return AR_ENORESOURCE;
   }

#ifdef COSIM_TRANSPORT
   if (qurt_isr_register(COSIM2DSP_INTERRUPT_NUM, csmd_isr))
   {
      AR_MSG(DBG_ERROR_PRIO, "ISR reg failed with err");
      return AR_EFAILED;
   }
#endif // COSIM_TRANSPORT

   return AR_EOK;
}

/*
@brief Create and deinitialize Shared memory between the host and IPC Manager

 @param[in]   none

 @return      ar_result

*/
uint32_t xport_deinit()
{
#ifdef COSIM_TRANSPORT

   if (AR_EOK != csmd_lpm_sm_deregister())
   {
      AR_MSG(DBG_ERROR_PRIO, "Xport_deinit: Couldn't deregister LPM memory between Q6 and Cosim");
      return AR_EFAILED;
   }
   csmd_global_ctx.transport_info_t.signal_mask = csmd_global_ctx.transport_info_t.signal_mask | (1 << CH_KILL_SIG_BIT);
   ar_osal_signal_set(csmd_global_ctx.transport_info_t.p_rx_signal);
   ar_osal_signal_destroy(csmd_global_ctx.transport_info_t.p_rx_signal);
#endif // COSIM_TRANSPORT

#ifdef WINSOCK_TRANSPORT
   apr_socket_close(csmd_global_ctx.transport_info_t.tx_connect_sock);
   csmd_global_ctx.transport_info_t.tx_connect_sock = 0;
   apr_socket_close(csmd_global_ctx.transport_info_t.rx_connect_sock);
   csmd_global_ctx.transport_info_t.rx_connect_sock = 0;
   apr_socket_close(csmd_global_ctx.transport_info_t.ob_tx_connect_sock);
   csmd_global_ctx.transport_info_t.ob_tx_connect_sock = 0;
   apr_socket_close(csmd_global_ctx.transport_info_t.ob_rx_connect_sock);
   csmd_global_ctx.transport_info_t.ob_rx_connect_sock = 0;
   ar_osal_thread_join_destroy(csmd_global_ctx.transport_info_t.ob_rcv_tid);
#endif
   ar_osal_thread_join_destroy(csmd_global_ctx.transport_info_t.rcv_tid);
   return AR_EOK;
}

/*
@brief Transmit data via Shared memory to the other host/IPC Manager

 @param[in]   *pkt_ptr          Pointer to the packet to be sent

 @param[in]   pkt_size          Size of the packet

 @param[in]   *data_ptr         Pointer to the data to be sent

 @param[in]   data_size         Size of data

 @return       ar_result

*/
uint32_t xport_send(void *pkt_ptr, uint32_t pkt_size, void *data_ptr, uint32_t data_size)
{
   if ((NULL == pkt_ptr) || (NULL == data_ptr) || (pkt_size <= 0) || (data_size <= 0) ||
       (data_size > GPR_CSMD_1024_BYTES_BUFFER))
   {
      AR_MSG(DBG_ERROR_PRIO, "Xport_send: Erroneus parameters for send.");
      return AR_EBADPARAM;
   }

#ifdef COSIM_TRANSPORT

   // First, mem copy the command packet to the commnad band of TX memory
   char_t *ptr_dest = (char_t *)SM_DSP2COSIM_PKT_SEND_ADDR;
   char_t *ptr_src  = (char_t *)pkt_ptr;
   ar_mem_cpy(ptr_dest, pkt_size, ptr_src, pkt_size);
   // Once the cosim knows the size of data (from the cmd pkt),copy the payload/data to the SM mem slice
   ptr_dest = (char_t *)SHARED_MEM_REG_START_ADDR;
   ptr_src  = (char_t *)data_ptr;
   ar_mem_cpy(ptr_dest, data_size, ptr_src, data_size);

   return AR_EOK;
#endif // COSIM_TRANSPORT

#ifdef WINSOCK_TRANSPORT
   char_t *msg_ptr = (char_t *)malloc(pkt_size + data_size);
   if (NULL == msg_ptr)
   {
      return AR_ENORESOURCE;
   }
   uint32_t rc = AR_EOK;
   ar_mem_cpy(msg_ptr, pkt_size, pkt_ptr, pkt_size);

   char_t *ptr_dest = msg_ptr + pkt_size;
   char_t *ptr_src  = (char_t *)data_ptr;
   ar_mem_cpy(ptr_dest, data_size, ptr_src, data_size); // appending payload to pkt

   rc = send_to_ipc_mgr((void *)msg_ptr, pkt_size + data_size);
   free((void *)msg_ptr);
   return rc;

#endif // WINSOCK_TRANSPORT
}

/*
@brief Receive data via Shared memory into RX intent from the client/IPC Manager

 @param[in]   *buf         Pointer to the data buffer to fill from

 @param[in]   size          Size of buffer

 @return       ar_result

*/
uint32_t xport_receive(uint32_t channel_id, void *buf, uint32_t size)
{
   // strip off the pkt header and forward
   char_t * ptr_src = (char_t *)buf + sizeof(csmd_pkt_t);
   void *intent_ptr = NULL;

   if (AR_EOK == use_intent(&intent_ptr))
   {
      char_t *ptr_dest = (char_t *)(intent_ptr);

      ar_mem_cpy(ptr_dest, size, ptr_src, size);

      isr_ctx_t *isr_handle = &csmd_global_ctx.global_ipc_cb_table;

      isr_handle->rx_cb(intent_ptr, size); // forwarding the intent to the GPR

      return AR_EOK;
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "Error in using intent");
      return AR_EFAILED;
   }
}

#ifdef COSIM_TRANSPORT
/**
@brief Create a memory region and register it with the ADSP

@param[in] none

@return  ADSP error code

*/
uint32_t csmd_lpm_sm_register(void)
{
   uint32_t result = AR_EOK;

   // check if this is the first time this function is being called
   // if first time, only then create the LPM region and heap manager
   if (0 == lpm_client_reg_count)
   {
      AR_MSG(DBG_HIGH_PRIO, "LPM client CSMD registering");
      lpm_client_reg_count++;
   }
   else
   {
      // shouldn't be called more than once
      lpm_client_reg_count++;
      return AR_EOK;
   }
   uint32_t size         = SHARED_MEM_REG_SIZE; // hex 1000
   uint32_t phy_addr_lsw = SHARED_MEM_REG_START_ADDR;

   qurt_mem_pool_t pool = 0;

   char pool_name[] = "ADSP_DRIVER_POOL";
   int  res         = qurt_mem_pool_attach(pool_name, &pool);

   if (QURT_EOK != res)
   {
      AR_MSG(DBG_ERROR_PRIO, "Failed to attach to memory pool, LPM cannot be used!");
      return AR_EFAILED;
   }

   // initialize region attributes
   qurt_mem_region_attr_t reg_attr;
   qurt_mem_region_attr_init(&reg_attr);

   qurt_mem_region_attr_set_cache_mode(&reg_attr, QURT_MEM_CACHE_NONE_SHARED);
   qurt_mem_region_attr_set_mapping(&reg_attr, QURT_MEM_MAPPING_PHYS_CONTIGUOUS);
   qurt_mem_region_attr_set_physaddr(&reg_attr, (qurt_addr_t)phy_addr_lsw);
   qurt_mem_region_attr_set_type(&reg_attr, QURT_MEM_REGION_LOCAL);

   // create the region
   res = qurt_mem_region_create(&lpm_region, size, pool, &reg_attr);

   if (QURT_EOK != res)
   {
      AR_MSG(DBG_ERROR_PRIO, "Failed to create memory region, LPM cannot be used!");
      return AR_EFAILED;
   }

   // get updated attributes
   res = qurt_mem_region_attr_get(lpm_region, &reg_attr);

   if (QURT_EOK != res)
   {
      AR_MSG(DBG_ERROR_PRIO, "Failed to get memory region attributes, Calling deregister!");
      csmd_lpm_sm_deregister();
      return AR_EFAILED;
   }

   uint32_t virt_addr = 0;
   qurt_mem_region_attr_get_virtaddr(&reg_attr, (unsigned int *)(&virt_addr));

   return result;
}
/**
@brief Destroy and deregister the created memory with the ADSP

@param[in] none

@return  ADSP error code

*/
uint32_t csmd_lpm_sm_deregister(void)
{
   // check if this is the last free to be done
   if (lpm_client_reg_count)
   {
      lpm_client_reg_count--;

      if (0 == lpm_client_reg_count)
      {
         AR_MSG(DBG_HIGH_PRIO, "LPM client deregistering");

         if (0 != lpm_region)
         {
            qurt_mem_region_delete(lpm_region);
            lpm_region = 0;
         }
      }
   }
   else
   {
      AR_MSG(DBG_ERROR_PRIO, "LPM client already deregistered");
      return AR_EFAILED;
   }
   return AR_EOK;
}
/**
@brief Does an L2VIC register write to clear the level interrrupt

@param[in] none

@return  ADSP error code

*/
void clear_level_interrupt()
{
   uint32 dspIntstatus = inp32(LPASS___SHARED_MEM_BASE + A_LPASS___SHARED_MEM_INTERRUPT_STATUS);
   outp32(LPASS___SHARED_MEM_BASE + A_LPASS___SHARED_MEM_INTERRUPT_CLR, dspIntstatus);
}
#endif //COSIM_TRANSPORT
