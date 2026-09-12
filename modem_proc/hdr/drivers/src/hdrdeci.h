#ifndef HDRDECI_H
#define HDRDECI_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    H D R     D E C O D E R     D R I V E R
                    
                   I N T E R N A L    H E A D E R    F I L E

GENERAL DESCRIPTION
  The Turbo decoder stores some decoder metrics in a packet header, stored
  in the output buffer. More packet information is set by the DSP, and stored
  in a table in DSP memory. This module contains internal Macro
  definitions and declarations to access various fields stored in decoB 
  packet header (2 words) and decode Header stored in DSP memory.
  

MACROS

  
  Copyright (c) 2000 - 2009 by Qualcomm Technologies, Inc.  All Rights Reserved.
  *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrdeci.h#1 $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/08/14   rmv     Updated FL Packet Header log packet to include packet
                   cancelled status and Preamble Detection type 
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
06/06/12   kss     Updated for Dime.
04/21/09   etv     Update for hdrdec_pkt_type_enum_type name change.
08/11/08   etv     Fixed fw/sw interface bug with Min LLR fix.
08/05/08   etv     Read Min LLR result instead of Min LLR.
08/20/07   etv     Changed "carrier_ind" to "demod_ind"
07/20/07   etv     Used header2 to read SlotIndex instead of header 1.
07/11/07   etv     Increased size of pkt_type field to 5 bits and added support
                   for sched_group_id field and concurrent field.
04/01/07   etv     Updated for Rev-B support.
09/14/06   mt      Added macro for forced preamble detection used status.
05/30/06   etv     Changed SlotsRemaining to SlotsToDecode.
10/10/05   jyw     Added mac_index macros.
03/31/05   kss     Added masks for CRC length, requested DRC.
04/26/04   kss     Added several defines for shifting header bits.
03/07/04   kss     Add support for new decoder header bits.
09/03/02   kss     Corrected some comments and moved some defines to hdrdec.c
07/31/02   kss     Added correct defines for min LLR.
03/11/02   kss     Added workaround support for DBIF_PACKET_COUNT misread.
10/23/00   kss     Added optimizations and a profiling mode. 
10/11/00   kss     Code review update; fixed error in header definitions.
08/25/00   kss     Created.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/* Decoder firmware packet header type */
typedef struct
{
  uint32 header0;
  uint32 header1;
  uint32 header2;
} hdrdec_fw_dec_pkt_header_type;

/* ------------------------------------------------------------------------
                 DECODER FRAME HEADER FORMAT & DEFINITIONS
                 
  Mask, value, and shifting information for extracting data from the 
  first word of header data.  Note that _S designates the number of right 
  shifts a bit field needs to be shifted to LSB align the field.
------------------------------------------------------------------------ */

/*==========================================================================
                 DECODER FIRMWARE PACKET HEADER 
==========================================================================*/

/* ------------------------------------------------------------------------
  Header0:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  +=======================+=======================+
  |++| X|+++++++++++|T |C |F |R |++|    Type      |
  +=======================+=======================+
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +=======================+=======================+
  |Concurrent |   Demod   |    Sequence Number    |
  +===============================================+

  Bits    Field Description
  ====    =================
  [30]    ((X == 1)? Packet Cancelled: Otherwise). 
          The contents of the decoder output buffer should be ignored.
  [25]    ((T == 1)? TC Forced Preamble Detection
  [24]    ((C == 1)? Packet has Passed CRC and Minimum LLR check: Otherwise)
  [23]    ((F = 1)? CC Forced Preamble Detection : Normal preamble detection)
  [22]    ((R == 1)? 24-bit CRC: 16-bit CRC)
  [20-16] Packet Type with all possible values shown below (5 bits)

  VALUE         CHANNEL             TYPE
  0x00(b00000)  HDRDEC_FTC_UNICAST  HDRDEC_UNI_PKT (Unicast Packet)
  0x08(b01000)  HDRDEC_FTC_MUP      HDRDEC_MUL_PKT_BASE_0 (Multiuser Rate 1)
  0x09(b01001)  HDRDEC_FTC_MUP      HDRDEC_MUL_PKT_BASE_1 (Multiuser Rate 2)
  0x0A(b01010)  HDRDEC_FTC_MUP      HDRDEC_MUL_PKT_BASE_2 (Multiuser Rate 3)
  0x0B(b01011)  HDRDEC_FTC_MUP      HDRDEC_MUL_PKT_BASE_3 (Multiuser Rate 4)
  0x0C(b01100)  HDRDEC_FTC_MUP      HDRDEC_MUL_PKT_BASE_4 (Multiuser Rate 5)
  0x10(b10000)  HDRDEC_CC           HDRDEC_CC_0_PKT (Control (38.4 or 76.8)
  0x11(b10001)  HDRDEC_CC           HDRDEC_CC_A_PKT (Extended Control)
  0x12(b10010)  HDRDEC_CC           HDRDEC_USER_CC_A_PKT (User-defined Control)
  0x18(b11000)  HDRDEC_BCC          HDRDEC_BCC_PKT (Gold Bcast packet)
  0x19(b11001)  HDRDEC_BCC          HDRDEC_BCC_ENH_PKT (Enhanced Gold Bcast Pkt)
  0x1A(b11010)  HDRDEC_BCC          HDRDEC_PB_PKT (Platinum Bcast packet)

  [11-8]  Demod index on which the packet was received. (4 bits)
  [7-0]   Sequence number for re-ordering packets. (8 bits)

  All other bit positions marked with a '+' sign are reserved.
------------------------------------------------------------------------*/

