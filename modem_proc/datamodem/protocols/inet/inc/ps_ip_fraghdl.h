#ifndef PS_IP_FRAGHDL_H
#define PS_IP_FRAGHDL_H
/*===========================================================================

                      P S _ I P _ F R A G H D L . H

DESCRIPTION
  Definitions and routines for the IP fragment handling layer

Copyright (c) 2004-2019 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/protocols/inet/inc/ps_ip_fraghdl.h#2 $
  $DateTime: 2019/10/22 02:53:17 $
  $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/09/19    rj     Added support for QSH MDUMP collection
12/23/14    ss     Add limit on maximum entries in frag queue
11/30/14    cx     Added support for checksum offload
11/21/14    ds     Added deinit API for ip frag in case of off target 
                   shutdown.
07/08/13    cx     Replace memcpy and memmove with memscpy and memsmove
02/25/13    op     Added function to set/get iface from fraghdl
07/27/09    sp     Fixed IP Fragmentation issues.
10/14/08    pp     Metainfo fixes, re-arranged frag API.
03/13/08    pp     Metainfo optimizations.
10/30/06    hm     Updated the value of FRAGHDL_TIMEOUT if FEATURE_JCDMA_2 
                   is defined.
08/17/06    mct    Added function to init meta info in fraghdls for sending
                   icmp errors.
05/11/05    sv     Lint changes.
04/20/05    sv     Modified user data to meat info pointer.
03/25/05    ifk    Added support to read v6 transport proto information from
                   fragment.
03/21/05    ssh    Changes pursuant to the new typedef ps_timer_handle_type
03/02/05    vp     Addition of iface_ptr in fraghdl structure.
02/02/05    ifk    Changed V4 timeout to 60 seconds as well.
01/10/05    ifk    Added user_data_valid field, get_initial_cont_bytes()
                   function.  Modified get_user_data, renamed
                   get_cont_start_bytes() to get_initial_cont_byte_count()
10/14/04    ifk    Added module
===========================================================================*/
/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"       /* Customer Specific Features */
#include "comdef.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif /* FEATURE_QSH_MDUMP */

#ifdef __cplusplus
extern "C"
{
#endif

#include "ps_ip4_hdr.h"
#include "ps_tx_meta_info.h"
#include "pstimer.h"

/*===========================================================================

                            DEFINITIONS AND TYPES

===========================================================================*/
/*---------------------------------------------------------------------------
  The number of elements in the reassembly structure. This value gives the
  total number of fragmented-datagrams that can be handled at a given time.
---------------------------------------------------------------------------*/
#define NUM_FRAGHDL    20 

/*-----------------------------------------------------------------------
  Maximum number of fragments one fragment handle can hold, i.e. for one
  IP Packet.
  Such limit has been placed to ensure that no rogue peer can cause DSM 
  exhaustion on the device just by sending a large number of fragments 
  while skipping the first fragment.
-------------------------------------------------------------------------*/
#define IP_FRAG_MAX_QUEUE_CAPACITY 50

/*---------------------------------------------------------------------------
  Fraghdl timer = 60 seconds for v6 and 60 sec for v4, time within which
  all the fragments of an IP packet must be receievd otherwise the packet
  is discarded. For FEATURE_JCDMA_2, both timers are set to 15 seconds.
---------------------------------------------------------------------------*/
#ifdef FEATURE_JCDMA_2
  #define IP4_FRAGHDL_TIMEOUT        15000
  #define IP6_FRAGHDL_TIMEOUT        15000
#else
  #define IP4_FRAGHDL_TIMEOUT        60000
  #define IP6_FRAGHDL_TIMEOUT        60000
#endif

/*---------------------------------------------------------------------------
  ip_fraghdl_meta_info_kind_enum_type is used to identify what kind of meta
  info is stored in frag handle.
---------------------------------------------------------------------------*/
typedef enum
{
  IP_FRAGHDL_INVALID_META_INFO = 0,
  IP_FRAGHDL_TX_META_INFO,
  IP_FRAGHDL_RX_META_INFO,
  IP_FRAFHDL_MAX = 0xff /* forcing it to 1 byte */
}ip_fraghdl_meta_info_kind_enum_type;

/*---------------------------------------------------------------------------
  ip_fraghdl_bridge_type_enum_type is used to identify what kind of bridging
  is used, to send out the fragments.
---------------------------------------------------------------------------*/
typedef enum
{
  IP_FRAGHDL_BRIDGE_TYPE_NONE,
  IP_FRAGHDL_BRIDGE_TYPE_IFACE_BRIDGE,
  IP_FRAGHDL_BRIDGE_TYPE_PPP_BRIDGE
}ip_fraghdl_bridge_type_enum_type;

/*---------------------------------------------------------------------------
   Transmit function for sending out the queued fragments.
---------------------------------------------------------------------------*/
typedef int (*ps_ip_fraghdl_tx_cmd_f_ptr_type)
(
  void                 * device_ptr,
  dsm_item_type       ** dsm_item_ptr,
  ps_tx_meta_info_type * meta_info_ptr
);


/*---------------------------------------------------------------------------
  ip_fraghdl_s_type is used to cache fragments belonging to a v4 or v6
  datagram
---------------------------------------------------------------------------*/
typedef struct ip_fraghdl_s_type
{
  ip_version_enum_type  ip_vsn;  /* IP version of the datagram */
  union
  {
    struct ip    v4;
#ifdef FEATURE_DATA_PS_IPV6
    ip6_hdr_type v6;
#endif /* FEATURE_DATA_PS_IPV6 */
  } ip_hdr;                      /* IPv4 header or IPv6 fragment header    */
  void    *meta_info_ptr;        /* Meta info ptr                          */
  ip_fraghdl_meta_info_kind_enum_type
           meta_info_kind;        /* TX / RX which ever is set             */
  boolean  meta_info_valid;      /* Metainfo validity flag               */
  q_type   frag_queue;           /* Queue of fragments                     */
  uint16   datagram_len;         /* Total length of complete datagram      */
  uint16   cont_start_bytes;     /* Contiguous bytes from beginning rcvd   */
  uint16   offset;               /* DSM offset to v4 or v6 frag header     */
  ps_timer_handle_type frag_timer;
                                 /* Timer for the fragment queue           */
  boolean  in_use;               /* Is the structure in use?               */
  ps_ip_protocol_enum_type
           xport_proto;          /* V6 transport layer protocol            */
  boolean xport_proto_info_valid;/* Is transport layer info valid?         */
#ifdef FEATURE_DATA_PS_IPV6
#ifdef FEATURE_SEC_IPSEC
  char xport_proto_info[12];      /* Transport layer info needed by filter  */
#else
  char xport_proto_info[6];       /* Transport layer info needed by filter  */
#endif /* FEATURE_SEC_IPSEC */
#endif /* FEATURE_DATA_PS_IPV6 */
  void *iface_ptr;               /* Iface ptr, using which ICMP error messages
                                    will be sent in V6 embedded calls */
  void                             *device_ptr; /* Iface or PPP Device info */
  ip_fraghdl_bridge_type_enum_type  device_type; /* Type of device_ptr */
  ps_ip_fraghdl_tx_cmd_f_ptr_type   frag_tx_cmd_f_ptr; /* Transmit function */
  boolean  is_local;              /* Bool to determine if the frag is meant
                     embedded stack */
  uint32 last_rcvd_frag_num;      /* Last received fragment number */
} ip_fraghdl_s_type;

#define IP_FRAGHDL_META_INFO_IS_VALID(fraghdl_ptr) \
  ((ip_fraghdl_s_type *)(fraghdl_ptr))->meta_info_valid == TRUE && \
    ((ip_fraghdl_s_type *)(fraghdl_ptr))->meta_info_kind == IP_FRAGHDL_RX_META_INFO

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================
FUNCTION IP_FRAGHDL_LOOKUP()

DESCRIPTION
  Returns a handle to the IP fragment handling structure corresponding to
  passed fragment header if already allocated or NULL otherwise.
  
DEPENDENCIES
  None

RETURN VALUE
  Handle to allocated IP fragment handling structure corresponding to the
  passed fragment.

SIDE EFFECTS
  None
===========================================================================*/
void *ip_fraghdl_lookup
(
  ip_pkt_info_type *pkt_info
);


/*===========================================================================
FUNCTION IP_FRAGHDL_ALLOC()

DESCRIPTION
  Allocates an IP fraghdl structure for the passed fragment header.
  
DEPENDENCIES
  Only one IP header should be passed

RETURN VALUE
  Handle to allocated IP fragment handling structure corresponding to the
  passed fragment.

SIDE EFFECTS
  Allocates a fraghdl structure along with a PS timer which is started
===========================================================================*/
void *ip_fraghdl_alloc
(
  dsm_item_type    *item_ptr,
  uint16            offset,
  ip_pkt_info_type *pkt_info
);


/*===========================================================================
FUNCTION IP_FRAGHDL_PUT()

DESCRIPTION
  This function is called to put the passed fragment onto the fragment
  queue corresponding to passed IP fraghdl handle.
  
DEPENDENCIES
  None

RETURN VALUE
  0 for success, -1 for failure

SIDE EFFECTS
  The passed datagram is placed on the queue associated with passed handle.
===========================================================================*/
int32 ip_fraghdl_put
(
  void             *handle,
  dsm_item_type    *item_ptr,
  ip_pkt_info_type *pkt_info
);


/*===========================================================================
FUNCTION IP_FRAGHDL_DELETE_FRAGMENT()

DESCRIPTION
  Deletes the passed fragment from the fragment queue, updating any relevant
  statistics.
  
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  The passed fragment is removed from the fragment queue.
===========================================================================*/
void ip_fraghdl_delete_fragment
(
  void             *handle,
  dsm_item_type    *item_ptr,
  ip_pkt_info_type *pkt_info
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_FRAGMENT()

DESCRIPTION
  Dequeues and returns the first fragment in the queue.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The passed fragment is removed from the fragment queue.  For overlapping
  fragments cont_start_bytes may be invalid after a get()
===========================================================================*/
dsm_item_type *ip_fraghdl_get_fragment
(
  void *handle
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_FIRST_FRAGMENT()

DESCRIPTION
  This function is called with an IP fraghdl handle and returns the first
  fragment queued in the fraghdl.

  THE FRAGMENT RETURNED SHOULD NOT BE DEQUEUED OR OTHERWISE MOVED IN THE
  QUEUE
  
DEPENDENCIES
  None

RETURN VALUE
  First fragment queued in the IP fraghdl passed.

SIDE EFFECTS
  None
===========================================================================*/
dsm_item_type *ip_fraghdl_get_first_fragment
(
  void *handle
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_NEXT_FRAGMENT()

DESCRIPTION
  This function returns the fragment after the passed fragment in the
  IP fraghdl structure corresponding to the passed handle
  
  THE FRAGMENT RETURNED SHOULD NOT BE DEQUEUED OR OTHERWISE MOVED IN THE
  QUEUE

DEPENDENCIES
  None

RETURN VALUE
  Fragment after the passed fragment

SIDE EFFECTS
  None
===========================================================================*/
dsm_item_type *ip_fraghdl_get_next_fragment
(
  void          *handle,
  dsm_item_type *item_ptr
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_INITIAL_CONT_BYTES()

DESCRIPTION
  This function returns specified number of bytes from the start of the
  fragmented datagram.
  
DEPENDENCIES
  None

RETURN VALUE
  -1 in case of error
   0 in case of success

SIDE EFFECTS
  None
===========================================================================*/
int ip_fraghdl_get_initial_cont_bytes
(
  void          *handle,
  char          *buffer,
  uint16         buf_size,
  uint16         byte_cnt
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_DATAGRAM_LEN()

DESCRIPTION
  Returns the datagram of the datagram being held by the passed IP fraghdl
  structure
  
DEPENDENCIES
  None

RETURN VALUE
  Datagram length of the datagram associated with the passed IP fraghdl
  0 if datagram length is unknown or an error occurs

SIDE EFFECTS
  None
===========================================================================*/
uint16 ip_fraghdl_get_datagram_len
(
  void *handle
);


/*===========================================================================
FUNCTION IP_FRAGHDL_GET_INITIAL_CONT_BYTES_COUNT()

DESCRIPTION
  Called with a handle to an IP fraghdl structure to return the contiguous
  bytes from the start of packet received so far
  
DEPENDENCIES
  None

RETURN VALUE
  Contiguous bytes from the start of packet received

SIDE EFFECTS
  None
===========================================================================*/
uint16 ip_fraghdl_get_initial_cont_bytes_count
(
  void *handle
);

/*===========================================================================
FUNCTION IP_FRAGHDL_SET_INIT_META_INFO_CACHE()

DESCRIPTION
  Called with a handle to an IP fraghdl structure and a the meta info to
  be initialized in the fragment handle structure. This is needed in case
  the first bytes of a packet never arrive. Having meta info will allow the
  icmp error messages to be properly sent.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void ip_fraghdl_set_init_meta_info_cache
(
  void                               * handle,
  void                               * meta_info_ptr,
  ip_fraghdl_meta_info_kind_enum_type  meta_info_kind
);

/*===========================================================================
FUNCTION IP_FRAGHDL_SET_RX_META_INFO_CACHE()

DESCRIPTION
  Called with a handle to an IP fraghdl structure and a value of meta_info_ptr
  to be registered with the passed handle
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the passed meta_info_ptr value in the passed handle
===========================================================================*/
void ip_fraghdl_set_rx_meta_info_cache
(
  void *handle,
  ps_rx_meta_info_type *meta_info_ptr
);

/*===========================================================================
FUNCTION IP_FRAGHDL_SET_TX_META_INFO_CACHE()

DESCRIPTION
  Called with a handle to an IP fraghdl structure and a value of meta_info_ptr
  to be registered with the passed handle
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the passed meta_info_ptr value in the passed handle
===========================================================================*/
void ip_fraghdl_set_tx_meta_info_cache
(
  void *handle,
  ps_tx_meta_info_type *meta_info_ptr
);

/*===========================================================================
FUNCTION IP_FRAGHDL_GET_CACHED_RX_META_INFO()

DESCRIPTION
  Called with a handle to an IP fraghdl to get the meta_info_ptr registered with
  it.
  
DEPENDENCIES
  None

RETURN VALUE
  0 if a meta_info_ref_ptr value is set, -1 otherwise
  Value of meta_info_ptr field corresponding to the passed IP fraghdl handle
  returned in meta_info_ref_ptr argument if return value is 0

SIDE EFFECTS
  None
===========================================================================*/
ps_rx_meta_info_type * ip_fraghdl_get_cached_rx_meta_info
(
  void  *handle
);

/*===========================================================================
FUNCTION IP_FRAGHDL_GET_CACHED_TX_META_INFO()

DESCRIPTION
  Called with a handle to an IP fraghdl to get the meta_info_ptr registered with
  it.
  
DEPENDENCIES
  None

RETURN VALUE
  0 if a meta_info_ref_ptr value is set, -1 otherwise
  Value of meta_info_ptr field corresponding to the passed IP fraghdl handle
  returned in meta_info_ref_ptr argument if return value is 0

SIDE EFFECTS
  None
===========================================================================*/
int ip_fraghdl_get_cached_tx_meta_info
(
  void  *handle,
  ps_tx_meta_info_type **meta_info_ref_ptr
);

/*===========================================================================
FUNCTION IP_FRAGHDL_GET_XPORT_PROTO()

DESCRIPTION
  Called with a handle to an IP fraghdl to get the transport layer protocol
  
DEPENDENCIES
  None

RETURN VALUE
  Value of xport_proto field corresponding to the passed IP fraghdl handle

SIDE EFFECTS
  None
===========================================================================*/
ps_ip_protocol_enum_type ip_fraghdl_get_xport_proto
(
  void  *handle
);


/*===========================================================================
FUNCTION IP_FRAGHDL_FREE()

DESCRIPTION
  Called with a handle to an IP fraghdl struct to free.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Frees the IP fraghdl data structure corresponding to the passed handle
===========================================================================*/
void ip_fraghdl_free
(
  void *handle
);


/*===========================================================================
FUNCTION IP_FRAGHDL_INIT()

DESCRIPTION
  Initializes the fraghdl layer
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Initializes fraghdl structures, allocates PS timers etc.
===========================================================================*/
void ip_fraghdl_init
(
  void
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */ 

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_SET_BRIDGE_DEVICE()

DESCRIPTION
  Sets the passed Bridge info [Iface or PPP dev] in the Fraghdl.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void ps_ip_fraghdl_set_bridge_device
(
  void * handle,
  void * bridge_ptr,
  ip_fraghdl_bridge_type_enum_type bridge_type,
  ps_ip_fraghdl_tx_cmd_f_ptr_type  frag_tx_cmd_f_ptr
);

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_SET_LOCAL()

DESCRIPTION
  Sets the is_local flag in the Fraghdl. Thus tags the frag for embedded.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void ps_ip_fraghdl_set_local
(
  void     *handle
);

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_IS_LOCAL()

DESCRIPTION
  Checks the is_local flag in the Fraghdl. Thus tags the frag for embedded.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
boolean ps_ip_fraghdl_is_local
(
  void * handle
);

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_INVALIDATE_RX_MI_CACHE()

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void ps_ip_fraghdl_invalidate_rx_mi_cache
(
  void * handle
);

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_SET_IFACE()

DESCRIPTION
  Sets the iface in the Fraghdl.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
void ps_ip_fraghdl_set_iface
(
  void          *handle,
  ps_iface_type *iface_ptr
);

/*===========================================================================
FUNCTION PS_IP_FRAGHDL_GET_IFACE()

DESCRIPTION
  Returns the iface in the Fraghdl.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
===========================================================================*/
ps_iface_type *ps_ip_fraghdl_get_iface
(
  void          *handle
);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION ps_ip_fraghdl_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for rcvd_fragments_num and ip_fraghdl_array 
  variable

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void ps_ip_fraghdl_qsh_mdump_collect
(
  void
);
#endif/* FEATURE_QSH_MDUMP */

#ifdef __cplusplus
}
#endif

#endif  /* PS_IP_FRGHDL_H */
