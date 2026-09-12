/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   L I N K   M A N A G E M E N T  A N D   C O N T R O L   P R O T O C O L

GENERAL DESCRIPTION
  The LMAC Protocol is the interface between the HDR protocol stack and the
  searcher.  In addition to wrapping command functions to the searcher and
  translating searcher callbacks into protocol indications, the LMAC also
  handles the details of handoff and frequency changing that are more
  appropriately performed at a higher priority than the protocol.

EXTERNALIZED FUNCTIONS (Global)

  hdrlmac_powerup_init() - LMAC powerup initialization
  hdrlmac_ind_cb() - Callback registered for delivery of indications
  hdrlmac_activate() - Command function to activate LMAC
  hdrlmac_deactivate() - Command function to deactivate LMAC
  hdrlmac_set_search_params() - Sets general search params from ovhd msgs
  hdrlmac_set_mgmt_same_channel_params() - Sets specific params for same chan
  hdrlmac_set_mgmt_diff_channel_params() - Sets specific params for other chans
  hdrlmac_set_cc_hash() - Sets our slot (not cycle) for the control channel
  hdrlmac_get_cc_hash() - Retrieves the current control channel slot value
  hdrlmac_acquire() - Command search to acquire a channel
  hdrlmac_synchronize() - Give sync message information to search
  hdrlmac_idle_mode() - Command search to enter idle mode
  hdrlmac_nset_update() - Give search updated neighbor list info
  hdrlmac_idle_chan_change() - Command search to change to a new channel
  hdrlmac_sleep() - Command search to sleep
  hdrlmac_access_mode() - Command search to enter access mode
  hdrlmac_perform_aho() - Command search to change pilot (durring access only)
  hdrlmac_pilot_report_reset() - Command search to reset pilot report info
  hdrlmac_tc_assign() - Give search info from TC Assignment message
  hdrlmac_chan_meas() - Command search to measure pilots on a list of channels
  hdrlmac_get_state() - Returns current HDR state
  hdrlmac_get_pilots() - Retrieves pilot strengths for Route Update (PSMM)
  hdrlmac_get_link_info() - Given a link ID, retrieves pilot PN, channel, etc.
  hdrlmac_get_rssi() - Returns the estimated HDR RSSI.  
  hdrlmac_advice_unlock_rf() - Advices search that 1x needs the RF lock
  hdrlmac_color_code_update() - Update search with the color code of the
                                current link
  hdrlmac_get_ecio_and_io() - Returns Ec/Io and Io for HDR
  hdrlmac_get_asp_pn_mapping() - Returns PN offsets and ASP indices of ASET pilots
  hdrlmac_get_pn_ecio() - Returns PN offset and correspond ecio of ASET pilots
  hdrlmac_update_bc_info - Updates HDRSRCH BCMCS status
  hdrlmac_set_bcmcs_page_cycle - Sets BCMCS page cycle
  hdrlmac_set_bcmcs_sleep_cycle - Sets BCMCS sleep cycle
  hdrlmac_set_tc_mode() - Set the TC mode (default or SHPT)
  hdrlmac_get_tc_mode() - Get the TC mode (default or SHPT)
  hdrlmac_disable_tc_ofs() - Disable the TC OFS at hdrsrch 
  hdrlmac_set_tap() - Indicates to L1 that TAP is active
  hdrlmac_rup_request() - Command searcher to process RUP request
  hdrlmac_clear_handoff_count() - Clears the handoff counter used for TAP
  hdrlmac_get_handoff_count() - Returns the handoff counter used for TAP
  hdrlmac_get_carrier_map() - Returns sub-activeset to carrier mapping for TAP
  hdrlmac_clear_pending_tca_queue() - Clears the pending tca queue

EXTERNALIZED FUNCTIONS (Regional)

  hdrlmac_process_cmd() - Function called by hdrtx task to process
                                a command for LMAC
  hdrlmac_process_ind() - Function called by hdrtx task to process
                                an indication for LMAC

CALLBACK FUNCTIONS REGISTERED WITH HDR SEARCH (not external, but interesting)

  hdrlmac_acquire_cb() - When a pilot is acquired
  hdrlmac_system_lost_cb() - When a system is lost
  hdrlmac_idle_ho_cb() - When the pilot has been autonomously changed
  hdrlmac_chan_change_rpt_cb() -When channel change is complete
  hdrlmac_wakeup_cb() - When waking up from sleep
  hdrlmac_suspend_ho_cb() - When access mode has been entered
  hdrlmac_request_aho_cb() - When a pilot change is needed during access
  hdrlmac_access_ho_rpt_cb() - When pilot change is complete during access
  hdrlmac_pilot_rpt_cb() - When a significant pilot event occurs
  hdrlmac_tc_assign_complete_cb() - When hard handoff has completed
  hdrlmac_chan_meas_cb() - Search results for given list of channels is ready
  hdrlmac_usable_cb() - When the HDR system usability changes

CALLBACK FUNCTIONS REGISTERED WITH HDR RMAC
  hdrlmac_rl_update_complete_cb() - When the rl update is completed

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrlmac.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/19/18   svu     Added hdrlmac_has_chain() API
02/01/18   rmv     Cleared the Deactivate Done sanity timer unconditionally
                   upon receiving the deactivate complete from SRCH 
01/30/18   vko     Add sanity timer for deactivate done procedure in D2L redirection
01/30/18   vko     Debug F3 in LMAC deactivate done cb
10/29/17   vko     FR 29153: Support HDR/CDMA on the 2nd sub
09/04/17   vko     Force LTA if LTE subs is not present on DO sub (temp DDS)
08/03/17   vko     MSGR Instance ID to Variant ID change
12/14/15   rmv     Fix KW issues from checker ASSERT.WITH.SIDE.EFFECTS 
10/19/15   vko     Report TCA processing true to SRCH when RUP is in 
                   HDRRUPS_SYNCHRONIZE_CONNECTION_SETUP
07/23/15   vko     FR 29379 : Disable tuneaway during Qchat call
06/05/15   vko     Added support for D2L redirection
05/25/15   vko     Fixed race condition in system loss and access attempt
05/07/15   vko     Inform srch if D2L reselection is aborted. 
04/29/15   vko     Inform TCA processing status, to HDRSRCH during 
                   IDLE_MODE_CMD
03/10/15   sjj     FR26262: LTE-U band support
02/25/15   wsh     Replaced efs API with mcfg version
02/02/15   vko     Pass srch window param during fastacq during cgi req
04/07/14   vke     Changes to prevent processing of HDRLMAC_FINISH_AHO_CMD
                   when hdrovhd is not in HDROVHDS_ACC_HO_IN_PROG state
10/06/14   wsh     Fixed compile warning related to ASDiv 
09/09/14   vke     Redesign fix for connection close during QTA gap
08/22/14   vke     Delay connection close during QTA gap
08/13/14   arm     Enabled QTA by default.
08/12/14   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold 
08/07/14   mbs     Added support for trm lock exchange during IRAT procedures
08/01/14   vko     Ignore fast connect TCA, when tune away already started
07/29/14   tnp     FR22375: Fix Medium compiler warnings in HDR
07/16/14   arm     Added NV control to enable/disable QTA.
07/09/14   cnx     Support NV refresh without reset.
06/25/14   wsh     Fixed compile issue on Jolokia due to missing FW API 
06/09/14   kss     Added support for handling TX_RESET_REQ_IND processing.
05/14/14   sat     Fixed Klocwork errors
02/26/14   vko     FR 17879: Extended EARFCN
01/15/14   wsh     Added DSDS ASDiv support
05/05/14   cnx     Update NSET and IDLE channel change in one command.
04/03/14  kss/wsh  Added T2P boost for RTT reduction
04/07/14   vke     Report different indication to CP when searcher moves from 
                   3a to 3f when compared with 3a to 3
03/14/14  vko/dsp  FR19338:Reduce call setup time by skipping RF Tx shutdown.
07/19/13   arm     Merged DSDS feature for triton. 
01/07/14   sat     Enabling LRE feature if NV read fails.
11/29/13   sat     NV control for Lock Release Enhancement (LRE)
11/14/13   cnx     Check FT valid until TCAssign is complete.
10/30/13  kss/rmv  FR3272 - Modem API for Cost of Power estimate changes 
01/07/14   vko     LMAC need to handle HDRIND_RTCMAC_SHUTDOWN_COMPLETE to clear 
                   hdrlmac pending queue
12/23/13   cnx     Notify SRCH about Sector ID change.
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/27/13  dsp/cnx  FR17482 - Page blocking management.
10/01/13   rkc     Do not forward advise unlock RF to HDRSRCH
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
08/08/13   vke     Added handling of sync substate callback
07/10/13   arm     Supported traffic DTX. 
06/28/13   cnx     Abort D2L if AdviseUnlock rx'ed during D2L resel.
05/06/13   vko     HDR CP not to go to IDLE unless HDR SRCH moves to MONITOR state
03/26/13   arm     Removed per band diversity bit, only use band mask now.
03/20/13   wsh     Moved NLB code out of SVDO feature and fixed SP parsing
03/15/13   cnx     Fix Triton compiler errors.
03/05/13   cnx     Replace NV reading errors with MSG_LEGACY_HIGH.
12/04/12   mbs     Fixed compilation errors due to removal of LTE on Triton
11/20/12   mbs     Fixed compilation errors for Triton build
10/05/12   ukl     Resolved compilation errors after removing FEATURE_BCMCS.
08/14/12   wsh     Reparse LoadInfo cache when ASET is updated
07/26/12   arm     Removed hdrsrch_set_dynamic_idle_diversity_pref. 
06/20/12   smd     Added Device only DTX and FlexCONN.
06/20/12   rmg     Fixed a crash due to race condition in processing of 
                   HDRIND_LMAC_ACCESS_MODE_ENTERED indication.
04/27/12   cnx     Avoid Reselection from eHRPD to LTE forbidden PLMNs.
03/23/12   cnx     Fixed getting sub ASET number bug in processing TC ASSIGNED cmd.
03/08/12   wsh     Merged NLB support
03/16/12   arm     Resolve TCB errors for Dime. 
03/02/12   cnx     Fixed bugs in processing TC_ASSIGNED_CMD.
02/27/12   smd     Added updates of FTM msgr interface changes. 
01/09/12   smd     Supported dynamic acquisition diversity.
12/14/11   rmg     Fixed a compiler warning generated by the last check in.
12/09/11   rmg     Added tx stop confirmation callback as an argument in 
                   hdrlmac_ta_stop_access_cb.
11/11/11   arm     HDR idle and access state diversity implemetation.
10/28/11   pba     Optimized handoff phase 2 support
10/10/11   arm     Supported second chain test call feature. 
09/24/11   cnx     Changed subtype checking for Optimized Handoff.
09/01/11   lyl     Mainlined hdrlmac_get_user_mac_debug_mask()
08/08/11   cnx     Fixed compiler error.
08/05/11   eme     Changes to support free floating threads.
06/01/11   cnx     Fixed compiler error.
05/31/11   cnx     Added hdrlmac_get_subpkt_protocol_data(). 
05/12/11   cnx     Removed sanity check ASSERT( tca_msg_ptr->num_sectors <= HDRSRCH_ASET_MAX_SIZE );
05/05/11   cnx     Added hdrlmac_get_band_subclass().
04/27/11   sju     Added changes to provide HDR revision to SRCH.
04/22/11   pba     Prune LTE TDD frequencies from OtherRATNL.
03/05/11   arm     New API added to provide Rx and Tx power info to CM. 
02/28/11   kss     Fix for compiler warning (variables never initialized).
11/18/10   kss     Added hdrlmac_get_user_mac_debug_mask(). 
09/02/10   pba     Added support for eHRPD to LTE reselection phase 1
08/26/10   cnx     Added Diag cmd to change diversity preference in runtime.
08/06/10   rkc     Added hdrlmac_get_state() and moved hdrlmac_state_enum_type
06/21/10   cnx     Fixed compiler warnings.
06/10/10   pba     IRAT Measurement feature.
06/07/10   pba     Fixed featurization
05/19/10   pba     Added support for Reverse Time Transfer.
03/24/10   pxu     Removed FEATURE_HDR_QSM features and FEATURE_HDR_EQ feature checks
02/05/10   wsh     Only provide avoid. list when related feature is defined 
03/08/10   wsh     Fixed compile error
03/04/10   wsh     Fixed double LMAC CMD queued crash
03/02/10   pba     Added support for LTE to HDR reselection
02/08/10   grl     Mainlined per band diversity NV control.
12/04/09   sju     Addd support for back to back TCAs. 
10/08/09   sju     Changed priority of debug message.
08/25/09   sju     Added support to handle back to back TCA commands.
07/22/09   sju     Passed the frame offset to HDR SRCH.
03/20/09   sju     Added rup_subtyp argument to hdrsrch_nset_update()
03/16/09   etv     Initialize decoder clock speed based on AT's capability 
                   for DORB
03/23/09   wsh     CMI: featurized CMI under FEATURE_CMI
03/16/09   sju     Fixed SofterHO value in non-rectangular sub-aset case.
03/10/09   rmg     Changes to provide subaset reportable status to RMAC.
02/10/09   kss     Added missing break statement.
02/10/09   sju     Removed unnecessary f3 message.
01/27/09   sju     Added support for intra scc sleep.
12/18/08   sju     Updated f3 message.
12/10/08   smd     Replaced assert.h with amssassert.h
12/05/08   ljl     Added hdrlmac_acmac_disable_ta().
12/05/08   kss     Lint fix.
12/04/08   rmg     Added changes to sync TCA processing by SRCH and MAC.
08/15/08   wsh     Fixed crash if TCA contains invalid channel
7/30/08    sju     Added support for sorting rev channels in ascending order
07/01/08   ljl     Added functions to get rx/tx frequency.
06/11/08   cc      Added aux_drc_cover.
06/09/08   etv     Used sys_channel_type instead of uint16 for carrier_map type
05/14/08   etv     Return num subactive sets from hdrlmac_get_carrier_map.
05/09/08   sju     Added support to remove avoided channels from NSET
04/18/08   mpa     Cleaned up mode from hdrsrch_nset_pilot_type struct
04/15/08   mpa     Updated hdrlmac_nset_update() to override pgid for rev0/A
02/22/08   mpa     Removed FEATURE_HDR_REVB_7500 + fixed warnings
02/20/08   ljl     Removed the parameter from hdrlmac_get_rssi().
02/15/08   wsh     Fixed rare crash caused by sending back-to-back AC msg in 
                   PSIST_NO_ACCCESS scenario
01/30/08   jyw     Added return value for hdrlmac_create_srch_subactive_set.
                   Obseleted hdrsrch_if_channel_change.
12/21/07   cws     Added support for HDRIND_LMAC_CARRIER_MAP_CHANGED for MCTAP.
12/18/07   sju     Fixed memory copy error in HHO case
11/20/07   sju     Fixed rmac rl_chan_index 
10/31/07   sju     Added FEATURE_HDR_REVB_7500
10/08/07   sju     Stored fmac tca params into static memory for consistency. 
                   Updated demod idx based on info provided by srch.
10/04/07   cws     Renamed parameters for consistency.
10/02/07   cws     Added way to invalidate a carrier in get_carrier_map.
09/21/07   kss     Include file cleanup.
09/18/07   wsh     Exported srch function to report diversity_comb mode
09/17/07   cws     Added get_carrier_map and abstracted some funcs for TAP.
09/07/07   sju     Changed hdrlmac_create_channel_list()
08/23/07   etv     Cleared high lint warnings.
08/23/07   mt      HDR per band diversity changes.
08/23/07   rkc     Read hdrsrch dbg array from NV.
08/20/07   etv     Used FMAC interface instead of srchstub.
08/23/07   pba     Do not do DDARF for rude wakeups
08/15/07   sju     Added support for revB FMAC interface and HHO
08/08/07   mpa     Added hdrlmac_get_tc_mode(). 
07/23/07   sju     Added support for scheduler group changed cb
07/18/07   sju     Added hdrlmac_get_schdgrp_info() 
07/13/07   sju     Added support for RevB phase1.5
04/27/07   grl     Removed tc_mode from TC assignment for access hybrid ctrl.
04/10/07   mt      Added hdrlmac_rup_request().
03/20/07   pba     DDARF update.
03/03/07   rmg     Added overhead channel gains compensation
12/05/06   dna     Changed command buffer queue size back to 1
10/20/06   kss     Update CC MAC with current link id on handoff and sync.
10/06/06   grl     Added passing of the negotiated SCI value to search.
10/03/06   ljl     Called hdrlmac_ta_time_cb() and 
                   hdrlmac_ta_stop_access_cb() without checking LMAC state.
09/25/06   ljl     In the function hdrlmac_ta_stop_access_cb(),
                   only stop ACMAC if LMAC is in access state.
09/19/06   pba     Added support for DDARF.
09/14/06   hal     Changed command buffer queue size to 2 and removed ASSERT
09/05/06   sq      Added new event (HDRTRACE_LMAC_UPDATE_QSM_STATUS)
08/28/06   sq      Added FEATURE_HDR_QSM
07/13/06   mt      Added support for hdrl1_dbg_msk NV item
07/12/06   mpa     Pass in system loss reason through ind_data union.
06/06/06   ljl     Added support for hybrid access
05/22/06   ljl     Added hdrlmac_set_tap().
05/18/06   mpa     Moved sd_ss_ind_hdr_opr_start|stop_activity() to hdrbccp.
03/06/06   sq      Added hdrlmac_set_bcmcs_sleep_cycle
02/16/06   jyw     Added command routing to disable the TC ofs for TAP calls.
01/15/05   jyw     Added support for the query of the exist of multiple rx chain.
12/15/05   mpa     Added channel_change_in_sleep flag to idle channel change cmd.    
11/08/05   dna     SD API change to fix 1x acq throttling during BCMCS idle
09/10/05   grl     Added dynamic TC mode support for HPTS mode.
09/06/05   mpa     Added event for hdrlmac_set_bcmcs_page_cycle().
08/28/05   mpa     Added Broadcast Trace Events
07/28/05   sq      Added hdrlmac_set_bcmcs_page_cycle
07/11/05   dna     Report channel list and cached sectors with neighbor list.
07/07/05   dna     Added support for Enhanced Idle State protocol
           dna     Add support for Rel. A TCA message fields
04/21/05   ds      Added support for equalizer preference NV item
01/13/05   sq      Mainlined FEATURE_HDR_NO_HO_DURING_SCC
11/29/04   dna     Count AEHO as AHO to avoid infinite handoffs.
                   Fix AEHO to return LMAC/SRCH to idle state and not give
                   AEHO indication unnecessarily (i.e. after AHO) 
11/29/04   grl     Added support for connected state stat logging.
11/03/04   mpa     Removed dependency on std.h (merged with sys.h)
08/30/04   aaj     Used enum type for bcmcs status 
07/30/04    sq     Added function hdrlmac_update_bc_info.
06/30/04   dna     hdrsrch_deactivate() now has a callback when its done
06/09/04    sq     Added function hdrlmac_get_pn_ecio().
06/08/04    sq     Added function hdrlmac_get_asp_pn_mapping().
04/13/04   dna     Wait to determine one way or the other if we need to enter
                   the suspend state before deciding to sleep.
04/02/04    sq     Returns Ec/Io and Io for HDR
02/27/04   mpa     Added hdrlmac_color_code_update().
12/31/03    sq     Removed redundant FTM code
11/10/03    km     Added hdrlmac_usable_cb()
11/05/03   mpa     Added argument to network lost event
10/13/03    km     Added function hdrlmac_advice_unlock_rf
09/10/03   mpa     Converted F3 messages to use new MSG2.0 PROT SSID
04/04/03   kss     Reenable handoff in both idle and access states when 
                   MISSED or END_OF_SYNC indications are received.
01/30/03   mpa     Give new reselect ind. instead of network lost indication
                   on reselection
01/29/03   kss     Added support to enable handoff after SCC has been 
                   accounted for (Rx'd or missed).
01/23/03   aaj     Added support for data activity indicator
01/02/03   mpa     Added reason argument to system_lost_cb
12/10/02   dna     Added support for HDRIDLES_SUSPEND_STATE
10/29/02   sq      Added function hdrlmac_get_rssi().
10/28/02   aaj     Changed hdrsrch acquisition command interface
08/26/02   kss     Make sure that the RTC frame offset is set before RTC MAC 
                   is told to start Tx.
08/21/02   mpa     Give ACCESS_ENTRY_HANDOFF ind in process_aho_cmd and
                   process_idle_chan_change_cmd
08/01/02   dna     Prevent handling an indication in interrupt context.
07/26/02   dna     New interface to inform srch of tc_established after tcc.
07/10/02   dna     Give a threshold to hdrsrch to use for access handoff, 
                   incrementing by 1.5 dB for each consecutive handoff up 
                   to three.  Don't allow more than 3 consecutive AHOs.
05/06/02   dna     Whether or not to tune away to 1x during HDR connection
04/16/02   dna     Take sleep duration as a parameter to hdrlmac_sleep()
04/16/02   dna     Use current link info if link_id is invalid
02/28/02   aaj     Add reading searcher diversity NV item in lmac powerup init
02/06/02   ajn     Added NULL callbacks to LMAC
01/17/02   kss     Call hdrsrch_tc_ack_received() on link acquired.
10/01/01   aaj     Corrected typo in get_link_info in MSG_LOW param syntax
09/21/01   vas     Changed lmac_get_link_info to not ERR_FATAL if the link_id
                   is the reference link id.
09/11/01   dna     Support new sleep interface to HDRSRCH
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/26/01   kss     Use renamed DRC supervision indications, use renamed
                   hdramac_stop() function.
08/20/01   vas     Printing link_id if ERR_FATAL while getting link.For debug
08/18/01   sh      Added protocol logging support
06/22/01   dna     Add call to deactivate SRCH on deactivate command.
06/07/01   kss     Added frame offset parameter to hdrfmac_set_channel_params
                   call.
05/08/01   dna     Re-naming for consistency.
                   Added pilot_pn to access HO and idle HO events
                   Added hdrrmac and hdrfmac calls to give TC Assign params.
                   Change tx_tcb to hdrtx_tcb
02/05/01   dna     ERR_FATAL if link info can't be retrieved now that searcher
                   supports link_id.
01/16/01   dna     Removed hdrlmac_set_params
01/09/01   kss     Changed hdramac_is_active() call to hdramac_is_in_attempt()
                   due to ACMAC interface changes.
09/08/00   dna     Created module

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "fs_lib.h"
#include "hdr_variation.h"
#include "comdef.h"
#include "target.h"
#include "dsm.h"
#include "hdrdebug.h"
#include "err.h"
#include "amssassert.h"
#include "memory.h"
#include "task.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#include "hdrhai.h"
#include "hdrbuf.h"
#include "hdrdec.h"
#include "hdrtx.h"
#include "hdrlmac.h"
#include "hdrlmaci.h"
#include "hdrcmac.h"
#include "hdrcmaci.h"
#include "hdramac.h"
#include "hdrfmac.h"
#include "hdrrmac0.h"
#include "hdrutil.h"
#include "hdrind.h"
#include "hdrtrace.h"
#include "queue.h"
#include "hdrsrchtypes.h"
#include "hdrsrch.h"
#include "hdrsrchutil.h"
#include "sys.h"
#include "hdrovhds.h"
#include "hdrrups.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif

#include "hdrplog.h"
#include "hdrscmgmmcdp.h"
#include "hdrscp.h"

#ifdef FEATURE_LTE_TO_HDR
#include "hdrcp_msg.h"
#endif

#ifdef FEATURE_HDR_TO_LTE
#include "hdralmps.h"
#include "hdralmp.h"
#include "lte_rrc_ext_api.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrmc_v.h"

#ifdef FEATURE_HDR_REVC
#include "hdrsrchset.h" /* hdrsrchset_get_cc_monitoring_chan(), etc */
#include "hdrfmac.h"    /* hdrfmac_is_loadinfo_supported() */
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_DUAL_SIM
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_DUAL_SIM */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/* <EJECT> */
/*===========================================================================

             DEFINITIONS  FROM  THE  HDR  AIR  INTERFACE  SPEC

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                 DEFINITIONS  FOR MESSAGE PARSING

===========================================================================*/


/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

typedef enum
{
  HDRLMAC_AHO_TIMER   /* Time between access handoff and next access */
  #ifdef FEATURE_HDR_MODEM_API
  ,HDRLMAC_MODEM_API_TIMEOUT_TIMER /* Timeout to invalidate modem API values */
  #endif /* FEATURE_HDR_MODEM_API */
  ,HDRLMAC_DEACTIVATE_DONE_TIMER  /* Timeout for deactivate done */
} hdrlmac_timer_enum_type;

/* If we perform AHO but do not start another access within this timeout,
 * we assume any subsequent access is a new access and not a continuation
 * of the old one.
 */
#define HDRLMAC_AHO_TIMEOUT 100
#define HDRLMAC_DEACTIVATE_DONE_TIMEOUT 100

#ifdef FEATURE_HDR_MODEM_API
/* Timeout..if API updates are not triggered within this period of time, 
   invalidate the stale values.  */
#define HDRLMAC_MODEM_API_UPDATE_TIMEOUT_MS 6000
#endif /* FEATURE_HDR_MODEM_API */

/* By default enable all diversity bands */
#define HDRLMAC_DEFAULT_DIV_BAND_MASK 0xFFFFFFFF

typedef struct
{
  hdrbuf_hdr_type hdr;         /* header */
  hdrlmac_cmd_type cmd; /* command */
} hdrlmac_cmd_buf_type;

/* TCA info passed by RUP */
typedef struct
{
  boolean initial_assignment;
  hdrsrch_link_id_type link_id;
  hdrsrch_traffic_tune_away_enum_type  tc_tune_away;
  uint8 cc_subactive_set_idx;
  hdrrup_tca_msg_type tca_msg;
  hdrhai_protocol_name_enum_type sender;
  uint16 rup_subtype;
} hdrlmac_rup_tca_params_type;

typedef struct
{
  hdrbuf_hdr_type hdr;         /* header */
  hdrlmac_rup_tca_params_type rup_tca_params; /* TCA parameters passed by RUP */
} hdrlmac_tca_buf_type;

/* Currently there can only be one command waiting to be processed.
 * This is because the upper layer will not try to change the channel
 * during a pilot change and the LMAC will ignore a pilot change while
 * waiting to change channels.  Also, a TC Assignment received during
 * a channel or pilot change overrides both.
 */

#define HDRLMAC_CMD_BUFS_NUM 1
#define HDRLMAC_TCA_BUFS_NUM HDRSRCH_MAX_NUM_CARRIERS

struct
{
  hdrlmac_cmd_buf_type cmd_pool[HDRLMAC_CMD_BUFS_NUM];
    /* Command pool for pending commands while access is occurring */

  q_type pending_cmd_q;
    /* Queued command queue */

  q_type pending_cmd_free_q;
    /* Free command queue */

  hdrlmac_tca_buf_type tca_pool[HDRLMAC_TCA_BUFS_NUM];
    /* Statically allocated space for free queue buffers */

  q_type pending_tca_q;
    /* Queue for pending TCA */

  q_type pending_tca_free_q;
    /* Free queue for pending TCA */

  hdrlmac_state_enum_type     state; 
    /* LMAC's opinion of HDRSRCH state */

  rex_timer_type              aho_timer; 
    /* Time between access handoff and next access */

  rex_timer_type				deactivate_done_timer;
    /* Time between deactivate and deactivate done from SRCH */

  int                         aho_count;            
    /* Number of consecutive access handoffs */

  int                         debug_aho_count;      
    /* Debug counter of total access handoffs */

  int                         debug_aho_amac_stop_count;  
    /* Debug counter of stopping ACMAC */

  uint8                       num_sub_asets;   
    /* Current number of subactive sets*/

  hdrsrch_subactive_set_type srch_subactive_set[HDRSRCH_MAX_NUM_SUBASETS];
    /* Subactive set storage for srch */

  hdrfmac_tcam_params_type fmac_tca_params;  
  /* Storage for fmac parameters from TCA */

  hdrlmac_rmac_params_type rmac_params;
    /* Storage for rmac parameters from TCA */

  boolean l1_is_processing_tca;
    /* Flag to indicate L1 layer is processing TCA */
    
#ifdef FEATURE_HDR_IS890
  hdrlmac_carrier_map_type    carrier_map[HDRSRCH_MAX_NUM_SUBASETS];
    /* Subactive set to carrier and channel mapping for TAP */
#endif /* FEATURE_HDR_IS890 */

  uint16 rup_subtype;
    /* RUP subtype - MC or Default RUP */

  rex_crit_sect_type              crit_sect; 
    /* Critical Section */

  uint64 mac_debug_mask;
    /* Local copy of mac debug mask from NV */

  boolean mac_debug_mask_valid;
    /* Valid flag for copy of debug mask */
    
 #ifdef FEATURE_HDR_MODEM_API
  rex_timer_type modem_api_timeout_timer;
    /* Timer for updating modem api info during a connection. */

  hdrlmac_modem_api_data_type last_valid_modem_api[2];
    /* Store most recent modem api data in a double-buffer array. */

  uint8 valid_api_buf;
    /* At any given time, the buffer from which to read  
       valid modem api data. */

  boolean awake_flag;
    /* Helper to track when LMAC first wakes. */

  boolean connection_open;
    /* Tracks connection state for Modem API */

#endif /* FEATURE_HDR_MODEM_API */

} hdrlmac;

/* This macro determines the Access Handoff Threshold level during an 
 * access attempt. For each access handoff, the access handoff count is
 * incremented. The Access Handoff threshhold is calculated based on the
 * number of consecutive access handoffs that have occurred without the 
 * AT getting a probe through.
 */
#define HDRLMAC_CALC_ACCESS_HO_THRESHOLD_HALF_DB( xx_access_ho_cnt ) \
   ( 6 + ( 3 * (xx_access_ho_cnt) ) )

/* Maximum number of consecutive Access Handoffs allowed */
#define HDRLMAC_MAX_AHO_COUNT 3

/* <EJECT> */

#define HDRLMAC_EFS_NV_DTX_CFG "/nv/item_files/modem/hdr/mac/hdr_dtx_cfg"

/* bit 0 is to disable device only dtx */
/* bit 1 is to disable flexconn */
/* bit 2 is to disable/enable  tDTX */
/* bit 3 is to disable QTA */
#define HDRLMAC_EFS_NV_QTA_DISABLED       0x08

#ifdef FEATURE_HDR_MODEM_API
/*=========================================================================

FUNCTION     : HDRLMAC_GET_MODEM_API_DATA

DESCRIPTION
  Retrieves Modem API data.
  During connection, data is collected at that instant.
  In idle, data collected is from last wakeup.
  Can be called from any task.
 
DEPENDENCIES
  None

INPUTS
  Structure with which to populate the Modem API data values
 
RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrlmac_get_modem_api_data
(
  hdrlmac_modem_api_data_type * modem_api_data
)
{
  hdrlmac_cost_of_modem_power_enum cost_of_modem_power;  

  uint8 valid_buf = hdrlmac.valid_api_buf;
    /* Values are double buffered. Holds the valid buffer index. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrlmac.connection_open )
  {
     /* When in Connected, Obtain the estimate at this instant */
     cost_of_modem_power = ( hdrlmac_cost_of_modem_power_enum ) hdrrmac_process_report_cost_of_modem_power_est_cmd();
  }
  else
  {
     /* When in Idle, Obtain the estimate from stored value
        Note: Estimate is collected and stored at every wakeup */
     cost_of_modem_power = 
       hdrlmac.last_valid_modem_api[valid_buf].cost_of_modem_power;
  }

  modem_api_data->cost_of_modem_power = cost_of_modem_power;

}

