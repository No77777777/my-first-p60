/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          L 1   D E D I C A T E D - I N T E R F A C E

GENERAL DESCRIPTION
   This module contains the procedures to handle the task level processing
   of the dedicated channels.

EXTERNALIZED FUNCTIONS
  L1_handle_dedicated_mode
    This function handles the L1 Task in dedicated mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_ded_if.c#2 $
  $DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who    what, where, why
--------   ---    ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
20/08/18   sk     CR2181447 Opt for fading cells reconf scheduling in dedicated mode
11/12/17   mk     CR2152940: Revert CR 2001095 and related CR changes
03/05/18   rv     CR2233364 : Add feature flags for FR38904 changes - GERAN
07/02/17   mk     CR2001059: VFR release and vocoder stop should be performed when STOP_GSM_MODE_REQ is received
11/04/17   rv     CR2031827 Excessive F3 log reduction for Oppo
30/06/17   sn     CR2059605 Handling of MAC_L1_RELEASE during Transition stage from Dedicated to DTM
20/03/17   mk     CR2020999 Prx Drx power difference needs to be calculated & applied to NCELLs before REP_PERIOD_IND to avoid delays
07/03/17   mk     CR2015800 Reset "l1_ded_prx_drx_pwr_diff" value when diversity is disabled
30/08/16   tsk    CR1026504: Update to 0x5096 RxD log packet with additional details.
10/08/16   snjv   CR1050221 Remove usage of Feature_gsm_rx_tx_split
06/03/17   snjv   CR2015632 Initialize local var before usage for change with CR1113152
30/01/17   mk     CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
30/05/17   rv     CR2031381: L1 fix to address issues with CR2001095 causing GCF failures
11/02/16   zf     CR975144: Release TRM when leaving DED to NULL state
08/12/16   mk     CR1097591 Enhancement for DRX: DRx and PRx difference will be applied to neighbor cell measurements, if DRx power level is greater than PRx
08/12/16   mk     CR1097205: Fixing AGC initialisation when Rxd is getting enabled and enhancing serving cell measurement reporting when Rxd was partially enabled in a SACCH period
07/12/16   mk     CR1094221 Blind HO enhancement - pass target cell BSIC to validate the BSIC decoded in SCH acquisition
25/07/16   sm     CR1045062 Resolved run time error in QTF : variable “gas_id “ is being using without initialization.
07/07/16   km     CR975036 Adjust qbit lag while hand over
27/05/16   snjv   CR1000909 pass ACI400 switchpoint detection stat from GFW to GRF via GL1
30/06/16   mk     CR957925  Do not send DTX disable to vocoder blindly in HO scenario which impacts voice quality
29/06/16   hd     CR1035604 Send a message to L1 task to turn tx off when dedicated channel is released.
06/07/15   mn     CR1024054 Disabled tx chain , While moving from L1_dedicated  to L1_NULL
18/04/16   hd     CR1004583 Vocoder and VFR should be stopped when channel release complete is done
10/03/16   dg     CR986989 - Re-work CR798863 After X2G HO is successful, don't adjust G frame number when G2X Handover is triggered
14/10/15   snjv   CR919955 FR38046:QSH Call End Data Collection
11/09/15   br     CR903740 Add mutex protection for rach abort,to prevent removal of handler while handler is executing
01/09/15   hd     CR899231 Remove the rach_tx_handler once MPH_RECONNECT_CHANNEL_REQ is received
14/07/15   cjl    CR871913 Release Tx/Rx devices after leaving dedicated mode
17/06/15   cjl    CR853555 Use dynamic connected mode reasons for CS/PS traffic
04/06/15   jk     CR846952:RxLEV report should be based on maximum of PRx and DRx average RSSIs
25/06/15   cjl    CR855143 Release TX chain when leaving dedicated and entering FIND BCCH
25/06/15   dg     CR798863 Adjust GSTMR back to its previous value after X2G HO Fail
30/06/15   ws     CR852519 - Resolve GL1 enum conversion compile warnings
15/06/15   pg     CR852376 Keep Voice AEQ clock bump on to avoid timeline overruns
11/05/15   pjr    CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
29/04/15   npt    CR804870 RF timeline error recovery mechanism
21/04/15   cs     CR808438 Ensure any pending TT requests are sent before cell
                  information is updated
08/04/15   pjr    CR818535 FR25951 Changes to support Single Voice Session
17/03/15   shr    CR808932: COEX support for DTM
13/03/15   pjr    CR797534 Ensure DTX mode is not enabled by an Update req during loopback
23/02/15   br     CR794803:UE not moving to a new  CS channel after recieving MAC_L1_DTM_CHANNEL_ASSIGN_REQ  which has cs reconfiguration.
27/02/15   am     CR646584 WLAN coex
30/01/15   ws     CR785980 - Add handling of MAC_L1_RELEASE in Dedicated mode to
                  catch any Synchronization issues in DTM between GL1 and GMAC
21/01/15   jj     CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK
16/12/14   jk     CR753423:LIF for Idle,voice and data
19/11/14   jk     CR758043:Don't disable AEQ while in SDCCH even if RxQUAL is good
07/11/14   jj     CR 752569 Fix thor bring up issues
19/08/14   pjr    CR711506 Support for AMR Partial Decode and DTX detection FR19667
11/09/14   ws     CR723099 Resolve GL1 compile warnings
28/05/14   ap     CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
17/07/14   pjr    CR672464 Removed fake VFR realignment sequence
02/07/14   cos    CR 688739 - Residual BER in RxD mode when PRx is disconnected
24/06/14   dg     CR683089 Rearranging if conditions inside function gl1_save_current_cell_data to
                  avoid errors with customer's compiler
29/07/14   ap     CR695234: Changes for GL1's New Rach interface
10/07/14   cgc    CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
12/05/14   hd     CR658294 Ignore TA if not in the range 0 - 63 transmitted in MPH_STOP_HANDOVER_ACCESS_REQ
02/07/14   cos    CR689665 AEQ PHASE 4 - Ignoring invalid measurements and invalid channels during call transition periods
17/06/14   cgc    CR680138 service LTE msg's while suspending add gl1_handle_message_router_msgs()
18/06/14   cos    CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
30/05/14   ws     CR657674 Data + MMS Phase 1
07/04/14   cja    CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14    cah     CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
04/04/14   sai    CR638388: Delay TBF re-establish in DSDS if G2W resel ongoing on other sub.
27/03/14   cs     CR581227 Always send Pre-Cell GPS Timetag during channel release
19/02/14   jk     CR608290:GERAN RxD ON/OFF adaptive algorithm
06/02/14   pg     CR610729: Correct for negative TA during PSEUDO_SYNCHRONISED HO
06/02/14   am     CR594685 Change timeslot before releasing the vocoder
24/01/14   cah    CR598552: enable sleep for DSDS GSTMR+VSTMR
17/01/14   cgc    CR600002 propagate missing code from CR504088, support for SGLTE
28/11/13   cgc    CR581186 Desense SYS_PARAM_MIN_ACCEPTABLE_RX_LVL in SGLTE mode
18/11/13   pjr    CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
28/11/13   cja    CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
22/11/13   cja    CR455527 Remove featurisation from files in API directory
23/10/13   pa     CR565614:Copied 128-bit ciphering key to internal data structure.
14/10/13   pa     CR536820:Added GL1 Msgr IF Task.
07/16/13   zc     CR470197 Replace memcpy with memscpy
24/07/13   jk     CR464982:Changes to support GEA5_4 ciphering
23/07/13   og     CR512476. DL TBF DTM assignment not getting connected in DTM mode
                  when the freq config message is not sent beforehand.
21/06/13   cs     Major Triton TSTS Syncup
24/05/13   ap     CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
20/05/13   pjr    CR487402: dual data spaced stored_dtx_indicator and l1_target_ncell_unavailable
20/05/13   ap     CR475498  Gl1 stuck aborting TCH
11/06/13   cs     TSTS Updates
10/04/13   pg     CR469228 Avoid schedule two tx band events
09/04/13   pjr    CR470217 Ported next_params.L1Data.pDedi_data asignments from DIME
10/04/13   zf     CR472103: Transfer isr task params when handover
03/04/13   pjr    CR470940 included gl1_hw_vs_g.h
02/04/13   ss     CR470255 Process Async messages sent from GFW
07/03/13   dv     CR461080: Introduction of new GSM VS interface
22/11/12   cja    CR414098 Tidy-up F3 for MCPM builds
09/11/12   pg     CR416728: Updates for WG IRAT on Dime modem
17/08/12   ip     CR384421 Disable GSM sleep during CCO activity
08/06/12   pa     GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
25/05/12   pg     Add support for RF Task
19/03/11   rc     CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
13/03/12   ws     CR335057 eSACCH buffer flush and restore after inter-cell handovers
06/03/12   pg     CR319120: Ensure latest DTX setting gets applied after vocoder init
10/02/12   ws     CR 336194 Enable TD-SCDMA IRAT for GERAN
10/01/12   cja    CR327967 Add request MCPM AEq on for FR
28/09/11   dv     CR309545 GL1 should restore previous DTX setting following transition to DTM
06/09/11   cja    CR305205 On reconnection delay new cell by 1 frame.
08/04/11   og     CR280835. Add mutex protection as a replacement for the fix of CR276511.
04/04/11   cja    Fix compiler error in merge for CR280412.
24/03/11   cja    CR280412 For H/O failure remove time tracking on failed cell
22/02/10   og     CR257655. Lock the mutex when processing the channel release req.
27/01/11   dmb    The following RR<->L1 messages are removed (not used):
                  MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
13/09/10   cja    CR251985. If returning to idle from SDCCH to reconnect do not run frame handlers.
07/09/10   ws     CR 252093 - Eng mode Rxqual fixes for PTM and enabling in dedicated
14/06/10   ap     CR 235196 3GPP:Cipher mode needs adding to DTM_ASSIGNNMENT message
20/05/10   ap     CR 241452  GL1 is trying to associate starttime with CMM request
20/05/10   ap     CR 238752  fix problem with DTM frame inc and reconnect
03/03/10   ab     Code Featurization for FEATURE_GSM_DTM and FEATURE_GSM_EDTM
08/02/10   cja    XO update
09/09/09   cs     Fix Klocwork bounds checking error
09/07/09   ab     CR 183589 Incorrect handling of RLC mode change request during extend
                  UL TBF operation
27/04/09   ws     CR 180552 - Force cv_bep =7 when mean_bep = 31 to allow for
                  snr estimate variation
24/03/09   dv     CR176933- Call mute after ending Loopback C Test
30/03/09   ws     Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                  to replace FEATURE_GSM_GPRS_MUSTANG
13/03/09   og     FEATURE_XO updates.
15/01/09   nt     CR167218 - changes to restore old rot and pdm values after HO fails
13/01/08   ap     CR165855 PH_CONNECT_IND given to MAC for UL assignment without UE receiving it
06/12/08   cs     Include gl1_hw_debug.h for new split gl1_hw.h
03/12/08   ws     Added FEATURE_GSM_DYNAMIC_SAIC
12/01/08   gj     CR165617 - mainline FEATURE_GSM_GPRS_ENHANCED_MEASUREMENT_REPORTING feature.
10/10/08   ap     Correction to CR 158238.
09/10/08   ap     CR 158238 - Complete L1 support to RR for Frequency Redefinition
                  Interface.
02/05/08   og     CR146017-Handle HO failure case for CR139102
28/07/98   hv     Added KxMutex support
15/07/08   nt     removal of redundant MPH_PART_CHANNEL_RELEASE_REQ
18/06/08   dv     CR147809 - Handle DTM Blind HO case
12/05/08   dv     Added FEATURE_GSM_AMR_WB - L1 component
19/05/08   og     Fix for SDCCH8 lockup after the handover request is received. CR146010.
28/04/08   cs     Always qualify RR TA data with valid flag even for NON_SYNC case
27/03/08   ap     CR 139993 Changed function gl1_msg_get_rxqual_full to take an argument
27/03/08   nf     Added NV Item handling to R-SACCH and R-FACCH
20/03/08   agv    Fixed the LCU PMIC Band setup for a Channel Change.
05/02/08   dv     Fix for CR137819 - Audio mute after blind HO failure
14/01/08   cs     Allow for separate IQ logging and saving
04/01/08   ws     CR 133085 - Only discard previous reads when transitioning
                  from DEDICATED MODE to DTM and not when in DTM mode.
09/11/07   nf     Added FEATURE_GSM_R_SACCH
06/11/07   sjw    CR115620 Removed use of FEATURE_INTER_RAT_HO_OPT around
                  gl1_msg_disable_vocoder(), required for Worldphone targets
05/10/07   ws     CR 128476 Initialise previous cell rx power using the last
                  measurement on the SACCH in the previous period
17/09/07   agv    Added I/Q logging to code base.
17/09/07   agv    CR127820 - iRAT W2G handover AGC setting.
08/08/07   ws     CR - 124046 Open loopback mode C when we need to send FACCH
03/08/07   agv    CR123023 - Combined fix for AFC/TT/AGC.
20/07/07   nt     CR107476 sync up the wcdma scheduling and the start of the
                  SACCH periods via addition of flag wcdma_start_of_sacch_indication.
20/07/07   nt     CR123397 - Allows L2 Ack to be sent for CMM rather than deleting
                  it when moving channels.
06/07/07   og     Allow W to inform G about the frequency offset of the G cell
                  in W2G non-blind handover scenarios. CR122524.
21/06/07   og     Ensure the afc information ise set correctly when doing G2G
                  non-blind handover. CR122525 (CR120843).
21/06/07   og     Allow the agc and rx power info of ncell to be primed with values
                  measured during dedic mode tch in G2G blind handovers. CR123136
01/06/07   ws     CR 117902,120248 turn off UL DTX during Handover, channel Assignment and
                  Channel Mode Modify
20/06/07   ws     Backed out CR 119738, performance gets worse with this change
21/05/07   dv     Update gl1_calc_ho_timing_advance()
06/06/07   ws     CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                  is called when we reselect to PBCCH cell stopping NC2 reporting
24/05/07   ws     CR 119738 - Manage RXQUAL_SUB average for weak signal areas where DTX in DL
                  is not used
23/05/07   nt     CR118485: Stops the SDCCH rx/tx whilst aborting, and alligns
                  new channels on block boundary.
23/05/07   agv    CR118309 - Large RSSI variation initial fix.
09/05/07   nt     CR118109:Delay sending of the Handover failure notification
                  until connecting back onto the old channels.
09/05/07   nt     CR118415:Handover to the same cell different BSIC
18/04/07   dv     CR115772:Incorrect TA calculation
30/03/07   ws     CR 114153 - remove current cell qbit calc dependency on SCE.
                  Change impact synchronised HO with no TA in gl1_is_handover_possible()
02/03/07   cs     Add support for new CGPS timetag
28/02/07   nt     CR112989: Turned on FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO, by
                  removing switch from around the code.
24/01/07   nt     109818:Added l1_sc_check_for_ncc_permitted_update to update the ncc
                  permitted if the BAlist has not changed but ncc_permitted has
23/01/07   og     Resolve RVCT2.1 compiler warnings, merge from the
                  gpl1_hummer.10.00.43.01 branch.
28/11/06   dv     Lint cleanup
22/11/06   og     Reverse the order of stopping SCE and setting the dedicated
                  stop main command. Resolves CR105367.
27/10/06   ws     Remove unecessary prototypes and fixed lint warnings
27/10/06   ws     CR 14307 reconnect original AMR config if HO fail's
05/09/06   og     CR100861 / SR860243 prevent SDCCH8 abort lockup fix merge.
14/08/06   ws     Allow dedicated to be timeslot slammed to lowest received tn
                  and also to place the CS timeslot on TN 2
18/07/06   nt     CR97716 - TC26.6.13.3 - Fix for CR91329 has broken this test
                  case. The setting of the flag previous_cell_on_after_channels
                  should not occur if there is a pending frequency redef.
10/07/06   dv     CR96645: Numerous changes to deal with HO TA calculation
23/06/06   og     Take the broadcast bep period into account when moving into
                  DTM mode.
13/06/06   nt     CR94517: Cleared out the starting time upon a reconnect reqest as the
                  code sometimes uses it even if the valid flag is false.
15/05/06   nt     CR91329 - Added flag to indicate if the previous starting time
                  on the old assignment had expired, upon returning to old channel
                  configuration with the reconnect_channel_req.
05/04/06   og     Merge the fix to CR 84621, change 308343 from the raven 3.0
                  branch.
17/03/06   og     Take downlink assignment mac mode into account. This is
                  required by release 4 ETSI spec.
22/02/06   nt     Fix for release handling when in UL+DL.
08/02/06   ws     Restore cipher related vars when assigning dtm from dedicated
07/02/06   ws     Added setting up of mac mode for EXT. Dynamic in DTM
25/11/05   nt     Added handling of ASSIGNMENT and HANDOVER commands in DTM
04/11/05   nt     Added handling of UL+DL in DTM_ASSIGNMENT
02/11/05   nt     Added code to handle the DTM_ASSIGNMENT with DTM mode
10/20/05   nt     Removed pseudo starting time in functions gpl1_dtm_save_ul_alloc
                  gpl1_dtm_save_dl_alloc. No ST required in DTM.
10/02/05   gfr    Use new gl1_msg_cfg_ul_rach API
09/21/05   dp     Fix to base RLA_C on RXLEV_SUB instead of RXLEV_FULL
20/09/05   nt     removed the static from gl1_determine_l1_vocoder_control under DTM
09/01/05   ws     Linted and code cleanup for FEATIRE_GSM_DTM
22/08/05   og     Add the setting of mac mode to NPL1.
17/08/05   og     Removal of dummy ctrl block dual buffer.
08/16/05   dp     Lint cleanup
08/12/05   ws     Added initial FEATURE_GSM_DTM support
05/05/05   nt     Added handling of MPH_STOP_NC_MEASUREMENTS_REQ from GRR.
04/12/05   dp     GSM L1 now releases the vocoder on a channel release
03/29/05   dp     InterRAT audio optimizations
03/16/05   pp     Fix to handle channel assignment failure after intercell handover
                  Fix to initialize agc for intercell handovers and reconnects
                  Changed gl1_complete_channel_reconnect() to reuse code
03/15/05   dp     Added support for enhanced measurement reporting
03/11/05   sv     Mainlined FEATURE_GSM_CB
02/25/05   pp     Added support for GSM to GSM Blind Handovers
                  Fix to not perform SYNCHRONISED and PSEUDO_SYNCHRONISED
                  handovers if BSIC is not known
                  Optimized MPH_CHANNEL_RELEASE_REQ processing by splitting
                  it between task and ISR mode
01/05/05   dp     Merged dedicated timeline changes for audio optimizations
                  to mainline
07/21/04   bk     Merged in changes to RACH to support EGPRS
07/13/04   dp     Fixed bug in MTD computation when offset is -ve for non-
                  synchronized handovers
06/09/04   gfr    Remove obselete calls to gl1_msg_store_tx_lev().
06/11/04   dp     Replaced reference to BCC with BSIC in MPH_CHANNEL_RELEASE_REQ
06/04/04   sv     Removed all references to XXX_mframe_power_XXX.
05/26/04   sv     Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                  code readability.
05/04/04   dp     Determine L1 vocoder control after suspending old channel.
                  Added vocoder blanking when reconnecting to old channel
03/31/04   gw     Added handling of messages from CB task.
03/26/04   gfr    Time tagging support.
03/18/04   gsc    Lint Cleanup.
03/12/04   dp     Start vocoder blanking after tearing down old channel
                  during handovers
01/20/04   dp     Implemented vocoder blanking in GSM to GSM handovers
01/09/04   gw     Merged in support for new SI reading API from branch.
11/21/03   dp     Fixed d/l mute issue observed in China (due to handover
                  during call setup )
11/11/03   dp     Fix to disable vocoder across handovers only if channel
                  mode changes
11/05/03   gfr    Support for quad-band.
11/04/03   gw     Store previous RXLEV_SUB value and use it if sending a
                  measurement report but there are no measurements to report.
10/03/03   dp     Change to have L1 be in control of the vocoder during
                  GSM to GSM inter-cell and intra-cell handovers (Audio
                  quality fix for Samsung )
09/02/03   rr     Change so L1 won't delete BA list when RR sends empty list.
08/15/03   dp     Fix for incorrect # of access bursts in 26.6.8.4.3.1
08/06/03   dp     Fixed incorrect number of HO access bursts during a
                  half rate synchronized handover
07/29/03   dp     Fixed bug in sending HO access bursts on SDCCH.  This
                  fixes 26.6.5.4.2 at Cetecom
07/24/03   dp     Added HR support for channel mode modify
07/11/03   gfr    Lint cleanup.
06/24/03   dp     Fixed bug in computing OTD when offset is negative
06/23/03   gfr       Compiler warning cleanup.
06/06/03   tb     Added support for dedicated mode gtow searching.
05/07/03   gsc    Added AMR support
05/02/03   gw     Merged in power transfer changes from branch.
04/14/03   ATM    Added dtx_used logic: CR28812
04/02/03   gw     Changes to reflect modification of monscan functions.
03/18/03   dp/gw  Fixes for HR.
03/07/03   tb     Added support for handling GtoW handover failure.
02/25/03   dp     Added HR support featurized under
                  FEATURE_GSM_HALF_RATE and enabled by default
02/21/03   gw     Added call to L1_setup_DEACTIVATE() to process
                  MPH_STOP_GSM_MODE_REQ.  This function contains support for
                  deep sleep.
01/10/03   gw     Changed some SCE function calls to reflect renaming of some
                  BA list functions done as part of SCE clean up.
11/15/02   dp     Added support for multiband reporting
10/01/02   ATM    Added central get and set FN routines
09/19/02   dp     Fix to correctly apply the TA on the old channel after a
                  handover failure
08/29/02   JC     Modified usage of gl1_msg_cfg_ul_rach() for multi-band
                  support.
08/12/02   gw     Removed unneeded prototype for L1_setup_PSCAN().
08/23/02   plm    Removed declaration of local variable error in the function
                  L1_handle_dedicated_mode() that was declared but not used to
                  eliminate compiler warning.
                  Added void argument list to the function gl1_load_current_cell_data()
                  to eliminate compiler warnings.
                  Moved handover_is_possible = gl1_is_handover_possible( message_ptr )
                  assignment out of conditional statement to eliminate compiler warning
                  in function L1_handle_dedicated_mode.
07/15/02   dp     Fix to not reset Ncell info when a DEDICATED_UPDATE_REQ is
                  received
06/10/02   dp     Support to stop sending SACCH meas rpts after channel release
                  Fix to use correct power level after reconnecting to the old
                  channel
05/22/02   mk     Changed Channel Release handling to send ENTER_NULL command
                  if no valid carrier available.
05/17/02   mk     Unified PowerScan and Acquisition PL1 I/f.
05/15/02   JC     Reset the RXLEV accumalators for each 104-SACCH period.
05/15/02   mk     Added INTLOCK()/INTFREE() around L1_FIND_BCCH_COMMAND.
05/14/02   mk     Updated RXQUAL reporting.
05/10/02   JC     Added support for new gl1_cfg_power_measure API.
05/10/02   tb     Added FEATURE_MULTIMODE_GSM support.
05/06/02   dp     Added debug messages for handovers
04/29/02   gw     Added error messages to indicate when messages to L1 are
                  ignored.
