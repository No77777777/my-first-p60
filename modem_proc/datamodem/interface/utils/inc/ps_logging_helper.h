#ifndef PS_LOGGING_HELPER_H
#define PS_LOGGING_HELPER_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     P S _ L O G G I N G _ H E L P E R . H

GENERAL DESCRIPTION

  Contains the helper routines to log data.

SUBSYSTEM INTERNAL FUNCTIONS
  DPLI_LOG_FULL_PACKET()
    Used to log full packet

Copyright (c) 2004-2010 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

 $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/inc/ps_logging_helper.h#3 $
 $Author: pwbldsvc $ $DateTime: 2021/02/09 01:09:33 $

when          who    what, where, why
--------      ---    -------------------------------------------------------- 
11/30/11      su     Adding log code support for DPL logging 
07/09/10      sy     Fixed type-punned compiler warnings.
07/23/09      pp     Removed diagtune.h from inclusions.
11/13/07      pp     Lint medium/low - fixes.
08/31/06      mjp    Added IPHC and ROHC Logging
06/05/06      mp     Added DPL PPP Partial Logging
05/12/05      ks     fixed lint errors
01/27/05      ssh    Changed include file name dsbyte.h to ps_byte.h
01/12/05      msr    Added code review comments.
01/10/05      ks     Fixed DPLI_LOG_LINK_FULL_PACKET macro.
01/10/05      msr    Added support for flow and link logging.
11/16/04    msr/ks   Fixed the offset_ptr parameter passed to
                     dsm_seek_extract() in DPLI_BUILD_LOG_PACKET macro.
11/09/04    msr/ks   Fixed DPLI_BUILD_LOG_PACKET to take values rather than
                     pointers.
10/31/04    msr/ks   Major Clean up. Also created ps_logging_helper.c file to
                     contain the code for dpli_log_frames() which is defined
                     here.
09/13/04    msr/ks   Created the module.
===========================================================================*/

/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "customer.h"

#include "log.h"
#include "err.h"
#include "log_codes.h"
#include "dsm.h"
#include "diagdiag.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"

#include "ps_byte.h"

#pragma clang diagnostic pop


#include "ps_logging_defs.h"
#include "ps_phys_link.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_dpm_hw.h"



/*===========================================================================

                               CONSTANTS

===========================================================================*/
/*---------------------------------------------------------------------------
  Size of header added by DIAG subsytem to each log packet
---------------------------------------------------------------------------*/
#define LOG_HDR_SIZE  sizeof(log_hdr_type)

/*---------------------------------------------------------------------------
  Size of header added by PS to each log packet
---------------------------------------------------------------------------*/
#define DPL_LOG_HDR_SIZE  8

/*---------------------------------------------------------------------------
  Total size of all headers, visible to PS, that are included in log packet
---------------------------------------------------------------------------*/
#define DPL_TOTAL_HDR_SIZE  (LOG_HDR_SIZE + DPL_LOG_HDR_SIZE)

/*---------------------------------------------------------------------------
  Maximum data bytes from the packet to be logged that can be carried
  in each log packet
---------------------------------------------------------------------------*/
#define MAX_PAYLOAD  256

/*---------------------------------------------------------------------------
Iface description buffer is 64 Bytes long to accomodate 32 byte long iface description and 
32 bytes for APN name(optimistic length)
---------------------------------------------------------------------------*/
#define  MAX_DPL_DESC_LEN  64


#define MAX_PDN_INFO_SIZE sizeof(dpl_sio_config)
#define MAX_DPL_HDR_INFO_SIZE 4

/*--------------------------------------------------------------------------
  Maximum data bytes to be logged in DS_INTERFACE_LOG_MSG_INFO.
  The DS_LOG_MAX_PAYLOAD is a hypothetical maximum. If any log message content
  exceeds 240 bytes, content needs to be reworked.  
---------------------------------------------------------------------------*/
#define DS_LOG_MAX_PAYLOAD                 240
#define DS_LOG_CATEGORY_MAX_LEN            1
#define DS_LOG_MAX_VERSION_ID_LEN          1
#define DS_LOG_MSG_VER_ID                  1
#define DS_LOG_MSG_MAX_FILE_LEN            45
#define DS_LOG_MSG_MAX_LINE_LEN            5

