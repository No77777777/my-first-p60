/** @mainpage HDR Receive Task
    @file hdrrx.c
    @brief This module contains definitions and declarations for HDR Receive 
    task processing. 

    The HDR Receive task runs the following protocols: 
    - Radio Link Protocol (RLP), 
    - Signalinng Link Protocol (SLP), 
    - Packet Consolidatio Protocol (PCP), 
    - Control Channel MAC Protocol (CCMAC), 
    - Forward Traffic Channel MAC Protocol (FTCMAC). */

/*===========================================================================
  Copyright (c) 2000 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrrx.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/17/15   vlc     Added code to handle Run Control Event Notification errors.
                   Mainlined FEATURE_MODEM_RCINIT.
06/18/15   tnp     Fix compiler warnings
04/27/15   kss     Clean up watchdog support. 
02/25/15   wsh     Replaced efs API with mcfg version
01/08/15   rmv     Move the check to ignore pkts rcvd in QTA gap to hdrrx 
10/14/14   wsh     Changes to support Jolokia bringup
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
08/08/13   rmv     Featurized tmc APIs with ifndef FEATURE_MODEM_RCINIT
06/18/14   cnx     Change HDR RX message queue to dynamic queue.
04/12/13   smd     Fixed the bug of opening srch NV config file in hdrrx. 
04/10/13   smd     Enabled DTX/FC by default (for DI PLs).
03/01/13   dsp     Moved watchdog timer code to watchdog heart-beat model.
10/05/12   ukl     Resolved compilation errors after removing FEATURE_BCMCS.
08/30/12   arm     Added null pointer check for hdrfmac_put_pkt_cb. 
06/06/12   ljl     Updated for Dime RUMI.
04/03/12   cnx     Register MSGR when task is initialized.
03/22/12   arm     Fixed featurization bug for RCINIT. 
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/08/12   wsh     Merged NLB support
03/16/12   arm     Resolve TCB errors for Dime. 
12/12/11   sur     QChatUPK changes for optimization. 
10/10/11   arm     Added support for RCINIT.
10/28/11   pba     Optimized handoff phase 2 support
10/20/11   wsh     Changed PCP init code path to avoid double init
10/17/11   rmg     Removed inclusion of clkregim.h.
09/26/11   grl     Removed use of obsolete AEEstd.h
08/31/11   cnx     Disabled tunnel mode when exiting HDR.
08/04/11   ssu     Reference ftm_mode using ftm_get_mode().       
08/04/11   ssu     Replace hdr_ftm_fwd_curr_state with hdrhitmsg API calls.
07/27/11   cnx     Merged Optimized Handoff changes.
06/27/11   ssu     Removed inclusion of "hdrrlp.h", "hdrrlpdnak.h", and "hdrrlpnak.h".
06/16/11   ssu     Removed inclusion of "hdrrlp.h", "hdrrlpdnak.h", and "hdrrlpnak.h".
05/24/11   ssu     Removed inclusion of "hdrrlplog.h", and included "hdrrlp_api.h" instead
04/26/11   kss     Added check for KW. 
10/12/10   kss     Moved missed SCC detection to CCMAC for Q6.
09/17/10   pxu     Memory heap changes.
09/15/10   ljl     Supported dynamic wdog.
08/06/10   kss     Moved early term. dipswitch support to hdrfmac.
06/21/10   cnx     Fixed compiler warnings.
06/07/10   kss     Added hdrrx_get_connected_state_per_data().
05/27/10   kss     Checked SCC_PREAMBLE_IND payload before decalaring lost_scc
05/18/10   rmg     Shifted DRC supervision timer management to FMAC: 
                   registered HDRRX to receive DRC supervision ind from FW.
03/04/10   kss     Added call to hdrpcp_rx_buf_empty() when rx buffer is empty
12/21/09   etv     Ignore bad CRC/Cancelled packets.
12/08/09   wsh     Fixed dog timeout after TASK_STOP_SIG
12/03/09   kss     Added HDRRX_MSG_Q_SIG to signal mask.
12/01/09   kss     Removed msgr deregistration.
10/16/09   ac      Added support for HDRRX_RLP_LOGGING_SIG
08/17/09   kss     Added hdrrx_get_fl_slot_count() support.
07/29/09   etv     Added support for connected state PER.
10/02/09   lyl     Updated for rumi bringup
07/29/09   etv     Replaced HDRHSTR_GET_CC_START with hdrsrch_get_cc_start.
07/29/09   etv     Included hdrfw_msg.h
07/20/09   etv     Added AEEstd.h
07/15/09   etv     Moved sw module definitions to hdrmsg.h
06/22/09   etv     Fixed Lint warnings.
05/22/09   etv     Added support for processing fw indication msgs.
06/04/09   wsh     Removed unneeded rx.h and tx.h from HDR files
04/21/09   etv     Q6 development. Adopted Doxygen commenting style.
02/26/09   etv     Floor RxAGC1 during diversity tune away.
03/23/09   wsh     CMI: featurized CMI under FEATURE_CMI
03/06/09   pmk     Clean up the resources before sending task_stop
02/11/09   kss     Fix pkt pending check.
01/25/09   kss     Call cmac's get_packet_num() before processing as required.
01/25/09   kss     Added new enh sleep logging support.
01/12/09   etv     Fixed MC FL Packet header buffer overflow issue.
12/10/08   smd     Replaced assert.h with amssassert.h
12/05/08   kss     Removed/replaced intlocks.
12/04/08   etv     Lint fixes.
11/04/08   rmg     Added support for HDRRX_RLP_CTA_EXP_SIG.
10/13/08   kss     Lint fixes.
10/08/08   kss     Mainline FEATURE_HDR_CMAC_SUBSYNC_STDBY_OPT.
10/06/08   etv     Logged power log pkt even during SOODA but floored rxagc.
09/03/08   etv     Moved previous change to decoder module.
08/22/08   etv     Flush the cancelled packet from decob.
06/11/08   cc/etv  Moved PacketSize from DSM's Kind field to AppPtr field.
05/29/08   etv     Added support for CBUFF based flow control.
05/28/08   ljl     Updated HDR_PWR_PROFILE_STRB_TOGGLE().
03/31/08   etv     Cleaned up FL Physical layer packet logging code.
03/13/08   etv     Removed RevB 7500 featurization.
                   Removed HDRRX_PKT_LINKED. 
02/04/08   etv     Added support for logging FL physical layer packet dump.
01/14/08   etv     Refactored Air link Summary log packet for Artemis.
12/19/07   etv     Added support for per-carrier Instantaneous PER.
12/12/07   etv     Handled CBuff overflow gracefully. Updated DUP detection
                   code to handle multiple carriers.
11/01/07   etv     Added support for MC Air link summary log packet.
12/04/07   etv     Fixed featureization of RLP inidcation processsing;
                   set carrier_id in DSM item.
11/14/07   etv     Fixed merge error by using hdrrxstats API.
08/25/07   grl     Fixed CCMER logging statistics.
11/02/07   etv     Used FEATURE_HDR_REVB_7500 to control Rx 7500 feature
10/31/07   etv     Fixed merge error for compilation.
10/19/07   kss     Set traffic bit in all items of an FTC packet.
10/19/07   kss     Merge code to log dec hdr log pkt immediately in idle.
10/15/07   da      Added signal handling for HDRRX_RLP_NAK_DELAY_TIMER_SIG
10/10/07   etv     Minor updates after code review comments.
09/10/07   etv     Added user field DSM macros and minimized usage of 
                   FEATURE_HDRRX_REVB_7500.
08/24/07   sb      Added support for multi-carrier decoder header log packet.
08/30/07   etv     Added the header file to avoid compilation warning.
08/24/07   etv     Featurized hdrrlp_process_ind under RevB7500 feature.
08/24/07   etv     Reverted incorrect Fix for featurization issue with 
                   set_sched_group_id.
08/20/07   etv     Changed "carrier_id" to "demod_id"
08/16/07   etv     Fixed featurization issue with set_sched_group_id.
07/11/07   etv     Separated Statistics code to hdrrxstats and general cleanup.
                   Increased the size of CBuf for Rev-B
07/06/07   etv     Update for changes in hdrdec_pkt_type_enum_type and ignore
                   call back and use fmac_put_pkt function directly.
04/05/07   etv     Updated for Rev-B firmware changes to aid compilation.
02/23/07   rmg     Fixed lint errors.
01/29/07   kss     Fixed typo in previous check-in.
01/26/07   kss     Clear pkt ptr for cancelled sequences; lint fix.
01/25/07   etv     Added support for un-expected stats handling/detection.
01/09/07   etv     Added F3 messages to find sleep optimization improvements.
01/04/06   etv     Sleep optimization fix.
01/02/06   kss     Expand dup pkt detection to include drc 1-4 short pkts.
12/23/06   etv     Sleep Optimization. Set Lost SYNC CC sig after bad Sync CC.
12/15/06   etv     Fixed a bug with previous check-in upon code review comment.
12/12/06   etv     Forget DSM item handle after it has been delivered to avoid
                   crash on 7500 caused due to duplicate dsm_free_buffer call.
11/02/06   kss     Externalized HDRRX_SCC_OFFSET_UNKNOWN definition.
10/26/06   etv     Added support for on-arrival delivery of RX packets.
10/04/06   mt      Enable missed CC ISR after CC packet if disabled due to FPD.
09/16/06   mt      Interface change for forced preamble used status.
09/15/06   etv     Incremented dec read address when we run out of DSM items. 
09/08/06   spn     Removed the Signal, for Reservation Module. 
08/21/06   mt      Added forced preamble detection used flag to CC ISR Messages.
08/09/06   etv     Store PacketSize in DSM item for Release 0 as well.
07/24/06   jyw     Added power profiling strobes.
07/21/06   etv     Moved hdrcmac_powerup_init from HDRRX to HDRMC.
06/20/06   etv     Handle HDRRX timers as events.
06/13/06   etv     Included CRC and sequence to fl_pkt_header_info log pkt.
06/07/06   etv     Fixed bug. Removed usage of hdrrx.pkt_info outside ISR.
05/18/06   etv     Fixed compiler warning.
05/09/06   sy      Added new signal HDRRX_RESV_BLD_MSG_TIMER_SIG for
                   reservation messages handling in HDRRX task.
05/09/06   etv     Fixed lint comment.
03/30/06   kss     Fixed hdrrx_get_cc_status().
03/13/06   etv     Disabled F3 messages. 
02/10/06   kss     Check for cancelled packets at least once per frame.
01/10/06   kss     Use the same log sequence number for all FL stats logs.
12/07/05   etv     Fixed Critical lint errors.
11/28/05   ksu     CR 81596: Rev 0 data call fails all the time with dsrlpsq
                   crash during bi-directional data transfer
11/15/05   kss     Added hstr16 to packet debug messages.
11/04/05   ksu/sy  Delete 5500-specific hdrfmac_rlp_reg_srvc().
11/01/05   jyw     Fixed the bug in the rx buf clean up.
10/19/05   jyw     Fixed the overflow condition in the phy header logging.
10/17/05   jyw     Modified the logic in the hdr rx buf init.
10/10/05   jyw     Added revA pkt phy header logging support.
10/12/05   etv     Added support for MUP User statistics summary.
09/28/05   ksu/sy  HDR: use one-shot abort timer instead of 20ms ticks.
07/26/05   etv     Added code to store the packet type into the DSM item. 
                   Added support for Multi-user packet Fwd Statistics summary.
07/08/05   jyw     Added support to IS890A.
06/22/05   etv     Added support for Packet chaining in RX-FMAC interface. 
06/21/05   etv     Added CMAC protocol to possible ind processing desitination.
06/13/05   etv     Changed #define from LOG_HDR_REVA_SU_FW_STATS_C to 
                   LOG_HDR_REVA_SU_FWD_STATS_C.
06/02/05   etv     Added support for logging Rev-A FTC physical layer packet.
05/17/05   etv     Added support for logging Rev-A Single-user packet forward
                   statistics summary.
05/13/05   kss     Set the MAC trailer bits using HDRDSM header.
05/13/05   etv     Added function pointer registration for FMAC pkt forwarding.
                   Moved hdrrx_put_chain functionality to FMAC.
04/16/05   kss     Changes for modified decoder interface.
04/11/05   etv     Used hdrdsm macros to set/get slots_to_decode
04/06/05   kss     Track DRC0 packets, update decoder interface.
02/28/05   kss     Merged changes from MSMSHARED\hdr\rx\hdrrx#145
02/14/05   kss     Use correct enum for HDR FTM sync check.
01/28/05   kss     Merged changes from MSMSHARED\hdr\rx\hdrrx#139.
11/19/04   etv     Added hdrrx_clear_bcc_stats to reset MCS packet rate metrics.
09/17/04   kss     PLT change to drop packets at task level rather than 
                   interrupt level.
09/09/04   vas     Preliminary QOS support.
08/02/04   kss     Store slot_time each packet arrives.
07/06/04   kss     Only update sleep packet MER when SCC packet is missed.
06/25/04   sq      Fix for updating CC info in hdrsrchlog
05/20/04   kss     Handle running out of DSM items gracefully (drop pkts).
05/05/04   kss     Rename feature to HDR_BCMCS for consistency.
04/26/04   kss     Integrate HDR Multicast support.
04/13/04   mpa     Set traffic bit in dsm item header if message was received
                   while in traffic regardless of the channel.
03/29/04   kss     Added hdrrx_get_average_per() function.
03/22/04   kss     Detect duplicate DRC1 packets.
11/20/03   kss     Fixed casting warnings.
11/11/03   sq      Added support for HDRSRCH SLEEP INFO logging
10/21/03   kss     Include rf.h instead of rfhdr.h.
09/16/03   mpa     Converted F3 msg to use new HDR PROT MSG2.0 SSID
09/16/03   kss     Use hstr%512 in messages.
09/12/03   vas     Removed featurization for inclusion of hdrds.h
08/29/03   km/kss  Use new F3 msg macros; always print CC message.
08/13/03   sq      Added support for FEATURE_PLT
07/30/03   kss     Add support for MSM6500.
04/10/03   kss     Differentiate between missed preamble and bad CRC for SCC.
04/07/03   kss     Call new hdrcmac function to detect missing SyncCC packets.
03/12/03   kss     Include HSTR in SyncCC miss message.
03/04/03   kss     Use decoder macros to check for false CRC pass.
02/24/03   kss     Added functions to enable/disable missed SyncCC detection.
01/30/03   vr/kss  Added support for HDRRX to yield CPU to lower priority
                   tasks if it is processing too many items back to back in
                   the unoptimized path (could happen if we get RLP packets
                   out of seq or a huge RLP NAK).
                   Also, added support for flow control of the base station
                   based on MAC layer buffer.
01/29/03   kss     Added supprt for detection of bad CRC on SyncCC.
01/09/03   mss     Corrected False CRC check message.
11/19/02   mpa     Moved early termination dipswitch to new interface.
11/12/02   kss     Added IRAM support on function-by-function basis.
11/11/02   kss     Reduced circular buffer size to save some RAM.
11/05/02   aaj     Remove FEATURE_HDRSRCH_PROFILE. Add msg for CC Dec ISR
09/30/02   mpa/kss Added hdrrx_log_fwd_stats (merged log rx and ftc rate cnt)
09/03/02   kss     Use new hdrdec.h integrity check and CRC check macros.
08/02/02   om      Removed FTAP MSG macro.
07/25/02   om      Added sanity check for single enc. FMT B packets.
07/23/02   om      Added CC decode attempt count for IS-890.
07/18/02   kss     Added workaround for when a bad packet header is detected.
07/10/02   vas     Added ASSERT to make sure the fast insert pointer for the
                   RLP reseq queue is not NULL before accessing it.
05/20/02   kss/vr  Added setting of HUNT bit in the DSM items from decoder
                   for 7E detection
05/09/02   hcg     Added checks for RLP reset conditions via macro in
                   hdrrx_put_chain().  The function will now dump all received
                   RLP data if it in RLP reset state, or it has been informed
                   to be reset at the next HDRTX frame building.
05/06/02   om      Added re-use optimizations for FTAP
04/18/02   kss     Don't print throughput message if nothing has been Rx'd;
                   clear sequence buffer on sequence number reset.
03/12/02   sq      Added support for FTM.
03/08/02   kss     Moved set_ack_mode() to hdrfmac.c.
03/03/02   kss     Removed clock enable/disable for HDR (moved to hdrsrch).
02/11/02   ss      Modified parameters of hdrfmac_enable_flow_control().
01/07/02   kss     Check metric to reduce false CRC passes, increase decoder
                   packet buffer size.
12/14/01   kss     Corrected instantaneous PER calculation.
12/11/01   kss     Added hdrrx_reset_pkt_sequences() for reseting seq. numbers.
12/09/01   vr      Changed HDRDS_PS_Q_LIM to hdrds_ps_q_lim
12/09/01   vr      Flow control HDR FL if PS PPP queue too large.
12/08/01   hcg     Fixed queue linking problem in hdrrx_put_chain().
12/07/01   kss     Disable HDR clocks when exiting HDR.
12/07/01   hcg     Fixed problem where data path optimizations were not taking
                   into account the is_valid bit of the FMAC packet.
11/30/01   hcg     Modified RLP logging packet payload size.
11/11/01   kss     Added early termination support.
11/09/01   aaj     Added support for FEATUER_HDRSRCH_PROFILE
11/09/01   rao     Added DSM REUSE Optimization support.
11/06/01   kss     Fill in ratchet bit of power log; call new get_tx_adjust
                   function in rf.
10/29/01   mpa     Added hdrrx_reset_dm_fwd_stats().
10/08/01   om      Added HDR IS890 signal handling
09/25/01   kss     Implemented IS890 test modes.
09/20/01   kss     Added hdrrx_set_ack_mode() function; revised power packet
                   logging.
09/20/01   rao     Added Partial RLP Frame logging support
09/17/01   kss     Added support for effective ARQ logging.
09/14/01   kss     Fixed instantaneous PER calculation.
09/14/01   hcg     Added changes to hdrrx_put_chain() to handle case where
                   flow is not enabled on the stream for the RLP instance.
09/13/01   vas     Changes to support RLP configured on any stream.
09/11/01   rao     Removed misleading Debug message
09/10/01   kss     Externalized reset logging function.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
09/04/01   kss/hcg First pass of Rx chain optimizations.
08/07/01   kss     Optimized sequencing/passing of packets to upper layers.
06/30/01   kss     Added Rx sequencing.
06/20/01   kss     Enable decoder clock in HDR entry.
06/15/01   kss     Added hdrrx subtask, HDR enter/exit signals, and dog
                   support; updated logging.
05/08/01   kss     Changed tasks from rx_tcb to hdrrx_tcb.
05/02/01   kss     Removed dependencies on MSM4500 DSP. (Logging must be
                   redone).
04/26/01   kss     Added HDRRX_CMAC_TIMER_SIG to wait mask.
03/29/01   vas     Added off-target simulation code (FEATURE_HDR_MOBILE_SIM)
02/09/01   aaj     Added logging support for hdrrx_stats.
11/09/00   kss     Re-merged new indication types.
05/22/00   om      Created module.

===========================================================================*/
/* <EJECT> */
/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "memory.h"
#include "amssassert.h"
#include "task.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#include "hdrdebug.h"
#include "mc.h"
#include "modem_mem.h"

