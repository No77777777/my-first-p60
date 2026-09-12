/*! \mainpage HDR Decoder Module 
    \file hdrdec.c
    \brief This module contains definitions and declarations necessary for 
           using the HDR Decoder driver. */

/*===========================================================================
  Copyright (c) 2000 - 2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrdec.c#2 $ $DateTime: 2020/01/11 11:39:51 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/08/20   dmd     Fixed KW error
12/16/16   svu     Fixed KW issues
08/17/15   vlc     Added code to handle Run Control Event Notification errors.
                   Mainlined FEATURE_MODEM_RCINIT.
06/18/15   tnp     Fix compilation warnings
04/27/15   kss     Clean up dog API.
03/24/15   ljl     Updated fw interface.
01/08/15   rmv     Move the check to ignore pkts rcvd in QTA gap to hdrrx 
12/26/14   rmv     Ignore packets received after QTA gap started
10/30/14   wsh     Fixed decob address for Jolokia
10/14/14   wsh     Changes to support Jolokia bringup
09/04/14   ljl     Updated decob address for Thor.
05/08/14   rmv     Updated FL Packet Header log packet to include packet
                   cancelled status and Preamble Detection type 
05/27/14   wsh     Fixed compile error for Jolokia build
05/05/14   ljl     Added FEATURE_HDR_THOR_MODEM to compile.
12/19/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
09/25/13   wsh     Changes to support BOLT bring up
09/04/13   wsh     Changes to support BOLT bring up
08/08/13   rmv     Featurized tmc APIs with ifndef FEATURE_MODEM_RCINIT
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
05/23/13   rmg     Removed change to rectify state decoded packets.
05/06/13   mbs     Fixed compiler errors for BOLT build
03/18/13   smd     Enabled FW API to reserve/release demback for Triton.
03/15/13   smd     Disabled FW API to reserve/release demback for Triton.
03/01/13   dsp     Moved watchdog timer code to watchdog heart-beat model.
03/01/13   kss     Use FW API to reserve/release demback.
03/01/13   rmg     Added change to rectify reading of stale decoded packets.
02/20/13   kss/wsh Do not consider QPCH packet as duplicate
12/20/12   vko     Fixed compiler warnings
10/25/12   kss     Allow CC packets to use the flex dsm API.
08/03/12   ljl     Handled packet cancel rsp on RUMI.
06/28/12   kss     Initialized the decode_ctl fw message before sending.
06/07/12   ljl     Checked null pointer to avoid crash.
06/06/12   kss     Updated for Dime.
04/03/12   cnx     Register MSGR when task is initialized.
03/20/12   arm     Fixed bug in hdrdec_log_pkt_headers().
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/16/12   arm     Resolve TCB errors for Dime. 
02/29/12   kss     Added sanity check of packet length read from hw header.
02/28/12   cnx     Handle HDRDEC_EXIT_HDR_SIG in inactive (exit). 
02/07/12   kss     Fixed issue with CC info reporting for sleep log.
10/10/11   arm     Added support for RCINIT.
11/22/11   mbs     Re-enabled handling of missed sync signaling
10/27/11   vlc     Fixed compiler errors.  
10/17/11   rmg     Removed calls to obsolete clk regim APIs.
10/10/11   grl     Removed use of obsolete AEEstd.h
10/05/11   kss     Fixed race condition and dec rate refresh
09/16/11   kss     Updated DSM-pool-exhausted check to account for the fact 
                   that only RevA can consume large DSM items.
09/16/11   kss     ERR_FATAL after specified number of packets are dropped 
                   due to dsm pool exhaustion.  
08/31/11   kss     Check that decoder is active before processing 
                   hdrdec_process_decob_pending_ind message from firmware. 
08/31/11   kss     Reverted 'Handle HDRDEC_EXIT_HDR_SIG in inactive (exit)'
08/12/11   kss     Handle HDRDEC_EXIT_HDR_SIG in inactive (exit). 
07/19/11   lyl     Supported FL data adaptive Rxd.
06/01/11   cnx     Fixed compiler error.
05/31/11   cnx     Added hdrdec_get_packet_info().
05/19/11   rmg     Removed turning on of MCPM data state clocks in Rx state.
04/27/11   rmg     Added HDR MCPM support changes.
04/26/11   kss     Added KW fixes.
02/01/11   rmg     Removed DECOB_RESET access for NikeL builds.
12/20/10   kss     Updated for initial NikeL build.
10/28/10   kss     Use flex dsm API to use large items for larger packet size.
10/28/10   kss     Don't count cancelled sequence # as bad CRC. 
10/24/10   pxu     Toggle pstrobe during hdrdec_process_decob_pending_ind.
09/21/10   kss     Enable DECOB clock gating.
08/10/10   arm     Remove Rex_disable_premption to make task free running.
09/15/10   ljl     Supported dynamic wdog.
06/21/10   cnx     Fixed compiler warnings.
03/25/10   kss     Fixed decoder LCG register.
03/23/10   kss     Updated register for Voyager 2.0. 
03/03/10   kss     Fix NULL pointer reference in logging.
02/25/10   kss     Initial support for FW logging on MDM9X00
02/20/10   lyl     Temporarily bypass sleep logging.
12/17/09   kss     Remove bringup bypass of header logging.
12/13/09   etv     Update statistics for packets that failed CRC.
12/01/09   kss     Changes to support MDM9K bringup.
11/09/09   etv     Handle cancelled packets due to duplicate detection.
10/06/09   etv     Fixed initialization issue.
10/02/09   lyl     Updated for rumi bring up.
08/17/09   etv     Use sub-aset index instead of demod index for logging.
07/17/09   etv     Fixed issue with receiving fw messages.
09/15/09   kss     Use new PACK() macro for Q6 compatibility.
08/12/09   wsh     Fixed compile error on genesis.2h09 MOB
08/05/09   etv     Updated for decoder deactivation call back interface.
07/29/09   etv     Replaced HDRHSTR_GET_CC_START with hdrsrch_get_cc_start.
07/28/09   etv     Included hdrfw_msg.h
07/20/09   etv     HWIO register changes for MDM9k
07/16/09   etv     Stub out hdrdec_init_clock_speed for MDM9k compilation.
07/15/09   etv     Moved sw module definitions to hdrmsg.h
07/14/09   kss     Remove include of hdrasm.h
06/22/09   etv     Fixed Lint warnings.
05/21/09   etv     Moved Decoder header logging to HDRDEC module.
03/10/09   etv     Created HDR Decoder task, MSG interface with fw and 
                   Doxygen commenting style.
03/28/09   etv     Moved decoder and demsymbuff clock configuration from 
                   hdrsrchrf_tune_sig to hdrdec_activate
03/16/09   etv     Added hdrdec_init_clock_speed.
03/12/09   rkc     Removed control for CLKRGM_RESOURCE_CDMA_TX.
12/10/08   smd     Included amssassert.h
11/10/08   kss     Removed intlocks.
10/13/08   kss     Lint fixes.
09/03/08   etv     Read header only if decob.pending is non-zero.
                   Flush bad CRC pkts only if it is not canceled.
08/27/08   etv     Fixed bug with programming decoder interrupt rate.
08/11/08   etv     Fixed fw/sw interface bug with Min LLR fix.
08/05/08   etv     Read Min LLR result instead of Min LLR.
03/05/08   etv     Used the right equation to input decoder interrupt rate.
11/29/07   ljl     Set turbo decoder clock to 144MHz.
10/09/07   etv     Removed the code that handles Race condition with 
                   numConcurrentPkts. mDSP handles this.
08/20/07   etv     Read Demod ID from decoder header.
07/20/07   etv     Enabled BURST interface.
07/11/07   etv     Read Rev-B fields from decoder header and updated for
                   fw_header:pkt_type changes in firmware interface.
07/06/07   etv     Disable DECOB_BURST_IF; Compute right byte offset from
                   burst base; work around for channel estimation.
04/12/07   cab     Add support for new sleep OKTS API.
04/05/07   etv     Updated for Rev-B support.
02/02/07   ljl     Set minimum preamble thresh.
12/22/06   grl     Set MDSP for improved partial loading performance.
09/18/06   mt      Added forced preamble detection status field to decoder info.
09/18/06   kss     Added include of clk.h.
09/15/06   etv     Added hdrdec_flush_packet.
09/08/06   ljl     Added hdrdec_uninstall_isr() and 
                   hdrdec_reset_and_deactivate().
06/28/06   jyw     Added support to the MPC.
06/22/06   etv     Firmware exports slot_index instead of slots_to_decode.
05/30/06   etv     Changed SlotsRemaining to SlotsToDecode.
03/09/06   kss     Compute slots_to_decode correctly for Rev A CC.
10/25/05   kss     Fixed bug in BCC slots_to_decode.
10/17/05   jyw     Added pkt cancel, read/confirm in the dec deact. and act.
10/10/05   jyw     Support pkt header logging.
09/01/05   kss     Fixed bug in BCC slots_to_decode.
05/20/05   kss     Move OKTS init to hdrdec_set_isr().
04/15/05   kss     Combined crc_pass and drop_reason into crc_status.
04/05/05   kss     Made DBIF use default behavior, clean up macros.
03/23/05   hrk/kss Added function hdrdec_pre_deactivate.
02/23/05   kss     Removed clk_regime calls that are not needed on MSM6800.
10/20/04   ds      Modified F3 to print out decoder header even if CC CRC fails
10/12/04   ds      Updated DRC 14 entry in DRC to Frame size translation table
11/17/03   kss     Make sure DEC clock is on.
10/01/03   kss     Remove non-MSM6500 code; save/restore DCG state.
09/21/03   bt      Disable Clock Gating for hybrid mode.
09/11/03   kss     Reduced default watermark.
08/15/03   kss     Enable/disable/reset both TURBO_DEC and HDR_TDEC clocks.
08/04/03   kss     Change minLLR threshold to 2 in FTT.
06/30/03   kss     Switch decoder clock in hdrdec_activate/deactivate().
08/03/02   kss     Included some definitions that were in hdrdeci.h
05/24/02   kss     Reset decoder on activation.
04/03/02   kss     Added some additional error checking.
03/17/02   kss     Reset Turbo decoder on activation.
03/11/02   kss     Added workaround for DBIF_PACKET_CNT misread.
03/03/02   kss     Added support for decoder clock enable/disable, and the 
                   hdrdec_set_isr() function.
01/03/02   kss     Used new decoder FTT - raised min iterations and min LLR
                   for fewer false CRC passes.
08/22/01   kss     Added optimizations.
07/12/01   kss     Add several frames to FTT for high rate - low iterations.
04/19/01   kss     Adjust for MSM5500.
02/20/01   kss     Get frame size from DRC.
10/23/00   kss     Added optmizations and a profiling mode via #defines.
10/11/00   kss     Code review update; added HDRDEC_NO_CHECKING option.
09/01/00   kss     Added functions for decoding more of the frame header 
                   information, and added an internal header file.
05/01/00   kss     Created.
         
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"       /* Needed for byte, uint16, uint32, INTLOCK(), etc. */
#include "amssassert.h"   /* Needed for ASSERT_FATAL */
#include "msm.h"          /* MSM register definitions */
#include "dog.h"          /* Dog APIs */
#include "dog_hb_rex.h"   /* Dog HB APIs */
#include "hdrdec.h"       /* Hdrdec public type declarations */
#include "hdrdeci.h"      /* Hdrdec internal type/macro definitions */
#include "err.h"          /* Error handling */
#include "hdrdebug.h"     /* Message routines */
#include "hdrmdsp.h"      /* MDSP isr rotines */
#include "hdrmdspmcr.h"   /* MDSP macros */
#include "hdrutil.h"      /* BIT count */
#include "hdrrxstats.h"   /* Statistics */
#include "hdrcmac.h"
#include "hdrsrchlog.h"
#include "hdrdsm.h"
#include "hdrhai.h"
#include "hdrrx.h"
#include "hdrlogi.h"
#include "hdrsrch.h"
#include "hdrbcmac.h"
#include "hdrmsg.h"
#include "hdrfw_msg.h"
#include "msgr_rex.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#include "task.h"
#include "dog.h"
#include "dsm.h"
#include "dsmutil.h"

#include "rcinit.h"
#define HDRDEC_RCEVT_PREFIX "HDRDEC:"
#define HDRDEC_RCEVT_READY HDRDEC_RCEVT_PREFIX "ready"
#define HDRSRCH_RCEVT_PREFIX "HDRSRCH:"
#define HDRSRCH_RCEVT_READY HDRSRCH_RCEVT_PREFIX "ready"

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h" /* needed to access hdrhit_rumi_cfg */
#endif /* FEATURE_HDR_RUMI */

/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* Uncomment for more debugging messages */
#ifdef FEATURE_HDR_RUMI
#define HDRDEC_DEBUG
#endif /* FEATURE_HDR_RUMI */
rex_tcb_type*    hdrdec_tcb_ptr;

/** Number of bytes in a native word */
#define HDRDEC_BYTES_PER_WORD                                     4

/** Maximum number of forward link carriers supported. */
#define HDRDEC_MAX_NUM_FL_CARRIERS                                3

/** Number of interlaces per frame */
#define HDRDEC_NUM_INTERLACES                                     4

/** Rev-A CRC length in bytes */
#define HDRDEC_REV_A_CRC_LEN                                      3

/** Release-0 CRC length in bytes */
#define HDRDEC_REV_0_CRC_LEN                                      2

/**
  Physical layer packet size to an index
  
  This macro is used to map the physical layer packet size to an index for
  ease of storage in the DSM item and to index into the table of pointers
  for revA packet stats. The mapping of physical layer packet size in units
  of 4 bytes as reported by decoder to an index is given as follows.
  
    @verbatim
    +---------------------------------------------------------+
    |PktSize (bytes) |16  |32  |64  |128 |256 |384 |512 |640  |
    +---------------------------------------------------------+
    |Index           |0   |1   |2   |3   |4   |5   |6   |7    |
    +---------------------------------------------------------+
    @endverbatim
*/
#define HDRDEC_PKT_SIZE_TO_INDEX( pkt_size ) \
    ( ( (pkt_size) > 64 )? ( ( (pkt_size) / 128 ) + 2 ): ( (pkt_size) / 32 ) )

/** Maximum Number of event buffers */
#define HDRDEC_MAX_NUM_EVENTS                                     10

/** Maximum number of on-arrival packet delivery buffer */
#define HDRDEC_MAX_NUM_OAPD                                       4

/** Max number of words to read at a time */
#define HDRDEC_MAX_READ_LENGTH                                    32


/** Calculate the number of HSTRs from CC start to CC dec */
#define HDRDEC_CALC_CC_TO_CCDEC( hstr, cc_start )                       \
  ( 512 - ( ( ( ( ( cc_start ) * 2 ) + 512 ) - ( ( hstr ) % 512 ) ) % 512 ) )


#define HDRDEC_NUM_HALF_SLOTS_INTERLACE                       8

/** Maximum number of records allowed in the decoder packet header log packet */
#define HDRDEC_PKT_HEADER_LOG_BUF_SIZE                        32

/** Maximum age (in halfslots) allowed for active log buffers (20 slots)
    Allowing a little over a frame gives chance to fill-up the buffer */
#define HDRDEC_LOG_BUFF_MAX_AGE_ALLOWED                       40

/**< Logging is done every frame so buffer can hold 16 records */
#define HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE  HDRLOG_NUM_SLOTS_IN_FRAME  

/**< SW double buffers the stored decoder packet information to avoid 
     contention between decoder ISR (which adds records to the buffer) 
     and RX Subtask (which reads from the buffer and logs the packet) */
#define HDRDEC_NUM_LOG_BUFFERS                                2


/*---------------------------------------------------------------------------
                          DECODER BUFFER MACROS
---------------------------------------------------------------------------*/

/** Invalid decoder buffer block index */
#define HDRDEC_DECOB_INVALID_BLOCK_INDEX                          -1

/** Invalid decoder buffer word offset */
#define HDRDEC_DECOB_INVALID_OFFSET                               -1

/** Most significant bit position in decobPending mapping to block index 24 */
#define HDRDEC_DECOB_PENDING_MSB                                  23

/** Are we done reading all packets from the decoder buffer? */
#define HDRDEC_DECOB_HAS_MORE_PKTS( pending ) ( (pending) != 0 )

/** Does the decoB block at blk_index has any packets to be read? */
#define HDRDEC_DECOB_BLOCK_HAS_PACKETS_READY( pending, blk_index ) \
  ( ( (pending) & ( 1 << (blk_index) ) ) != 0 )

/** Clears the bit in "pending" field at the bit position given by blk_index */
#define HDRDEC_BLOCK_READ_DONE( pending, blk_index )    \
  ( (pending) &= ( ~(uint32) ( 0x01 << (blk_index) ) ) )

/*---------------------------------------------------------------------------
                     DUPLICATE DECODER PACKET DETECTION
---------------------------------------------------------------------------*/
/** Duplicate packet detection is needed only for Canonical DRC 0/1 and short
    packets for DRCs 0-4  */
#define HDRDEC_DUP_DETECTION_IS_NEEDED( drc_rate, size )                      \
  ( ( ( (drc_rate) <= (int) HDRHAI_DRC_307_2_SHORT ) && ( (size) < 3 ) ) ||   \
    ( ( (drc_rate) <= (int) HDRHAI_DRC_38_4 ) && ( (size) == 3 ) ) )

/*---------------------------------------------------------------------------
              RESEQUENCING BUFFER DEFINITIONS AND DECLARATIONS
---------------------------------------------------------------------------*/

/** @note These masks need to be updated if the Number of FL carriers changes */

/** Sequence number Mask  */
#define HDRDEC_PKT_SEQUENCE_M                                     0xFF  
/** Mask for Seq Buff ind */
#define HDRDEC_PKT_BUFFER_INDEX_M                                 0xFF  

/** Size of the Re-sequence buffer

   The Circular buffer needs to be large enough to handle incoming packets for
   the longest out-of-order arrival time. The worst case scenario is DRC-1
   packet terminating on slot 16 on one interlace and all other interlaces 
   decoding packets in a single slot filling up the buffer. So, the maximum
   buffer size required for this case is 64. If we have N carriers, it is
   (64 * N). We need to account for some processing delays (64). So the 
   formula to compute the buffer size is (64 * N) + 64 */
#define HDRDEC_PKT_BUFFER_SIZE ( ( 64 * HDRDEC_MAX_NUM_FL_CARRIERS ) + 64 )

/** @note The watermark levels must be tweaked if the HDRRX_MAX_NUM_FL_CARRIERS
    change */

/** Number of packets in the circular buffer before we start flow control.
    Just leaving enough space in the buffer for one full decoder buffer
    (24 pkts), two DRC lengths (max DRC length is 8) of one slot
    terminations and 8 more slots for processing delays */
#define HDRDEC_CBUFF_HI_WATERMARK          ( HDRDEC_PKT_BUFFER_SIZE - 48 )

/** Number of items that must be on circular buffer before we stop flow
    control */
#define HDRDEC_CBUFF_LO_WATERMARK          ( HDRDEC_PKT_BUFFER_SIZE / 2 )

/* Number of consecutive times we will gracefully drop FL packets due to
   dsm pool being exhausted. After this AT will ERR_FATAL. */
#define HDRDEC_ERR_FATAL_ON_DSM_POOL_EXHAUSTED_COUNT_THRESHOLD      20

/** Macro to perform sequence wraparound */
#define HDRDEC_INCREMENT_SEQ( seq )               \
    ( (seq) = ( ( (seq) + 1 ) & HDRDEC_PKT_SEQUENCE_M ) )

/** Converts a sequence number to an index in the buffer array */
#define HDRDEC_SEQ_INDEX( seq ) ( (seq) & HDRDEC_PKT_BUFFER_INDEX_M )

/*-----------------------------------------------------------------------------
                          MACROS (INLINE FUNCTION CANDIDATES) 
  The macros below are good candidates for inline functions. These macros exist
  For lack of control over compiler 
---------------------------------------------------------------------------*/
/** Convenient macro to build user-fields for easy storage into the DSM item */
#define HDRDEC_BUILD_USER_FIELDS(                                             \
          app_field, kind_field, app_ptr_field, pkt_info )                    \
  do                                                                          \
  {                                                                           \
  } while ( 0 )

