/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 H D R    S R C H    S L E E P    S T A T E

                             Search Sleep Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2001 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchsleep.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
02/22/2018   vke     Use OVHD RF reason for treselect command in sleep state
02/22/2018   vke     Fix to change the RF reason based on the new cmd
02/22/2018   vke     Retry deactivate cmd if it is close to normal wakeup
02/20/2017   vke     Increment wakeup_ID before moving to 39 state from 3e 
12/22/2017   vke     FR42394 PBR randomization for non-DR idle feature
11/24/2017   vlc     Added support for FR34448 NULL2L IRAT measurement
11/07/2017   vko     Change reason from Page demod to OVHD, after coming from traffic
05/27/2016   vko     QSH framework for events capturing
07/29/2015   mbs     Ported changes needed to replace set_future_band()
06/18/2015   tnp     Fix compiler warnings
06/17/2015   wsh     Fixed deactivate not processed in REACQ
06/01/2015   kss     Ensure ols timer can only fire after MCPM is complete;
                     update MCPM for reschedule or fast wakeup.
05/28/2015   vlc     Added support for DO->LTE redirection in connected state.
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/01/2015   wsh     Cleaned up critical_section usage 
03/12/2015   wsh     Request future band for reacq when returning from 39
03/10/2015   vke     Changes to prevent simultaneous calls to msgr API 
                     and set ols timer without fast wakeup overhead value
02/24/2015   vke     Changes to prevent simultaneous calls to msgr API 
                     by adjusting the fast-wake up overhead
01/22/2014   sat     Changes to not force wakeup at SCC boundary for RUMI flow.
12/03/2014   bb      Changes to defer processing commands during sleep state
                     when wakeup point is very close
11/12/2014   bb      Schedule fast wakeup only after sleeping for atleast 2slots 
11/11/2014   ljl     Fixed the comparison of sleep start time and sleep duration calculation time,
                     and avoided wakeup close to the frame boundary.
11/07/2014   vlc     Changed call order to avoid crashing when D2L measurement 
                     is not allowed by ASDIV.
10/30/2014   wsh     Reenabled ASDiv testmode, added IDLE test supprot 
10/14/2014   wsh     Changes to support Jolokia bringup
08/23/2014   bb      Changes for not using HDRSRCHRF_OVHD_HIGH when DSDA is enabled
08/03/2014   arm     Changes to request RX_ANY chain in DR mode for idle. 
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
06/16/2014   vke     Prevent the HDR to do fast wake-up for access cmd 
                     soon after sleep
06/12/2014   vlc     Added code to update the HDR data indicator SINR one last time before 
                     deassigning fingers in enter_sleep().
06/02/2014   vlc     Changed TRM request sequence for D2L measurements. 
05/22/2014   vlc     Added check for ASDIV before entering LTE meas state. 
05/15/2014   arm     TRM API changes for DR-DSDS. 
05/14/2014   vke     Remove direct calling of extend_sleep in 
                     hdrsrchsleep_wakeup_manager_cb
05/12/2014   vke     Klockworks fix
05/12/2014   mbs/vlc Increased HDRSRCHSLEEP_REACQ_TIME_SLOTS_DEFAULT to 7. 
05/08/2014   vke     Used warmup_time as cur_warmup_time_us + fastwakeup offset
05/05/2014   ljl     Added HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD.
04/25/2014   bb      Changes to ignore wakeup manager event if it is close to
                     wakeup
04/01/2014   vlc     Removed redundant call to hdrsrchltemeas_init_meas_state(). 
                     Added more F3 messages to explain why LTE meas is skipped. 
03/24/2014   ljl     Used the slpc API slpc_get_wakeup_and_ols_tick().
03/15/2014   wsh     Change to support T/T and new RF concurrency concern 
03/05/2014   wsh     Bug fixes for DSDS-ASDiv and DSDA-ASDiv support 
02/28/2014   sat     Checking if MDSP is active before sending HDR_FW_SCC_CMD 
                     during RF warmup done call back
02/25/2014   ljl     Requested MCPM_DO_PARMS_UPDATE_REQ instead of 
                     MCPM_DO_GO_TO_SLEEP_REQ when HDR was in sleep already.
                     Requested MCPM_DO_PARMS_UPDATE_REQ instead of MCPM_DO_NOP_REQ.
02/21/2014   vlc     Changed TRM reservation for D2L to accomodate dualsim mode with G. 
02/10/2014   bb      Changes to fix variable overflow issue while calculating
01/27/2014   arm     Reduce 2 slots from trm duration to compensate for 2 
                     slot buffer between trm reservation and wakeup.
01/15/2014   wsh     Added DSDS ASDiv support
                     sleep duration
01/14/2014   wsh     Fixed wake in the past crash in conflict adjustment
11/29/2013   vke     Fixed bug in wakeup manager changes
11/14/2013   arm     Merged wakeup manager changes for DSDS.
11/27/2013   dsp     FR17482 for Page blocking management.
11/08/2013   MBS     Fixed compiler error
09/25/2013   wsh     Sleep changes for bolt
09/19/2013   sat     Fixed HDR to not release the chain immediately after 
                     grant, before fast wakeup.
09/13/2013   rkc     Added system lost rf warmup cannot get RF lock for 60s.
09/06/2013   rkc     Add 1s buffer to extend sleep safeguard check. 
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/29/2013   rkc     Initialize rf_wait_state when deactivated during sleep.
08/29/2013   rkc     Initialize rf_wait_state when RF chain is granted.
08/28/2013   sat     Fixed Chatty F3 and extra processing by deregistering the 
                     wake up notify callback when HDR is not in sleep.
08/21/2013   rkc     Wakeup power optimized if chain is denied. 
08/12/2013   rmg     Added fix for FPD being skipped.
08/01/2013   smd     Used new secapi to get random number.
07/15/2013   sat     Fixed race condition with 2sec tick ISR and RTC disable
06/25/2013   smd     Fixed compile error in bolt build. 
06/05/2013   smd     Avoided slot boundary when calculating sleep duration.
05/06/2013   mbs     Fixed compiler errors for BOLT build
05/02/2013   ljl     Fixed wake slot for intra-scc.
                     Savd scc offset and rate.
04/29/2013   dsp     Moved code which stores cc_start into prev_cc_start 
                     from compute_next_wakeup() to reacq_init(). Also,
                     added hdrsrchsleep_update_prev_cc_start().
04/22/2013   arm     Added changes for DSDA coexistence manager. 
04/15/2013   ljl     Adjusted wakeup slot if using fast wakeup sleep time.
03/13/2013   vlc/arm Prepared new parameters for sleep info packet. 
03/20/2013   wsh     Fixed F3 to include full 64bit system time
02/25/2013   smd     Added the changes of adjusting sleep duration back.
02/22/2013   smd     Backed off the changes of adjusting sleep duration.
02/19/2013   smd     Adjusted sleep duration in slots dpending on if slpc_start()  
                     and compute_sleep_time() is in the same slot.
02/06/2013   smd     Don't use 1x wakeup point to calculate HDR warmup ticks. 
02/06/2013   vlc     Decreased reacq time by 1 slot. 
01/29/2013   smd     Updated online_univ_stmr when sleep end time changes.
01/31/2013   ljl     Configured modtime_ref and mptime_ref at wakeup.
09/20/2012   vke     Added hdrsrchsleep_get_cur_warmup_time() for logging current 
                               warmup time
01/18/2013   ljl     Increased reacq time by 1 slot.
01/10/2013   vlc     Optimized sleep time line.
01/15/2013   ljl     Set MCPM state to wakeup at deactivate RF warmup. 
                     Set reacq slot to 6 slots before 4HS RF settling being removed. 
01/07/2013   vlc     Changed hdrsrchsleep_enter_sleep() to allow for zero sleep 
                     time when returning from OFS state, removed Error Fatal.
11/19/2012   smd/dsp Added wakeup conflict adjustment for DIME.
11/14/2012   wsh     Fixed SRCH not able to do fastwake up 
10/30/2012   smd     Reset last_sci used for search win size adjust algorithm 
                     to 9 if it is the first sleep 
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
09/27/2012   ljl     Updated RGS before deassigning fingers. 
                     Passed us to slpc_sufficient_warmup_time().  
09/25/2012   rmg     Added support for including freq info in TRM interaction.
08/31/2012   smd     Checked if we have enough warmup time to wakeup. 
08/15/2012   ljl     Fixed the featurization.
07/13/2012   vlc     Removed LTE deinit functionality when there is no LTE 
                     coverage.
07/19/2012   smd     Dime sleep updates.
07/11/2012   arm     Updated idle div algo: If CC pkt is decoded before 
                     psinr meas, then assume good psinr.
07/09/2012   dsp     Removed unnecessary F3s. 
07/05/2012   arm     Reduced reacq slots due to faster AGC. 
06/27/2012   wsh     Fixed FPD related crash
06/26/2012   cnx     Fixed compiler errors on Dime.
05/30/2012   wsh     Added PRI_CHAIN_EVENT handler so fastwakeup can happen
05/21/2012   vlc     Fixed check of intra SCC sleep prior to LTE meas state 
                     transition. 
05/20/2012   arm     Fix idle div featurization bug.
04/27/2012   wsh     Fixed using wrong band info after IDLE OFS/ACQ
04/20/2012   arm     Supported diversity in reacq. 
04/20/2012   ljl     Increased detour threshold to 40ms. 
04/09/2012   vlc     Changed TRM API for LTE measurements. 
03/30/2012   wsh     Do not request TRM in rf_warmup if already owns chain
03/29/2012   smd     Added DIME sleep updates.
03/19/2012   smd     Added aleep controller API updates for DIME. 
02/06/2012   grl/smd Added support for SHDR during small SCI sleep. 
03/19/2012   ljl     Fixed Dime compiler errors. 
03/09/2012   grl     Extend DO RF warmup to account for TCM Restoration by 3 ms. 
03/05/2012   vlc     Added IRATMan interface to D2L reselection feature.  
02/28/2012   ljl     Changed the sleep slots after set warmup time.
02/23/2012   ljl     Changed warmup time before set duration for fast wakeup.
02/22/2012   arm     Fixed Idle Div bug for intra scc sleep. 
02/19/2012   wsh     Fixed deactivate fast wakeup waiting for RF
02/19/2012   wsh     Moved Req&Notify pending logic to hdrsrchrf.c 
02/06/2012   grl/smd Added support for SHDR during small SCI sleep. 
02/06/2012   arm     Resolved size conflict in function definitions in hdrsrch.  
02/01/2012   ljl     Deactivate decoder before tuning RF.
01/26/2012   ljl     Updated MCPM with sleep info when extending sleep.
01/25/2012   kss     Added sanity check for FW readiness during wakeup. 
01/19/2012   cnx     Adjust HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK.
01/09/2012   ljl     Changed warmup time and reacq time.
12/22/2011   vlc     Added code to call RF sleep after LTE meas for D2L. 
12/12/2011   ljl     Changed the extended warmup time to 27ms. 
12/05/2011   ljl     Added hdrsrchsleep_get_cur_warmup_reacq_time().
11/28/2011   ljl     Changed reacq time to 8 slots.
                     Added 5 more slots for intra-scc sleep buffer.
11/27/2011   grl     Removed obsolete sleep API.
11/11/2011   arm     HDR idle and access state diversity implemetation.
11/11/2011   ljl     Used extended warmup time to calculate fast sleep time. 
11/11/2011   smd     Fixed bugs of search window size adjustment.  
11/08/2011   ljl     Avoided 1x and hdr rf tune conflict.
11/07/2011   ljl/smd Changed reacq time to 12 slots and
                     warmup time to 10ms and 17ms for optimized and extended timeline. 
11/04/2011   ljl/smd Disabled WUTA for 8960 and used extended timeline for fast wakeup.
10/24/2011   ljl     Supported RPM optimized timeline.
10/17/2011   rmg     Removed calls to obsolete clk regim APIs.
10/06/2011   vlc     Removed hdrsrchsleep_start_sleep_now().  Removed changes
                     in hdrsrchsleep_start_sleep() to have FW app active for
                     RF tuning for LTE measurements.  Basically, returned sleep
                     time line to the way it was prior to LTE meas changes.
10/05/2011   ljl     Extended reacq time by 2 slots.
09/29/2011   vlc     Removed obsolete code.  Moved sleep substate setting to 
                     a better location.
09/12/2011   ljl     Increased reacq time by 2 slots.
08/17/2011   ljl     Adjusted reacq and rf warmup time.
07/22/2011   arm     Added F3 message to display sleep sub state. 
07/12/2011   arm     Resolved size conflict in function definitions in hdrsrch.  
05/31/2011   cnx     Added hdrsrchsleep_get_bcmcs_sleep_cycle().
05/19/2011   rmg     Updated HDR MCPM interface.
05/12/2011   arm     Fixed bug in hdrsrchsleep_set_sleep_sub_state. 
05/06/2011   vlc     Updated sleep processing for D2L reselection, moved sleep 
                     controller processing, left mDSP active for RF tuning.
04/27/2011   rmg     Added HDR MCPM support changes.
03/23/2011   vlc     Sent LTE deinit request if ORNL cycle is disabled. 
03/03/2011   arm     Fixed NPA crash due to overlapping NPA calls. 
02/17/2011   vlc     Added code to change TRM priority for LTE pilot 
                     measurements.
12/09/2010   pxu     Moved dynamic sleep algorithm function prototypes to hdrsrchstates.h  
11/20/2010   pxu     Added API to increase wake up cycle and added code to 
                     adjust wake up time on next sleep.  
11/19/2010   rkc     Fixed intra-scc sleep extension calc when RF not granted.
11/12/2010   ljl     Moved fw enable to reacq state.  
11/11/2010   ljl     Fixed compiler error. 
11/10/2010   ljl     Fast wakeup if the original wakeup time is at least 3 slots away.
10/27/2010   vlc     Fixed OtherRat Neighbor List wake up cycle calculation, 
                     added a few debug messages, updated comments.
10/15/2010   ljl     Set the minimum sleep time to 28 slots. 
10/14/2010   rkc     Added parens in HDRSRCHSLEEP_MINIMUM_SLOTS and moved
                     misleading intra-scc compute sleep debug msg.
10/14/2010   ljl     Adjusted the min sleep time.
                     Used the adjusted hstr count based on sleep time for FPD.
                     Fixed scc offset for intra-SCC and sub SCC.
10/13/2010   rkc     Added parens in HDRSRCHSLEEP_MINIMUM_SLOTS and moved
                     misleading intra-scc compute sleep debug msg.
10/11/2010   grl     Allow RF to control the RX front clocks for power savings.
09/15/2010   vlc     Modified code to leave clocks and RF enabled for LTE pilot 
                     measurements. 
09/07/2010   grl     Decreased the RF warmup time to be less than 18ms.
09/01/2010   vlc     Added support for DO->LTE pilot measurements/reselection.
08/30/2010   grl     Decreased the RF warmup time to 18ms.
08/20/2010   grl     Increased RF warmup time. 
08/16/2010   lyl     Supported FW power management with suspend/resume.
08/16/2010   grl     Make sure clocks are synced after TCXO shutdown.
08/10/2010   ljl     Set detour time and minimum sleep time based on RF 
                     warmup time.
08/03/2010   ljl     Increased max detour time from 40ms to 60ms.
07/30/2010   ljl     Increased the RF warmup time to 43ms.
07/09/2010   grl     Decreased the min F warmup time to 33ms.
06/21/2010   ljl     Increased the maximum RF warmup time to 45ms.
06/21/2010   cnx     Fixed compiler warnings.
06/17/2010   rmg     Added phase 2 clock gating changes.
06/03/2010   cnx     Fixed Klocwork errors.
06/01/2010   ljl     Changed reacq time to 7 slots.
05/12/2010   ljl     Disabled rx before hdr fw standby.
05/12/2010   rmg     Featurized clock gating changes.
05/11/2010   rmg     Added support for clock gating.
04/03/2010   pxu     Replaced correct block of code for HDR_IDLE_TIMELINE_V3. 
03/31/2010   pxu     Removed FEATURE_HDR_IDLE_TIMELINE_V3 feature checks and 
                     FEATURE_HDR_QSM feature.
03/28/2010   kss     Increased RF warmup and reacq time.
03/12/2010   grl     Resolved lint issue.
03/11/2010   ljl     Supported the fw timing state.
03/10/2010   grl     Added missing qw.h include
03/08/2010   ljl     Increased the reacq time to 10 slots.
03/04/2010   rkc     Enabled/Disabled CDMA Rx Chain 0 Clocks.
02/26/2010   rkc     Perform fast wakeup if sleep timeline delayed.
02/20/2010   lyl     Updated for initial slotted mode functional;
11/11/2009   rkc     Fixed #elif/define error.
11/11/2009   rkc     Added NPA support.
11/03/2009   rkc     Increase SCMM WUTA start times and limits.
07/16/2009   lyl     Supported CC config MSGR interface.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
05/18/2009   lyl/ljl Updated for firmware MSGR interface.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
04/14/2009   ljl     Fixed compile error.
04/07/2009   grl     Do conflict adjust only for s-idle/idle modes.
04/02/2009   ljl     Avoided to wakeup at frame boundary.
03/20/2009   rkc     Passed param rup_subtype to hdrsrchutil_update_nset_pilots
03/19/2009   rkc     Increased WUTA start time/max to 14ms/21ms.
03/18/2009   rkc     Removed rf.h.
03/09/2009   smd     Added support for pilot measurement velcro solution.
02/27/2009   rkc     Moved conflict adjust to hdrsrchsleep_extend_sleep().
02/13/2009   rkc     Fixed cc_start to now calculation for enh sleep logging.
02/06/2009   rkc     Set conflict id when computing intra-scc sleep time.
02/04/2009   lyl     Fixed the lint errors.
02/02/2009   smd     Added function hdrsrchsleep_get_sleep_slots().
01/30/2009   ljl     Extended resume agc time for SC2x.
11/21/2008   kss     Back out sleep time optimization.
11/20/2008   rkc     Fixed lint errors.
11/20/2008   kss     Removed/replaced intlocks.
11/17/2008   grl     Do not use intra-scc reacq algorithm in certain cases.
11/13/2008   kss     Disable decoder isr before tasklock to avoid pending int.
11/13/2008   lyl     Renamed to more meaningful name of qpch_enabled & 
                     hdrsrchsleep_is_qpch_enabled().
11/07/2008   rkc     Added parens to HDRSRCHSLEEP_IDLE_BUFFER_SLOTS and
                     fixed double adjust callback.
10/14/2008   rkc     Skipped OFS when entering sleep if intra-scc enabled.
10/13/2008   rkc     Fixed sense of saving when adjusting for cpu conflict.
10/09/2008   lyl     Added QPCH support.
09/18/2008   rkc     Added Intra-SCC sleep support.
09/16/2008   smd     Added hdrsrchsleep_get_negotiated_sci()
07/29/2008   knp     Included the code for sleep time optimization.
07/18/2008   grl     Handle conflicts that push us earlier than our sleep time.
06/16/2008   lyl     Used hdrsrchbc_get_bc_reason() to support high priority
                     BCMCS.
05/16/2008   rkc     Increased HDRSRCHSLEEP_DETOUR_MS from 30ms to 40ms.
05/15/2008   grl     Added margin to HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK
05/05/2008   rkc     Added clkrgm control of CLKRGM_RESOURCE_CDMA_TX
04/17/2008   rkc     Increase reacq time when CGPS is active.
04/17/2008   rkc     Increase reacq time when CGPS is active.
04/11/2008   rkc     Added clkrgm control of CLKRGM_RESOURCE_CDMA_DEMOD
04/10/2008   rkc     Increased max WUTA range to 15 ms.
03/28/2008   grl     Added rtc_off return value to sleep adjust function.
03/27/2008   ljl     Added the avoided channel list to nset update function.
03/27/2008   ljl     Cleaned up the Rev B macro.
01/30/2008   ljl     Removed the obsolete comment related sleep.
01/22/2008   jyw     Compensated the sleep adjust in extended sleeps.
01/11/2008   jyw     Changed the sleepctl programming sequence.
01/03/2008   rkc     Increased minimum sleep to 28 slots.
12/26/2007   grl     Fixed WUTA drifts out of permissible range issue.
12/26/2007   grl     Disallow TCXO shutdown during deactivation.
12/12/2007   grl     Increased the range of the RF warmup time in WUTA.
11/26/2007   jyw     Fixed the compiling errors.
11/20/2007   jyw     Used hdrsrch sleep macros.
11/14/2007   jyw     Added the RTCcnt latch when hdrsrch is going to sleep.
11/13/2007   jyw     Added the power strobe event id.
11/11/2007   ljl     Configured pn to ASP.
11/08/2007   rkc     Spread wakeup on larger sleep when neg_sci is smaller.
11/05/2007   grl     Fixed logging calculation using wrong sci value.
10/17/2007   grl     Merged Rev B changes from branch.
09/27/2007   grl     Added WUTA info for rev A sleep log pkt.
08/19/2007   grl     Resolved Lint High warnings.
08/14/2007   grl     Marked wakeup reason as fast during fast wakeups.
07/06/2007   ljl     Supported rotator push.
06/02/2007   grl     Implemented the WUTA Algorithm.
04/10/2007   mt      Added logic to initiate OFS state if RUP request is pending.
03/20/2007   rkc     Remove unused struct fields ccc_slot and max_sleep_cycle
01/17/2007   ljl     Added HDR SRCH AFC stm.
01/09/2007   grl     Recalculate the cc_start when extending HDR sleep.
12/21/2006   grl     Moved CPU clock increase to earlier in timeline.
12/21/2006   grl     Fixed fast wakeups during small SCI idle operation.
11/27/2006   grl     Added CPU conflict adjustment support.
11/15/2006   grl     Reverted REACQ time increase of one slot.
11/15/2006   kss     Handle when RTC adjust is delayed until after RTC on.
11/15/2006   grl     Increased the REACQ time by one slot.
11/10/2006   mt      Added interface to retrieve the sci used for lasy sleep cycle.
11/10/2006   mt      Moved VARIANCE_REDUCTION_NV_ENABLE to hdrsrchi.h
10/27/2006   etv     Disabled on-arrival RX pkt delivery before sleep.
10/27/2006   kss     Adjust RTC for sleep when RTC is still disabled.
10/24/2006   grl     Extend sleep for correct time when the wakeup fails.
10/23/2006   grl     Fixed time jump of one frame.
10/16/2006   ljl     Do not change warmup time for deactivate cmd.
10/13/2006   grl     Pulled in BCMCS timeline.
10/03/2006   grl     Added TRM priority for small SCI paging operation.
09/28/2006   grl     Fixed CPU conflict during simultaneous idle/idle.
09/16/2006   mt      Changed scc offset restoration code for wake ups.
09/15/2006   mt      Changed value of HDRSRCHSLEEP_REACQ_TIME_SLOTS_DEFAULT.
09/07/2006   ljl     Disabled HDR App before fast decoder deactivation.
09/06/2006   jyw     Added FEATURE_DYNAMIC_CLOCK_FREQUENCY_CHANGING.
08/29/2006   mt      Removed sleep overhead slots.
08/28/2006   sq      Added support for FEATURE_HDR_QSM
08/25/2006   mt      Optimized going to sleep processing.
08/24/2006   grl     Shutoff DAST when using long SCI.
08/23/2006   grl     Implemented DAST algorithm.
08/20/2006   mt      For BCMCS wakeups use fat reacquisition time.
08/15/2006   mt      Restored HDR_bestAspFL MDSP register after wakeup.
08/14/2006   rmg     Added HDR Clk disable feature
08/08/2006   mt      Fixed HDR not voting to sleep when feature MPC undefined.
08/01/2006   grl     Perform fast decoder deactivation only when sleeping.
07/27/2006   mt      Moved forced preamble detection to hdrsrchreacq.
07/24/2006   jyw     Added power profiling strobes.
06/28/2006   jyw     Added support for MPC.
07/10/2006   mt      Added support for sleep timeline optimization.
06/14/2006   mt      Fixed BCMCS featurization
06/12/2006   sq      Added support of forced detection of missing preamble
06/07/2006   ljl     Supported idle OFS with small SCI.
05/19/2006   grl     Fixed crash when going to sleep without CC hash.
05/12/2006   sq      Disable idle OFS when BCMCS is enabled
05/11/2006   sq      Activate decoder at tune_done 
04/28/2006   sq      Fixed the wrong wake-up reason for 426ms sleep
04/21/2006   ljl     Added a parameter to hdrsrchafc_stop_fll_tracking().
04/21/2006   jyw     Added new featurized warmup and reacq time constants.
04/05/2006   sq      Added code review comments
03/31/2006   sq      Added definition of HDRSRCHSLEEP_RF_REASON
03/30/2006   sq      Added support for BCMCS slotted sleep
03/23/2006   grl     Save the DC offsets before we turn off RTC.
03/22/2006   sq      Supported fast wake-up after receive non-zero bom period
11/30/2005   sq      Added new re-acq failure reason in reacq_info log pkt
11/16/2005   sq      Limit HDR SCI to be <= 0xc
11/09/2005   sq      Called HDRMDSP_SET_MMSEINV if needed
10/18/2005   grl     Added comment to endif of FEATURE_HDR_REVA_L1
10/18/2005   sq      Added FEATURE_HDR_REVA_L1
10/07/2005   sq      Removed compilation warning
09/19/2005   sq      Supported SCIs smaller than 6 ( 426ms )
09/19/2005   sq      Changed system loss reason
08/18/2005   ljl     Saved cached sectors
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
08/11/2005   sq      Fixed an issue with 426ms sleep
07/28/2005   ds      Added hash channel list parameter to NSET update
07/27/2005   sq      Added support for BCMCS page monitoring
07/25/2005   ljl     Added broadcast and directed nset update 
07/13/2005   ajn     Inform HDRTS of aborted sleep
07/12/2005   sq      Added support for enhanced idle state protocol
06/30/2005   sq      Changed OFS API name
05/31/2005   grl     Added support for TRM and TCXOMgr.
04/25/2005   jyw     Added probes for HDR sleep/power timeline profiling
04/04/2005   ddh     Moved sclk conversions to timetick
03/31/2005   grl     Changed HDR's use of trk_lo to be GPS friendly
03/23/2005   hrk     Replaced hdrdec_deactivate by hdrsrchutil_dec_deactivate
02/14/2005   sq      Provided sci info for "sleep_info" log packet
01/27/2005   sq      Restored RF warm up time back to 9ms from 5ms
01/19/2005   kss     Removed wait before hdrdec_deactivate().
01/19/2005   ajn     Protect extend_sleep from setting a wakeup in the past
11/22/2004   kss     Wait additional time before deactivating decoder.
10/25/2004   ajn     Increased deactivate extension from 1 slot to 1 frame
09/27/2004   ajn     Reduced RF warmup time if sleep task owns TCXO warmup
09/10/2004   ajn     Adjusted RF warmup time and min RF warmup time
08/30/2004   aaj     Added enum type for BCMCS status
08/12/2004   ajn     sleephdr -> sleepctl
07/30/2004   sq      Supported fast wake-up when BC is enabled
07/22/2004   ajn     Added common signals to state machine
06/25/2004   sq      Bug fix in SLEEP INFO log packet
06/01/2004   sq      Replaced srch4_enable with new API
01/13/2004   sq      Reduced HDRSRCHSLEEP_RF_WARMUP_MS from 15ms to 8ms
12/02/2003   aaj     Do not enable HDR application for deactivate fast wakeup
12/02/2003   sq      Merged in code review comment
11/26/2003   sq      Changed the overhead time to 12 slots from 20 slots
11/21/2003   sq      Supported 3 new log packets( acq,reacq,sleep )
11/18/2003   aaj     Add rf sleep. Save & restore SCC offset
11/13/2003   aaj     Do not wait for DSP to be enabled in deactivate wakeup
11/11/2003   aaj     Saved/restored DSP variables for DSP enable/disable
11/04/2003   bt      Replaced srch4_hdr_en/disable with srch4_enable iface.
10/28/2003   aaj     Support enable/disable of hdrmdsp and srch4 
10/22/2003   aaj     Removed call to stubbed out rf_hdr_sleep
10/21/2003   ajn     Preserve wakeup slot-in-frame when a fast wakeup occurs
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/22/2003   aaj     Restored slot based sleep by removing & ~0xf operation
09/22/2003   ajn     More Hybrid mode integration.
09/18/2003   ajn     Merge from Jaguar orphan branch
09/17/2003   aaj     Support for splitting OFS_STATE in OFS_TC and OFS_IDLE.
09/17/2003   sq      HDR Traffic state OFS support needs change to ofs_init
09/10/2003   mpa     Converted F3 messages to use new MSG2.0 HDR search SSID
08/26/2003   ajn     Removed hdrSleeping status check for MSM6500.
03/14/2003   aaj     Added 2 slots of margin to the sleep timeline reacq time
03/11/2003   ajn     Added adjust_ & reset_sci functions, removed alter_sci
                     Allows for SCI to climb rapidly if slews are small.
03/11/2003   ajn     Updated Copyright notice
03/06/2003   aaj     Do not enter OFS if sleep mode is 426ms
02/14/2003   aaj     Do not reset SCI to 0 when 426ms sleep cmd is sent
11/15/2002   ajn     Advance lock required at time when lock is denied.
10/30/2002   ajn     Slot based sleep duration support added.
09/20/2002   aaj     Print Pilot info just before entering sleep
07/31/2002   ajn     Don't clear Chipx8_On signal on transition to REACQ
06/11/2002   ajn     Fixed fast wakeup critical section
05/30/2002   ajn     Fixed critical section (Wakeup & Deactivate at same time)
05/29/2002   ajn     Complete fast wakeup before deactivating.
05/15/2002   ajn     Sync with Time-of-Day moved to hdrts.c
04/30/2002   aaj     Deactivate HDR decoder before initiating h/w sleep
03/20/2002   ajn     Don't force HDR awake on keypress events.
03/12/2002   ajn     Moved enable demod to reacq (After tune is complete)
03/04/2002   kss     Added hdrdec activate/deactivate calls.
02/25/2002   ajn     Added hard limit of 60 seconds on HDR sleep.
02/21/2002   ajn     Reacq state now initializes own search list during RF
                     warmup period.
02/19/2002   sst     Changed hdrsrchutil_prepare_reacq_list to
                      hdrsrchreacq_prepare_search_list
02/19/2002   sst     Modified hdrsrchutil_prepare_reacq_list() to reflect
                      new input param
02/16/2002   aaj     Support for multiple CCC sleep. Introduced HDR SCI
02/12/2002   ajn     Ensure demod is not left disabled if no time to sleep.
02/06/2002   ajn     hdrerrno -> errno
01/31/2002   aaj     F3 messages for total finger rssi and sleep duration
01/16/2001   ajn     Split Reacq substate into hdrsrchreacq.c
01/16/2001   ajn     Switch to Idle state only from on_time_valid signal.
12/31/2001   ajn     Sleep extended if RF lock cannot be obtained.
12/14/2001   ajn     Check return value in fast wakeup.
                     Reimplemented DEACTIVATE cmd; force wakeup before exit.
12/13/2001   aaj     Change reacq time to 18 slots from 14 slots
12/12/2001   aaj     resetting decoder sequence now on wakeup
12/07/2001   aaj     Idle handoff threshold support for OFS/SLEEP
12/05/2001   ajn     Corrected timestamps after entering sleep
11/27/2001   aaj     Clear TimeValid signal while entering sleep
11/26/2001   aaj     Reworked sleep for including RF shutdown support
11/09/2001   aaj     Timetest support for srch profiling
11/07/2001   aaj     Prepare reacquisition list just before CPU sleep
10/31/2001   ajn     Deassign fingers in start_sleep
10/29/2001   ajn     RF warmup support after returning from OFS state
10/24/2001   ajn     First checkin.
10/16/2001   ajn     Created this file (Many functions from hdrsrchidle.c)

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrutil.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchi.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrsrchfing.h"
#include "hdrsrchrf.h"
#include "hdrsrchlog.h"
#include "hdrsrchafc.h"
#include "hdrsrchdef.h"

#include "hdrts.h"

#include "hdrrx.h"
/* included this file temporarily to allow RX to reset seq nos on wakeup
   replace this with hdrrxsrch.h which defines searcher-RX interfaces */

#ifdef FEATURE_HDR_DIME_MODEM
#include "slpc.h"
#else
#include "sleepctl.h"
#include "sclk.h"
#endif /* FEATURE_HDR_DIME_MODEM */

#include "timetick.h"
#include "qw.h"
#include "trm.h"
#include "hdrdec.h"

#include "hdrmdsp.h"
#include "hdrmdspmcr.h"
#include "hdrsrchmsg.h"
#include "hdrdebug.h"
#include "err.h"

#include "srch_hw.h"
#include "hdrsrchafc.h"


#include "ran.h"
#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrchltemeas.h"
#endif /* FEATURE_HDR_TO_LTE */

#include "hdrsrchmcpm.h"
#include "hdrsrchrfdiv.h"

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
#include "hdrsrchidlei.h"
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_BOLT_MODEM
#include "srch_hdr.h"
#endif /* FEATURE_HDR_BOLT_MODEM */

#include "hdr_log_qsh.h"
#include "hdrutil.h"


/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define HDRSRCHSLEEP_REACQ_TIME_SLOTS_DEFAULT             7
  /* Allow these many slots for re-acquisition, and finger assignment. */
  /* Subtract 8HS for RxAGC settling */

#define HDRSRCHSLEEP_REACQ_NORMAL_AGC_TIME_SLOTS_DEFAULT  5
  /* Allow these many slots for re-acquisition, and finger assignment. */

#define HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_SLOTS_DEFAULT  7
  /* Allow these many slots for re-acquisition, and finger assignment. Added 3slots for Nikel*/

#define HDRSRCHSLEEP_RF_WARMUP_OPTIMIZED_US            6000
  /* The warmup time with MCPM optimized sleep timeline */

#define HDRSRCHSLEEP_RF_WARMUP_DEFAULT_US              24000
  /* The warmup time with MCPM extended sleep timeline */

#define HDRSRCHSLEEP_MAX_RF_WARMUP_US                  60000
  /* Largest RF Warmup time permitted by WUTA */

#define HDRSRCHSLEEP_MAX_RF_WARMUP_WARNING_US          23000
  /* Reasonable RF Warmup time, used to print warning/error msg */

#define HDRSRCHSLEEP_RF_WARMUP_US              \
           ( ( hdrsrchsleep.cur_warmup_time_is_optimized == TRUE )?  \
             ( HDRSRCHSLEEP_RF_WARMUP_OPTIMIZED_US ): \
             ( HDRSRCHSLEEP_RF_WARMUP_DEFAULT_US ) )
  /* Allow this many usec for RF to warmup */

#define HDRSRCHSLEEP_MIN_RF_WARMUP_US          7000
  /* Smallest RF Warmup time permitted by WUTA. Reduced by 5ms for Nikel */

#define HDRSRCHSLEEP_RF_WARMUP_DC_US           1000
  /* DC of 1000 microseconds above the filtered output of the warmup time slack. */

#define HDRSRCHSLEEP_RF_WARMUP_FASTWAKEUP_OFFSET_US           3000
  /* The warmup time offset to use for fastwakeup case */

/* MACROS added to support sleep timeline optimization */

#define HDRSRCHSLEEP_REACQ_TIME_ADJ_BITS \
        (( ((uint32) hdrsrch_dbg_msk) >> 12) & 0xF)
  /* Bits [15:12] from HDR L1 debug mask to indicate 
     configurable reacquisition time in slots */

#define HDRSRCHSLEEP_CGPS_REACQ_SLOTS 0
  /* No extra reacq time added if feature is not defined. */

#define HDRSRCHSLEEP_REACQ_TIME_SLOTS \
        (HDRSRCHSLEEP_REACQ_TIME_ADJ_BITS ? \
         HDRSRCHSLEEP_REACQ_TIME_ADJ_BITS + HDRSRCHSLEEP_CGPS_REACQ_SLOTS : \
         HDRSRCHSLEEP_REACQ_TIME_SLOTS_DEFAULT + HDRSRCHSLEEP_CGPS_REACQ_SLOTS )
  /* HDRSRCHSLEEP_REACQ_TIME_ADJ_BITS is not all zero use it
     otherwise use the default value for reacquisiton time */

#define HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_ADJ_BITS \
        (( ((uint32) hdrsrch_dbg_msk) >> 24) & 0xF)
  /* Bits [24:27] from HDR L1 debug mask to indicate configurable 
     reacquisition time in slots when using the resume AGC acq
     algorithm. */

#define HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_SLOTS \
        (HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_ADJ_BITS ? \
         HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_ADJ_BITS : \
         HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_SLOTS_DEFAULT)
  /* HDRSRCHSLEEP_REACQ_RESUME_AGC_TIME_ADJ_BITS is not all zeros use it
     otherwise use the default value for reacquisiton time */

#define HDRSRCHSLEEP_BCMCS_REACQ_BUF_SLOTS 3
  /* An extra 3 slots gives us 2 extra reacq attempts in the worst case according
     to field logs. */

#define HDRSRCHSLEEP_MIN_RF_WARMUP_MS         2
  /* Skip warmup if less than this time remains */

#define HDRSRCHSLEEP_MIN_RF_WARMUP_SCLK       \
        TIMETICK_SCLK_FROM_MS( HDRSRCHSLEEP_MIN_RF_WARMUP_MS )
  /* Skip warmup if less than this time remains */

#define HDRSRCHSLEEP_IDLE_BUFFER_SLOTS        \
          ( ( ( hdrsrch_state_id != HDRSRCH_SLEEP_STATE ) ? 2 : 0 ) \
           + HDRSRCHSLEEP_DIV_TURN_OFF_SLOTS )
  /* Extra number of slots to add to minimum sleep duration when computing if
     sleep is possible; only non-zero in idle state.  This will account for the
     time between the idle computation and the actual sleep state
     computation.  */

#define HDRSRCHSLEEP_DIV_TURN_OFF_SLOTS        \
          ( ( hdrsrchrfdiv_diversity_is_enabled() ) ? 1 : 0 )
  /* Add extra slot for turning off diversity to minimum sleep duration
     when computing if sleep is possible; only non-zero in idle state.
     This will account for the time between the idle computation and the actual
     sleep state computation.  */

#define HDRSRCHSLEEP_SLEEP_BUFFER_SLOTS        2
  /* Extra number of buffer slots available before wakeup then only reprogram
     sleep sleepctl */


#define HDRSRCHSLEEP_INTRASCC_BUFFER_SLOTS    ( 10 + HDRSRCHSLEEP_IDLE_BUFFER_SLOTS )
  /* For intra-SCC, extra number of slots of buffer for minumum sleep slots.
     Note: This also takes into account rounding up the WUTA conversion from
     microseconds to slots. */

