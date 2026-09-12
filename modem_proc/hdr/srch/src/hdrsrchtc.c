/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

              H D R    S R C H    C O N N E C T E D   S T A T E

                   Search Connected (Traffic Channel) Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000-2021 by Qualcomm Technologies, Incorporated.  All Rights Reserved. 

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=

=============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchtc.c#4 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------- 
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
04/13/2020   svu     FR 55482: Extension of elevator mode to C2K RAT
12/04/2019   vaa     Changes to support clk plan V2.0 FR changes
09/20/2018   vke     Changes to update the FMAC subtype to fw
07/16/2018   vke     Convert QTA to LTA for GSM powerscan QTA timeline miss.
05/14/2018   vke     Changes to dis-allow QTA procedure when PA is OFF
02/07/2017   vke     FR36756 Changes to update TRM for APS state change
02/05/2018   vke     Fix for ASdiv traffic switch failure
01/24/2018   vke     Added hdrsrchtc_is_connection_close_in_progress().
01/24/2018   vke     Changes to resume txd in traffic only after rf tune
01/24/2018   vke     Skip ASDiv switch when OFS is about to start 
09/04/2017   vko     Force LTA if LTE subs is not present on DO sub (temp DDS)
08/24/2017   rmv     Corrected the rxlm buffer to be de-allocated
08/24/2017   rmv     DO+W QTA support 
08/24/2017   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM  
12/16/2016   svu     Fixed Klocwork Issues
07/04/2016   vko     Fixed KW issue
06/16/2016   vko     Reduce excessive F3
05/16/2016   vko     Reduce excessive F3
04/28/2016   vke     Changes to cancel QTA if PA is OFF
01/20/2016   wsh     Fixed incorrect ARD/ASDiv transistion involving 4d state
12/22/2015   rmv     Allow system lost cmd while sys loss is pending 
12/07/2015   wsh     Block all but deactivate cmd when sys loss is pending
11/26/2015   rmv     For QTA, fixed the order of Div disable, QTA start to FW 
                     and set_client to TRM
11/18/2015   rmv     Move QTA start to FW ahead of set client state to TRM 
                     to prevent crash due to FW receiving RX_START_CFG first
11/03/2015   wsh     Avoid race cond. between DIV grant and QTA start
10/29/2015   vke     Updated condition for TA started based on TA state
10/27/2015   wsh     Trigger sys loss if tune fails  
10/15/2015   arm     Redesign unlock_cancel while in D2LR DRCRampdown phase 
10/15/2015   arm     D2LR: Stop FLL tracking when coming out of QTA and going
                     to 4d state. 
10/15/2015   arm     D2LR: If in internal rampdown and next unlock buy is greater 
                     than HDRSRCHTC_MAX_TIME_TO_NEXT_TA ignore the unlock by.
10/13/2015   mbs     Support unlock_cancel while in D2LR DRCRampdown phase
10/09/2015   vke     Changes to re-evaluate the TA duration along with 
                     QTA condition just before starting the TA
10/08/2015   vke     Use TA information from TRM callback for deciding QTA
10/07/2015   mbs     Supported internal DRC rampdown for D2LR during QTA/LTA
10/04/2015   mbs/wsh Fixed going into D2L even after TA is requested
10/02/2015   wsh     Fixed not disabling TX in 4->4d 
09/23/2015   mbs     Ignore unlock_cancel from TRM during D2L redir
09/14/2015   mbs     Fix compilation errors caused by Dual Sim disabled - continuation
08/20/2015   rmv     Set QTA to FALSE in the last leg of OFS tune away proc 
08/20/2015   wsh     Prevent chipx16 cb blocking deactivate 
08/18/2015   arm     Fixed QTA featurization 
08/11/2015   mbs     Used ACCESS priority between TCA-TCCAck period for TRMU
08/11/2015   rmv     Check winning client just before calling trm_set_client 
                     to avoid race condition involving QTA with 1x
08/06/2015   vke     Changes to increase TAP call priority to Access urgent
07/23/2015   vko     FR 29379 : Disable tuneaway during Qchat call
07/23/2015   vke     Changes to send txd suspend on LTA
07/22/2015   mbs     Supported handling unlock cancel just before TA
07/16/2015   tnp     Fix compiler error associated  code without FEATURE_HDR_QTA
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
07/02/2015   mbs     Ignored unlock_by request if it exceeds DRC rampdown time
06/26/2015   vke     Changes to handle band grant failure in TCA tune
06/18/2015   tnp     Fix compiler warnings
06/03/2015   mbs/vlc Added support for D2L redirection during DO traffic.
06/02/2015   mbs     Removed deprecated API usage
05/14/2015   wsh     Fixed link error 
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
05/04/2015   mbs     Supported QTA using TRM API
03/26/2015   kss     Added use of hdrsrchrf_get_rf_device() and 
                     hdrsrchrf_get_reason().
02/24/2015   bb      Changes to handle scenarios when TRM winning changes
                     just before DO starts QTA
02/13/2015   ljl     Supported tuneaway of unlock by 15ms  
02/10/2015   vke     Changes to prevent diversity RF disable during QTA gap
01/31/2015   vke     Changes to cancel QTA if TA time is in rlsi duration
01/22/2015   arm     Added Band Avoidance feature control NV. 
01/15/2015   arm     Increased tuneaway overhead time by 3 ms, to account 
                     for delay in timer processing and div disable
12/12/2014   wsh     Fixed ASDiv statics getting reset in 4a/4c 
12/04/2014   arm     Changes to support new unified micro priority changes
11/13/2014   sat     Disable TA while processing TCA
11/04/14     arm     Band avoidance: move power info saving to hdrsrch task and
                     enable BA only for DSDA mode.
10/29/2014   arm     Called RF API to disable Intelliceiver during QTA gap.
08/27/2013   wsh     Fixed crash in 2nd chain test
12/19/2014   arm     Removed deprecated TRM APIs in HDR
12/19/2014   tnp     Revert Changes to update TRM about QTA start earliar to
                     avoid granting chain to other priority clients
12/10/2014   bb      Changes to update TRM about QTA start earliar to
                     avoid granting chain to other priority clients
12/09/2014   vke     Fix to avoid invalid TA substate change in hdrsrchtc
                     during rmac deactivate
11/19/2014   vke     Changes to store QTA winning client id and share with TRM
11/17/2014   rmv     Notify TX data disable status to MCPM only for non-QTA 
11/14/2014   wsh     Temp. revert setting MCPM to RX state in 4a
11/11/2014   wsh     Added support for DR-DSDS ASDiv
11/04/14     arm     Band avoidance: move power info saving to hdrsrch task and
                     enable BA only for DSDA mode.
10/31/2014   arm     Updated DR mode after 5->4 transition.
10/28/2014   sat     Featurizing some part of DRDSDS code in DR DSDS feature
                     flag
10/28/2014   arm     Handled Div granted just before QTA start.
10/23/2014   arm     Use BG_TRAFFIC instead of TRAFFIC for TRM reason. 
10/20/2014   vke     Disable the diversity state machine before QTA even when 
                     diversity is not owned
09/22/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
10/16/2014   arm     Disabled diversity for QTA before start QTA to FW  to 
                     avoid ARD enabling/disabling div inbetween QTA.
10/13/2014   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
10/13/2014   arm     HDR DR: use LOW_LAT reason for TAP call.
10/08/2014   bb      Chanegs to choose LTA if QTA end time collides with onex
                     reservation time
10/03/2014   arm     HDR QTA: check if TX is on before starting QTA
10/01/2014   arm     Enabled QTA for DR mode.
10/01/2014   arm     Changes to support HDR band avoidance.
09/22/2014   arm     HDR QTA code cleanup
09/09/2014   vke     Redesign fix for connection close during QTA gap
09/05/2014   vke     Recheck for QTA condition just before QTA start
09/02/2014   sat     Fixing a minor bug in rf_unlock_event, to block TA while 
                     TCC_ACK_PENDING
08/28/2014   arm     Changes to support QTA with diversity
08/28/2014   bb      Changes to clear QTA flag while processing ADVICE RF LOCK
                     command
08/26/2014   vke     Enable QTA for only DSDS mode
08/25/2014   bb      Changes to clear QTA flag while processing IDLE MODE command
08/22/2014   vke     Delay connection close during QTA gap
08/20/2014   arm     Disable Tx after qta if doing OFS or channel change.
08/18/2014   arm     Disabled diversity before start qta.
08/14/2014   arm     Disabled QTA for multicarrier revb.
08/13/2014   vke     On 4c->4 tranistion for QTA, schedule retune. 
08/13/2014   arm     Used trm_get_extension_flag API to check for QTA enable
08/12/2014   arm     Workaround to support diversity for QTA.
08/12/2014   arm     Changes to not reset packet sequences during QTA.
08/08/2014   sat     Updating LTE measurement across Traffic state
07/22/2014   bb      Fix to prevent enable of QTA for unlock immediate TA
07/22/2014   bb      Changes to reset QTA flag while processing UNLOCK
                     cancel events
07/22/2014   bb      Changes to fix RXLM buffer leakge during QTA
07/17/2014   bb      Changes to clear QTA flag if new state is 
                     not TRAFFIC SUSPEND state
07/16/2014   vke     Set trm extension flag to hold the BG traffic 
                     so that GSM FW is awake before QTA
07/16/2014   arm     Added NV control to enable/disable QTA.
07/15/2014   arm     After QTA do not enable AFC flag.
07/11/2014   bb      Changes to fix deadlock situation between processing
                     TCA and UNLOCK RF immediate command
07/03/2014   arm     Used BG TRAFFIC reason for DR-DSDS traffic.
06/27/2014   wsh     ARD/ASDiv incorrectly switching in RTAP mode  
06/05/2014   rmv     Delay setting MCPM state to HDRSRCHMCPM_STOP_TX_REQ, until 
                     searcher traffic exit in MCDO case 
06/04/2014   arm     Changes to support DO to G QTA
05/12/2014   vke     Klockworks fix
05/04/2014   ljl     Added preferred channel when update nset.
03/14/2014   rmv     Enabled D-LNA in DSDA depending on band combinations
03/12/2014   bb      Changes to enable tune-aways after TCC ACK is received
03/10/2014   rmv     Delay enabling of D-LNA till FAC gain calc is completed 
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
03/05/2014   vlc     Suspend DLNA before starting search. 
02/20/2014   rmv     Delay enabling of D-LNA after entering traffic to be able
                     to receive FE/RETAP AsyncCC Param Assign Message
02/13/2014   ljl     Supported handoff override after tuneaway.
01/24/2014   rmv     Enable DLNA when resuming traffic
01/15/2014   wsh     Added DSDS ASDiv support
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
11/25/2013   rmg     Moved call to RMAC notify_suspend at an earlier point.
11/07/2013   wsh     Disable ASDiv during TAP call
10/24/2013   rmg     Disabled DLNA in idle state.
09/30/2013   vke     DRC ramp down optimization for rude wakeup cases
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/30/2013   dsp     Removed mcdo_check from NPA featurization.
08/30/2013   dsp     Enable tuneaways in time sync state for OptHO.
08/26/2013   vke     Move back the copy of tune_away.pref after receiving 
                     the TCC ACK
08/19/2013   arm     Merged DSDS feature for triton. 
08/19/2013   arm     Merged DSDS feature for triton. 
08/13/2013   rmg     Fixed primary search dump being processed as default dump.
08/05/2013   wsh     Fixed 2nd chain test call on Dime
08/06/2013   rmg     Added support to convey power down reason in reverse link
                     power down command.
07/01/2014   vlc     Change state to HDRSRCHMCPM_STOP_TX_REQ without checking 
                     MCDO status in hdrsrchtc_done(). 
06/14/2013   vke     Allow tune-away while waiting for RT-ACK
06/26/2013   wsh     Added Selective TxD feature
06/20/2013   arm     Fixed Bolt compiler errors.
06/03/2013   rmv     Updated the logic that determines the time of the next 
                     OFS event 
05/31/2013   vko     Fixed klocwork issues
05/24/2013   smd     Reverted KW fixes. 
05/23/2013   rmg     Added change to schedule primary search 2 ms after 
                     submitting combined search in diversity mode.
05/22/2013   smd     Fixed an assertion. 
04/22/2013   arm     Added changes for DSDA coexistence manager.  
05/16/2013   vko     Fixed klocwork issues
02/21/2013   smd     Allowed diversity chain tune away first if primary chain 
                     release is done after unlock_by time.
01/25/2013   smd     Fixed HDR not releasing div when tune away is disabled in 
                     TC state.   
10/05/2012   ukl     Resolved compilation errors after removing FEATURE_BCMCS.
09/27/2012   ljl     Updated RGS before deassigning fingers.
09/25/2012   rmg     Added support for including freq info in TRM interaction.
09/05/2012   rmg     Added support for antenna tuner feature.
08/28/2012   kss/wsh Disable tuneway in TAP mode
08/24/2012   kss/wsh Fixed TAP mode turning off after tuneaway and OFS
08/07/2012   wsh     Fixed crash due to ARD enabled in IDLE
08/03/2012   wsh     Defined QPCH feature, but disable by NV
06/01/2012   wsh     Only complete ASET_UPDATE_CMD after on_idle handler
05/25/2012   wsh     Moved ARD logic into it's own state machine
04/11/2012   ljl     Increased the overhead to 17ms for tune away.
03/20/2012   arm     Resolve TCB errors for Dime. 
03/19/2012   ljl     Fixed Dime compiler errors.
03/08/2012   wsh     Merged NLB support
02/17/2012   ws      Delay TX power up after tune is complete.
02/09/2012   grl     Do OFS if you have time to return and tune away to 1x. 
02/01/2012   ljl     Deactivate decoder before disabling RF.
01/26/2012   ljl     Waited for finger rsp before lowering the clock.
01/25/2012   grl     Update gRice rank for all ASP changes.
01/18/2012   grl     Tune away in time for the 1x wakeup.
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
01/10/2012   ljl     Allowed ARD only in traffic state.
01/09/2012   ljl     Disabled div when entering TC suspend state. 
11/11/2011   arm     HDR idle and access state diversity implemetation.
10/28/2011   vlc/lyl Added gRICE CSET promotion support. 
10/27/2011   rkc     Resumed tuneaway on div chain if chain 0 unlock by changed. 
10/25/2011   lyl     Supported FL ARD enhancement.
10/25/2011   ljl     Handled the failure cases during optimized HO.
10/21/2011   ljl     Don't start TX before finger assigned for the first TCA. 
10/18/2011   lyl     Set valid demod carrier mask in ASP update message.
10/17/2011   rmg     Removed featurization around MCPM calls.
10/12/2011   ljl     Added HDRSRCH_SET_CC_START_CMD, HDRSRCH_SET_SCC_OFFSET_CMD,
                     and HDRSRCH_AT_SET_DRC_CMD.
10/03/2011   lyl     Deassign EQ before rx stop and before mcpm lower clks.
09/30/2011   ljl     Supported active HO.
09/29/2011   lyl     Set valid ASP snr rank before EQ is enabled.
06/02/2011   lyl     Supported HDR FL data driven adaptive RxD. 
05/12/2011   ljl     Removed sleep clk estimate.
04/27/2011   rmg     Added HDR MCPM support changes.
04/25/2011   ljl     Supported TX powerdown callback.
03/29/2011   vlc     Reset LTE measusrement state when entering traffic state 
                     from idle state.
03/03/2011   arm     Fixed NPA crash due to overlapping NPA calls. 
03/10/2011   ljl     Supported XO acquisition range improvement.
11/22/2010   grl     Stop tune away event processing in the TUNE_AWAY_GONE state.
12/14/2010   ljl     Limited the max fingers and disabled div for mc on 9k.
10/15/2010   pxu     Added dynamic checks for JCDMA features.  
10/19/2010   lyl     Aborted active search before entering idle state.
09/30/2010   lyl     Aborted active search before processing ASET update cmd.
09/25/2010   grl     Added support to inform 1x of HDR traffic state.
09/21/2010   grl     Perform slow clock estimates during traffic.
08/19/2010   arm     Switch to CLKRGM_HDR_TRAFFIC in traffic and deregister 
                     CLKRGM_HDR_WAKEUP
08/06/2010   arm     DCVS changes for MDM9k. 
06/25/2010   rkc     Added NPA MDSP Clock Speed support.
06/21/2010   cnx     Fixed compiler warnings.
05/26/2010   ljl     Supported hdrfmac_get_drc_length().
04/15/2010   ljl     Added F3 messages for handoff.
03/31/2010   pxu     Removed FEATURE_HDR_EQ, FEATURE_HDR_TC_OFS_ENHANCEMENT, 
                     feature checks 
03/31/2010   ljl     Increased the overhead time to 20ms.
03/18/2010   ljl     Supported FEATURE_HDR_DIV_SRCH_WORKAROUND.
03/11/2010   ljl     Supported the fw timg state.
02/16/2010   rkc     Made sure tuning was complete before transition to IDLE.
01/06/2010   rkc     Added NPA support for HDR traffic.
12/07/2009   ljl     Skipped RF tune when processing TCA with the same channel.
11/11/2009   ljl     Supported BCMCS in Rev B.
11/04/2009   rmg     Removed enabling of DRC channel from search code.
10/20/2009   etv     Included enhanced drc values for validation if feature defined.
10/14/2009   kss     Removed command to FW to enable/disable DRC.
10/13/2009   rkc     Moved tune away processing after srch is idle.
09/30/2009   kss     Send message to disable DRC.
07/22/2009   ljl     Added frame offset.
06/30/2009   lyl     Updated for generic responses process.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   lyl/ljl Updated for firmware MSGR interface.
05/08/2009   smd     Used tune away on search idle for HDRSRCH_ADVISE_UNLOCK_RF_CMD.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
04/08/2009   grl     Updated func name to hdrsrchrf_is_shdr_supported.
03/20/2009   rkc     Added rup_subtype param to hdrsrchutil_update_nset_pilots.
03/18/2009   rkc     Added RF SVDO API changes.
12/23/2008   ljl     Avoid DRC fix mode getting set to variable mode after HHO.
12/03/2008   grl     Prevented OFS processing to disable tune aways in TAP.
11/24/2008   lyl     Added DRC filter support.
11/03/2008   grl     Removed call as part of ARDA rearchitecture.
10/28/2008   smd     Fixed tab special character and indent issues.
10/17/2008   lyl     Disabled QuickPaging preamble detect in connected state
10/13/2008   lyl     Fixed lint errors.
08/29/2008   lyl     Supported primary chain ASET search.
08/06/2008   ljl     Checked whether RF is tuned before TC OFS.
07/15/2008   lyl     Fixed lint error (506: constant value boolean )
07/07/2008   ljl     Supported 1x handdown in Rev B.
06/17/2008   kss     Keep drc rampdown (in mDSP) enabled for tuneaway duration
06/11/2008   cc      Used 5-bit DRC Value.
                     Added MC suffix to HDRDRC_GET_FILTER_OUTPUT call.
06/12/2008   etv     Moved HDRDRC_SET_MAX_DRC call to FMAC instead.
06/08/2008   grl     Check we are not in FTS-4 mode before enabling HPT mode.
05/16/2008   rkc     Used demod index as HDRDRC_GET_FILTER_OUTPUT() parameter.
05/09/2008   rkc     Fixed compiler warnings.
03/27/2008   ljl     Added the avoided channel list to nset update function.
02/12/2008   etv     Added support for Multi-carrier AT command at$qchdrc.
02/08/2008   grl     Added full time SHDR support 3.
02/08/2008   jyw     Used MC tuning check including both channels and demods.
01/30/2008   jyw     Added demod index in the RF interfaces.
01/21/2008   ljl     Set DRC override parameter to the default value (0).
01/14/2008   etv     Removed logging drc_arq_buffer_record on state transition.
01/02/2008   ljl     Enabled equalizers.
12/18/2007   ljl     Enabled predictor and removed DRC override.
                     Updated for equalizer.
11/20/2007   rmg     Removed compilation errors on 7800.
11/14/2007   ljl     Override DRC for MDM7800 bringup.
11/13/2007   rkc     Fixed handdown mode initialization.
11/06/2007   grl     Fixed HPT mode not being saved properly in idle.
10/31/2007   ljl     Merged from Rev B branch.
10/17/2007   grl     Merged Rev B changes from branch.
10/14/2007   grl     Added full time SHDR support.
09/21/2007   mt      Removed debug F3 message.
09/20/2007   mt      HDR Adaptive diversity implementation.
08/19/2007   grl     Resolved Lint High warnings.
08/16/2007   mt      Changed TC OFS tuneaway interval to 426ms.
08/09/2007   rkc     Use FEATURE_HHO_FLL_ACQ_GAIN_MODE to featurize fll start
                     acq for HHO
08/08/2007   mt      Increased tuneaway forbidden HSTR interval.
07/06/2007   ljl     Supported rotator push.
06/22/2007   ljl     Updated cell numbers when processing aset update.
06/20/2007   rkc     Changed hdrsrchtc_process_int_cmd() input to payload ptr
06/20/2007   mt      Remove RUR unit test code.
04/27/2007   grl     Added support for access hybrid ctrl via the tc mode.
04/10/2007   mt      Enhanced OFS and optimized VOIP tuneaway timeline support.
02/02/2007   ljl     Set decoder to traffic mode if we came back to begining
                     of tune away timeline while we were in tx stopped state.
01/17/2007   ljl     Added HDR SRCH AFC stm.
01/08/2007   grl     Abort active searches when going to the OFS state.
01/02/2006   ljl     Dropped ASET pilots to NSET if TCA has a different 
                     channel.
12/27/2006   grl     Fixed traffic tune aways during div tune aways.
11/10/2006   mt      Reverted TCA resume tune back to normal AGC acquisition.
10/17/2006   kss     Clear pkt sequence #s when activating the decoder for TCA.
10/03/2006   grl     Deactivate the decoder during TCAs.
09/14/2006   kss     Add call to cancel suspend (tuneaway) if necessary.
08/22/2006   kss     Inform RTCMAC of tuneaway with hdrrmac_notify_suspend().
08/21/2006   grl     Supported the new hdrsrchrf_set_agc_acq_params prototype.
08/07/2006   mt      Changes to use new fast AGC acq during TC resume tuning.
07/13/2006   ljl     Deassigned equalizers after disabling diversity
06/30/2006   etv     Supported DSP Handoff algorithm changes.
06/22/2006   jyw     Relocated the hdrsrchtc_enable_ofs.
06/13/2006   grl     Moved control of the RCL from searcher to FMAC.
05/22/2006   ljl     Added support for FLAT.
05/12/2006   sq      Disable traffic OFS when BCMCS is enabled
04/26/2006   jyw     Used the second level on_search_idle callbacks.
04/20/2006   ljl     Added a parameter to hdrsrchafc_stop_fll_tracking().
04/18/2006   jyw     Fixed the variable name in the ERR_FATAL macro.
04/09/2006   grl     Don't process commands when moving to the suspended state.
03/28/2006   grl     Clear primary tune away signal when forcing unlock..
03/17/2006   sq      Clear tune-away timer when releasing RF
02/06/2006   jyw     Avoid the mdsp programming when ASET is empty at tc init.
12/14/2005   sq/jyw  Added the support to disable the TC OFS in the TAP calls.
11/18/2005   grl     Set the tune away type in the DSP during tune aways.
11/16/2005   sq      Added missing BCMCS featurization
11/10/2005   sq      Re-try idle mode command in 3b state
11/07/2005   grl     Switched to new 1x search interface file.
10/24/2005   jyw     Fixed the lint errors.
10/21/2005   sq      Abort tune-away before exiting traffic state
10/18/2005   grl     Added FEATURE_HDR_REVA_L1 ifdef to Rel A logging.
10/13/2005   ds      Added DRC-ARQ-DSC logging support
10/07/2005   sq      Removed compilation warning
09/10/2005   grl     Added dynamic TC mode support for HPTS mode.
09/10/2005   ds      Added AFC check before assigning and deassigning eqs
09/06/2005   ljl     Saved cached sector infor
09/06/2005   ds      Renamed maintain_fll_lock() to check_fll_lock_status()
09/01/2005   grl     Sped up response time for releasing the RF lock.
08/22/2005   sq      Added FEATURE_HDR_NO_ACTIVE_HANDDOWN
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
08/04/2005   ljl     Changed the DRC range from 12 to 14 for Rev A
08/02/2005   ds      Added hash channel list parameter to NSET update
08/01/2005   grl     Fixed compile errors in FEATURE_HDR_TC_OFS_ENHANCEMENT
07/25/2005   ljl     Added broadcast and directed nset update 
06/30/2005   sq      Changed OFS API name
06/20/2005   jyw/sq  Special handling of TCA when tune-away is in progress
06/04/2005   sq      Adjusts softer hand-off delay for MPS testing
06/01/2005   sq      Transitions into state 4c only when searcher is idle
05/31/2005   grl     Added support for TRM and TCXOMgr.
05/02/2005   sq      Transitions into state 4a only when searcher is idle
04/25/2005   sq      Added FEATURE_HDR_TC_OFS_ENHANCEMENT
04/21/2005   ds      Added support for equalizers under FEATURE_HDR_EQ
04/21/2005   kss/sq  Used new hdrdec mode setting APIs
03/18/2005   grl     Fixed serving sector handoff logging during TAP bug.
03/31/2005   grl     Changed HDR's use of trk_lo to be GPS friendly
03/23/2005   hrk     Replaced hdrdec_deactivate by hdrsrchutil_dec_deactivate 
01/18/2005   kss     Removed wait before hdrdec_deactivate().
01/13/2005   sq      Enable diversity at the end of tune back from 1X
01/05/2005   sq      Updated code comments
01/04/2005   sq      Mainlined FEATURE_HDR_TC_OFS & FEATURE_HDR_DRC_FILTERING
11/29/2004   grl     Added connected state stat logging support.
11/22/2004   kss     Wait additional slot before deactivating the decoder.
11/10/2004   kss     Removed hybrid BCMCS-specific support (no longer needed).
11/08/2004   sq      Disable diversity before tuning to new channel
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
09/13/2004   ajn     Use new 1x SRCH functions instead of 1x SRCH variables.
08/18/2004   aaj     Support for suspending/resuming BC operation 
08/23/2004   ds      Update HDR debug display record
07/30/2004   sq      Added support for FEATURE_HDR_BCMCS
07/22/2004   ajn     Added common signals to state machine
06/07/2004   aaj     Replaced hdrsrchidle_init_idle_state with new function
05/12/2004   sq      Mainlined FEATURE_HDR_TC_DIVERSITY_ONLY
02/03/2004   sq      Supported FEATURE_HDR_DRC_FILTERING
01/27/2004   ajn     Restricted hand-down evaluation to 1x ZZ state only.
01/19/2004   sq      Reset weak coverage timer to 1 when handing down to 1X
01/06/2004   aaj     Print correct A/C/N energies in handdown metric measurement
12/23/2003   sq      State changes from 4 to 4a after receiving unlock rf cmd
11/18/2003   aaj     Support usability API. RMAC API change. Reselect change.
10/29/2003   sq      Disable diversity before leaving TC state
10/29/2003   aaj     Do not include weak pilots in the RESELECT algorithm
10/28/2003   aaj     Mainlined feature FEATURE_HDR_RESELECT
10/20/2003   aaj     Fixed DRC rampdown. Corrected HSTR writing to mod 0xffff
10/14/2003   aaj     Support to validate search results to avoid bogus PNs
10/02/2003   aaj     Added HDRRMAC shutdown and powerup for TCA tune. HHO fix
10/02/2003   aaj     Re-enable hybrid operation in traffic
09/23/2003   ajn     RF function calls now go through hdrsrchrf_()
09/22/2003   aaj     Temporarily disable TUNE-AWAY to 1x for 6500
09/16/2003   sq      Added support for FEATURE_HDR_TC_OFS
09/10/2003   mpa     Converted F3 messages to use new MSG2.0 SSIDs
06/13/2003   ajn     Reverted back to dump-count based Reselect timer
06/05/2003   aaj     Do not enable DRC in TC init since FMAC does it already
06/04/2003   ajn     Reselect timer uses PN roll count instead of dump count.
06/02/2003   aaj     Initialze drc predicted value to 0 after tune back
04/04/2003   aaj     Added support to maintain FLL lock during steady state
03/11/2003   ajn     Updated Copyright notice
02/07/2003   ajn     Prevent Handdown to 1x when 1x is not active
02/06/2003   aaj     Ignore TCA which has incorrect pilot count
01/30/2003   sq      Added support for turning on diversity only in traffic
01/10/2002   ajn     Corrected EWC condition in FEATURE_HDR_RESELECT
01/06/2003   ajn     Added FEATURE_HDR_RESELECT
12/19/2002   ajn     Cleanup
12/18/2002   kss     Only start freq pullin if channel has actually changed.
12/04/2002   aaj     Fix the RL-FL ASP mismatch
12/02/2002   ajn     Reworked tune-away timeline.
11/25/2002   ajn     Suspend reports generated.  Suspend callback saved.
09/09/2002   aaj     Print bestASPHandoff F3 msg within 30ms of the handoff
09/05/2002   aaj     Cause an extra pilot report after entering traffic
08/23/2002   aaj     Reset pkt sequence numbers when resetting decoder
08/22/2002   aaj     Start Freq pullin after channel change in traffic
08/13/2002   ajn     Featurized system-time check
08/09/2002   ajn     Added system-time check
07/16/2002   aaj     Merged 3.1branch to tip
06/19/2002   ajn     Changed "|* MSG_HIGH *|" to "MSG_LOW".
05/15/2002   aaj     Set decoder stale timer. Added delay before dec_deactivate
05/15/2002   aaj     Move search sched stop from tune away pre-proc to proc
05/13/2002   aaj     Deactivate HDR Dec during 4->4a. Re-activate at ->4
                     Replace slot timer with rex timer for tune away
                     Handle pre-mature exit from tune away state machine
                     Stop searcher schedular in tc_done
                     Lock RF during session negotiation traffic connection
05/09/2002   ajn     Add enable parameter to PILOT_REPORT_CMD F3 msg
05/01/2002   aaj     Support not tuning away to IS2000 if necessary
04/29/2002   aaj     Don't be too aggressive about tune away. Added 4ms margin
03/20/2002   aaj     Clear tune away processing before exiting HDRSRCHTC
03/19/2002   aaj     Fixed a hybrid timeline issue where pre-tune processing
                     could be done twice resulting in "FT Valid: Lost"
03/15/2002   aaj     Corrected rampdown slot count from full to half slots
03/11/2002   aaj     Correct tune away timeline to avoid possibility of
                     trashing tx frame
03/11/2002   aaj     Fix DRC rampdown to write relative HSTR offset to DSP
03/04/2002   aaj     Check DRC param control variable for all any -> 4 xtions
02/16/2002   aaj     Reduce frequency of tune away F3 messages
02/06/2002   ajn     hdrerrno -> errno
01/31/2002   aaj     Print pilot information rcvd in TCA and ASET update
01/18/2002   aaj     Support for traffic hybrid mode
01/17/2002   aaj     support for RF band to the hdr sector structure type
01/01/2002   ajn     System Arbitration support for Tune away added.
12/31/2001   ajn     Added retain_lock / yield_rf functionality.
12/07/2001   ajn     Drop old ASET pilots on ASET channel change.
12/05/2001   aaj     Initialize bestASP to a valid value during tc_init
11/16/2001   aaj     Disable PN ROM bug fix
11/01/2001   aaj     Change to argument list of hdrsrchrf_tune_to_chan
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
10/08/2001   aaj     Made DRC param control function external for testApp
10/08/2001   ajn     Command retry now automatically done at state change.
09/13/2001   aaj     Added RABInfo log record support in TC state
09/04/2001   ajn     Cmd interface change - allows TCA from idle to restart.
07/31/2001   aaj     Disable DRC when leaving the connected state
07/30/2001   aaj     Changes to drc_param for AT controlled DRC settings
07/17/2001   aaj     Changed traffic channel default to be variable rate
06/26/2001   aaj     Initialize DRC cover for asp1 to 1 in srchtc_init
06/06/2001   ajn     Rework using OnDump and OnSearchIdle module global funcs
06/04/2001   aaj     Temporary fixes to force DRC rate and ASP index
04/25/2001   aaj     Change to search status enumerations
03/14/2001   aaj     changed search rate to 30ms (from 60ms) after TC testing
03/02/2001   aaj     included customer.h file
01/05/2001   ajn     TC -> IDLE ASET transition.
11/22/2000   ajn     Code review changes
10/31/2000   ajn     Added General Purpose Signals to State Machine
10/23/2000   ajn     Added VCS Header line
10/06/2000   ajn     Created this file

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchfing.h"
#include "hdrsrchset.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchafc.h"
#include "hdrsrch.h"
#include "hdrsrchmsg.h"
#include "hdrsrcheq.h"

#include "hdrsrchi.h"

#include "hdrmdspmcr.h"
#include "hdrsrchlog.h"
#include "timetick.h"
#include "hdrts.h"

#include "hdrrx.h"
#include "hdrfmac.h"
#include "hdrmac.h"
#include "hdrmdsp.h"

#include "hdrdebug.h"
#include "hdrhai.h"
#include "hdrmc_v.h"

#include "err.h"

#include "hdrmod.h"
#include "hdrdec.h"
#include "hdrsrchmcpm.h"

#include "tcxomgr.h"

#include "srch_hdr.h"
  /* Current 1x receive energy, for hand-down decisions */


#ifdef FEATURE_HDR_REVB
  #include "hdrsrchschdgrp.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_NPA
