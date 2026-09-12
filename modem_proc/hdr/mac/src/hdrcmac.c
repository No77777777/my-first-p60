/*! @mainpage This module contains the implementation of the HDR Control 
              Channel Media Access Control (MAC) Protocol.
    @file hdrcmac.c
    @brief This module contains definitions and declarations necessary for 
           using the HDR Control Channel MAC Module. */

/*===========================================================================
  Copyright (c) 2000 - 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$PVCSPath: L:/src/asw/COMMON/vcs/hdrcmac.c_v   1.46   10 Apr 2003 11:22:00   kevins  $
$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrcmac.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
02/02/16   vke     Changes to remove less significant F3s
08/20/15   arm     Atlas RUMI change, skip system lost if CC pkt HSTR is 
                   incorrect
08/19/15   rmv     Skip reporting system lost if the first pkt hstr check for 
                   enh CC rel A short pkt is failed
12/03/14   ljl     Declared system lost if CC packet HSTR was incorrect.
07/09/14   cnx     Support NV refresh without reset.
06/13/14   vlc     Added support for Enhanced DLNA. 
05/12/14   mbs/vlc Set clear pending indicator in function 
                   hdrcmac_enable_missed_scc_detection().  
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
09/09/13   rmg     Disabled early preamble detection in TAP mode.
08/19/13   rmg     Added support to disable DLNA in TAP mode.
07/31/13   rmg     Fixed SCC offset update during traffic.
07/23/13   rmg     Enabled D-LNA by default.
05/24/13   rmg     Added D-LNA support.
05/21/13   kss     Excluded QPCH packets from subsync end of packet checks.
05/02/13   ljl     Updated scc offset and scc rate to srch.
04/18/13   kss     Added conn fail as trigger to update CC short pkt settings
03/18/13   kss     Removed unused legacy code. 
01/07/13   kss     Don't act on missed preamble until one packet has been 
                   rx'd after time sync.
10/11/12   wsh     Do not ignore SessionChange in tunnel mode 
03/08/12   wsh     Merged NLB support
03/16/12   arm     Resolve TCB errors for Dime. 
02/27/12   smd     Added updates of FTM msgr interface changes.
11/16/11   rmg     Added workaround to short packet decode issue.
11/08/11   lyl     Added FW state check when cmac is deactivated.
10/28/11   lyl     Checked the FW state before sending CC demod config cmd.
08/04/11   cnx     Merged Optimized Handoff changes.
02/10/11   kss     Remove recently added rex pre-emption calls.
12/10/10   kss     Fixed sync issue by validating sync pkt hstr.
11/01/10   kss     Adjust sync HSTR during first packet processing to avoid 
                   rollover issues.
10/12/10   kss     Enable and process of missed scc indication (from hdrrx).
08/18/10   wsh     Workaround for HDRMC queue full
06/21/10   cnx     Fixed compiler warnings.
06/03/10   cnx     Fixed Klocwork errors.
05/18/10   arm     Remove Lint Warning in hdrcmac_pending_decode_has_sync_candidate()
02/03/10   mbs     Use correct bit to adjust the hstr MSB rollover
03/10/09   kss     Add support for hdrShortPacketMode_Valid field.
12/02/09   grl     Added HDR_FW_CC_DEMOD_CFG_CMD registration.
12/01/09   kss     Added frame time debug message.
07/29/09   etv     Used hdrsrch APIs to get/set cc_start and SCC offset.
05/22/09   etv     Added support for Q6 message interface
04/21/09   etv     Update for hdrdec_pkt_type_enum_type name change.
04/27/09   kss     For bad pkts, only calc pkt_num if previous pkt was rx'd.
03/31/09   kss     Added hstr error check for first packet of SCC.
03/23/09   wsh     CMI: featurized CMI under FEATURE_CMI
02/03/09   kss     Merge IntraSCC support, enh sleep logging, and QPCH.
11/18/08   kss     Removed/replaced intlocks.
11/10/08   kss     Fixed issue in calculation of cc_start value.
10/08/08   kss     Mainline FEATURE_HDR_CMAC_SUBSYNC_STDBY_OPT.
09/22/08   kss     Fixed featurization issue with last checkin.
09/04/08   kss     Clear cc_start on last_packet, avoid overflow.
02/28/07   etv     Fixed RVCT compiler warnings.
02/08/07   etv     Enable/Disable short packets after session changed ind.
01/09/07   etv     Give sleep indication after sub-SYNC wakeup.
11/02/06   kss     Track serving link_id; update scc based on link_id change.
10/26/06   etv     Added support for on-arrival delivery of RX packets.
09/28/06   etv     Fixed CMAC Short Pkt processing bug. 
08/10/06   etv     Made CC Pkt Rx'ed F3 message more meaningful.
07/13/06   etv     Fixed HSTR adjustment bug (CR 95258).
07/21/06   etv     Start with Current CMAC subtype on powerup instead of 
                   Default.
06/20/06   etv     Handle HDRRX timers as events.
07/06/06   mpa     Suspend CMAC timer instead of resetting if protocols are 
                   suspending.
06/12/06   sq      Remember the last SCCC rate
06/08/06   mpa     Fixed relA featurization.
05/09/06   etv     Fixed lint comment.
03/02/06   kss     Receive LMAC_AWAKE as trigger to update CC packet config.
02/23/06   kss     Added informational F3 messages.
01/18/06   etv     Temp. disable mDSP register writes to avoid race condition.
12/13/05   pba     Modified SCMDB calls to use updated API's
12/06/05   etv     Added support for Session Neg. using SCM.
09/28/05   sq      Detect sub-sync CC preamble miss. 
07/08/05   etv     Removed FEATURE_HDR_REVA_L1 for CCMAC_INTERRUPTED ind. 
06/21/05   etv     Added support for Rev-A. 
01/25/05   etv     cleaned lint errors.
11/03/04   mpa     Removed dependency on std.h (merged with sys.h)
09/16/03   mpa     Converted F3 messages to use new MSG2.0 SSID
09/16/03   kss     Use hstr%512 rather than raw hstr.
04/10/03   kss     Updated missed preamble message.
04/04/03   kss     Added function hdrcmac_sync_cc_is_missing().
03/11/03   kss     Write SCC offset value to MDSP.
01/29/03   kss     Tell hdrrx about SyncCC offset.
01/10/03   kss     Fixed bug in CC duplicate check.
09/19/02   kss     Added check for duplicate rate 1 CC packets; added support
                   for logging CC packets.
05/15/02   hcg     Check for bad SL packet lenght for bcast ATI.
04/22/02   kss     Handle the case where sl_pkt_len is bad gracefully.
04/17/02   mpa     Renamed hdramp_ to hdrampati_match_receive_ati_address
04/16/02   dna     Added HDRIND_CCMAC_SLEEP_CAPSULE_DONE support
03/12/02   sq      Added support for Factory test mode
02/12/02   kss     Set decoder's state timeout at init.
11/28/01   kss     Fixed typo in parsing of CC Slot offset.
09/18/01   kss     Moved end of sync indication to after CC MAC message 
                   processing.
09/04/01   kss     Fixed problem with SyncCC offset adjustment.
06/07/01   kss     Added support for DDRs #9, #10 (first pkt and sleep bits)
06/04/01   kss     Adjust hstr by SyncCC offset.
05/08/01   kss     Changed to hdr-based task. (hdrrx)
05/02/01   kss     Interface change for IS-856.
02/09/01   kss     Correctly use return value of hdrsboot_get_ati(). 
01/26/01   dna     Set BCAST_BIT properly in app_field of received CC msgs.
12/14/00   kss     Added end-of-sync capsule indication call.
11/03/00   dna     Added HDR event logging
03/30/00   st      Created.

===========================================================================*/

/* EJECT */
/*===========================================================================

                          INCLUDE FILES FOR MODULE
                     
===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#include "err.h"
#include "hdrdebug.h"
#include "rex.h"
#include "task.h"

#include "hdrrx.h"
#include "hdrrxi.h"
#include "hdrcmac.h"
#include "hdrcmaci.h"
#include "hdrcmacsm.h"
#include "hdrdec.h"
#include "hdrbuf.h"
#include "hdrind.h"
#include "hdrhai.h"
#include "hdrerrno.h"
#include "hdrdsm.h"
#include "hdrampati.h"
#include "hdrpcp.h"
#include "hdrtrace.h"
#include "hdrmdspmcr.h"
#include "hdrts.h"
#include "hdrlogi.h"
#include "hdrscp.h"
#include "hdrscmcmac.h"
#include "hdrscmdb.h"
#include "hdrmsg.h"
#include "hdrsrch.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#ifdef FEATURE_FACTORY_TESTMODE
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "dsmbit.h"
#include  "ftm.h"
#endif

#include "hdrmdsp.h"
#include "hdrsrchlog.h"

#include "hdrmc_v.h"

#ifdef FEATURE_LTE_TO_HDR_OH
#include "hdrsap.h"
#endif /* FEATURE_LTE_TO_HDR_OH */

#ifdef FEATURE_HDR_REVC
#include "hdrlmac.h"
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_DLNA
#include "hdrsrchutil.h"
#include "hdridlec.h"
#endif /* FEATURE_HDR_DLNA */

/* EJECT */
/*===========================================================================
                   
                   DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions and declarations for constants, macros,
types, variables and other items needed by this module.

===========================================================================*/
/** Pointer to HDRCMAC Task control block */
#define HDRCMAC_TASK_PTR HDRRX_TASK_PTR

/** Control Channel Cycle length, in slots */
#define HDRCMAC_CC_CYCLE_SLOTS                        256

/** Number of interlaces. */
#define HDRCMAC_NUM_INTERLACES                        4

/** Maximum Number of slots after which CC1 (38.4) packets are declared bad
    CRC on failed decode */
#define HDRCMAC_CC1_MAX_NUM_SLOTS                     64

/** Maximum Number of slots after which CC2 (76.8) packets are declared bad
    CRC on failed decode */
#define HDRCMAC_CC2_MAX_NUM_SLOTS                     32

/** Max possible value for HSTR of first SCC packet when time is
    synchronized. */
#define HDRCMAC_FIRST_SCC_PACKET_HSTR_MAX             6

/** Indicates that no previous sync packet has been received. */
#define HDRCMAC_NO_SYNC_IN_PROGRESS                   -1

/** Short packet MAC index */
#define HDRCMAC_CTRL3_MAC_INDEX                       0x0047

/** This value is used to distinguish whether a missed preamble interrupt
    occured due to the first packet of a syncCC capsule.  If the isr is
    processed anytime before this time, we assume that it was due to
    missing the preamble of that first packet (which should have been
    detected in somewhere between halfslots 0-7). */
#define HDRCMAC_FIRST_SYNC_CC_HSTR_LIMIT              64

/** Macro finds a SyncCC packet duration in slots based on its rate.
    This is only valid for SyncCC packet (not async or subsync, which can
    have a Rev-A format. */
#define HDRCMAC_SCC_PACKET_DURATION( scc_rate )                   \
    ( (scc_rate == HDRHAI_DRC_76_8)? HDRCMAC_CC2_MAX_NUM_SLOTS:   \
      HDRCMAC_CC1_MAX_NUM_SLOTS )

/** Rev-A system */
#define HDRCMAC_SYSTEM_MODE_REV_A                     1

/** Rev-0 system */
#define HDRCMAC_SYSTEM_MODE_REV_0                     0

/** Short packets enabled */
#define HDRCMAC_SHORT_PKT_ENABLED                     1

/** Short packets disabled */
#define HDRCMAC_SHORT_PKT_DISABLED                    0

/** Value to indicate no premable detected in possible SCC slots */
#define HDRCMAC_NO_SCC_PREAMBLE_DETECTED                       0xFFFF

/*===========================================================================
                                TYPEDEFS
===========================================================================*/

/** Enum for CMAC timers */
typedef enum
{
  HDRCMAC_SUPERVISION_TIMER
    /**< Supervision timer */

} hdrcmac_timer_name_enum_type;

/*===========================================================================
                            GLOBAL VARIABLES
===========================================================================*/

/** Static data in HDR Control Channel MAC module */
LOCAL struct
{

  uint8 prev_pkt_link_id;
    /**< The link ID of the previous packet */

  uint8 serving_link_id;
    /**< The link ID of the serving sector */

#ifdef FEATURE_HDR_REVA_L1

  uint16 current_subtype;
    /**< Current negotiated CMAC subtype */
#endif

  boolean waiting_for_last_pkt;
    /**< Whether we are waiting for a Sync. Control Channel Capsule to finish */

  int16 last_packet_num_received;
    /**< Packet number of last sync packet received.  */

  hdrscmcmac_config_struct_type cfg;
    /**< Config struct instance */

  uint8 scc_rate;
    /**< most recent SCC rate */

  rex_timer_type sup_timer;
    /**< Supervision timer. */

  int16 scc_offset_slots;
    /**< SCC offset in slots */

  uint16 cc_start;
    /**< CC start in slots */

  rex_timer_cnt_type saved_superv_timer_ms; 
    /**< Saved time left on supervision timer when Idle is suspended.  When 
         CMAC is re-activated, the CMAC timer will resume if this value is 
         non-zero. Otherwise, it will be re-started for the full duration of 
         the timer (5.12s) */

  hdrind_cmac_intra_scc_info_type intra_scc_info;
    /**< IntraSCC info */

  boolean process_missed_scc_flag;
    /**< Whether missed scc indication should be processed */

  boolean missed_scc_flag_rcvd;
    /**< Whether a missed scc indication has been received */

  rex_crit_sect_type timestamp_crit_sect;
    /**< Critical Section for protecting timestamp data */

  boolean block_missed_scc_flag;
    /**< Block missed scc processing */

#ifdef FEATURE_HDR_DLNA
  uint16 dlna_disable_requests;
    /**< Mask to track entities requesting DLNA disable */
#endif /* FEATURE_HDR_DLNA */

} hdrcmac;

