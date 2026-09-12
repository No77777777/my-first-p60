
/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           H D R   S E A R C H   U T I L I T Y   F U N C T I O N S

GENERAL DESCRIPTION
  This module provides general purpose routines used in various states.


EXTERNALIZED FUNCTIONS (Global)


EXTERNALIZED FUNCTIONS (Regional)

  hdrsrchutil_init

  hdrsrchutil_program_search_set
  hdrsrchutil_program_searches
  hdrsrchutil_enable_roll
  hdrsrchutil_disable_roll  

  hdrsrchutil_suspend_rpt
  hdrsrchacq_convert_ppm_to_hz
  hdrsrchutil_set_cgps_status
  hdrsrchutil_get_cgps_status
  hdrsrchutil_send_cgps_ack

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchutil.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
12/12/2016   svu     Fixed Kolcwork issues
05/27/2016   vko     QSH framework for events capturing
05/16/2016   vko     Reduce excessive F3
12/21/2015   rmv     Set flag for system lost pending 
12/16/2015   vko     Fixed KW issues
12/07/2015   wsh     Block all but deactivate cmd when sys loss is pending
11/06/2015   vke     Added FIR filter to filter SINR
10/27/2015   vke     Moved IIR_FILT_N macro definition to header file
10/04/2015   mbs/wsh Fixed going into D2L even after TA is requested
08/10/2015   vko     Removed banned API memcpy
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
06/09/2015   tnp     Update feature flag configuration
06/08/2015   vko     Removed redundant RF chain release in system lost 
05/15/2015   vko/vke Added TABASCO to featurization
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/01/2015   mbs     Supported RF interface change
02/13/2015   vke     Promotion for CR790412
02/04/2015   vke     Changes to ignore sync cmd if processing delay > SCC
12/08/2014   vke     Changes to remove rssi filtering based on ecio
11/04/2014   arm     Band avoidance: move power info saving to hdrsrch task and
                     enable BA only for DSDA mode.
11/04/2014   wsh     Disabled full PN search for JO for all except IRAT/REACQ
10/13/2013   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
10/07/2014   rmv     For Bringup of JO hw at reduced clks, limit the max fingers
10/01/2014   wsh     Added Full PN search for reacq and IRAT searches
08/22/2014   vke     Cancel the searches in ASDiv processing only 
                     when Searches are active
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
06/24/2014   sat     Fixed KW errors
06/13/2014   vlc     Added support for Enhanced DLNA. 
06/06/2014   vlc     Moved call to hdrsrchutil_update_data_ind_sinr() after 
                     finger assignment, changed reporting conditions.
05/13/2014   vlc     Modified power strobe GPIO API.
05/12/2014   vke     Klockworks fix
05/05/2014   ljl     Added preferred channel when purging pilots.
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
02/13/2014   ljl     Supported handoff override after tuneaway.
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
11/18/2013   vke     HDR enters sync suspend state for unlock immediate and 
                     unlock required notifications
10/29/2013   sat     Reduced F3 messages (or/and) ASSERTs
10/15/2013   arm     Updated atomic search F3 to be non DSDA specific.
09/25/2013   wsh     Changes to support BOLT bring up
09/04/2013   vke     klockworks fix, limit the array index to max
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/30/2013   dsp     Removed mcdo_check() from NPA featurization.
08/06/2013   rmg     Added support to convey power down reason in reverse link
                     power down command.
08/01/2012   vke     Fixed klockworks error, changed the length of memset 
07/31/2013   sat     Mainlining Modem Statistics Feature
07/16/2013   wsh     Disable QPCH upon access 
06/26/2013   wsh     Added Selective TxD feature
05/31/2013   vko     Fixed klocwork issue
05/24/2013   rmg     Added D-LNA support.
05/24/2013   smd     Reverted KW fixes.
04/22/2013   arm     Added changes for DSDA coexistence manager. 
05/16/2013   vko     Fixed klocwork issues
05/06/2013   mbs     Fixed compiler errors for BOLT build
05/02/2013   ljl     Saved scc offset and rate.
03/18/2013   arm     Used Pilot trk srch mode when no reacq rxd.
02/16/2013   arm     Do default search if enabling div for reacq timeouts
                     or current rtc and pilot pos are not within 1 ms.
02/11/2013   rkc     Handle wraparound conditions for 426ms cycles and slots.
01/10/2013   vlc     Set search mode accordingly before submitting search list. 
12/20/2012   vko     Fixed compiler warnings
12/14/2012   vko     Fixed klockwork issues
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
11/07/2012   rkc     Fixed SYNC time checking.
10/05/2012   ukl     Resolved compilation errors after removing FEATURE_BCMCS.
09/27/2012   ljl     Removed 288 chips offset for fast AGC. 
09/05/2012   wsh     Fixed incorrect NLB threshold
08/14/2012   wsh     Reparse LoadInfo cache when ASET is updated
07/09/2012   dsp     Removed unnecessary F3s. 
06/26/2012   vlc     Fixed gCSET parameters to get accurate pilot energy.
04/27/2012   wsh     Merged NLB support (several fixes)
04/20/2012   arm     Supported diversity in reacq.  
04/17/2012   ljl     Added critical section for aset update.
04/16/2012   rkc     Fixed dropping subset of CC carrier in drop former.
03/22/2012   rkc     Dropped all former ASET's on HHO.
03/19/2012   vlc     Fixed pilot purging to work with gRICE. 
03/16/2012   arm     Resolve TCB errors for Dime. 
03/08/2012   wsh     Added network load balancing support
03/08/2012   rkc     Saved last ASET[0] during TCA cmd processing.
02/27/2012   smd     Added updates of FTM msgr interface changes. 
02/27/2012   ljl     Increased TX shutdown time to 100ms.
02/27/2012   ljl     Increased TX shutdown time to 100ms.
02/15/2012   rkc     Saved last ASET[0] during TCA cmd processing.
02/03/2012   wsh     Added support for SVLTE+SHDR 
02/02/2012   arm     Changed idle div to record only max 255 psinr for any
                     idle wakeup.
02/02/2012   arm     Change default values for Idle Div thresholds.
01/25/2012   kss     Cleaned up compiler warnings.
01/25/2012   ljl     Updated RSSI and Ec/Io at search dump.
01/25/2012   grl     Update gRice rank for all ASP changes.
01/10/2012   vlc     Added check for traffic state when assigning equalizer.
12/22/2011   vlc     Removed chatty debug message. 
12/09/2011   arm     Change default SINR to -27.
12/07/2011   ljl     Added hdrsrchutil_convert_linear_sinr_to_db().
11/30/2011   vlc/lyl Added synchronization of ASP config and ASP ranking during
                     traffic for gRICE. 
11/07/2011   vlc     Fixed Klocwork warnings. 
10/28/2011   vlc/lyl Added MC gRice /cset promotion support.
10/25/2011   ljl     Handled the failure cases during optimized HO.
10/24/2011   wsh     Updated cancel FPD procedure 
10/21/2011   ljl     Increased tx powerdown wait time to 50ms. 
10/17/2011   rmg     Removed featurization around MCPM calls.
09/29/2011   lyl     Set valid ASP snr rank during traffic. 
09/28/2011   wsh     Temp. workaround to ensure FPD value is correct
09/26/2011   grl     Disable FPD before deactivating the decoder. 
09/07/2011   lyl     Set ASP SnrRank during ASP update. 
08/25/2011   smd     Used get_pilot_meas_rup_sect() for pilot measurement.
08/04/2011   ljl     Deassigned fingers at idle HO. 
08/01/2011   ssu     Added FTM support in hdrsrchutil_get_ref_sector_ptr().
05/05/2011   cnx     Fixed compiler error.
05/02/2011   kss     Fixed compiler warning.
04/26/2011   ljl     Fixed KW warning.
04/27/2011   rmg     Added HDR MCPM support changes. 
04/28/2011   kss     Removed halt code reads.
04/27/2011   kss     Added RF include to remove warnings. 
04/27/2011   smd     IRAT L2DO updates for NikeL.
04/25/2011   ljl     Supported TX powerdown callback.
04/12/2011   smd     Used srch4 search struture. 
03/24/2011   kss     Updated RF SVDO API support.
03/23/2011   vlc     Reset LTE measusrement state for HOs. 
03/03/2011   arm     Fixed NPA crash due to overlapping NPA calls. 
01/10/2011   ljl     Checked null pointer before de-referencing aset pointer. 
01/08/2011   rkc     Supported returning SINR in 0.5 dB units. 
01/06/2011   rkc     Added support for FAC gain calculation under feature 
                     FEATURE_HDR_GRICE.
12/14/2010   ljl     Limited the max fingers and disabled div for mc on 9k.
11/20/2010   pxu     Added code to adjust reacq window size. 
10/19/2010   lyl     Added null pointer check after hdrsrchset_get_sect().
10/13/2010   kss     Updated missed SCC detection enabling for Q6.
10/05/2010   pxu     Always output pstrobe f3 message when NV item is set. 
09/15/2010   pxu     Replaced JCDMA #ifdef macros with dynamic function che 
08/07/2010   smd     Fixed win center 0 issue in aquistiion/sync.
07/21/2010   smd     Fixed subaset/aset range checking bugs. 
07/02/2010   ljl     Mainlined FEATURE_HDRSRCH_1X_PILOT_SEARCH.
06/30/2010   grl     Removed redundant searcher logging.
06/22/2010   rkc     Added NPA control for mdsp clock speed.
06/21/2010   cnx     Fixed compiler warnings.
06/17/2010   rkc     Added NPA control for SSBI clocks.
06/11/2010   smd/vlc Added debug code for 4 quarter pn circle search. 
06/03/2010   cnx     Fixed Klocwork errors.
05/04/2010   rkc     Added check to only create NPA handle if previously NULL.
04/26/2010   lyl/smd Added reverse time transfer support.
04/23/2010   rkc     Checked for NULL npa handle before destroying.
04/16/2010   lyl     Updated generic timed waiting for signal function.
04/15/2010   lyl     Increased the DEC_DEACTIVATED_WAIT_MS.
04/15/2010   ljl     Fixed trigger mode issues.
04/08/2010   rkc     Removed ASSERT check for npa handle when cancelling NPA.
03/27/2010   rkc     Removed hdrsrchafc_deactivate() call when system lost.
03/26/2010   rkc     Reduced NPA wakeup request to 280 MIPs.
03/25/2010   pxu     Removed FEATURE_7800_HDEM_WORKAROUND features and 
                             FEATURE_HDR_EQ feature checks and
                             FEATURE_HDRSRCH_RBT feature checks and
                             FEATURE_HDR_TC_OFS_ENHANCEMENT checks and
                             FEATURE_HDRSRCH_SPMF checks.
03/24/2010   ljl     Supported FEATURE_HDR_DIV_SRCH_WORKAROUND. 
03/22/2010   wsh     CMI: replaced GPIO interface with new TLMM interface
03/19/2010   smd     Assigned rtc_offset to search list for IRAT pilot meas.
03/17/2010   ljl     Fixed the index to subaset array.
03/08/2010   lyl     Added hdrsrchutil_get_txagc().
03/03/2010   rkc     Reduced NPA traffic request to 120 MIPs.
03/02/2010   lyl     Fixed KW errors.
03/02/2010   grl     Enabled SPMF
02/09/2010   rkc     Changed to new GPIO interface for power strobes.
02/09/2010   smd     Added feature of LTE to DO pilot measurement.
01/06/2010   rkc     Added NPA support for traffic and removed
                     HDRSRCHUTIL_NPA_CPU_ACTIVATED support.
11/11/2009   rkc     Added NPA support.
11/11/2009   ljl     Supported BCMCS in Rev B.
11/04/2009   rmg     Removed enabling of DRC channel from search code.
10/14/2009   kss     Removed command to FW to enable/disable DRC.
10/12/2009   grl     Implemented new conditions for deactivating the decoder.
09/28/2009   smd     Fixed KW errors.
09/16/2009   grl     Fixed SVDO RF API XO compilation issue.
09/09/2009   wsh     CMI: to support changes in clk and ts APIs
08/12/2009   lyl     Updated for decoder deactivation call back interface.
08/11/2009   etv     Used actual demod carrier instead of HDRMDSP_DEMOD_CARRIER_0
08/07/2009   lyl     Removed unexpected ASP update message during idle handoff.
07/21/2009   ljl     Removed the assert checking reference carrier.
07/16/2009   lyl     Supported CC config MSGR interface.
07/09/2009   lyl     Removed the obsoleted interface (logCarrierValid).
07/07/2009   lyl     Commented out the pending interface logCarrierValid. 
07/06/2009   ljl     Removed the MDSP cmd interface to access RTC.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
06/30/2009   lyl     Updated for generic responses process.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/28/2009   ljl     Used demod_idx as csr_idx at TC suspend state.
05/28/2009   rkc     Removed chatty reference pilot carrier F3
05/20/2009   lyl/ljl Updated for firmware MSGR interface.
05/13/2009   rkc     Added CC carrier debug msg when selecting pilot group
                     representative.
05/04/2009   kss     Minor lint fixes.
04/24/2009   lyl     Fixed the search list parameter for pri chain search.
04/23/2009   rkc     Fixed CC carrier index after subaset integration.
04/21/2009   etv     Updated hdrdec APIs and removed hdrdec_pre_deactivate.
04/19/2009   ljl     Mainlined for T_MSM6800.
04/16/2009   rkc     Fixed check in looking for non-reportable pilot.
04/16/2009   rkc     Mainlined T_MSM6800 and removed T_QSC60X5 from mainline.
                     Used renamed HWIO_MDSP_UP_IRQ_VECTOR_OUT() on Poseidon2.
04/15/2009   rkc     Fixed indexing problem when dropping CC carrier ASETs and
                     renamed loop variables in drop former aset function.
04/14/2009   ljl     Fixed lint error.
04/03/2009   rkc     Updated serving sector's PGID in IDLE state from SPM.
04/01/2009   ljl     Fixed compile error for FEATURE_HDR_ALLOW_SPIBI_HANDOFFS.
03/30/2009   rkc     Purged using all subasets' ASET pilots in nset update.
                     Updated selected reportable subaset index in integrate.
                     Used renamed hdrsrchset_pilot_group_purge_mc() function
                     Fixed CC carrier index when dropping pilots and integrate.
03/25/2009   rkc     Fixed loop to drop ASET pilots from the bottom up.
03/20/2009   rkc     Renamed hdrsrchutil_appoint_pilot_report_deputy() to
                     hdrsrchutil_select_reportable_pilots().
03/20/2009   rkc     Deassign fingers if ASET is added to RSET.
03/20/2009   rkc     Added rup_subtype param to hdrsrchutil_update_nset_pilots().
03/20/2009   rkc     Changed to use reportable pilot and handling
                     of pilot group ID.
03/11/2009   rkc     Added hdrsrch_wait_for_sigs() and RF SVDO changes.
03/09/2009   smd     Added support for pilot measurement velcro solution.
03/04/2009   etv     Clear carrierValid for subasets removed from TCA
03/06/2009   pmk     Added support for QTF
02/04/2009   lyl     Fixed the lint errors.
02/02/2009   smd     Included NSET pilots when count strong pilots for SCI <= 6. 
01/26/2009   lyl     Logged the number of reacq search pilots in enh_sleep_info.
01/09/2009   etv     Informed DSP of carriers that are valid for logging.
12/16/2008   ljl     Fixed the non-persistent demod index issue.
12/09/2008   rkc     Fixed lint errors.
12/05/2008   kss     Removed/replaced intlocks.
12/01/2008   ljl     Checked NULL pointer in hdrsrchutil_get_ecio_and_io().
11/25/2008   lyl     Fixed the return value of asp_idx.
11/13/2008   ljl     Fixed the logging issue for mc pilot set/search packets.
11/03/2008   grl     Moved ARDA algorithm to hdrsrchrf module.
10/30/2008   smd     Fixed a typo in define FEATURE_HDR_REVB.
10/13/2008   lyl     Fixed lint errors.
10/02/2008   smd     Added improvement for emergency RAHO.
09/25/2008   rkc     Changed pilot sets info logging after pilot set management
09/25/2008   rkc     Fixed double assignment of res_ptr
09/23/2008   rkc     Used srch_list's res_ptr instead of resbuf.
09/15/2008   ljl     Fixed multi-carrier pilot report issue.
09/08/2008   ljl     Featurized for hdem workaround on 7800.
08/29/2008   lyl     Supported primary chain ASET search.
08/19/2008   smd     Added idle handoff improvement.
08/10/2008   lyl     Added break in loop function to avoid null ptr dereference.
08/08/2008   rkc     Validated and processed srch results per carrier.
08/04/2008   ljl     Fixed merge errors.
07/25/2008   rkc     Made sure duplicate NSET pilot is not added to srch list.
07/25/2008   ljl     Changed hdrsrchset_check_if_ofreq_chan() to
                     hdrsrchset_is_ofreq_chan().
                     Fixed the prioritized RUP set.
07/11/2008   ljl     Removed hdrsrchutil_diversity_init().
07/10/2008   rkc     Used existing pilot group sector's win center for new ASET
                     sector from TCA, if possible.
07/07/2008   ljl     Supported 1x handdown in Rev B.
06/18/2008   ljl     Reset tca_idx when entering idle from traffic.
06/11/2008   cc      Added Aux DRCCover support.
06/06/2008   rkc     Used CC Carrier instead of index 0 carrier to srch RSET.
05/09/2008   rkc     Fixed compiler warnings.
04/28/2007   ljl     Added support for switching div modes dynamically
04/22/2008   ljl     No AFC deactivate if system lost at sleep state.
04/17/2008   rkc     Added functions to set and get cgps status.
04/09/2008   ljl     Added ASP activate and deactivate.
04/07/2008   ljl     Fixed the size of rup request sector count.
03/27/2008   ljl     Moved the sectors on the avoided channels to empty set..
02/23/2003   jyw     Fixed the logic in updating persistent subasets.
02/21/2008   ljl     Preferred the same subnet handoff.
02/22/2008   jyw     Enforced the handoff control locked mode to firmware.
02/20/2008   ljl     Removed the parameter from hdrsrchutil_get_hdr_rssi().
02/08/2008   jyw     Fixed the bug in the demod index assignments.
02/01/2008   jyw     Kept the cc subaset at the TC to idle transition.
01/30/2008   jyw     Fixed the bug in the carrier drop.
01/30/2008   jyw     Removed the repetition in aset commit function.
01/30/2008   jyw     Included demod index in the RF interfces.
01/30/2008   jyw     Finalized the TC to idle transition. 
01/16/2008   ljl     Removed the code to configure predictor to DSP control 
                     when updating aset.
01/03/2008   ljl     - Fixed demod carrier and asp indices in 
                       hdrsrchutil_get_ref_sector_ptr().
                     - Deassigned fingers and updated demod carrier mask
                       when entering idle state from traffic state.
01/02/2008   ljl     Enabled equalizer.
12/26/2007   jyw     Added the support to UT platform.
12/17/2007   jyw     Added function hdrsrchutil_get_ref_sector_ptr().
12/17/2007   grl     Updated Nset/Rset window centers.12/18/2007   ljl     Enabled predictor and removed DRC override.
                     Updated for equalizer.
12/05/2007   ljl     Removed hacks for DSC and ACK and changed some 
                     messages to low.
12/03/2007   ljl     Added hacks to override DSC and ACK.
11/30/2007   jyw     Added an event_id into hdrsrchutil_pwr_strb for ETM purpose.
11/26/2007   jyw     Added a log message indicating MPC rtc i/f errors.
11/26/2007   ljl     Configured carrier of ASP structure and overrode DRC 
                     for MDM7800 bringup.
11/20/2007   rmg     Removed compilation errors on 7800.
11/16/2007   ljl     Configured RL MAC index and RAB index.
11/14/2007   ljl     Override DRC for MDM7800 bringup.
11/14/2007   jyw     Added hdrsrchutil_read_rtc and hdrsrchutil_restore_rtc.
11/13/2007   jyw     Added the duration info in the pwr strobe logs.
11/13/2007   rkc     Included hdrsrchafc.h to fix compiler warnings.
11/11/2007   ljl     Configured pn to ASP.
11/11/2007   ljl     Updated for MDM7800 bringup.
11/07/2007   jyw     Used GPIO31 for the power profiling on sc2x.
10/17/2007   jyw     Merged Rev B changes from branch.
10/05/2007   rkc     Deassigned all fingers before deactivating decoder
10/04/2007   rkc     Resolved Lint High warnings.
09/26/2007   mt      Fixed adaptive diversity featurization.
09/20/2007   mt      Adaptive diversity control feature.
09/05/2007   grl     Set mac indexes to be invalid during idle.
08/23/2007   grl     Added SPIBI HO support.
08/19/2007   grl     Resolved Lint High warnings.
08/01/2007   grl     Fixed RCVT 2.2 compiler warnings.
07/31/2007   rkc     Check if TCA is disjoint from current active pilot.
07/20/2007   mt      Fixed reset OPMODE timer issue.
06/19/2007   rmg     Added F3 msg to print DeltaT2P.
06/20/2007   mt      Remove RUR unit test code.
06/18/2007   rkc     hdrsrchdrv_send_srch_list() has new freq_offset param=0.
                     Added hdrsrchacq_convert_ppm_to_hz()
04/10/2007   mt      Enhanced OFS, RUP request and ATOM changes.
                     Added parameter to hdrsrchutil_submit_search_list().
03/09/2007   rkc     Fixed lint warnings/errors.
02/07/2007   rmg     Added support for TxT2P adjust (DeltaT2P).
11/09/2006   kss     Init FRAB for disjoint set to 0.
10/27/2006   mt      Added hdrsrchutil_ofs_count_strong_pilots().
10/17/2006   ljl     Added HDR SRCH stm.
10/04/2006   mt      Updated hdrsrchutil_enable_missed_scc_detection().
09/25/2006   grl     Fixed compilation warning.
09/07/2006   jyw     Added NV control to the power strobes.
09/07/2006   ljl     Removed hdrsrchutil_fast_dec_deactivate().
08/23/2006   grl     Added missed preamble isr reging API.
08/08/2006   mt      Added hdrsrchutil_get_agc_rtcoffset() function.
08/08/2006   sq      Moved #define to header file
08/01/2006   grl     Perform fast decoder deactivation only when sleeping.
07/28/2006   mt      Corrected ungraceful decoder deactivation check.
07/28/2006   jyw     Fixed the bug in enabling the power strobes.
07/19/2006   jyw     Added hdrsrchutil_force_wincenter_to_mstr.
06/28/2006   jyw     Added support to the MPC.
07/19/2006   kss     Featurized ungraceful decoder deactivation check.
07/18/2006   grl     Added ungraceful decoder deactivation capability.
06/30/2006   etv     Supported DSP Handoff algorithm changes.
06/14/2006   grl     Fixed compile error due to MDSP register removal.
06/07/2006   ljl     Added SBHO optimization.
06/01/2006   grl     Fixed tune away calc func to handle canceled tune aways.
04/20/2006   ljl     Added a parameter to hdrsrchafc_stop_fll_tracking().
04/20/2006   ljl     Fixed the problem that HO to the current aset pilot 
                     causes empty aset.
04/12/2006   ljl     Fixed the problem that the last aset pilot is not removed
                     when changing channel
04/06/2006   jyw     Added feature ROBN.
03/30/2006   kss     Allow BC scheduling to occur in REACQ state.
03/23/2006   ljl     Don't empty ASET and CSET at system loss
01/23/2006   ljl     Removed deassigning fingers from system loss
01/20/2006   ljl     Abort search and deassign all fingers at system loss
12/15/2005   ljl     Reset NSET srch index and OFS srch index if out of range
12/12/2005   sq      Avoid read fault on L4 build
11/08/2005   ds      Stop FLL tracking and update RGS during system loss.
10/27/2005   ljl     Removed all aset pilots during idle handoff
10/25/2005   ljl     Removed compile warning
10/21/2005   ljl     Checked NULL pointer dereference
10/04/2005   ljl     Fixed compile warnings
09/23/2005   ljl     Added support of RA channel gain
09/16/2005   sq      Leave schedule timer running in FTM mode at system loss
09/13/2005   ljl     Initialized RAB related registers at aset update
09/10/2005   ds      Added diversity settings update for rotators
09/06/2005   ljl     Saved cached sector info
09/06/2005   ljl     Added optimization after system loss
09/01/2005   grl     Added tune away time calculation function.
08/12/2005   sq      Fixed issue in selecting strongest NSET pilots 
07/28/2005   ds      Added Same PN multiple frequency (SPMF) support
07/27/2005   ljl     Kept NSET after system loss
07/25/2005   ljl     Added broadcast and directed nset update 
06/30/2005   sq      Added hdrsrchutil_check_ofs_condition
06/28/2005   jyw     Added probes for HDR sleep/power time line profiling
06/01/2005   ljl     Added DSC support.
05/31/2005   grl     Get channel info from hdrsrchrf instead of hdrsrchdrv..
05/12/2005   ds      Added round-robin OFS support
04/25/2005   sq      Added FEATURE_HDR_TC_OFS_ENHANCEMENT
04/18/2005   ds      Added support for equalizers under FEATURE_HDR_EQ
03/23/2005   hrk     Added function hdrsrchutil_dec_deactivate.
02/01/2005   sq      Added function hdrsrchutil_get_sinr_and_rxagc
01/27/2005   sq      Added support for MSM6800
01/25/2005   sq      Added debug feature DEBUG_HDR_SAMPLE_RAM_DUMP
01/14/2005   sq      Adjust srch window position when srch window size changes
01/05/2005   sq      Added more comments
01/04/2005   sq      Mainlined FEATURE_HDR_TC_OFS
12/02/2004   sq      Enhanced DSP halt work-around. Checked more halt codes
11/29/2004   grl     Added support for connected state stat logging.
11/19/2004   sq      Changed the OFS governing rate from 6 to 5 seconds
11/09/2004   sq      Added function hdrsrchutil_reset_srch_filters
11/04/2004   kss     Added hdrsrchutil_handle_pn_roll() with broadcast support.
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
08/02/2004   sq      Print hstr value as mod 512 in addition to the raw value
07/30/2004   sq      Support for FEATURE_HDR_BCMCS. Internal API changes.
06/21/2004   sq      Need to update srch win size info in sector structure
05/05/2004   aaj     Implemented DSP halt work-around to halt on known codes
04/05/2004   sq      Returns Ec/Io and Io for HDR
04/05/2004   aaj     Added typedef for color code type
03/04/2004   aaj     Support for minimizing subnet handoffs 
01/21/2004   aaj     Fixed a problem with validating of searcher energies
01/06/2004   aaj     Pass back the A/C/N energies in pilot metric utility func.
11/26/2003   aaj     Handle Lost Dump 
11/21/2003   sq      Added start/stop timer functions
11/18/2003   aaj     Created a separate function to get pilot metric
11/12/2003   sq      Print PN roll count in OFS as un-signed integer
10/21/2003   aaj     Fixed compilation warning for hdrsrchutil_tc_ofs_start_gov
10/14/2003   aaj     Added validation of searcher results to avoid bogus PNs
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/10/2003   mpa     Converted F3 messages to use MSG2.0 SSID
09/16/2003   sq      Added support for FEATURE_HDR_TC_OFS
07/22/2003   sq      Corrected search window position for MSM6500
06/05/2003   aaj     Do not initialize drop timer expired to FALSE on TC->IDLE
05/21/2003   sq      Write sector update info independent of DRC Cover change
05/16/2003   aaj     Write bestAspFL after raho/iho to speed up handoff process
04/04/2003   aaj     Fixed window center checking. Added cast operator
03/11/2003   aaj     Invalidate the DSP SCC offset before idle handoff
03/11/2003   ajn     Added hdrsrchutil_limit_window_size( ) function
03/11/2003   ajn     Updated Copyright notice
02/26/2003   aaj     Replace linkID with hstr when the idle handoff happens
02/23/2003   aaj     Added more checks to debug "Huge WinCenter" error message
12/19/2002   ajn     Added system-lost reason to SysLost callback
11/28/2002   aaj     Added support for data level indicator
11/25/2002   ajn     Added suspend report utility function
11/15/2002   aaj     Don't center windows around win_cenx2 if MSTR is invalid
11/08/2002   aaj     Deleted unused program quick searches and fill entry fn.s
11/07/2002   ajn     Roll handler moved to hdrts.
09/18/2002   aaj     Support for multipath window centering for HDR
09/18/2002   aaj     Added support for HDR RSSI for displaying signal bars
08/13/2002   ajn     Featurized System Time Check
08/09/2002   ajn     Added System Time Check/Resync
08/06/2002   aaj     Stop searcher scheduler when declaring system loss
08/02/2002   aaj     Set dump sig if DSP halts and HDRSRCH is waiting for dump
06/12/2002   aaj     Kill sched timer upon DSP reset. Print DSP's HW error code
06/05/2002   aaj     Fix a problem in sending off-freq neighbor searches
05/21/2002   aaj     Change system lost message to msg fatal
05/15/2002   ajn     Reset active_srch if MDSP halts.
                     Update RPC Cell indexes for all ASPs on ASET update.
04/17/2002   aaj     Implemented RAB Length table for TCA message
03/29/2002   aaj     Moved lost dump timer setting to drivers routine
03/28/2002   aaj     Enable support for recovering from DSP halts
03/23/2002   aaj     Fixed aset update command where RPC Cell map was not
                     written to the ASP structure
03/21/2002   aaj     Round mstrx2 to nearest pos when converting from x8->x2
03/06/2002   sst     Added earliest peak position mechanism
02/25/2002   aaj     Do not explicitly deactivate hdrsrch during system loss
02/21/2002   ajn     Altered signature on various search_list functions.
02/19/2002   sst     Added functions to handle filling of search list
02/19/2002   aaj     support for RPC Cell map in MDSP's ASP struct
02/19/2002   sst     Modified hdrsrchutil_prepare_reacq_list() to support
                      multiple reacq searches
02/16/2002   aaj     Increase reacquisition search window size temporarily
01/30/2002   ajn     Force unique pilot PNs.
01/17/2002   aaj     support for RF band to the hdr sector structure type
12/18/2001   aaj     Added general purpose stop watch mechanism
12/07/2001   ajn     Utilized new function hdrsrchset_drop_from_aset( )
12/06/2001   ajn     Implemented NSet pilot aging [6.6.5.3.6]
12/05/2001   aaj     Print Idle handoff message
12/03/2001   aaj     Set lost dump timer to 25ms instead of 30ms
11/07/2001   ajn     Added support for prepearing reacq list using a/c/n sets
11/01/2001   aaj     Set active set channel during idle handoff to OFS pilot
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
10/08/2001   ajn     Added hdrsrchutil_system_lost().
09/14/2001   aaj     Added PN ROLL registeration/deregistration mechanisms
09/13/2001   aaj     Added callback to set handoff delay params from FMAC
09/13/2001   aaj     Added RABLen and RABBuffer support
09/05/2001   aaj     Antenna Diversity integrated in the idle state
08/21/2001   aaj     Moved hdrsrchutil_get_sector_report to hdrsrchset.h
08/09/2001   ajn     Decrease ReAcq window minimum size to avoid DSP problems.
07/31/2001   aaj     Added function to handle MDSP halts. diversity support.
06/06/2001   ajn     Added LinkID to active pilot set map.
04/24/2001   aaj     Migration to TAZ3 drivers
03/26/2001   aaj     Consolidated finger variables into hdrsrchfing
03/08/2001   aaj     Consolidated search packet logging
03/02/2001   aaj     included customer.h file
03/02/2001   aaj     External control for antenna diversity
02/06/2001   aaj     Gen TA Support. Moved log funs to hdrsrchlog.c
02/01/2001   aaj     Set the new pilot ASP as the bestASP after idle handoff.
01/30/2001   aaj     Added res_ptr initialization, logging support etc etc
01/26/2001   dna     hdrsrchset.aset_cnt -> hdrsrchset.aset_cnt - 1 (2 places)
01/05/2001   ajn     TC -> IDLE ASET transition.
12/01/2000   ajn     Sector structure reorg / Window size management
10/19/2000   ajn     TCA will now forward new sector parameters to fingers
                     currently assigned to a current aset pilot
10/11/2000   ajn     Added aset_idle_handoff func
10/09/2000   ajn     Added aset/nset pilot management functions
10/06/2000   aaj     Added new report count to the manage sets function
10/05/2000   ajn     Created module

==========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "sys.h"

#ifdef FEATURE_CMI
#include "sys_v.h"
#endif
#include "time_svc.h"


#include "hdrsrchutil.h"
#include "hdrsrch.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchdrv.h"
#include "hdrsrchfing.h"
#include "hdrsrchtri.h"
#include "hdrsrchset.h"
#include "hdrmdspmcr.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"
#include "hdrsrchrf.h"
#include "hdrsrchafc.h"
#include "hdrsrchmsg.h"
#include "hdrsrcheq.h"
#include "hdrts.h"

#include "rfm_common.h"
#include "rfm_cdma.h"

#include "hdrmdsp.h"
#include "hdrsrchlog.h"
#include "hdrdebug.h"
#include "err.h"
#include "hdrdec.h"
#include "hdrrx.h"
#include "hdrmc_v.h"

#ifdef FEATURE_HDR_BCMCS
#include "hdrbcmac.h"
#endif /* FEATURE_HDR_BCMCS */

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm.h"
#endif


#ifdef FEATURE_HDR_REVB
#include "hdrsrchschdgrp.h"
#include "hdrfmac.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_CGPS_EVDO_IF
#include "cgps_api.h"
#endif /* FEATURE_CGPS_EVDO_IF */

#if (defined FEATURE_NPA) || (defined FEATURE_HDR_JOLOKIA_MODEM_BRINGUP)
#include "npa.h"
#endif /* FEATURE_NPA || FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */

#ifdef FEATURE_HDRSRCH_POWER_PROFILE
#include "DDITlmm.h"
#endif /* FEATURE_HDRSRCH_POWER_PROFILE */

#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrsrchmcpm.h"
#include "mcpm_api.h"

#include "hdr_log_qsh.h"
#include "hdrutil.h"

extern void hdrlmac_pilot_rpt_cb ( void );

#define HDRSRCHUTIL_MAX_WIN_SIZE_X1 1024

#ifdef FEATURE_HDR_RUMI
volatile boolean hdrsrchutil_full_pn_search = FALSE;
  /* Whether Full PN search is used. */
#endif /* FEATURE_HDR_RUMI */

volatile uint16 hdrsrchutil_full_pn_max_dist = HDRSRCHUTIL_MAX_WIN_SIZE_X1 * 2;
  /* Max distance from the strongest peak within which
     the peak is consider valid (0 to allow all distance) */
/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*-----------------------*
 * PN CIRCLE WRAP MACROS *
 *-----------------------*/
#define HDRSRCHUTIL_CIRC_INCR( i, wrap ) ( (i) = ((i)+1) % (wrap) )

#define HDRSRCHUTIL_CIRC_DECR( i, wrap ) \
                            ( (i) = (((i) < 1) ? ((wrap) - 1) : ((i) - 1)))


#define HDRSRCHUTIL_MIN_REACQ_NSECT_ENG   37
  /* a NSET sector has to be at least -12dB (37) to be included in the
     reacquisition list */

/*------------------------*
 * HDR Strength Indicator *
 *------------------------*/

#define   HDR_ECIO_MIN_THRESHOLD_DB   11  
  /* -dB units i.e. -11 dB */

#define   HDR_RSSI_MIN_VALUE_DBM       118  
  /* -dBm units i.e. -118 dBm */

#define   HDR_RSSI_FILT_LEN              5
  /* Value of N for the (N-1)/N Filter Coefficients for IIR filter. */

#define  HDRSRCH_MAX_WIN_CENTER_DELTA_X2     2
  /* maximum amount by which the window center can track earliest path */

#define  HDRSRCH_MIN_WIN_CENTER_DELTA_X2    -2
  /* minimum amount by which the window center can track earliest path */

#define  HDRSRCH_MAX_MSTR_WIN_CEN_ERR_X2     452
  /* Maximum difference between MSTR and window center is these many half
     chips. This number is half of maximum search window size. */

#define HDRSRCH_OFS_INTERVAL_PN_ROLL        142
  /* This constant is in units of pn roll count. It translates to about 3.8
     seconds. OFS happens no more than once per this number of PN rolls */

/*------------------------------*
 * Tune away calculation macros *
 *------------------------------*/

/* Converts a number into a natural number ( greater or equal to zero ). */
#define HDRSRCHUTIL_NAT_NUM( num ) ( num < 0 ? 0 : num )

/* Shrinks the current tune away time based on the newly requested tune
   away time and the minimum tune away duration. */
#define HDRSRCHUTIL_SHRINK_TIME_LEFT( cur_sclks_left,                      \
                                      new_sclks_left,                      \
                                      min_sclks_left )                     \
  ( ( new_sclks_left < (int32) min_sclks_left ) ?                          \
      ( ( cur_sclks_left < (int32) min_sclks_left ) ?                      \
          cur_sclks_left : (int32) min_sclks_left ) : new_sclks_left )
      
#define HDRSRCH_SBHO_INTERVAL_MS            5000
  /* The interval between two SBHOs */


/* Parameters for AT operational mode algorithm 
   values are in units of 0.5 dB */
#define HDRSRCH_POOR_COVERAGE_PILOT_ECIO_HIGH_THRESHOLD     ( -6 * 2 )
#define HDRSRCH_POOR_COVERAGE_PILOT_ECIO_LOW_THRESHOLD      ( -8 * 2 )
#define HDRSRCH_POOR_COVERAGE_PILOT_RXPOWER_HIGH_THRESHOLD  ( -87 * 2 )
#define HDRSRCH_POOR_COVERAGE_PILOT_RXPOWER_LOW_THRESHOLD   ( -90 * 2 )

#define HDRSRCH_GOOD_COVERAGE_PILOT_ECIO_HIGH_THRESHOLD     ( -4 * 2 )

#define HDRSRCH_IDLE_DUAL_MODE_TIME_TO_TRIGGER                   25000
#define HDRSRCH_IDLE_HDR_ONLY_MODE_TIME_TO_TRIGGER               30000
#define HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE             30000

#define HDRSRCH_TC_DUAL_MODE_TIME_TO_TRIGGER                      2000
#define HDRSRCH_TC_HDR_ONLY_MODE_TIME_TO_TRIGGER                  2000

#define HDRSRCH_DUAL_MODE_TIMER_SCALE                                5
#define HDRSRCH_HDR_ONLY_MODE_TIMER_SCALE                            6

#define HDRSRCH_OPMODE_MESSAGE_LOG_INTERVAL_SCLKS                    \
           TIMETICK_SCLK_FROM_MS(1000)

#define HDRSRCH_OPMODE_INDICATION_PERIOD_SCLKS                       \
           TIMETICK_SCLK_FROM_MS(30000)

#define HDRSRCHUTIL_RUP_MAX_SECTORS                                 15

#ifdef FEATURE_HDR_REVB
/* The following macros are used to calculate the distribution of the 
   search tasks among ASET, CSET, RSET and NSET. */
#define HDRSRCH_SPEED                                              480
  /* SRCH speed 480 correlations per chip */

#define HDRSRCH_SRCH_CHIP_ALLOWANCE                                600
  /* HDRSRCH search window limits */
#endif /* FEATURE_HDR_REVB */

/*------------------------------*
 * Decoder wait time macros     *
 *------------------------------*/
/* TBD: might need to tune this timer */
#define HDRSRCH_DEC_DEACTIVATED_WAIT_MS                            20
  /* Time limit to wait for the decoder deactivated sig */

#define HDRSRCH_TX_POWERDOWN_WAIT_MS                               100
  /* Time limit to wait for tx powerdown sig */

/*------------------------------*
 * Srch validation macros       *
 *------------------------------*/

/* Total energy threshold to detect bogus PNs is based on how many
   valid pilots are included in the total energy.  One threshold
   is used for 1-4 pilots found, and a higher threshold is used
   if greater than 4 pilots found. */

#define HDRSRCHUTIL_VALIDATE_SRCH_MAX_PILOTS_USE_LO_THRESH       4
  /* Maximum number of pilots found to use low total energy threshold */

#define HDRSRCHUTIL_VALIDATE_SRCH_LO_PILOT_THRESH                1536
  /* Threshold to use if number of pilots found is <=
     HDRSRCHUTIL_VALIDATE_SRCH_MAX_PILOTS_USE_LO_THRESH */

#define HDRSRCHUTIL_VALIDATE_SRCH_HI_PILOT_THRESH                2048
  /* Threshold to use if number of pilots found is >
     HDRSRCHUTIL_VALIDATE_SRCH_MAX_PILOTS_USE_LO_THRESH */

/* Total energy threshold to detect bogus PNs is based on how many
   valid pilots are included in the total energy.  One threshold
   is used for 1-4 pilots found, and a higher threshold is used
   if greater than 4 pilots found. */
#define HDRSRCHUTIL_VALIDATE_SRCH_THRESH( num_pilots )                \
          ( ( ( num_pilots ) <=                                       \
            HDRSRCHUTIL_VALIDATE_SRCH_MAX_PILOTS_USE_LO_THRESH ) ?    \
          HDRSRCHUTIL_VALIDATE_SRCH_LO_PILOT_THRESH :                 \
          HDRSRCHUTIL_VALIDATE_SRCH_HI_PILOT_THRESH )

#define HDRSRCHUTIL_QUARTER_PN_CIRCLE_CX1_8192                   8192
  /* Size of 1 of 4 quadrants of PN circle in chipx1 units for full
     PN circle search  */

#ifdef FEATURE_HDR_REVC
#define HDRSRCHUTIL_NLB_BACKOFF_DISABLED                         0
#define HDRSRCHUTIL_NLB_BACKOFF_RATIO                            3
#define HDRSRCHUTIL_NLB_BACKOFF_THRESHOLD                       -5
#endif /* FEATURE_HDR_REVC */


#define HDRSRCHUTIL_HALFSLOTS_PER_FRAME   HDRTS_SLOTS_PER_FRAME * 2
  /* Half slots (1.67/2ms) per frame (26.67ms) */

#define HDRSRCHUTIL_HALFSLOTS_PER_CYCLE   HDRTS_SLOTS_PER_SCC * 2
  /* Half slots per 426ms cycle */

#define HDRSRCHUTIL_CYCLE_WRAPAROUND_SLOTS      48
  /* Worst case to decode last SYNC msg in 8th CC in intra-scc is 31 slots in
     the next CC Cycle (426ms).  Add one more frame for processing delay. */

#ifdef FEATURE_HDR_DLNA
#define HDRSRCHUTIL_DLNA_CTL_MSG_ENABLE_DLNA        1
  /* Set bit 0 of the DLNA CTL message payload to enable DLNA */

#define HDRSRCHUTIL_DLNA_CTL_MSG_ENABLE_ENH_DLNA    2
  /* Set bit 1 of the DLNA CTL message payload to enable Enhanced DLNA */
#endif /* FEATURE_HDR_DLNA */

/* EJECT */
/*==========================================================================

                     HDR SEARCH UTIL DATA

==========================================================================*/

/* Search List Data */

typedef struct
{
  hdrsrchdrv_srch_list_struct_type  *list;
    /* Search list */

  hdrsrchdrv_srch_res_struct_type   *res;
    /* Search result buffer */

  uint8                             cnt;
    /* # of items in search list */

  hdrsrch_system_enum_type          system;
    /* DO or 1x pilot search to be performed */
}
hdrsrchutil_srch_struct_type;


typedef struct 
{
  sys_channel_type                  channel;
    /* The channel when system loss occured. */

  sys_pilot_pn_type                 pn_offset;
    /* The pn offset when system loss occured */
  
  uint8                             coset_grp;
    /* The coset group when system loss occured */

}
hdrsrchutil_system_loss_struct_type;

typedef struct 
{
  sys_link_type              sects[ HDRSRCH_MAX_CACHE_RECORDS ];
    /* Array for the cached sectors */

  uint8                      num_sects;
    /* The number of cached sectors */
}
hdrsrchutil_cached_sects_struct_type;

typedef struct 
{
  boolean                           a_c_sets_completed;
    /* flag that finished including A and C set PNs in RUP set */

  boolean                           rup_chan_nset_completed;
    /* flag that RUP channel N set PNs has been included in RUP set */

  boolean                           serv_chan_nset_completed;
    /* flag that serving channel N set PNs has been included in RUP set */

  boolean                           rset_completed;
    /* flag that finished including R set PNs in RUP set */
    
  uint16                            rup_chan_nset_index;
    /* index into the N set */

  uint16                            serv_chan_nset_index;
    /* index into the N set */
        
  uint16                            last_rset_pn;
    /* PN offset for the RSET pilot */
  
}
hdrsrchutil_rup_srch_struct_type;

/*----------------------*
 * HDR Search Util Data *
 *----------------------*/

typedef struct
{
  hdrsrchutil_srch_struct_type       srch;
    /* Search list */

  hdrsrchutil_stop_watch_struct_type stop_watch;
    /* to measure duration between two events using timestamps */

  uint16                             ofs_trigger_x26ms;
    /* pn roll count is served as the rate control timer for HDR OFS in 
       traffic state. It contains the pn roll count when OFS was 
       permitted the last time */

#ifdef DEBUG_HDR_SAMPLE_RAM_DUMP
  /* This debug feature allows HDR to read 2 slots worth samples from
     sample ram buffers into ARM memory */
  uint32   rx0_sample_ram[2048];
#endif /* DEBUG_HDR_SAMPLE_RAM_DUMP */

  hdrsrchutil_system_loss_struct_type system_loss;
    /* parameters related to system loss */

  hdrsrchutil_cached_sects_struct_type cached_sects;
    /* Parameters related to cached sectors */


#ifdef FEATURE_HDR_REVC
  boolean                            demod_all_carriers;
#endif /* FEATURE_HDR_REVC */

#ifdef FEATURE_HDR_DLNA
  boolean                            dlna_enabled;
    /* TRUE if DLNA is enabled */

  boolean                            enh_dlna_enabled;
    /* TRUE if Enhanced DLNA is enabled */
#endif /* FEATURE_HDR_DLNA */
#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
  uint32                             max_q6_clk_speed;
    /* MAX Q6 supported clk speed */
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */

}
hdrsrchutil_struct_type;


LOCAL hdrsrchutil_struct_type       hdrsrchutil;

hdrsrch_opmode_struct_type          hdrsrchmode;
  /* HDRSRCH Global Variables */

LOCAL hdrsrchutil_rup_srch_struct_type   hdrsrchrup;

LOCAL hdrsrch_rup_req_params_type   hdrsrch_rup_req;


#ifdef FEATURE_HDR_REVA_L1
/*--------------------------------------------------------------------------
  RA channel gain dB to linear conversion table. 
--------------------------------------------------------------------------*/

LOCAL uint16 ra_channel_gain_db_to_lin_table[ ] = 
{
  3284,  /* -6dB  */
  2325,  /* -9dB  */
  1646,  /* -12dB */
  1165   /* -15dB */
};
#endif /* FEATURE_HDR_REVA_L1 */

/* EJECT */
/*--------------------------------------------------------------------------
  Search Window Sizes
    From: Table 6.6.6.5-1. Search Window Sizes
--------------------------------------------------------------------------*/

LOCAL const uint16 hdrsrchutil_winsize[ HDRSRCH_WINSIZE_MAX_SIZE ] =
{
    4,   6,   8,  10,  14,  20,  28,  40,
   60,  80, 100, 130, 160, 226, 320, 452, 512
};


/* EJECT */
/*--------------------------------------------------------------------------
  Search Window Sizes
    From: Table 6.6.6.2-2. Encoding of RAB length Field
--------------------------------------------------------------------------*/

LOCAL const uint8 hdrsrchutil_rab_length[] = { 8, 16, 32, 64 };

/* EJECT */
/*--------------------------------------------------------------------------
                 HDR DATA LEVEL INDICATOR
                =========================
  HDR Data Level Indicator Look up table
    This look up table is used to get the data level indicator from the 
    received SINR. Note that this table is used only for displaying the
    data activity on the UI and is not related to the DRC table.
--------------------------------------------------------------------------*/

#define HDR_DATA_INDICATOR_FILT_LEN     32
  /* Filter length for data indicator IIR filter. This filtering is performed
     every search schedule which is 30ms. Therefore filter length of 33 will
     give us 1 second average */

#define HDR_FIR_FILT_LEN                32
  /* Filter length for data indicator FIR filter. */

uint16                       hdrsrchutil_data_ind_sinr = 0;
  /* Filtered best ASP SINR used to decide the data indicator */

/* SINR table.  This table must be in decending order to be searched by
   hdrsrchutil_table_search() */
LOCAL const uint16 hdrsrchutil_data_ind_tab_db[] = 
{
    32305,  /* 18.0 dB */
    28792,  /* 17.5 dB */
    25661,  /* 17.0 dB */
    22870,  /* 16.5 dB */
    20383,  /* 16.0 dB */
    18166,  /* 15.5 dB */
    16191,  /* 15.0 dB */
    14430,  /* 14.5 dB */
    12861,  /* 14.0 dB */
    11462,  /* 13.5 dB */
    10216,  /* 13.0 dB */
    9105,   /* 12.5 dB */
    8115,   /* 12.0 dB */
    7232,   /* 11.5 dB */
    6446,   /* 11.0 dB */
    5745,   /* 10.5 dB */
    5120,   /* 10.0 dB */
    4563,   /* 9.5 dB */
    4067,   /* 9.0 dB */
    3625,   /* 8.5 dB */
    3231,   /* 8.0 dB */
    2879,   /* 7.5 dB */
    2566,   /* 7.0 dB */
    2287,   /* 6.5 dB */
    2038,   /* 6.0 dB */
    1817,   /* 5.5 dB */
    1619,   /* 5.0 dB */
    1443,   /* 4.5 dB */
    1286,   /* 4.0 dB */
    1146,   /* 3.5 dB */
    1022,   /* 3.0 dB */
    910,    /* 2.5 dB */
    811,    /* 2.0 dB */
    723,    /* 1.5 dB */
    645,    /* 1.0 dB */
    574,    /* 0.5 dB */
    512,    /* -0.0 dB */
    456,      /* -0.5dB  */
    407,    /* -1.0 dB */
    362,    /* -1.5 dB */
    323,    /* -2.0 dB */
    288,    /* -2.5 dB */
    257,    /* -3.0 dB */
    229,    /* -3.5 dB */
    204,    /* -4.0 dB */
    182,    /* -4.5 dB */
    162,    /* -5.0 dB */
    144,    /* -5.5 dB */
    129,    /* -6.0 dB */
    115,    /* -6.5 dB */
    102,    /* -7.0 dB */
    91,     /* -7.5 dB */
    81,     /* -8.0 dB */
    72,     /* -8.5 dB */
    64,     /* -9.0 dB */
    57,     /* -9.5 dB */
    51,     /* -10.0 dB */
    46,     /* -10.5 dB */
    41,     /* -11.0 dB */
    36,     /* -11.5 dB */
    32,     /* -12.0 dB */
    29,     /* -12.5 dB */
    26,     /* -13.0 dB */
    23,     /* -13.5 dB */
    20,     /* -14.0 dB */
    18,     /* -14.5 dB */
    16,     /* -15.0 dB */
    14,     /* -15.5 dB */
    13,     /* -16.0 dB */
    11,     /* -16.5 dB */
    10,     /* -17.0 dB */
    9,      /* -17.5 dB */
    8,      /* -18.0 dB */
    7,      /* -18.5 dB */
    6       /* -19.0 dB */
};


LOCAL const uint16 hdrsrchutil_data_ind_tab[] = 
{ 
  64,       /* -9dB   : Max for Data Level: 0  */
  128,      /* -6dB   : Max for Data Level: 1  */
  180,      /* -4.5dB : Max for Data Level: 2  */
  256,      /* -3dB   : Max for Data Level: 3  */
  324,      /* -2dB   : Max for Data Level: 4  */
  644,      /* +1dB   : Max for Data Level: 5  */
  1024,     /* +3dB   : Max for Data Level: 6  */
  2048,     /* +6dB   : Max for Data Level: 7  */
  4096      /* +9dB   : Max for Data Level: 8  */
};


#ifdef FEATURE_NPA
/*--------------------------------------------------------------------------
   Define HDR's CPU requirements for AWAKE and ACTIVATED modes, and
   SSBI enable mode.
---------------------------------------------------------------------------*/
#define HDRSRCHUTIL_NPA_WAKEUP_MIPS           280
  /* Requested MIPS if only WAKEUP is requested. */

#define HDRSRCHUTIL_NPA_TRAFFIC_MIPS          120
  /* Requested MIPS if TRAFFIC is requested. */

#define HDRSRCHUTIL_SSBI_NPA_REQ_VAL          1
  /* SSBI NPA Request Value */
#endif /* FEATURE_NPA */


#ifdef FEATURE_HDRSRCH_POWER_PROFILE

/*--------------------------------------------------------------------------
    HDR SRCH power profiling along the HDR sleep timeline
---------------------------------------------------------------------------*/

typedef struct 
{
  boolean enable;
    /* TRUE if debug NV enables power strobing. */

  MCPM_RCM_TRIGGER                mcpm_trigger;
    /* MCPM stobe trigger */ 

  hdrsrchutil_pwr_strb_event_type curr_event_id;
    /* Current Event ID */
} hdrsrchutil_pwr_strb_type;

static hdrsrchutil_pwr_strb_type hdrsrchutil_pwr_strb = 
                               { FALSE, RCM_TRIGGER_MAX, HDRSRCH_PWR_EVENT_TCXO_ON };

#endif /* FEATURE_HDRSRCH_POWER_PROFILE */

#ifdef FEATURE_NPA
typedef struct
{
  npa_client_handle               cpu_handle;
    /* NPA client handle for CPU */

  uint32                          cpu_mask;
    /* Mask for HDR NPA requests. */

  npa_resource_state              cpu_wakeup_val;
    /* Static for debug */

  npa_resource_state              cpu_traffic_val;
    /* Static for debug */

  npa_client_handle               ssbi_handle;
    /* NPA client handle for SSBI */

  npa_client_handle               mdsp_clk_handle;
    /* NPA client handle for mdsp clock */

  hdrsrchutil_npa_mdsp_clk_type   curr_mdsp_clk_speed;
    /* Current NPA mdsp clk speed */

  int                             hdr_npa_cpu_node_debug_crtic_sect;
    /* Create our own semaphore for CPU node
       to catch overlapping NPA cases */

  int                             hdr_npa_mdsp_clk_node_debug_crtic_sect;
    /* Create our own semaphore for mdsp clk node
       to catch overlapping NPA cases */

  boolean                         hdr_npa_issue_request_from_cancel;

} hdrsrchutil_npa_type;
#endif /* FEATURE_NPA */

#ifdef FEATURE_NPA
static hdrsrchutil_npa_type      hdrsrchutil_npa = { NULL,
                                                     0,
                                                     HDRSRCHUTIL_NPA_WAKEUP_MIPS,
                                                     HDRSRCHUTIL_NPA_TRAFFIC_MIPS,
                                                     NULL,
                                                     NULL,
                                                     0,
                                                     0,
                                                     0,
                                                     FALSE };
  /* HDR NPA data structure */
#endif /* FEATURE_NPA */

/*==========================================================================

                     HDR SEARCH UTIL FUNCTION PROTOTYPES

==========================================================================*/

void hdrsrchutil_update_max_ecio( void );

void hdrsrchutil_update_rssi( void );

/* EJECT */
/*===========================================================================

                      FUNCTION  DEFINITIONS

===========================================================================*/



/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GET_WINSIZE

DESCRIPTION  : This function determines the window size for a pilot sector

DEPENDENCIES : None

RETURN VALUE : Window size in chips

SIDE EFFECTS : None

=========================================================================*/

uint2 hdrsrchutil_get_winsize
(
  hdrsrch_sect_struct_type        *sect
    /* sector we need a search window size for */
)
{
  hdrsrch_winsize_enum_type       winsize;
    /* Window Size parameter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Determine Window Size to use */

  switch ( sect->set )
  {
    case HDR_ASET:
    case HDR_CSET:
    case HDR_GHOST_GSET:  
    {
      winsize = hdrsrch.srch_params.aset_win_size;
    }
    break;

    case HDR_RUPSET:
      /* fall through to NSET processing */
      
    case HDR_NSET:
    {
      if ( sect->nset.win_size == HDRSRCH_WINSIZE_DEFAULT )
      {
        winsize = hdrsrch.srch_params.nset_win_size;
      }
      else
      {
        winsize = sect->nset.win_size;
      }
    }
    break;

    default:
    {
      winsize = hdrsrch.srch_params.rset_win_size;
    }
  } /* switch (sect->set) */

  return hdrsrchutil_winsize[ winsize ];

}/* hdrsrchutil_winsize */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GET_WINSIZE_X1

DESCRIPTION  : This function translates the given winsize in enum to 
               the window size in chipx1

DEPENDENCIES : None

RETURN VALUE : Window offset in chips

SIDE EFFECTS : None

=========================================================================*/
uint16 hdrsrchutil_get_winsize_x1
(
  hdrsrch_winsize_enum_type       winsize
  /* Window Size enum */
)
{
  return hdrsrchutil_winsize[winsize];
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GET_WIN_OFFSET

DESCRIPTION  : This function determines the window offset given the window
                size and the SearchWindowOffset value [6.6.6.2-4]

DEPENDENCIES : None

RETURN VALUE : Window offset in chips

SIDE EFFECTS : None

=========================================================================*/

int16 hdrsrchutil_get_win_offset
(
  hdrsrch_sect_struct_type        *sect
    /* sector we need a search window offset for */
)
{
  int16                       offset, win_size; 
    /* local variables for convenience */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( sect->set != HDR_NSET ) && ( sect->set != HDR_RUPSET ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,"No win_offset for non-NSET pilots PN=%d",
                    sect->pn_offset );

    return 0;
  }

  win_size = sect->win_sizex1;
   /* Determine the sector window size to use */

  switch ( sect->nset.win_offset )
  {
    case   HDRSRCH_WINOFFSET_NONE:
      offset = 0;
      break;

    case   HDRSRCH_WINOFFSET_PLUS_50_PERCENT:
      offset = win_size / 2;
      break;

    case   HDRSRCH_WINOFFSET_PLUS_100_PERCENT:
      offset = win_size;
      break;

    case   HDRSRCH_WINOFFSET_PLUS_150_PERCENT:
      offset = (3 * win_size) / 2;
      break;

    case   HDRSRCH_WINOFFSET_MINUS_50_PERCENT:
      offset = - (win_size / 2);
      break;

    case   HDRSRCH_WINOFFSET_MINUS_100_PERCENT:
      offset = - win_size;
      break;

    case   HDRSRCH_WINOFFSET_MINUS_150_PERCENT:
      offset = - (3 * win_size) / 2;
      break;


    default:
      offset = 0;
      break;

  } /* switch (win_offset) */

  return offset;

}/* hdrsrchutil_get_win_offset */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_INIT_SEARCH_LIST

DESCRIPTION  : This function prepares for the creation of a search list

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : winsizex1 is filled in with the search window size
               for the programmed sectors.

=========================================================================*/

void hdrsrchutil_init_search_list( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.srch.list = hdrsrchdrv.srch_list;
  hdrsrchutil.srch.res  = hdrsrchdrv.resbuf;
    /* Use the hdrsrchdrv buffers */

  hdrsrchutil.srch.cnt  = 0;
    /* No searches yet */

} /* hdrsrchutil_init_search_list( ) */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_TO_SEARCH_LIST

DESCRIPTION  : This function adds a sector to the search list.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : winsizex1 is filled in with the search window size
               for the programmed sector.
               win_cenx2 is saved for NSET and RSET pilots since
               they are generated in real-time by this function.

=========================================================================*/

void hdrsrchutil_add_to_search_list
(
  hdrsrch_sect_struct_type         *sect
    /* sector to be programmed in the list entry */
)
{
  hdrsrchdrv_srch_list_struct_type *srch;
    /* Search list element to be filled in */

#ifdef FEATURE_HDR_REVB
  hdrsrch_carrier_idx_type         carrier_idx;
    /* The CSR/SRAM index */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB 
  /* Find the carrier_idx for this sector */
  if ( !hdrsrchrf_get_carrier_idx( sect->chan, &carrier_idx ) )
  {
    /* No CSR is assigned to this channel. */
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "No CSR/SRAM assigned(chan=%d, band=%d)",
                    sect->chan.chan_num,
                    sect->chan.band );
    return;
  }

  HDR_MSG_SRCH_5( MSG_LEGACY_LOW, 
                  "Add set[%d] PN[%d] ch[%d] bd[%d] c[%d] into srchlist",
                  sect->set, sect->pn_offset, sect->chan.chan_num,
                  sect->chan.band, carrier_idx );
#endif /* FEATURE_HDR_REVB */

  sect->win_sizex1 = hdrsrchutil_get_winsize( sect );
    /* Determine Window Size to use */


  srch = &hdrsrchutil.srch.list[ hdrsrchutil.srch.cnt ];
    /* Brevity ... */

  srch->sect_ptr = sect;
    /* Remember the sector being programmed ... */

  /* sector specific parameters */
#ifdef FEATURE_HDR_REVB
  srch->srch_task.carrier_idx  = carrier_idx;
#endif /* FEATURE_HDR_REVB */
  srch->srch_task.coh_int      = hdrsrchdrv.params.coh_int;
    /* Note: this parameter needs to update based on search task type if it
       uses different value with diversity or non-diversity */

  srch->srch_task.coh_trunc    = HDR_SRCH_COH_TRUNC;
  
  /* Use non-diversity parameter settings if it's ASET primary chain search;
     Otherwise, use the regular settings based on diversity */
  if ( hdrsrchdrv.srch_task_type == HDRSRCH_PRI_CHAIN_ASET_SRCH )
  {  
    srch->srch_task.dual_ant     = FALSE; 
    srch->srch_task.noncoh_int   = HDR_SRCH_NONCOH_ACCUM_NON_DIV;
    srch->srch_task.burst_length = HDR_SRCH_BURST_LENGTH_NON_DIV;
  }
  else
  {
    srch->srch_task.dual_ant     = hdrsrchrfdiv_diversity_demod_is_enabled();
    srch->srch_task.noncoh_int   = hdrsrchdrv.params.noncoh_int;
    srch->srch_task.burst_length = hdrsrchdrv.params.burst_len;  
  }

  srch->srch_task.carrier      = (srch4_carrier_type) carrier_idx;
    /* By default use carrier 0. 
       TBD This needs to be changed with the real carrier */

  srch->srch_task.ant_sel      = hdrsrchdrv.params.default_ant;
   /* Set non-coherent integration length */

  /* common parameters */

  srch->srch_task.pn_offset    = sect->pn_offset;
  srch->srch_task.win_size     = sect->win_sizex1;

  /* If we are searching 1x system program 1x specific 
     search parameters */
  if ( hdrsrchutil.srch.system == HDRSRCH_SYSTYPE_1X )
  {
    srch->srch_task.dual_ant = FALSE;
      /* Use single antenna not to complicate result processing */

    /* program typical values of 1x pilot search */
    srch->srch_task.coh_int       = HDR_SRCH_1X_COH_ACCUM;
    srch->srch_task.noncoh_int    = HDR_SRCH_1X_NONCOH_ACCUM_NON_DIV;
    srch->srch_task.coh_trunc     = HDR_SRCH_1X_COH_TRUNC;
    srch->srch_task.burst_length  = HDR_SRCH_1X_BURST_LENGTH_NON_DIV;
      /* burst length is only applicable to DO pilots */
  }

  if ( ( hdrsrchdrv.mstr_valid == FALSE ) &&
       ( hdrsrch_state_id == HDRSRCH_OFS_MEAS_STATE ) )
  {
    /* In IRAT pilot measurement, timing is tranferred from LTE. 
       mstr_valid is false, but we saved calculated RTC_OFFSET in mstr */
     srch->srch_task.rtc_offsetx2 = ( hdrsrchdrv.mstr_x8 + 2 )/4;
  }
  else if ( ( hdrsrchdrv.mstr_valid == TRUE )&&
            ( srch->sect_ptr->set == HDR_ASET || srch->sect_ptr->set == HDR_CSET 
              || srch->sect_ptr->set == HDR_GHOST_GSET  
            ) 
           )
  {
    srch->srch_task.rtc_offsetx2 = sect->win_cenx2;
  }
  else
  {
    srch->srch_task.rtc_offsetx2 = (HDRMSTR_GET_MSTR_X8()+2)/4;
  }

  if ( ( srch->sect_ptr->set == HDR_NSET ) || 
       ( srch->sect_ptr->set == HDR_RUPSET ) )
  {
    srch->srch_task.rtc_offsetx2 += (hdrsrchutil_get_win_offset( srch->sect_ptr ) * 2);
      /* move the window center by the win_offset parametr specified for
         the nset sector */
  }

  if ( ( srch->sect_ptr->set == HDR_NSET ) || 
       ( srch->sect_ptr->set == HDR_RSET ) )
  {
    srch->sect_ptr->win_cenx2 = srch->srch_task.rtc_offsetx2;
      /* Nset and Rset pilots window centers are generated in real time  
         in this function, so we need to save the window center that was 
         generated. The Cset and Aset window centers are generated at the
         end of dump processing and are used above. */
  }

  /* For MSM6500, rtc_offsetx2 is the start position of searcher window */
  srch->srch_task.rtc_offsetx2 = ( srch->srch_task.rtc_offsetx2 - srch->srch_task.win_size ) & 0xffff;

#ifndef FEATURE_HDR_JOLOKIA_MODEM
  /* Jolokia has a limitation of on max window size to be 2048 or 1024 in
     IRAT mode. Below code will crash on Jolokia. We still support Full PN
     search in IRAT and REACQ cases by expending the first pilot into 32
     tasks. Only 1 pilot is supported in this mode */
  if ( HDRSRCH_PILOT_MEAS_PN_CIRCLE_SEARCH_NV_ENABLE )
  {
    srch->srch_task.win_size     = 
        HDRSRCHUTIL_QUARTER_PN_CIRCLE_CX1_8192*4 - 1; 
      /* Window size is 32767 -- full PN circle in chipx1 unit */

    srch->srch_task.burst_length = 0x1;

    srch->srch_task.rtc_offsetx2 = ( hdrsrchdrv.mstr_x8 + 2)/4; 
      /* Convert all calculation to chipx2 units and search one full pn circle,
         starting at hdrsrchdrv.mstr_x8. */

  }
#endif /* FEATURE_HDR_JOLOKIA_MODEM */

  srch->srch_task.res_ptr      = &hdrsrchutil.srch.res[ hdrsrchutil.srch.cnt ];
    /* Result buffer for this search */

  hdrsrchutil.srch.cnt++;
    /* Search list has one more element in it */

} /* hdrsrchutil_add_to_search_list( ) */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_SUBMIT_SEARCH_LIST

DESCRIPTION  : This function submits the search list to the search driver.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

LOCAL void hdrsrchutil_submit_search_list( void )
{
#ifdef FEATURE_HDR_REVB
  uint8                         chan_cnt;
    /* The number of channels coverred by the current RF config */

  const hdrsrch_demod_idx_type  *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchutil.srch.cnt > 0 )
  {
    hdrsrchdrv_send_srch_list( hdrsrchutil.srch.list,
                               REFRESH_BEFORE,
                               hdrsrchutil.srch.cnt,
                               hdrsrchutil.srch.system,
                               0,
                               hdrsrchutil_do_atomic_search()
                               );
      /* Send search request to hardware */
  }
  else
  {
#ifdef FEATURE_HDR_REVB
    (void) hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                  &chan_cnt,
                                  &demod_list );

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "Submitted Empty search list!( ch_cnt=%d )",
                    chan_cnt );
#else
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Submitted Empty search list!(ch=%d)",
                    hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN )->chan_num );
#endif /* FEATURE_HDR_REVB */
  }

  hdrsrchutil.srch.system = HDRSRCH_SYSTYPE_DO;
    /* restore default search type as DO search */
    
} /* hdrsrchutil_submit_search_list( ) */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_ASET_TO_SEARCH_LIST

DESCRIPTION  : This function adds the aset to the current search list

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : The number of aset sectors added into the search list. 

SIDE EFFECTS : None

=========================================================================*/
#ifndef FEATURE_HDR_REVB
uint8 hdrsrchutil_add_aset_to_search_list
(
  const sys_channel_type          * chan
    /* Channel from which aset pilots must be selected */
)
{
  uint8                             i;
    /* Loop index */

  uint8                             srch_cnt = 0;
    /* search count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in ASET searches *
   *-----------------------*/

  for ( i = 0; i < hdrsrchset.aset_cnt; i++ )
  {
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.aset[i]->chan, *chan ) )
    {
      hdrsrchutil_add_to_search_list( hdrsrchset.aset[i] );
        /* Add active set sector to search list */

      srch_cnt++;
    }
  }
  
  return srch_cnt;
} /* hdrsrchutil_add_aset_to_search_list */
#else
uint8 hdrsrchutil_add_aset_to_search_list
(
  const sys_channel_type*           chan_list,
    /* Channel from which aset pilots must be selected */

  uint8                             chan_cnt
    /* Channel Count */
)
{
  uint8                             i, j;
    /* Loop index */

  uint8                             srch_cnt = 0;
    /* search count */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in ASET searches *
   *-----------------------*/

  for ( i = 0; i < hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      if( !hdrsrchset_is_ofreq_chan( hdrsrchset.subaset[i].aset[j]->chan ))
      {
        hdrsrchutil_add_to_search_list( hdrsrchset.subaset[i].aset[j] );
        /* Add active set sector to search list */

        srch_cnt ++;
      }
      else
      {
        /* This should not happen ever */
        ERR_FATAL( "aset sector has an OFF frequency channel[band%d, chan%d]",
                   (uint8) hdrsrchset.subaset[i].aset[j]->chan.band,
                   (uint8) hdrsrchset.subaset[i].aset[j]->chan.chan_num, 0 );
      }
    }
  }

  return srch_cnt;
} /* hdrsrchutil_add_aset_to_search_list */
#endif /* FEATURE_HDR_REVB */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_GHOST_GCSET_TO_SEARCH_LIST

DESCRIPTION  : This function adds the ghost gRICE CSET pilots to the current 
               search list

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : The number of pilots added to the search list 

SIDE EFFECTS : None

=========================================================================*/

uint8 hdrsrchutil_add_ghost_gcset_to_search_list( void )
{
  uint8                             subaset_idx;
  uint8                             g_idx;
    /* Loop indices */

  uint8                             srch_cnt = 0;
    /* search count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Remember that only ghost gCSET pilots need to be added to search list, as
     real gCSET pilots are still CSET pilots and are already added automatically. */

  for ( subaset_idx = 0; 
        subaset_idx < hdrsrchset.subaset_cnt; 
        subaset_idx++ )
  {
    for( g_idx = hdrsrchset.subaset[ subaset_idx ].aset_cnt; 
         g_idx < ( hdrsrchset.subaset[ subaset_idx ].aset_cnt +
                   hdrsrchset.subaset[ subaset_idx ].gcset_cnt );
         g_idx++ )
    {
      if(( hdrsrchset_is_ofreq_chan( 
                hdrsrchset.subaset[ subaset_idx ].aset[ g_idx ]->chan ) != FALSE ))
      {
        ERR_FATAL( "Current RF ASET is an OFS channel", 0, 0, 0 );
      }
      if ( hdrsrchset.subaset[ subaset_idx ].aset[ g_idx ]->gcset_pilot == HDR_GRICE_GHOST_CSET )
      {
        hdrsrchutil_add_to_search_list( hdrsrchset.subaset[ subaset_idx ].aset[ g_idx ] );
        /* Add ghost gCSET sector to search list */
      }

      srch_cnt ++;
    }

  }

  return srch_cnt;

} /* hdrsrchutil_add_ghost_gcset_to_search_list */


#ifdef FEATURE_HDR_REVB
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_IF_SECT_SEARCHABLE

DESCRIPTION  : Given a channel list, the function checks if the channel of
               the given sector is one of the member in the channel list.

DEPENDENCIES : None

INPUT        : sector pointer, channel list

RETURN VALUE : Boolean indicates if the sector can be searched.

SIDE EFFECTS : None

==========================================================================*/

boolean hdrsrchutil_if_sect_searchable
( 
  hdrsrch_sect_struct_type*     sect, 
    /* The sect pointer to be searched */

  const sys_channel_type*       chan_list, 
    /* The channel list */

  uint8                         chan_cnt 
    /* The number of channels in the channel list */
)
{
  uint8    i;
    /* Loop index */

  boolean  searchable = FALSE;
    /* Init the return value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=0; i<chan_cnt; i++ )
  {
    if( SYS_CHAN_CMP_EQUAL( sect->chan, chan_list[i] ) )
    {
      searchable = TRUE;
        /* Set the return value */

      break;
    }
  }

  return searchable;
} /* boolean hdrsrchutil_if_sect_searchable */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHUTIL_MATCH_ASET_CHANNELS

DESCRIPTION
  The function checks if the channel list and demod assignment matches 
  identically to the current aset channel configuration.

  The function intends to make a decision if the RF tunning is needed
  for the incoming TCA.

DEPENDENCIES
  Idle or Connected Mode

PARAMETERS
  chan_cnt   - Number of channels in the channel list.
  chan_list  - The channel list.
  demod_list - The current demod assignment list.

RETURN VALUE
  TRUE      - The channel list matches identically to the current ASET channels.
  FALSE     - The channel list does not matches to the current ASET channels.

SIDE EFFECTS
  None.

==========================================================================*/

boolean hdrsrchutil_match_aset_channels
(
  uint8                         chan_cnt,
    /* The number of channels in the channel list */

  const sys_channel_type*       chan_list,
    /* Channel list */

  const hdrsrch_demod_idx_type* demod_list
    /* Demod assignment list */
)
{
  uint8    i, j;
    /* Loop index */

  boolean  matched = TRUE;
    /* return value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( chan_cnt != hdrsrch.aset_channel_cnt )
  {
    matched = FALSE;
      /* Set the result as the numbers of channels are different */
  }
  else 
  {
    for( i=0; i<chan_cnt; i++ )
    {
      for( j=0; j<hdrsrch.aset_channel_cnt; j++ )
      {
        if( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel[j], chan_list[i] ) &&
            ( demod_list[i] == hdrsrch.aset_demod[j] ) )
        {
          /* A channel matched to one of the current channels as well as 
             its demod index assignment. */
          break;
        }
      } /* for j*/

      if( j == hdrsrch.aset_channel_cnt )
      {
        /* A new channel or new demod index assignment */
        matched = FALSE; 
        break;
      } /* if j*/
    } /* for i */
  } /* if chan_cnt */

  return matched;
} /* boolean hdrsrchutil_match_aset_channels */
#endif /* FEATURE_HDR_REVB */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_CSET_TO_SEARCH_LIST

DESCRIPTION  : This function adds the cset to the current search list

DEPENDENCIES : None

INPUT        : chan - Ptr to channel from which nset pilots must be selected

RETURN VALUE : The number of CSET sectors added into search list.

SIDE EFFECTS : None

=========================================================================*/
#ifdef FEATURE_HDR_REVB
uint8 hdrsrchutil_add_cset_to_search_list
(
  const sys_channel_type*          chan_list,
    /* Channel from which nset pilots must be selected */

  uint8                            chan_cnt,
    /* Number of channels in the channel list */

  uint8                            max_allowed
    /* Maximum allowed srch cnt */
)
{
  uint8                             i;
    /* Loop index */
  
  uint8                             cset_srch_cnt = 0;
    /* The number of search count */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in CSET searches *
   *-----------------------*/

  for ( i = 0; i < hdrsrchset.cset_cnt; i++ )
  {
    if( cset_srch_cnt >= max_allowed )
    {
      break;
    }
    else
    {
      if ( hdrsrchutil_if_sect_searchable( hdrsrchset.cset[i], 
                                           chan_list, chan_cnt ) )
      {
        cset_srch_cnt++;
          /* Increase the cnt by 1 */

        hdrsrchutil_add_to_search_list( hdrsrchset.cset[i] );
          /* Add candidate set sectors to search list */
      }
    }
  }

  return cset_srch_cnt;
} /* hdrsrchutil_add_cset_to_search_list */
#else
uint8 hdrsrchutil_add_cset_to_search_list
(
  const sys_channel_type          * chan
    /* Channel from which nset pilots must be selected */
)
{
  uint8                             i;
    /* Loop index */

  uint8                             srch_cnt = 0;
    /* search count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in CSET searches *
   *-----------------------*/

  for ( i = 0; i < hdrsrchset.cset_cnt; i++ )
  {
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan, *chan ) )
    {
      hdrsrchutil_add_to_search_list( hdrsrchset.cset[i] );
        /* Add candidate set sectors to search list */

      srch_cnt++;
    }
  }

  return srch_cnt;
} /* hdrsrchutil_add_cset_to_search_list */
#endif /* !FEATURE_HDR_REVB */




/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_NSET_TO_SEARCH_LIST

DESCRIPTION  : This function adds a subset of the nset to the current
                search list

DEPENDENCIES : None

INPUT        : *chan           - Channel array
               chan_cnt        - Channel count
               max_pilots      - Maximum NSET pilots to add
               check_nset_dups - Check for NET dups flag

RETURN VALUE : Number of nset pilots selected for reacq

SIDE EFFECTS : None

==========================================================================*/

uint8 hdrsrchutil_add_nset_to_search_list
(
  const sys_channel_type          * chan,
    /* Channel from which the nset pilots must be selected */

#ifdef FEATURE_HDR_REVB
  uint8                             chan_cnt,
    /* Number of channels in the channel list */
#endif /* FEATURE_HDR_REVB */

  uint8                             max_pilots,
    /* Maximum number of nset pilots to add */

  boolean                           check_nset_dups
    /* Flag to check search list for NSET duplicates.  This is useful
       when best NSET pilots have already been added to the search list. */
)
{
  uint8                             n_idx, p_cnt;
    /* Loop index */

  hdrsrch_sect_struct_type        * sect;
    /* Pointer to an nset pilot */

  uint8                             next_srch_idx = 0;
   /* Index into off - frequency neighbors */

  uint8                             i;
    /* Loop variable for srch list */

  uint8                             initial_srch_cnt = hdrsrchutil.srch.cnt;
    /* Initial srch count.  Only search these pilots for NSET duplicates if
       check_nset_dups is TRUE. */

#ifdef FEATURE_HDR_REVB
  boolean                           ofs_flag = FALSE;
    /* Indicate if this is an OFS search */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in NSET searches *
   *-----------------------*/

  /* nset_srch_idx is used for deciding the starting index of the nset for
     next srch. The schedular may not be able to fit all 20 (max) nset
     searches in one search list, this variable is used to store the last
     sector searched. Next visit to this function starts searching from
     srch_start_idx onwards.
   */

  n_idx = p_cnt = 0;
    /* nset index  and programmed searches count */

#ifdef FEATURE_HDR_REVB
  /* If any of the channel does not belong to the current aset channels. */
  for( i=0; i< chan_cnt; i++ )
  {
    if ( hdrsrchset_is_ofreq_chan( chan[i] ) )
    {
      ofs_flag = TRUE;
        /* Set the ofs flag */
      break;
    }
  }
#endif /* FEATURE_HDR_REVB */

  while ( p_cnt < max_pilots && n_idx < hdrsrchset.nset_cnt )
  {
#ifdef FEATURE_HDR_REVB
    if ( !ofs_flag ) 
      /* The purpose of checking here is to decide if this search
         is an OFS search or a regular triage search; It should compare 
         to hdrsrch.aset_channel which is a channel list in revB case. */
#else
    if ( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel, *chan ) )
#endif /* FEATURE_HDR_REVB */
    {

      if ( hdrsrchset.nset_params.nset_srch_idx >= hdrsrchset.nset_cnt )
      {
        /* Nset srch index is out of range. Reset it. */
        hdrsrchset.nset_params.nset_srch_idx = 0;
      }

      sect = hdrsrchset.nset[ hdrsrchset.nset_params.nset_srch_idx ];
        /* Pointer to somewhere in search list */

      if ( check_nset_dups )
      {
        /* Do not add NSET pilot if it is already in the search list.
           This avoids re-adding an NSET pilot if it has already been
           added as a "best" NSET pilot during reacq srch.  Since best NSET
           sectors are added from the same aset channel only, look for matching
           PN and ASET Channel in existing srch list.  */
        for ( i = 0; i < initial_srch_cnt; i++ )
        {
          if ( sect->pn_offset == hdrsrchutil.srch.list[i].srch_task.pn_offset )
          {
            HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                            "Duplicate NSET pilot PN %d not added",
                            sect->pn_offset );
            sect = NULL;
              /* Do not add duplicate NSET to srch list. */
    
            break;
              /* Find the duplicate and do not continue */
          }
        } /* for loop */
      } /* if ( check_nset_dups ) */

      HDRSRCHUTIL_CIRC_INCR( hdrsrchset.nset_params.nset_srch_idx, 
                             hdrsrchset.nset_cnt );
        /* Increment the NSET index by one as we have added or skipped
           (duplicate) an NSET sector.  Note: NSET increment wraps around at
           nset_cnt as the nset sectors are filled in the remaining search
           entries for each search visit */
    }
    else
    {
      /* O-freq channel */

      next_srch_idx = hdrsrchofs_get_next_srch_idx( );
        /* Get the ofreq search index for this channel */

      if ( next_srch_idx >= hdrsrchset.nset_cnt )
      {
        /* Nset ofs srch index is out of range. Reset it. */
        next_srch_idx = 0;
      }

      ASSERT( next_srch_idx < HDR_MAX_NSET_SIZE );
        /* Array boundary sanity check */

      sect = hdrsrchset.nset[next_srch_idx];
        /* Pointer to an entry in the neighbor set search list */

      HDRSRCHUTIL_CIRC_INCR( next_srch_idx, hdrsrchset.nset_cnt );
        /* Increment the ofreq index by one as we have added one sector to the 
           search list. Note that ofreq increment wraps around at the end of 
           the nset as ofreq nset sectors are also searched in round-robin 
           fashion i.e. searched few at a time in each search visit */
  
      hdrsrchofs_update_next_srch_idx( next_srch_idx );
        /* Update the incremented value of the ofreq search index in the 
           hdrsrchofs structure */

      if ( next_srch_idx == 0)
      {
        n_idx = hdrsrchset.nset_cnt;
          /* Searched all current channel ofreqs, stop NSET search additions */
      }
    }

#ifndef FEATURE_HDR_REVB
    if ( ( sect != NULL ) && SYS_CHAN_CMP_EQUAL( sect->chan, *chan ) &&
         ( sect->sys_type == hdrsrchutil.srch.system ) )
    {
      hdrsrchutil_add_to_search_list( sect );
        /* Add neighbour set pilot to search list */

      p_cnt++;
    }
#else
    if ( ( sect != NULL ) &&
         ( sect->sys_type == hdrsrchutil.srch.system ) &&
         hdrsrchutil_if_sect_searchable( sect, chan, chan_cnt ) )
    {
      hdrsrchutil_add_to_search_list( sect );
        /* Add neighbour set pilot to search list */

      p_cnt++;
    }
#endif /* !FEATURE_HDR_REVB */

    n_idx++;
      /* we moved ahead by 1 pilot in neighbor set */

  } /* for each nset member or until list is full */

  return p_cnt;
    /* number of pilots added to search list */

} /* hdrsrchutil_add_nset_to_search_list */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADD_RSET_TO_SEARCH_LIST

DESCRIPTION  : This function adds the rset to the current search list

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/
#ifndef FEATURE_HDR_REVB
void hdrsrchutil_add_rset_to_search_list
(
  const sys_channel_type          * chan
    /* only search sectors on this channel */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in RSET searches *
   *-----------------------*/
  if ( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel, *chan ) )
  {

    hdrsrchset_next_rset_sect();
      /* update the hdrsrchset.rset with the next RSET PN offset */

    if ( hdrsrchset.rset )
    {
      hdrsrchutil_add_to_search_list( hdrsrchset.rset );
        /* Add 1 remaining set pilot to search list */
    }
  }
} /* hdrsrchutil_add_rset_to_search_list*/

#else
uint8 hdrsrchutil_add_rset_to_search_list
(
  const sys_channel_type*   chan
    /* only search sectors on this channel */
)
{
  uint8 num_srch_tasks = 0;
    /* Local variable to track how many tasks has been added */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------*
   * Fill in RSET searches *
   *-----------------------*/
  if ( !hdrsrchset_is_ofreq_chan( *chan ) )
  {

    hdrsrchset_next_rset_sect( *chan );
    /* update the hdrsrchset.rset with the next RSET PN offset */

    if ( hdrsrchset.rset )
    {
      hdrsrchutil_add_to_search_list( hdrsrchset.rset );
        /* Add 1 remaining set pilot to search list */

      num_srch_tasks = 1;
    }
  }

  return num_srch_tasks;
} /* hdrsrchutil_add_rset_to_search_list*/
#endif /* !FEATURE_HDR_REVB */


/* <EJECT> */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_ADD_BEST_NSET_TO_SEARCH_LIST

DESCRIPTION
    Adds the top "n" nset pilots to the search list.

    The criterion for nset selection is:
      1. On the same frequency as reacq_channel
      2. Crosses the minimum pilot energy threshold
      3. Top "n" pilots

DEPENDENCIES
  None

RETURN VALUE
  Number of nset pilots selected

SIDE EFFECTS
  None

==========================================================================*/

uint8 hdrsrchutil_add_best_nset_to_search_list
(
  const sys_channel_type          * chan,
    /* Channel from which the nset pilots must be selected */

  uint8                             max_pilots
    /* Maximum number of nset pilots to add */
)
{
  hdrsrch_sect_struct_type        * best_pilots[ HDR_MAX_SRCH_CNT ] = { 0 };
    /* Output: Array of best peaks */

  hdrsrch_sect_struct_type        * sect;
    /* Convenience pointer */

  uint8                             num_pilots;
    /* Number of peaks in best_pilots[ ] array */

  uint8                             weakest_eng;
    /* Energy of weakest peak in best_pilots[ ] array. */

  uint8                             weakest_inx;
    /* Index position of weakest peak in best_peak array. */

  uint8                             p, i;
    /* Loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  num_pilots = 0;
    /* No entries in best peaks array (yet) ... */

  if ( max_pilots > 0 )
  {

    weakest_eng = HDRSRCHUTIL_MIN_REACQ_NSECT_ENG;
      /* ... so the weakest best pilot is pretty weak. */

    for( p = 0; p < hdrsrchset.nset_cnt; p++)
    {
      sect = hdrsrchset.nset[ p ];
        /* For convenience */
  
      /* Select pilots stronger than the weakest of the strong */
  
      if ( sect->total_eng > weakest_eng  &&
           SYS_CHAN_CMP_EQUAL( sect->chan, *chan ) )
      {
        /* We have found a pilot strong enough to include in best_pilots[ ]. */
  
        if ( num_pilots < max_pilots )
        {
          weakest_inx = num_pilots++;
            /* Table isn't full yet; add to end */
  
          best_pilots[ weakest_inx ] = sect;
            /* Put new pilot in best_pilots[ ] array */
        }
        else 
        {
          /* Table is full.  Find the weakest pilot, and replace it. */
  
          weakest_eng = best_pilots[0]->total_eng;
          weakest_inx = 0;
  
          ASSERT(num_pilots <= HDR_MAX_SRCH_CNT );

          for ( i = 1; i < num_pilots ; i++ )
          {
            if ( best_pilots[i]->total_eng < weakest_eng )
            {
              weakest_eng = best_pilots[i]->total_eng;
              weakest_inx = i;
                /* new index of the weakest */
            }
  
          }/* for */
  
          if ( best_pilots[ weakest_inx ]->total_eng < sect->total_eng )
          {
            best_pilots[ weakest_inx ] = sect;
              /* Put new pilot in best_pilots[ ] array */
          }
  
        }
  
      } /* If pilot was strong */
  
    } /* For each pilot */
  
  
    /* Top "n" NSet pilots have been found - add them to search list */
  
    for ( i = 0; i < num_pilots; i++ )
    {
      hdrsrchutil_add_to_search_list( best_pilots[i] );
        /* Add sector to search list */
    }
  }

  return num_pilots;

} /* hdrsrchutil_add_best_nset_to_search_list */



/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_LIMIT_WINDOW_SIZE

DESCRIPTION  : This function checks the window size of the given
               window against the given limits.

DEPENDENCIES : None

RETURN VALUE : Window size

SIDE EFFECTS : None

=========================================================================*/

int hdrsrchutil_limit_window_size
(
  int                               window_size,
    /* Maximum window size, in chips */

  hdrsrch_winsize_enum_type         max_winsize,
    /* Maximum window size */

  hdrsrch_winsize_enum_type         min_winsize
    /* Minimum window size */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( window_size > hdrsrchutil_winsize[ max_winsize ] )
  {
    window_size = hdrsrchutil_winsize[ max_winsize ];
  }
  else if ( window_size < hdrsrchutil_winsize[ min_winsize ] )
  {
    window_size = hdrsrchutil_winsize[ min_winsize ];
  }

  return window_size;

} /* hdrsrchutil_limit_window_size */


/*=========================================================================

FUNCTION     : HDRSRCHUTIL_LIMIT_SEARCH_WINDOW

DESCRIPTION  : This function checks the window size of the current
                search list to ensure that the max and min window
                sizes are set properly.

DEPENDENCIES : None

INPUT        : Maximum and Minimum window size

RETURN VALUE : None

SIDE EFFECTS : Window sizes changed

=========================================================================*/

void hdrsrchutil_limit_search_window
(
  hdrsrch_winsize_enum_type         max_winsize,
    /* Maximum window size */

  hdrsrch_winsize_enum_type         min_winsize
    /* Minimum window size */
)
{
  uint16                            max_winsize_x1;
    /* Maximum window size, in chips */

  uint16                            min_winsize_x1;
    /* Minimum window size, in chips */

  uint8                             i;
    /* Loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  max_winsize_x1 = hdrsrchutil_winsize[ max_winsize ];
  min_winsize_x1 = hdrsrchutil_winsize[ min_winsize ];
    /* Convert max and min window sizes to chips */

  for ( i = 0; i < hdrsrchutil.srch.cnt; i++ )
  {
    if ( hdrsrchutil.srch.list[i].srch_task.win_size > max_winsize_x1 )
    {
      hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 = 
                  hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 +
                  hdrsrchutil.srch.list[i].srch_task.win_size     -
                  max_winsize_x1;
        /* Adjust the start position of the search window when search 
           window size changes                                      */

      hdrsrchutil.srch.list[i].srch_task.win_size = max_winsize_x1;

      hdrsrchutil.srch.list[i].sect_ptr->win_sizex1 = max_winsize_x1;
        /*  Update size info in sector structure  */
    }
    else if ( hdrsrchutil.srch.list[i].srch_task.win_size < min_winsize_x1 )
    {
      hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 = 
                  hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 +
                  hdrsrchutil.srch.list[i].srch_task.win_size     -
                  min_winsize_x1;
      /* Adjust the start position of the search window when search 
         window size changes                                      */

      hdrsrchutil.srch.list[i].srch_task.win_size = min_winsize_x1;
      
      hdrsrchutil.srch.list[i].sect_ptr->win_sizex1 = min_winsize_x1;
        /*  Update size info in sector structure  */
    }
  }

} /* hdrsrchutil_limit_search_window */

/*===========================================================================

FUNCTION HDRSRCHUTIL_CHK_CONVERT_TO_FULL_PN()

DESCRIPTION
  The function checks if Full PN search NV is enabled. If so,
  it expands the first search task into full PN. All other
  search tasks will be overwritten. hdrsrchutil_chk_triage_full_pn()
  then must be called to find the best peaks in the PN circle.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  All except the first search tasks will be overwritten

===========================================================================*/
void hdrsrchutil_chk_convert_to_full_pn( void )
{
  uint8 srch_cnt;
  uint8 i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( HDRSRCH_PILOT_MEAS_PN_CIRCLE_SEARCH_NV_ENABLE 
#ifdef FEATURE_HDR_RUMI
       || hdrsrchutil_full_pn_search 
#endif /* FEATURE_HDR_RUMI */
     )
  {
    srch_cnt = ( HDR_PN_CIRC_X1 + HDRSRCHUTIL_MAX_WIN_SIZE_X1 - 1 ) / 
               HDRSRCHUTIL_MAX_WIN_SIZE_X1;

    if ( srch_cnt > HDR_MAX_SRCH_CNT )
    {
      srch_cnt = HDR_MAX_SRCH_CNT;
    }

    if ( hdrsrchutil.srch.cnt != 1 )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "FullPN only supports 1 srch task (%d), rest will be lost!",
                      hdrsrchutil.srch.cnt );
    }

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Full PN search, %d tasks of size %d",
                    srch_cnt, HDRSRCHUTIL_MAX_WIN_SIZE_X1 );

    hdrsrchutil.srch.list[0].srch_task.win_size = HDRSRCHUTIL_MAX_WIN_SIZE_X1;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "FullPN: tasks #0 PN %d at RTC_X2 0x%x",
                    hdrsrchutil.srch.list[0].srch_task.pn_offset,
                    hdrsrchutil.srch.list[0].srch_task.rtc_offsetx2 );

    for (i=1; i<srch_cnt; i++)
    {
      if ( i<hdrsrchutil.srch.cnt )
      {
        HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "FullPN: overwriting tasks #%d PN %d WIN %d RTC_X2 0x%x",
                        i, 
                        hdrsrchutil.srch.list[i].srch_task.pn_offset,
                        hdrsrchutil.srch.list[i].srch_task.win_size,
                        hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 );
      }

      hdrsrchutil.srch.list[i] = hdrsrchutil.srch.list[0];

      hdrsrchutil.srch.list[i].srch_task.res_ptr = &hdrsrchutil.srch.res[i];

      hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 = 
        ( hdrsrchutil.srch.list[0].srch_task.rtc_offsetx2 + 
          ( HDRSRCHUTIL_MAX_WIN_SIZE_X1 * 2 * i ) ) % 
        ( HDR_PN_CIRC_X1 * 2 );


      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "FullPN: tasks #%d at RTC_X2 0x%x",
                      i, hdrsrchutil.srch.list[i].srch_task.rtc_offsetx2 );
    }

    hdrsrchutil.srch.cnt = srch_cnt;
  }
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROGRAM_REACQ_SEARCH

DESCRIPTION  : This function programs ASET & CSET sector searches

DEPENDENCIES : None

INPUT        : To do default search or not.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchutil_program_reacq_search( boolean default_search )
{
  uint32   reacq_rtc_pos = 0; /* RTC pos from FW in cx2*/

  uint32   hdr_rtc_cx2 = 0; /* RTC pos from HW in cx2*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  
  if ( default_search )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Div_debug: SRCH MODE DEFAULT! " );

    hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
      /* Set Srch4 param to default values */
  }
  else
  {
#ifdef FEATURE_HDR_BOLT_MODEM
    hdr_rtc_cx2 = hdrts_get_rtc_cx2();
#else
    HWIO_UNIV_STMR_STATUS_DUMP_CMD_OUT( \
    HWIO_UNIV_STMR_STATUS_DUMP_CMD_STATUS_DUMP_UP_BMSK );

    hdr_rtc_cx2 = HWIO_RTC_HDR_TIME_STATUS_RD_IN >> 4;
#endif /* FEATURE_HDR_BOLT_MODEM */

  
    reacq_rtc_pos = HDRMDSP_GET_TRKPILOT_POS_CX2();
    /* Read the RTC position in CX2 at which sample capture is performed, optimized
       search in reacq state for shorter time line. */

   HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Div_debug: hdr_rtc_cx2 = 0x%x, reacq_rtc_pos= 0x%x ",
                    hdr_rtc_cx2,
                    reacq_rtc_pos );

    /* If pilot position from fw and current RTC count is within 1ms,
       reacq search with pilot position srch mode
       ((HDR_RTC_CNT - pilotPos from SMEM) & 0xFFFF) > 2458
       2458 Cx2 = 1ms*1.2288 Mcps*2
       if the condition is TRUE, use DEFAULT mode, else TRK mode
    */
    if ( ( ( hdr_rtc_cx2 - reacq_rtc_pos ) & 0xFFFF ) >  MS_TO_CHIPX2  )
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Div_debug: SRCH MODE DEFAULT! " );
      hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
        /* Set Srch4 param to default values */
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Div_debug: SRCH MODE TRK PILOT! " );
      hdrsrchdrv_set_srch4_param( SRCH4_MODE_HDR_TRK_PILOTS, ( uint16 ) reacq_rtc_pos );
        /* Set Srch4 param to TRK pilots and include RTC position in search list */      
    }
  }

  hdrsrchutil_chk_convert_to_full_pn( );
    /* Check and convert to full PN search if NV is enabled */

  hdrsrchutil_submit_search_list( );
    /* Submit the reacq list to the hardware */

  HDRSRCH_PROFILE( REACQ_SRCH_SCHED );
    /* sleep timeline profiling output if enabled */

} /* hdrsrchutil_program_reacq_search() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESET_SRCH_FILTERS

DESCRIPTION  : This function resets the search EcIo filter of the sector 
               to be searched. The intended use of this function is to 
               reset the filters before OFS searches.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchutil_reset_srch_filters( void )
{
  hdrsrch_sect_struct_type          *sect_ptr;
    /* pointer to the sector structure */
    
  uint8           i;
    /* temporary storage    */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    
  for ( i = 0 ; i < hdrsrchutil.srch.cnt ; i++ )
  {
    sect_ptr = hdrsrchutil.srch.list[i].sect_ptr;
      /* Convenience pointer */
      
    sect_ptr->total_eng = 0;
    sect_ptr->filt_length = 0;
    sect_ptr->pri_chain_filt_eng = 0;
    sect_ptr->pri_chain_eng_filt_length = 0;
    sect_ptr->idho_filter.total_eng = 0;
    sect_ptr->idho_filter.length = 0;
    sect_ptr->sbho_filter.length = 0;
    sect_ptr->sbho_filter.total_eng = 0;
      /* Reset the search energy and filter length */
  }

} /* hdrsrchutil_reset_srch_filters */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESET_OFS_FILTER

DESCRIPTION  : This function resets the OFS filter

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchutil_reset_ofs_filter( void )
{
  uint16                            i;                               
    /* set loop counter */
  
  hdrsrch_sect_struct_type          *sect_ptr;
    /* sector which needs to be filtered */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i=0 ; i < hdrsrchutil.srch.cnt ; i++ )
  {
    sect_ptr = hdrsrchutil.srch.list[i].sect_ptr;
      /* get pointer to sector structure */
      
    sect_ptr->ofs_filter.accum = 0;
    sect_ptr->ofs_filter.num_samples = 0;
    sect_ptr->ofs_filter.filtered_eng = 0;
      /* Reset the search energy and number of samples */
  }
    
} /* hdrsrchutil_reset_ofs_filter() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROGRAM_SEARCHES

DESCRIPTION  : This function programs ASET, CSET, NSET and RSET searches
               in the steady state. It tries to program all ASET/CSET
               sectors, 1 RSET sector and remaining searches are filled
               from the NSET. NSET is searched in round robin fashion
               such that the number of searches of type NSET varies
               depending on the ASET and CSET size.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : winsizex1 is filled in with the search window size
               for the programmed sectors.

=========================================================================*/
#ifndef FEATURE_HDR_REVB
LOCAL void hdrsrchutil_program_searches
(
  const sys_channel_type      * chan
    /* only search sectors on this channel */
)
{
  uint8                         max;
    /* Maximum number of nset pilots that will fit in search list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.srch.system = HDRSRCH_SYSTYPE_DO;
    /* this function is used for DO searches */

  hdrsrchutil_add_aset_to_search_list( chan );
    /* Add the ASET to the search list */

  hdrsrchutil_add_cset_to_search_list( chan );
    /* Add the CSET to the search list */

  hdrsrchutil_add_rset_to_search_list( chan );
    /* Add an RSET to the search list */

  max = HDR_MAX_SRCH_CNT - hdrsrchutil.srch.cnt;
    /* Compute maximum number of neighbors to be added */

  (void) hdrsrchutil_add_nset_to_search_list( chan, max, FALSE );
    /* Add NSET to the search list */

} /* hdrsrchutil_program_searches() */
#else
LOCAL void hdrsrchutil_program_searches
(
  const sys_channel_type*       chan_list,
    /* The channel list on which searcher searches */

  uint8                         chan_cnt
    /* The number of channels in the channel list */
)
{
  int16                        max_allowed = HDR_MAX_SRCH_CNT;
    /* Local variable used to track how many pilots can be searched */

  sys_channel_type*            cc_mon_chan;
    /* Pointer to Control Channel Monitoring channel */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.srch.system = HDRSRCH_SYSTYPE_DO;
    /* This function is used for DO searches */

  ( void ) hdrsrchutil_add_aset_to_search_list( chan_list, chan_cnt );
    /* Add the ASET to the search list first */

  cc_mon_chan = hdrsrchset_get_cc_monitoring_chan();
    /* Get CC Monitoring channel if it exists. */

  if ( cc_mon_chan != NULL )
  {
    ( void ) hdrsrchutil_add_rset_to_search_list( cc_mon_chan );
      /* Add one RSET on the channel assigned to CC carrier */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "No CC Monitoring Channel found in SUBASET.  No RSET to search.");
  }

  max_allowed = HDR_MAX_SRCH_CNT - hdrsrchutil.srch.cnt;
    /* Calculate the max number of cset search tasks */

  ( void ) hdrsrchutil_add_cset_to_search_list( chan_list,
                                                chan_cnt,
                                                max_allowed );
    /* Add CSET sectors according to the channel list */

  if ( ( hdrsrcheq_is_grice_enabled() ) &&
       ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) )
  {
    max_allowed = HDR_MAX_SRCH_CNT - hdrsrchutil.srch.cnt;
      /* Calculate the max number of ghost gRICE CSET search tasks */
  
    ( void ) hdrsrchutil_add_ghost_gcset_to_search_list();
      /* Add ghost gRICE CSET sectors (created to replace pilots which
         may have been purged during pilot group purge) */
  }


  max_allowed = HDR_MAX_SRCH_CNT - hdrsrchutil.srch.cnt;
    /* Calculate the max number of nset search tasks */

  ( void ) hdrsrchutil_add_nset_to_search_list( chan_list, 
                                                chan_cnt, 
                                                max_allowed,
                                                FALSE );
    /* Fill the rest srch bandwith with NSET sectors */

} /* void hdrsrchutil_program_searches */
#endif /* FEATURE_HDR_REVB */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROGRAM_OFS_SEARCH_SET

DESCRIPTION
  Program a collection of searches, based on the current sets and channel.

DEPENDENCIES
  None

PARAMETERS
  sys - type of system the search has to be programmed for. 

RETURN VALUE
  True if at least one pilot was submitted for search else false.

SIDE EFFECTS
  Starts lost dump timer.

=========================================================================*/

boolean hdrsrchutil_program_ofs_search_set
(
  hdrsrch_system_enum_type    sys
    /* system type for the current channel */
)
{
  const sys_channel_type      *chan;
    /* only search sectors on this channel */

#ifdef FEATURE_HDR_REVB
  uint8                        chan_cnt;
    /* channel count */

  const hdrsrch_demod_idx_type *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */

  uint8                        max;
    /* Maximum number of nset pilots that will fit in search list */

  boolean                      srch_programmed = TRUE;
    /* Indicate search submitted to hardware if search list is not empty */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_init_search_list( );
    /* Reset the search list */

  hdrsrchutil.srch.system = sys;
    /* set type of system to search ( 1X or DO ) */

  /* Program the search only if the searcher is idle */
  if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
  { 
    if ( hdrsrchrfdiv_diversity_demod_is_enabled() ) 
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
    }
    else
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
    }

    max = HDR_MAX_SRCH_CNT;
      /* Set maximum number of neighbors to be added */

#ifdef FEATURE_HDR_REVB
    chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                  &chan_cnt,
                                  &demod_list ) ;
      /* Determine the current channel list the AT has tuned */

    (void) hdrsrchutil_add_nset_to_search_list( chan, chan_cnt, max, FALSE );
      /* Add NSET to the search list */
#else
    chan = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN ) ;
      /* determine the current channel the AT has tuned */

    (void) hdrsrchutil_add_nset_to_search_list( chan, max, FALSE );
      /* Add NSET to the search list */
#endif /* FEATURE_HDR_REVB */

    hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
      /* Set Srch4 param to default values */

    hdrsrchutil_submit_search_list( );
      /* Submit search list to the hardware */
  }
    
  /* If no pilot was submitted for search */
  if ( hdrsrchutil.srch.cnt == 0 )
  {
    srch_programmed = FALSE;
  }

  return srch_programmed;

} /* hdrsrchutil_program_ofs_search_set() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_MARK_A_C_SETS_FOR_REPORTING

DESCRIPTION
  The function sets pilot report pending flags for pilots in ASET and CSET,
  so that they will be reported to network when a RUP request is received
  for the serving channel.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_mark_a_c_sets_for_reporting( void )
{
  uint16                             i, j;
    /* loop variable */

#ifdef FEATURE_HDR_REVB
  uint8                              subaset_idx, k;    
    /* subaset loop index */

  boolean                            is_pilot_group_unique;
    /* TRUE if pilot group is unique. */

#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* set pilot report pending flags for pilots in ASET and CSET 
     so that they will be reported again due to RUP request */
#ifdef FEATURE_HDR_REVB

  ASSERT(hdrsrchset.subaset_cnt <= HDRSRCH_MAX_NUM_SUBASETS);
   
  for( subaset_idx=0; subaset_idx<hdrsrchset.subaset_cnt; subaset_idx++ )
  {
    for ( i = 0 ; 
          ( ( i < hdrsrchset.subaset[subaset_idx].aset_cnt ) &&
            ( i < HDRSRCH_ASET_MAX_SIZE ) );
          i++ )
    {
      /* Only mark one pilot from each pilot group to trigger a report.
         Search PREVIOUS pilots to see if pilot group was encountered already.
         Treat HDRSRCH_UNIQUE_PILOT_GID as unique, which also makes RevA
         pilots unique. */
      is_pilot_group_unique = TRUE;
      if ( hdrsrchset.subaset[subaset_idx].aset[i]->pilot_gid !=
           HDRSRCH_UNIQUE_PILOT_GID )
      {
        /* Start with current subaset's previous ASETs */
        for ( j=0; j < i; j++ )
        {
          if ( HDRSRCHSET_PILOT_GROUP_CMP(
                 *hdrsrchset.subaset[subaset_idx].aset[i],
                 *hdrsrchset.subaset[subaset_idx].aset[j] ) )
          {
            is_pilot_group_unique = FALSE;
              /* Found matching pilot GID already.
                 Don't report current pilot. */
            break;
          }
        } /* for j... */

        /* Now search previous subasets's asets. */
        for ( k = 0; k < subaset_idx; k++)
        {
          if ( is_pilot_group_unique == FALSE )
          {
            /* Already found previous pilot in same pilot group */
            break;
          }
          for ( j=0; j < hdrsrchset.subaset[k].aset_cnt; j++ )
          {
            if ( HDRSRCHSET_PILOT_GROUP_CMP(
                   *hdrsrchset.subaset[subaset_idx].aset[i],
                   *hdrsrchset.subaset[k].aset[j] ) )
            {
              is_pilot_group_unique = FALSE;
                /* Found matching pilot GID already.
                   Don't report current pilot. */
              break;
            }
          } /* for j... */
        } /* for k... */

      } /* if not HDRSRCH_UNIQUE_PILOT_GID */

      /* Was pilot group in previous asets? */
      if ( is_pilot_group_unique == TRUE )
      {
        /* Pilot group was not in previous asets. Flag for report pending. */
        hdrsrchset.subaset[subaset_idx].aset[i]->pilot_rpt_pending = TRUE;
      }
    }
  } /* for subaset */
#else
  for ( i = 0 ; i < hdrsrchset.aset_cnt ; i++ )
  {
    hdrsrchset.aset[i]->pilot_rpt_pending = TRUE;
  }
#endif /* FEATURE_HDR_REVB */
    
  for ( i = 0 ; i < hdrsrchset.cset_cnt ; i++ )
  {
    hdrsrchset.cset[i]->pilot_rpt_pending = TRUE;
  }
  
} /* hdrsrchutil_mark_a_c_sets_for_reporting() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_COPY_RUP_REQ_PARAMS

DESCRIPTION
  Copy the RUP request command from protocols.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_copy_rup_req_params
(
  const hdrsrch_rup_req_params_type  *params
    /* RUP Request parameters */
)
{
  uint16                             i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy RUP request message contents */
  hdrsrch_rup_req.channel_included = params->channel_included;
  
  /* get channel specified in RUP request message 
     if channel is not included use current channel */
  if ( hdrsrch_rup_req.channel_included == FALSE )
  {
#ifdef FEATURE_HDR_REVB
    hdrsrch_rup_req.channel.chan_num = 0xffff; 
      /* If channel is not included, the RUP report should cover the
         entire aset channels. Mark the channel as invalid! */
#else
    hdrsrch_rup_req.channel = hdrsrchset.aset[0]->chan;
#endif /* FEATURE_HDR_REVB */
    hdrsrch_rup_req.system_type = HDRSRCH_SYSTYPE_DO;
    hdrsrch_rup_req.sector_count = 0;
  }
  else
  {
    hdrsrch_rup_req.system_type = params->system_type ;
    hdrsrch_rup_req.channel.chan_num = params->channel.chan_num;
    hdrsrch_rup_req.channel.band = params->channel.band;
    hdrsrch_rup_req.sector_count = params->sector_count;
  }

  hdrsrch_rup_req.pilot_rpt_cb = params->pilot_rpt_cb;
    /* register callback function */

  /* get pilots specified in RUP request message */
  if ( hdrsrch_rup_req.sector_count == 0 )
  {
    hdrsrch_rup_req.window_size_included = FALSE;
    hdrsrch_rup_req.window_offset_included = FALSE;
  }
  else
  {
    /* copy sector PNs from RUP request */
    for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
    {
      hdrsrch_rup_req.sector_info[i].pilot_pn = params->sector_info[i].pilot_pn;
    }
    
    hdrsrch_rup_req.window_size_included = params->window_size_included;
      /* check whether window sizes have been specied in RUP request */
      
    if ( hdrsrch_rup_req.window_size_included == TRUE )
    {
      for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
      {
        hdrsrch_rup_req.sector_info[i].window_size = params->sector_info[i].window_size;
      }
    }

    hdrsrch_rup_req.window_offset_included = params->window_offset_included;
      /* check whether window offsets have been specied in RUP request */
          
    if ( hdrsrch_rup_req.window_offset_included == TRUE )
    {
      for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
      {
        hdrsrch_rup_req.sector_info[i].window_offset = params->sector_info[i].window_offset;
      }
    }
  }
  
  if ( hdrsrch_rup_req.system_type == HDRSRCH_SYSTYPE_1X )
  {
    hdrsrch_rup_req.chan_mgmt_1x_included = params->chan_mgmt_1x_included;
      /* Check whether 1x channel management params are included */
      
    if ( hdrsrch_rup_req.chan_mgmt_1x_included == TRUE )
    {
      hdrsrch.chan_mgmt_1x_params.pilot_add = params->chan_mgmt_1x.pilot_add;
      hdrsrch.chan_mgmt_1x_params.pilot_inc = params->chan_mgmt_1x.pilot_inc;
        /* copy 1x search parameters from protocols */
    }
    else
    {
      hdrsrch.chan_mgmt_1x_params.pilot_add = HDRSRCH_1X_DEFAULT_PILOT_ADD;
      hdrsrch.chan_mgmt_1x_params.pilot_inc = HDRSRCH_1X_DEFAULT_PILOT_INCREMENT;
        /* set default 1x channel management parameters */
    }
  }
} /* hdrsrchutil_copy_rup_req_params() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROCESS_RUP_REQ

DESCRIPTION
  Process the RUP request command from protocols.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_process_rup_req
(
  const hdrsrch_rup_req_params_type  *params
    /* RUP Request parameters */
)
{
  hdrsrchutil_copy_rup_req_params( params );
    /* copy the RUP parameters to hdrsrch_rup_req */

#ifdef FEATURE_HDR_REVB
  if ( ( hdrsrch_rup_req.channel_included == FALSE ) ||
       hdrsrchset_is_ofreq_chan( hdrsrch_rup_req.channel ) == FALSE )
#else
  if ( SYS_CHAN_CMP_EQUAL( hdrsrch_rup_req.channel, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
  {
    /* if rup search channel is same as current serving channel
       then just report whatever we have in ASET or CSET
       no search is initiated */
       
    hdrsrchutil_mark_a_c_sets_for_reporting();
      /* set pilot report pending flags for pilots in ASET and CSET
         so that they will be reported again due to RUP request */
         
    hdrsrchutil_send_rup_pilot_report();
      /* Send the pilot reports for the current channel */
  }
  else
  {
    hdrsrchofs_register_rup_req( hdrsrch_rup_req.channel,
                                 hdrsrch_rup_req.system_type,
                                 FALSE );     
      /* if rup search channel is different from current serving channel
         register the RUP request for RUP with OFS state */
  }
  
} /* hdrsrchutil_process_rup_req() */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_SEND_RUP_PILOT_REPORT

DESCRIPTION
  This function calls the call back function for RUP pilot reporting.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_send_rup_pilot_report( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_rup_req.pilot_rpt_cb != NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR RUP: Searcher Pilot Report pending... " );
      
    hdrsrch_rup_req.pilot_rpt_cb();
      /* call the pilot report call back */
  }                   
 
} /* hdrsrchutil_send_rup_pilot_report( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_RUP_REQ_CMD

DESCRIPTION
  This function passes RUP request command to searcher.

DEPENDENCIES
  None

PARAMETERS
  params - parameters for the RUP request command.

RETURN VALUE
  Status of the command.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchutil_rup_req_cmd
(
  const hdrsrch_rup_req_params_type     *params
    /* RUP Request parameters */
)
{
  errno_enum_type                       status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "RUP_REQ_CMD" );

  if ( hdrsrchofs_rup_pending() == TRUE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR RUP: Discarded new RUP request, State ID= %d ",
                    hdrsrch_state_id );
    
    status = E_FAILURE;
      /* Indicate that we will not serve another RUP request
         before the pending one is finished */
  }
  else
  {
    hdrsrchutil_process_rup_req( params );
      /* Process command parameters */    
  }
  
  return ( status );
    /* Command has been completed */

} /* hdrsrchutil_rup_req_cmd( ) */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESET_RUP_SET

DESCRIPTION
  Resets RUP search set.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
    
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_reset_rup_set( void )
{

  uint16                      i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Reset RUP search set */
  for ( i = 0 ; i < hdrsrchset.rupset_cnt ; i++ )
  {
    hdrsrchset.rupset[i] = NULL;
  }
  
  hdrsrchset.rupset_cnt = 0;
    /* Deleted all pilots in RUP search set */
  
} /* hdrsrchutil_reset_rup_set() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESET_RUP_SRCH_STATE

DESCRIPTION
  Reset RUP search set generation related state variables.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
    
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_reset_rup_srch_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize RUP search state related variables */
  hdrsrchrup.a_c_sets_completed = FALSE;
  hdrsrchrup.rup_chan_nset_completed = FALSE;
  hdrsrchrup.serv_chan_nset_completed = FALSE;
  hdrsrchrup.rset_completed = FALSE;
  hdrsrchrup.rup_chan_nset_index = 0;
  hdrsrchrup.serv_chan_nset_index = 0;
  hdrsrchrup.last_rset_pn = 0;
  
} /* hdrsrchutil_reset_rup_srch_state() */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GENERATE_PRIORITIZED_RUP_SRCH_SET

DESCRIPTION
  Generate the RUP sectors in prioritized order from pilots in 
  Active, Candidiate, Neighbor and Remaining set PNs..

DEPENDENCIES
  None

PARAMETERS
  sys - type of the system for the rup search. 

RETURN VALUE
  Whether the generated RUP set is the last one for the RUP request
  i.e we are done generating RUP sets for the pending RUP request.
  
  As the function is called only from the tcofs, folloing statements
  are true.
  (1) The RUP request channel can not be part of the aset channel.
  (2) The RUP request channel is defined in the RUP request message.
SIDE EFFECTS
  None
  
=========================================================================*/

boolean hdrsrchutil_generate_prioritized_rup_set
(
  hdrsrch_system_enum_type    sys
    /* system type for the rup search */
)
{
  sys_channel_type            chan;
    /* only search sectors on this channel */
        
  boolean                     last_rupset;
    /* flag whether the generated RUP set is the last one for the RUP request
      i.e we are done generating RUP sets for the pending RUP request. */

  uint16                      i;
    /* loop variable */
            
#ifdef FEATURE_HDR_REVB
  uint16                      j;
    /* Loop variable */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  chan = hdrsrch_rup_req.channel ;
    /* RUP search channel for brevity */
          
  hdrsrchset.rupset_cnt = 0;
    /* reset RUP set */
      
  /* Add Active set and Candidate set PNs to RUP set */
  if ( hdrsrchrup.a_c_sets_completed == FALSE )
  {
#ifdef FEATURE_HDR_REVB
    for( j=0; j<hdrsrchset.subaset_cnt; j++ )
    {
      for ( i = 0 ; i < hdrsrchset.subaset[j].aset_cnt ; i++ )
      {
        if(( SYS_CHAN_CMP_EQUAL( hdrsrchset.subaset[j].aset[i]->chan, chan )) &&
           ( hdrsrchset.subaset[j].aset[i]->sys_type == sys ))
        {
          hdrsrchset_add_rup_sect( hdrsrchset.subaset[j].aset[i] );
            /* Add sector to RUP search from ASET */
        }
        else
        {      
          ( void ) hdrsrchset_get_rup_sect( 
                                     hdrsrchset.subaset[j].aset[i]->pn_offset, 
                                     chan,
                                     sys );
        }
      } /* for i */
    } /* for j */
#else
    for ( i = 0 ; i < hdrsrchset.aset_cnt ; i++ )
    {
      if ( ( SYS_CHAN_CMP_EQUAL( hdrsrchset.aset[i]->chan, chan ) ) &&
           ( hdrsrchset.aset[i]->sys_type == sys ) )
      {
        hdrsrchset_add_rup_sect( hdrsrchset.aset[i] );
          /* Add sector to RUP search from ASET */
      }
      else
      {      
        (void) hdrsrchset_get_rup_sect( hdrsrchset.aset[i]->pn_offset, 
                                        chan,
                                        sys );
      }
    }
#endif /* FEATURE_HDR_REVB */

    for ( i = 0 ; i < hdrsrchset.cset_cnt ; i++ )
    {
      if ( ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan, chan ) ) &&
           ( hdrsrchset.cset[i]->sys_type == sys ) )
      {
        hdrsrchset_add_rup_sect( hdrsrchset.cset[i] );
          /* Add sector to RUP search from CSET */
      }
      else
      {      
        (void) hdrsrchset_get_rup_sect( hdrsrchset.cset[i]->pn_offset, 
                                        chan,
                                        sys );
      }    
    }
    
    hdrsrchrup.a_c_sets_completed = TRUE;
      /* Indicate that we finished going over A Set and C Set */
  }
  
  /* Add neighbors sectors to RUP set whose channel is 
     same as the channel included in RUP message, however 
     do only after A and C set sectors have been finished */
     
  if ( ( hdrsrchrup.rup_chan_nset_completed == FALSE ) && 
       ( hdrsrchrup.a_c_sets_completed == TRUE ) )
  {
    i = hdrsrchrup.rup_chan_nset_index;
      /* start from where we left in NSET */

    while ( ( hdrsrchset.rupset_cnt < HDR_MAX_RUP_SET_SIZE ) &&
            ( i < hdrsrchset.nset_cnt ) )
    {
      if ( ( SYS_CHAN_CMP_EQUAL( hdrsrchset.nset[i]->chan, chan ) ) &&
           ( hdrsrchset.nset[i]->sys_type == sys ) )
      {
        hdrsrchset_add_rup_sect( hdrsrchset.nset[i] );
          /* Add sector to RUP search from NSET */
      }
    
      i++;
    }

    hdrsrchrup.rup_chan_nset_index = i;
      /* save where we are in NSET */
      
    if ( i == hdrsrchset.nset_cnt )
    {
      hdrsrchrup.rup_chan_nset_completed = TRUE;
        /* We have completed adding neighbors to RUP set whose channel 
           is same as the channel included in RUP message */
    }
  }
  
#ifdef FEATURE_HDR_REVB
  if ( hdrsrchset_is_ofreq_chan( chan ) == FALSE )
#else
  if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.aset[0]->chan, chan ) )
#endif /* FEATURE_HDR_REVB */
  {
    hdrsrchrup.serv_chan_nset_completed = TRUE;
      /* if channel in RUP request and serving sector channel are same 
         then no need to go over the neighbor set twice */
  }
  
  /* Add neighbors sectors to RUP set whose channel is 
     same as the serving sector channel, however 
     do only after RUP channel N set sectors have been finished */ 

  if ( ( hdrsrchrup.serv_chan_nset_completed == FALSE ) && 
       ( hdrsrchrup.rup_chan_nset_completed == TRUE ) )
  {
    i = hdrsrchrup.serv_chan_nset_index;
      /* start from where we left in NSET */
      
    while ( ( hdrsrchset.rupset_cnt < HDR_MAX_RUP_SET_SIZE ) &&
            ( i < hdrsrchset.nset_cnt ) )
    {
#ifdef FEATURE_HDR_REVB
      if ( hdrsrchset_is_ofreq_chan( hdrsrchset.nset[i]->chan )  == FALSE )
        /* If the nset sector is covered by the current RF configuration */
#else
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.nset[i]->chan, hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
      {
        (void) hdrsrchset_get_rup_sect( hdrsrchset.nset[i]->pn_offset, 
                                        chan,
                                        sys );
          /* create sector in RUPSET from NSET serving sector PN */
      }
      
      i++;
    }

    hdrsrchrup.serv_chan_nset_index = i;
      /* save where we are in NSET */
        
    if ( i == hdrsrchset.nset_cnt )
    {
      hdrsrchrup.serv_chan_nset_completed = TRUE;
        /* We have completed adding neighbors to RUP set whose chan is 
           same as the serving sector channel */
    }
  }
    
  /* Add all other remaining PNs */
    
  if ( ( hdrsrchrup.rset_completed == FALSE ) && 
       ( hdrsrchrup.serv_chan_nset_completed == TRUE ) )
  {
    while ( hdrsrchset.rupset_cnt < HDR_MAX_RUP_SET_SIZE )
    {
      (void) hdrsrchset_get_rup_sect( hdrsrchrup.last_rset_pn, 
                                      chan,
                                      sys );
        /* create sector in RUPSET from RSET */
      
      if ( sys == HDRSRCH_SYSTYPE_1X )
      {
        hdrsrchrup.last_rset_pn += hdrsrch.chan_mgmt_1x_params.pilot_inc;
      }
      else
      {
        hdrsrchrup.last_rset_pn += hdrsrch.srch_params.pilot_inc;
          /* Advance to next PN in RSET */
      }
      /* Check whether we have included all RSET pilots or not */    
      if ( hdrsrchrup.last_rset_pn >= HDR_MAX_PN_OFFSET )
      {
        hdrsrchrup.last_rset_pn = 0;
          /* Reset PN for next RUP request */
          
        hdrsrchrup.rset_completed = TRUE; 
          /* We have now included all RSET pilots in RUP set */
          
        break;
      }
        
    }  
  }
  
  last_rupset = hdrsrchrup.rset_completed;
    /* if Rset is completed then all sectors to be searched for RUP request
       now have been included in the RUP set(s) */
       
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR RUP: Generated prioritized search set pilots= %d ",
                  hdrsrchset.rupset_cnt );

  return ( last_rupset );
} /* hdrsrchutil_generate_prioritized_rup_set() */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GENERATE_DIRECTED_RUP_SRCH_SET

DESCRIPTION
  Generate the RUP sectors from pilots in RUP Request.

DEPENDENCIES
  None

PARAMETERS
  sys - type of the system for the rup search. 

RETURN VALUE
  Whether the generated RUP set is the last one for the RUP request
  i.e we are done generating RUP sets for the pending RUP request.
  
SIDE EFFECTS
  None
  
=========================================================================*/

boolean hdrsrchutil_generate_directed_rup_set
(
  hdrsrch_system_enum_type    sys
    /* system type for the rup search */
)
{
  sys_channel_type            chan;
    /* only search sectors on this channel */
        
  hdrsrch_sect_struct_type    *sect;
    /* sector to be programmed in the list entry */
           
  boolean                     last_rupset;
    /* flag whether the generated RUP set is the last one for the RUP request
      i.e we are done generating RUP sets for the pending RUP request. */

  uint16                      i;
    /* loop variable */
            
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  chan = hdrsrch_rup_req.channel ;
    /* RUP search channel for brevity */
        
  hdrsrchset.rupset_cnt = 0;
    /* reset RUP set */
  
  if ( hdrsrch_rup_req.sector_count > HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE )
  {
    hdrsrch_rup_req.sector_count = HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE;
      /* Truncate the number of pilots to search */
      
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,"HDR RUP: Too many pilots to search = %d > %d, truncated. ",
                    hdrsrch_rup_req.sector_count, HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE );      
  }
  
  /* Create sectors in RUPSET from pilots specified in RUP request */   
  for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
  {
    
    /* Get the RUP sector, create if necessary */
    sect = hdrsrchset_get_rup_sect( hdrsrch_rup_req.sector_info[i].pilot_pn, 
                                    chan,
                                    sys );
    
    if ( sect != NULL )
    {
      if ( hdrsrch_rup_req.window_size_included == TRUE )
      {
        sect->nset.win_size = hdrsrch_rup_req.sector_info[i].window_size;
      }

      if ( hdrsrch_rup_req.window_offset_included == TRUE )
      {
        sect->nset.win_offset = hdrsrch_rup_req.sector_info[i].window_offset;
      }
    }  
  }

  last_rupset = TRUE;
    /* There can be at most 16 sectors included in RUP request 
       so we all should fit in RUP set in in one pass */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR RUP: Generated directed search set, Sectors= %d ",
                  hdrsrchset.rupset_cnt );
                            
  return ( last_rupset );
  
} /* hdrsrchutil_generate_directed_rup_set() */
  
/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GENERATE_RUP_SET

DESCRIPTION
  Generate the RUP search sectors from RUP Request.

DEPENDENCIES
  None

PARAMETERS
  sys - type of the system for the rup search. 

RETURN VALUE
  Whether the generated RUP set is the last one for the RUP request
  i.e we are done generating RUP sets for the pending RUP request.
  
SIDE EFFECTS
  None
  
=========================================================================*/

boolean hdrsrchutil_generate_rup_set
(
  hdrsrch_system_enum_type    sys
    /* system type for the rup search */
)
{
  boolean                     last_rupset;
    /* flag whether the generated RUP set is the last one for the RUP request
      i.e we are done generating RUP sets for the pending RUP request. */
      
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_rup_req.sector_count != 0 )
  {
    last_rupset = hdrsrchutil_generate_directed_rup_set( sys );
  }
  else
  {
    last_rupset = hdrsrchutil_generate_prioritized_rup_set( sys );  
  }

  hdrsrchofs_set_rupset_srch_index( 0 );
    /* reset index into RUP search set */
    
  return ( last_rupset );
}

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROGRAM_RUP_SEARCH_SET

DESCRIPTION
  Program a collection of searches, based on the current sets and channel.

DEPENDENCIES
  None

PARAMETERS
  sys - type of the system for the rup search. 

RETURN VALUE
  True if at least one pilot was submitted for search else false.

SIDE EFFECTS
  Starts lost dump timer.

=========================================================================*/

boolean hdrsrchutil_program_rup_search_set
(
  hdrsrch_system_enum_type    sys
    /* system type for the current channel */
)
{    
  hdrsrch_sect_struct_type    *sect;
    /* sector to be programmed in the list entry */

  boolean                      srch_programmed = TRUE;
    /* Indicate search submitted to hardware if search list is not empty */

  uint16                       index;
    /* index into the RUP set */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_init_search_list( );
    /* Reset the search list */

  hdrsrchutil.srch.system = sys;
    /* set type of system to search ( 1X or DO ) */

  /* Program the search only if the searcher is idle */
  if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
  { 
    if ( hdrsrchrfdiv_diversity_demod_is_enabled( ) ) 
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
    }
    else
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
    }

    index = hdrsrchofs_get_rupset_srch_index();
      /* where in RUP set to start adding sectors to search list */
    
    /* Reset index if it is out of bounds */    
    if ( index >= hdrsrchset.rupset_cnt )
    {
      index = 0;
    }
          
    while ( ( hdrsrchutil.srch.cnt < HDR_MAX_SRCH_CNT ) && 
            ( index < hdrsrchset.rupset_cnt ) )
    {
      ASSERT( index < HDR_MAX_RUP_SET_SIZE );
        /* Array boundary sanity check */

      sect = hdrsrchset.rupset[index];
        /* Get pointer to RUP set sector */
        
      hdrsrchutil_add_to_search_list( sect );
        /* Add RUP set sectors to search list */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR RUP:RUP search set pn=%d,chan=%d",
                      sect->pn_offset, sect->chan.chan_num);
        
      index++;
    }
    
    /* Reset index if we reached the end of RUPset */
    if ( index == hdrsrchset.rupset_cnt )
    {
      index = 0;
    }
    
    hdrsrchofs_set_rupset_srch_index( index );
      /* where in RUP set to start next time when adding sectors to search list */
    
    if ( hdrsrch_get_current_state() == HDRSRCH_OFS_MEAS_STATE )
    {
      hdrsrchutil_chk_convert_to_full_pn( );
        /* Check and convert to full PN search if NV is enabled */
    }

    hdrsrchutil_submit_search_list( );
      /* Submit search list to the hardware */
  }
    
  /* If no pilot was submitted for search */
  if ( hdrsrchutil.srch.cnt == 0 )
  {
    srch_programmed = FALSE;
  }

  return srch_programmed;

} /* hdrsrchutil_program_rup_search_set() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_PROGRAM_SEARCH_SET

DESCRIPTION
  Program a collection of searches, based on the current sets and channel.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  True if atleast one pilot was submitted for search else false

SIDE EFFECTS
  Starts lost dump timer.

===========================================================================*/

boolean hdrsrchutil_program_search_set( void )
{
  boolean srch_programmed = TRUE;
    /* Indicate search submitted to hardware if search list is not empty */

#ifdef FEATURE_HDR_REVB
  uint8                        chan_cnt;
    /* The number of channels coverred by the current RF config */

  const sys_channel_type       *chan_list;
    /* The current channel list*/

  const hdrsrch_demod_idx_type *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_init_search_list( );
    /* Reset the search list */

#ifdef FEATURE_HDR_REVB
  chan_list = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                     &chan_cnt,
                                     &demod_list );
    /* The channel list from rf module has been sorted in the order of lowest
       demod id to highest demod_id. */

  hdrsrchutil_program_searches( chan_list, chan_cnt );
    /* Add the ASET to the search list */
#else
  hdrsrchutil_program_searches( 
                           hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN ) );
    /* Determine the require pilots to search, based on the current sets
       and the current channel */
#endif /* FEATURE_HDR_REVB */

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchutil_submit_search_list( );
    /* Submit search list to the hardware */

  /* If no pilot was submitted for search */
  if ( hdrsrchutil.srch.cnt == 0 )
  {
    srch_programmed = FALSE;
  }
  
  return srch_programmed;

} /* hdrsrchutil_program_search_set( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_PROGRAM_PRI_CHAIN_ASET_SEARCH

DESCRIPTION
  Program a collection of primary chain ASET pilot search. It adds ASET to 
  search list and submits the search list to the hardware.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  True if at least one pilot was submitted for search else false

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchutil_program_pri_chain_aset_search( void )
{
  boolean srch_programmed = TRUE;
    /* Indicate search submitted to hardware if search list is not empty */
  uint8                        chan_cnt;
    /* The number of channels coverred by the current RF config */

  const sys_channel_type       *chan_list;
    /* The current channel list*/

  const hdrsrch_demod_idx_type *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_init_search_list( );
    /* Reset the search list */

  chan_list = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                     &chan_cnt,
                                     &demod_list );
    /* The channel list from rf module has been sorted in the order of lowest
       demod id to highest demod_id. */

  (void) hdrsrchutil_add_aset_to_search_list( chan_list, chan_cnt );
    /* Add the ASET to the search list */

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchutil_submit_search_list( );
    /* Submit search list to the hardware */
  
  /* If no pilot was submitted for search */
  if ( hdrsrchutil.srch.cnt == 0 )
  {
    srch_programmed = FALSE;
  }
  
  return srch_programmed;

} /* hdrsrchutil_program_pri_chain_aset_search( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_PRI_CHAIN_ASET_DUMP

DESCRIPTION
  This function processes primary chain ASET dump.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_pri_chain_aset_dump( void )
{
  boolean                    valid;
    /* Flag to indicate whether or not searcher results are valid */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  valid = hdrsrchutil_validate_searcher_results();
    /* Perform searcher result validation before using the results */

  hdrsrch_kill_signal( HDRSRCH_LOSTDUMP_SIG );
    /* Clear lostdump signal, and remove it from wait mask */

  if ( valid )
  {
    HDR_ASSERT( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
      /* KW fix : Error fatal if the list_cnt is greater than maximum  */

    hdrsrchset_calc_tot_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* Calculate search energies for the searched pilots */

    hdrsrchset_filt_pri_chain_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* Filter the primary chain energe for the searched pilots */
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "Invalid aset primary chain search dump! State= %x", 
                    hdrsrch_state_id);
  }

}  /* hdrsrchutil_pri_chain_aset_dump( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_SET_WIN_CENTER_TO_MSTR

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  Updates win_cenx2 parameter in the sector table

===========================================================================*/

void hdrsrchutil_set_win_center_to_mstr( void )
{
  int s;


#ifdef FEATURE_HDR_REVB
  uint8    i;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------- Update window center for CSET -----------*/
#ifdef FEATURE_HDR_REVB
  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for ( s = 0; s < hdrsrchset.subaset[i].aset_cnt; s++ )
    {
      hdrsrchset.subaset[i].aset[s]->win_cenx2 = (hdrsrchdrv.mstr_x8+2)/4;
    }
  }
#else
  for ( s = 0; s < hdrsrchset.aset_cnt; s++ )
  {
    hdrsrchset.aset[s]->win_cenx2 = (hdrsrchdrv.mstr_x8+2)/4;
  }
#endif /* FEATURE_HDR_REVB */

  /*-------- Update window center for CSET -----------*/
  for ( s = 0; s < hdrsrchset.cset_cnt; s++ )
  {
    hdrsrchset.cset[s]->win_cenx2 = (hdrsrchdrv.mstr_x8+2)/4;
  }

}/* hdrsrchutil_set_win_center_to_mstr */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_TRACK_WIN_CENTER

DESCRIPTION
  Updates the window center for a particular sector with the latest information
  about the earliest arriving multipath for each sector. sect->peak_pos
  denotes the earliest arriving multipath for each sector. As per IS-856,
  search windows should be centered around the earliest multipath.

  However, since peak_pos can be erroneous when pilot Ec/Io is weak which can
  cause false/noise peak to be earliest peak. Centering search window around
  false peak could cause the loss of actual pilot peak. Hence, we try to track
  the earliest peak at a slower rate. win_cenx2 denotes the intended window
  center which is tracks the peak_pos at a rate 1 chips/triage.

DEPENDENCIES
  None

PARAMETERS
  Sector for which to update search window center

RETURN VALUE
  None

SIDE EFFECTS
  Updates win_cenx2 parameter in the sector table

===========================================================================*/

LOCAL void hdrsrchutil_track_win_center
(
  hdrsrch_sect_struct_type    *sect_ptr
    /* sector being processed from the search list */
)
{
  int16                       deltax2;
    /* delta between earliest peak and prev window center */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  deltax2 = sect_ptr->peak_pos - sect_ptr->win_cenx2;
    /* delta between earliest peak and previous window center */

  /* Apply upper limit on deltax2 */
  deltax2 = deltax2 > HDRSRCH_MAX_WIN_CENTER_DELTA_X2 ?
                      HDRSRCH_MAX_WIN_CENTER_DELTA_X2 : deltax2;

  /* Apply lower limit on deltax2 */
  deltax2 = deltax2 < HDRSRCH_MIN_WIN_CENTER_DELTA_X2 ?
                      HDRSRCH_MIN_WIN_CENTER_DELTA_X2 : deltax2;

  sect_ptr->win_cenx2 += deltax2;
    /* compensate for the delta so that win_cenx2 moves towards earliest
       arriving peak */

  /*------------*
   *    Debug   *
   *------------*/

  if (LABS_VAL((int16)((int16) (hdrsrchdrv.mstr_x8/4) - sect_ptr->win_cenx2))>
                                        HDRSRCH_MAX_MSTR_WIN_CEN_ERR_X2 )
  {
    /* win_cenx2 and mstr are too far */
    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR, "Huge WinCenter Err wc=%d mstr=%d pn=%d", 
                    sect_ptr->win_cenx2,
                    (hdrsrchdrv.mstr_x8/4),
                    sect_ptr->pn_offset );

    sect_ptr->win_cenx2 = (hdrsrchdrv.mstr_x8 + 2)/4;
      /* try to restore the window center to MSTR. It has gone too far from
         MSTR. Something has gone wrong. Recover from it */
  }

} /* hdrsrchutil_track_win_center */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_UPDATE_WIN_CENTER

   This function updates the window center for different sectors in the
   search list. If just woke up from sleep then it slams the window center
   to the earliest peak position other wise it tracks window center towards
   earliest peak at a slow pace.

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS
  Sector for which to update search window center

RETURN VALUE
  None

SIDE EFFECTS
  Updates win_cenx2 parameter in the sector table

===========================================================================*/

void hdrsrchutil_update_win_center( void )
{
  int16                       s;

#ifdef FEATURE_HDR_REVB
  uint8                       i;
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchdrv.mstr_valid == FALSE )
  {
    return;
    /* Do not bother about marking the window center because MSTR is not
       valid yet. All the searches are sent w.r.t. MSTR */
  }

  /*-------- Update window center for ASET -----------*/
#ifdef FEATURE_HDR_REVB
  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for ( s = 0; s < hdrsrchset.subaset[i].aset_cnt; s++ )
    {
      if ( hdrsrchset.subaset[i].aset[s]->filt_length == 1 )
      {
        hdrsrchset.subaset[i].aset[s]->win_cenx2 = 
                                  hdrsrchset.subaset[i].aset[s]->peak_pos;
        /* if filt_length is one that means the sector eng was just reset
           due to sleep. We need to slam the window center to the earliest
           peak position since tracking could be too slow */

      }
      else
      {
        hdrsrchutil_track_win_center( hdrsrchset.subaset[i].aset[s] );
        /* otherwise track the window center slowly towards earliest arriving
           peak position */
      }
    }
  }
#else
  for ( s = 0; s < hdrsrchset.aset_cnt; s++ )
  {
    if ( hdrsrchset.aset[s]->filt_length == 1 )
    {
      hdrsrchset.aset[s]->win_cenx2 = hdrsrchset.aset[s]->peak_pos;
      /* if filt_length is one that means the sector eng was just reset
         due to sleep. We need to slam the window center to the earliest
         peak position since tracking could be too slow */

    }
    else
    {
      hdrsrchutil_track_win_center( hdrsrchset.aset[s] );
      /* otherwise track the window center slowly towards earliest arriving
         peak position */
    }
  }
#endif /* FEATURE_HDR_REVB */

  /*-------- Update window center for CSET -----------*/

  for ( s = 0; s < hdrsrchset.cset_cnt; s++ )
  {
    if ( hdrsrchset.cset[s]->filt_length == 1 )
    {
      hdrsrchset.cset[s]->win_cenx2 = hdrsrchset.cset[s]->peak_pos;
      /* if filt_length is one that means the sector eng was just reset
         due to sleep. We need to slam the window center to the earliest
         peak position since tracking could be too slow */

    }
    else
    {
      hdrsrchutil_track_win_center( hdrsrchset.cset[s] );
      /* otherwise track the window center slowly towards earliest arriving
         peak position */
    }
  }


} /* hdrsrchutil_update_win_center( ) */

#ifdef FEATURE_ATOM

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_RESET_FILTERS

DESCRIPTION
  The function resets the filters for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_operational_mode_reset_filters( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Initialize timer structures */
  hdrsrchmode.ecio_filter.accum = 0;
  hdrsrchmode.ecio_filter.num_samples = 0;
  hdrsrchmode.ecio_filter.filtered_eng = 0;  
  hdrsrchmode.ec_filter.accum = 0;
  hdrsrchmode.ec_filter.num_samples = 0;  
  hdrsrchmode.ec_filter.filtered_eng = 0;
      
} /* hdrsrchutil_operational_mode_reset_filters() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_SET_THRESHOLDS

DESCRIPTION
  The function sets the thresholds for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_operational_mode_set_thresholds( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* set algorithm thresholds */
  hdrsrchmode.threshold.poor_coverage_ecio_lo 
     = HDRSRCH_POOR_COVERAGE_PILOT_ECIO_LOW_THRESHOLD;
  hdrsrchmode.threshold.poor_coverage_ecio_hi 
     = HDRSRCH_POOR_COVERAGE_PILOT_ECIO_HIGH_THRESHOLD;
  hdrsrchmode.threshold.poor_coverage_ec_lo 
     = HDRSRCH_POOR_COVERAGE_PILOT_RXPOWER_LOW_THRESHOLD;
  hdrsrchmode.threshold.poor_coverage_ec_hi 
     = HDRSRCH_POOR_COVERAGE_PILOT_RXPOWER_HIGH_THRESHOLD;
  hdrsrchmode.threshold.good_coverage_ecio_hi 
     = HDRSRCH_GOOD_COVERAGE_PILOT_ECIO_HIGH_THRESHOLD;
     
} /* hdrsrchutil_operational_mode_set_thresholds() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_RESET_TIMERS

DESCRIPTION
  The function resets the timers for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_operational_mode_reset_timers( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Initialize timer structures */
  hdrsrchmode.dual_mode_timer_scale = HDRSRCH_DUAL_MODE_TIMER_SCALE;
  hdrsrchmode.hdr_only_mode_timer_scale = HDRSRCH_HDR_ONLY_MODE_TIMER_SCALE;  
  hdrsrchmode.ecio_timer.active = FALSE;  
  hdrsrchmode.ecio_timer.elapsed = 0;
  hdrsrchmode.ec_timer.active = FALSE;  
  hdrsrchmode.ec_timer.elapsed = 0;
  hdrsrchmode.ecio_timer2.active = FALSE;  
  hdrsrchmode.ecio_timer2.elapsed = 0;

  hdrsrchmode.indication_time = 
      hdrsrchmode.run_time - HDRSRCH_OPMODE_INDICATION_PERIOD_SCLKS;
    /* reset last indication time in sclks */

} /* hdrsrchutil_operational_mode_reset_timers() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_GET_NV_CONFIGURATION

DESCRIPTION
  The function reads the NV item configuration for  
  AT operational mode management algorithm. If necessary sets
  the algorithm mode and algorithm active flag. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_operational_mode_get_nv_configuration( void )
{

  hdrsrch_opmode_config_enum_type      nv_config;
    /* NV item configuration for AT operational mode algorithm */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  nv_config = (hdrsrch_opmode_config_enum_type) 
              HDRSRCH_NV_FORCE_OPMODE_CONFIGURATION;
  
  if ( ( nv_config == HDRSRCH_OPMODE_POOR_COVERAGE_DETECTION_ENABLED ) || 
       ( nv_config == HDRSRCH_OPMODE_GOOD_COVERAGE_DETECTION_ENABLED ) )
  {
    hdrsrchmode.config = nv_config;
      /* use NV forced configuration */
      
    hdrsrchmode.active = TRUE;
      /* AT operational mode is now active */
  }
  
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "OPMODE: NV Config= %d, Active= %d ", 
                  nv_config,
                  hdrsrchmode.active );
      
} /* hdrsrchutil_operational_mode_get_nv_configuration() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_ALGORITHM_INIT

DESCRIPTION
  The function initializes the structure for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_operational_mode_algorithm_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize operational mode algorithm state */
  hdrsrchmode.active = FALSE;
  hdrsrchmode.config = HDRSRCH_OPMODE_DISABLED;
  hdrsrchmode.first_run = TRUE;
  hdrsrchmode.current_mode = HDRSRCH_HDR_ONLY_MODE;
  hdrsrchmode.hdr_ecio = 0;
  hdrsrchmode.hdr_ec = 0;
    
  hdrsrchutil_operational_mode_reset_filters();
    /* Initialize filter structures */
    
  hdrsrchutil_operational_mode_set_thresholds();
    /* set thresholds for algorithms */

  hdrsrchutil_operational_mode_reset_timers();
    /* Initialize timer structures */
    
  /* initialize indication call back functions */
  hdrsrchmode.poor_coverage_cb = NULL;
  hdrsrchmode.good_coverage_cb = NULL;
  
  hdrsrchutil_operational_mode_get_nv_configuration();
    /* get NV item based configuration for AT operational mode */
        
} /* hdrsrchutil_operational_mode_algorithm_init() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_CONFIGURE_OPMODE

DESCRIPTION
  The function configures callback and algorithm enable flags for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_configure_opmode
( 
  hdrsrch_opmode_config_enum_type      new_config,
    /* which at operational mode algorithm if any to run */
  
  hdrsrch_opmode_indication_cb_type    poor_coverage_cb,
    /* cb to indicate poor HDR coverage */

  hdrsrch_opmode_indication_cb_type    good_coverage_cb
    /* cb to indicate good HDR coverage */
)
{
  hdrsrch_opmode_config_enum_type      previous_config;
    /* previous operational mode algorithm configuration */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
          
  if ( HDRSRCH_NV_FORCE_OPMODE_CONFIGURATION == 0x0 )
  /* no NV forced configuration so honor protocol's request */
  {
    previous_config = hdrsrchmode.config;
      /* remember old configuration */
  
    if ( new_config <= HDRSRCH_OPMODE_GOOD_COVERAGE_DETECTION_ENABLED )
    {
      hdrsrchmode.config = new_config;
        /* set the new configuration */
    }
    else
    {
      hdrsrchmode.config = HDRSRCH_OPMODE_DISABLED;
        /* set the new configuration */
    }
      
    if ( new_config == HDRSRCH_OPMODE_DISABLED )
    {
      hdrsrchutil_operational_mode_algorithm_init();
        /* reinitialize operational mode algorithm 
           hdrsrchmode.active flag will be FALSE 
           and config will be HDRSRCH_OPMODE_DISABLED */
    }
    else
    {
      if ( new_config != previous_config )
      {
        hdrsrchutil_operational_mode_reset_timers();
          /* reset timers if configuration has changed */    
      }
    
      hdrsrchmode.active = TRUE;
        /* AT operational mode is now active */
    }
  }
  
  /* operational mode algorithm callback functions */
  
  hdrsrchmode.poor_coverage_cb = poor_coverage_cb;
    /* set cb to indicate poor HDR coverage */
      
  hdrsrchmode.good_coverage_cb = good_coverage_cb;
    /* set cb to indicate good HDR coverage */
    
} /* hdrsrchutil_configure_opmode() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPMODE_TIME_KEEPER

DESCRIPTION
  The function calculates the elapsed time sinde the last run time of the 
  AT operational mode management algorithm and updates the run time stamp.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Elasped time since last opmode algorithm execution in ms

SIDE EFFECTS
  None
  
===========================================================================*/

uint16 hdrsrchutil_opmode_time_keeper( void )
{
  timetick_type                       current_time;
    /* the current time in sysclks */

  timetick_type                       elapsed_time_sclks;
    /* elasped time since last opmode algorithm execution in sysclks */

  uint16                              elapsed_time_ms = 0;
    /* elasped time since last opmode algorithm execution in ms */
                                                    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Mark the last time the opmode algorithm executed */
  current_time = timetick_get();
    /* current time in sclks */

  /* Check whether this is the first time algorithm executes after power up */
  if ( hdrsrchmode.first_run == TRUE )
  {
    hdrsrchmode.run_time = current_time;
      /* record last run time in sclks */
        
    hdrsrchmode.log_time = current_time;   
      /* record last message log time in sclks */

    hdrsrchmode.indication_time = 
       current_time - HDRSRCH_OPMODE_INDICATION_PERIOD_SCLKS;
      /* initialize last indication time in sclks */
    
    hdrsrchmode.first_run = FALSE;
      /* the first run of the algorithm is being executed */    
  }
        
  elapsed_time_sclks = current_time - hdrsrchmode.run_time;
    /* elapsed time in sclks from the previous execution */
    
  elapsed_time_ms = timetick_cvt_from_sclk( elapsed_time_sclks , T_MSEC );
    /* calculate elapsed time in ms */
    
  hdrsrchmode.run_time = current_time;
    /* update run time in sclks */
    
  return ( elapsed_time_ms );
  
} /* hdrsrchutil_opmode_time_keeper() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPMODE_CONFIGURE_TIMERS

DESCRIPTION
  The function configures the timer durations for 
  AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Flag whether this is a valid searcher state to run the
  operational mode algorithm.

SIDE EFFECTS
  None
  
===========================================================================*/

boolean hdrsrchutil_opmode_configure_timers( void )
{
  uint16                              sci;
    /* last sci value */
    
  uint32                              period_ms;
    /* sci value converted to sleep period in ms */
   
  boolean                             valid_state = TRUE;
    /* flag whether opmode algorithm should be run in current HDRSRCH state */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
          
  if ( ( hdrsrch_state_id == HDRSRCH_REACQ_STATE ) || 
       ( hdrsrch_state_id == HDRSRCH_IDLE_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE ) )
  {
    sci = hdrsrchsleep_get_last_sleep_sci();
    /* get sci value for the last sleep cycle*/
    
    /* If invalid SCI then default to SCI for 5.12 sec */
    if ( ( sci > 12 ) || ( sci < 5 ) )
    {
      sci = 9;
    }
  
    if ( sci >= 7 )
    {
      period_ms = ( ( ( 3 * HDRTS_SLOTS_PER_SCC ) << ( sci - 7 ) ) * 5 ) / 3;
        /* convert sci to sleep period in ms 
           3 * 256 slots * ( 5/3 ms/slot ) * 2^(sci - 7) */
    }
    else
    {
      period_ms = ( ( HDRTS_SLOTS_PER_SCC >> ( 6 - sci ) ) * 5 ) / 3;
        /* convert sci to sleep period in ms 
           256 slots * ( 5/3 ms/slot ) / 2^(6 - sci) */      
    }
  
    hdrsrchmode.ecio_timer.duration = hdrsrchmode.dual_mode_timer_scale * period_ms ;
    
    if ( hdrsrchmode.ecio_timer.duration > HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE )
    {
      hdrsrchmode.ecio_timer.duration = HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE;
    }
    
    hdrsrchmode.ec_timer.duration = hdrsrchmode.dual_mode_timer_scale * period_ms;
    
    if ( hdrsrchmode.ec_timer.duration > HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE )
    {
      hdrsrchmode.ec_timer.duration = HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE;
    }
        
    hdrsrchmode.ecio_timer2.duration = hdrsrchmode.hdr_only_mode_timer_scale * period_ms;
    
    if ( hdrsrchmode.ecio_timer2.duration > HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE )
    {
      hdrsrchmode.ecio_timer2.duration = HDRSRCH_IDLE_TIME_TO_TRIGGER_MAX_TIMER_VALUE;
    }
  }
  else if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
  {
    hdrsrchmode.ecio_timer.duration = HDRSRCH_TC_DUAL_MODE_TIME_TO_TRIGGER;
    hdrsrchmode.ec_timer.duration = HDRSRCH_TC_DUAL_MODE_TIME_TO_TRIGGER;
    hdrsrchmode.ecio_timer2.duration = HDRSRCH_TC_HDR_ONLY_MODE_TIME_TO_TRIGGER;
  }
  else
  {
    valid_state = FALSE;
      /* do not run opmode algorithm in other states */
  }
  
  return ( valid_state );
  
} /* hdrsrchutil_opmode_configure_timers() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPMODE_POOR_COVERAGE_DETECTION_ALGORITHM

DESCRIPTION
  The function implements the AT operational mode management 
  algorithm to decide whether poor RF condition to enable the dual
  mode has been satisfied.

DEPENDENCIES
  None

PARAMETERS
  Elapsed time since the previous run time.

RETURN VALUE
  None

SIDE EFFECTS
  May generate indication messages to upper layers.

===========================================================================*/

void hdrsrchutil_opmode_poor_coverage_detection_algorithm
(
  uint16                              elapsed_time_ms
    /* elasped time since last opmode algorithm execution in ms */
)
{
  boolean                             dual_mode_enable_trigger = FALSE;
    /* Flag whether to enable dual mode */
                        
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Run AT operational mode dual mode enable algorithm */
  
  if ( hdrsrchmode.hdr_ecio <= hdrsrchmode.threshold.poor_coverage_ecio_hi )
  {
    if ( hdrsrchmode.ecio_timer.active == FALSE )
    {
      hdrsrchmode.ecio_timer.active = TRUE;
        /* Turn on timer */
          
      hdrsrchmode.ecio_timer.elapsed = 0;
        /* Reset dual mode trigger Ec/Io timer */      
    }
    else
    {
      hdrsrchmode.ecio_timer.elapsed += elapsed_time_ms;
        /* Increment timer by elapsed time from last search */
    }
    
    if ( hdrsrchmode.hdr_ecio <= hdrsrchmode.threshold.poor_coverage_ecio_lo )
    {
      dual_mode_enable_trigger = TRUE; 
        /* Send indication to upper layers to enable dual mode */   
    }
    else if ( hdrsrchmode.ecio_timer.elapsed > hdrsrchmode.ecio_timer.duration )
    {
      dual_mode_enable_trigger = TRUE; 
        /* Send indication to upper layers to enable dual mode */
    }
  }
  else
  {
    hdrsrchmode.ecio_timer.active = FALSE;
      /* Turn off timer */
           
    hdrsrchmode.ecio_timer.elapsed  = 0;
      /* Reset dual mode trigger Ec/Io timer */
  }
    
  if ( hdrsrchmode.hdr_ec <= hdrsrchmode.threshold.poor_coverage_ec_hi )
  {
    if ( hdrsrchmode.ec_timer.active == FALSE )    
    {
      hdrsrchmode.ec_timer.active = TRUE;
        /* Turn on timer */
        
      hdrsrchmode.ec_timer.elapsed = 0;
        /* Reset dual mode trigger Ec timer */      
    }
    else
    {
      hdrsrchmode.ec_timer.elapsed += elapsed_time_ms;
        /* Increment timer by elapsed time from last search */
    }

    if ( hdrsrchmode.hdr_ec <= hdrsrchmode.threshold.poor_coverage_ec_lo )
    {
      dual_mode_enable_trigger = TRUE; 
        /* Send indication to upper layers to enable dual mode */   
    }
    else if ( hdrsrchmode.ec_timer.elapsed > hdrsrchmode.ec_timer.duration )
    {
      dual_mode_enable_trigger = TRUE; 
        /* Send indication to upper layers to enable dual mode */
    }
  }
  else
  {
    hdrsrchmode.ec_timer.active = FALSE;
      /* Turn off timer */
    
    hdrsrchmode.ec_timer.elapsed = 0;
      /* Reset dual mode trigger Ec timer */
  }
    
  if ( dual_mode_enable_trigger == TRUE )
  {
    if ( ( hdrsrchmode.run_time - hdrsrchmode.indication_time ) >= 
           HDRSRCH_OPMODE_INDICATION_PERIOD_SCLKS )      
    {
      hdrsrchmode.indication_time = hdrsrchmode.run_time;
        /* record last indication time in sclks */
        
      if ( hdrsrchmode.poor_coverage_cb != NULL )
      {
        hdrsrchmode.poor_coverage_cb();
          /* call the call back function for bad HDR RF condition */
      }
          
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "OPMODE: Sent Poor RF DualModeTriggerInd, EcIo= %d, Ec= %d ",
                      hdrsrchmode.hdr_ecio,
                      hdrsrchmode.hdr_ec );
        /* Send indication to upper layers to enable dual mode */        
    }
  }
    
} /* hdrsrchutil_opmode_poor_coverage_detection_algorithm() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPMODE_GOOD_COVERAGE_DETECTION_ALGORITHM

DESCRIPTION
  The function implements the AT operational mode management 
  algorithm to decide whether good RF condition to disable the dual
  mode has been satisfied.

DEPENDENCIES
  None

PARAMETERS
  Elapsed time since the previous run time.

RETURN VALUE
  None

SIDE EFFECTS
  May generate indication messages to upper layers.

===========================================================================*/

void hdrsrchutil_opmode_good_coverage_detection_algorithm
(
  uint16                              elapsed_time_ms
    /* elasped time since last opmode algorithm execution in ms */
)
{
  boolean                             dual_mode_disable_trigger = FALSE;
    /* Flag whether to enable dual mode */
                        
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
       
  /* Run AT operational mode dual mode disable algorithm */
  
  if ( hdrsrchmode.hdr_ecio >= hdrsrchmode.threshold.good_coverage_ecio_hi )
  {
    if ( hdrsrchmode.ecio_timer2.active == FALSE )
    {
      hdrsrchmode.ecio_timer2.active = TRUE;
        /* Turn on timer */
          
      hdrsrchmode.ecio_timer2.elapsed = 0;
        /* Reset HDR only mode trigger Ec/Io timer */      
    }
    else
    {
       hdrsrchmode.ecio_timer2.elapsed += elapsed_time_ms;
         /* Increment timer by elapsed time from last search */
    }
      
    if ( hdrsrchmode.ecio_timer2.elapsed > hdrsrchmode.ecio_timer2.duration )
    {
      dual_mode_disable_trigger = TRUE;
        /* Send indication to upper layers to disable dual mode */
    }
  }  
  else
  {
    hdrsrchmode.ecio_timer2.active = FALSE;
      /* Turn off timer */
      
    hdrsrchmode.ecio_timer2.elapsed = 0;
      /* Reset HDR only mode trigger Ec/Io timer */      
  }

  if ( dual_mode_disable_trigger == TRUE )
  {
    if ( ( hdrsrchmode.run_time - hdrsrchmode.indication_time ) >= 
           HDRSRCH_OPMODE_INDICATION_PERIOD_SCLKS )      
    {
      hdrsrchmode.indication_time = hdrsrchmode.run_time;
        /* record last indication time in sclks */
        
      if ( hdrsrchmode.good_coverage_cb != NULL )
      { 
        hdrsrchmode.good_coverage_cb();
          /* call the call back function for good HDR RF condition */
      }
            
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "OPMODE: Sent Good RF HDROnlyModeTriggerInd, EcIo= %d, Ec= %d ",
                      hdrsrchmode.hdr_ecio,
                      hdrsrchmode.hdr_ec );
      /* Send indication to upper layers to disable dual mode */                        
    }
  }
    
} /* hdrsrchutil_opmode_good_coverage_detection_algorithm() */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_OPERATIONAL_MODE_ALGORITHM

DESCRIPTION
  The function implements the AT operational mode management algorithm.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May generate indication messages to upper layers.

===========================================================================*/

void hdrsrchutil_operational_mode_algorithm
(
  hdrsrchdrv_srch_list_struct_type   *srch_list,
    /* search sweep list */

  uint8                               srch_cnt
    /* search count in the list */
)
{
  uint16                              elapsed_time_ms;
    /* elasped time since last opmode algorithm execution in ms */
    
  boolean                             valid_state;
    /* flag whether opmode algorithm should be run in current HDRSRCH state */                
                                                
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      
  elapsed_time_ms = hdrsrchutil_opmode_time_keeper();
    /* elasped time since last opmode algorithm execution in ms */
    
  valid_state = hdrsrchutil_opmode_configure_timers();
    /* configure timers for either idle or traffic state operation */
    
  /* Only run opmode algorithm in idle or traffic states */ 
  if ( valid_state == FALSE )
  {
    return;
  }
  
  hdrsrchset_opmode_filt_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
    /* Run Ec/Io and Ec filters for AT operational mode management */
      
  /* Run AT operational mode algorithm */
  if ( hdrsrchmode.config == HDRSRCH_OPMODE_POOR_COVERAGE_DETECTION_ENABLED )
  /* && ( hdrsrchmode.current_mode == HDRSRCH_HDR_ONLY_MODE ) */  
  {
    hdrsrchutil_opmode_poor_coverage_detection_algorithm( elapsed_time_ms );
      /* Run AT poor coverage detection algorithm to switch to dual mode */
  }
  else if ( hdrsrchmode.config == HDRSRCH_OPMODE_GOOD_COVERAGE_DETECTION_ENABLED )
  /* && ( ( hdrsrchmode.current_mode == HDRSRCH_HYBRID_MODE ) ||
          ( hdrsrchmode.current_mode == HDRSRCH_DUAL_MODE ) ) */
  
  {
    hdrsrchutil_opmode_good_coverage_detection_algorithm( elapsed_time_ms );
      /* Run AT good coverage detection algorithm to switch to hdr only mode */
  }
      
  /* Display Operational mode algorithm status periodically */
  if ( ( hdrsrchmode.run_time - hdrsrchmode.log_time ) >= 
       HDRSRCH_OPMODE_MESSAGE_LOG_INTERVAL_SCLKS )
  {
    hdrsrchmode.log_time = hdrsrchmode.run_time;
      /* record last message time in sclks */
        
    HDR_MSG_SRCH_9( MSG_LEGACY_HIGH, "OPMODE: Config=%d EcIo=%d, Ec= %d, T1=%d of %d, T2= %d of %d; T3= %d of %d.",
                    hdrsrchmode.config,
                    hdrsrchmode.hdr_ecio,
                    hdrsrchmode.hdr_ec,
                    hdrsrchmode.ecio_timer.elapsed,
                    hdrsrchmode.ecio_timer.duration,
                    hdrsrchmode.ec_timer.elapsed,
                    hdrsrchmode.ec_timer.duration,                    
                    hdrsrchmode.ecio_timer2.elapsed,
                    hdrsrchmode.ecio_timer2.duration );
  }
  
} /* hdrsrchutil_operational_mode_algorithm() */

#endif /* FEATURE_ATOM */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_UPDATE_NUM_SECTOR_USERS

DESCRIPTION
  Updates the number of active users in serving cell information.
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

void hdrsrchutil_update_num_sector_users
( 
  uint16                              num_active_users
    /* number of active users in serving sector */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* ARDA is not currently supported so this is not currently needed. */

} /* hdrsrchutil_update_num_sector_users() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_PROCESS_SEARCH_DUMP

DESCRIPTION
  Processes the search results for the just completed search.
  Active set processing and triage is not performed.

DEPENDENCIES
  An active search dump

PARAMETERS
  None

RETURN VALUE
  Number of new reports

SIDE EFFECTS
  Clears lost dump timer.

===========================================================================*/

int hdrsrchutil_process_search_dump( void )
{
  int                                 new_rpts = 0;
   /* number of new reports generated during set evaluation */

  uint8                               i;
    /* Loop variable */

  hdrsrchdrv_srch_list_struct_type    *srch_list_ptr;
    /* Pointer to beginning of each carrier's srch results */

  uint8                               num_srch_results;
    /* Number of srch results in srch list per carrier */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*----------------------------------------------------------
    Stop the lost dump timer - the search dump arrived in time
  ----------------------------------------------------------*/

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* Stop the timer, clear its signal, and remove from wait mask */

  /*------------------------*
   * Get Finger Status Dump *
   *------------------------*/

  hdrsrchfing_read_fing_status();
    /* read the finger status registers and update the finger list */
  hdrsrcheq_read_eq_status( );
    /* Read equalizer status registers */
  
  /*-----------------------------------*
   * Log searcher/finger info to QXDM  *
   *-----------------------------------*/

  hdrsrchlog_srch_diagnostics( FALSE );
    /* log diagnostics packet with rate governer */

  /*---------------------------------------------------------*
   * Filter the pilot energy, perform triage if required,    *
   * and do set maintainence. Loop through each carrier to   *
   * check if the srch results are valid first.              *
   *---------------------------------------------------------*/

  srch_list_ptr = hdrsrchdrv.srch_list;
    /* Initialize pointer to beginning of each carrier's srch list.
       This assumes the srch list has already been sorted per carrier */
  for ( i = 0; i < hdrsrchdrv.num_carriers; i++ )
  {

    num_srch_results = hdrsrchdrv.carrier_info[i].srch_task_cnt;
   
    /* Skip carriers with invalid srch results */
    if ( hdrsrchdrv.carrier_info[i].srch_valid )
    {

      hdrsrchset_calc_tot_eng( srch_list_ptr, num_srch_results );
        /* Filter search energies for the searched pilots into the
           respective search pilot structures */

      /* Maintain the primary chain filtered energy if it's non-diversity search */
      if ( hdrsrchdrv.srch_task_type == HDRSRCH_NON_DIV_SRCH )
      {
        hdrsrchset_filt_pri_chain_eng( srch_list_ptr, num_srch_results );
      }

      hdrsrchset_filt_eng( srch_list_ptr, num_srch_results );
        /* Filter search energies for sectors in search list */
    
      hdrsrchset_identify_earliest_peak( srch_list_ptr,
                                         num_srch_results );
        /* Identifies the earliest peak with sufficient energy and stores
           the position of the peak for use in the RUP msg */

      /* Candidate, Neighbour and Remaining Set Management ... */
      new_rpts += hdrsrchset_manage_sets( srch_list_ptr,
                                         num_srch_results );                  

#ifdef FEATURE_ATOM

      /* If active run mode management algorithm */    
      if ( hdrsrchmode.active == TRUE )
      {
        hdrsrchutil_operational_mode_algorithm( srch_list_ptr, 
                                                num_srch_results );
          /* Run AT operational mode management algorithm */
      }
  
#endif /* FEATURE_ATOM */
  
    } /* if srch results valid */

    srch_list_ptr += num_srch_results;
      /* Point to the beginning of the next carrier's srch results, if any */

  } /* for loop for each carrier */

  hdrsrchutil_update_max_ecio();
    /* Update max ecio */

  hdrsrchutil_update_rssi();
    /* Update RSSI*/

  hdrsrchlog_srch_diagnostics_pilot_sets( FALSE );
    /* Log pilot sets related info with rate governer */

  return new_rpts;
 
} /* hdrsrchutil_process_search_dump( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_PROCESS_OFS_SEARCH_DUMP

DESCRIPTION
  Processes the search results for the just completed OFS search.
  Active set processing and triage is not performed.

DEPENDENCIES
  None

PARAMETERS
  Flag to whether perform set management or not. 

RETURN VALUE
  Number of new reports

SIDE EFFECTS
  Clears lost dump timer.

===========================================================================*/

int hdrsrchutil_process_ofs_search_dump( boolean manage_sets )
{
  int                             new_rpts;
   /* number of new reports generated during set evaluation */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* Stop the timer, clear its signal, and remove from wait mask */

  HDR_ASSERT( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
    /* KW fix : Error fatal if the list_cnt is greater than maximum  */

  if ( hdrsrchdrv.system == HDRSRCH_SYSTYPE_1X )
  {
    hdrsrchset_get_1x_ecio_equivalent_energy( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* calculate equivalent DO search energy for given 1x Ec/Io result */
    
    hdrsrchset_identify_1x_peak( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* Find the peak location for 1x pilots */    
  }
  else
  {
    hdrsrchset_calc_tot_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* Filter search energies for the searched pilots into the
         respective search pilot structures */
       
    hdrsrchset_identify_earliest_peak( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
      /* Identifies the earliest peak with sufficient energy and stores
         the position of the peak for use in the RUP msg */  
  }
  
  hdrsrchset_filt_ofs_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
    /* Filter the OFS search results*/
  
  if ( manage_sets == TRUE )
  {
    /* Candidate, Neighbour and Remaining Set Management ... */
    new_rpts = hdrsrchset_manage_sets( hdrsrchdrv.srch_list,
                                       hdrsrchdrv.list_cnt );
  }
  else
  {
    new_rpts = 0;
      /* skipped set maintenance this time so no new reports. 
         however we still performed pilot filtering as above */
  }

  return ( new_rpts );

} /* hdrsrchutil_process_ofs_search_dump( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_INVALIDATE_SEARCH_DUMP

DESCRIPTION
  this function invalidates the searcher results. It performs the processing
  needed for the dump except for using the search results. This function will
  be called when the search results are found to be invalid (due to incorrect
  AGC resulting in elevated noise floor). This function should ensure that the
  search results are not used as well as all the necessary processing should
  be done to issue a new search.

DEPENDENCIES
  Results in ignoring the searcher results for the search cycle

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Clears lost dump timer.

===========================================================================*/

void hdrsrchutil_invalidate_search_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "Invalidate search dump! State=%x Count=%d", 
                  hdrsrch_state_id,
                  hdrsrchdrv.dump_cnt );

  /*----------------------------------------------------------
    Stop the lost dump timer - the search dump arrived in time
  ----------------------------------------------------------*/

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* Stop the timer, clear its signal, and remove from wait mask */

  /*------------------------*
   * Get Finger Status Dump *
   *------------------------*/

  hdrsrchfing_read_fing_status();
    /* read the finger status registers and update the finger list */

  hdrsrcheq_read_eq_status( );
    /* Read equalizer status registers */

  /*-----------------------------------*
   * Log searcher/finger info to QXDM  *
   *-----------------------------------*/

  hdrsrchlog_srch_diagnostics( FALSE );
    /* log diagnostics packet with rate governer */

  hdrsrchlog_srch_diagnostics_pilot_sets( FALSE );
    /* Log pilot sets related info with rate governer */

  /*-------------------------------------------*
   * Update the HDR data level indicator SINR  *
   *-------------------------------------------*/

  hdrsrchutil_update_data_ind_sinr();
    /* Update the HDR data indicator SINR */

  hdrsrchutil_update_max_ecio();
    /* Update max ecio */

  hdrsrchutil_update_rssi();
    /* Update RSSI*/

} /* hdrsrchutil_invalidate_search_dump ( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_PROCESS_SEARCH_TRIAGE

DESCRIPTION
  Assigns fingers to search peaks

DEPENDENCIES
  An active search dump

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_process_search_triage( void )
{

  const sys_channel_type       *chan_ptr;  
    /* Temp var used to pass into macro */

#ifdef FEATURE_HDR_REVB
  uint8                        chan_cnt, i;
    /* Channel count */

  const hdrsrch_demod_idx_type *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */ 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifndef FEATURE_HDR_REVB
  /* Store result in a temp pointer to avoid using function call in 
     SYS_CHAN_CMP_EQUAL macro as a fix to lint error 666 */
  chan_ptr = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );

  /* Only perform triage if we are on the channel on the active
     set channel */

  if ( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel, *chan_ptr ) )
    /* TBD: do we need this channel check in revB */
#else
  chan_ptr = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                    &chan_cnt,
                                    &demod_list );

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Process MC triage, chan_cnt=%d", chan_cnt ); 

  for( i=0; i<chan_cnt; i++ )
  {
      HDR_MSG_SRCH_6( MSG_LEGACY_LOW, 
                    "chan[%d].band=%d, chan[%d].chan_num=%d, demo_list[%d]=%d", 
                      i, chan_ptr[i].band,
                      i, chan_ptr[i].chan_num,
                      i, demod_list[i] );
  }
#endif /* FEATURE_HDR_REVB */
  {

    /* Track the number of pilots in the aset, cset, and nset every triage
       cycle.  This info is logged in search's connected state stats 
       packet.*/
    hdrsrchlog_inc_search_set_stats_counter( &hdrsrchset );

    hdrsrchlog_inc_conn_state_stats_counter(
      HDRSRCHLOG_TRIAGE_CYCLE_COUNT, 1 );
      /* Now track how many triage cycles have occurred so we can calculate
         the average number of pilots in each set. */

    hdrsrchtri_triage(hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt);
      /* Ensure fingers are on the best pilots */
    
  }

  /*------------------------------------------------------------------*
   * Update the HDR data level indicator SINR after finger assignment *
   *------------------------------------------------------------------*/

  hdrsrchutil_update_data_ind_sinr();
    /* Update the HDR data indicator SINR */

} /* hdrsrchutil_process_search_triage( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_PROCESS_SEARCH_SET

DESCRIPTION
  Processes the search results for the just completed search.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Clears lost dump timer.

===========================================================================*/

void hdrsrchutil_process_search_set
(
  hdrsrch_aset_mgmt_cb_type       aset_mgmt
    /* Callback for Active Set management */
)
{
  int8                            new_rpts;
   /* number of new reports generated during set evaluation */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  new_rpts = hdrsrchutil_process_search_dump( );
    /* Stop lost dump timer, get finger status, filter
       search energies into sets, perform C, N & R -set
       maintainence */

  /* Active Set Management ... ( state-dependent callback func) */
  if ( aset_mgmt != NULL )
  {
    aset_mgmt( new_rpts );
  }

  hdrsrchutil_process_search_triage();
    /* Perform triage on search results */

  hdrsrchutil_update_win_center();
    /* Update the window center for each sector as per earliest multipath */

} /* hdrsrchutil_process_search_set( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_UPDATE_NSET_PILOTS

DESCRIPTION
  Updates the neighbour list with new pilots.
  Pilots already in the Active Set or the Candidate Set are not demoted
  to the neighbour set.

DEPENDENCIES
  None

PARAMETERS
  nset_pilots          - Array of pilots to put into NSET
  num_nset_pilots      - Number of pilots in the array
  update_type          - Type of update - BROADCAST or DIRECTED message
  channel_list         - List of hash channels from SP broadcast message
  num_channels         - Number of hash channels in channel list
  avoided_channel_list - List of avoided channels
  num_avoided_channels - Number of avoided channels in the channel list
  serving_pilot_gid    - Group id of the pilot from which the nset update is rx'ed
  hdr_subtype          - HDR protocol subtype

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  NSet pilots are aged.

===========================================================================*/

boolean hdrsrchutil_update_nset_pilots
(
  const hdrsrch_nset_pilot_type *nset_pilots,
    /* Array of pilots to put in neighbour set */

  int                            num_nset_pilots,
    /* Number of pilots to put into neighbour set */

  hdrsrch_nset_update_enum_type  update_type,
    /* Broadcast or directed update */

  const sys_channel_type         *channel_list,
    /* List of hash channels from the SP BROADCAST message*/

  uint8                          num_channels,
    /* Number of hash channels in the channel list */

  const sys_channel_type         *avoided_channel_list,
    /* List of avoided channels */

  uint8                          num_avoided_channels
    /* Number of avoided channels in the channel list */

#ifdef FEATURE_HDR_REVB
  ,hdrsrch_pilot_gid_type        serving_pilot_gid,
    /* The gid of the pilot from which the SP is received */

  sys_active_prot_e_type          hdr_subtype
    /* HDR Protocol subtype */
#endif /* FEATURE_HDR_REVB */
  , const sys_channel_type        *pref_chan
    /* The prefered channel for nset update */
)
{
  int                       ch;
    /* Loop index for channels in hash channel list */

  boolean                   transitions = FALSE;
    /* Flag indicating if any set transitions occurred */

  uint8                          i, j;
    /* Loop variables */

  hdrsrchset_pilotpurge_struct_type         pilotpurge;
    /* Contains input ptr and default action type to be passed to the pilot purge
       function. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchset_age_nset_pilots( );
    /* Increase the age of all pilots in the neighbour set. */

#ifdef FEATURE_REMOVE_OFF_BAND_NEIGHBORS
  hdrsrchset_remove_off_band_neighbors( nset_pilots,
                                        num_nset_pilots,
                                        update_type );
    /* This is done to avoid off frequency searching across bands unless the
       new AT specifically lists the cross-band pilot in its SP. */
#endif /* FEATURE_REMOVE_OFF_BAND_NEIGHBORS */

#ifdef FEATURE_HDR_REVB

  /* Check if transitioning from non-revB to revB mode. */
  if ( ( hdrsrchstate_is_revb_mode() == FALSE ) &&
       ( hdr_subtype == SYS_ACTIVE_PROT_HDR_RELB ) )
  {
    hdrsrchset_pilot_group_purge_cnset_mc();
      /* Purge all sets since Rev0A keeps all pilots, i.e., even with same
         PGID. */
  }

  /* Use RUP subtype to determine RevB mode.  This is useful to
     check for Pilot Group ID (RevB) or not (Rev 0A) when checking
     pilot equality. */
  if ( hdr_subtype == SYS_ACTIVE_PROT_HDR_RELB )
  {
    hdrsrchstate_set_revb_mode( TRUE );
      /* Set RevB mode */
  }
  else
  {
    hdrsrchstate_set_revb_mode( FALSE );
    /* Not RevB mode */
  }

  /* Update serving sector's PGID.  Only overwrite in non-traffic state with a
     valid PGID.  In traffic states, PGID is only from TCA. */
  if ( ( HDRSRCHSTATE_IS_TRAFFIC_STATE == FALSE )  &&
       ( serving_pilot_gid != HDRSRCH_NOT_INCLUDED_PILOT_GID ) )
  {
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pilot_gid =
      serving_pilot_gid;
  }
#endif /* FEATURE_HDR_REVB */

  transitions = hdrsrchset_add_nset_pilots( nset_pilots, 
                                            num_nset_pilots, 
                                            update_type,
                                            pref_chan );
    /* Add the pilots to nset */

#ifdef FEATURE_HDR_REVB

  /* Since this function is called in both idle and traffic, loop through all
     subasets' asets and purge.
     Note: It is safe to start the loop at 0 since an ASET pilot will never be
           purged. */

  for ( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for ( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      pilotpurge.sect_ptr = hdrsrchset.subaset[i].aset[j];
      pilotpurge.action = HDRSRCHSET_PILOTPURGE_ACTION_NO_ACTION;
        /* Reset action to default value */
      hdrsrchset_pilot_group_purge_mc( &pilotpurge, NULL );
        /* Enforce group policy based on the current serving sector */
    }
  }

#endif /* FEATURE_HDR_REVB */

  hdrsrchmsg_send_multi_asp_update_msg( TRUE );
    /* All the purging above may have added/dropped GCSET's.  Need to send ASP update. */

  /* The following order is important:
      1. Reset nset search parameters to default
      2. Update with broadcast
      3. Update with directed */

  hdrsrchset_reset_nset_search_window_params();
    /* Set search window parameters to default */

  hdrsrchset_update_nset_search_window_params( HDRSRCH_NSET_UPDATE_BROADCAST );
    /* Update with broadcast parameters */

  hdrsrchset_update_nset_search_window_params( HDRSRCH_NSET_UPDATE_DIRECTED );
    /* Update with directed update parameters */

  /* If NSET update is received in Idle Monitor, Idle BC or BC Access */
  if ( hdrsrch_state_id == HDRSRCH_IDLE_STATE 
#ifdef FEATURE_HDR_BCMCS
       || hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE 
       || hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE 
#endif /* FEATURE_HDR_BCMCS */
     )
  {
    /* Delete pilots that have the same PN as the serving sector and whose 
       channel is different and is on the hash channel list. This is to avoid
       ping-pong between channel hashing and O-freq handoffs. */

    /* Fixing KW errors */  
    HDR_ASSERT( num_channels<= HDRSRCH_HASH_CHAN_MAX_SIZE);

    for ( ch = 0; ch < num_channels; ch++ )
    {
/* SPIBI (Same PN Inter-Band Idle Handoffs) */
      if(hdrmc_feature_is_enabled(
         HDRMC_FEATURE_HDR_ALLOW_SPIBI_HANDOFFS))
      {
      if ( channel_list[ch].band == 
              hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan.band )
      {
        HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Discarding pn=%d, BC=%d, chan=%d from nset", 
                        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
                        channel_list[ch].band,
                        channel_list[ch].chan_num );

      /* Delete other pilots with same PN on channels in hash channel list */
        hdrsrchset_remove_same_pn_on_diff_channel( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0], 
                                                 TRUE,
                                                 &channel_list[ch] );
      }
      } /* if(hdrmc_feature_is_enabled(HDRMC_FEATURE_HDR_ALLOW_SPIBI_HANDOFFS)) */
      else
      {
        HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Discarding pn=%d, BC=%d, chan=%d from nset", 
                        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
                        channel_list[ch].band,
                        channel_list[ch].chan_num );

      /* Delete other pilots with same PN on channels in hash channel list */
        hdrsrchset_remove_same_pn_on_diff_channel( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0], 
                                                 TRUE,
                                                 &channel_list[ch] );
      } /* else */
    } /* for ch */     
  } /* if hdrsrch_state_id */

  /* Remove sectors that are on the avoided channels */
  for ( ch = 0; ch < num_avoided_channels; ch++ )
  {
    hdrsrchset_remove_sectors_on_chan( &avoided_channel_list[ch] );
  }

  hdrsrchofs_notify_nset_update();
    /* notify OFS state that NSET update has been done 
       so that other frequency list can be updated as well */
  
  return transitions;

} /* hdrsrchutil_update_nset_pilots( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_UPDATE_CACHED_SECTORS

DESCRIPTION
  Update the cached sectors in protocol. The information is used by IHO.

DEPENDENCIES
  None

PARAMETERS
  cached_sects     - Array of cached sectors
  num_sects        - Number of cached sectors

RETURN VALUE
  None
SIDE EFFECTS
  none

===========================================================================*/

void hdrsrchutil_update_cached_sectors
( 
  const sys_link_type        *cached_sects,
    /* The cached sectors */

  uint8                      num_sects
    /* The number of cached sectors */
)
{
  uint8                      i;
    /* Index to sectors */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.cached_sects.num_sects = num_sects;
    /* Set number of cached sectors */

  /* Fixing KW errors */  
  HDR_ASSERT( num_sects <= HDRSRCH_MAX_CACHE_RECORDS);

  for ( i=0; i<num_sects; i++ )
  {
    hdrsrchutil.cached_sects.sects[ i ] = cached_sects[ i ];
      /* Copy cached sectors */
  }
}  /* hdrsrchutil_update_cached_sectors */

/*===========================================================================

FUNCTION HDRSRCHUTIL_SECTOR_IS_CACHED

DESCRIPTION
  Check whether a sector is cached.

DEPENDENCIES
  None

PARAMETERS
  sector       - The sector to check whether it is cached

RETURN VALUE
  TRUE         - The sector is cached
  FALSE        - The sector is not cached
SIDE EFFECTS
  none

===========================================================================*/

boolean hdrsrchutil_sector_is_cached
(
  hdrsrch_sect_struct_type   *sector
    /* Sector to check whethe it is cached */
)
{
  uint8                      i;
    /* Index to sectors */

  boolean                    cached = FALSE;
    /* Whether the sector is cached */

  sys_link_type              *pilot;
    /* Pilot pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sector != NULL )
  {
  
    for ( i = 0; i < hdrsrchutil.cached_sects.num_sects; i++ )
    {
      
      pilot                  = & hdrsrchutil.cached_sects.sects[i];

      if ( SYS_CHAN_CMP_EQUAL( sector->chan, pilot->channel )
           && ( sector->pn_offset == pilot->pilot_pn) )
      {
        cached               = TRUE;
        break;
      }
    } /* for */
  
  }  /* if */

  return cached;

}  /* hdrsrchutil_sector_is_cached */

#ifdef FEATURE_HDR_REVA_L1
/*===========================================================================

FUNCTION HDRSRCHUTIL_RA_CHANNEL_GAIN_DB_TO_LIN

DESCRIPTION
  This function converts RA channel gain from dB to linear. The formula is:
  
  ROUND(0.4*4*(sqrt(10^(RAChannelGain/10))) * 4096)
  
  The valid input is -6, -9, -12 or -15dB.

DEPENDENCIES
  None

PARAMETERS
  ra_cgannel_gain_db     - RA channel gain in dB unit

RETURN VALUE
  Linear RA channel gain

SIDE EFFECTS
  None

===========================================================================*/

uint16 hdrsrchutil_ra_channel_gain_db_to_lin
( 
  int8                       ra_channel_gain_db 
    /* RA channel gain in dB unit */
)
{

  uint16                     ra_channel_gain_lin;
    /* Linear RA channel gain */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( ra_channel_gain_db > -6 ) || ( ra_channel_gain_db < -15 ) )
  {
    ra_channel_gain_lin      = HDRSRCH_RA_CHANNEL_GAIN_LIN_DEFAULT;
  }
  else
  {
    ra_channel_gain_lin      =  
      ra_channel_gain_db_to_lin_table[ ( -ra_channel_gain_db - 6 ) / 3 ];
  }

  return ra_channel_gain_lin;

}  /* hdrsrchutil_ra_channel_gain_to_lin() */
#endif /* FEATURE_HDR_REVA_L1 */

#ifndef FEATURE_HDR_REVB
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_UPDATE_ASET_PILOTS

DESCRIPTION
  Updates the active set list with new active set pilots.

DEPENDENCIES
  Connected State

PARAMETERS
  aset_pilots     - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchutil_update_aset_pilots
(
  const hdrsrch_aset_pilot_type *aset_pilots,
    /* Array of pilots to put in active set */

  int                            num_aset_pilots
    /* Number of pilots to put into active set */
)
{
  hdrsrch_sect_struct_type  *sect;
    /* Sector pointer */

  int                       i, j;
    /* Loop indexes for pilots */

  boolean                   found;
    /* Flag - Pilot has been found in aset_pilots list */

  boolean                   transitions = FALSE;
    /* Flag indicating if any set transitions occurred */

  uint8                     rpc_cell_idx;
    /* Reverse Power Control cell index - for Softer Handoff */

#ifdef FEATURE_HDR_REVA_L1

  uint16                    fing_assign_status;
    /* Finger mask */

  uint8                     dsc;
    /* DSC */

#endif /* FEATURE_HDR_REVA_L1 */

  hdrsrch_sect_struct_type  *tca_sect[ HDR_MAX_ASET_SIZE ];
    /* Active Set sectors, in order appearing in TCA msg */

  uint16                     rab_len;
    /* RAB length in number of slots as sent by the AN */

  boolean disjoint = TRUE;
    /* Indicates that the active pilot is being updated with a disjoint set. 
       Initialize assuming it will be disjoint. */

  uint8         asp_idx;
    /* The best asp index read from mdsp */

  int8                       c = 0;
    /* demod carrier */
    /* TBD */
    /* Need to get the demod carrie either as a parameter or from sector struct */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*------------------------------------------------------------------------
     Check if the best active pilot from mdsp is disjoint from
     the new active set.  Do this BEFORE old ASET demotion.
     Note: If old ASET partially disjoint from new ASET, active pilot might be
     disjoint from new ASET.
  ------------------------------------------------------------------------*/

  asp_idx = HDRASP_GET_BASP_FL( );
    /* Read the current best FL ASP index */

  for ( i = 0; i < hdrsrchset.aset_cnt; i++ )
  {
    sect = hdrsrchset.aset[i];

    /* Compare index from mdsp to active set indices */
    if ( sect->aset.asp_idx == asp_idx )
    {
      /* Found matching index.  Compare active pn against new active set */
      for ( j=0; j<num_aset_pilots; j++ )
      {
        if ( sect->pn_offset == aset_pilots[j].pilot_pn )
        {
          /* Found best ASP in new ASET.  Not disjoint active pilot */
          disjoint = FALSE;
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Active pilot found in new ASET" );
          break;  /* Get out of new ASET for loop */
        }
      }
      break;  /* Get out of old ASET for loop */
    }

  } /* for each entry in the aset */

  /*------------------------------------------------------------------------
    Demote any ASET pilot which does not exist in the new ASET list
     [6.6.5.6.1.2]
  ------------------------------------------------------------------------*/

  for ( i = hdrsrchset.aset_cnt-1; i >= 0;  i-- )
  {
    sect = hdrsrchset.aset[i];

    found = FALSE;

    for ( j=0; j<num_aset_pilots; j++)
    {
      if ( sect->pn_offset == aset_pilots[j].pilot_pn )
      {
        found = TRUE;
        break;
      }
    }

    if ( !found )
    {
      /* Drop this pilot -- not in active set anymore */

      hdrsrchset_drop_from_aset( sect );
        /* Drop the sector from the aset */

      transitions = TRUE;
        /* We have made set transitions */
    }
  }

  /* If best active pilot does not exist in ASET update,
     we need to do a slightly different ASET init for this case. 
     (See FRAB below) */
  if (disjoint == TRUE)
  {

    hdrsrchfing_deassign_all_fings( );
      /* Unassign fingers */

    hdrsrchutil_dec_deactivate( );
      /* Deactivate HDR decoder to cancel all active packets */

    hdrdec_reset_pkt_sequences();
      /* Reset packet sequence numbers */

    hdrdec_activate();
      /* Activate HDR decoder after switching frequencies. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "Best active pilot not in new ASET. Cancel active packets");

  } 

#ifdef FEATURE_HDR_REVA_L1

  if ( transitions == TRUE )
  {
    (void) hdrsrchfing_program_fings();
      /* Deassign the fingers and commit to firmware */
    
    fing_assign_status =  hdrsrch_wait_for_high_pri_rsp
                                        ( 
                                          HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US, 
                                          HDRSRCH_FING_ASSIGN_RSP 
                                        );
      /* Wait for finger deassignment done */

    if ( fing_assign_status == FALSE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Finger assignment response is not received" );
    }

  } /* if transitions == TRUE */

#endif /* FEATURE_HDR_REVA_L1 */

  /*------------------------------------------------------------------------
    Add pilots in new ASET list that are not in old ASET [6.6.5.6.1.2]
  ------------------------------------------------------------------------*/

  for ( i=0; i < num_aset_pilots ; i++)
  {
    sect = hdrsrchset_get_sect( aset_pilots[i].pilot_pn,
                                hdrsrch.aset_channel );
      /* Find or create pilot sector for this pilot */

    if( sect != NULL )
    {
      tca_sect[i] = sect;
        /* Save this sector pointer, to avoid repeated searches */
  
      if ( sect->set != HDR_ASET )
      {
        /* New Active Set Pilot */
  
        if ( sect->set != HDR_CSET )
        {
          /* any sector that is coming to ASET from N/R set should
             have its win_cenx2 initialized to MSTR */
          sect->win_cenx2 = (hdrsrchdrv.mstr_x8 + 2)/4;
        }
  
        hdrsrchset_add_to_aset( sect );
          /* ... move the sector to the active set */

#ifdef FEATURE_HDR_REVA_L1
        /*  Initialization of FRAB should be cleared if entire aset is updated
            with disjoint set. */
#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
        if (disjoint)
        {
          /* For disjoint set, init frab and qrab for this ASP: 
             aspsFRABSoft = 0, aspsQRABSoft = 0 */
          HDRASP_SET_FRAB_SOFT( HDRMDSP_DEMOD_CARRIER_0,
                                sect->aset.asp_idx, 
                                0 );
        }
        else
        {
          /* Init frab and qrab for this ASP: 
             aspsFRABSoft = frab, aspsQRABSoft = 0 */
          HDRASP_SET_FRAB_SOFT( HDRMDSP_DEMOD_CARRIER_0, 
                                sect->aset.asp_idx, 
                                HDRRAB_GET_FRAB( HDRMDSP_DEMOD_CARRIER_0 ) );
        }

        HDRASP_SET_QRAB_SOFT( HDRMDSP_DEMOD_CARRIER_0,
                              sect->aset.asp_idx, 
                              0 );
#endif
#endif /* FEATURE_HDR_REVA_L1 */
 
        sect->drop_timer.expired  = FALSE;
        sect->drop_timer.active   = FALSE;
          /* This is a new pilot - start with non expired drop timer */
  
        sect->aset.rpc_cell_idx   = 0;    /* Unassigned, for now */
        sect->aset.drc_cover      = 0;    /* Unassigned, for now */
  
#ifdef FEATURE_HDR_REVA_L1
       
        sect->aset.ra_channel_gain_lin = HDRSRCH_RA_CHANNEL_GAIN_LIN_DEFAULT;
          /* Default RA channel gain */
  
        sect->aset.dsc            = 0;    
          /* Unassigned, for now */
  
#endif /* FEATURE_HDR_REVA_L1 */
  
        transitions = TRUE;
          /* We have made pilot set transitions */
  
        /* Since this is a new active set pilot just rxed from the AN,
           initialize other fields in the structure to defaults */
        sect->nset.win_size       = HDRSRCH_WINSIZE_DEFAULT;
        sect->nset.win_offset     = HDRSRCH_WINOFFSET_DEFAULT;
 
     }
    }
  }  /* if ( sect != NULL ) */


  /*------------------------------------------------------------------------
    Set DRC, and MAC index for pilots in active Set.
    Begin RPC Cell Index generation based on softer handoff.
  ------------------------------------------------------------------------*/

  rpc_cell_idx = 0;
    /* Previous Pilot's Reverse Power Control (RPC) cell index = None */

#ifdef FEATURE_HDR_REVA_L1

  dsc          = 0;
    /* DSC is set to NULL */

#endif /* FEATURE_HDR_REVA_L1 */

  for ( i=0; i < num_aset_pilots ; i++)
  {
    sect = tca_sect[i];
      /* Get sector corresponding to aset_pilots[i] in TCA msg */

    sect->aset.mac_idx = aset_pilots[i].mac_index;
      /* Copy the MAC index for this pilot */

    sect->aset.drc_cover = aset_pilots[i].drc_cover;
    /* Copy the DRC cover value */

    HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                   sect->aset.asp_idx, 
                   sect->pn_offset );
      /* Write PN offset */

    HDRASP_SET_MAC_INDEX( HDRMDSP_DEMOD_CARRIER_0, 
                          sect->aset.asp_idx, 
                          sect->aset.mac_idx );
      /* Set the MAC index in the ASP structure now. Different from MSM5500 */

    HDRASP_SET_RL_MAC_INDEX( HDRMDSP_DEMOD_CARRIER_0, 
                          sect->aset.asp_idx, 
                          sect->aset.rl_mac_idx );

    HDRASP_SET_RAB_MAC_INDEX( HDRMDSP_DEMOD_CARRIER_0, 
                          sect->aset.asp_idx, 
                          sect->aset.rab_mac_idx );

    HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0, 
                          sect->aset.asp_idx, 
                          sect->aset.drc_cover );
    /* Write the DRC walsh cover for the new ASP */

    HDRASP_SET_CELL_MAP( HDRMDSP_DEMOD_CARRIER_0, 
                         sect->aset.asp_idx, 
                         sect->aset.rpc_cell_idx );
    /* Write the RPC Cell map to the asp structure */

    HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0, 
                        sect->aset.asp_idx, 
                        sect->link_id );
    /* Write the Link ID as well */

    rab_len = (uint16) hdrsrchutil_rab_length[aset_pilots[i].rab_length];
    /* Read the RAB length field from the table */

    HDRASP_SET_RAB_LENGTH( HDRMDSP_DEMOD_CARRIER_0, 
                           sect->aset.asp_idx, 
                           rab_len );
    /* Set the RAB Length parameter. */

    HDRASP_SET_RAB_OFFSET( HDRMDSP_DEMOD_CARRIER_0, 
                           sect->aset.asp_idx,
             aset_pilots[i].rab_offset * (rab_len / 8) );
    /* Set the RAB Offset parameter. The standard specifies that the RAB
       offset is set to the value equal to "RAB OFFSET FIELD * RAB_LEN/8"*/

    if ( aset_pilots[i].softer_handoff  &&  rpc_cell_idx != 0 )
    {
      sect->aset.rpc_cell_idx = rpc_cell_idx;
        /* In softer Reverse Power Control with preceding pilot */
    }

    rpc_cell_idx = sect->aset.rpc_cell_idx;
      /* Remember this pilot's RPC Cell index */

#ifdef FEATURE_HDR_REVA_L1

    sect->aset.delta_t2p = aset_pilots[i].delta_t2p;

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "PilotPN:%d, DeltaT2P:%d", 
                    aset_pilots[i].pilot_pn,
                    aset_pilots[i].delta_t2p );

    if ( aset_pilots[i].softer_handoff == FALSE )
    {
      dsc = aset_pilots[i].dsc;
        /* A new cell */
    }

    sect->aset.dsc = dsc;

    HDRASP_SET_DSC( HDRMDSP_DEMOD_CARRIER_0,
                    sect->aset.asp_idx, 
                    sect->aset.dsc);
      /* Write the DSC to the asp structure */

    sect->aset.ra_channel_gain_lin = 
      hdrsrchutil_ra_channel_gain_db_to_lin( aset_pilots[i].ra_channel_gain );

    HDRASP_SET_RA_CHANNEL_GAIN( HDRMDSP_DEMOD_CARRIER_0,
                                sect->aset.asp_idx, 
                                sect->aset.ra_channel_gain_lin );
      /* write the linear value of RAChannelgain for the new ASP */

#endif /* FEATURE_HDR_REVA_L1 */

  }/* for i */


  /*------------------------------------------------------------------------
    Reverse populate the Active Set Pilot RPC Cell Indexes.
    This is probably not needed ... the base station will likely send any
    new pilot that is in soft handoff with an old pilot AFTER the old pilot
    in the TCA message.  But in case it is sent before ...
  ------------------------------------------------------------------------*/

  for ( i=num_aset_pilots-1; i>0; i-- )
  {
    if ( aset_pilots[i].softer_handoff )
    {
      /* This pilot is in Softer Handoff with preceding pilot! */

      tca_sect[i-1]->aset.rpc_cell_idx = tca_sect[i]->aset.rpc_cell_idx;
        /* Preceding pilot's RPC cell index is the same as this pilot's. */
    }
  }


  /*------------------------------------------------------------------------
    Forward populate any Null RPC Cell index sectors in active set.
  ------------------------------------------------------------------------*/

  for ( i=0; i<num_aset_pilots; i++ )
  {
    if ( tca_sect[i]->aset.rpc_cell_idx == 0 )
    {
      if ( aset_pilots[i].softer_handoff )
      {
        /* This pilot is in Softer Handoff with preceding pilot! */

        if ( i > 0 )
        {
          tca_sect[i]->aset.rpc_cell_idx = tca_sect[i-1]->aset.rpc_cell_idx;
            /* This pilot's RPC cell index is the same as this preceding's. */
        }
        else
        {
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, "First pilot has NULL RPC Cell Index" );
        }
      }
      else
      {
        tca_sect[i]->aset.rpc_cell_idx = hdrsrchset_get_new_rpc_idx();
          /* Retrieve a new RPC cell index */
      }
    }
    /* Update ASP structure with the new RPC Cell settings */
    HDRASP_SET_CELL_MAP( HDRMDSP_DEMOD_CARRIER_0,
                         tca_sect[i]->aset.asp_idx,
                         tca_sect[i]->aset.rpc_cell_idx );
  }


  /*------------------------------------------------------------------------
    Apply any sector parameter changes to fingers assigned to the sector
  ------------------------------------------------------------------------*/

  hdrsrchfing_update_all_sectors( );
    /* Get fingers to sit up and take notice. */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Changed ASET for new pilots" );

  /*----------------------------*
   * Log pkt for change in ASET *
   *----------------------------*/
  if ( transitions )
  {
    hdrsrchlog_aset_change_log();
  }

  return transitions;

} /* hdrsrchutil_update_aset_pilots( ) */
#endif /* !FEATURE_HDR_REVB */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_FORCE_WINCENTER_TO_MSTR

DESCRIPTION
  Update the srch window center to the current mstr.

DEPENDENCIES

PARAMETERS
  sect  - Sector to update

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_force_wincenter_to_mstr
(
  hdrsrch_sect_struct_type  *sect
    /* Sector to update */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( hdrsrchdrv.mstr_valid ) 
  {
     sect->win_cenx2 = (hdrsrchdrv.mstr_x8 + 2)/4;
  }
} /* void hdrsrchutil_update_wincenter */


#ifdef FEATURE_HDR_REVB
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ASET_COMMIT_TO_DSP

DESCRIPTION
  Commit the current Active Set to the HDR DSP ASP interface. Note in revB
  , firmware does not have a "dropping" interface. Once the ASET is updated,
  finger structure should be updated as well to indicate the dropping of a
  carrier/subaset/demod carrier.

DEPENDENCIES
  Idle and connected State. Need immediate finger programming to deassigne 
  the fingers from unused demod carriers.

PARAMETERS
  None

RETURN VALUE
  Demod carrier mask

SIDE EFFECTS
  None

===========================================================================*/

uint8 hdrsrchutil_aset_commit_to_dsp ( void )
{
  uint8  i, j; 
    /* loop idex*/

  hdrsrch_sect_struct_type   *sect;
    /* Convenience pointer */

  uint8                      demod_mask = 0;
    /* Demod mask */

  boolean                    locked_mode = TRUE;
    /* Serving sector handoff control locked mode */

  int8                       ref_subaset_idx = 0;
    /* Reference subactive set index */

  uint8                      demod_idx = 0;
    /* Demod index - loop variable */

  hdrsrchutil_rab_init_ctrl_enum_t rab_init_ctrl = HDRSRCH_RAB_NO_CHANGE;
    /* FRAB /QRAB init control */

  boolean                    asp_config_status;
    /* Flag indicating whether ASP update response received or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  asp_config_status = 
           hdrsrch_wait_for_high_pri_rsp( HDR_MAX_WAIT_ASP_UPDATE_RESP_USEC, 
                                          HDRSRCH_ASP_UPDATE_RSP );
    /* Wait for the previous configuration response, or wait for 1/2 slot
       for the response */

  if ( asp_config_status == FALSE )
  {
    ERR( "Unable to commit gCSET, ASP update response is outstanding",0,0,0 );
    return demod_mask;
  } 

  hdrsrchmsg_init_handoff_cfg_msg();
    /* Init handoff config message */

  hdrsrchmsg_init_drc_cfg_msg();
    /* Init DRC config message */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  for ( demod_idx = 0;
        demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx )
  {
    HDRLOG_CLEAR_DEMOD_CARRIER_VALID( demod_idx );
      /* Clear valid bits for all carriers. Will be enabled again only 
         for subasets from the recent TCA */
  }
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

  for ( j=0; j < hdrsrchset.subaset_cnt; j++ )
  {
    demod_idx = hdrsrchset.subaset[j].demod_idx;
      /* Get the demod index */

    /* RABSoft init control is only valid in connected state */
    if( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE || 
        hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
    {
      if( hdrsrchset.subaset[j].tca_disjoint )
      {
        rab_init_ctrl = HDRSRCH_RESET_QRAB_FRAB;
          /* Reset QRAB & FRAB in init control for disjoint subaset sectors */
      }
      else
      {
        rab_init_ctrl = HDRSRCH_RESET_QRAB_ONLY;
          /* Otherwise reset the QRAB only for now.
             note: furthur control of this is per sector below */ 
      }
    }

    for ( i=0; i < hdrsrchset.subaset[j].aset_cnt; i++)
    {
      sect = hdrsrchset.subaset[j].aset[i]; 
        /* Get sector corresponding to aset_pilots[i] in TCA msg */

      if( rab_init_ctrl == HDRSRCH_RESET_QRAB_ONLY && !sect->aset.new_sect )
      {
        rab_init_ctrl = HDRSRCH_RAB_NO_CHANGE;
          /* For non disjoint subaset, only new sector needs to reset QRAB,
             otherwise, no change */
      }

      sect->aset.new_sect = FALSE;
        /* Reset new sector indication flag */

      hdrsrchmsg_set_rab_init_ctrl( demod_idx, 
                                    sect->aset.asp_idx,
                                    rab_init_ctrl );
        /* Set FRABSoft/QRABSoft initialization control */

      if( hdrsrchset.subaset[j].cc_carrier )
      {
        hdrsrchmsg.handoff_cfg_msg.ccDemodCarrier = demod_idx;
        hdrsrchmsg.handoff_cfg_msg.ccDemodCarrier_Valid = HDRSRCHMSG_CC_CARRIER_VALID;
      }

      HDRASP_ACTIVATE( demod_idx,
                       sect->aset.asp_idx );
        /* Activate ASP */

      HDRASP_SET_PN( demod_idx,
                     sect->aset.asp_idx, 
                     sect->pn_offset );
        /* Write PN offset */

      HDRASP_SET_MAC_INDEX( demod_idx, 
                            sect->aset.asp_idx, 
                            sect->aset.mac_idx );
        /* Set the MAC index in the ASP structure now. Different from MSM5500 */

      HDRASP_SET_PHY_CARRIER_INDEX( demod_idx, 
                                    sect->aset.asp_idx, 
                                    hdrsrchset.subaset[j].csr_idx );

      HDRASP_SET_RL_MAC_INDEX( demod_idx, 
                            sect->aset.asp_idx, 
                            sect->aset.rl_mac_idx );

      HDRASP_SET_RAB_MAC_INDEX( demod_idx, 
                            sect->aset.asp_idx, 
                            sect->aset.rab_mac_idx );

      HDRASP_SET_DRC_COVER( demod_idx, 
                            sect->aset.asp_idx, 
                            sect->aset.drc_cover );
        /* Write the DRC walsh cover for the new ASP */

      HDRASP_SET_AUX_DRC_COVER( demod_idx, 
                                sect->aset.asp_idx, 
                                sect->aset.aux_drc_cover );

      HDRASP_SET_LINK_ID( demod_idx, 
                          sect->aset.asp_idx, 
                          sect->link_id );

      HDRASP_SET_RAB_LENGTH( demod_idx, 
                             sect->aset.asp_idx, 
                             sect->aset.rab_length );
        /* Set the RAB Length parameter. */

      HDRASP_SET_RAB_OFFSET( demod_idx, 
                             sect->aset.asp_idx,
                     sect->aset.rab_offset * ( sect->aset.rab_length / 8 ) );
      /* Set the RAB Offset parameter. The standard specifies that the RAB
         offset is set to the value equal to "RAB OFFSET FIELD * RAB_LEN/8"*/

      HDRASP_SET_DSC( demod_idx,
                      sect->aset.asp_idx, 
                      sect->aset.dsc);
        /* Write the DSC to the asp structure */

      HDRASP_SET_RA_CHANNEL_GAIN( demod_idx,
                                  sect->aset.asp_idx, 
                                  sect->aset.ra_channel_gain_lin );
        /* Write the linear value of RAChannelgain for the new ASP */

      HDRASP_SET_CELL_MAP( demod_idx,
                           sect->aset.asp_idx,
                           sect->aset.rpc_cell_idx );
        /* Cell map update */

      hdrsrchmsg_asp_update_set_asp_mask( demod_idx,
                                          sect->aset.asp_idx );
        /* Set the ASP mask in ASP update message */

#ifdef FEATURE_HDR_REVC
      HDR_MSG_SRCH_5( MSG_LEGACY_LOW, "nlb: ch:%d pn:%d demod idx:%d aps idx:%d loadadj:%d",
                      sect->chan.chan_num, sect->pn_offset, demod_idx, sect->aset.asp_idx, sect->aset.loadadj );

      HDRASP_SET_LOAD_ADJ( demod_idx, 
                           sect->aset.asp_idx, 
                           sect->aset.loadadj );
#endif 
    }/* for ( i=0; i < hdrsrchset.subaset[j].aset_cnt; i++) */

    hdrsrchmsg_asp_update_set_carrier_mask( demod_idx );
      /* Set the carrier mask in ASP update message */

    /* LJL */
    /* TBD Set to override for bringup */

    hdrsrchmsg_drc_cfg_set_drc_ovrd( demod_idx, HDRSRCHMSG_DRC_PRED_DSP_CTRL );
      /* Predictor controlled by MDSP */

    demod_mask |= 1 << demod_idx;
      /* Update the demod mask */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
    HDRLOG_SET_DEMOD_CARRIER_VALID( hdrsrchset.subaset[j].demod_idx );
      /* Set the carrier as valid for logging purposes */
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

#ifdef FEATURE_HDR_REVC
      HDRASP_SET_NLB_K( demod_idx, hdrsrchset.subaset[j].NLB_K );
#endif 
  }/* for ( j=0; j < hdrsrchset.subaset_cnt; j++ ) */

  /* Retrieve the handoff locked mode attribute from FMAC. The attribute is valid 
     only for MC_FMAC. For revA/rev0 traffic, the locked mode ( default ) should 
     be used. */

  if( hdrfmac_serving_sector_locked_across_sub_asets( &locked_mode ) 
                                                                != E_SUCCESS )
  {
    locked_mode = TRUE;
      /* Locked_mode is set for rev0 and revA by default */
  }

  /* Set the handoff mode control to firmware */

  if( locked_mode )
  {
    /* Set the handoff locked mode and the handoff reference subactive set. */

    ref_subaset_idx = hdrsrchset_get_ref_subaset_idx();

    ASSERT( ( ref_subaset_idx != -1 ) && 
            ( ref_subaset_idx < HDRSRCH_MAX_NUM_SUBASETS ) );
      /* Make sure we have a valid ASET */

    /* Set the handoff lock mode */
    hdrsrchmsg_ho_cfg_set_ref_carrier( 
                               hdrsrchset.subaset[ref_subaset_idx].demod_idx );
    hdrsrchmsg_ho_cfg_set_ho_ctrl( HDRSRCHMSG_LOCKED_HO_MODE );

    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Handoff locked mode is set with demod[%d]!",
                    hdrsrchset.subaset[ref_subaset_idx].demod_idx );
  }
  else
  {
    /* Set the handoff unlocked mode */
    hdrsrchmsg_ho_cfg_set_ho_ctrl( HDRSRCHMSG_UNLOCKED_HO_MODE );

    HDR_MSG_SRCH( MSG_LEGACY_MED, "Handoff unlocked mode is set!" );
  }

#ifdef FEATURE_HDR_REVC
  HDRASP_SET_DEMOD_ALL_CARRIER(hdrsrchutil.demod_all_carriers);
  if ( hdrsrchutil.demod_all_carriers )
  {
    HDRASP_SET_NLB_THRESHOLD(HDRSRCHUTIL_NLB_BACKOFF_THRESHOLD);
  }
  else
  {
    HDRASP_SET_NLB_THRESHOLD(HDRSRCHUTIL_NLB_BACKOFF_DISABLED);
  }
#endif

  hdrsrchmsg_send_msg( &hdrsrchmsg.handoff_cfg_msg.hdr, 
                       sizeof( hdrfw_handoff_cfg_msg_t ) );
    /* Send handoff config message */

  hdrsrchmsg_send_msg( &hdrsrchmsg.drc_cfg_msg.hdr, 
                       sizeof( hdrfw_drc_cfg_msg_t ) );
    /* Send DRC config message */

  hdrsrchschdgrp_commit_to_dsp();
    /* Commit the scheduler group id to the mdsp */

  return demod_mask;
} /* hdrsrchutil_aset_commit_to_dsp */
#endif /* FEATURE_HDR_REVB */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_ASET_IDLE_HANDOFF

DESCRIPTION
  Sets the ASET pilot to the given sector

DEPENDENCIES
  Idle State (Monitor / Acess)
  New pilot is assumed to be on the current channel

PARAMETERS
  sect  - Sector to become the new "Active Sector"

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_aset_idle_handoff
(
  hdrsrch_sect_struct_type  *sect
    /* New active set sector */
)
{
#ifdef FEATURE_HDR_REVB
  hdrsrch_sect_struct_type        
          *old_sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0];
    /* Old active set sector */

  uint8                            j;
    /* Loop index */

  uint8                            demod_carrier_mask = 0;
    /* Demod Carrier Mask to pass to fingers */
#else
  hdrsrch_sect_struct_type        *old_sect = hdrsrchset.aset[0];
    /* Old active set sector */

  uint8                           i;
    /* Index to aset pilot */
#endif /* FEATURE_HDR_REVB */

  uint16                          hstr;
    /* HSTR count, for temporary storage  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Drop all the old aset pilots except the first one. In BC idle state,
     there may be multiple aset pilots.
     
     We want to keep the first aset pilot until the new aset pilot is added. 
     Since this is in dile handoff, other tasks may assume aset pilot always
     exists. This might be the same reason to define the local variable 
     old_sect. */

#ifdef FEATURE_HDR_REVB

  hdrsrchmsg_init_asp_update_msg();
    /* Init the ASP update message */

  /* In Idle state, the number of subasets has to remain 1. if more than 1 
     happens, keep the subaset[0] and drop the reset. */
  if( hdrsrchset.subaset_cnt > 1 )
  {
    /* This is not expected. If comes from TC state, the subaset count should
       be reduced to 1 (cc carrier) already. Error fatal */
    ERR_FATAL( "IHO: unexpected subasetcnt(%d)", hdrsrchset.subaset_cnt, 0, 0 );
  }

  if( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt > 1 )
  {
    for( j=hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt-1; j>0; j-- )
    {
      hdrsrchset_drop_from_aset( 
                         hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[j] );
    }
  }

  /* Mark the new aset channel after handoff */

  hdrsrch.aset_channel_cnt = 1;
  hdrsrch.aset_channel[0]  = sect->chan;
  hdrsrch.aset_demod[0]    = HDRSRCH_DEMOD_IDX_0;
#else 
  if ( hdrsrchset.aset_cnt > 1 )
  {
    for( i=hdrsrchset.aset_cnt-1; i>0; i-- )
    {
      hdrsrchset_drop_from_aset( hdrsrchset.aset[i] );
    }
  }

  hdrsrch.aset_channel     = sect->chan;
    /* mark the new aset channel after handoff */
#endif /* FEATURE_HDR_REVB */

  hdrsrchset_add_to_aset( HDRSRCH_IDLE_SUBASET_IDX, sect );
    /* Add new sector to active set */

#ifdef FEATURE_HDR_REVB
  hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel = sect->chan;
    /* Update subaset channel attribute */

  if( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx != 
                                                 HDRMDSP_DEMOD_CARRIER_0 )
  {
    if( old_sect == sect )
    {
      HDRASP_DEACTIVATE( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx,
                         old_sect->aset.asp_idx );
        /* Deactivate the ASP if IHO to the same sector but just force demod to 0 */

      HDRASP_SET_DRC_COVER( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx, 
                            old_sect->aset.asp_idx, 0 );
        /* Clear the DRC cover for invalid asp */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "IHO: the same sector, deactivate the ASP in smem %d",
                      old_sect->aset.asp_idx );

      hdrsrchmsg_set_asp_update_mask( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx,
                                      old_sect->aset.asp_idx );
        /* Update the carrier and ASP mask */
    }

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "IHO: change demod_idx from %d to %d",
                    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx,
                    HDRMDSP_DEMOD_CARRIER_0 );

    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx = 
                                                  HDRMDSP_DEMOD_CARRIER_0;

    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].csr_idx = 
                                                  HDRMDSP_DEMOD_CARRIER_0;
      /* CSR index in the RF side is not changed yet. Set the value before
         the tunning. This is the difference b/w traffic and idle.  */
  }

  ASSERT( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].cc_carrier );
    /* It should be a cc carrier */
#endif /* FEATURE_HDR_REVB */

  sect->drop_timer.active   = FALSE;
  sect->drop_timer.expired  = FALSE;
    /* Start with no drop timer running for this pilot */

  sect->aset.drc_cover      = 0;
    /* No DRC cover - currently */

#ifdef FEATURE_HDR_REVA_L1

  sect->aset.ra_channel_gain_lin = HDRSRCH_RA_CHANNEL_GAIN_LIN_DEFAULT;
    /* Default RA channel gain */

  sect->aset.dsc            = 0;
    /* No DSC - currently */

#endif /* FEATURE_HDR_REVA_L1 */

  sect->aset.mac_idx        = HDRSRCH_INVALID_TRAFFIC_MAC_INDEX;
    /* Set the mac index to be invalid since we aren't in traffic. */

#ifdef FEATURE_HDR_REVB
  ASSERT( sect->aset.subaset_idx < HDRSRCH_MAX_NUM_SUBASETS );
    /* Array boundary sanity check */

  sect->aset.rpc_cell_idx   = 
                           hdrsrchset_get_new_rpc_idx( sect->aset.subaset_idx );
    /* Set Reverse Power Control (RPC) cell index */
#else
  sect->aset.rpc_cell_idx   = hdrsrchset_get_new_rpc_idx();
    /* Set Reverse Power Control (RPC) cell index */
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVB
  demod_carrier_mask = hdrsrchutil_aset_commit_to_dsp();
    /* Update the aset change to the firmware */

  hdrsrchfing_set_demod_carrier_mask( demod_carrier_mask );
    /* Update demo carrier mask to finger driver */

  hdrsrchfing_update_all_sectors( );
    /* Get fingers to sit up and take notice. */
#else
  HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                 sect->aset.asp_idx, 
                 sect->pn_offset );
    /* Write PN offset */

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0,
                        sect->aset.asp_idx, 
                        sect->aset.drc_cover );
    /* Write the DRC walsh cover for the new ASP */

  HDRASP_SET_CELL_MAP( HDRMDSP_DEMOD_CARRIER_0,
                       sect->aset.asp_idx, 
                       sect->aset.rpc_cell_idx );
    /* Write the RPC Cell Map for the new ASP */

#ifdef FEATURE_HDR_REVA_L1
  
  HDRASP_SET_DSC( HDRMDSP_DEMOD_CARRIER_0,
                  sect->aset.asp_idx, 
                  sect->aset.dsc );
    /* Write the RPC Cell Map for the new ASP */

  HDRASP_SET_RA_CHANNEL_GAIN( HDRMDSP_DEMOD_CARRIER_0,
                              sect->aset.asp_idx, 
                              sect->aset.ra_channel_gain_lin );
    /* write the linear value of RAChannelgain for the new ASP */

#endif /* FEATURE_HDR_REVA_L1 */
  
  HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0,
                      sect->aset.asp_idx, 
                      sect->link_id );
    /* Write the Link ID as well */
#endif /* FEATURE_HDR_REVB */

  hdrsrchmsg_basp_ho_ovrd( HDRMDSP_DEMOD_CARRIER_0, sect->aset.asp_idx ); 
    /* after the idle handoff, set the new pilot's ASP as the best
       ASP so that data can be demodulated from it */

  hdrsrchsleep_set_scc_info( sect->scc_offset, sect->scc_rate );
    /* Update SCC offset and rate */

  hdrsrchsleep_send_msg_update_scc_offset( );
    /* Send the message to update SCC offset */

  hstr = HDRHSTR_GET_COUNT( );
    /* Read HSTR count    */

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, "IHO: OldASP=%d NewASP=%d HSTR=%d (%d)", 
                  old_sect->aset.asp_idx,
                  sect->aset.asp_idx,
                  hstr % 512,
                  hstr );

  if ( old_sect != sect )
  {
    hdrsrchset_drop_from_aset( old_sect );
      /* drop the old sector from the aset */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "IHO to the same sector");
  }

  (void) hdrsrchfing_program_fings();
    /* Deassign the fingers */

  hdrsrchmsg_send_msg( &hdrsrchmsg.asp_update_msg.hdr,
                       sizeof( hdrfw_asp_update_msg_t ) );
    /* Send ASP update message */

  /*----------------------------*
   * Log pkt for change in ASET *
   *----------------------------*/
  hdrsrchlog_aset_change_log();

#ifdef FEATURE_HDR_TO_LTE
  hdrsrchltemeas_reset_ltemeas_state();
    /* Reset LTE measurement state */
#endif /* FEATURE_HDR_TO_LTE */

} /* hdrsrchutil_aset_idle_handoff( ) */

#ifdef FEATURE_HDR_BCMCS

/* <EJECT> */
/*=============================================================================
FUNCTION:     HDRSRCHUTIL_PROMOTE_TO_BC_ASET

DESCRIPTION:
  Adds the sector to ASET & initializes the fields of the structure to the 
  default values for IDLE state. The init values for these fields are based on
  what is being done in hdrsrchutil_aset_idle_handoff() function for the new 
  pilot.
   
RETURN VALUE:
  None
   
SIDE EFFECTS:
  None
  
=============================================================================*/

void hdrsrchutil_promote_to_bc_aset
(
   hdrsrch_sect_struct_type  *sect
  /* New active set sector */
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  hdrsrchset_add_to_aset( HDRSRCH_IDLE_SUBASET_IDX, sect );
    /* ... move the sector to the active set */
#else
  hdrsrchset_add_to_aset( sect );
    /* ... move the sector to the active set */
#endif /* FEATURE_HDR_REVB */

  sect->drop_timer.expired  = FALSE;
  sect->drop_timer.active   = FALSE;
    /* This is a new pilot - start with non expired drop timer */

  sect->aset.drc_cover      = 0;    
    /* No DRC cover, currently */

#ifdef FEATURE_HDR_REVA_L1

  sect->aset.ra_channel_gain_lin = HDRSRCH_RA_CHANNEL_GAIN_LIN_DEFAULT;
    /* Default RA channel gain */

  sect->aset.dsc            = 0;
    /* No DSC - currently */

#endif /* FEATURE_HDR_REVA_L1 */

  sect->aset.mac_idx        = HDRSRCH_INVALID_TRAFFIC_MAC_INDEX;
    /* Set the mac index to be invalid since we aren't in traffic. */

#ifdef FEATURE_HDR_REVB
  sect->aset.rpc_cell_idx   = hdrsrchset_get_new_rpc_idx( HDRSRCH_IDLE_SUBASET_IDX );
#else
  sect->aset.rpc_cell_idx   = hdrsrchset_get_new_rpc_idx();
#endif /* FEATURE_HDR_REVB */
   
  HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                 sect->aset.asp_idx, 
                 sect->pn_offset );
    /* Write PN offset */

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0, 
                        sect->aset.asp_idx, 
                        sect->aset.drc_cover );
    /* Write the DRC walsh cover for the new ASP */

#ifdef FEATURE_HDR_REVA_L1
  
  HDRASP_SET_RA_CHANNEL_GAIN( HDRMDSP_DEMOD_CARRIER_0,
                              sect->aset.asp_idx, 
                              sect->aset.ra_channel_gain_lin );
    /* write the linear value of RAChannelgain for the new ASP */
  
  HDRASP_SET_DSC( HDRMDSP_DEMOD_CARRIER_0,
                  sect->aset.asp_idx, 
                  sect->aset.dsc );
    /* Write the RPC Cell Map for the new ASP */

#endif /* FEATURE_HDR_REVA_L1 */

  HDRASP_SET_CELL_MAP( HDRMDSP_DEMOD_CARRIER_0,
                       sect->aset.asp_idx, 
                       sect->aset.rpc_cell_idx );
    /* Write the RPC Cell Map for the new ASP */

  HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0,
                      sect->aset.asp_idx, 
                      sect->link_id );
    /* Write the Link ID as well */

  hdrsrchmsg_set_asp_update_mask( HDRSRCH_IDLE_SUBASET_IDX, sect->aset.asp_idx );
    /* Update the carrier and ASP mask */

} /* hdrsrchutil_promote_to_bc_aset  */
#endif /* FEATURE_HDR_BCMCS */


#ifdef FEATURE_HDR_REVB
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSET_INTEGRATE_ASET

DESCRIPTION
  The function works on the hdrsrchset.subaset to remove empty subaset.

DEPENDENCIES
  Connected State

PARAMETERS

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_integrate_aset( void )
{
  int8 i, j;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=hdrsrchset.subaset_cnt-1; i>=0; i-- )
  {
    if( hdrsrchset.subaset[i].aset_cnt == 0 )
    {
      hdrsrchset.subaset[i] = hdrsrchset.subaset[hdrsrchset.subaset_cnt - 1];

      /* Update the subaset_idx to all the sectors */ 

      for( j=0; (j<hdrsrchset.subaset[i].aset_cnt) && (j < (HDRSRCH_MAX_SUBASET_SIZE+1)); j++ )
      {
        hdrsrchset.subaset[i].aset[j]->aset.subaset_idx  = i;
      } 

      memset( &hdrsrchset.subaset[hdrsrchset.subaset_cnt-1],
              0, sizeof( hdrsrch_subaset_struct_type ) );
        /* Reset the content of this subaset */

      hdrsrchset.subaset_cnt --;
        /* Decrease the subaset count by 1 */

      if ( ( hdrsrchset.subaset_cnt == 0 ) ||
           ( hdrsrchset.subaset_reportable_idx == i ) )
      {
        hdrsrchset.subaset_reportable_idx = -1;
          /* Nothing is reportable if no subaset exists or
             if reportable subaset was dropped. This should
             be reselected by hdrsrchutil_select_reportable_pilots() */
      }
      else if ( hdrsrchset.subaset_reportable_idx ==
                ( hdrsrchset.subaset_cnt - 1 ) )
      {
        hdrsrchset.subaset_reportable_idx = i;
          /* CC carrier subaset index is being moved. */
      }
    } /* if */
  } /* for */
} /* void hdrsrchutil_integrate_aset( void ) */
#endif /* FEATURE_HDR_REVB */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_GET_DEMOD_CARRIER_MASK

DESCRIPTION
  This function returns the demod carrier mask that indicates which demod 
  carrier is enabled.

DEPENDENCIES
  Connected State

PARAMETERS
  None

RETURN VALUE
  mask

SIDE EFFECTS
  None

===========================================================================*/

uint8 hdrsrchutil_get_demod_carrier_mask( void )
{

  uint8                      sa;
    /* Subaset index */

  uint8                      mask = 0;
    /* Mask for demod carrier */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
  {
    mask |= 1 << hdrsrchset.subaset[ sa ].demod_idx;
  }

  return mask;
} /* hdrsrchutil_get_demod_carrier_mask */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ASET_ENTER_IDLE

DESCRIPTION
  Drops pilots from the ASET until only the best pilot remains.

  For revB TC to Idle transition,
  - Camp on the strongest pilot assigned to demod carrier 0. 

DEPENDENCIES
  Traffic State

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Because there is no immediate tuning in the TC to idle transition, there
  might be a case the the subaset[0] uses a demod resource other then demod0.
===========================================================================*/

void hdrsrchutil_aset_enter_idle( void )
{
  hdrsrch_sect_struct_type  * best_sect;
    /* Best Sector */

  hdrsrch_sect_struct_type  * sect;
    /* Temporary */

  int8                        i;
    /* Loop index */

#ifdef FEATURE_HDR_REVB
  int8                        j;
    /* Loop index */

  uint8                       demod_carrier_mask;
    /* Demod carrier mask */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  /* For REVB, drop the non cc monitoring subasets first */
  /* Find the strongest ASET sector */

  hdrsrchmsg_init_asp_update_msg();
    /* Init the ASP update message */

  (void) hdrsrchset_grice_drop_gcset();
    /* Clear gRICE CSET */

  for( i=hdrsrchset.subaset_cnt-1; i>=0; i-- )
  {
    /* Traverse entire subasets; if it is not a cc monitoring subaset,
       then drop all the pilots */
    if ( hdrsrchset.subaset[i].cc_carrier == FALSE )
    {
      for( j=hdrsrchset.subaset[i].aset_cnt-1; j>=0; j-- )
      {
        sect = hdrsrchset.subaset[i].aset[j];
          /* Convenience pointer */

        hdrsrchset_drop_from_aset( sect );
          /* Drop the sector from ASET; it may add into either CSET, 
             NSET or RSET based the group policy, non reportable policy */
      }
    } /* if */
  } /* for i */

  hdrsrchutil_integrate_aset();
    /* Integrate the subaset list. After entering the idle state, only one
       subaset is in allowed in ASET. subaset_cnt=1 and subaset_idx=0 */

  ASSERT( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].cc_carrier );
    /* It should be a CC carrier */

  /* Find the sector with the max energy */

  best_sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0];

  for( i = 1;
       ( ( i < hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt ) &&
         ( i < HDRSRCH_MAX_SUBASET_SIZE ) );
       i++ )
  {
    if ( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[i]->total_eng > 
         best_sect->total_eng )
    {
      best_sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[i];
    }
  }

  /* If BCMCS is enabled, keep the subaset on cc carrier. If BCMCS is 
     disabled, keep the best sect on cc carrier and drop the rest of 
     the sectors */

#ifdef FEATURE_HDR_BCMCS /*BCMCS will be anyway disabled when FEATURE_HDR_BCMCS is off*/
  if ( hdrsrchbc_get_bc_status( ) == BCMCS_DISABLED )
#endif /* FEATURE_HDR_BCMCS */
  {
    for( i=hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt-1; i>=0; i-- )
    {
      sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[i];

      if ( sect != best_sect )
      {
        hdrsrchset_drop_from_aset( sect );
          /* Drop the pilot from the ASET */
      }
    }
  }

  hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].tca_idx = 0;
    /* Reset tca_idx */
  
  /* Update the aset channel/demod info. Note this might not be the 
     current RF configuration. It only reflects current ASET information. */

  hdrsrch.aset_channel_cnt = 1;
  hdrsrch.aset_channel[0]= hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel;
  hdrsrch.aset_demod[0] = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
#else
  best_sect = hdrsrchset.aset[0];

  for( i=1; i<hdrsrchset.aset_cnt; i++ )
  {
    if ( hdrsrchset.aset[i]->total_eng > best_sect->total_eng )
    {
      best_sect = hdrsrchset.aset[i];
    }
  }

  /* Drop all other sectors */

  for( i=hdrsrchset.aset_cnt-1; i>=0; i-- )
  {
    sect = hdrsrchset.aset[i];

    if ( sect != best_sect )
    {
      hdrsrchset_drop_from_aset( sect );
        /* Drop the pilot from the ASET */
    }
  }
#endif /* FEATURE_HDR_REVB */

  best_sect->drop_timer.active   = FALSE;
   /* Stop drop timer */
  
  /*best_sect->drop_timer.expired  = FALSE;*/
    /* Do not initialize the drop timer expired flag since the pilot is staying
       in the active set */

  best_sect->aset.drc_cover      = 0;
    /* No DRC cover any more. */

#ifdef FEATURE_HDR_REVA_L1

  best_sect->aset.ra_channel_gain_lin = HDRSRCH_RA_CHANNEL_GAIN_LIN_DEFAULT;
    /* Default RA channel gain */

  best_sect->aset.dsc            = 0;
    /* No DSC any more. */

#endif /* FEATURE_HDR_REVA_L1 */

  best_sect->aset.mac_idx        = HDRSRCH_INVALID_TRAFFIC_MAC_INDEX;
    /* Set the mac index to be invalid since we aren't in traffic. */

#ifdef FEATURE_HDR_REVB
  best_sect->aset.rpc_cell_idx   = 
                         hdrsrchset_get_new_rpc_idx( HDRSRCH_IDLE_SUBASET_IDX );
    /* Set Reverse Power Control (RPC) cell index */
#else
  best_sect->aset.rpc_cell_idx   = hdrsrchset_get_new_rpc_idx();
    /* Set Reverse Power Control (RPC) cell index */
#endif /* FEATURE_HDR_REVB */

#ifndef FEATURE_HDR_REVB
  HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                 best_sect->aset.asp_idx, 
                 best_sect->pn_offset );
    /* Write PN offset */

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0,
                        best_sect->aset.asp_idx, 
                        best_sect->aset.drc_cover );
    /* Write the DRC walsh cover for the new ASP */

#ifdef FEATURE_HDR_REVA_L1
  
  HDRASP_SET_RA_CHANNEL_GAIN( HDRMDSP_DEMOD_CARRIER_0,
                              best_sect->aset.asp_idx, 
                              best_sect->aset.ra_channel_gain_lin );
    /* write the linear value of RAChannelgain for the new ASP */
  
  HDRASP_SET_DSC( HDRMDSP_DEMOD_CARRIER_0,
                  best_sect->aset.asp_idx, 
                  best_sect->aset.dsc );
    /* Write the RPC Cell Map for the new ASP */

#endif /* FEATURE_HDR_REVA_L1 */

  HDRASP_SET_CELL_MAP( HDRMDSP_DEMOD_CARRIER_0,
                       best_sect->aset.asp_idx, 
                       best_sect->aset.rpc_cell_idx);
    /* Write the RPC Cell Map for the new ASP */

  HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0,
                      best_sect->aset.asp_idx, 
                      best_sect->link_id );
    /* Write the Link ID as well */
#else
  demod_carrier_mask = hdrsrchutil_aset_commit_to_dsp();
    /* Update the aset change to the firmware */


  hdrsrchmsg_send_multi_asp_update_msg( TRUE );
    /* Send ASP update message */

  hdrsrchfing_set_demod_carrier_mask( demod_carrier_mask );
    /* Update demo carrier number to finger driver */
#endif /* FEATURE_DHR_REVB */

  hdrsrchfing_update_all_sectors( );
    /* Get fingers to sit up and take notice. */

  (void) hdrsrchfing_program_fings();
    /* Program the deassigned the fingers. */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  /* Log carrier 0 only when Idle. So, when AT transitions out of 
     connected state, update the carrier valid logging flag */

  if ( ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_OFS_TC_STATE ) )
  {

    for ( demod_idx = 1; 
          demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx )
    {
      HDRLOG_CLEAR_DEMOD_CARRIER_VALID( demod_idx );
        /* Clear valid bits for all carriers other than demod 0 in Idle */
    }

  }
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

  hdrsrchfing_limit_max_fings( FALSE );
    /* Do notlimit max fingers */

  hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
    /* Use the NV setting for diversity */

} /* hdrsrchutil_aset_enter_idle( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ROLL

DESCRIPTION
  This is the HDRSRCH PN ROLL Callback function.
  It translates the PN ROLL callback into the ROLL signal for the task.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchutil_roll ( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_ROLL_SIG );
   /* set the Signal to the HDRSRCH task */

} /* hdrsrchutil_roll */

/*===========================================================================

FUNCTION HDRSRCHUTIL_ENABLE_ROLL

DESCRIPTION
  This function installs and enables the PN ROLL signal

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_enable_roll ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrts_register_roll_callback( hdrsrchutil_roll );
    /* Add the callback to the callback list */

  hdrsrch_add_to_wait_mask( HDRSRCH_ROLL_SIG );
    /* Add the ROLL signal to wait mask */

} /* hdrsrchutil_enable_roll */


/*===========================================================================

FUNCTION HDRSRCHUTIL_DISABLE_ROLL

DESCRIPTION
  This function un-installs and disables the PN ROLL signal

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_disable_roll ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrts_deregister_roll_callback( hdrsrchutil_roll );
    /* Remove the callback to the callback list */

  hdrsrch_remove_from_wait_mask( HDRSRCH_ROLL_SIG );
    /* Remove the ROLL signal from the wait mask */

} /* hdrsrchutil_disable_roll */


/*===========================================================================

FUNCTION HDRSRCHUTIL_HANDLE_PN_ROLL

DESCRIPTION
  This function performs the default PN roll processing.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_handle_pn_roll ( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_BCMCS
  if (( hdrsrchbc_get_bc_status() == BCMCS_ENABLED) &&
      ((hdrsrch_state_id == HDRSRCH_REACQ_STATE) ||
       (hdrsrch_state_id == HDRSRCH_BC_IDLE_STATE) ||
       (hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE) ||
       (hdrsrch_state_id == HDRSRCH_CONNECTED_STATE)))
  { 
    hdrbcmac_run_scheduler();  
      /* Run scheduling for next frame if broadcast active and searcher is
         in a valid state.  REACQ is included because the BC scheduler
         must run early (during reacq) to schedule Rx for BCC packets that 
         are to be received immediately after wakeup. */
  }
#endif /* FEATURE_HDR_BCMCS */

  if ( ( hdrsrchrfdiv_diversity_is_enabled() == TRUE  ) &&
       ( hdrsrchafc_get_tap_status() == TRUE )   )
  {
    hdrsrchrfdiv_consider_div_switch();
      /* Consider switching the diversity settings on signal 
         conditions. */
  }

  hdrsrchlog_pn_roll_logging();
    /* do the PN roll logging */

} /* hdrsrchutil_handle_pn_roll */


/* <EJECT> */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_UPDATE_SYSTEM_LOSS_INFO

DESCRIPTION
  This function sets the system loss related parameters so that the 
  corresponding coset group is searched first at next ACQ command. 
  
DEPENDENCIES
  None

INPUTS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchutil_update_system_loss_info( void )
{
  hdrsrchutil_system_loss_struct_type   *sys_loss;
    /* Pointer to the parameters of system loss */

  hdrsrch_sect_struct_type              *sect;
    /* The best sector before system loss */

  hdrsrch_demod_idx_type                demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  sys_loss                   = & hdrsrchutil.system_loss;

  if ( hdrmdsp_check_dsp_clk() )
  {
#ifdef FEATURE_HDR_REVB
    sect                     = hdrsrchset_get_asp_sect_ptr(
                               HDRSRCH_IDLE_SUBASET_IDX,
                               HDRASP_GET_BASP_FL( demod_idx ) );
#else
    sect = hdrsrchset_get_asp_sect_ptr( HDRASP_GET_BASP_FL( ) );
#endif /* FEATURE_HDR_REVB */
  }
  else /* if hdrmdsp_check_dsp_clk() */
  {
#ifdef FEATURE_HDR_REVB
    /* SRCH is in sleep state or deactivated */
    sect                     = hdrsrchset.subaset[0].aset[0];
#else
    /* SRCH is in sleep state or deactivated */
    sect                     = hdrsrchset.aset[0];
#endif /* FEATURE_HDR_REVB */

  } /* if hdrmdsp_check_dsp_clk() */

  if ( sect == NULL )
  {
    sys_loss->pn_offset      = HDR_NO_PN_OFFSET;
    sys_loss->channel.band   = SYS_BAND_CLASS_NONE;
  }
  else
  {
    sys_loss->pn_offset      = sect->pn_offset;
    sys_loss->channel        = sect->chan;
  } /* if sect == NULL */

  if ( sys_loss->pn_offset == HDR_NO_PN_OFFSET )
  {
    sys_loss->coset_grp      = 0;
      /* Search coset group 0 first */
  }
  else
  {
    sys_loss->coset_grp      = ( sys_loss->pn_offset % 16 ) % 4;
      /* Convert pn offset to coset group */
  }

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                  "System lost: band: %d chan: %d coset grp: %d pn: %d",
                  sys_loss->channel.band, sys_loss->channel.chan_num, 
                  sys_loss->coset_grp , sys_loss->pn_offset);

}  /* hdrsrchutil_update_system_loss_info() */

/*==========================================================================

FUNCTION        HDRSRCHUTIL_RESET_SYSTEM_LOSS_INFO

DESCRIPTION
  This function initializes the parameters related system loss.
  
DEPENDENCIES
  None

INPUTS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchutil_reset_system_loss_info( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil.system_loss.pn_offset      = HDR_NO_PN_OFFSET;
  hdrsrchutil.system_loss.channel.band   = SYS_BAND_CLASS_NONE;
  hdrsrchutil.system_loss.coset_grp      = 0;

}  /* hdrsrchutil_reset_system_loss_info */

/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_SYSTEM_LOSS_COSET_GROUP

DESCRIPTION
  This function returns the coset group being used before system loss.
  
DEPENDENCIES
  None

INPUTS
  None
  
RETURN VALUE
  The coset group when system loss occured.

SIDE EFFECTS
  None

==========================================================================*/

uint8 hdrsrchutil_get_system_loss_coset_group( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  return hdrsrchutil.system_loss.coset_grp;

}  /* hdrsrchutil_get_system_loss_coset_group */

/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_SYSTEM_LOSS_CHAN

DESCRIPTION
  This function returns the channel being used before system loss.
  
DEPENDENCIES
  None

INPUTS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  The channel when system loss occured.

==========================================================================*/

sys_channel_type *hdrsrchutil_get_system_loss_chan( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return & (hdrsrchutil.system_loss.channel);

}  /* hdrsrchutil_get_system_loss_chan */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_SYSTEM_LOST

DESCRIPTION  : This function declares system lost, and transfers back to
               the inactive state

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Exits to inactive.

=========================================================================*/

void hdrsrchutil_system_lost
(
  hdrsrch_system_lost_enum_type   reason
    /* Reason for declaring system lost */
)
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_FATAL, "System Lost: reason=%d", reason );

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_OOS);
#endif
  /* Here Set the flag to indicate system lost is in progress. This is also set
     in API hdrsrch_system_lost, which ultimately calls current API. But in
     majority of cases, current API is used directly to report system lost.
     Hence setting it here again.
     This flag is reset at the end of SRCH state powerdown flow */
  hdrsrch.sysloss_pending = TRUE;

#ifdef FEATURE_FACTORY_TESTMODE
  if ( ftm_get_mode() != FTM_MODE )
  {
    hdrsrch_stop_timer( &hdrsrch.schedule_timer );
      /* Stop the schedule timer since we are declaring system loss */
  }
#else /* FEATURE_FACTORY_TESTMODE */
  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop the schedule timer since we are declaring system loss */
#endif /* FEATURE_FACTORY_TESTMODE */

  if ( hdrsrch.system_lost )
  {
    hdrsrch.system_lost( reason );
      /* Declare system lost to the protocol layer */
  }
    
  hdrsrchutil_update_system_loss_info();
    /* Store information related to system loss */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  /* Log carrier 0 only when Idle. So, when AT transitions out of 
     connected state, update the carrier valid logging flag */

  if ( ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_OFS_TC_STATE ) )
  {

    for ( demod_idx = 1; 
          demod_idx < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; ++demod_idx )
    {
      HDRLOG_CLEAR_DEMOD_CARRIER_VALID( demod_idx );
        /* Clear valid bits for all carriers other than demod 0 in Idle */
    }

  }
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */
} /* hdrsrchutil_system_lost() */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_HANDLE_LOST_DUMP

DESCRIPTION  : This function handles the lost dump interrupt for MSM6500.
               For now it just declares system loss.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : Declares System Lost.

=========================================================================*/

void hdrsrchutil_handle_lost_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ERR("Lost Dump. Declare System Lost! St=%x", hdrsrch_state_id, 0, 0 );

  hdrsrchdrv.active_srch = SRCH_STATUS_INACTIVE;

  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
   /* Declare system lost.  Protocols will think of it as faded channel and
      will force search to de-activate state */

} /* hdrsrchutil_handle_lost_dump */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_CHECK_MDSP_HALT

DESCRIPTION  : This function checks if the DSP has halted

DEPENDENCIES : None

SIDE EFFECTS : None

=========================================================================*/

boolean hdrsrchutil_check_mdsp_halt( void )
{

  boolean      val = FALSE;
    /* return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return val;

}/* hdrsrchutil_check_mdsp_halt */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_HANDLE_MDSP_HALT

DESCRIPTION  : This function handles the case when the modem DSP halts.
               There is nothing search can do now except re-starting the
               DSP and declare system loss to the protocols.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : Declares System Lost.

=========================================================================*/

void hdrsrchutil_handle_mdsp_halt( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_ERR_FATAL_ON_MDSP_HALT
  ERR_FATAL("DSP Halted! Halt Code:%d", 0, 0, 0 );
#endif /* FEATURE_ERR_FATAL_ON_MDSP_HALT */

} /* hdrsrchutil_handle_mdsp_halt */


/* <EJECT> */
/*=========================================================================

FUNCTION        HDRSRCHUTIL_HSDELTA

DESCRIPTION
  This function calculates the delta between two half slot inputs.  The
  output is limited to 0-511 (426ms cycle) although the input is not.
 
  Assumption/Limitation:
  If one half slot time is within one frame of before wrap-around and one half
  slot time is within one frame after wrap-around, then treat delta as if
  wrap-around just occurred.

DEPENDENCIES
  None

INPUT
  Sync Messages

RETURN VALUE
  None

SIDE EFFECTS
  May adjust system time

=========================================================================*/

unsigned long hdrsrchutil_hsdelta
(
   unsigned long hs1,
   unsigned long hs2
)
{
  unsigned long delta;

  /* Modify input to within 426ms cycle. */
  hs1 &= 511;
  hs2 &= 511;

  /* Check for wrap-around conditions.  The assumption is that the two times
     are relatively close so only look for wrap-around when both inputs are
     within a frame of wrap-around. */
  if ( ( hs1 > (HDRSRCHUTIL_HALFSLOTS_PER_CYCLE - HDRSRCHUTIL_HALFSLOTS_PER_FRAME) ) &&
       ( hs2 < HDRSRCHUTIL_HALFSLOTS_PER_FRAME ) )
  {
    /* hs2 wrap-around case */
    delta = hs2 + HDRSRCHUTIL_HALFSLOTS_PER_CYCLE - hs1;
  }
  else if  ( ( hs2 > (HDRSRCHUTIL_HALFSLOTS_PER_CYCLE - HDRSRCHUTIL_HALFSLOTS_PER_FRAME) ) &&
             ( hs1 < HDRSRCHUTIL_HALFSLOTS_PER_FRAME ) )
  {
    /* hs1 wrap-around case */
    delta = hs1 + HDRSRCHUTIL_HALFSLOTS_PER_CYCLE - hs2;
  }
  else
  {
    if (hs1 > hs2)
    {
      delta = hs1 - hs2;
    }
    else
    {
      delta = hs2 - hs1;
    }
  }

  return delta & 0x1FF;
    /* Limit return to half slots in 426ms cycle */
} /* hdrsrchutil_hsdelta */


/* <EJECT> */
#ifdef FEATURE_HDR_SYNC_CHECK_AND_RESYNC
#error code not present
#endif /* FEATURE_HDR_SYNC_CHECK_AND_RESYNC */

/*=========================================================================

FUNCTION        HDRSRCHUTIL_SYNC_CHECK

DESCRIPTION
  This function validates time two different ways:
  1. Validate system time against the sync message, SYNC time must match
     the same system time in 426ms cycle units, with rollover handled in
     the event the SYNC is decoded near the end of previous 426ms cycle.
  2. System time slots must be within one frame (16 slots) of MDSP's HSTR.

DEPENDENCIES
  None

INPUT
  Sync Messages

RETURN VALUE
  None

SIDE EFFECTS
  May adjust system time

=========================================================================*/

errno_enum_type hdrsrchutil_sync_check
(
  const hdrsrch_synchronize_params_type *sync
    /* Synchronization message */
)
{
  qword                                 sync_frame;
    /* 26.7ms frames since the beginning of time, from the sync message */

  qword                                 cycle;
    /* 426.7ms cycles since the beginning of time */

  uint32                                slot;
    /* Slots after current 426.7ms cycle begin */

  qword                                 frame;
    /* 26.7ms frames since the beginning of time */

  uint32                                hstr;
    /* Half-Slot Timing Reference */

  uint32                                hs_error;
    /* Difference between HSTR and system time half slot */

  qword                                 delta_frames;
    /* Delta between System time frames and SYNC msg frames */

#ifdef FEATURE_HDR_SYNC_CHECK_AND_RESYNC
  #error code not present
#endif /* FEATURE_HDR_SYNC_CHECK_AND_RESYNC */

  errno_enum_type                       status = E_SUCCESS;
    /* Assume the best */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check whether the delay in processing of sync cmd is greater than SCC cycle */
  if( hdrsrchutil_stop_timer_ms( sync->sync_cmd_delay ) 
    > SLOTS_TO_MS ( HDRTS_SLOTS_PER_SCC ) )
  {

    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Delayed synchronize cmd ignored" );

    /* return failure for sync cmd */
    return E_FAILURE;
  }

  REX_DISABLE_PREMPTION();

  hdrts_get_cycle_and_slot( cycle, &slot, HDRTS_SLOTS_PER_SCC );
    /* Get # of SCC's and slots after scc */

  hstr = HDRHSTR_GET_COUNT() & 0x1FF;
    /* Get MDSP HDTS count mod 512 */

  REX_ENABLE_PREMPTION();

  qw_equ(frame, cycle);
  qw_shift(frame, 4);
    /* Convert SCC cycles into frames, i.e., multiply by 16 */

  qw_inc(frame, 6);
    /* Value, in frames 160ms after the SCC */

  qw_set(sync_frame, sync->sync_time>>32, sync->sync_time&0xFFFFFFFF);
    /* Capture sync message's frame time's uint64 type into a qword type */

  /* Compute difference between "slots after start of last SCC" and HSTR */

  hs_error = hdrsrchutil_hsdelta(slot * 2, hstr);
    /* Find difference between MDSP and system half slot timeing. Handle any wrap around conditions. */

  qw_sub( delta_frames, frame, sync_frame );
    /* delta_frames contains the difference between system time frame and sync msg frame */

  /* System lost if:
     1. System time frames and sync time frames are different
       1a. Exception of close proximity to next cycle, i.e., within 48 slots,
           if sync msg comes in 8th CC packet in intra-SCC, due to worst case
           decoding (31 slots after next CC cycle) and system
           delays, HDRSRCH could process this in next cycle, so frames can
           be off by exactly 16 frames (one 426ms cycle) in this case.
     2. Delta between System time half slot time and MDSP HSTR is > one frame
   */
  if ( ( qw_hi(delta_frames) !=0 ) || ( qw_lo(delta_frames) != 0 ) || 
       ( hs_error >= HDRSRCHUTIL_HALFSLOTS_PER_FRAME ) )
  {
    if ( ( qw_hi(delta_frames) ==0 ) && ( qw_lo(delta_frames) == 16 ) && 
         ( hs_error < HDRSRCHUTIL_HALFSLOTS_PER_FRAME &&
           slot < HDRSRCHUTIL_CYCLE_WRAPAROUND_SLOTS ) )
  {
      /* 426ms cycle wrap-around occurred, i.e., SYNC msg processing decoding
         took a long time and now it is the next CC cycle.  This is not a
         sync check error. */
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                      "SYNC CHECK CYCLE WRAPAROUND: Sync %ld (frames), SCC time %d (HS), Current System Time %ld (frames)", 
                      (uint32) sync->sync_time, 
                      sync->half_slot,
                      qw_lo(frame) );
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                      "SYNC CHECK CYCLE WRAPAROUND: System HS %d, MDSP HSTR %d", 
                      slot*2, 
                      hstr, 
                      hdrsrch_state_id );
    }
    else
    {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_SYNC_TIMING_ERROR);
#endif          
      HDR_MSG_SRCH_3( MSG_LEGACY_ERROR,
                      "SYNC CHECK HALF SLOT ERROR: Sync %ld (frames), SCC time %d (HS), Current System Time %ld (frames)", 
                      (uint32) sync->sync_time, 
                      sync->half_slot,
                      qw_lo(frame) );
      HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
                      "SYNC CHECK HALF SLOT ERROR: System HS %d, MDSP HSTR %d", 
                      slot*2, 
                      hstr );

#ifdef FEATURE_HDR_SYNC_CHECK_AND_RESYNC
#error code not present
#else
#ifdef FEATURE_HDR_SYNC_CHECK_AND_SYSLOST

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_SYNC_MSG_TIMEOUT );
        /* Not exactly a sync-msg-timeout, but a Sync problem anyway */

      status = E_FAILURE;

#endif /* FEATURE_HDR_SYNC_CHECK_AND_SYSLOST */
#endif /* FEATURE_HDR_SYNC_CHECK_AND_RESYNC */
    }
  }
  
  return status;

} /* hdrsrchutil_sync_check( ) */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_INIT

DESCRIPTION  : This function initializes HDRSRCH utilities related stuff

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchutil_init( void )
{
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchutil.ofs_trigger_x26ms = 0;
    /* Initialize the PN ROLL count to 0 */

  hdrsrchutil_reset_system_loss_info();
    /* Initialize system loss related parameters */

  hdrsrchutil.cached_sects.num_sects       = 0;
    /* No cached sector */

} /* hdrsrchutil_init */


/* EJECT */
/*==========================================================================
FUNCTION        HDRSRCHUTIL_START_STOP_WATCH

DESCRIPTION
  Starts stop watch timing. This is implemented using time stamps
  for better accuracy. Call hdrsrchutil_read_stop_watch() to get
  the difference between start and stop timings.

DEPENDENCIES
  None.

INPUTS
  None

RETURN VALUE

SIDE EFFECTS
  none

==========================================================================*/

void hdrsrchutil_start_stop_watch( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_get_ms ( hdrsrchutil.stop_watch.start_ms );
    /* start timing the events for debug purposes */

}/* hdrsrchacq_get_acq_duration */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_READ_STOP_WATCH

DESCRIPTION
  Returns the time between start and end events on the stop watch

DEPENDENCIES
  hdrsrchutil_start_stop_watch() should have been called before

INPUTS
  None

RETURN VALUE
 dword : Acquisition duration in msec

SIDE EFFECTS
  none

==========================================================================*/

dword hdrsrchutil_read_stop_watch( void )
{
  qword       delta_ms;
    /* difference between two stop watch events */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_get_ms ( hdrsrchutil.stop_watch.end_ms );
    /* time the acquisition process for debug purposes */

  /* take difference between start and end time */
  qw_sub ( delta_ms,
           hdrsrchutil.stop_watch.end_ms,
           hdrsrchutil.stop_watch.start_ms );

  return  qw_lo ( delta_ms );
    /* Compute and return acquisition duration in milliseconds */

}/* hdrsrchacq_get_acq_duration */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_HDR_RSSI

DESCRIPTION
  Returns HDR signal estimate to the caller.
    
DEPENDENCIES
  
INPUTS
  None

RETURN VALUE
 byte : Quality of HDR signal

SIDE EFFECTS
  none

==========================================================================*/
uint8 hdrsrchutil_get_hdr_rssi( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrch.rssi;
} /* hdrsrchutil_get_hdr_rssi */


/*==========================================================================

FUNCTION        HDRSRCHUTIL_UPDATE_RSSI

DESCRIPTION
  Updates HDR signal estimate to the caller. It returns the carrier power
  calculated from the Io estimate made by rx AGC loops in the DSP and the
  Ec/Io estimate found by the searcher.
  
    Ec = Io x Ec/Io in linear scale

    or
     
    Ec_dB = Io_dbm + EcIo_dB

  If Ec/Io is too small to be reliable then the Ec is saturated at minimum
  value of -118dB (obtained from -125dB in 1x by -125+7dB=-118dB)

  For revB TBD, the function can return the Ec for each carriers or the worst
  carrier or the best carrier. The may NOT be needed to do for every carrier.
    
DEPENDENCIES
  The last hdr_rssi is save in its corresponding subasets structure; 

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchutil_update_rssi( void )
{
  int                i;
    /* loop variables/temps */

  int                ecio = 0;
    /* ecio of the strongest hdr pilot */

  int                rx0_dbm, rx1_dbm, power = 0; 
    /* received power read from the AGCs */

  boolean            diversity;
    /* diversity settings */
  
  uint16             last_hdr_rssi;
    /* last signal estimatation needed for IIR filtering */

  uint16             hdr_rssi = 0;
    /* signal estimate */

  int8               subaset_idx;
    /* Ref subaset index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  subaset_idx = hdrsrchset_get_ref_subaset_idx();

  /* TBD */
  if ( ( subaset_idx == -1 ) ||
       ( subaset_idx >= HDRSRCH_MAX_NUM_SUBASETS ) ) 
  {
    return;
  }

  last_hdr_rssi = hdrsrchset.subaset[subaset_idx].hdr_rssi;
    /* retrieve the value of hdr_rssi for filtering */

  /*-------------------------*
   * Get EcIo in 0.5dB units *
   *-------------------------*/

  if ( hdrsrchset.subaset[subaset_idx].aset[0] != NULL )
  {
    ecio = hdrsrchset.subaset[subaset_idx].aset[0]->total_eng;
      /* initialize max ec/io with the aset 0 eng before comparing all pilots */

    /* Find the maximum ecio among all active set sectors in this subaset */
    for ( i = 1; 
          ( ( i < hdrsrchset.subaset[subaset_idx].aset_cnt ) &&
            ( i < HDRSRCH_MAX_SUBASET_SIZE ) );
          i++ )
    {
      if ( ( hdrsrchset.subaset[subaset_idx].aset[i] != NULL ) && 
           ( ecio < hdrsrchset.subaset[subaset_idx].aset[i]->total_eng ) )
      {
        ecio = hdrsrchset.subaset[subaset_idx].aset[i]->total_eng;
      } 
    }

    ecio = (uint16) ( hdrsrchset_eng_to_ecio( ecio ) / 2 );
      /* convert ecio to -dB units. Note that the number returned is +ve but
         it represents value in -ve dB units  e.g. -5dB is 5 */

    /*--------------*
     * Get Rx Power *
     *--------------*/

    hdrsrchrf_get_rx_power( &rx0_dbm, &rx1_dbm, &diversity );
      /* Get rx power in dB units. We may read the stored value of RX AGC
         here if the DSP clock has been off */

    if ( diversity )
    {
      power = MAX( rx0_dbm, rx1_dbm );
      /* Represents total Io. Take average of two antennas here. Both 
         antennae will report very similar numbers but take MAX. This will 
         cover the case where testing is done with ant0 disconnected with 
         diversity ON. */
    }
    else
    {
      power = rx0_dbm;
        /* else read just ant0 */
    }

    power = -power;
      /* Get the power in +ve units e.g. -75dbm will become 75 */

    /*-------------------------*
     * Compute Signal Estimate *
     *-------------------------*/

    hdr_rssi = (uint16) ( power + ecio );
      /* Get the carrier strength, Ec, by adding power (Io) and Ec/Io in log 
        scale which is equivalent to multiplying the two entities in linear 
         scale: 
           Signal estimate (Ec) = Interference (Io) x Ec/Io.
                    Log[ Ec ] = Log[ Io x Ec/Io]
                    Log[ Ec ] = Log[ Io ] + Log[ Ec / Io ]
                    hdr_rssi   = power + ecio
      */

    if ( hdr_rssi > HDR_RSSI_MIN_VALUE_DBM )
    {
      hdr_rssi = HDR_RSSI_MIN_VALUE_DBM;
        /* Use minimum value of HDR signal estimate when 
           rssi is lesser than -118dBm */
    }
  }  /* if (hdrsrchset.aset[0] != NULL ) */
  else
  {
    hdr_rssi = HDR_RSSI_MIN_VALUE_DBM;
      /* minimum value of HDR signal estimate */
  }

  hdr_rssi *= 16;
    /* upscale for better filtering results */

  /*------------------------*
   * Filter Signal Estimate *
   *------------------------*/

  if ( last_hdr_rssi != 0 )
  {
    hdr_rssi = IIR_FILT_N( last_hdr_rssi, hdr_rssi, HDR_RSSI_FILT_LEN );
      /* filter the signal estimate with an IIR filter */
  }

  /* Store the hdr rssi */
  hdrsrchset.subaset[subaset_idx].hdr_rssi = hdr_rssi;

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR Sig Est=%d p=%d ec=%d", 
                  hdr_rssi/16, power, ecio );

  hdrsrch.rssi = hdr_rssi / 16;

}/* hdrsrchutil_update_rssi */


/*==========================================================================

FUNCTION        HDRSRCHUTIL_UPDATE_MAX_ECIO
  
DESCRIPTION
  Updates HDR max Ec/Io among all aset sectors.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchutil_update_max_ecio( void )
{
  int                i;
    /* loop variables/temps */

  int16              ecio = 0;
    /* ecio of the strongest hdr pilot */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* Find the maximum ecio among all active set sectors in subaset[0].
     TBD: need check if subaset[0] is right selection for the non rec mode.  */
  for ( i = 0; i < hdrsrchset.subaset[0].aset_cnt; i++ )
  {
    if ( ( hdrsrchset.subaset[0].aset[i] != NULL ) &&
         ( ecio < hdrsrchset.subaset[0].aset[i]->total_eng ) )
    {
      ecio = hdrsrchset.subaset[0].aset[i]->total_eng;
    }
  }

  hdrsrch.max_ecio = ecio;
} /* hdrsrchutil_update_max_ecio */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_ECIO_AND_IO

DESCRIPTION
  Returns HDR Ec/Io and Io to the caller. It returns the Io estimate made by 
  rx AGC loops in the DSP and the Ec/Io estimate found by the searcher.

  Ec/Io range: from 5 ( -31.5 dB ) to 512 ( 0 dB )
  Io    range: from -106 dBm to -21 dBm 
    
  TBD: revB srch needs to know for which subaset to measuree; for now, use 
  subaset[0].

DEPENDENCIES
  None

INPUTS
  Pointers to the locations where EcIo and Io will be stored

RETURN VALUE
  EcIo and Io are returned through the passed pointers.

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchutil_get_ecio_and_io
(  
  int16 *ec_io,   
    /* pointer to EcIo value */

  int *io
    /* pointer to Io value   */    
)
{
  int              rx0_dbm, rx1_dbm, power; 
    /* received power read from the AGCs */

  boolean            diveristy;
    /* diversity settings */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ec_io != NULL )
  {
    *ec_io = hdrsrch.max_ecio;
      /* Pass the ecio value back to the caller   */
  }
    /*--------------*
    * Get Rx Power *
    *--------------*/

  if ( io != NULL )
  {
    hdrsrchrf_get_rx_power( &rx0_dbm, &rx1_dbm, &diveristy );
      /* Get rx power in dB units. We may read the stored value of RX AGC
         here if the DSP clock has been off */

    if ( diveristy )
    {
      power = MAX( rx0_dbm, rx1_dbm );
      /* Represents total Io. Take average of two antennas here. Both 
         antennae will report very similar numbers but take MAX. This will 
         cover the case where testing is done with ant0 disconnected with 
         diversity ON. */
    }
    else
    {
      power = rx0_dbm;
        /* else read just ant0 */
    }

    *io = power;
      /* Pass the Io value back to the caller   */
  }

}/* hdrsrchutil_get_ecio_and_io */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_TXAGC

DESCRIPTION
  Returns HDR txAGC in dBm unit to the caller. 
  txAGC range: from -64 to 42 dBm
      
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  tx_dbm - txAGC

SIDE EFFECTS
  None

==========================================================================*/

int hdrsrchutil_get_txagc( void )
{
  int tx_dbm;
    /* txAGC value */ 

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  tx_dbm = -64;
    /* Init the value to be minimum */

  if ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) 
  { 

    tx_dbm  =  rfm_hdr_get_tx_total_power( hdrsrchrf_get_rf_dev( 
                                             HDRSRCHRF_TX_CHAIN ), 
                                           RFM_HDR_CARRIER_ID__0, 
                                           RFM_HDR_AGC_FORMAT__DBM256 );

    tx_dbm += ( tx_dbm < 0 ? -128:128 ); 
      /* Round of to nearest 0.5dB */

    tx_dbm /= 256;                 
      /* Convert to full dBm unit  */
  }
  return tx_dbm;

} /* hdrsrchutil_get_txagc */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_SUSPEND_RPT

DESCRIPTION
  Report to the upper layers (if interested) gain/loss of RF.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchutil_suspend_rpt
(
  hdrsrch_suspend_enum_type     suspend_report
    /* Gain or Loss of RF resources */
)
{
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Suspend Report %d, state %x",
                  suspend_report, hdrsrch_state_id );

  if ( hdrsrch.suspend_cb != NULL )
  {
    hdrsrch.suspend_cb( suspend_report );
      /* Report to upper layer the gain/loss of the RF */
  }

} /* hdrsrchutil_suspend_rpt */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_FIR_FILTERING

DESCRIPTION
             This function returns FIR filtered values over lenth N. This is moving average filter and 
             recursively calculates the filtered output over multiple calls to this function.
    
DEPENDENCIES
  None

INPUTS
  Input unfiltered sample

RETURN VALUE
  Filtered values

SIDE EFFECTS
  none

==========================================================================*/

uint16 hdrsrchutil_fir_filtering( uint16 basp_sinr )
{
  static uint32 curr_sample_acc       = 0;
    /* Samples accumulated */
  static uint16 sample_index          = 0;
    /* Circular buffer index */
  static uint16 init_filt_sample_cnt  = 1;
    /* Init time sample count (for Length < filter length) */
  static uint16 earliest_sample       = 0;
    /* Earliest sample stored in circular buffer */
  static uint16 sample_array[HDR_FIR_FILT_LEN] = {0};
    /* Circular buffer */
  uint16 filtered_output = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  earliest_sample = sample_array[sample_index];
    /* Retrive the earliest sample from buffer */

  sample_array[sample_index] = basp_sinr;
    /* Store current sample as previous samples */

  if ( init_filt_sample_cnt < HDR_FIR_FILT_LEN )
  {
    /* If filter taps are not completely filled with samples use following algorithm (count from 1 to N-1) */
    curr_sample_acc = basp_sinr + curr_sample_acc;

    filtered_output = curr_sample_acc / init_filt_sample_cnt;
      /* Accumulate and average over sample count */
    
    init_filt_sample_cnt++;
  }
  else
  {
    /* All the TAPs are filled with samples */ 
    curr_sample_acc = basp_sinr + curr_sample_acc - earliest_sample;
      /* Running accumumaltion with adding new current sample and subtracting earliest sample */
       
    filtered_output = curr_sample_acc / HDR_FIR_FILT_LEN;
      /* Averaged over Filter length */
  }

  sample_index++;
 
  if ( sample_index >= HDR_FIR_FILT_LEN )
  {
    sample_index = 0;
  }      

  return filtered_output;

}


/*==========================================================================

FUNCTION        HDRSRCHUTIL_UPDATE_DATA_IND_SINR

DESCRIPTION
  This function updates the average SINR of the best sector that AT is
  listening to. This SINR can directly be mapped to the data rate that
  AT can request at this time.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  updates hdrsrchutil_data_ind_sinr

==========================================================================*/

void hdrsrchutil_update_data_ind_sinr ( void )
{
  uint16                     basp_sinr;
    /* best ASP SINR */

  hdrsrch_demod_idx_type     demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update the average SINR only if the DSP clocks are on */

  if ( hdrmdsp_check_dsp_clk() )
  {
    basp_sinr = HDRASP_GET_FILT_SINR( demod_idx,
                                      HDRASP_GET_BASP_RL( demod_idx ) );
      /* Get best ASP SINR */

    if( ( hdrsrchfing_are_fings_assigned() ) && ( basp_sinr == 0 ) )
    {
      /* Do not log, basp_sinr may not have been updated by FW */
      HDR_MSG_SRCH_2( 
        MSG_LEGACY_HIGH, 
        "hdrsrchutil_update_data_ind_sinr logging ignored: hdrsrchfing_are_fings_assigned = %d,basp_sinr = %d",
        hdrsrchfing_are_fings_assigned(), basp_sinr );
    }
    else
    {     
      if ( hdrsrch_get_current_state() == HDRSRCH_IDLE_STATE )
      {
        hdrsrchutil_data_ind_sinr = hdrsrchutil_fir_filtering( basp_sinr );
          /* filter the signal estimate with an FIR filter */
      }
      else
      {
      hdrsrchutil_data_ind_sinr = IIR_FILT_N( hdrsrchutil_data_ind_sinr,
                                              basp_sinr, 
                                              HDR_DATA_INDICATOR_FILT_LEN );
        /* filter the signal estimate with an IIR filter */
    }
   }
  }

}/* hdrsrchutil_update_data_ind_sinr */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_DATA_INDICATOR

DESCRIPTION
             This function returns the data level indicator. The data rate
             that the AT can receive is divided into 9 levels, 0 to 8. Level
             8 indicates highest data rate and level 0 indicates almost no
             data rate.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  0-8 equivalent to 9 levels of data activity. Meant to be mapped to bar 
  display 

SIDE EFFECTS
  none

==========================================================================*/

byte hdrsrchutil_get_data_indicator ( void )
{
  byte                       level;
    /* loop variable */

  int                        tab_size;
    /* size of the data level indicator table */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* get the table size. Note that the table contents and size can be 
     changed for different data level resolution easily */

  tab_size = sizeof( hdrsrchutil_data_ind_tab ) / 
             sizeof ( hdrsrchutil_data_ind_tab[0] ); 


  for ( level = 0; level < tab_size; level++ )
  {
    if ( hdrsrchutil_data_ind_sinr < hdrsrchutil_data_ind_tab[level] ) 
    {
      /* Stop. SINR is less than the max needed for this level */
      return level;
    }
  }

  return level-1;

}/* hdrsrchutil_get_data_indicator*/

/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_TABLE_SEARCH

DESCRIPTION  : Searches a decending sorted table for a given value.  The
               index of the largest value less than or equal to the
               given value is returned.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

LOCAL int hdrsrchutil_table_search
(
  uint16                          value,
    /* Value to search for */

  const uint16                    table[],
    /* Table to be searched */

  int                             size
    /* Table size */
)
{
  int bot, top, mid;
    /* Loop variables */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( value >= table[0] )
  {
    /* Value is larger than largest entry in table.  Clip to max */

    return 0;
  }
  else if ( value < table[ size - 2] )
  {
    /* Value is smaller than second smallest, clip to minimum */

    return size - 1;
  }
  else
  {
    top = 1;
    bot = size - 2;

    while ( top <= bot )
    {
      mid = (top + bot) / 2;
        /* Compute midpoint */

      if ( value >= table[ mid-1 ] )
      {
        /* One was too hot ... */

        bot = mid - 1;
      }
      else if ( value < table[ mid ] )
      {
        /* One was too cold ... */

        top = mid + 1;
      }
      else
      {
        /* The third was just right! */

        return mid;
      }

    } /* while */

    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR, "Search Failed: t=%d b=%d v=%d",
                    top, bot, value );

    return top;
  }

} /* hdrsrchutil_table_search */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_DATA_INDICATOR_DB

DESCRIPTION
             This function returns the data level indicator from 0 - 75
             in 0.5 db units with a range from 18 dB to -19 dB.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  0-75 in 0.5 db units with a range from 18 dB to -19 dB.

SIDE EFFECTS
  none

==========================================================================*/

byte hdrsrchutil_get_data_indicator_db ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return (byte) hdrsrchutil_table_search( hdrsrchutil_data_ind_sinr,
    hdrsrchutil_data_ind_tab_db,
    sizeof(hdrsrchutil_data_ind_tab_db)/sizeof(hdrsrchutil_data_ind_tab_db[0]) );
    /* Look for index of the largest less than or equal to
       hdrsrchutil_data_ind_sinr value in the decending table. */

}/* hdrsrchutil_get_data_indicator_db */


/*==========================================================================

FUNCTION        HDRSRCHUTIL_CONVERT_LINEAR_SINR_TO_DB

DESCRIPTION
             This function converts a linear sinr from 0 - 75
             in 0.5 db units with a range from 18 dB to -19 dB.
    
DEPENDENCIES
  None

RETURN VALUE
  0-75 in 0.5 db units with a range from 18 dB to -19 dB.

SIDE EFFECTS
  none

==========================================================================*/

byte hdrsrchutil_convert_linear_sinr_to_db 
( 
  uint16     sinr_lin
    /* linear SINR */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return (byte) hdrsrchutil_table_search( sinr_lin,
    hdrsrchutil_data_ind_tab_db,
    sizeof(hdrsrchutil_data_ind_tab_db)/sizeof(hdrsrchutil_data_ind_tab_db[0]) );
    /* Look for index of the largest less than or equal to
       hdrsrchutil_data_ind_sinr value in the decending table. */

}/* hdrsrchutil_convert_linear_sinr_to_db */


/*==========================================================================

FUNCTION        HDRSRCHUTIL_TC_OFS_START_GOV

DESCRIPTION
  This routine starts the OFS rate governer

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

==========================================================================*/

void hdrsrchutil_tc_ofs_start_gov ( void )
{
  uint16                      current_count;
    /* PN roll counter value for current time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  current_count = hdrts_get_roll_count();
    /* Get the now count for PN roll counter */

  hdrsrchutil.ofs_trigger_x26ms =  current_count + 
                                   HDRSRCH_OFS_INTERVAL_PN_ROLL;

}/* hdrsrchutil_tc_ofs_start_gov */


/*==========================================================================

FUNCTION        HDRSRCHUTIL_TC_OFS_ALLOWED

DESCRIPTION
  This routine services as the rate control timer for OFS in traffic state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Returns TRUE if there have been more than HDRSRCH_OFS_INTERVAL number of
  PN rolls happened, since the last time that the PN roll count was recored.
  Returns FALSE otherwise.

SIDE EFFECTS
  none

==========================================================================*/

boolean hdrsrchutil_tc_ofs_allowed ( void )
{
  uint16                      current_count;
    /* Get current PN roll count */

  int16                      result;
    /* Result of PN counter comparison */

  boolean                    ofs_allowed = FALSE;
    /* is OFS in tc allowed ? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  current_count = hdrts_get_roll_count();
    /* Get the now count for PN roll counter */

  result = HDRTS_ROLL_COUNT_COMPARE( current_count,
                                     hdrsrchutil.ofs_trigger_x26ms );
  /* if result == 0 now_count has reached trigger point
        result >  0 now_count has passed trigger point
        result <  0 now_count is yet to reach trigger point */

  if ( result >= 0 )
  {
    hdrsrchutil.ofs_trigger_x26ms =  current_count + 
                                     HDRSRCH_OFS_INTERVAL_PN_ROLL;
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "OFS in TC 6s Timer expired at %d!", 
                    current_count );

    ofs_allowed = TRUE;
  }

  return ofs_allowed;


}/* hdrsrchutil_tc_ofs_allowed */


/* EJECT */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_VALIDATE_SEARCHER_RESULTS

DESCRIPTION  : This function will be called after a status dump to validate
               if HDR AGC is working accurately. This function adds the valid
               path energies in the searched pilots and if the total Ec/Io
               exceeds the max allowed limit then it ignore the searcher 
               results.

               This function is being added to resolve the "bogus PN issue"
               seen occasionally on 1xEVDO where there is delay in AGC 
               compensation for deep fade resulting in increased gain for normal
               signal level. This causes elevated noise floor and hence we see
               bogus PNs. The following algorithm attempts to solve this 
               problem.

               Validation will be performed on a per carrier basis.

DEPENDENCIES : This function should be called only after dump and assumes the
               srch list has been sorted using carrier_idx prior to starting
               srch.

INPUT        : None

RETURN VALUE : TRUE = At least one carrier's search Results look ok
               FALSE= All carrier's search results exceed max allowed range

SIDE EFFECTS : May ignore searcher results if AGC seems to be saturating
               hdrsrchdrv.carrier_info[].srch_valid is set.

=========================================================================*/

boolean hdrsrchutil_validate_searcher_results ( void )
{
  int                               i, j, p;
    /* loop variables */

  boolean                           strong_pilot_flag;
    /* Is the pilot strong enough ? */

  hdrsrchdrv_srch_val_struct_type*  srch_val_ptr;
    /* Pointer to current demod carrier index in local data structure */

  hdrsrchdrv_srch_res_struct_type*  res_ptr;
    /* Pointer to current results buffer in hdrsrchdrv's srch_list */

  int16                             res_idx = 0;
    /* Index of srch_list's corresponding results buffer */

  boolean                           valid = FALSE;
    /* TRUE when searcher energies are valid for at least one carrier.
       Assume FALSE. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Loop through each carrier's srch result and perform the following:
     1. Per carrier, accumulate total energy and number of strong pilots.
     2. After all srch result energies are accummulated, compare each
        carrier's accumulated total energy against a total pilot threshold.
        The total pilot threshold value is dependant on the number of
        "strong pilots" encountered.
     3. If one or more of the carriers accumulated energies are valid, i.e.,
        below the total pilot threshold, then the entire srch results are
        considered valid, even if some carriers are below threshold. */

  res_ptr = hdrsrchdrv.srch_list[res_idx].srch_task.res_ptr;
    /* Initialize pointer to beginning of results buffer. */

  /* Loop through all carriers' srch results */
  for ( i = 0;
        ( ( i < hdrsrchdrv.num_carriers ) && ( i < HDRSRCH_MAX_DEMOD_CARRIERS_CNT ) );
        i++ )
  {

    memset( &hdrsrchdrv.carrier_info[i].srch_val,
            0,
            sizeof( hdrsrchdrv.carrier_info[i].srch_val ) );
      /* Initialize local energy accumulation. */

    /* Check if maximum carrier index is not exceeded */
    if ( i < HDRSRCH_MAX_DEMOD_CARRIERS_CNT )
    {
      srch_val_ptr = &hdrsrchdrv.carrier_info[i].srch_val;
        /* Use convenience pointer to accumulate results */

      /* Loop through carrier's srch result */
      for ( j = 0; j < hdrsrchdrv.carrier_info[i].srch_task_cnt; j++ )
      {
        strong_pilot_flag               = FALSE;
          /* Pilot is not strong enough until at least one*/

        for ( p = 0; p < HDR_MAX_SRCH_PEAKS; p++ )
        {
          if ( ( res_ptr != NULL ) &&
               ( res_ptr->eng[p] > HDR_SRCH_MIN_PATH_ENG ) )
          {
            srch_val_ptr->total_eng += res_ptr->eng[p];
              /* Add all the valid searcher Ec/Ios to get total searched Ec/Io */

            strong_pilot_flag = TRUE;
              /* this pilot is strong enough to be considered in total eng */

          }/* if */

        }/* for p */

        if ( strong_pilot_flag )
        {
          srch_val_ptr->strong_pilot_count++;
            /* Count how many pilots had at least one path > min path eng */
        }

        res_idx++;
        res_ptr = hdrsrchdrv.srch_list[res_idx].srch_task.res_ptr;
          /* Point to next resbuf entry. */

      } /* for j */

      /* Check if current carrier's total Ec/Io is too high */
      if ( srch_val_ptr->total_eng >
           HDRSRCHUTIL_VALIDATE_SRCH_THRESH( srch_val_ptr->strong_pilot_count ) )
      {
        hdrsrchdrv.carrier_info[i].srch_valid = FALSE;
          /* Carrier has an invalid total energy.  */

        HDR_MSG_SRCH_8( MSG_LEGACY_HIGH,
     "Total Ec/Io too high (> %d): Num Carr=%d, idx/eng=(%d,%d) (%d,%d) (%d,%d)",
                        HDRSRCHUTIL_VALIDATE_SRCH_THRESH(
                          srch_val_ptr->strong_pilot_count ),
                        hdrsrchdrv.num_carriers,
                        hdrsrchdrv.carrier_info[0].srch_val.carrier_idx,
                        hdrsrchdrv.carrier_info[0].srch_val.total_eng,
                        hdrsrchdrv.carrier_info[1].srch_val.carrier_idx,
                        hdrsrchdrv.carrier_info[1].srch_val.total_eng,
                        hdrsrchdrv.carrier_info[2].srch_val.carrier_idx,
                        hdrsrchdrv.carrier_info[2].srch_val.total_eng );

      }
      else
      {
        hdrsrchdrv.carrier_info[i].srch_valid = TRUE;

        valid     = TRUE;
          /* At least 1 carrier has an valid total energy. */
      }
    } /* i < HDRSRCH_MAX_DEMOD_CARRIERS_CNT */
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                      "Too many carriers (%d) in srch list. Abort srch validatiion.",
                      hdrsrchdrv.num_carriers );
      return FALSE;
    }

  } /* Per carrier srch result loop */

  return valid;

} /* hdrsrchutil_validate_searcher_results */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_HDR_PILOT_METRIC

DESCRIPTION
  This function computes the HDR pilot metric based on A,C,N set pilots 
  and returns the maximum energy of all the carriers.

DEPENDENCIES
  None

PARAMETERS
  Minimum pilot thershold to be used for consideration in the metric
  Pointers to fill the values for A/C/N set energies included in the metric
  and subaset with the maximum energy.
  

RETURN VALUE
  Total Metric Ec/Io computed.

SIDE EFFECTS
  None.

===========================================================================*/

uint16 hdrsrchutil_get_hdr_pilot_metric
(
  uint16                     min_eng_thresh,
    /* minimum ec/io threshold to include the pilot in the metric */

  uint16                     *aeng, 
  uint16                     *ceng, 
  uint16                     *neng
    /* pass back the aset/cset/nset energies to the calling function */
#ifdef FEATURE_HDR_REVB
  ,uint8                      *sa_index
    /* The subaset index with the max energy */
#endif /* FEATURE_HDR_REVB */

)
{
  int                     i;
    /* Loop Index */

  uint16                  aset_eng=0, cset_eng=0, nset_eng=0;
    /* Total ASet energy, Total CSet Energy, and Max NSet Energy */

  uint16                  total_eng = 0;
    /* Sum of ASet, CSet, and best NSet pilot energies */

#ifdef FEATURE_HDR_REVB
  uint16                  sa_eng = 0;
    /* The energy for individual subaset */

  uint8                   sa;
    /* Subaset index */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize */
#ifdef FEATURE_HDR_REVB
  *sa_index = 0;
#endif /* FEATURE_HDR_REVB */
  *aeng = *ceng = *neng = 0;

  /* Determine strength (raw energy) of all ASet pilots, CSet pilots,
     and the strongest NSet pilot */

#ifdef FEATURE_HDR_REVB
  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ ) 
  {
    aset_eng = 0;
      /* Reset aset energy */

    /* Calculate aset energy */
    for ( i = 0; i < hdrsrchset.subaset[sa].aset_cnt; i++ )
    {
      if ( hdrsrchset.subaset[sa].aset[i]->total_eng > min_eng_thresh )
      {
        aset_eng += hdrsrchset.subaset[sa].aset[i]->total_eng;
      }
    }

    cset_eng = 0;
      /* Reset cset energy */

    /* Calculate cset energy */
    for ( i = 0; i < hdrsrchset.cset_cnt; i++ )
    {
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.subaset[sa].channel, 
                               hdrsrchset.cset[i]->chan )       &&
           ( hdrsrchset.cset[i]->total_eng > min_eng_thresh ) )
      {
        cset_eng += hdrsrchset.cset[i]->total_eng;
      }
    }

    nset_eng = 0;
      /* Reset nset energy */

    /* Find max of the nset pilot */
    for ( i = 0; i < hdrsrchset.nset_cnt; i++ )
    {
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.subaset[sa].channel, 
                               hdrsrchset.nset[i]->chan )       &&
           ( hdrsrchset.nset[i]->total_eng > nset_eng ) )
      {
        nset_eng = hdrsrchset.nset[i]->total_eng;
      }
    }

    if ( nset_eng < min_eng_thresh )
    {
      nset_eng = 0;
    }

    sa_eng = aset_eng + cset_eng + nset_eng;
      /* Add total aset energy, total cset energy, and maximum nset energy */

    /* Pass individual energy back to the calling function*/
    if ( sa_eng > total_eng ) 
    {
      total_eng = sa_eng;
      *sa_index = sa;
      *aeng = aset_eng; 
      *ceng = cset_eng;
      *neng = nset_eng;
    }
  }
#else
  for(i=0; i<hdrsrchset.aset_cnt; i++)
  {
    if ( hdrsrchset.aset[i]->total_eng > min_eng_thresh )
    {
      aset_eng += hdrsrchset.aset[i]->total_eng;
    }
  }

  for(i=0; i<hdrsrchset.cset_cnt; i++)
  {
    if ( hdrsrchset.cset[i]->total_eng > min_eng_thresh )
    {
      cset_eng += hdrsrchset.cset[i]->total_eng;
    }
  }

  /* Find max of the nset pilot */
  for(i=0; i<hdrsrchset.nset_cnt; i++)
  {
    if (hdrsrchset.nset[i]->total_eng > nset_eng)
    {
      nset_eng = hdrsrchset.nset[i]->total_eng;
    }
  }

  if ( nset_eng < min_eng_thresh )
  {
    nset_eng = 0;
  }

  total_eng = aset_eng + cset_eng + nset_eng;
    /* Add total aset energy, total cset energy, and maximum nset energy */

  /* Pass the individual energies back to the calling function */
  *aeng = aset_eng; 
  *ceng = cset_eng;
  *neng = nset_eng;
#endif /* FEATUE_HDR_REVB */

  return total_eng;

} /* hdrsrchutil_get_hdr_pilot_metric */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_START_TIMER

DESCRIPTION
  This function reads the current sleep timertick count into the passed timer.

DEPENDENCIES
  None
  
PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchutil_start_timer( timetick_type *timer)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  (*timer) = timetick_get( );
    /* Read the current sleep timertick count */

} /* hdrsrchutil_start_timer */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_STOP_TIMER_MS

DESCRIPTION
  Compute the return time elapsed from a previous timetick value

DEPENDENCIES
  None
  
PARAMETERS
  None

RETURN VALUE
  Return the elapsed time in millisecond

SIDE EFFECTS

===========================================================================*/

timetick_type hdrsrchutil_stop_timer_ms( timetick_type timer)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return timetick_get_elapsed( timer, T_MSEC );
    /* return the elapsed time in millisecond   */
} /* hdrsrchutil_stop_timer_ms  */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_IS_DIFFERENT_SUBNET

DESCRIPTION
  This function checks whether two sectors are on different subnets.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : TRUE  = On different subnets.
               FALSE = On the same subnet or unknown.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrsrchutil_is_different_subnet
(
  hdrsrch_color_code_type aset_cc,
   /* color code of aset */

  hdrsrch_color_code_type cset_cc
    /* color code of cset */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if ( cset_cc != HDRSRCHSET_NO_COLOR_CODE && 
       aset_cc != HDRSRCHSET_NO_COLOR_CODE && cset_cc != aset_cc  )
  {
    /* Sector with unknown color code should be treated as in same subnet */
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}  /* hdrsrchutil_different_subnet */

/*===========================================================================

FUNCTION HDRSRCHUTIL_ADJUST_THRESH_FOR_SBHO

DESCRIPTION
  This function adjusts the idle handoff threshold depending upon whether or
  not the pilots have different subnets (denoted by the color code)

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

uint16 hdrsrchutil_adjust_thresh_for_sbho
(
  uint16                     aset_eng,
    /* Energy of the active set pilot which may be replaced */

  uint16                     comp_eng,
    /* Energy threshold that needs to be adjusted for Subnet handoff */

  hdrsrch_color_code_type    aset_cc,
  hdrsrch_color_code_type    cand_cc
    /* Color codes for two sectors */
)
{
  uint16                     mod_comp_eng;
    /* modified compare energy */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( cand_cc != HDRSRCHSET_NO_COLOR_CODE && 
       aset_cc != HDRSRCHSET_NO_COLOR_CODE && cand_cc != aset_cc  )
  {
    /* Pilots are on subnet boundary */

    if ( aset_eng > HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH )
    {
      mod_comp_eng = hdrsrchset_scale_eng( comp_eng,
                                           HDRSRCHIDLE_SBHO_ADDON_HI_HALF_DB );
    }
    else
    {
      mod_comp_eng = hdrsrchset_scale_eng( comp_eng,
                                           HDRSRCHIDLE_SBHO_ADDON_LO_HALF_DB );
    }
  }
  else
  {
    mod_comp_eng = comp_eng;
      /* No subnet handoff so don't change threshold */
  }

  return mod_comp_eng;

}/* hdrsrchutil_adjust_thresh_for_sbho */


#ifdef DEBUG_HDR_SAMPLE_RAM_DUMP 
/*===========================================================================

FUNCTION        HDRSRCHUTIL_READ_SAMPLE_RAM

DESCRIPTION     
  This function reads the sample ram buffer and stores the samples in an array

DEPENDENCIES   
  None

RETURN VALUE   
  None.

SIDE EFFECTS   
  None.

===========================================================================*/

void hdrsrchutil_read_sample_ram( void )
{
  int32 i;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();

  HWIO_SAMPSRV_TEST_RAM_WE_CTL_OUT(0);
  HWIO_SAMPSRV_TEST_RAM_ADDR_OUT(0);
    /* Configue sample ram test bus interface  */

  /* Now read Rx0 sample ram */

  for (i = 0; i < 2048; i++)
  {
     hdrsrchutil.rx0_sample_ram[i] = HWIO_SAMPSRV_TEST_RAM_DATA_IN;
  }

  REX_ENABLE_PREMPTION();
  
} /* hdrsrchutil_read_sample_ram */

#endif /* DEBUG_HDR_SAMPLE_RAM_DUMP */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_SINR_AND_RXAGC

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

void hdrsrchutil_get_sinr_and_rxagc
( 
  byte    *sinr, 
    /* pointer to SINR indicator value */

  int     *rxagc
    /* pointer to RxAgc value   */    
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sinr != NULL )
  {
    *sinr = hdrsrchutil_get_data_indicator();
      /* Get the SINR indicator value */
  }

  if ( rxagc != NULL )
  {
    hdrsrchutil_get_ecio_and_io( NULL, rxagc );
      /* Get the RxAgc value  */
  }

}/* hdrsrchutil_get_sinr_and_rxagc */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_SINR_AND_RXAGC_DB

DESCRIPTION
  This function returns SINR indicator and Rx AGC to the caller. SINR 
  indicator is from 0 to 75 in 0.5 dB units from 18.0dB to -19.0dB, e.g.
  0 = 18dB, 7=-14.5dB, 75 = -19dB.
  
  SINR indicator range: from 0 to 75 in 0.5 dB units from 18.0dB to -19.0dB
  RxAgc range         : from -106 dBm to -21 dBm 

DEPENDENCIES
  None

PARAMETERS
  *sinr - Pointer to return SINR value
  *rxagc - Pointer to return RxAGC

RETURN VALUE
  None
  
SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_get_sinr_and_rxagc_db
( 
  byte    *sinr, 
    /* pointer to SINR indicator value in db units */

  int     *rxagc
    /* pointer to RxAgc value   */    
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sinr != NULL )
  {
    *sinr = hdrsrchutil_get_data_indicator_db();
      /* Get the SINR indicator value in 0.5 dB units */
  }

  if ( rxagc != NULL )
  {
    hdrsrchutil_get_ecio_and_io( NULL, rxagc );
      /* Get the RxAgc value  */
  }

}/* hdrsrchutil_get_sinr_and_rxagc_db */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_DEC_DEACTIVATED_CB

DESCRIPTION
  This is the call back function to notify HDRSRCH task that the decoder is 
  deactivated.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_dec_deactivated_cb( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_DEC_DEACTIVATED_SIG );
    /* Set the signal */
} /* hdrsrchutil_dec_deactivated_cb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_DEC_DEACTIVATE

DESCRIPTION
  This function prepares decoder for deactivation, performs a timed wait 
  and then deactivates the decoder.

DEPENDENCIES
  A request to deassign fingers must be sent to the FW before calling this API.

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

void hdrsrchutil_dec_deactivate( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrdec_deactivate( hdrsrchutil_dec_deactivated_cb );
    /* deactivate HDR decoder */

  hdrsrch_timed_wait_for_sig( HDRSRCH_DEC_DEACTIVATED_SIG,
                              HDRSRCH_DEC_DEACTIVATED_WAIT_MS );
    /* Wait for decoder deactivated signal in the time limit */

} /* hdrsrchutil_dec_deactivate */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_CANCEL_FPD

DESCRIPTION
  This function cancels pending FPD.

DEPENDENCIES
  This function must be called in HDRSRCH task as it is not reentrant 

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void hdrsrchutil_cancel_fpd( void )
{
  if ( hdrsrchmsg.fpd_msg.forcePreamDetect != 0 )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_MED, "FPD %d => 0", 
                   hdrsrchmsg.fpd_msg.forcePreamDetect);

    hdrsrchmsg.fpd_msg.forcePreamDetect = 0;

    hdrsrchmsg_send_msg( &hdrsrchmsg.fpd_msg.hdr,
                         sizeof( hdrfw_fpd_msg_t ) );
      /* Disable forced preamble detection */

    hdrsrchreacq_reset_fpd_activation_status();
  }
} /* hdrsrchutil_cancel_fpd */

/*===========================================================================

FUNCTION HDRSRCHUTIL_TX_POWERDOWN_CB

DESCRIPTION
  This is the callback function to notify HDRSRCH task that tx is powerdown.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_tx_powerdown_cb( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_TX_POWERDOWN_SIG );
    /* Set the signal */

  HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, 
                 "tx powerdown cb is called and set sig 0x%x", 
                 HDRSRCH_TX_POWERDOWN_SIG );
} /* hdrsrchutil_tx_powerdown_cb */


/*===========================================================================

FUNCTION HDRSRCHUTIL_POWERDOWN_TX

DESCRIPTION
  This function powers down tx, performs a timed wait for tx powerdown.

DEPENDENCIES
  None.

PARAMETERS
  Tx powerdown reason 

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

void hdrsrchutil_powerdown_tx
(
  hdrrmac_tx_stop_reason_enum_type powerdown_reason
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_TX_POWERDOWN_SIG );
    /* Clear the Tx powerdown signal */

  hdrrmac_powerdown_tx( powerdown_reason, hdrsrchutil_tx_powerdown_cb );
    /* Power down tx */

  hdrsrch_timed_wait_for_sig( HDRSRCH_TX_POWERDOWN_SIG,
                              HDRSRCH_TX_POWERDOWN_WAIT_MS );
    /* Wait for tx powerdown signal in the time limit */

} /* hdrsrchutil_powerdown_tx */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHUTIL_CHECK_OFS_CONDITION

DESCRIPTION
  This function checks if all the aset and cset pilots are below a certain 
  threshold. The pilot energy used for each sector is the output of the 
  OFS IIR filter.

  TBD: does the revB use the same condition ?

DEPENDENCIES
  None

PARAMETERS
  Energy threshold

RETURN VALUE
  True if all the aset and cset pilots are below the passed threshold.
  False otherwise.

SIDE EFFECTS
  None

==========================================================================*/

boolean hdrsrchutil_check_ofs_condition
(
  uint16            energy_threshold
    /* Threshold for energy comparison, in linear scale  */
)
{
  int                                   i;
    /* loop index */

  boolean                               ofs_allowed = TRUE;
    /* Flag indicates if OFS is needed, from ASET and CSET perspective     */

#ifdef FEATURE_HDR_REVB
  uint8                ref_subaset_idx ;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_REVB
  /* For revB, the ASET should be vertically organized as pilot groups; 
     In the case, not need to compare entire ASET sectors; use one subASET
     instead. 
     TBD: which subaset should be used? should it be the pilot reference 
     subaset( the first one in TCA ) use subaset0 for now to be revA compatible */
  ref_subaset_idx = 0;
  /* ref_subaset_idx = hdrsrchset_get_ref_subaset_idx(); */

  for( i=0; i<hdrsrchset.subaset[ref_subaset_idx].aset_cnt; i++ )
  {
    if( hdrsrchset.subaset[ref_subaset_idx].aset[i]->pil_filter.total_eng >= 
                                                              energy_threshold )
    {
      /* Find a strong pilot, no need to do OFS   */
      ofs_allowed = FALSE;

      break;  /* No need to search more */

    } /* if */
  } /* for i=0  */
#else
  for (i=0; i < hdrsrchset.aset_cnt; i++)
  {
    if( hdrsrchset.aset[i]->pil_filter.total_eng >= energy_threshold )
    {
      /* Find a strong pilot, no need to do OFS   */
      ofs_allowed = FALSE;

      break;  /* No need to search more */

    } /* if */

  } /* for i=0  */
#endif /* FEATURE_HDR_REVB */

  /* Check CSET only when ASET pilots are weak        */
  if ( ofs_allowed == TRUE )
  {
    for (i=0; i<hdrsrchset.cset_cnt; i++)
    {
#ifdef FEATURE_HDR_REVB
      if( ! hdrsrchset_is_ofreq_chan( hdrsrchset.cset[i]->chan ) )
#else
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan , 
                               hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
      {
        /* Only look at CSET pilot on the current channel */

        if( hdrsrchset.cset[i]->pil_filter.total_eng >= energy_threshold )
        {
          /* Find a strong pilot, no need to do OFS   */
          ofs_allowed = FALSE;

          break;  /* No need to search more */

        } /* if hdrsrchset */

      } /* if SYS_CHAN_CMP_EQUAL  */

    } /* for i=0  */

  } /* if ofs_allowed */

  return ofs_allowed;

} /* hdrsrchutil_check_ofs_condition */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHUTIL_COUNT_STRONG_PILOTS

DESCRIPTION
  This function counts the number of aset and cset pilots that are above 
  the given threshold. It counts nset pilots also for small SCIs.

DEPENDENCIES
  None

PARAMETERS
  Energy threshold

RETURN VALUE
  The number of the aset and cset pilots that are above 
  the given threshold, and nset pilots above the threshold
  for small SCIs.
  
SIDE EFFECTS
  None

==========================================================================*/

uint16 hdrsrchutil_count_strong_pilots
(
  uint16            energy_threshold
    /* Threshold for energy comparison, in linear scale  */
)
{
  uint16                                strong_pilot_count;
    /* the number of strong pilots */

  int                                   i;
    /* loop index */

#ifdef FEATURE_HDR_REVB
  uint8                                 j;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  strong_pilot_count = 0;
    /* initialize the number of strong pilots */
#ifdef FEATURE_HDR_REVB
  for ( i = 0 ; i < hdrsrchset.subaset_cnt ; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      if( hdrsrchset.subaset[i].aset[j]->pil_filter.total_eng >= 
          energy_threshold )
      {
        strong_pilot_count++;
        /* increment the number of strong pilots */
      }
    }
  } /* for i=0  */
#else
  for ( i = 0 ; i < hdrsrchset.aset_cnt ; i++ )
  {
    if( hdrsrchset.aset[i]->pil_filter.total_eng >= energy_threshold )
    {
      strong_pilot_count++;
        /* increment the number of strong pilots */
    }
  } /* for i=0  */
#endif /* FEATURE_HDR_REVB */

  for ( i = 0 ; i < hdrsrchset.cset_cnt ; i++ )
  {
    /* Only look at CSET pilots on the current channel */
#ifdef FEATURE_HDR_REVB
    if( !hdrsrchset_is_ofreq_chan( hdrsrchset.cset[i]->chan ) ) 
#else
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan , 
                             hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {
      if( hdrsrchset.cset[i]->pil_filter.total_eng >= energy_threshold )
      {
        strong_pilot_count++;
          /* increment the number of strong pilots */
      }
    } /* if SYS_CHAN_CMP_EQUAL  */

  } /* for i=0  */


  /* Also look at NSET pilots for small SCIs.
     We use a different filter for small SCIs. It provide an more 
     accurate pilot measurement, but it delays the time to move the
     pilot from NSET to CSET comparing to old filter. That's why we
     need to count NSET pilots for small SCIs. */
  if ( hdrsrchsleep_get_negotiated_sci() <= 6 )
  {
    for ( i = 0 ; i < hdrsrchset.nset_cnt ; i++ )
    {
      /* Only look at NSET pilots on the current channel */
#ifdef FEATURE_HDR_REVB
    if( !hdrsrchset_is_ofreq_chan( hdrsrchset.nset[i]->chan ) ) 
#else
      if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.nset[i]->chan , 
                               hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
      {
        if( hdrsrchset.nset[i]->pil_filter.total_eng >= energy_threshold )
        {
          strong_pilot_count++;
            /* increment the number of strong pilots */
        }
      } /* if SYS_CHAN_CMP_EQUAL  */

    } /* for i=0  */
  }
  return ( strong_pilot_count );

} /* hdrsrchutil_count_strong_pilots */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_IS_SBHO_CANDIDATE

DESCRIPTION
  This function checks whether the cset pilot is an SBHO candidate.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : TRUE  = Cset pilot satisfies SBHO condition.
               FALSE = No.

SIDE EFFECTS
  None.

===========================================================================*/
boolean hdrsrchutil_is_sbho_candidate
(
  uint16           aset_eng,
    /* Active set energy */

  uint16           cset_eng
    /* Candidate set energy */
)
{

  timetick_type              now;
    /* The current time */

  timetick_type              delta_time;
    /* The time since the last handoff. */

  boolean                    sbho = FALSE;
    /* Is SBHO candidate */

  uint16                     thresh;
    /* Thresh for subnet HO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( aset_eng >= HDRSRCHIDLE_SBHO_STRONG_PILOT_ENG_THRESH )
  {
    now = timetick_get_ms();
      /* Get the current time */

    thresh = hdrsrchset_scale_eng( aset_eng, HDRSRCHIDLE_SBHO_HI_HALF_DB );
      /* Scale thresh */

    delta_time = ( uint32 ) ( (int32)now - (int32)hdrsrch.ho_time );

    if ( ( delta_time >= HDRSRCH_SBHO_INTERVAL_MS ) && 
         ( cset_eng > thresh )                                         )
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                      "SBHO candidate found cset_eng %d thresh %d delta %d ms",
                      cset_eng,
                      thresh,
                      delta_time );
      sbho = TRUE;
    }
  }
  else
  {
    /* aset < -9dB */
    thresh = hdrsrchset_scale_eng( aset_eng, HDRSRCHIDLE_SBHO_LO_HALF_DB );
      /* Scale thresh */

    if ( cset_eng > thresh )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                      "SBHO candidate found cset_eng %d thresh %d",
                      cset_eng,
                      thresh );
      sbho = TRUE;
    }
  }

  return sbho;

}  /* hdrsrchidle_is_sbho_candidate */


/*===========================================================================

FUNCTION HDRSRCHUTIL_SECT_IS_HO_CANDIDATE

DESCRIPTION
  This function determines whether a sector is a handoff candidate. The same
  subnet handoff is preferred. If a sector in the same subnet passes handoff
  criteria, then no cross subnet handoff no matter what its pilot energy is.

DEPENDENCIES
  None

PARAMETERS
  aset                - pointer to aset sector
  cset                - pointer to cset sector
  use_aset_inst_eng   - whether to use aset instantaneous energy
  best_eng            - engergy of the best sector so far
  min_thresh          - minimum threshold eng to be a ho candidate
  same_subnet_ho      - whether this is same subnet handoff 

RETURN VALUE : TRUE = the sector is handoff candidate. 

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchutil_sect_is_ho_candidate
(

  hdrsrch_sect_struct_type   *aset,
    /* aset sector */

  hdrsrch_sect_struct_type   *cset,
    /* cset sector */

  boolean                    use_aset_inst_eng,
    /* Whether to use aset instantaneous energy to pick ho candidate*/

  uint16                     best_eng,
    /* Best energy so far */

  uint16                     min_thresh,
    /* minimum threshold eng */

  boolean                    *same_subnet_ho
    /* Whether this is same subnet handoff */
)
{
  boolean                    subnet_boundary;
    /* Whether we are on subnet boundary */

  boolean                    handoff = FALSE;
    /* Whether this is a HO candidate */

  boolean                    sbho = FALSE;
    /* Whether this is a subnet handoff candidate */

  uint16                     cset_idho_cmp_eng;
    /* cset energy used to decide idle handoff candidate */

  uint16                     cset_sbho_cmp_eng;
    /* cset energy used to decide idle handoff candidate across subnet */

  uint16                     aset_sbho_cmp_eng;
    /* aset energy used to decide idle handoff candidate across subnet */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  subnet_boundary = hdrsrchutil_is_different_subnet( aset->color_code, 
                                                     cset->color_code );
    /* Check whether it is at the subnet boundary */

  if ( use_aset_inst_eng ) 
  {
    /* Use aset instantaneous energy */
    aset_sbho_cmp_eng = aset->inst_eng;
  }
  else
  {
    /* Use aset filtered energy */
    aset_sbho_cmp_eng = aset->sbho_filter.total_eng;
  }

  cset_idho_cmp_eng = cset->total_eng;
  cset_sbho_cmp_eng = cset->sbho_filter.total_eng;
    /* Use cset filtered energy to select handoff candidate */

  if ( *same_subnet_ho == FALSE ) 
  {
    /* There is no same-subnet handoff candidate yet */
    if ( subnet_boundary == TRUE )
    {  
      sbho = hdrsrchutil_is_sbho_candidate( aset_sbho_cmp_eng,
                                            cset_sbho_cmp_eng );

      if ( ( sbho == TRUE ) && ( cset_idho_cmp_eng > best_eng ) )
      {

        handoff = TRUE;
          /* Found a stronger pilot */
      }
    }
    else
    {
      if ( cset_idho_cmp_eng > min_thresh ) 
      {
        /* Found the first same subnet handoff candidate */

        *same_subnet_ho = TRUE;
          /* Set the flag for same subent handoff */

        handoff = TRUE;
          /* Found a stronger pilot */
      }
    } /* if ( subnet_boundary == TRUE ) */
  }
  else
  {
    /* Have previously found same subnet handoff candidate */
    if ( subnet_boundary == FALSE )
    {  
      if ( cset_idho_cmp_eng > best_eng )
      {
        handoff = TRUE;
          /* Found a stronger pilot */
      }
    }
  } /* if ( *same_subnet_ho == FALSE ) */

  return handoff;

} /* hdrsrchutil_sect_is_ho_candidate( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_SECT_IS_HO_CANDIDATE_DYNAMIC_THRESH

DESCRIPTION
  This function determines whether a sector is a handoff candidate. The same
  subnet handoff is preferred. It considers different thresholds for cached
  sector and not-cached sector.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : TRUE = the sector is handoff candidate. 

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchutil_sect_is_ho_candidate_dynamic_thresh
(

  hdrsrch_sect_struct_type   *aset,
    /* aset sector */

  hdrsrch_sect_struct_type   *cset,
    /* candidate sector */

  boolean                    use_inst_eng,
    /* Whether to use instantaneous energy to pick ho candidate*/

  uint16                     min_thresh_cached,
    /* Minimum threshold eng */

  uint16                     min_thresh_notcached,
    /* Minimum threshold eng */

  uint16                     best_eng,
   /* Best energy so far */

  hdrsrch_sect_struct_type   *best_sect,
    /* Strongest sector so far */

  boolean                    *candidate_cached,
    /* Whether the handoff candidate is cached */

  boolean                    *same_subnet_ho
    /* Whether this is same subnet handoff */
)
{
  uint16                    best_eng_tmp, min_thresh_tmp;
    /* Best energy and minimum threshold eng */

  boolean                   ho_candidate;
    /* Whether this is a HO candidate */

  boolean                   sect_cached;
    /* Flag whether the sector is cached */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sect_cached = hdrsrchutil_sector_is_cached( cset );
    /* Whether cset is cached */

  if (( *candidate_cached == FALSE ) && ( sect_cached == TRUE ))
  {
    /* we don't have a cached handoff candidate and this sector is cached */
    best_eng_tmp = min_thresh_cached;
    min_thresh_tmp = min_thresh_cached;
  }
  else if (( *candidate_cached == FALSE ) && ( sect_cached == FALSE ))
  {
    /* this sector is not cached */
    if ( best_sect == NULL )
    {
      /* We don't have any candidate sector yet */
      best_eng_tmp = min_thresh_notcached;
    }
    else
    {
      /* We have a not-cached candidate sector*/
      best_eng_tmp = best_eng;
    }
    min_thresh_tmp = min_thresh_notcached;
  }
  else if (( *candidate_cached == TRUE ) && ( sect_cached == TRUE ))
  {
    /* we have a cached handoff candidate and this sector is cached */
    best_eng_tmp = best_eng; 
    min_thresh_tmp = min_thresh_cached;
  }
  else 
  {
    /* We have a cached handoff candidate and this sector is not cached.
       This sector is not a candidate, so skip it*/
    return FALSE;
  }

  ho_candidate = hdrsrchutil_sect_is_ho_candidate(
                     aset,
                     cset,
                     FALSE,  /* use filtered energy */ 
                     best_eng_tmp,
                     min_thresh_tmp,
                     same_subnet_ho );

  if ( ho_candidate )
  {
    *candidate_cached = sect_cached;
      /* Remember whether the candidate sector is cached or not. */
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* hdrsrchutil_sect_is_ho_candidate_dynamic_thresh */

/*===========================================================================

FUNCTION HDRSRCHUTIL_CALC_DYNAMIC_HO_THRESHOLDS

DESCRIPTION
  This function calculates idle handoff thresholds. It considers factors as
  the sector is cached or not, how strong is the current aset pilot.


DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : 
  None

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_calc_dynamic_ho_thresholds
(
  uint16                     aset_eng,
   /* Aset energy */

  uint16                     *min_thresh_cached,
    /* handoff threshold for cached sector */

  uint16                     *min_thresh_notcached
    /* handoff threshold for not cached sector */
)
{
  uint8                     ho_thresh_cached;
    /* threshold in 0.5db for same subnet with cached overhead*/

  uint8                     ho_thresh_notcached;
    /* threshold in 0.5db for same subnet without cached overhead*/

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*ho_thresh_cached   = ( aset_eng <= HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH )  ?
           HDRSRCHIDLE_HO_THRESH_CACHED_LOW_HALF_DB :
           HDRSRCHIDLE_HO_THRESH_CACHED_HIGH_HALF_DB;

  ho_thresh_notcached =  ( aset_eng <= HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH )  ?
           HDRSRCHIDLE_HO_THRESH_NOTCACHED_LOW_HALF_DB :
           HDRSRCHIDLE_HO_THRESH_NOTCACHED_HIGH_HALF_DB;*/
  /* get threshold from debug mask -- this is temporarily for engineering build */
  ho_thresh_cached   = ( aset_eng <= HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH )  ?
           ((HDRSRCH_IDHO_THRESH_LO_CACHED == 0)? HDRSRCHIDLE_HO_THRESH_CACHED_LOW_HALF_DB : HDRSRCH_IDHO_THRESH_LO_CACHED ) :
           ((HDRSRCH_IDHO_THRESH_HI_CACHED == 0)? HDRSRCHIDLE_HO_THRESH_CACHED_HIGH_HALF_DB : HDRSRCH_IDHO_THRESH_HI_CACHED);

  ho_thresh_notcached =  ( aset_eng <= HDRSRCHIDLE_STRONG_PILOT_ENG_THRESH )  ?
           ((HDRSRCH_IDHO_THRESH_LO_NOT_CACHED == 0)? HDRSRCHIDLE_HO_THRESH_NOTCACHED_LOW_HALF_DB:HDRSRCH_IDHO_THRESH_LO_NOT_CACHED) :
           ((HDRSRCH_IDHO_THRESH_HI_NOT_CACHED == 0)? HDRSRCHIDLE_HO_THRESH_NOTCACHED_HIGH_HALF_DB : HDRSRCH_IDHO_THRESH_HI_NOT_CACHED);

  /* Temporarily put here for debug */

  *min_thresh_cached = hdrsrchset_scale_eng( aset_eng, ho_thresh_cached );
     /* minimum threshold to preform handoff to sector with canched overhead*/

  *min_thresh_notcached = hdrsrchset_scale_eng( aset_eng, ho_thresh_notcached );
      /* minimum threshold to preform handoff to sector without canched overhead*/

} /* hdrsrchutil_calc_dynamic_ho_thresholds() */

/* <EJECT> */

#ifdef FEATURE_HDRSRCH_POWER_PROFILE

/*===========================================================================

FUNCTION        hdrsrchutil_enable_pwr_strb

DESCRIPTION     
  init the GPIO and the power_strob_status

DEPENDENCIES   
  None

RETURN VALUE   
  None.

SIDE EFFECTS   
  None.

===========================================================================*/

void hdrsrchutil_enable_pwr_strb
(
  hdrsrchutil_pwr_strb_event_type event_id
    /* The event id associated with the strobe */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    hdrsrchutil_pwr_strb.enable = FALSE;
  hdrsrchutil_pwr_strb.curr_event_id   = event_id;
  hdrsrchutil_pwr_strb.mcpm_trigger = RCM_TRIGGER_MAX;

  if( HDRSRCHSLEEP_POWER_STROBE_NV_ENABLE == TRUE )
  {
  hdrsrchutil_pwr_strb.enable = TRUE;
    hdrsrchutil_pwr_strb.mcpm_trigger = RCM_TRIGGER_4;
      /* MCPM API specifies TRIGGER_4 for HDR to use */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "HDR pstrobe enabled!: ev_id=%d, trigger=%d",
                    event_id,
                    hdrsrchutil_pwr_strb.mcpm_trigger );
  }
} /* hdrsrchutil_enable_pwr_strobe */

/*===========================================================================

FUNCTION        hdrsrchutil_toggle_pwr_strb 

DESCRIPTION     
  If the pwr strobe is enable, then toggle the output; toggle is done by
  enabling the corresponding GPIO as a input and as a output.
  To not affect the power consumption, before go to sleep, the status should
  toggle to OFF state;

DEPENDENCIES   
  None

RETURN VALUE   
  None.

SIDE EFFECTS   
  None.

===========================================================================*/

void hdrsrchutil_toggle_pwr_strb
(
  hdrsrchutil_pwr_strb_event_type event_id
    /* The event id associated with the strobe */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !hdrsrchutil_pwr_strb.enable )
  {
    hdrsrchutil_enable_pwr_strb(event_id);
  }

  if ( ( hdrsrchutil_pwr_strb.enable ) &&
       ( hdrsrchutil_pwr_strb.mcpm_trigger < RCM_TRIGGER_MAX ) )
  {

    MCPM_GPIO_TOGGLE( hdrsrchutil_pwr_strb.mcpm_trigger );
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "HDR pstrobe toggled!: ev_id=%d, trigger=%d", 
                    event_id,
                    hdrsrchutil_pwr_strb.mcpm_trigger );
  }

} /* hdrsrchutil_toggle_pwr_strobe */

#endif /*FEATURE_HDRSRCH_POWER_PROFILE */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_CALC_TIME_TO_TUNE_AWAY

DESCRIPTION
  This function determines the appropriate tune away time given the 
  previously decided tune away time and the current request. Here are the
  rules:

  1) If the requested time is later than the currently selected tune away 
  time, always choose the requested time.
  2) If the requested time is earlier than the currently selected tune away
  time but later than the earliest time allowed to support DRC rampdown, choose
  the requested time.
  3) If the requested time is earlier than the currently selected tune away
  time but earlier than the earliest time allowed to support DRC rampdown, 
  choose the earliest time between the currently selected tune away time and
  the earliest time allowed to support DRC rampdown.

DEPENDENCIES
  None

RETURN VALUE
  The time remaining until tune away in sclks

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchutil_calc_time_to_tune_away
(
  uint32                                now_sclk_ts,
    /* Current time as an sclk timestamp */

  int32                                 min_ta_duration,
    /* What is the minimum time needed to tune away. */

  hdrsrchrf_retain_lock_state_t*        rl_state,
    /* How much time is left on the retain lock state */

  boolean*                              no_end,
    /* There is no time the lock must be release. This is both an 
       [in] parameter to indicate the previous state and an [out] 
       parameter to indicate the decided state. */

  uint32*                               end_sclk_ts,
    /* Tune away time as an sclk timestamp. This is both an 
       [in] parameter to indicate the previous value and an [out] 
       parameter to indicate the decided value. Note this is irrelevant
       if no_end is returned as TRUE. */

  boolean*                              is_ta_updated
    /* This flag will indicate if this function was able to update the
       tuneaway time to meet the (new) required time. */
)
{
  int32                                 cur_sclks_left;
    /* Number of remaining sclks on the lock. */

  int32                                 req_sclks_left = 0;
    /* Number of remaining sclks on the lock based on the request from
       TRM. */

  int32                                 new_sclks_left = 0;
    /* Number of remaining sclks on the lock. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !*no_end )
  {
    cur_sclks_left = 
                HDRSRCHUTIL_NAT_NUM( (int32)( *end_sclk_ts - now_sclk_ts ) );
      /* Calculate the number of sclks until the next tune away prior to the
         new calculation. Don't permit negative numbers. */
  }
  else
  {
    cur_sclks_left = HDRSRCHRF_MAX_DURATION;
      /* We don't currently plan on tuning away so set the current duration
         to the max. */
  }

  if ( rl_state->event == HDRSRCHRF_UNLOCK_CANCELLED )
  {
    req_sclks_left = HDRSRCHRF_MAX_DURATION;
      /* There is no longer an end time so just default the requested 
         duration to the maximum duration so that the below calculations
         will work out. */

    *no_end = TRUE;
        /* There is no longer an end time. */
  }
  else if ( rl_state->event == HDRSRCHRF_UNLOCK_BY )
  {
    req_sclks_left = HDRSRCHUTIL_NAT_NUM( 
                          (int32)( rl_state->unlock_by_sclk - now_sclk_ts ) );
      /* Calculate the newly requested number of sclks until the next tune 
         away. We'll see if we accept this new time below. */

    *no_end = FALSE;
      /* We know there is an end time now. */
  }
  else if ( ( hdrsrch_is_lre_enabled() && 
            rl_state->event == HDRSRCHRF_UNLOCK_IMMEDIATELY) ||
            rl_state->event == HDRSRCHRF_UNLOCK_REQUIRED ||
            rl_state->event == HDRSRCHRF_UNLOCK_BAND_INCOMPATIBLE )
  {
    req_sclks_left = 0;
      /* The request has us releasing the lock immediately. */

    *no_end = FALSE;
      /* We know there is an end time now. */
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Unknown tune away request: %d", 
                                                            rl_state->event );
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED,
                  "evt %d req: %d end: %d",
                  rl_state->event, req_sclks_left, *no_end );

  if ( cur_sclks_left < req_sclks_left )
  {
    if ( ( cur_sclks_left < min_ta_duration ) && ( *no_end == TRUE ) )
    {
      new_sclks_left = cur_sclks_left;
        /* If this is an unlock cancel but we are already within a tune away
           then let continue with the tune away. We don't know the exact time
           we must release the chain yet. */

      *no_end = FALSE;
        /* We are keeping the same end time. */
    }
    else
    {
      new_sclks_left = req_sclks_left;
        /* If the requested time known and is larger than the current tune
           away time, always accept it. */
    }
  }
  else
  {
    new_sclks_left = HDRSRCHUTIL_SHRINK_TIME_LEFT( cur_sclks_left, 
                                                   req_sclks_left,
                                                   min_ta_duration );
      /* Shrinks the current tune away time based on the newly requested tune
         away time and the minimum tune away duration. */
  }

  if ( is_ta_updated != NULL )
  {
    if ( new_sclks_left != req_sclks_left )
    {
      *is_ta_updated = FALSE;
    }
    else
    {
      *is_ta_updated = TRUE;
    }
  }

  *end_sclk_ts = now_sclk_ts + new_sclks_left;
    /* Update the time the lock will end. */

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, "Cur left: %d Proposed left: %d New left: %d %d",
                  cur_sclks_left, req_sclks_left, new_sclks_left, *end_sclk_ts );

  return (uint32) new_sclks_left;

} /* hdrsrchutil_calc_time_to_tune_away */


/* EJECT */
#ifdef FEATURE_HDR_REVB
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_NUM_UNIQUE_CELLS_IN_SUBASET

DESCRIPTION
  This function is used to find the number of unique cells in the sub active set.
  This number should be the same across all the subactive set ideally.

DEPENDENCIES
  None

LIMITATIONS
  The rpc_cell_idx can only be from 0 to 31. 

RETURN VALUE
  The number of unique cells in a SUBASET.  

SIDE EFFECTS
  None

===========================================================================*/

uint8 hdrsrchutil_get_num_unique_cells_in_subaset( uint8 subaset_idx )
{

  uint8 aset_idx; 
    /* aset index in the subaset */

  uint8 num_cells_in_subaset = 0; 
    /* Number of unique cells in an active set */

  uint32 bit_mask = 0;
    /* Bit mask with bit_position corresponding to cell idx set if 
       that cell is part of ASET */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* For each one of the cell indices in the ASET... */
  for( aset_idx = 0;
       ( aset_idx < hdrsrchset.subaset[subaset_idx].aset_cnt  );
       aset_idx++ )
  {
    /* If the bit position corresponding to that cell is NOT already set */
    if( !( bit_mask & 
    ( 1<<hdrsrchset.subaset[subaset_idx].aset[aset_idx]->aset.rpc_cell_idx ) ) )
    {
      /* Increment the number of unique cells count */
      ++num_cells_in_subaset;

      /* Set the bit_position, so it won't be counted again */
      bit_mask |= 
       (1<<(hdrsrchset.subaset[subaset_idx].aset[aset_idx]->aset.rpc_cell_idx));
    }
    /* else the cell idx is already counted */
  }

  return num_cells_in_subaset;

} /* hdrsrchutil_get_num_unique_cells_in_aset */
#else
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_NUM_UNIQUE_CELLS_IN_ASET

DESCRIPTION
  This function is used to find the number of unique cells in the active set.

DEPENDENCIES
  None

LIMITATIONS
  The rpc_cell_idx can only be from 0 to 31. 

RETURN VALUE
  The number of unique cells in ASET.  

SIDE EFFECTS
  None

===========================================================================*/

uint8 hdrsrchutil_get_num_unique_cells_in_aset (void)
{

  uint8 aset_idx;               /* aset index */
  uint8 num_cells_in_aset = 0;  /* Number of unique cells in an active set */
  uint32 bit_mask = 0;          /* Bit mask with bit_position corresponding to 
                                   cell idx set if that cell is part of ASET */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* For each one of the cell indices in the ASET... */
  for (aset_idx = 0; (aset_idx < hdrsrchset.aset_cnt); ++aset_idx)
  {
    /* If the bit position corresponding to that cell is NOT already set */
    if (!(bit_mask & (1 << hdrsrchset.aset[aset_idx]->aset.rpc_cell_idx)))
    {
      /* Increment the number of unique cells count */
      ++num_cells_in_aset;

      /* Set the bit_position, so it won't be counted again */
      bit_mask |= (1 << hdrsrchset.aset[aset_idx]->aset.rpc_cell_idx);
    }
    /* else the cell idx is already counted */
  }

  return num_cells_in_aset;

} /* hdrsrchutil_get_num_unique_cells_in_aset */
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_AGC_RTCOFFET

DESCRIPTION
  This function is used to calculate AGC RTCoffset 
  to be used in new fast AGC acquisition algorithm.

  REVB idle state is the same as in revA. Using the subaset[0].aset[0]

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  The AGC RTCoffset to be used in new fast AGC acquisition algorithm.

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchutil_get_agc_rtcoffset (void)
{
  uint32                  pilot_center;
    /* Center position of the current ASET pilot 
       adjusted for AGC measurements */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_REVB
  pilot_center = (uint16) 
      (hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->win_cenx2);
    /* Retrieve the current ASET window center position */
#else
  pilot_center = (uint16) (hdrsrchset.aset[0]->win_cenx2);
    /* Retrieve the current ASET window center position */
#endif /* FEATURE_HDR_REVB */

  pilot_center = (( pilot_center*4 ) & 0x3ffff );
    /* Calculate pilot position for AGC measurements. We skew the center
       by 288 chips so that we allow the LNA gain state and DC offsets
       to settle. */
#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Fast AGC Acq: WinCenx2= %d, AGC RTCOffsetx8 = %d ",
                  (hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->win_cenx2),
                  pilot_center );
#else
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Fast AGC Acq: WinCenx2= %d, AGC RTCOffsetx8 = %d ",
                  (hdrsrchset.aset[0]->win_cenx2),

#endif /* FEATURE_HDR_REVB */

  return pilot_center;

} /* hdrsrchutil_get_agc_rtcoffset */

#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ENABLE_MISSED_SCC_DETECTION

DESCRIPTION
  Enables missed sync CC detection. Will clear pending missed scc
  interrupts as parameter indicates.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrsrchutil_enable_missed_scc_detection( boolean clr_pending_int )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrrx_enable_missed_scc_detection( clr_pending_int );

} /* hdrsrchutil_enable_missed_scc_detection() */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHUTIL_READ_RTC

DESCRIPTION
  The function issues the rtc read command to firmware and latches the RTC 
  count and RTC RXTX compare count from the firmware interfaces.

DEPENDENCIES
  DSP clock should be enabled.

RETURN VALUE
  None

SIDE EFFECTS
  The function should be called when RTC is off and hdr firmware is on

===========================================================================*/
void hdrsrchutil_read_rtc
( 
  uint32*    rtc_cnt,
    /* Stored RTC value */

  uint32*    rxtx_cmp_cnt
    /* HDRRXTX compare cnt */
)
{  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* TBD */
  /* SW will read RTC from HW directly instead of from FW */
#if 0
  HDRMPC_WAIT_MPC_CMD_CLEAR( wait_cnt0 );
    /* Wait for MPC command clearance */

  HDRMPC_RD_RTC_CMD();
    /* Issue the READ command to mdsp to latch both the rtc cnt and txrx 
     * compare time */

#ifdef T_POSEIDON2
  HWIO_MDSP_UP_IRQ_VECTOR_OUT( 0x1 << HDRMPC_CMD_ISR );
    /* Set the mdsp ISR to make the mdsp latch the command */
#else
  HWIO_MICRO_TO_MDSP_IRQ_VECTOR_OUT( 0x1 << HDRMPC_CMD_ISR );
    /* Set the mdsp ISR to make the mdsp latch the command */
#endif /* T_POSEIDON2 */

  HDRMPC_WAIT_MPC_CMD_CLEAR( wait_cnt1 );
    /* Polling the cmd status register */

  /* Read the RTC cnt and compare cnt */
  *rtc_cnt = HDRMPC_GET_RTC_CNT();
  *rxtx_cmp_cnt = HDRMPC_GET_RXTX_CMP_CNT();
#endif /* if 0 */
 
} /* void hdrsrchutil_read_rtc( void ) */


/* EJECT */
#ifdef FEATURE_XO
/*==========================================================================

FUNCTION     : HDRSRCHACQ_CONVERT_PPM_TO_HZ 

DESCRIPTION  : Convert PPM (Parts Per Million) to Hz
   
   rfm_get_rx_carrier_freq() return value has KHz LSB
   So freq_offset_hz
       = ppm * rx_carrier_freq_khz * 1000 (Convert to freq to hz)
       = part/1000000 * rx_carrier_freq_khz * 1000 (ppm == part/1000000)
       = part * rx_carrier_freq_khz / 1000 (Simple reduction)
   Need to divide by 4.7 hz, the frequency resolution of the searcher4 rotator,
   where 1/4.7 = 109/2^9.
   So freq_offset-hz = ( 109 * (part * rx_carrier_freq_khz / 1000) ) >> 9

   Note: Return value in int16 since we need in that type for freq_offset

INPUT : ppm      - parts per million
        band     - Band class
        chan_num - Channel Number

DEPENDENCIES : None

RETURN VALUE : ppm convert to hz

SIDE EFFECTS :

==========================================================================*/

int16 hdrsrchacq_convert_ppm_to_hz
(
  int16                   ppm,
    /* Parts Per Million */

  sys_band_class_e_type   band,
    /* Band class - 0, 1, etc */

  sys_channel_num_type    chan_num
    /* CDMA Channel Number */
)
{
  int32                   freq_offset;

  sys_channel_type        band_chan;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  band_chan.band            = band;
  band_chan.chan_num        = chan_num;

  /* uint32 is returned */
  freq_offset =
    (int16)( rfm_cdma_get_rx_carrier_freq( band_chan ) /
             1000 );

  freq_offset *= 109;

  freq_offset = freq_offset >> 9;

  /* Note: Since ppm can be a negative number, and we are converting from
     int32 to int16, this step must be last, otherwise we may lose sign
     extension, i.e., negative int32 to int16 will lose sign extension. */
  return ((int16)freq_offset) * ppm;                                     

} /* hdrsrchacq_convert_ppm_to_hz */
#endif /* FEATURE_XO */



/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROCESS_PILOT_MEAS_REQ

DESCRIPTION
  Process the pilot meas request command from protocols.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_process_pilot_meas_req
(
  const hdrsrch_rup_req_params_type  *params
    /* RUP Request parameters */
)
{
 
  /* We will handle pilot measurement request same as RUP request. 
     So copy pilot measurement request paramenters to hdrsrch_rup_req */

  hdrsrchutil_copy_rup_req_params(params);

  hdrsrchofs_register_rup_req( hdrsrch_rup_req.channel,
                               hdrsrch_rup_req.system_type,
                               TRUE );
    /* register the RUP request for pilot measurement with OFS state */
 
} /* hdrsrchutil_process_pilot_meas_req() */

/*===========================================================================

FUNCTION HDRSRCHUTIL_PILOT_MEAS_REQ_CMD

DESCRIPTION
  This function passes pilot measurement request command to searcher.

DEPENDENCIES
  None

PARAMETERS
  params - parameters for the pilot measurement request command.

RETURN VALUE
  Status of the command.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchutil_pilot_meas_req_cmd
(
  const hdrsrch_rup_req_params_type     *params
    /* pilot measurement request parameters */
)
{
  errno_enum_type                       status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "PILOT_MEAS_REQ_CMD" );

  if ( hdrsrchofs_pilot_meas_pending() == TRUE )
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_HIGH,
      "HDR PILOT MEAS: Discarded new pilot meas request, State ID= %d ",
      hdrsrch_state_id );
    
    status = E_FAILURE;
      /* Indicate that we will not serve another pilot measurement request
         before the pending one is finished */
  }
  else
  {
    hdrsrchutil_process_pilot_meas_req( params );
      /* Process command parameters */    
  }
  
  return ( status );
    /* Command has been completed */

} /* hdrsrchutil_pilot_meas_req_cmd( ) */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GENERATE_DIRECTED_PILOT_MEAS_SET

DESCRIPTION
  Generate the RUP sectors from pilots in pilot measurement request.

DEPENDENCIES
  None

PARAMETERS
  systm - type of the system for the search. 

RETURN VALUE
  
  
SIDE EFFECTS
  None
  
=========================================================================*/

boolean hdrsrchutil_generate_directed_pilot_meas_set
(
  hdrsrch_system_enum_type    systm
    /* system type for the search */
)
{
  sys_channel_type            chan;
    /* only search sectors on this channel */
        
  hdrsrch_sect_struct_type    *sect;
    /* sector to be programmed in the list entry */
           
  boolean                     last_rupset;
    /* flag whether the generated RUP set is the last one for the pilot measure
       request. i.e we are done generating RUP sets for the pending request.
    */
      

  uint16                      i;
    /* loop variable */
            
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  chan = hdrsrch_rup_req.channel ;
    /* RUP search channel for brevity */
        
  hdrsrchset.rupset_cnt = 0;
    /* reset RUP set */
  
  if ( hdrsrch_rup_req.sector_count > HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE )
  {
    hdrsrch_rup_req.sector_count = HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE;
      /* Truncate the number of pilots to search */
      
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,"HDR PILOT MEAS: Too many pilots to search = %d > %d, truncated. ",
                    hdrsrch_rup_req.sector_count, HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE );      
  }
  
  /* Create sectors in RUPSET from pilots specified in RUP request */   
  for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
  {
    
    /* Get the RUP sector, create if necessary */
    sect = hdrsrchset_get_pilot_meas_rup_sect(
               hdrsrch_rup_req.sector_info[i].pilot_pn, 
               chan,
               systm );
    
    if ( sect != NULL )
    {
      if ( hdrsrch_rup_req.window_size_included == TRUE )
      {
        sect->nset.win_size = hdrsrch_rup_req.sector_info[i].window_size;
      }

      if ( hdrsrch_rup_req.window_offset_included == TRUE )
      {
        sect->nset.win_offset = hdrsrch_rup_req.sector_info[i].window_offset;
      }
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                  "HDR PILOT MEAS: Generated directed search set,pn=%d,win_size=%d win_offset=%d ",
                  sect->pn_offset, sect->nset.win_size, sect->nset.win_offset );
    }  
  }

  last_rupset = TRUE;
    /* There can be at most 16 sectors included in RUP request 
       so we all should fit in RUP set in in one pass */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "HDR PILOT MEAS: Generated directed search set, Sectors= %d ",
                  hdrsrchset.rupset_cnt );
                            
  return ( last_rupset );
  
} /* hdrsrchutil_generate_directed_pilot_meas_set() */

/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GENERATE_PILOT_MEAS_SET

DESCRIPTION
  Generate the search sectors from pilot measurement request.

DEPENDENCIES
  None

PARAMETERS
  systm - type of the system for the pilot measurement search. 

RETURN VALUE
  Whether the generated search set is the last one for the pilot
  measurement request

SIDE EFFECTS
  None
  
=========================================================================*/

boolean hdrsrchutil_generate_pilot_meas_search_set
(
  hdrsrch_system_enum_type    systm
    /* system type for the pilot measurement search */
)
{
  boolean                     last_rupset;
    /* flag whether the generated RUP set is the last one for the RUP request
      i.e we are done generating RUP sets for the pending RUP request. */
      
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_rup_req.sector_count != 0 )
  {
     last_rupset = hdrsrchutil_generate_directed_pilot_meas_set( systm );
  }
  else
  {
    /* TBD: should search all PN in one channel */

    last_rupset= TRUE;
  }

  hdrsrchofs_set_rupset_srch_index( 0 );
    /* reset index into RUP search set */

  return last_rupset;
}

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/*===========================================================================

FUNCTION HDRSRCHUTIL_PROCESS_OFS_SEARCH_DUMP_FOR_PILOT_MEAS

DESCRIPTION
  Processes the search results for the just completed OFS search.
  This search result is for pilot measurement request

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Clears lost dump timer.

===========================================================================*/

void hdrsrchutil_process_ofs_search_dump_for_pilot_meas( void )
{

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* Stop the timer, clear its signal, and remove from wait mask */

  HDR_ASSERT( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
    /* KW fix : Error fatal if the list_cnt is greater than maximum  */

  hdrsrchset_calc_tot_eng( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
    /* Filter search energies for the searched pilots into the
       respective search pilot structures */

  hdrsrchset_identify_strongest_peak_for_rupset(hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt);
    /* Identify the pn peak position for strongest path. This is needed for LTE to
       maintain cdma system time in LTE to DO pilot measurement case 
    */
  hdrsrchset_identify_earliest_peak( hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt );
    /* Identifies the earliest peak with sufficient energy and stores
       the position of the peak for use in the RUP msg */  

} /* hdrsrchutil_process_ofs_search_dump_for_pilot_meas( ) */


/*=========================================================================

FUNCTION     : HDRSRCHUTIL_LTE_PILOT_MEAS_STORE_SEARCH_PARAMS

DESCRIPTION
  Copy search related parameters to the RUP structure.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_lte_pilot_meas_store_search_params
(
  const hdrsrch_lte_pilot_meas_params_type  *params
    /* RUP Request parameters */
)
{
  uint16                             i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Copy search related parameters to RUP structure */

  hdrsrch_rup_req.channel_included = TRUE;
  
  /* store system type and channel */
  hdrsrch_rup_req.system_type = HDRSRCH_SYSTYPE_DO;
  hdrsrch_rup_req.channel.chan_num =
    params->lte_rfm_param.target.channel.chan_num;
  hdrsrch_rup_req.channel.band =
    params->lte_rfm_param.target.channel.band;

  hdrsrch_rup_req.sector_count = params->sector_count;
    /* store number of pilots in list */

  hdrsrch_rup_req.pilot_rpt_cb = NULL;
    /* register callback function which is NULL in lte pilot measurement */

  hdrsrch_rup_req.window_size_included = TRUE;
    /* window sizes is always specified in lte pilot measurement */

  hdrsrch_rup_req.window_offset_included = FALSE;
    /* window offset is not specified in lte pilot measurement */

  /* store sector PNs, window size*/
  for ( i = 0 ; i < hdrsrch_rup_req.sector_count ; i++ )
  {
    hdrsrch_rup_req.sector_info[i].pilot_pn = params->pn_list[i];
    hdrsrch_rup_req.sector_info[i].window_size = params->window_size;
  }
} /* hdrsrchutil_lte_pilot_meas_store_search_params() */


/*=========================================================================

FUNCTION     : HDRSRCHUTIL_PROCESS_LTE_PILOT_MEAS_REQ

DESCRIPTION
  Process the LTE pilot meas request command .

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
=========================================================================*/

void hdrsrchutil_process_lte_pilot_meas_req
(
  const hdrsrch_lte_pilot_meas_params_type *params
    /* LTE Pilot Measurement Request parameters */
)
{
  /* Copy parameters about gap/measurement config */
  hdrsrchofs_meas_store_config_params( params );

  /* We will handle search part of lte_pilot_meas_req  same as 
     RUP request. So copy paramenters related search to hdrsrch_rup_req */
  hdrsrchutil_lte_pilot_meas_store_search_params(params);

  hdrsrchofs_register_rup_req( hdrsrch_rup_req.channel,
                               hdrsrch_rup_req.system_type,
                               TRUE );
    /* register the RUP request for lte pilot measurement with OFS state */

} /* hdrsrchutil_process_lte_pilot_meas_req() */

/*===========================================================================

FUNCTION HDRSRCHUTIL_LTE_PILOT_MEAS_REQ_CMD

DESCRIPTION
  This function passes pilot measurement request command to searcher.

DEPENDENCIES
  None

PARAMETERS
  params - parameters for the pilot measurement request command.

RETURN VALUE
  Status of the command.

SIDE EFFECTS
  None

===========================================================================*/

hdrsrch_lte_pilot_meas_status_type hdrsrchutil_lte_pilot_meas_req_cmd
(
  const hdrsrch_lte_pilot_meas_params_type     *params
    /* pilot measurement request parameters */
)
{
  hdrsrch_lte_pilot_meas_status_type    status = PILOT_MEAS_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "LTE_PILOT_MEAS_REQ_CMD" );

  if ( hdrsrchofs_pilot_meas_pending() == TRUE )
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_HIGH,
      "IRAT: HDR Discarded lte pilot meas request, State ID= %d ",
      hdrsrch_state_id );
    
    status = PILOT_MEAS_PENDING_REQ_EXIST;
      /* Indicate that we will not serve another pilot measurement request
         before the pending one is finished */
  }
  else
  {
    if ( ( params->sector_count == 0 ) || 
         ( params->sector_count > HDRSRCH_RUP_SECTOR_LIST_MAX_SIZE ) ) 
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
               "IRAT Meas: error: sector count is 0 or too big sector_count=%d",
                params->sector_count );
      status = PILOT_MEAS_WRONG_PARAMS;
        /* Return failure if no pilot in search list */
    }
    else if ( params->gap_start == HDRSRCH_PILOT_MEAS_START_IMMEDIATE &&
              params->online_srch == FALSE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
               "IRAT Meas: error: IDLE meas does not support offline search" );
      status = PILOT_MEAS_WRONG_PARAMS;
        /* Return failure if no pilot in search list */
    }
    else if ( params->gap_start != HDRSRCH_PILOT_MEAS_START_IMMEDIATE &&
              params->online_srch == TRUE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
               "IRAT Meas: error: CONN meas does not support online search" );
      status = PILOT_MEAS_WRONG_PARAMS;
        /* Return failure if no pilot in search list */
    }
    else
    {
      hdrsrchutil_process_lte_pilot_meas_req( params );
        /* Process command parameters */    
    }
  }
  
  return ( status );
    /* Command has been completed */

} /* hdrsrchutil_lte_pilot_meas_req_cmd( ) */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

#ifdef FEATURE_HDR_REVB
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_REMOVE_SUBASET_FROM_ASET_TO_RSET

DESCRIPTION

DEPENDENCIES
  Connected State

PARAMETERS
  subaset_idx  - the subaset idx in the ASET

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_remove_subaset_from_aset_to_rset
( 
  uint8 subaset_idx 
)
{
  int8 i;
    /* Loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* Drop all the sectors from this subaset to RSET */
  for( i=hdrsrchset.subaset[subaset_idx].aset_cnt-1; i>=0; i-- )
  {
    hdrsrchset_drop_from_aset_to_rset( hdrsrchset.subaset[subaset_idx].aset[i] );
  }

} /* void hdrsrchutil_remove_subaset_from_aset_to_rset ( subaset_idx ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_REMOVE_SUBASET_FROM_ASET

DESCRIPTION

DEPENDENCIES
  Connected State

PARAMETERS
  subaset_list    - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_remove_subaset_from_aset
( 
  uint8 subaset_idx 
)
{
  int8 i;
    /* Loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  for( i=hdrsrchset.subaset[subaset_idx].aset_cnt-1; i>=0; i-- )
  {
    /* Drop all the sectors from this subaset */

    hdrsrchset_drop_from_aset( hdrsrchset.subaset[subaset_idx].aset[i] ); 
      /*  Drop the sector from aset */
  }
} /* void hdrsrchutil_remove_subaset_from_aset( subaset_idx ) */

/*===========================================================================

FUNCTION        HDRSRCHUTIL_GET_STRONGEST_ASP_IDX

DESCRIPTION     This function returns the strongest ASP index.

DEPENDENCIES    
  None

PARAMETERS
  c    - Demod carrier

RETURN VALUE    
  The strongest ASP index

SIDE EFFECTS    
  None
===========================================================================*/

uint8 hdrsrchutil_get_strongest_asp_idx
(
  int8                       sa
    /* Subaset index */
)
{
  int8                       a;
    /* Aset index */

  uint8                      best_sect = 0;
    /* best sector index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchset.subaset[ sa ].aset_cnt > 0 )
  {
    for ( a = 1; (a < hdrsrchset.subaset[ sa ].aset_cnt) && (a < (HDRSRCH_MAX_SUBASET_SIZE+1)); a++ )
    {
      if ( hdrsrchset.subaset[ sa ].aset[ a ]->total_eng >
           hdrsrchset.subaset[ sa ].aset[ best_sect ]->total_eng )
      {
        best_sect = a;
      }
    } /* for */
  } /* if */
  else
  {
    ERR_FATAL( "ASET is empty", 0, 0, 0 );
  }

  return hdrsrchset.subaset[ sa ].aset[ best_sect ]->aset.asp_idx;

}  /* hdrsrchutil_get_strongest_asp_idx */


/*===========================================================================

FUNCTION HDRSRCHUTIL_HANDOFF_OVERRIDE_PENDING

DESCRIPTION
  This function checks whether handoff override pending for any subaset.

DEPENDENCIES
  Connected State

PARAMETERS
  None
  
RETURN VALUE
  TRUE  - Handoff override pending.
  FALSE - No handoff override pending.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchutil_handoff_override_pending( void )
{

  uint8                      sa;
    /* Subaset index */

  boolean                    pending = FALSE;
    /* Whetehr handoff override is pending */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa ++ ) 
  {
    if ( hdrsrchset.subaset[ sa ].tc_ho_override == TRUE) 
    {
      pending = TRUE;
        /* Handoff override is ending*/

      break;
    }
  }

  return pending;

}  /* hdrsrchutil_handoff_override_pending */


/*===========================================================================

FUNCTION HDRSRCHUTIL_CLEAR_HANDOFF_OVERRIDE

DESCRIPTION
  This function clears handoff override for all subasets.

DEPENDENCIES
  Connected State

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_clear_handoff_override( void )
{

  uint8                      sa;
    /* Subaset index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa ++ ) 
  {
    hdrsrchset.subaset[ sa ].tc_ho_override = FALSE;
    hdrsrchset.subaset[ sa ].tune_away_ho_override = FALSE;
  }

}  /* hdrsrchutil_clear_handoff_override */


/*===========================================================================

FUNCTION HDRSRCHUTIL_PN_EXISTS

DESCRIPTION
  This function checks whether the a sector with the pn offset exists 
  in the subaset .

DEPENDENCIES
  Connected State

PARAMETERS
  demod_idx       - Demod carrier index
  
RETURN VALUE
  Sector pointer

SIDE EFFECTS
  None

===========================================================================*/

static boolean hdrsrchutil_pn_exists
(
   sys_pilot_pn_type         pn,
    /* PN offset */

  const hdrsrch_subactive_set_type *subaset
    /* Subaset */
)
{

  uint8                      a;
    /* Aset index */

  boolean                    exist = FALSE;
    /* The PN offset exists or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( subaset != NULL ) 
  {
    for ( a = 0; a < subaset->num_aset_pilots; a++ )
    {
      if ( pn == subaset->aset_list[ a ].pilot_pn )
      {
        exist = TRUE;
          /* The PN offset exist in the subaset */
        break;
      }
    } /* for */
  }  /* if */

  return exist;

}  /* hdrsrchutil_pn_exists */


/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_BEST_ASP

DESCRIPTION
  This function looks for the best ASP and return the pointer to the sector.

DEPENDENCIES
  Connected State

PARAMETERS
  demod_idx       - Demod carrier index
  
RETURN VALUE
  Sector pointer

SIDE EFFECTS
  None

===========================================================================*/

static hdrsrch_sect_struct_type *hdrsrchutil_get_best_asp
(
   hdrsrch_demod_idx_type   demod_idx
    /* Demod index */
)
{

  uint8                      sa;
    /* Subaset index */

  hdrsrch_sect_struct_type   *sect = NULL;
    /* Sector pointer */

  uint8                      basp_idx;
    /* Best ASP index */

  uint8                      a;
    /* aset index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
  {
    if ( hdrsrchset.subaset[ sa ].demod_idx == demod_idx )
    {
      basp_idx = HDRDRC_GET_VAR_BEST_ASP_PRED( demod_idx );
        /* Get the best ASP */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "demod_idx %d basp_idx %d",
                      demod_idx, basp_idx );

      for ( a = 0; a < hdrsrchset.subaset[ sa ].aset_cnt; a++ )
      {
        if ( hdrsrchset.subaset[ sa ].aset[ a ]->aset.asp_idx == basp_idx )
        {
          sect =  hdrsrchset.subaset[ sa ].aset[ a ];
            /* Get the sector pointer */

          break;
        }
      }

      break;
    }  /* if */
  }  /* for */

  return sect;

}  /*hdrsrchutil_get_best_asp */
/*===========================================================================

FUNCTION HDRSRCHUTIL_DETERMINE_HANDOFF_OVERRIDE

DESCRIPTION
  This function determines whether handoff override is necessary. 
  The condition for handoff override for one demod carrier:

    - on a different channel
    - or, the best ASP is not in the new subaset.


DEPENDENCIES
  Connected State

PARAMETERS
  subaset_list    - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void hdrsrchutil_determine_handoff_override
(
  const hdrsrch_subactive_set_type*  subaset_list,
    /* Array of pilots to put in active set */

  int                                num_subasets,
    /* Number of pilots to put into active set */

  boolean                            *ho_override
)
{

  int8                     sa;
    /* Loop index for the new subasets */

  hdrsrch_demod_idx_type   c;
    /* Demod carrier index */

  hdrsrch_sect_struct_type *sect;
    /* Sector for the best ASP */

  const hdrsrch_subactive_set_type *subaset;
    /* Subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT(( ( subaset_list != NULL ) && ( num_subasets != 0 ) ) &&
         ( ho_override != NULL ));

  for ( c = 0; c < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; c++ ) 
  {
    ho_override[ c ] = FALSE;
      /* By default, don't need handoff override */
  }

  for ( sa = 0; sa < num_subasets; sa++) 
  {
    subaset = &subaset_list[ sa ];

    ASSERT( subaset != NULL );

    sect = hdrsrchutil_get_best_asp( subaset->demod_idx );
      /* Get the best ASP */

    if ( sect != NULL )
    {
      if ( !SYS_CHAN_CMP_EQUAL( subaset_list[ sa ].channel, 
                                sect->chan ) )
      {
        /* Different channel */

        ho_override[ subaset->demod_idx ] = TRUE;
          /* Need handoff override */
      }  /* if !SYS_CHAN_CMP_EQUAL */
      else
      {
        /* The same channel */
        if ( hdrsrchutil_pn_exists( sect->pn_offset, subaset ) == FALSE )
        {
          /* The best ASP does not exist in the new subaset */

          ho_override[ subaset->demod_idx ] = TRUE;
            /* Need handoff override */
        }
      }  /* if !SYS_CHAN_CMP_EQUAL */

    }  /* if ( sect != NULL ) */
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "No best asp on demod_idx %d maybe disabled",
                  subaset->demod_idx );

      ho_override[ subaset->demod_idx ] = TRUE;
        /* Need handoff override */
    } /* if ( sect != NULL )  */
  }  /* for */

} /* hdrsrchutil_determine_handoff_override */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_CC_CARRIER_IDX

DESCRIPTION
  Search subaset and return the subaset index of the CC carrier.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  CC carrier subaset index, or -1 if one does not exist.

SIDE EFFECTS
  None

===========================================================================*/

static int8 hdrsrchutil_get_cc_carrier_idx( void )
{
  int8              subaset_idx;
    /* Subaset Index loop index */

  int8              cc_carrier_subaset_idx = -1;
    /* CC carrier subaset index
       Default is -1 so it is up to the caller to do error checking
       before using this index to reference an array index. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( subaset_idx = 0;
       subaset_idx < hdrsrchset.subaset_cnt;
      subaset_idx++)
  {
    if( hdrsrchset.subaset[subaset_idx].cc_carrier == TRUE )
    {
      cc_carrier_subaset_idx = subaset_idx;
        /* Save the CC carrier index. */

      break;
    }
  } /* for subaset_idx...*/

  return cc_carrier_subaset_idx;

} /* hdrsrchutil_get_cc_carrier_idx */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_DROP_FORMER_ASET_SECTS

DESCRIPTION
  The dropping should follow the order which nonreportable subaset first
  then reportable aset.

DEPENDENCIES
  Connected State

PARAMETERS
  tca_subaset_list    - Array of pilots to put into ASET
  tca_num_subasets    - Number of pilots in the array

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchutil_drop_former_aset_sects
(
  const hdrsrch_subactive_set_type*  tca_subaset_list,
    /* Array of pilots to put in active set */

  int                                tca_num_subasets
    /* Number of pilots to put into active set */
)
{
  int8                     old_subaset_idx;
    /* Index into old/existing subaset hdrsrchset.subaset[] */

  int8                     tca_subaset_idx;
    /* Index into "new" tca_subaset_list[] parameter */

  int8                     old_aset_idx;
    /* Index into old/existing ASET
       hdrsrchset.subaset[old_subaset_idx].aset[] */

  int8                     tca_aset_idx;
    /* Index into "new" tca_subaset_list[tca_subaset_idx].aset_list[] parameter */

  hdrsrch_demod_idx_type   demod_idx;
    /* Demod index */

  hdrsrch_sect_struct_type *sect;
    /* Temporary sector point */

  boolean                  transitions=TRUE;
    /* Sect match flag */

  int8                     cc_carrier_idx = -1;
    /* Index of CC carrier, if any.  -1 if none. */

  boolean                  drop_cc_carrier[HDRSRCH_MAX_SUBASET_SIZE];
    /* Flags to indicate whether to drop CC carrier from ASET or not. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* To cover the potential transient case of no ASET's, i.e., deleting
     all ASET's before adding new ASET's in the disjoint TCA cmd processing
     case, save the last ASET[0] for use when a request from upper layer
     comes during transient condition.
     Note: This is a temporary fix until at least one ASET is kept during
     ASET updates. */
  if ( ( hdrsrchset.subaset_cnt != 0) &&
       ( hdrsrchset.subaset[0].aset_cnt != 0 ) )
  {
    hdrsrchset.last_subaset0_aset0 = *hdrsrchset.subaset[0].aset[0];
  }

  /* Handle the drop of non reportable subactive set first; The reason for
     handling the non reportable subactive set first is to avoid: the hard
     handoff subactive set is reportable and we drop it first, if the rest
     subactive set is non reportable, then we might end up adding the entire
     ASET to EMPTYSET. The idea here is to process the non_reportable subactive 
     set first before the processing of the reportable subaset. */

  for( old_subaset_idx=hdrsrchset.subaset_cnt-1;
       old_subaset_idx>=0;
       old_subaset_idx-- )
  {
    demod_idx = hdrsrchset.subaset[old_subaset_idx].demod_idx;

    if( hdrsrchset.subaset[old_subaset_idx].non_reportable == TRUE )
    {
      /* Find the matching subactive set through the demod idx */

      for( tca_subaset_idx=0; tca_subaset_idx<tca_num_subasets; tca_subaset_idx++ )
      {
        if( demod_idx == tca_subaset_list[tca_subaset_idx].demod_idx )
        {
          /* If the demod index is used in the incoming TCA demod assignment */
          if( !SYS_CHAN_CMP_EQUAL( tca_subaset_list[tca_subaset_idx].channel,
                                hdrsrchset.subaset[old_subaset_idx].channel ) )
          {
            /* Hard handoff on this subaset. Remove the entire subasets to
               RSET as it is non-reportable */

            hdrsrchutil_remove_subaset_from_aset_to_rset( old_subaset_idx );

            break;
              /* Break the for tca_subaset_idx loop */
          }
          else
          {
            /* Not hard handoff on the subaset, individually process each of
               the subaset sector. Look for aset's pilot PN in new TCA
               subaset list.  If not found, drop to RSET. */

            for( old_aset_idx=hdrsrchset.subaset[old_subaset_idx].aset_cnt-1;
                 old_aset_idx>=0;
                 old_aset_idx-- )
            {
              sect = hdrsrchset.subaset[old_subaset_idx].aset[old_aset_idx];
                /* Convenience pointer */

              for( tca_aset_idx=0;
                   tca_aset_idx<tca_subaset_list[tca_subaset_idx].num_aset_pilots;
                   tca_aset_idx++ )
              {
                if( sect->pn_offset ==
                    tca_subaset_list[tca_subaset_idx].aset_list[tca_aset_idx].pilot_pn )
                {
                  break;
                }
              }

              /* Searched all of new subaset's aset pilots yet? */
              if( tca_aset_idx == tca_subaset_list[tca_subaset_idx].num_aset_pilots )
              {
                hdrsrchset_drop_from_aset_to_rset( sect );
                  /* Sector's pilot PN not found in new sector list. 
                     Deassign any fingers and drop from ASET to RSET */
              } /* if tca_subaset_idx */
            } /* for old_aset_idx */
          } /* if !SYS_CHAN_CMP_EQUAL */

          break;
            /* Break tca_subaset_idx loop, continue to process next subaset */
        } /* if demod_idx */
      } /* for tca_subaset_idx... */

      if( tca_subaset_idx == tca_num_subasets )
      {
        /* If the subaset has a demod index which is not used by the new TCA,
           then the subaset is dropped to RSET. */

        hdrsrchutil_remove_subaset_from_aset_to_rset( old_subaset_idx );
      } /* if tca_subaset_idx == tca_num_subasets */
    } /* if non-reportable */
  } /* for old_subaset_idx... */
  
  hdrsrchutil_integrate_aset();
    /* Integrate the ASET in case an entire subactive set is dropped. */

  /* Find the index of cc carrier in the ASET.
     Note: This has to be done after the integrate function since the
     subaset indices might change. */
  cc_carrier_idx = hdrsrchutil_get_cc_carrier_idx();

  /* From this point, handle the reportable subactive set. We should have
     avoided the worst case because at this moment, all the nonreportable
     subactive set sectors are purged, i.e., only reportable subasets exist. */

  for ( old_aset_idx=0; old_aset_idx<HDRSRCH_MAX_SUBASET_SIZE; old_aset_idx++ )
  {
    drop_cc_carrier[old_aset_idx] = FALSE;
      /* Init drop carrier flags. */
  }

  for( old_subaset_idx=hdrsrchset.subaset_cnt-1;
       old_subaset_idx>=0;
       old_subaset_idx-- )
  {
    demod_idx = hdrsrchset.subaset[old_subaset_idx].demod_idx;

    for( tca_subaset_idx=0; tca_subaset_idx< tca_num_subasets; tca_subaset_idx++)
    {
      if( demod_idx == tca_subaset_list[tca_subaset_idx].demod_idx )
      {
        /* Locate the replacing subaset through demod index */

        for( old_aset_idx=hdrsrchset.subaset[old_subaset_idx].aset_cnt-1;
             old_aset_idx>=0;
             old_aset_idx-- )
        {
          sect = hdrsrchset.subaset[old_subaset_idx].aset[old_aset_idx];
            /* Convenience pointer */

          /* Check if new subaset pilot is in existing subaset.  If it exists,
             then no need to drop it to C/RSET.  If it does not exist, then
             drop existing pilot. */
          for( tca_aset_idx=0;
               (tca_aset_idx<tca_subaset_list[tca_subaset_idx].num_aset_pilots)
               && ( tca_aset_idx < HDRSRCH_ASET_MAX_SIZE);
               tca_aset_idx++)
          {
            if( ( sect->pn_offset ==
                  tca_subaset_list[tca_subaset_idx].aset_list[tca_aset_idx].pilot_pn ) &&
                 SYS_CHAN_CMP_EQUAL( sect->chan, tca_subaset_list[tca_subaset_idx].channel ) )
            {
              /* Don't drop existing pilot since it matches the new one. */
              break;
            }
          } /* for tca_aset_idx */

          /* Was the new pilot found in existing subaset? */
          if ( tca_aset_idx == tca_subaset_list[tca_subaset_idx].num_aset_pilots )
          {
            /* Pilot was not found, so drop existing pilot.  Remember that
               all existing subasets are reportable at this point.
               Add reportable carrier to C/N/RSET.  If there is more than one
               reportable carrier, prefer CC carrier. Take care of CC carrier
               last since hdrsrchset_drop_from_aset() drops the input sect
               if a buddy is found. */
            if ( ( old_subaset_idx != cc_carrier_idx ) &&
                 ( cc_carrier_idx != -1 ) )
            {
              /* Dropping non-cc_carrier pilot.  If it is a buddy of CC carrier,
                 then this results in dropping to RSET, i.e., preferring CC
                 carrier.  */
              hdrsrchset_drop_from_aset( sect );
                /* Drop to C/NSET or if a buddy (same PN and PGID) is found
                   then the drop sector to RSET, i.e., keep the last one */

              cc_carrier_idx = hdrsrchutil_get_cc_carrier_idx();
                /* Get CC carrier in case subaset integration was performed. */
            }
            else if ( old_subaset_idx == cc_carrier_idx )
            {
              drop_cc_carrier[old_aset_idx] = TRUE;
                /* Need to drop CC carrier, but we do this last because of the
                   way hdrsrchset_drop_from_aset() works, i.e., it drops input
                   sector to RSET if buddy is found. */
            }
          } /* if tca_aset_idx */
        } /* for old_aset_idx */

        break;
          /* Break for tca_subaset_idx...; continue to process next subaset */
      } /* if demod_idx */
    } /* for tca_subaset_idx */

    if ( tca_subaset_idx == tca_num_subasets )
    {
      /* If the subaset[old_subaset_idx] has a demod index which is not used
         by the new TCA, then the entire subaset is dropped now or dropping is
         delayed until the end if it is a CC Carrier. */

      ASSERT(old_subaset_idx < HDRSRCH_MAX_NUM_SUBASETS);
      if ( old_subaset_idx == cc_carrier_idx )
      {
        for( old_aset_idx=hdrsrchset.subaset[old_subaset_idx].aset_cnt-1;
             old_aset_idx>=0;
             old_aset_idx-- )
        {
          drop_cc_carrier[old_aset_idx] = TRUE;
          /* Need to drop CC carrier, but we do this last because of the
             way hdrsrchset_drop_from_aset() works, i.e., it drops input
             sector to RSET if buddy is found. */
      }
      }
      else
      {
      for( old_aset_idx=hdrsrchset.subaset[old_subaset_idx].aset_cnt-1;
           old_aset_idx>=0;
           old_aset_idx-- )
      {
        sect = hdrsrchset.subaset[old_subaset_idx].aset[old_aset_idx];
          /* Convenience pointer */

        hdrsrchset_drop_from_aset( sect );
          /* Drop the sector from ASET; it might adds to the CSET or RSET based
             pilot group management rules. */

        cc_carrier_idx = hdrsrchutil_get_cc_carrier_idx();
          /* Get CC carrier in case subaset integration was performed. */
      } /* for old_aset_idx */
      } /* else */
    } /* if tca_subaset_idx == tca_num_subasets */
  } /* for old_subaset_idx... */

  /* Check if we need to drop CC carrier.  At this point, all CC carrier buddys
     should be dropped to the RSET because of the way
     hdrsrchset_drop_from_aset() works.  So now CC Carrier will only drop to
     C/NSET, satisfying the requirement to prefer CC carrier if multiple
     reportable same pilots/PGID need to be dropped to C/NSET. */

  cc_carrier_idx = hdrsrchutil_get_cc_carrier_idx();
    /* Get CC carrier in case subaset integration was performed. */

  /* Sanity check */
  ASSERT(cc_carrier_idx < HDRSRCH_MAX_NUM_SUBASETS);

  if ( cc_carrier_idx != -1 ) 
  {
    for ( old_aset_idx=hdrsrchset.subaset[cc_carrier_idx].aset_cnt-1;
          old_aset_idx>=0;
          old_aset_idx-- )
    {
      if ( drop_cc_carrier[old_aset_idx] == TRUE )
      {
        hdrsrchset_drop_from_aset( hdrsrchset.subaset[cc_carrier_idx].aset[old_aset_idx] );
          /* Only drop to C/NSET, not RSET. */
      }
    } /* for */
  } /* if ( cc_carrier_idx != -1 ) */
  
  hdrsrchutil_integrate_aset();
    /* Integrate the ASET incase an entire subactive set is dropped. */

  return transitions;
} /* hdrsrchutil_drop_former_aset_sects */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_GET_PILOT_GROUP_SECT

DESCRIPTION  : This function returns matching pilot group and pn sector in
               given pilot set.  NULL is returned if a matching sector is
               not found.

DEPENDENCIES : None

RETURN VALUE : Pointer to sector if found, else NULL.

SIDE EFFECTS : None

=========================================================================*/
hdrsrch_sect_struct_type *hdrsrchutil_get_pilot_group_sect
(
  hdrsrch_sect_struct_type        *sect,
    /* sector to use for pilot group and pn matching */

  hdrsrch_set_enum_type           set
    /* Set type to search */

)
{
  hdrsrch_sect_struct_type        *found_sect = NULL;
    /* Pointer to matching sector, if found */

  uint8                           i, j;
    /* Local loop variables */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch (set)
  {
    case HDR_ASET:
    {
      /* Search through the entire subactive set for matching pilot group id
         and pn.  Exit loop if matching sector is found. */
      for( i=0; ( i < hdrsrchset.subaset_cnt ) && ( found_sect == NULL ); i++ )
      {
        for ( j = 0; j < hdrsrchset.subaset[i].aset_cnt; j++ )
        {
          if ( HDRSRCHSET_PILOT_GROUP_CMP( *hdrsrchset.subaset[i].aset[j],
                                                 *sect ) )
          {
            found_sect = hdrsrchset.subaset[i].aset[j];
              /* Found ASET sector.  Save pointer to return.*/
            break;
          }
        }
      }
      break;
    }

    case HDR_CSET:
    {
      /* Search through the entire CSET for matching pilot group id and pn */
      for ( i = 0; i < hdrsrchset.cset_cnt; i++ )
      {
        if ( HDRSRCHSET_PILOT_GROUP_CMP( *hdrsrchset.cset[i], *sect ) )
        {
          found_sect = hdrsrchset.cset[i];
            /* Found CSET sector.  Save pointer to return.*/
          break;
        }
      }
      break;
    }

    default:
      /* Do nothing */
      break;

  } /* switch (set) */

  return found_sect;

} /* hdrsrchutil_get_pilot_group_sect */


/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_UPDATE_NEW_ASET_WINDOW_CENTER

DESCRIPTION  : This function looks through the subaset and cset sectors
               for a buddy and assigns the found sector's window center
               to the new ASET sector.

DEPENDENCIES : None

INPUT        : None

RETURN VALUE : None

SIDE EFFECTS : Passes in sector has window center updated

=========================================================================*/

void hdrsrchutil_update_new_aset_window_center
(
  hdrsrch_sect_struct_type* sect_ptr
    /* pilot to be added to aset */
)
{

  hdrsrch_sect_struct_type           *pilot_group_sect;
    /* Sector pointer to matching pilot group sector from ASET or CSET */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (sect_ptr->set != HDR_CSET) 
  { 
    /* Update new ASET sector's window center if NOT transitioning from CSET.
       Check if sector has representation in ASET or CSET, i.e., pilot
       group and pn exist in ASET or CSET, otherwise sect must be
       transitioning from N/R. */
    if ( ( ( pilot_group_sect = 
               hdrsrchutil_get_pilot_group_sect( sect_ptr, HDR_ASET ) ) != NULL ) ||
         ( ( pilot_group_sect = 
               hdrsrchutil_get_pilot_group_sect( sect_ptr, HDR_CSET ) ) != NULL ) )
    {
      /* Found matching pilot group and pn in A/C SET */
      sect_ptr->win_cenx2 = pilot_group_sect->win_cenx2;
        /* Set to existing A/C SET pilot's window center. */
      HDR_MSG_SRCH_7( MSG_LEGACY_HIGH,
 "Set new ASET PN %d/Ch %d win_cen to PN %d/Ch %d/set %d gid=%d, win_cenx2=%d",
                      sect_ptr->pn_offset,
                      sect_ptr->chan.chan_num,
                      pilot_group_sect->pn_offset,
                      pilot_group_sect->chan.chan_num,
                      pilot_group_sect->set,
                      pilot_group_sect->pilot_gid,
                      pilot_group_sect->win_cenx2 );

    }
    else
    {
      /* any sector that is coming to ASET from N/R set should
         have its win_cenx2 initialized to MSTR */
      sect_ptr->win_cenx2 = (hdrsrchdrv.mstr_x8 + 2)/4;
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                      "Set new ASET PN %d/Ch %d Window Center to MSTR (%d)",
                      sect_ptr->pn_offset,
                      sect_ptr->chan.chan_num,
                      sect_ptr->win_cenx2);
    }
  };

} /* hdrsrchutil_update_new_aset_window_center */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ADD_TCA_PILOT_TO_SUBASET

DESCRIPTION
  The funciton adds the tca active pilots into the ASET.

DEPENDENCIES
  Connected State

PARAMETERS
  subaset_list    - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  This function needs the hdrsrchset.subaset to be integrated which means all
  the existing subaset does not have size of zero sectors.  The table and the
  number of existing subaset match each other to maintain a valid list. It is
  required to run the function hdrsrchutil_integrate_aset before this function.
  

===========================================================================*/
void hdrsrchutil_add_tca_pilot_to_subaset
(

  hdrsrch_sect_struct_type           *sect,
    /* Convenience sect pointer */

  const hdrsrch_aset_pilot_type      *aset_pilot,
    /* Pointer to an array of aset structures */

  uint8                              subaset_idx
    /* The subaset index to add */
)
{
  hdrsrchset_pilotpurge_struct_type  pilotpurge;
    /* Contains input ptr and default action type to be passed to the pilot purge
       function. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the ASET info */

  sect->pilot_gid         = aset_pilot->pilot_group_id;
    /* Update the pilot group id */
     
  sect->aset.mac_idx      = aset_pilot->mac_index;
    /* Mac Index */

  sect->aset.rl_mac_idx   = aset_pilot->rl_mac_index;
    /* Mac Index */

  sect->aset.rab_mac_idx  = aset_pilot->rab_mac_index;
    /* Mac Index */

  sect->aset.drc_cover    = aset_pilot->drc_cover;
    /* DRC cover */

  sect->aset.aux_drc_cover = aset_pilot->aux_drc_cover;
    /* Aux DRC Cover */

  sect->aset.dsc          = aset_pilot->dsc;
    /* Set the DSC. The revB CP will fill dsc to each aset pilot structure 
       considering the softer_handoff flag. Searcher does not check it again.*/

  sect->aset.delta_t2p    = aset_pilot->delta_t2p;
    /* delta t2p */

  sect->aset.ra_channel_gain_lin = 
       hdrsrchutil_ra_channel_gain_db_to_lin( aset_pilot->ra_channel_gain );
    /* Reverse access channel linear gain */

  sect->aset.scheduler_tag = aset_pilot->scheduler_tag;
    /* Update the scheduler_tag */

  sect->aset.rab_length    = 
                    (uint16) hdrsrchutil_rab_length[aset_pilot->rab_length];
    /* Set the RAB length field from the table */

  sect->aset.rab_offset   = aset_pilot->rab_offset;
    /* Set the RAB Length parameter. */

  /* Add into ASET  */

  if (sect->set != HDR_ASET)
  {
    hdrsrchutil_update_new_aset_window_center( sect );
      /* Update the new ASET sector's window center */

    hdrsrchset_add_to_aset( subaset_idx, sect );
      /* Add into the ASET if not in the ASET yet */

    sect->aset.rpc_cell_idx = HDRSRCH_RPC_IDX_NOT_ASSIGNED;
      /* Initialize the RPC index to indicate that the rpc index has not 
         been assigned. for persistent aset pilots, the RPC cell index 
         should be persistent as well.  */

    sect->drop_timer.expired  = FALSE;
    sect->drop_timer.active   = FALSE;
        /* This is a new pilot - start with non expired drop timer */
  }

  /* Group policy enforcment if needed  */

  pilotpurge.action = HDRSRCHSET_PILOTPURGE_ACTION_NO_ACTION;
    /* Reset action to default value */
  pilotpurge.sect_ptr = sect;

  hdrsrchset_pilot_group_purge_mc( &pilotpurge, NULL );
    /* Group policy enforcement to CSET and NSET */
} /* hdrsrchutil_add_tca_pilot_to_subaset */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ADD_SUBASETLIST_TO_ASET

DESCRIPTION
  The function adds the tca subactive pilots into the ASET. 
  - Add the subaset pilots into the ASET ( triggering pilot group purge ).
  - Generate the rpc index for the newly add pilots based on the soft handoff 
    flag.
  - Generate the scheduler group tag if TCA does not include.

DEPENDENCIES
  Connected State

PARAMETERS
  subaset_list    - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE

SIDE EFFECTS
  This function needs the hdrsrchset.subaset to be integrated which means all
  the existing subaset does not have size of zero sectors.  The table and the
  number of existing subaset match each other to maintain a valid list. It is
  required to run the function hdrsrchutil_integrate_aset before this function.

===========================================================================*/

void hdrsrchutil_add_subasetlist_to_aset
(
  const hdrsrch_subactive_set_type   *subaset_list,
    /* Array of subactive set to put in active set */

  uint8                              num_subasets,
    /* Number of subactive set to put into active set */

  uint8                              cc_subactive_set_idx, 
    /* The index to the subaset which carries CC channel */

  boolean                            *ho_override
    /* Handoff override */
)
{
  int8      i, j, k;
    /* Loop index */

  uint8     csr_idx;
    /* Carrier selector index */

  hdrsrch_sect_struct_type*
           tca_sect[HDRSRCH_MAX_NUM_SUBASETS][HDRSRCH_MAX_SUBASET_SIZE];
    /* Temporary array points to the newly added sectors */

  hdrsrch_aset_pilot_type            pilot_to_add;
    /* pilot to add to the aset */


  hdrsrch_sect_struct_type           *sect_ptr;
    /* Convenience sect pointer */

  boolean                            not_persistent = TRUE;
    /* Subactive set persistent flag */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( tca_sect, (int) NULL, sizeof( tca_sect ) );
    /* Initilize the array to NULL */

  /* Add ASET pilots into ASET. The persistent subaset sector should be 
     update first as their asp index is not subject to change. */
  
  /* The first step: Update the persistent subaset which has the persistent 
     demod index assignment. */
     
  HDR_ASSERT(num_subasets <= HDRSRCH_MAX_NUM_SUBASETS);
  
  for( j=0; j<hdrsrchset.subaset_cnt; j++)
  {
    for ( i=0; i< num_subasets; i++ )
    {
      if( hdrsrchset.subaset[j].demod_idx == subaset_list[i].demod_idx )
      {
 
        /* Update the subaset attributes. */

        hdrsrchset.subaset[j].cc_carrier     = 
                                  ( cc_subactive_set_idx == i )? TRUE: FALSE;
          /* If the subaset is the cc monitoring one */

        hdrsrchset.subaset[j].tca_idx        = i;
          /* Update the TCA index. TCA index represents the position of the
             subactive set in the current TCA subaset list. The subaset with
             TCA index 0 indicates this subaset is the reference subaset. */

        hdrsrchset.subaset[j].demod_idx      = subaset_list[i].demod_idx;
          /* Update the demod index according to the assignment */

        hdrsrchset.subaset[j].channel        = subaset_list[i].channel;
          /* Channel information */

        hdrsrchset.subaset[j].non_reportable = subaset_list[i].not_reportable;
          /* Not reportable attribute */

        hdrsrchset.subaset[j].tc_ho_override =  
                                      ho_override[ subaset_list[i].demod_idx ];
          /* Handoff override is pending or not */

        if( hdrsrchrf_get_carrier_idx( subaset_list[i].channel, &csr_idx ) )
        {
          hdrsrchset.subaset[j].csr_idx = csr_idx;
            /* CSR/Sample RAM index fetched from the hdrsrchrf module. The 
               information is needed in fw ASP interface. */

          ASSERT( hdrsrchset.subaset[j].csr_idx == 
                                             hdrsrchset.subaset[j].demod_idx );
            /* Assert the matching b/w the csr index and demod index. */
        }
        else
        {
          if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
          {
            /* CSR assignment is cleared in TC suspend state, use demod_idx 
               as csr_idx. The assumption is that demod idx and csr idx are 
               the same. */
            hdrsrchset.subaset[j].csr_idx = hdrsrchset.subaset[j].demod_idx;

            HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                            "TC suspend state Subaset %d csr %d demod %d",
                            j, 
                            hdrsrchset.subaset[j].csr_idx,
                            hdrsrchset.subaset[j].demod_idx );
          }
          else
          {
            /* Error fatal as the channel is not listed in the CSR list */
  
            ERR_FATAL( "Could not locate band%d chan%d in the CSR config list",
                       (uint8) subaset_list[i].channel.band, 
                       subaset_list[i].channel.chan_num, 0  );
          }
        }

        /* Add pilots into this subaset. */

        for( k=0; k<subaset_list[i].num_aset_pilots; k++ )
        {
          pilot_to_add = subaset_list[i].aset_list[k];

          sect_ptr = hdrsrchset_get_sect( pilot_to_add.pilot_pn,
                                        subaset_list[i].channel );
            /* Find sector for each pilots */

          if ( sect_ptr != NULL )
          {
            tca_sect[i][k]=sect_ptr;
              /* Linked this one into the local sector list */
 
            hdrsrchutil_add_tca_pilot_to_subaset( sect_ptr, &pilot_to_add, j );
              /* Add the tca pilot into the subaset */
          }
          else
          {
            ERR_FATAL( "No free sector available!", 0, 0, 0 );
          }
        } /* for k */

        break;
      }/* if( hdrsrchset.subaset[j].demod_idx == subaset_list[i].demod_idx ) */
    } /* for i */
  } /* for j */

  /* The second step: add the new subasets */

  for ( i=0; i< num_subasets; i++ )
  {
    not_persistent = TRUE;

    for( j=0; (j<hdrsrchset.subaset_cnt) && (j < HDRSRCH_MAX_NUM_SUBASETS); j++)
    {
      if( hdrsrchset.subaset[j].demod_idx == subaset_list[i].demod_idx )
      { 
        /* Persistent subaset */
        not_persistent = FALSE;

        break; 
      }
    } /* for j */

    if( not_persistent )
    {
      ASSERT( j == hdrsrchset.subaset_cnt );
      
      hdrsrchset.subaset_cnt ++;
        /* Add a new subaset */

      if ( hdrsrchset.subaset_cnt >= ( HDRSRCH_MAX_NUM_SUBASETS + 1 ) )
      {
        ERR_FATAL( "Cannot add new subaset as it is full %d",
                   hdrsrchset.subaset_cnt, 0, 0 );
      }
      /* Update the subaset attributes. */

      hdrsrchset.subaset[j].cc_carrier     = 
                                  ( cc_subactive_set_idx == i )? TRUE: FALSE;
        /* If the subaset is the cc monitoring one */

      hdrsrchset.subaset[j].tca_idx        = i;
        /* Update the TCA index. TCA index represents the position of the
           subactive set in the current TCA subaset list. The subaset with
           TCA index 0 indicates this subaset is the reference subaset. */

      hdrsrchset.subaset[j].demod_idx      = subaset_list[i].demod_idx;
        /* Update the demod index according to the assignment */

      hdrsrchset.subaset[j].channel        = subaset_list[i].channel;
        /* Channel information */

      hdrsrchset.subaset[j].non_reportable = subaset_list[i].not_reportable;
        /* Not reportable attribute */

      hdrsrchset.subaset[j].tc_ho_override = ho_override[ subaset_list[i].demod_idx ];
        /* Handoff override is pending or not */

      if( hdrsrchrf_get_carrier_idx( subaset_list[i].channel, &csr_idx ) )
      {
        hdrsrchset.subaset[j].csr_idx = csr_idx;
          /* CSR/Sample RAM index fetched from the hdrsrchrf module. The 
         information is needed in fw ASP interface. */

        ASSERT( hdrsrchset.subaset[j].csr_idx == hdrsrchset.subaset[j].demod_idx );
          /* Assert the matching b/w the csr index and demod index. */
      }
      else
      {
         if ( hdrsrch_state_id == HDRSRCH_SUSPENDED_TC_STATE )
          {
            /* CSR assignment is cleared in TC suspend state, use demod_idx 
               as csr_idx. The assumption is that demod idx and csr idx are 
               the same. */
            hdrsrchset.subaset[j].csr_idx = hdrsrchset.subaset[j].demod_idx;

            HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                            "TC suspend state subaset %d csr %d demod %d",
                            j, 
                            hdrsrchset.subaset[j].csr_idx,
                            hdrsrchset.subaset[j].demod_idx );
          }
          else
          {
            /* Error fatal as the channel is not listed in the CSR list */

            ERR_FATAL( "Could not locate band%d chan%d in the CSR config list",
                     (uint8) subaset_list[i].channel.band, 
                     subaset_list[i].channel.chan_num, 0  );
          }
      }

      /* Add pilots into this subaset. */

      for( k=0; k<subaset_list[i].num_aset_pilots; k++ )
      {
        pilot_to_add = subaset_list[i].aset_list[k];

        sect_ptr = hdrsrchset_get_sect( pilot_to_add.pilot_pn,
                                      subaset_list[i].channel );
          /* Find sector for each pilots */

        if ( sect_ptr != NULL )
        {
          tca_sect[i][k]=sect_ptr;
            /* linked this one into the local sector list */
 
          hdrsrchutil_add_tca_pilot_to_subaset( sect_ptr, &pilot_to_add, j );
            /* Add the tca pilot into the subaset */
        }
        else
        {
          ERR_FATAL( "No free sector available!", 0, 0, 0 );
        }
      } /* for k */ 
    } /* if( not_persistent ) */
  } /* for i */

  /* RPC Cell Index Processing 
     1. The rpc_cell_idx should be persistent if the associated cell is 
        persistent b/w two TCAs. This is the way that the firmware can
        detect soft or hard handoff ( ? )
     2. A special case, an sector with RPC_cell_index is persistent but
        is listed behind a new tca sector.  */
  
  for ( i=0; i< num_subasets; i++ )
  {
    /* 1. Forward update the rpc index. 
       2. Backword update the rpc index.
       3. Grant new rpc_idx if still not assigned.
       4. Forward update the rpc index again. */

    /* Forward update the RPC cell index */

    for( j=1; 
         ( ( j < subaset_list[i].num_aset_pilots ) &&
           ( j < HDRSRCH_ASET_MAX_SIZE ) );
          j++)
    {
      if( tca_sect[i][j]->aset.rpc_cell_idx == HDRSRCH_RPC_IDX_NOT_ASSIGNED )
      {
        if( ( subaset_list[i].aset_list[j].softer_handoff == TRUE ) &&
            ( tca_sect[i][j-1]->aset.rpc_cell_idx != 
                                             HDRSRCH_RPC_IDX_NOT_ASSIGNED ) )
        {
          /* This pilot is in Softer Handoff with preceding pilot! */

          tca_sect[i][j]->aset.rpc_cell_idx = tca_sect[i][j-1]->aset.rpc_cell_idx;
            /* Update the rpc cell index only if the previous rpc cell index is
                 an assigned one and with the same cell */
        } /* if subaset_list */
      } /* if tca_sect */
    } /* for j */

    /* Backward update the RPC cell index */

    for( j=subaset_list[i].num_aset_pilots-2; 
         ( ( j >= 0 ) && ( j < HDRSRCH_ASET_MAX_SIZE - 1) );
         j--)
    {
      if( tca_sect[i][j]->aset.rpc_cell_idx == HDRSRCH_RPC_IDX_NOT_ASSIGNED )
      {
        if( ( subaset_list[i].aset_list[j+1].softer_handoff == TRUE ) &&
            ( tca_sect[i][j+1]->aset.rpc_cell_idx != HDRSRCH_RPC_IDX_NOT_ASSIGNED ) )
        {
          tca_sect[i][j]->aset.rpc_cell_idx = tca_sect[i][j+1]->aset.rpc_cell_idx;
            /* Update the rpc cell index only if the previous rpc cell index is
               an assigned one */
        }
      } /* if tca_sect */
    } /* for j*/

    /* Assign and forward update RPC cell index if needed */

    for( j = 0; 
         ( ( j < subaset_list[i].num_aset_pilots ) && 
           ( j < HDRSRCH_MAX_SUBASET_SIZE ) );
         j++)
    {
      if( tca_sect[i][j]->aset.rpc_cell_idx == HDRSRCH_RPC_IDX_NOT_ASSIGNED )
      {
        if ( subaset_list[i].aset_list[j].softer_handoff == FALSE ) 
        {
          /* If the next pilot in the list is not in the same cell */
          k = tca_sect[i][j]->aset.subaset_idx;

          tca_sect[i][j]->aset.rpc_cell_idx = 
                                         hdrsrchset_get_new_rpc_idx( k );
            /* get a new one if possible */
        } /* if subaset_list */
        else
        {
          ASSERT( j > 0 );
            /* j could never be 0 since sofer_handoff bit cannot be 0 for first sector in TCA */

          tca_sect[i][j]->aset.rpc_cell_idx = tca_sect[i][j-1]->aset.rpc_cell_idx;
            /* Update the rpc cell index only if the previous rpc cell index is
               an assigned one */
        }
      } /* if tca_sect */
    } /* for j */
  } /* for i */


  /* Assign schedtag if not included in the tCA */

  for ( i=0; i< num_subasets; i++ )
  {
    for( j = 0; 
         ( ( j < subaset_list[i].num_aset_pilots ) && 
           ( j < HDRSRCH_MAX_SUBASET_SIZE ) );
         j++)
    {
      /* Scheduler_tag is handled through softer_handoff flags if it not specified
         by TCA. Please be aware if the scheduler tag of the sect is not included,
         then the entire aset pilots should not include the scheduler tag. When
         this happens, searcher should generate its own scheduler tag based on the
         softer_handoff flags, i.e the sectors within the same softer handoff shall
         share the same scheduler tag. */
      /* TBD: check this? or CP check this */

      /* Different from the RPC cell index, the scheduler tag does not need to be
         persistent from the previous the TCA. This is the reason that there is no
         need to perform the forward and backward update. */

      /* Searcher only suports the TCA that have NumUniqueForwardTrafficMACIndices
         equal to 1, which means searcher does not support idle ASET sector. This 
         means if the schedtag of one aset pilot is not included, then the entire
         aset pilots should not include schedtag. */

      if( tca_sect[i][j]->aset.scheduler_tag == HDRSRCH_REV0A_SCHDTAG ) 
      {
        /* rev0 revA scheduler tags. no change */
      }
      else if( tca_sect[i][j]->aset.scheduler_tag == HDRSRCH_NOT_INCLUDED_SCHDTAG ) 
      {
        if( subaset_list[i].aset_list[j].softer_handoff == TRUE )
        {
          if( j == 0 )
          {
             ERR_FATAL(
                "The softer_handoff of the 1st sector in subaset[%d] is TRUE", 
                0, 0, 0);
          }

          tca_sect[i][j]->aset.scheduler_tag 
                                         = tca_sect[i][j-1]->aset.scheduler_tag;
            /* Update the scheduler tag based on the soft handoff indicator */
        }
        else
        {
          /* By standard, the scheduler tag is defined 7 bit; valid scheduer 
             tag ranges 0 to 127 */
          tca_sect[i][j]->aset.scheduler_tag = hdrsrchset_get_next_scheduler_tag();
        }
      } /* if tca_sect[i][j] */
      else if( tca_sect[i][j]->aset.scheduler_tag == HDRSRCH_NOT_USED_SCHDTAG )
      {
        /* This is the route when NumUniqueForwardTrafficMACIndices == 0 */

        ERR_FATAL("Do not support idle sector in traffic ASET", 0, 0, 0);
      } /* if tca_sect */
    } /* for j */
  } /* for i */

    /* No need to keep saved ASET[0] to cover empty ASET condition, e.g.,
       during TCA cmd processing, since one should now exist.  */
  if ( ( hdrsrchset.subaset_cnt != 0) &&
       ( hdrsrchset.subaset[0].aset_cnt != 0 ) )
  {
    hdrsrchset.last_subaset0_aset0.set = HDR_EMPTYSET;
  }

} /* boolean hdrsrchutil_add_subasetlist_to_aset */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_HAS_REPORTABLE_GROUP_MEMBER

DESCRIPTION
  The function checks the reportable flag in the drop timer. The motivation of
  the management is not to let every pilot generate pilot report in the revB 
  traffic state. Only one pilot from each pilot group is allowed to reports.

DEPENDENCIES
  Called only in the connected state during the TCA processing.

PARAMETERS
  sect_ptr - the ASET sector pointer 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchutil_has_reportable_group_member
( 
  hdrsrch_sect_struct_type* sect_ptr 
    /* ASET sector pointer */
)
{
  int8                         i, j;
    /* Loop index */

  boolean                      has_reportable_pal = FALSE;
    /* Return value */

  hdrsrch_sect_struct_type*    match_ptr;
    /* Convenience pointer */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Error check the input sector pointer */

  if( sect_ptr->set != HDR_ASET )
  {
    ERR_FATAL("Reportable pal check only applies to ASET sector", 0, 0, 0);
  }

  /* Find reportable pal within ASET */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      match_ptr = hdrsrchset.subaset[i].aset[j];

      if( ( match_ptr != sect_ptr ) &&
          ( match_ptr->pn_offset == sect_ptr->pn_offset ) &&
          ( match_ptr->pilot_gid == sect_ptr->pilot_gid ) )
      {
        ASSERT( !SYS_CHAN_CMP_EQUAL( match_ptr->chan, sect_ptr->chan ) );

        has_reportable_pal = TRUE ;
        break;
      }
    } /* for j */

    if( has_reportable_pal == TRUE )
    {
      break;
    }
  } /* for i*/

  return has_reportable_pal;
} /* boolean hdrsrchutil_has_reportable_group_member */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_SELECT_REPORTABLE_PILOTS

DESCRIPTION
  This function modifies the reportable flag for this sector in the drop timer. 
  The motivation of the management is not to let any pilot to generate pilot 
  report in the revB traffic state. Only one pilot from each pilot group is 
  used/allowed to report.

DEPENDENCIES
  Connected state

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  hdrsrchset.subaset_reportable_id set to cc_carrier's subaset index

===========================================================================*/

void  hdrsrchutil_select_reportable_pilots( void )
{
  int8                          i, j, k, l;
    /* Loop indices */

  int8                          start_subaset_idx, start_aset_idx;
    /* Starting subaset and ASET index to start loop */

  hdrsrch_pilot_gid_type        tmp_pgid;
    /* Temporary for pgid */

  hdrsrch_sect_struct_type*     sect_ptr;
    /* Convenience sector pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* The assigning reportable pilots rules are summarized as below:
     1. Mark the sectors in the nonreportable subaset as not reportable.
     2. If more than one reportable subaset in a pilot group,
        i.e., non_reportable is FALSE, then prefer CC carrier subaset.
     3. Arbitrarily select the 1st reportable pilot in a pilot group as
        reportable when pilot group does not contain CC carrier
   */

  /* Init all drop_timer.reportable flags to TRUE */
  for( i=0; (i<hdrsrchset.subaset_cnt) && (i<HDRSRCH_MAX_NUM_SUBASETS ); i++ )
  {
    for( j=0; (j<hdrsrchset.subaset[i].aset_cnt) && (j < (HDRSRCH_MAX_SUBASET_SIZE+1)); j++ )
    {
      hdrsrchset.subaset[i].aset[j]->drop_timer.reportable = TRUE;
    }
  }

 for( i=0; (i<hdrsrchset.subaset_cnt) && (i<HDRSRCH_MAX_NUM_SUBASETS ); i++ )
  {
    /* Set the nonreportable subaset sectors as not reportable */
    if( hdrsrchset.subaset[i].non_reportable )
    {
      for( j=0; (j<hdrsrchset.subaset[i].aset_cnt) && (j < (HDRSRCH_MAX_SUBASET_SIZE+1)); j++ )
      {
        hdrsrchset.subaset[i].aset[j]->drop_timer.reportable = FALSE;
      }
    }
    else
    {
      /* Must be reportable carrier.  If there are multiple reportable
         carriers, prefer the CC carrier for triggering RUP.
         Otherwise, first pilot in pilot group is arbitrarily
         appointed reportable of the pilot group. */
      if ( hdrsrchset.subaset[i].cc_carrier == TRUE )
      {
        hdrsrchset.subaset_reportable_idx = i;
          /* Index of reportable sub-index with CC carrier preferred. */

        for( j=0; (j<hdrsrchset.subaset[i].aset_cnt) && (j < (HDRSRCH_MAX_SUBASET_SIZE+1)); j++ )
        {
          hdrsrchset.subaset[i].aset[j]->drop_timer.reportable = TRUE;
            /* Since subaset is reportable, set all CC carrier pilots
               as reportable. */

          if ( hdrsrchset.subaset[i].aset[j]->pilot_gid !=
               HDRSRCH_UNIQUE_PILOT_GID )
          {
            /* Mark all non-cc carrier pilots with the same pilot gid as not reportable */
            for( k=0; (k<hdrsrchset.subaset_cnt) && (k<HDRSRCH_MAX_NUM_SUBASETS); k++ )
            {
              if ( hdrsrchset.subaset[k].cc_carrier == FALSE )
              {
                for( l=0; (l<hdrsrchset.subaset[k].aset_cnt) && (l<(HDRSRCH_MAX_SUBASET_SIZE+1)); l++ )
                {
                  /* Mark all pilots as non-reportable since CC carrier is of
                     group is already reportable. */
                  if ( HDRSRCHSET_PILOT_GROUP_CMP(
                         *hdrsrchset.subaset[i].aset[j],
                         *hdrsrchset.subaset[k].aset[l] ) == TRUE )
                  {
                    hdrsrchset.subaset[k].aset[l]->drop_timer.reportable = FALSE;
                      /* Another non-cc_carrier pilot in pilot group found.
                         Prefer the CC carrier as reportable. */
                  }
                } /* for l=0;... */
              } /* if not cc_carrier*/
            } /* for k=0... */
          } /* if PGID not HDRSRCH_UNIQUE_PILOT_GID */
        } /* for j=0... */
      } /* if .cc_carrier == TRUE */
      else
      {
        /* Not cc_carrier.  Make 1st pilot in PGID reportable.
           Rely on:
           1) drop_timer->reportable init to TRUE
           2) non-cc_carrier pilots in PGID as cc_carrier pilot are already set
              drop_timer->reportable to FALSE
           3) Any previous encountered PGID will have already set 1st pilot's
              drop_timer->reportable to TRUE and remaining ones to FALSE.
           So only deal pilots that still have drop_timer->reportable == TRUE
         */
       for( j=0; (j<hdrsrchset.subaset[i].aset_cnt) && (j < (HDRSRCH_MAX_SUBASET_SIZE+1)); j++ )
        {
          if ( hdrsrchset.subaset[i].aset[j]->drop_timer.reportable == TRUE )
          {
            /* Must be 1st pilot in this pilot group encountered.  Keep as
               reportable and set remaining pilots with same PGID as
               drop_timer.reportable = FALSE.  Only need to search past the
               current point in the subaset and aset.  */

            tmp_pgid = hdrsrchset.subaset[i].aset[j]->pilot_gid;
              /* Save pilot GID of first pilot in pilot group.  */

            /* Treat Pilot GID HDRSRCH_UNIQUE_PILOT_GID as different
               group */
            if ( tmp_pgid != HDRSRCH_UNIQUE_PILOT_GID )
            {

              /* Skip aset already encountered including any in subaset.
                 Set starting indices for subaset and aset. */
              if ( ( j + 1 ) == hdrsrchset.subaset[i].aset_cnt )
              {
                /* Skip to next subaset, if any. */
                start_subaset_idx = i + 1;
                start_aset_idx = 0;
              }
              else
              {
                /* Stay in current subaset, but skip current aset. */
                start_subaset_idx = i;
                start_aset_idx = j + 1;
              }
  
              /* Loop through remaining subasets and asets looking for same pgid.
                 Treat HDRSRCH_UNIQUE_PILOT_GID as different pilot group. */
              for ( k=start_subaset_idx; (k<hdrsrchset.subaset_cnt) && (k<HDRSRCH_MAX_NUM_SUBASETS); k++ )
              {
                for ( l=start_aset_idx; (l<hdrsrchset.subaset[k].aset_cnt) && (l<(HDRSRCH_MAX_SUBASET_SIZE+1)); l++ )
                {
                  sect_ptr = hdrsrchset.subaset[k].aset[l];
                    /* Temp Pointer */

                  /* Note: sect_ptr->pilot_gid cannot be HDRSRCH_UNIQUE_PILOT_GID
                     if it is equal to tmp_pgid since tmp_pgid cannot be
                     HDRSRCH_UNIQUE_PILOT_GID. */
                  if ( tmp_pgid == sect_ptr->pilot_gid )
                  {
                    hdrsrchset.subaset[k].aset[l]->drop_timer.reportable = FALSE;
                  }
                } /* for l=start_aset_idx... */
  
                start_aset_idx = 0;
                  /* For next subaset, go through all ASET's */
  
              } /* for k=start_subaset_idx... */
            } /* if ( tmp_gid != HDRSRCH_UNIQUE_PILOT_GID ) */

          } /* if ...drop_timer.reportable == TRUE */
        } /* for j=0... */
      } /* else not cc_carrier */

    } /* else */

  } /* for i */

} /* void  hdrsrchutil_select_reportable_pilots() */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_UPDATE_ASET_PILOTS_REVB

DESCRIPTION
  Updates the active set list with new active set pilots. The function is used
  only for revB HDR which is able to process multiple subactive ASETs.

DEPENDENCIES
  Connected State

PARAMETERS
  subaset_list - Array of pilots to put into ASET
  num_aset_pilots - Number of pilots in the array

RETURN VALUE
  TRUE  - Set transitions performed.
  FALSE - No set transitions occurred.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchutil_update_aset_pilots_revb
(
  const hdrsrch_subactive_set_type   *subaset_list,
    /* Array of subactive sets to put into the active set */

  uint8                              num_subasets,
    /* Number of pilots to put into active set */

  uint8                              cc_subactive_set_idx
    /* The index to the subaset which carries CC channel */
)
{

  boolean                        ho_override[ HDRSRCH_MAX_DEMOD_CARRIERS_CNT ];
    /* Handoff override pending flags */

  uint8                          demod_carrier_mask;
    /* Demod carrier mask indicating which demod carrier is enabled */

  uint8                          demod_idx;
    /* Demod index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_determine_handoff_override(subaset_list, num_subasets, ho_override );
    /* Determine whether handoff override is necessary */

#ifdef FEATURE_HDR_REVB_SUPPORTED

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "ho_override[0] %d ho_override[1] %d ho_override[2] %d",
  ho_override[ 0 ],ho_override[ 1 ],ho_override[ 2 ] );

#else

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "ho_override[0] %d ",
  ho_override[ 0 ]);

#endif /* FEATURE_HDR_REVB_SUPPORTED */

  hdrsrchmsg_init_asp_update_msg();
    /* Init the ASP update message */

  rex_enter_crit_sect(&hdrsrch.crit_sect);
    /* protect hdrsrchset access */

  (void) hdrsrchset_grice_drop_gcset();
    /* Clear gRICE CSET */

  (void) hdrsrchutil_drop_former_aset_sects( subaset_list, num_subasets );
    /* Drop the non-existent ASET sectors.  */

  (void) hdrsrchfing_program_fings();
    /* Reprogram fingers to release the finger resource */

  hdrsrchutil_add_subasetlist_to_aset( subaset_list, 
                                       num_subasets,
                                       cc_subactive_set_idx,
                                       ho_override );
    /* Add the subaset list to the ASET */

  rex_leave_crit_sect(&hdrsrch.crit_sect);
    /* leave critical section */

  /* Scheduler group TCA processing */

  if( hdrsrchschdgrp_tca_update() == FALSE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_INVALID_REVB_TCA );
      /* System lost on invalid scheduler group */
  }

  if( hdrsrch.schdgrp_rpt_cb != NULL )
  {
    hdrsrch.schdgrp_rpt_cb();
      /* Scheduler group change indication */
  }

  demod_carrier_mask = hdrsrchutil_get_demod_carrier_mask();
    /* Get which demod carriers are enabled */

  hdrsrchfing_set_demod_carrier_mask( demod_carrier_mask );
    /* Update demo carrier number to finger driver */

  (void) hdrsrchutil_aset_commit_to_dsp();
    /* Confirm to the fw about the ASET changes*/

  if( hdrsrcheq.grice_is_allowed ) 
  {
    hdrsrchset_grice_update_gcset();
      /* Update gCSET pilot information */
  }

  hdrsrchmsg_send_multi_asp_update_msg( TRUE );
    /* Send ASP update message */

  /* Only assign equalizer if DO is in traffic state, since this function is called
     upon reception of TCA and TCA may arrive when DO is in traffic suspend state.
     In traffic suspend state, FW is not active.  Recent targets require FW to be
     active when this function is called.
     This function will be called when DO re-enter traffic state from traffic
     suspend state after RF tune.  At that time, FW will be enabled. */
  if( ( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE ) &&
      ( hdrsrchafc_afc_uses_rot() ) )
  {  
    hdrsrcheq_assign_eqs( demod_carrier_mask );
      /* Assign equalizers */
  }

  hdrsrchutil_select_reportable_pilots();
    /* Select pilots to generate pilot report */

  return TRUE;
} /* hdrsrchutil_update_aset_pilots_revb */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_ASSIGN_DEMOD_IDXS_TO_SUBASETS

DESCRIPTION
  If the subactive_set_list is NULL, then the function returns the demod idx
  sequence according to the current TCA assignment.

  Based on the request from RMAC4, the demod index assigment should be persistent 
  if a carrier is persistent between 2 TCAs, for both hard handoff and soft 
  handoff.

DEPENDENCIES
  Connected State

PARAMETERS

RETURN VALUE
  Return an arry containing demod_idx assignment to each of the subaset in
  the order of the tca subaset list.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_assign_demod_idxes_to_subasets
(
  uint8 num_subactive_sets, 
    /* Number of subactive sets */

  const hdrsrch_subactive_set_type * subactive_set_list,
    /* The link list of subactive sets */

  hdrsrch_demod_idx_assignment_type * demod_idx_assigned
    /* The demod index assignments */
)
{
  int8 i, j, k;
    /* Loop index */

  struct
  {
    boolean                 assigned;
      /* If the subaset has assigned a demod_idx */

    hdrsrch_demod_idx_type  demod_idx;
      /* The demod idx assigned */
  }iflag[HDRSRCH_MAX_NUM_SUBASETS]; 
    /* Internal flags whose idexes refer to the incoming subaset list. The
       index matches the index order of the subactive_set_list.  */

  hdrsrch_demod_idx_type    demod_idx_to_use = 0;
    /* Variable to detect if the demod_idx is availabe */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( iflag, 0, sizeof( iflag ) );
    /* Clean the internal flags  */

  num_subactive_sets = MIN(num_subactive_sets, HDRSRCH_MAX_NUM_SUBASETS);
    /* klockworks fix, limit the array index to max */


  /* Step 0: If the subactive_set_list is NULL, return the demod index in 
     the current TCA order. */
  if( subactive_set_list == NULL )
  {
    for( i=0; i<hdrsrchset.subaset_cnt; i++ )
    {
      demod_idx_assigned[hdrsrchset.subaset[i].tca_idx].demod_idx 
         = hdrsrchset.subaset[hdrsrchset.subaset[i].tca_idx].demod_idx;
    }
    return;
  }

  /* Step 1: Search the current ASET for demod indexes of the persistent
     subasets. The assignment from this step is stored in the iflag 
     structure. */

  for( i=0; i<num_subactive_sets; i++ )
  {
    /* Search the subactive set across all aset sectors. If there is a 
       channel matched, then assign the demod_idx of the matching subaset
       to the incoming subactive set. */

    for( k=0;(k<hdrsrchset.subaset_cnt) && (k<HDRSRCH_MAX_NUM_SUBASETS); k++ )
    {
      if( ( iflag[i].assigned != TRUE ) &&
          SYS_CHAN_CMP_EQUAL( subactive_set_list[i].channel, 
                                hdrsrchset.subaset[k].channel ) )
      {
        /* Assign the demod index, mark the corresponding flag and bail 
           out */

        iflag[i].demod_idx = hdrsrchset.subaset[k].demod_idx;
        iflag[i].assigned  =  TRUE;
        break;
      } /* if iflag */
    } /* for k */
  } /* for i */


  /* Step 2: Assign the demod_idx to subactive set unassigned in the
     first step. */

  demod_idx_to_use = 0;
    /* Initialize Demod idx to use */

  for ( i=0; i<num_subactive_sets; i++)
  {
    if ( iflag[i].assigned == FALSE )
    {
      /* Search iflag table to check the availability of the 
         demod_idx_to_use */

      for( j=0; j<num_subactive_sets; j++ )
      {
        if( ( iflag[j].assigned == TRUE ) &&
            ( iflag[j].demod_idx == demod_idx_to_use ) )
        {
          /* If the demod_idx is used, then try the next one */

          demod_idx_to_use ++;

          if( demod_idx_to_use >= HDRSRCH_MAX_NUM_SUBASETS )  
          {
            /* This is the error case, we should find a unused before
               hit this point */

            ERR_FATAL("Demod idx assignemt Error!", 0, 0, 0);
          }

          j = -1;
            /* Reset the loop to restart the comparison */
        } /* if */
      } /* for j */

      /* Assign the demod index to the curret subactive set */

      iflag[i].demod_idx = demod_idx_to_use;
      iflag[i].assigned  = TRUE;
    } /* if */
  } /* for I */

  /* Step 3: Fill out the array to return the assignment. */

  for( i=0; i< num_subactive_sets; i++ )
  {
    demod_idx_assigned[i].demod_idx = iflag[i].demod_idx;
      /* Fill up the demod assignments */
  }
}
#endif /* FEATURE_HDR_REVB */


/*===========================================================================

FUNCTION HDRSRCHUTIL_SET_HANDOFF_CONTROL

DESCRIPTION
  This function sets handoff override trigger mode for the subasets 
  with override flag set.

DEPENDENCIES
  Connected State

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_set_handoff_control( void )
{

  uint8                      sa;
    /* Subaset index */

  uint8                      asp;
    /* The strongest ASP index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  hdrsrchmsg_init_handoff_ovrd_msg();
    /* Init handoff override message */

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa ++ ) 
  {
    if ( hdrsrchset.subaset[ sa ].tc_ho_override == TRUE )
    {
      asp = hdrsrchutil_get_strongest_asp_idx( sa );
        /* Get the strongest ASP index */

      hdrsrchmsg_ho_ovrd_set_trigger_mode( hdrsrchset.subaset[ sa ].demod_idx,
                                           asp );
        /* Set best ASP in handoff override trigger mode */

      hdrsrchset.subaset[ sa ].tc_ho_override = FALSE;
      hdrsrchset.subaset[ sa ].tune_away_ho_override = FALSE;
        /* HO override due to TCA has higher priority than tune away */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "handoff override trigger mode at demod idx %d asp idx %d", 
                      hdrsrchset.subaset[ sa ].demod_idx, asp);


    }
    else
    if ( hdrsrchset.subaset[ sa ].tune_away_ho_override == TRUE )
    {
      hdrsrchmsg_ho_ovrd_set_trigger_mode( hdrsrchset.subaset[ sa ].demod_idx,
                                           hdrsrchset.subaset[ sa ].tune_away_basp_index );
        /* Set best ASP in handoff override trigger mode */

      hdrsrchset.subaset[ sa ].tune_away_ho_override = FALSE;

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "handoff override trigger mode at demod idx %d asp idx %d due to tune away", 
                      hdrsrchset.subaset[ sa ].demod_idx, 
                      hdrsrchset.subaset[ sa ].tune_away_basp_index );
    }
    else
    {
      hdrsrchmsg_ho_ovrd_set_fw_ctrl( hdrsrchset.subaset[ sa ].demod_idx,
                                      HDRSRCHMSG_HO_OVRD_DSP_CTRL );
        /* Set firmware to control handoff */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "handoff controlled by mdsp at demod idx %d", 
                      hdrsrchset.subaset[ sa ].demod_idx);

    }
  }

  hdrsrchmsg_send_msg( &hdrsrchmsg.handoff_ovrd_msg, 
                       sizeof( hdrfw_handoff_override_msg_t ) );
    /* Send handoff override message */
}  /* hdrsrchutil_set_handoff_control */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_REF_SECTOR_PTR()

DESCRIPTION
  The function returns the current tracking sector pointer. If there  is no 
  tracking sector, the function returns NULL.

DEPENDENCIES
  DSP clock is up.

PARAMETERS

RETURN VALUE
  The sector pointer of the current tracking sector.

SIDE EFFECTS
  None

===========================================================================*/

hdrsrch_sect_struct_type* hdrsrchutil_get_ref_sector_ptr( void )
{
  uint16  ref_pilot_info = 0;
    /* Local variable to store the current firmware tracking info */

  uint8   ref_asp_idx = 0;
    /* Reference ASP index */

  hdrsrch_demod_idx_type ref_demod_idx = 0;
    /* Reference Demod Carrier index */

  hdrsrch_sect_struct_type* ref_sect = NULL;
    /* Return value */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_FACTORY_TESTMODE
   return hdrsrchset.subaset[0].aset[0];
#endif

#if (defined (HDRSRCH_UT_PLATFORM) || defined(TEST_FRAMEWORK))
  #error code not present
#else
  ref_pilot_info = HDRMSTR_GET_REF_PILOT_INFO();
    /* Read the reference pilot information from the fw interface. */

  if ( ( ref_pilot_info & HDRMSTR_TRACKING_BIT_MASK ) == 0x0 )
  {
    /* If there is a reference pilot currently in track */

    ref_demod_idx = ( ref_pilot_info & HDRMSTR_DEMOD_CARRIER_BIT_MASK ) >>
                                           HDRMSTR_DEMOD_CARRIER_BIT_SHIFT;

    ref_asp_idx = ( ref_pilot_info & HDRMSTR_ASP_BIT_MASK ) >> 
                                                     HDRMSTR_ASP_BIT_SHIFT;

    ref_sect = hdrsrchset_get_demod_asp_sect_ptr( ref_demod_idx, ref_asp_idx );
      /* Get the reference sector information from current ASET */
  }
#endif /* HDRSRCH_UT_PLATFORM */

  if ( ref_sect == NULL )
  {
    if ( ( ref_pilot_info & HDRMSTR_TRACKING_BIT_MASK ) == 0x0 )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,"No Reference pilot found in ASET. HDRMSTR_TRACKING=%x",
                      ( ref_pilot_info & HDRMSTR_TRACKING_BIT_MASK ) );
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_LOW,"No Reference pilot. No MSTR. HDRMSTR_TRACKING=%x",
                      ( ref_pilot_info & HDRMSTR_TRACKING_BIT_MASK ) );
    }
  }

  return ref_sect;
} /* hdrsrch_sect_struct_type* hdrsrchutil_get_ref_sector_ptr() */



/*===========================================================================

FUNCTION HDRSRCHUTIL_MCDO_CHECK

DESCRIPTION
  The function does the following:

  MCDO mode:
    Increase MDSP clock speed to 144 MHz

  Not MCDO (SCDO/Idle):
    Decrease MDSP clock speed to 72 MHz

DEPENDENCIES
  hdrsrchutil_npa.mdsp_clk_handle must be non-null.

PARAMETERS
  speed_mhz     - MDSP Clock speed in MHz.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_mcdo_check
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if MCDO Traffic */
  if ( hdrsrchset.subaset_cnt > 1 )
  {
#ifdef FEATURE_NPA
#if (!defined(FEATURE_MCPM) || defined(FEATURE_MCPM_CLKRGM))
    hdrsrchutil_npa_issue_mdsp_clk_request( HDRSRCHUTIL_NPA_MDSP_CLK_144MHZ );
      /* MCDO Traffic - Increase NPA MDSP CLK speed to 144 MHz */
#endif /* !FEATURE_MCPM || FEATURE_CLKRGM_MCPM */
#endif

    hdrsrchmcpm_set_mcdo_status( TRUE );
      /* Convey MCDO status to MCPM */

  }
  else
  {
#ifdef FEATURE_NPA
#if (!defined(FEATURE_MCPM) || defined(FEATURE_MCPM_CLKRGM))
    /* Equalizer is On only if NV is set and HDR is in Traffic State */
    if ( ( hdrsrcheq_is_grice_enabled() ) &&
         ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) )
    {
      hdrsrchutil_npa_issue_mdsp_clk_request( HDRSRCHUTIL_NPA_MDSP_CLK_144MHZ );
      /* MDSP needs to run at 144MHz speed to support gRICE Equalizer
         (gRICE Equalizer must be selected and HDR is in traffic) */
    }
    else
    {
      hdrsrchutil_npa_issue_mdsp_clk_request( HDRSRCHUTIL_NPA_MDSP_CLK_72MHZ );
        /* Not MCDO Traffic - Decrease NPA MDSP CLK speed to 72 MHz */
    }
#endif /* !FEATURE_MCPM || FEATURE_CLKRGM_MCPM */
#endif
 
    hdrsrchmcpm_set_mcdo_status( FALSE );
      /* Convey SCDO status to MCPM */

  }

} /* hdrsrchutil_mcdo_check */

#ifdef FEATURE_NPA
/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_CREATE_SYNC_CLIENT()

DESCRIPTION
  The function creates and stores a local copy of all HDR NPA client handles.

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
  hdrsrchutil_npa_cpu_handle is setup

===========================================================================*/

void hdrsrchutil_npa_create_sync_client( void )
{
  npa_query_type query_result;
    /* Value returned from the npa resource query */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* To avoid creating multiple NPA handles for HDR, only create a new handle
     if the NPA handle is alreay NULL.   HDR's NPA handle will be NULL at task
     init and after its NPA handle is destroyed. */

  /* Additional check to see if the npa node resource is available in the system
     before attempting to use it. */
  if ( ( hdrsrchutil_npa.cpu_handle == NULL ) && 
       ( npa_query_by_name( "/core/cpu",
                            0,
                            &query_result ) == NPA_QUERY_SUCCESS ) )
  {
    hdrsrchutil_npa.cpu_handle = npa_create_sync_client( 
                                   "/core/cpu",        /* resource name */
                                   "HDR",              /* client name   */
                                   NPA_CLIENT_REQUIRED /* work model    */
                                 );
      /* Create a handle to the /core/cpu resource */

    ASSERT( hdrsrchutil_npa.cpu_handle != NULL );
      /* Make sure handle is initialized. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR NPA CREATE CLIENT" );
  }

  /* Additional check to see if the SSBI npa node resource is available in the system
     before attempting to use it. */
  if ( ( hdrsrchutil_npa.ssbi_handle == NULL ) && 
       ( npa_query_by_name( "/bus/ssbi/modem*" ,
                            0,
                            &query_result ) == NPA_QUERY_SUCCESS ) )
  {

    hdrsrchutil_npa.ssbi_handle = npa_create_sync_client( 
                                   "/bus/ssbi/modem*" ,/* resource name */
                                   "HDR",              /* client name   */
                                   NPA_CLIENT_REQUIRED /* work model    */
                                 );
      /* Create a handle to the /bus/ssbi/modem* resource */

    ASSERT( hdrsrchutil_npa.ssbi_handle != NULL );
      /* Make sure handle is initialized. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR SSBI NPA CREATE CLIENT" );
  }

  /* Additional check to see if the mdsp clk npa node resource is available in the system
     before attempting to use it. */
  if ( ( hdrsrchutil_npa.mdsp_clk_handle == NULL ) && 
       ( npa_query_by_name( "/clk/modem/mdsp" ,
                            0,
                            &query_result ) == NPA_QUERY_SUCCESS ) )
  {

    hdrsrchutil_npa.mdsp_clk_handle = npa_create_sync_client( 
                                      "/clk/modem/mdsp",  /* resource name */
                                      "HDR",              /* client name   */
                                      NPA_CLIENT_REQUIRED /* work model    */
                                      );
      /* Create a handle to the /clk/modem/mdsp resource */

    ASSERT( hdrsrchutil_npa.mdsp_clk_handle != NULL );
      /* Make sure handle is initialized. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR MDSP CLK NPA CREATE CLIENT" );
  }

} /* hdrsrchutil_npa_create_sync_client */

/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_DESTROY_CLIENT()

DESCRIPTION
  This will cancel the client request (if any) and free the client
  structure. The client handle can not be used to issue requests after
  being destroyed

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
  hdrsrchutil_npa_cpu_handle is destroyed

===========================================================================*/

void hdrsrchutil_npa_destroy_client( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchutil_npa.cpu_handle != NULL )
  {
    npa_destroy_client( hdrsrchutil_npa.cpu_handle );
      /* Free client's structure and handle */

    hdrsrchutil_npa.cpu_handle = NULL;
      /* Change to NULL to trigger ASSERT sanity checks if necessary. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR NPA DESTROY CLIENT" );
  }

  /* Destroy HDR's SSBI NPA handle if it exists. */
  if ( hdrsrchutil_npa.ssbi_handle != NULL )
  {
    npa_destroy_client( hdrsrchutil_npa.ssbi_handle );
      /* Free client's structure and handle */

    hdrsrchutil_npa.ssbi_handle = NULL;
      /* Change to NULL to trigger ASSERT sanity checks if necessary. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR SSBI NPA DESTROY CLIENT" );
  }

  /* Destroy HDR's MDSP CLK NPA handle if it exists. */
  if ( hdrsrchutil_npa.mdsp_clk_handle != NULL )
  {
    npa_destroy_client( hdrsrchutil_npa.mdsp_clk_handle );
      /* Free client's structure and handle */

    hdrsrchutil_npa.mdsp_clk_handle = NULL;
      /* Change to NULL to trigger ASSERT sanity checks if necessary. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR MDSP CLK NPA DESTROY CLIENT" );
  }

#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
      hdrsrchutil_npa.max_q6_clk_speed = 0;
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */

} /* hdrsrchutil_npa_destroy_client */


/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_ISSUE_CPU_REQUEST

DESCRIPTION
  The function issues an NPA request for CPU/MIPS

DEPENDENCIES
  hdrsrchutil_npa.cpu_handle must be non-null.

PARAMETERS
  cpu_request     - Bit map of cpu requested.

RETURN VALUE
  Success/Failure - if the request was successful or not.

SIDE EFFECTS
  hdrsrchutil_npa.cpu_mask updated

===========================================================================*/

errno_enum_type hdrsrchutil_npa_issue_cpu_request
(
  uint32          cpu_request
    /* Bitmap of CPU request */
)
{
  errno_enum_type                       status = E_SUCCESS;
    /* Command completion status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT( hdrsrchutil_npa.cpu_handle != NULL );
    /* Make sure handle is initialized. */

  hdrsrchutil_npa.cpu_mask |= cpu_request;

        /* If this request is from cancel_cpu_request, then do not
       increment semaphore because it has already been
       incremented by the caller*/
  if( !hdrsrchutil_npa.hdr_npa_issue_request_from_cancel )
  {
    hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect++;
  }

   /* There should only be one request */
  if( hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect != 1 )
  {
    status = E_FAILURE;
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "Overlapping NPA call!"
                  "Skipping this NPA request (issue_request)");
  }

  else
  {
  
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                    "*** HDR NPA REQUEST %d (New request mask:%d, Outstanding request mask: %d)",
                    hdrsrchutil_npa.cpu_traffic_val,
                    cpu_request,
                    hdrsrchutil_npa.cpu_mask );
  
    /* Only 2 levels of CPU/MIPS are supported.  Order matters in if-else as
       TRAFFIC MIPS has priority over WAKEUP MIPS.
       Check bitmap to determine HDR MIPS requirements */
    if ( hdrsrchutil_npa.cpu_mask & HDRSRCHUTIL_NPA_CPU_TRAFFIC )
    {
      npa_issue_required_request( hdrsrchutil_npa.cpu_handle,
                                  hdrsrchutil_npa.cpu_traffic_val );
        /* Issue request for TRAFFIC CPU/MIPS */
    }
    else if ( hdrsrchutil_npa.cpu_mask & HDRSRCHUTIL_NPA_CPU_WAKEUP )
    {
      npa_issue_required_request( hdrsrchutil_npa.cpu_handle,
                                  hdrsrchutil_npa.cpu_wakeup_val );
        /* Issue request for WAKEUP CPU/MIPS */
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LVL_ERROR,
                      "*** HDR NPA REQUEST mask:%d not supported",
                      cpu_request );
    }
    /* HDR SSBI NPA request.  ssbi_handle might be NULL if it is unsupported,
       so skip the ASSERT check but still check if handle is NULL. */
    if ( hdrsrchutil_npa.ssbi_handle != NULL )
    {
      npa_issue_required_request( hdrsrchutil_npa.ssbi_handle,
                                  HDRSRCHUTIL_SSBI_NPA_REQ_VAL );
        /* Request HDR's SSBI */
    }
  }

   /* Decrement our semaphore as we are out of request */
  if( !hdrsrchutil_npa.hdr_npa_issue_request_from_cancel )
  {
    hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect--;
  }

  return ( status );

} /* hdrsrchutil_npa_issue_cpu_request */


/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_CANCEL_CPU_REQUEST()

DESCRIPTION
  The function cancels an NPA request for CPU/MIPS.

DEPENDENCIES
  hdrsrchutil_npa.cpu_handle must be non-null.

PARAMETERS
  cpu_request     - Bit map of cpu request cancelled.

RETURN VALUE
    Success/Failure - if the request was successful or not. 

SIDE EFFECTS
  hdrsrchutil_npa.cpu_mask updated

===========================================================================*/

errno_enum_type hdrsrchutil_npa_cancel_cpu_request
(
  uint32          cpu_request
    /* Bitmap of CPU request to cancel */
)
{
  errno_enum_type                       status = E_SUCCESS;
    /* Command completion status */

  hdrsrch_sleep_sub_state_enum_type    previous_state;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  previous_state = hdrsrchsleep_get_sleep_sub_state();

  hdrsrchsleep_set_sleep_sub_state( HDRSRCH_IN_NPA_CALL);

  /* Make sure handle is initialized. Since this might be called during hdrtask
     start-up while going to INACTIVE state, no need to err_fatal when the handle
     is NULL. */
  if ( hdrsrchutil_npa.cpu_handle != NULL )
  {
    hdrsrchutil_npa.cpu_mask &= ~cpu_request;
      /* Clear the bit of cancelled request. */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "*** HDR NPA CANCEL (Request mask: %d, Outstanding mask: %d)",
                    cpu_request,
                    hdrsrchutil_npa.cpu_mask );

          /* Not incremented yet, so semaphore should be 0*/
    if( hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect != 0 )
    {
      status = E_FAILURE;
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                    "Overlapping NPA call! "
                    "skipping this NPA call(cancel_request)");
    }
    else
    {
      hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect++;  
      
      if ( hdrsrchutil_npa.cpu_mask != 0 )
      {
        /* Let npa_issue_cpu_request know that semaphore
           has been incrmented.*/
        hdrsrchutil_npa.hdr_npa_issue_request_from_cancel = TRUE;

        /* There is still an outstanding HDR CPU request.  Issue another CPU
           request to change HDR's CPU requirement. */
  
        ( void ) hdrsrchutil_npa_issue_cpu_request( hdrsrchutil_npa.cpu_mask );
          /* Request HDR CPU/MIPS change. */

         /* decrement semaphore as we are out of critical section */
        hdrsrchutil_npa.hdr_npa_issue_request_from_cancel = FALSE;

      }
      else
      {
        /* No outstanding HDR CPU requests */
  
        npa_cancel_request( hdrsrchutil_npa.cpu_handle );
          /* Cancel request for HDR's CPU */
      }

      /* Make sure SSBI NPA handle is initialized. */
      if ( hdrsrchutil_npa.ssbi_handle != NULL )
      {
        npa_cancel_request( hdrsrchutil_npa.ssbi_handle );
        /* Cancel request for HDR's SSBI */
      }

      hdrsrchutil_npa.hdr_npa_cpu_node_debug_crtic_sect--;

    }
  }

  hdrsrchsleep_set_sleep_sub_state (previous_state);

  return ( status );

} /* hdrsrchutil_npa_cancel_cpu_request */

/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_ISSUE_MDSP_CLK_REQUEST

DESCRIPTION
  The function issues an NPA request for MDSP CLK SPEED

DEPENDENCIES
  hdrsrchutil_npa.mdsp_clk_handle must be non-null.

PARAMETERS
  speed_mhz     - MDSP Clock speed in MHz.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_npa_issue_mdsp_clk_request
(
  hdrsrchutil_npa_mdsp_clk_type         speed_mhz
    /* MDSP Clock speed in MHz */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* HDR MDSP CLK NPA request.  mdsp_clk_handle might be NULL if it is
     unsupported.  Skip request if the same as current clk speed. */
  if ( ( hdrsrchutil_npa.mdsp_clk_handle != NULL ) &&
       ( hdrsrchutil_npa.curr_mdsp_clk_speed != speed_mhz ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "*** HDR MDSP CLK NPA REQUEST %d Mhz",
                    speed_mhz );

    if( hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect != 0 )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                    "Overlapping MDSP CLK NPA call! "
                    "skipping this NPA call(issue_request)");
      return;
    }

    hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect++;

    npa_issue_required_request( hdrsrchutil_npa.mdsp_clk_handle,
                                speed_mhz );
      /* Request update to HDR's MDSP CLK speed */

    hdrsrchutil_npa.curr_mdsp_clk_speed = speed_mhz;
      /* Save current mdsp clk speed requested */

    hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect--;

  }

} /* hdrsrchutil_npa_issue_mdsp_clk_request */


/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_CANCEL_MDSP_CLK_REQUEST()

DESCRIPTION
  The function cancels an NPA request for MDSP CLK.

DEPENDENCIES
  hdrsrchutil_npa.mdsp_clk_handle must be non-null.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_npa_cancel_mdsp_clk_request
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Make sure MDSP CLK NPA handle is initialized. */
  if ( hdrsrchutil_npa.mdsp_clk_handle != NULL )
  {
    if( hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect != 0 )
    {

      HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                    "Overlapping MDSP CLK NPA call! "
                    "skipping this NPA call(cancel_request)");
      return;
    }

    hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect++;
    npa_cancel_request( hdrsrchutil_npa.mdsp_clk_handle );
      /* Cancel request for HDR's MDSP CLK */

    hdrsrchutil_npa.curr_mdsp_clk_speed = 0;
      /* Reset saved current mdsp clk speed. */

    hdrsrchutil_npa.hdr_npa_mdsp_clk_node_debug_crtic_sect--;

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "*** HDR MDSP CANCEL CLK NPA REQUEST" );
  }

} /* hdrsrchutil_npa_cancel_mdsp_clk_request */

#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_GET_MAX_Q6_CLK_SPEED()

DESCRIPTION
  The function gets the Max Q6 supported frequency.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Max Q6 supported frequency

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchutil_npa_get_max_q6_clk_speed
(
  void
)
{
   return hdrsrchutil_npa.max_q6_clk_speed;
} /* hdrsrchutil_npa_get_max_q6_clk_speed */
#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */

#endif /* FEATURE_NPA */

/*==========================================================================

FUNCTION        HDRSRCHUTIL_GET_RUP_CHAN

DESCRIPTION
  This function returns the channel being used for RUPrequest/PilotMeasReq/
  LTEPilotMeasReq.
  
DEPENDENCIES
  None

RETURN VALUE
  channel used for RUP

SIDE EFFECTS
  The channel when system loss occured.

==========================================================================*/

sys_channel_type hdrsrchutil_get_rup_chan( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return  ( hdrsrch_rup_req.channel );

}  /* hdrsrchutil_get_rup_chan */

/*==========================================================================

FUNCTION        HDRSRCHUTIL_IS_BRUTE_FORCE_ACQ

DESCRIPTION
  This function returns whether acquisition is a normal acq or brute-force
  acq
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE    :      it is a brute-force acq
  FALSE   :      it is a normal acq

SIDE EFFECTS
  The channel when system loss occured.

==========================================================================*/

boolean hdrsrchutil_is_brute_force_acq( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return  ( hdrsrch.brute_force_params.is_brute_force_acq );
}  /* hdrsrchutil_is_brute_force_acq */

/* <EJECT> */
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_ADJUST_WINDOW_SIZE

DESCRIPTION  : This function adjusts the search window size according to a 
               variable from hdrsrchreacq 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Window size of search list might be altered.

=========================================================================*/
void hdrsrchutil_adjust_window_size
(
  uint8                     win_size_fctr,
   /* adjustment to the window size */

  hdrsrch_winsize_enum_type max_win_size,
   /* maximum search window size, in enum */

  hdrsrch_winsize_enum_type min_win_size
   /* minimum search window size, in enum */
)
{
  uint16            adjusted_winsize_x1;
    /* Window size after adjustment */

  uint16            max_winsize_x1;
    /* Maximum window size, in chips */

  uint16            min_winsize_x1;
    /* Minimum window size, in chips */

  uint8             i;
    /* loop counter*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  max_winsize_x1 = hdrsrchutil_winsize[ max_win_size ];

  for ( i = 0; i < hdrsrchutil.srch.cnt; i++ )
  {     
      adjusted_winsize_x1 = hdrsrchutil.srch.list[i].srch_task.win_size 
                               * win_size_fctr;
        /* adjusted window size based on sector information */ 

     if( hdrsrchutil.srch.list[i].srch_task.win_size >
         hdrsrchutil_winsize[ min_win_size ] )
     {
       min_winsize_x1 = hdrsrchutil.srch.list[i].srch_task.win_size;
     }
     else
     {
       min_winsize_x1 = hdrsrchutil_winsize[ min_win_size ];
     }
      /* make sure window size is the bigger between the value issued
         by the sector and the one we specify */
                     
     if( adjusted_winsize_x1 < min_winsize_x1 )
     {
       hdrsrchutil.srch.list[i].srch_task.win_size = min_winsize_x1;
     }
       /* if window size will be too small after adjustment */  

     else if( adjusted_winsize_x1 > max_winsize_x1 )
     {
       hdrsrchutil.srch.list[i].srch_task.win_size = max_winsize_x1;
     }
       /* if window size will be too large */
     else
     {
       hdrsrchutil.srch.list[i].srch_task.win_size = adjusted_winsize_x1;
     }
     
     hdrsrchutil.srch.list[i].sect_ptr->win_sizex1 = 
                                  hdrsrchutil.srch.list[i].srch_task.win_size;
      /* update the sector win size */
  }
}


/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESTORE_ASET_WIN_SIZE

DESCRIPTION  : This function restores the aset win size. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Window size of search list might be altered.

=========================================================================*/
void hdrsrchutil_restore_aset_win_size( void )
{

  HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, 
                 "Restored aset win size, old %d and saved %d",
                 hdrsrch.srch_params.aset_win_size,
                 hdrsrch.saved_aset_win_size );
               
  hdrsrch.srch_params.aset_win_size = hdrsrch.saved_aset_win_size;
} /* hdrsrchutil_restore_aset_win_size*/


/*=========================================================================

FUNCTION     : HDRSRCHUTIL_SAVE_AND_SET_ASET_WIN_SIZE

DESCRIPTION  : This function saves the current aset win size and set to new one. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Window size of search list might be altered.

=========================================================================*/
void hdrsrchutil_save_and_set_aset_win_size
(
  hdrsrch_winsize_enum_type new_win_size
    /* Aset win size */
)
{

  HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, 
                 "Saved aset win size %d and set new win size %d",
                 hdrsrch.srch_params.aset_win_size,
                 new_win_size );
  
  hdrsrch.saved_aset_win_size       = hdrsrch.srch_params.aset_win_size;
  hdrsrch.srch_params.aset_win_size = new_win_size;
} /* hdrsrchutil_save_and_set_aset_win_size */

#ifdef FEATURE_HDR_REVC
/*===========================================================================

FUNCTION HDRSRCHUTIL_UPDATE_LOADADJ_PER_SUBASET()

DESCRIPTION
  The function sends loadadj to FW on a single subaset

DEPENDENCIES
  None

PARAMETERS
  loadadj_ptr     - LoadAdjust to send

RETURN VALUE
  E_SUCCESS       - LoadAdj successfully updated
  E_FAILURE       - Serving sector not in ASET, usually because
                    ASET has changed.

SIDE EFFECTS
  None

===========================================================================*/
LOCAL errno_enum_type hdrsrchutil_update_loadadj_per_subaset
(
  const hdrsrch_loadadj_subaset_type* loadadj_ptr
)
{
  uint8 i, j, k;
  uint8 serving_idx;
  errno_enum_type err = E_FAILURE;
  int8 nlb_k;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  nlb_k = loadadj_ptr->disable_nlb ? 
          HDRSRCHUTIL_NLB_BACKOFF_RATIO : 
          HDRSRCHUTIL_NLB_BACKOFF_DISABLED;

  for ( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    /* assuming members of a subaset have same carrier,
       find matching subaset for this loadadj update */
    if ( loadadj_ptr->serving_sector.channel.band ==
         hdrsrchset.subaset[i].channel.band &&
         loadadj_ptr->serving_sector.channel.chan_num ==
         hdrsrchset.subaset[i].channel.chan_num )
    {
      err = E_FAILURE;

      /* check if serving sector of NLB is part of ASET */
      for ( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
      {
        if ( hdrsrchset.subaset[i].aset[j]->pn_offset ==
             loadadj_ptr->serving_sector.pilot_pn)
        {
          hdrsrchset.subaset[i].aset[j]->aset.loadadj = 
              loadadj_ptr->loadadj_serving;

          HDR_MSG_SRCH_6(MSG_LEGACY_MED, 
                         "update_loadadj serving ch %d pn %d k %d load %d @ %d/%d",
                         hdrsrchset.subaset[i].channel.chan_num,
                         hdrsrchset.subaset[i].aset[j]->pn_offset,
                         nlb_k,
                         loadadj_ptr->loadadj_serving,
                         i, j);

          /* remember ASP index for serving sector */
          serving_idx = j;

          err = E_SUCCESS;
          
          break;
        }
      }

      /* AT should ignore NLB received on a sector that is not part of 
         current ASET. This can happen if ASET is updated */
      if ( err != E_SUCCESS )
      {
        break;
      }

      hdrsrchset.subaset[i].NLB_K = nlb_k;

      for ( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++  )
      {
        /* loadadj on serving sector already updated */
        if ( j == serving_idx )
        {
          continue;
        }

        for ( k=0; k<loadadj_ptr->loadadj_cnt; k++ )
        {
          if ( hdrsrchset.subaset[i].aset[j]->pn_offset ==
               loadadj_ptr->pn[k] )
          {
            hdrsrchset.subaset[i].aset[j]->aset.loadadj = 
                loadadj_ptr->loadadj[k];

            HDR_MSG_SRCH_6(MSG_LEGACY_MED, "update_loadadj ch %d pn %d load %d @ %d/%d/%d",
                           hdrsrchset.subaset[i].channel.chan_num,
                           hdrsrchset.subaset[i].aset[j]->pn_offset,
                           loadadj_ptr->loadadj[k], i, j, k);
            break;
          }
        }
        
        /* if ASET not included in load info update, use serving sector */
        if ( k >= loadadj_ptr->loadadj_cnt )
        {
          hdrsrchset.subaset[i].aset[j]->aset.loadadj = 
              loadadj_ptr->loadadj_serving;

          HDR_MSG_SRCH_5(MSG_LEGACY_MED, "apply serving loadadj ch %d pn %d load %d @ %d/%d",
                         hdrsrchset.subaset[i].channel.chan_num,
                         hdrsrchset.subaset[i].aset[j]->pn_offset,
                         loadadj_ptr->loadadj_serving, i, j);
        }
      }   
      /* Only one subaset should match given NLB. Exit the loop */
      break;
    }
  }

  return err;
}

/*===========================================================================

FUNCTION HDRSRCHUTIL_UPDATE_LOADADJ()

DESCRIPTION
  The function sends loadadj to FW for up to 3 carriers

DEPENDENCIES
  None

PARAMETERS
  loadadj_ptr     - LoadAdjust to send

RETURN VALUE
  E_SUCCESS       - LoadAdj successfully updated for all subaset
  E_FAILURE       - Error while updating any subaset


SIDE EFFECTS
  None

===========================================================================*/
errno_enum_type hdrsrchutil_update_loadadj
(
  const hdrsrch_loadadj_type* loadadj_ptr
)
{
  uint8 i;
  errno_enum_type err = E_SUCCESS;
  hdrsrch_state_enum_type state = hdrsrch_get_current_state();
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i=0; i<loadadj_ptr->subaset_cnt; i++ )
  {
    err |= hdrsrchutil_update_loadadj_per_subaset(&loadadj_ptr->subaset[i]);
  }

  if ( state == HDRSRCH_IDLE_STATE ||
       state == HDRSRCH_CONNECTED_STATE )
  {
    hdrsrchutil_aset_commit_to_dsp();
  }
  else
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_MED, "update_loadadj %d not idle/tc, delayed",
                   state);
  }

  return err;
}

/*===========================================================================

FUNCTION HDRSRCHUTIL_RESET_ALL_LOADADJ()

DESCRIPTION
  The function resets loadadj for all ASET members to 0. Per FW design,
  this is needed when NLB is disabled.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_reset_all_loadadj( void )
{
  uint8 i, j;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( i=0; i<hdrsrchset.subaset_cnt; i++ )
{
    hdrsrchset.subaset[i].NLB_K = HDRSRCHUTIL_NLB_BACKOFF_DISABLED;

    for ( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      hdrsrchset.subaset[i].aset[j]->aset.loadadj = 0;
    }
  }
}

/*===========================================================================

FUNCTION HDRSRCHUTIL_ENABLE_NLB()

DESCRIPTION
  The function enable/disable NLB.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_enable_nlb
(
  boolean enabled
)
{
  hdrsrch_state_enum_type state = hdrsrch_get_current_state();
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchutil.demod_all_carriers != enabled )
  {
    hdrsrchutil.demod_all_carriers = enabled;
    if ( !enabled )
    {
      hdrsrchutil_reset_all_loadadj();
    }

    if ( state == HDRSRCH_IDLE_STATE ||
         state == HDRSRCH_CONNECTED_STATE )
    {
      hdrsrchutil_aset_commit_to_dsp();
    }
    else
    {
      HDR_MSG_SRCH_2(MSG_LEGACY_MED, "enable_nlb %d not idle/tc, delayed",
                     enabled, state);
    }
  }
}
#endif /* FEATURE_HDR_REVC */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
/*=========================================================================

FUNCTION     : HDRSRCHUTIL_RESET_PREV_WAKEUP_INFO

DESCRIPTION  : This function resets all prev wakeup info to default value.
               

DEPENDENCIES : None.

INPUT        : None.

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchutil_reset_prev_wakeup_info( void )
{
   int i;
   /* Temp loop variable */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Reset current wakeup counts */
  hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed =  FALSE;
  hdrsrch.idle_state_div.wakeup_info.slot_index = 0;
  hdrsrch.idle_state_div.wakeup_info.num_slots =  0;
  HDRMDSP_CLEAR_MISSED_SYNC_CC_FLAG();
  hdrsrch.idle_state_div.wakeup_info.prev_sleep_time = 0;
  hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg = 
     HDRSRCHUTIL_IDLE_STATE_DIV_PSINR_DEFAULT_VALUE;
  
  for(i = 0; i < HDRSRCH_MAX_SLOTS_PSINR_PER_IDLE_WAKEUP; i++)
  {
     hdrsrch.idle_state_div.wakeup_info.psinr_per_slot[i] = 0;
  }

  hdrsrch.idle_state_div.wakeup_info.reacq_failed = FALSE;

  if( hdrsrch_state_id == HDRSRCH_NO_STATE )
  {
    hdrsrch.idle_state_div.wakeup_info.div_enabled = FALSE;
  }

  hdrsrch.idle_state_div.wakeup_info.div_timeout= FALSE;
} /* hdrsrchutil_reset_prev_wakeup_info*/
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
/*===========================================================================

FUNCTION HDRSRCHUTIL_REPORT_COEX_POWER

DESCRIPTION
  This function sends coex rx tx power to MCS.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_report_coex_power
( 
   timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{
  hdrmultirat_report_coex_power( FALSE ); 
    /* Save power and send to MCS */

  timer_set( &hdrsrch.coex_timer, HDRMULTIRAT_COEX_PWR_TIMER_MS,
                0, T_MSEC );
    /* Set the timer to go off again after 20 ms . */
  
} /* hdrsrchutil_report_coex_power */

#ifdef FEATURE_HDR_BAND_AVOIDANCE
/*===========================================================================

FUNCTION HDRSRCHUTIL_SAVE_BA_POWER

DESCRIPTION
  This function posts a command to HDR SRCH to save power for band avoidance.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_save_ba_power
( 
   timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

  hdrsrch_cmd_type                *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd = hdrsrchcmd_new( HDRSRCH_BAND_AVOID_TIMER_CMD );
    /* Post Band avoidance timer expired command */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }
} /* hdrsrchutil_save_ba_power */
#endif /* FEATURE_HDR_BAND_AVOIDANCE */

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

/*===========================================================================

FUNCTION HDRSRCHUTIL_DO_ATOMIC_SEARCH

DESCRIPTION
  This function returns whether to do atomic search or not

DEPENDENCIES
  None

RETURN VALUE
  TRUE: if to do atomic search
  FALSE: if we can reschedule searches.

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchutil_do_atomic_search( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Only do atomic search for reacq and irat searches rest all can
     be resceduled in between G */
  if ( ( hdrsrch_get_current_state() == HDRSRCH_REACQ_STATE ) ||
       ( hdrsrch_get_current_state() == HDRSRCH_OFS_MEAS_STATE )  )
  {
     HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "Submitting Atomic search" );
    return TRUE;
   
  }

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                    "Submitting Non-atomic search" );
  return FALSE;
} /* hdrsrchutil_do_atomic_search */

#ifdef FEATURE_HDR_DLNA
/*===========================================================================

FUNCTION HDRSRCHUTIL_SET_DLNA_ENABLE

DESCRIPTION
  This function sets internal flag that is checked to enable/disable DLNA. 
 
  Note: The function is called by CMAC (HDRRX task ) on powerup to set DLNA
  enable flag. The enable flag is used in HDRRX and HDRSRCH tasks.  Since
  the setting and the access occurs at two different times ( setting at
  powerup vs access post-sync ), there should not be any race condition 
  while accessing hdrsrchutil.dlna_enabled.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_set_dlna_enable
(
  boolean enable
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.dlna_enabled = enable;
    /* Set DLNA feature enabled flag */

  if ( hdrsrchutil.dlna_enabled == FALSE ) 
  {
    hdrsrchutil.enh_dlna_enabled = FALSE;
      /* Force Enhanced DLNA feature enabled flag to FALSE if DLNA is not enabled */
  }

} /* hdrsrchutil_set_dlna_enable( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_SET_ENH_DLNA_ENABLE

DESCRIPTION
  This function sets internal flag that is checked to enable/disable Enhanced
  DLNA feature. Enhanced DLNA can only be enabled if DLNA is enabled.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_set_enh_dlna_enable
(
  boolean enable
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil.enh_dlna_enabled = FALSE;
    /* Initialize Enhanced DLNA feature enabled flag. */

  if ( ( enable ) && ( hdrsrchutil.dlna_enabled == TRUE ) )
  {
    hdrsrchutil.enh_dlna_enabled = TRUE;
  }

} /* hdrsrchutil_set_enh_dlna_enable( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_DLNA_IS_ENABLED

DESCRIPTION
  This function returns enable/disable status of DLNA feature. The feature
  can be turned on/off via NV.
 
  Note: The function is executed from caller task context and is expected
  to be called from HDRRX and HDRSRCH tasks.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if DLNA feature is enabled
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchutil_dlna_is_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "D-LNA enabled=%d, Enhanced D-LNA enabled=%d",
                  hdrsrchutil.dlna_enabled, hdrsrchutil.enh_dlna_enabled );
  return hdrsrchutil.dlna_enabled;
    /* Return DLNA feature enabled/disabled status */

} /* hdrsrchutil_dlna_is_enabled( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_ENH_DLNA_IS_ENABLED

DESCRIPTION
  This function returns enable/disable status of Enhanced DLNA feature.
  The feature can be turned on/off via NV. Enhanced DLNA can only be enabled
  if DLNA is enabled.
 
  Note: The function is executed from caller task context and is expected
  to be called from HDRRX and HDRSRCH tasks.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if DLNA feature is enabled
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchutil_enh_dlna_is_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchutil.enh_dlna_enabled;
    /* Return Enhanced DLNA feature enabled/disabled status */

} /* hdrsrchutil_enh_dlna_is_enabled( ) */

/*===========================================================================

FUNCTION HDRSRCHUTIL_SEND_DLNA_CTL

DESCRIPTION
  This function sends D-LNA enable/disable command to FW.
  
  Note: The function is executed from caller task context and is expected
  to be called from HDRRX and HDRSRCH tasks.
 
DEPENDENCIES 
  None. 

PARAMETERS
  enable - TRUE  => Enable D-LNA
         - FALSE => Disable D-LNA

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_send_dlna_ctl
(
  boolean  enable
)
{
  boolean  enh_dlna_enable;
    /* Set to TRUE if Enhanced DLNA is enabled */

  if( hdrmdsp_check_dsp_clk() )
  {
    /* Initialize and populate the FW message. */
    memset ( &hdrsrchmsg.dlna_ctl_msg, 
             0, 
             sizeof( hdrfw_dlna_ctl_msg_t ) );
      /* Clear the message. */

    msgr_init_hdr ( &hdrsrchmsg.dlna_ctl_msg.hdr, 
                    MSGR_HDR_SRCH, 
                    HDR_FW_DLNA_CTL_CMD );
      /* Initialize message header. */

    enh_dlna_enable = hdrsrchutil_enh_dlna_is_enabled( );
      /* FW also needs to know whether or not Enhanced D-LNA is enabled */

    /* Populate the message. */
    if( enable )
    {
      hdrsrchmsg.dlna_ctl_msg.enable |= HDRSRCHUTIL_DLNA_CTL_MSG_ENABLE_DLNA;
      if ( enh_dlna_enable )
      {
        hdrsrchmsg.dlna_ctl_msg.enable |= 
          HDRSRCHUTIL_DLNA_CTL_MSG_ENABLE_ENH_DLNA;
      }
  
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "D-LNA enabled=%d, Enhanced D-LNA enabled=%d,DLNA Ctl_msg=%d",
                      enable, enh_dlna_enable, hdrsrchmsg.dlna_ctl_msg.enable );
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D-LNA disabled, Enhanced D-LNA disabled,DLNA Ctl_msg=%d",
                      hdrsrchmsg.dlna_ctl_msg.enable );
    }

    hdrsrchmsg_send_msg( &hdrsrchmsg.dlna_ctl_msg,
                         sizeof( hdrfw_dlna_ctl_msg_t ) );
      /* Send the message. */  
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "D-LNA control msg not sent: FW clocks not enabled" );
  }

} /* hdrsrchutil_send_dlna_ctl( ) */


/*===========================================================================

FUNCTION HDRSRCHUTIL_SEND_EARLY_PREAM_CTL

DESCRIPTION
  This function sends early preamble enable/disable command to FW.
  
  Note: The function is executed from caller task context and is expected
  to be called from HDRRX and HDRSRCH tasks.
 
DEPENDENCIES 
  None. 

PARAMETERS
  enable - TRUE  => Enable early preamble decodes
         - FALSE => Disable early preamble decodes

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_send_early_pream_ctl
(
  boolean  enable
)
{
  if( hdrmdsp_check_dsp_clk() )
  {
    /* Initialize and populate the FW message. */
    memset ( &hdrsrchmsg.early_pream_ctl_msg, 
             0, 
             sizeof( hdrfw_early_pream_ctl_msg_t ) );
      /* Clear the message. */

    msgr_init_hdr ( &hdrsrchmsg.early_pream_ctl_msg.hdr, 
                    MSGR_HDR_SRCH, 
                    HDR_FW_EARLY_PREAM_CTL_CMD );
      /* Initialize message header. */
  
    /* Populate the message. */
    if( enable )
    {
      hdrsrchmsg.early_pream_ctl_msg.enable = 1;

      HDR_MSG_SRCH( MSG_LEGACY_LOW,
                    "Early preamble detection enabled" );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_LOW,
                    "Early preamble detection disabled" );
    }

    hdrsrchmsg_send_msg( &hdrsrchmsg.early_pream_ctl_msg,
                         sizeof( hdrfw_early_pream_ctl_msg_t ) );
      /* Send the message. */  
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "Early preamble ctl msg not sent: FW clocks not enabled" );
  }

} /* hdrsrchutil_send_early_pream_ctl( ) */


/*===========================================================================

FUNCTION HDRSRCHUTIL_SCHEDULE_ODD_PREAM_DET

DESCRIPTION
  This function sends message to FW to schedule odd preamble detect at 
  requested slot for the requested duration.
  
  Note: The function is executed from caller task context and is expected
  to be called from HDRRX and HDRSRCH tasks.
 
DEPENDENCIES 
  None. 

PARAMETERS
  start_slot - Slot with the current CC cycle where odd preamble detection 
               is requested
  duration   - Duration in number of slots for odd preamble detection

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchutil_schedule_odd_pream_det
(
  uint8  start_slot,
  uint8  duration  
)
{
  uint16 end_slot = start_slot + duration - 1;
    /* last slot for requested odd preamble detection */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                  "Odd pream detect: start slot=%d, duration=%d",
                  start_slot,
                  duration );

  if( hdrmdsp_check_dsp_clk() )
  {
    if ( ( start_slot >= 4 ) || ( end_slot >= 4 ) )
    {
      /* Initialize and populate the FW message. */
      memset ( &hdrsrchmsg.odd_pream_detect, 
               0, 
               sizeof(hdrfw_force_odd_pream_msg_t) );
        /* Clear the message. */
       /* klockwork error :: Array '&hdrsrchmsg.odd_pream_detect' of  
           size 20 may use index value(s) 0..117  */

      msgr_init_hdr ( &hdrsrchmsg.odd_pream_detect.hdr, 
                      MSGR_HDR_SRCH, 
                      HDR_FW_FORCE_ODD_PREAM_CMD );
        /* Initialize message header. */

      hdrsrchmsg.odd_pream_detect.startSlotTime = start_slot;
      hdrsrchmsg.odd_pream_detect.slotDuration = duration; 
        /* Populate fields to schedule odd preamble detection. */

      hdrsrchmsg_send_msg( &hdrsrchmsg.odd_pream_detect, 
                           sizeof( hdrfw_force_odd_pream_msg_t ) );
        /* Send the message. */  
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
        "Odd pream det msg not sent: start/end slots within first 4 slots" );
    }

  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "Odd preamble detect msg not sent: FW clocks not enabled" );
  }

} /* hdrsrchutil_schedule_odd_pream_det( ) */

#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*===========================================================================

FUNCTION HDRSRCHTC_SUBMIT_DELAYED_SEARCH

DESCRIPTION
  This schedules a search after given delay

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_submit_delayed_search
( 
  uint8 delay
)
{
  if ( hdrsrchdrv_is_search_active() )
  {
    hdrsrchdrv_abort_search(FALSE);
      /* ensure that all the searches are aborted */

    /* Cancel and schedule new search only when past search is active */
    HDRSRCH_SIG_CLR(HDRSRCH_SCHEDULE_SIG);

    /* set timer to delay value. when timer expire, search will be submitted */
    rex_set_timer(&hdrsrch.schedule_timer, delay);
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "No active searches and hence ignoring delayed search");
  }
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */
#ifdef FEATURE_HDR_QUICK_IDLE
/*===========================================================================

FUNCTION HDRSRCHUTIL_DISABLE_QPCH()

DESCRIPTION
  The function disables QPCH detection.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchutil_disable_qpch( void )
{
  if ( hdrmdsp_check_dsp_clk() )
  {
    hdrsrchmsg.qpch_msg.qpchMACIndex = 0;

    hdrsrchmsg.qpch_msg.qForcePreamHSTR = 0;

    hdrsrchmsg.qpch_msg.qForcePreamDetect = 0;
     /* Disable QPCH forced preamble detection */

    hdrsrchmsg_send_msg( &hdrsrchmsg.qpch_msg, 
                         sizeof( hdrfw_qpch_msg_t ) );

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Stop QPCH detect");
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Cannot stop QPCH w/ mdsp clk off" );
  }
}
#endif /* FEATURE_HDR_QUICK_IDLE */

/*===========================================================================

FUNCTION HDRSRCHUTIL_CHK_TRIAGE_FULL_PN()

DESCRIPTION
  The function should be called after a full PN search constructed by
  hdrsrchutil_chk_convert_to_full_pn(). This function will pick
  the best peaks and assign back to the first search task.

DEPENDENCIES
  None

PARAMETERS
  None
              
RETURN VALUE
  None

SIDE EFFECTS
  Search tasks will be set to 1

===========================================================================*/
void hdrsrchutil_chk_triage_full_pn( void )
{
  srch4_srch_res_struct_type best;
  uint16 i, j, k;
  uint16 picked_j, picked_k, dist;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( !HDRSRCH_PILOT_MEAS_PN_CIRCLE_SEARCH_NV_ENABLE ) 
#ifdef FEATURE_HDR_RUMI
        && ( !hdrsrchutil_full_pn_search )
#endif /* FEATURE_HDR_RUMI */
     )
  {
    return;
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "Triaging FullPN %d srch for %d best peaks", 
                  hdrsrchutil.srch.cnt, SRCH4_NUM_PEAKS);

  /* search for i-th best peak */
  for (i=0; i<SRCH4_NUM_PEAKS; i++)
  {
    picked_j = picked_k = 0;

    best.eng[i] = hdrsrchutil.srch.res[0].eng[0];

    /* for each search result */
    for (j=0; j<hdrsrchutil.srch.cnt; j++)
    {    
      /* for each peak within a search */
      for (k=0; k<SRCH4_NUM_PEAKS; k++)
      {
        if ( hdrsrchutil.srch.res[j].eng[k] == 0 )
        {
          /* already picked this peak, skip to next */
          continue;
        }

        if ( best.eng[i] < hdrsrchutil.srch.res[j].eng[k] )
        {
          if ( ( i != 0 ) &&
               ( hdrsrchutil_full_pn_max_dist != 0 ) )
          {
            /* Check if this peak is too far from the strongest */
            dist = ( best.posx2[0] - hdrsrchutil.srch.res[j].posx2[k] ) &
                   HDR_PN_CIRC_MASK_X2;

            if ( dist > hdrsrchutil_full_pn_max_dist )
            {
              HDR_MSG_SRCH_7( MSG_LEGACY_LOW, 
                              "FullPn: dist %d between srch[%d].peak[%d]:%d@%d and best[0]@%d > %d", 
                              dist, j, k, 
                              hdrsrchutil.srch.res[j].eng[k],
                              hdrsrchutil.srch.res[j].posx2[k],
                              i, hdrsrchutil_full_pn_max_dist );
              continue;
            }
          }

          HDR_MSG_SRCH_6( MSG_LEGACY_LOW, 
                          "FullPn: srch[%d].peak[%d]:%d@%d > best[%d]:%d", 
                          j, k, hdrsrchutil.srch.res[j].eng[k], 
                          hdrsrchutil.srch.res[j].posx2[k],
                          i, best.eng[i] );

          best.eng[i] = hdrsrchutil.srch.res[j].eng[k];

          picked_j = j;
          picked_k = k;
        }

        /* peaks are pre-sorted, the rest of the peaks won't be better
           than this one */
        break;
      }
      /* for each peak within a search */
    }
    /* for each search result */

    best.posx2[i] = hdrsrchutil.srch.res[picked_j].posx2[picked_k];
    best.eng_int[i] = hdrsrchutil.srch.res[picked_j].eng_int[picked_k];
    best.posx8_int[i] = hdrsrchutil.srch.res[picked_j].posx8_int[picked_k];
    best.ecio_1x_linear = hdrsrchutil.srch.res[picked_j].ecio_1x_linear;
    best.ecio_1x_db = hdrsrchutil.srch.res[picked_j].ecio_1x_db;
    
    HDR_MSG_SRCH_5( MSG_LEGACY_MED, 
                    "FullPn: best[%d] <= srch[%d].peak[%d]:%d@%d",
                    i, picked_j, picked_k, 
                    best.eng[i],
                    best.posx2[i] );

    hdrsrchutil.srch.res[picked_j].eng[picked_k] = 0;
  }
  /* for i-th best peak */

  /* Found the best peaks from the whole PN circle, map it back
     to the first search */
  memset( hdrsrchutil.srch.res, 0, 
          sizeof(hdrsrchutil.srch.res[0])* hdrsrchutil.srch.cnt );
  
  hdrsrchutil.srch.res[0] = best;

  hdrsrchutil.srch.cnt = 1;
  hdrsrchdrv.list_cnt = 1;
  hdrsrchdrv.carrier_info[0].srch_task_cnt = 0;
  hdrsrchdrv.carrier_info[1].srch_task_cnt = 0;
  hdrsrchdrv.carrier_info[2].srch_task_cnt = 0;
  hdrsrchdrv.carrier_info[hdrsrchutil.srch.list[0].srch_task.carrier_idx].srch_task_cnt = 1;
}

#ifdef FEATURE_HDR_JOLOKIA_MODEM_BRINGUP
/*===========================================================================

FUNCTION HDRSRCHUTIL_NPA_QUERY_MAX_Q6_CLK_SPEED()

DESCRIPTION
  The function gets the Max Q6 supported frequency using NPA API and stores
  in local data structure.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_npa_query_max_q6_clk_speed( void )
{
   npa_query_type      query_result;

   //get MAX Q6 supported clk speed through NPA
   if ( npa_query_by_name("/clk/cpu", NPA_QUERY_RESOURCE_MAX, 
                         &query_result) == NPA_QUERY_SUCCESS )
   {
     if ( query_result.type == NPA_QUERY_TYPE_STATE )
     {
       hdrsrchutil.max_q6_clk_speed = query_result.data.state;
       HDR_MSG_SRCH_1(MSG_LEGACY_HIGH,"NPA MAX clk freq  %d", 
                    hdrsrchutil.max_q6_clk_speed);
     }
     else
     {
       HDR_MSG_SRCH_1(MSG_LEGACY_ERROR,"NPA max clk freq query returned %d", 
                    query_result.type);
     }
   }
   else
   {
     HDR_MSG_SRCH(MSG_LEGACY_HIGH,"NPA MAX clk freq reading failed");
   }
} /* hdrsrchutil_npa_query_max_q6_clk_speed */

/*===========================================================================

FUNCTION HDRSRCHUTIL_GET_MAX_Q6_CLK_SPEED()

DESCRIPTION
  The function gets the Max Q6 supported frequency from locally stored data
  structure.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Max Q6 supported frequency

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrsrchutil_get_max_q6_clk_speed( void )
{
   return hdrsrchutil.max_q6_clk_speed;
} /* hdrsrchutil_get_max_q6_clk_speed */

/*===========================================================================

FUNCTION HDRSRCHUTIL_RESET_MAX_Q6_CLK_SPEED()

DESCRIPTION
  The function resets the value to ZERO (called on srchst powerdown).

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchutil_reset_max_q6_clk_speed( void )
{
   hdrsrchutil.max_q6_clk_speed = 0;
} /* hdrsrchutil_reset_max_q6_clk_speed */

#endif /* FEATURE_HDR_JOLOKIA_MODEM_BRINGUP */