#ifdef FEATURE_CMI
#include "mc_v.h"
#endif

#include "fs_public.h"
#include "fs_sys_types.h"
#include "fs_errno.h"

#include "err.h"
#include "dog.h"
#include "dog_hb_rex.h"
#include "dsm.h"
#include "queue.h"
#include "comdef.h"

#include "hdrhai.h"
#include "hdrrx.h"
#include "hdrrxi.h"
#include "hdrrxstats.h"
#include "hdramac.h"
#include "hdrdec.h"
#include "hdrdsm.h"
#include "hdrmod.h"
#include "hdrlogi.h"
#include "hdrlog.h"
#include "hdrmdspmcr.h"
#include "hdrmdsp.h"
#include "hdrsrchlog.h"

#include "hdrcmac.h"
#include "hdrcmaci.h"
#include "hdrfmac.h"
#include "hdrfmaci.h"
#include "hdrrmac.h"
#include "hdrpcp.h"
#include "hdrsrch.h"
#include "hdrslp.h"
#include "hdrslpi.h"
#include "hdrrlp_api.h"

#include "hdrfmacutil.h"

#ifdef FEATURE_HDR_IS890
#include "hdris890.h"
#include "hdrftap.h"
#endif

#ifdef FEATURE_FACTORY_TESTMODE
#include "hdrhitmsg.h"
#include "ftm.h"
#endif