/** Half-slot timestamp. */
hdrcmac_timestamp_struct_type hdrcmac_timestamp;

/*==========================================================================
                             LOOK-UP TABLES 
===========================================================================*/

/** Sizes of CMAC physical layer packets in bits; 0xFFFF indicates invalid
    pkt_size_index */
uint16 hdrcmac_phy_pkt_size_bits_table [8] = 
  { 128, 256, 512, 1024, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };


/*==========================================================================
                          FUNCTION DECLARATIONS
===========================================================================*/

#ifdef FEATURE_HDR_DLNA
uint16 hdrcmac_get_next_scc_slot( void );
#endif /* FEATURE_HDR_DLNA */


/*============================================================================

                        LOCAL FUNCTION DEFINITIONS

=============================================================================*/

/*============================================================================
  HDRCMAC_TIMER_CB
*/
/**
  This function is the callback for all CMAC protocol timers.

  This function is called by TIMER task when the timer expires. The timer
  expiry processing itself will happen in the context of the task the
  defined the timer (HDRRX). So, this function would only queue an event
  for that task to process the timer expiry.

  @context TIMER task. */
/*============================================================================*/
void hdrcmac_timer_cb
(
  uint32 timer_id
    /**< The ID of the timer that has expired */
)
{
  /* Queue the timer expiry event to hdrrx task's event queue */
  hdrrx_queue_timer(HDRHAI_CC_MAC_PROTOCOL, timer_id);

} /* hdrcmac_timer_cb() */

/*============================================================================
  HDRCMAC_IS_ACTIVE
*/
/**
  This function queries the current state of the protocol.

  @return TRUE if protocol is Active and FALSE otherwise.

  @context Calling task (HDRRX, HDRMC) This routine is re-entrant */
/*============================================================================*/
boolean hdrcmac_is_active 
( 
  hdrhai_protocol_name_enum_type caller 
    /**< The protocol calling this externalized function (for sw trace) */
)
{

  SYS_ARG_NOT_USED( caller );

/* For FTM mode, we assume the CMAC is always active        */
#ifdef FEATURE_FACTORY_TESTMODE
  if (ftm_get_mode() == FTM_MODE)
  {
    return TRUE;
  }
  else
  {
    return((hdrcmacsm_get_state() != HDRCMACSM_INACTIVE_STATE) ? TRUE : FALSE);
  }
#else
  return((hdrcmacsm_get_state() != HDRCMACSM_INACTIVE_STATE) ? TRUE : FALSE);
#endif

} /* hdrcmac_is_active */

/*============================================================================
  HDRCMAC_PENDING_DECODE_HAS_SUB_SYNC_CANDIDATE
*/
/**
  This function is used to find out if any of the decodes that are active
  at the moment has any potential candidates for a sub-sync packet.

  @return TRUE, if we have a decode active in any interlace that could be a 
  Sub-Sync control channel packet; FALSE Otherwise.

  @context Calling task (HDRRX, HDRDEC). This routine is reentrant */
/*============================================================================*/
boolean hdrcmac_pending_decode_has_sync_candidate
(
  uint16 cc_start
    /**< Current CC Start */
)
{

  boolean candidate_found = FALSE; 
    /* Do we have a Sync/Sub-Sync candidate in any of the other interlaces? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check currently not supported */

  HDR_MSG_PROT_2(MSG_LEGACY_HIGH, 
                 "StdbyOpt2: Sync/SubSync candidate found %d, CC_start %d", 
                 candidate_found, cc_start);

  return candidate_found;

} /* hdrcmac_pending_decode_has_sync_candidate */


/*============================================================================

              EXTERNAL (TO ONLY MAC MODULE) FUNCTION DEFINITIONS

         Exported through hdrcmaci.h (scope restricted to MAC module)
=============================================================================*/

/*============================================================================
  HDRCMAC_PROCESS_CMD
*/
/**
  This function processes the commands for this protocol that were queued
  on the task command queue from a different task context.

  @context HDRRX task. */
/*============================================================================*/
void hdrcmac_process_cmd 
( 
  const hdrcmac_cmd_union_type *cmd_ptr                
    /**< pointer to received command buffer */
)
{ 
  switch ( cmd_ptr->hdr.cmd )
  {
    case HDRCMAC_ACTIVATE_CMD:
    {
      hdrcmac_activate(cmd_ptr->hdr.sender);
      break;
    }

    case HDRCMAC_DEACTIVATE_CMD:
    {
      hdrcmac_deactivate(cmd_ptr->hdr.sender);
      break;
    }

    case HDRCMAC_ENABLE_MISSED_SCC_DETECTION_CMD:
    {
      hdrcmac_enable_missed_scc_detection(cmd_ptr->hdr.sender, 
        cmd_ptr->enable_scc.clear_pending_ind);
      break;
    }

    case HDRCMAC_DISABLE_MISSED_SCC_DETECTION_CMD:
    {
      hdrcmac_disable_missed_scc_detection(cmd_ptr->hdr.sender);
      break;
    }

#ifdef FEATURE_HDR_DLNA
    case HDRCMAC_FORCE_DLNA_DISABLE_CMD:
    {
      hdrcmac_force_dlna_disable(cmd_ptr->disable_dlna.caller_id, 
        cmd_ptr->disable_dlna.disable);
      break;
    }
#endif /* FEATURE_HDR_DLNA */

    default:
    {
      ERR("Invalid command! 0x%h",cmd_ptr->hdr.cmd,0,0);
    }
  }

} /* hdrcmac_process_cmd */

/*============================================================================
  HDRCMAC_PROCESS_TIMER
*/
/**
  This function handles the timer expiry events assigned to this protocol.

  @context HDRRX task. */
/*============================================================================*/
void hdrcmac_process_timer
( 
  uint32 timer_id
    /* ID of the timer just expired. */
)
{
  switch (timer_id)
  {
    /* Control Channel MAC Supervision Timer */
    case HDRCMAC_SUPERVISION_TIMER:

      HDRTRACE_HDRRX_CMAC_TIMER_SIG( hdrcmacsm_get_state() );

      HDR_MSG_PROT (MSG_LEGACY_MED,  "HDRCMAC_SUPERVISION_TIMER expired!");

      /* Increments the CMAC Timer expiry count for SLEEP INFO log packet */
      hdrsrchlog_update_cmac_expiry_count( );

      /* Give Supervision Failed indication to CMAC */
      hdrind_give_ind( HDRIND_CCMAC_SUPERVISION_FAILED, NULL );

      /* Transition state to Inactive. */
      hdrcmacsm_change_state( HDRCMACSM_INACTIVE_STATE );

      break;

    default:
      /* Unrecognized Timer */
      HDR_MSG_PROT_1 (MSG_LEGACY_ERROR, "Invalid CMAC Timer ID (%d)", timer_id);
      break;
  }

} /* hdrcmac_process_timer */

