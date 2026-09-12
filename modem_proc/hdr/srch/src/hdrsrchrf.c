/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                H D R   S E A R C H   R F   F U N C T I O N S

GENERAL DESCRIPTION

  This module provides rf routines for wakeup, sleep, and channel changing.
  It also provides management of the RF Rx/Tx resources for HDR.

EXTERNALIZED FUNCTIONS (Global)

  Resource Management Functions:

  hdrsrchrf_init() - Must be called before any other function, to initialize
    module.


  hdrsrchrf_reserve_at() - Registers the need to access an RF chain at a 
    specific moment in the future for the indicated duration, such as for
    a paging slot.  hdrsrchrf_request() must be used at the specified moment,
    to actually obtain the RF resource.  Implicitly performs a 
    hdrsrchrf_release().

  hdrsrchrf_request() - Request access to an RF chain immediately, for the 
    indicated duration.  Implicitly performs a hdrsrchrf_release().

  hdrsrchrf_request_and_notify() - Request access to an RF chain, when it 
    becomes available.  Implicitly performs a hdrsrchrf_release().


  hdrsrchrf_retain_lock() - Requests that the lock be retained indefinitely
    so that the client can be asked to release the lock when needed.

  hdrsrchrf_get_retain_lock_state() - Retrieves information about when 
    the lock should be released if retain_lock was called.

  hdrsrchrf_extend_duration() - Requests an increase in the duration the 
    RF is held.  If the increase cannot be granted, the original grant 
    duration remains in effect.

  hdrsrchrf_change_duration() - Requests a change in the duration the RF 
    is held for.  If the full extension cannot be granted, a partial 
    extension may result.

  hdrsrchrf_change_priority() - Specifies a change in the reason (and 
    consequently priority) for a hold on an RF resource.

  hdrsrchrf_cancel_reserve_notify() - Indicates one no-longer wants an RF
    resource, specified in hdrsrchrf_reserve_at() or 
    hdrsrchrf_request_and_notify(). If the caller has been granted an RF
    resource IT IS NOT RELEASED.

  hdrsrchrf_release() - Releases the hold on an RF resource.  If no
    one else is waiting for the RF resource, the RF will be turned off.

  hdrsrchrf_has_lock() - Whether HDR has the lock on the given resource.

  hdrsrchrf_is_shdr_supported() - Whether the hardware supports SHDR operation.


  RF Tune Related Functions:

  hdrsrchrf_tune_to_chan() - This function tunes the RF to the specified 
    band and channel and then calls the given callback when done.

  hdrsrchrf_prep_to_sleep() - This function makes sure mDSP is in ACTIVE
    state before putting RF to sleep.

  hdrsrchrf_report_rx_power() - This function reports the RX power through 
    diag.

  hdrsrchrf_diversity_status() - Returns the diversity status ON/OFF.

  hdrsrchrf_diversity_ctrl() - Call this function to enable or disable 
    diversity. It will be enabled whenever the RF resources are available
    and the primary chain is enabled.  Otherwise, it will be be off.

  hdrsrchrf_pdm_capture_values() - Capture AGC PDMs so that they can be
    restored later.

  hdrsrchrf_pdm_restore_values() - Restore AGC PDMs after a tune away.

  hdrsrchrf_diversity_init() - Init drivers for diversity.

  hdrsrchrf_consider_div_switch() - Consider switching the diversity 
    settings based on the AGC values to improve overall throughput performance.

  hdrsrchrf_set_diversity_comb_mode() - Set the diversity settings.

  hdrsrchrf_user_diversity_pref() - Whether diversity is set to on or off.

  hdrsrchrf_set_diversity_band_mask() - Sets per band diversity mask.
  
  hdrsrchrf_check_div_for_band() - Checks if diversity is enabled for a 
    particular band class.

  hdrsrchrf_get_rf_dev() - Returns the identifier associated with the 
    granted RF chain.

  hdrsrchrf_cancel_in_progress_tunes() - Cancel any tunes that are currently
    in process.  This causes callbacks waiting for the end of a tune never
    to be called.

  hdrsrchrf_get_rx_power() - Get the current RX power for chain 0 and chain 1.

  hdrsrchrf_get_channel() - Get the current channel.

  hdrsrchrf_div_chain_event() - Processes diversity related events from TRM.

  hdrsrchrf_has_multi_rx() - Queries TRM if mulitple RX chains are supported.

  hdrsrchrf_set_agc_acq_params() - Sets the RF AGC acquisition parameters. 

  External RF Control Functions in Support of DAST:

  hdrsrchrf_reset_agc_est() - This function resets the AGC stability channel. 
    estimator.

  hdrsrchrf_est_agc_stability() - Estimates the stability of the AGC.

  hdrsrchrf_force_resume_agc_off() - Indicates that for reasons other than 
    the AGC estimator can determine, the resume AGC acquisition algorithm
    shouldn't be used for the next wakeup.
  
  hdrsrchrf_get_prev_agc() - Returns the last good AGC value seen while 
    awake on the same channel.

  hdrsrchrf_agc_is_stable() - Returns whether the AGC is stable enough 
    to use the resume AGC acquisition algorithm for DAST.

  hdrsrchrf_disable_div_ramp_down() - This function disables the RF module from performing ramp down
    activities related to disabling the diversity chain.

  hdrsrchrf_enable_div_ramp_down() - This function allows the RF module to perform ramp down
    activities related to disabling the diversity chain.

  hdrsrchrf_get_pri_rxlm_buffer_handle() - Returns the primary chain RXLM buffer
    handle, if one has been allocated.

  hdrsrchrf_request_to_sleep() - This function gives the RF module a chance
  to do any activities it needs to do to prepare for sleep.  For now that
  means stopping the AGC.
 
  hdrsrchrf_change_reason() - Specifies a change in the reason (LTE measurements)
    for a hold on an RF resource.
 
  hdrsrchrf_prep_for_irat_meas() - Prepares TRM for D2L measurements.
 
REGIONAL FUNCTIONS

  hdrsrchrf_tune_sig() - Handles the tune timer signal.


INITIALIZATION AND SEQUENCING REQUIREMENTS

  hdrsrchrf_init() must be called before any other function.

  Before using any RF functions, a user must be granted permission via
  hdrsrchrf_request( ) or hdrsrchrf_request_and_notify().

  When the user is finished using the RF, it must release its hold on the
  RF chain either directly via a call to hdrsrchrf_release(), or indirectly by
  calling hdrsrchrf_reserve_at(), hdrsrchrf_request_and_notify(), or 
  hdrsrchrf_request().


Copyright (c) 2005 - 2021 by Qualcomm Technologies, Inc.  All Rights Reserved.

=*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*=



==============================================================================

                           EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchrf.c#2 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
05/18/2021   vaa     Fix for compilation errors in MSIM disable build flavor
05/17/2019   vlc     Changed TRM parameters to be static. 
05/17/2019   rmv     Fill ta_info as TRM_TUNEAWAY_NONE when calling TRM 
                     Request, Reserve At, R&N APIs
04/05/2019   rmv     Fixed the crash during concurrent execution of TRM R&N API 
11/19/2018   svu     Added hdrsrchrf_has_chain() API  
02/20/2018   vke     Ignore tuneaway for LTE page reason in access state
02/07/2017   vke     FR36756 Changes to update TRM for APS state change
12/22/2017   vke     FR42394 PBR randomization for non-DR idle feature
10/29/2017   vlc     Changed client using HDR subscription ID in TRM exchange.
04/18/2016   rmv     DO+W QTA support 
04/18/2016   rmv     Mainline FEATURE_HDR_QTA_THRU_TRM 
05/27/2016   vko     Add QSH event for TRM lock wait timeout
06/10/2016   vke     Changes to clear the RxTX pending timer when 
                     the system deactivate is processed
05/27/2016   vko     QSH framework for events capturing
05/16/2016   vko     Reduce excessive F3
12/16/2015   vko     Fixed KW issues
11/18/2015   arm     Make sure we have valid chan id before rf warmup 
10/27/2015   rmv     Prevent reporting QTA END twice to TRM 
10/08/2015   vke     Use TA information from TRM callback for deciding QTA
10/07/2015   mbs     Supported internal DRC rampdown for D2LR during QTA/LTA
10/04/2015   mbs/wsh Fixed going into D2L even after TA is requested
10/01/2015   vlc     Added check for wakeup_rx_pending flag in hdrsrchrf_request_to_sleep(). 
09/30/2015   vko     Update subreason type to sys_proc_type_e_type
08/10/2015   mbs/vke Supported ACQ/ACQ_INV mechanism to be compliant with TRMU
09/14/2015   wsh     Use new antenna tuner API
09/14/2015   wsh     Fixed using demod_page reason on TX chain
08/20/2015   arm     Atlas RUMI change, program PBS registers
08/07/2015   wsh     Prevent chipx16 cb blocking deactivate 
08/06/2015   mbs     Mapped HDR access_urgent reason to be compliant with TRMU
07/30/2015   vke     Fixed klockwork issues
07/28/2015   wsh     Fixed race cond. between TRM release and grant cb 
07/24/2015   wsh     Fixed deactivate not getting processed 
07/24/2015   wsh     Fixed TX chain event being treated as PRI chain 
07/22/2015   mbs     Prevent out of sequence calling of RF APIs
07/22/2015   mbs     Supported handling unlock cancel just before TA
07/17/2015   mbs     Corrected API used to confirm lock granted during exchange
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
07/10/2015   wsh     Fixed crash due sys loss not processed in time 
06/30/2015   wsh     Added WTR hopping support
06/29/2015   wsh     Temp. disable WTR hopping during band tune
06/29/2015   vko     Pass subreason also, while calling the TRM API
06/26/2015   vke     Changes to handle band request with timeout timer
06/22/2015   wsh     Fixed not releasing TX chain 
06/22/2015   rmv     Retain req_notify_pending flag before doing system lost 
                     to use and release Tx later when releasing PRx
06/18/2015   tnp     Fix compiler warnings
06/09/2015   tnp     Update feature flag configuration
06/04/2015   wsh     Fixed FreqID mismatch
06/02/2015   wsh     Fixed possible corruption of tune.state
06/02/2015   mbs     Added more devices to be protected during QTA
05/22/2015   wsh     Added debug code for TRM/HDR out of sync 
05/15/2015   vko/vke Added TABASCO to featurization
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
05/04/2015   mbs     Supported QTA using TRM API
04/21/2015   wsh     Added per-band enable/disable to all states
04/01/2015   wsh     Cleaned up critical_section usage 
04/01/2015   wsh     Fixed double entering HDR
04/01/2015   mbs     Supported RF interface change
03/26/2015   kss     Added hdrsrchrf_get_reason(). 
03/20/2015   ljl     Added debug f3 msg.
03/09/2015   mbs     Restore the tune state on band grant cancel
02/10/2015   vke     Changes to return the band support information based on
                     whether the RF device is owned
02/09/2015   arm     Clear hdrsrchrf tune timer also when cancelling band grant 
                     request if it is pending for OFS state. 
01/27/2015   arm     Removed deprecated TRM API (trm_get_simult_cap)
01/22/2015   arm     Cancelled band grant request if it is pending for OFS 
                     state.
01/21/2015   vke     Changes to return bool If FEATURE_HDR_DR_DSDS 
                     is defined otherwise none
12/23/2014   arm     Passed correct resource to TRM while doing trm exchange.
12/19/2014   bb      Changes to restore is_tuned information if band tune
                     is pending from TRM
12/19/2014   arm     Removed deprecated TRM APIs in HDR 
12/18/2014   vke     Changes to return from tune_to_chan when the given band 
                     is not supported
12/17/2014   wsh     Fixed compile warnings
12/15/2014   bb      Changes to clear owns_chain while doing R&N 
11/20/2014   vke     Changes to check whether band channel is supported by 
                     the given rf device
11/26/2014   sat     Changes to call hdrsrchrfdiv_enable_diversity_chain API
11/19/2014   wsh     Added DR-DSDS support for ASDiv
11/19/2014   vke/sat Changes to store QTA winning client id and share with TRM
10/20/2014   arm     Ignore band grant pending from TRM if we dont own the
                     chain
10/20/2014   arm     Handled band grant pending from TRM.
10/14/2014   wsh     Changes to support Jolokia bringup
09/29/2014   arm     Provide  minimum duration for immediate unlock-by time
                     when doing change reason.
09/29/2014   arm     DR-DSDS code cleanup
09/22/2014   arm     HDR QTA code cleanup
09/11/2014   vlc     For L2D, use RF device provided by LTE. 
08/29/2014   arm     Fixed bug in updating DR mode from MCS.
08/26/2014   bb      Changes to handle diversity release when unlock event is 
                     ignored by primary client during Access state
08/18/2014   sat     Handling TRM_DENIAL in RF grant CB from TRM.
08/15/2014   mbs     Supported fall-back to TRM R&N when trm_exchange fails
08/12/2014   tnp     Fix compiler warnings
08/12/2014   wsh     Disable RTT in TAP call; force HiCap if boost, new threshold 
08/07/2014   sat     Okay to shorten extend duration in IRAT case
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
07/10/2014   arm     Use new TRM API for diversity as well.
06/30/2014   arm     Changes to support DR-DSDS.
06/04/2014   arm     Changes to support DO to G QTA
06/24/2014   dsp     Corrected FEATURE_HDR_DISABLE_ACCESS_TA_GSM
05/15/2014   arm     TRM API changes for DR-DSDS. 
06/12/2014   sat     Fixing Klockwork issues
08/27/2013   arm     Fixed wrong num channels being reported to MCS.
05/14/2014   vke     Fix to prevent blocking of TAs for 1x and G due to 
                     bug in the HDR TRM-interface
05/08/2014   dsp     Disabled Access TA for GSM.
05/12/2014   vke     Klockworks fix
04/03/2014   kss/wsh Added T2P boost for RTT reduction
02/21/2014   vlc     Added hdrsrchrf_prep_for_irat_meas(). 
12/19/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
12/16/2013   dsp     Removed update_aci_mitigation(). 
12/04/2013   sat     Removed unused parameters in F3s.
11/27/2013   dsp     FR17482 for Page blocking management.
10/29/2013   sat     Reduced F3 messages (or/and) ASSERTs
09/25/2013   wsh     Changes to support BOLT bring up
09/04/2013   wsh     Changes to support BOLT bring up
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/01/2012   vke     Fixed klockworks error, limited the size of num_channel
06/26/2013   wsh     Added Selective TxD feature
05/15/2013   vke     Fix for not changing the TRM request priority for idle
                     state from traffic suspend state
05/31/2013   vko     Fixed klocwork issues
05/24/2013   smd     Reverted KW fixes.
04/22/2013   arm     Added changes for DSDA coexistence manager. 
05/17/2013   wsh     Gracefully handle RxLM initialization error
05/16/2013   vko     Fixed klocwork issues
05/06/2013   mbs     Fixed compiler errors for BOLT build
04/25/2013   wsh     Fixed deactivate not processed due to bad RF cal 
03/21/2013   kss     Remove unused legacy code. 
03/04/2013   vko     Fixed compiler warnings
01/02/2013   mbs     Released primary chain only if HDR owns it
02/19/2013   vlc     Added new parameters for Pilot Meas Req message from LTE.
02/11/2013   rkc     Make sure RF is not put to sleep if already asleep.
01/15/2013   vlc     Removed obsolete hdrsrchrf_agc_settle_delay_hs.
11/24/2012   vko     Fix Klocwork issues
12/12/2012   wsh/dsp Fixed usage of tune_time before it is initialized.
12/04/2012   mbs     Fixed compilation errors due to removal of LTE on Triton
09/27/2012   ljl     Delayed 4 HS for RxAGC settling.
09/25/2012   rmg     Added support for including freq info in TRM interaction.
09/14/2012   wsh     Reset chain state before calling Req&Notify
09/05/2012   rmg     Added support for antenna tuner feature.
08/15/2012   ljl     Fixed the featurization.
08/03/2012   ljl     Updated based on new RF/FW interface. 
07/12/2012   wsh     Releases TRM lock with Req&Notify for when enter suspend
06/06/2012   arm     Do not error fatal if rf returns bad rf cal error. 
06/04/2012   dsp     hdrsrchrf_get_tx_power_info() now populates pa_gain_state.
05/23/2012   wsh     Clean up band tune logic and added safety checks
05/21/2012   ljl     Skipped RF APIs for Dime RUMI IQ interface.
04/27/2012   wsh     Fixed using wrong band info after IDLE OFS/ACQ
04/09/2012   vlc     Added hdrsrchrf_change_reason() to work with D2L meas. 
04/17/2012   cnx     Support rfm_is_band_chan_supported_v2().
04/12/2012   wsh     Use prev valid RF dev for band checking
03/20/2012   arm     Resolve TCB errors for Dime. 
03/19/2012   ljl     Fixed Dime compiler errors.
03/08/2012   wsh     Removed deprecated sample server interface
02/19/2012   wsh     Moved Req&Notify pending logic to hdrsrchrf.c
02/14/2012   wsh     Fixed TRM check for IRAT under leagacy SHDR config
02/03/2012   ljl     Checked RF cal before acquisition.
02/03/2012   wsh     Added support for SVLTE+SHDR
01/09/2011   wsh     Fixed compiler warnings. 
01/06/2011   grl     Indicate to 1x when we change bands in case of conflict.
01/05/2011   wsh     Fixed crash in HHO to same band 
12/22/2011   vlc     Added hdrsrchrf_request_to_sleep() to work with D2L meas.
12/07/2011   wsh     ERR_FATAL if RF returned unrecoverable error status 
11/15/2011   rkc     Removed unnecessary RF CSR update in TC to idle transition
10/24/2011   wsh     Updated cancel FPD procedure 
09/20/2011   smd     Removed not-needed CLK REGIME calls for IRAT L2DO.
07/26/2011   ljl     Checked whether RF is enabled before disabling it.
07/19/2011   smd     Mainlined FEATURE_IRAT_USE_MEAS_RF_API.
07/06/2011   lyl     Fixed rxAGC1 value for SVLTE type I and type II.
05/05/2011   cnx     Added hdrsrchrf_get_band_subclass().
04/27/2011   kss     Added RF include to remove warnings. 
04/27/2011   smd     IRAT L2DO updates for NikeL.
04/21/2011   kss     Added support for rxlm parameter in rfm_enter_mode().
04/17/2011   kss     Added support for rfm_hdr_exec_wakeup_rx().
04/15/2011   kss     Fixed featurization issue in last check-in. 
04/01/2011   kss     Integrate RxLM APIs.
03/24/2011   kss     Updated RF SVDO API.
03/05/2011   arm     New API added to provide Rx and Tx power info to CM. 
02/22/2011   rmg     Changed call to rfm_hdr_wakeup_rx to support RF change.
02/03/2011   rmg     Changed to updated RF tune API.
01/21/2011   rmg     Supported RF API changes for NikeL.
11/11/2010   smd     Used CLKRGM_MODEM_RXF_ANT0_SAMPLE_FB_CLK for rxf0_samp_clk.
10/26/2010   smd     Returned status at end of meas_setup_and_build_script().  
09/08/2010   pxu     Removed HDRSRCH_SCHEDULE_SIG before rf tune and added back 
                     after tune_done.  
09/17/2010   smd     Used new rf APIs for connected mode. 
07/30/2010   ljl     Removed DEM_SYM_BUFF_MODE config for pilot measurement. 
06/29/2010   vlc     Fixed compiler warnings.
06/21/2010   smd     Removed redundant rf calls of setting agc mode.
06/21/2010   LJL     REMOVED hdrsrchrf_temp_config_carrier_mode().
06/21/2010   cnx     Fixed compiler warnings.
06/15/2010   ljl     Supported RF MSGR interface.
06/10/2010   smd     Added support for RF meas API of idle mode.
06/10/2010   kss     Remove cast of REX_ENABLE_PREMPTION due to Core change.
06/02/2010   ljl     Removed mdsp.h.
06/05/2010   smd     Fixed bugs of calling RF meas API.
05/29/2010   grl     Removed extra character. 
05/18/2010   arm     Fixed high lint warnings. 
05/12/2010   ljl     Disabled rx before hdr fw standby.
04/26/2010   lyl     Added brute force acquistion support.
04/08/2010   smd     Mainlined hdrsrchrf_rfm_cb() and hdrsrchrf_rfm_wait().
04/05/2010   smd     Added RF APIs for pilot measurement.
03/25/2010   pxu     Removed FEATURE_HDR_IDLE_TIMELINE_V3 checks.
03/18/2010   smd     Added hdrsrchrf_force_chain_unlock() API for IRAT.
03/18/2010   smd     Added hdrsrchrf_force_chain_lock() API for IRAT.
12/07/2009   ljl     Defined hdrsrch_trm_skip_rfm_api.
12/01/2009   grl     Changes for MDM9K Bringup.
11/09/2009   lyl     Added hdrsrchrf_force_demod_carrier_zero().
10/27/2009   wsh     Fixed compiler warning for ARM compiler
10/23/2009   rkc     Verified return code of sample_server_configure().
10/21/2009   rkc     Configured only sample server for pri chain when granted.
10/01/2009   ljl     Commented out HDRAGC_SET_TX_OPEN_LOOP_MC for RUMI bringup.
08/11/2009   etv     Used actual demod idx instead of HDRMDSP_DEMOD_CARRIER_0.
07/13/2009   rkc     Send HDRSRCHRFDIV_RELEASE_DIV_CMD to RF Diversity STM
                     during unlock callback for HDRSRCHRF_DIV_CHAIN.
07/09/2009   rkc     Setup sample server for CDMA mode.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
06/30/2009   lyl     Updated for rxAGC unlocked mode interface.
06/19/2009   grl     Split out the diversity state machine from this file.
05/28/2009   rkc     Passed in CSR index info to RF driver for SVDO.
05/27/2009   rkc     Added temporary call to rfm_hdr_disable().
05/21/2009   rkc     Called rfm_hdr_sleep_rx()  in hdrsrchrf_prep_to_sleep().
                     Moved calls rfm_hdr_enable() and rfm_hdr_wakeup_rx() to
                     hdrsrchrf_tune().
05/18/2009   lyl     Updated for firmware MSGR interface.
05/13/2009   rkc     Removed redundant hdrsrchrf_tune() for SVDO
                     Called rfm_hdr_enable() in hdrsrchrf_enable_rf_chain().
05/04/2009   kss     Minor lint fixes.
04/29/2009   rkc     Fixed rex set/clr signal to hdrsrch_tcb.
                     Changed rfm_enter_mode() param to RFM_SVDO_MODE for SVDO.
04/16/2009   rkc     Mainlined T_MSM7600.
04/08/2009   grl     Updated func name to hdrsrchrf_is_shdr_supported.
04/02/2009   wsh     Temp change per band diversity logic to fix compile error
03/31/2009   kss     Initialized AGC registers in MDSP as early as possible.
03/28/2009   etv     Moved decoder and demsymbuff clock configuration from
                     hdrsrchrf_tune_sig to hdrdec_activate
03/20/2009   lyl     Fixed tuneAwayTime setting during hard diversity tuneaway.
03/19/2009   grl     Added per band diversity support to Rev B.
03/18/2009   rkc     Added RF SVDO API changes.
02/26/2009   etv     Floor RxAGC1 during diversity tune away.
02/04/2009   lyl     Fixed the lint errors.
01/28/2009   lyl     Defined the RF tune timer before it was used.
01/19/2009   smd     Fixed lint errors.
12/10/2008   smd     Replaced assert.h with amssassert.h
12/05/2008   kss     Removed/replaced intlocks.
12/04/2008   rmg     Added support to enable/disable ACI mitigation.
12/03/2008   ljl     Reduced the frequency of dynamic diversity F3 message.
11/23/2008   grl     Added ARDA feature to module.
11/21/2008   rkc     Fixed lint errors.
11/19/2008   ljl     Used the correct chain_id after tune done.
                     Removed the redundant rfm_enable_rx().
10/07/2008   lyl     Fixed lint errors.
10/07/2008   ljl     Fixed compiler warnings.
08/06/2008   ljl     Added hdrsrchrf_is_tuned().
08/15/2008   wsh     Fixed crash if TCA contains invalid channel
07/17/2008   smd     Fixed the klocwork error.
07/08/2008   ljl     Configured diversity mode to firmware.
07/03/2008   grl     Featurized code to support new and old sampserv iface.
06/30/2008   etv     Fixed typo with featurization in tune_done function
06/24/2008   gs      Updated sample server interface to include carrier.
06/08/2008   grl     Added hdrsrchrf_is_hpt_supported API for FTS.
05/15/2008   ljl     Fixed compile warning.
05/09/2008   etv     Created hdrsrchrf_rf_chain_tune_done to find if tuning is
                     done on the given chain.
05/09/2008   rkc     Fixed compiler warnings.
04/28/2007   ljl     Added support for switching div modes dynamically
04/10/2008   rkc     Added rf enable and tune times to debug msg.
03/28/2008   grl     Supported preping for wakeups with the RTC already on.
03/27/2008   ljl     Removed the RXF hack.
02/11/2008   ljl     Called rfm_enter_mode() if HDR did not own RF.
02/05/2008   ljl     Included hw.h.
01/30/2008   jyw     Enforced csr usages the same as demod index assignments.
                     Removed hdrsrchrf_get_next_available_csr( void ).
01/20/2008   jyw     Included demod index in the tuning and quering interfaces.
01/22/2008   jyw     Added a ghost channel for the test purpose.
01/16/2008   ljl     Used global variable for channel list passed to HDR 
                     SRCH AFC.
01/11/2008   ljl     Called rfm_get_lna_gain_state() to get lna state.
01/15/2008   vish    Added support for MC RF Rx/Tx AGC APIs.
12/18/2007   rkc     Add temp code to set afc chan in hdrsrchrf_start_tune()
12/18/2007   ljl     Removed lna hack.
12/12/2007   grl     Added missing include.
11/26/2007   ljl     Configured carriers 1 and 2 for 7800 bringup.
11/14/2007   jyw     Added the power strobe event id.
11/14/2007   grl     Featurized 7600 specific register write.
11/12/2007   grl     Changed featurization of hw system mode configuration.
11/11/2007   ljl     Updated for MDM7800 bringup.
11/06/2007   etv     Added support for PB diversity.
10/31/2007   ljl     Merged from Rev B branch.
10/17/2007   jyw     Merged Rev B changes from branch.
10/04/2007   rkc     Resolved Lint High warnings.
09/20/2007   mt      HDR Adaptive diversity implementation.
09/18/2007   wsh     Report correct DiversityComb and SimultaneousCommonChannelReceive
                     value in Multi-Mode Discovery protocol
07/18/2007   mt      HDR per band diversity changes.
06/01/2007   pa      Use MSMHW_MODEM_CLK_MISC_CTL_2_INVALID for clock fix.
05/29/2007   pa      Added antenna clock work around for 7600.
04/10/2007   mt      Mark RX chain as untuned in hdrsrchrf_prep_to_sleep().
02/20/2007   grl     Linted clean.
02/16/2007   grl     Fixed issue where NULL cover may not be released.
01/25/2007   rkc/hrk Programmed 7600 specific registers.
01/17/2007   ljl     Added HDR SRCH AFC stm.
12/27/2006   grl     Added APIs to disable diversity DRC ramp downs.
12/05/2006   grl     Added F3 to indicate when delayed AGC enable was skipped.
11/30/2006   grl     Use hard div tune aways only when served small packets.
11/10/2006   kss     Make sure RF is enabled before enabling AGC.
10/25/2006   grl     Improved SHDR throughput using hard div tune aways.
10/20/2006   kss     Use MDSP timer to enable AGC ACQ 1 hs after wakeup.
10/09/2006   grl     Print out DAST algorithm debug output when it is off.
09/29/2006   grl     Converted agc acq time to half slots from ms.
09/25/2006   grl     Fixed compilation warning.
09/22/2006   ljl     Added feature FEATURE_TRM_API_V2.
09/11/2006   kss     Call rf_hdr_init_agc() to init AGC earlier on wakeup.
08/23/2006   grl     Force channel stability based on NV setting.
08/21/2006   grl     Added DAST support.
08/15/2006   mt      Changed HDRSRCHRF_TUNE_DELAY_MS to 0 ms.
08/01/2006   mt      Added support for new fast AGC acquisition algorithm.
07/24/2006   jyw     Added power profiling strobes.
06/14/2006   sq      Made RF loop settling time programmable
04/28/2006   ljl     Saved channel info if it is different.
04/20/2006   ljl     Saved tune info when changing channel.
03/23/2006   grl     Save the DC offsets when we turn off the AGC.
01/15/2006   jyw     Added support to query of the exist of multiple rx chain.
01/19/2006   grl     Reset retain lock when changing the lock duration.
11/14/2005   grl     Notified the DSP of div tune aways early.
11/14/2005   grl     Fixed reset for traffic termination during SHDR.
10/25/2005   ds      Changed input parameter for hdrsrchafc_init_fll().
09/10/2005   grl     Added trm_mode enable and disable functions.
09/01/2005   grl     Added retain lock callback unregistering function.
08/01/2005   grl     Div now restarts after 1x slots complete.
06/20/2005   ds      Fix for RF div status update before finger div init
06/03/2005   sfm     srch_rx naming changes
05/31/2005   grl     Initial AMSS version from trm.h and the old hdrsrchrf.h

==========================================================================*/

/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/


#include "hdr_variation.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfi.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchi.h"
#include "hdrsrchset.h"
#include "hdrmdsp.h"

#include "lm_types.h"
#include "rxlm_intf.h"
#include "txlm_intf.h"
#include "rfm_hdr.h"
#include "rfm_cdma.h"

#include "hdrmdspmcr.h"

#include "hdrdebug.h"

#include "hdrsrch.h"
#include "hdrsrchafc.h"
#include "hdrsrcheq.h"
#include "hdrsrchfing.h"
#include "hdrsrchdrv.h"
#include "hdrsrchset.h"
#include "hdrsrchmsg.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchrf.h"

#include "trm.h"
#include "rex.h"
#include "timetick.h"
#ifndef FEATURE_HDR_DIME_MODEM
#include "hw.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#include "srch_hdr.h"

#ifdef FEATURE_HDR_REVB
#include "amssassert.h"
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_RUMI
#include "hdrhitmsg.h"
#endif /* FEATURE_HDR_RUMI */

#include "hdr_log_qsh.h"
#include "hdrutil.h"

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#define HDRSRCHRF_TUNE_ERROR -1
  /* Tune was unsuccessful due to bad rf cal */

#define HDRSRCHRF_TUNE_OK 1
  /* Tune was successful */

#define HDRSRCHRF_TUNE_DELAY_US 0
  /* How long we should wait after tuning. */

#define HDRSRCHRF_RX_PWR_OFF -128
  /* What RX power to report when an antenna is off. */

#define HDRSRCHRF_NORMAL_AGC_ACQ_DISPATCH      16
  /* Number of dispatches used for normal AGC acquisition. */

#define HDRSRCHRF_FAST_AGC_ACQ_DISPATCH        10
  /* Number of dispatches used for fast AGC acquisition. */

#define HDRSRCHRF_MAX_AGC_EST_PERIOD_SCLK  TIMETICK_SCLK_FROM_MS( 525 )
  /* Maximum time between AGC estimates before the particular AGC comparison
     is invalidated. */

#define HDRSRCHRF_MIN_AGC_EST_PERIOD_SCLK  TIMETICK_SCLK_FROM_MS( 100 )
  /* Minimum time between AGC estimates before the particular AGC comparison
     is invalidated. */

#define HDRSRCHRF_MAX_AGC_FLUCTUATION      6425
  /* The largest acceptable delta between the AGC of the last tracking
     interval and any AGC value of the current interval. 6425 corresponds
     with 10 dBm. */

#define HDRSRCHRF_ENTRY_RESUME_THRESH       4
  /* The maximum percentage of AGC overshoots before the AGC resume acquisition
     algorithm is engaged. */

#define HDRSRCHRF_EXIT_RESUME_THRESH        4
  /* If the percentage of AGC overshoots exceeds this number, the AGC 
     resume acquisition algorithm is disengaged. */

#define HDRSRCHRF_RX_STOP_RSP_US            834 /* 1HS */
  /* Max time to wait for RX_STOP_RSP */

hdrsrchrf_struct_type         hdrsrchrf;
 /* HDR Searcher RF parameters consolidated in one structure */

/* Revisit */
hdrsrchafc_chan_list_type     hdrsrchrf_chan_list;
  /* Multi-carrier channel list */

static uint8					hdrsrchrf_trm_log_info_num_of_entries;
  /* Logging number of entries from trm grouped async call back */

#define HDRSRCHRF_MIN_UNLOCK_DURATION        0
  /* Minimum duration for immediate unlock-by time (in sclks)
     For DO this value is 0, because we donot have a change reason
     that would cause DR to non DR switch.*/

#define HDRSRCHRF_RXTX_WAIT_MS               6000
  /* Time to wait for RxTx grant */

#if defined FEATURE_HDR_RUMI && defined FEATURE_HDR_ATLAS_MODEM
#define PBS_CTL_ADDR HWIO_ADDR(MSS_RESERVE_01)
uint16 HDRSRCH_RFLM_DTR_PBS_CFG  =           0x2;
#endif /* FEATURE_HDR_RUMI  && FEATURE_HDR_ATLAS_MODEM */

#define HDRSRCHRF_RXAGC_DELTA_HIGH   10
  /* Maximum acceptable delta between Rx0 and Rx1 */

/*==========================================================================

                     FORWARD DECLARATIONS FOR MODULE
                     
==========================================================================*/

void hdrsrchrf_grant_callback( trm_client_enum_t client, 
                               trm_grant_return_enum_type grant,
                               trm_request_tag_t tag );
