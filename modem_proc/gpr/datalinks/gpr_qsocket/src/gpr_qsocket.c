/*===========================================================================

Copyright (c) 2019-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //commercial/MPSS.HA.1.1/Main/modem_proc/gpr/datalinks/gpr_qsocket/src/gpr_qsocket.c#1 $
  $Author: mplcsds1 $

=============================================================================*/

/*===========================================================================
                          INCLUDE FILES
===========================================================================*/
#include "qsocket_ipcr.h"
#include "qurt_thread.h"
#include "gpr_qsocket.h"
#include "ar_msg.h"
#include "gpr_api_inline.h"
#include "ar_osal_mem_op.h"

/*===========================================================================
                  CONSTANT / MACRO DACLARATIONS
===========================================================================*/
#define GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES (1 * 1024)
#define GPR_QSOCKET_WAIT_FOR_SVC_TIMEOUT_IN_MS (-1) /* Infinite timeout */
#define GPR_QSOCKET_BROADCAST_PACKET_SIZE_IN_BYTES (100)
#define GPR_QSOCKET_TASK_PRIORITY                                                                                      \
   (181) // Need to check on how to fetch this as mdsp will not have elite thread priorites support
#define GPR_QSOCKET_TASK_STACK_SIZE_IN_BYTES (8 * 1024)
// qsocket thread names
#define GPR_QSOC_MODEM_CLIENT ("GPR_QSOC_MODEM_CLIENT")
#define GPR_QSOC_ADSP_SERVICE ("GPR_QSOC_ADSP_SERVICE")
#define GPR_DUMMY_TH ("NULL")

/*===========================================================================
                        TYPE DECLARATIONS
===========================================================================*/
uint32_t gpr_qsocket_tx(uint32_t port_index, void *buf, uint32_t length);
static uint32_t gpr_qsock_receive_done(uint32_t domain_id, void *buf);
struct qsockaddr_ipcr gpr_qsocket_client_addrs[GPR_PL_NUM_TOTAL_DOMAINS_V];

typedef struct gpr_qsocket_port_info_t gpr_qsocket_port_info_t;

struct gpr_qsocket_port_info_t
{
   uint32_t domain_id;
   char     thread_name[32];
   uint32_t service;
   uint32_t instance;
   int32_t  is_server;
};

typedef struct gpr_qsocket_port_t
{
   ipcr_name_t   socket_name; // stores the socket name
   int32_t       is_service;  // indicates service/client
   int32_t       port_index;
   int32_t       dst_domain_id;
   int32_t       src_domain_id;
   int32_t       data_fd;      // stores the data socket
   int32_t       ctrl_fd;      // stores control socket information
   int32_t       is_connected; // indicates connection established between end points
   qurt_thread_t thread_id;    // thread information
   void *        thread_stack; // thread stack
} gpr_qsocket_port_t;

/* Global struct to store GPR callback functions */
gpr_to_qsocket_cb_fn_t qsocket_cb_fn_tbl;

/*===========================================================================
                        GLOBAL VARIABLES
===========================================================================*/

gpr_qsocket_port_t *  gpr_qsocket_port_info[GPR_PL_NUM_TOTAL_DOMAINS_V];
struct qsockaddr_ipcr sender_addr;
void *                gpr_qsocket_temp_rcve_buffer;
qurt_thread_attr_t    gpr_qsocket_thread_attrs[GPR_PL_NUM_TOTAL_DOMAINS_V];
qurt_thread_t         gpr_qsocket_thread_ids[GPR_PL_NUM_TOTAL_DOMAINS_V];
static bool_t         first_dl_init_done = FALSE;

/* qsocket related port data for all domains */
static gpr_qsocket_port_info_t gpr_qsocket_port_info_table[GPR_PL_NUM_TOTAL_DOMAINS_V] =
   { { GPR_IDS_DOMAIN_ID_INVALID_V, GPR_DUMMY_TH, 0x1005, 2, 0 },
     { GPR_IDS_DOMAIN_ID_MODEM_V, GPR_QSOC_MODEM_CLIENT, 0x1005, 2, 0 },
     { GPR_IDS_DOMAIN_ID_ADSP_V, GPR_QSOC_ADSP_SERVICE, 0x1005, 2, 1 }, // server/service
     { GPR_IDS_DOMAIN_ID_APPS_V, GPR_DUMMY_TH, 0x1005, 2, 0 },
     { GPR_IDS_DOMAIN_ID_SDSP_V, GPR_DUMMY_TH, 0x1005, 2, 0 },
     { GPR_IDS_DOMAIN_ID_CDSP_V, GPR_DUMMY_TH, 0x1005, 2, 0 } };

