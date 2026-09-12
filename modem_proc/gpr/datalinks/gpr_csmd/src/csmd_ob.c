/*========================================================================

*/ /** @file csmd_ob.c
This file contains the implementations of the
functions/APIs exposed by the Xport Layer to the
GPR client and are
primarily invoked during out of band transfers.
 The functions directly transact with the shared
memory.

Copyright (c) 2018-2020, 2021 Qualcomm Technologies,Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
                                                                        */
/*========================================================================
Edit History

$Header: L:\Projects\avs.adsp.2.9.Mar8_SIM_2_9_gpr\adsp_proc\avs\main\gpr\datalink\gpr_csmd\src\csmd_ob.c


when       who       what, where, why
--------  -----     -------------------------------------------------------
05/03/18   akr      Created file.

========================================================================== */
/*
Amith:
Algorithm: For each memmap command that comes in, we get num_nodes
           we malloc that many nodes and mark them with indices in the reverse order.
           ForEx:3 memmap commands: 3 nodes, 2 nodes and 1 node  3->2->1->2->1->1
           We store the mapping information for each memory chunk in each node.
           The handle that is returned is the address of the starting node of
           the "region". In the example, for the forst command, the handle will
           be that of 3, for the second, it'll be the '2' in the center and so on.
           When Virt Addr is queried, we just search in the region and return the
           correct address.
*/
/*==========================================================================
Include files
========================================================================== */
#include "csmd_api.h"
#include "csmd_ob.h"
#include "ar_msg.h"
#include "internal_csmd.h"
#ifdef COSIM_TRANSPORT
#include "qube.h"
#include "qurt_memory.h"
/*==========================================================================
COSIM globals
========================================================================== */
mem_map_node_t *head_node = NULL;
mem_map_node_t *tail_node = NULL;
/*==========================================================================
Function Definitions
========================================================================== */
void *csmd_aligned_malloc_64(uint32_t unBytes)
{

   char *   ptr, *ptr2, *aligned_ptr;
   uint32_t unAlignSize = 64; // always
   uint32_t align_mask  = ~(unAlignSize - 1);

   /* allocate enough for requested bytes + alignment wasteage + 1 word for storing offset
    * (which will be just before the aligned ptr) */
   ptr = (char *)malloc(unBytes + unAlignSize + sizeof(int));
   if (ptr == NULL)
      return (NULL);

   /* allocate enough for requested bytes + alignment wasteage + 1 word for storing offset */
   ptr2        = ptr + sizeof(int);
   aligned_ptr = (char *)((uint32_t)(ptr2 - 1) & align_mask) + unAlignSize;

   /* save offset to raw pointer from aligned pointer */
   ptr2           = aligned_ptr - sizeof(int);
   *((int *)ptr2) = (int)(aligned_ptr - ptr);

   return (aligned_ptr);
}

void csmd_aligned_free_64(void *ptr)
{
   if (NULL == ptr)
   {
      return;
   }

   uint32_t *pTemp = (uint32_t *)ptr;
   uint32_t *ptr2  = pTemp - 1;

   /* Get the base pointer address */
   pTemp -= *ptr2 / sizeof(uint32_t);

   /* Free the memory */
   free(pTemp);
}