static void hdrsrchrf_cancel_tune_timer( hdrsrchrf_logical_chain_enum_t chain_id );

/*==========================================================================

                EXPORTED RF RESOURCE MANAGEMENT FUNCTIONS
        
==========================================================================*/
/* This enum keeps track of RxTx chain status after processing 
   TRM async event */
typedef enum
{
  HDRSRCHRF_RXTX_NOT_OWNED,
  HDRSRCHRF_RXTX_OWNED,
  HDRSRCHRF_RXTX_GRANTED,
  HDRSRCHRF_RXTX_UNLOCKED
} hdrsrchrf_rxtx_chain_status_enum_type;

#define HDRSRCHRF_RXTX_OWNED_OR_GRANTED( status ) \
  ( ( status == HDRSRCHRF_RXTX_OWNED ) || \
    ( status == HDRSRCHRF_RXTX_GRANTED ) )

/*===========================================================================

FUNCTION HDRSRCHRF_RFM_CB

DESCRIPTION
  This is the generic callback function passed to rfm function calls.
  Depending on the rf status, this function sends a signal to let the hdrsrch
  task know that rf processing is complete and hdr srch processing can
  continue.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrsrchrf_rfm_cb
(
  rfm_cb_event_enum_type      rf_status,
    /* Status of rf enable transmitter */

  void *                      data
    /* Callback data */
) 
{

    /* Callback from rfm_xxx() function.
       Send a signal to the hdrsrch task rf task is complete.
       Overload the same signal. */
    ( void ) rex_set_sigs( HDRSRCH_TCB, HDRSRCH_RFM_SIG );

} /* hdrsrchrf_rfm_cb() */


/*===========================================================================

FUNCTION HDRSRCHRF_RFM_WAIT

DESCRIPTION
  This function waits for and clears the generic HDRSRCH_RFM_SIG signal.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrsrchrf_rfm_wait( void )
{
  (void) hdrsrch_wait_for_sigs( HDRSRCH_RFM_SIG );
    /* Wait for rf tune to complete */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_RFM_SIG );
    /* Clear the only signal being waited on */

} /* hdrsrchrf_rfm_wait */

/*===========================================================================

FUNCTION HDRSRCHRF_GET_CSRS_IN_USE

DESCRIPTION
  The function gets the CSRs in use based on the current channel config.
  The CSR info is stored in the primary chain only.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_get_csrs_in_use
(
  int32                     *csrs_in_use
    /* The csr list passed in */
)
{

  uint8    chan_cnt, csr_idx; 
    /* Loop index and csr index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Assign CSRs based on the channel list */

  for( chan_cnt=0;
       chan_cnt<hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel_cnt;
       chan_cnt++ )
  {
    csr_idx = hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.demod_idx[chan_cnt];
      /* The csr index is set to the same as the demod index */

    if( csr_idx < HDRSRCH_MAX_NUM_CARRIERS )
    {
      csrs_in_use[chan_cnt] = csr_idx;
        /* Store in return data structure */
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                      "Demod index (%d) out of range!",
                      csr_idx );
    }
  } /* for chan_cnt */
} /* hdrsrchrf_get_csrs_in_use */

/*===========================================================================

FUNCTION HDRSRCHRF_TUNE

DESCRIPTION
  This is a wrapper function for calls to rfm_hdr_retune(), waiting for the
  generic HDRSRCH_RFM_SIG signal.

DEPENDENCIES
  None.
  
RETURN VALUE
  If tune was successful.

SIDE EFFECTS
  None.

===========================================================================*/
int hdrsrchrf_tune
(
  hdrsrchrf_logical_chain_t *     rf_chain_ptr,
  const uint8                     num_band_chan,
  const sys_channel_type * const  band_chan
)
{
  rfm_hdr_agc_acq_type            agc_mode;
    /* AGC parameter to rfm_hdr_wakeup_rx() */

  int32                           csrs_in_use[HDRSRCH_MAX_NUM_CARRIERS];

  int                            ret_val;
    /* Return val from rf tune function */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_csrs_in_use( csrs_in_use );
    /* Get list of CSRs in use */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Enable state: %d", rf_chain_ptr->is_enabled);

  /* Enable RF if it is not enabled */  
  if ( rf_chain_ptr->is_enabled == FALSE )
  {
#ifdef FEATURE_HDR_RUMI
#ifdef FEATURE_HDR_ATLAS_MODEM
    out_dword(PBS_CTL_ADDR, 0x3020000);
    out_dword(PBS_CTL_ADDR, 0x3000000);

    switch ( HDRSRCH_RFLM_DTR_PBS_CFG )
    {
      case 0x0: /* Do nothing */
      break;

      case 0x1: /* RUMI IQ streaming */
        out_dword(PBS_CTL_ADDR, 0x30003FF);
        out_dword(PBS_CTL_ADDR, 0x30103FF);
      break;

      case 0x2: /* RUMI Y1Y2 streaming - bitexact */
        out_dword(PBS_CTL_ADDR, 0x3011000);
      break;

      case 0x3: /* RUMI Y1Y2 streaming - nonbitexact */
        out_dword(PBS_CTL_ADDR, 0x3010000);
      break;

      case 0x10:  // XXX TODO Update for CDP once settings are available
      case 0x11:  // XXX TODO Update for CDP once settings are available
      break;
      case 0x20: /* RUMI TX flex capture */
        out_dword(PBS_CTL_ADDR, 0x3010000);
      break;

       default:
         HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Unsupported rflm_dtr_pbs_cfg value: 0x%x", HDRSRCH_RFLM_DTR_PBS_CFG);
    }
#endif /* FEATURE_HDR_ATLAS_MODEM */
    if ( hdrhit_rumi_cfg.y1y2_support )
    {
#endif /* FEATURE_HDR_RUMI */
      HDR_ASSERT (rf_chain_ptr->rxlm_handle_valid == TRUE);
                        
      ret_val = rfm_hdr_enable( 
        rf_chain_ptr->rf_chain, /* path to enable the HDR subsystem on         */
        rf_chain_ptr->rxlm_handle,      
                              /* The Rx Link Manager buffer to config modem HW */
        rf_chain_ptr->tune.channel[0], /* Band class and channel info          */
        hdrsrchrf_rfm_cb,     /* Called when enable sequence is complete       */
        NULL );    /* Data handle to pass additional info to the cb function   */

      hdrsrchrf_rfm_wait();
        /* Wait for rf task to complete. */

      if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
      {
        /* Bad RF Cal */
        HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"Bad RF cal!!");

        return HDRSRCHRF_TUNE_ERROR;    
      }
      else if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }

      rf_chain_ptr->is_enabled = TRUE;
        /* Mark RF enabled, since we just enabled it. */

      /* Sanity check on is_tuned.  If we just enabled, it can't be tuned. */
      if (rf_chain_ptr->is_tuned == TRUE) 
      {
        HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"RF tuned but not enabled?");

        rf_chain_ptr->is_tuned = FALSE;
      }

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Enable state: %d", rf_chain_ptr->is_enabled);
    } /* if is_enabled == FALSE */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Tune state: %d", rf_chain_ptr->is_tuned);

    /* If RF is not tuned, use prep/exec. If already tuned, use retune API. */
    if ( rf_chain_ptr->is_tuned == FALSE )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Tune using AGC ACQ Mode: %d", hdrsrchrf.agc_acq_mode);

      agc_mode.mode = ( rfm_hdr_agc_mode_type ) hdrsrchrf.agc_acq_mode; 
      agc_mode.params.fast_acq.agc_rtc_offset = hdrsrchrf.agc_rtc_offset;
#ifdef FEATURE_HDR_BOLT_RUMI
      agc_mode.mode = agc_mode.mode;
#endif /* FEATURE_HDR_BOLT_RUMI */

      HDR_ASSERT ( num_band_chan <= HDRSRCH_MAX_NUM_SUBASETS ) ;
        /* KW fix : Check to limit num_band_chan to max */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
      hdrmultirat_confirm_valid_freq_id();
        /* Make sure we have valid fre ids, if not then atleast send valid chan id */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */


      ret_val = rfm_hdr_prep_wakeup_rx(
        rf_chain_ptr->rf_chain, /* receive path to wakeup                      */
        rf_chain_ptr->rxlm_handle,
                              /* The Rx Link Manager buffer to config modem HW */
        num_band_chan,              /* The length of the band_chan array.   */
        band_chan,                 /* Band class and channel info array    */
        csrs_in_use,                /* Carrier sample RAM index array       */
        &agc_mode,                  /* AGC mode                             */
        hdrsrchrf_rfm_cb,           /* Called when wakeup sequence is complete */
        NULL );    /* Data handle to pass additional info to the cb function   */

      hdrsrchrf_rfm_wait();
        /* Wait for rf task to complete. */

      if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
      {
        /* Bad RF Cal */
        HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"Bad RF cal!! ");

        return HDRSRCHRF_TUNE_ERROR;
      }
      else if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }
#ifdef FEATURE_HDR_RUMI
    } /* if hdrhit_rumi_cfg.y1y2 support */
#endif /* FEATURE_HDR_RUMI */

    hdrsrchrf.rf_exec_wakeup_rx_pending = TRUE;
  }
  else
  {
#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.y1y2_support )
    {
#endif /* FEATURE_HDR_RUMI */
    HDR_ASSERT (rf_chain_ptr->rxlm_handle_valid == TRUE);

    HDR_ASSERT ( num_band_chan <= HDRSRCH_MAX_NUM_SUBASETS ) ;
      /* KW fix : Check to limit num_band_chan to max */

    ret_val = rfm_hdr_retune( 
      rf_chain_ptr->rf_chain,   /* RF physical chain to tune                */
      rf_chain_ptr->rxlm_handle,
                                /* The Rx Link Manager buffer to config modem HW */
      (uint32) num_band_chan,   /* Entries in band-chan parameter array     */
      band_chan,                /* Array of band-chan for assigned carriers */
      csrs_in_use,              /* Sample RAM index array                   */
      hdrsrchrf_rfm_cb,         /* Function called once tuning is complete  */
      NULL);                    /* Data to pass to callback function        */
      /* Call rf tuning.  Always pass hdrsrchrf_rfm_cb() as the callback and NULL data passed back. */

    hdrsrchrf_rfm_wait();
      /* Wait for signal from callback complete. */
    if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
    {
      /* Bad RF Cal */
      HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"Bad RF cal!!");

      return HDRSRCHRF_TUNE_ERROR;   
    }
    else if (ret_val < RFM_CDMA_HEALTHY_STATE )
    {
      /* Error fatal */
      ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                ret_val, 0, 0);
    }
#ifdef FEATURE_HDR_RUMI
    } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

    rf_chain_ptr->is_tuned = TRUE;
      /* We just retuned, mark tuned. */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "ReTune state: %d", rf_chain_ptr->is_tuned);
  }

  return HDRSRCHRF_TUNE_OK;
} /* hdrsrchrf_tune */

/*============================================================================

FUNCTION HDRSRCHRF_RESET_RETAIN_LOCK_STATE

DESCRIPTION
  Changes the state of retain lock to false so that it is known whether
  the retain lock state should be reset by calls to retain lock. If 
  retain lock is true, another call to retain lock will not change the
  state of the retain lock state.  Otherwise, the state is reset so that
  the lock is defaulted to indefinitely held.
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

static void hdrsrchrf_reset_retain_lock_state
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();

  hdrsrchrf.chain[chain_id].rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk = 0;
    /* Start off assuming we have the lock indefinitely. */

  REX_ENABLE_PREMPTION();

  HDR_MSG_SRCH_3( MSG_LEGACY_MED,
      "ext_unlock: chain %d, ext_unlock %d wc %d", chain_id,
                  hdrsrchrf.chain[chain_id].rl_state.ext_unlock,
                  hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id );

} /* hdrsrchrf_reset_retain_lock_state */


/*============================================================================

FUNCTION HDRSRCHRF_SET_LOCK_STATE

DESCRIPTION
  This function initializes the chain and event type to use for next
  unlock request
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_set_lock_state
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  uint32 unlock_by_time
    /* Time by which chain needs to be unlocked by in sclk */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();

  hdrsrchrf.chain[chain_id].rl_state.event = HDRSRCHRF_UNLOCK_BY;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk = unlock_by_time;
    /* Start off assuming we have the lock indefinitely. */

  hdrsrchrf.chain[chain_id].rl_state.ext_unlock = FALSE;
    /* Reset source of the unlock as internal */

#ifdef FEATURE_HDR_QTA
  hdrsrchrf.chain[chain_id].rl_state.ta_info = TRM_LONG_TUNEAWAY;
    /* Start off assuming we have the lock indefinitely. */
#endif /* FEATURE_HDR_QTA */

  hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id =
                                                    TRM_MAX_CLIENTS;
    /* Save the winning client */

  REX_ENABLE_PREMPTION();

  HDR_MSG_SRCH_3( MSG_LEGACY_MED,
      "ext_unlock: chain %d, ext_unlock %d wc %d", chain_id,
                  hdrsrchrf.chain[chain_id].rl_state.ext_unlock,
                  hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id );
} /* hdrsrchrf_set_lock_state */


/*============================================================================

FUNCTION HDRSRCHRF_CALL_UNLOCK_CB

DESCRIPTION
  Calls the registered callback for chain_id with the information saved
  from the last TRM unlock callback.
    
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

static void hdrsrchrf_call_unlock_cb
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{

  hdrsrch_rf_unlock_cb_t                unlock_cb;
    /* The unlock callback for this given client. */

  hdrsrchrf_unlock_event_enum_t         event;
    /* The event being sent to the client */

  uint32                                unlock_by_sclk;
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Critical section around the unlock information since it is changed
     from other contexts since TRM is a service. */
  
  unlock_cb = hdrsrchrf.chain[chain_id].unlock_cb;
    /* Save the unlock callback.  No need for critical sections here since
       unlock_cb is only written before retain lock is called and after
       the chain is released. */

  REX_DISABLE_PREMPTION();

  event = hdrsrchrf.chain[chain_id].rl_state.event;
    /* Save off the unlock by event information. */

  unlock_by_sclk = hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk;
    /* Save off the unlock by information. */

  REX_ENABLE_PREMPTION();

  if ( unlock_cb != NULL)
  {
    unlock_cb( chain_id, event, unlock_by_sclk );
      /* Tell the holder of the RF chain the new unlock time. */
  }

} /* hdrsrchrf_call_unlock_cb */


/*===========================================================================

FUNCTION HDRSRCHRF_INIT_TUNE_TIMER

DESCRIPTION
   Define/Initialize the RF tune timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_init_tune_timer( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  timer_def( &hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.tune_timer, 
             NULL, HDRSRCH_TCB, HDRSRCH_TUNE_SIG, NULL, 0 );
    /* Define the primary chain's tune timer. Other independent chains
       should have timer definitions like this. */ 

  hdrsrchrf.tune_timer_defined = TRUE;
    /* Now it is defined. */

}  /* hdrsrchrf_init_tune_timer */


/*===========================================================================

FUNCTION HDRSRCHRF_INIT

DESCRIPTION
   Initializes the HDR SRCH RF module.  Should be called at least once before 
   using the module.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.tune_sig = HDRSRCH_TUNE_SIG;
    /* Save the tune signal dedicated to the primary logical chain. */

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxlm_handle_valid = FALSE;
  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rxlm_handle_valid = FALSE;
    /* Initialized valid flags for RxLM buffer. */

  hdrsrchrf.rf_exec_wakeup_rx_pending = FALSE;
    /* Init the exec wakeup pending to false. */

  hdrsrchrf_reset_retain_lock_state( HDRSRCHRF_PRI_CHAIN );
    /* If we were retaining the lock we aren't anymore. */

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
    /* Initialize AGC acquisition parameters to normal mode */

  msgr_init_hdr( &hdrsrchrf.fw_msg.div_cfg_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_DIVERSITY_CFG_CMD );
    /* Init diveristy config message */

  msgr_init_hdr( &hdrsrchrf.fw_msg.force_comb_mode_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_FORCE_COMB_MODE_CMD );
    /* Init diversity combining mode message */
      
} /* hdrsrchrf_init() */

/*===========================================================================

FUNCTION HDRSRCHRF_CHECK_AND_RELEASE_TX_CHAIN

DESCRIPTION
   This function checks and releases TX chain if the requested 
   resource is not for TX

DEPENDENCIES
  None

PARAMETERS
  resource - resource requested for TX chain

RETURN VALUE
  void

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchrf_check_and_release_tx_chain
( 
  hdrsrchrf_resource_enum_t tx_resource
)
{
  if ( ( tx_resource == HDRSRCHRF_INVALID_RESOURCE ) &&
       ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain ||
         hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].req_notify_pending ) )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                    "RxTx: rel non-Tx rsrc:%d own:%d pend:%d",
                    tx_resource, 
                    hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain,
                    hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].req_notify_pending );

    rex_clr_timer( &hdrsrchrf.rxtx_pending_timer );

    hdrsrchrf_release( HDRSRCHRF_TX_CHAIN );
    
    hdrsrchrf.rxtx_pending = FALSE;
  }
}

/*===========================================================================

FUNCTION HDRSRCHRF_EXTRACT_RXTX_RSRC

DESCRIPTION
   This function extracts a RxTx resource into Rx and Tx component.
   If the given resource is Rx or Tx only, the corresponding component
   will be set to INVALID_RESOURCE
   

DEPENDENCIES
  None

PARAMETERS
  resource - resource requested for TX chain
  rx_p     - pointer to Rx resource to be updated
  tx_p     - pointer to Tx resource to be updated

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchrf_extract_rxtx_rsrc
(
  hdrsrchrf_resource_enum_t rxtx,
  hdrsrchrf_resource_enum_t *rx_p,
  hdrsrchrf_resource_enum_t *tx_p
)
{
  hdrsrchrf_resource_enum_t rx = HDRSRCHRF_INVALID_RESOURCE;
  hdrsrchrf_resource_enum_t tx = HDRSRCHRF_INVALID_RESOURCE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( rxtx )
  {
  case HDRSRCHRF_RXTX_BEST:
    rx = HDRSRCHRF_RX_BEST;
    tx = HDRSRCHRF_TX_BEST;
    break;

  case HDRSRCHRF_RXTX_ANY:
    rx = HDRSRCHRF_RX_ANY;
    tx = HDRSRCHRF_TX_ANY;
    break;

  case HDRSRCHRF_RX_BEST:
  case HDRSRCHRF_RX_ANY:
  case HDRSRCHRF_RX_SECONDARY:
    rx = rxtx;
    break;

  case HDRSRCHRF_TX_BEST:
  case HDRSRCHRF_TX_ANY:
    tx = rxtx;
    break;

  default:
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "RxTx: unexpected rxtx resource:%d", rxtx );
  }

  if ( rx_p != NULL )
  {
    *rx_p = rx;
  }

  if ( tx_p != NULL )
  {
    *tx_p = tx;
  }
}

/*============================================================================

FUNCTION HDRSRCHRF_RESERVE_AT

DESCRIPTION
  Specifies that the caller needs the given RF resource at the given
  time, for the given duration, for the supplied reason.

  This would be used with the "hdrsrchrf_request()" function, below.
  
  Not RxTx compatable
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the caller currently holds an RF chain, the chain is released.

============================================================================*/

void hdrsrchrf_reserve_at
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_resource_enum_t             resource,
    /* The RF resource which is being requested */

  hdrsrchrf_time_t                      when,
    /* When the resource will be needed (sclks timestamp) */

  hdrsrchrf_duration_t                  duration,
    /* How long the resource will be needed for (in sclks) */

  hdrsrchrf_reason_enum_t               reason
    /* Why the resource is needed (used for priority decisions) */
)
{

  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

  trm_reserve_at_input_info             res_at_info;
    /* Input Information for reserve at */

  hdrsrchrf_resource_enum_t             tx_resource;
/*--------------------------------------------------------------------------*/

  memset(&res_at_info,0,sizeof(trm_reserve_at_input_info));

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[chain_id].owns_chain )
  {
     /* Request failure is an implicit release */
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DISABLING_RX );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrchrf_extract_rxtx_rsrc( resource, &resource, &tx_resource );

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                  "reserve_at chain:%d rx:%d tx:%d reason:%d", 
                  chain_id, resource, tx_resource, reason );

  if ( chain_id == HDRSRCHRF_PRI_CHAIN )
  {
    /* If we requested PRI chain and not for TX, release
       TX chain (if owned) */   
    hdrsrchrf_check_and_release_tx_chain( tx_resource );
  }

  freq_info_ptr = hdrsrchrf_get_trm_freq_info();
    /* Obtain band/chan information for TRM request. */

  hdrsrchrf_reset_rf_chain_state( chain_id );
    /* Reserve at will release the RF resources but we need to clean up
       our hdrsrchrf state for the RF chain. */

  /* ReserveAt doesn't support RxTx */
  res_at_info.client_id = TRM_HDR;
  res_at_info.duration = ( trm_duration_t ) duration;
  res_at_info.freq_info.num_bands = 1;
  res_at_info.freq_info.bands[0].band = freq_info_ptr->freq_info.band;
  res_at_info.reason = ( trm_reason_enum_t ) reason;
  res_at_info.sub_reason = hdrsrchrf_get_sub_reason(reason);
  res_at_info.resource = ( trm_resource_enum_t ) resource;
  res_at_info.wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
  res_at_info.when = ( trm_time_t ) when;
  /* Fill tuneaway preference as none. HDR as target tech doesn't have any
     preference for tune away type */
  res_at_info.ta_info = TRM_TUNEAWAY_NONE;

  trm_reserve_at( &res_at_info );
    /* Let TRM know about the request. */

  hdrsrchrf.chain[chain_id].reason = reason;
  hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);
    /* Remember the reason the lock is being held. */

} /* hdrsrchrf_reserve_at */


/*============================================================================

CALLBACK HDRSRCHRF_GRANT_CALLBACK

DESCRIPTION
  The trm rf grant callback function, used by the Transceiver manager
  to inform the client of transceiver management events.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_grant_callback
(
  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_grant_return_enum_type      grant,
    /* The grant being sent to the client */

  trm_request_tag_t               tag
    /* Identfying tag for this request/grant transaction */
)
{
  hdrsrchrf_logical_chain_enum_t  chain_id;
    /* The logical chain ID associated with the callback */

/*--------------------------------------------------------------------------*/

  NOTUSED( tag );

  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );
    /* Convert the client ID to the chain ID. */

  if ( grant == TRM_DENIAL )
  {
    HDR_MSG_SRCH(MSG_LEGACY_ERROR, 
                 "TRM has granted DENIAL. Performing System Lost");
    hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;
    hdrsrchutil_system_lost (HDRSRCH_SYSLOST_BAD_RF_CAL);
    return;
  }

  /* It might appear this state info should be protected by critical sections.
     But it isn't since HDR should not call any RF functions until the RF
     resources are granted. */
  hdrsrchrf.chain[chain_id].rf_chain =  hdrsrchrf_get_rf_device( client );
    /* Save the RF device for this client. */

  hdrsrchrf.chain[chain_id].owns_chain = TRUE;
    /* The client now owns the specified chain. */

  HDR_MSG_SRCH_2(MSG_LEGACY_MED, "TRM granted RF %d for HDR %d",
                 hdrsrchrf.chain[chain_id].rf_chain,
                 chain_id);

  hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;

  /* Make sure we get the chain we expected. */
  switch ( chain_id )
  {
    case HDRSRCHRF_PRI_CHAIN:
    {

      if ( hdrsrchrf.chain[chain_id].grant_cb != NULL)
      {
        hdrsrchrf.chain[chain_id].grant_cb( chain_id, TRUE );
          /* Tell the relevant client that the lock has been granted. */
      }

#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RX_ENABLED );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      break;
    }

    case HDRSRCHRF_DIV_CHAIN:
     {
       HDR_MSG_SRCH_2(MSG_LEGACY_MED, "TRM granted DIV RF client %d for event %d",
                 client, grant);
       hdrsrchrfdiv_grant_callback ( client, grant ,0);

       break;
     }
    default:
    {
      ERR_FATAL( "TRM grant sent to wrong client", 0, 0, 0 );

      break;
    }
  }
} /* hdrsrchrf_grant_callback */

/*============================================================================

CALLBACK HDRSRCHRF_RXTX_TIMEOUT_CB

DESCRIPTION
  The handles RxTx request timing out. It declares system lost if so.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void hdrsrchrf_rxtx_timeout_cb
( 
  uint32 timer_id 
)
{
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "RxTx: R&N timed out owns:%d/%d", 
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain, 
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT);
#endif

  hdrsrch_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
}

/*============================================================================

FUNCTION HDRSRCHRF_REQUEST_AND_NOTIFY

DESCRIPTION
  Specifies the given caller needs the given RF chain resource, for the
  given duration, for the supplied reason.

  When the resource can be granted to the caller, the event callback for the
  caller will be called with the result of the lock request.
  
  This function is RxTx compatable
  
DEPENDENCIES
  HDR must not call ANY RF functions related to the chain requested until the
  RF resources are granted after this call is made. Calling request_and_notify
  again from the exact same code path is OK, and will result in a no-op if
  previous Req&Notify is pending. 

RETURN VALUE
  None

SIDE EFFECTS
  If the caller currently holds an RF chain, the chain is released.

============================================================================*/

void hdrsrchrf_request_and_notify
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_resource_enum_t             resource,
    /* The RF resource which is being requested */

  hdrsrchrf_duration_t                  duration,
    /* How long the resource will be needed for (in sclks) */

  hdrsrchrf_reason_enum_t               reason,
    /* Why the resource is needed (used for priority decisions) */

  hdrsrchrf_grant_callback_t            grant_callback
    /* Callback to notify caller when resource is granted */
)
{

  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

  static trm_grouped_request_notify_input_info req_info;
    /* Input Structure for Request */

  hdrsrchrf_resource_enum_t             rx_resource, tx_resource;
    /* Request RF receive/transmit chain */

  static boolean                        def_timer = TRUE;
    /* Indicates if timer has been defined */

/*--------------------------------------------------------------------------*/

  memset( &req_info, 0, sizeof( req_info ) );
    /* Initialize the structure before passing to TRM */

  if ( def_timer )
  {
    def_timer = FALSE;
    rex_def_timer_ex( &hdrsrchrf.rxtx_pending_timer,
                       hdrsrchrf_rxtx_timeout_cb, 0);
  }

  hdrsrchrf_extract_rxtx_rsrc( resource, &rx_resource, &tx_resource );

  HDR_MSG_SRCH_6( MSG_LEGACY_MED, 
                  "R&N chain:%d rx:%d tx:%d reason:%d own?%d pend?%d", 
                  chain_id, rx_resource, tx_resource, reason,
                  hdrsrchrf.chain[chain_id].owns_chain,
                  hdrsrchrf.chain[chain_id].req_notify_pending );

  if ( chain_id == HDRSRCHRF_PRI_CHAIN )
  {
    /* If we requested PRI chain and not for TX, release
       TX chain (if owned) */
    hdrsrchrf_check_and_release_tx_chain( tx_resource );
  }

  if ( !hdrsrchrf.chain[chain_id].req_notify_pending )
  {
      /* First things first, lets make sure we don't enter this again from some
         other task context by marking R&N pending */
    hdrsrchrf.chain[chain_id].req_notify_pending = TRUE;

      /* Initialize the structure before passing to TRM */
    memset( &req_info, 0, sizeof( req_info ) );

      /* if no pending TRM req, pass to TRM and set pending flag */
    hdrsrchrf.chain[chain_id].grant_cb = 
      ( hdrsrchrf_grant_callback_t )grant_callback;
      /* Save the callback for this client. */

    if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
         hdrsrchrf.chain[chain_id].owns_chain )
    {
#ifdef FEATURE_HDR_SELECTIVE_TXD
       /* Request failure is an implicit release */
      hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DISABLING_RX );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
    }

    hdrsrchrf.chain[chain_id].owns_chain = FALSE;

    hdrsrchrf.chain[chain_id].rf_chain = RFM_INVALID_DEVICE;

    hdrsrchrf.chain[chain_id].unlock_cb = NULL;

    hdrsrchrf.chain[chain_id].reason = reason;
    /* Remember the reason the lock is being held. */

    hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);

    hdrsrchrf.chain[chain_id].resource = resource;
    /* Remember resource (not rx_resource) in case
       we need to send R&N again */

    hdrsrchrf.chain[chain_id].duration = duration;

    hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_NO_EVENT;    

    freq_info_ptr = hdrsrchrf_get_trm_freq_info();
      /* Obtain band/chan information for TRM request. */

    req_info.request_info[0].client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );

    /* The transceiver resource which is being requested */
    if ( chain_id == HDRSRCHRF_TX_CHAIN )
    {      
      req_info.request_info[0].resource = ( trm_resource_enum_t ) tx_resource;
    }
    else
    {
      req_info.request_info[0].resource = ( trm_resource_enum_t ) rx_resource;
    }   

    /* How long the resource will be needed for (in sclks) */
    req_info.request_info[0].duration = ( trm_duration_t ) duration;
    /* Why the resource is needed (used for priority decisions) */
    req_info.request_info[0].reason = ( trm_reason_enum_t ) reason;
    req_info.request_info[0].sub_reason = hdrsrchrf_get_sub_reason(reason);
    /* Band/s that would be used */
    req_info.request_info[0].freq_info.num_bands = 1;
    req_info.request_info[0].freq_info.bands[0].band = freq_info_ptr->freq_info.band;
    /* Unique Identifier to indicate the wakeup for which reservation is done */
    req_info.request_info[0].wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
  /* Fill tuneaway preference as none. HDR as target tech doesn't have any
     preference for tune away type */
    req_info.request_info[0].ta_info = TRM_TUNEAWAY_NONE;
   
    if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
         ( tx_resource != HDRSRCHRF_INVALID_RESOURCE ) )
    {
      /* if request is for PRI chain and has Tx component,
         and Tx isn't already owned or pending */
      (void) rex_set_timer( &hdrsrchrf.rxtx_pending_timer, 
                            HDRSRCHRF_RXTX_WAIT_MS );

      hdrsrchrf.rxtx_pending = TRUE;

      hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN] = hdrsrchrf.chain[chain_id];
      hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].resource = tx_resource;
     
      req_info.num_of_entries = 2;
      req_info.request_info[1] = req_info.request_info[0];
      req_info.request_info[1].client_id = TRM_HDR_TX;
      req_info.request_info[1].resource = ( trm_resource_enum_t ) tx_resource;
      /* Fill tuneaway preference as none. HDR as target tech doesn't have any
         preference for tune away type */
      req_info.request_info[1].ta_info = TRM_TUNEAWAY_NONE;
    }
    else
    {
      req_info.num_of_entries = 1;
    }
    
    trm_grouped_request_and_notify( &req_info );
      /* Nothing to do internally, just let TRM know about the reservation */
  }
  else if ( (hdrsrchrf.chain[chain_id].grant_cb != NULL) &&
            (hdrsrchrf.chain[chain_id].grant_cb != grant_callback) )
  {
    /* if Req pending, and callback has changed, log possible issue */
    HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "R&N cb %x!=%x",
                   hdrsrchrf.chain[chain_id].grant_cb, grant_callback);
  }

#ifdef FEATURE_HDR_RUMI
  if ( hdrhit_rumi_cfg.force_trm_grant )
  {
    hdrsrchrf_grant_callback( HDRSRCHRF_CHAIN_TO_TRM( chain_id ), 
                              TRM_GRANTED,
                              0 );
  }
#endif /* FEATURE_HDR_RUMI */
} /*  hdrsrchrf_request_and_notify */


/*============================================================================

FUNCTION HDRSRCHRF_REQUEST

DESCRIPTION
  Specifies that the RF resource is needed, for how long, and for the supplied
  reason.

  The resource request is immediately evaluated, and the result returned.

  This may be used in conjunction with hdrsrchrf_reserve_at().
  
  This function is RxTx compatable
  
DEPENDENCIES
  None

RETURN VALUE
  Whether the caller was granted the appropriate resources.

SIDE EFFECTS
  If the caller currently holds an RF chain, that chain is released before
  the request is evaluated.

============================================================================*/

