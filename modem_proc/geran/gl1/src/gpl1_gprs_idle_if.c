/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1  I D L E  M O D E  T A S K  G P R S  M E S S A G E
                              P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events associated with GPRS
   while the L1 task is in the IDLE mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_idle_if.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   -----  ----------------------------------------------------------
15/06/18   ng     CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
18/05/17   sn     CR2048462 Release the Tx chain when MAC_L1_RELEASE is received in multi sim idle mode when TRM status is Granted also
07/03/18   sn     CR2199105 Priortize DL TBF over CCCH based on Reserved Frame number
25/05/17   hd     CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
10/05/17   sk     CR2024440 Add an explicit intlock for DivRx meas buffer update
09/02/16   mk     CR2002074 Add LB_MS_TXPWR_MAX_CCH for Tx power level initialization
09/09/16   sn     CR1062272 TRM RESERVE AT during IDLE to transfer
01/08/16   og     CR1038345. UE does not get RAU accept after SRVCC.
08/09/17   sn     CR1061730 FR43082: TA during PS Procedures- GL1 changes
16/03/17   mn     CR2020307 FR40846:Packet transfer Error Recovery mechanism.
22/07/16   sn     CR1044686 TRM is not released during TBF Release in Idle to TBF transition mode.
18/5/16    sm     CR1017901 Added GL1 idle mode QSH events 
14/10/15   sk     CR912014 Ensure idle SCE stopped when entering dedicated mode from idle mode in EDTM scenario
21/04/15   cs     CR808438 Ensure any pending TT requests are sent before cell
                  information is updated
07/04/15   ab     CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX
08/04/15   pjr    CR818535 FR25951 Changes to support Single Voice Session
08/04/15   pjr    CR818535 FR25951 Changes to support Single Voice Session
05/12/14   sp     CR767083: Override paging multiframe to 2, for PDT tests
11/02/15   ap     CR775735: Remove TRM request when handling MAC_L1_DL_TBF_CONFIG command in idle mode
30/01/15   pa     CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
09/01/15   nk     CR779439 Release ISR mutext after moving main command to IDLE
12/12/14   pa     CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   ak     CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
25/11/14   nk     CR762438 Release ISR mutex only after moving main_command to IDLE command while handling MAC_L1_RELEASE
03/12/14   npt    CR759467 : Fix compiler warnings
11/10/14   js     CR719550 : Dont Reject DL TBF config when QTA is ongoing.
31/10/14   js     CR748297 Removing check to reject DL TBF config when RX is going on in QTA
28/09/14   mko    CR746655 Suspend TRM release by 2 frames after FW load
20/10/14   ap     CR742915 DRDSDS: GL1 is holding non TX device when moving to Transfer mode from Idle
12/09/14   nm     CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
08/06/14   am     CR704022 Don't reserve TRM unconditionaly on DL_TBF_CONFIG
21/08/14   ap     CR689764: DDS switch to sub2 for MMS results in "stuck in Idle abort"
06/08/14   df     CR536059 Allow Asynchronous sleep for all idle activities
29/07/14   ap     CR695234: Changes for GL1's New Rach interface
04/07/14   cs     CR688003 GRM functions moved to gpl1_grm_intf.c
30/05/14   ws     CR657674 Data + MMS Phase 1
17/06/14   cs/ch  CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa  CR660831 Use new trm_status definitions specific to GL1
08/05/14   sp     CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14   br     CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
06/05/14   sp     CR655959: Ensure reserve_at is not done when pending release on TBF Config request
15/04/14   ka     CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
11/04/14   pg     CR645875 Fix Bolt multisim compilation errors
03/04/14   cja    CR641428 Correct serving cell qbit lag for ccch_group when add to ba list
08/03/14   am     CR624825 If TRM is retained for TBF assignment then TRM release should happen on TBF release
19/02/14   ap   CR618768 In SGLTE+G SS Retain TRM lock for TRAFFIC
27/01/14   sp     CR605100: Attempt SCH Reconfirmation if cell timing is known on Reselection Request.
08/01/14   nk     CR593528 Update trm_status when MAC_L1_RELEASE has come even when l1_state is still MULTI_SIM_IDLE
11/12/13   sk     CR588079 Ensure Transfer doesnt start when TRM release pending in the task
05/12/13   am     CR581771 Ensure frequency correction to re-selection cell is always applied
02/12/13   zc   CR583866 Adding case statement for UL+DL Abnormal TBF Release
24/12/13   ap     CR593525: GL1 Consecutive initializing of geran firmware within 1ms - causing gfw crash
14/12/13   sjw    CR589502: Install different unlock callback for change of acq priority
13/12/13   ap     CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
19/11/13   ssh/ip CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
18/11/13   pjr    CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
15/11/13   nm     CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
05/10/13   sk     CR554665 Backout the power opt temperorily(CR537690)
05/10/13   js     CR461049Cell Reselection Failure when 3 KHz frequency drift is applied on N cell
01/10/13   sk     CR552259 CR sync from 1.0/1.1
27/09/13   sp     CR551580: Add NV logging of outgoing L1 messages
20/08/13   nn     CR518344 Use secapi_get_random() for generating random numbers.
24/09/13   zf     CR537690: Reduce G wakeup timeline for MS mode
17/09/13   sp     CR542963: Removed Reset of release pending at gprs idle mode req.
05/07/13   ws     CR508303 Check dual_sim idle mode state if in DSDx mode when releasing TBF
08/08/13   dv     CR519879 Revert CR513969
16/07/13   dv     CR513969 Prevent Changing from Idle to xfer in QTA gap
21/06/13   cs     Major Triton TSTS Syncup
28/05/13   ap     CR461268: DSDS XO management + async time drift problem
22/05/13   ap     CR485912: Revised GFW state transitions
06/06/13   cs     Further TSTS Updates
22/05/13   cs     TSTS Updates
26/04/13   zf     CR477992: Fix G2W IRAT issue
09/04/13   pjr    CR470217 Ported next_params.L1Data.pDedi_data asignments from DIME
21/02/13   ap     CR451556 DSDS: Bad AFC/timing on PCH decode when long TBF start time is given
15/02/13   og     CR453012. Prevent sleep when the idle mode handler processes
                  the MAC_L1_DL_TBF_CONFIG message.
14/02/13   pjr    CR452272 added support to set multisim mode at activation time
15/02/13   ws     CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
09/10/12   cgc    CR393032 add l1_idle_if_gps_notify()
20/09/12   ws     CR 401931 - removed references to assign_search_params()
17/08/12   ip     CR384421 Disable GSM sleep during CCO activity
16/08/12   zf     CR390228: Featurize Opt 20.
28/06/12   zf     CR318881: Adding previous serving cell to balist when reselection.
25/04/12   pa     CR355881: Removed warning.
29/03/12   cja    CR316437 Inhibit sleep on X2G RAT change until it is complete
28/03/12   pa     CR 346062 Workaround for G2T Abort.
07/03/12   ws     CR 341284 - Ensure Bitmap that stores received MAC message indication
                  is reset to zero when release is received to return to IDLE mode
29/02/12   ws     CR340356  - G2L broken due to sending LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ twice.
06/09/11   cja    CR304637. Only wait for frame tick if request leads to FW enable/disable
31/08/11   cja    CR304401. Wait for frame tick so FW not deactivated when suspended.
03/05/11   cja    Nikel - only send G2L init when L neighbours
14/01/11   ab        CR 258308   : P0 write buffer overflow in func
                     gpl1_gprs_calculate_paging_data()
17/12/10   og     Changed G2L INIT and DEINIT.
14/12/10   og     Extra Featurisation for G2L.
29/11/10   og     Adding G2L support.
01/02/11   dmb    Remove functionality intended to support 'extended measurements'
                  (FEATURE_GPRS_EXT_MEAS) - not required
13/07/10   cs     Updates for supporting CMAS
14/06/10   ap     CR: 235190 - 3GPP: Enhanced control of maximum output power in
                   a common BCCH cell
09/06/09   nf     Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
18/05/09   ap     CR174981 For engineeing mode. Reset timing advance when coming out of
                  transfer,dedicated or dtm modes.
06/05/09   ap     Fix Compile Issue
29/04/09   og     CR180512: Ensure SCE is stopped correctly when processing
                  a DL_TBF_CONFIG.
11/03/09   og     CR173804 - Ignore downlink mac mode for Rel 6.
28/10/08   agv    CR161919 - Fix for UL Transmit Slots not reset at end of
                  E/GPRS TBF for GSM call.
28/07/98   hv     Added KxMutex support
23/05/08   ap     Klocwork fixes
15/05/08   nf     CR117393 fix for NC2 non-drx handling
06/06/07   ws     CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                  is called when we reselect to PBCCH cell stopping NC2 reporting
02/03/07   cs     Add support for new CGPS timetag
27/10/06   ws     Fixed ccch_idle_mode_params.ext_utbf_nodata correctlyfor 51 mode
05/09/06   og     Fix to SR852825 CR100279, prevent new_serving_cell_info
                  pointer access. Remove wrong new_serving_cell_info
                  pointer accesses.
17/08/06   og     Basic featured-off EDTM code drop.
16/08/06   ws     Added Exhanced Extended uplink tbf feature. Don't tx dummy
                  control blocks if RLC fifo is empty
31/07/06   av     CR85810 NB AB TSC, and TA fix.
07/07/06   ws     Added GL1_UNIT_TEST around rex_wait() calls
13/06/06   dv     Conditional test put around fix for CR92771
26/05/06   dv     Fix for CR92771 - not changing ts when ccch_conf value changes
17/03/06   og     Take downlink assignment mac mode into account. This is
                  required by release 4 ETSI spec.
29/09/05   og     Merge to fixes of CR74437 and CR70332 from the mainline branch
                  Fixes inter RAT TC and 42.4.8.4.5 respectively.
29/08/05   sv     Use default WCDMA parameters on reselection.
22/08/05   og     Add the setting of mac mode to NPL1.
04/08/05   og     Merge change 225343 from the raven branch.
19/07/05   nt     CR69217 - Added gprs flag initialisation into mph_start_gprs_idle_req
27/06/05   av     CR66482 - Resolved NON DRX in PIM.
23/06/05   og     Lint error/warnings removal.
20/06/05   ws     Changed include of gMacL1.h to gmacl1.h
02/06/05   og     Removal of header-header includes.
31/05/05   og     Inform the egprs qual meas module of the broadcast bep period
                  directly.
18/05/05   og     Ensure that starting time elapsed is set to FALSE.
17/05/05   og     Remove the forcing of pkt paging mode to normal.
12/05/05   og     Fix to incorrect start time expiry check.
26/04/05   ws     CR 61477 - Adjust PDAS start time if PCA RRBP is due to be sent
20/04/05   og     Addition of filtered bep params to the msg metrics log packet.
01/04/05   ws     Added gl1_drx_require_tick() to wake up DRX when enabling
                  MM_NON_DRX MODE
11/03/05   sv     Mainlined FEATURE_GSM_CB.
11/3/05    og     Changes to egprs dl tfi and IR handling.
13/01/05   ws     Copy access burst type from gprs_l1_idle_data when camping on
                  a PBCCH cell,since PTCCH/U handler fetches access burst type
                  form l1_idle_data structure
13/1/05    og     MS timebase adjustements confined to radio block gap.
09/12/04   og     Fix to tx power control by tracking c value on ccch.
30/11/04   og     Enable interference measurements when cuta is ON.
17/11/04   og     Fix to ul tbf release confirm being sent.
11/11/04   og     Copy in BCCH/PBCCH bep_period into L1 CCCH/PCCCH data structures.
2/11/04    og     Addition of egprs srb test mode capability.
01/11/04   dlh    Added gprs_l1_idle_data.prog = TRUE to stop_acquire message.
21/10/04   dlh    Corrected text formatting.
18/10/04   dlh    Ensure all RRBP flags are cleared before entering idle.
18/10/04   dlh    Modified reselection so that there will be no delay going to 51 idle.
18/10/04   dlh    Removed the change to L1_GPRS_IDLE_INIT sub_sub_state from 52 idle
18/10/04   dlh    Added clear all rrbp flags into setup start_idle func.
19/10/04   og     Added single block bep period2 handling.
12/10/04   ws     Added Multiblock allocation for EGPRS
07/10/04   dlh    Added the power params update to L1_update_params.
30/09/04   ws     Corrected setting of PRACH TSC for GPRS access mode to zero
22/09/04   kf     Added case for dl release.
15/09/04   ws     Corrected setting of PRACH TSC for GPRS access mode
03/09/04   ws     Added temp setting of e and bep_period
09/08/04   sv     Corrected the interface to handle "blind" PCCO case.
03/08/04   sv     Changed interface to send "RESELECTION_CNF" instead of "SPECIFIC_BCCH_CNF"
                  during full acquisition on a GPRS reselection.
20/07/04   kf     Reinstated full fch/sch decode when sync flag set in
                  MPH_GPRS_RESELECTION_REQ.
06/12/04   gfr    Control slew rate limiting in PL1.
21/06/04   ws     Modified min reaction time for MAC_L1_DL_TBF_CONFIG to 4
11/06/04   dp     Changes to L1_setup_RESELECTION to use BSIC and not BCC.
                  Also set pAgc_data tnd l1_idle_data.campedon_agc struct to
                  point to AGC struct of cell we're reselecting to
08/06/04   kf     setting of l1_isrtsk_blk when pccch camped.
26/05/04   sv     Changed "l1_isrtsk_blk" structure parameters for better
                  code readability.
23/06/04   og     Egprs Qual measurements handling.
13/05/04   rm     Removed merge error
27/04/04   rm     Replaced TA and RRBp functionalities with new function call
                  to PTCCH and RRBp modules.
23/04/04   ws     Corrected Lint Error's and Warnings
12/04/04   gw     Do SCH decode rather than full combined acquisition
                  when sync flag is set in MPH_GPRS_RESELECTION_REQ.
31/03/04   gw     Process CB channel information in MPH_START_GPRS_IDLE_REQ.
30/03/04   ws     Added clearing of PACCH ul buffer when leaving transfer
03/26/04   gfr    Time tagging support.
23/03/04   ws     Added tbf_freq_cnf_t handling to MAC_L1_FREQ_CONFIG handler
18/03/04   ws     Added support to cancel non drx timer if a surround update
                  is requested
11/03/04   ws     Ensure ta param pointer is null to stop cont. TA params
                  swapping to UL
09/03/04   ws     Added abort_receive setting for stopping rx's after final
                  PCA is txd
27/01/04   kf     setting of band for ul power control
06/01/04   dlh    Added handling for MAC_L1_RELEASE in 52 Idle
09/12/03   gw     Changes to support new SI reading API.
26/11/03   ws     Lint cleanup
25/11/03   dmb    Inclusion of page mode in CCCH51 start idle parameters
24/11/03   ws     Added starting_time_valid flag to cases where starting time
                  has expired
06/11/03   gfr    Support for quad-band.
14/10/03   npr    Changes to handling of alpha/gamma in MAC_L1_DL_TBF_CONFIG
07/09/03   dlh    Removed the rrbp_flag as no longer needed.
26/09/03   dlh    Modified Acquire psi message and sleep mode bug fix.
25/09/03   ws     set power for PRACH to max_tx_pwr_ccch
23/09/03   kf     wcdma changes Workaround for Aglent 8960 removed.
03/09/03   dlh    Modified where INTFREE_SAV was for 51 Idle.
29/08/03   dlh    Removed superfluous code for optimised sleep timeline.
12/08/03   ws     Added clearing of pending dl and pending dynamic tbf for new ass's
11/08/03   ws     Added missing gl1_drx_require_tick() in MAC_L1_DL_TBF_CONFIG
07/08/03   kf     lint changes.
06/08/03   dlh    Removed superfluous gl1_drx_require_next_tick call.
28/07/03   ws     Modified update to elapsed starting time for dl single block
28/07/03   kf     removed function gpl1_re_start_surround_cell_52_idle,
                  and gpl1_re_start_surround_cell
28/07/03   kf     added set up of parameters in pbcch sync.
23/07/03   dlh    Added rrbp_52_idle variable RRPB for 52 Idle.
22/07/03   dlh    Removed F3 from Random Access message handler.
22/07/03   dlh    Modified INTLOCK/FREE for 52 Idle.
21/07/03   dlh    Tidy up of F3's and sleep mode 52 additions.
17/07/03   ws     Added checking in MAC_L1_DL_TBF_CONFIG to ensure we are in IDLE_MODE
                  before setting configuring DL_TBF
16/07/03   kf     modified ncell pbcch reading.
14/07/03   gw     Updated comments.
11/07/03   gw     Changes to speed up reselection.
10/09/03   bk     Changed the prototype for gpl1_gprs_setup_start_idle() so that it
                  can be called from the task context. Moved l1_sc_start out of
                  sections that had locked interrupts.
10/07/03   ws     Modified structure of DL_TBF_CONFIG and DYNAMIC_TBF_CONFIG
                  to ensure min starting time is actioned correctly
08/07/03   kf     changed surround update req
08/07/03   gw     GRR/L1 SCE cleanup.
04/07/03   ws     Modified handling of SAME_AS_BEFORE page mode if no page mode
                  has been ordered via MAC
03/07/03   ws     Changed min reaction offset for DL assignment from 8 to 4 frames
26/06/03   ag     Removed call to l1_sc_start() from
                  gpl1_re_start_surround_cell_52_idle() as SCE already running.
25/06/03   gw     Added function gpl1_gprs_setup_start_idle().
19/06/03   pjr    Added gprs_serv_cell_meas_buf[] for GPRS serving cell
                  running average
19/06/03   gw     Changes to starting and stopping of SCE.
18/06/03   pjr    Added call to function gpl1_gprs_calc_rf_pwr() to update
                  uplink power control whenever the power control parameters
                  are updated.
13/06/03   pjr    Changed main command setting to L1_PBCCH_RESELECTION in
                  MPH_GPRS_RESELECTION_REQ.
11/06/03   gw     Removed unneeded calls to gpl1_gprs_update_bcch_stop().
10/06/03   pjr    Added functions gpl1_gprs_L1_params_pending and
                  gpl1_gprs_update_L1_params,
                  Added initalisation of the serving cell parameter
                  quality_report_time_index and the transfer data paramerters
                  dl_st and ul_st.
04/06/03   sw     Added Agilent 8960 page mode workaround (defaulted to off)
30/05/03   ws     Set called_from_sngl_blk to FALSE and removed compiler
                  warnings
29/05/03   kf     ncell pbcch reading changes
23/05/03   ws     Added start idle req to return to 51 idle
20/05/03   ws     Modifications to use correct starting time calculation
14/05/03   dlh    Added handling for changing TN on the fly.
12/05/03   dp     Removed call to l1_sc_balist_gprs_set_sync from
                  gpl1_gprs_re_start_surround_cell.  Was put in earlier
                  to handle transferring the Ncell BSIC
08/05/03   kf     Changed ba list copying.
06/05/03   ws     Added wait for DRX to wake up in DL_SINGLE_CONFIG
02/05/03   dp     Changes to preserve Ncell BSIC across transitions
02/05/03   gw     Merged in power transfer changes from branch.
02/05/03   ws     Added MPH_PBCCH_DESCRIPTION_IND
01/05/03   ag     Updates to ncell pbcch reading.
01/05/03   kf     Fix for reselection 51 idle/52 transfer.
28/04/03   gw     Changes for preserving power measure info.
24/04/03   ws     preset pending_alloc_type to L1_NULL_ALLOC
02/04/03   gw     Changes to reflect modifications in monscan module.
                  Replaced tabs with spaces.
17/04/03   dlh    Removed the change into L1_IDLE_WAIT sub_sub_state
10/04/03   dlh    Added storage for RACH params into 51 Idle struct for
                  MPH_Start_Idle_Req in 52 mode.
26/03/03   kf     Changed reselection request handling with sync flag.
20/03/03   pjr    Changed l1_sc_start( L1SCModeGprsTrans ) for l1_sc_stop() in
                  RANDOM ACCESS REQUEST. Also took out setting of sub_sub_state
                  to L1_GPRS_IDLE_WAIT in MPH_STOP_PSI_ACQUIRE_REQ
17/03/03   kf     Removed change of page mode when leaving transfer.
06/03/03   kf     Added functionality for pbcch reselection
28/02/03   kf     Changes for ncell pbcch reading and ba list transfer 52 idle
21/02/03   dlh    Slight modes to F3 messages.
21/02/03   ws     Added code to wake up DRX during RRBP handling
20/02/03   pjr    Added mods to handle reception of RRBP in IDLE for transmission
                  in transfer
14/02/03   dlh    Mods to gprs page mode and mm_non_drx functionality.
14/02/03   pjr    Updated actions when MAC_PH_RANDOM_ACCESS_REQ message received.
                  Modifed sleep functionality.
13/02/03   ws     Updates for downlink power control
12/02/03   ws     Added code to wait for drx manager to wakeup before reading FN
06/02/03   ws     Changed min reaction time for DL_TBF_CONFIG from 5 to 8 frames
05/02/03   ws     Added PCA_UL_FAI to RRBP ul msg count
23/01/03   KF     Modified mph_gprs_l1param_update_req.
21/01/03   dlh    Modification to PSI aquisition.
10/01/03   gw     Changed some SCE function calls to reflect renaming of some
                  BA list functions done as part of SCE clean up.
07/01/03   ws     Added sending of reselection_cnf for GPRS and setting of the
                  multiband reporting parameter
19/05/02   pjr    In MAC_L1_DL_TBF_CONFIG, moved power control parameters to
                  serving cell structure.
12/05/02   gw     Changed reference to read_xtnd_BCCH in l1_idle_data struct to
                  xtnd_BCCH_mask.
12/05/02   ws     Enabled l1_sc_stop() in DL TBF and SINGLE DL TBF as SCe is now defined
12/05/02   pjr    Replaced some of the specific copies removed in previous change.
12/02/02   pjr    Added layer1_extra_gprs_sys_info copy in START_GPRS_IDLE_REQ
                  and removed unwanted specific copies.
12/02/02   ws     Added handling of MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
11/25/02   pjr    Added feature switch, FEATURE_GSM_GPRS_PCCCH, around PRACH
                  functionality.
11/21/02   ws     Added reseting of RLC read buffer when moving into transfer mode
11/20/02   dlh    Added MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
11/19/02   ws     Ammended starting time for CCCH_51 to move to TRANSFER ASAP
                  if starting time has elapsed
11/14/02   ws     Added setting of starting time for GPRS 51 Idle mode
04/11/02   pjr    Merge Neighbour cell measurements under feature switch
01/11/02   pjr    Added serving cell measurements under feature switch
10/21/02   he     Replaced FN by gl1_get_FN().
16/10/02   pjr    Changed start time for no supplied start time to FN+4
09/10/02   ws     Added Single_dl_block handling and set receive_all_paging_blocks to TRUE
10/08/02   dlh    Added 51 GPRS params local storage to GPRS_Idle_if.
27/08/02   dlh    Added init for gprs_current_paging_mode/gprs_stored_paging_mode
                  DRX_timer.
09/18/02   ws     Moved setting of main_command to end of MAC_L1_DL_TBF_CONFIG and
                  MAC_L1_FREQ_CONFIG
09/18/02   ws     Added check in RRBP handler to adjust rrbp fn if it were received
                  on the immediate assignment
09/11/02   pjr    Changed to PDAN_WITH_FAI and PDAN_WITH_CRD.
08/27/02   dlh    Added init for gprs_current_paging_mode/gprs_stored_paging_mode
DRX_timer
08/19/02   pjr    Corrected more Lint errors.
08/08/02   ws     Changed 51 handling of START_GPRS_IDLE_REQ as per GN1004
08/08/02          dlh,ws Corrected Lint errors
31/07/02   dlh    Added Extended Mode handling in page mode case.
31/07/02   ws     Added RRBP handler to idle task handler
24/07/02   pjr    Added setting of prach sub_state
07/19/02   pjr    Removed gl1_msg_store_ta from MAC_PH_RANDOM_ACCESS_REQ signal
15/07/02   ws     changed timing advance handing for MAC_L1_DL_TBF_CONFIG
                  to allow dl tbf cont. ta parameters to be stored correctly
02/07/02   ws     Added tbf_est_type to allow MAC_PH_CONNECT_IND to be setup
                  correctly
06/27/02   ws     added support of gpl1_gprs_get_starting_time()
05/28/02   pjr    Read correct system information for Tn and tsc instead of
06/18/02   pjr    Modified MAC_PH_RANDOM_ACCESS_REQ handler to reset timing
                  advance, setup prach_config_once flag and make setting of
                  main_command the last action.
05/28/02   pjr    Read correct system information for Tn and TSC instead of
                  'hard coding'.
05/24/02   pjr    Added debug message to MAC_PH_RANDOM_ACCESS_REQ, and general
                  tidy.
05/23/02   ws     Added Global TA pointer for concurrent TBF work
05/23/02   ws     Put re-entered changes lost due to archive overwrite
05/23/02   pjr    Put re-entered changes lost due to archive overwrite
05/16/02   dlh    Added gpl1_gprs_calculate_paging_data/L1_calculate_paging_data to MPH_GPRS_L1PARAM_UPDATE_REQ
05/16/02   dlh    Added L1_calculate_paging_data() to 51 section of MPH_START_GPRS_IDLE_MODE_REQ
05/16/02   dlh    Added INTLOCK/INTFREE calls
02/22/02   pjr    Added handling of prach_ind and arfcn to
                  MAC_PH_RANDOM_ACCESS_REQ, plus general tidy.
02/11/02   dlh    MAC page mode handler
01/17/02   dlh    GPRS CCCH 51 PCCCH 52 IDLE mode RR-L1 messages handlers
12/14/01   pjr    Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


/* Alphabetical Order */
#include "comdef.h"
#include "memory.h"
#include "nas_exp.h"
#include "l1_task.h"
#include "task.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include <stdlib.h>
#include "rex.h"

#include "geran_tasks.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_task.h"
#include "gmacl1.h"
#include "l1_utils.h"
#include "l1_isr.h"
#include "rr_public_store.h"
#include "gpl1_gprs_isr.h"
#include "msg.h"
#include "ms.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "l1_drx.h"
#include "l1_log.h"
#include "l1_fm.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"

#ifdef PL1_FW_SIM
#include "pl1_test_harness.h"
#endif

#include "l1_smscb.h"
#include "cbl1if.h"
#include "l1_utils.h" /* for gl1_get_FN() */
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "gl1_hw_g.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "l1i.h"
#endif

#include "geran_eng_mode_info.h"

#include "gpl1_dual_sim.h"
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "l1_sc_irat.h"
#include "gpl1_grm_intf.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

/*========================== #defines =====================================*/
/* Used to set the UL back to 1 TS for GSM. */
#define DEFAULT_ONE_TS    1

/*========================== global function declarations ==================*/
extern void gpl1_gprs_setup_RESELECTION_GPRS( IMH_T *msg_header );


/*======================== global Data =====================================*/
extern gpl1_gprs_rrbp_info_T l1_get_gpl1_gprs_rrbp_info ( gas_id_t gas_id );
extern void l1_init_gpl1_gprs_rrbp_data(gas_id_t gas_id);

#ifdef FEATURE_GSM_TDS
extern boolean l1_sc_irat_performing_acq_meas(gas_id_t gas_id);
#endif

extern dedicated_data_T l1_dedicated_data[];

extern boolean gpl1_client_in_tuneaway_gap(volatile ISRTIM_CMD_BLK *l1_tskisr_blk,
                                           gas_id_t gas_id);

gprs_l1_idle_data_t gprs_l1_idle_data_store[NUM_GERAN_DATA_SPACES];
prach_params_T      prach_params[NUM_GERAN_DATA_SPACES];


 gpl1_gprs_meas_mode_T gpl1_gprs_meas_mode[NUM_GERAN_DATA_SPACES];

/*========================== Local types/enums ============================*/
typedef struct
{
 gprs_l1_idle_data_t gprs_l1_update_pending;
 idle_data_T         l1_update_pending;
// uint16 copy_gprs_multiframe[16][5];
/* target cell for reselection requests */
 l1_sc_bs_T*           new_serving_cell_info;
 boolean pdas_pending_awake_flag ;
 /*  Set up local static to determine if prach callbacks
     have been setup */
 boolean prach_callbacks_setup;
}gpl1_gprs_idle_if_data_t;
/*========================== Local function declarations ==================*/
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
gpl1_gprs_idle_if_data_t * gl1_ms_switch_gprs_idle_if_data(gas_id_t gas_id);

#endif
/* Used to set the UL back to 1 TS for GSM. */
#define DEFAULT_ONE_TS    1

extern void  gsm_l1_set_ecio_rssi_levels( uint16   qsearch,
                                          uint16   ecode,
                                          gas_id_t gas_id
                                        );


/*======================== Local Data =====================================*/
static gpl1_gprs_idle_if_data_t gpl1_gprs_idle_if_data[NUM_GERAN_DATA_SPACES];
/*===========================================================================

    DEFINITIONS AND DECLARATIONS FOR MODULE

    This section contains definitions for constants, macros, types, variables
    and other items needed by this module.

===========================================================================*/


/*===========================================================================
FUNCTION gl1_ms_switch

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
gpl1_gprs_idle_if_data_t* gl1_ms_switch_gprs_idle_if_data(gas_id_t gas_id)
{
  return ( &gpl1_gprs_idle_if_data[gas_id] );
}
/*===========================================================================
FUNCTION gl1_ms_switch

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
prach_params_T* gl1_ms_switch_prach_params_idle_if_data(gas_id_t gas_id)
{
  return ( &prach_params[gas_id] );
}
/*===========================================================================
FUNCTION gl1_ms_switch

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
gprs_l1_idle_data_t* gl1_ms_switch_gprs_idle_data(gas_id_t gas_id)
{
  return ( &gprs_l1_idle_data_store[gas_id] );
}
/*===========================================================================
FUNCTION gl1_ms_switch

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
gpl1_gprs_meas_mode_T* gl1_ms_switch_meas_mode_data(gas_id_t gas_id)
{
  return ( &gpl1_gprs_meas_mode[gas_id] );
}

/*===========================================================================

    FUNCTION  gpl1_gprs_handle_grr_idle_mode

    DESCRIPTION
    This function handles the GPRS functionality of the L1 Task in idle mode.

    DEPENDENCIES
    None

    RETURN VALUE
    None

    SIDE EFFECTS
    None

===========================================================================*/
void gpl1_gprs_handle_grr_idle_mode(IMH_T *msg_header)
{
  gas_id_t gas_id;
  /* Locals */
inter_task_BCCH_allocation_T  ba_buffer;

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
  *                MPH_START_GPRS_IDLE_REQ               *
  *                                                      *
  ********************************************************/

  case MPH_START_GPRS_IDLE_MODE_REQ:
    {

      mph_start_gprs_idle_mode_req_t *message_ptr;
      gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
      uint8 i;

      message_ptr = (mph_start_gprs_idle_mode_req_t *)msg_header;

      gas_id = check_gas_id(message_ptr->gas_id);
      gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

      gl1_handle_mph_start_gprs_idle_mode_req(message_ptr);

      /*  Initialise the tx power array to maximum allowable power  */
      for (i=0; i<8; i++)
      {
        gprs_serv_cell_meas_ptr->pch[i] = gprs_serv_cell_meas_ptr->pmax;
      }

      gprs_serv_cell_meas_ptr->quality_report_time_index = 0;

      l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
      l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;


      /*  Initialise the running average structure  */
      /*  Initialize serving cell measurement averaging filter */
      gprs_serv_cell_meas_ptr->rla_p_count_idle = GPRS_SC_RLA_P_COUNT_IDLE;

      l1_running_avg_init(&gprs_serv_cell_meas_ptr->rla_p_buffer,
                          gpl1_gprs_get_serv_cell_meas_buf(PRX_MEAS_BUFFER, gas_id),
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                          gpl1_gprs_get_serv_cell_meas_buf(DIVRX_MEAS_BUFFER, gas_id),
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
                          (L1_GPRS_SERV_CELL_MEAS_BUF_LEN - 1),
                          gas_id);

      /*  Set for Idle 52 mode measurements */
      l1_running_avg_set_avg_len(&gprs_serv_cell_meas_ptr->rla_p_buffer, GPRS_SERV_CELL_IDLE_52_AVG_LEN,gas_id);

      l1_idle_if_gps_notify(gas_id);

    }
    break;

  /********************************************************
  *                                                      *
  *                MPH_GPRS_L1_PARAM_UPDATE_REQ          *
  *                                                      *
  ********************************************************/


  case MPH_GPRS_L1PARAM_UPDATE_REQ:
    {
      /*  Store the received parameters into a pending store. */
      gpl1_gprs_L1_params_pending(msg_header);
      MSG_GERAN_MED_0("MPH_GPRS_L1PARAM_UPDATE_REQ");
    }

    break;


    /********************************************************
    *                                                       *
    *                MPH_NC_MEASUREMENT_REQ                 *
    *                                                       *
    ********************************************************/
  case MPH_NC_MEASUREMENTS_REQ:
    {
      mph_nc_measurements_req_t *message_ptr;
      gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;
      idle_data_T               *l1_idle_data;

      message_ptr = ( mph_nc_measurements_req_t *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);
      gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];
      GL1_ISR_SAVE_LOCK(gas_id);
      L1_calculate_paging_data(gas_id);
      GL1_ISR_SAVE_UNLOCK(gas_id);

      gpl1_gprs_meas_mode_ptr->nc_mode=TRUE;
      L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);

      /* There was a need to speed up SCE for GCF 3.18.  The MS needs to */
      /* react to new  BA  List cells for PMO NC2 measurements of 7 sec. */
      /* It was taking about 2 sec for the new list to be used, and with */
      /* a further 5 sec to monitor the new cell leaving no time to sync */
      /* and then send a complete  NC_MEAS_IND to RR failing 42.4.8.4.5. */
      gpl1_gprs_meas_mode_ptr->nc_new_ba_list_pending = TRUE;

      l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);
      /*tell surrounding cell engine gap between paging blocks*/
      l1_sc_set_paging_multiframes(l1_idle_data->
        campedon_cell_data.BS_PA_MFRMS,gas_id);

      MSG_GERAN_HIGH_0_G("MPH_NC_MEASUREMENTS_REQ");


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

      gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
      gpl1_gprs_meas_mode_ptr->nc_mode=FALSE;
      L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);
      l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);
      MSG_GERAN_HIGH_0_G("MPH_STOP_NC_MEASUREMENTS_REQ");

      break;
    }

    /********************************************************
    *                                                       *
    *                MPH_GPRS_SURROUND_UPDATE_REQ           *
    *                                                       *
    ********************************************************/
  case MPH_GPRS_SURROUND_UPDATE_REQ:
    {
      mph_gprs_surround_update_req_t *message_ptr;
      idle_data_T               *l1_idle_data;

      message_ptr = (mph_gprs_surround_update_req_t *)msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];

      l1_idle_data->cancel_non_drx = message_ptr->cancel_non_drx;

      {
        gpl1_gprs_update_bcch(
          message_ptr->gprs_surround_data.bcch_params.bcch_arfcn,
          message_ptr->gprs_surround_data.bcch_params.si_requested,
          message_ptr->gprs_surround_data.bcch_params.si13_normal_or_ext, gas_id );
      }
      break;
    }

    /********************************************************
    *                                                       *
    *                MPH_STOP_SURROUND_UPDATE_REQ           *
    *                                                       *
    ********************************************************/

  case MPH_STOP_SURROUND_UPDATE_REQ:
    {
      mph_stop_surround_update_req_t* message_ptr = ( mph_stop_surround_update_req_t* )msg_header;

      gas_id = check_gas_id(message_ptr->gas_id);
      /* This function stop sboth bcch and pbcch updates. */
      gpl1_gprs_update_bcch_stop( FALSE, gas_id );
      break;
    }

    /********************************************************
    *                                                       *
    *                MPH__GPRS_RESELECTION_REQ              *
    *                                                       *
    ********************************************************/

  case MPH_GPRS_RESELECTION_REQ:
    {
      mph_gprs_reselection_req_t* message_ptr = ( mph_gprs_reselection_req_t* )msg_header;

      gas_id = check_gas_id(message_ptr->gas_id);

      {/* check for any RRBP's to go out before reselecting */
        /* if L1 is in 51 mode reselect straight away */
        if((l1_get_gpl1_gprs_rrbp_info(gas_id).num_rrbp_msgs == 0) )
        {
          MSG_GERAN_MED_0("(1)51 DO_RESELECTION_GPRS");
          gpl1_gprs_do_RESELECTION_GPRS( msg_header );
        }/* RRBP is scheduled... delay reselection until after RRBP tx */
      }

    /* Use default parameters for WCDMA scheduling when reselecting to a new cell
     * default params => 15 (never schedule measurements)
     *                => 7  (Ec\No threshold for UTRAN reselection, -12dB)
     * These parameters will be later updated by RR through the
     * MPH_WCDMA_CELL_UPDATE_LIST_REQ after reading the SI2quater info
     */
      /*gsm_l1_set_ecio_rssi_levels(15,7,gas_id);*/
      l1_sc_gsm_l1_set_ecio_rssi_levels(15,15,7,gas_id);
      break;
    }

    /********************************************************
    *                                                      *
    *                MPH_START_MM_NON_DRX_IND:             *
    *                                                      *
    ********************************************************/
  case MPH_START_MM_NON_DRX_IND:
    {
      mph_start_mm_non_drx_ind_t* message_ptr = ( mph_start_mm_non_drx_ind_t* )msg_header;
      idle_data_T               *l1_idle_data;

      gas_id = check_gas_id(message_ptr->gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];
      MSG_GERAN_HIGH_1_G("MPH_START_MM_NON_DRX_IND %d",message_ptr->non_updated_lai_in_nmo1);
      /* Enable the MM non-drx mode period */
      l1_idle_data->mm_non_drx_mode = TRUE;
      l1_idle_data->non_updated_lai_in_nmo1 = message_ptr->non_updated_lai_in_nmo1;
      gl1_drx_require_next_tick(gas_id);
  
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_START_IND);
#endif
      break;
    }

    /********************************************************
    *                                                      *
    *                MPH_STOP_MM_NON_DRX_IND:              *
    *                                                      *
    ********************************************************/
  case MPH_STOP_MM_NON_DRX_IND:
    {
      mph_stop_mm_non_drx_ind_t* message_ptr = ( mph_stop_mm_non_drx_ind_t* )msg_header;
      idle_data_T               *l1_idle_data;

      gas_id = check_gas_id(message_ptr->gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];
      MSG_GERAN_MED_0_G("MPH_STOP_MM_NON_DRX_IND");
      /* Disable the MM non-drx mode period */
      l1_idle_data->mm_non_drx_mode = FALSE;
      l1_idle_data->non_updated_lai_in_nmo1 = FALSE;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_STOP_IND);
#endif

      break;
    }

    /********************************************************
    *                                                      *
    *                MPH_START_IDLE_REQ                    *
    *                                                      *
    ********************************************************/
  case MPH_START_IDLE_REQ:
    {
      mph_start_idle_req_T   *message_ptr= (mph_start_idle_req_T *)msg_header;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

      gas_id = check_gas_id(message_ptr->gas_id);
      l1_tskisr_blk = &l1_tsk_buffer[gas_id];


      L1_setup_START_IDLE( msg_header );

      /* Change the ISR mode */
      l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

      (void)l1_log_paging_data(gas_id);

      MSG_GERAN_HIGH_0_G("L1: Move to 51 GSM Idle");

      break;
    }

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

    /* @@OG: EDTM design */
    /*******************************************************
    *                                                      *
    *                MPH_IMMED_ASSIGN_REQ                  *
    *                                                      *
    ********************************************************/

    case MPH_IMMED_ASSIGN_REQ:
    {
      mph_immed_assign_req_T  *message_ptr;
      idle_data_T               *l1_idle_data;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
      dedicated_data_T *l1_dedicated_data_ptr;
      byte                    temp;

      message_ptr = ( mph_immed_assign_req_T *)msg_header; /*lint !e740 unusual pointer cast */
      gas_id = check_gas_id(message_ptr->gas_id);

      l1_idle_data=&l1_idle_data_store[gas_id];
      l1_tskisr_blk = &l1_tsk_buffer[gas_id];
      l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

      /* give the L1 ISR some previous history */
      l1_dedicated_data_ptr->previous_chnl_type = SDCCH;
      /* Load up the data  from the message */
      l1_dedicated_data_ptr->channel_specification.DTX_indicator = message_ptr->DTX_indicator;

      temp = message_ptr->timing_advance;
      l1_dedicated_data_ptr->timing_advance_in_use = temp;
      l1_dedicated_data_ptr->timing_advance_to_use = temp;
      l1_dedicated_data_ptr->ordered_timing_advance = temp;
      gl1_msg_store_ta( temp,gas_id );


      l1_dedicated_data_ptr->channel_specification.starting_time_valid = message_ptr->starting_time_valid;

      temp = calculate_init_txpwr_max_dedicated(gas_id);

      l1_dedicated_data_ptr->ms_power_level_in_use = temp;
      l1_dedicated_data_ptr->ms_power_level_to_use = temp;
      l1_dedicated_data_ptr->ordered_ms_power_level = temp;

      /* Transfer the AGC information from IDLE to DEDICATED */
      l1_dedicated_data_ptr->c0_agc = l1_idle_data->campedon_cell_agc;
      sys_algo_agc_enable_slew_rate_limiting(&l1_dedicated_data_ptr->c0_agc);

      l1_dedicated_data_ptr->channel_specification.starting_time[0] = message_ptr->starting_time[0];
      l1_dedicated_data_ptr->channel_specification.starting_time[1] = message_ptr->starting_time[1];
      l1_dedicated_data_ptr->starting_time = (message_ptr->starting_time[0]*256)+
                                        (message_ptr->starting_time[1]%256);
      l1_dedicated_data_ptr->freq_redef_imminent = FALSE;
      l1_dedicated_data_ptr->channel_specification.before_parameters_valid =
        message_ptr->before_parameters_valid;
      l1_dedicated_data_ptr->channel_specification.channel_info_1_before = message_ptr->channel_info_before;
      l1_dedicated_data_ptr->channel_specification.channel_info_1_after = message_ptr->channel_info_after;
      /* Set default Value */
      l1_dedicated_data_ptr->channel_specification.channel_mode_1 = SIGNALLING_ONLY;
      l1_dedicated_data_ptr->channel_specification.channel_mode_2 = SIGNALLING_ONLY;

      l1_dedicated_data_ptr->L1_confirm_msg = (uint16)MPH_IMMED_ASSIGN_CNF;

      /* Tell the SCE to stop */
      l1_sc_stop(gas_id);
	  
      /* Change the ISR mode */
      GL1_ISR_LOCK(gas_id);

      l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;

      l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;

      GL1_ISR_UNLOCK(gas_id);

      /* Reset the l1 tx data buffers */
      l1_dcch_s0_buffer[gas_id].data_valid = FALSE;
      l1_sacch_s0_buffer[gas_id].data_valid = FALSE;

#ifdef DEBUG
      if (message_ptr->before_parameters_valid) {
        temp = message_ptr->channel_info_before.channel_type;
        if ( temp == SDCCH_4 ) {
          qsprintf(debug_string,
                   "beforechnl=SDCCH/4(%d)\n",
                   message_ptr->channel_info_before.subchannel );
        }

        else if (  temp == SDCCH_8 ) {
          qsprintf(debug_string,
                   "beforechnl=SDCCH/8(%d)\n",
                   message_ptr->channel_info_before.subchannel );
        }

        gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
                              GS_LOG_TYPE_TASK_PROGRESS,
                              debug_string);
      }
      temp = message_ptr->channel_info_after.channel_type;

      if ( temp == SDCCH_4 ) {
        qsprintf(debug_string,
                 "afterchnl=SDCCH/4(%d)\n",
                 message_ptr->channel_info_after.subchannel );
      } else if (  temp == SDCCH_8 ) {
        qsprintf(debug_string,
                 "afterchnl=SDCCH/8(%d)\n",
                 message_ptr->channel_info_after.subchannel );
      }

      gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
                            GS_LOG_TYPE_TASK_PROGRESS,
                            debug_string);
#endif


      /* inform surround cell engine */
      switch ( message_ptr->channel_info_after.channel_type ) {
        case SDCCH_8:
        case SDCCH_4:
          l1_sc_start( L1SCModeDediSDCCh, gas_id );
          break;
        default:
          l1_sc_start( L1SCModeDediTCh, gas_id );
          break;
      }

      L1_remove_oob_ba(&ba_buffer,&l1_idle_data->campedon_cell_data.BA_list);
      l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);
      break;
    }

#endif  /* FEATURE_GSM_EDTM */
#endif  /* FEATURE_GSM_DTM */

    /*******************************************************
    *                                                      *
    *                MPH_SET_PRIORITY_IND                  *
    *                                                      *
    ********************************************************/

    case MPH_SET_PRIORITY_IND:
    {
      /* no specific action required */;
      break;
    }

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    /********************************************************
    *                                                       *
    *           MPH_REDUCED_NCELL_MEAS_IND                  *
    *                                                       *
    ********************************************************/
    case MPH_REDUCED_NCELL_MEAS_IND:
    {
      mph_reduced_ncell_meas_ind_T *message_ptr = (mph_reduced_ncell_meas_ind_T *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);
      l1_sc_config_reduced_ncell_list_rr( message_ptr->reduced_ncell_list, gas_id );
      break;
    }
#endif


  default:
    MSG_GERAN_ERROR_0("PL1: Bad Message recvd!");
    break;

  } /* End of switch */
}

/*===========================================================================

    FUNCTION  gpl1_gprs_handle_mac_idle_mode

    DESCRIPTION
    This function handles the GPRS functionality of the L1 Task in idle mode.

    DEPENDENCIES
    None

    RETURN VALUE
    None

    SIDE EFFECTS
    None

===========================================================================*/
void gpl1_gprs_handle_mac_idle_mode(IMH_T *msg_header)
{
  gas_id_t gas_id;

  /*******************************************************
  *                                                      *
  *               MAC_PH_RANDOM_ACCESS_REQ               *
  *                                                      *
  ********************************************************/
  switch (msg_header->message_id)
  {
    /*******************************************************
    *                                                      *
    *                MAC_L1_DL_TBF_CONFIG                  *
    *                                                      *
    ********************************************************/
  case MAC_L1_DL_TBF_CONFIG:
    {

      mac_l1_dl_tbf_config_t  *message_ptr;
      gprs_l1_idle_data_t *gprs_l1_idle_data;
      idle_data_T               *l1_idle_data;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
      gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
      boolean                 st_elapsed;

      /*lint -e(740)*/
      message_ptr = ( mac_l1_dl_tbf_config_t *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);

      gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];
      l1_tskisr_blk = &l1_tsk_buffer[gas_id];
      gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

      gl1_drx_task_active(gas_id);

#ifndef GL1_UNIT_TEST
      /* Not required for Host test */
      /* wake up layer 1 isr */
      gl1_drx_require_next_tick(gas_id);
      (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
      (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
#endif

      MSG_GERAN_MED_2_G( "MAC_L1_DL_TBF_CONFIG [%d] ts_alloc 0x%X ",
               gl1_get_FN(gas_id),
               message_ptr->ts_allocation);

      /* Setup mode to determine whether this TBF is EGPRS or GPRS */
      l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->tbf_mode;

      /* (1) Used for passing MCS 1- 9 crc failed payload data to RLC ONLY in UNACK mode
      * for RLC ACK mode  MCS 1- 9 crc failed payload data is thrown away
      *
      * (2) Used to Setup Incremental redundancy for Downlink TBF
      * RLC Unack mode = IR OFF
      * RLC ACK mode  = IR ON
      */
      l1_transfer_data[gas_id].dl_tbf_1.rlc_mode = message_ptr->rlc_mode;

      if( FALSE != message_ptr->bep_period2.present )
      {
        l1_transfer_data[gas_id].dl_tbf_1.bep_period2 =
          message_ptr->bep_period2.bep_period2;
      }
      else
      {
        l1_transfer_data[gas_id].dl_tbf_1.bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
      }

      l1_transfer_data[gas_id].dl_tbf_1.link_qual_meas_mode =
        message_ptr->link_qual_meas_mode;

      /* Set the mac mode to invalid in the two
       * available dynamic tbf structures.
       * Ignore the mac mode included in the downlink
       * assignment (Rel 6).
       */
      l1_transfer_data[gas_id].dl_tbf_1.mac_mode      = L1_MAC_MODE_INVALID;
      l1_transfer_data[gas_id].dl_tbf_2.mac_mode      = L1_MAC_MODE_INVALID;

      l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = L1_MAC_MODE_INVALID;
      l1_transfer_data[gas_id].dynamic_tbf_2.mac_mode = L1_MAC_MODE_INVALID;

      /* Make sure these are reset */
      l1_transfer_data[gas_id].dynamic_tbf_1.ul_tfi_st = L1_GPRS_INVALID_FN;
      l1_transfer_data[gas_id].dynamic_tbf_2.ul_tfi_st = L1_GPRS_INVALID_FN;

      /* CR180512 - Ensure the Mutex is locked before SC STOP */
      GL1_ISR_SAVE_LOCK(gas_id);

      /* Stop the SCE */
      l1_sc_stop(gas_id);

      /* Put the SCE in Transfer Mode.  It won't start until
      * l1_sci_go is called from ISR.
      */
      l1_sc_start( L1SCModeGprsTrans, gas_id );

      if (message_ptr->power_control.present)
      {
        uint8 index;
        power_control_gamma_T *pwr_cntrl_ptr;

        /*  Update Alpha value  */
        gprs_serv_cell_meas_ptr->alpha = message_ptr->power_control.alpha;

        /*  Extract gamma_tn values  */
        pwr_cntrl_ptr = &(message_ptr->power_control.gamma_TN0);
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
      l1_transfer_data[gas_id].dl_power_control = message_ptr->downlink_pwr;

      /* Not checking present flag as assuming DL timeslot alloc
      * always present.
      *
      * Change order of timeslot description
      */
      l1_transfer_data[gas_id].dl_tbf_1.nts = gpl1_gprs_reorder_tn(
        &message_ptr->ts_allocation,
        &l1_transfer_data[gas_id].dl_tbf_1.ts_allocation
        );

      gpl1_gprs_add_to_active_dl_ta_params(message_ptr->ta_params,gas_id);

      if (message_ptr->dl_tfi_present)
      {
        l1_transfer_data[gas_id].dl_tbf_1.dl_tfi_present = TRUE;
        l1_transfer_data[gas_id].dl_tbf_1.dl_tfi = message_ptr->dl_tfi;
      }
      else
        l1_transfer_data[gas_id].dl_tbf_1.dl_tfi_present = FALSE; // should already have tfi from UL TBF

      /*  Reset ul_desc pointer so any previous READS discarded. */
      memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
      l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

      /* check we are still in idle mode and have not transitioned
      * to transfer mode via a previous assignment
      */

      if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
      {
        MSG_GERAN_MED_0_G("DL_TBF_CONFIG rcvd in transition to transfer");
        GL1_ISR_SAVE_UNLOCK(gas_id);

        gl1_drx_task_inactive(gas_id);

        return;
      }

      /* check to see if a start time has been specified */
      if (message_ptr->tbf_start_time_valid)
      {
        /*  Determine if starting time present and if it needs adjusting  */
        st_elapsed = gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time,
          message_ptr->tbf_start_time,CCCH_51, gas_id);

        /* check if the starting time was received from a immed. Ass */

        /* if the starting time has elapsed or we haven't enough time to process it */
        if ( (st_elapsed) || ( ((gl1_get_FN(gas_id)+ 4)% FRAMES_IN_HYPERFRAME) >= l1_transfer_data[gas_id].starting_time))
        {
          /* action the starting time in 4 frames */
          MSG_GERAN_MED_1_G("st elapsed [%d]",gl1_get_FN(gas_id));
          l1_transfer_data[gas_id].starting_time_valid = TRUE;
          l1_transfer_data[gas_id].starting_time = ((gl1_get_FN(gas_id) + TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
          gpl1_gprs_chk_blk_bndry(&l1_transfer_data[gas_id].starting_time);
        }
      }
      else
      {
        const uint32  current_fn = gl1_get_FN(gas_id);
        /* Work out the new start time using the ADD_FN macro */
        l1_transfer_data[gas_id].starting_time = ADD_FN( current_fn, 4 );

        /* Make sure the start time is aligned on a radio block
         * boundary.
         * No need to check for expiry here since the start time is
         * made up since the assignment was immediate.
         */
        gpl1_gprs_chk_blk_bndry( &( l1_transfer_data[gas_id].starting_time ) );

        l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
        l1_transfer_data[gas_id].starting_time_valid   = TRUE;

        l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time = l1_transfer_data[gas_id].starting_time;

        MSG_GERAN_HIGH_1_G("start time %d", l1_transfer_data[gas_id].starting_time);

      } /* End of if start time is invalid */

      /* If we are doing class B GPRS inform the idle isr of the new starting time */
      gl1_drx_require_next_tick(gas_id);
      l1_idle_data->starting_time = l1_transfer_data[gas_id].starting_time;
      l1_idle_data->starting_time_valid = TRUE;

      gprs_l1_idle_data->start_time_valid = FALSE;

      /* Mark mac_l1_dl_tbf_config received */
      SET_DOWNLINK_TBF_RECV(gas_id);

      /* Check if transition to TRANSFER mode is possible */
      if (DOWNLINK_TBF_ALLOC_COMP(gas_id))
      {
        /* If all signals have been received to fully
        * describe a DOWNLINK allocation then
        * move to L1_TRANSFER state and service it
        */

        /* setup tbf establish type  */
        l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

        l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
        l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
        l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
        l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
        l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;
        RESET_BITMAP(gas_id);

        l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
        l1_tskisr_blk->command = L1_NULL_COMMAND;
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
        gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/
      }
      gl1_drx_task_inactive(gas_id);
      GL1_ISR_SAVE_UNLOCK(gas_id);

      break;
    }

    /*******************************************************
    *                                                      *
    *                MAC_L1_FREQ_CONFIG                    *
    *                                                      *
    ********************************************************/

  case  MAC_L1_FREQ_CONFIG:
    {
      mac_l1_freq_config_t  *message_ptr;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
      boolean error;
      tbf_freq_cnf_t tbf_type;

      /*lint -e(740)*/
      message_ptr = ( mac_l1_freq_config_t *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);

      l1_tskisr_blk = &l1_tsk_buffer[gas_id];
      MSG_GERAN_MED_0_G("PL1: MAC_L1_FREQ_CONFIG Rcvd");
      /*  check for un supported frequencies in ma list that are not in the
      supported bands*/
      error = gpl1_gprs_check_frequency_params(
        &(message_ptr->frequency_information.frequency_list),
        message_ptr->frequency_information.hopping_flag, gas_id
        );

      /* read out tbf type from msg */
      tbf_type = (tbf_freq_cnf_t) message_ptr->tbf_freq_cnf;

      /* send signal to mac to confirm or fail frequency params */
      L1_send_l1_mac_freq_ass_conf(error,tbf_type, gas_id);

      /* if no error then continue */
      if(error != TRUE)
      {

        l1_transfer_data[gas_id].frequency_info_1 = message_ptr->frequency_information;

        /* Mark MAC_L1_FREQ_CONFIG as received */
        SET_FREQ_CONFIG_RECV(gas_id);


        if (DOWNLINK_TBF_ALLOC_COMP(gas_id))
        {
          /* If all signals have been received to fully
          * describe a Downlink allocation then
          * move to L1_TRANSFER state and service it
          */
          /* Stop the SCE and prepare it to start in transfer mode.
          * It won't start until l1_sci_go is
          * called from the ISR.
          */
          l1_sc_stop(gas_id);
          l1_sc_start( L1SCModeGprsTrans, gas_id );
          l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
          l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
          l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;
          RESET_BITMAP(gas_id);

          l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];// BK HACK
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/
        }

      } /* End of if error */

      break;
    } /* End of case */

    /********************************************************
    *                                                       *
    *                MAC_L1_DL_SINGLE_BLOCK                 *
    *                                                       *
    ********************************************************/
  case MAC_L1_DL_SINGLE_BLOCK:
    {
      mac_l1_dl_single_block_t *message_ptr;
      gprs_l1_idle_data_t *gprs_l1_idle_data;
      idle_data_T               *l1_idle_data;
      boolean starting_time_elapsed;
      message_ptr = ( mac_l1_dl_single_block_t *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);

#ifndef GL1_UNIT_TEST
      /* Not required for Host test */
      /* wake up layer 1 isr */
      gl1_drx_require_next_tick(gas_id);
      (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
      (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
#endif


      /*lint -e(740) ccww legacy code */
      message_ptr = ( mac_l1_dl_single_block_t *) msg_header;
      gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
      l1_idle_data=&l1_idle_data_store[gas_id];

      MSG_GERAN_MED_1_G("PL1: MAC_L1_DL_SINGLE_BLOCK Rcvd [%d]",gl1_get_FN(gas_id));


      /* We can only be a GPRS TBF here so setup mode to GPRS */
      l1_transfer_data[gas_id].egprs_or_gprs_tbf = TBF_MODE_GPRS;

      /* Set Parameter to invalid since it is not available in
      * the message
      */
      l1_transfer_data[gas_id].single_block_tbf_1.bep_period2 =
        GPRS_PL1_BEP_PERIOD2_INVALID;

      l1_transfer_data[gas_id].single_block_tbf_1.two_phase_access = 0;
      l1_transfer_data[gas_id].single_block_tbf_1.sb_ul_dl_type = SINGLE_DL_BLOCK;
      l1_transfer_data[gas_id].single_block_tbf_1.ts_number = message_ptr->tn;

      /*  Reset ul_desc pointer so any previous READS discarded. */
      memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
      l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

      /*  Starting time, also perform some checks */

      /*  Now check if the starting time is the first frame of a block, if
      not move to next block boundary. Function will return modified
      starting time, If starting time has elapsed function will return
      TRUE.
      */
      starting_time_elapsed = gpl1_gprs_get_starting_time(NULL,
        message_ptr->tbf_starting_time,CCCH_51, gas_id);

      MSG_GERAN_MED_2_G(" st %d fn %d",l1_transfer_data[gas_id].starting_time,gl1_get_FN( gas_id ));

      /* make sure we have enough time to action the stating time */
      if(((l1_transfer_data[gas_id].starting_time - (gl1_get_FN( gas_id )) < 4)) && (!starting_time_elapsed))
      {
        starting_time_elapsed = TRUE;
      }

      /* If we are doing class B GPRS inform the idle isr of the new starting time */

      if(!starting_time_elapsed)
      {

        l1_idle_data->starting_time_valid = TRUE;
        gprs_l1_idle_data->start_time_valid = TRUE;
        l1_idle_data->starting_time = l1_transfer_data[gas_id].starting_time;
      }

      /*  Check if starting time has elapsed, if so, inform MAC, stop process
      and return to IDLE  */

      if (starting_time_elapsed)
      {
        /*  Inform MAC  */
        l1_mac_sig_t            message_buffer,*message;

        RESET_BITMAP(gas_id);

        MSG_GERAN_MED_1_G("PL1: SNGLE BLCK ST elapsed %d",l1_transfer_data[gas_id].config_bitmap);

        /* Align the pointers */
        message = &message_buffer;

        /* Set up the message header */
        message->task_id = MS_MAC_L1;
        message->sig_id = L1_MAC_51_SB_FAILURE;
        message->msg.sb_51_failure.gas_id = check_gas_id(message_ptr->gas_id);


        /* Send the message, NULL used for MAC_PH_DATA_IND */
        L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);


        /*  Return to idle  */
        //l1_tskisr_blk->sub_sub_state = L1_GPRS_IDLE_INIT;
        //l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

      }



      /*  If single block config has been previously received but not sent,
      (waiting for starting time) and another single block config is
      received, then clear Freq config and uplink data valid flags
      as new ones will be sent */
      if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) )
      {
        /*  Clear freq config flags  */
        CLEAR_FREQ_CONFIG_RECV(gas_id);
        MSG_GERAN_MED_2_G("clr f_cfg %d %d",gl1_get_FN( gas_id ),l1_transfer_data[gas_id].config_bitmap);
      }

      /* Mark mac_l1_single_block_config received if the startiing time has not elapsed*/
      if(!starting_time_elapsed)
      {
        SET_SINGLE_BLOCK_RECV(gas_id);
      }

      if (SINGLE_DL_BLOCK_ALLOC_COMP(gas_id))
      {
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        /*  All signals have been received to fully describe a SINGLE BLOCK
        UL allocation then so move to L1_TRANSFER state and service it */
        MSG_GERAN_MED_1_G("alloc comp %d",gl1_get_FN( gas_id ));
        /* setup tbf establish type  */

        /* Stop the SCE */
        l1_sc_stop(gas_id);
        l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

        l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
        l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
        l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
        l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
        l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;
        RESET_BITMAP(gas_id);

        GL1_ISR_SAVE_LOCK(gas_id);
        l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
        l1_tskisr_blk->command = L1_NULL_COMMAND;
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
        gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        /* wake up layer 1 isr */
        gl1_drx_require_next_tick(gas_id);

        GL1_ISR_SAVE_UNLOCK(gas_id);
      }

      break;
    }

    /*******************************************************
    *                                                      *
    *                MAC_L1_RELEASE                        *
    *                                                      *
    ********************************************************/
  case MAC_L1_RELEASE:
    {
      mac_l1_release_t *message_ptr;
      prach_params_T  *l1_prach_params_ptr;

      /*lint -e(740) ccww legacy code */
      message_ptr = (mac_l1_release_t *) msg_header;
      gas_id = check_gas_id(message_ptr->gas_id);

      l1_prach_params_ptr = gl1_ms_switch_prach_params_idle_if_data(gas_id);

      MSG_GERAN_HIGH_0_G("MAC_L1_REL");

      GL1_ISR_SAVE_LOCK(gas_id);
  
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      gpl1_gprs_register_release_recovery_event(message_ptr->release_tbf,gas_id);
#endif

      switch (message_ptr->release_tbf)
      {
      case  UL_TBF_NORMAL:
        {
          /*  Check if there is an UL RRBP pending  */
          if (gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
          {
            l1_prach_params_ptr->rrbp_pending = TRUE;
            MSG_GERAN_MED_1_G("(IDLE_IF) prach-rrbp release pending [%d]",gl1_get_FN( gas_id ));
            MSG_GERAN_MED_0_G("Unexpected UL_RRBP pending ");
          }
          else
          {
            MSG_GERAN_MED_1_G("Idle normal release[%d]",gl1_get_FN( gas_id ));

            /*  set release type for release confirm which is sent we
            *  get back to IDLE mode
            */

            l1_prach_params_ptr->rrbp_pending = FALSE;

            /* Ensure that the release confirm will be sent */
            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;
          }

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
          #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          break;
        }

      case  UL_TBF_ABNORMAL:
        {
          /* As abnormal there will be no RRBP outstanding */
          MSG_GERAN_MED_1_G("Idle abnormal release[%d]",gl1_get_FN( gas_id ));

          /* set release type for release confirm which is sent we
          *  get back to IDLE mode
          */
          l1_prach_params_ptr->rrbp_pending = FALSE;

          /* Ensure that the release confirm will be sent */
          l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;
      
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
          #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          break;
        }

      case DL_TBF_NORMAL:
      case DL_TBF_ABNORMAL:
        {

          /* As abnormal and start time not gone off there will be no RRBP outstanding */
          MSG_GERAN_MED_1_G("Idle of release DL [%d]",gl1_get_FN( gas_id ));

          /* set release type for release confirm which is sent we
          *  get back to IDLE mode
          */
          l1_prach_params_ptr->rrbp_pending = FALSE;

          /* Ensure that the release confirm will be sent */
          l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
          #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          break;
        }

        case UL_AND_DL_TBF_NORMAL:
        case UL_AND_DL_TBF_ABNORMAL:
        {
          /* As abnormal and start time not gone off there will be no RRBP outstanding */
          MSG_GERAN_MED_2_G("Idle of release UL+DL [%d], rel_type %d",gl1_get_FN( gas_id ), message_ptr->release_tbf);
          /* set release type for release confirm which is sent we
           *  get back to IDLE mode
           */
          l1_prach_params_ptr->rrbp_pending = FALSE;

          /* Ensure that the release confirm will be sent */
          l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL_DL;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        }

      default:
        break;

      } /* End of switch */


      {
        volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        l1_local_idle_data_t      *l1_local_idle_data_p = l1_get_l1_local_idle_data(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if (gl1_msg_get_multi_sim_mode())
        {
          if((l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE ) &&
             (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
          {
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);
            l1_transfer_data[gas_id].confirm_pending = SENT;
            l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
            l1_local_idle_data_p->next_req_frame = GL1_DEFS_INVALID_FN;
            if(grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_RETAINED_FOR_ACCESS ||
                grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) ==  GL1_TRM_GRANTED)
            {
              grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
              grm_set_trm_release_required(TRUE, l1_tskisr_blk->client_id, gas_id);

              /* Release the tx chain when TBF is about to be established and TRM is granted for both Tx and Rx chain  *
               * but MAC_L1_RELEASE is received in idle mode itself; setting trm_release_required may not help here    *
               * as from frame manager if TRM reserve at happens instead of a TRM release                              */
              grm_release_tx_chain(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);

            }
            else if ((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_CALLBACK_PENDING) &&
                     (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND_WAIT_FOR_TRM))
            {
               grm_release(l1_tskisr_blk->client_id, gas_id);
            }
          }

        }
        else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        if((l1_tskisr_blk->l1_state == L1_IDLE_MODE) &&
         (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
        {
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);
          l1_transfer_data[gas_id].confirm_pending = SENT;
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
        }
      }

      gl1_drx_require_next_tick(gas_id);

      /*  Set starting time to invalid as release has been received */
      l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;

      l1_sc_stop(gas_id);

      /* Return to IDLE when release received */
      gpl1_gprs_setup_start_idle((void *)0, gas_id);
      GL1_ISR_SAVE_UNLOCK(gas_id);
      break;
    }

    /*******************************************************
    *                                                      *
    *                MAC_L1_PACCH_RRBP                     *
    *                                                      *
    ********************************************************/
  case MAC_L1_PACCH_RRBP:
    {
      gpl1_gprs_idle_add_rrbp_msg(( mac_l1_pacch_rrbp_t *) msg_header);
      break;
    }

  default:
      MSG_GERAN_ERROR_0("PL1: Bad Message recvd!");
    break;
  } /* End of switch */
}

/*===========================================================================

FUNCTION  gpl1_gprs_do_RESELECTION_GPRS

DESCRIPTION Used for deciding whether L1 needs to do SCH or BCCH at reselection
times.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_do_RESELECTION_GPRS(IMH_T *msg_header)
{
  gas_id_t gas_id;
  mph_gprs_reselection_req_t *message_ptr;
  gpl1_gprs_idle_if_data_t *gpl1_gprs_idle_if_data_ptr;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
  idle_data_T      *l1_idle_data ;

  message_ptr = ( mph_gprs_reselection_req_t *) msg_header;
  gas_id = check_gas_id(message_ptr->gas_id);
  l1_idle_data = &l1_idle_data_store[gas_id];

  gpl1_gprs_idle_if_data_ptr = gl1_ms_switch_gprs_idle_if_data(gas_id);
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gpl1_gprs_setup_RESELECTION_GPRS( msg_header );

  {
    GL1_ISR_SAVE_LOCK(gas_id);
    /* The structure "new_serving_cell_info" is updated by the function
    * "gpl1_gprs_setup_RESELECTION_GPRS". Make sure that the function is called
    * before doing the following check
    */
    if(gpl1_gprs_idle_if_data_ptr->new_serving_cell_info != NULL)
    {
      if( gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->bsic_known == TRUE )
      {
        MSG_GERAN_MED_3_G("L1_FIND_SCH_COMMAND sync_flag(%d) new_serving_cell_info(%p) bsic_known(%d)",
                message_ptr->sync_flag,
                gpl1_gprs_idle_if_data_ptr->new_serving_cell_info,
                gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->bsic_known);

        l1_tskisr_blk->main_command = L1_FIND_SCH_COMMAND;
        l1_tskisr_blk->command = L1_DECODE_SCH_COMMAND;
    l1_idle_data->reselection_apply_freq_correction = TRUE;
      }
      else
      {
        MSG_GERAN_MED_3_G( "L1_FIND_BCCH_COMMAND sync_flag(%d) new_serving_cell_info(%p) bsic_known(%d)",
                 message_ptr->sync_flag,
                 gpl1_gprs_idle_if_data_ptr->new_serving_cell_info,
                 gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->bsic_known );

        L1_send_ISRTIM_ACQ_REQ(message_ptr->gprs_surround_data.bcch_params.bcch_arfcn, gas_id);
        l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
        l1_tskisr_blk->command = L1_ACQUIRE_DURING_RESELECTION_COMMAND;
      }
    }
    else
    {
      MSG_GERAN_MED_2_G( "L1_FIND_BCCH_COMMAND sync_flag(%d) new_serving_cell_info(%p) ",
               message_ptr->sync_flag,
               gpl1_gprs_idle_if_data_ptr->new_serving_cell_info);

      L1_send_ISRTIM_ACQ_REQ(message_ptr->gprs_surround_data.bcch_params.bcch_arfcn, gas_id);
      l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
      l1_tskisr_blk->command = L1_ACQUIRE_DURING_RESELECTION_COMMAND;
    }
    GL1_ISR_SAVE_UNLOCK(gas_id);
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_setup_RESELECTION_GPRS

DESCRIPTION
  ??

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_setup_RESELECTION_GPRS( IMH_T *msg_header )
{
  gas_id_t gas_id;
  mph_gprs_reselection_req_t *message_ptr;
  gpl1_gprs_idle_if_data_t *gpl1_gprs_idle_if_data_ptr;
  idle_data_T               *l1_idle_data;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

  /* align pointers */
  message_ptr = (mph_gprs_reselection_req_t *)msg_header;
  gas_id = check_gas_id(message_ptr->gas_id);

  gpl1_gprs_idle_if_data_ptr = gl1_ms_switch_gprs_idle_if_data(gas_id);
  l1_idle_data=&l1_idle_data_store[gas_id];
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  {
    /* Tell the SCE to stop its idle mode procedures.  Must be done before
    * new serving cell ARFCN is set.
    */
    l1_sc_stop(gas_id);

    /* Tell CGPS we are leaving this cell */
    gl1_timetag_cgps(l1_tskisr_blk->l1_state, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);

    /* Wait for real next frame tick to send pending TT update from DPC task */
    gl1_wait_for_dpc( gas_id );

    /*copy new serving cell BCCH chan num*/
    l1_idle_data->campedon_cell_ARFCN = message_ptr->gprs_surround_data.bcch_params.bcch_arfcn;
    gpl1_gprs_idle_if_data_ptr->new_serving_cell_info = l1_sc_balist_get_cell_data(l1_idle_data->campedon_cell_ARFCN,gas_id);

    if( NULL != gpl1_gprs_idle_if_data_ptr->new_serving_cell_info )
    {
      l1_idle_data->campedon_cell_BSIC = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->bsic;
      MSG_GERAN_MED_3_G("setup_RESELECTION_GPRS campedon_cell_ARFCN(%d) new_serving_cell_info(%d) campedon_cell_BSIC(%d)", l1_idle_data->campedon_cell_ARFCN.num, gpl1_gprs_idle_if_data_ptr->new_serving_cell_info, l1_idle_data->campedon_cell_BSIC);

      /* Change the ISR mode */
      l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

      l1_tskisr_blk->next_params.BSIC = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->bsic;

      l1_tskisr_blk->next_params.ARFCN =  gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->arfcn;

      /* Update Idle AGC struct */
      l1_idle_data->campedon_cell_agc = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->agc;
      sys_algo_agc_enable_slew_rate_limiting(&l1_idle_data->campedon_cell_agc);

      /* Use ncell's AGC when reselecting to that cell */
      l1_tskisr_blk->next_params.L1Data.pAgc_data =  &(l1_idle_data->campedon_cell_agc);

      /*disable reading of ext BCCH*/
      l1_tskisr_blk->next_params.extended_BCCH_mask = SERV_EXT_BCCH_MASK_NONE;


      /* Setup default BCCH decoding mask in for when we go
      * back to Idle Mode.
      */
      l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM;
      l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask( l1_idle_data->si_mask );
      l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );

      /*copy new timing info to ISR*/
      l1_tskisr_blk->next_params.frame_lag  = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->fn_lag;

      /*lint -save -e732 no loss of sign, sign restored in ISR handler */
      l1_tskisr_blk->next_params.QB_lag = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->qbit_lag;
      l1_tskisr_blk->next_params.rx_power = gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->rx_power;
      /*lint -restore */

      /* This flag tells the ISR that a reselection cnf is to be sent */
      l1_tskisr_blk->next_params.reselection_cnf_reqd = TRUE;

#ifdef FEATURE_GSM_CELL_RESELECTION_OPTION20
      /* Adding the previous serving cell to BA list */
      l1_sc_balist_add_serving_cell(gas_id);
#endif /* FEATURE_GSM_CELL_RESELECTION_OPTION20 */

     l1_sc_balist_set_serving_cell( gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->arfcn,gas_id );

#ifdef FEATURE_GSM_COEX_SW_CXM
      garb_intf_notify_find_bcch_update_arfcn(gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->arfcn,gas_id);
      garb_intf_notify_find_bcch_update_rx_power(gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->arfcn,
                                                 gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->rx_power,gas_id);

#endif /* FEATURE_GSM_COEX_SW_CXM */

      MSG_GERAN_MED_1_G("new_serving_cell_arfcn %d",gpl1_gprs_idle_if_data_ptr->new_serving_cell_info->arfcn.num);

    }
    /* Make sure ISR is running - don't want to wait until next
    * paging block.
    */
    gl1_drx_require_next_tick(gas_id);

  }

}




/*===========================================================================

FUNCTION  gpl1_gprs_L1_params_pending

DESCRIPTION
    This function stores the update L1 parameters sent from GRR due to changes
    received in system parameters. These changes are stored in a pending store
    to allow the update to occurr in a controlled manner and synchronised to the
    GPL1 activities.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_L1_params_pending(IMH_T *msg_header)
{
  gas_id_t gas_id;
  mph_gprs_l1param_update_req_t *message_ptr;
  gpl1_gprs_idle_if_data_t *gpl1_gprs_idle_if_data_ptr;
  gprs_l1_idle_data_t *gprs_l1_idle_data;

  /*lint -e(740)*/
  message_ptr = (mph_gprs_l1param_update_req_t *)msg_header;
  gas_id = check_gas_id(message_ptr->gas_id);

  gpl1_gprs_idle_if_data_ptr = gl1_ms_switch_gprs_idle_if_data(gas_id);
  gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);

  {
    gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data = message_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_sys_info;
    gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data.BS_PA_MFRMS, gas_id);

    /* Config the egprs quality measurements module using the bep period
     * broadcast on the BCCH.
     */
    gpl1_egprs_scell_qual_meas_broadcast_config
    ( message_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.bep_period,gas_id );

    gpl1_gprs_idle_if_data_ptr->l1_update_pending.layer1_extra_gprs_sys_info = message_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info;

    /****************************************************************************************/
    /* Copy over the CBCH parameters immediatly instead of waiting for the next frame tick. */
    /* This is to prevent the race condition whereby CB_SCHEDULING_REQ is handled when CBCH */
    /* parameters are stuck in the pending update. CR56042.                                 */
    /****************************************************************************************/
    if ( l1_smscb_is_active(gas_id) )
    {
      ARFCN_T cbch_arfcn = l1_smscb_get_arfcn(gas_id);
      ARFCN_T serving_cell_arfcn = gl1_get_serving_cell_arfcn(gas_id);
      if (!ARFCNS_EQUAL(cbch_arfcn, serving_cell_arfcn) ||
      l1_smscb_get_bsic(gas_id) != gl1_get_serving_cell_BSIC(gas_id))
      {
        l1_smscb_init(gas_id);
      }
    }

    l1_smscb_set_chan_type(gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data.cbch_channel_information.channel_type,
                           gas_id);

    l1_smscb_set_arfcn( gl1_get_serving_cell_arfcn(gas_id),gas_id);

    l1_smscb_set_bsic( gl1_get_serving_cell_BSIC(gas_id),gas_id );

    l1_smscb_set_cb_supported(gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data.cbch_channel_information_valid,
                              gas_id);

  }

  /*  Set flag to indicate idle parameters have changed and need to be updated. */
  gprs_l1_idle_data->update_pending = TRUE;

}


/*===========================================================================

FUNCTION  gpl1_gprs_update_L1_params

DESCRIPTION
    This function uses the stored update L1 parameters sent from GRR due to changes
    received in system parameters. The stored parameters are held in a pending store
    and placed in there by the function gpl1_gprs_L1_params_pending. This function
    will now update the used parameters from the pending parameters.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_update_L1_params(gas_id_t gas_id)
{
  inter_task_BCCH_allocation_T  ba_buffer;
  gpl1_gprs_idle_if_data_t *gpl1_gprs_idle_if_data_ptr = gl1_ms_switch_gprs_idle_if_data(gas_id);
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];


  MSG_GERAN_HIGH_1_G("gpl1_gprs_update_L1_params %d",gl1_get_FN( gas_id ));

  /* Transfer data from the pending buffer. Load up the data from the message */
  if (gpl1_gprs_idle_if_data_ptr == NULL)
  {
    MSG_GERAN_ERROR_0_G("gpl1_gprs_idle_if_data_ptr 0");
    return;
  }
  else
  {
   GL1_ISR_LOCK(gas_id);

    l1_idle_data->campedon_cell_data = gpl1_gprs_idle_if_data_ptr->l1_update_pending.campedon_cell_data;
    l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id);

    l1_idle_data->layer1_extra_gprs_sys_info = gpl1_gprs_idle_if_data_ptr->l1_update_pending.layer1_extra_gprs_sys_info;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    MSG_GERAN_MED_1_G("PCHXFER read_pch_in_transfer %d",l1_idle_data->layer1_extra_gprs_sys_info.read_pch_in_transfer);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

    gprs_serv_cell_meas_ptr->alpha = gprs_l1_idle_data->gprs_campedon_cell_data.power_control.alpha;

    if(l1_idle_data->campedon_cell_data.LB_MS_TXPWR_MAX_CCH_valid)
  {
      gprs_serv_cell_meas_ptr->lb_pmax = l1_idle_data->campedon_cell_data.LB_MS_TXPWR_MAX_CCH;
      gprs_serv_cell_meas_ptr->gprs_LB_MS_TXPWR_MAX_CCH_valid=TRUE;

    }else
    {
      gprs_serv_cell_meas_ptr->gprs_LB_MS_TXPWR_MAX_CCH_valid=FALSE;
    }

    gprs_serv_cell_meas_ptr->pmax = l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;


    /*  Update uplink power based on the modified power control parameters. */
    gpl1_gprs_calc_rf_pwr( gas_id);

    L1_calculate_paging_data(gas_id);

    /* To ensure that l1_change_timeslot() is called to apply timeslot
     * changes we need to set the main_command to L1_IDLE_COMMAND.
     * Do this only if we are in L1 state IDLE_MODE  & main_command = NULL  */
    if ( (l1_tskisr_blk->l1_state == L1_IDLE_MODE) &&
         (l1_tskisr_blk->main_command == L1_NULL_COMMAND) )
    {
        l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
    }


   GL1_ISR_UNLOCK(gas_id);

    L1_remove_oob_ba(&ba_buffer,&l1_idle_data->campedon_cell_data.BA_list);
    l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);

    l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id);

    /* Set multiband reporting parameter */
    l1_sc_balist_set_multiband_reporting(l1_idle_data->campedon_cell_data.multiband_reporting,gas_id);


  }

}

void gpl1_gprs_setup_start_idle( void *dummy_param, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint8 dummy_value=0xFF;
  /* parameter not used */
  NOTUSED(dummy_param);

  geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&(dummy_value),FALSE, gas_id);

  /* clear out the PACCH UL FIFO if we have any msgs remaining from transfer */
  gpl1_gprs_reset_pacch_fifo(gas_id);

  /* Ensure we clear all rrbp's before returning to idle */
  l1_init_gpl1_gprs_rrbp_data(gas_id);

  /* Ensure MAC Rx message bitmap is reset to zero when moving to IDLE mode */
  RESET_BITMAP(gas_id);

  /* work out which idle mode to return back to */
  {
    /* Prepare the SCE to restart in Idle mode.
    * The ISR will probably need to run to complete aborts
    * associated with the stop.  The SCE won't start again
    * until l1_sci_go is called from the init
    * sub state of Idle mode ISR.
    */
    l1_sc_start( L1SCModeIdle, gas_id );

    /* Tell SCE that GRR will schedule ncell BCCH decodes. */
    l1_sc_set_bcch_autonomy( FALSE,gas_id );

    l1_sc_set_paging_multiframes( l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id );

    l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
    l1_tskisr_blk->command      = L1_NULL_COMMAND;

    /* ignore all receives on this TBF from now on, we are returning to idle */
    l1_transfer_data[gas_id].abort_receives = TRUE;
    l1_transfer_data[gas_id].polling_at_tbf_start_time = FALSE;

  }

  /* Re-initialise the assigned Tx Slots to "1" for GSM */
  /* if that is scheduled, and in the case where the UE */
  /* goes back to E/GPRS the new assignment will set it.*/
  gl1_msg_set_num_assigned_tx_slots (DEFAULT_ONE_TS, gas_id);
  MSG_GERAN_HIGH_0_G("GSM Tx Slots=1");

}

