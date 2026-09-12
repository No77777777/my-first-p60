/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        I P v 4    S O U R C E     F I L E

GENERAL DESCRIPTION
 Upper half of IP, consisting of send/receive primitives,
  Not needed when running as a standalone gateway.
  Copyright 1991 Phil Karn, KA9Q
  Additional material Copyright 1992 William Allen Simpson
  fragment reassembly removed.


EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 1995-2020 QUALCOMM Technology Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ps_ip.c_v   1.8   19 Feb 2003 14:33:04   ubabbar  $
  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_ip4.c#3 $ $DateTime: 2020/02/18 00:01:30 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ---------------------------------------------------------- 
02/18/20    ds     Added fix to drop packet when ESP encrypt fails
01/12/18    rj     CR 2171422 : MPSS.HE.1.0 - Fix KW issue
13/12/17    my     CR 2157129 : FR 42790: F3 reduction
10/22/15    kc     Added logging for droppped packet 
02/08/15    cx     Log the timestamp before sending the packet in UL
11/11/14    cx     Added support for checksum offload
11/09/14    yl     IP data path re-architecture
03/12/14    ss     Changes to generate IPSEC info in IP layer.
11/20/13    fn     Update pkt info after parsing secip hdr and route pkt to
                   IPSec iface if it is the next iface
11/11/13    rk     Removed feature mobileap.
04/23/13    ash    Set Qos filter result only if it is not NOMATCH.
02/25/13    op     Fix to reset the IP filter in ip4_input appropriately
02/11/13    ds     Added do_reassembly validation before doing ipsec 
                   processing.
04/06/12    pgm    Resetting filter result and filter mask for socket client
                   in ip4_input().
03/19/12    ss     Drop IP pkts with IP hdr length greater than or equal to
                   dsm item length.
12/02/11    ss     Added index validation while accessing IPSEC iface list
                   array.
05/18/11    pp     Pkt's DestIP, IFACE addr validation moved to embedded pkts
                   to support LTE Mode-B.
03/16/11    ss     ip4_input: Move the check for local packet to the last.
03/10/11    cp     ip4_send: fragment offset field is initialized to 0.
02/24/11    cp     Fixed compiler warnings.
02/22/11    cp     Generate PktInfo in IP layer, if its not generated.
01/07/11    cp     Made changes to calculate the path mtu value used for
                   IP packet fragmentaion, using ps_ip_calc_path_mtu() API.
12/23/10    ss     Cleaning up of Globals for thread safety in Q6 Free
                   Floating environment.
10/18/10    pp     Generate transport protocol information, and set
                   is_pkt_info_valid to TRUE to avoid Pkt Info re-generation.
10/01/10    sa     Added appropriate log messages before ASSERT(0).
04/18/10    ss     KW fixes.
11/14/09    ss     IP Multicast feature addition.
11/14/09    sp     Removed dependency on internal ipsec structure.
09/24/09    ss     KW warning fixes.
05/22/09    kk     Q6 compiler warning fixes.
04/14/09    sp     ip4_send: If the IP address type is invalid, the packet
                   is dropped.
03/27/09    ssh    ip4_send: Fixed possible NULL dereference
09/12/08    pp     Metainfo optimizations.
06/26/07    rs     Added code to use Multicast TTL value if the address is
                   multicast. Also added code to support router alert option.
04/25/07    vp     Using meta_info's pkt_info_ptr in ip4_send
03/29/07    rt     Removed IP_MAXTTL. Use IP_DEF_TTL instead.
02/09/07    rt     Initialized local var errno in ip4_input()
02/06/07    scb    Fixed Klockwork Critical and high errors in ip4_input()
04/20/06    mct    Optimized IPv4 loopback processing.
04/19/06    rt     Modified ip4_send to take generic ip pkt info in
                   secipsah_hdr_create().
03/28/06    msr    Fixed compilation error caused when LOG_MSG_INFO3 is compiled
09/26/05    msr    Fixed ip4_send() to not populate IPCB's metainfo if it is
                   NULL
08/15/05    mct    Fixed naming causing some issues w/C++ compilers.
08/15/05    vp     Allow packets if the dst_addr does no match the iface IP
                   addr only if iface state is CONFIGURING and iface IP addr
                   is zero.
08/12/05    kr     Removed existing stats code and added code for updating
                   the new statistics framework
08/05/05    sv     Genreate meta_info and cache it in IPCB if metainfo is NULL.
                   so that filtering results done at lower layers get cached.
05/11/05    sv     Lint changes.
04/22/05    ks     Fixed checking of IPV4 version no. in ip4_input.
04/20/05    sv     optimize ip4_input to use passed pkt information.
04/19/05    mct    Only check for loopback if iface is a v4 iface.
04/19/05    jd     Reverted ip4_route to only generate metainfo for broadcast
                   interfaces to optimize non-broadcast iface performance.
04/18/05    vp     Verify that the address of the input iface is equal to
                   dest addr of the incoming packet in ip4_input().
03/15/05    lyr    Change the byte ordering for next hop addr meta info in
                   ipv4_route to host byte order - ARP currently expects this
03/10/05    jd     Ensure ipv4_route adds metainfo for non-broadcast
                   interfaces to ensure correct operation with lan_llc
03/02/05    vp     Support for ICMP error generation.
01/26/05    sv     Renamed esp/ah header parsing functions.
01/10/05    sv     Merged IPSEC changes.
01/06/05    msr    Not freeing meta info and DSM item in ip4_route() when
                   ps_iface_tx_cmd() fails.
12/27/04    ifk    Replaced setting of tracer field with DSM_TOUCH_ITEM
12/23/04    lyr    Added meta info population in ip4_route() for bcast ifaces
12/07/04    msr    Updating IP header with user provided TOS.
10/30/04    ks     Changed ptr to ptr of ps_meta_info_type to single pointer
                   in the call to lo_tx_cmd().
10/15/04    ifk    Added call to fragmentation routine in ip4_route()
10/13/04    vp     Removed inclusion of ps_iphdr.h and included ps_ip4_hdr.h.
                   Changes due to restructuring of struct ip and replacement
                   of ntohip() and htonip() with ps_ip4_hdr_parse() and
                   ps_ip4_hdr_create().
08/30/04    usb    Fixed if check in ip4_route() to loopback to vaid iface.
07/22/04    mct    Fixed issue seen when compiling with RVDS 21 compiler.
07/01/04    ifk    Removed setting of DSM kind field.
06/11/04    vp     Removed inclusion of psglobal.h and internet.h. Included
                   files ps_pkt_info.h and ps_stas.h.Changes for representation
                   of IP addresses as struct ps_in_addr or struct ps_in6_addr.
                   Replaced use of TCP_PTCL etc with PS_IPPROTO_TCP etc.
                   Replaced use of IPVERSION with IP_V4.