boolean hdrsrchrf_request
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_resource_enum_t             resource,
    /* The RF chain which is being requested */

  hdrsrchrf_duration_t                  duration,
    /* How long the resource will be needed for (in sclks) */

  hdrsrchrf_reason_enum_t               reason
    /* Why the resource is needed (used for priority decisions) */
)
{

  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

  static trm_grouped_request_input_info req_info;
    /* Input Structure for Request */
  
  static trm_grouped_request_return_data ret_data;
    /* Output Structure from Request Processing */

  hdrsrchrf_resource_enum_t             rx_resource, tx_resource;

  boolean                               all_granted = TRUE;

  uint8                                 i;
/*--------------------------------------------------------------------------*/

  memset( &req_info, 0, sizeof( req_info ) );
  memset( &ret_data, 0, sizeof( ret_data ) );
    /* Initialize the structures before passing to TRM */

  hdrsrchrf_extract_rxtx_rsrc( resource, &rx_resource, &tx_resource );

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                  "request chain:%d rx:%d tx:%d reason:%d", 
                  chain_id, rx_resource, tx_resource, reason );

  if ( chain_id == HDRSRCHRF_PRI_CHAIN )
  {
    /* If we requested PRI chain and not for TX, release
       TX chain (if owned) */
    hdrsrchrf_check_and_release_tx_chain( tx_resource );
  }

  if ( hdrsrchrf.chain[chain_id].req_notify_pending )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Req while R&N pending %x",
                   hdrsrchrf.chain[chain_id].grant_cb);
  }  

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[chain_id].owns_chain )
  {
     /* Request failure is an implicit release */
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DISABLING_RX );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrchrf.chain[chain_id].owns_chain = FALSE;
  hdrsrchrf.chain[chain_id].rf_chain = RFM_INVALID_DEVICE;
  hdrsrchrf.chain[chain_id].unlock_cb = NULL;
  hdrsrchrf.chain[chain_id].reason = reason;
  /* Remember the reason the lock is being held. */
  hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);
  hdrsrchrf.chain[chain_id].resource = resource;
  hdrsrchrf.chain[chain_id].duration = duration;
  hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;
  hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_NO_EVENT;   

  freq_info_ptr = hdrsrchrf_get_trm_freq_info();
    /* Obtain band/chan information for TRM request. */

  /* Let's ask TRM if we can have the resource. */

  req_info.request_info[0].client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  req_info.request_info[0].duration = duration;
  req_info.request_info[0].freq_info.num_bands = 1;
  req_info.request_info[0].freq_info.bands[0].band = freq_info_ptr->freq_info.band;
  req_info.request_info[0].reason = ( trm_reason_enum_t ) reason;
  req_info.request_info[0].sub_reason = hdrsrchrf_get_sub_reason(reason);
  /* Fill tuneaway preference as none. HDR as target tech doesn't have any
     preference for tune away type */
  req_info.request_info[0].ta_info = TRM_TUNEAWAY_NONE;

  if ( chain_id == HDRSRCHRF_TX_CHAIN )
  {
    req_info.request_info[0].resource = ( trm_resource_enum_t ) tx_resource;
  }
  else
  {
    req_info.request_info[0].resource = ( trm_resource_enum_t ) rx_resource;
  }
  
  req_info.request_info[0].wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
   /* Unique Identifier to indicate the wakeup for which reservation is done */

  if ( ( chain_id != HDRSRCHRF_TX_CHAIN ) &&
       ( tx_resource != HDRSRCHRF_INVALID_RESOURCE ) )
  {
    /* if request wasn't for TX directly, but has Tx component,
       and TX isn't already owned */
    hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN] = hdrsrchrf.chain[chain_id];
    hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].resource = tx_resource;

    req_info.num_of_entries = 2;
    req_info.request_info[1] = req_info.request_info[0];
    req_info.request_info[1].client_id = TRM_HDR_TX;
    req_info.request_info[1].resource = ( trm_resource_enum_t) tx_resource;
    /* Fill tuneaway preference as none. HDR as target tech doesn't have any
       preference for tune away type */
    req_info.request_info[1].ta_info = TRM_TUNEAWAY_NONE;
  }
  else
  {
    req_info.num_of_entries = 1;
  }
  trm_grouped_request ( &req_info, &ret_data );
  /* TODO check for band compatibility here */

  if ( chain_id != HDRSRCHRF_DIV_CHAIN  )
  {
    hdrsrchrf.rxtx_pending = FALSE;
  }

  /* Check if all chains are granted */
  for (i=0; i<ret_data.num_of_entries; i++)
  {
#ifdef FEATURE_HDR_RUMI
    if ( ( ret_data.return_info[i].grant != TRM_GRANTED ) &&
       ( hdrhit_rumi_cfg.force_trm_grant ) )
  {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                      "RUMI: TRM returned failure %d for client %d Forcing to granted", 
                      ret_data.return_info[i].grant,
                      ret_data.return_info[i].client_id );
      ret_data.return_info[i].grant = TRM_GRANTED;
  }
#endif /* FEATURE_HDR_RUMI */

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "request chain:%d granted:%d", 
                    HDRSRCHRF_CHAIN_FROM_TRM( ret_data.return_info[i].client_id ),
                    ret_data.return_info[i].grant );

    all_granted &= ( ret_data.return_info[i].grant == TRM_GRANTED );
  }

  if ( all_granted )
  {
    /* All chain granted, go thru each chain and update info */
    for (i=0; i<ret_data.num_of_entries; i++)
    {   
      chain_id = HDRSRCHRF_CHAIN_FROM_TRM(ret_data.return_info[i].client_id);

      hdrsrchrf.chain[chain_id].owns_chain = TRUE;

      hdrsrchrf.chain[chain_id].rf_chain = 
        hdrsrchrf_get_rf_device( ret_data.return_info[i].client_id );

      hdrsrchrf.chain[chain_id].reason = reason;
      hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);  

#ifdef FEATURE_HDR_SELECTIVE_TXD
      if ( chain_id == HDRSRCHRF_PRI_CHAIN )
      {
        hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RX_ENABLED );
      }    
#endif /* FEATURE_HDR_SELECTIVE_TXD */
    }
  }
  else
  {
    /* Not all chains granted, release the ones granted and
       return failure */
    for (i=0; i<ret_data.num_of_entries; i++)
    {      
      if ( ret_data.return_info[i].grant == TRM_GRANTED )
      {
        chain_id = HDRSRCHRF_CHAIN_FROM_TRM( ret_data.return_info[i].client_id );

        hdrsrchrf_release( chain_id );
      }
    }
  }

  return all_granted;
} /*  hdrsrchrf_request */

/*============================================================================

FUNCTION HDRSRCHRF_EXCHANGE

DESCRIPTION
  Exchanges the RF resource with home RAT during resel/redirection/HO
  scenarios in IRAT. If the TRM API succeeds HDR owns the chain
  immediately. Otherwise HDR will attempt legacy request_&_notify as
  this may be single RAT scenario where exchange mechanism is not used.
  The exchange mechanism is used only when more than 2 RATs are active
  like in 1xSRLTE, 1xSRLTE+G etc
  
  Not RxTx compatable
  
DEPENDENCIES
  Home RAT LTE to have enabled exchange for multiRAT scenario

RETURN VALUE
  Whether the caller was granted the appropriate resources. TRUE if
  the exchange succeeded. FALSE otherwise.

SIDE EFFECTS
  None
============================================================================*/

boolean hdrsrchrf_exchange
(
  hdrsrchrf_duration_t                  duration,
    /* How long the resource will be needed for (in sclks) */

  hdrsrchrf_reason_enum_t               reason
    /* Why the resource is needed (used for priority decisions) */
)
{
  hdrsrchrf_logical_chain_enum_t        chain_id = HDRSRCHRF_PRI_CHAIN;
    /* The logical chain ID associated with the request */
    
  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

  trm_client_exchange_input_info        exchange_info;
    /* Input to pass to TRM for TRM exchange */

  trm_request_input_info                req_info;
    /* Input Structure for Request */
  
  rfm_device_enum_type                  rf_dev;
    /* RF device received by HDR through exchange */
/*--------------------------------------------------------------------------*/

  chain_id = HDRSRCHRF_PRI_CHAIN;
    /* TRM exchange is always with primary chain */

  HDR_MSG_SRCH_3(MSG_LEGACY_HIGH,
                 "exchange %d %d %d",
                 chain_id,
                 hdrsrchrf.chain[chain_id].owns_chain,
                 hdrsrchrf.chain[chain_id].lock_grantor);

  if ( hdrsrchrf.chain[chain_id].req_notify_pending )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Exchange req while R&N pending %x",
                   hdrsrchrf.chain[chain_id].grant_cb);
    return FALSE;
  }

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[chain_id].owns_chain )
  {
    /* Request failure is an implicit release */
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DISABLING_RX );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  if( hdrsrchrf.chain[chain_id].owns_chain == TRUE )
  {
    /* HDR owns the specified chain. */

    HDR_MSG_SRCH(MSG_LEGACY_HIGH, "HDR owns the chain");
     
    if( hdrsrchrf.chain[chain_id].lock_grantor == HDRSRCHRF_IRAT)
    {
      hdrsrchrf.chain[chain_id].lock_grantor = HDRSRCHRF_NONE;
        /* HDR has released the lock */
        
      hdrsrchrf.chain[chain_id].owns_chain = FALSE;
        /* The client no more owns the specified chain. */

      exchange_info.client_1 = TRM_IRAT;
      if ( hdrsrch_get_subscriber_id ( ) == SYS_MODEM_AS_ID_2 )
      {
        exchange_info.client_1 = TRM_IRAT2;
      }
        /* The default is TRM_IRAT, use TRM_IRAT2 if HDR is subscriber 2 */

      exchange_info.client_2 = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
      HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, 
                     "HDR releases chain,exchange client1=%d,exchange client2=%d",
                     exchange_info.client_1,
                     exchange_info.client_2
                    );
      trm_client_exchange( &exchange_info );
      
      return TRUE;
    }
    return FALSE;
  }
  else
  if( duration != 0 )
  {  
    hdrsrchrf.chain[chain_id].lock_grantor = HDRSRCHRF_NONE; 
    /* Initialize the HDR lock grantor state */

    exchange_info.client_1 = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
    exchange_info.client_2 = TRM_IRAT;
    if ( hdrsrch_get_subscriber_id ( ) == SYS_MODEM_AS_ID_2 )
    {
      exchange_info.client_2 = TRM_IRAT2;
    }
      /* The default is TRM_IRAT, use TRM_IRAT2 if HDR is subscriber 2 */

    HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, 
                   "HDR about to exchange chain,exchange client1=%d,exchange client2=%d",
                   exchange_info.client_1,
                   exchange_info.client_2
                  );
    trm_client_exchange( &exchange_info );

    rf_dev = hdrsrchrf_get_rf_device ( HDRSRCHRF_CHAIN_TO_TRM( chain_id ) );
    HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "HDR recv RF dev %d", rf_dev);
    if (rf_dev == RFM_MAX_DEVICES)
    {
      HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Exchange req denied %x",
                     chain_id);
      return FALSE;
    }
    HDR_MSG_SRCH_1(MSG_LEGACY_LOW, "HDR exchanged chain %d", rf_dev);

    hdrsrchrf.chain[chain_id].lock_grantor = HDRSRCHRF_IRAT;
      /* Save the lock provider for this client. */

    hdrsrchrf.chain[chain_id].rf_chain = 
      hdrsrchrf_get_rf_device( HDRSRCHRF_CHAIN_TO_TRM( chain_id ) );
      /* Save the RF device for this client. */

    hdrsrchrf.chain[chain_id].owns_chain = TRUE;
      /* The client now owns the specified chain. */

    hdrsrchrf.chain[chain_id].reason = reason;
      /* Remember the reason the lock is being held. */

    hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason( reason );

#ifdef FEATURE_HDR_SELECTIVE_TXD
      if ( chain_id == HDRSRCHRF_PRI_CHAIN )
      {
        hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_RX_ENABLED );
      }    
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      HDR_MSG_SRCH_3(MSG_LEGACY_HIGH,
                     "exchange %d %d %d",
                     chain_id,
                     hdrsrchrf.chain[chain_id].owns_chain,
                     hdrsrchrf.chain[chain_id].lock_grantor);
      return TRUE;
  }
  else
  {
    HDR_MSG_SRCH(MSG_LEGACY_HIGH, "HDR does not own chain");
    return FALSE;
  }
} /*  hdrsrchrf_exchange */


/*============================================================================

FUNCTION HDRSRCHRF_RELEASE

DESCRIPTION
  Release the RF chain resource currently held by a caller.  This API
  should not be called with the HDRSRCHRF_DIV_CHAIN parameter.  Instead
  hdrsrchrf_release_diversity should be called. 
  
  RxTx compatable
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If no client is waiting for the resource, the RF chain will be turned off.

============================================================================*/

void hdrsrchrf_release
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{

  static trm_grouped_release_input_type  input_info;
    /* Input for trm release */
/*--------------------------------------------------------------------------*/

  memset( &input_info, 0, sizeof( input_info ) );
    /* Initialize the structure before passing to TRM */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[chain_id].owns_chain )
  {
     /* Request failure is an implicit release */
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_DISABLING_RX );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */


  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Rel chain:%d grantor:%d reason:%d",
                  chain_id,
                  hdrsrchrf.chain[chain_id].lock_grantor,
                  hdrsrchrf.chain[chain_id].reason );

  input_info.rel_info[0].client = HDRSRCHRF_CHAIN_TO_TRM( chain_id );  

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain ||
         hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].req_notify_pending ) )
  {
    input_info.num_of_entries = 2;
    input_info.rel_info[1].client = TRM_HDR_TX;  
  }
  else
  {
    input_info.num_of_entries = 1;
  }

  /* Clear pending flag and timer when cancelling the RF request */
  if ( ( chain_id != HDRSRCHRF_DIV_CHAIN  ) 
    &&  ( hdrsrchrf.rxtx_pending == TRUE ) )
  {
    hdrsrchrf.rxtx_pending = FALSE;

    rex_clr_timer( &hdrsrchrf.rxtx_pending_timer );
  }
    
  trm_grouped_release( &input_info ); 
    /* Pass this request on to TRM. */    
    
  hdrsrchrf_reset_rf_chain_state( chain_id );
    /* Reserve at will release the RF resources but we need to clean up
       our hdrsrchrf state for the RF chain. */

  if ( input_info.num_of_entries == 2 )
  {
    hdrsrchrf_reset_rf_chain_state( HDRSRCHRF_TX_CHAIN );
  }
} /* hdrsrchrf_release */

/*============================================================================

FUNCTION HDRSRCHRF_UPDATE_PRIORITY

DESCRIPTION
  When a chain is used for a different reason, it should change the advertised
  reason for holding the RF chain resource, so its priority will change.

  Eg) A client request the RF chain resource for listening for a PAGE.  If
  it receives one, it would change its priority to PAGE_RESPONSE and attempt
  to respond to the page, and eventually change its priority to TRAFFIC.
  If HDR was using IRAT inherited priority, now it will switch back to
  its native priority. Hence this function should be called only for IRAT
  scenarios.
  
  RxTx compatable
  
DEPENDENCIES
  The client must be holding a RF chain resource lock

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_update_priority
(
  hdrsrchrf_reason_enum_t               reason
    /* Why the resource is needed (used for priority decisions) */
)
{
  hdrsrchrf_logical_chain_enum_t        chain_id = HDRSRCHRF_PRI_CHAIN;
    /* The logical chain ID associated with the request */

/*--------------------------------------------------------------------------*/

  chain_id = HDRSRCHRF_PRI_CHAIN;
  hdrsrchrf.chain[chain_id].reason = reason;
  hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);  
  /* Now that redir is complete go back to normal priority */

  HDR_MSG_SRCH_4 (MSG_LEGACY_LOW, "Upd Prio 1 %d %d %d %d",
                  hdrsrchrf.chain[chain_id].lock_grantor,
                  chain_id,
                  hdrsrchrf.chain[chain_id].owns_chain,
                  hdrsrchrf.chain[chain_id].reason);
  
  if( ( hdrsrchrf.chain[chain_id].lock_grantor == HDRSRCHRF_IRAT ) &&
      (hdrsrchrf.chain[chain_id].owns_chain == TRUE ) )
  {
    hdrsrchrf.chain[chain_id].lock_grantor = HDRSRCHRF_NONE;
    /* Restore the lock provider status for this client. */

    HDR_MSG_SRCH_4 (MSG_LEGACY_LOW, "Upd Prio 2 %d %d %d %d",
        hdrsrchrf.chain[chain_id].lock_grantor,
        chain_id,
        hdrsrchrf.chain[chain_id].rf_chain,
        hdrsrchrf.chain[chain_id].reason);
    hdrsrchrf_change_priority (chain_id, hdrsrchrf.chain[chain_id].reason);
      /* Pass this request to TRM finally */
  }
} /*  hdrsrchrf_update_priority */


/*============================================================================

CALLBACK HDRSRCHRF_UNLOCK_CALLBACK

DESCRIPTION
  The prototype for unlock event callback functions, used by the Transceiver
  Resource Manager to inform the clients of when it should unlock a resource
  it holds.
  
DEPENDENCIES
  The callback will be called by the Transceiver Manager.  It may be called
  from a task context of another client, or from interrupt context.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_unlock_callback
(
  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_unlock_event_enum_t         event,
    /* The event being sent to the client */

  uint32                          unlock_by_sclk,
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */

  trm_client_info_t               winning_client,
    /* Winning Client */

  trm_tuneaway_enum_type         ta_info
    /* TA status (QTA or LTA) */
)
{

  hdrsrchrf_logical_chain_enum_t  chain_id;
    /* The logical chain ID associated with the callback */

  hdrsrch_rf_unlock_cb_t          unlock_cb;
    /* The unlock callback for this given client. */

  rfm_device_enum_type rf_dev;
    /* RF device currently held by HDR */
/*--------------------------------------------------------------------------*/

#ifdef FEATURE_HDR_DISABLE_ACCESS_TA_G_W
  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );

  if ( (hdrsrch_state_id == HDRSRCH_IDLE_STATE)  
       && hdrsrchidle_is_in_access()
       && ( (winning_client.client_id == TRM_GSM1)
            ||(winning_client.client_id == TRM_GSM2)
            ||(winning_client.client_id == TRM_GSM3)
            ||(winning_client.client_id == TRM_UMTS)
            ||(winning_client.client_id == TRM_UMTS2)
            ||(winning_client.client_id == TRM_LTE )
            ||(winning_client.client_id == TRM_LTE_SUB2 ) )
       && (event != TRM_UNLOCK_IMMEDIATELY)
       && ( (chain_id == HDRSRCHRF_PRI_CHAIN) || 
            (chain_id == HDRSRCHRF_TX_CHAIN) ) )
  {
    /* GSM/UMTS/LTE page will hog RF chain and hence ignore TA */
    HDR_MSG_SRCH_3 (MSG_LEGACY_HIGH, 
                    "Ignoring current TA for winning client %d in Access state. evt %d chain_id %d",
                     winning_client.client_id, event, chain_id);

    hdrsrchrf.pri_chain_unlock_ignored = TRUE;
      /* set that primary chain UNLOCK is ignored */

    return;
  }
  else
  {
    if ( chain_id == HDRSRCHRF_PRI_CHAIN)
    {
      hdrsrchrf.pri_chain_unlock_ignored = FALSE;
        /* flag to indicate that primary chain UNLOCK is ignored */
    }
  }
#else
  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );
#endif /* FEATURE_HDR_DISABLE_ACCESS_TA_G_W */

    /* Convert the client ID to the chain ID. */

  rf_dev = hdrsrchrf_get_rf_dev(chain_id);
  HDR_MSG_SRCH_4( MSG_LEGACY_LOW, 
                   "unlock event:%d on chain:%d => rf_dev %d cb:%d", 
                   event, chain_id, rf_dev,
                   hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].unlock_cb );

  /* Make sure we get the chain we expected. */
  switch ( chain_id )
  {
    case HDRSRCHRF_PRI_CHAIN:
    case HDRSRCHRF_TX_CHAIN:
    {
      unlock_cb = hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].unlock_cb;
        /* Save the unlock callback. */

      if ( unlock_cb != NULL )
      {
        REX_DISABLE_PREMPTION();

        hdrsrchrf.chain[chain_id].rl_state.event = 
                                        ( hdrsrchrf_unlock_event_enum_t ) event;
          /* Save off the unlock by event information. */

        hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk = unlock_by_sclk;
          /* Save off the unlock by information. */

        hdrsrchrf.chain[chain_id].rl_state.ext_unlock = TRUE;
          /* Save off the unlock by information. */

        hdrsrchrf.chain[chain_id].rl_state.winning_client = winning_client;
          /* Save the winning client */

#ifdef FEATURE_HDR_QTA
        hdrsrchrf.chain[chain_id].rl_state.ta_info = ta_info;
          /* Save the winning client */          
#endif /* FEATURE_HDR_QTA */

        REX_ENABLE_PREMPTION();

        HDR_MSG_SRCH_6( MSG_LEGACY_MED,
                        "unlock event:%d chain:%d ext_unlock: %d wc %d rf_dev %d cb:%d",event,
                        chain_id, 
                        hdrsrchrf.chain[chain_id].rl_state.ext_unlock,
                        hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id,
                        rf_dev,
                        hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].unlock_cb);


        unlock_cb( HDRSRCHRF_PRI_CHAIN, 
                   ( hdrsrchrf_unlock_event_enum_t ) event,
                   unlock_by_sclk );
          /* Invoke PRI chain cb for TX chain.
             Tell the holder of the RF chain the new unlock time. */
      }
      else
      {
	  HDR_MSG_SRCH_5(
		  MSG_LEGACY_MED, 
		  "TA disabled unlock event:%d UBSclk %d WClnt %d rf_dev %d cb:%d",
		  event,			
		  hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk,
		  hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id,
		  rf_dev,
		  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].unlock_cb);
      }

      break;
    }

    case HDRSRCHRF_DIV_CHAIN:
    {
     HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                   "unlock event:%d on chain:%d => rf_dev %d cb:%d", 
                   event, chain_id, rf_dev,
                   hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].unlock_cb );
	 
      REX_DISABLE_PREMPTION();

      hdrsrchrf.chain[chain_id].rl_state.event = 
                                      ( hdrsrchrf_unlock_event_enum_t ) event;
        /* Save off the unlock by event information. */

      hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk = unlock_by_sclk;
        /* Save off the unlock by information. */

      hdrsrchrf.chain[chain_id].rl_state.winning_client = winning_client;
        /* Save the winning client */


      REX_ENABLE_PREMPTION();

      hdrsrchrfdiv_release_div();
        /* Send the command to the RF Diversity State Machine */


      break;
    }

    default:
    {
    
      ERR_FATAL( "TRM grant sent to wrong client", 0, 0, 0 );

      break;
    }
  }
} /*  hdrsrchrf_unlock_callback_t */

/*============================================================================

CALLBACK HDRSRCHRF_RXTX_UNLOCK_CB

DESCRIPTION
  The is a dummy callback used when RxTx R&N is pending. The callback
  is needed for retain lock, but is not expected to be called, because
  hdrsrchrf_rxtx_unlock_callback() will handle the unlock and should
  not pass it down.
  
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void hdrsrchrf_rxtx_unlock_cb
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_unlock_event_enum_t         event,
    /* The event being sent to the client */

  uint32                                unlock_by_sclk
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */
)
{
  HDR_MSG_SRCH_4( MSG_LEGACY_ERROR, 
                  "RxTx cb: unexpected unlock:%d chain:%d while RX or TX pending:%d/%d", 
                  event, chain_id, 
                  !hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain,
                  !hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );  
}

/*============================================================================

CALLBACK HDRSRCHRF_RXTX_UNLOCK_CALLBACK

DESCRIPTION
  This function handles unlock event when RxTx request and notify is
  pending. It only handles UNLOCK_IMMEDIATE and UNLOCK_REQUIRED. UNLOCK_BY
  is ignored.
  
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
boolean hdrsrchrf_rxtx_unlock_callback
(
  trm_client_enum_t                     client,
    /* The logical chain ID associated with the request */

  trm_unlock_event_enum_t               event,
    /* The event being sent to the client */

  uint32                                unlock_by_sclk,
    /* Sclk timestamp for HDRSRCHRF_UNLOCK_BY */

  trm_client_info_t                     winning_client,
    /* Winning client from TRM */

  trm_tuneaway_enum_type               ta_info
      /* TA status (QTA or LTA) */
)
{
  hdrsrchrf_logical_chain_enum_t chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );

  if ( hdrsrchrf.rxtx_pending &&
       ( chain_id != HDRSRCHRF_DIV_CHAIN ) )
  {
    /* RXTX R&N pending and received unlock, regardless of chain_id,
       call R&N for RXTX again to release the partial grant and put in
       a new request */
    HDR_MSG_SRCH_4( MSG_LEGACY_MED,
                    "RxTx cb: chain:%d unlock:%d owns:%d/%d",
                    chain_id, event,
                    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain,
                    hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );

    /* Only honor unlock required/unlock immediate if RxTx pending */
    if ( ( event == HDRSRCHRF_UNLOCK_REQUIRED ) ||
         ( event == HDRSRCHRF_UNLOCK_IMMEDIATELY ) )
    {    
      hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_UNLOCK_REQUESTED;
      return TRUE;
    }    
  }
  else 
  {
    /* Remember unlock happened */
    hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_UNLOCK_REQUESTED;

    hdrsrchrf_unlock_callback( client, 
                               event,
                               unlock_by_sclk,
                               winning_client,
                               ta_info
                                 /* TA status (QTA or LTA) */
                              );
  }

  return FALSE;
}

/*============================================================================

FUNCTION HDRSRCHRF_RETAIN_LOCK

DESCRIPTION
  Informs the HDR Search RF Manager that the caller wants to hold
  the resource indefinitely.  The HDRSRCHRF may inform the caller that it must
  give up the lock through the supplied unlock callback.
  
DEPENDENCIES
  The client must be holding a RF chain resource lock

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_retain_lock
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_unlock_callback_t           unlock_callback
    /* The required sclk extension, from "now" */
)
{

  static trm_grouped_retain_lock_input_info    retain_lock_info;
    /* Input to retain_lock */
/*--------------------------------------------------------------------------*/

  memset( &retain_lock_info, 0, sizeof( retain_lock_info ) );
    /* Initialize the structure before passing to TRM */

  hdrsrchrf.chain[chain_id].unlock_cb = unlock_callback;
    /* Save the callback for this client. */

  hdrsrchrf_reset_retain_lock_state( chain_id );
    /* Clear out the retain lock state. */

  hdrsrchrf.chain[chain_id].rl_state.ext_unlock = FALSE;
    /* Reset source of the unlock as internal */

  hdrsrchrf.chain[chain_id].rl_state.winning_client.client_id =
                                                    TRM_MAX_CLIENTS;
    /* Save the winning client */

  retain_lock_info.num_of_entries = 1;
  retain_lock_info.retain_info[0].client = HDRSRCHRF_CHAIN_TO_TRM(chain_id);

  if ( chain_id == HDRSRCHRF_PRI_CHAIN )
  {
    hdrsrchrf.pri_chain_unlock_ignored = FALSE;

    if ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
    {
      hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].unlock_cb = unlock_callback;
      hdrsrchrf_reset_retain_lock_state( HDRSRCHRF_TX_CHAIN );

      retain_lock_info.num_of_entries = 2;
      retain_lock_info.retain_info[1].client = TRM_HDR_TX;
    }
  }

  trm_grouped_retain_lock( &retain_lock_info );
    /* Pass this request on to TRM. */

  /* reset unlock ignored flag */

  if ( chain_id != HDRSRCHRF_TX_CHAIN )
  {
    hdrsrchrf_call_unlock_cb( chain_id );
    /* The lock is already retained so lets just simulate a call to 
       TRM by forcing the unlock_callback to be called. If we just
       called trm_retain_lock it may not send the callback if nothing
       had changed since the last callback.  But in this case, we want
       the caller to see the state of the lock as it would if it was
       the initial call to trm_retain_lock. */
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                  "retain lock chain:%d tx:%d unlock_cb:%x", 
                  chain_id, 
                  ( retain_lock_info.num_of_entries == 2 ),
                  unlock_callback );

} /*  hdrsrchrf_retain_lock */


/*============================================================================

FUNCTION HDRSRCHRF_GET_RETAIN_LOCK_STATE

DESCRIPTION
  Retrieves the information related to the current retain lock instance. This
  information is related to when the lock is supposed to be released.
    
DEPENDENCIES
  The client must be holding a RF chain resource lock and have called
  retain lock.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_get_retain_lock_state
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_retain_lock_state_t*        rx
    /* How much time is left on the retain lock state. */
)
{
  hdrsrchrf_retain_lock_state_t         tx;
/*--------------------------------------------------------------------------*/

  REX_DISABLE_PREMPTION();

  rx->event = hdrsrchrf.chain[chain_id].rl_state.event;
    /* Return the unlock by event information. */

  rx->unlock_by_sclk = hdrsrchrf.chain[chain_id].rl_state.unlock_by_sclk;
    /* Return the unlock by information. */

  rx->ext_unlock = hdrsrchrf.chain[chain_id].rl_state.ext_unlock;
    /* Return source of the unlock as internal */

#ifdef FEATURE_HDR_QTA
  rx->winning_client = hdrsrchrf.chain[chain_id].rl_state.winning_client;

  rx->ta_info = hdrsrchrf.chain[chain_id].rl_state.ta_info;
#endif /* FEATURE_HDR_QTA */

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain ) )
  {
    hdrsrchrf_get_retain_lock_state( HDRSRCHRF_TX_CHAIN, 
                                     &tx );
#ifdef FEATURE_HDR_QTA
    HDR_MSG_SRCH_8( MSG_LEGACY_MED, 
                    "RxTx: rl state evt:%d/%d unlock_by:%d/%d winning:%d/%d/%d chain:%d ext_unlock: %d", 
                    rx->event, tx.event,
                    rx->unlock_by_sclk, 
                    tx.unlock_by_sclk,
                    rx->winning_client.client_id, 
                    tx.winning_client.client_id,
                    chain_id,
                    rx->ext_unlock);

#else
    HDR_MSG_SRCH_6( MSG_LEGACY_MED, 
                    "RxTx: rl state evt:%d/%d unlock_by:%d/%d chain:%d ext_unlock: %d", 
                    rx->event, tx.event,
                    rx->unlock_by_sclk, 
                    tx.unlock_by_sclk,
                    chain_id,
                    rx->ext_unlock);

#endif /* FEATURE_HDR_QTA */

    if ( tx.event > rx->event )
    {
      /* Tx unlock event is more urgent than rx, use tx */
      rx->event = tx.event;
      rx->unlock_by_sclk = tx.unlock_by_sclk;
#ifdef FEATURE_HDR_QTA
      rx->winning_client = tx.winning_client;
#endif /* FEATURE_HDR_QTA */
    }
    else if ( ( tx.event == rx->event ) &&
              ( tx.event == TRM_UNLOCK_BY ) &&
              ( tx.unlock_by_sclk < rx->unlock_by_sclk ) )
    {
      /* Tx unlock_by is more urgent */
      rx->unlock_by_sclk = tx.unlock_by_sclk;
#ifdef FEATURE_HDR_QTA
      rx->winning_client = tx.winning_client;
#endif /* FEATURE_HDR_QTA */
    }
  }
  else
  {
     HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                  "chain:%d ext_unlock: %d", chain_id, 
                  hdrsrchrf.chain[chain_id].rl_state.ext_unlock );
  }
  REX_ENABLE_PREMPTION();
  
} /* hdrsrchrf_get_retain_lock_state */


/*============================================================================

FUNCTION HDRSRCHRF_UNREGISTER_UNLOCK_CB

DESCRIPTION
  Informs the HDRSRCHRF that the client no longer wants to be notified of 
  unlock requests. Although information in unlock requests will be saved and
  accessible via hdrsrchrf_get_retain_lock_state, the unlock callback will
  no longer be called. A call to hdrsrchrf_retain_lock will cause the unlock
  callback to be called with the information that was saved in the last
  unlock call by TRM..
      
DEPENDENCIES
  The client must have called trm_retain_lock.

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_unregister_unlock_cb
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  hdrsrchrf_unlock_callback_t           unlock_cb;
    /* The unlock callback for this given client. */

/*--------------------------------------------------------------------------*/

  if ( chain_id >= HDRSRCHRF_NUM_CHAINS ) 
  {
    ERR_FATAL( "RF chain is invalid : %d  ", chain_id, 0, 0 );
  }

  unlock_cb = hdrsrchrf.chain[chain_id].unlock_cb;

  if ( unlock_cb != NULL )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                    "HDR unlock cb on chain:%d unregistered.",
                    chain_id );

    (void ) hdrsrchrf_change_duration( chain_id,
                                       ( trm_duration_t ) 0 );
      /* Extend the lock duration by 0 just so that we unregister the
         unlock callback. */


    hdrsrchrf.chain[chain_id].unlock_cb = NULL;
      /* Client no longer wants to be informed of unlock callbacks. */

  }

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain ) )
  {
    /* change_duration call above should've already uregistered TX
       with TRM, just set callback to NULL */
    hdrsrchrf.chain[chain_id].unlock_cb = NULL;
  }

} /* hdrsrchrf_unregister_unlock_cb */


/*============================================================================

FUNCTION HDRSRCHRF_EXTEND_DURATION

DESCRIPTION
  Attempts to extend the duration an RF chain lock is held for.
  If the entire extension can be granted, it will be granted.
  If the entire extension cannot be granted, the lock duration remains
  unchanged, and the caller should release the lock at the original lock
  expiry point.

  The extension is all or nothing.  If a partial extension is desired, the
  hdrsrchrf_change_duration( ) function should be used.
  
  RxTx compatible (but not using grouped API)

DEPENDENCIES
  The caller must be holding an RF chain lock

RETURN VALUE
  Duration extended for.

SIDE EFFECTS
  None

============================================================================*/

trm_grant_return_enum_type hdrsrchrf_extend_duration
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_duration_t                  duration
    /* The required sclk extension, from "now" */
)
{
  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*--------------------------------------------------------------------------*/

  if ( hdrsrchrf.chain[chain_id].req_notify_pending )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "ExtDur while R&N pending %x",
                   hdrsrchrf.chain[chain_id].grant_cb);

    hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;
  }

  modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  modify_state_info.modify_type = TRM_MODIFY_DURATION;
  modify_state_info.modify_info.modify_duration.duration = ( trm_duration_t ) duration;
  modify_state_info.tag = 0;
  trm_modify_chain_state( &modify_state_info );
    /* Just pass this request on to TRM. */

  hdrsrchrf_rfm_wait();
    /* Wait to ensure we receive response from TRM */

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
  {
    /* Owns TX chain, mirrors call on PRI chain */
    hdrsrchrf_extend_duration( HDRSRCHRF_TX_CHAIN, 
                               duration );
  }

  HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Extend Duration granted %d",
                   hdrsrchrf.chain[chain_id].modify_chain_granted );
  return ( hdrsrchrf.chain[chain_id].modify_chain_granted );

} /*  hdrsrchrf_extend_duration */