/*=========================================================================

FUNCTION     : HDRLMAC_RESET_MODEM_API_DATA

DESCRIPTION
  Resets Modem API data buffer. 
 
DEPENDENCIES
  Should be run in HDR Tx task

INPUTS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void hdrlmac_reset_modem_api_data( void )
{

  uint8 read_buf;
  uint8 write_buf;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Valid read buffer, we may peek at it. */
  read_buf = hdrlmac.valid_api_buf;

  /* This is the unused buffer, the one we are allowed to modify.
     (Operation just toggles between 1 and 0) */
  write_buf = 1 - read_buf;

  /* Update the unused buffer */ 
  hdrlmac.last_valid_modem_api[write_buf].cost_of_modem_power 
    = HDRLMAC_EST_COST_OF_POWER_IS_INVALID;

  /* Make unused buffer the valid buffer */
  hdrlmac.valid_api_buf = write_buf;
}

/*=========================================================================

FUNCTION     : HDRLMAC_MODEM_API_TIMEOUT

DESCRIPTION
  Timeout for Modem API updates has triggered (ie, no updates run).
 
DEPENDENCIES
  None.

INPUTS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void hdrlmac_modem_api_timeout
(
   hdrhai_protocol_name_enum_type sender
     /**< The protocol giving this command (only for software trace) */
)
{
  
  /* Build command into local struct */
  hdrlmac_cmd_type cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*-------------------------------------------------------------------------
   If not in the home task, queue the command for execution in the home task.
  -------------------------------------------------------------------------*/
  if ( rex_self() != HDRTX_TASK_PTR )
  {
    cmd.name = HDRLMAC_MODEM_API_TIMEOUT_CMD;
    cmd.sender = sender;
    hdrtx_queue_cmd( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL, 
                     &cmd, sizeof( cmd ) );
  }

  /*-------------------------------------------------------------------------
   Else, in the home task, execute the command processing synchronously.
  -------------------------------------------------------------------------*/
  else
  {
    hdrlmac_reset_modem_api_data();
  }
}



/*=========================================================================

FUNCTION     : HDRLMAC_UPDATE_MODEM_API_DATA

DESCRIPTION
  Updates Modem API data. 
 
DEPENDENCIES
  Should be run in HDR Tx task

INPUTS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void hdrlmac_update_modem_api_data( void )
{

  hdrlmac_cost_of_modem_power_enum cost_of_modem_power;
  uint8 read_buf;
  uint8 write_buf;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Reset timer */
  (void) rex_set_timer(&hdrlmac.modem_api_timeout_timer, 
                       HDRLMAC_MODEM_API_UPDATE_TIMEOUT_MS);

  /* Valid read buffer, we may peek at it. */
  read_buf = hdrlmac.valid_api_buf;

  /* This is the unused buffer, the one we are allowed to modify.
     (Operation just toggles between 1 and 0) */
  write_buf = 1 - read_buf;

  /* Update cost of modem power */
  cost_of_modem_power = ( hdrlmac_cost_of_modem_power_enum ) hdrrmac_process_report_cost_of_modem_power_est_cmd();

  /* Update the unused buffer */ 
  hdrlmac.last_valid_modem_api[write_buf].cost_of_modem_power 
    = cost_of_modem_power;

  /* Make unused buffer the valid buffer */
  hdrlmac.valid_api_buf = write_buf;

}

#endif /* FEATURE_HDR_MODEM_API */

/*===========================================================================
FUNCTION HDRLMAC_TIMER_CB

DESCRIPTION
  This function is the callback for LMAC protocol timers.
  This function is called by REX when the timer expires, and runs
  in the context of the task that defined the timer (HDRMC).

DEPENDENCIES
  None.

PARAMETERS
  timer_id - The ID of the timer that has expired

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrlmac_timer_cb
(
  uint32 timer_id
)
{
  #ifndef FEATURE_HDR_MODEM_API
  ASSERT (timer_id == HDRLMAC_AHO_TIMER);
  #endif /* FEATURE_HDR_MODEM_API */
  
  #ifdef FEATURE_HDR_MODEM_API
  switch (timer_id)
  {
    case HDRLMAC_DEACTIVATE_DONE_TIMER:
      ERR_FATAL( "Deactivate done timer expired. Force Crash", 0, 0, 0 );
      break;	
	
    case HDRLMAC_AHO_TIMER:
#endif /* FEATURE_HDR_MODEM_API */

  /* There was not another consecutive access following the last access
   * handoff, so we can return our threshold back to the minimum.
   */
  hdrlmac.aho_count = 0;
  
  #ifdef FEATURE_HDR_MODEM_API
      break;

    case HDRLMAC_MODEM_API_TIMEOUT_TIMER:
      /* Invalidate Modem API values as they haven't been updated. */
      hdrlmac_modem_api_timeout(
        HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);
      break;

    default:
      HDR_MSG_PROT( MSG_LEGACY_ERROR, "Unknown timer");
      break;
  }
 #endif /* FEATURE_HDR_MODEM_API */
} /* hdrlmac_timer_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CHANGE_STATE

DESCRIPTION
  This function maintains a state variable that mirrors the relevant
  changes to the HDRSRCH state machine.

DEPENDENCIES
  None

PARAMETERS
  new_state LMAC's opinion of HDRSRCH state
  None

RETURN VALUE
  None

SIDE EFFECTS
  hdrlmac.state is changed

===========================================================================*/

void hdrlmac_change_state
( 
  hdrlmac_state_enum_type new_state
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDRTRACE_STATE( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    hdrlmac.state, new_state);
                                   
  /* To prevent infinite Access Handoff we make each consecutive Access
   * Handoff more difficult.  To keep the logic simple, we don't return
   * the threshold to its initial setting until we spend HDRLMAC_AHO_TIMEOUT
   * out of access state.
   */

  if ((hdrlmac.state  == HDRLMAC_ACCESS_STATE) && 
      (new_state      == HDRLMAC_IDLE_STATE))
  {
    /* Time how long we remain out of access state */
    (void) rex_set_timer(&hdrlmac.aho_timer, HDRLMAC_AHO_TIMEOUT);
  }
  /* Avoid extra processing in the common case, when no AHO is happening */
  else if ((hdrlmac.state  == HDRLMAC_IDLE_STATE) && 
           (new_state      == HDRLMAC_ACCESS_STATE))
  {
    /* We are re-entering access state after access handoff */
    (void) rex_clr_timer(&hdrlmac.aho_timer);
  }
  else if (hdrlmac.state != new_state)
  {
    /* Any other state transition (e.g. idle->sleep, access->traffic)
     * means we are no longer performing an access attempt so we can
     * clear the timer and re-set our count.
     */
    (void) rex_clr_timer(&hdrlmac.aho_timer);
    hdrlmac.aho_count = 0;
  }

  hdrlmac.state = new_state;
} /* hdrlmac_change_state() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_STATE

DESCRIPTION
  This function returns the current HDR state stored in hdrlmac.state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  HDR state (hdrlmac_state_enum_type)

SIDE EFFECTS
  None

===========================================================================*/

hdrlmac_state_enum_type hdrlmac_get_state( void )
{

  return hdrlmac.state;
    /* Return the current hdrlmac state */

} /* hdrlmac_get_state() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_USER_MAC_DEBUG_MASK

DESCRIPTION
  This function retrieves the HDR MAC debug mask from the hdrl1_dbg_msk
  NV item.

DEPENDENCIES
  None

PARAMETERS
  mac_user_debug_mask - filled in with value from NV

RETURN VALUE
  Whether debug mask is valid.

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrlmac_get_user_mac_debug_mask
(
  uint64 * mac_debug_mask
    /* Mac debug mask valid from NV */
)
{
  
  if ( mac_debug_mask != NULL)
  {
    *mac_debug_mask = hdrlmac.mac_debug_mask;
      /* Fill in value and return */

    return hdrlmac.mac_debug_mask_valid;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================
 
FUNCTION HDRLMAC_FEATURE_SKIP_DISABLE_TX_ENABLED
 
DESCRIPTION
  This function retrieves the HDR MAC debug mask from the hdrl1_dbg_msk
  NV item.
 
DEPENDENCIES
  None
 
PARAMETERS
  mac_user_debug_mask - filled in with value from NV
 
RETURN VALUE
  Whether debug mask is valid.
 
SIDE EFFECTS
  None.
 
===========================================================================*/
 
boolean hdrlmac_feature_skip_disable_tx_enabled(void)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(hdrlmac.mac_debug_mask_valid)
  {
    if( (hdrlmac.mac_debug_mask & HDRLMAC_USER_MAC_DEBUG_SKIP_DISABLE_TX) == (HDRLMAC_USER_MAC_DEBUG_SKIP_DISABLE_TX) )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "RF Tx skip optimization off" );
      return FALSE; 
    }
  }
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "RF Tx skip optimization on" );
  return TRUE; 
} /* hdrlmac_feature_skip_disable_tx_enabled */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_READ_SRCH_NV_ITEMS

DESCRIPTION
  This function reads HDR Searcher NV items

  Read the NV items for searcher. Right now there is only one such item
  so it is decided that it would be read in HDRMC context during powerup
  init and will be set via a hdrsrch_ interface. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Sets the user diversity preference in hdrsrchrf data structure

===========================================================================*/

void hdrlmac_read_srch_nv_items( void )
{
  uint8         div_ctrl = 0;
    /* receive diversity control bit field */

  uint8         eq_ctrl = 0;
    /* HDR equalizer control field */

  nv_hdrl1_dbg_msk_type    debug_mask;
    /* HDR L1 debug mask field */

  uint8                    debug_array[64];  /* nv item is 64 bytes */
    /* HDR SRCH debug mask field */

  uint32                   div_mask = HDRLMAC_DEFAULT_DIV_BAND_MASK;
    /* HDR per band diversity mask */

  boolean                  nv_read_valid;
    /* Flag to indicate NV item has been succesfully read */

#ifdef FEATURE_HDR_QTA
  uint16                   qta_cfg;
   /* Variable to read DTX NV value into */
#endif /* FEATURE_HDR_QTA */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Read Rx diversity control parameter from NV */

  if( hdrutil_read_nv_item( NV_HDR_RX_DIVERSITY_CTRL_I,
                            (nv_item_type *) &div_ctrl ) != NV_DONE_S )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Read NV_RX_DIVERSITY_CTRL failed ...using default [off]" );
  }

  /* Read HDR diversity enable band class mask from NV */
  if( hdrutil_read_nv_item( NV_HDR_RX_DIV_BC_ENABLE_MASK_I,
                            (nv_item_type *) &div_mask ) != NV_DONE_S )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
                  "Read NV_HDR_RX_DIV_BC_ENABLE_MASK_I failed ... "
                  "using default %d", HDRLMAC_DEFAULT_DIV_BAND_MASK );
  }
  
  hdrsrch_set_user_diversity_pref( div_ctrl, div_mask );
    /* set the user diversity preference */

  /* Read Equalizer control parameter from NV */
  if( hdrutil_read_nv_item( NV_HDR_EQUALIZER_CTRL_I,
                            (nv_item_type *) &eq_ctrl ) != NV_DONE_S )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Read NV_EQ_CTRL failed ...using default [off]" );
  }

  hdrsrch_set_user_equalizer_pref( eq_ctrl );
    /* Set the user Equalizer/Rake/best selection demod preference */

  /* Read HDR L1 debug mask from NV */
  nv_read_valid = TRUE;

  if( hdrutil_read_nv_item( NV_HDRL1_DBG_MSK_I,
                            (nv_item_type *) &debug_mask ) != NV_DONE_S )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Read NV_HDRL1_DBG_MSK_I failed ..." );

    nv_read_valid = FALSE;
      /* Set flag to indicate that failed to read debug mask */

    hdrsrch_set_lre_enable( TRUE );
      /* Enable LRE if NV read fails */

    /*  NV read unsucessful, set to not valid. */
    hdrlmac.mac_debug_mask = 0x0;
    hdrlmac.mac_debug_mask_valid = FALSE;
  }
  else
  {
    /* Store mac debug mask for later retrieval. */
    hdrlmac.mac_debug_mask = debug_mask.hdrmac_dbg_msk;
    hdrlmac.mac_debug_mask_valid = TRUE;

  /* Check if LRE is enabled/disabled via NV. */
    if ( ( debug_mask.hdrmac_dbg_msk & HDRLMAC_USER_MAC_DEBUG_DISABLE_LRE )
        == HDRLMAC_USER_MAC_DEBUG_DISABLE_LRE )
    {
      /* Reset LRE enable flag */
      hdrsrch_set_lre_enable( FALSE );
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Lock Release Enhancement (LRE) feature disabled");
    }
    else
    {
      /* Set LRE enable flag */
      hdrsrch_set_lre_enable( TRUE );
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Lock Release Enhancement (LRE) feature enabled");
    }
  }

  hdrsrch_set_user_debug_mask( debug_mask.hdrsrch_dbg_msk, nv_read_valid );
    /* Set the user hdr layer1 srch debug mask */

  nv_read_valid = TRUE;
    /* Read HDR srch debug mask (64 byte array) from NV */

  if( hdrutil_read_nv_item( NV_HDRSRCH_DBG_MSK_I,
                            (nv_item_type *) &debug_array[0] ) != NV_DONE_S )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "Read NV_HDRSRCH_DBG_MSK_I failed ..." );

    nv_read_valid = FALSE;
      /* Set flag to indicate that failed to read debug mask */
  }

  hdrsrch_set_user_debug_array( debug_array,
                                nv_read_valid,
                                sizeof( debug_array ) );
    /* Set the user hdr srch debug array */
#ifdef FEATURE_HDR_QTA
  if ( hdrutil_fs_get( HDRLMAC_EFS_NV_DTX_CFG,
                       &qta_cfg,
                       sizeof(uint16) ) == MCFG_FS_STATUS_OK )
  {
    if ( ( qta_cfg & HDRLMAC_EFS_NV_QTA_DISABLED ) ==
         HDRLMAC_EFS_NV_QTA_DISABLED )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "HDR QTA disabled by NV ");
      
      hdrsrch_set_qta_nv( FALSE );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "HDR QTA enabled by NV ");
      hdrsrch_set_qta_nv( TRUE );
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
            "HDR QTA NV not set, enabling by default");

    hdrsrch_set_qta_nv( TRUE );
  }
#endif /* FEATURE_HDR_QTA */

} /* hdrlmac_read_srch_nv_items() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_POWERUP_INIT

DESCRIPTION
  This function performs one-time powerup initialization required for HMP

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_powerup_init( void )
{
  uint16 i;            /* loop counter */
  boolean dorb_capable; /* Is AT DORB capable? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( hdrmc_is_first_init() )
  {
    rex_init_crit_sect(&hdrlmac.crit_sect);

    HDRTRACE_INIT_DEFAULT(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);

    /* Initialize queues. */
    ( void ) q_init( &hdrlmac.pending_cmd_q );
    ( void ) q_init( &hdrlmac.pending_cmd_free_q );

    /* Fill free queues */

    for ( i = 0; i < HDRLMAC_CMD_BUFS_NUM; i++ )
    {
      hdrlmac.cmd_pool[i].hdr.done_q_ptr = &hdrlmac.pending_cmd_free_q;
      q_put( &hdrlmac.pending_cmd_free_q,
       q_link( &hdrlmac.cmd_pool[i],
         &hdrlmac.cmd_pool[i].hdr.link ) );
    }

    /* Initialize tca queues. */
    ( void ) q_init( &hdrlmac.pending_tca_q );
    ( void ) q_init( &hdrlmac.pending_tca_free_q );

    /* Fill free queues */
    for ( i = 0; i < HDRLMAC_TCA_BUFS_NUM; i++ )
    {
      hdrlmac.tca_pool[i].hdr.done_q_ptr = &hdrlmac.pending_tca_free_q;
      q_put( &hdrlmac.pending_tca_free_q,
       q_link( &hdrlmac.tca_pool[i],
         &hdrlmac.tca_pool[i].hdr.link ) );
    }
  }

  hdrlmac_read_srch_nv_items();
    /* read the NV items for searcher. Right now there is only one such item
       so it is decided that it would be read in HDRMC context during powerup
       init and will be set via a hdrsrch_ interface. */

  hdrlmac.state = HDRLMAC_INACTIVE_STATE;
  hdrlmac.l1_is_processing_tca = FALSE;

  if ( hdrmc_is_first_init() )
  {
    rex_def_timer_ex( &hdrlmac.aho_timer,
                      hdrlmac_timer_cb,
                      HDRLMAC_AHO_TIMER );
  }
		    
#ifdef FEATURE_HDR_MODEM_API
  if ( hdrmc_is_first_init() )
  {
    /* Define timer (6 sec) to invalidate modem API data */
    rex_def_timer_ex( &hdrlmac.modem_api_timeout_timer,
                      hdrlmac_timer_cb,
                      HDRLMAC_MODEM_API_TIMEOUT_TIMER );
	
    rex_def_timer_ex( &hdrlmac.deactivate_done_timer,
                      hdrlmac_timer_cb,
                      HDRLMAC_DEACTIVATE_DONE_TIMER );
  }

  /* Initialize values used for Modem API values */
  hdrlmac.awake_flag = FALSE;
  hdrlmac.connection_open = FALSE;
  hdrlmac.valid_api_buf = 0;
  hdrlmac_reset_modem_api_data();
#endif /* FEATURE_HDR_MODEM_API */

  /* Program decoder clock speed based on AT's capability for DORB */
  dorb_capable = hdrscp_at_is_dorb_capable();
  hdrdec_init_clock_speed( dorb_capable );

} /* hdrlmac_powerup_init() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_QUEUE_PENDING_CMD

DESCRIPTION
  This function queues a command that can not be processed until the access
  attempt is stopped.

DEPENDENCIES
  None

PARAMETERS
  cmd_ptr - Pointer to the command to be queued

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrlmac_queue_pending_cmd( hdrlmac_cmd_type *cmd_ptr )
{
  hdrlmac_cmd_buf_type *buf_ptr;  /* buffer for pending cmd */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cmd_ptr == NULL)
  {
    ERR( "Attempt to queue null cmd",0,0,0);
    return;
  }

  HDR_MSG_PROT( MSG_LEGACY_MED,  "LMAC cmd queued" );
  if ( ( buf_ptr = ( hdrlmac_cmd_buf_type * )
   q_get( &hdrlmac.pending_cmd_free_q ) ) == NULL )
  {
    /* Currently the only time this should happen is when we have already
     * received and queued a LMAC_FINISH_AHO_CMD or HDRLMAC_TC_ASSIGN_CMD 
     * and have now received another one.  
     * We account for this special case here.
     */
    if (cmd_ptr->name == HDRLMAC_FINISH_AHO_CMD)
    {
      if ((buf_ptr = (hdrlmac_cmd_buf_type *)
                q_get( &hdrlmac.pending_cmd_q )) != NULL)
      {
        /* Re-use the buffer from the previous LMAC_FINISH_AHO_CMD */
        ASSERT (buf_ptr->cmd.name == HDRLMAC_FINISH_AHO_CMD);
        HDR_MSG_PROT( MSG_LEGACY_MED,  "(redundant AHO cmd)" );
      }
      else
      {
        ERR_FATAL( "Lost buffer from hdrlmac.pending_cmd_free_q", 0, 0, 0 );
      }
    }
    else if (cmd_ptr->name == HDRLMAC_TC_ASSIGN_CMD)  
    {
      if ((buf_ptr = (hdrlmac_cmd_buf_type *)
                q_get( &hdrlmac.pending_cmd_q )) != NULL)
      {
        /* Received back to back TCAs, overriding the first one
         * Re-use buffer from the previous TC_ASSIGN_CMD */
        ASSERT ((buf_ptr->cmd.name == HDRLMAC_TC_ASSIGN_CMD) &&
                (hdrlmac.state == HDRLMAC_CONNECTED_STATE));
        HDR_MSG_PROT( MSG_LEGACY_HIGH,  "( back to back TCA commands)" );
      }
      else
      {
        ERR_FATAL( "Lost buffer from hdrlmac.pending_cmd_free_q (TCA)", 0, 0, 0 );
      }
    }
    else
    {
      ERR_FATAL( "Unexpected command to be queued", 0, 0, 0 );
    }
  }

  /* Copy the fields of the command to the queue item buffer */
#ifdef FEATURE_MEMCPY_REMOVAL
  buf_ptr->cmd = *cmd_ptr;/*lint !e613 no nulls */
#else
  ( void ) memcpy( &buf_ptr->cmd, cmd_ptr,
                   sizeof( hdrlmac_cmd_type ) );  /*lint !e613 no nulls */
#endif /* FEATURE_MEMCPY_REMOVAL */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link ); /*lint !e613 no nulls */
  q_put( &hdrlmac.pending_cmd_q, &buf_ptr->hdr.link ); /*lint !e613 no nulls */
} /* hdrlmac_queue_pending_cmd() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IND_CB

DESCRIPTION
  This function is registered to be called whenever a protocol gives an
  indication of interest to LMAC.

DEPENDENCIES
  None

PARAMETERS
  ind_name - Indication to process, is a global enum for all
                    possible indications in HDR signaling
  ind_data - Optional data pointer for indication specific data

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void hdrlmac_ind_cb (hdrind_ind_name_enum_type ind_name, void *ind_data)
{
  HDR_MSG_PROT_1( MSG_LEGACY_LOW, "hdrlmac_ind_cb %d", ind_name );

  /* This protocol runs in the tx task. */
  if ((rex_self () == HDRTX_TASK_PTR) && !rex_is_in_irq_mode())
  {
    /* Indication is from another protocol in the same task, so consider it
     * an internal indication
     */
    hdrlmac_process_ind (ind_name, (hdrind_ind_data_union_type *)ind_data);
  }
  else
  {
    hdrtx_queue_ind (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
           ind_name, ind_data);
  }
} /* hdrlmac_ind_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ACTIVATE

DESCRIPTION
  This function commands LMAC to activate.

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_activate (hdrhai_protocol_name_enum_type sender)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ACTIVATE_CMD;
  hdrlmac_cmd.sender = sender;

  if ( rex_self(  ) == HDRTX_TASK_PTR )
  {
    ERR( "Command %d issued from LMAC task context", hdrlmac_cmd.name, 0, 0);
  }

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));
} /* hdrlmac_activate() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_DEACTIVATE

DESCRIPTION
  This function commands LMAC to deactivate.

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_deactivate (hdrhai_protocol_name_enum_type sender)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_DEACTIVATE_CMD;
  hdrlmac_cmd.sender = sender;

  if ( rex_self(  ) == HDRTX_TASK_PTR )
  {
    ERR( "Command %d issued from LMAC task context", hdrlmac_cmd.name, 0, 0);
  }

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));
} /* hdrlmac_deactivate() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SET_SEARCH_PARAMS

DESCRIPTION
  The protocol setting the parameters should keep the parameters stored in a
  structure of hdrsrch_search_params_struct_type and call LMAC with the
  pointer to that structure.  This command directly calls
  hdrsrch_set_search_params() (IS-856) to give the parameters to HDRSRCH,
  who copies them into its own local storage.

DEPENDENCIES
  None

PARAMETERS
  params_ptr - pointer to searcher parameters
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_set_search_params
(
  hdrsrch_search_params_struct_type * params_ptr,
  hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SET_SRCH_PARAMETERS_CMD, sender, 0 );
  hdrsrch_set_search_params (params_ptr );
} /* hdrlmac_set_search_params() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SET_MGMT_SAME_CHANNEL_PARAMS

DESCRIPTION
  This function sets new parameters for the current channel to searcher.

DEPENDENCIES
  None

PARAMETERS
  params_ptr - pointer to searcher parameters
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_set_mgmt_same_channel_params
(
  hdrsrch_chan_mgmt_params_struct_type * params_ptr,
  hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SET_MGMT_SAME_CHAN_CMD, sender, 0 );
  hdrsrch_set_mgmt_same_channel_params(params_ptr );
} /* hdrlmac_set_mgmt_same_channel_params() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SET_MGMT_DIFF_CHANNEL_PARAMS

DESCRIPTION
  This function sets new parameters for a different channel to searcher.

DEPENDENCIES
  None

PARAMETERS
  params_ptr - pointer to searcher parameters
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_set_mgmt_diff_channel_params
(
  hdrsrch_chan_mgmt_params_struct_type * params_ptr,
  hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SET_MGMT_DIFF_CHAN_CMD, sender, 0 );
  hdrsrch_set_mgmt_diff_channel_params(params_ptr );
} /* hdrlmac_set_mgmt_diff_channel_params() */


