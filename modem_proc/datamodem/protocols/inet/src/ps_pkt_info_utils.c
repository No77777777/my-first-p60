/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ P K T _ I N F O . C

DESCRIPTION
  Header containing packet information which needs to be passed through
  layers.

Copyright (c) 2005-2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/src/ps_pkt_info_utils.c#3 $ $DateTime: 2020/01/15 09:31:56 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/15/20    rj     Free the DSM item where base ipv6_header parse fails 
                   for the invalid packet.
06/11/18    rj     CR 2188309: KW Image Metrics (Lykan) - 02/09 fix
12/12/17    js     Added support for RFC2460 Regulator Mandatory Requirement
13/12/17    my     CR 2157129 : FR 42790: F3 reduction
05/08/15    skc    Use of dsm_item_set_priority() to set priority
03/25/15    cb     Fix LLVM SA warnings 
12/23/14    ss     Add limit on maximum entries in frag queue
11/11/14    cx     Added support for checksum offload
11/09/14    yl     IP data path re-architecture
07/08/13    cx     Replace memcpy and memmove with memscpy and memsmove
05/15/13    ssh    Don't assert if fail to obtain sufficient continuous bytes
03/27/13    ssh    Purge new enqueued tx/rx frags if meta-info already cached
02/25/13    op     Using saved iface information in fraghdl to determine
                   packet routing
02/05/13    ssb    Removing overhead f3 message
12/03/12    ss     Adding logic to validate IPv6 frag header.
09/20/12    dvk    Made changes for accessing tcp_ack_prio_enabled global 
                   variable from a accessor function.
03/14/12    ss     Using ps_ip4_hdr_parse API to extract IP header.
09/14/11    sm     Added TCP ACK PRIO NV item check for dynamic enablement
                   of ACK tagging.
08/11/11    sm     FEATURE_DATA_PS_TCP_ACK_PRIO Tagging/Prioritization.
03/11/11    am     Added ICMP ID field to pkt_info struct.
01/20/11    mct    IPSec bug fixes.
10/18/10    pp     API ps_transport_ptcl_pkt_info_generate added.
05/18/10    pp     Parse & skip HopByHop header during Packet Info generation.
08/29/09    am     Chk IPV6 version before calling ps_ip6_frag_hdr_parse.
07/27/09    sp     Fixed IP Fragmentation issues.
06/24/09    am     Renamed hton/ntoh macros with ps_ prefix.
10/24/08    pp     Fixed compiler warnings observed in OFFTARGET build.
10/14/08    pp     Metainfo fixes to handle frag cases correctly.
09/12/08    pp     Metainfo optimizations.
07/08/07    msr    Fixed merge issue in parsing ESP/AH headers
05/21/07    sv     Updated pkt info generate to parse ESP/AH headers.
04/29/07    ss     Added support for ICMPv6 sockets (PS_IPPROTO_ICMP6).
03/23/07    scb    Fixed NULL check
03/16/07    scb    Fixed Klocwork High errors
02/21/07    rt     Return ENOMEM if fragment allocation fails.
02/21/07    scb    Fixed High LINT errors
02/06/07    scb    Fixed Klockwork Critical errors
08/14/06    mct    Meta info should be set in all cases, not just if first
                   bytes are received. Ensures icmp errs can always be sent.
07/07/06    msr    Checking if filter mask is set instead of filter result
04/19/06    rt     Modified IPv6 header names.
12/15/05    msr    Using macros instead of directly looking in to fi_result of
                   meta info
11/05/05    rt     Fixed coverity errors.
09/02/05    msr    Optimized generation of pkt_info in ps_pkt_info_generate()
08/29/05    msr    Fixed overwriting of metainfo in ps_pkt_info_generate()
08/16/05    sv     Addeds support for filtering ESP and AH packets.
05/25/05    vp     Fixed bug in processing of routing header in
                   ps_pkt_info_process_pkt_ipv6().
05/12/05    rt     Fixed Lint Errors.
04/22/05    mct    Fixed some merge issues.
01/25/05    sv     Created the file.
===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"
#include "ps_pkt_info_utils.h"
#include "ps_in.h"
#include "ps_ip4_hdr.h"
#include "ps_stat_ipv4.h"
#include "ps_ip_fraghdl.h"
#include "ps_iface.h"
#include "ps_byte.h"
#include "ps_udp_io.h"
#include "ps_metai_info.h"
#ifdef FEATURE_DATA_PS_IPV6
#include "ps_ip6_hdr.h"
#include "ps_ip6i_hdr.h"
#endif
#ifdef FEATURE_SEC_IPSEC
#include "secipssp.h"
#endif /* FEATURE_SEC_IPSEC */
#include "ds_Utils_DebugMsg.h"
#include "ps_icmp.h"
#include "ps_icmp6_msg.h"
#include "ps_tcp_hdr.h"
#include "ps_iputil.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif

/*---------------------------------------------------------------------------
  TCP ACK PRIORITY NV item (by default enabled)
---------------------------------------------------------------------------*/
static uint8  tcp_ack_prio_enabled = 1;

/*-----------------------------------------------------------------------
Macros used for TCP ACK parsing
-----------------------------------------------------------------------*/
#define IPV6_HEADER_NEXT_HEADER_FIELD_OFFSET 6
#define TCP_HEADER_LENGTH_FIELD_OFFSET 12
#define TCP_HEADER_FLAGS_FIELD_OFFSET 13
#define TCP_ACK_BIT_MASK 0x10

#define PS_PKT_INFO_UTILS_ASSERT(expression)  \
          ps_pkt_info_utils_assert_wrapper(__LINE__, expression)


/*===========================================================================

FUNCTION ps_pkt_info_utils_assert_wrapper

DESCRIPTION
  Wrapper function for PS_PKT_INFO_UTILS ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

=============================================================================*/
static void ps_pkt_info_utils_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("PS_PKT_INFO_UTILS_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
  
}/* ps_pkt_info_utils_assert_wrapper */



/*===========================================================================
FUNCTION PS_PKT_INFO_VALIDATE_IPV4()

DESCRIPTION
  Validate the IPv4 header (checksum, length sanity check).

DEPENDENCIES

PARAMETERS
  pkt_ref_ptr - IPv4 pkt.

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  none.
===========================================================================*/
static int ps_pkt_info_validate_ipv4
(
  dsm_item_type                       *pkt_ref_ptr_ptr
)
{
  uint16 ip_len;

  /*-------------------------------------------------------------------------
    Sneak a peek at the IP header's IHL field to find its length
  -------------------------------------------------------------------------*/
  ip_len = (pkt_ref_ptr_ptr->data_ptr[0] & 0xf) << 2;
  if(ip_len < IPLEN)
  {
    /*-----------------------------------------------------------------------
      The IP header length field is too small
    -----------------------------------------------------------------------*/
    IPV4_INC_STATS(bad_hdr_len,1) ;
    return -1;
  }

  if(dsm_length_packet(pkt_ref_ptr_ptr) <= ip_len)
  {
    /*-----------------------------------------------------------------------
      The packet is shorter than or equal to header length.
      Even packets equal to IP hdr must be dropped. No payload to be passed
      on to upper protocols. Better to drop here itself.
    -----------------------------------------------------------------------*/
    IPV4_INC_STATS(too_short,1);
    return -1;
  }

  if(cksum(NULL,pkt_ref_ptr_ptr,ip_len, 0) != 0)
  {
    /*-------------------------------------------------------------------------
      Bad IP header checksum; discard
    -------------------------------------------------------------------------*/
    IPV4_INC_STATS(bad_chksum,1) ;
    return -1;
  }

  return 0;
}

#ifdef FEATURE_DATA_PS_IPV6
/*===========================================================================
FUNCTION PS_PKT_INFO_VALIDATE_IPV6_FRAG_HDR()

DESCRIPTION
  Validate the IPv6 fragment header in the passed in pkt.

DEPENDENCIES

PARAMETERS
  pkt_ref_ptr - IPv6 pkt.
  pi_ref_ptr  - Pkt info.
  dsm_offset  - Offset  to frag hdr in pkt.
  next_hdr    - If frag hdr removed, would indicate the next hdr.

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  Fragment header would be removed if invalid.
===========================================================================*/
static int ps_pkt_info_validate_ipv6_frag_hdr
(
  dsm_item_type                       **pkt_ref_ptr_ptr,
  ip_pkt_info_type                     *pi_ptr,
  uint16                               *dsm_offset_ptr,
  uint8                                *next_hdr_ptr
)
{
  uint32   mf;
  uint16   start_offset;
/*-------------------------------------------------------------------------*/

  /* Parse the fragment header */
  if( TRUE == ps_ip6_frag_hdr_parse( *pkt_ref_ptr_ptr, dsm_offset_ptr,
                                     &pi_ptr->ip_hdr.v6) )
  {
    /* Extract mf and offset field */
    mf = pi_ptr->ip_hdr.v6.hdr_body.frag_hdr.offset_flags & 0x1;
    start_offset =
      pi_ptr->ip_hdr.v6.hdr_body.frag_hdr.offset_flags & ~0x7;

    /* More fragment and offset should not both be zero for a valid 
       fragment */
    /* FR 43289 upper layer will take care  */
    if( 0 == mf && 0 == start_offset && *next_hdr_ptr != (uint8)PS_IPPROTO_FRAG_HDR )
    {
      LOG_MSG_ERROR_0("IPv6 frag with both mf/offset 0, ignoring frag hdr");

      /* Update next header for pkt info */
      *next_hdr_ptr = pi_ptr->ip_hdr.v6.hdr_body.frag_hdr.next_hdr;
    }
    else
    {
      /* Genuine frag hdr, restore dsm_offset */
      *dsm_offset_ptr -= sizeof(ip6_frag_hdr_type);
    }
  }
  else
  {
    LOG_MSG_ERROR_0("ps_ip6_frag_hdr_parse() returned FALSE");
    return -1;
  }

  return 0;
}