/*============================================================================

FUNCTION HDRSRCHRF_CHANGE_DURATION

DESCRIPTION
  Attempts to extend the duration an RF chain resource lock is held for.
  
  RxTx compatible (but not using grouped API)

DEPENDENCIES
  The client must be holding an RF chain resource lock

RETURN VALUE
  The new lock duration, from "now", in sclks.

SIDE EFFECTS
  None

============================================================================*/

hdrsrchrf_duration_t hdrsrchrf_change_duration
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  trm_duration_t                        duration
    /* The new lock duration granted. */

)
{

  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*--------------------------------------------------------------------------*/

  if ( hdrsrchrf.chain[chain_id].req_notify_pending )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "ChgDur while R&N pending %x",
                   hdrsrchrf.chain[chain_id].grant_cb);

    hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;
  }

  
  modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  modify_state_info.modify_type = TRM_MODIFY_DURATION;
  modify_state_info.modify_info.modify_duration.duration = duration;
  modify_state_info.tag = 0;
  trm_modify_chain_state( &modify_state_info );
    /* Just pass this request on to TRM. */

  hdrsrchrf_reset_retain_lock_state( chain_id );
    /* Clear out the retain lock state since this is no longer relevant. */

  hdrsrchrf_rfm_wait();
    /* Wait to ensure we receive response from TRM */

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
  {
    /* Owns TX chain, mirrors call on PRI chain */
    hdrsrchrf_change_duration( HDRSRCHRF_TX_CHAIN, 
                               duration );
    }

  return hdrsrchrf.chain[chain_id].modify_duration;
 

} /*  hdrsrchrf_change_duration */


/*============================================================================

FUNCTION HDRSRCHRF_CHANGE_PRIORITY

DESCRIPTION
  When a chain is used for a different reason, it should change the advertised
  reason for holding the RF chain resource, so its priority will change.

  Eg) A client request the RF chain resource for listening for a PAGE.  If
  it receives one, it would change its priority to PAGE_RESPONSE and attempt
  to respond to the page, and eventually change its priority to TRAFFIC.
  
  RxTx compatible (but not using grouped API)
  
DEPENDENCIES
  The client must be holding a RF chain resource lock

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_change_priority
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_reason_enum_t               reason
    /* The new resource why the RF lock is held (used for priority decisions) */
)
{
  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*--------------------------------------------------------------------------*/

  modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  modify_state_info.modify_type = TRM_MODIFY_REASON;
  modify_state_info.modify_info.modify_reason.new_reason = ( trm_reason_enum_t ) reason;
  modify_state_info.modify_info.modify_reason.sub_reason = hdrsrchrf_get_sub_reason( reason );
  modify_state_info.modify_info.modify_reason.min_duration = HDRSRCHRF_MIN_UNLOCK_DURATION;
  
#ifdef FEATURE_HDR_PBR_RANDOMIZATION
  modify_state_info.modify_info.modify_reason.wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

  modify_state_info.tag = 0;
  trm_modify_chain_state( &modify_state_info );
    /* Just pass this request to TRM. */

  hdrsrchrf.chain[chain_id].reason = reason;
    /* Remember the reason the lock is being held. */
  hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
  {
    switch ( reason )
    {
       case HDRSRCHRF_TRAFFIC:
       case HDRSRCHRF_BG_TRAFFIC:
       case HDRSRCHRF_LOW_LAT_TRAFFIC:

       case HDRSRCHRF_ACCESS:
#ifndef FEATURE_HDR_TRM_UNIFICATION
       case HDRSRCHRF_ACCESS_URGENT:
#endif /* FEATURE_HDR_TRM_UNIFICATION */
         /* TX chain only allows TX reason. */
         hdrsrchrf_change_priority( HDRSRCHRF_TX_CHAIN, 
                                    reason );
         break;

       default:
         HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                         "TX chain owned, skipped change reason to %d", 
                         reason );

    }
  }

} /*  hdrsrchrf_change_priority */


/*============================================================================

FUNCTION HDRSRCHRF_CHANGE_REASON

DESCRIPTION 
  This function is only used in DO to LTE reselection.  It is called to determine
  if TRM will allow the chain to be used for IRAT measurements.
 
  RxTx compatible (but not using grouped API)
 
DEPENDENCIES
  The client must be holding a RF chain resource lock

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

trm_grant_return_enum_type hdrsrchrf_change_reason
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_reason_enum_t               reason
    /* The new resource why the RF lock is held (used for priority decisions) */
)
{
  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*--------------------------------------------------------------------------*/

  modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  modify_state_info.modify_type = TRM_MODIFY_REASON;
  modify_state_info.modify_info.modify_reason.new_reason = ( trm_reason_enum_t ) reason;
  modify_state_info.modify_info.modify_reason.sub_reason = hdrsrchrf_get_sub_reason( reason );
  modify_state_info.modify_info.modify_reason.min_duration = HDRSRCHRF_MIN_UNLOCK_DURATION;

#ifdef FEATURE_HDR_PBR_RANDOMIZATION
  modify_state_info.modify_info.modify_reason.wakeup_identifier = hdrsrchsleep_get_unique_wakeup_id();
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

  modify_state_info.tag = 0;
  trm_modify_chain_state( &modify_state_info );
      /* Just pass this request to TRM. */

  hdrsrchrf_rfm_wait();
    /* Wait to ensure we receive response from TRM */

  hdrsrchrf.chain[chain_id].reason = reason;
    /* Remember the reason the lock is being held. */
  hdrsrchrf.chain[chain_id].sub_reason = hdrsrchrf_get_sub_reason(reason);

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
  {
    switch ( reason )
    {
       case HDRSRCHRF_TRAFFIC:
       case HDRSRCHRF_BG_TRAFFIC:
       case HDRSRCHRF_LOW_LAT_TRAFFIC:

       case HDRSRCHRF_ACCESS:
#ifndef FEATURE_HDR_TRM_UNIFICATION
       case HDRSRCHRF_ACCESS_URGENT:
#endif
         /* TX chain only allows TX reason. */
         hdrsrchrf_change_reason( HDRSRCHRF_TX_CHAIN, 
                                    reason );
         break;

       default:
         HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                         "TX chain owned, skipped change reason to %d", 
                         reason );

    }
  }

  return ( hdrsrchrf.chain[chain_id].modify_chain_granted );

} /*  hdrsrchrf_change_reason */


/*============================================================================

FUNCTION HDRSRCHRF_GET_RF_DEV

DESCRIPTION
  Get the physical RF device identifier mapped to the given logical chain.
  
DEPENDENCIES
  None
  
RETURN VALUE
  RF device or RFM_INVALID_DEVICE if chain_id is invalid or not owned by
  HDR.

SIDE EFFECTS
  None

=========================================================================*/
rfm_device_enum_type hdrsrchrf_get_rf_dev
( 
  hdrsrchrf_logical_chain_enum_t chain_id
)
{

  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;

  if ( (chain_id < HDRSRCHRF_NUM_CHAINS) &&
       hdrsrchrf.chain[chain_id].owns_chain )
  {
    rf_device = hdrsrchrf.chain[chain_id].rf_chain;
  }

  return rf_device;
}

/*============================================================================

FUNCTION HDRSRCHRF_GET_LAST_VALID_RF_DEV

DESCRIPTION
  Get the physical RF device identifier mapped to the PRI logical chain.
  This function always return a valid RF device. When HDR owns RF, it
  returns the owned RF device, otherwise, it turns the last owned RF, if
  HDR never owned any chain, it returns a default based on RF capability.
  
DEPENDENCIES
  None
  
RETURN VALUE
  When HDR owns RF:             PRI RF chain owned
  When HDR does not own RF:     Last PRI RF chain owned
  When HDR never owned RF:      Default PRI chain based on RF capability
  
SIDE EFFECTS
  None

=========================================================================*/
rfm_device_enum_type hdrsrchrf_get_last_valid_rf_dev( void )
{
  static rfm_device_enum_type prev_rf_dev = RFM_INVALID_DEVICE;

  rfm_device_enum_type rf_dev;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Intialize default RF device */
  if ( prev_rf_dev == RFM_INVALID_DEVICE )
  {
    /* REVISIT: Curently only support PRI on RF dev0 or 2. May need to be
       updated on future hardware */

    if ( !HDRSRCHRF_IS_SVDO_ENABLED() &&
         HDRSRCHRF_IS_SVLTE_ENABLED() )
    {
      prev_rf_dev = RFM_DEVICE_2;
    }
    else
    {
      prev_rf_dev = RFM_DEVICE_0;
    }
  }

  rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN);

  if ( rf_dev == RFM_INVALID_DEVICE )
  {
    /* Per discussion with RF, band/chan capability is based on RF CAL
       and unrelated to whether HDR owns the chain. So, when HDRSRCH
       is asleep/tunedaway, we can safely use prev device */
    rf_dev = prev_rf_dev;
  }
  else
  {
    /* have current valid RF device */
    prev_rf_dev = rf_dev;
  }
  return rf_dev;
}
  
/*============================================================================

FUNCTION HDRSRCHRF_HAS_LOCK

DESCRIPTION
  Whether the given chain is locked and thus controlled by HDR.
  
DEPENDENCIES
  None
  
RETURN VALUE
  True if it is locked by HDR, False otherwise.

SIDE EFFECTS
  None

============================================================================*/

boolean hdrsrchrf_has_lock
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{

/*--------------------------------------------------------------------------*/

  return hdrsrchrf.chain[chain_id].owns_chain;
    /* Return whether the chain is locked for use by HDR. */

} /* hdrsrchrf_has_lock */


/*=========================================================================

FUNCTION HDRSRCHRF_HAS_MULTI_RX

DESCRIPTION 
  This function queries TRM if mulitple RX chains are supported.

DEPENDENCIES
  None

RETURN VALUE

  TRUE  - support multiple receive chains
  FALSE - support single receive chain

SIDE EFFECTS 
  None

=========================================================================*/

boolean hdrsrchrf_has_multi_rx( void )
{
  trm_rf_enum_t  rf_rx_cfg;
  boolean        has_multi_rx = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  rf_rx_cfg = trm_get_rf_config();
    /* Get the RF RX chain config from hdrsrchrf */

  if ( rf_rx_cfg > TRM_RF_DIVERSITY ) 
  {
    /* if TRM reports more one receive chain */

    has_multi_rx = TRUE;
  }

  return has_multi_rx;
} /* hdrsrchrf_has_multi_rx( ) */


/*============================================================================

FUNCTION HDRSRCHRF_IS_SHDR_SUPPORTED

DESCRIPTION
  Returns whether the hardware supports SHDR operation.
  
DEPENDENCIES
  None
  
RETURN VALUE
  Whether the hardware supports SHDR operation.

SIDE EFFECTS
  None

============================================================================*/

boolean hdrsrchrf_is_shdr_supported( void )
{

/*--------------------------------------------------------------------------*/

#ifdef FEATURE_FULL_TIME_SHDR_3
   return hdrsrchrf_has_multi_rx( ) && 
                        ( trm_get_1x_mode() != TRM_1X_MODE_FORCE_SOODA );
#else
   return hdrsrchrf_has_multi_rx( );
#endif /* FEATURE_FULL_TIME_SHDR_3 */

} /* hdrsrchrf_get_shdr_supported( ) */


/*==========================================================================

                     STATIC FUNCTIONS FOR MODULE

==========================================================================*/


/*===========================================================================

FUNCTION HDRSRCHRF_START_TIMER

DESCRIPTION
  Begins timing an RF event.  If the time specified is zero, event
  is asserted immediately.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  TUNE_SIG will eventually be asserted, causing hdrsrchrf_tune_sig( ) to
  be called.

===========================================================================*/

static void hdrsrchrf_start_timer
(
  hdrsrchrf_logical_chain_enum_t  chain_id,
    /* The logical chain ID associated with the request */

  timetick_type                   duration_us,
    /* Duration the timer should run for */

  hdrsrchrf_tune_state_type       reason
    /* Reason why RF tune timer is running */
)
{
  hdrsrchrf_logical_chain_t*      rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Pointer to the RF chain that requested the tune. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) HDRSRCH_SIG_CLR( rf_chain->tune.tune_sig );
    /* Ensure the signal is not set */

  rf_chain->tune.state = reason;
    /* Update the reason for the tune_timer running */

    /* no timeout for bandclass change if duration is 0 */
  if ( reason != HDRSRCHRF_WAITING_BAND_GRANT ||
       duration_us != 0 )
  {
    hdrsrch_add_to_wait_mask( rf_chain->tune.tune_sig );
      /* Ensure the task is watching for TUNE signal only if timer is started */    

    timer_set( &rf_chain->tune.tune_timer, duration_us, 0, T_USEC );
  }
    /* Set the tune timer for the appropriate duration */

} /* hdrsrchrf_start_timer */


/*===========================================================================

FUNCTION HDRSRCHRF_CANCEL_TUNE_TIMER

DESCRIPTION
  Cancel the tune timer associated with the chain_id so that the tune complete
  callback never fires.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

static void hdrsrchrf_cancel_tune_timer
(
  hdrsrchrf_logical_chain_enum_t  chain_id
    /* The logical chain ID associated with the request */
)
{
  hdrsrchrf_logical_chain_t*      rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Pointer to the RF chain that requested the tune. */

  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchrf.tune_timer_defined == TRUE  )
  {
    (void) timer_clr( &rf_chain->tune.tune_timer, T_NONE );
      /* Clear the RF tune timer, if it happens to be running */

    hdrsrch_kill_signal( rf_chain->tune.tune_sig );
      /* ... and kill the timer's signal */

    if ( rf_chain->tune.band_change.band_changing )
    {
      modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
      modify_state_info.modify_type = TRM_MODIFY_BAND;
      modify_state_info.modify_info.modify_band.action = TRM_BAND_TUNE_CANCEL;
      modify_state_info.tag = 0;
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "hdrsrchrf_cancel_tune_timer: start TRM_MODIFY_BAND TRM_BAND_TUNE_CANCEL");
      trm_modify_chain_state( &modify_state_info );
        /* Just pass this request to TRM. */
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "hdrsrchrf_cancel_tune_timer: end TRM_MODIFY_BAND TRM_BAND_TUNE_CANCEL");
    }

    rf_chain->tune.band_change.band_changing = FALSE;
    rf_chain->tune.band_change.band_granted = FALSE;

    rf_chain->tune.state = HDRSRCHRF_NO_TUNE_IN_PROGRESS;
      /* And clear the tune state. */
  }
} /* hdrsrchrf_cancel_tune_timer */


#ifdef FEATURE_HDR_REVB 
/*===========================================================================

FUNCTION HDRSRCHRF_DEASSIGN_CSRS

DESCRIPTION
  The functon update the local CSR structures to disable all the CSRs. Please
  note that only the primary chain CSR structure is updated and maintined during
  the HDR rf activities. This is because HDR either uses the primary chain or
  uses the 2nd chain as diversity to the primary chain.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_deassign_csrs( void )
{
  uint8    i;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Init the CSR list as RF is tunned. */

  for( i=0; i<HDRSRCH_MAX_NUM_CARRIERS; i++ )
  {
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].enabled = FALSE;
  } /* for i */
} /* void hdrsrchrf_deassign_csrs( void ) */


/*===========================================================================

FUNCTION HDRSRCHRF_ASSIGN_CSRS

DESCRIPTION
  The function assign the CSRs based on the current channel config. The function
  update the local data structure of the CSR allocation based on the current
  demod index assignment list from TCA. Please be aware that the CSR info is 
  stored in the primary chain only.


DEPENDENCIES
  The hdrsrchrf.chain[0].tune.channel should be updated before calling this 
  function. To simplify the design, hdrsrchrf_deassign_csrs should be called 
  before this.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_assign_csrs
(
  int32                     *csrs_in_use
    /* The csr list passed in */
)
{

  uint8    i, csr_idx; 
    /* Loop index and next available csr index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Assign CSRs based on the channel list */

  for( i=0; i<hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel_cnt; i++ )
  {
    csr_idx = hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.demod_idx[i];
      /* The csr index is set to the same as the demod index */

    if( csr_idx < HDRSRCH_MAX_NUM_CARRIERS )
    {
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[csr_idx].enabled = TRUE;
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[csr_idx].channel =  
                          hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[i];
      csrs_in_use[i] = csr_idx;
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Could not find a free CSR!");
        /* TBD: should we error fatal on this one? */
    }
  } /* for i */
} /* void hdrsrchrf_assign_csrs */


/*===========================================================================

FUNCTION HDRSRCHRF_ASSIGN_SAMPLE_SERVERS

DESCRIPTION
  The function assign the sample based on the current channel configuration.
  The sample server configuration should match to the CSR configuration in 
  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr.

DEPENDENCIES
  The channel list should be updated before calling this function. For the
  Diversity chain, the tunning info and CSR info should be copied from
  the primary chain to the secondary chain.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_assign_sample_servers
(
  hdrsrchrf_logical_chain_enum_t  chain_id
    /* Logic chain ID */
)
{
  uint8    carrier_idx;
    /* Loop index */

  uint8    carrier_mask = 0;
    /* Sample server mode */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( carrier_idx = 0; carrier_idx < HDRSRCH_MAX_NUM_CARRIERS; carrier_idx++ )
  {
    /* Always use the primary chain CSR information to configure the sample 
       servers for both primary chain and the 2nd chain . */

    if( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[carrier_idx].enabled == TRUE )
    {
      carrier_mask |= 1 << carrier_idx;
        /* Enable the sample server if the corresponding CSR is enabled  */
    }

  } /* for i */

} /* void hdrsrchrf_assign_sample_servers */


/*===========================================================================

FUNCTION HDRSRCHRF_TC_ENTER_IDLE

DESCRIPTION
  The function update the tunning infomation, channel_list, channel_cnt,
  csr configuration, sample servers during the state transition from traffic
  to idle.

DEPENDENCIES
  The function should be called after the set management change from traffic
  to idle.

  The function only handles the RF configuration on the primary chain. Therefor
  the funciton should be called right after the diversity is disabled at
  "hdrsrchtc_done" during the state transition from TC to Idle.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_tc_enter_idle
(
  sys_channel_type * channel
    /* The channel to idle on */
)
{ 
  int8    i, idle_csr_idx = -1;
    /* Loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the tunning information. The current solution is not to retune
     the radio until the first idle wakeup. The benefits of doing this are
     - No impact to the legacy time line when move from MC traffic to 1x idle.
     - AT is able to continue monitor the CC channel without interruption 
       caused by the tunning */

  /* Update the tunning information */

#if 0
  /* TBD: the information may not need update as we are moving from the traffic
     to idle. no RF tunning is request at this time. */
  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel_cnt = 1;
  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[0]  = *channel;
#endif 

  /* Update the CSR structure */

  for( i=0; i<HDRSRCH_MAX_NUM_CARRIERS; i++ )
  {
    if( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].enabled == TRUE )
    {

      if( SYS_CHAN_CMP_EQUAL( *channel,
          hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].channel ) == FALSE )
      {
        /* Turn off CSRs not on the idle channels */

        hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].enabled = FALSE;
      }
      else
      {
        idle_csr_idx = i;
      } /* if */
    }
  } /* for */

  if( idle_csr_idx == -1 )
  {
    ERR_FATAL( "Idle channel does not match to the CSR config.", 0, 0, 0 );
  }
  else
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                    "From TC to Idle at chan%d band%d, keep CSR[%d]",
                hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[0].chan_num,
                    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[0].band,
                    idle_csr_idx );
  }

  /* Configure the sample servers based on the new RF configuration */

  hdrsrchrf_assign_sample_servers( HDRSRCHRF_FIRST_CHAIN );
    /* Only one sample server should be enabled */

  /* Reconfigure the CSRs through RF interface */

} /* void hdrsrchrf_tc_enter_idle */
#endif /* FEATURE_HDR_REVB */


/*===========================================================================

FUNCTION HDRSRCHRF_START_TUNE

DESCRIPTION
  This function begins tuning to the RF to the specified band and channel
  for the given chain_id.  

DEPENDENCIES
  The tune structure must be populated for the passed in chain.

RETURN VALUE
  The number of microseconds until the tune is complete if tune is successful.
  If tune fails return -1.

SIDE EFFECTS
  None

===========================================================================*/

static timetick_type hdrsrchrf_start_tune
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
#ifdef FEATURE_HDR_REVB
  int32                     csrs_in_use[HDRSRCH_MAX_NUM_CARRIERS];
    /* Temporary list to hold indexes of CSRs in use */

  uint8                     i, demod_idx;
    /* Loop index, demod_idx */

  uint8                     chan_cnt = hdrsrchrf.chain[chain_id].tune.channel_cnt;
    /* Current channel count */

  sys_channel_type*         chan_list = hdrsrchrf.chain[chain_id].tune.channel;
    /* Convinient pointor points to the channel list */

  int8                      chan_num_offset;
    /* LO offset in the unit of the channel number */

  int                       ret_val;               
    /* Return value from rf tune */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Two steps here
     1. tune to multple channel band
     2. assign the CSR to each of the carrier   */

  hdrsrchrf_deassign_csrs();
    /* Reset/reclaim all the csrs */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  /* Suspend ASDiv */
  (void ) hdrsrchrftxd_suspend_asdiv( TRUE, 
                                      TRM_ASDIV_STATE_UPDATE_REASON_RF_INTERACT );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  if( ( HDRSRCH_GHOST_CHAN_OFFSET != 0 ) &&
      ( hdrsrchrf.chain[chain_id].tune.channel_cnt < HDRSRCH_MAX_NUM_CARRIERS ) )
  {
    /* Add a ghost channel to the channel list based on the NV settings for the
       test purpose. The offset adds to the channel number of the first channel
       in the channel list */

    chan_num_offset = (int8) HDRSRCH_GHOST_CHAN_OFFSET;
    chan_list[chan_cnt].band = hdrsrchrf.chain[chain_id].tune.channel[0].band;
    chan_list[chan_cnt].chan_num = 
       hdrsrchrf.chain[chain_id].tune.channel[0].chan_num + 2 * chan_num_offset;

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                    "Ghost chan[%d] band[%d] is added. LO is shifted %d",
                    chan_list[chan_cnt].chan_num,
                    chan_list[chan_cnt].band,
                    chan_num_offset );
                                    
    chan_cnt ++;

    HDR_ASSERT ( chan_cnt <= HDRSRCH_MAX_NUM_SUBASETS ) ;
      /* KW fix : Check to limit chan_cnt to max */

    ret_val = hdrsrchrf_tune( &hdrsrchrf.chain[chain_id],
                    chan_cnt,
                    chan_list );
  }
  else
  {

    HDR_ASSERT ( hdrsrchrf.chain[chain_id].tune.channel_cnt <= HDRSRCH_MAX_NUM_SUBASETS ) ;
      /* KW fix : Check to limit channel_cnt to max */

    ret_val = hdrsrchrf_tune( &hdrsrchrf.chain[chain_id],  
                    hdrsrchrf.chain[chain_id].tune.channel_cnt,
                    hdrsrchrf.chain[chain_id].tune.channel );
  }

    if( ret_val == HDRSRCHRF_TUNE_ERROR )
    {
      HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"Bad RF cal!! Tune Fail ");

#ifdef FEATURE_HDR_SELECTIVE_TXD
    /* Allow ASDiv */
    (void ) hdrsrchrftxd_suspend_asdiv( FALSE, 
                                        TRM_ASDIV_STATE_UPDATE_REASON_RF_INTERACT );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      return HDRSRCHRF_TUNE_ERROR;
    }

  hdrsrchrf_assign_csrs( csrs_in_use );
    /* Assign csrs based on the current rf configuration */

  hdrsrchrf_assign_sample_servers( chain_id );
    /* Setup the configuration of the sample servers on the primary chain to 
       match to the configuration of the CSRs */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  /* Allow ASDiv */
  (void ) hdrsrchrftxd_suspend_asdiv( FALSE, 
                                      TRM_ASDIV_STATE_UPDATE_REASON_RF_INTERACT );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  memset( &hdrsrchrf_chan_list, 0, sizeof( hdrsrchafc_chan_list_type ) );
    /* Initialized the structure before being passed to the hdrsrchafcsm */

  for ( i = 0; i < hdrsrchrf.chain[chain_id].tune.channel_cnt; i ++ )
  {
    demod_idx = hdrsrchrf.chain[chain_id].tune.demod_idx[i];
    hdrsrchrf_chan_list.carrier[demod_idx].enable = TRUE;
    hdrsrchrf_chan_list.carrier[demod_idx].chan = 
                                    hdrsrchrf.chain[chain_id].tune.channel[i];
      /* For those inactivated carriers, value zeros are passed in. */
  }

  hdrsrchafc_set_chan( &hdrsrchrf_chan_list );
    /* Initialize the FLL based on the band. */

  return HDRSRCHRF_TUNE_DELAY_US;

} /* hdrsrchrf_start_tune( ) */


#ifdef FEATURE_HDR_RUMI
void hdrsrchrf_start_rxagc_rumi( void )
{
  
  int i, idx;  
  // Send back to back RX_START_CFG, RX_START
  cfw_rx_agc_cfg_msg_t agc_cfg_msg;
  cfw_rx_start_msg_t start_msg;
  cfw_rx_start_cfg_msg_t start_cfg_msg;
  // configure enabled chain override (with this we can use RXLM trigger function)
  //fw_rxlm_enabled_chain_s enabled_chains;

  //setting up the shared memory so that after memcpy in the msg processing LNA params
  //are available
  //the following is temporary definition of LNA switch points and timers 
  int16 lna_rise_thresholds[2][4] = {{-18206,-3644,10918,25480},{-18206,-3644,10918,25480}};
  int16 lna_fall_thresholds[2][4] = {{-25487,-10925,3637,18199},{-25487,-10925,3637,18199}};
  uint16 lna_bypass_timers[2][4] = {{5,6,7,8},{5,6,7,8}};
  uint16 lna_nonbypass_timers[2][4] = {{5,6,7,8},{5,6,7,8}};

  volatile hdrfw_smem_write_intf_t     *hdrSharedMemRdPtr = &(hdrmdsp_shared_mem_interface->write);

  //enabled_chains.intval = 0;
  //enabled_chains.wb_idx = 0;
  //enabled_chains.nb_idx_0 = 0;
  //enabled_chains.nb_idx_1 = 0xF;
  //enabled_chains.nb_idx_2 = 0xF;

  //setting up agc_cfg message
  memset((void *)&agc_cfg_msg, 0, sizeof(agc_cfg_msg)); 

  msgr_init_hdr( &agc_cfg_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_CMD_RX_AGC_CFG );

  agc_cfg_msg.antenna = 0;
  agc_cfg_msg.wb_idx = 0;
  agc_cfg_msg.acq_gain = 0x1e1e;
  agc_cfg_msg.trk_gain = 0x1e1e;
  agc_cfg_msg.acq_duration = 10;

  
  for(i=0;i<HDRFW_NUM_ANTENNAS;i++) {

    //add a non zero value for the acq duration so that the interrupt is actually serviced 
    //we do not see handler_list_len overflow crash in mpi_taskq_add_task_handler()
    hdrSharedMemRdPtr->cfw_lna_cfg[i].dc_acq_duration = 511;//~52 us in cx8
    hdrSharedMemRdPtr->cfw_lna_cfg[i].agc_max = (int16)32767;
     
    for (idx=0; idx < CFW_NUM_LNA_STATES; idx++) {
      hdrSharedMemRdPtr->cfw_lna_cfg[i].agc_min[idx] = (int16)-32768;           
      hdrSharedMemRdPtr->cfw_lna_cfg[i].cal_gain_offset[0][idx] = (int16)0;              
    }

    for(idx=0;idx < CFW_NUM_LNA_STATES-1; idx++) {
      hdrSharedMemRdPtr->cfw_lna_cfg[i].rise_threshold[idx] = lna_rise_thresholds[i][idx];
      hdrSharedMemRdPtr->cfw_lna_cfg[i].fall_threshold[idx] = lna_fall_thresholds[i][idx];
      hdrSharedMemRdPtr->cfw_lna_cfg[i].bypass_timer_values[idx] = lna_bypass_timers[i][idx];
      hdrSharedMemRdPtr->cfw_lna_cfg[i].nonbypass_timer_values[idx] = lna_nonbypass_timers[i][idx];
    }

  }
  //flush to memory
  //hdr_shared_mem_dcache_clean(&hdrSharedMemRdPtr->cfw_lna_cfg[0], 2*sizeof(cfw_rf_LNA_interface_t));

  hdrsrchmsg_send_msg(&agc_cfg_msg, sizeof(agc_cfg_msg));

  hdrsrch_timed_wait_usecs(10000);
    /* Wait for 10ms */

  // Everything 0 including RxLM buffer index and RF script index
  memset((void *)&start_cfg_msg, 0, sizeof(start_cfg_msg));

  // XXX Disabled for now !! - To try this on simulation unit test
  // disable PA_RANGE update in txagc. Most likely model issue.
  msgr_init_hdr( &start_cfg_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_CMD_RX_START_CFG );

  start_cfg_msg.rx_config.rf_script_idx = 0xFFFFFFFF;

  start_cfg_msg.rx_config.settling_rtc_cx1 = 600;

  hdrsrchmsg_send_msg(&start_cfg_msg, sizeof(start_cfg_msg));

  hdrsrch_timed_wait_usecs(10000);
    /* Wait for 10ms */

  memset((void *)&start_msg, 0, sizeof(start_msg));

  msgr_init_hdr( &start_msg.hdr,
                 MSGR_HDR_SRCH,
                 HDR_FW_CMD_RX_START );

  start_msg.immediate_trigger = 1;

  hdrsrchmsg_send_msg(&start_msg, sizeof(start_msg));  

  /* Wait for RX_START_RSP, this is to avoid a FW crash */
  hdrsrchmsg_set_rsp_waiting( HDRSRCH_RX_START_RSP, TRUE );
  hdrsrch_wait_for_high_pri_rsp( 60000,
                                 HDRSRCH_RX_START_RSP );
}
#endif /* FEATURE_HDR_RUMI */

/*===========================================================================

FUNCTION HDRSRCHRF_HANDLE_UNRECOVERABLE_RF_ERR

DESCRIPTION
  Common handling for RF returning an unrecoverable error. HDR will
  print the error code and declare system loss.

DEPENDENCIES
  Called in response to HDRSRCH_TUNE_SIG/HDRSRCHRF_TUNING_IN_PROGRESS:

RETURN VALUE
  None

SIDE EFFECTS
  Starts a timer, which causes HDRSRCH_TUNE_SIG/HDRSRCHRF_LOOPS_SETTLING 

===========================================================================*/
void hdrsrchrf_handle_unrecoverable_rf_err
(
  hdrsrchrf_logical_chain_enum_t chain_id,
  int err
)
{
  /* Bad RF Cal */
  HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,"RF returned err %d, system loss", 
                  err );

  hdrsrch_kill_signal( HDRSRCH_TUNE_SIG );

  hdrsrchrf_disable_rx( chain_id );

  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_BAD_RF_CAL );
}

/*===========================================================================

FUNCTION HDRSRCHRF_SETTLE_START

DESCRIPTION
  Allows RF loops a chance to settle before declaring tuning complete

DEPENDENCIES
  Called in response to HDRSRCH_TUNE_SIG/HDRSRCHRF_TUNING_IN_PROGRESS:

RETURN VALUE
  None

SIDE EFFECTS
  Starts a timer, which causes HDRSRCH_TUNE_SIG/HDRSRCHRF_LOOPS_SETTLING 

===========================================================================*/
#ifdef FEATURE_RUMI_BRINGUP
#error code not present
#endif /* FEATURE_RUMI_BRINGUP */

static void hdrsrchrf_settle_start
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  int ret_val;
  /* Return value from rf API */

  hdrsrchrf_tune_done_cb_t            tune_done_func;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Enable AGC */
  {
#ifndef FEATURE_HDR_REVB 
    rf_hdr_start_agc_acq( hdrsrchrf.chain[chain_id].rf_chain );
      /* turn on automatic loop control now that we are ready on new channel */
#else

#ifdef FEATURE_RUMI_BRINGUP
#error code not present
#endif /* FEATURE_RUMI_BRINGUP */

#endif /* FEATURE_HDR_REVB */
  }

  if ( hdrsrchrf.rf_exec_wakeup_rx_pending == TRUE )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    if ( chain_id == HDRSRCHRF_PRI_CHAIN )
    {
      hdrsrchrftxd_init_ant_switch( hdrsrchrf.chain[chain_id].rf_chain );
    }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_RUMI
    if ( !hdrhit_rumi_cfg.y1y2_support )
    {
      hdrsrchrf_start_rxagc_rumi();
    }
    else
    {
#endif /* FEATURE_HDR_RUMI */
#ifdef FEATURE_HDR_RUMI
#if 0
      if ( hdrhit_rumi_cfg.div_ctrl &&
           ( hdrhit_rumi_cfg.div_on_hack == 2 ) )
      {                
        hdrSharedMemRdPtr->div_info.div_rx_mode = HDRFW_DIV_RX_MODE_DUAL_TRIG;
          /* Delay PRI rx start until DIV. This is to ensure FLEX PRI/DIV alignment */
      }      
#endif 
#endif /* FEATURE_HDR_RUMI */

      ret_val = rfm_hdr_exec_wakeup_rx(
        hdrsrchrf.chain[chain_id].rf_chain, /* receive path to wakeup          */
        hdrsrchrf.chain[chain_id].rxlm_handle,
                              /* The Rx Link Manager buffer to config modem HW */
        hdrsrchrf_rfm_cb,           /* Called when wakeup sequence is complete */
        NULL );    /* Data handle to pass additional info to the cb function   */

      hdrsrchrf_rfm_wait();

      if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, 
                      "Unexpected RF failure during TUNE" );

        if ( hdrsrchrf.chain[chain_id].tune.done_cb != NULL )
        {
          tune_done_func = hdrsrchrf.chain[chain_id].tune.done_cb;
          hdrsrchrf.chain[chain_id].tune.done_cb = NULL;
          tune_done_func(HDRSRCHRF_TUNE_DENIED);
            /* Inform appropriate state cb that tuning failed */
        }

        hdrsrchrf_handle_unrecoverable_rf_err( chain_id, ret_val );
        
        return;  
      }
      else if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }

#ifdef FEATURE_HDR_RUMI
      if ( hdrhit_rumi_cfg.div_ctrl &&
           ( hdrhit_rumi_cfg.div_on_hack != 0 ) )
      {                
        hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].owns_chain = TRUE;
        hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].rf_chain = 
            hdrsrchrf_get_rf_device( HDRSRCHRF_CHAIN_TO_TRM( HDRSRCHRF_DIV_CHAIN ) );
          /* Force DIV to be initialized (before TRM grant) */

        hdrsrchrfdiv_enable_diversity_chain();
          /* Enable DIV */

        hdrhit_rumi_cfg.div_enable_stage = 2;
      }
#endif /* FEATURE_HDR_RUMI */
#ifdef FEATURE_HDR_RUMI
    }
#endif /* FEATURE_HDR_DIME_RUMI_IQ */

    hdrsrchrf.chain[chain_id].is_tuned = TRUE;
      /* Mark tuned now that rfm_hdr_exec_wakeup has been run. */

    hdrsrchrf.rf_exec_wakeup_rx_pending = FALSE;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Tune state: %d", hdrsrchrf.chain[chain_id].is_tuned);
  }

  hdrsrchrf_start_timer( chain_id,
                         HDRSRCHRF_HS_TO_US( hdrsrchrf.loop_settling_time ),
                         HDRSRCHRF_LOOPS_SETTLING );
    /* Wait the appropriate time for loops to settle */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "AGC ACQ Mode: %d, Time %dhs, Prev AGC: %d",
                  hdrsrchrf.agc_acq_mode,
                  hdrsrchrf.loop_settling_time,
                  hdrsrchrf.agc_value );

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
    /* Set AGC acquisition parameters for next tune to normal mode 
       Note this function should be called last since it changes
       value of AGC acq mode, hdrsrchrf.loop_settling_time, etc */

} /* hdrsrchrf_settle_start( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_TUNE_DONE

DESCRIPTION
  This function completes the tune to the required channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  tune_done( ) callback called

===========================================================================*/

void hdrsrchrf_tune_done
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  hdrsrchrf_tune_done_cb_t              tune_done_func;
    /* Temporary to prevent error if tune_done calls tune */

#ifdef FEATURE_HDR_REVB
  uint8                                 i;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

  static trm_grouped_modify_state_input_info   grouped_modify;
    /* Input for change_duration */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RF_TUNE_DONE );

  memset( &grouped_modify, 0, sizeof( grouped_modify ) );
    /* Initialize the structure before passing to TRM */

  hdrsrchrf.chain[chain_id].tune.state = HDRSRCHRF_RF_TUNED;
    /* The RF is tuned so save the state information. */

#ifndef FEATURE_HDR_REVB
  rf_hdr_start_agc_track( hdrsrchrf.chain[chain_id].rf_chain );
    /* Switch HDR agc to tracking mode */
#else

#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVB
  for( i=0; i<hdrsrchrf.chain[chain_id].tune.channel_cnt; i++)
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Tuned to band=%d chan=%d Div=%d",
                    hdrsrchrf.chain[chain_id].tune.channel[i].band,
                    hdrsrchrf.chain[chain_id].tune.channel[i].chan_num,
                    hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled );
  }
#else
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Tuned to band=%d chan=%d Div=%d",
                  hdrsrchrf.chain[chain_id].tune.channel.band,
                  hdrsrchrf.chain[chain_id].tune.channel.chan_num,
                  hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled );
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    
   if(hdrsrchrf.chain[chain_id].tune.channel_cnt > HDRSRCH_MAX_NUM_SUBASETS)
   {
       HDR_MSG_PROT_2 (MSG_LEGACY_ERROR, "channel count is %d greater than max allowed. limiting to max %d."
               ,hdrsrchrf.chain[chain_id].tune.channel_cnt, HDRSRCH_MAX_NUM_SUBASETS);  
       hdrsrchrf.chain[chain_id].tune.channel_cnt = HDRSRCH_MAX_NUM_SUBASETS;
   }

    hdrmultirat_process_freq_change( hdrsrchrf.chain[chain_id].rf_chain,
                                     hdrsrchrf.chain[chain_id].tune.channel,
                                     HDRMULTIRAT_FORWARD_LINK,
                                     hdrsrchrf.chain[chain_id].tune.channel_cnt );
      /* Send band-chan change msg to FW*/
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  hdrsrchrf_report_rx_power();
    /* Show the Rx power of this new channel */

  if ( hdrsrchrf.chain[chain_id].tune.band_change.band_changing )
  {
    grouped_modify.modify_type = TRM_MODIFY_BAND;
    grouped_modify.modify_info[0].client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
    grouped_modify.modify_info[0].modify_data.modify_band.action = TRM_BAND_TUNE_CONFIRM;

    if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
         hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
    {
      grouped_modify.num_of_entries = 2;
      grouped_modify.modify_info[1].client_id = TRM_HDR_TX;
      grouped_modify.modify_info[1].modify_data.modify_band.action = TRM_BAND_TUNE_CONFIRM;
    }
    else
    {
      grouped_modify.num_of_entries = 1;
    }
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "hdrsrchrf_tune_done: start TRM_MODIFY_BAND TRM_BAND_TUNE_CONFIRM");

    trm_grouped_modify_chain_state( &grouped_modify );
      /* confirm band tune completed */
      /* TODO actually confirm band tune from cb */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "hdrsrchrf_tune_done: end TRM_MODIFY_BAND TRM_BAND_TUNE_CONFIRM");

    hdrsrchrf.chain[chain_id].tune.band_change.band_changing = FALSE;
    hdrsrchrf.chain[chain_id].tune.band_change.band_granted = FALSE;
  }

  if ( hdrsrchrf.chain[chain_id].tune.done_cb != NULL )
  {
    tune_done_func = hdrsrchrf.chain[chain_id].tune.done_cb;
    hdrsrchrf.chain[chain_id].tune.done_cb = NULL;
    tune_done_func(HDRSRCHRF_TUNE_SUCCESS);
      /* Inform appropriate state function that tuning is complete */
  }

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( chain_id == HDRSRCHRF_PRI_CHAIN )
  {
    hdrsrchrftxd_set_band( hdrsrchrf.chain[chain_id].tune.channel[0].band );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */
  hdrsrch_remove_from_wait_mask( hdrsrchrf.chain[chain_id].tune.tune_sig );
    /* tune is completed, tune sig should be cleared */

  hdrsrch_add_to_wait_mask( HDRSRCH_SCHEDULE_SIG );
      /* add back the schedule sig so it can be processed again*/

} /* hdrsrchrf_tune_done( ) */


/*============================================================================

FUNCTION HDRSRCHRF_RESET_RF_CHAIN_STATE

DESCRIPTION
  Reset the state of the RF chain resource currently held by a caller.  This 
  API should not be called with the HDRSRCHRF_DIV_CHAIN parameter.  Instead
  hdrsrchrf_release_diversity should be called. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The diversity chain will be disabled and released if the primary chain
  was given.

============================================================================*/

void hdrsrchrf_reset_rf_chain_state
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
#ifdef FEATURE_HDR_REVB 
  uint8    i;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

/*--------------------------------------------------------------------------*/

  hdrsrchrf_cancel_tune_timer( chain_id );
    /* Cancel any pending tune of this chain. */

#ifdef FEATURE_HDR_REVB 
  hdrsrchrf.chain[chain_id].tune.channel_cnt = 0;
    /* Reset the channel number we are tuned to to an invalid number. */

  for( i=0; i<HDRSRCH_MAX_NUM_SUBASETS; i++ )
  {
    hdrsrchrf.chain[chain_id].tune.channel[i].chan_num = 0xFFFF;
      /* Reset the channel number we are tuned to to an invalid number. */
  }

  for( i=0; i<HDRSRCH_MAX_NUM_CARRIERS; i++ )
  {
    hdrsrchrf.chain[chain_id].tune.csr[i].enabled  = FALSE;
      /* Reset the carrier informations. */
  }
#else
  hdrsrchrf.chain[chain_id].tune.channel.chan_num = 0xFFFF;
    /* Reset the channel number we are tuned to to an invalid number. */
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf.chain[chain_id].unlock_cb = NULL;
    /* Reset the unlock callback pointer for this client. */

  hdrsrchrf.chain[chain_id].grant_cb = NULL;
    /* Reset the grant callback pointer for this client. */

  hdrsrchrf.chain[chain_id].owns_chain = FALSE;
    /* HDR no longer controls this chain. */

  hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;

  hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_NO_EVENT;

  hdrsrchrf.chain[chain_id].new_dev = RFM_INVALID_DEVICE;

  hdrsrchrf_reset_retain_lock_state( chain_id );
    /* If we were retaining the lock we aren't anymore. */

} /* hdrsrchrf_reset_rf_chain_state */


/*==========================================================================

                      EXPORTED RF TUNING FUNCTIONS

==========================================================================*/


/*===========================================================================

FUNCTION HDRSRCHRF_BAND_GRANTED

DESCRIPTION
  Moves band tune state machine to in_progress after band is granted

DEPENDENCIES
  Called in response to HDRSRCH_TUNE_SIG/HDRSRCHRF_WAITING_BAND_GRANT.
  The function must be called in HDRSRCH task.

RETURN VALUE
  None

SIDE EFFECTS
  Movies tune state to HDRSRCHRF_TUNE_IN_PROGRESS 

===========================================================================*/
static void hdrsrchrf_band_granted
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  uint8 i;

  hdrsrchrf_logical_chain_t* rf_chain;

  timetick_type tune_time;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Grab hold of a pointer to the chain. */

  hdrsrch_kill_signal( HDRSRCH_SCHEDULE_SIG );
    /* Clear signal, then remove signal from wait mask */

  tune_time = hdrsrchrf_start_tune( chain_id );
    /* begin RF tune */

  if( tune_time == HDRSRCHRF_TUNE_ERROR )
    {
      if (hdrsrchrf.chain[chain_id].tune.done_cb != NULL)
      {
        hdrsrchrf.chain[chain_id].tune.done_cb(HDRSRCHRF_TUNE_DENIED);

        hdrsrchrf.chain[chain_id].tune.done_cb = NULL;
      }

      hdrsrchrf_cancel_tune_timer( chain_id );
        /* Cancel any pending tune of this chain. */

      hdrsrchrf_handle_unrecoverable_rf_err( chain_id, HDRSRCHRF_TUNE_ERROR );

      return;
    }

  for( i=0; i<rf_chain->tune.channel_cnt; i++ )
  {
    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "Tune to band=%d channel=%d Demod_idx=%d (%d ms)",
                    rf_chain->tune.channel[i].band,
                    rf_chain->tune.channel[i].chan_num,
                    rf_chain->tune.demod_idx[i],
                    tune_time);
  }

  hdrsrchrf_start_timer( chain_id, tune_time, HDRSRCHRF_TUNING_IN_PROGRESS );
    /* Start the timer for the tune period */
}

/*===========================================================================

FUNCTION HDRSRCHRF_TUNE_SIG

DESCRIPTION
  This function handles the TUNE signal from the Tune Timer.

DEPENDENCIES
  The function must be called in HDRSRCH task.

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/

void hdrsrchrf_tune_sig
( 
  hdrsrchrf_logical_chain_enum_t chain_id
    /* Chain ID of the RF chain tuned. */
)
{
  hdrsrchrf_tune_state_type       state;
    /* The previous tune state of the chain. */

  boolean band_granted;

  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_remove_from_wait_mask( hdrsrchrf.chain[chain_id].tune.tune_sig );
    /* We've got the tune signal - no longer need to watch for it. */

  state = hdrsrchrf.chain[chain_id].tune.state;
    /* Get reason why tune-timer was running */

  hdrsrchrf.chain[chain_id].tune.state  = HDRSRCHRF_NO_TUNE_IN_PROGRESS;
    /* And clear the reason */

  switch ( state )
  {
    case HDRSRCHRF_TUNING_IN_PROGRESS:
    {
      hdrsrchrf_settle_start( chain_id );
        /* Now let's start settling the AGC loops. */

      break;
    }

    case HDRSRCHRF_LOOPS_SETTLING:
    {
      hdrsrchrf_tune_done( chain_id );
        /* AGC loops are settled, let HDR search know the tune is complete. */

      break;
    }

    case HDRSRCHRF_NO_TUNE_IN_PROGRESS:
    case HDRSRCHRF_RF_TUNED:
    default:
    {
      hdrsrchrf.chain[chain_id].tune.state = state;
        /* Restore tune state for unexpected state */

      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Received tune sig in invalid state");

      break;
    }
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                  "RF Chain %d Tune Sig processed: Prev %d Next %d",
                  hdrsrchrf.chain[chain_id].rf_chain, 
                  state, 
                  hdrsrchrf.chain[chain_id].tune.state );
    /* Print out debug message. */

} /* hdrsrchrf_tune_sig() */


/*===========================================================================

FUNCTION HDRSRCHRF_ENABLE_RF_CHAIN

DESCRIPTION
  This function enables the RF chain.  HDRSRCHRF_DIV_CHAIN is not supported
  as a possible parameter.  This because the RF API is not symmetric between
  the diversity chain and primary chain.  The RF API returns after enabling
  has settled in the case of the primary chain but will return immediately
  in the case of the diversity chain.  Nothing will happen if it is passed in.  
  Use hdrsrchrf_enable_div_chain instead.

  When the function returns the chain is enabled and ready to be tuned. No
  delay is needed.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static boolean hdrsrchrf_enable_rf_chain
( 
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  hdrsrchrf_logical_chain_t*            rf_chain;
    /* Pointer to the RF chain that requested the tune. */

  lm_status_type                        lm_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Save a pointer to the chain_id passed in. */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Start RF enable. Switch SRAM Source");

  /* Grab buffer if we do not have one. */
  if (rf_chain->rxlm_handle_valid != TRUE)
  {
#ifdef FEATURE_HDR_RUMI
    rf_chain->rxlm_handle_valid = TRUE; 
    if ( hdrhit_rumi_cfg.y1y2_support )
    {
#endif /* FEATURE_HDR_RUMI */
    lm_status = rxlm_allocate_buffer( 
                  hdrsrchrf_get_rxlm_chain(chain_id), 
                  LM_HDR,
                  &rf_chain->rxlm_handle);

    if (lm_status == LM_SUCCESS)
    {
      rf_chain->rxlm_handle_valid = TRUE; 
        /* Mark handle as valid */

      if ( chain_id == HDRSRCHRF_PRI_CHAIN )
      {
        hdrsrchrf_enter_mcdo( );
          /* Make sure that the RF is in the MCDO mode */
      }
    }
    else
    {
      return FALSE;
    }
#ifdef FEATURE_HDR_RUMI
    } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */
  }

  /* Make sure we own the chain and that this isn't the diversity chain. */
  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) && 
       ( rf_chain->is_enabled == FALSE ) &&
       ( rf_chain->owns_chain == TRUE ) )
  {
#ifdef FEATURE_MULTIPLE_MODEM_CORES
    /* REVISIT: configure diversity chain when diversity is enabled */
    if (hw_set_system_mode(HW_SYSTEM_MODE_CDMA,HW_MODEM_CHAIN_0) == FALSE)
    {
       HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Could not set system mode to HDR on chain 0");
    }

#endif /* FEATURE_MULTIPLE_MODEM_CORES */

#ifndef FEATURE_HDR_DIME_MODEM
    /* sets DEMOD symbol buffer memory to HDR mode */
    HWIO_DEM_SYM_BUFF_MODE_OUT(0x01);
#endif /* FEATURE_HDR_DIME_MODEM */

      hdrsrchrf_pdm_restore_values();
  }

  return TRUE;
} /* hdrsrchrf_enable_rf_chain */


/*===========================================================================

FUNCTION HDRSRCHRF_GET_RXLM_CHAIN

DESCRIPTION
  This function returns the RXLM chain number needed to initialize
  rxlm buffer. 

DEPENDENCIES
  None

RETURN VALUE
  The RXLM chain id based on the RF config. Note this is a value
  unaffected by if HDR owns the chain or not.

SIDE EFFECTS
  None

===========================================================================*/
rxlm_chain_type hdrsrchrf_get_rxlm_chain
(
  hdrsrchrf_logical_chain_enum_t hdr_chain
)
{
  rxlm_chain_type lm_chain;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* In all current RF configs, PRI chain is assigned RXLM chain 0,
     and DIV chain is assigned RXLM chain 1 */
  if ( hdr_chain == HDRSRCHRF_PRI_CHAIN )
  {
    lm_chain =  RXLM_CHAIN_0;
  }
  else
  {
    lm_chain =  RXLM_CHAIN_1;
  }

  return lm_chain;
}

/*===========================================================================

FUNCTION HDRSRCHRF_GET_TXLM_CHAIN

DESCRIPTION
  This function returns the RXLM chain number needed to initialize
  rxlm buffer. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
txlm_chain_type hdrsrchrf_get_txlm_chain( void )
{
  txlm_chain_type lm_chain;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* In all current RF configs, if PRI is assigned RF_DEVICE_0, TXLM will
     be 0, otherwise, it will be 1*/

  switch( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain )
  {
    case RFM_DEVICE_0:
      lm_chain = TXLM_CHAIN_0;
      break;

    case RFM_DEVICE_2:
      lm_chain = TXLM_CHAIN_1;
      break;

    default:
      lm_chain = TXLM_CHAIN_0;
      HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Unsupported PRI chain: %d",
                     hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain);
      break;
  }

  return lm_chain;
}

/*===========================================================================

FUNCTION HDRSRCHRF_GET_COMBINED_GRANT

DESCRIPTION
  This function returns combined grant for PRI/TX chain

DEPENDENCIES
  None

RETURN VALUE
  Combined grant for PRI/TX chain

SIDE EFFECTS
  none

===========================================================================*/
trm_modify_return_enum_type hdrsrchrf_get_combined_grant
(
  hdrsrchrf_logical_chain_enum_t chain_id
)
{
  trm_modify_return_enum_type grant;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( chain_id != HDRSRCHRF_PRI_CHAIN ) ||
       ( !hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain ) )
  {
    /* For PRI chain + owning TX chain, check both chain,
       otherwise, check only the requested chain */
    grant = hdrsrchrf.chain[chain_id].modify_band_granted;
  }
  else if ( ( hdrsrchrf.chain[chain_id].modify_band_granted == TRM_DENIAL ) ||
            ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].modify_band_granted == TRM_DENIAL ) )
  {
    grant = TRM_DENIAL;
  }
  else if ( ( hdrsrchrf.chain[chain_id].modify_band_granted == TRM_PENDING ) ||
            ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].modify_band_granted == TRM_PENDING ) )
  {
    grant = TRM_PENDING;
  }
  else
  {
    grant = TRM_GRANTED;
  }

  HDR_MSG_SRCH_5( MSG_LEGACY_MED, 
                  "combined_grant chain:%d tx:%d grant:%d/%d=>%d", 
                  chain_id, 
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain, 
                  hdrsrchrf.chain[chain_id].modify_band_granted,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].modify_band_granted,
                  grant );

  return grant;
}

/*============================================================================

FUNCTION HDRSRCHRF_REQUEST_FREQUENCY_TUNE

DESCRIPTION 
  Request for a new band tune before retuning RF.
  
DEPENDENCIES
  None

RETURN VALUE
  TRM_BAND_REQUEST_GRANTED - Client can go through with the RF tune
  TRM_BAND_REQUEST_PENDING - Client needs to wait for the band grant cb

SIDE EFFECTS
  None

============================================================================*/
trm_modify_return_enum_type hdrsrchrf_request_frequency_tune
( 
  trm_client_enum_t            client,
    /* Client requesting a band change */
  trm_frequency_type_t         freq_info
    /* New band that the client wants to use */
)
{
  static trm_grouped_modify_state_input_info   group_modify;
    /* Input for change_duration */

  hdrsrchrf_logical_chain_enum_t        chain_id;
/*--------------------------------------------------------------------------*/

  memset( &group_modify, 0, sizeof( group_modify ) );
    /* Initialize the structure before passing to TRM */

  chain_id = HDRSRCHRF_CHAIN_FROM_TRM(client);

  group_modify.modify_type = TRM_MODIFY_BAND;
  group_modify.modify_info[0].client_id = client;
  group_modify.modify_info[0].modify_data.modify_band.action = TRM_BAND_TUNE_REQUEST;
  group_modify.modify_info[0].modify_data.modify_band.new_freq_info.num_bands = 1;
  group_modify.modify_info[0].modify_data.modify_band.new_freq_info.bands[0].band = freq_info.band;

  if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
       hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
  {
    group_modify.num_of_entries = 2;
    group_modify.modify_info[1] = group_modify.modify_info[0];
    group_modify.modify_info[1].client_id = TRM_HDR_TX;
  }
  else
  {
    group_modify.num_of_entries = 1;
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                 "band req client:%d tx:%d band:%d", 
                  client, 
                  (group_modify.num_of_entries == 2) ? 1 : 0, 
                  freq_info.band );

  trm_grouped_modify_chain_state( &group_modify );
    /* Just pass this request to TRM. */

  hdrsrchrf_rfm_wait();
    /* Wait to ensure we receive response from TRM */

  return hdrsrchrf_get_combined_grant(chain_id);
    /* Return pending for now, once we receive cb from trm we will update the result */
}

/*===========================================================================

FUNCTION HDRSRCHRF_SLEEP_AND_DISABLE_PRI

DESCRIPTION
  This function disables PRI before releasing it.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void hdrsrchrf_sleep_and_disable_pri( void )
{
  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "sleep_and_disable_pri" );

  hdrsrchrf_prep_to_sleep( );

  hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
}

/*===========================================================================

FUNCTION HDRSRCHRF_TUNE_TO_CHAN

DESCRIPTION
  This function tunes the RF to the specified band and channel and then
  calls the given callback when done. If band tune is not allowed by MCS
  return false.

DEPENDENCIES
  This function must be called with FPD disabled

RETURN VALUE 
  None for non-DR
  For DR:
  TRUE: if tune is allowed
  FALSE if tune fails

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchrf_tune_to_chan
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */
 
  const sys_channel_type                *channel,
    /* Channel to tune the RF to */

#ifdef FEATURE_HDR_REVB
  const hdrsrch_demod_idx_type          *demod_idx,
    /* Demod index assignment matching to the channel assignment*/

  uint8                                 channel_cnt,
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_tune_done_cb_t              tune_done,
    /* Callback when tuning is complete */

  rex_timer_cnt_type                    timeout
    /* timeout in mS, only used for band req API */
)
{
  hdrsrchrf_logical_chain_t*            rf_chain;
    /* Pointer to the RF chain that requested the tune. */

#ifndef FEATURE_HDR_REVB
  static hdrsrchrf_time_struct_type     debug_time;
    /* Debug storage for saved times in sclk */
#endif /* FEATURE_HDR_REVB */

  uint8                                 i;
    /* Loop index */

  hdrsrchrf_tune_state_type             tune_state;

  trm_modify_return_enum_type           band_granted = TRM_DENIAL;

  hdrsrchrf_trm_freq_info_type          tune_info;
    /* Band/chan info for TRM reservation. */

  hdrsrchrf_trm_freq_info_type          *freq_info_ptr;
    /* Pointer to frequency info to be passed to TRM. */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  rfm_device_enum_type                  rf_device;
    /* Rf device type */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT((channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS) &&
         (channel_cnt <= HDRSRCH_MAX_NUM_SUBASETS));
    /* Sanity check. */

  if ( ! hdrsrchrf_is_band_chan_supported( channel->band, channel->chan_num ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "Band-Chan not supported for the rf chain");
  
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_BAD_RF_CAL );
      /* Report to higher layer that the RF chain does not support 
      required band and channel */

    return FALSE;
      /* Exit tuning the RF when band is not supported */
  }

  memset(&tune_info, 0, sizeof(tune_info));
  tune_info.freq_info.band = channel[0].band;
  tune_info.freq_info.num_channels = channel_cnt;
  for( i=0; i<channel_cnt; i++)
  {
    tune_info.channel_list[i] = channel[i].chan_num;
  }
  hdrsrchrf_set_trm_freq_info( &tune_info );
    /* Convey band/chan info update for TRM interaction. */

  rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Grab hold of a pointer to the chain. */

  tune_state = rf_chain->tune.state;

#ifndef FEATURE_HDR_REVB
  debug_time.start_time_sclk = timetick_get();
    /* Save start time for debug */
#endif /* FEATURE_HDR_REVB */

  rf_chain->tune.done_cb        = tune_done;
    /* Save the callback function */

  /* Check wheather we should tune the radio. */
  if ( ( rf_chain->tune.state == HDRSRCHRF_RF_TUNED ) &&
     !hdrsrchrf_channel_reconfig_is_needed( channel_cnt, channel, demod_idx ) )
  {
    /* The logic here try to cover the case to avoid tune from channel change
       if the channel and its demod assignment are matched identically. For MC 
       TCA tunning, when reaches here, RF tune is always needed. */

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                    "Tune to Band=%d, Chan=%d Div=%d (No-op)",
                    rf_chain->tune.channel[0].band,
                    rf_chain->tune.channel[0].chan_num,
                    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.state == 
                    HDRSRCHRF_RF_TUNED );

    if ( !hdrsrchrf_enable_rf_chain( chain_id ) )
    {
      return FALSE;
    }

    hdrsrchrf_start_timer( chain_id, 0, HDRSRCHRF_LOOPS_SETTLING );
      /* the loops need 0us to settle, again since we didn't tune. */

  }
  else
  {
    /* If HDR didn't own the chain, always need to request band. Otherwise,
       check if band tune is needed. Assumption is that all channels are
       on the same band */
    if (rf_chain->tune.state == HDRSRCHRF_NO_TUNE_IN_PROGRESS ||
        rf_chain->tune.channel[0].band != channel[0].band)
    {
      rf_chain->tune.band_change.band_changing = TRUE;
      rf_chain->tune.band_change.band_granted = FALSE;
      rf_chain->tune.band_change.prev_state = rf_chain->tune.state;
  
      /* TuneSig shouldn't have been set at this stage, log and clear it */
      if ( HDRSRCH_SIG_GET() & rf_chain->tune.tune_sig )
      {
        /* REVISIT: remove err_fatal after initial testing */
        ERR_FATAL("TuneSig set before tune_to_chan", 0, 0, 0);
  
        HDR_MSG_SRCH(MSG_LEGACY_ERROR,
                     "TuneSig set before tune_to_chan");
      }
  
      freq_info_ptr = hdrsrchrf_get_trm_freq_info();
        /* Obtain band/chan information for TRM request. */
  
      band_granted = hdrsrchrf_request_frequency_tune( 
        HDRSRCHRF_CHAIN_TO_TRM(chain_id),
        freq_info_ptr->freq_info );
  
      if ( band_granted == TRM_PENDING )
      {
        rf_chain->tune.state = HDRSRCHRF_WAITING_BAND_GRANT;
      }

      while ( ( band_granted == TRM_PENDING ) && ( timeout > 0 ) )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
          "TRM Grant Pending, waiting for grant with timeout %d ms", 
          timeout );
  
        /* While waiting for band tune, there is a possibility of 
        HDR state is exited from current state to sleep state. 
        For these scenarios, RF chain is not disabled thinking 
        that it is not tuned, so restore this flag band grant
        pending scenarios. 
        */
        timeout = hdrsrch_timed_wait_with_sig_status( 
                    HDRSRCH_RFM_SIG, timeout);
  
        band_granted = hdrsrchrf_get_combined_grant(chain_id);
      }
  
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
        "TRM Grant status %d", 
        band_granted );
    
      if ( band_granted == TRM_GRANTED )
      {
        rf_chain->tune.band_change.band_granted = TRUE;

        if ( chain_id == HDRSRCHRF_PRI_CHAIN )
        {
          rfm_device_enum_type org_dev = hdrsrchrf.chain[chain_id].rf_chain;

          if ( hdrsrchrf.chain[chain_id].new_dev !=
               hdrsrchrf.chain[chain_id].rf_chain )
          {
            /* If PRI chain is assigned a new device, do RF clean up 
               on old device */
            hdrsrchrf_sleep_and_disable_pri( );

            hdrsrchrf.chain[chain_id].rf_chain = 
              hdrsrchrf.chain[chain_id].new_dev;            
          }

          if ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain &&
               ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].new_dev != 
                 hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].rf_chain ) )
          {
            /* Assumption: DIV/TX dev should've been disabled when we
               are tuning PRI */
            hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].rf_chain = 
              hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].new_dev;            
          }

          if ( !hdrsrchrf_enable_rf_chain( chain_id ) )
          {
            rf_chain->tune.state = rf_chain->tune.band_change.prev_state;

            if ( hdrsrchrf.chain[chain_id].rf_chain != org_dev )
            {
              hdrsrchrf.chain[chain_id].rf_chain = org_dev;

              if ( rf_chain->is_tuned )
              {
                hdrsrchrf_enable_rf_chain( chain_id );
              }
            }

            return FALSE;
          }
          else if ( hdrsrchrf.chain[chain_id].rf_chain != org_dev )
          {
            HDR_MSG_SRCH( MSG_LEGACY_MED, 
                          "Dev hopping complete, continue tune" );

            hdrsrchrf.chain[chain_id].new_dev = RFM_INVALID_DEVICE;
            hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].new_dev = RFM_INVALID_DEVICE;

            /* Jump to a different device, cannot restore tune state */
            rf_chain->tune.band_change.prev_state = HDRSRCHRF_NO_TUNE_IN_PROGRESS;
          }
        }
      }
      else
      {
        /* If Band denied or even if its pending tune to channel failed */
        if ( band_granted == TRM_PENDING )
        {
          hdrsrchrf_cancel_band_grant_request(HDRSRCHRF_PRI_CHAIN);
        }
  
        rf_chain->tune.state = rf_chain->tune.band_change.prev_state;
          /* Continue with the old band with previous state */
  
        if ( rf_chain->tune.done_cb != NULL )
        {
          rf_chain->tune.done_cb(HDRSRCHRF_TUNE_DENIED);
  
          rf_chain->tune.done_cb = NULL;
        }
  
        HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Band tune denied or band grant timed-out!!" );
        
        return FALSE;
      }
    }
    else
    {
      /* band is not changing, so AT can transition to TUNE_IN_PROGRESS */
      rf_chain->tune.band_change.band_changing = FALSE;
    }/* if (rf_chain->tune.state == HDRSRCHRF_NO_TUNE_IN_PROGRESS || */

    rf_chain->tune.channel_cnt = channel_cnt;
  
    for( i=0; i<channel_cnt; i++)
    {
      rf_chain->tune.channel[i] = channel[i];
      rf_chain->tune.demod_idx[i] = demod_idx[i];
    }  
  
      hdrsrchrf_band_granted(chain_id);
        /* If the band is granted or band is not changing then call band granted */
    }
  
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                  "RF Chain %d tune_to_chan: Prev %d Next %d",
                  rf_chain->rf_chain, tune_state, 
                  rf_chain->tune.state );
  return TRUE;

} /* hdrsrchrf_tune_to_chan( ) */  /*lint !e550 */


/*===========================================================================

FUNCTION HDRSRCHRF_PDM_CAPTURE_VALUES

DESCRIPTION
    Saves the TX open loop agc value. 

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_pdm_capture_values( void )
{

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.tx_open_loop = HDRAGC_GET_TX_OPEN_LOOP_MC( demod_idx );
    /* Capture Tx open loop value */

} /* hdrsrchrf_pdm_capture_values */


/*===========================================================================

FUNCTION HDRSRCHRF_GET_RX_POWER

DESCRIPTION
  This function stores rx power estimate in dbm read from the rx AGC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_get_rx_power
(
 int                            * rx0_dbm,
 int                            * rx1_dbm,
 boolean                        * diversity
)
{

  hdrsrchrf_logical_chain_t*      rf_chain;
    /* Pointer to the RF chain that requested the tune. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_chain = &( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN] );

#ifdef FEATURE_HDR_RUMI
  if ( hdrhit_rumi_cfg.y1y2_support )
  {
#endif /* FEATURE_HDR_RUMI */
  REX_DISABLE_PREMPTION();

  /* the clocks could be turned OFF after the clock check is successful but 
     before accessing the DSP registers. INTLOCKs are added to avoid that */

  if ( hdrmdsp_check_dsp_clk() )
  {
    if ( rf_chain->is_enabled )
    {
#ifdef FEATURE_HDR_REVB
       *rx0_dbm = rfm_hdr_get_rx_agc( rf_chain->rf_chain,
                                       RFM_HDR_CARRIER_ID__CUMULATIVE,
                                       RFM_HDR_AGC_FORMAT__DBM256);
#else
        *rx0_dbm = RF_HDR_GET_RX0_AGC_DB256();
#endif /* FEATURE_HDR_REVB */
          /* read AGC value in 1/256th of a dbm unit */

        HDRSRCHRF_CONVERT_AGC_TO_DB( *rx0_dbm );
          /* Convert to full dbm unit */
      }
      else
      {
        *rx0_dbm = HDRSRCHRF_RX_PWR_OFF;
          /* default is -128dbm if antenna is not ON */
      }

    rf_chain = &( hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN] );

    if ( rf_chain->is_enabled )
    {
#ifdef FEATURE_HDR_REVB
      *rx1_dbm = rfm_hdr_get_rx_agc( rf_chain->rf_chain,
                                     RFM_HDR_CARRIER_ID__CUMULATIVE,
                                     RFM_HDR_AGC_FORMAT__DBM256);
#else
      *rx1_dbm = RF_HDR_GET_RX1_AGC_DB256();
#endif /* FEATURE_HDR_REVB */
        /* read AGC value in 1/256th of a dbm unit */

      HDRSRCHRF_CONVERT_AGC_TO_DB( *rx1_dbm );
        /* Convert to full dbm unit */
    }
    else
    {
      *rx1_dbm = HDRSRCHRF_RX_PWR_OFF;
        /* default is -128dbm if antenna is not ON */
    }
  }
  else
  {
      *rx0_dbm = hdrsrchrf.rx0_dbm;
      *rx1_dbm = hdrsrchrf.rx1_dbm;
      /* Save the power values. */
    }

  REX_ENABLE_PREMPTION();