/* glink ipc datalink function table*/
static ipc_to_gpr_vtbl_t qsocket_vtbl = {
   gpr_qsocket_tx,
   gpr_qsock_receive_done,
};

/*===========================================================================
                        LOCAL FUNCTIONS
===========================================================================*/

/* Function to receive the data from given client or service name.*/
static uint32_t gpr_qsock_receive_done(uint32_t domain_id, void *buf)
{
   return AR_EOK;
}

/* Function for service to receive the packets */
static int32_t gpr_qsocket_service_receive_data(int32_t port_index)
{

   int32_t rc = AR_EOK, len;
   // gpr_cmd_async_send_t send_args;
   uint32_t             src_domain_id;
   gpr_packet_t *       temp_buffer;
   gpr_qsocket_port_t * port           = gpr_qsocket_port_info[port_index];
   qsocklen_t           sender_addrlen = sizeof(sender_addr);
   gpr_cmd_alloc_send_t alloc_args;

   // Read the next available packet size (and not the actual packet) from the connected endpoint/client
   rc = qrecv(port->data_fd, NULL, 0, QMSG_PEEK);

   // negative error code indicates failure
   if (rc < 0)
   {
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOC_SERVICE:Packet peeking returned failure: %d\n", rc);
      return AR_EFAILED;
   }

   if (rc > GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR_QSOC_SERVICE:Cannot receive as rcved pkt size:[%d] > expected packet size:[%d]",
             rc,
             GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES);
   }

   len = (size_t)rc;
   rc  = qrecvfrom(port->data_fd,
                  gpr_qsocket_temp_rcve_buffer,
                  len,
                  QMSG_DONTWAIT,
                  (struct qsockaddr *)&sender_addr,
                  &sender_addrlen);

   if (rc != (int)len) // Check length copied and peaked length are same, else bailout...
   {
      // ipc_router_os_free(gpr_qsocket_rcve_tmp_buffer);
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOC_SERVICE:Received [%d] and Peaked length [%d] are not matching", rc, (int)len);
      return AR_EFAILED;
   }
   else // If packet is peaked successfully, now process it.
   {
      temp_buffer = ((gpr_packet_t *)gpr_qsocket_temp_rcve_buffer);

      AR_MSG(DBG_HIGH_PRIO,
             "GPR_QSOCKET_SERVICE: Incoming packet opcode (%lx)",
             temp_buffer->opcode); // dbg purposer remove later..

      // If it's not a broadcast pkt, forward to intended clients..
      if (temp_buffer->opcode != GPR_CMDID_HACK_INIT_IPC)
      {

         // Send the packet to the intended users/clients.

         // rc = qsocket_cb_fn_tbl.rx_cb((void *)temp_buffer, len);
         alloc_args.src_port      = temp_buffer->src_port;
         alloc_args.src_domain_id = temp_buffer->src_domain_id;
         alloc_args.dst_port      = temp_buffer->dst_port;
         alloc_args.dst_domain_id = temp_buffer->dst_domain_id;
         alloc_args.client_data   = temp_buffer->client_data;
         alloc_args.token         = temp_buffer->token;
         alloc_args.opcode        = temp_buffer->opcode;
         alloc_args.payload_size  = GPR_PKT_GET_PAYLOAD_BYTE_SIZE(temp_buffer->header);
         alloc_args.payload       = GPR_PKT_GET_PAYLOAD(void, temp_buffer);

         rc = __gpr_cmd_alloc_send(&alloc_args);

         if (rc != AR_EOK)
         {
            AR_MSG(DBG_ERROR_PRIO, "GPR_QSOC_SERVICE:Packet send returned failure [%d]", rc);
            // ipc_router_os_free(temp_buffer);
            // return AR_EFAILED;
         }
      }
      // If it's a broadcast packet then store the client address
      else
      {
         src_domain_id = temp_buffer->src_domain_id;
         AR_MSG(DBG_HIGH_PRIO,
                "GPR_QSOC_SERVICE: broadcast opcode [0x%08x] src_port [0x%x]",
                temp_buffer->opcode,
                temp_buffer->src_port);
         AR_MSG(DBG_HIGH_PRIO,
                "GPR_QSOC_SERVICE:broadcast packet: got [%d] bytes from client [%d]:[%d]",
                rc,
                sender_addr.address.addr.port_addr.node_id,
                sender_addr.address.addr.port_addr.port_id);
         // Store the client address for further processing..
         ar_mem_cpy((char *)&gpr_qsocket_client_addrs[src_domain_id],
                    sizeof(gpr_qsocket_client_addrs[src_domain_id]),
                    (char *)&sender_addr,
                    sender_addrlen);
         port->is_connected = 1;
      }
   }

   return AR_EOK;
}