#define HDRSRCHSLEEP_CONVERT_US_TO_SLOTS(x) ( x * 6 ) / 10000
  /* Convert microseconds to slots.  There are 600 slots per second,
     or 6/10000 slots per us */

#define HDRSRCHSLEEP_MINIMUM_SLOTS                                                   \
     ( hdrsrchsleep.intra_scc_start_slot_valid ?                                       \
       ( HDRSRCHSLEEP_CONVERT_US_TO_SLOTS(hdrsrchsleep.rf_warmup.cur_warmup_time_us) + \
         HDRSRCHSLEEP_INTRASCC_BUFFER_SLOTS ) :                                        \
       28 )

     /* Calculate minimum sleep time by considering Rf warmup time */

  /* For Intra-SCC Sleep, enforce a minimum of WUTA slots +
     HDRSRCHSLEEP_INTRASCC_BUFFER_SLOTS (reacq slots are accounted in wake_slot).
     Otherwise...
     Enforce a minimum of 26.7 ms of sleep (more than RF warmup time).
     Note: The buffer slots take into account rounding up the WUTA conversion
           from microseconds to slots. */

#define HDRSRCHSLEEP_MAXIMUM_SLOTS           (60 * HDRTS_SLOTS_PER_SEC)
  /* Enforce a maximum time for sleep.  Over this, declare system lost */
  
#define HDRSRCHSLEEP_EXTEND_SLEEP_SAFEGUARD_SLOTS   (HDRSRCHSLEEP_MAXIMUM_SLOTS + HDRTS_SLOTS_PER_SEC)
  /* Add 1s buffer to extend sleep safeguard to cover boundary cases close to 60s timeout */

#define CHIPS_PER_SLOT                        2048uL
  /* Conversion from slots to chips */

#define CHIPS_PER_SLOT_SHIFT                  11
  /* Bit shift for conversion between slots and chips */

#define SYSCLK_PER_SLOT_SHIFT                 (CHIPS_PER_SLOT_SHIFT+3)
  /* Bit shift for conversion between slots and chipx8s */

#define HDRSRCHSLEEP_BEFORE_CPU_CONFLICT_SCLK TIMETICK_SCLK_FROM_MS( 120 )
  /* If the next other modem wakeup is less than this time before the next
     HDR wakeup, give the HDR timeline more slack. */

#define HDRSRCHSLEEP_AFTER_CPU_CONFLICT_SCLK  TIMETICK_SCLK_FROM_MS( 10 )
  /* If the next other modem wakeup is less than this time after the next
     HDR wakeup, give the HDR timeline more slack. */

#define HDRSRCHSLEEP_CONFLICT_ADJUST_SLOTS    4
  /* The number of slots to relax the timeline in the case of a CPU 
     conflict. */

#define HDRSRCHSLEEP_HALF_SLOT_DELAY_USEC    900
  /* Half slot delay in uSec for the DSP registers to take effect */

#define HDRSRCHSLEEP_FULL_SLOT_DELAY_USEC   1700
  /* Full slot delay for the sample RAM to fillup with valid I&Q samples
     after chipx16 clock is started during wakeup */


#define HDRSRCHSLEEP_DETOUR_MS                \
    ( 40 + hdrsrchsleep.rf_warmup.cur_warmup_time_us/1000 )

#define HDRSRCHSLEEP_DETOUR_SCLK \
        TIMETICK_SCLK_FROM_MS( HDRSRCHSLEEP_DETOUR_MS )
  /* Maximum time red'd for SFS, OFS, or OSS (more than RF tune time) */

#ifdef FEATURE_HDR_TO_LTE
#define HDRSRCHSLEEP_ONE_LTE_MEAS_MS         200
  /* Maximum time required to make 1 LTE Pilot frequency measurement in msec unit */

#define HDRSRCHSLEEP_ONE_LTE_MEAS_SLOTS     \
        MS_TO_SLOTS( HDRSRCHSLEEP_ONE_LTE_MEAS_MS )
  /* Maximum time required to make 1 LTE Pilot frequency measurement in slot unit */

#define HDRSRCHSLEEP_ONE_LTE_MEAS_SCLK       \
        TIMETICK_SCLK_FROM_MS( HDRSRCHSLEEP_ONE_LTE_MEAS_MS )
  /* Maximum time required to make 1 LTE Pilot frequency measurement in sclk unit */
#endif /* FEATURE_HDR_TO_LTE */

#define HDRSRCHSLEEP_DETOUR_MAX_SLOTS   MS_TO_SLOTS(HDRSRCHSLEEP_DETOUR_MS)
  /* Maximum time req'd for SFS, OFS, or OSS (more than RF tune time) */

#define HDRSRCHSLEEP_MIN_SLOTS_FOR_DETOUR  \
        ( HDRSRCHSLEEP_MINIMUM_SLOTS + HDRSRCHSLEEP_DETOUR_MAX_SLOTS )
  /* Minimum sleep time required for a detour to SFS, OFS, OSS */

#define DETOUR_OK( sleep_slots ) \
        ( (sleep_slots) >= HDRSRCHSLEEP_MIN_SLOTS_FOR_DETOUR )
  /* Returns TRUE or FALSE based on sufficient time for "Detour" */


#define HDRSRCHSLEEP_DEACTIVATE_WARMUP_US   1500
  /* Deactivation "RF Warmup time" */

#define HDRSRCHSLEEP_DEACTIVATE_EXTENSION     16
  /* Sleep extention (in slots) if deactivate cannot wakeup immediately */

#define MS_TO_CHIPS(ms) ((ms)*1228uL)
  /* Conversion from milliseconds to chips */

#define HDRSRCHSLEEP_MIN_WAKE_TIME_MS   100
#define HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK \
        TIMETICK_SCLK_FROM_MS( HDRSRCHSLEEP_MIN_WAKE_TIME_MS )
  /* The minimum time in ms we need to be allowed to awake before we'll 
     wake up. */

#define HDRSRCHSLEEP_MAX_SCI                  3
  /* Maximum sleep "Slot Cycle Index".
     0 = 5.12s, 1=10.24s, 2=20.48s, 3=40.96s */

/* The reason used when requesting RF depends on whether BCMCS is enabled
   or not. TRM uses this information to decide how to grant receivers   */

/* The reason for paging is either BCMCS, Intra-SCC Paging,
   SMALL SCI Paging or Normal Paging. */
#ifdef FEATURE_HDR_BCMCS
#ifndef FEATURE_HDR_PAGE_BLOCKING_MGMT
#define HDRSRCHSLEEP_RF_REASON                                               \
        ( ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED ) ?                  \
          hdrsrchbc_get_bc_reason() :                                         \
          ( ( hdrsrchsleep.until <= 6 ) && ( hdrsrchsleep.neg_sci <= 6 ) ?   \
           ( hdrsrchsleep_get_intra_scc_valid() ?                             \
            HDRSRCHRF_SMALL_SCI_PAGE_CONTINUATION : HDRSRCHRF_SMALL_SCI_PAGE ) : \
           ( hdrsrchsleep_get_intra_scc_valid() ?                             \
             HDRSRCHRF_DEMOD_PAGE_CONTINUATION : HDRSRCHRF_DEMOD_PAGE ) ) )
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT*/

#ifdef FEATURE_FULL_TIME_SHDR_3
/* Whether we are in a mode which requires conflict adjustment. With FTS-3
   we always do conflict adjustment if in FTS-3 mode. We don't in FTS-4 
   (no SHDR).*/
#define HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT                                   \
        ( hdrsrchrf_is_shdr_supported() )
#else
/* Whether we are in a mode which requires conflict adjustment. */
#define HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT                                   \
        ( ( hdrsrchrf_is_shdr_supported() ) &&                                \
          ( ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED ) ||                \
             ( ( hdrsrchsleep.until <= 6 ) && ( hdrsrchsleep.neg_sci <= 6 ) ) ) )
#endif /* FEATURE_FULL_TIME_SHDR_3 */
#else 
/* The reason for paging is either Intra-SCC Paging, SMALL SCI Paging or
   Normal Paging. */
#ifndef FEATURE_HDR_PAGE_BLOCKING_MGMT
#define HDRSRCHSLEEP_RF_REASON                              \
        ( ( hdrsrchsleep.until <= 6 ) && ( hdrsrchsleep.neg_sci <= 6 ) ? \
          ( hdrsrchsleep_get_intra_scc_valid() ?                              \
            HDRSRCHRF_SMALL_SCI_PAGE_CONTINUATION : HDRSRCHRF_SMALL_SCI_PAGE ) : \
           ( hdrsrchsleep_get_intra_scc_valid() ?                             \
             HDRSRCHRF_DEMOD_PAGE_CONTINUATION : HDRSRCHRF_DEMOD_PAGE ) )
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT*/

#ifdef FEATURE_FULL_TIME_SHDR_3
/* Whether we are in a mode which requires conflict adjustment. */
#define HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT                                   \
          ( hdrsrchrf_is_shdr_supported() )
#else
/* Whether we are in a mode which requires conflict adjustment. */
#define HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT                                   \
          ( ( hdrsrchrf_is_shdr_supported() ) &&                              \
             ( ( hdrsrchsleep.until <= 6 ) && ( hdrsrchsleep.neg_sci <= 6 ) ) ) 
#endif /* FEATURE_FULL_TIME_SHDR_3 */

#endif /* FEATURE_HDR_BCMCS */

#define HDRSRCHSLEEP_HS_PER_SCC               512
#define HDRSRCHSLEEP_SUBSYNC_HS_LOW           226
#define HDRSRCHSLEEP_SUBSYNC_HS_HIGH          255
  /* Boundaries to define sub sync wakeups in half slots */

#define HDRSRCHSLEEP_CONFLICT_END_MARGIN_SCLK TIMETICK_SCLK_FROM_MS( 10 )
  /* Extra margin on the end of the conflict wakeup added. */

#define HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK   120
  /* The minimum number of sclks from now we can set the wakeup point to be. */

#define HDRSRCHSLEEP_RX_COMPARE_ADVANCE_CHIPS 288
  /* Number of chips to advance the DSP's Rx interrupt on wakeup.  This 
     advances the DSP's Rx processing so that there are enough samples to
     AGC over a dispatch window centered on the last best pilot. */

#define HDRSRCHSLEEP_LOG_CONFLICT_TIME_PERIOD_SCLK TIMETICK_SCLK_FROM_MS( 5 )
  /* Used to calculate number of instances where HDR wakes up within "TIME_PERIOD" of
     1x wakeup.  TIME_PERIOD is set to 5ms for now.
  */


#ifdef FEATURE_HDR_DIME_MODEM

#define HDRSRCHSLEEP_CONFLICT_END_MARGIN_XO_TICK   192000
  /* Extra margin on the end of the conflict wakeup added.
     10 ms = 6 slots = 12288 chips
     12288 chips = (12288*8)*19.2/(1.2288*8) = 192000 xo tick
   */

#define HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK  70312
  /* The minimum number of xo tick from now we can set the wakeup point to be.
     120 sclk = 120/32768*19200000 = 70312
  */

#define HDRSRCHSLEEP_LOG_CONFLICT_TIME_PERIOD_XO_TICK   96000
  /* Used to calculate number of instances where HDR wakes up within "TIME_PERIOD" of
     1x wakeup.  TIME_PERIOD is set to 5ms for now.
     5ms = 3 slots = 6144 chips
     6144 chips = (6144*8)*19.2/(1.2288*8) = 96000 xo ticks
  */

#define HDRSRCHSLEEP_SLOT_BOUNDARY_BUFF_CX8           160*8
   /* buffer time to slot boundary. Since the maximum search window size is 320 in
      reacq, 160 chips buffer time should be good enough
   */

#define HDRSRCHSLEEP_AWAY_FROM_SLOT_BOUNDARY_CX8           160*8
  /* slpc start should be 160 chips away from slot boundary */

#endif /* FEATURE_HDR_DIME_MODEM */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
#define HDRSRCHSLEEP_LOG_IDLE_DIV_MISSED_SYNC_CC   0x1
#define HDRSRCHSLEEP_LOG_IDLE_DIV_PSINR_AVG        0x2 
#define HDRSRCHSLEEP_LOG_IDLE_DIV_PREV_SLEEP_TIME  0x4
#define HDRSRCHSLEEP_LOG_IDLE_DIV_REACQ_FAILED     0x8
  /* Reason why RX diversity chain is enabled
     Bit 0: syncc_cc_missed
     Bit 1: prev_psinr_avg is less than threshold
     Bit 2: prev_sleep_time is higher than threshold
     Bit 3: reacq_failed 
  */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#define HDRSRCHSLEEP_NUM_SLOTS_IN_60SEC   36000
  /* Number of 1.67ms slots in 60 seconds, i.e., 60000 / 1.67 */

#define HDRSRCHSLEEP_SLOTS_TO_SCLK_TICKS( slots )                                  \
  ( TIMETICK_SCLK_FROM_MS ( SLOTS_TO_MS ( slots ) ) )

#define HDRSRCHSLEEP_CX8_TO_US(cx8)  ( (cx8) * 2500 / 8192 / 3 )
  /* 3HS = 2500uS, 1HS = 8192cx8 */

#define HDRSRCHSLEEP_MS_TO_XO_TICK(ms)  ((ms) * 19200)
  /* XO is running at 19.2mHZ, so 1ms has 19.2k ticks */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
#define HDRSRCHSLEEP_MAX_ALLOWED_SLOTS_FROM_A_SCI_BOUNDARY    20
 /* Maximum slots from a boundary to be considered as waking up at a boundary */

#define HDRSRCHSLEEP_MAX_ALLOWED_OVHD_DENIALS                 4
 /* Maximum allowed TRM chain denials after which we increase to OVHD_HI priority */ 
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT*/

#define HDRSRCHSLEEP_FAST_WAKEUP_SLOT_OFFSET_MAX  12
  /* The max slot offset from frame boundary at fast wakeup */

#define CHIPS_PER_2_SLOTS      (2*CHIPS_PER_SLOT)

/* <EJECT> */
/*==========================================================================

                     HDR SEARCH SLEEP STATE DATA

As much IDLE data is lumped together in one structure, so that it becomes
possible to dynamically allocate acquisition data memory during the
IDLE state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchsleep.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchsleep  (*hdrsrchsleep_struct_ptr)

==========================================================================*/

//static uint8       hdrsrchsleep_sci_period[] = { 0, 1, 3, 7 };
  /* period in CCC for which to sleep, will be used later */

/*---------------------------------------------------------------------
   RF Warmup State for the implementation of the WUTA (Warm Up Time Adjustment)
   algorithm.
---------------------------------------------------------------------*/
typedef struct
{
  timetick_type                               tune_done_time_sclks;
    /* Sclk timestamps when the RF tune was completed. */

  timetick_type                               rtc_on_time_sclks;
    /* Sclk timestamp when the RTC interrupt fired. */

  uint32                                      cur_warmup_time_us;
    /* Current determined warmup time based on WUTA. */

  timetick_type                               rf_warmup_start_sclks;
    /* Sclk timestamp when the RF warmup function was called. */

  timetick_type                               slack_start_sclks;
    /* Sclk timestamp when the RF warmup slack started. */

  uint32                                      actual_warmup_time_us;
    /* Actual warmup time from when the RF warmup function was called
       until RF warmup processing was done. */
}
hdrsrchsleep_rf_warmup_type;

/*---------------------------------------------------------------------
   Firmware messages data structure
---------------------------------------------------------------------*/
typedef struct
{
  hdrfw_scc_msg_t                            cc_config_msg;
    /* SCC control message */
}
hdrsrchsleep_fw_msg_struct_type;

/*---------------------------------------------------------------------
   Sleep State data structure
---------------------------------------------------------------------*/

#ifdef FEATURE_HDR_TO_LTE

#define HDRSRCHSLEEP_ORNL_CYCLE_MASK     7
  /* Other RAT Neighbor List Cycle range is 0 to 7 */

/* Look up table containing values for Other RAT Neighbor List cycle,
   as received from AN in the Quick Config Overhead message.  This is
   the cycle which DO needs to wake up to receive the Other RAT Neighbor
   List Overhead message containing LTE neighbors for possible LTE reselection. */
#define HDRSRCHSLEEP_ORNL_CYCLE_TAB_SIZE 8

LOCAL const uint8 hdrsrchsleep_ornl_cycle_tab[ HDRSRCHSLEEP_ORNL_CYCLE_TAB_SIZE ] = 
{ 
    2,      /* entry 0, 2 SCC cycle */
    3,      /* entry 1, 3 SCC cycle */
    6,      /* entry 2, 6 SCC cycle */
   10,      /* entry 3, 10 SCC cycle */
   14,      /* entry 4, 14 SCC cycle */
   24,      /* entry 5, 24 SCC cycle */
   40,      /* entry 6, 40 SCC cycle */
   64,      /* entry 7, 64 SCC cycle */
};

#endif /* FEATURE_HDR_TO_LTE */

/*---------------------------------------------------------------------
   RF Wait State for saving power when HDR cannot get RF resources
   when waking up from sleep.
---------------------------------------------------------------------*/
typedef enum
{
  HDRSRCHSLEEP_RF_NOT_WAITING,
    /* Not waiting on RF */

  HDRSRCHSLEEP_RF_WAITING,
    /* After waking up from sleep, started wait for RF but before RF timeout */

  HDRSRCHSLEEP_RF_TIMEOUT,
    /* Timed out waiting for RF resources but before system lost is complete */

  HDRSRCHSLEEP_RF_GRANTED_BEFORE_TIMEOUT
    /* While waiting for RF after wakeup, RF is granted but before sleep is reprogrammed */
}
hdrsrchsleep_rf_wait_state_type;


typedef struct
{
  uint8                                       until;
    /* When should we wakeup next? */

  qword                                       wake_cycle;
  uint16                                      wake_slot;
    /* Wakeup time, in 5.12s control channel cycles and slot */

  uint32                                      sleep_slots;
    /* Length of sleep, in slots */

  uint16                                      hstr_at_sleep;
    /* HSTR count at sleep */

  boolean                                     intra_scc_start_slot_valid;
    /* Flag used to indicate whether intra_scc_start_slot is valid */

  uint16                                      intra_scc_start_slot;
    /* CC start in slots. This field is valid if intra_scc_start_slot_valid is
       TRUE and is modulo 256.  256 is the number of slots from one Sync
       Capsule to the next Sync Capsule.  */

  uint32                                      conflict_adjust_slots;
    /* Length of cpu conflict adjust, in slots */

  uint8                                       control_channel_cycle;
    /* Hash value "R" for "(C + R) mod Nsleep == 0" */

  boolean                                     sleeping;
    /* Flag to indicate that we are sleeping */

  int8                                        sci;
    /* slot cycle index. Valid values 0 to 3 */

  uint16                                      scc_offset;
    /* SCC offset. Restore it at the wakeup */
  
  uint8                                       scc_rate;
    /* CC rate */

  int8                                        previous_sci;
    /* previous slot cycle index. Valid values 0 to 3 */

  uint8                                       sci_log;
    /*  This field is logged in "sleep_info" log packet
        4: 426ms      sleep
        3: 5.12   sec sleep
        2: 10.24  sec sleep
        1: 20.48  sec sleep
        0: 40.96  sec sleep   */

#ifdef FEATURE_HDR_REVA_L1
  uint8                                      cc_start;
    /* Time in slots that FW expects to find CC/SCC preamble    */

  uint8                                      prev_cc_start;
    /* Previous cc_start used for logging purposes. */
#endif /* FEATURE_HDR_REVA_L1 */

  hdrsrch_wakeup_reason_enum_type            wakeup_reason;
    /* the reason for next HDR wake-up                      */

#ifdef FEATURE_HDR_BCMCS
  uint8                                      bcmcs_page_cycle;
    /* the BCMCS page cycle that AT should wake-up for      */
  
  uint8                                      bcmcs_sleep_cycle;
    /* the BCMCS sleep cycle     */

#endif /* FEATURE_HDR_BCMCS */

  uint16                                     reacq_slots;
    /* the number of slots allocated to Reacquisition timeline */

  uint8                                      neg_sci;
    /* The negotiated SCI value as determined from the protocol
       stack which may differ from "until" which is what the
       protocol actually wants us to do. */

  hdrsrchsleep_rf_warmup_type                rf_warmup;
    /* Tracks information needed for the WUTA algorithm. */

  dword                                      ran_next_sav;
    /* Calculate and save a random number when CCC is set.  This is used
       in hashing the CCC over a larger SCI (until) when neg_sci is smaller */

  boolean                                    qpch_enabled;
    /* Flag to indicate whether QPCH is enabled or not */

#ifdef FEATURE_HDR_TO_LTE
  uint8                                      ornl_cycle;
    /* Other RAT (Radio Access Technologies) Neighbor List message has its own cycle */
#endif /* FEATURE_HDR_TO_LTE */

  uint8                                      new_wakeup_sci;
    /* new wakeup slow for next sleep */

  hdrsrch_sleep_sub_state_enum_type          sleep_sub_state;
   /*  Enum to indicate which sleep state we are in */

  uint8                                      last_normal_sleep_sci;
   /* SCI of last non intra scc sleep */

  boolean                                    cur_warmup_time_is_optimized;
    /* Whether the current sleep timeline is optimized */

  boolean                                    rpm_warmup_time_is_optimized;
    /* Whether to change to optimized sleep timeline */

#ifdef FEATURE_HDR_DIME_MODEM
  slpc_tstmr_type                            slpc_start_t_stmr;
    /* HDR STMR at the point when slpc dump univ_stmr and hdr_stmr */

  uint64                                     sleep_duration_cx8;
    /* sleep duration from slpc_start_t_stmr to online interrupt point */

  slpc_tstmr_type                            online_hdr_t_stmr;
      /* HDR STMR at online interrupt */

  uint64                                     online_univ_stmr;
     /* Univ STMR at online interrupt */

  uint64                                     online_cdma_sys_time;
     /* CDMA system time at online interrupt */

#endif /* FEATURE_HDR_DIME_MODEM */

  hdrsrchsleep_rf_wait_state_type            rf_wait_state;
    /* RF wait state, used for power optimzation when RF resource is not
       available after waking up from sleep. */

#ifdef FEATURE_HDR_BOLT_MODEM
  rex_timer_type                             ols_timer;
  boolean                                    ols_timer_init;
  slpc_event_callback_type                   ols_cb;
    /* OLS callback is removed from bolt. Emulate it internally with timer */
#endif /* FEATURE_HDR_BOLT_MODEM */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  boolean                                   session_is_open;
    /* Whether DO session is opened */

  boolean                                   ovhd_is_updated;
    /* Whether OVHD is updated */

  uint8                                    ovhd_denied_cnt;
    /* Number of times TRM chain was denied for OVHD reason */ 

  hdrsrchrf_reason_enum_t                   trm_reason;
    /* Why the resource is needed (used for priority decisions) */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

  uint64                                   wakeup_id;
     /* Unique Identifier to indicate the wakeup for which reservation is done */

}
hdrsrchsleep_struct_type;


LOCAL hdrsrchsleep_struct_type    hdrsrchsleep;

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
#define HDRSRCHSLEEP_NUM_SCIS 13
 /* Total number of allowed SCIs */

/* Static table of actual sleep values */
static const uint16 hdrsrchsleep_sci_ms[HDRSRCHSLEEP_NUM_SCIS] =
{
  6,     /* SCI 0 */
  13,    /* SCI 1 */
  26,    /* SCI 2 */
  53,    /* SCI 3 */
  106,   /* SCI 4 */
  213,   /* SCI 5 */
  426,   /* SCI 6 */
  853,   /* SCI 7 */
  2560,  /* SCI 8 */
  5120,  /* SCI 9 */
  10240, /* SCI 10 */
  20480, /* SCI 11 */
  40960  /* SCI 12 */
};
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT*/

/*==========================================================================

                     HDR SEARCH SLEEP FUNCTION PROTOTYPES

==========================================================================*/


extern uint64 timetick_sclk_to_prec_us
(
  uint32      sclks
    /* Duration in sclks to be converted into microseconds */
);

#ifdef FEATURE_HDR_DIME_MODEM
void hdrsrchsleep_dump_time( void );

void hdrsrchsleep_wakeup_notify
( 
  slpc_id_type                       slpc_id, 
    /* slpc id -- 1x, HDR, etc */             

  uint64                             wakeup_tick, 
    /* absolute wakeup point in XO units */

  boolean                            update, 
    /* whether this is an update to a previously set wakeup point */

  boolean extension
    /* whether new wakeup is later than old */
);
#else /* FEATURE_HDR_DIME_MODEM */
uint32 hdrsrchsleep_adjust_wakeup_for_cpu_conflict
(
  const sleepctl_wakeup_info_type *hdr_wi,
    /* Current wakeup information for the client doing the adjusting */

  const sleepctl_wakeup_info_type *conflict_wi,
    /* Current wakeup information for the conflicting client */

  boolean                         save_adjust_val
    /* Return adjustment value only.  TRUE means save adjust value in
       hdrsrchsleep. */
);
#endif /* FEATURE_HDR_DIME_MODEM */

LOCAL boolean hdrsrchsleep_schedule_fast_wakeup
(
  boolean need_rf
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  ,hdrsrchrf_reason_enum_t trm_reason
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
);

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
LOCAL hdrsrchrf_reason_enum_t hdrsrchsleep_get_trm_reason
(
  hdrsrch_cmd_name_enum_type cmd
);
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

LOCAL void hdrsrchsleep_compute_next_wakeup( void );

LOCAL void hdrsrchsleep_init( hdrsrch_state_enum_type last_state );

LOCAL int32 hdrsrchsleep_get_time_to_next_wakeup( void );

LOCAL uint32 hdrsrchsleep_get_sleep_duration( void );

#ifdef FEATURE_HDR_PBR_RANDOMIZATION
void hdrsrchsleep_pbr_trm_update
(
  boolean                               wakeup_failed
    /* Update wakeup status to TRM */
);
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

/* EJECT */
/*=============================================================================

FUNCTION HDRSRCHSLEEP_SLEEP_NOW

DESCRIPTION
  Check if it is necessary to sleep now.

DEPENDENCIES
  NONE

RETURN VALUE
  TRUE if sleep SCI is incremented.

SIDE EFFECTS
  None

=============================================================================*/

boolean hdrsrchsleep_sleep_now( void )
{
  uint8                           saved_until;
    /* Temp holder of SCI when CP wants to use */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_GRANTED_BEFORE_TIMEOUT )
  {
    /* Since current wakeup is right after RF got granted before timeout,
       go back to sleep to wakeup on SCI boundary. */

    saved_until = hdrsrchsleep.until;
    hdrsrchsleep.until = hdrsrchsleep.neg_sci;
      /* Use only negiated wakeup, for example in 40s sleep, use neg sci. */
    hdrsrchsleep_compute_next_wakeup();
      /* Wakeup slot is calculated */
    hdrsrchsleep.until = saved_until;

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH,
                    "HDR SLEEP POWER OPT (state %d): After computing next wakeup, until=%d neg_sci=%d wake_slots=%d sleep_slots=%d",
                    hdrsrchsleep.rf_wait_state,
                    hdrsrchsleep.until,
                    hdrsrchsleep.neg_sci,
                    hdrsrchsleep.wake_slot,
                    hdrsrchsleep.sleep_slots );

    hdrsrchsleep_init_sleep_state();
      /* By the time rf warmup is done, HDR is in reacq state.  Initiate sleep
         to wakeup on proper SCI boundary. */

    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_NOT_WAITING;

    return TRUE;
  }

  return FALSE;

} /* hdrsrchsleep_sleep_now */

/* EJECT */
/*=============================================================================

FUNCTION HDRSRCHSLEEP_INCREMENT_WAKEUP_SCI

DESCRIPTION
  Increment the SCI for sleep slot.

DEPENDENCIES
  NONE

RETURN VALUE
  TRUE if sleep SCI is incremented.

SIDE EFFECTS
  None

=============================================================================*/

boolean hdrsrchsleep_increment_wakeup_sci( void )
{
  if( hdrsrchsleep_get_intra_scc_valid ())
  {
    /* For intra scc sleep, we don't want to reduce search window size for next 
       reacq. Reducing search window size for intra scc sleep may cause reacq
       failure at the reacq after the sleep after intra scc sleep. The normal
       sleep after intra scc sleep is usually much longer.
       Returning TRUE won't trigger reducing search window size.
    */
    HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "DEBUG intra_scc_valid is true last_sci= %d util=%d", 
                   hdrsrchsleep.last_normal_sleep_sci,hdrsrchsleep.until );

    return TRUE;
  }
  else if ( hdrsrchsleep.last_normal_sleep_sci < hdrsrchsleep.until )
  {
    /* hdrsrchsleep.last_sleep_sci is the last non instra scc sleep SCI.
       If it is smaller than the current SCI asked by CP, let's increment
       sleep time slowly to reduce the possibility of reacq failure.
    */
    HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "DEBUG intra_scc_valid is false last_sci= %d util=%d", 
                   hdrsrchsleep.last_normal_sleep_sci,hdrsrchsleep.until );
    hdrsrchsleep.until = hdrsrchsleep.last_normal_sleep_sci +1;

    HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "next wake up slot SCI incremented to %d", 
                   hdrsrchsleep.until );

    return TRUE;
  }
  else
  {
    /* No space to increment sleep time, returning false will trigger reducing
       seach window size
    */
    HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "DEBUG cannot increase SCI last_sci= %d util=%d", 
                   hdrsrchsleep.last_normal_sleep_sci,hdrsrchsleep.until );
  return FALSE;
  }
}/* hdrsrchsleep_increment_wakeup_sci */

/* EJECT */
/*=============================================================================

FUNCTION HDRSRCHSLEEP_DECREMENT_WAKEUP_SCI

DESCRIPTION
  Decrement the SCI for sleep slot.

DEPENDENCIES
  NONE

RETURN VALUE
  TRUE if sleep SCI is decremented.

SIDE EFFECTS
  None

=============================================================================*/
boolean hdrsrchsleep_decrement_wakeup_sci( void )
{
  if( hdrsrchsleep.until > 6 )
    /* SCI of 6 is the minimum sleep duration */
  {
    hdrsrchsleep.until -= 1;
      /* assign a smaller SCI for next sleep cycle */

    HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "next wake up slot SCI decremented to %d",
                   hdrsrchsleep.until );
    return TRUE;
  }

  HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "DEBUG: Cannot reduce next wake up SCI last_sci=%d until=%d",
                 hdrsrchsleep.last_normal_sleep_sci,hdrsrchsleep.until );
  return FALSE;
}/* hdrsrchsleep_decrement_wakeup_slot */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_RESET_LAST_NORMAL_SLEEP_SCI

DESCRIPTION
  This function sets the LAST_NORMAL_SLEEP_SCI to the dafult 9

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
  
==========================================================================*/
void hdrsrchsleep_reset_last_normal_sleep_sci( void )
{
  hdrsrchsleep.last_normal_sleep_sci = 9;
}

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_SET_TIME_VALID

DESCRIPTION
  This callback function is called when time is valid.

DEPENDENCIES
  Must only be passed as a callback function to hdrts_wake( ).

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Signal set here will be processed in the hdrsrchreacq_on_time_valid( ),
  once a pilot has been reacquired.

===========================================================================*/

LOCAL void hdrsrchsleep_set_time_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_TIME_VALID );
    /* output powr profile strobe if feature is enabled */

  HDRSRCH_PROFILE( TIME_VALID );
    /* sleep timeline profiling output if enabled */

  HDRSRCH_SIG_SET( HDRSRCH_TIME_VALID_SIG );
    /* Indicate to the task that HDR System Time is now valid */

} /* hdrsrchsleep_set_time_valid( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_COMPUTE_RF_RESERVE_TIME

DESCRIPTION
  This function calculates the approximate wakeup time from sleep
  in order to reserve the RF.

DEPENDENCIES
  None

PARAMETERS
  sleep_slots - Length of sleep, in slots to sleep from now.

RETURN VALUE
  Length of sleep time in sclks.

SIDE EFFECTS
  None
 

===========================================================================*/

LOCAL timetick_type hdrsrchsleep_compute_rf_reserve_time
(
  uint32                          sleep_slots
    /* Length of sleep, in slots */
)
{

  timetick_type                   current_time;
    /* Slow clock counter value for current time */

  timetick_type                   wakeup_time;
    /* Slow clock counter value for wakeup int time */

  uint32                          wakeup_sclk;
    /* Duration from sleep start to wakeup event in sleep-clocks */

  uint32                          ending_sclk;
    /* Duration from sleep start to sleep end event in sleep-clocks */

  uint16                          warmup_sclk;
    /* Duration of RF warmup event in sleep-clocks */

#ifdef FEATURE_HDR_DIME_MODEM
  uint64                          ending_cx8;
    /* How long to sleep_ending in cx8 */
#else
  uint16                          ending_fine;
    /* Sleep ending event duration in residual sys_clk's (eg, chipx8's) */
#endif /* FEATURE_HDR_DIME_MODEM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  current_time = timetick_get();
    /* Get the current timetick */
#ifdef FEATURE_HDR_DIME_MODEM
  ending_cx8 = sleep_slots << SYSCLK_PER_SLOT_SHIFT ;
  ending_sclk = ( ( ending_cx8 >> 3) *625/768 ) / 1000;
    /* Convert cx8 to micro second. 1 cdma chip = 1000/1228.8 us = 625/768 us
       Then convert it to ms
    */
  ending_sclk = timetick_cvt_to_sclk( ending_sclk, T_MSEC);
#else
  /* Convert sys_clk's to sleep clocks & sys_clk residual. */
  ending_sclk = sclk_from_sysclks( SCLK_HDR,
                                   ( sleep_slots << SYSCLK_PER_SLOT_SHIFT ),
                                   &ending_fine );
#endif /* FEATURE_HDR_DIME_MODEM */
  warmup_sclk = timetick_cvt_to_sclk( 
                        hdrsrchsleep.rf_warmup.cur_warmup_time_us, T_USEC);
    /* Get RF warmup RF warmup time in sleep clocks */

  wakeup_sclk = ending_sclk - warmup_sclk; 
    /* Compute duration from sleep start to wakeup event in sleep-clocks */

  wakeup_time = current_time + wakeup_sclk;
    /* Compute the timestamp when the wakeup int is expected to occur */

  return (wakeup_time);

} /* hdrsrchsleep_compute_rf_reserve_time( ) */

#ifdef FEATURE_HDR_DIME_MODEM

/*===========================================================================

FUNCTION HDRSRCHSLEEP_CALCULATE_ONLINE_TIME

DESCRIPTION
  This function is to calculate HDR stmr (in cx8) value at online interrupt and
  sleep duration (in cx8) from slpc_start_t_stmr.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  wakeup stmr

SIDE EFFECTS
  none

===========================================================================*/

LOCAL void hdrsrchsleep_calculate_online_time ( void )
{

  uint32                          offset_from_slot_boundary;
    /* offset from slot boundary, in cx8 */

  qword                           online_sys_time;
    /* cdma system time at online interrupt */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  offset_from_slot_boundary = 
        ( ( hdrsrchsleep.slpc_start_t_stmr.time + HDR_PN_CIRC_WRAPUP_X8 - hdrsrchdrv_get_mstr_x8() )  
        & HDR_PN_CIRC_MASK_X8 ) & 0x3FFF;

    /* offset from the slot boundary at slpc_start_t_stmr point */
  hdrsrchsleep.sleep_duration_cx8 = (uint64) (( hdrsrchsleep.sleep_slots + 1 ) 
                              << CHIPS_PER_SLOT_SHIFT ) << 3;
    /* Now we get sleep duration from current slot boundary to online slot boundary.
       It is in cx8.
       hdrsrchsleep.sleep_slots is the number of slot from the next slot boundary after slpc_start()
       to the slot boundary of online interrupt.
       This is guranteed when calculating sleep duration before calling this function.
       It is consitent with old targets. That's why we need to add 1 here.
    */
  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                 "mstr_x8=0x%x,offset_from_slot_boundary=0x%x ",
                  hdrsrchdrv_get_mstr_x8(),
                  offset_from_slot_boundary);

  hdrsrchsleep.sleep_duration_cx8 -= (uint64) offset_from_slot_boundary ;
    /* Now we get sleep duration from slpc_start_t_stmr point to online slot boundary */

  hdrsrchsleep.online_hdr_t_stmr.time = 
    (uint32) ( hdrsrchsleep.slpc_start_t_stmr.time + hdrsrchsleep.sleep_duration_cx8 );
    /* HDR RTC value at online interrupt */
  hdrsrchsleep.online_hdr_t_stmr.phase = 0;
    /* phase is not used so far. */

  HDR_MSG_SRCH_9( MSG_LEGACY_MED, 
                  "slpc_start prev slot cx8:0x%x"
                  " wakeup cycle/slot/cx8:0x%x%08x/%d/0x%x%08x"
                  " duration slot/cx8:%d/0x%x%08x",
                  I64_LO(hdrsrchsleep.slpc_start_t_stmr.time - offset_from_slot_boundary),
                  qw_hi(hdrsrchsleep.wake_cycle),
                  qw_lo(hdrsrchsleep.wake_cycle),
                  hdrsrchsleep.wake_slot,
                  I64_HI(hdrsrchsleep.online_hdr_t_stmr.time),
                  I64_LO(hdrsrchsleep.online_hdr_t_stmr.time),
                  hdrsrchsleep.sleep_slots + 1,
                  I64_HI(hdrsrchsleep.sleep_duration_cx8),
                  I64_LO(hdrsrchsleep.sleep_duration_cx8) );

  /* For Debug only. We calculate online_univ_stmr here and we can compare it with the return value from
     slpc.
  */
  
  qw_mul( online_sys_time, hdrsrchsleep.wake_cycle, HDRTS_SLOTS_PER_CCC );
  qw_inc( online_sys_time, hdrsrchsleep.wake_slot );
  qw_shift( online_sys_time, CHIPS_PER_SLOT_SHIFT );

  hdrsrchsleep.online_cdma_sys_time = ( ((uint64)(qw_hi(online_sys_time)) << 32) | 
                                        qw_lo(online_sys_time) );
    /* Get chip number at wakeup. This is CDMA system time */

  //wakeup_stmr = (uint64)( wakeup_stmr + delta_slots);
    /* adjust slots */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
          "online_sys_time=0x%x%08x",
          I64_HI(hdrsrchsleep.online_cdma_sys_time),
          I64_LO(hdrsrchsleep.online_cdma_sys_time) );
} /* hdrsrchsleep_calculate_online_time */

/*===========================================================================

FUNCTION HDRSRCHSLEEP_SET_WARMUP_AND_SLEEP_DURATION

DESCRIPTION
  This function is to set warmup time and sleep duration to SLPC

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  none

===========================================================================*/