04/30/04    vp     Removed FEATURE_PS_UNSUPPORTED and changes for that.
04/30/04    vp     Code for correct handling of packets destined to itself.
04/30/04    usb    Passing IP pkt up the stack without stripping the IP hdr.
04/21/04    usb    Passing ptr instead of ref ptr to metainfo thru the stack.
04/16/04    sv     Removed assert in ip4_send when we get an invalid IP address
                   from the ps_iface.
03/16/04    mct    Renamed acl_ipfilter_info_type to ip_pkt_info_type.
03/02/04    rc     Removed tx interface cache entry from ip4_send() signature.
                   Fixed ip4_send() to always use the routing cache from ip_cb.
01/09/04    rc     Initial Version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */

#include "msg.h"
#include "err.h"
#include "amssassert.h"
#include "memory.h"
#include "IxErrno.h"

#include "dsm.h"
#include "ps_ppp.h"
#include "psi.h"

#include "ps_tcp.h"
#include "ps_udp_io.h"
#include "ps_icmp.h"

#include "ps_acl.h"
#include "ps_inbound_acl.h"
#include "ps_routei.h"
#include "ps_routei_acl.h"
#include "ps_routei_lo.h"

#include "ps_ip4.h"
#include "ps_iputil.h"
#include "ps_ip4_hdr.h"
#include "ps_lo.h"
#include "ps_pkt_info.h"
#include "ps_ip4_frag.h"
#include "ps_icmp_error.h"
#include "ps_pkt_info_utils.h"
#include "ps_stat_ipv4.h"
#include "ps_ip_fraghdl.h"
#include "ps_metai_info.h"

#ifdef FEATURE_SEC_IPSEC
#include "secips.h"
#include "ipsec_iface_hdlr.h"
#endif

#include "ds_Utils_DebugMsg.h"
#include "ps_pkt_info_utils.h"
#include "ps_dpm_hw.h"
#include "ps_timestamp.h"
#include "ps_logging.h"

#define PS_IP4_ASSERT(expression)  \
          ps_ip4_assert_wrapper(__LINE__, expression)


/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*-------------------------------------------------------------------------*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/



/*===========================================================================

                    PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION ps_ip4_assert_wrapper

DESCRIPTION
  Wrapper function for PS_IP4 ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

=============================================================================*/
static void ps_ip4_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("PS_IP4_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
  
}/* ps_IP4_assert_wrapper */