#ifdef FEATURE_HDR_BCMCS
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SET_BCMCS_PAGE_CYCLE

DESCRIPTION
  This command calls hdrsrch_set_bcmcs_page_cycle() to update the BCMCS page
  cycle value.

DEPENDENCIES
  None

PARAMETERS
  page_cycle    - The BCMCS paging channel period which the AT monitors, in
                  units of SCC ( 256 slots ).

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void hdrlmac_set_bcmcs_page_cycle
(
  uint8 page_cycle
)
{
  HDRTRACE_LMAC_SET_BCMCS_PAGE_CYCLE(page_cycle);
  hdrsrch_set_bcmcs_page_cycle(page_cycle);
} /* hdrlmac_set_bcmcs_page_cycle */


/*===========================================================================

FUNCTION HDRLMAC_SET_BCMCS_SLEEP_CYCLE

DESCRIPTION
  This command calls hdrsrch_set_bcmcs_sleep_cycle() to update the BCMCS sleep
  cycle value.

DEPENDENCIES
  None

PARAMETERS
  sleep_cycle   - The BCMCS sleep period , in units of SCC ( 256 slots ).

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void hdrlmac_set_bcmcs_sleep_cycle
(
  uint8 sleep_cycle
    /* In units of SCC ( 256 slots )  */
)
{
  hdrsrch_set_bcmcs_sleep_cycle( sleep_cycle );
} /* hdrlmac_set_bcmcs_sleep_cycle  */


#endif /* FEATURE_HDR_BCMCS */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SET_CCC

DESCRIPTION
  This command calls hdrsrch_set_control_channel_cycle() to update the control
  channel cycle value.  This is a separate command from set parameters because
  the control channel cycle value is generated at a different time than the
  rest of the parameters.

DEPENDENCIES
  None

PARAMETERS
  ccc    - The paging channel slot which the AT hashed to.
  max_sleep_cycle - Maximum sleep cycle duration negotiated (SlotCycle3)
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_set_ccc 
(
  uint8 ccc, 
  uint8 max_sleep_cycle,
  hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SET_CC_HASH_CMD, sender, 0 );
  hdrsrch_set_control_channel_cycle (ccc, max_sleep_cycle);
  HDRTRACE_LMAC_CHANGING_CC_HASH(ccc);
} /* hdrlmac_set_ccc() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_PCCC

DESCRIPTION
  This command calls hdrsrch_get_preferred_control_channel_cycle() to get the
  preferred control channel cycle.  This function should only be called if the 
  1x page slot is known.  Otherwise the default HDR control channel cycle 
  should be used.  It is OK for the 1x system to assume a page slot based on
  IMSI_M when it is not sure, then give an indication if it acquires a system
  that uses IMSI_T.  This allows us to set the cycle value once during initial
  session negotiation and only change it in the rare case when IMSI_T is
  being used.

DEPENDENCIES
  None

PARAMETERS
  page_slot   - Assigned slot in IS-95A/B or IS2000 1x system
  max_hdr_slot_cycle - Largest slot cycle currently configured for HDR
  default_ccc - Default control channel index calculated by hash function, 
                given the maximum sleep cycle
  sender      - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

uint8 hdrlmac_get_pccc
(
  uint16                          page_slot,
  uint8                           max_hdr_slot_cycle,
  uint8                           default_ccc,
  hdrhai_protocol_name_enum_type  sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SET_CC_HASH_CMD, sender, 0 );
  return ( hdrsrch_get_preferred_control_channel_cycle( page_slot, 
                                                        max_hdr_slot_cycle, 
                                                        default_ccc ));
} /* hdrlmac_get_pccc() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ACQUIRE_CB

DESCRIPTION
  Callback called from HDR searcher to give outcome of acquisition attempt

DEPENDENCIES
  None

PARAMETERS
  channel - The band and channel that acquisition was attempted on
  acq_result - The outcome of the acquisition

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_acquire_cb
(
  const sys_channel_type *channel,
  hdrsrch_acquire_enum_type acq_result
)
{
  if (acq_result == HDRSRCH_ACQUIRED)
  {
    /* Do we need to pass the channel along with the indication??? */
    hdrind_give_ind( HDRIND_LMAC_ACQUIRED, NULL);
    HDRTRACE_LMAC_ACQ_SUCCESS();
  }
  else
  {
    hdrind_give_ind( HDRIND_LMAC_ACQUISITION_FAILED, NULL);
    HDRTRACE_LMAC_ACQ_FAIL_PILOT();
    hdrlmac_change_state(HDRLMAC_INACTIVE_STATE);
  }
} /* hdrlmac_acquire_cb */ /*lint !e715 channel not used */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SYSTEM_LOST_CB

DESCRIPTION
  Callback called from HDR searcher to report when the system is lost
  (after it is acquired)

DEPENDENCIES
  None

PARAMETERS
  reason - reason for declaring system lost

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_system_lost_cb 
( 
  hdrsrch_system_lost_enum_type   reason
)
{

  hdrind_ind_data_union_type ind_data;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRTRACE_LMAC_NETWORK_LOST(reason);
  ind_data.sys_lost_reason = reason;

  if (reason == HDRSRCH_SYSLOST_RESELECT)
  {
    hdrind_give_ind( HDRIND_LMAC_RESELECT, NULL );
  }
  else
  {
    hdrind_give_ind( HDRIND_LMAC_NETWORK_LOST, (void*)&ind_data);
  }

} /* hdrlmac_system_lost_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_USABLE_CB

DESCRIPTION
  Callback called from HDR searcher to report whether the HDR signal 
  is strong enough to be usable or not
  
DEPENDENCIES
  None

PARAMETERS
  usability

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_usable_cb 
( 
  hdrsrch_system_usable_enum_type   usability
)
{
  if (usability == HDRSRCH_USABLE_TRUE)
  {
    hdrind_give_ind( HDRIND_LMAC_HDR_USABLE, NULL );
  }
  else
  {
    hdrind_give_ind( HDRIND_LMAC_RESELECT, NULL );
  }

} /* hdrlmac_usable_cb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SYNC_SUSPEND_CB

DESCRIPTION
  Send indication to CP based on the sync substate

DEPENDENCIES
  None

PARAMETERS
  srch_sync_suspend - substate of sync 

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_sync_suspend_cb
(
  hdrsrch_sync_substate_enum_type srch_sync_suspend
)
{
  switch (srch_sync_suspend)
  {
    case HDRSRCH_SYNC_SUSPEND:
        hdrind_give_ind( HDRIND_LMAC_SYNC_SUSPEND, NULL);
      break;

    case HDRSRCH_SYNC_RESUME:
      hdrind_give_ind( HDRIND_LMAC_SYNC_RESUME, NULL);
      break;
      
    default:
      /* There are other events that HDRSRCH may report using this callback,
       * but they are ignored for now.
       */
      break;
  }
}  /* hdrlmac_sync_suspend_cb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ACQUIRE

DESCRIPTION
  The caller passes in the channel to acquire.  LMAC directly calls
  hdrsrch_acquire(), passing in the channel and two callback functions.

DEPENDENCIES
  None

PARAMETERS
  channel - The band and channel to attempt acquisition on
  mode    - Mode of acquisition (deep, shallow, full, etc.)
  pn_list_size - Number of pn's in list
  pilot_pn - Pointer to HDR pilot pn
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_acquire
(
  sys_channel_type channel,
  const sys_hdr_acq_mode_e_type mode,
#ifdef FEATURE_LTE_TO_HDR
  uint8 pn_list_size,
  sys_pilot_pn_type *pilot_pn,
  uint16 time_allowed_ms,
  hdrsrch_acq_reason_enum_type reason,
#endif /* FEATURE_LTE_TO_HDR */
  hdrhai_protocol_name_enum_type sender
)
{
 /* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;
/* ---------------------------------------------------------------------- */
  
  memset ( &hdrlmac_cmd, 0, sizeof(hdrlmac_cmd_type) );

    /* Fill in all members of the hdrlmac_cmd_type for this command*/
  hdrlmac_cmd.name = HDRLMAC_ACQUIRE_CMD;
  hdrlmac_cmd.sender = sender;
  hdrlmac_cmd.params.acquire.channel = channel;
  hdrlmac_cmd.params.acquire.acq_cb = hdrlmac_acquire_cb;
  hdrlmac_cmd.params.acquire.mode = mode;
  hdrlmac_cmd.params.acquire.system_lost_cb = hdrlmac_system_lost_cb;
  hdrlmac_cmd.params.acquire.usability_cb = hdrlmac_usable_cb;
  if ( hdrlmac_is_lre_enabled() )
  {
  hdrlmac_cmd.params.acquire.sync_suspend_cb = hdrlmac_sync_suspend_cb;
  }

#ifdef FEATURE_LTE_TO_HDR
  hdrlmac_cmd.params.acquire.num_of_pn = pn_list_size;
#ifdef FEATURE_MEMCPY_REMOVAL
  memscpy ( &hdrlmac_cmd.params.acquire.pn_list, 
            sizeof(hdrlmac_cmd.params.acquire.pn_list), 
            pilot_pn, 
           (sizeof(sys_pilot_pn_type) * HDR_CP_MAX_PN_LIST_SIZE) );
#else
  memcpy ( &hdrlmac_cmd.params.acquire.pn_list, pilot_pn, 
           (sizeof(sys_pilot_pn_type) * HDR_CP_MAX_PN_LIST_SIZE) );
#endif /* FEATURE_MEMCPY_REMOVAL */
  hdrlmac_cmd.params.acquire.overall_time_allowed_ms = time_allowed_ms;
  hdrlmac_cmd.params.acquire.reason = reason;  
#endif /* FEATURE_LTE_TO_HDR */

   /* This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_acquire() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_TIMING_INFO_RETURN_CB

DESCRIPTION
  Callback called from HDR searcher to report timing information.


DEPENDENCIES
  None

PARAMETERS
  status - whether the timing info is available or not
  lte_ostmr - LTE RTC counter
  cdma_sys_time - CDMA System time

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_timing_info_return_cb
(
  boolean status,
  uint32 lte_ostmr,
  uint64 cdma_sys_time
)
{
  hdrind_lmac_timing_info_type timing_info;
    /* HDR timing retrieved from sync message */
/*-------------------------------------------------------------------------*/

  timing_info.status = status;
  timing_info.cdma_sys_time = cdma_sys_time;
  timing_info.lte_ostmr = lte_ostmr;

  hdrind_give_ind( HDRIND_LMAC_TIMING_INFO, (void *)&timing_info );

}  /* hdrlmac_timing_info_return_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SYNCHRONIZE

DESCRIPTION
  LMAC is a passthrough interface for time synchronization.  It directly calls
  hdrsrch_synchronize().

DEPENDENCIES
  None

PARAMETERS
 link_id - The link ID associated with the received sync message (not
           currently used)
 pilot_pn - The pilot PN that this sector is using
 sys_time - The time specified in the sync message, in 80ms units from
            UTC=0 (00:00:00 Jan 6,  1980 GMT)
 half_slot - The half-slot the synchronous control channel capsule started on.
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_synchronize
(
  hdrsrch_link_id_type link_id,
  sys_pilot_pn_type pilot_pn,
  qword sys_time,
  hdrdec_half_slot_time_type half_slot,
  hdrhai_protocol_name_enum_type sender
)
{
   /* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command*/
  hdrlmac_cmd.name = HDRLMAC_SYSTEM_TIME_SYNC_CMD;
  hdrlmac_cmd.sender = sender;
  hdrlmac_cmd.params.synchronize.pilot_pn = pilot_pn;
  hdrlmac_cmd.params.synchronize.half_slot = half_slot;
  qw_set(hdrlmac_cmd.params.synchronize.sync_time, qw_hi(sys_time), qw_lo(sys_time));
  hdrlmac_cmd.params.synchronize.link_id = link_id;

   /* This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_synchronize() */ /*lint !e715 link_id not used */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IDLE_HO_CB

DESCRIPTION
  Callback called from HDR searcher to report when a different pilot PN has
  been given (autonomously) to the DSP.  This does not guarantee that nothing
  more will be decoded on the old pilot.

DEPENDENCIES
  None

PARAMETERS
  link_id - The searcher-assigned ID for the new link we are switching to
  link_ptr - A pointer to a description of the new link

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_idle_ho_cb
(
  const hdrsrch_link_id_type link_id,
  const sys_link_type *link_ptr
)
{
  /* Notify CC MAC of the new link */
  hdrcmac_update_link( link_id );

  hdrind_give_ind( HDRIND_LMAC_IDLE_HANDOFF, (void *)&link_id);
  HDRTRACE_LMAC_IDLE_HO(link_ptr->pilot_pn);
  hdrplog_log_ho();

}  /* hdrlmac_idle_ho_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IDLE_SUSPEND_CB

DESCRIPTION
  Callback called from HDR searcher to report when the RF has been taken
  away from or given back to HDR.  This callback reflects transitions of
  HDR searcher to and from the idle suspend state.

DEPENDENCIES
  None

PARAMETERS
  suspend_resume - Whether callback should cause the protocol to suspend 
                   or resume

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_idle_suspend_cb
(
  hdrsrch_suspend_enum_type suspend_resume
)
{
  switch (suspend_resume)
  {
    case HDRSRCH_SUSPEND_STARTED:
    case HDRSRCH_SUSPEND_STILL_SUSPENDED:
      hdrind_give_ind( HDRIND_LMAC_IDLE_SUSPEND, NULL);
      break;

    case HDRSRCH_SUSPEND_NONE:
    case HDRSRCH_SUSPEND_ENDED:
      hdrind_give_ind( HDRIND_LMAC_IDLE_RESUME, NULL);
      break;
    
    case HDRSRCH_SUSPEND_TO_REACQ:
      hdrind_give_ind( HDRIND_LMAC_IDLE_SUSPEND_TO_REACQ, NULL );
      break;
	  
    default:
      /* There are other events that HDRSRCH may report using this callback,
       * but they are ignored for now.
       */
      break;
  }

}  /* hdrlmac_idle_suspend_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IDLE_MODE

DESCRIPTION
  When commanded to Idle Mode, LMAC directly calls hdrsrch_idle_mode() passing
  a callback function.

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_idle_mode (hdrhai_protocol_name_enum_type sender)
{
  /* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command*/ 
  hdrlmac_cmd.name = HDRLMAC_IDLE_MODE_CMD;
  hdrlmac_cmd.sender = sender;

#ifdef FEATURE_HDR_REVC
  /* turn off demod CC on all carriers */
  hdrsrch_enable_nlb(FALSE);
#endif /* FEATURE_HDR_REVC */
 
  /* This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_idle_mode() */