#include "npa.h"
#endif /* FEATURE_NPA */
#ifndef FEATURE_HDR_DIME_MODEM
#include "sclk.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrsrchard.h"

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#if defined ( FEATURE_HDR_IS890 ) || defined ( FEATURE_HDR_IS890A )
#include "hdris890_v.h"
#endif    /* FEATURE_HDR_IS890 || FEATURE_HDR_IS890A */

#if defined ( FEATURE_HDR_MODEM_COEXISTENCE_SW ) || defined ( FEATURE_HDR_BAND_AVOIDANCE )
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_QTA
#include "gl1_hw.h"
#endif /* FEATURE_HDR_QTA */

#include "hdrrmac.h"
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchtclte.h"
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

/*===========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

===========================================================================*/

#define HDRSRCHTC_SEARCH_INTERVAL_MS                  30
  /* time interval between two search cycles */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#define HDRSRCHTC_LTEMEAS_INTERVAL_MS                300
  /* time interval between two LTE meas cycles */

#define HDRSRCHTC_MAX_TA_TIME_FOR_D2L_MEAS           100
  /* Total time that can be used to measure LTE */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#define HDRSRCHTC_COMB_TO_PRI_SEARCH_INTERVAL_MS       2
  /* time interval between submission of combined search result and that
     of primary search when receive diversity is enabled */

#ifdef FEATURE_HDR_DLNA
#define HDRSRCHTC_SUSPEND_DLNA_B4_SEARCH_INTERVAL_MS   5
  /* Suspend DLNA before search to make sure samples are good for search */
#endif /* FEATURE_HDR_DLNA */

/*---------------------------------------------------------------------------
  1xEVDO Reselection Constants
---------------------------------------------------------------------------*/

#define HDRSRCHTC_RESELECT_THRESHOLD_DB         7
  /* Reselect if HDR coverage strength is less than -7.0db ... */

#define HDRSRCHTC_RESELECT_THRESHOLD_MS      4000
  /* ... for more than 4 seconds ... */

#define HDRSRCHTC_RESELECT_1X_THRESHOLD_DB     14
  /* ... and if 1x coverage strength is better than -14.0db */

#define HDRSRCHTC_RESELECT_DRC_THRESHOLD    0x4000
  /* If DRC filter output is below this threshold, handdown might happen.
     The range of DRC filter output is from 0 to 0x7fff, mapped from 0 to 1.
     0x4000 is the mid-point of this range                               */

#define HDRSRCHTC_RESELECT_MIN_ENG_THRESHOLD   40
  /* Pilot has to be at least this strong to be included in reselect metric 
     Set this value to -14dB/20 for disabling this. Default is -11dB/40 */

/*---------------------------------------------------------------------------
  Tune away constants
---------------------------------------------------------------------------*/

#define HDRSRCHTC_EXTEND_POLL_MS          300
  /* Poll for RF lock extension every these many ms */

#ifdef FEATURE_HDR_QTA
#define HDRSRCHTC_OVERHEAD_MS        \
        ((hdrsrchtc.tune_away.is_qta) ? 6 : 20)
  /* In case of QTA overhead is zero as RF is disabled by FW */
#else
#define HDRSRCHTC_OVERHEAD_MS              20
#endif /* FEATURE_HDR_QTA */
  /* Allow 10ms overhead for releasing RF */

#define HDRSRCHTC_DRC_RAMP_MS    (SLOTS_TO_MS( 80 ) + HDRSRCHTC_OVERHEAD_MS )
  /* prepare for DRC rampdown 80 slots before tune away */

#ifdef FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION
#define HDRSRCHTC_DRC_RAMP_MIN_MS   (SLOTS_TO_MS( 20 ) + HDRSRCHTC_OVERHEAD_MS )
  /* prepare for DRC rampdown time based on DRC3 before tune away */
#endif /* FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION */

#define HDRSRCHTC_VOIP_TUNEAWAY_MIN_TIMER_HALFSLOTS  6
  /* minimum next event timer duration to be programmed */

#define HDRSRCHTC_VOIP_OUTAGE_NOTIFY_SLOTS    80
  /* as per ATOM design document */

#define HDRSRCHTC_VOIP_OUTAGE_NOTIFY_MS \
        ( SLOTS_TO_MS( HDRSRCHTC_VOIP_OUTAGE_NOTIFY_SLOTS ) + HDRSRCHTC_OVERHEAD_MS )
  /* Send VOIP outage notification 64 slots before VOIP tune away */

#define HDRSRCHTC_VOIP_DRC_RAMP_SLOTS   ( 24 + 2 )
  /* Start DRC ramp down this many slots before VOIP tune away 
     2 slots is just safety against timer inaccuracies */

/* #define HDRSRCHTC_VOIP_DRC_RAMP_SLOTS \
        ( HDRDRC_GET_DRC_LENGTH() + 16 + 2 ) */

#define HDRSRCHTC_VOIP_DRC_RAMP_MS \
        ( SLOTS_TO_MS( HDRSRCHTC_VOIP_DRC_RAMP_SLOTS) + HDRSRCHTC_OVERHEAD_MS )
  /* prepare for DRC rampdown, 16 + DRCLength slots before VOIP tune away 
     1 slots is reserved for any processing delay */

#define HDRSRCHTC_VOIP_DRC_RAMP_MAX_SLOTS ( 16 + 8 + 2 )
  /* longest VOIP DRC ramp down duration 
     corresponding to DRC Length of 8 slots */ 

#define HDRSRCHTC_VOIP_PRETUNE_SLOTS         ( 16 + 3 )
  /* equivalent to 16 + 5 ms slots for regular tuneaways */

#define HDRSRCHTC_VOIP_PRETUNE_MS \
        ( SLOTS_TO_MS( HDRSRCHTC_VOIP_PRETUNE_SLOTS) + HDRSRCHTC_OVERHEAD_MS )

#define HDRSRCHTC_VOIP_DRC_RAMPDOWN_TO_PRETUNE_MAX_HALFSLOTS \
    (  2 * ( HDRSRCHTC_VOIP_DRC_RAMP_MAX_SLOTS + 2 - HDRSRCHTC_VOIP_PRETUNE_SLOTS ) )
  /* times 2 is due to half slots to slots conversion
     plus 2 is safety against timer inaccuries.
     HDRSRCHTC_VOIP_DRC_RAMP_MAX_SLOTS - HDRSRCHTC_VOIP_PRETUNE_SLOTS 
     is time bewteen drc ramp down start and pretune time */

#define HDRSRCHTC_DRC_RAMP_SCLK (TIMETICK_SCLK_FROM_MS(HDRSRCHTC_DRC_RAMP_MS))
  /* Time to start DRC rampdown in sclks */

#ifdef FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION

#define HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK (TIMETICK_SCLK_FROM_MS(HDRSRCHTC_DRC_RAMP_MIN_MS))
  /* Time to start DRC rampdown in sclks for LTA cases */

#ifdef FEATURE_HDR_DR_DSDS
#define HDRSRCHTC_FAST_DRC_RAMP_MIN_MS (15)

#define HDRSRCHTC_FAST_DRC_RAMP_MIN_SCLK (TIMETICK_SCLK_FROM_MS(HDRSRCHTC_FAST_DRC_RAMP_MIN_MS))
  /* Min time to TA */
#endif /* FEATURE_HDR_DR_DSDS */

#define HDRSRCHTC_LTA_OVERHEAD_MS      17 
  /* SW overhead of LTA  */

#define HDRSRCHTC_LTA_DRC_RAMP_MIN_MS   (SLOTS_TO_MS( 20 ) + HDRSRCHTC_LTA_OVERHEAD_MS )
  /* prepare for DRC rampdown time based on DRC3 before tune away */

#define HDRSRCHTC_LTA_DRC_WORSTCASE_RAMP_SCLK (TIMETICK_SCLK_FROM_MS(HDRSRCHTC_LTA_DRC_RAMP_MIN_MS))
  /* Time to start DRC rampdown in sclks */

#define HDRSRCHTC_SW_OVERHEAD_QTA_END_MS    8
  /* sw overheads required to release chain after QTA END */

#define HDRSRCHTC_SW_OVERHEAD_QTA_END_SCLK                 \
                    (TIMETICK_SCLK_FROM_MS(HDRSRCHTC_SW_OVERHEAD_QTA_END_MS))

  /* sw overheads required to release chain after QTA END in sclks */
#endif /* FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION */

#define HDRSRCHTC_EXTEND_MS   (HDRSRCHTC_EXTEND_POLL_MS+HDRSRCHTC_DRC_RAMP_MS)
#define HDRSRCHTC_EXTEND_MS_SCLK (TIMETICK_SCLK_FROM_MS( HDRSRCHTC_EXTEND_MS ))
  /* request extension for poll duration + the DRC ramp down duration */

#define HDRSRCHTC_PRE_TUNE_MS    (27 + 5 + HDRSRCHTC_OVERHEAD_MS)
  /* pretune away starts at 16 slots + 5ms before actual tune away */

#define HDRSRCHTC_MAX_HO_COUNT 65535
  /* The maximum number of handoffs tracked in the HO register. */

#define HDRSRCHTC_OFS_RF_TUNING_MS        20
  /* Time reserved for one RF tuning for OFS  */

#define HDRSRCHTC_OFS_OVERHEAD_MS                               \
                      ( ( HDRSRCHTC_DRC_RAMP_MS + HDRSRCHTC_OFS_RF_TUNING_MS ) * 2 )
  /* Time needed for an OFS in traffic state. This includes:
     1. Time for DRC ramp-down procedure twice 
     2. Time for tuning RF twice
     3. Some overheads, included in DRC RAMP time               */

#define HDRSRCHTC_OFS_VOIP_OVERHEAD_MS                               \
        ( HDRSRCHTC_VOIP_OUTAGE_NOTIFY_SLOTS + HDRSRCHTC_OFS_RF_TUNING_MS * 2 )

#define HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_MS                5400
  /* AT can only start a new OFS search group if at least this much 
     amount of time has elapsed since the last search group started. 
     Essentially this interval defines OFS search rate limiter */
     
#define HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_SCLKS               \
           ( TIMETICK_SCLK_FROM_MS( HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_MS ) )

#define HDRSRCHTC_OFS_CHECK_INTERVAL_MS                         \
           ( HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_MS + 10 )
  /* Interval between evaluating traffic OFS condition */

#define HDRSRCHOFSTC_HALFSLOT_IN_SCLKS ( TIMETICK_SCLK_FROM_US(833) )

#define HDRSRCHOFSTC_80MS_IN_SCLKS ( TIMETICK_SCLK_FROM_MS(80) )

#define HDRSRCHTC_OFS_REPEAT_INTERVAL_MS                  426 
#define HDRSRCHTC_OFS_REPEAT_INTERVAL_SCLKS                 \
            ( TIMETICK_SCLK_FROM_MS(HDRSRCHTC_OFS_REPEAT_INTERVAL_MS) )

#define HDRSRCHTC_OFS_RUP_SRCH_INTERVAL_MS                426 
#define HDRSRCHTC_OFS_RUP_SRCH_INTERVAL_SCLKS               \
            ( TIMETICK_SCLK_FROM_MS( HDRSRCHTC_OFS_RUP_SRCH_INTERVAL_MS ) )

#define HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_LOW               320 
#define HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH              456

#define HDRSRCHTC_OFS_TIMELINE_DITHER_MAX_MS                \
            ( HALFSLOTS_TO_MS( ( 512 - HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH ) ) )

#define HDRSRCHTC_OFS_TIMELINE_DITHER_MAX_SCLKS             \
            ( TIMETICK_SCLK_FROM_MS( HDRSRCHTC_OFS_TIMELINE_DITHER_MAX_MS ) )

#define HDRSRCHTC_OFS_REPEAT_MIN_INTERVAL_SCLKS             \
  ( HDRSRCHTC_OFS_REPEAT_INTERVAL_SCLKS - HDRSRCHTC_OFS_TIMELINE_DITHER_MAX_SCLKS )

#define HDRSRCHTC_OFS_RUP_SRCH_MIN_INTERVAL_SCLKS             \
  ( HDRSRCHTC_OFS_RUP_SRCH_INTERVAL_SCLKS - HDRSRCHTC_OFS_TIMELINE_DITHER_MAX_SCLKS )

#define HDRSRCHTC_VOIP_TUNEAWAY_OUTAGE_DURATION   ( SLOTS_TO_MS( 48 ) )
  /* VOIP outage duration in ms */

#define HDRSRCHTC_OFS_TA_TIMER    (&hdrsrch.gp_timer0)
#define HDRSRCHTC_OFS_TA_SIG      HDRSRCH_GP_TMR0_SIG

#ifdef FEATURE_HDR_DLNA
#define HDRSRCHTC_DELAY_DLNA_ENABLE_TIMER    (&hdrsrch.gp_timer1)
#define HDRSRCHTC_DELAY_DLNA_ENABLE_SIG      HDRSRCH_GP_TMR1_SIG
#define HDRSRCHTC_DELAY_DLNA_ENABLE_DURATION           500
  /* D-LNA enabled 500 ms after entering traffic */
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1
#define HDRSRCHTC_USE_FAST_AGC_IN_RESUME_TUNE          FALSE
  /* whether to use FAST AGC algorithm when resuming traffic */
#else
#define HDRSRCHTC_USE_FAST_AGC_IN_RESUME_TUNE          FALSE
  /* whether to use FAST AGC algorithm when resuming traffic */
#endif

#ifdef FEATURE_HDR_SHORT_TC_OFS_TUNEAWAY_TIMELINE
#define HDRSRCHTC_SHORT_OFS_TUNEAWAYS_ENABLED           TRUE
#else
#define HDRSRCHTC_SHORT_OFS_TUNEAWAYS_ENABLED          FALSE
#endif /* FEATURE_HDR_SHORT_TC_OFS_TUNEAWAY_TIMELINE */

#define HDRSRCH_RUP_UNIT_TEST                          FALSE

#define HDRSRCHTC_BAND_GRANT_TIMEOUT_MS                (100)
  /* Band grant time out */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION

#define HDRSRCHTC_MAX_TIME_TO_NEXT_TA                \
              (HDRSRCHTC_DRC_RAMP_MIN_MS + 500)
  /* Is next TA far away? Value in ms */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

/*---------------------------------------------------------------------------
  Tune away
---------------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHTC_TUNE_AWAY_INIT,

  HDRSRCHTC_TUNE_AWAY_NO_CONFLICT,

  HDRSRCHTC_TUNE_AWAY_VOIP_OUTAGE_NOTIFICATION,

  HDRSRCHTC_TUNE_AWAY_DRC_RAMP,

  HDRSRCHTC_TUNE_AWAY_TX_STOP,

  HDRSRCHTC_TUNE_AWAY_GONE,

  HDRSRCHTC_TUNE_AWAY_RETURNING,

  HDRSRCHTC_TUNE_AWAY_REACQ
}
hdrsrchtc_tune_away_enum_type;


/* EJECT */
/*==========================================================================

                     IDLE STATE PROTOCOL STATES

==========================================================================*/

typedef enum
{
  HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE,
    /* Traffic state is waiting for RT-Ack, 
       prevent RF unlock-by processing */
    
  HDRSRCHTC_TCC_ACK_RECEIVED_SUBSTATE,
    /* RT-Ack received and data transfer is in inprogress, 
       allow RF unlock-by processing  */

  HDRSRCHTC_CONNECTION_CLOSE_INPROGRESS_SUBSTATE,
    /* Connection Close in progress. Do not allow TA  */

  HDRSRCHTC_NUM_SUBSTATES
}
hdrsrchtc_substate_enum_type;

/* EJECT */
/*===========================================================================

              CONNECTED (Traffic Channel) STATE PROTOCOL STATES

===========================================================================*/

/*---------------------------------------------------------------------
   Connected Protocol States
---------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHTC_INIT_STATE,
  HDRSRCHTC_ACTIVE_STATE,
  HDRSRCHTC_NUM_STATES
}
hdrsrchtc_state_enum_type;


#ifdef FEATURE_HDR_DLNA
typedef enum
{
  HDRSRCHTC_DLNA_INACTIVE_STATE,
  HDRSRCHTC_DLNA_ACTIVE_STATE,
  HDRSRCHTC_DLNA_SUSPEND_STATE
}
hdrsrchtc_dlna_state_enum_type;
  /* DLNA state is active when DLNA mode has been enabled via FW command.
     DLNA can be suspended during search to allow for good samples */
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
/*  D2LR state machine.  The following is a description of the D2LR state machine:
    D2LR state is part of DO traffic (hdrsrchtc.d2lr_state.)  This state machine
    is used to decide if DO should make LTE measurement and how to do so to 
    minimize DO throughput degradation.  There are 2 possible tune away scenarios for LTE
    measurement, control by this state machine:
    1.  Piggyback LTE meas:  DO transitions to LTE meas state (if required) right after a tune away.
        This is the preferred method as DRC rampdown is already performed for the tune away.
        DO just uses left over time to make LTE measurement.
    2.  LTE meas with DRC rampdown:  If there is no tune away in the near future and DO is required
        to make LTE measurement, DO performs DRC rampdown for the measurement.  This is the less
        preferred method as the DRC rampdown is only used for LTE measurement. */

typedef enum
{
  HDRSRCHTC_D2LR_START,          /* Initial state, no measurement */
  HDRSRCHTC_D2LR_ENABLE_MEAS,    /* LTE NSET UPDATE REDIR cmd is received from CP */
  HDRSRCHTC_D2LR_DRC_RAMPDOWN,   /* D2LR requires it's own DRC rampdown, not piggy back */
  HDRSRCHTC_D2LR_WAIT_MEAS,      /* Piggy back is possible once tune away is finished */
  HDRSRCHTC_D2LR_MEASURE         /* Transition to HDRSRCH_LTE_MEAS_TC_STATE */
}
hdrsrchtc_d2l_redir_state_enum_type;
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  
/*---------------------------------------------------------------------
   AT command to control the DRC parameter is received from the LapTop
   It updates this parameter for DRC settigns.
---------------------------------------------------------------------*/
uint8  hdrsrchtc_drc_param = 0;


/* <EJECT> */
/*===========================================================================

                     HDR SEARCH CONNECTED STATE DATA

As much CONNECTED data is lumped together in one structure, so that it
becomes possible to dynamically allocate acquisition data memory during the
CONNECTED state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchtc.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchtc must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchtc  (*hdrsrchtc_struct_ptr)

===========================================================================*/

typedef struct
{
  uint16                                  threshold_eng;
    /* Energy less that this value indicates weak HDR coverage */

  uint16                                  weak_coverage_ms;
    /* Accumulated weak HDR coverage duration */
}
hdrsrchtc_reselect_struct_type;


typedef struct
{
  hdrsrchtc_tune_away_enum_type           state;
    /* Tune away state */

  hdrsrch_traffic_tune_away_enum_type     pref;
    /* Tune away selection sent by the protocols for this particular
       traffic connection */

  rex_timer_type                          timer;
    /* Timer to program hdr traffic-1x tune aways */

  boolean                                 no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  timetick_type                           ofs_time_sclks;
    /* Sclk timestamp for when we started last OFS tune away. */
    
  timetick_type                           ofs_group_time_sclks;
    /* Sclk timestamp for when we started last group of OFS tune aways. */
    
  timetick_type                           ofs_check_time_sclks;
    /* Sclk timestamp for when we last checked OFS condition */

  boolean                                 urgent_rel_only;
    /* Only release the lock if there is an urgent request. */

  boolean                                 is_voip_tuneaway;
    /* Flag whether the tuneaway is during VOIP */

  trm_client_enum_t                       ta_client_id;
    /* We are tuning away for this TA client */

#ifdef FEATURE_HDR_QTA

  boolean                                 is_qta_enabled_via_nv;
    /* Flag whether QTA is enabled via NV */

  boolean                                 is_qta;
    /* Flag whether to do QTA or LTA */

  uint32                                  rxlm_qta_target_buf_idx;
    /* Save target QTA client rxlm idx to deallocate it later */

  timetick_type                            sclks_at_retain_lock;
    /* same sclks at retain lock time */

  rfm_meas_common_param_type               rf_params;
      /* Common RF parameters passed to G for QTA */
#endif /* FEATURE_HDR_QTA */

//hdrmdsp_timer_handle_type               slot_timer_handle;
//  /* tune away slot timer interrupt handler */
  boolean                                 force_lta;
    /* if LTA has to be forced for temperory DDS type scenarios */
}
hdrsrchtc_tune_away_struct_type;


typedef struct
{
  const hdrsrch_tc_assignment_params_type *tca_params;
    /* Traffic Channel Assignment message that caused the transition
       to traffic state */

  hdrsrch_pilot_rpt_cb_type               pilot_rpt;
    /* Function to call when interesting pilot events occur. */

  boolean                                 pending_pilot_rpt;
    /* Flag indicating a pilot event occured, but no callback function
       existed to report the event to. */

  boolean                                 new_frequency;
    /* Traffic channel assignment caused tune to new frequency; In revB
       the flag indicates if LO relocation is needed. that probably 
       means that the outer carriers in the carriers list has changed. */

  hdrsrchtc_tune_away_struct_type         tune_away;
    /* Tune away state */

  hdrsrchtc_reselect_struct_type          reselect;
    /* 1xEVDO -> 1x Handdown data */

  boolean                                 first_tc_dump;
    /* Flag that indicates whether the current search dump is 
       the first dump after entering traffic state             */

  boolean                                 waiting_for_close_loop;
    /* Whether we are waiting for TCXO close loop permission to continue. */

  uint32                                  time_accum;
    /* The accumulative time duration that HDR is in traffic state
       without interruption. It is equivalent to the accumulative
       duration that HDR holds the RF in traffic state without 
       tuning away to other systems                            */

  boolean                                 ofs_disabled;
    /* Flag indicates whether OFS is disalbed by some application. For
       example, FTAP or RTAP                                             */

#ifdef FEATURE_HDR_REVB
  uint8                  num_cells_in_subaset[HDRSRCH_MAX_NUM_SUBASETS];
    /* Number of unique cells in ASET. For the unlocked mode, the number
       might be differenct on different subaset. */
#else
  uint8                                   num_cells_in_aset;
    /* Number of unique cells in ASET */
#endif /* FEATURE_HDR_REVB */

  boolean                                 is_first_tca;
    /* Whether this is the first TCA */

  boolean                                 is_slam_needed;
    /* Whether slam is needed at the first dump */

  uint8                                   failed_srch_cnt;
    /* Failed search count */

  boolean                                 okay_to_do_pri_srch;
    /* TRUE if it is okay to perform primary search at the next search 
       scheduling opportunity */

  hdrsrchtc_substate_enum_type            substate;
    /* variable to track sub-state of traffic state */

  hdrsrchtc_substate_enum_type            last_substate;
    /* Store the past substate */

  boolean                                 pending_unlock_rf_immed;
    /* TRUE if UNLOCK RF immediate is pending */

#ifdef FEATURE_HDR_DLNA
  hdrsrchtc_dlna_state_enum_type          dlna_state;
    /* Whether DLNA is active, inactive, or suspended (DLNA is suspended
       to get good samples during search) */
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  hdrsrchtc_d2l_redir_state_enum_type     d2lr_state;
    /* D2LR state machine */
  timetick_type                           d2lr_start_meas_time;
    /* Time interval to make LTE meas in DO connected state */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
}
hdrsrchtc_struct_type;

typedef struct
{
  hdrsrch_voip_outage_notify_cb_type      outage_cb;
    /* callback to notify VOIP outage to upper layers */

  uint16                                  outage_notify_hs;
    /* hstr count at outage notification time */

  uint16                                  tuneaway_hs;
    /* hstr count at desired tuneway time */

  uint16                                  actual_tuneaway_hs;
    /* hstr count at tuneway time */

  uint16                                  rampdown_hs;
    /* hstr count at desired drc rampdown time */

  uint16                                  actual_rampdown_hs;
    /* hstr count at drc rampdown time */

  uint16                                  pretune_hs;
    /* hstr count at desired pretune time */

  uint16                                  actual_pretune_hs;
    /* hstr count at pretune time */
}
hdrsrchtc_voip_tuneaway_struct_type;
  /* most of the halfslot counts in above structure is for 
     instrumentation and debugging */

LOCAL hdrsrchtc_struct_type    hdrsrchtc;

LOCAL hdrsrchtc_voip_tuneaway_struct_type voip_tuneaway;

/* EJECT */
/*===========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

===========================================================================*/


LOCAL void            hdrsrchtc_init( hdrsrch_state_enum_type old_state );
LOCAL void            hdrsrchtc_done( hdrsrch_state_enum_type next_state );
LOCAL errno_enum_type hdrsrchtc_cmd( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchtc_sched( void );
LOCAL void            hdrsrchtc_dump( void );
LOCAL void            hdrsrchtc_ofs_tune_away_proc( void );
#ifdef FEATURE_HDR_DLNA
LOCAL void            hdrsrchtc_dlna_enable_proc( void );
#endif /* FEATURE_HDR_DLNA */
LOCAL void            hdrsrchtc_rf_unlock_event( void );
LOCAL void            hdrsrchtc_process_int_cmd
                      (
                        hdrsrchsm_cmd_payload_type *
                      );

#ifdef FEATURE_HDR_QTA

void                  hdrsrchtc_update_qta_mode
                      ( 
                        hdrsrchrf_retain_lock_state_t  * rl_state
                          /* State of retain lock. */
                      );

void                  hdrsrchtc_update_qta_on_rlsi
                      ( 
                        uint32 time_to_ta 
                          /* TA time in ms */
                      );
#endif /* FEATURE_HDR_QTA */

LOCAL hdrsrch_state_struct_type hdrsrchtc_state =
{
  HDRSRCH_CONNECTED_STATE,
  hdrsrchtc_init,
  hdrsrchtc_done,
  hdrsrchtc_cmd,
  hdrsrchtc_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrchtc_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrchtc_ofs_tune_away_proc,
#ifdef FEATURE_HDR_DLNA
  hdrsrchtc_dlna_enable_proc,
#else
hdrsrch_default_gp_timer1,
#endif /* FEATURE_HDR_DLNA */
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchtc_rf_unlock_event,
  hdrsrchtc_process_int_cmd,
  hdrsrch_default_process_low_pri_rsp_ind
};


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the traffic state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_enable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are now interested in primary chain events. */

  hdrsrchtc.tune_away.start_time_sclks = timetick_get();
    /* Save the current time in sclks. */

#ifdef FEATURE_HDR_QTA
  hdrsrchtc.tune_away.sclks_at_retain_lock = hdrsrchtc.tune_away.start_time_sclks;
    /* Reference for unlock time */
#endif /* FEATURE_HDR_QTA */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "Ena-TA Time now in sclk %d", 
                  hdrsrchtc.tune_away.start_time_sclks );
  
  hdrsrchtc.tune_away.no_tune_away = TRUE;
    /* There is no tune away as of now. */

  hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
    /* Register an unlock callback so that we may be interrupted. */

} /* hdrsrchtc_enable_tune_aways */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_DISABLE_TUNE_AWAYS

DESCRIPTION
  Turns off the ability for tune aways to happen within the traffic state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_disable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );
    /* Unregister for unlock callbacks.  We now will respond to them. */

  (void) rex_clr_timer( &hdrsrchtc.tune_away.timer );
    /* clear the tune away timer if it is active */

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are no longer interested in primary chain events. */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "Dis-TA Time now in sclk %d", 
                  timetick_get());
} /* hdrsrchtc_disable_tune_aways */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_GET_TC_REASON

DESCRIPTION
  Retrieves the current reason for Traffic: BCMCS/Low Latency Traffic/or
  regular traffic.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  The reason for being in traffic. 

SIDE EFFECTS
  None

===========================================================================*/

hdrsrchrf_reason_enum_t hdrsrchtc_get_tc_reason( void )
{
  hdrsrchrf_reason_enum_t     reason;
    /* The new resource why the RF lock is held. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* First determine the reason for traffic. */
  if ( ( ( hdrsrch.tc_mode == HDRSRCH_S1XHDR_HP_TC_MODE ) && 
         hdrsrchrf_is_shdr_supported( ) ) ||
       ( hdrsrchafc_get_tap_status() == TRUE ) )
  {
    /* The HPT mode needs the 2nd independent chain for 1x paging,
       or, when TAP is active, need not to worry about 1x */

    reason = HDRSRCHRF_LOW_LAT_TRAFFIC;
      /* Alter priority to indicate we are in traffic. */
  }
  else if(hdrsrch.tc_mode == HDRSRCH_DISABLE_TA_TC_MODE)
  {
	  reason = HDRSRCHRF_LOW_LAT_TRAFFIC;
		/* Alter priority to indicate we are in traffic. */
  }
  else
  {
#ifdef FEATURE_HDR_BCMCS
    if ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED )
    {
      reason = HDRSRCHRF_BROADCAST_TRAFFIC;
        /* Alter priority to indicate we are in traffic with BCMCS active. */
    }
    else
    {
      reason = HDRSRCHRF_BG_TRAFFIC;
        /* Alter priority to indicate we are in normal trafic mode. */
    }
#else /* FEATURE_HDR_BCMCS  */

    if ( hdrsrchtc.substate == HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE )
    {
      reason = HDRSRCHRF_ACCESS;
      HDR_MSG_SRCH_1( MSG_LEGACY_LOW, 
                      "Priority %d", 
                      reason);
    }
    else
    {
    reason = HDRSRCHRF_BG_TRAFFIC;
      HDR_MSG_SRCH_1( MSG_LEGACY_LOW, 
                      "Priority %d", 
                      reason);
    }
      /* Alter priority to indicate we are in normal trafic mode. */

#endif /* FEATURE_HDR_BCMCS  */
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TC mode %d TRM Reason %d", hdrsrch.tc_mode, reason);

  return reason;

} /* hdrsrchtc_get_tc_reason */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_SET_TC_MODE

DESCRIPTION
  Sets the selected traffic mode which may change the priority for holding
  the RF lock while in traffic and enables/disables simultaneous mode.

  The options currently supported are:

  HDRSRCH_DEFAULT_TC_MODE   - Standard HDR traffic connection.

  HDRSRCH_S1XHDR_HP_TC_MODE - High Priority Traffic Mode where HDR forces 1x
                              paging onto chain 1, and preempts 1x acquisition.


DEPENDENCIES
  None

PARAMETERS
  tc_mode - The traffic mode to be invoked.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_set_tc_mode
(
  hdrsrch_tc_mode_enum_type   tc_mode
    /* The specific HDR traffic mode selected. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, 
                             hdrsrchtc_get_tc_reason() );
    /* Alter priority to indicate we are in the desired reason. */

} /* hdrsrchtc_set_tc_mode */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_TCA_DONE

DESCRIPTION
  Traffic Channel Assignment processing - wrapup

DEPENDENCIES
  HDRSRCHTC_TCA_CMD

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Calls "traffic channel assignment complete" callback.

===========================================================================*/

LOCAL void hdrsrchtc_tca_done( void )
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* At this point, the searcher was stopped, the RF was tuned to the
     required channel, a new search was started, the search completed, and
     fingers have been assigned to the multipaths. */

  if ( hdrsrchtc.is_slam_needed == TRUE )
  {

    /* Check whether any finger is assigned.
       If yes, slam. Otherwise, we need to increase the search window size and search again */
    if ( hdrsrchfing_are_fings_assigned() == TRUE )
    {
      hdrsrchdrv_start_slam( FALSE, 0, 0, 0, 0 );
        /* Adjust timing to the sector we reacquired on */
  
      hdrsrchtc.is_slam_needed = FALSE;

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Finger assigned and Slam" );

      if ( hdrsrchtc.failed_srch_cnt > 0 )
      {
        hdrsrchutil_restore_aset_win_size();
          /* Restore aset win size */
      }
    }
    else
    {
      /* TBD Repeat search */

      hdrsrchtc.failed_srch_cnt++;

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "Failed to detect the pilot failed cnt %d", 
                      hdrsrchtc.failed_srch_cnt );

      if ( hdrsrchtc.failed_srch_cnt <= 1 )
      {
        hdrsrchutil_save_and_set_aset_win_size( HDRSRCH_WINSIZE_512_CHIPS );
          /* Set aset win size to 512 chips */
        
        hdrsrch_on_dump ( hdrsrchtc_tca_done );
          /* Search again with larger win size */
  
        HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
          /* To trigger a search */
      }
      else
      {
        hdrsrchtc.tca_params->tc_assign_cb( HDRSRCH_TCA_FAILED );
          /* Report that TCA failed */

        hdrsrchtc.tca_params = NULL;
        hdrsrch_complete_cmd( );
          /* Discard the TCA message parameters & continue command processing */
      }

      return;
    }
  }

#ifdef FEATURE_HDR_SELECTIVE_TXD
  payload.tca_band = hdrsrchtc.tca_params->subaset_list[0].channel.band;

  hdrsrchrftxd_event_with_payload( 
     HDRSRCHRFTXD_EVENT_TRAFFIC_ENTERED,
     &payload );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrchtc.tca_params->tc_assign_cb( HDRSRCH_TCA_SUCCEEDED );
    /* Report that the fingers have now been assigned to the TCA pilots */

  hdrsrchtc.tca_params = NULL;
  hdrsrch_complete_cmd( );
    /* Discard the TCA message parameters & continue command processing */

  hdrfmac_update_subtype_to_fw();
    /* Inform the fw to sync the current FMAC subtype */

