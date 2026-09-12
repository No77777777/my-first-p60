/*!
  @file IPA_test.h

  @brief
    IPA Test general header file
*/                               
/*==============================================================================

  Copyright (c) 2013 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test.h#1 $
$Date: 2019/07/05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
07/24/14   mi      Adding IPV6 frag support.
07/20/14   SaCh    Adding Dest_Opts IPv6 Extn hdr.
07/13/14   SaCh    Fixes in Dl/UL/Loopback CLAT areas.
05/27/14   mi      CLAT test support added.
06/12/14   mi      IPA 2.1 Fixes.
05/25/14   rp      Test engine fixes/cleanup for issues seen on full stack build
04/16/14   pgm     API changes for 2.1 tests.
03/06/14   mi      Added fragmentation test changes.
12/5/13    mi      Added DPL support.
11/19/13   pgm     TCP Ack Pkt support.
10/24/13   rp      Heap memory leak fix
09/19/13   rp      Move from QMAPv2 to QMAPv3
08/22/13   rp      Initial Checkin
==============================================================================*/
#ifndef IPA_TEST_H
#define IPA_TEST_H
/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include <ipa.h>
#include "ipa_hw_common.h"

/*==============================================================================

                         MACROS

==============================================================================*/
/*==============================================================================
                   Off-Target/On-Target Abstraction Layer
==============================================================================*/
#ifndef FEATURE_IPA_TEST_ON_TARGET_QTF_ENABLED

#define IPA_TF_ASSERT(x)                   TF_ASSERT(x)                                            
#define IPA_TF_ASSERT_LOG(x, fmt, a, b, c) TF_ASSERT(x)                                            
#define IPA_QTF_MSG(fmtString, x, y, z)                                       \
  TF_MSG("%s(%d):(%s): " fmtString,                                           \
         ipa_test_get_filename(__FILE__), __LINE__, this->GetName(), x, y, z)

#define IPA_QTF_MSG_4(fmtString, v, x, y, z)                                  \
  TF_MSG("%s(%d):(%s): " fmtString,                                           \
         ipa_test_get_filename(__FILE__), __LINE__, this->GetName().c_str(),  \
         v, x, y, z)

#define IPA_QTF_MSG_11(fmtString, x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11)         \
  TF_MSG("%s(%d):(%s): " fmtString,                                           \
         ipa_test_get_filename(__FILE__), __LINE__,  this->GetName().c_str(), \
         x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11)

#define IPA_QTF_MSG_8(fmtString, x1,x2,x3,x4,x5,x6,x7,x8)                     \
  TF_MSG("%s(%d):(%s): " fmtString,                                           \
         ipa_test_get_filename(__FILE__), __LINE__, this->GetName().c_str(),  \
         x1,x2,x3,x4,x5,x6,x7,x8)
                               
#define IPA_QTF_MSG_12(fmtString, x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12)     \
  TF_MSG("%s(%d):(%s): " fmtString,                                           \
         ipa_test_get_filename(__FILE__), __LINE__, this->GetName().c_str(),  \
         x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12)

#else /* FEATURE_IPA_TEST_ON_TARGET_QTF_ENABLED */

#define IPA_TF_ASSERT(x)                   IPA_ASSERT(x)
#define IPA_TF_ASSERT_LOG(x, fmt, a, b, c) IPA_ASSERT_LOG(x, fmt, a, b, c)
#define IPA_QTF_MSG(fmtString, x, y, z)                                       \
  IPA_MSG_SPRINTF_4("(%s): " fmtString,                                       \
                    this->GetName().c_str(), x, y, z)

#endif /* FEATURE_IPA_TEST_ON_TARGET_QTF_ENABLED */

/* External Macros */
#define IPA_QTF_TRACE(fmt, a, b, c)                                           \
  IPA_QTF_MSG (fmt, a, b, c)

#define IPA_QTF_ERROR(fmt, a, b, c)                                           \
  IPA_QTF_MSG (fmt, a, b, c)                             

#define IPA_QTF_ERR_FATAL(fmt, a, b, c)                                       \
  IPA_QTF_MSG (fmt, a, b, c);                                                 \
  IPA_TF_ASSERT(0)

#define IPA_QTF_ASSERT(fmt, a, b, c)                                          \
  IPA_QTF_MSG (fmt, a, b, c);                                                 \
  IPA_TF_ASSERT(0)

#define IPA_TEST_UTIL_HTONS(x)                                                \
  (((x) << 8) | ((x) >> 8))