/*============================================================================
  HDRCMAC_PUT_PKT
*/
/**
  This function parses a Control Channel packet for Security Layer packets.
  Security Layer packets that are of broadcast type are forwarded to the 
  Security Layer for processing.  Security Layer packets that are of mobile
  directed type are forwarded to the Security Layer if the ATI value matches
  the ATI value currently in use by the Security Layer.  All other Security
  Layer packets are discarded.

  @sideeffects The input Control Channel packet buffer is freed.
  
  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
void hdrcmac_put_pkt 
( 
  dsm_item_type      *cc_pkt_ptr          
    /**< Pointer to buffered Control Channel Packet. */
)
{ 
  /* Access Terminal Identifier (ATI) record. */
  hdrhai_ati_record_type  received_ati;

  /* Control Channel packet payload byte offset. */
  uint16              cc_pkt_offset;

  /* Security Layer packet length, in bytes. */
  uint16              sl_pkt_len;      

  /* Security Layer packet pointer. */
  dsm_item_type      *sl_pkt_ptr;

  /* Security Layer packet header buffer. */
  byte                sl_pkt_hdr[ HDRCMAC_SL_PKT_UATI_HDR_LEN ];

  /* Control Channel packet header buffer. */
  byte                cc_pkt_hdr[ HDRCMAC_CC_PKT_HDR_LEN ];

  /* Security Layer packet size included status.  Always TRUE for Security
     Layer packets that are contained in Control Channel packets. */
  boolean             is_size_inc; 

  /* Flag if the packet was addressed with BATI or UATI. */
  boolean             is_pkt_bcast;

  /* Link ID of packet. Identifies the sector that sent the pkt. */
  uint8               pkt_link_id;

  /* Size of CMAC Packet in bytes */
  uint16              cmac_pkt_size_bytes = 0;

  /* Packet's slot time reference */
  uint16              str = 0;

  /* Flag if HDRIND_CCMAC_SCC_PKT_RXED indication needs to be sent */
  boolean             send_scc_pkt_rxed_ind = FALSE;

  /* Payload for HDRIND_CCMAC_SCC_PKT_RXED indication*/   
  hdrind_ind_data_union_type ind_data;

  /* Temp for computing cc_start. */
  uint16 temp_cc_start; 

  /* Packet duration, in slots */
  uint16 scc_packet_duration;

  /* Index for mapping CMAC physical layer packets size in bits */
  uint8 pkt_size_ind;

  /* Temp value used only to check whether time is valid. */
  hdr_timestamp_t     dummy_ts;

  /* @todo Remove frame time F3 message */
  qword sync_time;

  /* 0-based sl packet index within this CC pkt */
  uint8               sl_no = 0;

  /* Current HSTR */
  uint16                               hstr16_now;

  /* 16 bit HSTR of CMAC pkt that contained the SYNC message */
  uint16                               hstr16_pkt;

  /* HSTR value of sync first packets stored in timestamp buffer */
  uint16                               hstr_value;

#ifdef FEATURE_HDR_REVC
  boolean                              pri_cc;
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_DLNA
  uint16                               cc_start_sci;
    /* Slot for next control channel packet start for current SCI config.
       This includes subsync CC pkts. */

  uint8                                odd_pream_det_duration;
    /* Duration in slots for odd preamble detection. */
#endif /* FEATURE_HDR_DLNA */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Clear intra scc info */
  memset( (void*) (&hdrcmac.intra_scc_info),
    0,
    sizeof(hdrcmac.intra_scc_info) );

    /* Take note of link id. */
  pkt_link_id = HDRDSM_GET_LINK_ID( cc_pkt_ptr );

  /* Always clear this flag on receiving a CC packet. */
  hdrcmac.block_missed_scc_flag = FALSE;

#ifdef FEATURE_HDR_REVC
  pri_cc = hdrlmac_is_linkid_on_cc_monitoring_chan(pkt_link_id);
  /* to be removed after IOT */
#endif /* FEATURE_HDR_REVC */

  /* Find the length of CMAC packet excluding CRC and TAIL */
  cmac_pkt_size_bytes = dsm_length_packet(cc_pkt_ptr);

  /*-------------------------------------------------------------------------
   Determine if the protocol is in the Active state.  If so, process the
   Control Channel packet.  Otherwise, discard the Control Channel packet.
  -------------------------------------------------------------------------*/
  if ( hdrcmac_is_active(HDRHAI_CC_MAC_PROTOCOL) == TRUE )
  {

    pkt_size_ind = HDRDSM_GET_PACKET_SIZE( cc_pkt_ptr );

    /* Make sure index is not out of bound */
    pkt_size_ind = (pkt_size_ind < 8)? pkt_size_ind:7;

    HDR_MSG_PROT_3( 
      MSG_LEGACY_HIGH,
      "CC Pkt rx'd: Release: %c, Size: %d bits, Slot %d", 
      ((HDRDSM_GET_PACKET_TYPE( cc_pkt_ptr ) & 0x3)? 'A': '0'),
      (hdrcmac_phy_pkt_size_bits_table[pkt_size_ind]),
      (HDRDSM_GET_TIMESTAMP( cc_pkt_ptr ) / 2));

    /*-----------------------------------------------------------------------
     Initialize the Control Channel packet byte offset to zero and extract 
     the Control Channel packet header from the beginning of the packet.
    -----------------------------------------------------------------------*/
    cc_pkt_offset = 0;
    (void) dsm_extract( cc_pkt_ptr, cc_pkt_offset, cc_pkt_hdr, 
                 HDRCMAC_CC_PKT_HDR_LEN );

#ifdef FEATURE_HDR_REVC
    if (pri_cc)
    {
#endif /* FEATURE_HDR_REVC */
    /* Get CC Start from firmware */
    hdrcmac.cc_start = hdrsrch_get_cc_start();

    /*-----------------------------------------------------------------------
     If this is a Synchronous Control Channel Capsule,
     get a copy of the packet timestamp and restart the supervision timer. 
    -----------------------------------------------------------------------*/
    if ( HDRCMAC_IS_SYNC_CAPS_PKT( cc_pkt_hdr ) )
    {
      /* Save the current SCC rate  */
      hdrcmac.scc_rate = HDRDSM_GET_DRC( cc_pkt_ptr);
      
      /* If we are waiting for a SYNC capsule to finish, the packet just
         received should be a continuation of the SYNC capsule if it is sent by 
         the same Sector. Check if it is sent by the same sector by comparing
         the link ID */
      if ( hdrcmac.waiting_for_last_pkt )
      {
        if ( hdrcmac.prev_pkt_link_id != pkt_link_id )
        {
          /* The link ID has changed. The AT must have handed-off to another
             sector. Give CCMAC_Interupted indication to SLP to flush the 
             SYNC capsule buffered */
          hdrind_give_ind( HDRIND_CCMAC_INTERRUPTED, NULL );

          /* Since we have flushed the buffered sync capsule, reset the flag */
          hdrcmac.waiting_for_last_pkt = FALSE;
          hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;
        }
      }

      HDRDSM_SET_CC_SYNC_BIT( cc_pkt_ptr );

      if ( (HDRCMAC_IS_FIRST_CAPS_PKT( cc_pkt_hdr ) ) )
      {

        /* @todo Remove frame time F3 message */
        if ( hdrts_get_frame_time( sync_time ) )
        {
          HDR_MSG_PROT_3( MSG_LEGACY_HIGH,
                          "SyncCC: Time hi, lo: %x, %x; hstr %x",
                           qw_hi(sync_time), qw_lo(sync_time), HDRHSTR_GET_COUNT());
        }

        /* Set last packet flag accordingly.  If this is both the first and
           the last packet, the flag will simply be cleared again below. */
        hdrcmac.waiting_for_last_pkt = TRUE;

        hdrcmac.prev_pkt_link_id = HDRDSM_GET_LINK_ID( cc_pkt_ptr );

        /* Get SyncCC offset and convert to halfslots */  
        hdrcmac_timestamp.scc_offset = (HDRCMAC_GET_OFFSET( cc_pkt_hdr )) * 2;
        hstr_value  = HDRDSM_GET_TIMESTAMP( cc_pkt_ptr );

        if (hstr_value < hdrcmac_timestamp.scc_offset)
        {
          hstr_value += 512;
        }

        /* Adjust HSTR by SyncCC offset */
        hstr_value -= hdrcmac_timestamp.scc_offset;
        
        /*---------------------------------------------------------------------
         CMAC remembers only the least significant 9 bits from the SYNC packet 
         HSTR. However the most significant 7 bits could be retrieved using
         the current HSTR count. These seven bits should be the same for both
         the HSTR counts (the pkt_hstr and current_hstr), unless bit 10 has
         rolled over. 
        ---------------------------------------------------------------------*/
        /* Get the current HSTR Count */
        hstr16_now = HDRHSTR_GET_COUNT();

        /* Strip most significant seven bits from current HSTR assuming that
           bit 10 hasn't rolled over */
        hstr16_pkt = hstr16_now & 0xFE00;

        /* Check if bit 10 has rolled over */
        if (hstr_value > (hstr16_now & 0x1FF))
        {
          /* Subtract to adjust for roll-over */
          hstr16_pkt -= 0x200;
        }

        /* Construct 16 bit packet HSTR by ORing in upper bits. */
        hstr_value |= hstr16_pkt;

        /* Entering critical section... */
        rex_enter_crit_sect(&hdrcmac.timestamp_crit_sect);

        /* Update index before recording */
        hdrcmac_timestamp.idx++;
        hdrcmac_timestamp.idx %= HDRCMAC_TIMESTAMP_BUFFER_COUNT;

        /* Record and mark as valid */
        hdrcmac_timestamp.info[hdrcmac_timestamp.idx].value = hstr_value;
        hdrcmac_timestamp.info[hdrcmac_timestamp.idx].valid = TRUE;
        hdrcmac_timestamp.status = HDRCMAC_TIMESTAMP_VALID;

        /* Leave critical section */
        rex_leave_crit_sect(&hdrcmac.timestamp_crit_sect);

#ifdef FEATURE_FACTORY_TESTMODE
        /* Disable the supervision timer when we are in FTM mode    */
        if(ftm_get_mode() != FTM_MODE)
        {
          (void) rex_clr_timer ( &hdrcmac.sup_timer );
          (void) rex_set_timer ( &hdrcmac.sup_timer, HDRCMAC_SUPV_TIMER_MSECS );
        }
#else
        rex_clr_timer ( &hdrcmac.sup_timer );
        rex_set_timer ( &hdrcmac.sup_timer, HDRCMAC_SUPV_TIMER_MSECS );
#endif

        /*--------------------------*
         * Write SCC Offset to MDSP *
         *--------------------------*/

        /* We need to write the SCC offset to the DSP so that DSP can generate
           the "SCC preamble missed" interrupt in the correct slot.  Make sure
           packet is from the desired sector before updating. 
           Serving link id will be INVALID in the case of acquisition/sync.
           Always update SCC offset during traffic. */
        if ((pkt_link_id == hdrcmac.serving_link_id) ||
            (hdrcmac.serving_link_id == HDRCMAC_INVALID_LINK_ID) ||
            (hdrfmac_is_active(HDRHAI_CC_MAC_PROTOCOL)))
        {
          /* Remember the SCC offset */
          hdrcmac.scc_offset_slots = (hdrcmac_timestamp.scc_offset / 2);

          /* Tell hdrrx about offset */
          hdrrx_set_cc_offset(hdrcmac_timestamp.scc_offset);

          /* Write the new SCC to the DSP.  Divide by two since the
             DSP takes slots and we are storing it in halfslots. */
          hdrsrch_set_scc_info ( (hdrcmac_timestamp.scc_offset/2), hdrcmac.scc_rate, pkt_link_id );

          HDR_MSG_PROT_3( MSG_LEGACY_MED,"SCC_offset: %d SCC rate %d Link id: %d", 
                          (hdrcmac_timestamp.scc_offset/2), hdrcmac.scc_rate, pkt_link_id);
        }

        /* Check that HSTR is in expected range for first packet of SCC. 
           If DO time is valid, HSTR must be even and <= 6. */
        if ((HDRDSM_GET_TIMESTAMP( cc_pkt_ptr ) > 
               HDRCMAC_FIRST_SCC_PACKET_HSTR_MAX) ||
            ((HDRDSM_GET_TIMESTAMP( cc_pkt_ptr ) % 2 ) != 0)) 
        {   
          /* Check that time is valid.  Note check may be slightly expensive, 
             so only check when HSTR is suspect.   Separate conditional used
             for clarity (rather than relying on lazy evaluation). */
          if (hdrts_get(dummy_ts) == TRUE)
          {
            HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                           "HDRCMAC: Sync first packet bad hstr: %d", 
                           HDRDSM_GET_TIMESTAMP( cc_pkt_ptr ));
#ifndef FEATURE_HDR_RUMI
            /* In some commercial deployments, it is observed that N/W is
               sending enh Rel A CCMAC short pkts with sync capsule bit and
               first packet bit set at hstr != 0 .
               Ideally sync capsule bit should not have been set. However
               to avoid frequent network lost due to such mis-configuration,
               provided a work around to only do system lost for Rev 0 pkts */
            if ( HDRDSM_GET_PACKET_TYPE( cc_pkt_ptr ) == HDRDEC_CC_0_PKT )
            {
              hdrsrch_system_lost( HDRSRCH_SYSLOST_BAD_CC_HSTR );
              /* declare system lost */
            }
#endif /* FEATURE_HDR_RUMI */
          }
         }

      }/* if first SCC packet */


      /* If sleep capsule done bit is set, set flag for later. */
      if ((HDRCMAC_IS_SLEEP_CAPS_DONE( cc_pkt_hdr )))
      {
        hdrcmac.intra_scc_info.sleep_caps_done = TRUE;

        /* Set flag for sending SCC_PKT_RXED indication */
        send_scc_pkt_rxed_ind = TRUE;
      }

      if (HDRCMAC_IS_LAST_CAPS_PKT( cc_pkt_hdr ))
      {
        /* Last packet Rx'ed.  Set last packet flags accordingly. */
        hdrcmac.waiting_for_last_pkt = FALSE;
        hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;
        hdrcmac.intra_scc_info.last_pkt = TRUE;
        
        /* Reset cc_start to correspond to the start of next SCC
           Capsule */
        hdrcmac.cc_start = 0;

        /* Set flag for sending SCC_PKT_RXED indication */
        send_scc_pkt_rxed_ind = TRUE;

#ifdef FEATURE_HDR_DLNA
        if ( hdrsrchutil_dlna_is_enabled() )
        {
          /* If AT is in idle state, schedule odd preamble detection for
             the next sync/subsync cc slot. */

          if ( hdrsrch_get_current_state() == HDRSRCH_IDLE_STATE )
          {
            /* Obtain next sync/subsync cc slot. The value returned by the 
             following API assumes SCC offset of zero. */
            cc_start_sci = hdrcmac_get_next_scc_slot(); 

            if( hdrcmac.scc_offset_slots != HDRRX_SCC_OFFSET_UNKNOWN )
            {
              /* Add SCC offset. */
              cc_start_sci += hdrcmac.scc_offset_slots;

              /* Mark duration of odd preamble detection as one slot */
              odd_pream_det_duration = 1;
            }
            else
            {
              /* Since SCC offset is not available, mark duration of odd 
                 preamble detection to cover all possible offsets */
              odd_pream_det_duration = 4;
            }

            /* Schedule odd preamble detection */
            hdrsrchutil_schedule_odd_pream_det( 
              ( cc_start_sci % HDRCMAC_CC_CYCLE_SLOTS ),
              odd_pream_det_duration );
          }
        }
#endif /* FEATURE_HDR_DLNA */

      }
      else
      {

        /* Get packet duration in slots */
        scc_packet_duration = HDRCMAC_SCC_PACKET_DURATION( hdrcmac.scc_rate );

        /* Update CC start based on this packet.  Do computation in temp variable,
           then copy to working version. */

        /* Refresh CC start based on this packet's time, convert to slots */
        temp_cc_start = HDRDSM_GET_TIMESTAMP( cc_pkt_ptr ) / 2;

        hdrcmac.last_packet_num_received = (temp_cc_start / 
                                           scc_packet_duration) + 1;

        /* CC start must always correspond to interlace 0 */
        temp_cc_start = (temp_cc_start / HDRCMAC_NUM_INTERLACES) 
                                       * HDRCMAC_NUM_INTERLACES;

        /* Increment the CC Start to start of next packet in capsule */
        temp_cc_start += scc_packet_duration;
        temp_cc_start %= HDRTS_SLOTS_PER_SCC; 


        /* Copy final cc_start */
        hdrcmac.cc_start = temp_cc_start;
 
        hdrcmac.intra_scc_info.cc_start = hdrcmac.cc_start;

        /* Set flag for sending SCC_PKT_RXED indication */
        send_scc_pkt_rxed_ind = TRUE;

        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                        "Continued SCC pkt expected: slot %d", hdrcmac.cc_start);

#ifdef FEATURE_HDR_DLNA
        if ( hdrsrchutil_dlna_is_enabled() )
        {
          /* Schedule odd premble detect at the time the continuing SCC packet
             is expected. */
          hdrsrchutil_schedule_odd_pream_det( 
            ( ( hdrcmac.cc_start + hdrcmac.scc_offset_slots ) 
            % HDRCMAC_CC_CYCLE_SLOTS ), 
            1 );
        }
#endif /* FEATURE_HDR_DLNA */

      }

      /* Reprogram CC start */
      hdrsrch_set_cc_start ( hdrcmac.cc_start );

      /* Disable on-arrival delivery. */
      hdrdec_disable_on_arrival_delivery();

    }/* if SCC packet */

    /*-----------------------------------------------------------------------
     If this is a Sub-Synchronous control channel packet, set flag for
     sleep indication.
    -----------------------------------------------------------------------*/

    /* If it is not a SYNC packet, and not QPCH */
    if ((!HDRCMAC_IS_SYNC_CAPS_PKT(cc_pkt_hdr)) && 
        (HDRDSM_GET_PACKET_TYPE( cc_pkt_ptr ) != HDRDEC_QPCH_PKT))
    {
      /* Did we just hand-off? */
      if (hdrcmac.scc_offset_slots != HDRRX_SCC_OFFSET_UNKNOWN)
      {
        /* SCC Offset is valid. We were NOT in the middle of an hand-off */

        /* Get packet Slot time from HSTR */
        str = (HDRDSM_GET_TIMESTAMP(cc_pkt_ptr) / 2);

        /* If the packet is Sub-Sync, Sanity check the last packet bit... Note 
           that it should always be set for sub-SYNC packets. */
        if (HDRCMAC_IS_LAST_CAPS_PKT(cc_pkt_hdr) &&
            ((str - hdrcmac.cc_start) == hdrcmac.scc_offset_slots))
        {
          /* We just received the Sub-sync packet that we woke up for.
             Set the LastPkt flag */
          hdrcmac.intra_scc_info.last_pkt = TRUE;

          /* Set flag for sending SCC_PKT_RXED indication */
          send_scc_pkt_rxed_ind = TRUE;

          HDR_MSG_PROT_3(
            MSG_LEGACY_HIGH, 
            "StdbyOpt3: cc_start: %d, SCC Offset: %d, STR: %d", 
            hdrcmac.cc_start,  
            hdrcmac.scc_offset_slots, str);
        }
        else
        {
          HDR_MSG_PROT_3(
            MSG_LEGACY_HIGH, 
            "StdbyOpt3 skipped: cc_start: %d, SCC Offset: %d, STR: %d", 
            hdrcmac.cc_start,  
            hdrcmac.scc_offset_slots, str);
        }
      }
      else
      {
        /* We do not know the SCC Offset. We are in the middle of an hand-off.
           It is not possible to say if the current packet is really a sub-sync. 
           Peek into the current decodes in each one of the interlaces to see if 
           we have a candidate for sub-sync packet. Otherwise, set LastPkt 
           flag to true. */

        /* If we did not find any other candidate for sub-sync packet being
           decoded at the moment... */
        if (!hdrcmac_pending_decode_has_sync_candidate(hdrcmac.cc_start))
        { 
          /* Declare the packet that we just received as sub-sync and 
             set the LastPkt flag */
          hdrcmac.intra_scc_info.last_pkt = TRUE;

          /* Set flag for sending SCC_PKT_RXED indication */
          send_scc_pkt_rxed_ind = TRUE;
        }
      }
    } /* If not Sync */
#ifdef FEATURE_HDR_REVC   
    } /* if primary CC */