#ifdef FEATURE_HDR_BCMCS
#include "hdrbcmac.h"
#endif


#ifdef FEATURE_HDRSRCH_POWER_PROFILE
#include "hdrsrchutil.h"
#endif /* FEATURE_HDRSRCH_POWER_PROFILE  */

#ifdef FEATURE_HDR_QCHAT
#include "hdrqchatupk_opts.h"
#include "hdrqchatupk.h"
#endif /*FEATURE_HDR_QCHAT*/

#include "hdrmsg.h"
#include "msgr_rex.h"
#include "hdrfw_msg.h"

#ifdef FEATURE_LTE_TO_HDR_OH
#include "emm_irat_if_msg.h"
#include "hdrsaprx.h"
#include "hdrsap.h"
#endif /* FEATURE_LTE_TO_HDR_OH */

#include "rcinit.h"
#define HDRRX_RCEVT_PREFIX "HDRRX:"
#define HDRRX_RCEVT_READY HDRRX_RCEVT_PREFIX "ready"
#define HDRTX_RCEVT_PREFIX "HDRTX:"
#define HDRTX_RCEVT_READY HDRTX_RCEVT_PREFIX "ready"

#ifdef FEATURE_HDR_REVC
#include "hdrlmac.h"
#endif /* FEATURE_HDR_REVC */

#include "hdrutil.h"

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
rex_tcb_type*    hdrrx_tcb_ptr;


/*==========================================================================
                                MACROS 
===========================================================================*/

/** Number of event buffers available. */
#define HDRRX_EVENT_BUFS_NUM                                      5

/** Calculate the number of HSTRs from CC start to CC dec */
#define HDRRX_CALC_CC_TO_CCDEC( hstr, cc_start )            \
  ( 512 - ( ( ( ( cc_start * 2 ) + 512 ) -                  \
  ( ( hstr ) % 512 ) ) % 512 ) )

/** Value to indicate no premable detected in possible SCC slots */
#define HDRRX_NO_SCC_PREAMBLE_DETECTED                       0xFFFF


#define HDRRX_EFS_NV_CONF_FILE_PATH  "/nv/item_files/conf/hdrmac_config_info.conf"

/*---------------------------------------------------------------------------
                          TASK WATCHDOG REPORTING
---------------------------------------------------------------------------*/
#undef  DOG_HDRRX_RPT
#define DOG_HDRRX_RPT hdrrx_dog_rpt_var
dog_report_type   hdrrx_dog_rpt_var = 0;
  /* Dog rpt */


/*==========================================================================
                               TYPEDEFS 
===========================================================================*/
/** Task event handling Buffer type definition. */
typedef struct
{
  hdrbuf_hdr_type hdr;                       
    /**< Header for this item       */

  union
  {
    hdrrx_protocol_cmd_union_type cmd;      
      /**< Cmds for prot's in rx task */
    hdrbuf_ind_type ind;      
      /**< All ind's have this type */
    dsm_item_type *item_ptr; 
      /**< Msg is a DSM item (chain) */
    uint32 timer_id; 
      /**< Timer ID */
  } item;

} hdrrx_buf_type;

/*==========================================================================
                              GLOBAL VARIABLES
===========================================================================*/

/** Data structure for hdrrx global data */
LOCAL struct
{
  int scc_offset_in_half_slots;
    /**< Sync cc offset */

  hdrrx_put_fmac_pkt_cb_type hdrfmac_put_pkt_cb;
    /**< Callback to forward FMAC packet to FMAC */

  q_type event_q;
    /**< Queue of pending commands, indications, and messages. */

  q_type msg_q;
    /**< Queue of in-use message buffers */

  msgr_client_t msgr_client_id;
    /* Messenger client ID */

} hdrrx;

/*============================================================================
                        LOCAL FUNCTION DEFINITIONS
=============================================================================*/

void hdrrx_create_efs_item_conf_file( void );

/*============================================================================
  HDRRX_INIT
*/
/**
  This procedure does any task specific initialization.

  Initialized the HDR Receive task by
    - Initializing the watchdog timer
    - Setting up the event queues.
    - Initializes SLP, PCP and HDRRXSTATS modules.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_init ( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize queues */

  ( void ) q_init ( &hdrrx.event_q );

  hdrrxstats_init();

} /* hdrrx_init */

/*============================================================================
  HDRRX_MSG_INIT
*/
/**
  This function performs Rx task message initialization.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_msg_init ( void )
{
  errno_enum_type estatus;
    /* Error status */

  msgr_id_t queue_id;
    /* Queue Identifier */

  msgr_umid_type hdrrx_ind[] = 
  {
    HDR_FW_SCC_PREAMBLE_IND,
    HDR_FW_DRC_SUPERVISION_TIMEOUT_IND,
    HDR_FW_DRC_SUPERVISION_RESTART_IND
#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
    ,NAS_EMM_IRAT_HDR_DL_MSG_IND
#else
    ,NAS_EMM_IRAT_DL_MSG_IND
#endif /* FEATURE_IRAT_DSM_SUPPORT */
#endif /* FEATURE_LTE_TO_HDR_OH */
  };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ( void ) q_init ( &hdrrx.msg_q );

  estatus = msgr_client_create( &hdrrx.msgr_client_id );

  ASSERT ( estatus == E_SUCCESS );

  /* Create the msgr client and associated with rex queues */
  estatus = msgr_client_add_rex_q ( &hdrrx.msgr_client_id,
                                    HDRRX_TASK_PTR,
                                    HDRRX_MSG_Q_SIG,
                                    &hdrrx.msg_q,
                                    MSGR_NO_QUEUE,
                                    MSGR_OFFSETOF( hdrrx_msg_s_type, msg ),
                                    MSGR_NO_OFFSET,
                                    (uint16) sizeof(hdrrx_msg_s_type),
                                    &queue_id );

  ASSERT ( estatus == E_SUCCESS );

  /* Register indications */
  estatus = msgr_register_block ( MSGR_HDR_RX,
                                  &hdrrx.msgr_client_id,
                                  MSGR_ID_REX,
                                  hdrrx_ind,
                                  sizeof(hdrrx_ind)/sizeof(msgr_umid_type));

  ASSERT ( estatus == E_SUCCESS );

} /* hdrrx_msg_init */

/*============================================================================
  HDRRX_MSG_STOP
*/
/**
  Cleansup the message interface. 

  Cleans up the message interface by doing the following. 
    - De-registers the registered messages. 
    - Deletes the messenger Client ID for HDRRX module. 

  @context HDRRX */
/*===========================================================================*/
void hdrrx_msg_stop ( void )
{
  errno_enum_type  estatus;
    /* Error status */

  msgr_umid_type hdrrx_ind[] = 
  {
    HDR_FW_SCC_PREAMBLE_IND,
    HDR_FW_DRC_SUPERVISION_TIMEOUT_IND,
    HDR_FW_DRC_SUPERVISION_RESTART_IND
#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
    ,NAS_EMM_IRAT_HDR_DL_MSG_IND
#else
    ,NAS_EMM_IRAT_DL_MSG_IND
#endif /* FEATURE_IRAT_DSM_SUPPORT */
#endif /* FEATURE_LTE_TO_HDR_OH */
  };

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (hdrrx.msgr_client_id == 0)
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "MSGR already deregistered" );
    return;
  }

  estatus = msgr_deregister_block( MSGR_HDR_RX,
                                   &hdrrx.msgr_client_id,
                                   hdrrx_ind,
                                   sizeof(hdrrx_ind)/sizeof(msgr_umid_type) );

  ASSERT ( estatus == E_SUCCESS );

  estatus = msgr_client_delete ( &hdrrx.msgr_client_id );

  ASSERT ( estatus == E_SUCCESS );

} /* hdrrx_msg_stop */