#ifdef FEATURE_HHO_FLL_ACQ_GAIN_MODE
  if ( hdrsrchtc.new_frequency )
  {
    /* We just tuned to a different channel in traffic state.
       Start FLL pullin to adjust the frequency error faster */

    hdrsrchafc_start_fll_acq( 0 );
      /* Start the Frequency Locked Loop acquisition a.k.a. Pull-In */
  }
#endif /* FEATURE_HHO_FLL_ACQ_GAIN_MODE */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  hdrsrchard_event( HDRSRCHARD_EVENT_ENTER_TC );
    /* treat a TCA same as TC state enter. Timer is always reset due
         to TCA anyway. */
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

  /* Enable TA after processing TCA*/
  HDR_MSG_SRCH( MSG_LEGACY_LOW, "Enabling TA after TCA processing");
  hdrsrchtc_enable_tune_aways();

} /* hdrsrchtc_tca_done( ) */


/*===========================================================================

FUNCTION HDRSRCHTC_TCA_TUNE_DONE

DESCRIPTION
  Called after tuning to a new frequency to start search scheduler

DEPENDENCIES
  Called from hdrsrchrf_tune_to_chan callback

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_tca_tune_done
( 
  hdrsrchrf_tune_status_enum_type status
)
{
#ifdef FEATURE_HDR_REVB
  uint8    i; 
    /* Loop index */

  uint8    c;
    /* Demod Index */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

#ifdef FEATURE_HDR_REVB
  /* Update the current RF configuration */

  hdrsrch.aset_channel_cnt = hdrsrchtc.tca_params->num_subaset;

  for( i=0; i<hdrsrchtc.tca_params->num_subaset; i++ )
  {
    hdrsrch.aset_channel[i] = hdrsrchtc.tca_params->subaset_list[i].channel;
    hdrsrch.aset_demod[i]   = hdrsrchtc.tca_params->subaset_list[i].demod_idx;
      /* Set the new ASET channel */
  }
#else
  hdrsrch.aset_channel = hdrsrchtc.tca_params->channel;
    /* Set the new ASET channel */
#endif /* FEATURE_HDR_REVB */

  if ( hdrsrchtc.new_frequency )
  {

    hdrdec_reset_pkt_sequences();
      /* Reset packet sequence numbers */

    hdrdec_activate();
      /* Activate HDR decoder after switching frequencies. */
  }

#ifndef FEATURE_HDR_REVB
  (void) hdrsrchutil_update_aset_pilots( hdrsrchtc.tca_params->aset_pilots,
                                  hdrsrchtc.tca_params->num_pilots );
    /* Set the active set pilots */

  hdrsrchtc.num_cells_in_aset = hdrsrchutil_get_num_unique_cells_in_aset();
    /* Save the number of unique cells in ASET for later use */
#else
  HDR_ASSERT(hdrsrchtc.tca_params->num_subaset <= HDRSRCH_MAX_NUM_SUBASETS);
  (void) hdrsrchutil_update_aset_pilots_revb( hdrsrchtc.tca_params->subaset_list,
                                       hdrsrchtc.tca_params->num_subaset,
                                   hdrsrchtc.tca_params->cc_subactive_set_idx );
    /* The index to the subaset which carries CC channel */
    /* Set the active set pilots for revB */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    hdrsrchtc.num_cells_in_subaset[i] = 
                            hdrsrchutil_get_num_unique_cells_in_subaset(i);

    c = hdrsrchset.subaset[i].demod_idx;

    hdrsrchtc_drc_param_ctrl( c );
  }
    /* Save the number of unique cells in ASET for later use */

  hdrsrch.frame_offset = hdrsrchtc.tca_params->frame_offset;
    /* Save frame offset */

  hdrsrchutil_mcdo_check();
    /* Change MDSP clock speed, if necessary. */

#endif /* FEATURE_HDR_REVB */


  /* If this is the first TCA from Idle -> TC or Timesync -> TCA, 
     LMAC will enbable tx later */
  if ( hdrsrchtc.is_first_tca == TRUE )
  {
    hdrsrchtc.is_first_tca = FALSE;
      /* Clear the flag */
  }
  else
  {
    hdrrmac_powerup_tx(
#ifdef FEATURE_HDR_QTA
     hdrsrchtc.tune_away.is_qta
#endif /* FEATURE_HDR_QTA */
    );
    /* Now that RF tune is complete we should start the 2nd chain */
  }

  if ( hdrsrchutil_handoff_override_pending( ) == TRUE ) 
  {
    hdrsrchdrv_abort_search( FALSE );
      /* Abort the current search. */
    /* TBD should be aborted before tune? */
  }
  hdrsrch_on_dump( hdrsrchtc_tca_done );
    /* Complete TCA after next dump and triage */

  HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
    /* Set the schedule signal, to start the next search on the now current
       frequency */

  hdrsrch_set_fing_and_div_limitation( );
    /* Set the finger or diversity limitation if necessary */
} /* hdrsrchtc_tca_tune_done() */


#ifdef FEATURE_HDR_REVB 
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_CHECK_IF_TUNE_NEEDED 

DESCRIPTION
  Traffic Channel Assignment processing - check if RF tune is needed. 

  The function may not be needed if the RF driver provides similiar func.

DEPENDENCIES
  HDRSRCHTC_TCA_CMD

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

boolean hdrsrchtc_check_if_tune_needed
( 
  const hdrsrch_tc_assignment_params_type *tca_params
    /* New TCA parameters */
)
{
  boolean                tune_needed = FALSE;
    /* Flag indicate if RF tune is needed */

  uint8                  chan_cnt, i;
    /* Channel count and loop index */

  sys_channel_type       chan_list[HDRSRCH_MAX_NUM_CARRIERS];
    /* ASET channel list */

  hdrsrch_demod_idx_type demod_list[HDRSRCH_MAX_NUM_CARRIERS];
    /* Current demod assignment. Please note that the demod assignment change
       should always cause RF tuning */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check with RF driver to see if retune is needed given a channel list;
     TBD: if the interface is needed  */

  chan_cnt = tca_params->num_subaset;

  HDR_ASSERT ( chan_cnt <= HDRSRCH_MAX_NUM_CARRIERS );
    /* KW fix : Check to limit chan_cnt to max */

  for( i=0; i<chan_cnt; i++ )
  {
    chan_list[i]  = tca_params->subaset_list[i].channel;
    demod_list[i] = tca_params->subaset_list[i].demod_idx;
  }

  tune_needed = hdrsrchrf_channel_reconfig_is_needed( chan_cnt, 
                                                      chan_list,
                                                      demod_list );

  return tune_needed;
} /* boolean hdrsrchtc_check_if_tune_needed */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_DROP_ASET_UPON_NEW_TCA

DESCRIPTION
  Traffic Channel Assignment processing - Drop the ASET when a new TCA is
  received. Select one of the existing to add into NSET and drop rest into
  RSET.

DEPENDENCIES
  HDRSRCHTC_TCA_CMD

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

boolean hdrsrchtc_check_if_ofreq_chan
( 
  sys_channel_type channel,
    /* The channel to test */

  const hdrsrch_tc_assignment_params_type *tca_params
    /* New TCA parameters */
)
{
  int8    i;
    /* Loop index */

  boolean ofreq_channel = TRUE;
    /* Return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=0; (i < tca_params->num_subaset) && (i < HDRSRCH_MAX_NUM_SUBASETS); i++ )
  {
    /* Idealy the chech should against the csr list. TBD */
    if( SYS_CHAN_CMP_EQUAL( channel, tca_params->subaset_list[i].channel ) )
    {
      ofreq_channel = FALSE;
      break;
    }
  }

  return ofreq_channel;
}
#endif /* FEATURE_HDR_REVB */

/*===========================================================================

FUNCTION HDRSRCHTC_TCA_TUNE

DESCRIPTION
  Traffic Channel Assignment processing - tune substep

DEPENDENCIES
  HDRSRCHTC_TCA_CMD

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Tunes to the new channel,
  Sets the new Active Pilot Set.
  Starts a search on the new channel,
  Assigns fingers to ASET multipath,
  Calls "traffic channel assignment complete" callback.

===========================================================================*/

LOCAL void hdrsrchtc_tca_tune( void )
{
  int                             i;
    /* Loop index for ASET pilots */

#ifdef FEATURE_HDR_REVB 
  uint8                          chan_cnt;
    /* Temporary channel count */

  sys_channel_type               chan_list[HDRSRCH_MAX_NUM_CARRIERS];
    /* Tempory channel list copied from TCA params */

  hdrsrch_demod_idx_type         demod_list[HDRSRCH_MAX_NUM_CARRIERS];
    /* Demod index assignment */
#endif /* FEATURE_HDR_REVB */

  boolean                        tca_tune_success = TRUE;
    /* TCA tune status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* At this point, the searcher is stopped.  Safe to tune (if required) */

#ifdef FEATURE_HDR_REVB

  HDR_ASSERT ( hdrsrchtc.tca_params->num_subaset <= HDRSRCH_MAX_NUM_CARRIERS );
    /* KW fix : Limit num_subaset to max */

  /* The subaset pre TCA procesing management rule is described below,
     (1) When this interface is used, it should be sure that tune is needed.
     (2) For revB hard handoff, different for revA hard handoff, no sector
         is dropped from ASET. All the fingers are de assigned.
         used to choose which one to add to CSET/NSET */
  if( hdrsrchtc_check_if_tune_needed( hdrsrchtc.tca_params ) )
    /* When this interface is used, LMAC has decided that tunning is a must. 
       Before this point, TX should have been stopped. Because it might be 
       redundent to do a check again here, the function always return TRUE
       for now. TBD: we might need remove the check here. */
#else
  if ( !SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel,
                            hdrsrchtc.tca_params->channel) )
#endif /* FEATURE_HDR_REVB */
  {

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_EXIT_TC );
      /* Treat a TCA tune same as exit TC state, this is because
         when tune finished, ARD timer will reset (due to TCC) */
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
      /* Turn off the diversity receiver before tuning to a new channel */

    /* Deassign all fingers to be ready for a revB tunning. Please be aware 
       that the entire ASET is preserved during the tunning in the revB hard
       handoff */

    hdrsrchfing_deassign_all_fings( );
      /* Deassign fingers */

    if ( hdrsrchafc_afc_uses_rot() )
    {
      hdrsrcheq_deassign_all_eqs( );
        /* Deassign all equalizers before TCA tune (before rx_stop) */
    }

    hdrsrchutil_dec_deactivate( );
      /* Deactivate HDR decoder to cancel all active packets */

    hdrsrchutil_powerdown_tx( HDRRMAC_TX_STOP_FOR_RETUNE );
      /* Since there is a channel change involved, shut-down TX before 
         starting RF tune process */

    hdrsrchtc.new_frequency = TRUE;
      /* Tuning to a new frequency */

    hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
      /* Set AGC acquisition params for normal mode. */
    
    /* Tune to the new traffic channel. When tuning (if required) is complete, 
       call hdrsrchtc_tca_tune_done */

#ifdef FEATURE_HDR_REVB 
    /* Copy the channel list from TCA params. It is in the TCA order. */

    chan_cnt = hdrsrchtc.tca_params->num_subaset;

    for( i=0; i<chan_cnt; i++ )
    {
      chan_list[i]  = hdrsrchtc.tca_params->subaset_list[i].channel;
      demod_list[i] = hdrsrchtc.tca_params->subaset_list[i].demod_idx;
    }

    tca_tune_success = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                            chan_list,
                            demod_list,
                            chan_cnt,
                            hdrsrchtc_tca_tune_done,
                            HDRSRCHTC_BAND_GRANT_TIMEOUT_MS );
#else
    tca_tune_success = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                            &hdrsrchtc.tca_params->channel,
                            hdrsrchtc_tca_tune_done,
                            HDRSRCHTC_BAND_GRANT_TIMEOUT_MS );
#endif /* FEATURE_HDR_REVB */

    if ( tca_tune_success == FALSE )
    {
      hdrsrchtc.tca_params->tc_assign_cb( HDRSRCH_TCA_FAILED );
        /* Report that TCA failed */
      
      hdrsrchtc.tca_params = NULL;

      hdrsrch_complete_cmd( );
        /* Discard the TCA message parameters and complete command */

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    }
  }
  else
  {

    HDR_MSG_SRCH( MSG_LEGACY_ERROR, " Noop tuning, should be the 1st TCA" );
      /* This can only happen for the 1st TCA */

    hdrsrchtc.new_frequency = FALSE;
      /* Not tuning to a new frequency */

    hdrsrchtc_tca_tune_done( HDRSRCHRF_TUNE_SUCCESS );
      /* Tune is not required so perform next step of TCA processing */
  }
} /* hdrsrchtc_tca_tune( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_SET_DRC_PARAM

DESCRIPTION
  This function sets the DRC parameters to the "drc_param" format

  Bit pattern which controls the DRC behaviour is:

    Bit7    : if 1 then the remaining bits are valid
    Bit6    : if 1 then enable real variable rate (disable predictor override)
    Bit5    : Reserved (Can be used for Rate Control Loop)
    Bit4    : Reserved
    Bit3-bit0 : DRC Values


DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Sets the "drc_param" variable

===========================================================================*/

void hdrsrchtc_set_drc_param
(
#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type demod_idx,
#endif /* FEATURE_HDR_REVB */

  uint8   drc_param
   /* bit pattern for DRC */

)
{
#ifdef FEATURE_HDR_REVA_L1
#ifdef FEATURE_HDR_REVB_ENH_RATES
  hdrhai_drc_value_enum_type max_drc = HDRHAI_MAX_REL_B_DRC;
#else
  hdrhai_drc_value_enum_type max_drc = HDRHAI_MAX_REL_A_DRC;
#endif /* FEATURE_HDR_REVB_ENH_RATES */
#else
  hdrhai_drc_value_enum_type max_drc = HDRHAI_MAX_REL_0_DRC;
#endif /* FEATURE_HDR_REVA_L1 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( drc_param == 0x0f )
  {
    hdrsrchtc_drc_param = 0xc0;
      /* Bit7 indicates change in the status
         Bit6 indicates entry to variable rate state */
  }
  else
  {
    hdrsrchtc_drc_param = 0x80;
      /* Bit7 indicates change in the status
         Bit6 is zero to indicate DRC overrride mode */

    hdrsrchtc_drc_param |= 
      ( drc_param <= max_drc ? drc_param : max_drc );
         /* Bit3-0 indicate DRC rate to be set to the DSP */   
  }
} /* hdrsrchtc_set_drc_param( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_DRC_PARAM_CTRL

DESCRIPTION
  This function sets the DRC parameters which are received from the AT command

  Bit pattern which controls the predictor behaviour.

    Bit7    : if 1 then the remaining bits are valid
    Bit6    : if 1 then enable real variable rate (disable predictor override)
    Bit5    : Reserved
    Bit4    : Reserved
    Bit3-bit0 : DRC Values


DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Changes the DRC rate/mode

===========================================================================*/

void hdrsrchtc_drc_param_ctrl
(
  uint8                      c
    /* demod carrier */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchmsg_init_drc_cfg_msg();
    /* Init DRC config message */

  /* read the drc parameter from NV here */

  if ( hdrsrchtc_drc_param & BIT( 7 ) )
  {
    /* Other bits in this field are valid */

    if ( hdrsrchtc_drc_param & BIT( 6 ) )
    {
      hdrsrchmsg_drc_cfg_set_drc_ovrd( c, HDRSRCHMSG_DRC_PRED_DSP_CTRL );
       /* Set Real Variable Rate Mode where Predictor controls the DRC values */
    }
    else
    {
      hdrsrchmsg_drc_cfg_set_drc_ovrd( c, HDRSRCHMSG_DRC_PRED_MICRO_CTRL );
        /* DRC is uP Controlled */

      hdrsrchmsg_drc_cfg_set_dict_drc( c, ( hdrsrchtc_drc_param & 0x1f ) );
        /* Set Var Rate as specified in the AT command */
    }

    hdrsrchmsg_send_msg( &hdrsrchmsg.drc_cfg_msg.hdr,
                         sizeof( hdrfw_drc_cfg_msg_t ) );
      /* Send the DRC config message */

  }/* if settings are changed */

} /* hdrsrchtc_drc_param_ctrl( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_INIT_DRC

DESCRIPTION
  This function initializes and enables DRC channel for connected state.

DEPENDENCIES
  Called from traffic channel init

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrsrchtc_init_drc( void )
{
#ifdef FEATURE_HDR_REVB
  uint8 i; 
    /* Loop index */
  uint8                      c;
    /* demod carrier */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*--------------------------------*
   * Default DRC/Predictor Settings *
   *--------------------------------*/

#ifndef FEATURE_HDR_REVB

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0, 1, 1 );
    /* Initialize the DRC cover to some valid value. Right now it is
       set to 1 for ASPIndex= 1. Note that it will be overwritten with correct
       value when the ASET is written to the DSP */

  if( hdrsrchset.aset[0] == NULL ) 
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "empty ASET! %d ", hdrsrchset.aset_cnt);
      /* Report the empty ASET without doing anything */
  }
  else
  {
    HDRDRC_SET_BEST_ASP( hdrsrchset.aset[0]->aset.asp_idx );
    /* initialize best ASP to some known value in the DSP */
  }

  HDRASP_HANDOFF_CTRL_DSP_MC( HDRMDSP_DEMOD_CARRIER_0 );
    /* Set DSP to control handoff */

  HDRDRC_PREDICTOR_CTRL_DSP_MC(HDRMDSP_DEMOD_CARRIER_0);
    /* Predictor is DSP Controlled */

  /*------------------------*
   * Execute DRC AT command *
   *------------------------*/

  hdrsrchtc_drc_param_ctrl( HDRMDSP_DEMOD_CARRIER_0 );
    /* Change the DRC mode as per the AT command from the user */

#else
  hdrsrchmsg_init_handoff_ovrd_msg();
    /* Init the HO override message */

  hdrsrchmsg_init_asp_update_msg();
    /* Init the ASP update message */

  hdrsrchmsg_init_drc_cfg_msg();
    /* Init DRC config message */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    c = hdrsrchset.subaset[i].demod_idx;

    HDRASP_SET_DRC_COVER( c, 0, 1 );
      /* Initialize the DRC cover to some valid value. Right now it is
         set to 1 for ASPIndex= 0. Note that it will be overwritten with correct
         value when the ASET is written to the DSP */
    hdrsrchmsg_set_asp_update_mask( c, 0 );
      /* Update the carrier and ASP mask */


    if( hdrsrchset.subaset[i].aset[0] == NULL ) 
    {
      ERR_FATAL( "empty ASET in traffic state! %d", 
                hdrsrchset.subaset[i].aset_cnt, 0, 0 );
    }
    else
    {
      hdrsrchmsg_ho_ovrd_set_trigger_mode( c, 
                 hdrsrchset.subaset[i].aset[0]->aset.asp_idx );
       /* initialize best ASP to some known value in the DSP */
       /* TBD To find out whether to use trigger mode? */


      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "handoff override trigger mode at demod idx %d asp idx %d", 
                      c, hdrsrchset.subaset[i].aset[0]->aset.asp_idx);

    }

    hdrsrchmsg_drc_cfg_set_drc_ovrd( c, HDRSRCHMSG_DRC_PRED_DSP_CTRL );
      /* Predictor is DSP Controlled */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "handoff controlled by mdsp at demod idx %d", c);


    /*------------------------*
     * Execute DRC AT command *
     *------------------------*/

    hdrsrchtc_drc_param_ctrl( c );
      /* Change the DRC mode as per the AT command from the user */

  }
  hdrsrchmsg_send_multi_asp_update_msg( TRUE );
    /* Send ASP update message */

  hdrsrchmsg_send_msg( &hdrsrchmsg.handoff_ovrd_msg, 
                       sizeof( hdrfw_handoff_override_msg_t ) );
    /* Send handoff override message */

  hdrsrchmsg_send_msg( &hdrsrchmsg.drc_cfg_msg,
                       sizeof( hdrfw_drc_cfg_msg_t ) );
    /* Send DRC config message */
#endif /* FEATURE_HDR_REVB */

}/* hdrsrchtc_init_drc */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_PILOT_REPORT_RESET_SECTOR

DESCRIPTION
  This function clears the "report" flag for the given pilot.

DEPENDENCIES
  None

PARAMETERS
  sect  -  Sector to be reset

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

LOCAL void hdrsrchtc_pilot_report_reset_sector
(
  hdrsrch_sect_struct_type    *sect
    /* Sector to reset pilot report for */
)
{
  if ( sect->pilot_rpt_pending == FALSE )
  {
    sect->pilot_rpt = HDRSET_PILOT_RPT_NONE;
      /* No more reports pending */
  }
  else
  {
    /* This event has been regenerated since the last time a pilot report
       was generated.  Signal again! */

    hdrsrchtc.pending_pilot_rpt = TRUE;
      /* Signal again! */
  }

} /* hdrsrchtc_pilot_report_reset_sector( ) */



/*===========================================================================

FUNCTION HDRSRCHTC_PILOT_REPORT_RESET

DESCRIPTION
  This function clears the "report" flag for pilots in the active
  and candidate sets.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May immediately cause pilot_rpt_cb( ) to be called, if new pilot events
  have occurred since the last call to hdrsrch_get_pilot_report( ).

===========================================================================*/

LOCAL void hdrsrchtc_pilot_report_reset_cmd( void )
{
  int   i;
    /* Loop index */

#ifdef FEATURE_HDR_REVB
  uint8 j;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Active Set */
#ifdef FEATURE_HDR_REVB
  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    for( j=0; j<hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      hdrsrchtc_pilot_report_reset_sector( hdrsrchset.subaset[i].aset[j] );
        /* Reset this sector's pilot report indication */
    }
  }
#else
  for( i=0; i<hdrsrchset.aset_cnt; i++)
  {
    hdrsrchtc_pilot_report_reset_sector( hdrsrchset.aset[i] );
      /* Reset this sector's pilot report indication */
  }
#endif /* FEATURE_HDR_REVB */


  /* Candidate Set */

  for( i=0; i<hdrsrchset.cset_cnt; i++)
  {
    hdrsrchtc_pilot_report_reset_sector( hdrsrchset.cset[i] );
      /* Reset this sector's pilot report indication */
  }


  if ( hdrsrchtc.pilot_rpt != NULL  &&  hdrsrchtc.pending_pilot_rpt )
  {
    /* If a report is pending, and we just installed a valid callback,
       generate a new report. */

    hdrsrchtc.pending_pilot_rpt = FALSE;
      /* Clear the pending report condition */

    hdrsrchtc.pilot_rpt( );
      /* Generate the report */
  }

} /* hdrsrchtc_pilot_report_reset_cmd( ) */



/* EJECT */

/*===========================================================================

FUNCTION HDRSRCHTC_SET_RX0_EVENT_HANDLER

DESCRIPTION
  This function installs a new signal handler for rx0 event.

DEPENDENCIES

PARAMETERS
  A new signal handler
  
RETURN VALUE
  The old signal handler

SIDE EFFECTS
  None

===========================================================================*/
    
LOCAL hdrsrch_sig_func hdrsrchtc_set_rx0_event_handler 
( 
  hdrsrch_sig_func           rx0_event_handler
    /* signal handler for rx0 event */
)
{

  hdrsrch_sig_func           old_handler;
    /* The original signal handler to be returned   */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  old_handler   = hdrsrchtc_state.pri_chain_event;
    /* Save the old signal handler        */

  hdrsrchtc_state.pri_chain_event = rx0_event_handler;
    /* set the rx0 event handler to the new handler */

  return  old_handler;

} /* hdrsrchtc_set_rx0_event_handler  */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_TUNE_BACK

DESCRIPTION
  Tune back to traffic after visit to 1x or OFS

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_tune_back
( 
  hdrsrchrf_tune_status_enum_type status
)
{

  dword                      tune_away_ms;
    /* time in msec for which AT had tuned away */

  uint8     sa;
    /* Subaset index */

  uint8                      demod_carrier_mask;
    /* Demod carrier mask */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

  hdrrmac_powerup_tx(
     #ifdef FEATURE_HDR_QTA
     hdrsrchtc.tune_away.is_qta
     #endif /* FEATURE_HDR_QTA */
     );

    /* send command to TX task to enable TX */

#ifdef FEATURE_HDR_IS890
  for( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
  {
    hdrsrchtc_drc_param_ctrl( hdrsrchset.subaset[ sa ].demod_idx );
      /* allow the new drc value to be activated if it was set during transition
         to suspended states */
  } /* for */
#endif

  /*---------------------------*
   * Reset Pkt Sequence number *
   *---------------------------*/

  hdrsrchtc_sched();
    /* start search schedular and send first set of searches */

  tune_away_ms = hdrsrchutil_read_stop_watch();
    /* read the time between start of tune away and present time */

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDED );
    /* We are starting Ramp */

  hdrrmac_resume_connection( ( int ) tune_away_ms
#ifdef FEATURE_HDR_QTA
, hdrsrchtc.tune_away.is_qta
#endif /* FEATURE_HDR_QTA */
  );
    /* let RMAC know that it can resume sending data on the conneciton */

#ifdef FEATURE_HDR_QTA
  if ( hdrsrchtc.tune_away.is_qta )
  {
    hdrsrchtc_set_qta_end( );
      /* Done with QTA now */
    rxlm_deallocate_buffer( hdrsrchtc.tune_away.rxlm_qta_target_buf_idx );
      /* Deallocate the buffer created for G QTA */
  }
#endif /* FEATURE_HDR_QTA */


  hdrsrchtc_enable_tune_aways();
    /* Enable tune aways now that the call is up */

  hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_INIT;
    /* init state for tune away */

  hdrsrchtc.tune_away.is_voip_tuneaway = FALSE;
    /* initialize to not an VOIP tuneaway */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Resume HDR TC after %dms", tune_away_ms );

  hdrsrchlog_log_tune_away_completion_info();
    /* The tune away has completed. Now log the information about this tune
       away. */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  hdrsrchard_event( HDRSRCHARD_EVENT_RESUME );
#else
  hdrsrchrfdiv_diversity_ctrl( TRUE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
    /* Enable diversity after tuning back from 1x or OFS */

#ifdef FEATURE_HDR_REVB
  if ( hdrsrchafc_afc_uses_rot() )
  {
    demod_carrier_mask = hdrsrchutil_get_demod_carrier_mask();
      /* Get which demod carriers are enabled */

    hdrsrcheq_assign_eqs( demod_carrier_mask );
      /* Assign equalizers */
  }
#endif /* FEATURE_HDR_REVB */
} /* hdrsrchtc_tune_back */


/* EJECT */
/*===========================================================================

FUNCTION      HDRSRCHTC_TUNE_AWAY_DRC_RAMP_DOWN

DESCRIPTION

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchtc_tune_away_drc_ramp_down
(
  uint16                     ramp_down_slots
    /* how many slots (full slots) before tune away ? */
)
{
  uint16         curr_hstr, ta_hstr;
    /* current and tune away HSTR count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  curr_hstr = HDRHSTR_GET_COUNT();
    /* read the current HSTR count */

  hdrrmac_notify_suspend( ramp_down_slots );
    /* Inform RTC MAC of upcoming suspension */

  /*----------------------------*
   * Start DRC Rampdown         *
   *----------------------------*/

  ta_hstr = (curr_hstr + ramp_down_slots * 2) & 0xffff;
    /* move forward by the ramp down number of slots */

  /* Set for a complete tune away rather than just a diversity
     tune away */
  hdrsrchmsg_start_drc_rampdown( HDRDRC_TUNE_AWAY_BOTH,
                                 ta_hstr );
    /* Start DRC ramp down process */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Start DRC rampdown.hstr=%d tune=%d",
                  curr_hstr, ta_hstr );

  hdrsrchrfdiv_disable_div_ramp_down();
    /* Make sure the diversity ramp down procedure doesn't disrupt
       this DRC ramp down. */

}/* hdrsrchtc_tune_away_drc_ramp_down */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHTC_PRE_TUNE_AWAY_PROC

DESCRIPTION
  Stops search scehdular

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchtc_pre_tune_away_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Suspend RMAC connection" );

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_STARTING );
    /* We are starting Ramp */

  hdrrmac_suspend_connection();
    /* suspend traffic connection */

  hdrdec_enter_mode ( HDRDEC_MONITOR_MODE );
    /* make sure that the decoder interrupt do not wait in the decoder
       buffer */

} /* hdrsrchtc_pre_tune_away_proc */

/*===========================================================================

FUNCTION      HDRSRCHTC_TUNE_AWAY_SET_HO_OVERRIDE_MODE

DESCRIPTION
  This function saves the best asp index and sets ho override mode at tuneaway.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void hdrsrchtc_tune_away_set_ho_override_mode( void )
{
  uint8            sa;
    /* Subaset index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
  {
    /* Save best ASP and set ho override for each subaset */
    hdrsrchset.subaset[sa].tune_away_ho_override = TRUE;
    hdrsrchset.subaset[sa].tune_away_basp_index = 
        HDRDRC_GET_VAR_BEST_ASP_PRED( hdrsrchset.subaset[ sa ].demod_idx );

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Tune away demod idx %d basp idx %d", 
                    hdrsrchset.subaset[ sa ].demod_idx, 
                    hdrsrchset.subaset[ sa ].tune_away_basp_index );

  }
} /* hdrsrchtc_tune_away_set_ho_override_mode */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHTC_TUNE_AWAY_PROC

DESCRIPTION
  Stops search scehdular

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/

void hdrsrchtc_tune_away_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_start_stop_watch();
    /* start measuring for how long AT stays away from HDR traffic */

#ifdef FEATURE_HDR_QTA
  /* In case of QTA donot powerdown RF, let FW do it */
  if ( hdrsrchtc.tune_away.is_qta )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "hdrsrchutil_powerdown_tx");
    hdrsrchutil_powerdown_tx( HDRRMAC_TX_STOP_FOR_TUNEAWAY_QTA );
      /* RF Tx chain is disabled (instant) */
  } 
  else
#endif /* FEATURE_HDR_QTA */
  {
    hdrsrchafc_rotator_push();
      /* Push rotator samples */
   
    hdrsrchafc_update_rgs();
      /* Report RGS */
   
    hdrsrchtc_tune_away_set_ho_override_mode();
      /* Save best ASP and set override mode at tuneaway */
   
    hdrsrchfing_deassign_all_fings( );
      /* Unassign fingers */
   
    if ( hdrsrchafc_afc_uses_rot() )
    {
      hdrsrcheq_deassign_all_eqs( );
        /* Deassign all equalizers before TCA tune (before rx_stop) */
    }
   
    hdrsrchutil_dec_deactivate( );
      /* deactivate HDR decoder before going to 1x */
   
    hdrsrchrf_pdm_capture_values();
      /* Capture PDM values for fast RF warmup */
   
    hdrsrchafc_stop_fll_tracking();
      /* Disable FLL tracking and save the FLL accum value. */
   
   
    hdrsrchutil_powerdown_tx( HDRRMAC_TX_STOP_FOR_TUNEAWAY );
      /* RF Tx chain is disabled (instant) */
   
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_SUSPEND );
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
      /* Disable diversity before going into sleep */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
    if ( hdrsrchtc.d2lr_state != HDRSRCHTC_D2LR_DRC_RAMPDOWN )
      /* If we are going to perform LTE meas, RF needs to stay in RX state for
         LTE meas. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
    {
      hdrsrchrf_prep_to_sleep();
        /* Prepare the RF for sleep */
    }

  }
  
  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_STARTED );
    /* The RF is now gone. */
}/* hdrsrchtc_tune_away_proc */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_OFS_ON_SEARCH_IDLE_CB

DESCRIPTION
  This callback function is invoked when searcher is idle during TC OFS.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_ofs_on_search_idle_cb( void )
{

  hdrsrchtc_tune_away_proc();
    /* perform tune away processing */

  hdrsrchofs_init_ofs_state();
    /* Begin OFS processing */

} /* hdrsrchtc_ofs_on_search_idle_cb */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_OFS_TUNE_AWAY_PROC