/* Convenient macro to drop a packet when the resequencing buffer overflows */
#define HDRDEC_DROP_OLD_PKT_ON_OVERFLOW()                                     \
  do                                                                          \
  {                                                                           \
    hdrdec.buff_entry_cnt--;                                                  \
    if (hdrdec.pkt_buffer[hdrdec.process_seq] != NULL)                        \
    {                                                                         \
      /* Free the DSM item */                                                 \
      dsm_free_packet ( &( hdrdec.pkt_buffer[hdrdec.process_seq] ) );         \
    }                                                                         \
                                                                              \
    hdrdec.pkt_buffer[hdrdec.process_seq] = NULL;                             \
    hdrdec.pkt_status[hdrdec.process_seq] = HDRDEC_PKT_WAITING;               \
    hdrdec.pkt_slot_time[hdrdec.process_seq] = -1;                            \
    HDRDEC_INCREMENT_SEQ ( hdrdec.process_seq );                              \
                                                                              \
    /* Number of packets dropped */                                           \
    ++hdrdec.num_dropped_pkts;                                                \
  } while ( 0 );                                                              

/* Convenient macro to drop packet when we run out of DSM items */
#define HDRDEC_DROP_PKT_ON_DSM_EXHAUST( crc_status )                          \
  do                                                                          \
  {                                                                           \
    HDR_MSG_PROT ( MSG_LEGACY_ERROR, "Out of DSM items! Dropping packets" );  \
                                                                              \
    /* Flush the packet from decoder buffer and treat like bad CRC */         \
    hdrdec_flush_packet();                                                    \
    crc_status = HDRDEC_CRC_FAIL;                                             \
  } while ( 0 )

/*---------------------------------------------------------------------------
                              DEBUGGING
---------------------------------------------------------------------------*/

#ifdef HDRDEC_DEBUG

#define HDRDEC_DEBUG_DUMP_DECODER_HEADER_INFO( pkt_info, num_mac_pkts )       \
  do                                                                          \
  {                                                                           \
    HDR_MSG_PROT_1(                                                           \
      MSG_LEGACY_HIGH, "----Num MAC Packets: %d", num_mac_pkts );             \
    HDR_MSG_PROT_5(                                                           \
      MSG_LEGACY_HIGH, "DId: %d CRC %d, DRCd %d, DRCr %d. HSTR %d",           \
      pkt_info.demod_id, pkt_info.crc_status,                                 \
      pkt_info.drc_decoded, pkt_info.drc_requested, pkt_info.hstr );          \
    HDR_MSG_PROT_4(                                                           \
      MSG_LEGACY_HIGH, "ByteL %d, LinkId %d, Seq %d, SlotI %d",               \
      pkt_info.byte_length, pkt_info.link_id,                                 \
      pkt_info.sequence, pkt_info.slots_to_decode );                          \
    HDR_MSG_PROT_5(                                                           \
      MSG_LEGACY_HIGH, "Ch 0x%x, Ty 0x%x, Rev %d, Con: %d, Sc: %d",           \
      pkt_info.channel, pkt_info.pkt_type, pkt_info.rev_a,                    \
      pkt_info.num_concurrent_pkts, pkt_info.sched_group_id );                \
    HDR_MSG_PROT_3(                                                           \
      MSG_LEGACY_HIGH, "HW hdr0:%x hdr1:%x, HW CRC:%x",                       \
      hdrdec.hw_header.header0,                                               \
      hdrdec.hw_header.header1,                                               \
      HDRDEC_GET_CRC_STATUS(hdrdec.hw_header) );                              \
  } while ( 0 )

#define HDRDEC_DEBUG_DUMP_NUM_BYTES_USED( dsm_item_ptr )                      \
  do                                                                          \
  {                                                                           \
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Used: %d",dsm_item_ptr->used );         \
  } while ( 0 )

#else /* HDRDEC_DEBUG */
/* No Debugging info required */

#define HDRDEC_DEBUG_DUMP_DECODER_HEADER_INFO( pkt_info, num_dec_pkts )  
#define HDRDEC_DEBUG_DUMP_NUM_BYTES_USED( dsm_item_ptr )  

#endif /* HDRDEC_DEBUG */

#ifdef FEATURE_HDR_THOR_MODEM
#define HDRDEC_DEC_BUF_START_ADDR       ( hdrmdsp_shared_mem_interface->read.flInterface.decobAddr )
#else
#ifdef FEATURE_HDR_JOLOKIA_MODEM
#define HDRDEC_DEC_BUF_START_ADDR       ( hdrmdsp_shared_mem_interface->read.decobStartAddr )
#elif defined(FEATURE_HDR_BOLT_MODEM)
#define HDRDEC_DEC_BUF_START_ADDR       HWIO_MEM_POOL_AXI_START_ADDR 
#else
#define HDRDEC_DEC_BUF_START_ADDR       HWIO_MEM_POOL_MEM_PAGE1_START_ADDR
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_THOR_MODEM */

/*===========================================================================
                                TYPEDEFS
===========================================================================*/

/** @enum hdrdec_cmd_name_e_type
    @brief Enum of all commands supported by HDRDEC task */
typedef enum
{
  HDRDEC_ACTIVATE_CMD = 0,
    /**< Command to activate decoder */
  HDRDEC_ENTER_MODE_CMD = 1,
    /**< Command to enter idle or traffic modes */
  HDRDEC_RESET_SEQ_CMD = 2,
    /**< Command to reset decoder sequence numbers */
  HDRDEC_DEACTIVATE_CMD = 3,
    /**< Command to deactivate decoder */
  HDRDEC_LOG_MC_PKT_HEADERS_CMD = 4,
    /**< Command to log MC pkt header */
  HDRDEC_SHIP_AGED_LOG_BUFFER_CMD = 5,
    /**< Command to ship the pkt header info log*/
  HDRDEC_LOG_PKT_HEADERS_CMD = 6,
    /**< Command to log the pkt PHY header*/
  HDRDEC_MAX_NUM_CMDS
    /**< Total number of commands supported by decoder */

} hdrdec_cmd_name_e_type;

/** @union hdrdec_cmd_data_u_type
    @brief Union of data for all commands supported by HDRDEC module */
typedef union
{
  hdrdec_decoder_mode_e_type mode;
    /**< Decoder mode. Should be either HDRDEC_MONITOR_MODE or 
         HDRDEC_TRAFFIC_MODE */
  hdrdec_shutdown_complete_cb_type srch_cb;
    /**< HDRSRCH callback to notify decoder shutdown complete */
  boolean immediate;
    /**< HDRDEC_LOG_PKT_HEADERS Do we log the packet immediately? */

} hdrdec_cmd_data_u_type;

/** @struct hdrdec_cmd_s_type;
    @brief  Encapsulation of an HDRDEC command */
typedef struct
{
  hdrdec_cmd_name_e_type cmd;
    /**< One of the commands supported by HDRDEC module */
  hdrdec_cmd_data_u_type data;
    /**< Command data */
} hdrdec_cmd_s_type;

/** @struct hdrdec_event_s_type
 *  @brief  Events handled by HDRDEC module */
typedef struct
{
  hdrbuf_hdr_type hdr;
    /**< Event header */

  union
  {
    hdrdec_cmd_s_type cmd;
      /**< HDR Decoder Command */

    hdrfw_msg_ind_u msg;
      /**< HDR Decoder Messages */

  } event;

} hdrdec_event_s_type;


/** Function type to update forward link statistics */
typedef void (*hdrdec_update_fl_stats_f_type)
(
  hdrdec_pkt_info_s_type *pkt_info,
    /**< Information about the decoded packet */
  uint8 pkt_size_index
    /**< Packet size index */
);

/** Information needed to forward each packet and update its statistics */
typedef struct
{
  hdrdec_pkt_status_e_type status;
    /**< Status of the decoded packet */
  hdrdec_update_fl_stats_f_type update_stats_fptr;
    /**< function to call for updating statistics */
} hdrdec_pkt_fwd_s_type;

/** On-arrival packet delivery buffer struct type */
typedef struct
{
  q_link_type link;
    /**< Link to next item to be delivered on arrival */
  dsm_item_type *pkt_ptr;
    /**< Pointer to DSM item containing the packet to be delivered on-arrival */
  hdrdec_pkt_status_e_type status;
    /**< Packet status */
} hdrdec_oapd_s_type;

/** Packet information required to determine if the packet is a duplicate
    detection */
typedef struct
{
  boolean valid;
    /**< Whether valid info is stored above for this interlace */

  uint32 crc_field;
    /**< Packet's CRC field (unformatted uint32, so it contains trailer and
         preceeding byte) */

  uint16 hstr;
    /**< Half slot time reference */

  hdrdec_pkt_channel_e_type channel;
    /**< Packet's forward link channel */

  int size;
    /**< Packet size index */

  uint32 drc_rate;
    /**< DRC Rate of packet */

} hdrdec_dup_pkt_info_s_type;

/*===========================================================================
                            GLOBAL VARIABLES
===========================================================================*/
void (*hdrdec_fw_log_cb)(int active_buffer);
  /* Track logging callback function. */

/** Global data in HDR decoder module */
static struct
{
  boolean  active;
    /**< Track decoder active state */

  msgr_client_t msgr_client_id;
    /**< Messenger client ID */

  uint16 decob_msg_rate;
    /**< Decoder buffer packet pending indication message rate in slots */

  hdrdec_shutdown_complete_cb_type shutdown_notification_cb;
    /**< HDR SRCH callback to notify shutdown complete */

  hdrdec_pkt_info_s_type  pkt_info;
    /**< Decoder packet info buffer. */

  /* EVENT/OAPD POOL AND QUEUES */

  q_type event_free_q;
    /**< Queue of free buffers for commands, indications, and messages. */

  q_type event_q;
    /**< Queue of pending commands, indications, and messages. */

  hdrdec_event_s_type event_pool [ HDRDEC_MAX_NUM_EVENTS ];
    /**< Statically allocated buffers. */

  boolean deliver_on_arrival;
    /**< Should we deliver packets as and when they arrive? */

  q_type oapd_q;
    /**< Queue of Rx packets to be delivered on arrival */

  q_type oapd_free_q;
    /**< A Queue to maintain the free on-arrival packet delivery buffers */

  hdrdec_oapd_s_type oapd_pool [ HDRDEC_MAX_NUM_OAPD ];
    /**< Pool of On-arrival packet delivery buffers */

  /* RESEQUENCE BUFFER AND POINTERS */

  int expected_seq;
    /**<  Expected sequence number of next packet */

  int process_seq;
    /**< Sequence number of last packet passed to MAC layer */

  int buff_entry_cnt;
    /**< Number of entries in sequence buffer */

  dsm_item_type* pkt_buffer[HDRDEC_PKT_BUFFER_SIZE];
    /**< HDR Rx packet buffer, indexed by sequence. */

  hdrdec_pkt_status_e_type pkt_status[HDRDEC_PKT_BUFFER_SIZE];
    /**< Status array for Rx packet buffer */

  int16 pkt_slot_time[HDRDEC_PKT_BUFFER_SIZE];
    /**< Time packet arrived (-1 means unused) */

  boolean  buff_flow_control;
    /**< Circular buffer flow control - true when flow controlled */

  boolean early_term_disabled;
    /**< Whether early termination is disabled. */

  /* DECODER BUFFER MANAGEMENT */

  uint32 decob_pending;
    /**< Working variable while the packet is being read from the
         decoder. The decoder ISR reads from the decoB until this
         variable is driven to zero */

  uint32 decob_clear;
    /**< Used to retain the value of decobPending that the ISR
         started with. This variable is used to notify firmware that
         the block indices denoted by bit positions set in
         decob_clear have been read already */

  int next_block_index;
    /**< Working variable containing the block index being read */

  uint16 frames;
    /**< Number of decoder frames ready to be read. */

  int decob_word_offset;
    /**< Word (4-bytes) offset from start of decoB to start of block being 
         read in the decoder buffer */

  volatile uint32* decob_ptr;
    /**< Pointer to pkt in decob in mempool. Updated as packet is read. */

  hdrdec_hw_dec_pkt_header_type hw_header;
    /**< Stores most recent decoder hardware header */

  hdrdec_fw_dec_pkt_header_type fw_header;
    /**< Stores most recent decoder firmware header */

  uint16 crc_len;
    /**< CRC length (2 bytes or 3 bytes) */

  uint16 num_mac_pkts;
    /**< Number of 128-byte chunks that comprise the current decoder frame. */

  uint16 bytes_read;
    /**< Maintains the number of bytes read of each decoder frame. */

  hdrdec_dup_pkt_info_s_type
    last_pkt_info [HDRDEC_MAX_NUM_FL_CARRIERS] [HDRDEC_NUM_INTERLACES];
    /**< Information of last packet received on each interlace to detect 
         duplicates. */

  uint32 num_dropped_pkts;
    /**< Number of packets dropped due to CBUFF overflow */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  uint16 latest_ftc_pkt_hstr;
    /* Latest FTC packet HSTR (16bit) */
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */ 
  rex_crit_sect_type              hdrdec_crit_sect; 
    /* Critical Section */ 
  

  uint32 dsm_pool_exhausted_count;
    /* Number of consecutive times dsm pool has been exhausted */

} hdrdec;


/** Per-carrier Packet header info buffer */
typedef PACK(struct)
{
  hdrlog_mc_carrier_id_type carrier_id;
    /**< Carrier ID */

  uint8 rec_cnt;
    /**< Number of the record in the rec_buf */

  hdrlog_mc_fl_pkt_header_info_type rec[HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE];
    /**< Record buffer can hold 16 records for 1 frame */
  
} hdrdec_per_carrier_pkt_header_buf_type;

typedef struct
{
  boolean ready_for_logging;
    /**< Is this buffer ready for logging? */

  hdrdec_per_carrier_pkt_header_buf_type carriers [HDRDEC_MAX_NUM_FL_CARRIERS];
    /**< Multi-carrier Packet header buffers */

} hdrdec_mc_pkt_header_buff_type;


/** Decoder packet header log packet buffer */
LOCAL struct
{
  uint8 rec_cnt;
    /**< number of the record in the rec_buf */

  log_hdr_fl_pkt_header_info_type rec[HDRDEC_PKT_HEADER_LOG_BUF_SIZE];
    /**< record buffer can hold 32 records for 2 frames period */
  
} hdrdec_pkt_header_log_buf;

/** Multi-carrier decoder packet header log packet buffer */
LOCAL struct
{
  uint8 active_buff_idx;
    /**< Index of the buffer for double buffering */

  uint16 ts_start;
    /**< Timestamp when first record entered the active buffer */

  boolean ts_valid;
    /**< Is the timestamp valid? */

  hdrdec_mc_pkt_header_buff_type buff [HDRDEC_NUM_LOG_BUFFERS];
    /**< Double MC Packet header buffers for concurrency */

} hdrdec_mc_pkt_hdr;


/*==========================================================================
                             LOOK-UP TABLES 
===========================================================================*/

/*--------------------------------------------------------------------------
  Duplicate Packet detect info. These tables are used to figure out whether 
  a new packet is a possible dup packet. 
  -------------------------------------------------------------------------*/

/** Packet slot lengths for DRC rates with possible dups. Indexed by DRC. */
LOCAL const uint8 hdrdec_dup_packet_slot_length[] = 
{
  16,   /* DRC 0 - 16 slots */
  16,   /* DRC 1 - 16 slots */
  8,    /* DRC 2 -  8 slots */
  4,    /* DRC 3 -  4 slots */
  2     /* DRC 4 -  2 slots */
};

/** Possible slot time when dups may occur. Indexed by pkt_size. */
LOCAL const uint8 hdrdec_dup_packet_slot_delta[] = 
{
  1,  /* Size index 0 (128 bits/16 bytes)   - possible dup every 1 slots */
  2,  /* Size index 1 (256 bits/32 bytes)   - possible dup every 2 slots */
  4,  /* Size index 2 (512 bits/64 bytes)   - possible dup every 4 slots */
  8   /* Size index 3 (1024 bits/128 bytes) - possible dup every 8 slots */
};

/** Location of last 32-bit word of packet, which contains CRC field.
    Indexed by size. */
LOCAL const uint8 hdrdec_dup_packet_crc_start[] = 
{
  (16-4),   /* Size index 0 (128 bits/16 bytes) */
  (32-4),   /* Size index 1 (256 bits/32 bytes) */
  (64-4),   /* Size index 1 (512 bits/64 bytes) */
  (128-4)   /* Size index 2 (1024 bits/128 bytes) */
};

/** List of packet forwarding information for each packet type indexed by
    CRC status and all possible forward link channels */
static hdrdec_pkt_fwd_s_type hdrdec_pkt_fwd_update_stats_lut 
    [HDRDEC_NUM_CRC_STATUS] [HDRDEC_MAX_NUM_FL_CHANNELS] = 
{
  /* CRC fail */
  {
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_tc_pkt_stats },
      /* Unicast Traffic Channel */
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_tc_pkt_stats },
      /* Multicast Traffic Channel */
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_cc_pkt_stats }
      /* Control Channel */
#ifdef FEATURE_HDR_BCMCS
    ,{ HDRDEC_BCC_PKT_RXD, hdrrxstats_update_bad_bcc_pkt_stats }
      /* Broadcast Channel */
#endif
  },
  /* CRC Pass */
  {
    { HDRDEC_FTC_PKT_READY, hdrrxstats_update_good_tc_pkt_stats },
      /* Unicast Traffic Channel */
    { HDRDEC_FTC_PKT_READY, hdrrxstats_update_good_tc_pkt_stats },
      /* Multicast Traffic Channel */
    { HDRDEC_CC_PKT_READY, hdrrxstats_update_good_cc_pkt_stats }
      /* Control Channel */
#ifdef FEATURE_HDR_BCMCS
    ,{ HDRDEC_BCC_PKT_RXD, hdrrxstats_update_good_bcc_pkt_stats }
      /* Broadcast Channel */
#endif
  },
  /* CRC False Pass */
  {
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_tc_pkt_stats },
      /* Unicast Traffic Channel */
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_tc_pkt_stats },
      /* Multicast Traffic Channel */
    { HDRDEC_PKT_CANCELED, hdrrxstats_update_bad_cc_pkt_stats }
      /* Control Channel */
#ifdef FEATURE_HDR_BCMCS
    ,{ HDRDEC_BCC_PKT_RXD, hdrrxstats_update_bad_bcc_pkt_stats }
      /* Broadcast Channel */
#endif
  }
};

#undef  DOG_HDRDEC_RPT
#define DOG_HDRDEC_RPT hdrdec_dog_rpt_var
dog_report_type   hdrdec_dog_rpt_var = 0;
  /* Dog rpt */

/*============================================================================

                        LOCAL FUNCTION DEFINITIONS

=============================================================================*/

/*============================================================================
  HDRDEC_UPDATE_PKT_HEADER_BUF
*/
/**
  The function is called from the ISR to save the current pkt header info in
  the log record buffer.

  @return Current record count in the log buffer

  @context HDRDEC task. */
/*============================================================================*/
uint8 hdrdec_update_pkt_header_buf
( 
  hdrdec_pkt_info_s_type header_info 
    /**< Header Info struct to record the decoder pkt header info */
)
{
  uint8 rec_ind;
    /* Index of the current record into the pkt header log buffer */

  log_hdr_fl_pkt_header_info_type *dec_hdr_info_ptr;
    /* Convenience pointer to decoder pkt header info */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Calculate the index to add the new record at the end of the buf limiting 
   * the mac rec count to 32; the rec_cnt is reset in the function 
   * hdrrx_log_phy_header */
  if ( ( hdrdec_pkt_header_log_buf.rec_cnt + 1 ) <= 
       HDRDEC_PKT_HEADER_LOG_BUF_SIZE )
  {
    rec_ind = hdrdec_pkt_header_log_buf.rec_cnt ++ ;
      /* Get the current frame cnt and then increase the rec cnt */

    dec_hdr_info_ptr = &hdrdec_pkt_header_log_buf.rec[rec_ind];
      /* Initialize the convenience pointer to start of current record */

    /* Copy the current record into the buffer */
    dec_hdr_info_ptr->data_len = (uint8)(header_info.byte_length/4);
    dec_hdr_info_ptr->drc_value = (uint8) header_info.drc_decoded;
    dec_hdr_info_ptr->hstr = (uint16) header_info.hstr;
    dec_hdr_info_ptr->pkt_type = (uint8) header_info.pkt_type;
    dec_hdr_info_ptr->seq_num = (uint8) header_info.sequence;
    dec_hdr_info_ptr->bit_fields.slots_to_decode = (uint16) header_info.slots_to_decode;

    dec_hdr_info_ptr->bit_fields.crc_status = 
       (uint16) ( (header_info.pkt_is_cancelled)? 
         HDRDEC_PKT_CANCELED : header_info.crc_status );
       /* Indicate Cancelled packet under CRC status column in log packet 
          CRC status - (0)Fail, (1)Pass, (2)False Pass, (3)Pkt Cancelled */

    dec_hdr_info_ptr->bit_fields.pd_type = 
       (uint16) header_info.sw_forced_preamble;
       /* Indicates Premable Detection Type*/
  }
  else
  {
    /* The buf is full; stop logging and dont increase the
     * the rec_cnt. This should be unusual */
  }          

  return hdrdec_pkt_header_log_buf.rec_cnt;

} /* hdrdec_update_pkt_header_buf */