/*--------------------------------------------------------------------------
   This is the type that is used to define different category for ds interface
   log messages. This is to be used in DS_INTERFACE_LOG_MSG_INFO.
---------------------------------------------------------------------------*/
typedef enum
{
  PS_LOGGING_CAT_SYS_CONF    = 1,  
  PS_LOGGING_CAT_QMI         = 2, 
  PS_LOGGING_CAT_PS_IFACE    = 3,  
  PS_LOGGING_CAT_DS_PROFILE  = 4,
  PS_LOGGING_CAT_DSNET       = 5,
  PS_LOGGING_CAT_SOCKET      = 6,
} ps_logging_category_enum_type;

/*===========================================================================

                                VARIABLES

===========================================================================*/

extern dpl_state_enum_type dpl_state;

/*===========================================================================

                          HELPER FUNCTIONS

===========================================================================*/

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION    DPLI_LOG_FULL_PACKET

DESCRIPTION
  Logs a full packet, potentially splitting it to multiple segments

PARAMETERS
  item_ptr      : pointer to the dsm chain to be logged
  len           : length of the packet to be logged
  iid           : IID of the data log point
  frame_seq_num : sequence number for the packet being logged

RETURN VALUE
  TRUE  : packet was logged
  FALSE : packet was not logged

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpli_log_full_packet
(
  dsm_item_type  * item_ptr,
  uint32           len,
  dpl_iid_type     iid,
  uint16           frame_seq_num
);

/*===========================================================================
FUNCTION    DPLI_LOG_PPP_PARTIAL_PACKET

DESCRIPTION
  Logs a packet potentially only partially and possibly by splitting it
  to multiple segments

PARAMETERS
  item_ptr          : pointer to the dsm chain to be logged
  log_flag_link_cb  : pointer to the ppp link layer control block

RETURN VALUE
  TRUE  : partial packet was logged
  FALSE : partial packet was not logged

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dpli_log_ppp_partial_packet
(
  const dsm_item_type   *  item_ptr,
  dpl_iid_type             dpl_id,
  dpl_link_dir_cb_type  *  log_flag_link_cb
);

/*===========================================================================
FUNCTION    DPLI_fill_apn_info

DESCRIPTION
  Reterive APN info from 3GPP and 3GPP2 profile and fill into log pointer

PARAMETERS

  log_ptr               : log pointer
  space_left          : remain spec in log pointer
  bridged_iface_ptr : bridged iface ptr(will be used in case of RM ifaces) 

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
uint8 * dpl_fill_apn_info
(
  ps_iface_type  * this_iface_ptr,
  uint8          * log_ptr,
  uint8            size
);


/*===========================================================================
FUNCTION    DPLI_LOG_IFACE_DESC_AND_STATUS

DESCRIPTION
  Logs a packet with iface status(UP/DOWN) and iface description

PARAMETERS
  this_iface_ptr      : logged this iface description
  bridged_iface_ptr : bridged iface ptr(will be used in case of RM ifaces) 

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/


void dpli_log_iface_desc_and_status
(
  ps_iface_type                   *this_iface_ptr,
  ps_iface_type                   *bridged_iface_ptr
);

/*===========================================================================
FUNCTION    DPLI_GET_IP_HEADER_LENGTH

DESCRIPTION
  Provides the length of the IP header of the input packet

PARAMETERS
  item_ptr          : pointer to the dsm chain

RETURN VALUE
  header_length     : length of the IP header in the packet

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
uint32 dpli_get_ip_header_length
(
  dsm_item_type  * item_ptr
);

/*===========================================================================
FUNCTION    DPL_LOG_SIO_DESC_AND_STATUS

DESCRIPTION
  Log a packet with sio info(mux id) and iface info

PARAMETERS
  dpl_pdn_info      :  PDN specific info
  meta_info_type    :  Log type mask

RETURN VALUE
  TRUE  : logged
  FALSE : falied to log

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpl_log_sio_desc_and_status
(
  dpl_sio_config       *dpl_sio_info_ptr,
  uint8                 meta_info_type
);


/*===========================================================================
FUNCTION    DPL_LOG_SKIP_HDR_BYTES

DESCRIPTION
  Log a packet with number of bytes to be skipped in IP packet processing

PARAMETERS
  mux_id      :  MUX ID (PDN)
  src_endp    :  Source end point
  hdr_bytes   :  Number of bytes
  direction   :  Direction 

RETURN VALUE
  TRUE  : logged
  FALSE : falied to log

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean dpl_log_skip_hdr_bytes
(
  uint8                mux_id,
  uint8                src_endp,
  uint8                hdr_bytes,
  uint8                direction
);

/*===========================================================================
FUNCTION    DS_INTERFACE_LOGGING_MSG

DESCRIPTION
  Replace frequent and important date services log messages using log packet
  to reduce the frquency and count of log messages. Payload should contain 
  formated string only. All arguments should be placed in payload before 
  invoking this API. 

PARAMETERS
  file                            :  file name
  line                            :  line number
  ps_logging_category_enum_type   :  category for log packet
  payload                         :  content of the message 

RETURN VALUE
  None

DEPENDENCIES
  Payload should be formated

SIDE EFFECTS
  None.
===========================================================================*/
void ds_interface_logging_msg
(
  uint8                             file[],
  uint32                            line,
  ps_logging_category_enum_type     category_id,
  uint8                             *payload
);