DESCRIPTION
  This function is called before a traffic state to traffic OFS state
  transition. It "ramps" down DRC requests and stops RMAC before tuning
  to a new channel for off-frequency search. The idea behind this 
  process is the same as that in HDR traffic before tuning to 1X.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_ofs_tune_away_proc( void )
{

  uint16  next_event_ms = 0;
    /* when do we want to check it */

  uint16  drc_rampdown_slots;
    /* the duration of the DRC rampdown */

  uint16  current_hs;
    /* the current hstr value */

  uint16  tuneaway_hs;
    /* the current hstr value */

  uint16  frame_offset;
    /* transmit frame offset */

  uint16  drc_length;
    /* DRC length in half slots */
        
  uint16  hs_from_scc;
    /* the distance to previous scc boundary in half slots */
    
  uint16  drc_boundary_hs;
    /* the distance to previuos DRC boundary in half slots */
    
  uint16  next_event_hs;
    /* time to next event in half slots */
              
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_NO_CONFLICT )
  {
  
#if ( HDRSRCHTC_SHORT_OFS_TUNEAWAYS_ENABLED == TRUE )

    hdrsrchtc.tune_away.is_voip_tuneaway = 
       ( hdrsrchtc_get_tc_reason() == HDRSRCHRF_LOW_LAT_TRAFFIC );
      /* If low latency traffic use short tuneaway timeline for OFS */

#else
    hdrsrchtc.tune_away.is_voip_tuneaway = FALSE;
      /* disable the optimized timeline in software 
         regular tuneaway will be used for all OFS tuneaways */
#endif
    
    if ( hdrsrchtc.tune_away.is_voip_tuneaway == TRUE )
    {
      current_hs = HDRHSTR_GET_COUNT();
        /* get current half slot count */
        
      tuneaway_hs = current_hs + ( 2 * HDRSRCHTC_VOIP_OUTAGE_NOTIFY_SLOTS );
        /* Calculate the tuneaway HSTR */
      
      /* Avoid tuning away around CC boundary 
         delay or advance tuneaway time */
      hs_from_scc = tuneaway_hs % ( 2 * HDRTS_SLOTS_PER_SCC ) ;
        /* calculate tuneaway time distance to scc boundary */
        
      if ( hs_from_scc > HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH )
      {
        tuneaway_hs -= ( hs_from_scc - HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH );
      }
      else if ( hs_from_scc < HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_LOW )
      {
        tuneaway_hs += (HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_LOW - hs_from_scc );
      }
          
      frame_offset = hdrsrch.frame_offset;
        /* Get frame offset*/
              
      if ( frame_offset > 15 )
      {
        frame_offset = 0;
          /* If read invalid frame offset, use 0 as frame offset */
      }
      
#ifdef FEATURE_HDR_REVB
      drc_length = 2 * hdrfmac_get_drc_length( hdrsrchset.subaset[ 0 ].tca_idx );
        /* get DRC length in half slots */
#else
      drc_length = 2 * HDRDRC_GET_DRC_LENGTH();
        /* get DRC length in half slots */
#endif /* FEATURE_HDR_REVB */
      
      /* round tuneway time to nearest DRC boundary taking into acccount 
         the reverse link frame offset so that 
         ( Tuneaway time - Frame offset) mod DRC_LENGTH = 0 */
         
      drc_boundary_hs = ( ( tuneaway_hs - ( 2 * frame_offset ) ) % drc_length ) ;
        /* calculate distance to previous DRC boundary */
        
      tuneaway_hs -= drc_boundary_hs;
        /* round tuneaway time to DRC boundary just before original tuneaway time */
                
      if ( drc_boundary_hs > ( drc_length / 2 ) )
      {
        tuneaway_hs += drc_length ;
          /* round tuneaway time to DRC boundary just after original tuneaway time */
      }
            
      voip_tuneaway.tuneaway_hs = tuneaway_hs;
        /* record tuneaway halfslot */

      /* Send outage notification to VOIP services 
         hdrsrchtc_send_voip_outage_notification() */
         
      voip_tuneaway.outage_notify_hs = current_hs;
        /* record the halfslot the outage notification has been sent */         
               
      current_hs = HDRHSTR_GET_COUNT();
        /* get current half slot count */

      voip_tuneaway.rampdown_hs = tuneaway_hs - (2 * HDRSRCHTC_VOIP_DRC_RAMP_SLOTS );
        /* calculate DRC rampdown start hstr from tuneaway time */
      
      next_event_hs =  voip_tuneaway.rampdown_hs - current_hs;
        /* calculate time to next event in half slots*/

      next_event_ms = HALFSLOTS_TO_MS( next_event_hs );
        /* Set next event to fire at DRC ramp down start time */

      if ( voip_tuneaway.outage_cb != NULL )
      {
        voip_tuneaway.outage_cb( (uint16) HALFSLOTS_TO_MS( ( next_event_hs + drc_length ) ),
                                 (uint16) HDRSRCHTC_VOIP_TUNEAWAY_OUTAGE_DURATION );
          /* send outage notifiction to FMAC */
      }
                                        
      HDR_MSG_SRCH_5( MSG_LEGACY_MED, "VOIP Outage Notif @hstr= %d, Tuneaway @ %d, Orig TA (mod)= %d. DRC_Length= %d, next event= %d ",
                      current_hs, voip_tuneaway.tuneaway_hs, hs_from_scc, drc_length, next_event_hs );

      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "VOIP Outage Notif Time2Outage= %d ms, Duration= %d ms",
                      HALFSLOTS_TO_MS( ( next_event_hs + drc_length ) ),
                      HDRSRCHTC_VOIP_TUNEAWAY_OUTAGE_DURATION );
                      
      hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_VOIP_OUTAGE_NOTIFICATION ;
    }
    else
    {
      drc_rampdown_slots = MS_TO_SLOTS( HDRSRCHTC_DRC_RAMP_MS - HDRSRCHTC_OVERHEAD_MS );
        /* use regular DRC ramp down for non VOIP */

      hdrsrchtc_tune_away_drc_ramp_down( drc_rampdown_slots );
        /* start DRC ramp down */

      next_event_ms = HDRSRCHTC_DRC_RAMP_MS - HDRSRCHTC_PRE_TUNE_MS;
        /* Set next event to fire at the PRE_TUNE time */

      hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_DRC_RAMP;
    }
  }
  else if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_VOIP_OUTAGE_NOTIFICATION )
  {
    current_hs = HDRHSTR_GET_COUNT();

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Starting VOIP DRC rampdown @hstr= %d. estimated= %d ",
                    current_hs, voip_tuneaway.rampdown_hs );

    drc_rampdown_slots = HDRSRCHTC_VOIP_DRC_RAMP_SLOTS;
      /* use expedited DRC ramp down for VOIP */

    hdrsrchtc_tune_away_drc_ramp_down( drc_rampdown_slots );
      /* start DRC ramp down */

    voip_tuneaway.actual_rampdown_hs = current_hs;
      /* actual half slot DRC ramp down initiated */
    
    voip_tuneaway.pretune_hs = voip_tuneaway.tuneaway_hs - 
                               ( 2 * HDRSRCHTC_VOIP_PRETUNE_SLOTS );
                               
    next_event_hs =  voip_tuneaway.pretune_hs - current_hs;
      /* calculate time to next event in half slots */
    
    /* safety check against a pretune time in the past 
       or against very immediate pretune event */
    if ( next_event_hs > HDRSRCHTC_VOIP_DRC_RAMPDOWN_TO_PRETUNE_MAX_HALFSLOTS )
    {
      next_event_hs = HDRSRCHTC_VOIP_TUNEAWAY_MIN_TIMER_HALFSLOTS;
    }
    else if ( next_event_hs < HDRSRCHTC_VOIP_TUNEAWAY_MIN_TIMER_HALFSLOTS )
    {
      next_event_hs = HDRSRCHTC_VOIP_TUNEAWAY_MIN_TIMER_HALFSLOTS;
        /* ensure that next_event_hs is non zero and above minumum value */
    }
                                     
    next_event_ms = HALFSLOTS_TO_MS( next_event_hs );;
      /* Set next event to fire at the PRE_TUNE time */

    hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_DRC_RAMP;
  }
  else if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_DRC_RAMP )
  {
    current_hs = HDRHSTR_GET_COUNT();

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Starting pre tune away processing @hstr= %d, estimated= %d ",
                    current_hs, voip_tuneaway.pretune_hs );

    if ( hdrsrchtc.tune_away.is_voip_tuneaway == TRUE )
    {
      voip_tuneaway.actual_pretune_hs = current_hs;
        /* record actual half slot count at pre tune away processing time */
        
      next_event_hs = ( 2 * HDRSRCHTC_VOIP_PRETUNE_SLOTS );
        /* calculate time to next event in half slots */

      next_event_ms = HALFSLOTS_TO_MS( next_event_hs );
        /* Set next event to fire at the final TUNE_AWAY time */
    }
    else
    {
      next_event_ms = HDRSRCHTC_PRE_TUNE_MS - HDRSRCHTC_OVERHEAD_MS;
        /* Set next event to fire at the final TUNE_AWAY time */
    }

    hdrsrchtc_pre_tune_away_proc();
      /* perform pre-tune away processing */

    hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_TX_STOP;
  } 
  else if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_TX_STOP )
  {
    current_hs = HDRHSTR_GET_COUNT();

    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Starting tune away processing @hstr= %d ",
                    HDRHSTR_GET_COUNT() ); 

    if ( hdrsrchtc.tune_away.is_voip_tuneaway == TRUE )
    {
      voip_tuneaway.actual_tuneaway_hs = current_hs;
        /* record actual half slot count at tune away time */
    }
    
    hdrsrchdrv_abort_search( FALSE );
      /* Abort pending searches, if any */

    hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_GONE;

#ifdef FEATURE_HDR_QTA 
    hdrsrchtc.tune_away.is_qta = FALSE;
      /* Before OFS tune away set QTA flag to FALSE */
#endif /* FEATURE_HDR_QTA */ 

    hdrsrch_stop_timer( &hdrsrch.schedule_timer );
      /* Stop scheduler ... we are tuning away */

    next_event_ms = 0;

    hdrsrch_tune_away_on_searcher_idle( hdrsrchtc_ofs_on_search_idle_cb );
      /* change to traffic OFS state */
  }
  else
  {
      ERR_FATAL("Unexpected tune away state", 
                hdrsrchtc.tune_away.state, 0, 0);
  }

  if ( next_event_ms )
  {
    (void) rex_set_timer( HDRSRCHTC_OFS_TA_TIMER, next_event_ms );
      /* Poll timer for testing FLL Acq status */

    hdrsrch_add_to_wait_mask( HDRSRCHTC_OFS_TA_SIG );
      /* Watch for the OFS TA timeout */
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "HDR TC OFS tune-away state %d, next event in %dms",
                  hdrsrchtc.tune_away.state, next_event_ms );

} /* hdrsrchtc_ofs_tune_away_proc */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_DETERMINE_NEXT_OFS_EVENT_TIME

DESCRIPTION
  This function determines the time of the next OFS event. 
  The next event may need to be scheduled due to a RUP request search, 
  OFS repetition search or to evaluate OFS condition on poll time.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  ofs_event_ms - time to next OFS event.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL unsigned hdrsrchtc_determine_next_ofs_event_time( void )
{
  unsigned                   ofs_event_ms = 100000;
    /* when do we want to check it 
       initialize to a large number (100 secs )*/

  timetick_type              sclks_now;
    /* current time in sclks */

  timetick_type              sclks_tuneaway;
    /* tuneaway time in sclks */

  timetick_type              sclks_left;
    /* tuneaway time in sclks */

  int32                      sclks_dither;
    /* dither in tuneaway time in sclks */

#ifdef FEATURE_HDR_ENHANCED_OFS
  uint16                     tuneaway_hs;
    /* half slot count at tuneaway time */
#endif /* FEATURE_HDR_ENHANCED_OFS */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check whether there is a pending RUP request */
  if ( hdrsrchofs_rup_pending() == TRUE )
  {
    sclks_tuneaway = ( hdrsrchtc.tune_away.ofs_time_sclks 
                       + HDRSRCHTC_OFS_RUP_SRCH_INTERVAL_SCLKS );
      /* Calculate tune away process start time to perform RUP search */
  }   
  /* Check whether RUP or OFS search repetition is pending 
     if so set primary chain event timer to go off at correct time */        
  else if ( hdrsrchofs_repetition_required() == TRUE )
  {
    sclks_tuneaway = ( hdrsrchtc.tune_away.ofs_time_sclks 
                       + HDRSRCHTC_OFS_REPEAT_INTERVAL_SCLKS );
      /* Calculate tune away process start time to repeat OFS search */
  }
  else
  {
    sclks_tuneaway = ( hdrsrchtc.tune_away.ofs_check_time_sclks 
                       + TIMETICK_SCLK_FROM_MS( HDRSRCHTC_OFS_CHECK_INTERVAL_MS ) );
      /* Calculate time to check OFS condition again */    
  }
  
  sclks_now = timetick_get();
    /* Current time in sclks. */
    
  if ( ( (int32) sclks_tuneaway - (int32) sclks_now ) < 0 )
  {
    sclks_left = 0;
      /* we have already passed the desired tune away time */
  }
  else
  {
    sclks_left = sclks_tuneaway - sclks_now;
      /* Calculate time left for tune away to repeat OFS search */
  }

  /* If sclks_left is close to ~1ms (1.e between 0-32 sclks), then we are
     already processing the event so there is no need to schedule yet
     another one.
     If OFS is disabled then we don't need to schedule OFS events either. */
  if ( ( sclks_left <= TIMETICK_SCLK_FROM_MS(1)) || 
       ( hdrsrchtc.ofs_disabled == TRUE ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Skip scheduling an event for OFS processing");
    
    /* Leave the OFS event time at 10 seconds. */
  }
  else
  {
#ifdef FEATURE_HDR_ENHANCED_OFS
  
    /* Just to make sure that we stay in traffic state 
       for a while before repeating OFS */
    if ( sclks_left <= HDRSRCHOFSTC_80MS_IN_SCLKS )
    {
      sclks_left = HDRSRCHOFSTC_80MS_IN_SCLKS;
    }
  
    /* Calculate halfslot count at expected tuneaway time */
    tuneaway_hs = HDRHSTR_GET_COUNT();
      /* get current hstr count */
        
    tuneaway_hs += ( ( timetick_cvt_from_sclk( sclks_left, T_MSEC ) * 6 ) / 5 );
      /* advance by time to tuneaway timeline start */
  
#if ( HDRSRCHTC_SHORT_OFS_TUNEAWAYS_ENABLED == TRUE )
    hdrsrchtc.tune_away.is_voip_tuneaway = 
       ( hdrsrchtc_get_tc_reason() == HDRSRCHRF_LOW_LAT_TRAFFIC );
      /* short tuneaway will be used for OFS tuneaway */
#else
    hdrsrchtc.tune_away.is_voip_tuneaway = FALSE;
      /* regular tuneaway will be used for OFS tuneaway */
#endif
  
    if ( hdrsrchtc.tune_away.is_voip_tuneaway == TRUE )
    {
      tuneaway_hs += ( 2 * HDRSRCHTC_VOIP_OUTAGE_NOTIFY_SLOTS );
        /* advance by expected VOIP tuneaway timeline duration */
    }
    else
    {
      tuneaway_hs += MS_TO_HALFSLOTS( HDRSRCHTC_DRC_RAMP_MS - HDRSRCHTC_OVERHEAD_MS );
    }
  
    tuneaway_hs %= ( 2 * HDRTS_SLOTS_PER_SCC ) ;
      /* calculate tuneaway time distance to SCC boundary */
      
    /* If tuneaway falls near SCC boundary  
       then advance or delay tuneaway time */
    if ( tuneaway_hs  > HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH )
    {
      sclks_dither = ( HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_HIGH - tuneaway_hs ) *
                       HDRSRCHOFSTC_HALFSLOT_IN_SCLKS;      
    }
    else if ( tuneaway_hs  < HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_LOW )
    {
      sclks_dither = ( HDRSRCHTC_NO_VOIP_TUNEAWAY_HSTR_LOW - tuneaway_hs ) * 
                       HDRSRCHOFSTC_HALFSLOT_IN_SCLKS;
    }
    else
    {
      sclks_dither = 0;
    }
#else
    sclks_dither = 0;
      /* do not dither OFS timeline */  
#endif /* FEATURE_HDR_ENHANCED_OFS */
  
    sclks_left += sclks_dither;
      /* Adjust tuneaway time if it falls around CC boundary */
  
    ofs_event_ms = timetick_cvt_from_sclk( sclks_left, T_MSEC );
  
    HDR_MSG_SRCH_7( MSG_LEGACY_MED, "OFS last= %d now = %d left= %d dither= %d. HSTR= %d, Next event=%d ms ID=%d",
                    hdrsrchtc.tune_away.ofs_time_sclks,
                    sclks_now,
                    sclks_left,
                    sclks_dither,
                    HDRHSTR_GET_COUNT(),
                    ofs_event_ms,
                    hdrsrchtc.tune_away.state );

  }

  return ( ofs_event_ms );
  
} /* hdrsrchtc_determine_next_ofs_event_time() */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_ON_SEARCH_IDLE_CB

DESCRIPTION
  This callback function is invoked when searcher is idle during tune away
  processing.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_on_search_idle_cb( void )
{

  hdrsrchtc_tune_away_proc();
    /* perform tune away processing */

  hdrsrchsus_init_suspended_state();
    /* Begin suspended TC state */

} /* hdrsrchtc_on_search_idle_cb */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_PROCESS_TUNE_AWAY

DESCRIPTION
  This function performs proper tune away procedure based on the time that
  is currently left on the lock and the current tune away state. It then
  schedules the next event to be processed. Note if the next event to
  be processed is later than the next time OFS should be considered, then
  the next event scheduled will be based on the next OFS poll time.

DEPENDENCIES
  None

PARAMETERS
  extend_ms - maximum duration for which we can have the RF.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_process_tune_away
(
  unsigned                   extend_ms
    /* maximum duration for which we can have the RF */
)
{
  unsigned                   next_event_ms = 0;
    /* when do we want to check it */

  unsigned                   ofs_event_ms = 0;
    /* when do we want to check it */

#ifdef FEATURE_HDR_QTA

  hdrsrchrf_trm_freq_info_type   *freq_info_ptr;
   /* Pointer to get frequency info */

  int                            rx_agc0 = 0, rx_agc1 = 0;
    /* Rx Strength */

  boolean                        diversity;
    /* diversity setting */

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  rfm_hdr_qta_params             *rf_qta_param = NULL ; 
     /* Param to RF for Intelliceiver disable */

  rfm_meas_header_type           *header = NULL;
     /* To access header parameters locally */

  rfm_meas_hdr_params_type       *hdr_params = NULL;
     /* To access hdr parameters locally */
	 
  lm_status_type lm_buffer_allocated = LM_INVALID_PARAMETER; /* rxlm Buffer */	 
#endif /* FEATURE_HDR_QTA */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( extend_ms > HDRSRCHTC_DRC_RAMP_MS + 5 )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR ext ms %d", extend_ms );
    /*-----------------------------------*\
    |* Lock extend between 148 and 433 ms *|
    \*-----------------------------------*/


    /* We are far enough away that we do not need to be ramping the DRC.
       If we are, because some module cancelled a lock request and we
       ended up with more extend time, stop the DRC ramp */

    if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_DRC_RAMP )
    {
      /* we came back to begining of tune away timeline while we were
         in DRC ramp down state */

      hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                           sizeof( hdrfw_tuneaway_stop_msg_t ) );
        /* get back to normal mode of DRC i.e. stop DRC ramp down mode */

      hdrsrchrfdiv_enable_div_ramp_down();
        /* Enable diversity ramp down. */
      
      hdrrmac_notify_suspend( HDRRMAC_CANCEL_SUSPEND );
        /* Tell MAC to cancel the scheduled suspend time. */
    }
    else if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_TX_STOP )
    {
      /* we came back to begining of tune away timeline while we were
         in tx stopped state */

      hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                         sizeof( hdrfw_tuneaway_stop_msg_t ) );
        /* get back to normal mode of DRC i.e. stop DRC ramp down mode */

      hdrsrchrfdiv_enable_div_ramp_down();
        /* Enable diversity ramp down. */

      hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDED );
        /* We are starting Ramp */

      hdrrmac_notify_suspend( HDRRMAC_CANCEL_SUSPEND );
        /* Tell MAC to cancel the scheduled suspend time. */

      hdrrmac_resume_connection( 0
#ifdef FEATURE_HDR_QTA
      , hdrsrchtc.tune_away.is_qta
#endif /* FEATURE_HDR_QTA */
      );

        /* resume traffic connection */

      hdrsrchtc_sched();
        /* start search schedular and send first set of searches */

      hdrdec_enter_mode ( HDRDEC_TRAFFIC_MODE );
        /* Set decoder for traffic mode */
    }

    next_event_ms = extend_ms - HDRSRCHTC_DRC_RAMP_MS;
      /* Set next event to fire when the DRC ramp must start, assuming the
         lock cannot be further extended.  If the maximum length extension
         is granted, this duration simplifies to HDRSRCHTC_EXTEND_POLL_MS. */

    hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_NO_CONFLICT;
      /* We are not ramping the DRC */

    ofs_event_ms = hdrsrchtc_determine_next_ofs_event_time();
      /* determine next ofs event time */

    /* Set timer to go off for repeat OFS search */
    if ( ofs_event_ms < next_event_ms )
    {
        next_event_ms = ofs_event_ms;
    }
    /* Must come back and check to see if it is a good time for OFS if 
       we aren't in a tune away and the next event is farther away than 
       when we are supposed to check for OFS. */
  }
  else if ( extend_ms > HDRSRCHTC_PRE_TUNE_MS )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR ext ms %d", extend_ms );
    /*-----------------------------------*\
    |* Lock extend between 43 and 147 ms *|
    \*-----------------------------------*/

    if ( hdrsrchtc.tune_away.state != HDRSRCHTC_TUNE_AWAY_DRC_RAMP)
    {
      /* Event 1 : DRC Rampdown at between 43 and 147ms from tuneAway */

      hdrsrchtc_tune_away_drc_ramp_down(
           MS_TO_SLOTS( extend_ms - HDRSRCHTC_OVERHEAD_MS ) );
        /* start tune away for remaining time */

      hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_DRC_RAMP;
    }

    next_event_ms = extend_ms - HDRSRCHTC_PRE_TUNE_MS;
      /* Set next event to fire at the PRE_TUNE time */
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR ext ms %d", extend_ms );
    /*---------------------------------*\
    |* Lock extend between 0 and 42 ms *|
    \*---------------------------------*/

#ifdef FEATURE_HDR_DR_DSDS
    if ( hdrmultirat_is_dr_dsds_enabled() )
    {
      if ( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_NO_CONFLICT )
      {
        /* If current time is 4ms( 2 slots ) before overhead, start drc ramp down at overhead time.
           Otherwise, drc ramp down start at 4ms (2 slots) from now */
        if ( extend_ms > (HDRSRCHTC_OVERHEAD_MS + 4 ) )
        {
          hdrsrchtc_tune_away_drc_ramp_down(
               MS_TO_SLOTS( extend_ms - HDRSRCHTC_OVERHEAD_MS ) );
            /* start tune away for remaining time */

        }
        else
        {
          hdrsrchtc_tune_away_drc_ramp_down( 2 );
            /* start tune away for 2 slots from now */
        }

        hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_DRC_RAMP;

      }
    } /* if hdrmultirat_is_dr_dsds_enabled */
#endif /* FEATURE_HDR_DR_DSDS */

    if ( hdrsrchtc.tune_away.state != HDRSRCHTC_TUNE_AWAY_TX_STOP )
    {
      /* Event 2 : Pre-tune away proc at between 15ms and 42ms from tuneAway */
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR TA state %d",
                      hdrsrchtc.tune_away.state );

      hdrsrchtc_pre_tune_away_proc();
        /* perform pre-tune away processing */

      hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_TX_STOP;
    }


    if ( extend_ms <= HDRSRCHTC_OVERHEAD_MS + 5 )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR ext ms %d", extend_ms );
      /* Event 3 : Tune Away Proc at less than 15ms from tuneAway */

      hdrsrchdrv_abort_search( FALSE );
        /* Abort pending searches, if any   */

      hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_GONE;

      hdrsrch_stop_timer( &hdrsrch.schedule_timer );
        /* Stop scheduler ... we are tuning away */

#ifdef FEATURE_HDR_QTA
      hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
        /* Retrieve the retain lock state. */

      hdrsrchtc_update_qta_mode( &rl_state );
        /* Based on current winning clinet check QTA condition */

      if( ( ( rl_state.unlock_by_sclk - hdrsrchtc.tune_away.sclks_at_retain_lock ) 
          <= HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK ) &&
            ( hdrsrchtc.tune_away.is_qta == TRUE ) )
      {
        /* If the TA time requested is less than min ramp down time then cancel QTA and 
           QTA is allowed */
        hdrsrchtc.tune_away.is_qta = FALSE;

        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                        "HDR_TA: QTA TA Time requested is %d < TA overhead %d sclks, do LTA", 
                        (rl_state.unlock_by_sclk - hdrsrchtc.tune_away.sclks_at_retain_lock), 
                        HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK);
      }

      hdrsrchtc_update_qta_on_rlsi( extend_ms );
        /* Disable QTA if it coincides RLSI */

      hdrsrchtc_disable_tune_aways();
        /* Disable tune aways now because we do not permit processing unlock
           cancels which will abort the tune away at this stage of the
           process. */

#ifdef FEATURE_HDR_SELECTIVE_TXD
      if ( hdrsrchtc.tune_away.is_qta )
      {
        hdrsrchtc.tune_away.is_qta = 
          hdrsrchrftxd_suspend_asdiv( TRUE, 
                                      TRM_ASDIV_STATE_UPDATE_REASON_QTA );
           /* Block ASDiv antenna switch while in QTA */
      }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      if ( hdrsrchtc.tune_away.is_qta )
      {
        if ( ( rfm_hdr_prep_qta ( hdrsrchrf_get_rf_device( TRM_HDR ), rf_qta_param )) == TRUE )
        {
          freq_info_ptr = hdrsrchrf_get_trm_freq_info();
            /* Obtain band/chan information for TRM request. */

          header = &(hdrsrchtc.tune_away.rf_params.header);
		  /*Initialize RF header */
          rfm_meas_common_init_rf_header(header);


          header->source_tech = RFM_1XEVDO_MODE;
          header->num_source_item = 1;
          header->num_target_item = 1;
          header->source_param_v2[0].rx_dev_handle[0].device = 
            hdrsrchrf_get_rf_device( TRM_HDR );

          hdrsrchrf_get_rxlm_buffer_handle( 
                 HDRSRCHRF_PRI_CHAIN,
                 &(header->source_param_v2[0].rx_dev_handle[0].lm_handle ));

          header->source_param_v2[0].band = 
            freq_info_ptr->freq_info.band;

          hdr_params = &(hdrsrchtc.tune_away.rf_params.source_param.hdr_params);
          hdr_params->band_chan.band = freq_info_ptr->freq_info.band;
          hdr_params->band_chan.chan_num = freq_info_ptr->freq_info.channels[0];
          hdr_params->agc_mode = RFM_HDR_AGC_MODE_FAST;
          hdr_params->agc_rtc_offset =
                           hdrsrchutil_get_agc_rtcoffset();
          hdrsrchrf_get_rx_power( &rx_agc0, &rx_agc1, &diversity );
           /* read the RF parameters and diversity settings */
          hdr_params->prev_agc = rx_agc0;

          hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
            /* Retrieve the retain lock state info */

          hdrsrchrf_set_qta_winning_client_id();
          /* Store the winning client ID of the current QTA */

            /* Fill target client specific RF header params */
          switch ( rl_state.winning_client.client_id )
          {
            case TRM_GSM1:
            case TRM_GSM2:
            case TRM_GSM3:
              header->target_tech = RFM_EGSM_MODE; 
              lm_buffer_allocated = rxlm_allocate_buffer( RXLM_CHAIN_0, LM_GSM,
                 &(header->target_param_v2[0].rx_dev_handle[0].lm_handle ));

              break;

            case TRM_UMTS:
              header->target_tech = RFM_IMT_MODE; 
              lm_buffer_allocated = rxlm_allocate_buffer( RXLM_CHAIN_0, LM_UMTS,
                 &(header->target_param_v2[0].rx_dev_handle[0].lm_handle ));
              break;

            case TRM_UMTS2:
              header->target_tech = RFM_IMT_MODE_2; 
              lm_buffer_allocated = rxlm_allocate_buffer( RXLM_CHAIN_0, LM_UMTS,
                 &(header->target_param_v2[0].rx_dev_handle[0].lm_handle ));
              break;

            default:
              break;
          }

          if ( ( lm_buffer_allocated == LM_SUCCESS ) &&
               ( hdrsrchrf_is_qta_allowed(rl_state.winning_client.client_id) ) )
          {
            /* Save target QTA client RxLM index*/
            hdrsrchtc.tune_away.rxlm_qta_target_buf_idx = 
                header->target_param_v2[0].rx_dev_handle[0].lm_handle;

            /* Note: Do not change below order
             
               if HDR does not have diversity
                - Cancel/suspend ARD so that HDR does not get TRM grant while in to QTA
                - Inform FW about QTA start [Should be before TRM set_client_state]
                - Inform TRM about QTA through set_client_state with Div protect
               else
                - Inform FW about QTA start [Should be before TRM set_client_state]
                - Inform TRM about QTA through set_client_state with Div protect
                - Release diversity [Should be done after TRM set_client_state]
            */

            if ( !hdrsrchrfdiv_diversity_is_enabled() )
            {
              /* This is to cancel any pending reservation, so that HDR does
                 not get grant Div after entering QTA gap */
              hdrsrchrfdiv_diversity_disable_for_qta();
            }

            /* Let FW know QTA is starting. Moved this ahead of set client to
               TRM intentionally as we have seen a crash where FW received the
               RX_START_CFG with scripts information even before SW sent QTA
               start. As per FW team they will not store scripts, needed during
               tune back, if received prior to QTA Start msg. */
            HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR QTA start to FW  ");
            hdrmultirat_send_qta_cfg_msg( HDRMULTIRAT_QTA_START );

            /* Let TRM know that QTA going to start to avoid granting chain to 
             * high priority clients while QTA is started */
            hdrsrchrf_set_qta_client_state
              (
                HDRSRCHRF_QTA_START,
                &(hdrsrchtc.tune_away.rf_params)
              );

            if ( hdrsrchrfdiv_diversity_is_enabled() )
            {
              /* Suspend ARD / Release diversity if we have Div chain.
                 It is safe to do here as we have already protected Div for QTA
                 in set_client_state to TRM. This will ensure we get back Div
                 when requested after QTA gap */
              hdrsrchrfdiv_diversity_disable_for_qta();
            }

#ifdef FEATURE_HDR_SELECTIVE_TXD
            hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_UNIFIED_MICRO_PRIORITY
            hdrmultirat_priority_switch( HDRMULTIRAT_LOW_PRIORITY_QTA );
#else
            hdrmultirat_priority_switch( HDRMULTIRAT_LOWER_PRIORITY_25 );
#endif /* FEATURE_HDR_UNIFIED_MICRO_PRIORITY */
          }
          else
          {
            hdrsrchtc.tune_away.is_qta = FALSE;
                 /* winning client has changed and disable QTA */

	    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
	                  "HDR QTA: Disable QTA!!!, win_client=%d, lm_buf_status=%d",
                           rl_state.winning_client.client_id, 
                           lm_buffer_allocated );

			
             if( rfm_hdr_end_qta ( hdrsrchrf_get_rf_device( TRM_HDR ) ) == FALSE ) 
             {
                HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR QTA: rfm_hdr_end_qta Failed!!!");
             }
			
             if ( lm_buffer_allocated == LM_SUCCESS )
             {
              rxlm_deallocate_buffer( header->target_param_v2[0].rx_dev_handle[0].lm_handle );
          /* Deallocate the buffer created for G QTA */
             }
	   }
         }
         else
         {
		  HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                                "HDR QTA: Disable QTA!!! - rfm_hdr_prep_qta failed ");
		  hdrsrchtc.tune_away.is_qta = FALSE;
         }
      }
      else
      {
#ifdef FEATURE_HDR_SELECTIVE_TXD
        hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
		/* Send suspend txd cmd in case of lta */
#endif /* FEATURE_HDR_SELECTIVE_TXD */
      }
#endif /* FEATURE_HDR_QTA */

      HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "HDR DLR SRCH_IDLE_CB ext ms %d",
                                                            extend_ms );
      hdrsrch_tune_away_on_searcher_idle( hdrsrchtc_on_search_idle_cb );
        /* change to suspended traffic state, only when searcher is idle  */
    }
    else
    {
      next_event_ms = extend_ms - HDRSRCHTC_OVERHEAD_MS;
        /* Next event at the overhead time-mark. */
    }
  }

  /* Program next event if necessary */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR next_evt ms %d", next_event_ms );

  if ( next_event_ms )
  {
    if ( next_event_ms < 5 )
    {
      next_event_ms = 5;
    }
      /* ensure at least 5 ms are set to rex timer for proper operation */

    (void) rex_set_timer( &hdrsrchtc.tune_away.timer, next_event_ms );
      /* set timer for next event */

    if ( hdrsrchtc.tune_away.state !=  HDRSRCHTC_TUNE_AWAY_NO_CONFLICT )
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "%dms from taway Next event=%dms ID=%d",
                      extend_ms, next_event_ms, hdrsrchtc.tune_away.state );
    }
    else
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Next event=%dms ID=%d",
                      next_event_ms, hdrsrchtc.tune_away.state );
    }

  }/* if next event */
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR next_evt ms is 0");
  }
  
} /* hdrsrchtc_process_tune_away( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_IS_OFS_TUNEAWAY_REQUIRED

DESCRIPTION
  This function determines whether we need to initiate a tuneaway now
  for performing OFS or RUP request search.
   
DEPENDENCIES
  None
  
PARAMETERS
  None
  
RETURN VALUE
  init_ofs_tuneaway - whether need to initiate tuneaway for OFS. 

SIDE EFFECTS
  None

===========================================================================*/