LOCAL void hdrsrchsleep_set_warmup_and_sleep_duration ( void )
{
  slpc_err_type                   slpc_status;
    /* error code returned by slpc APIs */

#ifdef FEATURE_HDR_BOLT_MODEM
  uint32                          duration_us;
#endif /* FEATURE_HDR_BOLT_MODEM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep_calculate_online_time( );
    /* Calculate hdr stmr at online interrupt point and sleep duration. The related fields in
       hdrsrchsleep will be updated.
    */
  slpc_status = slpc_set_duration_and_warmup( SLPC_HDR,
                   hdrsrchsleep.sleep_duration_cx8,
                   hdrsrchsleep.rf_warmup.cur_warmup_time_us );

#ifdef FEATURE_HDR_BOLT_MODEM
  duration_us = HDRSRCHSLEEP_CX8_TO_US(hdrsrchsleep.sleep_duration_cx8);
    /* convert to uS */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "set warmup@%d warmup_done@%d warmup_duration:%d" ,                  
                  duration_us - hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                  duration_us, 
                  hdrsrchsleep.rf_warmup.cur_warmup_time_us );
    /* for bolt we install a timer to emulate ols cb, print the timeline */
#endif /* FEATURE_HDR_BOLT_MODEM */

  if ( slpc_status != SLPC_ERR_NO_ERR )
  {
     ERR_FATAL( "slpc_set_duration_and_warmup failed, err=%d", slpc_status, 0, 0 );
  }

  hdrsrchsleep.online_univ_stmr = slpc_get_ols_tick(SLPC_HDR);

#ifdef FEATURE_HDR_BOLT_MODEM
  hdrsrchsleep.online_univ_stmr += 
    HDRSRCHSLEEP_MS_TO_XO_TICK(hdrsrchsleep.rf_warmup.cur_warmup_time_us / 1000);
    /* slpc_get_ols_tick() returns the virtual slam point. For bolt, this is
       slightly after wakeup cb, instead of ols cb. We make an approximate
       correction to keep legacy calcuation. */
#endif /* FEATURE_HDR_BOLT_MODEM */

} /* hdrsrchsleep_set_warmup_and_sleep_duration */


#endif /* FEATURE_HDR_DIME_MODEM */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_EXTEND_SLEEP

DESCRIPTION
  This function is called if it is not possible to complete the wakeup.

DEPENDENCIES
  Must only be called from hdrsrchsleep_rf_warmup()

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchsleep_extend_sleep
(
  uint32                          extension_slots
)
{
  uint32                          old_slots;
    /* Old sleep duration, in slots */

  uint32                          slots;
    /* New sleep duration, in slots */

  uint32                          slept;
    /* How long we've slept so far, in slots */

  uint32                          warmup_slots;
    /* How long the rf warmup time is in slots. */

#ifdef FEATURE_HDR_REVA_L1
  uint32                          delta_slots;
    /* New delta in slots between the new and old durations. */

  uint32                          num_slots_in_curr_sci;
    /* The period in slots of the current SCI          */
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.sleeping = TRUE;
    /* We are extending sleep */

  hdrsrchsleep.intra_scc_start_slot_valid = FALSE;
    /* If we were trying to perform an intra-SCC wakeup we no longer will be,
       we are missing the continuation packet anyways so we are reverting
       to the normal reacq algorithm. */

  REX_DISABLE_PREMPTION();

  old_slots = hdrsrchsleep.sleep_slots;
    /* Capture old sleep duration */

  slots = old_slots + extension_slots;
    /* Extend the sleep duration the given amount */

  if( hdrsrchsleep.conflict_adjust_slots > 0 )
  {
    slots += hdrsrchsleep.conflict_adjust_slots; 
      /* Put the slot dedution back because hdr may not have confliction
         with 1x in the next wake cycle */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Added hdrsrchsleep.conflict_adjust_slots(%d) to extension_slots(%d)",
                    hdrsrchsleep.conflict_adjust_slots, extension_slots );
            
    hdrsrchsleep.conflict_adjust_slots = 0; 
      /* Reset the confict adjust slots */
  }

#ifdef FEATURE_HDR_DIME_MODEM
  slept = (uint32) ( slpc_get_sysclk_count( SLPC_HDR ) >> SYSCLK_PER_SLOT_SHIFT );
    /* get the time we have slept for, in unit of cx8. We covert it to slots */

#else /* FEATURE_HDR_DIME_MODEM */
  slept = sleepctl_get_sys_clk_cnt( SLEEPCTL_HDR ) >> SYSCLK_PER_SLOT_SHIFT;
    /* Get number of slots we've slept for */
#endif /* FEATURE_HDR_DIME_MODEM */

  warmup_slots = 
     MS_TO_SLOTS( ( hdrsrchsleep.rf_warmup.cur_warmup_time_us + 999 ) / 1000 );
    /* Calculate a conservative estimate of the warmup time in slots. */

  /* Repeatedly add extension to sleep duration until it is at least the 
     RF warmup time longer than we've already slept.  */
  while ( slots  <=  slept + warmup_slots )
  {
    slots += extension_slots;
  }

  hdrsrchsleep.sleep_slots = slots;
   /* Save the new sleep duration */

#ifdef FEATURE_HDR_REVA_L1
  num_slots_in_curr_sci = ( hdrsrchsleep.until >= 6 ) ? HDRTS_SLOTS_PER_SCC :
                          HDRTS_SLOTS_PER_SCC >> ( 0x6 - hdrsrchsleep.until );
    /* How many slots between meaningful packets to this AT in idle operation
       based on the SCI? */

  delta_slots = hdrsrchsleep.sleep_slots - old_slots;
    /* Calculate how long the extension is. */

  /* If the extension doesn't align with the next meaningful packet push it 
     forward. */
  if ( ( delta_slots % num_slots_in_curr_sci ) != 0 )
  {
    delta_slots = delta_slots - ( delta_slots % num_slots_in_curr_sci );
      /* Round down based on the current SCI */

    delta_slots += num_slots_in_curr_sci;
      /* And compute the next SCI slot. We will use this in our determination
         of the cc_start. */
  }

  hdrsrchsleep.cc_start = ( hdrsrchsleep.cc_start - 
      ( hdrsrchsleep.intra_scc_start_slot % num_slots_in_curr_sci ) +
      delta_slots ) % HDRTS_SLOTS_PER_SCC;

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "CC_Start %d intra_scc_start_slot %d num_slots_in_curr_sci %d",
                    hdrsrchsleep.cc_start,
                    hdrsrchsleep.intra_scc_start_slot,
                    num_slots_in_curr_sci );

    /* Redetermine the CC start based on the extension so that the MDSP will
       know whether to look for a subsync packet. */
#endif /* FEATURE_HDR_REVA_L1  */

#ifdef FEATURE_HDR_DIME_MODEM

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,"extend sleep old sleep_slots %d new sleep slots %d",
                  old_slots, hdrsrchsleep.sleep_slots);
  delta_slots = hdrsrchsleep.sleep_slots - old_slots;
    /* number of slots of extended sleep time */
  hdrsrchsleep.wake_slot += delta_slots;
    /* update wakeup slot */
  hdrsrchsleep_set_warmup_and_sleep_duration();

  slept = (uint32) ( slpc_get_sysclk_count( SLPC_HDR ) >> SYSCLK_PER_SLOT_SHIFT );
    /* get the time we have slept for, in unit of cx8. We convert it to slots */
  if ( ( hdrsrchsleep.sleep_slots - slept ) > 2 )
  {
    /* Failed to wakeup at the required time and hence use different WakeupID */
    if( !hdrsrchsleep_get_intra_scc_valid() )
    {
#ifdef FEATURE_HDR_PBR_RANDOMIZATION
      hdrsrchsleep_pbr_trm_update( TRUE );
        /* Update Page demod failure to TRM */
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

      hdrsrchsleep.wakeup_id++;
      /* Increment wakeup id for next wakeup irrespective of RF reason */
    }
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  hdrsrchsleep.ovhd_is_updated = FALSE;
  hdrsrchsleep_reserve_trm_at( HDRSRCHRF_PRI_CHAIN,
                               HDRSRCHRF_RX_ANY,
                               hdrsrchsleep_compute_rf_reserve_time( ( hdrsrchsleep.sleep_slots - slept ) - 2 ),
                               HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK);
#else
  hdrsrchrf_reserve_at( HDRSRCHRF_PRI_CHAIN,
                        HDRSRCHRF_RX_ANY,
                        hdrsrchsleep_compute_rf_reserve_time( ( hdrsrchsleep.sleep_slots - slept ) - 2 ),
                        HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK,
                        HDRSRCHSLEEP_RF_REASON );
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
               "extend sleep: not enough time to do rf reserve ");
  }


#else /* FEATURE_HDR_DIME_MODEM */
  sleepctl_set_duration( SLEEPCTL_HDR, slots << CHIPS_PER_SLOT_SHIFT );
    /* And program this as the new sleep duration */

  /* Failed to wakeup at the required time and hence use different WakeupID */
  if( !hdrsrchsleep_get_intra_scc_valid() )
  {
#ifdef FEATURE_HDR_PBR_RANDOMIZATION
    hdrsrchsleep_pbr_trm_update( TRUE );
      /* Update Page demod failure to TRM */
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

    hdrsrchsleep.wakeup_id++;
    /* Increment wakeup id for next wakeup */
  }
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  hdrsrchsleep.ovhd_is_updated = FALSE;
  hdrsrchsleep_reserve_trm_at( HDRSRCHRF_PRI_CHAIN,
                               HDRSRCHRF_RX_ANY,
                               sleepctl_get_wakeup_time( SLEEPCTL_HDR ),
                               HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK );
  /* Make RF chain reservation for when we wake up. */
#else
  hdrsrchrf_reserve_at( HDRSRCHRF_PRI_CHAIN,
                        HDRSRCHRF_RX_ANY,
                        sleepctl_get_wakeup_time( SLEEPCTL_HDR ),
                        HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK,
                        HDRSRCHSLEEP_RF_REASON );
    /* Make RF chain reservation for when we wake up. */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
#endif /* FEATURE_HDR_DIME_MODEM */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Extend sleep and notify MCPM" );

  hdrsrchmcpm_extend_sleep();
    /* Notify MCPM sleep time changed */

  REX_ENABLE_PREMPTION();

  if ( slots != old_slots + extension_slots )
  {
    ERR("Extension too short: old=%d,ext=%d,slept=%d",
        old_slots, extension_slots, slept);
  }


  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Extend HDR Sleep:Old=%d,new=%d,Slept=%dms",
                  old_slots, slots, SLOTS_TO_MS(slept) );

  /* Safeguard check to make sure time in is not too long. */
  if ( slots > ( HDRSRCHSLEEP_EXTEND_SLEEP_SAFEGUARD_SLOTS ) )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
      /* Asleep for too long - give up on HDR. */
  }

} /* hdrsrchsleep_extend_sleep( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_RF_WARMUP

DESCRIPTION
  This function is called at the end of sleep, when the RF must be turned
  back on.

DEPENDENCIES
  Must only be used as callback to sleepctl_set_warmup( )

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchsleep_rf_warmup( void )
{
  boolean                         chain_granted;
    /* Which chain was granted after the request for RF resources. */

  int                             sleep_dur_ms;
    /* get sleep duration in msec */

  int32                           num_extension_slots = 0;
    /* The number of extension slots to be used if we can't wakeup. */

#ifdef FEATURE_NPA
  errno_enum_type                 status;
    /* Command completion status */
#endif

  uint32                          time_to_rtc_on_us = 0;
    /* Time in sclk until to rtc on */

  uint64                          time_slept_cx8;
    /* time already slept */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RFWU_ISR ); 
   /* output the power profiling strobe at the 1st slp controller ISR */ 

  HDRSRCH_PROFILE( WAKEUP );
    /* sleep timeline profiling output if enabled */

#ifdef FEATURE_HDR_RUMI  
  HDR_MSG_SRCH( MSG_LEGACY_MED, "rf warmup called");
#endif /* FEATURE_HDR_RUMI */

  slpc_deregister_notify_callback( hdrsrchsleep_wakeup_notify );
    /* Wakeup notify callback is deregistered while waking up as there is no further need to adjust wakeup time. */
    
  if ( hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_WAITING )
  {
    /* Timed out after waiting for RF resources because previously set an
       RF timer via sleepctl when no RF resources when waking up.  So if
       waking up again and still waiting for RF, it must be due to timeout. */

    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_TIMEOUT;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "HDR SLEEP POWER OPT (state %d): 60s RF Resource timeout",
                    hdrsrchsleep.rf_wait_state );

    hdrsrchsleep_extend_sleep( HDRTS_SLOTS_PER_SCC );
      /* Extend sleep to the by 426ms so that system lost below can be processed
         properly including deactivate cmd in sleep state. */

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
      /* Eventually deactivate cmd will be sent while in sleep state.  A fast
         wakeup is required to process this which is why the sleep must be
         extended to the future.*/

    return;
  }
  else if ( hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_GRANTED_BEFORE_TIMEOUT )
  {

    /* RF was granted to HDR while waiting for RF but before timeout. */

    HDR_MSG_SRCH_5( MSG_LEGACY_MED,
                    "HDR SLEEP POWER OPT (state %d): Wakeup in rf_warmup, RF granted before timeout until=%d neg_sci=%d wake_slots=%d sleep_slots=%d slept=%d",
                    hdrsrchsleep.rf_wait_state,
                    hdrsrchsleep.until,
                    hdrsrchsleep.neg_sci,
                    hdrsrchsleep.wake_slot,
                    hdrsrchsleep.sleep_slots );

    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_NOT_WAITING;
      /* Just wake up normally for fast wakeup and re-initilize rf wait state. */
  }
    /* Determine how many slots sleep should be extended if we can't wakeup. */
  if ( hdrsrchsleep.until >= 6 )
  {
    /* Not 213 ms sleep cycle */
    num_extension_slots = HDRTS_SLOTS_PER_SCC;

    if ( hdrsrchsleep.intra_scc_start_slot_valid == TRUE )
    {
      /* Since num_extension_slots is added to the original sleep controller
         sleep time relative to the 1st CC packet, when extending sleep for
         intra-scc, we need to take into account the sleep controller is armed
         with the nth CC packet.  intra_scc_start_slot is within the 256 slot
         for each 426ms CC period.  */
      num_extension_slots -= hdrsrchsleep.intra_scc_start_slot;
    }
  }
  else
  {
    /* Must be 213 ms sleep cycle */
    num_extension_slots = HDRTS_SLOTS_PER_SCC >> ( 0x6 - hdrsrchsleep.until );

    if ( hdrsrchsleep.intra_scc_start_slot_valid == TRUE )
    {
      /* intra_scc_start_slot is within the 256 slot but for 213 ms.  Handle
         the case where multi CC processing has gone beyond 213 ms boundary */

      num_extension_slots -= hdrsrchsleep.intra_scc_start_slot;
      if ( num_extension_slots <= 0 )
      {
        num_extension_slots += ( HDRTS_SLOTS_PER_SCC >> ( 0x6 - hdrsrchsleep.until ) );
          /* Due to multi-cc wakeup beyond 213ms, we cannot extend negative time.
             Add to next 213ms CC wakeup. */
      }
    }
  }

  if( hdrsrchsleep.sleep_sub_state != HDRSRCH_NOT_IN_SLEEP_PREPROCESSING )
  {
  
    /* Still doing some sleep processing skip wakeup */
    hdrsrchsleep_extend_sleep( num_extension_slots );

    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                  "**** WAKEUP ISR FIRED while still in SLEEP STATE, %d "
                  "extending sleep, not waking up ****", hdrsrchsleep.sleep_sub_state);
    return;
  }

  hdrsrchsleep.rf_warmup.rf_warmup_start_sclks = timetick_get_safe();
    /* Save the time RF warmup starts. */

#ifdef FEATURE_NPA
  status = hdrsrchutil_npa_issue_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Request CPU for HDR wakeup. */

  /* Check for NPA overlap, this is the only place NPA call overlap
     can take place as this function is called from ISR context */
  if( status == E_FAILURE )
  {
      /*overlap in npa_issue_cpu_request  - skip wakeup */
    hdrsrchsleep_extend_sleep( num_extension_slots );

    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "NPA_ISSUE_CPU_REQUEST returned error, extending sleep,"
                  "not waking up");

    return;  
  }
#endif /* FEATURE_NPA */

#ifdef FEATURE_HDR_DIME_MODEM
  if ( !slpc_sufficient_warmup_time( SLPC_HDR,
                                    HDRSRCHSLEEP_MIN_RF_WARMUP_MS * 1000 ) )  
#else
  if ( !sleepctl_sufficient_warmup_time( SLEEPCTL_HDR,
                                        HDRSRCHSLEEP_MIN_RF_WARMUP_SCLK ) )
#endif /* FEATURE_HDR_DIME_MODEM */
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "Not sufficient time to warmup, extend sleep");

    /* Start of RF warmup was delayed too much - skip wakeup */
    hdrsrchsleep_extend_sleep( num_extension_slots );

#ifdef FEATURE_NPA
   ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup. */
#endif /* FEATURE_NPA */

    return;
  }

  if ( hdrsrchrf_has_lock(HDRSRCHRF_PRI_CHAIN) )
  {
    chain_granted = TRUE;
  }
  else
  {
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    chain_granted = hdrsrchrf_request( HDRSRCHRF_PRI_CHAIN,
                                       HDRSRCHRF_RX_ANY,
                                       ( HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK - 
                                        ( TIMETICK_SCLK_FROM_MS( SLOTS_TO_MS(2) ))),
                                       hdrsrchsleep.trm_reason );
    /* Reduce 2 slots from trm duration because we keep 2 slot buffer between
       TRM reservation and HDR wakeup */
#else
    chain_granted = hdrsrchrf_request( HDRSRCHRF_PRI_CHAIN,
                                       HDRSRCHRF_RX_ANY,
                                       ( HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK - 
                                        ( TIMETICK_SCLK_FROM_MS( SLOTS_TO_MS(2) ))),
                                       HDRSRCHSLEEP_RF_REASON );
    /* Reduce 2 slots from trm duration because we keep 2 slot buffer between
       TRM reservation and HDR wakeup */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
  }
    /* Request lock from for the best transciever since HDR wants to wakeup
       and program RF. */

  if ( chain_granted )
  {
    /* RF Warmup */
    hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
      /* Tell MCPM to turn on clocks grouped under wake-up */

#ifdef FEATURE_HDR_DIME_MODEM
    if ( ! slpc_complete_wakeup( SLPC_HDR, HDRSRCHSLEEP_MIN_RF_WARMUP_MS * 1000, 
                                 &hdrsrchsleep.online_hdr_t_stmr ) )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR,"complete_wakeup() fails, extend sleep" );

      hdrsrchmcpm_set_state( HDRSRCHMCPM_GO_TO_SLEEP_REQ );

      hdrsrchsleep_extend_sleep( num_extension_slots );
        /* skip wakeup */
      return;
    }
#else
    sleepctl_complete_wakeup( SLEEPCTL_HDR );
      /* Commit to this wakeup */
#endif /* FEATURE_HDR_DIME_MODEM */

   /* Calculate online time for rf_warmup_end processing */
    time_slept_cx8 = slpc_get_sysclk_count( SLPC_HDR );
      /* time that we have already slept, in cx8 */

    if (hdrsrchsleep.sleep_duration_cx8 > time_slept_cx8 )
    {
      time_to_rtc_on_us = hdrsrchsleep.sleep_duration_cx8 - time_slept_cx8;
        /* Get time to online interrupt. Now it is in cx8 */

      time_to_rtc_on_us = (time_to_rtc_on_us >> 3)*625/768;
        /* Convert it to micro second. 1 cdma chip = 1000/1228.8 us = 625/768 us */
    }
    else
    {
      time_to_rtc_on_us = 1000;
        /* Fire soon, seems we've already missed the boat. */

      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "RF warmup started after online time!");
    }

    rex_set_timer( &hdrsrchsleep.ols_timer, 
                   (time_to_rtc_on_us + 999 ) / 1000 );

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "set ols cb after %d + 999 / 1000 ms (exp %d)",
                   time_to_rtc_on_us, hdrsrchsleep.rf_warmup.cur_warmup_time_us );    


    sleep_dur_ms = hdrsrchutil_read_stop_watch();
      /* Read the duration for which the phone slept in HDR mode. Note that the
         time stamps may not be accurate at this time however some error is not
         a problem since the F3 message is meant for a very approximate
         estimate of sleep duration */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR RF Warmup start. Slept=%dms",
                    sleep_dur_ms );

    hdrsrchsleep.sleeping = FALSE;
      /* We are waking up */

    hdrsrchsleep.rf_warmup.rtc_on_time_sclks = 0;
    hdrsrchsleep.rf_warmup.tune_done_time_sclks = 0;
      /* Make sure to reset WUTA variables every wakeup. */

    hdrsrchlog.reacq_info.sleep_duration_ms  = sleep_dur_ms;
      /* Record the sleep duration                                    */

    hdrsrchutil_start_timer( &hdrsrchlog.awake_timer );
      /* awake_timer needs to be started now, because we just woke up */



    hdrsrchreacq_init_reacq_state();
      /* And switch to the Reacquisition state. */

#ifndef FEATURE_HDR_RUMI
    HDR_MSG_SRCH( MSG_LEGACY_MED, "init reacq");
#endif /* FEATURE_HDR_RUMI */
  }
  else
  {
    /* Could not get RF resources - skip wakeup
       Simultaneously start an up to 60s sleep and invoked a fast wakeup,.i.e.,
       TRM request and notify.  This will save power since MCPM will
       not need to grant HDR resources for legacy 426ms wakeups to check for RF
       chain availability. */

    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_WAITING;
      /* Initialize RF wait state to waiting */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_PAGE_MISS);
#endif
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                    "HDR SLEEP POWER OPT (state %d): Start 60s timer (already slept %d slots, timeout in %d slots and TRM R&N",
                    hdrsrchsleep.rf_wait_state,
                    hdrsrchsleep.sleep_slots,
                    HDRSRCHSLEEP_MAXIMUM_SLOTS - hdrsrchsleep.sleep_slots );
            
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    if ( hdrsrchsleep.session_is_open == FALSE || hdrsrchsleep.ovhd_is_updated == FALSE || hdrsrchsleep.trm_reason == HDRSRCHRF_OVHD  )
    {
      hdrsrchsleep.ovhd_denied_cnt ++;
    }
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

    /* If TRM denial because 1x was in traffic, do the following */
    if ( HDRSRCHSLEEP_MAXIMUM_SLOTS > hdrsrchsleep.sleep_slots )
    {
      hdrsrchsleep_extend_sleep( HDRSRCHSLEEP_MAXIMUM_SLOTS - hdrsrchsleep.sleep_slots );
      /* Use sleepctl as a 60s max sleep timer. Take prevous sleep slots are
         taken into account too. */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
      (void) hdrsrchsleep_schedule_fast_wakeup( TRUE, HDRSRCHRF_OVHD );
        /* Wake up as soon as RF resources are granted */
#else
      (void) hdrsrchsleep_schedule_fast_wakeup( TRUE);
        /* Wake up as soon as RF resources are granted */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

#ifdef FEATURE_NPA
   ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup. */
#endif /* FEATURE_NPA */
    }
    else
    {
      hdrsrchsleep_extend_sleep( HDRTS_SLOTS_PER_SCC );
        /* Extend sleep to the by 426ms so that system lost below can be processed
           properly including deactivate cmd in sleep state. */

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
        /* Asleep for too long - give up on HDR. */
    }

     HDRSRCH_SIG_SET( HDRSRCH_TICKLE_SIG );
    /* force reprocessing of deactivate command (if any) */
  }

} /* hdrsrchsleep_rf_warmup( ) */


#ifdef FEATURE_HDR_DIME_MODEM
/*===========================================================================

FUNCTION HDRSRCHSLEEP_SEND_SLEEP_ADJUST_MSG

DESCRIPTION
  This function sends sleep_adjust message to FW.
  
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsleep_send_sleep_adjust_msg( void )
{
  
  /*  Send sleep adjust message to FW. FW should adjust RTC and Rx/TX compare
      count, enable half slot interrupt upon receiving this message. In addition
      FW should ajust RTC to make sure the interrupt won't fire before online
      interrupt.
  */
#ifdef FEATURE_HDR_RUMI
  qword cycle;
  uint32 slot = 0;
#endif

  uint32                          rtc_used;
    /* Final rtc value given to FW */

  uint32                          time_to_rtc_on_us = 0;
    /* Time in sclk until to rtc on */

  uint64                          time_slept_cx8;
    /* time already slept */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
  hdrts_get_cycle_and_slot(cycle, &slot, HDRTS_SLOTS_PER_CCC);
    /* extra debugging info for bring up only */
#endif /* FEATURE_HDR_RUMI */

  /* Fail safe to avoid system time errors if MCPM wakeup is longer than warmup time. 
     Check that the rtc now is earlier than rtc start time sent to FW */
  time_slept_cx8 = slpc_get_sysclk_count( SLPC_HDR );
    /* time that we have already slept, in cx8 */

  if (hdrsrchsleep.sleep_duration_cx8 > time_slept_cx8 )
  {
    time_to_rtc_on_us = hdrsrchsleep.sleep_duration_cx8 - time_slept_cx8;
      /* Get time to online interrupt. Now it is in cx8 */

    time_to_rtc_on_us = (time_to_rtc_on_us >> 3)*625/768;
      /* Convert it to micro second. 1 cdma chip = 1000/1228.8 us = 625/768 us */
  }
  else
  {
    time_to_rtc_on_us = 0;
      /* Fire soon, seems we've already missed the boat. */
  }

  rtc_used = (hdrsrchsleep.online_hdr_t_stmr.time & HDR_PN_CIRC_MASK_X8);

  HDR_MSG_SRCH_7( MSG_LEGACY_HIGH, 
       "Sleep_adj_msg, online_univ_stmr=0x%x,cdma_sys_time=0x%x%08x,online_rtc=0x%x%08x,diff=%d,rtc=0x%x",
       hdrsrchsleep.online_univ_stmr,
       I64_HI(hdrsrchsleep.online_cdma_sys_time),
       I64_LO(hdrsrchsleep.online_cdma_sys_time),
       I64_HI(hdrsrchsleep.online_hdr_t_stmr.time),
       I64_LO(hdrsrchsleep.online_hdr_t_stmr.time),
       time_to_rtc_on_us,
       hdrts_get_rtc_cx8()
       );

  /* If fast wakeup and less than 75 us to go, not enough time to process message 
     or if start point is in past, extend our time line to FW */
  if (( time_to_rtc_on_us < 75) && (hdrsrchsleep.cur_warmup_time_is_optimized == FALSE))
  {
    hdrsrchsleep.online_cdma_sys_time += 0x4000;   /* Add half frame, in chips */
    rtc_used += 0x20000;                           /* Add half frame, in chipx8 */

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
       "Error: Delayed wake, adjusted Sleep_adj_msg, cdma_sys_time=0x%x%08x,online_rtc=0x%x,diff=%d",
       I64_HI(hdrsrchsleep.online_cdma_sys_time),
       I64_LO(hdrsrchsleep.online_cdma_sys_time),
       rtc_used,
       time_to_rtc_on_us
       );
  }

  hdrsrchmsg_send_sleep_adj_msg ( hdrsrchsleep.online_univ_stmr,
                                  hdrsrchsleep.online_cdma_sys_time,
                                  (rtc_used & HDR_PN_CIRC_MASK_X8)
                                 );
} /* hdrsrchsleep_start_and_adjust_rtc */
#else /* FEATURE_HDR_DIME_MODEM */
/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_RTC

DESCRIPTION
  This function adjusts RTC count after sleep, based on the time
  slept. The function also configures the DSP's initial interrupt
  timing for immediately following RTC on.

DEPENDENCIES
  Must be called during sleep, when RTC is off.

PARAMETERS
  None

RETURN VALUE
  Whether the adjustment was made before the RTC came on.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchsleep_adjust_rtc( void )
{

  uint32 rtc_cnt;
    /* RTC Count, value at sleep. */

  uint32 rtc_cnt_adjust;
    /* RTC Count, adjusted for sleep. */

  uint32 rxtx_cmp_cnt;
    /* Rx and Tx Compare Count. In DSP command format. */

  uint16 rx_compare_count;
    /* Rx Compare Count: Controls timing of DSP Rx interrupt. */

  uint16 tx_compare_count;
    /* Tx Compare Count: Controls timing of DSP Tx interrupt. */

  uint16 hstr;
    /* HSTR. Value from entering sleep. */

  uint16 hstr_to_restore;
    /* HSTR to restore into the firmware */

  uint32 sleep_chips = hdrsrchsleep.sleep_slots * CHIPS_PER_SLOT;
    /* Sleep slots, in chips. */

  uint32 rx_compare_advance_chips = HDRSRCHSLEEP_RX_COMPARE_ADVANCE_CHIPS;
    /* Number of chips to advance the DSP's Rx interrupt. */

  uint32 sclks_remaining;
    /* Number of sclks until end event (RTC on). */

  boolean rtc_off;
    /* Whether the sleep adjustment happened before the RTC came on. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize the counters */
  rtc_cnt = rxtx_cmp_cnt = 0;

  /* Adjust HSTR by slots sleep, *2 to account for halfslots */
  hstr = HDRHSTR_GET_COUNT();
    /* Get the hstr saved in the firmware */
  
  hstr_to_restore = hstr + (hdrsrchsleep.sleep_slots * 2);
    /* The hstr count to restore in the firmware */

  hdrsrchutil_read_rtc( &rtc_cnt, &rxtx_cmp_cnt );
    /* Latch RTCcnt through firmware interface */

  rtc_cnt_adjust = ((rtc_cnt + (sleep_chips * 8)) & HDR_PN_CIRC_MASK_X8);

  /* Separate into different variables for clarity. */
  rx_compare_count = rxtx_cmp_cnt >> 16;
  tx_compare_count = rxtx_cmp_cnt & 0xFFFF;

  /* Adjust RxCompare, including any advancement, and TxCompare.
     These are in chipx2. */
  rx_compare_count += (uint16)(sleep_chips + rx_compare_advance_chips) * 2;
  tx_compare_count += (uint16)(sleep_chips * 2);

  /* Intlock to guarantee timing.  Don't worry, we will be brief. */
  REX_DISABLE_PREMPTION();

  sclks_remaining = sleepctl_get_sclk_till_rtc_on(SLEEPCTL_HDR);
    /* Grab slow clocks until RTC on. */

  /* Ensure that RTC is not yet on, and that we have enough time to write the
     RTC adjustments. */
/* TBD: Not to use MPC cmd for now */
#if 0
  if (sclks_remaining > 2)
  {

    hdrsrchmsg_send_mpc_cfg_msg( hstr_to_restore, 
                                 rtc_cnt_adjust,
                                 tx_compare_count,
                                 rx_compare_count );
      /* Set HSTR, RTC, and RxTx to new values and send the message */

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                    "Restored RTCcnt 0x%08x, RXTXcnt 0x%x0x%x",
                    rtc_cnt_adjust,
                    rx_compare_count,
                    tx_compare_count );

    rtc_off = TRUE;
  }
  else
#endif /* if 0 */
  {
    hdrsrchmsg_send_sleep_adj_msg( hdrsrchsleep.sleep_slots );
      /* If RTC is already on, we missed our deadline. Inform the MDSP how 
         long chipx16 has been off using sleepAdjust interface and allow 
         the MDSP to adjust itself. Note that in this condition the timeline
         has had significant delay. */

    rtc_off = FALSE;
  }

  /* See? */
  REX_ENABLE_PREMPTION();

  HDR_MSG_SRCH_2(MSG_LEGACY_MED,
                 "Slept %d slots. %d sclks until RTC on", 
                 hdrsrchsleep.sleep_slots, sclks_remaining );

  return rtc_off;
} /* hdrsrchsleep_adjust_rtc */
#endif /* FEATURE_HDR_DIME_MODEM */

#ifdef FEATURE_HDR_WUTA
/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_RF_WARMUP_ON_RTC_ON

DESCRIPTION
  This function is called when the RTC on interrupt fires. This call may 
  reduce the warmup time if there is enough slack in the timeline. 

DEPENDENCIES
  hdrsrchsleep_adjust_rf_warmup_on_tune_done must be called when the RF tune
  is complete.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Updates rf_warmup structure to reflect current timing of RTC on.

===========================================================================*/

LOCAL void hdrsrchsleep_adjust_rf_warmup_on_rtc_on( void )
{
  uint32        cur_slack_us;
    /* Slack time in the current wakeup. */

  timetick_type cur_time_sclks; 
    /* Absolute time in sclks. */

  uint32        old_warmup_time_us;
    /* Old warmup time. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();
    /* This is a critical section. */

  cur_time_sclks = timetick_get_safe();
    /* Get the time RTC is done. */

  /* Was the tune completed before the RTC came on. */
  if ( hdrsrchsleep.rf_warmup.tune_done_time_sclks > 0 )
  {
    old_warmup_time_us = hdrsrchsleep.rf_warmup.cur_warmup_time_us;
      /* The old warmup time before adjustment. */

    cur_slack_us = (uint32) timetick_sclk_to_prec_us( cur_time_sclks - 
                                 hdrsrchsleep.rf_warmup.tune_done_time_sclks );
      /* Determine how much slack is in the timeline this wakeup. */

    hdrsrchsleep.rf_warmup.cur_warmup_time_us -= 
                                             ((int32)( (int32)cur_slack_us - 
                                             HDRSRCHSLEEP_RF_WARMUP_DC_US ))/6;
      /* Increase the current wakeup time. */

    hdrsrchsleep.rf_warmup.tune_done_time_sclks = 0;
      /* Reset tune done time value. */

    if ( hdrsrchsleep.rf_warmup.cur_warmup_time_us < 
                                                HDRSRCHSLEEP_MIN_RF_WARMUP_US )
    {
      hdrsrchsleep.rf_warmup.cur_warmup_time_us = 
                                                 HDRSRCHSLEEP_MIN_RF_WARMUP_US;
        /* Cap the minimum the warmup time can be set to. */
    }
    else if ( hdrsrchsleep.rf_warmup.cur_warmup_time_us > 
                                                HDRSRCHSLEEP_MAX_RF_WARMUP_US )
    {
      hdrsrchsleep.rf_warmup.cur_warmup_time_us = 
                                                 HDRSRCHSLEEP_MAX_RF_WARMUP_US;
        /* Cap the maximum the warmup time can be set to. */
    }

    hdrsrchlog.sleep_info.cur_warmup_time_us = old_warmup_time_us;
      /* Capture for the sleep info packet what our warmup time was. */

    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR,
                    "DO RF warmup fine old %d new %d slack %d", 
                    old_warmup_time_us,
                    hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                    cur_slack_us);

    if ( hdrsrchsleep.rf_warmup.cur_warmup_time_us >
           HDRSRCHSLEEP_MAX_RF_WARMUP_WARNING_US )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                      "WARNING: DO RF warmup %d", 
                      hdrsrchsleep.rf_warmup.cur_warmup_time_us );
    }
  }
  else
  {
    hdrsrchsleep.rf_warmup.rtc_on_time_sclks = cur_time_sclks;
      /* Capture the RTC on time. */
  }

  REX_ENABLE_PREMPTION();
    /* End the critical section. */

} /* hdrsrchsleep_adjust_rf_warmup_on_rtc_on */


/*===========================================================================

FUNCTION HDRSRCHSLEEP_CHECK_SLACK_ON_RTC_ON

DESCRIPTION
  This function checks the difference between rtc on and warmup done. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
LOCAL void hdrsrchsleep_check_slack_on_rtc_on( void )
{
  uint32        cur_slack_us;
    /* Slack time in the current wakeup. */

  timetick_type cur_time_sclks; 
    /* Absolute time in sclks. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();
    /* This is a critical section. */

  cur_time_sclks = timetick_get_safe();
    /* Get the time RTC is done. */

  /* Was the tune completed before the RTC came on. */
  if ( hdrsrchsleep.rf_warmup.tune_done_time_sclks > 0 )
  {
    cur_slack_us = (uint32) timetick_sclk_to_prec_us( cur_time_sclks - 
                                 hdrsrchsleep.rf_warmup.tune_done_time_sclks );
      /* Determine how much slack is in the timeline this wakeup. */

    hdrsrchsleep.rf_warmup.tune_done_time_sclks = 0;
      /* Reset tune done time value. */

    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "DO RF warmup fine slack %d,WARMUP=%d", 
                    cur_slack_us, HDRSRCHSLEEP_RF_WARMUP_OPTIMIZED_US );

    hdrsrchlog.sleep_info.slack_us = cur_slack_us;
      /* Save for sleep log packet */

    hdrsrchsleep.rf_warmup.actual_warmup_time_us = 
      (uint32) timetick_sclk_to_prec_us
        ( cur_time_sclks - 
          hdrsrchsleep.rf_warmup.rf_warmup_start_sclks );
      /* Actual warm up time is the difference between current time and
         when the RF warmup function was called  */

    hdrsrchlog.sleep_info.actual_rf_warmup_us = 
      hdrsrchsleep.rf_warmup.actual_warmup_time_us;
      /* Capture actual warm up time for sleep info packet */
  }
  else
  {
    hdrsrchsleep.rf_warmup.rtc_on_time_sclks = cur_time_sclks;
      /* Capture the RTC on time. */
  }

  REX_ENABLE_PREMPTION();
    /* End the critical section. */

} /* hdrsrchsleep_adjust_rf_warmup_on_rtc_on */

/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_WAKEUP_SCI

DESCRIPTION
  This adjusts the wake up time. 

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None. 

SIDE EFFECTS
  None
  
==========================================================================*/

LOCAL void hdrsrchsleep_adjust_wakeup_sci( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchreacq_calc_win_size_fctr( );
   /* Calculate the window size fctor for adjustment to srch window
      on next reacq search. Sleep time adjustment is done in this function
      too.
   */

} /* hdrsrchsleep_adjust_wakeup_sci*/


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_RF_WARMUP_ON_TUNE_DONE

DESCRIPTION
  This function is called when the RF tune is completed. This call can only 
  increase the warmup time if it notices the RTC is already on.  

DEPENDENCIES
  hdrsrchsleep_adjust_rf_warmup_on_rtc_on must be called when the RTC on 
  interrupt fires.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Updates rf_warmup structure to reflect current timing of the tune operation
  completing.

===========================================================================*/

void hdrsrchsleep_adjust_rf_warmup_on_tune_done( void )
{
  uint32        overrun_us;
    /* How much we overran the current timeline. */

  timetick_type cur_time_sclks; 
    /* Absolute time in sclks. */

  uint32        old_warmup_time_us;
    /* Old warmup time. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();
    /* This is a critical section. */

  cur_time_sclks = timetick_get_safe();
    /* Get the time RTC is done. */

  if ( hdrsrchsleep.rf_warmup.rtc_on_time_sclks > 0 )
  {
    old_warmup_time_us = hdrsrchsleep.rf_warmup.cur_warmup_time_us;
      /* The old warmup time before adjustment. */

    overrun_us = (uint32) timetick_sclk_to_prec_us( cur_time_sclks - 
                                 hdrsrchsleep.rf_warmup.rtc_on_time_sclks);
      /* Determine how much we overran the timeline this wakeup. */

    hdrsrchsleep.rf_warmup.cur_warmup_time_us += 
                      ( overrun_us + HDRSRCHSLEEP_RF_WARMUP_DC_US )/6;
      /* Increase the current wakeup time. */

    hdrsrchsleep.rf_warmup.rtc_on_time_sclks = 0;
      /* Reset the RTC on time. */

    if ( hdrsrchsleep.rf_warmup.cur_warmup_time_us < 
                                                HDRSRCHSLEEP_MIN_RF_WARMUP_US )
    {
      hdrsrchsleep.rf_warmup.cur_warmup_time_us = 
                                                 HDRSRCHSLEEP_MIN_RF_WARMUP_US;
        /* Cap the minimum the warmup time can be set to. */
    }
    else if ( hdrsrchsleep.rf_warmup.cur_warmup_time_us > 
                                                HDRSRCHSLEEP_MAX_RF_WARMUP_US )
    {
      hdrsrchsleep.rf_warmup.cur_warmup_time_us = 
                                                 HDRSRCHSLEEP_MAX_RF_WARMUP_US;
        /* Cap the maximum the warmup time can be set to. */
    }

    hdrsrchlog.sleep_info.cur_warmup_time_us = old_warmup_time_us;
      /* Capture for the sleep info packet what our warmup time was. */

    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR,
                    "DO RF warmup overrun old %d new %d slack -%d", 
                    old_warmup_time_us,
                    hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                    overrun_us);
    hdrsrchlog.sleep_info.slack_us = 0 - overrun_us;
      /* Convert slack to negative value (it is overrun) for logging purpose */

  }
  else
  {
    hdrsrchsleep.rf_warmup.tune_done_time_sclks = cur_time_sclks;
      /* Get the time RTC is done. */
  }

  REX_ENABLE_PREMPTION();
    /* End the critical section. */

  hdrsrchsleep.rf_warmup.actual_warmup_time_us = 
    (uint32) timetick_sclk_to_prec_us
      ( cur_time_sclks - 
        hdrsrchsleep.rf_warmup.rf_warmup_start_sclks );
    /* Actual warm up time is the difference between current time and
       when the RF warmup function was called  */

  hdrsrchlog.sleep_info.actual_rf_warmup_us = 
    hdrsrchsleep.rf_warmup.actual_warmup_time_us;
    /* Capture actual warm up time for sleep info packet */

} /* hdrsrchsleep_adjust_rf_warmup_on_tune_done */


/*===========================================================================

FUNCTION HDRSRCHSLEEP_CHECK_SLACK_ON_TUNE_DONE

DESCRIPTION
  This function checks how much warmup overuun.  

DEPENDENCIES
  None
PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void hdrsrchsleep_check_slack_on_tune_done( void )
{
  uint32        overrun_us;
    /* How much we overran the current timeline. */

  timetick_type cur_time_sclks; 
    /* Absolute time in sclks. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();
    /* This is a critical section. */

  cur_time_sclks = timetick_get_safe();
    /* Get the time RTC is done. */

  if ( hdrsrchsleep.rf_warmup.rtc_on_time_sclks > 0 )
  {
    overrun_us = (uint32) timetick_sclk_to_prec_us( cur_time_sclks - 
                                 hdrsrchsleep.rf_warmup.rtc_on_time_sclks);
      /* Determine how much we overran the timeline this wakeup. */

    hdrsrchsleep.rf_warmup.rtc_on_time_sclks = 0;
      /* Reset the RTC on time. */

    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DO RF warmup overrun slack -%d", overrun_us);

    hdrsrchlog.sleep_info.slack_us = 0 - overrun_us;
      /* Convert slack to negative value (it is overrun) for logging purpose */

    hdrsrchsleep.rf_warmup.actual_warmup_time_us = 
      (uint32) timetick_sclk_to_prec_us
        ( cur_time_sclks - 
          hdrsrchsleep.rf_warmup.rf_warmup_start_sclks );
      /* Actual warm up time is the difference between current time and
         when the RF warmup function was called  */

    hdrsrchlog.sleep_info.actual_rf_warmup_us = 
      hdrsrchsleep.rf_warmup.actual_warmup_time_us;
      /* Capture actual warm up time for sleep info packet */

  }
  else
  {
    hdrsrchsleep.rf_warmup.tune_done_time_sclks = cur_time_sclks;
      /* Get the time RTC is done. */
  }

  REX_ENABLE_PREMPTION();
    /* End the critical section. */

} /* hdrsrchsleep_check_slack_on_tune_done */
#endif /* FEATURE_HDR_WUTA */

/* EJECT */
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_SEND_CC_CONFIG_MSG

DESCRIPTION
  This function sends the message to the firmware to set CC start and/or 
  SCC offset.
  
DEPENDENCIES
  None 

INPUTS
  scc_offset: SCC offset sent to FW.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchsleep_send_cc_config_msg
(
  uint16           scc_offset
    /* SCC offset*/
)
{
  hdrsrchsleep_fw_msg_struct_type            fw_msg;
    /* Messages sent to firmware */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &fw_msg.cc_config_msg,
          0,
          sizeof( hdrfw_scc_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &fw_msg.cc_config_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SCC_CMD );
    /* Init the message header */

#ifdef FEATURE_HDR_REVA_L1
  fw_msg.cc_config_msg.ccStart = hdrsrchsleep.cc_start;
  fw_msg.cc_config_msg.ccStart_Valid = 1;
    /* Set cc start in the message payload */
#endif /* FEATURE_HDR_REVA_L1 */

  fw_msg.cc_config_msg.sccOffset = scc_offset;
  fw_msg.cc_config_msg.sccOffset_Valid = 1;
   /* Set SCC offset in the message payload */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV

  if( hdrsrch.idle_state_div.idle_div_enabled &&  
      (hdrsrch.idle_state_div.subpkt_thresh > 0) &&
      (hdrsrch.idle_state_div.subpkt_thresh < 15) )
  {
    fw_msg.cc_config_msg.enableSccDecodeInd = 1;
    fw_msg.cc_config_msg.sccDecodeIndThreshold = 
      hdrsrch.idle_state_div.subpkt_thresh;
    fw_msg.cc_config_msg.enableSccDecodeInd_Valid = 1;
  
  }
  else
  {
    fw_msg.cc_config_msg.enableSccDecodeInd_Valid = 0;
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  hdrsrchmsg_send_msg( &fw_msg.cc_config_msg.hdr,
                       sizeof( hdrfw_scc_msg_t ) );
    /* Msgr copies the message and hence ok to use local variable */

} /* hdrsrchsleep_send_cc_config_msg */

/*==========================================================================

FUNCTION      HDRSRCHSLEEP_DEACTIVATE_SCC_DECODE_IND

DESCRIPTION
  This function sends the message to the firmware to deactivate scc decode
  indication.
  
DEPENDENCIES
  None 

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_deactivate_scc_decode_ind( void )
{
  hdrsrchsleep_fw_msg_struct_type            fw_msg;
    /* Messages sent to firmware */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: hdrsrchsleep_deactivate_scc_decode_ind");

  memset( &fw_msg.cc_config_msg,
          0,
          sizeof( hdrfw_scc_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &fw_msg.cc_config_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SCC_CMD );
    /* Init the message header */

  /* Set SCC offset in the message payload */
  fw_msg.cc_config_msg.enableSccDecodeInd = 0;
  fw_msg.cc_config_msg.enableSccDecodeInd_Valid = 1;  
  
  hdrsrchmsg_send_msg( &fw_msg.cc_config_msg.hdr,
                       sizeof( hdrfw_scc_msg_t ) );
    /* Msgr copies the message and hence ok to use local variable */

} /* hdrsrchsleep_deactivate_scc_decode_ind */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_ACTIVATE_SCC_DECODE_IND

DESCRIPTION
  This function sends the message to the firmware to send
  scc decode indication for m slots.
  
DEPENDENCIES
  None 

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_activate_scc_decode_ind ( void )
{
  hdrsrchsleep_fw_msg_struct_type            fw_msg;
    /* Messages sent to firmware */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &fw_msg.cc_config_msg,
          0,
          sizeof( hdrfw_scc_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &fw_msg.cc_config_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SCC_CMD );
    /* Init the message header */

  if( hdrsrch.idle_state_div.idle_div_enabled &&  
      (hdrsrch.idle_state_div.subpkt_thresh > 0) &&
      (hdrsrch.idle_state_div.subpkt_thresh < 15) )
  {
    fw_msg.cc_config_msg.enableSccDecodeInd = 1;
    fw_msg.cc_config_msg.sccDecodeIndThreshold = 
      hdrsrch.idle_state_div.subpkt_thresh;
    fw_msg.cc_config_msg.enableSccDecodeInd_Valid = 1;
  }
  
  hdrsrchmsg_send_msg( &fw_msg.cc_config_msg.hdr,
                       sizeof( hdrfw_scc_msg_t ) );
    /* Msgr copies the message and hence ok to use local variable */

} /* hdrsrchsleep_activate_scc_decode_ind */

#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV */
/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_RF_WARMUP_DONE

DESCRIPTION
  This function is called at the end of RF warmup after sleep.

  DSP Starts operating after this event has occured.

DEPENDENCIES
  Must only be called after hdrsrchidle_rf_warmup( ), after rxcx16 is
  re-enabled

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Resets Pilot Filter

===========================================================================*/

LOCAL void hdrsrchsleep_rf_warmup_done( void )
{
  uint16                          rotate;
    /* PN Circle rotation (or RTC Offset adjustment) */

  uint16                          current_hstr;
    /* the current HSTR value */

#ifdef FEATURE_HDR_REVB
  hdrsrch_sect_struct_type        *aset_sect;
    /* Convinience pointor */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_SRCH( MSG_LEGACY_MED, "rf_warmup_done");
#endif /* FEATURE_HDR_RUMI */

#ifdef FEATURE_HDR_WUTA
  if ( HDRSRCH_WUTA_NV_ENABLE )
  {
    hdrsrchsleep_adjust_rf_warmup_on_rtc_on();
      /* Adjust the rf warmup time if necessary. */
  }
  else
  {
    hdrsrchsleep_check_slack_on_rtc_on();
      /* Check slack */
  }
#endif /* FEATURE_HDR_WUTA */

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RFWU_DONE_ISR );
    /* Output powr profile strobe at 2nd slp controller ISR if 
       feature is enable */

  hdrsrchsleep_dump_time ();

  current_hstr = hdrsrchsleep.hstr_at_sleep + hdrsrchsleep.sleep_slots * 2;
    /* Get the hstr count at wakeup */    
  
  hdrts_univ_dump();
    /* Dump RTC */

  hdrts_config_mod_mp_time_ref();
    /* Config modetime_ref */

  hdrts_wake( hdrsrchsleep.sleep_slots, hdrsrchsleep_set_time_valid );
    /* Let time-keeping know how long we slept for */

  rotate = hdrsrchsleep.sleep_slots % HDRTS_SLOTS_PER_FRAME;
    /* Determine the fractional frames (PN rolls) of sleep, in slots.
       The PN Circle must be rotated by this amount, which can be
       done by adjusted RTC Offset by a corresponding amount. */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDR RF Warmup complete: Rotate=%d",
                  rotate );

  /*----------------------*
   * Ensure DSP resources *
   *----------------------*/

  /* MSM6500 DSP applications are expected to run 1x, HDR and GPS concurrently.
     So when HDR wants to enable DSP application, it should be made available
     to HDR immediately. We don't expect to break this rule in MSM6500. However
     following is added as recovery if the dsp services can not enable the 
     HDR application even after few msec after warmup isr */

  if ( hdrmdsp_check_dsp_clk() == FALSE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_FATAL, "Could not wakeup w/o HDR DSP application" );

    /* Option to declare system loss if this situation is found to be 
       recoverable */

    hdrsrchreacq_update_reacq_info( NULL, HDRLOG_REACQ_LOST_DSP_NOT_READY );
      /* Update REACQ INFO for re-acquisition failure reason  */
    
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_DSP_NOT_READY );
      /* declare system lost since DSP resources are not available */

    return;
      /* Return if we are declaring system lost! */
  }

  /*---------------------------*
   * Reset Pkt Sequence number *
   *---------------------------*/

  hdrdec_reset_pkt_sequences();
    /* Reset packet sequence numbers */

  /*----------------------------------------*
   * Restore CC config after HDR app enable *
   *----------------------------------------*/

  /* Sending CC config message to FW only if MDSP is active */
  if ( hdrmdsp_is_mdsp_active() )
  {
  if ( ( ( current_hstr % HDRSRCHSLEEP_HS_PER_SCC ) < HDRSRCHSLEEP_SUBSYNC_HS_LOW ) &&
       ( hdrsrchsleep_get_intra_scc_valid() == FALSE ) )
  {
    hdrsrchsleep_send_cc_config_msg( 0xffff );
      /* Send CC config message to FW for CC start and SCC offset.
         Initialize to decode all interlaces for CC Sync wakeups only;
         no change for Sub-Sync wakeups or intra-SCC wakeups */
  }
  else
  {
    hdrsrchsleep_send_cc_config_msg( hdrsrchsleep.scc_offset );
      /* Send CC config message to FW for CC start and SCC offset.
         No change for Sub-Sync wakeups or intra-SCC wakeups */
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "CC_Start = %d", hdrsrchsleep.cc_start );
  }

  /* TBD: use MAC API or remove */
  /*HDRMDSP_CLEAR_MISSED_SYNC_CC_FLAG();*/
    /* Now clear the missed sync cc flag. The missed CC ISR will be installed
       once we enter the idle monitor state. Interrupts set in between now
       and then will fire after the ISR is installed. */

#ifdef FEATURE_HDR_REVB

  aset_sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0];
    /* Points to the aset0 of the idle subaset */

  /* Restore best ASP FL register */
  hdrsrchmsg_basp_ho_ovrd( HDRMDSP_DEMOD_CARRIER_0, 
                               aset_sect->aset.asp_idx );
    /* Send the handoff override message */

  HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                 aset_sect->aset.asp_idx, 
                 aset_sect->pn_offset );
    /* Write PN offset */

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0,
                        aset_sect->aset.asp_idx,
                        aset_sect->aset.drc_cover );
  /* write the DRC walsh cover for the new ASP */

  HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0,
                      aset_sect->aset.asp_idx,
                      aset_sect->link_id );
  
  hdrsrchmsg_send_asp_update_msg( HDRMDSP_DEMOD_CARRIER_0,
                                  aset_sect->aset.asp_idx );
    /* Send the ASP update message */

#else
  HDRDRC_SET_BEST_ASP( hdrsrchset.aset[0]->aset.asp_idx );
  
  HDRASP_SET_BASP_FL( hdrsrchset.aset[0]->aset.asp_idx );
    /* Restore best ASP FL register */

  HDRASP_SET_PN( HDRMDSP_DEMOD_CARRIER_0,
                 hdrsrchset.aset[0]->aset.asp_idx, 
                 hdrsrchset.aset[0]->pn_offset );
    /* Write PN offset */

  HDRASP_SET_DRC_COVER( HDRMDSP_DEMOD_CARRIER_0,
                        hdrsrchset.aset[0]->aset.asp_idx,
                        hdrsrchset.aset[0]->aset.drc_cover );
  /* write the DRC walsh cover for the new ASP */

  HDRASP_SET_LINK_ID( HDRMDSP_DEMOD_CARRIER_0,
                      hdrsrchset.aset[0]->aset.asp_idx,
                      hdrsrchset.aset[0]->link_id );

#endif /* FEATURE_HDR_REVB */
  /*------------------*
   * Activate Decoder *
   *------------------*/

  HDRSRCH_SIG_SET( HDRSRCH_RXCHIPX16_SIG );
    /* Signal the task that RxChipx16 has been gated on, again */

  hdrsrchreacq_start_fpd_timer();
    /* Schedule preamble detection in FW 1ms after sleep */

} /* hdrsrchsleep_rf_warmup_done( ) */

#ifdef FEATURE_HDR_BOLT_MODEM
/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_OLS_HANDLER

DESCRIPTION
  This function handles OLS signal (equivalent to rf_warmup_done) and
  call either rf_warmup_done or deactivate_rf_warmup_done()

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchsleep_ols_handler
( 
  uint32 timer_id 
)
{
  if ( hdrsrchsleep.ols_cb != NULL )
  {
    hdrsrchsleep.ols_cb( );
  }
}
#endif /* FEATURE_HDR_BOLT_MODEM */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_COMPUTE_FAST_SLEEP_TIME_SLOTS

DESCRIPTION
  Computes a fast sleep time, i.e., shortest time to sleep from now in slots.

DEPENDENCIES
  None

PARAMETERS
  chips_slept - How long sleepctl has slept

RETURN VALUE
  Fast Sleep Time in slots.

SIDE EFFECTS
  None

===========================================================================*/
static uint32 hdrsrchsleep_compute_fast_sleep_time_slots
(
  uint32 chips_slept
)
{
  uint32                          slots;
    /* Time, in slots.  Used to compute new early wakeup time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  chips_slept += MS_TO_CHIPS( 
                ( HDRSRCHSLEEP_RF_WARMUP_DEFAULT_US + 999 ) / 1000 );
    /* Add in a conservative estimate of the RF warmup time. */

  slots = (chips_slept + (CHIPS_PER_SLOT/2)) >> CHIPS_PER_SLOT_SHIFT;
    /* And convert to slots */

  slots += 3;
    /* Add 5ms for reading, modify, write overhead. */

  slots = hdrsrchsleep.sleep_slots - slots;
    /* Convert to "slots remaining in sleep" */

  slots &= ~15;
    /* Round "slots remaining down" to a multiples of frames.
       This preserves the wakeup slot in a frame. */

  slots = hdrsrchsleep.sleep_slots - slots;
    /* Convert back to slots from the start of sleep */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "compute_fast_wakeup new sleep_slots= %d", 
                 slots );

  return slots;

} /* hdrsrchsleep_compute_fast_sleep_time_slots */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_FAST_WAKEUP

DESCRIPTION
  Reprograms the sleep controller to wake up as soon as possible.

DEPENDENCIES
  Must only be used as callback to sleepctl_set_warmup( )

PARAMETERS
  None

RETURN VALUE
  Whether the fast wake up has been programmed into sleep controller. 

SIDE EFFECTS
  None

===========================================================================*/

LOCAL boolean hdrsrchsleep_fast_wakeup( void )
{
  uint64                          slept_cx8;
    /* slept duration in cx8 */
  uint32                          chips;
    /* Time, in chips.  Used to compute new early wakeup time */

  uint32                          slots;
    /* Time, in slots.  Used to compute new early wakeup time */

  uint32                          slept_slots;
    /* Time, in slots.  Used to compute new early wakeup time */

  boolean                         fast_wakeup = FALSE;                       
    /* Whether the sleep controller is reprogrammed */

  boolean                         change_sleep_duration = FALSE;
    /* Whether to change sleep duration */

#ifdef FEATURE_HDR_DIME_MODEM
  int32                           delta_slots;
    /* New delta in slots between the new and old durations. */

  uint32                          old_sleep_slots;
    /* old sleep durations. */

  uint32                          slots_to_frame_boundary = 0;
    /* Slots to the frame boundary */

#endif /* FEATURE_HDR_DIME_MODEM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.intra_scc_start_slot_valid = FALSE;
    /* If we were trying to perform an intra-SCC wakeup we no longer will be,
       since the most important thing is that we acquire at all in the fast
       wakeup case. We will revert to the normal reacq algorithm. */

  REX_DISABLE_PREMPTION();

  /* Determine how long we will have slept for, in slots,
     if RF warmup started right now. */
#ifdef FEATURE_HDR_DIME_MODEM
  old_sleep_slots = hdrsrchsleep.sleep_slots;
    /* Store the old sleep duration */
  slept_cx8 = slpc_get_sysclk_count( SLPC_HDR ) ;
  chips = slept_cx8 >> 3;
    /* How long we've slept for, in chipx8. Convert it to cx1 */
  slept_slots = chips >> CHIPS_PER_SLOT_SHIFT;
        /* And convert to slots */

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
        "Fast wakeup: slept hi 0x%x,lo 0x%x cx8 0x%x chips, %d slots",
        (uint32)(slept_cx8>>32 ), (uint32)(slept_cx8&0xFFFFFFFF),
         chips,slept_slots );
  if ( slept_slots > hdrsrchsleep.sleep_slots)
  {
     HDR_MSG_SRCH( MSG_LEGACY_ERROR,"fast wakeup, wrong slept duration from slpc");  
  }
#else /* FEATURE_HDR_DIME_MODEM */
  chips = sleepctl_get_chip_cnt( SLEEPCTL_HDR );
    /* How long we've slept for, in chips */
#endif /* FEATURE_HDR_DIME_MODEM */

  /* When HDR is deactivated during sleep, HDR tries to fast wakeup. If fast wakeup succeeds,
     the wakeup and end ISRs are updated to deactivate and does not transition to reacq state.
     There is a race condition between wakeup interrupt and ISR update if this function always returns
     fast wakeup success. The solution is to let HDR transition to reacq state if the current
     time is within 3 slots from the original wakeup time. Otherwise, deactivate at sleep state. */
  if ( chips > CHIPS_PER_SLOT )
  {

    slots = hdrsrchsleep_compute_fast_sleep_time_slots( chips );

    if ( slots < hdrsrchsleep.sleep_slots )
    {
      /* This new sleep duration is shorter that the first.  Use it */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Fast wakeup: Old = %d, new = %d",
                      hdrsrchsleep.sleep_slots, slots );

      fast_wakeup = TRUE;
        /* We are programming a fast wakeup time. */

      if ( hdrsrchsleep.cur_warmup_time_is_optimized == TRUE )
      {
        hdrsrchsleep.rf_warmup.cur_warmup_time_us += HDRSRCHSLEEP_RF_WARMUP_FASTWAKEUP_OFFSET_US;
          /* Use cur_warmup_time_us + fast wakeup offset for fast wakeup*/

        hdrsrchsleep.cur_warmup_time_is_optimized = FALSE;
          /* Non-optimized timeline for fast wakeup */

#ifndef FEATURE_HDR_DIME_MODEM
        sleepctl_set_warmup( SLEEPCTL_HDR, hdrsrchsleep.rf_warmup.cur_warmup_time_us );
          /* Set warmup time to sleepctl */
#endif /* !FEATURE_HDR_DIME_MODEM */

        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                        "Fast wakeup, RF warmup time was changed to %d, RPM optimized timeline %d, cur timeline %d", 
                        hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                        hdrsrchsleep.rpm_warmup_time_is_optimized,
                        hdrsrchsleep.cur_warmup_time_is_optimized );
      }

      hdrsrchsleep.sleep_slots = slots;
        /* Save the new sleep duration */

      change_sleep_duration = TRUE;
        /* Change sleep duration */

    }
    else
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Fast wakeup: new sleep_slots >= old sleep_slots Old = %d, new = %d",
                      hdrsrchsleep.sleep_slots, slots );

      chips += MS_TO_CHIPS( 
                    ( HDRSRCHSLEEP_RF_WARMUP_DEFAULT_US + 999 ) / 1000 );
        /* Add in a conservative estimate of the RF warmup time. */

      slept_slots = (chips + (CHIPS_PER_SLOT/2)) >> CHIPS_PER_SLOT_SHIFT;
        /* And convert to slots */

      if ( ( slept_slots + 3 ) < hdrsrchsleep.sleep_slots )
      {
        fast_wakeup = TRUE;
          /* Set the flag */

        if ( hdrsrchsleep.cur_warmup_time_is_optimized == TRUE )
        {
          hdrsrchsleep.rf_warmup.cur_warmup_time_us += HDRSRCHSLEEP_RF_WARMUP_FASTWAKEUP_OFFSET_US;
            /* Use cur_warmup_time_us + fast wakeup offset for fast wakeup*/

          hdrsrchsleep.cur_warmup_time_is_optimized = FALSE;
            /* Non-optimized timeline for fast wakeup */

#ifdef FEATURE_HDR_DIME_MODEM
        (void) slpc_set_warmup( SLPC_HDR, hdrsrchsleep.rf_warmup.cur_warmup_time_us );
#else
          sleepctl_set_warmup( SLEEPCTL_HDR, hdrsrchsleep.rf_warmup.cur_warmup_time_us );
            /* Set warmup time to sleepctl */
#endif /* FEATURE_HDR_DIME_MODEM */

          HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                          "Fast wakeup, RF warmup time was changed to %d, RPM optimized timeline %d, cur timeline %d", 
                          hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                          hdrsrchsleep.rpm_warmup_time_is_optimized,
                          hdrsrchsleep.cur_warmup_time_is_optimized );
        }

        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                        "Fast wakeup sleep duration is not changed. old=%d new=%d, slept %d",
                        hdrsrchsleep.sleep_slots, slots, slept_slots );
      }
      else
      {
        HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Fast wakeup ignored. old=%d new=%d slept %d",
                        hdrsrchsleep.sleep_slots, slots, slept_slots );
      }
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Fast Wakeup Ignored.  Slots=0" );
  }

  hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_FAST;
    /* Mark this wakeup as a fast wakeup regardless of whether the time
       is actually changed or not. */

  if ( change_sleep_duration == TRUE )
  {
#ifdef FEATURE_HDR_DIME_MODEM
    delta_slots = old_sleep_slots - hdrsrchsleep.sleep_slots;
      /* delta of original sleep time and current sleep time with fast wakeup. */

    /* Update wakeup time.*/
    if ( hdrsrchsleep.wake_slot > delta_slots )
    {
      hdrsrchsleep.wake_slot -= delta_slots;
    }
    else
    {
      qw_dec( hdrsrchsleep.wake_cycle, 1);
      hdrsrchsleep.wake_slot += HDRTS_SLOTS_PER_CCC;
      hdrsrchsleep.wake_slot -= delta_slots;
    }

    slots_to_frame_boundary = hdrsrchsleep.wake_slot  % HDRTS_SLOTS_PER_FRAME;

    if ( slots_to_frame_boundary >= HDRSRCHSLEEP_FAST_WAKEUP_SLOT_OFFSET_MAX )
    {
        hdrsrchsleep.wake_slot += ( HDRTS_SLOTS_PER_FRAME - slots_to_frame_boundary ) + 1;
        hdrsrchsleep.sleep_slots += ( HDRTS_SLOTS_PER_FRAME - slots_to_frame_boundary ) + 1;

        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                        "Slots to frame boundary %d slots, changed the sleep dur to %d slots", 
                        slots_to_frame_boundary, 
                        hdrsrchsleep.sleep_slots );
    }

    if ( (  hdrsrchsleep.wake_slot  % HDRTS_SLOTS_PER_FRAME )== 0  )
    {
        /* Avoid online interrupt at frame boundary. 
        */
        hdrsrchsleep.wake_slot += 1;
        hdrsrchsleep.sleep_slots += 1;
    }

    hdrsrchsleep_set_warmup_and_sleep_duration( );

#else /* FEATURE_HDR_DIME_MODEM */
    sleepctl_set_duration( SLEEPCTL_HDR, hdrsrchsleep.sleep_slots << CHIPS_PER_SLOT_SHIFT);
      /* And program this as the new sleep duration */
#endif /* FEATURE_HDR_DIME_MODEM */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Change sleep duration to %d slots", hdrsrchsleep.sleep_slots );

    hdrsrchmcpm_extend_sleep();
      /* Update MCPM with new time */

  }

  REX_ENABLE_PREMPTION();

  hdrsrchsleep_reset_sci();
    /* set the SCI back to 0 after fast wakeup */

  return fast_wakeup;

} /* hdrsrchsleep_fast_wakeup( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_SCHEDULE_FAST_WAKEUP

DESCRIPTION
  Request TRM lock before scheduling fast_wakeup. This function is added to
  support band incompatability. Such request will result in other RAT
  releasing TRM lock on incompatable band (if HDR has high priority).

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Whether the fast wake up has been programmed into sleep controller. 

SIDE EFFECTS
  None

===========================================================================*/
LOCAL boolean hdrsrchsleep_schedule_fast_wakeup
(
  boolean need_rf
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  ,hdrsrchrf_reason_enum_t trm_reason
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
)
{
  boolean fast_wakeup;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!need_rf || hdrsrchrf_has_lock(HDRSRCHRF_PRI_CHAIN))
  {
    fast_wakeup = hdrsrchsleep_fast_wakeup();
  }
  else
  {
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    hdrsrchsleep.trm_reason = trm_reason;
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

    fast_wakeup = FALSE;

    hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
      /* Watch for Lock Granted signal */

    (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_PRI_CHAIN_EVENT_SIG );
      /* Clear the Lock Granted signal */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    hdrsrchrf_request_and_notify(HDRSRCHRF_PRI_CHAIN,
                                 HDRSRCHRF_RX_ANY,
                                 HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK,
                                 hdrsrchsleep.trm_reason,
                                 hdrsrch_rf_grant_cb);
#else
    hdrsrchrf_request_and_notify(HDRSRCHRF_PRI_CHAIN,
                                 HDRSRCHRF_RX_ANY,
                                 HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK,
                                 HDRSRCHSLEEP_RF_REASON,
                                 hdrsrch_rf_grant_cb);
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
  }

  return fast_wakeup;
} /* hdrsrchsleep_schedule_fast_wakeup */

#ifdef FEATURE_HDR_DIME_MODEM
/*===========================================================================

FUNCTION HDRSRCHSLEEP_SLPC_START_IN_SAME_SLOT

DESCRIPTION
  Check if slpc_start() and compute_sleep_time() are called in the same slot
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Whether slpc_start() and compute_sleep_time() are called in the same slot 

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchsleep_slpc_start_in_same_slot( uint32 slot_at_compute_sleep_time )
{
  uint32     slot_num_at_slpc_start;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  slot_num_at_slpc_start = 
    ( ( hdrsrchsleep.slpc_start_t_stmr.time + HDR_PN_CIRC_WRAPUP_X8 - hdrsrchdrv_get_mstr_x8() )  
        & HDR_PN_CIRC_MASK_X8 ) >> 3 >> CHIPS_PER_SLOT_SHIFT;
   /* get the slot number (within one frame) at slpc_start */

  slot_at_compute_sleep_time = slot_at_compute_sleep_time % 16;
    /* Within one PN circle */

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_SRCH_5( MSG_LEGACY_MED, "slpc_start:%x%08x rtc:0x%x mstr:0x%x txRTC:0x%x", 
                  hdrsrchsleep.slpc_start_t_stmr.time >> 32, 
                  hdrsrchsleep.slpc_start_t_stmr.time & 0xFFFFFFFFL,
                  hdrts_get_rtc_cx8(), 
                  hdrsrchdrv_get_mstr_x8(), 
                  HDRMDSP_GET_TX_RTC_OFFSET_CX8() );
#endif /* FEATURE_HDR_RUMI */
  
  if ( ( ( slot_at_compute_sleep_time - slot_num_at_slpc_start ) % 
         HDRTS_SLOTS_PER_FRAME ) == 0 )
  {

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                  " HDR: slpc_start(%d) is in the same slot as computing slot time(%d)",
                   slot_num_at_slpc_start,slot_at_compute_sleep_time );
    return TRUE;
  }
  else if ( ( slot_at_compute_sleep_time - slot_num_at_slpc_start ) == 1 )
  {
    /* Need to adjust online time */
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "HDR: slpc_start(%d) is one slot before computing slot time(%d) ",
                    slot_num_at_slpc_start,slot_at_compute_sleep_time );
    return FALSE;
  }
  else
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "ERROR!!! HDR TIME IS NOT RIGHT %d %d",
                    slot_num_at_slpc_start,slot_at_compute_sleep_time );
    return TRUE;
  }
}

/*===========================================================================

FUNCTION HDRSRCHSLEEP_AVOID_SLOT_BOUNDARY

DESCRIPTION
  Check if the current time is close to slot boundary. If it is close, wait
  after the slot boundary is passed
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsleep_avoid_slot_boundary( void )
{
  uint32    curr_hdr_rtc_cx8 = 0;
    /* current hdr rtc */

  uint32    curr_to_slot_boundary_cx8;
    /* current time to next half slot boundary */

  uint32     time_to_wait;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_HDR_BOLT_MODEM
  curr_hdr_rtc_cx8 = hdrts_get_rtc_cx8();  
#else
  curr_hdr_rtc_cx8 = HWIO_IN( HDR_RTC_CNT );
      /* Get the current hdr rtc. HDR_RTC_CNT is in CX2 */
  curr_hdr_rtc_cx8 = curr_hdr_rtc_cx8 << 2;  
#endif /* FEATURE_HDR_BOLT_MODEM */  

  curr_to_slot_boundary_cx8 = 
    ( ( curr_hdr_rtc_cx8 + HDR_PN_CIRC_WRAPUP_X8 - hdrsrchdrv_get_mstr_x8() )  
      & HDR_PN_CIRC_MASK_X8 ) & 0x3FFF;

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "hdrsrchsleep_avoid_half_slot_boundary: hdr_rtc_cx8 0x%x,curr_to_slot_boundary_cx8 0x%x",
                  curr_hdr_rtc_cx8, curr_to_slot_boundary_cx8);

  if ( ( curr_to_slot_boundary_cx8 > ( 2048*8 - HDRSRCHSLEEP_SLOT_BOUNDARY_BUFF_CX8 ) ) ||
       ( curr_to_slot_boundary_cx8 < HDRSRCHSLEEP_SLOT_BOUNDARY_BUFF_CX8 ) )
  {
    /* too close to slot boundary */
    time_to_wait =  ( ( 2048*8 + HDRSRCHSLEEP_AWAY_FROM_SLOT_BOUNDARY_CX8 ) 
                          - curr_to_slot_boundary_cx8 ) & 0x3FFF;
      /* Now time to wait is in cx8 */

    HDR_MSG_SRCH_1( MSG_LEGACY_LOW,
                    "hdrsrchsleep_avoid_half_slot_boundary: wait %d cx8 to next hs",
                    time_to_wait );
    time_to_wait = time_to_wait/10;
      /* Convert time_to_wait to us. 1 chip is about 0.8 us.
         time_to_wait_us = (time_to_wait_cx8 /8) *8/10 = time_to_wait_cx8/10
      */
     HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                    "hdrsrchsleep_avoid_half_slot_boundary: wait %d us to next hs",
                    time_to_wait );
    hdrsrch_timed_wait_usecs( time_to_wait );
  }

  return;

} /* hdrsrchsleep_avoid_slot_boundary */
#endif /* FEATURE_HDR_DIME_MODEM */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_COMPUTE_SLEEP_TIME

DESCRIPTION
  Determine the length of time the AT should sleep for, assuming it starts
  sleep now.

DEPENDENCIES
  hdrsrchsleep_compute_next_cycle( ) must be called prior to this function.

PARAMETERS
  The threshold for minimum sleep duration in slots to go to sleep.
  
RETURN VALUE
  Sleep time in slots, or 0 if insufficient time to sleep.

SIDE EFFECTS
  Updates hdrsrchsleep.sleep_slots

===========================================================================*/

LOCAL uint16 hdrsrchsleep_compute_sleep_time
(
  uint32                          sleep_duration_threshold,
    /* Threshold for minimum sleep duration in slots */
  uint32                          * slot_at_compute_sleep_time
    /* Slot number (within a frame) when compute_sleep_time() is called */
)
{
  qword                           cycle;
    /* Current control channel cycle */

  uint32                          slot;
    /* Current slot */

  uint32                          slots_adj_for_cpu_conflict = 0;
    /* Adjustment backwards in time in slots if there is CPU conflict.  This
       is only used in the Intra-SCC cases, otherwise it is 0. */

  uint32                          adjusted_sleep_time = 0;
    /* New potential sleep time adjusted for CPU conflict, if any. */

  qword                           diff;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrts_get_cycle_and_slot( cycle, &slot, HDRTS_SLOTS_PER_CCC );
      /* Determine the current time in HDRTS_SLOTS_PER_SCC cycles and
         1.67ms slots. */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "compute sleep time cycle 0x%x%08x, slot %d", qw_hi(cycle), qw_lo(cycle), slot );  

  if ( qw_cmp(hdrsrchsleep.wake_cycle, cycle) >= 0)
  {    
    qw_sub(diff, hdrsrchsleep.wake_cycle, cycle);
    qw_mul(diff, diff, HDRTS_SLOTS_PER_CCC);
    hdrsrchsleep.wake_slot += qw_lo(diff);

    qw_equ(hdrsrchsleep.wake_cycle, cycle);
  }
  else
  {    
    qw_sub(diff, cycle, hdrsrchsleep.wake_cycle);
    qw_mul(diff, diff, HDRTS_SLOTS_PER_CCC);
    slot = slot + qw_lo(diff);
  }

  *slot_at_compute_sleep_time = slot % HDRTS_SLOTS_PER_CCC;

  /* If this is intra-SCC sleep, then we need to use intra_scc_start_slot
     to determine if there is enough time to sleep. */
  if ( hdrsrchsleep.intra_scc_start_slot_valid )
  {

    /* Check if there is enough time to sleep for a given duration.  Potential
       wakeup is at SCC Offset boundary since RAHO and IHO are disabled. 
       hdrsrchsleep.wake_slot should take dynamic reacq slots which already
       take CGPS and BCMCS into account.  For Intra-SCC sleep, when checking
       if enough time to sleep, sleep_duration_threshold should contain
       WUTA slots + some buffer.  */
    if ( hdrsrchsleep.wake_slot > ( slot +  sleep_duration_threshold ) )
    {
      /* Must be potential intra-SCC sleep.  Since small SCI's are affected
         by CPU conflicts, check with sleep controller if there is a conflict
         and back-off, if necessary.  First, get each client's wakeup
         information from sleepctl. */

      hdrsrchsleep.sleep_slots = ( (uint32) hdrsrchsleep.wake_slot ) - slot;
        /* Conflict calculation needs this setup aprior */
#ifndef FEATURE_HDR_DIME_MODEM
      /* Only calculated adjustment if duration is non-zero to avoid this
         adjustment from being performed twice.  When this function is called
         in start_sleep, then this adjustment is calculated later. */
      if ( ( HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT == TRUE ) &&
           ( sleep_duration_threshold > 0 ) )
      {

        adjusted_sleep_time =
          sleepctl_get_cpu_conflict_adjust( SLEEPCTL_HDR,
                                            SLEEPCTL_1X,
          ( hdrsrchsleep.sleep_slots << SYSCLK_PER_SLOT_SHIFT ),
            hdrsrchsleep_adjust_wakeup_for_cpu_conflict ) >>
          SYSCLK_PER_SLOT_SHIFT;
          /* Calculate the new sleep time due to CPU conflict, if any.
             Pass callback function to do the conflict calculation of the new
             sleep time. */

        slots_adj_for_cpu_conflict = hdrsrchsleep.sleep_slots -
                                     adjusted_sleep_time;

        /* Calculate the adjustment (backwards), if any, due to CPU conflict. */
        HDR_MSG_SRCH_6( MSG_LEGACY_MED,
                        "Compute sleep: intra_scc wake_slot = %d, sleep_slots = %d, curr slot=%d, dura=%d, adj= %d, adj_sleep_time=%d",
                        hdrsrchsleep.wake_slot,
                        hdrsrchsleep.sleep_slots,
                        slot,
                        sleep_duration_threshold,
                        slots_adj_for_cpu_conflict,
                        adjusted_sleep_time );

      }
#endif /* ! FEATURE_HDR_DIME_MODEM */

      /* Slide wake_slot earlier by the conflict adjustment, if any (HDR is
         always moved earlier) and make sure this new wake time is after the
         current slot plus the sleep duration threshold. */
      if ( ( hdrsrchsleep.wake_slot - slots_adj_for_cpu_conflict ) >
           ( slot + sleep_duration_threshold ))
      {
        /* There is enough time to sleep. */

        return hdrsrchsleep.wake_slot - slots_adj_for_cpu_conflict - slot;
          /* Return sleep time in slots taking conflict adjustment into
             account */
      }
      else
      {
        HDR_MSG_SRCH_5( MSG_LEGACY_LOW,
                        "Not ENOUGH time to sleep: intra_scc wake_slot = %d, curr slot=%d, dura=%d, adj= %d, adj_sleeptime=%d",
                        hdrsrchsleep.wake_slot,
                        slot,
                        sleep_duration_threshold,
                        slots_adj_for_cpu_conflict,
                        adjusted_sleep_time );
          /* REVISIT: Debug only */
        return 0;
        /* Not enough time to sleep */
      }
    }
    else
    {
      HDR_MSG_SRCH_4( MSG_LEGACY_LOW,
                      "Not ENOUGH time to sleep: intra_scc wake_slot = %d, curr slot=%d, dura=%d, adj= %d",
                      hdrsrchsleep.wake_slot,
                      slot,
                      sleep_duration_threshold,
                      slots_adj_for_cpu_conflict);
        /* REVISIT: Debug only */
      return 0;
        /* Not enough time to sleep */
    }

  } /* Intra-SCC Sleep compute */
  else
  {
    /* Check if we can sleep for at least the minimum sleep time ... */

    if ( hdrsrchsleep.wake_slot >= slot + sleep_duration_threshold )
    {
      return hdrsrchsleep.wake_slot - slot;
        /* Return the sleep time, in slots. */
    }
    else
    {
      return 0;
        /* Can't sleep - insufficent time */
    }

  } /* else not Intra-SCC Sleep */

} /* hdrsrchsleep_compute_sleep_time( ) */


#ifdef FEATURE_HDR_DIME_MODEM

/*===========================================================================

FUNCTION HDRSRCHSLEEP_WAKEUP_NOTIFY

DESCRIPTION
  SLPC calls it to notify wakeup time change of any tech. We only cares about
  1x wakeup and will send a command to ourself to process it later.
  we will discard the nofications of other techs. 

DEPENDENCIES
  None.

PARAMETERS
  
  
RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchsleep_wakeup_notify
( 
  slpc_id_type                       slpc_id, 
    /* slpc id -- 1x, HDR, etc */             

  uint64                             wakeup_tick, 
    /* absolute wakeup point in XO units */

  boolean                            update, 
    /* whether this is an update to a previously set wakeup point */

  boolean extension
    /* whether new wakeup is later than old */
)
{

  hdrsrch_cmd_type                      *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( slpc_id == SLPC_1X )
  {

    HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                    "slpc wakeup notification for 1x, new wakeup hi %x lo %x", 
                    (uint32)( wakeup_tick>>32 ),
                    (uint32)( wakeup_tick&0xFFFFFFFF) );
    cmd = hdrsrchcmd_new( HDRSRCH_WAKEUP_ADJUST_CMD );
      /* Command HDRSRCH_TASK to change sleep timeline */

    if ( cmd != NULL )
    {
      cmd->params.wakeup_adjust.conflict_wakeup_tick = wakeup_tick;

      hdrsrchcmd_post( cmd );
        /* Post the command */
    }
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED," slpc wakeup notification for %d", slpc_id);
  }
} /* hdrsrchsleep_wakeup_notify */

/*===========================================================================

FUNCTION HDRSRCHSLEEP_WAKEUP_ADJUST

DESCRIPTION
  Adjust wakeup time based on 1x wakeup point.

DEPENDENCIES
  None.

PARAMETERS
  
  
RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchsleep_wakeup_conflict_adjust ( void )
{
  uint64                           left;
    /* The rightmost left point of both time invervals. */

  uint64                           right;
    /* The leftmost right point of both time invervals. */

  uint64                           conflict_wake;
    /* Conflicting client's wake time in XO ticks. */

  uint64                           conflict_end;
    /* Conflicting client's online interrupt time in XO ticks. */

  uint64                           hdr_wake;
    /* HDR wake time in  XO ticks. */

  uint64                           hdr_end;
    /* HDR ending int time in XO ticks. */

  uint64                           hdr_reacq_end;
    /* HDR SCC boundary time in XO ticks. */

  uint64                           xo_tick_adj = 0;
    /* Adjustment backwards in time in xo tickx. */

  uint32                           slots_adj = 0;
    /* Adjustment backwards in time in slots.  Default to no adjustment. */

  uint64                           now_xo_tick;
    /* Current time stamp in xo ticks. */

  uint64                           proposed_end_xo_tick;
    /* The proposed ending time in xo ticks. */

  uint64                           hdr_warmup_xo_tick;
    /* HDR RF warmup time between the two interrupts. */

  uint64                           xo_tick_tmp;
    /* temp variable to storre xo tick */
  
  uint32                           warmup_1x;
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get conflict wakeup start and end */
  slpc_get_wakeup_and_ols_tick( SLPC_1X, &conflict_wake, &conflict_end );

  conflict_end += HDRSRCHSLEEP_CONFLICT_END_MARGIN_XO_TICK;
    /* If 1x has never set sleep time since power up, conflict_wakeup and conflict_end will
       be 0. It is still covered by the following algorithm.
     */

#ifdef FEATURE_HDR_BOLT_MODEM
  warmup_1x = srch_hdr_get_1x_warmup_ms();
  conflict_end += HDRSRCHSLEEP_MS_TO_XO_TICK( warmup_1x );
    /* For bolt slpc_get_ols_tick() returns the same value as get_wakeup_tick()
       since OLS is now removed. Get the warmup directly from 1x.
       This function will return 0 if 1x is awake (same as legacy) */
#endif /* FEATURE_HDR_BOLT_MODEM */

  /* Get HDR wakeup start and end */
  hdr_end = hdrsrchsleep.online_univ_stmr;
  hdr_wake = slpc_get_wakeup_tick( SLPC_HDR );
  hdr_reacq_end = hdr_end + 
     ( ( ( (uint64)hdrsrchsleep.reacq_slots ) << SYSCLK_PER_SLOT_SHIFT ) >> 8 )*500ULL;
    /* cx8 to ustmr conversion:  ustmr = cx8*19.2/(1.2288*8)
            ustmr = cx8 * 1000 / 512 = ( cx8 >> 8) * 500
       ustmr is in unit of xo tick 
    */
  
  if ( ( hdr_wake - conflict_wake ) <= HDRSRCHSLEEP_LOG_CONFLICT_TIME_PERIOD_XO_TICK )
  {
    INC_SAT( hdrsrchlog.sleep_info.num_conflict_within_time_period );
      /* Keep count of # HDR wakeups within time_period of 1x wakeups,
         time_period is set to 5ms for now */
  }

  /* Determine the rightmost left point and leftmost right point in time
     of the two intervals.
  */
  left = ( hdr_wake < conflict_wake )? conflict_wake : hdr_wake;

  right = ( hdr_reacq_end < conflict_end ) ? hdr_reacq_end : conflict_end;

  now_xo_tick = slpc_get_tick();
    /* Save the current time so that we make sure we aren't trying to set 
       the new wakeup point in the past */

  HDR_MSG_SRCH_7( MSG_LEGACY_MED, 
                  "Sleep adjust check: HW %u CW %u HE %u CE %u HR %u N %u warmup %u",
                  (uint32)hdr_wake, (uint32)conflict_wake, (uint32)hdr_end, (uint32)conflict_end, 
                  (uint32)hdr_reacq_end, (uint32)now_xo_tick, warmup_1x );
    /* we are able to print the low 32 bits here. All variables are uint64. */

  /* Make sure that the HDR wakeup time isn't already so close we can't adjust. */
  if ( ( (int32)( hdr_wake - now_xo_tick ) ) < 
                                          HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "Sleep adjust: Too close to wakeup to adjust %d < %d",
                    (uint32)( hdr_wake - now_xo_tick ),
                    HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK );
  }
  /* Check to see if the two intervals overlap.  Again, this relies on uint64
     math, so if the "right" most left point is less than the "left" most right
     point, then ( right - left ) will result in a really big number.  If there
     is overlap, the ( right - left ) result will be a small number, i.e., passes
     the check. */
  else if ( ( right - left ) <= MIN( ( hdr_reacq_end - hdr_wake ),
                                     ( conflict_end - conflict_wake ) ) )
  {
    xo_tick_adj = right - left;
      /* Determine the amount of time in sclks the reacq time should be
         adjusted. */

    /* Adjust both the wakeup and sleep ending time */
    hdr_wake -= xo_tick_adj;
    hdr_end -= xo_tick_adj;

    /* Now make sure there is no residual overlap during both RF warmup times.
       RF warmup should be completely disjoint so that we are sure the second
       interrupt will occur after all initialization is completed.*/

    /* Determine the rightmost left point and leftmost right point in time 
       after the adjustment. But this time look for overlap only during the RF
       warmup portion of the HDR time. */

    left = ( hdr_wake < conflict_wake )? conflict_wake : hdr_wake;

    right = ( hdr_end < conflict_end ) ? hdr_end : conflict_end;

    /* If the RF warmup times are still overlapping */
    if ( ( right - left ) <= MIN ( ( hdr_end - hdr_wake ), 
                                   ( conflict_end - conflict_wake ) ) )
    {
      xo_tick_adj += ( hdr_end - conflict_wake );
        /* Move the HDR RF warmup time to end right before the conflicting
           RF warmup time. */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
          "Sleep adjust: RF Warmup CPU Conflict overlap of %d", (uint32)( right - left ) );
    }

    slots_adj = (uint32)( ( xo_tick_adj * 64 / 125 ) >> SYSCLK_PER_SLOT_SHIFT ) + 1; 
      /* Convert the sclk adjustment to slots and add 1 slot to account for 
         quantization.
         tstmr (cx8) = ustmr (xo tick) * ( 1.2288*8)/19.2
         tstmr (cx8) = ustmr (xo tick) * 256 /500
         tstmr (cx8) = ustmr (xo tick) * 64 /125
      */

    /* Check whether adjusted wakeup time is on the frame boundary. 
       If yes, increment slots_adj to avoid the frame boundary */
    if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 )
    {
      /* Adjusted wakeup time is on frame boundary */

      slots_adj++;
        /* Increment */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
       "Adjusted wakeup time was on frame boundary, incremented slots_adj to %d",
                      slots_adj );

    }

    /* Don't adjust the sleep time by more than the current total time. */
    if ( hdrsrchsleep.sleep_slots <= slots_adj )
    {
      proposed_end_xo_tick = slpc_get_tick();
        /* Propose an end time that is as early as possible. */
    }
    else
    {
      xo_tick_tmp = ( slots_adj << SYSCLK_PER_SLOT_SHIFT ) * 125 /64;
        /* ustmr(xo tick) = tstmr(cx8) * 19.2/(1.2288*8)= tstmr(cx8)*125/64 */

      proposed_end_xo_tick = hdrsrchsleep.online_univ_stmr - xo_tick_tmp;
        /* determine the ending time in xo tick */
    }

    hdr_warmup_xo_tick = hdrsrchsleep.online_univ_stmr - slpc_get_wakeup_tick( SLPC_HDR );
      /* Determine the HDR RF warmup time. */

    /* If the proposed wakeup point is earlier than now, adjust the wakeup 
       point to be as early as we can */
    if ( ( (int32) ( proposed_end_xo_tick - hdr_warmup_xo_tick - now_xo_tick ) ) < 
                                          HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK )
    {
      xo_tick_adj = hdrsrchsleep.online_univ_stmr - hdr_warmup_xo_tick - now_xo_tick - 
                                           HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK;
        /* Change the adjustment to be immediately after now. */

      slots_adj = (uint32)( ( xo_tick_adj * 64 / 125 ) >> SYSCLK_PER_SLOT_SHIFT ); 
      /* Convert the sclk adjustment to slots and add 1 slot to account for 
         quantization.
         tstmr (cx8) = ustmr (xo tick) * ( 1.2288*8)/19.2
         tstmr (cx8) = ustmr (xo tick) * 256 /500
         tstmr (cx8) = ustmr (xo tick) * 64 /125
      */

      HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                "Sleep adjust: Can't adjust earlier than now: %d xo_tick %d slots",
                 (uint32)xo_tick_adj, slots_adj );

      /* Check whether the adjusted wakeup time is on frame boundary.
         If yes, decrment slots_adj since we at now */

      if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 )
      {
        /* Adjusted wakeup time is on frame boundary */

        if ( slots_adj != 0 ) 
        {
          slots_adj--;
            /* Increment */

          HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
        "Adjusted wakeup time was on frame boundary, decremented slots_adj to %d",
                          slots_adj );

        }
        else
        {
          /* This should not happen unless reacq_slots is 16 */
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                          "Wakeup on frame boundary but cannot adjust" );
        }
      }/* if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 ) */
    }/* if ( ( (int32) ( proposed_end_timetick - hdr_warmup_sclk - now_sclk ) ) */

    /* adjust the conflict */
    hdrsrchsleep.sleep_slots -= slots_adj;
      /* Save the new sleep duration in slots. */

    hdrsrchsleep.wake_slot -= slots_adj;
        /* Save the new wakeup time */

    hdrsrchsleep.reacq_slots += slots_adj;
        /* Save the reacq time in slots. */

    hdrsrchsleep.conflict_adjust_slots = slots_adj;
        /* Reset the conflict adjust slots. */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Sleep adjust: Total adjust %d xo tick, %d slots, update online time", 
                    xo_tick_adj, slots_adj );

    hdrsrchsleep_set_warmup_and_sleep_duration( );

    hdrsrchmcpm_extend_sleep();
      /* Update MCPM with new time */

    INC_SAT( hdrsrchlog.sleep_info.num_conflict_adjustment_triggered );
      /* Keep count of sleep adjustment occurrences */
    
  }
  else
  {
    HDR_MSG_SRCH_6( MSG_LEGACY_LOW,
                    "right(%d) - left (%d) <= MIN ((%d -%d), (%d - %d))",
                    (uint32)right, (uint32)left,
                    (uint32)hdr_reacq_end, (uint32)hdr_wake,
                    (uint32)conflict_end, (uint32)conflict_wake );
      /* REVISIT: DEBUG ONLY - the entire else and F3... */
  }

} /* hdrsrchsleep_wakeup_conflict_adjust */

#else /* FEATURE_HDR_DIME_MODEMD */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_WAKEUP_FOR_CPU_CONFLICT

DESCRIPTION
  This function returns a calculated sleep duration in chipx8s to account for a
  possible CPU conflict. It does this in several steps:

  1) It increases the HDR reacq time by the amount the two time intervals
     overlap (1x RF warmup and HDR RF warmup + reacq).
  2) It makes sure that the two RF warmup times don't overlap. It adjusts
     the HDR warmup period to be before the conflicting one if there is.
  3) It quantizes the HDR adjustment to slots and makes sure the adjustment
     doesn't make the wakeup prior to now.

  Explanation for the notation of the below code: 

  The right variable below indicates the earliest end point of the two
  intervals. The left variable indicates the latest wakeup point of the two
  intervals.  Thus, if subtracting the right value from the left value gives
  a very large value, we know that the two intervals overlap.

DEPENDENCIES
  None

PARAMETERS
  hdr_wi      - Current wakeup information for the client doing the adjusting
  conflict_wi - Current wakeup information for the conflicting client
  save_adjust_val - boolean on whether to save adjustment value or not to
                    hdrsrchsleep data structure

RETURN VALUE
  The new sleep duration in sys_clks.

SIDE EFFECTS
  None
 
===========================================================================*/

uint32 hdrsrchsleep_adjust_wakeup_for_cpu_conflict
(
  const sleepctl_wakeup_info_type *hdr_wi,
    /* Current wakeup information for the client doing the adjusting */

  const sleepctl_wakeup_info_type *conflict_wi,
    /* Current wakeup information for the conflicting client */

  boolean                         save_adjust_val
    /* Return adjustment value only.  TRUE means save adjust value in
       hdrsrchsleep. */
)
{
  uint32                           left;
    /* The rightmost left point of both time invervals. */

  uint32                           right;
    /* The leftmost right point of both time invervals. */

  uint32                           hdr_wake;
    /* HDR wake time in timeticks. */

  uint32                           conflict_wake;
    /* Conflicting client's wake time in timeticks. */

  uint32                           hdr_end;
    /* HDR ending int time in timeticks. */

  uint32                           hdr_reacq_end;
    /* HDR SCC boundary time in timeticks. */

  uint32                           conflict_end;
    /* Conflicting client's ending int time in timeticks. */

  uint32                           hdr_warmup_sclk;
    /* HDR RF warmup time between the two interrupts. */

  uint32                           sclk_adj = 0;
    /* Adjustment backwards in time in sclks. */

  uint16                           ending_fine;
    /* Residual time when converting from sys_clks to sclks. */

  uint32                           slots_adj = 0;
    /* Adjustment backwards in time in slots.  Default to no adjustment. */

  uint32                           now_sclk;
    /* Current time stamp in timeticks. */

  uint32                           proposed_end_sys;
    /* The proposed ending duration in sys clocks. */

  uint32                           proposed_end_timetick;
    /* The proposed ending time in timeticks. */

  uint32                           adjusted_sleep_time = 
                                     hdrsrchsleep.sleep_slots;
    /* Adjusted sleep time returned by this function.
       Initialize to current sleep slots. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start off assuming the wakeup points are unchanged. */
  hdr_wake = hdr_wi->wakeup_timetick;
  conflict_wake = conflict_wi->wakeup_timetick;

  if ( ( hdr_wake - conflict_wake ) <= HDRSRCHSLEEP_LOG_CONFLICT_TIME_PERIOD_SCLK )
  {
    INC_SAT( hdrsrchlog.sleep_info.num_conflict_within_time_period );
      /* Keep count of # HDR wakeups within time_period of 1x wakeups,
         time_period is set to 5ms for now */
  }

  /* Start off assuming the sleep ending points are unchanged. */
  hdr_end = hdr_wi->ending_timetick;
  conflict_end = conflict_wi->ending_timetick + 
                                       HDRSRCHSLEEP_CONFLICT_END_MARGIN_SCLK;

  /* Determine the point at which the HDR reacq should be completed. */
  hdr_reacq_end = sclk_from_sysclks( SCLK_HDR,
                                   ( hdrsrchsleep.reacq_slots 
                                     << SYSCLK_PER_SLOT_SHIFT ),
                                     &ending_fine ) + hdr_end;

  /* Determine the rightmost left point and leftmost right point in time
     of the two intervals.  Note: uint32 is used, so if a small number
     subtracts a larger number, the result is a "really big number". */
  left = ( hdr_wake - conflict_wake) < (conflict_wake - hdr_wake) ? 
                                               hdr_wake : conflict_wake;
  right = (hdr_reacq_end - conflict_end) < (conflict_end - hdr_reacq_end) ?
                                               conflict_end : hdr_reacq_end;

  now_sclk = timetick_get_safe();
    /* Save the current time so that we make sure we aren't trying to set 
       the new wakeup point in the past */

  HDR_MSG_SRCH_7( MSG_LEGACY_MED, 
                  "Sleep adjust check: HW %u CW %u HE %u CE %u HR %u N %u HS %u",
                  hdr_wake, conflict_wake, hdr_end, conflict_end, 
                  hdr_reacq_end, now_sclk, hdr_wi->starting_timetick );

  /* Make sure that the HDR wakeup time isn't already so close we can't adjust. */
  if ( ( (int32)( hdr_wake - now_sclk ) ) < 
                                          HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "Sleep adjust: Too close to wakeup to adjust %d < %d",
                    (int32)( hdr_wake - now_sclk ),
                    HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK );
  }
  /* Check to see if the two intervals overlap.  Again, this relies on uint32
     math, so if the "right" most left point is less than the "left" most right
     point, then ( right - left ) will result in a really big number.  If there
     is overlap, the ( right - left ) result will be a small number, i.e., passes
     the check. */
  else if ( ( right - left ) <= MIN( ( hdr_reacq_end - hdr_wake ),
                                     ( conflict_end - conflict_wake ) ) )
  {
    sclk_adj = right - left;
      /* Determine the amount of time in sclks the reacq time should be
         adjusted. */

    /* Adjust both the wakeup and sleep ending time */
    hdr_wake -= sclk_adj;
    hdr_end -= sclk_adj;

    /* Now make sure there is no residual overlap during both RF warmup times.
       RF warmup should be completely disjoint so that we are sure the second
       interrupt will occur after all initialization is completed.*/

    /* Determine the rightmost left point and leftmost right point in time 
       after the adjustment. But this time look for overlap only during the RF
       warmup portion of the HDR time. */
    left = ( hdr_wake - conflict_wake) < (conflict_wake - hdr_wake) ? 
                                                 hdr_wake : conflict_wake;
    right = (hdr_end - conflict_end) < (conflict_end - hdr_end) ?
                                                   conflict_end : hdr_end;

    /* If the RF warmup times are still overlapping */
    if ( ( right - left ) <= MIN ( ( hdr_end - hdr_wake ), 
                                   ( conflict_end - conflict_wake ) ) )
    {
      sclk_adj += ( hdr_end - conflict_wake );
        /* Move the HDR RF warmup time to end right before the conflicting
           RF warmup time. */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
          "Sleep adjust: RF Warmup CPU Conflict overlap of %d", right - left );
    }

    slots_adj = ( sclk_to_sysclks( SCLK_HDR, sclk_adj ) 
                                                >> SYSCLK_PER_SLOT_SHIFT ) + 1;
      /* Convert the sclk adjustment to slots and add 1 slot to account for 
         quantization. */

    /* Check whether adjusted wakeup time is on the frame boundary. 
       If yes, increment slots_adj to avoid the frame boundary */
    if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 )
    {
      /* Adjusted wakeup time is on frame boundary */

      slots_adj++;
        /* Increment */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
       "Adjusted wakeup time was on frame boundary, incremented slots_adj to %d",
                      slots_adj );

    }

    /* Don't adjust the sleep time by more than the current total time. */
    if ( hdrsrchsleep.sleep_slots <= slots_adj )
    {
      proposed_end_timetick = hdr_wi->starting_timetick;
        /* Propose an end time that is as early as possible. */
    }
    else
    {
      proposed_end_sys = ( hdrsrchsleep.sleep_slots - slots_adj ) 
                                                      << SYSCLK_PER_SLOT_SHIFT;
        /* Determine the proposed duration in chipx8s. */

      proposed_end_timetick = sclk_from_sysclks( SCLK_HDR,
                                                 proposed_end_sys,
                                                 &ending_fine) + 
                                                 hdr_wi->starting_timetick;
        /* Convert sys_clk's to sleep clocks to determine the ending time in 
           timeticks. */
    }

    hdr_warmup_sclk = hdr_wi->ending_timetick - hdr_wi->wakeup_timetick;
      /* Determine the HDR RF warmup time. */

    /* If the proposed wakeup point is earlier than now, adjust the wakeup 
       point to be as early as we can */
    if ( ( (int32) ( proposed_end_timetick - hdr_warmup_sclk - now_sclk ) ) < 
                                          HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK )
    {
      sclk_adj = hdr_wi->ending_timetick - hdr_warmup_sclk - now_sclk - 
                                           HDRSRCHSLEEP_ADJUSTMENT_MARGIN_SCLK;
        /* Change the adjustment to be immediately after now. */

      slots_adj = sclk_to_sysclks( SCLK_HDR, sclk_adj ) >> SYSCLK_PER_SLOT_SHIFT;
        /* Now convert to slots truncating the adjustment. */

      HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                "Sleep adjust: Can't adjust earlier than now: %d sclks %d slots",
                 sclk_adj, slots_adj );

      /* Check whether the adjusted wakeup time is on frame boundary.
         If yes, decrment slots_adj since we at now */
      if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 )
      {
        /* Adjusted wakeup time is on frame boundary */

        if ( slots_adj != 0 ) 
        {
          slots_adj--;
            /* Increment */

          HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
        "Adjusted wakeup time was on frame boundary, decremented slots_adj to %d",
                          slots_adj );

        }
        else
        {
          /* This should not happen unless reacq_slots is 16 */
          HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                          "Wakeup on frame boundary but cannot adjust" );
        }
      }/* if ( ( ( slots_adj + hdrsrchsleep.reacq_slots ) % HDRTS_SLOTS_PER_FRAME ) == 0 ) */
    }/* if ( ( (int32) ( proposed_end_timetick - hdr_warmup_sclk - now_sclk ) ) */

    /* Check if this function needs to save adjust value */
    if ( save_adjust_val == TRUE )
    {
      hdrsrchsleep.sleep_slots -= slots_adj;
        /* Save the new sleep duration in slots. */

      hdrsrchsleep.wake_slot -= slots_adj;
        /* Save the new wakeup time */

      adjusted_sleep_time = hdrsrchsleep.sleep_slots;
        /* Set return value. */

      hdrsrchsleep.reacq_slots += slots_adj;
        /* Save the reacq time in slots. */

      hdrsrchsleep.conflict_adjust_slots = slots_adj;
        /* Reset the conflict adjust slots. */
    }
    else
    {
      /* Do not save anything. */

      adjusted_sleep_time = hdrsrchsleep.sleep_slots - slots_adj;
        /* Calculate the adjusted sleep time to return. */
    }

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                    "Sleep adjust(%d): Total adjust %d sclks, %d slots", 
                    save_adjust_val, sclk_adj, slots_adj );

    INC_SAT( hdrsrchlog.sleep_info.num_conflict_adjustment_triggered );
      /* Keep count of sleep adjustment occurrences */

  }
  else
  {
    HDR_MSG_SRCH_6( MSG_LEGACY_LOW,
                    "right(%d) - left (%d) <= MIN ((%d -%d), (%d - %d))",
                    right, left,
                    hdr_reacq_end, hdr_wake,
                    conflict_end, conflict_wake );
      /* REVISIT: DEBUG ONLY - the entire else and F3... */
  }

  return ( adjusted_sleep_time << SYSCLK_PER_SLOT_SHIFT );

} /* hdrsrchsleep_adjust_wakeup_for_cpu_conflict( ) */
#endif /* FEATURE_HDR_DIME_MODEM */

#ifdef FEATURE_HDR_TO_LTE
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_LTEMEAS_STATE_VISIT_RETURN

DESCRIPTION
  This function is called to return to sleep state after a visit to the LTE
  Pilot measurement state to measure LTE neighbor frequencies.
 
  The next step is to reserve the TRM lock for the next DO wake up (see
  function hdrsrchsleep_start_sleep() as a reference.)
 
  As the last step, DO turns off the clocks similar to what it would do when
  it goes to sleep (see function hdrsrchsleep_start_sleep()
  as a reference.) 
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsleep_ltemeas_state_visit_return( void )
{
  uint32                          chips;
    /* Time, in chips.  Used to compute new TRM reservation */

  uint32                          slots;
    /* Time, in slots.  Used to compute new TRM reservation */

  uint32                          remaining_slots;

  hdrsrchrf_trm_freq_info_type    tune_info;
    /* Band/chan info for TRM reservation */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                "D2L debug hdrsrchsleep_ltemeas_state_visit_return() is called" );

  /* Update remaining slots to request TRM for next wake up.  Some time
     has been used to measure LTE frequencies since DO starts sleeping. */
#ifdef FEATURE_HDR_DIME_MODEM
  chips = (uint32) ( slpc_get_sysclk_count( SLPC_HDR ) >> 3 );
    /* How long we've slept for, in chip. slpc return is in cx8. */

#else /* FEATURE_HDR_DIME_MODEM */
  chips = sleepctl_get_chip_cnt( SLEEPCTL_HDR );
    /* Determine how long we slept for, in chips */
#endif /* FEATURE_HDR_DIME_MODEM */

  slots = (chips + (CHIPS_PER_SLOT/2)) >> CHIPS_PER_SLOT_SHIFT;
    /* And convert to slots */

  remaining_slots = hdrsrchsleep.sleep_slots - slots;
    /* Calculate the remaining slots from the original sleep_slots */

  /* Turn off CDMA clocks
     Reference hdrsrchsleep_start_sleep() */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Request MCPM to go to sleep" );

  hdrsrchmcpm_set_state( HDRSRCHMCPM_GO_TO_SLEEP_REQ );
    /* Tell MCPM to switch to sleep state clock configuration */

    /* Reserve TRM resource
       Reference hdrsrchsleep_start_sleep() */
  if ( remaining_slots > 2 )
  {

    if ( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] != NULL )
    {
      memset( &tune_info, 0, sizeof(tune_info) );

      tune_info.freq_info.band = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].
        aset[0]->chan.band;
      tune_info.freq_info.num_channels = 1;
      tune_info.channel_list[0] = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].
        aset[0]->chan.chan_num;
      /* Populate band/chan info for TRM interaction. */

      hdrsrchrf_set_trm_freq_info( &tune_info );
    }

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    hdrsrchsleep_reserve_trm_at( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_RX_ANY,
                          hdrsrchsleep_compute_rf_reserve_time( ( remaining_slots - 2 ) ),
                          HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK );
      /* Request RF resource lock for the up coming wakeup time.  Note that as a
         side effect both the primary and diversity receiver will be released by this call. 
         We subtract 2 slots to make sure that reserve time is before wakeup time. */
#else
    hdrsrchrf_reserve_at( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_RX_ANY,
                          hdrsrchsleep_compute_rf_reserve_time( ( remaining_slots - 2 ) ),
                          HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK, HDRSRCHSLEEP_RF_REASON );
      /* Request RF resource lock for the up coming wakeup time.  Note that as a
         side effect both the primary and diversity receiver will be released by this call. 
         We subtract 2 slots to make sure that reserve time is before wakeup time. */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "D2L Reserve TRM,OrigSlots=%d,LTESlots=%d,Remain=%d",
                    hdrsrchsleep.sleep_slots,
                    slots,
                    remaining_slots );
  }
  else
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR, "D2L Hold TRM,OrigSlots=%d,LTESlots=%d,Remain=%d",
                    hdrsrchsleep.sleep_slots,
                    slots,
                    remaining_slots );
  }

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_START_SLEEP );
    /* output powr profile strobe if feature is enable */


#ifdef FEATURE_NPA
   ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup. */
#endif /* FEATURE_NPA */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L HDR entered sleep and clock is off");
  hdrsrchsleep.sleep_sub_state = HDRSRCH_NOT_IN_SLEEP_PREPROCESSING;

} /* hdrsrchsleep_ltemeas_state_visit_return( ) */

/* EJECT */

/*===========================================================================

FUNCTION HDRSRCHSLEEP_LTEMEAS_STATE_VISIT_REQUIRED

DESCRIPTION
  This function is called to determine if DO should do LTE Pilot measurements
  during the start of this sleep cycle. The procedure is as followed:
 
  - Procedure executes while DO is in sleep state at every SCI cycle.
  - At the start of sleep state, DO decides to request an LTE measurement.
  - LTE tunes to the desired frequency and does the measurement.
  - DO runs the reselection criteria on the obtained LTE measurement.
  - If decision is made to reselect to LTE, LTE module tries to acquire on the chosen
    frequency.
  - If LTE measurement does not pass reselection criteria, DO performs measurement
    on the next LTE frequency in list.
  - Continue until list is exhausted without interfering with normal DO wake up.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  True if it's time to do neighbor LTE Pilot measurement

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchsleep_ltemeas_state_visit_required( void )
{

  boolean                         do_ltemeas = FALSE; 
    /* Return value */

  uint32                          avail_time_in_ms = 0;
    /* Length of time available */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( !hdrsrchrftxd_suspend_asdiv( TRUE, TRM_ASDIV_STATE_UPDATE_REASON_IRAT ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L AS Div cannot be suspended, do not measure LTE" );
    return do_ltemeas;
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  avail_time_in_ms =
    ( SLOTS_TO_MS( hdrsrchsleep.sleep_slots - HDRSRCHSLEEP_MINIMUM_SLOTS ) );
    /* Calculate how much time DO has for LTE measurements before its next
       wake up */
   
  do_ltemeas = hdrsrchltemeas_state_visit_required( avail_time_in_ms );
    /* Is LTE measurement required this sleep cycle? */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( do_ltemeas == FALSE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L No LTE meas, set SUSPEND ASDIV to FALSE" );
    /* At this point, ASDIV has been disabled for D2L.
       Since we do not perform D2L, immediately reenable ASDIV.
      (If we do perform D2L, ASDIV is re-enabled after D2L is done.) */
    
    (void) hdrsrchrftxd_suspend_asdiv( FALSE, TRM_ASDIV_STATE_UPDATE_REASON_IRAT );
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  return do_ltemeas;

} /* hdrsrchsleep_ltemeas_state_visit_required( ) */
#endif /* FEATURE_HDR_TO_LTE */

/*===========================================================================

FUNCTION HDRSRCHSLEEP_START_SLEEP

DESCRIPTION
  This function begins sleep

DEPENDENCIES
  Must only be called when searcher is idle.
  Ie, should be called from hdrsrch_on_searcher_idle( )
  hdrsrchidle_compute_next_cycle( ) must be called prior to this function.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_start_sleep( void )
{
  uint16                          hstr;
    /* Half-slot timing reference when sleep starts */

  uint16                          sleep_duration_slots;
    /* Sleep duration recalculated just before sleep in slots */  

  uint16                          fast_sleep_duration_slots;
    /* Fast sleep duration calculated */  

  int16                           sleep_slot_delta = 0;
    /* The difference between fast wakeup sleep time and the current sleep time */

#ifdef FEATURE_HDR_TO_LTE
  boolean                         do_lte_meas = FALSE;
#endif /* FEATURE_HDR_TO_LTE */

  hdrsrchrf_trm_freq_info_type    tune_info;
    /* Band/chan info for TRM reservation */

  uint32                          slot_at_compute_sleep_time;
    /* slot number when compute_sleep_time() is called */

#ifdef FEATURE_HDR_RUMI
  qword                           cycle_before_start;
  uint32                          slot_before_start;
#endif /* FEATURE_HDR_RUMI */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                "D2L hdrsrchsleep_start_sleep() is called" );
  hdrsrchsleep.sleep_sub_state = HDRSRCH_IN_SLEEP_PREPROCESSING;

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  /* Record coex power to report on next wakeup */
  hdrmultirat_report_coex_power( TRUE );
    /* Save only -> dont report to MCS, we will report on wakeup */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  /* Disable on-arrival delivery of RX packets before entering sleep. */
  hdrdec_disable_on_arrival_delivery();

  hdrsrchutil_dec_deactivate( );
    /* Disable HDR decoder */

  hdrsrchrf_prep_to_sleep();
    /* Prepare the RF for sleep. The AGC is stopped and the 
       DC offsets are saved. */

#ifdef FEATURE_HDR_TO_LTE
  /* We only want to do LTE pilot measurements if all the conditions below are
     satisfied.
  */
  if ( hdrsrchsleep_get_intra_scc_valid() == TRUE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L Intra SCC, do not measure LTE" );
  }
  else if ( hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_GRANTED_BEFORE_TIMEOUT )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L RF was just granted before timeout, do not measure LTE" );
  }
  /* 
     *************** Important note ******************
     Please read.  The order of these checks matters.  
     If more conditions need to be added, they need to be added BEFORE
     hdrsrchsleep_ltemeas_state_visit_required() is called.
     The call to hdrsrchsleep_ltemeas_state_visit_required() must be the last
     call in the series of check.  Once hdrsrchsleep_ltemeas_state_visit_required()
     is called, DO must enter LTE meas state so it can end the LTE meas
     transaction gracefully with other technologies (TRM/IRATMan/LTE) when leaving
     LTE meas state.  This is due to new TRM design for DR DSDS.
     *************** Important note ******************
  */
  else
  { 
    do_lte_meas = hdrsrchsleep_ltemeas_state_visit_required();

    if ( do_lte_meas == TRUE )
    {
      hdrsrchltemeas_init_meas_state();
        /* DO is taking LTE pilot measurements this sleep cycle. TRM lock will be released 
             after leaving LTE Pilot measurement state, as the TRM lock is required
             for this operation. */
    }
  }
#endif /* FEATURE_HDR_TO_LTE */

  REX_DISABLE_PREMPTION();


  /* Change state here, once sleep controller is armed we should
     complete sleep processing without interference */
  hdrsrchsleep.sleep_sub_state = HDRSRCH_SLEEP_CTL_ARMED;

#ifdef FEATURE_HDR_DIME_MODEM
  hdrsrchsleep_avoid_slot_boundary();
   /* Avoid slot boundary when calculating sleep duration */

#ifdef FEATURE_HDR_RUMI
  hdrts_get_cycle_and_slot( cycle_before_start, 
                            &slot_before_start, HDRTS_SLOTS_PER_CCC );
    /* capture cycle/slot before slpc start and compare with those
       at compute_sleep_time */
#endif  /* FEATURE_HDR_RUMI */

  hdrsrchsleep.slpc_start_t_stmr = slpc_start( SLPC_HDR );

#ifdef FEATURE_HDR_BOLT_MODEM
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "slpc_start raw:0x%x%08x, cx8:0x%x", 
                  I64_HI(hdrsrchsleep.slpc_start_t_stmr.time),
                  I64_LO(hdrsrchsleep.slpc_start_t_stmr.time),
                  I64_LO(hdrsrchsleep.slpc_start_t_stmr.time >> 32) );

  hdrsrchsleep.slpc_start_t_stmr.time = 
    (hdrsrchsleep.slpc_start_t_stmr.time >> 32) & HDR_PN_CIRC_MASK_X8;
    /* for bolt slpc returns vstmr_t format which is cx8 << 32 */
#else
  hdrsrchsleep.slpc_start_t_stmr.time = 
          (hdrsrchsleep.slpc_start_t_stmr.time >> 2) & HDR_PN_CIRC_MASK_X8;
    /* slpc dumps a snapshot of hdr_stmr and univ_stmr. Sleep duration should be
       counted from this snapshot. The returned value from slpc_start() in in cx32.
       We need to covert it to cx8. In addition, we only care about the hdr rtc value
       within one frame. 
    */
#endif /* FEATURE_HDR_BOLT_MODEM */

  hdrts_sleep( );
    /* Let Time Services know the mobile is going to sleep */
#else
  hdrts_sleep( );
    /* Let Time Services know the mobile is going to sleep */
  sleepctl_arm(SLEEPCTL_HDR);
    /* Arm the sleep controller */

  sleepctl_block_until_sleeping(SLEEPCTL_HDR);

#endif /* FEATURE_HDR_DIME_MODEM */

#ifdef FEATURE_HDR_RUMI
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "slpc_start prev slot %d",
                  slot_before_start );
#endif /* FEATURE_HDR_RUMI */

  sleep_duration_slots = hdrsrchsleep_compute_sleep_time( 0, &slot_at_compute_sleep_time );
    /* Recompute the sleep duration, We are already committed 
       to sleep, so use zero as sleep duration threshold. Note the
       sleep time computation uses the current time to calculate the
       sleep duration. The intlock is used to make sure no extra
       delay happens b/w the arming and the duration computation. 
       If there is insufficient time to sleep, the return value will be 0.
    */

  fast_sleep_duration_slots = hdrsrchsleep_compute_fast_sleep_time_slots( 0 );
    /* Calculate fast sleep duration to use as duration if sleep timeline was
       delayed, e.g., another task hogged CPU after entering sleep processing. */

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                  "Sleep check: sleep time %d fast_wakeup_time %d "
                  "minimum time %d, curr_slot %d", sleep_duration_slots,
                  fast_sleep_duration_slots,
                  HDRSRCHSLEEP_MINIMUM_SLOTS,slot_at_compute_sleep_time );

  /* Check if minimum sleep (rf warmup + padding) is possible*/
  if ( ( sleep_duration_slots < HDRSRCHSLEEP_MINIMUM_SLOTS ) &&
       ( fast_sleep_duration_slots > sleep_duration_slots ) )
  {
    /* Sleep timeline might have been delayed by another task.  Use
       fastest wakeup from now possible which actually pushes wakeup
       further out to avoid setting sleepctl wakeup in the past. */

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Old sleep slots %d, wake slot %d",
                    hdrsrchsleep.sleep_slots,
                    hdrsrchsleep.wake_slot );

    sleep_slot_delta = fast_sleep_duration_slots - sleep_duration_slots;
      /* Get the delta */

    hdrsrchsleep.wake_slot += sleep_slot_delta;
      /* Update wake slot */

    sleep_duration_slots = fast_sleep_duration_slots;
      /* Change to new fast wakeup sleep duration to avoid wakeup times in the
         past but at least wakeup ASAP. */


    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                    "Set fast wakeup %d slots from now.",
                    fast_sleep_duration_slots);

  }

  if ( sleep_duration_slots != 0 )
  {
    hdrsrchsleep.sleep_slots = sleep_duration_slots;
      /* Update the sleep duration if new value is non-zero */

    hdrsrchsleep.conflict_adjust_slots = 0;
      /* Reset the conflict adjust slots for each normal sleep. */
  }

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "SleepTime=%d Bed=%d Wake=%d",
                  hdrsrchsleep.sleep_slots,
                  hdrsrchsleep.wake_slot - hdrsrchsleep.sleep_slots,
                  hdrsrchsleep.wake_slot );

  /* Log NV sleep optimization parameters */
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "SLEEP OPT: Strobes= %d, FPD= %d, Reacq time = %d slots", 
                  HDRSRCHSLEEP_POWER_STROBE_NV_ENABLE,
                  HDRSRCH_FORCED_PREAMBLE_DETECT_NV_ENABLE,
                  hdrsrchsleep.reacq_slots );

#ifdef FEATURE_HDR_DIME_MODEM

  if ( hdrsrchsleep_slpc_start_in_same_slot ( slot_at_compute_sleep_time ) == TRUE )
  {
     hdrsrchsleep.sleep_slots -= 1;
       /* sleep_slots should be the number of slots from next slot boundary after slpc_start() to
          the online slot boundary. However, currently hdrsrchsleep.sleep_slots is calculated
          in hdrsrchsleep_compute_sleep_time(). It uses the current slot to do the calculation.
          That's why we need to adjust 1 slot.
          There is a very small chance that slpc_start() is called right before a slot
          boundary, and hdrsrchsleep_compute_sleep_time() is called in next slot. In this
          case we don't need to do the adjustment.
          The reason for the adjustment is to keep the behavior the same as legacy target
          (those before Nikel). In legacy target hdrsrchsleep.sleep_slots is duration from
          the next slot boudary to sleep end interrupt. sleepctl_block_until_sleeping() has
          guranteed it, since it is a blocking call till the next slot boundary. hdrts_sleep() 
          and hdrts_wakeup() assume that when calculating frame number from sleep_slots.
          There is also a very small possibility that hdrts_sleep() and slpc_start()
          are called in two different slots. If it happens and online interrupt ( or
          sleep end interrupt in old target) fires at frame boundary, the frame number 
          calclulated in hdrts_wakeup() would be off by one. We need to avoid online interrupt
          fires at frame boundary. This should be take care of in fast wakeup and 
          sleep conflict adjustment. 
       */
  }

  hdrsrchsleep_set_warmup_and_sleep_duration( );
  
  hdrsrchsleep_wakeup_conflict_adjust();
    /* We know HDR online interrupt stmr value now. We can check if there is any conflict */

  slpc_set_notify_callback( hdrsrchsleep_wakeup_notify );
    /* Set slpc sleep time notify callback. If 1x changes its wakeup point later, we will
       adjust accordingly
     */
    
#else /* FEATURE_HDR_DIME_MODEM */
  if ( HDRSRCHSLEEP_DO_CONFLICT_ADJUSTMENT == TRUE )
  {
    sleepctl_set_cpu_conflict_adjustment_callback( SLEEPCTL_HDR, SLEEPCTL_1X, 
                                 hdrsrchsleep_adjust_wakeup_for_cpu_conflict );
  }

  sleepctl_set_duration( SLEEPCTL_HDR,
                         hdrsrchsleep.sleep_slots << CHIPS_PER_SLOT_SHIFT );
    /* Program sleep controller to go to sleep for given duration */
#endif /* FEATURE_HDR_DIME_MODEM */

  REX_ENABLE_PREMPTION();

  hdrsrchdrv_disable_srch4( );
    /* Disable SRCH4 for HDR */

  hdrsrchafc_stop_fll_tracking();
    /* TCXO control is no longer needed, so let TCXO Mgr know */

  hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
    /* Disable rx */

  hstr = HDRHSTR_GET_COUNT();
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Ready to sleep hstr=%d (%d)",
                  hstr & 511, hstr );

  hdrsrchsleep.hstr_at_sleep = hstr;
    /* Save hstr count */

#ifdef FEATURE_HDR_DIME_MODEM
  hdrsrchsleep_dump_time();
#endif /* FEATURE_HDR_DIME_MODEM */

  hdrts_save_mstr( HDRMDSP_GET_MSTR_RTC_OFFSET_CX8() );
    /* Save modtime_ref */

  hdrmdsp_config_hdr_app( HDRFW_STATE_STANDBY, TRUE, FALSE );
    /* Disable the HDR MDSP Application */

  hdrmdsp_fws_suspend_app( );
    /* HDR vote to suspend FW */

  if( !hdrsrchsleep_get_intra_scc_valid() )
  {
#ifdef FEATURE_HDR_PBR_RANDOMIZATION
    hdrsrchsleep_pbr_trm_update( FALSE );
      /* successfully waked-up for the current page */
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */

    hdrsrchsleep.wakeup_id++;
    /* Increment wakeup id for next wakeup */
  }

#ifdef FEATURE_HDR_TO_LTE
  /* Disable DO clock regimes and release TRM lock (by reserving it for the next
     wake up) if it is not time to measure neighbor LTE frequencies.
   
     If it is time to do LTE pilot measurements, DO needs to hold on to TRM lock and keep DO
     clocks enabled (for RF.)  LTE pilot measurement procedure requires DO to do so.
   
     DO will do the rest of the shut down processing after LTE measurements
     are done (DO leaves LTE meas state - see function
     hdrsrchsleep_ltemeas_state_visit_return() for RF shutdown, clock shutdown,
     TRM duration update and reservation for next wake up.) */
  if( do_lte_meas == FALSE )
#endif /* FEATURE_HDR_TO_LTE */
  {
    hdrsrchmcpm_set_state( HDRSRCHMCPM_GO_TO_SLEEP_REQ );
      /* Tell MCPM to switch to sleep state clock configuration */

    if ( hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] != NULL )
    {
      memset( &tune_info, 0, sizeof(tune_info) );

      tune_info.freq_info.band = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].
        aset[0]->chan.band;
      tune_info.freq_info.num_channels = 1;
      tune_info.channel_list[0] = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].
        aset[0]->chan.chan_num;
      /* Populate band/chan info for TRM interaction. */

      hdrsrchrf_set_trm_freq_info( &tune_info );
    }
      
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
    hdrsrchsleep_reserve_trm_at( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_RX_ANY,
                          hdrsrchsleep_compute_rf_reserve_time( (hdrsrchsleep.sleep_slots - 2 ) ),
                          HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK );
    /* Request RF resource lock for this wakeup time.  Note that both the
       primary and diversity receiver will be released by this call. 
       We subtract 2 slots to make sure that reserve time is before wakeup 
       time */
#else
    hdrsrchrf_reserve_at( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_RX_ANY,
                          hdrsrchsleep_compute_rf_reserve_time( (hdrsrchsleep.sleep_slots - 2 ) ),
                          HDRSRCHSLEEP_MIN_WAKE_TIME_SCLK, HDRSRCHSLEEP_RF_REASON );
    /* Request RF resource lock for this wakeup time.  Note that both the
       primary and diversity receiver will be released by this call. 
       We subtract 2 slots to make sure that reserve time is before wakeup 
       time */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
    HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_START_SLEEP );
      /* output powr profile strobe if feature is enable */


#ifdef FEATURE_NPA
     ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
      /* Cancel CPU request for HDR wakeup. */
#endif /* FEATURE_NPA */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR entered sleep and clock is off");
    hdrsrchsleep.sleep_sub_state = HDRSRCH_NOT_IN_SLEEP_PREPROCESSING;
      
  }

} /* hdrsrchsleep_start_sleep( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_NSET_UPDATE_CMD

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

LOCAL void hdrsrchsleep_nset_update_cmd
(
  const hdrsrch_nset_update_params_type *params
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_ASSERT ( ( params->num_pilots <= HDRSRCH_NSET_MAX_SIZE ) 
  && ( params->num_channels <= HDRSRCH_HASH_CHAN_MAX_SIZE ) 
  && ( params->num_sects <= HDRSRCH_MAX_CACHE_RECORDS) );
    /* KW fix : Check to limit num_pilots to max */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "NSET_UPDATE_CMD Num=%d",
                  params->num_pilots );

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

} /* hdrsrchsleep_nset_update_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_DEACTIVATE_RF_WARMUP

DESCRIPTION
  Warmup interrupt during DEACTIVATE

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_deactivate_rf_warmup( void )
{

  uint32           time_to_rtc_on_us = 0;
    /* Time in sclk until to rtc on */

  uint64            time_slept_cx8;
    /* time already slept */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
    /* Tell MCPM to turn on clocks grouped under wake-up */

#ifdef FEATURE_HDR_DIME_MODEM
  if ( !slpc_complete_wakeup( SLPC_HDR, HDRSRCHSLEEP_MIN_RF_WARMUP_US,
                     &hdrsrchsleep.online_hdr_t_stmr ) )
#else
  if ( !sleepctl_sufficient_warmup_time( SLEEPCTL_HDR, 15 ) )
#endif /* FEATURE_HDR_DIME_MODEM */
  {
    hdrsrchmcpm_set_state( HDRSRCHMCPM_GO_TO_SLEEP_REQ );
    /* Start of RF warmup was delayed too much - skip wakeup */
    hdrsrchsleep_extend_sleep( HDRSRCHSLEEP_DEACTIVATE_EXTENSION );

    return;
  }
 
 hdrsrchsleep.sleeping = FALSE;
    /* We are waking up */

  slpc_deregister_notify_callback( hdrsrchsleep_wakeup_notify );
    /* Deregister wake up notify callback when deactivated during sleep as there is no need to adjust wakeup time */

#ifndef FEATURE_HDR_DIME_MODEM
  sleepctl_complete_wakeup( SLEEPCTL_HDR );
    /* Commit to this wakeup */
#endif /* !FEATURE_HDR_DIME_MODEM */


  /* RF Warmup */
  /* Calculate online time for rf_warmup_end processing */
  time_slept_cx8 = slpc_get_sysclk_count( SLPC_HDR );
    /* time that we have already slept, in cx8 */

  if (hdrsrchsleep.sleep_duration_cx8 > time_slept_cx8 )
  {
    time_to_rtc_on_us = hdrsrchsleep.sleep_duration_cx8 - time_slept_cx8;
      /* Get time to online interrupt. Now it is in cx8 */

    time_to_rtc_on_us = (time_to_rtc_on_us >> 3)*625/768;
      /* Convert it to micro second. 1 cdma chip = 1000/1228.8 us = 625/768 us */
  }
  else
  {
    time_to_rtc_on_us = 1000;
      /* Fire soon, seems we've already missed the boat. */

    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "RF warmup started after online time!");
  }

  rex_set_timer( &hdrsrchsleep.ols_timer, 
                 (time_to_rtc_on_us + 999 ) / 1000 );

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "set ols cb after %d + 999 / 1000 ms (exp %d)",
                 time_to_rtc_on_us, hdrsrchsleep.rf_warmup.cur_warmup_time_us );    


  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "RF Warmup (Deactivating)" );

} /* hdrsrchsleep_deactivate_rf_warmup( ) */

/*===========================================================================

FUNCTION HDRSRCHSLEEP_DEACTIVATE_RF_WARMUP_DONE

DESCRIPTION
  Wakeup (Sleep Ending) interrupt during DEACTIVATE

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Asserts RxChipx16 ON signal

===========================================================================*/

LOCAL void hdrsrchsleep_deactivate_rf_warmup_done( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrts_abort_sleep();
    /* Inform hdrts that sleep has been aborted */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "RF Warmup Done (Deactivating)" );

  HDRSRCH_SIG_SET( HDRSRCH_RXCHIPX16_SIG );
    /* Signal the task that RxChipx16 has been gated on */

} /* hdrsrchsleep_deactivate_rf_warmup_done( ) */


/*===========================================================================

FUNCTION HDRSRCHSLEEP_DEACTIVATE_RXCHIPX16_ON

DESCRIPTION
  Completes the Deactivate procedure once the sleep controller has stopped.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Causes transition to START state.
  Completes the command in progress

===========================================================================*/

void hdrsrchsleep_deactivate_rxchipx16_on( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "RxChipx16 On - Deactivating" );

  hdrsrch_complete_cmd();
    /* Deactivate command is now complete */

  hdrsrchst_init_inactive_state( );

} /* hdrsrchsleep_deactivate_rxchipx16_on( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_DEACTIVATE_CMD

DESCRIPTION
  Exit immediately to the deactivate state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Command status

SIDE EFFECTS
  Disables the HDR autokicker

===========================================================================*/

LOCAL errno_enum_type hdrsrchsleep_deactivate_cmd( void )
{
  errno_enum_type               status;
    /* Command return status */

  boolean                       sleep_state;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  REX_DISABLE_PREMPTION();
    /* An interrupt can move us to the ReAcq state.
       Prevent these interrupts from executing while we change tracks. */

  sleep_state = hdrsrchsleep.sleeping;

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  if ( ( sleep_state ) && 
       ( hdrsrchsleep_schedule_fast_wakeup( FALSE, HDRSRCHRF_OVHD ) == TRUE ) )
#else
  if ( ( sleep_state ) && 
       ( hdrsrchsleep_schedule_fast_wakeup( FALSE ) == TRUE ) )
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "DEACTIVATE_CMD" );

    hdrsrch_add_to_wait_mask( HDRSRCH_RXCHIPX16_SIG );

#ifdef FEATURE_HDR_DIME_MODEM
    slpc_set_wakeup_callback( SLPC_HDR, hdrsrchsleep_deactivate_rf_warmup );
#ifdef FEATURE_HDR_BOLT_MODEM
    hdrsrchsleep.ols_cb = hdrsrchsleep_deactivate_rf_warmup_done;  
#else
    slpc_set_ols_callback( SLPC_HDR, hdrsrchsleep_deactivate_rf_warmup_done );
#endif /* FEATURE_HDR_BOLT_MODEM */
#else
    sleepctl_set_wakeup_callback( SLEEPCTL_HDR,
                                  hdrsrchsleep_deactivate_rf_warmup );
    sleepctl_set_ending_callback( SLEEPCTL_HDR,
                                  hdrsrchsleep_deactivate_rf_warmup_done );
#endif /* FEATURE_HDR_DIME_MODEM */

    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_NOT_WAITING;
      /* Going to INACTIVE state so initialize rf_wait_state. */

    status = E_IN_PROGRESS;
      /* Deactivate is now in progress */
  }
  else
  {
    /* We've already started to wakeup; the RF warmup interrupt must have
       occurred between dispatching of the DEACTIVATE command, and the
       INTLOCK above. */

    status = E_AGAIN;
      /* Handle the DEACTIVATE command in REACQ state. */
  }

  REX_ENABLE_PREMPTION();
    /* Thank-you for waiting */

  return status;

} /* hdrsrchsleep_deactivate_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_CMD

DESCRIPTION
  This function dispatches commands to the appropriate command handler

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

LOCAL errno_enum_type hdrsrchsleep_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  /* We are either Awake, or we don't need to be for this command */

  switch ( cmd->name )
  {
#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BC_INFO_CMD:
    {
      if ( cmd->params.bc_info.bcmcs_status == BCMCS_DISABLED )
      {
        hdrsrchbc_set_bc_status( BCMCS_DISABLED );
          /* If BC is disabled during sleep, just set the status  */
      }
      else
      {
        /* BC is enabled during sleep, we need to wake up ASAP  */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
        (void) hdrsrchsleep_schedule_fast_wakeup( 
                TRUE, 
                HDRSRCHRF_BROADCAST_ACCESS );
          /* Encourage sleep controller to wake us up sooner than normal */
#else
        (void) hdrsrchsleep_schedule_fast_wakeup( TRUE );
          /* Encourage sleep controller to wake us up sooner than normal */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Command suspended cmd=%d", cmd->name );

        status = E_AGAIN;
          /* Retry this command when we wake up */
      }

      break;
    }

    case HDRSRCH_UPDATE_BOM_PERIOD_CMD:
    {
      if ( cmd->params.bom.period == 0 )
      {
        hdrsrchsleep_set_bcmcs_page_cycle( 0 );
          /* AT decides to stop monitoring BOM here ... */

        break;
      }

      /* else fall through ...  */
    }

#endif /* FEATURE_HDR_BCMCS */

    case HDRSRCH_IDLE_CMD:
    case HDRSRCH_CHANNEL_CHANGE_CMD:
    case HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD:
    case HDRSRCH_SLEEP_CMD:
    case HDRSRCH_ACCESS_CMD:
    case HDRSRCH_TC_ASSIGNMENT_CMD:
    case HDRSRCH_PILOT_MEAS_CMD:
    {
      /* These commands require that we wakeup before we process them.
         Delay execution of these commands, and wakeup early. */
        int32 time_to_next_wakeup = 0;

        time_to_next_wakeup = hdrsrchsleep_get_time_to_next_wakeup();
      if ( (  hdrsrchsleep.sleeping == TRUE ) &&
            ( time_to_next_wakeup > 
                   HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK ) )
      {
        if ( hdrsrchsleep_get_sleep_duration() > CHIPS_PER_2_SLOTS )
          {
#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
            (void) hdrsrchsleep_schedule_fast_wakeup( TRUE, 
                    hdrsrchsleep_get_trm_reason( cmd->name ) );
#else
      (void) hdrsrchsleep_schedule_fast_wakeup( TRUE );
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
          /* Schedule fast wakeup only after sleeping for atleast 2 slots */
        }
        else
        {
        hdrsrch_timed_wait_usecs( HDRSRCHSLEEP_FULL_SLOT_DELAY_USEC );
          /* Timed wait till HDR spends 1 slot in sleep 
                      and after 1 slot sleep call fast wakeup*/
        }
      }
      else
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                        "defer processing command: Too close to wakeup to adjust %d < %d",
                         time_to_next_wakeup,
                         HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK );
      }

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Command suspended cmd=%d", cmd->name );

      status = E_AGAIN;
        /* Retry this command when we wake up */
    } break;

    case HDRSRCH_DEACTIVATE_CMD:
    {
      int32 time_to_next_wakeup = 0;

      time_to_next_wakeup = hdrsrchsleep_get_time_to_next_wakeup();

      /* Do we have enough margin to execute deactivate_cmd? If not defer processing it
          to a later point. This was done to avoid race condition where normal wakeup
          processing will obstruct the deactivate cmd processing */
      if ( ( hdrsrchsleep.sleeping == TRUE ) &&
           ( time_to_next_wakeup > 
                   HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK ) )
      {
      status = hdrsrchsleep_deactivate_cmd( );
        /* Exit HDR gracefully */
      }
      else
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                        "Defer processing DEACTIVATE command: Too close to normal wakeup point %d < %d",
                         time_to_next_wakeup,
                         HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK );
        status = E_AGAIN;
          /* Retry this command when we wake up */
      }
    } break;

    case HDRSRCH_NSET_UPDATE_CMD:
    {
      hdrsrchsleep_nset_update_cmd( &cmd->params.nset_update );
        /* Update the N-SET */

    } break;

    
#ifdef FEATURE_HDR_DIME_MODEM
    case HDRSRCH_WAKEUP_ADJUST_CMD:
    {
      /* this command is only processed in sleep state */
      hdrsrchsleep_wakeup_conflict_adjust();
      break;
    }
#endif /* FEATURE_HDR_DIME_MODEM */

#ifdef FEATURE_HDR_WAKEUP_MANAGER
    case HDRSRCH_EXTEND_SLEEP_CMD:
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
              "RXD extend sleep cmd  %d",cmd->params.extend_sleep.num_extension_slots );

      /* this command is only processed in sleep state */
      hdrsrchsleep_extend_sleep( cmd->params.extend_sleep.num_extension_slots );

      break;
    }
#endif /* FEATURE_HDR_WAKEUP_MANAGER */

    default:
    {
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */
    }

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchsleep_cmd(cmd) */


/*===========================================================================

FUNCTION HDRSRCHSLEEP_SET_WARMUP_TIME

DESCRIPTION
  This function determines the warmup time based on RPM optimized timeline
  and set to sleepctl.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchsleep_set_warmup_time( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "RF warmup time RPM optimized timeline %d, cur timeline %d", 
                  hdrsrchsleep.rpm_warmup_time_is_optimized,
                  hdrsrchsleep.cur_warmup_time_is_optimized );

  hdrsrchsleep.cur_warmup_time_is_optimized = hdrsrchsleep.rpm_warmup_time_is_optimized;
    /* Update the twarmup time flag flag */

  hdrsrchsleep.rf_warmup.cur_warmup_time_us = HDRSRCHSLEEP_RF_WARMUP_US;
    /* Set the default warmup time.*/
#ifdef FEATURE_HDR_DIME_MODEM
   (void) slpc_set_warmup( SLPC_HDR, hdrsrchsleep.rf_warmup.cur_warmup_time_us );
#else
  sleepctl_set_warmup( SLEEPCTL_HDR,
                       hdrsrchsleep.rf_warmup.cur_warmup_time_us );
#endif /* FEATURE_HDR_DIME_MODEM */
  /* Set RF warmup time.
     Note: RF warmup time is set here since
           intra-scc sleep needs sleepctl to know this info ahead of time for
           any wakeup CPU conflict calculation now called when computing sleep
           time for Intra-SCC sleep.. */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "RF warmup time was set to %d, cur optimized timeline %d", 
                  hdrsrchsleep.rf_warmup.cur_warmup_time_us,
                  hdrsrchsleep.cur_warmup_time_is_optimized );

} /* hdrsrchsleep_set_warmup_time */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_INIT

DESCRIPTION
  Sleep State Initialization

DEPENDENCIES
  Idle on an HDR System

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  Shuts down the RF, disables clocks, starts the autokicker, etc.

===========================================================================*/

LOCAL void hdrsrchsleep_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_TO_LTE

  /* If we are coming back from the LTE Pilot measurement state (entered after
     sleep), then we are already sleeping.  Instead of starting to sleep, we
     need to continue sleeping and make sure we wake up at the right time. */
  if( last_state == HDRSRCH_LTE_MEAS_IDLE_STATE )
  {
    hdrsrch_default_init( last_state );
      /* Use default handling */

#ifdef FEATURE_HDR_SELECTIVE_TXD
    (void) hdrsrchrftxd_suspend_asdiv( FALSE, TRM_ASDIV_STATE_UPDATE_REASON_IRAT );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    hdrsrchsleep_ltemeas_state_visit_return();
      /* Set up to continue sleeping */
    return;
      /* All done, already executed the rest of the sleep initialization
         before the LTE Pilot measurement state. */
  }
#endif /* FEATURE_HDR_TO_LTE */

  HDRSRCH_PROFILE( SLP_INIT );
    /* sleep timeline profiling output if enabled */

  // Point of No Return; sleep is now

  // This should be done at least 1/2 slot (0.833ms) before Cx16 is disabled.
  // From this point, we are registering keypress handlers, setting
  // warmup callbacks, disabling demod, putting TS to sleep, and so on.
  // There is even a half-slot delay if the RF is owned.  We should be safe.
#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Sleep: ASET pn=%d e=%d ch=%d",
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng,
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan.chan_num );
#else
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Sleep: ASET pn=%d e=%d ch=%d",
                  hdrsrchset.aset[0]->pn_offset,
                  hdrsrchset.aset[0]->total_eng,
                  hdrsrchset.aset[0]->chan.chan_num );
#endif /* FEATURE_HDR_REVB */

  hdrsrch_kill_signal( HDRSRCH_TIME_VALID_SIG );
    /* Clear TimeValid processing */

  hdrsrch_default_init( last_state );
    /* Use default handling */

  hdrsrchsleep_set_warmup_time();
    /* determine warmup time */
#ifdef FEATURE_HDR_DIME_MODEM
  slpc_set_wakeup_callback( SLPC_HDR, hdrsrchsleep_rf_warmup );

#ifdef FEATURE_HDR_BOLT_MODEM
  if (!hdrsrchsleep.ols_timer_init)
  {
    hdrsrchsleep.ols_timer_init = TRUE;
    rex_def_timer_ex( &hdrsrchsleep.ols_timer, 
                      hdrsrchsleep_ols_handler, 0 );
  }
  hdrsrchsleep.ols_cb = hdrsrchsleep_rf_warmup_done;  
#else
  slpc_set_ols_callback( SLPC_HDR, hdrsrchsleep_rf_warmup_done );
#endif /* FEATURE_HDR_BOLT_MODEM */

#else /* FEATURE_HDR_DIME_MODEM */
  sleepctl_set_wakeup_callback( SLEEPCTL_HDR, hdrsrchsleep_rf_warmup );
  sleepctl_set_ending_callback( SLEEPCTL_HDR, hdrsrchsleep_rf_warmup_done );  
    /* Set RF warmup callbacks.  Note: RF warmup time is set in hdrsrchsleep_enter_sleep() */
#endif /* FEATURE_HDR_DIME_MODEM */
  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_SLEEP_INIT );

  hdrsrchsleep_start_sleep( );
    /* Attempt to start sleeping */

  hdrsrchsleep.sleeping = TRUE;
    /* Sleep has started */

  hdrsrchutil_start_stop_watch();
    /* start counting the sleep duration */

  HDRSRCH_PROFILE( SLEEP );
    /* sleep timeline profiling output if enabled */

} /* hdrsrchsleep_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_DONE

DESCRIPTION
  Sleep State Finalization

DEPENDENCIES
  Exiting Sleep state

PARAMETERS
  next_state  - State we are transitioning to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_done( hdrsrch_state_enum_type next_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_default_done( next_state );
    /* Use default handling */


  if ( next_state != HDRSRCH_REACQ_STATE )
  {
    hdrsrch_kill_signal( HDRSRCH_RXCHIPX16_SIG );

#ifdef FEATURE_HDR_BOLT_MODEM
    rex_clr_timer( &hdrsrchsleep.ols_timer );
#endif /* FEATURE_HDR_BOLT_MODEM */
  }

  /* TODO: Other finalizations... */

} /* hdrsrchsleep_done( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_PRI_CHAIN_EVENT

DESCRIPTION
  TRM PRI chain event handler, called if req&notify has called callback

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_pri_chain_event( void )
{ 
  if ( hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_WAITING )
  {
    hdrsrchsleep.rf_wait_state = HDRSRCHSLEEP_RF_GRANTED_BEFORE_TIMEOUT;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "HDR SLEEP POWER OPT (state %d): While within 60s RF Resource timer, RF is granted",
                    hdrsrchsleep.rf_wait_state );
            
  }

  hdrsrchsleep_fast_wakeup();
} /* hdrsrchsleep_pri_chain_event( ) */

/* EJECT */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

LOCAL hdrsrch_state_struct_type hdrsrchsleep_state =
{
  HDRSRCH_SLEEP_STATE,
  hdrsrchsleep_init,
  hdrsrchsleep_done,
  hdrsrchsleep_cmd,
  hdrsrch_default_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrchsleep_deactivate_rxchipx16_on,
  hdrsrchsleep_pri_chain_event,
  hdrsrch_process_int_cmd_default,
  hdrsrch_default_process_low_pri_rsp_ind
};


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_INIT_SLEEP_STATE

DESCRIPTION
  This function causes the state machine to begin sleep state processing.

DEPENDENCIES
  Idle state

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the sleep state

===========================================================================*/

void hdrsrchsleep_init_sleep_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SLEEPING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrch_change_state( &hdrsrchsleep_state );
    /* Switch to sleep state processing */

} /* hdrsrchsleep_init_sleep_state( ) */


/* EJECT */
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_SEND_MSG_UPDATE_CC_START

DESCRIPTION
  This function sends the message to the firmware to update CC start.

DEPENDENCIES
  None 

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_send_msg_update_cc_start( void )
{
  hdrsrchsleep_fw_msg_struct_type            fw_msg;
    /* Messages sent to firmware */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &fw_msg.cc_config_msg,
          0,
          sizeof( hdrfw_scc_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &fw_msg.cc_config_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SCC_CMD );
    /* Init the message header */

  fw_msg.cc_config_msg.ccStart = hdrsrchsleep.cc_start;
  fw_msg.cc_config_msg.ccStart_Valid = 1;
    /* Set cc start in the message payload */

  hdrsrchmsg_send_msg( &fw_msg.cc_config_msg.hdr,
                       sizeof( hdrfw_scc_msg_t ) );
    /* Msgr copies the message and hence ok to use local variable */

} /* hdrsrchsleep_send_msg_update_cc_start */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*==========================================================================

FUNCTION        HDRSRCHSLEEP_CHECK_IDLE_DIV_CONDITIONS

DESCRIPTION
  This function checks if conditions for turning on diversity in Idle
  state are true.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_check_idle_div_conditions ( void )
{

/*-----------------------------------------------------------------------*/

  hdrsrchrf_report_rx_power( );

  hdrsrch.idle_state_div.wakeup_info.prev_sleep_time = 
      hdrsrchsleep.until;
   /* How long are we sleeping for */

  /* If not intra scc sleep */
  if( !hdrsrchsleep_get_intra_scc_valid() )
  {
    hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg = 
        hdrsrchidle_get_prev_wakeup_psinr();

    if( hdrsrch.idle_state_div.wakeup_info.num_slots == 0 )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Idle Div: This Wakeup:"
                                  "Sync cc missed %d, prev_sinr NA Prev_sleep_time %d",
                  hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed,
                  hdrsrch.idle_state_div.wakeup_info.prev_sleep_time );
    }
    else
    {
  
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Idle Div: This Wakeup:"
                                  "Sync cc missed %d, prev_sinr %d, Prev_sleep_time %d",
                  hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed,
                  hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg, 
                  hdrsrch.idle_state_div.wakeup_info.prev_sleep_time );
    }
  
    /* Check if we needs to turn on diversity, if
       1. Sync cc was missed in prev wakeup
       2. PSINR of prev wakeup was greater then PSINR threshold
       3. If slept longer than sleep time threshold
       4. If this is intra scc wakeup and div was on before
       5. If we failed reacq the first time */
    
    if( hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed ||
        ( ( hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg < 
            hdrsrch.idle_state_div.psinr_thresh ) && 
          ( hdrsrch.idle_state_div.wakeup_info.num_slots != 0 ) ) ||
        ( hdrsrch.idle_state_div.wakeup_info.prev_sleep_time > 
          hdrsrch.idle_state_div.sci_thresh )||
        hdrsrch.idle_state_div.wakeup_info.reacq_failed )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: Idle conditions met,"
                                     "Will enable div next wakeup ");
  
      /* Tell reacq to enable div next time */
      hdrsrch.idle_state_div.wakeup_info.div_enabled = TRUE;
    
      /* For sleep logging */
      hdrsrchlog.sleep_info.rxdiv_enable = TRUE;

      hdrsrchlog.sleep_info.rxdiv_enable_reason = 0x0;
      if ( hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed )
      {
        hdrsrchlog.sleep_info.rxdiv_enable_reason = 
          hdrsrchlog.sleep_info.rxdiv_enable_reason | 
          HDRSRCHSLEEP_LOG_IDLE_DIV_MISSED_SYNC_CC;
      }
      if ( ( hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg < 
            hdrsrch.idle_state_div.psinr_thresh ) && 
          ( hdrsrch.idle_state_div.wakeup_info.num_slots != 0 ) )
      {
         hdrsrchlog.sleep_info.rxdiv_enable_reason = 
          hdrsrchlog.sleep_info.rxdiv_enable_reason | 
          HDRSRCHSLEEP_LOG_IDLE_DIV_PSINR_AVG;
      }
      if ( hdrsrch.idle_state_div.wakeup_info.prev_sleep_time > 
          hdrsrch.idle_state_div.sci_thresh )
      {
        hdrsrchlog.sleep_info.rxdiv_enable_reason = 
          hdrsrchlog.sleep_info.rxdiv_enable_reason | 
        HDRSRCHSLEEP_LOG_IDLE_DIV_PREV_SLEEP_TIME;
      }
      if ( hdrsrch.idle_state_div.wakeup_info.reacq_failed  )
      {
        hdrsrchlog.sleep_info.rxdiv_enable_reason = 
          hdrsrchlog.sleep_info.rxdiv_enable_reason | 
        HDRSRCHSLEEP_LOG_IDLE_DIV_REACQ_FAILED;
      }
    }
    else
    {
      hdrsrch.idle_state_div.wakeup_info.div_enabled = FALSE;

      /* For sleep logging */
      hdrsrchlog.sleep_info.rxdiv_enable = FALSE;
      hdrsrchlog.sleep_info.rxdiv_enable_reason = 0x0;
    }

    hdrsrchutil_reset_prev_wakeup_info();
     /* Reset all prev wakeup info */
  } 

} /* hdrsrchsleep_check_idle_div_conditions */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ENTER_SLEEP

DESCRIPTION
  This function will cause the HDR SRCH state machine to enter sleep. But,
  it might take a few detours before we actually arrive at the sleep state.
  If required, and sufficient time exists, it may:
    - start off-frequency searching,
  before it finally enters the sleep state.

DEPENDENCIES
  Must be called from IDLE state, and from any "detour" states.

RETURN VALUE
  Whether HDR will actually sleep based on the time to next wakeup.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchsleep_enter_sleep( void )
{
  uint16                          sleep_time;
    /* Time to sleep for, if sleep started now      */

  uint16                          cc_start_to_now_hs;
    /* Half slot from orginal cc_start to now.
       That is, not the new cc_start from MDSP.  */

  uint16                          cc_start_to_ccdec_hs;
    /* Time from original CC start to CC dec, in half slots */

  boolean                         will_sleep = TRUE;
    /* Will be returned to the caller to indicate whether HDR will
       actually sleep */

   uint32                          slot_at_compute_sleep_time;
    /* Slot number when computer_sleep_time() is called */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep_set_warmup_time();
    /* Deterime warmup time */

  HDR_MSG_SRCH_4( MSG_LEGACY_MED,
                 "Enough sleep time? WUTA=%d, reacq=%d, buffer=%d, min sleep=%d",
                 HDRSRCHSLEEP_CONVERT_US_TO_SLOTS(hdrsrchsleep.rf_warmup.cur_warmup_time_us),
                 hdrsrchsleep.reacq_slots,
                 HDRSRCHSLEEP_INTRASCC_BUFFER_SLOTS,
                 HDRSRCHSLEEP_MINIMUM_SLOTS);
    /* REVISIT: DEBUG ONLY */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  if( hdrsrch.idle_state_div.idle_div_enabled &&
      ( hdrsrch_state_id != HDRSRCH_OFS_IDLE_STATE ) )
  {
    hdrsrchsleep_check_idle_div_conditions();

     /* If div on for next wakeup backup timeline */
    if( hdrsrch.idle_state_div.wakeup_info.div_enabled )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Idle Div: "
                                      "wake_slot %d reacq_slot %d",
                     hdrsrchsleep.wake_slot,
                     hdrsrchsleep.reacq_slots );

      hdrsrchsleep.wake_slot -= HDRSRCHUTIL_SLOTS_TO_ENABLE_DIV_FOR_REACQ;
        /* Adjust the wake up to account for the number of slots required
           to reacq with div on. */

      hdrsrchsleep.reacq_slots += HDRSRCHUTIL_SLOTS_TO_ENABLE_DIV_FOR_REACQ;

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Idle Div: adjusting sleep to enable div for reacq"
                                       "wake_slot %d reacq_slot %d",
                      hdrsrchsleep.wake_slot,
                      hdrsrchsleep.reacq_slots );
    }
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  sleep_time = hdrsrchsleep_compute_sleep_time( HDRSRCHSLEEP_MINIMUM_SLOTS, &slot_at_compute_sleep_time );
    /* Compute how long we could sleep for, if sleep started now */

  if ( ( sleep_time == 0 ) && 
       ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE  ||
           hdrsrch_state_id == HDRSRCH_SUSPENDED_IDLE_STATE ) ) )
  {
    /* Insuffient time exists for sleep!  If we are in Idle or Idle suspend state,
       do not sleep at all. */

    HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "Zero sleep time in Idle or Idle suspend state, do not sleep" );

    will_sleep = FALSE;
      /* Let the caller know that HDR won't actually sleep. */

#ifdef FEATURE_HDR_REVA_L1
    hdrsrchsleep_send_msg_update_cc_start( );
      /* Set the offset of next CC preamble  */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "CC_Start = %d", hdrsrchsleep.cc_start );
      /* Print out the CC that we are using for this wakeup. */
#endif /* FEATURE_HDR_REVA_L1  */

    hdrsrchreacq_wakeup_report( NULL );
      /* It was not ok to sleep - pretend we did, and have now woke up. */
  }
  else
  {

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "SleepTime=%d Bed=%d Wake=%d",
                    sleep_time,
                    hdrsrchsleep.wake_slot - sleep_time,
                    hdrsrchsleep.wake_slot );

    /* There are 2 possibilities here:
       1.  There is enough time to sleep -- perhaps enough time to do a few
           other things, such as off-frequency searching
       2.  There is zero time to sleep after coming back from OFS state --
           perhaps OFS search took too long, go to sleep as planned and wake
           up immediately.  Function hdrsrchsleep_start_sleep() handles sleeping
           with zero sleep time. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV

    if( hdrsrch.idle_state_div.idle_div_enabled )
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: turning off div in Idle/Access mode--> sleep");
  
      hdrsrchrf_report_rx_power( );
  
      hdrsrchsleep_deactivate_scc_decode_ind();
        /* Deactivate decode m slot indication */
  
      hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                        NULL );
        /* Deregister for callback() for mdsp logging */ 
  
      hdrsrchrfdiv_diversity_ctrl( FALSE );
        /* Disable diversity when we go to sleep. */
    }

#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

    switch ( hdrsrch_state_id )
    {
      case HDRSRCH_IDLE_STATE:
      {
        hdrsrchafc_rotator_push();
          /* Push rotator samples */

        hdrsrchafc_update_rgs();
         /* Report RGS */

        hdrsrchutil_cancel_fpd( );

        hdrsrchutil_update_data_ind_sinr( );
          /* Update the HDR data indicator SINR one last time before deassigning fingers */

        hdrsrchfing_deassign_all_fings( );
          /* Unassign fingers */

        hdrsrchutil_dec_deactivate( );
          /* Deactivate HDR decoder to cancel all active packets */

        hdrsrchrf_pdm_capture_values();
          /* Capture PDM values for fast RF warmup */

        hdrsrchafc_stop_fll_tracking();
          /* Disable FLL tracking and save the FLL accum value. */

        hdrsrchlog.sleep_info.ofs_status  = HDRLOG_OFS_NO_OFS;
          /* Make NO_OFS as default ofs status.             */

        /* Check conditions to perform OFS or not.
           For example, no OFS if Intra-SCC Sleep is enabled. */
        if ( ( DETOUR_OK(sleep_time) )                            &&
#ifdef FEATURE_HDR_BCMCS
             ( hdrsrchbc_get_bc_status()  ==  BCMCS_DISABLED    ) &&
#endif /* FEATURE_HDR_BCMCS */
             ( hdrsrchofs_idle_ofs_state_visit_required() )       &&
             ( hdrsrchsleep_get_intra_scc_valid() == FALSE ) )
        {
          if ( hdrsrchrf_extend_duration( HDRSRCHRF_PRI_CHAIN, 
                                          HDRSRCHSLEEP_DETOUR_SCLK ) ==
               TRM_GRANTED )
          {
            HDR_MSG_SRCH( MSG_LEGACY_LOW,
                          "Lock extended for O-Freq Searching" );
            
            hdrsrchofs_register_idle_ofs_return_state( HDRSRCH_SLEEP_STATE );
              /* after idle ofs we should proceed to sleep state */
            
            hdrsrchofs_init_ofs_state();
              /* Since we have lock for OFS let's go do it. */

            break;
          }
          else
          {
            HDR_MSG_SRCH( MSG_LEGACY_MED,
                          "Cannot extend lock for O-Freq Searching" );
          }
        }
      }
          
      /* Fall through, if Off Frequency searching is not required */

      case HDRSRCH_OFS_IDLE_STATE:
  
        cc_start_to_now_hs = HDRSRCHSLEEP_HS_PER_SCC - 
          ( ( ( ( hdrsrchsleep.prev_cc_start * 2 ) + HDRSRCHSLEEP_HS_PER_SCC ) -
              ( HDRHSTR_GET_COUNT() % HDRSRCHSLEEP_HS_PER_SCC ) ) 
            % HDRSRCHSLEEP_HS_PER_SCC );
          /* Get the delta from the orginal CC start to now, i.e., we go to
             sleep.  Use the orginal cc_start save in hdrsrchsleep instead
             of HDRHSTR_GET_CC_START() since the mdsp has alrady been updated
             by cmac with the new/next cc_start.         */

        cc_start_to_ccdec_hs     = hdrsrchlog.sleep_info.cc_start_to_ccdec_hs;
          /* Get CC decoding time in halfslot from sleep info log structure */

        hdrsrchlog.sleep_info.ccdec_to_sleep_hs   = 
          ( cc_start_to_now_hs >= cc_start_to_ccdec_hs  ) ?
          ( cc_start_to_now_hs - cc_start_to_ccdec_hs   ) :
          ( cc_start_to_now_hs + HDRSRCHSLEEP_HS_PER_SCC - cc_start_to_ccdec_hs );
          /* calculate the time between decoding CC to entering sleep */

        hdrsrchlog.sleep_info.next_srch_state = HDRSRCH_SLEEP_STATE;
          /* Updates the next_srch_state field in the log packet            */

        hdrsrchlog_log_sleep_info( );
          /* State transition from IDLE OFS to SLEEP, needs to log packet   */

        /* Fall through                                          */

      case HDRSRCH_SUSPENDED_IDLE_STATE:

      default:
      {
        hdrsrchsleep.sleep_slots = sleep_time;
          /* Save computed sleep time */

        hdrsrchsleep_init_sleep_state();
          /* Finally, we can go to sleep */
      }

    } /* switch ( hdrsrch_state_id ) */
  }

  return will_sleep;

} /* hdrsrchsleep_enter_sleep( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_RESET_SCI

DESCRIPTION
  Resets the SCI index to zero.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchsleep_reset_sci( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.sci = 0;

  hdrsrchsleep.previous_sci = 0;

} /* hdrsrchsleep_reset_sci */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_SCI

DESCRIPTION
  Based on the reacquision slew, the reacquisition window size, and the
  sleep duration, compute the SCI for HDRSRCH_SLEEP_UNTIL_MAX_CCCS

DEPENDENCIES
  hdrsrchsleep.sleep_slots

RETURN VALUE
  None

SIDE EFFECTS
  Adjusts hdrsrchsleep.sci

===========================================================================*/

void hdrsrchsleep_adjust_sci
(
  int                             slew,
    /* Reacquisition slew, in half-chips */

  int                             winsize
    /* Reacquisition window, in chips */
)
{
  int                             max_sleep;
    /* Maximum sleep duration, in slots */

  int                             sci;
    /* SCI corresponding to maximum sleep duration */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Convert negative slews to positive slews */
  if (slew < 0)
  {
    slew = -slew;
  }

  /* Slight pessimism: Inflate slew by 2 chips (4 chipx2's) to account for
     quantization errors, and allow for boundary conditions.
     Also prevents division by zero, below. */

  slew += 4;


  /* Compute the maximum sleep duration where the reacquistion slew still
     falls within the reacquisition window.
     Note: should be "winsize/2" for the positive half of the window, and
           should be "slew/2" to convert chipx2's to chips,
           but the /2's cancel each other out. */

  max_sleep = (int) hdrsrchsleep.sleep_slots * winsize / slew;

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "Adjust: S=%d W=%d MS=%d",
                  hdrsrchsleep.sleep_slots, winsize, max_sleep );


  /* Search for the highest allowable SCI */
  for ( sci = 0;  sci < HDRSRCHSLEEP_MAX_SCI;  sci++ )
  {
    /* If the sleep duration of the next higher SCI exceeds max sleep ...*/
    if ( (HDRTS_SLOTS_PER_CCC << (sci+1)) > max_sleep )
    {
      /* ... don't climb up any higher */
      break;
    }
  }

  if ( sci != hdrsrchsleep.sci )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "SCI Change: %d -> %d",
                    hdrsrchsleep.sci, sci );

    hdrsrchsleep.previous_sci = hdrsrchsleep.sci;
      /* remember the old SCI */

    hdrsrchsleep.sci = sci;
  }

} /* hdrsrchsleep_adjust_sci */


#ifdef FEATURE_HDR_BCMCS

/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_FOR_BCMCS_WAKEUP

DESCRIPTION
  Determines if we need to wake up earlier to monitor BCMCS overhead or to 
  decode BCMCS data. If so, adjusts the wakeup time accordingly.

DEPENDENCIES
  hdrsrchsleep.wake_cycle
  hdrsrchsleep.wake_slot
  hdrsrchsleep.bcmcs_page_cycle
  hdrsrchsleep.bcmcs_sleep_cycle

PARAMETERS
  cycle - current sleep cycle count
  slot  - Slot offset in current sleep cycle
  num_of_scc_in_curr_cycle - how many SCCs in the current sleep cycle

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_adjust_for_bcmcs_wakeup
(
  qword       cycle,
    /* Current sleep cycle count */

  uint32      slot,
    /* Slot offset in current sleep cycle */

  uint8       num_of_scc_in_curr_cycle
    /* Number of SCCs in current sleep cycle */
)
{
  qword       curr_bcmcs_page_cycle;
    /* Current BCMCS page cycle count  */

  qword       curr_bcmcs_sleep_cycle;
    /* Current BCMCS sleep cycle count  */

  qword       wakeup_scc;
    /* The SCC count when AT needs to wake up */
  
  uint32      wakeup_slot;
    /* The offset slot count when AT needs to wake up */

  qword       next_bcmcs_page_scc;
    /* The next SCC count where we expect a BCMCS page */

  qword       next_bcmcs_data_scc;
    /* The next SCC count where AT can expect BCMCS data */

  boolean     wakeup_time_adjusted = FALSE;
    /* indicates whether wakeup time is adjusted or not */

  uint16      remaining_scc;
    /* temporary storage  */

  int8        comp;
    /* temporary storage  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ( hdrsrchsleep.bcmcs_page_cycle  != 0 ) ||
       ( hdrsrchsleep.bcmcs_sleep_cycle != 0 )  )
  {
    qw_mul( cycle, cycle, num_of_scc_in_curr_cycle );
    qw_inc( cycle, slot / HDRTS_SLOTS_PER_SCC );
      /* Get the current SCC count                */

    qw_mul( wakeup_scc, hdrsrchsleep.wake_cycle, HDRTS_SCC_PER_CCC );
    qw_inc( wakeup_scc, hdrsrchsleep.wake_slot / HDRTS_SLOTS_PER_SCC );
      /* Get the SCC count when AT needs to wake up. This count is computed
         before entering this "adjustment" function and is based on regular
         HDR slot cycle index                                             */

    /* Find out next where we are in terms of BCMCS page cycle */
    if ( hdrsrchsleep.bcmcs_page_cycle != 0 )
    {
      remaining_scc = qw_div( curr_bcmcs_page_cycle, 
                              cycle,
                              hdrsrchsleep.bcmcs_page_cycle );

      qw_equ( next_bcmcs_page_scc, cycle );
      qw_inc( next_bcmcs_page_scc, 
              hdrsrchsleep.bcmcs_page_cycle - remaining_scc );

      comp = qw_cmp( next_bcmcs_page_scc, wakeup_scc );

      if ( comp < 0 )
      {
        qw_equ( wakeup_scc, next_bcmcs_page_scc );

        wakeup_time_adjusted = TRUE;
          /* wake-up needs to be adjusted */

        hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_BCMCS_OVHD;
          /* the new wake-up reason: for BCMCS overhead         */
      }
      else if ( comp == 0 )
      {
        hdrsrchsleep.wakeup_reason |= HDRSRCH_WAKEUP_BCMCS_OVHD;
          /* add one new wake-up reason: for BCMCS overhead     */
      }

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "Curr B-PageCycle=%d Next Wakeup SCC=%d",
                      qw_lo( curr_bcmcs_page_cycle ),
                      qw_lo( next_bcmcs_page_scc ) );
    }

    /* Find out next where we are in terms of BCMCS sleep cycle */
    if ( hdrsrchsleep.bcmcs_sleep_cycle != 0 )
    {
      remaining_scc = qw_div( curr_bcmcs_sleep_cycle, 
                              cycle, 
                              hdrsrchsleep.bcmcs_sleep_cycle );

      qw_equ( next_bcmcs_data_scc, cycle );
      qw_inc( next_bcmcs_data_scc, 
              hdrsrchsleep.bcmcs_sleep_cycle - remaining_scc );

      comp = qw_cmp( next_bcmcs_data_scc, wakeup_scc );

      if ( comp < 0 )
      {
        qw_equ( wakeup_scc, next_bcmcs_data_scc );

        wakeup_time_adjusted = TRUE;
          /* wake-up needs to be adjusted       */

        hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_BCMCS_DATA;
          /* the new wake-up reason: for BCMCS page         */
      }
      else if ( comp == 0 )
      {
        hdrsrchsleep.wakeup_reason |= HDRSRCH_WAKEUP_BCMCS_DATA;
          /* add one new wake-up reason: for BCMCS page     */
      }

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "Curr B-SleepCycle=%d Next Wakeup SCC=%d",
                      qw_lo( curr_bcmcs_sleep_cycle ),
                      qw_lo( next_bcmcs_data_scc  ) );
    }

    if ( wakeup_time_adjusted == TRUE )
    {
      /* Currently we plan to wake up later than the next bcmcs SCC(page/data),
         hence we need to adjust the wakeup point ahead. The wakeup time is 
         converted back to 5.12s base as well                                */

      qw_dec( wakeup_scc, 1);
      wakeup_slot = HDRTS_SLOTS_PER_SCC;

      remaining_scc = qw_div( hdrsrchsleep.wake_cycle,
                              wakeup_scc, HDRTS_SCC_PER_CCC );

      wakeup_slot += remaining_scc * HDRTS_SLOTS_PER_SCC;

      hdrsrchsleep.wake_slot = wakeup_slot;

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                      "Wakeup Adjusted for BCMCS: %d/%d/%d reason:0x%x",
                      qw_hi( hdrsrchsleep.wake_cycle ),
                      qw_lo( hdrsrchsleep.wake_cycle ),
                      wakeup_slot,
                      hdrsrchsleep.wakeup_reason      );
    } /* if wakeup_time_adjusted == TRUE  */

  } /* if hdrsrchsleep.bcmcs_page_cycle */

} /* hdrsrchsleep_adjust_for_bcmcs_wakeup */

#endif /* FEATURE_HDR_BCMCS */


#ifdef FEATURE_HDR_TO_LTE

/*===========================================================================

FUNCTION HDRSRCHSLEEP_ADJUST_FOR_ORNL_WAKEUP

DESCRIPTION
  Determines if we need to wake up earlier to monitor Other RAT (Radio Access
  Technology Neighbor List overhead message.  If so, adjusts the wakeup time
  accordingly.

  The OtherRATNeighborList transmission cycle is as follows (per Table
  5.12.6.2.1-1 of 3GPP2 C.S0087-0 v2.0 spec.
 
     Field value (binary)    Duration in unit of
                             Control Channel Cycle
     ‘000’                    2
     ‘001’                    3
     ‘010’                    6
     ‘011’                   10
     ‘100’                   14
     ‘101’                   24
     ‘110’                   40
     ‘111’                   64
     
DEPENDENCIES
  hdrsrchsleep.wake_cycle
  hdrsrchsleep.wake_slot
  hdrsrchsleep.ornl_cycle

PARAMETERS
  cycle - current sleep cycle count
  slot  - Slot offset in current sleep cycle
  num_of_scc_in_curr_cycle - how many SCCs in the current sleep cycle

RETURN VALUE
  None

SIDE EFFECTS 
  Return immediately if ornl_cycle is invalid.

===========================================================================*/

LOCAL void hdrsrchsleep_adjust_for_ornl_wakeup
(
  qword       cycle,
    /* Current sleep cycle count */

  uint32      slot,
    /* Slot offset in current sleep cycle */

  uint8       num_of_scc_in_curr_cycle
    /* Number of SCCs in current sleep cycle */
)
{
  qword       curr_ornl_cycle;
    /* Current Other RAT neighbor list cycle count  */

  qword       wakeup_scc;
    /* The SCC count when AT needs to wake up */
  
  uint32      wakeup_slot;
    /* The offset slot count when AT needs to wake up */

  qword       next_ornl_scc;
    /* The next SCC count where we expect an Other RAT neighbor list message */

  uint8       ornl_scc;
    /* Obtained from look up table hdrsrchsleep_ornl_cycle_tab, in unit of
       SCCs (426.7ms) */

  boolean     wakeup_time_adjusted = FALSE;
    /* Indicates whether wakeup time is adjusted or not */

  uint16      remaining_scc;
    /* Temporary storage  */

  int8        comp;
    /* Temporary storage  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_4( 
    MSG_LEGACY_HIGH, 
    "D2L adjust_for_ornl_wakeup(),ornl_cycle=%d,Cycle=%d,Slot=%d,NumSCC=%d",
    hdrsrchsleep.ornl_cycle,
    qw_lo( cycle ),
    slot,
    num_of_scc_in_curr_cycle
  );

  ornl_scc = 
    hdrsrchsleep_ornl_cycle_tab[ hdrsrchsleep.ornl_cycle & HDRSRCHSLEEP_ORNL_CYCLE_MASK ];
    /* Use the look up table to get the Other RAT neighbor list duration in unit of SCCs */

  if( ornl_scc != 0 )
    /* Avoid divide by 0, although this check should never fail because the look up table
       does not contain 0 */
  {
    qw_mul( cycle, cycle, num_of_scc_in_curr_cycle );
    qw_inc( cycle, slot / HDRTS_SLOTS_PER_SCC );
      /* Get the current SCC count */

    qw_mul( wakeup_scc, hdrsrchsleep.wake_cycle, HDRTS_SCC_PER_CCC );
    qw_inc( wakeup_scc, hdrsrchsleep.wake_slot / HDRTS_SLOTS_PER_SCC );
      /* Get the SCC count when AT needs to wake up. This count is computed
         before entering this "adjustment" function and is based on regular
         HDR slot cycle index                                             */

    /* Find out next where we are in terms of Other RAT neighbor list cycle */
    remaining_scc = qw_div( curr_ornl_cycle, 
                            cycle,
                            ornl_scc );

    qw_equ( next_ornl_scc, cycle );
    qw_inc( next_ornl_scc, 
            ornl_scc - remaining_scc );

    comp = qw_cmp( next_ornl_scc, wakeup_scc );

    /* If Other RAT wake up cycle occurs first, then adjust wake up time */
    if ( comp < 0 )
    {
      qw_equ( wakeup_scc, next_ornl_scc );

      wakeup_time_adjusted = TRUE;
        /* Wake-up needs to be adjusted */

      hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_OTHER_RAT_NL;
        /* The new wake-up reason is to get Other RAT Neighbor List OTA message */
    }
    else if ( comp == 0 )
    {
      hdrsrchsleep.wakeup_reason |= HDRSRCH_WAKEUP_OTHER_RAT_NL;
        /* add one new wake-up reason, to get Other RAT Neighbor List OTA message */
    }

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "D2L Curr ORAT Cycle=%d Next Wakeup SCC=%d",
                    qw_lo( curr_ornl_cycle ),
                    qw_lo( next_ornl_scc ) );

    if ( wakeup_time_adjusted == TRUE )
    {
      /* Previously we planned to wake up later than the next Other RAT Neighbor list
         cycle, hence we need to adjust the wakeup point earlier. The wakeup time is 
         converted back to 5.12s base as well. */

      qw_dec( wakeup_scc, 1);
      wakeup_slot = HDRTS_SLOTS_PER_SCC;

      remaining_scc = qw_div( hdrsrchsleep.wake_cycle,
                              wakeup_scc, HDRTS_SCC_PER_CCC );

      wakeup_slot += remaining_scc * HDRTS_SLOTS_PER_SCC;

      hdrsrchsleep.wake_slot = wakeup_slot;

      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                      "D2L Wakeup Adjusted for Other RAT: %d/%d/%d reason:0x%x",
                      qw_hi( hdrsrchsleep.wake_cycle ),
                      qw_lo( hdrsrchsleep.wake_cycle ),
                      wakeup_slot,
                      hdrsrchsleep.wakeup_reason );

    } /* if wakeup_time_adjusted == TRUE  */

  } /* if hdrsrchsleep.ornl_cycle */

} /* hdrsrchsleep_adjust_for_ornl_wakeup */

#endif /* FEATURE_HDR_TO_LTE */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_COMPUTE_NEXT_WAKEUP

DESCRIPTION
  Determine the next cycle the AT must monitor. The next cycle can be regular
  HDR page cycle, HDR BCMCS page cycle, or a BCMCS data cycle.

DEPENDENCIES
  If a Control Channel Cycle has been specified, the next SCC is based on
  a 5.12 second period.

  If a Control Channel Cycle has not been specified, the next SCC is based
  on a 426.7 millisecond period.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchsleep_compute_next_wakeup( void )
{
  qword                       cycle;
    /* Number of sleep cycles since the beginning of time */

  uint32                      slot;
    /* Current slot within the current sleep cycle */

  uint32                      num_of_slots_in_curr_sci;
    /* The period in slots of current SCI          */

  uint32                      scc_slot;
    /* Next SCC slot */

  uint8                       num_of_scc_in_curr_cycle = HDRTS_SCC_PER_CCC;
    /* 3 for 1.28s, 6 for 2.56s, 12 for 5.12s, etc          */

  uint16                      num_of_scc_in_neg_sci;
    /* 3 for 1.28s, 6 for 2.56s, 12 for 5.12s, etc          */

  uint32                      wake_slot;
    /* Slot at which the AT must wake up at to monitor the SCC_SLOT */

  uint16                      remaining_scc;
    /* temporary storage                   */

  uint8                       virtual_ccc = hdrsrchsleep.control_channel_cycle;
    /* Hash value "R" for "(C + R) mod Nsleep == 0"
       Note: If until > neg_sci, then this is randomized over the larger
             "until" period */

  uint16                      ran_dist_value;
    /* Random distributed value between 0 and number of smaller periods in
       larger period. */

  uint16                      scc_offset_adjustment;
    /* SCC offset adjustment in slots */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep_adjust_wakeup_sci();
    /* adjust wake up time according to the new wake up slot */

#ifndef FEATURE_HDR_RUMI
  if ( hdrsrchsleep.control_channel_cycle == HDRSRCH_NO_CCC_HASH )
  {
    hdrsrchsleep.until = 0x6;
      /* The CC hash value has not been given by protocols so force wakeups
         at all SCC boundaries. */
  }
#endif /* FEATURE_HDR_RUMI */

  hdrsrchsleep.sci_log = hdrsrchsleep.until;
    /* Updates sci_log for "sleep_info" log packet  */

  hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_PAGE;
    /* for now, the next wake-up is for HDR page  */

  /* Determine the next SCC slot we must monitor */
  if ( hdrsrchsleep.intra_scc_start_slot_valid )
  {
    /* Do not use SCI to calculate the next wake slot.  Instead,
       the upper layer has provided this information.  This is useful
       during intra-SCC sleep calculations.  Get current cycle and
       use given intra_scc_start_slot as scc_slot for wakeup.  */

    hdrts_get_cycle_and_slot( cycle, &slot, HDRTS_SLOTS_PER_CCC );
      /* Get the present time */

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d curr cycle(hi)=%d (lo)=%d slot=%d slot_per_ccc=%d",
                    hdrsrchsleep.until,
                    qw_hi(cycle),
                    qw_lo(cycle),
                    slot,
                    HDRTS_SLOTS_PER_CCC);

    scc_offset_adjustment = hdrsrchsleep.scc_offset;

    if ( scc_offset_adjustment > 3 )
    {
      scc_offset_adjustment = 0;
        /* SCC Offset might be 0xFFFF for decoding all interleaves.  Assume 0
           slots for wakeup purpose */
    }

    /* calculate the SCC boundary */
    scc_slot = slot / HDRTS_SLOTS_PER_SCC;
    scc_slot *= HDRTS_SLOTS_PER_SCC;

    scc_slot += hdrsrchsleep.intra_scc_start_slot + scc_offset_adjustment;
      /* Use CC start time as given by the upper layer adding scc_offset */

    hdrsrchsleep.cc_start = hdrsrchsleep.intra_scc_start_slot % HDRTS_SLOTS_PER_SCC;
      /* Time in slots that DSP expects to find CC/SCC when we wake up,
         indirectly the same value given by protocols.
         Note: Do not include scc_offset_adjustment since that is already taken into
               consideration by calling hdrsrchsleep_set_cc_start() when rf warm is done. */

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d NegSCI=%d CCC=%d CurrSlot: %d WakeupSlot:%d",
                    hdrsrchsleep.until,
                    hdrsrchsleep.neg_sci,
                    hdrsrchsleep.control_channel_cycle,
                    slot,
                    scc_slot);
  }
  else if ( hdrsrchsleep.until <= 0x6 )
  {
    /* For 426ms sleep, wake up at next SCC boundary  */

    hdrts_get_cycle_and_slot( cycle, &slot, HDRTS_SLOTS_PER_CCC );
      /* Get the present time */

   HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d curr cycle(hi)=%d (lo)=%d slot=%d slot_per_ccc=%d",
                    hdrsrchsleep.until,
                    qw_hi(cycle),
                    qw_lo(cycle),
                    slot,
                    HDRTS_SLOTS_PER_CCC);

    num_of_slots_in_curr_sci = HDRTS_SLOTS_PER_SCC >> (0x6-hdrsrchsleep.until);
      /* Get the period ( in slots ) of current SCI */

    scc_slot = slot - slot % num_of_slots_in_curr_sci;
      /* Determine the slot of the previous SCI period */

    scc_slot += num_of_slots_in_curr_sci;
      /* And compute the next SCI slot */
    
#ifdef FEATURE_HDR_REVA_L1
    hdrsrchsleep.cc_start = scc_slot % HDRTS_SLOTS_PER_SCC;
      /* Time in slots that DSP expects to find CC/SCC when we wake up  */
#endif /* FEATURE_HDR_REVA_L1  */

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d NegSCI=%d CCC=%d CurrSlot: %d WakeupSlot:%d",
                    hdrsrchsleep.until,
                    hdrsrchsleep.neg_sci,
                    hdrsrchsleep.control_channel_cycle,
                    slot,
                    scc_slot);
  }
  else
  {
#ifdef FEATURE_HDR_REVA_L1
    hdrsrchsleep.cc_start = 0;
      /* For SCI greater than 6, always wake up at SCC boundary */
#endif /* FEATURE_HDR_REVA_L1 */
      
    num_of_scc_in_curr_cycle = 3 * ( 1 << ( hdrsrchsleep.until - 7 ) );

    hdrts_get_cycle_and_slot( cycle, &slot, 
                              HDRTS_SLOTS_PER_SCC * num_of_scc_in_curr_cycle );

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d curr cycle(hi)=%d (lo)=%d slot=%d slot_per_cycle=%d",
                    hdrsrchsleep.until,
                    qw_hi(cycle),
                    qw_lo(cycle),
                    slot,
                    HDRTS_SLOTS_PER_SCC * num_of_scc_in_curr_cycle );

    /* If the negotiated SCI (protocol stack) is less than what protocol
       actually wants us to do, to avoid multiple AT's from waking up in
       a smaller period (of neg_sci), hash again to the larger sleep cycle.
       Note: For RevA, this spreads wakeup over a larger sleep cycle set by
       protocol since the Control Channel Cycle is computed based on the
       smaller neg_sci. */
     if ( ( hdrsrchsleep.until > hdrsrchsleep.neg_sci ) &&
          ( hdrsrchsleep.neg_sci > 0x6 ) )
     {
       num_of_scc_in_neg_sci =  3 * ( 1 << ( hdrsrchsleep.neg_sci - 7 ) );
         /* SCI=7 represents 3 CCC in 1.28s (assume SCI > 6) */

       ran_dist_value = ran_dist( hdrsrchsleep.ran_next_sav, 0,
                     ( 1 << ( hdrsrchsleep.until - hdrsrchsleep.neg_sci ) ) );

       virtual_ccc = hdrsrchsleep.control_channel_cycle +
                     ran_dist_value * num_of_scc_in_neg_sci;
         /* Use random value to spread wakeup across larger SCI by calculating
            a new control channel cycle by proportionally adding random
            multiples of number of scc in neg_sci.  For example, when neg_sci
            is 9 and protocol sci is 12 then the new ccc will be spread across
            96 control channel cycles instead of 12.
            Note: Random value is between 0 and 2 ^ (SCI - neg_sci).  Use a
            random number (ran_next_sav) computed when CCC was set to avoid
            changing hash too frequently.                                    */

       HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                       "Hash CCC over larger SCI. rand_value=%d * %d",
                       ran_dist_value,
                       num_of_scc_in_neg_sci );
     }

     HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "SCI=%d NegSCI=%d virtual_ccc=%d CurrTime: %d/%d",
                    hdrsrchsleep.until,
                    hdrsrchsleep.neg_sci,
                    virtual_ccc,
                    qw_lo( cycle ),
                    slot);
  
    scc_slot = virtual_ccc % num_of_scc_in_curr_cycle;
      /* Find the SCC index in the current sleep cycle. Remember that control
         channel cycle in "hdrsrchsleep" is calculated based on the maximum
         sleep cycle, and the current sleep cycle can be less than that. Here
         we use scc_slot as a temp variable for SCC index within the current
         sleep cycle     */

    scc_slot = HDRTS_SLOTS_PER_SCC * ( num_of_scc_in_curr_cycle - scc_slot );
      /* Start of the previous or the next assigned SCC message, in slots 
         relative to the start of current sleep cycle                 */
    
    /* If the SCC slot is "before" the current slot, assume it is the
       previous assigned SCC slot, and project into future for next. */
    if ( scc_slot <= slot )
    {
      scc_slot += HDRTS_SLOTS_PER_SCC * num_of_scc_in_curr_cycle;
    }

  }

  wake_slot = scc_slot;
    /* Determine slot the AT must wake up at to monitor the given
       SCC slot */

  /* Next we normalize wake_cycle/wake_slot to 5.12s cycles, which is needed
     to calculate the actual sleep time                                     */

  if ( ( hdrsrchsleep.until <= 0x6 ) ||
       ( hdrsrchsleep_get_intra_scc_valid() == TRUE ) )
  {
    qw_equ( hdrsrchsleep.wake_cycle, cycle);
    hdrsrchsleep.wake_slot  = wake_slot;
      /* For SCI <= 6, the computation is complete by now    */
  }
  else
  {
    /* For Sleep SCI greater than 0x6 ( 426ms sleep )       */
    if ( hdrsrchsleep.until < 0x9 )
    {
      /* for SCI 7 and 8    */
      remaining_scc = qw_div( hdrsrchsleep.wake_cycle, cycle, 
                              HDRTS_SCC_PER_CCC/num_of_scc_in_curr_cycle);
        /* Now wake_cycle is aligned to 5.12s cycle */

      remaining_scc *= num_of_scc_in_curr_cycle;
        /* Remaining number of SCCs           */

      wake_slot += remaining_scc * HDRTS_SLOTS_PER_SCC;
    }
    else
    {
      qw_mul( hdrsrchsleep.wake_cycle, cycle, 
              num_of_scc_in_curr_cycle/HDRTS_SCC_PER_CCC);
    } /* if hdrsrchsleep.until */

    hdrsrchsleep.wake_slot = wake_slot;

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "Wakeup Time: %d/%d/%d Mod 8 cycle=%d",
                    qw_hi( hdrsrchsleep.wake_cycle ),
                    qw_lo( hdrsrchsleep.wake_cycle ),
                    wake_slot,
                    qw_lo(hdrsrchsleep.wake_cycle) % 8 );
      /* Mod 8 of wake_cycle is useful when in 5.12 sleep and transitioning to
         40 sec sleep, to see which 5.12 sec cycle within the 40 sec cycle. */

#ifdef FEATURE_HDR_BCMCS
    /* Next we look at BCMCS paging cycle, to see if we need to wake-up
       earlier to monitor BCMCS overhead information                    */

    hdrsrchsleep_adjust_for_bcmcs_wakeup(cycle,slot,num_of_scc_in_curr_cycle);

#endif /* FEATURE_HDR_BCMCS */

#ifdef FEATURE_HDR_TO_LTE
    /* Next we look at Other RAT Neighbor List (ORNL) cycle to see if we need
       to wake up earlier to monitor ORNL overhead information */

    if( hdrsrchsleep.ornl_cycle == HDRSRCH_ORNL_CYCLE_DISABLED )
      /* Only continue on if other RAT neighbor list cycle is enabled */
    {
      hdrsrchltemeas_treselect_timer_clear();
        /* Clear pending Treselect timers */

      /* Reset all LTE measurement information once LTE reselection occurs.
         If LTE fails to acquire, DO should start LTE measurements again from
         scratch, not from staled data. */
      hdrsrchltemeas_reset_deinit();

      hdrsrchltemeas_clear_reselect_status();
        /* There is no LTE coverage here */

      HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L ORNL cycle is set to Disabled" );
    }
    else if( hdrsrchsleep.ornl_cycle == HDRSRCH_ORNL_CYCLE_NO_UPDATE )
      /* Only continue on if other RAT neighbor list needs to be updated */
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L ORNL cycle is set to No Update" );
    }
    else if (hdrsrchsleep.rf_wait_state == HDRSRCHSLEEP_RF_NOT_WAITING )
    {
      hdrsrchsleep_adjust_for_ornl_wakeup(cycle,slot,num_of_scc_in_curr_cycle);
    }
#endif /* FEATURE_HDR_TO_LTE */

  } /* else */

#ifdef FEATURE_HDR_BCMCS
  if ( hdrsrchbc_get_bc_status() == BCMCS_ENABLED )
  {
    if ( HDRSRCH_NORMAL_AGC_ACQ_NV_ENABLE )
    {
      hdrsrchsleep.wake_slot -= HDRSRCHSLEEP_REACQ_NORMAL_AGC_TIME_SLOTS_DEFAULT;
        /* For BCMCS wakeup earlier than regular optimized wakeup so that 
           all reacquisition searches can complete before the SCC boundary */  

      hdrsrchsleep.reacq_slots = HDRSRCHSLEEP_REACQ_NORMAL_AGC_TIME_SLOTS_DEFAULT;
    }
    else
    {
      hdrsrchsleep.wake_slot -= HDRSRCHSLEEP_REACQ_TIME_SLOTS + 
                                         HDRSRCHSLEEP_BCMCS_REACQ_BUF_SLOTS;
        /* For BCMCS wakeup earlier than regular optimized wakeup so that 
           three reacquisition searches can complete before the SCC boundary */  

      hdrsrchsleep.reacq_slots = HDRSRCHSLEEP_REACQ_TIME_SLOTS + 
                                         HDRSRCHSLEEP_BCMCS_REACQ_BUF_SLOTS;
    }

    hdrsrchrf_reset_agc_est();
      /* We will reenable the algorithm when BCMCS is shut off. */
  }
  else
#endif /* FEATURE_HDR_BCMCS */
  {
    if ( ( hdrsrchsleep_get_intra_scc_valid() == FALSE ) &&
         ( ( hdrsrchsleep.until > 0x6 ) || ( hdrsrchsleep.neg_sci > 0x6 ) ) )
    {
      hdrsrchrf_reset_agc_est();
        /* We will reenable the algorithm when going to a smaller SCI. */
    }

    hdrsrchsleep.wake_slot -= HDRSRCHSLEEP_REACQ_TIME_SLOTS;
      /* Save wakeup time (cycle & slot).  We'll make the reservation
         when we can release the lock. */

    hdrsrchsleep.reacq_slots = HDRSRCHSLEEP_REACQ_TIME_SLOTS;
  }
  
  if ( ( hdrsrchsleep.qpch_enabled == TRUE ) && 
       ( hdrsrchsleep.intra_scc_start_slot_valid == FALSE ) )
  {
    hdrsrchsleep.wake_slot--;
      /* Wake up 1 slot earlier to monitor the QPCH */
  }
} /* hdrsrchsleep_compute_next_wakeup( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHSLEEP_SET_WAKEUP

DESCRIPTION
  This function specifies when to wakeup next (SCC or CCC), and who to
  inform when we wakeup.  When either until or neg_sci parameters are <= 0x6,
  until will be used, i.e., no support of until > neg_sci when until <= 0x6
  or neg_sci <= 0x6.

DEPENDENCIES
  None

PARAMETERS
  until                      - Wakeup for the next SCC, or the next CCC.
                               Not used if cc_start_slot_valid is TRUE.
  neg_sci                    - Negotiated SCI which may be different from the
                               next wakeup time
  qpch_enabled               - Whether QPCH is enabled or not
  wakeup_cb                  - Who to inform when we wakeup
  cc_start_slot_valid        - Flag whether cc_start_slot parameter is
                               valid or not.  If TRUE, use cc_start_slot
                               instead SCI to calculate  wake_slot.
  cc_start_slot              - CC start in slots.  This field is used if
                               cc_start_slot_valid is TRUE.
  ornl_cycle                 - Other RAT Neighbor List message has its own
                               cycle

RETURN VALUE
  None

SIDE EFFECTS
  Wakeup cycle and slot is computed. Also updates reva_sleep_info log packet.
  The first time the neg_sci is set after reacquiring.

===========================================================================*/