/*===========================================================================
FUNCTION    DPL_LOG_FC_EVENT_PKT

DESCRIPTION
  This API logs the flow control event with qxdm.

PARAMETERS
  log_ev_info_ptr      :  Flow control logging payload

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dpl_log_fc_event_pkt
(
  dpl_log_fc_ev_type  * log_ev_info_ptr
);

/*===========================================================================
FUNCTION    DPL_LOG_FC_EVENT_PKT

DESCRIPTION
  This API logs the flow control configuration packet with qxdm

PARAMETERS
  log_fc_cfg_pkt_ptr      :  flow control cfg logging payload

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void dpl_log_fc_cfg_pkt
(
  dpl_log_pkt_cfg_fc_type  * log_fc_cfg_pkt_ptr
);

#ifdef FEATURE_DATA_IPA
/*===========================================================================
FUNCTION      PS_IPA_DPL_INIT

DESCRIPTION
  This function initialize IPA DPL module. It needs to be called at power up.

PARAMETERS
  none.

DEPENDENCIES
  none.

RETURN VALUE
  none.

SIDE EFFECTS
  none.
===========================================================================*/
void ps_ipa_dpl_init
(
  void
);

/*===========================================================================
FUNCTION      PS_IPA_DPL_DE_INIT

DESCRIPTION
  This function flushes all packets from the A2 DPL watermark.

PARAMETERS
  none.

DEPENDENCIES
  none.

RETURN VALUE
  none.

SIDE EFFECTS
  none.
===========================================================================*/

void ps_ipa_dpl_de_init
(
  void
);
#endif /* FEATURE_DATA_IPA */

#ifdef FEATURE_DATA_A2
/*===========================================================================
FUNCTION      PS_A2_DPL_INIT

DESCRIPTION   Function to initialize the module. This function needs to be
              called once during ps init. All one time initializations
              will happen in this module.

PARAMETERS    none.

DEPENDENCIES  none.

RETURN VALUE  none.

SIDE EFFECTS  none.
===========================================================================*/

void ps_a2_dpl_init
(
  void
);

/*===========================================================================
FUNCTION      PS_A2_DPL_DE_INIT

DESCRIPTION
  This function flushes all packets from the A2 DPL watermark.

PARAMETERS
  none.

DEPENDENCIES
  none.

RETURN VALUE
  none.

SIDE EFFECTS
  none.
===========================================================================*/