/**
@brief API to allocate the Shared memory region like with various memory chunks and bookkeep

@param[in] *regions: array which contains list of remote addresses and sizes

@param[in] num_nodes: number of individual memory chunks in the region

@param[in] is_offset_map: to indicate if the mapping  will be queried by offset later

@param[in] is_va_map: to indicate if the mapping is virtual (or physical)

@return handle for the region

*/
uint32_t csmd_malloc_map_region(uint32_t num_nodes, void *regions, bool_t is_offset_map, bool_t is_va_map)
{
   memorymap_shm_region_t *remote_shm = (memorymap_shm_region_t *)regions;
   uint32_t                ret_handle = NULL; // starting node for each command
   uint32_t                index      = num_nodes;

   for (uint32_t i = 0; i < num_nodes; i++)
   {
      void *ptr_local = NULL;
      if (!is_va_map)
      {
         ptr_local = csmd_aligned_malloc_64(remote_shm[i].mem_size);
         if (NULL == ptr_local)
         {
            return 0;
         }
      }
      if (NULL == tail_node) // first node
      {
         tail_node = (mem_map_node_t *)malloc(sizeof(mem_map_node_t));
         if (NULL == tail_node)
         {
            return 0; // 0 handle will be errored out by the caller
         }
         head_node       = tail_node; // at the beginning, head and tail are together
         tail_node->next = NULL;
      }
      else
      {
         tail_node->next = (mem_map_node_t *)malloc(sizeof(mem_map_node_t));
         if (NULL == tail_node->next)
         {
            return 0; // 0 handle will be errored out by the caller
         }
         tail_node = tail_node->next;
      }

      tail_node->ptr_local     = ptr_local; //will be null for va map node.. 
      tail_node->addr_remote   = remote_shm[i].shm_addr_lsw;
      tail_node->size          = remote_shm[i].mem_size;
      tail_node->index         = index;
      tail_node->is_offset_map = is_offset_map;
      tail_node->is_va_map     = is_va_map;
      tail_node->next          = NULL; // tail is tracked

      AR_MSG(DBG_HIGH_PRIO,
             "Mapping ptr_local = %p with addr_remote %p, Index = %lu",
             (void *)ptr_local,
             (void *)tail_node->addr_remote,
             index);

      if (index == num_nodes)
      {
         ret_handle = (uint32_t)tail_node;
      }
      index--;
   }
   return ret_handle;
}
/**
@brief API to de-allocate the mirror memory and un map the from the list

@param[in] ob region handle

@return none
*/
uint32_t csmd_free_unmap_region(uint32_t *handle)
{
   if (NULL == handle)
   {
      AR_MSG(DBG_ERROR_PRIO, "Invalid handle");
      return AR_EBADPARAM;
   }
   AR_MSG(DBG_HIGH_PRIO, "In CSMD FREE and UNMAP");
   /*Iterators
   prev - starts from the head_node and stops at the node prior to curr
   curr - starts at the node - start of the region to be unmapped
   next - used as needed*/
   mem_map_node_t *prev = head_node; // iterator
   mem_map_node_t *curr = (mem_map_node_t *)handle;
   mem_map_node_t *next = NULL;
   uint32_t        count;
   // if we've to delete the first region
   if (prev == curr)
   {
      count = head_node->index;
      while (0 != count)
      {
         prev      = head_node;
         head_node = head_node->next;
         csmd_aligned_free_64(prev->ptr_local);
         free(prev);
         count--;
      }
      if (NULL == head_node)
      {
         // was the only region
         tail_node = NULL;
      }
      return AR_EOK;
   }
   // else - not the first region
   while ((NULL != prev) && (prev->next != curr))
   {
      // find the region boundary
      // prev stops at a node behind curr (region to remove)
      prev = prev->next;
   }
   if (NULL == prev)
   {
      // Error
      AR_MSG(DBG_ERROR_PRIO, "Incorrect handle %p provided to unmap -> not mapped or already unmapped", (void *)handle);
      return AR_EBADPARAM;
   }
   // let's proceed to delete the region of interest
   count = curr->index;
   while (0 != count)
   {
      next = curr->next;
      csmd_aligned_free_64(curr->ptr_local);
      free(curr);
      curr = next;
      count--;
   }
   prev->next = curr; // or next - both are now beyond the region
   if (NULL == curr)
   {
      tail_node = prev;
   }
   return AR_EOK;
}

