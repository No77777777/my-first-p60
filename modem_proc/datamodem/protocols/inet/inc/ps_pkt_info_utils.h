#ifndef PS_PKT_INFO_UTILS_H
#define PS_PKT_INFO_UTILS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        P S _ P K T _ I N F O _ U T I L S. H

DESCRIPTION
  Header containing packet information which needs to be passed through 
  layers.

Copyright (c) 2004-2014 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/inc/ps_pkt_info_utils.h#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/11/14    cx     Added support for checksum offload
11/09/14    yl     IP data path re-architecture
09/20/12    dvk    Made changes for accessing tcp_ack_prio_enabled global 
                   variable from a accessor function.
09/14/11    sm     Added TCP ACK PRIO NV item check for dynamic enablement
                   of ACK tagging.
08/11/11    sm     API ps_pkt_info_parse_packet added to do explicit parsing.
10/18/10    pp     API ps_transport_ptcl_pkt_info_generate added.
01/25/05    sv     Created the file.
===========================================================================*/

#include "comdef.h"
#include "customer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ps_tx_meta_info.h"
#include "IxErrno.h"
/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/

typedef enum
{
  TCP_ACK_NONE = 0,
  TCP_ACK_REG,
  TCP_ACK_PIGGYBACK,
  TCP_ACK_MAX = 0xFF
}tcp_ack_type;

/*---------------------------------------------------------------------------
  Set the cksum value to packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_SET_CKSUM(pkt_info_ptr, cksum) \
  ((pkt_info_ptr)->cksum_offload_info.cksum_value = (cksum))

/*---------------------------------------------------------------------------
  Get the cksum value from packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_GET_CKSUM(pkt_info_ptr) \
  ((pkt_info_ptr)->cksum_offload_info.cksum_value)

/*---------------------------------------------------------------------------
  Set the cksum offload flag to packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_SET_CKSUM_OFFLOAD_FLAG(pkt_info_ptr, flag) \
  ((pkt_info_ptr)->cksum_offload_info.is_cksum_computed = (flag))

/*---------------------------------------------------------------------------
  Get the cksum offload flag from packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_GET_CKSUM_OFFLOAD_FLAG(pkt_info_ptr) \
  ((pkt_info_ptr)->cksum_offload_info.is_cksum_computed)

/*---------------------------------------------------------------------------
  Set the flag for checksum is computed over pseudo header
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_SET_IS_PSEUDO_HEADER_CKSUM_COMPUTED(pkt_info_ptr, flag) \
  ((pkt_info_ptr)->cksum_offload_info.is_pseudo_header_cksum_computed = (flag))

/*---------------------------------------------------------------------------
  Get the flag for checksum is computed over pseudo header
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_GET_IS_PSEUDO_HEADER_CKSUM_COMPUTED(pkt_info_ptr) \
  ((pkt_info_ptr)->cksum_offload_info.is_pseudo_header_cksum_computed)

/*---------------------------------------------------------------------------
  Set the cksum offset to packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_SET_CKSUM_OFFSET(pkt_info_ptr, offset) \
   ((pkt_info_ptr)->cksum_offload_info.cksum_start_offset = (offset))

/*---------------------------------------------------------------------------
  Get the cksum offset to packet info pointer
---------------------------------------------------------------------------*/
#define PS_PKT_INFO_GET_CKSUM_OFFSET(pkt_info_ptr) \
   ((pkt_info_ptr)->cksum_offload_info.cksum_start_offset)

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
  This does not handle fragment queue-ing.
===========================================================================*/
int ps_tx_ip_pkt_info_generate
(
  dsm_item_type                       **pkt_ref_ptr,
  ps_tx_meta_info_type                **mi_ref_ptr,
  ps_ip_protocol_enum_type             *next_hdr,
  uint16                               *ip_offset,
  errno_enum_type                      *ps_errno
);