/* Client pkt rcve data function */
static int32_t gpr_qsocket_client_receive_data(int32_t port_index)
{

   int32_t       rc = AR_EOK, len;
   gpr_packet_t *temp_buffer;
   // gpr_cmd_async_send_t send_args;
   gpr_qsocket_port_t * port = gpr_qsocket_port_info[port_index];
   gpr_cmd_alloc_send_t alloc_args;
   // uint32_t header_size;

   // Read the next available packet size (and not the actual packet) from the connected endpoint/service
   rc = qrecv(port->data_fd, NULL, 0, QMSG_PEEK);

   if (rc < 0)
   {
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT:Packet peeking returned failure: %d\n", rc);
      return AR_EFAILED;
   }

   // negative error code indicates failure
   if (rc > GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR_QSOCKET_CLIENT:Cannot copy as rcved pkt size:[%d] > expected packet size:[%d]",
             rc,
             GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES);
   }

   len = (size_t)rc;
   rc  = qrecvfrom(port->data_fd, gpr_qsocket_temp_rcve_buffer, (size_t)len, QMSG_DONTWAIT, NULL, NULL);

   if (rc != (int)len)
   {
      // ipc_router_os_free(gpr_qsocket_rcve_tmp_buffer);
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT:Recved %d and Peaked length (%d) are not matching", rc, (int)len);
      return AR_EFAILED;
   }
   else // If packet is peaked successfully, now process it.
   {
      temp_buffer = ((gpr_packet_t *)gpr_qsocket_temp_rcve_buffer);

      AR_MSG(DBG_HIGH_PRIO,
             "GPR_QSOCKET_CLIENT: Incoming packet opcode (0x%08x)",
             temp_buffer->opcode); // dbg purposer remove later..

      {
         // Send the packet to the intended users.

         // rc = qsocket_cb_fn_tbl.rx_cb((void *)temp_buffer, len);
         alloc_args.src_port      = temp_buffer->src_port;
         alloc_args.src_domain_id = temp_buffer->src_domain_id;
         alloc_args.dst_port      = temp_buffer->dst_port;
         alloc_args.dst_domain_id = temp_buffer->dst_domain_id;
         alloc_args.client_data   = temp_buffer->client_data;
         alloc_args.token         = temp_buffer->token;
         alloc_args.opcode        = temp_buffer->opcode;
         alloc_args.payload_size  = GPR_PKT_GET_PAYLOAD_BYTE_SIZE(temp_buffer->header);
         alloc_args.payload       = GPR_PKT_GET_PAYLOAD(void, temp_buffer);

         rc = __gpr_cmd_alloc_send(&alloc_args);

         if (rc != AR_EOK)
         {
            AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT:Packet send returned failure [%d]", rc);
            // ipc_router_os_free(temp_buffer);
            // return AR_EFAILED;
         }
      }
   }

   return AR_EOK;
}