LOCAL boolean hdrsrchtc_is_ofs_tuneaway_required( void )
{
  timetick_type                  sclks_now;
    /* current sclks  count */

  timetick_type                  sclks_delta;
    /* Sclks sice last OFS search */

  boolean                        init_ofs_tuneaway = FALSE;
    /* whether to initiate tuneaway for OFS or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sclks_now = timetick_get();
    /* Current time in sclks. */ 

  if ( hdrsrchofs_rup_pending() == TRUE )
  {
    sclks_delta = sclks_now - hdrsrchtc.tune_away.ofs_time_sclks;
      /* Sclks sice last OFS search */
         
    if ( sclks_delta >= HDRSRCHTC_OFS_RUP_SRCH_MIN_INTERVAL_SCLKS )
    {
      init_ofs_tuneaway = TRUE;
        /* we will initiate OFS tuneaway to begin new RUP search */
    }
  }
  else if ( hdrsrchofs_repetition_required() == TRUE )
  {
    sclks_delta = sclks_now - hdrsrchtc.tune_away.ofs_time_sclks;
      /* Sclks sice last OFS search */
         
    if ( sclks_delta >= HDRSRCHTC_OFS_REPEAT_MIN_INTERVAL_SCLKS )
    {
      init_ofs_tuneaway = TRUE;
        /* we will initiate OFS tuneaway to repeat last search */
    }
  }
  else
  {
    sclks_delta = sclks_now - hdrsrchtc.tune_away.ofs_group_time_sclks;
      /* Sclks sice last OFS group search started */

    if ( sclks_delta >= HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_SCLKS )
    {
      hdrsrchtc.tune_away.ofs_check_time_sclks = sclks_now;
        /* update the last OFS check time, need to check 
           ofs condition again in HDRSRCHTC_OFS_CHECK_INTERVAL_MS */
        
      if ( hdrsrchofs_tc_ofs_reqd() == TRUE )
      {
        init_ofs_tuneaway = TRUE;
          /* we will switch to OFS state to begin new OFS search  */
              
        hdrsrchtc.tune_away.ofs_group_time_sclks = sclks_now;
          /* update the last OFS search group start time */
      }
    }
  }

  return ( init_ofs_tuneaway );
  
} /* hdrsrchtc_is_ofs_tuneaway_required() */


/*===========================================================================

FUNCTION HDRSRCHTC_DRC_RAMPDOWN_TIME_SCLK

DESCRIPTION
  This fucntion returns the DRC rampdown time.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  drc rampdown time

SIDE EFFECTS
  None

===========================================================================*/
LOCAL int32 hdrsrchtc_drc_rampdown_time_sclk( void )
{
#ifdef FEATURE_HDR_DR_DSDS
  if ( hdrmultirat_is_dr_dsds_enabled() )
  {
    return (int32) HDRSRCHTC_FAST_DRC_RAMP_MIN_SCLK;
  }
#endif /* FEATURE_HDR_DR_DSDS */

#ifdef FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION
  return (int32) HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK;
#else
  return (int32) HDRSRCHTC_DRC_RAMP_SCLK;
#endif /* FEATURE_HDR_DRC_RAMP_DOWN_OPTIMIZATION */

} /* hdrsrchtc_drc_rampdown_time_sclk */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_RF_UNLOCK_EVENT

DESCRIPTION
  Handle the incoming RF lock event based on the current state of the retain
  lock unlock information.  Schedule a DRC ramp down if we are supposed to
  tune away. If the required tune away is urgent, just force an unlock
  immediately.

  It may transition to TC OFS state when it detects that HDR has been in 
  traffic state for too long ( with FEATURE_HDR_TC_OFS_ENHANCEMENT defined ).

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_rf_unlock_event( void )
{

  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  timetick_type                  sclks_left;
    /* Sclks remaining until lock release */

  timetick_type                  sclks_now;
    /* current sclks  count */

  boolean                        init_ofs_tuneaway = FALSE;
    /* whether to switch to OFS state or not */

  boolean                         is_ta_updated = TRUE;
    /* Has tuneaway time been updated? */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  timetick_type                  d2l_sclks_left;
    /* Sclks remaining until internal rampdown */

  hdrsrchtc_d2l_redir_state_enum_type earlier_state;
	  /* D2LR state machine for logging purpose */

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "Primary chain unlock requested" );

  if ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "Primary chain unlock immediately requested" );
      /* Print out debug message. */

    hdrsrchtc_disable_tune_aways();
      /* Unregister for unlock callbacks.  We no longer care. */

    hdrsrch_advise_unlock_rf();
      /* Tell search to unlock the RF as soon as possible. */

    hdrsrchtc.pending_unlock_rf_immed = TRUE;
      /* Mark that UNLOCK RF command is pending for processing */
#ifdef FEATURE_HDR_QTA 
    hdrsrchtc.tune_away.is_qta = FALSE;
      /* clear if there is any pending QTA flag */
#endif /* FEATURE_HDR_QTA */ 
  }
  else if ( hdrsrchtc.tune_away.urgent_rel_only == FALSE )
  {

    sclks_now = timetick_get();
      /* Current time in sclks. */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION

    /*  TA was cancelled. Now determine if HDR is able to 
     *  handle this or ignore it
     *  If the DRC rampdown is due to internal tuneaway,
     *  do not handle unlock cb
     */
    if ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED && hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_DRC_RAMPDOWN )
    {
       HDR_MSG_SRCH_2( MSG_LEGACY_MED, "TRM unlock cancelled states %d %d"
                                       "Do not cancel internal DRC rampdown",
                    hdrsrchtc.tune_away.state,
                    hdrsrchtc.d2lr_state );

       sclks_left = hdrsrchtc.tune_away.end_time_sclks  - sclks_now;
       ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
        /* Convert lock extension to milliseconds. */

       HDR_MSG_SRCH_4( MSG_LEGACY_MED, "D2L: end_time_sclks %d sclks_now %d sclks_left %d ms_left %d ",
                    hdrsrchtc.tune_away.end_time_sclks,
                    sclks_now,
                    sclks_left,
                    ms_left );

       /* Do not honor unlock call back, instead use the old tuneaway time to continue with the TA */
       hdrsrchrf_set_lock_state( HDRSRCHRF_PRI_CHAIN, sclks_left );
    }
    else
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
    {
      
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
      /* If cancelling TA reset D2L state */
      if ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED  && hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_WAIT_MEAS )
      {
        hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;
        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Reset D2L state %d due to unlock cancel",
                        hdrsrchtc.d2lr_state );
      }
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#ifdef FEATURE_HDR_QTA
      hdrsrchtc_update_qta_mode( &rl_state );
        /* Based on current winning clinet check QTA condition */

      HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                      "HDR QTA curr %d unlock_by %d starting %d max %d (sclk)", 
                      sclks_now,
                      rl_state.unlock_by_sclk,
                      hdrsrchtc.tune_away.sclks_at_retain_lock, 
                      HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK);

      if( ( ( hdrsrchtc.tune_away.is_qta == TRUE ) &&
            ( ( rl_state.unlock_by_sclk - hdrsrchtc.tune_away.sclks_at_retain_lock ) 
                  <= HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK ) )
        )
        {
      /* If the TA time requested is less than min ramp down time then cancel QTA. 
      If converting QTA to LTA is also not useful ignore TA  */
          hdrsrchtc.tune_away.is_qta = FALSE;

      if ( rl_state.winning_client.reason == HDRSRCHRF_DEMOD_PAGE )
      {
        HDR_MSG_SRCH_4( 
          MSG_LEGACY_HIGH, 
          "HDR_TA: Cannot TA for page-demod timeline:%d < TA ovhd:%d sclks, exiting tuneaway process, Winning client:%d, reason:%d",
          (rl_state.unlock_by_sclk - hdrsrchtc.tune_away.sclks_at_retain_lock), HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK, 
          rl_state.winning_client.client_id, rl_state.winning_client.reason );

          return;
          /* Ignore the unlock notification !! */
      }
      else
      {
        HDR_MSG_SRCH_4( 
          MSG_LEGACY_HIGH, 
          "HDR_TA: QTA TA Time requested is %d < TA overhead %d sclks, process LTA instead of QTA, Winning client:%d, reason:%d",
                      (rl_state.unlock_by_sclk - hdrsrchtc.tune_away.sclks_at_retain_lock), 
          HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK,
          rl_state.winning_client.client_id, rl_state.winning_client.reason );
      }
        }
#endif /* FEATURE_HDR_QTA */

      sclks_left = (timetick_type) hdrsrchutil_calc_time_to_tune_away( 
                                            (uint32) sclks_now,
                                            hdrsrchtc_drc_rampdown_time_sclk(),
                                            &rl_state,
                                            &hdrsrchtc.tune_away.no_tune_away,
                                            &hdrsrchtc.tune_away.end_time_sclks,
                                            &is_ta_updated);
        /* How much more time should we stay in traffic? */
#ifdef FEATURE_HDR_QTA
      if ( !is_ta_updated )
      {
        hdrsrchtc.tune_away.is_qta = FALSE;

        HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "HDR QTA: TA time not updated disable QTA" );
      }
#endif /* FEATURE_HDR_QTA */

      hdrsrchtc.time_accum += 
        timetick_cvt_from_sclk((sclks_now - hdrsrchtc.tune_away.start_time_sclks),
                               T_MSEC);
        /* Figure out how much time has elapsed since the last OFS check. */

      hdrsrchtc.tune_away.start_time_sclks = sclks_now;
        /* Keep track of the current time. */

      ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
        /* Convert lock extension to milliseconds. */

#ifdef FEATURE_HDR_QTA
      hdrsrchtc_update_qta_on_rlsi( ms_left );
        /* Disable QTA if it coincides RLSI */
#endif

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
	earlier_state = hdrsrchtc.d2lr_state; 

    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "D2LR ext_unlock:%d state %d",
                    rl_state.ext_unlock,
                    hdrsrchtc.d2lr_state );

    if ( ( rl_state.ext_unlock == TRUE ) &&
           ( hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_DRC_RAMPDOWN ) )
    {
        if ( ms_left < HDRSRCHTC_MAX_TIME_TO_NEXT_TA ) 
        {
          HDR_MSG_SRCH( MSG_LEGACY_MED, "D2LR Cancelling internal rampdown due to new TA" );

      hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;
      hdrsrchtclte_release_trm_grant();
        /* Cancel outstnding TRM grant for D2L redirection, if any. */
      }
        else 
        {
          HDR_MSG_SRCH_2( MSG_LEGACY_MED, "TRM unlock by greater than max time, states %d %d"
                                          "Do not cancel internal DRC rampdown",
                                          hdrsrchtc.tune_away.state,
                                          hdrsrchtc.d2lr_state );

         d2l_sclks_left = hdrsrchtc.tune_away.end_time_sclks  - sclks_now;
         
         /* Do not honor unlock call back, instead use the old tuneaway time to continue with the TA */
         hdrsrchrf_set_lock_state( HDRSRCHRF_PRI_CHAIN, d2l_sclks_left );

        }
      }

   HDR_MSG_SRCH_4( MSG_LEGACY_MED, "D2LR MaxTime %d state %d ext_unlock:%d earlier DLR state %d",
                   HDRSRCHTC_MAX_TIME_TO_NEXT_TA,
                   hdrsrchtc.d2lr_state,
                   rl_state.ext_unlock,
                   earlier_state);

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

    HDR_MSG_SRCH_7( MSG_LEGACY_MED, 
                    "Unlock St: %d, %dms away State=%d SINR=%d ta.end_sclk=%d, unlock_by_sclk=%d  RF is tuned? (%d)",
                    rl_state.event,
                    ms_left,
                    hdrsrchtc.tune_away.state,
                    HDRASP_GET_FILT_SINR ( HDRMDSP_DEMOD_CARRIER_0, 
                             HDRASP_GET_BASP_FL( HDRMDSP_DEMOD_CARRIER_0 ) ),
                    hdrsrchtc.tune_away.end_time_sclks,
                    rl_state.unlock_by_sclk,
                    hdrsrchrf_is_tuned());
      /* Print out debug message. */

    HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "RF is tuned? (%d)", hdrsrchrf_is_tuned() );

#if defined FEATURE_HDR_JOLOKIA_MODEM && defined FEATURE_HDR_QTA
      if( !hdrsrchtc.tune_away.is_qta )
#endif
      {
        /* For QTA delay diversity execution till qta_start flag is set for diversity (Jolokia only) */
        hdrsrchrfdiv_resume_tuneaway();
          /* Send the command to the RF Diversity State Machine.
           This will kick start div tuneaway timer, if necessary. */
      }
       
      if ( ( (hdrsrchtc.substate != HDRSRCHTC_CONNECTION_CLOSE_INPROGRESS_SUBSTATE) &&
             (hdrsrchtc.substate != HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE) ) ||
                                              (!hdrsrch_is_lre_enabled()) )
      {
        /* if the TC state is not HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE and not in 
         HDRSRCHTC_CONNECTION_CLOSE_INPROGRESS_SUBSTATE then process RF unlock-by */
        if ( ( hdrsrchtc.ofs_disabled     ==  FALSE                           ) &&
             ( ms_left                    >   HDRSRCHTC_OFS_OVERHEAD_MS       ) &&
             ( hdrsrchtc.tune_away.state  ==  HDRSRCHTC_TUNE_AWAY_NO_CONFLICT ) &&
             ( hdrsrchrf_is_tuned()       ==  TRUE                            ) )
        {

          init_ofs_tuneaway = hdrsrchtc_is_ofs_tuneaway_required();
            /* determine whether we need to initiate tuneaway for OFS purposes */

          if ( init_ofs_tuneaway == TRUE )
          {
            hdrsrchtc.tune_away.urgent_rel_only = TRUE;
              /* Now don't be interrupted unless it is really necessary. */

            /* We tune to a neighbor HDR channel for off-frequency search when the 
               following conditions are met:

               1. More than HDRSRCHTC_OFS_GROUP_SEARCH_INTERVAL_MS has elapsed since
                 the last OFS group search.
              2. We can continue to hold the RF for "HDRSRCHTC_OFS_OVERHEAD_MS" ms,
                 which is long enough to ramp down the DRC and finish an OFS.
              3. We are not currently in the tuning away process.
              4. Other OFS conditions, as in idle,  are also satisfied as well.  */

            hdrsrchtc.tune_away.ofs_time_sclks = sclks_now;
              /* update the last OFS start time */
      
            hdrsrchutil_tc_ofs_start_gov();
              /* Reset the OFS rate governer timer    */

            hdrsrchtc_disable_tune_aways();
              /* disable tuneaways */

            hdrsrchtc_ofs_tune_away_proc( );
              /* Starts the tune-away process and transitions into TC OFS state   */
          }
        }

        if ( init_ofs_tuneaway == FALSE )
        { 
          HDR_MSG_SRCH( MSG_LEGACY_LOW, 
                        "hdrsrchtc_process_tune_away" );
          hdrsrchtc_process_tune_away( ms_left );
        }/* if ( init_ofs_tuneaway == FALSE ) */
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, 
        "Pending on TCC ACK/connection close SUBSTATE cancelling pri chain tuneaway" );
#ifdef FEATURE_HDR_QTA
        hdrsrchtc_set_qta_end( );
#endif /* FEATURE_HDR_QTA */
       }/* if ( hdrsrchtc.substate != HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE ) */

    } /* if ( HDRSRCHRF_UNLOCK_CANCELLED && HDRSRCHTC_D2LR_DRC_RAMPDOWN )*/

  }/* else if ( hdrsrchtc.tune_away.urgent_rel_only == FALSE ) */

} /* hdrsrchtc_rf_unlock_event( void ) */


/*===========================================================================

FUNCTION HDRSRCHTC_INIT_FOR_TRAFFIC_START

DESCRIPTION
 Connected State Initialization when starting traffic.

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtc_init_for_traffic_start( void )
{
  timetick_type                  sclks_now;
    /* current sclks count */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc.is_first_tca = TRUE;
    /* This is the first TCA for traffic */

  /* Log the ARQ and DRC buffers as soon as idle/access is exited */

  /* Synchronize with the FW on ASP updates by making sure any pending ASP 
     responses arrive. */
  hdrsrch_timed_wait_usecs( HDR_MAX_WAIT_ASP_UPDATE_RESP_USEC );

  hdrsrchlog_log_rab_buffer_record();
    /* Reverse Activity Bit Buffer Record sampled in every slot */

  hdrsrchtc.pilot_rpt         = NULL;
    /* No function specified yet to report pilot events to */

  hdrsrchtc.pending_pilot_rpt = FALSE;
    /* Clear the pending report condition */

  hdrsrchtc.reselect.threshold_eng =
          hdrsrchset_ecio_to_eng( HDRSRCHTC_RESELECT_THRESHOLD_DB * 2 );
    /* Convert DB to energy units */

  hdrsrchtc.reselect.weak_coverage_ms = 0;
    /* Initialize time in weak coverage to 0 */

  hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_INIT;
    /* init state for tune away */

  hdrsrchtc.tune_away.is_voip_tuneaway = FALSE;
    /* initialize to not an VOIP tuneaway */
        
  sclks_now = timetick_get();
  
  hdrsrchtc.tune_away.ofs_time_sclks = sclks_now;
    /* initialize last OFS time to now */

  hdrsrchtc.tune_away.ofs_group_time_sclks = sclks_now;
    /* initialize last OFS search group time to now */

  hdrsrchtc.tune_away.ofs_check_time_sclks = sclks_now;
    /* initialize last OFS check time to now */
    
  hdrsrchofs_reset_repeat_count();
    /* Reset OFS and RUP repeat counts */
    
  srch_hdr_set_hdr_in_traffic( TRUE );
    /* Tell 1x search that HDR is in traffic */

#ifdef FEATURE_HDR_QTA
  hdrsrchtc.tune_away.is_qta = FALSE;
#endif /* FEATURE_HDR_QTA */

} /* hdrsrchtc_init_for_traffic_start */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_FROM_IDLE_INIT

DESCRIPTION
 Connected State Initialization when entering from idle state

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_from_idle_init( void )
{

  /* Log the ARQ and DRC buffers as soon as idle/access is exited */

  hdrsrchlog_log_rab_buffer_record();
    /* Reverse Activity Bit Buffer Record sampled in every slot */

  hdrsrchtc_init_for_traffic_start();
    /* Init for traffic */

  hdrsrchtc_pilot_report_reset_cmd( );
    /* And quiet any pending reports from the IDLE state */

  hdrsrchtc_init_drc();
    /* Init DRC channel for Traffic state */

#ifdef FEATURE_HDR_DUAL_SIM
  if ( hdrmultirat_is_dsds_enabled() )
  {
    trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_PRIORITY_INV_ENABLED, TRUE );

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DSDS: Enabled BG traffic" );
  }
#ifdef FEATURE_HDR_QTA
  if ( hdrmultirat_is_dsds_enabled() 
#ifdef FEATURE_HDR_DR_DSDS
         || hdrmultirat_is_dr_dsds_enabled()
#endif /* FEATURE_HDR_DR_DSDS */
     )
  {
    if( hdrsrchtc_is_qta_nv_enabled() == TRUE )
    {
      trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_QUICK_TA_SUPPORTED, TRUE );
        /* In BG traffic, keep G awake for QTA */
    }
  }
#endif /* FEATURE_HDR_QTA */
    /* To let GSM perform neighborcell measurement after demod page */

#endif /* FEATURE_HDR_DUAL_SIM */

} /* hdrsrchtc_from_idle_init */

/*===========================================================================

FUNCTION HDRSRCHTC_FROM_TIMESYNC_INIT

DESCRIPTION
 Connected State Initialization when entering from timesync state

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_from_timesync_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc_init_for_traffic_start();
    /* Init for traffic */

  hdrsrchtc.is_slam_needed = TRUE;
    /* Need to slam at the first dump */

  hdrsrchtc.failed_srch_cnt = 0;
    /* Defayult */

} /* hdrsrchtc_from_timesync_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_RESUME_TRAFFIC_INIT

DESCRIPTION

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_resume_traffic_init( void )
{
  boolean tune_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1

#if ( HDRSRCHTC_USE_FAST_AGC_IN_RESUME_TUNE == TRUE )

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_FAST_AGC_ACQ_MODE,
                                hdrsrchutil_get_agc_rtcoffset(),
                                0 );
    /* Set AGC acquisition params for fast mode. */
#else

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
    /* Set AGC acquisition params for normal mode. */
#endif

#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */

  /* Enable RF and Tune to the channel */
#ifdef FEATURE_HDR_REVB

  HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS );
    /* KW fix : Limit aset_channel_cnt to max */

  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          hdrsrch.aset_channel,
                          hdrsrch.aset_demod,
                          hdrsrch.aset_channel_cnt,
                          hdrsrchtc_tune_back,
                          HDRSRCHTC_BAND_GRANT_TIMEOUT_MS );
#else
  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          &hdrsrch.aset_channel,
                          hdrsrchtc_tune_back,
                          HDRSRCHTC_BAND_GRANT_TIMEOUT_MS );
#endif /* FEATURe_HDR_REVB */

  if ( tune_status == FALSE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDING );
    /* We are starting Ramp */

  hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_RETURNING;
    /* note that we are returning from suspended traffic */

  hdrsrchtc.tune_away.is_voip_tuneaway = FALSE;
    /* initialize to not an VOIP tuneaway */
  
} /* hdrsrchtc_resume_traffic_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_INIT_DRC_FILTER

DESCRIPTION
 This function initializes the DRC filter operation upon entering traffic 
 state.

DEPENDENCIES
 None

PARAMETERS
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_init_drc_filter( void )
{
#ifdef FEATURE_HDR_REVB
  uint8    i;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */
 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
 
  hdrsrchtc.first_tc_dump = TRUE;
    /* We just entered traffic state. The next search dump is the first */
 
  hdrsrchmsg_init_drc_filt_reset_msg();
    /* Init DRC filter reset message */

#ifndef FEATURE_HDR_REVB
  HDRDRC_SET_INIT_VALUE( HDRMDSP_DEMOD_CARRIER_0,
                         HDRSRCHTC_RESELECT_DRC_THRESHOLD );
    /* DRC filter output will be set to the passed value upon filter reset */
 
  HDRDRC_RESET_FILTER( HDRMDSP_DEMOD_CARRIER_0 );
    /* The DRC filter will be reset whenever HDRSRCH enters traffic */
 
#else
  for( i = 0; i < HDRSRCH_MAX_DEMOD_CARRIERS_CNT; i++ )
  {
 
    hdrsrchmsg_set_drc_filt_reset_value( i, HDRSRCHTC_RESELECT_DRC_THRESHOLD );
  
  }  /* for */

  hdrsrchmsg_send_drc_filt_reset_msg();
    /* Send DRC filter reset message */

#endif  /* FEATURE_HDR_REVB */
 
} /* hdrsrchtc_init_drc_filter */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_GET_FILT_VALUE

DESCRIPTION
 This function returns the DRC filter output value.

DEPENDENCIES
 None

PARAMETERS
 None

RETURN VALUE
  DRC filter output value

SIDE EFFECTS
  None

===========================================================================*/

LOCAL uint16 hdrsrchtc_get_filt_value( void )
{
  uint16    filt_value = 0;
    /* DRC filter output value  */

#ifdef FEATURE_HDR_REVB
  uint8     sa;
    /* Subaset index */

  uint16    sa_filt_value;
    /* DRC filter output value for a subaset */
#endif  /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Since it takes up to 1 slot for DSP to detect the reset command, the
     DRC filter output value we use after a reset will depend on whether
     the reset has completed                                            */

#ifdef FEATURE_HDR_REVB
  if( hdrsrchmsg_rsp_is_rcvd( HDRSRCH_DRCFILT_RESET_RSP ) )
  {
    for ( sa = 0; sa < hdrsrchset.subaset_cnt; sa++ )
    {
      sa_filt_value = HDRDRC_GET_FILTER_OUTPUT_MC( 
                                        hdrsrchset.subaset[sa].demod_idx );
        /* DRC filter has been reset before we read the output */
    
    if ( sa_filt_value > filt_value ) 
    {
      filt_value = sa_filt_value;
        /* Save the biger value */
    }
    }  /* for sa */
    }
    else
    {
      sa_filt_value = HDRSRCHTC_RESELECT_DRC_THRESHOLD;
        /* Reset is pending when we want to use the DRC filter output */

      filt_value = sa_filt_value;
      /* Save the value to return */
    }

#else
  if( HDRDRC_RESET_IS_COMPLETE( HDRMDSP_DEMOD_CARRIER_0 ) )
  {
    filt_value = HDRDRC_GET_FILTER_OUTPUT_MC( HDRMDSP_DEMOD_CARRIER_0 );
      /* DRC filter has been reset before we read the output */
  }
  else
  {
    filt_value = HDRSRCHTC_RESELECT_DRC_THRESHOLD;
      /* Reset is pending when we want to use the DRC filter output */
  }
#endif  /* FEATURE_HDR_REVB */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "DRC Metric: %x Threshold: %x",
                  filt_value, 
                  HDRSRCHTC_RESELECT_DRC_THRESHOLD );

  return filt_value;

} /* hdrsrchtc_get_filt_value */

/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHTC_ENABLE_OFS

DESCRIPTION  : This function enables OFS in traffic state

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchtc_enable_ofs ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtc.ofs_disabled = FALSE;
    /* Enables TC_OFS */

} /* hdrsrchtc_enable_ofs */

/*=========================================================================

FUNCTION     : HDRSRCHTC_DISABLE_OFS

DESCRIPTION  : This function is called if any application ( so far FTAP or
               RTAP ) wants to disable TC OFS. TC OFS will be re-enabled
               by default whenever HDRSRCH enters traffic state. 

DEPENDENCIES : None

PARAMETERS   : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchtc_disable_ofs ( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtc.ofs_disabled = TRUE;
    /* Disable TC_OFS */

} /* hdrsrchtc_disable_ofs */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_INIT

DESCRIPTION
 Connected State Initialization

DEPENDENCIES
  Synchronized on an HDR System

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_init( hdrsrch_state_enum_type last_state )
{

  static boolean   tune_away_timer_defined = FALSE;
    /* define tune away rex timer only one time during powerup */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  dword            tune_away_ms;
    /* time in msec for which AT had tuned away */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

  boolean tune_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_issue_cpu_request( HDRSRCHUTIL_NPA_CPU_TRAFFIC );
    /* Request CPU for HDR traffic. */
#endif

  hdrsrchutil_mcdo_check();
    /* Change MDSP clock speed, if necessary. */

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup*/

#endif /* FEATURE_NPA */

  hdrsrch_default_init( last_state );
    /* Use default handling */

  hdrsrchtc_init_drc_filter( );
    /* Initializes DRC filter operation */

  hdrsrchtc.time_accum = 0;
    /* We just entered TC state, so clear the value in "time_accum"   */

  hdrsrchtc.waiting_for_close_loop = FALSE;
    /* Not waiting for TCXOMgr to give permission to close the TCXO loop. */

  hdrsrchtc.tune_away.urgent_rel_only = FALSE;
    /* Default to tune away for any reason. */

  hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_TRAFFIC );
    /* Set search priority */

  hdrsrchtc.is_slam_needed = FALSE;
    /* No slam by default */

  hdrsrchtc.failed_srch_cnt = 0;
    /* Default */

  hdrsrchtc.is_first_tca = FALSE;
    /* Not the first TCA by default. */

#ifdef FEATURE_HDR_DLNA
  hdrsrchtc.dlna_state = HDRSRCHTC_DLNA_INACTIVE_STATE;
    /* Default */
#endif /* FEATURE_HDR_DLNA */

  switch ( last_state )
  {
    case HDRSRCH_IDLE_STATE:
#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_BC_IDLE_STATE:
#endif /* FEATURE_HDR_BCMCS */

      if ( hdrsrch_is_lre_enabled() )
      {
        /* On entry to traffic from idle state enable tune aways 
           and update the traffic sub-state variable*/

        hdrsrchtc_enable_tune_aways();
          /* allow for unlock callbacks to be called */

        hdrsrchtc.substate = HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE;
          /* entered the traffic from Idle and hence wait for RT-Ack */

      (void) rex_clr_timer( &hdrsrchtc.tune_away.timer );
        /* clear the tune away timer */
      }

      hdrsrchtc_enable_ofs( );
        /* Enables OFS when entering traffic  */

      hdrsrchtc_from_idle_init();
        /* initialization for idle->TC state */

      hdrsrchafc_set_tap( FALSE );
        /* By default, no TAP is running */

#ifdef FEATURE_HDR_BAND_AVOIDANCE
      if( 
#ifdef FEATURE_HDR_DUAL_SIM
           hdrmultirat_is_dsda_enabled() &&
#endif
           hdrmultirat_is_band_avoidance_enabled() )
      {
        hdrmultirat_init_ba_data();

        timer_set( &hdrsrch.band_avoid_timer, HDRMULTIRAT_COEX_PWR_TIMER_MS,
                   0, T_MSEC );
      }
#endif /* FEATURE_HDR_BAND_AVOIDANCE */

#ifdef FEATURE_HDR_DLNA
#if defined ( FEATURE_HDR_IS890 ) || defined ( FEATURE_HDR_IS890A )
        /* if TAP is Activated delay D-LNA Enable*/
      if (hdris890a_is_activated())
      {
        /* It was found that some test boxes are sending TAP param assignment
           message over AsyncCC and AT is unable to receive them in traffic if
           D-LNA is enabled. It is also found from different test boxes that
           RE/FETAP Param Assignment message is sent within 500 ms of AT moving
           to traffic state.
           Hence to facilitate reception of AsyncCC TAP messages, decision to
           enable/disable D-LNA is delayed by 500 ms after entering traffic. */
       
        (void) rex_set_timer( HDRSRCHTC_DELAY_DLNA_ENABLE_TIMER, 
                               HDRSRCHTC_DELAY_DLNA_ENABLE_DURATION );
           /* Enable D-LNA upon expiry of this timer */

        hdrsrch_add_to_wait_mask( HDRSRCHTC_DELAY_DLNA_ENABLE_SIG );
           /* Watch for Delayed DLNA timeout signal*/
      }
      else
#endif /* (FEATURE_HDR_IS890 || FEATURE_HDR_IS890A) */
      {
      /* AT is transitioning to traffic state from idle state. DLNA as well
         as early preamble detection should be enabled in FW when these
         transitions are detected if the feature has been enabled. */
         hdrsrchtc_dlna_enable_proc();
      }
#endif /* FEATURE_HDR_DLNA */

      hdrsrchutil_clear_handoff_override();
        /* Clear pending handoff override */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
      hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_START;
      hdrsrchtc.d2lr_start_meas_time = timetick_get();
        /* Set time interval between measurement, starting now */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
      break;

    case HDRSRCH_SUSPENDED_TC_STATE:
    case HDRSRCH_OFS_TC_STATE:

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
      if (hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_WAIT_MEAS)
      {
        tune_away_ms = hdrsrchutil_read_stop_watch();
        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR: tune_away_ms %d",
            tune_away_ms);
        if ( tune_away_ms < HDRSRCHTC_MAX_TA_TIME_FOR_D2L_MEAS )
        {
          if ( hdrsrchtclte_request_trm_grant(
                 HDRSRCHTC_MAX_TA_TIME_FOR_D2L_MEAS -
                 tune_away_ms, 0 ) )
            /* HDR can make LTE measurements in 
               HDRSRCHTC_MAX_TA_TIME_FOR_D2L_MEAS - time consumed by prior TA */
        {
#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1
#if ( HDRSRCHTC_USE_FAST_AGC_IN_RESUME_TUNE == TRUE )
          hdrsrchrf_set_agc_acq_params( HDRSRCHRF_FAST_AGC_ACQ_MODE,
                                        hdrsrchutil_get_agc_rtcoffset(),
                                        0 );
            /* Set AGC acquisition params for fast mode. */
#else

          hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
            /* Set AGC acquisition params for normal mode. */
#endif

#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */

            /* Enable RF and Tune to the channel */
#ifdef FEATURE_HDR_REVB
          HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS );
            /* KW fix : Limit aset_channel_cnt to max */

#ifdef FEATURE_HDR_QTA
          if ( hdrsrchtc_is_qta_enabled() )
          {
            /* If we are starting LTE meas disable TX/Div */
            HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR QTA: Disable TX due to LTE meas");
            hdrsrchtc_disable_tx_qta();
              hdrsrchafc_stop_fll_tracking();
                /* Disable FLL tracking and save the FLL accum value. */
          }
#endif /* FEATURE_HDR_QTA */


          tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                                                hdrsrch.aset_channel,
                                                hdrsrch.aset_demod,
                                                hdrsrch.aset_channel_cnt,
                                                hdrsrchtc_tclte_tune_done,
                                                0 );
#else
          tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                                                &hdrsrch.aset_channel,
                                                hdrsrchtc_tclte_tune_done,
                                                0 );
#endif /* FEATURe_HDR_REVB */

          if ( tune_status == FALSE )
          {
            hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

            return;
          }

          break; /* No need to continue */
        }
        else
        {
          hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;
          HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR D2LR: Unable to meas LTE %d",
                          hdrsrchtc.d2lr_state);
        }
      }
      else
      {
            hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;
            HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "HDR D2LR: Too large TA value %d",
                            hdrsrchtc.d2lr_state,
                            tune_away_ms );
        }
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR D2LR: D2LR state %d",
                        hdrsrchtc.d2lr_state );
      }
      /* Fall through to next case */

    case HDRSRCH_LTE_MEAS_TC_STATE:
      /* Fall through to this case, which bypasses D2L measurements as
       * we  just came  from D2L  meas state,  but we  want to  resume
       * traffic just like other  suspend state. TBD******* Should LTE
       * meas state returns  to TC suspend state first?  Right now, it
       * goes straight to TC state, which may or may not work
       */

      if (hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_MEASURE)
      {
        hdrsrchtc.d2lr_start_meas_time = timetick_get();
        /* Set time interval between measurement, starting now */

        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR: d2lr_state %d",
                        hdrsrchtc.d2lr_state);
        hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;
        HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR: d2lr_state %d time %d",
                        hdrsrchtc.d2lr_state,
                        hdrsrchtc.d2lr_start_meas_time );
      }
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#ifdef FEATURE_HDR_QTA
      if ( !hdrsrchtc.tune_away.is_qta )
#endif /* FEATURE_HDR_QTA */
      {
         /* IF LTA or QTA and CSR channel is different then request trk */
        hdrsrchtc.waiting_for_close_loop = TRUE;
          /* Now we need permission from TCXO Manager before we
             close the TCXO loop. */

        hdrsrchafc_request_continue_trk();
          /* Now we need to close the TCXO loop so we can resume tc. */

        hdrdec_reset_pkt_sequences();
          /* Reset packet sequence numbers */

        hdrdec_activate();
          /* activate HDR decoder before going to 1x */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION

        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR: waiting_for_close_loop %d",
                        hdrsrchtc.d2lr_state);
#endif 
      }

      hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                           sizeof( hdrfw_tuneaway_stop_msg_t ) );
        /* get back to normal mode of DRC i.e. stop DRC ramp down mode */

      if( last_state == HDRSRCH_OFS_TC_STATE )
      {
#ifdef FEATURE_HDR_DUAL_SIM
        if ( hdrmultirat_is_dsds_enabled() )
        {
          trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_PRIORITY_INV_ENABLED, TRUE );

          HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DSDS: Enabled BG traffic" );
          /* To let GSM perform neighborcell measurement after demod page */

        }
#ifdef FEATURE_HDR_QTA
        if ( hdrmultirat_is_dsds_enabled() 
#ifdef FEATURE_HDR_DR_DSDS
         || hdrmultirat_is_dr_dsds_enabled()
#endif /* FEATURE_HDR_DR_DSDS */
          )
        {
          if( hdrsrchtc_is_qta_nv_enabled() == TRUE )
          {
            trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_QUICK_TA_SUPPORTED, TRUE );
              /* In BG traffic, keep G awake for QTA */
          }
        }
#endif /* FEATURE_HDR_QTA */

#endif /* FEATURE_HDR_DUAL_SIM */
      }

#ifdef FEATURE_HDR_DLNA
      /* AT is resuming traffic from suspend/ofs state. DLNA as well
         as early preamble detection should be enabled in FW when these
         transitions are detected. */
      hdrsrchtc_dlna_enable_proc();
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_QTA
      if ( hdrsrchtc.tune_away.is_qta )
      {
        hdrsrchtc_tune_back( HDRSRCHRF_TUNE_SUCCESS );
      }
#endif /* FEATURE_HDR_QTA */


      break;

    case HDRSRCH_TIME_SYNC_STATE:
      if ( hdrsrch_is_lre_enabled() )
      {
        /* On entry to traffic from idle state enable tune aways 
           and update the traffic sub-state variable*/

        hdrsrchtc_enable_tune_aways();
          /* allow for unlock callbacks to be called */

        hdrsrchtc.substate = HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE;
          /* entered the traffic from Time sync and hence wait for RT-Ack */
      }

      hdrsrchtc_enable_ofs( );
        /* Enables OFS when entering traffic  */

      hdrsrchtc_from_timesync_init();
        /* Initialization for Timesync -> Traffic */

      hdrsrchtc.waiting_for_close_loop = TRUE;
        /* Now we need permission from TCXO Manager before we
           close the TCXO loop. */

      hdrsrchafc_request_fast_acq();
        /* Request AFC state machine to perform fast acquisition mode. */

      hdrsrchutil_clear_handoff_override();
        /* Clear pending handoff override */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    hdrmultirat_dsda_mode_change();
      /* If in DSDA tell FW */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */  

#ifdef FEATURE_HDR_DR_DSDS
    /* If we are in DSDS mode or DR-DSDS check with TRM if DR was enabled/disabled */
    if ( hdrmultirat_is_dsds_enabled() || hdrmultirat_is_dr_dsds_enabled() )
    {
      hdrsrchrf_update_dr_mode();
        /* Check with TRM if we are in DR mode */

    }
#endif /* FEATURE_HDR_DR_DSDS */

      break;

    default:
      ERR_FATAL("Entering TC from unexpected state %d", last_state, 0, 0);

  }/* switch */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  if (hdrsrchtc.d2lr_state != HDRSRCHTC_D2LR_MEASURE)
    /* Skip the following steps if we are going to do LTE measurement */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  {
    hdrdec_enter_mode( HDRDEC_TRAFFIC_MODE );
      /* set the decoder for traffic mode */
   
    if ( tune_away_timer_defined == FALSE  )
    {
      /* define special timer for traffic tune aways */
      rex_def_timer( &hdrsrchtc.tune_away.timer,
                     HDRSRCH_TCB,
                     HDRSRCH_PRI_CHAIN_EVENT_SIG );
   
      tune_away_timer_defined = TRUE;
        /* define the timer only once */
    }
   
    hdrsrchset_reset_tca_disjoint_flag();
      /* Clear the tca_disjoint flag */
   
   
    if ( hdrsrcheq_is_grice_enabled() ) 
    {
      hdrsrchset_grice_activate_asp_snr_order_updates( TRUE );
        /* Enable timer to update ASP SNR Order list regardless of gRICE CSET
           promotion. */
    }
   
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    hdrsrchutil_report_coex_power( (timer_cb_data_type)NULL );
      /* Report coex power and start timer to keep reporting it */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
  }

} /* hdrsrchtc_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_ADVISE_UNLOCK_RF_CMD

DESCRIPTION
  This function commands the searcher to unlock the RF. Before RF lock is 
  released, tune-away procedures are invoked and HDR searcher state changes 
  from traffic state to traffic-suspended state. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_advise_unlock_rf_cmd( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ADVISE_UNLOCK_RF_CMD in HDR traffic" );

  #ifdef FEATURE_HDR_QTA
  if (hdrsrchtc.tune_away.is_qta == TRUE )
  {
    hdrsrchtc_set_qta_end( );

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR QTA: QTA flag was set and clearing it" );
    /* clear QTA flag if QTA is not really started */
  }
  #endif /* FEATURE_HDR_QTA */

  hdrsrch_kill_timer( HDRSRCHTC_OFS_TA_TIMER );
    /* Stop the tune-away timer and clear the pending signal  */

  (void) rex_clr_timer( &hdrsrchtc.tune_away.timer );
    /* clear the tune away timer if it is active */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Clear the requested signals ... */

  hdrsrchtc_pre_tune_away_proc();
    /* perform pre-tune away processing */

  hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_GONE;

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop scheduler ... we are tuning away */

  hdrsrchdrv_abort_search( FALSE );
    /* abort pending searches if any */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrch_tune_away_on_searcher_idle( hdrsrchtc_on_search_idle_cb );
    /* change to suspended traffic state */

  return E_SUCCESS;

} /* hdrsrchtc_advise_unlock_rf_cmd   */

#ifdef  FEATURE_HDR_BCMCS

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_HANDLE_IDLE_MODE_SWITCH

DESCRIPTION
  This function commands the searcher to transition to IDLE from TC state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Command status.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_handle_idle_mode_switch( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_aset_enter_idle( );
    /* Deallocated the unnecessary ASET pilots, set DRC covers to NULL, etc */

  /* Check the global bc_status flag here */
  if ( hdrsrchbc_get_bc_status( ) == BCMCS_DISABLED )
  {
#ifdef FEATURE_HDR_REVB
    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                "TC to Idle with pn=%d; demod=%d; chan=%d; band=%d", 
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx,
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel.chan_num,
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel.band );
#else
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "TC to Idle with pn=%d", 
                    hdrsrchset.aset[0]->pn_offset );
#endif /* FEATURE_HDR_REVB */

    hdrsrchmon_init_idle_state( );
      /* Transitition to Idle */

  }
  else 
  {
    hdrsrchbc_init_bc_state( );
      /* Transitions to Idle broadcast state                      */
  }

  return E_AGAIN;

} /* hdrsrchtc_handle_idle_mode_switch */

#endif /* FEATURE_HDR_BCMCS */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_ABORT_TUNE_AWAY

DESCRIPTION
  This function aborts the tune away in traffic state. It changes the tune
  away state back to "no conflict". It also cleans the callback function to
  be called when searcher becomes idle.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_abort_tune_away( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_clear_on_searcher_idle_proc( );
    /* If the "on_searcher_idle" callback is not NULL, we need to clear it
       because the tune-away in progress is about to be cancelled         */

  hdrsrchtc.tune_away.state = HDRSRCHTC_TUNE_AWAY_NO_CONFLICT;
    /* Reset the state since we are cancelling the tune-away              */

} /* hdrsrchtc_abort_tune_away  */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_IDLE_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to IDLE

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_idle_mode_cmd( void )
{
  errno_enum_type               status = E_AGAIN;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IDLE_MODE_CMD" );

  #ifdef FEATURE_HDR_QTA
  if (hdrsrchtc.tune_away.is_qta == TRUE )
  {
    hdrsrchtc_set_qta_end( );

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR QTA: QTA flag was set and clearing it" );
    /* clear QTA flag if QTA is not really started */
  }
  #endif /* FEATURE_HDR_QTA */

  /* The following order is important:
    1. Empty directed list 
    2. Reset nset search parameters to default
    3. Update with broadcast */
    
  hdrsrchset_init_nset_update_list( HDRSRCH_NSET_UPDATE_DIRECTED );
    /* Empty the directed nset update list */

  hdrsrchset_reset_nset_search_window_params();
    /* Set search window parameters to default */

  hdrsrchset_update_nset_search_window_params( HDRSRCH_NSET_UPDATE_BROADCAST );
    /* Update with broadcast parameters */

  hdrsrchdrv_abort_search( FALSE );
    /* Abort any active search before entering idle state */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  ( void ) hdrsrchtclte_send_deinit_req();
    /* If DO has sent an Init request to LTE to perform LTE pilot measurements,
       make sure DO also sends a Deinit request.  Otherwise, LTE will be left
       in an unexpected state when it is activated. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#ifdef  FEATURE_HDR_BCMCS
  status = hdrsrchtc_handle_idle_mode_switch( );
    /* handle the idle mode switch command from TC */
#else /*  FEATURE_HDR_BCMCS */

  hdrsrchutil_aset_enter_idle( );
    /* Deallocate n-1 ASET pilots, set DRC covers to NULL, etc. */

  /* TBD need to find out the demod carrier/subaset used in idle */
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "TC to Idle with pn=%d", 
                    hdrsrchset.subaset[0].aset[0]->pn_offset );

  hdrsrchmon_init_idle_state( );
    /* Transitition to Idle */

#endif /*  FEATURE_HDR_BCMCS */

  return status;
    /* Retry the HDRSRCH_IDLE_CMD once we're back in idle state */

} /* hdrsrchtc_idle_mode_cmd( ) */


/* EJECT */
/*=========================================================================

FUNCTION     : HDRSRCHTC_LOG_RAB_INFO_RECORD

DESCRIPTION  : Log the reverse activity bit information received in the
               Traffic channel assignment message from the AN

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/
void hdrsrchtc_log_rab_info_record
(
  const hdrsrch_tc_assignment_params_type *params
    /* Traffic Channel Assignment parameters */
)
{
  LOG_HDR_RAB_INFO_C_type   *log_ptr;
  /* log pointer to the reverse activity bit information record type */

  int8   p;        /* loop variable */

#ifdef FEATURE_HDR_REVB
  uint8  i, num_pilots;
    /* Loop index and number of TCA pilots */
#endif /* FEATURE_HDR_REVB */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB
  num_pilots = 0;

  for( i=0; i<params->num_subaset; i++ )
  {
    num_pilots += params->subaset_list[i].num_aset_pilots;
  }

  log_ptr   = (LOG_HDR_RAB_INFO_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_RAB_INFO_C,
                            HDRLOG_VAR_SIZEOF ( LOG_HDR_RAB_INFO_C_type,
                                                log_hdr_rab_info_type,
                                                num_pilots ) );  
  if ( log_ptr )
  {

    log_ptr->pilot_cnt = num_pilots;
    /* number of active set pilots received in the TCA message */

    for( i=0; i<params->num_subaset; i++ )
    {
      /* update the fields in the log record */

      for ( p = 0; p < params->subaset_list[i].num_aset_pilots; p++ )
      {
        log_ptr->rab_info[p].rab_len = 
                     params->subaset_list[i].aset_list[p].rab_length;
        log_ptr->rab_info[p].rab_offset  = 
                     params->subaset_list[i].aset_list[p].rab_offset;
        log_ptr->rab_info[p].pn_offset = 
                     params->subaset_list[i].aset_list[p].pilot_pn;
      }

    }
    log_commit( (log_type *) log_ptr);
    /* submit the log to DIAG */
  }

#else
  log_ptr   = (LOG_HDR_RAB_INFO_C_type *)
              log_alloc_ex( (log_code_type) LOG_HDR_RAB_INFO_C,
                            HDRLOG_VAR_SIZEOF ( LOG_HDR_RAB_INFO_C_type,
                                                log_hdr_rab_info_type,
                                                params->num_pilots ) );
  if ( log_ptr )
  {

    log_ptr->pilot_cnt = params->num_pilots;
    /* number of active set pilots received in the TCA message */

    /* update the fields in the log record */
    for ( p = 0; p < params->num_pilots; p++ )
    {
      log_ptr->rab_info[p].rab_len     = params->aset_pilots[p].rab_length;
      log_ptr->rab_info[p].rab_offset  = params->aset_pilots[p].rab_offset;
      log_ptr->rab_info[p].pn_offset   = params->aset_pilots[p].pilot_pn;
    }

    log_commit( (log_type *) log_ptr);
    /* submit the log to DIAG */

  }
#endif /* FEATURE_HDR_REVB */
}/* hdrsrchtc_log_rab_info_record */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_TCA_CMD

DESCRIPTION
  This function handles a traffic channel assignment msg in traffic mode.

DEPENDENCIES
  None

PARAMETERS
  params  - Traffic Channel Assignment parameters

RETURN VALUE
  FALSE   - Command processing is to be suspended until TCA is complete.

SIDE EFFECTS
  Waits for any search in progress to complete,
  Tunes to the new channel,
  Sets the new Active Pilot Set.
  Starts a search on the new channel,
  Assigns fingers to ASET multipath,
  Calls "traffic channel assignment complete" callback.
  Command processing is halted until TCA is complete.

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_tca_cmd
(
  const hdrsrch_tc_assignment_params_type *params
    /* Traffic Channel Assignment parameters */
)
{

  int                        i;
    /* loop variable */

#ifdef FEATURE_HDR_REVB
  int8                       j;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_trm_freq_info_type  tune_info;
    /* Band/chan info for TRM reservation. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  if ( params->num_subaset == 0 )
  {
     HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "TCA subaset count[%d]. No Action",
                     params->num_subaset );
     return E_SUCCESS;
  }
#else 
  if ( params->num_pilots == 0 )
  {
     HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "TCA pilot count[%d]. No Action",
                     params->num_pilots );
     return E_SUCCESS;
  }
#endif /* FEATURE_HDR_REVB*/  

  /* Special attention is needed when HDRSRCH has started tune-away process,
     either for Off-Frequency-Search or for 1x page monitoring. 
     
     1. If there is no tune-away in progress, TCAM will proceed as before.
     2. Otherwise if the TCAM causes channel change, it will be delayed.
     3. TCAM will also be delayed if the receipt of it is too close to
        RF tuning.
        
     The reason for 3 above is not to delay ASET update too much when no
     RF tuning will be involved.                                          */

  if( hdrsrchtc.tune_away.state >= HDRSRCHTC_TUNE_AWAY_DRC_RAMP )
  {
    /* HDRSRCH has already started tune-away process    */

#ifdef FEATURE_HDR_REVB
    if ( hdrsrchtc_check_if_tune_needed( params ) )
      /* In revB, when TCA command is received, RF tunning is needed always. */
#else
    if ( !SYS_CHAN_CMP_EQUAL( params->channel, hdrsrch.aset_channel ) )
#endif /* FEATURE_HDR_REVB */
    {
      /* TCA will be re-tried when tune-away is finished      */
      return E_AGAIN;
    }
    else if ( hdrsrchtc.tune_away.state >= HDRSRCHTC_TUNE_AWAY_GONE )
    {
      /* TCA is received just before tuning-away, try it later as well  */
      return E_AGAIN;
    }
  }

  /* Disable TA when processing TCA*/
  HDR_MSG_SRCH( MSG_LEGACY_LOW, "Disabling TA due to processing TCA");
  hdrsrchtc_disable_tune_aways();

  hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, 
                             hdrsrchtc_get_tc_reason() );
    /* Alter priority to indicate we are in the desired reason. */

  if ( hdrsrchtc.is_first_tca == TRUE )
  {
#ifdef FEATURE_HDR_QTA
    hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_START_STATE );
      /* Inform TRM for APS state start */
#endif /* FEATURE_HDR_QTA */
  }

  hdrsrchtc.tune_away.pref = params->tc_tune_away;
    /* store the tune away selection as perferred by the protocols */

  hdrsrch.suspend_cb       = params->suspend_cb;
    /* Remember the Suspend (RF Gain/Loss) callback */

#ifdef FEATURE_HDR_REVB
  hdrsrch.schdgrp_rpt_cb = params->schdgrp_rpt_cb;
    /* Remember the scheduler group report callback */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "TC_ASSIGN_CMD Num=%d Tune Away=%d",
                  params->num_subaset,
                  (params->tc_tune_away == HDRSRCH_TUNE_AWAY_IS2000) );
#else
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "TC_ASSIGN_CMD Num=%d Tune Away=%d",
                  params->num_pilots,
                  (params->tc_tune_away == HDRSRCH_TUNE_AWAY_IS2000) );
#endif /* FEATURE_HDR_REVB */

  memset(&tune_info, 0, sizeof(tune_info));
    /* Clear freq info struct before populating it. */

  /* Print TCA pilots list on F3 screen. This is requested by the test teams
     for ease of field testing */

#ifdef FEATURE_HDR_REVB
  tune_info.freq_info.num_channels = params->num_subaset;
  tune_info.freq_info.band = params->subaset_list[0].channel.band;
    /* Populate band/chan information. */

  for ( j = 0; (j < params->num_subaset) && (j < HDRSRCH_MAX_NUM_SUBASETS); j++ )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "===  TCA subaset[%d]  ===", j);

    tune_info.channel_list[j] = params->subaset_list[0].channel.chan_num;

    for ( i = 0; (i < params->subaset_list[j].num_aset_pilots) && (i < HDRSRCH_ASET_MAX_SIZE); i++ )
    {
       HDR_MSG_SRCH_5( MSG_LEGACY_MED, "TCA Pilots PN=%d Cov=%d Mac=%d, PGID=%d subaset[%d]",
			params->subaset_list[j].aset_list[i].pilot_pn,
			params->subaset_list[j].aset_list[i].drc_cover,
			params->subaset_list[j].aset_list[i].mac_index,
			params->subaset_list[j].aset_list[i].pilot_group_id,
			j);
    }/* for i */
  } /* for j */

  if ( hdrsrchset.cset_cnt != 0 )
  {
    for( i=0; i<hdrsrchset.cset_cnt; i++)
    {
      HDR_MSG_SRCH_6( MSG_LEGACY_HIGH, "gR:CSET Ch=%d/%d PN=%d PGID=%d E=%d in [st=%x]", 
                      hdrsrchset.cset[i]->chan.chan_num,
                      hdrsrchset.cset[i]->chan.band,
                      hdrsrchset.cset[i]->pn_offset,
                      hdrsrchset.cset[i]->pilot_gid,
                      hdrsrchset.cset[i]->total_eng,
                      hdrsrch_state_id );
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "gR:CSET is empty" );
  }
  
#else
  tune_info.freq_info.num_channels = 1;
  tune_info.freq_info.band = params->channel.band;
  tune_info.channel_list[0] = params->channel.chan_num;
    /* Populate band/chan information. */

  for ( i = 0; i < params->num_pilots; i++ )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "TCA Pilots PN=%d Cov=%d Mac=%d",
                    params->aset_pilots[i].pilot_pn,
                    params->aset_pilots[i].drc_cover,
                    params->aset_pilots[i].mac_index );
  }/* for */
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_set_trm_freq_info( &tune_info );
    /* Convey band/chan info update for TRM interaction. */

  /*---------------------------------------*
   * Log active set change related packets *
   *---------------------------------------*/

#ifndef FEATURE_HDR_REVB
  hdrsrchtc_log_rab_info_record( params );
    /* log rev activity bit info; need update TBD logging  */
#endif  /* FEATURE_HDR_REVB */

  hdrsrchtc.tca_params = params;
    /* Save the TCA command parameters */

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop scheduler ... we are tuning */

  hdrsrch_on_searcher_idle( hdrsrchtc_tca_tune );
    /* When the searcher is idle, start tune to assigned traffic channel */

  return E_IN_PROGRESS;
    /* Command is not yet complete */

} /* hdrsrchtc_tca_cmd */


/* EJECT */
  
/*===========================================================================

FUNCTION HDRSRCHTC_ASET_UPDATE_SEARCH

DESCRIPTION
  This function causes an immediate re-search using the updated ASET list.

DEPENDENCIES
  Called from "on_search_idle"

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes an immediate search - or an immediate re-search after the current
  search completes.

===========================================================================*/

LOCAL void hdrsrchtc_aset_update_search( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
    /* Force a new search immediately. */

  hdrsrch_complete_cmd( );
    /* Complete the pending command (ASET UPDATE) */

} /* hdrsrchtc_aset_update_search( ) */

/*===========================================================================

FUNCTION HDRSRCHTC_ASET_UPDATE_CMD

DESCRIPTION
  This function adjusts the neighbour set

DEPENDENCIES
  None

PARAMETERS
  params   - New active set list message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_aset_update_cmd
(
  const hdrsrch_aset_update_params_type *params
)
{
  boolean update;
    /* Active pilot set has been updated flag */

  int                        i;
    /* loop variable */

#ifdef FEATURE_HDR_REVB
  uint8                      j;
    /* loop variable */

  uint8                      c;
    /* demod carrier index */

#endif /* FEATURE_HDR_REVB */

  errno_enum_type status = E_SUCCESS;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchdrv_abort_search( FALSE );
    /* Abort search before processing ASET update cmd */

#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "ASET_UPDATE_CMD Num Of Subasets=%d",
                  params->num_subasets );

  /* Printing out the aset pilot list is a requirement from test teams */
  for ( j = 0; j < params->num_subasets; j++ )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "===  TCA subASET[%d]  ===", j );

    for ( i = 0; i < params->subaset_list[j].num_aset_pilots; i++ )
    {
	HDR_MSG_SRCH_4( MSG_LEGACY_MED, "TCA Pilots PN=%d Cov=%d Mac=%d subASET[%d]",
			params->subaset_list[j].aset_list[i].pilot_pn,
			params->subaset_list[j].aset_list[i].drc_cover,
			params->subaset_list[j].aset_list[i].mac_index,
			j);
    }/* for i */
  } /* for j */

  update = hdrsrchutil_update_aset_pilots_revb( params->subaset_list,
                                                params->num_subasets,
                                                params->cc_subactive_set_idx );
    /* Set the active set pilots for revB */

  for( i=0; i<hdrsrchset.subaset_cnt; i++ )
  {
    hdrsrchtc.num_cells_in_subaset[i] = 
                          hdrsrchutil_get_num_unique_cells_in_subaset( i );

    c = hdrsrchset.subaset[ i ].demod_idx;

    hdrsrchtc_drc_param_ctrl( c );
  }

  hdrsrch.frame_offset = params->frame_offset;
    /* Save frame offset */

  hdrsrchutil_mcdo_check();
    /* Change MDSP clock speed, if necessary. */

#else
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "ASET_UPDATE_CMD Num=%d",
                  params->num_pilots );

  /* Printing out the aset pilot list is a requirement from test teams */
  for ( i = 0; i < params->num_pilots; i++ )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "ASET Pilots PN=%d Cov=%d Mac=%d",
                    params->aset_pilots[i].pilot_pn,
                    params->aset_pilots[i].drc_cover,
                    params->aset_pilots[i].mac_index );
  }/* for */


  update = hdrsrchutil_update_aset_pilots( params->aset_pilots,
                                       params->num_pilots );
    /* Update the active set pilots */

  hdrsrchtc.num_cells_in_aset = hdrsrchutil_get_num_unique_cells_in_aset();
    /* Save the number of unique cells in ASET for later use */
#endif  /* FEATURE_HDR_REVB */

  if ( update )
  {
    /* If the ASET update actually changed the ASET, triage must
       be called to cause new pilot assignments.  Since Triage
       is done after searches, we must cause a new search. */
    status = E_IN_PROGRESS;

    hdrsrch_on_searcher_idle( hdrsrchtc_aset_update_search );
      /* Start new search immediately, or when searcher idle. */
  }

  /* ASET struct has been updated at this point, even before
     search becomes idle, so for our purpose ASET is updated */
#ifdef FEATURE_HDR_REVC
  if ( params->update_complete_cb )
  {
    params->update_complete_cb();
  }
#endif /* FEATURE_HDR_REVC */

  return status;
} /* hdrsrchtc_aset_update_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_NSET_UPDATE_CMD

DESCRIPTION
  This function adjusts the neighbour set

DEPENDENCIES
  None

PARAMETERS
  params   - New neighbour list message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_nset_update_cmd
(
  const hdrsrch_nset_update_params_type *params
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "NSET_UPDATE_CMD Num=%d",
                  params->num_pilots );

  HDR_ASSERT ( ( params->num_pilots <= HDRSRCH_NSET_MAX_SIZE ) 
    && ( params->num_channels <= HDRSRCH_HASH_CHAN_MAX_SIZE) 
    && ( params->num_sects <= HDRSRCH_MAX_CACHE_RECORDS ) );
    /* KW fix : Limit to max */

  (void) hdrsrchutil_update_nset_pilots( params->nset_pilots, 
                                  params->num_pilots,
                                  params->update_type, 
                                  params->channel_list,
                                  params->num_channels,
                                  params->avoided_channel_list,
                                  params->num_avoided_channels
#ifdef FEATURE_HDR_REVB
                                 ,params->serving_pilot_gid,
                                  params->hdr_subtype
#endif /* FEATURE_HDR_REVB */
                                  , NULL  );
    /* Update the neighbour set */
  
  hdrsrchutil_update_cached_sectors( params->cached_sects, 
                                     params->num_sects );
    /* Update cached sectors */

} /* hdrsrchtc_nset_update_cmd */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_TC_ACK_RECEIVED_CMD

DESCRIPTION
  This function lets traffic state know that tc ack has been received

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_tc_ack_received_cmd ( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* At this time we have either received acknowledgement for TC complete
     OR SLP has timed out waiting for acknowledegement) for TC complete msg */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Rxed TCC Ack. May start tuneAway" );

  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchtc.substate = HDRSRCHTC_TCC_ACK_RECEIVED_SUBSTATE;
      /* RT-Ack received and data transfer is in inprogress, enable RF unlock-by processing */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "substate HDRSRCHTC_TCC_ACK_RECEIVED_SUBSTATE" );
    hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, 
                               hdrsrchtc_get_tc_reason() );
    /* Alter priority to indicate we are in the desired reason. */
  }

  /* Generate a pilot report unconditionally to report the pilot transitions
     that happended after RUP was generated for connReq but before TCA was
     received. */

  if ( hdrsrchtc.pilot_rpt != NULL )
  {
    hdrsrchtc.pending_pilot_rpt = FALSE;
      /* Clear the pending report condition */

    hdrsrchtc.pilot_rpt();
      /* generate an extra pilot report after opening traffic connection */
  }

#ifdef FEATURE_HDR_DUAL_SIM
  if ( !hdrmultirat_is_dsds_enabled() )
  {
#endif /* FEATURE_HDR_DUAL_SIM */

  /* tune_away.pref is recevied from CP as part of TCA and apply it after 
      receveing TCC ACK*/
  if ( hdrsrchtc.tune_away.pref == HDRSRCH_TUNE_AWAY_IS2000 )
  {
    /* Start IS2000 paging channel monitor tune away */

    hdrsrchtc.tune_away.urgent_rel_only = FALSE;
      /* Willing to be interrupted for any purpose. */
  }
  else
  {
    hdrsrchtc.tune_away.urgent_rel_only = TRUE;
      /* Only tune away if something urgent interrupts us */
  }

#ifdef FEATURE_HDR_DUAL_SIM
  }
  else if ( hdrmultirat_is_dsds_enabled() && 
      ( hdrsrchtc.tune_away.pref == HDRSRCH_TUNE_AWAY_IS2000 ) )
  {
    /* Start IS2000 paging channel monitor tune away */

    hdrsrchtc.tune_away.urgent_rel_only = FALSE;
      /* Willing to be interrupted for any purpose. */
  }
  else
  {
    hdrsrchtc.tune_away.urgent_rel_only = TRUE;
      /* Only tune away if something urgent interrupts us */
  }
#endif /* FEATURE_HDR_DUAL_SIM */

  hdrsrchutil_tc_ofs_start_gov();
    /* start the rate governer for HDR TC OFS */

  hdrsrchtc_enable_tune_aways();
    /* Enable tune aways now that the call is up */
  
}/* hdrsrchtc_tc_ack_received_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_ENABLE_PILOT_REPORT

DESCRIPTION
  This function enables pilot reporting

DEPENDENCIES
  None

PARAMETERS
  params - PILOT_REPORT parameters

RETURN VALUE
  None

SIDE EFFECTS
  May immediately cause pilot_rpt_cb( ) to be called.

===========================================================================*/

LOCAL void hdrsrchtc_enable_pilot_report_cmd
(
  const hdrsrch_pilot_report_params_type *params
    /* Parameters for Idle Mode command */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "PILOT_REPORT_CMD enable=%d",
                  params->pilot_rpt_cb ? 1 : 0 );

  hdrsrchtc.pilot_rpt = params->pilot_rpt_cb;
    /* Save new pilot report callback function */

  /* Any pending reports? */

  if ( hdrsrchtc.pilot_rpt != NULL  &&  hdrsrchtc.pending_pilot_rpt )
  {
    /* If a report is pending, and we just installed a valid callback,
       generate a new report. */

    hdrsrchtc.pending_pilot_rpt = FALSE;
      /* Clear the pending report condition */

    hdrsrchtc.pilot_rpt( );
      /* Generate the report */
  }

} /* hdrsrchtc_enable_pilot_report_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_TC_MODE_CMD

DESCRIPTION
  This function handles an arriving traffic mode change command. This may 
  change the priority of the current traffic session, and turns on 
  simultaneous HDR TC and 1x paging compatibility if they aren't already 
  compatible.

DEPENDENCIES
  None

PARAMETERS                         
  tc_mode  - Contains the traffic mode to be invoked.

RETURN VALUE
  E_SUCCESS - Command processing is complete.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_tc_mode_cmd
(
  const hdrsrch_tc_mode_params_type *tc_mode
    /* Traffic Channel Assignment parameters */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc_set_tc_mode( tc_mode->mode );
    /* Set the traffic mode as requested. */

  return E_SUCCESS;
    /* Command was successfully processed */

} /* hdrsrchtc_tc_mode_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_DEFAULT_TRAFFIC_CMD

DESCRIPTION
  This function handles the default traffic mode commands. These commands
  are same in active traffic and suspended traffic modes.

DEPENDENCIES
  None

PARAMETERS
  cmd   - Command to be processed

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.  Execute hdrsrch_retry_cmd( ) at a
                    later point in time to reattempt the command.
                    [hdrsrch_retry_cmd( ) is called at every state change]

SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd( ).

===========================================================================*/

errno_enum_type hdrsrchtc_default_traffic_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type            status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( cmd->name )
  {
    case HDRSRCH_ASET_UPDATE_CMD:
      status = hdrsrchtc_aset_update_cmd( &cmd->params.aset_update );
      break;

    case HDRSRCH_NSET_UPDATE_CMD:
      hdrsrchtc_nset_update_cmd( &cmd->params.nset_update );
      break;

    case HDRSRCH_PILOT_REPORT_CMD:
      hdrsrchtc_enable_pilot_report_cmd( &cmd->params.pilot_report );
      break;

    case HDRSRCH_PILOT_REPORT_RESET_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "PILOT_REPORT_RESET_CMD" );
      hdrsrchtc_pilot_report_reset_cmd( );
      break;

    case HDRSRCH_TC_ACK_RECEIVED_CMD:
      hdrsrchtc_tc_ack_received_cmd();
      break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BC_INFO_CMD:
      hdrsrchbc_set_bc_status( cmd->params.bc_info.bcmcs_status );
        /* Set the BCMCS status                                   */
      break;
#endif /* FEATURE_HDR_BCMCS */

    case HDRSRCH_SYNCHRONIZE_CMD:
      status = hdrsrchutil_sync_check( &cmd->params.synchronize );
      break;

    case HDRSRCH_TC_MODE_CMD:
      status = hdrsrchtc_tc_mode_cmd( &cmd->params.tc_mode );
        /* Set the Traffic mode while in SUS or TC state. */
      break;

    case HDRSRCH_RUP_REQ_CMD:
      status = hdrsrchutil_rup_req_cmd( &cmd->params.rup_req );

      hdrsrchtc_rf_unlock_event();
        /* make sure that we enter TC OFS state or set up 
           timers to enter TC OFS state for RUP search */

      break;  

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
    case HDRSRCH_LTE_NSET_UPDATE_REDIR_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR:HDRSRCH_LTE_NSET_UPDATE_REDIR_CMD");
      status = hdrsrchtclte_process_lte_nset_cmd
                                  ( &cmd->params.lte_nset_update_redir );
      if( (status == E_SUCCESS) &&
          ( hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_START) )
      {
        hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_ENABLE_MEAS;

        hdrsrchtc.d2lr_start_meas_time = timetick_get();
          /* Set time interval between measurement, starting now */

        HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                        "HDR DLR: HDRSRCH_LTE_NSET_UPDATE_REDIR_CMD %d %d",
                        hdrsrchtc.d2lr_state,
                        hdrsrchtc.d2lr_start_meas_time );
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR DLR: Not enough time %d",
                        hdrsrchtc.d2lr_state);
      }
      /* Update LTE neighbor list - used in DO->LTE redirection */

      break;
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

#ifdef FEATURE_HDR_BAND_AVOIDANCE

     case HDRSRCH_BAND_AVOID_TIMER_CMD:
       status = hdrmultirat_save_ba_power();

#endif /* FEATURE_HDR_BAND_AVOIDANCE */

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchtc_default_traffic_cmd(cmd) */

#ifdef FEATURE_HDR_REVC
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_LOAD_INFO_CMD

DESCRIPTION
  This function sends LoadAdj update to FW from up to 3 carriers
    
DEPENDENCIES
  None

INPUTS
  loadadj_ptr - pointer to loadadj information

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
LOCAL errno_enum_type hdrsrchtc_update_loadadj_cmd
(
  const hdrsrch_loadadj_type* loadadj_ptr
)
{
  HDR_MSG_SRCH(MSG_LEGACY_MED, "update_loadadj_cmd");

  return hdrsrchutil_update_loadadj(loadadj_ptr);
}

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_ENABLE_NLB_CMD

DESCRIPTION
  This function enables/disables demod CC on all carriers. If disabling,
  this command also set all LoadAdj to default
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
LOCAL errno_enum_type hdrsrchtc_enable_nlb_cmd
(
  const boolean enable_nlb
)
{
  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "enable_nlb_cmd %d",
                 enable_nlb);

  hdrsrchutil_enable_nlb(enable_nlb);

  return E_SUCCESS;
}
#endif /* FEATURE_HDR_REVC */


/*==========================================================================

FUNCTION        HDRSRCHTC_AT_SET_DRC_CMD

DESCRIPTION
  This function set DRC parameter andsend message to FW to config DRC.
    
DEPENDENCIES
  None

INPUTS
  demod_idx
  drc_param

RETURN VALUE
  E_SUCCESS

SIDE EFFECTS
  None

==========================================================================*/

errno_enum_type hdrsrchtc_at_set_drc_cmd
(
  hdrsrch_demod_idx_type demod_idx,
    /* demod indxe */

  uint8                  drc_param
    /* parameter to be used for setting the DRC from AT command */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* TBD: handle revB AT commands. now only set DRC predictor at demod0 */
  hdrsrchtc_set_drc_param( demod_idx, drc_param );
    /* set the bit field which will set the DRC parameter */

#ifdef FEATURE_HDR_IS890
  hdrsrchtc_drc_param_ctrl( demod_idx );
    /* allow the new drc value to be activated during traffic state */
#endif

  return E_SUCCESS;
} /* hdrsrchtc_at_set_drc_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTC_CMD

DESCRIPTION
  This function ...

DEPENDENCIES
  None

PARAMETERS
  cmd   - Command to be processed

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.  Execute hdrsrch_retry_cmd( ) at a
                    later point in time to reattempt the command.
                    [hdrsrch_retry_cmd( ) is called at every state change]

SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd( ).

===========================================================================*/

LOCAL errno_enum_type hdrsrchtc_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
     /* Command completion status */

  hdrsrch_cmd_type                  *tmp_cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Only process the command if we aren't in the process of moving to
     the suspended state already. We may be waiting for the searcher
     to be idle. */

  if ( ( cmd->name != HDRSRCH_DEACTIVATE_CMD ) &&
       ( cmd->name != HDRSRCH_SYSTEM_LOST_CMD ) &&
       hdrsrch_sys_loss_is_pending( ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "SysLoss pending, fail command: %x", 
                    cmd->name );

    status = E_FAILURE;
  }
  else if ( hdrsrchtc.tune_away.state != HDRSRCHTC_TUNE_AWAY_GONE )
  {
    switch ( cmd->name )
    {
      case HDRSRCH_ADVISE_UNLOCK_RF_CMD:

#if defined ( FEATURE_FULL_TIME_SHDR_3 )
        /* We need to declare system loss when being kicked off the chain. The
           default command handler does that. */
        status = hdrsrchtc_default_traffic_cmd( cmd );
#else  /* !FEATURE_FULL_TIME_SHDR_3 */
        status = hdrsrchtc_advise_unlock_rf_cmd( );
#endif /* !FEATURE_FULL_TIME_SHDR_3 */

        break;

      case HDRSRCH_IDLE_CMD:
        if ( hdrsrchtc.waiting_for_close_loop == TRUE )
        {
          /* Make sure tuning is complete in TC state before transition
             to IDLE state. */
          status = E_AGAIN;
        }
        else
        {
#ifdef FEATURE_HDR_QTA
          /* Moving to idle state inform APS state end to TRM */
          hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
#endif /* FEATURE_HDR_QTA */
          status = hdrsrchtc_idle_mode_cmd( );
        }
        break;

      case HDRSRCH_TC_ASSIGNMENT_CMD:

        /* If UNLOCK RF immediate command is in the queue, means that HDR needs to 
         * release RF immediately.So, re-post TCA cmd instead of retry so that 
         * UNLOCK RF command is processed ahead of the reposted TCA cmd in the 
         * cmd queue and this would avoid dead lock situation */
        if ( hdrsrchtc.pending_unlock_rf_immed == TRUE )
        {
          tmp_cmd = hdrsrchcmd_new( HDRSRCH_TC_ASSIGNMENT_CMD );
          /* Command HDRSRCH_TASK to enter CONNECTED (traffic) mode */

          if ( tmp_cmd != NULL )
          {
            tmp_cmd->params.tc_assign = cmd->params.tc_assign;
              /* copy payload of the command */

            hdrsrchcmd_post( tmp_cmd );
              /* Re post the command */

            hdrsrchtc.pending_unlock_rf_immed = FALSE; 
              /* reset the pending processing flag */
   
            status = E_SUCCESS;
              /* command is done and remove from queue */
          }
        } /* ( hdrsrchtc.pending_unlock_rf_immed == TRUE ) */
        else
        {
          if ( hdrsrchtc.waiting_for_close_loop == FALSE )
          {
            status = hdrsrchtc_tca_cmd( &cmd->params.tc_assign );
          }
          else
          {
            /* waiting for closed loop */
            status = E_AGAIN;
          }
        }
        break;

      case HDRSRCH_AT_SET_DRC_CMD:
        status = hdrsrchtc_at_set_drc_cmd( cmd->params.at_set_drc.demod_idx,
                                           cmd->params.at_set_drc.drc_param );
        break;

#ifdef FEATURE_HDR_REVC
      case HDRSRCH_UPDATE_LOADADJ_CMD:
        status = hdrsrchtc_update_loadadj_cmd( &cmd->params.loadadj );
        break;

      case HDRSRCH_ENABLE_NLB_CMD:
        status = hdrsrchtc_enable_nlb_cmd( cmd->params.enable_nlb );
        break;
#endif /* FEATURE_HDR_REVC */

      default:
        status = hdrsrchtc_default_traffic_cmd( cmd );
          /* Handle default traffic mode commands */

    } /* switch (cmd->name) */
  }
  else
  {
    status = E_AGAIN;
      /* We are in the process of switching out of the TC state, just retry the command
         for the next state. */
  }

  return status;

} /* hdrsrchtc_cmd(cmd) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_SCHED

DESCRIPTION
  This function programs the search tasks. If the searcher is idle, it checks 
  the diversity status. If diversity is on, set the search type to be 
  HDRSRCH_TC_DIV_SRCH. Otherwise, set the search type to be 
  HDRSRCH_TC_NON_DIV_SRCH. If the previous search task is pending, it only 
  restarts scheduler timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_sched( void )
{
  rex_timer_cnt_type  next_srch_time;
    /* Timer value for next scheduled search. */

  uint16              advance_sched_interval_ms = 0;
    /* Advance the next schedule search interval, if required */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_DLNA
  if ( hdrsrchutil_dlna_is_enabled() )
  {
    advance_sched_interval_ms = HDRSRCHTC_SUSPEND_DLNA_B4_SEARCH_INTERVAL_MS;
      /* Set the advance value to have the timer fire a little bit more than
         2 slots before the actual search interval.  At this early time, 
         disable D-LNA and EPD and set another timer to fire at the intended search 
         interval.  Do this to ensure that we have good samples at the time 
         of the actual search. */

    if ( hdrsrchtc.dlna_state == HDRSRCHTC_DLNA_ACTIVE_STATE )
    {
      /* Set to FALSE since HDR is going to search
         and the samples should be good. */
      hdrsrchutil_send_dlna_ctl( FALSE );
      hdrsrchutil_send_early_pream_ctl( FALSE );

      hdrsrchtc.dlna_state = HDRSRCHTC_DLNA_SUSPEND_STATE; 
        /* Indicate that DLNA is enabled, but suspended */
                                                                              
      next_srch_time = HDRSRCHTC_SUSPEND_DLNA_B4_SEARCH_INTERVAL_MS;
        /* Set timer for the interval between:
             - this early timer occurence to disable DLNA
             - the intended search time */
    
#ifdef FEATURE_RUMI_BRINGUP
     #error code not present
#else
     (void) rex_set_timer( &hdrsrch.schedule_timer, next_srch_time );
        /* schedule next search request at intended time */
#endif /* FEATURE_RUMI_BRINGUP */

     return;
       /* After suspendig DLNA, just return, it's not yet time to program search */
    }
  }
#endif /* FEATURE_HDR_DLNA */

  if ( hdrsrchrfdiv_diversity_demod_is_enabled( ) ) 
  {
    if ( hdrsrchdrv.srch_task_type == HDRSRCH_DIVERSITY_SRCH )
    {
      /* Last search was a combined search and primary search schedule timer
         has expired
         => 1. Program a primary search if we have already received combined
               search result. If it is not okay to perform primary search
               yet, give a go-ahead to submit one on combined search result
               arrival.
            2. Schedule the next combined search */

      if ( hdrsrchtc.okay_to_do_pri_srch )
      {
        /* Program the search only if the searcher is idle */
        if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
        { 
            /* To perform the primary chain ASET search */
          if ( hdrsrchutil_program_pri_chain_aset_search( ) )
          {
            /* Search programmed successfully
               => store the last submitted search type. */
            hdrsrchdrv.srch_task_type = HDRSRCH_PRI_CHAIN_ASET_SRCH;
          }
          else
          {
            HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
              "Primary chain ASET search could not be programmed" );
          }
        }
        else
        {
          HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
            "Primary chain search skipped due to active search [%u]",
            hdrsrchdrv.active_srch );
        }
      }
      else
      {
        hdrsrchtc.okay_to_do_pri_srch = TRUE;
          /* Set flag to submit primary search on combined search result 
             arrival. */
      }

      next_srch_time = HDRSRCHTC_SEARCH_INTERVAL_MS - 
        HDRSRCHTC_COMB_TO_PRI_SEARCH_INTERVAL_MS - advance_sched_interval_ms;
        /* Set next search time to schedule combined search.
           Take into account that the timer may go off earlier to disable
           DLNA if the feature is enabled. */
    }
    else
    {
      /* Last search was a primary chain search and combined search schedule
         timer has expired 
         => 1. Program a combined search
            2. Schedule the next primary search */

      /* Program the search only if the searcher is idle */
      if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
      {
          /* Perform the standard search scheduling */
        if ( hdrsrchutil_program_search_set( ) )
        {
          /* Search programmed successfully
             => store the last submitted search type. */
          hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
        }
        else
        {
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
            "Combined search could not be programmed" );
        }
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
          "Combined search skipped due to active search [%u]",
          hdrsrchdrv.active_srch );
      }

      hdrsrchtc.okay_to_do_pri_srch = FALSE;
        /* Reset okay-to-perform-primary-search flag */

      next_srch_time = HDRSRCHTC_COMB_TO_PRI_SEARCH_INTERVAL_MS;
        /* Set next search time to schedule primary search */
    }
  }
  else
  {
    /* Program the search only if the searcher is idle */
    if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
    {
        /* Perform the standard search scheduling */
      if ( hdrsrchutil_program_search_set( ) )
      {
        /* Search programmed successfully
           => store the last submitted search type. */
        hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
          "Non diversity search could not be programmed" );
      }
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
        "Non diversity search skipped due to active search [%u]",
         hdrsrchdrv.active_srch );
    }

    next_srch_time = HDRSRCHTC_SEARCH_INTERVAL_MS - advance_sched_interval_ms;
      /* Set next search time to schedule search.
         Take into account that the timer may go off earlier to disable
         DLNA if the feature is enabled. */
  }

#ifdef FEATURE_RUMI_BRINGUP
  #error code not present
#else
  (void) rex_set_timer( &hdrsrch.schedule_timer, next_srch_time );
    /* And schedule next search request */
#endif /* FEATURE_RUMI_BRINGUP */

  if ( hdrsrchdrv.srch_task_type != HDRSRCH_DIVERSITY_SRCH )
  {
    hdrsrchset_grice_monitor_asp_snr_order_updates();
      /* Check if SW needs to update ASP SNR order list
         after -
         - submitting non-diverstiy search
         - scheduling primary search in combined-primary search unit */
  }

} /* hdrsrchtc_sched( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_ASET_MGMT

DESCRIPTION
  This function performs Active Set management in Connected (Traffic) State.
  Specifically, if any pilot events occur, the upper layer is notified.

DEPENDENCIES
  None

PARAMETERS
  new_rpts - # of new pilot report events (R/N to C Set, C>A, Weak A)

RETURN VALUE
  None.

SIDE EFFECTS
  May call pilot report callback

===========================================================================*/

void hdrsrchtc_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C Set, C>A, Weak A) */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchmsg_send_multi_asp_update_msg( TRUE );
    /* Send an ASP update message if needed. This is after set management
       but before finger assignment. This is needed to program promoted
       gcset pilots to the FW. */

  if ( new_rpts > 0 )
  {
    hdrsrchtc.pending_pilot_rpt = TRUE;
      /* Hold for later */
  }

} /* hdrsrchtc_aset_mgmt( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_REPORT_BASP_CHANGE

DESCRIPTION
  This function reports change in the best ASP made by the DSP

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_report_basp_change
(
#ifdef FEATURE_HDR_REVB
  uint8 subaset_idx
    /* the subaset index of the report */
#else
  void
#endif /* FEATURE_HDR_REVB */
)
{
  static uint16               ho_count = 0;
    /* initialize with the current value of ASP change count */

  uint16                      ho_count_new;
    /* new handoff count */

  hdrsrch_sect_struct_type *ptr;
    /* sector pointer for new best ASP */

#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type      demod_idx;
    /* Demod carrier index*/

  uint8                       sw_idx;
    /* Local subaset index */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  if( subaset_idx == HDRSRCH_REF_SUBASET_IDX )
  {
    sw_idx = hdrsrchset_get_ref_subaset_idx();
  }
  else
  {
    sw_idx = subaset_idx;
  }

  HDR_ASSERT ( sw_idx < HDRSRCH_MAX_NUM_SUBASETS );
    /* KW fix : Check to limit aset_channel_cnt to max */

  demod_idx = hdrsrchset.subaset[sw_idx].demod_idx;
    /* Get the demod index */
#endif /* FEATURE_HDR_REVB */

  if ( hdrmdsp_check_dsp_clk() )
  {
#ifdef FEATURE_HDR_REVB
    ho_count_new = hdrsrchset_get_asp_change_cnt( sw_idx );
      /* get the new value of handoff count */
#else
    ho_count_new = HDRASP_GET_ASP_CHANGE_CNT();
      /* get the new value of handoff count */
#endif /* FEATURE_HDR_REVB */

    if ( ho_count != ho_count_new )
    {

      hdrsrchlog_set_serving_sect_handoff_count( ho_count_new, 
                                                 FALSE );
        /* Keep track of the number of Handoffs for logging purposes. */
        /* TBD: need work with logging people */

      ho_count = ho_count_new;

#ifdef FEATURE_HDR_REVB
      ptr = hdrsrchset_get_asp_sect_ptr( sw_idx, 
                                         HDRASP_GET_BASP_RL( demod_idx ) );
        /* get the sector pointer for best ASP */
#else
      ptr = hdrsrchset_get_asp_sect_ptr( HDRASP_GET_BASP_RL() );
        /* get the sector pointer for best ASP */
#endif /* FEATURE_HDR_REVB */

      if ( ptr )
      {
        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "BASP: PN=%d EcIo=%d HOCount=%u",
                        ptr->pn_offset,
                        ptr->total_eng,
                        ho_count_new );
      }

    }

  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DSP Clocks not ON. St=%x Dump",
                    hdrsrch_state_id );
  }

}/* hdrsrchtc_report_basp_change */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_RESELECT_EVALUATE_1X

DESCRIPTION
  This function measures the HDR pilot strength, and compares it against
  1x. It also uses HDR DRC filter output as the second handdown metric. 
  Access terminal will not hand down from HDR to 1X, if it has been 
  requesting Non-Null DRCs even when HDR pilot is weak.

  If both HDR and 1x are weak, nothing is done.

  If HDR is weak and 1x is reasonably strong, the AT may be moving out of
  an HDR coverage area, and sends a RESELECT event up to the protocol.
  This may trigger a:
    - reselection to a new (Stronger) HDR system, or
    - handdown from 1xEV to 1x.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchtc_reselect_evaluate_1x( void )
{
  uint16                  aset_eng=0, cset_eng=0, nset_eng=0;
    /* Total ASet energy, Total CSet Energy, and Max NSet Energy */

  uint16                  total_eng;
    /* Sum of ASet, CSet, and best NSet pilot energies */

  int                     eng_db;
    /* Energy, in 0.5 dB units */

  int                     eng_1x_db;
    /* 1x Energy, in 0.5 dB units */

  uint16                  drc_filt;
    /* DRC filter output          */

  uint8                   sa = 0;
    /* Subaset with the max energy */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Determine an strength (raw energy) of all ASet pilots, CSet pilots,
     and the strongest NSet pilot */

  total_eng = hdrsrchutil_get_hdr_pilot_metric( 
                                       HDRSRCHTC_RESELECT_MIN_ENG_THRESHOLD,
                                       &aset_eng, &cset_eng, &nset_eng
#ifdef FEATURE_HDR_REVB
                                       ,&sa
    /* The subaset index with the max energy */
#endif /* FEATURE_HDR_REVB */
                                        );
    /* Add total aset energy, total cset energy, and maximum nset energy */


  if ( total_eng < hdrsrchtc.reselect.threshold_eng )
  {
    /* We are in weak coverage! */

    if ( hdrsrchtc.reselect.weak_coverage_ms == 0 )
    {
#ifdef FEATURE_HDR_REVB
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, "Begin weak coverage: sa=%d a=%d c=%d n=%d",
                      sa, aset_eng, cset_eng, nset_eng );
#else
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Begin weak coverage: a=%d c=%d n=%d",
                      aset_eng, cset_eng, nset_eng );
#endif /* FEATURE_HDR_REVB */

      eng_db = hdrsrchset_eng_to_ecio( total_eng );
        /* Convert to -0.5 db units */

      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Handdown Metric: -%d.%d db",
                      eng_db / 2, (eng_db & 1) * 5 );
    }

    if ( hdrsrchtc.reselect.weak_coverage_ms < HDRSRCHTC_RESELECT_THRESHOLD_MS )
    {
      hdrsrchtc.reselect.weak_coverage_ms += HDRSRCHTC_SEARCH_INTERVAL_MS;
    }
      /* Add in search time only when weak_coverage_ms is smaller than the
         threshold, to prevent it from overflowing                          */

    if ((hdrsrchtc.first_tc_dump == FALSE) &&
        (hdrsrchtc.reselect.weak_coverage_ms > HDRSRCHTC_RESELECT_THRESHOLD_MS))
      /* We don't evaluate handdown metric at the first search dump. We give
         the DRC filter one search cycle to stabilize                     */
    {
#ifdef FEATURE_HDR_REVB
      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "Weak coverage: sa=%d a=%d c=%d n=%d",
                      sa, aset_eng, cset_eng, nset_eng );
#else
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Weak coverage: a=%d c=%d n=%d",
                      aset_eng, cset_eng, nset_eng );
#endif /* FEATURE_HDR_REVB */

      eng_db = hdrsrchset_eng_to_ecio( total_eng );
        /* Convert to -0.5 db units */

      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Handdown Metric: -%d.%d db, >=%d ms",
                      eng_db / 2,
                      (eng_db & 1) * 5,
                      hdrsrchtc.reselect.weak_coverage_ms );

      /* Check 1x energy.
         Down shift by 2, to convert from 1/8th dB to 1/2 dB units. */
      eng_1x_db = srch_hdr_get_channel_estimate() >> 2;

      drc_filt = hdrsrchtc_get_filt_value( );
        /* Get the current DRC filter output value  */

      if ( ( eng_1x_db < HDRSRCHTC_RESELECT_1X_THRESHOLD_DB * 2 ) &&
           ( drc_filt <= HDRSRCHTC_RESELECT_DRC_THRESHOLD ))
      {
        if(hdrmc_feature_is_enabled(HDRMC_FEATURE_HDR_NO_ACTIVE_HANDDOWN))
        {  
          HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Hand-down disabled" );
        }
        else
        {
          HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Reselect: 1x @ -%d.%d",
                          eng_1x_db/2,
                          (eng_1x_db & 1) * 5 );
  
          hdrsrchtc.reselect.weak_coverage_ms = 1;
            /* Reset weak coverage timer to 1. This will prevent HDRSRCH
               from giving more than one HDRSRCH_USABLE_FALSE indication to
               the protocol, if the protocol takes more than one search
               cycle to process HDRSRCH_USABLE_FALSE. By resetting it to 1
               instead of 0, it prevents the "Begin weak coverage" message
               from being emitted in the middle of weak coverage                 */
  
          hdrsrch.system_usable_cb( HDRSRCH_USABLE_FALSE );
            /* HDR System is not usable as per the RESELECT criteria.   */
        }
      }
      else
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_MED, "No Reselect: 1x @ -%d.%d",
                        eng_1x_db/2,
                        (eng_1x_db & 1) * 5 );
      }
    }
  }
  else
  {
    /* We are not in weak coverage */

    if ( hdrsrchtc.reselect.weak_coverage_ms > 0 )
    {
#ifdef FEATURE_HDR_REVB
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, "End weak coverage: sa=%d a=%d c=%d n=%d",
                      sa, aset_eng, cset_eng, nset_eng );
#else
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "End weak coverage: a=%d c=%d n=%d",
                      aset_eng, cset_eng, nset_eng );
#endif /* FEATURE_HDR_REVB */

      eng_db = hdrsrchset_eng_to_ecio( total_eng );
        /* Convert to -0.5 db units */

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Handdown Metric: -%d.%d db, %d ms",
                      eng_db / 2,
                      (eng_db & 1) * 5,
                      hdrsrchtc.reselect.weak_coverage_ms );

      hdrsrchtc.reselect.weak_coverage_ms = 0;
    }
  }

} /* hdrsrchtc_reselect_evaluate_1x */


/*===========================================================================

FUNCTION HDRSRCHTC_RESELECT_EVALUATE

DESCRIPTION
  This function measures the HDR pilot strength, and compares it against
  other systems.

  If both HDR and the other systems are weak, nothing is done.

  If HDR is weak and the other system is reasonably strong, the AT may be
  moving out of an HDR coverage area, and sends a RESELECT event up to the
  protocol.  This may trigger a:
    - reselection to a new (Stronger) HDR system, or
    - handdown from 1xEV to the other system.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchtc_reselect_evaluate( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( srch_hdr_in_service() )
  {
    /* Only run handdown algorithm if handdown to 1x is allowed */
    if ( hdrsrch.handdown_mode == HDRSRCH_1X_HANDDOWN_ENABLED_MODE )
    {
      hdrsrchtc_reselect_evaluate_1x( );
        /* Compare HDR against 1x */
    }
  }
  else
  {
    hdrsrchtc.reselect.weak_coverage_ms = 0;
      /* No other system - make sure reselect "timer" is not running. */
  }

} /* hdrsrchtc_reselect_evaluate */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_DEFAULT_DUMP

DESCRIPTION
  This function processes regular dump and programs ASET primary chain
  search if the search task type is HDRSRCH_DIVERISTY_SRCH.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_default_dump( void )
{
  boolean                    valid;
    /* Flag to indicate whether or not searcher results are valid */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  valid = hdrsrchutil_validate_searcher_results();
    /* Perform searcher result validation before using the results */

  if ( valid )
  {
    hdrsrcheq_update_com( );
      /* Update center of mass for equalizer */

    hdrsrchutil_process_search_set( hdrsrchtc_aset_mgmt );
      /* Perform standard processing on the search results */

    /* TBD */
    /* Should SW wait for finger assignment processed by FW? */
    hdrsrchutil_set_handoff_control();
      /* Set handoff override trigger mode */

    /* Generate report if one is pending, and we have a way of reporting */

    if ( hdrsrchtc.pending_pilot_rpt  &&  hdrsrchtc.pilot_rpt != NULL )
    {
      hdrsrchtc.pending_pilot_rpt = FALSE;
      hdrsrchtc.pilot_rpt( );
        /* Generate a report */
    }

#ifdef FEATURE_HDR_REVB
    hdrsrchtc_report_basp_change( HDRSRCH_REF_SUBASET_IDX );
      /* report change in the best ASP of the reference subaset via F3 msg */
#else
    hdrsrchtc_report_basp_change();
      /* report change in the best ASP via F3 msg */
#endif /* FEATURE_HDR_REVB */

    hdrsrchtc_reselect_evaluate( );
      /* Evaluate system reselection criteria */
  }
  else
  {
    hdrsrchutil_invalidate_search_dump();
      /* invalidate searcher results */
  }

  /* Change the flag to FALSE if this is the first dump in traffic state.
     This has to be at the end of this function.                        */
  if ( hdrsrchtc.first_tc_dump == TRUE )
  {
    hdrsrchtc.first_tc_dump = FALSE;
  }

  /* Update HDR debug display record every 300 ms */
  if ( hdrsrchdrv.dump_cnt % 10  == 0 )
  {
    hdrsrchlog_debug_display_fill_rec();
  }

  /* Call RF API every x search dumps where 
      x = antenna tuner action interval */
  if( ( hdrsrchdrv.dump_cnt % 
    HDRSRCHRF_ANTENNA_TUNER_ACTION_INTERVAL ) == 0 )
  {
    hdrsrchrf_antenna_tuner_action();
      /* Send antenna tuner action tick to RF. */
  }

  hdrfmac_update_handoff_registers( );
    /* Adjust the handoff registers */

  if ( hdrsrchdrv.srch_task_type == HDRSRCH_DIVERSITY_SRCH )
  { 
    /* Last search was a combined search for which search result has arrived
       => Program a primary search if primary search schedule timer has 
          already expired. If it is not okay to perform primary search yet,
          give a go-ahead to submit one on primary search schedule timer 
          expiration. */

    if ( hdrsrchtc.okay_to_do_pri_srch )
    {
      /* Program the search only if the searcher is idle */
      if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
      {
        /* To perform the primary chain ASET search */
        if ( hdrsrchutil_program_pri_chain_aset_search( ) )
        {
          /* Search programmed successfully
             => store the last submitted search type. */
      hdrsrchdrv.srch_task_type = HDRSRCH_PRI_CHAIN_ASET_SRCH;
        }
        else
        {
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
            "Primary chain ASET search could not be programmed" );
        }
      }
      else
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
          "Primary chain search skipped due to active search [%u]",
          hdrsrchdrv.active_srch );
      }
    }
    else
    {
      hdrsrchtc.okay_to_do_pri_srch = TRUE;
        /* Set flag to submit primary search on primary search schedule
           timer expiration. */
    }
  }

} /* hdrsrchtc_default_dump( ) */


#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
/*===========================================================================

FUNCTION HDRSRCHTC_HANDLE_LTE_MEAS

DESCRIPTION
  This function handles LTE meas (D2LR) state machine to measure LTE neighbors while
  DO is in traffic state.
  The following is a description of the D2LR state machine:
  D2LR state is part of DO traffic (hdrsrchtc.d2lr_state.)  This state machine
  is used to decide if DO should make LTE measurement and how to do so to 
  minimize DO throughput degradation.  There are 2 possible tune away scenarios for LTE
  measurement, control by this state machine:
  1.  Piggyback LTE meas:  DO transitions to LTE meas state (if required) right after a tune away.
      This is the preferred method as DRC rampdown is already performed for the tune away.
      DO just uses left over time to make LTE measurement.
  2.  LTE meas with DRC rampdown:  If there is no tune away in the near future and DO is required
      to make LTE measurement, DO performs DRC rampdown for the measurement.  This is the less
      preferred method as the DRC rampdown is only used for LTE measurement.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_handle_lte_meas( void )
{
  dword                             ms_left;
  timetick_type                     elapsed_time;
    /* elapsed time in sclks */

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  timetick_type                  sclks_now, /* current sclks  count */
                                 total_sclks; /* sclks to next TA */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );

  switch( hdrsrchtc.d2lr_state )
  {
    case HDRSRCHTC_D2LR_START:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: HDRSRCHTC_D2LR_START" );
      /* Nothing to be done till CP receives a measurement request.
         As soon as L1 receives the request from CP set a flag
         We will check for the flag here and move the state if
         the flag is set by L1 API */
      break;
      
    case HDRSRCHTC_D2LR_ENABLE_MEAS:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: HDRSRCHTC_D2LR_ENABLE_MEAS" );
      /* Check if aset energy < Thresh */
      /* Check for HDR signal quality */

      elapsed_time =
        timetick_get_elapsed (hdrsrchtc.d2lr_start_meas_time, T_MSEC);
        /* How much time has elapsed since the previous measurement?  DO does not
           want to measure LTE too often as it degrades DO throughput performance. */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED,
          "DLR: HDRSRCHTC_D2LR_ENABLE_MEAS, elapsed time=%d", elapsed_time);

      if ( ( elapsed_time > HDRSRCHTC_LTEMEAS_INTERVAL_MS ) &&
           (
            (( hdrsrchtclte_serving_threshold_is_triggered() == TRUE ) &&
           ( hdrsrchtclte_min_rpt_interval_is_triggered() == FALSE ) &&
             ( hdrsrchtclte_max_report_count_is_triggered() == FALSE )
            )||(hdrsrchtclte_is_in_fast_lte_srch_mode() == TRUE)
           )
         )
      {
        /* Now the HDR serving cell energy is below threshold lets start
         * measuring LTE signal in preparation for handover to LTE */
        /* Check if there are any TA in short time */
        /* Check if there is any TA in next 500 ms if so move to
           wait state otherwise start DRC rampdown */

        ms_left = HDRSRCHTC_MAX_TIME_TO_NEXT_TA + 1;
          /* Assume there is no upcoming TA */

          sclks_now = timetick_get();
            /* Current time in sclks. */

        if ( hdrsrchtc.tune_away.no_tune_away == FALSE )
        {
          if( TIME_OCCURS_FIRST( sclks_now, rl_state.unlock_by_sclk ) )
          {
            ms_left =
              timetick_cvt_from_sclk(
                  ( rl_state.unlock_by_sclk - sclks_now ), T_MSEC );
          }
          else
          {
            ms_left = 0;
          }
        }
          
        HDR_MSG_SRCH_4( MSG_LEGACY_MED,
            "DLR: HDRSRCHTC_D2LR_ENABLE_MEAS, tune away? %d ms_left=%d %d %d",
            hdrsrchtc.tune_away.no_tune_away,
            ms_left,
            rl_state.unlock_by_sclk,
            sclks_now );

        if(ms_left < HDRSRCHTC_MAX_TIME_TO_NEXT_TA)
        {
          // Piggy back case
          HDR_MSG_SRCH_1( MSG_LEGACY_MED,
              "DLR: HDRSRCHTC_D2LR_WAIT_MEAS,ms_left=%d",ms_left );
          hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_WAIT_MEAS;
            /* There is an upcoming TA that we can use to piggy back our
             * measurements after the TA is complete. So no need to open
             * a new gap for L measurement
             */
        }
        else if ( hdrsrchtc_tune_away_allowed() )
        {
          if ( hdrsrchtclte_request_trm_grant(
                HDRSRCHTC_MAX_TA_TIME_FOR_D2L_MEAS,
                HDRSRCHTC_DRC_RAMP_MIN_MS + 5 ) )
        {
          // DRC rampdown case
          HDR_MSG_SRCH_1( MSG_LEGACY_MED,
              "DLR: HDRSRCHTC_D2LR_DRC_RAMPDOWN,ms_left=%d",ms_left );
          /* There is no TA coming up in near future that we can use to
           * piggy back our measurements after the TA is complete. So
           * need to open a new gap for L measurement
           */

            total_sclks = timetick_get() + HDRSRCHTC_DRC_WORSTCASE_RAMP_SCLK +
              TIMETICK_SCLK_FROM_MS ( 5 );

              /* Total ticks to completion of rampdown */

            HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR: unlock_by in sclk %d",
                 total_sclks );
            hdrsrchrf_set_lock_state ( HDRSRCHRF_PRI_CHAIN, total_sclks );
              /* Set unlock_by for the next internal TA request */

            hdrsrchtc_process_tune_away( HDRSRCHTC_DRC_RAMP_MIN_MS + 5 );
              /* Trigger internal TA request */

            hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_DRC_RAMPDOWN;
        }
      }
      }
      break;
      
    case HDRSRCHTC_D2LR_WAIT_MEAS:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: HDRSRCHTC_D2LR_WAIT_MEAS" );
      /* wait for QTA/LTA gap. After returning from TC susp or OFS state
         HDR will measure L. Then it will move the state to D2L_MEASURE if
         TRM allows us to continue */
      break;
      
    case HDRSRCHTC_D2LR_DRC_RAMPDOWN:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: HDRSRCHTC_D2LR_DRC_RAMPDOWN" );
      break;
      
    case HDRSRCHTC_D2LR_MEASURE:
      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DLR: HDRSRCHTC_D2LR_MEASURE" );
      /* Wait till measurement is complete. After that go back
       * to HDRSCRCHTC_D2LR_START_MEAS state
       */
      break;

    default:
      ERR_FATAL(" Invalid D2LR state ...", 0, 0, 0 );
  }

} /* hdrsrchtc_handle_lte_meas */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_DUMP

DESCRIPTION
  This function processes the dump based on the search task type.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchtc_dump( void )
{

  boolean                           last_search_dump = FALSE;
    /* Indicate if this is the last search dump for this search cycle since
       diversity mode has 2 search dumps */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchdrv.srch_task_type == HDRSRCH_PRI_CHAIN_ASET_SRCH )
  {
    hdrsrchutil_pri_chain_aset_dump( );
    last_search_dump = TRUE;
      /* Primary Aset search is always the last search so this is the last dump */
  }
  else
  {
    hdrsrchtc_default_dump( );
    if ( !hdrsrchrfdiv_diversity_demod_is_enabled( ) ) 
    {
      last_search_dump = TRUE;
        /* Diversity demod is not enabled so this is the last dump */
    }
  }

#ifdef FEATURE_HDR_DLNA
    /* To save power, D-LNA and EPD should be enabled in FW until the next
       search cycle
       Check 1: Feature has been enabled
       Check 2: This is the last search dump in the scheduled search cycle
       Check 3: Query CXM if it is ok to enable D-LNA based on DSDA band
        combinations
       Check 4: check FAC gain estimate is available
        Note:With D-LNA enabled, FAC gain estimation takes longer time. To 
        allow faster settling of FAC Gain we keep D-LNA disabled till initial
        FAC Gain estimate is available after powerup.
       Check 5: D-LNA state is suspended
        Note: Below check on dlna_state will take care of the case where 500 ms
        delay_dlna_timer is still running. In that case dlna_state is inactive
        and it should not be enabled here */
  if ( ( hdrsrchutil_dlna_is_enabled() ) && 
       ( last_search_dump == TRUE ) &&
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
       ( hdrmultirat_cxm_allows_dlna() ) &&
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
       ( hdrfmac_is_fac_gain_est_available() ) &&
       ( hdrsrchtc.dlna_state == HDRSRCHTC_DLNA_SUSPEND_STATE ) )
  {
    hdrsrchutil_send_dlna_ctl( TRUE );
    hdrsrchutil_send_early_pream_ctl( TRUE );
    hdrsrchtc.dlna_state = HDRSRCHTC_DLNA_ACTIVE_STATE;
  }
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  if ( last_search_dump == TRUE )
  {
    hdrsrchtc_handle_lte_meas();
      /* If this is the last search dump, give the D2LR state machine a chance to
         determine if DO should make LTE measurement. */
  }
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
  
} /* hdrsrchtc_dump( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCH_INIT_CONNECTED_STATE

DESCRIPTION
  This function is called to transition from Idle State to
  Connected State

DEPENDENCIES
  Expects a Suspended TCA Command for TCA parameters.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchtc_init_connected_state( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchtc_state );
    /* Switch to connected state processing */

} /* hdrsrchtc_init_connected_state( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCH_RESUME_CONNECTED_STATE

DESCRIPTION
  This function is called to transition from Idle State to
  Connected State

DEPENDENCIES
  Expects a Suspended TCA Command for TCA parameters.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchtc_resume_connected_state( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchtc_state );
    /* Switch to connected state processing */

} /* hdrsrchtc_resume_connected_state( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION      HDRSRCHTC_DONE

DESCRIPTION
  Exit processing for search connected state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes state transition.

===========================================================================*/

void hdrsrchtc_done
(
  hdrsrch_state_enum_type next_state
    /* next state after connected state */
)
{
#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_done( next_state );

  /* If tune-away has started, we need to cancel it since we are transitioning
     into idle state instead                                                */
  if ( hdrsrchtc.tune_away.state != HDRSRCHTC_TUNE_AWAY_NO_CONFLICT )
  {
    hdrsrchtc_abort_tune_away( );
  }
  

  hdrsrchtc.pending_unlock_rf_immed = FALSE; 
   /* reset the pending processing flag */

  if ( hdrsrchafc_afc_uses_rot() )
  {
    hdrsrcheq_deassign_all_eqs( );
    /* Deassign all equalizers on leaving Traffic(0x04). This ensures that it 
       is good for HDR only and hybrid mode operations.
       Equalizers in Broadcast - TBD */
  } /* if hdrsrchafc_afc_uses_rot() */

  /* The following if-else statement implement:
              next_state  action
     ASDIV:   4a          suspend
              4c          no-op 
              4d          suspend
              others      exit tc
   
     ARD:     4a          suspend
              4c          suspend   
              4d          suspend
              others      exit tc */

#ifdef FEATURE_HDR_SELECTIVE_TXD
    payload.next_state = next_state;
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
  if ( ( next_state == HDRSRCH_SUSPENDED_TC_STATE ) ||
       ( next_state == HDRSRCH_LTE_MEAS_TC_STATE ) )
#else
  if ( next_state == HDRSRCH_SUSPENDED_TC_STATE )
#endif
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_SUSPENDING, 
                                     &payload );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_SUSPEND );
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
  }
  else if ( next_state == HDRSRCH_OFS_TC_STATE )
  {
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_SUSPEND );
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
  }
  else
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_EXITING_TRAFFIC, 
                                       &payload );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
    hdrsrchard_event( HDRSRCHARD_EVENT_EXIT_TC );
#else
    hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

    hdrsrchafc_set_tap( FALSE );
     /* tap is not running */
  }

  if ( next_state != HDRSRCH_SUSPENDED_TC_STATE )      
  {
#ifdef FEATURE_HDR_DUAL_SIM

#ifdef FEATURE_HDR_QTA
    if ( hdrmultirat_is_dsds_enabled() 
#ifdef FEATURE_HDR_DR_DSDS
         || hdrmultirat_is_dr_dsds_enabled()
#endif /* FEATURE_HDR_DR_DSDS */
       )
    {
      if( hdrsrchtc_is_qta_nv_enabled() == TRUE )
      {
        trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_QUICK_TA_SUPPORTED, FALSE );
          /* BG traffic end allow G to sleep */
      }
    }
#endif /* FEATURE_HDR_QTA */   

    if ( hdrmultirat_is_dsds_enabled() )
    {
      trm_set_extension_flag( TRM_HDR, TRM_BG_TRAFFIC, TRM_PRIORITY_INV_ENABLED, FALSE );

      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR DSDS: Disabled BG traffic" );
    }
#endif /* FEATURE_HDR_DUAL_SIM */
  }

#ifdef FEATURE_HDR_REVB 
  if (next_state == HDRSRCH_IDLE_STATE)
  {
     hdrsrchrf_tc_enter_idle( &hdrsrch.aset_channel[0] );
        /* Update the hdrsrchrf */

     srch_hdr_set_hdr_in_traffic( FALSE );
       /* Tell 1x search that HDR is no longer in traffic */
#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
     hdrsrchtclte_release_trm_grant();
    /* Cancel outstnding TRM grant for D2L redirection, if any. */
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

  }
#endif /* FEATURE_HDR_REVB */

  if( next_state != HDRSRCH_SUSPENDED_TC_STATE )
  {
    hdrsrchmsg_send_msg( &hdrsrchmsg.tuneaway_stop_msg.hdr, 
                         sizeof( hdrfw_tuneaway_stop_msg_t ) );
      /* Get back to normal mode of DRC i.e. stop DRC ramp down mode.
         (Leave it enabled in suspend state; it will be cleared on restart. 
         This keeps the DRC supervision timer in the MDSP frozen.) */

    #ifdef FEATURE_HDR_QTA
    if (hdrsrchtc.tune_away.is_qta == TRUE )
    {
      hdrsrchtc_set_qta_end( );

      HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR QTA: QTA flag was set and clearing it" );
      /* clear QTA flag if QTA is not really started */
    }
#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_BAND_AVOIDANCE
    (void) timer_clr( &hdrsrch.band_avoid_timer, T_NONE );
      /* Clear the timer, and remove from wait mask */
#endif /* FEATURE_HDR_BAND_AVOIDANCE */
  }

  hdrsrchrfdiv_enable_div_ramp_down();
    /* Enable diversity ramp down. */

//hdrmdsp_clr_slot_timer( hdrsrchtc.tune_away.slot_timer_handle );
//      /* clear the slot timer handler, if it is active */

  (void) rex_clr_timer( &hdrsrchtc.tune_away.timer );
    /* clear the tune away timer if it is active */

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* stop the search schedule timer */

  hdrsrchtc_disable_tune_aways();
    /* Disable tune aways now */

  hdrsrch_kill_timer( HDRSRCHTC_OFS_TA_TIMER );
    /* Clear the stage2 timer, and remove from wait mask */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  (void) timer_clr( &hdrsrch.coex_timer, T_NONE );
    /* Clear the timer, and remove from wait mask */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_DLNA
  hdrsrch_kill_timer( HDRSRCHTC_DELAY_DLNA_ENABLE_TIMER );
  if ( hdrsrchutil_dlna_is_enabled() )
  {
    /* Disable DLNA and early preamble detection when exiting
       traffic state. */
    hdrsrchutil_send_dlna_ctl( FALSE );
    hdrsrchutil_send_early_pream_ctl( FALSE );
    hdrsrchtc.dlna_state = HDRSRCHTC_DLNA_INACTIVE_STATE;
  }
#endif /* FEATURE_HDR_DLNA */

  ( void ) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                          HDRSRCH_FING_ASSIGN_RSP );
    /* Before changing the clocks, make sure FW processed finger deassignment
       if there was any pending finger deassignment */

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_issue_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Request CPU for HDR wakeup. */
#endif

#ifdef FEATURE_HDR_QTA
  if (hdrsrchtc.tune_away.is_qta == FALSE )
  {
#endif /* FEATURE_HDR_QTA */
    hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_TX_REQ );
    /* Notify TX data disable status to MCPM */
#ifdef FEATURE_HDR_QTA
  }
#endif /* FEATURE_HDR_QTA */

  hdrsrchutil_mcdo_check();
    /* Change MDSP clock speed, if necessary. */

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_TRAFFIC );
    /* Cancel CPU request for HDR traffic. */

#endif /* FEATURE_NPA */

  if( next_state == HDRSRCH_INACTIVE_STATE )
  {
#ifdef FEATURE_HDR_QTA
    hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
      /* Inform TRM for APS state end */   
#endif /* FEATURE_HDR_QTA */
  }
} /* hdrsrchtc_done( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHTC_SEND_PILOT_REPORT

DESCRIPTION
  This function is originally designed to be used during TC OFS state, to
  send out any route update message when needed.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE when route update is sent out successfully.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchtc_send_pilot_report( void )
{
  boolean                    reported = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchtc.pending_pilot_rpt  &&  hdrsrchtc.pilot_rpt != NULL )
  {
    hdrsrchtc.pending_pilot_rpt = FALSE;
    hdrsrchtc.pilot_rpt( );
      /* Generate a report */

    reported = TRUE;
  }

  return reported;

} /* hdrsrchtc_send_pilot_report( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHTC_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for traffic state.

DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchtc_process_int_cmd
(
  hdrsrchsm_cmd_payload_type                     *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( payload->cmd )
  {
    case HDRSRCH_AFC_CLOSE_LOOP_CMD:

      /* Closed loop */

      if ( hdrsrchtc.waiting_for_close_loop == TRUE )
      {
        hdrsrchtc.waiting_for_close_loop = FALSE;
          /* We don't actually track until we assign fingers, but this is close
             enough, there isn't any harm declaring this a bit early, and
             we currently don't check to see when we end up acquiring again
             when we resume from traffic. */

        if ( hdrsrchtc.is_slam_needed == FALSE )
        {
        hdrsrchtc_resume_traffic_init();
          /* Now go ahead and resume traffic. */
      }
      }

      break;
    
    case HDRSRCH_AFC_TCXO_RELEASED_CMD:
      /* TCXO is released */
      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected internal cmd %d at TC state",
                      payload->cmd );
      break;

  }  /* switch */
}  /* hdrsrchtc_process_int_cmd */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_GET_NUM_CELLS_IN_ASET

DESCRIPTION
  This function is used to get the number of unique cells in an Active Set.
    
DEPENDENCIES
  None

PARAMETERS
  subaset_idx - the subaset index of the subaset requseted.

RETURN VALUE
  Number of unique cells in an Active Set.

SIDE EFFECTS
  None

==========================================================================*/
uint8 hdrsrchtc_get_num_cells_in_aset 
(
#ifdef FEATURE_HDR_REVB
  uint8 subaset_idx
    /* The subaset index of request */
#else
  void
#endif
)
{
  uint8  idx = 0;
    /* local subaset idex */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_REVB
  if( ( subaset_idx > hdrsrchset.subaset_cnt ) &&
      ( subaset_idx != HDRSRCH_REF_SUBASET_IDX ) )
  {
    ERR_FATAL(" Invalid subaset index ...", 0, 0, 0 );
  }
  else if( subaset_idx == HDRSRCH_REF_SUBASET_IDX )
  {
    idx = hdrsrchset_get_ref_subaset_idx();
      /* Get the reference subaset idex if not valid */
  }
  else
  {
    idx = subaset_idx;
  }

  idx = MIN ( idx , ( HDRSRCH_MAX_NUM_SUBASETS - 1 ) );
    /* KW fix : Limit the index to maximum */

  return hdrsrchtc.num_cells_in_subaset[idx];
#else
  return hdrsrchtc.num_cells_in_aset;
#endif /* FEATURE_HDR_REVB */
} /* uint8 hdrsrchtc_get_num_cells_in_aset (void) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_REGISTER_VOIP_OUTAGE_CB

DESCRIPTION
  This registers VOIP outage notification function with
  traffic state.
    
DEPENDENCIES
  None

INPUTS
  voip_outage_cb - pointer to the VOIP outage callback function.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_register_voip_outage_cb
( 
  hdrsrch_voip_outage_notify_cb_type    voip_outage_cb
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  voip_tuneaway.outage_cb = voip_outage_cb;

} /* hdrsrchtc_register_voip_outage_cb */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_CLEAR_VOIP_OUTAGE_CB

DESCRIPTION
  This clears VOIP outage notification function pointer.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_clear_voip_outage_cb( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  voip_tuneaway.outage_cb = NULL;
    /* set VOIP outage notification call back to NULL */
    
} /* hdrsrchtc_clear_voip_outage_cb */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_TC_OFS_TIME

DESCRIPTION
  This function records the last OFS start time.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_update_tc_ofs_time
(
  timetick_type          sclks_now
    /* current sclks count */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc.tune_away.ofs_time_sclks = sclks_now;
    /* update the last OFS start time */
    
} /* hdrsrchtc_update_tc_ofs_time */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_TC_OFS_GROUP_TIME

DESCRIPTION
  This function records the last OFS search group start time.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_update_tc_ofs_group_time
(
  timetick_type          sclks_now
    /* current sclks count */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc.tune_away.ofs_group_time_sclks = sclks_now;
    /* update the last OFS search group start time */
    
} /* hdrsrchtc_update_tc_ofs_group_time */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_TC_OFS_CHECK_TIME

DESCRIPTION
  This function records the time for last TC OFS condition evaluation.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_update_tc_ofs_check_time
(
  timetick_type          sclks_now
    /* current sclks count */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchtc.tune_away.ofs_check_time_sclks = sclks_now;
    /* update the last OFS check time, need to check 
       ofs condition again in HDRSRCHTC_OFS_CHECK_INTERVAL_MS */
  
} /* hdrsrchtc_update_tc_ofs_check_time */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHTC_OFS_ALLOWED

DESCRIPTION
  This function checks if OFS is allowed (set by TAP)
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrchtc_ofs_allowed( void )
{
  return !hdrsrchtc.ofs_disabled;
}
/*==========================================================================

FUNCTION        HDRSRCHTC_TUNE_AWAY_ALLOWED

DESCRIPTION
  This function checks if tune away is allowed
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrchtc_tune_away_allowed( void )
{
  if ( hdrsrch_is_lre_enabled() )
  {
     return (!hdrsrchtc.tune_away.urgent_rel_only 
                && ( hdrsrchtc.substate != HDRSRCHTC_PENDING_ON_TCCACK_SUBSTATE ) );
  }
  else
  {
    return !hdrsrchtc.tune_away.urgent_rel_only;
  }
}

/*==========================================================================

FUNCTION        HDRSRCHTC_GET_PRIMARY_TA_TIME

DESCRIPTION
  This function return the primary chain tune away time (in sclk).

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Primary chain tune away time (end_time_sclks)

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchtc_get_primary_ta_time( void )
{
  return hdrsrchtc.tune_away.end_time_sclks;
}

#ifdef FEATURE_HDR_DLNA
/*===========================================================================

FUNCTION HDRSRCHTC_DLNA_ENABLE_PROC

DESCRIPTION
  This function is called in traffic state after
  HDRSRCHTC_DELAY_DLNA_ENABLE_DURATION
  In this, D-LNA state is set to suspend.
  Later after processing search dump, state is set to Active and then
  D-LNA and EPD are enabled.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchtc_dlna_enable_proc( void )
{
    /* Based on the votes from other clients, ex: RE/FETAP, feature is
       temporarily turned ON/OFF. So we first check the feature status
       before changing DLNA state.
       Note: Here we set D-LNA state to suspend. It is set to Active later,
       after search dump processing is completed */

    if ( hdrsrchutil_dlna_is_enabled() )
    {
       hdrsrchtc.dlna_state = HDRSRCHTC_DLNA_SUSPEND_STATE;
    }
}
#endif /* FEATURE_HDR_DLNA */

#ifdef FEATURE_HDR_QTA
/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_QTA_MODE

DESCRIPTION
  This function checks whether QTA is allowed based on current status

DEPENDENCIES
  None

INPUTS
  retain lock state information

RETURN VALUE
  TRUE: in QTA
  FALSE: in LTA

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_update_qta_mode( 
  hdrsrchrf_retain_lock_state_t  * rl_state
    /* State of retain lock. */
)
{
  boolean curr_qta;

  curr_qta = hdrsrchtc.tune_away.is_qta;

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR QTA evt %d, subaset %d TA %d",
                rl_state->event,
                hdrsrchset.subaset_cnt,
                hdrsrchtc.tune_away.is_qta);
  
#ifdef FEATURE_HDR_DUAL_SIM
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR QTA dsds %d",
                hdrmultirat_is_dsds_enabled());
#endif /* FEATURE_HDR_DUAL_SIM */


  if ( ( rl_state->event != HDRSRCHRF_UNLOCK_CANCELLED ) &&
#ifdef FEATURE_HDR_DUAL_SIM
#ifdef FEATURE_HDR_DR_DSDS
            (hdrmultirat_is_dsds_enabled() ||
             hdrmultirat_is_dr_dsds_enabled()) &&
#endif /* FEATURE_HDR_DR_DSDS */
#endif /* FEATURE_HDR_DUAL_SIM */
      ( ( rl_state->winning_client.client_id == TRM_GSM1 ) || 
        ( rl_state->winning_client.client_id == TRM_GSM2 ) || 
        ( rl_state->winning_client.client_id == TRM_GSM3 ) ||
        ( rl_state->winning_client.client_id == TRM_UMTS ) ||
        ( rl_state->winning_client.client_id == TRM_UMTS2 ) 
      ) 
      && ( hdrsrchtc.tune_away.is_qta_enabled_via_nv == TRUE) 
      && ( hdrsrchset.subaset_cnt == 1) && hdrrmac_is_tx_on() /*RF does not support multi carrier QTA */
      && ( hdrmod_pa_is_on() ) )  
  {
    /* Use the TA information from TRM */
    switch ( rl_state->ta_info )
    {
      case TRM_QUICK_TUNEAWAY:
        hdrsrchtc.tune_away.is_qta = TRUE;
        break;

      case TRM_LONG_TUNEAWAY:
      default:
        hdrsrchtc.tune_away.is_qta = FALSE;
        break;
    }
	HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "override HDR QTA %d based on TRM info",hdrsrchtc.tune_away.is_qta );

    if((hdrsrchtc.tune_away.is_qta == TRUE) && 
        (( rl_state->winning_client.client_id == TRM_UMTS ) ||
        ( rl_state->winning_client.client_id == TRM_UMTS2 )) &&
        (hdrsrchtc.tune_away.force_lta == TRUE))
    {
        hdrsrchtc.tune_away.is_qta = FALSE;
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
			            "Force LTA is enabled : Override QTA to LTA");
    }
	
  }
  else
  {
    hdrsrchtc.tune_away.is_qta = FALSE;
  }

  hdrsrchtc.tune_away.ta_client_id = rl_state->winning_client.client_id;
  HDR_MSG_SRCH_7( MSG_LEGACY_HIGH, "HDR QTA win client %d reason %d, QTA NV Enabled %d TX %d, PA ON %d,Update QTA mode %d --> %d",
                rl_state->winning_client.client_id,
                rl_state->winning_client.reason,
                hdrsrchtc.tune_away.is_qta_enabled_via_nv,
                hdrrmac_is_tx_on(), /* If TX is OFF then disallow QTA */
                hdrmod_pa_is_on(),  /* If PA is OFF then disallow QTA */
                curr_qta,
                hdrsrchtc.tune_away.is_qta);
}


/*==========================================================================

FUNCTION        HDRSRCHTC_UPDATE_QTA_ON_RLSI

DESCRIPTION
  This function checks whether QTA is allowed based on RLSI scheduled

DEPENDENCIES
  None

INPUTS
  time_to_ta  -  Time in-which TA is planned (in ms)

RETURN VALUE
  TRUE: in QTA
  FALSE: in LTA

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_update_qta_on_rlsi( 
  uint32 time_to_ta
)
{
  qword  frame , temp_slot;
  uint32 slot_num = 0;

  /* If TA is scheduled and QTA is true */
  if( hdrsrchtc.tune_away.is_qta == TRUE )
  {
    /* Check whether the QTA falls in silence duration */ 
    hdrts_get_cycle_and_slot(frame, 
                             &slot_num, 
                             16);

    qw_mul( temp_slot, frame, HDRTS_SLOTS_PER_FRAME );
      /* Convert frames into slots */

    slot_num = slot_num + MS_TO_SLOTS(time_to_ta);
      /* Add remaining slots of current frame with slots left before TA */

    qw_inc( temp_slot, slot_num );
      /* Add remaining slots with current slot number */

    qw_div( frame, temp_slot, HDRTS_SLOTS_PER_FRAME );
      /* convert total slots to frame */

    /* Check If the frame in which QTA is in silence duration */
    if ( hdrrmac_is_silence_frame( frame ) )
    {
      hdrsrchtc.tune_away.is_qta = FALSE;
        /* Cancel QTA if silence duration intersects QTA */

      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "HDR QTA: QTA collides with RLSI and hence cancel QTA" );
    }
  }
}


/*==========================================================================

FUNCTION        HDRSRCHTC_IS_QTA_ENABLED

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
boolean hdrsrchtc_is_qta_enabled( void )
{
  return hdrsrchtc.tune_away.is_qta;
}

/*==========================================================================

FUNCTION        HDRSRCHTC_SET_QTA_END

DESCRIPTION
  This function sets QTA end.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_set_qta_end( void )
{
  hdrsrchtc.tune_away.is_qta = FALSE;

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_suspend_asdiv( FALSE, TRM_ASDIV_STATE_UPDATE_REASON_QTA );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
}

/*==========================================================================

FUNCTION        HDRSRCHTC_SET_QTA_NV

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
void hdrsrchtc_set_qta_nv( boolean is_enabled )
{
  hdrsrchtc.tune_away.is_qta_enabled_via_nv = is_enabled;

}

/*==========================================================================

FUNCTION        HDRSRCHTC_GET_QTA_RXLM_BUF_INDEX

DESCRIPTION
 This function returns RXLM buffer index which is allocated during QTA

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  RXLM Buffer index allocated during QTA

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchtc_get_qta_rxlm_buf_index( void )
{
  return hdrsrchtc.tune_away.rxlm_qta_target_buf_idx;
}


/*==========================================================================

FUNCTION        HDRSRCHTC_IS_QTA_NV_ENABLED

DESCRIPTION
  This function gets QTA NV status.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Status of QTA NV item

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrchtc_is_qta_nv_enabled( void )
{
  
  return hdrsrchtc.tune_away.is_qta_enabled_via_nv;
}

/*==========================================================================
 
FUNCTION        HDRSRCHTC_DISABLE_TX_QTA

DESCRIPTION
  This function disables Tx after QTA.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchtc_disable_tx_qta( void )
{

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,"HDR QTA end, turning Tx OFF!" );

  hdrsrchrfdiv_diversity_disable_after_qta();
    /* Disable diversity if we are not going to traffic */

  /* If QTA is enabled then tune is done by now and end QTA */
  hdrsrchtc_set_qta_end();

  hdrsrchfing_deassign_all_fings( );
    /* Unassign fingers */
  
  if ( hdrsrchafc_afc_uses_rot() )
  {
    hdrsrcheq_deassign_all_eqs( );
      /* Deassign all equalizers before TCA tune (before rx_stop) */
  }

  /* Deactivate HDR decoder to cancel all active packets */
  hdrsrchutil_dec_deactivate( );
      
  /* Disable TX */
  hdrsrchutil_powerdown_tx( HDRRMAC_TX_STOP_FOR_TUNEAWAY );
    /* RF Tx chain is disabled (instant) */

  rxlm_deallocate_buffer( hdrsrchtc.tune_away.rxlm_qta_target_buf_idx );
    /* Done with QTA now */
  
}
#endif /* FEATURE_HDR_QTA */

/*===========================================================================

FUNCTION HDRSRCHTC_IS_TA_STARTED

DESCRIPTION
  This function returns tune away status in traffic. This will be helpful in 
  stopping connection close processing while tuning away in traffic.

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

boolean hdrsrchtc_is_ta_started( void )
{

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  if ( ( hdrsrchtc.tune_away.no_tune_away == FALSE ) 
    && ( hdrsrchtc.substate == HDRSRCHTC_TCC_ACK_RECEIVED_SUBSTATE )) 
  {
    if( hdrsrchtc.tune_away.state == HDRSRCHTC_TUNE_AWAY_GONE )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,"TA state = %d, return is_ta TRUE", hdrsrchtc.tune_away.state );
      
      return TRUE;
        /* already tuned away TA = TRUE */
    }
    else
    {
      return FALSE;
        /* No Tune away as there is no timer running. return TA = FALSE */
    }
  }
  else
  {
    return FALSE;
      /* No Tune away and return TA = FALSE */
  }
} /* hdrsrchidle_is_ta_started */


/*===========================================================================

FUNCTION HDRSRCHTC_IS_OFS_SCHEDULED

DESCRIPTION
  This function returns whether OFS or tune away scheduled is near. 
  This will be helpful in stopping ASDiv switch

DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if OFS scheduled
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchtc_is_ofs_scheduled( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If ofs tune away is HDRSRCHTC_TUNE_AWAY_TX_STOP then OFS is near */
  if( ( hdrsrch_get_current_state() == HDRSRCH_CONNECTED_STATE ) 
    && ( hdrsrchtc.tune_away.state > HDRSRCHTC_TUNE_AWAY_DRC_RAMP ) )
  {
    return TRUE;
      /* Tune away is near */
  }
  else
  {
    return FALSE;
      /* No Tune away and return TA = FALSE */
  }
} /* hdrsrchidle_is_ofs_started */


/*===========================================================================

FUNCTION HDRSRCHTC_CHANGE_SUBSTATE

DESCRIPTION
  This function changes the substate of hdrsrchtc. 
  This function currently handles only connection close substate

DEPENDENCIES 
  None.

PARAMETERS
  Sub-State

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchtc_change_substate( boolean connection_close_state )
{
  if( connection_close_state )
  {
      hdrsrchtc.last_substate = hdrsrchtc.substate;
        /* Store previous state */

      hdrsrchtc.substate = HDRSRCHTC_CONNECTION_CLOSE_INPROGRESS_SUBSTATE;
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
        "Connection close inprogress, block TA" );
  }
  else if ( hdrsrchtc.last_substate == HDRSRCHTC_TCC_ACK_RECEIVED_SUBSTATE )
  {
      hdrsrchtc.substate = hdrsrchtc.last_substate;
        /* Retrive previous state and continue with old substate */
  }
  else
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
      "Invalid TA substate, Requested substate %d, TA substate %d, TA last substate %d", 
      connection_close_state, hdrsrchtc.substate, hdrsrchtc.last_substate );
      /* invalid substate */
  }
}


#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
/*==========================================================================

FUNCTION HDRSRCHTC_READY_TO_MEASURE_LTE

DESCRIPTION
  Check the D2LR state machine to determine if DO has performed DRC rampdown. 
 
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
 TRUE  - done with DRC rampdown
 FALSE - DRC rampdown has not been performed

SIDE EFFECTS
  none

==========================================================================*/

boolean hdrsrchtc_ready_to_measure_lte( void )
{
  HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR DLR: state %d",
                  hdrsrchtc.d2lr_state );
  return ( hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_DRC_RAMPDOWN );
}

/*==========================================================================

FUNCTION HDRSRCHTC_IS_MEASURING_LTE

DESCRIPTION
  Check the D2LR state machine for MEASURE state.
 
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
 TRUE  - D2LR state machine is in HDRSRCHTC_D2LR_MEASURE
 FALSE - different state

SIDE EFFECTS
  none

==========================================================================*/
boolean hdrsrchtc_is_measuring_lte( void )
{
  HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR DLR: state %d",
                                                 hdrsrchtc.d2lr_state );
  return ( hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_MEASURE );
}

/*==========================================================================

FUNCTION HDRSRCHTC_START_MEASURE_LTE

DESCRIPTION 
  When DRC ramp down is finished, transition to the next state to prepare for
  LTE measurement.
  The following is a description of the D2LR state machine:
  D2LR state is part of DO traffic (hdrsrchtc.d2lr_state.)  This state machine
  is used to decide if DO should make LTE measurement and how to do so to 
  minimize DO throughput degradation.  There are 2 possible tune away scenarios for LTE
  measurement, control by this state machine:
  1.  Piggyback LTE meas:  DO transitions to LTE meas state (if required) right after a tune away.
      This is the preferred method as DRC rampdown is already performed for the tune away.
      DO just uses left over time to make LTE measurement.
  2.  LTE meas with DRC rampdown:  If there is no tune away in the near future and DO is required
      to make LTE measurement, DO performs DRC rampdown for the measurement.  This is the less
      preferred method as the DRC rampdown is only used for LTE measurement.
 
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  none

==========================================================================*/
void hdrsrchtc_start_measure_lte( void )
{
  if ( hdrsrchtc.d2lr_state == HDRSRCHTC_D2LR_DRC_RAMPDOWN )
  {
    hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_MEASURE;
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR DLR: state %d",
                                                   hdrsrchtc.d2lr_state );
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR DLR: Incorrect state %d",
                                                   hdrsrchtc.d2lr_state );
  }
}

/*==========================================================================

FUNCTION HDRSRCHTC_TCLTE_TUNE_DONE

DESCRIPTION 
  Upon returning to traffic state, this function is called once RF finishes
  tuning back to DO.
 
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  none

==========================================================================*/
LOCAL void hdrsrchtc_tclte_tune_done
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

  hdrsrchtc.d2lr_state = HDRSRCHTC_D2LR_MEASURE;
  hdrsrchtclte_init_tclte_state();
    /* Transition to LTE measurement state */
}
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */


/*===========================================================================

FUNCTION HDRSRCHTC_SET_FORCE_LTA

DESCRIPTION
  This function sets the force LTA. Based on this, QTA is overriden for LTA,
  only for W client

DEPENDENCIES
  None

PARAMETERS
  value - if force LTA needs to be enabled or not

RETURN VALUE
  None.

SIDE EFFECTS
  Sets the "force_lta" variable

===========================================================================*/

void hdrsrchtc_set_force_lta
(
boolean value
)
{
   HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Force LTA curr %d updated %d",
                   hdrsrchtc.tune_away.force_lta, value );
   hdrsrchtc.tune_away.force_lta = value;
}

/*==========================================================================

FUNCTION HDRSRCHTC_IS_CONNECTION_CLOSE_IN_PROGRESS

DESCRIPTION
  Check if Searcher is in the middle of connection close

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  TRUE:  Searcher is in Connection close substate
  FALSE: Otherwise

SIDE EFFECTS
  None

==========================================================================*/
boolean hdrsrchtc_is_connection_close_in_progress( void )
{
  if ( hdrsrchtc.substate == HDRSRCHTC_CONNECTION_CLOSE_INPROGRESS_SUBSTATE ) 
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* hdrsrchtc_is_connection_close_in_progress */

/*==========================================================================

FUNCTION HDRSRCHTC_GET_TA_WINNING_CLIENT

DESCRIPTION
  API to get TA winning client

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  TA winning client

SIDE EFFECTS
  None

==========================================================================*/
trm_client_enum_t hdrsrchtc_get_ta_winning_client( void )
{
  return hdrsrchtc.tune_away.ta_client_id;
}