#endif /* FEATURE_HDR_REVC */

    /*-----------------------------------------------------------------------
     Increment the Control Channel packet byte offset to the first byte 
     following the Control Channel packet header.
    -----------------------------------------------------------------------*/
    cc_pkt_offset += HDRCMAC_CC_PKT_HDR_LEN;

    /*-----------------------------------------------------------------------
     Parse the Control Channel packet for Security Layer packets.  Forward
     all broadcast packets and relevant mobile-directed packets to the
     Security Layer for processing.
    -----------------------------------------------------------------------*/
    while ( cc_pkt_offset < cmac_pkt_size_bytes )
    {  
      /*---------------------------------------------------------------------
       Extract the next Security Layer packet header and determine how many
       bytes are in the corresponding Security Layer packet.  Also determine
       if the corresponding Security Layer packet is authenticated or 
       encrypted.
      ---------------------------------------------------------------------*/
      (void) dsm_extract( cc_pkt_ptr, cc_pkt_offset, sl_pkt_hdr, 
                   HDRCMAC_SL_PKT_BATI_HDR_LEN );

      sl_pkt_len  = HDRCMAC_GET_PKT_LEN( sl_pkt_hdr );
      is_size_inc = HDRCMAC_PKT_IS_SIZE( sl_pkt_hdr );
      
      /*---------------------------------------------------------------------
       If the packet header length field is zero, there are no more valid 
       packets to process so exit. 
      ---------------------------------------------------------------------*/
      if ( sl_pkt_len == 0 ) 
      {
        break;
      }

      /*---------------------------------------------------------------------
      Else if the packet header length field indicates that the Security  
      Layer packet is longer than the space remaining in the Control Channel
      packet, then an error has occured so exit.
      ---------------------------------------------------------------------*/
      else if ( ( cc_pkt_offset + HDRCMAC_SL_PKT_LEN_FIELD_LEN + sl_pkt_len ) 
                > cmac_pkt_size_bytes )
      {
        ERR("Control Channel packet size overflow!",0,0,0);
        cc_pkt_offset = cmac_pkt_size_bytes;
        break;
      }

      /*---------------------------------------------------------------------
       Else, process the Security Layer packet.  Determine if the packet is a 
       broadcast packet, a unicast packet directed to this access terminal, 
       or a unicast packet directed to another terminal.  Broadcast packets 
       and relevant mobile-directed packets are forwarded the the Security 
       Layer.  All other packets are ignored.
      ---------------------------------------------------------------------*/
      else
      {
        /*-------------------------------------------------------------------
         Broadcast ATI.  Adjust the Security Layer packet length to exclude 
         the packet header bytes and advance the Control Channel packet byte 
         offset past the Security Layer packet header. 
        -------------------------------------------------------------------*/
        if ( HDRCMAC_GET_PKT_ATI_TYPE( sl_pkt_hdr ) == HDRHAI_BATI )
        {
          /* Make sure SL length is valid */
          if ( sl_pkt_len <= 
              ( HDRCMAC_SL_PKT_BATI_HDR_LEN - HDRCMAC_SL_PKT_LEN_FIELD_LEN ) )
          {
            ERR("Bad SL packet length (%d). Dumping pkt...",sl_pkt_len,0,0);
            break;
          }
            
          sl_pkt_len    -= HDRCMAC_SL_PKT_BATI_HDR_LEN -
                           HDRCMAC_SL_PKT_LEN_FIELD_LEN;

          cc_pkt_offset += HDRCMAC_SL_PKT_BATI_HDR_LEN;
          is_pkt_bcast = TRUE;

          HDR_MSG_PROT( MSG_LEGACY_LOW,  "CCMAC: rcvd brdcst pkt" );
        }

        /*-------------------------------------------------------------------
         Mobile-directed ATI.  Extract the complete Unicast packet header and 
         then determine if the packet is directed to this mobile.  If so,
         adjust the Security Layer packet header length to exclude the packet
         header bytes and advance the Control Channel packet byte offset past
         the Security Layer packet header.  Otherwise, just advance the
         Control Channel packet byte offset to the first byte following this
         Security Layer packet and return to the top of the Control Channel
         packet parsing loop.
        -------------------------------------------------------------------*/
        else 
        {
          is_pkt_bcast = FALSE;
          (void) dsm_extract( cc_pkt_ptr, cc_pkt_offset, sl_pkt_hdr, 
                       HDRCMAC_SL_PKT_UATI_HDR_LEN );
          
          received_ati.ati_type = (hdrhai_ati_enum_type) 
            HDRCMAC_GET_PKT_ATI_TYPE( sl_pkt_hdr );

          received_ati.ati_value = HDRCMAC_GET_PKT_ATI_VAL( sl_pkt_hdr );
          if (
#ifdef FEATURE_HDR_REVC       
           pri_cc &&
#endif /* FEATURE_HDR_REVC */
              hdrampati_match_receive_ati_address (&received_ati, 
                                                HDRHAI_CC_MAC_PROTOCOL))
          {
            /* Make sure SL length is valid */
            if ( sl_pkt_len <= 
                ( HDRCMAC_SL_PKT_UATI_HDR_LEN - HDRCMAC_SL_PKT_LEN_FIELD_LEN ) )
            {
              ERR("Bad SL packet length (%d). Dumping pkt...",sl_pkt_len,0,0);
              break;
            }

            sl_pkt_len    -= HDRCMAC_SL_PKT_UATI_HDR_LEN -
                             HDRCMAC_SL_PKT_LEN_FIELD_LEN;

            cc_pkt_offset += HDRCMAC_SL_PKT_UATI_HDR_LEN;

#ifdef FEATURE_FACTORY_TESTMODE
            /* Indication is not needed in FTM mode                     */
            if(ftm_get_mode() != FTM_MODE)
#endif
              hdrind_give_ind( HDRIND_CCMAC_MOBILE_DIRECTED_PKT, NULL );

            HDR_MSG_PROT( MSG_LEGACY_MED,  "CCMAC: rcvd directed pkt" );
          }
          else
          {
            cc_pkt_offset += HDRCMAC_SL_PKT_LEN_FIELD_LEN + sl_pkt_len;
            continue;
          }
        }

        /* To simplify the implementati?on, instead of checking message 
           received in PCP layer, we check HDRMC event queue in CMAC 
           when receiving Security Layer packet. Security Layer packet
           may still contain lot of messages and cause many events 
           (greater than ?HDRMC_EVENT_?BUFS_NUM - threshold) queued to 
           HDRMC. We try to avoid HDRMC event queue crash, however 
           it's only best effort. */

        if (hdrmc_event_q_over_threshold(HDRHAI_CC_MAC_PROTOCOL))
        {
          HDR_MSG_PROT_2(MSG_LEGACY_ERROR, 
                         "HDRMC Q over threshold, skip CC pkt %x SL pkt# %d",
                         cc_pkt_ptr, sl_no);

          cc_pkt_offset += sl_pkt_len;
          sl_no++;
          continue;
        }
        /*-------------------------------------------------------------------
         Forward the Security Layer packet.  Allocate a duplicate dsm item  
         for the Security Layer packet, copy the dsm item header application
         specific header data to the header of the duplicate dsm item, 
         forward the duplicate dsm item for processing, and then adjust the 
         Control Channel packet byte offset to the first byte following this 
         Security Layer packet.
        -------------------------------------------------------------------*/
        if (dsm_dup_packet( &sl_pkt_ptr, cc_pkt_ptr,
                            cc_pkt_offset, sl_pkt_len ) == 0)
        {
          ERR("Bad input to DSM dup. Dumping packet...",0,0,0);
          break;
        }

        /* Check to ensure that sl_pkt_ptr will not be dereferenced
           when it is NULL */
        if ( sl_pkt_ptr == NULL )
        {
          ERR_FATAL("No more duplicate dsm items available!",0,0,0);
        }

        HDRDSM_SET_TRACER(sl_pkt_ptr, HDRDSM_RX_CMAC);

        if (is_pkt_bcast)
        {
          HDRDSM_SET_BCAST_BIT( sl_pkt_ptr );
        }

#ifdef FEATURE_HDR_REVC
        /* to be removed after IOT */
        if (!pri_cc)
        {
          HDR_MSG_PROT_2(MSG_LEGACY_MED, "non-pri CC pkt 0x%x @ %d",
                         sl_pkt_ptr, cc_pkt_offset);
        }
#endif /* FEATURE_HDR_REVC */       
        hdrpcp_put_pkt( sl_pkt_ptr, is_size_inc );  
          
        cc_pkt_offset += sl_pkt_len;

      } /* if ( sl_pkt_len == 0 ) */
      sl_no++;
    } /* while ( cc_pkt_offset < CONTROL_PKT_BYTES ) */

    if (send_scc_pkt_rxed_ind)
    {
      if (hdrmc_event_q_over_threshold(HDRHAI_CC_MAC_PROTOCOL))
      {
        HDR_MSG_PROT(MSG_LEGACY_ERROR, 
                     "HDRMC Q over threshold, drop SCC_PKT_RXED");
      }
      else
      {
        /* Send SCC pkt received indication */
#ifdef FEATURE_MEMCPY_REMOVAL
        ind_data.intra_scc_info = hdrcmac.intra_scc_info;
#else
        memcpy(&ind_data.intra_scc_info, &hdrcmac.intra_scc_info, 
               sizeof(hdrcmac.intra_scc_info));
#endif /* FEATURE_MEMCPY_REMOVAL */
        hdrind_give_ind( HDRIND_CCMAC_SCC_PKT_RXED, 
          (void *) (&ind_data) );
      }
    }

#ifdef FEATURE_HDR_REVC
    if (pri_cc)
    {
#endif /* FEATURE_HDR_REVC */
    /* Log CC packet */
    if (log_status(LOG_HDR_CC_MAC_PACKET_C))
    {   

      /* Pointer to memory allocated for log */
      LOG_HDR_CC_MAC_PACKET_C_type *log_ptr;
     
      /* Logged packet length. Log entire decoder packet for now */
      uint16 length;

      length = MIN(cc_pkt_offset, cmac_pkt_size_bytes);
    
      /* Get log record */
      log_ptr = (LOG_HDR_CC_MAC_PACKET_C_type *)
              log_alloc(LOG_HDR_CC_MAC_PACKET_C,
              sizeof(LOG_HDR_CC_MAC_PACKET_C_type)+length-1);

      if (log_ptr != NULL)
      {

        log_ptr->pkt_length = length;

        /* Fill in and send the log record. */
        /* Disables the "possible data overrun" * error #669. The data field 
           of log_ptr is an array of size one and we are copying 'length' bytes 
           starting from the beginning of the array. Hence lint complains. */
        /*lint -save -e669 */
#ifdef FEATURE_MEMCPY_REMOVAL
        memscpy((void*)&(log_ptr->data[0]),
                 length, 
                (void*)(cc_pkt_ptr->data_ptr),
                 length);
#else
        memcpy((void*)&(log_ptr->data[0]),
               (void*)(cc_pkt_ptr->data_ptr),
               length);
#endif /* FEATURE_MEMCPY_REMOVAL */
        /*lint -restore */

        log_commit((log_type*)log_ptr);
      }                      
    }
#ifdef FEATURE_HDR_REVC   
    } /* if primary CC */