/*============================================================================
  HDRDEC_UPDATE_MC_PKT_HEADER_BUF
*/
/**
  The function is called from the ISR to save the current pkt header info in
  the log record buffer.

  @return TRUE if the packet is ready for logging and FALSE otherwise.

  @context HDRRX task. */
/*============================================================================*/
boolean hdrdec_update_mc_pkt_header_buf
(
  hdrdec_pkt_info_s_type *header_info_ptr     
    /**< Pointer to decoder packet header information */
)
{
  hdrlog_mc_fl_pkt_header_info_type *dec_hdr_info_ptr;
    /* Convenience pointer to decoder pkt header info */
    
  hdrdec_per_carrier_pkt_header_buf_type *active_buffer;
    /* Active buffer that logs the decoder packet records */

  boolean ready_for_logging = FALSE;
    /* Do we have a buffer ready for logging? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  active_buffer = 
    &hdrdec_mc_pkt_hdr.buff [hdrdec_mc_pkt_hdr.active_buff_idx].
      carriers [header_info_ptr->demod_id];
    /* Initialize the convenience pointer */

  /* If we have max records in the active buffer, or if the buffer has
     been active for too long, */
  if (active_buffer->rec_cnt == HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE)
  {
    /* Mark the buffer for logging */
    ready_for_logging = TRUE;
    hdrdec_mc_pkt_hdr.buff [hdrdec_mc_pkt_hdr.active_buff_idx].
      ready_for_logging = TRUE;

    hdrdec_mc_pkt_hdr.active_buff_idx = 
      ((hdrdec_mc_pkt_hdr.active_buff_idx == 0)? 1: 0);
      /* Toggle active buffer index */

    hdrdec_mc_pkt_hdr.ts_start =
      (uint16) ( header_info_ptr->hstr +  
                 ( (header_info_ptr->slots_to_decode - 1) * 
                   HDRDEC_NUM_HALF_SLOTS_INTERLACE ) );
      /* Find the HSTR of slot when the packet terminated. We want to log this 
         packet within 32 slots from termination */

    hdrdec_mc_pkt_hdr.ts_valid = TRUE;

    active_buffer = 
      &hdrdec_mc_pkt_hdr.buff [hdrdec_mc_pkt_hdr.active_buff_idx].
        carriers [header_info_ptr->demod_id];
      /* Now that we have toggled the active buffer, Re-initialize the 
         active_buffer convenience pointer */
  }
  else if (hdrdec_mc_pkt_hdr.ts_valid == FALSE)
  {
    /*-------------------------------------------------------------------------
       Active buffer can either be toggled from ship_aged_log_buffer function 
       or this function in the above if block. If the buffer gets toggled from
       ship_aged_log_buffer function, the ts_start marked as invalid needs
       to be initialized here. Set ts_start the first time we enter this 
       function after it was marked invalid 
       ----------------------------------------------------------------------*/

    hdrdec_mc_pkt_hdr.ts_start =
      (uint16) ( header_info_ptr->hstr +  
                 ( ( header_info_ptr->slots_to_decode - 1 ) * 
                   HDRDEC_NUM_HALF_SLOTS_INTERLACE ) );
      /* Find the HSTR of slot when the packet terminated. We want to trigger 
         logging of this packet within 20 slots from termination */

    hdrdec_mc_pkt_hdr.ts_valid = TRUE;
  }

  /* Calculate the index to add the new record at the end of the buf limiting 
   * the mac rec count to 16; the rec_cnt is reset in the function 
   * hdrdec_log_mc_pkt_headers */
  if ( active_buffer->rec_cnt < HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE )
  {
    dec_hdr_info_ptr = &active_buffer->rec[active_buffer->rec_cnt];
      /* Initialize the convenience pointer to start of current record */

    /* Copy the current record into the buffer */
    active_buffer->carrier_id.channel_id = header_info_ptr->demod_id;
    dec_hdr_info_ptr->str = (hdrlog_slot_time_ref_type)
      (header_info_ptr->hstr/2);
    dec_hdr_info_ptr->data_len = (uint8)(header_info_ptr->byte_length/4);
    dec_hdr_info_ptr->drc_value = header_info_ptr->drc_decoded;
    dec_hdr_info_ptr->seq_num = header_info_ptr->sequence;
    dec_hdr_info_ptr->link_id = header_info_ptr->link_id;
    dec_hdr_info_ptr->bit_fields.slots_to_decode = 
      (header_info_ptr->slots_to_decode);

    dec_hdr_info_ptr->bit_fields.crc_status = 
       ( (header_info_ptr->pkt_is_cancelled)? 
         HDRDEC_PKT_CANCELED : header_info_ptr->crc_status );
       /* Indicate Cancelled packet under CRC status column in log packet 
          CRC status - (0)Fail, (1)Pass, (2)False Pass, (3)Pkt Cancelled */

    dec_hdr_info_ptr->bit_fields.pd_type = header_info_ptr->sw_forced_preamble;
       /* Indicates Premable Detection Type*/

    dec_hdr_info_ptr->bit_fields.crc_is_24_bits = header_info_ptr->rev_a;
    dec_hdr_info_ptr->bit_fields.pkt_type = header_info_ptr->pkt_type;

    active_buffer->rec_cnt++;
      /* Increase the rec cnt */
  }
  else
  {
    HDR_MSG_PROT_2(
      MSG_LEGACY_ERROR,
      "Pkt header log buffer full Rec: %d, Slot: %d", 
      active_buffer->rec_cnt, header_info_ptr->hstr/2);
    /* The buf is full; stop logging and dont increase the rec_cnt. 
       This should be unusual */
  }

  return ready_for_logging;

} /* hdrdec_update_mc_pkt_header_buf */


/*============================================================================
  HDRDEC_SET_DECOB_MSG_RATE
*/
/**
  Configures decoder buffer indication message rate. 

  This function programs decoder buffer indication message rate by sending 
  hdrfw_decob_ind_rate_msg_t message. Sends the following message to mDSP. 
    - HDRFW_DECOB_IND_RATE_MSG_T

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_set_decob_msg_rate 
( 
  uint16 ind_rate
    /**< Indication message rate in slots. Use one of the following values. 
          - HDRDEC_IDLE_DECOB_MSG_RATE_V 
          - HDRDEC_TRAFFIC_DECOB_MSG_RATE_V */
)
{
  hdrfw_msg_u fw_cmd;
    /* Message to configure decoder buffer indication message rate */

  errno_enum_type estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  msgr_init_hdr ( 
    &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_DECOB_IND_RATE_CMD );

  fw_cmd.decob_ind_rate.decobIndMsgRate = ind_rate;

  estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
    /* Send the message */

  ASSERT ( estatus == E_SUCCESS );

  HDR_MSG_DRIVERS_1 ( MSG_LEGACY_HIGH, 
                      "Dec indication message rate: %d slots", ind_rate );

} /* hdrdec_set_decob_msg_rate */


/*============================================================================
  HDRDEC_INIT
*/
/**
  Initializes the decoder. 

  Initialized the decoder and decoder driver by 
    - Initializing the watchdog timer
    - Setting up the event queues for in-order and on-arrival delivery.
    - Reset packet sequence, resequencing buffer and pointers
    - Setting the indication message rate. Until activated, the decoder driver
      will flush any packets that arrive in the decoder buffer.  

  @context HDRDEC task. 

  @sideeffects None

  @internal 
    - Internal function. 
    - Move call from tmc_libraries_init_before_tasks_start to hdrdec_task */
/*============================================================================*/
void hdrdec_init ( void )
{
  uint16 buff_idx;
    /* Buffer index - Loop variable */

  uint8 demod_idx;
    /* Demod index - Loop variable */

  uint8 ilace_idx;
    /* Interlace index - Loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrdec.active = FALSE;
    /* Mark decoder as inactive */

  hdrdec.decob_msg_rate = HDRDEC_IDLE_DECOB_MSG_RATE_V;
    /* Initialize decoder buffer indication message rate */

 /* Initialize hdrdec Critical Section*/
  rex_init_crit_sect( &hdrdec.hdrdec_crit_sect );

  ( void ) q_init ( &hdrdec.event_q );
  ( void ) q_init ( &hdrdec.event_free_q );

  /* Fill hdrdec_event_free_q */
  for ( buff_idx = 0; buff_idx < HDRDEC_MAX_NUM_EVENTS; buff_idx++ )
  {
    hdrdec.event_pool[buff_idx].hdr.done_q_ptr = &hdrdec.event_free_q;

    q_put ( &hdrdec.event_free_q, 
            q_link ( &hdrdec.event_pool[buff_idx], 
                     &hdrdec.event_pool[buff_idx].hdr.link ) );
  }

  /* Initialize on-arrival packet delivery queue/free queue */
  ( void ) q_init ( &hdrdec.oapd_q );
  ( void ) q_init ( &hdrdec.oapd_free_q );

  /* Fill OAPD buffer free queue */
  for ( buff_idx = 0; buff_idx < HDRDEC_MAX_NUM_OAPD; ++buff_idx )
  {
    hdrdec.oapd_pool[buff_idx].pkt_ptr = NULL;
    hdrdec.oapd_pool[buff_idx].status = HDRDEC_PKT_WAITING;

    q_put ( &hdrdec.oapd_free_q, 
            q_link ( &hdrdec.oapd_pool[buff_idx], 
                     &hdrdec.oapd_pool[buff_idx].link ) );
  }

  hdrdec.dsm_pool_exhausted_count = 0;
    /* Initialize count to zero */

  hdrdec.deliver_on_arrival = FALSE;
    /* Disable on-arrival delivery by default */

  /* Initialize duplicate DRC1 detection info */
  for ( demod_idx = 0; demod_idx < HDRDEC_MAX_NUM_FL_CARRIERS; ++demod_idx ) 
  {
    for ( ilace_idx = 0; ilace_idx < HDRDEC_NUM_INTERLACES; ilace_idx++ )
    {
      hdrdec.last_pkt_info [demod_idx][ilace_idx].valid = FALSE;
    }
  }

  //hdrdec_reset_pkt_sequences();
    /* Reset packet sequences */

  memset( &(hdrdec_pkt_header_log_buf), 0, sizeof(hdrdec_pkt_header_log_buf));
    /* Clean up the pkt header log buffer */

  memset( &(hdrdec_mc_pkt_hdr), 0, sizeof(hdrdec_mc_pkt_hdr));

} /* hdrdec_init */


/*============================================================================
  HDRDEC_PROCESS_WDOG
*/
/**
  This procedure reports wdog and start wdog timer.

  @todo none

  @context HDRDEC task. 

  @sideeffects None

  @return None */
/*============================================================================*/
void hdrdec_process_wdog( void )
{

  (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_RPT_TIMER_SIG );
  dog_hb_report( DOG_HDRDEC_RPT );
} /* hdrdec_process_wdog */

/*============================================================================
  HDRDEC_WAIT
*/
/**
  This procedure waits for specified signals to be set.  

  This routine also checks any queues that are being waited on.  If the queue 
  has an item, this routine sets the signal without waiting.  This allows for 
  the queue signal to be cleared after each item is removed.  If a queue signal 
  is set, this routine checks the queue and clears the signal if there is 
  nothing on it.  This centralizes the queue checking.

  @todo what queue signals is the comment talking about here? Is this obsolete?

  @context HDRDEC task. 

  @sideeffects None

  @return For non-queue signals, a signal bit is set in the return mask if 
  the signal was in the requested mask and is also set in the REX TCB. For 
  queue signals, a signal bit is set in the return mask if the signal was
  in the requested mask and the queue associated with the signal has an item
  on it. */
/*============================================================================*/
rex_sigs_type hdrdec_wait 
( 
  rex_sigs_type  requested_mask         
    /**< Mask of REX signals to wait for */
)
{
  rex_sigs_type  rex_signals_mask; 
    /* Signal mask returned by rex_get_sigs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Loop until at least one requested signal is set
  -------------------------------------------------------------------------*/
  do 
  {
    /* Actually wait for one of the requested signals to be set */
    rex_signals_mask = rex_wait ( requested_mask | HDRDEC_RPT_TIMER_SIG);

    /*-----------------------------------------------------------------------
     Watchdog report timer signal. Kick watchdog and reset timer.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRDEC_RPT_TIMER_SIG ) != 0 )
    {
      hdrdec_process_wdog();
    }

  } while ( ( requested_mask & rex_signals_mask ) == 0 );

  /*-------------------------------------------------------------------------
   Kick watchdog one last time on the way out.
  -------------------------------------------------------------------------*/
  (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_RPT_TIMER_SIG );
  dog_hb_report ( DOG_HDRDEC_RPT );
  /*-------------------------------------------------------------------------
   Assemble return mask.  Only return signals that were requested.
  -------------------------------------------------------------------------*/
  return ( rex_signals_mask & requested_mask );

} /* hdrdec_wait */


/*============================================================================
  HDRDEC_PKT_IS_DUPLICATE
*/
/**
  Finds whether the decoded packet is duplicate.  

  Forward link packets are turbo-encoded (Code rate 1/5 or 1/3), interleaved, 
  and modulated using QPSK/8-PSK/16-QAM modulation schemes. A repetition of 
  hence generated symbols are transmitted over the nominal transmit duration of 
  the packet, until AT decodes and acks it. For certain forward link packet 
  transmission formats, the Nth repetition of those symbols could start exactly 
  P chips away from slot i, where 
  0 > i < Nominal transmit duration of that packet format and 
  P - Preamble length in chips.  
  and if those P chips trigger a false preamble detect, the rest looks like 
  well-formed packet. Please note that the preamble is not re-transmitted, 
  but rather a false detect.

  For example, let's take DRC 3, 128 bit packet format ([128, 4, 256]). After 
  turbo encoding, the 128-bit packet gets converted to 640 symbols (1/5 code 
  rate) and after QPSK modulation gets converted to 320 chips. Now, the 
  smallest integer i, such that 320 x N = 1600 x i, (where N is an integer), 
  gives us the slot at which the 5th (Nth) repetition of 320 symbol packet 
  overlaps with slot-boundary. Please note that the 1600 denotes the number 
  of data chips in a slot. 

  Another example, For DRC3, 256 bit packet [256, 4, 256], we get 640 chips 
  and the smallest integer i which satisfies equation (640 x N = 1600 x i) is 2
              
  The table below denotes the Transmission formats that could potentially lead 
  to duplicate detection in their transmit duration.

  @verbatim 

  DRC     XFormat           Chips     N     i    
  ===========================================
  0       [128, 16, 1024]   320       5     1
  0       [256, 16, 1024]   640       5     2
  0       [512, 16, 1024]   1280      5     4
  0       [1024, 16, 1024]* 2560      5     8
  -------------------------------------------
  1       [128, 16, 1024]   320       5     1
  1       [256, 16, 1024]   640       5     2
  1       [512, 16, 1024]   1280      5     4
  1       [1024, 16, 1024]* 2560      5     8
  -------------------------------------------
  2       [128, 8, 512]     320       5     1
  2       [256, 8, 512]     640       5     2
  2       [512, 8, 512]     1280      5     4
  -------------------------------------------
  3       [128, 4, 256]     320       5     1
  3       [256, 4, 256]     640       5     2
  -------------------------------------------
  4       [128, 2, 128]     320       5     1

  * - Canonical transmission format.

  @endverbatim 

  Note: Dup detection is needed only for Canonical formats of DRCs 0/1 and 
  Short formats from DRCs 0-4.

  @return TRUE if the decoded packet is duplicate FALSE otherwise */
/*===========================================================================*/
boolean hdrdec_pkt_is_duplicate
(
  dsm_item_type *pkt,
    /**< Packet to be checked for duplicate */
  hdrdec_pkt_info_s_type *pkt_info
    /**< Decoded packet information */
)
{

  uint32 crc_field;
    /* CRC field, unformatted */

  boolean dup_packet = FALSE;
    /* Whether packet has been found to be a duplicate of a 
       packet that was already decoded. */

  uint16 dup_delta_slots;
    /* Working variable used for duplicate packet detection. */

  uint8 interlace = (pkt_info->hstr % 8) / 2;
    /* Interlace of current packet */

  hdrdec_dup_pkt_info_s_type *dup_pkt_info = NULL ;
    /* Convenience pointer */

  uint8 pkt_size_index = 
    (uint8) HDRDEC_PKT_SIZE_TO_INDEX ( pkt_info->byte_length );
    /* Index of packet when arranged by byte-length */

  void* tmp_ptr;
    /* Temporary pointer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// Fix for KW issue
   HDR_ASSERT(  pkt_info->demod_id < HDRDEC_MAX_NUM_FL_CARRIERS );
   dup_pkt_info = &hdrdec.last_pkt_info[pkt_info->demod_id][interlace];

  /* Duplicate packet detection.  Needed only for Canonical DRC 0/1 and 
     short packets for DRCs 0-4  */
  if ( HDRDEC_DUP_DETECTION_IS_NEEDED ( pkt_info->drc_decoded, pkt_size_index ) &&
       ( pkt_info->pkt_type != HDRDEC_QPCH_PKT ) )
  {

    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    tmp_ptr = (void *)( &pkt->data_ptr 
                   [ hdrdec_dup_packet_crc_start [ pkt_size_index ] ] );
    crc_field = *(uint32*) tmp_ptr;
      /* Extract bytes containing CRC from last MAC packet */
    
    /* ------------------------------------------------------------------
      -- Duplicate packet detection --

      The following conditions must be met to detect a duplicate packet:
       1. last packet on current interlace was possible dup type
       2. crc, drc, size, and channel type of that last packet match 
          that of current pkt.
       3. time between packets lines up with times where a dup packet 
          is a possibility. 
     ----------------------------------------------------------------- */
    if ( ( dup_pkt_info->valid ) &&
         ( dup_pkt_info->crc_field == crc_field ) &&
         ( dup_pkt_info->drc_rate == pkt_info->drc_decoded ) &&
         ( dup_pkt_info->channel == pkt_info->channel ) &&
         ( dup_pkt_info->size == pkt_size_index ) )
    {
      dup_delta_slots =
        ( ( ( ( pkt_info->hstr - dup_pkt_info->hstr ) & 0xFFFF ) / 2 ) / 
          HDRDEC_NUM_INTERLACES );
        /* Find delta between this packet and the previous one on this 
           interlace, in slots */

      if ( ( dup_delta_slots < 
             hdrdec_dup_packet_slot_length [ pkt_info->drc_decoded ] ) && 
           ( ( dup_delta_slots % 
               hdrdec_dup_packet_slot_delta [ pkt_size_index ] ) == 0 ) )
      {

       HDR_MSG_PROT_3 ( 
         MSG_LEGACY_HIGH, 
         "Dup pkt dropped. SGId: %d, Slot delta: %d; Dup of HSTR: 0x%x", 
         pkt_info->sched_group_id, dup_delta_slots, dup_pkt_info->hstr );

        dup_packet = TRUE;
      }

    } /* Is it a duplicate packet? */

    /* Decoder packet wasn't a dup. */
    if ( !dup_packet ) 
    {
      /* Update last packet structure with info from the new packet. */
      dup_pkt_info->hstr      = pkt_info->hstr;
      dup_pkt_info->channel   = pkt_info->channel;
      dup_pkt_info->drc_rate  = pkt_info->drc_decoded;
      dup_pkt_info->crc_field = crc_field;
      dup_pkt_info->size      = pkt_size_index;
      dup_pkt_info->valid     = TRUE;
    }

  } /* Is Dup detection needed? */
  else
  {
    dup_pkt_info->valid = FALSE;
      /* Getting any packet with DRC > 4 on an interlace means
         that the next packet (on that interlace) cannot be a dup. */
  }

  return dup_packet;

} /* hdrdec_pkt_is_duplicate */


/*============================================================================
  HDRDEC_DELIVER_PKT_ON_ARRIVAL
*/
/**
  This function enqueues the control channel packet in OAPD queue. 

  Enqueue the decoded packet to an higher priority queue to be delivered 
  immediately on arrival. 

  The resequencing buffer ensures an in-order (in order of preamble detection) 
  delivery of forward link packets. This could occasionally cause poor standby 
  time in some scenarios during reacquisition and handoff when AT does not 
  know the SCC offset. 

  For example, when AT does not know SCC offset and it detects a CC preamble 
  in slot x and another preamble in slot (x+1). Let's say that the former is 
  a false detection and the latter is the actual SYNC capsule which terminates 
  in one slot. However, because of in-order delivery enforced by CBuf, AT does 
  not get to sleep until the first detection is declared bad CRC after 64 slots
  (16 slots interlaced). 

  In order to avoid this scenario HDRRX also maintains an out-of-order delivery 
  queue through which the packets are delivered in the order of packet 
  termination. As soon as the control channel packet terminates, it is copied 
  in to this queue to be forwarded by HDRRX task to CMAC right away. 

  @return TRUE if the packet was enqueued for on-arrival delivery and FALSE 
  otherwise */
/*===========================================================================*/
boolean hdrdec_deliver_pkt_on_arrival 
(
  dsm_item_type *cc_pkt_ptr,
    /**< Packet to be delivered on arrival */
  int packet_seq
    /**< Sequence number of the packet to be delivered on arrival */
)
{
  hdrdec_oapd_s_type *buf_ptr;
    /* Pointer to On-arrival packet delivery buffer fetched from Queue */

  boolean delivered = FALSE;
    /* Did we deliver the packet on arrival */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( buf_ptr = 
         (hdrdec_oapd_s_type*) q_get ( &hdrdec.oapd_free_q ) ) == NULL )
  {
    HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, 
                     "No free buffers on oapd_free_q. oapd_q_cnt: %d",
                     q_cnt ( &hdrdec.oapd_q ) );
  }
  else
  {
    /* Queue the packet for immediate on-arrival delivery */
    buf_ptr->pkt_ptr = cc_pkt_ptr;
    buf_ptr->status = HDRDEC_CC_PKT_READY;
    q_put ( &hdrdec.oapd_q, &buf_ptr->link );

    delivered = TRUE;
      /* Packet will be delivered right away. Set the flag */
  }

  return delivered;

} /* hdrdec_deliver_pkt_on_arrival */


