#ifndef PS_LOGGING_DEFS_H
#define PS_LOGGING_DEFS_H
/*===========================================================================

                        P S _ L O G G I N G _ D E F S . H

GENERAL DESCRIPTION
  This file defines the data sturctures, constants used by PS to
  frame diagnostic responses to the requests from the tool (DM).

Copyright (c) 2004-2013 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

 $Header: //components/rel/data.mpss/3.5..2.1/api/public/ps_logging_defs.h#3 $
 $Author: pwbldsvc $ $DateTime: 2023/10/16 00:28:16 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
10/09/12    sj     DPL logging for 8 PDN support
03/08/12    su     Changes for VToLTE feature 
02/10/12    rt     Added support for SoftAPv6 iface
01/04/12    hs     Added support for eMBMS
12/07/11   vpk     Adding 5 PDN logging to DPL list
11/22/11    sa     Adding TLB Mode B Rm Ifaces to DPL list.
11/28/11    sun    Adding support for DPL logging using log codes  
03/14/11    kk     Adding new RmNet ifaces to DPL list.
11/11/10    gn     Adding new WLAN ifaces to DPL list 
06/30/10    rt     Added DPL support on NAT iface.
03/15/10    hs     Added DPL support on two new LTE Ifaces
08/31/09   mga     Merged from eHRPD branch
12/14/08    pp     Common Modem Interface: Public/Private API split.
21/11/08    am     Macro name changes for DPL IID flags.
10/17/08    am     Fixed ANSI C warnings for bit-field usage.
09/04/08    ssh    Removed featurization from dpl_iid_ifname_enum_type
09/01/08    ssh    Added UICC to iface list
12/25/06    msr    Fixed broken secondary link logging
08/31/06    mjp    Added IPHC and ROHC Logging
06/06/06    ss     Renamed DVBH_IFACE to DVBH_V4_IFACE, added DVBH_V6_IFACE
04/14/06    ss     Added DVBH to ifname list
05/31/05   aku/jd  Added WLAN iface to ifname list
03/15/05    ks     changed name of RMNET iface in dpl_iid_ifname_enum_type
01/12/05    msr    Added code review comments.
01/10/05    msr    Added support for flow and link logging.
11/08/04    msr    Moved the condition, which checks if DPL_IID_IFNAME_MAX is
                   greater than MAX_SYSTEM_IFACES, to ps_logging_diag.c
10/31/04   msr/ks  Changed the name of this file from ps_loggingi.h to
                   ps_logging_defs.h.
09/15/04   ks/msr  Major cleanup.
08/09/04    vd     Created the file.
===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#include "comdef.h"
#include "ps_iface_defs.h"


/*===========================================================================

                            CONSTANTS

===========================================================================*/
/*---------------------------------------------------------------------------
  Maximum length of ps_iface's and phys link's description
---------------------------------------------------------------------------*/
#define DPL_IFNAME_DESC_S_LEN  32
#define DPL_APN_DESC_S_LEN     32
#define DPL_NUM_IFACES_PER_PDN  4

#define DPL_LINK_DESC_S_LEN    DPL_IFNAME_DESC_S_LEN
#define DPL_FLOW_DESC_S_LEN    DPL_IFNAME_DESC_S_LEN

/*---------------------------------------------------------------------------
  Snaplen value special case to indicate that whole packet needs to be
  logged
---------------------------------------------------------------------------*/
#ifndef UINT32_MAX
#define UINT32_MAX           4294967295u
#endif

#define SNAPLEN_ALL  UINT32_MAX

#define DPL_PACKET_PARTIAL_LENGTH 100

/*---------------------------------------------------------------------------
  Snaplen value special case to indicate the maximum partial logging length,
  that is supporte
---------------------------------------------------------------------------*/
#define SNAPLEN_MAX  65535

/*---------------------------------------------------------------------------
  DPL flag masks and access macros.
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------

Interface Identifier (IID) fields:

  + - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +
  | Flags           | Ifname          | Protocol        | Link instance   |
  + - - - - - - - - + - - - - - - - - + - - - - - - - - + - - - - - - - - +
  |                  \
  |                   \
  + - - - - - - - - -- +
  |L | D| F|  Reserved |
  + - - - - - - - - -  +

---------------------------------------------------------------------------*/