#endif /* FEATURE_HDR_REVC */   
  } /* if ( hdrcmac_state == HDRCMAC_ACTIVE_STATE ) */

  /*-------------------------------------------------------------------------
   Free the original Control Channel packet and exit. 
  -------------------------------------------------------------------------*/
  dsm_free_packet( &cc_pkt_ptr );

} /* hdrcmac_put_pkt() */

/*============================================================================
  HDRCMAC_SYNC_CC_IS_MISSING
*/
/**
  Determines whether a SyncCC packet (part of a SyncCC capsule) is missing.
  This is done by checking cc_start and the last_packet_flag.
  Must only be called when DSP detects that the AT did not see preamble for a 
  SyncCC packet when it should have detected one, or when AT did not see 
  preamble for subSync packet when it could have detected one.

  @dependencies Should only be called after DSP detects a missed preamble.

  @return TRUE if synchronous control channel packet preamble was missed and 
  FALSE if a sub-synchronous control channel packet preamble was missed.

  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
boolean hdrcmac_sync_cc_is_missing
(
  uint16  *packet_num
    /**< Determined packet number of packet missed if continued packet. 
         (Packet_num n means nth packet of sync capsule missed, starting 
         from 1.) */
)  
{
  uint16  current_hstr;
    /* The HSTR now, assumed to be close to HSTR when missed preamble 
       interrupt fired. Informational purposes only. */

  uint16  current_cc_start_slot;
    /*  The start slot. This is where a CC packet was expected (excluding 
        CC offset slots). */

  uint16 scc_packet_duration;
    /*  Duration of the Sync Capsule packets at current rate. */

  boolean is_sync_cc;
    /* Whether it was a SyncCC packet that was missed (TRUE) or 
       a missed/nonexistent subSyncCC (FALSE) */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get current HSTR to get an idea of when interrupt was triggered. */
  current_hstr = (HDRHSTR_GET_COUNT()) % 512;

  /* Get the current CC starting slots      */
  current_cc_start_slot = hdrsrch_get_cc_start();

  /* If waiting for a continued packet use cc_start value to find out which
     packet AT is waiting on. */
  if (hdrcmac.waiting_for_last_pkt)
  {
    
    scc_packet_duration = HDRCMAC_SCC_PACKET_DURATION( hdrcmac.scc_rate );
    
    *packet_num = (current_cc_start_slot / scc_packet_duration) + 1;

    /* Only used the calculated packet num if the previous packet num 
       was received */
    if (((*packet_num) - hdrcmac.last_packet_num_received) != 1)
    {
      is_sync_cc = FALSE;
      *packet_num = 1;
    }
    else
    {
      is_sync_cc = TRUE;
    }

    /* We didn't detect preamble for the packet we were waiting for.
       Mark as no longer waiting. */
    hdrcmac.waiting_for_last_pkt = FALSE;
    hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;

  }
  else
  {
    /* Otherwise must be subsync or first sync. */
    if (current_cc_start_slot == 0)
    {
      is_sync_cc = TRUE;   
    }
    else
    {
      is_sync_cc = FALSE;
    }

    /* Whether this is subsync or sync, it is the first packet. */
    *packet_num = 1;

  }

  HDR_MSG_PROT_4( MSG_LEGACY_HIGH, 
    "No SyncCC (missed preamble; HSTR=%d, ccStart=%d, pkt_num=%d, sync=%d)", 
    current_hstr, current_cc_start_slot, *packet_num, is_sync_cc);

  return is_sync_cc;

} /* hdrcmac_sync_cc_is_missing() */

#ifdef FEATURE_HDR_REVA_L1

/*============================================================================
  HDRCMAC_PROCESS_IND
*/
/**
  This function processes the indications given to CMAC.

  @context HDRRX task. This routine is NOT reentrant. */
/*============================================================================*/
void hdrcmac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
    /**< Name of the indication for CMAC to process. */
  hdrind_ind_data_union_type *ind_data_ptr
    /**< pointer to data associated with the indication. */
)
{

  hdrfw_msg_u fw_cmd;
    /* Firmware command for sync CC message */

  errno_enum_type estatus;
    /* Error status */

  boolean send_message = FALSE;
    /* Is it necessary to send the message? */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRTRACE_IND(HDRHAI_CC_MAC_PROTOCOL, ind_name, hdrcmacsm_get_state() );

#ifdef FEATURE_LTE_TO_HDR_OH
  if (hdrsap_tunnel_mode_is_enabled(HDRHAI_CC_MAC_PROTOCOL) &&
      (ind_name != HDRIND_SCM_SESSION_CHANGED) &&
      (ind_name != HDRIND_LMAC_TIME_SYNC_DONE) )
  {
    HDRTRACE_IND_IGNORED_STATE( HDRHAI_CC_MAC_PROTOCOL, ind_name, 
                                hdrcmacsm_get_state() );
    return;
  }
#endif /* FEATURE_LTE_TO_HDR_OH */

  /* The hdrcmac_ind_cb queues indications only when the negotiated subtype
     of CMAC is Enhanced. So it is not necessary to check if the subtype
     is enhanced again. Proceed to processing indications assuming that 
     the subtype is always Enhanced when we get here! */
  switch (ind_name)
  {

    case HDRIND_SCM_SESSION_CHANGED:

      hdrcmac.current_subtype = 
        hdrscp_get_current_subtype( HDRHAI_CC_MAC_PROTOCOL );

      /* Get the current value and the version number for each attribute */
      hdrscmdb_get_inuse_config( 
        HDRHAI_CC_MAC_PROTOCOL,
        &hdrcmac.cfg);

      /* Fall through intentional. 
         Enable/Disable short packets based on the current session */

    /*------------------------------------------------------------------------
      The AT need not look for preambles of short control channel packets in 
      connected state. CMAC registers to receive two indications to learn about
      when a new connection is opened and when it is closed. The decoding of 
      short control channel packets are disable when 
      HDRIND_IDLE_CONNECTION_OPENED indication is received and enabled back 
      again when HDRIND_CONNECTED_CONNECTION_CLOSED indication is received. 
    -------------------------------------------------------------------------*/

    /* Connection closed, fail, abort, or wake up.
       Enable CC3/4 short packets if needed. */      
    case HDRIND_CONNECTED_CONNECTION_CLOSED:
    case HDRIND_IDLE_CONNECTION_ABORTED:
    case HDRIND_IDLE_CONNECTION_FAILED:
    case HDRIND_LMAC_AWAKE:

      /* If CMAC is active, update config. */
      if (hdrcmac_is_active(HDRHAI_CC_MAC_PROTOCOL) == TRUE )
      {

        if( hdrmdsp_check_dsp_clk() )
        {
          msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_FW, HDR_FW_CC_DEMOD_CFG_CMD );
            /* Initialize firmware command for CC demod message */
  
          if ( hdrcmac.current_subtype == HDRSCP_ENH_CCMAC )
          {
            /* Re-configure short packets on wakeup or connection close. */
            HDR_MSG_PROT_1(MSG_LEGACY_HIGH, 
                          "CC1: Rev A; Enabling CC short packets (mac 71, %d)",
                          hdrcmac.cfg.sht_pkt_mac_index);
  
            /* Make sure short packets and Rev A are enabled. */
            fw_cmd.cc_demod_cfg.hdrSystemModeConfig = HDRCMAC_SYSTEM_MODE_REV_A;
            fw_cmd.cc_demod_cfg.hdrSystemModeConfig_Valid = 1;
            fw_cmd.cc_demod_cfg.hdrShortPacketMode = HDRCMAC_SHORT_PKT_ENABLED;
            fw_cmd.cc_demod_cfg.hdrShortPacketMode_Valid = 1;
  
            /* Enable CC short packet in DSP */
            fw_cmd.cc_demod_cfg.ctrl3MACIndex = HDRCMAC_CTRL3_MAC_INDEX;
            HDR_MSG_PROT_1(MSG_LEGACY_MED,
              "Enabling short pkts: ctrl3MACIndex = %d",
              fw_cmd.cc_demod_cfg.ctrl3MACIndex);

            fw_cmd.cc_demod_cfg.ctrl4MACIndex = hdrcmac.cfg.sht_pkt_mac_index;
  
          }
          else
          {
            HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                          "CC0: Rev 0; Disabling CC short packets");
  
            /* Disable short packets and set system mode to rel 0 */
            fw_cmd.cc_demod_cfg.hdrSystemModeConfig = HDRCMAC_SYSTEM_MODE_REV_0;
            fw_cmd.cc_demod_cfg.hdrSystemModeConfig_Valid = 1;
            fw_cmd.cc_demod_cfg.hdrShortPacketMode = HDRCMAC_SHORT_PKT_DISABLED;
            fw_cmd.cc_demod_cfg.hdrShortPacketMode_Valid = 1;
  
            /* Disable CC short packets. */
            fw_cmd.cc_demod_cfg.ctrl3MACIndex = 0x0000;
            fw_cmd.cc_demod_cfg.ctrl4MACIndex = 0x0000;
          }
  
          send_message = TRUE;
        }
        else
        {
          HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                         "CC%d: Unable to configure CC short pkts.",
                         hdrcmac.current_subtype);
        }
      }

      break;

    /* Connection opened. Disable CC3/4 short packets */
    case HDRIND_IDLE_CONNECTION_OPENED:

      if( hdrmdsp_check_dsp_clk() )
      {
        /* Disable short packets on connection open */
        HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                        "CC%d: Disabling CC short packets",
                        hdrcmac.current_subtype);
  
        msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_FW, HDR_FW_CC_DEMOD_CFG_CMD );
          /* Initialize firmware command for CC demod message */
        
        /* Don't override system mode or short packets. FTC MAC will set
           these accordingly for the connection. */
        fw_cmd.cc_demod_cfg.hdrSystemModeConfig_Valid = 0;
        fw_cmd.cc_demod_cfg.hdrShortPacketMode_Valid = 0;
  
        /* Disable CC short packets by clearing the MAC indices. */      
        fw_cmd.cc_demod_cfg.ctrl3MACIndex = 0x0;
        fw_cmd.cc_demod_cfg.ctrl4MACIndex = 0x0;
  
        send_message = TRUE;
      }
      else
      {
        HDR_MSG_PROT_1(MSG_LEGACY_ERROR,
                       "CC%d: Unable to configure CC short pkts.",
                       hdrcmac.current_subtype);
      }

      break;
  
     case HDRIND_LMAC_TIME_SYNC_DONE:
       hdrcmac.block_missed_scc_flag = TRUE;

       HDR_MSG_PROT(MSG_LEGACY_HIGH, "Blocking next missed preamble flag");
       break;

    default:
      ERR("Invalid indication.", 0, 0, 0);

  } /* switch */

  if (send_message)
  {
    estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );

    ASSERT ( estatus == E_SUCCESS );
  }

} /* hdrcmac_process_ind */

#endif /* FEATURE_HDR_REVA_L1 */

/*============================================================================
  HDRCMAC_START_TIMER
*/
/**
  This function starts or resumes the CMAC supervision timer

  @context Calling task (HDRRX, HDRMC). This routine is NOT? reentrant. */
/*============================================================================*/
void hdrcmac_start_timer( void )
{

  /* If the protocols were suspended, CMAC deactivated but saved how much time
     was left on its supervision timer.  When reactivating, CMAC will now
     resume the supervision timer rather than restarting it for its full 
     duration (5.12s).  This allows the CMAC supervision timer to still expire
     if in weak HDR coverage, but the AT keeps suspending but doing short 
     tune aways to 1X.
   */
  if ( hdrcmac.saved_superv_timer_ms != 0 )
  {
    (void) rex_set_timer( &hdrcmac.sup_timer, hdrcmac.saved_superv_timer_ms );
  }
  else
  {
    (void) rex_set_timer( &hdrcmac.sup_timer, HDRCMAC_SUPV_TIMER_MSECS );
  }

  /* Reset the saved supervision time */
  hdrcmac.saved_superv_timer_ms = 0;

} /* hdrcmac_start_timer() */

/*============================================================================
  HDRCMAC_STOP_TIMER
*/
/**
  This function stops the CMAC supervision timer

  @context Calling task (HDRRX, HDRMC). This routine is NOT? reentrant. */
/*============================================================================*/
void hdrcmac_stop_timer( void )
{

  (void) rex_clr_timer( &hdrcmac.sup_timer );

} /* hdrcmac_stop_timer() */

/*============================================================================
  HDRCMAC_UPDATE_LINK
*/
/**
  This function updates the link id indicating the serving sector. This
  is the sector on which we are monitoring the control channel.

  @context Calling task. This routine is reentrant. */
/*============================================================================*/
void hdrcmac_update_link
(
  uint8 link_id
    /**< link id of serving sector. */
)
{
  /* Note that ultimately this should be a command. */
  REX_DISABLE_PREMPTION();

  hdrcmac.serving_link_id = link_id;

  hdrcmac.scc_offset_slots = HDRRX_SCC_OFFSET_UNKNOWN;

  /* Tell hdrrx that link has changed to unknown. */
  hdrrx_set_cc_offset( HDRRX_SCC_OFFSET_UNKNOWN );

  REX_ENABLE_PREMPTION();

  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "CC MAC link updated: %d", link_id);

} /* hdrcmac_update_link() */