void ps_a2_dpl_de_init
(
  void
);
#endif /* FEATURE_DATA_A2 */
/*===========================================================================

                          HELPER MACROS

===========================================================================*/
/*===========================================================================
MACRO    DPLI_BUILD_LOG_PACKET

DESCRIPTION
  Constructs the log packet by filling in header and data information.

PARAMETERS
  item_ptr_ptr  : pointer to the dsm chain to extract data from
  log_ptr       : Pointer to the log packet
  len_ptr       : Number of bytes to be extracted
  iid           : IID associated with this log packet
  frame_seq_num : Sequence number for this log packet
  seg_num       : Segment number for this log packet
  offset_ptr    : Number of bytes to offset into packet

RETURN VALUE
  NONE
===========================================================================*/
#define DPLI_BUILD_LOG_PACKET(item_ptr_ptr, log_ptr, offset_ptr,         \
                              iid, len_ptr, frame_seq_num, seg_num)      \
  /*lint -save -e740 IID is a 4 byte value but represented in a struct */\
  /*casting to void* to take care of type-punned compiler warning      */\
  log_ptr = put32(log_ptr, *(uint32 *)((void *) &(iid)));                \
  /*lint -restore */                                                     \
  log_ptr = hput16(log_ptr, frame_seq_num);                              \
  log_ptr = hput16(log_ptr, seg_num);                                    \
                                                                         \
  (void) dsm_seek_extract(item_ptr_ptr, offset_ptr, log_ptr, len_ptr);   \
  *offset_ptr += *len_ptr;

/*===========================================================================
MACRO  DPLI_LOG_GET_PACKET_SNAPLEN

DESCRIPTION
  returns length of network protocol packet 

PARAMETERS
  log_flag_net_cb : pointer to logging control block
  proto                : network protocol of packet to be logged
  
RETURN VALUE
  integer length of packet
  Will return 0 when logging is not enabled 
  and logging for protocol is not set
===========================================================================*/
#define DPLI_LOG_GET_PACKET_SNAPLEN(log_flag_net_cb, proto)   \
  ((TRUE == log_flag_net_cb.is_logging_enabled &&           \
     0 != (log_flag_net_cb.mask & (0x1 << (proto - 1))))    \
     ? log_flag_net_cb.snaplen[proto]                     \
     : 0)     
/*===========================================================================
MACRO   DPLI_LOG_NETWORK_PACKET

DESCRIPTION
  Log a network protocol packet (Common for network and flow logging)

PARAMETERS
  log_flag_net_cb : pointer to logging control block
  packet          : packet to be logged
  protocol        : network protocol of packet to be logged
  dropped count       : counter to maintain number of packets dropped 

RETURN VALUE
  TRUE on success
  FALSE otherwise
===========================================================================*/
#define DPLI_LOG_NETWORK_PACKET(log_flag_net_cb, dpl_id, packet, proto, dropped_count)  \
  if (TRUE  == log_flag_net_cb.is_logging_enabled &&                     \
       0 != (log_flag_net_cb.mask & (0x1 << (proto - 1))))              \
  {                                                                      \
    /*-------------------------------------------------------------------\
      Only allocate IID variable if logging is turned on, to impact      \
      less on data path in the common case (logging disabled).           \
    -------------------------------------------------------------------*/\
    dpl_iid_type net_dpl_id;                                             \
                                                                         \
    if (NULL == packet)                                                  \
    {                                                                    \
      LOG_MSG_ERROR_0("NULL item ptr");                                  \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      /*-----------------------------------------------------------------\
        IID is passed at beginning of each DPL log packet. Only protocol \
        field needs to be filled here. Rest of the IID is populated when \
        ps_iface is created in ps_iface_create()                         \
      -----------------------------------------------------------------*/\
      net_dpl_id          = dpl_id;                                      \
      net_dpl_id.protocol = proto;                                       \
                                                                         \
      /*-----------------------------------------------------------------\
        If snaplen is SNAPLEN_ALL log whole packet unless DPL  state     \
        is DPL_ENABLED_IP_HEADER, then log the IP header only            \
      -----------------------------------------------------------------*/\
      if (SNAPLEN_ALL == log_flag_net_cb.snaplen[proto])                 \
      {                                                                  \
        uint32 pkt_len;                                                  \
        /*---------------------------------------------------------------\
          If dpl state is DPL_ENABLED_IP_HEADER, then get the length of  \
          the IP header form the packet                                  \
        ---------------------------------------------------------------*/\
        if (DPL_ENABLED_IP_HEADER == dpl_state)                          \
        {                                                                \
          pkt_len = dpli_get_ip_header_length(packet);                   \
        }                                                                \
        else                                                             \
        {                                                                \
          pkt_len = dsm_length_packet(packet);                           \
        }                                                                \
        if (TRUE != dpli_log_full_packet                                 \
                   (                                                     \
                     packet,                                             \
                     pkt_len,                                            \
                     net_dpl_id,                                         \
                     log_flag_net_cb.seq_num[proto]++                    \
                   ))                                                    \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
      else if (0 == log_flag_net_cb.snaplen[proto])                      \
      {                                                                  \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             dsm_length_packet(packet),                  \
                             net_dpl_id,                                 \
                      log_flag_net_cb.seq_num[proto]++                   \
                    ))                                                   \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
      else                                                               \
      {                                                                  \
        uint32  pkt_len = dsm_length_packet(packet);                     \
        pkt_len = MIN(log_flag_net_cb.snaplen[proto], pkt_len);          \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             pkt_len,                                    \
                             net_dpl_id,                                 \
                      log_flag_net_cb.seq_num[proto]++                   \
                    ))                                                   \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
    }                                                                    \
  }



