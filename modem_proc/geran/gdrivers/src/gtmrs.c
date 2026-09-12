/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     G S M   T I M E R   S E R V I C E S

GENERAL DESCRIPTION
   This module contains the GSM timer drivers.  The GSTMR and AMBA timers
   are supported by this module.

EXTERNALIZED FUNCTIONS
  gstmr_init
    Procedure that registers the GSTMR ISR handler and unmasks the GSTMR
    interrupt.
  gstmr_gsm_core_reset
    Procedure that takes the GSTMR out of reset.
  gstmr_register_handler
    Procedure to register a handler for the TDMA strobe processing.
  gstmr_deregister_handler
    Procedure to deregister a handler for the TDMA strobe processing.
  gstmr_delay_interrupt
    Procedure to specify when the ARM should get the TDMA interrupt.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gtmrs.c#4 $
$DateTime: 2020/09/08 23:34:17 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
06/09/20   rks      CR2754778 corrected fetching of device mode while setting frame tick signal in suspend mode.
13/07/20   rks      CR2723948 setting frame tick signal in fn handler when in suspend mode.
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix 
23/06/17   mn       CR2055160 Fix for regression caused by CR2009497(PDTCH last burst over PCH first burst)
03/01/18   ksb       CR2164555 Geran support to send Global Time Services Information TO GPS module
11/07/17   sn        CR2137112 Correct USTMR_REF_START calculation of IDLE SUB first burst after wakeup 
31/07/17   mn        CR2077649 Fix for PDTCH last burst is yeilding over PCH first burst by using last appended USTMR start time
07/11/16    og      CR1079285. Replace pcycle with qtimer.
12/01/16   sp       CR945229: Ensure Async Command issued only after Enter Mode to GFW in Normal ISR
11/10/15   tsk      CR937467: Relocate dynamic RxD handler to end of gstmr_frame_tick_process.
11/08/15   tsk      CR929882: Handle RxD exit processing.
09/10/15   akh      CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
03/06/15   zf       CR847986: Align HW FN during warmup
19/05/15   cws      CR835172 Enable W segment loading in Sub2
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/05/15   npt      CR821672 GERAN power logging update 
21/04/15   cs       CR808438 Ensure any pending TT requests are sent
09/04/15   dp       CR819956 Thor 2.x RF API change, G2W use MSGR
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
10/12/14   ws       CR768376 Q6 compile warning fixes
27/11/14   jj       CR 760174  fix compiler warning
17/11/14   npt      CR642426 Added power reporting log packet
21/10/14   fan      CR734490 Rework the wake up recovery mechanism
17/10/14   ws       CR730418 Correct GPS timetagging being out by 1 TDMA frame
26/09/14   zf       CR732436: Delay starting ACQ when RF wakes up late
26/09/14   zf       CR730930: Cancel pending GFW commands when RF does not wakeup before next frame to mitigate TRM/RF concurrency delay issue.
23/09/14   fan      CR727696 Avoid race condition between starting the timer in warmup CB and RF wakeup cnf processing
12/09/14   fan      CR636420: Run Sleep commit after GSTMR ISR at wakeup
04/09/14   zf       CR702512: Correction in detecting GSTMR interrupt pending
03/09/14   df       CR716674 Check hw state before getting quarter symbol count
25/08/14   df       CR660576 Enhanced timeline debug
20/08/14   npt      CR709922 De-register VSTMR from task context if going to sleep on opt3
06/08/14   df       CR608202 Avoid running DRX in silent frame if next_tick_required asserted
06/08/14   df       CR598015 Avoid silent frame DRX processing when pending firmware activity trigger is set
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
08/05/14   hd       CR660875: Defined gstmr_init_odd_even_fnmod4()
10/07/14   dp       CR687658 Fix XO -> GSM-FN bug causing rare snapshot issues
12/06/14   pg       CR661021 Fix GPS time snapshot before sleep for early go to sleep
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
30/05/14   pg       CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
09/06/14   pg       CR676887 Protect against reads from NULL geran VSTMR view
12/05/14   ssh      CR661514 Additional protection around panic reset handling [workaround for buffer overflow]
24/04/14   pg       CR655101 Compilation error fixes due to CR644441
11/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
11/04/14   pg       CR645875 Fix Bolt multisim compilation errors
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
03/04/14   ip/ssh   CR643132 Restrict 2 TS ISR overrun margin to non-LMM and DSDA modes only
28/03/14   zc       CR640586: API to switch antenna needs to support multi-sim config
26/03/14   npt      CR613395 Do not perform frame extension in consecutive frames
12/03/14   npt      CR628641 Ensure vstmr de-register never gets called without matching vstmr register
12/03/14   ssh      CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
03/03/14   npt      CR622060 Rework wake-up
21/02/14   ssh      CR619127: Increase ISR overrun margin for DSDS/TSTS/DSDA cases to 2 TS.
18/02/14   nk       CR619009 Register TDMA_ISR handler only when stack is not deactivated
04/03/14   nm       CR601168 register TDMA_isr handler if gstmr is re-init
04/03/14   nm       CR593290  update the fix for CR CR576915
03/03/14   cgc      CR622086 Create Instance CLSID_WCDMA only once.
24/02/14   am       CR622045 Use correct interrupt vector number for 3rd gstmr
18/02/14   ssh      CR616652: Use both gstmr_is_int_pending and mod4 FN to check for pending GSTMR interrupt.
21/11/13   dv       CR579907 Back out CR 519537
13/02/14   ka       CR614268: Add debug for BPLMN search not resuming
12/02/14   cja      CR610689 Add API gstmr_rd_hw_fn_qs (synchronised FN and qs)
06/02/14   cgc      CR598057 add lte_ml1_md_gsm_tick handler to GL1 isr logging
07/02/14   ws       CR607213 - Implement segment loading for wl1trm_can_gsm_disable_fw_app_wcdma()
04/02/14   ssh      CR607829: Skip the first burst if we are in QTA gap during wake up transition.
03/02/14   ssh      CR609484: Incorrect usage of USTMR ticks whilst GTS pending GSTMR detection.
30/01/14   cja      CR608949 When register gstmr, set FN based on fn_modMaxFN when deregistered.
29/01/14   npt      CR602149 Use vstmr_geran_view_adjust_offset api for time tracking adjustment
24/01/14   cah      CR598552: enable sleep for DSDS GSTMR+VSTMR
20/01/14   cos      CR604531 Synchronize interrupt deregistration and ISR handling:
                         GERAN updates for treating VSTMR deregister based on the current task context
20/01/14   cja      CR599086 For all X/G transitions sleep and wake-up gstmr, correct for missed frame ticks
16/01/14   sp       CR600563: Handle ISR Overrun from Idle State machines instead of Warmup ISR
13/01/14   ss       CR598548 Deregister/Register gstmr view in VSTMR during W Sleep / wakeup cycle
06/01/14   cja      CR596240 Wake-up gstmr and schedule event, even if not re-initialising gstmr
02/12/14   js       CR596125 :  T+G compilation errors removed
15/01/14   ssh      CR600467:GL1 to implement a GTS recovery mechanism if there is a ISR overrun
09/01/14   cah      CR598075  PCH decode failures observed on sub 1 after voice call on sub 2
08/01/14   cah      fix for vstmr stability issues
07/01/14   CAH      CR593152 - fix for deep sleep crash when disabling VSTMR view
30/12/13   ssh      CR594683 - DI3.0 | GERAN LLVM compilation error removal for new version of LLVM version
20/12/13   cja      CR552831 Add mutex for logging handler timestamps
17/12/13   cja      CR590767 Bolt G2W - add gap start time to g2w startup
12/12/13   ssh      CR588188: ISR overrun recovery mechanism
16/12/13   xz       CR580972 when handler is full, and fail to register, Extend buffer length.
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
04/12/13   js       CR576915 GL1 needs to check if GSTMR is initialized before invoking gstmr_rd_qsymbol_count() - issue detected after panic reset
27/11/13   sp       CR568097 Don't adjust FN increment in oddparity check if W is active.
26/11/13   npt/ip   CR582180 Enable sleep on Bolt
25/11/13   pa     CR579842:Incorporate CR533574 and issue cleanup immediately after receiving results.
24/10/13   cos      CR566292 - Request for API for reading current GFN based on XO cnt value
18/10/13   npt      CR542638 - Changes to support sleep on Bolt
29/08/13   sp       CR519537: Enable GSTMR F3s
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
22/11/13   cja      CR455527 Remove featurisation from files in API directory
11/14/13   ab       CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
04/11/13   cja      CR562152 Add F3 for gsmtr sleep status during gsmtr init
31/10/13   cgc      CR569801  Add wrapper function gl1_cm_advance_fn
25/10/13   ws       CR473460 - W/T segment loading, removed WCDMA API calls
02/10/13   cja      CR553247 Add debug, recovery for FCW set as 0.
23/08/13   ws       CR533027 data space DAL timetick handle
14/06/13   mc       CR494538 Initialise and bound check index into GSTMR debug buffer
02/10/13   cja      CR552831 Add mutex for logging handler timestamps
29/08/13   sp       CR519537: Enable GSTMR F3s
28/08/13   cja      CR533574 Correct frame number for immediate async command at frame start.
05/09/13   cs       Map correct GSTMR for sleep timetagging in Multi-SIM
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
14/08/13   npt      CR528491 - Use mod4096 instead of mode4 for interactions with FW
18/06/13   npt      CR497521 - Added VSTMR changes
15/08/13   sk       CR524039 GBTA bringup changes
08/08/13   cja      CR519510 Use DPC to call MCPM, send Tx Config only when MCPM ready
05/08/13   cja      CR514498 Only increment current_buffer_mod_3 once per frame, plus for T2G async
10/06/13   cja      CR498279 Do not increment current_buffer_mod_3 twice in G2W only frame
15/07/13   sp       CR508123: Update panic reset to use multi-sim data space
10/07/13   ip       CR508975 - Adjust Qsymbol calcultaion
22/07/13   og       CR500731. Sanity check the GSTMR handler index.
20/06/13   cs       Major Triton TSTS Syncup
16/06/13   cs       Further TSTS Updates
12/06/13   ip       CR359544: Fixed xCCH decodes when option 3 runs in DSDS mode
03/06/13   ws       CR495031 pass IST stack size and Trigger configuration to DAL interupt Controller
21/05/13   cja      CR489091 Set gstmr_FN_mod4 when set gstmr FN
13/05/13   cs       TSTS Updates
13/05/13   ss       CR486978 err_fatal if value of gstmr num_handlers is greater than valid handler ptr
13/05/13   jk       CR463435:Using DAL interface to calculate GSM sleep time tag
25/04/13   pjr      CR480197 updated parameters to gl1_hw_cm_get_status
13/03/13   dv       CR458558 Don't issue GCC from ISR for X2G
03/04/13   ss       CR470885 Control mcpm mutex corresponding to gas_id
08/03/13   ws       CR460576 -  Implement gl1_cm_set_fn() API
22/02/13   pg       CR455347: Expose gstmr_wakeup_init as external API
07/02/13   pg       CR449164: include TT adjustment in sleep calculation
06/02/13   pg       CR449662: remove GSTMR delay and overrun debug
01/02/13   cja      CR448030 Clear gap commands for aborted gap
30/01/13   jk       CR434373 - Reporting GSM sleep time tag to GPS
25/01/13   pg       CR444247: GSTMR compatability changes for Dime V1 & V2 HW
03/01/13   npt      CR430322 - Modify sleep debug structure
10/12/12   cja      CR411308 Dime T2G changes (msg router to GFW)
06/12/12   npt      CR346414 - Remove first GSTMR after wake-up
26/11/12    pg      CR423852: G2W startup can be scheduled in next frame on Dime modem
26/11/12    ws      CR424198 - API backward compatible with TRITON DSDA changes
19/11/12   cja      CR422874 Add pseudo Hw FN for Dime
14/11/12   npt      CR420954 - Added changes for power collapse
08/11/12   npt      CR419262 - Enabled sleep
07/11/12   dv       CR418770 T2G: Race condition between GL1 issuing async int
                             from gstmr_isr & GFW clearing buffer
26/10/12   dv       CR400143 T2G: Crash caused when in 1 GSM frame 2 sets of API
                             calls are made which are not for back to back gaps
10/09/12   dv       CR382139 ftick parity error causing T2G cmds not to be issued.
04/09/12   og       CR395649. T2G Acquisition and BSIC decode enhancements.
24/08/12   npt      CR382936 - Reworked the initial fix - Avoid race condition between MDSP CMD done ISR and
                    GSM sleep controller start sleep ISR on async interrupt
01/08/12   npt      CR382936 - Avoid race condition between MDSP CMD done ISR and
                    GSM sleep controller start sleep ISR on async interrupt
05/07/12   cja      CR373298 Use mutex to prevent frame number increment whilst setting FN
09/07/12   ky       Sending async interface message to GFW if we put commands while processing
                    GERAN_FW_SLEEP_READY_CMD(replacement to opt3 legacy) message
27/06/12   pg       DAL IC updates: avoid multiple DAL registrations for same handler
25/06/12   pg       GSTMR timing adjustments via online start
30/05/12   pg       Add access function for reading mod4 HW frame number
30/05/12   pg       Dime GSTMR enabling and IRQ mapping updates (RUMI BU)
11/05/12   og       CR359890. Prevent incorrect queuing of x2g command nodes.
27/04/12   ws       CR355863 Fix compile issue for original checkin where TDSCDMA
                    is not defined
25/04/12   ws       CR 355838 FR 1604 - GERAN changes to cleanup WCDMA- GL1 API
25/04/12   og       CR355863. Ensure the SW GFN is always sanity checked against
                    the odd_even bit when T2G IRAT is enabled.
23/04/12   pg       CR314929: Reduce overhead for processing ISR delays
23/04/12   ws       CR355017 Fix Compile errors and warnings introduced to fix FR 352228
16/04/12   cja      CR350203 Send loopback command in ISR
14/04/12   og       CR352228. T2G updates and fixes.
11/04/12   pg       CR351140: Make HW/SW FN check failure non-fatal
30/03/12   cja      CR340180 Add reading of GSM HW frame counter back into Nikel
19/03/12   pg       Dime modem specific HWIO implementation  (esp. new GSTMR block)
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
14/03/12   pg       Add support for QuRT
14/03/12   pg       Add support for DAL interrupt controller (tramp deprecated)
03/02/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
29/03/12   cja      CR341225 RxFE recovery for X2G
15/02/12   dv       T2G:Integrate FCCH/SCH bring changes
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
06/02/12   cja      CR332659 Always send generic config, to fix WTR KVCal issue
02/02/12   cja      CR328529 Do not add generic config at the end of the frame if empty buffer
22/11/11   jj       CR319573 incorporate with review comments
16/11/11   jj       CR319573   changes for  profiling the modem performance
03/11/11   og       CR316831. Remove customer's compiler/linker errors.
21/09/11   cja      CR305075 (contd) correction of line deleted by mistake
20/09/11   cja      CR305075 - Do not reset GSTMR on every gstmr_inint (for W2G handover)
24/08/11   tjw      std_strlcpy -> strlcpy to obviate deprecated AEE/Component Services string library
25/08/11   ws       CR303300 - Reorder MCPM and GDRIVER MUTEX's to avoid lockup in LTE and GSM sleep
22/08/11   ws       CR302666 - Add MCPM GSTMR mutex locking mechanism
01/08/11   jj       CR294221 GSTMR toggles every time gstmr_init is called
22/06/11   cja      CR288499 Use API to access REX TCB
16/06/11   cgc      Cr289990 Add FEATURE_GSM_GPRS_QDSP6 to gl1_log_handler_timestamp_event
20/05/11   ws       Fixed compile warnings with Function defined but not used
12/05/11   ws       Fixed featurisation bug in GSTMR_RESET setting
28/04/11   ws       Disabled GSTMR_HW_FRAME_COUNT for NIKEL 1.0 as HW Frame count is not supported
28/04/11   ws       Toggle GSTMR_RESET register to enable GSTMR HW block
28/04/11   ky       Removed srhcm gsm ft handler from performace logging because CMI changes
18/03/11   ws       Added Initial MCPM updates
01/02/11   ws       Feturised EDGE core reset with MCPM and renamed MICRO_IRQ_CONFIG_RW
                    to SW_IRQ_CONFIG_RW