/*============================================================================
  HDRRX_PROCESS_WDOG
*/
/**
  This function reports wdig and starts wdog timer.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_process_wdog( void )
{
  (void) rex_clr_sigs( HDRRX_TASK_PTR,  HDRRX_RPT_TIMER_SIG );
  dog_hb_report(DOG_HDRRX_RPT); 
} /* hdrrx_process_wdog */

/*============================================================================
  HDRRX_ENTRY_PROCESSING
*/
/**
  This function performs Rx task entry processing when entering HDR mode.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_entry_processing ( void )
{
  /* Reset ack mode */
  hdrfmac_set_ack_mode ( IS856_COMPLIANT );

  hdrrx.scc_offset_in_half_slots = HDRRX_SCC_OFFSET_UNKNOWN;

  hdrrxstats_entry_processing();

} /* hdrrx_entry_processing */


/*============================================================================
  HDRRX_EXIT_PROCESSING
*/
/**
  This function performs exit processing when exiting HDR mode.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_exit_processing ( void )
{
  hdrrxstats_exit_processing();

} /* hdrrx_exit_processing */


/*============================================================================
  HDRRX_WAIT
*/
/**
  This procedure waits for specified signals to be set.  This routine also
  checks any queues that are being waited on.  If the queue has an item,
  this routine sets the signal without waiting.  This allows for the queue
  signal to be cleared after each item is removed.  If a queue signal is set,
  this routine checks the queue and clears the signal if there is nothing
  on it.  This centralizes the queue checking.

  @context HDRRX task. 

  @return For non-queue signals, a signal bit is set in the return mask if the 
  signal was in the requested mask and is also set in the REX TCB. For queue 
  signals, a signal bit is set in the return mask if the signal was in the 
  requested mask and the queue associated with the signal has an item on it.

  @sideeffects None */