/**
@brief API to compute and return the virtual address to be accessed in the local domain

@param[in] *handle: handle specific to a region

@param[in] *ptr1_remote: pointer to an address in the OB memory chunk on the remote proc

@return virtual address in the adsp space

*/
uint32_t csmd_get_virt_addr(uint32_t *handle, void *ptr1_remote)
{
   if (NULL == handle)
   {
      return 0; // errors out in posal layer
   }
   mem_map_node_t *node          = (mem_map_node_t *)handle;
   uint64_t        src_virt_addr = 0;

   // if we're here, we're at the last node in the region (can be the only offset mode region as well)
   if (TRUE == node->is_offset_map)
   {
      // Local VA + incoming OFFSET
      src_virt_addr = (uint64_t)(node->ptr_local) + (uint64_t)ptr1_remote;
      if (src_virt_addr > (uint64_t)(node->ptr_local) + node->size)
      {
         AR_MSG(DBG_ERROR_PRIO, "CSMD_OB: Query Error: Provided offset %p is out of bounds", (void *)ptr1_remote);
         return 0; // error
      }
   }
   else // not offset mode
   {

      // Traverse the region LL
      //(will not enter the while loop in offset mode since there's only one region (node)by definition for offset
      // mode)
      while ((NULL != node) && (NULL != node->next) && (node->next->index == node->index - 1))
      {
         if ((ptr1_remote >= (void *)node->addr_remote) &&
             ((char_t *)ptr1_remote < ((char_t *)node->addr_remote + node->size)))
         {
            if(node->is_va_map)
            {
               return (uint32_t)ptr1_remote; //return the same address
            }
            // Local VA + Remote OFFSET
            src_virt_addr =
               (uint64_t)((char_t *)(node->ptr_local) + ((char_t *)ptr1_remote - (char_t *)(node->addr_remote)));
            return src_virt_addr;
         }
         node = node->next;
      }

      if (NULL == node)
      {
         AR_MSG(DBG_ERROR_PRIO, "No map found with this remote address %p", (void *)ptr1_remote);
         return 0; // error
      }

      if ((ptr1_remote >= (void *)node->addr_remote) &&
          ((char_t *)ptr1_remote < ((char_t *)node->addr_remote + node->size)))
      {
         if (node->is_va_map)
         {
            return (uint32_t)ptr1_remote; // return the same address
         }
         // Local VA + Remote OFFSET
         src_virt_addr =
            (uint64_t)((char_t *)(node->ptr_local) + ((char_t *)ptr1_remote - (char_t *)(node->addr_remote)));
      }
      else
      {
         AR_MSG(DBG_ERROR_PRIO,
                "No map found with this remote address %p in this region %p",
                (void *)ptr1_remote,
                (void *)handle);
      }
   }
   return src_virt_addr;
}
/**
@brief API to bring in the data from the out of band memory to the Q6 mirror memory

@param[in] *virt_addr: Q6 Virtual Address

@param[in] mem_size: size of the memory to mirror

@return ar_result

*/
uint32_t csmd_mem_invalidate(uint32_t virt_addr, uint32_t mem_size)
{
   // here we need to bring in the data into the mirror memory

   // here, we have to traverse our entire list, looking for the remote VA
   mem_map_node_t *temp             = head_node;
   uint32_t        offset           = 0;
   uint64_t        remote_virt_addr = 0;
   while (NULL != temp)
   {
      if (((void *)virt_addr >= (void *)temp->ptr_local) &&
          ((char_t *)virt_addr < (char_t *)temp->ptr_local + temp->size))
      {
         // we're in the correct node now.
         if (temp->is_va_map)
         {
            // no cosim exchange.. Just clean and move on
            if ((EOK !=
                 qurt_mem_cache_clean((qurt_addr_t)(virt_addr), mem_size, QURT_MEM_CACHE_INVALIDATE, QURT_MEM_DCACHE)))
            {
               return AR_EFAILED;
            }
            return AR_EOK;
         }
         offset           = (uint32_t)((char_t *)virt_addr - (char_t *)temp->ptr_local);
         remote_virt_addr = (uint64_t)((char_t *)temp->addr_remote + offset);
         break;
      }
      temp = temp->next;
   }

   if (NULL != temp)
   {
      // Let's send a command into the cosim to do this
      csmd_pkt_t to_send;
      to_send.opcode     = CSMD_CMD_INVALIDATE_REGION;
      to_send.channel_id = 0xFFFF; // don't care
      to_send.size       = sizeof(csmd_cmd_invalidate_flush_region_t);

      csmd_cmd_invalidate_flush_region_t data;
      data.src_phy_addr  = qurt_lookup_physaddr_64((qurt_addr_t)virt_addr);
      data.dst_phy_addr  = 0; // not used
      data.src_virt_addr = 0; // not used
      data.dst_virt_addr = remote_virt_addr;
      data.size          = mem_size;
      // bring in the data with a command
      xport_send((void *)&to_send, sizeof(csmd_pkt_t), &data, sizeof(csmd_cmd_invalidate_flush_region_t));
   }
   // else
   // {
   //    AR_MSG(DBG_ERROR_PRIO, "No map found with this virt address %p", (void *)virt_addr);
   // }

   if ((EOK != qurt_mem_cache_clean((qurt_addr_t)(virt_addr), mem_size, QURT_MEM_CACHE_INVALIDATE, QURT_MEM_DCACHE)))
   {
      return AR_EFAILED;
   }
   return AR_EOK;
}
/**
@brief API to bring in the data from the out of band memory to the Q6 mirror memory

@param[in] *virt_addr: Q6 Virtual Address

@param[in] mem_size: size of the memory to mirror

@return ar_result

*/
uint32_t csmd_mem_flush(uint32_t virt_addr, uint32_t mem_size)
{
   if (0 >= mem_size)
   {
      return AR_EBADPARAM;
   }
   // here, we have to traverse our entire list, looking for the remote VA
   mem_map_node_t *temp             = head_node;
   uint32_t        offset           = 0;
   uint64_t        remote_virt_addr = 0;
   while (NULL != temp)
   {
      if (((void *)virt_addr >= (void *)temp->ptr_local) &&
          ((char_t *)virt_addr < (char_t *)temp->ptr_local + temp->size))
      {
         // we're in the correct node now.
         if (temp->is_va_map)
         {
            // no cosim exchange.. Just clean and move on
            if ((EOK !=
                 qurt_mem_cache_clean((qurt_addr_t)(virt_addr), mem_size, QURT_MEM_CACHE_FLUSH, QURT_MEM_DCACHE)))
            {
               return AR_EFAILED;
            }
            return AR_EOK;
         }
         offset           = (uint32_t)((char_t *)virt_addr - (char_t *)temp->ptr_local);
         remote_virt_addr = (uint64_t)((char_t *)temp->addr_remote + offset);
         break;
      }
      temp = temp->next;
   }
   if (NULL != temp)
   {
      // Let's send a command into the cosim to do this
      csmd_pkt_t to_send;
      to_send.opcode     = CSMD_CMD_FLUSH_REGION;
      to_send.channel_id = 0xFFFF; // don't care
      to_send.size       = sizeof(csmd_cmd_invalidate_flush_region_t);

      csmd_cmd_invalidate_flush_region_t data;
      data.src_phy_addr  = qurt_lookup_physaddr_64((qurt_addr_t)virt_addr);
      data.dst_phy_addr  = 0; // not used
      data.src_virt_addr = 0; // not used
      data.dst_virt_addr = remote_virt_addr;
      data.size          = mem_size;

      // send the data with a command
      xport_send((void *)&to_send, sizeof(csmd_pkt_t), &data, sizeof(csmd_cmd_invalidate_flush_region_t));
   }
   // else
   // {
   //    AR_MSG(DBG_ERROR_PRIO, "No map found with this virt address %p", (void *)virt_addr);
   // }
   if ((EOK != qurt_mem_cache_clean((qurt_addr_t)(virt_addr), mem_size, QURT_MEM_CACHE_FLUSH, QURT_MEM_DCACHE)))
   {
      return AR_EFAILED;
   }
   return AR_EOK;
}
#endif // COSIM_TRANSPORT