#define DPL_IID_RESERVED_MASK (0x1F)

#define IS_DPL_IID_RESERVED_NONZERO(iid)         \
  ((iid).dpl_flags & (DPL_IID_RESERVED_MASK))

/*---------------------------------------------------------------------------
  Flow bit (Which of flow and network logging is enabled?)

typedef enum
{
  DPL_IID_FLOW_BIT_MIN      = 0,
  DPL_IID_FLOW_BIT_NONFLOW  = 0,
  DPL_IID_FLOW_BIT_FLOW     = 1,
  DPL_IID_FLOW_BIT_WIDTH    = 1        Ensures that enum occupies 1 bit
} dpl_iid_flow_bit_enum_type;
---------------------------------------------------------------------------*/
#define DPL_IID_FLOW_MASK (0x20)

#define SET_DPL_IID_F_BIT_FLOW(iid)              \
  ((iid).dpl_flags |= (DPL_IID_FLOW_MASK))

#define SET_DPL_IID_F_BIT_NONFLOW(iid)           \
  ((iid).dpl_flags &= ~(DPL_IID_FLOW_MASK))

#define IS_DPL_IID_F_BIT_FLOW(iid)               \
  ((iid).dpl_flags & (DPL_IID_FLOW_MASK))

#define IS_DPL_IID_F_BIT_NONFLOW(iid)            \
  (!IS_DPL_IID_F_BIT_FLOW(iid))

/*---------------------------------------------------------------------------
  D bit (Direction)

typedef enum
{
  DPL_IID_DIR_MIN   = 0,
  DPL_IID_DIR_RX    = 0,
  DPL_IID_DIR_TX    = 1,
  DPL_IID_DIR_WIDTH = 1                Ensures that enum occupies 1 bit
} dpl_iid_d_bit_enum_type;
---------------------------------------------------------------------------*/
#define DPL_IID_DIRECTION_MASK (0x40)

#define SET_DPL_IID_DIR_TX(iid)                  \
  ((iid).dpl_flags |= (DPL_IID_DIRECTION_MASK))

#define SET_DPL_IID_DIR_RX(iid)                  \
  ((iid).dpl_flags &= ~(DPL_IID_DIRECTION_MASK))

#define IS_DPL_IID_DIR_TX(iid)                   \
  ((iid).dpl_flags & (DPL_IID_DIRECTION_MASK))

#define IS_DPL_IID_DIR_RX(iid)                   \
  (!IS_DPL_IID_DIR_TX(iid))


/*---------------------------------------------------------------------------
  L bit (Is link level logging enabled?)

typedef enum
{
  DPL_IID_L_BIT_MIN      = 0,
  DPL_IID_L_BIT_NETWORK  = 0,
  DPL_IID_L_BIT_LINK     = 1,
  DPL_IID_L_BIT_WIDTH    = 1           Ensures that enum occupies 1 bit
} dpl_iid_l_bit_enum_type;
---------------------------------------------------------------------------*/
#define DPL_IID_LINK_MASK (0x80)

#define SET_DPL_IID_L_BIT_LINK(iid)              \
  ((iid).dpl_flags |= (DPL_IID_LINK_MASK))

#define SET_DPL_IID_L_BIT_NETWORK(iid)           \
  ((iid).dpl_flags &= ~(DPL_IID_LINK_MASK))

#define IS_DPL_IID_L_BIT_LINK(iid)               \
  ((iid).dpl_flags & (DPL_IID_LINK_MASK))

#define IS_DPL_IID_L_BIT_NETWORK(iid)            \
  (!IS_DPL_IID_L_BIT_LINK(iid))
  
/*===========================================================================
MACRO   DPL_RESET_IFACE_LOG_DROP_COUNT

DESCRIPTION
  Reset the dropped count of the iface for log packets

PARAMETERS
  ps_iface_ptr : pointer to ps_iface

RETURN VALUE
   None
===========================================================================*/

#define DPL_RESET_IFACE_LOG_DROP_COUNT(ps_iface_ptr)                            \
  ps_iface_ptr->dpl_net_cb.dropped_count = 0