/*============================================================================*/
LOCAL rex_sigs_type hdrrx_wait
(
  rex_sigs_type  requested_mask         
    /**< Mask of REX signals to wait for */
)
{
  rex_sigs_type rex_signals_mask; 
    /**< Signal mask returned by rex_get_sigs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Loop until at least one requested signal is set
  -------------------------------------------------------------------------*/
  do 
  {
    rex_signals_mask = rex_wait( requested_mask | HDRRX_RPT_TIMER_SIG);
      /* Actually wait for one of the requested signals to be set */

    /*-----------------------------------------------------------------------
     Watchdog report timer signal. Kick watchdog and reset timer.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_RPT_TIMER_SIG) != 0 )
    {
      hdrrx_process_wdog();
    }

  } while ( (requested_mask & rex_signals_mask) == 0 );

  /*-------------------------------------------------------------------------
   Kick watchdog one last time on the way out.
  -------------------------------------------------------------------------*/
  (void) rex_clr_sigs( HDRRX_TASK_PTR,  HDRRX_RPT_TIMER_SIG );
  dog_hb_report( DOG_HDRRX_RPT );
  /*-------------------------------------------------------------------------
   Assemble return mask.  Only return signals that were requested.
  -------------------------------------------------------------------------*/
  return ( rex_signals_mask & requested_mask );

} /* hdrrx_wait */


/*============================================================================
  HDRRX_PROTOCOL_CMD
*/
/**
  All asynchronous commands for protocols that run in the RX task context
  are routed through here. This function dispatches the command to the
  protocol it is for.  Synchronous commands are processed in the context of
  the protocol issuing the command, so they are not queued and thus are not
  routed through here.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_protocol_cmd
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Protocol name of the sender */
  hdrrx_protocol_cmd_union_type *hdrrx_cmd_union_ptr
    /**< Pointer to the command union */
)
{
  switch ( protocol_name )
  {
    case HDRHAI_CC_MAC_PROTOCOL:
    {
      hdrcmac_process_cmd( &hdrrx_cmd_union_ptr->cmac );
      break;
    }

    case HDRHAI_FTC_MAC_PROTOCOL:
    {
      hdrfmac_process_cmd( &hdrrx_cmd_union_ptr->fmac );
      break;
    }

    default:
    {
      ERR( "CMD for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }

} /* hdrrx_protocol_cmd */


/*============================================================================
  HDRRX_PROTOCOL_CMD
*/
/**
  All indications for protocols that run in the RX task context are routed
  through here. This function dispatches the indication to the protocol it
  is for.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_protocol_ind
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Protocol name of the sender */
  hdrbuf_ind_type *hdrrx_ind_ptr
    /**< Pointer to the indication for a protocol in HDRRX task */
)
{
  switch ( protocol_name )
  {
    case HDRHAI_FTC_MAC_PROTOCOL:
    {
      hdrfmac_process_ind( hdrrx_ind_ptr->ind_name,
                          &hdrrx_ind_ptr->ind_data );
      break;
    }

#ifdef FEATURE_HDR_REVB
    case HDRHAI_RADIO_LINK_PROTOCOL:
    {
#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlp_process_ind( hdrrx_ind_ptr->ind_name,
                          &hdrrx_ind_ptr->ind_data );
#endif /* FEATURE_RUMI3_BRINGUP */
      break;
    }
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVA_L1

    case HDRHAI_CC_MAC_PROTOCOL:
    {
      hdrcmac_process_ind( hdrrx_ind_ptr->ind_name,
                           &hdrrx_ind_ptr->ind_data );
      break;
    }
#endif

#ifdef FEATURE_LTE_TO_HDR_OH
    case HDRHAI_SAP_RX_MODULE:
      hdrsaprx_process_ind( hdrrx_ind_ptr->ind_name,
                           &hdrrx_ind_ptr->ind_data );
      break;
#endif /* FEATURE_LTE_TO_HDR_OH */

    default:
    {
      ERR( "IND for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }

} /* hdrrx_protocol_ind */


/*============================================================================
  HDRRX_PROTOCOL_MSG
*/
/**
  All messages for protocols that run in the RX task context are routed
  through here. This function dispatches the message to the protocol it
  is for.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_protocol_msg
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Protocol name of the sender */
  hdrhai_protocol_instance_enum_type protocol_instance,
    /**< In-config or In-use instance of the protocol */
  dsm_item_type *item_ptr
    /**< Message for a protocol in HDRRX task  */
)
{
  switch ( protocol_name )
  {
    case HDRHAI_FTC_MAC_PROTOCOL:
    {
      hdrfmac_process_msg( item_ptr, protocol_instance );
      break;
    }

#ifdef FEATURE_HDR_QCHAT
    case HDRHAI_CC_MAC_PROTOCOL:
      if (hdrqchatupk_mcs_enabled()) 
      {
        hdrqchatupk_process_acm_msg(item_ptr);
      }
      break;
#endif /*FEATURE_HDR_QCHAT*/
    default:
    {
      ERR( "MSG for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }

} /* hdrrx_protocol_msg */


/*============================================================================
  HDRRX_PROTOCOL_TIMER
*/
/**
  All timer expiration events for protocols that run in the HDRRX task context
  are routed here. This function dispatches the timer expiry to the protocol it
  is for.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_protocol_timer
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Protocol that owns the timer */
  uint32 timer_id
    /**< Timer identification */
)
{
  switch (protocol_name)
  {
    case HDRHAI_CC_MAC_PROTOCOL:
      hdrcmac_process_timer(timer_id);
      break;

#ifdef FEATURE_HDR_REVC
    case HDRHAI_FTC_MAC_PROTOCOL:
      hdrfmac_process_timer( timer_id );
      break;
#endif /* FEATURE_HDR_REVC */

    default:
      ERR( "TIMER for unknown protocol %d", protocol_name, 0, 0 );
      break;
  }

} /* hdrrx_protocol_timer */


/*============================================================================
  HDRRX_PROTOCOL_ITEM
*/
/**
  This function routes the item to the protocol it is for, according to
  whether it is a command, indication, or message

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_protocol_item
(
  hdrrx_buf_type *hdrrx_buf_ptr
    /**< Pointer to buffer containing either command, message or indication 
         for a protocol in HDRRX task */
)
{
  ASSERT (hdrrx_buf_ptr != NULL);

  switch ( hdrrx_buf_ptr->hdr.type_of_item )
  {
    /* The item is a command */
    case HDRBUF_COMMAND: 
    {
      hdrrx_protocol_cmd( hdrrx_buf_ptr->hdr.protocol_name,
                          &hdrrx_buf_ptr->item.cmd );
      break;
    }

    /* The item is an indication */
    case HDRBUF_INDICATION: 
    {
      hdrrx_protocol_ind( hdrrx_buf_ptr->hdr.protocol_name,
                          &hdrrx_buf_ptr->item.ind );
      break;
    }

    /* The item is a message (DSM item) */
    case HDRBUF_MESSAGE: 
    {
      hdrrx_protocol_msg( hdrrx_buf_ptr->hdr.protocol_name,
                          hdrrx_buf_ptr->hdr.protocol_instance,
                          hdrrx_buf_ptr->item.item_ptr );
      break;
    }

    /* The item is a timer */
    case HDRBUF_TIMER:  
    {
      hdrrx_protocol_timer( hdrrx_buf_ptr->hdr.protocol_name,
                            hdrrx_buf_ptr->item.timer_id );

      break;
    }

    default:
    {
      ERR( "BUF for unknown item %d", hdrrx_buf_ptr->hdr.type_of_item, 0, 0 );
      break;
    }
  }

} /* hdrrx_protocol_item */

/*============================================================================
  HDRRX_HANDLE_LOST_SYNC_CC
*/
/**
  Sets the HDRRX_SYNC_CC_LOST_SIG.  This routine is called when the AT
  misses sync CC preamble. *It may be called in interrupt context.*

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_handle_lost_sync_cc ( void )
{
  uint32  current_hstr;
    /* The current hstr at the time of the isr firing. */

  uint16 packet_num;
    /* packet_num if continued SCC packet was missed. */
  
  boolean inc_cc_mer = FALSE;
    /* Count this missed preamble against the CC MER. */

  boolean is_sync;
    /* Whether packet in question is sync (TRUE) or subsync (FALSE). */

  uint16 cc_start;
    /* Current CC Start */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  is_sync = hdrcmac_sync_cc_is_missing ( &packet_num );

  if ( is_sync )
  {
    hdrrxstats_update_cc_status ( HDRLOG_CC_PREAMBLE_LOST );
      /* Record the latest control channel status */

    inc_cc_mer = TRUE;
      /* Increments CC MER count, since at least one packet of SyncCapsule
         was lost. */
  }
  else
  {
    hdrrxstats_update_cc_status ( HDRLOG_CC_SUBSYNC_PREAMBLE_LOST );
      /* Record the latest control channel status */

    /* Don't mark to increment MER since AN may not have sent anything. */
  }

  current_hstr = HDRHSTR_GET_COUNT() % 512;
    /* Grab the current HSTR count. */

  cc_start = hdrsrch_get_cc_start();
    /* Grab the CC start */

  hdrsrchlog_update_cc_info ( HDRRX_CALC_CC_TO_CCDEC ( current_hstr, cc_start ),
                              0xff,
                              HDRSRCHLOG_PKT_DECODE_FAILURE_SLOT_CNT,
                              FALSE,
                              inc_cc_mer,
                              packet_num,
                              !is_sync );
    /* Update the control channel infomation for SLEEP INFO log packet  */
   
  (void) rex_set_sigs ( HDRRX_TASK_PTR, HDRRX_SYNC_CC_LOST_SIG );
    /* Give sig so we can handle this at task level */

} /* hdrrx_handle_lost_sync_cc */


/*============================================================================
  HDRRX_PROCESS_MSG
*/
/**
  This function processes indication messages from firmware.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_process_msg 
( 
  hdrrx_msg_s_type *msg_ptr
    /**< Pointer to received indication message */
)
{

  ASSERT (msg_ptr != NULL);

  HDR_MSG_PROT_1 ( MSG_LEGACY_HIGH, "Rcvd. FW indication 0x%x", 
                   msg_ptr->msg.fw_ind.hdr.id );

  switch ( msg_ptr->msg.fw_ind.hdr.id )
  {
    case HDR_FW_SCC_PREAMBLE_IND:
      hdrcmac_process_missed_scc_indication(
        msg_ptr->msg.fw_ind.scc_preamble.detectedSccOffset );
      break;

    case HDR_FW_DRC_SUPERVISION_TIMEOUT_IND:
      hdrfmac_enter_drc_supervision(msg_ptr->msg.fw_ind.
        drc_supervision_timeout.drcSupervsionTimeOutMask);
      break;
  
    case HDR_FW_DRC_SUPERVISION_RESTART_IND:
      hdrfmac_exit_drc_supervision(msg_ptr->msg.fw_ind.
        drc_supervision_restart.drcSupervsionRestartTxMask);
      break;

#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
    case NAS_EMM_IRAT_HDR_DL_MSG_IND:
      hdrsaprx_process_lte_dl_msg( &msg_ptr->msg.irat_dl_ind );
#else
    case NAS_EMM_IRAT_DL_MSG_IND:
      hdrsaprx_process_lte_dl_msg( &msg_ptr->msg.irat_dl_ind.msg );
#endif /* FEATURE_IRAT_DSM_SUPPORT */
      break;
#endif /* FEATURE_LTE_TO_HDR_OH */


    default:
      HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, 
                       "Unknown msg: 0x%x", msg_ptr->msg.fw_ind.hdr.id );
  } /* switch */

} /* hdrrx_process_msg */

/*============================================================================
  HDRRX_SUBTASK
*/
/**
  This function runs when HDR is active.  It contains the HDR event
  processing loop.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
LOCAL void hdrrx_subtask( void )
{
  rex_sigs_type rex_signals_mask;   
    /* Task signal mask.     */

  hdrrx_buf_type *hdrrx_buf_ptr;      
    /* Event buffer pointer. */

  boolean exit_hdr_mode = FALSE;     
    /* Exit flag */

  rex_sigs_type rex_wait_mask;   
    /* Task wait mask.     */

  hdrdec_pkt_status_e_type delivered_pkt_status;
    /* To be Delivered packet status */

  int16 delivered_pkt_slot_time;
    /* Time packet(about to be delivered) arrived */

  dsm_item_type* delivered_pkt_ptr;
    /* Packet about to be delivered */

  boolean ftc_pkt_received = FALSE;
    /* Marks whether ftc packet has been received */

  rex_tcb_type *hdrdec_tcb_ptr; /* temp ptr */

#ifdef FEATURE_HDR_RUMI
  uint16 i; /* loop variable */
  uint8 data_pattern[2], data[2]; 
    /* data pattern and temp data needed to match CSIM */
#endif /* FEATURE_HDR_RUMI */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Entering HDR Rx subtask...");

  hdrrx_entry_processing();
    /* HDR mode entry processing */

  rex_wait_mask =                ( HDRRX_EVENT_Q_SIG             |
                                   HDRRX_MSG_Q_SIG               |
                                   HDRRX_PKT_READY_SIG           |
                                   HDRRX_SYNC_CC_LOST_SIG        |
                                   HDRRX_RLP_ABT_TIMER_SIG       |
                                   HDRRX_RLP_NAK_DELAY_TIMER_SIG |
                                   HDRRX_RLP_CTA_EXP_SIG         |
                                   HDRRX_RLP_LOGGING_SIG         |
                                   HDRRX_IS890_FTAP_PN_SIG       |
                                   HDRRX_LOG_BUFFER_READY_SIG    |
                                   HDRRX_EXIT_HDR_SIG            |
                                   TASK_OFFLINE_SIG              |
                                   TASK_STOP_SIG );

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  while ( !exit_hdr_mode )
  {

    rex_signals_mask = hdrrx_wait( rex_wait_mask );

    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_STOP_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "TASK_STOP_SIG received");
      (void) rex_clr_sigs( HDRRX_TASK_PTR, TASK_STOP_SIG );

     /* Deregister the registered messages and delete the messenger client */
      hdrrx_msg_stop();

    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_OFFLINE_SIG) != 0 )
    {
      HDR_MSG_PROT (MSG_LEGACY_MED,  "TASK_OFFLINE_SIG received");
      (void) rex_clr_sigs( HDRRX_TASK_PTR, TASK_OFFLINE_SIG );

    }

    /*-----------------------------------------------------------------------
     RLP NAK abort timer signal. Clear signal, and invoke the
     protocol signal handling function.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_RLP_ABT_TIMER_SIG) != 0 )
    {

      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_RLP_ABT_TIMER_SIG );

#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlpnak_age_lists();
#endif /* FEATURE_RUMI3_BRINGUP */

    }

    /*-----------------------------------------------------------------------
     RLP NAK Delay timer signal. Clear signal, and invoke the
     protocol signal handling function.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_RLP_NAK_DELAY_TIMER_SIG) != 0 )
    {
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_RLP_NAK_DELAY_TIMER_SIG );

#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlpdnak_age_lists();
#endif /* FEATURE_RUMI3_BRINGUP */

    }

    /*-----------------------------------------------------------------------
     RLP CTA expiration signal. Clear signal, and invoke the
     protocol signal handling function.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_RLP_CTA_EXP_SIG) != 0 )
    {

      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_RLP_CTA_EXP_SIG );
#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlp_cta_exp_sig_hdlr();
#endif /* FEATURE_RUMI3_BRINGUP */
    }

    if ( (rex_signals_mask & HDRRX_RLP_LOGGING_SIG) != 0 )
    {

      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_RLP_LOGGING_SIG );
#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlp_log_sig_hdlr();
#endif /* FEATURE_RUMI3_BRINGUP */
    }

#if defined( FEATURE_HDR_IS890 ) || defined( FEATURE_HDR_IS890A )
    /*-----------------------------------------------------------------------
     IS-890 FTAP signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_IS890_FTAP_PN_SIG) != 0 )
    {
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_IS890_FTAP_PN_SIG );
#ifndef FEATURE_RUMI3_BRINGUP
      hdrftap_process_sig( HDRRX_IS890_FTAP_PN_SIG );
#endif /* FEATURE_RUMI3_BRINGUP */
    }
