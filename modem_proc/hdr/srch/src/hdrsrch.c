/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              H D R    S R C H    C O M M A N D    I N T E R F A C E

                           Search Interface Module

GENERAL DESCRIPTION
  This file provide all external interface functions for the HDR Searcher.
  These functions may directly handle the request, or queue the command
  on the HDR Searcher's command queue, for processing in the HDR Search
  context.

EXTERNALIZED FUNCTIONS
  hdrsrch_deactivate
  hdrsrch_set_search_params
  hdrsrch_set_mgmt_same_channel_params
  hdrsrch_set_control_channel_cycle
  hdrsrch_get_preferred_control_channel_cycle
  hdrsrch_acquire
  hdrsrch_synchronize
  hdrsrch_idle_mode
  hdrsrch_nset_update
  hdrsrch_idle_channel_change
  hdrsrch_sleep
  hdrsrch_access_mode
  hdrsrch_access_handoff
  hdrsrch_access_mode_ended
  hdrsrch_chan_and_aset_assign
  hdrsrch_aset_update
  hdrsrch_enable_pilot_reports
  hdrsrch_pilot_report_reset
  hdrsrch_get_pilot_report
  hdrsrch_get_link_info
  hdrsrch_at_set_drc
  hdrsrch_get_mac_index
  hdrsrch_get_drc_cover
  hdrsrch_set_ho_delays
  hdrsrch_register_roll_callback
  hdrsrch_deregister_roll_callback
  hdrsrch_get_ecio_and_io
  hdrsrch_get_sinr_and_rxagc
  hdrsrch_get_asp_pn_mapping
  hdrsrch_get_pn_ecio
  hdrsrch_set_user_diversity_pref
  hdrsrch_set_user_equalizer_pref
  hdrsrch_set_user_debug_mask
  hdrsrch_set_user_debug_array
  hdrsrch_set_bcmcs_page_cycle  
  hdrsrch_set_bcmcs_sleep_cycle
  hdrsrch_update_bc_info
  hdrsrch_update_qsm_info
  hdrsrch_set_tc_mode  
  hdrsrch_get_tc_mode
  hdrsrch_set_tap
  hdrsrch_acmac_ready  
  hdrsrch_acmac_ta_time_ok
  hdrsrch_rup_request
  hdrsrch_notify_cgps_status
  hdrsrch_chan_and_aset_assign_revb
  hdrsrch_aset_update_revb
  hdrsrch_get_schdgrp_rpt
  hdrsrch_assign_demod_idxes_to_subasets
  hdrsrch_lte_nset_update
  hdrsrch_is_grice_enabled
  hdrsrch_update_sector
  hdrsrch_lte_nset_update_redir

REGIONAL FUNCTIONS
  None

INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrsrch_task( ) must be called to initialize the HDR Searcher.
  hdrsrch_task( ) will call mc_task_start( ) when it has initialized itself.

  Copyright (c) 2000 - 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrch.c#2 $
$DateTime: 2019/11/19 23:42:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
11/20/2019   vaa     Updating RF with sub id
11/12/2018   svu     Added debug F3 at updatiopn of subid from LMAC
11/16/2017   svu     Fixed setting the sub id bit for the time transfer cmd
10/29/2017   vlc     Create API for L1 to get HDR subscription ID. 
10/29/2017   vko     FR 29153: Support HDR/CDMA on the 2nd sub
09/04/2017   vko     Force LTA if LTE subs is not present on DO sub (temp DDS)
08/03/2017   svu     Added API that retuns hdr SUBID
12/22/2015   rmv     Set sysloss_pending when posting to HDRSRCH
07/24/2015   wsh     Fixed TX chain event being treated as PRI chain 
06/30/2015   wsh     Removed incorrect TRM deregistering.
05/28/2015   vlc     Added support for DO->LTE redirection in connected state.
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
05/07/2015   vlc     Added hdrsrch_abort_reselect_to_lte().  
04/29/2015   vko     Inform TCA processing status, to HDRSRCH during IDLE_MODE_CMD
02/16/2015   vke     Changes to use the Aset win size shared via IRAT
02/13/2015   vke     Promotion for CR790412
02/04/2015   vke     Changes to ignore sync cmd if processing delay > SCC
12/18/2014   wsh     Enabled MMSE by default
12/03/2014   ljl     Declared system lost if CC packet HSTR was incorrect.
10/14/2014   wsh     Changes to support Jolokia bringup
09/09/2014   vke     Redesign fix for connection close during QTA gap
08/01/2014   sat     Changes to ignore Fast connect TCA processing while TA
08/22/2014   vke     Delay connection close during QTA gap
08/12/2014   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold 
08/07/2014   wsh     Enable MMSE detection by default on BOLT
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
07/16/2014   arm     Added NV control to enable/disable QTA.
07/09/2014   cnx     Support NV refresh without reset.
05/15/2014   arm     TRM API changes for DR-DSDS. 
05/12/2014   vke     Klockworks fix
05/05/2014   ljl     Added hdrsrch_idle_channel_change_and_nset_update().
04/28/2014   wsh     Added NV control to pass MMSE config to FW
08/19/2013   arm     Merged DSDS feature for triton. 
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
12/20/2013   vlc     Added support for Tbackoff timer optimization. 
12/19/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/27/2013   dsp     FR17482 for Page blocking management.
10/29/2013   sat     Reduced F3 messages (or/and) ASSERTs
08/08/2013   vke     Added handling of sync substate callback
07/31/2013   sat     Mainlining Modem Statistics Feature
06/26/2013   wsh     Added Selective TxD feature
05/23/2013   sat     Fix Compilation errors after enabling FEATURE_MODEM_STATISTICS
05/02/2013   ljl     Added hdrsrch_set_scc_info().
03/08/2013   rmv     Added support for stats cdma_evdo_rx0_agc [rx0_dbm], 
                     cdma_evdo_rx1_agc [rx1_dbm]
01/23/2013   wsh     Initialize DIV/ARD on timesync
01/14/2013   ljl     Added hdrsrch_acq_search_active(). 
12/14/2012   ukl     Fix Klocwork issues
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
05/22/2012   smd     Fixed a bug in cmd purge when both tx and srch accessing
                     command queue.
04/20/2012   arm     Supported diversity in reacq. 
04/25/2012   smd     Changes to repost some commands when purging command queue.
04/19/2012   wsh     Fixed race cond. that lead to time tras cb being NULL
04/17/2012   ljl     Added critical section for pilot report.
03/08/2012   wsh     Merged NLB support
02/14/2012   wsh     Added support for SVLTE+SHDR
01/09/2012   smd     Supported dynamic acquisition diversity.
12/08/2011   arm     Change default values for Idle Div thresholds.
11/11/2011   arm     HDR idle and access state diversity implemetation.
11/08/2011   ljl     Avoided 1x and hdr rf tune conflict.
10/25/2011   ljl     Handled the failure cases during optimized HO.
10/24/2011   ljl     Supported RPM optimized timeline.
10/10/2011   arm     Supported second chain test call feature.
09/30/2011   ljl     Supported active HO.
09/23/2011   ljl     Added HDRSRCH_SET_CC_START_CMD, HDRSRCH_SET_SCC_OFFSET_CMD,
                     and HDRSRCH_AT_SET_DRC_CMD.
06/30/2011   ssu     Added hdrsrch_get_band(). 
05/31/2011   ssu     Returned ecio in -0.5dB unit in hdrsrch_get_ecio_and_io(). 
05/31/2011   smd     Featurized IRAT L2DO updates for Nikel.
05/05/2011   cnx     Added hdrsrch_get_band_subclass().
04/28/2011   smd     IRAT LTE to DO updates for Nikel.
04/27/2011   rmg     Added HDR MCPM support changes.
04/28/2011   kss     Removed unused FW message fields.
04/27/2011   kss     Added RF include to remove warnings.
03/22/2011   arm     Added support for thermal pa backoff feature. 
04/12/2011   smd     Used a hdr abstract type for fw state. 
03/31/2011   smd     Exposed API to conver linear eng to db. 
03/24/2011   kss     Updated RF SVDO API support.
03/07/2011   smd     Removed rxAgcAccum from PilotMeasReq message. 
03/05/2011   arm     New API added to provide Rx and Tx power info to CM. 
01/17/2011   smd     Supported FTM for IRAT connected mode measurement.
01/08/2011   rkc     Supported returning SINR in 0.5 dB units. 
01/06/2011   kss     Added hdrsrch_is_grice_enabled(). 
11/21/2010   pxu     Added code to calculate and save fctr to make window sizes max.  
11/18/2010   grl/lyl Updated mac index with correct value in FTD data structure.
11/11/2010   vlc     Fixed sizeof parameter in hdrsrch_lte_nset_update().
09/16/2010   lyl     Fixed FTD channel info for FEATURE_HDR_REVB;
09/14/2010   arm     Replace TASKLOCK  with critical section.  
09/01/2010   vlc     Added support for DO->LTE pilot measurements/reselection.
06/22/2010   ljl     Replaced INTLOCK/INTFREE with REX_DISABLE/ENABLE_PREMPTION.
06/21/2010   cnx     Fixed compiler warnings.
06/10/2010   lyl     Added hdrsrch_runtime_diversity_pref_ctrl().
06/05/2010   smd     Fixed bug of not copying param overall_time_allowed.
05/01/2010   smd     Updated acquire and synchronization API for reverse time 
                     transfer.
03/24/2010   pxu     Removed FEATURE_HDR_QSM featuers, FEAURE_HDR_EQ feature checks
                     FEATURE_7800_HDEM_WORKAROUND features. 
03/10/2010   smd     Replaced TRAN_SYS_TIME_CMD with message id.
03/10/2010   lyl     Added hdrsrch_get_ftd_data().
03/05/2010   smd     Fixed bugs in LTE pilot measurement feature.
02/12/2010   ljl     Added div settings in hdrsrch struct.
02/09/2010   smd     Added APIs for LTE to DO reselection.
12/29/2009   ljl     Added API to enable firmware HDR App.
09/28/2009   smd     Fixed KW errors.
08/11/2009   etv     Used actual demod carrier instead of HDRMDSP_DEMOD_CARRIER_0
08/18/2009   etv     Changed hdrsrch_get_demod_carrier_info to 
                     hdrsrch_get_fl_carrier_info and added 
                     hdrsrch_get_subaset_index.
07/22/2009   ljl     Passed the frame offset to HDR SRCH.
07/16/2009   lyl     Supported CC config MSGR interface.
06/19/2009   grl     Switched to the hdrsrchrfdiv API. Mainlined per band div.
05/18/2009   lyl     Updated for firmware MSGR interface.
03/18/2009   rkc     Added RF SVDO API changes.
03/20/2009   rkc     Added rup_subtype param to hdrsrch_nset_update().
03/09/2009   smd     Added support for pilot measurement velcro solution.
03/06/2009   ljl     Added hdrsrch_get_asp_index.
12/10/2008   smd     Replaced assert.h with amssassert.h
11/22/2008   grl     Added ARDA enable/disable APIs.
11/20/2008   rkc     Fixed lint errors.
11/10/2008   kss     Removed/replaced intlocks.
11/10/2008   ljl     Added hdrsrch_acmac_disable_ta().
10/13/2008   lyl     Fixed lint errors.
10/09/2008   lyl     Added QPCH support
09/18/2008   rkc     Copied Intra-SCC params to sleep cmd.
09/08/2008   rkc     Added Intra-SCC sleep params to hdrsrch_sleep().
08/31/2008   lyl     Suppressed lint error in hdrsrch_set_user_debug_array().
08/15/2008   wsh     Fixed crash if TCA contains invalid channel
07/29/2008   etv     Removed redundant hdrsrchrf_enter_mcdo.
07/09/2008   etv     Save the CC SubASETIdx after ASET Update Command.
07/01/2008   ljl     Added functions to get rx/tx frequency.
05/09/2008   rkc     Added hdrsrchlog.h.
04/17/2008   rkc     Added hdrsrch_notify_cgps_status().
03/27/2008   ljl     Added the avoided channel list to nset update function.
02/29/2008   ljl     Initialized the debug mask to disable access hybrid.
02/20/2008   ljl     Removed the parameter from hdrsrch_get_hdr_rssi().
02/12/2008   jyw     Added sanity check in the TCA processing.
02/10/2008   jyw     Added demod overriding mode for the revA and revB traffic.
01/30/2008   jyw     Modified the demod assignment interface.
01/30/2008   jyw     Obsoleted hdrsrch_if_chan_change.
12/17/2007   jyw     Updated with revB get_pilot_report with revB MSTR macros.
11/20/2007   jyw     Fixed the logic in the hdrsrch_if_channel_change.
11/13/2007   rkc     Fixed handdown mode initialization.
11/06/2007   grl     Fixed HPT mode not being saved properly in idle.
10/31/2007   ljl    Merged from Rev B branch.
10/17/2007   grl     Merged Rev B changes from branch.
10/04/2007   rkc     Resolved Lint High warnings.
09/20/2007   mt      Added hdrsrch_update_num_sector_users().
09/18/2007   wsh     Report correct DiversityComb and SimultaneousCommonChannelReceive
                     value in Multi-Mode Discovery protocol
08/19/2007   grl     Resolved Lint High warnings.
08/08/2007   mpa     Added hdrsrch_get_tc_mode(). 
07/18/2007   mt      HDR per band diversity changes.
06/18/2007   rkc     Added support for hdrsrch_dbg_array[].
05/15/2007   mt      Added hdrsrch_rup_request().
05/14/2007   grl     Added support for the FEATURE_HDR_ENG_IN_ECIO_UNITS iface.
04/27/2007   grl     Removed tc_mode from TC assignment for access hybrid ctrl.
11/10/2006   mt      Changed default value for hdrsrch_dbg_msk.
10/11/2006   grl     Added passing of the negotiated SCI value to search.
10/09/2006   mt      Added hdrsrch_notify_cc_packet().
08/30/2006   sq      Added FEATURE_HDR_QSM
08/29/2006   mt      Defined default value for hdrsrch_dbg_msk.
07/10/2006   mt      Added hdrsrch_set_user_debug_mask().
06/30/2006   etv     Supported DSP Handoff algorithm changes.
06/07/2006   ljl     Added support for hybrid access
05/22/2006   ljl     Added hdrsrch_set_tap().
03/30/2006   sq      Added hdrsrch_set_bcmcs_sleep_cycle
03/22/2006   sq      Changed BOM period update to a command interface
01/15/2006   jyw     Added support to query of the exist of multiple rx chain.
12/14/2005   sq      Added support to disable TC OFS in the TAP calls.
10/20/2005   jyw     Fixed lint errors.
09/10/2005   grl     Added dynamic TC mode support for HPTS mode.
08/18/2005   ljl     Supported cached sector information for IHO
07/28/2005   ds      Added usage of hash channel list in NSET update
07/27/2005   sq      Added support for BCMCS page monitoring
07/12/2005   sq      Added support for enhanced idle state protocol
07/12/2005   ds      Corrected NSET update interface sect list pointer
07/12/2005   ds      Add channel list, cached sectors to NSET update interface
06/29/2005   ds      Changed function name to set equalizer preference
05/18/2005   ljl     Fixed NULL pointer problem
04/21/2005   ds      Added function to set equalizer preference read from NV
03/23/2005   hrk     Purge command queue, then post DEACTIVATE command and sig 
02/01/2005   sq      Added function hdrsrch_get_sinr_and_rxagc
01/13/2005   sq      Mainlined FEATURE_HDR_NO_HO_DURING_SCC
12/16/2004   sq      Removed hdrsrch_access_mode_ended function
12/08/2004   sq/dna  Keep the non-null deactivate cb until it is called
11/12/2004   ajn     Added #include for previous indirect #include.
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
08/30/2004   aaj     Added enum type for bcmcs status
07/30/2004   sq      Added support for FEATURE_HDR_BCMCS
07/19/2004   sq      Bug fix in function hdrsrch_get_asp_pn_mapping
06/30/2004   dna     Inactive state support. Added hdrsrch_activate cmd
06/09/2004   sq      Added function hdrsrch_get_pn_ecio
06/08/2004   sq      Added function hdrsrch_get_asp_pn_mapping
04/05/2004   sq      Returns Ec/Io and Io for HDR
04/05/2004   aaj     Added INTLOCK for color code update
03/04/2004   aaj     Support for getting color_code from protocols
11/18/2003   aaj     Support for usability API with LMAC
10/09/2003   ajn     Added support for "Advise Unlock RF" command
09/10/2003   mpa     Converted F3 messages to MSG 2.0
05/27/2003   ajn     Moved Pilot Report generation logic to hdrsrchset_*
03/11/2003   ajn     Updated Copyright notice
02/26/2003   aaj     Changes to MPS threshold setting
02/19/2003   sq      Do not write to DSP if we are not in MPS mode
01/30/2003   ajn     get_pilot_report() now returns pilot phase w.r.t. MSTR,
                     not the reference pilot's earliest arriving multipath.
01/21/2003   sq      Added support for MPS testing
01/10/2002   ajn     Added FEATURE_HDR_NO_HO_DURING_SCC
11/28/2002   aaj     Added support hdr data level indicator
10/28/2002   aaj     Added support for acq mode for acquisition commands
11/07/2002   ajn     Roll handler moved to hdrts
09/18/2002   aaj     Added API for getting HDR RSSI for displaying signal bars
08/28/2002   aaj     Do not report CSET sector in RUP if it is reference
08/08/2002   aaj     Corrected even slot check in pref control cycle function
06/14/2002   ajn     Access H/O Threshold added to ACCESS_MODE_CMD
04/29/2002   aaj     Change to F3 msg to avoid 40 char overflow
03/07/2002   aaj     Added state checking in IS890 code for DRC param ctrl
                     <merged from the branch 1.37.1.1>
03/06/2002   sst     Updated calls to hdrsrchset_get_sector_report()
02/28/2002   aaj     Set external diversity control as per user pref thru NV
02/22/2002   sst     Updated calls to hdrsrchset_get_sector_report()
02/05/2002   ajn     hdrerrno -> errno, suspend callback add to Idle/TC
01/23/2002   ajn     Deactivate CMD changed to SIG.
01/17/2002   aaj     Changed sector struct to contain band+chan number
01/14/2002   aaj     Added traffic chan ack received cmd support
12/07/2001   ajn     Added Pilot to Channel Change command.
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
                     hdrsrchidle_set_control_channel_hash now hdrsrchsleep_*
10/08/2001   aaj     Add get_drc_cover function for test Application
09/25/2001   ajn     Default to aset[0] if no reference pilot for report.
09/21/2001   aaj     If the ref pilot is invalid, write invalid pn in linkptr
09/13/2001   aaj     Added hdrsrch_register/deregister_roll_callback functions
09/13/2001   aaj     Added hdrsrch_set_ho_delays() function
09/04/2001   ajn     hdrsrch_sleep() interface change
08/30/2001   aaj     Removed ASSERT to verify ref pilot was in ASET
08/21/2001   aaj     Changes in pilot reporting for reference pilot
08/01/2001   aaj     Changes to DRC AT command interface.
07/05/2001   aaj     Added find MAC index function from Link ID
06/19/2001   aaj     Added AT command support for DRC rate change
06/18/2001   ajn     Improved link_id support.
04/10/2001   ajn     MSM5500 link_id support added.
03/02/2001   aaj     included customer.h file
02/27/2001   ajn     Deprecated functions & some magic numbers removed.
02/06/2001   aaj     functions name changes to conform with conventions
11/30/2000   ajn     Code Review changes
10/23/2000   ajn     hdrsrch_synchronize expects a qword instead of uint64.
10/19/2000   ajn     NULL channel pointer in TCA means current ASET channel.
07/07/2000   ajn     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrch.h"
#include "hdrsrchrf.h"
#include "hdrsrchi.h"
#include "hdrsrchcmd.h"
#include "hdrsrchstates.h"
#include "hdrsrchstate.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrmdspmcr.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchdef.h"
#include "hdrsrchtask.h"
#include "hdrsrchfing.h"
#include "hdrsrchafc.h"
#include "hdrsrchlog.h"
#include "hdrlog.h"
#include "hdrsrchidlei.h"
#include "msgr.h"
#include "hdrsrcheq.h"
#include "hdrts.h"
#include "amssassert.h"
#include "hdrdebug.h"

#ifndef FEATURE_HDR_REVB
#include "rf.h"
#include "rfm.h"
#else

#include "rfm_hdr.h"
#include "rfm_cdma.h"
#include "rfm_common.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVB
#include "hdrsrchschdgrp.h"
#include "hdrscp.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrmdsp.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/*------------------------------------------------------------------------
  Advance to next structure in an array of structures of a given size.
  Used when an array of large structures is passed in, where the elements
  at the start of the structure are known, and elements at the end of the
  structure may be unknown private information of the calling function.
------------------------------------------------------------------------*/