/*==============================================================================
                       General Test Utility Macros
==============================================================================*/
/*! Watermark Levels */
#define IPA_TEST_MAX_NUM_PKTS               300
#define IPA_TEST_MAX_PACKET_SIZE            IPA_TEST_ENGINE_IP_PKT_SIZE_MAX
#define IPA_TEST_MAX_AGGR_PACKET_SIZE       4100
#define IPA_TEST_WM_DNE                     IPA_TEST_MAX_PACKET_SIZE * IPA_TEST_MAX_NUM_PKTS
#define IPA_TEST_WM_HIGH                    IPA_TEST_MAX_PACKET_SIZE * 200
#define IPA_TEST_WM_LOW                     IPA_TEST_MAX_PACKET_SIZE * 100 
#define IPA_TEST_SIO_WM_DNE                 IPA_TEST_WM_DNE
#define IPA_TEST_PDCP_WM_DNE                IPA_TEST_WM_DNE
#define IPA_TEST_DS_WM_DNE                  IPA_TEST_WM_DNE
#define IPA_TEST_SIM_WM_DNE                 IPA_TEST_WM_DNE

#define IPA_TEST_IPV4_HDR_LEN               20
#define IPA_TEST_IPV6_HDR_LEN               40
#define IPA_TEST_TCP_HDR_LEN                20
#define IPA_TEST_UDP_HDR_LEN                8
#define IPA_TEST_ICMP_HDR_LEN               8
#define IPA_TEST_QMAP_HDR_LEN               4
#define IPA_TEST_QOS_HDR_LEN                6
#define IPA_TEST_ETH_HDR_LEN                14
#define IPA_TEST_ETH_MAC_ADDR_LEN           6
#define IPA_TEST_RANDOM_DATA_BUFFER_SIZE    3000
#define IPA_TEST_TCP_PROT                   0x06
#define IPA_TEST_UDP_PROT                   0x11
#define IPA_TEST_ICMP_PROT                  0x01
#define IPA_TEST_TCP_CKSUM_HDR_OFST         16
#define IPA_TEST_TCP_FLAGS_HDR_OFST         13
#define IPA_TEST_UDP_CKSUM_HDR_OFST         6
#define IPA_TEST_MAX_CIPHER_PARAMS          15
#define IPA_TEST_LOOPBACK_EXTRACT_HDR_LEN   28 
#define IPA_TEST_BRIDGE_MODE_ENABLE
#define IPA_TEST_GEN_AGGR_PKT_LIMIT         5
#define IPA_TEST_ETHER_TYPE_IPV4            0x8000
#define IPA_TEST_ETHER_TYPE_IPV6            0x86DD
#define IPA_TEST_IPV4_SRC_ADDR_OFST         12
#define IPA_TEST_IPV4_DEST_ADDR_OFST        16
#define IPA_TEST_IPV6_SRC_ADDR_OFST         8
#define IPA_TEST_IPV6_DEST_ADDR_OFST        24

#define IPA_TEST_NTH_HEADER_SIZE            0x000C
#define IPA_TEST_NDP_HEADER_SIZE            0x0008
#define IPA_TEST_NDP_PER_PKT_HDR_SIZE       0x0004
#define IPA_TEST_NTH_SIG_VAL                0x484D434E //"NCMH"
#define IPA_TEST_NDP_SIG_VAL                0x00535049 //"IPS"
#define IPA_TEST_MBIM_SIG_SIZE              0x0004
#define IPA_TEST_NDP_TERM_SIZE              0x0004
#define IPA_TEST_NDP_INDEX_SIZE             2

#define IPA_TEST_RNDIS_MSG_TYPE             0x00000001
#define IPA_TEST_RNDIS_NULL_DATA            0x00000000
#define IPA_TEST_RNDIS_RSVD                0x0000000000000000
#define IPA_TEST_RNDIS_HDR_LEN              44
#define IPA_TEST_RNDIS_PAYLOAD_OFFSET       36 //from start of data offset field.
#define IPA_TEST_RNDIS_DATA_OFFSET          8  //offset of data_offset field from head.

/*!Different TCP ACK pkt types*/
#define IPA_TEST_TCP_ACK_PKT_INVALID         0
#define IPA_TEST_TCP_ACK_PKT_1               1
#define IPA_TEST_TCP_ACK_PKT_2               2
#define IPA_TEST_TCP_ACK_PKT_3               3
#define IPA_TEST_TCP_ACK_PKT_MAX             4

#define IPA_TEST_WM_INIT(wm_q_ptr, wm_ptr) \
  do \
  {  \
    dsm_queue_init(wm_ptr, IPA_TEST_WM_DNE, wm_q_ptr); \
  } while (0)

#define IPA_TEST_WM_DESTROY(wm_ptr) \
  do \
  {  \
    dsm_queue_destroy(wm_ptr); \
  } while (0)

#define IPA_TEST_CIPH_DSM_BUF_INIT(dsm_ptr) \
  do \
  {  \
    dsm_ptr = dsm_new_buffer(IPA_CFG_GEN_DSM_ITEM_POOL); \
    IPA_TF_ASSERT(dsm_ptr != NULL); \
    IPA_MEMSET(dsm_ptr->data_ptr, 0, dsm_ptr->size); \
    dsm_ptr->used = dsm_ptr->size; \
  } while(0)


/*=================================================================== 
                
====================================================================*/
/*! @brief
   Return the max of the two values passed.*/
INLINE uint8 IPA_TEST_MAX_VAL(uint8 val_1, uint8 val_2)
{
   return (val_1 > val_2) ? val_1 : val_2;
}

/*=================================================================== 
                 IP packet generation methods
====================================================================*/
/*! @brief
  Sets version in a IPV4/IPV6 header in the specified buffer.*/
INLINE void IPA_TEST_SET_IP_VERSION(uint8 *buf_pkt_ptr, ipa_ip_type_e version)
{
  if (version == IPA_IP_TYPE_IPV4)
  {
    buf_pkt_ptr[0] |= 0x40;
  }
  else
  {
    buf_pkt_ptr[0] |= 0x60;
  }
}

/*! @brief
  Gets version in a IPV4/IPV6 header in the specified buffer.*/
INLINE uint8 IPA_TEST_GET_IP_VERSION(uint8 *buf_pkt_ptr)
{
  return ((buf_pkt_ptr[0] >> 4) & 0x0F);
}

/*! @brief
  Sets IHL in a IPV4 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_IHL(uint8 *buf_pkt_ptr, uint8 hdr_len)
{
  IPA_TF_ASSERT((IPA_BYTES_TO_WORDS(hdr_len) >= 5) &&  \
                (IPA_BYTES_TO_WORDS(hdr_len) < 0x10));
  buf_pkt_ptr[0] |= IPA_BYTES_TO_WORDS(hdr_len);
}

/*! @brief
  Gets IHL in a IPV4 header in the specified buffer. */
INLINE uint8 IPA_TEST_GET_IP_IHL(uint8 *buf_pkt_ptr)
{
  return ((buf_pkt_ptr[0]) & 0x0F);
}

/*! @brief
  Sets Protocol in a IPV4 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_PROTOCOL(uint8 *buf_pkt_ptr, uint8 protocol)
{
  buf_pkt_ptr[9] = protocol;
}

/*! @brief
  Gets Protocol in a IPV4 header in the specified buffer. */
INLINE uint8 IPA_TEST_GET_IP_PROTOCOL(uint8 *buf_pkt_ptr)
{
  return buf_pkt_ptr[9];
}

/*! @brief
  Sets Total length in a IPV4 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_PKT_LEN(uint8 *buf_pkt_ptr, uint16 pkt_len)
{
  *(uint16 *)&buf_pkt_ptr[2] = ipa_ntohs(pkt_len);
}

/*! @brief
  Sets payload length in a IPV6 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_PAYLOAD_LEN(uint8 *buf_pkt_ptr, uint16 payload_len)
{
  *(uint16 *)&buf_pkt_ptr[4] = ipa_ntohs(payload_len); 
}
/*! @brief
  Sets next hdr type in a IPV6 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_NXT_HEADER(uint8 *buf_pkt_ptr, uint8 nxt_hdr_prot)
{
  buf_pkt_ptr[6] = nxt_hdr_prot;
}

/*! @brief
  Gets nxt hdr type in a IPV6 header in the specified buffer. */
INLINE uint8 IPA_TEST_GET_IP_NXT_HEADER(uint8 *buf_pkt_ptr)
{
  return buf_pkt_ptr[6];
}

/*! @brief
  Sets IPID in a IPV4 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_IPID(uint8 *buf_pkt_ptr, uint16 ip_id_val)
{
  *(uint16 *)&buf_pkt_ptr[4] = ipa_ntohs(ip_id_val);
}

/*! @brief
  Sets Frag offset in a IPV6 fagmentation extension header */
INLINE void IPA_TEST_SET_IPV6_FRAG_HDR(uint8 *buf_pkt_ptr, uint64 frag_hdr)
{
  *(uint64*)&buf_pkt_ptr[40] = (frag_hdr);
}

/*! @brief
  Sets IPID in a IPV4 header in the specified buffer. */
INLINE void IPA_TEST_SET_IP_FRAG_HDR(uint8 *buf_pkt_ptr, uint16 frag_hdr)
{
  *(uint16 *)&buf_pkt_ptr[6] = ipa_ntohs(frag_hdr);
}

/*! @brief
  Sets Next_Header in a IPv6 header in the specified buffer. */
INLINE void IPA_TEST_SET_IPV6_NEXT_HDR(uint8 *buf_pkt_ptr, uint8 next_hdr)
{
  buf_pkt_ptr[6] = next_hdr;
}

/*! @brief
  Sets Next_Header field in a IPv6 Extension header in the specified buffer. */
INLINE void IPA_TEST_SET_IPV6_EXT_NEXT_HDR(uint8 *buf_pkt_ptr, uint8 next_hdr)
{
  buf_pkt_ptr[40] = next_hdr;
}

/*! @brief
  Sets Hdr_Ext_len field in a IPv6 Extension header in the specified buffer. */
INLINE void IPA_TEST_SET_IPV6_EXT_HDR_EXT_LEN(uint8 *buf_pkt_ptr, uint8 hdr_ext_len)
{
  buf_pkt_ptr[41] = hdr_ext_len;
}

/*! @brief
   Sets the UDP source and destination addresses. */
INLINE void IPA_TEST_SET_UDP_PORTS(uint8 *buf_pkt_ptr, uint16 src_port, uint16 dst_port)
{
  *(uint16 *)&buf_pkt_ptr[0] = ipa_ntohs(src_port);
  *(uint16 *)&buf_pkt_ptr[2] = ipa_ntohs(dst_port);
}

/*! @brief
   Sets the ICMP header type & code value. */
INLINE void IPA_TEST_SET_ICMP_TYPE_CODE(uint8 *buf_pkt_ptr, uint8 type, uint8 code)
{
  buf_pkt_ptr[0] = type;
  buf_pkt_ptr[1] = code;
}

/*! @brief
   Sets the ICMP header checksum value. */
INLINE void IPA_TEST_SET_ICMP_CKSUM(uint8 *buf_pkt_ptr, uint16 cksum)
{
  *(uint16 *)&buf_pkt_ptr[2] = ipa_ntohs(cksum);
}

/*! @brief
   Sets the TCP ACK flag in IPv4 packet*/
INLINE void IPA_TEST_SET_TCP_ACK_FLAG(uint8 *buf_pkt_ptr)
{
  //*(uint8 *)&buf_pkt_ptr[33] = 0x08; /*TCP flag field = 34th byte from IPv4 hdr*/
  *(uint8 *)&buf_pkt_ptr[33] = 0x10; /*TCP flag field = 34th byte from IPv4 hdr*/
}

/*! @brief
   Sets the TCP ACK flag in IPv6 packet*/
INLINE void IPA_TEST_SET_V6_TCP_ACK_FLAG(uint8 *buf_pkt_ptr)
{
  //*(uint8 *)&buf_pkt_ptr[53] = 0x08; /*TCP flag field = 54th byte from IPv6 hdr*/
  *(uint8 *)&buf_pkt_ptr[53] = 0x10; /*TCP flag field = 54th byte from IPv6 hdr*/
}

/*! @brief
   Sets the TCP hdr len in data offset field of TCP hdr*/
INLINE void IPA_TEST_SET_TCP_HDR_LEN(uint8 *buf_pkt_ptr, uint8 tcp_hdr_len)
{
  buf_pkt_ptr[32] |= IPA_BYTES_TO_WORDS(tcp_hdr_len) << 4;
}

/*! @brief
   Sets the TCP hdr len in data offset field of TCP hdr in V6 pkt*/
INLINE void IPA_TEST_SET_V6_TCP_HDR_LEN(uint8 *buf_pkt_ptr , uint8 tcp_hdr_len)
{
  buf_pkt_ptr[52] |= IPA_BYTES_TO_WORDS(tcp_hdr_len) << 4;
}

/*! @brief
   Sets the SRC Address in a V4 pkt*/
INLINE void IPA_TEST_SET_V4_SRC_ADDR(uint8 *buf_pkt_ptr, uint32 src_addr)
{
  *(uint32*)&buf_pkt_ptr[IPA_TEST_IPV4_SRC_ADDR_OFST] = src_addr;
}

/*! @brief
   Sets the DEST Address in a V4 pkt*/
INLINE void IPA_TEST_SET_V4_DEST_ADDR(uint8 *buf_pkt_ptr, uint32 dest_addr)
{
  *(uint32*)&buf_pkt_ptr[IPA_TEST_IPV4_DEST_ADDR_OFST] = dest_addr;
}

/*! @brief
  Get prefix length*/
INLINE uint8 IPA_TEST_GET_PREFIX_LEN(ipa_clat_prefix_len_e v6_dest_prefix_len)
{
  uint8 prefix_len = 0;
  switch (v6_dest_prefix_len)
  {
    case IPA_CLAT_V6_PREFIX_LEN_96:
    {
      prefix_len = 96;
      break;
    }
    case IPA_CLAT_V6_PREFIX_LEN_64:
    { 
      prefix_len = 64;
      break;;
    }
    case IPA_CLAT_V6_PREFIX_LEN_56:
    {
      prefix_len = 56;
      break;;
    }
    case IPA_CLAT_V6_PREFIX_LEN_48:
    {
      prefix_len = 48;
      break;
    }
    case IPA_CLAT_V6_PREFIX_LEN_40:
    {
      prefix_len = 40;
      break;
    }
    case IPA_CLAT_V6_PREFIX_LEN_32:
    {
      prefix_len = 32;
      break;
    }
    default:
      break;
  }
  return prefix_len;
}

/*! @brief
   Sets the SRC Prefix in a V6 pkt*/
void IPA_TEST_SET_V6_SRC_PREFIX(uint8 *buf_pkt_ptr, uint8 *src_prefix,
                                ipa_clat_prefix_len_e v6_src_prefix_len);

/*! @brief
   Sets the Dest Prefix in a V6 pkt*/
void IPA_TEST_SET_V6_DEST_PREFIX(uint8 *buf_pkt_ptr, uint8 *dest_prefix,
                                 ipa_clat_prefix_len_e v6_dest_prefix_len);

/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/

typedef PACKED struct PACKED_POST
{
  uint16  ofst    : 13;
  uint16  mf      : 1;
  uint16  df      : 1;
  uint16  rsvd    : 1;
} ipa_test_ip_pkt_frag_hdr_s;

typedef PACKED struct PACKED_POST
{
  uint16  mf      : 1;
  uint16  rsvd    : 2;
  uint16  ofst    : 13;
} ipa_test_ipv6_pkt_frag_ofst_s;

typedef PACKED struct PACKED_POST
{
  uint64  nxt_hdr : 8;
  uint64  rsvd1   : 8;
  uint64  ofst    : 13;
  uint64  rsvd    : 2;
  uint64  mf      : 1;
  uint64  ipid    : 32;
} ipa_test_ipv6_pkt_frag_extn_hdr_s;

typedef PACKED struct PACKED_POST
{
  uint8   next_hdr;
  uint8   hdr_ext_len;
  uint16  options_1;
  uint32  options_2;
} ipa_test_ipv6_pkt_routing_next_hdr_s;

typedef PACKED struct PACKED_POST
{
  uint8   next_hdr;
  uint8   hdr_ext_len;
  uint16  options_1;
  uint32  options_2;
} ipa_test_ipv6_pkt_dest_opts_next_hdr_s;

/*! @brief DL checksum trailer def */
typedef PACKED struct PACKED_POST
{
  uint32 start_offset:16;
  uint32 valid:1;
  uint32 rsvd2:7; 
  uint32 rsvd1:8; 
  uint32 value:16;
  uint32 len:16;
} ipa_test_dl_cksum_trailer_s;

/*! QMAP flow control command */
typedef PACKED struct PACKED_POST
{
  uint32 seq_num    : 16; 
  uint32 ip_family  : 2;
  uint32 reserved   : 14;
  uint32 qos_id;
} ipa_test_qmap_cmd_flow_ctl_s;

/*! @brief Ethernet header def */
typedef PACKED struct PACKED_POST
{
  uint8  dst_mac_addr[IPA_TEST_ETH_MAC_ADDR_LEN];
  uint8  src_mac_addr[IPA_TEST_ETH_MAC_ADDR_LEN];
  uint16 ether_type;
} ipa_test_eth_hdr_s;

typedef PACKED struct PACKED_POST
{
  uint16  src_addr[8];
  uint16  dest_addr[8];
  uint32 tcp_length;
  //uint16 reserved_1;
  //uint16  reserved_2;
  uint32  protocol;   
  
} ipa_test_ipv6_pseudo_header_type;

typedef PACKED struct PACKED_POST
{
  uint32  src_addr;
  uint32  dest_addr;
  //uint8   reserved;
  uint16   protocol;   
  uint16  tcp_length;
} ipa_test_ipv4_pseudo_header_type;


/*! @brief Frag context for packet generation */
typedef struct
{
  /*! Frag is ongoing, caller can check this flag to determine
   * if more frags are to be expected */
  uint8         frag_on;
  /*! For frag cfg, this len != 0, each frag would
   * contain this payload len maybe except for last one */
  uint16        frag_pkt_len;
  /*! Last generated frag had this offset, so new one
   * would be this + frag_pkt_len */
  uint16        last_frag_ofst; 
  /*! Store the original pkt len when we started
   * fragmenting and replace the one in parent struct
   * with frag pkt len */
  uint16        orig_pkt_len;
  /*! Remaining packet len to create frags */
  uint16        rem_pkt_len;
  /*! Current IP ID */
  uint16        curr_ip_id; 
} ipa_test_ip_pkt_frag_ctxt_s;

/*! @brief Structure storing the NTH header of MBIM */
//typedef PACKED struct PACKED_POST
typedef struct
{
  uint32 sig_nth16;
  uint16 hdr_len;
  uint16 seq;
  uint16 tot_len;
  uint16 idx_ndp;    
} ipa_test_nth_hdr_s;

/*! @brief Structure storing the NDP header of MBIM */
//typedef PACKED struct PACKED_POST
typedef struct
{
  uint32 sig_ndp16;
  uint16 hdr_len;
  uint16 idx_nxt;
} ipa_test_ndp_hdr_s;

/*! @brief Struct storing per pkt hdr in NDP of MBIM*/
//typedef PACKED struct PACKED_POST
typedef struct
{
  uint16 dgram_idx;
  uint16 dgram_len;
} ipa_test_ndp_hdr_pkt_s;

/*! @brief MBIM frame format types*/
typedef enum
{
  IPA_TEST_MBIM_NDP_FIRST,
  IPA_TEST_MBIM_PKTS_FIRST,
  IPA_TEST_MBIM_RANDOM,
  IPA_TEST_MBIM_MAX
} ipa_test_mbim_frame_type_e;

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
INLINE const char *ipa_test_get_filename
(
  const char *path
)
{
  const char *fname_ptr = NULL;

  /* strip the path */
  fname_ptr = strrchr(path,'/');

  /* if forward slash didn't match try a backslash */
  if (fname_ptr == NULL)
  {
    fname_ptr = strrchr(path,'\\');
  }

  /* if the path can't be stripped */
  if (fname_ptr == NULL)
  {
    fname_ptr = path;
  }
  else
  {
    /* skip the '/' char */
    fname_ptr++;
  }

  return(fname_ptr);
}

/*=========================================================================
                         Generic functions
==========================================================================*/
/*==============================================================================

 FUNCTION: ipa_test_timetick_init

==============================================================================*/
/*!
  @brief
  Inits the timetick for tests

  @return
  None
*/
/*============================================================================*/
void ipa_test_timetick_init(void);

/*==============================================================================

 FUNCTION: ipa_test_timetick_get

==============================================================================*/
/*!
  @brief
  Gets the timetick for tests

  @return
  None
*/
/*============================================================================*/
ipa_timetick_t ipa_test_timetick_get(void);

/*==============================================================================

 FUNCTION: ipa_test_generate_random_num

==============================================================================*/
/*!
  @brief
  Generates a random number between min & max

  @return
  None
*/
/*============================================================================*/
uint32 ipa_test_generate_random_num
(
  uint32 max_number,
  uint32 min_number
);

/*==============================================================================

 FUNCTION: ipa_test_generate_unique_random_num

==============================================================================*/
/*!
  @brief
  Generates a random number between & including min & max 
  Generates random number which does not already exist in the array provided

  @return
  None
*/
/*============================================================================*/
uint8 ipa_test_generate_unique_random_num
(
  uint8 max_number,
  uint8 min_number,
  uint8 *exist_num_arr_ptr,
  uint8 exist_num_arr_size
);

/*==============================================================================

 FUNCTION: ipa_test_generate_random_num

==============================================================================*/
/*!
  @brief
  Generates random data into a buffer 

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_random_data
(
  uint8 *rand_data,
  uint32 rand_data_size
);

/*==============================================================================

  FUNCTION: ipa_test_generate_sequence_data

==============================================================================*/
/*!
  @brief
  Generate sequence data (1-256) in the given memory location for 
  specifed size

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_sequence_data
(
  uint8 *seq_data,
  uint32 data_size
);

/*==============================================================================

  FUNCTION: ipa_test_get_index_from_bitmask

==============================================================================*/
/*!
  @brief
  Get the index for SIO/SIM/bearer from the bit mask

  @return
  None
*/
/*============================================================================*/
uint8 ipa_test_get_index_from_bitmask
(
  uint64 bitmask
);

/*==============================================================================

  FUNCTION: ipa_test_set_64_bitmask

==============================================================================*/
/*!
  @brief
  Set the bitmask for 64 bit 

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_set_64_bitmask
(
  uint64 *mask_ptr,
  uint8  bit_pos
)
{
  uint32 bit_mask = (1 << (bit_pos % 32));

  if (bit_pos >= 32)
  {
    (*((uint32 *)mask_ptr + 1)) |= bit_mask;
  }
  else
  {
    (*(uint32 *)mask_ptr) |= bit_mask;
  }
}

/*==============================================================================

  FUNCTION: ipa_test_reset_64_bitmask

==============================================================================*/
/*!
  @brief
  Set the bitmask for 64 bit 

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_reset_64_bitmask
(
  uint64 *mask_ptr,
  uint8  bit_pos
)
{
  uint32 bit_mask = (1 << (bit_pos % 32));

  if (bit_pos >= 32)
  {
    (*((uint32 *)mask_ptr + 1)) &= ~bit_mask;
  }
  else
  {
    (*(uint32 *)mask_ptr) &= ~bit_mask;
  }
}

/*=========================================================================
                         Endp Cfg functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_set_endp_map

==============================================================================*/
/*!
  @brief
  Sets the Endpoint mapping with the driver

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_endp_map
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    hw_replication
);

/*==============================================================================

  FUNCTION:  ipa_test_register_per_sim

==============================================================================*/
/*!
  @brief
  Registers a per sim with the driver and initializes the watermarks associated 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_register_per_sim
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    tx_delay  
);

/*==============================================================================

  FUNCTION:  ipa_test_deregister_per_sim

==============================================================================*/
/*!
  @brief
  DeRegisters a per sim with the driver and destroys the watermarks associated 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deregister_per_sim
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_open_phy_sio_port

==============================================================================*/
/*!
  @brief
  Opens a physical SIO port and initilializes the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_open_phy_sio_port 
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_close_phy_sio_port

==============================================================================*/
/*!
  @brief
  Closes a physical SIO port and destroys the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_close_phy_sio_port 
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_open_log_sio_port

==============================================================================*/
/*!
  @brief
  Opens a logical SIO port and initilializes the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_open_log_sio_port 
(
  uint8                    log_ch_idx, 
  ipa_test_engine_log_ch_s *log_ch_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_close_log_sio_port

==============================================================================*/
/*!
  @brief
  Closes a logical SIO port and destroys the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_close_log_sio_port 
(
  ipa_test_engine_log_ch_s *log_ch_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_sio_get_port_id

==============================================================================*/
/*!
  @brief
  Gets a new SIO port ID

  @return
  None
*/
/*============================================================================*/
sio_port_id_type ipa_test_sio_get_port_id
(
  uint8   is_logical,
  uint32  port_index 
);

/*==============================================================================

  FUNCTION:  ipa_test_sio_get_log_ch_idx

==============================================================================*/
/*!
  @brief
  Find the logical channel index on a physical channel

  @return
  None
*/
/*============================================================================*/
ipa_test_engine_log_ch_s *ipa_test_sio_get_log_ch_idx
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    mux_id 
);

/*==============================================================================

  FUNCTION:  ipa_test_check_sio_aggr_prot_support

==============================================================================*/
/*!
  @brief
  Checks whether the aggregation protocols in current sio cfg is supoorted 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_check_sio_aggr_prot_support
(
  ipa_sio_endpoint_config_s *sio_cfg_ptr,
  uint8                     ic_type
);

/*==============================================================================

  FUNCTION:  ipa_test_set_sio_cfg_map

==============================================================================*/
/*!
  @brief
  Configures a SIO mapping on the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_sio_cfg_map
(
  ipa_test_engine_log_ch_s  *log_ch_ptr,
  ipa_sio_endpoint_config_s *sio_cfg_ptr
);

/*=========================================================================
                         WAN Cfg functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_set_bearer_cfg

==============================================================================*/
/*!
  @brief
  Configures a WAN beaerer with the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_bearer_cfg
(
  uint8                     bearer_idx, 
  ipa_test_engine_bearer_s  *bearer_ptr,
  ipa_test_engine_log_ch_s  *log_ch_ptr,
  uint8                     uid, 
  ipa_wan_subscription_id_t subs_id,
  uint8                     is_bridged, 
  uint8                     is_dpl_enabled,
  ipa_wan_bearer_tech_e     bearer_tech,
  uint8                     is_clat
);

/*==============================================================================

  FUNCTION:  ipa_test_bearer_destroy

==============================================================================*/
/*!
  @brief
  Destroys a WAN beaerer with the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_bearer_destroy
(
  ipa_test_engine_bearer_s  *bearer_ptr
);

/*=========================================================================
                         DPL Cfg functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_set_dpl_cfg

==============================================================================*/
/*!
  @brief
  Configures a DPL with the driver 

  @return
  None
*/
/*============================================================================*/
void ipa_test_set_dpl_cfg
(
  ipa_test_engine_dpl_s   *dpl_ptr
);

/*==============================================================================

  FUNCTION:  ipa_test_dpl_destroy

==============================================================================*/
/*!
  @brief
  Destroys DPL watermarks

  @return
  None
*/
/*============================================================================*/
void ipa_test_dpl_destroy
(
  ipa_test_engine_dpl_s   *dpl_ptr
);

/*=========================================================================
                         Ciphering functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_generate_ciph_keystream

==============================================================================*/
/*!
  @brief
  Apply the ciphering algorithm on the data set to all zeroes to 
  get the keystream values for all the cipher param configs

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_ciph_keystream
(
  ipa_test_engine_ciph_params_s *ciph_params
);

/*==============================================================================

  FUNCTION:  ipa_test_xor_ciph_keystream

==============================================================================*/
/*!
  @brief
  Cipher the DSM buffer data by XORing the passed DSM pointer 
  with the keystream value of the cipher param used

  @return
  None
*/
/*============================================================================*/
void ipa_test_xor_ciph_keystream
(
  dsm_item_type                   *tx_dsm_ptr,
  uint16                          pkt_len,
  ipa_test_engine_ciph_params_s *ciph_params
);

/*==============================================================================
                         IP Pkt generation functions
==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_generate_ip_packet

==============================================================================*/
/*!
  @brief
  Generate IP Packet according to the ip_pkt_cfg structure 
  in the specified memory location. 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_ip_packet
(
  ipa_test_engine_ip_pkt_cfg_s  *ip_pkt_cfg_ptr, 
  ipa_test_engine_ip_pkt_info_s *pkt_info
);

/*==============================================================================

  FUNCTION:  ipa_test_gen_tcp_ack_packet

==============================================================================*/
/*!
  @brief
  Generate IP Packet according to the ip_pkt_cfg structure 
  in the specified memory location. 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_tcp_ack_packet
(
  ipa_test_engine_ip_pkt_cfg_s  *ip_pkt_cfg_ptr, 
  ipa_test_engine_ip_pkt_info_s *pkt_info
);

/*==============================================================================

  FUNCTION:  ipa_test_generate_dsm_chain

==============================================================================*/
/*!
  @brief
  Make a dsm chain from the packet buffer provided. 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_dsm_chain
(
  dsm_item_type **tx_dsm_item_ptr,
  uint8         tot_num_dsm_buffers,
  uint8         *pkt_buffer_ptr,
  uint32        pkt_len
);

/*==============================================================================

  FUNCTION:  ipa_test_generate_random_dsm_chain

==============================================================================*/
/*!
  @brief
  Make a dsm chain from the packet buffer provided with zero length buffers
  in between. For now this is used for testing DMA task failure when zero
  length buffers are programmed into IPA.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_random_dsm_chain
(
  dsm_item_type * head_dsm_ptr
);

/*==============================================================================
                         Mem comparison functions
==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_compare_mem

==============================================================================*/
/*!
  @brief
  Compare the two specified memory location, word by word.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_compare_mem
(
  uint16 buf_len,
  uint8  *buf_ptr1, 
  uint8  *buf_ptr2,
  uint16 *no_match_locn
);
/*==============================================================================

  FUNCTION:  ipa_test_get_frag_status

==============================================================================*/
/*!
  @brief
  Get the frag info whether its on or not

  @return
  TRUE if frag is ON. FALSE if frag is off.
*/
/*============================================================================*/
boolean ipa_test_get_frag_status();

/*==============================================================================
                         Aggregation/Deaggregation functions
==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_aggregate_none_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_NONE type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_none_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_ethernet_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_ETHERNET type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_ethernet_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qos_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_NONE & qos_hdr type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qos_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);


/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qmap_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_QMAP type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qmap_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s  *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qmap_v3_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_QMAP_V2 type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qmap_v3_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  uint8                         is_clat,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*=========================================================================

  FUNCTION:  ipa_test_aggregate_mbim_endp_pkt

==========================================================================*/
  /*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_MBIM type frame
   
  @return
  None
*/
/*========================================================================*/ 
ipa_test_ret_code_e ipa_test_aggregate_mbim_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s   *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s  *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*=========================================================================

  FUNCTION:  ipa_test_aggregate_rndis_endp_pkt

==========================================================================*/
  /*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_RNDIS type frame
   
  @return
  None
*/
/*========================================================================*/ 

ipa_test_ret_code_e ipa_test_aggregate_rndis_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s   *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s  *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);


/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_none_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_NONE type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_none_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_ethernet_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_ETHERNET type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_ethernet_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_qmap_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_QMAP type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_qmap_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_qmap_v3_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_QMAP_v3 type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_qmap_v3_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*=========================================================================

  FUNCTION:  ipa_test_deaggregate_mbim_endp_pkt

==========================================================================*/
  /*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_MBIM type frame
   
  @return
  None
   
*/
/*========================================================================*/ 
ipa_test_ret_code_e ipa_test_deaggregate_mbim_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*=========================================================================

  FUNCTION:  ipa_test_deaggregate_rndis_endp_pkt

==========================================================================*/
  /*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_RNDIS type frame
   
  @return
  None
   
*/
/*========================================================================*/ 

ipa_test_ret_code_e ipa_test_deaggregate_rndis_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
);

/*==============================================================================

  FUNCTION:  ipa_test_calculate_cksum

==============================================================================*/
/*!
  @brief
  Calculates the Cksum value

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_cksum
(
  /* Array of bytes of data */
  uint16  *buf_ptr,
  /* Number of bytes */
  uint32  count
);


/*==============================================================================

  FUNCTION:  ipa_test_calculate_pseudo_hdr_cksum

==============================================================================*/
/*!
  @brief
  Calculates the Cksum value of Pseudo hdr

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_pseudo_hdr_cksum
(
  /* Array of bytes of data */
  uint8  *buf_pkt_ptr
);


/*==============================================================================

  FUNCTION:  ipa_test_calculate_tcp_cksum

==============================================================================*/
/*!
  @brief
  Calculates the TCP Cksum value

  @return
  None
*/
/*============================================================================*/
void ipa_test_calculate_tcp_cksum
(
  /* IP Packet buffer */
  uint8 *buf_pkt_ptr
);


/*==============================================================================

  FUNCTION:  ipa_test_calculate_cksum

==============================================================================*/

/*! @brief Calculates the Cksum value */

/*===========================================================================*/
uint16 IpaTestCalculateCksum
(
  /* Array of bytes of data */
  uint16  *buf,
  /* Number of bytes */
  uint32  count
);


ipa_test_ret_code_e ipa_test_deaggregate_replicated_pkts 
( 
	ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr_ul,
   	uint8                         *num_pkts_used_ul,
   	ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr_dl,
   	uint8                         *num_pkts_used_dl,
   	dsm_item_type                 **aggr_frame_dsm_ptr
);



#endif /* IPA_TEST_H */