/*===========================================================================

FUNCTION IP4_SEND()

DESCRIPTION
  This function is used by the upper layers (UDP and TCP) to send
  an IPv4 datagram.
  (Modeled after the example interface on p 32 of RFC 791.)

DEPENDENCIES
  None

RETURN VALUE
  0 for success and -1 for failure (ps_errno contains error code)

SIDE EFFECTS
  None
===========================================================================*/
int ip4_send
(
  ip_cb_type         *ip_cb,          /* IP control block                  */
  struct ps_in_addr  dest,            /* Destination address               */
  uint8              protocol,        /* Protocol                          */
  dsm_item_type*     send_dg_ptr,     /* Pointer to incoming datagram      */
  uint16             length,          /* Optional length of data portion   */
  ps_tx_meta_info_type* meta_info_ptr, /* Metainfo about packet             */
  int16*             ps_errno         /* Error code for failure case       */
)
{
  static uint16 Id_cntr = 0;
  ip_pkt_info_type   *ip_pkt_info;         /* IP info (hdr + policiy info ) */
  ps_ip_addr_type ip_addr;
#ifdef FEATURE_SEC_IPSEC
  void* esp_sa_ptr = NULL;
  void* ah_sa_ptr = NULL;
  ip_pkt_info_type temp_ip_pkt_info;
  int              result = 0;
#endif /* FEATURE_SEC_IPSEC */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
    Update statistics
  -------------------------------------------------------------------------*/
  IPV4_INC_STATS(pkts_tx,1) ;
  /*-------------------------------------------------------------------------
    Check for UDP or ICMP packets which contain per-packet routing info
  -------------------------------------------------------------------------*/
  if ( ip_cb == NULL )
  {
    IPV4_INC_STATS(pkts_dropped_tx,1);
    dsm_free_packet(&send_dg_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    /* DATA_ERR_FATAL(" ip4_send(): ip_cb ptr is NULL, Dropping the pkt");*/
	PS_IP4_ASSERT(0);
    return -1;
  }

  if(meta_info_ptr == NULL)
  {
    if(ip_cb->meta_info_ptr != NULL)
    {
      PS_TX_META_INFO_DUP(ip_cb->meta_info_ptr, &meta_info_ptr);
    }
    else
    {
      /*---------------------------------------------------------------------
        Don't populate IPCB's metainfo as there are couple of cases, where
        IPCB passed in to this function is not part of TCB, or UCB etc. If
        it is part of TCB, or UCB, its metainfo would have already been
        populated
      ---------------------------------------------------------------------*/
      PS_TX_META_INFO_AND_RT_META_INFO_GET(meta_info_ptr);
      if(meta_info_ptr != NULL &&
         PS_TX_META_GET_RT_META_INFO_PTR(meta_info_ptr) != NULL)
      {
        PS_TX_META_SET_ROUTING_CACHE(meta_info_ptr, ip_cb->routing_cache);
      }
    }
  }

  /* Meta info cannot be NULL here! */
  if((meta_info_ptr == NULL) ||
     (PS_TX_META_GET_RT_META_INFO_PTR(meta_info_ptr) == NULL))
  {
    IPV4_INC_STATS(pkts_dropped_tx,1);
    dsm_free_packet(&send_dg_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    /* DATA_ERR_FATAL("ip4_send(): meta info ptr is NULL, Dropping the pkt");*/
	PS_IP4_ASSERT(0);
    return -1;
  }

  ip_pkt_info = &PS_TX_META_GET_PKT_INFO(meta_info_ptr);

  /* Moved it above IPSEC generation to fetch source address */
  /*-----------------------------------------------------------------------
    Retrieve the new IP addr from psiface.
  -----------------------------------------------------------------------*/
  if(PS_IN6_IS_ADDR_UNSPECIFIED(&ip_cb->src_ip_addr))
  {
    /*-------------------------------------------------------------------------
      Get the source address if src_ip_addr is NULL. This must be done for
      ICMP messages.
    -------------------------------------------------------------------------*/
    ip_addr.type = IPV4_ADDR;
    ps_iface_get_addr(ip_cb->routing_cache, &ip_addr);
    if(ip_addr.type == IP_ADDR_INVALID)
    {
      LOG_MSG_ERROR_0("IP Address Invalid, Dropping the packet");
      IPV4_INC_STATS(pkts_dropped_tx,1);
      dsm_free_packet(&send_dg_ptr);
      PS_META_INFO_FREE(&meta_info_ptr);
      return -1;
    }
    else
    {
      ip_pkt_info->ip_hdr.v4.source = ip_addr.addr.v4;
    }
  }
  else
  {
    ip_pkt_info->ip_hdr.v4.source.ps_s_addr =
      PS_IN6_GET_V4_FROM_V4_MAPPED_V6(&ip_cb->src_ip_addr);
  }

#ifdef FEATURE_SEC_IPSEC
  /* Generate IPSEC info if not yet generated */
  if( PS_TX_META_GET_IPSEC_INFO(meta_info_ptr).esp_sa_ptr == NULL &&
      PS_TX_META_GET_IPSEC_INFO(meta_info_ptr).ah_sa_ptr == NULL )
  {
    /* Populate temp ip pkt info with IP hdr details */
    memset( &temp_ip_pkt_info, 0, sizeof( temp_ip_pkt_info ) );

    temp_ip_pkt_info.ip_vsn = IP_V4;
    temp_ip_pkt_info.ip_hdr.v4.dest.ps_s_addr = dest.ps_s_addr;
    temp_ip_pkt_info.ip_hdr.v4.source.ps_s_addr = 
      ip_pkt_info->ip_hdr.v4.source.ps_s_addr;
    
    /* Generate IPSEC info */
    ps_ip_udpate_ipsec_info_in_metainfo( meta_info_ptr, send_dg_ptr, 
                                         &temp_ip_pkt_info, protocol );
  }

  /* Get esp_sa_ptr, ah_sa_ptr from Meta info */
  esp_sa_ptr = PS_TX_META_GET_IPSEC_INFO(meta_info_ptr).esp_sa_ptr;
  ah_sa_ptr = PS_TX_META_GET_IPSEC_INFO(meta_info_ptr).ah_sa_ptr;

  /*-------------------------------------------------------------------------
    Add ESP header.
  -------------------------------------------------------------------------*/
  if(esp_sa_ptr != NULL && send_dg_ptr != NULL)
  {
    ip_pkt_info->ip_hdr.v4.protocol = PS_IPPROTO_ESP;
    ip_pkt_info->ip_vsn = IP_V4;

    result = secipsesp_hdr_create(esp_sa_ptr,
                                &protocol,
                                &send_dg_ptr);

    if( 0 != result )
    {
      LOG_MSG_ERROR_0("secipsesp_hdr_create returned -1. Dropping packet");
      IPV4_INC_STATS(pkts_dropped_tx,1);
      dsm_free_packet(&send_dg_ptr);
      PS_TX_META_INFO_FREE(&meta_info_ptr);
      return -1;
    }

    if(length != 0)
    {
      length = (uint16)dsm_length_packet(send_dg_ptr);
    }

    /* ----------------------------------------------------------------------
      Update ESP Protocol Pkt information into ip_pkt_info.
    ------------------------------------------------------------------------*/
    if (-1 == ps_transport_ptcl_pkt_info_generate ( send_dg_ptr,
                                                    ip_pkt_info,
                                                    PS_IPPROTO_ESP,
                                                    0, /* dsm_offset */
                                                    FALSE)
       )
    {
      DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
      IPV4_INC_STATS(pkts_dropped_tx,1);
      dsm_free_packet(&send_dg_ptr);
      PS_TX_META_INFO_FREE(&meta_info_ptr);
      return -1;
    }

    /*-------------------------------------------------------------------------
      QoS filter execution would happen at iface layer.
    -------------------------------------------------------------------------*/
  }  
#endif

  /*-------------------------------------------------------------------------
    Use implicit length of DSM item chain
  -------------------------------------------------------------------------*/
  if(length == 0 && send_dg_ptr != NULL)
  {
    length = (uint16)dsm_length_packet(send_dg_ptr);
  }

  if (ip_cb->ttl == 0)
  {
    ip_cb->ttl = IP_DEF_TTL;
  }

  if (ip_cb->mcast_ttl == 0)
  {
    ip_cb->mcast_ttl = IP_DEF_MCAST_TTL;
  }

  /*-------------------------------------------------------------------------
    Fill in IP header
  -------------------------------------------------------------------------*/
  ip_pkt_info->ip_vsn                  = IP_V4;
  ip_pkt_info->ip_hdr.v4.version       = IP_V4;
  ip_pkt_info->ip_hdr.v4.length        = IPLEN + length;

  ip_pkt_info->ip_hdr.v4.id            = Id_cntr++;

  ip_pkt_info->ip_hdr.v4.off           = 0;
  ip_pkt_info->ip_hdr.v4.offset        = 0;
  ip_pkt_info->ip_hdr.v4.mf            = 0;

  /*-------------------------------------------------------------------------
    Update MCAST TTL - if the address is multicast address.
  -------------------------------------------------------------------------*/
  if( PS_IN_IS_ADDR_MULTICAST(ip_pkt_info->ip_hdr.v4.dest.ps_s_addr) )
  {
    if(ip_cb->mcast_ttl_set == TRUE)
    {
      ip_pkt_info->ip_hdr.v4.ttl           = ip_cb->mcast_ttl;
    }
    else
    {
      ip_pkt_info->ip_hdr.v4.ttl = IP_DEF_MCAST_TTL;
    }
  }
  else
  {
    ip_pkt_info->ip_hdr.v4.ttl           = ip_cb->ttl;
  }

  /*-------------------------------------------------------------------------
    MCAST Loop - Is Mcast loop enabled - this will be used in ip4_route to
    do mcast loop back.
  -------------------------------------------------------------------------*/
  ip_pkt_info->is_mcast_loop            = ip_cb->mcast_loop;

  if (ip_cb->ip_cb_router_alert == TRUE)
  {
    ip_pkt_info->ip_hdr.v4.router_alert = TRUE;
  }
  else
  {
    ip_pkt_info->ip_hdr.v4.router_alert = FALSE;
  }

  ip_pkt_info->ip_hdr.v4.tos           = ip_cb->tos;
  ip_pkt_info->ip_hdr.v4.df            = ip_cb->df;

  ip_pkt_info->ip_hdr.v4.congest       = 0;
  ip_pkt_info->ip_hdr.v4.protocol      = protocol;
  ip_pkt_info->ip_hdr.v4.dest          = dest;
  ip_pkt_info->ip_hdr.v4.optlen        = 0;

  /*-------------------------------------------------------------------------
    Set filter interface to NULL indicating it's a locally
    generated packet.
  -------------------------------------------------------------------------*/
  ip_pkt_info->if_ptr = NULL;

  /* ----------------------------------------------------------------------
    Count the number of multicast packets transmitted
  ------------------------------------------------------------------------*/
  if( PS_IN_IS_ADDR_MULTICAST(ip_pkt_info->ip_hdr.v4.dest.ps_s_addr) )
  {
     IPV4_INC_STATS(mcast_tx,1) ;
  }
#ifdef FEATURE_SEC_IPSEC
  if(ah_sa_ptr != NULL)
  {
    ip_pkt_info->ip_hdr.v4.protocol = protocol = PS_IPPROTO_AH;
    ip_pkt_info->ip_hdr.v4.length += sizeof(secips_ah_hdr);
    (void ) secipsah_hdr_create(ah_sa_ptr,
                                protocol,
                                (*ip_pkt_info),
                                &send_dg_ptr);
    /* ----------------------------------------------------------------------
      Update AH Protocol Pkt information into ip_pkt_info.
    ------------------------------------------------------------------------*/
    if (-1 == ps_transport_ptcl_pkt_info_generate ( send_dg_ptr,
                                                    ip_pkt_info,
                                                    protocol,
                                                    0, /* dsm_offset */
                                                    FALSE)
       )
    {
      DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
      IPV4_INC_STATS(pkts_dropped_tx,1);
      dsm_free_packet(&send_dg_ptr);
      PS_TX_META_INFO_FREE(&meta_info_ptr);
      return -1;
    }

    /*-------------------------------------------------------------------------
      QoS filter execution would happen at iface layer.
      -------------------------------------------------------------------------*/
  }
#endif

  /* ----------------------------------------------------------------------
    Update Transport Protocol Pkt information into ip_pkt_info, to avoid
    re-generation of ip_pkt_info in IFACE I/O routines dueto filtering.
  ------------------------------------------------------------------------*/
  if (-1 == ps_transport_ptcl_pkt_info_generate ( send_dg_ptr,
                                                  ip_pkt_info,
                                                  protocol,
                                                  0, /* dsm_offset */
                                                  FALSE)
     )
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    IPV4_INC_STATS(pkts_dropped_tx,1);
    dsm_free_packet(&send_dg_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    return -1;
  }

  /* ----------------------------------------------------------------------
    Update is_pkt_info_valid flag to TRUE. This will make sure  to avoid pkt_info
    re-generation.
  ------------------------------------------------------------------------*/
  ip_pkt_info->is_pkt_info_valid = TRUE;

  /*-------------------------------------------------------------------------
    Fill in IP header in network byte order. NOTE: The passed in 'send_dg_ptr'
    field may be changed to account for IP header bytes needed.
  -------------------------------------------------------------------------*/
  if(ps_ip4_hdr_create( &send_dg_ptr, &(ip_pkt_info->ip_hdr.v4), IP_CS_NEW)
      == FALSE)
  {
    IPV4_INC_STATS(pkts_dropped_tx,1);
    dsm_free_packet(&send_dg_ptr);
    PS_TX_META_INFO_FREE(&meta_info_ptr);
    return -1;
  }
  if (send_dg_ptr == NULL)
  {
    return 0;
  }

  /*------------------------------------------------------------------------
    Removed the tcp_ppp_wm - so directly calling function instead of queueing
    item up in a watermark.
  -------------------------------------------------------------------------*/
  DSM_TOUCH_ITEM(send_dg_ptr);
  
  /*-------------------------------------------------------------------------
    Find out which interface this should go to
  -------------------------------------------------------------------------*/
  return ip4_route( ip_pkt_info,
                    meta_info_ptr,
                    &send_dg_ptr,
                    ip_cb->routing_cache,
                    ps_errno );

} /* ip4_send() */


/*===========================================================================

FUNCTION IP4_INPUT()

DESCRIPTION
  This function accepts received IP packet(-fragment)s from network
  interfaces and applies ipfilter for the DMSS inbound ACL.
  It then forwards the packet to ip_route for local delivery
  respectively forwarding.

DEPENDENCIES
  None

RETURN VALUE
  0 for success, -1 for failure

SIDE EFFECTS
  None
===========================================================================*/
int ip4_input
(
  ps_iface_type *in_if_ptr,           /* Input interface                   */
  dsm_item_type *bp,                  /* Input packet                      */
  ps_rx_meta_info_type * meta_info_ptr   /* Meta info ptr                  */
)
{
  ip_pkt_info_type       *pkt_info_ptr;      /* IP header being processed  */
  ps_iface_type*          next_if_ptr = NULL; /* further inbound proc iface*/
  uint16                  ip_len;            /* IP header length */
  uint16                 offset = 0;        /* current offset to proto hdr */
  boolean                done_processing = FALSE;
  uint32                 if_ipv4_addr = 0;
  errno_enum_type        error_num = E_SUCCESS;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( in_if_ptr == NULL )
  {
    /*-----------------------------------------------------------------------
      Incoming interface cannot be NULL
    -----------------------------------------------------------------------*/
    dsm_free_packet( &bp );
    /* DATA_ERR_FATAL("ip4_send(): Input interface ptr is NULL, Dropping the pkt"); */
	PS_IP4_ASSERT(0);
    return -1;
  }
  IPV4_INC_STATS(pkts_rx,1) ;

  LOG_MSG_DATA_PATH_INFO_1("ip4_input iface : 0x%x",in_if_ptr->name);

  /*-----------------------------------------------------------------------
      Generate packet information.
    -----------------------------------------------------------------------*/
  if( meta_info_ptr == NULL ||
      PS_RX_META_GET_PKT_INFO(meta_info_ptr).is_pkt_info_valid == FALSE )
  {
    if( ps_rx_pkt_info_generate(&bp,&meta_info_ptr, &error_num)== -1)
    {
      PS_RX_META_INFO_FREE(&meta_info_ptr);
      if(error_num == E_WOULD_BLOCK)
      {
        return 0;
      }
      DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
      dsm_free_packet(&bp);
      return -1;
    }
  }

  pkt_info_ptr = &PS_RX_META_GET_PKT_INFO(meta_info_ptr);
  if(pkt_info_ptr == NULL)
  {
     LOG_MSG_ERROR_0("ip4_input(): NULL pkt_info_ptr");
     return -1;
  }
  pkt_info_ptr->if_ptr = in_if_ptr;

  /*-----------------------------------------------------------------------
    Count the number of multicast packets received
  ------------------------------------------------------------------------*/
  if( PS_IN_IS_ADDR_MULTICAST(pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) )
  {
     IPV4_INC_STATS(mcast_rx,1) ;
  }

  done_processing = FALSE;
  while(done_processing == FALSE)
  {
    done_processing = TRUE; /* assume we're done */
    /*-------------------------------------------------------------------------
      Check if we received a fragment.
      The following table shows what should be done for handling fragments:

      ip.flags.mf   ip.offset    Comments
      0              0           This is a complete datagram with no fragments.
                                 So, no need to handle these fragments.
      1              0           We received the starting fragment of the
                                 datagram. Handle it. Store the ip header of
                                 this fragment and pass it to upper layer.
      0              !0          We received the last fragment of the datagram
                                 Handle it. Use this to calculate the total
                                 length of the incoming datagram.
      1              !0          We received some intermittent fragment of the
                                 datagram. Handle it.

      Note that following cases are not taken care by the implementation:
      1. We donot handle any overlapping of the data in different fragments.
         It is left to upper layers to thrash it off.
      2. We donot have any time outs for fragments. If we donot receive any
         fragment of a datagram, we wait until the entire reassembly array is
         full. We donot time out , but leave it to the application to take
         care of it.
    -------------------------------------------------------------------------*/
    if (FALSE != PS_IFACE_GET_REASSEMBLY_FLAG(in_if_ptr))
    {
      LOG_MSG_DATA_PATH_INFO_0("ip4_input(): Reassembling packet");

      if (pkt_info_ptr->fraghdl != NULL)
      {
        /*-----------------------------------------------------------------------
          Received IP fragments process them before giving it to other layers
        -----------------------------------------------------------------------*/
        DSM_TOUCH_ITEM(bp);
        ip_len = 0;
        IPV4_INC_STATS(reasm_reqd,1) ;

        if((bp = ip4_reassemble(pkt_info_ptr, bp, &ip_len)) == NULL)
        {
          PS_RX_META_INFO_FREE(&meta_info_ptr);
          return 0;
        }
        else
        {
          IPV4_INC_STATS(reasm_ok,1) ;
        }
        /*done_processing = FALSE;  reassembled a packet so re-do,
                                      potentially can be removed if reassembly precedes IPsec */
      }
    }

    ip_len = pkt_info_ptr->ip_hdr.v4.optlen+IPLEN;
    offset = ip_len;

#ifdef FEATURE_SEC_IPSEC

    /* check if IPsec handling is relevant to this packet */
    /*if(*/
    /*    ((pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_UDP*/
    /*    && pkt_info_ptr->esp_hdr.spi != 0)) ||*/
    /*    (pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_ESP) ||*/
    /*    (pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_AH)*/
    /*  )*/
    /*{*/
      /* get the next iface (IPsec iface), */
      /* not needed because the test in ip_receive using this was removed */
      /* ipsec_iface_find(in_if_ptr, &next_if_ptr, &bp, meta_info_ptr); */
    /*}*/

    /* test if we will handle IPsec processing */
    if(FALSE != PS_IFACE_GET_IPSEC_PROCESSING_FLAG(in_if_ptr))
    {
      /* defined here to avoid another #ifndef */
      boolean is_ipsec_processing_done = FALSE;

      /* rfc3948: UDP Encapsulation of IPsec ESP Packets */
      if(pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_UDP
         && pkt_info_ptr->esp_hdr.spi != 0)
      {
        /* use offset to ignore UDP hdr, rfc3948 section 3.2*/
        /* set offset to point to ESP header, right after UDP header */
        offset += sizeof(udp_hdr_type);
        if (secipsesp_hdr_parse(&bp,&offset,pkt_info_ptr) == -1)
        {
          LOG_MSG_INFO1_0("Error in parsing UDP ENCAPS ESP header");
          DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
          dsm_free_packet(&bp);
          PS_RX_META_INFO_FREE(&meta_info_ptr);
          return -1;
        }
        is_ipsec_processing_done = TRUE;
          /* done_processing = TRUE;  needed only when supporting tunnel mode */
      }
      else
      {
        while(pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_ESP ||
              pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_AH)
        {
          if(pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_ESP)
          {
            if( secipsesp_hdr_parse(&bp,&offset,pkt_info_ptr) == -1)
            {
              LOG_MSG_INFO1_0("Error in parsing ESP header");
              DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
              dsm_free_packet(&bp);
              PS_RX_META_INFO_FREE(&meta_info_ptr);
              return -1;
            }
            is_ipsec_processing_done = TRUE;
          }
          if(pkt_info_ptr->ip_hdr.v4.protocol == PS_IPPROTO_AH)
          {
            if( secipsah_hdr_parse(&bp,&offset,pkt_info_ptr) == -1)
            {
              LOG_MSG_INFO1_0("Error in parsing AH header");
              DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
              dsm_free_packet(&bp);
              PS_RX_META_INFO_FREE(&meta_info_ptr);
              return -1;
            }
            is_ipsec_processing_done = TRUE;
          }
          /* done_processing = FALSE;  needed only when supporting tunnel mode */
        }
      }

      /* update the transport layer info if needed and find the ipsec iface */
      /* This the point in which we will have to invalidate 
         (through the valid flag) packet info in the tunnel case */
      if (TRUE == is_ipsec_processing_done)
      {
        ps_ip_update_transport_layer_data(pkt_info_ptr,
          bp,
          offset,
          pkt_info_ptr->ip_hdr.v4.protocol);
        PS_PKT_INFO_SET_CKSUM_OFFLOAD_FLAG(pkt_info_ptr,FALSE);
      }
    }
#endif
  }

  /*------------------------------------------------------------------------
    Determine where to route the packet
      1. If the filter mask is set from the ps layer, send pkt up to stack
      2. else, use the next pointer already set
  --------------------------------------------------------------------------*/
  
  /* set assumed next iface */
  next_if_ptr = in_if_ptr->iface_private.back_iface_ptr;

  if(PS_RX_META_GET_FILTER_RESULT(meta_info_ptr, IP_FLTR_CLIENT_SOCKETS) ==
    PS_IFACE_IPFLTR_NOMATCH && next_if_ptr != NULL )
  {
    LOG_MSG_DATA_PATH_INFO_0("ip4_input(): Routing packet to next iface.");
    /*-----------------------------------------------------------------------
      pass to the next iface for input processing
    -----------------------------------------------------------------------*/
    if(next_if_ptr->iface_private.rx_proc_f_ptr != NULL)
    {
      /* filter result was reset in the common iface input function */
      (void) next_if_ptr->iface_private.rx_proc_f_ptr( next_if_ptr,
                                                       &bp,
                                                       meta_info_ptr);
      return 0;
    }
    else
    {
      DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
      dsm_free_packet(&bp);
      PS_RX_META_INFO_FREE(&meta_info_ptr);
      /* DATA_ERR_FATAL("ip4_input(): rx_proc_f_ptr is NULL, Dropping the pkt"); */
	  PS_IP4_ASSERT(0);
      return -1;
    }
  }
  else
  {
    LOG_MSG_DATA_PATH_INFO_0(
                    "ip4_input(): Comparing dest address with iface addreses");

    /*-------------------------------------------------------------------------
      Confirm that the destination address of the IP packet is same as that
      of the interface it came in.(only if the destination address is not
      multicast or a broadcast address)
    -------------------------------------------------------------------------*/
    if(!PS_IN_IS_ADDR_MULTICAST(pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) &&
       !PS_IN_IS_ADDR_LIMITED_BROADCAST(pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) &&
       (pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr != 0xFFFFFFFFUL))
    {
      /*-----------------------------------------------------------------------
        Note that this function performs no input validation
      -----------------------------------------------------------------------*/   
      if_ipv4_addr = ps_iface_get_cached_ipv4_addr(in_if_ptr);

      if(pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr != if_ipv4_addr &&
         !(IFACE_CONFIGURING == ps_iface_state(in_if_ptr) &&
           if_ipv4_addr == 0))
      {
        LOG_MSG_ERROR_0("Dest addr of IP pkt does not match the addr of interface "
                        "it came in, dropping pkt");
        DPL_LOG_NETWORK_RX_DROPPED_PACKET(in_if_ptr, bp, DPL_IID_NETPROT_IP);
        IPV4_INC_STATS(pkts_dropped_rx,1) ;
        dsm_free_packet(&bp);
        PS_RX_META_INFO_FREE(&meta_info_ptr);
        return -1;
      }
    }
    /*-----------------------------------------------------------------------
      Deliver the packet to the local stack
    -----------------------------------------------------------------------*/
    ip_receive(bp, offset, meta_info_ptr);
  }

  return 0;
}  /* ip4_input() */


/*===========================================================================

FUNCTION IP4_ROUTE()

DESCRIPTION
  This function finds the appropriate interface to send a
  given IPv4 packet on and forwards it appropriately.

DEPENDENCIES
  If this function calls another function, say X, then X is responsible for
  freeing up memory in ALL cases.

RETURN VALUE
  0 for success, -1 for failure (ps_errno contains error code)

   ps_errno can contain:
   E_NOTALLOWED - Pkt forwarding is disabled
   E_NOROUTE    - No route to destination could be found
   E_NONET      - Route to destination is cuurently not accessible

SIDE EFFECTS
  None
===========================================================================*/
int ip4_route
(
  ip_pkt_info_type*        ip_pkt_info_ptr,   /* Pkt and policy info       */
  ps_tx_meta_info_type*    meta_info_ptr,     /* Policy info e.g. from SCB */
  dsm_item_type**          ip_pkt_ptr,        /* Input packet              */
  ps_iface_type*           tx_if_cache,       /* cached Tx route           */
  int16*                   ps_errno           /* Error code for faillure   */
)
{
  ps_iface_type*  rt_if_ptr = NULL;
  ps_ip_addr_type gateway;
  ps_ip_addr_type next_hop_addr;
  int             ret_val = 0;
  boolean         same_subnet;
  dsm_item_type * clone_item_ptr = NULL;
  uint32          ip_pkt_len;
  int16           path_mtu = 0;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {
    /*-----------------------------------------------------------------------
      Check if this host allows forwarding between interfaces.
      The if_ptr in the filter info is set to the receiving
      interface or NULL for locally generated packets.
      IP Forwarding is currently allowed, hence the below code block
      commented, as the check always results to FALSE.
    -----------------------------------------------------------------------*/
    /*
    if ( ACL_IPFI_IF_PTR( ip_pkt_info_ptr )
         &&
         ! ps_ip_forwarding )
    {
      LOG_MSG_INFO2( "IPRoute: Not allowed to fwd pkt from %d",
               (ACL_IPFI_IF_PTR( ip_pkt_info_ptr ) ?
                ACL_IPFI_IF_PTR( ip_pkt_info_ptr )->name : -1), 0, 0 );

      IPV4_INC_STATS(no_route,1) ;
      *ps_errno = E_NOT_ALLOWED;
      dsm_free_packet( ip_pkt_ptr );
      PS_TX_META_INFO_FREE( &meta_info_ptr );
      ret_val = -1;
      break;
    }
    */

    /*-----------------------------------------------------------------------
      Check whether the packet is being sent to one of the addresses of our
      interfaces. If so then store the address of that iface_ptr in the
      app_ptr of dsm_item for input processing. Queue this packet to
      the loopback interface instead of going through the usual processing
      as this packet is destined to ourselves.
    -----------------------------------------------------------------------*/
    if(!PS_IN_IS_ADDR_MULTICAST(ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) &&
       !PS_IN_IS_ADDR_LIMITED_BROADCAST(ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr))
    {
      if((rt_if_ptr = ps_route_lo_ipv4(&ip_pkt_info_ptr->ip_hdr.v4.dest))
                                                                == tx_if_cache)
      {
        /*---------------------------------------------------------------------
          NOTE:  Meta Info and DSM Item are freed by tx_cmd. No need to free
          anything.
        ---------------------------------------------------------------------*/
        LOG_MSG_INFO3_1("Data going on IPv4 LO w/iface %p", rt_if_ptr);
        ret_val = lo_tx_cmd(rt_if_ptr,
                            ip_pkt_ptr,
                            meta_info_ptr,
                            NULL);
        if (-1 == ret_val)
        {
          LOG_MSG_ERROR_1("Failed to transmit data on LO w/iface 0x%p",
                          rt_if_ptr);
          IPV4_INC_STATS(no_route,1);
        }
        break;
      }
    }

    /*-----------------------------------------------------------------------
      Check if Multicast Packet needs to be looped back. For all Multicast
      datagrams we use the source interface to send it back and UDP layer
      will send it to all sockets bound to all interfaces.
    -----------------------------------------------------------------------*/
    if(PS_IN_IS_ADDR_MULTICAST(ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr))
    {
      /* Loopback enabled? */
      if (ip_pkt_info_ptr->is_mcast_loop)
      {
        /* Use the source interface to loop it back */
        if((rt_if_ptr = ps_route_lo_ipv4(&ip_pkt_info_ptr->ip_hdr.v4.source))
                                                                   != NULL)
        {
          /* Dup the packet and loop it back */
          ip_pkt_len = dsm_length_packet(*ip_pkt_ptr);
          if(dsm_dup_packet(&clone_item_ptr,
                            *ip_pkt_ptr,
                            0,
                            (uint16)ip_pkt_len) == (uint16)ip_pkt_len)
          {
            ret_val = lo_tx_cmd(rt_if_ptr,
                                &clone_item_ptr,
                                NULL, /* No metainfo */
                                NULL);
            if (-1 == ret_val)
            {
              LOG_MSG_ERROR_1("Failed to transmit data on LO w/iface 0x%p",
                              rt_if_ptr);
              IPV4_INC_STATS(no_route,1);
            }
          }
          else
          {
            LOG_MSG_ERROR_0("Running out of buffers - Loopback routing ignored");
          }
        } /* rt_if_ptr = ps_route_lo_ipv4() */
      } /* if(ip_pkt_info_ptr->is_mcast_loop) */

      /* If MCAST TTL is zero..the packet shouldn't go out from the device */
      if (ip_pkt_info_ptr->ip_hdr.v4.ttl == 0)
      {
        /* Consume the packet */
        LOG_MSG_INFO1_0("MCAST TTL is zero, dropping packet!");
        DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
        dsm_free_packet( ip_pkt_ptr );
        PS_META_INFO_FREE( &meta_info_ptr );
        return 0;
      } /* ip_pkt_info_ptr->ip_hdr.v4.ttl == 0 */
    } /* IN_IS_ADDR_MULTICAST */

    /*-----------------------------------------------------------------------
      Check if we have an available route cached.
      Use the cached entry only if the interface is not disabled
    -----------------------------------------------------------------------*/
    if ( tx_if_cache && (ps_iface_state( tx_if_cache ) != IFACE_DISABLED) )
    {
      LOG_MSG_INFO3_2( "RT: Cache match (if = 0x%x[%d])",
                       tx_if_cache, tx_if_cache->instance );
      rt_if_ptr = tx_if_cache;
    }
    /*-----------------------------------------------------------------------
      Get the route for this destination and deliver pkt to acc. interface
    -----------------------------------------------------------------------*/
    else if ( route_get( ip_pkt_info_ptr,
                         NULL,
                         FALSE,
                         &gateway,
                         &rt_if_ptr ) == -1 )
    {
      LOG_MSG_INFO1_1( "IPRoute: No route, discard pkt from %d",
                (ACL_IPFI_IF_PTR( ip_pkt_info_ptr ) ?
                 ACL_IPFI_IF_PTR( ip_pkt_info_ptr )->name : -1 ));
      IPV4_INC_STATS(no_route,1) ;
      DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
      dsm_free_packet( ip_pkt_ptr );
      PS_TX_META_INFO_FREE( &meta_info_ptr );
      ret_val = -1;
      *ps_errno = E_NO_ROUTE;
      break;
    }

    /*-----------------------------------------------------------------------
      Destination IF should be different from Source IF
    -----------------------------------------------------------------------*/
    if(rt_if_ptr != ip_pkt_info_ptr->if_ptr)
    {
      /*---------------------------------------------------------------------
        Save the routing result in the ip filter info
      ---------------------------------------------------------------------*/
      ip_pkt_info_ptr->if_ptr = (void*) rt_if_ptr;
    }
    else
    {
      LOG_MSG_INFO1_1( "IPRoute: Loopback Route, discard pkt from %d",
                (ACL_IPFI_IF_PTR( ip_pkt_info_ptr ) ?
                 ACL_IPFI_IF_PTR( ip_pkt_info_ptr )->name : -1 ));
      *ps_errno = E_NOT_ALLOWED;
      DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
      dsm_free_packet( ip_pkt_ptr );
      PS_TX_META_INFO_FREE( &meta_info_ptr );
      ret_val = -1;
      break;
    }

    /* should we allow broadcast packets on non-broadcast interface?? */

    /*-----------------------------------------------------------------------
      If interface supports broadcast, fill in relevant meta info
    -----------------------------------------------------------------------*/
    if( PS_IFACE_GET_IS_BCAST_IFACE(rt_if_ptr) == TRUE )
    {
      /*---------------------------------------------------------------------
        Broadcast interface specific code
        - If metainfo not present, create a new meta info item.
        - Set the next hop address and the IP address scope for the packet
          being enqueued.
      ---------------------------------------------------------------------*/
      if(NULL == meta_info_ptr)
      {
        PS_TX_META_INFO_AND_RT_META_INFO_GET(meta_info_ptr);
        if((NULL == meta_info_ptr) ||
           (NULL == PS_TX_META_GET_RT_META_INFO_PTR(meta_info_ptr)))
        {
          DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
          *ps_errno = E_NO_MEMORY;
          DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
          dsm_free_packet(ip_pkt_ptr);
          PS_TX_META_INFO_FREE(&meta_info_ptr);
          ret_val = -1;
          break;
        }
      }

      /*---------------------------------------------------------------------
        Determine if the destination is on the same subnet as the outgoing
        interface.  Subnet is calculated as (subnet_mask & host_ip_address)

        This is used below to determine the scope of the outgoing packet.
      ---------------------------------------------------------------------*/
      same_subnet = ( rt_if_ptr->v4_net_info.net_mask.ps_s_addr &
                      PS_IFACE_IP_V4_ADDR(rt_if_ptr) ) ==
                    ( rt_if_ptr->v4_net_info.net_mask.ps_s_addr &
                      ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr );

      /*---------------------------------------------------------------------
        Destination is a broadcast address if

        a.  Destination address starts with 0xF
        b.  Destination address is the subnet broadcast address
        (which is basically the subnet number with all host bits set to 1)

        Subnet number is calculated as (subnet_mask & host_ip_address)
      ---------------------------------------------------------------------*/
      if( PS_IN_IS_ADDR_LIMITED_BROADCAST(
           ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) ||
           (same_subnet && ((ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr &
                             ~rt_if_ptr->v4_net_info.net_mask.ps_s_addr) ==
                             ~rt_if_ptr->v4_net_info.net_mask.ps_s_addr)) )
      {
        /*-------------------------------------------------------------------
          Destination is a broadcast address -> fill relevant meta info
        -------------------------------------------------------------------*/
        PS_TX_META_SET_IP_ADDR_SCOPE( meta_info_ptr, IP_ADDR_BROADCAST);
      }
      /*---------------------------------------------------------------------
        Destination is a multicast address if in the range of
        224.0.0.0 to 239.255.255.255

        i.e. if address starts with 0xE
      ---------------------------------------------------------------------*/
      else if( PS_IN_IS_ADDR_MULTICAST(ip_pkt_info_ptr->ip_hdr.v4.dest.ps_s_addr) )
      {
        /*-------------------------------------------------------------------
          Destination is a multicast address -> fill relevant meta info
        -------------------------------------------------------------------*/
        PS_TX_META_SET_IP_ADDR_SCOPE( meta_info_ptr, IP_ADDR_MULTICAST);
        /*-----------------------------------------------------------------
            next hop is itself.
        -----------------------------------------------------------------*/
        next_hop_addr.type    = IPV4_ADDR;
        next_hop_addr.addr.v4 = ip_pkt_info_ptr->ip_hdr.v4.dest;
        PS_TX_META_SET_NEXT_HOP_ADDR( meta_info_ptr, next_hop_addr);
      }
      else
      {
        /*-------------------------------------------------------------------
          In the default case, the packet is unicast

          Next hop IP address need only be populated in this case, since
          otherwise the link layer will broadcast the packet (ARP is only
          place that needs next hop, and ARP is only performed for unicast
          packets)
        -------------------------------------------------------------------*/
        PS_TX_META_SET_IP_ADDR_SCOPE( meta_info_ptr, IP_ADDR_UNICAST);

        /*-------------------------------------------------------------------
          If destination and source are in the same subnet, send directly
          to the host.  if not, send to the gateway.
        -------------------------------------------------------------------*/
        if( !same_subnet )
        {
          /*-----------------------------------------------------------------
            Different subnet - next hop is the gateway
          -----------------------------------------------------------------*/
          next_hop_addr.type    = IPV4_ADDR;
          next_hop_addr.addr.v4 = rt_if_ptr->v4_net_info.gateway;
        }
        else
        {
          /*-----------------------------------------------------------------
            Same subnet - next hop is host itself
          -----------------------------------------------------------------*/
          next_hop_addr.type    = IPV4_ADDR;
          next_hop_addr.addr.v4 = ip_pkt_info_ptr->ip_hdr.v4.dest;
        }

        PS_TX_META_SET_NEXT_HOP_ADDR( meta_info_ptr, next_hop_addr);
      }
    }

    /*-----------------------------------------------------------------------
      If packet length is larger then the PATH MTU then try fragmenting
    -----------------------------------------------------------------------*/
    /* Get the path mtu value */
    path_mtu = ps_ip_calc_path_mtu( IFACE_IPV4_ADDR_FAMILY,
                                    rt_if_ptr);
    if ( path_mtu == -1 )
    {
      LOG_MSG_ERROR_1( "Unable to get the path mtu value, on iface 0x%x",
                       rt_if_ptr);
      DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
      dsm_free_packet( ip_pkt_ptr );
      PS_TX_META_INFO_FREE( &meta_info_ptr );
      ret_val = -1;
      break;
    }

    if(ip_pkt_info_ptr->ip_hdr.v4.length > (uint16)path_mtu )
    {
      if( ip_pkt_info_ptr->ip_hdr.v4.df == 0 )
      {
        /*-------------------------------------------------------------------
          NOTE:  Meta Info and DSM Item are freed by tx_cmd. No need to free
          anything.
        -------------------------------------------------------------------*/
        /*-----------------------------------------------------------------
          Statistics collection : Increment frag_ok if fragmentation is
          successful; else increment frag_fails
        -----------------------------------------------------------------*/
        ret_val = ip4_fragment(ip_pkt_ptr,
                               &ip_pkt_info_ptr->ip_hdr.v4,
                               rt_if_ptr,
                               meta_info_ptr,
                               path_mtu);
        if (-1 == ret_val)
        {
          LOG_MSG_ERROR_2("Unable to fragment IP packet, 0x%x, on iface 0x%x",
                          ip_pkt_ptr, rt_if_ptr);
          IPV4_INC_STATS(frag_fails,1) ;
          IPV4_INC_STATS(pkts_dropped_tx,1);
          break;
        }
        else
        {
           IPV4_INC_STATS(frag_ok,1) ;
        }
      }
      else
      {
        LOG_MSG_INFO1_3("As packet's (0x%x) size (%d) is greater "
                 "than PATH MTU (%d) and DF (don't fragment) bit is set on it, "
                 "packet is dropped",
                 ip_pkt_ptr,
                 ip_pkt_info_ptr->ip_hdr.v4.length,
                 path_mtu);
        /*-------------------------------------------------------------------
        Statistics : Increment frag_fails
        -------------------------------------------------------------------*/
        IPV4_INC_STATS(frag_fails,1) ;
        DPL_LOG_NETWORK_RX_DROPPED_PACKET(rt_if_ptr, *ip_pkt_ptr, DPL_IID_NETPROT_IP);
        dsm_free_packet( ip_pkt_ptr );
        PS_TX_META_INFO_FREE( &meta_info_ptr );
        ret_val = -1;
        break;
      }
    }
    else
    {
      /*-----------------------------------------------------------------------
        Log the timestamp before sending the packet
      -----------------------------------------------------------------------*/ 
      PS_DSM_SET_TIMESTAMP(*ip_pkt_ptr,PS_GET_TIMESTAMP_3_4_MS());  
    
      /*---------------------------------------------------------------------
        Call the interface's write function w/ the next hop's address

        NOTE:  Meta Info and DSM Item are freed by tx_cmd. No need to free
        anything.
      ---------------------------------------------------------------------*/
      ret_val = ps_iface_tx_cmd(rt_if_ptr, ip_pkt_ptr, meta_info_ptr);
      if (-1 == ret_val)
      {
        LOG_MSG_ERROR_1("Failed to transmit data on iface 0x%x", rt_if_ptr);
        break;
      }
    }
  } while( 0 );

  return ret_val;

}  /* ip4_route() */