12/11/10   ws       Renamed MICRO_GSM_TIME_RD to GSM_TIME_RD for NikeL modem
14/06/11   npt      CR280679 added check to ASYNC if next tick already set
14/06/11   npt      Slight redesign of asynch int to allow for a silent frame
                    interrupt for the frame after a failed asynch attempt
13/06/11   pg       CR291689: Reinstate exiting from GSTMR if GSM clock is off
                    for non-Q6 targets
03/06/11   ky       290264:made gl1_trace_buffer global
20/05/11   ip       Added F3 logging to capture go to sleep failure
16/05/11   sjw      DSDS CR285521 rework of compressed mode handler detection
04/05/11   ky       Printing all the task names delaying the GSM ISR and how much time they have run
10/02/11   nt       DSDS Change to adjust the frame number if a miss is detected.
09/03/11   ky       When ISR over run happens, if ISR delay is more than 625qs, we go through
                    the trace buffer and find the task which is delaying the ISR and print the
                    task name and amount of duration it has run.
03/02/11   ky       266163:Reverted CR261247 fix. Added three frame delay before deactivating the
                    FW during the panic reset procedure to allow fw to process all the commands
                    and ready for deactivation. Also removed sleep during the inactivation procedure
22/02/11   ip/cs    CR267278 GP clock vote handled by RF drivers
15/02/11   ky       Changed ERR FATAL to MSG ERR for Missed %d GSM frames message irrespective
                    of RECOVERY RESTART NV item is set or not
26/01/11   pg       Added FEATURE_GSM_GP_CLK_DISABLE for sourcing GSM clock
                    from MSM in all modes i.e. GP_CLK always disabled
22/12/10   ky       For Q6, isr over run will turn into err fatal if GSM RECOVERY
                    RESTART NV item is not set.
15/12/10   nt       DSDS - force on the tripler clock always in DSDS
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
07/12/10   nt       DSDS - ensure that the gstmr_fn_handler is always registered as belonging
                    to the gsm tcb.
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
18/11/10   ky       Added pcycle count into the isr performance logging handler once
                    per frame.
09/11/10   ky       Removed the FEATURE_GSM_ISR_HANDLER_PERFORMACE_LOGGING from the code
                    so that this logging always enabled by default. Isr usage calculation
                    done by using the sysclks instead of qs count
29/10/10   cgc      Defer setting number of cmds until the last was sent to mdsp
                    on the 1st frame on sleep wake up
29/10/10   ky       261817: Added two wait for frame tick in the hw init so that
                    app mode command sent to the fw and fw will process that command
                    Hardcoding the app mode command moved to switch command buffers function
25/10/10   ky       259645: Installed gstmr handler for sending pending commands is not working.
                    Hardcoded the sending these commands in gstmr isr function.
18/10/10   ky       259645: Installed gstmr handler for sending pending commands. This handler
                    sends the generic config command and app mode command to fw if they are
                    pending
11/10/10   og       Make gstmr_qsym_at_int_start( ) independent of FEATURE_GSTMR_TRACE.
08/10/10   og       Fixes to CR217386 and CR182441.
01/10/10   tjw      Lint: eliminate critical error by () macro arg. Also medium error that could
                    cause a buffer overrun (-ve int coerced to uint arrary index)
30/09/10   ky       9K: CR 257913: GENERIC config command will be sent to fw after
                    populating all the shared variables into a pending buffer
02/09/10   ky       Added quarter symbol count value to the gl1 handlers logging
                    and replaced ERR FATAL with warning when MDSP DEBUG ENHANCEMENTS
                    FEATURE is not enabled. Moved GL1 Handler logging under
                    FEATURE_GSM_ISR_HANDLER_PERFORMACE_LOGGING. if NV item 1027 is invalid
                    then gl1 crashes for any subset of errors reported by mdsp.
                    Crashing fw when seq num mismatch is moved out of MDSP DEBUG ENHAN
                    CEMENT FEATURE
24/07/10   pg       CR248443/248114: Avoid forcing tripler on if SWC is not enabled
                    and avoid reading GSTMR registers if GSM clk is off.
09/07/10   pg       Add mutex lock around use of global *_odd_even_check variable
                    setting (allows INTLOCK removal from wl1sleep.c)
05/07/10   rc       Enables GP clock if its off in Q6
29/06/10   ky       Added the performace logging for gstmr handlers and rf api's
28/06/10   og       Add mutex protection to gstmr_init. Resolves CR172717.
16/06/10   ky       Err fatal is removed for FEATURE_GSM_MDSP_DEBUG_ENHANCEMENTS because
                    mdsp_memory_dump_to_file function does the err fatal after dumping
09/06/10   ky       Brought mdsp debug improvement changes under FEATURE_GSM_MDSP_DEBUG_ENHANCEMENTS feature
02/06/10   ky       Added MDSP debug improvement changes
18/03/10   ws       Added QDSP6 featurisation around rfm_get_power_collapse_statue()
12/03/10   ip       CR229279 - Ensure Tripler is on before reading GSMTR registers
02/03/10   ip       CR222839 - Use new RF API for Tripler clk status
25/02/10   tjw      strncpy -> std_strlcpy
14/01/10   pg       Adding support for BLAST OS
23/09/09   ws       Added support for GERAN Vitual platform
30/10/09   nf       Exclude ERR_FATAL check for W to G
09/09/09   cs       Fix Klocwork bounds checking for deregister_handler
13/07/09   nf       Changed MSG_ERROR to ERR_FATAL for GTMRS overruns
15/04/09   agv      Mustang changes for int_pending, int overun, and usage.
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
26/03/09   ws       Added PACKED_POST for QDSP6 targets
01/12/08   cs       Add new split gl1_hw_clk_ctl.h from gl1_hw.h
28/07/98   hv       Added KxMutex support
09/06/08   ps       Lint Cleanup
03/10/07   og       CR 131066 Provide a recovery mechanism for the missing ftick issue
08/10/07   cs       Fix the correct L4 DPC call for logging
13/04/07   agv      Added Task Name, and uS time to GSTMR ISR delayed.
03/04/07   agv      PLT complie ddefn added.
19/02/07   agv      Added l1_check_state for gstmr use to resolve WDCMA Intlock issue.
08/02/07   agv      Check CLK_RGM_GSM_M is on before gstmr_rd_qsymbol_count call.
24/10/06   ip       Fix for L4 builds
19/09/06   agv      Fixed Lint warning for rex_self() call.
06/07/06   gfr      Featurize call to tramp_clear_interrupt since older tramp
                    does not support it.
05/11/06   gfr      Save and restore GSM core register values across a reset
04/28/06   gfr      Added gstmr_disable_interrupt, gstmr_enable_interrupt,
                    and gstmr_clear_interrupt
02/01/06   agv      Merged RVDS change needed for log_alloc.
01/12/06   gfr      Fix odd/even check and add support for HW frame counter,
                    rename MSM_x macros to HWIO_x
12/08/05   gfr      Modify DPC code to support L4
11/04/05   gfr      QLint cleanup
10/18/05   gfr      Check if handler is already registered in gstmr_register_handler
10/17/05   gfr      Add support for GSTMR tracing
10/17/05   gfr      Increase max number of handlers from 5 to 8
10/12/05   gfr      Make reg/dereg of handlers while in the ISR have
                    deterministic behavior
08/26/05   gfr      Removed DEBUG_TRANSFER and FEATURE_SBI_MDSP_CONTROLLER
07/27/05   gw       Added two functions for sleep, gstmr_start_sleep() and
                    gstmr_start_sleep_was_ok().
06/29/05   gfr      Lint again.
06/01/05   gfr      Print warning if gstmr is delayed.  Lint.
06/01/05   gfr      Support for multiple calls to gstmr_adjust_terminal_count
                    in a single frame.
03/29/05   gfr      Ensure that GSM clock is on before processing interrupt
03/16/05   gfr      Added gstmr_force_reinit.
12/07/04   gfr      Add warning if the GSTMR ISR takes up too much CPU.
11/23/04   gfr      Added support for a gstmr adjust debug trace buffer,
                    featurize the mDSP controller, use proper FMSK for
                    reading HW registers.
06/08/04   gfr      Added FNmo13 for GPRS.
06/24/04   gw       Added check for ISR over run at end of gstmr_isr().  Modified
                    looping in gstmr_isr() to eliminate possibility of infinite
                    loop.
03/26/04   gfr      Exported gstmr_rd_qsymbol_count.
02/25/04   bm       Mainlined DEBUG_MISSING_FN
01/29/04   gfr      Mainlined FEATURE_INHIBIT_PSBI
01/16/04   bm       Support for Dynamicallly enable/disable
                    "Missed one GSM FTICK" debug
11/08/03   asr      Added some more debug code under
                    FEATURE_MAINTAIN_GTIME_IN_WCDMA_DEBUG.
11/05/03   gw       Added functions gstmr_set_terminal_count_symbol() and
                    gstmr_is_int_pending() to support sub-frame scheduling
                    in the sleep time line.
10/17/03   jc       Add function
10/05/03   asr      Implemented GSTMR_SET_FN().
08/11/03   jc       Add new function.  When deregistering handlers fix bug that
                    was decrementing too early.
04/11/03   ATM      Moved Frame Number management entirely inside GTMRS
02/20/02   dp       Made change to not enable IRQ and odd/even frame interrupt
                    if GSTMR has already been initialized.   Seems to mess up
                    W2G compressed mode handover
12/11/02   ih       Added call to mdsp_send_sbi_reqs at the end of ISR
12/11/02   ATM      Gabe's bugfix: unde previous change, just prevent
                    gstmr_gsm_core_reset() from reseting more than once.
12/04/02   ATM      Moved call to gstmr_gsm_core_reset() to gstmr_init() for CMHO.
10/08/02   ATM      Relocated Frame Number / Offset logging to GTMRS
10/01/02   ATM      Optimized gstmr_fn_handler() routine
08/01/02   jc       register_handler() and deregister_handler() now both use
                    num_handlers to run hand in hand. Prior versions would not
                    work when both were called.In gstmr_init moved some code
                    so that it gets called again.
07/26/02   jc       Correct double increment in deregister handler.
06/28/02   jc       Init gstmr only once, add function to delay gstrm interrupt.
06/10/02   jc       Add support for deregister_handler, install GFN handler
                    on init.
05/07/02   plm      Mainlined code associated with CHEETAH_GSM.
04/30/02   mk       Added gstmr_rd_symbol_count(), gstmr_rd_odd_even().
04/02/02   mk       Added support for CHEETAH_GSM.
02/27/02   JC       Renamed gstmr_reset() to gstmr_gsm_core_reset().
02/15/02   mk       Changed gstmr_adjust_terminal_count() to support both
                    afc/tt and L1 functionality. Fixed gstmr prescaler bug.
01/16/02   mk       Added support for afc/tt.
08/14/01   chatlani Made changes for KOVU.
04/12/01   jc/jc    Initialized GSTMR tick to occur 1 symbol after PP frame.
03/08/01   jc       Added gstmr_adjust_terminal_count() for acq.
01/12/01   mk/jc    Initial GSM version (Sina emulaton board)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif
#include "target.h"
#include "comdef.h"
#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "mcs_hwio.h"
#include "geran_msgs.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "vtmrs_g.h"
#include "log.h"
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"
#include "l1_os.h"


#include "qurt.h"

#include "rex.h"

#include "gl1_mutex.h"

#ifdef FEATURE_QSH_MDUMP
#include "l1_log_qsh.h"
#endif


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gl1_msg_g.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gl1_defs.h"
#include "gl1_defs_g.h"

#include "l1_sc_int.h"
#include "gl1_hw_sleep_ctl.h"
#include "l1_drx.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1_isr.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_hwi.h"
#ifdef FEATURE_GSM_L4_TRACE_BUFF
#include "l4/cust/msm_syscall.h"
#endif
#include "rfm.h"
#include "l1_task.h"
#include "l1_utils.h"
#ifdef FEATURE_GERAN_VP
#include <assert.h>
#endif

#include "string.h"

#ifdef FEATURE_WCDMA
#include "wl1api.h"
#endif

#include "msgr.h"
#include "geran_msg.h"

extern void gl1_hw_issue_mdsp_loopback_cmd(gas_id_t gas_id);

#ifdef FEATURE_SEGMENT_LOADING
#include "IWCDMA.h"
interface_t *pi_gl1_wcdma = NULL;
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Structure of saved GSM core registers */
typedef struct
{
   uint32 edge_cgc_ena;
} gstmr_saved_gsm_core_registers_type;

#define NO_OF_FRAMES_DELAY_FOR_PANIC_RESET_TRIGGER 3

/* extern'd reference to the SRCH compressed mode handler */
/* which WCDMA L1 will register with this GTMRS module */
extern void srhcm_gsm_frame_tick_handler(sys_modem_as_id_e_type as_id);

#ifdef FEATURE_GSM_TO_LTE
void lte_ml1_md_gsm_tick(sys_modem_as_id_e_type as_id );
#endif

void gstmr_isr( void );

#if defined ( FEATURE_DUAL_SIM )
void gstmr_g1_isr( void );

#if defined ( FEATURE_TRIPLE_SIM )
void gstmr_g2_isr( void );
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM */

uint32 calc_sync_gstmr_tick( uint32 *ustmr, gas_id_t gas_id );
uint32 gstmr_get_ota_ustmr_start_time( gas_id_t gas_id );
uint32 gstmr_get_ota_ustmr_start_time_sw_fn(uint8 offset,  gas_id_t gas_id );


static void gstmr_fn_handler(sys_modem_as_id_e_type as_id);

static void gstmr_exec_handlers( gas_id_t gas_id);

/* The UE is in GSM Mode or not. */
extern boolean l1_check_gsm_state( gas_id_t gas_id );

#if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT)
extern boolean l1_check_for_wtog_activity( gas_id_t gas_id );
#endif

#ifdef FEATURE_GSM_ISR_PROFILING
extern void log_profiling(gas_id);
#endif /* FEATURE_GSM_ISR_PROFILING */

/* Store the current executing handler, -2 if not in the ISR.  -1 is not
   used because it is a valid value if we remove the 0th entry. */
#define GSTMR_NOT_EXECUTING (-2)

#ifdef GSTMR_HW_FRAME_COUNT
static void gstmr_align_hw_frame_count (void);
#endif

/* Update all FNMOD counters */
static void GSTMR_ALIGN_FNMOD_COUNTERS( gas_id_t gas_id );

#ifdef FEATURE_GSTMR_TRACE
static void gstmr_trace_record (void);
#endif

#ifdef FEATURE_GERAN_VP
void tramp_clear_interrupt_vp (unsigned int interruptNum);
boolean tramp_is_interrupt_pending_vp (unsigned int interruptNum);
void vp_isr_callback(void);
void  tramp_set_isr_vp(unsigned int interruptNum, void (*fp) (void));
void tramp_isr_common_init_vp (void);
#endif

#ifdef FEATURE_GSM_TDS
extern gl1_hw_x2g_startup_info_t gl1_hw_x2g_startup_info;
#endif

/* handle used for timetick services */
extern DalDeviceHandle *gsmtt_handle[];
static uint64 gl1_get_daltick ( gas_id_t gas_id );

gtmrs_data_t gtmrs_data[NUM_GERAN_DATA_SPACES];

supporting_frame_counters_t frame_counters[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION gstmr_init_static_vars

DESCRIPTION
  Sets up static data for GTMR module at power up

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void gstmr_init_static_vars(gas_id_t gas_id)
{
  static boolean gstmr_static_var_init[NUM_GERAN_DATA_SPACES] =
                   { INITIAL_VALUE( FALSE ) };

  if (gstmr_static_var_init[gas_id] == FALSE)
  {
#ifdef FEATURE_MAINTAIN_GTIME_IN_WCDMA_DEBUG
    #error code not present
#endif

    gtmrs_data[gas_id].gstmr_gsm_core_initialized = FALSE;
    gtmrs_data[gas_id].gstmr_initialized = FALSE;
    gtmrs_data[gas_id].gstmr_gsm_core_reinit = FALSE;
    gtmrs_data[gas_id].gstmr_reinit = FALSE;
    gtmrs_data[gas_id].gstmr_FN = 0;
    gtmrs_data[gas_id].gstmr_FN_modMaxFN = 0;
    gtmrs_data[gas_id].vstmr_deregister_sw_fn = 0;
    gtmrs_data[gas_id].vstmr_deregister_fn_modMaxFN = 0;

    gtmrs_data[gas_id].num_handlers = 0;
    gtmrs_data[gas_id].gstmr_delay_qs = 0;

    gtmrs_data[gas_id].last_oe = 0;
    gtmrs_data[gas_id].init_odd_even_check = TRUE;
    gtmrs_data[gas_id].gstmr_last_start_sleep_was_ok = TRUE;
    gtmrs_data[gas_id].gstmr_asynch_int_executing    = FALSE;
    gtmrs_data[gas_id].gtmrs_hw                      = GSTMR_NONE;
    gtmrs_data[gas_id].gstmr_initialized             = FALSE;
    gtmrs_data[gas_id].gstmr_disabled_for_sleep      = FALSE;
    gtmrs_data[gas_id].drift_qs                      = 0;
    gtmrs_data[gas_id].gl1_handler_timestamp.last    = 0;
    gtmrs_data[gas_id].gl1_hw_sleep_gts_gstmr_fired  = FALSE;

    gtmrs_data[gas_id].UstmrSleepStartTime           = 0;
    gtmrs_data[gas_id].GstmrSleepStartTime           = 0;
    gtmrs_data[gas_id].GsmFNSleepStart               = 0;

    memset(gtmrs_data[gas_id].gstmr_handlers,NULL,sizeof(gtmrs_data[gas_id].gstmr_handlers));

    /* set flag to indicate init is done*/
    gstmr_static_var_init[gas_id] = TRUE;
  }
}

/*===========================================================================

FUNCTION GSTMR_REGISTER_HANDLER

DESCRIPTION
   This function allows the application code to register up to
   MAX_NUM_REGISTRATIONS TDMA handlers.

   NOTE:  MAX_NUM_REGISTRATIONS is (MAX_NUM_ENTRIES-1) by design.  This
   is such so that there is always one NULL entry in the table of handlers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_register_handler (
#ifdef FEATURE_DUAL_SIM
   gstmr_handler_type handler_ptr, sys_modem_as_id_e_type as_id
#else
   gstmr_handler_multirat_type handler_ptr
#endif /*FEATURE_DUAL_SIM */
                             )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   /** In case FEATURE_DUAL_SIM is not defined, casting pointer to
      function containing no parameter with a pointer to function
      containing one argument. It is not as per C standard, but
      calling a function with a parameter which is not expecting
      any parameter won't cause any crash and the parameter passed
      as an argument will be ignored.
     */
   return gstmr_register_handler_geran((gstmr_handler_type)handler_ptr,as_id);
}
/*===========================================================================

FUNCTION GSTMR_REGISTER_HANDLER GERAN

DESCRIPTION
   This function allows the application code to register up to
   MAX_NUM_REGISTRATIONS TDMA handlers.

   NOTE:  MAX_NUM_REGISTRATIONS is (MAX_NUM_ENTRIES-1) by design.  This
   is such so that there is always one NULL entry in the table of handlers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_register_handler_geran (gstmr_handler_type handler_ptr,sys_modem_as_id_e_type as_id)
{
   gtmrs_data_t *gtmrs_data_p;
   gas_id_t      gas_id = geran_map_nas_id_to_gas_id(as_id);
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gtmrs_data_p = &gtmrs_data[as_idx];

   /* Make sure handler is valid */
   if ( !handler_ptr )
   {
      MSG_GERAN_ERROR_0_G("Cannot register null gstmr handler");
      return;
   }

   GDRV_ISR_SAVE_LOCK(gas_id);

   if ( gtmrs_data_p->num_handlers >= MAX_NUM_REGISTRATIONS )
   {
      MSG_GERAN_ERROR_2_G("gstmrs_handler table full (%d) for 0x%08x",
                gtmrs_data_p->num_handlers, handler_ptr);
      GDRV_ISR_SAVE_UNLOCK(gas_id);
      return;
   }

   /* Make sure the handler is not already registered */
   if (gstmr_is_handler_registered_geran(handler_ptr, as_id))
   {
      MSG_GERAN_ERROR_1_G("Handler 0x%08x already registered", handler_ptr);
      GDRV_ISR_SAVE_UNLOCK(gas_id);
      return;
   }
   else
   {
     /* Add the handler */
     gtmrs_data_p->gstmr_handlers[gtmrs_data_p->num_handlers++] = handler_ptr;
     MSG_GERAN_HIGH_3_G("Add gstmrs_handler 0x%08x, as_id %d, num_handlers %d", handler_ptr, as_id, gtmrs_data_p->num_handlers );
   }

   GDRV_ISR_SAVE_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION GSTMR_DEREGISTER_HANDLER

DESCRIPTION
   This function allows the application code to remove an existing handler.
   If this handler is successfully removed all handlers that followed it
   are moved up one notch in the handler table.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_deregister_handler(
#ifdef FEATURE_DUAL_SIM
   gstmr_handler_type handler_ptr, sys_modem_as_id_e_type as_id
#else
   gstmr_handler_multirat_type handler_ptr
#endif /*FEATURE_DUAL_SIM */
                              )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   /** In case FEATURE_DUAL_SIM is not defined, casting pointer to
      function containing no parameter with a pointer to function
      containing one argument. It is not as per C standard, but
      calling a function with a parameter which is not expecting
      any parameter won't cause any crash and the parameter passed
      as an argument will be ignored.
     */
   gstmr_deregister_handler_geran((gstmr_handler_type)handler_ptr, as_id);
   return;
}

/*===========================================================================

FUNCTION GSTMR_DEREGISTER_HANDLER GERAN

DESCRIPTION
   This function allows the application code to remove an existing handler.
   If this handler is successfully removed all handlers that followed it
   are moved up one notch in the handler table.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_deregister_handler_geran( gstmr_handler_type handler_ptr,sys_modem_as_id_e_type as_id )
{
   uint16  handler_idx;
   boolean handler_found = FALSE;

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   /* Null is not a valid handler */
   if ( !handler_ptr )
   {
      MSG_GERAN_ERROR_0_G("Cannot deregister null gstmr handler");
      return;
   }

   /* Lock interrupts to prevent gstmr from running */
   GDRV_ISR_SAVE_LOCK(gas_id);

#if 0
   if ( gtmrs_data[gas_id].num_handlers >= MAX_NUM_REGISTRATIONS )
   {
      MSG_GERAN_ERROR_2_G( "gstmrs_handler table full (%d) for 0x%08x",
                 gtmrs_data[gas_id].num_handlers, handler_ptr);

      GDRV_ISR_SAVE_UNLOCK();

      return;
   }
#endif

   /* Search the handler table for a match */
   for (handler_idx = 0; handler_idx < gtmrs_data[gas_id].num_handlers; handler_idx++)
   {
      if (gtmrs_data[gas_id].gstmr_handlers[handler_idx] == handler_ptr)
      {
        handler_found = TRUE;
        break;
      }
   }

   if (!handler_found)
   {
      MSG_GERAN_ERROR_2_G("Handler 0x%08x not found (%d handlers)", handler_ptr,gtmrs_data[gas_id].num_handlers);
      GDRV_ISR_SAVE_UNLOCK(gas_id);
      return;
   }
   else
   {
      MSG_GERAN_HIGH_3_G("Remove gstmrs_handler 0x%08x, index %d, as_id %d", handler_ptr, handler_idx, as_id  );
   }

   /* If we are currently executing gstmr handlers we need to adjust the
      execution index if we are removing the current or earlier one. */
   if (gtmrs_data[gas_id].gstmr_handler_executing != GSTMR_NOT_EXECUTING)
   {
      if (handler_idx <= gtmrs_data[gas_id].gstmr_handler_executing)
      {
         gtmrs_data[gas_id].gstmr_handler_executing--;
      }
      if (handler_idx <= gtmrs_data[gas_id].last_handler)
      {
         gtmrs_data[gas_id].last_handler--;
      }
   }

   /* Move all those that follow down one notch */
   for (; handler_idx < gtmrs_data[gas_id].num_handlers; handler_idx++)
   {
      gtmrs_data[gas_id].gstmr_handlers[handler_idx] = gtmrs_data[gas_id].gstmr_handlers[handler_idx + 1];

   }

   /* Keep unused entries as NULL for safety */
   gtmrs_data[gas_id].gstmr_handlers[gtmrs_data[gas_id].num_handlers] = NULL;

   /* Decrement number of active handlers */
   gtmrs_data[gas_id].num_handlers--;

   GDRV_ISR_SAVE_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gstmr_is_handler_registered

DESCRIPTION
   This function checks the handler table to see if the requested handler
   is present.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if requested handler is present
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_is_handler_registered(
#ifdef FEATURE_DUAL_SIM
   gstmr_handler_type handler_ptr , sys_modem_as_id_e_type as_id
#else
   gstmr_handler_multirat_type handler_ptr
#endif /*FEATURE_DUAL_SIM*/
)
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM */

   /** In case FEATURE_DUAL_SIM is not defined, casting pointer to
      function containing no parameter with a pointer to function
      containing one argument. It is not as per C standard, but
      calling a function with a parameter which is not expecting
      any parameter won't cause any crash and the parameter passed
      as an argument will be ignored.
     */
   return gstmr_is_handler_registered_geran((gstmr_handler_type)handler_ptr,as_id);
}

/*===========================================================================

FUNCTION gstmr_is_handler_registered_geran

DESCRIPTION
   This function checks the handler table to see if the requested handler
   is present.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if requested handler is present
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_is_handler_registered_geran( gstmr_handler_type handler_ptr,sys_modem_as_id_e_type as_id )
{
   uint16  handler_idx;
   boolean handler_found = FALSE;

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   /* Null is not a valid handler */
   if ( !handler_ptr )
   {
      return FALSE;
   }

   GDRV_ISR_LOCK(gas_id);

   /* Search the handler table for a match */
   for (handler_idx = 0; handler_idx < gtmrs_data[gas_id].num_handlers; handler_idx++)
   {
      if (gtmrs_data[gas_id].gstmr_handlers[handler_idx] == handler_ptr)
      {
        handler_found = TRUE;
        break;
      }
   }

   GDRV_ISR_UNLOCK(gas_id);

   return(handler_found);
}



#ifdef DEBUG_GSTMR_ADJUST_TRACE
/*===========================================================================

FUNCTION GSTMR_ADJUST_TRACE

DESCRIPTION
   This function adds a new element to the gstmr adjust trace buffer.

===========================================================================*/
void gstmr_adjust_trace (int16 new_tc_adjust, int32 new_tc_set,gas_id_t gas_id)
{
   gtmrs_data[gas_id].gstmr_adjust_trace_buf[gtmrs_data[gas_id].gstmr_adjust_trace_index].FN = GSTMR_GET_FN_GERAN(gas_id);
   gtmrs_data[gas_id].gstmr_adjust_trace_buf[gtmrs_data[gas_id].gstmr_adjust_trace_index].tc_set = new_tc_set;
   gtmrs_data[gas_id].gstmr_adjust_trace_buf[gtmrs_data[gas_id].gstmr_adjust_trace_index].tc_adjust = new_tc_adjust;
   if (++gtmrs_data[gas_id].gstmr_adjust_trace_index >= MAX_GSTMR_ADJUST_TRACE)
   {
       gtmrs_data[gas_id].gstmr_adjust_trace_index = 0;
   }
}
#endif



/*===========================================================================

FUNCTION GSTMR_DISABLE_INTERRUPT

DESCRIPTION
   This function disables the gstmr interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_disable_interrupt (gtmrs_hw_t gtmrs_hw)
{
#ifdef FEATURE_GERAN_VP
   tramp_set_isr_vp(TRAMP_GSM_MICRO_ISR, NULL);
#endif /* FEATURE_GERAN_VP */
}

/*===========================================================================

FUNCTION GSTMR_ENABLE_INTERRUPT

DESCRIPTION
   This function enables the gstmr interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_enable_interrupt (gtmrs_hw_t gtmrs_hw)
{
#ifdef FEATURE_GERAN_VP
   tramp_set_isr_vp(TRAMP_GSM_MICRO_ISR, gstmr_isr);
#endif
}
/*===========================================================================

FUNCTION GSTMR_CLEAR_INTERRUPT

DESCRIPTION
   This function clears any pending gstmr interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_clear_interrupt (gtmrs_hw_t gtmrs_hw)
{
#ifdef FEATURE_GERAN_VP
   tramp_clear_interrupt_vp(TRAMP_GSM_MICRO_ISR);
#endif
}

/*===========================================================================

FUNCTION GSTMR_SLEEP_INIT

DESCRIPTION
   This function modifies necessary registers before going to sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_sleep_init(
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id
#else
    void
#endif /*FEATURE_DUAL_SIM*/
   )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM */
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   /* Store FN before deregistering VSTMR */
   vstmr_snapshot_before_vstmr_deregister (gas_id);
   /* Deregister VSTMR */
   vstmr_geran_vstmr_deregister ( gas_id );
}

/*===========================================================================

FUNCTION GSTMR_WAKEUP_INIT

DESCRIPTION
   This function modifies necessary registers after in wake-up

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_wakeup_init(
#ifdef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id
#else
    void
#endif /*FEATURE_DUAL_SIM*/
   )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM */

   vstmr_x2g_wakeup_init(as_id);
}

/*===========================================================================

FUNCTION GSTMR_WAKEUP_INIT_GERAN

DESCRIPTION
   This function modifies necessary registers after in wake-up

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_wakeup_init_geran( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  vstmr_wake_up ( gas_id );
  vstmr_schedule_event ( gas_id, 0, TRUE );

  gtmrs_data[gas_id].gstmr_disabled_for_sleep = FALSE;

  /* Always clear to avoid freezing out the gstmr */
  gstmr_set_asynch_int_executing( FALSE, gas_id );

  /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
  gl1_drx_set_pch_offset( 0, gas_id );

  gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );

  gtmrs_data[gas_id].init_odd_even_check = TRUE;

  reset_vstmr_FN_modMaxFN(gas_id);
}

/*===========================================================================

FUNCTION GSTMR_CONFIG_HW
DESCRIPTION
   This function configures the GSTMR MTC HW to enable the GSTMR interupt and calls
   the DAL Interrupt controller to associated the interrupt with the sw handler

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_config_hw(gtmrs_hw_t gtmrs_hw,gas_id_t gas_id)
{
#ifdef FEATURE_GERAN_VP
  tramp_isr_common_init_vp();
#else
  vstmr_enable_interrupt(gtmrs_hw, gas_id);
#endif
}

/*===========================================================================

FUNCTION GSTMR_INIT

DESCRIPTION
   This function establishes an interrupt handler for the GSTMR interrupt
   source.  It then enables the GSTMR interrupt and aligns its
   occurrence such that it is 1 symbol delayed from PP frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_init( gas_id_t gas_id )
{
  gtmrs_data_t *gtmrs_data_p;

  sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );
  int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  gtmrs_data_p = &gtmrs_data[as_idx];

  gstmr_init_static_vars(gas_id);

#ifdef FEATURE_SEGMENT_LOADING
  /*
  ** create only once
  */
  if (geran_is_multi_mode_gas_id(gas_id) && (pi_gl1_wcdma == NULL))
  {
    if(create_instance(CLSID_WCDMA, &pi_gl1_wcdma)!=E_SUCCESS)
    {
      MSG_GERAN_ERROR_0_G("GL1:Failed to create instance of CLSID_WCDMA");
    }
  }
#endif

  /* Hard assign the GSTMR HW for this gas_id */
  gtmrs_data_p->gtmrs_hw = gstmr_determine_hw_id( gas_id );

  GDRV_ISR_LOCK( gas_id );

  /* Always clear to avoid freezing out the gstmr */
  gstmr_set_asynch_int_executing( FALSE, gas_id );

  /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
  gl1_drx_set_pch_offset( 0, gas_id );

  gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );

  /* Only initialize the gstmr once */
  if (!gtmrs_data_p->gstmr_initialized)
  {
    uint16        i;

    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    /* configure the HW for this GSTMR */
    gstmr_config_hw( gtmrs_data_p->gtmrs_hw, gas_id );

    /* Init the odd/even check since the gstmr has been reset */
    gtmrs_data_p->init_odd_even_check = TRUE;

    /* Reset for first frame handler registration */
    gtmrs_data_p->num_handlers = 0;

    /* Clear out to avoid stale entries */
    for ( i = 0; i < ARR_SIZE( gtmrs_data[gas_id].gstmr_handlers ); i++ )
    {
      gtmrs_data_p->gstmr_handlers[i] = NULL;
    }

    /* Register the GSM frame number handler (if we are forcing a re-init
       it may actually already be installed) */
    if (!gstmr_is_handler_registered_geran(gstmr_fn_handler,as_id))
    {
       /* Init the odd/even check since the handler has not run yet */
       gtmrs_data_p->init_odd_even_check = TRUE;

#ifdef GSTMR_HW_FRAME_COUNT
       /* Align the HW frame counter with our current gstmr FN */
       gstmr_align_hw_frame_count();
#endif

       gstmr_register_handler_geran(gstmr_fn_handler,as_id);
    }
     /*If we are in G mode(not CM mode) re-register tdma_isr handler if not there*/
    if ((gtmrs_data[gas_id].gstmr_reinit) && l1_tskisr_blk->l1_state != L1_INACTIVE_MODE)
    {
       if(!gstmr_is_handler_registered_geran(l1_tdma_1_ISR,as_id))
       {
         MSG_GERAN_ERROR_0_G("ERROR - l1_tdma_1_ISR not registered !!!!.register it forcefully");
         gstmr_register_handler_geran(l1_tdma_1_ISR,as_id);
       }
    }

    gtmrs_data_p->gstmr_initialized = TRUE;
    gtmrs_data_p->gstmr_disabled_for_sleep = FALSE;
  }

  vstmr_wake_up ( gas_id );
  vstmr_schedule_event_and_set_fn(gas_id);

  gtmrs_data_p->gstmr_handler_executing = GSTMR_NOT_EXECUTING;

  gtmrs_data_p->tc_adjust_qs = 0;
  gtmrs_data_p->tc_set       = 0;

  /* Print a message if we re-initialized to help debugging */
  if (gtmrs_data_p->gstmr_reinit)
  {
     MSG_GERAN_HIGH_0_G("GSTMR re-initialized");
     gtmrs_data_p->gstmr_reinit = FALSE;
  }

  GDRV_ISR_UNLOCK( gas_id );
}

/*===========================================================================

FUNCTION GSTMR_GSM_CORE_RESET

DESCRIPTION
   This function effectively resets the entire GSM core.  It does this by
   resetting the hardrware that is operating on the VDSP, MDSP and GSM
   clocks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_gsm_core_reset( void )
{
/* Function not needed if MCPM is defined */
}

/*===========================================================================

FUNCTION GSTMR_FORCE_REINIT

DESCRIPTION
   This function clears the initialized variables, meaning that the next
   time we init the gstmr it will really happen.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_force_reinit (gas_id_t gas_id)
{
   gtmrs_data_t *gtmrs_data_p;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gtmrs_data_p = &gtmrs_data[as_idx];

   if (gtmrs_data_p->gstmr_gsm_core_initialized)
   {
      gtmrs_data_p->gstmr_gsm_core_initialized = FALSE;
      gtmrs_data_p->gstmr_gsm_core_reinit = TRUE;
   }

   if (gtmrs_data_p->gstmr_initialized)
   {
      gtmrs_data_p->gstmr_initialized = FALSE;
      gtmrs_data_p->gstmr_reinit = TRUE;
      gtmrs_data_p->gstmr_disabled_for_sleep = FALSE;
   }
}

/*===========================================================================
FUNCTION      gl1_log_handler_timestamp_event

DESCRIPTION   Write an entry in the local memory event log.

              The log idx is treated with extreme caution to prevent writing
              outside of the event array.

ARGUMENTS     event - id tag for the event


RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void gl1_log_handler_timestamp_event(tag_id tag,gas_id_t gas_id)
{
  /* Pointer to current sleep event */
  gl1_handler_timestamp_event_type    *msg;

  gtmrs_data_t          *gtmrs_data_p;
  sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );
  int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  gtmrs_data_p = &gtmrs_data[as_idx];

  /* Mutex lock while setting msg as gl1_handler_timestamp.last could be updated by 2 threads */
  gstmr_log_mtx_lock(gas_id);
     if(gtmrs_data[gas_id].gl1_handler_timestamp.last > GL1_HAND_PERF_LOG_BUFFER_SIZE)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_2_G("gtmrs_data[%d].gl1_handler_timestamp.last corrupted %d", gas_id, gtmrs_data[gas_id].gl1_handler_timestamp.last);
  }
  /* Circular shared memory log buffer.
     Reached end of allocated space so reset index */
  if(++gtmrs_data_p->gl1_handler_timestamp.last == GL1_HAND_PERF_LOG_BUFFER_SIZE)
  {
    gtmrs_data_p->gl1_handler_timestamp.last = 0;
  }

  /* Get pointer to current event */
  msg = &gtmrs_data_p->gl1_handler_timestamp.events[gtmrs_data_p->gl1_handler_timestamp.last];

  gstmr_log_mtx_unlock(gas_id);

  /* write log data into shared memory buffer with valid idx */
  msg->tag              = tag;

  if(tag == START_GSTMR_ISR)
  {
    msg->qtimer            = qurt_sysclock_get_hw_ticks();

    msg->gl1_timestamp    = timetick_get_safe();
  }
  else
  {
    msg->qtimer            = 0;
    msg->gl1_timestamp    = 0;
  }
  msg->frame_number     = GSTMR_GET_FN_GERAN(gas_id);

  /*read from the GSTMR HW only if it is ON*/
  msg->gl1_qsymcount    = gstmr_hw_on(as_id)? gstmr_rd_qsymbol_count_geran(as_id): 0;

  msg->modMaxFN_frame_num = vstmr_rd_modMaxFN_frame_num(gas_id);

#ifdef FEATURE_GSM_ISR_PROFILING
  gtmrs_data_p->time_result_buffer[msg->tag]= msg->gl1_qsymcount;
#endif /* FEATURE_GSM_ISR_PROFILING*/

}
/*===========================================================================

FUNCTION gl1_performance_log_handler_start_time

DESCRIPTION
   This function logs the handler start time through GL1_PERFORMANCE_LOGGING
   MACRO

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_performance_log_handler_start_time(int16 gstmr_handler_num,gas_id_t gas_id)
{
  if (gstmr_handler_num < 0)
  {
    return;
  }

  if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == l1_tdma_1_ISR)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_GSM_TDMA_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_cm_mode_sw_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_CM_MODE_SW_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_disable_egprs_mode_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_DISABLE_EGPRS_MODE_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_gsac_clk_off_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_GSAC_CLK_OFF_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gstmr_fn_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_GSTMR_FN_ISR,gas_id);
  }
#if defined FEATURE_WCDMA
  else if (gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == srhcm_gsm_frame_tick_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_SRCHCM_GSM_ISR,gas_id);
  }
#endif
#if defined FEATURE_GSM_TO_LTE
  else if (gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == lte_ml1_md_gsm_tick)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(START_SRCHCM_GSM_ISR,gas_id);
  }
#endif
}

/*===========================================================================

FUNCTION gl1_performance_log_handler_stop_time

DESCRIPTION
   This function logs the handler stop time through GL1_PERFORMANCE_LOGGING
   MACRO
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_performance_log_handler_stop_time(int16 gstmr_handler_num,gas_id_t gas_id)
{
  if (gstmr_handler_num < 0)
  {
    return;
  }

  if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == l1_tdma_1_ISR)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSM_TDMA_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_cm_mode_sw_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_CM_MODE_SW_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_disable_egprs_mode_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_DISABLE_EGPRS_MODE_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gl1_hw_gsac_clk_off_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSAC_CLK_OFF_ISR,gas_id);
  }
  else if(gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == gstmr_fn_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_GSTMR_FN_ISR,gas_id);
  }
#if defined FEATURE_WCDMA
  else if (gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == srhcm_gsm_frame_tick_handler)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_SRCHCM_GSM_ISR,gas_id);
  }
#endif
#if defined FEATURE_GSM_TO_LTE
  else if (gtmrs_data[gas_id].gstmr_handlers[gstmr_handler_num] == lte_ml1_md_gsm_tick)
  {
    GL1_HANDLER_PERFORMANCE_LOGGING(STOP_SRCHCM_GSM_ISR,gas_id);
  }
#endif

}
#ifdef FEATURE_GSM_L4_TRACE_BUFF
/*===========================================================================

FUNCTION gl1_find_task_delaying_gstmr_isr_from_tb

DESCRIPTION
   This function finds the task delaying gstmr isr by going through the
   trace buffer


DEPENDENCIES
  This function calls the following api provided by core bsp team to access
  the trace buffer
  L4_MSM_DUMP_TB_TO_USER

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_find_task_delaying_gstmr_isr_from_tb(uint32 qs_start, uint32 start_in_sclks)
{
  uint32 *tb_array = NULL;
  uint32 index  = 0;
  uint32 isr_actual_start_in_sclks  = 0;
  gl1_trace_buffer_type *gl1_local_trace_buffer_ptr = NULL;
  gl1_trace_buffer_type *gl1_trace_buffer_ptr = NULL;

  /* calculate the isr delay in sclks from the qs start*/
  uint32 isr_delay_in_sclks = (qs_start * GSTMR_NO_OF_SYS_CLKS_IN_FRAME)/QS_PER_FRAME;

  /* Find the actual start position of ISR in sclks*/
  isr_actual_start_in_sclks = start_in_sclks - isr_delay_in_sclks;

  tb_array = (uint32 *)(&gl1_trace_buffer);

  /* call L4 api to get the trace buffer*/
  L4_MSM_DUMP_TB_TO_USER((uint32)tb_array, GL1_MAX_NO_TRACE_BUFFER_ENTRIES);

  gl1_trace_buffer_ptr = (gl1_trace_buffer_type *)tb_array;
  gl1_local_trace_buffer_ptr = (gl1_trace_buffer_type *)tb_array;

  /* go through the trace buffer and identify the task which has run before the
     GSM ISR interrupt is fired */
  for(index = 0;index < GL1_MAX_NO_TRACE_BUFFER_ENTRIES; index++)
  {
    if(isr_actual_start_in_sclks > gl1_local_trace_buffer_ptr->timestamp)
    {
      /* identified the task which is runing and this task is taking more time to execute and that delayed
          GSM ISR*/
      MSG_GERAN_ERROR_0_G("GSM ISR delay is the reason for GSM ISR over run.");
      MSG_SPRINTF_2(MSG_SSID_DFLT,
                    MSG_LEGACY_HIGH,
                    "The task caused this GSM ISR delay is %s and it has run for %d ticks",
                    gl1_local_trace_buffer_ptr->taskname,
                    gl1_local_trace_buffer_ptr->tick_duration);
      gl1_local_trace_buffer_ptr --;
      /* print all the tasks delaying the gsm ISR */
      while(gl1_local_trace_buffer_ptr != gl1_trace_buffer_ptr)
      {
        MSG_SPRINTF_2(MSG_SSID_DFLT,
                    MSG_LEGACY_HIGH,
                    "The other task caused this GSM ISR delay is %s and it has run for %d ticks",
                    gl1_local_trace_buffer_ptr->taskname,
                    gl1_local_trace_buffer_ptr->tick_duration);
        gl1_local_trace_buffer_ptr --;
      }
      return;
    }
    gl1_local_trace_buffer_ptr++;
  }
}
#endif

/*===========================================================================

FUNCTION GSTMR_FRAME_TICK_PROCESS

DESCRIPTION
   Runs gstmr functions which are run every frame tick:
     MDSP frame tick processing
     gstmr handlers
     Issue Aync Message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_frame_tick_process( gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   mdsp_frame_tick_process(gas_id);

   /* Execute any registered handlers */
   gstmr_exec_handlers(gas_id);



   if (gl1_msg_get_multi_sim_mode() && (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) 
    && (l1_tskisr_blk->rf_wakeup_pending == TRUE))
   {
     MSG_GERAN_HIGH_0_G("Dont Issue Async Command, as RF wakeup/GFW EnterMode Pending");
   }
   /* In Async case leave this for silent frame if required */
   else if ( !gstmr_get_asynch_int_executing( gas_id ))
   {
     mdsp_issue_async_msg( GFW_ASYNC_CMD, gas_id );
   }

#ifdef FEATURE_GSM_QSC_TX_DIV
   if (antenna_switch_required_at_start_of_next_frame[gas_id] )
   {
   gl1_trm_ant_switch_div_set_config (gas_id);
   antenna_switch_required_at_start_of_next_frame[gas_id] = FALSE ;
   }
#endif /* FEATURE_GSM_QSC_TX_DIV */

   /*Can't use FTick handler for Dynamic Rxd as it is decided during Rx processing 
    and on adding Ftick handler during Rx processing causes mismatch in Ftick handlers
    on executing remaining handlers. Therefore, pass tick to dynamic rxd handler.*/
#ifdef FEATURE_GSM_RX_DIVERSITY
   if (gl1_hw_get_dynamic_rxd_tick_required(gas_id)) 
   {
      gl1_msg_dynamic_rxd_tick(gas_id); 
   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

}

/*===========================================================================

FUNCTION GSTMR_ASYNCH_SILENT_FRAME_ISR

DESCRIPTION
   This is the silent ISR that can run when the Async frame
   tick has executed but the UE has not slept.
   In which case merely exec the FW Asynch message router trigger
   after resynching the FN count etc.
   Also tick DRX incase some new entry exists.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_asynch_silent_frame_isr( gas_id_t gas_id )
{
   boolean       drx_run_isr;
   gtmrs_data_t *gtmrs_data_p;

   boolean       asynch_dbg = gl1_asynch_int_dbg( gas_id );

   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );
   drx_run_isr	= FALSE;

   gtmrs_data_p =
     &gtmrs_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id )];

   /* Clear here so next asynch is treated cleanly as factored into DRX calculation */
   gl1_drx_set_pch_offset( 0, gas_id );

   gtmrs_data_p->gstmr_qs_start    = gstmr_rd_qsymbol_count_geran( as_id );
   gtmrs_data_p->gstmr_ustmr_start = slpc_get_tick();

   if ( asynch_dbg )
   {
     MSG_GERAN_HIGH_4_G( "Asynch Int silent frame GSTMR execute S/W FN:%d start:%dqs gstmr_ustmr_start:%d H/W FN:%d",
                         GSTMR_GET_FN_GERAN( gas_id ), gtmrs_data_p->gstmr_qs_start,
                         gtmrs_data_p->gstmr_ustmr_start, gstmr_rd_hw_frame_count_geran( as_id ) );
   }

   /* Need to realign the HW FN for the frame after a failed asynch int */
   gtmrs_data_p->gstmr_FN_modMaxFN = vstmr_rd_modMaxFN_frame_num(gas_id);

   /* Need to realign the OE check info for the frame after a failed asynch int */
   GSTMR_ENABLE_ODD_EVEN_CHECK_GERAN( as_id );

   /* Issue any pending firmware command from previous async
    * avoid running DRX in this case as F/W processing of this command
    * required so avoid any possible sleep
    */
   if ( mdsp_commands_pending( gas_id ) )
   {
     mdsp_issue_async_msg( GFW_ASYNC_CMD, gas_id );
   }
   else if ( !gl1_drx_get_next_tick_req( gas_id ) )
   {
     /*
      * A failure to sleep during asynch int on previous frame so try at start of this frame
      * to try and avoid the extra frame delay. Providing the Next Tick flag has not been set.
      * Must be run after we have realigned the s/w and h/w frame numbers so any calculations
      * are correct in the drx goto sleep.
      */
     drx_run_isr = gl1_drx_tick( gas_id );

     /*
      * No ISR to run here as already run in previous asynch processing.
      * Also no gl1_drx_post_tick required as ISR not run, no extra
      * ISR based events posted so nothing for post_tick to check
      */
   }

   /* Clear here to avoid freezing out gstmr */
   gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );

   if ( asynch_dbg )
   {
     /* Check if DRX_ASLEEP in which case gstmr is OFF */
     if ( !gl1_drx_is_gsm_asleep( gas_id ) )
     {
       MSG_GERAN_HIGH_4_G( "Asynch silent frame realign FN end:%dqs gstmr_ustmr_end:%d H/W FN:%d drx_tick_isr %d",
                           gstmr_rd_qsymbol_count_geran( as_id ), slpc_get_tick(),
                           gstmr_rd_hw_frame_count_geran( as_id ), drx_run_isr );
     }
     else
     {
       MSG_GERAN_HIGH_3_G( "Asynch silent frame realign FN DRX_Asleep gstmr_ustmr_end:%d H/W FN:%d drx_tick_isr %d",
                           slpc_get_tick(), gstmr_rd_hw_frame_count_geran( as_id ), drx_run_isr );
     }
   }

   return;
}


gas_id_t gstmr_determine_gas_id( int gtmrs_hw )
{
   gas_id_t gstmr_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

   /* Check which GSTMR HW block this handler was fired on*/
   switch ( gtmrs_hw )
   {
     case GSTMR_0:
       gstmr_gas_id = GERAN_ACCESS_STRATUM_ID_1;
     break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case GSTMR_1:
       gstmr_gas_id = GERAN_ACCESS_STRATUM_ID_2;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GSTMR_2:
       gstmr_gas_id = GERAN_ACCESS_STRATUM_ID_3;
     break;
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   }

   return ( gstmr_gas_id );
}

int gstmr_determine_hw_id( gas_id_t gas_id )
{
   int gstmr_hw_id = GSTMR_0;

   /* Check which GSTMR HW block this handler was fired on*/
   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
       gstmr_hw_id = GSTMR_0;
     break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_2:
       gstmr_hw_id = GSTMR_1;
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       gstmr_hw_id = GSTMR_2;
     break;
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
     default:
       MSG_GERAN_FATAL_0_G( "Unknown GSTMR config" );
     break;
   }

   return ( gstmr_hw_id );
}

/*===========================================================================

FUNCTION wakeup_gstmr_isr

DESCRIPTION
   Fake GSTMR ISR on wake up. Runs gstmr functions which are run every frame tick:
  MDSP frame tick processing
  gstmr handlers
  Issue Aync Message


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void wakeup_gstmr_isr( gas_id_t gas_id )
{
   mdsp_frame_tick_process( gas_id );

   /* Execute any registered handlers */
   gstmr_exec_handlers( gas_id );
}

/*===========================================================================

FUNCTION wakeup_check_isr_overrun

DESCRIPTION
  Check if too much into frame and avoid giving commands to mdsp to prevent ISR overruns

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean wakeup_check_isr_overrun(gas_id_t gas_id)
{
   slpc_id_type              slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );
   uint64                    ols_tick = slpc_get_ols_tick( slpc_from_gas );
   uint64                    curr_tick = 0;
   uint64                    margin_tick = 8640; // for DSDS/SS/TSTS modes ~0.45ms margin before the frametick.

   /* In DSDA mode or when in DSDS/TSTS and not LMM mode, then we use 2 TS to avoid ISR overrun crashes */
   if( (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) ||
       ( !gl1_hw_ncell_pch_low_monitor_active(gas_id) && gl1_msg_get_multi_sim_standby_mode())
     )
   {
      /* GFW should be sent commands ~1200qs (~1.1ms) before OLS tick or ~1800qs before
         Sync Rx gets scheduled (601qs in next frame) */
      margin_tick = 26880; //1.3ms in USTMR ticks
   }
   /* If the GSTMR already fired in warm up ISR (pending to run), then we needn't send any commands to the mDSP
       we rely on the fact that 2/3/4th bursts would be decoded properly. So, we flush the current buffer. */
   curr_tick = slpc_get_tick();
   if ((curr_tick + margin_tick) > ols_tick )
   {
      MSG_GERAN_ERROR_2_G("ISR OVERRUN: from warmup ISR, curr_tick - %d, ols_tick - %d",slpc_get_tick(), slpc_get_ols_tick(slpc_from_gas));

      return TRUE;
   }

   return FALSE;
}

/*===========================================================================

FUNCTION GSTMR_EXEC_HANDLERS

DESCRIPTION
   Executes any registered GSTMR handlers.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gstmr_exec_handlers( gas_id_t gas_id )
{
   gtmrs_data_t          *gtmrs_data_p;
   int                    as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id( gas_id );

   gtmrs_data_p = &gtmrs_data[as_idx];

   /* Invoke the registered handler functions for the TDMA time strobe */
   gtmrs_data_p->gstmr_handler_executing = 0;

   if ( gtmrs_data_p->num_handlers > MAX_NUM_ENTRIES )
   {
     uint32 num_handlers_idx = 0;

     MSG_GERAN_ERROR_1_G( "Invalid num hdlrs %d", gtmrs_data_p->num_handlers );

     while( num_handlers_idx < MAX_NUM_ENTRIES )
     {
       if ( NULL == gtmrs_data_p->gstmr_handlers[num_handlers_idx] )
       {
         break;
       }

       num_handlers_idx++;
     }

     MSG_GERAN_ERROR_2_G("invalid num hdlrs %d correct to %d", gtmrs_data_p->num_handlers, num_handlers_idx );

     gtmrs_data_p->num_handlers = num_handlers_idx;
   }

   gtmrs_data_p->last_handler            = (int16)( gtmrs_data_p->num_handlers - 1 );

   while ( gtmrs_data_p->gstmr_handler_executing <= gtmrs_data_p->last_handler )
   {
      /* Sanity check - should never happen */
      if ( !gtmrs_data_p->gstmr_handlers[gtmrs_data_p->gstmr_handler_executing] )
      {
         MSG_GERAN_FATAL_3_G( "NULL GSTMR handler %d num:%d last:%d",
           gtmrs_data_p->gstmr_handler_executing, gtmrs_data_p->num_handlers, gtmrs_data_p->last_handler );

         break;
      }

      /* Put this after any dual sim FN adjustment above */
      if(!l1_check_for_panic_reset(gas_id))
      {
        gl1_performance_log_handler_start_time( gtmrs_data_p->gstmr_handler_executing,gas_id );

        /* Execute the handler */
        gtmrs_data_p->gstmr_handlers[gtmrs_data_p->gstmr_handler_executing](as_id);

        gl1_performance_log_handler_stop_time( gtmrs_data_p->gstmr_handler_executing,gas_id );
      }
      else
      {
        /* only execute the fn handler */
        if(gtmrs_data_p->gstmr_handlers[gtmrs_data_p->gstmr_handler_executing] == gstmr_fn_handler)
        {
          gl1_performance_log_handler_start_time( gtmrs_data_p->gstmr_handler_executing,gas_id );
          /* Execute the handler */
          gtmrs_data_p->gstmr_handlers[gtmrs_data_p->gstmr_handler_executing](as_id);
          gl1_performance_log_handler_stop_time( gtmrs_data_p->gstmr_handler_executing,gas_id );
        }
      }

      GSTMR_TRACE( gtmrs_data_p->gstmr_handler_executing );

      gtmrs_data_p->gstmr_handler_executing++;
   }

   /* Add any pending loopback command */
   gl1_hw_issue_mdsp_loopback_cmd(gas_id);

   /* Reset flag */
   gtmrs_data_p->gstmr_handler_executing = GSTMR_NOT_EXECUTING;
}

/*===========================================================================

FUNCTION gstmr_asynch_dsp_int

DESCRIPTION
   This function is the asynch dsp interrupt handler. Only allowed to execute
   if we have time before the next real GSTMR interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_asynch_dsp_int( gas_id_t gas_id )
{
   boolean                asynch_dbg = gl1_asynch_int_dbg( gas_id );
   sys_modem_as_id_e_type as_id      = geran_map_gas_id_to_nas_id( gas_id );

   GDRV_ISR_LOCK( gas_id );

   if ( asynch_dbg )
   {
     MSG_GERAN_HIGH_2_G( "GSTMR start asynch int FN:%d %dqs",
                         GSTMR_GET_FN_GERAN( gas_id ),
                         gstmr_rd_qsymbol_count_geran( as_id ) );
   }

   /*
    * Make sure do not have a pending gstmr and we are not too close
    * to the next real gstmr interrupt to avoid updating hw/sw counts
    * just before a real gstmr comes in
    */
   if ( gl1_hw_is_asynch_int_possible( gas_id ) )
   {
     /* Set before we exec handlers to allow for FN handling changes */
     gstmr_set_asynch_int_executing( TRUE, gas_id );

     /* Avoids first frame appearing as an mdsp stall */
     mdsp_set_last_debugftick( gas_id );

     /*
      * This is used to tell DRX how many extra frames the gap will be
      * and also home many frames to shorten the wakeup frame number by
      */
     gl1_drx_set_pch_offset( 1, gas_id );

     gl1_ustmr_gstmr_snapshot_before_sleep( gas_id );

     /* Will call the normal FN handler so SW/HW FN will be incremented */
     gstmr_frame_tick_process( gas_id );

     /* Set this so that next gstmr is ignored as all ISR processing already performed */
     gl1_drx_set_asynch_int_ignore_gstmr( TRUE, gas_id );

     if ( asynch_dbg )
     {
       MSG_GERAN_HIGH_3_G( "GSTMR finish asynch int FN:%d %dqs asynch_int_exec:%d",
                           GSTMR_GET_FN_GERAN( gas_id ),
                           gstmr_rd_qsymbol_count_geran( as_id ),
                           gstmr_get_asynch_int_executing( gas_id ) );
     }
   }
   else
   {
     gstmr_set_asynch_int_executing( FALSE, gas_id );

     /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
     gl1_drx_set_pch_offset( 0, gas_id );

     if ( asynch_dbg )
     {
       MSG_GERAN_HIGH_3_G( "GSTMR not possible to exec asynch int FN:%d %dqs asynch_int_exec:%d",
                           GSTMR_GET_FN_GERAN( gas_id ),
                           gstmr_rd_qsymbol_count_geran( as_id ),
                           gstmr_get_asynch_int_executing( gas_id ) );
     }
   }

   #ifdef FEATURE_GSM_RX_DIVERSITY
     gl1_msg_process_rxd_active(gas_id);
   #endif /* FEATURE_GSM_RX_DIVERSITY */

   GDRV_ISR_UNLOCK( gas_id );
}

/*===========================================================================

FUNCTION GSTMR_ENABLE_ODD_EVEN_CHECK

DESCRIPTION
   This function enables or disables the odd/even bit check which detects
   missed GSM frame ticks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_enable_odd_even_check (boolean enable
#ifdef FEATURE_DUAL_SIM
                                  , sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM */
                                  )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   gstmr_enable_odd_even_check_geran(enable, as_id);
}

/*===========================================================================

FUNCTION GSTMR_ENABLE_ODD_EVEN_CHECK

DESCRIPTION
   This function enables or disables the odd/even bit check which detects
   missed GSM frame ticks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_enable_odd_even_check_geran (boolean enable
                                  , sys_modem_as_id_e_type as_id
                                  )
{
   gtmrs_data_t *gtmrs_data_p;

   gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gtmrs_data_p = &gtmrs_data[as_idx];

   GDRV_ISR_LOCK(gas_id);

   if (enable)
   {
      gtmrs_data_p->init_odd_even_check = TRUE;
   }
   else
   {
      gtmrs_data_p->gstmr_odd_even_check = FALSE;
   }

   GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION GSTMR_FN_HANDLER

DESCRIPTION
   This function is GSM frame number handler. It increments the frame number
   counts. Rewritten to prevent expensive modulo arithmetic

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_TDS
extern boolean  tdssrchgsm_is_drx_state( void );
#endif

static void gstmr_fn_handler (sys_modem_as_id_e_type as_id)
{
   uint32        current_oe, hw_fn, sw_fn;

   gtmrs_data_t *gtmrs_data_p;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   boolean       can_gl1_disable_wfw_app = TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gtmrs_data_p = &gtmrs_data[as_idx];

   volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
/* setting frame tick signal in suspend mode*/
   if (l1_tskisr_blk->l1_state == L1_SUSPEND_MODE && (geran_get_multi_sim_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM))
   {
     MSG_GERAN_HIGH_0_G("setting frame tick signal in SSIM and in suspend mode from fn handler");
     (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
   }

   /* Check frame number matches what hardware has - must do this before
      calling GSTMR_INC_FN_BY_ONE since that function will re-align them.
      If a mismatch is detected this realignment will let us "recover".
      In future we could try the other way which could allow recovery
      from missed frame ticks - just need to validate the check first. */
   hw_fn = gstmr_rd_hw_frame_count_geran(as_id);
   sw_fn = ADD_FN(gtmrs_data_p->gstmr_FN, 1);

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
   if (!gl1_hw_get_idle_mode(gas_id))
   {
      l1_log_physical_layer_power_info(FALSE, gas_id);
   }
#endif

   gdrivers_FN_mtx_lock(gas_id);

   /*
    * For the asynch int the HW counter hasn't yet flipped as real HW GSTMR
    * has not yet executed so do a sw increment here and align will be called
    * when the real GSTMR interrupt goes off
    */
   if ( gstmr_get_asynch_int_executing(gas_id) || gl1_hw_sleep_get_entered_recovery(gas_id))
   {
     hw_fn = sw_fn;
   }

   if ((hw_fn != sw_fn) && (!gtmrs_data_p->init_odd_even_check))
   {
#if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT)
      if(l1_check_for_wtog_activity(gas_id) == FALSE)
#endif
      {
        MSG_GERAN_ERROR_3_G("Missed %d GSM frames (sw=%d, hw=%d)", SUB_FN(hw_fn, sw_fn), sw_fn, hw_fn);
      }
#if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT)
      else
      {
        MSG_GERAN_ERROR_3_G("Missed %d GSM frames (sw=%d, hw=%d)", SUB_FN(hw_fn, sw_fn), sw_fn, hw_fn);
      }
#endif
   }

   /* ------------------------------------------------------ */
   /*               Master Frame Counters                    */
   /* ------------------------------------------------------ */
   GSTMR_INC_FN_BY_ONE(gas_id);

   gtmrs_data_p->gstmr_FN_modMaxFN = vstmr_rd_modMaxFN_frame_num(gas_id);

#ifdef DEBUG_GSM_GPRS_LED_TEST
   #error code not present
#endif

   /* Check if the odd/even bit has changed - if not we probably missed
      an entire frame tick */
   if(gtmrs_data_p->gstmr_odd_even_check)
   {
      if (gtmrs_data_p->init_odd_even_check)
      {
          gtmrs_data_p->last_oe = vstmr_rd_odd_even(gas_id);
          gtmrs_data_p->init_odd_even_check = FALSE;
      }
      else
      {
          current_oe = vstmr_rd_odd_even(gas_id);

         /* Ensure as actual HW GSTMR has not occurred then odd/even bit check is invalid */
         if ( gstmr_get_asynch_int_executing(gas_id) )
         {
           /* We expect the current oe bit to be the same as lst oe value for asynch int */
           if(  gtmrs_data_p->last_oe != current_oe )
           {
              MSG_GERAN_ERROR_1_G( "Asynch int GSM ftick parity error FN:%d",
                         gtmrs_data_p->gstmr_FN);

              /* Increment the frame counters to compensate
               * for this lost frame and increase chances of
               * recovery.
               */
              GSTMR_INC_FN_BY_ONE(gas_id);
           }
         }
         else
         {
           if (current_oe == gtmrs_data_p->last_oe)
           {
              MSG_GERAN_ERROR_1_G( "GSM ftick parity error FN:%d",
                         gtmrs_data_p->gstmr_FN);

              /*multimode_sub*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_WCDMA
#ifdef FEATURE_SEGMENT_LOADING
              if (pi_gl1_wcdma != NULL)
              {
                can_gl1_disable_wfw_app = IWCDMA_wl1trm_can_gsm_disable_fw_app_wcdma(pi_gl1_wcdma);
              }
#else
              can_gl1_disable_wfw_app = wl1trm_can_gsm_disable_fw_app_wcdma();
#endif
#endif
              if ((gas_id == GERAN_ACCESS_STRATUM_ID_1) &&
                    !can_gl1_disable_wfw_app)
              {
                MSG_GERAN_ERROR_0_G( "Don't adjust sw_fn as W is active");
              }
              else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
              {
                /* Increment the frame counters to compensate
                 * for this lost frame and increase chances of
                 * recovery.
                 */
                GSTMR_INC_FN_BY_ONE(gas_id);
              }
            }
         }

         gtmrs_data_p->last_oe = current_oe;
      }
   }


   gdrivers_FN_mtx_unlock(gas_id);
}

/*===========================================================================

FUNCTION GSTMR_ADJUST_TERMINAL_COUNT

DESCRIPTION
  This function elongates the current frame, so expect the next TDMA tick to
  occur a little later than usual.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gstmr_adjust_terminal_count (int16 quarter_symbol_count
#ifdef FEATURE_DUAL_SIM
                                  ,sys_modem_as_id_e_type as_id
#endif /*FEATURE_DUAL_SIM*/
                                  )
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   vstmr_adjust_terminal_count_geran(quarter_symbol_count,as_id);

   return;
}

/*
 * Read symbol counter
 */
uint32 gstmr_rd_qsymbol_count(
#ifdef FEATURE_DUAL_SIM
     sys_modem_as_id_e_type as_id
#else
     void
#endif /*FEATURE_DUAL_SIM*/
     )
  {
#ifndef FEATURE_DUAL_SIM
     sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
     return gstmr_rd_qsymbol_count_geran(as_id);
  }


uint32 gstmr_rd_symbol_count( gas_id_t gas_id )
{
   return ( gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id(gas_id) ) >> 2 );
}

/*===========================================================================

FUNCTION GSTMR_RD_QSYMBOL_COUNT_GERAN

DESCRIPTION
  This function returns the qsymbol count.

DEPENDENCIES
  HW must be awake, onlywise call to vstmr function is non-terminating.

RETURN VALUE
  int32 qsymbol count

SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_rd_qsymbol_count_geran( sys_modem_as_id_e_type as_id )
{
   uint32 gsm_time = 0;
   vstmr_gstmr_t  current_gstmr;
   int            as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(geran_map_nas_id_to_gas_id(as_id));

   if (gtmrs_data[as_idx].geran_view != NULL)
   {
     current_gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );
     gsm_time = ( current_gstmr.raw_13M * G_STMR_RAW_13MHZ_TO_QSYM ) >> 16;
   }

   return ( gsm_time );
}


/*===========================================================================

FUNCTION GSTMR_GET_FN

DESCRIPTION
   This function retrieves the GSTMR frame number.

DEPENDENCIES
  None

RETURN VALUE
  uint32  -- the framenumber

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_SIM
uint32 GSTMR_GET_FN(sys_modem_as_id_e_type as_id)
#else
uint32 GSTMR_GET_FN(void)
#endif /*FEATURE_DUAL_SIM*/
{
#ifdef FEATURE_DUAL_SIM
  return(GSTMR_GET_FN_GERAN(geran_map_nas_id_to_gas_id(as_id)));
#else
  return (gtmrs_data[GERAN_ACCESS_STRATUM_ID_1].gstmr_FN);
#endif /*FEATURE_DUAL_SIM*/
}


/*===========================================================================

FUNCTION gl1_cm_advance_fn

DESCRIPTION
  Wrapper function for GSTMR_ADVANCE_FN_GERAN

===========================================================================*/
void gl1_cm_advance_fn ( uint32 fn_diff, sys_modem_as_id_e_type as_id )
{
   GSTMR_ADVANCE_FN_GERAN(fn_diff, geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION GSTMR_GET_FN_GERAN

DESCRIPTION
   This function retrieves the GSTMR frame number.

DEPENDENCIES
  None

RETURN VALUE
  uint32  -- the framenumber

SIDE EFFECTS
  None

===========================================================================*/
uint32 GSTMR_GET_FN_GERAN( gas_id_t gas_id )
{
  return (gtmrs_data[gas_id].gstmr_FN);
}
/*===========================================================================

FUNCTION gl1_cm_get_FN

DESCRIPTION
   This function retrieves the GSTMR frame number for external clients use.

DEPENDENCIES
  None

RETURN VALUE
  uint32  -- the framenumber

SIDE EFFECTS
  None

===========================================================================*/

uint32 gl1_cm_get_FN(sys_modem_as_id_e_type as_id)
{
  return(GSTMR_GET_FN_GERAN(geran_map_nas_id_to_gas_id(as_id)));
}

/*===========================================================================

FUNCTION GSTMR_ADVANCE_FN

DESCRIPTION
   This function advances the GSTMR frame number by the specified ammount.
   This is for "slamming" - increments are handled by gstmr_fn_handler()

DEPENDENCIES
  Must be run in ISR context.

RETURN VALUE
  None

SIDE EFFECTS
  1. Adjustment is logged.

===========================================================================*/
void GSTMR_ADVANCE_FN( uint32 fn_diff
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                       ,sys_modem_as_id_e_type as_id
#endif
                      )
{
#if !defined ( FEATURE_DUAL_SIM ) && !defined ( FEATURE_TRIPLE_SIM )
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /* !FEATURE_DUAL_SIM & !FEATURE_TRIPLE_SIM */

   GSTMR_ADVANCE_FN_GERAN(fn_diff, geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION GSTMR_ADVANCE_FN_GERAN

DESCRIPTION
   This function advances the GSTMR frame number by the specified ammount.
   This is for "slamming" - increments are handled by gstmr_fn_handler()

DEPENDENCIES
  Must be run in ISR context.

RETURN VALUE
  None

SIDE EFFECTS
  1. Adjustment is logged.

===========================================================================*/
void GSTMR_ADVANCE_FN_GERAN( uint32 fn_diff, gas_id_t gas_id )
{
   uint32 old_gstmr_FN = gtmrs_data[gas_id].gstmr_FN;

   /* Set the new frame number */
   gtmrs_data[gas_id].gstmr_FN = ADD_FN(gtmrs_data[gas_id].gstmr_FN,fn_diff);

#ifdef GSTMR_HW_FRAME_COUNT
   gstmr_align_hw_frame_count();
#endif

   /* Realign all FNMOD counters */
   GSTMR_ALIGN_FNMOD_COUNTERS( gas_id );

   gl1_hw_log_gfn_adjust( old_gstmr_FN, fn_diff );
}

/*===========================================================================

FUNCTION GSTMR_SET_FN

DESCRIPTION
   This function sets the GSTMR frame number to the specified ammount.
   This is for "slamming" - increments are handled by gstmr_fn_handler()

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  1. Adjustment is logged.

===========================================================================*/
void GSTMR_SET_FN( uint32 new_fn
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                   , sys_modem_as_id_e_type as_id
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                   )
{
#if !defined ( FEATURE_DUAL_SIM ) && !defined ( FEATURE_TRIPLE_SIM )
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /* !FEATURE_DUAL_SIM & !FEATURE_TRIPLE_SIM */

   GSTMR_SET_FN_GERAN(new_fn,geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION GSTMR_SET_FN_GERAN

DESCRIPTION
   This function sets the GSTMR frame number to the specified amount.
   This is for "slamming" - increments are handled by gstmr_fn_handler()

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  1. Adjustment is logged.

===========================================================================*/
void GSTMR_SET_FN_GERAN( uint32 new_fn , gas_id_t gas_id)
{
   uint32 old_gstmr_FN;

#ifdef FEATURE_MAINTAIN_GTIME_IN_WCDMA_DEBUG
   #error code not present
#endif

   /* Prevent frame number increment whilst setting FN */
   GDRV_ISR_LOCK(gas_id);

   old_gstmr_FN = gtmrs_data[gas_id].gstmr_FN;

   /* Set the new frame number */
   gtmrs_data[gas_id].gstmr_FN = new_fn;

#ifdef GSTMR_HW_FRAME_COUNT
   gstmr_align_hw_frame_count();
#endif

   /* Realign all FNMOD counters */
   GSTMR_ALIGN_FNMOD_COUNTERS( gas_id );

   gtmrs_data[gas_id].gstmr_FN_modMaxFN = vstmr_rd_modMaxFN_frame_num(gas_id);

   GDRV_ISR_UNLOCK(gas_id);

   gl1_hw_log_gfn_adjust( old_gstmr_FN, SUB_FN( new_fn, old_gstmr_FN ) );

#ifdef FEATURE_MAINTAIN_GTIME_IN_WCDMA_DEBUG
   #error code not present
#endif
}

void GSTMR_REWIND_FN( uint32 fn_lag , gas_id_t gas_id)
{
  GSTMR_ADVANCE_FN_GERAN( NEG_FN(fn_lag), gas_id );
}

/*===========================================================================

FUNCTION GSTMR_SET_HWFN

DESCRIPTION
   This function sets the gstmr_FN_modMaxFN to the current value in vstmr.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void GSTMR_SET_HWFN( gas_id_t gas_id )
{
  GDRV_ISR_LOCK(gas_id);
  gtmrs_data[gas_id].gstmr_FN_modMaxFN = vstmr_rd_modMaxFN_frame_num(gas_id);
  GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_cm_set_FN

DESCRIPTION
   This function sets the GSTMR frame number for external clients use.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_cm_set_fn( uint32 new_fn, sys_modem_as_id_e_type as_id)
{
  GSTMR_SET_FN_GERAN(new_fn,geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION GSTMR_ALIGN_FNMOD_COUNTERS

DESCRIPTION
   This function sets the various FNMOD frame numbers based
   on the current FN value. This allows for any updates in
   the SW gstmr_FN to be reflected in the mod counters, such
   as the case when we readjust the FN when a frame miss is
   detected.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void GSTMR_ALIGN_FNMOD_COUNTERS( gas_id_t gas_id )
{
   uint32 gstmr_FN = gtmrs_data[gas_id].gstmr_FN;

   /* ------------------------------------------------------ */
   /*               Supporting Frame Counters                */
   /* ------------------------------------------------------ */

   frame_counters[gas_id].FNmod26    = ( gstmr_FN % MULTIFRAME_26 );
   frame_counters[gas_id].FNmod104   = ( gstmr_FN % ( MULTIFRAME_26 * 4 ) );

   frame_counters[gas_id].FNmod8     = ( gstmr_FN % 8 );
   frame_counters[gas_id].FNmod13    = ( gstmr_FN % ( MULTIFRAME_52 / 4 ) );
   frame_counters[gas_id].FNmod52    = ( gstmr_FN % MULTIFRAME_52 );
   frame_counters[gas_id].FNmod416   = ( gstmr_FN % ( MULTIFRAME_52 * 8 ) );

   frame_counters[gas_id].FNmod51    = ( gstmr_FN % MULTIFRAME_51 );
   frame_counters[gas_id].FNmod102   = ( gstmr_FN % ( MULTIFRAME_51 * 2 ) );

   frame_counters[gas_id].FNmod42432 = ( gstmr_FN % 42432 ); /* Used for RACH */
}

/*===========================================================================

FUNCTION GSTMR_INC_FN_BY_ONE

DESCRIPTION
   This function advances the GSTMR frame number by one.
   Rewritten to prevent expensive modulo arithmetic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void GSTMR_INC_FN_BY_ONE( gas_id_t gas_id )
{
   /* ------------------------------------------------------ */
   /*               Master Frame Counters                    */
   /* ------------------------------------------------------ */
   gtmrs_data[gas_id].gstmr_FN    = ADD_FN ( gtmrs_data[gas_id].gstmr_FN, 1 );

#ifdef GSTMR_HW_FRAME_COUNT
   gstmr_align_hw_frame_count();
#endif

   GSTMR_ALIGN_FNMOD_COUNTERS( gas_id );

#ifdef FEATURE_MAINTAIN_GTIME_IN_WCDMA_DEBUG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION GSTMR_DEC_FN_BY_ONE

DESCRIPTION
   This function subtracts the GSTMR frame number by one.
   Rewritten to prevent expensive modulo arithmetic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void GSTMR_DEC_FN_BY_ONE( gas_id_t gas_id )
{
   /* ------------------------------------------------------ */
   /*               Master Frame Counters                    */
   /* ------------------------------------------------------ */
   gtmrs_data[gas_id].gstmr_FN    = SUB_FN ( gtmrs_data[gas_id].gstmr_FN, 1 );

#ifdef GSTMR_HW_FRAME_COUNT
   gstmr_align_hw_frame_count();
#endif

   GSTMR_ALIGN_FNMOD_COUNTERS( gas_id );
}

/*===========================================================================

FUNCTION gstmr_is_int_pending

DESCRIPTION
  This function can be called from inside INTLOCK/INTFREE pair to see if
  GSTMR interrupt occured since INTLOCK was called.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_is_int_pending( gas_id_t gas_id )
{
   boolean gstmr_int   = FALSE;
   uint32  software_fn = 0; // frame number at the last software frame tick
   uint32  current_fn  = 0; // the current frame number (coming from HW or VSTMR module)

   gstmr_int = (gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)) < gstmr_qsym_at_int_start(gas_id));
   software_fn = get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id));
   current_fn  = vstmr_rd_modMaxFN_frame_num(gas_id);

   /* Check if there is an INT pending. */
   if (gstmr_int || (software_fn != current_fn))
   {
      MSG_GERAN_ERROR_3_G("gstmr_is_int_pending: gstmr_is_int_pending: %d, software_fn:%d, current_fn:%d",
                          gstmr_int,
                          software_fn,
                          current_fn);
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/*===========================================================================

FUNCTION gstmr_start_sleep

DESCRIPTION
  ARM the GSTMR to start sleep. If delay is greater than 0 reprogram
  the terminal count so that the GSTMR roll-over (and start of sleep)
  will occur delay symbols from now.

  For delay == 0 sleep will be started at the next regular GSTMR roll-
  over, i.e. the GSTMR terminal count is not adjusted.

  NOTE: caller is responsible for ensuring that the current symbol count
  plus delay is a sensible value.

DEPENDENCIES
  Call with interrupts locked.

RETURN VALUE
  For:
  delay > 0  - the GSTMR symbol count when it will roll-over
  delay == 0 - 0

SIDE EFFECTS
  Sleep time line will start when the GSTMR rolls over.

===========================================================================*/
uint32 gstmr_start_sleep( uint32 delay, gas_id_t gas_id )
{
  if ( gstmr_get_asynch_int_executing( gas_id ) )
  {
    vstmr_geran_vstmr_deregister( gas_id );
  }
  else
  {
    vstmr_geran_vstmr_deregister_from_isr ( gas_id );
  }

  return (0);
}

/*===========================================================================

FUNCTION gstmr_start_sleep_was_ok

DESCRIPTION
  Check whether last call to gstmr_start_sleep() went OK or not.

RETURN VALUE
  TRUE  - Last call to gstmr_start_sleep() completed sucessfully
  FALSE - There was a problem during the last call the gstmt_start_sleep().

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_start_sleep_was_ok( gas_id_t gas_id )
{
   return( gtmrs_data[gas_id].gstmr_last_start_sleep_was_ok );
}


#ifdef GSTMR_HW_FRAME_COUNT
/*===========================================================================

FUNCTION gstmr_align_hw_frame_count

DESCRIPTION
  Sets the HW GSM hyperframe counter to the current gstmr_FN value.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gstmr_align_hw_frame_count (void)
{
   uint32 hw_fn;

   /* Create the command word and write to HW */
   hw_fn = gstmr_FN[gas_id] | HWIO_FMSK(GSM_HYPERFRAME_LOAD_CTL_CMD, HYPERFRAME_LOAD_EN);
   HWIO_OUT(GSM_HYPERFRAME_LOAD_CTL_CMD, hw_fn);
}
#endif
/*===========================================================================

FUNCTION gstmr_rd_hw_frame_count

DESCRIPTION
  Reads the HW GSM hyperframe counter

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_SIM
uint32 gstmr_rd_hw_frame_count(sys_modem_as_id_e_type as_id)
#else
uint32 gstmr_rd_hw_frame_count(void)
#endif /*FEATURE_DUAL_SIM*/
{
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/
   return gstmr_rd_hw_frame_count_geran(as_id);
}

/*===========================================================================

FUNCTION gstmr_rd_hw_frame_count_geran

DESCRIPTION
  Wrapper function to read the HW GSM hyperframe counter either calculated using VSTMR or GSTMR references.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_rd_hw_frame_count_geran (sys_modem_as_id_e_type as_id)
{
   /* For VSTMR targets calculate the frame number usinge the modMaxFN_frame_num at the start of the frame, */
   /* with the current modMaxFN_frame_num and the SW FN (gstmr_FN) */
   uint32   hwfn, fn_modMaxFN;

   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   gdrivers_FN_mtx_lock( gas_id );

   fn_modMaxFN = vstmr_rd_modMaxFN_frame_num( gas_id );

   if (fn_modMaxFN == gtmrs_data[as_idx].gstmr_FN_modMaxFN)
   {
     hwfn = gtmrs_data[as_idx].gstmr_FN;
   }
   else
   {
     if (fn_modMaxFN < gtmrs_data[as_idx].gstmr_FN_modMaxFN)
     {
        fn_modMaxFN += GERAN_VSTMR_MAX_FN;
     }
     hwfn = ADD_FN(gtmrs_data[as_idx].gstmr_FN, fn_modMaxFN);
     hwfn = SUB_FN(hwfn, gtmrs_data[as_idx].gstmr_FN_modMaxFN);
   }
   gdrivers_FN_mtx_unlock( gas_id );

   return (hwfn);
}

/*===========================================================================

FUNCTION gstmr_rd_hw_frame_count_from_xo

DESCRIPTION
  Reads the HW GSM hyperframe counter

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gstmr_rd_hw_frame_count_from_xo
(
  uint32 xo_cnt,
  uint32 *hwfn,
  uint32 *qsymbol_cnt
  #ifdef FEATURE_DUAL_SIM
  , sys_modem_as_id_e_type as_id
  #endif
)
{
#ifndef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif

  /* For VSTMR targets calculate the frame number usinge the modMaxFN_frame_num at the start of the frame, */
  /* with the current modMaxFN_frame_num and the SW FN (gstmr_FN) */
  uint32        fn_modMaxFN, isr_fn_modMaxFN;
  vstmr_gstmr_t current_gstmr;

  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  int      as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  gdrivers_FN_mtx_lock( gas_id );

  current_gstmr = vstmr_geran_xo_to_gstmr(gtmrs_data[as_idx].geran_view, xo_cnt);

  fn_modMaxFN = current_gstmr.frame_num;

  isr_fn_modMaxFN = gtmrs_data[as_idx].gstmr_FN_modMaxFN;

  MSG_GERAN_LOW_3_G("SNAPSHOT read VSTMR FN %d (Prev-Stored: VSTMR FN %d, FN %d)",
                     fn_modMaxFN,
                     isr_fn_modMaxFN,
                     gtmrs_data[as_idx].gstmr_FN);

  /* assume the passed in XO snapshot time was taken within +-(GERAN_VSTMR_MAX_FN/2)
     frames of the current GSM frame. */

  if((GERAN_VSTMR_MAX_FN + fn_modMaxFN - isr_fn_modMaxFN) % GERAN_VSTMR_MAX_FN < GERAN_VSTMR_MAX_FN/2)
  {
    /* the snapshot xo time is equal or in the future relative to isr_fn_modMaxFN */

    /* check for 4095->0 rollovers on fn_modMaxFN */
    if(fn_modMaxFN < isr_fn_modMaxFN)
    {
      fn_modMaxFN += GERAN_VSTMR_MAX_FN;
    }
  }
  else
  {
    /* the snapshot xo time is in the past relative to isr_fn_modMaxFN */

    /* check for 4095->0 rollovers on isr_fn_modMaxFN */
    if(isr_fn_modMaxFN < fn_modMaxFN)
    {
      isr_fn_modMaxFN += GERAN_VSTMR_MAX_FN;
    }
  }

  *hwfn = ADD_FN(gtmrs_data[as_idx].gstmr_FN, fn_modMaxFN);
  *hwfn = SUB_FN(*hwfn, isr_fn_modMaxFN);

  *qsymbol_cnt = ( current_gstmr.raw_13M * G_STMR_RAW_13MHZ_TO_QSYM ) >> 16;

  gdrivers_FN_mtx_unlock( gas_id );
}

/*===========================================================================

FUNCTION gstmr_rd_hw_fn_qs

DESCRIPTION
  Reads the HW GSM hyperframe counter and qs (synchronised)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_SIM
gsmtr_fn_qs_type gstmr_rd_hw_fn_qs(sys_modem_as_id_e_type as_id)
#else
gsmtr_fn_qs_type gstmr_rd_hw_fn_qs(void)
#endif /*FEATURE_DUAL_SIM*/
{
  gas_id_t   gas_id;
#ifndef FEATURE_DUAL_SIM
   sys_modem_as_id_e_type as_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
#endif /*FEATURE_DUAL_SIM*/

   gas_id = geran_map_nas_id_to_gas_id(as_id);

   return gstmr_rd_hw_fn_qs_geran(gas_id);
}

/*===========================================================================

FUNCTION gstmr_rd_hw_fn_qs_geran

DESCRIPTION
  Reads the HW GSM hyperframe counter and qs (synchronised)

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
gsmtr_fn_qs_type gstmr_rd_hw_fn_qs_geran(gas_id_t gas_id)
{
  gsmtr_fn_qs_type current_fn_qs;
  uint32           hw_fn;

   vstmr_gstmr_t current_gstmr;

   gdrivers_FN_mtx_lock( gas_id );

   current_gstmr = vstmr_geran_get_current_gstmr( gtmrs_data[gas_id].geran_view );

   /* Convert vstmr FN to GL1 FN */
   hw_fn = current_gstmr.frame_num;
   if (hw_fn == gtmrs_data[gas_id].gstmr_FN_modMaxFN)
   {
      current_fn_qs.fn = gtmrs_data[gas_id].gstmr_FN;
   }
   else
   {
      /* Correct for vstmr FN wrap around */
      if (hw_fn < gtmrs_data[gas_id].gstmr_FN_modMaxFN)
      {
        hw_fn += GERAN_VSTMR_MAX_FN;
      }
      hw_fn -= gtmrs_data[gas_id].gstmr_FN_modMaxFN;
      current_fn_qs.fn = ADD_FN(gtmrs_data[gas_id].gstmr_FN,hw_fn);
   }
   current_fn_qs.qs = ( current_gstmr.raw_13M * G_STMR_RAW_13MHZ_TO_QSYM ) >> 16;
   gdrivers_FN_mtx_unlock( gas_id );

  return (current_fn_qs);
}

/*===========================================================================

FUNCTION gstmr_qs_in_next_frame_to_xo

DESCRIPTION
  Returns the xo time for the input qs offset in the next frame

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_qs_in_next_frame_to_xo(uint16 qs_offset,gas_id_t gas_id)
{
   vstmr_gstmr_t gstmr_time;
   uint32        xo_cnt, xo_time;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   xo_cnt = VSTMR_XO_READ_FULL() & VSTMR_XO_MASK;
   gstmr_time = vstmr_geran_xo_to_gstmr(gtmrs_data[as_idx].geran_view, xo_cnt);
   gstmr_time.frame_num = ADD_FN(gstmr_time.frame_num, 1);
   gstmr_time.raw_13M = ((uint32)qs_offset * G_STMR_QSYM_TO_RAW_13MHZ);

   xo_time = vstmr_geran_gstmr_to_xo(gtmrs_data[as_idx].geran_view, gstmr_time);

   return (xo_time);
}

/*===========================================================================

FUNCTION gstmr_qs_in_future_frame_to_xo

DESCRIPTION
  Returns the xo time for the input qs offset in the future frame

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_qs_in_future_frame_to_xo(uint16 qs_offset, gas_id_t gas_id, int fn_offset)
{
   vstmr_gstmr_t gstmr_time;
   uint32        xo_cnt, xo_time;
   int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

   xo_cnt = VSTMR_XO_READ_FULL() & VSTMR_XO_MASK;
   gstmr_time = vstmr_geran_xo_to_gstmr(gtmrs_data[as_idx].geran_view, xo_cnt);
   gstmr_time.frame_num = ADD_FN(gstmr_time.frame_num, fn_offset);
   gstmr_time.raw_13M = ((uint32)qs_offset * G_STMR_QSYM_TO_RAW_13MHZ);

   xo_time = vstmr_geran_gstmr_to_xo(gtmrs_data[as_idx].geran_view, gstmr_time);

   return (xo_time);
}


#ifdef FEATURE_GSTMR_TRACE
/*===========================================================================

                         GSTMR TRACING FUNCTIONS


  This code allows a certain amount of code tracing to occur during the
  execution of the GSTMR ISR.  A trace of locations and timestamps is
  kept, updated whenever gstmr_trace is called.  If the ISR usage goes over
  50% then the trace for the frame is saved to EFS.  This trace can also be
  viewed in T32 (say after hitting a breakpoint at the ISR usage warning).

===========================================================================*/

#include "fs_public.h"
#include <stdio.h>

#define MAX_TRACE_PER_FRAME  40
#define MAX_TRACE_FRAMES     20
#define FNAME_LEN            15

#define GSTMR_TRACE_LOG_FILE           "gstmr_trace.txt"
#define GSTMR_TRACE_LOG_FILE_MAX_SIZE  (50 * 1024)


/* Single trace element */
typedef struct
{
   uint16 timestamp;
   uint16 delta;
   char   fname[FNAME_LEN+1];
   uint32 line;
   int32  param;
} gstmr_trace_type;

/* One frame's worth of trace elements */
typedef struct
{
   uint32           FN;
   uint32           qs_start;
   uint16           num_trace;
   gstmr_trace_type trace[MAX_TRACE_PER_FRAME];
   boolean          overflow_count;
} gstmr_trace_log_type;

/* Storage for the trace */
static gstmr_trace_log_type gstmr_trace_log[MAX_TRACE_FRAMES];
static uint32 gstmr_trace_index;

/* Flag to prevent further writes when storing */
static boolean gstmr_trace_locked = FALSE;

/*===========================================================================

FUNCTION gstmr_trace

DESCRIPTION
  Saves the location, timestamp and given parameter to the gstmr trace
  log.

===========================================================================*/
void gstmr_trace (const char *fname, uint32 line, int32 param)
{
   static uint16 last_ts;   /* last timestamp - save it to be faster */
   uint16 num_trace, ts;
   const char *p;

   /* Skip the trace if we are currently saving */
   if (gstmr_trace_locked) return;

   /* Get the current qsymbol count */
   ts = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));

   /* If the frame number has changed, start a new trace */
   if (gstmr_trace_log[gstmr_trace_index].FN != GSTMR_GET_FN_GERAN( gas_id))
   {
      if (++gstmr_trace_index >= MAX_TRACE_FRAMES)
      {
         gstmr_trace_index = 0;
      }

      /* Reset the frame */
      gstmr_trace_log[gstmr_trace_index].FN = GSTMR_GET_FN_GERAN( gas_id);
      gstmr_trace_log[gstmr_trace_index].qs_start = gstmr_qs_start;
      gstmr_trace_log[gstmr_trace_index].num_trace = 0;
      gstmr_trace_log[gstmr_trace_index].overflow_count = 0;
      last_ts = gstmr_qs_start;
   }
   /* Check for trace overflow */
   else if (gstmr_trace_log[gstmr_trace_index].num_trace == MAX_TRACE_PER_FRAME)
   {
      gstmr_trace_log[gstmr_trace_index].overflow_count++;
      return;
   }

   /* Store trace details */
   num_trace = gstmr_trace_log[gstmr_trace_index].num_trace;
   gstmr_trace_log[gstmr_trace_index].trace[num_trace].param = param;
   gstmr_trace_log[gstmr_trace_index].trace[num_trace].line = line;
   gstmr_trace_log[gstmr_trace_index].trace[num_trace].timestamp = ts;
   gstmr_trace_log[gstmr_trace_index].trace[num_trace].delta = ts - last_ts;
   last_ts = ts;

   /* Filename is in the form "..\..\drivers\" so just get the name */
   p = strrchr(fname, '\\');
   p = (p == NULL ? fname : p + 1);
   (void)strlcpy(gstmr_trace_log[gstmr_trace_index].trace[num_trace].fname, p, FNAME_LEN);

   gstmr_trace_log[gstmr_trace_index].num_trace++;
}



/*===========================================================================

FUNCTION gstmr_trace_record_dpc

DESCRIPTION
   Records gstmr trace statistics to the log file.  Must be called from
   task context.

===========================================================================*/
static void gstmr_trace_record_dpc (uint32 index)
{
   /* Define a static buffer for creating the output.  This is because
      each call to efs_write has a lot of overhead so we do it all in one
      write at the end.  Each trace line is about 70 chars long. */
   static char gstmr_trace_buf[(MAX_TRACE_PER_FRAME + 1) * 80 + 20];
   static uint32 total_bytes = 0;
   int32 fd, i, nbytes, oflag, loc_nbytes = 0;

   MSG_GERAN_HIGH_1_G("Logging GSTMR trace for FN %d",gstmr_trace_log[index].FN);

   /* Open the file, overwriting if it gets too large */
   oflag = O_WRONLY | O_CREAT;
   if (total_bytes > GSTMR_TRACE_LOG_FILE_MAX_SIZE) total_bytes = 0;
   if (total_bytes > 0) oflag |= O_APPEND;
   fd = efs_open(GSTMR_TRACE_LOG_FILE, oflag, 0755);
   if (fd < 0) return RET_VAL;

   /* Write the header if this is a new file */
   if (total_bytes == 0)
   {
      char hdr[] = "GSTMR TRACE LOG\r\n"
                   "FN           Time  (Delta)             File   Line    Param\r\n";
      (void) efs_write(fd, hdr, sizeof(hdr) - 1);  /* -1 to skip the null */
   }

   /* Write a line showing when the ISR started */
   nbytes = snprintf( gstmr_trace_buf, sizeof(gstmr_trace_buf), "%10lu %6lu  (%5lu) %16s %6u %8d\r\n",
                      gstmr_trace_log[index].FN,
                      gstmr_trace_log[index].qs_start,
                      gstmr_trace_log[index].qs_start,
                      "<ISR_START>", 0, 0 );

   /* Write the trace information */
   for (i = 0; i < gstmr_trace_log[index].num_trace; i++)
   {
     if ( (nbytes > 0) && (nbytes < sizeof(gstmr_trace_buf)-1) )
     {
       loc_nbytes = snprintf(&gstmr_trace_buf[nbytes], sizeof(gstmr_trace_buf)-nbytes, "%10lu %6u  (%5u) %16s %6lu %8ld\r\n",
       gstmr_trace_log[index].FN,
       gstmr_trace_log[index].trace[i].timestamp,
       gstmr_trace_log[index].trace[i].delta,
       gstmr_trace_log[index].trace[i].fname,
       gstmr_trace_log[index].trace[i].line,
       gstmr_trace_log[index].trace[i].param);
     }

     if (loc_nbytes < 0)
     {
       nbytes = sizeof(gstmr_trace_buf);
       break;
     }
     else
     {
       nbytes += loc_nbytes;
     }
   }

   loc_nbytes = 0;
   if (gstmr_trace_log[index].overflow_count > 0)
   {
     if ( (nbytes > 0) && (nbytes < sizeof(gstmr_trace_buf)-1) )
     {
       loc_nbytes = snprintf(&gstmr_trace_buf[nbytes], sizeof(gstmr_trace_buf)-nbytes, "(overflow=%d)\r\n",
       gstmr_trace_log[index].overflow_count);
     }

     if (loc_nbytes < 0)
     {
       nbytes = sizeof(gstmr_trace_buf);
       break;
     }
     else
     {
       nbytes += loc_nbytes;
     }
   }

   if ( (nbytes > 0) && (nbytes < sizeof(gstmr_trace_buf)-2) )
   {
     /* Add trailing newline, note no need for a trailing null */
     gstmr_trace_buf[nbytes++] = '\r';
     gstmr_trace_buf[nbytes++] = '\n';

     (void) efs_write(fd, gstmr_trace_buf, nbytes);
   }
   else
   {
     (void) efs_write(fd, gstmr_trace_buf, sizeof(gstmr_trace_buf));
   }

   (void) efs_close(fd);

   /* Release the trace */
   gstmr_trace_locked = FALSE;
   total_bytes += nbytes;

   return;
}



/*===========================================================================

FUNCTION gstmr_trace_record

DESCRIPTION
   Schedules a DPC to records gstmr trace statistics to the log file

===========================================================================*/
static void gstmr_trace_record (void)
{
   /* Don't bother if we are already storing */
   if (gstmr_trace_locked) return;

   /* Lock to prevent further access */
   gstmr_trace_locked = TRUE;

   /* Queue up a DPC to do the writing to EFS */
   if (!rexl4_queue_dpc(gstmr_trace_record_dpc, gstmr_trace_index))
   {
      MSG_GERAN_ERROR_0_G("Failed to queue dpc");
      gstmr_trace_locked = FALSE;
   }
}
#endif  /* FEATURE_GSTMR_TRACE */

/*===========================================================================

FUNCTION gstmr_qsym_at_int_start

DESCRIPTION
  This function allows other calling functions to know the quarter symbol
  value is at the start of the interrupt, and this allows the process to
  check to see if an interrupt was pending. For Mustang the function
  gstmr_is_int_pending is no longer supported by Tramp/OS.

DEPENDENCIES
  None

RETURN VALUE
  Quarter Symbol Value at the start if the GSTMR ISR

SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_qsym_at_int_start (gas_id_t gas_id)
{
  return (gtmrs_data[gas_id].gstmr_qs_start);
}

/*===========================================================================

FUNCTION gstmr_set/get_asynch_int_executing

DESCRIPTION
  This function informs other layers as to whether we are actively executing
  the asynchronous frame interrupt from the mDSP.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if executing asynch mDSP int or FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

void gstmr_set_asynch_int_executing( boolean is_asynch_int_running,gas_id_t gas_id )
{
  gtmrs_data[gas_id].gstmr_asynch_int_executing = is_asynch_int_running;
}

boolean gstmr_get_asynch_int_executing( gas_id_t gas_id )
{
  return ( gtmrs_data[gas_id].gstmr_asynch_int_executing );
}

/*===========================================================================

FUNCTION gstmr_align_timebase

DESCRIPTION
  This function Aligns or delays the timebase of one GSTMR with reference
  to the other.

  if delay = 0, GSTMR_G1 will be delayed to align with GSTMR

DEPENDENCIES
  GSTMR_G1 and GSTMR needs to be initiailized

RETURN VALUE
  none
SIDE EFFECTS
  None

===========================================================================*/
void gstmr_align_timebase(uint16 delay_qs)
{
#ifdef FEATURE_DUAL_SIM
  gas_id_t gas_id = gstmr_determine_gas_id( GSTMR_1 );

  /* Work out which GAS ID is connected to GSTMR G1*/
  if ( gas_id != GERAN_ACCESS_STRATUM_ID_1 )
  {
    MSG_GERAN_ERROR_0_G("GTMRS align failed");
    return;
  }
#endif
}

/*===========================================================================

FUNCTION calc_sync_gstmr_tick

DESCRIPTION
  This function gives a qs snapshot for the point at which it is called
  except for the ASYNCH int where it needs to factor in the time in qs
  to the start of the next real GSTMR.

DEPENDENCIES
  None

RETURN VALUE
  Snapshot qs to the next GSTMR.
SIDE EFFECTS
  None

===========================================================================*/
uint32 calc_sync_gstmr_tick( uint32 *ustmr, gas_id_t gas_id )
{
   uint32 sync_gstmr_tick_qs =
            gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id( gas_id ) );

   /* Work out start of next GSM frame tick */
   if ( gstmr_get_asynch_int_executing( gas_id ) )
   {
     sync_gstmr_tick_qs = ( QS_PER_FRAME - sync_gstmr_tick_qs );
     *ustmr = USTMR_ADD( *ustmr, USTMR_FROM_QS( sync_gstmr_tick_qs ) );
   }
   else
   {
     *ustmr = USTMR_SUB( *ustmr, USTMR_FROM_QS( sync_gstmr_tick_qs ) );
   }

   return ( sync_gstmr_tick_qs );
}


/*===========================================================================

FUNCTION gstmr_get_ota_ustmr_start_time

DESCRIPTION
  This function returns the USTMR time for the start of the frame at which
  commands issued on this GSTMR will go OTA.

DEPENDENCIES
  None

RETURN VALUE
  USTMR
SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_get_ota_ustmr_start_time( gas_id_t gas_id )
{
   uint32 ustmr;

   int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   vstmr_gstmr_t gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   gstmr.raw_13M = QS_PER_SLOT*G_STMR_QSYM_TO_RAW_13MHZ;

   gstmr.frame_num = (gstmr.frame_num + 1)&(GERAN_VSTMR_MAX_FN - 1);
   ustmr = vstmr_geran_gstmr_to_xo(gtmrs_data[as_idx].geran_view, gstmr);

   return ustmr;
}

/*===========================================================================

FUNCTION gstmr_get_ota_ustmr_start_time_sw_fn

DESCRIPTION
  This function returns the USTMR time for the start of the offseted frame with current fn.

DEPENDENCIES
  None

RETURN VALUE
  USTMR
SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_get_ota_ustmr_start_time_sw_fn(uint8 offset,  gas_id_t gas_id )
{
   uint32 ustmr;

   int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   vstmr_gstmr_t gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   gstmr.raw_13M = QS_PER_SLOT*G_STMR_QSYM_TO_RAW_13MHZ;

   gstmr.frame_num = (gstmr.frame_num + offset)&(GERAN_VSTMR_MAX_FN - 1);
   
   ustmr = vstmr_geran_gstmr_to_xo(gtmrs_data[as_idx].geran_view, gstmr);
   
   return ustmr;
}



/*===========================================================================

FUNCTION gstmr_get_current_fn_ustmr_start_time

DESCRIPTION
  This function returns the USTMR time for the start of the current frame .

DEPENDENCIES
  None

RETURN VALUE
  USTMR
SIDE EFFECTS
  None

===========================================================================*/
uint32 gstmr_get_current_fn_ustmr_start_time(gas_id_t gas_id)
{
   uint32 ustmr;

   int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

   vstmr_gstmr_t gstmr = vstmr_geran_get_current_gstmr ( gtmrs_data[as_idx].geran_view );

   gstmr.raw_13M = QS_PER_SLOT*G_STMR_QSYM_TO_RAW_13MHZ;

   ustmr = vstmr_geran_gstmr_to_xo(gtmrs_data[as_idx].geran_view, gstmr);
   
   return ustmr;
}

#ifdef FEATURE_GERAN_VP

/*===========================================================================

FUNCTION TRAMP_CLEAR_INTERRUPT_VP

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tramp_clear_interrupt_vp (unsigned int interruptNum)
{
   qinterrupt_deregister(interruptNum);
}

/*===========================================================================

FUNCTION TRAMP_IS_INTERRUPT_PENDING_VP

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tramp_is_interrupt_pending_vp (unsigned int interruptNum)
{
   return (FALSE);
}

/*===========================================================================

FUNCTION TRAMP_SET_ISR_VP

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  tramp_set_isr_vp(unsigned int interruptNum, void (*fp) (void))
{
   /* right now function pointer is ignored - later we will store it into a global table */
   if(fp == NULL)
   {
       qinterrupt_deregister(interruptNum);
   }
   else
   {
      qinterrupt_register(interruptNum);
   }

}

/*===========================================================================

FUNCTION TRAMP_ISR_COMMON_INIT_VP

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tramp_isr_common_init_vp ()
{
     int i;
     qthread_t isrR;
     qthread_attr_t attrR;

     qthread_attr_init(&attrR);
     qthread_attr_setpriority(&attrR, 99);
     qthread_attr_setname(&attrR, "ISR");

     i = qthread_create(&isrR, &attrR);
     if(i != EOK)
     {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G("Cannot create ISR thread ");
     }
     i = qthread_start(isrR, vp_isr_callback, NULL);
     if(i != EOK)
     {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G("Cannot start ISR thread ");
     }

}

/*===========================================================================

FUNCTION VP_ISR_CALLBACK

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void vp_isr_callback( void )
{
        int status, int_number;

        status = qinterrupt_register(GSM_MICRO_INT);
        if(status != EOK)
        {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_0_G("Cannot register interrupt  ");
        }
        while(1)
        {
            status = qinterrupt_receive(&int_number);
            if(status != EOK)
            {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                ERR_GERAN_FATAL_0_G("Error from interrupt handler  ");
            }
            if(int_number != GSM_MICRO_INT)
            {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                ERR_GERAN_FATAL_0_G("Unexpected interrupt received ");
            }
            else
            {
                gstmr_isr();
            }
            printf( "from isr callback function \n" );
        }
}

#endif /* FEATURE_GERAN_VP */

/*===========================================================================

FUNCTION gl1_ustmr_gstmr_snapshot_before_sleep

DESCRIPTION
  This function takes a snapshot of GSM FN, USTMR and GSTMR values. This function is called just before L1 goes
  to sleep mode.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_ustmr_gstmr_snapshot_before_sleep(gas_id_t gas_id)
{
  vstmr_gstmr_t vstmr_gstmr;
  int           as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* get current GFN */
  gtmrs_data[gas_id].GsmFNSleepStart = GSTMR_GET_FN_GERAN(gas_id);

  /* get current (full, 56 bit) XO time*/
  gtmrs_data[gas_id].UstmrSleepStartTime = VSTMR_XO_READ_FULL();

  /* get current GSTMR time - converted from lower 24 bits of XO time i.e. USTMR time */
  vstmr_gstmr = vstmr_geran_xo_to_gstmr(gtmrs_data[as_idx].geran_view,
                                        (gtmrs_data[gas_id].UstmrSleepStartTime & VSTMR_XO_MASK));
  /* store GSTMR frame time */
  gtmrs_data[gas_id].GstmrSleepStartTime = vstmr_gstmr.raw_13M;
}

/*===========================================================================

FUNCTION gl1_get_gsm_time_tag

DESCRIPTION
  This function calculates the GSM time tag interms of current FN and Quarter symbols

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_get_gsm_time_tag( gas_id_t gas_id, uint32 *fn, uint32 *qsym )
{
  uint32 QsymbolsElapsed,FramesElapsed;
  uint64 GstmrTimeElapsed,UstmrTimeElapsed,UstmrTimeNowFromDAL;
  int32 XOErrorInppm;

  UstmrTimeNowFromDAL = gl1_get_daltick(gas_id);

  /* Calculating the time elapsed since L1 went to sleep in USTMR (XO) units */
  UstmrTimeElapsed = UstmrTimeNowFromDAL - gtmrs_data[gas_id].UstmrSleepStartTime;

  /* Converting elapsed USTMR time to GSTMR uints */
  GstmrTimeElapsed = (uint64)UstmrTimeElapsed * 13000000LL;
  GstmrTimeElapsed = (GstmrTimeElapsed + (19200000 >> 1))/19200000LL;

  /* Adding the elapsed time to sleep start GSTMR time */
  GstmrTimeElapsed = GstmrTimeElapsed + (gtmrs_data[gas_id].GstmrSleepStartTime);

  /* Considering XO offset error in the GSTMR time elapsed caculations. XO offset error is interms of 1/1024ppm.*/
  XOErrorInppm = gl1_hw_get_xo_acc_freq_err(gas_id);
  GstmrTimeElapsed = GstmrTimeElapsed - (((int64)GstmrTimeElapsed * (int64)XOErrorInppm)/1024000000LL);

  /* Calculating the number of frames and Quater symbols elapsed since L1 went to sleep  */
  QsymbolsElapsed = ((uint32)GstmrTimeElapsed + (NUM_GSM_CLKS_PER_QS >> 1))/NUM_GSM_CLKS_PER_QS;
  FramesElapsed = QsymbolsElapsed/QS_PER_FRAME;
  QsymbolsElapsed =  QsymbolsElapsed % QS_PER_FRAME;

  /* Calculate current time interms of FN and Quarter symbols */
  *fn   = ADD_FN( gtmrs_data[gas_id].GsmFNSleepStart, FramesElapsed );
  *qsym = QsymbolsElapsed;
}

/*===========================================================================

FUNCTION gl1_get_daltick

DESCRIPTION
  This function returns the DAL time tick

RETURN VALUE
  64bit DAL time tick

SIDE EFFECTS
  None

===========================================================================*/
static uint64 gl1_get_daltick ( gas_id_t gas_id )
{
  uint64 tick = 0;
  uint32 offset = 0;

  if (DAL_SUCCESS != DalTimetick_GetTimetick64(gsmtt_handle[gas_id],
            (DalTimetickTime64Type *) &tick ))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("timetick error!");
  }

  DalTimetick_GetOffset(gsmtt_handle[gas_id], &offset);

  return (tick - offset);
}

#if defined(TEST_FRAMEWORK)
#error code not present
#endif

/*===========================================================================

FUNCTION gstmr_hw_on

DESCRIPTION
  Returns True if GSTMR HW is initialized && not disabled for sleep

DEPENDENCIES
  None

RETURN VALUE
  Returns True if GSTMR HW is initialized && not disabled for sleep

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_hw_on (sys_modem_as_id_e_type as_id)
{
  gas_id_t      gas_id = geran_map_nas_id_to_gas_id( as_id );
  return (!gtmrs_data[gas_id].gstmr_disabled_for_sleep && gtmrs_data[gas_id].gstmr_initialized );
}

/*===========================================================================

FUNCTION gstmr_is_tdma_handler_registered

DESCRIPTION
 Returns whether the l1_tdma_1_ISR() handler is registered

DEPENDENCIES
  None

RETURN VALUE
  Boolean - TRUE - l1_tdma_1_ISR is registered
  FALSE - l1_tdma_1_ISR is not registered

SIDE EFFECTS
  None

===========================================================================*/
boolean gstmr_is_tdma_handler_registered( gas_id_t gas_id )
{
  int           as_idx       = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  gtmrs_data_t *gtmrs_data_p = NULL;
  boolean       tdma_isr_reg = FALSE;

  gtmrs_data_p = &gtmrs_data[as_idx];

  if ( gtmrs_data_p && gtmrs_data_p->num_handlers )
  {
    uint8 i;

    for (i = 0; i < gtmrs_data_p->num_handlers; i++)
    {
      if (gtmrs_data_p->gstmr_handlers[i] == l1_tdma_1_ISR)
      {
        tdma_isr_reg = TRUE;
      }
    }
  }

  return tdma_isr_reg;
}

/* EOF */