#ifdef WINSOCK_TRANSPORT
/*Object of the Global CSMD Context Structure*/
extern csmd_ctx_t csmd_global_ctx;

/**
@brief API to send the requested portion of data from the OB memory chunk to the remote via winsock

@param[in] *recv_buf: buffer containing the data request

@param[in] recv_size: size of the data request buffer

@return : none

*/
uint32_t honour_ob_req(void *recv_buf, uint32_t recv_size)
{
   if (recv_size < sizeof(csmd_pkt_t) + sizeof(csmd_data_read_write_t))
   {
      return AR_EBADPARAM;
   }
   csmd_data_read_write_t *payload_ptr = (csmd_data_read_write_t *)((char_t *)recv_buf + sizeof(csmd_pkt_t));

   // we have to send the data from this part of the memory
   // or copy to this address
   void *   ptr  = (void *)payload_ptr->address;
   uint32_t size = payload_ptr->size;
   if ((NULL == ptr) || (size < 0))
   {
      AR_MSG(DBG_ERROR_PRIO, "honour OB req: Received bad payload");
      return AR_EBADPARAM;
   }
   // parse the payload
   csmd_pkt_t *header = (csmd_pkt_t *)recv_buf;
   switch (header->opcode)
   {
      case CSMD_CMD_DATA_READ:
      {
         AR_MSG(DBG_LOW_PRIO, "Opcode READ, requested size of data = %lu from local address 0x%lx", size, ptr);
         void *data_buf = malloc(size);
         if (NULL == data_buf)
         {
            return AR_ENORESOURCE;
         }
         memcpy(data_buf, ptr, size);
         uint32_t rc;
         rc = send(csmd_global_ctx.transport_info_t.ob_tx_connect_sock, (char_t *)data_buf, size, 0);
         free(data_buf);
         if (rc == SOCKET_ERROR)
         {
            AR_MSG(DBG_ERROR_PRIO, "send failed with error: %d\n", WSAGetLastError());
            closesocket(csmd_global_ctx.transport_info_t.ob_tx_connect_sock);
            csmd_global_ctx.transport_info_t.ob_tx_connect_sock = (SOCKET)0;
            WSACleanup();
            return AR_EFAILED;
         }
         return AR_EOK;
         break; // CSMD_CMD_DATA_READ
      }
      case CSMD_CMD_DATA_WRITE:
      {
         AR_MSG(DBG_LOW_PRIO, "Opcode WRITE, size of data = %lu to local address 0x%lx", size, ptr);
         char_t *data_ptr = (char_t *)payload_ptr + sizeof(csmd_data_read_write_t);
         memcpy(ptr, data_ptr, size); // write to local memory
         // send ack
         uint32_t ack          = CSMD_RSP_IBASIC_RESULT;
         void *   ack_code_ptr = malloc(sizeof(ack));
         if (NULL == ack_code_ptr)
         {
            return AR_ENORESOURCE;
         }
         memcpy(ack_code_ptr, &ack, sizeof(ack));
         uint32_t rc;
         rc = send(csmd_global_ctx.transport_info_t.ob_tx_connect_sock, (char_t *)ack_code_ptr, sizeof(ack), 0);
         free(ack_code_ptr);
         if (rc == SOCKET_ERROR)
         {
            AR_MSG(DBG_ERROR_PRIO, "send failed with error: %d\n", WSAGetLastError());
            closesocket(csmd_global_ctx.transport_info_t.ob_tx_connect_sock);
            csmd_global_ctx.transport_info_t.ob_tx_connect_sock = (SOCKET)0;
            WSACleanup();
            return AR_EFAILED;
         }
         return AR_EOK;
         break; // CSMD_CMD_DATA_WRITE
      }
      default:
      {
         AR_MSG(DBG_ERROR_PRIO, "Received unsupported opcode %lx", header->opcode);
         return AR_EBADPARAM;
      }
   }
}
#endif // WINSOCK_TRANSPORT