/*===========================================================================
MACRO   DPL_UPDATE_IFACE_INFO

DESCRIPTION
  Reset the dropped count of the iface for log packets

PARAMETERS
  ps_iface_ptr : pointer to ps_iface

RETURN VALUE
   None
===========================================================================*/

#define DPL_UPDATE_IFACE_INFO(dpl_iface_info_ptr, iface_ptr)                            \
  {                                                                                     \
    dpl_iface_info_ptr->iface_id =  iface_ptr->dpl_net_cb.recv_dpl_id.ifname;           \
    memscpy(dpl_iface_info_ptr->iface_desc,                                             \
        DPL_IFNAME_DESC_S_LEN,                                                          \
        iface_ptr->dpl_net_cb.desc,                                                     \
        DPL_IFNAME_DESC_S_LEN);                                                         \
  }
/*===========================================================================

                                   VARIABLES

===========================================================================*/
/*---------------------------------------------------------------------------
  Enum used for IID Interface names
---------------------------------------------------------------------------*/
typedef enum
{
  /* Value 0 is reserved and must not be defined or used
  DPL_IID_IFNAME_RESERVED                   = 0                         */
  DPL_IID_IFNAME_MIN                        = 1,
  DPL_IID_IFNAME_CDMA_SN_IFACE_1            = 1,
  DPL_IID_IFNAME_CDMA_SN_IFACE_2            = 2,
  DPL_IID_IFNAME_CDMA_SN_IFACE_3            = 3,
  DPL_IID_IFNAME_CDMA_SN_IFACE_4            = 4,
  DPL_IID_IFNAME_CDMA_SN_IFACE_5            = 5,
  DPL_IID_IFNAME_CDMA_SN_IFACE_6            = 6,
  DPL_IID_IFNAME_CDMA_SN_IFACE_7            = 7,
  DPL_IID_IFNAME_CDMA_SN_IFACE_8            = 8,
  DPL_IID_IFNAME_CDMA_SN_IFACE_9            = 9,
  DPL_IID_IFNAME_CDMA_SN_IFACE_10           = 10,
  DPL_IID_IFNAME_CDMA_SN_IFACE_11           = 11,
  DPL_IID_IFNAME_CDMA_SN_IFACE_12           = 12,
  DPL_IID_IFNAME_CDMA_SN_IFACE_13           = 13,
  DPL_IID_IFNAME_CDMA_SN_IFACE_14           = 14,
  DPL_IID_IFNAME_CDMA_SN_IFACE_15           = 15,
  DPL_IID_IFNAME_CDMA_SN_IFACE_16           = 16,
  DPL_IID_IFNAME_CDMA_SN_IFACE_ANY_PKT      = 17,
  DPL_IID_IFNAME_CDMA_SN_IFACE_ASYNC        = 18,
  DPL_IID_IFNAME_CDMA_AN_IFACE              = 19,
  DPL_IID_IFNAME_SIO_IFACE_UMTS_1           = 20,
  DPL_IID_IFNAME_SIO_IFACE_CDMA             = 21,
  DPL_IID_IFNAME_CDMA_BCAST_IFACE           = 22,
  DPL_IID_IFNAME_IPSEC_IFACE                = 23,
  DPL_IID_IFNAME_LO_V4_IFACE                = 24,
  DPL_IID_IFNAME_LO_V6_IFACE                = 25,
  DPL_IID_IFNAME_DVBH_V4_IFACE              = 26,
  DPL_IID_IFNAME_DVBH_V6_IFACE              = 27,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_1          = 28,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_2          = 29,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_3          = 30,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_4          = 31,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_5          = 32,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_6          = 33,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_7          = 34,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_8          = 35,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_9          = 36,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_10         = 37,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_11         = 38,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_12         = 39,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_13         = 40,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_14         = 41,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_15         = 42,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_16         = 43,
  DPL_IID_IFNAME_UICC_IFACE                 = 44,
  DPL_IID_IFNAME_IWLAN_3GPP2_IFACE          = 45,
  DPL_IID_IFNAME_IWLAN_3GPP_IFACE_PDP_0     = 46,
  DPL_IID_IFNAME_IWLAN_3GPP_IFACE_PDP_1     = 47,
  DPL_IID_IFNAME_IWLAN_3GPP_IFACE_PDP_2     = 48,
  DPL_IID_IFNAME_STA_IFACE                  = 49,
  DPL_IID_IFNAME_UW_FMC_IFACE               = 50,
  DPL_IID_IFNAME_PROXY_IFACE_0              = 51,
  DPL_IID_IFNAME_PROXY_IFACE_1              = 52,
  DPL_IID_IFNAME_PROXY_IFACE_2              = 53,
  DPL_IID_IFNAME_PROXY_IFACE_3              = 54,
  DPL_IID_IFNAME_PROXY_IFACE_4              = 55, 
  DPL_IID_IFNAME_3GPP_CONTEXT_0             = 56,
  DPL_IID_IFNAME_3GPP_CONTEXT_1             = 57,
  DPL_IID_IFNAME_3GPP_CONTEXT_2             = 58,
  DPL_IID_IFNAME_3GPP_CONTEXT_3             = 59,
  DPL_IID_IFNAME_3GPP_CONTEXT_4             = 60,
  DPL_IID_IFNAME_3GPP_CONTEXT_5             = 61,
  DPL_IID_IFNAME_3GPP_CONTEXT_6             = 62,
  DPL_IID_IFNAME_3GPP_CONTEXT_7             = 63,
  DPL_IID_IFNAME_3GPP_CONTEXT_8             = 64,
  DPL_IID_IFNAME_3GPP_CONTEXT_9             = 65,
  DPL_IID_IFNAME_3GPP_CONTEXT_10            = 66,
  DPL_IID_IFNAME_3GPP_CONTEXT_11            = 67,
  DPL_IID_IFNAME_3GPP_CONTEXT_12            = 68,
  DPL_IID_IFNAME_3GPP_CONTEXT_13            = 69,
  DPL_IID_IFNAME_3GPP_CONTEXT_14            = 70,
  DPL_IID_IFNAME_3GPP_CONTEXT_15            = 71,
  DPL_IID_IFNAME_3GPP_EMERGENCY_0           = 72,
  DPL_IID_IFNAME_3GPP_EMERGENCY_1           = 73,
  DPL_IID_IFNAME_NAT_IFACE_1                = 74,
  DPL_IID_IFNAME_WLAN_IFACE                 = 75,
  DPL_IID_IFNAME_WLAN_IFACE_ADHOC           = 76,
  DPL_IID_IFNAME_WLAN_IFACE_SOFTAP          = 77,
  DPL_IID_IFNAME_TLB_RM_IFACE_V4            = 78,
  DPL_IID_IFNAME_TLB_RM_IFACE_V6            = 79,
  DPL_IID_IFNAME_EMBMS_IFACE                = 80,  /**< IID for EMBMS Interface*/ 
  DPL_IID_IFNAME_SOFTAP_V6_IFACE_1          = 81,
  DPL_IID_IFNAME_MODEM_LINK_LOCAL_V6_IFACE  = 82,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_1          = 83,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_2          = 84,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_3          = 85,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_4          = 86,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_5          = 87,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_6          = 88,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_7          = 89,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_8          = 90,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_9          = 91,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_10         = 92,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_11         = 93,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_12         = 94,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_13         = 95,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_14         = 96,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_15         = 97,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_16         = 98,  
  DPL_IID_IFNAME_SIO_IFACE_UMTS_2           = 99,
  DPL_IID_IFNAME_CLAT_IFACE_1               = 100,
  DPL_IID_IFNAME_CLAT_IFACE_2               = 101,
  DPL_IID_IFNAME_CLAT_IFACE_3               = 102,
  DPL_IID_IFNAME_CLAT_IFACE_4               = 103,
  DPL_IID_IFNAME_CLAT_IFACE_5               = 104,
  DPL_IID_IFNAME_CLAT_IFACE_6               = 105,
  DPL_IID_IFNAME_CLAT_IFACE_7               = 106,
  DPL_IID_IFNAME_CLAT_IFACE_8               = 107,
  DPL_IID_IFNAME_SIO_IFACE_UMTS_3           = 108,
  DPL_IID_IFNAME_SIO_IFACE_UMTS_4           = 109,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_17         = 110,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_18         = 111,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_19         = 112,
  DPL_IID_IFNAME_SIO_IFACE_RMNET_20         = 113,

 /* IID 114 - 121 Range Reseve for ThreadX, 4 PDN*/
  DPL_IID_IFNAME_RESERVED_1                 = 114,
  DPL_IID_IFNAME_RESERVED_2                 = 115,
  DPL_IID_IFNAME_RESERVED_3                 = 116,
  DPL_IID_IFNAME_RESERVED_4                 = 117,
  DPL_IID_IFNAME_RESERVED_5                 = 118,
  DPL_IID_IFNAME_RESERVED_6                 = 119,
  DPL_IID_IFNAME_RESERVED_7                 = 120,
  DPL_IID_IFNAME_RESERVED_8                 = 121,

  DPL_IID_IFNAME_V2X_IFACE_1                = 122,
  DPL_IID_IFNAME_V2X_IFACE_2                = 123,
  DPL_IID_IFNAME_V2X_IFACE_3                = 124,
  DPL_IID_IFNAME_V2X_IFACE_4                = 125,

  DPL_IID_IFNAME_3GPP_EMERGENCY_2           = 126,
  DPL_IID_IFNAME_3GPP_EMERGENCY_3           = 127,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_17         = 128,
  DPL_IID_IFNAME_IWLAN_S2B_IFACE_18         = 129,

  DPL_IID_IFNAME_IWLAN_IKE_PROXY_V4         = 130,
  DPL_IID_IFNAME_IWLAN_IKE_PROXY_V6         = 131,

  DPL_IID_IFNAME_MAX,
  DPL_IID_IFNAME_WIDTH                  = 0xFF /* Ensures that enum
                                                  occupies 8 bits          */
} dpl_iid_ifname_enum_type;