04/24/02   dp     Fixed bug in TXLEV reporting on SACCH.  We now correctly report
                  the TXLEV of the last burst of the previous reporting period
04/23/02   mk     Added state transition to full power scan if no valid bcch
                  carrier available while in MPH_CHANNEL_RELEASE_REQ.
04/23/02   dp     Fixed bug in intra-cell handovers.  BSIC on current(old) cell
                  was not being saved correctly
04/19/02   dp     Cipher mode req now transfers ciphering mode parameters to the
                  ISR without suspending and reinitializing the channel.   Fix to
                  apply new timing advance immediately on new channel after handover.
                  Fix to correctly compute reported OTD in HANDOVER_COMPLETE
03/25/02   dp     Fix to correctly change cipher mode setting when reconnecting
                  the old channel
03/20/02   dp     Changes to support change from txlev to power index for access
                  bursts
03/06/02   dp     Support to frequency hop access bursts
03/05/02   JC     Added RXLEV reporting support. Also, added SC_TIMING_OFFSET
                  definition here to remove legacy header files.
02/28/02   mk     Added initial support for RXQUAL_FULL and RXQUAL_SUB
                  measurements.
02/27/02   gw     Added support for MPH_CHANNEL_RELEASE_REQ with no valid
                  BCCH carrier.
02/20/02   dp     Added support for handovers
02/13/02   jc     Add support for DTX.
02/07/02   gw     Changes to improve starting and stopping of SCE.
12/18/01   dp     Changes to support SDCCH SC functionality
10/15/01   JC     Added MPH_CHANNEL_MODE_MODIFY_CNF as an L1_confirm_msg
                  to the ISR so that the ISR sends a PH_READY_TO_SEND
                  primitive to L2 in the L1_notify_start() function.
09/20/01   jc     Cleanup on 9/4 messed up channel assign cmd.  Use message
                  layer for storing ta, and txlev.
09/04/01   JC     Cleanup of obsoleted compile switch and addition of std
                  QCT fcn hdr templates.
04/24/01   jc     Initial version (Sina emulaton board)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_utils.h"   /* For ADD_FN and SUB_FN macros */
#include "l2_l1.h"
#include "l2_l1_g.h"
#include "l1_task.h"
#include "l1_sc_int.h"
#include "memory.h"
#include "ms.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "sys_stru.h"
#include "l1_isr.h"
#include "l1_sc.h"
#include "msg.h"
#include "gl1_sys_params.h"
#include "l1_smscb.h"
#include "cbl1if.h"
#include "geran_eng_mode_info.h"
#include "geran_dual_sim_g.h"
#include <stringl/stringl.h>

#ifdef FEATURE_GSM_AMR
    #include "l1_amr.h"
#endif

#include "gl1_hw_debug.h"
#include "gl1_voice_server.h"

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gpl1_gprs_isr.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GSM_DTM
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_isr.h"
#include "gmacsigl1.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

static void gpl1_dtm_save_ul_alloc( mac_dynamic_tbf_information_T *ul_tbf, gas_id_t gas_id );
static void gpl1_dtm_save_dl_alloc( mac_downlink_tbf_t *ul_tbf, gas_id_t gas_id );
static void gpl1_dtm_save_uldl_alloc( mac_l1_dtm_channel_assign_req_t *ul_tbf );
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
#include "l1_log_qsh.h"
#endif /*FEATURE_QSH_EVENT_METRIC*/
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
boolean dtm_assign_cs_present[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_GSM_AMR
extern gl1_msg_amr_mode_type curr_dl_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern gl1_msg_amr_mode_type curr_ul_acs[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES];
extern uint8 curr_thresh_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_hyst_tbl[NUM_GERAN_DATA_SPACES][MAX_CODEC_MODES - 1];
extern uint8 curr_dl_icm[];
extern uint8 curr_ul_icm[];
extern uint8 curr_dl_num_acm[];
extern uint8 curr_ul_num_acm[];
extern cmi_phase_type curr_phase[];
extern boolean ratscch_rsp_pending[];
extern ratscch_req_type ratscch_pending_req[];
#endif
/*
 * Flag to indicate if a full set of rxqual values have been taken
 * during a reporting period.
 */
extern boolean meas_report_valid[NUM_GERAN_DATA_SPACES];
extern dedicated_data_T l1_dedicated_data[];

typedef struct
{
  /* Needed for iRAT W2G handover AGC power. */
  sys_algo_agc_T   l1_interrat_agc_data;
  boolean          l1_interrat_ho_agc_update;

  int16            handover_ta;
  byte             handover_power_level;
  uint32           handover_rtd;
  //dword            lag; /* in bits */

  /* otd = observed time difference - defined in 45.010 */
  int16            otd;
  byte             current_rr_l1_msg_id;
  boolean          need_to_sync;
  boolean          do_gsm_blind_handover;
  byte             old_channel_DCCH;
  boolean          valid_bcch_carrier;
  byte             prev_rxlev_sub;
  byte             prev_rxlev_full;
  /* Used to remember last DTX_indicator setting if going
   * to DTM mode OR receiving a Channel Mode Modify
   */
  boolean          stored_dtx_indicator;
  /* Used in G2G handover to prime the agc data of the acquisition
   * attempt using the ncell measurements performed during dedicated
   * tch mode.
   */
  boolean          l1_target_ncell_unavailable;
} l1_ded_if_data_t;

static l1_ded_if_data_t l1_ded_if_data[NUM_GERAN_DATA_SPACES];


static boolean gl1_is_handover_possible
(
mph_handover_req_T  *message_ptr
);

#ifdef FEATURE_GSM_RX_DIVERSITY
#define RXD_RXQUAL_THRESHOLD 2
#endif
#ifdef FEATURE_AEQ_PHASE4
#define AEQ4_OFF_RXQUAL_THR 2
#define AEQ4_ON_RXQUAL_THR 4
#define AEQ4_RXQUAL_INVALID 8

#endif /*FEATURE_AEQ_PHASE4*/


void gl1_load_current_cell_data( gas_id_t gas_id );

extern void gl1_msg_tch_loopback( gl1_defs_loopback_type loopback, gas_id_t gas_id );


#ifndef FEATURE_GSM_DTM
static void gl1_complete_channel_assignment ( void );

/* The apply afc boolean allows the UE to make use of any freq offset
 * information applicable to the GSM ncell we are being handed over to.
 * This applies in G2G and W2G handover scenarios.
 */
static void gl1_complete_channel_handover ( boolean  apply_afc, gas_id_t gas_id );
static void gl1_save_current_cell_data( boolean store_afc_info,gas_id_t gas_id );
static void gl1_complete_channel_reconnect ( gas_id_t gas_id );
#endif

static void gl1_complete_channel_mode_modify ( gas_id_t gas_id );
static void gl1_complete_channel_release ( gas_id_t gas_id );
extern void gl1_setup_new_channel ( gas_id_t gas_id );
void gl1_begin_gsm_blind_handover( boolean acq_success, gas_id_t gas_id );

/* from l1_null_if.c */
extern void L1_setup_blind_ho_ACQ( ARFCN_T                arfcn,
                                   const sys_algo_agc_T*  agc_ptr, gas_id_t gas_id );



/*===========================================================================

                          LOCAL FUNCTION

===========================================================================*/

/*===========================================================================

FUNCTION  l1_ded_if_init_data

DESCRIPTION
  This function initialise the gloabal data used by l1_ded_if.c

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Initialise l1_ded_if_data

===========================================================================*/
void l1_ded_if_init_data( void )
{
  int i;

  for (i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++)
  {
    /* Needed for iRAT W2G handover AGC power. */
    l1_ded_if_data[i].l1_interrat_ho_agc_update = FALSE;

    /* otd = observed time difference - defined in 45.010 */
    l1_ded_if_data[i].otd                         = 0;
    l1_ded_if_data[i].current_rr_l1_msg_id        = NULL;
    l1_ded_if_data[i].need_to_sync                = FALSE;
    l1_ded_if_data[i].do_gsm_blind_handover       = FALSE;
    l1_ded_if_data[i].old_channel_DCCH            = NULL;
    l1_ded_if_data[i].valid_bcch_carrier          = FALSE;
    l1_ded_if_data[i].prev_rxlev_sub              = 0;
    l1_ded_if_data[i].prev_rxlev_full             = 0;
    l1_ded_if_data[i].stored_dtx_indicator        = FALSE;
    l1_ded_if_data[i].l1_target_ncell_unavailable = FALSE;
  }
}


/*===========================================================================

FUNCTION  l1_interrat_ho_update

DESCRIPTION
  This function sets/resets the iRAT Handover test to prime the AGC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_interrat_ho_update(boolean update, gas_id_t gas_id)
{
  l1_ded_if_data[gas_id].l1_interrat_ho_agc_update = update;
}

/*===========================================================================

FUNCTION  l1_interrat_ho_agc

DESCRIPTION
  This function primes the Interrat Handover AGC value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_interrat_ho_agc(sys_algo_agc_T handover_agc, gas_id_t gas_id)
{
  l1_ded_if_data[gas_id].l1_interrat_agc_data = handover_agc;
  l1_interrat_ho_update(TRUE, gas_id);
}


/*===========================================================================

FUNCTION  l1_prime_agc_for_handover

DESCRIPTION
  This function sets the appropriate Handover AGC by selecting the
  target cell value from the Best 6. Additionally, it also now
  has the iRAT Handovers Blind, and CM processed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_prime_agc_for_handover(ARFCN_T arfcn, gas_id_t gas_id)
{
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   mph_surround_meas_ind_T msg;
   l1_sc_bs_T*             cells[ MAX_NUMBER_OF_SURROUND_CELLS ];
   l1_sc_bs_T*             cell;
   uint8                   n;

   if (l1_ded_if_data[gas_id].l1_interrat_ho_agc_update)
   {
     l1_interrat_ho_update(FALSE, gas_id);
     gl1_msg_set_ho_pwr( l1_ded_if_data[gas_id].l1_interrat_agc_data.pwr_dBm_x16,gas_id );
     MSG_GERAN_MED_2_G("iRAT Handover AGC pwr=%d,Arfcn=%d",
              l1_ded_if_data[gas_id].l1_interrat_agc_data.pwr_dBm_x16/16,
              arfcn.num);
   }
   else
   {
     msg.best_six_info.no_of_entries = l1_sc_balist_get_best_six( cells, gas_id );

     if ( msg.best_six_info.no_of_entries > ( sizeof( cells ) / sizeof( cells[0] ) ) )
     {
        MSG_GERAN_ERROR_2_G( "Too many best_six_info entries %d clamping to max %d",
                   msg.best_six_info.no_of_entries,
                   ( sizeof( cells ) / sizeof( cells[0] ) ));

        msg.best_six_info.no_of_entries = ( sizeof( cells ) / sizeof( cells[0] ) );
     }

     for ( n = 0; n < msg.best_six_info.no_of_entries; ++n )
     {
       cell = cells[ n ];
       if ((arfcn.num  == cell->arfcn.num) &&
           (arfcn.band == cell->arfcn.band))
       {
          sys_algo_agc_init( &l1_dedicated_data_ptr->c0_agc, (dBx16_T)cell->rx_power );
         gl1_msg_set_ho_pwr( (int16)cell->rx_power,gas_id );
         MSG_GERAN_MED_2_G("GSM Handover AGC pwr=%d,Arfcn=%d",
                   (dBx16_T)cell->rx_power/16, arfcn.num);
         break;
       }
     }
   }
}


/*===========================================================================

FUNCTION  L1_handle_dedicated_mode

DESCRIPTION
  This function handles the L1 Task in dedicated mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_dedicated_mode(IMH_T *msg_header)

{
    /*This function interprets all message in this mode.
      when the ISR sends a ISR_SCAN_CNF message to the L1 task. */
    gas_id_t gas_id;
    byte temp=0;
    inter_task_BCCH_allocation_T  ba_buffer;
    boolean    gl1_in_ftm_mode;

#ifdef FEATURE_AEQ_PHASE4
    gl1_hw_dtm_rx_aeq4_type mdsp_aeq4_ctl_info;
#endif /* FEATURE_AEQ_PHASE4*/

    gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

    if (!msg_header)
    {
        MSG_GERAN_ERROR_0("Rcvd NULL msg_header pointer");
        return;
    }

    /********************************************************
    *                                                      *
    *                PH_DATA_REQ                           *
    *                                                      *
    ********************************************************/

    if (msg_header->message_set == MS_L2_L1)
    {
        switch (msg_header->message_id)
        {
        /* Request to send data on the LAPDm channel */
        case PH_DATA_REQ:
            {
                ph_data_req_T  *message_ptr;
                l1_dcch_buffer_T  *dcch_buffer_ptr;
                l1_sacch_buffer_T *sacch_buffer_ptr;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (ph_data_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];


                /* Check which channel the message is to be sent on */
                if (message_ptr->l2_channel_type == DCCH)
                {
                    dcch_buffer_ptr = &l1_dcch_s0_buffer[gas_id];

                    /* Check if the buffer is full */
                    if (dcch_buffer_ptr->data_valid == TRUE)
                    {
                        /* Buffer is full */
#ifdef HL_DEBUG
                        #error code not present
#endif
                    }

                    else
                    {
                      /* Are we currently doing loopback C ? */
                      if(gl1_msg_get_tch_loopback(gas_id) == GL1_DEFS_LOOPBACK_TYPE_C)
                      {
                        /* break the loop and send the FACCH, note due to TCH interleaving
                         * we need to break the loop for 2 speech frames
                         */
                        gl1_msg_tch_loopback(GL1_DEFS_NO_LOOPBACK, gas_id);

                        if ((message_ptr->layer2_frame[3]== TEST_MANAGEMENT_L1 ) &&
                            (message_ptr->layer2_frame[4]== OPEN_LOOP_CMD_L1 ) &&
                            (message_ptr->layer2_frame[5]== OPEN_LOOP_CMD_ACK_L1))
                        {
                          l1_dedicated_data_ptr->l1_loopback_disabled = LOOPBACK_C_OPEN;
                        }
                        else
                        {
                          l1_dedicated_data_ptr->l1_loopback_disabled = LOOPBACK_C_TEMP_OPEN;
                        }

                        MSG_4(MSG_SSID_DFLT,MSG_LEGACY_HIGH,
                              "Loopback disabled to send FACCH [%d],l2[3]=%x, l2[4]=%x, l2[5]=%x",gl1_get_FN( gas_id ),
                              message_ptr->layer2_frame[3],message_ptr->layer2_frame[4],
                              message_ptr->layer2_frame[5]);
                      }
                      memscpy(dcch_buffer_ptr->dcch_data,
                              sizeof(dcch_buffer_ptr->dcch_data),
                              message_ptr->layer2_frame,
                              DCCH_MESSAGE_UNIT);

                      dcch_buffer_ptr->data_valid = TRUE;
                    }
                }

                else
                {
                    /* It's a SACH message */
                    /* Set the message pointer */
                    gas_id = check_gas_id(message_ptr->gas_id);
                    sacch_buffer_ptr = &l1_sacch_s0_buffer[gas_id];

                    /* Check if the buffer is full */
                    if (sacch_buffer_ptr->data_valid)
                    {
                        /* Buffer is full */
#ifdef HL_DEBUG
                        #error code not present
#endif
                    }

                    else
                    {
                        memscpy(sacch_buffer_ptr->sacch_data,
                                sizeof(sacch_buffer_ptr->sacch_data),
                                message_ptr->layer2_frame,
                                SACCH_MESSAGE_UNIT);

                        sacch_buffer_ptr->data_valid = TRUE;
                    }
                    if(l1_dedicated_data_ptr->rsacch_ctrl.nv_active == TRUE)
                    {
                        /*copy repetition info flag*/
                        l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info = message_ptr->l2_repetition_info;
                    }
                }
                break;
            }
            /*******************************************************
             *                                                      *
             *                PH_EMPTY_FRAME_REQ                    *
             *                                                      *
             ********************************************************/

        case PH_EMPTY_FRAME_REQ:
            {
                /* Request to send blank data on the LAPDm channel */

                ph_empty_frame_req_T  *message_ptr;
                l1_dcch_buffer_T  *dcch_buffer_ptr;
                l1_sacch_buffer_T *sacch_buffer_ptr;

                message_ptr = (ph_empty_frame_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);

                /* Check which channel the message is to be sent on */
                if ( message_ptr->l2_channel_type == DCCH)
                {
                    dcch_buffer_ptr = &l1_dcch_s0_buffer[gas_id];

                    /* Check if the buffer is full */
                    if (dcch_buffer_ptr->data_valid)
                    {
                        /* Buffer is full */
#ifdef HL_DEBUG
                        #error code not present
#endif
                    }
                    else
                    {
                        memscpy(dcch_buffer_ptr->dcch_data,
                                sizeof(dcch_buffer_ptr->dcch_data),
                                fill_frame,
                                DCCH_MESSAGE_UNIT);

                        dcch_buffer_ptr->data_valid = TRUE;
                    }
                }

                else
                {
                    /* It's a SACH message */
                    /* Set the message pointer */
                    sacch_buffer_ptr = &l1_sacch_s0_buffer[gas_id];

                    /* Check if the buffer is full */
                    if (sacch_buffer_ptr->data_valid)
                    {
                        /* Buffer is full */
#ifdef HL_DEBUG
                        #error code not present
#endif

                    }

                    else
                    {
                        memscpy(sacch_buffer_ptr->sacch_data,
                                sizeof(sacch_buffer_ptr->sacch_data),
                                fill_frame,
                                SACCH_BUFFER_SIZE);

                        sacch_buffer_ptr->data_valid = TRUE;
                    }
                }
                break;
            }
            /*******************************************************
             *                                                      *
             *                PH_RANDOM_ACCESS_IND                  *
             *                                                      *
             ********************************************************/
        case PH_RANDOM_ACCESS_IND:
          {
            ph_random_access_ind_T *message_ptr = (ph_random_access_ind_T *)msg_header;
            gas_id = check_gas_id(message_ptr->gas_id);
            if (message_ptr->rach_for_ps_traffic)
            {
              MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for PS");
            } else
            {
              MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for CS");
            }
            L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
          }
            /*******************************************************
             *                                                      *
             *                PH_RANDOM_ACCESS_REQ                  *
             *                                                      *
             ********************************************************/
        case PH_RANDOM_ACCESS_REQ:
            {
                /*
                    Potential race condition - check when testing
                    If the random access req comes in before the
                    dedicated mode on current cell has been suspended
                    and target cell details loaded, then target_cell_details
                    will be incorrect.  In that case HANDOVER_IND will have to
                    be sent after dedicated mode has been suspended
                */

                ph_random_access_req_T   *message_ptr;
                gl1_msg_rach_cb_type cb;
                idle_data_T               *l1_idle_data;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = ( ph_random_access_req_T *) msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);

                l1_idle_data=&l1_idle_data_store[gas_id];
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                l1_dedicated_data_ptr->access_burst_content = message_ptr->random_access_content;

                /* No callbacks required for handover access bursts -
                   just transmit RACHs.  No need to read anything on the
                   AGCH
                */
                cb.data_cb    = NULL;
                cb.abort_cb   = NULL;

                /* Configure handover access bursts */
                gl1_msg_cfg_ul_rach ( NULL, l1_idle_data->campedon_cell_BSIC,
                                      GL1_MSG_RACH_HANDOVER, &cb,gas_id);

                /* Restart L1 in dedicated mode */
                GL1_ISR_LOCK(gas_id);
                l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
                l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
                GL1_ISR_UNLOCK(gas_id);
                break;
            }

            /********************************************************
             *                                                         *
             *                MPH_STOP_SACCH_MEASUREMENT_REPORT        *
             *                                                         *
             ***********************************************************/

        case PH_STOP_SACCH_MEASUREMENT_REPORT:
            {
                ph_stop_sacch_measurement_report_T* message_ptr = ( ph_stop_sacch_measurement_report_T* )msg_header;
                gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
                dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
                /* Stop sending SACCH measurement reports as soon as
                   L2 tells us too.    This is to fix the issue with
                   the network receiving a measurement report after it
                   has commanded a release
                */
                l1_dedicated_data_ptr->stop_meas_rpt = TRUE;

                GL1_ISR_LOCK(gas_id);
                /* The change is propagated to the ISR */
                L1_transfer_new_data( gas_id);
                GL1_ISR_UNLOCK(gas_id);
                break;
            }

        default:
            MSG_GERAN_ERROR_1("L2->L1(DED) message %d ignored.", msg_header->message_id);
            break;
        } /* end of switch */
    }

    else if (msg_header->message_set == MS_RR_L1)
    {

      switch (msg_header->message_id)
        {
        /* Enable Sleep indication */
        case MPH_ENABLE_SLEEP_IND:
      {
      L1_handle_ENABLE_SLEEP_IND ( msg_header );
          break;
        }
        /********************************************************
         *                                                      *
         *                MPH_CHANNEL_ASSIGN_REQ                *
         *                                                      *
         ********************************************************/

        case MPH_CHANNEL_ASSIGN_REQ:
            {
                mph_channel_assign_req_T  *message_ptr;
                idle_data_T               *l1_idle_data;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (mph_channel_assign_req_T *)msg_header;
                gas_id = check_gas_id(message_ptr->gas_id);

                l1_idle_data=&l1_idle_data_store[gas_id];
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                if (l1_tskisr_blk->sub_state == L1_DED_BEFORE )
                {
                    l1_dedicated_data_ptr->previous_chnl_type =
                       l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
/* HV: exclude as per AV: PMIC signaling needed for Solo. Mustang uses RTR6285 and diff PMIC */

                }
                else
                {
                    l1_dedicated_data_ptr->previous_chnl_type =
                       l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type;
/* HV: exclude as per AV: PMIC signaling needed for Solo. Mustang uses RTR6285 and diff PMIC */

                }

                /* In this case target cell arfcn is same as current cell arfcn - used in case
                   of a assignment failure while handling MPH_RECONNECT_CHANNEL_REQ
                */
                l1_dedicated_data_ptr->target_cell_details.arfcn =
                l1_idle_data->campedon_cell_ARFCN;

                /* Save the old channel parameters in case we need to reconnect and use
                   the old parameters in case of a failed channel assignment
                */
                gl1_save_current_cell_data( FALSE,gas_id );

                /* Now fill in the new dedicated data */
                memscpy ( &l1_dedicated_data_ptr->new_channel_specification,
                         sizeof(l1_dedicated_data_ptr->new_channel_specification),
                         &message_ptr->channel_spec,
                         sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
                /* Now fill in the multirate configuration data */
                memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                         sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                         &message_ptr->multirate_config,
                         sizeof(multirate_config_T) );
#endif
                l1_dedicated_data_ptr->new_starting_time =
                (message_ptr->channel_spec.starting_time[0] * 256) +
                (message_ptr->channel_spec.starting_time[1] % 256);


                /* Previous cell applies only to inter-cell handovers */
                l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;


                /* Save off msg id of message currently being processed
                 * If current_rr_l1_msg_id is not NULL (its reset in
                 * gl1_setup_new_channel), we are currently processing a req
                 * from RR, so don't set this variable as it will mess up the
                 * request being processed
                 */
#ifdef FEATURE_GSM_DTM
                /*
                Dont save messages and set the main command if actually in DTM.
                This can happen as RR messages received in DTM are processed here
                */

                if( l1_tskisr_blk->l1_state != L1_DTM_MODE )
                {
                  if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                  {
                      l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                  }
                  else
                  {
                      MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[message_ptr->gas_id].current_rr_l1_msg_id,msg_header->message_id);
                  }

                  /* Suspend L1 ISR from doing anymore */
                  l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
                }
#else

                if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                {
                    l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                }
                else
                {
                    MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                }

                /* Suspend L1 ISR from doing anymore */
                l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
#endif
                /* stop SCE */
                l1_sc_stop(gas_id);

                l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;

                break;
            }

#ifdef FEATURE_SGLTE

           /********************************************************
           *                                                       *
           *                MPH_UE_MODE_CHANGE_REQ                 *
           *                                                       *
           ********************************************************/

        case MPH_UE_MODE_CHANGE_REQ:
            {
               mph_ue_mode_change_req_T  *message_ptr;
               message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 unusual pointer cast */
               gas_id = check_gas_id(message_ptr->gas_id);
               /* Set SGLTE mode */
               gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, gas_id);


               l1_sc_wcdma_set_mode(message_ptr->gsm_only, gas_id);

               break;
            }
#endif

            /********************************************************
             *                                                      *
             *                MPH_CHANNEL_MODE_MODIFY_REQ           *
             *                                                      *
             ********************************************************/

        case MPH_CHANNEL_MODE_MODIFY_REQ:
            {
                mph_channel_mode_modify_req_T  *message_ptr;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (mph_channel_mode_modify_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);

                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                /* Stop the SCE */
                l1_sc_stop(gas_id);

                /* Now fill in the new dedicated data */
                l1_dedicated_data_ptr->new_channel_specification.channel_mode_1 =
                message_ptr->channel_mode;

#ifdef FEATURE_GSM_AMR
                /* Now fill in the multirate configuration data */
                memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                         sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                         &message_ptr->multirate_config,
                         sizeof(multirate_config_T) );
#endif


                /* Previous cell applies only to inter-cell handovers */
                l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;


                /* Save off msg id of message currently being processed
                 * If current_rr_l1_msg_id is not NULL (its reset in
                 * gl1_setup_new_channel), we are currently processing a req
                 * from RR, so don't set this variable as it will mess up the
                 * request being processed
                 */
                if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                {
                  l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                }
                else
                {
                  MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                }

                /* Suspend L1 ISR from doing anymore */
                l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

                break;
            }

            /********************************************************
             *                                                      *
             *                MPH_CHANNEL_RELEASE_REQ               *
             *                                                      *
             ********************************************************/

        case MPH_CHANNEL_RELEASE_REQ:
            {
                mph_channel_release_req_T  *message_ptr;
                idle_data_T               *l1_idle_data;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (mph_channel_release_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);

                l1_idle_data=&l1_idle_data_store[gas_id];
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                MSG_GERAN_HIGH_0_G("GL1: Received channel release req from L1");

                /* Previous cell applies only to inter-cell handovers */
                l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;

#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
                gl1_msg_set_iq_state( FALSE, gas_id );
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

                /* Channel Release so always send pre-cell change to GPS */
                /* Tell CGPS we are leaving this cell */
                gl1_timetag_cgps( l1_tskisr_blk->l1_state, CGPS_TIMETAG_PRE_CELL_CHANGE, gas_id );

                /* Wait for real next frame tick to send pending TT update from DPC task */
                gl1_wait_for_dpc( gas_id );

                /* Copy the data */
                if (message_ptr->valid_bcch_carrier)
                {
                    l1_ded_if_data[gas_id].valid_bcch_carrier = TRUE;

                    l1_idle_data->campedon_cell_ARFCN =
                      message_ptr->serving_bcch_carrier;
                    l1_idle_data->campedon_cell_BSIC =
                      message_ptr->BSIC.BS_colour_code;
                    l1_idle_data->campedon_cell_BSIC +=
                      (byte)(message_ptr->BSIC.PLMN_colour_code * 8);

                    /* Setup default BCCH decoding mask in case we go
                     * to Idle Mode.
                     */
                    l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM;
                    l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask( l1_idle_data->si_mask );
                    l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );
                }
                else
                {
                    l1_ded_if_data[gas_id].valid_bcch_carrier = FALSE;
                }

                if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                {
                    l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                }
                else
                {
                    MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                }

                GL1_ISR_LOCK(gas_id);

                /* Suspend L1 ISR from doing anymore */
                l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

                /*send*/

                /* Stop SCE */
                l1_sc_stop(gas_id);

                GL1_ISR_UNLOCK(gas_id);

                break;
            }

            /********************************************************
             *                                                      *
             *                MPH_DEDICATED_UPDATE_REQUEST          *
             *                                                      *
             ********************************************************/

        case MPH_DEDICATED_UPDATE_REQ:
            {
                mph_dedicated_update_req_T  *message_ptr;
                idle_data_T *l1_idle_data;
                dedicated_data_T *l1_dedicated_data_ptr;
                gas_id_t gas_id;

                message_ptr = (mph_dedicated_update_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_idle_data = &l1_idle_data_store[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                GL1_ISR_LOCK(gas_id);

                if ( message_ptr->BA_list.no_of_entries > 0 )
                {
                    L1_remove_oob_ba(&ba_buffer,&message_ptr->BA_list);
                    l1_sc_set_ba_list(&ba_buffer, message_ptr->NCC_permitted,gas_id );
                }

                /* If we know previous cell's timing add it to BA list */
                if ( l1_dedicated_data_ptr->previous_cell_details.bsic_known )
                {

                    l1_sc_balist_add_cell ( &l1_dedicated_data_ptr->previous_cell_details,gas_id );
                }

                /* Sets the multiband parameter specified by the network */
                l1_sc_balist_set_multiband_reporting( message_ptr->multiband_reporting,gas_id );

                GL1_ISR_UNLOCK(gas_id);

                MSG_GERAN_MED_1_G("L1: Setting MULTIBAND_PARAMETER=%d",message_ptr->multiband_reporting);

                l1_dedicated_data_ptr->channel_specification.DTX_indicator =
                message_ptr->DTX_indicator;
                l1_idle_data->campedon_cell_data.PWRC =
                message_ptr->PWRC;

                if( message_ptr->DTX_indicator != RR_L1_DTX_OVERRIDE && 
					l1_ded_if_data[gas_id].stored_dtx_indicator != message_ptr->DTX_indicator)
                {
                  /*  Do not change DTX mode while Loopback is enabled */
                  if (gl1_msg_tch_get_loopback_type(gas_id) == GL1_DEFS_NO_LOOPBACK)
                  {
                    if (message_ptr->DTX_indicator)
                    {
                        l1_vs_set_dtx_mode(DTX_ENABLED, gas_id);
                        l1_ded_if_data[gas_id].stored_dtx_indicator = TRUE;
                    }
                    else
                    {
                        l1_vs_set_dtx_mode(DTX_DISABLED, gas_id);
                        l1_ded_if_data[gas_id].stored_dtx_indicator = FALSE;
                    }
                  }
                  else
                  {
                    /* Setting the prior dtx mode will make sure that the
                       dtx mode is updated when loopback terminates */
                    if (message_ptr->DTX_indicator)
                    {
                        gl1_msg_tch_set_prior_dtx_mode(TRUE, gas_id);
                        l1_ded_if_data[gas_id].stored_dtx_indicator = TRUE;
                    }
                    else
                    {
                        gl1_msg_tch_set_prior_dtx_mode(FALSE, gas_id);
                        l1_ded_if_data[gas_id].stored_dtx_indicator = FALSE;
                    }
                    MSG_GERAN_HIGH_0_G("DTX mode change delayed until loopback terminates");
                  }
                }

                MSG_GERAN_HIGH_1("Stored dtx indicator %d",
                                 l1_ded_if_data[gas_id].stored_dtx_indicator);

                /* Check to see if the ncc_permitted has changed but the BAlist
                has not. In a case such as this the ncc_permitted wasn't updated.
                This function now performs the update here.*/
                l1_sc_check_for_ncc_permitted_update(gas_id);

                break;
            }

            /********************************************************
             *                                                         *
             *                MPH_FREQUENCY_REDEFINITION_REQ           *
             *                                                         *
             ***********************************************************/

        case MPH_FREQUENCY_REDEFINITION_REQ:
            {
                mph_frequency_redefinition_req_T  *message_ptr;
                word                               stMod42432;
                byte                               index, num_arfcns;
                boolean                            channels_in_same_band;
                boolean                            starting_time_elapsed;
                dedicated_data_T                  *l1_dedicated_data_ptr;
                gas_id_t gas_id;

                message_ptr = (mph_frequency_redefinition_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                /* check whether the starting time has elapsed */
                stMod42432 = (message_ptr->starting_time[0] * 256) + (message_ptr->starting_time[1]%256);
                starting_time_elapsed = L1_determine_time(frame_counters[gas_id].FNmod42432,stMod42432);

                /* check whether the supplied list of ARFCNs are all in the same band */
                num_arfcns = message_ptr->channel_information.frequency_list.no_of_items;
                channels_in_same_band = TRUE;
                for (index = 1; index < num_arfcns; index++)
                {
                  if (message_ptr->channel_information.frequency_list.channel[index-1].band !=
                      message_ptr->channel_information.frequency_list.channel[index].band)
                  {
                     channels_in_same_band = FALSE;
                     break;
                  }
                }

                /* only process the redefinition request if the channels are all in the same band */
                if (channels_in_same_band == TRUE)
                {
                  /* copy parameters over from the message */
                  l1_dedicated_data_ptr->freq_redef_starting_time = stMod42432;
                  l1_dedicated_data_ptr->new_channel_info         = message_ptr->channel_information;
                  l1_dedicated_data_ptr->freq_redef_imminent      = TRUE;
                }

                /* send results of the validation back to RR */
                L1_send_MPH_FREQUENCY_REDEFINITION_IND(channels_in_same_band,starting_time_elapsed,gas_id);

                break;
            }

            /********************************************************
             *                                                         *
             *                MPH_HANDOVER_REQ                         *
             *                                                         *
             ***********************************************************/

        case MPH_HANDOVER_REQ:
            {
                mph_handover_req_T        *message_ptr;
                boolean                   handover_is_possible = TRUE;
                l1_sc_bs_T                *target_cell_details;
                uint8                     target_cell_bsic;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T          *l1_dedicated_data_ptr;
                idle_data_T               *l1_idle_data;
                gas_id_t gas_id;

                message_ptr = (mph_handover_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
                gl1_hw_pow_mon_drop(gas_id);
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

                /* Always reset the iRAT handover Ind used for AGC. */
                l1_interrat_ho_update(FALSE, gas_id);

                l1_ded_if_data[gas_id].do_gsm_blind_handover = FALSE;
                l1_ded_if_data[gas_id].old_channel_DCCH = NULL;

                l1_idle_data = &l1_idle_data_store[gas_id];

                target_cell_bsic = message_ptr->BSIC.BS_colour_code + (message_ptr->BSIC.PLMN_colour_code << 3);

                MSG_GERAN_HIGH_3_G("MPH_HANDOVER_REQ to Cell %d BSIC:%d",
                  message_ptr->BCCH_ARFCN.num,
                  target_cell_bsic,
                  0);


                /* Determine the type of the old channel */
                if (l1_tskisr_blk->sub_state == L1_DED_BEFORE)
                {
                  l1_dedicated_data_ptr->previous_chnl_type =
                    l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
                }
                else
                {
                  l1_dedicated_data_ptr->previous_chnl_type =
                    l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type;
                }

                if ( l1_dedicated_data_ptr->previous_chnl_type == FULL_RATE_TRAFFIC )
                {
                    l1_ded_if_data[gas_id].old_channel_DCCH = FACCH_F;
                }
                else if ( l1_dedicated_data_ptr->previous_chnl_type == HALF_RATE_TRAFFIC )
                {
                    l1_ded_if_data[gas_id].old_channel_DCCH = FACCH_H;
                }
                else
                {
                    l1_ded_if_data[gas_id].old_channel_DCCH = SDCCH;
                }

                if (!ARFCNS_EQUAL(message_ptr->BCCH_ARFCN, l1_idle_data->campedon_cell_ARFCN) ||
                    (target_cell_bsic != l1_idle_data->campedon_cell_BSIC) )
                {
                    MSG_GERAN_MED_0_G("Inter Cell Handover ");

                    /* Get target cell details */
                    target_cell_details =
                    l1_sc_balist_get_cell_data( message_ptr->BCCH_ARFCN,gas_id );

                    if ( target_cell_details )
                    {
                        l1_dedicated_data_ptr->target_cell_details = *target_cell_details;
                        MSG_GERAN_HIGH_2_G("Target cell qbit lag=%d fn_lag=%d", target_cell_details->qbit_lag,target_cell_details->fn_lag);

                        /* Check if handover is possible - validate timing advance */
                        if ( (l1_dedicated_data_ptr->target_cell_details.bsic_known) &&  
                              (l1_dedicated_data_ptr->target_cell_details.bsic == target_cell_bsic) )
                        {
                            handover_is_possible = gl1_is_handover_possible( message_ptr );

                        }
                        else
                        {
                            MSG_GERAN_HIGH_0_G("BSIC not known...trying GSM->GSM Blind Handover ");
                            handover_is_possible = FALSE;
                            l1_ded_if_data[gas_id].do_gsm_blind_handover = TRUE;
                        }
                        l1_ded_if_data[gas_id].l1_target_ncell_unavailable = FALSE;
                    }
                    else
                    {
                        MSG_GERAN_HIGH_0_G("Target cell unavailable...trying GSM->GSM Blind Handover ");
                        handover_is_possible = FALSE;
                        l1_ded_if_data[gas_id].do_gsm_blind_handover = TRUE;

                        l1_ded_if_data[gas_id].l1_target_ncell_unavailable = TRUE;
                    }

                    if ( handover_is_possible )
                    {
                        /* Sync needed for inter-cell handovers */
                        l1_ded_if_data[gas_id].need_to_sync = TRUE;
                    }
                    else if ( !l1_ded_if_data[gas_id].do_gsm_blind_handover )
                    {
                        L1_send_MPH_HANDOVER_IND(HANDOVER_OUT_OF_RANGE, gas_id);

                        /* Resume signalling on the old channel */
                        L1_send_PH_READY_TO_SEND_IND(SACCH, gas_id);
                        if ( l1_ded_if_data[gas_id].old_channel_DCCH != NULL )
                        {
                            L1_send_PH_READY_TO_SEND_IND(l1_ded_if_data[gas_id].old_channel_DCCH, gas_id);
                        }
                        else
                        {
                            MSG_GERAN_ERROR_0_G("Rcvd Null Old_channel_DCCH");
                        }
                    }

                }
                else
                {
                    MSG_GERAN_MED_0_G("Intra Cell Handover ");

                    /* This is a handover to the same cell - used in 11.10 13.3.4.1 */
                    /* Save old ARFCN and BSIC in case of a handover failure */
                    l1_dedicated_data_ptr->target_cell_details.arfcn =
                    l1_idle_data->campedon_cell_ARFCN;

                    l1_dedicated_data_ptr->target_cell_details.bsic =
                    l1_idle_data->campedon_cell_BSIC;

                    l1_ded_if_data[gas_id].need_to_sync = FALSE;

                    l1_ded_if_data[gas_id].l1_target_ncell_unavailable = FALSE;
                }

                /* Continue with the Handover if all seems OK up to now
                */
                if ( handover_is_possible || l1_ded_if_data[gas_id].do_gsm_blind_handover )
                {
                    /* Tell CGPS we are leaving this cell */
                    gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);

                    /* ERSACCH - Inter Cell Handover now in progress*/
                    if(l1_ded_if_data[gas_id].need_to_sync == TRUE)
                    {
                      l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = TRUE;
                    }
                    else
                    {
                      l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;
                    }

                    /* Save off msg id of message currently being processed
                     * If current_rr_l1_msg_id is not NULL (its reset in
                     * gl1_setup_new_channel), we are currently processing a req
                     * from RR, so don't set this variable as it will mess up the
                     * request being processed
                     */
#ifdef FEATURE_GSM_DTM
                    /*
                    Dont save messages and set the main command if actually in DTM.
                    This can happen as RR messages received in DTM are processed here
                    */

                    if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                    {
                        l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                    }
                    else
                    {
                        MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                    }

                    /* Suspend L1 ISR from doing anymore */
                    l1_tskisr_blk->main_command = L1_DEDICATED_STOP;

#else

                    if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                    {
                        l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                    }
                    else
                    {
                        MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                    }

                    /* Suspend L1 ISR from doing anymore */
                    l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
#endif
                    /* stop SCE */
                    l1_sc_stop(gas_id);

                    /* Now that the ISR is suspended, save the old cell data in
                       case we need to reconnect to the old cell in case of a failed
                       handover
                    */
                    gl1_save_current_cell_data( TRUE, gas_id );

                    if ( l1_ded_if_data[gas_id].do_gsm_blind_handover )
                    {
                        /* Store handover details for later use */
                        gl1_store_handover_details ( message_ptr );
                    }

                    /* Now fill in the new dedicated data */
                    memscpy ( &l1_dedicated_data_ptr->new_channel_specification,
                             sizeof(l1_dedicated_data_ptr->new_channel_specification),
                             &message_ptr->channel_spec,
                             sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
                    /* Now fill in the multirate configuration data */
                    memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                             sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                             &message_ptr->multirate_config,
                             sizeof(multirate_config_T) );
#endif
                    l1_dedicated_data_ptr->new_starting_time =
                    (message_ptr->channel_spec.starting_time[0] * 256) +
                    (message_ptr->channel_spec.starting_time[1] % 256);

                    l1_dedicated_data_ptr->target_cell_details.arfcn = message_ptr->BCCH_ARFCN;
                    l1_dedicated_data_ptr->handover_synchronization = message_ptr->handover_synchronisation;

#ifdef FEATURE_WLAN_COEX_SW_CXM
                    garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_HO,gas_id);
#endif
                }
                else
                {
                    /*an error has occurred*/
                    MSG_GERAN_ERROR_0_G("Handover not possible");

#ifdef  DEBUG
                    gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
                                          GS_LOG_TYPE_TASK_PROGRESS,
                                          debug_string);
#endif
                }

                /* otherwise it stays on the original channel while RR sends
                   a HANDOVER FAIL message */

                break;
            }

            /********************************************************
             *                                                         *
             *                MPH_RECONNECT_CHANNEL_REQ                *
             *                                                         *
             ***********************************************************/

        case MPH_RECONNECT_CHANNEL_REQ:
            {
                mph_reconnect_channel_req_T *message_ptr;
                volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (mph_reconnect_channel_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
                gl1_msg_set_iq_state( FALSE, gas_id );
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */


                /* Previous cell applies only to inter-cell handovers */
                l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;


                /* Determine if the reconnect is to a new cell */
                if (!ARFCNS_EQUAL(l1_dedicated_data_ptr->target_cell_details.arfcn,
                                  l1_dedicated_data_ptr->current_cell_details.arfcn))
                {
                    l1_ded_if_data[gas_id].need_to_sync = TRUE;

                    /* Tell CGPS we are leaving this cell */
                    gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);
                   /* ESACCH Inter HO failed */
                  l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = TRUE;
                  l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail = TRUE;
                }
                else
                {
                    l1_ded_if_data[gas_id].need_to_sync = FALSE;
                }

                /* Stop SCE */
                l1_sc_stop(gas_id);

                /* Now fill in the new dedicated data */
                memscpy ( &l1_dedicated_data_ptr->new_channel_specification,
                         sizeof(l1_dedicated_data_ptr->new_channel_specification),
                         &message_ptr->channel_spec,
                         sizeof(channel_spec_T) );

                l1_dedicated_data_ptr->new_starting_time =
                (message_ptr->channel_spec.starting_time[0] * 256) +
                (message_ptr->channel_spec.starting_time[1]%256);

#ifdef FEATURE_GSM_AMR
                    /* Now fill in the multirate configuration data */
                    memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                             sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                             &message_ptr->multirate_config,
                             sizeof(multirate_config_T) );
#endif

                /*
                If when previously on this cell the starting time had expired then
                invalidate this starting time here. The value of the starting time must also
                be cleared as this is sometimes used without the check for starting_time_valid
                in l1_ded.c
                */
                if ( l1_dedicated_data_ptr->previous_cell_on_after_channels == TRUE )
                {
                  l1_dedicated_data_ptr->channel_specification.starting_time_valid = FALSE;
                  l1_dedicated_data_ptr->new_starting_time = 0;
                }

                /* Stop sending access bursts */
                l1_dedicated_data_ptr->no_of_ab_to_send = 0;
                GL1_ISR_LOCK(gas_id);
                gl1_msg_abort_rach(gas_id);
                GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_DTM
                /*
                Dont save messages and set the main command if actually in DTM.
                This can happen as RR messages received in DTM are processed here
                */
                if( l1_tskisr_blk->l1_state != L1_DTM_MODE )
                {
                  if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                  {
                    l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                  }
                  else
                  {
                    MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                  }

                  /* Suspend L1 ISR from doing anymore */
                  l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
                }
#else

                /* Save off msg id of message currently being processed
                 * If current_rr_l1_msg_id is not NULL (its reset in
                 * gl1_setup_new_channel), we are currently processing a req
                 * from RR, so don't set this variable as it will mess up the
                 * request being processed
                 */
                if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                {
                    l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                }
                else
                {
                    MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                }

                /* Suspend L1 ISR from doing anymore */
                l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
#endif
                break;

            }

            /********************************************************
             *                                                         *
             *                MPH_SET_CIPHERING_MODE_REQ               *
             *                                                         *
             ***********************************************************/

        case MPH_SET_CIPHERING_MODE_REQ:
            {
                mph_set_ciphering_mode_req_T  *message_ptr;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;
                gas_id_t gas_id;

                message_ptr = (mph_set_ciphering_mode_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                /*
                Copy ciphering info from message to shared memory for ISR.
                No need to stop and restart the ISR since this will
                reinitialize the channel.  The INTLOCK is imp while invoking
                L1_transfer_new_data to copy data to ISR
                */
                l1_dedicated_data_ptr->channel_specification.cipher_flag =
                message_ptr->cipher_flag;
                l1_dedicated_data_ptr->channel_specification.cipher_algorithm =
                message_ptr->cipher_algorithm;

                memscpy(l1_dedicated_data_ptr->cipher_key,
               sizeof(l1_dedicated_data_ptr->cipher_key),
                        message_ptr->cipher_key,
                        CIPHER_KEY_LENGTH);

                memscpy(l1_dedicated_data_ptr->cipher_key_128,
               sizeof(l1_dedicated_data_ptr->cipher_key_128),
                       message_ptr->cipher_key_128,
                       CIPHER_KEY_128_LENGTH);

                /* retain as much data as possible */
                l1_dedicated_data_ptr->dsp_tch_init = 2;

                l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;

                GL1_ISR_LOCK(gas_id);
                L1_transfer_new_data( gas_id);
                l1_tskisr_blk->command = L1_SET_CIPHERING_COMMAND;
                GL1_ISR_UNLOCK(gas_id);

                break;

            }

            /********************************************************
             *                                                         *
             *                MPH_STOP_HANDOVER_ACCESS_REQ             *
             *                                                         *
             ***********************************************************/

        case MPH_STOP_HANDOVER_ACCESS_REQ:
            {
                mph_stop_handover_access_req_T  *message_ptr;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;
                gas_id_t gas_id;

                message_ptr = (mph_stop_handover_access_req_T *)msg_header; /*lint !e740 */
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                temp = message_ptr->timing_advance;

                if (temp > 63)
                {
                  MSG_GERAN_ERROR_0_G("TA sent in Physical Info is not in the range 0-63");
                }
                else
                {
                  gl1_msg_store_ta( temp,gas_id);
                  l1_dedicated_data_ptr->timing_advance_in_use = temp;
                  l1_dedicated_data_ptr->timing_advance_to_use = temp;
                  l1_dedicated_data_ptr->ordered_timing_advance = temp;
                }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
                gl1_msg_set_iq_state( FALSE, gas_id );
#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

                /* Stop sending access bursts */
                l1_dedicated_data_ptr->no_of_ab_to_send = 0;
                GL1_ISR_LOCK(gas_id);
                gl1_msg_abort_rach(gas_id);
                GL1_ISR_UNLOCK(gas_id);

                /*Calculate observed timing difference in bits*/
                if (l1_dedicated_data_ptr->target_cell_details.bsic_known)
                {
                    /* Note: OTD and t0 values should be from the old
                     * cell's perspective, not the one we've handed off to.
                     * Report OTD in bit periods.  RR converts to half bit
                     * period. RR also adds the TA value so only need to
                     * send OTD
                     */

                    l1_dedicated_data_ptr->observed_timing_difference = l1_ded_if_data[gas_id].otd/4;

                }

                else
                {
                    l1_dedicated_data_ptr->observed_timing_difference = 0;
                }

                /*send confirmation to L3*/
                L1_send_MPH_STOP_HANDOVER_ACCESS_CNF(l1_dedicated_data_ptr->
                                                     observed_timing_difference,
                                                     l1_dedicated_data_ptr->
                                                     last_cell_timing_advance,
                                                     gas_id);

                /* Inform L2 */

                L1_send_PH_READY_TO_SEND_IND(SACCH, gas_id);

                if ( l1_tskisr_blk->sub_state == L1_DED_BEFORE )
                {
                    if (l1_dedicated_data_ptr->channel_specification.
                        channel_info_1_before.channel_type == FULL_RATE_TRAFFIC)
                    {
                        L1_send_PH_READY_TO_SEND_IND(FACCH_F, gas_id);
                    }
                    else if (l1_dedicated_data_ptr->channel_specification.
                             channel_info_1_before.channel_type == HALF_RATE_TRAFFIC)
                    {
                        L1_send_PH_READY_TO_SEND_IND(FACCH_H, gas_id);
                    }
                    else
                    {
                        L1_send_PH_READY_TO_SEND_IND(SDCCH, gas_id);
                    }
                }

                else
                {
                    if ( l1_dedicated_data_ptr->channel_specification.
                         channel_info_1_after.channel_type == FULL_RATE_TRAFFIC)
                    {
                        L1_send_PH_READY_TO_SEND_IND(FACCH_F, gas_id);
                    }
                    else if (l1_dedicated_data_ptr->channel_specification.
                             channel_info_1_after.channel_type == HALF_RATE_TRAFFIC)
                    {
                        L1_send_PH_READY_TO_SEND_IND(FACCH_H, gas_id);
                    }
                    else
                    {
                        L1_send_PH_READY_TO_SEND_IND(SDCCH, gas_id);
                    }
                }
#ifdef FEATURE_WLAN_COEX_SW_CXM
                garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_HO,gas_id);
#endif
                break;
            }


            /********************************************************
             *                                                      *
             *       MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST_REQ       *
             *                                                      *
             ********************************************************/

        case MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST_REQ:
            {
                mph_dedicated_wcdma_cell_update_list_req_T* msg;
                msg = ( mph_dedicated_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 */
                l1_sc_ded_wcdma_update_list( msg, msg->gas_id );
                break;
            }

            /********************************************************
             *                                                      *
             *                MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ     *
             *                                                      *
             ********************************************************/

        case MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ:
          {
              mph_specific_irat_freq_update_req_T* msg;
              msg = ( mph_specific_irat_freq_update_req_T* )msg_header;

              l1_sc_specific_irat_freq_update(msg, msg->gas_id);
              break;
          }


            /********************************************************
             *                                                      *
             *                MPH_STOP_GSM_MODE_REQ                 *
             *                                                      *
             ********************************************************/

        case MPH_STOP_GSM_MODE_REQ:
            {
                mph_stop_gsm_mode_req_T *message_ptr;
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
                dedicated_data_T *l1_dedicated_data_ptr;

                message_ptr = (mph_stop_gsm_mode_req_T *)msg_header;
                gas_id = check_gas_id(message_ptr->gas_id);
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
                GL1_ISR_LOCK(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                l1_tsk_buffer[gas_id].g2x_resel_in_progress = message_ptr->g2x_resel_in_progress;
                MSG_GERAN_HIGH_1_G("L1_handle_dedicated_mode: MPH_STOP_MODE_REQ, g2x_resel_in_progress:%d", l1_tsk_buffer[gas_id].g2x_resel_in_progress);
#endif
                gl1_save_current_cell_data( FALSE,gas_id );

                MSG_GERAN_HIGH_1_G("MPH_STOP_GSM_MODE_REQ x2g_ho_fail %d",message_ptr->x2g_ho_fail);

                /*Check whether stop mode is for X2G HO failure*/
                l1_tskisr_blk->x2g_inter_rat_ho_triggered = message_ptr->x2g_ho_fail;

                /* Stop SCE */
                l1_sc_stop(gas_id);
                GL1_ISR_UNLOCK(gas_id);
                /* Stop L1 ISR */
                l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
                if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id == NULL )
                {
                    l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;
                }
                else
                {
                    MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: New req 0x%x",l1_ded_if_data[gas_id].current_rr_l1_msg_id,msg_header->message_id);
                }
#endif
                /* Wait for L1 ISR to stop */
                while (l1_tskisr_blk->sub_state != L1_DED_SUSPEND);  /*lint !e722*/

#ifdef FEATURE_GSM_DTM
                if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
                {
                  /* Now change the mode back to the lowest TN Slam */
                  l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

                }
#endif /* FEATURE_GSM_DTM */

                /* Change the ISR mode */
                L1_setup_DEACTIVATE( msg_header );
                break;
            }

            /********************************************************
            *                                                       *
            *                MPH_STOP_NC_MEASUREMENT_REQ            *
            *                                                       *
            ********************************************************/
        case MPH_STOP_NC_MEASUREMENTS_REQ:
            {
              mph_stop_nc_measurements_req_t *message_ptr;
              gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;

              message_ptr = ( mph_stop_nc_measurements_req_t *) msg_header;
              gas_id = check_gas_id(message_ptr->gas_id);

              gpl1_gprs_meas_mode_ptr =
                gl1_ms_switch_meas_mode_data(gas_id);
              gpl1_gprs_meas_mode_ptr->nc_mode = FALSE;

              MSG_GERAN_HIGH_0_G("MPH_STOP_NC_MEASUREMENTS_REQ");

              break;
            }

         /*******************************************************
         *                                                      *
         *                MPH_SET_PRIORITY_IND                  *
         *                                                      *
         ********************************************************/
         case MPH_SET_PRIORITY_IND:
             {
               mph_set_priority_ind_T* message_ptr = (mph_set_priority_ind_T* )msg_header;
               gas_id = check_gas_id(message_ptr->gas_id);

               if (message_ptr->procedure_type == GERAN_CONN_PROCEDURE)
               {
                   grm_check_connected_reason(
                   l1_tsk_buffer[gas_id].client_id,
                   GRM_CS_CONNECTION,
                   gas_id);
               }
               break;
             }

        default:
            MSG_GERAN_ERROR_1("RR->L1(DED) message %d ignored.", msg_header->message_id);
            break;
        } /* end if switch */
    }

    else if (msg_header->message_set == MS_L1_L1)
    {
        switch (msg_header->message_id)
        {


        /********************************************************
         *                                                         *
         *                L1_REP_PERIOD_IND                        *
         *                                                         *
         ***********************************************************/

        case L1_REP_PERIOD_IND:
            {
                l1_rep_period_ind_T* message_ptr = ( l1_rep_period_ind_T* )msg_header;
                gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
                l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
                l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
                dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
                volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

                /* The reporting period is over, first do serving cell reporting */
                uint8  rxlev_full,prxlev_full,rxlev_sub,prxlev_sub;
                byte   rxqual_full,rxqual_sub;
                uint8 rxlev_val,prxlev_val, mean_bep, cv_bep;
#ifdef FEATURE_GSM_RX_DIVERSITY
                uint8 divrxlev_full,divrxlev_sub,divrxlev_val;
                gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                boolean gl1_was_dtx_used = FALSE;
                boolean gl1_in_sdcch_configuration = FALSE;
                l1_tskisr_blk = &l1_tsk_buffer[gas_id];


                /* average all the power measurements made this frame */
                ENTER_NO_INTERRUPTS


               gl1_was_dtx_used = gl1_get_dtx_used_this_period(gas_id);

                /* RXLEV_FULL Average */
                if (l1_serving_cell_meas_ptr->no_of_measurements != 0)
                {
                    l1_serving_cell_meas_ptr->rx_power /=l1_serving_cell_meas_ptr->no_of_measurements;
                    prxlev_full = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->rx_power);
#ifdef FEATURE_GSM_RX_DIVERSITY
                    if((gl1_get_rxd_ctl_nv(gas_id) == TRUE) &&(l1_serving_cell_meas_ptr->divrx_power != 0)
                                 && (l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period == FALSE))
                      {
                        l1_serving_cell_meas_ptr->divrx_power /=l1_serving_cell_meas_ptr->no_of_measurements;
                        divrxlev_full = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->divrx_power);
                        rxlev_full = MAX(prxlev_full,divrxlev_full);
                        MSG_GERAN_MED_5_G("GSM Serving Cell Measure: prx_rxlev=%d, divrx_rxlev=%d "
                                        "RXLEV_average=%d, rx_power=%d, divrx_power=%d",
                                        prxlev_full,
                                        divrxlev_full,
                                        rxlev_full,
                                        l1_serving_cell_meas_ptr->rx_power,
                                        l1_serving_cell_meas_ptr->divrx_power );
                      }
                      else
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                      {
                        rxlev_full = prxlev_full;
                      }
                }
                else
                {
                    rxlev_full = l1_ded_if_data[gas_id].prev_rxlev_full;
                    MSG_GERAN_HIGH_1_G("RXLEV_FULL: No meas in this rep period Rx Power=%d",l1_serving_cell_meas_ptr->rx_power);
                }

                /* RXLEV_SUB Average */
                /* Sometimes SDCCH->TCH transitions can leave us with no measurements
                 * to average.  When this happens just use the previous value.
                 */
                if (l1_serving_cell_meas_ptr->no_of_measurements_sub != 0)
                {
                    l1_serving_cell_meas_ptr->rx_power_sub /=l1_serving_cell_meas_ptr->no_of_measurements_sub;
                    prxlev_sub  = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->rx_power_sub);
#ifdef FEATURE_GSM_RX_DIVERSITY
                    if((gl1_get_rxd_ctl_nv(gas_id) == TRUE) &&(l1_serving_cell_meas_ptr->divrx_power_sub != 0) 
                                          && (l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period == FALSE))
                      {
                        l1_serving_cell_meas_ptr->divrx_power_sub /=l1_serving_cell_meas_ptr->no_of_measurements_sub;
                        divrxlev_sub = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->divrx_power_sub);
                        rxlev_sub = MAX(prxlev_sub,divrxlev_sub);
                        MSG_GERAN_MED_5_G("GSM Serving Cell Measure: prx_rxlev_sub=%d, divrx_rxlev_sub=%d "
                                        "RXLEV_SUB_average=%d, rx_power_sub=%d, divrx_power_sub=%d",
                                        prxlev_sub,
                                        divrxlev_sub,
                                        rxlev_sub,
                                        l1_serving_cell_meas_ptr->rx_power_sub,
                                        l1_serving_cell_meas_ptr->divrx_power_sub );                        
                      }
                      else
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                      {
                        rxlev_sub = prxlev_sub;
                      }
                }
                else
                {
                    rxlev_sub = l1_ded_if_data[gas_id].prev_rxlev_sub;
                    prxlev_sub = rxlev_sub;
                    MSG_GERAN_HIGH_2_G("RXLEV_SUB: No meas in this rep period pwr=%d, sub=%d",l1_serving_cell_meas_ptr->rx_power,l1_serving_cell_meas_ptr->rx_power_sub);
                }

                /* RXLEV_VAL Average */
                if (l1_serving_cell_meas_ptr->no_of_measurements_val != 0)
                {
                   l1_serving_cell_meas_ptr->rx_power_val /=l1_serving_cell_meas_ptr->no_of_measurements_val;
                   prxlev_val = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->rx_power_val);
#ifdef FEATURE_GSM_RX_DIVERSITY
                   if((gl1_get_rxd_ctl_nv(gas_id) == TRUE) &&(l1_serving_cell_meas_ptr->divrx_power_val != 0)
                               && (l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period == FALSE))
                     {
                       l1_serving_cell_meas_ptr->divrx_power_val /=l1_serving_cell_meas_ptr->no_of_measurements_val;
                       divrxlev_val = L1_map_dBm_x16_to_rxlev((dBx16_T)l1_serving_cell_meas_ptr->divrx_power_val);
                       rxlev_val = MAX(prxlev_val,divrxlev_val);
                       MSG_GERAN_MED_5_G("GSM Serving Cell Measure: prxlev_val=%d, divrxlev_val=%d "
                                         "rxlev_val=%d, rx_power_val=%d, divrx_power_val=%d",
                                         prxlev_val,
                                         divrxlev_val,
                                         rxlev_val,
                                         l1_serving_cell_meas_ptr->rx_power_val,
                                         l1_serving_cell_meas_ptr->divrx_power_val );
                     }
                     else
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                     {
                       rxlev_val = prxlev_val;
                     }
                }
                else
                {
                  rxlev_val = 0;
                }

                if (l1_serving_cell_meas_ptr->rx_power_sub <
                    SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16)
                {
                    l1_set_serving_RLA_C(SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16, gas_id);
                }
                else
                {
                    l1_set_serving_RLA_C((dBx16_T)l1_serving_cell_meas_ptr->rx_power_sub, gas_id);
                }
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
                // In case of GPLT and no RF, return default/canned result
                MSG_GERAN_HIGH_1_G("GPLT DBG: Making serving RLA_C %d", gplt_g2t_ded_irat_meas_data.serving_RLA_C);
                l1_set_serving_RLA_C(gplt_g2t_ded_irat_meas_data.serving_RLA_C);
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

                /* save rxlev_sub/rxlev_full in case we need to use it again */
                l1_ded_if_data[gas_id].prev_rxlev_sub  = rxlev_sub;
                l1_ded_if_data[gas_id].prev_rxlev_full = rxlev_full;

                /* RxQUAL_FULL Average */
                rxqual_full = gl1_msg_get_rxqual_full((boolean*)&meas_report_valid,gas_id);
                /* RxQUAL_SUB Average */
                rxqual_sub  = gl1_msg_get_rxqual_sub( gas_id);

                /* Engineering mode, update Rxqual sub for dedicated mode */
                geran_eng_mode_data_write(ENG_MODE_RX_QUAL, &rxqual_sub, TRUE, gas_id);

#ifdef FEATURE_QSH_EVENT_METRIC
#ifdef FEATURE_GSM_RX_DIVERSITY
                if (gl1_get_enable_rxdiversity(gas_id)) {
                  /*Before Re-Set Cache for QSH*/
                  gl1_qsh_cache_tch_rx_params (l1_serving_cell_meas_ptr->rx_power, 
                                               l1_serving_cell_meas_ptr->rx_power_sub,
                                               l1_serving_cell_meas_ptr->divrx_power, 
                                               l1_serving_cell_meas_ptr->divrx_power_sub,
                                               rxqual_full, 
                                               rxqual_sub,
                                               gas_id);
                }
                else {
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                  /*Before Re-Set Cache for QSH*/
                  gl1_qsh_cache_tch_rx_params (l1_serving_cell_meas_ptr->rx_power, 
                                               l1_serving_cell_meas_ptr->rx_power_sub,
                                               0, 
                                               0,
                                               rxqual_full, 
                                               rxqual_sub,
                                               gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
                }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#endif /*FEATURE_QSH_EVENT_METRIC*/

                /* Reset RXLEV accumulations for next reporting period */
                l1_serving_cell_meas_ptr->rx_power               = 0;
                l1_serving_cell_meas_ptr->no_of_measurements     = 0;
                l1_serving_cell_meas_ptr->rx_power_sub           = 0;
                l1_serving_cell_meas_ptr->no_of_measurements_sub = 0;
                l1_serving_cell_meas_ptr->rx_power_val           = 0;
                l1_serving_cell_meas_ptr->no_of_measurements_val = 0;

#ifdef FEATURE_GSM_RX_DIVERSITY
                l1_serving_cell_meas_ptr->divrx_power            = 0;
                l1_serving_cell_meas_ptr->divrx_power_sub        = 0;
                l1_serving_cell_meas_ptr->divrx_power_val        = 0;
                l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

               



                if(l1_tskisr_blk->sub_state == L1_DED_AFTER)
                 {
                 if((l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type == SDCCH_4)
                     || (l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type == SDCCH_8)
                   )
                   {
                   gl1_in_sdcch_configuration = TRUE;
                   MSG_GERAN_HIGH_2_G("Debug:DED_AFTER, SDCCH:%d,RXQUAL_FULL:%d",gl1_in_sdcch_configuration,gl1_msg_get_hard_rxqual_full_avg(gas_id));
                   }
                 }
                else if(l1_tskisr_blk->sub_state == L1_DED_BEFORE)
                 {
                 if((l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type == SDCCH_4)
                     || (l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type == SDCCH_8)
                   )
                   {
                   gl1_in_sdcch_configuration = TRUE;
                   MSG_GERAN_HIGH_2_G("Debug:DED_BEFORE, SDCCH:%d,RXQUAL_FULL:%d",gl1_in_sdcch_configuration,gl1_msg_get_hard_rxqual_full_avg(gas_id));
                   }
                 }

#ifdef FEATURE_GSM_DYNAMIC_SAIC
                /* If SAIC NV is disabled then don't run SAIC disable code */
                if((gl1_hw_feature_disabled_saic(gas_id) != TRUE) && (!gl1_in_sdcch_configuration))
                {
                    MSG_GERAN_LOW_3_G(" RXQUAL %d saic d/e %d state %d",gl1_msg_get_hard_rxqual_full_avg(gas_id),
                                                              gl1_msg_get_saic_state(gas_id),
                                                              l1_dedicated_data_ptr->l1_saic_dis_ena_state);

                  /* If we have GOOD RXQUAL and SAIC is on, turn it off */
                  if( (gl1_msg_get_hard_rxqual_full_avg(gas_id) == L1_PERFECT_RXQUAL) &&
                      (gl1_msg_get_saic_state(gas_id) == TRUE) &&
                      (l1_dedicated_data_ptr->l1_saic_dis_ena_state == FALSE) &&
                      (!(gl1_rxd_ctl_flag.RxdForcedOn && gl1_in_ftm_mode))  )
                  {
                    /* Turn SAIC OFF */
                    gl1_hw_cfg_saic(FALSE, gas_id);
                    /* temporarily disable dynamic AEQ clock bump and remain at voice AEQ Q6 speed */
                    //gl1_hw_mcpm_amr_aeq_update(FALSE, gas_id);
                  }
                }
#endif

                if ( l1_dedicated_data_ptr->prev_rxqual != RXD_INVALID_RXQUAL )
                {
                  uint8 rxqual = gl1_was_dtx_used ? rxqual_sub : rxqual_full;
                  if ( ( rxqual + l1_dedicated_data_ptr->prev_rxqual )/2 < RXD_RXQUAL_THRESHOLD )
                  {
                    l1_dedicated_data_ptr->schedule_fading_cells = FALSE;
                  }
                  else
                  {
                    l1_dedicated_data_ptr->schedule_fading_cells = TRUE; 
                  }
                }
                else
                {
                  l1_dedicated_data_ptr->schedule_fading_cells = FALSE;                
                }

#ifdef FEATURE_GSM_RX_DIVERSITY
                gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
                if(gl1_rxd_ctl_flag.enableRxd)
                  {
                  if(gl1_was_dtx_used)
                    {
                     MSG_GERAN_HIGH_2_G("rxqual_sub:%d,FN:%d",rxqual_sub,gl1_get_FN(gas_id));
                     if(l1_dedicated_data_ptr->l1_enable_rxdiversity == TRUE)
                       {
                       if((rxqual_sub < RXD_RXQUAL_THRESHOLD) &&
                          (l1_dedicated_data_ptr->prev_rxqual != RXD_INVALID_RXQUAL)
              && (FALSE == gl1_get_divrx_stream_sel_rate(gas_id))
              && (!(gl1_rxd_ctl_flag.RxdForcedOn && gl1_in_ftm_mode))
                          )
                         {
                         l1_dedicated_data_ptr->l1_enable_rxdiversity = FALSE;
                         l1_dedicated_data_ptr->prev_rxqual = RXD_INVALID_RXQUAL;
                         gl1_set_enable_rxdiversity(FALSE,gas_id);
                         }
                       else
                         {
                         l1_dedicated_data_ptr->prev_rxqual = rxqual_sub;
                         }
                       }
                     }
                   else
                     {
                     MSG_GERAN_HIGH_2_G("rxqual_full:%d,FN:%d",rxqual_full,gl1_get_FN(gas_id));
                     if(l1_dedicated_data_ptr->l1_enable_rxdiversity == TRUE)
                       {
                       if((rxqual_full < RXD_RXQUAL_THRESHOLD) &&
                          (l1_dedicated_data_ptr->prev_rxqual != RXD_INVALID_RXQUAL)
               && (FALSE == gl1_get_divrx_stream_sel_rate(gas_id))
               && (!(gl1_rxd_ctl_flag.RxdForcedOn && gl1_in_ftm_mode))
                          )
                          {
                          l1_dedicated_data_ptr->l1_enable_rxdiversity = FALSE;
                          l1_dedicated_data_ptr->prev_rxqual = RXD_INVALID_RXQUAL;
                          gl1_set_enable_rxdiversity(FALSE,gas_id);
                          }
                        else
                          {
                          l1_dedicated_data_ptr->prev_rxqual = rxqual_full;
                          }
                       }
                     }
          /*  reset only total cnt and DRx cnt for averaging. keep previous average   */
           gl1_reset_divrx_sel_avg(FALSE,gas_id);
                   }
#endif

#ifdef FEATURE_AEQ_PHASE4
                mdsp_aeq4_ctl_info = gl1_get_aeq4_info(gas_id);
                MSG_GERAN_MED_3_G("AEQ4 status : control_nv=%d, enableAeq4=%d, sacch_cnt=%d",
                 mdsp_aeq4_ctl_info.control_nv,mdsp_aeq4_ctl_info.enableAeq4,mdsp_aeq4_ctl_info.sacch_cnt);
        if(
           (l1_get_tch_chan_mode(gas_id) == ENG_MODE_HALF_RATE) ||
           (l1_get_tch_chan_mode(gas_id) == ENG_MODE_AMR_HALF_RATE) ||
           (l1_tskisr_blk->l1_state != L1_DEDICATED_MODE)
          )
          {
            if(mdsp_aeq4_ctl_info.enableAeq4)
              {
               gl1_clkdata_speed(FALSE, L1_DED_SPEEDUP_AEQ_PH4, L1_DED_SPEEDUP_AEQ, L1_DED_AEQ_PH4, gas_id);
              }
            mdsp_aeq4_ctl_info.sacch_cnt  = 0xFF;
            mdsp_aeq4_ctl_info.enableAeq4 = FALSE;
            gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_ENABLED_AND_SACCH_CNT,gas_id);
            MSG_GERAN_HIGH_0("AEQ4 feature is disabled for HALF RATE channels and for DTM mode!");
          }
        else if ((rxqual_full != 255) && (l1_get_tch_chan_mode(gas_id)!= 255) )
          {
                   if(mdsp_aeq4_ctl_info.control_nv && gl1_hw_get_dedicated(gas_id))
            {
             if(gl1_rxd_ctl_flag.RxdForcedOn && gl1_in_ftm_mode)
                  {
                    if(!mdsp_aeq4_ctl_info.enableAeq4)
                      {
                        mdsp_aeq4_ctl_info.sacch_cnt   = 0;
                        mdsp_aeq4_ctl_info.enableAeq4  = TRUE;
                        MSG_GERAN_HIGH_1_G("AEQ4 SWITCHED ON : gl1_rxd_ctl_flag.RxdForcedOn=%d", gl1_rxd_ctl_flag.RxdForcedOn);
                        gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_ENABLED_AND_SACCH_CNT,gas_id);
                        gl1_clkdata_speed(TRUE, L1_DED_SPEEDUP_AEQ_PH4, L1_DED_SPEEDUP_AEQ,L1_DED_AEQ_PH4, gas_id);
                      }
                  }
              /* -TURN AEQ4 ON at the first SACCH period with rxqual_sub worse than 4 */
             else if ((rxqual_sub >= AEQ4_ON_RXQUAL_THR)&&(!mdsp_aeq4_ctl_info.enableAeq4))
                  {
                 mdsp_aeq4_ctl_info.sacch_cnt   = 0;
                 mdsp_aeq4_ctl_info.enableAeq4  = TRUE;
                 MSG_GERAN_HIGH_1_G("AEQ4 SWITCHED ON! rxqual_sub=%d",rxqual_sub);
                 gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_ENABLED_AND_SACCH_CNT,gas_id);
                 gl1_clkdata_speed(TRUE, L1_DED_SPEEDUP_AEQ_PH4, L1_DED_SPEEDUP_AEQ,L1_DED_AEQ_PH4, gas_id);
                  }
              /* -Keeping AEQ4 ON for all the SACCH periods with rxqual_sub worse than 4 and also resetting the sacch_cnt to be 0 at all times if we are under this threshold */
             else if ((rxqual_sub >= AEQ4_ON_RXQUAL_THR)&&(mdsp_aeq4_ctl_info.enableAeq4))
                   {
                 mdsp_aeq4_ctl_info.sacch_cnt = 0;
                 gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_SACCH_CNT,gas_id);
                   }

                /* -TURN AEQ4 OFF Only after three consecutive SACCH periods with rxqual_sub better than 2 */
             else if((rxqual_sub < AEQ4_OFF_RXQUAL_THR)&&(mdsp_aeq4_ctl_info.enableAeq4))
                   {
                    if(mdsp_aeq4_ctl_info.sacch_cnt < 3)
                      {
                       mdsp_aeq4_ctl_info.sacch_cnt++;
                   gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_SACCH_CNT,gas_id);
                      }
                    else
                      {
                       mdsp_aeq4_ctl_info.sacch_cnt = 0xFF;
                       mdsp_aeq4_ctl_info.enableAeq4 = FALSE;
                       gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_ENABLED_AND_SACCH_CNT,gas_id);
                       gl1_clkdata_speed(FALSE, L1_DED_SPEEDUP_AEQ_PH4, L1_DED_SPEEDUP_AEQ,L1_DED_AEQ_PH4, gas_id);
                   MSG_GERAN_HIGH_1_G("AEQ4 SWITCHED OFF! rxqual_sub=%d",rxqual_sub);
                      }
                   }
            }
            else if (!mdsp_aeq4_ctl_info.control_nv)
                {
                 MSG_GERAN_HIGH_0_G("AEQ4 forced OFF from NV item ");
                 mdsp_aeq4_ctl_info.sacch_cnt = 0xFF;
                 mdsp_aeq4_ctl_info.enableAeq4  = FALSE;
                 gl1_set_aeq4_info(mdsp_aeq4_ctl_info,AEQ4_ENABLED_AND_SACCH_CNT,gas_id);
                }
          }
#endif  /*FEATURE_AEQ_PHASE4*/



                mean_bep = gl1_msg_get_mean_bep(gas_id );
                cv_bep = gl1_msg_get_cv_bep (gas_id );

                /* Our SNR estimator has some variation and as a result even
                 * in perfect conditions the CV calculation can make it look
                 * low but we know conditions are excellent
                 */
                if(mean_bep == 31)
                {
                  cv_bep = 7;
                }

#ifdef FEATURE_GSM_RX_DIVERSITY
                  gl1_hw_set_mean_bep(mean_bep, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */

                gl1_msg_bep_init( gas_id );

                /* Reset for next reporting period */
                gl1_msg_rxqual_init( gas_id );

                /* Restart serving cell measurements */
                l1_set_serv_pwr_meas_permitted(TRUE, gas_id);

                /* Send the message */
                L1_send_MPH_SERVING_DED_MEAS_IND(rxlev_full,
                                                 rxlev_sub,
                                                 rxlev_val,
                                                 mean_bep,
                                                 cv_bep,
                                             l1_serving_cell_meas_ptr->num_received_blocks,
                                                 rxqual_full,
                                                 rxqual_sub,
                                                 gl1_was_dtx_used,
                                                 prxlev_sub,
                                                 gas_id );

                l1_serving_cell_meas_ptr->num_received_blocks = 0;

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
                /* Run DTX detection algorithm */
                gl1_msg_dtx_detection_algorithm(rxqual_full, rxqual_sub, rxlev_sub, gas_id);

                /* Only run AMR partial decode algorithm in AMR FR */
#ifdef FEATURE_GSM_AMR
                if ( l1_dedicated_data_ptr->channel_specification.channel_info_1_after.channel_type == FULL_RATE_TRAFFIC &&
                    (   l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3
#ifdef FEATURE_VAMOS_II
                     || l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3_VAMOS2
#endif
#ifdef FEATURE_GSM_AMR_WB
                     || l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V5
#ifdef FEATURE_VAMOS_II
                     || l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V5_VAMOS2
#endif
#endif
                    )
                   )
                {
                  gl1_msg_amr_pd_algorithm(rxqual_sub, rxqual_full, rxlev_sub, gas_id);
                }
#endif /* FEATURE_GSM_AMR */
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

                /*
                Set the indication to WCDMA measurements that this is now
                the start of a new SACCH reporting period
                */
                l1_sc_globals_ptr->wcdma_start_of_sacch_indication = TRUE;

#ifdef FEATURE_GSM_RX_DIVERSITY
                  gl1_hw_set_rxqual_full(rxqual_full, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */
                break;
            }

            /*********************************************************
             *                                                        *
             *                ISR_HANDOVER_CNF                        *
             *                                                        *
             **********************************************************/

        case ISR_HANDOVER_CNF:
            {
                isrtim_handover_cnf_T* message_ptr = ( isrtim_handover_cnf_T* )msg_header;
                gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
                dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

                /*calculate observed timing difference in bits*/
                if (l1_dedicated_data_ptr->target_cell_details.bsic_known)
                {
                    /* Note: OTD and t0 values should be from the old
                     * cell's perspective, not the one we've handed off to.
                     * Report OTD in bit periods.  RR converts to half bit
                     * period. RR also adds the TA value so only need to
                     * send OTD
                     */

                    l1_dedicated_data_ptr->observed_timing_difference = l1_ded_if_data[gas_id].otd/4;

                }
                else
                {
                    l1_dedicated_data_ptr->observed_timing_difference = 0;
                }

                L1_send_MPH_HANDOVER_CNF( l1_dedicated_data_ptr->
                                          observed_timing_difference,
                                          l1_dedicated_data_ptr->
                                          last_cell_timing_advance,
                                          gas_id);
                break;
            }
        default:
            MSG_GERAN_ERROR_1("L1->L1(DED) message %d ignored.", msg_header->message_id);
            break;
        }
    }
    /*******************************************************
    *                                                      *
    *                   GSM CB-L1 messages                 *
    *                                                      *
    ********************************************************/
    else if ( msg_header->message_set == MS_CB_L1)
    {
        if (msg_header->message_id == CB_SCHEDULING_REQ)   /*lint !e641 */
        {
            cb_scheduling_req_type *message_ptr;
            message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

            l1_smscb_process_scheduling_info
            ( message_ptr->channel_ind,
              message_ptr->scheduling_mode,
              message_ptr->scheduling_descriptor.scheduling_period_fn_list,
              message_ptr->scheduling_descriptor.scheduling_period_length,
              geran_map_nas_id_to_gas_id(message_ptr->as_id)
            );
        }
        else if (msg_header->message_id == CB_SKIP_REQ)   /*lint !e641 */
        {
            cb_skip_req_type *message_ptr;
            message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

            l1_smscb_process_skip_info
            ( message_ptr->channel_ind,
              message_ptr->scheduling_descriptor.scheduling_period_fn_list,
              message_ptr->scheduling_descriptor.scheduling_period_length,
              message_ptr->skip_ind,
              geran_map_nas_id_to_gas_id(message_ptr->as_id)
            );
        }
        else if (msg_header->message_id == CB_DEACTIVATE_REQ )  /*lint !e641 */
        {
            cb_deactivate_req_type *message_ptr;
            message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

            l1_smscb_process_deactivate_info( message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id) );
        }
    }

#ifdef FEATURE_GSM_DTM
    /*******************************************************
    *                                                      *
    *                   DTM MAC-L1 messages                *
    *                                                      *
    ********************************************************/
    else if ( msg_header->message_set == MS_MAC_L1)
    {

      /******************************************************
      * MAC_L1_FREQ_CONFIG                                  *
      * ------------------                                  *
      * Provides the frequency parameters for the new TBF   *
      * allocation (for a DTM assignment)                   *
      *******************************************************/
      if ( msg_header->message_id == (uint16) MAC_L1_FREQ_CONFIG )
      {
        mac_l1_freq_config_t  *message_ptr;
        boolean error = FALSE;
        gas_id_t gas_id;


        message_ptr = ( mac_l1_freq_config_t *) msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);


        MSG_GERAN_MED_1_G("MAC_L1_FREQ_CONFIG Rcvd - cs re-alloc:%d",
                                              message_ptr->dtm_freq_param_present );

        if ( message_ptr->dtm_freq_param_present )
        {
          /* check for un supported frequencies in ma list that
           * are not in the supported bands
           */
          error = gpl1_gprs_check_frequency_params
                            (
                              &(message_ptr->frequency_information.frequency_list),
                              message_ptr->frequency_information.hopping_flag, gas_id
                            );

          /*
           * Copy in the frequency info into the frequency_info_1 structure, there is no
           * TBF at present as L1 is in the dedicated state.
           */
          if( !error )
          {
            l1_transfer_data[gas_id].frequency_info_1 = message_ptr->frequency_information;
          }

        }

        /* send signal to mac to confirm or fail frequency params */
        L1_send_l1_mac_freq_ass_conf(error,(tbf_freq_cnf_t)message_ptr->tbf_freq_cnf, gas_id);

        /* Mark MAC_L1_FREQ_CONFIG as received */
        SET_FREQ_CONFIG_RECV(gas_id);

      }

      /******************************************************
      * MAC_L1_DUMMY_CTRL_BLOCK                             *
      * -----------------------                             *
      * Places the dummy control block within the message   *
      * into the transfer data dummy control buffer.        *
      *******************************************************/
      else if ( msg_header->message_id == (uint16) MAC_L1_DUMMY_CTRL_BLOCK )
      {

        mac_l1_dummy_ctrl_block_t  *message_ptr;
        gas_id_t gas_id;

        message_ptr = ( mac_l1_dummy_ctrl_block_t *) msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);


        MSG_GERAN_MED_0_G("MAC_L1_DUMMY_CONTROL ");

        memscpy(
              l1_transfer_data[gas_id].dummy_control_block,
              sizeof(l1_transfer_data[gas_id].dummy_control_block),
              message_ptr->dummy_ctrl_block,
              sizeof(message_ptr->dummy_ctrl_block)
              );

        /* Mark mac_l1_dummy_ctrl_block received */
        SET_DUMMY_CTRL_RECV(gas_id);

      }
      /******************************************************
      * MAC_L1_DTM_CHANNEL_ASSIGN_REQ                       *
      * -----------------------------                       *
      * Provides L1 with the DTM assignment alloactions.    *
      * It may contain and UL and or DL alloaction.         *
      *******************************************************/
      else if (msg_header->message_id == (uint16) MAC_L1_DTM_CHANNEL_ASSIGN_REQ)
      {
        mac_l1_dtm_channel_assign_req_t *message_ptr;
        boolean move_to_dtm = FALSE;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        dedicated_data_T *l1_dedicated_data_ptr;

        message_ptr = ( mac_l1_dtm_channel_assign_req_t *) msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

        MSG_GERAN_HIGH_3_G("Dedicated MAC_L1_DTM_CHANNEL_ASSIGN_REQ CS present:%d",
                                                          message_ptr->cs_present,
                                                          0,
                                                          0);
        dtm_assign_cs_present[gas_id] =   message_ptr->cs_present;
        gpl1_set_dtm_assig_status(TRUE, gas_id);
        /* Read the broadcast bep period from the gprs options structure and
         * use it to configure the egprs quality measurements module.
         * This is really vital in W2G tch handover cases where the MS starts up in
         * W and the EGPRS downlink assignment does not contain a valid bep period2.
         */
        gpl1_egprs_scell_qual_meas_broadcast_config( message_ptr->gprs_l1_cell_opt.bep_period, gas_id );

        /**************************************************************************
        * If a CS description is present then store this ready for use by the ISR *
        * Some parameters were received in the previous MAC_L1_FEQ_CONFIG message.*
        *                                                                         *
        * Perform the check to make sure that if a CS is present that the         *
        * MAC_L1_FREQ_CONFIG has already been received.                           *
        ***************************************************************************/
        if ( message_ptr->cs_present && !FREQ_CONFIG_RECV(gas_id) )
        {
          MSG_GERAN_ERROR_0_G("MAC_L1_DTM_CHANNEL_ASSIGN_REQ [CS desc]. before MAC_L1_FREQ_CONFIG"
                                                                                );
          return;
        }

        /**************************************************************************
         * Make sure that there is no ongoing processing of other messages.
         * Interrupting a process may cause data corruption. If one is already
         * running e.g. handover do not interrupt/overwrite it
         **************************************************************************/
        else if ( l1_ded_if_data[gas_id].current_rr_l1_msg_id != NULL )
        {
          MSG_GERAN_ERROR_2_G("RR_L1 req 0x%x in progress: Abandon New req 0x%x",
                              l1_ded_if_data[gas_id].current_rr_l1_msg_id,
                              msg_header->message_id );
          return;
        }
        else if ( message_ptr->cs_present && FREQ_CONFIG_RECV(gas_id) )
        {

          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.frequency_list
                                  = l1_transfer_data[gas_id].frequency_info_1.frequency_list;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.hopping_flag
                                  = l1_transfer_data[gas_id].frequency_info_1.hopping_flag;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.HSN
                                  = l1_transfer_data[gas_id].frequency_info_1.hsn;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.MAIO
                                  = l1_transfer_data[gas_id].frequency_info_1.maio;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.TSC
                                  = l1_transfer_data[gas_id].frequency_info_1.tsc;

          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.subchannel
                                  = message_ptr->cs_desc.cs_chan_info.subchannel;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.TN
                                  = message_ptr->cs_desc.cs_chan_info.TN;
          l1_dedicated_data_ptr->new_channel_specification.channel_info_1_after.channel_type
                                  = message_ptr->cs_desc.cs_chan_info.channel_type;
          l1_dedicated_data_ptr->new_channel_specification.before_parameters_valid = FALSE;
          l1_dedicated_data_ptr->new_channel_specification.starting_time_valid = FALSE;
          l1_dedicated_data_ptr->new_channel_specification.channel_mode_1
                                  = message_ptr->cs_desc.channel_mode;
          l1_dedicated_data_ptr->new_channel_specification.DTX_indicator
                                  = message_ptr->cs_desc.DTX_indicator;
          l1_dedicated_data_ptr->new_channel_specification.power_level
                                  = message_ptr->cs_desc.power_level;

          l1_dedicated_data_ptr->new_multirate_configuration
                                  = message_ptr->cs_desc.multi_rate_config;

          if(message_ptr->cs_desc.cipher_flag == FALSE)
          {
            l1_dedicated_data_ptr->new_channel_specification.cipher_flag = l1_dedicated_data_ptr->channel_specification.cipher_flag;
            l1_dedicated_data_ptr->new_channel_specification.cipher_algorithm = l1_dedicated_data_ptr->channel_specification.cipher_algorithm;
          }else
          {
            l1_dedicated_data_ptr->new_channel_specification.cipher_flag = message_ptr->cs_desc.cipher_flag;
            l1_dedicated_data_ptr->new_channel_specification.cipher_algorithm = message_ptr->cs_desc.cipher_algorithm;
          }

          /* Currently not present in message */
          l1_dedicated_data_ptr->new_channel_specification.number_of_ded_channels = 0;


          /* This is required so that the need_to_sync flag is set false. This will
           * then prevent sync to current cell activity (Note current cell activty activity is
           * mechanism specific for HO). The movement back to initial cell will be controlled
           * by dtm change timeslot mechanism
           */
          l1_dedicated_data_ptr->current_cell_details.arfcn = l1_dedicated_data_ptr->target_cell_details.arfcn;

        }else if ( (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST) && FREQ_CONFIG_RECV(gas_id))
        {
          /* This is our first entry into DTM as we are currently slammed to our lowest tn
           * we need to re-start the CS channels and re-init the vocoder as we will now
           * slam to place the CS on TN 2. Need to copy current CS channel data to new
           * channel structure to allow re-starting of CS
           */
           /* Now copy current data into the new dedicated data */
           memscpy ( &l1_dedicated_data_ptr->new_channel_specification,
                    sizeof(l1_dedicated_data_ptr->new_channel_specification),
                    &l1_dedicated_data_ptr->channel_specification,
                    sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
          /* Now copy in the current multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                   &l1_dedicated_data_ptr->multirate_configuration,
                   sizeof(multirate_config_T) );
#endif
          /* Setup cs present flag to force config of CS and PS */
          message_ptr->cs_present = TRUE;
          if(message_ptr->cs_desc.cipher_flag == FALSE)
          {
            l1_dedicated_data_ptr->new_channel_specification.cipher_flag = l1_dedicated_data_ptr->channel_specification.cipher_flag;
            l1_dedicated_data_ptr->new_channel_specification.cipher_algorithm = l1_dedicated_data_ptr->channel_specification.cipher_algorithm;
          }else
          {
            l1_dedicated_data_ptr->new_channel_specification.cipher_flag = message_ptr->cs_desc.cipher_flag;
            l1_dedicated_data_ptr->new_channel_specification.cipher_algorithm = message_ptr->cs_desc.cipher_algorithm;
          }

          /* This is required so that the need_to_sync flag is set false. This will
           * then prevent sync to current cell activity (Note current cell activty activity is
           * mechanism specific for HO). The movement back to initial cell will be controlled
           * by dtm change timeslot mechanism
           */
          l1_dedicated_data_ptr->current_cell_details.arfcn = l1_dedicated_data_ptr->target_cell_details.arfcn;

          MSG_GERAN_ERROR_1_G(" First move to DTM reslam to TN 2 [%d]",gl1_get_FN( gas_id ));
        }

        /* Store the TBF parameters depending upon the type of TBF */
        switch ( message_ptr->dtm_tbf_type )
        {
        case NO_TBF_PRESENT:
          MSG_GERAN_ERROR_1_G("No TBF type in DTM Assignment",message_ptr->dtm_tbf_type);
          break;

        case UL_TBF_PRESENT:
          {
            gpl1_dtm_save_ul_alloc( &message_ptr->ul_tbf, gas_id );

            if ( FREQ_CONFIG_RECV(gas_id) && DUMMY_CTRL_RECV(gas_id) )
            {
              move_to_dtm = TRUE;
              RESET_BITMAP(gas_id);
            }
            else
            if ( !DUMMY_CTRL_RECV(gas_id) )
            {
              MSG_GERAN_ERROR_0_G("No MAC_L1_DUMMY_CTRL_BLOCK recieved for UL");
            }
            break;
          }

        case DL_TBF_PRESENT:
          {
            gpl1_dtm_save_dl_alloc( &message_ptr->dl_tbf , gas_id);

            if( ( FREQ_CONFIG_RECV(gas_id) ) || ( l1_tskisr_blk->l1_state == L1_DTM_MODE ) )
            {
              move_to_dtm = TRUE;
              RESET_BITMAP(gas_id);
            }
            break;
          }
        case UL_DL_TBF_PRESENT:
          {
            gpl1_dtm_save_uldl_alloc( message_ptr );

            if ( FREQ_CONFIG_RECV(gas_id) )
            {
              move_to_dtm = TRUE;
              RESET_BITMAP(gas_id);
            }
            break;
          }
        default:
          MSG_GERAN_ERROR_3_G("Unsupported dtm_tbf_t:%d in DTM Assignment",
                                                        message_ptr->dtm_tbf_type,
                                                        0,
                                                        0);
        }

        GL1_ISR_SAVE_LOCK(gas_id);
        /* The TBF has been setup ready for use, it is now time to move to the
        DTM state providing this message handling is not already from the
        DTM state - see gpl1_dtm_if.c*/
        if ( move_to_dtm && ( l1_tskisr_blk->l1_state != L1_DTM_MODE ) )
        {
          l1_ded_if_data[gas_id].current_rr_l1_msg_id = msg_header->message_id;

          l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
          l1_tskisr_blk->next_params.L1Data.pDtm_data = &gpl1_dtm_data[gas_id];



          l1_tskisr_blk->command = L1_NULL_COMMAND;

          if ( message_ptr->cs_present )
          {
            l1_tskisr_blk->main_command = L1_DEDICATED_STOP;
            l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode
                                                                    = L1_DTM_CS_PS;
          }
          else
          {
            l1_tskisr_blk->main_command = L1_DEDICATED_TO_DTM_COMMAND;
            l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode
                                                                    = L1_DTM_PS_ONLY;
          }
        }
        else
        {
          /* Store the message and begin the DTM transition */
          gpl1_dtm_set_current_rr_mac_l1_msg_id( message_ptr->message_header,gas_id  );

          l1_set_dtm_command_event(L1_DTM_STOP,gas_id);

        }
        GL1_ISR_SAVE_UNLOCK(gas_id);

        /* The SCE needs to be stopped to make this transition to the DTM */
        l1_sc_stop(gas_id);
      }

      /******************************************************
      * MAC_L1_RELEASE_REQ                                  *
      * -----------------------------                       *
      *  Handle Any out of sync DTM issues where MAC still  *
      *  Belives GL1 has TBF assigned.                      *
      *                                                     *
      * Confirm back any thing Mac wants GL1 to release.    *
      *******************************************************/
      else if (msg_header->message_id == (uint16) MAC_L1_RELEASE)
      {
        release_T local_release_cause = L1_NO_RELEASE;
        mac_l1_release_t  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        l1_dtm_mode_T dtm_init_mode;

        message_ptr = (mac_l1_release_t *)msg_header;
        gas_id = message_ptr->gas_id;
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        switch(message_ptr->release_tbf)
        {
          case UL_TBF_NORMAL:
          case UL_TBF_ABNORMAL:
          {
            local_release_cause = L1_FULL_RELEASE_UL;
            break;
          }

          case DL_TBF_NORMAL:
          case DL_TBF_ABNORMAL:
          {
            local_release_cause = L1_FULL_RELEASE_DL;
            break;
          }

          case UL_AND_DL_TBF_NORMAL:
          case UL_AND_DL_TBF_ABNORMAL:
          {
            local_release_cause = L1_FULL_RELEASE_UL_DL;
            break;
          }

          default:
            MSG_GERAN_ERROR_1_G("MAC_L1_RELEASE - unknown release type %d",message_ptr->release_tbf );
        }
        GL1_ISR_SAVE_LOCK(gas_id);

        dtm_init_mode = l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode;

        MSG_GERAN_HIGH_1_G(" MAC_L1_RELEASE Rcvd %d",message_ptr->release_tbf);

         /*DTM Channel assignment received. But Since MAC release PS , we need to cancel connecting DTM assignment
          and Inform MAC and GRR. GRR will send DTM_ASS_FAILURE to NW*/
        if(local_release_cause == L1_FULL_RELEASE_UL || 
             local_release_cause == L1_FULL_RELEASE_DL ||
              local_release_cause == L1_FULL_RELEASE_UL_DL)
        {
           if(((l1_tskisr_blk->main_command == L1_DEDICATED_STOP) && (dtm_init_mode == L1_DTM_CS_PS))||
                ( (l1_tskisr_blk->main_command == L1_DEDICATED_TO_DTM_COMMAND) && (dtm_init_mode == L1_DTM_PS_ONLY)))
           {
                MSG_GERAN_HIGH_0_G(" Stay in dedicated ");
                l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
                l1_transfer_data[gas_id].pending_frequency_info_ptr = NULL;

                l1_transfer_data[gas_id].current_alloc_type = L1_NULL_ALLOC;
                l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;

                l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;
                l1_reset_rr_current_msg_id( gas_id );
                l1_sc_start(L1SCModeSameAsBefore,gas_id);

                L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (DTM_ASS_FAILED, TRUE, FALSE, gas_id);
           }
        } 
        gpl1_gprs_send_l1_mac_tbf_rel_confirm( local_release_cause, gas_id );
        GL1_ISR_SAVE_UNLOCK(gas_id);
      }
      else
      {
         MSG_GERAN_ERROR_1("Unknown MAC-L1 message id=%d",msg_header->message_id);
      }
    }
#endif

    else
    {
        MSG_GERAN_ERROR_0("L1(DED) - Unknown message set.");
    }

}


/*===========================================================================

FUNCTION  L1_get_prxdrx_pwrdiff

DESCRIPTION
  This function checks if Drx power avg is more than Prx power for current reporting period.
  To be used before REP_PERIOD_IND.

DEPENDENCIES
  None

RETURN VALUE
  If Drx power is more than Prx power, returns the difference
  Else returns 0

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_RX_DIVERSITY
uint8 L1_get_prxdrx_pwrdiff(gas_id_t gas_id)

{
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
    uint8  l1_ded_prx_drx_pwr_diff = 0;

    if( (l1_serving_cell_meas_ptr->no_of_measurements_sub != 0) && (gl1_get_rxd_ctl_nv(gas_id) == TRUE) 
         && (l1_serving_cell_meas_ptr->divrx_power_sub != 0) 
         && (l1_serving_cell_meas_ptr->is_divrx_disabled_in_sacch_period == FALSE) )
    {
       uint8  prxlev_sub, divrxlev_sub;
       int32  rx_power_sub, divrx_power_sub;

       rx_power_sub = l1_serving_cell_meas_ptr->rx_power_sub / l1_serving_cell_meas_ptr->no_of_measurements_sub;
       prxlev_sub  = L1_map_dBm_x16_to_rxlev((dBx16_T)rx_power_sub);
    
       divrx_power_sub = l1_serving_cell_meas_ptr->divrx_power_sub / l1_serving_cell_meas_ptr->no_of_measurements_sub;
       divrxlev_sub = L1_map_dBm_x16_to_rxlev((dBx16_T)divrx_power_sub);
                            
       if((divrxlev_sub - prxlev_sub) > gl1_get_Rxd_threshold(gas_id))
       {
           l1_ded_prx_drx_pwr_diff = divrxlev_sub - prxlev_sub;
       }
	   
       MSG_GERAN_MED_1_G("L1_get_prx_drx_diff_this_SACCH = %d", l1_ded_prx_drx_pwr_diff);
    }

    return l1_ded_prx_drx_pwr_diff;    
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/


/*===========================================================================
FUNCTION GL1_IS_HANDOVER_POSSIBLE

DESCRIPTION
  This function validates and computes parameters required for a inter-handover
  based on the type of handover.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - if the requested handover is possible.
  FALSE - if the requested handover is not possible.

SIDE EFFECTS
  None
===========================================================================*/
static boolean gl1_is_handover_possible ( mph_handover_req_T  *message_ptr )
{

  boolean            handover_is_possible = TRUE;
  word               real_qbit_lag,n;
  int16              current_cells_qbit_offset,qbit_lag,qbit_offset;
  dedicated_data_T  *l1_dedicated_data_ptr;
  cell_T*            matched_cell = NULL;
  cell_T*            cell=NULL;
  gas_id_t           gas_id;
  idle_data_T        *l1_idle_data_ptr; 
  l1_sc_globals_T    *l1_sc_globals_ptr;

  if (!message_ptr)
  {
      MSG_GERAN_ERROR_0("Rcvd NULL message_ptr");
      return FALSE;
  }

  gas_id = check_gas_id(message_ptr->gas_id);
  l1_idle_data_ptr = &l1_idle_data_store[gas_id]; 
  l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);
  l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  /* According to 45.010 A3.2, after each succesful HO,
   * MS must provide OTD in HO COMPLETE msg, even though
   * in Synchronisation Indication I.E. it is selectable.
   * So may as well work it out every time
   */

  /* Get current cells Qbit offset and subtract it from targets cells
   * Qbit offset to get OTD.
   */
  current_cells_qbit_offset = (8 - l1_get_current_timeslot(gas_id)) * 625;

  l1_ded_if_data[gas_id].otd = ( l1_dedicated_data_ptr->target_cell_details.qbit_lag - current_cells_qbit_offset );

  /* It is possible, theoretically, to obtain a otd value of -9999(if ncell is
     reported at -4999 and we are on TS0). Therfore need to account for this.
     We are going to use otd to calculate TA which is always positive and further
     down this function this value is cast to a word - therefore it needs to be +ve.
  */
  while (l1_ded_if_data[gas_id].otd < 0)
  {
      l1_ded_if_data[gas_id].otd = l1_ded_if_data[gas_id].otd + 5000;
  }

  /*
    Determine type of handover andtake action accordingly.
  */
  if ( message_ptr->handover_synchronisation == NON_SYNCHRONISED )
  {
    /* For non-synchronized handover, BSIC must be known and time
    ** synchronization is needed.
    */
    if ( message_ptr->timing_advance_valid )
    {
      l1_ded_if_data[gas_id].handover_ta = (int16) message_ptr->timing_advance;
    }
    else
    {
      l1_ded_if_data[gas_id].handover_ta = 0;
    }
  }
  else if ( message_ptr->handover_synchronisation == SYNCHRONISED )
  {
    /* See 05.10 section A.1.13 */
    if ( message_ptr->timing_advance_valid )
    {
      l1_ded_if_data[gas_id].handover_ta = (int16) message_ptr->timing_advance;
    }
    else
    { /* TA not known */
      /* we have to calculate it */
     qbit_lag = 0 ;
	 
     for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count ; ++n )
     {
       cell = l1_sc_globals_ptr->sorted_cells[ n ]; 

       if((l1_idle_data_ptr->campedon_cell_ARFCN.num == cell->bs.arfcn.num) && (l1_idle_data_ptr->campedon_cell_BSIC == cell->bs.bsic))
       {
         matched_cell = cell;
         break;		  
       }
     }

      real_qbit_lag = (word) l1_ded_if_data[gas_id].otd;

      if(matched_cell!=NULL)
      {
        qbit_lag = matched_cell->bs.qbit_lag ;
	   
        if((qbit_lag % QS_PER_SLOT) != 0)
        {
          qbit_offset = ((0-l1_get_current_timeslot(gas_id)) * QS_PER_SLOT);

          if(qbit_offset < 0)
          {
            qbit_offset += 5000;
          }

          real_qbit_lag -= (qbit_lag - qbit_offset) ;
        }
         MSG_GERAN_HIGH_2_G("Adjustment: adjtd_lag =%d ori_lag=%d",real_qbit_lag,((word) l1_ded_if_data[gas_id].otd) );
      } 

      if ( real_qbit_lag > 4999 )
      {
        real_qbit_lag -= 5000;
        l1_ded_if_data[gas_id].handover_ta = (int16) ((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);
      }
      /* This computation is a re-arrangement of the formula given in 45.010
       * of OTD = RTD +t1 - t0.
       */
      else if ( real_qbit_lag > 1250 )
      {
        l1_ded_if_data[gas_id].handover_ta = (int16) ((l1_dedicated_data_ptr->timing_advance_in_use
                    - (5000 - real_qbit_lag)/2)%256);
      }
      else
      {
        l1_ded_if_data[gas_id].handover_ta = (int16) ((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);
      }

      /* handover_ta is signed, so check for negative is needed */
      if ( l1_ded_if_data[gas_id].handover_ta > 229  || l1_ded_if_data[gas_id].handover_ta < 0  )
      {
        /* This is a negative timing advance - GSM 05.10 section A.3.3 */
        l1_ded_if_data[gas_id].handover_ta = 0;
      }

      else if ( l1_ded_if_data[gas_id].handover_ta >  63 )
      {
        /* Timing advance is too big - for NCI see GSM 04.08 table 10.55*/
        if ( message_ptr->NCI == 0 )
        {
          /* Ignore calculated value - use default */
          l1_ded_if_data[gas_id].handover_ta = 63;
        }
        else
        {
          /* Abort handover */
          handover_is_possible = FALSE;
        }
      }
    }
  }

  else if ( message_ptr->handover_synchronisation == PRE_SYNCHRONISED )
  {

    /* If the timing advance is not valid, use the default of 1 as specified
    ** by the standards.
    */
    if ( message_ptr->timing_advance_valid )
    {
      l1_ded_if_data[gas_id].handover_ta = (int16) message_ptr->timing_advance;
    }
    else
    {
      l1_ded_if_data[gas_id].handover_ta = 1;
    }

    /* correct qbit lag for timeslot number */
    real_qbit_lag = (word)(l1_dedicated_data_ptr->target_cell_details.qbit_lag + l1_get_current_timeslot(gas_id)*625);

    if ( l1_dedicated_data_ptr->target_cell_details.qbit_lag < 0 )
    {
      real_qbit_lag += 5000;
    }

    if ( real_qbit_lag > 4999 )
    {
      real_qbit_lag -= 5000;
    }
  }
  else if ( message_ptr->handover_synchronisation == PSEUDO_SYNCHRONISED )
  {
    /*
        BTS provides the real time difference in the case of pseudo synchronized
        handovers
    */
    if ( message_ptr->real_time_difference_valid )
    {
      l1_ded_if_data[gas_id].handover_rtd = message_ptr->real_time_difference;
    }
    else
    {
      l1_ded_if_data[gas_id].handover_rtd = 0;
    }

    /* If the timing advance is not valid, use the default of 1 as specified
    ** by the standards.
    */
    if ( message_ptr->timing_advance_valid )
    {
      l1_ded_if_data[gas_id].handover_ta = (int16) message_ptr->timing_advance;
    }
    else
    /* TA unknown - need to calculate */
    {
      /* correct for timeslot number */

     for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count ; ++n )
     {
       cell = l1_sc_globals_ptr->sorted_cells[ n ]; 
       if((l1_idle_data_ptr->campedon_cell_ARFCN.num == cell->bs.arfcn.num) && (l1_idle_data_ptr->campedon_cell_BSIC == cell->bs.bsic))
       {
          matched_cell = cell;
          break;		  
       }
     }

	  /* correct qbit lag for timeslot number */
      real_qbit_lag = (word)(l1_dedicated_data_ptr->target_cell_details.qbit_lag + l1_get_current_timeslot(gas_id)*625);

      if ( l1_dedicated_data_ptr->target_cell_details.qbit_lag < 0 )
      {
          real_qbit_lag += 5000;
      }

	 
      if(matched_cell!=NULL)
      {
         qbit_lag = matched_cell->bs.qbit_lag ;

         if((qbit_lag % QS_PER_SLOT) != 0)
         {
           qbit_offset = ((0-l1_get_current_timeslot(gas_id)) * QS_PER_SLOT);
           if(qbit_offset < 0)
           {
             qbit_offset += 5000;
           }
           real_qbit_lag -= (qbit_lag - qbit_offset) ;
           l1_ded_if_data[gas_id].otd  -= (qbit_lag - qbit_offset) ;
         }
         MSG_GERAN_HIGH_2_G("Adjustment: adjtd_lag =%d ori_lag=%d",real_qbit_lag,((word) l1_ded_if_data[gas_id].otd) );
      } 

      if ( real_qbit_lag > 4999 )
      {
          real_qbit_lag -= 5000;
          l1_ded_if_data[gas_id].handover_ta = (int16)((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);
      }
      /* What is this computation ?? */
      else if ( real_qbit_lag > 2500 )
      {
          l1_ded_if_data[gas_id].handover_ta = (int16)((l1_dedicated_data_ptr->timing_advance_in_use
                      - (5000 - real_qbit_lag)/2)%256);
      }
      else
      {
          l1_ded_if_data[gas_id].handover_ta = (int16)((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);
      }

      /* Use the RTD provided by the BTS for pseudo-psynchronized handovers */
      l1_ded_if_data[gas_id].handover_ta -= (int16)l1_ded_if_data[gas_id].handover_rtd;

      /* handover_ta is signed, so check for negative is needed */
      if ( l1_ded_if_data[gas_id].handover_ta > 229 || l1_ded_if_data[gas_id].handover_ta < 0 )
      {
          /* This is a negative timing advance - GSM 05.10 section A.3.3 */
          l1_ded_if_data[gas_id].handover_ta = 0;
      }

      else if ( l1_ded_if_data[gas_id].handover_ta >  63 )
      {
        /* Timing advance is too big - for NCI see GSM 04.08 table 10.55*/
        if ( message_ptr->NCI == 0 )
        {
            /* Ignore calculated value - use default */
            l1_ded_if_data[gas_id].handover_ta = 63;
        }
        else
        {
            /* Abort handover */
            handover_is_possible = FALSE;
        }
      }
    }
  }

  return( handover_is_possible );

} /* end of gl1_is_handover_possible() */

/*===========================================================================

FUNCTION GL1_SAVE_CURRENT_CELL_DATA

DESCRIPTION
  This function saves the last cell's parameters in case we need to reconnect

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_GSM_DTM
void gl1_save_current_cell_data( boolean store_afc_info,gas_id_t gas_id )
#else
static void gl1_save_current_cell_data( boolean store_afc_info,gas_id_t gas_id )
#endif
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  l1_serving_cell_meas_T    *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];


    if ( store_afc_info )
    {
      /* Store the AFC setup for this scell */
      gl1_msg_get_afc_params(
        &l1_dedicated_data_ptr->previous_cell_rotator_value,
        &l1_dedicated_data_ptr->previous_cell_freq_value,gas_id);

      l1_dedicated_data_ptr->previous_cell_afc_valid = TRUE;

      MSG_GERAN_MED_3_G("Storing away the AFC params Freq=%d ROT=%d",
        l1_dedicated_data_ptr->previous_cell_freq_value,
        l1_dedicated_data_ptr->previous_cell_rotator_value,
        0);
    }
    else
    {
      l1_dedicated_data_ptr->previous_cell_afc_valid = FALSE;
    }

    /* Save the old cell data */
    l1_dedicated_data_ptr->last_cell_timing_advance =
    l1_dedicated_data_ptr->timing_advance_in_use;

    l1_dedicated_data_ptr->last_cell_tx_power =
    l1_dedicated_data_ptr->ms_power_level_in_use;

    if ( l1_ded_if_data[gas_id].prev_rxlev_sub != 0 )
    {
       /* Save previous reporting period's rxlev_sub */
       l1_dedicated_data_ptr->last_cell_rx_power = L1_map_rxlev_to_dBm_x16(l1_ded_if_data[gas_id].prev_rxlev_sub);
    }
    else
    {
       l1_dedicated_data_ptr->last_cell_rx_power = SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16;
    }

    MSG_GERAN_MED_3_G("Current cell data ta:%d tx_power:%d rx_power:%d",
            l1_dedicated_data_ptr->last_cell_timing_advance, l1_dedicated_data_ptr->last_cell_tx_power, l1_dedicated_data_ptr->last_cell_rx_power );

    l1_dedicated_data_ptr->current_cell_details.arfcn =
    l1_idle_data->campedon_cell_ARFCN;

    l1_dedicated_data_ptr->current_cell_details.bsic =
    l1_idle_data->campedon_cell_BSIC;


    /* Save off current cell info, so that we don't have to reacquire
     * if its in the new cell's BA list
     */
    l1_dedicated_data_ptr->previous_cell_details.arfcn =
    l1_dedicated_data_ptr->current_cell_details.arfcn;

    l1_dedicated_data_ptr->previous_cell_details.bsic =
    l1_dedicated_data_ptr->current_cell_details.bsic;

    if ( l1_serving_cell_meas_ptr->no_of_measurements_sub )
    {
      l1_dedicated_data_ptr->previous_cell_details.rx_power =
      (dBx16_T)(l1_serving_cell_meas_ptr->rx_power_sub/l1_serving_cell_meas_ptr->no_of_measurements_sub);

      MSG_GERAN_HIGH_1_G("PCD Uses current val:%d",l1_dedicated_data_ptr->previous_cell_details.rx_power);
    }
    else
    {
      MSG_GERAN_HIGH_1_G("PCD Uses PREV RXLEV SUB:%d",l1_dedicated_data_ptr->last_cell_rx_power);

      /*Initialise using the last measurement on the SACCH in the previous period*/
      l1_dedicated_data_ptr->previous_cell_details.rx_power = l1_dedicated_data_ptr->last_cell_rx_power;
      /* Initialize the cell's AGC structure */
      sys_algo_agc_init(&l1_dedicated_data_ptr->previous_cell_details.agc, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
    }

    l1_dedicated_data_ptr->previous_cell_details.bsic_known = TRUE;

    l1_dedicated_data_ptr->previous_cell_details.fn_lag = 0;

    /* This is because we're currently slammed to current_timeslot and
     * our own SCH is on timeslot 0
     */
    l1_dedicated_data_ptr->previous_cell_details.qbit_lag = (int16)(( 0 - l1_get_current_timeslot(gas_id) ) * 625);

    /* If qbit_lag is 0, don't adjust qbit_lag, otherwise subtract 1
     * because ncell qbit_lag would be -ve relative to our timeslot
     */
    if ( l1_dedicated_data_ptr->previous_cell_details.qbit_lag  )
    {
        l1_dedicated_data_ptr->previous_cell_details.fn_lag =
        SUB_FN( l1_dedicated_data_ptr->previous_cell_details.fn_lag, 1);
    }

    MSG_GERAN_MED_3_G("Current cell data qbit_lag:%d fn_lag:%d ts:%d",
            l1_dedicated_data_ptr->previous_cell_details.qbit_lag, l1_dedicated_data_ptr->previous_cell_details.fn_lag, l1_get_current_timeslot(gas_id) );

    /* Reset ho_tt_adjust so can keep track of TT on new cell in case of HO failure. */
    gl1_msg_reset_ho_tt_adjust(gas_id);

#ifdef FEATURE_GSM_AMR
    /* If we are handing over from an AMR cell */
#ifdef FEATURE_GSM_AMR_WB
    if ( (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3)
    || (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V5) )
#else
    if (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3)
#endif

    {
        uint8 i;

        for (i = 0; i < MAX_CODEC_MODES; i++)
        {
            l1_dedicated_data_ptr->amr_details.dl_acs[i] = curr_dl_acs[gas_id][i];
            l1_dedicated_data_ptr->amr_details.ul_acs[i] = curr_ul_acs[gas_id][i];

            if ( i < (MAX_CODEC_MODES - 1) )
            {
                l1_dedicated_data_ptr->amr_details.thresh_tbl[i] =
                curr_thresh_tbl[gas_id][i];
                l1_dedicated_data_ptr->amr_details.hyst_tbl[i] =
                curr_hyst_tbl[gas_id][i];
            }
        }

        l1_dedicated_data_ptr->amr_details.dl_num_acm = curr_dl_num_acm[gas_id];
        l1_dedicated_data_ptr->amr_details.ul_num_acm = curr_ul_num_acm[gas_id];
        l1_dedicated_data_ptr->amr_details.dl_icm = curr_dl_icm[gas_id];
        l1_dedicated_data_ptr->amr_details.ul_icm = curr_ul_icm[gas_id];
        l1_dedicated_data_ptr->amr_details.dl_phase = curr_phase[gas_id];
    }
#endif

    /************************************************************************
    Store away the channel allocation being used. This is so that upon a
    return to the old channels L1 will know whether to ignore the starting
    time or not, and therefore BEFORE parameters.
    NOTE: If there is a pending frequency redefinition the don't
    set this flag as RR will send the current allocation as the BEFORE
    parameters and the frequency redefinition as the AFTER parameters in the
    mph_reconnect_channel_req message. Therefore this flag should not be set
    to ignore these parameters.... As seen in 26.6.13.3
    **************************************************************************/
    if ( (l1_tskisr_blk->sub_state == L1_DED_AFTER) &&
         (l1_dedicated_data_ptr->freq_redef_imminent == FALSE)
       )
    {
      l1_dedicated_data_ptr->previous_cell_on_after_channels = TRUE;
    }
    else
    {
      l1_dedicated_data_ptr->previous_cell_on_after_channels = FALSE;
    }
}


/*===========================================================================

FUNCTION GL1_SETUP_NEW_CHANNEL

DESCRIPTION
  Invoked in the ISR context to setup and restart a new dedicated channel
  after the old one has been suspended

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_setup_new_channel( gas_id_t gas_id )
{
/* Determine which primitive was being processed in the task and process
 * accordingly
 */
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    switch ( l1_ded_if_data[gas_id].current_rr_l1_msg_id )
    {
    case MPH_CHANNEL_ASSIGN_REQ:
        gl1_complete_channel_assignment(gas_id);
        break;

    case MPH_HANDOVER_REQ:
        if ( l1_ded_if_data[gas_id].do_gsm_blind_handover )
        {
            if( FALSE == l1_ded_if_data[gas_id].l1_target_ncell_unavailable )
            {

              L1_setup_blind_ho_ACQ( l1_dedicated_data_ptr->ho_details.BCCH_ARFCN,  /* list of one freq */
                                     &(l1_dedicated_data_ptr->target_cell_details.agc), gas_id );
            }
            else
            {

              L1_setup_blind_ho_ACQ( l1_dedicated_data_ptr->ho_details.BCCH_ARFCN,  /* list of one freq */
                                     NULL, gas_id );
            }

            L1_send_ISRTIM_ACQ_REQ( l1_dedicated_data_ptr->ho_details.BCCH_ARFCN, gas_id );

#ifdef DEBUG_GSM_BLIND_HANDOVER
            #error code not present
#endif

            l1_tskisr_blk->next_params.bsic_expected = l1_dedicated_data_ptr->ho_details.BSIC;

            l1_change_state( L1_FIND_BCCH, gas_id );
            l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
            L1_transfer_new_data( gas_id);
            l1_tskisr_blk->sub_state     = L1_ACQUIRING;
            l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE_GSM_BLIND_HO;
        }
        else
        {
            MSG_GERAN_HIGH_0_G("Transfer dedi data");
            l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
            L1_transfer_new_data( gas_id);
            /* Apply afc */
            gl1_complete_channel_handover( TRUE, gas_id );
        }

        break;

    case MPH_CHANNEL_MODE_MODIFY_REQ:
        gl1_complete_channel_mode_modify(gas_id);
        break;

    case MPH_RECONNECT_CHANNEL_REQ:
        gl1_complete_channel_reconnect(gas_id);
        break;

    case MPH_CHANNEL_RELEASE_REQ:
        gl1_complete_channel_release(gas_id);
        break;

#ifdef FEATURE_GSM_DTM
    case MAC_L1_DTM_CHANNEL_ASSIGN_REQ:
      {
        /********************************************************************/
        /* Complete the DTM_CHANNEL_ASSIGNMENT_REQ, copying over any new CS */
        /* allocation into the dedicated data structure.                    */
        /********************************************************************/
        if ( l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode == L1_DTM_CS_PS )
        {
          byte temp;

          /* Now fill in the new dedicated data */
          memscpy ( &l1_dedicated_data_ptr->channel_specification,
                   sizeof(l1_dedicated_data_ptr->channel_specification),
                   &l1_dedicated_data_ptr->new_channel_specification,
                   sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
          /* Now fill in the multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->multirate_configuration),
                   &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(multirate_config_T) );
#endif

          l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;

          /* Load up transmit power */
          temp = l1_dedicated_data_ptr->channel_specification.power_level;
          l1_dedicated_data_ptr->ms_power_level_in_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_report = temp;
          l1_dedicated_data_ptr->ordered_ms_power_level = temp;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_DTM_CHANNEL_ASSIGN_CNF;

          l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;

        }

        l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_DTM_CHANNEL_ASSIGN_CNF;
        l1_change_state( L1_DTM_MODE, gas_id );
        l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
        l1_tskisr_blk->sub_state     = L1_DTM_INIT;
        L1_transfer_new_data( gas_id);
        l1_set_dtm_command_event(L1_DTM_ACTIVATE,gas_id);

        if (l1_ded_if_data[gas_id].stored_dtx_indicator)
        {
          l1_vs_set_dtx_mode(DTX_ENABLED, gas_id);
        }
        MSG_GERAN_HIGH_1("DTM:Stored dtx indicator %d",
                          l1_ded_if_data[gas_id].stored_dtx_indicator);

        break;
      }
#endif
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
    case MPH_STOP_GSM_MODE_REQ:
    {
        gl1_reset_dtx_used_flag(gas_id);
        l1_vs_set_dtx_mode(DTX_DISABLED, gas_id);
        l1_ded_if_data[gas_id].stored_dtx_indicator = FALSE;
        l1_vs_stop_client(gas_id);

        break;
    }
#endif
    default:
        MSG_GERAN_ERROR_1_G("Trying to resume invalid channel msg id %d",l1_ded_if_data[gas_id].current_rr_l1_msg_id);
        break;
    }
    l1_ded_if_data[gas_id].current_rr_l1_msg_id = NULL;
}

/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_ASSIGNMENT

DESCRIPTION
  Invoked in the ISR context to copy over and setup the new channel parameters
  after the old one has been suspended

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_GSM_DTM
void gl1_complete_channel_assignment(gas_id_t gas_id)
#else
static void gl1_complete_channel_assignment(gas_id_t gas_id)
#endif
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    byte temp;

    /* Now fill in the new dedicated data */
    memscpy ( &l1_dedicated_data_ptr->channel_specification,
             sizeof(l1_dedicated_data_ptr->channel_specification),
             &l1_dedicated_data_ptr->new_channel_specification,
             sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
    /* Now fill in the multirate configuration data */
    memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
             sizeof(l1_dedicated_data_ptr->multirate_configuration),
             &l1_dedicated_data_ptr->new_multirate_configuration,
             sizeof(multirate_config_T) );
#endif

   /* Clear out any pending Frequency Redefinition */
   l1_dedicated_data_ptr->freq_redef_imminent = FALSE;

    l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;

    /* Load up transmit power */
    temp = l1_dedicated_data_ptr->channel_specification.power_level;
    l1_dedicated_data_ptr->ms_power_level_in_use = temp;
    l1_dedicated_data_ptr->ms_power_level_to_use = temp;
    l1_dedicated_data_ptr->ms_power_level_to_report = temp;
    l1_dedicated_data_ptr->ordered_ms_power_level = temp;

    l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_CHANNEL_ASSIGN_CNF;

    /* Restart L1 in dedicated mode */
    /* ISR context - no need for INTLOCK */
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
    l1_tskisr_blk->sub_state    = L1_DED_INIT;

    /* Run this so that the state transition takes effect immediately.
     * Otherwise we'd waste a frame till the ISR runs again
     */
    (void)L1_control_dedicated(gas_id);
}

/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_MODE_MODIFY

DESCRIPTION
  Invoked in the ISR context to copy over and setup the new channel parameters
  after the old one has been suspended

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_complete_channel_mode_modify( gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /* Now fill in the new dedicated data */
    l1_dedicated_data_ptr->channel_specification.channel_mode_1 =
      l1_dedicated_data_ptr->new_channel_specification.channel_mode_1;
    /* Initialise channel starting values otherwise incorrect value may be used
     * when initializing new modified channel
     */
    l1_dedicated_data_ptr->channel_specification.starting_time_valid = FALSE;
    l1_dedicated_data_ptr->starting_time = 0;

#ifdef FEATURE_GSM_AMR
    /* Now fill in the multirate configuration data */
    memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
             sizeof(l1_dedicated_data_ptr->multirate_configuration),
             &l1_dedicated_data_ptr->new_multirate_configuration,
             sizeof(multirate_config_T) );
#endif


    /* Save last tx power  & TA on current channel in case we need to
       reconnect
    */
    l1_dedicated_data_ptr->last_cell_tx_power =
    l1_dedicated_data_ptr->ms_power_level_in_use;

    l1_dedicated_data_ptr->last_cell_timing_advance =
    l1_dedicated_data_ptr->timing_advance_in_use;

    l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_CHANNEL_MODE_MODIFY_CNF;

    /* Restart L1 in dedicated mode */
    /* ISR context - no need for INTLOCK */
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
    l1_tskisr_blk->sub_state    = L1_DED_INIT;

    /* Run this so that the state transition takes effect immediately.
     * Otherwise we'd waste a frame till the ISR runs again
     */
    (void)L1_control_dedicated(gas_id);
}

/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_RECONNECT

DESCRIPTION
  Invoked in the ISR context to copy over and setup the old channel parameters
  after lower layer failure on new one

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_GSM_DTM
void gl1_complete_channel_reconnect( gas_id_t gas_id )
#else
static void gl1_complete_channel_reconnect( gas_id_t gas_id )
#endif
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /* Now fill in the original dedicated data */
    l1_dedicated_data_ptr->channel_specification =
    l1_dedicated_data_ptr->new_channel_specification;

#ifdef FEATURE_GSM_AMR
    /* Now fill in the multirate configuration data */
    memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
             sizeof(l1_dedicated_data_ptr->multirate_configuration),
             &l1_dedicated_data_ptr->new_multirate_configuration,
             sizeof(multirate_config_T) );
#endif

    l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;

    gl1_load_current_cell_data(gas_id);

    l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_RECONNECT_CHANNEL_CNF;

    /*skip re-synchronisation if moving to same cell*/
    if (l1_ded_if_data[gas_id].need_to_sync)
    {
        /* Now resynchronise to the new cell using data found by the
           surrounding cell monitoring process */
        l1_tskisr_blk->command = L1_SYNCH_TO_CURRENT_COMMAND;

        /* @@OG: Set the afc and frequency error */

        /* Initialise the band and the pdm value */
        gl1_msg_acq_afc_init( l1_dedicated_data_ptr->current_cell_details.arfcn, gas_id );

        if ( l1_dedicated_data_ptr->previous_cell_afc_valid )
        {
          /* Slam the rotator and the PDM back to the old values before the handover took place */
          gl1_msg_set_afc_params( l1_dedicated_data_ptr->previous_cell_rotator_value,
                                  l1_dedicated_data_ptr->previous_cell_freq_value,gas_id);

          l1_dedicated_data_ptr->previous_cell_afc_valid = FALSE;
          MSG_GERAN_MED_2_G("Get previous cell AFC params Freq=%d ROT=%d",
            l1_dedicated_data_ptr->previous_cell_freq_value,
            l1_dedicated_data_ptr->previous_cell_rotator_value);
        }
        else
        {
          MSG_GERAN_ERROR_0_G("No stored previous cell AFC info");

          /* @@OG: fine freq Now set the stored frequency offset
           * Use a quick hack to convert the Hz error to pdm value.
           */
          gl1_msg_reset_pdm( l1_dedicated_data_ptr->current_cell_details.frequency_offset,
                             l1_dedicated_data_ptr->current_cell_details.arfcn ,gas_id);
        }

        /* Run this so that the state transition takes effect immediately.
         * Otherwise we'd waste a frame till the ISR runs again
         */
        (void)L1_control_dedicated(gas_id);

        /* Tell CGPS we are leaving this cell */
        gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_POST_CELL_CHANGE,gas_id);
    }

    /* Set up for state transition to take effect on next frame, when ISR runs,
     * otherwise clash between Rx on old cell and monitor on reconnected cell */
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
    l1_tskisr_blk->sub_state    = L1_DED_INIT;
}

/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_HANDOVER

DESCRIPTION
  Invoked in the ISR context to copy over and setup the new channel parameters
  after the old one has been suspended

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_GSM_DTM
void gl1_complete_channel_handover( boolean  apply_afc, gas_id_t gas_id)
#else
static void gl1_complete_channel_handover( boolean  apply_afc, gas_id_t gas_id)
#endif
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    /*
       L1 does not need to remember that freq redef is
       imminent after a handover.  RR handles it after we
       switch back to the old cell.  There is the risk of
       the start time expiring w.r.t the new cell's timebase
    */
    l1_dedicated_data_ptr->freq_redef_imminent = FALSE;

    /* Now fill in the new dedicated data */
    l1_dedicated_data_ptr->channel_specification =
    l1_dedicated_data_ptr->new_channel_specification;

    /* Apply timing advance calculated above and save old TA */
    gl1_msg_store_ta( (uint8)l1_ded_if_data[gas_id].handover_ta,gas_id );
    l1_dedicated_data_ptr->timing_advance_in_use  = (byte)l1_ded_if_data[gas_id].handover_ta;
    l1_dedicated_data_ptr->timing_advance_to_use  = (byte)l1_ded_if_data[gas_id].handover_ta;
    l1_dedicated_data_ptr->ordered_timing_advance = (byte)l1_ded_if_data[gas_id].handover_ta;

    /* Apply handover power level */
    l1_ded_if_data[gas_id].handover_power_level = l1_dedicated_data_ptr->channel_specification.power_level;
    l1_dedicated_data_ptr->ms_power_level_in_use = l1_ded_if_data[gas_id].handover_power_level;
    l1_dedicated_data_ptr->ms_power_level_to_use = l1_ded_if_data[gas_id].handover_power_level;
    l1_dedicated_data_ptr->ms_power_level_to_report = l1_ded_if_data[gas_id].handover_power_level;
    l1_dedicated_data_ptr->ordered_ms_power_level = l1_ded_if_data[gas_id].handover_power_level;

    /* Update Idle ARFCN to that of target cell*/
    l1_idle_data->campedon_cell_ARFCN = l1_dedicated_data_ptr->target_cell_details.arfcn;
    /* Update Idle BSIC to that of target cell */
    l1_idle_data->campedon_cell_BSIC = l1_dedicated_data_ptr->target_cell_details.bsic;

    l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;
    l1_dedicated_data_ptr->L1_confirm_msg = (byte)PH_RANDOM_ACCESS_CNF;

    /* Set the number of handover access bursts to be sent */
    if ( l1_dedicated_data_ptr->handover_synchronization == NON_SYNCHRONISED )
    {
        l1_dedicated_data_ptr->no_of_ab_to_send = 0xff;
    }
    else
    {
        l1_dedicated_data_ptr->no_of_ab_to_send = 4 + 1;
    }

#ifdef FEATURE_GSM_AMR
    /* Now fill in the multirate configuration data */
    memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
             sizeof(l1_dedicated_data_ptr->multirate_configuration),
             &l1_dedicated_data_ptr->new_multirate_configuration,
             sizeof(multirate_config_T) );
#endif

    /*skip re-synchronisation if moving to same cell*/
    if (l1_ded_if_data[gas_id].need_to_sync)
    {
        /* Now resynchronise to the new cell using data found by the
           surrounding cell monitoring process */
        l1_tskisr_blk->command = L1_SYNCH_TO_TARGET_COMMAND;

        if( FALSE != apply_afc )
        {
          /* @@OG: Set the afc and frequency error */

          /* Initialise the band and the pdm value */
          gl1_msg_acq_afc_init( l1_dedicated_data_ptr->target_cell_details.arfcn, gas_id );

          /* @@OG: fine freq Now set the stored frequency offset
           * Use a quick hack to convert the Hz error to pdm value.
           */
          gl1_msg_reset_pdm( l1_dedicated_data_ptr->target_cell_details.frequency_offset,
                             l1_dedicated_data_ptr->target_cell_details.arfcn ,gas_id);

          l1_dedicated_data_ptr->current_cell_details.frequency_offset =
            (-1)*( l1_dedicated_data_ptr->target_cell_details.frequency_offset );
          MSG_GERAN_HIGH_1_G("HO current cell freq_offset %d",l1_dedicated_data_ptr->current_cell_details.frequency_offset);
        }

        /* Run this so that the state transition takes effect immediately.
         * Otherwise we'd waste a frame till the ISR runs again
         */
        (void)L1_control_dedicated(gas_id);

        /* Tell CGPS we are leaving this cell */
        gl1_timetag_cgps(L1_DEDICATED_MODE, CGPS_TIMETAG_POST_CELL_CHANGE,gas_id);
    }

    /* For Blind HO the AGC is primed correctly. */
    if ( !l1_ded_if_data[gas_id].do_gsm_blind_handover )
    {
      l1_prime_agc_for_handover(l1_dedicated_data_ptr->target_cell_details.arfcn,gas_id);
    }

#ifdef GERAN_L1_ENHANCED_RX
    /*Reset LIF table during handovers*/
    gl1_hw_reset_lif_table(gas_id);
#endif /*GERAN_L1_ENHANCED_RX*/

    /*Reset ACI Global on Handover*/
    gl1_hw_aci_reset_table(gas_id);

    L1_send_MPH_HANDOVER_IND(NO_ERROR, gas_id);

    /* L1 ISR is now in DEDICATED - L1_DED_SUSPEND mode and will wait
       there for a PH_RANDOM_ACCESS_REQ  */
}

/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_RELEASE

DESCRIPTION
  Invoked in the ISR context to transition to IDLE or NULL state after
  the dedicated channel has been released

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gl1_complete_channel_release( gas_id_t gas_id )
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

#ifdef FEATURE_GSM_DTM
    /* Since we want to change the timebase back to TN 0
     * we only need CS TN when new_tn > current_tn
     */
    if(l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_LOWEST)
    {
      MSG_GERAN_HIGH_1_G("Slam CS to lowest -> tn 0 [%d]",gl1_get_FN( gas_id ));
      L1_change_timeslot(0, gas_id);

      /* Inform Message layer to Rx and Tx all CS chanels on TN 0 */
      gl1_msg_set_cs_timeslot(GL1_DEFS_TN_0, gas_id );

    }else if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
    {

      MSG_GERAN_HIGH_1_G("Slam CS to rel 2 -> tn 0 [%d]",gl1_get_FN( gas_id ));
      /* change timeslot so that the CS slot lies on relative TN 2 */
      L1_dtm_change_timeslot( 0,NULL, gas_id );

      /* Inform Message layer to Rx and Tx all CS chanels on TN 2 */
      gl1_msg_set_cs_timeslot(GL1_DEFS_TN_0, gas_id );

      /* Now change the mode back to the lowest TN Slam */
      l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

    }
#endif

    if ( l1_ded_if_data[gas_id].valid_bcch_carrier )
    {
        /* Change the ISR mode */
        l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
        l1_tskisr_blk->next_params.BSIC = l1_idle_data->campedon_cell_BSIC;
        l1_tskisr_blk->next_params.ARFCN = l1_idle_data->campedon_cell_ARFCN;
        l1_tskisr_blk->next_params.extended_BCCH_mask = SERV_EXT_BCCH_MASK_NONE;

        /* Transition to L1_FIND_BCCH state. We do not go back into the
         * start of find BCCH , but the part that reads the BCCH
         * ISR context - no need for INTLOCK
         */
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_tskisr_blk->command = L1_NULL_COMMAND;
        L1_transfer_new_data( gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        /* Need to decide whether to return to DUAL SIM or single SIM states */
        if ( gl1_msg_get_multi_sim_mode() )
        {
          l1_tskisr_blk->sub_state = L1_MULTI_SIM_RESELECTION;
          l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
          l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
          l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
#ifdef FEATURE_GSM_RX_TX_SPLIT
          /* Release RF, FW and TRM to prevent GL1 holding Tx Device */
          call_in_task_grm_release(l1_tskisr_blk->client_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
        }
        else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        {
          l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
          l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
          l1_change_state( L1_FIND_BCCH, gas_id );
          l1_call_in_task_turn_tx_off(gas_id);
        }

        L1isr_send_MPH_CHANNEL_RELEASE_CNF (NO_ERROR, gas_id);
        gl1_msg_disable_ciphering(gas_id);
    }
    else
    {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        /* Need to decide whether to return to DUAL SIM or single SIM states */
        if ( gl1_msg_get_multi_sim_mode() )
        {
          l1_change_state( L1_MULTI_SIM_NULL, gas_id );
          /* Release RF, FW and TRM to prevent GL1 holding Tx Device */
          call_in_task_grm_release(l1_tskisr_blk->client_id);
        }
        else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        {
          /* Transition to L1_NULL_MODE state
           * ISR context - no need for INTLOCK
           */
          l1_change_state( L1_NULL_MODE, gas_id );
        }

        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        L1_terminate_dedicated();
        L1isr_send_MPH_CHANNEL_RELEASE_CNF (NO_ERROR, gas_id);
        gl1_msg_disable_ciphering(gas_id);
        l1_call_in_task_turn_tx_off(gas_id);
    }

    gl1_msg_terminate_tch_amr_fr_hr(gas_id);
    if(l1_ded_if_data[gas_id].stored_dtx_indicator)
    {
        l1_vs_set_dtx_mode(DTX_DISABLED, gas_id);
        l1_ded_if_data[gas_id].stored_dtx_indicator = FALSE;
    }
    l1_vs_stop_client(gas_id);
}


#ifdef FEATURE_GSM_DTM

#if 0 /* Not used until DTM released 0.2 */
/*===========================================================================

FUNCTION GL1_COMPLETE_CHANNEL_ASSIGNMENT

DESCRIPTION
  Invoked in the ISR context to copy over and setup the new channel parameters
  after the old one has been suspended

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gpl1_dtm_complete_channel_assignment(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  byte temp;

  /* Now fill in the new dedicated data */
  memscpy (&l1_dedicated_data_ptr->channel_specification,
           sizeof(l1_dedicated_data_ptr->channel_specification),
           &l1_dedicated_data_ptr->new_channel_specification,
           sizeof(channel_spec_T) );

#ifdef FEATURE_GSM_AMR
  /* Now fill in the multirate configuration data */
  memscpy (&l1_dedicated_data_ptr->multirate_configuration,
           sizeof(l1_dedicated_data_ptr->multirate_configuration),
           &l1_dedicated_data_ptr->new_multirate_configuration,
           sizeof(multirate_config_T) );
#endif

  l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;

  /* Load up transmit power */
  temp = l1_dedicated_data_ptr->channel_specification.power_level;
  l1_dedicated_data_ptr->ms_power_level_in_use = temp;
  l1_dedicated_data_ptr->ms_power_level_to_use = temp;
  l1_dedicated_data_ptr->ms_power_level_to_report = temp;
  l1_dedicated_data_ptr->ordered_ms_power_level = temp;

  l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_DTM_CHANNEL_ASSIGN_CNF;

  /* Restart L1 in dedicated mode */
  /* ISR context - no need for INTLOCK */
  l1_tskisr_blk->main_command = L1_NULL_COMMAND;
  l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
  l1_tskisr_blk->sub_state    = L1_DED_INIT;

  /* Run this so that the state transition takes effect immediately.
   * Otherwise we'd waste a frame till the ISR runs again
   */
  (void)L1_control_dedicated(gas_id);

}
#endif /* end of if 0 */
#endif /*FEATURE_GSM_DTM*/

/*===========================================================================

FUNCTION GL1_LOAD_CURRENT_CELL_DATA

DESCRIPTION
  This function installs the dedicated mode data for traffic on the handover's
  current cell.(When reconnecting to the old cell due to a handover failure)


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_load_current_cell_data( gas_id_t gas_id )
{
    idle_data_T               *l1_idle_data = &l1_idle_data_store[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    byte temp;

    /* Retrieve the old cell data */

    /* Apply previous cell's TA */
    temp = l1_dedicated_data_ptr->last_cell_timing_advance;
    gl1_msg_store_ta( temp,gas_id);
    l1_dedicated_data_ptr->timing_advance_in_use = temp;
    l1_dedicated_data_ptr->timing_advance_to_use = temp;
    l1_dedicated_data_ptr->ordered_timing_advance = temp;

    /* Apply previous cell's tx pwr */
    temp = l1_dedicated_data_ptr->last_cell_tx_power;
    l1_dedicated_data_ptr->ms_power_level_to_use = temp;
    l1_dedicated_data_ptr->ms_power_level_in_use = temp;
    l1_dedicated_data_ptr->ms_power_level_to_report = temp;
    l1_dedicated_data_ptr->ordered_ms_power_level = temp;

    /* Apply previous cell's rx_pwr to initialize the agc */
    MSG_GERAN_MED_1_G("Applying previous cell rx_power: %d", l1_dedicated_data_ptr->last_cell_rx_power);
    sys_algo_agc_init( &l1_dedicated_data_ptr->c0_agc, l1_dedicated_data_ptr->last_cell_rx_power );

    /* Revert idle data back to old cell */
    l1_idle_data->campedon_cell_BSIC = l1_dedicated_data_ptr->current_cell_details.bsic;
    l1_idle_data->campedon_cell_ARFCN = l1_dedicated_data_ptr->current_cell_details.arfcn;

    /* Flag to remove any time tracking from failed cell */
    gl1_msg_set_tt_failed_ho(gas_id);

#ifdef FEATURE_GSM_AMR
#ifdef FEATURE_GSM_AMR_WB
 if ( (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3 ) ||
      (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V5 ) )
#else
    if (l1_dedicated_data_ptr->channel_specification.channel_mode_1 == SPEECH_V3 )
#endif
    {
        uint8 i;

        /* Take care of AMR stuff */
        for (i = 0; i < MAX_CODEC_MODES; i++ )
        {
            curr_dl_acs[gas_id][i] = l1_dedicated_data_ptr->amr_details.dl_acs[i];
            curr_ul_acs[gas_id][i] = l1_dedicated_data_ptr->amr_details.ul_acs[i];

            if ( i < (MAX_CODEC_MODES - 1) )
            {
                curr_thresh_tbl[gas_id][i] =
                l1_dedicated_data_ptr->amr_details.thresh_tbl[i];
                curr_hyst_tbl[gas_id][i] =
                l1_dedicated_data_ptr->amr_details.hyst_tbl[i];
            }
        }
        curr_dl_num_acm[gas_id] = l1_dedicated_data_ptr->amr_details.dl_num_acm;
        curr_ul_num_acm[gas_id] = l1_dedicated_data_ptr->amr_details.ul_num_acm;
        curr_dl_icm[gas_id] = l1_dedicated_data_ptr->amr_details.dl_icm;
        curr_ul_icm[gas_id] = l1_dedicated_data_ptr->amr_details.ul_icm;
        curr_phase[gas_id] = l1_dedicated_data_ptr->amr_details.dl_phase;

        if ( (ratscch_rsp_pending[gas_id] == FALSE) &&
             (ratscch_pending_req[gas_id] != RATSCCH_UNDEF_REQ )
           )
        {
            ratscch_activate_req_in_dl(gas_id);
            ratscch_activate_req_in_ul(gas_id);
        }
    }
#endif
}

/*===========================================================================

FUNCTION gl1_begin_gsm_blind_handover

DESCRIPTION
  Invoked in the ISR context to perform GSM->GSM blind handover if possible
  and transition to DED_SUSPEND on success, or DED_INIT on failure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_begin_gsm_blind_handover( boolean acq_success, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    boolean blind_handover_is_possible = FALSE;
    byte ho_ta;

    if ( acq_success )
    {
        /* Check if handover is possible - validate timing advance */
        if ( l1_dedicated_data_ptr->target_cell_details.bsic_known )
        {
            if( gl1_calc_ho_timing_advance ( &l1_dedicated_data_ptr->target_cell_details, &ho_ta, gas_id ) &&
                (l1_dedicated_data_ptr->ho_details.BSIC == l1_dedicated_data_ptr->target_cell_details.bsic) )
            {
                blind_handover_is_possible = TRUE;
            }
        }
    }

    if ( blind_handover_is_possible )
    {
        /* Sync needed for inter-cell handovers */
        l1_ded_if_data[gas_id].need_to_sync = TRUE;

        /* Transition to DEDICATED SUSPEND mode (waiting for PH_RANDOM_ACCESS_REQ)
         * ISR context - no need for INTLOCK
         */
        l1_change_state( L1_DEDICATED_MODE, gas_id );
        l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        l1_tskisr_blk->command = L1_NULL_COMMAND;

        /* Copy over channel specs first because L1_control_dedicated,
         * L1_sync_to_target use pDedi_data
         */
        l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
        L1_transfer_new_data( gas_id );

        /* @OG: Do not apply, it is already applied by the acquisition process */
        gl1_complete_channel_handover( FALSE, gas_id );

        MSG_GERAN_HIGH_0_G("GSM->GSM Blind Handover success");
    }
    else
    {
        /* Reset the BHO flag */
        l1_tskisr_blk->acq_for_blind_ho = FALSE;
        /*Send the Handover Ind message upon reconnection to the old channels*/
        l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_HANDOVER_IND;

        /* Restart L1 in DEDICATED INIT mode
         * ISR context - no need for INTLOCK
         */
        l1_change_state( L1_DEDICATED_MODE, gas_id );
        l1_tskisr_blk->sub_state    = L1_DED_INIT;
        l1_tskisr_blk->main_command = L1_NULL_COMMAND;

        /* Copy over channel specs first because L1_control_dedicated
         * uses pDedi_data
         */
        l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;
        L1_transfer_new_data( gas_id );

        /* Don't call L1_control_dedicated() here - will result in
         * an infinite loop
         */
        MSG_GERAN_HIGH_0_G("GSM->GSM Blind Handover failed");
    }
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION gpl1_dtm_save_dl_alloc

DESCRIPTION
  Takes the parameters decribing an downlink allocation from the MAC_L1_DTM_ASSIGNMENT
  and apply them to the l1_transfer_data[gas_id] structure in preparation for entering
  DL TBF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gpl1_dtm_save_dl_alloc( mac_downlink_tbf_t *dl_tbf, gas_id_t gas_id )
{

downlink_tbf_information_T *dl_msg_dest_ptr=NULL;
volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

  MSG_GERAN_MED_0_G("DL_TBF_PRESENT in DTM assignment");

  /* Sort out which of the DL data pointers to use */
  if ( l1_transfer_data[gas_id].dl_tbf_ptr == &(l1_transfer_data[gas_id].dl_tbf_1) )
  {
    /* Reconfigure downlink as part of concurrent tbf */
    dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

    /* Setup pending downlink structure */
    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
  }
  else
  {

    /* Reconfigure downlink as part of concurrent tbf */
    dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

    /* Setup pending downlink structure */
    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

  }


  /* Setup mode to determine whether this TBF is EGPRS or GPRS */
  l1_transfer_data[gas_id].egprs_or_gprs_tbf = dl_tbf->tbf_mode;

  /* (1) Used for passing MCS 1- 9 crc failed payload data to RLC ONLY in UNACK mode
   * for RLC ACK mode  MCS 1- 9 crc failed payload data is thrown away
   *
   * (2) Used to Setup Incremental redundancy for Downlink TBF
   * RLC Unack mode = IR OFF
   * RLC ACK mode  = IR ON
   */
  dl_msg_dest_ptr->rlc_mode = dl_tbf->rlc_mode;

  dl_msg_dest_ptr->link_qual_meas_mode = dl_tbf->link_qual_meas_mode;

  if( FALSE != dl_tbf->bep_period2.present )
  {
    dl_msg_dest_ptr->bep_period2 = dl_tbf->bep_period2.bep_period2;
  }
  else
  {
    dl_msg_dest_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
  }


  l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

  if( L1_DEDICATED_MODE == l1_tskisr_blk->l1_state )
  {
    /*  Reset ul_desc pointer so any previous READS discarded. */
    memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
    l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  =
                                              &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];
  }

  /* copy mac mode for the downlink assignment */
  l1_transfer_data[gas_id].dl_tbf_1.mac_mode = dl_tbf->mac_mode;

  /* copy power control parameters */
  /*  Load serving cell parameters from message */
  if (dl_tbf->power_control.present)
  {
    uint8 index;
    power_control_gamma_T   *pwr_cntrl_ptr;

    /*  Update Alpha value  */
    gprs_serv_cell_meas_ptr->alpha = dl_tbf->power_control.alpha;

    /*  Extract gamma_tn values  */
    pwr_cntrl_ptr = &(dl_tbf->power_control.gamma_TN0);
    index = 0;

    do
    {
      if (pwr_cntrl_ptr->present)
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
      }
      else
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
      }

      /*  Increment pointer  */
      pwr_cntrl_ptr++;
      index++;

    }while (index < 8);

  }
  else
  {
    uint8 index;
    /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
    /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
    for(index=0;index<8;index++)
    {
      gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
    }

    gprs_serv_cell_meas_ptr->alpha = 0;

  }

  /*  Update uplink power based on the modified power control parameters. */
  gpl1_gprs_calc_rf_pwr( gas_id);

  /* setup downlink power control parameter's */
  l1_transfer_data[gas_id].dl_power_control = dl_tbf->downlink_pwr;

  /* Not checking present flag as assuming DL timeslot alloc
   * always present.
   *
   * Change order of timeslot description
   */
  dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn
                                            (
                                              &dl_tbf->ts_allocation,
                                              &dl_msg_dest_ptr->ts_allocation
                                            );

  gpl1_gprs_add_to_active_dl_ta_params(dl_tbf->ta_params, gas_id);

  if (dl_tbf->dl_tfi_present)
  {
    dl_msg_dest_ptr->dl_tfi_present = TRUE;
    dl_msg_dest_ptr->dl_tfi = dl_tbf->dl_tfi;
  }
  else
  {
    /* should already have tfi from UL TBF */
    dl_msg_dest_ptr->dl_tfi_present = FALSE;
  }

  /* There is no starting time in DTM assignments */
  l1_transfer_data[gas_id].starting_time_valid = FALSE;
  l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
  l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
  l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

  l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;
  l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
  l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;

}

/*===========================================================================

FUNCTION gpl1_dtm_save_ul_alloc

DESCRIPTION
  Takes the parameters decribing an uplink allocation from the MAC_L1_DTM_ASSIGNMENT
  and apply them to the l1_transfer_data structure in preparation for entering
  UL TBF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gpl1_dtm_save_ul_alloc( mac_dynamic_tbf_information_T *ul_tbf, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint8  ul_ts_alloc = 0;
  uint16 i;
  dynamic_tbf_information_T *dynamic_msg_dest_ptr=NULL;

  MSG_GERAN_MED_0_G("UL_TBF_PRESENT in DTM assignment");

  if( l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_1) )
  {
    /* Reconfigure dynamic tbf or add to existing DL tbf */
    dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

    /* Setup pending dynamic structure */
    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

  }
  else
  {
    /* reconfigure dynamic tbf */
    dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

    /* Setup pending dynamic structure */
    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
   }


  /* Setup mode to determine whether this TBF is EGPRS or GPRS */
  l1_transfer_data[gas_id].egprs_or_gprs_tbf = ul_tbf->tbf_mode;

  /* Setup test mode for EGPRS
  * either TEST_MODE_OFF, TEST_MODE_B or TEST_MODE_SRB
  */
  dynamic_msg_dest_ptr->test_mode = ul_tbf->test_mode;


  /* Setup mac mode */
  dynamic_msg_dest_ptr->mac_mode = ul_tbf->mac_mode;

  if( FALSE != ul_tbf->bep_period2.present )
  {
    dynamic_msg_dest_ptr->bep_period2 = ul_tbf->bep_period2.bep_period2;
  }
  else
  {
    dynamic_msg_dest_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
  }

  l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

  if (ul_tbf->power_control_params.present)
  {
    uint8                   index;
    power_control_gamma_T   *pwr_cntrl_ptr;

    /*  Update Alpha value  */
    gprs_serv_cell_meas_ptr->alpha = ul_tbf->power_control_params.alpha;

    /*  Extract gamma_tn values  */
    pwr_cntrl_ptr = &(ul_tbf->power_control_params.gamma_TN0);
    index = 0;

    do
    {
      if (pwr_cntrl_ptr->present)
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
      }
      else
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
      }

      /*  Increment pointer  */
      pwr_cntrl_ptr++;
      index++;

    }while (index < 8);

  }
  else
  {
    uint8 index;
    /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
    /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
    for(index=0;index<8;index++)
    {
      gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
    }
    gprs_serv_cell_meas_ptr->alpha = 0;

  }

  /*  Update uplink power based on the modified power control parameters. */
  gpl1_gprs_calc_rf_pwr( gas_id);

  /* setup downlink power control parameter's */
  if (ul_tbf->dl_ctrl_param.params_valid)
  {
    l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
    l1_transfer_data[gas_id].dl_power_control.po = ul_tbf->dl_ctrl_param.po;
  }
  else
  {
    l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
  }

  dynamic_msg_dest_ptr->usf_granularity = ul_tbf->usf_granularity;

  gpl1_gprs_add_to_active_ul_ta_params(ul_tbf->timing_advance_params,FALSE,0, gas_id);

  /* Map MAC description of USF values and timeslots into L1_transfer_data */
  /* Mapping element 0 = usf value for TN0
   *         element 7 = usf value for TN7
   *
   * USF value range 0 to 7
   * USF value = 0xff TN not supported
   */
  dynamic_msg_dest_ptr->nts = 0;

  /* ensure the Downlink TBF pointer is null */
  l1_transfer_data[gas_id].dl_tbf_ptr=0;

  for(i=0;i< MAX_TN;i++)
  {
    if (ul_tbf->usf[i].present)
    {
      /* For debug purposes */
      ul_ts_alloc |= ( 0x80 >> i );

      dynamic_msg_dest_ptr->usf_value[i] = ul_tbf->usf[i].usf_value;
      dynamic_msg_dest_ptr->nts++;
    }
    else
    {
      /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
      dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;
    }
  }


  /* For the benifit of any possible error condition check that nts is valid */
  if ( dynamic_msg_dest_ptr->nts > GL1_DEFS_MAX_ASSIGNED_UL_TS )
  {
    MSG_GERAN_ERROR_2_G("Unsupported NTS %d - clip to %d",
      dynamic_msg_dest_ptr->nts,GL1_DEFS_MAX_ASSIGNED_UL_TS);

    dynamic_msg_dest_ptr->nts = GL1_DEFS_MAX_ASSIGNED_UL_TS;
  }

  if( L1_DEDICATED_MODE == l1_tskisr_blk->l1_state )
  {
    /*  Reset ul_desc pointer so any previous READS discarded. */
    memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );

    l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  =
                                      &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];
  }

  /* There is no starting time in DTM assignments */
  l1_transfer_data[gas_id].starting_time_valid = FALSE;
  l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
  l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
  l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

  /* setup tbf establish type */
  l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
  l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
  l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
  l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

  MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

}

/*===========================================================================

FUNCTION gpl1_dtm_save_uldl_alloc

DESCRIPTION
  Takes the parameters decribing an uplink and downlink allocation from
  the MAC_L1_DTM_ASSIGNMENT. This can only be for a reassignment during
  concurrent TBF's.
  For parameters common to both assignments the DL parameters are used.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gpl1_dtm_save_uldl_alloc( mac_l1_dtm_channel_assign_req_t *dtm_channel_assign )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[dtm_channel_assign->gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[dtm_channel_assign->gas_id];
  uint8  ul_ts_alloc = 0;
  uint16 i;
  dynamic_tbf_information_T *dynamic_msg_dest_ptr=NULL;
  downlink_tbf_information_T *dl_msg_dest_ptr=NULL;
  gas_id_t gas_id = dtm_channel_assign->gas_id;

  MSG_GERAN_MED_0_G("DL_TBF_PRESENT in DTM assignment");

  /* Sort out which of the DL data pointers to use */
  if ( l1_transfer_data[gas_id].dl_tbf_ptr == &(l1_transfer_data[gas_id].dl_tbf_1) )
  {
    /* Reconfigure downlink as part of concurrent tbf */
    dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

    /* Setup pending downlink structure */
    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
  }
  else
  {

    /* Reconfigure downlink as part of concurrent tbf */
    dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

    /* Setup pending downlink structure */
    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

  }


  /* Setup mode to determine whether this TBF is EGPRS or GPRS */
  l1_transfer_data[gas_id].egprs_or_gprs_tbf = dtm_channel_assign->dl_tbf.tbf_mode;

  /* (1) Used for passing MCS 1- 9 crc failed payload data to RLC ONLY in UNACK mode
   * for RLC ACK mode  MCS 1- 9 crc failed payload data is thrown away
   *
   * (2) Used to Setup Incremental redundancy for Downlink TBF
   * RLC Unack mode = IR OFF
   * RLC ACK mode  = IR ON
   */
  dl_msg_dest_ptr->rlc_mode = dtm_channel_assign->dl_tbf.rlc_mode;

  dl_msg_dest_ptr->link_qual_meas_mode = dtm_channel_assign->dl_tbf.link_qual_meas_mode;

  if( FALSE != dtm_channel_assign->dl_tbf.bep_period2.present )
  {
    dl_msg_dest_ptr->bep_period2 = dtm_channel_assign->dl_tbf.bep_period2.bep_period2;
  }
  else
  {
    dl_msg_dest_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
  }


  l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

  if( L1_DEDICATED_MODE == l1_tskisr_blk->l1_state )
  {

    /*  Reset ul_desc pointer so any previous READS discarded. */
    memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
    l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  =
                                              &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];
  }

  /* copy power control parameters */
  /*  Load serving cell parameters from message */
  if (dtm_channel_assign->dl_tbf.power_control.present)
  {
    uint8 index;
    power_control_gamma_T   *pwr_cntrl_ptr;

    /*  Update Alpha value  */
    gprs_serv_cell_meas_ptr->alpha = dtm_channel_assign->dl_tbf.power_control.alpha;

    /*  Extract gamma_tn values  */
    pwr_cntrl_ptr = &(dtm_channel_assign->dl_tbf.power_control.gamma_TN0);
    index = 0;

    do
    {
      if (pwr_cntrl_ptr->present)
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
      }
      else
      {
        gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
      }

      /*  Increment pointer  */
      pwr_cntrl_ptr++;
      index++;

    }while (index < 8);

  }
  else
  {
    uint8 index;
    /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
    /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
    for(index=0;index<8;index++)
    {
      gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
    }

    gprs_serv_cell_meas_ptr->alpha = 0;

  }

  /*  Update uplink power based on the modified power control parameters. */
  gpl1_gprs_calc_rf_pwr( dtm_channel_assign->gas_id);

  /* setup downlink power control parameter's */
  l1_transfer_data[gas_id].dl_power_control = dtm_channel_assign->dl_tbf.downlink_pwr;

  /* Not checking present flag as assuming DL timeslot alloc
   * always present.
   *
   * Change order of timeslot description
   */
  dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn
                                            (
                                              &dtm_channel_assign->dl_tbf.ts_allocation,
                                              &dl_msg_dest_ptr->ts_allocation
                                            );

  gpl1_gprs_add_to_active_dl_ta_params(dtm_channel_assign->dl_tbf.ta_params,dtm_channel_assign->gas_id);

  if (dtm_channel_assign->dl_tbf.dl_tfi_present)
  {
    dl_msg_dest_ptr->dl_tfi_present = TRUE;
    dl_msg_dest_ptr->dl_tfi = dtm_channel_assign->dl_tbf.dl_tfi;
  }
  else
  {
    /* should already have tfi from UL TBF */
    dl_msg_dest_ptr->dl_tfi_present = FALSE;
  }

//UL ASSIGNMENT

  MSG_GERAN_MED_0_G("UL_TBF_PRESENT in DTM assignment");

  if( l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_1) )
  {
    /* Reconfigure dynamic tbf or add to existing DL tbf */
    dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

    /* Setup pending dynamic structure */
    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

  }
  else
  {
    /* reconfigure dynamic tbf */
    dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

    /* Setup pending dynamic structure */
    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
   }


  /* setup downlink power control parameter's */
  if (dtm_channel_assign->ul_tbf.dl_ctrl_param.params_valid)
  {
    l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
    l1_transfer_data[gas_id].dl_power_control.po = dtm_channel_assign->ul_tbf.dl_ctrl_param.po;
  }
  else
  {
    l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
  }

  dynamic_msg_dest_ptr->usf_granularity = dtm_channel_assign->ul_tbf.usf_granularity;

  gpl1_gprs_add_to_active_ul_ta_params(dtm_channel_assign->ul_tbf.timing_advance_params,FALSE,0, dtm_channel_assign->gas_id);

  /* Setup mac mode */
  dynamic_msg_dest_ptr->mac_mode = dtm_channel_assign->ul_tbf.mac_mode;

  /* Map MAC description of USF values and timeslots into L1_transfer_data */
  /* Mapping element 0 = usf value for TN0
   *         element 7 = usf value for TN7
   *
   * USF value range 0 to 7
   * USF value = 0xff TN not supported
   */
  dynamic_msg_dest_ptr->nts = 0;

  for(i=0;i< MAX_TN;i++)
  {
    if (dtm_channel_assign->ul_tbf.usf[i].present)
    {
      /* For debug purposes */
      ul_ts_alloc |= ( 0x80 >> i );

      dynamic_msg_dest_ptr->usf_value[i] = dtm_channel_assign->ul_tbf.usf[i].usf_value;
      dynamic_msg_dest_ptr->nts++;
    }
    else
    {
      /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
      dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;
    }
  }


  /* For the benifit of any possible error condition check that nts is valid */
  if ( dynamic_msg_dest_ptr->nts > GL1_DEFS_MAX_ASSIGNED_UL_TS )
  {
    MSG_GERAN_ERROR_2_G("Unsupported NTS %d - clip to %d",
      dynamic_msg_dest_ptr->nts,GL1_DEFS_MAX_ASSIGNED_UL_TS);

    dynamic_msg_dest_ptr->nts = GL1_DEFS_MAX_ASSIGNED_UL_TS;
  }

  if( L1_DEDICATED_MODE == l1_tskisr_blk->l1_state )
  {
    /*  Reset ul_desc pointer so any previous READS discarded. */
    memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );

    l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  =
                                        &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

  }

  /* There is no starting time in DTM assignments */
  l1_transfer_data[gas_id].starting_time_valid = FALSE;
  l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
  l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
  l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

  /* setup tbf establish type */
  l1_transfer_data[gas_id].tbf_est_type = UPLINK_DOWNLINK_TBF_ESTABLISHED;
  l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
  l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

  MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

}

#endif /* FEATURE_GSM_DTM */

/* This function is used to allow clients to reset the
 * current_rr_l1_msg_id variable.
 */
void  l1_reset_rr_current_msg_id( gas_id_t gas_id )
{

  /* Set the msg id to NULL */
  if( NULL != l1_ded_if_data[gas_id].current_rr_l1_msg_id )
  {
    MSG_GERAN_MED_1_G(" Nulling current_rr_l1_msg_id %d ", l1_ded_if_data[gas_id].current_rr_l1_msg_id);

    l1_ded_if_data[gas_id].current_rr_l1_msg_id = NULL;
  }
}

/* This function is used to allow SDCCH control
 * to get value of current_rr_l1_msg_id variable.
 */

byte l1_get_current_rr_l1_msg_id( gas_id_t gas_id )
{
   return (l1_ded_if_data[gas_id].current_rr_l1_msg_id);
}

/*===========================================================================

FUNCTION gl1_msg_get_stored_dtx_indicator

DESCRIPTION
  Access function to allow gdriver funtions to access the stored_dtx_indicator
  variable

DEPENDENCIES
  None

RETURN VALUE
  boolean        stored_dtx_indicator

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_stored_dtx_indicator( gas_id_t gas_id )
{
   return l1_ded_if_data[gas_id].stored_dtx_indicator;
}