#endif /* FEATURE_HDR_IS890 or FEATURE_HDR_IS890A */

    /*-----------------------------------------------------------------------
     Event queue signal. Clear signal and handle all events on event queue.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRRX_EVENT_Q_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_EVENT_Q_SIG );

      while( q_cnt( &hdrrx.event_q ) != 0 )
      {
        hdrrx_buf_ptr = (hdrrx_buf_type*) q_get( &hdrrx.event_q );
        hdrrx_protocol_item( hdrrx_buf_ptr );
        modem_mem_free( hdrrx_buf_ptr, MODEM_MEM_CLIENT_HDR_CRIT );
      }
    }

    if ( ( rex_signals_mask & HDRRX_MSG_Q_SIG ) != 0 )
    {
      hdrrx_msg_s_type *msg_ptr;

      (void) rex_clr_sigs ( HDRRX_TASK_PTR, HDRRX_MSG_Q_SIG );

      while ( q_cnt ( &hdrrx.msg_q ) != 0 )
      {
        msg_ptr = (hdrrx_msg_s_type*) q_get ( &hdrrx.msg_q );

        hdrrx_process_msg ( msg_ptr );

        msgr_rex_free_msg_buf (&(msg_ptr->link));
      }
    }

    /*-----------------------------------------------------------------------
     Missed sync CC signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_SYNC_CC_LOST_SIG) != 0 )
    {
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_SYNC_CC_LOST_SIG );

      hdrind_give_ind( HDRIND_LMAC_CC_MISSED, NULL);
    }

    /*-----------------------------------------------------------------------
     Data queue signal. Clear signal and handle all packets on data queue.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRRX_PKT_READY_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_PKT_READY_SIG );


      while ( hdrdec_get_next_pkt ( &delivered_pkt_ptr, 
                                    &delivered_pkt_status, 
                                    &delivered_pkt_slot_time ) )
      {

        /* For Bad CRC and Cancelled packets, skip this iteration 
           and continue */
        if ( ( delivered_pkt_ptr == NULL ) ||
             ( delivered_pkt_status == HDRDEC_PKT_CANCELED ) )
        {
          continue;
        }

        if ( log_status ( LOG_HDR_FL_PHY_LAYER_PKT_C ) )
        {
          hdrrxstats_log_fl_phy_layer_pkt ( delivered_pkt_ptr );
        }

        /* Deliver the packet to upper layers */
        switch ( delivered_pkt_status )
        {
          case HDRDEC_CC_PKT_READY:
          {
#ifdef FEATURE_FACTORY_TESTMODE
            if ( ( ftm_get_mode() == FTM_MODE ) &&
                 ( hdrhitmsg_get_fwd_curr_state() != HDRHITMSG_HDR_FTM_SYNC_STATE )

#ifdef FEATURE_HDR_RUMI
                 &&
                 ( hdrhitmsg_get_fwd_curr_state() != HDRHITMSG_HDR_FTM_PIL_ACQ )
#endif /* FEATURE_HDR_RUMI */
                 )
            {
              dsm_free_packet ( &delivered_pkt_ptr );
            }
            else
#endif /* FEATURE_FACTORY_TEST_MODE */
            {
#ifdef FEATURE_HDR_QTA
                 /* Received CC Pkt after tuned away. Ignore and free pkt */
               if ( hdrsrch_get_current_state() == HDRSRCH_SUSPENDED_TC_STATE )
               {
                  dsm_free_packet ( &delivered_pkt_ptr );
                  HDR_MSG_PROT( MSG_LEGACY_MED, 
                                  "CC Pkt Rcvd after TA - Ignored!");
               }
               else
#endif /* FEATURE_HDR_QTA */
               {
                  hdrcmac_put_pkt ( delivered_pkt_ptr );
                   /* If AT is idling, */
                  if ( hdrfmac_is_active ( HDRHAI_RX ) == FALSE )
                  {
                     hdrdec_log_pkt_headers ( TRUE );
                       /* Log packet headers immediately */
                  }
               }
            }
            break;
          } /* Case HDRDEC_CC_PKT_READY */

          case HDRDEC_FTC_PKT_READY:
          {
#ifdef FEATURE_HDR_RUMI
            if ( delivered_pkt_ptr != NULL )
            {
              dsm_extract( delivered_pkt_ptr, 0, data_pattern, 2 );
                /* extract the data pattern (first 2-byte) */

              /* CSIM sets a 2-byte fixed data pattern for the entire FTC pkt.
                 We grab the first 2 bytes as data pattern and try to match
                 the entire FTC pkt, printing out any mismatch */
              HDR_MSG_PROT_2( MSG_LEGACY_MED, "FTC data len:%d pattern:%x", 
                              delivered_pkt_ptr->used,
                              ( (data_pattern[1] << 8) | data_pattern[0] ) );

              for (i=2; i<delivered_pkt_ptr->used; i+=2)
              {
                dsm_extract( delivered_pkt_ptr, i, data, 2 );

                if ( ( data[0] != data_pattern[0] ) ||
                     ( data[1] != data_pattern[1] ) )
                {
                  HDR_MSG_PROT_2( MSG_LEGACY_MED, "Pkt data mismatch: 0x%x@%d", 
                                  (data[1] << 8 | data[0]), i );
                  /* Print out unmatched data. The last byte (if odd) is expected to mismatch */
                }
              }
            }
#endif /* FEATURE_HDR_RUMI */

#ifdef FEATURE_FACTORY_TESTMODE
            if ( ftm_get_mode() == FTM_MODE )
            {
              dsm_free_packet ( &delivered_pkt_ptr );
            }
            else
#endif /* FEATURE_FACTORY_TESTMODE */
            {
              if( hdrrx.hdrfmac_put_pkt_cb != NULL )
              {
#ifdef FEATURE_HDR_QTA
                  /* Received FTC Pkt after tuned away. Ignore and free pkt */
                if ( hdrsrch_get_current_state() == HDRSRCH_SUSPENDED_TC_STATE )
                {
                  dsm_free_packet ( &delivered_pkt_ptr );
                  HDR_MSG_PROT( MSG_LEGACY_MED, 
                                  "FTC Pkt Rcvd after TA - Ignored!");
                }
                else
#endif /* FEATURE_HDR_QTA */
                {
                  hdrrx.hdrfmac_put_pkt_cb ( delivered_pkt_ptr );
                    /* Call upper layer with FTC chain */

                  ftc_pkt_received = TRUE;
                    /* Mark packet received. */
                }
              }
              else
              {
                HDR_MSG_PROT (MSG_LEGACY_HIGH,  "Ignoring HDRDEC_FTC_PKT_READY, cb not registered!");
                dsm_free_packet ( &delivered_pkt_ptr );

              }
            }
            break;
          } /* Case HDRDEC_FTC_PKT_READY */

#ifdef FEATURE_HDR_BCMCS
          case HDRDEC_BCC_PKT_RXD:
          {
            hdrbcmac_queue_pkt ( delivered_pkt_ptr, delivered_pkt_slot_time );
              /* Send (queue) BCC packets to MAC for processing. Note that for 
                 broadcast, packet status is set to BCC even when we received 
                 a failed CRC packet.  (pkt_buffer will be NULL). */

            break;

          } /* Case HDRDEC_BCC_PKT_RXD */
#endif /* FEATURE_HDR_BCMCS */

          default:
          {
            HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, "Unexpected packet type: %d",
                             delivered_pkt_status );
            dsm_free_packet ( &delivered_pkt_ptr );
            break;
          } /* Case Default */

        } /* Switch on pkt type */


        delivered_pkt_ptr = NULL;
        delivered_pkt_status = HDRDEC_PKT_WAITING;
        delivered_pkt_slot_time = -1;

      } /* While there are more packets */

      /* If FTC packet(s) were received, now call to signal the 
         batch of packet has been delivered. */
      if ( ftc_pkt_received )
      {
        hdrpcp_rx_buf_empty();
        ftc_pkt_received = FALSE;
      }

      hdrdec_ship_aged_log_buffer();
        /* Ship aged log buffers */
    }

    /*-----------------------------------------------------------------------
     Log buffer ready signal. Clear signal and log the buffer.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_LOG_BUFFER_READY_SIG) != 0 )
    {
      (void) rex_clr_sigs ( HDRRX_TASK_PTR, HDRRX_LOG_BUFFER_READY_SIG );
      hdrdec_log_mc_pkt_headers();
    }

    /*-----------------------------------------------------------------------
     Exit HDR signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRRX_EXIT_HDR_SIG) != 0 )
    {
#ifdef FEATURE_LTE_TO_HDR_OH
      if ( hdrsap_tunnel_mode_is_enabled( HDRHAI_RX ) )
      {
        /* Disable tunnel mode when exiting HDR. */
        hdrsap_set_tunnel_mode( HDRHAI_TUNNEL_MODE_DISABLED );
      }
      else
#endif /* FEATURE_LTE_TO_HDR_OH*/
      {
        /* Set signal to HDR decoder task to exit HDR online mode */
        hdrdec_tcb_ptr = rcinit_lookup_rextask("hdrdec");
        ( void ) rex_set_sigs( hdrdec_tcb_ptr, HDRDEC_EXIT_HDR_SIG );
      }

      HDR_MSG_PROT (MSG_LEGACY_MED,  "HDRRX_EXIT_HDR_SIG received");
      (void) rex_clr_sigs( HDRRX_TASK_PTR, HDRRX_EXIT_HDR_SIG );
      exit_hdr_mode = TRUE;
    }

  } /* while ( !exit_hdr_mode ) */

  hdrrx_exit_processing();

} /* hdrrx_subtask */


/*============================================================================
                        EXTERNAL FUNCTION DEFINITIONS
=============================================================================*/

/*============================================================================
  HDRRX_QUEUE_CMD
*/
/**
  This function queues a command on the hdrrx.event_q and sets the signal.
  The command data is copied into the command buffer before it is queued.
  This function is called in the task context of whoever is giving the command.

  @dependencies This routine must be multi-thread safe, as two different tasks 
  may give commands to protocols in the RX task at the same time. cmd_size must 
  not be larger than hdrrx_protocol_cmd_union_type, because this is the data 
  type the command gets copied into.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_queue_cmd
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Who the command is for? */
  void *input_cmd_ptr,
    /**< Pointer to a union of all protocols in HDRRX subtask */
  uint32 cmd_size
    /**< Size of the command being given */
)
{
  hdrrx_buf_type *buf_ptr;         
    /* buffer for command */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( input_cmd_ptr == NULL )
  {
    ERR( "No command to queue!", 0, 0, 0 );
    return;
  }

  ASSERT (cmd_size <= sizeof(hdrrx_protocol_cmd_union_type) );

  /* Get a buffer from the heap */
  if ( ( buf_ptr = ( hdrrx_buf_type * ) modem_mem_alloc( 
         sizeof( hdrrx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT ) ) 
         == NULL )
  {
    ERR( "No free memory in heap.", 0, 0, 0 );
    return;
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */
  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.type_of_item = HDRBUF_COMMAND;
#ifdef FEATURE_MEMCPY_REMOVAL
  (void) memscpy( &buf_ptr->item.cmd, sizeof(buf_ptr->item.cmd), 
                  input_cmd_ptr, cmd_size );
#else
  (void) memcpy( &buf_ptr->item.cmd, input_cmd_ptr, cmd_size );
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrrx.event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRRX_TASK_PTR, HDRRX_EVENT_Q_SIG );

} /* hdrrx_queue_cmd */


/*============================================================================
  HDRRX_QUEUE_MSG
*/
/**
  This function queues a message on the hdrrx.event_q and sets the signal.
  The queued buffer simply holds a pointer to the DSM item chain that is
  the message.  This function is called in the task context of whoever is
  giving the message, which is always the task in which SLP runs.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_queue_msg
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Who the message is for */
  hdrhai_protocol_instance_enum_type protocol_instance,
    /**< In-config or In-use instance of the protocol */
  dsm_item_type *item_ptr
    /**< Pointer to DSM item chain holding the message */
)
{
   hdrrx_buf_type *buf_ptr;           
    /* Buffer for msg (w/ptr to DSM item */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get a buffer from the heap */
  if ( ( buf_ptr = ( hdrrx_buf_type * ) modem_mem_alloc( 
    sizeof( hdrrx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT ) ) 
       == NULL )
  {
    ERR( "No free memory in heap.", 0, 0, 0 );
    return;
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */
  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.protocol_instance = protocol_instance;
  buf_ptr->hdr.type_of_item  = HDRBUF_MESSAGE;
  buf_ptr->item.item_ptr     = item_ptr;

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrrx.event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRRX_TASK_PTR, HDRRX_EVENT_Q_SIG );

} /* hdrrx_queue_msg */


/*============================================================================
  HDRRX_QUEUE_TIMER
*/
/**
  This function queues a timer event on the hdrrx.event_q and sets the
  signal.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_queue_timer
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Who the timer is for */
  uint32 timer_id
    /**< Unique identifier for the timer that has expired */
)
{

  hdrrx_buf_type *buf_ptr;           
    /* Buffer for timer event */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get a buffer from the heap */
 if ( ( buf_ptr = ( hdrrx_buf_type * ) modem_mem_alloc( 
        sizeof( hdrrx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT ) ) 
        == NULL )
 {
   ERR( "No free memory in heap.", 0, 0, 0 );
   return;
 }


 /* Fill in the buffer.  Make sure all fields are filled in. */
 buf_ptr->hdr.protocol_name = protocol_name;
 buf_ptr->hdr.type_of_item  = HDRBUF_TIMER;
 buf_ptr->item.timer_id     = timer_id;

 /* Queue the buffer and set a signal */
 ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
 q_put( &hdrrx.event_q, &buf_ptr->hdr.link );
 ( void ) rex_set_sigs( HDRRX_TASK_PTR, HDRRX_EVENT_Q_SIG );

} /* hdrrx_queue_timer */


/*============================================================================
  HDRRX_QUEUE_IND
*/
/**
  This function queues an indication on the hdrrx.event_q and sets the
  signal.  If there is any data associated with the indication, it is copied
  into the buffer before it is queued.  This function is called in the task
  context of whoever is giving the indication.

  @dependencies This routine must be multi-thread safe, as two different tasks 
  may give an indication to protocols in the RX task at the same time. 
  ind_data_size must not be larger than hdrind_ind_data_union_type, because 
  this is the data type the command gets copied into.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_queue_ind
(
  hdrhai_protocol_name_enum_type protocol_name,
    /**< Who the indication is for */
  hdrind_ind_name_enum_type ind_name,
    /**< Name of the indication given */
  void *input_ind_data_ptr
    /**< Size of the data associated with this indication */
)
{
  hdrrx_buf_type              *buf_ptr;        /* Buffer for indication */
  uint32                       ind_data_size;  /* Size of data with ind */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* Get a buffer from the heap */
  if ( ( buf_ptr = ( hdrrx_buf_type * ) modem_mem_alloc( 
    sizeof( hdrrx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT ) ) 
       == NULL )
  {
    ERR( "No free memory in heap.", 0, 0, 0 );
    return;
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */

  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.type_of_item = HDRBUF_INDICATION;
  buf_ptr->item.ind.ind_name = ind_name;

  if ( input_ind_data_ptr != NULL )
  {
    ind_data_size = hdrind_get_ind_data_size (ind_name);
    ASSERT (ind_data_size <= sizeof(hdrind_ind_data_union_type) );
#ifdef FEATURE_MEMCPY_REMOVAL
    (void) memscpy( &buf_ptr->item.ind.ind_data, 
                    sizeof(buf_ptr->item.ind.ind_data), 
                    input_ind_data_ptr,
                    ind_data_size );
#else
    (void) memcpy( &buf_ptr->item.ind.ind_data, input_ind_data_ptr,
                   ind_data_size );
#endif /* FEATURE_MEMCPY_REMOVAL */
  }

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrrx.event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRRX_TASK_PTR, HDRRX_EVENT_Q_SIG );

} /* hdrrx_queue_ind */


/*============================================================================
  HDRRX_REGISTER_FMAC_PKT_CB
*/
/**
  FMAC uses this function to register a packet forwarding call-back routine 
  based on the negotiated FMAC subtype.  

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_register_fmac_pkt_cb
( 
  hdrrx_put_fmac_pkt_cb_type ptr_fmac_put_pkt 
    /**< Pointer to function used to forward the FTCMAC packet */
)
{
  hdrrx.hdrfmac_put_pkt_cb = ptr_fmac_put_pkt;

} /* hdrrx_register_fmac_pkt_cb */


/*============================================================================
  HDRRX_SET_CC_OFFSET
*/
/**
  Function to set the CC offset so that SyncCC's can be determined from
  HSTR value.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_set_cc_offset
( 
  int offset 
    /**< SCC Offset in half slots */
)
{
  hdrrx.scc_offset_in_half_slots = offset;
}