static int32_t gpr_qsocket_wait_for_service_connect(ipcr_name_t *svc, int timeout, int data_fd, int32_t *ctrl_fd)
{

   int                   rc = AR_EOK;
   struct qsockaddr_ipcr srv_addr;
   struct qpollfd        pfd;

   /* Create a socket */
   *ctrl_fd = qsocket(AF_IPC_ROUTER, QSOCK_DGRAM, 0);
   if (*ctrl_fd < 0)
   {
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT:Socket creation failed. ctrl_fd = %d", *ctrl_fd);
      return -1;
   }

   /* Mark this as a control socket to listen on events
    * on the name server */
   rc = qsetsockopt(*ctrl_fd, QSOL_IPC_ROUTER, QSO_IPCR_SET_CONTROL_PORT, NULL, 0);
   if (rc)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR_QSOCKET_CLIENT: qsetsockopt(SET_CONTROL_PORT) failed with rc = %d, closing the socket = %d",
             rc,
             *ctrl_fd);
      qclose(*ctrl_fd);
      return -1;
   }

   /* Specify the service ID "0x1005" that we are intersted in */
   rc = qsetsockopt(*ctrl_fd, QSOL_IPC_ROUTER, QSO_IPCR_SET_SERVICE_PREF, &svc->service, sizeof(svc->service));
   if (rc)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR_QSOCKET_CLIENT: qsetsockopt(SET_SERVICE_PREF) failed with rc= %d, closing the socket = %d",
             rc,
             *ctrl_fd);
      qclose(*ctrl_fd);
      return -1;
   }

   pfd.fd      = *ctrl_fd;
   pfd.events  = QPOLLIN;
   pfd.revents = 0;

   while (1)
   {

      unsigned int  num_entries = 1;
      ipcr_ctrl_msg msg;
      rc = ipcr_find_name(data_fd, svc, &srv_addr, NULL, &num_entries, 0);

      if (rc > 0)
      {
         AR_MSG(DBG_HIGH_PRIO,
                "GPR_QSOCKET_CLIENT: found port with node_id(%u), port_id(%u) matching service name",
                srv_addr.address.addr.port_addr.node_id,
                srv_addr.address.addr.port_addr.port_id);

         // Once service name is found, connect the socket "data_fd" to the endpoint/service.
         rc = qconnect(data_fd, (struct qsockaddr *)&srv_addr, sizeof(srv_addr));
         if (rc < 0)
         {
            AR_MSG(DBG_ERROR_PRIO,
                   "GPR_QSOCKET_CLIENT: connecting to service with node_id(%u), port_id(%u) failed with result:%d",
                   srv_addr.address.addr.port_addr.node_id,
                   srv_addr.address.addr.port_addr.port_id,
                   rc);
         }
         break;
      }

      /* Wait (infinite timeout) until the service is up */
      rc = qpoll(&pfd, 1, timeout);
      if (rc < 0)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: qpoll for service returned error = %d", rc);
         qclose(data_fd);
         break;
      }
      else if (rc == 0)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: qpoll for servicec timedout!");
         rc = -1;
         qclose(data_fd);
         break;
      }
      else
      {
         while (qrecv(*ctrl_fd, &msg, sizeof(msg), QMSG_DONTWAIT) > 0)
         {
         }
      }
   }

   return rc;
}

GPR_INTERNAL void gpr_qrecv_workloop_server(void *thread_params)
{

   int32_t               rcve_status   = AR_EOK;
   int32_t               rc            = AR_EOK;
   int32_t               return_status = AR_EOK;
   int32_t               port_index;
   struct qsockaddr_ipcr svc_name;
   gpr_qsocket_port_t *  port;

   port_index      = *(int32_t *)thread_params;
   port            = gpr_qsocket_port_info[port_index]; /*Pointer to the gpr qsocket port*/
   port->thread_id = qurt_thread_get_id();              // Get the thread id for later use

   AR_MSG(DBG_ERROR_PRIO, "Server workloop in progress :%d", *(int32_t *)thread_params);

   // create a socket
   port->data_fd = qsocket(AF_IPC_ROUTER, QSOCK_DGRAM, 0);

   if (port->data_fd < 0)
   {
      AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_SERVICE: socket creation failed. data_fd = %d", port->data_fd);
   }

   /*Make the socket structure filled up and ready for binding in to qsocket */
   svc_name.sa_family              = AF_IPC_ROUTER;
   svc_name.address.addrtype       = IPCR_ADDR_NAME;
   svc_name.address.addr.port_name = port->socket_name;

   /* Bind the socket/port to the server, so that clients will get notified/connected */
   rc = qbind(port->data_fd, (struct qsockaddr *)&svc_name, sizeof(svc_name));

   if (rc < 0)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "GPR_QSOCKET_SERVICE:qbind failed with result = %d, closing the socket %d",
             rc,
             port->data_fd);
      // qclose(port->data_fd);
      return_status = AR_EFAILED;
   }

   /* Enter to infinite loop for receiving the packets */
   do
   {
      rcve_status = gpr_qsocket_service_receive_data(port_index);

      if (rcve_status != AR_EOK)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_SERVICE: service_receive_data received error [%d]", rcve_status);
         return_status = rcve_status;
         // qclose(port->data_fd); //clean up by closing the ports..
         break; // exit on failure..
      }

   } while (return_status == AR_EOK); // If no failure, continue to rcve pkts

   qclose(port->data_fd); // clean up by closing the ports before thread close..
}