/*============================================================================
  HDRDEC_QUEUE_CMD
*/
/**
  Queues a command to HDRDEC event queue 

  This function queues a command on the hdrdec.event_q and sets the signal. The 
  command data is copied into the command buffer before it is queued. This 
  function is called in the task context of whoever is giving the command. This 
  routine is re-entrant, and two different tasks may give commands to HDRDEC 
  task at the same time. cmd_size must not be larger than hdrdec_cmd_s_type, 
  because this is the data type the command gets copied into. */
/*===========================================================================*/
void hdrdec_queue_cmd
(
  hdrdec_cmd_s_type *input_cmd_ptr,
    /**< Pointer to a union of all protocols in HDRDEC task */
  uint32 cmd_size
    /**< Size of the command being given */
)
{

  hdrdec_event_s_type *event_ptr = NULL;
    /* Pointer to an available event */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( input_cmd_ptr == NULL )
  {
    HDR_MSG_DRIVERS ( MSG_LEGACY_ERROR, "No command to queue!" );
    return;
  }

  ASSERT ( cmd_size <= sizeof ( hdrdec_cmd_s_type ) );

  /* Get a buffer from the event free queue */
  if ( ( event_ptr = 
         ( hdrdec_event_s_type* ) q_get ( &hdrdec.event_free_q ) ) == NULL )
  {
    HDR_MSG_DRIVERS ( MSG_LEGACY_ERROR, "No free buffers on hdrdec event free q" );
    return;
  }

  /* Fill in the buffer.  */

  event_ptr->hdr.type_of_item = HDRBUF_COMMAND;
#ifdef FEATURE_MEMCPY_REMOVAL
  event_ptr->event.cmd = *input_cmd_ptr;
#else
  (void) memcpy ( &event_ptr->event.cmd, input_cmd_ptr, cmd_size );
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Queue the buffer and set the signal */

  (void) q_link ( event_ptr, &event_ptr->hdr.link );
  q_put ( &hdrdec.event_q, &event_ptr->hdr.link );
  (void) rex_set_sigs ( HDRDEC_TASK_PTR, HDRDEC_EVENT_SIG );

} /* hdrdec_queue_cmd */


/*============================================================================
  HDRDEC_MSG_INIT
*/
/**
  Initializes the message interface. 

  Initializes message interface by doing the following. 
    - Creates a messenger Client ID for Decoder module. 
    - Initializes the free queue and event queue. 
    - Registers the rex queues for the client with the messenger. 
    - Registers to receive indication messages. 

  @todo Understand if there is a need to specify offset to legacy cmd type 
  argument 7 

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_msg_init ( void )
{

  errno_enum_type  estatus;
    /* Error status */

  msgr_id_t queue_id;
    /* Queue Identifier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  estatus = msgr_client_create ( &hdrdec.msgr_client_id );

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_client_add_rex_q ( &hdrdec.msgr_client_id,
                                    HDRDEC_TASK_PTR,
                                    HDRDEC_EVENT_SIG,
                                    &hdrdec.event_q,
                                    &hdrdec.event_free_q,
                                    MSGR_OFFSETOF( hdrdec_event_s_type, event ),
                                    MSGR_OFFSETOF( hdrbuf_hdr_type, type_of_item ),
                                    (uint16) sizeof( hdrdec_event_s_type ),
                                    &queue_id );
    /* Create the msgr client and associated with rex queues */
    /** @todo Understand if there is a need to specify offset to
     *  legacy cmd type argument 7 */

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_register ( MSGR_HDR_DEC,
                            &hdrdec.msgr_client_id,
                            MSGR_ID_REX,
                            HDR_FW_DECOB_STATUS_IND );
    /* Register for messages */

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_register ( MSGR_HDR_DEC,
                            &hdrdec.msgr_client_id,
                            MSGR_ID_REX,
                            HDR_FW_CANCEL_ACTIVE_PACKET_RSP );
    /* Register for messages */

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_register ( MSGR_HDR_DEC,
                            &hdrdec.msgr_client_id,
                            MSGR_ID_REX,
                            HDR_FW_LOG_BUFFER_IND );
    /* Register for messages */

  ASSERT ( estatus == E_SUCCESS );

} /* hdrdec_msg_init */
 

/*============================================================================
  HDRDEC_MSG_STOP
*/
/**
  Cleansup the message interface. 

  Cleans up the message interface by doing the following. 
    - De-registers the registered messages. 
    - Deletes the messenger Client ID for Decoder module. 

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_msg_stop ( void )
{

  errno_enum_type  estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrdec.msgr_client_id == 0 )
  {
    HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "MSGR already deregistered" );
    return;
  }

  estatus = msgr_deregister ( MSGR_HDR_DEC,
                              &hdrdec.msgr_client_id,
                              HDR_FW_DECOB_STATUS_IND );

  estatus |= msgr_deregister ( MSGR_HDR_DEC,
                              &hdrdec.msgr_client_id,
                              HDR_FW_CANCEL_ACTIVE_PACKET_RSP );

  estatus |= msgr_deregister ( MSGR_HDR_DEC,
                              &hdrdec.msgr_client_id,
                              HDR_FW_LOG_BUFFER_IND );

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_client_delete ( &hdrdec.msgr_client_id );

  ASSERT ( estatus == E_SUCCESS );

} /* hdrdec_msg_stop */


/*============================================================================
  HDRDEC_SCAN_FOR_NEXT_BLOCK
*/
/**
  Scans for the next decoder buffer block ready to be read.

  The scanning loop starts from the bit position given by 'start' and exits 
  when no more bits are set.

  Please note that the call to this function from hdrdec_setup_decob_read 
  scans from HDRDEC_DECOB_PENDING_MSB (23). i.e. The first call
  to this function scans from MSB and subsequent calls scan from where it 
  stopped during previous call.

  @assumptions There is at least one pending block to be processed in the 
  decoder buffer

  @context HDRDEC 

  @return Block index of the next available buffer */
/*===========================================================================*/
uint16 hdrdec_scan_for_next_block 
(
  uint16 start
    /**< Starting block index of the scanning loop */
)
{
  int blk_ind;
    /* Loop index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  for ( blk_ind = start; ( blk_ind >= 0 ); --blk_ind )
  {
    /* Check if the decoder block at "blk_ind" has packets ready to be read. */
    if ( HDRDEC_DECOB_BLOCK_HAS_PACKETS_READY ( hdrdec.decob_pending, blk_ind ) )
    {
      return blk_ind;
    }

    /* Check for next block */
  }

  return HDRDEC_DECOB_INVALID_BLOCK_INDEX;
    /* NOT REACHED! See Assumption */

} /* hdrdec_scan_for_next_block */


/*============================================================================
  HDRDEC_SETUP_DECOB_READ
*/
/**
  Starts a decoder buffer read session.

  This interface starts a read session by initializing the decoder internal
  variables. This function must be called before reading data out of the 
  decoder.  Once it is called, all the frames currently in the decoder buffer
  (the exact number is returned by this function) *must* be read out.

  @context HDRDEC 

  @return Number of decoder frames ready to be read from the decoder buffer */
/*===========================================================================*/
uint16 hdrdec_setup_decob_read 
(
  hdrfw_msg_ind_u *msg_ptr
    /**< Pointer to the struct holding the firmware message */
)
{

  hdrdec.decob_pending = hdrdec.decob_clear = msg_ptr->decob_status.decobPending;
    /* Read decobPending into both the variables */

  hdrdec.frames = (uint16) hdrutil_bit_count32 ( hdrdec.decob_pending );
    /* Count the number of bits set in decob_pending */

  /* If the number of frames to be read is zero,
     Print message to detect stray decoder message. */
  if ( hdrdec.frames == 0 )
  {
    HDR_MSG_DRIVERS_1 ( MSG_LEGACY_HIGH,
                        "Stray decob pending message. Frames: %d",
                        hdrdec.frames );
  }
  else
  {
    hdrdec.next_block_index =
      hdrdec_scan_for_next_block ( HDRDEC_DECOB_PENDING_MSB );
      /* Now that we know that there is at least one pending block in the decoB,
         Get the next available block index. During setup, we start scanning
         from MSB (23) position of decobPending */

    hdrdec.decob_word_offset =
      HDRMDSP_DECOB_WORD_OFFSET_FROM_BLK_INDEX ( hdrdec.next_block_index );
  }

  return hdrdec.frames;
    /* Number of decoder frames available */

} /* hdrdec_setup_decob_read */

/*============================================================================
  HDRDEC_JUMP_TO_NEXT_DECOB_BLOCK
*/
/**
  Shifts the internal read pointer to next decob block. 

  Marks the block indicated by current block index as done and moves the block 
  index and word offset to next block with pending packet. 

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_jump_to_next_decob_block ( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDRDEC_BLOCK_READ_DONE ( hdrdec.decob_pending, hdrdec.next_block_index );
    /* Clear the bit in decobPending to mark that we are done with the
       current block. Note that we could either have completed reading
       the block or skipped the block to avoid race condition! */

  if ( HDRDEC_DECOB_HAS_MORE_PKTS ( hdrdec.decob_pending ) )
  {
    /* Now that we know that there is at least one pending block in the decoB,
       Get the next available block index. */

    hdrdec.next_block_index =
      hdrdec_scan_for_next_block ( hdrdec.next_block_index );

    hdrdec.decob_word_offset = 
      HDRMDSP_DECOB_WORD_OFFSET_FROM_BLK_INDEX ( hdrdec.next_block_index );
      /* Update the word offset to start of current block */
  }
  else
  {
    hdrdec.next_block_index = HDRDEC_DECOB_INVALID_BLOCK_INDEX;
    hdrdec.decob_word_offset = HDRDEC_DECOB_INVALID_OFFSET;
  }

} /* hdrdec_jump_to_next_decob_block */

/*============================================================================
  HDRDEC_FLUSH_PACKET
*/
/**
  This routine just jumps to the next decoder block. 

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_flush_packet( void )
{

  hdrdec_jump_to_next_decob_block();

} /* hdrdec_flush_packet */


/*============================================================================
  HDRDEC_READ_NEXT_HEADER
*/
/**
  Reads and parses the next decoder packet header. The supplied structure is 
  filled in with decoder info. 

  @assumptions Must be called after either hdrdec_setup_decob_read() or 
  hdrdec_read_next_packet().

  @context HDRDEC */
/*===========================================================================*/

void hdrdec_read_next_header
( 
  hdrdec_pkt_info_s_type *pkt_info 
    /**< Structure that is filled in with data from the decoder header. */
) 
{
  uint32 word_length;
    /* Length of the decoded packet in 32-bit words */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Why ASSERTing makes sense here?
     This function is called from hdrdec_process_decob_pending_ind as many times 
     as the number of bits set in decob_pending and for each call the bit 
     corresponding to that block is cleared. So, we should never get here 
     when decob_pending is zero. If decob_pending is zero, the word_offset 
     and block_index are initialized to -1. Proceeding further with these 
     values would mess-up the address computation below and would cause 
     AT to crash. If pending is zero and we get here that means the decob 
     pending and num_dec_pkts (derived from num bits set in decob_pending) 
     maintained by hdrrx_isr are inconsistent. A graceful way to handle this 
     is to skip reading the block. However, if we did that, it might introduce 
     a hole in the Rx CBUF, which would block the Rx processing and eventually 
     start dropping packets making it hard to debug. */
  ASSERT ( hdrdec.decob_pending != 0 );

#ifndef FEATURE_HDR_DIME_MODEM
  HWIO_DECOB_LONG_BUFFER_ADDR_OUT ( hdrdec.decob_word_offset );
    /* Sync DDO to the beginning of the next packet */

  /* Read header */
  hdrdec.hw_header.header0 = 
    ( *( (volatile dword *) ( HWIO_DECOB_LONG_BUFFER_DATA_ADDR ) ) ); 
  hdrdec.hw_header.header1 = 
    ( *( (volatile dword *) ( HWIO_DECOB_LONG_BUFFER_DATA_ADDR ) ) ); 
  hdrdec.hw_header.header2 = 
    ( *( (volatile dword *) ( HWIO_DECOB_LONG_BUFFER_DATA_ADDR ) ) ); 
  hdrdec.hw_header.header3 = 
    ( *( (volatile dword *) ( HWIO_DECOB_LONG_BUFFER_DATA_ADDR ) ) ); 
#else
  /* Set up pointer */
  hdrdec.decob_ptr = (volatile uint32*) HDRDEC_DEC_BUF_START_ADDR;
  
#ifdef FEATURE_HDR_THOR_MODEM
#ifdef FEATURE_HDR_RUMI
  HDR_MSG_DRIVERS_2( MSG_LEGACY_MED, "decoder start:%x valid:%x", 
                     hdrdec.decob_ptr,
                     HDRMDSP_GET_DECOB_ADDR_VALID( ) );
#endif /* FEATURE_HDR_RUMI */
#else
#ifdef FEATURE_HDR_RUMI
  HDR_MSG_DRIVERS_3( MSG_LEGACY_MED, "decoder start:%x offset:%x valid:%x", 
                     hdrdec.decob_ptr,
                     HDRMDSP_GET_DECOB_OFFSET( ), 
                     HDRMDSP_GET_DECOB_OFFSET_VALID( ) );
#endif /* FEATURE_HDR_RUMI */
    
#ifdef FEATURE_HDR_BOLT_MODEM
  hdrdec.decob_ptr += HDRMDSP_GET_DECOB_OFFSET( );
#else
  hdrdec.decob_ptr += HDRFW_DECOB_MEMPL_BRDG_1_WORD_OFFSET;
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif /* FEATURE_HDR_THOR_MODEM */

  hdrdec.decob_ptr += hdrdec.decob_word_offset; 

  /* Read header */
  hdrdec.hw_header.header0 = *( (volatile uint32*)hdrdec.decob_ptr++ ); 
  hdrdec.hw_header.header1 = *( (volatile uint32*)hdrdec.decob_ptr++ ); 
#endif /* FEATURE_HDR_DIME_MODEM */

  /* Read the FW decoder header from DSP memory */
  hdrdec.fw_header.header0 = 
    HDRMDSP_GET_FW_DEC_HEADER_N ( hdrdec.next_block_index, 0 );
  hdrdec.fw_header.header1 = 
    HDRMDSP_GET_FW_DEC_HEADER_N ( hdrdec.next_block_index, 1 );
  hdrdec.fw_header.header2 = 
    HDRMDSP_GET_FW_DEC_HEADER_N ( hdrdec.next_block_index, 2 );

  pkt_info->pkt_type = HDRDEC_GET_PKT_TYPE ( hdrdec.fw_header );

#ifdef FEATURE_HDR_REVB

  /* Read the Demod Id, Scheduler Group ID and the Number of Concurrent 
     packets */
  pkt_info->sched_group_id = 
    (uint8) HDRDEC_GET_SCHED_GROUP_ID ( hdrdec.fw_header );
  pkt_info->demod_id = HDRDEC_GET_DEMOD_INDEX ( hdrdec.fw_header );
  pkt_info->num_concurrent_pkts = 
    HDRDEC_GET_NUM_CONCURRENT_DECODES ( hdrdec.fw_header );
 
#endif /* FEATURE_HDR_REVB */

  /* Parse hardware header info into packet info structure */  
  word_length = HDRDEC_GET_PKT_WORD_LENGTH ( hdrdec.hw_header );
  pkt_info->crc_status = (hdrdec_crc_status_e_type)    
    HDRDEC_PKT_HAS_PASSED_CRC_AND_MIN_LLR ( hdrdec.fw_header );

  /* Parse firmware header info into packet info structure */
  pkt_info->pkt_is_cancelled = HDRDEC_SEQ_IS_CANCELLED ( hdrdec.fw_header );
  pkt_info->drc_decoded = HDRDEC_GET_DECODED_DRC ( hdrdec.fw_header );
  pkt_info->drc_requested = HDRDEC_GET_REQUESTED_DRC ( hdrdec.fw_header );
  pkt_info->hstr = HDRDEC_GET_PKT_HSTR_COUNT ( hdrdec.fw_header );
  pkt_info->link_id = HDRDEC_GET_LINK_ID ( hdrdec.fw_header );
  pkt_info->sequence = HDRDEC_GET_SEQUENCE ( hdrdec.fw_header );
  pkt_info->channel = HDRDEC_GET_PKT_CHANNEL ( hdrdec.fw_header );
  pkt_info->slots_to_decode = 
    ( HDRDEC_GET_SLOT_INDEX ( hdrdec.fw_header ) + 1 );

  pkt_info->sw_forced_preamble = (HDRDEC_PKT_IS_TC_FPD ( hdrdec.fw_header ))?
     HDRDEC_PD_FORCED_TC :
     HDRDEC_PKT_IS_CC_FPD ( hdrdec.fw_header );

  pkt_info->rev_a = HDRDEC_PKT_IS_REV_A ( hdrdec.fw_header );

  pkt_info->byte_length = ( word_length * HDRDEC_BYTES_PER_WORD );

  hdrdec.crc_len = 
    ( ( pkt_info->rev_a )? HDRDEC_REV_A_CRC_LEN: HDRDEC_REV_0_CRC_LEN );
    /* Get CRC Length in bytes from PHY revision of the packet */

  /* Sanity check the packet length read from hw header. */
  if ( word_length == 0 )
  {
    HDR_MSG_DRIVERS_1( MSG_LEGACY_ERROR, 
                     "Bad word length! Discarding.", word_length );

    /* Dump header for possible analysis */
    HDR_MSG_DRIVERS_4( MSG_LEGACY_HIGH, 
                     "Dec HW header: %lx, %lx, %lx, %lx ", 
                     hdrdec.hw_header.header0, hdrdec.hw_header.header1, 
                     hdrdec.hw_header.header2, hdrdec.hw_header.header3 );

    HDR_MSG_DRIVERS_3( MSG_LEGACY_HIGH, 
                     "Dec FW Header: %lx, %lx, %lx",
                     hdrdec.fw_header.header0, hdrdec.fw_header.header1,
                     hdrdec.fw_header.header2);

    /* Give valid word length since 0 isn't valid. */
    word_length = 32;
    pkt_info->crc_status = HDRDEC_CRC_FAIL;
  }

  /* Find out the size of the reads from the buffer and the number of 
     read function calls required. */
  if ( word_length > HDRDEC_MAX_READ_LENGTH ) 
  {
    hdrdec.num_mac_pkts = ( word_length / HDRDEC_MAX_READ_LENGTH );
    hdrdec.bytes_read = ( HDRDEC_MAX_READ_LENGTH * HDRDEC_BYTES_PER_WORD ); 
  }
  else
  {
    hdrdec.bytes_read = ( word_length * HDRDEC_BYTES_PER_WORD );
    hdrdec.num_mac_pkts = 1;
  }

  pkt_info->num_mac_pkts = hdrdec.num_mac_pkts;
 

    /* Don't bother to read bad CRC packets or Canceled packets. Note that
       the CRC bit is not guaranteed to be 0/1 when a packet is canceled. */
  if ( ( pkt_info->crc_status != HDRDEC_CRC_PASS ) || 
       ( pkt_info->pkt_is_cancelled ) )
  {
    pkt_info->num_mac_pkts = 0;
      /* Read zero MAC packets when CRC fails or packet is canceled */

    hdrdec_jump_to_next_decob_block();
      /* For failure, we will not read the packet. Skip this block and
         jump to next */

    HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, 
      "No CRC pass or packet cancelled!" );
  }

} /* hdrdec_read_next_header */