/*============================================================================

                       EXTERNAL FUNCTION DEFINITIONS

              Exported through hdrcmac.h (Scope restricted to HDR)
=============================================================================*/

/*============================================================================
  HDRCMAC_POWERUP_INIT
*/
/**
  This function initializes the protocol prior to use.  This function should 
  be called upon entering the HDR mode of operation.

  @sideeffects Supervision timer is defined and timestamp status is set to 
  inactive.

  @context Calling task. This routine is reentrant */
/*============================================================================*/
void hdrcmac_powerup_init ( void )
{

#ifdef FEATURE_HDR_DLNA
  uint64 debug_mask;           
    /* Temp variable MAC debug mask from NV */
#endif /* FEATURE_HDR_DLNA*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( hdrmc_is_first_init() )
  {
    /* Define the CMAC Supervision timer */
    rex_def_timer_ex(&hdrcmac.sup_timer,
                     hdrcmac_timer_cb, 
                     HDRCMAC_SUPERVISION_TIMER);

    /* Initialize the timestamp critical section variable */
    rex_init_crit_sect(&hdrcmac.timestamp_crit_sect);
  }

  hdrcmac.saved_superv_timer_ms = 0;

  hdrcmac_timestamp.status = HDRCMAC_TIMESTAMP_INVALID;

#ifdef FEATURE_HDR_REVA_L1

  hdrcmac.current_subtype = hdrscp_get_current_subtype(HDRHAI_CC_MAC_PROTOCOL);

#endif /* FEATURE_HDR_REVA_L1 */

  hdrcmac.prev_pkt_link_id = HDRCMAC_INVALID_LINK_ID;
  hdrcmac.serving_link_id = HDRCMAC_INVALID_LINK_ID;
  hdrcmac.waiting_for_last_pkt = FALSE;
  hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;
  hdrcmac.scc_offset_slots = HDRRX_SCC_OFFSET_UNKNOWN;
  hdrcmac.process_missed_scc_flag = FALSE;
  hdrcmac.missed_scc_flag_rcvd = FALSE;
  hdrcmac.block_missed_scc_flag = FALSE;

  /* Get the current value and the version number for each attribute */
  hdrscmdb_get_inuse_config( 
    HDRHAI_CC_MAC_PROTOCOL,
    &hdrcmac.cfg);

#ifdef FEATURE_HDR_DLNA
  /* Initialize DLNA disble request mask */
  hdrcmac.dlna_disable_requests = 0;

  /* Check if D-LNA is enabled via NV.
     Bit HDRLMAC_USER_MAC_DEBUG_DISABLE_ENH_DLNA == 0 if both Enhanced D-LNA and D-LNA are enabled.
     Bit HDRLMAC_USER_MAC_DEBUG_DISABLE_DLNA == 0 if D-LNA is enabled.
     FW only supports enabling Enhanced D-LNA if D-LNA is also being enabled.
     So if D-LNA is being disabled, both Enhanced D-LNA and standard D-LNA will be disabled.
  */
    hdrsrchutil_set_dlna_enable( TRUE );
  hdrsrchutil_set_enh_dlna_enable( TRUE );

  if ( hdrlmac_get_user_mac_debug_mask( &debug_mask ) == TRUE )
  {
    if ( ( debug_mask & HDRLMAC_USER_MAC_DEBUG_DISABLE_DLNA ) ==
               HDRLMAC_USER_MAC_DEBUG_DISABLE_DLNA ) 
    {
      hdrsrchutil_set_dlna_enable( FALSE );
       
    }
    if ( ( debug_mask & HDRLMAC_USER_MAC_DEBUG_DISABLE_ENH_DLNA ) == 
         HDRLMAC_USER_MAC_DEBUG_DISABLE_ENH_DLNA )
    {
      /* Set Enhanced D-LNA enable flag. */
      hdrsrchutil_set_enh_dlna_enable( FALSE );
    }
  }
#endif /* FEATURE_HDR_DLNA */

  HDRTRACE_INIT_DEFAULT(HDRHAI_CC_MAC_PROTOCOL);

} /* hdrcmac_powerup_init */

/*============================================================================
  HDRCMAC_ACTIVATE
*/
/**
  This function transitions the protocol to the Active state and installs the 
  Control Channel supervision timer.  If the protocol is already in the 
  Active state then no action is taken.

  @dependencies The protocol must be initialized by calling the hdrcmac_init 
  function prior to activation.

  @context CMAC activation will always execute in HDRRX task context by queuing
  a command to HDRRX task even when called from a different task context. */
/*============================================================================*/
void hdrcmac_activate 
( 
  hdrhai_protocol_name_enum_type sender 
    /**< The protocol giving this command (only for software trace) */
)
{
  hdrcmac_cmd_union_type cmd_buf;              
    /* Temporary command buffer */

  hdrfw_msg_u fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */

  int i;
    /* Loop index */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRCMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRCMAC_ACTIVATE_CMD;
    cmd_buf.hdr.sender = sender;
    hdrrx_queue_cmd( HDRHAI_CC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    HDRTRACE_CMD( HDRHAI_CC_MAC_PROTOCOL, HDRCMAC_ACTIVATE_CMD, 
      sender, hdrcmacsm_get_state() );

#ifdef FEATURE_HDR_REVA_L1
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "hdrcmac activate: subtype %d", 
                  hdrcmac.current_subtype);
#endif /* FEATURE_HDR_REVA_L1 */

    if ( hdrcmac_is_active(HDRHAI_CC_MAC_PROTOCOL) == FALSE ) 
    {

      msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_FW, HDR_FW_CC_DEMOD_CFG_CMD );
        /* Initialze firmware command for CC Demod configuration */

      hdrcmacsm_change_state( HDRCMACSM_ACTIVE_STATE );

      /* Initialize timestamp info buffer */
      hdrcmac_timestamp.status = HDRCMAC_TIMESTAMP_INVALID;
      hdrcmac_timestamp.idx = 0;
      for (i = 0; i < HDRCMAC_TIMESTAMP_BUFFER_COUNT; i++)
      {
        hdrcmac_timestamp.info[i].valid = FALSE;
      }

#ifdef FEATURE_HDR_REVA_L1
      
      /* Configure CC short packets.
         It is not necessarily an error if the update does
         not succeed (due to mdsp clocks off), as config will 
         be refreshed on receiving the LMAC_AWAKE indication. */  
      if( hdrmdsp_check_dsp_clk() )
      {
        if ( hdrcmac.current_subtype == HDRSCP_ENH_CCMAC )
        {
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                          "CC1: Rev A; Enabling CC short packets (mac 71, %d)",
                          hdrcmac.cfg.sht_pkt_mac_index);
  
          /* Make sure short packets and Rev A are enabled. */
          fw_cmd.cc_demod_cfg.hdrSystemModeConfig = HDRCMAC_SYSTEM_MODE_REV_A;
          fw_cmd.cc_demod_cfg.hdrSystemModeConfig_Valid = 1;
          fw_cmd.cc_demod_cfg.hdrShortPacketMode = HDRCMAC_SHORT_PKT_ENABLED;
          fw_cmd.cc_demod_cfg.hdrShortPacketMode_Valid = 1;
  
          /* Enable CC short packet in DSP */
          fw_cmd.cc_demod_cfg.ctrl3MACIndex = HDRCMAC_CTRL3_MAC_INDEX;
          fw_cmd.cc_demod_cfg.ctrl4MACIndex = hdrcmac.cfg.sht_pkt_mac_index;
        }
        else
        {
          HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                        "CC0: Rev 0; CC short packets disabled" );
  
          /* Disable short packets and set system mode to rel 0 */
          fw_cmd.cc_demod_cfg.hdrSystemModeConfig = HDRCMAC_SYSTEM_MODE_REV_0;
          fw_cmd.cc_demod_cfg.hdrSystemModeConfig_Valid = 1;
          fw_cmd.cc_demod_cfg.hdrShortPacketMode = HDRCMAC_SHORT_PKT_DISABLED;
          fw_cmd.cc_demod_cfg.hdrShortPacketMode_Valid = 1;
  
          /* Disable CC short packets. */
          fw_cmd.cc_demod_cfg.ctrl3MACIndex = 0x0000;
          fw_cmd.cc_demod_cfg.ctrl4MACIndex = 0x0000;
        }
  
        estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
  
        ASSERT ( estatus == E_SUCCESS );
  
  #endif /* FEATURE_HDR_REVA_L1 */
  
        /* -- Enable Missed Preamble Indication from FW -- */
  
        msgr_init_hdr ( 
          &fw_cmd.hdr, MSGR_HDR_RX, HDR_FW_INDICATION_CFG_CMD );
  
        fw_cmd.indication_cfg.type = HDR_FW_IND__SCC_PREAM;
        fw_cmd.indication_cfg.enable = 1;
  
        estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
          /* Send the message */
  
        ASSERT ( estatus == E_SUCCESS );
      }
    }
    else
    {
      HDRTRACE_CMD_IGNORED_STATE( HDRHAI_CC_MAC_PROTOCOL, 
        HDRCMAC_ACTIVATE_CMD, sender, hdrcmacsm_get_state() );
    }
  }

} /* hdrcmac_activate */

/*============================================================================
  HDRCMAC_SUSPEND_AND_DEACTIVATE
*/
/**
  This function transitions the protocol to the Inactive state and suspends  
  the Control Channel supervision timer.  If the protocol is already in the
  Inactive state then no action is taken.

  @context HDRMC task. */
/*============================================================================*/
void hdrcmac_suspend_and_deactivate 
( 
  hdrhai_protocol_name_enum_type sender 
    /**< The protocol giving this command (only for software trace) */
)
{ 

  HDR_MSG_PROT( MSG_LEGACY_LOW, "hdrcmac_suspend_and_deactivate" );

  if ( hdrcmac_is_active(HDRHAI_CC_MAC_PROTOCOL) == TRUE ) 
  {
    hdrcmac.saved_superv_timer_ms = rex_get_timer(&hdrcmac.sup_timer);
  }

  hdrcmac_deactivate(sender);

} /* hdrcmac_suspend_and_deactivate */

/*============================================================================
  HDRCMAC_DEACTIVATE
*/
/**
  This function transitions the protocol to the Inactive state and clears the 
  Control Channel supervision timer.  If the protocol is already in the
  Inactive state then no action is taken.

  @context CMAC deactivation will always execute in HDRRX task context by 
  queuing a command to HDRRX task even when called from a different task 
  context. */