/*---------------------------------------------------------------------------
  When link bit is DPL_IID_L_BIT_NETWORK, the protocol field uses this enum
---------------------------------------------------------------------------*/
typedef enum
{
  /* Value 0 is reserved and must not be used
  DPL_IID_NETPROT_RESERVED      = 0                                         */
  DPL_IID_NETPROT_MIN           = 1,
  DPL_IID_NETPROT_IP            = 1,
  DPL_IID_NETPROT_HDLC_UNFRAMED = 2,
  DPL_IID_NETPROT_MAX,
  DPL_IID_NETPROT_WIDTH         = 0xFF /* Ensures that enum occupies 8 bits */
} dpl_iid_netprot_enum_type;

/*---------------------------------------------------------------------------
  When link bit is DPL_IID_L_BIT_LINK, the protocol field uses this enum
---------------------------------------------------------------------------*/
typedef enum
{
  DPL_IID_LINKPROT_MIN          = 0,
  DPL_IID_LINKPROT_WILDCARD     = 0,
  DPL_IID_LINKPROT_ETHERNET     = 1,
  DPL_IID_LINKPROT_PPP_IN_HDLC  = 2,
  DPL_IID_LINKPROT_ROHC_COMP_IP = 3,
  DPL_IID_LINKPROT_IPHC_COMP_IP = 4,
  DPL_IID_LINKPROT_MAX,
  DPL_IID_LINKPROT_WIDTH        = 0xFF /* Ensures that enum occupies 8 bits */
} dpl_iid_linkprot_enum_type;