/*============================================================================
  HDRDEC_READ_NEXT_PACKET 
*/
/**
  Reads the decoder packet at the block "hdrdec.next_block_index" in decoB.

  @assumptions Must always be called after hdrdec_read_next_header

  @context HDRDEC 

  @return Number of data bytes read from the decoder */
/*===========================================================================*/
uint16 hdrdec_read_next_packet
(
  byte * data_ptr,
    /**< Array of bytes to fill-in the data read from the decoder block. */

  uint32 n_128b_chunks
    /**< Number of 128b chunks to read. */
)
{
  uint32* packet_ptr;
    /* Pointer to destination address for decoder data */
  
  uint16 bytes_read = 0;
    /* Holds return value -- the number of data bytes read */

  int word_offset;
    /* Loop counter */

  void* tmp_ptr;
    /* Temporary pointer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrdec.num_mac_pkts )
  {
    /* Use void pointer as a bridge to avoid the following compiler warning: 
       "cast increases required alignment of target type". */
    tmp_ptr = (void *)data_ptr;
    packet_ptr = (uint32*)tmp_ptr;
      /* Set up convenience pointer for destination buffer */
    
    /* Copy one word at a time from DDO interface */
    for( word_offset = 0; word_offset < (HDRDEC_MAX_READ_LENGTH * n_128b_chunks); word_offset++ )
    {
/* TBD need to update for Dime */
#ifndef FEATURE_HDR_DIME_MODEM
      *packet_ptr++ = *( (volatile uint32*) HWIO_DECOB_LONG_BUFFER_DATA_ADDR );
#else
      *packet_ptr++ = *( (volatile uint32*)hdrdec.decob_ptr++ );
#endif /* FEATURE_HDR_DIME_MODEM */
    }

    /* Decrement packet read count */
    hdrdec.num_mac_pkts -= n_128b_chunks;

    /* Set up return value */
    bytes_read = (hdrdec.bytes_read * n_128b_chunks);

    /* If this is the last read for a decoder frame or the frame is Rev 0,
       subtract CRC bytes from the read count */
    if ( ( hdrdec.crc_len == HDRDEC_REV_0_CRC_LEN ) || 
         ( hdrdec.num_mac_pkts == 0 ) )
    {
      bytes_read -= ( hdrdec.crc_len + 1 );
    }

    /* If this is the last read for a frame, update the buffer pointer to 
       point to the next packet. */
    if ( hdrdec.num_mac_pkts == 0 )
    { 
      hdrdec_jump_to_next_decob_block();
    }

  } /* If we have packet left to read */

  return bytes_read;
    /* Returns the number of bytes copied into the dest_ptr */

} /* hdrdec_read_next_packet */


/*============================================================================
  HDRDEC_END_READ
*/
/**
  Complete the decoder buffer read session.

  Completes the decoder buffer read session by sending HDRFW_DECOB_UPDATE_MSG_T
  message.

  @context HDRDEC 

  @return Number of decoder frames ready to be read from the decoder buffer */
/*===========================================================================*/
void hdrdec_end_read ( void )
{

  hdrfw_msg_u decob_update_cmd;
    /* Message to inform fw about the decoder block ready for new packets. */

  errno_enum_type estatus;
    /* Error status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  msgr_init_hdr ( 
    &decob_update_cmd.hdr, MSGR_HDR_DEC, HDR_FW_DECOB_UPDATE_CMD );

  decob_update_cmd.decob_update.decobClear = hdrdec.decob_clear;

  hdrdec.frames = 0;
    /* Clear local frame count. */

  estatus = msgr_send ( &decob_update_cmd.hdr, sizeof ( hdrfw_msg_u ) );
    /* Send the message */

  ASSERT ( estatus == E_SUCCESS );

} /* hdrdec_end_read */


/*============================================================================
  HDRDEC_GATHER_CC_INFO_FOR_SLEEP_LOG
*/
/**
  Gathers information from the current control channel log packet for 
  Enhanced Sleep Info log packet.

  @context HDRDEC 

  @return TRUE if AT lost sync CC and FALSE otherwise */
/*===========================================================================*/
boolean hdrdec_gather_cc_info_for_sleep_log 
(
  dsm_item_type *cc_pkt_ptr,
    /**< Pointer to DSM item containing the control channel packet */
  uint16 num_dec_pkts,
    /**< Number of decoder packets left to read in the current session */
  hdrdec_crc_status_e_type crc_status,
    /**< CRC status of the packet */
  hdrdec_half_slot_time_type hstr9
    /* 9-bit Half slot time reference */
)
{
  boolean lost_sync_cc = FALSE;
    /* Did AT lose Sync CC? */

  boolean is_mer_detected = FALSE;
    /* Is MER detected? */

  boolean log_cc_info = FALSE;
    /* Do we need to log information about this CC packet? */

  boolean is_subsync = FALSE;
    /* Is the CC packet subsync? */

  uint16 cur_hstr = HDRHSTR_GET_COUNT();
    /* Current half slot time reference */

  uint16 scc_packet_num = 1;
    /* Refers to nth packet in SCC Capsule. Always 1 for subsync. */

  byte *pkt_hdr_ptr = NULL;
    /* Pointer to packet data */

  boolean pkts_pending = FALSE;
    /* Do we have any packet pending in the resequencing buffer? */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  is_mer_detected = FALSE;
    /* Reset flag */

  /* Figure out whether to log this packet info with sleep, and if so
     whether to increase MER count, and the packet number. */

  if ( hdrdec.pkt_info.pkt_type == HDRDEC_QPCH_PKT )
  {
    scc_packet_num = 0;
    log_cc_info = TRUE;
  }
  else
  {
    if (( crc_status == HDRDEC_CRC_PASS ) && ( cc_pkt_ptr != NULL ))
    {
      pkt_hdr_ptr = cc_pkt_ptr->data_ptr;
        /* Initialize pointer to packet data. CC MAC is going to peek at the CC
           header. Absence of data ptr implies CRC failure to hdrcmac function 
           below. */
    }

    log_cc_info = hdrcmac_get_cc_packet_num ( hstr9,
                                              hdrdec.pkt_info.drc_decoded,
                                              pkt_hdr_ptr,
                                              &scc_packet_num,
                                              &is_subsync );
      /* Find packet number and if packet was a sync/subsync packet (in both 
         cases we want to log it). */

  } /* Not QPCH packet */

  if ( crc_status != HDRDEC_CRC_PASS )
  {
    pkts_pending = ( ( num_dec_pkts > 0 ) || ( hdrdec.buff_entry_cnt > 1 ) );
      /* Do we have any packets pending in the decoder buffer or CBuf
         yet to be processed? */

    is_mer_detected = 
      hdrcmac_process_bad_crc ( ( hstr9 / 2 ),
                                pkts_pending, 
                                hdrdec.pkt_info.pkt_type );
      /* Process bad control channel packet */
  }

  if ( is_mer_detected )
  {
    lost_sync_cc = TRUE;
      /* Trigger sync cc lost indication at end of function */
  }

  if ( log_cc_info )
  {
    uint16 cc_start = hdrsrch_get_cc_start();

    hdrsrchlog_update_cc_info ( HDRDEC_CALC_CC_TO_CCDEC ( cur_hstr, cc_start ),
                                (uint8) hdrdec.pkt_info.drc_decoded,
                                hdrdec.pkt_info.slots_to_decode,
                    (hdrdec.pkt_info.sw_forced_preamble == HDRDEC_PD_FORCED_CC),
                                is_mer_detected,
                                scc_packet_num,
                                is_subsync );
      /* Update the control channel infomation for SLEEP INFO log packet */
  }

  return lost_sync_cc;

} /* hdrdec_gather_cc_info_for_sleep_log */


/*============================================================================
  HDRDEC_HANDLE_CC_PKT
*/
/**
  Handle control channel packets

  Set channel and traffic bits in DSM of the control channel packet.
  Detects if a synchronous control channel capsule was lost on receiving a 
  bad control channel packet.
  Enqueue packet to OAPD queue if it qualifies for a high priority delivery.
  Gather information for sleep logging.

  @context HDRDEC 

  @return TRUE if we lost synchronous control channel capsule and FALSE 
  otherwise. */
/*===========================================================================*/
boolean hdrdec_handle_cc_pkt 
(
  hdrdec_crc_status_e_type crc_status,
    /**< CRC status of the packet */
  uint16 num_dec_pkts,
    /**< Number of decoder packets */
  dsm_item_type **cc_pkt_ptr,
    /**< Pointer to DSM item with the CC packet */
  hdrdec_pkt_status_e_type *pkt_status,
    /**< Pointer to packet Status */
  hdrdec_half_slot_time_type hstr9
    /* 9-bit Half slot time reference */
)
{

  boolean lost_sync_cc = FALSE;
    /* Did AT lose Sync CC? */

  hdrlog_cc_status_enum_type cc_status;
    /* CC CRC status for logging purposes */

  int packet_seq = HDRDEC_SEQ_INDEX ( hdrdec.pkt_info.sequence );
    /* Packet sequence number */

  dsm_item_type *log_pkt_ptr = *cc_pkt_ptr;
    /* Copy of pointer for logging */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  hdrsrch_notify_cc_packet();
    /* Notify searcher that a CC packet has been received so that SRCH can 
       re-enable missed CC ISR if necessary because forced premable detection 
       could have disabled missed CC ISR */

  if ( crc_status == HDRDEC_CRC_PASS )
  {
    HDR_MSG_PROT_5 (
      MSG_LEGACY_HIGH,
      "Good CC ISR R=%d slots=%d hstr9=%d hstr16=%d ForcedPreamble= %d",
      hdrdec.pkt_info.drc_decoded, hdrdec.pkt_info.slots_to_decode,
      hstr9, hdrdec.pkt_info.hstr, hdrdec.pkt_info.sw_forced_preamble );

#ifdef FEATURE_HDR_RUMI
    hdrhit_rumi_cfg.cc_pkt_hstr = hdrdec.pkt_info.hstr;
      /* This assume scc offset is 0 */
#endif /* FEATURE_HDR_RUMI */

    cc_status = HDRLOG_CC_GOOD_CRC;

    HDRDSM_SET_CHAN_BIT ( *cc_pkt_ptr );
      /* Indicates control channel packet. No need to loop here. There 
         can be only one CC packet per decoder frame */

    /* Check to see if CC packet was received while in traffic */
    if ( hdrfmac_is_active ( HDRHAI_DECODER ) )
    {
      HDRDSM_SET_TRAFFIC_BIT ( *cc_pkt_ptr );
    }

    if ( ( hdrdec.deliver_on_arrival ) && 
         ( hdrdec_deliver_pkt_on_arrival ( *cc_pkt_ptr, packet_seq ) ) )
    {
      *pkt_status = HDRDEC_CC_PKT_DELIVERED_ON_ARRIVAL;
        /* Fill the hole in re-sequencing buffer */

      *cc_pkt_ptr = NULL;
    }

  } /* If CRC passed */
  else
  {
    HDR_MSG_PROT_5 (
      MSG_LEGACY_HIGH,
      "Bad CC R=%d slots=%d hstr9=%d hstr16=%d ForcedPreamble= %d",
      hdrdec.pkt_info.drc_decoded, hdrdec.pkt_info.slots_to_decode,
      hstr9, hdrdec.pkt_info.hstr, hdrdec.pkt_info.sw_forced_preamble );

    cc_status = ( ( crc_status == HDRDEC_CRC_FALSE_PASS )? 
                    HDRLOG_CC_FALSE_CRC: HDRLOG_CC_BAD_CRC );

  } /* CRC failed */

  hdrrxstats_update_cc_status ( cc_status );
    /* Update CC Status for logging */

  lost_sync_cc = 
    hdrdec_gather_cc_info_for_sleep_log ( 
      log_pkt_ptr, num_dec_pkts, crc_status, hstr9 );
    /* Gather information from this CC packet for Sleep logging */

  return lost_sync_cc;

} /* hdrdec_handle_cc_pkt */

/*============================================================================
  HDRDEC_SET_FW_LOGGING_CB
*/
/**
  Set callback for FW subframe logging.

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_set_fw_logging_cb
( 
  void(*fw_log_cb)(int active_buffer)
  /**< Logging callback for subframe logging. */
)
{
  hdrfw_msg_u fw_cmd;
    /* Message to configure decoder buffer indication message rate */

  errno_enum_type estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrdec_fw_log_cb = fw_log_cb;
    /* Record pointer to logging callback function. */

  msgr_init_hdr ( 
    &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_INDICATION_CFG_CMD );

  fw_cmd.indication_cfg.type = HDR_FW_IND__LOG_BUFFER;

  if (fw_log_cb != NULL )
  {
    fw_cmd.indication_cfg.enable = 1;
    HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH,"Enabling FW logging indication" );
  }
  else
  {
     fw_cmd.indication_cfg.enable = 0;
     HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH,"Disabling FW logging indication" );
  }

  estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
    /* Send the message */

  ASSERT ( estatus == E_SUCCESS );

} /* hdrdec_set_fw_logging_cb */


/*============================================================================
  HDRDEC_PROCESS_LOG_BUFFER_IND
*/
/**
  Process HDRDEC_PROCESS_LOG_BUFFER_IND indication message.

  Processes the logging indication message from FW.  Currently run in 
  decoder task.   (Was ISR context previously.)

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_process_log_buffer_ind
(
  hdrfw_msg_ind_u *msg_ptr
    /**< Pointer to the struct holding the firmware message */
)
{

  int active_buffer;
    /* active log buffer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  active_buffer = (int)((hdrfw_log_buffer_ind_msg_t*)msg_ptr)->bufferIndex;
    /* Retrieve active_buffer from message */

  if (hdrdec_fw_log_cb != NULL)
  {
    hdrdec_fw_log_cb( active_buffer );
      /* Call callback, passing the active buffer for logging use. */
  }

}

/*============================================================================
  HDRDEC_PROCESS_DECOB_PENDING_IND
*/
/**
  Process HDRFW_DECOB_STATUS_IND_MSG_T indication message.

  Reads the packet out of the decoder buffer and stores into the resequencing 
  buffer, if the decoder packet header indicates that the decoder
  packet CRC has passed.  Otherwise the corresponding packets are discarded.
  Signals HDRRX task with HDRRX_PKT_READY_SIG to notify that packets are ready
  for processing in resequencing buffer.

  @todo move TRAFFIC_BIT to user field?
  @todo Change this to dsm_new_buffer flexible API once it is available on 2H09
  @todo Move HDRDSM_SET_MAC_TRAILER to user field?A
  @todo add some kind of stats for cancelled pkts

  @context HDRDEC */
/*===========================================================================*/

