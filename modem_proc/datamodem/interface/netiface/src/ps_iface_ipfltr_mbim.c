/*===========================================================================

              P S _ I F A C E _ I P F L T R _ M B I M . C

DESCRIPTION
  This file provides functions to use MBIM IP filters for inbound iface specific
  processing of IP packets received on that iface.

EXTERNALIZED FUNCTIONS

Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_iface_ipfltr_mbim.c#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
===========================================================================*/


/*===========================================================================

                       INCLUDE FILES FOR THE MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ps_mem.h"
#include "ps_iface.h"
#include "ps_crit_sect.h"
#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"
#include "modem_mem.h"
#include "ps_iface_ipfltr.h"
#include "ps_iface_ipfltr_mbim.h"
#include "ps_iface_ipfltr_mbimi.h"

/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================
  FUNCTION ps_iface_ipfltr_mbimi_pattern_match()

  DESCRIPTION
    Match data buffer against one filter patter/mask pair

  PARAMETERS
    buf_size    : size of data buffer to be compared
    buf         : pointer to data buffer to be compared
    filter_size : size of filter pattern/mask
    pattern     : pointer to filter pattern array
    mask        : pointer to filter mask array

  RETURN VALUE
    TRUE if matches, FALSE otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
INLINE boolean ps_iface_ipfltr_mbimi_pattern_match
(
  uint16   buf_size,
  uint8  * buf,
  uint8    filter_size,
  uint8  * pattern,
  uint8  * mask
)
{
  uint8    index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  if (buf_size < filter_size)
  {
    return FALSE;
  }

   /*-------------------------------------------------------------------------
    Filters are applied from 1st byte of the packet. So if there is only 1 
    byte of pattern in filter then only 1st byte of packet is matched  with 
    that filter. If there is 4 byte of pattern in filter then only first four
    bytes of packet is matched against the filter
  -------------------------------------------------------------------------*/    
  for (index = 0; index < filter_size; index++)
  {
    if ((buf[index] & mask[index]) != pattern[index])
    {
      LOG_MSG_INFO3_3("packet_filters_matched offset %d (%x - %x)",
                      buf-buf, pattern[0], pattern[filter_size-1]);      
      return FALSE;                      
    }
  }
 
  return TRUE;
} /*ps_iface_ipfltr_mbimi_pattern_match()*/

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
ps_iface_ipfltr_result_type ps_iface_ipfltr_mbim_execute
(
  ps_iface_type                     * iface_ptr,
  ps_iface_ipfltr_client_id_enum_type client_id,
  dsm_item_type                     * dsm_item_ptr
)
{
  uint8                         * data_buf_ptr;
  uint16                          pkt_len = 0;
  ip_filter_mbim_type           * mbim_filter_ptr;
  q_type                        * q_ptr;
  ps_iface_ipfilteri_type       * filter_buf_ptr;
  ps_iface_ipfltr_result_type     result = PS_IFACE_IPFLTR_NOMATCH;
  boolean                         is_pattern_match;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(iface_ptr) || NULL == dsm_item_ptr)
  {
    return PS_IFACE_IPFLTR_NOMATCH;
  }

  pkt_len = MIN(dsm_length_packet(dsm_item_ptr),
                PS_IFACE_IPFLTR_MBIM_PACKET_FILTER_MAX_SIZE);
  
  /*-------------------------------------------------------------------------
    Pkt filter is for low throughput traffic only, so copy dsm
     data to contingent buffer for easy processing.
     
     Allocated 192 bytes and do dsm_extrac with 192
  -------------------------------------------------------------------------*/     
  PS_SYSTEM_HEAP_MEM_ALLOC(data_buf_ptr, pkt_len, uint8*);
  if (!data_buf_ptr)
  {
    return PS_IFACE_IPFLTR_NOMATCH;
  }
  
  /*-------------------------------------------------------------------------
    pkt_len would be less than or equal to 
    PS_IFACE_IPFLTR_MBIM_PACKET_FILTER_MAX_SIZE
  -------------------------------------------------------------------------*/   
  pkt_len = dsm_extract(dsm_item_ptr, 0, data_buf_ptr, pkt_len);
                      
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (iface_ptr == NULL)
  {
    q_ptr = &(global_ipfltr_info[client_id]);
  }
  else
  {
    q_ptr = &(iface_ptr->iface_private.ipfltr_info[client_id]);
  }

  /*-------------------------------------------------------------------------
    Iterate through all filters on a queue and apply pattern matching. As
    per specs, Filters are applied from 1st byte of the packet. So if there 
    is only 1 byte of pattern in filter then only 1st byte of packet is 
    matched  with that filter. If there is 4 byte of pattern in filter then
    only first four bytes of packet is matched against the filter
  -------------------------------------------------------------------------*/     
  for (filter_buf_ptr = q_check(q_ptr);
       filter_buf_ptr != NULL;
       filter_buf_ptr = q_next(q_ptr, &(filter_buf_ptr->link)))
  {
    /*-----------------------------------------------------------------------
      Skip the filters which are disabled.
    -----------------------------------------------------------------------*/
    if (!filter_buf_ptr->disabled)
    {
      mbim_filter_ptr = filter_buf_ptr->filter.fi_mbim;   
      LOG_MSG_INFO3_1("ps_iface_ipfltr_mbim_execute() packet_filters_match " 
                      "handle %d", filter_buf_ptr->fi_handle);
      
      is_pattern_match = ps_iface_ipfltr_mbimi_pattern_match
                         (
                           pkt_len, 
                           data_buf_ptr, 
                           mbim_filter_ptr->len, 
                           mbim_filter_ptr->pattern, 
                           mbim_filter_ptr->mask
                         );
                          
      if (is_pattern_match)
      {
        LOG_MSG_INFO1_1("ps_iface_ipfltr_mbim_execute(): "
                        "Packet filters matched handle %d",
                        filter_buf_ptr->fi_handle);
        result = filter_buf_ptr->fi_result;
        break;
      }
    }
  }
  
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  
  PS_SYSTEM_HEAP_MEM_FREE(data_buf_ptr);
  
  return result;
} /* ps_iface_ipfltr_mbim_execute */

/*===========================================================================
  FUNCTION ps_iface_ipfltr_mbimi_alloc()

  DESCRIPTION
    Allocate memory for mbim filter

  PARAMETERS
    None

  RETURN VALUE
    Allocated mbim filter

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
ip_filter_mbim_type * ps_iface_ipfltr_mbimi_alloc
(
  uint8      alloc_len
)
{
  ip_filter_mbim_type        * mbim_filter_ptr = NULL;
/*-------------------------------------------------------------------------*/
  do
  {
    /*-------------------------------------------------------------------------
      Allocate the memory for filter node, pattern and mask
    -------------------------------------------------------------------------*/   
    PS_SYSTEM_HEAP_MEM_ALLOC(mbim_filter_ptr, 
                             sizeof(ip_filter_mbim_type), 
                             ip_filter_mbim_type *);

    if (NULL == mbim_filter_ptr)
    {
      DS_UTILS_ASSERT(0); 
      break;    
    }

    memset(mbim_filter_ptr, 0, sizeof(ip_filter_mbim_type));

    PS_SYSTEM_HEAP_MEM_ALLOC(mbim_filter_ptr->pattern, alloc_len, uint8 *);
    if (!mbim_filter_ptr->pattern)
    {                  
      break;
    }

    PS_SYSTEM_HEAP_MEM_ALLOC(mbim_filter_ptr->mask, alloc_len, uint8 *);
    if (!mbim_filter_ptr->mask)
    {                
      break;
    }
    
    return mbim_filter_ptr;
  } while(0);
  
  LOG_MSG_ERROR_INT_0("ps_iface_ipfltr_mbimi_alloc() : Failed to "                   
                  "allocate memory");  
  
  if (NULL != mbim_filter_ptr)
  {
    ps_iface_ipfltr_mbimi_free(mbim_filter_ptr);
  }

  return NULL;
} /* ps_iface_ipfltr_mbimi_alloc() */

/*===========================================================================
  FUNCTION ps_iface_ipfltr_mbimi_free()

  DESCRIPTION
    free packet filter rule

  PARAMETERS
    mbim_filter_ptr : pointer to the filter node

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void ps_iface_ipfltr_mbimi_free
(
  ip_filter_mbim_type        * mbim_filter_ptr
)
{
/*-------------------------------------------------------------------------*/
  if (NULL == mbim_filter_ptr)
  {
    LOG_MSG_ERROR_1("ps_iface_ipfltr_mbimi_free() : Invalid"                   
                    "arg mbim_filter_ptr 0x%p ", mbim_filter_ptr);
    DS_UTILS_ASSERT(0);   
    return;      
  }

  if (mbim_filter_ptr->pattern)
  {
    PS_SYSTEM_HEAP_MEM_FREE(mbim_filter_ptr->pattern);
  }

  if (mbim_filter_ptr->mask)
  {
    PS_SYSTEM_HEAP_MEM_FREE(mbim_filter_ptr->mask);
  }
  
  PS_SYSTEM_HEAP_MEM_FREE(mbim_filter_ptr);
} /* ps_iface_ipfltr_mbimi_free() */

/*===========================================================================
  FUNCTION PS_IFACE_IFPLTR_MBIM_GET_FILTER()

  DESCRIPTION
    Get filter rule associated with mbim filter handle

  PARAMETERS
    iface_ptr: iface ptr
    handle   : rule handle
    p_buf    : pointer to bytes array in sequence of len/pattern/len/mask

  RETURN VALUE
    NULL : No filter found
    ip_filter_mbim_type : mbim filter found

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
ip_filter_mbim_type * ps_iface_ipfltr_mbim_get_filter
(
  ps_iface_type       * iface_ptr,
  uint32                handle
)
{
  ip_filter_mbim_type          * mbim_filter_ptr = NULL;
  ps_iface_ipfilteri_type      * filter_buf_ptr;
  q_type                       * q_ptr;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_3("ps_iface_ipfltr_mbim_get_filter(): "
                  "iface 0x%x:%d handle %d",
                  PS_IFACE_IS_VALID(iface_ptr)?iface_ptr->name:0, 
                  PS_IFACE_IS_VALID(iface_ptr)?iface_ptr->instance:0,
                  handle);

  if (!PS_IFACE_IS_VALID(iface_ptr))
  {    
    return NULL;
  }   

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);  

  /*-------------------------------------------------------------------------
    Retrieve the filter that matches the handle by iterating through all
    mbim filters on iface
  -------------------------------------------------------------------------*/ 
  q_ptr = &(iface_ptr->iface_private.ipfltr_info[IP_FLTR_CLIENT_MBIM]);
  for (filter_buf_ptr = q_check(q_ptr);
       filter_buf_ptr != NULL;
       filter_buf_ptr = q_next(q_ptr, &(filter_buf_ptr->link)))
  { 
    if(handle == filter_buf_ptr->fi_handle)
    {
      mbim_filter_ptr = filter_buf_ptr->filter.fi_mbim;
      break;
    }
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section); 
  
  LOG_MSG_INFO2_2("ps_iface_ipfltr_mbim_get_filter() : "
                  "handle %d len %d", handle, 
                  (NULL != mbim_filter_ptr) ? mbim_filter_ptr->len : 0);
                  
  return mbim_filter_ptr;
} /*ps_iface_ipfltr_mbim_get_filter()*/