#ifdef FEATURE_HDR_RUMI
  } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

  *diversity = hdrsrchrf.chain[HDRSRCHRF_DIV_CHAIN].is_enabled; 
    /* return the diversity status as well */

} /* hdrsrchrf_get_rx_power ( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_REPORT_RX_POWER

DESCRIPTION
  This function reports the RX power and the current LNA state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_report_rx_power( void )
{
  int                             rx_agc0 = 0, rx_agc1 = 0, delta_agc = 0;

    /* Rx Strength */
  boolean                         diversity;
    /* diversity setting */

#ifdef FEATURE_HDR_REVB
  rfm_lna_gain_state_type         lna_state;
#else
  lna_gain_state_type             lna_state;
    /* LNA state   */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_REVB

  lna_state = rfm_hdr_get_lna_gain_state(
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain);
  /* Get current MCDO LNA state       */
#else
  lna_state = rf_get_lna_gain_state( );
    /* Get current LNA state       */
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_get_rx_power( &rx_agc0, &rx_agc1, &diversity );
    /* read the RF parameters and diversity settings */


    if(diversity == TRUE)
    {
       delta_agc = (rx_agc0 > rx_agc1) ? (rx_agc0 - rx_agc1) : (rx_agc1 - rx_agc0);
       
       if( delta_agc >HDRSRCHRF_RXAGC_DELTA_HIGH)
       {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_HIGH_RXLEV_DIFF_PRI_DIV);
#endif
       }
    }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Rx0=%d Rx1=%d (dbm) LNA state:%d", 
                  rx_agc0, rx_agc1, lna_state );

} /* hdrsrchrf_report_rx_power( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_REQUEST_TO_SLEEP

DESCRIPTION
  This function gives the RF module a chance to do any activities
  it needs to do to prepare for sleep.  For now that means stopping
  the AGC.
  
DEPENDENCIES
  This function must be called with FPD disabled

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
volatile uint16 hdrsrchrf_loop_count = 300;
volatile uint8  hdrsrchrf_loop_count_dummy_write = 0;

void hdrsrchrf_request_to_sleep( void )
{

  int loop_cnt = 0;

  hdrsrchrf_logical_chain_t* rf_chain;

  int ret_val;
    /* Return value from rf API */

#ifdef FEATURE_HDR_RUMI
  cfw_rx_stop_msg_t               stop_msg;  
#endif /* FEATURE_HDR_RUMI */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifndef FEATURE_HDR_REVB
    rfm_prepare_to_sleep( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN) );
      /* This function saves the DC offsets */

    rf_hdr_stop_agc( hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN) );
      /* Disable HDR AGC */ 
#else

    rf_chain = &hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN];
    
    if ( ( rf_chain->is_enabled == TRUE ) &&
         ( ( hdrsrchrf.rf_exec_wakeup_rx_pending == TRUE ) || 
           ( rf_chain->is_tuned == TRUE ) ) )
    {

      if ( ( hdrsrchrf.rf_exec_wakeup_rx_pending == TRUE ) &&
           ( rf_chain->is_tuned != TRUE ) )
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Reacq deactivate reproduced!" );
      }

#ifdef FEATURE_HDR_RUMI
      if ( !hdrhit_rumi_cfg.y1y2_support )
      {
        hdrsrchmsg_set_rsp_waiting( HDRSRCH_RX_STOP_RSP, TRUE );
          /* RUMI bring up: without waiting for RX_STOP_RSP, AT will crash. */

        memset((void *)&stop_msg, 0, sizeof(stop_msg));

        msgr_init_hdr( &stop_msg.hdr,
                       MSGR_HDR_SRCH,
                       HDR_FW_CMD_RX_STOP );
        stop_msg.immediate_trigger = 1;
        stop_msg.rx_config.settling_rtc_cx1 = 600;
        stop_msg.rx_config.rf_script_idx = -1;
        hdrsrchmsg_send_msg(&stop_msg, sizeof(stop_msg));

       ( void ) hdrsrch_wait_for_high_pri_rsp( HDRSRCHRF_RX_STOP_RSP_US,
                                               HDRSRCH_RX_STOP_RSP );
      }
      else
      {
#endif /* FEATURE_HDR_RUMI */
      ASSERT (rf_chain->rxlm_handle_valid == TRUE);

      ret_val = rfm_hdr_sleep_rx (
        rf_chain->rf_chain,      /* The receive path to put to sleep               */
        rf_chain->rxlm_handle,
                           /* The Rx Link Manager buffer to config modem HW  */
        hdrsrchrf_rfm_cb,  /* Called when wakeup sequence is complete        */
        NULL );  /* Data handle to pass additional info to the cb function   */
        /* Turn off the receiver. */

      hdrsrchrf_rfm_wait();
        /* Wait for signal from callback complete. */

      if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
      {
        hdrsrchrf_handle_unrecoverable_rf_err( HDRSRCHRF_PRI_CHAIN, ret_val );
          
        return;   
      }
      else if (ret_val < RFM_CDMA_HEALTHY_STATE )
      {
        /* Error fatal */
        ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                  ret_val, 0, 0);
      }
#ifdef FEATURE_HDR_RUMI
      } /* if !hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

      /* Wait for RxStop to be done ( RF does not wait currently ) */
      for(loop_cnt=0; loop_cnt<=hdrsrchrf_loop_count; loop_cnt++)
      {
        hdrsrchrf_loop_count_dummy_write = 1 - hdrsrchrf_loop_count_dummy_write;
      }
    }

#endif /* FEATURE_HDR_REVB */

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.state = HDRSRCHRF_NO_TUNE_IN_PROGRESS;
    /* since AGC has been stopped mark chain as not tuned */

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].is_tuned = FALSE;
    /* Mark chain as not tuned */

} /* hdrsrchrf_request_to_sleep( ) */

/*===========================================================================

FUNCTION HDRSRCHRF_PREP_TO_SLEEP

DESCRIPTION
  This function makes sure the mDSP is ACTIVE before putting RF to sleep.
  RF needs the mDSP in ACTIVE state for this activity.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_prep_to_sleep( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_RUMI
  hdrsrchstate_sram_check( FALSE );
#endif

  if ( hdrmdsp_check_dsp_clk( ) )
  {
    hdrsrchrf_request_to_sleep();  
  }

} /* hdrsrchrf_prep_to_sleep( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_PDM_RESTORE_VALUES

DESCRIPTION
    Restores the TX open loop agc value.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_pdm_restore_values( void )
{
#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT

  hdrsrch_demod_idx_type demod_idx = 
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx;
    /* Demod index for IDLE subaset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* TBD */
  /* This will be updated when RF interface is ready */
  HDRAGC_SET_TX_OPEN_LOOP_MC( demod_idx,
                              hdrsrchrf.tx_open_loop );
    /* Restore saved open loop value.  */
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

} /* hdrsrchrf_pdm_restore_values */


/*===========================================================================

FUNCTION HDRSRCHRF_CANCEL_IN_PROGRESS_TUNES

DESCRIPTION
  If any RF chains are in the process of being tuned, cancel the tune so 
  we can power down earlier.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_cancel_in_progress_tunes( void )
{

  hdrsrchrf_logical_chain_enum_t        chain_id;
    /* The logical chain ID associated with the request */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for ( chain_id = HDRSRCHRF_FIRST_CHAIN; 
                                  chain_id < HDRSRCHRF_NUM_CHAINS; chain_id++ )
  {
    hdrsrchrf_cancel_tune_timer( chain_id );
      /* Cancel the timer associated with this chain if it is active. */
  }

} /* hdrsrchrf_cancel_in_progress_tunes */


/*===========================================================================

FUNCTION HDRSRCHRF_GET_CHANNEL

DESCRIPTION
  This function returns the channel that the given RF chain is tuned to.

DEPENDENCIES
  None

RETURN VALUE
  A pointer to the channel structure that defines the channel.  This 
  structure shall not be modified by the caller.

SIDE EFFECTS
  None

===========================================================================*/

const sys_channel_type* hdrsrchrf_get_channel
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */

#ifdef FEATURE_HDR_REVB
  ,uint8                                *chan_cnt,
    /* The number of channel coverred by the current RF config */


  const hdrsrch_demod_idx_type          **demod_idx
    /* A pointor to the demod index assignment table. The structure
       shall not be modified by the caller. */
#endif /* FEATURE_HDR_REVB */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  *chan_cnt  = hdrsrchrf.chain[chain_id].tune.channel_cnt;
  *demod_idx = hdrsrchrf.chain[chain_id].tune.demod_idx;

  return hdrsrchrf.chain[chain_id].tune.channel;
#else
  return &hdrsrchrf.chain[chain_id].tune.channel;
#endif /* FEATURE_HDR_REVB */

} /* hdrsrchrf_get_channel */


/*=========================================================================

FUNCTION HDRSRCHRF_SET_AGC_ACQ_PARAMS

DESCRIPTION
  This function sets the RF AGC acquisition parameters. Note that the AGC
  settings chosen are only persistent for the next tune.

DEPENDENCIES 
  None

RETURN VALUE 
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrf_set_agc_acq_params
( 
  hdrsrchrf_agc_acq_mode_enum_type  agc_acq_mode,
    /* Mode for AGC acquisition: Normal, Fast, or Resume */

  uint32                            agc_rtc_offset,
    /* RTC offset to use for fast and resume AGC acquisition mode */

  int16                             resume_agc
    /* AGC used to start from when using the resume acquisition algorithm */)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Save AGC acquistion parameters 
     since they will be used during tuning */
  hdrsrchrf.agc_acq_mode   = agc_acq_mode;
  hdrsrchrf.agc_rtc_offset = agc_rtc_offset;
  hdrsrchrf.agc_value      = resume_agc;

  if ( HDRSRCH_NORMAL_AGC_ACQ_NV_ENABLE )
  {
    hdrsrchrf.agc_acq_mode = HDRSRCHRF_NORMAL_AGC_ACQ_MODE;
      /* If fast AGC algorithm is not enabled by NV item 
         always use normal AGC acquisition */
  }

  /* Set AGC loop settling time according to mode */
  if (hdrsrchrf.agc_acq_mode == HDRSRCHRF_NORMAL_AGC_ACQ_MODE) 
  {
    hdrsrchrf.loop_settling_time = HDRSRCHRF_ACQ_LOOPS_SETTLING_TIME_HS;
      /* set the loop settling time for normal AGC acquisition */
  }
  else
  {
    hdrsrchrf.loop_settling_time = HDRSRCHRF_FAST_LOOPS_SETTLING_TIME_HS;
      /* set the loop settling time for fast AGC acquisition */

    if ( HDRSRCH_FAST_AGC_ACQ_NV_ENABLE )
    {
      hdrsrchrf.loop_settling_time = HDRSRCH_NV_FAST_AGC_SETTLING_TIME_HS;
        /* Set the loop settling time to value provided in HDR L1 dbg mask NV item */

      hdrsrchrf.agc_acq_mode       = HDRSRCHRF_FAST_AGC_ACQ_MODE;
        /* Force the mode to FAST. */
    }
    else if ( HDRSRCH_RESUME_AGC_ACQ_NV_ENABLE )
    {
      hdrsrchrf.loop_settling_time = HDRSRCH_NV_RESUME_AGC_SETTLING_TIME_HS;
        /* Set the loop settling time to value provided in HDR L1 dbg mask NV item */

      hdrsrchrf.agc_acq_mode       = HDRSRCHRF_RESUME_AGC_ACQ_MODE;
        /* Force the mode to RESUME. */
    }
    else if ( HDRSRCH_DAST_AGC_ACQ_NV_ENABLE )
    {
      /* If the resume algorithm, choose resume. Otherwise, default to FAST. */
      if ( hdrsrchrf.agc_acq_mode == HDRSRCHRF_RESUME_AGC_ACQ_MODE )
      {
        hdrsrchrf.loop_settling_time = HDRSRCH_NV_RESUME_AGC_SETTLING_TIME_HS;
          /* Set the resume loop settling time to value provided in HDR L1 dbg mask 
             NV item */
      }
      else
      {
        hdrsrchrf.loop_settling_time = HDRSRCH_NV_FAST_AGC_SETTLING_TIME_HS;
          /* Set the fast loop settling time to value provided in HDR L1 dbg mask 
             NV item */
      }
    }
  }

} /* hdrsrchrf_set_agc_acq_params */


/*=========================================================================

FUNCTION HDRSRCHRF_RESET_AGC_EST

DESCRIPTION 
  This function resets the AGC stability channel estimator.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrf_reset_agc_est( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrf.agc_est.channel.chan_num = SYS_INVALID_CHANNEL;
    /* Set the channel to an invalid channel to invalidate the AGC
       estimator. */

  hdrsrchrf.agc_est.is_stable_chan = FALSE;
    /* Set the channel to be unstable. */

} /* hdrsrchrf_init_agc_est */


/*=========================================================================

FUNCTION HDRSRCHRF_EST_AGC_STABILITY

DESCRIPTION 
  This function estimates the stability of the AGC.  It should
  be invoked at the end of every tracking session if the
  AGC resume acquisition algorithm is desired for the next
  time the RF is tuned to the same channel.

DEPENDENCIES
  The radio must be tuned with AGC already settled when called.

RETURN VALUE
  Whether the AGC estimate determines the AGC resume acquisition algorithm
  is appropriate for this channel.
  
SIDE EFFECTS
  Alters the agc_est structure with the information related to this past
  AGC tracking interval.

=========================================================================*/

boolean hdrsrchrf_est_agc_stability
( 
  hdrsrchrf_logical_chain_enum_t    chain_id
    /* The logical chain ID associated with the request */
)
{
  rfm_hdr_agc_info_struct_type      agc_info;
    /* Used to capture the latest AGC tracking statistics */

  hdrsrchrf_time_t                  cur_time_sclk;
    /* The last time in an SCLK timestamp (wraps every 18 hours) which 
       AGC information was captured. */

  uint16                            abs_agc_delta;
    /* Largest delta between the AGC of the last tracking interval and 
       any AGC value of the current interval */

  uint8                             overshoot_percentage = 0;
    /* The percentage of tracking intervals in which the AGC varied 
       beyond the threshold compared with the previous interval. */

  uint8                             i = 0;
    /* Counter used to enumerate all overshoot instances. */

  hdrsrchrf_time_t                  delta_time_sclk;
    /* The time delta in sclks from the last AGC estimate. */

  unsigned                          delta_time_ms;
    /* The time delta in ms from the last AGC estimate. */

  hdrsrchrf_agc_est_struct_type    *agc_est = &hdrsrchrf.agc_est;
    /* The AGC stability estimator information which is updated based
       on the current tracking interval. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  agc_info.max_agc = agc_info.min_agc = agc_info.last_agc = 0;
  
  rfm_hdr_capture_agc( hdrsrchrf.chain[chain_id].rf_chain, &agc_info );
    /* Capture information about the AGC's behavior over the last
       tracking period. */

  cur_time_sclk = timetick_get();
    /* Capture an sclk timestamp */

  /* If the radio has been retuned than reset the channel estimator. */
#ifdef FEATURE_HDR_REVB
  ASSERT( hdrsrchrf.chain[chain_id].tune.channel_cnt == 1 );
    /* only appies to idle state sleep. */

  if ( !SYS_CHAN_CMP_EQUAL( agc_est->channel, 
                            hdrsrchrf.chain[chain_id].tune.channel[0] ) )
#else
  if ( !SYS_CHAN_CMP_EQUAL( agc_est->channel, 
                            hdrsrchrf.chain[chain_id].tune.channel ) )
#endif /* FEATURE_HDR_REVB*/
  {
    memset( agc_est, 0, sizeof( hdrsrchrf_agc_est_struct_type ) );
      /* Clear out the structure. */

#ifdef FEATURE_HDR_REVB
    /* Save the channel and band that the channel estimate will be for. */
    agc_est->channel.band = hdrsrchrf.chain[chain_id].tune.channel[0].band;
    agc_est->channel.chan_num = 
                        hdrsrchrf.chain[chain_id].tune.channel[0].chan_num;
#else
    /* Save the channel and band that the channel estimate will be for. */
    agc_est->channel.band = hdrsrchrf.chain[chain_id].tune.channel.band;
    agc_est->channel.chan_num = 
                               hdrsrchrf.chain[chain_id].tune.channel.chan_num;
#endif /* FEATURE_HDR_REVB */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "DAST: Resetting AGC channel estimator" );

  }
  else
  {
    abs_agc_delta = (uint16) MAX( 
                         LABS_VAL(agc_info.max_agc - agc_est->prev_agc ), 
                         LABS_VAL(agc_info.min_agc - agc_est->prev_agc ) );
      /* Determine the largest delta between the AGC of the last tracking
         interval and any AGC value of the current interval. */

    delta_time_sclk = (uint32) HDRSRCH_NAT_NUM( (int32)( cur_time_sclk - 
                                                agc_est->prev_time ) );
      /* Calculate the time since the last stability estimate */

    delta_time_ms = timetick_cvt_from_sclk( delta_time_sclk, T_MSEC );
      /* Convert delta time to milliseconds. */

    /* Make sure the last sample was not too far away in the past */
    if ( ( delta_time_sclk < HDRSRCHRF_MAX_AGC_EST_PERIOD_SCLK ) &&
         ( delta_time_sclk > HDRSRCHRF_MIN_AGC_EST_PERIOD_SCLK ) )
    {
      if ( abs_agc_delta > HDRSRCHRF_MAX_AGC_FLUCTUATION ) 
      {
        agc_est->overshoot_inst[ agc_est->overshoot_idx ] = 1;
          /* The AGC overshot the maximum threshold. */

      }
      else
      {
        agc_est->overshoot_inst[ agc_est->overshoot_idx ] = 0;
          /* The AGC did not overshoot the maximum threshold. */
      }

      agc_est->overshoot_idx = ( agc_est->overshoot_idx + 1) % 
                               HDRSRCHRF_OVERSHOOT_LEN;
        /* Keep track of where to the next overshoot instance. */
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DAST: Skipping measurement due to time: %l",
                      delta_time_ms );
    }

    if ( agc_est->force_resume_agc_off == TRUE )
    {
      agc_est->is_stable_chan = FALSE;
        /* Indicate that the channel is no longer stable. */

      agc_est->force_resume_agc_off = FALSE;
        /* Force only lasts for one wakeup. */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DAST: AGC Resume forced off, delta_t: %d",
                      delta_time_ms );
    }
    else
    {
      /* Determine the overall percentage of time the AGC has fluctuated too 
         much. */
      for ( i = 0; i < HDRSRCHRF_OVERSHOOT_LEN; i++ )
      {
        overshoot_percentage += agc_est->overshoot_inst[i];
          /* Accumulate the overshoot instances. */
      }

      if ( ( agc_est->is_stable_chan ) && 
           ( overshoot_percentage > HDRSRCHRF_EXIT_RESUME_THRESH ) )
      {
        agc_est->is_stable_chan = FALSE;
          /* Rx AGC variation is outside the desired limits */
      }
      else if ( ( !agc_est->is_stable_chan ) && 
                ( overshoot_percentage <= HDRSRCHRF_ENTRY_RESUME_THRESH ) )
      {
        agc_est->is_stable_chan = TRUE; 
          /* Rx AGC variation is within the desired limits */
      }

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "DAST: Stable Chan: %d, Overshoot: %d%% delta_t: %d",
                      agc_est->is_stable_chan,
                      overshoot_percentage,
                      delta_time_ms );
    }
  }
  

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DAST: AGC Prev: %d, Cur: %d, Min: %d, Max: %d",
                  agc_est->prev_agc,
                  agc_info.last_agc,
                  agc_info.min_agc,
                  agc_info.max_agc );

  agc_est->prev_agc = agc_info.last_agc;
    /* Save the last AGC value. */

  agc_est->prev_time = cur_time_sclk;
    /* Save an sclk timestamp */

  if ( HDRSRCH_RESUME_AGC_ACQ_NV_ENABLE )
  {
    return TRUE;
      /* Force the channel to be stable if RESUME is turned on */
  }
  else if ( !HDRSRCH_DAST_AGC_ACQ_NV_ENABLE )
  {
    return FALSE;
      /* Force the channel to be unstable if DAST is turned off */
  }
  else
  {
    return agc_est->is_stable_chan;
  }

} /* hdrsrchrf_est_agc_stability */


/*=========================================================================

FUNCTION HDRSRCHRF_FORCE_RESUME_AGC_OFF

DESCRIPTION 
  This function indicates that for reasons other than the AGC estimator
  can determine, the resume AGC acquisition algorithm shouldn't be used
  for the next wakeup.

DEPENDENCIES
  This function is latched during hdrsrchrf_est_agc_stability and thus
  must be called prior to take affect.

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrf_force_resume_agc_off( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrf.agc_est.force_resume_agc_off = TRUE;
    /* Indicate that the resume AGC acquisition algorithm shouldn't be used
       for the next wakeup. */

} /* hdrsrchrf_force_resume_agc_off */


/*=========================================================================

FUNCTION HDRSRCHRF_GET_PREV_AGC

DESCRIPTION 
  This function returns the last good AGC value seen while awake on 
  the same channel.

DEPENDENCIES
  hdrsrchrf_est_agc_stability must be called before going to sleep
  so that the hdrsrchrf can capture the last AGC.

RETURN VALUE
  The AGC value that was last captured by hdrsrchrf_est_agc_stability.

SIDE EFFECTS
  None

=========================================================================*/

int16 hdrsrchrf_get_prev_agc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchrf.agc_est.prev_agc;
    /* Return the last captured good AGC value */

} /* hdrsrchrf_get_prev_agc */


/*=========================================================================

FUNCTION HDRSRCHRF_AGC_IS_STABLE

DESCRIPTION 
  This function returns whether the he AGC is stable enough
  to use the resume acquisition algorithm for DAST.

DEPENDENCIES
  hdrsrchrf_est_agc_stability must be called before going to sleep
  so that the hdrsrchrf can measure the stability.

RETURN VALUE
  Whether the AGC estimate determined that the AGC is stable enough
  to use the resume acquisition algorithm for DAST.
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_agc_is_stable( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( HDRSRCH_RESUME_AGC_ACQ_NV_ENABLE )
  {
    return TRUE;
      /* Force the channel to be stable if RESUME is turned on */
  }
  else if ( !HDRSRCH_DAST_AGC_ACQ_NV_ENABLE )
  {
    return FALSE;
      /* Force the channel to be unstable if DAST is turned off */
  }
  else
  {
    return hdrsrchrf.agc_est.is_stable_chan;
      /* Return whether the channel is stable enough to use the resume
         AGC acquisition algorithm */
  }

} /* hdrsrchrf_agc_is_stable */


/*=========================================================================

FUNCTION HDRSRCHRF_IS_TUNED

DESCRIPTION 
  This function returns if the primary chain state is HDRSRCHRF_RF_TUNED.

DEPENDENCIES
  None

RETURN VALUE
  True  - If the primary chain is HDRSRCHRF_RF_TUNED.
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_is_tuned( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.state == 
                                                       HDRSRCHRF_RF_TUNED );

} /* hdrsrchrf_is_tuned */

/*=========================================================================

FUNCTION HDRSRCHRF_HAS_CHAIN

DESCRIPTION 
  This function returns if the HDR has chain or not

DEPENDENCIES
  None

RETURN VALUE
  True  - If the primary chain is HDRSRCHRF_CHAIN is locked to HDR..
  False - Otherwise. 
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_has_chain( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return hdrsrchrf_is_tuned( );

} /* hdrsrchrf_has_chain */

#ifdef FEATURE_HDR_REVB
/*=========================================================================

FUNCTION HDRSRCHRF_CHANNEL_RECONFIG_IS_NEEDED

DESCRIPTION 
  The function will check if rf reconfiguration is needed given a channel
  list and its corresponding demod assignment list.

  The function only supports the case that the channel list has differnt 
  channels and different demod assignments and this rule is enforced at the 
  RUP protocol's TCA checking.

DEPENDENCIES
  None

RETURN VALUE
  boolean - if RF reconfiguration (tune ) is needed given the new channel
            list and its demod assignments.
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_channel_reconfig_is_needed
( 
  uint8                         chan_cnt, 
    /* channel count */

  const sys_channel_type*       chan_list,
    /* Channel list */

  const hdrsrch_demod_idx_type* demod_list
    /* Demod assignment list */
)
{
  boolean                        reconfig_needed=FALSE;
    /* Return value */

  hdrsrchrf_tune_struct_type*    tune_ptr = 
                               &hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune;
    /* Convinient pointor to the current tune structure */

  uint8                          i, j;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( tune_ptr->channel_cnt != chan_cnt )
  {
    reconfig_needed = TRUE; 
  }
  else 
  {
    for( i=0; i<chan_cnt; i++ )
    {
      for( j=0; j<tune_ptr->channel_cnt; j++ )
      {
        if( SYS_CHAN_CMP_EQUAL( chan_list[i], tune_ptr->channel[j] ) &&
            ( demod_list[i] == tune_ptr->demod_idx[j] ) )
        {
          /* A channel matched to one of the current channels as well as 
             the demod index assignment. */
          break;
        }
      } /* for j*/

      /* The logic here is if no channel combined its demod assignmet has been 
         found in the current RF tuning table, then a tune is a must. variable 
         j is used in the following 'if' block to indicate the searching has 
         gone though the entire table */

      if( j == tune_ptr->channel_cnt )
      {
        /* A new channel or new demod index assignment */
        reconfig_needed = TRUE; 
        break;
      } /* if j */
    } /* for i */
  } /* if tune_ptr */

  return reconfig_needed;
} /* boolean hdrsrchrf_channel_reconfig_is_needed */


/*=========================================================================

FUNCTION HDRSRCHRF_GET_CARRIER_IDX

DESCRIPTION 
  This function return the carrier (CSR/SRAM) index assigned to the given
  channel.

DEPENDENCIES
  None

RETURN VALUE
  boolean - if a valid channel under the current RF configuration
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_get_carrier_idx
( 
  sys_channel_type channel,
    /* Channel to query*/

  uint8            *csr_idx
    /* The carrier index to return  */
)
{

  boolean csr_configured = FALSE;
    /* Init value assumes the channel is not configured */

  int8    i;
    /* Loop index and CSR index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for( i=0; i<HDRSRCH_MAX_NUM_CARRIERS; i++ )
  {
    if( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].enabled &&
        SYS_CHAN_CMP_EQUAL( channel, 
          hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].channel ) )
    {
      csr_configured = TRUE;
        /* The channel is covered by the current RF config */

      *csr_idx    = i;
        /* Fill the csr index */
    }
  }

  return csr_configured;
} /* boolean hdrsrchrf_get_carrier_idx */


/*=========================================================================

FUNCTION HDRSRCHRF_ENTER_MCDO

DESCRIPTION 
  This function makes RF module enter the MC DO mode. The function should
  be called everytime the HDR enters DO mode from other technology like
  hybrid tune away, wakeup from sleep, etc.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  RF module enter the MC DO mode.

=========================================================================*/

void hdrsrchrf_enter_mcdo( void )
{
  int ret_val;
    /* Return val from rf tune function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_RUMI
  if ( hdrhit_rumi_cfg.y1y2_support )
  {
#endif /* FEATURE_HDR_RUMI */
    ret_val = rfm_enter_mode( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain,
                           (rfcom_mode_enum_type)RFM_1XEVDO_MODE,
                           NULL,
                           hdrsrchrf_rfm_cb 
#ifdef FEATURE_RF_COMMON_LM_RFM_INTERFACE 
                          , hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain
#endif
                         );

    hdrsrchrf_rfm_wait();
      /* Wait for rf task to complete. */

    if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
    {
      hdrsrchrf_handle_unrecoverable_rf_err( HDRSRCHRF_PRI_CHAIN, ret_val );
      
      return;   
    }
    else if (ret_val < RFM_CDMA_HEALTHY_STATE )
    {
      /* Error fatal */
      ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                ret_val, 0, 0);
    }

#ifdef FEATURE_HDR_RUMI
  } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */

    /* REVISIT: Do we need to register an enable/disable function with TRM here? */
}
#endif /* FEATURE_HDR_REVB */


/*=========================================================================

FUNCTION HDRSRCHRF_RF_CHAIN_TUNE_DONE

DESCRIPTION 
  This function checks if the RF tuning is done on the given chain.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If tuning is done on the given chain.
  FALSE - otherwise.
  
SIDE EFFECTS
  None.

=========================================================================*/

boolean hdrsrchrf_rf_chain_tune_done ( hdrsrchrf_logical_chain_enum_t chain_id )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return (hdrsrchrf.chain[chain_id].tune.state == HDRSRCHRF_RF_TUNED);

} /* hdrsrchrf_rf_chain_tune_done */


/*============================================================================

FUNCTION HDRSRCHRF_GET_BAND_SUBCLASS

DESCRIPTION
  This function gets the currently supported band subclasses of the given band.
  
DEPENDENCIES
  None
  
RETURN VALUE
  The mask of the current subclasses of the given band.

SIDE EFFECTS
  None

============================================================================*/
uint32 hdrsrchrf_get_band_subclass
(
  uint8    band_class
    /* Band Class */
)
{
  uint32 band_sc_mask;
    /* The mask representing the current subclasses of the given band */
/*--------------------------------------------------------------------------*/

  band_sc_mask = rfm_cdma_get_sub_class( hdrsrchrf_get_last_valid_rf_dev(), 
                   (sys_band_class_e_type) band_class );
    /* Get the currently supported band subclasses of the given band */

  return band_sc_mask;
} /* hdrsrchrf_get_band_subclass */

#ifdef FEATURE_HDR_LTE_TO_EHRPD_IRAT
/*=========================================================================

FUNCTION HDRSRCHRF_MEAS_BUILD_SCRIPT

DESCRIPTION 
  This function calls rf APIs to build scripts for IRAT pilot
  measurement.

DEPENDENCIES
  LTE has to call hdrl1_lte_meas_enter() first

RETURN VALUE
  rfm_meas_result_type from RF pilot measurement script 
 
SIDE EFFECTS
  None

=========================================================================*/
rfm_meas_result_type hdrsrchrf_meas_build_script
(
  rfm_meas_common_param_type                       *rf_params,
    /* Common RF parameters for build script */

  boolean                                           is_idle_mode
    /* whether it is connected mode measurement */
)
{
   rfm_meas_setup_param_type      rf_script_param;
     /* parameter structure for rf meas API */

   rfm_meas_result_type           rf_status;
     /* RF API return status */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* call RF API to do setup and build RF scripts. No need for callback */

   rf_script_param.setup_params = rf_params;
   rf_status = rfm_meas_common_script_build_scripts(
                   &rf_script_param,
                   NULL) ;
   return rf_status; 
     /* Return the rf function call status */
} /* hdrsrchrf_meas_setup_and_build_script */
#endif /* FEATURE_HDR_LTE_TO_EHRPD_IRAT */

/*===========================================================================

FUNCTION HDRSRCHRF_FORCE_TUNE_TO_CHAN

DESCRIPTION
  This function set up internal data structure and sample server as if RF
  is tuned. It is used for idle mode RF MEAS API work around.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_force_tune_to_chan
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */
 
  const sys_channel_type                *channel,
    /* Channel to tune the RF to */

#ifdef FEATURE_HDR_REVB
  const hdrsrch_demod_idx_type          *demod_idx,
    /* Demod index assignment matching to the channel assignment*/

  uint8                                 channel_cnt,