void hdrdec_process_decob_pending_ind
(
  hdrfw_msg_ind_u *msg_ptr
    /**< Pointer to the struct holding the firmware message */
)
{

  uint16 num_dec_pkts = 0;
    /* Number of decoder packets - Working variable */

  int packet_seq = 0;
    /* Packet sequence number */

  hdrdec_pkt_channel_e_type channel;
    /* Forward link channel on which the packet was received */

  hdrdec_pkt_status_e_type pkt_status;
    /* Packet Status */

  hdrdec_update_fl_stats_f_type update_stats_fptr = NULL;
    /* Pointer to function that updates statistics */

  hdrdec_crc_status_e_type crc_status = HDRDEC_CRC_FAIL;
    /* CRC status of the packet */

  uint16 pkt_size_index = 0;
    /* Packet size index */

  hdrdec_half_slot_time_type hstr9 = 0;
    /* 9-bit Half slot time reference */

  dsm_item_type *prev_dsm_ptr = NULL;
    /* Pointer to previous DSM item in the chain */

  dsm_item_type *root_dsm_ptr = NULL;
    /* Pointer to head DSM item in the chain */

  dsm_item_type *dsm_item_ptr = NULL;
    /* Pointer to current DSM item */

  uint32 app_field = 0;
    /* User field for app field */

  uint8 kind_field = 0;
    /* User field for kind field */

  uint32 app_ptr_field = 0;
    /* User field for app_ptr field */

  boolean lost_sync_cc = FALSE;
    /* Did AT lose Sync CC? */

  boolean duplicate_pkt = FALSE;
    /* Is this packet duplicate? */

#ifdef FEATURE_DSM_DYNAMIC_POOL_SELECTION
  uint32 allocated_dsm_size;
    /* Size of the allocated dsm item */
#endif /* FEATURE_DSM_DYNAMIC_POOL_SELECTION */

  uint32 n_128b_chunks_left;
    /* Number of 128 byte chunks left to read from decoder buffer */

  uint32 n_128b_chunks_read;
    /* Number of 128-byte chunks that will be read during this iteration */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDRSRCH_POWER_PROFILE
  HDR_PWR_PROFILE_STRB_TOGGLE(HDRSRCH_PWR_EVENT_CC_ISR_START);
#endif /* FEATURE_HDRSRCH_POWER_PROFILE */

  num_dec_pkts = hdrdec_setup_decob_read ( msg_ptr );

  /*-----------------------------------------------------------------------
    Process each decoder packet in the decoder output buffer.
  -----------------------------------------------------------------------*/
  while ( num_dec_pkts-- > 0 )
  {
    /*---------------------------------------------------------------------
      Read the decoder packet header into the local buffer and if the 
      decoder block is ready to be ready out,
    ---------------------------------------------------------------------*/
    hdrdec_read_next_header ( &hdrdec.pkt_info );

    /* Read the num_mac_pkts into the working variable */
    n_128b_chunks_left = hdrdec.pkt_info.num_mac_pkts;

    HDRDEC_DEBUG_DUMP_DECODER_HEADER_INFO ( hdrdec.pkt_info, num_dec_pkts );

    /* Read packet information in convenient variables */
    channel = hdrdec.pkt_info.channel;
    crc_status = hdrdec.pkt_info.crc_status;
    pkt_size_index = HDRDEC_PKT_SIZE_TO_INDEX ( hdrdec.pkt_info.byte_length );
    hstr9 = ( hdrdec.pkt_info.hstr % 512 );
    packet_seq = HDRDEC_SEQ_INDEX ( hdrdec.pkt_info.sequence );

    /* Drop packets if we don't have enough dsm items */
    if ( HDRDSM_GET_FREE_COUNT() < n_128b_chunks_left )
    {
#ifdef FEATURE_DSM_DYNAMIC_POOL_SELECTION
      if ( ( ( DSM_POOL_FREE_CNT( DSM_DS_LARGE_ITEM_POOL ) == 0  ) && 
             ( hdrdec.pkt_info.rev_a ) ) ||
           ( ( !hdrdec.pkt_info.rev_a ) && (hdrdec.pkt_info.channel != HDRDEC_CC) ) )
#endif 
      {      
        HDRDEC_DROP_PKT_ON_DSM_EXHAUST ( crc_status );

        hdrdec.dsm_pool_exhausted_count++;
        
        /* ERR_FATAL if we've reached the threshold,
           since the dsm pool(s) are not recovering. */
        if ( hdrdec.dsm_pool_exhausted_count >= 
             HDRDEC_ERR_FATAL_ON_DSM_POOL_EXHAUSTED_COUNT_THRESHOLD )
        {
          ERR_FATAL( "DSM pool exhausted too many consective times!", 0, 0, 0);
        }
      }
#ifdef FEATURE_DSM_DYNAMIC_POOL_SELECTION
      else
      {
        /* DSM pool may be recovering; reset count. */
        hdrdec.dsm_pool_exhausted_count = 0;
      }
#endif
    }
    else
    {
      /* DSM pool may be recovering; reset count. */
      hdrdec.dsm_pool_exhausted_count = 0;
    }

    hdrdec.pkt_slot_time[packet_seq] = (int16) ( hstr9 / 2 );
      /* Store slot time for packet based on sequence. */

    /* If the packet is not cancelled by DSP... */
    if ( !hdrdec.pkt_info.pkt_is_cancelled )
    {


#ifdef FEATURE_HDR_BCMCS
    hdrbcmac_update_decode_info ( ( hstr9 / 2 ),
                                  (boolean) ( channel == HDRDEC_BCC ), 
                                  (boolean) ( crc_status == HDRDEC_CRC_PASS ) );
      /* Inform HDR BCMCS suite that a packet has been rx'd */
#endif

      /* Re-initialization - Clear memory of previous decoder frame */

      prev_dsm_ptr = NULL;
      root_dsm_ptr = NULL;
      dsm_item_ptr = NULL;
      app_field = 0;
      app_ptr_field = 0;
      kind_field = 0;

      /*---------------------------------------------------------------------
        If the decoder packet CRC is valid, read each MAC layer packet into a
        dsm item, copy the decoder packet header data to the dsm item header,
        and place the dsm item onto the receive array.
      ---------------------------------------------------------------------*/
      if ( crc_status == HDRDEC_CRC_PASS )
      {
        /*-------------------------------------------------------------------
                                GOOD CRC PACKETS
        -------------------------------------------------------------------*/

        /* Fill in the user fields with the packet information which will be 
           set to user fields of the DSM item later for upper layer's use */

        HDRDSM_SET_USER_FIELD_TIMESTAMP ( app_field, hdrdec.pkt_info.hstr );
        HDRDSM_SET_USER_FIELD_LINK_ID ( app_field, hdrdec.pkt_info.link_id );
        HDRDSM_SET_USER_FIELD_DRC ( app_field, hdrdec.pkt_info.drc_decoded );
        HDRDSM_SET_USER_FIELD_SLOTS_TO_DECODE (
          app_field, hdrdec.pkt_info.slots_to_decode );

#ifdef FEATURE_HDR_REVA_L1
        if ( hdrdec.pkt_info.rev_a )
        {
          HDRDSM_SET_USER_FIELD_REV_A ( app_field );
        }
#endif /* FEATURE_HDR_REVA_L1 */
        HDRDSM_SET_USER_FIELD_PACKET_TYPE ( 
          kind_field, hdrdec.pkt_info.pkt_type );
        HDRDSM_SET_USER_FIELD_PACKET_SIZE (
          app_ptr_field, 
          HDRDEC_PKT_SIZE_TO_INDEX ( hdrdec.pkt_info.byte_length ) );

#ifdef FEATURE_HDR_REVB
        HDRDSM_SET_USER_FIELD_SCHED_GROUP_ID (
          app_ptr_field, hdrdec.pkt_info.sched_group_id );
        HDRDSM_SET_USER_FIELD_NUM_SIMUL_PACKETS (
          app_ptr_field, hdrdec.pkt_info.num_concurrent_pkts );
        HDRDSM_SET_USER_FIELD_CARRIER_ID (
          app_ptr_field, hdrdec.pkt_info.demod_id );
#endif /* FEATURE_HDR_REVB */

        /*-----------------------------------------------------------------
              READ DECODER PACKET (ONE MAC PACKET AT A TIME)
            (There are 1 to 5 MAC packets per decoder frame)
        -----------------------------------------------------------------*/
        do /* while (num_n_128b_chunks_left_w > 0) */
        {

#ifdef FEATURE_DSM_DYNAMIC_POOL_SELECTION
          if ((hdrdec.pkt_info.rev_a) || (hdrdec.pkt_info.channel == HDRDEC_CC))
          {
            /* The size of the DSM item assigned by this API might be smaller
               or larger than the requested size. */
            dsm_item_ptr = dsm_new_buffer_flexible( DSM_DS_SMALL_ITEM_POOL, 
                                                    (n_128b_chunks_left * 128),
                                                    TRUE,
                                                    DSM_ITEM_SUBSYSTEM_HDR );
          }
          else
#endif /* FEATURE_DSM_DYNAMIC_POOL_SELECTION */
          {
            /* Get a new dsm item from the pool. */
            dsm_item_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL );
          }

          /* Make sure we got an item */
          if ( dsm_item_ptr == NULL )
          {
            /* We shouldn't get here. We already made sure we had enough number
               of DSM items left for the entire decoder frame, unless some other
               higher priority task used it before HDRDEC. If this happens,
               just drop the entire decoder frame and mark the CRC status
               as failure */

            HDRDEC_DROP_PKT_ON_DSM_EXHAUST ( crc_status );

            dsm_free_packet ( &root_dsm_ptr );
              /* Throw away what was read so far from the decoder frame */

            break;
              /* Break out of the do-while loop */
          }

 #ifdef FEATURE_DSM_DYNAMIC_POOL_SELECTION
          /*-----------------------------------------------------------------
            Adjust the number of bytes read during this iteration so that it 
            doesn't exceed the size of the allocated DSM item. 
           ---------------------------------------------------------------*/
          if ((hdrdec.pkt_info.rev_a) || (hdrdec.pkt_info.channel == HDRDEC_CC))
          {
            allocated_dsm_size = DSM_ITEM_GET_SIZE(dsm_item_ptr);

            /* Number of 128-byte checks that we can read during this iteration is */
            n_128b_chunks_read = (allocated_dsm_size / 128);

            /* Number of 128-byte chunks that we can read should always be less than 
               the number of 128-byte chunks left to read */
            if (n_128b_chunks_read > n_128b_chunks_left)
            {
              n_128b_chunks_read = n_128b_chunks_left;
            }
          }
          else
#endif /* FEATURE_DSM_DYNAMIC_POOL_SELECTION */
          {
            /* For release-0 always read one MAC packet at a time */
            n_128b_chunks_read = 1;
          }

          /* Is this the first MAC packet in the current decoder packet? */
          if ( prev_dsm_ptr )
          {
            prev_dsm_ptr->pkt_ptr = dsm_item_ptr;
              /* No - therefore we link this packet to the previous one. */
          }
          else
          {
            root_dsm_ptr = dsm_item_ptr;
              /* Yes. Mark the DSM item as start of chain (root) */
          }

          /*-----------------------------------------------------------------
            Read the MAC layer packet from the decoder output buffer into the
            dsm item and set the dsm item header fields.
          -----------------------------------------------------------------*/
          dsm_item_ptr->used = 
            hdrdec_read_next_packet( dsm_item_ptr->data_ptr, n_128b_chunks_read );

          HDRDEC_DEBUG_DUMP_NUM_BYTES_USED( dsm_item_ptr );

          /* Set the app fields of DSM item with appropriate user fields 
             packed already with packet information */

          HDRDSM_SET_USER_FIELD_APP_FIELD( dsm_item_ptr, app_field );
          HDRDSM_SET_USER_FIELD_KIND( dsm_item_ptr, kind_field );
          HDRDSM_SET_USER_FIELD_APP_PTR( dsm_item_ptr, app_ptr_field );

          if ( ( channel != HDRDEC_CC ) && ( channel != HDRDEC_BCC ) )
          {
            /** @todo move TRAFFIC_BIT to user field?  */
            HDRDSM_SET_TRAFFIC_BIT( dsm_item_ptr );
              /* Update traffic bit for FTC packets */
          }

#ifdef FEATURE_HDR_REVA_L1
          if ( ( hdrdec.pkt_info.rev_a ) && ( n_128b_chunks_left == 1 ) )
          {
            HDRDSM_SET_LAST_ITEM ( dsm_item_ptr );
          }
#endif /* FEATURE_HDR_REVA_L1 */

          prev_dsm_ptr = dsm_item_ptr;
            /* Remember current DSM item as previous, so if there is a next
               DSM item, it could be chained */

          n_128b_chunks_left -= n_128b_chunks_read;

        } while ( n_128b_chunks_left > 0 );

        /* We could have broken out of while loop due to insuffient DSM items;
           In this case, the CRC of the decoder frame will be marked as a 
           failure. Do not assume that the CRC status is good here. */

      } /* If packet passed CRC */


      /* Initialize convenient local variables */

      update_stats_fptr = 
        hdrdec_pkt_fwd_update_stats_lut [crc_status][channel].update_stats_fptr;
      pkt_status = 
        hdrdec_pkt_fwd_update_stats_lut [crc_status][channel].status;
        /* Initialize the packet status given the CRC status and channel */

      if ( crc_status == HDRDEC_CRC_PASS )
      {
        /** @todo Move HDRDSM_SET_MAC_TRAILER to user field? */
        HDRDSM_SET_MAC_TRAILER ( 
          root_dsm_ptr, 
          ( ( dsm_item_ptr->data_ptr[dsm_item_ptr->used] ) >> 6 ) );
          /* Set MAC trailer. Note that this is only useful for Rev A */

        if ( hdrdec_pkt_is_duplicate ( root_dsm_ptr, &hdrdec.pkt_info ) )
        {
          /*-----------------------------------------------------------------
                               DECODED PACKET DUPLICATE
          ----------------------------------------------------------------*/
          dsm_free_packet ( &root_dsm_ptr );
            /* Free the packet. Since size is always <= 1024, there is only 
               one MAC packet to free */

          /* Mark the packet status as bad */
          root_dsm_ptr = NULL;
          pkt_status = HDRDEC_PKT_CANCELED;
          duplicate_pkt = TRUE;
        }

      } /* If CRC passed */

      /* We don't want to include duplicate packet in statistics. Check here */
      if (!duplicate_pkt)
      {
        if ( channel == HDRDEC_CC )
        {
          lost_sync_cc = hdrdec_handle_cc_pkt ( crc_status, 
                                                num_dec_pkts,
                                                &root_dsm_ptr, 
                                                &pkt_status,
                                                hstr9 );
            /* pkt_status and root_dsm_ptr might be changed within this function 
               if the CC packet was chosen to be delivered on arrival */

        } /* Control channel? */
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
        else if ( channel == HDRDEC_FTC_UNICAST || channel == HDRDEC_FTC_MUP )
        {
          hdrdec.latest_ftc_pkt_hstr = HDRHSTR_GET_COUNT();
            /* save the hstr of the latest FTC pkt_hstr */
        }
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

        ( *update_stats_fptr ) ( &hdrdec.pkt_info, pkt_size_index );
          /* Update statistics */
        
      } /* if !duplicate */

      /* Re-initialize the flag */
      duplicate_pkt = FALSE;

      /* Store packet into the resequencing buffer along with its channel 
         information. 'root_dsm_ptr' will be NULL if the CRC failed */
      hdrdec.pkt_buffer [packet_seq] = root_dsm_ptr;
      hdrdec.pkt_status [packet_seq] = pkt_status;

      hdrrxstats_update_per_stats ( hdrdec.pkt_info.demod_id );
        /* Update PER statistics */

    } /* If packet is not cancelled */
    else
    {
      /* Pkt sequence is cancelled */
      /* @todo add some kind of stats for cancelled pkts */

      HDR_MSG_PROT_1 (MSG_LEGACY_MED, "DSP Cancelled Seq %x", packet_seq);
      hdrdec.pkt_status[packet_seq] = HDRDEC_PKT_CANCELED;
      hdrdec.pkt_buffer[packet_seq] = NULL;
    }

    rex_enter_crit_sect(&hdrdec.hdrdec_crit_sect);
    hdrdec.buff_entry_cnt++;
      /* Increment the number of outstanding packets in the resequencing 
         buffer */

    /* If the current packet fills the first hole, increment expected sequence,
       until we hit the next hole */
    if ( packet_seq == hdrdec.expected_seq )
    {
      do
      {
        HDRDEC_INCREMENT_SEQ ( hdrdec.expected_seq );

        /* Check for buffer overflow */
        if ( hdrdec.expected_seq == hdrdec.process_seq )
        {
          HDRDEC_DROP_OLD_PKT_ON_OVERFLOW();
            /* Overflow! Drop oldest (yet to be delivered) out of order packet
               in the resequencing buffer to make room for new packet. */
        }

      } while ( hdrdec.pkt_status[hdrdec.expected_seq] != HDRDEC_PKT_WAITING );

    } /* If the current packet is in order */

    rex_leave_crit_sect(&hdrdec.hdrdec_crit_sect);

    /* Logging */


    (void) hdrdec_update_pkt_header_buf ( hdrdec.pkt_info );

    if ( hdrdec_update_mc_pkt_header_buf ( &hdrdec.pkt_info ) )
    {
      (void) rex_set_sigs ( HDRRX_TASK_PTR, HDRRX_LOG_BUFFER_READY_SIG );
    }

  } /* while ( num_dec_pkts-- > 0 ) */

  /* End of current read session */
  hdrdec_end_read();

  /*---------------------------------------------------------------------
    If we failed CRC for the Sync Capsule, give signal to hdrrx task!
   --------------------------------------------------------------------*/
  if ( lost_sync_cc )
  {
    HDR_MSG_PROT (MSG_LEGACY_HIGH, "No SyncCC (bad CRC)");

    (void) rex_set_sigs ( HDRRX_TASK_PTR, HDRRX_SYNC_CC_LOST_SIG );
      /* Give sig so we can handle this at task level */
  }

  /* @todo Flow control needs to be added */
  /* Check the circular buffer to see if flow control is needed. */
  //if ( ( !hdrdec.buff_flow_control ) &&
  //     ( hdrdec.buff_entry_cnt > HDRDEC_CBUFF_HI_WATERMARK ) )
  //{
  //  HDR_MSG_PROT_1 ( MSG_LEGACY_MED,
  //                   "Reseq buffer %d Hi WM Event", hdrdec.buff_entry_cnt );

    /* Call to enable flow control, and remember that we did so. */
  //  hdrdec.buff_flow_control = TRUE;
  //  hdrfmac_enable_flow_control ( HDRFMAC_DRC_REQ_CBUF );
  //}

  /*-------------------------------------------------------------------------
   When finished processing the packets in the decoder output buffer
   set the task data queue signal.
  -------------------------------------------------------------------------*/
  (void) rex_set_sigs ( HDRRX_TASK_PTR, HDRRX_PKT_READY_SIG );

#ifdef FEATURE_HDRSRCH_POWER_PROFILE
  HDR_PWR_PROFILE_STRB_TOGGLE(HDRSRCH_PWR_EVENT_CC_ISR_END);
#endif /* FEATURE_HDRSRCH_POWER_PROFILE */

} /* hdrdec_process_decob_pending_ind */


/*============================================================================
  HDRDEC_PROCESS_DECODER_SHUTDOWN_IND
*/
/**
  Process HDRFW_DECOB_STATUS_IND_MSG_T indication message.

  Reads the packet out of the decoder buffer and stores into the resequencing 
  buffer. 

  @context HDRDEC */
/*===========================================================================*/

void hdrdec_process_decoder_shutdown_ind
(
  hdrfw_msg_ind_u *msg_ptr
    /**< Pointer to the struct holding the firmware message */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_DRIVERS( MSG_LEGACY_MED, "dec deactivate done" );
#endif /* FEATURE_HDR_RUMI */

  hdrdec.active = FALSE;
    /* Remember that we deactivated */
  
  if ( hdrdec.shutdown_notification_cb != NULL  )
  {
      hdrdec.shutdown_notification_cb();
  }

} /* hdrdec_process_decoder_shutdown_ind */


/*============================================================================
  HDRDEC_PROCESS_CMD
*/
/**
  Processes commands given to HDR Decoder task.  */
/*===========================================================================*/
void hdrdec_process_cmd 
( 
  hdrdec_cmd_s_type *cmd_ptr 
    /**< Pointer to the event that needs to be processed */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( cmd_ptr->cmd )
  {
    case HDRDEC_ACTIVATE_CMD:
    {
      hdrdec_activate();
      break;
    }

    case HDRDEC_ENTER_MODE_CMD:
    {
      hdrdec_enter_mode( cmd_ptr->data.mode );
      break;
    }

    case HDRDEC_RESET_SEQ_CMD:
    {
      hdrdec_reset_pkt_sequences();
      break;
    }

    case HDRDEC_DEACTIVATE_CMD:
    {
      hdrdec_deactivate( cmd_ptr->data.srch_cb );
      break;
    }

    case HDRDEC_LOG_MC_PKT_HEADERS_CMD:
    {
      hdrdec_log_mc_pkt_headers();
      break;
    }

    case HDRDEC_SHIP_AGED_LOG_BUFFER_CMD:
    {
      hdrdec_ship_aged_log_buffer();
      break;
    }

    case HDRDEC_LOG_PKT_HEADERS_CMD:
    {
      hdrdec_log_pkt_headers( cmd_ptr->data.immediate );
      break;
    }

    default:
    {
      HDR_MSG_DRIVERS_1( MSG_LEGACY_ERROR, "Unknown cmd %d", cmd_ptr->cmd );
      break;
    }

  } /* switch */

} /* hdrdec_process_cmd */


/*============================================================================
  HDRDEC_PROCESS_EVENT
*/
/**
  Processes events dequeued from the event buffer.   */
/*===========================================================================*/
void hdrdec_process_event 
( 
  hdrdec_event_s_type *event_ptr 
    /**< Pointer to the event that needs to be processed */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT (event_ptr != NULL);

  switch ( (int) event_ptr->hdr.type_of_item )
  {
    /* The item is a command */
    case (int) HDRBUF_COMMAND: 
    {
      hdrdec_process_cmd ( &event_ptr->event.cmd );
      break;
    }

    case (int) HDR_FW_DECOB_STATUS_IND:
    {
      if ( hdrdec.active )
      {
        hdrdec_process_decob_pending_ind ( &event_ptr->event.msg );
      }
      else
      {
        HDR_MSG_DRIVERS ( MSG_LEGACY_ERROR, 
                          "Packet pending message Rx'd from FW while inactive!");
      }
      break;
    }

    case (int) HDR_FW_CANCEL_ACTIVE_PACKET_RSP:
    {
      hdrdec_process_decoder_shutdown_ind ( &event_ptr->event.msg );
      break;
    }

    case (int) HDR_FW_LOG_BUFFER_IND:
    {
      hdrdec_process_log_buffer_ind ( &event_ptr->event.msg );
      break;
    }

    default:
    {
      HDR_MSG_DRIVERS_1 ( MSG_LEGACY_ERROR, 
                          "Unknown event or firmware message %x", 
                          event_ptr->hdr.type_of_item );
      break;
    }

  } /* switch */

} /* hdrdec_process_event */

/*============================================================================
  HDRDEC_ENTER_HDR
*/
/**
  Processes signals given in HDR online mode.

  This function is the main task processing loop that processes the signals, 
  commands and indication messages to this task during HDR online mode. 

  @todo Doesn't set exit_hdr_mode!!! Is this loop supposed to process HDR 
  signals even after TASK stop? */