/*client workloop */
GPR_INTERNAL void gpr_qrecv_workloop_client(void *thread_params)
{

   int                   rc, rcve_status = 0;
   struct qpollfd        pfd[2];
   gpr_qsocket_port_t *  port;
   int32_t               port_index;
   struct qsockaddr_ipcr svc_addr;
   int                   return_status = AR_EOK;
   // uint32_t my_handle=0;

   port_index = *(int32_t *)thread_params;

   AR_MSG(DBG_ERROR_PRIO, "Client workloop in progress :%d", *(int32_t *)thread_params);
   port            = gpr_qsocket_port_info[port_index];
   port->thread_id = qurt_thread_get_id(); // Get the thread id for later use

   // Put all here if it is needed to be done once..
   gpr_packet_t broadcast_packet;

   /* Client enters this loop to process below
   1. Open data socket
   2. open control socket
   3. wait for server to come up
   */
   do
   {
      if (rcve_status != AR_EOK)
      {
         break; // exit...
      }

      /* Create a socket */
      port->data_fd = qsocket(AF_IPC_ROUTER, QSOCK_DGRAM, 0);

      if (port->data_fd < 0)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: socket creation failed. data_fd = %d", port->data_fd);
         break;
      }

      // Wait for service to come up...
      if (gpr_qsocket_wait_for_service_connect(&port->socket_name,
                                               GPR_QSOCKET_WAIT_FOR_SVC_TIMEOUT_IN_MS,
                                               port->data_fd,
                                               &port->ctrl_fd) < 0)
      {
         AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: service did not start within the expected wait time!");
         qclose(port->data_fd);
         break;
      }
      else
      {
         AR_MSG(DBG_HIGH_PRIO, "GPR_QSOCKET_CLIENT: service started!");
         port->is_connected = 1;
      }

      // Broadcast client address to service, so that service can send packets to the client..
      /* Multicast the message to all services and wait for one to respond */
      svc_addr.sa_family              = AF_IPC_ROUTER;
      svc_addr.address.addrtype       = IPCR_ADDR_NAME;
      svc_addr.address.addr.port_name = port->socket_name;

      // Prepare the packet for broadcast..

      /*Broadcast Packet packing */
      broadcast_packet.dst_domain_id = port->dst_domain_id;
      broadcast_packet.src_domain_id = port->src_domain_id;
      broadcast_packet.src_port      = AR_EOK;
      broadcast_packet.dst_port      = AR_EOK;
      broadcast_packet.token         = AR_EOK;
      broadcast_packet.opcode        = GPR_CMDID_HACK_INIT_IPC; // Opcode to identify broadcast

      // send the broadcast packet to service
      rc = qsendto(port->data_fd,
                   &broadcast_packet,
                   GPR_QSOCKET_BROADCAST_PACKET_SIZE_IN_BYTES,
                   0,
                   (struct qsockaddr *)&svc_addr,
                   sizeof(svc_addr));

      if (rc == GPR_QSOCKET_BROADCAST_PACKET_SIZE_IN_BYTES)
      {
         AR_MSG(DBG_HIGH_PRIO,
                "GPR_QSOCKET_CLIENT: Broadcast packet to service (address=%d) is successful!",
                port->socket_name.service);
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO,
                "GPR_QSOCKET_CLIENT: Broadcast packet to service (address=%d) is failed!",
                port->socket_name.service);
         return_status = AR_EOK;
         qclose(port->ctrl_fd);
         qclose(port->data_fd);
         break;
      }

      /* This loop will wait on below events
      1. QPOLLHUP --> Remote side has hungup on this port
      2. QPOLLIN  --> when there is an event to receive the packet..
      */

      /* Fill up qpoll struct for ctrl events */
      pfd[0].fd      = port->ctrl_fd; // service going down.up
      pfd[0].events  = QPOLLIN;
      pfd[0].revents = 0;

      /* Fill up qpoll struct for data events */
      pfd[1].fd      = port->data_fd; /// nrmal packets
      pfd[1].events  = QPOLLIN;
      pfd[1].revents = 0;

      while (1)
      {
         rc = qpoll(pfd, 2, -1); //-1 indicates infinite wait..

         if (rc < 0)
         {
            AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: qpoll returned %d\n", rc);
            return_status = rc;
            break;
         }
         else if (rc == 0)
         {
            AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: qpoll timedout!\n", 0, 0, 0);
            continue;
         }
         else
         {
            {                                /* data socket */
               if (pfd[1].revents & QPOLLIN) // incoming packet.
               {
                  // Rcve the packet once a event is notified...
                  rcve_status = gpr_qsocket_client_receive_data(port_index);
                  if (rcve_status != AR_EOK)
                  {
                     AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: Incoming packet receive from service failed..");
                     return_status = rcve_status;
                     break; // exit...
                  }
               }
               else if (pfd[1].revents & QPOLLHUP) // Assuming server restarted or hung (ADSP SSR), restart client...
               {
                  AR_MSG(DBG_ERROR_PRIO,
                         "GPR_QSOCKET_CLIENT: Remote/service disconnected for the client port: %d",
                         port_index);
                  AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: Restarting client");
                  // restart the client connection assuming remote went down..
                  // Restarting client..

                  break;
               }
            }
            {                                /* control socket */
               if (pfd[0].revents & QPOLLIN) /* any incoming packets on control socket? */
               {
                  ipcr_ctrl_msg msg;
                  while (qrecv(port->ctrl_fd, &msg, sizeof(msg), QMSG_DONTWAIT) > 0)
                  {
                     /* just drop the packets*/
                     AR_MSG(DBG_ERROR_PRIO, "GPR_QSOCKET_CLIENT: packets received on control socket: %d", port_index);
                  }
               }
            }
         }

      } // << end while for qpoll..
      qclose(port->data_fd);
      qclose(port->ctrl_fd);

   } while (return_status == AR_EOK);
}