#ifdef FEATURE_HDR_BCMCS
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_UPDATE_BC_INFO

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_update_bc_info
( 
  boolean bcmcs_status
)
{
  /* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Fill in all members of the hdrlmac_cmd_type for this command*/
  hdrlmac_cmd.name = HDRLMAC_UPDATE_BC_INFO_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.update_bc_info_bcmcs_status = bcmcs_status;

  /* This function copies the command into a buffer before it return*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_update_bc_info() */
#endif /* FEATURE_HDR_BCMCS */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_NSET_UPDATE

DESCRIPTION
  The protocol who receives the neighbor list should keep the list stored in
  an array of hdrsrch_nset_pilot_type structures.  If a new neighbor list is
  received, the caller must first copy each pilot PN and channel from the
  message into a hdrsrch_nset_pilot_type structure, then call LMAC and pass
  the pointer to the head of the array and the total number of neighbors in
  the list.  This command directly calls hdrsrch_nset_update() to give the
  list to HDRSRCH.

DEPENDENCIES
  None

PARAMETERS
  num_nset_pilots - Number of pilots to add to neighbor list
  nset_list_ptr - Pointer to an array of nset structures
  num_channels - Number of channels in the channel list
  channel_list_ptr - Channel fields from the SectorParameters message
  num_sectors - Number of sectors in the overehad cache
  sector_list_ptr - List of sectors currently in the overhead cache
  num_avoided_channels - Number of systems in the avoided channel list
  avoided_list_ptr - List of avoided channels
  update_type - NSET information from a BROADCAST or DIRECTED message?
  sender - Protocol calling this function, for debug.
  serving_pilot_gid - The group id of the pilot from which the nset update is received 

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_nset_update
(
  uint8                           num_nset_pilots,
  hdrsrch_nset_pilot_type         *nset_list_ptr,
  uint8                           num_channels,
  const sys_channel_type          *channel_list_ptr,
  uint8                           num_sectors,
  sys_link_type                   *sector_list_ptr,
  const uint8                     num_avoided_channels,
  const sys_avoided_channel_record_type   *avoided_list_ptr,
  hdrsrch_nset_update_enum_type   update_type,
  hdrhai_protocol_name_enum_type  sender,
  hdrsrch_pilot_gid_type          serving_pilot_gid
)
{                    

   /* Build command into local struct*/ 
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Fill in all members of the hdrlmac_cmd_type for this command*/ 
  hdrlmac_cmd.name = HDRLMAC_NEIGHBOR_LIST_UPDATE_CMD;
  hdrlmac_cmd.sender = sender;
  hdrlmac_cmd.params.nset_update.num_pilots = num_nset_pilots;
  if (nset_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &hdrlmac_cmd.params.nset_update.nset_pilots[0], 
              sizeof(hdrlmac_cmd.params.nset_update.nset_pilots), 
              nset_list_ptr, 
             (HDRSRCH_NSET_MAX_SIZE * sizeof ( hdrsrch_nset_pilot_type ))); 
#else
    memcpy ( &hdrlmac_cmd.params.nset_update.nset_pilots[0], nset_list_ptr, 
             (HDRSRCH_NSET_MAX_SIZE * sizeof ( hdrsrch_nset_pilot_type ))); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
   
  hdrlmac_cmd.params.nset_update.num_channels = num_channels;
  if(channel_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &hdrlmac_cmd.params.nset_update.channel_list[0], 
              sizeof(hdrlmac_cmd.params.nset_update.channel_list),  
              channel_list_ptr, 
             (HDRSRCH_HASH_CHAN_MAX_SIZE * sizeof ( sys_channel_type ))); 
#else
    memcpy ( &hdrlmac_cmd.params.nset_update.channel_list[0], channel_list_ptr, 
           (HDRSRCH_HASH_CHAN_MAX_SIZE * sizeof ( sys_channel_type ))); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  
  hdrlmac_cmd.params.nset_update.num_sects = num_sectors;
  if(sector_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &hdrlmac_cmd.params.nset_update.cached_sects[0], 
               sizeof(hdrlmac_cmd.params.nset_update.cached_sects), 
               sector_list_ptr, 
              (HDRSRCH_MAX_CACHE_RECORDS * sizeof ( sys_link_type ))); 
#else
    memcpy ( &hdrlmac_cmd.params.nset_update.cached_sects[0], sector_list_ptr, 
           (HDRSRCH_MAX_CACHE_RECORDS * sizeof ( sys_link_type ))); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  
  hdrlmac_cmd.params.nset_update.num_avoided_channels = num_avoided_channels;
  if(avoided_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &hdrlmac_cmd.params.nset_update.avoided_list[0], 
              sizeof(hdrlmac_cmd.params.nset_update.avoided_list), 
              avoided_list_ptr, 
             (HDRSRCH_AVOIDED_CHAN_MAX_SIZE * 
                sizeof (sys_avoided_channel_record_type))); 
#else
    memcpy ( &hdrlmac_cmd.params.nset_update.avoided_list[0], avoided_list_ptr, 
           (HDRSRCH_AVOIDED_CHAN_MAX_SIZE * 
              sizeof (sys_avoided_channel_record_type))); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
   
  hdrlmac_cmd.params.nset_update.update_type = update_type;
  hdrlmac_cmd.params.nset_update.serving_pilot_gid = serving_pilot_gid;

  /* This function copies the command into a buffer before it returns*/ 
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_nset_update() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CHAN_CHANGE_RPT_CB

DESCRIPTION
  Callback called from HDR searcher to report when SRCH has finished tuning
  to a new channel (as ordered by the hdrsrch_idle_channel_change() command.
  This does not guarantee that the decoder won't still deliver a packet from
  the old channel since there is some decoder delay.

DEPENDENCIES
  None

PARAMETERS
  link_id  - The link_id associated with the link to the new channel
  link_ptr - A pointer to a description of the new link

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_chan_change_rpt_cb
(
  const hdrsrch_link_id_type link_id,
  const sys_link_type *link_ptr
)
{
  /* Notify CC MAC of the new link */
  hdrcmac_update_link( link_id );

  hdrind_give_ind( HDRIND_LMAC_FREQ_CHANGE_COMPLETE, (void *)&link_id);
  HDRTRACE_LMAC_CHAN_CHANGE_COMPLETE();

} /* hdrlmac_chan_change_rpt_cb */ /*lint !e715 link_ptr not used */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IDLE_CHAN_CHANGE

DESCRIPTION
  This command requires special processing by LMAC, so it is queued and
  ndled in LMAC task context.

DEPENDENCIES
  None

PARAMETERS
  link - The band, channel and pilot to change channel to
  channel_change_in_sleep - channel change requested while sleeping
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_idle_chan_change
(
  sys_link_type *link_ptr,
  boolean channel_change_in_sleep,
  hdrhai_protocol_name_enum_type sender
)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_IDLE_CHAN_CHANGE_CMD;
  hdrlmac_cmd.sender = sender;
#ifdef FEATURE_MEMCPY_REMOVAL
  hdrlmac_cmd.params.idle_chan_change.link = *link_ptr;
#else
  ( void ) memcpy( &hdrlmac_cmd.params.idle_chan_change.link, link_ptr,
                   sizeof( sys_link_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
  hdrlmac_cmd.params.idle_chan_change.channel_change_in_sleep = 
    channel_change_in_sleep;

  if ( rex_self(  ) == HDRTX_TASK_PTR )
  {
    ERR( "Command %d issued from LMAC task context", hdrlmac_cmd.name, 0, 0);
  }

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_idle_chan_change () */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IDLE_CHANNEL_CHANGE_AND_NSET_UPDATE

DESCRIPTION
  This command takes idle channel change and nset update parameters and
  passes it to SRCH.

DEPENDENCIES
  None

PARAMETERS
  num_nset_pilots - Number of pilots to add to neighbor list
  nset_list_ptr - Pointer to an array of nset structures
  num_channels - Number of channels in the channel list
  channel_list_ptr - Channel fields from the SectorParameters message
  num_sectors - Number of sectors in the overehad cache
  sector_list_ptr - List of sectors currently in the overhead cache
  num_avoided_channels - Number of systems in the avoided channel list
  avoided_list_ptr - List of avoided channels
  update_type - NSET information from a BROADCAST or DIRECTED message?
  serving_pilot_gid - The group id of the pilot from which the nset update is received 
  link - The band, channel and pilot to change channel to
  channel_change_in_sleep - channel change requested while sleeping
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_idle_channel_change_and_nset_update
(
  uint8                           num_nset_pilots,
  hdrsrch_nset_pilot_type         *nset_list_ptr,
  uint8                           num_channels,
  const sys_channel_type          *channel_list_ptr,
  uint8                           num_sectors,
  sys_link_type                   *sector_list_ptr,
  const uint8                     num_avoided_channels,
  const sys_avoided_channel_record_type   *avoided_list_ptr,
  hdrsrch_nset_update_enum_type   update_type,
  hdrsrch_pilot_gid_type          serving_pilot_gid,
  sys_link_type                   *link_ptr,
  boolean                         channel_change_in_sleep,
  hdrhai_protocol_name_enum_type  sender
)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
  /* Convenience pointer */
  hdrlmac_idle_chan_chg_and_nset_update_params_type *ptr =
    &hdrlmac_cmd.params.idle_chan_chg_and_nset_update;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrlmac_cmd.name = HDRLMAC_IDLE_CHAN_CHG_AND_NSET_UPDATE_CMD;
  hdrlmac_cmd.sender = sender;

#ifdef FEATURE_MEMCPY_REMOVAL
  ptr->link = *link_ptr;
#else
  memcpy( &ptr->link, link_ptr, sizeof(sys_link_type) );
#endif /* FEATURE_MEMCPY_REMOVAL */
  ptr->channel_change_in_sleep = channel_change_in_sleep;

  ptr->num_pilots = num_nset_pilots;
  if (nset_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &ptr->nset_pilots[0], 
              sizeof(ptr->nset_pilots), 
              nset_list_ptr, 
              (HDRSRCH_NSET_MAX_SIZE * sizeof(hdrsrch_nset_pilot_type)) ); 
#else
    memcpy ( &ptr->nset_pilots[0], 
             nset_list_ptr, 
             (HDRSRCH_NSET_MAX_SIZE * sizeof(hdrsrch_nset_pilot_type)) ); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  ptr->num_channels = num_channels;
  if (channel_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &ptr->channel_list[0], 
              sizeof(ptr->channel_list),  
              channel_list_ptr, 
              (HDRSRCH_HASH_CHAN_MAX_SIZE * sizeof(sys_channel_type)) ); 
#else
    memcpy ( &ptr->channel_list[0], 
             channel_list_ptr, 
             (HDRSRCH_HASH_CHAN_MAX_SIZE * sizeof(sys_channel_type)) ); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  ptr->num_sects = num_sectors;
  if (sector_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &ptr->cached_sects[0], 
              sizeof(ptr->cached_sects), 
              sector_list_ptr, 
              (HDRSRCH_MAX_CACHE_RECORDS * sizeof(sys_link_type)) ); 
#else
    memcpy ( &ptr->cached_sects[0], 
             sector_list_ptr, 
             (HDRSRCH_MAX_CACHE_RECORDS * sizeof(sys_link_type)) ); 
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  ptr->num_avoided_channels = num_avoided_channels;
  if(avoided_list_ptr)
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy ( &ptr->avoided_list[0], 
              sizeof(ptr->avoided_list), 
              avoided_list_ptr, 
              (HDRSRCH_AVOIDED_CHAN_MAX_SIZE * 
                sizeof(sys_avoided_channel_record_type)) ); 
#else
    memcpy ( &ptr->avoided_list[0],
             avoided_list_ptr, 
             (HDRSRCH_AVOIDED_CHAN_MAX_SIZE * 
               sizeof (sys_avoided_channel_record_type)) );
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  ptr->update_type = update_type;
#ifdef FEATURE_HDR_REVB
  ptr->serving_pilot_gid = serving_pilot_gid;
#endif /* FEATURE_HDR_REVB */

  if ( rex_self() == HDRTX_TASK_PTR )
  {
    ERR( "Command %d issued from LMAC task context", hdrlmac_cmd.name, 0, 0 );
  }
  hdrtx_queue_cmd( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                   &hdrlmac_cmd, sizeof(hdrlmac_cmd_type) );

} /* hdrlmac_idle_channel_change_and_nset_update () */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_WAKEUP_CB

DESCRIPTION
  Callback called from HDR searcher to report when SRCH wakes up from sleep.
  This callback doubles as an idle handoff callback in the event we wake up
  on a pilot different than the one we were on when we went to sleep.

DEPENDENCIES
  None

PARAMETERS
  link_id - HDRSRCH_REF_LINK_ID if we woke up on the same pilot we were on
            when we went to sleep, or the new link_id if the link changed
  link_ptr - NULL if link_id is HDRSRCH_REF_LINK_ID, otherwise a pointer to
             the information about the new link on which we woke up.

  reason - the reason for this wake-up

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_wakeup_cb
(
  const hdrsrch_link_id_type link_id,
  const sys_link_type *link_ptr,
  hdrsrch_wakeup_reason_enum_type   reason,
  boolean reacq_succeeded
)
{

#ifdef FEATURE_HDR_DDARF
  hdrind_lmac_wakeup_status_type wakeup_status;
/* --------------------------------------------------------------------------*/

  /* Consolidate the wakeup reason and whether reacq succeeded and pass it to 
     IDLE so that it can make a decision whether to declare system lost or not */
  wakeup_status.reacq_succeded = reacq_succeeded;
  wakeup_status.wakeup_reason = reason;

  hdrind_give_ind( HDRIND_LMAC_AWAKE, &wakeup_status );
#else
  hdrind_give_ind( HDRIND_LMAC_AWAKE, NULL);
#endif /* FEATURE_HDR_DDARF */

  HDRTRACE_LMAC_WAKEUP();

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_set_reacq_status( reacq_succeeded );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
  
#ifdef FEATURE_HDR_MODEM_API
  /* Flag for Modem API reporting after wakeup */
  hdrlmac.awake_flag = TRUE;
#endif /* FEATURE_HDR_MODEM_API */

  /* This callback may occur while LMAC state is ACCESS or IDLE */

  if ( link_id != HDRSRCH_REF_LINK_ID )
  {
    hdrlmac_idle_ho_cb( link_id, link_ptr );
  }

  HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Wake-up reason: 0x%x", reason );

} /* hdrlmac_wakeup_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SLEEP

DESCRIPTION
  The Idle State Protocol gives this command to put the HDR hardware to sleep.
  LMAC directly calls hdrsrch_sleep(), passing a callback function.  If we
  are not allowed to sleep at this time, HDRSRCH will call the callback
  function immediately.

DEPENDENCIES
  None

PARAMETERS
  hdr_slot_cycle - How long to sleep for
  hdr_neg_slot_cycle - What is the current negotiated SCI which can be
                       different from what the protocols are asking for
                       this particular sleep cycle.
  sender - Protocol calling the function, for debug
  intra_scc_start_slot_valid -  Flag used to indicate whether 
                               intra_scc_start_slot is valid 
  intra_scc_start_slot - CC start in slots. 
  qpch_enabled - whether QPCH is enabled or not.
  other_rat_tx_cycle - Indicates the transmission cycle of the 
                       OtherRATNeighborList message
  session_is_open - whether EVDO session is open
  ovhd_is_updated - whether OVHD info is updated

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_sleep
(
  uint8   hdr_slot_cycle,  /* e.g. 6: 426ms, 9: 5.12s, 12: 40.96s */
  uint8   hdr_neg_slot_cycle,
  hdrhai_protocol_name_enum_type sender,
  boolean intra_scc_start_slot_valid,
  uint16 intra_scc_start_slot,
  boolean qpch_enabled
#ifdef FEATURE_HDR_TO_LTE
  ,uint8 other_rat_tx_cycle
#endif /* FEATURE_HDR_TO_LTE*/
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  ,boolean session_is_open
  ,boolean ovhd_is_updated
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
)
{
  
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   
    /* Fill in all members of the hdrlmac_cmd_type for this command*/ 
  hdrlmac_cmd.name = HDRLMAC_SLEEP_CMD;
  hdrlmac_cmd.sender = sender;
  hdrlmac_cmd.params.sleep.hdr_slot_cycle = hdr_slot_cycle;
  hdrlmac_cmd.params.sleep.hdr_neg_slot_cycle = hdr_neg_slot_cycle;
  hdrlmac_cmd.params.sleep.qpch_enabled = qpch_enabled;
  hdrlmac_cmd.params.sleep.wakeup_cb = hdrlmac_wakeup_cb;
  hdrlmac_cmd.params.sleep.cc_start_slot_valid = intra_scc_start_slot_valid ;
  hdrlmac_cmd.params.sleep.cc_start_slot = intra_scc_start_slot;
#ifdef FEATURE_HDR_TO_LTE
    hdrlmac_cmd.params.sleep.ornl_cycle = other_rat_tx_cycle;
#endif /* FEATURE_HDR_TO_LTE*/
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  hdrlmac_cmd.params.sleep.session_is_open = session_is_open;
  hdrlmac_cmd.params.sleep.ovhd_is_updated = ovhd_is_updated;
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
  
   /*This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));
  
} /* hdrlmac_sleep () */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SUSPEND_HO_CB

DESCRIPTION
  Callback called from HDR searcher to acknowledge that SRCH will not
  change the pilot until commanded to do so.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_suspend_ho_cb ( void )
{
  hdrind_give_ind( HDRIND_LMAC_ACCESS_MODE_ENTERED, NULL);
  HDRTRACE_LMAC_ACCESS_MODE_ENTERED();
  hdrplog_access_mode(TRUE);
} /* hdrlmac_suspend_ho_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_REQUEST_AHO_CB

DESCRIPTION
  Callback called from HDR searcher to request to change the pilot.

DEPENDENCIES
  None

PARAMETERS
  req_link_ptr - Pointer to structure describing the link to which searcher
                 wishes to change to.

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_request_aho_cb
(
  const sys_link_type *req_link_ptr
)
{
  hdrind_give_ind( HDRIND_LMAC_AHO_REQUEST, NULL);
  HDRTRACE_LMAC_ACCESS_HO_NEEDED(req_link_ptr->pilot_pn);
} /* hdrlmac_request_aho_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ACCESS_MODE

DESCRIPTION
  HMP gives this command prior to starting an access attempt.  HMP waits for
  an indication that the LMAC has entered access mode before checking overhead
  messages, etc., to make sure the pilot will not change out from under it.
  While in Access Mode, HDRSRCH must call hdrlmac_request_aho_cb() if it
  decides a pilot change in needed.  HDRSRCH can not autonomously change the
  pilot while in access mode

  To HDRSRCH, the access mode command is also an implied wakeup command, if
  HDRSRCH is asleep when the command is received.  In this case HDRSRCH must
  call hdrlmac_wakeup_cb() prior to calling hdrlmac_suspend_ho_cb().

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_access_mode (hdrhai_protocol_name_enum_type sender)
{

   /* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ACCESS_MODE_CMD;
  hdrlmac_cmd.sender = sender;
  
    /* This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_access_mode () */

#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRLMAC_TA_TIME_CB

DESCRIPTION
  Callback function for HDR SRCH to send tune away time to ACMAC.
  
DEPENDENCIES
  None

PARAMETERS
  no_tune_away - Whether tune away or not. TRUE: no tune away. FALSE: tune away.
  ta_time      - Tune away time

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrlmac_ta_time_cb
(
  boolean no_tune_away,
    /* Whether tune away or not */

  qword ta_time
    /* Tune away time */
)
{
    
  hdramac_ta_time( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                   no_tune_away,
                   ta_time );

}  /* hdrlmac_ta_time_cb() */


/*===========================================================================

FUNCTION HDRLMAC_TA_STOP_ACCESS_CB

DESCRIPTION
  Callback function for HDR SRCH to stop access because of tune away. This
  function depends on the high prority of HDRTX task.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrlmac_ta_stop_access_cb
(
  void (*tx_stopped_cb)(void)
)
{
    hdramac_ta_stop_access_cmd( 
      HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
      ((hdramac_ta_stop_tx_cb_type) tx_stopped_cb) );

}  /* hdrlmac_ta_stop_access_cb() */


/*===========================================================================

FUNCTION HDRLMAC_ACMAC_READY

DESCRIPTION
  This function notifies HDR SRCH that ACMAC is ready for access 
  and passes the callback functions.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrlmac_acmac_ready ( void )
{
/* Build command into local struct*/
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ACMAC_READY_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
    
  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

}  /* hdrlmac_acmac_ready() */


/*===========================================================================

FUNCTION HDRLMAC_TA_TIME_OK

DESCRIPTION
  This function notifies HDR SRCH whether ACMAC considers the tune away 
  time ok or not.
  
DEPENDENCIES
  None

PARAMETERS
  ta_time_ok - Whether tune away time is ok

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrlmac_ta_time_ok ( boolean ta_time_ok )
{
/* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ACMAC_TA_TIME_OK_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.ta_time_ok = ta_time_ok;
  
    /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

}  /* hdrlmac_ta_time_ok() */

/*===========================================================================

FUNCTION HDRLMAC_ACMAC_DISABLE_TA

DESCRIPTION
  This function notifies HDR SRCH that ACACK has been received and 
  disables tune away for the duration specificed by no_ta_duration ms.
  
DEPENDENCIES
  None

PARAMETERS
  no_ta_duration - The duration that tune away should be disabled in ms.

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

void hdrlmac_acmac_disable_ta ( uint16 no_ta_duration )
{
/* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ACMAC_DISABLE_TA_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.no_ta_duration = no_ta_duration;
  
    /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

}  /* hdrlmac_acmac_disable_ta() */

#endif /* FEATURE_HDR_HYBRID_ACCESS */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ACCESS_HO_RPT_CB

DESCRIPTION
  Callback called from HDR searcher to report when SRCH wakes up from sleep.

DEPENDENCIES
  None

PARAMETERS
  link_id - The searcher-assigned ID for the new link we are switching to
  link_ptr - A pointer to a description of the new link

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_access_ho_rpt_cb
(
  const hdrsrch_link_id_type link_id,
  const sys_link_type *link_ptr
)
{
  /* Notify CC MAC of the new link */
  hdrcmac_update_link( link_id );

  hdrind_give_ind( HDRIND_LMAC_AHO_COMPLETE, (void *)&link_id);
  HDRTRACE_LMAC_ACCESS_HO_COMPLETE(link_ptr->pilot_pn);
  hdrplog_log_ho();
} /* hdrlmac_access_ho_rpt_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PERFORM_AHO

DESCRIPTION
  This command requires special processing by LMAC, so it is queued and
  handled in LMAC task context.

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_perform_aho (hdrhai_protocol_name_enum_type sender)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_PERFORM_AHO_CMD;
  hdrlmac_cmd.sender = sender;

  if ( rex_self(  ) == HDRTX_TASK_PTR )
  {
    ERR( "Command %d issued from LMAC task context", hdrlmac_cmd.name, 0, 0);
  }

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_perform_aho () */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PILOT_RPT_CB
  Callback called from HDR searcher to report that a significant pilot event
  has occurred.

  Note that this callback does not deliver any pilot measurements.  The upper
  layer must retrieve that information separately.

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_pilot_rpt_cb ( void )
{
  hdrind_give_ind( HDRIND_LMAC_SEND_PILOT_REPORT, NULL);
  HDRTRACE_LMAC_PILOT_MEAS_REQUIRED();
} /* hdrlmac_pilot_rpt_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PILOT_REPORT_RESET

DESCRIPTION
  This command is a passthrough interface for resetting the pilot reporting
  mechanism during connected state operation.  It directly calls
  hdrsrch_pilot_report_reset().
DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_pilot_report_reset (hdrhai_protocol_name_enum_type sender)
{
/* Build command into local struct*/ 
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_PILOT_REPORT_RESET_CMD;
  hdrlmac_cmd.sender = sender;
  
    /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_pilot_report_reset () */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_RL_UPDATE_COMPLETE_CB

DESCRIPTION
  This cb is called by MC RMAC when it is ready to receive a new TCA.
  For rev0/A, this function will be called after receiving 
  tc_assign_complete_cb sent by srch. 

DEPENDENCIES
  None

PARAMETERS
  None
                     
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_rl_update_complete_cb( void )
{
  hdrlmac_tca_buf_type *tca_buf_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rex_enter_crit_sect(&hdrlmac.crit_sect);
  hdrlmac.l1_is_processing_tca = FALSE;

  HDR_MSG_PROT_1(MSG_LEGACY_MED, "RL CB:Number of pending TCA %d", q_cnt(&hdrlmac.pending_tca_q));
  tca_buf_ptr = (hdrlmac_tca_buf_type *) q_get(&hdrlmac.pending_tca_q );

  rex_leave_crit_sect(&hdrlmac.crit_sect);

  if ( tca_buf_ptr != NULL )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Processing queued tca ... ");
    hdrlmac_tc_assign( tca_buf_ptr->rup_tca_params.initial_assignment,
                       tca_buf_ptr->rup_tca_params.link_id,
                       tca_buf_ptr->rup_tca_params.tc_tune_away,
                       tca_buf_ptr->rup_tca_params.cc_subactive_set_idx,
                       &tca_buf_ptr->rup_tca_params.tca_msg,
                       tca_buf_ptr->rup_tca_params.sender,
                       tca_buf_ptr->rup_tca_params.rup_subtype
                     );
    hdrbuf_done( &tca_buf_ptr->hdr );
    HDR_MSG_PROT_1(MSG_LEGACY_MED, "DEBUG:Number of pending TCA %d", q_cnt(&hdrlmac.pending_tca_q));
  }
} /* hdrlmac_rl_update_complete_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_TC_ASSIGN_COMPLETE_CB
  Callback called from HDR searcher to report that it has completed the
  traffic channel assignemnt it was given.

  There is no need for an indication that the active set has been changed.
  The upper layers can assume the traffic channel assignment is completed
  immediately, because even if they queue something for transmit the RTCMAC
  will not send it until the link settles on the new channel and active set.

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS
  tca_result - whether SRCH was able to process TCA successfully or not

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_tc_assign_complete_cb 
( 
  hdrsrch_tca_result_type tca_result 
)
{
  HDR_MSG_PROT(MSG_LEGACY_MED, "DEBUG: tc_assign_complete_cb");

  hdrfmac_set_awaiting_tc_assign_complete(FALSE);

#ifdef FEATURE_LTE_TO_HDR_OH
  if ( tca_result == HDRSRCH_TCA_SUCCEEDED )
#endif /* FEATURE_LTE_TO_HDR_OH */
  {
#ifdef FEATURE_HDR_REVC
    /* Notify HDRCP that TCA is processed and ASET is updated */
    hdrind_give_ind(HDRIND_LMAC_ASET_CHANGED, NULL);
#endif /* FEATURE_HDR_REVC */

    /* TODO: in later phase, RMAC will use single interface for both 
       rev0/A and B*/
    if ( hdrlmac.rup_subtype == HDRSCP_MC_RUP
  #ifdef FEATURE_LTE_TO_HDR_OH
         || hdrlmac.rup_subtype == HDRSCP_IRAT_MC_RUP
  #endif /* FEATURE_LTE_TO_HDR_OH */ 
       )
    {
      hdrrmac_update_rl_carriers( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                                  hdrlmac.rmac_params.num_carriers,
                                  hdrlmac.rmac_params.rl_carrier,
                                  hdrlmac.rmac_params.frame_offset,
                                  hdrlmac.rmac_params.dsc_gain_base,
                                  hdrlmac.rmac_params.num_subasets,
                                  hdrlmac.rmac_params.subaset_info,
                                  hdrlmac_rl_update_complete_cb);
    }

    hdrrmac_start_tx(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);

    if ( !( hdrlmac.rup_subtype == HDRSCP_MC_RUP
  #ifdef FEATURE_LTE_TO_HDR_OH
            || hdrlmac.rup_subtype == HDRSCP_IRAT_MC_RUP
  #endif /* FEATURE_LTE_TO_HDR_OH */
          )
       )
    {
      /* hdrlmac_rl_update_complete_cb() is NOT available in rev0/A, need to 
       * process queued TCA now. 
       * This part will be chaged later to use the same cb for rev0/A/B
       */
      hdrlmac_rl_update_complete_cb();
    }
#if defined(FEATURE_HDR_DEVICE_ONLY_DTX)|| defined FEATURE_HDR_REVB_DTX_MODE\
 || defined FEATURE_HDR_TRAFFIC_DTX
    hdrrmac_check_dtx_after_tca( FALSE );
      /* This is TCA complete cb for TCA of conn setup or HHO. So pass soft_ho==FALSE
         here.
      */
#endif /*  FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_REVB_DTX_MODE 
|| FEATURE_HDR_TRAFFIC_DTX */
  }
#ifdef FEATURE_LTE_TO_HDR_OH
  else if ( tca_result == HDRSRCH_TCA_FAILED )
  {
    hdrind_give_ind ( HDRIND_LMAC_TCA_FAILED, NULL );
  }
#endif /* FEATURE_LTE_TO_HDR_OH */

} /* hdrlmac_tc_assign_complete_cb */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_TC_SUSPEND_CB

DESCRIPTION
  Callback called from HDR searcher to report that it entered or left 
  suspended mode.

DEPENDENCIES
  None

PARAMETERS
  suspend_resume - Whether we are suspending or coming out of suspension.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_tc_suspend_cb
(
  hdrsrch_suspend_enum_type suspend_resume
)
{
  switch (suspend_resume)
  {
    /* We report suspension to the protocol stack for any of these two events.
     */
    case HDRSRCH_SUSPEND_STARTED:
    case HDRSRCH_SUSPEND_STILL_SUSPENDED:
      hdrind_give_ind( HDRIND_LMAC_TC_SUSPEND, NULL);
      break;

    /* We report resumes to the protocol stack for any of these two events. */
    case HDRSRCH_SUSPEND_NONE:
    case HDRSRCH_SUSPEND_ENDED:
      hdrind_give_ind( HDRIND_LMAC_TC_RESUME, NULL);
      break;
      
    default:
      /* There are other events that HDRSRCH may report using this callback,
       * but they are ignored for now.
       */
      break;
  }

} /* hdrlmac_tc_suspend_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_SCHDGRP_CHANGED_CB

DESCRIPTION
  Callback called from HDR searcher to report scheduler group changed 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_schdgrp_changed_cb (void)
{
  HDR_MSG_PROT(MSG_LEGACY_HIGH, "SCHEDULER GROUP CHANGED");

  hdrind_give_ind( HDRIND_LMAC_SCHEDULER_GROUP_CHANGED, NULL);

} /* hdrlmac_schdgrp_changed_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CREATE_SRCH_SUBACTIVE_SET

DESCRIPTION
  This function uses unpacked revB TCA fields to create srch subactive set list  

DEPENDENCIES
  tca_msg_ptr should point to valid memory.

PARAMETERS
  tca_msg_ptr - Pointer to the unpacked revB TCA 
  num_sub_asets - Number of sub-active sets 
  srch_subactive_set_ptr - Location where to store the srch sub-active set structure   

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL boolean hdrlmac_create_srch_subactive_set
(
  const hdrrup_tca_msg_type *tca_msg_ptr,
  const uint8                num_sub_asets,   
  hdrsrch_subactive_set_type *srch_subactive_set_ptr
)
{
  int subaset_id;    /* Sub-active set id*/
  int cnt;           /* Loop counter  */
  int num_pilots;    /* Number of pilots in each subactive set*/

  hdrsrch_aset_pilot_type aset_list[HDRSRCH_ASET_MAX_SIZE]; 
    /* storage for aset pilot*/
  
  hdrsrch_demod_idx_assignment_type demod_idx_assign[HDRSRCH_MAX_NUM_SUBASETS]; 
    /* Demod index obtained from srch*/ 

  boolean  tune_needed = FALSE;
  boolean change_softer_ho_to_false = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialization */
  memset((void *)&aset_list[0], 0, sizeof(hdrsrch_aset_pilot_type)*HDRSRCH_ASET_MAX_SIZE);

  for (subaset_id = 0; subaset_id < num_sub_asets; subaset_id++)
  {
    /* Initialize num_pilots for each sub-active set*/
    num_pilots = 0;

    change_softer_ho_to_false = FALSE;

    /* Channel this sub-active set is assigned.
     * Should be a list of channels, but AT only supports one channel (K=1) per sub-active 
     * set for revB (not only for phase1?), get the channel info from the first element in 
     * fwd_channel array 
     */
    srch_subactive_set_ptr[subaset_id].channel.band = 
      tca_msg_ptr->sub_aset[subaset_id].fwd_channel[0].channel.band;

    srch_subactive_set_ptr[subaset_id].channel.chan_num = 
      tca_msg_ptr->sub_aset[subaset_id].fwd_channel[0].channel.chan_num;


    HDR_MSG_PROT_3 (MSG_LEGACY_MED, "sub-active set: %d, band: %d, channel : %d",
                     subaset_id,
                     srch_subactive_set_ptr[subaset_id].channel.band,
                     srch_subactive_set_ptr[subaset_id].channel.chan_num);

    /* Whether this subactive set should not be reported. */
    srch_subactive_set_ptr[subaset_id].not_reportable = 
      tca_msg_ptr->sub_aset[subaset_id].not_reportable;

    /* Create srch active set pilot information */ 
    for (cnt = 0; cnt < tca_msg_ptr->num_sectors; cnt++)
    {
      if (tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].pilot_included)
      {
        /* pilot_pn*/
        aset_list[num_pilots].pilot_pn = tca_msg_ptr->sector_info[cnt].pilot_pn;

        /* softer_handoff*/
        if (change_softer_ho_to_false && (tca_msg_ptr->sector_info[cnt].softer_handoff == 1))
        {
          HDR_MSG_PROT_2 (MSG_LEGACY_MED, "Changing SofterHandoff to 0 for subaset %d, sector %d",
                           subaset_id, cnt);//auto-gen, to change remove 'QSR_' and first param

          aset_list[num_pilots].softer_handoff = 0;
        }
        else
        {
          aset_list[num_pilots].softer_handoff = tca_msg_ptr->sector_info[cnt].softer_handoff;
        }

        /* mac_index */
        aset_list[num_pilots].mac_index = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].ftc_params[0].ftc_mac_index;

        /* drc_cover*/
        aset_list[num_pilots].drc_cover = tca_msg_ptr->sector_info[cnt].drc_cover;

        /* aux_drc_cover */
        aset_list[num_pilots].aux_drc_cover = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].aux_drc_cover;

        /* rl_mac_index :
         * Use the first element of rtc_params because we only supporte one rev ch (for phase1?)
         */
        aset_list[num_pilots].rl_mac_index = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].rtc_params[0].rev_mac_index;

        /* rab_mac_index*/
        aset_list[num_pilots].rab_mac_index = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].rtc_params[0].rab_mac_index;

        /* delta_t2p*/
        aset_list[num_pilots].delta_t2p = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].rtc_params[0].delta_t2p;

        /* ra_channel_gain*/
        aset_list[num_pilots].ra_channel_gain = tca_msg_ptr->sector_info[cnt].ra_channel_gain;

        /* dsc*/
        aset_list[num_pilots].dsc = tca_msg_ptr->sector_info[cnt].dsc;

        /* pilot_group_id*/
        aset_list[num_pilots].pilot_group_id = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].pilot_group_id;

        /* scheduler_tag*/
        aset_list[num_pilots].scheduler_tag = 
          tca_msg_ptr->sub_aset[subaset_id].sector_params[cnt].scheduler_tag;

        /* Following two fields are for rev0/A only*/
        aset_list[num_pilots].rab_length = 
          tca_msg_ptr->sector_info[cnt].rab_length;

        aset_list[num_pilots].rab_offset = 
          tca_msg_ptr->sector_info[cnt].rab_offset;

        num_pilots++;
        change_softer_ho_to_false = FALSE;
      }
      else if (tca_msg_ptr->sector_info[cnt].softer_handoff == 0)
      {
        /* If SofterHandoff is 0 and PilotInThisSectorIncluded is 0
         * We need to let srch know by giving the SofterHandoff of next sector as '0'
         */
        change_softer_ho_to_false = TRUE;
      }
    }

    srch_subactive_set_ptr[subaset_id].num_aset_pilots = num_pilots;

    HDR_MSG_PROT_2 (MSG_LEGACY_LOW, "sub-active set: %d, num of pilots: %d", 
                    subaset_id, num_pilots);

    /* Store active set pilot information */
    for (cnt = 0; cnt < num_pilots; cnt++ )
    {
      srch_subactive_set_ptr[subaset_id].aset_list[cnt] = aset_list[cnt];
    }
  }

  /* Fixing KW errors */ 
  HDR_ASSERT(num_sub_asets <= HDRSRCH_MAX_NUM_SUBASETS);
  
  tune_needed = hdrsrch_assign_demod_idxes_to_subasets( num_sub_asets,
                                          srch_subactive_set_ptr,
                                          demod_idx_assign);

  for (cnt = 0; cnt < num_sub_asets; cnt++)
  {
    /* Store demod index*/
    srch_subactive_set_ptr[cnt].demod_idx = demod_idx_assign[cnt].demod_idx;
    HDR_MSG_PROT_2 (MSG_LEGACY_LOW, "DEBUG: subaset %d, demod_idx: %d", 
                    cnt, srch_subactive_set_ptr[cnt].demod_idx);
  }

  return tune_needed;
} /* hdrlmac_create_srch_subactive_set*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_FEEDBACK_RL_CHANNEL

DESCRIPTION
  This function is used to get the channel record of reverse link CDMA channel
  on which the DSC, DRC, and ACK channels are sent associated with the 
  sub-Active Set  

DEPENDENCIES
  tca_msg_ptr should point to valid memory with rev_channel[].a_order set  

PARAMETERS
  fb_rev_chan - reverse link channel  
  tca_msg_ptr - Pointer to the unpacked revB TCA 
  ascending_order - refer to the reverse link channel in ascending order
                    of frequency  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL hdrerrno_enum_type  hdrlmac_get_feedback_rl_channel
(
  sys_channel_type *fb_rev_chan_ptr,
  const hdrrup_tca_msg_type *tca_msg_ptr,
  const uint8 ascending_order
)
{
  int subaset_id;       /* sub-active set id*/
  int cnt;              /* Loop counter  */
  hdrerrno_enum_type result = E_FAILURE;
/*-----------------------------------------------------------------------*/

  /* *fb_rev_chan_ptr is a returned value.  Make sure it is always set */
  fb_rev_chan_ptr->band = 0;
  fb_rev_chan_ptr->chan_num = 0;

  for (subaset_id = 0; subaset_id < tca_msg_ptr->num_sub_asets; subaset_id++)
  {
    if (result == E_SUCCESS)
    {
      /* found rev channel*/
      break;
    }

    for (cnt=0; cnt < tca_msg_ptr->sub_aset[subaset_id].num_rev_channels; cnt++)
    {
      if (tca_msg_ptr->sub_aset[subaset_id].rev_channel[cnt].a_order == ascending_order)
      {
        fb_rev_chan_ptr->band = tca_msg_ptr->sub_aset[subaset_id].rev_channel[cnt].channel.band;
        fb_rev_chan_ptr->chan_num = tca_msg_ptr->sub_aset[subaset_id].rev_channel[cnt].channel.chan_num;
        result = E_SUCCESS;
        break;
      }
    }
  }

  return result;
} /* hdrlmac_get_feedback_rl_channel */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CREATE_RMAC_PARAMS

DESCRIPTION
  This function uses unpacked revB TCA fields to create params for RMAC4  
  srch.

DEPENDENCIES
  tca_msg_ptr should point to valid memory.

PARAMETERS
  tca_msg_ptr - Pointer to the unpacked revB TCA 
  rmac_params_ptr - Location where to store the rmac tcam params  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_create_rmac_params