/*===========================================================================
MACRO   DPLI_LOG_LINK_FULL_PACKET

DESCRIPTION
  Log a full link protocol packet

PARAMETERS
  log_flag_link_cb : pointer to logging control block
  packet           : packet to be logged
  protocol         : link protocol of packet to be logged

RETURN VALUE
  TRUE on success
  FALSE otherwise
===========================================================================*/
#define DPLI_LOG_LINK_FULL_PACKET(log_flag_link_cb, dpl_id, packet,      \
                                  proto,dropped_count)                   \
  if (TRUE == log_status(LOG_DATA_PROTOCOL_LOGGING_C) &&                 \
      TRUE == log_flag_link_cb.is_logged)                                \
  {                                                                      \
    /*-------------------------------------------------------------------\
      Only allocate IID variable if logging is turned on, to impact      \
      less on data path in the common case (logging disabled).           \
    -------------------------------------------------------------------*/\
    dpl_iid_type link_dpl_id;                                            \
    uint32       dsm_pkt_len;                                            \
                                                                         \
    if (NULL == packet)                                                  \
    {                                                                    \
      LOG_MSG_ERROR_0("NULL item ptr");                                  \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      /*-----------------------------------------------------------------\
        IID is passed at beginning of each DPL log packet. Only protocol \
        field needs to be filled here. Rest of the IID is populated when \
        ps_iface is created in ps_iface_create()                         \
      -----------------------------------------------------------------*/\
      link_dpl_id               = dpl_id;                                \
      link_dpl_id.protocol      = proto;                                 \
                                                                         \
      /*-----------------------------------------------------------------\
        If snaplen is SNAPLEN_ALL log whole packet else log snaplen      \
        number of bytes. By doing this partial logging is supported      \
      -----------------------------------------------------------------*/\
      if (SNAPLEN_ALL == log_flag_link_cb.snaplen                        \
	    || 0 == log_flag_link_cb.snaplen)                            \
      {                                                                  \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             dsm_length_packet(packet),                  \
                             link_dpl_id,                                \
                      log_flag_link_cb.seq_num[proto]++                  \
                     ))                                                  \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
      else                                                               \
      {                                                                  \
        dsm_pkt_len = dsm_length_packet(packet);                         \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             MIN(log_flag_link_cb.snaplen, dsm_pkt_len), \
                             link_dpl_id,                                \
                      log_flag_link_cb.seq_num[proto]++                  \
                    ))                                                   \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
    }                                                                    \
  }