/*===========================================================================
FUNCTION ipc_dl_qsocket_init
Initalizes the needed qsockets and spawns the worker threads for receiving the packets.
===========================================================================*/

GPR_INTERNAL uint32_t ipc_dl_qsocket_init(uint32_t                 src_domain_id,
                                          uint32_t                 dest_domain_id,
                                          const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                          ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl)
{

   int32_t rc, checkpoint = 0;
   int32_t domain_id   = dest_domain_id;
   *pp_ipc_to_gpr_vtbl = &qsocket_vtbl;

   if (!first_dl_init_done)
   {
      /*memset gpr qsocket adress table */
      memset(&gpr_qsocket_client_addrs[0], 0, GPR_PL_NUM_TOTAL_DOMAINS_V * sizeof(struct qsockaddr_ipcr));
      first_dl_init_done = TRUE;

      // Save gpr cb functions in global struct
      qsocket_cb_fn_tbl.rx_cb      = p_gpr_to_ipc_vtbl->receive;
      qsocket_cb_fn_tbl.tx_done_cb = p_gpr_to_ipc_vtbl->send_done;

      /*Create a buffer for copying the packets. All the packets received from qsocket are first copied to this buffer
      and then they are sent to clients using gpr_cmd_alloc_send */
      gpr_qsocket_temp_rcve_buffer = malloc(GPR_QSOCKET_RCVE_BUFFER_SIZE_IN_BYTES);

      if (gpr_qsocket_temp_rcve_buffer == NULL)
      {
         AR_MSG(DBG_ERROR_PRIO, "gpr_qsocket_init: could not allocate memory for rcvng gpr qsocket packets");
         return AR_EFAILED;
      }
   }

   /* Create the attributes needed for thread creation */

   /*Open all the qsockets as per the ipc configuration.*/

   AR_MSG(DBG_HIGH_PRIO, "gpr_qsocket_init for port_index :%d", domain_id);

   gpr_qsocket_port_info[domain_id] = malloc(sizeof(gpr_qsocket_port_t));
   if (gpr_qsocket_port_info[domain_id] == NULL)
   {
      AR_MSG(DBG_ERROR_PRIO, "gpr_qsocket_init: could not allocate memory for gpr qsocket port :%d", domain_id);
      rc         = AR_ENOMEMORY;
      checkpoint = 1;
      goto checkpoint_jump;
   }

   /*Copy all the data to the qsocket port info structure, so that globally this structure can be referred */
   gpr_qsocket_port_info[domain_id]->socket_name.service =
      gpr_qsocket_port_info_table[domain_id].service; // Service address
   gpr_qsocket_port_info[domain_id]->socket_name.instance =
      gpr_qsocket_port_info_table[domain_id].instance; // Instance address
   gpr_qsocket_port_info[domain_id]->is_service    = gpr_qsocket_port_info_table[domain_id].is_server; // service/client
   gpr_qsocket_port_info[domain_id]->is_connected  = 0;
   gpr_qsocket_port_info[domain_id]->dst_domain_id = domain_id;
   gpr_qsocket_port_info[domain_id]->src_domain_id = src_domain_id;

   /*Thread stack creation*/
   gpr_qsocket_port_info[domain_id]->thread_stack = NULL;
   gpr_qsocket_port_info[domain_id]->thread_stack = malloc(GPR_QSOCKET_TASK_STACK_SIZE_IN_BYTES);

   /*Qurt thread creation attributes*/
   qurt_thread_attr_init(&gpr_qsocket_thread_attrs[domain_id]); // Init the params
   qurt_thread_attr_set_name(&gpr_qsocket_thread_attrs[domain_id], gpr_qsocket_port_info_table[domain_id].thread_name);
   qurt_thread_attr_set_stack_addr(&gpr_qsocket_thread_attrs[domain_id],
                                   gpr_qsocket_port_info[domain_id]->thread_stack);
   qurt_thread_attr_set_stack_size(&gpr_qsocket_thread_attrs[domain_id], GPR_QSOCKET_TASK_STACK_SIZE_IN_BYTES);
   qurt_thread_attr_set_priority(&gpr_qsocket_thread_attrs[domain_id], GPR_QSOCKET_TASK_PRIORITY);

   if (gpr_qsocket_port_info[domain_id]->thread_stack == NULL)
   {
      AR_MSG(DBG_ERROR_PRIO,
             "gpr_qsocket_init: %s thread creation failed due to malloc failure",
             gpr_qsocket_port_info_table[domain_id].thread_name);
      rc         = AR_ENOMEMORY;
      checkpoint = 2;
      goto checkpoint_jump;
   }

   /*Service/Client thread creation */
   if (gpr_qsocket_port_info[domain_id]->is_service == 1)
   {
      rc = qurt_thread_create(&gpr_qsocket_thread_ids[domain_id],
                              &gpr_qsocket_thread_attrs[domain_id],
                              gpr_qrecv_workloop_server,
                              (int32_t *)&gpr_qsocket_port_info[domain_id]->dst_domain_id);
   }
   else
   {
      rc = qurt_thread_create(&gpr_qsocket_thread_ids[domain_id],
                              &gpr_qsocket_thread_attrs[domain_id],
                              gpr_qrecv_workloop_client,
                              (int32_t *)&gpr_qsocket_port_info[domain_id]->dst_domain_id);
   }

   if (rc != AR_EOK)
   {
      AR_MSG(DBG_HIGH_PRIO,
             "gpr_qsocket_init: %s thread creation failed with error %d",
             gpr_qsocket_port_info_table[domain_id].thread_name,
             rc);
      rc         = AR_EFAILED;
      checkpoint = 3;
      goto checkpoint_jump;
   }

checkpoint_jump:

   AR_MSG(DBG_ERROR_PRIO, "Cleaning up failed resources %d", checkpoint);

   switch (checkpoint)
   {
      case 3:
         free(gpr_qsocket_port_info[domain_id]->thread_stack);
      /*-fallthru */
      case 2:
         free(gpr_qsocket_port_info[domain_id]);
      /*-fallthru */
      case 1:
         free(gpr_qsocket_temp_rcve_buffer);
      /*-fallthru */
      default:
         break;
   }

   return rc;

} /*<<end  qsocket init >> */