#define HDRDEC_FW_HEADER0_CANCEL_M                0x40000000
#define HDRDEC_FW_HEADER0_CANCEL_V                0x40000000

#define HDRDEC_FW_HEADER0_TC_FPD_M              0x02000000
#define HDRDEC_FW_HEADER0_TC_FPD_V              0x02000000

#define HDRDEC_FW_HEADER0_CRC_STATUS_M            0x01000000
#define HDRDEC_FW_HEADER0_CRC_PASS_V              0x01000000

#define HDRDEC_FW_HEADER0_FPD_M                   0x00800000
#define HDRDEC_FW_HEADER0_FPD_V                   0x00800000

#define HDRDEC_FW_HEADER0_CRC_LENGTH_M            0x00400000
#define HDRDEC_FW_HEADER0_CRC_LENGTH_24_V         0x00400000

#define HDRDEC_FW_HEADER0_PKT_TYPE_M              0x001F0000
#define HDRDEC_FW_HEADER0_PKT_TYPE_S              16
#define HDRDEC_FW_HEADER0_CHAN_TYPE_S             19 

#define HDRDEC_FW_HEADER0_CONCURRENT_M            0x0000F000
#define HDRDEC_FW_HEADER0_CONCURRENT_S            12

#define HDRDEC_FW_HEADER0_DEMOD_IND_M             0x00000F00
#define HDRDEC_FW_HEADER0_DEMOD_IND_S             8

#define HDRDEC_FW_HEADER0_PKT_SEQ_M               0x000000FF
#define HDRDEC_FW_HEADER0_PKT_SEQ_S               0

/*-------------------------------------------------------------------------
  Header1:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  +=======================+=======================+
  |                 HSTR Count                    |
  +=======================+=======================+
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +=======================+=======================+
  |        Group ID       |       Link ID         |
  +===============================================+

  Bits    Field Description
  ====    =================
  [31-16] The Half-Slot time reference count corresponding to start of the
          packet (16 bits).
  [15-8]  Group ID: The scheduler group ID copied from ASP table for the ASP
          which served this packet.
  [7-0]   The link ID copied from the ASP table for the ASP that served this
          packet (8 bits).

  All other bit positions marked with a '+' sign are reserved.
----------------------------------------------------------------------- */

#define HDRDEC_FW_HEADER1_HSTR_COUNT_M            0xFFFF0000
#define HDRDEC_FW_HEADER1_HSTR_COUNT_S            16

#define HDRDEC_FW_HEADER1_SCHED_GROUP_ID_M        0x0000FF00
#define HDRDEC_FW_HEADER1_SCHED_GROUP_ID_S        8

#define HDRDEC_FW_HEADER1_LINK_ID_M               0x000000FF
#define HDRDEC_FW_HEADER1_LINK_ID_S               0


/*-------------------------------------------------------------------------
  Header2:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +=======================+=======================+
  |++++++++|Requested DRC |++++++++|  Decoded DRC |
  +======================+========================+
  |+++++++++++++++++++++++++++++++++++|Slot Index |
  +===============================================+

  Bits    Field Description
  ====    =================
  [28-24] Data Rate requested. This may differ from decoded DRC in the 
          case of multi-user packets.
  [20-16] Decoded DRC. Data rate of packet.
  [3-0]   Slot index (0..15) at which this packet terminated. Note that 
          with extended preambles, the slot index may be larger than the 
          nominal packet span (4 bits).

  All other bit positions marked with a '+' sign are reserved.
----------------------------------------------------------------------- */