/*---------------------------------------------------------------------------
  Data Protocol Logging Status
---------------------------------------------------------------------------*/
  typedef enum
  {
    DPL_DISABLED           = 0,
    DPL_ENABLED_PARTIAL    = 1,
    DPL_ENABLED_FULL       = 2,
    DPL_ENABLED_IP_HEADER  = 3
  } dpl_state_enum_type;

/*---------------------------------------------------------------------------
  Data Protocol Logging 
---------------------------------------------------------------------------*/
  typedef enum
  {
    DPL_META_INFO_NONE          = 0,
    DPL_META_INFO_SKIP_HDR_BYTE = 1,
    DPL_META_SIO_CONFIG         = 2,
  } dpl_meta_info_type;

/*---------------------------------------------------------------------------
 Iface type
---------------------------------------------------------------------------*/
  typedef enum
  {
    DPL_INVALID_TYPE         = 0,
    DPL_V4_UM_IFACE_TYPE     = 1,
    DPL_V6_UM_IFACE_TYPE     = 2,
    DPL_V4_RM_IFACE_TYPE     = 3,
    DPL_V6_RM_IFACE_TYPE     = 4
  } dpl_iface_type_enum_type;
/*---------------------------------------------------------------------------
  IID data structure.
---------------------------------------------------------------------------*/
typedef struct
{
  unsigned int                   dpl_flags:8;
  unsigned int                   ifname:8;
  unsigned int                   protocol:8;
  unsigned int                   link_instance:8;
} dpl_iid_type;

/*---------------------------------------------------------------------------
  ROHC and IPHC Headers for DPL logging
---------------------------------------------------------------------------*/
typedef struct
{
  byte   ppp_flag;
  byte   ppp_prot[2];
} dpl_comp_header_type;

/*---------------------------------------------------------------------------
  Link logging control block for Tx and Rx directions

    - partial_bytes_logged : records the number of bytes in a packet logged
                             so far
    - snaplen              : partial logging length for link protocols
    - mask                 : A set bit implies that logging is enabled for
                             corresponding protocol
    - seq_num              : Sequence number sent in last log packet
    - dpl_id               : DPL IID of this Physlink. Protocol field must
                             always be 0
    - is_logged            : Indicates if logging is enables or not

  There are two iid's in the link control block so resets can be done easily
  and are consistent over time.

  This is not featurized so ROHC and IPHC libraries will build if logging is
  turned off
---------------------------------------------------------------------------*/
typedef struct
{
  uint32   partial_bytes_logged;
  uint32   snaplen;
  uint16   seq_num[DPL_IID_LINKPROT_MAX];
  uint16   seg_num[DPL_IID_LINKPROT_MAX];
  boolean  is_logged;
} dpl_link_dir_cb_type;

typedef struct
{
  dpl_link_dir_cb_type  tx_cb;
  dpl_link_dir_cb_type  recv_cb;
  dpl_iid_type          tx_dpl_id;
  dpl_iid_type          recv_dpl_id;
  char                  desc[DPL_LINK_DESC_S_LEN];
  uint16                dropped_count;
} dpl_link_cb_type;

/*---------------------------------------------------------------------------
  Network logging control block for Tx and Rx directions
    - snaplen                    : partial logging length for network protocols
    - mask                       : A set bit implies that logging is enabled
                                   for corresponding protocol (see netprot enum)
    - seq_num                    : Sequence number sent in last log packet
    - is_logging_enabled         : Indicates if logging is enabled or not
---------------------------------------------------------------------------*/
typedef struct
{
  uint32        mask;
  uint32        snaplen[DPL_IID_NETPROT_MAX];
  uint16        seq_num[DPL_IID_NETPROT_MAX];
  boolean       is_logging_enabled;
} dpl_net_dir_cb_type;
/*---------------------------------------------------------------------------
   Iface configuration
    - iface_type           : Iface type(Um/Rm) and family(v4/v6)
    - iface_id             : A number associated  with iface
    - iface_desc           : Iface description
---------------------------------------------------------------------------*/
typedef struct
{
  uint8     iface_type;
  uint8     iface_id;
  uint8     ep_id;
  char      iface_desc[DPL_IFNAME_DESC_S_LEN+1];
}dpl_iface_config;

/*---------------------------------------------------------------------------
  Network logging control block
    - mux_id               : RmNet mux id.
    - num_valid_ifaces     : Number of valid iface in ifaces_list
    - apn_desc             : APN name
    - ifaces_list          : Ifaces list associated  with PDN 
---------------------------------------------------------------------------*/
typedef struct
{
  uint8              mux_id;    
  uint8              num_valid_ifaces;
  byte               apn_desc[DPL_APN_DESC_S_LEN+1];
  dpl_iface_config   ifaces_list[DPL_NUM_IFACES_PER_PDN];
}dpl_sio_config;