/*===========================================================================*/
void hdrdec_enter_hdr ( void )
{
  boolean exit_hdr_mode = FALSE;
    /* Exit from HDR mode */

  rex_sigs_type rex_signals_mask;
  rex_sigs_type rex_wait_mask;

  hdrdec_event_s_type *event_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT ( MSG_LEGACY_HIGH, "Decoder task entering HDR mode" );

  memset ( hdrdec.pkt_status, 0, sizeof(hdrdec.pkt_status) );
    /* Clear out status buffer */

  rex_wait_mask = ( HDRDEC_EVENT_SIG      |
                    HDRDEC_EXIT_HDR_SIG   |
                    TASK_OFFLINE_SIG      |
                    TASK_STOP_SIG );

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  while ( !exit_hdr_mode )
  {
    rex_signals_mask = hdrdec_wait( rex_wait_mask );

    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & TASK_STOP_SIG ) != 0 )
    {
      /*! @todo Doesn't set exit_hdr_mode!!! Is this loop supposed to process
          HDR signals even after TASK stop? */
      HDR_MSG_PROT ( MSG_LEGACY_MED,  "TASK_STOP_SIG received" );
      (void) rex_clr_sigs ( HDRDEC_TASK_PTR, TASK_STOP_SIG );

      /* Deregister the registered messages and delete the messenger client */
      hdrdec_msg_stop();
      
    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & TASK_OFFLINE_SIG ) != 0 )
    {
      /*! @todo Doesn't set exit_hdr_mode!!! Is this loop supposed to process
          HDR signals even after TASK offline? */
      HDR_MSG_PROT (MSG_LEGACY_MED,  "TASK_OFFLINE_SIG received");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, TASK_OFFLINE_SIG );
    }


    /*-----------------------------------------------------------------------
     Event queue signal. Clear signal and handle all events on event queue.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRDEC_EVENT_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_EVENT_SIG );

      while ( q_cnt( &hdrdec.event_q ) != 0 )
      {
        event_ptr = (hdrdec_event_s_type*) q_get ( &hdrdec.event_q );
          /* Dequeue the next event from the event queue */

        hdrdec_process_event( event_ptr );
          /* Process the event that was just dequeued */

        hdrbuf_done( &event_ptr->hdr );
          /* Return the event buffer back to free queue */
      }
    }

    /*-----------------------------------------------------------------------
     Exit HDR signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRDEC_EXIT_HDR_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "HDRDEC_EXIT_HDR_SIG received");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_EXIT_HDR_SIG );
      exit_hdr_mode = TRUE;
    }

  } /* !exit_hdr_mode */

} /* hdrdec_enter_hdr */


/*============================================================================

                        EXTERNAL FUNCTION DECLARATIONS

=============================================================================*/


/*============================================================================
  HDRDEC_LOG_MC_PKT_HEADERS
*/
/**
  Log the pkt PHY header through the logging service.

  @context HDRRX task. 

  @sideeffects: This function locks the interrupts briefly to avoid race 
  condition that could arise from reading some variables updated in the decoder 
  ISR context as well. */
/*============================================================================*/
void hdrdec_log_mc_pkt_headers ( void )
{
  LOG_HDR_MC_FL_PHY_PKT_HEADER_C_type* log_ptr;
    /* Pointer to multi carrier fwd link phy pkt header log packet */

  uint8  demod_idx;
    /* Carrier index */

  hdrsrch_subaset_idx_type saset_idx;
    /* Sub ASET index */

  uint8  cum_rec_cnt = 0;  
    /* Total number of records in all carriers */

  uint8  carrier_cnt = 0; 
    /* Total number of carriers carrying records */

  uint8 buff_idx;
    /* Index of the buffer */

  hdrlog_mc_fl_pkt_header_per_carrier_info_type *dest_cptr;
    /* Pointer to per carrier fl link pkt header records */

  uint16 num_bytes = 0;

  uint16 num_bytes_copied_so_far = 0;

  hdrsrch_fl_carrier_info_type saset_info;
    /* FL carrier information */

  boolean ready_for_logging = FALSE;
    /* Is the buffer ready for logging? */

  hdrdec_mc_pkt_header_buff_type *src_cptr = NULL;
    /* Source convenience pointer */

  hdrdec_cmd_s_type cmd_buf;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_LOG_MC_PKT_HEADERS_CMD;
    hdrdec_queue_cmd( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {

  /* This function could be called from both HDRRX and HDRSRCH task context. */

  /* Choose the in-active buffer */
  buff_idx = ((hdrdec_mc_pkt_hdr.active_buff_idx == 0)? 1: 0);

  ready_for_logging = hdrdec_mc_pkt_hdr.buff[buff_idx].ready_for_logging;
  hdrdec_mc_pkt_hdr.buff[buff_idx].ready_for_logging = FALSE;

  /* Even if there was a race condition entering this function, only one task
     would be allowed to enter the "if" block */
  if ( ready_for_logging )
  {
    src_cptr = &hdrdec_mc_pkt_hdr.buff[buff_idx];

    if ( log_status ( LOG_HDR_MC_FL_PHY_PKT_HEADER_C ) )
    {

      /* Use the for loop to find total no of records and carrier count */
      for ( demod_idx = 0; 
            demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; demod_idx++ )
      {
        /* Just for the Sanity Check; This should not happen */
        if ( src_cptr->carriers[demod_idx].rec_cnt >
             HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE )
        {
          src_cptr->carriers[demod_idx].rec_cnt =
            HDRDEC_MC_PKT_HEADER_LOG_BUF_SIZE;

          HDR_MSG_PROT_2(
            MSG_LEGACY_ERROR,
            "PHY header log cnt for carrier id %d is over limit rec cnt: %d",
            demod_idx, src_cptr->carriers[demod_idx].rec_cnt);
        }

        /* Does the current carrier have non-zero record count? */
        if ( src_cptr->carriers[demod_idx].rec_cnt > 0 )
        { 
          cum_rec_cnt += src_cptr->carriers[demod_idx].rec_cnt ;
            /* Accumulate the number of records... */

          carrier_cnt ++;
        }

      } /* for each carrier */

      /* Logging should be done only if there are any records. Is there 
         at least one carrier with non-zero record count? */
      if (carrier_cnt > 0)
      {

        log_ptr = (LOG_HDR_MC_FL_PHY_PKT_HEADER_C_type *)
            log_alloc ( 
              LOG_HDR_MC_FL_PHY_PKT_HEADER_C, 
              sizeof ( LOG_HDR_MC_FL_PHY_PKT_HEADER_C_type ) + 
              ( ( carrier_cnt - 1 ) * 
                sizeof(hdrlog_mc_fl_pkt_header_per_carrier_info_type) ) + 
              ( ( cum_rec_cnt - carrier_cnt ) * 
                sizeof(hdrlog_mc_fl_pkt_header_info_type) ) );
          /* Allocate memory for log packet */

        if ( log_ptr != NULL )
        {

          log_ptr->version = LOG_HDR_MC_FL_PHY_PKT_HEADER_VERSION;

          log_ptr->carrier_cnt = carrier_cnt;
            /* Store the carrier count in the log packet */

          /* For each carrier */
          for ( saset_idx=0; saset_idx < HDRSRCH_MAX_NUM_SUBASETS; saset_idx++ )
          {

            /* Get the demod information from the demod ID. The API returns 
               TRUE only if the demod is in use */
            if ( hdrsrch_get_fl_carrier_info( saset_idx, &saset_info ) )
            {
              demod_idx = saset_info.demod_idx;

              /* Initialize the convenience pointer to per-carrier information */
              dest_cptr = (hdrlog_mc_fl_pkt_header_per_carrier_info_type*)
                ((uint8*) (&log_ptr->per_carrier_pkt_header_info[0]) + 
                num_bytes_copied_so_far);

              if (src_cptr->carriers[demod_idx].rec_cnt > 0)
              {
                /* Store the carrier id in the record packet */
                dest_cptr->carrier_id.channel_id = demod_idx;
                dest_cptr->carrier_id.channel_no = saset_info.channel.chan_num;
                dest_cptr->carrier_id.band_class = saset_info.channel.band;

                /* Store the record cnt in the log packet */
                dest_cptr->rec_cnt = src_cptr->carriers[demod_idx].rec_cnt;

                num_bytes = (src_cptr->carriers[demod_idx].rec_cnt * 
                  sizeof (hdrlog_mc_fl_pkt_header_info_type));

                /* MemCopy the stored msg */
#ifdef FEATURE_MEMCPY_REMOVAL
                memscpy(
                  (void*) &(dest_cptr->fl_pkt_header_info[0]), 
                   num_bytes, 
                  (void*) &(src_cptr->carriers[demod_idx].rec[0]),
                  num_bytes);
#else
                memcpy(
                  (void*) &(dest_cptr->fl_pkt_header_info[0]), 
                  (void*) &(src_cptr->carriers[demod_idx].rec[0]),
                  num_bytes);
#endif /* FEATURE_MEMCPY_REMOVAL */

                /* We have copied "rec_cnt" records, carrier ID and the rec_cnt 
                 (uint8) */
                num_bytes_copied_so_far += 
                  (num_bytes + sizeof(hdrlog_mc_carrier_id_type) + sizeof(uint8));
              }

              HDR_MSG_PROT_1( MSG_LEGACY_LOW, 
                              "Logging PHY PKT Header %d rec_cnts", 
                              src_cptr->carriers[demod_idx].rec_cnt );

            } /* if the demod is in use */

          } /* for each carrier */

          /* Commit the log packet to the logging service */
          log_commit((log_type*)log_ptr);

        } /* if (log_ptr != NULL) */

      } /* if (carrier_cnt > 0) */

    } /* If log enabled */

    /* Clean up the log buffer */
    memset( src_cptr,
            0,
            sizeof(hdrdec_mc_pkt_header_buff_type) );

  } /* buffer ready for logging? */

  } /* HDRDEC task context */

} /* hdrdec_log_mc_pkt_headers*/


/*============================================================================
  HDRDEC_SHIP_AGED_LOG_BUFFER
*/
/**
  The function is called from the HDRSRCH task to ship the pkt header info log 
  if the current active buffer is too old.

  @context HDRRX task. */
/*============================================================================*/
void hdrdec_ship_aged_log_buffer ( void )
{
  uint16 age;
    /* Age in halfslots of the current active buffer */

  uint16 ts_now;
    /* Current HSTR */

  boolean submit = FALSE;
    /* Submit log? */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrdec_cmd_s_type cmd_buf;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_SHIP_AGED_LOG_BUFFER_CMD;
    hdrdec_queue_cmd( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
  ts_now = HDRHSTR_GET_COUNT();
    /* Current HSTR */

  /* If the start time is valid */
  if ( hdrdec_mc_pkt_hdr.ts_valid )
  {
    /*------------------------------------------------------------------------- 
      If we are here, it means we have at least one record in the current
      active buffer
    ----------------------------------------------------------------------*/

    age = ( ts_now - hdrdec_mc_pkt_hdr.ts_start );
      /* Find the number of half slots since the first record entry into 
         the active buffer */

    /*------------------------------------------------------------------------- 
       During idle scenarios, the packet record in the buffer may not have 
       aged enough to log it right away. Still, after receiving just one packet
       AT might sleep for a long time and when it wakes up it would have aged
       beyond acceptable limits. So, dis-regard age restrictions during idle
       and log the buffer right away. 
       ----------------------------------------------------------------------*/

    /* If we are in idle or if the buffer has been active for too long */
    if ( ( hdrfmac_is_active(HDRHAI_DECODER) == FALSE )  || 
         ( age >= HDRDEC_LOG_BUFF_MAX_AGE_ALLOWED ) )
    {
      hdrdec_mc_pkt_hdr.buff[hdrdec_mc_pkt_hdr.active_buff_idx].
        ready_for_logging = TRUE;

      hdrdec_mc_pkt_hdr.active_buff_idx = 
        ( ( hdrdec_mc_pkt_hdr.active_buff_idx == 0 )? 1: 0 );
        /* Toggle active buffer index */

      hdrdec_mc_pkt_hdr.ts_valid = FALSE;
        /* Mark Start timestamp as invalid */

      submit = TRUE;
    }

  } /* start time valid? */

  if ( submit )
  {
    hdrdec_log_mc_pkt_headers();
      /* Ship the buffer just marked ready for logging */
  }
  } /* HDRDEC task context */

} /* hdrdec_ship_aged_log_buffer */

/*============================================================================
  HDRDEC_ACTIVATE
*/
/**
  Activates the decoder.

  If called from a task other than HDRDEC, 
    - posts command \link HDRDEC_ACTIVATE_CMD \endlink to HDRDEC task to 
      activate the decoder 
  otherwise 
    - Enable turbo decoder clocks. 
    - Reset the turbo decoder hardware, setup turbo decoder mode for HDR mode. 

  @assumptions hdrdec_init() called already at power-up. 

  @context HDRDEC

  @see hdrdec_init() */
/*===========================================================================*/
void hdrdec_activate ( void )
{

  hdrdec_cmd_s_type cmd_buf;
  hdrfw_msg_u fw_cmd;
    /* Firmware command message */

  errno_enum_type estatus;
    /* Error status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_ACTIVATE_CMD;
    hdrdec_queue_cmd( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
    /*-------------------------------------------------------------------------
     In HDRDEC task context. Execute the command processing synchronously.
    -------------------------------------------------------------------------*/

    /* DEM_SYM_BUFF_MODE 
       Type: Read/Write (Control)
       Clock: Offline
       Reset State: 0x0
       Bits Name          Description
       5    MICRO_ACCESS  Bus access of Dem_Sym_Buff memories
       4    UMTS_VO       UMTS Voice Only Mode
       3    CONC_CDMA     Concurrent CDMA which supports 1x Paging
       2:0  MODE_SEL      Select the users of forward-link symbol buffer RAMs
                            0b000: CDMA
                            0b001: HDR
                            0b010: UMTS 
                            0b011: LTE 
                            0b100: WIMAX
                            0b101:RSVD_5
                            0b110: RSVD_6
                            0b111: RSVD_7 
       Select HDR Mode */
#ifndef FEATURE_HDR_DIME_MODEM
    HWIO_DEM_SYM_BUFF_MODE_OUTM(
        HWIO_FMSK (DEM_SYM_BUFF_MODE, MODE_SEL), 0x1);
#endif /* FEATURE_HDR_DIME_MODEM */

#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
    /* DEM_SYM_BUFF_CLK_CTL 
       Type: Read/Write (Control)
       Clock: Offline
       Reset State: 0x0
       Bits Name                Description
       1    CC_MEMORY_CLK_EN    0b1: clock on 0b0: clock off
       0    SHRD_MEMROY_CLK_EN  0b1: clock on 0b0: clock off 
       Enable clocks for demodulator symbol buffer */
    HWIO_DEM_SYM_BUFF_CLK_CTL_OUTM (
        HWIO_FMSK ( DEM_SYM_BUFF_CLK_CTL, CC_MEMORY_CLK_EN ) |
        HWIO_FMSK ( DEM_SYM_BUFF_CLK_CTL, SHRD_MEMORY_CLK_EN ), 
        0x3 );

    /* Reset Turbo Decoder and output buffer */ 

    /* DECODER_RESET
       Type: Write/Command
       Clock: offline_clk
       Bits Name                    Description
       31:5 RESERVED_BITS_30_5
       4    OFDMA_OFFLINE_RESET     0b1: RESET
       3*   CDMA_OFFLINE_RESET      0b1: RESET
       2    UMTS_OFFLINE_RESET      0b1: RESET
       1*   COMMON_OFFLINE_RESET    0b1: RESET
       0*   TD_CLK_RESET            0b1: RESET */
    HWIO_DECODER_RESET_OUT (
        HWIO_FMSK (DECODER_RESET, TD_CLK_RESET) |
        HWIO_FMSK (DECODER_RESET, COMMON_OFFLINE_RESET) |
        HWIO_FMSK (DECODER_RESET, CDMA_OFFLINE_RESET) );
#endif

    /* TBD to find out whether this is needed on Dime */
#ifndef FEATURE_HDR_DIME_MODEM
    /* DECODER_MODE_CTL
       Type: Write/Read
       Clock: offline_clk
       Reset State: 0x0
       Bits Name      Description
       2:0  STANDARD  0b000: CDMA
                      0b001: HDR
                      0b010: UMTS
                      0b011: LTE
                      0b100: WMX
                      Other values are reserved.  
       Select HDR mode : 1 */
    HWIO_DECODER_MODE_CTL_OUTM (
        HWIO_FMSK ( DECODER_MODE_CTL, STANDARD ), 0x1 );

    /* DECODER_CLK_CTL
       Type: Write/Read
       Clock: offline_clk
       Reset State: 0x0
       Bits Name                      Description 
       1    TBVD_HW_LCG_EN            0b0: SW_CLK_CTL 0b1: HW_CLK_CTL
       0    TD_HW_LCG_EN              0b0: SW_CLK_CTL 0b1: HW_CLK_CTL
      
       The Clocks in Decoder is controlled (enable or disabled) by the 
       corresponding HW signals when the corresponding bits of this register 
       are SET to '1'. Otherwise, they are controlled by DECODER_SW_CLK_EN 
       register.  All bits should be programmed to 1 in the functional mode. 
       Writing '0' to allow the SW control and this should be used only in 
       debugging or characterization.  */
    HWIO_DECODER_CLK_CTL_OUTM (
        HWIO_FMSK (DECODER_CLK_CTL, TBVD_HW_LCG_EN) |
        HWIO_FMSK (DECODER_CLK_CTL, TD_HW_LCG_EN) , 
        0x7 );

    HWIO_DECOB_HW_CLK_CTL_OUT(1);
      /* Enable Decoder Output Buffer clock gating */
#endif /* FEATURE_HDR_DIME_MODEM */

    /* DECOB_RESET 
       Type: Write
       Clock: MICRO_CLK
       Reset State:
       Bits Name   Description
       0    RESET  SW reset */
    /* TBD: DECOB_RESET is removed from NikeL SWI */
#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
    HWIO_DECOB_RESET_OUT ( 1 );   
#endif

#ifdef FEATURE_HDR_RUMI
    HDR_MSG_DRIVERS( MSG_LEGACY_MED, "dec activated" );
#endif /* FEATURE_HDR_RUMI */

    hdrdec.active = TRUE;
      /* Remember that we are now active */

    memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u ) );
      /* Initialize the message data with zeroes */

    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_DECODE_CTL_CMD );

    fw_cmd.decode_ctl.initDemback = 1;
      /* Indicate that demback is needed. */

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );

    /* Set decoder indication messaeg rate */
    hdrdec_set_decob_msg_rate ( hdrdec.decob_msg_rate );

    /* Flush Decoder buffer */
    hdrdec.decob_clear = 0x00FFFFFF;

    hdrdec_end_read(); 

  } /* HDRDEC task context */

} /* hdrdec_activate */

/*============================================================================
  HDRDEC_RESET_PKT_SEQUENCES
*/
/**
  Reset the decoder sequence numbers; 

  If called from a task other than HDRDEC, 
    - posts command \link HDRDEC_RESET_SEQ_CMD \endlink to HDRDEC task to reset
      the decoder sequence space. 
  otherwise 
    - Reset the mDSP sequence space. 
    - Reset the decoder re-sequencing buffer 

  @todo Send RESET_SEQUENCE message instead.

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_reset_pkt_sequences ( void )
{

  hdrdec_cmd_s_type cmd_buf;
    /* Command buffer variable */

  uint16 buff_idx;
    /* Buffer index - Loop variable */

  errno_enum_type estatus;
    /* Error status */

  hdrfw_msg_u fw_cmd;
    /* Message to configure decoder buffer indication message rate */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_RESET_SEQ_CMD;
    hdrdec_queue_cmd ( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
    /*-------------------------------------------------------------------------
     In HDRDEC task context. Execute the command processing synchronously.
    -------------------------------------------------------------------------*/

    HDR_MSG_PROT ( MSG_LEGACY_HIGH, "Reseting sequence numbers and buffer..." );

    /* Note that INTLOCK shouldn't be needed based on existing requirements of 
       function, namely that demod is disabled. */

    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_RESET_SEQ_NUM_CMD );

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );

    /* If there are any packets still in the sequence buffer, free them */
    if ( hdrdec.buff_entry_cnt > 0 )
    {
      HDR_MSG_PROT_1 ( 
        MSG_LEGACY_HIGH, "Clearing %d unused packets", hdrdec.buff_entry_cnt );
    }

    /* For each index in the resequencing buffer */
    for ( buff_idx = 0; buff_idx < HDRDEC_PKT_BUFFER_SIZE; buff_idx++ )
    {
      /* If the buffer index has a valid packet  */
      if ( hdrdec.pkt_status[buff_idx] != HDRDEC_PKT_WAITING )
      {
        HDR_MSG_PROT_1 ( MSG_LEGACY_HIGH, "Clearing unused packet: %d", buff_idx );

        if ( hdrdec.pkt_buffer[buff_idx] != NULL )
        {
          dsm_free_packet ( &hdrdec.pkt_buffer[buff_idx] );
            /* Free the dsm item */
        }
      }

      /* Reset the status */
      hdrdec.pkt_status[buff_idx] = HDRDEC_PKT_WAITING;
      hdrdec.pkt_buffer[buff_idx] = NULL;
      hdrdec.pkt_slot_time[buff_idx] = -1;

    } /* for each packet in the re-sequencing buffer */

    rex_enter_crit_sect(&hdrdec.hdrdec_crit_sect);

    /* Set the expected and process sequences to zero */
    hdrdec.expected_seq = 0;
    hdrdec.process_seq = 0;
    hdrdec.num_dropped_pkts = 0;
    hdrdec.buff_entry_cnt = 0;

    rex_leave_crit_sect(&hdrdec.hdrdec_crit_sect);

  } /* HDRDEC task */

} /* hdrdec_reset_pkt_sequences */