#define HDRDEC_FW_HEADER2_REQUESTED_DRC_M         0x1F000000
#define HDRDEC_FW_HEADER2_REQUESTED_DRC_S         24

#define HDRDEC_FW_HEADER2_DECODED_DRC_M           0x001F0000
#define HDRDEC_FW_HEADER2_DECODED_DRC_S           16

#define HDRDEC_FW_HEADER2_SLOT_INDEX_M            0x0000000F
#define HDRDEC_FW_HEADER2_SLOT_INDEX_S            0 


/*===========================================================================

                            MACRO  DEFINITIONS

===========================================================================*/

/*===========================================================================
MACRO HDRDEC_SEQ_IS_CANCELLED

DESCRIPTION
  Returns whether any sequence values have been cancelled since the last
  packet read.
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  TRUE = There have been cancelled packets.
  FALSE = No packets have been cancelled.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_SEQ_IS_CANCELLED(header)                         \
         (boolean)(((header).header0 &                          \
         HDRDEC_FW_HEADER0_CANCEL_M) == HDRDEC_FW_HEADER0_CANCEL_V)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_PKT_HAS_PASSED_CRC_AND_MIN_LLR

DESCRIPTION
  Returns a boolean value indicating whether the packet passed CRC and 
  Minimum LLR check. 
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  TRUE if the packet has passed CRC and Minimum LLR check.
  FALSE Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_PKT_HAS_PASSED_CRC_AND_MIN_LLR( header )           \
         (boolean)(((header).header0 &                          \
         HDRDEC_FW_HEADER0_CRC_STATUS_M) == HDRDEC_FW_HEADER0_CRC_PASS_V) 

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_PKT_IS_CC_FPD

DESCRIPTION
  Returns TRUE if it is CC forced preamble detection for this packet and 
  FALSE if the preamble detection was normal.

DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  TRUE  = CC FPD.
  FALSE = Normal Preamble detection.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_PKT_IS_CC_FPD( header )               \
         (boolean)(((header).header0 &                            \
         HDRDEC_FW_HEADER0_FPD_M) == HDRDEC_FW_HEADER0_FPD_V)

/*===========================================================================
MACRO HDRDEC_PKT_IS_TC_FPD

DESCRIPTION
  Returns true if the packet started using TC FPD (TRUE if TC FPD and false 
  otherwise).
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  TRUE if TC FPD.
  FALSE Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_PKT_IS_TC_FPD(header)                        \
         (boolean)(((header).header0 &                              \
         HDRDEC_FW_HEADER0_TC_FPD_M) == HDRDEC_FW_HEADER0_TC_FPD_V)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_PKT_IS_REV_A

DESCRIPTION
  Returns whether the decoded packet is Rev-A by checking for the CRC length. 
  24-bit CRC denotes Rev-A packets and 16-bit CRC denotes Rel-0 packets. 
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  TRUE if Packet is Rev-A
  FALSE Otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_PKT_IS_REV_A( header )                           \
         (boolean)(((header).header0 &                          \
         HDRDEC_FW_HEADER0_CRC_LENGTH_M) == HDRDEC_FW_HEADER0_CRC_LENGTH_24_V)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_PKT_TYPE

DESCRIPTION
  Returns the packet type in an enum (hdrdec_pkt_type_e_type)

  Please refer to possible enumerations for packet type listed in header0 
  comment at the top of the file.

DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The packet type.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_PKT_TYPE( header )                        \
          (hdrdec_pkt_type_e_type)(((header).header0 &       \
          HDRDEC_FW_HEADER0_PKT_TYPE_M) >> HDRDEC_FW_HEADER0_PKT_TYPE_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_PKT_CHANNEL

DESCRIPTION
  Return the packet channel in an enum. (CC, FTC, BCC). The Most significant
  two bits of Packet type field in the header can be used to classify 
  channel type as shown below. 

  0x0 (00) - Forward Traffic channel - Unicast.
  0x1 (01) - Forward Traffic Channel (MUP).
  0x2 (10) - Control channel.
  0x3 (11) - Broadcast channel.
    
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The packet channel.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_PKT_CHANNEL( header )                          \
          (hdrdec_pkt_channel_e_type)(((header).header0 &      \
          HDRDEC_FW_HEADER0_PKT_TYPE_M) >> HDRDEC_FW_HEADER0_CHAN_TYPE_S)


/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_NUM_CONCURRENT_DECODES

DESCRIPTION
  The number of packets detected on the same slot, for ordering. Normal 
  range is 1..DEMOD_CARRIERS. If this value is 0, the DSP has not yet 
  detected all carriers; SW shall skip this packet and check it again on 
  the next decoder interrupt 
    
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  Number of Concurrent decodes 

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_NUM_CONCURRENT_DECODES( header )                   \
          (((header).header0 &                                        \
          HDRDEC_FW_HEADER0_CONCURRENT_M) >> HDRDEC_FW_HEADER0_CONCURRENT_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_DEMOD_INDEX

DESCRIPTION
  Returns the Demod Index on which the packet was received.

DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The packet type.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_DEMOD_INDEX( header )                            \
          (((header).header0 &                                      \
          HDRDEC_FW_HEADER0_DEMOD_IND_M) >> HDRDEC_FW_HEADER0_DEMOD_IND_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_SEQUENCE

DESCRIPTION
  Returns the 8-bit sequence number associated with the decoded packet.
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The sequence associated with the packet.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_SEQUENCE(header)                                \
         (((header).header0 &                                      \
         HDRDEC_FW_HEADER0_PKT_SEQ_M) >> HDRDEC_FW_HEADER0_PKT_SEQ_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_PKT_HSTR_COUNT

DESCRIPTION
  Returns the 9-bit half slot time reference (HSTR) associated with the 
  given frame header. 
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The 9-bit half slot time reference (HSTR) associated with the packet.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_PKT_HSTR_COUNT( header )                           \
         (hdrdec_half_slot_time_type)(((header).header1 &             \
         HDRDEC_FW_HEADER1_HSTR_COUNT_M) >> HDRDEC_FW_HEADER1_HSTR_COUNT_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_SLOT_INDEX

DESCRIPTION
  Returns the slot-index (starting at zero) at which the packet terminated. 

  Note: For extended preambles, the slot-index may be larger than nominal
  transmit duration (span).

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type) 
           associated with the frame.

RETURN VALUE
  Slot-index (starting at zero) at which the packet terminated. 
===========================================================================*/
#define HDRDEC_GET_SLOT_INDEX(header)                             \
         (((header).header2 &                                     \
         HDRDEC_FW_HEADER2_SLOT_INDEX_M) >> HDRDEC_FW_HEADER2_SLOT_INDEX_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_SCHED_GROUP_ID

DESCRIPTION
  The scheduler group ID copied from the ASP table for the ASP which served 
  this packet.  

DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The scheduler Group ID

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_SCHED_GROUP_ID(header)                             \
        (((header).header1 & HDRDEC_FW_HEADER1_SCHED_GROUP_ID_M) >>   \
        HDRDEC_FW_HEADER1_SCHED_GROUP_ID_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_LINK_ID

DESCRIPTION
  Returns the 8-bit link id copied from the ASP table for the ASP which 
  served this packet. 
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The link id associated with the packet.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_LINK_ID(header)                    \
         (((header).header1 &                         \
           HDRDEC_FW_HEADER1_LINK_ID_M) >> HDRDEC_FW_HEADER1_LINK_ID_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_REQUESTED_DRC

DESCRIPTION
  Returns the requested DRC rate associated with the received packet.
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The reqeusted DRC rate associated with the received packet

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_REQUESTED_DRC(header)                              \
          (((header).header2 & HDRDEC_FW_HEADER2_REQUESTED_DRC_M) >>  \
          HDRDEC_FW_HEADER2_REQUESTED_DRC_S)

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_DECODED_DRC

DESCRIPTION
  Returns the DRC rate of the packet indicated by the given frame header.
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_fw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The DRC rate at which the frame was received.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_DECODED_DRC( header )                              \
          (((header).header2 & HDRDEC_FW_HEADER2_DECODED_DRC_M) >>    \
           HDRDEC_FW_HEADER2_DECODED_DRC_S)


/*==========================================================================
                 DECODER BUFFER HARDWARE PACKET HEADER 
==========================================================================*/

/* Decoder Buffer hardware packet header type */
typedef struct 
{
  uint32 header0;
  uint32 header1;
  uint32 header2; /* Not used */
  uint32 header3; /* Not used */
} hdrdec_hw_dec_pkt_header_type;

#ifdef FEATURE_HDR_DIME_MODEM

/*-------------------------------------------------------------------------
  Header0:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +==============+================================+
  |C |ET|+++++|Iterations |++++++++| Energy ...   |
  +======================+========================+
  |               ...Energy Metric                |
  +===============================================+

  Bits    Field Description
  ====    =================
  [31]    CRC_FAIL. 
  [30]    Early termination flag
  [29-28] Unused
  [27-24] Number of iterations before CRC passed (or failed at max).
  [23-21] Unused
  [20-00] Energy metric.

  Software does not use Iterations and Energy Metric fields.
----------------------------------------------------------------------- */
/* Raw CRC status. ((C == 0)? Fail: May be Pass) */
#define HDRDEC_HW_HEADER0_RAW_CRC_STATUS_M              0x80000000
#define HDRDEC_HW_HEADER0_RAW_CRC_PASS_V                0x00000000

#else

/*-------------------------------------------------------------------------
  Header0:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +==============+================================+
  |C |Iterations |        Minimum LLR             |
  +======================+========================+
  |               Energy Metric                   |
  +===============================================+

  Bits    Field Description
  ====    =================
  [31]    Raw CRC. ((C == 0)? Fail: May be CRC Pass)
          Packet has passed CRC only when Raw CRC is 1 and Min-LLR > 0
  [30-27] Number of iterations before CRC passed (or failed at max).
  [26-16] The Minimum LLR of the last decoder iteration.
  [15-0]  The truncated energy metric.

  Software does not use Iterations and Energy Metric fields.
----------------------------------------------------------------------- */
/* Raw CRC status. ((C == 0)? Fail: May be Pass) */
#define HDRDEC_HW_HEADER0_RAW_CRC_STATUS_M              0x80000000
#define HDRDEC_HW_HEADER0_RAW_CRC_PASS_V                0x80000000

/* Minimum LLR */
#define HDRDEC_HW_HEADER0_MIN_LLR_M                     0x07FF0000
#define HDRDEC_HW_HEADER0_MIN_LLR_S                     16

#endif

/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_CRC_STATUS

DESCRIPTION
  Returns TRUE if the packet passed Raw CRC.
   
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_hw_dec_pkt_header_type)
           associated with the frame.
  None.  

RETURN VALUE
  Returns TRUE if the packet passed Raw CRC and FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_CRC_STATUS( header )                             \
  (boolean) (((header).header0 & HDRDEC_HW_HEADER0_RAW_CRC_STATUS_M) == \
  HDRDEC_HW_HEADER0_RAW_CRC_PASS_V)


/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_MIN_LLR

DESCRIPTION
  Returns the minimum LLR for the last decode.
   
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_hw_dec_pkt_header_type)
           associated with the frame.
  None.  

RETURN VALUE
  Returns minimum LLR for the last decode.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_MIN_LLR( header )                  \
          (((header).header0 &                        \
          HDRDEC_HW_HEADER0_MIN_LLR_M) >> HDRDEC_HW_HEADER0_MIN_LLR_S)

#ifdef FEATURE_HDR_DIME_MODEM
/*-------------------------------------------------------------------------
  Header1:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +===============================================+
  |++++++++++++++++++++++++++|  Packet Length...  |
  +====================+==========================+
  |..Len|              Min LLR                    |
  +===============================================+

  Bits    Field Description
  ====    =================
  [22-14] Packet Length (not including header) in units of 32-bit words. 
  [13-00] Min LLR
  All other bit positions marked with a '+' sign are reserved.
----------------------------------------------------------------------- */

#define HDRDEC_HW_HEADER1_PKT_LENGTH_M          0x007FC000
#define HDRDEC_HW_HEADER1_PKT_LENGTH_S          14

#else

/*-------------------------------------------------------------------------
  Header1:

  +===============================================+
  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|
  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  +===============================================+
  |+++++++++++++++++++++++++++++++++++++++++++++++|
  +====================+==========================+
  |++++++++++++++++++++|      Packet Length       |
  +===============================================+

  Bits    Field Description
  ====    =================
  [8-0]   Packet Length (not including header) in units of 32-bit words. 

  All other bit positions marked with a '+' sign are reserved.
----------------------------------------------------------------------- */

#define HDRDEC_HW_HEADER1_PKT_LENGTH_M          0x000001FF
#define HDRDEC_HW_HEADER1_PKT_LENGTH_S          0

#endif


/* EJECT */ 
/*===========================================================================
MACRO HDRDEC_GET_PKT_WORD_LENGTH

DESCRIPTION
  Returns the length of the decoder frame in units of 32-bit words.
  
DEPENDENCIES
  None.

PARAMETERS
  header - The header data structure (hdrdec_hw_dec_pkt_header_type)
           associated with the frame.

RETURN VALUE
  The length of the decoder frame, in 32-bit words.

SIDE EFFECTS
  None.
===========================================================================*/
#define HDRDEC_GET_PKT_WORD_LENGTH( header )                        \
         (((header).header1 & HDRDEC_HW_HEADER1_PKT_LENGTH_M) >>    \
         HDRDEC_HW_HEADER1_PKT_LENGTH_S)

#endif /* HDRDECI_H */