/*============================================================================*/
void hdrcmac_deactivate 
( 
  hdrhai_protocol_name_enum_type sender 
    /**< The protocol giving this command (only for software trace) */
)
{ 
  hdrcmac_cmd_union_type cmd_buf;              
    /* Temporary command buffer */

  errno_enum_type estatus;
    /* Error status */

  hdrfw_msg_u fw_cmd;
    /* Firmware command */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRCMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRCMAC_DEACTIVATE_CMD;
    cmd_buf.hdr.sender = sender;
    hdrrx_queue_cmd( HDRHAI_CC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {

    HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrcmac deactivate");

    HDRTRACE_CMD( HDRHAI_CC_MAC_PROTOCOL, 
      HDRCMAC_DEACTIVATE_CMD, sender, hdrcmacsm_get_state() );

    if ( hdrcmac_is_active(HDRHAI_CC_MAC_PROTOCOL) == TRUE ) 
    {
      if( hdrmdsp_check_dsp_clk( ) )
      {
        /* -- Disable Missed Preamble Indication from FW -- */
        msgr_init_hdr ( 
          &fw_cmd.hdr, MSGR_HDR_RX, HDR_FW_INDICATION_CFG_CMD );

        fw_cmd.indication_cfg.type = HDR_FW_IND__SCC_PREAM;
        fw_cmd.indication_cfg.enable = 0;

        estatus = msgr_send ( &fw_cmd.hdr, sizeof ( hdrfw_msg_u ) );
          /* Send the message */

        ASSERT ( estatus == E_SUCCESS );
      }
      else
      {
        HDR_MSG_PROT( MSG_LEGACY_HIGH,
                      "FW is in standby and scc pream ind cfg is not sent" );
      }

      /* Reset the flag */
      hdrcmac.waiting_for_last_pkt = FALSE;
      hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;
      
      hdrind_give_ind( HDRIND_CCMAC_INTERRUPTED, NULL );
      hdrcmacsm_change_state( HDRCMACSM_INACTIVE_STATE );
    }
    else
    {
      /* CMAC could already be inactive because the AT went into suspend 
         state.  If HDR is deactivated before the AT resumes, then clear
         the saved timer. */
      if ( hdrcmac.saved_superv_timer_ms != 0 )
      {
        hdrcmac.saved_superv_timer_ms = 0;
      }

      HDRTRACE_CMD_IGNORED_STATE( HDRHAI_CC_MAC_PROTOCOL, 
        HDRCMAC_DEACTIVATE_CMD, sender, hdrcmacsm_get_state() );
    }
  }

} /* hdrcmac_deactivate */

/*============================================================================
  HDRCMAC_GET_SYNC_CAPS_TIMESTAMP
*/
/**
  This function provides the current value of the Synchronous Control Channel
  Capsule timestamp.  The return value indicates the availability of a valid 
  timestamp value.

  @return E_SUCCESS if timestamp is successfully copied to destination 
  address and E_NOT_AVAILABLE if timestamp value was not available.

  @sideeffects If available, timestamp is copied to client specified 
  destination address

  @context Calling task (HDRMC). This routine is reentrant */
/*============================================================================*/
hdrerrno_enum_type hdrcmac_get_sync_caps_timestamp 
( 
  hdrdec_half_slot_time_type msg_hstr,
    /**< HSTR of message  */  
  hdrdec_half_slot_time_type *timestamp_ptr
    /**< Destination address for timestamp */
)
{
  /* Current HSTR */
  uint16                               hstr16_now;

  /* Least Significant 9 bits of HSTR of the CMAC pkt that contained the 
     SYNC message */
  uint16                               hstr9_pkt;

  /* 16 bit HSTR of CMAC pkt that contained the SYNC message */
  uint16                               hstr16_pkt;

  /* Whether value in hstr timestamp buffer is valid */
  boolean                              hstr_valid;

  /* HSTR value of first sync capsule packet */
  uint16                               hstr_value;

  /* Loop index and counter */
  int index, i;   

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   If the timestamp value is valid, copy it to the destination address.  The
   value of the current timestamp can be read using a single halfword load 
   instruction so it is not necessary to lock interrupts to ensure that this
   operation is atomic.
  -------------------------------------------------------------------------*/
  if ( hdrcmac_timestamp.status == HDRCMAC_TIMESTAMP_VALID )
  {

    /*---------------------------------------------------------------------
     CMAC remembers only the least significant 9 bits from the SYNC packet 
     HSTR. However the Most significant 7 bits could be easily retrieved
     using the current HSTR count. These seven bits should be the same for 
     both the HSTR counts(pkt_hstr and current_hstr), unless the bit 10 
     rolled over. Getting hstr_offset from current HSTR count and the 9 LS 
     bits of packet HSTR from DSM item.
    ---------------------------------------------------------------------*/
    hstr9_pkt = msg_hstr;

    /* Get the current HSTR Count */
    hstr16_now = HDRHSTR_GET_COUNT();

    /* Strip the Most significant seven bits from current HSTR assuming that
       bit 10 hasn't rolled over */
    hstr16_pkt = hstr16_now & 0xFE00;

    /* Check if the bit 10 has rolled over */
    if (hstr9_pkt > (hstr16_now & 0x1FF))
    {
      /* Subtract to adjust for roll-over */
      hstr16_pkt -= 0x200;
    }

    /* Construct 16 bit packet HSTR using the LS 9 bits stored in dsm */
    msg_hstr = hstr16_pkt | hstr9_pkt;

    /* Entering critical section... */
    rex_enter_crit_sect(&hdrcmac.timestamp_crit_sect);

    /* Initialize index and hstr */
    index = hdrcmac_timestamp.idx;
    hstr_valid = hdrcmac_timestamp.info[index].valid;
    hstr_value = hdrcmac_timestamp.info[index].value;

    /* Leave critical section */
    rex_leave_crit_sect(&hdrcmac.timestamp_crit_sect);

    /* Loop through the buffers */
    for (i = 0; i < HDRCMAC_TIMESTAMP_BUFFER_COUNT; i++)
    {

      /*-------------------------------------------------------------------------
       If the timestamp value is valid, copy it to the destination address.  The
       value of the current timestamp can be read using a single halfword load 
       instruction so it is not necessary to lock interrupts to ensure that this
       operation is atomic.
      -------------------------------------------------------------------------*/
      if ( hstr_valid == TRUE )
      {
        /* Check that the hstr_value (the hstr of the first packet of the sync
           capsule) is actually in the same sync capsule identified by msg_hstr
           (typically the hstr of the packet of the sync capsule that the sync
           message arrived on).
         
           To be in the same capsule, msg_hstr must have arrived within 512
           half-slots of the hstr_value.      
        */
        if ((((msg_hstr + 0x10000) - hstr_value) & 0xFFFF) < 512)
        {
          *timestamp_ptr = hstr_value;

          return ( E_SUCCESS );
        }
      }

      /* Update index and hstr from next buffer */
      index++;
      index %= HDRCMAC_TIMESTAMP_BUFFER_COUNT;

      /* Entering critical section... */
      rex_enter_crit_sect(&hdrcmac.timestamp_crit_sect);

      hstr_valid = hdrcmac_timestamp.info[index].valid;
      hstr_value = hdrcmac_timestamp.info[index].value;

      /* Leave critical section */
      rex_leave_crit_sect(&hdrcmac.timestamp_crit_sect);

    }
  }

  return ( E_NOT_AVAILABLE );

} /* hdrcmac_get_sync_caps_timestamp */

/*============================================================================
  HDRCMAC_GET_SCC_RATE
*/
/**
  This function returns the last saved synchronous control channel rate.

  @return control channel rate.

  @context Calling task (HDRSRCH). This routine is reentrant */
/*============================================================================*/
uint8 hdrcmac_get_scc_rate( void )
{

  return hdrcmac.scc_rate;

} /* hdrcmac_get_scc_rate */

/*============================================================================
  HDRCMAC_IND_CB
*/
/**
  This function processes incoming indications for this protocol.

  @context Calling task. This routine is reentrant. */
/*============================================================================*/
void hdrcmac_ind_cb
(
  hdrind_ind_name_enum_type ind_name,
    /**< Name of the indication from hdrhai.h */
  void *ind_data
    /**< Pointer to any indication-specific data */
)
{
  
#ifdef FEATURE_HDR_REVA_L1

  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdrcmac_ind_cb %d", ind_name );

  if ( rex_self () == HDRCMAC_TASK_PTR )
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdrcmac_process_ind(ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    hdrrx_queue_ind (HDRHAI_CC_MAC_PROTOCOL, ind_name, ind_data);
  }  

#endif /* FEATURE_HDR_REVA_L1 */

} /* hdrcmac_ind_cb */

/*============================================================================
  HDRCMAC_PROCESS_BAD_CRC
*/
/**
  This function processes the bad CRC for RX and detects if a SYNC packet
  has been lost to give appropriate sleep indication.

  @return TRUE if a synchronous or a sub-synchronous control channel packet
  has been lost and FALSE otherwise.

  @context HDRDEC task. This routine is NOT reentrant. */
/*============================================================================*/
boolean hdrcmac_process_bad_crc
(
  uint16 pkt_slot_time,
    /**< Slot time of the bad packet */
  boolean pkts_pending,
    /**< Do we have any packets in decoB/CBuf waiting to be processed? */
  hdrdec_pkt_type_e_type pkt_type
    /**< Type of bad CRC CC packet. */
)
{
  uint16 cc_start = 0;
    /* Synchronous/Sub-Sync control channel packet start time in slots */

  boolean lost_sync_or_subsync_cc = FALSE;
    /* Did we lose a Sync/Sub-sync CC? */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Find the slot within the CC cycle */
  pkt_slot_time = pkt_slot_time % HDRCMAC_CC_CYCLE_SLOTS;

  /* If we are not connected, */
  if (!hdrfmac_is_active(HDRHAI_CC_MAC_PROTOCOL))
  {
    /* Get CC Start from firmware */
    cc_start = hdrsrch_get_cc_start();

    /* Do we know the SCC offset? */
    if ( hdrcmac.scc_offset_slots != HDRRX_SCC_OFFSET_UNKNOWN)
    {
      /* We know the SCC offset */

      /* Don't treat QCPH as lost sync */
      if (pkt_type == HDRDEC_QPCH_PKT)
      {
        if (cc_start != 0)
        {
          /* We received a bad sub-sync QPCH packet. Enable missed preamble 
             interrupt */
          hdrcmac_enable_missed_scc_detection( HDRHAI_CC_MAC_PROTOCOL, FALSE );

          /* Please note that the above code is not needed when the SCC offset is 
             unknown (else part below). In this case, FPD would not be enabled, 
             since we do not know for sure if we have a packet and hence the 
             missed Preamble detection interrupt would already be enabled. */
        }
      }
      /* otherwise check if packet is an expected sub-synchronous CC (which 
         uses cc_start), or a SyncCC (which is zero-based) */
      else if ((pkt_slot_time == hdrcmac.scc_offset_slots) ||
          ((pkt_slot_time - cc_start ) == hdrcmac.scc_offset_slots))
      {

        lost_sync_or_subsync_cc = TRUE;

        HDR_MSG_PROT_3(
          MSG_LEGACY_HIGH, 
          "StdbyOpt1: STR: %d, cc_start: %d, SCC Offset: %d", 
          pkt_slot_time, cc_start, hdrcmac.scc_offset_slots);
      }

    }
    else
    {
      /* We don't know the SCC offset */

      /* Bad QPCH doesn't count as lost sync. */
      if (pkt_type != HDRDEC_QPCH_PKT)
      {
        /* If we don't have any packets pending in the decoder/CBuf (which could
           potentially be a SYNC CC) yet to be processed, */
        if (!pkts_pending)
        {
          /* Peek into each interlace's decode to check for any Sync/Sub-sync 
             candidate */
          lost_sync_or_subsync_cc = 
            (!hdrcmac_pending_decode_has_sync_candidate(cc_start));
        }
      }
    }
  }

  if (lost_sync_or_subsync_cc == TRUE)
  {
      /* This was the packet we were waiting for; unfortunately we couldn't
         decode it. Reset waiting flag. */
      hdrcmac.waiting_for_last_pkt = FALSE;
      hdrcmac.last_packet_num_received = HDRCMAC_NO_SYNC_IN_PROGRESS;
  }

  return lost_sync_or_subsync_cc;

} /* hdrcmac_process_bad_crc */

/*============================================================================
  HDRCMAC_IS_WAITING_FOR_SYNC_PKT
*/
/**
  Returns whether hdrcmac is waiting for a continued sync packet in the 
  current sync capsule.

  @return TRUE if the synchronous capsule is ongoing and a continuation 
  packet is expected and FALSE otherwise.

  @context NOT USED. @todo Remove? This routine is reentrant. */
/*============================================================================*/
boolean hdrcmac_is_waiting_for_sync_pkt( void )
{
  return hdrcmac.waiting_for_last_pkt;
}

/*============================================================================
  HDRCMAC_GET_CC_PACKET_NUM
*/
/**
  Determined the packet num of a CC packet if it is a continued Sync packet.
  Packet_num for subsync packets is always 1.  Note that this function must
  be called before the CC packet is processed.  Once processed, CC MAC may 
  reset some of the information needed to determine the packet num.

  @assumptions Function should not be called for QPCH. 

  @return TRUE if the synchronous or sub-synchronous control channel packet
  and FALSE if asynchronous control channel packet. 

  @context Calling task (HDRDEC) This routine is reentrant. */
/*============================================================================*/
boolean hdrcmac_get_cc_packet_num
(
  uint16 pkt_hstr,
    /**< timestamp of packet start with in Control Channel Cycle; must be in 
         the range 0-511 */

  uint16 scc_rate,
    /**< Decode rate of CC packet. */

  byte * pkt_hdr_ptr,
    /**< Pointer to first byte of payload. */

  uint16  * packet_num,
    /**< Return value: Determined packet number of packet missed if continued 
         packet. (Packet_num n means nth packet of sync capsule missed.) */

  boolean * is_subsync
    /**< Return value: Fills in TRUE if packet is a subsync packet FALSE 
         otherwise. */
)  
{
  uint16  cc_start_slot;
    /*  The start slot. This is where a CC packet was expected (excluding 
        CC offset slots). */

  uint16 pkt_slot_time;
    /* Packet timestamp in slots. */

  uint16 scc_packet_duration;
    /*  Duration of the Sync Capsule packets at current rate. */

  boolean is_sync_or_subsync = FALSE;
    /* Whether this was any type of sync or subsync packet. */

  boolean is_sync = FALSE;
    /* Whether this was a sync packet. */

  boolean cc_offset_aligned, cc_start_aligned;
    /* Flags that indicate whether the packet is aligned with the system
       CC offset and the specific CC start values, respectively. */

  boolean is_first_packet = FALSE;
    /* Whether this is first packet of SCC. */

  uint16 possible_packet_num;
    /* Packet num based on slots from Sync boundary */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Convert halfslots to slots */
  pkt_slot_time = pkt_hstr / 2;

  /* Grab HSTR and calculate whether packet is aligned with expected values.
     Note that cc_offset_aligned will be FALSE if cc_offset is unknown. */
  cc_start_slot = hdrsrch_get_cc_start();
  cc_offset_aligned = (((pkt_slot_time) % HDRCMAC_NUM_INTERLACES) 
                       == hdrcmac.scc_offset_slots);

  cc_start_aligned  = (((pkt_slot_time) / HDRCMAC_NUM_INTERLACES) 
                       == (cc_start_slot / HDRCMAC_NUM_INTERLACES));

  /* Determine the max packet duration based on rate. */
  scc_packet_duration = HDRCMAC_SCC_PACKET_DURATION( scc_rate );

  possible_packet_num = (pkt_slot_time / scc_packet_duration) + 1;

  /* If cc_offset is known and aligned, and we have a CRC failure: */     
  if ((cc_offset_aligned) && (pkt_hdr_ptr==NULL))
  {
    if (hdrcmac.waiting_for_last_pkt)
    {
      /* If this was a bad packet, only used the calculated packet num if the
         previous packet num was received */
      if ((pkt_slot_time == 0) ||
          ((possible_packet_num - hdrcmac.last_packet_num_received) == 1))
      {
        /* We have a continued packet, folks! Mark that packet_num needs to be
           calculated, and that this is a sync/subsync packet. */
        is_sync_or_subsync = TRUE;
        is_sync = TRUE;
      }
    }
    else if (cc_start_aligned)
    {
      /* Check against cc_start to avoid having async messages pollute stats. */
      is_sync_or_subsync = TRUE;

      if (cc_start_slot == 0)
      {
        is_sync = TRUE;
      }
    }     
  }
  else if (pkt_hdr_ptr)  /* Good packet case */
  {
    /* Sneakily peek at CC header to determine if this is a real syncCC */
    if (HDRCMAC_IS_SYNC_CAPS_PKT(pkt_hdr_ptr))
    { 
      /* Mark as sync */
      is_sync_or_subsync = TRUE;
      is_sync = TRUE;

      /* First packet? */
      if (HDRCMAC_IS_FIRST_CAPS_PKT(pkt_hdr_ptr))
      {
        is_first_packet = TRUE;
      }

    }
    else if ((cc_offset_aligned) && (cc_start_aligned))
    {
      /* Again, check against cc_start to avoid having 
         async messages pollute stats. */
      is_sync_or_subsync = TRUE;
    }
  }

  /* If sync and not first packet, calculate packet_num. 
     (Otherwise it will be 1). */
  if ((is_sync) && (!is_first_packet))
  {

    /* Check if this is a valid SCC slot time */
    if ((pkt_slot_time % scc_packet_duration) < HDRCMAC_NUM_INTERLACES)
    {
      *packet_num = possible_packet_num;
    }
    else
    {
      *packet_num = 1;

      /* Time is not sync'd.  Report as async since there is no time ref. */
      is_sync_or_subsync = FALSE;
    }
    
  }
  else
  {
    /* Whether this is subsync or sync, it is the first packet. */
    *packet_num = 1;
  }

  *is_subsync = !is_sync;

  return is_sync_or_subsync;

} /* hdrcmac_get_cc_packet_num() */

/*============================================================================
  HDRRX_ENABLE_MISSED_SCC_DETECTION
*/
/**
  Enables missed sync CC detection. hdrcmac_handle_lost_scc() is called
  when a missed packet is suspected.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrcmac_enable_missed_scc_detection
( 
  hdrhai_protocol_name_enum_type sender,
    /**< The protocol giving this command (only for software trace) */
  
  boolean clear_pending
    /**< Whether to clear any pending indication */
)
{
  hdrcmac_cmd_union_type cmd_buf;              
    /* Temporary command buffer */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRCMAC_TASK_PTR )
  {
    cmd_buf.hdr.sender = sender;
    cmd_buf.hdr.cmd = HDRCMAC_ENABLE_MISSED_SCC_DETECTION_CMD;
    cmd_buf.enable_scc.clear_pending_ind = TRUE;
      /* Always clear pending, it may be stale */

    hdrrx_queue_cmd( HDRHAI_CC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    HDRTRACE_CMD( HDRHAI_CC_MAC_PROTOCOL, 
      HDRCMAC_ENABLE_MISSED_SCC_DETECTION_CMD, 
      sender, hdrcmacsm_get_state() );

      hdrcmac.process_missed_scc_flag = TRUE;
        /* Turn on processing of missed scc indication from firmware. */
    
      clear_pending = TRUE;
        /* Always clear pending, it may be stale */

      HDR_MSG_PROT_2( MSG_LEGACY_HIGH, 
              "Enable missed scc detection: clear_pending %d, rcvd %d", 
              clear_pending, hdrcmac.missed_scc_flag_rcvd);

    /* If we are not told to clear pending indications, 
       process any pending indication now. */
    if (!clear_pending)
    {
      if (hdrcmac.missed_scc_flag_rcvd == TRUE )
      {
        hdrrx_handle_lost_sync_cc();
      }
    }

    hdrcmac.missed_scc_flag_rcvd = FALSE;
      /* Clear pending flag. */
  }
} /* hdrcmac_enable_missed_scc_detection() */