#endif /* FEATURE_HDR_REVB */

  hdrsrchrf_tune_done_cb_t              tune_done
    /* Callback when tuning is complete */
)
{
  hdrsrchrf_logical_chain_t*            rf_chain;
    /* Pointer to the RF chain that requested the tune. */

  uint8                                 i;
    /* Loop index */

  int32                                 csrs_in_use[HDRSRCH_MAX_NUM_CARRIERS];
    /* Temporary list to hold indexes of CSRs in use */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_chain = &( hdrsrchrf.chain[chain_id] );
    /* Grab hold of a pointer to the chain. */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Force RF Tune. Switch SRAM Source");

  /* Make sure we own the chain and that this isn't the diversity chain. */
  if ( ( chain_id != HDRSRCHRF_DIV_CHAIN ) && 
       ( rf_chain->is_enabled == FALSE ) &&
       ( rf_chain->owns_chain == TRUE ) )
  {
#ifdef FEATURE_MULTIPLE_MODEM_CORES
    /* REVISIT: configure diversity chain when diversity is enabled */
    if (hw_set_system_mode(HW_SYSTEM_MODE_CDMA,HW_MODEM_CHAIN_0) == FALSE)
    {
       HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Could not set system mode to HDR on chain 0");
    }

#endif /* FEATURE_MULTIPLE_MODEM_CORES */

    if ( chain_id == HDRSRCHRF_PRI_CHAIN )
    {
      hdrsrchrf_pdm_restore_values();
        /* Restore the PDM Values for HDR channel */
    }

  }

  rf_chain->tune.done_cb        = tune_done;
    /* Save the callback function */

  rf_chain->tune.state = HDRSRCHRF_RF_TUNED;

  rf_chain->tune.channel_cnt = channel_cnt;
    /* Save the channel tuning information. */

  for( i=0; i<channel_cnt; i++)
  {
    rf_chain->tune.channel[i] = channel[i];
    rf_chain->tune.demod_idx[i] = demod_idx[i];
  }

     
  hdrsrchrf_assign_csrs( csrs_in_use );
    /* Assign csrs based on the current rf configuration */

  hdrsrchrf_assign_sample_servers( chain_id );
    /* Setup the configuration of the sample servers on the primary chain to 
       match to the configuration of the CSRs */

} /* hdrsrchrf_force_tune_to_chan( ) */


/*=========================================================================

FUNCTION HDRSRCHRF_FORCE_CHAIN_LOCK

DESCRIPTION 
  This function uses the device provided by LTE

DEPENDENCIES
  None

RETURN VALUE
  TRUE   - if lock is granted
  FALSE  - otherwise
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_force_chain_lock
(
  hdrsrchrf_reason_enum_t            reason,
    /* The reason the lock is being held */

  rfm_device_enum_type               rf_chain
    /* The RF physical chain mapped into the logical chain. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "force_chain_lock() uses RF device provided by LTE %d",
                  rf_chain );

  if ( rf_chain != TRM_NO_DEVICE )
  {
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain = TRUE;
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rf_chain = rf_chain;
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].reason = reason;
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].sub_reason = hdrsrchrf_get_sub_reason(reason);
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* hdrsrchrf_force_chain_lock */

/*=========================================================================

FUNCTION HDRSRCHRF_FORCE_CHAIN_UNLOCK

DESCRIPTION 
  This function force hdrsrchrf not having rf chain lock

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchrf_force_chain_unlock( void )
{   
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain = FALSE;
    /* The client now does not own the specified chain. */   
} /* hdrsrchrf_force_chain_unlock */

/*=========================================================================

FUNCTION HDRSRCHRF_DISABLE_RX

DESCRIPTION 
  This function disables rx.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchrf_disable_rx
(
  hdrsrchrf_logical_chain_enum_t        chain_id
    /* The logical chain ID associated with the request */
)
{
  rfm_device_enum_type                rf_chain;
    /* RF device */

  int ret_val;
    /* return value from rfm API */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  rf_chain = hdrsrchrf.chain[chain_id].rf_chain;

  if ( hdrsrchrf.chain[chain_id].is_enabled == TRUE )
  {
#ifdef FEATURE_HDR_RUMI
    if ( hdrhit_rumi_cfg.y1y2_support )
    {
#endif /* FEATURE_HDR_RUMI */
    ASSERT (hdrsrchrf.chain[chain_id].rxlm_handle_valid == TRUE);


    ret_val = rfm_hdr_disable(
      rf_chain,          /* The receive path to disable                   */
      hdrsrchrf.chain[chain_id].rxlm_handle,
                         /* The Rx Link Manager buffer to config modem HW  */
      hdrsrchrf_rfm_cb,  /* Called when wakeup sequence is complete        */
      NULL );  /* Data handle to pass additional info to the cb function   */

    if( ret_val == RFM_CDMA_ERROR_BAD_CONFIG )    
    {
      /* Bad RF Cal */
      HDR_MSG_SRCH ( MSG_LEGACY_ERROR,"Bad RF cal!! Declaring system loss ");

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_BAD_RF_CAL );    

      return;
    }
    else if (ret_val < RFM_CDMA_HEALTHY_STATE )
    {
      /* Error fatal */
      ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                ret_val, 0, 0);
    }
    hdrsrchrf_rfm_wait();
      /* Wait for signal from callback complete. */

    hdrsrchrf.chain[chain_id].is_enabled = FALSE;   
      /*  Mark not enabled, just been disabled. */

    hdrsrchrf.chain[chain_id].is_tuned = FALSE;   
      /*  When chain is disabled it needs to be tuned again */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "Enable/Tune state: %d/%d",
                    hdrsrchrf.chain[chain_id].is_enabled,
                    hdrsrchrf.chain[chain_id].is_tuned);

    /* De-allocate buffer and check result */
    if ( hdrsrchrf.chain[chain_id].rxlm_handle_valid &&
         ( rxlm_deallocate_buffer( hdrsrchrf.chain[chain_id].rxlm_handle ) != 
           LM_SUCCESS ) )
    {
      ERR ("HDR RF Disable: Unable to deallocate RxLM buffer (device %d)", 
             rf_chain, 0, 0 );
         /* Do not call hdrsrchrf_handle_unrecoverable_rf_err(). It will
            cause a loop. Just print ERR */
    }
#ifdef FEATURE_HDR_RUMI
  } /* if hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */
  }

  hdrsrchrf.chain[chain_id].rxlm_handle_valid = FALSE;
        /* Mark handle as no longer valid */

} /* hdrsrchrf_disable_rx() */


/*=========================================================================

FUNCTION HDRSRCHRF_FORCE_DEMOD_CARRIER_ZERO

DESCRIPTION 
  This function forces primary chain to be on demod carrier 0.

DEPENDENCIES
  This function is only called when AT changes state from 4a->3a, where
  no RF available.

RETURN VALUE
  None
  
SIDE EFFECTS
  It will change the hdrsrchrf structure only but no actual tuning.

=========================================================================*/
void hdrsrchrf_force_demod_carrier_zero ( void )
{
  uint8    i;
    /* Loop index */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel_cnt = 1;
      /* Set the channel count */

    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.channel[0] = 
                         hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel;
      /* Set the channel to be the one in subaset[0] */

    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[0].channel = 
                         hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel;
      /* Update the CSR/SRAM info */

    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[0].enabled = TRUE;
      /* Set the flag */

    for ( i = 1; i < HDRSRCH_MAX_NUM_CARRIERS; i++ )
    {
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.csr[i].enabled = FALSE;
        /* Disable the csr for other carriers */
    }

    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].tune.demod_idx[0] = HDRSRCH_DEMOD_IDX_0;
      /* Set the demod index */

} /* hdrsrchrf_force_demod_carrier_zero */


/*=========================================================================

FUNCTION HDRSRCHRF_GET_PRI_RXLM_BUFFER_HANDLE

DESCRIPTION 
  This function returns the primary chain RXLM buffer DO is using if the
  buffer has been allocated.

DEPENDENCIES
  None

RETURN VALUE
  Allocation status and buffer handle
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_get_rxlm_buffer_handle
( 
  hdrsrchrf_logical_chain_enum_t chain_id,
  lm_handle_type            *rxlm_buf
    /* The RXLM buffer handle */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Grab buffer if we do not have one. */
  ASSERT(chain_id < HDRSRCHRF_NUM_CHAINS);

  if ( hdrsrchrf.chain[chain_id].rxlm_handle_valid )
  {
    *rxlm_buf = hdrsrchrf.chain[chain_id].rxlm_handle;
  }
  else
  {
    *rxlm_buf = 0;
  }

  return hdrsrchrf.chain[chain_id].rxlm_handle_valid;

} /* hdrsrchrf_get_pri_rxlm_buffer_handle */


/*==========================================================================

FUNCTION        HDRSRCHRF_GET_TX_POWER_INFO

DESCRIPTION
  Returns HDR tx power to the caller in traffic state.
  It returns tx power in 1/10 dBm unit.
  
    
DEPENDENCIES
  

INPUTS
  None

RETURN VALUE 
 hdrsrch_tx_pwr_info_struct_type 
 

SIDE EFFECTS
  none

==========================================================================*/

void hdrsrchrf_get_tx_power_info
(
  hdrsrch_tx_pwr_info_struct_type *tx_pwr_info
)
{
  int32 total_power;
  /* The total cumulative Tx power in db256 units on success */

  rfm_hdr_tx_agc_state_param_type tx_agc_state_param; 
  /*  Variable to hold the current Tx AGC State parameter */

  rfm_device_enum_type rf_dev;
  /* To specify which RF device in the system */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Intialize tx_pwr_info with invalid state Values */
  tx_pwr_info->is_in_traffic = FALSE; 
  tx_pwr_info->tx_pwr = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
  tx_pwr_info->pa_gain_state = HDRSRCH_PA_STATE_INFO_INVALID_VALUE;

  if( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
  {
    tx_pwr_info->is_in_traffic = TRUE;
    
    if ( (rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN))== RFM_INVALID_DEVICE )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,"Received invalid RF device value = (%d)",rf_dev );
      return;
    }
    if ( (rfm_hdr_get_tx_agc_state_parameter(rf_dev, &tx_agc_state_param)) != RFM_CDMA_HEALTHY_STATE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,"RF CDMA NOT in a healty state" );
      return;
    }
    else
    {
      total_power = tx_agc_state_param.total_power;
      total_power += (total_power<0)?-128:128;    /* For rounding off */ 
      tx_pwr_info->tx_pwr = (total_power/256)*10; /*convert it to 1/10 dbm */
      tx_pwr_info->pa_gain_state = tx_agc_state_param.pa_state;
    }
  }
  
} /* hdrsrchrf_get_tx_power_info */


/*==========================================================================

FUNCTION        HDRSRCHRF_GET_RX_POWER_INFO

DESCRIPTION
  Returns HDR RX power info to the caller. It returns the rx agc per rx chain
  in 1/10 dBm unit, and ecio (instantaneous primary in traffic and combined in
  idle state) in negative 1/10 dBm unit.
    
DEPENDENCIES
  

INPUTS
  None

RETURN VALUE 
 hdrsrch_rx_pwr_info_struct_type
 If any value is invaled 0x80000000 is returned for that value.
 

SIDE EFFECTS
  none

==========================================================================*/

void hdrsrchrf_get_rx_power_info
(
  hdrsrch_rx_pwr_info_struct_type *rx_pwr_info
)
{
  int                i;
    /* loop variables/temps */                                                                                            

  int                ecio = 0;
    /* ecio of the strongest hdr pilot */

  int                rx0_dbm = 0, rx1_dbm = 0; 
    /* received power read from the AGCs */

  boolean            diversity;
    /* diversity settings */

  int8               subaset_idx;
    /* Ref subaset index */

  //int16              *ec_io = NULL;   
    /* pointer to EcIo value */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



  /* If in sleep or OFS, radio is not tuned */
  if( hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE ||
      hdrsrch_state_id == HDRSRCH_OFS_IDLE_STATE ||
       hdrsrch_state_id == HDRSRCH_SLEEP_STATE )
  {
    rx_pwr_info->rx_diversity_indicator = HDRSRCH_RX_CHAIN_UNAVAIL;
    rx_pwr_info->rx_pwr_chain[0].is_radio_tuned = FALSE;
    rx_pwr_info->rx_pwr_chain[1].is_radio_tuned = FALSE;
    rx_pwr_info->rx_pwr_chain[0].rx_pwr = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
    rx_pwr_info->rx_pwr_chain[1].rx_pwr = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;

  }
  else
  {
    rx_pwr_info->rx_pwr_chain[0].is_radio_tuned = TRUE;
    
    /*--------------*
     * Get Rx Power *
     *--------------*/

    hdrsrchrf_get_rx_power( &rx0_dbm, &rx1_dbm, &diversity );
      /* Get rx power in dB units. We may read the stored value of RX AGC
         here if the DSP clock has been off */

    rx_pwr_info->rx_pwr_chain[0].rx_pwr = rx0_dbm * 10; /* convert it into pseudo 1/10 dbm */

    if ( diversity )
    {
      rx_pwr_info->rx_diversity_indicator = HDRSRCH_RX_CHAIN_ALL_AVAIL;
      rx_pwr_info->rx_pwr_chain[1].is_radio_tuned = TRUE;
      rx_pwr_info->rx_pwr_chain[1].rx_pwr = rx1_dbm * 10; /* convert it into pseudo 1/10 dbm */
     
    }
    else
    {
      rx_pwr_info->rx_diversity_indicator = HDRSRCH_RX_CHAIN_0_AVAIL;
      rx_pwr_info->rx_pwr_chain[1].is_radio_tuned = FALSE;
      rx_pwr_info->rx_pwr_chain[1].rx_pwr = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
     
    }

  }

  /* Get Ecio, Ecio will be valid only for priority chain. Div chain will
     always be invalid*/
  
   /*  In idle state primary chain ecio will be total energy */
  if( hdrsrch_state_id == HDRSRCH_IDLE_STATE )
  {
    subaset_idx = hdrsrchset_get_ref_subaset_idx();

    if ( ( subaset_idx != -1 ) && (hdrsrchset.subaset[subaset_idx].aset[0] != NULL) )
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

      ecio = (uint16) ( hdrsrchset_eng_to_ecio( ecio ) );
      /* convert ecio to -1/2dB units. Note that the number returned is +ve but
       it represents value in -ve dB units  e.g. -5dB is 5 */

      ecio = -ecio * 5;
      /* make it nageative ecio what is was orginally and convert it into pseudo 1/10 dbm*/

      /* Get total energy */
      rx_pwr_info->rx_pwr_chain[0].ecio = ecio;
    }
    else
    { 
      ecio = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
    }
 
  }
  /*  In traffic primary chain ecio will be instantaneous primary energy */
  else if( hdrsrch_state_id == HDRSRCH_CONNECTED_STATE )
  {
#ifdef FEATURE_HDR_REVB

    subaset_idx = hdrsrchset_get_ref_subaset_idx();

    if ( ( subaset_idx != -1 ) &&  (hdrsrchset.subaset[subaset_idx].aset[0] != NULL) )
    {
      /* Find the maximum ecio among all active set sectors in the reference subaset */
      for ( i = 0; i < hdrsrchset.subaset[subaset_idx].aset_cnt; i++ )
      {
        if ( ( hdrsrchset.subaset[subaset_idx].aset[i] != NULL ) &&
             ( ecio < hdrsrchset.subaset[subaset_idx].aset[i]->pri_chain_inst_eng ) )
          {
          ecio = hdrsrchset.subaset[subaset_idx].aset[i]->pri_chain_inst_eng;
          } 
      }
    }

    else
    {
       ecio = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
    }
  
#else

    if ( hdrsrchset.aset[0] != NULL )
    {
      ecio = hdrsrchset.aset[0]->total_eng;
      /* initialize max ec/io with the aset 0 eng before comparing all pilots */

      /* Find the maximum ecio among all active set sectors */
      for ( i = 0; i < hdrsrchset.aset_cnt; i++ )
      {
        if ( ( hdrsrchset.aset[i] != NULL )         && 
             ( ecio < hdrsrchset.aset[i]->pri_chain_inst_eng ) )
        {
          ecio = hdrsrchset.aset[i]->pri_chain_inst_eng;
        }
      }
    }

    else
    {
       ecio = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
    }
      
#endif /* FEATURE_HDR_REVB */

    if( ecio != HDRSRCH_SIGNAL_INFO_INVALID_VALUE )
    {
      ecio = (uint16) ( hdrsrchset_eng_to_ecio( ecio ) );
        /* convert ecio to -1/2dB units. Note that the number returned is +ve but
         it represents value in -ve dB units  e.g. -5dB is 5 */

      ecio = -ecio *5;
       /* make it nageative ecio what is was orginally and covert it to pseudo 1/10dbm */

      rx_pwr_info->rx_pwr_chain[0].ecio = ecio;
        /* Pass the ecio value back to the caller   */
    }
  }
  else
  {
    rx_pwr_info->rx_pwr_chain[0].ecio = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
  }

  rx_pwr_info->rx_pwr_chain[1].ecio = HDRSRCH_SIGNAL_INFO_INVALID_VALUE;
    /* Div chain ecio will always be invalid for HDR */  
  
} /* hdrsrchrf_get_rx_power_info() */


/*==========================================================================

FUNCTION        HDRSRCHRF_IS_BAND_CHAN_SUPPORTED_V2

DESCRIPTION
  Returns RF supports the band/chan with good RF Cal.
    
DEPENDENCIES
  

INPUTS
  band, chan

RETURN VALUE 
 TRUE: good RF cal for band/chan.
 FALSE: Bad RF cal.
 

SIDE EFFECTS
  none

==========================================================================*/

boolean hdrsrchrf_is_band_chan_supported_v2
(
  sys_band_class_e_type   band, 
    /* Band */
  word                    chan
    /* Channel number */
)
{
#ifdef FEATURE_HDR_RUMI
  if ( !hdrhit_rumi_cfg.y1y2_support )
  {
    return TRUE;
  }
  else
  {
#endif /* FEATURE_HDR_RUMI */
  return rfm_is_band_chan_supported_v2( RFM_1XEVDO_MODE, band, chan );
#ifdef FEATURE_HDR_RUMI
  } /* if !hdrhit_rumi_cfg.y1y2_support */
#endif /* FEATURE_HDR_RUMI */
} /* hdrsrchrf_is_band_chan_supported_v2 */


/*==========================================================================

FUNCTION        HDRSRCHRF_IS_BAND_CHAN_SUPPORTED

DESCRIPTION
  Returns RF supports the band/chan.
    
DEPENDENCIES
  None

INPUTS
  band, chan

RETURN VALUE 
 TRUE: band/chan is supported
 FALSE: band/chan is not supported
 

SIDE EFFECTS
  none

==========================================================================*/

boolean hdrsrchrf_is_band_chan_supported
(
  sys_band_class_e_type   band, 
    /* Band */
  word                    chan
    /* Channel number */
)
{
#ifdef FEATURE_HDR_DIME_RUMI_IQ
    return TRUE;
#else
  sys_channel_type band_chan;

  boolean is_supported;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the HDR owns the device then get band the support information for 
     rf driver and RF device */
  /* Always check RF driver instead of current RF device
     to support dev hopping. Keeping original code in place
     just in case
   
  if ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain == TRUE )
  {
  band_chan.band      = band;
  band_chan.chan_num  = chan;

    is_supported = rfm_cdma_is_band_chan_supported( 
            hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN ), 
      band_chan );
  }
  else */

  {
    /* If the HDR doesnt owns the device then get band the support 
       information for only rf driver */
    is_supported = rfm_is_band_chan_supported_v2( 
      RFM_1XEVDO_MODE, band, chan );
  }

  return is_supported;
#endif /* FEATURE_HDR_DIME_RUMI_IQ */

} /* hdrsrchrf_is_band_chan_supported */


/*==========================================================================

FUNCTION        HDRSRCHRF_SET_FUTURE_BAND

DESCRIPTION
  This function sets the future band HDR will be on. This function
  must be called before requesting a TRM lock if the previous band
  HDR was on (for PRI chain) was different. so that TRM can have 
  the updated bandclass info to unlock 1x if needed.
    
DEPENDENCIES
  It must be called when HDR does not own the chain.

INPUTS
  RF chain id.
  Pointer to populated band/channel information.

RETURN VALUE 
 TRUE    if HDR does not own the chain
 FALSE   Otherwise
 

SIDE EFFECTS
  none

==========================================================================*/
boolean hdrsrchrf_set_future_band
(
  hdrsrchrf_logical_chain_enum_t  chain_id,
  hdrsrchrf_trm_freq_info_type*   freq_info_ptr
)
{
  trm_modify_return_enum_type band_granted;
  boolean                     retval;

  ASSERT(( chain_id < HDRSRCHRF_NUM_CHAINS ) &&
         (freq_info_ptr != NULL));
    /* Sanity check. */

  hdrsrchrf_set_trm_freq_info( freq_info_ptr );
    /* Store band/chan info update. */

  if ( hdrsrchrf.chain[chain_id].owns_chain )
  {

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "hdrsrchrf_set_future_band: start request freq tune");

    band_granted = hdrsrchrf_request_frequency_tune( 
      HDRSRCHRF_CHAIN_TO_TRM(chain_id),
      freq_info_ptr->freq_info );
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "hdrsrchrf_set_future_band: end request freq tune");

    /* TODO handle denial */

    retval = TRUE;
  }
  else
  {
    retval = FALSE;
  }

  HDR_MSG_SRCH_2(MSG_LEGACY_MED, 
                 "set future band for chain %d sucess %d", 
                 chain_id, retval);

  return retval;
}

/*==========================================================================

FUNCTION        HDRSRCHRF_ANTENNA_TUNER_ACTION

DESCRIPTION
  This function calls RF API for antenna tuner action.
  The function is expected to be called approximately 
  every 200 ms.
    
DEPENDENCIES
  None.

INPUTS
  None

RETURN VALUE 
  None

SIDE EFFECTS
  none

==========================================================================*/
void hdrsrchrf_antenna_tuner_action( void )
{
  rfm_device_enum_type rx_device;
  rfm_device_enum_type tx_device;

  rx_device = hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN);
  tx_device = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);
    /* Obtain RF device. */

  if ( tx_device >= RFM_INVALID_DEVICE )
  {
    tx_device = RFM_INVALID_DEVICE;
  }

  if ( rx_device < RFM_INVALID_DEVICE )
  {
    /* Note: TX device may be invalid. RF can be called without TX */
    rfm_hdr_antenna_tuner_action_time_v2( rx_device, tx_device );
      /* Call RF API for antenna tuner action. */
  }
  else
  {
    HDR_MSG_SRCH(MSG_LEGACY_ERROR, 
      "hdrsrchrf_antenna_tuner_action: invalid RF device");
  }

} /* hdrsrchrf_antenna_tuner_action */

/*==========================================================================

FUNCTION        HDRSRCHRF_SET_TRM_FREQ_INFO

DESCRIPTION
  This function sets band/channel information to be passed to TRM for 
  resource reservation.
    
DEPENDENCIES
  None

INPUTS
  Pointer to populated band/channel information.

RETURN VALUE 
  None

SIDE EFFECTS
  none

==========================================================================*/
void hdrsrchrf_set_trm_freq_info
(
  hdrsrchrf_trm_freq_info_type *freq_info_ptr
)
{
  ASSERT(freq_info_ptr != NULL);
    /* Sanity check. */

#ifdef FEATURE_MEMCPY_REMOVAL
  hdrsrchrf.trm_freq_info = *freq_info_ptr;
#else
  memcpy( &(hdrsrchrf.trm_freq_info), 
    freq_info_ptr, 
    sizeof(hdrsrchrf.trm_freq_info) );
#endif /* FEATURE_MEMCPY_REMOVAL */
    /* Save frequency info. */

  hdrsrchrf.trm_freq_info.freq_info.channels = 
    &(hdrsrchrf.trm_freq_info.channel_list[0]);
    /* Update pointer reference. */

} /* hdrsrchrf_set_trm_freq_info */


/*==========================================================================

FUNCTION        HDRSRCHRF_GET_TRM_FREQ_INFO

DESCRIPTION
  This function returns pointer to saved band/channel information to be 
  passed to TRM for resource reservation.
    
DEPENDENCIES
  hdrsrchrf_set_trm_freq_info() must be called before calling this function.

INPUTS
  None.

RETURN VALUE 
  Pointer to populated band/channel information.

SIDE EFFECTS
  none

==========================================================================*/
hdrsrchrf_trm_freq_info_type * hdrsrchrf_get_trm_freq_info( void )
{
  
  return &(hdrsrchrf.trm_freq_info);
    /* Return pointer to stored band/chan info. */  

} /* hdrsrchrf_set_trm_freq_info */

/*==========================================================================

FUNCTION        HDRSRCHRF_GET_CHAIN_HELD_REASON

DESCRIPTION
  This function returns the reason for which the chain is held
  currently by HDR
    
DEPENDENCIES
  None

INPUTS
  chain id

RETURN VALUE 
  Reason for taking the chain 

SIDE EFFECTS
  none

==========================================================================*/
hdrsrchrf_reason_enum_t hdrsrchrf_get_chain_held_reason
(
  hdrsrchrf_logical_chain_enum_t  chain_id
)
{
  return hdrsrchrf.chain[chain_id].reason;
} /* hdrsrchrf_get_chain_held_reason */

/*============================================================================

FUNCTION HDRSRCHRF_REENABLE_REQUEST_NOTIFY

DESCRIPTION
  Resets the content of the req_notify_pending flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/

void hdrsrchrf_reenable_request_notify(void)
{

/*--------------------------------------------------------------------------*/

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].req_notify_pending = FALSE;

} /* hdrsrchrf_reenable_request_notify */


#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
/*==========================================================================

FUNCTION        HDRSRCHRF_SEND_TX_FREQ_TO_COEX

DESCRIPTION
  This function sends frequency change info to coex manager.
    
DEPENDENCIES
  None

INPUTS
  chain id

RETURN VALUE 
  None

SIDE EFFECTS
  none

==========================================================================*/
void hdrsrchrf_send_tx_freq_to_coex
(
   rfm_device_enum_type                  rf_device
     /* The logical chain ID */
)
{
  uint8 ch;
    /* temp loop variable */

  sys_channel_type channel_info[HDRSRCH_MAX_NUM_CARRIERS];
    /* Band channel */

  if ( rf_device == RFM_INVALID_DEVICE  )
  {
    hdrmultirat_process_freq_change( RFM_INVALID_DEVICE,
                                     NULL,
                                     HDRMULTIRAT_REVERSE_LINK,
                                     0);
  }
  else
  {
    for ( ch = 0; ch < hdrsrchrf.trm_freq_info.freq_info.num_channels ; ch++)
    {
      channel_info[ch].band = hdrsrchrf.trm_freq_info.freq_info.band;
      channel_info[ch].chan_num = 
        hdrsrchrf.trm_freq_info.freq_info.channels[ch];
    }

    hdrsrchrf.trm_freq_info.freq_info.num_channels = MIN( 
                            hdrsrchrf.trm_freq_info.freq_info.num_channels, 
                            HDRL1_SRCH_MAX_NUM_SUBASETS );
      /* Limit num_channel size to maximum of HDRL1_SRCH_MAX_NUM_SUBASETS */
      /* klockwork error :: Array 'hdrmultirat.coex_info.tx.channel' of size 3
           may use index value 0..254 */

    hdrmultirat_process_freq_change( rf_device,
                                     channel_info,
                                     HDRMULTIRAT_REVERSE_LINK,
                                     hdrsrchrf.trm_freq_info.freq_info.num_channels
                                     );

  }
  
} /* hdrsrchrf_send_tx_freq_to_coex */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
/*==========================================================================

FUNCTION        HDRSRCHRF_REGISTER_SCI_WITH_TRM 
 
DESCRIPTION
  This function registers the sleep cycle in ms with TRM. TRM uses it for
  its page block management algorithms.
    
DEPENDENCIES
  None

INPUTS
  Sleep cycle in milli secs

RETURN VALUE 
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchrf_register_sci_with_trm
(
   uint16                               sci_ms
     /* sleep cycle in ms */
)
{
   trm_register_feature_input_info register_info;
   /* TRM variable to pass sci-cycle and tech info */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* Intialization and registration of trm drx structure */
   register_info.client_id = TRM_HDR;
   register_info.feature_info = TRM_FEATURE_PBR;
   register_info.register_info.new_pbr_info.type = TRM_MODIFY_PBR_DRX_CYCLE;
   register_info.register_info.new_pbr_info.new_drx_cycle.client = TRM_HDR;

   register_info.register_info.new_pbr_info.new_drx_cycle.drx_cycle = sci_ms;

   trm_register_feature( &register_info );
   
   HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "Registered drx=%d with trm", sci_ms); 

} /* hdrsrchrf_register_sci_with_trm */


/*==========================================================================

FUNCTION        HDRSRCHRF_REGISTER_PBR_MODE_WITH_TRM 

DESCRIPTION
  This function registers the PBR mode with TRM. 
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE 
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchrf_register_pbr_mode_with_trm(void)
{
  trm_register_feature_input_info register_info;
    /* TRM variable to pass pbr mode info */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* Intialization and registration of trm pbr structure*/ 
   register_info.client_id = TRM_HDR;
   register_info.feature_info = TRM_FEATURE_PBR;
   register_info.register_info.new_pbr_info.type = TRM_MODIFY_PBR_MODES;
   register_info.register_info.new_pbr_info.new_modes.client = TRM_HDR;
   register_info.register_info.new_pbr_info.new_modes.pbr_modes = 1<< TRM_DEMOD_PAGE;
   
   trm_register_feature( &register_info );

   HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "Registered pbr mode=demod page with TRM"); 
} /* hdrsrchrf_register_pbr_mode_with_trm */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */


/*==========================================================================

FUNCTION        HDRSRCHRF_REGISTER_APS_MODE_WITH_TRM 

DESCRIPTION
  This function registers the APS support mask with TRM. 
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE 
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchrf_register_aps_mode_with_trm(void)
{
  trm_register_feature_input_info register_info;
    /* TRM variable to pass pbr mode info */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /* Intialization and registration of trm pbr structure*/ 
   register_info.client_id = TRM_HDR;
   register_info.feature_info = TRM_FEATURE_APS;
   register_info.register_info.new_aps_info.reason_mask = ((uint64)1) << TRM_BG_TRAFFIC;
   
   trm_register_feature( &register_info );

   HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "Registered APS for BG_TRAFFIC with TRM"); 
} /* hdrsrchrf_register_pbr_mode_with_trm */


/*============================================================================

FUNCTION HDRSRCHRF_PREP_FOR_IRAT_MEAS

DESCRIPTION 
  This function is only used in DO to LTE reselection.  It is called as part of
  the TRM reservation sequence for IRAT measurements.
 
  
DEPENDENCIES
  The client must be holding an RF chain resource lock

RETURN VALUE
  The new lock duration

SIDE EFFECTS
  None

============================================================================*/

hdrsrchrf_duration_t hdrsrchrf_prep_for_irat_meas
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_duration_t                  duration
    /* The new lock duration granted. */

)
{

  trm_modify_chain_state_input_info     modify_state_info;
    /* Input for change_duration */
/*--------------------------------------------------------------------------*/

  modify_state_info.client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
  modify_state_info.modify_type = TRM_MODIFY_DURATION;
  modify_state_info.modify_info.modify_duration.duration = ( trm_duration_t ) duration;
  modify_state_info.modify_info.modify_duration.okay_to_shorten = TRUE;
  modify_state_info.tag = 0;
  
  trm_modify_chain_state( &modify_state_info );
    /* Just pass this request on to TRM. */

  hdrsrchrf_rfm_wait();
    /* Wait to ensure we receive response from TRM */

  return hdrsrchrf.chain[chain_id].modify_duration;

} /* hdrsrchrf_prep_for_irat_meas */

#ifdef FEATURE_HDR_QTA

/*===========================================================================

FUNCTION HDRSRCHRF_GET_QTA_WINNING_CLIENT_ID

DESCRIPTION
  This function returns the winning client ID information.
  
DEPENDENCIES
  None

RETURN VALUE
  Winning Client ID of given QTA

SIDE EFFECTS
  None

===========================================================================*/

trm_client_enum_t hdrsrchrf_get_qta_winning_client_id(  void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR QTA winning client %d ", 
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rl_state.qta_winning_client_id );

  return hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rl_state.qta_winning_client_id;

} /* hdrsrchrf_get_qta_winning_client_id( ) */

/*===========================================================================

FUNCTION HDRSRCHRF_SET_QTA_WINNING_CLIENT_ID

DESCRIPTION
  This function stores the winning client ID information
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchrf_set_qta_winning_client_id( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rl_state.qta_winning_client_id = 
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rl_state.winning_client.client_id;

} /* hdrsrchrf_set_qta_winning_client_id( ) */

/*===========================================================================

FUNCTION HDRSRCHRF_SET_QTA_CLIENT_STATE

DESCRIPTION
  This function calls TRM API to update the QTA gap start and end.
  
DEPENDENCIES
  Expects non-NULL RF params for start QTA
  Return value is valid only for end QTA

RETURN VALUE
  TRUE if the previous gap was an empty gap
  FALSE otherwise

SIDE EFFECTS
  TRM initiates QTA with target RAT. But target RAT can start QTA
  only after source tech HDR here releases the primary chain

===========================================================================*/

