#ifndef PS_OPT_PATH_DEFS_H
#define PS_OPT_PATH_DEFS_H
/*===========================================================================

                     PS_OPT_PATH_DEFS . H
DESCRIPTION
  Header file describing all Optimized data path definitions used by
    both UL/DL.

Copyright (c) 2011-2013 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE
  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_opt_defs.h#2 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/27/13    wc     Support dynamic port configuration
01/29/13    vb     Fix to handle the GRE packet processing in
                   NAT optimized path.
09/14/12    tw     IPv6 prefix missmatch errors fixed
05/24/2012  pgm    RNDIS defns.
05/07/2012  mp     Fix to send TCP Reset for exhausted NAT entry.
08/15/2011  AM     created module.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "customer.h"

#define PS_DPM_UL_OPT_PATH_WORD_SIZE          4
#define PS_DPM_UL_OPT_PATH_WORD_SHIFT         2

/*-----------------------------------------------------------------------
    ETHERNET HDR
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_ETH_HEADER_OFFSET  14
#define PS_DPM_OPT_ETHERTYPE_OFFSET   12

/*-----------------------------------------------------------------------
    IP HDR
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_IP_VERSION_MASK    0xF0
#define PS_DPM_OPT_IP_HDR_LEN_MASK    0x0F

#define PS_DPM_OPT_QOS_HEADER_OFFSET  6

/*-----------------------------------------------------------------------
    IPV4
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_V4_VERSION           0x40
#define PS_DPM_OPT_V4_HDR_LEN           20
#define PS_DPM_OPT_MAX_V4_HDR_LEN       60
#define PS_DPM_OPT_V4_TOTAL_LEN_OFFSET  2
#define PS_DPM_OPT_V4_FRAG_OFFSET       6
#define PS_DPM_OPT_V4_PROT_FIELD_OFFSET 9
#define PS_DPM_OPT_V4_SRC_ADDR_OFFSET   12
#define PS_DPM_OPT_V4_DEST_ADDR_OFFSET  16
#define PS_DPM_OPT_V4_FRAG_MASK         0x3FFF
#define PS_DPM_OPT_V4_ADDRESS_LEN       4
#define PS_DPM_OPT_V4_SRC_PORT_OFFSET   20
#define PS_DPM_OPT_V4_DST_PORT_OFFSET   22
#define PS_DPM_OPT_V4_TCP_FLAGS_OFFSET  33
#define PS_DPM_OPT_V4_TCP_CKSUM_OFFSET  36
#define PS_DPM_OPT_V4_TCP_SEQNO_OFFSET  24
#define PS_DPM_OPT_V4_UDP_CKSUM_OFFSET  26
#define PS_DPM_OPT_V4_GRE_CALLID_OFFSET 26
#define PS_DPM_OPT_V4_ID_OFFSET         32
#define PS_DPM_OPT_V4_ID_LEN            16

/*-----------------------------------------------------------------------
    IPV6
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_V6_VERSION           0x60
#define PS_DPM_OPT_V6_HDR_LEN           40
#define PS_DPM_OPT_V6_TOTAL_LEN_OFFSET  4
#define PS_DPM_OPT_V6_NXT_HDR_OFFSET    6
#define PS_DPM_OPT_V6_DEST_ADDR_OFFSET  24
#define PS_DPM_OPT_V6_SRC_ADDR_OFFSET   8
#define PS_DPM_OPT_V6_MCAST_MASK        0xFF


#define PS_DPM_OPT_PORT_INFO_LEN            4
#define PS_DPM_OPT_EXP_HEADER_LEN_DL                                     \
    ( PS_DPM_OPT_MAX_V4_HDR_LEN + PS_DPM_OPT_PORT_INFO_LEN )

/*-----------------------------------------------------------------------
    TCP
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_TCP_HDR_LEN_OFFSET   12
#define PS_DPM_OPT_TCP_HDR_LEN_EXTRACT  13
#define PS_DPM_OPT_TCP_HDR_LEN_MASK     0xF0
#define PS_DPM_OPT_TCP_RST_FIN_SET      0x05
#define PS_DPM_OPT_TXPRT_PORT_LEN       2

#define PS_DPM_OPT_UDP_HDR_LEN          8

/*-----------------------------------------------------------------------
   HEADER LEN FOR ALL MODES
   PS_OPT_PATH_IP_NO_QOS_HEADER_LEN: IP(40) + TCP Header len(13)
   PS_OPT_PATH_IP_QOS_HEADER_LEN: QOS(6) + IP(40) + TCP Header len(13)
   PS_OPT_PATH_ETH_NO_QOS_HEADER_LEN:IP(40)+ETH(14) +
                                           TCP Header len(13)
   PS_OPT_PATH_ETH_QOS_HEADER_LEN:QOS(6)+IP(40)+ETH(14) +
                                           TCP Header len(13)
 -----------------------------------------------------------------------*/
#define PS_OPT_PATH_IP_NO_QOS_HEADER_LEN         \
   (PS_DPM_OPT_V6_HDR_LEN +                     \
    PS_DPM_OPT_TCP_HDR_LEN_EXTRACT)

#define PS_OPT_PATH_IP_QOS_HEADER_LEN            \
   (PS_DPM_OPT_V6_HDR_LEN +                     \
    PS_DPM_OPT_QOS_HEADER_OFFSET +              \
    PS_DPM_OPT_TCP_HDR_LEN_EXTRACT)

#define PS_OPT_PATH_ETH_NO_QOS_HEADER_LEN        \
  (PS_DPM_OPT_V6_HDR_LEN +                      \
   PS_DPM_OPT_ETH_HEADER_OFFSET +               \
   PS_DPM_OPT_TCP_HDR_LEN_EXTRACT)

#define PS_OPT_PATH_ETH_QOS_HEADER_LEN            \
  (PS_DPM_OPT_V6_HDR_LEN        +                \
   PS_DPM_OPT_QOS_HEADER_OFFSET +                \
   PS_DPM_OPT_ETH_HEADER_OFFSET +                \
   PS_DPM_OPT_TCP_HDR_LEN_EXTRACT)

/*-----------------------------------------------------------------------
    HEADER LEN CHECKS FOR DSM_EXTRACT
 -----------------------------------------------------------------------*/
#define PS_DPM_OPT_IP_NO_QOS_HEADER_LEN_CHECK    \
   PS_DPM_OPT_V6_HDR_LEN

#define PS_DPM_OPT_IP_QOS_HEADER_LEN_CHECK       \
  (PS_DPM_OPT_V6_HDR_LEN +                       \
   PS_DPM_OPT_QOS_HEADER_OFFSET)

#define PS_DPM_OPT_ETH_NO_QOS_HEADER_LEN_CHECK   \
  (PS_DPM_OPT_V6_HDR_LEN +                       \
   PS_DPM_OPT_ETH_HEADER_OFFSET)

#define PS_DPM_OPT_ETH_QOS_HEADER_LEN_CHECK      \
  (PS_DPM_OPT_V6_HDR_LEN        +                \
   PS_DPM_OPT_QOS_HEADER_OFFSET +                \
   PS_DPM_OPT_ETH_HEADER_OFFSET)

/*-----------------------------------------------------------------------
    MBIM HEADER
 -----------------------------------------------------------------------*/

/*NTH COMMON HEADER*/
#define PS_OPT_PATH_NTHC_LEN      8
#define PS_OPT_PATH_NTHS_LEN      4

/*NDP HEADER*/
#define PS_OPT_PATH_NDP_HDR_LEN           8
#define PS_OPT_PATH_NDP_DG_OFFSET         4
#define PS_OPT_PATH_NDP_MAX_DG            10
#define PS_OPT_PATH_NDP_HDR_FIELD_OFFSET  4
#define PS_OPT_PATH_NDP_HDR_FIELD_LEN     2
#define PS_OPT_PATH_NDP_LEN_CHECK         16
#define PS_OPT_PATH_NDP_IP_OFF_LEN_PAIR_LEN          \
        (PS_OPT_PATH_NDP_MAX_DG * PS_OPT_PATH_NDP_DG_OFFSET)
#define PS_OPT_PATH_MAX_ST_NDP_LEN                   \
        (PS_OPT_PATH_NDP_IP_OFF_LEN_PAIR_LEN +       \
         PS_OPT_PATH_NDP_HDR_LEN)

#endif