/*============================================================================
  HDRRX_DISABLE_MISSED_SCC_DETECTION
*/
/**
  Disables missed sync CC detection.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrcmac_disable_missed_scc_detection 
(
  hdrhai_protocol_name_enum_type sender 
    /**< The protocol giving this command (only for software trace) */
)
{

  hdrcmac_cmd_union_type cmd_buf;              
    /* Temporary command buffer */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRCMAC_TASK_PTR )
  {
    cmd_buf.hdr.sender = sender;
    cmd_buf.hdr.cmd = HDRCMAC_DISABLE_MISSED_SCC_DETECTION_CMD;
    hdrrx_queue_cmd( HDRHAI_CC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    HDRTRACE_CMD( HDRHAI_CC_MAC_PROTOCOL, 
      HDRCMAC_DISABLE_MISSED_SCC_DETECTION_CMD, 
      sender, hdrcmacsm_get_state() );

    hdrcmac.process_missed_scc_flag = FALSE;
      /* Do not process missed scc indication from firmware. */

    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
            "Disabled missed scc detection");
  }

} /* hdrcmac_disable_missed_scc_detection() */

/*============================================================================
  HDRCMAC_PROCESS_MISSED_SCC_INDICATION  
*/
/**
  Handles missed scc indication from firmware.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrcmac_process_missed_scc_indication 
( 
  int detectedSccOffset 
    /**< Indicates whether scc was detected */
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (detectedSccOffset == HDRCMAC_NO_SCC_PREAMBLE_DETECTED) 
  {

    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
          "Missed scc detect - enable: %d", hdrcmac.process_missed_scc_flag);

    if (hdrcmac.block_missed_scc_flag == FALSE)
    {

      /* Process missed SCC if enabled */
      if ( hdrcmac.process_missed_scc_flag == TRUE)
      {
        hdrrx_handle_lost_sync_cc();  
      }
      else
      {
        hdrcmac.missed_scc_flag_rcvd = TRUE;
          /* Remember one happened if not enabled. */
      }
    }
    else
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Blocked missed scc detection");
    }
  }
  else
  {
      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "SCC detect at: %x", detectedSccOffset);
  }

} /* hdrcmac_process_missed_scc_indication */


#ifdef FEATURE_HDR_DLNA
/*=========================================================================== 
  HDRCMAC_GET_NEXT_SCC_SLOT
*/
/**
  This is a utility function for D-LNA implementation. The function returns
  the next slot when SCC packet is expected based on the current slot and the
  negotiated SCI value.
  
  Note: The function does not account for SCC offset in SCC slot 
  calculation i.e. assumes SCC offset of zero.
 
  @return Next SCC slot calculated from the negotiated SCI value.
 
  @context HDRRX task.
 
  @sideeffects None */
/*=========================================================================*/

uint16 hdrcmac_get_next_scc_slot( void )
{
  uint8                       neg_sci;
    /* Negotiated SCI */

  qword                       cycle;
    /* Number of sleep cycles since the beginning of time */

  uint32                      slot;
    /* Current slot within the current sleep cycle */

  uint32                      num_of_slots_in_curr_sci;
    /* The period in slots of current SCI          */

  uint32                      scc_slot;
    /* Next SCC slot */

  hdridlec_slotted_mode_type  slotted_mode_attrib;
    /* Negotiated slotted mode attribute */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Obtain negotiated SCI value */
  if ( hdridlec_get_slotted_mode_attrib( &slotted_mode_attrib ) )
  {
    neg_sci = slotted_mode_attrib.slot_cycle1;
      /* Read negotiated SCI */
  }
  else
  {
    neg_sci = 0x6;
      /* In case of unavailability of negotiated SCI, use any SCI value
         >= 6. The value would result in expected SCC slot being returned
         as 0. */
  }

  if ( neg_sci < 0x6 )
  {
    /* Small SCI case. Calculate next expected SCC slot. */
    hdrts_get_cycle_and_slot( cycle, &slot, HDRTS_SLOTS_PER_CCC );
      /* Get the present time. */

    num_of_slots_in_curr_sci = HDRTS_SLOTS_PER_SCC >> (0x6 - neg_sci);
      /* Get the period ( in slots ) of current SCI. */

    scc_slot = slot - ( slot % num_of_slots_in_curr_sci );
      /* Determine the slot of the previous SCI period. */

    scc_slot += num_of_slots_in_curr_sci;
      /* Compute the next SCI slot. */

    scc_slot %= HDRTS_SLOTS_PER_SCC;
      /* Slot at which the next SCC packet is expected. */
  }
  else
  {
    /* Control channel packets are expected at control channel boundary
       for SCI values >= 6.  Return SCC slot as zero. */
    scc_slot = 0;
  }

  return ((uint16) scc_slot);
    /* Return next CC/SCC packet time. */

} /* hdrcmac_get_next_scc_slot( ) */


/*===========================================================================
  HDRCMAC_FORCE_DLNA_DISABLE
*/
/**
  This function facilitates disabling of DLNA during TAP/test mode operation.
  If the disable parameter passed to this function is TRUE and if DLNA is 
  enabled, it is disabled.
  If the disable parameter passed to this function is FALSE, DLNA is enabled 
  if permitted by NV setting.
 
  @param caller_id entity requesting this command
  @param override forces DLNA disable when set to TRUE
 
  @context HDRRX task
 
  @sideeffects None */
/*=========================================================================*/

void hdrcmac_force_dlna_disable
(
  hdrcmac_caller_id_enum_type caller_id,
    /**< The entity giving this command */

  boolean disable
    /**< Whether to enforce DLNA disable if it is enabled via NV */
)
{
  hdrcmac_cmd_union_type cmd_buf;              
    /* Temporary command buffer */

  uint64 debug_mask;           
    /* MAC debug mask from NV */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sanity check */
  if( ( caller_id != HDRCMAC_FTAP_ID ) && ( caller_id != HDRCMAC_RTAP_ID ) )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_ERROR, 
      "hdrcmac_force_dlna_disable: invalid caller id (%u)",
      caller_id );

    return;
  }

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRCMAC_TASK_PTR )
  {
    cmd_buf.hdr.cmd = HDRCMAC_FORCE_DLNA_DISABLE_CMD;
    cmd_buf.hdr.sender = HDRHAI_CC_MAC_PROTOCOL;
    cmd_buf.disable_dlna.caller_id = caller_id;
    cmd_buf.disable_dlna.disable = disable;
    hdrrx_queue_cmd( HDRHAI_CC_MAC_PROTOCOL, &cmd_buf, sizeof( cmd_buf ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    if ( disable )
    {
      /* Set bit */
      hdrcmac.dlna_disable_requests |= caller_id;

      /* Disable DLNA and early preamble detection if it is enabled */
      if ( hdrsrchutil_dlna_is_enabled() )
      {
        hdrsrchutil_set_dlna_enable( FALSE );
        hdrsrchutil_send_dlna_ctl( FALSE );
        hdrsrchutil_send_early_pream_ctl( FALSE );
      }

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "Added force DLNA disable vote: %u",
        caller_id );
    }
    else
    {
      /* Clear bit */
      hdrcmac.dlna_disable_requests &= ~caller_id;

      /* Enable DLNA if -
         - there are no other clients with active DLNA disable request 
         - permitted by NV 
         - it is not currently enabled */ 
      if ( ( hdrcmac.dlna_disable_requests == 0 ) 
        && ( ( hdrlmac_get_user_mac_debug_mask( &debug_mask ) == FALSE )
        || ( ( debug_mask & HDRLMAC_USER_MAC_DEBUG_DISABLE_DLNA )
        != HDRLMAC_USER_MAC_DEBUG_DISABLE_DLNA ) )
        && ( !hdrsrchutil_dlna_is_enabled() ) )
      {
        hdrsrchutil_set_dlna_enable( TRUE );

         /* Only when D-LNA is enabled, also check if Enhanced D-LNA is enabled via NV */
        if ( ( hdrlmac_get_user_mac_debug_mask( &debug_mask ) == TRUE )
          && ( ( debug_mask & HDRLMAC_USER_MAC_DEBUG_DISABLE_ENH_DLNA )
            == HDRLMAC_USER_MAC_DEBUG_DISABLE_ENH_DLNA ) )
        {
          /* Reset Enhanced D-LNA enable flag */
          hdrsrchutil_set_enh_dlna_enable( FALSE );
        }
        else
        {
          /* Set D-LNA enable flag */
          hdrsrchutil_set_enh_dlna_enable( TRUE );
        }
      }

      HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
        "Removed force DLNA disable vote: %u",
        caller_id );
    }
  }

} /* hdrcmac_force_dlna_disable() */

#endif /* FEATURE_HDR_DLNA */

 