/*===========================================================================
FUNCTION PS_PKT_INFO_PROCESS_PKT_IPV6()

DESCRIPTION

DEPENDENCIES

PARAMETERS
  iface_ptr   - Iface to pick filters from for execution
  client_id   - Filtering client id
  pkt_ptr     - Ptr to IP pkt to process
  mi_ref_ptr  - Reference to a an optional ptr to meta info block
  fraghdl     - Cookie indicating presence of other fragments.
                Retruns non-NULL if fragments present.
  ps_errno       - Error value returned by the function

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error
  In case of presence of fragments the fraghdl argument is set to a
  non-NULL value.  In case of error the error value is placed in ps_errno

SIDE EFFECTS

===========================================================================*/
static int ps_pkt_info_process_pkt_ipv6
(
  dsm_item_type                       **pkt_ref_ptr,
  ip_pkt_info_type                    **pi_ref_ptr,
  uint16                               *dsm_offset,
  errno_enum_type                      *ps_errno
)
{
  uint8               next_hdr;
  uint16              packet_len;
  uint16              payload_len;
  uint8               extract_buf[2];
  dsm_item_type      *pkt_ptr          = *pkt_ref_ptr;
  ip_pkt_info_type   *pi_ptr;
  uint16              prev_offset      = IP6LEN;  
  uint16              next_hdr_offset  = IP6_BASE_HDR_NEXT_HDR_OFFSET;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    If the length of the IP packet is less than the length of the IPv6 header
    free the IP packet.
  -------------------------------------------------------------------------*/
  if((packet_len = (uint16)(dsm_length_packet(pkt_ptr) - *dsm_offset)) < IP6LEN)
  {
    /* The packet is shorter than a legal IP header */
    *ps_errno = E_BAD_DATA;
    return -1;
  }

  /*-------------------------------------------------------------------------
    If the length of the payload specified in the IP header is less than the
    length of the IP packet free the IP packet.
    Payload Length is specified at an offset of 5 bytes and is 16 bytes long.
  -------------------------------------------------------------------------*/
  (void) dsm_extract(pkt_ptr,4,extract_buf,2);
  payload_len = get16((byte*)extract_buf);
  if (packet_len < payload_len)
  {
    *ps_errno = E_BAD_DATA;
    return -1;
  }

  /*-----------------------------------------------------------------------
    Get the IPv6 base header
  -----------------------------------------------------------------------*/
  pi_ptr = *pi_ref_ptr;
  pi_ptr->ip_hdr.v6.hdr_type = PS_IPV6_BASE_HDR;
  if(FALSE == ps_ip6_base_hdr_parse( pkt_ptr, dsm_offset, &pi_ptr->ip_hdr.v6 ))
  {
    //DSM item should be freed later by the subsequent caller 
    *ps_errno = E_BAD_DATA;
    return -1;
  }
  next_hdr = pi_ptr->ip_hdr.v6.hdr_body.base_hdr.next_hdr;

  /*-------------------------------------------------------------------------
    Process all the extension headers until an upper layer header,
    fragmentation, AH/ESP, or the header NO_NEXT_HEADER is found.
  -------------------------------------------------------------------------*/
  while (next_hdr != (uint8)PS_IPPROTO_TCP &&
         next_hdr != (uint8)PS_IPPROTO_UDP &&
         next_hdr != (uint8)PS_IPPROTO_FRAG_HDR &&
         next_hdr != (uint8)PS_IPPROTO_AH &&
         next_hdr != (uint8)PS_IPPROTO_ESP &&
         next_hdr != (uint8)PS_IPPROTO_ICMP6)
  {
    LOG_MSG_INFO1_1("Handle %d header in filtering", next_hdr);
    switch( next_hdr )
    {
    case PS_IPPROTO_ROUTING_HDR:
      if (0 == dsm_extract(pkt_ptr, *dsm_offset,
                           extract_buf, sizeof(uint16)))
      {
        return -1;
      }
      *dsm_offset += sizeof(ip6_routing_hdr_type) + (extract_buf[1]<<3);
      next_hdr = extract_buf[0];
      break;

    case PS_IPPROTO_HOP_BY_HOP_OPT_HDR:
      if (0 == dsm_extract(pkt_ptr, *dsm_offset,
                           extract_buf, sizeof(uint16)))
      {
        return -1;
      }
      *dsm_offset += sizeof(ip6_hopbyhop_hdr_type) + (extract_buf[1]<<3);
      next_hdr = extract_buf[0];
      break;

    case PS_IPPROTO_DEST_OPT_HDR:
      if (0 == dsm_extract(pkt_ptr, *dsm_offset,
                           extract_buf, sizeof(uint16)))
      {
        return -1;
      }

      /*---------------------------------------------------------------------
         RFC2460: Hdr Ext Len
         8-bit unsigned integer. Length of the Destination Options header in
         8-octet units, not including the first 8 octets.
      ---------------------------------------------------------------------*/
      *dsm_offset += ( (extract_buf[1]+1) << 3 );
      
      next_hdr = extract_buf[0];
      break;

    case PS_NO_NEXT_HDR:
      pi_ptr->ip_hdr.v6.hdr_type = PS_NO_NEXT_HDR;
      return 0;

    default:
      *ps_errno = E_BAD_DATA;
      pi_ptr->ptcl_info.icmp.type = ICMP6_PAR_PROBLEM;
      pi_ptr->ptcl_info.icmp.code = ICMP6_UNRECOGNIZED_NEXT_HDR;
      pi_ptr->ptcl_info.icmp.prob_offset = next_hdr_offset;

      LOG_MSG_ERROR_1("Invalid Next Hdr %d", *dsm_offset);
      return -1;
    }

    next_hdr_offset = prev_offset;
    prev_offset     = *dsm_offset;
  }

  /* If this is a fragment, validate */
  if( next_hdr == (uint8)PS_IPPROTO_FRAG_HDR )
  {
    if( ps_pkt_info_validate_ipv6_frag_hdr( pkt_ref_ptr,
                                            pi_ptr,
                                            dsm_offset,
                                            &next_hdr ) != 0 )
    {
      LOG_MSG_ERROR_0("Invalid IPv6 frag.");
      return -1;
    }
  }

  pi_ptr->ip_hdr.v6.hdr_type = (ip6_hdr_enum_type) next_hdr;

  return 0;
} /* ps_pkt_info_process_pkt_ipv6() */
#endif
/*===========================================================================
FUNCTION PS_PKT_INFO_PROCESS_TX_FRAGMENT()

DESCRIPTION
  This function processes an IP fragment, executing all filters in an
  iface installed by the client
  The filtering results and information extracted from IP packet is stored in
  the TX meta info which is passed along with the packet down the stack.
  If the packet has an associated meta info, the information already
  available in meta info is used as is and remaining fields are updated.

DEPENDENCIES
  IP pkt information should be passed inside meta info or a valid IP packet
  should be provided.

PARAMETERS
  pkt_ptr            - Ptr to IP pkt to process
  meta_info_ref_ptr  - Reference to a an optional ptr to TX meta info block
  ps_errno              - Returned error

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  TX meta info is generated which contains the filtering result and extracted
  pkt information if packet processing is successful.

===========================================================================*/
static int ps_pkt_info_process_tx_fragment
(
  dsm_item_type                       **pkt_ptr,
  ps_tx_meta_info_type                **meta_info_ref_ptr,
  uint16                                offset,
  errno_enum_type                      *ps_errno
)
{
  void                    *fraghdl;
  ps_tx_meta_info_type    *result = NULL;
  ps_tx_meta_info_type    *mi_ptr = NULL;
  ip_pkt_info_type        *pi_ptr;
#ifdef FEATURE_SEC_IPSEC
  char                     buf[12];
#else
  char                     buf[6];
#endif
  uint16                   bytes_needed  = sizeof(buf);
  ps_ip_protocol_enum_type next_hdr = PS_NO_NEXT_HDR;
  boolean                  allocated = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(NULL == meta_info_ref_ptr || NULL == (mi_ptr = *meta_info_ref_ptr) ||
     (NULL == PS_TX_META_GET_RT_META_INFO_PTR(mi_ptr)))
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return -1;
  }

  pi_ptr = &PS_TX_META_GET_PKT_INFO(mi_ptr);
  fraghdl = pi_ptr->fraghdl;

  /*-------------------------------------------------------------------------
    Locate a fragment handling entry, if already allocated, for this
    fragment.
  -------------------------------------------------------------------------*/
  if (NULL == fraghdl)
  {
    LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_tx_fragment: Looking up fraghdl (%d) for pkt ptr", fraghdl);
    if( NULL == (fraghdl = ip_fraghdl_lookup( pi_ptr )) )
    {
      /*---------------------------------------------------------------------
        No fragment handling entry found, allocate a new one.
      ---------------------------------------------------------------------*/
      if( NULL == (fraghdl = ip_fraghdl_alloc( *pkt_ptr, offset, pi_ptr )) )
      {
        DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
        *ps_errno = E_NO_MEMORY;
        return -1;
      }
      allocated = TRUE;
      /*---------------------------------------------------------------------
        Must always have a valid meta info in the fraghdl to allow for
        sending icmp errors.
      ---------------------------------------------------------------------*/
      LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_process_tx_fragment: Lookup failed" 
                           "allocating frag entry and Initializing meta info");
      PS_TX_META_INFO_DUP(mi_ptr, &result);
      ip_fraghdl_set_init_meta_info_cache(fraghdl, (void *)result,
                                          IP_FRAGHDL_TX_META_INFO);
    }
  }

  pi_ptr->fraghdl = fraghdl;

  LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_tx_fragment: Checking for cached meta info for fraghdl (%d).", fraghdl);
  /*-------------------------------------------------------------------------
    Located a fragment handling entry.Check if already have cached meta-info.
  -------------------------------------------------------------------------*/
  if( 0 == ip_fraghdl_get_cached_tx_meta_info( fraghdl, &result ) )
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_process_tx_fragment: Got cached meta info.");
    /*-------------------------------------------------------------------
      Copy the meta info and return;
    -------------------------------------------------------------------*/
    PS_TX_META_INFO_FREE(meta_info_ref_ptr);
    PS_TX_META_INFO_DUP(result, meta_info_ref_ptr);

    LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_process_tx_fragment: Duplicating cached meta info and returning.");

    /*-------------------------------------------------------------------
      If this fragment was enqueued due to new entry allocation, delete it
    -------------------------------------------------------------------*/
    if( TRUE == allocated ) 
    {
      LOG_MSG_ERROR_1("proc_TX_frag: found cached meta-info! Dequeuing frag 0x%x",
                      *pkt_ptr);
      ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );
    }
    return 0;
  }

  /*---------------------------------------------------------------------
    No pkt info  yet.  Put the latest fragment on the fragment
    handling structure.
  ---------------------------------------------------------------------*/
  if (FALSE == allocated)
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_process_tx_fragment: Putting latest fragment in the queue.");
    if(ip_fraghdl_put( fraghdl, *pkt_ptr, pi_ptr ) != 0)
    {
      LOG_MSG_ERROR_0("Failed to put frag to queue, freeing packet");

      /* Free the packet and set error code */
      dsm_free_packet( pkt_ptr );
      *pkt_ptr = NULL;
      *ps_errno = E_WOULD_BLOCK;
      return -1;
    }
  }

  /*---------------------------------------------------------------------
    Check if we have enough bytes from the start to filter.
  ---------------------------------------------------------------------*/
  LOG_MSG_DATA_PATH_INFO_2("ps_pkt_info_process_tx_fragment: Checking initial cont bytes(%d) > bytes_needed (%d).", 
                           ip_fraghdl_get_initial_cont_bytes_count( fraghdl ),
                           bytes_needed);
  if( ip_fraghdl_get_initial_cont_bytes_count( fraghdl ) >= bytes_needed )
  {
    /*-------------------------------------------------------------------
      Extract the bytes
    -------------------------------------------------------------------*/
    if( -1 == ip_fraghdl_get_initial_cont_bytes(fraghdl,
                                                buf,
                                                sizeof(buf),                                      
                                                bytes_needed) )
    {
      LOG_MSG_DATA_PATH_INFO_1( "ps_pkt_info_process_tx_fragment: Unable to get desired bytes from fraghdl (%d)", fraghdl);
      ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );
      return -1;
    }

    /*-------------------------------------------------------------------
      Delete the passed fragment from the queue.
    -------------------------------------------------------------------*/
    ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );

    /*-------------------------------------------------------------------
      Find protocol payload
    -------------------------------------------------------------------*/
    if (IP_V4 == pi_ptr->ip_vsn)
    {
      next_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v4.protocol;
    }
    else
    {
#ifdef FEATURE_DATA_PS_IPV6
      next_hdr = ip_fraghdl_get_xport_proto(fraghdl);
      pi_ptr->ip_hdr.v6.hdr_type = next_hdr;
#endif
    }

    /*-------------------------------------------------------------------
      Use the IP payload bytes to setup information needed by filter
    -------------------------------------------------------------------*/
    switch( next_hdr )
    {
      case PS_IPPROTO_TCP:
        pi_ptr->ptcl_info.tcp.src_port = *((uint16*)&buf[0]);
        pi_ptr->ptcl_info.tcp.dst_port = *((uint16*)&buf[2]);
        break;

      case PS_IPPROTO_UDP:
        pi_ptr->ptcl_info.udp.src_port = *((uint16*)&buf[0]);
        pi_ptr->ptcl_info.udp.dst_port = *((uint16*)&buf[2]);
#ifdef FEATURE_SEC_IPSEC
        /* handle UDP encapsulated ESP packets. */
        if(pi_ptr->ptcl_info.udp.dst_port
           == ps_htons(SECIPSTYPES_GATEWAY_NATT_PORT))
        {
          pi_ptr->esp_hdr.spi =  *((uint32*)&buf[8]);
        }
#endif
        break;

      case PS_IPPROTO_ICMP:
      case PS_IPPROTO_ICMP6:
        pi_ptr->ptcl_info.icmp.type = buf[0];
        pi_ptr->ptcl_info.icmp.code = buf[1];
        if (pi_ptr->ptcl_info.icmp.type == ICMP_ECHO_REPLY ||
            pi_ptr->ptcl_info.icmp.type == ICMP_ECHO_REQ ||
            pi_ptr->ptcl_info.icmp.type == ICMP6_ECHO_REQ ||
            pi_ptr->ptcl_info.icmp.type == ICMP6_ECHO_RSP)
        {
          /* Store the ID */
          pi_ptr->ptcl_info.icmp.id = *((uint16*)&buf[4]);
        }
        break;

      case PS_IPPROTO_ESP:
        pi_ptr->esp_hdr.spi =  *((uint32*)&buf[0]);
        break;

      case PS_IPPROTO_AH:
        pi_ptr->ah_hdr.spi =  *((uint32*)&buf[4]);
        break;

      default:
        /* try filtering only with IP information */
        break;
    }
    PS_TX_META_INFO_DUP(mi_ptr, &result);
    LOG_MSG_DATA_PATH_INFO_1( "ps_pkt_info_process_tx_fragment: Setting tx cached meta info to valid for fraghdl (%d)", fraghdl);
    ip_fraghdl_set_tx_meta_info_cache(fraghdl, result);
  }
  else
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_process_tx_fragment: Failed to obtain initial cont bytes, returning E_WOULDBLOCK");
    /*-------------------------------------------------------------------
      Packet needs to be queued.  Free pkt_ptr
    -------------------------------------------------------------------*/
    *pkt_ptr = NULL;
    *ps_errno = E_WOULD_BLOCK;
    return -1;
  }

  return 0;
} /* ps_pkt_info_process_tx_fragment */