/* Use void pointer as a bridge to avoid the following compiler warning: 
   "cast increases required alignment of target type". */
#define NEXT_STRUCT(type,ptr,struct_size) \
  (ptr = (type *)((void *)((char*)ptr + struct_size)))


#ifdef FEATURE_HDR_THERMAL_BACKOFF

/* set bit 1 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_SET_USR_PREF_DIV_DISABLE(div_flag) \
  (div_flag = ( div_flag | ( 0x2)))

/* reset bit 1 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_CLR_USR_PREF_DIV_DISABLE(div_flag) \
  (div_flag = ( div_flag & ( 0xFD )))

/* set bit 0 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_SET_THERMAL_BACKOFF_DIV_DISABLE(div_flag) \
  (div_flag = (div_flag | ( 0x1 )))

/* reset bit 0 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_CLR_THERMAL_BACKOFF_DIV_DISABLE(div_flag) \
  (div_flag = (div_flag & ( 0xFE )))

/* set bit 2 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_SET_SECOND_CHAIN_PREF_ENABLE(div_flag) \
  (div_flag = ( div_flag | ( 0x04)))

/* reset bit 2 of the hdrsrch.div_ctrl_pref_vote flag */
#define HDRSRCH_CLR_SECOND_CHAIN_PREF_ENABLE(div_flag) \
  (div_flag = ( div_flag & ( 0xFB )))

/* check if div is enabled by ETM and the user (bit 0 and bit 1) */
#define HDRSRCH_ARE_ALL_DIV_VOTES_ENABLED(div_flag) \
  (( !(div_flag & 0x1) && !(div_flag & 0x2) )? TRUE : FALSE)

/* Check if second chain only test call is enabled */
#define HDRSRCH_IS_SECOND_CHAIN_ONLY_ENABLED(div_flag) \
  (( div_flag & 0x04 )? TRUE : FALSE)

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV

#define HDRSRCH_IS_ACCESS_DIVERSITY_ALLOWED( div_ctrl )   \
       ( ( (div_ctrl) & 0x40 ) && ( (div_ctrl) & 0x01 ) )
  /* Whether the diversity is allowed in access state */

#define HDRSRCH_IS_IDLE_DIVERSITY_ALLOWED( div_ctrl )  \
       ( (div_ctrl) & 0x80 && ( (div_ctrl) & 0x01 ) )
  /* Whether the diversity is allowed in idle state */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */


/*------------------------------------------------------------------------
  HDR Search Data Structure
------------------------------------------------------------------------*/

hdrsrch_struct_type hdrsrch;
  /* Data Storage for Global HDR SRCH Data */

uint64              hdrsrch_dbg_msk;
  /* Data storage for HDR SRCH debug mask item read from NV */

uint8               hdrsrch_dbg_array[HDRSRCH_DBG_ARRAY_SIZE];
  /* Data storage for HDR SRCH debug array item read from NV */

/* EJECT */
/*==========================================================================
                                   SUNDRY
==========================================================================*/


/*==========================================================================

FUNCTION HDRSRCH_ACTIVATE

DESCRIPTION
  Initialize hardware for HDR operation.  This is performed when HDR protocol
  is activated. This is supposed to be one time operation when HDR is enabled.
  Example: when HDR-only mode is entered from GSM only mode then MMoC will 
  activate HDR protocol by sending the activate command.  

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrch_activate()
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* hdrsrch_activate could be added as a new command, queued to SRCH
     This provices a hook to do one-time initialization for HDRSRCH */

} /* hdrsrch_activate */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_DEACTIVATE

DESCRIPTION
  Terminate all HDR processing.  To be used prior to Deep Sleep, transition
  back to 1x/AMPS.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Exits (aborts) any HDR operations in progress.

==========================================================================*/

void hdrsrch_deactivate
(
  hdrsrch_deactivate_done_cb_type         deactivate_done_cb
    /* Callback function for when the deactivation is complete */
)
{
  hdrsrch_cmd_type  *cmd;

  trm_async_event_cb_input_type input;
    /* Input for registering trm API */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( deactivate_done_cb != NULL )
  {
    hdrsrch.deactivate_done_cb = deactivate_done_cb;
      /* Store the deactivate done callback in the hdrsrch common structure */
  }

  /* To deactivate, all pending commands must be terminated, and the
     command queue emptied, before the deactivate command is sent.
     This should be done from the task context, for consistancy.
     A command cannot be used here, since it could remain on the command
     queue behind a suspend command; a signal is used instead. */

  rex_enter_crit_sect(&hdrsrch.crit_sect); 
    /* Prevent any other task from interrupting */

  hdrsrchcmd_purge( );
   /* Purge all commands in queue . We will repost some commands like 
     CHANGE_SLEEP_TIMELINE. Those commands only update values of internal data
     structure.
   */

  cmd = hdrsrchcmd_new( HDRSRCH_DEACTIVATE_CMD );
    /* Post DEACTIVATE command */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */

    HDRSRCH_SIG_SET( HDRSRCH_DEACTIVATE_SIG );
      /* Send signal to task to begin deactivation procedures. */
  }

  rex_leave_crit_sect( &hdrsrch.crit_sect );

} /* hdrsrch_deactivate() */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SET_SEARCH_PARAMS

DESCRIPTION
  Sets various HDR Searcher Parameters, such as window size and pilot inc.

DEPENDENCIES
  None

PARAMETERS
  search_params_ptr

RETURN VALUE
  None

SIDE EFFECTS
  Regional data updated

==========================================================================*/

void hdrsrch_set_search_params
(
  const hdrsrch_search_params_struct_type  *search_params_ptr
    /* Parameter structure pointer */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();
    /* Block interrupts during this update */

  hdrsrch.srch_params = *search_params_ptr;
    /* Copy the data */

  hdrsrch.saved_aset_win_size = hdrsrch.srch_params.aset_win_size;
    /* Copy the data */

  if ( search_params_ptr->pilot_inc == 0 )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                    "Invalid Pilot Inc (%d)", search_params_ptr->pilot_inc );
    hdrsrch.srch_params.pilot_inc = 4;
  }

  hdrsrchreacq_calc_max_win_factor(search_params_ptr->aset_win_size);
    /* Calculate the fctr to get maximum window size and save in hdrsrchreacq */

  REX_ENABLE_PREMPTION();
    /* Unblock interrupts */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Search Defaults: pn_inc=%d awin=%d nwin=%d",
                  search_params_ptr->pilot_inc,
                  search_params_ptr->aset_win_size,
                  search_params_ptr->nset_win_size );

} /* hdrsrch_set_search_params( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SET_MGMT_SAME_CHANNEL_PARAMS

DESCRIPTION
  Sets HDR Set Management Parameters for pilots on the ASET channel.

DEPENDENCIES
  None

PARAMETERS
  params_ptr

RETURN VALUE
  None

SIDE EFFECTS
  Regional data updated

==========================================================================*/

void hdrsrch_set_mgmt_same_channel_params
(
  const hdrsrch_chan_mgmt_params_struct_type  *params_ptr
    /* Parameter structure pointer */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();
    /* Block interrupts during this update */

  hdrsrch.same_chan_mgmt_params = *params_ptr;
    /* Copy the data */

  hdrsrchfing_process_pilot_drop( params_ptr->pilot_drop );
    /* act on the pilot drop threshold for the fingers */

  REX_ENABLE_PREMPTION();
    /* Unblock interrupts */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Same Chan:tadd=%d tdrop=%d tcomp=%d[.5dB]",
                  params_ptr->pilot_add,
                  params_ptr->pilot_drop,
                  params_ptr->pilot_compare );

} /* hdrsrch_set_mgmt_same_channel_params( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SET_MGMT_DIFF_CHANNEL_PARAMS

DESCRIPTION
  Sets HDR Set Management Parameters for pilots not on the ASET channel.

DEPENDENCIES
  None

PARAMETERS
  params_ptr

RETURN VALUE
  None

SIDE EFFECTS
  Regional data updated

==========================================================================*/

void hdrsrch_set_mgmt_diff_channel_params
(
  const hdrsrch_chan_mgmt_params_struct_type  *params_ptr
    /* Parameter structure pointer */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();    
    /* Block interrupts during this update */

  hdrsrch.diff_chan_mgmt_params = *params_ptr;
    /* Copy the data */

  REX_ENABLE_PREMPTION();
    /* Unblock interrupts */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Diff Chan:tadd=%d tdrop=%d tcomp=%d[.5dB]",
                  params_ptr->pilot_add,
                  params_ptr->pilot_drop,
                  params_ptr->pilot_compare );

} /* hdrsrch_set_params( ) */


#ifdef FEATURE_HDR_BCMCS
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_SET_BCMCS_PAGE_CYCLE

DESCRIPTION
  This command updates the BCMCS page cycle value.

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

void hdrsrch_set_bcmcs_page_cycle
(
  uint8 page_cycle
)
{

  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_UPDATE_BOM_PERIOD_CMD );
    /* Set Command Message Type */

  if ( cmd != NULL )
  {
    cmd->params.bom.period = page_cycle;
      /* Copy the BCMCS status information in the local command struct */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, " No command buffer available " );   
  }

} /* hdrsrch_set_bcmcs_page_cycle */


/*===========================================================================

FUNCTION HDRSRCH_SET_BCMCS_SLEEP_CYCLE

DESCRIPTION
  This command updates the BCMCS sleep cycle value.

DEPENDENCIES
  None

PARAMETERS
  sleep_cycle   - The BCMCS sleep period, in units of SCC ( 256 slots ).

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void hdrsrch_set_bcmcs_sleep_cycle
(
  uint8 sleep_cycle
    /* In units of SCC ( 256 slots )  */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchsleep_set_bcmcs_sleep_cycle ( sleep_cycle );

} /* hdrsrch_set_bcmcs_sleep_cycle */

#endif /* FEATURE_HDR_BCMCS */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SET_CONTROL_CHANNEL_CYCLE

DESCRIPTION
  Sets the control channel hash value, which defines the HDR slotting.
  Also sets the current maximum sleep cycle negotiated with AN.
  
DEPENDENCIES
  None

PARAMETERS
  control_channel_cycle - Value which defines HDR slot for mobile.
  max_sleep_cycle - negotiated max sleep cycle with AN.
  
RETURN VALUE
  None

SIDE EFFECTS
  Sleep in Idle Mode is enabled once the hash value is set.

===========================================================================*/

void hdrsrch_set_control_channel_cycle
(
  uint8   control_channel_cycle,
    /* Hash value "R" for "(C+R) mod N == 0"  */
  
  uint8   max_sleep_cycle
    /* final decided sleep cycle index between AT and AN */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchsleep_set_control_channel_cycle( control_channel_cycle, 
                                          max_sleep_cycle       );
    /* Direct call - no need for task switch! */

} /* hdrsrch_set_control_channel_cycle( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_PREFERRED_CONTROL_CHANNEL_CYCLE

DESCRIPTION
  Gets the preferred control channel cycle value for HDR slotting.

DEPENDENCIES
  None

PARAMETERS
  page_slot   - assigned slot in IS-95A/B or IS2000 1x system.
  max_hdr_sleep_cycle - current maximum HDR sleep cycle index
  default_scc - slot cycle index calculated by hashing
  
RETURN VALUE
  Hash value "R" for "(C+R) mod N == 0"

SIDE EFFECTS
  None

==========================================================================*/

uint8 hdrsrch_get_preferred_control_channel_cycle
(
  uint16                          page_slot,
    /* Assigned slot in IS-95A/B or IS2000 1x system */

  uint8                           max_hdr_sleep_cycle,
    /* maximum HDR sleep cycle index currently in effect  */

  uint8                           default_scc
    /* Default SCC index calculated by hash function, given the maximum
       sleep cycle above                                                */
)
{
  uint8                           slot;
    /* working variable for conversion */

  uint8                           cycle;
    /* HDR Cycle (counting backwards from 5.12s in 426.7ms counts */

  uint8                           pccc;
    /* Preferred control channel cycle, based on 5.12s sleep  */
  
  uint8                           adjusted_pccc;
    /* Adjusted Preferred control channel cycle, based on the current
       max sleep cycle and default cycle                                */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                      "PCCC: page_slot = %d Max SCI = %d Def SCI = %d",
                       page_slot, 
                       max_hdr_sleep_cycle,
                       default_scc );

  /* For sleep period smaller than or equal to 426ms, return 0            */
  if ( max_hdr_sleep_cycle <= 6 )
  {
    return 0;
  }

  if ( max_hdr_sleep_cycle > 12 )
  {
    max_hdr_sleep_cycle = 12;
  }

  /* The following code computes the PCCC based on 5.12 sleep period      */

  page_slot &= (1<<2) * 16 - 1;
    /* Convert page slot to SCI=2 based */

  cycle = (HDRTS_SCC_PER_CCC - page_slot * 3 / 16) % HDRTS_SCC_PER_CCC;
    /* Convert page slot into cycle based position */

  slot = page_slot * 3 / 8;
    /* Convert page slot from sixteenths of a 1.28s frame into sixths */

  /* The following calculation attempts to maximize the distance between
     the 1x page slot at the HDR page slot.  With a slot cycle index
     of 0, if the page slot is in the 1st, 2nd or 3rd third of a
     1.28s slot, the corresponding HDR cycle (mod 3) must be 1, 0, and
     2.  With SCI=1 and SCI=2, the task is easier, but we still obey
     the SCI=0 restrictions */

  /* Perform a slightly different calculation if the page slot falls in
     the first half or the second half of a 426.7ms cycle (1/3 frame) */

  if ( (slot & 1) == 0 )
  {
    /* Page slot is in the first half of a 426.7ms period */

    pccc = ( cycle + 8 ) % HDRTS_SCC_PER_CCC;
  }
  else
  {
    /* Page slot is in the second half of a 426.7ms period */

    pccc = ( cycle + 4 ) % HDRTS_SCC_PER_CCC;
  }

  if ( max_hdr_sleep_cycle <= 9 )
  {
    /* Here it adjusts PCCC for 1.28s/2.56s/5.12s slot cycle            */
    adjusted_pccc = pccc % ( 12 / ( 1 << ( 9 - max_hdr_sleep_cycle ) ) );
  }
  else
  {
    /* Here it adjusts PCCC for other slot cycles                       */
    adjusted_pccc = pccc + ( default_scc / 12 ) * 12;
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Computed PCCC : old = %d adjusted = %d",
                                    pccc, adjusted_pccc );

  return adjusted_pccc;

} /* hdrsrch_get_preferred_control_channel_cycle( ) */



/* EJECT */
/*==========================================================================
                 NETWORK ACQUISISTION FUNCTIONS
==========================================================================*/


/*==========================================================================

FUNCTION HDRSRCH_ACQUIRE

DESCRIPTION
  Attempts to acquire a given channel

DEPENDENCIES
  None

PARAMETERS
  acq_params       -  structure to hold parameters of acquire command

RETURN VALUE
  None

SIDE EFFECTS
  RF is Tuned to given channel.
  acq_cb() will be called some indeterminate time in the future.
  system_lost_cb( ) may be called when the HDR Searcher is in any state
  to indicate the HDR system has been lost.

==========================================================================*/

void hdrsrch_acquire
(
   hdrsrch_acquire_params_type      *acq_params
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_acquire_params_type     *params;
    /* Convenience pointer */

  int                             i;
    /* Loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACQUIRE_CMD );
    /* Command HDRSRCH_TASK to acquire an HDR system */

  if ( cmd != NULL )
  {
    params = &cmd->params.acquire;
      /* Convenience pointer */

    params->channel         = acq_params->channel;
    params->mode            = acq_params->mode;
    params->acq_cb          = acq_params->acq_cb;
    params->system_lost_cb  = acq_params->system_lost_cb;
    params->usability_cb    = acq_params->usability_cb;
    params->num_of_pn       = acq_params->num_of_pn;
    params->reason          = acq_params->reason;
    for ( i=0; i < params->num_of_pn; i++ )
    {
      params->pn_list[i] = acq_params->pn_list[i];
    }

    params->overall_time_allowed_ms = acq_params->overall_time_allowed_ms;
      /* Copy parameters */

    if ( hdrsrch_is_lre_enabled() )
    {
      params->sync_suspend_cb       = acq_params->sync_suspend_cb;
        /* copy the sync substate callback */
    }

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_acquire( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SYNCHRONIZE

DESCRIPTION
  Once a system is acquired, and a synchronous control channel message is
  received, the HDR system must be "synchronized" to the system time that
  was specified in that message.  This function performs the required
  synchronization.

  The time specified in the SCC Message is for a point 160ms after
  the start of the SCC Capsule.

DEPENDENCIES
  hdrsrch_acquire( ) must have completed successfully.

PARAMETERS
  sync_time - Time specified in synchronous control channel (SCC) message.
  pilot_pn  - Pilot PN from SCC msg.
  half_slot - H/W "half slot" value at which SCC capsule started.
  return_timing_cb - callback to return CDMA system timing to upper layer.

RETURN VALUE
  None

SIDE EFFECTS
  A time discontinuity may occur.

==========================================================================*/

void hdrsrch_synchronize
(
  qword                           sync_time,
    /* Time from Synchronous Control Channel (SCC) message */

  sys_pilot_pn_type               pilot_pn,
    /* Pilot PN from SCC msg */

  uint16                          half_slot,
    /* The hardware half-slot value at the SCC message capsule start */

  hdrsrch_return_timing_cb_type   return_timing_cb
    /* Callback to return CDMA system time */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_synchronize_params_type *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_SYNCHRONIZE_CMD );
    /* Set Command Message Type */

  if ( cmd != NULL )
  {
    params = &cmd->params.synchronize;
      /* Convenience pointer */

    params->sync_time = ((uint64)qw_hi(sync_time) << 32) | qw_lo(sync_time);
    params->pilot_pn  = pilot_pn;
    params->half_slot = half_slot;
    params->return_timing_cb = return_timing_cb;
      /* Copy parameters to the command structure */

    hdrsrchutil_start_timer( &params->sync_cmd_delay );
      /* Count time since the sync cmd is received */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_synchronize( ) */

#ifdef FEATURE_HDR_BCMCS

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_UPDATE_BC_INFO

DESCRIPTION
  This function informs HDRSRCH about the current BCMCS status.

DEPENDENCIES
  None

PARAMETERS
  bcmcs_status - boolean

  TRUE means that we are monitoring BCMCS channels. 
  FALSE means that we stopped monitoring of BCMCS channels.

RETURN VALUE
  None

SIDE EFFECTS
  This command might cause a HDRSRCH state transition.

==========================================================================*/

void hdrsrch_update_bc_info 
( 
  hdrsrch_bcmcs_enum_type bcmcs_status
    /* Flag indicates whether BCMCS is on/off       */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_UPDATE_BC_INFO_CMD );
    /* Set Command Message Type */

  if ( cmd != NULL )
  {
    cmd->params.bc_info.bcmcs_status = bcmcs_status;
      /* Copy the BCMCS status information in the local command struct */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, " No command buffer available " );   
  }

} /* hdrsrch_update_bc_info */

#endif /* FEATURE_HDR_BCMCS */

/* EJECT */
/*==========================================================================
                          IDLE MODE FUNCTIONS
==========================================================================*/


/*==========================================================================

FUNCTION HDRSRCH_IDLE_MODE

DESCRIPTION
  Causes the Searcher to enter Idle Mode processing.
  When a handoff to a NSet pilot takes place, the given callback function
  will be called.

  RF will be taken out of Tx mode if it is in Tx mode.

  Note: The NSet pilot information will only be valid for the duration
  of the callback.  The callback must copy the data if it is needed after
  the callback returns.

DEPENDENCIES
  hdrsrch_synchronize( ) must have completed successfully.

PARAMETERS
  idle_ho_cb - Callback function for idle handoff reports
  suspend_cb - Callback function for idle suspended mode event reports.
  is_tca_processing - is tca processing in progress.

RETURN VALUE
  None

SIDE EFFECTS
  Idle state processing is started.

==========================================================================*/

void hdrsrch_idle_mode
(
  hdrsrch_idle_ho_cb_type         idle_ho_cb,
    /* Callback function for idle handoff reports */

  hdrsrch_suspend_cb_type         suspend_cb,
    /* Callback function for suspend/resume events (RF lock loss/regain) */

  boolean                         is_tca_processing
    /* If this IDLE mode is due to processing of TCA */

)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_idle_params_type        *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_IDLE_CMD );
    /* Command HDRSRCH_TASK to begin IDLE state processing */

  if ( cmd != NULL )
  {
    params = &cmd->params.idle;
      /* Convenience pointer */

    params->idle_ho_cb = idle_ho_cb;
    params->suspend_cb = suspend_cb;
    params->is_tca_processing = is_tca_processing;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_idle_mode( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_NSET_UPDATE

DESCRIPTION
  Update the neighbour list.

DEPENDENCIES
  In Idle (Monitor, Access or Sleep), or Connected mode

PARAMETERS
  num_nset_pilots  - Number of pilots in new NSET list
                     Must be <= HDRSRCH_NSET_MAX_SIZE
  nset_list        - Pointer to array of pilots
  struct_size      - Size (bytes) of the NSET structure, to allow passing a
                     pointer to an array of structures larger than
                     hdrsrch_nset_type
  update_type      - NSET info from a Broadcast or Directed message?
                     Directed info overrides Broadcast info
  num_channels     - Number of hash channels in the channel list 
  channel_list_ptr - Pointer to list of hash channels from the SP message 
  num_sectors      - Number of sectors in the overehad cache
  sector_list_ptr  - List of sectors currently in the overhead cache
  serving_pilot_gid - Group id of the pilot from which the nset update is rx'ed
  hdr_subtype       - HDR protocol subtype

RETURN VALUE
  None

SIDE EFFECTS
  Sets the neighbour pilot list.  The search schedule will be updated to
  reflect the new neighbour pilot list.

==========================================================================*/

void hdrsrch_nset_update
(
  uint8                           num_nset_pilots,
    /* Number of pilots in the neighbour set */

  const hdrsrch_nset_pilot_type * nset_list,
    /* The neighbour set pilots */

  unsigned int                    struct_size,
    /* Size of the neighbour set pilot structure.  Allows an array of
       items larger than sizeof(hdrsrch_nset_pilot_type) to be used. */

  hdrsrch_nset_update_enum_type   update_type,
    /* NSET information from a BROADCAST or DIRECTED message? */

  uint8                           num_channels,
    /* Number of hash channels in the channel list */

  const sys_channel_type          *channel_list_ptr,
    /* List of hash channels from the BROADCAST message*/
 
  uint8                           num_avoided_channels,
    /* Number of avoided channels */

  const sys_channel_type          *avoided_channel_list_ptr,
    /* List of avoided channels */

  uint8                           num_sectors,
    /* Number of sectors in the overehad cache */
 
  sys_link_type                   *sector_list_ptr 
    /* List of sectors currently in the overhead cache */

#ifdef FEATURE_HDR_REVB
  , hdrsrch_pilot_gid_type        serving_pilot_gid,
    /* The pilot group id of the sector from which the neighbor info
       is received. */

  sys_active_prot_e_type          hdr_subtype
    /* HDR Protocol subtype */

#endif /* FEATURE_HDR_REVB */  
)
{

  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_nset_update_params_type *params;
    /* Convenience pointer */

  int i;
    /* Loop variables */

  int n;
    /* Store num of cached sects */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_NSET_UPDATE_CMD );
    /* Command HDRSRCH_TASK to update the NSET */

  if ( cmd != NULL )
  {
    params = &cmd->params.nset_update;
      /* Convenience pointer */

    params->num_pilots = MIN( num_nset_pilots, HDRSRCH_NSET_MAX_SIZE );
      /* Enforce limit on NSet size */

    for( i = 0; i < params->num_pilots; i++ )
    {
      params->nset_pilots[i] = *nset_list;
        /* Copy all parameters from one structure to the other */

      NEXT_STRUCT(const hdrsrch_nset_pilot_type, nset_list, struct_size);
        /* Point to next structure */
    } /* for */

    params->update_type  = update_type;
      /* Record information source (broadcast or directed msg) */

    params->num_channels = MIN( num_channels, HDRSRCH_HASH_CHAN_MAX_SIZE );
      /* Enforce limit on hash channel list size */

    for( i = 0; i < params->num_channels; i++ )
    {
      params->channel_list[i] = channel_list_ptr[i];
        /* Copy all parameters from one structure to the other */
    } /* for */

    if ( num_avoided_channels > HDRSRCH_AVOIDED_CHAN_MAX_SIZE ) 
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "The avoided channel number(%d) is too large",
                      num_avoided_channels );

    }
    params->num_avoided_channels = MIN( num_avoided_channels, 
                                        HDRSRCH_AVOIDED_CHAN_MAX_SIZE );
      /* Enforce limit on avoided channel list size */

    for( i = 0; i < params->num_avoided_channels; i++ )
    {
      params->avoided_channel_list[i] = avoided_channel_list_ptr[i];
        /* Copy all parameters from one structure to the other */
    } /* for */

    n = MIN( num_sectors, HDRSRCH_MAX_CACHE_RECORDS );
      /* Enforce limit on cached records */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Nset update: cached records %d: %d\n", 
                    num_sectors, 
                    n );

    for ( i=0; i<n; i++ )
    {
      params->cached_sects[i] = *sector_list_ptr;
        /* Copy all parameters from one structure to the other */

      sector_list_ptr++;
        /* Point to next structure */
    }

    params->num_sects = n;
      /* Record the number of cached records */

#ifdef FEATURE_HDR_REVB
    params->serving_pilot_gid = serving_pilot_gid;
      /* Passing the group id of the serving pilot */

    params->hdr_subtype       = hdr_subtype;
      /* Passing hdr subtype.  This is used to determine Rev A/B mode. */
#endif /* FEATURE_HDR_REVB */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_nset_update( ) */


/*==========================================================================

FUNCTION HDRSRCH_IDLE_CHANNEL_CHANGE_AND_NSET_UPDATE

DESCRIPTION
  Change to a new HDR channel and update the nset.  This should be called when the HDR AT
  hashes to a new channel and update nset pilots.

DEPENDENCIES
  Idle mode

PARAMETERS
  idle_cc_params      - params for channel change.
  nset_update_params  - params for nset update.

RETURN VALUE
  None

SIDE EFFECTS
  RF is tuned to a new channel.

==========================================================================*/

void hdrsrch_idle_channel_change_and_nset_update
(
   hdrsrch_idle_channel_change_type *idle_cc_params,
     /* Idle channel change params*/

   hdrsrch_nset_update_type         *nset_update_params
     /* Nset update params*/
)
{

  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_channel_change_and_nset_update_params_type *params;
    /* Convenience pointer */

  int i;
    /* Loop variables */

  int n;
    /* Store num of cached sects */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD );
    /* Change the current channel */

  if ( cmd != NULL )
  {
    params = &cmd->params.channel_change_and_nset_update;
      /* Convenience pointer */

    params->link            = *(idle_cc_params->link_ptr);
      /* Copy new channel information into command */

    params->chan_change_rpt = idle_cc_params->chan_change_rpt_cb;
      /* Copy report function for channel change */

    params->num_pilots = MIN( nset_update_params->num_nset_pilots, 
                              HDRSRCH_NSET_MAX_SIZE );
      /* Enforce limit on NSet size */

    for( i = 0; i < params->num_pilots; i++ )
    {
      params->nset_pilots[i] = *(nset_update_params->nset_list);
        /* Copy all parameters from one structure to the other */

      NEXT_STRUCT(hdrsrch_nset_pilot_type, 
                  nset_update_params->nset_list, 
                  nset_update_params->struct_size);
        /* Point to next structure */
    } /* for */

    params->update_type  = nset_update_params->update_type;
      /* Record information source (broadcast or directed msg) */

    params->num_channels = MIN( nset_update_params->num_channels, 
                                HDRSRCH_HASH_CHAN_MAX_SIZE );
      /* Enforce limit on hash channel list size */

    for( i = 0; i < params->num_channels; i++ )
    {
      params->channel_list[i] = nset_update_params->channel_list_ptr[i];
        /* Copy all parameters from one structure to the other */
    } /* for */

    if ( nset_update_params->num_avoided_channels > HDRSRCH_AVOIDED_CHAN_MAX_SIZE ) 
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "The avoided channel number(%d) is too large",
                      nset_update_params->num_avoided_channels );

    }
    params->num_avoided_channels = MIN( nset_update_params->num_avoided_channels, 
                                        HDRSRCH_AVOIDED_CHAN_MAX_SIZE );
      /* Enforce limit on avoided channel list size */

    for( i = 0; i < params->num_avoided_channels; i++ )
    {
      params->avoided_channel_list[i] = nset_update_params->avoided_channel_list_ptr[i];
        /* Copy all parameters from one structure to the other */
    } /* for */

    n = MIN( nset_update_params->num_sectors, HDRSRCH_MAX_CACHE_RECORDS );
      /* Enforce limit on cached records */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Nset update: cached records %d: %d\n", 
                    nset_update_params->num_sectors, 
                    n );

    for ( i=0; i<n; i++ )
    {
      params->cached_sects[i] = *(nset_update_params->sector_list_ptr);
        /* Copy all parameters from one structure to the other */

      nset_update_params->sector_list_ptr++;
        /* Point to next structure */
    }

    params->num_sects = n;
      /* Record the number of cached records */

#ifdef FEATURE_HDR_REVB
    params->serving_pilot_gid = nset_update_params->serving_pilot_gid;
      /* Passing the group id of the serving pilot */

    params->hdr_subtype       = nset_update_params->hdr_subtype;
      /* Passing hdr subtype.  This is used to determine Rev A/B mode. */
#endif /* FEATURE_HDR_REVB */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_idle_channel_change_and_nset_update */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_IDLE_CHANNEL_CHANGE

DESCRIPTION
  Change to a new HDR channel.  This should be called when the HDR AT
  hashes to a new channel.

DEPENDENCIES
  Idle mode

PARAMETERS
  channel             - The new channel.
  chan_change_rpt_cb  - Function to call when channel change is complete.

RETURN VALUE
  None

SIDE EFFECTS
  RF is tuned to a new channel.

==========================================================================*/

void hdrsrch_idle_channel_change
(
  const sys_link_type *               link_ptr,
    /* New HDR Channel, and an initial pilot on that channel. */

  hdrsrch_chan_change_rpt_cb_type     chan_change_rpt_cb
    /* Callback function for idle channel change reports */
)
{
  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_channel_change_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_CHANNEL_CHANGE_CMD );
    /* Change the current channel */

  if ( cmd != NULL )
  {
    params = &cmd->params.channel_change;
      /* Convenience pointer */

    params->link            = *link_ptr;
      /* Copy new channel information into command */

    params->chan_change_rpt = chan_change_rpt_cb;
      /* Copy report function for channel change */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_channel_change( ) */



/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_SLEEP

DESCRIPTION
  Enter Sleep.

  The access terminal will wakeup in time to demodulate its next assigned
  synchronous control channel capsule.

  The access terminal may be forced to wakeup by calling:
    hdrsrch_acquire( )
    hdrsrch_access_mode( )
    hdrsrch_traffic_channel_assignment( )

DEPENDENCIES
  For long-duration sleeps (ie, 5.12 seconds) the Control Channel Hash
  value must be set.  If this value has not been set, then the sleep
  duration will be no longer than 426.7ms.

PARAMETERS
  hdr_slot_cycle             - Indicates when to wakeup.  Not used if
                               cc_start_slot_valid is TRUE.
  hdr_neg_slot_cycle         - What is the current negotiated SCI which can be
                               different from what the protocols are asking for
                               this particular sleep cycle.
  wakeup_cb                  - Callback function to indicate when UT awakes.
  qpch_enabled               - Whether QPCH is enabled or not.                                           
  cc_start_slot_valid        - Flag whether cc_start_slot parameter is
                               valid or not.  If TRUE, use cc_start_slot
                               instead SCI to calculate  wake_slot.
  cc_start_slot              - CC start in slots.  This field is used if
                               cc_start_slot_valid is TRUE.
  ornl_cycle                 - Other RAT Neighbor List message has its own cycle 

RETURN VALUE
  None

SIDE EFFECTS
  Regardless of the reason for the wakeup, the wakeup callback will be
  called.

  The HDR Searcher may remain awake for part of the "sleep" period, so it
  may perform background searches (off-frequency &/or off-system pilots).

  5ms tick may be disabled - Rex timers may not expire at the "proper"
  time.

==========================================================================*/

void hdrsrch_sleep
(
  uint8                           hdr_slot_cycle,
    /* e.g. 6: 426ms, 9: 5.12s, 12: 40.96s
       Note: Not used if cc_start_slot_valid is TRUE. */

  uint8                           hdr_neg_slot_cycle,
    /* e.g. 6: 426ms, 9: 5.12s, 12: 40.96s */

  boolean                         qpch_enabled,
    /* Whether QPCH is enabled or not */

  hdrsrch_wakeup_cb_type          wakeup_cb,
    /* Callback to indicate AT has woke up */

  boolean                         cc_start_slot_valid,
    /* Flag used to indicate whether param cc_start_slot is valid */

  uint16                          cc_start_slot
    /* CC start in slots. This field is valid if cc_start_slot_valid is TRUE */

#ifdef FEATURE_HDR_TO_LTE
  ,uint8                          ornl_cycle
    /* Other RAT (Radio Access Technologies) Neighbor List message has its own
       cycle.  Since value 0 is valid, this field needs to be set to
       HDRSRCH_ORNL_CYCLE_INACTIVE if ornl_cycle is not active. */
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  ,boolean                         session_is_open,
    /* whether EVDO session is open */

  boolean                         ovhd_is_updated
    /* whether OVHD info is updated */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_sleep_params_type       *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_SLEEP_CMD );
    /* Command HDRSRCH_TASK to enter sleep until the next assigned SCC */

  if ( cmd != NULL )
  {
    params = &cmd->params.sleep;
      /* Convenience pointer */

    params->hdr_slot_cycle                = hdr_slot_cycle;
    params->hdr_neg_slot_cycle            = hdr_neg_slot_cycle;
    params->qpch_enabled                  = qpch_enabled;
    params->wakeup_cb                     = wakeup_cb;
    params->cc_start_slot_valid           = cc_start_slot_valid;
    params->cc_start_slot                 = cc_start_slot;

#ifdef FEATURE_HDR_TO_LTE
    params->ornl_cycle                    = ornl_cycle;
      /* Other RAT (Radio Access Technologies) Neighbor List message has its own
         cycle.  Since value 0 is valid, this field needs to be set to
         HDRSRCH_ORNL_CYCLE_INACTIVE if ornl_cycle is not active. */
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    params->session_is_open               = session_is_open;
    params->ovhd_is_updated               = ovhd_is_updated;
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
} /* hdrsrch_sleep( ) */



/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_REENABLE_IDLE_HANDOFFS

DESCRIPTION
  This function re-enables Idle Handoffs.

  Idle handoffs are disabled at the start of a Synchronous Control Capsule.
  They must be re-enabled when:
    - The control channel capsule is completely received.
    - No preamble is detected for the next packet of the SCC.
    - A CRC error occurs during the SCC.

  A sleep command implies "Ok to Handoff"

DEPENDENCIES
  Must only be sent when in IDLE state.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

==========================================================================*/

void hdrsrch_reenable_idle_handoffs( void )
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_REENABLE_HANDOFFS_CMD );
    /* Re-enable idle handoffs */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_reenable_idle_handoffs */


/* EJECT */
/*==========================================================================
                          ACCESS MODE FUNCTIONS
==========================================================================*/


/*==========================================================================

FUNCTION HDRSRCH_ACCESS_MODE

DESCRIPTION
  Enter Access Mode processing.  Automatic handoffs are suspended.
  RF will enter Tx mode.

  Note: May be called while in Sleep - will cause a wakeup (surprise!)

DEPENDENCIES
  Must be in Idle-Monitor or Idle-Sleep states.

PARAMETERS
  suspend_ho_cb - Callback function to indicate when handoffs have
                  been suspended.
  request_ho_cb - Callback function to indicate that the searcher
                  has found a better pilot to be on.  The upper
                  layer may call hdrsrch_access_handoff( ) at a
                  convinenent time.
  thresh_0p5_db - Access handoff threshold, in 0.5 db units

RETURN VALUE
  None

SIDE EFFECTS
  Idle handoffs will be suspended.

==========================================================================*/

void hdrsrch_access_mode
(
  hdrsrch_suspend_ho_cb_type      suspend_ho_cb,
    /* Callback function indicating idle handoff have been suspended. */

  hdrsrch_request_ho_cb_type      request_ho_cb,
    /* Callback function indicating a handoff is desired in access mode */

  int                             thresh_0p5_db
    /* Access Handoff threshold, in 0.5 dB */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_access_params_type      *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACCESS_CMD );
    /* Command HDRSRCH_TASK to begin ACCESS state processing */

  if ( cmd != NULL )
  {
    params = &cmd->params.access;
      /* Convenience pointer */

    params->suspend_ho_cb = suspend_ho_cb;
    params->request_ho_cb = request_ho_cb;
    params->thresh_0p5_db = thresh_0p5_db;
      /* Copy parameters */


    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_access_mode( ) */


#ifdef FEATURE_HDR_HYBRID_ACCESS
/*==========================================================================

FUNCTION HDRSRCH_ACMAC_READY

DESCRIPTION
  This function notifies HDR SRCH that ACMAC is ready.

DEPENDENCIES
  None

PARAMETERS
  ta_time_cb - callback function for tune away time.
  stop_cb    - callback function to stop access.
  
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_acmac_ready
(
  hdrsrch_acmac_ta_time_cb_type      ta_time_cb,
    /* Callback function indicating tune away time */

  hdrsrch_acmac_stop_cb_type         stop_cb
    /* Callback function to stop ACMAC */

)
{
  hdrsrch_cmd_type                   *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACMAC_READY_CMD );

  if ( cmd != NULL )
  {
    cmd->params.acmac_ready_params.ta_time_cb = ta_time_cb;
    cmd->params.acmac_ready_params.stop_cb    = stop_cb;

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_acmac_ready( ) */


/*==========================================================================

FUNCTION HDRSRCH_ACMAC_TA_TIME_OK

DESCRIPTION
  This function notifies HDR SRCH whether tune away time is ok.

DEPENDENCIES
  None

PARAMETERS
  ta_time_ok  - tune away time is ok or not.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_acmac_ta_time_ok
(
  boolean                         ta_time_ok
    /* Whether tune away time is ok */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACMAC_TA_TIME_OK_CMD );

  if ( cmd != NULL )
  {
    cmd->params.ta_time_ok_params.ta_time_ok = ta_time_ok;
    
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_acmac_ta_time_ok( ) */


/*==========================================================================

FUNCTION HDRSRCH_ACMAC_DISABLE_TA

DESCRIPTION
  This function notifies HDR SRCH that ACACK has been received and 
  disables tune away for the duration specificed by no_ta_duration ms.
  
DEPENDENCIES
  None

PARAMETERS
  no_ta_duration - The duration that tune away should be disabled.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_acmac_disable_ta
(
  uint16                     no_ta_duration
    /* The duration that tune away should be disabled */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACMAC_DISABLE_TA_CMD );

  if ( cmd != NULL )
  {
    cmd->params.acmac_disable_ta_params.no_ta_duration = no_ta_duration;
    
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_acmac_disable_ta( ) */
#endif /* FEATURE_HDR_HYBRID_ACCESS */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ACCESS_HANDOFF

DESCRIPTION
  Command a handoff to occur while in access mode.

  This should be called after the request_ho_cb( ) specified in the
  hdrsrch_access_mode( ) command has been called, and the current access
  probe has been suspended.

DEPENDENCIES
  Access mode, with no access probes underway.

PARAMETERS
  new_pilot   - Specifies the pilot to hand off to.  This may be NULL, to
                allow Searcher to choose the best pilot to handoff to, as
                conditions may have changed since it issued the
                request_ho_cb( ) indication.

  access_ho_rpt - Function to call to report completion of access handoff.

RETURN VALUE
  None

SIDE EFFECTS
  The Access Mode access_ho_cb will be issued after the handoff is
  complete.

==========================================================================*/

void hdrsrch_access_handoff
(
  const sys_link_type *               new_pilot,
    /* Pilot to handoff to */

  hdrsrch_access_ho_cb_type           access_ho_rpt
    /* Report of handoff complete */
)
{
  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_access_handoff_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ACCESS_HANDOFF_CMD );
    /* Command HDRSRCH_TASK to begin an ACCESS Handoff */

  if ( cmd != NULL )
  {
    params = &cmd->params.access_handoff;
      /* Convenience pointer */

    if ( new_pilot != NULL )
    {
      params->new_pilot     = *new_pilot; /* Note: Structure copy */
      params->use_new_pilot = TRUE;
        /* We are being told which pilot to handoff to */
    }
    else
    {
      params->use_new_pilot = FALSE;
        /* Handoff to the best pilot available */
    }

    params->access_ho_cb  = access_ho_rpt;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_access_handoff( ) */


/* EJECT */
/*==========================================================================
                       CONNECT (Traffic) MODE FUNCTIONS
==========================================================================*/

#ifndef FEATURE_HDR_REVB
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_CHAN_AND_ASET_ASSIGN

DESCRIPTION
  Enters Connected Mode, or Effect a Hard Handoff to a new frequency
  while in Connected Mode.

  Sets the channel and active set pilots for the Connected (Traffic) state.
  Used when a traffic channel is initially assigned, and when a handoff
  to a different frequency is required.

  RF will enter Tx mode.

DEPENDENCIES
  Idle or Connected Mode

PARAMETERS
  channel           - HDR Channel for the traffic channel
                      (NULL implies current ASET channel)
  tc_tune_away      - Whether HDR is willing to allow tuneaways.
  num_aset_pilots   - Number of Active Set pilots
                      Must be <= HDRSRCH_ASET_MAX_SIZE
  aset_list         - The Active Set pilots
  struct_size       - Size (bytes) of each aset_list structure, allowing an
                      array of structures larger than hdrsrch_aset_pilot_type
                      to be used.
  traffic_channel_assigned_rpt
                    - Callback used to indicate RF has tuned to req'd
                      frequency, and Search has assigned fingers to the
                      Active Set pilots.

  suspend_cb        - Callback for TC suspended mode event reports.

RETURN VALUE
  None

SIDE EFFECTS
  May cause Search to wakeup from sleep.

  hdrsrch_enable_pilot_reports( NULL ) is implied if transitioning from
  idle to connected state.  To enable pilot reports, call
  hdrsrch_enable_pilot_reports( ).

  hdrsrch_enable_pilot_reports( ) is unchanged if effecting a hard h/o
  to a new frequency.

==========================================================================*/

void hdrsrch_chan_and_aset_assign
(
  const sys_channel_type *        channel,
    /* HDR Channel to operate on (NULL -> Current ASet channel) */

  hdrsrch_traffic_tune_away_enum_type  tc_tune_away,
    /* What other systems we are allowed to tune away from HDR to visit */

  uint8                           num_aset_pilots,
    /* Number of pilots in the active set */

  const hdrsrch_aset_pilot_type * aset_list,
    /* The active set pilots */

  unsigned int                    struct_size,
    /* Size of the active set pilot structure.  Allows an array of
       items larger than sizeof(hdrsrch_aset_pilot_type) to be used. */

  hdrsrch_tca_cb_type             traffic_channel_assigned_rpt,
    /* Function to call when the RF has tuned to the required channel,
       and the search has set fingers on some subset of the active set.
       It is safe to turn on the reverse traffic channel */

  hdrsrch_suspend_cb_type         suspend_cb
    /* Callback function for suspend/resume events (RF lock loss/regain) */
)
{
  hdrsrch_cmd_type                  *cmd;
    /* Command structure */

  hdrsrch_tc_assignment_params_type *params;
    /* Convenience pointer */

  int i, n;
    /* Loop indexes */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_TC_ASSIGNMENT_CMD );
    /* Command HDRSRCH_TASK to enter CONNECTED (traffic) mode */

  if ( cmd != NULL )
  {
    params = &cmd->params.tc_assign;
      /* Convenience pointer */


    /* Copy parameters */

    if ( channel != NULL )
    {
      params->channel = *channel;
        /* Copy Channel we are assigned to */
    }
    else
    {
      params->channel = hdrsrch.aset_channel;
        /* Copy the current ASet channel */
    }

    n = MIN( num_aset_pilots, HDRSRCH_ASET_MAX_SIZE );
      /* Limit active set size to what we can support */

    for(i=0; i<n; i++)
    {
      params->aset_pilots[i] = *aset_list;
        /* Copy all parameters from one structure to the other */

      NEXT_STRUCT( const hdrsrch_aset_pilot_type, aset_list, struct_size );
        /* Point to next structure */
    }

    params->num_pilots = n;
      /* Record the number of neighbour pilots */

    params->tc_tune_away = tc_tune_away;
      /* Save our tune away restriction */

    params->tc_assign_cb = traffic_channel_assigned_rpt;
    params->suspend_cb   = suspend_cb;
      /* And save report and suspend callback functions */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_chan_and_aset_assign( ) */
#endif /* !FEATURE_HDR_REVB */


#ifdef FEATURE_HDR_REVB
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_CHAN_AND_ASET_ASSIGN_REVB

DESCRIPTION
  RF will enter Tx mode.

DEPENDENCIES
  Idle or Connected Mode

PARAMETERS
  hdr_subtype          - The subtype of the HDR protocol.
  tc_tune_away         - Whether HDR is willing to allow tuneaways.
  num_subactive_set    - Number of subActive Sets 
                         Must be <= HDRSRCH_MAX_NUM_SUBASET
  subasetive_set_list  - The subActive Set list.
  cc_subactive_set_idx - The suActive Set index of the subActive Set
                         carring the control channel.
  traffic_channel_assigned_rpt
                       - Callback used to indicate RF has tuned to 
                         req'd frequency, and Search has assigned 
                         fingers to the Active Set pilots.

  suspend_cb           - Callback for TC suspended mode event reports.

  schdgrp_rpt_cb       - Callback for scheduler group changes.

RETURN VALUE
  None

SIDE EFFECTS
  May cause Search to wakeup from sleep.

  hdrsrch_enable_pilot_reports( NULL ) is implied if transitioning from
  idle to connected state.  To enable pilot reports, call
  hdrsrch_enable_pilot_reports( ).

  hdrsrch_enable_pilot_reports( ) is unchanged if effecting a hard h/o
  to a new frequency.

==========================================================================*/

void hdrsrch_chan_and_aset_assign_revb
(
  sys_active_prot_e_type               hdr_subtype,
    /* HDR Protocol subtype */

  hdrsrch_traffic_tune_away_enum_type  tc_tune_away,
    /* Whether HDR is permitted to tune away to the suspended state for 
       another technology.  This is set to FALSE for session negotiation.*/

  uint8                                num_subactive_sets,
    /* Number of subactive asets to add to active set */

  const hdrsrch_subactive_set_type     *subactive_set_list,
    /* Pointer to an array of subactive aset structures */

  uint8                                cc_subactive_set_idx,
    /* The index of the subactive set which contains the control channel */

  uint8                                frame_offset,
    /* Frame offset */

  hdrsrch_tca_cb_type                  traffic_channel_assigned_rpt,
    /* Function to call when the RF has tuned to the required channel,and 
       the search has tried to assign fingers on some subset of the active set.  
       It is safe to turn on the reverse traffic channel */

  hdrsrch_suspend_cb_type              suspend_cb,
    /* Callback function for suspend/resume events (RF lock loss/regain). */

  hdrsrch_schdgrp_rpt_cb_type          schdgrp_rpt_cb
    /* Callback function for added and deleted scheduler groups. */
)
{
  hdrsrch_cmd_type                  *cmd;
    /* Command structure */

  hdrsrch_tc_assignment_params_type *params;
    /* Convinience pointer */

  uint8 i, j, n;
    /* Loop indexes */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_TC_ASSIGNMENT_CMD );
    /* Command HDRSRCH_TASK to enter CONNECTED (traffic) mode */

  if ( cmd != NULL )
  {
    params = &cmd->params.tc_assign;
      /* Convenience pointer */

     /* Sanity check the subaset size and cc subaset index */

    if( hdr_subtype != SYS_ACTIVE_PROT_HDR_RELB )
    {
        /* Only one carrier is allowed for revA and rev0 */
      ASSERT(( num_subactive_sets == 1 ) &&
             ( cc_subactive_set_idx == 0 ));
        /* Subactive set is marked as cc carrier */
    }
    else
    {
       /* Max supported Sub Active Sets */
      ASSERT(( num_subactive_sets <= HDRSRCH_MAX_NUM_SUBASETS ) &&
             ( cc_subactive_set_idx < num_subactive_sets ));
       /* Only one subaset is marked as cc carrier */
    }

    params->hdr_subtype = hdr_subtype;
      /* Protocol subtype */

    params->frame_offset = frame_offset;
      /* Frame offset */

    /* Copy parameters */
    for( j=0; j<num_subactive_sets; j++ )
    {

      n = MIN( subactive_set_list[j].num_aset_pilots, 
               HDRSRCH_MAX_SUBASET_SIZE );
        /* Limit active set size to what we can support */

      /* Copy subaset parameters */

      params->subaset_list[j].num_aset_pilots = n;
      params->subaset_list[j].channel         = subactive_set_list[j].channel;
      params->subaset_list[j].systype         = subactive_set_list[j].systype;
      params->subaset_list[j].demod_idx       = subactive_set_list[j].demod_idx;
      params->subaset_list[j].not_reportable  =
                                           subactive_set_list[j].not_reportable;

      for( i=0; i<n; i++ )
      {
        /* Copy sector parameters */

        params->subaset_list[j].aset_list[i] 
                                         = subactive_set_list[j].aset_list[i];
      }

    }

    params->num_subaset = num_subactive_sets;
      /* Record the number of neighbour pilots */

    params->tc_tune_away = tc_tune_away;
      /* Save our tune away restriction */

    params->cc_subactive_set_idx = cc_subactive_set_idx;
      /* The idx of the subaset which carris the control channel */

    params->tc_assign_cb   = traffic_channel_assigned_rpt;
    params->suspend_cb     = suspend_cb;
    params->schdgrp_rpt_cb = schdgrp_rpt_cb;
      /* And save report and suspend callback functions */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
} /* hdrsrch_chan_and_aset_assign_revb( ) */
#endif /* FEATURE_HDR_REVB */


#ifndef FEATURE_HDR_REVB
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ASET_UPDATE

DESCRIPTION
  Sets the active set for the Connected (Traffic) state.

DEPENDENCIES
  hdrsrch_traffic_channel_assignment( ) must be called before
  hdrsrch_aset_update( ) calls are allowed.

PARAMETERS
  num_aset_pilots   - Number of Active Set pilots
  aset_list         - The Active Set pilots
  struct_size       - Size (bytes) of each aset_list structure, allowing an
                      array of structures larger than hdrsrch_aset_pilot_type
                      to be used.

RETURN VALUE
  None

SIDE EFFECTS
  May cause an immediate reschedule of searches.

==========================================================================*/

void hdrsrch_aset_update
(
  uint8                                   num_aset_pilots,
    /* Number of pilots in the active set */

  const hdrsrch_aset_pilot_type *         aset_list,
    /* The active set pilots */

  unsigned int                            struct_size
    /* Size of the active set pilot structure.  Allows an array of
       items larger than sizeof(hdr_aset_pilot_type) to be used. */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_aset_update_params_type *params;
    /* Convenience pointer */

  int i, n;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ASET_UPDATE_CMD );
    /* Command HDRSRCH_TASK to update the active pilot set */

  if ( cmd != NULL )
  {
    params = &cmd->params.aset_update;
      /* Convenience pointer */


    /* Copy parameters */

    n = MIN( num_aset_pilots, HDRSRCH_ASET_MAX_SIZE );
      /* Limit active set size to what we can support */

    for(i=0; i<n; i++)
    {
      params->aset_pilots[i] = *aset_list;
        /* Copy all parameters from one structure to the other */

      NEXT_STRUCT( const hdrsrch_aset_pilot_type, aset_list, struct_size );
        /* Point to next structure */
    }

    params->num_pilots = n;
      /* Record the number of neighbour pilots */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_aset_update( ) */
#endif /* !FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVB
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ASET_UPDATE_INTERNAL

DESCRIPTION
  Sets the active set for the Connected (Traffic) state.

DEPENDENCIES
  hdrsrch_chan_and_aset_assign_revb( ) must be called before
  hdrsrch_aset_update_internal( ) calls are allowed.

PARAMETERS
  hdr_subtype          - Subtype of the HDR protocol.
  num_subactive_sets   - Number of subActive Set pilots
  subactive_set_list   - The subActive Set list
  cc_subactive_set_idx - the index to the subaset which carries CC channel
  update_complete_cb   - Callback to inform ASET has been updated

RETURN VALUE
  None

SIDE EFFECTS
  May cause an immediate reschedule of searches.

==========================================================================*/
LOCAL void hdrsrch_aset_update_internal
(
  sys_active_prot_e_type             hdr_subtype,
    /* HDR Protocol subtype */

  uint8                              num_subactive_sets,
    /* Number of pilots in the active set */

  const hdrsrch_subactive_set_type * subactive_set_list,
    /* The active set pilots */

  uint8                              cc_subactive_set_idx, 
    /* The index to the subaset which carries CC channel */

  uint8                              frame_offset
    /* Frame offset */
#ifdef FEATURE_HDR_REVC
  , hdrsrch_aset_update_cb_type      update_complete_cb
#endif /* FEATURE_HDR_REVC */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_aset_update_params_type *params;
    /* Convenience pointer */

  int                             i, j, n, m;
    /* Loop index and temporary count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ASET_UPDATE_CMD );
    /* Command HDRSRCH_TASK to update the active pilot set */

  if ( cmd != NULL )
  {
    params = &cmd->params.aset_update;
      /* Convenience pointer */

    /* Copy parameters */

    if( ( hdr_subtype != SYS_ACTIVE_PROT_HDR_REL0 ) &&
        ( hdr_subtype != SYS_ACTIVE_PROT_HDR_RELA ) &&
        ( hdr_subtype != SYS_ACTIVE_PROT_HDR_RELB ) )
    {
      /* Error fatal the incorrect RUP subtype */

      ERR_FATAL("Incorrect RUP subtype in TCA processing", 0, 0, 0 );
    }
    else
    {
      /* Sanity check the subaset size and cc subaset index */

      if( ( hdr_subtype == SYS_ACTIVE_PROT_HDR_REL0 ) ||
          ( hdr_subtype == SYS_ACTIVE_PROT_HDR_RELA ) )
      {
      /* Only one carrier is allowed for revA0 */
       ASSERT(( num_subactive_sets == 1 ) &&
              ( cc_subactive_set_idx == 0 ));
      /* Subactive set is marked as cc carrier */
      }
      else
      {
        ASSERT( cc_subactive_set_idx < num_subactive_sets );
         /* Only one subaset is marked as cc carrier */
      }
    }

    params->cc_subactive_set_idx = cc_subactive_set_idx;
      /* Save the CC carrier SubASET index */

    params->hdr_subtype = hdr_subtype;
      /* HDR subtype tells rev0, A or revB */

    params->frame_offset = frame_offset;
      /* Frame offset */

    m = MIN( num_subactive_sets, HDRSRCH_MAX_NUM_SUBASETS );

    params->num_subasets = m;
      /* Number of subactive sets */

    for( j=0; j<m; j++ )
    {

      n = MIN( subactive_set_list[j].num_aset_pilots, HDRSRCH_ASET_MAX_SIZE );
        /* Limit active set size to what we can support */

      for(i=0; i<n; i++)
      {
        params->subaset_list[j].aset_list[i] =
                                           subactive_set_list[j].aset_list[i];
        /* Copy all parameters from one structure to the parameter struct */

      }
      params->subaset_list[j].num_aset_pilots = n;
        /* Copy the number of neighbour pilots */
 
      params->subaset_list[j].channel         = subactive_set_list[j].channel;
        /* Copy channel info for this subaset */

      params->subaset_list[j].systype         = subactive_set_list[j].systype;
        /* System type */

      params->subaset_list[j].demod_idx       = subactive_set_list[j].demod_idx;
        /* Demod index for this subaset */

      params->subaset_list[j].not_reportable  = 
                                         subactive_set_list[j].not_reportable;
        /* Reportable attribute */
    }

#ifdef FEATURE_HDR_REVC
    params->update_complete_cb = update_complete_cb;
#endif /* FEATURE_HDR_REVC */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
} /* hdrsrch_aset_update_internal( ) */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ASET_UPDATE_REVB

DESCRIPTION
  Sets the active set for the Connected (Traffic) state.

DEPENDENCIES
  hdrsrch_chan_and_aset_assign_revb( ) must be called before
  hdrsrch_aset_update_revb( ) calls are allowed.

PARAMETERS
  rup_subtype          - Subtype of the RUP protocol.
  num_subactive_sets   - Number of subActive Set pilots
  subactive_set_list   - The subActive Set list
  cc_subactive_set_idx - the index to the subaset which carries CC channel

RETURN VALUE
  None

SIDE EFFECTS
  May cause an immediate reschedule of searches.

==========================================================================*/
void hdrsrch_aset_update_revb
(
  sys_active_prot_e_type             hdr_subtype,
    /* HDR Protocol subtype */

  uint8                              num_subactive_sets,
    /* Number of pilots in the active set */

  const hdrsrch_subactive_set_type * subactive_set_list,
    /* The active set pilots */

  uint8                              cc_subactive_set_idx, 
    /* The index to the subaset which carries CC channel */

  uint8                              frame_offset
    /* Frame offset */
)
{
  hdrsrch_aset_update_internal(hdr_subtype,
                               num_subactive_sets,
                               subactive_set_list,
                               cc_subactive_set_idx,
                               frame_offset
#ifdef FEATURE_HDR_REVC
                               , NULL
#endif /* FEATURE_HDR_REVC */
                               );
}

#ifdef FEATURE_HDR_REVC
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ASET_UPDATE_REVC

DESCRIPTION
  Sets the active set for the Connected (Traffic) state.

DEPENDENCIES
  hdrsrch_chan_and_aset_assign_revb( ) must be called before
  hdrsrch_aset_update_revb( ) calls are allowed.

PARAMETERS
  rup_subtype          - Subtype of the RUP protocol.
  num_subactive_sets   - Number of subActive Set pilots
  subactive_set_list   - The subActive Set list
  cc_subactive_set_idx - the index to the subaset which carries CC channel
  update_complete_cb   - Callback to inform ASET has been updated

RETURN VALUE
  None

SIDE EFFECTS
  May cause an immediate reschedule of searches.

==========================================================================*/
void hdrsrch_aset_update_revc
(
  sys_active_prot_e_type             hdr_subtype,
    /* HDR Protocol subtype */

  uint8                              num_subactive_sets,
    /* Number of pilots in the active set */

  const hdrsrch_subactive_set_type * subactive_set_list,
    /* The active set pilots */

  uint8                              cc_subactive_set_idx, 
    /* The index to the subaset which carries CC channel */

  uint8                              frame_offset,
    /* Frame offset */

  hdrsrch_aset_update_cb_type        update_complete_cb
)
{
  hdrsrch_aset_update_internal(hdr_subtype,
                               num_subactive_sets,
                               subactive_set_list,
                               cc_subactive_set_idx,
                               frame_offset,
                               update_complete_cb);
}
#endif /* FEATURE_HDR_REVC */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_SCHDGRP_RPT

DESCRIPTION
  Get the scheduler group report.

DEPENDENCIES

PARAMETERS
  schdgrp_rpt  - Pointor to the report struct.

RETURN VALUE
  None

SIDE EFFECTS

==========================================================================*/

void hdrsrch_get_schdgrp_rpt
(
  hdrsrch_schdgrp_rpt_type *schdgrp_rpt
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchschdgrp_get_schdgrp_rpt( schdgrp_rpt );
    /* Fill the scheduler group report */
}


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_ASSIGN_DEMOD_IDXS_TO_SUBASETS

DESCRIPTION
  The function returns the demod idx assignment based on the current incoming
  TCA. 

DEPENDENCIES
  Connected State

PARAMETERS
  num_subactive_sets  - number of the subactive sets.
  subactive_set_list  - ptr to the subactive set lists.
  demod_idx_assigned  - ptr to the assignement struct.

RETURN VALUE
  Return the demod_idx assignment of each of the subActive Set according to the 
  order of the subActive Set of the incoming TCA.

  The return boolean indicates if RF reconfiguration is required.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrch_assign_demod_idxes_to_subasets
(
  uint8 num_subactive_sets, 
    /* Number of subactive sets */

  const hdrsrch_subactive_set_type * subactive_set_list,
    /* The link list of subactive sets */

  hdrsrch_demod_idx_assignment_type * demod_idx_assigned
    /* The demod index assignments */
)
{
  uint8                   i, chan_cnt; 
    /* Loop index and channel count */

  sys_channel_type        chan_list[ HDRSRCH_MAX_DEMOD_CARRIERS_CNT ];
    /* Channel List */

  hdrsrch_demod_idx_type  demod_list[ HDRSRCH_MAX_DEMOD_CARRIERS_CNT ];
    /* Demod list */

  boolean                 tune_needed = FALSE;
    /* Return value indicating if an RF tune is needed */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* If tune is needed, the demod index assigment starts from demod0; by 
     doing this, we can keep the demod index usage as a compact list, 
     i.e. demod0 is always in use, which avoids a 'hole' in the demod carrier
     assignments. */


  if( ( HDRSRCH_DEMOD_ASSIGN_OVERRIDE_ENABLE == 0 ) ||
      ( hdrscp_get_current_subtype( HDRHAI_RTC_MAC_PROTOCOL ) <= 
                                                      HDRSCP_SUB1_RMAC ) )
  {
    hdrsrchutil_assign_demod_idxes_to_subasets( num_subactive_sets,
                                                subactive_set_list,
                                                demod_idx_assigned );
      /* If a soft handoff, assign the demod index based on the persistence of
         the current subactive set. */
  }
  else
  {
    /* Demod override mode is enabled; override the demod index assignment based
       on the NV settings. Please note this settings will be effect for all
       the revA and revB TCA processings.  */
    for( i=0; i<num_subactive_sets; i++ )
    {
      demod_idx_assigned[i].demod_idx = 
       ( i+HDRSRCH_DEMOD_ASSIGN_OVERRIDE_INDEX )%HDRSRCH_MAX_DEMOD_CARRIERS_CNT;
    }
  }

  chan_cnt = num_subactive_sets;

  for( i=0; i<num_subactive_sets; i++ )
  {
    chan_list[i] = subactive_set_list[i].channel;
    demod_list[i] = demod_idx_assigned[i].demod_idx;
  }

  chan_cnt = MIN ( chan_cnt , HDRSRCH_MAX_DEMOD_CARRIERS_CNT );
    /* KW fix :  limit the chan_cnt to maximum value */

  /* Check if RF tuning is needed */
  tune_needed = !hdrsrchutil_match_aset_channels( chan_cnt,
                                                  chan_list,
                                                  demod_list );

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,"======== Demod Index Assignment ========" );

  for( i=0; i<num_subactive_sets; i++ )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,"subactive_set_idx[%d] demod_idx[%d]",
                  i, demod_idx_assigned[i].demod_idx );
  }
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,"TCA tuning needed:%d", tune_needed);

  return tune_needed;
} /* void hdrsrch_assign_demod_idxes_to_subasets */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_GET_FL_CARRIER_INFO

DESCRIPTION
  The function returns the information about the forward link carrier.

DEPENDENCIES
  None

PARAMETERS
  subaset_idx - the sub aset idx associated with the carrier requested
  fl_carrier_info - The pointer to the info structure to fill.

RETURN VALUE
  TRUE if the carrier identified Sub ASET index is in use and FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrch_get_fl_carrier_info
(
  hdrsrch_subaset_idx_type           subaset_idx,
    /* The sub active set index */

  hdrsrch_fl_carrier_info_type*       fl_carrier_info
    /* The pointer to the table to fill */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  return  hdrsrchset_get_fl_carrier_info( subaset_idx, fl_carrier_info );

} /* boolean hdrsrch_get_fl_carrier_info */

#endif /* FEATURE_HDR_REVB */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ENABLE_PILOT_REPORTS

DESCRIPTION
  Enables or Disables generation of pilot reports.

DEPENDENCIES
  Connected Mode

PARAMETERS
  pilot_rpt_cb  - Function to call when a pilot report is required.
                  NULL to disable pilot reports.

RETURN VALUE
  None

SIDE EFFECTS
  May immediately cause pilot_rpt_cb( ) to be called.

==========================================================================*/

void hdrsrch_enable_pilot_reports
(
  hdrsrch_pilot_rpt_cb_type pilot_rpt_cb
    /* Pilot report indication callback, or NULL to disable */
)
{
  hdrsrch_cmd_type  *cmd;
    /* Command structure */

  hdrsrch_pilot_report_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_PILOT_REPORT_CMD );
    /* Command HDRSRCH_TASK to generate pilot report indications */

  if ( cmd != NULL )
  {
    params = &cmd->params.pilot_report;
      /* Convenience pointer */

    params->pilot_rpt_cb  = pilot_rpt_cb;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_enable_pilot_reports */



/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_PILOT_REPORT_RESET

DESCRIPTION
  Reset the "Reported" flag for "interesting" pilots in the Active and
  Candidate sets ( C >> A, or A's Drop Timer expired)

DEPENDENCIES
  Connected Mode

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May immediately cause pilot_rpt_cb( ) to be called, if new pilot events
  have occurred since the last call to hdrsrch_get_pilot_report( ).

==========================================================================*/

void hdrsrch_pilot_report_reset( void )
{
  hdrsrch_cmd_type  *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_PILOT_REPORT_RESET_CMD );
  /* Command HDRSRCH_TASK to reset reported status of reporting pilots */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_pilot_report_reset( ) */



/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_PILOT_REPORT

DESCRIPTION
  Retrieves a snapshot of pilot information.

DEPENDENCIES
  Connected Mode

PARAMETERS
  max_pilots    - Maximum number of pilots which will be retrieved.
  pilots        - Array of length max_pilots, in which the pilot report
                  information will be stored.

RETURN VALUE
  None

SIDE EFFECTS
  Clears the "ReportPending" status for the reported sectors.

==========================================================================*/

int hdrsrch_get_pilot_report
(
  int                             max_pilots,
    /* Maximum # of pilot entries which may be placed in "pilots" array */

  hdrsrch_pilot_rpt_type *        pilots
    /* Array to store pilot information in */
)
{
  hdrsrch_sect_struct_type *      sect;
    /* sector type for reference sector */

#ifndef FEATURE_HDR_REVB
  sys_pilot_pn_type               ref_pn;
    /* reference PN offset */
#endif /* FEATURE_HDR_REVB */

  int                             n;
    /* Counter/Index into report structure */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  rex_enter_crit_sect(&hdrsrch.crit_sect);
    /* Protect the set structure */

  /*---------------------------------*
   * Determine the Reference Pilot.  *
   *---------------------------------*/

#ifdef FEATURE_HDR_REVB
  sect = hdrsrchutil_get_ref_sector_ptr();
    /* Get the reference sector from the firmware */
#else
  ref_pn = (sys_pilot_pn_type) HDRMSTR_GET_REF_PN();
    /* Get the Pilot PN offset which MSTR is tracking */

  sect = hdrsrchset_find_sect( ref_pn, hdrsrch.aset_channel );
    /* Get the sector pointer for the reference PN */
#endif /* FEATURE_HDR_REVB */
      

  /* Make sure the ref sect is not NULL. Can the ref pilot be moved
     to RSET and thus be NULL? TBD */
#ifdef FEATURE_HDR_REVB
  if ( sect == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "No MSTR tracking sector" );

    sect = hdrsrchset.subaset[0].aset[0];
      /* Default to first active set pilot as reference pilot */
  }
#else
  if ( sect == NULL )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "RefPN=%d not in ASET [%d]",
                    ref_pn,
                    hdrsrchset.aset_cnt );

    sect = hdrsrchset.aset[0];
      /* Default to first active set pilot as reference pilot */
  }
#endif /* FEATURE_HDR_REVB */


  n = hdrsrchset_get_pilot_report(max_pilots, pilots, sect);
    /* Get the pilot report, using "sect" as the reference pilot */

  rex_leave_crit_sect(&hdrsrch.crit_sect);
    /* Ok, done! */

  return n;

} /* hdrsrch_get_pilot_report( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_RUP_REQUEST

DESCRIPTION
  Posts a RUP request command to searcher.

DEPENDENCIES
  None
  
PARAMETERS
  route_update_request   - Parameters of the route update message.
  pilot_rpt_cb           - Pilot report indication callback

RETURN VALUE
  None

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_rup_request
(
  const hdrsrch_rup_req_params_type   *route_update_request,
    /* pointer to RUP request params structure */
    
  hdrsrch_pilot_rpt_cb_type           pilot_rpt_cb
    /* Pilot report indication callback, or NULL to disable */
)
{
  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_rup_req_params_type         *params;
    /* Convenience pointer */

  uint8                               sector_count;
    /* number of sectos in message */

  int                                 i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_RUP_REQ_CMD );
    /* Command HDRSRCH_TASK to update the active pilot set */

  if ( cmd != NULL )
  {
    params = &cmd->params.rup_req;
      /* Convenience pointer */

    /* Copy parameters */
    params->channel_included = route_update_request->channel_included;
    params->channel.chan_num = route_update_request->channel.chan_num;
    params->channel.band = route_update_request->channel.band;
    params->system_type = route_update_request->system_type;
    params->sector_count = route_update_request->sector_count;
    sector_count = route_update_request->sector_count;
    params->pilot_rpt_cb = pilot_rpt_cb;

    if ( sector_count != 0)
    {
      params->window_size_included = route_update_request->window_size_included;
      params->window_offset_included = route_update_request->window_offset_included;
    }
    else
    {
      params->window_size_included = 0;
      params->window_offset_included = 0;    
    }
        
    /* copy individual sector parameters if sector count is non zero */
    if ( sector_count != 0)
    {
      for ( i = 0 ; i < sector_count ; i++ )
      {
        params->sector_info[i].pilot_pn = route_update_request->sector_info[i].pilot_pn;
      }
      
      if ( params->window_offset_included == TRUE )
      {
        for ( i = 0 ; i < sector_count ; i++ )
        {
          params->sector_info[i].window_size = route_update_request->sector_info[i].window_size;
        }
      }
        
      if ( params->window_size_included == TRUE )
      {
        for ( i = 0 ; i < sector_count ; i++ )
        {
          params->sector_info[i].window_offset = route_update_request->sector_info[i].window_offset;        
        }
      }  
      
    }      
      
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_rup_request( ) */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_LINK_INFO

DESCRIPTION
  Retrieves the stored information on a given link ID

DEPENDENCIES

PARAMETERS
  link_id       - The link ID in the DSM item of a received message
  link_info_ptr - Location where to write the link info corresponding to
                  the given link ID.

RETURN VALUE
  E_SUCCESS if the link ID is found, E_FAILURE if it is not found.

SIDE EFFECTS
  None

==========================================================================*/

errno_enum_type hdrsrch_get_link_info
(
  hdrsrch_link_id_type            link_id,
    /* Link id information is being requested for */

  sys_link_type                   *link_info_ptr
    /* Location to store the pilot info about the specified link id */
)
{
  hdrsrch_sect_struct_type        *sect;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sect = hdrsrchset_find_link_id( link_id );
    /* Find the sector with the given link id */

  if ( sect != NULL )
  {
    link_info_ptr->pilot_pn         = sect->pn_offset;
    link_info_ptr->channel          = sect->chan;

    return E_SUCCESS;
      /* Link id was found - link information has been provided */
  }
  else
  {
    link_info_ptr->pilot_pn         = HDR_NO_PN_OFFSET;
      /* No reference found */

    return E_FAILURE;
      /* Link id was not found */
  }

} /* hdrsrch_get_link_info( ) */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_AT_SET_DRC

DESCRIPTION
  Sets the DRC rate/mode from the AT command. These values are set only
  after ~30msec after the traffic channel has been initiated.

DEPENDENCIES
  None.

PARAMETERS
  drc_param = 0 to 12 for setting override mode with constant DRC rate
  drc_param = 0xf     for setting variable rate DRC

RETURN VALUE
  None

SIDE EFFECTS
  Changes the DRC rate or mode

==========================================================================*/

void hdrsrch_at_set_drc
(
#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type demod_idx,
#endif /* FEATURE_HDR_REVB */

  uint8       drc_param
    /* parameter to be used for setting the DRC from AT command */
)
{
  hdrsrch_cmd_type  *cmd;
    /* Command structure */

  hdrsrch_at_set_drc_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Received DRC AT command %x", drc_param );

  cmd = hdrsrchcmd_new( HDRSRCH_AT_SET_DRC_CMD );
    /* Command HDRSRCH_TASK to set DRC */

  if ( cmd != NULL )
  {
    params = &cmd->params.at_set_drc;
      /* Convenience pointer */

    params->demod_idx = demod_idx;
    params->drc_param  = drc_param;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_at_set_drc( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_MAC_INDEX

DESCRIPTION
  Retrieves the MAC index for the passed Link ID

DEPENDENCIES

PARAMETERS
  link_id       - The link ID in the DSM item of a received message

RETURN VALUE

  MAC_INDEX : 8 bit MAC Index for the given link ID (4 through 63)
  -1        : No MAC Index associated with the link ID

SIDE EFFECTS
  None

==========================================================================*/

int8 hdrsrch_get_mac_index
(
  hdrsrch_link_id_type            link_id
    /* Link id information is being requested for */
)
{

  hdrsrch_sect_struct_type        *sect;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sect = hdrsrchset_find_link_id( link_id );
    /* Find the sector with the given link id */

  if ( sect && sect->set == HDR_ASET )
  {
    return ( int8 ) sect->aset.mac_idx;
  }
  else
  {
    return -1;
     /* Link ID not found so no MAC index associated with it */
  }

} /* hdrsrch_get_mac_index() */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_RL_MAC_INDEX

DESCRIPTION
  Retrieves the reverse link MAC index for the passed Link ID

DEPENDENCIES

PARAMETERS
  link_id       - The link ID in the DSM item of a received message

RETURN VALUE

  MAC_INDEX : 16 bit MAC Index for the given link ID (4 through 63)
  -1        : No MAC Index associated with the link ID

SIDE EFFECTS
  None

==========================================================================*/

int16 hdrsrch_get_rl_mac_index
(
  hdrsrch_link_id_type            link_id
    /* Link id information is being requested for */
)
{

  hdrsrch_sect_struct_type        *sect;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sect = hdrsrchset_find_link_id( link_id );
    /* Find the sector with the given link id */

  if ( sect && sect->set == HDR_ASET )
  {
    return ( int8 ) sect->aset.rl_mac_idx;
  }
  else
  {
    return -1;
     /* Link ID not found so no MAC index associated with it */
  }
} /* hdrsrch_get_mac_index() */



/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_GET_DRC_COVER

DESCRIPTION
  Retrieves the DRC cover for the passed Link ID

  //TODO/TBD: Consolidate get_mac_index, get_link_info & get_drc_cover into a
  //single function.

DEPENDENCIES
  This function searches all the pilot sets (A, C & N) for the given link
  id and returns the DRC cover. It is recommended that this function be
  called only if the linkID is changed in the dsm header.

PARAMETERS
  link_id       - The link ID in the DSM item of a received message

RETURN VALUE

  DRC Cover : 4 bit Drc Index for the given link ID
  -1        : No DRC Cover associated with the link ID

SIDE EFFECTS
  None

==========================================================================*/

int8 hdrsrch_get_drc_cover
(
  hdrsrch_link_id_type            link_id
    /* Link id information is being requested for */
)
{

  hdrsrch_sect_struct_type        *sect;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sect = hdrsrchset_find_link_id( link_id );
    /* Find the sector with the given link id */

  if ( sect )
  {
    return ( int8 ) sect->aset.drc_cover;
  }
  else
  {
    return -1;
     /* Link ID not found so no DRC Cover associated with it */
  }

} /* hdrsrch_get_drc_cover() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_REGISTER_ROLL_CALLBACK

DESCRIPTION  : This function registers PN ROLL callback function which will
               be called after receiving the PN ROLL ISR.

               The callback functions are executed in the PN ROLL ISR
               context so it is very important to have these callback
               functions quick and simple. Note that there is a limitation
               on maximum number of callbacks that are supported by the
               search task. Presently this number is set to "4"

               It is recommended that these callback functions just set a
               Rex Signal to the concerned task.

DEPENDENCIES : None

INPUT        : Callback function pointer

RETURN VALUE : None

SIDE EFFECTS : May begin periodic PN roll ISR

=========================================================================*/

void hdrsrch_register_roll_callback
(
  hdrsrch_roll_cb_type       cb
    /* callback function to be registered */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrts_register_roll_callback( cb );
    /* Pass to HDRTS to do the real work */

} /* hdrsrch_register_roll_callback() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_DEREGISTER_ROLL_CALLBACK

DESCRIPTION  : This function unregisters PN ROLL callback function

DEPENDENCIES : None

INPUT        : Callback function pointer

RETURN VALUE : None

SIDE EFFECTS : May end periodic PN roll ISR

=========================================================================*/

void hdrsrch_deregister_roll_callback
(
  hdrsrch_roll_cb_type       cb
    /* callback function to be registered */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrts_deregister_roll_callback( cb );
    /* Pass to HDRTS to do the real work */

} /* hdrsrch_deregister_roll_callback() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_ADVISE_UNLOCK_RF

DESCRIPTION  : This function is a hint from the upper layers that HDRSRCH
               should release its lock on the RF resources, if it has
               the lock.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_advise_unlock_rf( void )
{
  hdrsrch_cmd_type  *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ADVISE_UNLOCK_RF_CMD );
  /* Command HDRSRCH_TASK to release the RF lock, if it is held */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_advise_unlock_rf() */



/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_TC_ACK_RECEIVED

DESCRIPTION  : This function tells that TC ack is received by protocols

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_tc_ack_received( void )
{
  hdrsrch_cmd_type  *cmd;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_TC_ACK_RECEIVED_CMD );
    /* Command HDRSRCH_TASK to exit to INACTIVE state */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_tc_ack_received */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_SET_USER_DIVERSITY_PREF

DESCRIPTION  : This function sets the user diversity preference. HDRMC reads
               the diversity preference from the NV and calls this fucntion
               to set it.

               This is called during hdrmc powerup initialization

DEPENDENCIES : MDSP should already be downloaded and running by this time

INPUT        : Whether diversity is on and which bands it is on for.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_set_user_diversity_pref
(
  uint8       div_ctrl,
    /* User diversity control read from NV */

  uint32      div_band_mask
    /* HDR diversity enable band class mask read from NV */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Save the div info */
  hdrsrch.div_ctrl = div_ctrl;
  hdrsrch.div_band_mask = div_band_mask;

#ifdef FEATURE_HDR_THERMAL_BACKOFF

  /* Initialize diversity pref vote to true.
     So that when thermal backoff/runtime user wants to switch it
     on, it is able to do so*/
  hdrsrch.div_ctrl_pref_vote = 0;

#endif /* FEATURE_HDR_THERMAL_BACKOFF */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
  hdrsrch.idle_state_div.access_div_enabled =
    HDRSRCH_IS_ACCESS_DIVERSITY_ALLOWED ( div_ctrl );

  hdrsrch.idle_state_div.idle_div_enabled =
    HDRSRCH_IS_IDLE_DIVERSITY_ALLOWED ( div_ctrl );

   HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "HDR IDLE DIV: Access Div on = %d, Idle Div = %d ",
                  hdrsrch.idle_state_div.access_div_enabled,
                  hdrsrch.idle_state_div.idle_div_enabled );
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  hdrsrch.acq_div.general_div_enabled = (boolean)( div_ctrl & 0x01 );
  hdrsrch.acq_div.acq_div_runtime_ctrl = TRUE;
    /* Set runtime ctrl default to TRUE */
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "HDR ACQ DIV: General Div on = %d runtime_acq_div_ctrl=%d",
                  hdrsrch.acq_div.general_div_enabled,
                  hdrsrch.acq_div.acq_div_runtime_ctrl );

#ifdef FEATURE_HDR_RUMI
  hdrsrch.acq_div.acq_div_nv_enable = hdrsrch.idle_state_div.idle_div_enabled;
    /* For RUMI, set NV enable */
#endif /* FEATURE_HDR_RUMI */

#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */
} /* hdrsrch_set_user_diversity_pref */


/*=========================================================================

FUNCTION     : HDRSRCH_SET_USER_EQUALIZER_PREF

DESCRIPTION  : This function sets the user equalizer/Rake preference. HDRMC 
               reads the equalizer preference from the NV and calls this 
               function to set it.
               This is called during hdrmc powerup initialization

DEPENDENCIES : MDSP should already be downloaded and running by this time

INPUT        : User equalizer control value read from NV

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_set_user_equalizer_pref
(
  uint8 eq_ctrl
    /* user equalizer control read from NV */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrcheq_set_user_eq_pref( eq_ctrl );
    /* set user preference for Equalizer/Rake demod setting */

} /* hdrsrcheq_set_user_eq_pref */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_SET_USER_DEBUG_MASK

DESCRIPTION  : This function sets the user searcher debug mask. HDRMC reads
               the debug mask from the NV and calls this function
               to set it.

               This is called during hdrmc powerup initialization.

DEPENDENCIES : None

INPUT        : user HDR L1 searcher debug mask read from NV

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_set_user_debug_mask
(
  uint64      debug_mask,
    /* user HDR L1 searcher debug mask read from NV */

  boolean     mask_valid
    /* Flag to indicate that NV item has been successfully read */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_SRCH_DBG_MASK_ACCESS_HYBRID_DISABLED 
  hdrsrch_dbg_msk = 0x0080;
    /* Initialize the debug mask to disable access hybrid */
#else
  hdrsrch_dbg_msk = 0x0000;
    /* Initialize to the default debug mask */
#endif /* FEATURE_HDR_SRCH_DBG_MASK_ACCESS_HYBRID_DISABLED */

  if ( mask_valid == TRUE )
  {
    hdrsrch_dbg_msk = debug_mask;
      /* Set user searcher_debug_mask */
  }

} /* hdrsrch_set_user_debug_mask */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_SET_USER_DEBUG_ARRAY

DESCRIPTION  : This function sets the user searcher debug array. HDRMC reads
               the debug array from the NV and calls this function
               to set it.  Since the array type and size are not exported
               byte nv_items, check if the size of the array is correct.
               If the input is larger, it is ok to copy less.

               This is called during hdrmc powerup initialization.

DEPENDENCIES : None

INPUT        : user HDR searcher debug array read from NV
               flag to indicate NV item was successfully read
               size of the input debug array in number of uint8 bytes

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

/*lint -esym(18, hdrsrch_set_user_debug_array) */
/* Note: LINT error 18 is a false positive and not a real error. */

void hdrsrch_set_user_debug_array
(
  const uint8 *debug_ptr,
    /* user HDR searcher debug array read from NV */

  boolean     array_valid,
    /* Flag to indicate that NV item has been succesfully read */

  size_t      debug_array_size
    /* Size of input debug array in uint8 bytes */
)
{

  size_t bytes_to_copy;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( array_valid == TRUE )
  {
    bytes_to_copy = MIN( debug_array_size, sizeof(hdrsrch_dbg_array) );

    memset( hdrsrch_dbg_array, 0, sizeof(hdrsrch_dbg_array) );
      /* Initialize to the default hdr srch debug array */

#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy( hdrsrch_dbg_array, sizeof(hdrsrch_dbg_array), 
             debug_ptr,
             bytes_to_copy );
#else
    memcpy( hdrsrch_dbg_array,
            debug_ptr,
            bytes_to_copy );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Set user searcher_debug_array */

    hdrmdsp_set_mmse_cfg( 
       HDRSRCH_DBG_NV_MMSE_RAB,
       HDRSRCH_DBG_NV_MMSE_ARQ
       );
  }
  else
  {
    /* BOLT/JOLOKIA/THOR modem: enable MMSE if NV is not programmed */
    hdrmdsp_set_mmse_cfg( 
       TRUE,
       TRUE
       );
  }

} /* hdrsrch_set_user_debug_array */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_HDR_RSSI

DESCRIPTION  : This function returns the RSSI estimate for HDR to be used
               by the User Interface to indicate quality of HDR service.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

byte hdrsrch_get_hdr_rssi
( 
  void 
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchutil_get_hdr_rssi();

} /* hdrsrch_get_hdr_rssi */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_ECIO_AND_IO

DESCRIPTION
  Returns HDR Ec/Io and Io to the caller. It returns the Io estimate made by 
  rx AGC loops in the DSP and the Ec/Io estimate found by the searcher.

  Ec/Io range: from 0 to 63 (Returned in -.5 dB units)
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

void hdrsrch_get_ecio_and_io( int16* ecio, int* io )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil_get_ecio_and_io( ecio, io );
    /* Get the raw io and energy values. */

  *ecio = hdrsrchset_eng_to_ecio( *ecio ); 
    /* Convert the raw energy value to ec/io quantized to -0.5 dB. */

} /* hdrsrch_get_ecio_and_io */


/* <EJECT> */

/*===========================================================================

FUNCTION HDRSRCH_GET_SINR_AND_RXAGC

DESCRIPTION
  This function returns SINR indicator and Rx AGC to the caller. SINR 
  indicator is from 0 to 8. Level 8 represents highest SINR and level 0 
  represents lowest SINR.
  
  SINR indicator range: from 0 to 8
  RxAgc range         : from -106 dBm to -21 dBm 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrch_get_sinr_and_rxagc 
(
  byte    *sinr, 
    /* pointer to SINR indicator value */

  int     *rxagc
    /* pointer to RxAgc value   */    
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil_get_sinr_and_rxagc( sinr, rxagc );

} /* hdrsrch_get_sinr_and_rxagc  */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_GET_SINR_AND_RXAGC_DB

DESCRIPTION
  This function returns SINR indicator and Rx AGC to the caller. SINR 
  indicator is from 0 to 75 in 0.5 dB units from -18dB to -19dB, e.g.
  0 = -9dB, 7=-5.5dB, 35 = +9dB.
  
  SINR indicator range: from 0 to 37 in 0.5 dB units from -18dB to -19 dB
  RxAgc range         : from -106 dBm to -21 dBm 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrch_get_sinr_and_rxagc_db
(
  byte    *sinr, 
    /* pointer to SINR indicator value */

  int     *rxagc
    /* pointer to RxAgc value   */    
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil_get_sinr_and_rxagc_db( sinr, rxagc );

} /* hdrsrch_get_sinr_and_rxagc_db  */


/*===========================================================================

FUNCTION HDRSRCH_GET_FTD_DATA

DESCRIPTION
  This function returns FTD data to the caller including:
  
  EcIo: from -31 to 0 dB;
  C/I: from -27 to 18 dB;
  rxAGC: from -115 dBm to -13 dBm;
  txAGC: from -64 dBm to 42 dBm;
  MAC index: Mac index of the cc monitoring pilot;
  ASET channel info.

DEPENDENCIES
  None

PARAMETERS
  ftd_data - Pointer of the FTD data;

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

===========================================================================*/
extern uint16  hdrsrchutil_data_ind_sinr;
  /* Filtered best ASP SINR used to decide the data indicator */

void hdrsrch_get_ftd_data
( 
  hdrlog_ftd_l1_stats_type*     ftd_data
    /* Pointer of FTD L1 info */
)
{
  int16* ecio = &ftd_data->ecio; 
    /* pointer to EcIo value */

  boolean diversity;
   /* Unused - just needed for calling API hdrsrchrf_get_rx_power */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil_get_ecio_and_io( ecio, (int *) (&ftd_data->rx_dbm) );
    /* Fill in ecio and rx_dbm */

  *ecio = - (int16)( hdrsrchset_eng_to_ecio( *ecio ) / 2 );
    /* Convert ecio with 1dB accuracy */

  hdrsrchrf_get_rx_power( (int *) &ftd_data->rx0_dbm, (int *) &ftd_data->rx1_dbm, &diversity );
      /* Get rx power in dB units. We may read the stored value of RX AGC
         here if the DSP clock has been off */

  ftd_data->c2i = hdrsrchset_sinr_linear_to_db( hdrsrchutil_data_ind_sinr );
    /* Get the sinr value in dB unit */
  
  ftd_data->tx_dbm = hdrsrchutil_get_txagc( );
    /* Get the txAGC */

  if  ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
  {
    ftd_data->mac_index = (uint16) ( hdrsrch_get_mac_index( HDRSRCH_MON_LINK_ID ) );
      /* Get the mac index of the cc monitoring pilot */
  }
  else
  {
    ftd_data->mac_index = (uint16) ( -1 );
      /* Return invalid mac index for non-traffic state */
  }

#ifndef FEATURE_HDR_REVB
  if ( &hdrsrch.aset_channel != NULL )
  {
    ftd_data->channel = hdrsrch.aset_channel;
      /* Get the ASET channel info */
  }
#else
  if ( hdrsrch.aset_channel_cnt != 0 )
  {
    ftd_data->channel = hdrsrch.aset_channel[0];
      /* Get the ASET channel[0] info */
  }
#endif /* FEATURE_HDR_REVB */

} /* hdrsrch_get_ftd_data */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_ASP_PN_MAPPING

DESCRIPTION  : This function returns the PN offsets and ASP indices of all 
               the active set pilots, in an array passed by the caller.

DEPENDENCIES : None

INPUT        : A pointer to an array of type hdrsrch_asp_to_pn_info_type.

RETURN VALUE : Number of ASET pilots, from 0 to 6.
               PN and ASP info will be updated in the array passed by caller.
               
SIDE EFFECTS : None

=========================================================================*/

uint8 hdrsrch_get_asp_pn_mapping
( 
  hdrsrch_asp_to_pn_info_type * aset_pn
)
{
  uint8       i;
    /* Loop index     */

#ifdef FEATURE_HDR_REVB
  uint8       j, n=0;
    /* Loop index     */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB 
  for( i =0; i < hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      aset_pn[n].asp_idx      = hdrsrchset.subaset[i].aset[j]->aset.asp_idx;
      aset_pn[n++].pn_offset  = hdrsrchset.subaset[i].aset[j]->pn_offset;
        /* Copy ASP index and pn offset into the passed array   */
    }
  }

  return n;
    /* return the number of ASET pilots. TBD if the function is called
       in the traffic, CP need be sure the size of the aset_pn. */
#else
  for (i =0; i < hdrsrchset.aset_cnt; i++ )
  {
    aset_pn[i].asp_idx    = hdrsrchset.aset[i]->aset.asp_idx;
    aset_pn[i].pn_offset  = hdrsrchset.aset[i]->pn_offset;
      /* Copy ASP index and pn offset into the passed array   */
  }

  return hdrsrchset.aset_cnt;
    /* return the number of ASET pilots */
#endif /* FEATURE_HDR_REVB */
} /* hdrsrch_get_asp_pn_mapping */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_PN_ECIO

DESCRIPTION  : This function returns the PN offsets and EcIo of all the 
               active set pilots, in an array passed by the caller. The
               range of EcIo returned is from 0 to 63, in -0.5db unit.
               

DEPENDENCIES : None

INPUT        : A pointer to an array of type hdrsrch_pn_ecio_info_type.

RETURN VALUE : Number of ASET pilots, from 0 to 6. 
               PN and EcIo info will be updated in the array passed by caller.

SIDE EFFECTS : None

=========================================================================*/
 
uint8 hdrsrch_get_pn_ecio
( 
  hdrsrch_pn_ecio_info_type* pn_ecio
)
{
  uint8       i;
    /* Loop index     */

#ifdef FEATURE_HDR_REVB
  uint8       j, n=0;
    /* Loop index     */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

 #ifdef FEATURE_HDR_REVB 
  /* TBD: the caller needs be aware of the size change of the revB ASET */
  for( i =0; i < hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      pn_ecio[n].pn_offset = hdrsrchset.subaset[i].aset[j]->pn_offset;
      pn_ecio[n++].ecio = hdrsrchset_eng_to_ecio(
                                 hdrsrchset.subaset[i].aset[j]->total_eng );
        /* Copy ASP index and pn offset into the passed array   */
    }
  }

  return n;
    /* return the number of ASET pilots */
#else
 
  for (i =0; i < hdrsrchset.aset_cnt; i++ )
  {
    pn_ecio[i].pn_offset = hdrsrchset.aset[i]->pn_offset;
      /* Copy PN offset value   */

    pn_ecio[i].ecio = hdrsrchset_eng_to_ecio( hdrsrchset.aset[i]->total_eng );
      /* Convert pilot strength into DB scale ( in -0.5db unit ) */
  }

  return hdrsrchset.aset_cnt;
    /* return the number of ASET pilots */
#endif /* FEATURE_HDR_REVB */
} /* hdrsrch_get_pn_ecio */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_DATA_INDICATOR

DESCRIPTION  : This function returns the data level indicator. The data rate
               that the AT can receive is divided into 9 levels, 0 to 8. Level
               8 indicates highest data rate and level 0 indicates almost no
               data rate.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : Data activity indicator between 0 to 8.

SIDE EFFECTS : None

=========================================================================*/

byte hdrsrch_get_data_indicator( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchutil_get_data_indicator();

} /* hdrsrch_get_hdr_rssi */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_COLOR_CODE_UPDATE

DESCRIPTION  : This function informs the searcher of the color code for the
               current link

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_color_code_update
(
  sys_link_type link_info,
    /* link which needs to be updated */

  uint8         color_code
    /* color code value for the link */
)
{
  hdrsrch_sect_struct_type   *sect;
    /* sector to which the link ID belongs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();

  sect = hdrsrchset_find_sect( link_info.pilot_pn, link_info.channel );
    /* find the sector with matching pn offset and channel */

  if ( sect != NULL )
  {
    sect->color_code = (hdrsrch_color_code_type) color_code;
      /* update color code for the sector. */
  }

  REX_ENABLE_PREMPTION();


  if ( sect != NULL )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                    "Update PN=%d, Chan=%d with ColorCode=%d",
                    sect->pn_offset, sect->chan.chan_num, color_code );
  }
  else
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR, 
                    "Can't find PN=%d, Chan=%d for ColorCode=%d update",
                    link_info.pilot_pn, link_info.channel.chan_num, 
                    color_code );
  }

} /* hdrsrch_color_code_update*/


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_SET_TC_MODE

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

void hdrsrch_set_tc_mode
( 
  hdrsrch_tc_mode_enum_type  tc_mode
    /* Traffic mode that HDR should transition to. */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_tc_mode_params_type *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch.tc_mode = tc_mode;
    /* Save the new traffic mode. We must make sure that we save this reliably
       as the queue might be purged.  Then ask searcher to take action on
       this via the state machine if need be. */

  cmd = hdrsrchcmd_new( HDRSRCH_TC_MODE_CMD );
    /* Allocate new command for switching the HDR mode. */

  if ( cmd != NULL )
  {
    params = &cmd->params.tc_mode;
      /* Convenience pointer */

    params->mode = tc_mode;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_set_tc_mode */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_GET_TC_MODE

DESCRIPTION  : This function gets the current traffic mode

DEPENDENCIES : None

PARAMETERS   : None
 
RETURN VALUE : 

   HDRSRCH_DEFAULT_TC_MODE   - Standard HDR traffic connection. 

   HDRSRCH_S1XHDR_HP_TC_MODE - High Priority Traffic Mode where
                               HDR preempts 1x paging onto 
                               chain 1 and preempts acquisition.

SIDE EFFECTS : None

=========================================================================*/

hdrsrch_tc_mode_enum_type hdrsrch_get_tc_mode ( void )
{

  return hdrsrch.tc_mode;

} /* hdrsrch_get_tc_mode*/

#ifdef FEATURE_HDR_IS890

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_DISABLE_TC_OFS

DESCRIPTION  : This function is called if any application ( so far FTAP or
               RTAP ) wants to disable TC OFS. TC OFS is enabled by default 
               whenever HDRSRCH enters traffic state. 

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_disable_tc_ofs ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtc_disable_ofs( );
    /* Disable TC_OFS */

} /* hdrsrch_disable_tc_ofs */


/*=========================================================================

FUNCTION     : HDRSRCH_SET_TAP

DESCRIPTION  : This function is called if any TAP application is running. 

DEPENDENCIES : None

PARAMETERS   : 

  tap -  whether TAP is running

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_set_tap
(
  boolean                    tap
    /* Whether tap is running */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchafc_set_tap( tap );

}  /* hdrsrch_set_tap */

/*===========================================================================

FUNCTION HDRSRCH_CLEAR_HANDOFF_COUNT()
 
DESCRIPTION
  This function clears out the HO count in the dsp but makes sure
  hdrsrch ho count is maintained properly for logging purposes.
 
DEPENDENCIES
  None
 
PARAMETERS
  subactive_set_idx - SubActiveSet index to clear.
   
RETURN VALUE
  None
  
SIDE EFFECTS
  Updates hdrsrchlog with ho count information.
  
===========================================================================*/

void hdrsrch_clear_handoff_count
( 
  uint8 subactive_set_idx
    /* SubActiveSet Index to clear */
)
{
 
  uint16          curr_ho_count;  
    /* The current HO counter register value. */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  curr_ho_count = hdrsrchset_get_asp_change_cnt( subactive_set_idx );
    /* Update the serving sector change count */
 
  hdrsrchset_reset_asp_change_cnt( subactive_set_idx ); 
    /* At this point we can safely intialize the DSP's ASP count */
 
  hdrsrchlog_set_serving_sect_handoff_count( curr_ho_count, TRUE );
    /* Keep track of the number of Handoffs for logging purposes.
     If we don't do this here we won't know the ho count has been
     reset. */
 
} /* hdrsrch_clear_handoff_count */
 
/* <EJECT> */ 
/*===========================================================================

FUNCTION HDRSRCH_GET_HANDOFF_COUNT()
 
DESCRIPTION
  This function gets the HO count from the dsp.
 
DEPENDENCIES
  None
 
PARAMETERS
  subactive_set_idx - SubActiveSet index to get handoff count.
   
RETURN VALUE
  Handoff count from the DSP.
  
SIDE EFFECTS
  None
  
===========================================================================*/

uint32 hdrsrch_get_handoff_count
( 
  uint8 subactive_set_idx
    /* SubActiveSet Index to get handoff count */
)
{
 
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 
  return ( hdrsrchset_get_asp_change_cnt( subactive_set_idx ) );
} /* hdrsrch_get_handoff_count */

#endif /* FEATURE_HDR_IS890 */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_HAS_MULTI_RX

DESCRIPTION  : This function queries if mulitple RX chains are supported.

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : 

  TRUE  - support multiple receive chains
  FALSE - support single receive chain

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrch_has_multi_rx( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchrf_has_multi_rx();
    /* Get the RF RX chain config from hdrsrchrf */
}


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_ASET_HAS_MORE_THAN_ONE_CELL

DESCRIPTION  : This function queries to find if the active set has more than
               one cell.

DEPENDENCIES : None

PARAMETERS   : subaset_idx - the subaset index of the subaset requseted.

RETURN VALUE : 

  TRUE  - ASET has sectors from more than one cell
  FALSE - ASET has sectors from only one cell

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrch_aset_has_more_than_one_cell
( 
#ifdef FEATURE_HDR_REVB
  uint8 subaset_idx
    /* The subaset index used in the query */
#else
  void 
#endif /* FEATURE_HDR_REVB */
)
{
#ifdef FEATURE_HDR_REVB
    return ( hdrsrchtc_get_num_cells_in_aset( subaset_idx ) > 1 );
#else
  return ( hdrsrchtc_get_num_cells_in_aset() > 1 );
#endif /* FEATURE_HDR_REVB */

} /* hdrsrch_aset_has_more_than_one_cell */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_NOTIFY_CC_PACKET

DESCRIPTION  : This function notifies searcher that a CC packet has been 
               received. It can be with good a CRC or with bad a CRC.
                
DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_notify_cc_packet( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchreacq_get_fpd_activation_status() == TRUE ) 
  {
    hdrsrchutil_enable_missed_scc_detection( TRUE );
      /* Forced preamble detection disables missed CC ISR
         so enable it here and clear any pending misses CC ISRs */

    hdrsrchreacq_reset_fpd_activation_status();
      /* Reset the forced premable activation flag */
  }

} /* hdrsrch_notify_cc_packet */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_REGISTER_VOIP_OUTAGE_CB

DESCRIPTION  : This function registers a callback function with searcher 
               to be called when a VOIP outage notification event 
               is to be generated.
                
DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_register_voip_outage_cb
( 
  hdrsrch_voip_outage_notify_cb_type    voip_outage_cb
    /* cb to notify VOIP outage to upper layers */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtc_register_voip_outage_cb( voip_outage_cb );
    /* Register callback function with connected state */

} /* hdrsrch_register_voip_outage_cb */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_CONFIGURE_AT_OPERATIONAL_MODE

DESCRIPTION  : This function registers callback functions with searcher 
               to be called when an AT operational mode management
               notification event occurs. It also configures
               whether two enable poor DO coverage and good DO coverage
               algorithms.
                
DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_configure_at_operational_mode
( 
  hdrsrch_opmode_config_enum_type      config,
    /* which at operational mode algorithm if any to run */
    
  hdrsrch_opmode_indication_cb_type    poor_coverage_cb,
    /* cb to indicate poor HDR coverage */

  hdrsrch_opmode_indication_cb_type    good_coverage_cb
    /* cb to indicate good HDR coverage */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_ATOM

  hdrsrchutil_configure_opmode( config,
                                poor_coverage_cb,
                                good_coverage_cb );
    /* Register AT operational mode callback functions 
       and enable/disable algorithms */

#endif /* FEATURE_ATOM */

} /* hdrsrch_register_opmode_cb */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_SET_TC_HANDDOWN_MODE

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

void hdrsrch_set_tc_handdown_mode
( 
  hdrsrch_tc_handdown_mode_enum_type   handdown_mode
    /* the handdown mode selected */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch.handdown_mode = handdown_mode;
    /* set handdown mode in traffic */

} /* hdrsrch_set_tc_handdown_mode */

/* <EJECT> */
/*=========================================================================

FUNCTION HDRSRCH_DIVERSITY_COMB_ENABLED

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

boolean hdrsrch_diversity_comb_enabled( void )
{
  return hdrsrchrfdiv_diversity_comb_enabled();
} /* hdrsrchrf_diversity_comb_enabled */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_UPDATE_NUM_SECTOR_USERS

DESCRIPTION
  Notifies the searcher with the number of active users in serving cell.
  This number is retrieved from FTCValid bits information in
  sector's quick config message.
    
DEPENDENCIES
  None

INPUTS
  Number of active users in serving sector.

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrch_update_num_sector_users
( 
  uint16 num_active_users
    /* number of active users in serving sector */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil_update_num_sector_users( num_active_users );

} /* hdrsrch_update_num_sector_users() */



/*===========================================================================

FUNCTION HDRSRCH_GET_TX_CARRIER_FREQ

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
uint32 hdrsrch_get_tx_carrier_freq
(
  sys_band_class_e_type      band,
    /* Band */

  sys_channel_num_type       chan_num
    /* Channel number */
)
{

  sys_channel_type          band_chan;
    /* Input parameter to rfm_get_tx_carrier_freq(); */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  band_chan.band          = band;
  band_chan.chan_num      = chan_num;

  /* Cast the band type. The two types defined the same value for BC0-15 */
  return rfm_cdma_get_tx_carrier_freq( band_chan );

}  /* hdrsrch_get_tx_carrier_freq */


/*===========================================================================

FUNCTION HDRSRCH_GET_RX_CARRIER_FREQ

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
uint32 hdrsrch_get_rx_carrier_freq
(
  sys_band_class_e_type      band,
    /* Band */

  sys_channel_num_type       chan_num
    /* Channel number */
)
{

  sys_channel_type          band_chan;
    /* Input parameter to rfm_get_tx_carrier_freq(); */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  band_chan.band          = band;
  band_chan.chan_num      = chan_num;

  /* Cast the band type. The two types defined the same value for BC0-15 */
  return rfm_cdma_get_rx_carrier_freq( band_chan );

}  /* hdrsrch_get_rx_carrier_freq */

/*=========================================================================

FUNCTION HDRSRCH_IS_BAND_CHAN_SUPPORTED

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
boolean hdrsrch_is_band_chan_supported 
( 
  sys_band_class_e_type band, 
  word chan
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchrf_is_band_chan_supported(band, chan);

} /* hdrsrch_band_chan_supported */


/*============================================================================

FUNCTION HDRSRCH_GET_BAND_SUBCLASS

DESCRIPTION
  This function gets the currently supported band subclasses of the given band.
  It is a wrapper of function hdrsrchrf_get_band_subclass().
  
DEPENDENCIES
  None
  
RETURN VALUE
  The mask of the current subclasses of the given band.

SIDE EFFECTS
  None

============================================================================*/
uint32 hdrsrch_get_band_subclass
(
  uint8    band_class
    /* Band Class */
)
{
  return hdrsrchrf_get_band_subclass(band_class);
} /* hdrsrch_get_band_subclass */


#ifdef FEATURE_HDR_ADAPTIVE_DIV
#error code not present
#endif /* FEATURE_HDR_ADAPTIVE_DIV */

/*===========================================================================

FUNCTION HDRSRCH_GET_ASP_INDEX

DESCRIPTION
  The function returns the asp index that matches the drc cover.

DEPENDENCIES
  None

PARAMETERS
  demod_idx - demod carrier index
  drc-cover - drc cover

RETURN VALUE
  return asp index that matches drc cover or invalid asp index if no match.

SIDE EFFECTS
  None

===========================================================================*/

hdrsrch_demod_idx_type hdrsrch_get_asp_index
(
  uint8                      demod_idx,
    /* Demod carrier index */

  hdrmac_drc_cover_type      drc_cover
    /* drc cover */
)  
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchset_get_asp_index( demod_idx, drc_cover );

} /* hdrsrch_get_asp_index */


/*===========================================================================

FUNCTION HDRSRCH_GET_SUB_ASET_INDEX

DESCRIPTION
  The function returns the subaset index of the given demod carrier.

DEPENDENCIES
  None

PARAMETERS
  demod_idx - Index of the demod carrier

RETURN VALUE
  Sub Active Set index

SIDE EFFECTS
  None

===========================================================================*/

hdrsrch_subaset_idx_type hdrsrch_get_subaset_index
(
  uint8                      demod_idx
    /* Demod index */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchset_get_subaset_index(demod_idx);

} /* hdrsrch_get_subaset_index */


/*==========================================================================

FUNCTION HDRSRCH_PILOT_MEAS_REQUEST

DESCRIPTION
  Posts a pilot measurement request command to searcher. This command allows
  the application to get pilot measurement of a list of PNs in one channel.
  It is similar to RUP request, but pilot measurement requests is more flexible.
  RUP request is trigger by Route Update Request Message from network. Here is 
  the list of difference of pilot measurement request command and RUP command:
     - RUP only measures pilots in CSET/ASET/NSET if the channel is same as
       aset channel. Pilot measurement request could measure any PN in the same
       channel_change.
     - RUO could be called in IDLE or Traffic state. Pilot measurement could be 
       only called in IDLE
     - hdrsrch_get_pilot_report returns the result for RUP request. 
       hdrsrch_get_pilot_meas_report returns the result for pilot measurement
  

DEPENDENCIES
  None
  
PARAMETERS
  pilot_meas_request   - Parameters of the pilot measurement request.
  pilot_meas_rpt_cb    - Pilot measurement report indication callback

RETURN VALUE
  None

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_pilot_meas_request
(
  const hdrsrch_pilot_meas_req_params_type   *pilot_meas_request,
    /* pointer to pilot measurement request params structure */
    
  hdrsrch_pilot_rpt_cb_type                  pilot_meas_rpt_cb
    /* Pilot report indication callback, or NULL to disable */
)
{
  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_rup_req_params_type  *params;
    /* Convenience pointer */

  uint8                               sector_count;
    /* number of sectos in message */

  int                                 i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_PILOT_MEAS_CMD );
    /* Command HDRSRCH_TASK to perform pilot measurement */

  if ( cmd != NULL )
  {
    /* We reuse rup_req parameter data structure for pilot measurement request.
    */

    params = &cmd->params.rup_req;
      /* Convenience pointer */

    /* Copy parameters */
    params->channel_included = pilot_meas_request->channel_included;
    params->channel.chan_num = pilot_meas_request->channel.chan_num;
    params->channel.band = pilot_meas_request->channel.band;
    params->system_type = HDRSRCH_SYSTYPE_DO;
    params->sector_count = pilot_meas_request->sector_count;
    sector_count = pilot_meas_request->sector_count;
    params->pilot_rpt_cb = pilot_meas_rpt_cb;

    if ( sector_count != 0)
    {
      params->window_size_included = pilot_meas_request->window_size_included;
      params->window_offset_included = pilot_meas_request->window_offset_included;
    }
    else
    {
      params->window_size_included = 0;
      params->window_offset_included = 0;    
    }
        
    /* copy individual sector parameters if sector count is non zero */
    if ( sector_count != 0)
    {
      for ( i = 0 ; i < sector_count ; i++ )
      {
        params->sector_info[i].pilot_pn = pilot_meas_request->sector_info[i].pilot_pn;
      }
      
      if ( params->window_offset_included == TRUE )
      {
        for ( i = 0 ; i < sector_count ; i++ )
        {
          params->sector_info[i].window_size = pilot_meas_request->sector_info[i].window_size;
        }
      }
        
      if ( params->window_size_included == TRUE )
      {
        for ( i = 0 ; i < sector_count ; i++ )
        {
          params->sector_info[i].window_offset = pilot_meas_request->sector_info[i].window_offset;        
        }
      }  
      
    }      

    params->chan_mgmt_1x_included = 0;
      /* This field is not in pilot measurement request, set it to 0 */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_pilot_meas_request( ) */


/*==========================================================================

FUNCTION HDRSRCH_GET_PILOT_MEAS_REPORT

DESCRIPTION
  Retrieves a pilot measurement result.

DEPENDENCIES
  

PARAMETERS
  ref_pn          - Reference pilot pn
  ref_pn_channel  - Channel number of reference pilot pn
  ref_pn_strength - Pilot strenth of reference pilot pn
  max_pilots      - Maximum number of pilots which will be retrieved.
  pilots          - Array of length max_pilots, in which the pilot report
                    information will be stored.

RETURN VALUE
  number of returned pilot in pilots array.

SIDE EFFECTS
  

==========================================================================*/

int hdrsrch_get_pilot_meas_report
(
  sys_pilot_pn_type              * ref_pn,
    /* reference pilot pn */

  sys_channel_type               * ref_pn_channel,
    /* reference pilot pn channel number */

  int16                          * ref_pn_strength,
    /* reference pilot pn strength */

  int                            max_pilots,
    /* Maximum # of pilot entries which may be placed in "pilots" array */

  hdrsrch_pilot_meas_rpt_type    * pilots
    /* Array to store pilot information in */
)
{
  hdrsrch_sect_struct_type *      sect;
    /* sector type for reference sector */

  int                             n;
    /* Counter/Index into report structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_DISABLE_PREMPTION();
    /* Can't process any search dumps, or other set management until
       this function returns! */

  /*---------------------------------*
   * Determine the Reference Pilot.  *
   *---------------------------------*/

  sect = hdrsrchutil_get_ref_sector_ptr();

  if ( sect != NULL )
  {
    *ref_pn = sect->pn_offset;
      /* get reference pilot pn */

    *ref_pn_channel = sect->chan;
      /* get reference pilot channel */

    *ref_pn_strength = hdrsrchset_eng_to_ecio_ceiling_convert( sect->total_eng );
      /* get reference pilot strength in db */   
  }
  else
  {
    *ref_pn = 0;
    *ref_pn_strength = 0;
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Get_Pilot_Meas_Report:RefPN=%d ref_pn_channel= %d",
                  *ref_pn,
                  ref_pn_channel->chan_num );

  n = hdrsrchset_get_pilot_meas_report(max_pilots, pilots, sect);
    /* Get the pilot report, using "sect" as the reference pilot */

  REX_ENABLE_PREMPTION();
    /* Ok, done! */

  return n;

} /* hdrsrch_get_pilot_meas_report( ) */


/*==========================================================================

FUNCTION      HDRSRCH_GET_CC_START

DESCRIPTION
  This function returns the CC start value in slot.
  
DEPENDENCIES
  None 

INPUTS
  None

RETURN VALUE
  CC start in slot

SIDE EFFECTS
  None

==========================================================================*/

uint8 hdrsrch_get_cc_start( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchsleep_get_cc_start();
    /* return CC start */

}/* hdrsrch_get_cc_start */


/*==========================================================================

FUNCTION      HDRSRCH_SET_CC_START

DESCRIPTION
  This function sets the CC start and sends the message to tell firmware 
  when to look for CC or SCC preambles.
  
DEPENDENCIES
  This function needs to be called after AT wakes up. 

INPUTS
  cc_start - CC start in slot

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_set_cc_start
(
  uint8               cc_start
    /* Time in slot that firmware expects to find CC/SCC premable */
)
{
  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_set_cc_start_params_type    *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd = hdrsrchcmd_new( HDRSRCH_SET_CC_START_CMD );
    /* Command HDRSRCH_TASK to set cc_start */

  if ( cmd != NULL )
  {
    params = &cmd->params.cc_start;
      /* Convenience pointer */

    params->cc_start = cc_start;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_set_cc_start */


/*==========================================================================

FUNCTION HDRSRCH_SET_SCC_OFFSET

DESCRIPTION
  This function sends the message to the firmware to set scc offset.

DEPENDENCIES
  None

PARAMETERS
  scc_offset - SCC offset
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_set_scc_offset
(
  uint16                scc_offset
    /* SCC offset */
)
{

  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_set_scc_offset_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd = hdrsrchcmd_new( HDRSRCH_SET_SCC_OFFSET_CMD );
    /* Command HDRSRCH_TASK to set scc_offset */

  if ( cmd != NULL )
  {
    params = &cmd->params.scc_offset;
      /* Convenience pointer */

    params->scc_offset = scc_offset;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_set_scc_offset */


/*==========================================================================

FUNCTION HDRSRCH_SET_SCC_INFO

DESCRIPTION
  This function sets CC info to hdr srch.

DEPENDENCIES
  None

PARAMETERS
  scc_offset - SCC offset
  cc_rate    - SCC rate
  link_id    - Link Id
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_set_scc_info
(
  uint16                scc_offset,
    /* SCC offset */

  uint8                 cc_rate,
    /* SCC rate */

  uint8                 link_id
    /* Link id */
)
{

  hdrsrch_cmd_type                    *cmd;
    /* Command structure */

  hdrsrch_set_scc_info_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd = hdrsrchcmd_new( HDRSRCH_SET_SCC_INFO_CMD );
    /* Command HDRSRCH_TASK to set scc_offset */

  if ( cmd != NULL )
  {
    params = &cmd->params.scc_info;
      /* Convenience pointer */

    params->scc_offset = scc_offset;
    params->scc_rate = cc_rate;
    params->link_id = link_id;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_set_scc_offset */


/*==========================================================================

FUNCTION HDRSRCH_ENABLE_FW_HDR_APP

DESCRIPTION
  This function sends the command to HDR SRCH task to enable CDMA clocks and
  the firmware HDR App.

DEPENDENCIES
  None

PARAMETERS
  call_back - Call back function
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_enable_fw_hdr_app
(
  hdrsrch_enable_fw_hdr_app_cb_type  call_back,
    /* Call back function */

  hdrsrch_fw_state_type               hdr_fw_next_state
   /* hdr fw new state */
)
{
  hdrsrch_cmd_type                       *cmd;
    /* Command structure */

  hdrsrch_enable_fw_hdr_app_params_type  *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd = hdrsrchcmd_new( HDRSRCH_ENABLE_FW_HDR_APP );
    /* Command HDRSRCH_TASK to enable HDR App */

  if ( cmd != NULL )
  {

    params = &cmd->params.enable_fw_hr_app;
      /* Convenience pointer */

    params->enable_fw_hdr_app_cb = call_back;
    params->fw_state = hdr_fw_next_state;
      /* Copy parameters */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
} /* hdrsrch_enable_fw_hdr_app */

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/*==========================================================================

FUNCTION HDRSRCH_SEND_IRAT_MSG_TO_FW

DESCRIPTION
  This function sends the PilotMeasCfgReq to HDR FW. It is an API for FTM.

DEPENDENCIES
  None

PARAMETERS
  call_back - Call back function
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrch_send_irat_msg_to_fw( void )
{
  hdrfw_pilot_meas_cfg_req_msg_t            pilot_meas_cfg_req_msg;
    /* pilot meas cfg request message */

  rfm_meas_time_const_type                  meas_time_constant;
    /* measurement time constant from RF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Send PilotMeasCfg request message to fw */
    memset( &pilot_meas_cfg_req_msg, 0, sizeof( hdrfw_pilot_meas_cfg_req_msg_t ) );
      /* clear the message */
    msgr_init_hdr( &pilot_meas_cfg_req_msg.hdr, 
                   MSGR_HDR_SRCH, 
                   HDR_FW_PILOT_MEAS_CFG_REQ_CMD );
      /* Init message header */

    pilot_meas_cfg_req_msg.durationInfinite = TRUE ;
      /* no limit for duration */

    pilot_meas_cfg_req_msg.startImmediate = TRUE ;
      /* start pilot measurement ASAP */

    pilot_meas_cfg_req_msg.idleOfflineSearch = FALSE;
      /* Set fw for online search, give enough time for RF FTM */

    pilot_meas_cfg_req_msg.rotError = 0;
      /* assign rotator error */

    pilot_meas_cfg_req_msg.numSamplesDesired = 2048;
      /* 2 half slots (2048 samples) captured. May not be used since
         StartImmediately is true */

    meas_time_constant = rfm_meas_common_get_time_constant(
                             RFM_LTE_MODE,        /* source mode */
                             RFM_1XEVDO_MODE );   /* target mode */
      /* Get time constants from RF, in unit of micro second */

    pilot_meas_cfg_req_msg.rfTuneBackTimeConstant = 
           ( meas_time_constant.clean_up_time ) * 768 / 625;
      /* Set time constants in unit of CDMA chip to firmware */
    hdrsrchmsg_send_msg( &pilot_meas_cfg_req_msg.hdr, 
                         sizeof( hdrfw_pilot_meas_cfg_req_msg_t ) );
    /* Send the message */
} /* hdrsrch_send_irat_msg_to_fw */

/*==========================================================================

FUNCTION HDRSRCH_FAST_ACQ

DESCRIPTION
  Attempts to acquire a given pn with CDMA system time transfer

DEPENDENCIES
  None

PARAMETERS
  fast_acq_params  -- structure to hold parameters         

RETURN VALUE
  None

SIDE EFFECTS

==========================================================================*/

void hdrsrch_fast_acq
(
  hdrsrch_fast_acq_params_type    *fast_acq_params
    /* fast acq parameters */
)
{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

  hdrsrch_fast_acq_params_type     *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_FAST_ACQ_CMD );
    /* Command HDRSRCH_TASK to do fast acq */

  if ( cmd != NULL )
  {
    params = &cmd->params.fast_acq_req;
      /* Convenience pointer */

    params->reason               = fast_acq_params->reason;
    params->channel              = fast_acq_params->channel;
    params->pilot_pn             = fast_acq_params->pilot_pn;
    params->fast_acq_cb          = fast_acq_params->fast_acq_cb;
    params->system_lost_cb       = fast_acq_params->system_lost_cb;
    params->usability_cb         = fast_acq_params->usability_cb;
    params->srch_window_size     = fast_acq_params->srch_window_size;
        /* Copy the valid window size */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_fast_acq( ) */

/*==========================================================================

FUNCTION HDRSRCH_TRAN_SYS_TIME

DESCRIPTION
  Attempts to transfer cdma system time

DEPENDENCIES
  None

PARAMETERS
  

RETURN VALUE
  None

SIDE EFFECTS
  

==========================================================================*/

void hdrsrch_tran_sys_time
(
  hdrsrch_tran_sys_time_params_type  *tran_sys_time_params,
    /* parameters  of trans_sys_time command */

  hdrsrch_tran_sys_time_cb_type      tran_sys_time_cb_func
    /* callback function to indicate time transfer done */
)
{
  hdrsrch_cmd_type                      *cmd;
    /* Command structure */

   hdrsrch_tran_sys_time_params_type    *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_TRAN_SYS_TIME_REQ );
    /* Command HDRSRCH_TASK to do fast acq */

  if ( cmd != NULL )
  {
    params = &cmd->params.tran_sys_time_req;
      /* Convenience pointer */

    params->cdma_sys_time           = tran_sys_time_params->cdma_sys_time;
    params->lte_ostmr               = tran_sys_time_params->lte_ostmr;
       /* Copy parameters */

    hdrsrchtimesync_set_cb( tran_sys_time_cb_func );
    MSGR_SET_VARIANT(cmd->name, hdrsrch_get_subscriber_id());
    HDR_MSG_PROT_2( MSG_LEGACY_HIGH, "time transfer req cmd :%d  SRCH SUB %d",
                      cmd->name,hdrsrch_get_subscriber_id());
    hdrsrchcmd_post( cmd );
      /* Post the command */    
  }

} /* hdrsrch_trans_sys_time( ) */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCH_RUNTIME_DIVERSITY_PREF_CTRL

DESCRIPTION  : This function provides the API for upper layers to set the 
               diversity preference at runtime. 
               By default, the preference is div on. If it's called to 
               disable div, diversity will be shut off immediately without 
               DRC ramp down.

DEPENDENCIES : 

INPUT        : Whether diversity is on.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_runtime_diversity_pref_ctrl
(
  hdrsrch_rx_div_enum_type       div_ctrl
    /* Runtime diversity control */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_MSM_SUPPORT

  /* Make sure DO is not disabled on the device */
  if( msm_support_is_available( MSM_SUPPORT_1XEVDO ) != 
    MSM_SUPPORT_DISABLED )

#endif /* FEATURE_MSM_SUPPORT*/
  {
    #ifdef FEATURE_HDR_THERMAL_BACKOFF

    if( div_ctrl == HDRSRCH_RX_DIV_OFF )
    {
      HDRSRCH_SET_USR_PREF_DIV_DISABLE(hdrsrch.div_ctrl_pref_vote);
    }
    else
    {
      HDRSRCH_CLR_USR_PREF_DIV_DISABLE(hdrsrch.div_ctrl_pref_vote);
    }
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL   
    if( div_ctrl == HDRSRCH_SECOND_CHAIN_ONLY )
    {
      HDRSRCH_SET_SECOND_CHAIN_PREF_ENABLE(hdrsrch.div_ctrl_pref_vote);
    }
    else
    {
      HDRSRCH_CLR_SECOND_CHAIN_PREF_ENABLE(hdrsrch.div_ctrl_pref_vote);
    }
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */

    /* If user says OFF then set to OFF, else if both say enabled, then set to enable */
   if( (HDRSRCH_ARE_ALL_DIV_VOTES_ENABLED(hdrsrch.div_ctrl_pref_vote) ) || 
       (div_ctrl == HDRSRCH_RX_DIV_OFF) )

#endif /* FEATURE_HDR_THERMAL_BACKOFF */
   {
      hdrsrchrfdiv_runtime_diversity_pref_ctrl( div_ctrl );
   }
      /* set runtime preference */
  }

} /* hdrsrch_runtime_diversity_pref_ctrl */

#ifdef FEATURE_HDR_TO_LTE
/*==========================================================================

FUNCTION HDRSRCH_LTE_NSET_UPDATE

DESCRIPTION
  Update the LTE neighbor list from the OtherRATNeighborList OTA message

DEPENDENCIES
  OtherRATNeighborList OTA message has its own transmission cycle

PARAMETERS 
  LTE frequency information 

RETURN VALUE
  None

SIDE EFFECTS
  Update the LTE neighbor list for DO to request measurements.  If LTE
  pilot measurements meet reselection criteria, Mobile will reselect to LTE.

==========================================================================*/

void hdrsrch_lte_nset_update
(
  hdrsrch_lte_nset_update_params_type   *lte_nset_update_params
)
{

  hdrsrch_cmd_type                      *cmd;
    /* Command structure */

  hdrsrch_lte_nset_update_params_type   *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_LTE_NSET_UPDATE_CMD );
    /* Command HDRSRCH_TASK to update the LTE NSET */

  if ( cmd != NULL )
  {
    params = &cmd->params.lte_nset_update;
      /* Convenience pointer */

#ifdef FEATURE_MEMCPY_REMOVAL
    *params = *lte_nset_update_params;
#else
    memcpy( params, lte_nset_update_params, sizeof( hdrsrch_lte_nset_update_params_type ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_lte_nset_update */

/*==========================================================================

FUNCTION HDRSRCH_ABORT_RESELECT_TO_LTE 
 
DESCRIPTION
  Inform SRCH that LTE reselection has been aborted

DEPENDENCIES
  None

PARAMETERS 
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_abort_reselect_to_lte ( void )

{
  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_ABORT_LTE_RESELECT_CMD );
    /* Srch should abort LTE reselection  */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_abort_reselect_to_lte */

#endif /* FEATURE_HDR_TO_LTE */

/*=========================================================================

FUNCTION     : HDRSRCH_LINEAR_ENG_TO_DB

DESCRIPTION  : This function converts linear energy to db in unit of -0.5db

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : energy in db

SIDE EFFECTS : None

=========================================================================*/

int hdrsrch_linear_eng_to_db
( 
  int                             lin_eng
    /* Linear energy to be converted to -0.5dB units */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchset_eng_to_ecio( lin_eng );

} /* hdrsrch_linear_eng_to_db */

#ifdef FEATURE_HDR_THERMAL_BACKOFF

/*=========================================================================

FUNCTION     : HDRSRCH_THERMAL_DIVERSITY_PREF_CTRL

DESCRIPTION  : This function provides the API for CCM to set the 
               diversity preference at runtime according to thermal
               based flow control.
               By default, the preference is div on. If it's called to 
               disable div, diversity will be shut off immediately without 
               DRC ramp down.

DEPENDENCIES : 

INPUT        : Whether diversity is on.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_thermal_diversity_pref_ctrl
(
  boolean       div_ctrl
    /* Runtime diversity control */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_MSM_SUPPORT

  /* Make sure DO is not disabled on the device */
  if( msm_support_is_available( MSM_SUPPORT_1XEVDO ) != 
    MSM_SUPPORT_DISABLED )

#endif /* FEATURE_MSM_SUPPORT*/
  {
    if( div_ctrl== FALSE )
    {
      HDRSRCH_SET_THERMAL_BACKOFF_DIV_DISABLE(hdrsrch.div_ctrl_pref_vote);
    }
    else
    {
      HDRSRCH_CLR_THERMAL_BACKOFF_DIV_DISABLE(hdrsrch.div_ctrl_pref_vote);
    }
    /* If CCM says OFF then set to OFF, else if both say enabled, then set to enable */
    if( (HDRSRCH_ARE_ALL_DIV_VOTES_ENABLED(hdrsrch.div_ctrl_pref_vote) )
         || (div_ctrl == FALSE) )
    {
#ifdef FEATURE_HDR_SECOND_CHAIN_TEST_CALL   
      if( HDRSRCH_IS_SECOND_CHAIN_ONLY_ENABLED(hdrsrch.div_ctrl_pref_vote) )
      {
        hdrsrchrfdiv_runtime_diversity_pref_ctrl( HDRSRCH_SECOND_CHAIN_ONLY );
      }
      else
#endif /* FEATURE_HDR_SECOND_CHAIN_TEST_CALL */
    {
      hdrsrchrfdiv_runtime_diversity_pref_ctrl( div_ctrl );
      }
      /* set thermal preference */
    }
    
  }

} /* hdrsrch_thermal_diversity_pref_ctrl */

#endif /* FEATURE_HDR_THERMAL_BACKOFF */


/*============================================================================

FUNCTION HDRSRCH_GET_BAND

DESCRIPTION
  This function gets the band which HDR is currently operating on.
  
DEPENDENCIES
  None
  
RETURN VALUE
  The band HDR is operating on.

SIDE EFFECTS
  None

============================================================================*/
sys_band_class_e_type hdrsrch_get_band( void )
{
  const sys_channel_type                *channel;

#ifdef FEATURE_HDR_REVB
  uint8                                 chan_cnt;
    /* Channel count */

  const hdrsrch_demod_idx_type          *demod_idx;
    /* A pointor to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  channel = hdrsrchrf_get_channel(HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_idx);
#else
  channel = hdrsrchrf_get_channel(HDRSRCHRF_PRI_CHAIN);
#endif /* FEATURE_HDR_REVB */
  /* Get band class and channel number */

  return channel->band;
    /* Return band class */
} /* hdrsrch_get_band */

/*=========================================================================

FUNCTION HDRSRCH_IS_GRICE_ENABLED

DESCRIPTION 
  This function checks if GRICE is enabled (does not have to be currently
  active to be enabled).

DEPENDENCIES
  None

RETURN VALUE
  TRUE -  If GRICE is enabled.
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/
boolean hdrsrch_is_grice_enabled( void )
{
  return hdrsrcheq_is_grice_enabled();
}


/*=========================================================================

FUNCTION     : HDRSRCH_GET_RX_TX_POWER_INFO

DESCRIPTION
  Returns HDR RX and TX AGC to the caller.
 
  Rxagc and txagc reported in 1\10 dBm granularity.
 
  Ecio will only be reported for primary chain, in traffic it will be
  instantaneous primar energy, in idle state it will be total energy.
  Ecio will also be reported in in 1\10 dBm.
  Range 0 to -315 (0.1 dbm)
 
    
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

void hdrsrch_get_rx_tx_power_info
(
  hdrsrch_rx_pwr_info_struct_type   *rx_power_info,
  hdrsrch_tx_pwr_info_struct_type   *tx_power_info
)
{

  hdrsrchrf_get_tx_power_info( tx_power_info );

  hdrsrchrf_get_rx_power_info( rx_power_info );

  return;
  
}/* hdrsrch_get_rx_tx_power_info */

#ifdef FEATURE_HDR_REVC
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_UPDATE_LOADADJ

DESCRIPTION
  Queues loadadj command to FW from up to 3 carriers

DEPENDENCIES

PARAMETERS 
  load_info_ptr - Pointer loadadj information

RETURN VALUE 
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrch_update_loadadj
(
  const hdrsrch_loadadj_type* loadadj_ptr
)
{
  hdrsrch_cmd_type* cmd;

  HDR_MSG_SRCH(MSG_LEGACY_MED, "hdrsrch_update_loadadj");

  cmd = hdrsrchcmd_new( HDRSRCH_UPDATE_LOADADJ_CMD );    

  if ( cmd != NULL )
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    cmd->params.loadadj = *loadadj_ptr;
#else
    memcpy(&cmd->params.loadadj, 
           loadadj_ptr, 
           sizeof(hdrsrch_loadadj_type));
#endif /* FEATURE_MEMCPY_REMOVAL */

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
}

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCH_ENABLE_NLB

DESCRIPTION
  This function notify HDRSRCH to enable/disable NLB. When NLB is
  enabled, CC demod is enabled on all 3 carriers (in traffic), when
  it is disabled, 3 carrier CC demod is turned off, and any LoadAdj
  is reset.

DEPENDENCIES

PARAMETERS 
  enable_nlb - Whether to enable or disable NLB

RETURN VALUE 
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrch_enable_nlb
(
  const boolean enable_nlb
)
{
  hdrsrch_cmd_type* cmd;

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "hdrsrch_enable_nlb %d", enable_nlb);

  cmd = hdrsrchcmd_new( HDRSRCH_ENABLE_NLB_CMD );
    /* Command HDRSRCH_TASK to acquire an HDR system */

  if ( cmd != NULL )
  {
    cmd->params.enable_nlb = enable_nlb;

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
}
#endif /* FEATURE_HDR_REVC */

/*=========================================================================

FUNCTION HDRSRCH_CHANGE_SLEEP_TIMELINE

DESCRIPTION 
  This function notifies HDR SRCH to use extended or optimized timeline.

DEPENDENCIES
  None

RETURN VALUE
  TRUE -  optimized timeline.
  FALSE - extended timeline.
  
SIDE EFFECTS
  None.

=========================================================================*/
void hdrsrch_change_sleep_timeline
(
  boolean       optimized
    /* Whether use the extended or optimized timeline */
)
{
  hdrsrch_cmd_type                      *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_CHANGE_SLEEP_TIMELINE_CMD );
    /* Command HDRSRCH_TASK to change sleep timeline */

  if ( cmd != NULL )
  {
    cmd->params.optimized_sleep_timeline.optimized = optimized;

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_change_sleep_timeline */


/*==========================================================================

FUNCTION HDRSRCH_TIME_UNTIL_TUNE_COMPLETE

DESCRIPTION
  If the HDR tune is happening within 1x tune duration, return the time that tune will be 
  completed. Otherwise, return 0ms.

DEPENDENCIES
  None

PARAMETERS
 tune_duration_1x_us: 1x tune duration in usec

RETURN VALUE
  0: There is no conflict between 1x and HDR RF tune.
  >0 : There is conflict and the time in usec until the HDR tune completes 

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrch_time_until_tune_complete
(
  uint32 tune_duration_1x_us
    /* 1x tune duration in usec*/
)
{
  uint32           time_to_complete = 0;
    /* Time in usec until to tune complete */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_to_complete = hdrsrchsleep_time_until_tune_complete( tune_duration_1x_us );

  return time_to_complete;

}  /* hdrsrch_time_until_tune_complete */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV

/*=========================================================================

FUNCTION     : HDRSRCH_ACQ_DIV_CTRL

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

void hdrsrch_acq_div_ctrl
(
  boolean             acq_div_runtime_ctrl
)
{
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "ACQ Div:  run time acq div ctrl=%d",
                  (uint8)acq_div_runtime_ctrl );
  hdrsrch.acq_div.acq_div_runtime_ctrl = acq_div_runtime_ctrl;
  return;

} /* hdrsrch_acq_div_ctrl */
     
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV

/*=========================================================================

FUNCTION     : HDRSRCH_RECORD_MISSED_CC

DESCRIPTION  : This function lets SRCH know that we missed sync cc.

DEPENDENCIES : None.

INPUT        : None.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrch_record_missed_cc()
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  
  if( hdrsrch.idle_state_div.idle_div_enabled )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: MISSED SYNC CC");

    hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed = TRUE;
  }

} /* hdrsrch_record_missed_cc */

#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

/*=========================================================================

FUNCTION HDRSRCH_ACQ_SEARCH_ACTIVE

DESCRIPTION 
  This function returns whether HDR Acq search is active.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: Acq srch is active.
  FALSE: Otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/
boolean hdrsrch_acq_search_active(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  return ( ( hdrsrch_state_id == HDRSRCH_ACQ_STATE ) && ( hdrsrchdrv.active_srch == SRCH_STATUS_ACTIVE ) );
} /* hdrsrch_acq_search_active */
/* EJECT */

/*==========================================================================

FUNCTION        HDRSRCH_INIT_DIV_SM

DESCRIPTION
  This function initialize DIV/ARD state machine only once. Subsequent
  calls are no-ops

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

void hdrsrch_init_div_sm( void )
{
  static boolean first_init = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( first_init == FALSE )
  {
    first_init = TRUE;

    hdrsrchrfdiv_start_stm();
      /* Start div stm */

    hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
      /* set user preference for antenna diversity setting */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_start_stm();
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
  }
} /* hdrsrch_init_div_sm */


/*===========================================================================

FUNCTION HDRSRCH_SET_LRE_ENABLE

DESCRIPTION
  This function sets internal flag that is checked to see if LRE is enabled/disabled
 
DEPENDENCIES 
  None
  
PARAMETERS
  LRE enabled/disabled status

RETURN VALUE
  None.

SIDE EFFECTS
  If status flag is FALSE, then LRE feature will be disabled.

===========================================================================*/

void hdrsrch_set_lre_enable
(
  boolean status
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch.lre_enabled = status;
    /* Set LRE feature enabled flag */

} /* hdrsrch_set_lre_enable( ) */

/*===========================================================================

FUNCTION HDRSRCH_IS_LRE_ENABLED

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

boolean hdrsrch_is_lre_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrch.lre_enabled;
    /* Return LRE feature enabled/disabled status */

} /* hdrsrch_is_lre_enabled( ) */

/*===========================================================================

FUNCTION HDRSRCH_IS_IDLE_TA_STARTED

DESCRIPTION
  This function returns tune away status in Idle. This will be helpful in 
  stopping Fast connect TCA processing while tuning away in Idle.

DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tune away processing has started.
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrch_is_idle_ta_started( void )
{

  return hdrsrchidle_is_ta_started();

} /* hdrsrch_is_idle_ta_started */


#ifdef FEATURE_HDR_QTA

/*===========================================================================

FUNCTION HDRSRCH_IS_TA_STARTED

DESCRIPTION
  This function returns tune away status. This is interface function.

DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tune away processing has started.
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrch_is_ta_started( void )
{

  boolean is_ta = FALSE;

  switch ( hdrsrch_get_current_state() )
  {
    case HDRSRCH_CONNECTED_STATE:
      is_ta = hdrsrchtc_is_ta_started();
      break;

    case HDRSRCH_SUSPENDED_TC_STATE:
    case HDRSRCH_SUSPENDED_IDLE_STATE:
      is_ta = TRUE;
      break;

    default:
      is_ta = FALSE;
      break;
  }

  return is_ta;
  
} /* hdrsrch_is_ta_started */

/*==========================================================================

FUNCTION        HDRSRCH_IS_QTA_ENABLED

DESCRIPTION
  This function returns whether we are in QTA currently.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  TRUE: in QTA
  FALSE: in LTA

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrch_is_qta_enabled( void )
{
  return hdrsrchtc_is_qta_enabled();
}

/*===========================================================================

FUNCTION HDRSRCH_CONNECTION_CLOSE_STATE

DESCRIPTION
  This function intimates the connection close state to HDR-SRCH
 
DEPENDENCIES 
  None. 

PARAMETERS
  Connection close state
  0 - No connetion close
  1 - Connection close inprogress

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrch_connection_close_state( boolean connection_close_state )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc_change_substate( connection_close_state );

} /* hdrsrch_connection_close_state( ) */
#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_TO_LTE
/*==========================================================================

FUNCTION HDRSRCH_UPDATE_SECTOR

DESCRIPTION
  This function provides HDRSRCH of the current sector ID.  

DEPENDENCIES
  None

PARAMETERS
  Sector ID of the current sector, including the Access Terminal 
    Identifier (ATI), Sector ID, and Subnet ID

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrch_update_sector
(
  hdraddr_type        sector_id
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( hdrsrchltemeas_update_sector( sector_id ) );
    /* Update HDR current sector */

} /* hdrsrch_update_sector */

#endif /* FEATURE_HDR_TO_LTE */

/*===========================================================================
FUNCTION HDRSRCH_READ_NV

DESCRIPTION
  Read value of NV values.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

NOTE
  None
===========================================================================*/
void hdrsrch_read_nv( void )
{
  hdrsrch_read_efs_nv_items();

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_read_nv_config();
#endif /* FEATURE_HDR_SELECTIVE_TXD */

} /* hdrsrch_read_nv() */

#ifdef FEATURE_HDR_QTA
/*==========================================================================

FUNCTION        HDRSRCH_SET_QTA_NV

DESCRIPTION
  This function sets QTA NV status.

DEPENDENCIES
  None

INPUTS
  If QTA is enabled via NV or not.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrch_set_qta_nv( boolean is_enabled )
{
  
  hdrsrchtc_set_qta_nv(is_enabled);

}
#endif /* FEATURE_HDR_QTA */


/*=========================================================================

FUNCTION     : HDRSRCH_SYSTEM_LOST

DESCRIPTION  : This function declares system lost, and transfers back to
               the inactive state

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Exits to inactive.

=========================================================================*/

void hdrsrch_system_lost
(
  hdrsrch_system_lost_enum_type   reason
    /* Reason for declaring system lost */
)
{
    hdrsrch_cmd_type                      *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, 
                 "System lost cmd at hdrsrch_state_id %d", 
                 hdrsrch_state_id);

  if ( ( hdrsrch_state_id != HDRSRCH_ACQ_STATE ) && 
       ( hdrsrch_state_id != HDRSRCH_SYNC_STATE ) )
  {
    cmd = hdrsrchcmd_new( HDRSRCH_SYSTEM_LOST_CMD );
      /* Command HDRSRCH_TASK to change sleep timeline */

    if ( cmd != NULL )
    {
      cmd->params.system_lost_reason.reason = reason;
      hdrsrch.sysloss_pending = TRUE;

      hdrsrchcmd_post( cmd );
        /* Post the command */
    }
  }
}
/*=========================================================================

FUNCTION     : HDRSRCH_SYS_LOSS_IS_PENDING

DESCRIPTION  : This function checks if there is a system loss pending 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
boolean hdrsrch_sys_loss_is_pending( void )
{
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "sysloss pending:%d", 
                  hdrsrch.sysloss_pending );

  return hdrsrch.sysloss_pending;
}

/*=========================================================================

FUNCTION     : HDRSRCH_SYS_LOSS_IS_PENDING

DESCRIPTION  : This function indicates system loss has been completed

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
void hdrsrch_sys_loss_done( void )
{
  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "sysloss done" );

  hdrsrch.sysloss_pending = FALSE;
}

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
/*==========================================================================

FUNCTION HDRSRCH_LTE_NSET_UPDATE_REDIR

DESCRIPTION
  Update the LTE neighbor list for the purpose of redirecting to LTE while
  DO is in connected state

DEPENDENCIES
  None

PARAMETERS 
  LTE frequency parameters 

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrch_lte_nset_update_redir
(
  hdrsrch_lte_nset_update_redir_params_type   *lte_nset_update_redir_params
)
{

  hdrsrch_cmd_type                            *cmd;
    /* Command structure */

  hdrsrch_lte_nset_update_redir_params_type   *params;
    /* Convenience pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_FATAL, "DLR:hdrsrch_lte_nset_update_redir()" );
  cmd = hdrsrchcmd_new( HDRSRCH_LTE_NSET_UPDATE_REDIR_CMD );
    /* Command HDRSRCH_TASK to update the LTE NSET for LTE redirection */

  if ( cmd != NULL )
  {
    params = &cmd->params.lte_nset_update_redir;
      /* Convenience pointer */

    *params = *lte_nset_update_redir_params;

    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrch_lte_nset_update_redir */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */


/*===========================================================================

FUNCTION HDRSRCH_UPDATE_SUBSCRIBER_ID

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
void hdrsrch_update_subscriber_id
(
  sys_modem_as_id_e_type sid
)
{
  if ( sid == SYS_MODEM_AS_ID_1 ) 
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH," Updting SRCH subid from %d --> %d",
                    hdrsrch.subscription_id, sid );

    hdrsrch.subscription_id = sid;
      /* HDR can be in either subscription ID 1 or 2 */
  }
  else if ( sid == SYS_MODEM_AS_ID_2 )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH," Updting SRCH subid from %d --> %d",
                    hdrsrch.subscription_id, sid );

    hdrsrch.subscription_id = sid;
  }
  else 
  {
    hdrsrch.subscription_id = SYS_MODEM_AS_ID_1; 
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, 
                    "Unexpected HDR subscriber_id received=%d, default to %d",sid,hdrsrch.subscription_id );
  }

  rfm_hdr_set_subscriber_id( sid ); 

}

/*===========================================================================

FUNCTION HDRSRCH_SET_FORCE_LTA

DESCRIPTION
  This function sets the force LTA in traffic. 

DEPENDENCIES
  None

PARAMETERS
  value - if force LTA needs to be enabled or not

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrch_set_force_lta
(
  boolean value
)
{
  hdrsrchtc_set_force_lta(value); 
}

/*==========================================================================

FUNCTION HDRSRCH_GET_SUBSCRIBER_ID

DESCRIPTION
  Retrieves the HDR subscription ID as HDR can be in either sub 1 or sub 2 (refer to enum type)

DEPENDENCIES 
  Called after hdrsrch_update_subscriber_id() is called to initialize subscriber ID 

PARAMETERS
  None

RETURN VALUE
  HDR subscription ID
 
SIDE EFFECTS
  None

==========================================================================*/
sys_modem_as_id_e_type hdrsrch_get_subscriber_id ( void )
   
{
   return ( hdrsrch.subscription_id );
    /* HDR subscription ID as HDR can be in either sub 1 or sub 2 (refer to enum type) */

}