/*===========================================================================
MACRO   DPLI_LOG_LINK_FULL_COMP_PACKET

DESCRIPTION
  Log a full link compressed protocol packet. This is needed to increment pkt
  length by one when snaplength is non zero.

PARAMETERS
  log_flag_link_cb : pointer to logging control block
  packet           : packet to be logged
  protocol         : link protocol of packet to be logged
  type             : type of packet being logged

RETURN VALUE
  TRUE on success
  FALSE otherwise
===========================================================================*/
#define DPLI_LOG_LINK_FULL_COMP_PACKET(log_flag_link_cb,                 \
                                       dpl_id, packet,                   \
                                       proto, dropped_count)             \
  if (TRUE == log_status(LOG_DATA_PROTOCOL_LOGGING_C) &&                 \
      TRUE == log_flag_link_cb.is_logged)                                \
  {                                                                      \
    /*-------------------------------------------------------------------\
      Only allocate IID variable if logging is turned on, to impact      \
      less on data path in the common case (logging disabled).           \
    -------------------------------------------------------------------*/\
    dpl_iid_type link_dpl_id;                                            \
                                                                         \
    if (NULL == packet)                                                  \
    {                                                                    \
      LOG_MSG_ERROR_0("NULL item ptr");                                  \
    }                                                                    \
    else                                                                 \
    {                                                                    \
      /*-----------------------------------------------------------------\
        IID is passed at beginning of each DPL log packet. Only protocol \
        field needs to be filled here. Rest of the IID is populated when \
        ps_iface is created in ps_iface_create()                         \
      -----------------------------------------------------------------*/\
      link_dpl_id               = dpl_id;                                \
      link_dpl_id.protocol      = proto;                                 \
                                                                         \
      /*-----------------------------------------------------------------\
        If snaplen is SNAPLEN_ALL log whole packet else log snaplen      \
        number of bytes. By doing this partial logging is supported.     \
        For compressed packet types this is incremented by one so that   \
        pkt                                                              \
        type takes no space                                              \
      -----------------------------------------------------------------*/\
      if (SNAPLEN_ALL == log_flag_link_cb.snaplen)                       \
      {                                                                  \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             dsm_length_packet(packet),                  \
                             link_dpl_id,                                \
                      log_flag_link_cb.seq_num[proto]++                  \
                    ))                                                   \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
      else if (0 == log_flag_link_cb.snaplen)                            \
      {                                                                  \
        if (TRUE != dpli_log_full_packet                                 \
                    (                                                    \
                      packet,                                            \
                             dsm_length_packet(packet),                  \
                             link_dpl_id,                                \
                      log_flag_link_cb.seq_num[proto]++                  \
                    ))                                                   \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
      else                                                               \
      {                                                                  \
        /*---------------------------------------------------------------\
          Note that compressed type has already been added as part of    \
          the DSM chain. snaplen must also consider this extra byte.     \
          note hack re. 65533 - assumes sizeof(dpl_comp_hder) is 3       \
        ---------------------------------------------------------------*/\
        uint32 pkt_len = dsm_length_packet(packet);                      \
        if(log_flag_link_cb.snaplen < 65533)                             \
        {                                                                \
          pkt_len = MIN(log_flag_link_cb.snaplen +                       \
                                 sizeof(dpl_comp_header_type), pkt_len); \
        }                                                                \
        else                                                             \
        {                                                                \
          pkt_len = MIN(log_flag_link_cb.snaplen, pkt_len);              \
        }                                                                \
                                                                         \
        if ( TRUE != dpli_log_full_packet                                \
                     (                                                   \
                       packet,                                           \
                             pkt_len,                                    \
                             link_dpl_id,                                \
                       log_flag_link_cb.seq_num[proto]++                 \
                     ))                                                  \
        {                                                                \
          dropped_count++;                                               \
        }                                                                \
      }                                                                  \
    }                                                                    \
  }

/*===========================================================================
MACRO   DPLI_LOG_PPP_PARTIAL_PACKET

DESCRIPTION
  Partially log a ppp protocol packet

PARAMETERS
  log_flag_link_cb : pointer to logging control block
  dpl_id           : pointer to the DPL ID
  packet           : packet to be logged

RETURN VALUE
  TRUE on success
  FALSE otherwise
===========================================================================*/
#define DPLI_LOG_PPP_PARTIAL_PACKET(log_flag_link_cb, dpl_id, packet)    \
  if (TRUE == log_status(LOG_DATA_PROTOCOL_LOGGING_C) &&                 \
      TRUE == log_flag_link_cb.is_logged)                                \
  {                                                                      \
    /*-------------------------------------------------------------------\
      IID is stored in the Link Logging Control Block in the ps_iface    \
      structure. The IID is populated when ps_iface is created in        \
      ps_iface_create(). The dpl_id.protocol is automatically set to     \
      ppp-hdlc since this may only be called when using ppp.             \
    -------------------------------------------------------------------*/\
                                                                         \
    dpl_id.protocol  =  (unsigned int)DPL_IID_LINKPROT_PPP_IN_HDLC;      \
                                                                         \
    dpli_log_ppp_partial_packet(packet, dpl_id, &log_flag_link_cb);     \
  }