(
  const hdrrup_tca_msg_type *tca_msg_ptr,
  hdrlmac_rmac_params_type *rmac_params_ptr
)
{
  int subaset_id;       /* sub-active set id*/
  int cnt;              /* Loop counter  */
  int num_rl_carriers;  /* number of rl carriers in TCA*/
  sys_channel_type fb_rev_chan; 
    /* Rev channel on which the DSC/DRC and ACK channels are sent*/
  hdrerrno_enum_type result = E_FAILURE; 
/*-----------------------------------------------------------------------*/
  num_rl_carriers = 0;

  for (subaset_id = 0; subaset_id < tca_msg_ptr->num_sub_asets; subaset_id++)
  {
    for (cnt=0; cnt < tca_msg_ptr->sub_aset[subaset_id].num_rev_channels; cnt++)
    {
      rmac_params_ptr->rl_carrier[num_rl_carriers].channel_record = 
         tca_msg_ptr->sub_aset[subaset_id].rev_channel[cnt].channel;

      rmac_params_ptr->rl_carrier[num_rl_carriers].drop_rank = 
         tca_msg_ptr->sub_aset[subaset_id].rev_channel[cnt].drop_rank;
      
      rmac_params_ptr->rl_carrier[num_rl_carriers].rl_chan_index = 
        hdrlmac.srch_subactive_set[subaset_id].demod_idx;

      rmac_params_ptr->rl_carrier[num_rl_carriers].subaset_index = subaset_id;

      HDR_MSG_PROT_4( MSG_LEGACY_MED, "saset %d, rl_chan_index: %d, band %d, chan_num %d",
                      rmac_params_ptr->rl_carrier[num_rl_carriers].subaset_index,
                      rmac_params_ptr->rl_carrier[num_rl_carriers].rl_chan_index,
                      rmac_params_ptr->rl_carrier[num_rl_carriers].channel_record.band,
                      rmac_params_ptr->rl_carrier[num_rl_carriers].channel_record.chan_num);

      num_rl_carriers++;
    } /* number of rl carriers*/

    rmac_params_ptr->subaset_info[subaset_id].subaset_index = subaset_id;

    rmac_params_ptr->subaset_info[subaset_id].gains.ack = 
      tca_msg_ptr->sub_aset[subaset_id].ftc_params.ack_channel_gain;
    rmac_params_ptr->subaset_info[subaset_id].gains.drc = 
      tca_msg_ptr->sub_aset[subaset_id].ftc_params.drc_channel_gain;
    rmac_params_ptr->subaset_info[subaset_id].dsc_enabled = 
      tca_msg_ptr->sub_aset[subaset_id].ftc_params.dsc_enabled;

    result = hdrlmac_get_feedback_rl_channel( &fb_rev_chan, tca_msg_ptr,
      tca_msg_ptr->sub_aset[subaset_id].ftc_params.feedback_rev_ch_id);
    if (result != E_SUCCESS)
    {
      ERR_FATAL( "Can't get correct FeedbackReverseChannel", 0, 0, 0 );
    }

    rmac_params_ptr->subaset_info[subaset_id].feedback_rl_channel_record = fb_rev_chan;
  
    rmac_params_ptr->subaset_info[subaset_id].reportable = 
        (tca_msg_ptr->sub_aset[subaset_id].not_reportable == 0) ? TRUE: FALSE;
  }

  rmac_params_ptr->num_carriers = num_rl_carriers;
  rmac_params_ptr->frame_offset = tca_msg_ptr->frame_offset;;
  rmac_params_ptr->dsc_gain_base = tca_msg_ptr->dsc_channel_gain;;
  rmac_params_ptr->num_subasets = tca_msg_ptr->num_sub_asets;;
} /* hdrlmac_create_rmac_params */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CREATE_FMAC_PARAMS

DESCRIPTION
  This function uses unpacked revB TCA fields to create FMAC TCA params  

DEPENDENCIES
   hdrlmac.srch_subactive_set[sub_aset].demode_idx needs to contain correct 
   value. (hdrlmac_create_srch_subactive_set() needs to be called first)

PARAMETERS
  tca_msg_ptr - Pointer to the unpacked revB TCA 
  fmac_params_ptr - Location where to store the fmac tcam params  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_create_fmac_params
(
  const hdrrup_tca_msg_type *tca_msg_ptr,
  hdrfmac_tcam_params_type *fmac_params_ptr
)
{
  uint8 carrier_id; 
 
  fmac_params_ptr->dsc_channel_gain = tca_msg_ptr->dsc_channel_gain;
  fmac_params_ptr->frame_offset = tca_msg_ptr->frame_offset;
  fmac_params_ptr->feedback_mux_enabled = tca_msg_ptr->feedback_mux_enabled;
  fmac_params_ptr->num_carriers = tca_msg_ptr->num_sub_asets;

  for (carrier_id = 0; carrier_id < fmac_params_ptr->num_carriers; carrier_id++)
  {
    fmac_params_ptr->sub_aset[carrier_id].demod_id =  
      hdrlmac.srch_subactive_set[carrier_id].demod_idx;

    fmac_params_ptr->sub_aset[carrier_id].drc_length = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.drc_length;
    fmac_params_ptr->sub_aset[carrier_id].drc_channel_gain = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.drc_channel_gain;
    fmac_params_ptr->sub_aset[carrier_id].ack_channel_gain = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.ack_channel_gain;

    fmac_params_ptr->sub_aset[carrier_id].fm.feedback_enabled = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.feedback_enabled;
    fmac_params_ptr->sub_aset[carrier_id].fm.feedback_mux_index = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.feedback_mux_id;
    fmac_params_ptr->sub_aset[carrier_id].fm.feedback_rev_chan_index = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.feedback_rev_ch_id;
    fmac_params_ptr->sub_aset[carrier_id].fm.dsc_enabled = 
      tca_msg_ptr->sub_aset[carrier_id].ftc_params.dsc_enabled;

  }

} /* hdrlmac_create_fmac_params*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CREATE_CHANNEL_LIST

DESCRIPTION
  This function is used to create a list of fwd cdma channels included in the TCA   

DEPENDENCIES

PARAMETERS
  chan_cnt - Location where to store the number of channel 
  chan_list_ptr - Location where to store the channel list  
  chan_size - maximum number of channels allowed  
  tca_msg_ptr - Pointer to the unpacked revB TCA 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_create_channel_list
(
  uint8 *chan_cnt,
  sys_channel_type *chan_list_ptr,
  uint8 chan_size,
  const hdrrup_tca_msg_type *tca_msg_ptr
)
{
  uint8 saset_id;
  uint8 chan_id;
  uint8 num_chans=0;

  for (saset_id = 0; saset_id < tca_msg_ptr->num_sub_asets; saset_id++)
  {
    for (chan_id = 0; chan_id < tca_msg_ptr->sub_aset[saset_id].num_fwd_channels; chan_id++)
    {
      chan_list_ptr[num_chans].band = 
        tca_msg_ptr->sub_aset[saset_id].fwd_channel[chan_id].channel.band;

      chan_list_ptr[num_chans].chan_num = 
        tca_msg_ptr->sub_aset[saset_id].fwd_channel[chan_id].channel.chan_num;

      HDR_MSG_PROT_4(MSG_LEGACY_MED,"saset %d chan %d, band %d, chan_num %d",
                     saset_id, chan_id, chan_list_ptr[num_chans].band,
                     chan_list_ptr[num_chans].chan_num);

      num_chans++; 

      if (num_chans == chan_size)
      {
        /* Number of channels should not be greater than chan_size*/
        ASSERT((saset_id == (tca_msg_ptr->num_sub_asets - 1)) &&
        (chan_id == (tca_msg_ptr->sub_aset[saset_id].num_fwd_channels -1)));
      }
    }
  }

  *chan_cnt = num_chans;

} /* hdrlmac_create_channel_list*/

#ifdef FEATURE_HDR_IS890
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CREATE_CARRIER_MAP

DESCRIPTION
  This function uses unpacked MC TCA fields to create a carrier map used for
  MC TAP.

DEPENDENCIES
  tca_msg_ptr should point to valid memory.

PARAMETERS
  tca_msg_ptr - Pointer to the unpacked MC TCA 
  num_sub_asets - Number of sub-active sets 
  carrier_map_ptr - Location where to store the carrier_map_type structure   
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_create_carrier_map
(
  const hdrrup_tca_msg_type *tca_msg_ptr,
    /* Pointer to the unpacked MC TCA */
  const uint8                num_sub_asets,   
    /* Number of sub-active sets */
  hdrlmac_carrier_map_type *carrier_map_ptr
    /* Location where to store the carrier_map_tupe structure*/
)
{
  uint8   sas_idx; 
  boolean map_changed = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Loop through subactive sets and fill out carrier_map */
  for (sas_idx = 0; sas_idx < num_sub_asets; sas_idx++)
  {
    if( ( carrier_map_ptr[sas_idx].carrier_index != 
          hdrlmac.srch_subactive_set[sas_idx].demod_idx )                   || 
        ( carrier_map_ptr[sas_idx].fwd_channel.band !=
          tca_msg_ptr->sub_aset[sas_idx].fwd_channel[0].channel.band )      ||
        ( carrier_map_ptr[sas_idx].fwd_channel.chan_num != 
          tca_msg_ptr->sub_aset[sas_idx].fwd_channel[0].channel.chan_num )  ||
        ( carrier_map_ptr[sas_idx].rev_channel.band != 
          tca_msg_ptr->sub_aset[sas_idx].rev_channel[0].channel.band )      ||
        ( carrier_map_ptr[sas_idx].rev_channel.chan_num != 
          tca_msg_ptr->sub_aset[sas_idx].rev_channel[0].channel.chan_num ) )
    {
      map_changed = TRUE;
        /* If any of the new values differ from the old ones, mark that the 
        carrier map has changed */
    }
    
    carrier_map_ptr[sas_idx].carrier_index = 
      hdrlmac.srch_subactive_set[sas_idx].demod_idx;

    carrier_map_ptr[sas_idx].fwd_channel.band = 
      tca_msg_ptr->sub_aset[sas_idx].fwd_channel[0].channel.band;
    carrier_map_ptr[sas_idx].fwd_channel.chan_num = 
      tca_msg_ptr->sub_aset[sas_idx].fwd_channel[0].channel.chan_num;

    carrier_map_ptr[sas_idx].rev_channel.band =
      tca_msg_ptr->sub_aset[sas_idx].rev_channel[0].channel.band;
    carrier_map_ptr[sas_idx].rev_channel.chan_num = 
      tca_msg_ptr->sub_aset[sas_idx].rev_channel[0].channel.chan_num;
  }

  /* Set all remaining values to default */
  for ( sas_idx = num_sub_asets; 
        sas_idx < HDRSRCH_MAX_NUM_SUBASETS; 
        sas_idx++ )
  {
    if( ( carrier_map_ptr[sas_idx].carrier_index != 
          HDRLMAC_INVALID_CARRIER_INDEX )                     || 
        ( carrier_map_ptr[sas_idx].fwd_channel.chan_num != 
          HDRLMAC_INVALID_CHANNEL )                           ||
        ( carrier_map_ptr[sas_idx].rev_channel.chan_num != 
          HDRLMAC_INVALID_CHANNEL ) )
    {
      map_changed = TRUE;
        /* If any of the new values differ from the old ones, mark that the 
           carrier map has changed */
    }

    carrier_map_ptr[sas_idx].carrier_index = HDRLMAC_INVALID_CARRIER_INDEX;
      /* Any carrier_index value greater than max possible is invalid */

    carrier_map_ptr[sas_idx].fwd_channel.chan_num = HDRLMAC_INVALID_CHANNEL;
      /* Channel 0 is invalid */

    carrier_map_ptr[sas_idx].rev_channel.chan_num = HDRLMAC_INVALID_CHANNEL;
      /* Channel 0 is invalid */
  }

  if( map_changed == TRUE )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Carrier Map Changed" );
    hdrind_give_ind( HDRIND_LMAC_CARRIER_MAP_CHANGED, NULL );
      /* If carrier map has changed, give indication */
  }
  
} /* hdrlmac_create_carrier_map */
#endif /* FEATURE_HDR_IS890 */

#ifdef FEATURE_HDR_REVC
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_ASET_UPDATE_COMPLETE_CB

DESCRIPTION
  This function is called when ASET update is completed by HDRSRCH

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_aset_update_complete_cb( void )
{
  hdrind_give_ind(HDRIND_LMAC_ASET_CHANGED, NULL);
}
#endif /* FEATURE_HDR_REVC */
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_TC_ASSIGN

DESCRIPTION
  This command requires special processing by LMAC, so it is queued and
  handled in LMAC task context.

DEPENDENCIES
  None

PARAMETERS
  initial_assignment - Whether this is the initial channel assignment
  link_id            - Link ID for which the pilot information is needed
  tc_tune_away       - If 1x tune-away is allowed
  cc_subactive_set_idx - The sub-active set that carries Control Channel 
  tca_msg_ptr          - Pointer to a structure that contains MC RUP msg 
  sender               - Protocol calling the function, for debug
  rup_subtype          - MC or Default RUP

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_tc_assign
(
  boolean initial_assignment,
  hdrsrch_link_id_type link_id,
  hdrsrch_traffic_tune_away_enum_type  tc_tune_away,
  uint8 cc_subactive_set_idx,
  hdrrup_tca_msg_type * tca_msg_ptr,
  hdrhai_protocol_name_enum_type sender,
  uint16 rup_subtype
)
{
  hdrlmac_cmd_type hdrlmac_cmd; 
    /* Build command into local struct */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* This is a hard handoff that involves more processing than just a
   * function call to SRCH, so queue it and process in LMAC task 
   */

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_TC_ASSIGN_CMD;
  hdrlmac_cmd.sender = sender;
  hdrlmac_cmd.params.tc_assign.link_id = link_id;
  hdrlmac_cmd.params.tc_assign.tc_tune_away = tc_tune_away;
  hdrlmac_cmd.params.tc_assign.rup_subtype = rup_subtype;
  hdrlmac_cmd.params.tc_assign.cc_subactive_set_idx = cc_subactive_set_idx;
  hdrlmac_cmd.params.tc_assign.initial_assignment = initial_assignment;

  if( tca_msg_ptr )
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    hdrlmac_cmd.params.tc_assign.tca_msg = *tca_msg_ptr;
#else
    memcpy( &hdrlmac_cmd.params.tc_assign.tca_msg, tca_msg_ptr,
            sizeof(hdrrup_tca_msg_type) );
#endif /* FEATURE_MEMCPY_REMOVAL */
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_ERROR, "tca_msg_ptr is NULL" );
  }

  if ( rex_self() == HDRTX_TASK_PTR )
  {
    HDR_MSG_PROT_1( MSG_LEGACY_MED, "TC_ASSIGNED_CMD issued from LMAC",
                    hdrlmac_cmd.name );
    hdrlmac_process_cmd( &hdrlmac_cmd );
  }
  else
  {
    hdrtx_queue_cmd( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                     &hdrlmac_cmd, sizeof(hdrlmac_cmd_type) );
  }

} /* hdrlmac_tc_assign() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CHAN_MEAS_CB

DESCRIPTION
  This callback is called when searcher completes a channel measurement.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_chan_meas_cb ( void )
{
  hdrind_give_ind( HDRIND_LMAC_CHAN_MEAS_COMPLETE, NULL );
  HDRTRACE_LMAC_CHAN_MEAS_COMPLETE();
} /* hdrlmac_chan_meas_cb() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CHAN_MEAS

DESCRIPTION
  This command is a passthrough interface for requesting HDRSRCH to perform
  a measurement on a list of channels.  It directly calls hdrsrch_chan_meas()
  passing a callback function.

DEPENDENCIES
  None

PARAMETERS
  meas_method - Rx power or time correlation measurement
  chan_list_ptr - List of channels to measure
  meas_value_list_ptr - Pointer to array in which the lower layer is to place
                        its measurements
  num_chans - Number of channels to be measured
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_chan_meas
(
  hdrsrch_cmeas_method_enum_type meas_method,
  sys_channel_type * chan_list_ptr,
  hdrsrch_cmeas_value_type * meas_value_list_ptr,
  uint8 num_chans,
  hdrhai_protocol_name_enum_type sender
)
{
//DA need to implement this as soon as SD is ready
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_CHANNEL_MEASUREMENT_CMD, sender, 0 );
  ERR( "Channel Measurment not supported at this time",0,0,0);
} /* hdrlmac_chan_meas() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_TC_ESTABLISHED

DESCRIPTION
  This command is a passthrough interface for informing hdr searcher 
  that the traffic channel has been established.

DEPENDENCIES
  None

PARAMETERS
  sender - Protocol calling the function, for debug

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_tc_established (hdrhai_protocol_name_enum_type sender)
{

/* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_TC_ACK_RECEIVED_CMD;
  hdrlmac_cmd.sender = sender;

    /* This function copies the command into a buffer before it returns*/
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_tc_established() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_RUP_REQUEST_CB

DESCRIPTION
  Callback called from HDR searcher to report that a significant pilot event
  has occurred for the RUP Request.

  Note that this callback does not deliver any pilot measurements.  The upper
  layer must retrieve that information separately.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_rup_request_cb ( void )
{
  /* Give the LMAC_SEND_PILOT_REPORT indication for sending out
   * RouteUpdate message */
  hdrind_give_ind( HDRIND_LMAC_SEND_PILOT_REPORT, NULL);
  HDRTRACE_LMAC_PILOT_MEAS_REQUIRED();

  /* Indication that pilot report for RUP Request has been 
   * received, so RUP can pass the pending Request to Search
   * if there are any */
  hdrind_give_ind( HDRIND_RUP_PILOT_RPT_RECEIVED, NULL);

} /* hdrlmac_rup_request_cb */



/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_RUP_REQ

DESCRIPTION
  This is just a wrapper for hdrsrch_rup_request.

DEPENDENCIES
  None

PARAMETERS
  route_update_request_ptr - Pointer to structure that cotains the parameters 
         for the route update message from protocols.

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void hdrlmac_rup_request
(
  hdrsrch_rup_req_params_type     *route_update_request_ptr
    /* pointer to RUP request params structure */
)
{
  hdrsrch_rup_request( route_update_request_ptr, hdrlmac_rup_request_cb );
    /* call hdrsrch function to post route update command */
    
} /* hdrlmac_rup_request_message() */



/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_OPMODE_POOR_COVERAGE_CB
      
DESCRIPTION
  Callback called from HDR searcher to report that a poor HDR coverage 
  has detected so AT should switch to dual mode (HDR/1x) operation.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_opmode_poor_coverage_cb( void )
{
  hdrind_lmac_rf_change_enum_type rf_cond = HDRIND_LMAC_RF_CONDITION_BAD;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrind_give_ind( HDRIND_LMAC_RF_CONDITION_CHANGED, &rf_cond );

} /* hdrlmac_opmode_poor_coverage_cb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_OPMODE_GOOD_COVERAGE_CB
      
DESCRIPTION
  Callback called from HDR searcher to report that a good HDR coverage 
  has detected so AT should switch to HDR only mode operation

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrlmac_opmode_good_coverage_cb( void )
{
  hdrind_lmac_rf_change_enum_type rf_cond = HDRIND_LMAC_RF_CONDITION_GOOD;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrind_give_ind( HDRIND_LMAC_RF_CONDITION_CHANGED, &rf_cond );

} /* hdrlmac_opmode_poor_coverage_cb */



/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_CONFIGURE_AT_OPERATIONAL_MODE

DESCRIPTION
  This is just a wrapper for hdrsrch_configure_at_operational_mode.

DEPENDENCIES
  None

PARAMETERS
  config - which at operational mode algorithm if any to run.

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void hdrlmac_configure_at_operational_mode
(
  hdrsrch_opmode_config_enum_type      config
    /* which at operational mode algorithm if any to run */
)
{
  hdrsrch_configure_at_operational_mode(config, 
                                        hdrlmac_opmode_poor_coverage_cb,
                                        hdrlmac_opmode_good_coverage_cb);

} /* hdrlmac_configure_at_operational_mode() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_SET_TC_HANDDOWN_MODE

DESCRIPTION  : This function sets the handdown mode during  HDR connected
               state. Handdown can be enabled or disabled 
               during HDR traffic connection.

  HDRSRCH_1X_HANDDOWN_DISABLED_MODE - Handdown to is 1x disabled.
  HDRSRCH_1X_HANDDOWN_ENABLED_MODE  - Handdown to 1x is enabled.

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_set_tc_handdown_mode
( 
  hdrsrch_tc_handdown_mode_enum_type   handdown_mode
    /* the handdown mode selected */
)
{
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_set_tc_handdown_mode( handdown_mode );
    /* set handdown mode in traffic */

} /* hdrlmac_set_tc_handdown_mode */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_PILOTS

DESCRIPTION
  This is just a wrapper for hdrsrch_get_pilot_report().

DEPENDENCIES
  None

PARAMETERS
  max_pilots - Less than or equal to size of array pointed to by pilots_ptr
  pilots_ptr - Pointer to array of structures comprising all the active and
               candidate set pilots.  The first pilot is the reference pilot.
  caller - Protocol calling the function, for debug

RETURN VALUE
  int - number of pilot records in the array pointed to by pilots_ptr

SIDE EFFECTS

===========================================================================*/

int hdrlmac_get_pilots
(
  int max_pilots,
  hdrsrch_pilot_rpt_type * pilots_ptr,
  hdrhai_protocol_name_enum_type caller
)
{
  return (hdrsrch_get_pilot_report (max_pilots, pilots_ptr ));
} /* hdrlmac_get_pilots() */ /*lint !e715  'caller' not used */

/* <EJECT> */
/*==========================================================================

FUNCTION HDRLMAC_GET_SCHDGRP_INFO

DESCRIPTION
  Get the scheduler group report.

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  May cause an immediate reschedule of searches.

==========================================================================*/

void hdrlmac_get_schdgrp_info
( 
  hdrsrch_schdgrp_rpt_type *schdgrp_info_ptr 
)
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, "hdrlmac_get_schdgrp_info");

  hdrsrch_get_schdgrp_rpt(schdgrp_info_ptr);

} /* hdrlmac_get_schdgrp_info */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_GET_LINK_INFO

DESCRIPTION
  This is just a wrapper for hdrsrch_get_link_info().  Currently the upper
  layers can not recover from the link ID not being found, so we ERR_FATAL
  if searcher returns an error.

DEPENDENCIES
  None

PARAMETERS
  link_id - The searcher-assigned ID for the link we want info about
  link_ptr - A pointer to a description of the link with the given link_id

RETURN VALUE
  None

SIDE EFFECTS
  Calls ERR_FATAL if the link ID is not recognized by the searcher.

===========================================================================*/

void hdrlmac_get_link_info
(
  hdrsrch_link_id_type link_id,
  sys_link_type * link_ptr
)
{
  if(hdrsrch_get_link_info( link_id, link_ptr ) != E_SUCCESS)
  {
    if(link_id != HDRSRCH_REF_LINK_ID)
    {
      ERR( "Unknown link ID: %d!",link_id, 0, 0);
      /* If there is some unexpected error preventing us from getting the 
       * link info, still continue if we can give the info about the 
       * current link.  Most of the time this will be the correct info,
       * and in the case where it is wrong, the side effect is not fatal.
       */
      if(hdrsrch_get_link_info( HDRSRCH_REF_LINK_ID, link_ptr ) != E_SUCCESS)
      {
        ERR_FATAL("No Reference Link either!",0,0,0);
      }
    }
    else
    {
      ERR( "No Reference Link",0,0,0);
    }
  }

  HDR_MSG_PROT_2( MSG_LEGACY_LOW, "hdrlmac_get_link_info chan:%d pn:%d", 
                  link_ptr->channel.chan_num,
                  link_ptr->pilot_pn );
} /* hdrlmac_get_link_info() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_IDLE_CHAN_CHANGE_CMD

DESCRIPTION
  LMAC discards this command and gives an error if it is received while the
  RTCMAC is active.  If the ACMAC is active, the access attempt in progress
  must be stopped before the channel is changed.  In this case LMAC calls
  hdramac_stop() to stop the access attempt and waits for the
  AccessChannelMAC.TxEnded indication before proceeding with the channel
  change.

  If the ACMAC is not active the LMAC immediately calls
  hdrsrch_idle_chan_change(), passing in a callback function.

DEPENDENCIES
  None

PARAMETERS
  lmac_cmd_ptr - Pointer to the LMAC_IDLE_CHAN_CHANGE command to be processed.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_process_idle_chan_change_cmd
(
  hdrlmac_cmd_type * lmac_cmd_ptr
)
{
  HDR_MSG_PROT_1(MSG_LEGACY_MED, "LMAC_PROC_IDLE_CHAN_CHG sleep? %d",
                 lmac_cmd_ptr->params.idle_chan_change.channel_change_in_sleep);

  if (hdrrmac_is_active(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    ERR( "Can't change channel while connection is open", 0, 0, 0);
    return;
  }

  if (hdramac_is_in_attempt(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    /* Command ACMAC to stop probing and wait for response before changing
     * channels.
     */
    hdramac_stop(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);
    hdrlmac_queue_pending_cmd( lmac_cmd_ptr );
    HDRTRACE_LMAC_WAIT_FOR_ACMAC_ABORT();
  }
  else
  {
    HDRTRACE_LMAC_IDLE_CHAN_CHANGE(
      lmac_cmd_ptr->params.idle_chan_change.link.channel.chan_num );

    if (!lmac_cmd_ptr->params.idle_chan_change.channel_change_in_sleep)
    {      
      hdrind_give_ind(HDRIND_LMAC_ACCESS_ENTRY_HANDOFF, NULL);
    }
    /* A channel change cmd could be issued in sleep state when a BCMCS
       flow request is received while sleeping. The AccessEntryHandoff
       indication might not be needed (to be confirmed from logging)
       as there can't be any pending messages to handle.*/

    hdrsrch_idle_channel_change(
      &(lmac_cmd_ptr->params.idle_chan_change.link),
      hdrlmac_chan_change_rpt_cb );
  }
} /* hdrlmac_process_idle_chan_change_cmd() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_IDLE_CHAN_CHG_AND_NSET_UPDATE_CMD

DESCRIPTION
  LMAC discards this command and gives an error if it is received while the
  RTCMAC is active.  If the ACMAC is active, the access attempt in progress
  must be stopped before the channel is changed.  In this case LMAC calls
  hdramac_stop() to stop the access attempt and waits for the
  AccessChannelMAC.TxEnded indication before proceeding with the channel
  change.

  If the ACMAC is not active the LMAC immediately calls
  hdrsrch_idle_chan_change(), passing in a callback function.

DEPENDENCIES
  None

PARAMETERS
  lmac_cmd_ptr - Pointer to the LMAC_IDLE_CHAN_CHANGE command to be processed.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_process_idle_chan_chg_and_nset_update_cmd
(
  hdrlmac_cmd_type * lmac_cmd_ptr
)
{
  /* Convenience pointer */
  hdrlmac_idle_chan_chg_and_nset_update_params_type *ptr =
    &lmac_cmd_ptr->params.idle_chan_chg_and_nset_update;
  hdrsrch_idle_channel_change_type idle_cc_params; /* Idle channel change params */
  hdrsrch_nset_update_type nset_update_params; /* Nset update params */
  uint8 cnt; /* iterator*/ 
  sys_channel_type avoided_channel_list[HDRSRCH_AVOIDED_CHAN_MAX_SIZE];

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT_2( MSG_LEGACY_MED, 
                  "LMAC_PROC_IDLE_CHAN_CHG_AND_NSET_UPDATE sleep? %d, PilotGID %d",
                  ptr->channel_change_in_sleep, ptr->serving_pilot_gid );

  if (hdrrmac_is_active(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    ERR( "Can't change channel while connection is open", 0, 0, 0);
    return;
  }

  if (hdramac_is_in_attempt(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    /* Command ACMAC to stop probing and wait for response before changing
     * channels.
     */
    hdramac_stop(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);
    hdrlmac_queue_pending_cmd( lmac_cmd_ptr );
    HDRTRACE_LMAC_WAIT_FOR_ACMAC_ABORT();
  }
  else
  {
    HDRTRACE_LMAC_IDLE_CHAN_CHANGE( ptr->link.channel.chan_num );

    if (!ptr->channel_change_in_sleep)
    {      
      hdrind_give_ind(HDRIND_LMAC_ACCESS_ENTRY_HANDOFF, NULL);
    }

    idle_cc_params.link_ptr = &ptr->link;
    idle_cc_params.chan_change_rpt_cb = hdrlmac_chan_change_rpt_cb;

    nset_update_params.num_nset_pilots = ptr->num_pilots;
    nset_update_params.nset_list = ptr->nset_pilots;
    nset_update_params.struct_size = sizeof(hdrsrch_nset_pilot_type); 
    nset_update_params.update_type = ptr->update_type;
    nset_update_params.num_channels = ptr->num_channels;
    nset_update_params.channel_list_ptr = ptr->channel_list;
    nset_update_params.num_avoided_channels = ptr->num_avoided_channels;
    /* Convert sys_channel_record_type to sys_channel_type*/
    for (cnt = 0; cnt < nset_update_params.num_avoided_channels; cnt++)
    {
      avoided_channel_list[cnt] = ptr->avoided_list[cnt].channel;
    }
    nset_update_params.avoided_channel_list_ptr = avoided_channel_list;
    nset_update_params.num_sectors = ptr->num_sects;
    nset_update_params.sector_list_ptr = ptr->cached_sects;
#ifdef FEATURE_HDR_REVB
    nset_update_params.hdr_subtype = hdrscp_get_current_hdr_rev();
    nset_update_params.serving_pilot_gid = ptr->serving_pilot_gid;
#endif /* FEATURE_HDR_REVB */

    hdrsrch_idle_channel_change_and_nset_update(
      &idle_cc_params, &nset_update_params);
  }

} /* hdrlmac_process_idle_chan_chg_and_nset_update_cmd() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_PERFORM_AHO

DESCRIPTION
  This command is given by Overhead Messages Protocol when it receives the
  HDRIND_LMAC_AHO_REQUEST indication.  This puts the Overhead Messages Protocol
  in control of when the access handoff begins, so it can be sure to keep the
  overhead information marked as not being current while the access handoff is
  in progress.

  Upon receipt of this command LMAC checks if the ACMAC is active.  If it is,
  then the access attempt in progress must be stopped before the pilot is
  changed.  In this case LMAC calls hdramac_stop() to stop the access attempt
  and waits for the AccessChannelMAC.TxEnded indication before proceeding with
  the access handoff.  If the ACMAC is not active then this is an access entry
  handoff, so LMAC can immediately call hdrsrch_access_handoff(), passing in a
  callback function.

  hdrsrch_access_handoff() has an option for a pointer to a specific pilot
  record to be specified, but LMAC always sets this pointer to NULL to allow
  HDRSRCH to pick the strongest pilot, or even change its mind and stay on the
  same pilot. HDRSRCH must always call the callback function in response to a
  call to hdrsrch_access_handoff(), even if HDRSRCH does not change pilots.

DEPENDENCIES
  None

PARAMETERS
  lmac_cmd_ptr - Pointer to the LMAC_IDLE_CHAN_CHANGE command to be processed.
                 In this case it is not used since this command currently
                 does not have any parameters.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_process_perform_aho (hdrlmac_cmd_type * lmac_cmd_ptr)
{
  if (hdrrmac_is_active(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    ERR( "Access Handoff not allowed while connection is open", 0, 0, 0);
    return;
  }

  if (hdramac_is_in_attempt(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL))
  {
    if( lmac_cmd_ptr->name == HDRLMAC_PERFORM_AHO_CMD )
    {
      /* Queue HDRLMAC_FINISH_AHO_CMD if ACMAC is active when trying to process
       * access handoff. We re-use the command buffer here, for convenience.
       */
      lmac_cmd_ptr->name = HDRLMAC_FINISH_AHO_CMD;
    }
    else
    {
      ERR_FATAL("ACMAC still in attempt after TX_ENDED!",0,0,0);
    }

    hdrlmac_queue_pending_cmd( lmac_cmd_ptr );
    HDRTRACE_LMAC_WAIT_FOR_ACMAC_ABORT();
    hdramac_stop(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);
    hdrlmac.debug_aho_amac_stop_count++; /* Count how many times we stop AMAC */
  }
  else
  {
    if( ( lmac_cmd_ptr->name == HDRLMAC_FINISH_AHO_CMD )
      && ( hdrovhds_get_state() != HDROVHDS_ACC_HO_IN_PROG ) )
    {
      ERR( "Access Handoff ignored since hdrovhd is not in HDROVHDS_ACC_HO_IN_PROG state", 0, 0, 0);
      return;
    }

    /* Count how many access handoffs we perform */
    hdrlmac.debug_aho_count++;

    hdrsrch_access_handoff( NULL, hdrlmac_access_ho_rpt_cb );

    if( lmac_cmd_ptr->name == HDRLMAC_PERFORM_AHO_CMD )
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Performing AEHO" );
      if (hdrlmac.state == HDRLMAC_ACCESS_STATE)
      {
        hdrlmac_idle_mode( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL );
      }
      hdrind_give_ind(HDRIND_LMAC_ACCESS_ENTRY_HANDOFF, NULL);
    }
    else /* lmac_cmd_ptr->name == HDRLMAC_FINISH_AHO_CMD */
    {
      HDR_MSG_PROT( MSG_LEGACY_HIGH, "Performing AHO" );
    }
  }
} /* hdrlmac_process_perform_aho() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_TC_ASSIGN_CMD

DESCRIPTION
  The traffic channel assignment may include a different channel to be used or
  a disjoint set of active set pilots (i.e. hard handoff).  In this case the
  LMAC first stops the reverse link transmission to prevent anything queued by
  the upper layer (like a traffic channel complete message) from being sent out
  on the wrong pilot or frequency.  LMAC calls hdrrmac_stop_tx() and waits for
  the HDRIND_RTCMAC_TX_STOPPED indication before continuing.

  If the traffic channel assignment is simply to update the active set (soft
  handoff) then LMAC calls hdrsrch_aset_update() and passes in the new active
  set.

  If the traffic channel assignment is for initial assignment of a traffic
  channel or for a hard handoff, LMAC calls hdrsrch_chan_and_aset_assign()
  and gives the new channel and active set, and a callback function.

DEPENDENCIES
  None

PARAMETERS
  lmac_cmd_ptr - Pointer to the LMAC_IDLE_CHAN_CHANGE command to be processed.
                 In this case it is not used since this command currently
                 does not have any parameters.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrlmac_process_tc_assign_cmd( hdrlmac_cmd_type * lmac_cmd_ptr )
{
  hdrlmac_tc_assign_params_type *tcap_ptr =
    &lmac_cmd_ptr->params.tc_assign;
    /* Convenient pointer to tc assign command */

  hdrrup_tca_msg_type *tca_msg_ptr = &tcap_ptr->tca_msg;
    /* Convenient pointer to TCA msg */

  hdrrmac_channel_gains_struct_type channel_gains;
    /* Channel gains to rev0/A RMAC (will be removed in later phase) */

  sys_channel_type chan_list[HDRRUP_TCA_FWD_CH_MAX_SIZE];
    /* Storage for fwd channels in TCA*/

  uint8 chan_cnt;
    /* Number of fwd channels in TCA*/

  boolean changes_channel = FALSE; 
   /* If channel change is needed*/

  hdrlmac_tca_buf_type *tca_buf_ptr;
    /* Buffer for pending TCA message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Store RUP subtype */
  hdrlmac.rup_subtype = tcap_ptr->rup_subtype;

  /* Check if channel change is needed*/ 
  if (!tcap_ptr->initial_assignment)
  {
    /* If TCA is not for initial traffic channel assignment, we need to check if it is 
     * for HHO ( channel change is needed) or for aset update.
     */

    /* If srch/rmac is still processing previous TCA, queue current TCA */ 
    if (hdrlmac.l1_is_processing_tca)
    {
      /* Queue current TCA into pending_tca_q and process it 
       * after receiving rl_update_complete_cb() for the previous TCA 
       */
      rex_enter_crit_sect(&hdrlmac.crit_sect);

      /* Get a buffer from the special queue for pending tca message */
      if ( ( tca_buf_ptr = ( hdrlmac_tca_buf_type * )
       q_get( &hdrlmac.pending_tca_free_q ) ) == NULL )
      {
        ERR_FATAL( "No free buffers on hdrlmac.pending_tca_free_q", 0, 0, 0 );
      }

      HDR_MSG_PROT(MSG_LEGACY_MED, "Queuing a new TCA now...");

      tca_buf_ptr->rup_tca_params.initial_assignment = tcap_ptr->initial_assignment;
      tca_buf_ptr->rup_tca_params.link_id = tcap_ptr->link_id;
      tca_buf_ptr->rup_tca_params.tc_tune_away = tcap_ptr->tc_tune_away;
      tca_buf_ptr->rup_tca_params.cc_subactive_set_idx = tcap_ptr->cc_subactive_set_idx;
      tca_buf_ptr->rup_tca_params.sender = lmac_cmd_ptr->sender;
      tca_buf_ptr->rup_tca_params.rup_subtype = tcap_ptr->rup_subtype;
#ifdef FEATURE_MEMCPY_REMOVAL
      tca_buf_ptr->rup_tca_params.tca_msg = *tca_msg_ptr;
#else
      (void) memcpy (&tca_buf_ptr->rup_tca_params.tca_msg , tca_msg_ptr, 
                     sizeof(hdrrup_tca_msg_type));
#endif /* FEATURE_MEMCPY_REMOVAL */

      ( void ) q_link( tca_buf_ptr, &tca_buf_ptr->hdr.link ); /*lint !e613 no nulls */
      q_put( &hdrlmac.pending_tca_q, &tca_buf_ptr->hdr.link ); /*lint !e613 no nulls */

      HDR_MSG_PROT_1(MSG_LEGACY_MED, "Number of pending TCA %d", q_cnt(&hdrlmac.pending_tca_q));

      rex_leave_crit_sect(&hdrlmac.crit_sect);
      return;
    }

    /* Extract fwd cdma channels from TCA msg into chan_list
     * chan_list will be passed to srch later to determine if channel change is needed
     */
    hdrlmac_create_channel_list(&chan_cnt, chan_list, HDRRUP_TCA_FWD_CH_MAX_SIZE, tca_msg_ptr);
  }

  /* Store number of sub-active sets*/
  hdrlmac.num_sub_asets = tca_msg_ptr->num_sub_asets;
  ASSERT(hdrlmac.num_sub_asets <= HDRSRCH_MAX_NUM_SUBASETS);

  /* Create SRCH subactive set */
  changes_channel = hdrlmac_create_srch_subactive_set(tca_msg_ptr, 
                                    hdrlmac.num_sub_asets, 
                                    hdrlmac.srch_subactive_set);

  /* Create FMAC params*/
  hdrlmac_create_fmac_params(tca_msg_ptr, &hdrlmac.fmac_tca_params);

  /* Create RMAC params*/
  hdrlmac_create_rmac_params(tca_msg_ptr, &hdrlmac.rmac_params);

#ifdef FEATURE_HDR_IS890
  hdrlmac_create_carrier_map(tca_msg_ptr, hdrlmac.num_sub_asets, hdrlmac.carrier_map);
#endif /* FEATURE_HDR_IS890 */

  tcap_ptr->num_sub_asets = hdrlmac.num_sub_asets;
  tcap_ptr->hdr_rev = hdrscp_get_current_hdr_rev();

  if (tcap_ptr->initial_assignment)
  {
    HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
      HDRLMAC_TC_ASSIGN_CMD, lmac_cmd_ptr->sender, 0 );

    /* For the initial assignment, SRCH needs to enter the connected mode
     * and RTCMAC needs to be told to start transmitting once we are on the
     * correct active set.  Both are accomplished by sending the
     * hdrsrch_chan_and_aset_assign command to SRCH.  Note that the channel
     * may or may not change.  Initial assignment is the only instance for
     * which we send hdrsrch_chan_and_aset_assign when the channel has not
     * changed.
     */

    hdrlmac_change_state(HDRLMAC_CONNECTED_STATE);

    /* TODO: in later phase, RMAC will use single interface for both 
       rev0/A and B*/
    if (tcap_ptr->rup_subtype != HDRSCP_MC_RUP)
    {
      channel_gains.drc = hdrlmac.rmac_params.subaset_info[0].gains.drc;
      channel_gains.ack = hdrlmac.rmac_params.subaset_info[0].gains.ack;
      channel_gains.dsc = hdrlmac.rmac_params.dsc_gain_base;

      HDR_MSG_PROT_3(MSG_LEGACY_LOW, "drc: %d, ack %d dsc %d",
                       channel_gains.drc,
                       channel_gains.ack,
                       channel_gains.dsc);

      hdrrmac_set_channel_params( &channel_gains,
                                  hdrlmac.rmac_params.frame_offset );
    }

    hdrfmac_set_channel_params(&hdrlmac.fmac_tca_params);

    hdrsrch_chan_and_aset_assign_revb (
      tcap_ptr->hdr_rev,
      tcap_ptr->tc_tune_away,
      hdrlmac.num_sub_asets,
      hdrlmac.srch_subactive_set,
      tcap_ptr->cc_subactive_set_idx,
      hdrlmac.rmac_params.frame_offset,
      hdrlmac_tc_assign_complete_cb,
      hdrlmac_tc_suspend_cb,
      hdrlmac_schdgrp_changed_cb
    );
  }
  else if ( changes_channel )
  {
    /* This flag will be set to FALSE after rl_update_complete_cb is called*/
    hdrlmac.l1_is_processing_tca = TRUE;

#ifdef FEATURE_MEMCPY_REMOVAL
    (void) memscpy( lmac_cmd_ptr->params.tc_assign.srch_subactive_set, 
                    sizeof(lmac_cmd_ptr->params.tc_assign.srch_subactive_set), 
                    hdrlmac.srch_subactive_set,
                   ( sizeof(hdrsrch_subactive_set_type) * 
                     hdrlmac.num_sub_asets ) ); 
    
    lmac_cmd_ptr->params.tc_assign.rmac_params = hdrlmac.rmac_params;
    
    lmac_cmd_ptr->params.tc_assign.fmac_tca_params = hdrlmac.fmac_tca_params;
#else
    (void) memcpy( lmac_cmd_ptr->params.tc_assign.srch_subactive_set,
                   hdrlmac.srch_subactive_set,
                   ( sizeof(hdrsrch_subactive_set_type) * hdrlmac.num_sub_asets ) ); 
      
    (void) memcpy( &(lmac_cmd_ptr->params.tc_assign.rmac_params),
                   &hdrlmac.rmac_params,
                   sizeof( hdrlmac_rmac_params_type ) ); 

    (void) memcpy( &(lmac_cmd_ptr->params.tc_assign.fmac_tca_params),
                   &hdrlmac.fmac_tca_params,
                   sizeof( hdrfmac_tcam_params_type ) ); 
#endif /* FEATURE_MEMCPY_REMOVAL */

    hdrrmac_stop_tx(HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL);
    hdrlmac_queue_pending_cmd( lmac_cmd_ptr );
    HDRTRACE_LMAC_WAIT_FOR_RTCMAC_STOPTX();

    /* Performing a hard handoff.  Increment the logging counter. */
    hdrplog_inc_conn_state_stats_counter( HDRPLOG_HARD_HANDOFF_COUNT, 1 );
  }
  else
  {
    HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
      HDRLMAC_TC_ASSIGN_CMD, lmac_cmd_ptr->sender, 0 );

#ifdef FEATURE_HDR_REVC
    /* This is just an ASET update.  Pass it directly to searcher */
    hdrsrch_aset_update_revc( tcap_ptr->hdr_rev,
                              hdrlmac.num_sub_asets,
                              hdrlmac.srch_subactive_set,
                              tcap_ptr->cc_subactive_set_idx, 
                              hdrlmac.rmac_params.frame_offset,
                              hdrlmac_aset_update_complete_cb );
#else
    /* This is just an ASET update.  Pass it directly to searcher */
    hdrsrch_aset_update_revb( tcap_ptr->hdr_rev,
                              hdrlmac.num_sub_asets,
                              hdrlmac.srch_subactive_set,
                              tcap_ptr->cc_subactive_set_idx, 
                              hdrlmac.rmac_params.frame_offset );
#endif /* FEATURE_HDR_REVC */

    /* Set RMAC params*/
    if (tcap_ptr->rup_subtype == HDRSCP_MC_RUP)
    {
      /* We can't set l1_is_processing_tca for rev0/A because no 
       * tc_assign_complete_cb() will be called in aset upate case.
       * This flag will be set to FALSE after rl_update_complete_cb is called 
       * in revB case.
       */
      hdrlmac.l1_is_processing_tca = TRUE;

      hdrrmac_update_rl_carriers( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                                  hdrlmac.rmac_params.num_carriers,
                                  hdrlmac.rmac_params.rl_carrier,
                                  hdrlmac.rmac_params.frame_offset,
                                  hdrlmac.rmac_params.dsc_gain_base,
                                  hdrlmac.rmac_params.num_subasets,
                                  hdrlmac.rmac_params.subaset_info,
                                  hdrlmac_rl_update_complete_cb);
    }
    else
    {
      hdrrmac_set_frame_offset( hdrlmac.rmac_params.frame_offset );
    }

    /* Set FMAC params*/
    hdrfmac_set_channel_params(&hdrlmac.fmac_tca_params);
#if defined(FEATURE_HDR_DEVICE_ONLY_DTX)|| defined FEATURE_HDR_REVB_DTX_MODE\
 || defined FEATURE_HDR_TRAFFIC_DTX
    hdrrmac_check_dtx_after_tca( TRUE );
      /* it must be called after hdrfmac_set_channel_params(). Because we want to check
         drc_length inside this function. drc_length is passed to fmac through
         hdrfmac_set_channel_params(). Rmac get drc_length from fmac.
         Pass soft_ho==TURE since this is TCA for soft HO.
      */
#endif /*  FEATURE_HDR_DEVICE_ONLY_DTX || FEATURE_HDR_REVB_DTX_MODE 
|| FEATURE_HDR_TRAFFIC_DTX */
  }

} /* hdrlmac_process_tc_assign_cmd() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_DEACTIVATE_DONE_CB

DESCRIPTION
  This callback is called when searcher completes the processing of the 
  deactivate command.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_deactivate_done_cb ( void )
{
  
  HDR_MSG_PROT(MSG_LEGACY_HIGH,"L1 called deactivate done cb. Clearing the timer");
  
  /* Deactivate done received from SRCH. Clear timer */
  (void) rex_clr_timer(&hdrlmac.deactivate_done_timer);

   hdrind_give_ind( HDRIND_LMAC_DEACTIVATE_DONE, NULL );
//DA  HDRTRACE_LMAC_DEACTIVATE_DONE();
} /* hdrlmac_deactivate_done_cb() */

/* <EJECT> */
#ifdef FEATURE_HDR_TO_LTE
/*===========================================================================

FUNCTION HDRLMAC_IRAT_RESELECT_CB

DESCRIPTION
  Callback called from HDR searcher to report when the IRAT reselection 
  is to triggered.

DEPENDENCIES
  None

PARAMETERS
  earfcn_id - Frequency of the LTE cell to be selected
  cell_id - Cell id of the LTE cell to be selected

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_irat_reselect_cb 
(
  lte_earfcn_t earfcn_id,
  uint16 cell_id
)
{
  hdrind_lmac_lte_resel_info_type lte_resel_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  lte_resel_info.earfcn = earfcn_id; 
  lte_resel_info.cell_id = cell_id;

  hdrind_give_ind( HDRIND_LMAC_IRAT_RESELECT, (void *)&lte_resel_info );

} /* hdrlmac_irat_reselect_cb() */
/* <EJECT> */
#endif /* FEATURE_HDR_TO_LTE */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_NSET_UPDATE_CMD

DESCRIPTION
  This function processes a nset_update command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_nset_update_cmd
(
  hdrlmac_nset_update_params_type nset_update,
  hdrhai_protocol_name_enum_type  sender
)
{
  uint8 cnt; /* iterator*/ 
  sys_channel_type avoided_channel_list[HDRSRCH_AVOIDED_CHAN_MAX_SIZE];
  sys_active_prot_e_type hdr_rev; /* Current HDR revision */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  /* Initialize  local varariables*/ 
  memset(avoided_channel_list, 0, 
         (sizeof(sys_channel_type) * HDRSRCH_AVOIDED_CHAN_MAX_SIZE));

  hdr_rev = hdrscp_get_current_hdr_rev();

  /* Convert sys_channel_record_type to sys_channel_type*/
  for (cnt = 0; cnt < nset_update.num_avoided_channels; cnt++)
  {
     avoided_channel_list[cnt] = nset_update.avoided_list[cnt].channel;
  }

  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
                HDRLMAC_NEIGHBOR_LIST_UPDATE_CMD, sender, 0 );

  hdrsrch_nset_update( nset_update.num_pilots, 
                       nset_update.nset_pilots,
                       sizeof (hdrsrch_nset_pilot_type),
                       nset_update.update_type,
                       nset_update.num_channels, 
                       nset_update.channel_list,
#ifdef FEATURE_HDR_REMOVE_AVOIDED_NSET_CHANNELS
                       nset_update.num_avoided_channels, 
#else
                       0,
#endif
                       avoided_channel_list,
                       nset_update.num_sects,
                       nset_update.cached_sects,
                       nset_update.serving_pilot_gid,
                       hdr_rev
                     );

  HDR_MSG_PROT_1(MSG_LEGACY_HIGH,"NSET UPDATE, PilotGID %d", nset_update.serving_pilot_gid);
  
}/*hdrlmac_process_nset_update_cmd*/

#ifdef FEATURE_HDR_TO_LTE
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_LTE_NSET_UPDATE_CMD

DESCRIPTION
  This function processes a lte_nset_update command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_lte_nset_update_cmd
(
  hdrsrch_lte_nset_update_params_type   *lte_nset_update_params,
#if (LTE_BAND_NUM == 256)
sys_lte_band_mask_e_type lte_band_pref
#else
sys_band_mask_type lte_band_pref
#endif /* (LTE_BAND_NUM == 256) */
)
{
  hdrsrch_lte_nset_update_params_type pruned_lte_nset_params;
  uint8 num_eutra_freqs = 0;
  uint8 i = 0;
  boolean keep_earfcn = TRUE;
#ifdef FEATURE_HDR_MSGR_VARIANT
  boolean tdd_prune_required = lte_rrc_get_tdd_prune_required(hdrmc_get_hdr_sub_id());
#else
  boolean tdd_prune_required = lte_rrc_get_tdd_prune_required();
#endif /* FEATURE_HDR_MSGR_VARIANT */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT( MSG_LEGACY_HIGH, "LTE nset updated with searcher");

  lte_nset_update_params->reselect_cb = hdrlmac_irat_reselect_cb;

  /* Copy the entire Other RAT info first */
#ifdef FEATURE_MEMCPY_REMOVAL
  pruned_lte_nset_params = *lte_nset_update_params;
#else
  memcpy ( &pruned_lte_nset_params, lte_nset_update_params, 
           sizeof ( hdrsrch_lte_nset_update_params_type )); 
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Loop through and remove the unsupported LTE freq and LTE TDD freq */
  for ( i = 0; i < lte_nset_update_params->num_eutra_freqs; i++)
  {
    /* Reset flag whether the given EARFCN need to pruned or not */
    keep_earfcn = TRUE;

    /* Remove any unsupported frequencies from ORNL message */
    if ( lte_rrc_earfcn_is_supported ( 
#ifdef FEATURE_HDR_MSGR_VARIANT
                       hdrmc_get_hdr_sub_id(),
#endif /* FEATURE_HDR_MSGR_VARIANT */
                       lte_nset_update_params->earfcn_params_arr[i].earfcn,
                       lte_band_pref ))
    {
      /* Remove LTE TDD frequency if required */
      if ( tdd_prune_required && lte_rrc_cell_is_tdd( 
                 lte_nset_update_params->earfcn_params_arr[i].earfcn ))
      {
        keep_earfcn = FALSE;
        HDR_MSG_PROT_1 (MSG_LEGACY_ERROR, 
                        "Removed LTE TDD freq = %d from ORNL message",
                        lte_nset_update_params->earfcn_params_arr[i].earfcn);
      }
      else
      {
        keep_earfcn = TRUE;
      }
    }
    else
    {
      keep_earfcn = FALSE;
      HDR_MSG_PROT_1 (MSG_LEGACY_ERROR, 
                      "Removed unsupported LTE freq = %d from ORNL message",
                      lte_nset_update_params->earfcn_params_arr[i].earfcn);
    }

    if (keep_earfcn)
    {
#ifdef FEATURE_MEMCPY_REMOVAL
      pruned_lte_nset_params.earfcn_params_arr[num_eutra_freqs] = 
                         lte_nset_update_params->earfcn_params_arr[i];
#else
      memcpy ( &pruned_lte_nset_params.earfcn_params_arr[num_eutra_freqs],
               &lte_nset_update_params->earfcn_params_arr[i],
               sizeof (hdrsrch_lte_freq_params_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
      num_eutra_freqs++;
    }
  }

  pruned_lte_nset_params.num_eutra_freqs = num_eutra_freqs;

  hdrsrch_lte_nset_update( &pruned_lte_nset_params );

  
}/*hdrlmac_process_lte_nset_update_cmd*/
#endif /* FEATURE_HDR_TO_LTE*/

#ifdef FEATURE_LTE_TO_HDR
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_TIME_TRANSFER_CMD

DESCRIPTION
  This function processes a time_transfer command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_time_transfer_cmd
(
  hdrsrch_tran_sys_time_params_type *tran_sys_time_req
)
{

  hdrsrch_tran_sys_time( tran_sys_time_req, hdrlmac_time_transfer_done_cb );

}/*hdrlmac_process_time_transfer_cmd*/
#endif /* FEATURE_LTE_TO_HDR */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_SET_TC_MODE_CMD

DESCRIPTION
  This function processes a set_tc_mode command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_set_tc_mode_cmd
(
   hdrsrch_tc_mode_enum_type tc_mode
)
{

  hdrsrch_set_tc_mode( tc_mode );
     /*Pass the request to hdrsrch*/

}/*hdrlmac_process_set_tc_mode_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_ADVICE_UNLOCK_RF_CMD

DESCRIPTION
  This function processes a advice_unlock_rf command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_advice_unlock_rf_cmd
(
   void
)
{
  
  HDR_MSG_PROT( MSG_LEGACY_HIGH,  
      "Processing advise unlock RF cmd but not forwarding to HDRSRCH" );

}/*hdrlmac_process_advice_unlock_rf_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_TC_ESTABLISHED_CMD

DESCRIPTION
  This function processes a tc_established command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_tc_established_cmd
(
   void
)
{
  /* Let physical layer know that traffic chan has been established */
  hdrsrch_tc_ack_received();
//  hdrsrch_tc_established();

}/*hdrlmac_process_tc_established_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_PILOT_REPORT_RESET_CMD

DESCRIPTION
  This function processes a pilot_report_reset command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_pilot_report_reset_cmd
(
   hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_PILOT_REPORT_RESET_CMD, sender, 0 );
  hdrsrch_pilot_report_reset();

}/*hdrlmac_process_pilot_report_reset_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_ACCESS_MODE_CMD

DESCRIPTION
  This function processes an access_mode command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_access_mode_cmd
(
   hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_ACCESS_MODE_CMD, sender, 0 );

  hdrlmac_change_state(HDRLMAC_ACCESS_STATE);

  hdrsrch_access_mode( hdrlmac_suspend_ho_cb,
      (hdrlmac.aho_count < HDRLMAC_MAX_AHO_COUNT) 
        ? hdrlmac_request_aho_cb 
        : NULL,
      HDRLMAC_CALC_ACCESS_HO_THRESHOLD_HALF_DB(hdrlmac.aho_count) );

}/*hdrlmac_process_access_mode_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_SLEEP_CMD

DESCRIPTION
  This function processes a sleep command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_sleep_cmd
(
   hdrsrch_sleep_params_type *sleep,
   hdrhai_protocol_name_enum_type sender
)
{
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SLEEP_CMD, sender, 0 );
  HDRTRACE_LMAC_SLEEP();

  /* LMAC does not track HDRSRCH transitions in and out of sleep, but 
   * LMAC state could be access at this point, so change it to idle
   */
  hdrlmac_change_state(HDRLMAC_IDLE_STATE);
  hdrsrch_sleep(sleep->hdr_slot_cycle,
                sleep->hdr_neg_slot_cycle,
                sleep->qpch_enabled,
                sleep->wakeup_cb,
                sleep->cc_start_slot_valid,
                sleep->cc_start_slot
#ifdef FEATURE_HDR_TO_LTE
                ,sleep->ornl_cycle
#endif /* FEATURE_HDR_TO_LTE */
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
                ,sleep->session_is_open
                ,sleep->ovhd_is_updated
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
                ); 
  
} /*hdrlmac_process_sleep_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_ACMAC_READY_CMD

DESCRIPTION
  This function processes an acmac_ready command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_acmac_ready_cmd
(
   void
)
{
  if( hdrlmac.state == HDRLMAC_ACCESS_STATE )
  {
    hdrsrch_acmac_ready( hdrlmac_ta_time_cb, hdrlmac_ta_stop_access_cb );
  }
  else
  {
    HDR_MSG_PROT_1( MSG_LEGACY_HIGH,  
      "LMAC no longer in ACCESS, skipping search notification. LMAC state: %d",
      hdrlmac.state );
  }

}/*hdrlmac_process_acmac_ready_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_ACMAC_DIABLE_TA_CMD

DESCRIPTION
  This function processes an acmac_disable_ta command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_acmac_disable_ta_cmd
(
   uint16 no_ta_duration
)
{
  HDR_MSG_PROT_1( MSG_LEGACY_FATAL, 
                  "hdrlmac_acmac_disable_ta() lmac state %d",
                  hdrlmac.state );
  
  hdrsrch_acmac_disable_ta( no_ta_duration );
}/*hdrlmac_process_acmac_disable_ta_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_TA_TIME_OK_CMD

DESCRIPTION
  This function processes a ta_time_ok command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_ta_time_ok_cmd
(
   boolean ta_time_ok
)
{
  HDR_MSG_PROT_1( MSG_LEGACY_MED, 
                "hdrlmac_ta_time_ok() lmac state %d",
                hdrlmac.state );
  
  hdrsrch_acmac_ta_time_ok( ta_time_ok );

}/*hdrlmac_process_ta_time_ok_cmd*/

/* <EJECT> */
#ifdef FEATURE_HDR_BCMCS
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_UPDATE_BC_INFO_CMD

DESCRIPTION
  This function processes an update_bc_info command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_update_bc_info_cmd
(
   boolean bcmcs_status 
)
{
  HDRTRACE_LMAC_UPDATE_BC_STATUS(bcmcs_status);
  HDR_MSG_PROT_1(MSG_LEGACY_MED,"Update BC Status (%d)",bcmcs_status);
  hdrsrch_update_bc_info( bcmcs_status ? BCMCS_ENABLED : BCMCS_DISABLED ); 

}/*hdrlmac_process_update_bc_info_cmd*/
#endif /* FEATURE_HDR_BCMCS */
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_IDLE_MODE_CMD

DESCRIPTION
  This function processes an idle_mode command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_idle_mode_cmd
(
   hdrhai_protocol_name_enum_type sender
)
{
  boolean is_tca_processing = FALSE;
  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_IDLE_MODE_CMD, sender, 0 );
  hdrlmac_change_state(HDRLMAC_IDLE_STATE);
  if((hdrrups_get_state() == HDRRUPS_CONNECTION_SETUP)||
     (hdrrups_get_state() == HDRRUPS_SYNCHRONIZE_CONNECTION_SETUP))/* Processing the TCA */
  {
    is_tca_processing = TRUE;
        HDR_MSG_PROT_2(MSG_LEGACY_MED,"IDLE cmd: RUP state %d is_tca_processing %d",
                                       hdrrups_get_state(),is_tca_processing);
  }
  hdrsrch_idle_mode( hdrlmac_idle_ho_cb, hdrlmac_idle_suspend_cb, is_tca_processing );
  hdrplog_access_mode(FALSE);

}/*hdrlmac_process_idle_mode_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_SYNCHRONIZE_CMD

DESCRIPTION
  This function processes a synchronize command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_synchronize_cmd
(
   hdrlmac_synchronize_params_type      *sync_params,
   hdrhai_protocol_name_enum_type        sender
)
{
/* Notify CC MAC of the new link*/
  hdrcmac_update_link( sync_params->link_id );

  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_SYSTEM_TIME_SYNC_CMD, sender, 0 );
  hdrlmac_change_state(HDRLMAC_SYNC_STATE);
  hdrsrch_synchronize( sync_params->sync_time,
                       sync_params->pilot_pn,
                       sync_params->half_slot, 
                       hdrlmac_timing_info_return_cb
                     );

}/*hdrlmac_process_synchornize_cmd*/

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_ACQUIRE_CMD

DESCRIPTION
  This function processes an acquire command given to the
  the hdrlmac_process_cmd protocol

DEPENDENCIES
  

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_acquire_cmd
(
    hdrsrch_acquire_params_type      *acq_params,
    hdrhai_protocol_name_enum_type    sender
)
{

  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    HDRLMAC_ACQUIRE_CMD, sender, 0 );
  HDRTRACE_LMAC_ACQUIRE(acq_params->channel.chan_num);
  HDR_MSG_PROT_3( MSG_LEGACY_HIGH, "Acquiring on c=%d b=%d mode=%d",
                  acq_params->channel.chan_num,
                  acq_params->channel.band, 
                  acq_params->mode );
  hdrsrch_deactivate(NULL);
  hdrlmac_change_state(HDRLMAC_ACQ_STATE);

   /*Notify CC MAC that there is no current link*/
  hdrcmac_update_link( HDRCMAC_INVALID_LINK_ID );
  
  hdrsrch_acquire( acq_params );

}/*hdrlmac_process_acquire_cmd*/

/* <EJECT> */
#ifdef FEATURE_IRAT_PILOT_MEAS
/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_IRAT_PILOT_MEAS_RSP_CB

DESCRIPTION
  Callback called by searcher to indicate measurement request is done.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void hdrlmac_irat_pilot_meas_rsp_cb(void)
{
  hdrind_give_ind( HDRIND_LMAC_IRAT_PLT_MEAS_DONE_IND, NULL);
} /* hdrlmac_irat_pilot_meas_rsp_cb() */
#endif /* FEATURE_IRAT_PILOT_MEAS */

#ifdef FEATURE_HDR_BOLT_MODEM
#ifndef FEATURE_HDR_UNIFIED_TX
/*===========================================================================

FUNCTION HDRLMAC_TRIGGER_SYSTEM_LOST

DESCRIPTION
  Triggers a system lost due to failure at lower layers (usually FW). 
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrlmac_trigger_system_lost( void )
{

  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_TRIGGER_SYSTEM_LOST_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;

 /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));
}

/*===========================================================================

FUNCTION HDRLMAC_PROCESS_TRIGGER_SYSTEM_LOST

DESCRIPTION
  Processes the trigger system lost command in Tx context. 
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrlmac_process_trigger_system_lost( void )
{

  HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                "Triggering system lost" );

  /* Trigger the system lost via search */
  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_FW_RESET_REQ );
}
#endif /* FEATURE_HDR_UNIFIED_TX */
#endif /* FEATURE_HDR_BOLT_MODEM */

/*===========================================================================

FUNCTION HDRLMAC_PROCESS_CMD

DESCRIPTION
  This function processes commands given to LMAC.

DEPENDENCIES
  None

PARAMETERS
  lmac_cmd_ptr - Pointer to the command for LMAC to process

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_cmd (hdrlmac_cmd_type * lmac_cmd_ptr)
{
  hdrlmac_cmd_buf_type *buf_ptr; /* buffer for pending command, if any */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRTRACE_CMD( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
    lmac_cmd_ptr->name, lmac_cmd_ptr->sender, hdrlmac.state );
  switch (lmac_cmd_ptr->name)
    {
    case HDRLMAC_ACTIVATE_CMD: /* Starts HDR mode operation */
      hdrsrch_activate();
      break;

    case HDRLMAC_DEACTIVATE_CMD: /* Stops HDR mode operation */
      while ((buf_ptr = (hdrlmac_cmd_buf_type *)
                q_get( &hdrlmac.pending_cmd_q )) != NULL)
      {
        ERR( "Pending message left in LMAC!",0,0,0);
        hdrbuf_done( &buf_ptr->hdr );
      }
#ifdef FEATURE_HDR_REVC
      hdrsrch_enable_nlb(FALSE);
#endif /* FEATURE_HDR_REVC */
      hdrlmac_change_state(HDRLMAC_INACTIVE_STATE);
      
#ifdef FEATURE_HDR_MODEM_API
      /* Reset Modem API values since we are going inactive */
      hdrlmac_reset_modem_api_data();
  
      /* Clear timeout timer */
      (void) rex_clr_timer(&hdrlmac.modem_api_timeout_timer);
#endif

    if (lmac_cmd_ptr->sender == HDRHAI_AIR_LINK_MANAGEMENT_PROTOCOL)
    {
        /* hdrlmac_deactivate_done_cb is only needed when MMoC (via ALMP)
         * is deactivating HDR.
         */
        hdrsrch_deactivate(hdrlmac_deactivate_done_cb);
      if(hdralmp_hdr_to_lte_redir_is_in_progress() == TRUE)
      {
	    /* Start timer for 100ms only for D2L redir scenario */
            (void) rex_set_timer(&hdrlmac.deactivate_done_timer, 
                                 HDRLMAC_DEACTIVATE_DONE_TIMEOUT);
             HDR_MSG_PROT_1( MSG_LEGACY_MED, 
                             "DEACTIVATE DONE timer started for %d ms",
                             HDRLMAC_DEACTIVATE_DONE_TIMEOUT);
      }
    }
      else
      {
        hdrsrch_deactivate(NULL);
      }
      break;

    case HDRLMAC_IDLE_CHAN_CHANGE_CMD: /* Change channels while idle */
      hdrlmac_process_idle_chan_change_cmd( lmac_cmd_ptr );
      break;

    case HDRLMAC_PERFORM_AHO_CMD: /* Perform Access Handoff */
      hdrlmac_process_perform_aho( lmac_cmd_ptr );
      hdrlmac.aho_count++; /* Count how many access handoffs we perform */
      HDR_MSG_PROT_2( MSG_LEGACY_MED, "LMAC aho_count %d->%d",
                      hdrlmac.aho_count-1, hdrlmac.aho_count );
      break;

    case HDRLMAC_FINISH_AHO_CMD:                   /* Perform access handoff */
      /* This command is queued when ACMAC is active while trying to process
       * AHO request, then processed now, when ACMAC stops probing 
       */
      hdrlmac_process_perform_aho( lmac_cmd_ptr ); 
      break;                   

    case HDRLMAC_TC_ASSIGN_CMD: /* Traffic channel assignment or ASET update */
      hdrlmac_process_tc_assign_cmd( lmac_cmd_ptr );
      break;

#ifdef FEATURE_LTE_TO_HDR
    case HDRLMAC_FAST_ACQUISITION:
      hdrsrch_fast_acq( &lmac_cmd_ptr->params.fast_acq_cmd );
      break;
#endif /* FEATURE_LTE_TO_HDR */

    case HDRLMAC_ACQUIRE_CMD:
      hdrlmac_process_acquire_cmd(&lmac_cmd_ptr->params.acquire, lmac_cmd_ptr->sender );
      break;

    case HDRLMAC_SYSTEM_TIME_SYNC_CMD:
      hdrlmac_process_synchronize_cmd(&lmac_cmd_ptr->params.synchronize, lmac_cmd_ptr->sender);
      break;

    case HDRLMAC_IDLE_MODE_CMD:
      hdrlmac_process_idle_mode_cmd(lmac_cmd_ptr->sender);
      break;

#ifdef FEATURE_HDR_BCMCS
    case HDRLMAC_UPDATE_BC_INFO_CMD:
      hdrlmac_process_update_bc_info_cmd(lmac_cmd_ptr->params.update_bc_info_bcmcs_status);
      break;
#endif /* FEATURE_HDR_BCMCS */

    case HDRLMAC_ACMAC_TA_TIME_OK_CMD:
      hdrlmac_process_ta_time_ok_cmd(lmac_cmd_ptr->params.ta_time_ok);
      break;

    case HDRLMAC_ACMAC_DISABLE_TA_CMD:
      hdrlmac_process_acmac_disable_ta_cmd(lmac_cmd_ptr->params.no_ta_duration);
      break;

    case HDRLMAC_ACMAC_READY_CMD:
      hdrlmac_process_acmac_ready_cmd();
      break;

    case HDRLMAC_SLEEP_CMD:
      hdrlmac_process_sleep_cmd(&lmac_cmd_ptr->params.sleep, lmac_cmd_ptr->sender);
      break;

    case HDRLMAC_ACCESS_MODE_CMD:
      hdrlmac_process_access_mode_cmd(lmac_cmd_ptr->sender);
      break;

    case HDRLMAC_PILOT_REPORT_RESET_CMD:
      hdrlmac_process_pilot_report_reset_cmd(lmac_cmd_ptr->sender);
      break;

    case HDRLMAC_TC_ACK_RECEIVED_CMD:
      hdrlmac_process_tc_established_cmd();
      break;

    case HDRLMAC_ADVISE_UNLOCK_RF_CMD:
      hdrlmac_process_advice_unlock_rf_cmd();
      break;

    case HDRLMAC_TC_MODE_CMD:
      hdrlmac_process_set_tc_mode_cmd(lmac_cmd_ptr->params.tc_mode.mode);
      break;

#ifdef FEATURE_LTE_TO_HDR
    case HDRLMAC_TRAN_SYS_TIME_REQ:
      hdrlmac_process_time_transfer_cmd(&lmac_cmd_ptr->params.tran_sys_time_req);
      break;
#endif /* FEATURE_LTE_TO_HDR */

#ifdef FEATURE_IRAT_PILOT_MEAS
    case HDRLMAC_PILOT_MEAS_CMD:
      hdrsrch_pilot_meas_request( &lmac_cmd_ptr->params.pilot_meas_req, hdrlmac_irat_pilot_meas_rsp_cb);
      break;
#endif  /*FEATURE_IRAT_PILOT_MEAS */

#ifdef FEATURE_HDR_TO_LTE
    case HDRLMAC_LTE_NSET_UPDATE_CMD:
      hdrlmac_process_lte_nset_update_cmd(&lmac_cmd_ptr->params.lte_nset_update.lte_nset_update_params, lmac_cmd_ptr->params.lte_nset_update.lte_band_pref);
      break;
#endif /*FEATURE_HDR_TO_LTE*/

    case HDRLMAC_NEIGHBOR_LIST_UPDATE_CMD:
      hdrlmac_process_nset_update_cmd(lmac_cmd_ptr->params.nset_update,
                                      lmac_cmd_ptr->sender
                                      );
      break;
      
#ifdef FEATURE_HDR_MODEM_API
    case HDRLMAC_MODEM_API_TIMEOUT_CMD:
      hdrlmac_reset_modem_api_data();
      break;
#endif /* FEATURE_HDR_MODEM_API */

#ifdef FEATURE_HDR_BOLT_MODEM
#ifndef FEATURE_HDR_UNIFIED_TX
    case HDRLMAC_TRIGGER_SYSTEM_LOST_CMD:
      hdrlmac_process_trigger_system_lost();
      break;
#endif /* FEATURE_HDR_UNIFIED_TX */
#endif /* FEATURE_HDR_BOLT_MODEM */

    case HDRLMAC_IDLE_CHAN_CHG_AND_NSET_UPDATE_CMD:
      hdrlmac_process_idle_chan_chg_and_nset_update_cmd(lmac_cmd_ptr);
      break;

    default:
      ERR_FATAL( "Unknown command", 0, 0, 0 );
      break;
    }
} /* hdrlmac_process_cmd() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_PROCESS_IND

DESCRIPTION
  This function processes indications given to LMAC.

DEPENDENCIES
  None

PARAMETERS
  ind_name     - Name of the indication for LMAC to process
  ind_data_ptr - Any data associated with the indication

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_process_ind
(
  hdrind_ind_name_enum_type ind_name,
  hdrind_ind_data_union_type * ind_data_ptr
)
{

  hdrlmac_cmd_buf_type *buf_ptr; /* buffer for pending command, if any */
  hdrlmac_tc_assign_params_type *tca_param_ptr; 
    /* convenient pointer to TC_ASSIGNED_CMD parameters */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRTRACE_IND( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL, ind_name, hdrlmac.state );
  switch (ind_name)
  {

#ifdef FEATURE_HDR_HYBRID_ACCESS

    case HDRIND_ACMAC_READY:
      
      hdrlmac_acmac_ready();
      break;

    case HDRIND_ACMAC_TUNE_AWAY_TIME_OK:
      
      hdrlmac_ta_time_ok( *(boolean *)ind_data_ptr );
      break;

#endif /* FEATURE_HDR_HYBRID_ACCESS */

    case HDRIND_ACMAC_TRANSMISSION_SUCCESSFUL:
      hdrlmac.aho_count = 0; /* If we get a probe through, clear the count */
      break;

    case HDRIND_ACMAC_TX_ENDED:
      if (q_cnt( &hdrlmac.pending_cmd_q) != 0)
      {
        if (q_cnt( &hdrlmac.pending_cmd_q) != 1)
        {
          ERR_FATAL( "LMAC Pending queue has more than 1 cmd pending", 0, 0, 0 );
        }
        /* Currently only one command should ever be pending, but if
         * there is reason to queue more we should be OK here.
         */

        while ((buf_ptr = (hdrlmac_cmd_buf_type *)
                  q_get( &hdrlmac.pending_cmd_q )) != NULL)
        {
          ASSERT((buf_ptr->cmd.name == HDRLMAC_IDLE_CHAN_CHANGE_CMD) ||
                 (buf_ptr->cmd.name == HDRLMAC_IDLE_CHAN_CHG_AND_NSET_UPDATE_CMD) ||
                 (buf_ptr->cmd.name == HDRLMAC_FINISH_AHO_CMD));
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, "Process pending LMAC command %d",
                          buf_ptr->cmd.name );
          hdrlmac_process_cmd( &buf_ptr->cmd );
          hdrbuf_done( &buf_ptr->hdr );
        }
        /* The important thing is that searcher call the callback for
         * access_handoff or idle_channel_change before performing an
         * idle handoff.  This is guaranteed as long as access_mode_ended
         * is sent after the others.
         */
      }

      if (hdrlmac.state == HDRLMAC_ACCESS_STATE)
      {
        hdrlmac_idle_mode( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL );
      }
      break;

    case HDRIND_RTCMAC_DRC_TX_STOPPED:
      hdrsrch_enable_pilot_reports( NULL );
      break;

    case HDRIND_RTCMAC_DRC_TX_RESTARTED:
      hdrsrch_enable_pilot_reports( hdrlmac_pilot_rpt_cb );
      break;

    case HDRIND_RTCMAC_LINK_ACQUIRED:

      hdrsrch_enable_pilot_reports( hdrlmac_pilot_rpt_cb );
      break;

    case HDRIND_RTCMAC_TX_STOPPED:
	case HDRIND_RTCMAC_SHUTDOWN_COMPLETE:

      while ((buf_ptr = (hdrlmac_cmd_buf_type *)
                q_get( &hdrlmac.pending_cmd_q )) != NULL)
      {
        /* Only the TC_ASSIGN command should be queued at this time */
        ASSERT( buf_ptr->cmd.name == HDRLMAC_TC_ASSIGN_CMD );

        /* Since we are in TX context, don't queue cmd again, process now */
        tca_param_ptr = &buf_ptr->cmd.params.tc_assign;

        hdrsrch_chan_and_aset_assign_revb(
          tca_param_ptr->hdr_rev,
          tca_param_ptr->tc_tune_away,
          tca_param_ptr->num_sub_asets,
          tca_param_ptr->srch_subactive_set,
          tca_param_ptr->cc_subactive_set_idx,
          tca_param_ptr->rmac_params.frame_offset,
          hdrlmac_tc_assign_complete_cb,
          hdrlmac_tc_suspend_cb,
          hdrlmac_schdgrp_changed_cb );

        /* TODO, RMAC will use single interface later */
        if ( !( tca_param_ptr->rup_subtype == HDRSCP_MC_RUP
#ifdef FEATURE_LTE_TO_HDR_OH
                || tca_param_ptr->rup_subtype == HDRSCP_IRAT_MC_RUP
#endif /* FEATURE_LTE_TO_HDR_OH */
              )
           )
        {
          hdrrmac_set_frame_offset( tca_param_ptr->rmac_params.frame_offset );
        }
        hdrfmac_set_channel_params( &tca_param_ptr->fmac_tca_params );

        hdrbuf_done( &buf_ptr->hdr );
      }
      break;

    case HDRIND_LMAC_CC_MISSED:
      /* If in idle, enable handoff after sync CC */
      if ((hdrlmac.state == HDRLMAC_IDLE_STATE) || 
          (hdrlmac.state == HDRLMAC_ACCESS_STATE))
      {
        /* Enable handoff now */
        hdrsrch_reenable_idle_handoffs();
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV

        /* Let Srch know about the missed CC*/
        hdrsrch_record_missed_cc();
        
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
      }
      
#ifdef FEATURE_HDR_MODEM_API
      /* Update modem API values once per wakeup. 
         This function is run in Tx task. */
      if  ((hdrlmac.awake_flag) && (!hdrlmac.connection_open))
      {
        hdrlmac_update_modem_api_data();
        hdrlmac.awake_flag = FALSE;
      }
#endif /* FEATURE_HDR_MODEM_API */

      break;

    case HDRIND_CCMAC_SCC_PKT_RXED:
      /* Error catching */
      ASSERT(ind_data_ptr != NULL);

      /* Check for last packet */
      if (ind_data_ptr->intra_scc_info.last_pkt)
      {
        /* Still print old IND for now per AST request */
        HDRTRACE_IND( HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL, 
                      HDRIND_CCMAC_END_OF_SYNC_CAPSULE, 0);

        /* If in idle, enable handoff after sync CC */
        if ((hdrlmac.state == HDRLMAC_IDLE_STATE) || 
            (hdrlmac.state == HDRLMAC_ACCESS_STATE))
        {
          /* Enable handoff now */
          hdrsrch_reenable_idle_handoffs();
        }
      }  

#ifdef FEATURE_HDR_MODEM_API
      /* Update modem API values once per wakeup. 
         This function is run in Tx task. */
      if  ((hdrlmac.awake_flag) && (!hdrlmac.connection_open))
      {
        hdrlmac_update_modem_api_data();
        hdrlmac.awake_flag = FALSE;
      }
      break;

    case HDRIND_IDLE_CONNECTION_OPENED:
      /* Connection has opened */
      hdrlmac.connection_open = TRUE;
      break;

    case HDRIND_CONNECTED_CONNECTION_CLOSED:
      /* Connection is closed */
      hdrlmac.connection_open = FALSE;
#endif /* FEATURE_HDR_MODEM_API */
      break;

    default:
      ERR_FATAL( "Unknown indication", 0, 0, 0 );
      break;
  }
} /* hdrlmac_process_ind() */ /*lint !e715 ind_data_ptr not used */

/*=========================================================================

FUNCTION     : HDRLMAC_GET_RSSI

DESCRIPTION  : This function returns the RSSI estimate for HDR to be used
               by the User Interface to indicate quality of HDR service.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : Estimated HDR RSSI.

SIDE EFFECTS : None

=========================================================================*/

byte hdrlmac_get_rssi
( 
  void 
)
{
    return hdrsrch_get_hdr_rssi();
} /* hdrlmac_get_rssi() */

/*=========================================================================

FUNCTION     : HDRLMAC_GET_ECIO_AND_IO

DESCRIPTION
  Returns HDR Ec/Io and Io to the caller. It returns the Io estimate made by 
  rx AGC loops in the DSP and the Ec/Io estimate found by the searcher.

  Ec/Io range: from 5 ( -31.5 dB ) to 512 ( 0 dB )
  Io    range: from -106 dBm to -21 dBm 
    
DEPENDENCIES
  None

INPUTS
  Pointers to the locations where EcIo and Io will be stored

RETURN VALUE
  EcIo and Io are returned through the passed pointers.

SIDE EFFECTS
  None

=========================================================================*/

void hdrlmac_get_ecio_and_io( int16* ecio, int* io )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
  hdrsrch_get_ecio_and_io( ecio, io );

} /* hdrlmac_get_ecio_and_io() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_DATA_INDICATOR

DESCRIPTION  : This function returns the data level indicator. The data rate
               that the AT can receive is divided into 9 levels, 0 to 8. Level
               8 indicates highest data rate and level 0 indicates almost no
               data rate.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : Data activity indicator between 0 to 8.

SIDE EFFECTS : None

=========================================================================*/

byte hdrlmac_get_data_indicator( void )
{
  return hdrsrch_get_data_indicator();
}


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_ASP_PN_MAPPING

DESCRIPTION  : This function returns the PN offsets and ASP indices of all 
               the active set pilots, in an array passed by the caller.

DEPENDENCIES : None

INPUT        : A pointer to an array of type hdrsrch_asp_to_pn_info_type.

RETURN VALUE : Number of ASET pilots, from 0 to 6.
               PN and ASP info will be updated in the array passed by caller.

SIDE EFFECTS : None

=========================================================================*/

uint8 hdrlmac_get_asp_pn_mapping( hdrsrch_asp_to_pn_info_type * aset_pn)
{
  return hdrsrch_get_asp_pn_mapping( aset_pn );
}


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_PN_ECIO

DESCRIPTION  : This function returns the PN offsets and EcIo of all the 
               active set pilots, in an array passed by the caller. The
               range of EcIo returned is from 0 to 63, in -0.5db unit.

DEPENDENCIES : None

INPUT        : A pointer to an array of type hdrsrch_pn_ecio_info_type.

RETURN VALUE : Number of ASET pilots, from 0 to 6.
               PN and EcIo info will be updated in the array passed by caller.
               
SIDE EFFECTS : None

=========================================================================*/
 
uint8 hdrlmac_get_pn_ecio( hdrsrch_pn_ecio_info_type* pn_ecio)
{
  return hdrsrch_get_pn_ecio( pn_ecio );
}


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_ADVICE_UNLOCK_RF

DESCRIPTION  : This function advices SEARCH that 1x needs the RF lock

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_advice_unlock_rf( void )
{
   /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
  
   /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_ADVISE_UNLOCK_RF_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;

   /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_COLOR_CODE_UPDATE

DESCRIPTION  : This function informs the searcher of the color code for the
               current link

DEPENDENCIES : None

PARAMETERS   :
  link_info  - link info for the current sector
  color_code - color code of the current subnet

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_color_code_update
(
  sys_link_type link_info,
  uint8 color_code
)
{
  hdrsrch_color_code_update(link_info, color_code);
} /* hdrlmac_color_code_update*/

#ifdef FEATURE_HDR_TO_LTE
/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_UPDATE_SECTOR

DESCRIPTION  : This function informs the searcher of current sector ID

DEPENDENCIES : None

PARAMETERS   :
  sector_id  - current sector ID

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrlmac_update_sector
(
  hdraddr_type sector_id
)
{
  hdrsrch_update_sector(sector_id);
} /* hdrlmac_update_sector() */
#endif /* FEATURE_HDR_TO_LTE */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_SET_TC_MODE

DESCRIPTION  : This function changes the HDR traffic mode during the
               middle of a traffic connection. Current modes
               supported are:

               HDRSRCH_DEFAULT_TC_MODE   - Standard HDR traffic connection. 

               HDRSRCH_S1XHDR_HP_TC_MODE - High Priority Traffic Mode where
                                           HDR preempts 1x paging onto 
                                           chain 1 and preempts acquisition.

DEPENDENCIES : None

PARAMETERS   :

  tc_mode    - Traffic mode that HDR should transition to.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_set_tc_mode
( 
  hdrsrch_tc_mode_enum_type tc_mode
    /* Traffic mode that HDR should transition to. */
)
{
    /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  hdrsrch_tc_mode_enum_type current_tc_mode = hdrlmac_get_tc_mode();
  
   if(current_tc_mode == HDRSRCH_DISABLE_TA_TC_MODE)
   {
	   /* If current TC mode is DISABLE TA, then allow only enable setting ENABLE TA.
		  This to handle the case where after connection exit, RUP sets to default TC_MODE. 
		  However, now it should NOT be set to default TC mode, until CM calls it 
		  again with low priority */
	   if(tc_mode == HDRSRCH_ENABLE_TA_TC_MODE)
	   {
		 tc_mode = HDRSRCH_DEFAULT_TC_MODE;
		 HDR_MSG_PROT( MSG_LEGACY_MED, "Setting Default TC mode");
	   }
	   else
	   {
		 HDR_MSG_PROT_2 ( MSG_LEGACY_MED, "Ignored !! Setting TC mode to %d, as the current TC mode is %d",
										   tc_mode, current_tc_mode);
		 return;		 
	   }
   }
   
   HDR_MSG_PROT_2 ( MSG_LEGACY_MED, "Setting TC mode to %d, as the current TC mode is %d",
										   tc_mode, current_tc_mode);


  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_TC_MODE_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.tc_mode.mode = tc_mode;

   /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_set_tc_mode */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_TC_MODE

DESCRIPTION  : This function retrieves the HDR traffic mode. 

               HDRSRCH_DEFAULT_TC_MODE   - Standard HDR traffic connection. 

               HDRSRCH_S1XHDR_HP_TC_MODE - High Priority Traffic Mode where
                                           HDR preempts 1x paging onto 
                                           chain 1 and preempts acquisition.

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : HDRSRCH_DEFAULT_TC_MODE   - Standard HDR traffic connection. 

               HDRSRCH_S1XHDR_HP_TC_MODE - High Priority Traffic Mode where
                                           HDR preempts 1x paging onto 
                                           chain 1 and preempts acquisition.
SIDE EFFECTS : None

=========================================================================*/

hdrsrch_tc_mode_enum_type hdrlmac_get_tc_mode (void)
{

  return hdrsrch_get_tc_mode();

} /* hdrlmac_get_tc_mode */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_DEMOD0_SASET_ID

DESCRIPTION  : This function retrieves the sub-active set id that was assigned
               to demod index 0.

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : sub-active id which was assigned to demode index 0 

SIDE EFFECTS : None

=========================================================================*/

uint8 hdrlmac_get_demod0_saset_id (void)
{
  uint8 saset_id;
  boolean found_saset = FALSE;

  for (saset_id = 0; saset_id < hdrlmac.num_sub_asets; saset_id++)
  {
    if (hdrlmac.srch_subactive_set[saset_id].demod_idx == 0)
    {
      found_saset = TRUE;
      break;
    }
  }

  if (!found_saset)
  {
    HDR_MSG_PROT ( MSG_LEGACY_ERROR, "Demod idx 0 was not assigned to any subaset");
    saset_id = 0;
  }
  else
  {
    HDR_MSG_PROT_1 ( MSG_LEGACY_MED, "Demod idx 0 was assigned to subaset %d", saset_id);
  }

  return (uint8) saset_id;

} /* hdrlmac_get_demod0_saset_id */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRLMAC_HAS_MULTI_RX_CAPABILITY

DESCRIPTION  : This function returns if hdr supports certain recevier 
               configuration.

DEPENDENCIES : None

PARAMETERS   :

  rx_cap    - rx chain configuration defined in the SCP layer.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

boolean hdrlmac_has_multi_rx_capability
( 
  hdrscmgmmcdp_multi_rx_capability_type rx_cap
    /* Multiple receiver capability */
)
{
  boolean capable = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  switch ( rx_cap )
  {
    case HDRSCMGMMCDP_MULTI_RX_CHAIN_FOR_COMMON_CHAN: 

      /* Pass the request to hdrsrch. */

      capable = hdrsrch_has_multi_rx( );

      break;

    default:

      /* Not capable for the multi receivers for different
       * mulitple dedicated channels */

      capable = FALSE;
  }

  return capable;

} /* hdrlmac_has_multi_rx_capability */

/* <EJECT> */
#ifdef FEATURE_HDR_IS890
/*=========================================================================

FUNCTION     : HDRLMAC_DISABLE_TC_OFS

DESCRIPTION  : This function is called if any application ( so far FTAP or
               RTAP ) wants to disable TC OFS. TC OFS is enabled by default 
               whenever HDRSRCH enters traffic state. 

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_disable_tc_ofs ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_disable_tc_ofs( );

} /* hdrlmac_disable_tc_ofs */


/*=========================================================================

FUNCTION     : HDRLMAC_SET_TAP

DESCRIPTION  : This function is called if any TAP application is running. 

DEPENDENCIES : None

PARAMETERS   : 

  tap -  whether TAP is running

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrlmac_set_tap
(
  boolean                    tap
    /* Whether tap is running */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_set_tap( tap );

}  /* hdrlmac_set_tap */

/*=========================================================================

FUNCTION     : HDRLMAC_GET_CARRIER_MAP

DESCRIPTION  : 
  This function gets the carrier map from the most recent TCAM, and returns 
  the current number of active carriers.
  If no pointer is provided for the carrier map info, only the number of 
  carriers is returned.

DEPENDENCIES : Active traffic connection is assumed.

PARAMETERS   : 
  num_carriers - Maximum number of elements to place in the carrier_map 
                 array.
  carrier_map - Pointer to an array of hdrlmac_carrier_map_type.

RETURN VALUE : 
  hdrlmac.num_sub_asets

SIDE EFFECTS : None
  
=========================================================================*/

uint8 hdrlmac_get_carrier_map
( 
  uint8                     num_carriers,
    /* Maximum number of elements to place in the carrier_map array */

  hdrlmac_carrier_map_type  *carrier_map 
    /* Pointer to an array of hdrlmac_carrier_map_type */
)
{

  /* If no structure is supplied, just return number of carriers. */
  if (carrier_map != NULL)
  {
    if( num_carriers <= HDRSRCH_MAX_NUM_SUBASETS ) 
    {
      /* Copy contents of stored carrier map from last TCA */
#ifdef FEATURE_MEMCPY_REMOVAL
      memscpy( 
        (void*)carrier_map, 
        sizeof(hdrlmac_carrier_map_type) * num_carriers, 
        (void*)hdrlmac.carrier_map, 
        sizeof( hdrlmac_carrier_map_type ) * num_carriers );
#else
      memcpy( 
        (void*)carrier_map, 
        (void*)hdrlmac.carrier_map, 
        sizeof( hdrlmac_carrier_map_type ) * num_carriers );
#endif /* FEATURE_MEMCPY_REMOVAL */
    }
    else
    {
      /* Error checking */
      HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, "num_carriers too large %d", 
                       num_carriers );
    }
  }

  return hdrlmac.num_sub_asets;
}

/* <EJECT> */ 
/*===========================================================================
FUNCTION HDRLMAC_CLEAR_HANDOFF_COUNT()

DESCRIPTION
  This function clears out HO counter accessed via 
  hdrlmac_get_handoff_count(). 

DEPENDENCIES
  None
 
PARAMETERS
  tca_subaset_idx - SubActiveSet index to clear.
   
RETURN VALUE
  None
  
SIDE EFFECTS
  Updates hdrsrchlog with ho count information.
  
===========================================================================*/
void hdrlmac_clear_handoff_count
( 
  uint8 tca_subaset_idx
    /* SubActiveSet Index to clear */
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_clear_handoff_count( tca_subaset_idx );

} /* hdrlmac_clear_handoff_count */

/* <EJECT> */ 
/*===========================================================================
FUNCTION HDRLMAC_GET_HANDOFF_COUNT()

DESCRIPTION
  This function gets the number of handoffs since the last time 
  hdrlmac_clear_handoff_count was called. 

DEPENDENCIES
  None
 
PARAMETERS
  tca_subaset_idx - SubActiveSet index to get handoff count.
   
RETURN VALUE
  Number of handoffs since last hdrlmac_clear_handoff_count.
  
SIDE EFFECTS
  None
  
===========================================================================*/
uint32 hdrlmac_get_handoff_count
( 
  uint8 tca_subaset_idx
    /* SubActiveSet Index to get handoff count */
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrch_get_handoff_count( tca_subaset_idx );

} /* hdrlmac_get_handoff_count */

/*=========================================================================

FUNCTION HDRLMAC_DIVERSITY_COMB_ENABLED

DESCRIPTION 
  This function returns if diversity combining is
  enabled.

DEPENDENCIES
  None

RETURN VALUE
  True  - if diversity combining is enabled.
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrlmac_diversity_comb_enabled( void )
{
  return hdrsrch_diversity_comb_enabled();
}

#endif /* FEATURE_HDR_IS890 */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRLMAC_RUNTIME_DIVERSITY_PREF_CTRL

DESCRIPTION
  This function sets the diversity preference at runtime.
 
  It is the wrapper of the api hdrsrch_runtime_diversity_pref_ctrl

DEPENDENCIES
  None

PARAMETERS
  div_ctrl - Whether diversity is on.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_runtime_diversity_pref_ctrl
(
  hdrlmac_rx_div_enum_type       div_ctrl
    /* Runtime diversity control */ 
)
{
  /* Set the diversity preference at runtime */
  hdrsrch_runtime_diversity_pref_ctrl(div_ctrl);
}  /* hdrlmac_runtime_diversity_pref_ctrl */

/*========================================================================

FUNCTION     : hdrlmac_is_in_access_state

DESCRIPTION  : This function returns if lmac is in access state

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : Whether LMAC is in access state

SIDE EFFECTS : None

=========================================================================*/

boolean hdrlmac_is_in_access_state( void )
{
  return (hdrlmac.state == HDRLMAC_ACCESS_STATE);
}  /* hdrlmac_is_in_access_state */


/*===========================================================================

FUNCTION HDRLMAC_GET_TX_CARRIER_FREQ

DESCRIPTION
  This function returns the tx carrier freq in KHz for the band and channel
  specified in the parameters.

DEPENDENCIES
  None

PARAMETERS
  band       - Band
  chan_num   - Channel number

RETURN VALUE
  Freq in KHz.

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrlmac_get_tx_carrier_freq
(
  sys_band_class_e_type      band,
    /* Band */

  sys_channel_num_type       chan_num
    /* Channel number */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrch_get_tx_carrier_freq( band, chan_num );

}  /* hdrlmac_get_tx_carrier_freq */


/*===========================================================================

FUNCTION HDRLMAC_GET_RX_CARRIER_FREQ

DESCRIPTION
  This function returns the rx carrier freq in KHz for the band and channel
  specified in the parameter.

DEPENDENCIES
  None

PARAMETERS
  band       - Band
  chan_num   - Channel number

RETURN VALUE
  Freq in KHz.

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrlmac_get_rx_carrier_freq
(
  sys_band_class_e_type      band,
    /* Band */

  sys_channel_num_type       chan_num
    /* Channel number */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrch_get_rx_carrier_freq( band, chan_num );

}  /* hdrlmac_get_rx_carrier_freq */

/*=========================================================================

FUNCTION HDRLMAC_IS_BAND_CHAN_SUPPORTED

DESCRIPTION 
  This function checks if the band and channel is support by RF driver.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If band/channel is supported.
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/

boolean hdrlmac_is_band_chan_supported 
( 
  sys_band_class_e_type   band,
    /* Band */
  word                    chan
    /* Channel number */
)
{
  return hdrsrch_is_band_chan_supported(band, chan);
}

/*============================================================================

FUNCTION HDRLMAC_GET_BAND_SUBCLASS

DESCRIPTION
  This function gets the currently supported band subclasses of the given band.
  It is a wrapper of function hdrsrch_get_band_subclass().
  
DEPENDENCIES
  None
  
RETURN VALUE
  The mask of the current subclasses of the given band.

SIDE EFFECTS
  None

============================================================================*/

uint32 hdrlmac_get_band_subclass
(
  uint8    band_class
    /* Band Class */
)
{
  return hdrsrch_get_band_subclass(band_class);
} /* hdrlmac_get_band_subclass */

/*===========================================================================

FUNCTION HDRLMAC_CLEAR_PENDING_TCA_QUEUE

DESCRIPTION
  Clears pending TCA queue 

DEPENDENCIES
  None

PARAMETERS
  None
                     
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_clear_pending_tca_queue( void )
{
  hdrlmac_tca_buf_type *tca_buf_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rex_enter_crit_sect(&hdrlmac.crit_sect);

  while ((tca_buf_ptr = (hdrlmac_tca_buf_type *) 
              q_get(&hdrlmac.pending_tca_q)) != NULL)
  {
    hdrbuf_done( &tca_buf_ptr->hdr );
  }

  hdrlmac.l1_is_processing_tca = FALSE;

  rex_leave_crit_sect(&hdrlmac.crit_sect);
} /* hdrlmac_clear_pending_tca_queue */


#ifdef FEATURE_HDR_ADAPTIVE_DIV
#error code not present
#endif/* FEATURE_HDR_ADAPTIVE_DIV */

#ifdef FEATURE_LTE_TO_HDR
/*===========================================================================

FUNCTION HDRLMAC_TIME_TRANSFER

DESCRIPTION
  This function allows HDR timing information provided to HDR MC by other RAT 
  to be transferred to HDR searcher.
  
DEPENDENCIES
  None

PARAMETERS
  lte_ostmr - LTE timing information.
  cdma_sys_time - CDMA system time.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_time_transfer
(
  uint32 lte_ostmr,
  uint64 cdma_sys_time
)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_TRAN_SYS_TIME_REQ;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.tran_sys_time_req.lte_ostmr = lte_ostmr;
  hdrlmac_cmd.params.tran_sys_time_req.cdma_sys_time = cdma_sys_time;

 /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_time_transfer() */

/*===========================================================================

FUNCTION HDRLMAC_TIME_TRANSFER_DONE_CB

DESCRIPTION
  This function is called by searcher once the timing transfer is completed.
  
DEPENDENCIES
  None

PARAMETERS
  tran_sys_time_result - Time transfer success or failure status

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_time_transfer_done_cb
(
  hdrsrch_tran_sys_time_enum_type tran_sys_time_result
)
{
  hdrind_ind_data_union_type ind_data;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ind_data.tran_sys_time_result = tran_sys_time_result;

  hdrind_give_ind( HDRIND_LMAC_TIME_SYNC_DONE, &ind_data );

} /* hdrlmac_time_transfer_done_cb() */

/*===========================================================================

FUNCTION HDRLMAC_FAST_ACQUISITION

DESCRIPTION
  This function is called by HDRMC to initiate a fast acquisition procedure.
  
DEPENDENCIES
  HDR timing information should have already been provided to searcher.

PARAMETERS
  channel - Channel to acquire.
  pilot_pn - Pilot pn to acquire. 
  fast_acq_reason - reason for fast acqusition (reselection or active ho)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_fast_acquisition
( 
  sys_channel_type channel,
  sys_pilot_pn_type pilot_pn
#ifdef FEATURE_LTE_TO_HDR_OH
  ,hdrsrch_fast_acq_reason_enum_type fast_acq_reason
  ,uint8 srch_window_size
#endif /* FEATURE_LTE_TO_HDR_OH */
)
{
  /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   /* Fill in all members of the hdrlmac_cmd_type for this command*/
  hdrlmac_cmd.name = HDRLMAC_FAST_ACQUISITION;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.fast_acq_cmd.channel = channel;
  hdrlmac_cmd.params.fast_acq_cmd.pilot_pn = pilot_pn;
  hdrlmac_cmd.params.fast_acq_cmd.fast_acq_cb = hdrlmac_fast_acquisition_done_cb;
  hdrlmac_cmd.params.fast_acq_cmd.system_lost_cb = hdrlmac_system_lost_cb;
  hdrlmac_cmd.params.fast_acq_cmd.usability_cb = hdrlmac_usable_cb;
#ifdef FEATURE_LTE_TO_HDR_OH
  hdrlmac_cmd.params.fast_acq_cmd.reason = fast_acq_reason;
  hdrlmac_cmd.params.fast_acq_cmd.srch_window_size = srch_window_size;
#endif /* FEATURE_LTE_TO_HDR_OH */

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));

} /* hdrlmac_fast_acquisition() */

/*===========================================================================

FUNCTION HDRLMAC_FAST_ACQUISITION_DONE_CB

DESCRIPTION
  This function is called by searcher once the fast acquisition is completed.
  
DEPENDENCIES
  None

PARAMETERS
  fast_acq_result - Fast Acquisition success or failure status

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_fast_acquisition_done_cb
(
  hdrsrch_fast_acq_enum_type fast_acq_result
)
{
  hdrind_ind_data_union_type ind_data;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ind_data.fast_acq_result = fast_acq_result;

  hdrind_give_ind( HDRIND_LMAC_FAST_ACQ_DONE, &ind_data );

} /* hdrlmac_fast_acquisition_done_cb() */
#endif /* FEATURE_LTE_TO_HDR */

#ifdef FEATURE_IRAT_PILOT_MEAS
/*===========================================================================

FUNCTION HDRLMAC_IRAT_PILOT_MEASUREMENTS_REQ

DESCRIPTION
  This function is called when IRAT pilot measurements are required 
  from searcher.
  
DEPENDENCIES
  None

PARAMETERS
 pilot_meas_req - DO Pilots for which measurement is requested.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_irat_pilot_measurements_req
(   
  hdrsrch_pilot_meas_req_params_type pilot_meas_req
)
{
 /* Build command into local struct */
  hdrlmac_cmd_type hdrlmac_cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   /* Fill in all members of the hdrlmac_cmd_type for this command */
  hdrlmac_cmd.name = HDRLMAC_PILOT_MEAS_CMD;
  hdrlmac_cmd.sender = HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL;
  hdrlmac_cmd.params.pilot_meas_req = pilot_meas_req;

  /* This function copies the command into a buffer before it returns */
  hdrtx_queue_cmd (HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL,
         &hdrlmac_cmd, sizeof (hdrlmac_cmd_type));*/

} /* hdrlmac_irat_pilot_measurements_req */

#endif /* FEATURE_IRAT_PILOT_MEAS */

#ifdef FEATURE_HDR_TO_LTE
/*===========================================================================

FUNCTION HDRLMAC_LTE_NSET_UPDATE

DESCRIPTION
  This function is called to update the searcher with LTE nset.
  
DEPENDENCIES
  None

PARAMETERS
  lte_nset_update_params - LTE frequency information.
  lte_band_pref - LTE band pref info from SD.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_lte_nset_update
(   
  hdrsrch_lte_nset_update_params_type   *lte_nset_update_params,
#if (LTE_BAND_NUM == 256)
  sys_lte_band_mask_e_type lte_band_pref
#else
	sys_band_mask_type lte_band_pref
#endif /* (LTE_BAND_NUM == 256) */
)
{
  lte_nset_update_params->reselect_cb = hdrlmac_irat_reselect_cb;

  hdrsrch_lte_nset_update( lte_nset_update_params );

} /* hdrlmac_lte_nset_update */

/*===========================================================================

FUNCTION HDRLMAC_ABORT_RESELECT_TO_LTE

DESCRIPTION
  This function is called to inform the searcher that D2L reselection is 
  being aborted.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_abort_reselect_to_lte( void )
{

  /* Call the appropriate SRCH API, which posts the abort reselection command
     to SRCH. */
  hdrsrch_abort_reselect_to_lte();
}

#endif /* FEATURE_HDR_TO_LTE*/

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION



/*===========================================================================

FUNCTION HDRLMAC_IRAT_RESELECT_CB

DESCRIPTION
  Callback called from HDR searcher to report when the IRAT reselection 
  is to triggered.

DEPENDENCIES
  None

PARAMETERS
  earfcn_id - Frequency of the LTE cell to be selected
  cell_id - Cell id of the LTE cell to be selected

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrlmac_irat_meas_rpt_cb 
(
 hdrsrchtclte_rpt_struct_type *redirect_rpt_ptr
)
{
  hdrind_ind_data_union_type ind_data;
  ind_data.redirect_rpt = *redirect_rpt_ptr;
  hdrind_give_ind( HDRIND_LMAC_ORAT_MEAS_RPT, (void *)&ind_data );
} /* hdrlmac_irat_redirect_rpt_cb() */

/*===========================================================================

FUNCTION HDRLMAC_LTE_NSET_UPDATE_REDIR

DESCRIPTION
  This function is called to update the searcher with LTE nset.
  
DEPENDENCIES
  None

PARAMETERS
  lte_nset_update_redir_params - LTE measurement info.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_lte_nset_update_redir
(   
  hdrsrch_lte_nset_update_redir_params_type   *lte_nset_update_redir_params
)
{
  lte_nset_update_redir_params->redirect_cb = hdrlmac_irat_meas_rpt_cb;

  hdrsrch_lte_nset_update_redir( lte_nset_update_redir_params );

} /* hdrlmac_lte_nset_update_redir */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION*/


/*=========================================================================

FUNCTION HDRLMAC_GET_SUBPKT_PROTOCOL_DATA

DESCRIPTION
  This function gets value for the fields, which are related to HDRMAC, in
  subpacket protocol data. 

DEPENDENCIES
  None

PARAMETERS
  subpkt_ptr - pointer to the subpacket to be set

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrlmac_get_subpkt_protocol_data
(
  hdrdiag_hdr_protocol_data_subpkt_protocol_data_type *subpkt_ptr
)
{
  hdrdec_pkt_info_s_type pkt_info;   /* Packet info */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* RRI */
  subpkt_ptr->cur_pkt_rri = (uint8)hdrrmac0_get_current_pkt_rri();
  subpkt_ptr->max_pkt_rri = (uint8)hdrrmac0_get_max_pkt_rri();

  /* Frame header info */
  hdrdec_get_packet_info(&pkt_info);
  subpkt_ptr->drc = (uint16)pkt_info.drc_decoded;
  subpkt_ptr->drc_requested = (uint16)pkt_info.drc_requested;
  subpkt_ptr->link_id = (uint8)pkt_info.link_id;

} /* hdrlmac_get_subpkt_protocol_data */
/*=========================================================================

FUNCTION     : HDRLMAC_GET_RX_TX_POWER_INFO

DESCRIPTION
  Returns HDR RX and TX AGC to the caller.
 
  Rxagc and txagc will be reported in 1\10 dBm granularity.
 
  Ecio will only be reported for primary chain, in traffic it will be
  instantaneous primar energy, in idle state it will be total energy.
  Ecio will also be reported in 1\10 dBm.

    
DEPENDENCIES
  None

INPUTS
  Pointers to the location where Rx and Tx power info structure will be stored

RETURN VALUE
  Rx and Tx agc value are returned through the passed hdrsrch_rx_tx_power_info_type
  pointer.

SIDE EFFECTS
  None

=========================================================================*/

void hdrlmac_get_rx_tx_power_info
(
  hdrsrch_rx_pwr_info_struct_type   *rx_power_info,
  hdrsrch_tx_pwr_info_struct_type   *tx_power_info
)
{

  hdrsrch_get_rx_tx_power_info(rx_power_info, tx_power_info);
  
  return;
  
}

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
/*=========================================================================

FUNCTION     : HDRLMAC_ACQ_DIV_CTRL

DESCRIPTION
    Enables or disables the acquisition diversity algorithm which dynamically
    uses the diversity antenna to improve acquisition performance in poor channels.
    If disabled, diversity will never be used in the acquisition state. If enabled,
    it will enable diversity selectively based on the channel condition and whether
    the diversity chain is available.
 
    A call to this API effects the next acquisition command that is accepted by the
    HDR searcher. It will not impact in progress search attempts.

DEPENDENCIES
  None

INPUTS
  Boolean which indicates whether to enable or disable the acquisition algorithm
 
RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrlmac_acq_div_ctrl
(
  boolean             acq_div_ctrl
)
{

  hdrsrch_acq_div_ctrl( acq_div_ctrl );
  
  return;
  
}

#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

#ifdef FEATURE_HDR_REVC
/* EJECT */
/*==========================================================================

FUNCTION HDRLMAC_GET_CC_MONITORING_CHAN

DESCRIPTION
  Checks if given channel is part of ASET channels

DEPENDENCIES

PARAMETERS
  chan_ptr      - Pointer to channel structure to check if it is part of 
                  ASET channels

RETURN VALUE
  True          - If channel is part of ASET
  False         - Otherwise

SIDE EFFECTS
  None

==========================================================================*/
sys_channel_type* hdrlmac_get_cc_monitoring_chan( void )
{
  return hdrsrchset_get_cc_monitoring_chan();
}

/*==========================================================================

FUNCTION HDRLMAC_PILOT_IS_IN_ASET

DESCRIPTION
  Check if the given pilot is part of ASET

DEPENDENCIES
  None

PARAMETERS
  chan - Channel to search for
  pn   - Pilot PN to search for
 
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrlmac_pilot_is_in_aset
(
  const sys_channel_type* chan,
  const sys_pilot_pn_type pn
)
{
  return hdrsrchset_pilot_is_in_aset(chan, pn);
}


/*==========================================================================

FUNCTION hdrlmac_get_aset_channel_list

DESCRIPTION
  Retrieves the list of aset channels from srch

DEPENDENCIES
  None

PARAMETERS
  chan - Channel info to be filled
 
RETURN VALUE
  Number of channels in aset

SIDE EFFECTS
  None

==========================================================================*/
uint8 hdrlmac_get_aset_channel_list
(
  sys_channel_type* aset_list  
)
{
  return hdrsrchset_get_aset_channel_list(aset_list);
}

/*==========================================================================

FUNCTION HDRLMAC_IS_LINKID_ON_CC_MONITORING_CHAN

DESCRIPTION
  Checks if given link_id is on CC monitoring chan. This API is created
  as a faster alternative to calling hdrsrch_get_link_info() then compare
  the channel with hdrsrchset_get_cc_monitoring_chan().

DEPENDENCIES
  None

PARAMETERS 
  link_id - link_id to check 

RETURN VALUE
  True   - if link_id is on CC monitoring chan
  False  - otherwise

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrlmac_is_linkid_on_cc_monitoring_chan
(
  const hdrsrch_link_id_type link_id
)
{
  return hdrsrchset_is_linkid_on_cc_monitoring_chan(link_id);
}
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_DUAL_SIM
/*===========================================================================

FUNCTION HDRLMAC_STANDBY_PREF_CHGD

DESCRIPTION
  This function is called to inform HDR that standby pref has
  changed.
  
DEPENDENCIES
  Must be called when HDR is inactive

PARAMETERS
  ds_pref - standby preference

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_standby_pref_chgd
(
  sys_modem_dual_standby_pref_e_type ds_pref
)
{
  HDR_MSG_PROT( MSG_LEGACY_HIGH, 
                  "HDR DSDS: mode pref changed" );
  hdrmultirat_standby_pref_chgd(ds_pref);
}

#endif /* FEATURE_HDR_DUAL_SIM */

/*===========================================================================

FUNCTION HDRLMAC_IS_LRE_ENABLED

DESCRIPTION
  This function returns enable/disable status of LRE feature. The feature
  can be turned on/off via NV.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if LRE feature is enabled
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrlmac_is_lre_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrch_is_lre_enabled();
    /* Return LRE feature enabled/disabled status from srch*/

} /* hdrlmac_is_lre_enabled( ) */

/*===========================================================================

FUNCTION HDRLMAC_IS_IDLE_TA_STARTED

DESCRIPTION
  This function returns TA status from L1.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tune away is started
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrlmac_is_idle_ta_started( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrch_is_idle_ta_started();
    /* Return if tune away is to be started in SRCH*/

} /* hdrlmac_is_idle_ta_started( ) */
/*=========================================================================

FUNCTION HDRLMAC_HAS_CHAIN

DESCRIPTION 
  This function returns if HDR has chain or not
DEPENDENCIES
  None

RETURN VALUE
  True  - If the primary chain is locked to HDR..
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrlmac_has_chain( void )
{
  return hdrsrchrf_has_chain();
}/*hdrlmac_has_chain()*/

/*===========================================================================

FUNCTION HDRLMAC_SET_FORCE_LTA

DESCRIPTION
  This function sets the force LTA in SRCH. 

DEPENDENCIES
  None

PARAMETERS
  value - if force LTA needs to be enabled or not

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrlmac_set_force_lta
(
  boolean value
)
{
  hdrsrch_set_force_lta(value); 
}

/*===========================================================================

FUNCTION HDRLMAC_UPDATE_SUBSCRIBER_ID

DESCRIPTION
  This function updates RF module the current HDR subscriber id
  
DEPENDENCIES
  Must be called after RF task is initialized

PARAMETERS
  sid - current subscriber id

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrlmac_update_subscriber_id
(
  sys_modem_as_id_e_type sid
)
{
  HDR_MSG_PROT_1( MSG_LEGACY_MED, 
                  "Updating subscriber id: %d", 
                  sid );

  hdrsrch_update_subscriber_id( sid );
}