/*---------------------------------------------------------------------------
  LOGGING OF FLOW CONTROL EVENTS FOR A CALL
  LOG_PKT_EVENT_CODE : PS_DPM_LOG_FC_EV
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8    version;        /* version */
  uint8    fc_state;       /* 1 : disabled  0 : enabled */
  uint32   fc_flow_mask;      /* Mask (FC reason) associated with PS Flow */
  uint32   fc_phys_link_mask; /* Mask (FC reason) associated with PS Physlink */
  uint32   fc_iface_mask;     /* Mask (FC reason) associated with PS iface */  
  uint32   ep_id;             /* End point id for given call */
  uint8    mux_id;         /* Mux id associated with the call */
  uint8    rmnet_inst;     /* Rmnet instance associated with the call */
  uint8    subs_id;        /* Subscription id associated with the call */
  uint32   profile_id;      /* Profile id associated with the call */
  uint32   addr_family;    /* Address Family associated with the call*/
  uint32   v4_iface_name_um;  /* Um base iface name of the call */
  uint8    v4_iface_inst_um;  /* Um base iface instance of the call */
  uint32   v6_iface_name_um;  /* Um base iface name of the call */  
  uint8    v6_iface_inst_um;  /* Um base iface instance of the call */
  uint8    eps_id;         /* EPS Id associated with call */
  uint32   phys_link_ptr;  /* Phys link ptr */
  uint32   flow_ptr;       /* Flow ptr  */
  uint32   allowed_num_bytes;  /*allowed number of bytes in this flow control trigger*/
} dpl_log_fc_ev_type;                                          

/*---------------------------------------------------------------------------
  LOG PKT FOR FLOW CONTROL CONFIGURATION FOR A CALL
  LOG_PKT_CODE : PS_DPM_LOG_CFG_FC_LOGCODE
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8    version;
  uint32   ep_id;           /* End point id for given call */
  uint8    mux_id;          /* Mux id associated with the call */
  uint8    subs_id;        /* Subscription id associated with the call */  
  uint8    rmnet_inst;      /* Rmnet instance associated with the call */
  uint8    eps_id;        /* EPS Id for the bearer */  
  uint32   addr_family;    /* Address Family associated with the call*/
  uint32   iface_name_um;   /* Um iface name of the call */
  uint8    iface_inst_um;   /* Um iface instance of the call */
  uint8    is_qmap_fc_enabled;     /* if qmap fc is enabled or disabled */
  uint8    is_qmi_qos_fc_enabled;  /* if qos fc is enabled or disabled */
  uint8    powersave_mode;  /* if AP powersave mode is on or off*/
} dpl_log_pkt_cfg_fc_type;

/*---------------------------------------------------------------------------
  LOG PKT FOR CONFIGURATION OF UPLINK DATA PATH
  LOG_PKT_CODE : PS_DPM_LOG_CFG_UL_DATA_PATH_LOGCODE
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8    version;
  uint32   ep_id;          /* End point id for given call */
  uint8    mux_id;       /* Mux id associated with the call */
  uint8    subs_id;        /* Subscription id associated with the call */  
  uint8    rmnet_inst;   /* Rmnet instance associated with the call */
  uint16   ul_data_path; /* Data path pipe set for the uplink path*/  
  uint32   v4_iface_name_um; /* v4 Um iface name of the call */
  uint8    v4_iface_inst_um; /* v4 Um iface instance of the call */   
  uint32   v6_iface_name_um; /* v6 Um iface name of the call */
  uint8    v6_iface_inst_um; /* v6 Um iface instance of the call */
  uint32   profile_id;      /* Profile id associated with the call */
} dpl_log_pkt_cfg_ul_data_path_type;

/*---------------------------------------------------------------------------
  LOG PKT FOR CONFIGURATION OF DOWNLINK DATA PATH
  LOG_PKT_CODE :
---------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST
{
  uint8    version;
  uint32   ep_id;          /* End point id for given call */
  uint8    mux_id;       /* Mux id associated with the call */
  uint8    subs_id;        /* Subscription id associated with the call */  
  uint8    rmnet_inst;   /* Rmnet instance associated with the call */
  uint32   ps_phys_link_ptr;     /* Phys link associated with call */
  uint8    is_bridged;    /* Data path in bridged or unbridged state*/  
  uint32   v4_iface_name_um; /* v4 Um iface name of the call */
  uint8    v4_iface_inst_um; /* v4 Um iface instance of the call */   
  uint32   v6_iface_name_um; /* v6 Um iface name of the call */
  uint8    v6_iface_inst_um; /* v6 Um iface instance of the call */
  uint32   profile_id;      /* Profile id associated with the call */
} dpl_log_pkt_cfg_dl_data_path_type;

#endif /* PS_LOGGING_DEFS_H */