/*===========================================================================
FUNCTION PS_TRANSPORT_PTCL_PKT_INFO_GENERATE()

DESCRIPTION
  This function processes a DSM pakcet and generate Transport Layer [TCP/UDP/ICMP] pkt 
  information used for filtering.
  protocol - needs to be passed in, to avoid IPv4 / IPv6 check with in this function.

DEPENDENCIES
  None.

PARAMETERS
  pkt_ptr          - DSM packet to parse
  pkt_info_ptr     - Pointer to a meta info block.
  protocol         - next_hdr after processing IPv4/v6 header
  dsm_offset       - Offset into DSM item to read Transport header
  is_rx_packet     - Is this a tx or rx packet.

RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error

SIDE EFFECTS
  None

NOTE
  Generates ONLY the passed protocol specific packet info is generated.
===========================================================================*/
int ps_transport_ptcl_pkt_info_generate
(
  dsm_item_type                        *pkt_ref_ptr,
  ip_pkt_info_type                     *pkt_info_ptr,
  uint8                                 protocol,
  uint16                                dsm_offset,
  boolean                               is_rx_packet
);

/*===========================================================================
FUNCTION PS_TX_PKT_INFO_GENERATE()

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
  Meta info is generated which contains packet information and may contain
  filtering results for fragmented packets.

===========================================================================*/

extern int ps_tx_pkt_info_generate
(
  dsm_item_type                       **pkt_ptr,
  ps_tx_meta_info_type                **meta_info_ref_ptr,
  errno_enum_type                      *ps_errno
);

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
extern int ps_rx_pkt_info_generate
(
  dsm_item_type                       **pkt_ptr,
  ps_rx_meta_info_type                **meta_info_ref_ptr,
  errno_enum_type                      *ps_errno
);

/*===========================================================================
  FUNCTION PS_PKT_INFO_FILTER_TX_PKT()

DESCRIPTION
  This function processes an IP packet and then executes all the filters 
  in an iface installed by the spcecific client.

  The filtering results and information extracted from IP packet is stored in
  the meta info which is passed along with the packet down the stack. 
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
extern int ps_pkt_info_filter_tx_pkt
( 
  ps_iface_type                        *iface_ptr,  
  ps_iface_ipfltr_client_id_enum_type   client_id,
  dsm_item_type                       **pkt_ref_ptr,
  ps_tx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
);

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
extern int ps_pkt_info_filter_rx_pkt
( 
  ps_iface_type                        *iface_ptr,  
  ps_iface_ipfltr_client_id_enum_type   client_id,
  dsm_item_type                       **pkt_ref_ptr,
  ps_rx_meta_info_type                **mi_ref_ptr,
  errno_enum_type                      *ps_errno
);

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
  execute_iface_filter : To execute iface filter or not
  execute_group_filter : To execute group filter or not
  ps_errno       - Error value returned by the function
  
RETURN VALUE
  0 for successful processing of the pkt, -1 in case of error
  In case of presence of fragments the fraghdl argument is set to a
  non-NULL value.  In case of error the error value is placed in ps_errno

SIDE EFFECTS
  none

===========================================================================*/
extern int32 ps_pkt_info_filter_rx_pkt_internal
( 
  ps_iface_type                        *iface_ptr,  
  ps_iface_ipfltr_client_id_enum_type   client_id,
  ps_rx_meta_info_type                 *mi_ref_ptr,
  boolean                               execute_iface_filter,
  boolean                               execute_group_filter,
  errno_enum_type                      *ps_errno
);

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
);

/*===========================================================================
FUNCTION      PS_PKT_INFO_SET_TCP_ACK_PRIO_ENABLED

DESCRIPTION   This function sets the tcp_ack_prio_enabled variable to the 
              specified value.

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ps_pkt_info_set_tcp_ack_prio_enabled(uint8 is_enabled);

/*===========================================================================
FUNCTION      PS_PKT_INFO_GET_TCP_ACK_PRIO_ENABLED

DESCRIPTION   This function returns the value of tcp_ack_prio_enabled variable

DEPENDENCIES  None.

RETURN VALUE  Value of tcp_ack_prio_enabled variable

SIDE EFFECTS  None.
===========================================================================*/
uint8 ps_pkt_info_get_tcp_ack_prio_enabled( void );

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
);

#ifdef __cplusplus
}
#endif
#endif