/*============================================================================
  HDRDEC_GET_NEXT_PKT
*/
/**
  Returns the next packet available to serve from the resequence buffer.

  @context HDRRX task.

  \internal 
  There is a potential race condition here between HDRDEC and HDRRX tasks in 
  updating the resequencing buffer and internal pointers. Need to use rex 
  critical section variables to prevent race condition 

  @todo Replace DISABLE_PREMPTION with critical section

  @return TRUE if a valid packet was returned and FALSE otherwise */
/*===========================================================================*/
boolean hdrdec_get_next_pkt 
(
  dsm_item_type** pkt_ptr,
    /**< Next packet in resequencing buffer to be delivered */
  hdrdec_pkt_status_e_type *pkt_status,
    /**< Decode status of the packet to be delivered */
  int16 *pkt_slot_time 
    /**< Slot time of the packet to be delivered */
)
{
  boolean pkt_available = FALSE;
    /* Do we have a packet available */

  hdrdec_oapd_s_type *buf_ptr;
    /* Pointer to an on-arrival delivery buffer */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Do we have a packet in on-arrival packet delivery queue? */
  if ( ( buf_ptr = 
         (hdrdec_oapd_s_type*) q_get( &hdrdec.oapd_q ) ) != NULL )
  {

    ASSERT ( buf_ptr->status == HDRDEC_CC_PKT_READY );
      /* We only expect control channel packets out of OAPD queue */

    *pkt_ptr  = buf_ptr->pkt_ptr;
    *pkt_status = buf_ptr->status;

    pkt_available = TRUE;

    /* Reset the buffer and queue it back in free-queue */
    buf_ptr->pkt_ptr = NULL;
    buf_ptr->status = HDRDEC_PKT_WAITING;
    q_put(&hdrdec.oapd_free_q, &buf_ptr->link);
  }
  else if ( hdrdec.process_seq != hdrdec.expected_seq )
  {
    /* If we couldn't get a packet out of OAPD, check the resequencing 
       buffer for packets */

    rex_enter_crit_sect(&hdrdec.hdrdec_crit_sect);

    /* Store the status, DSM item handle and slot time in local variables */
    *pkt_ptr = hdrdec.pkt_buffer[hdrdec.process_seq];
    *pkt_status = hdrdec.pkt_status[hdrdec.process_seq];

    pkt_available = TRUE;

#ifdef FEATURE_HDR_BCMCS
    *pkt_slot_time = hdrdec.pkt_slot_time[hdrdec.process_seq];
#endif /* FEATURE_HDR_BCMCS */

    /* Reset the status, DSM item handle and arrival slot time in CBuf for 
       the packet about to be delivered. */
    hdrdec.pkt_buffer[hdrdec.process_seq] = NULL;
    hdrdec.pkt_status[hdrdec.process_seq] = HDRDEC_PKT_WAITING;
    hdrdec.pkt_slot_time[hdrdec.process_seq] = -1;

    hdrdec.buff_entry_cnt--;

    HDRDEC_INCREMENT_SEQ ( hdrdec.process_seq );

    rex_leave_crit_sect(&hdrdec.hdrdec_crit_sect); 

    /*---------------------------------------------------------------------
      Disable flow control if the number of items left to process in the
      buffer is less than the Low watermark
      @TODO if flow control enabled 9k check if this part is sthread safe.
    ---------------------------------------------------------------------*/
    if ( ( hdrdec.buff_flow_control ) &&
         ( hdrdec.buff_entry_cnt < HDRDEC_CBUFF_LO_WATERMARK ) )
    {

      HDR_MSG_PROT_1 ( MSG_LEGACY_MED,
                       "Rx CBUFF %d Lo WM Event", hdrdec.buff_entry_cnt );

      /* Disable buffer flow control and remember that we did so. */
      hdrdec.buff_flow_control = FALSE;
      hdrfmac_disable_flow_control( HDRFMAC_DRC_REQ_CBUF );
    }
      
  }

  return pkt_available;

} /* hdrdec_get_next_pkt */

/*============================================================================
  HDRDEC_ENABLE_ON_ARRIVAL_DELIVERY
*/
/**
  Enables on arrival delivery of control channel packets 

  There are two modes of RX packet delivery to upper layers.  
    - In-order delivery: A circular buffer is used to resequence the packets 
      and packets are not delivered until all their predecessors have been 
      delivered.  
    - On-Arrival delivery: An high priority queue is maintained to deliver 
      packets as and when they arrive. This mode applies only to control channel 
      packets. 
  Mode 1 is default mode. This function is used to turn ON the second mode.  

  @context Calling task */
/*===========================================================================*/
void hdrdec_enable_on_arrival_delivery ( void )
{

  hdrdec.deliver_on_arrival = TRUE;

} /* hdrdec_enable_on_arrival_delivery */

/*============================================================================
  HDRDEC_DISABLE_ON_ARRIVAL_DELIVERY
*/
/**
  Disables on arrival delivery of control channel packets 

  There are two modes of RX packet delivery to upper layers.  
    - In-order delivery: A circular buffer is used to resequence the packets 
      and packets are not delivered until all their predecessors have been 
      delivered.  
    - On-Arrival delivery: An high priority queue is maintained to deliver 
      packets as and when they arrive. This mode applies only to control channel 
      packets. 

  Mode 1 is default mode. This function is used to turn OFF the second mode and 
  revert back to Mode 1. 

  @context Calling task.  */
/*===========================================================================*/
void hdrdec_disable_on_arrival_delivery ( void )
{

  hdrdec.deliver_on_arrival = FALSE;

} /* hdrdec_disable_on_arrival_delivery */

/*============================================================================
  HDRDEC_DEACTIVATE
*/
/**
  Deactivates the decoder. 

  If called from a task other than HDRDEC, 
    - posts command \link HDRDEC_DEACTIVATE_CMD \endlink to HDRDEC task to 
      deactivate the decoder 
  otherwise 
    - Cancels all pending decodes by sending the message HDRFW_DECODE_CTL_MSG_T 
    - Waits for the fw response message (hdrfw_cancel_active_packet_rsp_msg_t) 

  @sideeffects The BCMCS scheduler configuration is also cleared implicitly 
  when firmware cancels the active packets 

  @context HDRDEC task */
/*===========================================================================*/
void hdrdec_deactivate 
(
  hdrdec_shutdown_complete_cb_type cb 
    /**< Pointer to the callback function called after decoder shutdown is 
         complete */
)
{
  hdrfw_msg_u fw_cmd;
    /* Firmware command message to cancel all active packets and shutdown 
       decoder */

  errno_enum_type estatus;
    /* Error status */

  hdrdec_cmd_s_type cmd_buf;
    /* Event command buffer */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_DEACTIVATE_CMD;
    cmd_buf.data.srch_cb = cb;
    hdrdec_queue_cmd( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
    /*-------------------------------------------------------------------------
     In HDRDEC task context. Execute the command processing synchronously.
    -------------------------------------------------------------------------*/

    hdrdec.shutdown_notification_cb = cb;
      /* SRCH callback to call when shutdown completes */

    if ( hdrdec.active == TRUE )
    {

      memset ( &fw_cmd, 0, sizeof(hdrfw_msg_u ) );
        /* Initialize the message data with zeroes */

      msgr_init_hdr (
        &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_DECODE_CTL_CMD );

      fw_cmd.decode_ctl.cancelActivePackets = 1;
        /* Cancel any active packets. Includes clearning BCC detect also */
      fw_cmd.decode_ctl.releaseDemback = 1;
        /* Indicate to FW that it should release Demback. */

      estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
        /* Send the message */

      ASSERT ( estatus == E_SUCCESS );

    } /* Decoder is active */
    else
    {
      hdrdec.shutdown_notification_cb();
        /* Notify HDRSRCH task the decoder is already off */

      HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH, 
                        "Dec deactivate called when already off" );
    }

  } /* In HDRDEC task context */

} /* hdrdec_deactivate */

/*============================================================================
  HDRDEC_ENTER_MODE
*/
/**
  Enter Traffic mode or Monitor (Idle) mode 

  If called from a task other than HDRDEC, 
    - posts command \link HDRDEC_ENTER_MODE_CMD \endlink to HDRDEC 
  otherwise 
    - Sets interrupt rate for the mode 
    - Sets MinPreamble Threshold. 

  @context HDRDEC */
/*===========================================================================*/
void hdrdec_enter_mode 
( 
  hdrdec_decoder_mode_e_type mode 
    /**< Decoder mode. Could be either one of the following 
          - HDRDEC_MONITOR_MODE 
          - HDRDEC_TRAFFIC_MODE */
)
{
  hdrdec_cmd_s_type cmd_buf;
    /* Command buffer variable */

  uint16 ind_rate;
    /* Indication message rate */

  uint16 min_pream_thresh;
    /* Min Preamble Threshold */

  hdrfw_msg_u fw_cmd;
    /* Min-Preamble threshold Message */

  errno_enum_type estatus;
    /* Error status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_ENTER_MODE_CMD;
    cmd_buf.data.mode = mode;
    hdrdec_queue_cmd ( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {

    /* Choose indicate message rate based on the mode AT is about to enter */
    if ( mode == HDRDEC_MONITOR_MODE )
    {
      ind_rate = HDRDEC_IDLE_DECOB_MSG_RATE_V;
      min_pream_thresh = HDRDEC_MIN_PREAMBLE_THRESH_IDLE;
    }
    else
    {
      ind_rate = HDRDEC_TRAFFIC_DECOB_MSG_RATE_V;
      min_pream_thresh = HDRDEC_MIN_PREAMBLE_THRESH_TRAFFIC;
    }

    hdrdec.decob_msg_rate = ind_rate;
      /* Save dec rate so hdrdec_activate can refresh to FW */

    hdrdec_set_decob_msg_rate ( ind_rate );
      /* Set the message rate */


    msgr_init_hdr (
      &fw_cmd.hdr, MSGR_HDR_DEC, HDR_FW_MIN_PREAM_THRESH_CFG_CMD );

    fw_cmd.min_pream_thresh_cfg.minPreambleThresh = min_pream_thresh; 

    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
      /* Send the message */

    ASSERT ( estatus == E_SUCCESS );


  } /* HDRDEC task */

} /* hdrdec_enter_mode */


/*============================================================================
  HDRDEC_TASK
*/
/**
  Entry point to the HDR decoder task. 

  This function is the main task processing loop that processes the signals, 
  commands and indication messages to this task. */
/*===========================================================================*/
void hdrdec_task 
( 
  dword dummy 
    /**< Parameter to maintain REX backward compatibility */
)
{

  rex_sigs_type   rex_signals_mask;  /* Task signal mask.     */

  hdrdec_event_s_type *event_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Perform task initialization.
  -------------------------------------------------------------------------*/
  rcinit_handshake_startup();

  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */
  if ( rcevt_wait_name(HDRSRCH_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  if ( rcevt_signal_name(HDRDEC_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }


  /* RCINIT calls init function defined in task scons file in rcinit context
     All tcb related intializations should be done after rcinit starts our
     task */
   /* Store task tcb pointer */
  hdrdec_tcb_ptr = rex_self();
  /* Initialize task timers and queues */

  hdrdec_dog_rpt_var = dog_hb_register_rex( (rex_sigs_type) HDRDEC_RPT_TIMER_SIG );
  /* Register with dog HB */

  /* Initialize message client and register for messages */
  hdrdec_msg_init();

  /*-------------------------------------------------------------------------
   Send the first watchdog report and set the timer for report interval.
  -------------------------------------------------------------------------*/
  hdrdec_process_wdog();

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  for(;;)
  {

    rex_signals_mask = hdrdec_wait( HDRDEC_ENTER_HDR_SIG |
                                    HDRDEC_EXIT_HDR_SIG  |
                                    TASK_OFFLINE_SIG     | 
                                    TASK_STOP_SIG        |
                                    HDRDEC_EVENT_SIG );

    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_STOP_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "TASK_STOP_SIG received");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, TASK_STOP_SIG );

      /* Deregister the registered messages and delete the messenger client */
      hdrdec_msg_stop();

    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_OFFLINE_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "TASK_OFFLINE_SIG received");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, TASK_OFFLINE_SIG );

    }

    /*-----------------------------------------------------------------------
     Enter HDR signal. Clear signal, and enter subtask.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRDEC_ENTER_HDR_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "HDRDEC_ENTER_HDR_SIG received");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_ENTER_HDR_SIG );

      /* Enter HDR mode processing subtask */
      hdrdec_enter_hdr();

    }

    /*-----------------------------------------------------------------------
     Exit HDR signal. Just clear signal.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRDEC_EXIT_HDR_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_ERROR,  
                    "HDRDEC_EXIT_HDR_SIG received...already exited");
      (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_EXIT_HDR_SIG );

      /* Do nothing! Already exited. */
    }

    /*-----------------------------------------------------------------------
     Signal for MSGR. Ingore since HDRDEC is offline.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRDEC_EVENT_SIG ) != 0 )
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR, 
                    "Rx'ed HDRDEC_EVENT_SIG when HDRDEC is offline" );

      (void) rex_clr_sigs( HDRDEC_TASK_PTR, HDRDEC_EVENT_SIG );

      while ( q_cnt( &hdrdec.event_q ) != 0 )
      {
        event_ptr = (hdrdec_event_s_type*) q_get ( &hdrdec.event_q );
          /* Dequeue the next event from the event queue */
        if ( event_ptr != NULL )
        {
          hdrbuf_done( &event_ptr->hdr );
            /* Return the event buffer back to free queue */
        }
        else
        {
          ERR_FATAL( "event_ptr is NULL", 0, 0, 0 );
        }
      }
    }

  } /* Task loop */

} /* hdrdec_task */

/*============================================================================
  HDRDEC_INIT_CLOCK_SPEED
*/
/**
  Initializes decoder clock speed 

  Initialized decoder clock speed based on whether AT is capable of DO 
  release B or not. 

  @sideeffects This API would stop the TDEC and OFFLINE clock briefly to 
  program the clock speed. So, it should only be called at power-up. */
/*===========================================================================*/
void hdrdec_init_clock_speed 
( 
  boolean is_dorb_capable 
    /**< True if the AT is DORB capable; False otherwise */
)
{

  /* This function is not supported on 8960 and later targets */

} /* hdrdec_init_clock_speed */

/*============================================================================
  HDRDEC_LOG_PKT_HEADERS
*/
/**
  Log the pkt PHY header through the logging service.

  @context HDRRX task. 

  @sideeffects: This function disables preemption briefly to avoid race 
  condition that could arise from reading some variables updated in the 
  HDR decoder task context as well. */
/*============================================================================*/
void hdrdec_log_pkt_headers
(
  boolean immediate
  /**< Do we log the packet immediately? */
)
{
  LOG_HDR_FL_PHY_PKT_HEADER_C_type* log_ptr;
    /* pointer to fwd stats log packet */

  uint8  rec_cnt;

  static uint8 frm_cnt = 0;

  hdrdec_cmd_s_type cmd_buf;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( rex_self() != HDRDEC_TASK_PTR )
  {
    /*-------------------------------------------------------------------------
     Not in HDRDEC task context. Queue the command for execution.
    -------------------------------------------------------------------------*/
    cmd_buf.cmd = HDRDEC_LOG_PKT_HEADERS_CMD;
    cmd_buf.data.immediate = immediate;
    hdrdec_queue_cmd( &cmd_buf, sizeof( cmd_buf ) );
  }
  else
  {
  /* use the frm_cnt to log every other frame */
  if ((!immediate) && ( frm_cnt == 0 )) 
  {
    frm_cnt ++;
    return;
  }

  frm_cnt = 0;
    /* Reset the frm cnt */
  
  rec_cnt = hdrdec_pkt_header_log_buf.rec_cnt;
    /* get the number of pending record in the log buffer */

  if ( rec_cnt >= 1 ) 
  {

    /*------------------------------------------------------------------------
      The following section is critical because the 
      hdrdec_pkt_header_log_buf is updated in the ISR context as well; the
      section needs block the ISR 
      ----------------------------------------------------------------------*/

    rec_cnt = hdrdec_pkt_header_log_buf.rec_cnt;
      /* Get the rec count again after lock the ISR */

    /* Just for the Sanity Check; This should not happen */
    if (rec_cnt > HDRDEC_PKT_HEADER_LOG_BUF_SIZE ) 
    {
      rec_cnt = HDRDEC_PKT_HEADER_LOG_BUF_SIZE; 

      /* _ETV_ Shouldn't probably error fatal here... Replace with an
         error message? */
      ERR_FATAL( "PHY header log cnt is over the limit !", 0, 0, 0 );
    }

    log_ptr = ( LOG_HDR_FL_PHY_PKT_HEADER_C_type * )
              log_alloc( LOG_HDR_FL_PHY_PKT_HEADER_C,
                sizeof ( LOG_HDR_FL_PHY_PKT_HEADER_C_type ) 
                + ( rec_cnt - 1 ) * sizeof(log_hdr_fl_pkt_header_info_type) );
      /* Allocate memory for log packet */


    if ( log_ptr != NULL )
    {
      /* Store the record cnt in the log packet*/
      log_ptr->rec_cnt = rec_cnt;
      log_ptr->version = LOG_HDR_FL_PHY_PKT_HEADER_VERSION;

      /* memCopy the stored msg */
      /*lint -save -e669 The memcpy below copies multiple consecutive records
        in a single shot. It is more efficient that looping around. There is
        a sanity check on the rec_cnt above which ensures that this memcpy will
        not overflow. So it is safe to ignore this lint warning */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy(( void* )&(log_ptr->fl_pkt_header_info),
               rec_cnt * sizeof(log_hdr_fl_pkt_header_info_type),
              ( void* )&(hdrdec_pkt_header_log_buf.rec[0]),
               rec_cnt * sizeof(log_hdr_fl_pkt_header_info_type));
#else
      memcpy(( void* )&(log_ptr->fl_pkt_header_info),
             ( void* )&(hdrdec_pkt_header_log_buf.rec[0]),
             rec_cnt * sizeof(log_hdr_fl_pkt_header_info_type));
#endif /* FEATURE_MEMCPY_REMOVAL */
      /*lint -restore */
    }

    memset( &(hdrdec_pkt_header_log_buf), 0,
            sizeof(hdrdec_pkt_header_log_buf));
      /* Clean up the log buffer */


    log_commit((log_type*)log_ptr);
      /* Commit the log packet to the logging service */
  }

  HDR_MSG_PROT_1 ( MSG_LEGACY_LOW, 
                   "Logging PHY PKT Header %d rec_cnts", rec_cnt );

  } /* HDRDEC task context */

} /* hdrdec_log_pkt_headers */

/*============================================================================
  HDRDEC_GET_PACKET_INFO
*/
/**
  This function copies packet info.

  @context Calling task */
/*===========================================================================*/
void hdrdec_get_packet_info
(
  hdrdec_pkt_info_s_type *pkt_info_ptr
  /**< Pointer to the decoder packet info structure to be copied to */
)
{
#ifdef FEATURE_MEMCPY_REMOVAL
  *pkt_info_ptr = hdrdec.pkt_info;  
#else
  memcpy(pkt_info_ptr, &hdrdec.pkt_info, sizeof(hdrdec_pkt_info_s_type));
#endif /* FEATURE_MEMCPY_REMOVAL */
}


#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
/*===========================================================================
 HDRDEC_GET_LAST_FTC_PKT_HSTR
*/
/**
  Returns the HSTR timestamp of last FTC packet from decoder header.

  @context Calling task */
/*===========================================================================*/
uint16 hdrdec_get_last_ftc_pkt_hstr( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrdec.latest_ftc_pkt_hstr;
    /* Get last FTC packet hstr. */

} /* hdrrx_get_last_pkt_hstr */
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