/*===========================================================================
FUNCTION PS_PKT_INFO_PROCESS_RX_FRAGMENT()

DESCRIPTION
  This function processes an IP fragment, executing all filters in an
  iface installed by the client
  The filtering results and information extracted from IP packet is stored in
  the RX meta info which is passed along with the packet up the stack.
  If the packet has an associated meta info, the information already
  available in meta info is used as is and remaining fields are updated.

DEPENDENCIES
  IP pkt information should be passed inside meta info or a valid IP packet
  should be provided.

PARAMETERS
  pkt_ptr            - Ptr to IP pkt to process
  meta_info_ref_ptr  - Reference to a an optional ptr to RX meta info block
  ps_errno              - Returned error

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  RX meta info is generated which contains the filtering result and extracted
  pkt information if packet processing is successful.

===========================================================================*/
static int ps_pkt_info_process_rx_fragment
(
  dsm_item_type                       **pkt_ptr,
  ps_rx_meta_info_type                **meta_info_ref_ptr,
  uint16                                offset,
  errno_enum_type                      *ps_errno
)
{
  void                    *fraghdl;
  ps_rx_meta_info_type    *result = NULL;
  ps_rx_meta_info_type    *mi_ptr = NULL;
  ip_pkt_info_type        *pi_ptr;
  ip_pkt_info_type        *pinfo_ptr = NULL;
#ifdef FEATURE_SEC_IPSEC
  char                     buf[12];
#else
  char                     buf[6];
#endif
  uint16                   bytes_needed  = sizeof(buf);
  ps_ip_protocol_enum_type    next_hdr = PS_NO_NEXT_HDR;
  boolean                  allocated = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(NULL == meta_info_ref_ptr || NULL == (mi_ptr = *meta_info_ref_ptr))
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return -1;
  }

  pi_ptr = &PS_RX_META_GET_PKT_INFO(mi_ptr);
  fraghdl = pi_ptr->fraghdl;

  LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_rx_fragment: fraghdl = %x", 
                            fraghdl);

  /*-------------------------------------------------------------------------
    Locate a fragment handling entry, if already allocated, for this
    fragment.
  -------------------------------------------------------------------------*/
  if (NULL == fraghdl)
  {
    if( NULL == (fraghdl = ip_fraghdl_lookup( pi_ptr )) )
    {
      /*---------------------------------------------------------------------
        No fragment handling entry found, allocate a new one.
      ---------------------------------------------------------------------*/
      if( NULL == (fraghdl = ip_fraghdl_alloc( *pkt_ptr, offset, pi_ptr )) )
      {
       DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
        *ps_errno = E_NO_MEMORY;
        return -1;
      }

      LOG_MSG_DATA_PATH_INFO_1(
           "ps_pkt_info_process_rx_fragment: Allocated fraghdl = %x", fraghdl);

      allocated = TRUE;
      /*---------------------------------------------------------------------
        Must always have a valid meta info in the fraghdl to allow for
        sending icmp errors.
      ---------------------------------------------------------------------*/
      PS_RX_META_INFO_DUP(mi_ptr, &result);
      ip_fraghdl_set_init_meta_info_cache(fraghdl, (void *)result,
                                          IP_FRAGHDL_RX_META_INFO);
    }
  }

  LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_rx_fragment: "
                           "Updating fraghdl = %x in pkt info", fraghdl);
  pi_ptr->fraghdl = fraghdl;

  result = ip_fraghdl_get_cached_rx_meta_info(fraghdl);
  /*-------------------------------------------------------------------------
    If the cksum offload flag in cached meta info is false, we should not
    do the cksum offload for following fragments
  -------------------------------------------------------------------------*/
  if(PS_PKT_INFO_GET_CKSUM_OFFLOAD_FLAG(&PS_RX_META_GET_PKT_INFO(result)))
  {
    /*------------------------------------------------------------------------
      If the passed in meta info is not the cached meta info, this is a 
      new packet. 
    ------------------------------------------------------------------------*/
    if(mi_ptr != result)   
    {
      /*------------------------------------------------------------------------
        If checksum offload flag in passed in meta info is ture, we should 
        adjust the checksum in cached meta info
      ------------------------------------------------------------------------*/
      if( PS_PKT_INFO_GET_CKSUM_OFFLOAD_FLAG(pi_ptr))
      {
        PS_PKT_INFO_SET_CKSUM
          (&PS_RX_META_GET_PKT_INFO(result),
           cksum_add(PS_PKT_INFO_GET_CKSUM(pi_ptr),
                     PS_PKT_INFO_GET_CKSUM(&PS_RX_META_GET_PKT_INFO(result))));
      
        LOG_MSG_DATA_PATH_INFO_1("Combined cksum:%d",
          PS_PKT_INFO_GET_CKSUM(&PS_RX_META_GET_PKT_INFO(result)));
      }
      /*------------------------------------------------------------------------
         If checksum offload flag in passed in meta info is false, we should 
         set the checksum offload flag in cache meta info to false so that 
         we wont do the checksum offload for following fragments
      ------------------------------------------------------------------------*/
      else 
      {
      
        PS_PKT_INFO_SET_CKSUM_OFFLOAD_FLAG(&PS_RX_META_GET_PKT_INFO(result),
                                           FALSE);
      }
    }
  }
  /*-------------------------------------------------------------------------
    Located a fragment handling entry.Check if already have cached meta-info.
  -------------------------------------------------------------------------*/
  if(IP_FRAGHDL_META_INFO_IS_VALID(fraghdl))
  {
    /*-------------------------------------------------------------------
      Copy the meta info and return;
    -------------------------------------------------------------------*/
    PS_RX_META_INFO_FREE(meta_info_ref_ptr);
    PS_RX_META_INFO_DUP(result, meta_info_ref_ptr);

    /*-------------------------------------------------------------------
      If this fragment was enqueued due to new entry allocation, delete it
    -------------------------------------------------------------------*/
    if( TRUE == allocated ) 
    {
      LOG_MSG_ERROR_1("proc_RX_frag: found cached meta-info! Dequeuing frag 0x%x",
                      *pkt_ptr);
      ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );
    }
    else if(IP_V6 == pi_ptr->ip_vsn)
    {
      /* FR 43289: MDM/SDX - IPv6 - RFC2460 Support - Regulator Mandatory Requirement  */
#ifdef FEATURE_DATA_PS_IPV6
      pinfo_ptr = &PS_RX_META_GET_PKT_INFO(*meta_info_ref_ptr);
      pinfo_ptr->ip_hdr.v6.hdr_type = ip_fraghdl_get_xport_proto(fraghdl);
#endif
    }
    return 0;
  }

  /*---------------------------------------------------------------------
    No pkt info  yet.  Put the latest fragment on the fragment
    handling structure.
  ---------------------------------------------------------------------*/
  if (FALSE == allocated)
  {
    if(ip_fraghdl_put( fraghdl, *pkt_ptr, pi_ptr ) != 0)
    {
      LOG_MSG_ERROR_0("Failed to put frag to queue, freeing packet");

      /* Free the packet and set error code */
      dsm_free_packet( pkt_ptr );
      *pkt_ptr = NULL;
      *ps_errno = E_WOULD_BLOCK;
      return -1;
    }
  }

  /*---------------------------------------------------------------------
    Check if we have enough bytes from the start to filter.
  ---------------------------------------------------------------------*/
  if( ip_fraghdl_get_initial_cont_bytes_count( fraghdl ) >= bytes_needed )
  {
    LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_rx_fragment: fraghdl = %x "
                             "get initial byte counts", fraghdl);
    /*-------------------------------------------------------------------
      Extract the bytes
    -------------------------------------------------------------------*/
    if( -1 == ip_fraghdl_get_initial_cont_bytes(fraghdl,
                                                buf,
                                                sizeof(buf),
                                                bytes_needed) )
    {
      LOG_MSG_ERROR_0( "unable to get desired bytes from fraghdl" );
      ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );
      return -1;
    }

    /*-------------------------------------------------------------------
      Delete the passed fragment from the queue.
    -------------------------------------------------------------------*/
    ip_fraghdl_delete_fragment( fraghdl, *pkt_ptr, pi_ptr );

    /*-------------------------------------------------------------------
      Find protocol payload
    -------------------------------------------------------------------*/
    if (IP_V4 == pi_ptr->ip_vsn)
    {
      next_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v4.protocol;
    }
    else
    {
#ifdef FEATURE_DATA_PS_IPV6
      next_hdr = ip_fraghdl_get_xport_proto(fraghdl);
      pi_ptr->ip_hdr.v6.hdr_type = next_hdr;
#endif
    }

    /*-------------------------------------------------------------------
      Use the IP payload bytes to setup information needed by filter
    -------------------------------------------------------------------*/
    switch( next_hdr )
    {
      case PS_IPPROTO_TCP:
        pi_ptr->ptcl_info.tcp.src_port = *((uint16*)&buf[0]);
        pi_ptr->ptcl_info.tcp.dst_port = *((uint16*)&buf[2]);
        break;

      case PS_IPPROTO_UDP:
        pi_ptr->ptcl_info.udp.src_port = *((uint16*)&buf[0]);
        pi_ptr->ptcl_info.udp.dst_port = *((uint16*)&buf[2]);
#ifdef FEATURE_SEC_IPSEC
        /* handle UDP encapsulated ESP packets. */
        if(pi_ptr->ptcl_info.udp.dst_port
           == ps_htons(SECIPSTYPES_GATEWAY_NATT_PORT))
        {
          pi_ptr->esp_hdr.spi =  *((uint32*)&buf[8]);
        }
#endif
        break;

      case PS_IPPROTO_ICMP:
      case PS_IPPROTO_ICMP6:
        pi_ptr->ptcl_info.icmp.type = buf[0];
        pi_ptr->ptcl_info.icmp.code = buf[1];
        if (pi_ptr->ptcl_info.icmp.type == ICMP_ECHO_REPLY ||
            pi_ptr->ptcl_info.icmp.type == ICMP_ECHO_REQ ||
            pi_ptr->ptcl_info.icmp.type == ICMP6_ECHO_REQ ||
            pi_ptr->ptcl_info.icmp.type == ICMP6_ECHO_RSP)
        {
          /* Store the ID */
          pi_ptr->ptcl_info.icmp.id = *((uint16*)&buf[4]);
        }
        break;

      case PS_IPPROTO_ESP:
        pi_ptr->esp_hdr.spi =  *((uint32*)&buf[0]);
        break;

      case PS_IPPROTO_AH:
        pi_ptr->ah_hdr.spi =  *((uint32*)&buf[4]);
        break;

      default:
        /* try filtering only with IP information */
        break;
    }

    /*------------------------------------------------------------------
      Copy the cksum from the cache meta info to the current meta info 
      because the cache meta info will be freed after the current meta
      info is set
    -------------------------------------------------------------------*/
    if(PS_PKT_INFO_GET_CKSUM_OFFLOAD_FLAG(pi_ptr))
    {
      PS_PKT_INFO_SET_CKSUM(&PS_RX_META_GET_PKT_INFO(mi_ptr),
        PS_PKT_INFO_GET_CKSUM(&PS_RX_META_GET_PKT_INFO(result)));
    }
    
    PS_RX_META_INFO_DUP(mi_ptr, &result);
    ip_fraghdl_set_rx_meta_info_cache(fraghdl, (void *)result);
  }
  else
  {
    LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_process_rx_fragment: fraghdl = %x "
                             "initial bytes not available, returning E_WOULD_BLOCK", 
                              fraghdl);
    /*-------------------------------------------------------------------
      Packet needs to be queued.  Free pkt_ptr
    -------------------------------------------------------------------*/
    *pkt_ptr = NULL;
    *ps_errno = E_WOULD_BLOCK;
    return -1;
  }

  return 0;
} /* ps_pkt_info_process_rx_fragment */

/*===========================================================================
FUNCTION PS_TX_IP_PKT_INFO_GENERATE()

DESCRIPTION
  This function processes an IP pakcet and generate IP layer [v4/v6] pkt
  information used for routing and filtering.

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr            - IP packet to parse
  meta_info_ref_ptr  - Pointer to a meta info block.
  next_hdr           - next_hdr after processing IPv4/v6 header
  ip_offset          - Offset into DSM item after reading IP header
  ps_errno           - Error code in case of failure cases

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  TX meta info is generated which contains IP packet information.

NOTE
  This doesnot handle IP fragments.
===========================================================================*/
int ps_tx_ip_pkt_info_generate
(
  dsm_item_type                       **pkt_ref_ptr,
  ps_tx_meta_info_type                **mi_ref_ptr,
  ps_ip_protocol_enum_type             *next_hdr,
  uint16                               *ip_offset,
  errno_enum_type                      *ps_errno
)
{

  dsm_item_type          * pkt_ptr;
  ip_pkt_info_type       * pi_ptr = NULL;   /* IP pkt info being processed */
  ps_tx_meta_info_type   * mi_ptr = NULL;   /* Meta info ptr               */
  uint8                    octet_val;
  ip_version_enum_type     ip_ver;
  ps_ip_protocol_enum_type local_nxt_hdr;
  uint16                   local_ip_off = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Perform argument validation
  -----------------------------------------------------------------------*/
  if (NULL == ps_errno)
  {
    /* DATA_ERR_FATAL("ip_pkt_info_gen: NULL ps_errno");*/
	PS_PKT_INFO_UTILS_ASSERT(0);
    return -1;
  }

  if ((NULL == mi_ref_ptr) || (NULL == pkt_ref_ptr))
  {
    *ps_errno = E_INVALID_ARG;
    /* DATA_ERR_FATAL("ip_pkt_info_gen: NULL arg");*/
	PS_PKT_INFO_UTILS_ASSERT(0);
    return -1;
  }

  pkt_ptr = *pkt_ref_ptr;

  /*---------------------------------------------------------------------
    Get and verify IP version of packet
  ---------------------------------------------------------------------*/
  if (sizeof(uint8) != dsm_extract(pkt_ptr, 0, &octet_val, sizeof(uint8)))
  {
    LOG_MSG_INFO2_0("ip_pkt_info_gen: unable to get IP version");
    *ps_errno = E_BAD_DATA;
    return -1;
  }

  ip_ver = (ip_version_enum_type) (octet_val >> 4);

  if (IP_V4 != ip_ver && IP_V6 != ip_ver)
  {
    LOG_MSG_INFO2_1("ip_pkt_info_gen: Invalid IP version %d", ip_ver);
    *ps_errno = E_VERSION_MISMATCH;
    return -1;
  }

  /*-----------------------------------------------------------------------
    Allocate TX meta info if not already present.
  -----------------------------------------------------------------------*/
  mi_ptr = *mi_ref_ptr;
  if (NULL == mi_ptr)
  {
    PS_TX_META_INFO_AND_RT_META_INFO_GET(*mi_ref_ptr);
    if (NULL == (mi_ptr = *mi_ref_ptr))
    {
      *ps_errno = E_NO_MEMORY;
      DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
      return -1;
    }
  }

  if ( NULL == PS_TX_META_GET_RT_META_INFO_PTR(mi_ptr) )
  {
    *ps_errno = E_NO_MEMORY;
    DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
    PS_TX_META_INFO_FREE(&mi_ptr);
    return -1;
  }

  pi_ptr = &PS_TX_META_GET_PKT_INFO( mi_ptr );

  /*---------------------------------------------------------------------
    Check Packet Info is alreardy populated for this packet. This is
    quite possible for packets coming from Rm interface and going on Um
    Interface [Packet information copied/re-used from RX to TX to avoid
    multiple generations] and in between filters need to be executed.
    This check will make sure we don't end up doing frag queuing again.
  ---------------------------------------------------------------------*/
  if( pi_ptr->is_pkt_info_valid )
  {
    return 0;
  }

  /*---------------------------------------------------------------------
    Now extract the IP header. Copy only, don't remove the header
  ---------------------------------------------------------------------*/
  switch(pi_ptr->ip_vsn = ip_ver)
  {
    case IP_V4:
      if (FALSE ==
           ps_ip4_hdr_parse(pkt_ptr, &local_ip_off, &(pi_ptr->ip_hdr.v4)))
      {
        /* error in extracting IP header */
        *ps_errno = E_BAD_DATA;
        return -1;
      }

      local_nxt_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v4.protocol;

      /*---------------------------------------------------------------------
        If this is an IP fragment filter it.
      ---------------------------------------------------------------------*/
      if(pi_ptr->ip_hdr.v4.mf != 0 || pi_ptr->ip_hdr.v4.offset != 0)
      {
        local_nxt_hdr = PS_IPPROTO_FRAG_HDR;
      }
      break;

    case IP_V6:
#ifdef FEATURE_DATA_PS_IPV6
      if (-1 == ps_pkt_info_process_pkt_ipv6( pkt_ref_ptr,
                                              &pi_ptr,
                                              &local_ip_off,
                                              ps_errno ))
      {
        return -1;
      }

      local_nxt_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v6.hdr_type;
      break;
#endif
    default:
      LOG_MSG_INFO2_1("ip_pkt_info_gen: Invalid IP version %d", ip_ver);
      *ps_errno = E_VERSION_MISMATCH;
      return -1;
  } /* switch(ip_ver) */

  /* Fill outgoing parameters */
  if (NULL != next_hdr)
  {
    *next_hdr = local_nxt_hdr;
  }

  if (NULL != ip_offset)
  {
    *ip_offset = local_ip_off;
  }
  return 0;
}/* ps_tx_ip_pkt_info_generate */

/*===========================================================================
FUNCTION PS_TRANSPORT_PTCL_PKT_INFO_GENERATE()

DESCRIPTION
  This function processes a DSM pakcet and generate Transport Layer 
  [TCP/UDP/ICMP] pkt information used for filtering.
  Protocol - needs to be passed in, to avoid IPv4 / IPv6 check with in this
  function.

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr            - DSM packet to parse
  pkt_info_ptr     - Pointer to a meta info block.
  protocol           - next_hdr after processing IPv4/v6 header
  dsm_offset       - Offset into DSM item to read Transport header
  ps_errno           - Error code in case of failure cases

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  None

NOTE
  Generates ONLY the passed protocol specific packet info is generated.
===========================================================================*/
int ps_transport_ptcl_pkt_info_generate
(
  dsm_item_type                        *pkt_ptr,
  ip_pkt_info_type                     *pkt_info_ptr,
  uint8                                 protocol,
  uint16                                dsm_offset,
  boolean                               is_rx_packet
)
{
  uint8                 buf[16] = {0}; /* scratchpad */
  uint16                buf1=0;

  uint32                pkt_len;
  uint16                tcp_hdr_len;
  uint32                payload_len;
/*-------------------------------------------------------------------------*/

  (void)is_rx_packet;

  /*-----------------------------------------------------------------------
    Perform argument validation
  -----------------------------------------------------------------------*/
  if ((NULL == pkt_info_ptr) || (NULL == pkt_ptr))
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return -1;
  }

  /*---------------------------------------------------------------------
    Extract transport layer protocol information.
  ---------------------------------------------------------------------*/
  switch((ps_ip_protocol_enum_type)protocol)
  {
    case PS_IPPROTO_TCP:
      /* extract src and dst port */
      (void)dsm_extract(pkt_ptr, dsm_offset, buf, 8*sizeof(uint16));
      pkt_info_ptr->ptcl_info.tcp.src_port = *((uint16*)&buf[0]);
      pkt_info_ptr->ptcl_info.tcp.dst_port = *((uint16*)&buf[2]);

      /*---------------------------------------------------------------------
        Extract flags.
      ---------------------------------------------------------------------*/
      /* Check for the TCP_ACK_PRIORITY NV item */
      if( tcp_ack_prio_enabled )
      {
        switch(pkt_info_ptr->ip_vsn)
        {
          case IP_V4: 
            pkt_len = pkt_info_ptr->ip_hdr.v4.length;
            break;
          case IP_V6: 
            pkt_len = dsm_length_packet(pkt_ptr); /* TODO - do in a better way */
            break;
          default:    
            LOG_MSG_INFO2_1("Invalid IP version %d", pkt_info_ptr->ip_vsn);
            return 0;
        }
      
      /*---------------------------------------------------------------------
        Tag & prioritize, if its a TCP regular ACK packet.
      ---------------------------------------------------------------------*/
        tcp_hdr_len = ((buf[TCP_HEADER_LENGTH_FIELD_OFFSET] & 0xf0) >> 4) * 4;
        payload_len = pkt_len - dsm_offset - tcp_hdr_len;
        if ( ( buf[TCP_HEADER_FLAGS_FIELD_OFFSET] & TCP_ACK_BIT_MASK ) &&
             ( payload_len == 0 ) )
        {          
          dsm_item_set_priority(pkt_ptr, DSM_QUEUE_PRIORITY_HIGH);
        }
      }
      break;

    case PS_IPPROTO_UDP:
      /* extract src and dst port */
      (void)dsm_extract(pkt_ptr, dsm_offset, buf, 2*sizeof(uint16));

      pkt_info_ptr->ptcl_info.udp.src_port = *((uint16*)&buf[0]);
      pkt_info_ptr->ptcl_info.udp.dst_port = *((uint16*)&buf[2]);
#ifdef FEATURE_SEC_IPSEC
      /* handle UDP encapsulated ESP packets. */
      if(pkt_info_ptr->ptcl_info.udp.dst_port
         == ps_htons(SECIPSTYPES_GATEWAY_NATT_PORT))
      {
        if(dsm_extract( pkt_ptr,
                        dsm_offset + sizeof(udp_hdr_type),
                        buf,
                        sizeof(uint32) )
           == sizeof(uint32))
        {
          pkt_info_ptr->esp_hdr.spi =  *((uint32*)&buf[0]);
        }
      }
#endif
      break;

    case PS_IPPROTO_ICMP:
    case PS_IPPROTO_ICMP6:
      /* extract type and code fields */
      (void)dsm_extract(pkt_ptr, dsm_offset, buf, 2*sizeof(uint8));
      pkt_info_ptr->ptcl_info.icmp.type = buf[0];
      pkt_info_ptr->ptcl_info.icmp.code = buf[1];
      if(!pkt_info_ptr->ptcl_info.icmp.type)
      {
        (void)dsm_extract(pkt_ptr, dsm_offset+sizeof(uint32), &buf1, sizeof(uint16));
        pkt_info_ptr->ptcl_info.icmp.id = buf1;
      }
      break;

    case PS_IPPROTO_ESP:
      (void)dsm_extract(pkt_ptr, dsm_offset, buf, sizeof(uint32));
      pkt_info_ptr->esp_hdr.spi =  *((uint32*)&buf[0]);
      break;

    case PS_IPPROTO_AH:
      (void)dsm_extract(pkt_ptr,
                        dsm_offset + sizeof(uint32),
                        buf,
                        sizeof(uint32));
      pkt_info_ptr->ah_hdr.spi =  *((uint32*)&buf[0]);
      break;

    default:
      /* try filtering only with IP information. Log a message future 
      expansion of this code*/
    LOG_MSG_ERROR_1("ptcl_pkt_info_generate: Unhandled protocol: %d",
                    protocol);
    break;
  }
  return 0;
} /* ps_transport_ptcl_pkt_info_generate */

/*===========================================================================
FUNCTION PS_TX_PKT_INFO_GENERATE()

DESCRIPTION
  This function processes an IP packet and generate pkt information used
  for routing and filtering.

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr            - IP packet to parse
  meta_info_ref_ptr  - Reference to a an optional ptr to meta info block.
RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  TX meta info is generated which contains packet information and may contain
  filtering results for fragmented packets.

===========================================================================*/
int ps_tx_pkt_info_generate
(
  dsm_item_type                       **pkt_ref_ptr,
  ps_tx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
)
{

  dsm_item_type          * pkt_ptr;
  ip_pkt_info_type       * pi_ptr = NULL;   /* IP pkt info being processed */
  ps_tx_meta_info_type   * mi_ptr = NULL;   /* Meta info ptr               */
  uint16                   dsm_offset;
  ps_ip_protocol_enum_type next_hdr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Perform argument validation
  -----------------------------------------------------------------------*/
  if (NULL == ps_errno)
  {
    /* DATA_ERR_FATAL("pkt_info_generate: NULL ps_errno");*/
	PS_PKT_INFO_UTILS_ASSERT(0);
    return -1;
  }
  
  if ((NULL == mi_ref_ptr) || (NULL == pkt_ref_ptr))
  {
    *ps_errno = E_INVALID_ARG;
    /* DATA_ERR_FATAL("pkt_info_generate: NULL args");*/
	PS_PKT_INFO_UTILS_ASSERT(0);
    return -1;
  }

  LOG_MSG_DATA_PATH_INFO_0("ps_tx_pkt_info_generate: Generating packet info");
  /*---------------------------------------------------------------------
    Generate IP packet information!
  ---------------------------------------------------------------------*/
  if (-1 == ps_tx_ip_pkt_info_generate(pkt_ref_ptr,
                                       mi_ref_ptr,
                                       &next_hdr,
                                       &dsm_offset,
                                       ps_errno))
  {
    LOG_MSG_ERROR_1("Error %d in ip_pkt_info_gen!", *ps_errno);
    return -1;
  }

  mi_ptr = *mi_ref_ptr;

  if (( NULL == mi_ptr) ||
      (NULL == PS_TX_META_GET_RT_META_INFO_PTR(mi_ptr)))
  {
    *ps_errno = E_NO_MEMORY;
    DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
    PS_TX_META_INFO_FREE(&mi_ptr);
    return -1;
  }

  pi_ptr = &PS_TX_META_GET_PKT_INFO( mi_ptr );

  /*---------------------------------------------------------------------
    Check Packet Info is alreardy populated for this packet. This is
    quite possible for packets coming from Rm interface and going on Um
    Interface [Packet information copied/re-used from RX to TX to avoid
    multiple generations] and in between filters need to be executed.
    This check will make sure we don't end up doing frag queuing again.
  ---------------------------------------------------------------------*/
  if( pi_ptr->is_pkt_info_valid )
  {
    return 0;
  }

  pkt_ptr = *pkt_ref_ptr;


  /*-------------------------------------------------------------------
   Process FRAG header processing, if any.
  -------------------------------------------------------------------*/
  if ( next_hdr == PS_IPPROTO_FRAG_HDR )
  {
#ifdef FEATURE_DATA_PS_IPV6
    if (pi_ptr->ip_vsn == IP_V6)
    {
      /*-------------------------------------------------------------------
        Parse the fragment header
      -------------------------------------------------------------------*/
      if( FALSE == ps_ip6_frag_hdr_parse(pkt_ptr, &dsm_offset,
                                         &pi_ptr->ip_hdr.v6) )
      {
        LOG_MSG_ERROR_0("ps_ip6_frag_hdr_parse() returned FALSE");
        *ps_errno = E_FAILURE;
        return -1;
      }
      /*-------------------------------------------------------------------
        Set offset back to the fragment header
      -------------------------------------------------------------------*/
      dsm_offset -= sizeof(ip6_frag_hdr_type);
    }
#endif /* FEATURE_DATA_PS_IPV6 */
    if (pi_ptr->ip_vsn == IP_V4)
    {
      dsm_offset = 0;
    }
    LOG_MSG_DATA_PATH_INFO_0("ps_tx_pkt_info_generate: Processing Tx Fragment");
    if (-1 == ps_pkt_info_process_tx_fragment( pkt_ref_ptr,
                                               mi_ref_ptr,
                                               dsm_offset,
                                               ps_errno ))
    {
      return -1;
    }
    /*-------------------------------------------------------------------
    Frag processing is done, update is_pkt_info_valid to TRUE (at the end) 
    and return.
    -------------------------------------------------------------------*/
  }
  else
  {
    /*---------------------------------------------------------------------
      If no FRAGs, extract "upper layer" transport layer protocol info.
    ---------------------------------------------------------------------*/
    if ( -1 == ps_transport_ptcl_pkt_info_generate( pkt_ptr,
                                                    pi_ptr,
                                                    (uint8)next_hdr,
                                                    dsm_offset,
                                                    FALSE ) )
    {
      DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
     *ps_errno = E_FAILURE;
      return -1;
    }
  }

  /*---------------------------------------------------------------------
    PktInfo generation successful - update is_pkt_info_valid field
  ---------------------------------------------------------------------*/
  pi_ptr->is_pkt_info_valid = TRUE;

  return 0;
}

/*===========================================================================
  FUNCTION PS_PKT_INFO_FILTER_TX_PKT()

DESCRIPTION
  This function processes an IP packet and then executes all the filters
  in an iface installed by the spcecific client.

  The filtering results and information extracted from IP packet is stored in
  the meta info which is passed along with the packet down or up the stack.
  If the packet has an associated meta info, the information already
  available in meta info is used as is and remaining fields are updated.

  If the packet is a fragment, is filterable and has other fragments which
  have been queued earlier then the fraghdl argument will be non-NULL.  The
  caller should call ps_iface_ipfltr_get_queued_fragments() to get the
  queued fragments.  If the fragment cannot be filtered currently it is
  queued and ps_errno is set to E_WOULD_BLOCK.  The passed pkt pointer is set
  to NULL and meta info is freed.  No further action should be taken by the
  caller on the fragment.

DEPENDENCIES
  IP pkt information should be passed inside meta info or a valid IP packet
  should be provided.

PARAMETERS
  iface_ptr   - Iface to pick filters from for execution
  client_id   - Filtering client id
  pkt_ref_ptr - Ptr to IP pkt to process
  mi_ref_ptr  - Reference to a an optional ptr to meta info block
  fraghdl     - Cookie indicating presence of other fragments.
                Retruns non-NULL if fragments present.
  ps_errno       - Error value returned by the function

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error
  In case of presence of fragments the fraghdl argument is set to a
  non-NULL value.  In case of error the error value is placed in ps_errno

SIDE EFFECTS
  Meta info is generated which contains the filtering result and extracted
  pkt information if packet processing is successful.  In case of a fragment
  if filtering cannot be performed because of insufficient information the
  fragment is queued and E_WOULD_BLOCK returned as ps_errno.  If a fragment is
  filterable and there are already queued fragments for the same fragmented
  datagram fraghdl argument is set to non-NULL.

===========================================================================*/
int ps_pkt_info_filter_tx_pkt
(
  ps_iface_type                        *iface_ptr,
  ps_iface_ipfltr_client_id_enum_type   client_id,
  dsm_item_type                       **pkt_ref_ptr,
  ps_tx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
)
{
  ps_tx_meta_info_type *mi_ptr = NULL;       /* Meta info ptr                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( (!PS_IFACE_IS_VALID(iface_ptr)) || (NULL == mi_ref_ptr) )
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, iface_ptr);
    *ps_errno = E_INVALID_ARG;
    return -1;
  }

  LOG_MSG_DATA_PATH_INFO_2("ps_pkt_info_filter_tx_pkt: iface: 0x%x:%d",
                           iface_ptr->name,
                           iface_ptr->instance);

  if (IP_FLTR_CLIENT_INPUT_MAX > client_id ||
      IP_FLTR_CLIENT_MAX <= client_id)
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, client_id);
    *ps_errno = E_INVALID_ARG;
    PS_PKT_INFO_UTILS_ASSERT( 0 );
    return -1;
  }

  /*-------------------------------------------------------------------------
    It is possible that the packet didn't match any installed filters, in
    which case filter mask is set but filter result will be NULL. So checking
    if filter mask is set instead of if filter result is NULL
  -------------------------------------------------------------------------*/

  if (*mi_ref_ptr == NULL ||
      (NULL == PS_TX_META_GET_RT_META_INFO_PTR(*mi_ref_ptr)) ||
      !PS_TX_META_IS_FILTER_MASK_SET(*mi_ref_ptr, client_id))
  {
    LOG_MSG_DATA_PATH_INFO_0("ps_pkt_info_filter_tx_pkt: Generating packet info");
    if( ps_tx_pkt_info_generate(pkt_ref_ptr, mi_ref_ptr, ps_errno) == -1)
    {
      return -1;
    }

    mi_ptr = *mi_ref_ptr;

    LOG_MSG_ERROR_2("ps_pkt_info_filter_tx_pkt(): Setting filter result for QOS for tx_meta_info_ptr = 0x%x"
        "ps_errno is %d",
                    mi_ptr,*ps_errno);

    if ( NULL == mi_ptr )
    {
      DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
      return -1;
    }

    PS_TX_META_SET_FILTER_RESULT
    (
      mi_ptr,
      client_id,
      ps_iface_ipfltr_execute(iface_ptr,
                              client_id,
                              &PS_TX_META_GET_PKT_INFO(mi_ptr))
    );
  }
  
  LOG_MSG_DATA_PATH_INFO_3("ps_pkt_info_filter_tx_pkt: *mi_ref_ptr = 0x%x,"
                           "PS_TX_META_GET_RT_META_INFO_PTR(*mi_ref_ptr) = 0x%x,"
                           "PS_TX_META_IS_FILTER_MASK_SET(*mi_ref_ptr, client_id) = %d",
                           *mi_ref_ptr,
                           *mi_ref_ptr != NULL? PS_TX_META_GET_RT_META_INFO_PTR(*mi_ref_ptr) : NULL,
                           PS_TX_META_IS_FILTER_MASK_SET(*mi_ref_ptr, client_id));

  return 0;
} /* ps_pkt_info_filter_tx_pkt */

/*===========================================================================
  FUNCTION PS_PKT_INFO_FILTER_RX_PKT()

DESCRIPTION
  This function processes an IP packet and then executes all the filters
  in an iface installed by the spcecific client.

  The filtering results and information extracted from IP packet is stored in
  the meta info which is passed along with the packet up the stack.
  If the packet has an associated meta info, the information already
  available in meta info is used as is and remaining fields are updated.

  If the packet is a fragment, is filterable and has other fragments which
  have been queued earlier then the fraghdl argument will be non-NULL.  The
  caller should call ps_iface_ipfltr_get_queued_fragments() to get the
  queued fragments.  If the fragment cannot be filtered currently it is
  queued and ps_errno is set to E_WOULD_BLOCK.  The passed pkt pointer is set
  to NULL and meta info is freed.  No further action should be taken by the
  caller on the fragment.

DEPENDENCIES
  IP pkt information should be passed inside meta info or a valid IP packet
  should be provided.

PARAMETERS
  iface_ptr   - Iface to pick filters from for execution
  client_id   - Filtering client id
  pkt_ref_ptr - Ptr to IP pkt to process
  mi_ref_ptr  - Reference to a an optional ptr to meta info block
  fraghdl     - Cookie indicating presence of other fragments.
                Retruns non-NULL if fragments present.
  ps_errno       - Error value returned by the function

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error
  In case of presence of fragments the fraghdl argument is set to a
  non-NULL value.  In case of error the error value is placed in ps_errno

SIDE EFFECTS
  Meta info is generated which contains the filtering result and extracted
  pkt information if packet processing is successful.  In case of a fragment
  if filtering cannot be performed because of insufficient information the
  fragment is queued and E_WOULD_BLOCK returned as ps_errno.  If a fragment is
  filterable and there are already queued fragments for the same fragmented
  datagram fraghdl argument is set to non-NULL.

===========================================================================*/
int ps_pkt_info_filter_rx_pkt
(
  ps_iface_type                        *iface_ptr,
  ps_iface_ipfltr_client_id_enum_type   client_id,
  dsm_item_type                       **pkt_ref_ptr,
  ps_rx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_IFACE_IS_VALID(iface_ptr) || NULL == mi_ref_ptr)
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, iface_ptr);
    *ps_errno = E_INVALID_ARG;
    return -1;
  }

  if (IP_FLTR_CLIENT_INPUT_MAX <= client_id)
  {
    DATA_PROTO_ERROR_LOG_1(PS_BAD_ARG_OR_NULL_PTR, client_id);
    *ps_errno = E_INVALID_ARG;
    return -1;
  }

  /*-------------------------------------------------------------------------
    It is possible that the packet didn't match any installed filters, in
    which case filter mask is set but filter result will be NULL. So checking
    if filter mask is set instead of if filter result is NULL
  -------------------------------------------------------------------------*/
  if (*mi_ref_ptr == NULL ||
      !PS_RX_META_IS_FILTER_MASK_SET(*mi_ref_ptr, client_id))
  {
    if( ps_rx_pkt_info_generate(pkt_ref_ptr, mi_ref_ptr, ps_errno) == -1)
    {
      return -1;
    }

    return ps_pkt_info_filter_rx_pkt_internal(iface_ptr,
                                              client_id,
                                              *mi_ref_ptr,
                                              TRUE,
                                              TRUE,
                                              ps_errno);
  }

  return 0;
} /* ps_pkt_info_filter_rx_pkt() */

/*===========================================================================
FUNCTION PS_PKT_INFO_FILTER_RX_PKT_INTERNAL()

DESCRIPTION
  This function performs the same action as ps_pkt_info_filter_rx_pkt,
  but it does not generate the packet info and assume it was already generated

  
DEPENDENCIES
  IP pkt information should be passed inside meta info or a valid IP packet
  should be provided. This function will not perform any input validation.
  
PARAMETERS
  iface_ptr   - Iface to pick filters from for execution
  client_id   - Filtering client id  
  mi_ref_ptr  - Reference to a an optional ptr to meta info block
  ps_errno       - Error value returned by the function
  
RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error
  In case of presence of fragments the fraghdl argument is set to a
  non-NULL value.  In case of error the error value is placed in ps_errno

SIDE EFFECTS
  none

===========================================================================*/
int32 ps_pkt_info_filter_rx_pkt_internal
(
  ps_iface_type                        *iface_ptr,
  ps_iface_ipfltr_client_id_enum_type   client_id,
  ps_rx_meta_info_type                 *mi_ptr,
  boolean                               execute_iface_filter,
  boolean                               execute_group_filter,
  errno_enum_type                      *ps_errno
)
{
  ip_pkt_info_type* pkt_info_ptr = &(PS_RX_META_GET_PKT_INFO(mi_ptr));
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    It is possible that the packet didn't match any installed filters, in
    which case filter mask is set but filter result will be NULL. So checking
    if filter mask is set instead of if filter result is NULL
  -------------------------------------------------------------------------*/
  if (!PS_RX_META_IS_FILTER_MASK_SET(mi_ptr, client_id))
  {
    LOG_MSG_DATA_PATH_INFO_0(
                 "ps_pkt_info_filter_rx_pkt_internal: Executing iface filter");

    if (TRUE == execute_iface_filter)
    {
      PS_RX_META_SET_FILTER_RESULT
      (
        mi_ptr,
        client_id,
        ps_iface_ipfltr_execute(iface_ptr,
                                client_id,
                                pkt_info_ptr)
      );
    }

    if (PS_IFACE_IPFLTR_NOMATCH == 
         PS_RX_META_GET_FILTER_RESULT(mi_ptr, client_id))
    {
      LOG_MSG_DATA_PATH_INFO_0(
                 "ps_pkt_info_filter_rx_pkt_internal: Executing group filter");
      if (TRUE == execute_group_filter)
      {
        PS_RX_META_SET_FILTER_RESULT
        (
          mi_ptr,
          client_id,
          ps_iface_ipfltr_execute_by_group( iface_ptr->group_mask,
                                            client_id,
                                            pkt_info_ptr)
        );
      }
#ifdef TEST_FRAMEWORK
      #error code not present
#endif
    }

    /*-----------------------------------------------------------------------
      If a filter was found during fragmented packet processing, save the
      iface information in the fraghdl
    -----------------------------------------------------------------------*/
    if( PS_RX_META_GET_FILTER_RESULT(mi_ptr, IP_FLTR_CLIENT_SOCKETS) !=
        PS_IFACE_IPFLTR_NOMATCH && 
        NULL != pkt_info_ptr->fraghdl)
    {
      LOG_MSG_DATA_PATH_INFO_1("ps_pkt_info_filter_rx_pkt_internal: "
                               "Setting iface in fraghdl = %x", 
                                pkt_info_ptr->fraghdl);
      ps_ip_fraghdl_set_iface(pkt_info_ptr->fraghdl, iface_ptr);
    }    
  }

  return 0;
} /* ps_pkt_info_filter_rx_pkt_internal() */

/*===========================================================================
FUNCTION PS_RX_PKT_INFO_GENERATE()

DESCRIPTION
  This function processes an IP pakcet and generate pkt information used
  for routing and filtering.

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr            - IP packet to parse
  meta_info_ref_ptr  - Reference to a an optional ptr to meta info block.
RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  RX Meta info is generated which contains packet information and may contain
  filtering results for fragmented packets.

===========================================================================*/
int ps_rx_pkt_info_generate
(
  dsm_item_type                       **pkt_ref_ptr,
  ps_rx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
)
{

  dsm_item_type          * pkt_ptr;
  ip_pkt_info_type       * pi_ptr = NULL;   /* IP pkt info being processed */
  ps_rx_meta_info_type   * mi_ptr = NULL;   /* RX meta info ptr            */
  uint16                   dsm_offset;
  uint8                    octet_val;
  ip_version_enum_type     ip_ver;
  ps_ip_protocol_enum_type next_hdr;
  uint16 redundant_cksum = 0;
  uint16 redundant_len = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mi_ptr = *mi_ref_ptr;
  pkt_ptr = *pkt_ref_ptr;

  /*---------------------------------------------------------------------
    Get and verify IP version of packet
  ---------------------------------------------------------------------*/
  if (sizeof(uint8) != dsm_extract(pkt_ptr, 0, &octet_val, sizeof(uint8)))
  {
    LOG_MSG_INFO2_0("ipfltr_process_pkt: unable to get IP version");
    *ps_errno = E_BAD_DATA;
    return -1;
  }
  ip_ver = (ip_version_enum_type) (octet_val >> 4);

  if (IP_V4 != ip_ver && IP_V6 != ip_ver)
  {
    LOG_MSG_INFO2_1("ipfltr_process_pkt: Invalid IP version %d", ip_ver);
    *ps_errno = E_VERSION_MISMATCH;
    return -1;
  }

  /*-----------------------------------------------------------------------
    Allocate meta info and pkt info if not already present.
  -----------------------------------------------------------------------*/
  if (NULL == mi_ptr)
  {
    PS_RX_META_INFO_GET(*mi_ref_ptr);
    if (NULL == (mi_ptr = *mi_ref_ptr))
    {
      *ps_errno = E_NO_MEMORY;
      DATA_PROTO_ERROR_LOG_0(PS_MEM_ALLOC_ERR);
      return -1;
    }
    LOG_MSG_DATA_PATH_INFO_0("ps_rx_pkt_info_generate: Allocated rx meta info");
  }

  pi_ptr     = &(PS_RX_META_GET_PKT_INFO(mi_ptr));
  dsm_offset = 0;

  /*-------------------------------------------------------------------------
    Now extract the IP header. Copy only, don't remove the header
  -------------------------------------------------------------------------*/
  switch(pi_ptr->ip_vsn = ip_ver)
  {
    case IP_V4:
    {
      LOG_MSG_DATA_PATH_INFO_0("ps_rx_pkt_info_generate: Validating ipv4 packet");
      if(ps_pkt_info_validate_ipv4(pkt_ptr) < 0)
      {
        /* invalid ipv4 packet */
        *ps_errno = E_BAD_DATA;
        return -1;
      }

      LOG_MSG_DATA_PATH_INFO_0("ps_rx_pkt_info_generate: Parsing ipv4 packet");
      if (FALSE ==
           ps_ip4_hdr_parse(pkt_ptr, &dsm_offset, &(pi_ptr->ip_hdr.v4)))
      {
        /* error in extracting IP header */
        *ps_errno = E_BAD_DATA;
        return -1;
      }

      /*---------------------------------------------------------------------
        If this is an IP fragment filter it.
      ---------------------------------------------------------------------*/
      if(pi_ptr->ip_hdr.v4.mf != 0 || pi_ptr->ip_hdr.v4.offset != 0)
      {
        LOG_MSG_DATA_PATH_INFO_0(
                    "ps_rx_pkt_info_generate: Setting Next hdr as frag hdr");
        next_hdr = PS_IPPROTO_FRAG_HDR;
      }
      else
      {
        next_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v4.protocol;
        LOG_MSG_DATA_PATH_INFO_1(
                  "ps_rx_pkt_info_generate: Setting Next hdr as %d", next_hdr);
      }

      break;
    }

#ifdef FEATURE_DATA_PS_IPV6
    case IP_V6:
    {
      if (-1 == ps_pkt_info_process_pkt_ipv6( pkt_ref_ptr,
                                              &pi_ptr,
                                              &dsm_offset,
                                              ps_errno ))
      {
        return -1;
      }

      next_hdr = (ps_ip_protocol_enum_type) pi_ptr->ip_hdr.v6.hdr_type;
      break;
    }
#endif

    default:
    {
      LOG_MSG_INFO2_1("ps_rx_pkt_info_generate(): Invalid IP vsn %d", ip_ver);
      *ps_errno = E_VERSION_MISMATCH;
      return -1;
    }
  } /* switch(ip_ver) */

  /*-------------------------------------------------------------------------
    Adjust the cksum if cksum calculation is offloaded 
  -------------------------------------------------------------------------*/
  if(PS_PKT_INFO_GET_CKSUM_OFFLOAD_FLAG(pi_ptr))
  {
    /*------------------------------------------------------------------
      +----------+-------------+--------------------------------------+
      |    IP header           |           payload                    | 
      +----------+-------------+--------------------------------------+
      |ipa_offset|redundant_len|
      |      dsm_offset        |
      
       redundant_len = dsm_offset - ipa_offset
       transport_cksum = ipa_cksum - redundant_cksum
    -------------------------------------------------------------------*/
    
    /*-------------------------------------------------------------------
     Adjust the offset
    -------------------------------------------------------------------*/
    PS_PKT_INFO_UTILS_ASSERT(dsm_offset >= PS_PKT_INFO_GET_CKSUM_OFFSET(pi_ptr));
    redundant_len = dsm_offset - PS_PKT_INFO_GET_CKSUM_OFFSET(pi_ptr);

    if(pi_ptr->ip_vsn == IP_V6 && next_hdr == PS_IPPROTO_FRAG_HDR)
    {
      redundant_len +=  sizeof(ip6_frag_hdr_type);
    }
    
    /*---------------------------------------------------------------------
      Calculate the redundant checksum
    ----------------------------------------------------------------------*/  
    redundant_cksum = cksum( NULL, 
                             pkt_ptr, 
                             redundant_len, 
                             PS_PKT_INFO_GET_CKSUM_OFFSET(pi_ptr));

    /*---------------------------------------------------------------------
      Set the adjusted cksum into pkt info
    ----------------------------------------------------------------------*/ 
    PS_PKT_INFO_SET_CKSUM( pi_ptr,
                           cksum_sub(PS_PKT_INFO_GET_CKSUM(pi_ptr),
                           redundant_cksum));
    
    LOG_MSG_DATA_PATH_INFO_4("redundant cksum: 0x%x, redundant_len:%d"
                             "transport cksum: 0x%x, dsm_offset:%d",
                             redundant_cksum,redundant_len,
                             PS_PKT_INFO_GET_CKSUM(pi_ptr),dsm_offset);
  }

  /*-------------------------------------------------------------------------
   Process FRAG header processing, if any.
  -------------------------------------------------------------------------*/
  if ( next_hdr == PS_IPPROTO_FRAG_HDR )
  {
    if (pi_ptr->ip_vsn == IP_V4)
    {
      dsm_offset = 0;
      LOG_MSG_DATA_PATH_INFO_0(
            "ps_rx_pkt_info_generate: resetting dsm offset for ipv6 fragment");

    }
#ifdef FEATURE_DATA_PS_IPV6
    else
    {
      /*---------------------------------------------------------------------
        Parse the fragment header
      ---------------------------------------------------------------------*/
      if (FALSE == ps_ip6_frag_hdr_parse(pkt_ptr,
                                         &dsm_offset,
                                          &pi_ptr->ip_hdr.v6) )
      {
        LOG_MSG_ERROR_0("ps_ip6_frag_hdr_parse() returned FALSE");
        *ps_errno = E_FAILURE;
        return -1;
      }

      /*---------------------------------------------------------------------
        Set offset back to the fragment header
      ---------------------------------------------------------------------*/
      dsm_offset -= sizeof(ip6_frag_hdr_type);
    }
#endif /* FEATURE_DATA_PS_IPV6 */

    LOG_MSG_DATA_PATH_INFO_0("ps_rx_pkt_info_generate: Processing rx fragment");

    if (-1 == ps_pkt_info_process_rx_fragment( pkt_ref_ptr,
                                                mi_ref_ptr,
                                                dsm_offset,
                                                ps_errno ))
    {
      return -1;
    }
  }
  else
  {
    LOG_MSG_DATA_PATH_INFO_0(
        "ps_rx_pkt_info_generate: Adding transport layer info in packet info");
    /*-----------------------------------------------------------------------
      If no FRAGs, extract "upper layer" transport layer protocol info.
    -----------------------------------------------------------------------*/
    if ( -1 == ps_transport_ptcl_pkt_info_generate( pkt_ptr,
                                                    pi_ptr,
                                                    (uint8)next_hdr,
                                                    dsm_offset,
                                                    TRUE) )
    {
      DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
      *ps_errno = E_FAILURE;
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    PktInfo generation successful - update is_pkt_info_valid field
  -------------------------------------------------------------------------*/
  pi_ptr->is_pkt_info_valid = TRUE;

  return 0;
} /* ps_rx_pkt_info_generate() */

/*===========================================================================

FUNCTION     PS_PKT_INFO_CHECK_TCP_ACK

DESCRIPTION  
  This function is used to find out if the packet is a regular
  tcp ack packet or a piggy back ack packet.

DEPENDENCIES 
  None

PARAMETERS   
  pkt_ptr          - DSM item containing the ip packet.
  ps_errno         - Error code in case of failure cases.

RETURN VALUE
 TCP_ACK_REG       - If regular TCP ACK  
 TCP_ACK_PIGGYBACK - Piggybacked TCP ACK
 TCP_ACK_NONE      - Not a TCP &/ ACK packet
 

SIDE EFFECTS 
 None
===========================================================================*/
tcp_ack_type ps_pkt_info_check_tcp_ack
(
  dsm_item_type    *pkt_ptr,
  errno_enum_type  *ps_errno
)
{
  uint8                    extract_buf[2] = {0};
  ip_version_enum_type     ip_ver;
  struct ip                hdr;
  uint16                   offset;
  ps_ip_protocol_enum_type local_nxt_hdr;
  uint16                   pkt_len;
  uint16                   tcp_hdr_len;
  uint32                   payload_len;
  
  /*---------------------------------------------------------------------
    Sanity checks!
  ---------------------------------------------------------------------*/  
  if ( NULL == ps_errno  || NULL == pkt_ptr )
  {
    DATA_PROTO_ERROR_LOG_0(PS_BAD_ARG_OR_NULL_PTR);
    return TCP_ACK_NONE;
  }
  
  /*---------------------------------------------------------------------
    Get and verify IP version of the packet
    - TODO - Combine all dsm_extracts for future optimzation!
  ---------------------------------------------------------------------*/
  if (sizeof(uint8) != dsm_extract(pkt_ptr, 0, &extract_buf, sizeof(uint8)))
  {
    LOG_MSG_INFO2_0("ps_pkt_info_check_tcp_ack: unable to get IP version");
    *ps_errno = E_BAD_DATA;
    return TCP_ACK_NONE;
  }
 
  ip_ver = (ip_version_enum_type) (extract_buf[0] >> 4);
  
  switch (ip_ver)
  {
    case IP_V4:
      /*-------------------------------------------------------------------
        Extract IP header
      -------------------------------------------------------------------*/
      offset = 0;
      if( FALSE == ps_ip4_hdr_parse(pkt_ptr, &offset, &hdr) )
      {
        LOG_MSG_ERROR_0("Error parsing IP header.");
        return TCP_ACK_NONE;
      }

      /*-------------------------------------------------------------------
        Return, if its a IP fragment!
      -------------------------------------------------------------------*/
      if(hdr.mf != 0 || hdr.off != 0)
      {
        return TCP_ACK_NONE;
      }
    
      /*-------------------------------------------------------------------
        Get IP header length, Total Packet length, Next header
      -------------------------------------------------------------------*/
      offset = (uint16)(hdr.ihl * 4);
      pkt_len = hdr.length;
      local_nxt_hdr = (ps_ip_protocol_enum_type) hdr.protocol;
      break;
 
    case IP_V6:
      (void)dsm_extract(pkt_ptr,IPV6_HEADER_NEXT_HEADER_FIELD_OFFSET,
                        extract_buf, sizeof(uint8));
      local_nxt_hdr = (ps_ip_protocol_enum_type)extract_buf[0];
      offset = IP6LEN;
      pkt_len = (uint16)dsm_length_packet(pkt_ptr);
      /*---------------------------------------------------------------------
        Process all the extension headers until an upper layer header or the
        header NO_NEXT_HEADER is found.
      ---------------------------------------------------------------------*/
      while (local_nxt_hdr != PS_IPPROTO_TCP &&
             local_nxt_hdr != PS_IPPROTO_UDP &&
             local_nxt_hdr != PS_IPPROTO_FRAG_HDR &&
             local_nxt_hdr != PS_IPPROTO_AH &&
             local_nxt_hdr != PS_IPPROTO_ESP &&
             local_nxt_hdr != PS_IPPROTO_ICMP6)
     {
	   LOG_MSG_INFO2_1("Handle %d header in filtering",
                           local_nxt_hdr);
       switch( local_nxt_hdr )
       {
         case PS_IPPROTO_ROUTING_HDR:
           if (0 == dsm_extract(pkt_ptr, offset,
                                extract_buf, 
                                sizeof(uint16)))
           {
             return TCP_ACK_NONE;
           }
           offset += sizeof(ip6_routing_hdr_type) + (extract_buf[1]<<3);
           local_nxt_hdr = (ps_ip_protocol_enum_type)extract_buf[0];
           break;

         case PS_IPPROTO_HOP_BY_HOP_OPT_HDR:
           if (0 == dsm_extract(pkt_ptr, offset,
                                extract_buf, 
                                sizeof(uint16)))
           {
             return TCP_ACK_NONE;
           }
           offset += sizeof(ip6_hopbyhop_hdr_type) + (extract_buf[1]<<3);
           local_nxt_hdr = (ps_ip_protocol_enum_type)extract_buf[0];
           break;

         default:
           return TCP_ACK_NONE;
       }
     } /* while */
     break;
    
   default:
     LOG_MSG_ERROR_1("ps_pkt_info_check_tcp_ack: Invalid IP version %d",
                     ip_ver);
     *ps_errno = E_VERSION_MISMATCH;
     return TCP_ACK_NONE;
  }

  /*------------------------------------------------------------------
    Tag and prioritize TCP ACK packet!
  -------------------------------------------------------------------*/
  if ( PS_IPPROTO_TCP != local_nxt_hdr )
  {
    return TCP_ACK_NONE;
  }

  /*------------------------------------------------------------------
    Extract, TCP header lenght, flags fields.
  -------------------------------------------------------------------*/
  (void)dsm_extract(pkt_ptr, 
                    offset + TCP_HEADER_LENGTH_FIELD_OFFSET,
                    extract_buf,
                    sizeof(uint16));
  
  tcp_hdr_len = ((extract_buf[0] & 0xf0) >> 4) * 4;
  payload_len = pkt_len - offset - tcp_hdr_len;

  /*------------------------------------------------------------------
    Check if its TCP ACK
  -------------------------------------------------------------------*/
  if ( (extract_buf[1] &  TCP_ACK_BIT_MASK) == 0 )
  {
    return TCP_ACK_NONE;
  }
  else if ( payload_len == 0 )
  {  
    return TCP_ACK_REG;
  }
  else
  {
    return TCP_ACK_PIGGYBACK;
  }
} /* ps_pkt_info_check_tcp_ack */

/*===========================================================================
FUNCTION      PS_PKT_INFO_SET_TCP_ACK_PRIO_ENABLED

DESCRIPTION   This function sets the tcp_ack_prio_enabled variable to the 
              specified value.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ps_pkt_info_set_tcp_ack_prio_enabled(uint8 is_enabled)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  tcp_ack_prio_enabled   = is_enabled; 
 
  LOG_MSG_INFO1_1("Set tcp_ack_prio_enabled =%d", tcp_ack_prio_enabled);
}

/*===========================================================================
FUNCTION      PS_PKT_INFO_GET_TCP_ACK_PRIO_ENABLED

DESCRIPTION   This function returns the value of tcp_ack_prio_enabled variable

DEPENDENCIES  None.

RETURN VALUE  Value of tcp_ack_prio_enabled variable

SIDE EFFECTS  None.
===========================================================================*/
uint8 ps_pkt_info_get_tcp_ack_prio_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  return tcp_ack_prio_enabled;
}

/*===========================================================================
FUNCTION PS_IP_UPDATE_TRANSPORT_LAYER_DATA()

DESCRIPTION
  This function updates transport layer meta info
  to be used following IPsec descryption/authentication.
  This function will not perform validation on the input.

DEPENDENCIES
  None

PARAMETERS
  pkt_info_ptr - meta info for the packet
  bp           - the DSM item (the packet)
  offset       - offset to the transport header
  protocol     - transport layer protocol

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void
ps_ip_update_transport_layer_data
(
  ip_pkt_info_type       *pkt_info_ptr,
  dsm_item_type          *bp,
  uint16                 offset,
  uint8                  protocol
)
{
  uint8                  buf[4];

  /*-----------------------------------------------------------------------
    Update packet info with the protocol specific paramaters like ports
  -----------------------------------------------------------------------*/
  switch (protocol)
  {
    case PS_IPPROTO_TCP:
      /* extract src and dst port */
      (void)dsm_extract(bp, offset, buf, 2*sizeof(uint16));

      pkt_info_ptr->ptcl_info.tcp.src_port = *((uint16*)&buf[0]);
      pkt_info_ptr->ptcl_info.tcp.dst_port = *((uint16*)&buf[2]);
      break;

    case PS_IPPROTO_UDP:
      /* extract src and dst port */
      (void)dsm_extract(bp, offset, buf, 2*sizeof(uint16));

      pkt_info_ptr->ptcl_info.udp.src_port = *((uint16*)&buf[0]);
      pkt_info_ptr->ptcl_info.udp.dst_port = *((uint16*)&buf[2]);
      break;

    case PS_IPPROTO_ICMP:
      /* extract type and code fields */
      (void)dsm_extract(bp, offset, buf, 2*sizeof(uint8));

      pkt_info_ptr->ptcl_info.icmp.type = buf[0];
      pkt_info_ptr->ptcl_info.icmp.code = buf[1];
      break;

    default:
      /* filter with IP packet information*/
      break;
  }
} /*ps_ip_update_transport_layer_data*/