/*============================================================================
  HDRRX_TASK
*/
/**
  This function is the entry point to the HDR Receive task.  It contains the
  task event processing loop, which executes while waiting to enter HDR mode.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_task
(
  dword dummy              
    /**< Required for REX, ignore */
)
{
  rex_sigs_type rex_signals_mask;  
    /* Task signal mask.     */

  rex_tcb_type *hdrdec_tcb_ptr; /* temp ptr */

  hdrrx_msg_s_type *msg_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SYS_ARG_NOT_USED ( dummy );

  /*-------------------------------------------------------------------------
   Perform task initialization.
  -------------------------------------------------------------------------*/

  rcinit_handshake_startup();

  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */
  if ( rcevt_wait_name(HDRTX_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  if ( rcevt_signal_name(HDRRX_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  /* RCINIT calls init function defined in task scons file in rcinit context
     All tcb related intializations should be done after rcinit starts our
     task */
  hdrrx_tcb_ptr = rex_self();

  hdrrx_dog_rpt_var = dog_hb_register_rex( (rex_sigs_type) HDRRX_RPT_TIMER_SIG );
    /* Register with dog HB */

  hdrrx_create_efs_item_conf_file();

  /* Initialize message client and register for messages */
  hdrrx_msg_init();

  /*-------------------------------------------------------------------------
   Send the first watchdog report and set the timer for report interval.
  -------------------------------------------------------------------------*/
  hdrrx_process_wdog();

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  for ( ; ; )
  {
    rex_signals_mask = hdrrx_wait ( HDRRX_ENTER_HDR_SIG | 
                                    TASK_OFFLINE_SIG    | 
                                    TASK_STOP_SIG       |
                                    HDRRX_MSG_Q_SIG );

    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & TASK_STOP_SIG ) != 0 )
    {
      HDR_MSG_PROT ( MSG_LEGACY_MED,  "TASK_STOP_SIG received" );
      (void) rex_clr_sigs ( HDRRX_TASK_PTR, TASK_STOP_SIG );

     /* Deregister the registered messages and delete the messenger client */
      hdrrx_msg_stop();

    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & TASK_OFFLINE_SIG ) != 0 )
    {
      HDR_MSG_PROT ( MSG_LEGACY_MED,  "TASK_OFFLINE_SIG received" );
      (void) rex_clr_sigs ( HDRRX_TASK_PTR, TASK_OFFLINE_SIG );

    }

    /*-----------------------------------------------------------------------
     Enter HDR signal. Clear signal, and enter subtask.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRRX_ENTER_HDR_SIG ) != 0 )
    {
#ifdef FEATURE_LTE_TO_HDR_OH
      if (!hdrsap_tunnel_mode_is_enabled(HDRHAI_RX))
      {
#endif /* FEATURE_LTE_TO_HDR_OH*/
        /* Set signal to HDR decoder task to enter HDR online mode */
        hdrdec_tcb_ptr = rcinit_lookup_rextask("hdrdec");
        ( void ) rex_set_sigs( hdrdec_tcb_ptr, HDRDEC_ENTER_HDR_SIG );
#ifdef FEATURE_LTE_TO_HDR_OH
      }
#endif /* FEATURE_LTE_TO_HDR_OH*/

      HDR_MSG_PROT ( MSG_LEGACY_MED,  "HDRRX_ENTER_HDR_SIG received" );
      (void) rex_clr_sigs ( HDRRX_TASK_PTR, HDRRX_ENTER_HDR_SIG );

      hdrrx_subtask();
        /* Enter HDR mode processing subtask */

    }

    /*-----------------------------------------------------------------------
     Signal for MSGR. Ingore since HDRRX is offline.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRRX_MSG_Q_SIG ) != 0 )
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR, 
                    "Rx'ed HDRRX_MSG_Q_SIG when HDRRX is offline" );

      (void) rex_clr_sigs ( HDRRX_TASK_PTR, HDRRX_MSG_Q_SIG );

      while ( q_cnt ( &hdrrx.msg_q ) != 0 )
      {
        msg_ptr = (hdrrx_msg_s_type*) q_get ( &hdrrx.msg_q );
        msgr_rex_free_msg_buf (&(msg_ptr->link));
      }
    }

  } /* for (;;) */

} /* hdrrx_task */


/*============================================================================
  HDRRX_ENABLE_MISSED_SCC_DETECTION
*/
/**
  Enables missed sync CC detection. hdrcmac_handle_lost_scc() is called
  when a missed packet is suspected.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_enable_missed_scc_detection
(
  boolean clear_pending
    /**< Whether to clear any pending indication */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrcmac_enable_missed_scc_detection( HDRHAI_RX, clear_pending );

} /* hdrrx_enable_missed_scc_detection() */


/*============================================================================
  HDRRX_DISABLE_MISSED_SCC_DETECTION
*/
/**
  Disables missed sync CC detection.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_disable_missed_scc_detection ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrcmac_disable_missed_scc_detection( HDRHAI_RX );

} /* hdrrx_disable_missed_scc_detection() */


/*============================================================================
  HDRRX_GET_AVERAGE_PER
*/
/**
  Function to access average packet error rate.  The supplied structure
  is filled in with the measured sample size and the number of those packets
  that were in error.

  @context HDRRX task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_get_average_per
(
  hdrrx_average_per_type *per
    /* Pointer to location where the PER information will be filled */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrrxstats_get_average_per ( &per->meas_pkts, &per->err_pkts );

} /* hdrrx_get_average_per */

/*============================================================================
  HDRRX_RESET_DM_FWD_STATS
*/
/**
  This function resets all RX rate statistics and sends a log packet to the DM.

  @context HDRRX task. */
/*============================================================================*/
void hdrrx_reset_dm_fwd_stats ( void )
{
  hdrrxstats_reset_dm_fwd_stats ( );

} /* hdrrx_reset_dm_fwd_stats */

#ifdef FEATURE_HDR_BCMCS 
/*============================================================================
  HDRRX_CLEAR_BCC_STATS
*/
/**
  This function resets the broadcast channel statistics.
  - BCC early decode and
  - BCC rate CRC count

  @context HDRRX task. */
/*============================================================================*/
void hdrrx_clear_bcc_stats ( void )
{
  hdrrxstats_clear_bcc_stats ();

} /* hdrrx_clear_bcc_stats */
#endif /* FEATURE_HDR_BCMCS */

/*============================================================================
  HDRRX_RESET_CONNECTED_STATE_PER
*/
/**
  Reset packet error rate statistics maintained for recent traffic session.

  @context Calling task. 

  @sideeffects None */
/*============================================================================*/
void hdrrx_reset_connected_state_per ( void )
{

  hdrrxstats_reset_connected_state_per();

} /* hdrrx_reset_connected_state_per */

/*============================================================================
  HDRRX_GET_CONNECTED_STATE_PER
*/
/**
  Return packet error rate observed during current or recent traffic session.

  Upper layers expect support for PERs in range 0.01% to 8.0%. In order
  to support this range, PER is scaled up by 10000, so that the resulting 
  PER would be an integer for easy representation and also support the 
  expected range

  @context Calling task. 

  @return Connected State Packet error rate scaled up by 10000

  @sideeffects None */
/*============================================================================*/

uint16 hdrrx_get_connected_state_per ( void )
{

  return hdrrxstats_get_connected_state_per();

} /* hdrrx_get_connected_state_per */


/*============================================================================
  HDRRX_GET_CONNECTED_STATE_PER_DATA
*/
/**
  Return packet counts for bad and total traffic packets. Control channel
  is not counted.

  total_tc_packets and total_tc_errors must point to valid uint32 
    variable locations to be filled in.

  @context Calling task. 

  @return  Bad and total packet counts are filled in. 

  @sideeffects None */
/*============================================================================*/

void hdrrx_get_connected_state_per_data
(
  uint32 * total_tc_packets, 
    /**< Total traffic channel packets received. */

  uint32 * total_tc_errors
    /**< Traffic channel packets received with errors (failed CRC). */
)
{
  hdrrxstats_get_connected_state_per_data( total_tc_packets, 
                                           total_tc_errors );
} /* hdrrx_get_connected_state_per_data */


/*============================================================================
  HDRRX_GET_FL_SLOT_COUNT
*/
/**
  Returns the number of slots occupied by packet directed at this AT. All 
  TC packets (Rev 0, Rev A/B SU and MU), good or bad, contribute to this 
  count.

  @return Number of slots occupied by packets directed at this AT since the last 
  time the counter was cleared.

  @context Calling task. 

  @sideeffects Count is cleared if reset is TRUE */
/*============================================================================*/

uint32 hdrrx_get_fl_slot_count
(
  boolean reset
    /**< Count is reset if TRUE */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrrxstats_get_fl_slot_count(reset);
    /* Get value from hdrrxstats. */

} /* hdrrx_get_fl_slot_count */

/*===========================================================================
FUNCTION HDRRX_CREATE_EFS_ITEM_CONF_FILE

DESCRIPTION
  Put(append and add newline) item_file_path into conf_file_path
  A call to this function is necessary so that QPST can back up EFS items
  to QCN files. For HDR MAC, use "/nv/item_files/conf/hdrmac_config_info.conf"
  for conf_file_path.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
 None

SIDE EFFECTS
  None

NOTE
  Please list all the EFS/NV item nos. & description in the commnents below
===========================================================================*/
void hdrrx_create_efs_item_conf_file( void )
{
  int32              config_fd, result;

  char               new_line[] = "\n";
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 


  /* Open conf file. If it doesn't exist create it and reopen it. */
  config_fd = hdrutil_fs_open( HDRRX_EFS_NV_CONF_FILE_PATH, O_RDWR|O_APPEND );
  if ( config_fd >= 0  )
  {

    result = hdrutil_fs_close ( config_fd );
    return;
      /* File already exists, no need to put nv item into conf file */
  }
  else 
  {
    if ( ENOENT == hdrutil_fs_errno() ) /* Conf file does not exist */
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Creating EFS NV item config file");

      config_fd = hdrutil_fs_open( HDRRX_EFS_NV_CONF_FILE_PATH, 
                                   O_WRONLY|O_CREAT|O_TRUNC );

      if ( 0 > config_fd )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                        "Error creating EFS NV item config file errno %d",
                        hdrutil_fs_errno() );
        return;
      }

    }
    else /* Could not open conf file for some other reason */
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error opening config file errno %d", 
                      hdrutil_fs_errno() );
      return;
    }

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
    result = hdrutil_fs_write( config_fd, HDRRMAC_EFS_NV_DTX_CFG,
                               strlen( HDRRMAC_EFS_NV_DTX_CFG ) );
    result += hdrutil_fs_write( config_fd, new_line, 1 );
    if ( result != ( strlen( HDRRMAC_EFS_NV_DTX_CFG )+1 ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Error writing to config file errno %d", 
                      hdrutil_fs_errno() );
      result = hdrutil_fs_close( config_fd );
      return;
    }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) */


    result = hdrutil_fs_close ( config_fd );
    if ( 0 != result )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                      "Error closing newly created EFS NV item config file errno %d",
                      hdrutil_fs_errno() );
      return;
    }
  }
} /* hdrrx_create_efs_item_conf_file() */