boolean hdrsrchrf_set_qta_client_state
(
  hdrsrchrf_client_state_enum_type state,
  rfm_meas_common_param_type *rf_params
)
{

  static trm_set_client_state_input_type       input_state;
  static trm_set_client_state_output_data      output_state;
  rfm_device_enum_type                  hdr_pri_device;
  rfm_device_enum_type                  hdr_div_device;
  rfm_device_enum_type                  hdr_tx_device;
  uint16                                dev_count = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR QTA TRM tech_id client %d %d", 
      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rl_state.winning_client.client_id,
      hdrsrchrf_get_qta_winning_client_id());

  if (hdrsrchrf.qta_client_state == state )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR QTA - Skipped setting TRM client state,"
                                     "current %d, previous %d",
                    hdrsrchrf.qta_client_state, state);
    return FALSE;
  }

  hdrsrchrf.qta_client_state = state;

  memset ( &input_state, 0, sizeof ( trm_set_client_state_input_type ) );
    // Clear local data structure

  input_state.client_id = TRM_HDR;
  input_state.client_state = TRM_QTA;
  input_state.data.qta_data.operation= ( trm_state_oper_enum_type ) state;
  input_state.data.qta_data.target_client =
                          hdrsrchrf_get_qta_winning_client_id();

  input_state.data.qta_data.num_dev_protect = 0;
  dev_count = 0;

  hdr_pri_device = hdrsrchrf_get_rf_device( TRM_HDR );
  /* Obtain primary RF device ID */

  if(hdr_pri_device < RFM_MAX_DEVICES)
  {
    /* Primary device ID is valid. Let TRM protect the device during */
    /* QTA so that no other client with higher priority request */
    /* can get it after QTA is started */

    input_state.data.qta_data.dev_protect [dev_count] = hdr_pri_device;
    input_state.data.qta_data.num_dev_protect += 1;
    dev_count++;
  }

  hdr_div_device = hdrsrchrf_get_rf_device( TRM_HDR_SECONDARY );
  /* Obtain diversity RF device ID */

  if(hdr_div_device < RFM_MAX_DEVICES)
  {
    /* Diversity device ID is valid. Let TRM protect the device during */
    /* QTA so that no other client with higher priority request */
    /* can get it after QTA is started */
    input_state.data.qta_data.dev_protect [dev_count] = hdr_div_device;
    input_state.data.qta_data.num_dev_protect += 1;
    dev_count++;
  }

  hdr_tx_device = hdrsrchrf_get_rf_device( TRM_HDR_TX);
    /* Obtain tx RF device ID */

  if(hdr_tx_device < RFM_MAX_DEVICES)
  {
    /* TX device ID is valid. Let TRM protect the device during */
    /* QTA so that no other client with higher priority request */
    /* can get it after QTA is started */
    input_state.data.qta_data.dev_protect [dev_count] = hdr_tx_device;
    input_state.data.qta_data.num_dev_protect += 1;
  }

   input_state.data.qta_data.rf_params = rf_params;
    /* Will be NULL for output operation */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "HDR QTA devices %d %d %d", 
          input_state.data.qta_data.dev_protect [0],
          input_state.data.qta_data.dev_protect [1],
          input_state.data.qta_data.dev_protect [2]);

  trm_set_client_state( &input_state, &output_state );
  
  return output_state.result.qta_result.qta_gap_empty;
    /* Valid only for END_QTA */
 
} /* hdrsrchrf_set_qta_client_state( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_SET_APS_CLIENT_STATE

DESCRIPTION
  This function calls TRM API to update the APS request start and end.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
  
===========================================================================*/

void hdrsrchrf_set_aps_client_state
(
  hdrsrchrf_trm_client_state_enum_type  state
)
{
  static trm_set_client_state_input_type       input_state;
  static trm_set_client_state_output_data      output_state;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
  "Update_APS_state_information:%d, Current_state:%x", 
  state, hdrsrch_get_current_state());

  memset ( &input_state, 0, sizeof ( trm_set_client_state_input_type ) );
    // Clear local data structure

  input_state.client_id = TRM_HDR;
  input_state.client_state = TRM_APS;
  input_state.data.aps_data.operation= ( trm_state_oper_enum_type ) state;
  input_state.data.aps_data.aps_type = TRM_APS_REQUESTED;

  trm_set_client_state( &input_state, &output_state );
  
  return;
} /* hdrsrchrf_set_aps_client_state( ) */


/*===========================================================================

FUNCTION HDRSRCHRF_IS_QTA_ALLOWED

DESCRIPTION
  This function calls TRM API to check whether HDR can proceed with QTA
  
DEPENDENCIES 
 
INPUT 
  Winning client ID as informed by TRM in the Unlock callback

RETURN VALUE
  TRUE if it can proceed
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchrf_is_qta_allowed
(
  trm_client_enum_t winning_client
)
{
  trm_get_info_input_type      input;
  trm_get_info_return_type     output;

  memset(&input,0,sizeof(trm_get_info_input_type));
  memset(&output,0,sizeof(trm_get_info_return_type));

  input.client_id = TRM_HDR;
  input.input_data.qta_proceed_check_info.idle_tech = winning_client;

  /* we only call this API for PRI unlock */
  input.info_type = TRM_QTA_PROCEED_CHECK_INFO;

  trm_get_info(&input,&output);

  HDR_MSG_SRCH_3(MSG_LEGACY_HIGH, "HDR QTA: TRM allow QTA to proceed:%d,"
                                  " idle_tech(%d), win_client:%d", 
                 output.info.qta_proceed_info.proceed_with_qta,
                 input.input_data.qta_proceed_check_info.idle_tech,
                 winning_client);
  return output.info.qta_proceed_info.proceed_with_qta;
}

#endif /* FEATURE_HDR_QTA */

/*===========================================================================

FUNCTION HDRSRCHRF_RXTX_GRANT_CB

DESCRIPTION
  This function serves as TRM chain granted cb for RxTx chain.
  It passes all non-RxTx TRM grant to legacy callbacks.
  
DEPENDENCIES
  None

RETURN VALUE
  True - There is some grant that require processing in
         PRI_CHAIN_EVENT handler.
 
  False - Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchrf_rxtx_grant_cb
(
  trm_client_enum_t               client,
    /* The client which is being informed of an event */

  trm_grant_return_enum_type      grant,
    /* The grant being sent to the client */

  trm_request_tag_t               tag
    /* Identfying tag for this request/grant transaction */
)
{
  hdrsrchrf_logical_chain_enum_t chain_id;
  boolean rxtx_event_sig = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( client );

  HDR_MSG_SRCH_5( MSG_LEGACY_MED,
                  "RxTx cb: rxtx:%d chain:%d granted:%d owns:%d/%d",
                  hdrsrchrf.rxtx_pending, chain_id, grant,
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );

  if ( grant == TRM_GRANTED )
  {
    hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_GRANTED;
  }
  else if ( grant == TRM_DENIAL )
  {
    hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_DENIAL;
  }
  else
  {
    /* TRM should only send grant or denial for R&N */
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "RxTx cb: ignored unexpected trm grant:%d", 
                    grant );

    rxtx_event_sig = FALSE;
  }

  /* Only handles grant and denial, ignore all other grant types */
  if ( ( chain_id == HDRSRCHRF_DIV_CHAIN ) ||
       ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
         ( !hdrsrchrf.rxtx_pending ) ) )
  {
    /* Pass thru DIV chain and non-RxTx related PRI chain grant */
    hdrsrchrf_grant_callback( client,
                              grant,
                              tag );

    rxtx_event_sig = FALSE;
  }
  else if ( grant == TRM_DENIAL )
  {
    /* Pass PRI/TX denial to PRI handler */
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event = HDRSRCHRF_DENIAL;

    hdrsrchrf_grant_callback( TRM_HDR,
                              grant,
                              tag );

    rxtx_event_sig = FALSE;
  }

  return rxtx_event_sig;
}

/*===========================================================================

FUNCTION hdrsrchrf_rxtx_event_on_chain

DESCRIPTION
  This function handles TRM chain event (grant or unlock) for the given
  RxTx chain.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
hdrsrchrf_rxtx_chain_status_enum_type hdrsrchrf_rxtx_event_on_chain
(
  hdrsrchrf_logical_chain_enum_t chain_id
)
{
  hdrsrchrf_async_event_type event = hdrsrchrf.chain[chain_id].rxtx_event;
  hdrsrchrf_rxtx_chain_status_enum_type status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf.chain[chain_id].rxtx_event = HDRSRCHRF_NO_EVENT;

  if ( event == HDRSRCHRF_GRANTED )
  {
    status = HDRSRCHRF_RXTX_GRANTED;

    HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                    "RxTx hdlr: chain:%d granted",
                    chain_id );

    hdrsrchrf.chain[chain_id].owns_chain = TRUE;

    hdrsrchrf.chain[chain_id].req_notify_pending = FALSE;

    hdrsrchrf.chain[chain_id].rf_chain = 
      hdrsrchrf_get_rf_device( HDRSRCHRF_CHAIN_TO_TRM( chain_id) );    
  }
  else if ( event == HDRSRCHRF_UNLOCK_REQUESTED )
  {
    status = HDRSRCHRF_RXTX_UNLOCKED;

    /* It's possible TRM unlocks a chain that we haven't updated
       owns flag (2 callbacks before PRI_CHAIN_EVENT_SIG) */
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "RxTx hdlr: chain:%d unlocked owned:%d", 
                    chain_id, hdrsrchrf.chain[chain_id].owns_chain );
  }
  else
  {
    if ( event == HDRSRCHRF_DENIAL )
    {
      /* Shouldn't get a DENIAL event, print error */
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "RxTx hdlr: unexpected TRM DENIAL" );
    }

    status = hdrsrchrf.chain[chain_id].owns_chain ? 
      HDRSRCHRF_RXTX_OWNED : HDRSRCHRF_RXTX_NOT_OWNED;
  }

  return status;
}

/*===========================================================================

FUNCTION HDRSRCHRF_RXTX_EVENT_HANDLER

DESCRIPTION
  This function handles TRM chain event (grant or unlock) for the given
  RxTx chain 
  
  Logic table:
    Process each chain first:
      Grant: set chain to granted
      Unlock: R&N on the chain
    After both chains are processed:
      Both chain owned: un-retain lock and pass grant to PRI
      Only 1 chain is granted: retain lock on newly granted chain
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchrf_rxtx_event_handler( void )
{
  hdrsrchrf_rxtx_chain_status_enum_type pri;
  hdrsrchrf_rxtx_chain_status_enum_type tx;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_5( MSG_LEGACY_MED, 
                  "RxTx hdlr: rxtx:%d event:%d/%d own:%d/%d", 
                  hdrsrchrf.rxtx_pending,
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].rxtx_event,
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );

  if ( !hdrsrchrf.rxtx_pending )
  {
    if ( hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].rxtx_event == HDRSRCHRF_GRANTED )
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, 
                    "RxTx hdlr: unexpected TX grant, rel TX" );

      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event = HDRSRCHRF_NO_EVENT;

      hdrsrchrf_release( HDRSRCHRF_TX_CHAIN );
    }

    if ( hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event != HDRSRCHRF_NO_EVENT )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "RxTx hdlr: pass event:%d to pri chain handler", 
                      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event );

      hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event = HDRSRCHRF_NO_EVENT;

      rex_set_sigs( HDRSRCH_TCB, HDRSRCH_PRI_CHAIN_EVENT_SIG );
    }

    return;
  }

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                  "RxTx hdlr: event:%d/%d own:%d/%d", 
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].rxtx_event,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].rxtx_event,
                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].owns_chain,
                  hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain );

  /* Process event on each chain */
  pri = hdrsrchrf_rxtx_event_on_chain( HDRSRCHRF_PRI_CHAIN );
  tx = hdrsrchrf_rxtx_event_on_chain( HDRSRCHRF_TX_CHAIN );

  if ( HDRSRCHRF_RXTX_OWNED_OR_GRANTED( pri ) &&
       HDRSRCHRF_RXTX_OWNED_OR_GRANTED( tx ) )
  {
    /* Both chains granted */
    (void) rex_clr_timer( &hdrsrchrf.rxtx_pending_timer );

    HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "RxTx hdlr: both chain granted!" );

    hdrsrchrf.rxtx_pending = FALSE;
    
    /* Unreg on PRI will unreg both chains */
    hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );

    /* Only call grant cb for PRI chian */
    hdrsrchrf_grant_callback( TRM_HDR,
                              TRM_GRANTED,
                              0 );
    rex_set_sigs( HDRSRCH_TCB, HDRSRCH_PRI_CHAIN_EVENT_SIG );
  }
  else if ( ( pri == HDRSRCHRF_RXTX_UNLOCKED ) ||
            ( tx == HDRSRCHRF_RXTX_UNLOCKED ) )
  {
    /* Always R&N PRI+TX together whenever there is a unlock on either chain */
    hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].req_notify_pending = FALSE;
    
    hdrsrchrf_request_and_notify( HDRSRCHRF_PRI_CHAIN, 
                                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].resource, 
                                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].duration, 
                                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].reason,
                                  hdrsrchrf.chain[HDRSRCHRF_PRI_CHAIN].grant_cb );
  }
  else if ( pri == HDRSRCHRF_RXTX_GRANTED )
  {
    hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, 
                           hdrsrchrf_rxtx_unlock_cb );
  }
  else if ( tx == HDRSRCHRF_RXTX_GRANTED )
  {
    hdrsrchrf_retain_lock( HDRSRCHRF_TX_CHAIN, 
                           hdrsrchrf_rxtx_unlock_cb );
  }
}

/*===========================================================================

FUNCTION HDRSRCHRF_TRM_GROUPED_ASYNC_CB

DESCRIPTION
  This function handles grouped TRM async cb. It simply calls
  legacy async cb with one by one.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchrf_trm_grouped_async_cb
( 
   trm_grouped_async_callback_information_type  *trm_cb_info
     /* Callback information passed back */
)
{
  uint8 i;
  boolean rxtx_event_sig = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, 
                  "TRM grouped async cb for %d clients",
                  trm_cb_info->num_of_entries );

  hdrsrchrf_trm_log_info_num_of_entries = trm_cb_info->num_of_entries;
  for (i=0; i<trm_cb_info->num_of_entries; i++)
  {
    /* If any cb requested PRI_CHAIN_EVENT_SIG, set it once
       at the end to avoid multiple task switches */
    rxtx_event_sig |= hdrsrchrf_trm_async_cb( &trm_cb_info->data[i] );
  }
  hdrsrchrf_trm_log_info_num_of_entries = 0;

  if ( rxtx_event_sig )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "RxTx: setting RxTx sig" );
    rex_set_sigs( HDRSRCH_TCB, HDRSRCH_RXTX_SIG );
  }
}

/*============================================================================

FUNCTION HDRSRCHRF_TRM_ASYNC_CB

DESCRIPTION 
  This is the callback called by TRM in response to any TRM API's invoked. 
  
DEPENDENCIES
  In response to TRM API
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchrf_trm_async_cb
( 
   trm_async_callback_information_type  *trm_cb_info
     /* Callback information passed back */
)
{

  hdrsrchrf_logical_chain_enum_t        chain_id;
    /* The logical chain ID associated with the request */

  hdrsrch_cmd_type  *cmd;
    /* Command structure */

  boolean rxtx_event_sig = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  chain_id = HDRSRCHRF_CHAIN_FROM_TRM( trm_cb_info->client );

  if ( ( trm_cb_info->client != TRM_HDR ) &&  
       ( trm_cb_info->client != TRM_HDR_SECONDARY ) &&
       ( trm_cb_info->client != TRM_HDR_TX ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                  "TRM callback received for invalid client %d", 
                   trm_cb_info->client);

    return rxtx_event_sig;
  }
  
  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "TRM callback chain:%d type:%d modify:%d (out of %d clients)",
                  chain_id, trm_cb_info->cb_type, 
                  trm_cb_info->data.chain_modify_state_info.modify_type,
                  hdrsrchrf_trm_log_info_num_of_entries);

  switch ( trm_cb_info->cb_type )
  {
     case TRM_REQUEST_AND_NOTIFY_ASYNC_INFO:
    {
       rxtx_event_sig = hdrsrchrf_rxtx_grant_cb( 
                               trm_cb_info->client, 
                                 trm_cb_info->data.chain_req_n_not_info.grant,
                                 trm_cb_info->tag );
       break;
     }

     case TRM_MODIFY_CHAIN_STATE_ASYNC_INFO:
     {
       if( trm_cb_info->data.chain_modify_state_info.modify_type == TRM_MODIFY_DURATION )
       {
         hdrsrchrf.chain[chain_id].modify_chain_granted = 
           trm_cb_info->data.chain_modify_state_info.modify_info.mod_duration_info.result;

         hdrsrchrf.chain[chain_id].modify_duration = 
           trm_cb_info->data.chain_modify_state_info.modify_info.mod_duration_info.new_duration;

          /* Callback from rfm_xxx() function.
            Send a signal to the hdrsrch task rf task is complete.
            Overload the same signal. */
        ( void ) rex_set_sigs( HDRSRCH_TCB, HDRSRCH_RFM_SIG );

       }
       else if( trm_cb_info->data.chain_modify_state_info.modify_type == TRM_MODIFY_REASON )
       {
         hdrsrchrf.chain[chain_id].modify_chain_granted =
           trm_cb_info->data.chain_modify_state_info.modify_info.mod_reason_info.result;

        /* Callback from rfm_xxx() function.
            Send a signal to the hdrsrch task rf task is complete.
            Overload the same signal. */
        ( void ) rex_set_sigs( HDRSRCH_TCB, HDRSRCH_RFM_SIG );

       
       }
       if ( trm_cb_info->data.chain_modify_state_info.modify_type == TRM_MODIFY_BAND )
       {
         hdrsrchrf.chain[chain_id].modify_band_granted =
          trm_cb_info->data.chain_modify_state_info.modify_info.mod_band_info.result;
         
         hdrsrchrf.chain[chain_id].new_dev = 
           trm_cb_info->data.chain_modify_state_info.modify_info.mod_band_info.band_alloc[0].dev_allocated;

         /* Per discussion with TRM, for grouped band request, first callback will be grouped together */
         ( void ) rex_set_sigs( HDRSRCH_TCB, HDRSRCH_RFM_SIG );
           /* Callback from rfm_xxx() function. Send a signal to the hdrsrch task rf task is complete. 
           Overload the same signal. */

         HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "TRM callback with Band grant %d chain:%d dev:%d=>%d",
                         hdrsrchrf.chain[chain_id].modify_band_granted, 
                         chain_id,
                         hdrsrchrf.chain[chain_id].rf_chain,
                         hdrsrchrf.chain[chain_id].new_dev );
       }
       break;
     
     }
     case TRM_UNLOCK_CHAIN_ASYNC_INFO:
     {
       rxtx_event_sig = hdrsrchrf_rxtx_unlock_callback(
          trm_cb_info->data.unlock_info.unlock_client,
          trm_cb_info->data.unlock_info.event,
          trm_cb_info->data.unlock_info.unlock_by_sclk,
          trm_cb_info->data.unlock_info.winning_client_info,
          trm_cb_info->data.unlock_info.ta_info 
          );

       break;
     }

#ifdef FEATURE_HDR_DR_DSDS
     case TRM_DUAL_RECEIVE_INFO:
     {
       if (trm_cb_info->data.dual_receive_info.dual_rec_info == TRM_DUAL_RECEIVE_DISABLED )
       {
         HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR DR-DSDS Dual receive disabled!");

         hdrmultirat_dr_mode_change( HDRMULTIRAT_DUAL_SIM_DUAL_STANDBY );
         
       }
       else if ( trm_cb_info->data.dual_receive_info.dual_rec_info == TRM_DUAL_RECEIVE_ENABLED  )
       {
         HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDR DR-DSDS Dual receive enabled!");
         hdrmultirat_dr_mode_change( HDRMULTIRAT_DR_DUAL_SIM_DUAL_STANDBY );
       }
       else
       {
         HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR DR-DSDS unknown dual receive info!");
       }
       break;
       
     }
#endif /* FEATURE_HDR_DR_DSDS */

     default:
       break;

  }

  return rxtx_event_sig;

}

/*============================================================================

FUNCTION HDRSRCHRF_GET_RF_DEVICE

DESCRIPTION 
  This function returns the RF dev for the client. 
  
DEPENDENCIES
  None

RETURN VALUE
  RF Dev

SIDE EFFECTS
  None

============================================================================*/
rfm_device_enum_type hdrsrchrf_get_rf_device
( 
   trm_client_enum_t               client
    /* Which client do we need the RF device for */
)
{
  trm_get_info_input_type        input_info;
    /* Input to get trm info */

  trm_get_info_return_type       output_info;
    /* Output from get trm info */
/*--------------------------------------------------------------------------*/

  input_info.client_id = client;
  input_info.info_type = TRM_CLIENT_INFO;
  input_info.input_data.client_info.requested_client_id = client;
  trm_get_info( &input_info, &output_info);

#ifdef FEATURE_HDR_RUMI
  if ( output_info.info.client_info.dev >= RFM_MAX_DEVICES )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TRM returned invalid dev for client %d, forcing to RUMI cfg",
                    output_info.info.client_info.dev,
                    input_info.client_id );

    if ( client == TRM_HDR )
    {
      output_info.info.client_info.dev = hdrhit_rumi_cfg.pri_chain_override;
    }
    else if ( client == TRM_HDR_SECONDARY )
    {
      output_info.info.client_info.dev = hdrhit_rumi_cfg.div_chain_override;
    }
  }
#endif /* FEATURE_HDR_RUMI */
  
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TRM client %d rfdevice %d",
                  input_info.client_id,
                  output_info.info.client_info.dev );

  return output_info.info.client_info.dev;
}


/*============================================================================

FUNCTION HDRSRCHRF_GET_REASON

DESCRIPTION 
  This function returns the trm reason for the client being queried. 
  
DEPENDENCIES
  None

RETURN VALUE
  Client TRM reason

SIDE EFFECTS
  None

============================================================================*/
trm_reason_enum_t  hdrsrchrf_get_reason
( 
   trm_client_enum_t               client
    /* Which client we need the TRM reason for */
)
{
  trm_get_info_input_type        input_info;
    /* Input to get trm info */

  trm_get_info_return_type       output_info = {0};
    /* Output from get trm info */
/*--------------------------------------------------------------------------*/

  input_info.client_id = TRM_HDR;   
    
  input_info.info_type = TRM_CLIENT_INFO;
  input_info.input_data.client_info.requested_client_id = client;
  trm_get_info( &input_info, &output_info);
  
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "TRM client %d Reason %d",
                  client,
                  output_info.info.client_info.reason );

  return output_info.info.client_info.reason;
}


/*============================================================================

FUNCTION HDRSRCHRF_REGISTER_WITH_TRM

DESCRIPTION 
  Register for primary  and secondary TRM async callback
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void hdrsrchrf_register_with_trm ( void )
{
  trm_async_event_cb_input_type    input;
    /* Input for registering trm API */
/*--------------------------------------------------------------------------*/

  /* Register for primary TRM async callback */
  input.client_id = TRM_HDR;
    /* Identifier of the client registering for the event callbacks */
  input.operation = TRM_ASYNC_GROUPED_CB_REGISTER;
    /* Operation */
  input.data.grouped_reg_info.num_of_clients = 3;
  input.data.grouped_reg_info.client_ids[0] = TRM_HDR;
  input.data.grouped_reg_info.client_ids[1] = TRM_HDR_SECONDARY;
  input.data.grouped_reg_info.client_ids[2] = TRM_HDR_TX;
  
#ifdef FEATURE_HDR_DR_DSDS
  input.data.grouped_reg_info.events_bitmask = 1;
   /* Bitmask of optional events the client is registering for.
     Bit 0: DR-DSDS <--> DSDS notification. 
     Techs can use trm_async_optional_event_bitmask for bit shifting */
#else
  input.data.grouped_reg_info.events_bitmask = 0;
#endif /* FEATURE_HDR_DR_DSDS */

  input.data.grouped_reg_info.callback_ptr  =  hdrsrchrf_trm_grouped_async_cb;
  trm_handle_async_event_cb ( &input );

  hdrsrchsleep_wakeup_id_init();
    /* Also intialize wakeup id for TRM */
}
#ifdef FEATURE_HDR_DR_DSDS
/*============================================================================

FUNCTION HDRSRCHRF_UPDATE_DR_MODE

DESCRIPTION 
  Check with TRM if we are in DR mode
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void hdrsrchrf_update_dr_mode ( void )
{
  trm_get_info_input_type   input;
    /* Input info to send to TRM */

  trm_get_info_return_type  output;
    /* output info received from TRM */
 /*--------------------------------------------------------------------------*/

  
  input.client_id = TRM_HDR;

  input.info_type = TRM_DR_CAPABILITY;

  input.input_data.client_info.requested_client_id = TRM_HDR;


  trm_get_info( &input, &output);
   /* Get Info from TRM */

  if ( output.info.dual_rec_info.dual_rec_info == TRM_DUAL_RECEIVE_ENABLED )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "HDR DR-DSDS Dual receive enabled!");
         hdrmultirat_dr_mode_change( HDRMULTIRAT_DR_DUAL_SIM_DUAL_STANDBY );

  }

  HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "HDR DR-DSDS mode %d", 
                  output.info.dual_rec_info.dual_rec_info );

}

#endif /* FEATURE_HDR_DR_DSDS */

/*===========================================================================

FUNCTION HDRSRCHRF_GET_PRI_CHAIN_UNLOCK_STATUS

DESCRIPTION
  This function is called to get primary chain unlock status

DEPENDENCIES
  none

RETURN VALUE 
                  TRUE --> UNLOCK is ignored by HDR primary chain as GSM is winning client
                  FALSE --> UNLOCK is honoured by HDR
SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchrf_get_pri_chain_unlock_status( void ) 
{
  
  /*--------------------------------------------------------------------------*/

  return ( hdrsrchrf.pri_chain_unlock_ignored );

} /* hdrsrchrf_get_pri_chain_unlock_status */

/*==========================================================================

FUNCTION        HDRSRCHRF_GET_CLIENT_ASID_MAPPING 

DESCRIPTION
  Gets the GSM client ASID mapping.
  TRM_GSM1 will always map to Multi-mode subscription 
    
DEPENDENCIES
  None

INPUTS
  Client id

RETURN VALUE 
  ASID corresponding to a client, currently this is being handled
  only for GSM client

SIDE EFFECTS
  None

==========================================================================*/
sys_modem_as_id_e_type hdrsrchrf_get_client_asid_mapping
(
   trm_client_enum_t                client_id
     /* client_id */
)
{
  trm_get_info_input_type   input;
    /* Input info to send to TRM */

  trm_get_info_return_type  output;
    /* output info received from TRM */
 /*--------------------------------------------------------------------------*/

  
  input.client_id = TRM_HDR;

  input.info_type = TRM_ASID_MAPPING;

  input.input_data.client_info.requested_client_id = client_id;

  trm_get_info( &input, &output);
   /* Get Info from TRM */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                "Client %d ASID mapping  %d ", client_id, output.info.asid_mapping_info ); 

  return output.info.asid_mapping_info;
  
} /* hdrsrchrf_get_client_asid_mapping */

/*==========================================================================

FUNCTION        HDRSRCHRF_GET_SIMUL_CAP 

DESCRIPTION
  TRM get simultaneous capability 
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE 
 TRM Simultaneous Capability Information
   
SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchrf_get_simul_cap( void )
{
  trm_get_info_input_type        input_info;
    /* Input to get trm info */

  trm_get_info_return_type       output_info;
    /* Output from get trm info */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  input_info.client_id = TRM_HDR;
  input_info.info_type = TRM_SIMUL_CAPABILITY;
  input_info.input_data.client_info.requested_client_id = TRM_HDR;
  trm_get_info( &input_info, &output_info);

  return output_info.info.simul_cap_info;
}

/*===========================================================================

FUNCTION HDRSRCHRF_CANCEL_BAND_GRANT_REQUEST

DESCRIPTION
  This function cancels any pending band grant request if we are waiting on
  one.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/

void hdrsrchrf_cancel_band_grant_request
( 
  hdrsrchrf_logical_chain_enum_t chain_id
    /* Chain ID of the RF chain tuned. */
)
{
  hdrsrchrf_tune_state_type       state;
    /* The previous tune state of the chain. */

  static trm_grouped_modify_state_input_info     modify_group;
    /* Input for change_duration */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &modify_group, 0, sizeof( modify_group ) );
    /* Initialize the structure before passing to TRM */

  state = hdrsrchrf.chain[chain_id].tune.state;
    /* Get reason why tune-timer was running */

  if( state == HDRSRCHRF_WAITING_BAND_GRANT )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "Tune state %d prev tune state %d",
                    hdrsrchrf.chain[chain_id].tune.state,
                    hdrsrchrf.chain[chain_id].tune.band_change.prev_state);

    hdrsrchrf.chain[chain_id].tune.state  =
                    hdrsrchrf.chain[chain_id].tune.band_change.prev_state;
      /* And restore the reason */

    /* band grant timedout, move to NO_TUNE state */
    modify_group.modify_type = TRM_MODIFY_BAND;
    modify_group.modify_info[0].client_id = HDRSRCHRF_CHAIN_TO_TRM( chain_id );
    modify_group.modify_info[0].modify_data.modify_band.action = TRM_BAND_TUNE_CANCEL;

    if ( ( chain_id == HDRSRCHRF_PRI_CHAIN ) &&
         hdrsrchrf.chain[HDRSRCHRF_TX_CHAIN].owns_chain )
    {
      modify_group.num_of_entries = 2;
      modify_group.modify_info[1].client_id = TRM_HDR_TX;
      modify_group.modify_info[1].modify_data.modify_band.action = TRM_BAND_TUNE_CANCEL;
    }
    else
    {
      modify_group.num_of_entries = 1;
    }

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "hdrsrchrf_cancel_band_grant_request: start TRM_MODIFY_BAND TRM_BAND_TUNE_CANCEL");

    trm_grouped_modify_chain_state( &modify_group );
      /* Just pass this request to TRM. */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "hdrsrchrf_cancel_band_grant_request: start TRM_MODIFY_BAND TRM_BAND_TUNE_CANCEL");
 
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Band grant request cancelled!");   
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Not waiting on band grant!");
  }
} /* hdrsrchrf_cancel_band_grant_request() */

/*===========================================================================

FUNCTION HDRSRCHRF_GET_SUB_REASON

DESCRIPTION
  This function returns the appropriate subreason based on the reason that is being passed 
  to TRM.

DEPENDENCIES
  None

RETURN VALUE
  sub_reason to be passed to TRM

SIDE EFFECTS
  none

===========================================================================*/

hdrsrchrf_sub_reason_enum_t hdrsrchrf_get_sub_reason
(
  hdrsrchrf_reason_enum_t reason
)
{
  hdrsrchrf_sub_reason_enum_t sub_reason;

  switch(reason)
  {
    case HDRSRCHRF_ACCESS:
#ifndef FEATURE_HDR_TRM_UNIFICATION
    case HDRSRCHRF_ACCESS_URGENT:
#endif /* FEATURE_HDR_TRM_UNIFICATION */
    /*  
    All the below enumerate to same value as HDRSRCHRF_ACCESS_URGENT.
    Hence commented to prevent compiler error.
    case HDRSRCHRF_LOW_LAT_TRAFFIC:
    */
      sub_reason = SYS_PROC_TYPE_PS_CALL_SIGNALING;
      break;

    case HDRSRCHRF_TRAFFIC:
      sub_reason = SYS_PROC_TYPE_PS_CALL_USER_PLANE;
      break;

    case HDRSRCHRF_ACQUISITION:
      sub_reason = SYS_PROC_TYPE_ACQUISITION;
      break;

    case HDRSRCHRF_DEMOD_PAGE:
    case HDRSRCHRF_SMALL_SCI_PAGE:
      sub_reason = SYS_PROC_TYPE_PAGE;
      break;

    case HDRSRCHRF_IRAT_MEASUREMENT:
      sub_reason = SYS_PROC_TYPE_IRAT_MEASUREMENT;
      break;

    case HDRSRCHRF_BG_TRAFFIC:
      sub_reason = SYS_PROC_TYPE_PS_CALL_USER_PLANE;
      break;

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT 
    case HDRSRCHRF_OVHD:
    case HDRSRCHRF_OVHD_HIGH:
    /*  
    The below enumerate to same value as HDRSRCHRF_OVHD_HIGH. 
    Hence commented to prevent compiler error.
    case HDRSRCHRF_DEMOD_PAGE_CONTINUATION:
    case HDRSRCHRF_OFS:
    */
      sub_reason = SYS_PROC_TYPE_SYSTEM_INFO_READING;
      break;
#else
    case HDRSRCHRF_OFS:
      sub_reason = SYS_PROC_TYPE_INTER_FCELL_MEASUREMENT;
      break;

    case HDRSRCHRF_DEMOD_PAGE_CONTINUATION:
    /*  
    The below enumerate to same value as HDRSRCHRF_DEMOD_PAGE_CONTINUATION. 
    Hence commented to prevent compiler error.
    case HDRSRCHRF_SMALL_SCI_PAGE_CONTINUATION:
    */
      sub_reason = SYS_PROC_TYPE_EXTENDED_PAGE;
      break;
#endif

    case HDRSRCHRF_DIVERSITY:
    /*  
    All the below enumerate to same value. Hence commented to prevent 
    compiler error.
    case HDRSRCHRF_IDLE_DIVERSITY:
    case HDRSRCHRF_ACCESS_DIVERSITY:
    case HDRSRCHRF_ACQ_DIVERSITY:
    */
      sub_reason = SYS_PROC_TYPE_DIVERSITY_CHAIN;
      break;

    default:
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Default case : Subreason NONE!");   
      sub_reason = SYS_PROC_TYPE_NONE;
      break;
    }
  return sub_reason;
}


#ifdef FEATURE_HDR_PBR_RANDOMIZATION
/*============================================================================

FUNCTION HDRSRCHRF_UPDATE_RF_GRANT_EVENT

DESCRIPTION 
  Updates to TRM whether HDR completed the current rf grant event
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void hdrsrchrf_update_rf_grant_event 
( 
  hdrsrchrf_reason_enum_t           reason,
  uint64                          wakeup_identifier,
  hdrsrchrf_pbr_grant_event_enum_t  grant_event
)
{
  trm_inform_grant_event_input_info   input;
    /* Input info to send to TRM */

  trm_inform_grant_event_return_data  output;
    /* output info received from TRM */
 /*--------------------------------------------------------------------------*/
  
  input.client_id = TRM_HDR;

  input.reason = (trm_reason_enum_t) reason;

  input.wakeup_identifier = wakeup_identifier;

  input.grant_event = (trm_grant_event_enum_t) grant_event;

  trm_inform_grant_event(&input, &output);
   /* Set Info from TRM */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "PBR: Grant event (%d) to TRM with reason %d, wakeup_identifier %d", 
                  grant_event, reason, wakeup_identifier );

  return;
}
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */
