/*===========================================================================

                         P S _ I C M P _ E R R O R . C

DESCRIPTION
  Internet Control Message Protocol (ICMP) (RFC 792)
  Internet Control Message Protocol Version 6 (ICMPv6) (RFC 2463)

EXTERNALIZED FUNCTIONS
  icmpv4_error_send()
    Create and send an ICMPv4 error message.

  icmpv4_error()
    Generate an ICMPv4 error. Wrapper around icmpv4_error_send().

  icmpv6_error_send()
    Create and send an ICMPv6 error message.

  icmpv6_error()
    Generate an ICMPv6 error. Wrapper around icmpv4_error_send().

  icmp_send_unreach_error()
    Function to send ICMP(v4/v6) destination unreachable error from the
    transport layers. This function calls v4 or v6 error send depending on
    the IP info.

  icmp_send_quench_error()
    Function to send ICMPv4 source quench error from the transport layers.


Copyright (c) 2005-2015 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_icmp_error.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
13/12/17    my     CR 2157129 : FR 42790: F3 reduction
10/22/15   kc      Added logging for dropped packet 
03/16/15   kc      Sending ICMP error for valid ICMP Time Exceed Message  
06/10/14   cx      Add support for ICMP6_PKT_TOO_BIG.
12/12/12   ds      Replacing LOG_MSG_FATAL_ERROR with LOG_MSG_ERROR. 
01/11/10    ss     Lint warning fixes.
06/24/09   am      Renamed hton/ntoh macros with ps_ prefix.
05/17/06   msr     Not sending ICMP error messages if iface is not UP
05/12/06   rt      Changed one of the debug messages from HIGH to LOW.
04/19/06   rt      Removed the field ip_sec_reqd form IP control block.
10/24/05   vp      Restricting ICMP error generation if iface state=CONFIGURING
08/08/05   kr      Added code for ICMPV4 _STATS and ICMPV6_STATS
06/09/05   mct     Featurized some v6 includes.
03/02/05   vp      Created the module.
===========================================================================*/

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "datamodem_variation.h"
#include "target.h"
#include "comdef.h"
#include "customer.h"
#include "ps_in.h"
#include "ps_iface.h"
#include "ps_icmp_error.h"
#include "ps_ip4.h"
#include "err.h"
#include "msg.h"
#include "token_bucket.h"
#include "ps_stat_icmpv4.h"
#include "ps_icmp.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_logging.h"

#ifdef FEATURE_DATA_PS_IPV6
#include "ps_icmp6.h"
#include "ps_ip6.h"
#include "ps_stat_icmpv6.h"
#include "ps_iface_addr_v6.h" 
#endif /* FEATURE_DATA_PS_IPV6 */

/*===========================================================================

                      INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION     ICMPV4_GENERATE_DUMMY_ADDRESS()

DESCRIPTION  This function generates dummy address based on client's address 
             for cases where source and dest address of the packet is same
             because of NAT..

DEPENDENCIES None.

PARAMETERS   ip   - Client's IP address.

RETURN VALUE Dummy IP address

SIDE EFFECTS  
    None.
===========================================================================*/
static uint32 icmpv4_generate_dummy_address
(
  uint32 ip
)
{
  uint32 bits = 1;
  uint32 mask = ~bits;
  uint32 host;
  uint32 network;

  LOG_MSG_INFO1_1("icmpv4_generate_dummy_address(): ip = %u", ip);
  /*------------------------------------------------------------------------ 
    If LSB is 1, then skip all 1's till 0 to generate netmask.
    For Example:
    IP      = 10.0.0.3        ( 0000 1010 0000 0000 0000 0000 0000 0011 )
    netmask = 255.255.255.248 ( 1111 1111 1111 1111 1111 1111 1111 1000 )
   
    If LSB is 0, then skip all 0's till 1 to genrate netmask.
    For Example:
    IP      = 10.0.0.8        ( 0000 1010 0000 0000 0000 0000 0000 1000 )
    netmask = 255.255.255.240 ( 1111 1111 1111 1111 1111 1111 1111 0000 )
  ------------------------------------------------------------------------*/
  if (bits & ip) 
  {
    while( (bits & ip) )
    {
      bits <<= 1;
      mask <<= 1;
    }
  }
  else
  {
    while( !(bits & ip) )
    {
      bits <<= 1;
      mask <<= 1;
    }
  }

  /*------------------------------------------------------------------------ 
    Generate Server IP by adding 1 to IP address.
    If by adding 1 the host address becomes broadcast address
    then subtract 1 to generate server address.
   
    For Example (Subtract):
    IP      =  10.0.0.10       (0000 1010 0000 0000 0000 0000 0000 1010)
    netmask =  255.255.255.252 (1111 1111 1111 1111 1111 1111 1111 1100)
    Server  =  10.0.0.9        (0000 1010 0000 0000 0000 0000 0000 1001)
   
    For Example (Addition):
    IP      =  10.0.0.11       (0000 1010 0000 0000 0000 0000 0000 1011)
    netmask =  255.255.255.248 (1111 1111 1111 1111 1111 1111 1111 1000)
    Server  =  10.0.0.12       (0000 1010 0000 0000 0000 0000 0000 1100)
  ------------------------------------------------------------------------*/
  if ( (~mask & (ip + 1)) == ~mask )
  {
    host = ~mask & (ip - 1);
  }
  else
  {
    host = ~mask & (ip + 1);
  }

  network = mask & ip;

  /*------------------------------------------------------------------------ 
    Combine Network & Host address to form server address.
  ------------------------------------------------------------------------*/
  ip = network | host;

  LOG_MSG_INFO1_1("icmpv4_generate_dummy_address(): dummy ip = %u", ip);

  return ip;
}

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION     ICMPV4_ERROR_SEND()

DESCRIPTION  This function is used to send ICMPv4 error messages.

DEPENDENCIES Passed dsm item must contain the complete offending IP packet.

PARAMETERS   dsm_item_type**   - Dsm item containing the offending packet.
             struct icmp*      - Structure describing the icmp error to send.
             ip_pkt_info_type* - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS  
    DSM item containing offending packet will be freed in case of error;
    The offending packet will be sent as payload of icmp error packet in case 
    of success.
===========================================================================*/
void icmpv4_error_send
(
  dsm_item_type   **data,
  struct icmp      *icmp,
  ip_pkt_info_type *pkt_info
)
{
  ip_cb_type ip_cb;
  int16      ps_errno;
  pseudo_header_type ph;
  ps_ip_addr_type ip_addr;
  ps_iface_state_enum_type iface_state;
  ps_iface_type *iface_ptr = NULL;
  uint32 dummy_address;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ICMPV4_INC_STATS(msgs_tx,1);
  /*-------------------------------------------------------------------------
    Sanity checks
  -------------------------------------------------------------------------*/
  if( data == NULL || *data == NULL)
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return;
  }

  if(NULL == icmp || NULL == pkt_info)
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    dsm_free_packet(data);
    return;
  }
  iface_ptr = pkt_info->if_ptr;
  iface_state = ps_iface_state(iface_ptr);
  if(IFACE_UP != iface_state && IFACE_ROUTEABLE != iface_state)
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO1_1("ICMP error dropped, iface in state, 0x%x",
                    ps_iface_state(iface_ptr));
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  if(PS_IN_IS_ADDR_LIMITED_BROADCAST(pkt_info->ip_hdr.v4.dest.ps_s_addr))
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO1_2("Packet received on a broadcast address, cannot send ICMPv4"
                    " error(type %d, code %d)",
                    icmp->info.type, icmp->info.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  if(pkt_info->ip_hdr.v4.dest.ps_s_addr == 0xffffffffU)
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO1_2("Packet received on all nodes broadcast address, cannot send "
                    "ICMPv4 error(type %d, code %d)",
                    icmp->info.type, icmp->info.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  if(PS_IN_IS_ADDR_MULTICAST(pkt_info->ip_hdr.v4.dest.ps_s_addr))
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO3_2("Packet received on a multicast address, cannot send ICMPv4"
                    " error(type %d, code %d)",
                    icmp->info.type, icmp->info.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }
  /*-------------------------------------------------------------------------
    Dont send ICMP error on receiving datagram with source address as
    * Zero address
    * Loopback address
    * Class D address (Multicast)
    * Class E address (Reserved)
  -------------------------------------------------------------------------*/
  if(pkt_info->ip_hdr.v4.source.ps_s_addr == PS_INADDR_ANY ||
     pkt_info->ip_hdr.v4.source.ps_s_addr == ps_htonl(0x7f000001) ||
     PS_IN_IS_ADDR_MULTICAST(pkt_info->ip_hdr.v4.source.ps_s_addr) ||
     PS_IN_IS_ADDR_LIMITED_BROADCAST(pkt_info->ip_hdr.v4.source.ps_s_addr))
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO1_2("Cannot send ICMP error(type %d, code %d), invalid source"
                    " address of rx packet", icmp->info.type, icmp->info.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  LOG_MSG_DATA_PATH_INFO_1("Error code %d passed" 
                  "message in icmpv4_error_send()", icmp->info.code);

  switch(icmp->info.type)
  {
    case ICMP_DEST_UNREACH:
      if(icmp->info.code != (uint8)ICMP_PROTO_UNREACH &&
         icmp->info.code != (uint8)ICMP_PORT_UNREACH &&
         icmp->info.code != (uint8)ICMP_FRAG_NEEDED)
      {
        ICMPV4_INC_STATS(msgs_dropped_tx,1);
        LOG_MSG_ERROR_0("Error in icmp_dest_unreach, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP_TIME_EXCEED:
      if(icmp->info.code != (uint8)ICMP_FRAG_EXCEED &&
         icmp->info.code != (uint8)ICMP_TTL_EXCEED)
      {
        ICMPV4_INC_STATS(msgs_dropped_tx,1);
        LOG_MSG_ERROR_0("Error in icmp_time_exceed, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP_PARAM_PROB:
      if(icmp->info.code >= (uint8)ICMP_PARAM_MAX)
      {
        ICMPV4_INC_STATS(msgs_dropped_tx,1);
        LOG_MSG_ERROR_0("Error in icmp_param_prob, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP_QUENCH:
      if(icmp->info.code != 0)
      {
        ICMPV4_INC_STATS(msgs_dropped_tx,1);
        LOG_MSG_ERROR_0("Error in icmp_quench, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP_REDIRECT:
    default:
      ICMPV4_INC_STATS(msgs_dropped_tx,1);
      LOG_MSG_ERROR_1("Unsupported error type %d passed in ICMP structure to"
                      "icmpv4_error_send()", icmp->info.type);
      LOG_MSG_ERROR_0("icmp_redirect or unsupported type, dropping packet");
      DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
      dsm_free_packet(data);
      return;
  }/* End switch */

  /*------------------------------------------------------------------------
    Rate limit the error message before doing further processing.
  ------------------------------------------------------------------------*/
  if(icmp_rate_limit_on_type( (ps_icmp_msg_enum_type)icmp->info.type) == 0)
  {
    ICMPV4_INC_STATS(rate_limit_tx, 1);
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  /*------------------------------------------------------------------------
    Safe to send the message now.
  ------------------------------------------------------------------------*/
  if ( icmp_hdr_create(icmp, data, &ph) == FALSE)
  {
    ICMPV4_INC_STATS(msgs_dropped_tx,1);
    LOG_MSG_INFO1_0("Error during icmpv4_hdr_create(), discarding packet");
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  DSM_TOUCH_ITEM(*data);
  memset(&ip_cb, 0, sizeof(ip_cb));
  ip_cb.routing_cache = (ps_iface_type*)pkt_info->if_ptr;

  /*------------------------------------------------------------------------
    HACK: Source address match with um iface indicates NAT packet from
    RMnet/WLAN client. Change the source addres of ICMP error to dummy
    address to allow proper routing.
    TODO: Need refactor later.
  ------------------------------------------------------------------------*/
  ip_addr.type = IPV4_ADDR;
  ps_iface_get_addr(ip_cb.routing_cache, &ip_addr);
  if (pkt_info->ip_hdr.v4.source.ps_s_addr == ip_addr.addr.v4.ps_s_addr)
  {
    ip_cb.routing_cache = (ps_iface_type*)pkt_info->if_ptr;

    dummy_address = ps_ntohl(pkt_info->ip_hdr.v4.source.ps_s_addr);
    dummy_address = icmpv4_generate_dummy_address( dummy_address );
    dummy_address = ps_htonl(dummy_address);

    ip_cb.src_ip_addr.ps_s6_addr32[2] = ps_htonl(0xffff);
    ip_cb.src_ip_addr.ps_s6_addr32[3] = dummy_address;
     LOG_MSG_INFO1_1("Src and dest address same as modem, "
                     "using dummy (%u) as source address",
                     dummy_address);  
  }

  (void) ip4_send( &ip_cb,
                   pkt_info->ip_hdr.v4.source,
                   (uint8)PS_IPPROTO_ICMP,
                   *data,
                   ph.length,
                   NULL,
                   &ps_errno);

  return;

}/* icmpv4_error_send() */

/*===========================================================================

FUNCTION     ICMPV4_ERROR()

DESCRIPTION  This function is used to create ICMPv4 error messages from
             passed type and code values. It is a wrapper around function
             icmpv4_error_send().

DEPENDENCIES Passed dsm item must contain the complete offending IP packet.

PARAMETERS   dsm_item_type**   - Dsm item containing the offending packet.
             uint8             - Type of ICMPv4 error.
             uint8             - Code of ICMPv4 error.
             ip_pkt_info_type* - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS None.
===========================================================================*/
void icmpv4_error
(
  dsm_item_type   **data,
  uint8             type,
  uint8             code,
  ip_pkt_info_type *pkt_info
)
{
  struct icmp icmp;
/*- - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - */
  icmp.info.type = type;
  icmp.info.code = code;

  icmpv4_error_send(data, &icmp, pkt_info);
  return;
}/* icmpv4_error() */

/*===========================================================================

FUNCTION     ICMPV4_FRAG_NEEDED_ERROR()

DESCRIPTION  This function is used to create ICMPv4 Host Unreachable error 
             messages with Fragmentation Needed error code.
             It internally calls icmpv4_error_send() function to send out
             the acutal ICMP packet.

DEPENDENCIES Passed dsm item must contain the complete offending IP packet.

PARAMETERS   dsm_item_type**   - Dsm item containing the offending packet.
             uint16            - The max MTU that the sender can use on 
                                 this link.
             ip_pkt_info_type* - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS None.
===========================================================================*/
void icmpv4_frag_needed_error
(
  dsm_item_type   **data,
  uint16            mtu,
  ip_pkt_info_type *pkt_info
)
{
  struct icmp icmp;
/*- - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - */
  icmp.info.type = (uint8)ICMP_DEST_UNREACH;
  icmp.info.code = (uint8)ICMP_FRAG_NEEDED;
        
  icmp.arg.mtu.mtu = mtu;

  icmpv4_error_send(data, &icmp, pkt_info);
  return;
}/* icmpv4_frag_needed_error() */

#ifdef FEATURE_DATA_PS_IPV6
/*===========================================================================

FUNCTION     ICMPV6_ERROR_SEND()

DESCRIPTION  This function is used to send ICMPv6 error messages.DSM item 
             containing offending packet will be freed in case of error;

DEPENDENCIES Passed dsm item must contain the complete offending IPv6 packet.

PARAMETERS   dsm_item_type**    - Dsm item containing the offending packet.
             ps_icmp6_msg_type* - Structure describing the icmp error to send.
             ip_pkt_info_type*  - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS Frees the DSM item containing offending packet in case of error.
===========================================================================*/
void icmpv6_error_send
(
  dsm_item_type    **data,
  ps_icmp6_msg_type *icmp6,
  ip_pkt_info_type  *pkt_info
)
{
  ip_cb_type ip_cb;
  int16      ps_errno;
  ps_iface_state_enum_type iface_state;
  ps_ip_addr_type          modem_ip_addr;
  ps_iface_type *iface_ptr = NULL;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
    Sanity checks
  -------------------------------------------------------------------------*/
  if( data == NULL || *data == NULL)
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return;
  }

  if( NULL == icmp6 || NULL == pkt_info)
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    dsm_free_packet(data);
    return;
  }
  iface_ptr = pkt_info->if_ptr;
  iface_state = ps_iface_state(iface_ptr);
  if(IFACE_UP != iface_state && IFACE_ROUTEABLE != iface_state)
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_INFO1_1("ICMPv6 error dropped, iface in state, 0x%x",
                    ps_iface_state(pkt_info->if_ptr));
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }
  ICMPV6_INC_STATS(msgs_tx, 1);

  /*-------------------------------------------------------------------------
    RFC 2463 (ICMPv6) section 2.4
         ...
         (e.2) a packet destined to an IPv6 multicast address (there are
               two exceptions to this rule: (1) the Packet Too Big
               Message - Section 3.2 - to allow Path MTU discovery to
               work for IPv6 multicast, and (2) the Parameter Problem
               Message, Code 2 - Section 3.4 - reporting an unrecognized
               IPv6 option that has the Option Type highest-order two
               bits set to 10), or

         (e.3) a packet sent as a link-layer multicast, (the exception
               from e.2 applies to this case too)
         ...
   *** NOTE *** We are not router yet so we do not send packet too big msg
                Exception (1) of (e.2) above need to be taken care of when
                MS acts as a router.
  -------------------------------------------------------------------------*/

  if(!(icmp6->cmn_hdr.type == (uint8)ICMP6_PAR_PROBLEM &&
       icmp6->cmn_hdr.code == (uint8)ICMP6_UNRECOGNIZED_IPV6_OPT) &&
     (PS_IN6_IS_ADDR_MULTICAST(
        pkt_info->ip_hdr.v6.hdr_body.base_hdr.dst_addr.ps_s6_addr) ||
      PS_IN6_IS_ADDR_MC_LINKLOCAL(
        pkt_info->ip_hdr.v6.hdr_body.base_hdr.dst_addr.ps_s6_addr)))
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_INFO1_2("Packet received on a multicast address, cannot send ICMPv6"
                    " error (type %d, code %d)",
                    icmp6->cmn_hdr.type, icmp6->cmn_hdr.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }
  /*-------------------------------------------------------------------------
    Dont send ICMPv6 error on receiving datagram with source address as
    * IPv6 unspecified address
    * Loopback address
    * Multicast address
    * Anycast address ??? (Cannot check this until we support anycast)
  -------------------------------------------------------------------------*/
  if(PS_IN6_IS_ADDR_UNSPECIFIED(
        pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr.ps_s6_addr) ||
     PS_IN6_IS_ADDR_LOOPBACK(
        pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr.ps_s6_addr) ||
     PS_IN6_IS_ADDR_MULTICAST(
        pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr.ps_s6_addr))
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_INFO1_2("Cannot send ICMPv6 error (type %d, code %d), invalid source"
                    " address of rx pkt",
                    icmp6->cmn_hdr.type, icmp6->cmn_hdr.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  LOG_MSG_ERROR_1("Error code %d passed "
                  "message in icmpv6_error_send()", icmp6->cmn_hdr.code);

  /* Fetch Modem IP Address, Would be used for:
       - Fetching correct MTU for packet too big error
       - Computing checksum for this icmp error packet */
  ps_iface_get_ipv6_addr(
    (ps_iface_type*)pkt_info->if_ptr,                /* Routing Cache */
    &pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr, /* Destination for this
                                                        ICMP error packet */ 
    &modem_ip_addr                                   /* Modem IP to fetch */
  );

  /* Validate Modem Address */
  if(modem_ip_addr.type == IP_ADDR_INVALID)
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_ERROR_0("Failed to fetch Modem Address, can't send ICMP error");
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  LOG_MSG_INFO1_0("Fetched Modem Address:");
  IPV6_ADDR_MSG(modem_ip_addr.addr.v6.in6_u.u6_addr64);

  /* Now send appropriate error */  
  switch(icmp6->cmn_hdr.type)
  {
    case ICMP6_DEST_UNRCH:
      if(icmp6->cmn_hdr.code != (uint8)ICMP6_PORT_UNRCH)
      {
        LOG_MSG_ERROR_0("Error in icmp6_dest_unrch, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        ICMPV6_INC_STATS(msgs_dropped_tx, 1);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP6_TIME_EXCEEDED:
      if(icmp6->cmn_hdr.code != (uint8)ICMP6_FRAG_REASS_EXCEED)
      {
        ICMPV6_INC_STATS(msgs_dropped_tx, 1);
        LOG_MSG_ERROR_0("Error in icmp6_time_exceeded, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP6_PAR_PROBLEM:
      if(icmp6->cmn_hdr.code >= (uint8)ICMP6_MAX_PAR_PROBLEM)
      {
        ICMPV6_INC_STATS(msgs_dropped_tx, 1);
        LOG_MSG_ERROR_0("Error in icmp6_par_problem, dropping packet");
        DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
        dsm_free_packet(data);
        return;
      }
      break;

    case ICMP6_PKT_TOO_BIG:
      /* In case the (too big) packet was destined to Modem, 
         calculate MTU */
      if( memcmp( &pkt_info->ip_hdr.v6.hdr_body.base_hdr.dst_addr,
                  &modem_ip_addr.addr.v6,
                  sizeof(struct ps_in6_addr)) == 0 )
      {
        icmp6->msg_type.mtu = 
          ps_ip_calc_path_mtu( IPV6_ADDR, pkt_info->if_ptr );
      }
      /* Else, simply fetch MTU stored in iface */
      else
      {        
        icmp6->msg_type.mtu = PS_IFACE_GET_MTU( pkt_info->if_ptr );
      }
      break;
      
    default:
      ICMPV6_INC_STATS(msgs_dropped_tx, 1);
      LOG_MSG_ERROR_1("Unsupported error type %d passed in icmp6 structure to"
                      "icmpv6_error_send()", icmp6->cmn_hdr.type);
      DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
      dsm_free_packet(data);
      return;
  }/* End switch */

  /*-------------------------------------------------------------------------
    Rate limit the message before sending it.
  -------------------------------------------------------------------------*/
  if(icmp6_rate_limit_on_type((ps_icmp6_msg_enum_type) icmp6->cmn_hdr.type) == 0)
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_INFO1_2("ICMPv6 error message with type %d and code %d is rate limited,"
                    "cannot send", icmp6->cmn_hdr.type, icmp6->cmn_hdr.code);
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  LOG_MSG_INFO1_1("MTU: %d, Packet Info Source/Dest:", icmp6->msg_type.mtu );
  IPV6_ADDR_MSG(pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr.in6_u.u6_addr64);
  IPV6_ADDR_MSG(pkt_info->ip_hdr.v6.hdr_body.base_hdr.dst_addr.in6_u.u6_addr64);

  /*-------------------------------------------------------------------------
    Create the pseudo header and create the icmp6 packet.
    The data portion of the ICMP6 error message includes the IPv6 header
    hence add IP6LEN to the pseudo header length.
  -------------------------------------------------------------------------*/
  pkt_info->pseudo_hdr.source = modem_ip_addr.addr.v6;
  pkt_info->pseudo_hdr.dest = pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr;
  pkt_info->pseudo_hdr.length   =
               pkt_info->ip_hdr.v6.hdr_body.base_hdr.payload_len + IP6LEN;
  pkt_info->pseudo_hdr.protocol = (uint8)PS_IPPROTO_ICMP6;

  if ( ps_icmp6_msg_create( data, icmp6, &pkt_info->pseudo_hdr) == FALSE)
  {
    ICMPV6_INC_STATS(msgs_dropped_tx, 1);
    LOG_MSG_INFO1_0("ps_icmp6_msg_create() conversion err, discard");
    DPL_LOG_NETWORK_TX_DROPPED_PACKET(iface_ptr, *data, DPL_IID_NETPROT_IP);
    dsm_free_packet(data);
    return;
  }

  /*---------------------------------------------------------------------
    Transmit the ICMP echo response packet.
    pseudo_hdr length is updated in icmp6_msg_create to include the icmp6
    header length.
  ---------------------------------------------------------------------*/
  DSM_TOUCH_ITEM(*data);
  memset(&ip_cb, 0, sizeof(ip_cb));
  ip_cb.routing_cache = (ps_iface_type*)pkt_info->if_ptr;

  (void)
  ip6_send( &ip_cb,
            pkt_info->ip_hdr.v6.hdr_body.base_hdr.src_addr,
            (uint8)PS_IPPROTO_ICMP6,
            *data,
            pkt_info->pseudo_hdr.length,
            NULL,
            &ps_errno
          );

  return;
}/* icmpv6_error_send() */

/*===========================================================================

FUNCTION     ICMPV6_ERROR()

DESCRIPTION  This function is used to create ICMPv6 error messages from
             passed type and code values. It is a wrapper around function
             icmpv6_error_send().

DEPENDENCIES Passed dsm item must contain the complete offending IPv6 packet.

PARAMETERS   dsm_item_type**   - Dsm item containing the offending packet.
             uint8             - Type of ICMPv6 error.
             uint8             - Code of ICMPv6 error.
             ip_pkt_info_type* - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS None.
===========================================================================*/
void icmpv6_error
(
  dsm_item_type   **data,
  uint8             type,
  uint8             code,
  ip_pkt_info_type *pkt_info
)
{
  ps_icmp6_msg_type icmp6;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  icmp6.cmn_hdr.type = type;
  icmp6.cmn_hdr.code = code;

  icmpv6_error_send(data, &icmp6, pkt_info);
  return;
}/* icmpv6_error() */
#endif /* FEATURE_DATA_PS_IPV6 */

/*===========================================================================

FUNCTION     ICMP_SEND_UNREACH_ERROR()

DESCRIPTION  Function to send ICMP(v4/v6) destination unreachable error from
             the transport layers. This function calls v4 or v6 error send
             depending on the IP info.

DEPENDENCIES Passed dsm item must contain the complete offending IP(v6) pkt.

PARAMETERS   dsm_item_type**   - Dsm item containing the offending packet.
             ip_pkt_info_type* - IP info for the outgoing message.


RETURN VALUE None

SIDE EFFECTS Frees the DSM item in case ICMPv6 is not supported.
===========================================================================*/
void icmp_send_unreach_error
(
  dsm_item_type   **data,
  ip_pkt_info_type *pkt_info
)
{
  if(pkt_info->ip_vsn == IP_V4)
  {
    icmpv4_error ( data,
                   (uint8)ICMP_DEST_UNREACH,
                   (uint8)ICMP_PORT_UNREACH,
                   pkt_info
                 );
  }
  else
  {
#ifdef FEATURE_DATA_PS_IPV6
    icmpv6_error( data,
                  (uint8)ICMP6_DEST_UNRCH,
                  (uint8)ICMP6_PORT_UNRCH,
                  pkt_info
                );
#else
    LOG_MSG_ERROR_0("ICMPv6 not supported");
    dsm_free_packet(data);
#endif
  }

}/* icmp_send_unreach_error() */