/*Function to send the packets to remote processors/addresses using qsocket*/
GPR_INTERNAL uint32_t gpr_qsocket_tx(uint32_t domain_id, void *buf, uint32_t length)
{

   uint32_t      dst_domain_id;
   int32_t       rc = AR_EOK;
   gpr_packet_t *packet;
   packet = ((gpr_packet_t *)buf);

   dst_domain_id = domain_id;

   AR_MSG(DBG_ERROR_PRIO,
          "gpr_qsocket_tx()sending: client from dest_domain_id(%d) - opcode %lx",
          dst_domain_id,
          packet->opcode);

   /*if iam server, then i will lookup my client address in routing table to send to approriate remote client*/
   if (gpr_qsocket_port_info[dst_domain_id]->is_service == 1)
   {
      /* if client is registered with service*/
      if (gpr_qsocket_client_addrs[dst_domain_id].address.addr.port_addr.port_id != 0)
      {
         rc = qsendto(gpr_qsocket_port_info[dst_domain_id]->data_fd,
                      buf,
                      length,
                      0,
                      (struct qsockaddr *)&gpr_qsocket_client_addrs[dst_domain_id],
                      sizeof(struct qsockaddr_ipcr));
         if (rc < 0)
         {
            AR_MSG(DBG_ERROR_PRIO,
                   "gpr_qsocket_tx(): qsendto() from service to client failed. socket descriptor=(%d), "
                   "dest_domain_id=(%d)",
                   gpr_qsocket_port_info[dst_domain_id]->data_fd,
                   dst_domain_id);
         }
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO,
                "gpr_qsocket_tx() failed: client from dest_domain_id(%d) not registered",
                dst_domain_id);
         return AR_ENOTEXIST; // client does not exists..
      }
   }
   else
   {
      /* if i am client, i can send to service directly, as i am a connected service */
      rc = qsend(gpr_qsocket_port_info[dst_domain_id]->data_fd, buf, length, 0);
      if (rc < 0)
      {
         AR_MSG(DBG_ERROR_PRIO,
                "gpr_qsocket_tx(): qsend() from client to service failed. socket descriptor=(%d)",
                gpr_qsocket_port_info[dst_domain_id]->data_fd);
      }
   }

   /* if the sent bytes is not matching with the returned bytes, then treat as a failure */
   if (rc != length)
   {
      if (rc == QEHOSTUNREACH) /* Host is unreachable */
      {
         AR_MSG(DBG_HIGH_PRIO, "gpr_qsocket_tx() failed: Client is down [%d], sent to [%d] failed", rc, dst_domain_id);
      }
      else
      {
         AR_MSG(DBG_HIGH_PRIO, "gpr_qsocket_tx() failed: sent [%d], expected [%d]", rc, length);
      }
      return AR_EFAILED;
   }
   else
   {
      /* Free the packet after the packet is placed in qsocket FIFO */
      (void)__gpr_cmd_free(packet);
      return AR_EOK;
   }
}

GPR_INTERNAL uint32_t ipc_dl_qsocket_deinit(uint32_t src_domain_id, uint32_t dest_domain_id)
{
   AR_MSG(DBG_HIGH_PRIO, "ipc dl deinit in progress.. for %d", dest_domain_id);

   if (first_dl_init_done)
   {
      free(gpr_qsocket_temp_rcve_buffer);
      first_dl_init_done = FALSE;
   }
   free(gpr_qsocket_port_info[dest_domain_id]->thread_stack);
   free(gpr_qsocket_port_info[dest_domain_id]);

   return AR_EOK;
}