void hdrsrchsleep_set_wakeup
(
  uint8                                       until,
    /* When should we wakeup next?  Not used if cc_start_slot_valid is TRUE. */

  uint8                                       neg_sci,
    /* Negotiated SCI which may be different from the next wakeup time */

  boolean                                     qpch_enabled,
    /* Whether QPCH is enabled or not */

  hdrsrch_wakeup_cb_type                      wakeup_cb,
    /* What to do when we wakeup? */

  boolean                                     cc_start_slot_valid,
    /* Flag used to indicate whether cc_start_slot is valid */

  uint16                                      cc_start_slot
    /* CC start in slots. This field is valid if cc_start_slot_valid is TRUE */

#ifdef FEATURE_HDR_TO_LTE
  ,uint8                                      ornl_cycle
    /* Other RAT (Radio Access Technologies) Neighbor List message has its own cycle */
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  ,boolean                                   session_is_open,
    /* Whether session is open */

  boolean                                   ovhd_is_updated
    /* Whether Ovhd is updated */    
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
  /* Store the Session and Ovhd updated statuses */
  hdrsrchsleep.session_is_open = session_is_open;
  hdrsrchsleep.ovhd_is_updated = ovhd_is_updated;

  /* Reset the Ovhd denied count */
  if ( ( hdrsrchsleep.session_is_open == TRUE ) && ( hdrsrchsleep.ovhd_is_updated == TRUE )  )
  {
    hdrsrchsleep.ovhd_denied_cnt = 0; 
  }

  /* Register drx cycle with trm if needed */
  if ( hdrsrchsleep.neg_sci != neg_sci )
  {
    hdrsrchrf_register_sci_with_trm ( hdrsrchsleep_sci_ms[neg_sci] );
  }
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

  if( !cc_start_slot_valid )
  {
    hdrsrchsleep.last_normal_sleep_sci = hdrsrchsleep.until;
      /* Remember the last sleep sci for non intra scc sleep */
    if ( hdrsrchsleep.last_normal_sleep_sci == 0 )
    {
      hdrsrchsleep.last_normal_sleep_sci = 9;
        /* until might be 0 if it is the first time we go to sleep.
           Set last_normal_sleep_sci to 9 in this situation.
        */
      HDR_MSG_SRCH( MSG_LEGACY_LOW,"Reset last_sci to 9");
    }
  }

  hdrsrchsleep.until     = ( until <= 0xc ) ? ( until ) : 0xc ;
    /* Remember Sleep Until time. And saturates it at 0xc */

  hdrsrchsleep.neg_sci   = ( neg_sci <= 0xc ) ? ( neg_sci ) : 0xc;
    /* Remember what the negotiated SCI is which may differ from what protocols
       wants us to do for this wakeup. */

  hdrsrchsleep.qpch_enabled = qpch_enabled;
    /* Remember whether QPCH is enabled or not */

  hdrsrchsleep.intra_scc_start_slot_valid = cc_start_slot_valid;
  hdrsrchsleep.intra_scc_start_slot       = cc_start_slot;
    /* Remember intra_scc_start_slot info. */

#ifdef FEATURE_HDR_TO_LTE
  hdrsrchsleep.ornl_cycle         = ornl_cycle;
    /* Remember the Other RAT neighbor list cycle */
#endif /* FEATURE_HDR_TO_LTE */

  /* Estimate the number of pages missed due to being out of service. */
  if ( hdrsrchlog.time_oos_ms > 0 )
  {
    if ( hdrsrchsleep.neg_sci <= 6 )
    {
      hdrsrchlog.sleep_info.est_out_of_service_ccmer += 
                              hdrsrchlog.time_oos_ms / 
                              SLOTS_TO_MS( ( 1 << hdrsrchsleep.neg_sci ) * 4 );
        /* Estimate wakeups missed due to being out of service. Conversion for
           scis less than a CC cycle. Number 4 is the the number of slots 
           based on the conversion in the standard. */
    }
    else
    {
      hdrsrchlog.sleep_info.est_out_of_service_ccmer += 
                      hdrsrchlog.time_oos_ms / 
                      SLOTS_TO_MS( ( 1 << ( hdrsrchsleep.neg_sci - 7 ) ) * 768 );
        /* Estimate wakeups missed due to being out of service. Conversion for
           scis larger than a CC cycle. Number 768 is the the number of slots 
           based on the conversion in the standard. */
    }

    hdrsrchlog.time_oos_ms = 0;
  }

  hdrsrchreacq_set_wakeup_report( wakeup_cb );
    /* Forward wakeup report callback to reacq state. */

  hdrsrchsleep_compute_next_wakeup( );
    /* Compute time of next required wakeup */

} /* hdrsrchsleep_set_wakeup( ) */


/* EJECT */
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_SET_CONTROL_CHANNEL_CYCLE

DESCRIPTION
  Set the Control Channel Hash value "R", for "(C+R) mod Nsleep == 0"
  Also sets the maximum HDR sleep cycle

DEPENDENCIES
  None

INPUTS
  ccc - Control Channel Hash
  max_hdr_sleep_cycle - maximum HDR sleep cycle

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_set_control_channel_cycle
(
  uint8   ccc,
    /* Control Channel Hash value */

  uint8   max_sleep_cycle
    /* Maximum sleep period       */
    /* REVISIT: max_sleep_cycle is unused so this can be removed as a
       parameter.  It is left in now because multiple functions and files
       are affected by its removal, so to limit the scope of changes, it
       remains for now. */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
                  "Set CC Cycle = %d Max SCI = %d SCI = %d, negotiated SCI = %d", 
                  ccc,
                  max_sleep_cycle,
                  hdrsrchsleep.until,
                  hdrsrchsleep.neg_sci );

  hdrsrchsleep.control_channel_cycle = ccc;
    /* Set the control channel hash value */

  hdrsrchsleep.ran_next_sav = hdrutil_get_random();
    /* Compute a random number to be used in hashing when computing next
       wakeup.  Getting the random number at this point prevent computing
       a different hash if protocol SCI changes. */

} /* hdrsrchsleep_set_control_channel_hash( ) */


/* EJECT */
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_GET_CC_START

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

uint8 hdrsrchsleep_get_cc_start( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchsleep.cc_start;
    /* return CC start */

} /* hdrsrchsleep_get_cc_start */

#ifdef FEATURE_HDR_REVA_L1
/* EJECT */
/*==========================================================================

FUNCTION      HDRSRCHSLEEP_SET_CC_START

DESCRIPTION
  This function sets the CC start to indicate where to expect CC/SCC premable.
  
DEPENDENCIES
  This function needs to be called after AT wakes up. 

INPUTS
  cc_start - CC start in slot

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchsleep_set_cc_start
(
  uint8               cc_start
    /* Time in slot */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.cc_start = cc_start;
    /* Set CC start */

}/* hdrsrchsleep_set_cc_start */
#endif /* FEATURE_HDR_REVA_L1 */


/*=========================================================================

FUNCTION      HDRSRCHSLEEP_GET_SCI

DESCRIPTION
  This function returns the previous HDR SCI value. This is provided as an
  access function for HDRSRCH logging.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Current SCI value

SIDE EFFECTS
  None

=========================================================================*/

int8 hdrsrchsleep_get_sci( )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchsleep.sci_log;
    /* return sci_log for sleep_info log packet   */

} /* hdrsrchsleep_get_sci ( ) */


#ifdef FEATURE_HDR_BCMCS
/*=========================================================================

FUNCTION      HDRSRCHSLEEP_SET_BCMCS_PAGE_CYCLE

DESCRIPTION
  This command updates the BCMCS page cycle value.

DEPENDENCIES
  None

INPUTS
  page_cycle    - The BCMCS paging channel period which the AT monitors, in
                  units of SCC ( 256 slots ).

RETURN VALUE
  Current SCI value

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchsleep_set_bcmcs_page_cycle
(
  uint8     page_cycle
    /* BCMCS page cycle */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.bcmcs_page_cycle = page_cycle;
    /* Sets the BCMCS page cycle value   */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Set BCMCS page cycle: %d", page_cycle);

} /* hdrsrchsleep_set_bcmcs_page_cycle */


/*=========================================================================

FUNCTION      HDRSRCHSLEEP_SET_BCMCS_SLEEP_CYCLE

DESCRIPTION
  This command updates the BCMCS sleep cycle value. This function is called
  when AT is awake. The new value is used when AT goes back to sleep. No 
  critical section protection is required because of the timing dependency.

DEPENDENCIES
  None

INPUTS
  sleep_cycle    - The BCMCS sleep period, in units of SCC ( 256 slots ).

RETURN VALUE
  Current Sleep period

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchsleep_set_bcmcs_sleep_cycle
(
  uint8     sleep_cycle
    /* BCMCS sleep period */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.bcmcs_sleep_cycle = sleep_cycle;
    /* Sets the BCMCS sleep cycle value   */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Set BCMCS sleep cycle: %d", sleep_cycle);

} /* hdrsrchsleep_set_bcmcs_sleep_cycle */

/*=========================================================================

FUNCTION      HDRSRCHSLEEP_GET_BCMCS_SLEEP_CYCLE

DESCRIPTION
  This command returns the current BCMCS sleep cycle value.
 
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Current BCMCS sleep period

SIDE EFFECTS
  None

=========================================================================*/

uint8 hdrsrchsleep_get_bcmcs_sleep_cycle( void )
{
  return hdrsrchsleep.bcmcs_sleep_cycle;
} /* hdrsrchsleep_get_bcmcs_sleep_cycle */

#endif /* FEATURE_HDR_BCMCS */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_RESET_WAKEUP_REASON

DESCRIPTION
  This function sets the wakeup reason to "no wakeup pending".

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrchsleep_reset_wakeup_reason( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.wakeup_reason = HDRSRCH_WAKEUP_NO_WAKEUP_PENDING;

} /* hdrsrchsleep_reset_wakeup_reason */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_WAKEUP_REASON

DESCRIPTION
  This function returns the wakeup reason.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  Reason for this wake-up

SIDE EFFECTS
  None
  
==========================================================================*/

hdrsrch_wakeup_reason_enum_type hdrsrchsleep_get_wakeup_reason( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.wakeup_reason );

} /* hdrsrchsleep_get_wakeup_reason */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_SCC_OFFSET

DESCRIPTION
  This function returns the scc offset saved before going to sleep.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value of scc offset saved before going to sleep. 

SIDE EFFECTS
  None
  
==========================================================================*/

uint16 hdrsrchsleep_get_scc_offset( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.scc_offset );

} /* hdrsrchsleep_get_scc_offset */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_SET_SCC_OFFSET

DESCRIPTION
  This function sets scc offset.

DEPENDENCIES
  None

PARAMETERS
  scc_offset - SCC offset
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrchsleep_set_scc_offset
(
  uint16                scc_offset
    /* SCC offset */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.scc_offset = scc_offset;
    /* Set SCC offset */

} /* hdrsrchsleep_set_scc_offset */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_SET_SCC_INFO

DESCRIPTION
  This function sets scc offset and rate.

DEPENDENCIES
  None

PARAMETERS
  scc_offset - SCC offset
  scc_rate   - SCC rate
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrchsleep_set_scc_info
(
  uint16                scc_offset,
    /* SCC offset */

  uint8                 scc_rate
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchsleep.scc_offset = scc_offset;
    /* Set SCC offset */

  hdrsrchsleep.scc_rate = scc_rate;
    /* Set SCC rate */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Set scc_offset %d scc_rate %d", scc_offset, scc_rate);

} /* hdrsrchsleep_set_scc_info */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_SCC_RATE

DESCRIPTION
  This function gets scc rate.

DEPENDENCIES
  None

PARAMETERS
  None
RETURN VALUE
  scc rate

SIDE EFFECTS
  None
  
==========================================================================*/

uint8 hdrsrchsleep_get_scc_rate( void )
{
  return hdrsrchsleep.scc_rate;
} /* hdrsrchsleep_get_scc_rate */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_SEND_MSG_UPDATE_SCC_OFFSET

DESCRIPTION
  This function sends the message to the firmware to update scc offset.

DEPENDENCIES
  None

PARAMETERS
  scc_offset - SCC offset
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrchsleep_send_msg_update_scc_offset( void )
{
  hdrsrchsleep_fw_msg_struct_type            fw_msg;
    /* Messages sent to firmware */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset( &fw_msg.cc_config_msg,
          0,
          sizeof( hdrfw_scc_msg_t ) );
    /* Clear the message */

  msgr_init_hdr( &fw_msg.cc_config_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_SCC_CMD );
    /* Init the message header */

  fw_msg.cc_config_msg.sccOffset = hdrsrchsleep.scc_offset;
  fw_msg.cc_config_msg.sccOffset_Valid = 1;
    /* Set SCC offset in the message payload */

  hdrsrchmsg_send_msg( &fw_msg.cc_config_msg.hdr,
                       sizeof( hdrfw_scc_msg_t ) );
    /* Msgr copies the message and hence ok to use local variable */

} /* hdrsrchsleep_send_msg_update_scc_offset */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_LAST_SLEEP_SCI

DESCRIPTION
  This function returns the value of sci used for last sleep interval.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value of sci used for last sleep interval. 

SIDE EFFECTS
  None
  
==========================================================================*/

uint8 hdrsrchsleep_get_last_sleep_sci( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.until );

} /* hdrsrchsleep_get_last_sleep_sci */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_NEGOTIATED_SCI

DESCRIPTION
  This function returns the value of negotiated sci 

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value of negotiated sci. 

SIDE EFFECTS
  None
  
==========================================================================*/

uint8 hdrsrchsleep_get_negotiated_sci( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.neg_sci );

} /* hdrsrchsleep_get_negotiated_sci */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_INTRA_SCC_VALID

DESCRIPTION
  This function returns intra-SCC sleep is enabled or not.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value intra-SCC start slot valid flag, i.e., TRUE is intra-SCC is enabled

SIDE EFFECTS
  None
  
==========================================================================*/

boolean hdrsrchsleep_get_intra_scc_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.intra_scc_start_slot_valid );

} /* hdrsrchsleep_get_intra_scc_valid */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_INTRA_SCC_START_SLOT

DESCRIPTION
  This function returns intra-SCC start slot.  Return 0 if
  hdrsrchsleep.intra_scc_start_slot_valid is FALSE.

DEPENDENCIES

PARAMETERS
  None
  
RETURN VALUE
  Value of intra-SCC start slot

SIDE EFFECTS
  None
  
==========================================================================*/

uint16 hdrsrchsleep_get_intra_scc_start_slot( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchsleep.intra_scc_start_slot_valid == TRUE )
  {
    return ( hdrsrchsleep.intra_scc_start_slot );
  }
  else
  {
    return ( 0 );
      /* Intra-SCC is disabled. */
  }

} /* hdrsrchsleep_get_intra_scc_start_slot */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_IS_QPCH_ENABLED

DESCRIPTION
  This function returns TRUE if QPCH is enabled, FALSE otherwise.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  TRUE  - if AT should monitor QPCH;
  FALSE - if AT is not supposed to monitor QPCH.

SIDE EFFECTS
  None
  
==========================================================================*/

boolean hdrsrchsleep_is_qpch_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.qpch_enabled );

} /* hdrsrchsleep_is_qpch_enabled */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_SLEEP_SLOTS

DESCRIPTION
  This function returns the value of sleep slots. 

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value of sleep slots. 

SIDE EFFECTS
  None
  
==========================================================================*/

uint32 hdrsrchsleep_get_sleep_slots( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.sleep_slots );

} /* hdrsrchsleep_get_sleep_slots */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_SLEEP_SUB_STATE

DESCRIPTION
  This function returns the value of the current sleep sub state.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  The value of current sleep sub state (hdrsrch_sleep_sub_state_enum_type). 

SIDE EFFECTS
  None
  
==========================================================================*/

hdrsrch_sleep_sub_state_enum_type hdrsrchsleep_get_sleep_sub_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ( hdrsrchsleep.sleep_sub_state );

} /* hdrsrchsleep_get_sleep_sub_state */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_SET_SLEEP_SUB_STATE

DESCRIPTION
  This function sets the value of the current sleep sub state.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  None 

SIDE EFFECTS
  None
  
==========================================================================*/

void hdrsrchsleep_set_sleep_sub_state
( 
  hdrsrch_sleep_sub_state_enum_type sleep_sub_state
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2(
    MSG_LEGACY_HIGH, "HDR setting sleep sub-state from: %d to: %d",
    hdrsrchsleep.sleep_sub_state,sleep_sub_state );

  hdrsrchsleep.sleep_sub_state = sleep_sub_state;

} /* hdrsrchsleep_set_sleep_sub_state */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_SET_SLEEP_TIMELINE_DEFAULT

DESCRIPTION
  This function sets the sleep timeline optmized or extended.

DEPENDENCIES
  None

PARAMETERS
  None
  
RETURN VALUE
  TRUE:  optimized timeline
  FALSE: extended timeline

SIDE EFFECTS
  None
  
==========================================================================*/
void hdrsrchsleep_set_sleep_timeline_default
(
  boolean            optimized
    /* Whether to use optimized sleep timeline */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "Set sleep timeline from: %d to: %d (extended 0, optimzed 1)",
                  hdrsrchsleep.rpm_warmup_time_is_optimized, optimized );

  hdrsrchsleep.rpm_warmup_time_is_optimized = optimized;

}  /* hdrsrchsleep_set_sleep_timeline_default */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_TIME_UNTIL_TUNE_COMPLETE

DESCRIPTION
  If the HDR tune is happening within 1x tune duration, return the time that tune will be 
  completed. Otherwise, return 0ms.

DEPENDENCIES
  None

PARAMETERS
 tune_duration_1x: 1x tune duration in usec

RETURN VALUE
  0: There is no conflict between 1x and HDR RF tune.
  >0 : There is conflict and the time in usec until the HDR tune completes 

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchsleep_time_until_tune_complete
(
  uint32 tune_duration_1x_us
    /* 1x tune duration in usec*/
)
{
  uint32           time_to_rtc_on_us = 0;
    /* Time in sclk until to rtc on */

  uint32           time_to_wakeup_us = 0;
    /* Time in usec until to wake up */

  uint32           time_to_complete_us = 0;
    /* Time in usec until to complete tune */

#ifdef FEATURE_HDR_DIME_MODEM
  uint64            time_slept_cx8;
    /* time already slept */
#endif /* FEATURE_HDR_DIME_MODEM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( ( hdrsrch_state_id == HDRSRCH_SLEEP_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_REACQ_STATE ) )
  {
#ifdef FEATURE_HDR_DIME_MODEM
   time_slept_cx8 = slpc_get_sysclk_count( SLPC_HDR );
     /* time that we have already slept, in cx8 */

   time_to_rtc_on_us = hdrsrchsleep.sleep_duration_cx8 - time_slept_cx8;
     /* Get time to online interrupt. Now it is in cx8 */
   time_to_rtc_on_us = (time_to_rtc_on_us >> 3)*625/768;
     /* Convert it to micro second. 1 cdma chip = 1000/1228.8 us = 625/768 us */
   time_to_wakeup_us = time_to_rtc_on_us - hdrsrchsleep.rf_warmup.cur_warmup_time_us;
     /* Get time to warmup in us */
#else
    time_to_wakeup_us = timetick_sclk_to_prec_us( sleepctl_get_sclk_till_wakeup( SLEEPCTL_HDR ) );

    time_to_rtc_on_us = timetick_sclk_to_prec_us( sleepctl_get_sclk_till_rtc_on( SLEEPCTL_HDR ) );
#endif /* FEATURE_HDR_DIME_MODEM */
    if ( time_to_rtc_on_us == 0 )
    {
      time_to_complete_us = 1000 * SLOTS_TO_MS( HDRSRCHSLEEP_REACQ_TIME_SLOTS );
        /* HDR sleep ended but we are in sleep/reacq state,
           no 1x rf tune until reacq time passed. */
    }
    else
    if ( time_to_wakeup_us == 0 )
    {
      time_to_complete_us = hdrsrchsleep.rf_warmup.cur_warmup_time_us + 
                            1000 * SLOTS_TO_MS( HDRSRCHSLEEP_REACQ_TIME_SLOTS );
        /* HDR woke up but we are in sleep/reacq state,
           no 1x rf tune until warmup and reacq time passed. */
    }
    else
    if ( time_to_wakeup_us < tune_duration_1x_us )
    {
      time_to_complete_us = time_to_wakeup_us + 
                            hdrsrchsleep.rf_warmup.cur_warmup_time_us +
                            1000 * SLOTS_TO_MS( HDRSRCHSLEEP_REACQ_TIME_SLOTS );
        /* There is overlap and 1x need to wait */
    }
  }

  HDR_MSG_SRCH_4(MSG_LEGACY_HIGH, 
                 "time_to_wakeup_us %d, time_to_rtc_on_us %d, tune_duration_1x_us %d, time_to_complete_us %d ",
                 time_to_wakeup_us, time_to_rtc_on_us, tune_duration_1x_us, time_to_complete_us );

  return time_to_complete_us;
} /* hdrsrchsleep_time_until_tune_complete */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_GET_CUR_WARMUP_REACQ_TIME

DESCRIPTION
  This function returns the sum of the current warmup time and reacq time.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  warmup + reacq time in us 

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchsleep_get_cur_warmup_reacq_time( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( hdrsrchsleep.rf_warmup.cur_warmup_time_us + 
           SLOTS_TO_MS( HDRSRCHSLEEP_REACQ_TIME_SLOTS ) * 1000 );

} /* hdrsrchsleep_get_cur_warmup_reacq_time */


/*==========================================================================

FUNCTION hdrsrchsleep_get_cur_warmup_time

DESCRIPTION
  This function returns the current warmup time.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  warmup in us 

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchsleep_get_cur_warmup_time( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( hdrsrchsleep.rf_warmup.cur_warmup_time_us );

} /* hdrsrchsleep_get_cur_warmup_time */


/*==========================================================================

FUNCTION HDRSRCHSLEEP_DUMP_TIME

DESCRIPTION
  This function prints out current ustmr and hdr rtc

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchsleep_dump_time( void )
{
  uint32                          hdr_rtc_cx2;
  uint32                          ustmr_rd;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_BOLT_MODEM
  hdr_rtc_cx2 = hdrts_get_rtc_cx2( );
  ustmr_rd = 0;
#else
  HWIO_UNIV_STMR_STATUS_DUMP_CMD_OUT( \
    HWIO_UNIV_STMR_STATUS_DUMP_CMD_STATUS_DUMP_UP_BMSK );

  hdr_rtc_cx2 = HWIO_RTC_HDR_TIME_STATUS_RD_IN >> 4;
  ustmr_rd  = HWIO_UNIV_STMR_TIME_STATUS_RD_IN;
#endif /* FEATURE_HDR_BOLT_MODEM */

  HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, "HDR dump time: rtc_cx2=0x%x, ustmr=0x%x",
                 hdr_rtc_cx2, ustmr_rd);
} /* hdrsrchsleep_dump_time */

/*==========================================================================

FUNCTION HDRSRCHSLEEP_UPDATE_PREV_CC_START

DESCRIPTION
  This function prints updates prev_cc_start with cc_start

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None 

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchsleep_update_prev_cc_start(void)
{
  hdrsrchsleep.prev_cc_start = hdrsrchsleep.cc_start;
   /* Save previous CC start for enh sleep logging calculations. */
} /* hdrsrchsleep_update_prev_cc_start */

#ifdef FEATURE_HDR_WAKEUP_MANAGER
/*=========================================================================

FUNCTION    HDRSRCHSLEEP_WAKEUP_MANAGER_CB

DESCRIPTION
  Call back to inform HDR about MDSP download and RF warm-up information
  for the next wakeup and about any collision during the next wakeup.

DEPENDENCIES
  None

INPUTS  
  rat - RAT ID.
  result -The enum with the result for wakeup.
  transition - Layer1 transitions.
  reserved -  Reserved for enhancements.
  user_data- User defined data if any. 

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchsleep_wakeup_manager_cb
(
  wmgr_client_enum_t rat,
    /* RAT ID */
  
  wmgr_result_type result,
    /* The enum with the result */
  
  uint32 transition,
    /* Layer1 transitions */
  
  uint32 reserved,
    /* Reserved for enhancements */
  
  void *user_data
    /* User defined data */
)
{
  int32                          num_extension_slots = 0;
    /* The number of extension slots to be used if we can't wakeup. */
  uint64                         now_xo_ticks=0;
    /*  variables to store current xo ticks */
  uint64                         wakeup_xo_ticks=0;
    /*  variables to store wakeup xo ticks */
  boolean                        sleep_state = FALSE;
    /* If in sleep or not */
  hdrsrch_cmd_type               *cmd;
    /* Command structure */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Check for RAT ID */
  if ( rat != WMGR_CLIENT_HDR)
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "Invalid! Not for HDR, ignore wakeup mgr update" );
    return;
  }

  sleep_state = hdrsrchsleep.sleeping;

  /* Check if in sleep state */
  if ( ( hdrsrch_state_id != HDRSRCH_SLEEP_STATE ) || 
                     ( sleep_state == FALSE ) )
  
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                  "Not in sleep, skip wakeup mgr update" );
    return;
  }

  /* ignore wakeup manager event if it is close to wakeup point */
  now_xo_ticks     = slpc_get_tick();
  wakeup_xo_ticks = slpc_get_wakeup_tick(SLPC_HDR);
  if ( ( (int32) ( wakeup_xo_ticks - now_xo_ticks ) < HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK ) )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Ignore Wmgr event: Too close to wakeup to adjust %d < %d",
                    (uint32)( wakeup_xo_ticks - now_xo_ticks ),
                    HDRSRCHSLEEP_ADJUSTMENT_MARGIN_XO_TICK );
    return;
  }
  
  switch(result)
  {
    case WMGR_RESULT_WAKEUP_CANCEL:
    {
      /* HDR cannot wakeup in the current page slot so:
       - Reprogram Sleep controller to the next page slot
       - Make a new TRM reservation
      */

      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "Wakeup mgr update:WAKEUP CANCEL extending sleep" );   
      
      /* Determine how many slots sleep should be extended if we can't wakeup.*/
      if ( hdrsrchsleep.until >= 6 )
      {
        /* Not 213 ms sleep cycle */
        num_extension_slots = HDRTS_SLOTS_PER_SCC;
      }
      else
      {
        /* Must be 213 ms sleep cycle */
         HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                      "Wakeup mgr update: Incorrect sleep cycle!!!" );
                
      }

      if( num_extension_slots > 0 )
      {
        sleep_state = hdrsrchsleep.sleeping;

        /* Check if in sleep state */ 
        if ( sleep_state == TRUE ) 
        {
            cmd = hdrsrchcmd_new( HDRSRCH_EXTEND_SLEEP_CMD );
              /* Command HDRSRCH_TASK to change sleep timeline */

            if ( cmd != NULL )
            {
              
              cmd->params.extend_sleep.num_extension_slots = num_extension_slots;

              hdrsrchcmd_post( cmd );
                /* Post the command */
             }
        }
        else
        {
          HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                        "Not in sleep, skip wakeup mgr update" );

        }
      }
    }
    break;

    case WMGR_RESULT_MDSP_DLOAD_RF_WARMUP_REQ:
    case WMGR_RESULT_RF_WARMUP_REQ:
    case WMGR_RESULT_MDSP_DLOAD_RF_WARMUP_NOT_REQ:
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                        "Wakeup mgr update: Received %d --> No action", result );
     
      break;
    
    default:
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                    "Wakeup mgr update: Unrecognized update!!! %d",result );
    }
  }
  
} /* hdrsrchsleep_wakeup_manager_cb( ) */
#endif /* FEATURE_HDR_WAKEUP_MANAGER */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
/*============================================================================

FUNCTION HDRSRCHSLEEP_RESERVE_TRM_AT

DESCRIPTION
  Specifies that the caller needs the given RF resource at the given
  time, for the given duration, for the supplied reason.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the caller currently holds an RF chain, the chain is released.

============================================================================*/
void hdrsrchsleep_reserve_trm_at 
(
  hdrsrchrf_logical_chain_enum_t        chain_id,
    /* The logical chain ID associated with the request */

  hdrsrchrf_resource_enum_t             resource,
    /* The RF resource which is being requested */

  hdrsrchrf_time_t                      when,
    /* When the resource will be needed (sclks timestamp) */

  hdrsrchrf_duration_t                  duration
    /* How long the resource will be needed for (in sclks) */
)
{
  boolean                         wakeup_at_neg_sci_boundary = FALSE;
   /* Flag to see if the wakeup is at the SCI boundary */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchsleep.intra_scc_start_slot_valid == FALSE )
  {
    if ( hdrsrchsleep.session_is_open == FALSE || hdrsrchsleep.ovhd_is_updated == FALSE )
    {
      wakeup_at_neg_sci_boundary = hdrsrchsleep_is_wakeup_at_neg_sci_boundary();
      if ( ( hdrsrchsleep.ovhd_denied_cnt >= HDRSRCHSLEEP_MAX_ALLOWED_OVHD_DENIALS ) &&
	   ( hdrmultirat_get_mode_pref() != HDRMULTIRAT_DUAL_SIM_DUAL_ACTIVE ) )
      {
        hdrsrchsleep.trm_reason = HDRSRCHRF_OVHD_HIGH; 
        HDR_MSG_SRCH_3 (MSG_LEGACY_HIGH, "Not Intra-SCC OVHD_HIGH, Session_open=%d, Ovhd_updated=%d, Denied_Cnt=%d" , hdrsrchsleep.session_is_open, hdrsrchsleep.ovhd_is_updated, hdrsrchsleep.ovhd_denied_cnt ); 
      }
      else if ( wakeup_at_neg_sci_boundary == TRUE )
      {
        hdrsrchsleep.trm_reason = HDRSRCHRF_DEMOD_PAGE; 
        HDR_MSG_SRCH_4 (MSG_LEGACY_HIGH, "Not Intra-SCC DEMOD_PAGE, Session_open=%d, Ovhd_updated=%d, Denied_Cnt=%d, wakeup_at_sci=%d " , hdrsrchsleep.session_is_open, hdrsrchsleep.ovhd_is_updated, hdrsrchsleep.ovhd_denied_cnt, wakeup_at_neg_sci_boundary ); 
      }
      else
      {
        hdrsrchsleep.trm_reason = HDRSRCHRF_OVHD;
        HDR_MSG_SRCH_4 (MSG_LEGACY_HIGH, "Not Intra-SCC OVHD, Session_open=%d, Ovhd_updated=%d, Denied_Cnt=%d, wakeup_at_sci=%d " , hdrsrchsleep.session_is_open, hdrsrchsleep.ovhd_is_updated, hdrsrchsleep.ovhd_denied_cnt, wakeup_at_neg_sci_boundary ); 
      }
    }
    else 
    {
      hdrsrchsleep.trm_reason = HDRSRCHRF_DEMOD_PAGE; 
      hdrsrchsleep.ovhd_denied_cnt = 0;
      HDR_MSG_SRCH_2 (MSG_LEGACY_HIGH, "Not Intra-SCC DEMOD_PAGE, Session_open=%d Ovhd_updated=%d", hdrsrchsleep.session_is_open, hdrsrchsleep.ovhd_is_updated ); 
    }
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Calling hdrsrchrf_reserve_at with reason=%d", hdrsrchsleep.trm_reason);  
  hdrsrchrf_reserve_at ( chain_id, resource, when, duration, hdrsrchsleep.trm_reason );    
} /* hdrsrchsleep_reserve_trm_at  */


/*============================================================================

FUNCTION HDRSRCHSLEEP_IS_WAKEUP_AT_NEG_SCI_BOUNDARY

DESCRIPTION

  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

============================================================================*/
boolean hdrsrchsleep_is_wakeup_at_neg_sci_boundary( void )
{
  uint16						  neg_sci_in_slots = 0;
   /* Negotiated SCI in slots */
	
  uint16						  diff_in_slots = 0xFFFF; 
   /* Difference between wakeup and SCI boundary in slots */
	
  uint16						  po_do_slots = 0xFFFF; 
   /* Difference between wakeup and SCI boundary in slots */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
  if ( hdrsrchsleep.neg_sci <= 0x06 )
  {
	neg_sci_in_slots =	( HDRTS_SLOTS_PER_SCC >> (0x06 - hdrsrchsleep.neg_sci) );
  }
  else
  {
	neg_sci_in_slots = ( HDRTS_SLOTS_PER_SCC * (3 * ( 1 << ( hdrsrchsleep.neg_sci - 7 ) ))); 
  }
	  
  diff_in_slots = hdrsrchsleep.wake_slot % neg_sci_in_slots;
	
  po_do_slots = ((HDRTS_SCC_PER_CCC - hdrsrchsleep.control_channel_cycle) * HDRTS_SLOTS_PER_SCC) % neg_sci_in_slots;
	
  diff_in_slots = abs(diff_in_slots - po_do_slots);
	
  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, 
				  "neg_sci=%d neg_sci_in_slots=%d wake_slot=%d po_do_slots=%d diff_in_slots=%d", 
				   hdrsrchsleep.neg_sci, neg_sci_in_slots, hdrsrchsleep.wake_slot, po_do_slots, diff_in_slots );
	  
  return ( (diff_in_slots < HDRSRCHSLEEP_MAX_ALLOWED_SLOTS_FROM_A_SCI_BOUNDARY )  );
}
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

#ifdef FEATURE_HDR_PBR_RANDOMIZATION
/*============================================================================

FUNCTION HDRSRCHSLEEP_PBR_TRM_UPDATE

DESCRIPTION
  Specifies that the caller needs the given RF resource at the given
  time, for the given duration, for the supplied reason.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  If the caller currently holds an RF chain, the chain is released.

============================================================================*/
void hdrsrchsleep_pbr_trm_update
(
  boolean                               wakeup_failed
)
{
  hdrsrchrf_pbr_grant_event_enum_t  grant_event;
    /* Grant event needs to be shared with TRM */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  grant_event = ( wakeup_failed == TRUE ) ? ( HDRSRCHRF_PBR_RF_DENIED ) : ( HDRSRCHRF_PBR_RF_GRANTED );
    /* Based on chain grant for page demod update status to TRM */

  if ( hdrsrchsleep.trm_reason == HDRSRCHRF_DEMOD_PAGE )
  {
    hdrsrchrf_update_rf_grant_event(
     hdrsrchsleep.trm_reason, 
     hdrsrchsleep_get_unique_wakeup_id(),
     grant_event );
  }/* if ( hdrsrchsleep.trm_reason == HDRSRCHRF_DEMOD_PAGE ) */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
    "PBR: Wakup failed? %d, trm_reason %d, wakeup_id %d", 
    wakeup_failed, hdrsrchsleep.trm_reason, hdrsrchsleep_get_unique_wakeup_id() );
} /* hdrsrchsleep_pbr_trm_update  */
#endif /* FEATURE_HDR_PBR_RANDOMIZATION */


/*============================================================================
FUNCTION HDRSRCHSLEEP_GET_UNIQUE_WAKEUP_ID

DESCRIPTION 
  Return a unique id for every paging cycle to TRM 

DEPENDENCIES 
  None

RETURN VALUE
  Return unqiue wakeup id for every paging cycle

SIDE EFFECTS


============================================================================*/

uint64 hdrsrchsleep_get_unique_wakeup_id( void )
{


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ( hdrsrchsleep.wakeup_id );

} /* hdrsrchsleep_get_unique_wakeup_id */

/*============================================================================

FUNCTION HDRSRCHSLEEP_WAKEUP_ID_INIT

DESCRIPTION 

  Initialize wakeup_id

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS

============================================================================*/
void hdrsrchsleep_wakeup_id_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchsleep.wakeup_id = 0;

} /* hdrsrchsleep_wakeup_id_init */


/*============================================================================
FUNCTION HDRSRCHSLEEP_GET_SLEEP_DURATION

DESCRIPTION 
  Returns the sleep duration till this instance

DEPENDENCIES
  None

RETURN VALUE
  Sleep duration till now in chipx1

SIDE EFFECTS
  None
============================================================================*/
uint32 hdrsrchsleep_get_sleep_duration( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  uint32 chip_x1 = 0;
  /* Time, in chipx1  */

  REX_DISABLE_PREMPTION();
  chip_x1 = slpc_get_sysclk_count( SLPC_HDR ) >> 3;
    /* get the duration for which the HDR sleept so far in chipX1 from chipX8 */
  REX_ENABLE_PREMPTION();

  return chip_x1;
} /* hdrsrchsleep_get_sleep_duration */


/*============================================================================
FUNCTION HDRSRCHSLEEP_GET_TIME_TO_NEXT_WAKEUP

DESCRIPTION 
  Returns the time remaining to next wakeup in ticks

DEPENDENCIES
  None

RETURN VALUE
  time remaining to next normal wakeup in ticks

SIDE EFFECTS
  None
============================================================================*/
int32 hdrsrchsleep_get_time_to_next_wakeup( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  uint64  now_xo_ticks=0;     // current XO ticks
  uint64  wakeup_xo_ticks=0;  // expected wakeup time in XO ticks
  int32   diff_ticks = 0;

  now_xo_ticks     = slpc_get_tick();
  wakeup_xo_ticks  = slpc_get_wakeup_tick(SLPC_HDR);

  diff_ticks = (int32) ( wakeup_xo_ticks - now_xo_ticks );

  HDR_MSG_SRCH_3(MSG_LEGACY_MED, "Debug: now %d expected %d diff %d",
      now_xo_ticks,
      wakeup_xo_ticks,
      diff_ticks);

  if (diff_ticks < 0) // Diff ticks cannot be negative
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "HDR yet to wakeup %d", diff_ticks);
  }

  return diff_ticks;
} /* hdrsrchsleep_get_time_to_next_wakeup */


#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
/*============================================================================
FUNCTION HDRSRCHSLEEP_GET_TRM_REASON

DESCRIPTION 
  Returns the trm reason based on the command passed as parameter

DEPENDENCIES
  None

RETURN VALUE
  TRM reason

SIDE EFFECTS
  None
============================================================================*/
LOCAL hdrsrchrf_reason_enum_t hdrsrchsleep_get_trm_reason
(
  hdrsrch_cmd_name_enum_type cmd_name
)
{
  hdrsrchrf_reason_enum_t trm_reason;

  switch ( cmd_name )
  {
    case HDRSRCH_ACCESS_CMD:
    case HDRSRCH_TC_ASSIGNMENT_CMD:
      /* perform TCA tune ASAP and hence use access reason */
      trm_reason = HDRSRCHRF_ACCESS;
      break;

    case HDRSRCH_LTE_TRESELECT_EXP_CMD:
      /* TRM Priority of  HDRSRCHRF_IRAT_MEASUREMENT 
      and HDRSRCHRF_OVHD are same and avoid changing 
      to IRAT RF reason in sleep state */
    case HDRSRCH_IDLE_CMD: 
      /* Missed the timeline need to update overheads */
    case HDRSRCH_PILOT_MEAS_CMD: 
      /* RUP measurement and CM reason is mapped to OVHD */
    case HDRSRCH_CHANNEL_CHANGE_CMD:
    case HDRSRCH_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD:
      /* CM reason is mapped to OVHD */
    case HDRSRCH_SLEEP_CMD:
      /* perform fast wakeup and OVHD reason */
    default:
      trm_reason = HDRSRCHRF_OVHD;
      break;
  }

  return trm_reason;
}/* hdrsrchsleep_get_trm_reason */
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */

