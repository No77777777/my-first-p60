/*===========================================================================

                      P S _ I F A C E _ I P F L T R _ A 2 . C

DESCRIPTION
  This file provides functions to use IP filters on A2 hardware.

EXTERNALIZED FUNCTIONS

Copyright (c) 2014-2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_iface_ipfltr_a2.c#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/27/15    vrk    Added support for filter info logging
===========================================================================*/

/*===========================================================================

                       INCLUDE FILES FOR THE MODULE

===========================================================================*/
#include "ps_mem.h"
#include "a2_ipfilter.h"
#include "ps_iface_ipfltr.h"
#include "ps_ip6_addr.h"
#include "ps_pkt_info.h"
#include "ps_crit_sect.h"
#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"

#include "ps_ifacei.h"
#include "ps_iface_addr_v6.h"

#include "ps_iface_ipfltr_hw.h"
#include "sio.h"
#include "ps_dpm.h"

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/



/*===========================================================================

                                    MACROS

===========================================================================*/
#define A2_OFFSET_IPV4_SRC_ADDR       12
#define A2_OFFSET_IPV4_DST_ADDR       16
#define A2_OFFSET_IPV4_NEXT_HDR_PROT  9
#define A2_OFFSET_IPV6_SRC_ADDR       8
#define A2_OFFSET_IPV6_DST_ADDR       24
#define A2_OFFSET_TCP_SRC_PORT        0
#define A2_OFFSET_TCP_DST_PORT        2
#define A2_OFFSET_UDP_SRC_PORT        0
#define A2_OFFSET_UDP_DST_PORT        2
#define A2_OFFSET_ICMP_TYPE           0
#define A2_OFFSET_ICMP_CODE           1
#define A2_OFFSET_IPV6_HEADER         40
#define A2_MASK_ICMP_TYPE             0xFF000000
#define A2_MASK_ICMP_CODE             0x00FF0000
#define A2_OFFSET_ESP_SPI             0
#define A2_OFFSET_AH_SPI              4


/*===========================================================================

                      INTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================
FUNCTION ps_iface_ipfltri_a2_add_v4_param()

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE
  FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ps_iface_ipfltri_a2_add_v4_param
(
  a2_ipfilter_rule_set_handle_t   *a2_ruleset_handle,
  ip_filter_type                  *filter,
  uint8                           *next_hdr_prot
)
{
  uint32                          tmp_mask;
  a2_ipfilter_rule_result_e       a2_ret_val = A2_IPFILTER_RULE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((NULL == filter) || (NULL == next_hdr_prot))
  {  
    LOG_MSG_ERROR_INT_0("filter or next_hdr_prot is NULL");
    return FALSE;
  }

  tmp_mask = filter->ip_hdr.v4.field_mask;
  while (tmp_mask)
  {
    if (tmp_mask & IPFLTR_MASK_IP4_SRC_ADDR)
    {
      /*----------------------------------------------------------------------
        The value that A2 expects for comparison has to be in byte order.
      ----------------------------------------------------------------------*/
      a2_ret_val = a2_ipfilter_add_rule_meq_32
                   (
                     a2_ruleset_handle,
                     A2_OFFSET_IPV4_SRC_ADDR,
                     ps_ntohl(filter->ip_hdr.v4.src.subnet_mask.ps_s_addr),
                     ps_ntohl(filter->ip_hdr.v4.src.subnet_mask.ps_s_addr) &
                       ps_ntohl(filter->ip_hdr.v4.src.addr.ps_s_addr)
                   );
      tmp_mask &= ~IPFLTR_MASK_IP4_SRC_ADDR;
    }
    else if (tmp_mask & IPFLTR_MASK_IP4_DST_ADDR)
    {
      a2_ret_val = a2_ipfilter_add_rule_meq_32
                   (
                     a2_ruleset_handle,
                     A2_OFFSET_IPV4_DST_ADDR,
                     ps_ntohl(filter->ip_hdr.v4.dst.subnet_mask.ps_s_addr),
                     ps_ntohl(filter->ip_hdr.v4.dst.subnet_mask.ps_s_addr) &
                       ps_ntohl(filter->ip_hdr.v4.dst.addr.ps_s_addr)
                   );
      tmp_mask &= ~IPFLTR_MASK_IP4_DST_ADDR;
    }
    else if (tmp_mask & IPFLTR_MASK_IP4_NEXT_HDR_PROT)
    {
      a2_ret_val =
        a2_ipfilter_add_rule_protocol_eq(a2_ruleset_handle,
                                         filter->ip_hdr.v4.next_hdr_prot);
      *next_hdr_prot = filter->ip_hdr.v4.next_hdr_prot;
      tmp_mask &= ~IPFLTR_MASK_IP4_NEXT_HDR_PROT;
    }
    else if (tmp_mask & IPFLTR_MASK_IP4_TOS)
    {
      a2_ret_val = a2_ipfilter_add_rule_tos_eq(a2_ruleset_handle,
                                               filter->ip_hdr.v4.tos.val);
      tmp_mask &= ~IPFLTR_MASK_IP4_TOS;
    }

    /*check for errors*/
    if (a2_ret_val != A2_IPFILTER_RULE_SUCCESS)
    {
      LOG_MSG_ERROR_INT_1("ps_iface_ipfltri_a2_add_v4_param(): "
                      "ERROR: ps_iface_ipfltri_add_a2_filter_ipv4: %d",
                      a2_ret_val);
      return FALSE;
    }
  } /* while (tmp_mask) */

  return TRUE;
} /* ps_iface_ipfltri_add_a2_filter_ipv4() */

/*===========================================================================
FUNCTION ps_iface_ipfltri_a2_add_v6_param()

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE
  FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean ps_iface_ipfltri_a2_add_v6_param
(
  a2_ipfilter_rule_set_handle_t   *a2_ruleset_handle,
  ip_filter_type                  *filter,
  uint8                           *next_hdr_prot
)
{
  a2_ipfilter_rule_result_e       ret_val = A2_IPFILTER_RULE_SUCCESS;
  uint32                          tmp_mask = 0;
  uint8                           a2_mask[16] = {0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF,
                                                 0xFF, 0xFF, 0xFF, 0xFF};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((NULL == filter) || (NULL == next_hdr_prot))
  {  
    LOG_MSG_ERROR_INT_0("filter or next_hdr_prot is NULL");
    return FALSE;
  }

  tmp_mask = filter->ip_hdr.v6.field_mask;
  while (tmp_mask)
  {
    if (tmp_mask & IPFLTR_MASK_IP6_SRC_ADDR)
    {
      if (0 != filter->ip_hdr.v6.src.prefix_len)
      {
        PS_IFACE_IPFLTR_HW_CALCULATE_V6_ADDR_MASK(a2_mask, 
                                                  filter->ip_hdr.v6.src.prefix_len);
      }
      ret_val = a2_ipfilter_add_rule_meq_128(
                  a2_ruleset_handle,
                  A2_OFFSET_IPV6_SRC_ADDR,
                  a2_mask,
                  filter->ip_hdr.v6.src.addr.in6_u.u6_addr8);
      tmp_mask &= ~IPFLTR_MASK_IP6_SRC_ADDR;
    }
    else if (tmp_mask & IPFLTR_MASK_IP6_DST_ADDR)
    {
      if (0 != filter->ip_hdr.v6.dst.prefix_len)
      {
        PS_IFACE_IPFLTR_HW_CALCULATE_V6_ADDR_MASK(a2_mask, 
                                                  filter->ip_hdr.v6.dst.prefix_len);
      }   
      
      ret_val = a2_ipfilter_add_rule_meq_128(
                  a2_ruleset_handle,
                  A2_OFFSET_IPV6_DST_ADDR,
                  a2_mask,
                  filter->ip_hdr.v6.dst.addr.in6_u.u6_addr8);
      tmp_mask &= ~IPFLTR_MASK_IP6_DST_ADDR;
    }
    else if (tmp_mask & IPFLTR_MASK_IP6_NEXT_HDR_PROT)
    {
      *next_hdr_prot = filter->ip_hdr.v6.next_hdr_prot;
      ret_val = a2_ipfilter_add_rule_protocol_eq(a2_ruleset_handle,
                                                 *next_hdr_prot);
      tmp_mask &= ~IPFLTR_MASK_IP6_NEXT_HDR_PROT;
    }
    else if (tmp_mask & IPFLTR_MASK_IP6_TRAFFIC_CLASS)
    {
      ret_val = a2_ipfilter_add_rule_tc_eq(a2_ruleset_handle,
                                       filter->ip_hdr.v6.trf_cls.val);
      tmp_mask &= ~IPFLTR_MASK_IP6_TRAFFIC_CLASS;
    }
    else if (tmp_mask & IPFLTR_MASK_IP6_FLOW_LABEL)
    {
      ret_val = a2_ipfilter_add_rule_flow_eq(a2_ruleset_handle,
                                             filter->ip_hdr.v6.flow_label);
      tmp_mask &= ~IPFLTR_MASK_IP6_FLOW_LABEL;
    }

    if (ret_val != A2_IPFILTER_RULE_SUCCESS)
    {
      LOG_MSG_ERROR_2("ps_iface_ipfltri_a2_add_v6_param(): "
                      "ERROR: A2_IPFILTER_ADD_RULE: %d, 0x%x",
                      ret_val, tmp_mask);
      DS_UTILS_ASSERT(0);
      return FALSE;
    }
  }//end while (tmp_mask)v6

  return TRUE;

}/*ps_iface_ipfltri_a2_add_v6_param*/

/*===========================================================================
FUNCTION ps_iface_ipfltri_a2_add_next_hdr_prot_param()

DESCRIPTION

DEPENDENCIES
  *assumes the filter and iface have already been verified

RETURN VALUE
  TRUE  for success
  FALSE  otherwise

SIDE EFFECTS
  None.

===========================================================================*/
static boolean ps_iface_ipfltri_a2_add_next_hdr_prot_param
(
  a2_ipfilter_rule_set_handle_t    *a2_ruleset_handle,
  ip_filter_type                   *filter,
  uint8                            next_hdr_prot
  )
{
  a2_ipfilter_rule_result_e        ret_val = A2_IPFILTER_RULE_SUCCESS;
  uint32                           tmp_mask;
  uint32                           ipv6_value;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == filter)
  {  
    LOG_MSG_ERROR_INT_0("filter is NULL");
    return FALSE;
  }

  switch (next_hdr_prot)
  {
    case PS_IPPROTO_TCP:
      tmp_mask = filter->next_prot_hdr.tcp.field_mask;
      if (tmp_mask & IPFLTR_MASK_TCP_SRC_PORT)
      {
        /*-------------------------------------------------------------------
          The -1 for the SRC port low is needed due to A2 implementation.
          Also the value that A2 expects for comparison has to be in byte
          order.
        -------------------------------------------------------------------*/
        ret_val = a2_ipfilter_add_rule_ihl_offset_range_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_TCP_SRC_PORT,
                    ps_ntohs(filter->next_prot_hdr.tcp.src.port) - 1,
                    ps_ntohs(filter->next_prot_hdr.tcp.src.port) +
                      filter->next_prot_hdr.tcp.src.range
                  );
      }

      if (tmp_mask & IPFLTR_MASK_TCP_DST_PORT)
      {
        ret_val = a2_ipfilter_add_rule_ihl_offset_range_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_TCP_DST_PORT,
                    ps_ntohs(filter->next_prot_hdr.tcp.dst.port) - 1,
                    ps_ntohs(filter->next_prot_hdr.tcp.dst.port) +
                      filter->next_prot_hdr.tcp.dst.range
                  );
      }

      break;

    case PS_IPPROTO_UDP:
      tmp_mask = filter->next_prot_hdr.udp.field_mask;
      if (tmp_mask & IPFLTR_MASK_UDP_SRC_PORT)
      {
        ret_val = a2_ipfilter_add_rule_ihl_offset_range_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_UDP_SRC_PORT,
                    ps_ntohs(filter->next_prot_hdr.udp.src.port) - 1,
                    ps_ntohs(filter->next_prot_hdr.udp.src.port) +
                      filter->next_prot_hdr.udp.src.range
                  );
      }

      if (tmp_mask & IPFLTR_MASK_UDP_DST_PORT)
      {
        ret_val = a2_ipfilter_add_rule_ihl_offset_range_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_UDP_DST_PORT,
                    ps_ntohs(filter->next_prot_hdr.udp.dst.port) - 1,
                    ps_ntohs(filter->next_prot_hdr.udp.dst.port) +
                      filter->next_prot_hdr.udp.dst.range
                  );
      }

      break;

    case PS_IPPROTO_ICMP:
      tmp_mask = filter->next_prot_hdr.icmp.field_mask;
      if (tmp_mask & IPFLTR_MASK_ICMP_MSG_TYPE)
      {
        ret_val = a2_ipfilter_add_rule_ihl_offset_eq_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_ICMP_TYPE,
                    filter->next_prot_hdr.icmp.type
                  );
      }

      if (tmp_mask & IPFLTR_MASK_ICMP_MSG_CODE)
      {
        ret_val = a2_ipfilter_add_rule_ihl_offset_eq_16
                  (
                    a2_ruleset_handle,
                    A2_OFFSET_ICMP_CODE,
                    filter->next_prot_hdr.icmp.code
                  );
      }

      break;

    case PS_IPPROTO_ICMP6:
      tmp_mask = filter->next_prot_hdr.icmp.field_mask;
      if (tmp_mask & IPFLTR_MASK_ICMP_MSG_TYPE)
      {
        ipv6_value = (uint32)filter->next_prot_hdr.icmp.type;
        ipv6_value = ipv6_value << 24;
        ret_val    = a2_ipfilter_add_rule_meq_32(a2_ruleset_handle,
                                                 A2_OFFSET_IPV6_HEADER,
                                                 A2_MASK_ICMP_TYPE,
                                                 ipv6_value);
      }

      if (tmp_mask & IPFLTR_MASK_ICMP_MSG_CODE)
      {
        ipv6_value = (uint32) filter->next_prot_hdr.icmp.code;
        ipv6_value = ipv6_value << 16;
        ret_val    = a2_ipfilter_add_rule_meq_32(a2_ruleset_handle,
                                                 A2_OFFSET_IPV6_HEADER,
                                                 A2_MASK_ICMP_CODE,
                                                 ipv6_value);
      }

      break;

    case PS_IPPROTO_ESP :
    {
      tmp_mask = filter->next_prot_hdr.esp.field_mask;      
      if (tmp_mask & IPFLTR_MASK_ESP_SPI)
      {
        ret_val =
          a2_ipfilter_add_rule_ihl_offset_eq_32
          (
            a2_ruleset_handle,
            A2_OFFSET_ESP_SPI,
            ps_ntohl(filter->next_prot_hdr.esp.spi)
          );
      }      
      break;
    }    

    case PS_IPPROTO_AH : 
    {
      tmp_mask = filter->next_prot_hdr.ah.field_mask;      
      if (tmp_mask & IPFLTR_MASK_AH_SPI)
      {
        ret_val =
          a2_ipfilter_add_rule_ihl_offset_eq_32
          (
            a2_ruleset_handle,
            A2_OFFSET_AH_SPI,
            ps_ntohl(filter->next_prot_hdr.ah.spi)
          );
      }      
      break;
    }
      
    case PS_IPPROTO_FRAG_HDR:
    {
      break;
    }
      
    case PS_IPPROTO_TCP_UDP:
    {
      tmp_mask = filter->next_prot_hdr.tcp_udp_port_range.field_mask;

     /*------------------------------------------------------------------------
       The -1 for the SRC/DST port low is needed due to A2 implementation.
       Also the value that A2 expects for comparison has to be in byte order.
     ------------------------------------------------------------------------*/
      if (tmp_mask & IPFLTR_MASK_TCP_UDP_SRC_PORT)
      {
        ret_val = 
            a2_ipfilter_add_rule_ihl_offset_range_16
            (
              a2_ruleset_handle,
              A2_OFFSET_TCP_SRC_PORT,
              ps_ntohs(filter->next_prot_hdr.tcp_udp_port_range.src.port) - 1,
              ps_ntohs(filter->next_prot_hdr.tcp_udp_port_range.src.port) +
              filter->next_prot_hdr.tcp_udp_port_range.src.range
            );
      }
    
      if ((tmp_mask & IPFLTR_MASK_TCP_UDP_DST_PORT) &&
          ret_val == A2_IPFILTER_RULE_SUCCESS)
      {
        ret_val = 
            a2_ipfilter_add_rule_ihl_offset_range_16
            (
              a2_ruleset_handle,
              A2_OFFSET_TCP_DST_PORT,
              ps_ntohs(filter->next_prot_hdr.tcp_udp_port_range.dst.port) - 1,
              ps_ntohs(filter->next_prot_hdr.tcp_udp_port_range.dst.port) +
              filter->next_prot_hdr.tcp_udp_port_range.dst.range
            );
      }
      break;
    }

    default:
      LOG_MSG_ERROR_INT_0("ps_iface_ipfltri_a2_add_next_hdr_prot_param(): "
                      "A2 ipfilter NEXT_HDR_PROT unsupported protocol");
      return FALSE;
  }

  if (ret_val != A2_IPFILTER_RULE_SUCCESS)
  {
    LOG_MSG_ERROR_2("ps_iface_ipfltri_a2_add_next_hdr_prot_param(): "
                    "ERROR: A2_IPFILTER_ADD_RULE: %d, 0x%x",
                    ret_val, tmp_mask);
    return FALSE;
  }

  return TRUE;
} /* ps_iface_ipfltri_a2_add_next_hdr_prot_param() */

/*===========================================================================
FUNCTION PS_IFACE_IPFLTRI_ADD_A2_FILTER()

DESCRIPTION
  This function adds the input filter to A2

DEPENDENCIES
  *assumes the filter and iface have already been verified

RETURN VALUE
  TRUE  for success
  FALSE  otherwise

SIDE EFFECTS
  None.

===========================================================================*/
static ps_iface_ipfltr_hw_rule_handle_type ps_iface_ipfltri_a2_add
(
  ip_filter_type                        * fltr_ptr,
  ps_iface_ipfltr_client_id_enum_type     client_id
)
{
  uint8                           next_hdr_prot = PS_NO_NEXT_HDR;
  boolean                         ret_val = FALSE;
  ps_iface_ipfltr_hw_rule_handle_type  a2_ruleset_handle = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == fltr_ptr)
  {  
    LOG_MSG_ERROR_INT_0("fltr_ptr is NULL");
    return NULL;
  }

  LOG_MSG_INFO2_1("ps_iface_ipfltri_a2_add(): "
                  "adding a2 fltrs for  client %d", client_id);
                  
  do
  {
    if (IP_V4 == fltr_ptr->ip_vsn)
    {    
      a2_ruleset_handle = a2_ipfilter_add_ruleset(A2_IPV4, SIO_PORT_NULL);  
    } 

    if (IP_V6 == fltr_ptr->ip_vsn)
    {    
      a2_ruleset_handle = a2_ipfilter_add_ruleset(A2_IPV6, SIO_PORT_NULL); 
    }    

    if (NULL == a2_ruleset_handle)
    {
      DS_UTILS_ERROR_FATAL("ps_iface_ipfltri_a2_add(): Couldn't alloc A2 rule handle");
      break;
    }

    if (IP_V4 == fltr_ptr->ip_vsn)
    {
      ret_val = ps_iface_ipfltri_a2_add_v4_param(a2_ruleset_handle,
                                                fltr_ptr,
                                                &next_hdr_prot);
      if (ret_val != TRUE)
      {
        LOG_MSG_ERROR_INT_0("ps_iface_ipfltri_a2_add(): "
                        "Failed to add A2 IPv4 Filter ");
        break;
      }
    }
    else
    {
      ret_val = ps_iface_ipfltri_a2_add_v6_param(a2_ruleset_handle,
                                                fltr_ptr,
                                                &next_hdr_prot);
      if (ret_val != TRUE)
      {
        LOG_MSG_ERROR_INT_0("ps_iface_ipfltri_a2_add(): "
                        "Failed to add A2 IPv4 Filter ");
        break;
      }
    }

    if (PS_NO_NEXT_HDR != next_hdr_prot)
    {
      ret_val = ps_iface_ipfltri_a2_add_next_hdr_prot_param(a2_ruleset_handle,
                                                           fltr_ptr,
                                                           next_hdr_prot);
      if (ret_val != TRUE)
      {
        break;
      }
    }

    a2_ipfilter_commit_rules();
   

    LOG_MSG_INFO2_2("ps_iface_ipfltri_a2_add(): Added rule handle 0x%p "
                    "IP vsn %d ",
                    a2_ruleset_handle,
                    fltr_ptr->ip_vsn);

    return a2_ruleset_handle;
  } while(0);

  
  /*-------------------------------------------------------------------------
    Because of error we reached here, so do the cleanup
  -------------------------------------------------------------------------*/  
  if (NULL != a2_ruleset_handle)
  {
    ps_iface_ipfltr_hw_delete(a2_ruleset_handle, client_id, fltr_ptr->ip_vsn);
  }

  return NULL;
} /* ps_iface_ipfltri_a2_add() */

/*===========================================================================
FUNCTION PS_IFACE_IPFLTRI_A2_ADD_V4_FRAG_FLTR()

DESCRIPTION
  This function adds the input filter to A2

DEPENDENCIES
  *assumes the filter and iface have already been verified

RETURN VALUE
  TRUE  for success
  FALSE  otherwise

SIDE EFFECTS
  None.

===========================================================================*/
void ps_iface_ipfltri_a2_add_v4_frag_fltr
(
  void
)
{
  a2_ipfilter_rule_set_handle_t  a2_frag_ruleset_hdl = NULL;
  a2_ipfilter_rule_result_e      ret_val = A2_IPFILTER_RULE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  a2_frag_ruleset_hdl =
    a2_ipfilter_add_ruleset(A2_IPV4, SIO_PORT_NULL);

  if (a2_frag_ruleset_hdl == NULL)
  {
    DS_UTILS_ERROR_FATAL("ps_iface_ipfltri_a2_add_v4_frag_fltr(): Couldn't alloc rule");
    return;
  }
  
  ret_val = a2_ipfilter_add_rule_protocol_eq(a2_frag_ruleset_hdl,
                                             PS_IPPROTO_FRAG_HDR);
  if (A2_IPFILTER_RULE_SUCCESS != ret_val)
  {
    LOG_MSG_ERROR_INT_1("ps_iface_ipfltri_a2_add_v4_frag_fltr(): "
                    "Couldn't add protocol rule, err %d ", ret_val);
    DS_UTILS_ASSERT(0);
    a2_ipfilter_remove_ruleset(a2_frag_ruleset_hdl, A2_IPV4);
    return;
  }
  
  a2_ipfilter_commit_rules();

  return;
} /* ps_iface_ipfltri_a2_add_v4_frag_fltr() */

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
int32 ps_iface_ipfltr_hw_add
(
  ps_iface_type                         * iface_ptr,
  ps_iface_ipfltr_client_id_enum_type     client_id,
  ps_iface_ipfilteri_type               * filter_buf_ptr
)
{
  int32                      ret_val = -1;
  ip_filter_type            *fltr_ptr = NULL;
  struct ps_in6_addr         v6_addr;
  uint8                    * next_hdr_prot = NULL;
    ps_iface_type          * base_iface_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == filter_buf_ptr)
  {
    LOG_MSG_ERROR_INT_0("filter_buf_ptr is NULL");   
    return -1;                      
  }

  do
  {
    if (NULL != filter_buf_ptr->hw_rule_handle)
    {
      LOG_MSG_INFO1_1("ps_iface_ipfltr_hw_add() : "
                      "hw_rule_handle %d not null", filter_buf_ptr->hw_rule_handle);   
      break;                      
    }
    
    if (NULL != filter_buf_ptr->hw_rule_handle_2)
    {
      LOG_MSG_INFO1_1("ps_iface_ipfltr_hw_add(): hw_rule_handle_2 %d not null."
                      "This should not happen since hw_rule_handle is NULL",
                      filter_buf_ptr->hw_rule_handle_2);
      DS_UTILS_ASSERT(0);
      break;
    }

    fltr_ptr = &(filter_buf_ptr->filter.fi_default);

    /*-------------------------------------------------------------------------
      Don't add the filters in HW if the filter & the iface are not of the 
      same addr type (i.e v4 ifaces should add only v4 filters)
    -------------------------------------------------------------------------*/
    if (PS_IFACE_IS_VALID(iface_ptr) && 
        (((IP_V4 == fltr_ptr->ip_vsn) && 
         (IPV4_ADDR != ps_iface_get_addr_family(iface_ptr))) ||
        ((IP_V6 == fltr_ptr->ip_vsn) && 
         (IPV6_ADDR != ps_iface_get_addr_family(iface_ptr))))
       )
    {
      LOG_MSG_INFO2_0("ps_iface_ipfltr_hw_add(): Not adding fltr to IPA since "
                      "filter & iface addr type are different");
      ret_val = 0;
      break;      
    }

    if (client_id == IP_FLTR_CLIENT_SOCKETS)
    {
      /*---------------------------------------------------------------------
      Skip application filters Installation with IPA if Software path is enabled
      ---------------------------------------------------------------------*/
      if (PS_IFACE_IS_VALID(iface_ptr) && (FALSE == ps_dpm_is_bridging_allowed(iface_ptr)))
      {
        ret_val = 0;
        LOG_MSG_INFO2_0("ps_iface_ipfltr_hw_add(): Not adding fltr to A2 since "
                      "Data path is software");
        break;
      }
      
      base_iface_ptr = PS_IFACE_GET_BASE_IFACE(iface_ptr);
      /*-------------------------------------------------------------------------
        Install filters in Hardware for hardware capable ifaces (3gpp, rm ifaces)
      -------------------------------------------------------------------------*/
      if(!PS_IFACE_GET_CAPABILITY(base_iface_ptr,PS_IFACE_CAPABILITY_HW_CAPABLE))
      {
         LOG_MSG_INFO2_0("ps_iface_ipfltr_hw_add(): Not adding fltr to A2 since "
                      "Iface is logical iface");
        ret_val = 0;
        break;
      }
      
      /*---------------------------------------------------------------------
        Install application registered v6 filters with A2 only if destination
        addr is an external address. There is no need to install other filters
        as IPv6 module installs destination addr based filters for
        default IID and private IIDs
      ---------------------------------------------------------------------*/
      if ((IP_V6 == fltr_ptr->ip_vsn) && (iface_ptr != NULL) &&
          (0 != (IPFLTR_MASK_IP6_DST_ADDR & fltr_ptr->ip_hdr.v6.field_mask)))
      {
        if (FALSE == ps_iface_is_v6_address_external
                                           (iface_ptr,
                                            &(fltr_ptr->ip_hdr.v6.dst.addr)))
        {
          LOG_MSG_INFO2_0("ps_iface_ipfltr_hw_add(): Not adding fltr to A2 "
                          "as v6 destination addrees is not external");
          ret_val = 0;
          break;
        }
        
        v6_addr.ps_s6_addr64[0] = fltr_ptr->ip_hdr.v6.dst.addr.ps_s6_addr64[0];
        v6_addr.ps_s6_addr64[1] = fltr_ptr->ip_hdr.v6.dst.addr.ps_s6_addr64[1];

        IPV6_ADDR_MSG(v6_addr.ps_s6_addr64);
        /*-------------------------------------------------------------------
          Install Fragmentation based filter for given v6 address only if
          Filter hasnt been install before
        -------------------------------------------------------------------*/        
        ps_iface_ipv6_add_frag_filter(iface_ptr, &v6_addr);                                                                    
      }     
    }
    else if(client_id == IP_FLTR_CLIENT_QOS_OUTPUT)
    {
      ret_val = 0;
      break;      
    }    
    else
    {
      LOG_MSG_INFO1_1("ps_iface_ipfltr_hw_add() : "
                     "Filters are not installed for client_id %d", client_id);
      break;
    }

    if (IP_V4 == fltr_ptr->ip_vsn)
    {
      next_hdr_prot = &fltr_ptr->ip_hdr.v4.next_hdr_prot;
    }

    else 
    {
      next_hdr_prot = &fltr_ptr->ip_hdr.v6.next_hdr_prot;
    }

    /*-----------------------------------------------------------------------
      if next_hdr_protocol is TCP_UDP then install filters on both TCP & UDP
      since HW does not have support for TCP_UDP type
    -----------------------------------------------------------------------*/      
    if (PS_IPPROTO_TCP_UDP == *next_hdr_prot)
    {
      *next_hdr_prot = PS_IPPROTO_TCP;
      filter_buf_ptr->hw_rule_handle = ps_iface_ipfltri_a2_add(fltr_ptr, 
                                                               client_id);

      *next_hdr_prot = PS_IPPROTO_UDP;
      filter_buf_ptr->hw_rule_handle_2 = ps_iface_ipfltri_a2_add(fltr_ptr, 
                                                               client_id);

      *next_hdr_prot = PS_IPPROTO_TCP_UDP;
    }

    else
    {
      filter_buf_ptr->hw_rule_handle = ps_iface_ipfltri_a2_add(fltr_ptr, 
                                                               client_id);
    }

    if (NULL == filter_buf_ptr->hw_rule_handle || 
        (PS_IPPROTO_TCP_UDP == *next_hdr_prot && 
         NULL == filter_buf_ptr->hw_rule_handle_2))
    {
      LOG_MSG_ERROR_INT_0("ps_iface_ipfltr_hw_add(): "
                      "Couldn't install filter with A2");                      
      break;
    }

    ret_val = 0;
  } while (0);

  return ret_val;
} /* ps_iface_ipfltr_hw_add() */

void ps_iface_ipfltr_hw_delete
(
  ps_iface_ipfltr_hw_rule_handle_type    hw_rule_handle,
  ps_iface_ipfltr_client_id_enum_type    client_id,
  ip_version_enum_type                   ip_vsn  
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (client_id == IP_FLTR_CLIENT_SOCKETS)
  {
    if (IP_V4 == ip_vsn)
    {
      a2_ipfilter_remove_ruleset(hw_rule_handle, A2_IPV4);
    }
    else
    {
      a2_ipfilter_remove_ruleset(hw_rule_handle, A2_IPV6);
    }
    
    a2_ipfilter_commit_rules();
  }
}  /* ps_iface_ipfltr_hw_delete */

ps_iface_ipfltr_hw_rule_handle_type  ps_iface_ipfltr_hw_add_v6_frag_filter
(
  struct ps_in6_addr     * ipv6_addr_ptr,
  ps_iface_type          * iface_ptr
)
{
  a2_ipfilter_rule_set_handle_t  *a2_hndl = NULL;
  ip_filter_type                  v6_frag_fltr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((NULL == iface_ptr) || (NULL == ipv6_addr_ptr))
  {  
    LOG_MSG_ERROR_INT_0("iface_ptr or ipv6_addr_ptr is NULL");
    return NULL;
  }

  LOG_MSG_INFO1_4("ps_iface_ipfltri_a2_add_v6_frag_fltr(): "
                  "Installing IPV6 frag  fltr to Hardware"
                  " iface 0x%x:%d external prefix 0x%lx iid 0x%lx",
                   iface_ptr->name, 
                   iface_ptr->instance,
                   ipv6_addr_ptr->ps_s6_addr64[0],
                   ipv6_addr_ptr->ps_s6_addr64[1]);

  memset(&v6_frag_fltr,0, sizeof(ip_filter_type));
                   
  v6_frag_fltr.ip_hdr.v6.field_mask = 
    IPFLTR_MASK_IP6_NEXT_HDR_PROT|IPFLTR_MASK_IP6_DST_ADDR;
    
  v6_frag_fltr.ip_hdr.v6.next_hdr_prot = PS_IPPROTO_FRAG_HDR;  
  
  v6_frag_fltr.ip_hdr.v6.dst.addr.in6_u.u6_addr64[0] = 
    ipv6_addr_ptr->ps_s6_addr64[0];
  v6_frag_fltr.ip_hdr.v6.dst.addr.in6_u.u6_addr64[1] = 
    ipv6_addr_ptr->ps_s6_addr64[1];
    
  v6_frag_fltr.ip_vsn = IP_V6;      

  /*-------------------------------------------------------------------
    Install Fragmentation header based filter so that for hybrid
    architecture, fragments are routed to Modem side for further
    processing.
  -------------------------------------------------------------------*/  
  a2_hndl = ps_iface_ipfltri_a2_add(&v6_frag_fltr,
                                    IP_FLTR_CLIENT_SOCKETS);    


  LOG_MSG_INFO1_1("ps_iface_ipfltri_a2_add_v6_frag_fltr(): "
                  "A2 filter with ruleset 0x%p added",
                  a2_hndl);  

  return a2_hndl;   
} /* ps_iface_ipfltri_a2_add_v6_frag_fltr() */


void ps_iface_ipfltr_hw_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_iface_ipfltri_a2_add_v4_frag_fltr();
} /* ps_iface_ipfltr_hw_init */

#ifdef FEATURE_DATA_PS_464XLAT
ps_iface_ipfltr_hw_rule_handle_type ps_iface_ipfltr_hw_add_clat_v6_dst_addr_fltr
(
  ps_iface_type          * iface_ptr,
  struct ps_in6_addr     * v6_addr_ptr,
  uint8                    prefix_len
)
{
  ip_filter_type                   filter;
  a2_ipfilter_rule_set_handle_t  * a2_hndl = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((NULL == iface_ptr) || (NULL == v6_addr_ptr))
  {  
    LOG_MSG_ERROR_INT_0("iface_ptr or v6_addr_ptr is NULL");
    return NULL;
  }

  LOG_MSG_INFO1_2("ps_iface_ipfltr_hw_add_clat_v6_dst_addr_fltr(): "
                  "Installing IPV6 address based fltr to A2"
                  " iface 0x%x:%d",
                   iface_ptr->name, 
                   iface_ptr->instance);
                   
  IPV6_ADDR_MSG(v6_addr_ptr->ps_s6_addr64);                   

  memset(&filter, 0, sizeof(ip_filter_type));
  filter.ip_hdr.v6.dst.addr.in6_u.u6_addr64[0] =  v6_addr_ptr->ps_s6_addr64[0];
  filter.ip_hdr.v6.dst.addr.in6_u.u6_addr64[1] =  v6_addr_ptr->ps_s6_addr64[1];
  filter.ip_hdr.v6.dst.prefix_len = prefix_len;
  filter.ip_hdr.v6.field_mask = IPFLTR_MASK_IP6_DST_ADDR;
  filter.ip_vsn = IP_V6;
  
  a2_hndl = ps_iface_ipfltri_a2_add(&filter,
                                    IP_FLTR_CLIENT_SOCKETS);                                  
  
  LOG_MSG_INFO1_2("ps_iface_ipfltri_a2_add_v6_dst_filter(): "
                  "A2 filter with ruleset 0x%x added for IP vsn %d",
                  a2_hndl,
                  filter.ip_vsn);  

  return a2_hndl;

} /* ps_iface_ipfltr_hw_add_clat_v6_dst_addr_fltr */
#endif /* FEATURE_DATA_PS_464XLAT */

ps_iface_ipfltr_hw_rule_handle_type ps_iface_ipfltr_hw_add_v6_dst_addr_fltr
(
  ps_iface_type          * iface_ptr,
  struct ps_in6_addr     * v6_addr_ptr
)
{
  ip_filter_type                   filter;
  a2_ipfilter_rule_set_handle_t  * a2_hndl = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((NULL == iface_ptr) || (NULL == v6_addr_ptr))
  {  
    LOG_MSG_ERROR_INT_0("iface_ptr or v6_addr_ptr is NULL");
    return NULL;
  }

  LOG_MSG_INFO1_2("ps_iface_ipfltr_hw_add_v6_dst_addr_fltr(): "
                  "Installing IPV6 address based fltr to A2"
                  " iface 0x%x:%d",
                   iface_ptr->name, 
                   iface_ptr->instance);
                   
  IPV6_ADDR_MSG(v6_addr_ptr->ps_s6_addr64);                   

  memset(&filter, 0, sizeof(ip_filter_type));
  filter.ip_hdr.v6.dst.addr.in6_u.u6_addr64[0] =  v6_addr_ptr->ps_s6_addr64[0];
  filter.ip_hdr.v6.dst.addr.in6_u.u6_addr64[1] =  v6_addr_ptr->ps_s6_addr64[1];
  filter.ip_hdr.v6.field_mask = IPFLTR_MASK_IP6_DST_ADDR;
  filter.ip_vsn = IP_V6;
  
  a2_hndl = ps_iface_ipfltri_a2_add(&filter,
                                    IP_FLTR_CLIENT_SOCKETS);                                  
  
  LOG_MSG_INFO1_2("ps_iface_ipfltri_a2_add_v6_dst_filter(): "
                  "A2 filter with ruleset 0x%x added for IP vsn %d",
                  a2_hndl,
                  filter.ip_vsn);  

  return a2_hndl;

} /* ps_iface_ipfltr_hw_add_v6_dst_addr_fltr */

void ps_iface_ipfltr_hw_delete_v6_dst_addr_fltr
(
  ps_iface_ipfltr_hw_rule_handle_type  v6_dst_fltr_handle_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ps_iface_ipfltr_hw_delete(v6_dst_fltr_handle_ptr, 
                            IP_FLTR_CLIENT_SOCKETS, 
                            IP_V6); 
  return;                        
} /* ps_iface_ipfltr_hw_delete_v6_dst_addr_fltr() */


void ps_iface_ipfltr_hw_add_all_iface_filters
(
  ps_iface_type                         * iface_ptr,
  ps_iface_ipfltr_client_id_enum_type     client_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
} /* ps_iface_ipfltr_hw_add_all_iface_filters */

void ps_iface_ipfltr_hw_delete_all_iface_filters
(
  ps_iface_type                         * iface_ptr,
  ps_iface_ipfltr_client_id_enum_type     client_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
} /* ps_iface_ipfltr_hw_delete_all_iface_filters */

void ps_iface_ipfltr_hw_delete_v6_prefix_comp_fltr
(
  ps_iface_ipfltr_hw_rule_handle_type  hw_rule_handle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
} /* ps_iface_ipfltr_hw_delete_v6_prefix_comp_fltr */

ps_iface_ipfltr_hw_rule_handle_type ps_iface_ipfltr_hw_add_ul_prefix_comp_fltr
(
  ps_iface_type * iface_ptr,
  uint32          stream_id,
  uint32          ipa_fltr_result
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
  return NULL;
} /* ps_iface_ipfltr_hw_add_ul_prefix_comp_fltr */

ps_iface_ipfltr_hw_rule_handle_type ps_iface_ipfltr_hw_add_ul_prefix_comp_fltr_ex
(
  uint32          stream_id,
  uint8           prefix_len,
  uint8           prefix_ipa[16],
  uint32          ipa_fltr_result
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
  return NULL;
} /* ps_iface_ipfltr_hw_add_ul_prefix_comp_fltr_ex */

/*===========================================================================
FUNCTION ps_iface_ipfltr_hw_commit_rules()

DESCRIPTION
  This function commits all hardware rule handles to hardware
  
DEPENDENCIES
  *assumes the filter and iface have already been verified

RETURN VALUE
  0 for success else failure

SIDE EFFECTS
  None.

===========================================================================*/
int32 ps_iface_ipfltr_hw_commit_rules
(
  ps_iface_ipfltr_hw_rule_handle_type   * hw_rule_handles,
  uint32                                  num_hw_rule_handles,
  ps_iface_ipfltr_client_id_enum_type     client_id
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
  return 0;    
} /* ps_iface_ipfltr_hw_commit_rules() */

void ps_iface_ipfltr_hw_log_filters()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*NO-OP*/
  /*As of now, A2 underlying funtionality not implemented,
   added functionality for IPA only*/
  return;
}/*ps_iface_ipfltr_hw_log_filters*/

#ifdef FEATURE_DATA_PS_464XLAT 
boolean ps_iface_ipfltr_hw_get_clat_cap()
{
    
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* A2 does not support HW CLAT , so always return FALSE*/
     return 0;
}/*ps_iface_ipfltr_hw_get_clat_cap*/
#endif