/*===========================================================================
MACRO   PS_LOG_SPACE_LEFT

DESCRIPTION
  Calculate the available space in log packet for logging  

PARAMETERS
  max_payload_size 
  curr_log_ptr
  base_log_ptr

RETURN VALUE
   None
===========================================================================*/
#define PS_LOG_SPACE_LEFT( max_payload_size, curr_log_ptr, base_log_ptr)  \
    (max_payload_size - ( curr_log_ptr - base_log_ptr))

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_0

DESCRIPTION
  Log DS interface message with no argument  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_0( cate_id, fmtString)                     \
{                                                                             \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                         \
  uint8            file[] = __FILENAME__;                                     \
  uint32           line = (uint32)__LINE__;                                   \
/*--------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                            \
  snprintf((char *)&event_payload,DS_LOG_MAX_PAYLOAD, (const char*)fmtString);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);     \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_1

DESCRIPTION
  Log DS interface message with one argument  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_1( cate_id, fmtString, a)                     \
{                                                                                \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                            \
  uint8            file[] = __FILENAME__;                                        \
  uint32           line = (uint32)__LINE__;                                      \
/*-----------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                               \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);        \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_2

DESCRIPTION
  Log DS interface message with two arguments  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_2( cate_id, fmtString, a, b)                     \
{                                                                                   \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                               \
  uint8            file[] = __FILENAME__;                                           \
  uint32           line = (uint32)__LINE__;                                         \
/*--------------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                                  \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a, b);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);           \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_3

DESCRIPTION
  Log DS interface message with three arguments  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_3( cate_id, fmtString, a, b, c)                     \
{                                                                                      \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                                  \
  uint8            file[] = __FILENAME__;                                              \
  uint32           line = (uint32)__LINE__;                                            \
/*-----------------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                                     \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a, b, c);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);              \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_4

DESCRIPTION
  Log DS interface message with four arguments  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_4( cate_id, fmtString, a, b, c, d)                     \
{                                                                                         \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                                     \
  uint8            file[] = __FILENAME__;                                                 \
  uint32           line = (uint32)__LINE__;                                               \
/*--------------------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                                        \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a, b, c, d);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);                 \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_5

DESCRIPTION
  Log DS interface message with five arguments  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_5( cate_id, fmtString, a, b, c, d, e)                     \
{                                                                                            \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                                        \
  uint8            file[] = __FILENAME__;                                                    \
  uint32           line = (uint32)__LINE__;                                                  \
/*-----------------------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                                           \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a, b, c, d, e);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);                    \
}

/*===========================================================================
MACRO   DS_INTERFACE_LOG_MSG_INFO1_6

DESCRIPTION
  Log DS interface message with six arguments  

PARAMETERS
  CATEGORY ID - cate_id
  fmtString - Format String
RETURN VALUE
   None
===========================================================================*/

#define DS_INTERFACE_LOG_MSG_INFO1_6( cate_id, fmtString, a, b, c, d, e, f)                     \
{                                                                                               \
  uint8            event_payload[DS_LOG_MAX_PAYLOAD];                                           \
  uint8            file[] = __FILENAME__;                                                       \
  uint32           line = (uint32)__LINE__;                                                     \
/*--------------------------------------------------------------------------------------------*/\
  memset(&event_payload, 0x0, DS_LOG_MAX_PAYLOAD);                                              \
  snprintf((char *)&event_payload, DS_LOG_MAX_PAYLOAD,(const char*)fmtString, a, b, c, d, e, f);\
  ds_interface_logging_msg(file, line, cate_id , (uint8 *)event_payload);                       \
}

#endif /* PS_LOGGING_HELPER_H */
