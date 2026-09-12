/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  L A Y E R  1  - M D S P   I N T E R F A C E
                    mDSP Command Interface Module

GENERAL DESCRIPTION

    This module handles communications to and from the mDSP.

    The host writes one or more commands into the mDSP command buffer in the
    frame prior to the required action.  When the frame tick interrupt occurs
    the mDSP will retrieve all those commands and process it for the current
    frame.

    Two command buffers exist.  While the mDSP is processing information from
    one buffer the host will write commands into the other.

    In addition to the command buffers, other buffers also exist for the
    purpose of delivering data to and from the mDSP.  These buffers include:

    fcch_results_buffer
    sch_results_buffer
    tx_profile_buffer
    sbi_results_buffer
    sbi_request_buffer
    nonsbi_request_buffer
    general_buffers for rx, tx, debug, metrics


                                  -------------
                                  |            |
                                  |   host     |
                                  |            |
                                  -------------
                                      | |  ^  ^
              ------------------------| |  |  ------------------------
              |                         |  ----------|                |
              |                         |            |                |
              |                         |            |                |
    =======================================================================
              |                         |            |                |
        ------------                    |            |                |
        |          |                    |            |                |
        v          v                    v            |                v
    --------    --------     ---------------  ----------------   --------------
    |      |    |      |     |             |  |  fcch         |  |            |
    |  c   |    |  c   |     | tx profile  |  |---------------|  |  general   |
    |  o   |    |  o   |     |             |  |  sch          |  |            |
    |  m   |    |  m   |     |------------ |  |---------------|  |  buffers   |
    |  m   |    |  m   |     |-------------|  |---------------|  |            |
    |  a   |    |  a   |     |             |  |               |  |    for     |
    |  n   |    |  n   |     | sbi request |  |  error        |  |            |
    |  d   |    |  d   |     |             |  |  results      |  |    rx      |
    |      |    |      |     |-------------|  |---------------|  |    tx      |
    |      |    |      |     |-------------|  |---------------|  |    metrics |
    |      |    |      |     |             |  |               |  |            |
    |  A   |    |  B   |     |             |  |  debug        |  |    sbi     |
    |      |    |      |     |             |  |               |  |    nonsbi  |
    |      |    |      |     |             |  |               |  |            |
    --------    --------     |--------------  ----------------   --------------
        |          |                   |         ^                    ^
        |          |             ------          |                    |
        ----------------------|  |  --------------                    |
                              v  v  |                                 |
                             -------------                            |
                             |            |                           |
                             |   mdsp     | <--------------------------
                             |            |
                             -------------

      =======================================================================

EXTERNALIZED FUNCTIONS                      PHONE T FUNCTIONS
    mdsp_set_pa_profile                     mdsp_start_tx_tone_cont
    mdsp_ciphering                          mdsp_get_pa_profile
    mdsp_start_acquisition                  mdsp_peek_one_port
    mdsp_stop_acquisition                   mdsp_get_grfc_polarity
    mdsp_power_measure                      mdsp_get_grfc_out
    mdsp_transmit_burst                     mdsp_get_grfc_omode
    mdsp_sync_rx                            mdsp_get_grfc_tristate
    mdsp_async_rx                           mdsp_start_tx_rnd_cont
    mdsp_async_rx_sch
    mdsp_init
    mdsp_generic_configuration
    mdsp_clr_current_num_cmd
    mdsp_nonsbi_write
    mdsp_grfc_out
    mdsp_grfc_tristate
    mdsp_grfc_in
    mdsp_set_sync_rx_time_offset
    mdsp_set_sync_tx_offset
    mdsp_switch_command_buffers
    mdsp_get_FCCH_results
    mdsp_get_SCH_results
    mdsp_display_ftick_ctr
    mdsp_build_sbi_burst_buffer
    mdsp_build_nonsbi_burst_buffer
    mdsp_clear_nonsbi_static_buffer
    mdsp_build_sbi_static_buffer
    mdsp_build_nonsbi_static_buffer
    mdsp_download_and_startup
    mdsp_set_sync_rx_search_width
    mdsp_set_async_rx_search_width
    mdsp_set_bsic
    mdsp_get_decoded_data
    mdsp_get_async_decoded_data
    mdsp_get_async_rx_metrics
    mdsp_set_loopback
    mdsp_advance_rx_metrics_ptrs
    mdsp_reset_async_rx_ptrs
    mdsp_program_cal_amam_table
    mdsp_issue_async_msg

INITIALIZATION AND SEQUENCING REQUIREMENTS
   mdsp_switch_command_buffers() must be installed as a frame tick handler
   before the sbi, and nonsbi write functions are used.

  Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_intf.c#4 $
$DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ------- -----------------------------------------------------
24/07/20    sal     CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
27/01/20    rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
11/04/17     rv      CR2031827 Excessive F3 log reduction for Oppo
11/08/17    rc        CR2138578  ML logging - GL1 Changes
07/04/16   snjv      CR1000205 Remove depracated function mdsp_gsm_config_tx 
05/12/17     km      CR987605 G2X immediate cleanup
14/02/17     mk      CR2004610 : FR 40251 (SeNS) - Skip reporting NCELL if SCH reconfirmation failed with good GDET score
14/08/17    nm       CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
19/07/17     hd      CR2077085 Buffer overflow issue in the function mdsp_get_async_decoded_data()
17/05/16     zf      CR1016821: Moving EFS reading out of DPC.
24/11/15     cc      CR928622 Use idle Rx Alpha for T2G idle in DR mode
06/11/15     og      CR837817 Ensure the async burst metrics semaphore bit is reset after reading the results.
25/09/15     jk      CR912783 Read jdetreading from GFW for NSCH and X2G SCH
07/09/15     npt     CR898493 GL1 NV control of T2G acquisition GFW error recovery
26/08/15     ws      CR896163 Fix compile errors for undefining FEATURE_DUAL_SIM
14/08/15     cja     CR890072 Dual data mdsp_cmd_buf_ptr for delayed T2G gaps on GS2
14/08/14     ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
23/07/15     pg      CR877745 Reorder multisim FW RF init to reduce critical path through RF wakeup 
14/02/15     smd     CR794015 Added support for RF self test
22/05/15     ws      CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
28/05/15     cws     CR843724 Fix compile error with ASDIV feature enable
15/04/15     pa      CR822841 Split Rx/Tx GL1 TRM changes for Single SIM.
05/07/15     zc      CR826075 Band Specific ASDIV Enable
01/05/15     npt     CR831592 Change definition of gl1_gfw_nv_recovery variable to fix QTF build  
29/04/15     npt     CR804870 RF timeline error recovery mechanism
22/04/15     ab      CR822921 Provide Rx ARFCN in SYNC RX / ASYNC RX for PCHXFER Scheduling in GFW and RXLEV info for IQ Scaling in ALL ASYNC RX. 
04/21/15     zc      CR805003 Set default ASDIV EFS values for idle/traffic thresholds
20/04/15     ip      CR811868 Dual Multisim mode code changes + debug overhaul
23/02/15     sjv     CR799313 Using new mcfg APIs replacing efs_get for Geran
07/04/15     og      CR814507 Further stack reduction in the GSM L1 ISR.
04/03/14     xz      CR801386 set correct sch_rx.reason when it is not GBTA and DSDS scenario
20/02/15     ab      CR773948 GL1 Support for provding Dumps and Metrics for QSH
23/02/15     sjv     CR799313: Using new mcfg APIs replacing efs_get for Geran
07/04/15     og      CR814507. Further stack reduction in the GSM L1 ISR.
04/03/14     xz     CR801386 set correct sch_rx.reason when it is not GBTA and DSDS scenario
24/03/15     dg      CR811888 Don't ignore SCH data if metrics not available in case of inactive mode
25/03/15     gk/npt  CR711799 GL1 changes for triggering recovery mechanism in GFW when RF CCS Events are in incorrect state
23/03/15     pg      CR786023 Add support for FTM DRx IQ logging (FR22183)
23/03/15     pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
20/03/15     jj      CR 811098 GL1 casts GFW Debug NVs to uint16 
09/03/15     dg      CR800014 During first attempt of NCELL SCH decode, ignore SCH data if metrics is not available
05/03/15     cja     CR803591 Ignore stale SCH data for parallel FCCH/SCH
27/02/15     cja     CR768169 Add RF Seq Number
09/02/15     jj      CR 793229 making debug f3 for crash in the middle of the burst under NV control 
17/11/14     jj      CR 752202 GL1 - bolt - Debug support for crashing in the 
                      middle of rx burst for a given range of rssi value
23/01/15     npt     CR697236 Mainline FEATURE_GERAN_VSTMR
19/01/15     jj      CR 783242 fix merge error 
19/11/14     cja     CR758107 Do not define rex timer multiple times - so add rex_undef  
12/12/14     cs      CR754853 Add support for Parallel SCH detection handling
03/12/14     npt     CR759467 Fix compiler warnings
28/11/14     am      CR760135 Enable SBD and EPD for page decode in transfer mode
05/11/14     jj      CR 751077 remove gfwTxTablesBuffer   from GL1 side 
20/11/14     jj      CR 760263 treat all channels as spur channels in DR-DSDS
01/10/14     zc	     CR707083: FR 22623: ASDiv (+tuner) for TDD-LTE/TDS/G+G DR-DSDS w/ TDS/G CSFB
29/10/14     ab      CR745429 - Featurization correction for PCHXFER for SS / CR merges.
27/10/14     cja     CR745407 Remove GFW interface mismatch check and F3.
27/10/14     npt     CR746130 Use local instead of global timer to avoid a race condition 
                     with different tasks setting the global timer 
10/09/14     gk      CR713276 Reset the reason flag inside mdsp_get_ncell_sch_decoded_data
17/10/14     df      CR726944:Delete unused header file mdsp_dtm_g.h 
07/10/14     pg      CR730132 F3 cleanup 
03/10/14     aga     CR732455 Changes required to tune away framework for TH 1.0 single sim build 
10/09/14     ss      CR722740 Print F3 only if  pGfwIqLogBuffer == NULL
17/09/14     df      CR725802 Update burst_num during error recovery 
15/09/14     sjv     CR724543:add protection for tx_burst_offset against invalid values from RF 
09/09/14     jk      CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14     nm      CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
11/09/14     ws      CR723099 Resolve GL1 compile warnings
04/09/14     zf      CR702512: Correction in detecting GSTMR interrupt pending
09/09/14     cah     CR718841 GL1 missing support for RFSW changes for RXFE bus contention
07/08/14     cja     CR643648 Desense for channel adjacent to spur
24/07/14     nk      CR699150 W2G IRAT timeline improvement (remove F3s from GL1)
11/02/14     jk      CR614254:GL1 changes to support IP2 calibration
06/08/14     df      CR536059 Full asynchronous sleep support in idle
24/04/13     jj      CR 477224  NV control the enabling of feature FEATURE_GSM_SEARCH_ALL_WCELL
24/07/14     ss      CR699614 pGfwIqLogBuffer before sending iq_samples_to_tcxomg 
23/06/14     ap      CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
10/07/14     js      CR593074- ROTA/QBTA Feature implementation
02/07/14     mc      CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14     jj      CR 668965 merge ASDIV from DI.3.0 to BO 2.0 
27/03/14     pjr     CR639777 Configure TX chain Id correctly after connected wtr hopping
30/05/14     pg      CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM 
13/06/14     og      CR668940. Clear the wfft timer at the end of mdsp_wait_for_frame_tick_non_blocking( ).
03/06/14     cja     CR673927 When MSP disable do not check GFW is ready if already disabled.
28/05/14     cja     CR657368 Fix Klockwork issue of NULL data space pointer
22/05/14     cja     CR665033 Check GFW ready to sleep before disabling
02/05/14     cja     CR658621 Optimise QTA and T2G F3.
01/05/14     zc      CR658120 GSM ASDIV NV read only done once
15/04/14     ka      CR648968 PTMSI value being assigned to TMSI
17/04/14     cos     CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
07/04/14     cja     CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14     cgc     CR643674 changed static poll_timer to dual data space 
21/03/14     mc      CR.588853 : COEX : GL1 changes for Enhanced FCCH algorithm
12/03/14     pa      CR630228:Bolt: Changes due to obsolete FW API cleanup
03/03/14     npt     CR622060 Rework wake-up
10/03/13     ka      CR628885 Remove unused functions
27/02/14     ap      CR600310 Enable Channel id support
23/02/14     mc      CR621753 Back out 3WCXM changes due to stability issues
19/02/14     ap      CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
14/02/14     us      CR609569 Ensure correct GFW interface offset is getting stored
27/01/14     pa      CR607716:GL1 changes related to legacy GFW interface changes.
21/11/13     dv      CR579907 Back out CR 519537
29/01/14     cs      CR605061:Set decode_flag based on feature Macro(FEATURE_DISABLE_EARLY_DECODE)
19/12/13     jk       CR503359 - GL1 changes to support Rx diversity
20/12/13     sk      CR590776 Ensure TX commands not issued before Tx config issued
09/12/13     cja     CR586971 Remove function mdsp_issue_enter_mode_cmd
14/04/14     jj      CR600114 tx- diversity in dime SGLTE part 2
08/01/14     us       CR562238 Partial T2G gap usage for RSSI measurement
19/12/13     jk      CR503359 - GL1 changes to support Rx diversity
28/08/13     ws      CR534724 Print current TMSI value in place of variable 
26/11/13     sai     CR572198: X2W mode cfg cmd issued after startup/cleanup scripts built,move G2W gap further into GSM frame
                     to avoid overrun.
28/11/13     cja     CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
28/11/13     cja     CR583253 Correct merge error in CR455527 which broke sub 2 in G+G
22/11/13     cja     CR455527 Remove featurisation from files in API directory
20/11/13     jk      CR485738:Remove the condition check for rex_ints_are_locked( ) in mdsp_wait_for_frame_tick( )
11/15/13     ws      CR534724 Print current TMSI value in place of variable 
05/11/13     js      CR555528 : COEX : CXM Priority and Freq ID for Async Rx
05/11/13     pg      CR571341 pass correct CCS buffer index for continunous Tx stop 
31/10/13     cgc     CR569801  Add wrapper function mdsp_cm_set_ftsm()
26/11/12     jj      CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
11/10/13     zf      CR552306: Notify GFW OLS USTMR time.
04/10/13     pjr     CR536133 removed static qualifier from mdsp_process_cxm_logs
17/09/13     pjr     CR538893 Changes to enable CXM logging
02/07/13     pjr     CR507542 Implementation of cxm information logging
01/10/13     sk      CR552259 CR sync from 1.0/1.1
25/09/13     dv      CR549679 set mdsp_init_complete to avoid CCS crash 
24/09/13     cja     CR539766 Set num_tasks to 0 when clearing CCS buffers enter mode, therm read and tx band events
23/09/13     sk      2.0 syncup from 1.1
13/09/13     jk      CR413653:Set flag spurChanFlag for desense channel
13/09/13     dv      CR544094 CCS crash due to rfBufIndex not being incremented  
03/09/13     cja     CR538140 Add mcpm_tx_on flag for race condition in sending Tx Config
21/08/13     dv      CR532560 Improve T2G SCH performance 
26/08/13     sk      CR534005 Use correct API to get the dual/triple standby mode
06/08/13     ws      CR521868: Introduce API mdsp_rf_set_iq_log() to enable IQ logging on 2nd RF chain
03/09/13     npt     CR524770 - GL1 changes to support RFLM FED 
21/08/13     npt     CR531633 - Remove FEATURE_DIME_MODEM dependencies
23/08/13     sk      CR533467 Partial QBTA bringup on 2.0 in DSDS
19/01/12     ab      CR440436 - Reset the RxDecodedDataBuffer  prior to usage 
                     to  avoid SSBI FW crash seen during NCELL BCCH decode.
15/08/13     sk      CR524039 GBTA bringup changes
08/08/13     cja     CR519510 Use DPC to call MCPM, send Tx Config only when MCPM ready
23/07/13     og      CR503871. Don't send the async command if the host i/f memory ptr is not setup.
06/08/13     ws      CR521868: Introduce API mdsp_rf_set_iq_log() to enable IQ logging on 2nd RF chain
05/08/13     cja     CR514498 Only increment current_buffer_mod_3 once per frame, plus for T2G async
31/07/13     cja     CR514255 Only add generic config commands when issue message to FW
10/06/13     cja     CR498279 Do not increment current_buffer_mod_3 twice in G2W only frame
02/08/13     pg      CR447578: Enable partial tone detection only during X2G
7/16/13      zc      CR470197 Replace memcpy with memscpy
06/08/13     ws      CR521868: Introduce API mdsp_rf_set_iq_log() to enable IQ logging on 2nd RF chain
29/07/13     sk      CR519663 Partial QBTA bringup changes
12/07/13     dv      CR503516: F3 cleanup 
11/06/13     ps      CR482677 - Mutex protection when reset the command buffers 
21/06/13     sk      CR502900 Ensure GSTMR2 uses correct host interface segment to populate the commands
21/06/13     cs      Major Triton TSTS Syncup
04/06/13     cja     CR495095 Correct rx_qual_hard in SCH decode results
15/02/13     br      CR451260 Stop issuing generic commands when Fw is asleep.
22/05/13     ap      CR485912: Revised GFW state transitions
20/05/13     pg      CR475490: Correct AMAM/AMPM table index toggling
09/05/13     cs      TSTS Updates
01/05/13     jk      CR477606:Changes to support both RF IQ logging and GSM XO CAL
25/04/13     pjr     CR480105 updated parameters for mdsp_dtm_reset_rx
22/04/13     pjr     CR478293 dual dataspaced mdsp_cmd_offset, mdsp_current_amam_buffer
                     and mdsp_current_ampm_buffer
15/04/13     cja     CR475568 Correct T2G data metrics
31/03/12     ws      CR466973 - Triton DSDS BU: WRite Tx burst offset to 2nd Sub in DSDS mode
28/03/13     cja     CR466302 Don't increment current_buf_mod3 if already done by immediate command
28/03/13     jk      CR464952:Handling ping pong buffer used for IQ logging
28/03/13     jk      CR452710: Changes to support continuous IQ logging and GSM XO calibration
25/03/13     cja     CR466375 Check for errors when writing first command into buffer
22/03/13     cja     CR462223 Clear monitor CCS buffers if gap aborted, reduce F3
20-03-13     pjr     CR465451 modified init of mdps private and public data structures
12-02-13     pjr     CR451837 modified mdsp_log_qdsp6_async_cmd to log correct
                     async command. Changed mdsp_init to calculate the correct
                     pointer to pMdspHostResultsStore for GERAN_ACCESS_STRATUM_ID_2
01/02/13     cja     CR446356 Set msgr_init_hdr 'from' parameter as MSGR_GERAN_GL1
01/02/13     cja     CR448030 Clear gap commands for aborted gap
24/01/13     cja     CR443889 Tidy up rat_info.mode
23/01/13     jk      CR427351: passing pointer to the thermistor read CCS event buffer to RF
22/01/13     cja     CR443834 Extend mdsp_awake to indicate FW enable/disable
19/01/13     cja     CR438712 Set FN to match command FN/offset for mdsp immediate cmd
18/01/13     jj      CR432203 During IQ capture, the ping pong buffer being used is incorrect
16/01/13     cja     CR440981 Dime TD re-instate check fo FCCH results seq num
04/01/13     cja     CR437603 Remove use of AsyncImmediateCmd, always use AsyncCmd
10/12/12     cja     CR411308 Dime T2G changes (msg router to GFW)
07/12/12     cja     CR429173 Remove Dime compiler warnings
05/12/12     pg      CR427361: GFW/RF shared memory interface cleanup
28/11/12     br      CR383165 Removed unsed variable to avoid compilation warnings
26/11/12     pg      CR423852: G2W startup can be scheduled in next frame on Dime modem
26/11/12     ws      CR424198 - API backward compatible with TRITON DSDA changes
22/11/12     cja     CR414098 For Dime only need W clks if W neighbours
19/11/12     cja     CR422874 Add pseudo Hw FN for Dime
08/11/12     npt     CR419262 - Ensure that sequence number is updated correctly in command buffers
31/10/12     pg      CR409233: updates for G2W on Dime modem
03/10/12     dv      CR404964: Race condition when 2 T2G CMDs issued in 1 G frame
16/10/12     ws      CR409719 - DIME/TRITON FTM API compatability support
05/09/12     cja     CR385323 If restart gsmtr also restart MDSP
04/09/12     og      CR395649. T2G Acquisition and BSIC decode enhancements.
20/08/12     cja     CR385484 Add flag for async interrupt in num cmds
17/08/12     cja     CR379606 Add check for command buffer overflow
15/08/12     cja     CR384314 Reset mdsp_cmd_count as part of mdsp init
03/08/12     og      CR383632. Don't request a tuneback to GSM when the G2L startup request
                     is not sent to LTE L1.
26/07/12     ss      mdsp_clear_buffer - All SBI buffer not cleared
22/07/12     jj      CR375828 [KW] Buffer overflow, array index of 'gfwGrfcBuffers'
                     may be out of bounds.
24/07/12     dv      CR377931 Only check SBi/GRFC buffers when they are used.
19/07/12     tc      CR380260 Gl1 interface update for MOD indicator
09/07/12     cja     CR377216 If gstmr stalled, attempt to re-start it
19/06/12     dv      CR371669: G2T Crash due to seq_num being re-used
08/06/12     cja     CR363269 Do not clear IRAT SBI/NonSBI buffers
25/05/12     cja     CR363431 Suspend G power measurements during T measurements
23/05/12     jj      CR364221 remove FEATURE_GSM_TX_PCL
21/05/12     cja     CR360514 Set mdsp init complete, even if SW/FW interface mismatch
17/05/12     og      CR362647 Stop generic config command updates in T2G gaps
14/05/12     og      CR360987. Avoid sending the generic config update when a gap
                     is aborted in T2G.
01/05/12     ws      CR355391 - Deprecate arm.h, armasm.h and hw.h
17/04/12     cja     CR352538 Clear command buffer after FW disable.
12/04/12     dv      CR351558 Ensure num_cmds reset after each set of T2G calls
22/11/10     ky      mdsp_nv_recovery_restart_type is defined and this structure is pouplated after
                     reading nv item from efs. if the recovery restart is enabled then mdsp drivers
                     ignore the errors other wise it does error fatal and also dump the mdsp memory.
                     mdspsvc provided api to update this nv item read value. so updated this value
                     during mdsp_init. Calling mdsp_vote_for_recover_dsp function is disabled until
                     MDSPSVC releases code for this
02/04/12     og      CR348583. Clear num cmds, sbi, grfc in mdsp_first_frame( ) for
                     the CM case only.
29/03/12     cja     CR347401 Make stop acq SBI/GRFC triple buffered
29/03/12     cja     CR341225 RxFE recovery for X2G
26/03/12     pg      CR346657: Update SBI/GRFC buffer clearing for tripple buffering
22/03/12     ws      CR345988 - Featurization FEATURE_GSM_TDS for further compilation errors
15/03/12     ws      CR344126 - remove VAMOS featurisation around GFW_DEBUG shared variable
09/03/12     dv      T2G:CR342537 Triple buffer all monitor buffers
13/03/12     ws      CR335057 eSACCH buffer flush and restore after inter-cell handovers
09/03/12     rc      GL1 QPA new shared variables
22/02/12     dv      T2G:Re-introduce mdsp_check_for_errors()
20/02/12     dv      T2G:Remove abberrations from bringup check-in
10/02/12     ws      CR 336194 Enable TD-SCDMA IRAT for GERAN
06/02/12     cja     CR334358 Ignore semaphore in test for commands pending
06/02/12     cja     CR331207 Ensure seq num is in sync with command buffer
06/02/12     cja     CR331207 Do not add semaphore to empty buffer in CM, use internal num_cmd
03/02/12     cja     CR332184 Remove semaphore for CM
02/02/12     cja     CR329883 Ensure seq number written before semaphore in command buffer
02/02/12     cja     CR328529 Optimise SBI/Non-SBI transfer
02/02/12     cja     CR329327 Ensure mdsp init complete before using command buffer
02/02/12     cja     CR328278 Use semaphore in MDSP command buffer header
02/02/12     npt     CR326943 - Clear mdsp command buffer on wake up from sleep.
27/01/12     jj      CR329722 add indication for  scheduleTimeError
27/01/12     ky      Added mechanism to disable/enable vamos in firmware
23/01/12     ky      vamos changes
14/12/11     jj      CR325467 making "rfm_log_iq_data" called when log_iq flag is ON
07/12/11     ws      CR 323157 Compilation errors when FEATURE_GSM_TO_LTE is not defined.
07/11/11     jj      315253 IQ capture capability for GSM via QRCT
11/11/11     jj      CR318563 Klock work compiler warning correction
28/10/11     jj      CR308254 Correction to RF interface for the dc spur removal
19/10/11     ws      CR313663 - Fix compile error in mdsp_build_spi_buffer()
11/10/11     jj      CR311863 GL1 interface to shared variable  "spiAntTunerDelta " to fw
13/09/11     jj      CR 306047 Antenna tuner interface from rf driver to fw
08/09/11     jj      CR302378  Added GL1 changes for RSB along with spectral inversion
02/09/11     jj      CR304819 Add interface to update smps pdm to dynamic tx buffer
31/08/11     jj      CR304393 Dynamic tx path delay set interface for rf driver
25/08/11     jj      CR301918 Add interface to flag 'disableGpSync'
23/08/11     jj      CR302678 corrected the sw fw version mismatch
23/08/11     ws      CR301096 corrected typo in previous checkin
12/08/11     cja     CR301096 Pass index to RF driver for startup and cleanup buffers
12/08/11     cja     CR301307 Change g2x cleanup time as starup not tune
24/06/11     cja     Nikel AMAM/AMPM table definition change
24/06/11     cja     CR293168 Correct setting SCH Cmd enableAeqOnSch so not overwrite grfcSbiBuffIndex
17/06/11     cja     Remove Nikel BU workarounds
08/06/11     cja     Nikel G2W changes (so WFW does startup)
07/06/11     ws      Added mdsp_program_cal_amam_table() for pre-distortion cal
06/06/11     ws      Fixed mdsp_update_parange_init_data() to write to correct r1 GRFC register
18/05/11     cja     Add last_tx_cal_cmd, txDacGrfcNumber and GFW Interface revision check
16/05/11     cja     Add setting GFW_SLOT_MODE_CTRL_REG_ADDR shared GFW/RF variable
09/05/11     cja     Correct ID for freq command
28/04/11     cja     Add Nikel freq correction by GFW
26/04/11     cja     AM-PM GFW interface change for Nikel
21/04/11     cja     Merge from tip of rel/14.00 to Nikel
20/04/11     cja     Nikel GFW I/F changes
05/04/11     cja     L1 I/F change for Nikel IRAT
11/04/11     pg      CR282109: Ensure AEQ PH2 switching only occurs at a block boundary
08/04/11     ky      Brought SPI feature changes under FEATURE_GSM_RAFT_SPI_CONTROL
07/04/11     ky      Changed sbi_infor pointer to rtr_sbi_infor pointer in SPI feature
09/03/11     ky      CR278054: Increasing the priority of the task/isr when it is executing
                     mdsp_nonsbi_write function. This change is to avoid preempting the
                     gsm l1 task when executing this function.
21/03/11     ky      CR261598: Reinitialising the sbi/grfc pointers for each step while populating the
                     tx cal command
08/03/11     cja     Initial IRAT for Nikel
28/01/11     cja     Changes for RxLM and TxLM for NikeL
15/03/11     og      CR279056. Add mutex protection to setting the pending app mode command.
18/02/11     cs      CR275882 Remove the mdsp isr initialisation code as causes reset
14/02/11     npt     CR264682 Add support for asynch dsp completion interrupt and removed OPT3 stubs
08/02/11     cja     CR264690 Acquisition frequency capture range larger
31/01/11     pg      CR270330 Add FEATURE_GSM_DYNAMIC_AEQ_8PSK
27/01/11     npt     Added Stubs for OPT3 development to allow QDSP6 to link.
                     mdsp_asynch_dsp_int_enabled() and mdsp_set_asynch_int_enabled()
27/01/11     ky      GFW_PACFG2RO_GRFC_INFO & GFW_PACFG2R1_GRFC_INFO no longer used after
                     introducing tx pcl ind feature. so using GFW_TX_PA_GRFC_INFO even when
                     tx pcl ind feature is not present in the code
25/01/11     og      Fix for CR271256.
21/01/11     ky      271848:For tx cal cmd, initialise Unused pointers from grfc buffer
                     (pStaticGrfcBurstBuf) and from sbi buffer
                     (staticSbiBurstBufferPointer, staic pointers) to NULL
12/01/11     ky      271064:Gsm tx pcl ind feature changes for Q6 targets
12/01/11     ky      Lint & compiler warnings fixes
11/01/11     cgc     Fix compiler warnings add cast
30/11/10     og      Adding G2L support.
19/11/10     ky      Enhancements to pre distortion calibration. Added modulator_start_time_offset_adj
                     tx cal structure so that rf driver controls the start time of the
                     modulator
03/11/10     ky      fixed compiler warnings for 9k
29/10/10     cgc     Defer setting number of cmds until the last was sent to mdsp
                     on the 1st frame on sleep wake up
29/10/10     ky      261817: Added two wait for frame tick in the hw init so that
                     app mode command sent to the fw and fw will process that command
                     Hardcoding the app mode command moved to switch command buffers function
25/10/10     ky      259645: Installed gstmr handler for sending pending commands is not working.
                     Hardcoded the sending these commands in gstmr isr function.
18/10/10     ky      259645: Installed gstmr handler for sending pending commands. This handler
                     sends the generic config command and app mode command to fw if they are
                     pending
18/10/10     og      FEATURE_LTE_TO_GSM
14/10/10     ws      FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
13/10/10     ky      258524: Ignoring G2X_STARTUP_BURST,GFW_X2G_STARTUP_CMD,GFW_X2G_CLEANUP_CMD
                     and G2X_CLEANUP_BURSTs in mdsp_use_nonsbi_static_buffer function
07/10/10     ky      254466:Removed fw unused shared variables from the sw code
08/10/10     og      Fixes to CR217386 and CR182441.
30/09/10     ky      9K: CR 257913: GENERIC config command will be sent to fw after
                     populating all the shared variables into a pending buffer
23/09/10     tjw     Replace deprecated clk_busy_wait with DALSYS_BusyWait
09/09/10     ky      255066:9k:As there is no cache in sw side so sw dont require to do
                     the cache invalidation. Removing this functionality form mdsp drivers
03/09/10     ws      Added FEATURE_GSM_MDSP_AEQ_SCH for QDSP6 targets
27/08/10     ky      250646: added a intlock protection for mdsp update shared variable function
25/08/10     pg      CR251505: Toggle Tx shared memory buffers only when they are updated
12/08/10     ky      CR 242606: reg element in mdsp_pending_nonsbi_write_type needs
                     to be changed from 16 bit to 32 bit for Q6
10/08/10     ws      Corrected Q6 Featurisation for MOB environment
19/07/10     ky      Klocwork fixes
13/07/10     ky      Fixed 9k warnings
14/06/10     cja     Fix compilation error.
09/06/10     cja     Optimise shared memory buffers to allow 38 SBI transactions in compressed mode.
07/06/10     pg      Re-instate execution of FWS_APP_DISABLE in DPC, as IRAT
                     crash issue has been fixed in Rex code (CR239406)
28/05/10     ky      Multi step per frame TX CAL and pre distortion cal changes
26/05/10     dv      revert change by ws(24/03/10) as its causing IRAT crash
23/05/10     dv      fix compile error in 9k irat update
20/05/10     og      9k irat updates.
17/05/10     pg      Reinstate function stubs for mdsp_grfc_* for Q6 targets
17/05/10     ky      Added single burst decode changes for 9K
13/05/10     pg      Further removal of legacy Q4 to allow removal of mdspi.h for Q6 targets
16/04/10     ky      Added first tx cal cmd flag to rf drivers interface,firmware and correctly
                     populating the grfc and sbi buffers for each step frame
07/04/10     ky      Replacing mdsp_memcpy with memcpy_v function
06/04/10     cja     Seperate function for feature check
24/03/10     ws      Execute FWS_APP_DISABLE in DPC to avoid ISR overrun
18/03/10     ws      Added FEATURE_GSM_RFA_IRAT to protect 10 monitor code for now
17/03/10     og      Further x2g and g2x updates.
10/03/10     ws      Further featurisation of G2X and X2G
05/03/10     og      Ensure the correct grfc and sbi pointers are filled in
                     the cm startup and cleanup commands.
01/03/10     og      g2x and x2g updates.
26/02/10     og      Added g2x start and stop commands.
25/02/10     og      Added cm fw enable/disable and set app mode.
22/02/10     og      Increase the maximum number of compressed mode power
                     monitors to 10 per frame.
23/02/09    ky       Stubbed out the mdsp_grfc_tristate, mdsp_grfc_out,mdsp_grfc_polarity, mdsp_grfc_omode
                     functions for QDSP6
04/02/09    ws       Removed leagcy Q4 MDSP services and image for Q6 targets
01/02/10    ky       Extracting Ncell SCH Burst metrics should happen from gfwSchRxBurstMetricsBuffer
                     instead of gfwAsyncMetricBuffers for Q6.
28/01/10     ky      Use MDSP MEMCPY while tx cal command filling. Added the tx cal cmd
                     to mdsp_issue_command_qdsp6 function
22/01/10     ws      Use dcache_inval_region() to invalidate cache for compatability
                     with BLAST OS
15/01/10     ky      Removed FEATURE_GSM_TX_CAL_QDSP6 to enable tx cal command
22/12/09     ws      Fixed semaphore len check in mdsp_get_async_decoded_data() to
                     indicate when data is ready
14/12/09     ws      Fixed memcpy size when copying data for continuous Tx
08/12/09     ws      Fixed mdsp_update_parange_init_data() to update PA_CFG Ro and R1
                     Corrected mdsp_get_async_rx_metrics() to use correct sequence numbers
30/11/09     ws      Added FEATURE_GSM_TX_CAL_QDSP6 to protect previous checkin
26/11/09     ky      Added the support for TX CAL comamnd
25/11/09     ws      Added support for FWS App disable and renamed SHARED_MEM_START
                     to GFW_SHARED_MEM_START
25/11/09     ky      Number variables in the command is not incremented correctly in shared variable fun
24/11/09     ky      Incorrect featurisation for q6 code. Replaced FEATURE_MDSP_QDSP6 with FEATURE_GSM_GPRS_QDSP6
19/11/09     ky      GFW_STOP_ACQ_CMD grfc list buffer is assigned correctly to this command
19/11/09     ky      Before calling mdsp update shared variable for paTransitionProfile,
                     made a local copy of paTransitionProfile to avoid the casting problems
09/11/09     ky      Removed msg high from issue qdsp6 command and calling q6 version of logging func
06/11/09     ws      Removed MDSP SVC Apps logging as not supported on 9K
05/11/09     ky      changed memcpy to mdsp_memcpy for QDSP6
30/09/09     ws      Renamed mdsp_issue_command2 to mdsp_issue_qdsp6_command
24/09/09     ws      Added support for GERAN Virtual platform
05/08/09     ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
24/06/09     nf      SBD: new threshold function
16/06/09     ap      Fix Lint/compile warnings
09/06/09     nf      Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
26/05/09     cs      CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09     ws      FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
30/03/09     ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
26/03/09     ws      merged taxis specific changes for QDSP6 targets
25/02/09     scm     Don't busy wait on mdsp_frame_tick_count ISR increment.
13/02/09     ap      Lint fixes for high Warning
13/01/09     scm     Add debug message to mdsp_start_wcdma_rf_cmd().
18/12/08     scm     Add debug message to mdsp_rf_sample_ram_cmd().
03/12/08     agv     Added mdsp_start_tx_data_cont for FTM KV Calibration.
30/10/08     ws      CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
23/10/08     agv     CR161524-Fixed the AMPM table pointers for builds with
                     W-AMR as mDSP moved the AMPM from 32bit MEMC to 16bit MEMA.
09/09/08     agv     Added GtW interface change for MUSTANG.
15/08/08     agv     Added Linear PA 128 word AMPM change for LCU
01/08/08     agv     Changes for LCU G2W Interface Change for HW clock bug.
19/03/08     agv     LCU RF Spur Interface Support.
18/03/08     agv     Fix mdsp_am_am_tbl_size setting for non LCU builds.
18/01/08     agv     LCU RF Linear PA Interface Support.
05/03/08     ws      Added ESAIC functionality to Async Rx interfaces
14/01/08     agv     New feature for Switched Mode Power Supply which will
                     provide better talk time.
24/12/07      nt     Added the GCF flag to the mDSP interface for async/sync_rx cmds
03/12/07     cs      Fix CR 128185 to compile for targets that dont't support
                     ANT1 gating
19/10/07     ed      Fix for CR 128185. Clk Source to  Antenna 1 chain of Adie needs
                     to be gated by GSM using MDSP
27/09/07     ed      CR: 117937 - Need to reduce inter-band rf tune time (currently 10-15ms)
                     Added Async Peek/poke command fr this CR.
22/10/07     ws      Add support for ESAIC NV 2508 item control
05/09/07     efytal  Add new function for Phoenix GSM support in FTM mode
24/07/07      cs     Ensure buffer indices and grfc_buf are reset at startup
23/07/07      og     UOOS changes, reduce number of measured samples.CR119147.
23/07/07      cs     CR 111539 Backout some of the previous change as fix in RF drivers now
16/01/07     agv     Fix for AMAM table update. RF driver does not re-signal
                     an AMAM update if previously the same UL TS were set.
28/11/06      ws     High Lint warning fixes
21/04/06     agv     Added FTM feature for continuous 8PSK tx needed by Revathi.
02/20/06     gfr     Mainline command and apps logging features, consolidate
                     task waiting functionality
01/13/06     gfr     Add include file required for latest RF changes.
12/19/05     gfr     Check if mDSP is awake before clearing the ISR semaphore
                     when registering a new ISR.
12/09/05     gfr     Added mdsp_load_nv function to abstract loading of all
                     mDSP NV items.  Move mdsp_debug_init to mdsp_init to avoid
                     writing to mDSP before it is ready.
11/04/05     gfr     Use the right ramp when calculating the non-polar PA
                     table offsets, add mdsp_ftsm and mdsp_set_ftsm to replace
                     global variable
11/04/05     gfr     Move ciphering code to mdsp_private.c
10/17/05     gfr     Add the mdsp_saic flag
10/18/05     gfr     Add support for having the mDSP do all ciphering
10/04/05     gfr     Improve debug msg for "Unexpected empty command buffer"
09/23/05     gfr     Optimize DM active period to just during PDTCH decoding
09/22/05     gfr     Removed FEATURE_SBI_MDSP_CONTROLLER
09/12/05     gfr     Add check for SAIC support mismatch
09/01/05     gfr     Do not issue new GRFC writes if we already have pending ones
09/01/05     gfr     Buffer GRFC writes if we are in the gstmr delayed window
08/26/05     gfr     Use narrower filter
08/19/05     gfr     Use mdsp_burst_current_buffer to get buffer index
08/17/05     gfr     QCLint cleanup - shorten lines to less than 90 chars
08/10/05     gfr     Only set early decoding bypass bit if decode is set
08/05/05     gfr     Improved cmd buf overflow error message
08/01/05     gfr     Delay nonsbi writes if the gstmr isr is pending
07/29/05     gfr     DTM support
07/08/05      bk     Disabled the setting of the channel filter coeff
                     (wider filter now in effect)
07/05/05     gfr     Set the default channel filter coefficients and add
                     mdsp_nonsbi_writes_pending
06/13/05     gfr     Only panic once per reset
06/09/05     gfr     Buffer nonsbi_writes if we are asleep, add grfc logging
05/25/05     gfr     Recover from command buffer overflows
05/18/05     gfr     Ignore unhandled interrupts, disable extra NOPs if the
                     HW fix is in place
05/18/05     gfr     Mainline new SBI format
05/18/05     gfr     Clean up includes
05/03/05     gfr     Mainline FEATURE_GSM_MDSP_EER
04/28/05     gfr     Add support for the DM active interrupt and mdsp_inact
04/21/05     yh      Support for panic reset
04/12/05     gfr     Clean up early decoding interface
04/08/05     gfr     AMR phase is a uint8 not a boolean, minor cleanup
04/08/05     gfr     Add force_reset flag to download_and_startup
04/07/05     gfr     Print out fw version at startup
04/05/05     gfr     Do not print out an error interrupts when asleep
03/14/05     yh      Support for mdsp apps log.
03/07/05     gfr     Do not print out an error for every unhandled interrupt,
                     set the use rotator bit for the continuous tx command
03/09/05     yh      API change for mdsp_get_decoded_data() and
                     mdsp_get_async_decoded_data().
02/14/05     gfr     Support for new Raven SBI format
02/11/05     gfr     Use MDSP_RX_HDR_SIZE when calculating data length
02/05/05     gfr     Increase the number of GSM mDSP ISR handlers.
02/02/05     gfr     Add configure_IR_window command.
01/28/05     yh      mdsp_download_and_startup takes 2 signals from NPL1
                     as parameter
01/24/05     gfr     General cleanup and restructure.
01/18/05     gfr     Set the CM_SWITCH_EN bit for g2w SBI transactions
01/12/05     gfr     Support for single mDSP ISR targets,
                     mainlined FEATURE_MDSP_USES_MODULES
12/22/04     gfr     Support pending NonSBI transactions instead of
                     mdsp_wait_for_frame_tick
12/20/04     gfr     Early warnings if mdsp_nonsbi_write is called with interrupts
                     locked or no handler installed.
12/14/04     gfr     Do not dump mDSP memory when rxqual is zero.
12/15/04     yhong   Added indicator for having gstmr and gprs tx traces in
                     mdsp memory dump
12/14/04     yhong   Mdsp memory dump version 3 with gstmr and gprs tx traces
12/07/04     gfr     Check if handler is installed when waiting for frame tick
12/06/04     yhong   Support for FEATURE_GSM_MDSP_EGPRS
12/06/04     gfr     Featurize the mDSP SBI controller
12/02/04     gfr     Support for EER and polar RF.
12/01/04     yhong   Support for multiple wakeup retries if mdsp fails to wake up in
                     the first try
11/18/04     yhong   ERR_FATAL when download timeout
11/04/04     yhong   Handle MDSP_MOD_DISABLE events with 2 reasons. It is wrapped on
                     FEATURE_MDSP_ATTEMPT_RECOVERY
11/02/04     gfr     Fix gsmError compilation.
10/12/04     gfr     Add checks for late or unexpected sleep isrs from the dsp,
                     consolidate mdsp memory dumping parameters.
10/05/04     gfr     Support for new 32-bit mdsp error codes.
09/28/04     gfr     Enhanced memory dumping and error check command buffer length.
09/08/04     gfr     Mask off asleep bit when querying if dsp is ready for sleep.
08/26/04     gfr     Lint cleanup and enhanced memory dumping.
07/22/04     gfr     EGPRS support.
07/08/04     gfr     Print an error if sync rx data is not available, removed
                     SACCH specific code and LOOK_FOR_FACCH channel type hack.
06/29/04     gfr     Temporarily suppress metrics no available error.
06/28/04     gfr     Fill in look for facch field and print an error if
                     metrics are unavailable.
06/25/04     gfr     Add mdsp_process_error to consolidate error processing.
06/24/04     gfr     Add access function for mdsp_awake_flag
06/17/04     gfr/gw  Changed mDSP interrupt used by sleep interface from 2 to 4.
06/14/04     gfr     Support for mdsp sleep interface.
06/10/04     gfr     Support for early CBCH decode, and general async rx cleanup.
04/26/04     bm      Mainlined FEATURE_MDSP_NO_STATIC_BUFFERS
04/09/04     gfr     current_buffer_mod3 for the debug buffer seq_num
04/02/04     bm      Added two new functions mdsp_gsm_drv_go_to_sleep,
                     mdsp_gsm_drv_wakeup to be called from wcdma sleep
03/05/04     gfr     Lint cleanup and remove uwire code.
02/17/04     gfr     GSM rotators support.
02/09/04     gfr     Skip validating the metrics SNR.
01/30/04     gfr     Support for firecode and logging mdsp scaling factors.
01/26/04     gfr     Add compile-time check for AMR support mismatch.
01/26/04     gfr     Skip mdsp results validation for async rx metrics.
01/16/04     gfr     Support for mdsp results validation.
12/15/03     gfr     Cleaned up mem dumping routine, disable PSBI during the dump.
11/05/03     jcw     Moved mdsp_check_sbi_semaphores() to mdsp_process_sbi_transactions()
10/28/03     gfr     Added FEATURE_LOG_MDSP_CMDS_CONTINUOUSLY.
10/15/03     gw      Moved mDSP clock switching into mDSP services when
                     FEATURE_MDSP_SVC_DOES_CLK_SWITCHING is defined.
10/14/03     jc      Prevent sporadic 0x100 errors in compressed mode. CR33289
09/23/03     gfr     Allow retrieving of debug buffer at arbitrary intervals.
09/10/03     gfr     Adjust debug buffer pointers after early decode.
08/27/03     gfr     Support for dumping mdsp memory to flash on error.  Cleaned up
                     command logging code.
08/26/03     gfr     Do not wait for mdsp tick if we are in irq mode, print
                     an error message instead and continue.
08/21/03     gw      Added function mdsp_reset_async_rx_ptrs().
08/16/03     jc      Removed mdsp_set_sbi_burst_buffer_eot() now done in rf driver.
08/06/03     gw      Added function mdsp_abort_early_decode().
07/23/03     jtn     Some renames for #defines for MDSP SBI transactions moved
                     from mdsp_intf.h to sbii.h.
07/18/03     jc      Revert wait_for_frame_tick_back, but allow nonsbi writes to
                     to be closer to end of frame ie 1200 symbols into frame.
07/16/03     jc      AMR fix for seq num.  Wait for frame tick use gstmr odd/even bit.
07/07/03     jc      Really remove checking of stop_acq semaphore.  Add check
                     for correct GSM fw image is loaded. remove kovuramversion
06/30/03     jc      Improve semaphores error printout by >> 16 bits.
06/23/03     bm      Removed compilation warnings inside mdsp_sync_rx_early_decode
06/24/03     jc      Remove checking of stop_acq semaphores for now.
06/21/03     jc      Fix for rf restructure using static nonsbi buffers for gtow
06/20/03     bm      Modified mdsp_nonsbi_write to wait for a frame tick if current
                     time is near to the end of frame.
                     Removed compilation warnings inside mdsp_build_sbi_static_buffer
06/12/03     gw      Swapped order of issuing commands for early paging
                     decode to make sure MDSP_skipMsgCols is always left
                     in correct state.
06/09/03     jtn     Added function mdsp_pa_profile_last_rampon_value().
06/06/03     jc      Overload STOP_ACQ onto MON_BURST_9 instead of MON_BURST_3.
                     Convert ERR_FATAL to MSG_ERROR.  Use bitmasks for when
                     semaphores errors occurs.
06/04/03     gfr     Add functions to retrieve the search width.
06/02/03     gfr     Add frame number to error messages.
06/02/03     jc      Lint cleanup again.
05/21/03     gw      Added support for early paging decode.
05/15/03     gfr     Added option to log mdsp commands only when an error
                     is encountered - FEATURE_LOG_MDSP_CMDS_ON_ERROR.
                     Also removed hack to set power profile on every burst.
05/12/03     gfr     Added tx_alpha parameter to transmit command.
05/08/03     jc      Changes for RF restructure.
05/02/03     hg      Added GSM AMR support.
04/28/03     gfr     Improved command logging support.
04/23/03     thh     Added the function mdsp_set_tx_alpha() to allow the RF
                     driver to set the Tx alpha at the next frame tick.
04/02/03     jc      Use new enum MDSP_MAX_NONSBI_REQUEST_TRANSACTIONS.
04/02/03     jc      More lint cleanup.
04/02/03     jc      Mainline RF_MULTIMODE, use GSTMR_GET_FN_GERAN(gas_id) instead of gl1_get_fn.
04/01/03     pgh     Lint clean up
03/28/03     gfr     Log mdsp commands if DEBUG_LOG_MDSP_CMDS is defined.
                     Report frame number on sbi/non-sbi not empty errors.
03/20/03     thh     Always load the PA profile as commanded even if the same
                     profile has been loaded.  This fixes the problem where
                     there is a bad/corrupted profile in the hardware and the
                     software refuses to re-load.
03/14/03     gfr     Changed stop_acq to use the MON_BURST_3 buffer.
03/03/03     jc      Fix for start/stop_tx_tone for phoneT.
02/26/03     jc      Change sample ram duration to 2312 qs.
02/24/03     drl     included timedtest.h for visibility to TIMETEST_PORT
02/05/02     ATM     Made changes for MDSPSVC API mods (6300)
02/04/03     jc/ma   Lint cleanup.
01/31/02     jc      Increase sample ram duration to 2.2ms per twilborn.
01/31/03     jc      Increase sample ram duration to 2.5ms per twilborn.
01/16/03     jc      Correct cut and paste errors in variable init cleanup.
12/18/02     jc      Removed a couple of variable initializations - redundant.
12/13/02     ih      Removed unused variable max_trans in
                     mdsp_set_sbi_burst_buffer_eot.
12/11/02     thh     QDSP_CMD_SUCCESS/FAILED was renamed to MDSP_CMD_SUCCESS/FAILED.
12/05/02     jc      Support for FEATURE_HWTC.
12/04/02     jc      Semaphore checking reenabled for GPRS.
11/18/20     ih      Added code to invalidate previous PA profile configuration
                     on init to fix problem where PA profile is not loaded on
                     RAT switch.
11/03/02     tb      Added support for INTERRAT_GTOW
10/28/02     gw      Added support for back-to-back async receives.
10/10/02     gr      Support for ping-pong debug buffers.
10/07/02     jc      Added variable frame_tick_setup_margin.
10/07/02     jc      Corrected FTSM cut and paste error in start/stop acq.
10/03/02     ih      Changed MDSP_UWIRE_CTL_FIFO to MDSP_UWIRE_CTL_FIFO_TIMED
                     to support burst uwire transactions.
10/01/02     jc      Moved sbi_burst_status_buffer init into mdsp_sbi.c
09/28/02     jc      Moved FTSM for start and stop acquisition into this driver.
09/25/02     ih      Separated mdsp_sbi_status_buffer into burst and access
                     buffers
09/23/02     jc      Added new phoneT function - mdsp_start_tx_rnd_cont.
09/18/02     jc      Moved some functions into mdsp_sbi.c.  Added new
                     functions for phoneT to get grfc shadow regs.
09/16/02     ih      Added CTL_FIFO support for the upcoming firmware
09/11/02     sd      Added support for RF driver multimode API that includes
                     GSM drivers.
09/12/02     gr      Removed MDSP write that occured before initializing
                     MDSP memory table.
09/09/02     gw/ih   Added new functions and modified existing build burst
                     buffer functions to support static SBI/nonSBI buffers
09/06/02     gr      Support for new GPRS PA profile interface.
09/05/02     jc      Added FEATURE_GSM around whole file for WPLT build
08/22/02     gw      Merged in changes for sleep that were on a branch:
                     Added functions mdsp_go_to_sleep and mdsp_wakeup.
08/28/02     jc      Timeout after 500ms if mdsp fw fails to load.
08/07/02     jc      Register the mdsp app only once per power on cycle.
08/07/02     jc      Fix build_sbi_burst_buffer when using mon_burst_10.
                     For the SBI CTL FIFO use 0x57 instead of 7 for compressedmode.
08/02/02     jc      Add !FEATURE_WPLT around the tx functions as not used here.
06/28/02     thh     Defined and use timed mode SBI CTL FIFO for timed SBI
                     transactions.
06/26/02     tb      Change MDSP_RAM_VERSION to MDSP_kovuRamVersion.
06/18/02     tb      Changes to support mdsp services and new image parser.
06/06/02     jc      Added support for up to 9 power measures.
06/05/02     gw      Added third debug buffer.
06/04/02     bbd     Changed FEATURE_GPRS to FEATURE_GSM_GPRS
06/02/02      jc     Add support for async_rx for FEATURE_COMPRESSED_MODE.
05/21/02     plm     Added volatile specifier for mdsp_frame_tick_count to fix
                     infinite loop problem when compiling with -O2
05/20/02     jc      Mainlined CHEETAH_GSM, add support for FEATURE_COMPRESSED_MODE
05/08/02     thh     Added support for SBI reads.
04/25/02     ih      Implemented dynamic MDSP buffer allocation. Changed code
                     to get buffer offsets and addresses using table lookup.
04/11/02     jc      Change comp_factor to int16 in mdsp_set_pa_profile().
04/11/02     jc      Don't check semaphores for GPRS. Rearranged sbi code.
04/10/02     thh     The reserved fields in the SBI control units are
                     now explicitly set to 0.  The compiler does not implicitly
                     set these to 0.  The MDSP was using these reserved fields
                     to mis-configure the SBI transactions.
                     Define MDSP_UWIRE_DATA_MAX_NUM_BITS to be the maximum
                     number of bits for the uwire data for both KOVU and
                     Cheetah.
04/03/02     thh     Added support for txprofile and SBI's new API. Only the
                     SBI driver is now allowed to access MDSP's SBI and uwire
                     reads/writes.
04/02/02     mk      Added support for CHEETAH_GSM.
03/28/02     dp      Merged in Gabe's changes to rename the phone_T commands
03/21/03     gw      Added support for a third async rx metrics buffer.
03/13/02     thh     Added mdsp_set_pa_precharge_data().
                     The function mdsp_set_pa_profile(...) now takes 3 parms:
                     step size, pointer to the on-ramp and pointer to of- ramp.
03/08/02     gw      Added support for multiple sbi/uwire and nonsbi writes
                     during initialization.  Mainlined T_KOVU.
03/04/02     gw      Changed sbi and nonsbi buffers from ping pong buffers
                     to cirucluar sets of three buffers.
02/20/02     dp      Changed mdsp_get_async_rx_metrics interface to return
                     metrics availability.   Added fifo buffer for offsets
                     and seq nums as in sync rx
02/04/02     jc      Added cipher control cmd. Removed freq correct cmd.
                     Added support for gprs tx which requires 3 sbi/nonsbi
                     buffers instead of 2. Added mdsp_check_semphores().
02/04/02     mk/gw   Added mdsp_async_rx2() to support using sync Rx for
                     ncell BCCH.
02/01/02     thh     The software now responds to all timing advance values
                     received from the BS.
12/12/01     jc      Change buffers for async_rx and power measure.
12/12/01     jc      Cleaned up some unused vars and compiler warnings.
                     Display mdsp fw version number.
12/11/01     thh     The software no longer responds to timing advance values
                     greater than mdsp_max_ta_adjust.
12/11/01     thh     Added range checking for Timing Advance (TA).
12/10/01     thh     Removed ';' at the end of mdsp_build_uwire_burst_buffer()
                     declaration.
12/10/01     thh     Merged in Brian RF support.
                     Removed obsolete mdsp_set_lna().
12/04/01      jc     Added FEATURE_GPRS. Fixed search width to be an even #.
11/12/01      jc     Support for firmware 3-0470.  Semaphore bit is now used
                     on all sbi, nonsbi buffers.
11/06/01      jc     Added support for GPRS. Semaphore is now a bit not the
                     entire word for nonsbi burst buffer.
                     Burst metrics struct changed size.
10/31/01      gw/dp  Added support for logging debug info.  Added support for
                     returning metrics in dedicated mode.  Changed interface to
                     accept the number of data bytes to be copied from DSP
                     memory when retrieving sync,async and SACCH decoded data.
10/24/01      dp     Changed mdsp_start_acquisition to use the monitor buffers.
                     Changed mdsp_async_rx to use the PWR_BURST SBI and GRFC
                     buffers.  This is to avoid conflicts on the TCH when we
                     have an async Rx and monitors in adjacent frames
10/23/01     mk/gw   Mainlined code for multiple power measurements per frame.
                     Added support for firmware PLT0441 buffer organization to
                     power measurements and async receives.  Added function
                     mdsp_get_async_rx_metrics.  Revised interface to
                     mdsp_get_power_measure_results so it is more like other
                     mdsp functions.
10/17/01     jc      Added timetest.h, removed unused phoneT functions, added
                     a couple phoneT cmds, mdsp_nonsbi_write() now uses a word
                     as parameter. Added support for mdsp_loopback_cmd. Added
                     support for firmware PLT0441, which has a larger host
                     interface segment. Merged in KOVU support such that this
                     one file should support SINA or KOVU using T_KOVU to
                     indicate a KOVU platform.
10/16/01     gw      Changes to mdsp_power_measure to improve error handling
                     and correct bug.
10/10/01     gw      Modified handling of buffers for async receives.
09/25/01     mk/gw   Mainlined code under MONITOR_DEBUG. Cleaned up and added
                     support for SyncRx metrics and multiple MON bursts per
                     frame.
09/20/01      jc     Added mdsp_write_tx_data. Change to use SACCH/TCH buffers
                     instead of pingpong method.
09/04/01      JC     Changed prototype for mdsp_switch_commad_buffers().
08/20/01     mk/gw   Added initial Ncell suppot, featurized via MONITOR_DEBUG.
08/01/01      jc     Use MDSP_DEBUG_SEG_SIZE when retrieving debug data and
                     no longer need to check other buffer for data w PLT0416.
07/26/01      JC     Created MDSP_INHERENT_TX_HW_DELAY constant to
                     account for delay in MDSP TX chain.  This is a
                     temporary measure until further analysed.
07/17/01      jc     Add support for plt0415 - ie nonsbi request buffer is
                     now part of general buffer segment.  Remove checking of
                     both ping pong buffers as the mdsp now puts it in the
                     correct area.  Add support for BSIC as part of tx cmd.
06/14/01      jc     Use correct segment with debug buffers.
06/13/01     thh     The build_tx_lut_entry() function now takes 2 parms:
                     the entry number and its value.
05/24/01      jc     Added support for PLT0404, ie debug buffer is now
                     separate from host buffer. New debug struct. Size of
                     host segment increased ie SBI,NONSBI buffers increased.
05/28/01     thh     In previous versions, the Tx burst was late by 21 bits.
                     This version, we have accounted for 12 (4 bits from the
                     modulator and 8 leading bits) of the 21 bits.  There is
                     more work to do to find out where the other 9-bit delay
                     is coming from.
05/07/01      jc     Add support for mdsp firmware 000300(PLT0300). Correct
                     burst offsets to be negative values.
04/26/01      jc     Add support for mdsp firmware 010202(PLT0202). Moved
                     synth_init, synth_write to hw_gsm.c
04/24/01      jc     Added cmd and metrics logging options, Modified tx cmd
                     to accept offset as a parameter. Added code to use
                     seq_num to decode data rather than FN (not activated
                     till bug in mdsp firmware is fixed). Added error
                     checking in mdsp_switch_command_buffer
04/16/01      JC     Moved MDSP download code to this interface module.
03/06/01      jc     New functions added.  Code still under development.
01/05/01      jc     Initial version.
==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "geran_variation.h"
#include "customer.h"
#include "geran_nv.h"

#include "comdef.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_async_intf.h"
#include "mdsp_cmd.h"
#include "mdsp_debug.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "vtmrs_g.h"
#include "rfm.h"
#include "DALSys.h"

#include "rex.h"

#include "msg.h"
#include "err.h"


#include "mdsp_dtm.h"

#include "gl1_mutex.h"

#include "gfw_sw_intf.h"
#include "fws.h"
#include "stringl.h"

#include "modem_fw_memmap.h"
#include "fw_rf_common_intf.h"

#include "string.h"

#include "gl1_msg_g.h"

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "fs_lib.h"

#include "msgr.h"
#include "geran_msg.h"

#include "l1_utils.h"
#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "gpl1_dual_sim.h"
#include "gpl1_grm_intf.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
#include "l1_log.h"
#include "gl1_arbitrator_cxm.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "ftm.h"
#endif/*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_QSH_DUMP
#include "fws.h"
#include "modem_fw_ipc.h"
#endif /* FEATURE_QSH_DUMP */

/*==========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

==========================================================================*/

/* Command buffer length (buffer size is defined in gfw_sw_intf.h = 1024)*/
#define MDSP_COMMAND_BUFFER_LENGTH (sizeof(GfwHostCmdBuff))
boolean nv_for_search_all_w_cell = 0;
/* Definitions to time wait (us) for MDSP to be ready for disable */
#define MDSP_DISABLE_DELAY_LOOP           500
#define MDSP_DISABLE_MAX_DELAY            5000
#define MDSP_DISABLE_MAX_DELAY_COUNT      (MDSP_DISABLE_MAX_DELAY / MDSP_DISABLE_DELAY_LOOP)
#define MDSP_MAX_TX_BURST_OFFSET          2500

#ifdef FEATURE_GSM_TDS
extern gl1_hw_x2g_startup_info_t gl1_hw_x2g_startup_info;
#endif

extern boolean g1l_hw_is_mcpm_tx_on (gas_id_t gas_id);

const char  Gfw_Interface_Version[sizeof(GFW_INTERFACE_VERSION)] = GFW_INTERFACE_VERSION;

uint32 gl1_gfw_nv_recovery;
/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/
extern void antenna_swth_get_nv_item (void);

typedef struct
{
  /* Buffer indices incremented every frame */
  uint8  current_buffer;
  uint8  current_buffer_mod3;

  /* sequence number of current unprocessed command buffer */
  uint16 mdsp_current_seq_num;

  /* Search Width the mdsp will use to search for a rx signal */
  uint16 mdsp_sync_rx_search_width;
  uint16 mdsp_async_rx_search_width;

  uint16 mdsp_async_rx_metrics_seq_num_fifo[3];
  uint16 mdsp_async_rx_data_seq_num_fifo[MDSP_ASYNC_RX_DATA_FIFO_LEN];
  uint8  mdsp_async_rx_data_wr_ptr;
  uint8  mdsp_async_rx_data_rd_ptr;


  mdsp_pending_nonsbi_write_type mdsp_pending_nonsbi_write_buffer[MAX_PENDING_NONSBI_WRITES];
  uint8 mdsp_pending_nonsbi_writes;
  uint8 mdsp_pending_nonsbi_write_rd_ptr;
  uint8 mdsp_pending_nonsbi_write_wr_ptr;
  boolean mdsp_processing_pending_nonsbi_writes;

/* Storage for the tx power control information */
  mdsp_tx_power_control_struct mdsp_tx_pwr_ctrl_data;

#ifdef FEATURE_POLAR
#endif

#if defined(MDSP_gsmDsrSpurParamBuf)
/* RF Spur Data Buffers for the mDSP */
  uint16 mdsp_rf_spur_buffer_offset[2];
#endif

   /* Frame Tick Setup Margin in quarter symbols */
  uint16 frame_tick_setup_margin;

  /* Only set when all cmds written */
  uint32  mdsp_cmd_count;

  uint16 mdsp_cmd_offset;

  /* Callback handler in NPL1 for mDSP halts. */
  mdsp_gsm_panic_cb_type mdsp_panic_cb;
  boolean mdsp_panicking;

  uint16 mdsp_am_am_tbl_size;

  /* AMPM Table is 256 Words for Polar, and 128 Words for Linear PA */
  uint16 mdsp_am_pm_tbl_size;

  uint16 mdsp_is_asynch_int_enabled;

  boolean mdsp_init_complete;

  /* stores the information regarding GFW recovery restart is enabled or not in case of RF CCS bad event state */
  boolean mdsp_gfw_nv_recovery_restart;

  /* Time offset till start of sync rx or sync tx burst */
  uint16    mdsp_sync_rx_time_offset;

  /* Store the version of the firmware */
  uint16    mdsp_gsm_ram_version[2];
  uint16    mdsp_fw_version;

/* SAIC enabled flag */
  boolean mdsp_saic;

#ifdef FEATURE_GSM_MDSP_ESAIC
/* ESAIC ENABLE FLAG */
  boolean mdsp_esaic;
#endif

/* AEQ ENABLE FLAG */
  boolean mdsp_aeq;
  uint8 mdsp_stop_acq_buffer_mod3;
  uint8 mdsp_stop_cont_tx_buffer_mod3;
  uint16   mk_lastSeqNumRead;

  uint8 mdsp_current_amam_buffer;
  uint8 mdsp_current_ampm_buffer;

} mdsp_intf_private_data_t;

/*===========================================================================
  STATIC FUNCTIONS
===========================================================================*/
static void mdsp_pre_switch_command_buffers (gas_id_t gas_id);
static void mdsp_asynch_pch_int_init(gas_id_t gas_id );
static void mdsp_asynch_pch_int_inact(gas_id_t gas_id );

static void mdsp_clear_cmd_buff(gas_id_t gas_id );
static boolean mdsp_nonsbi_write_schedulable (gas_id_t gas_id);
static void mdsp_process_nonsbi_transactions (gas_id_t gas_id);
static uint16 mdsp_inc_current_seq_num (gas_id_t gas_id);

static uint32 mdsp_get_mdsp_frame_tick_count(gas_id_t gas_id );

static mdsp_intf_private_data_t* get_private_dataspace_ptr(const gas_id_t gas_id);

static void mdsp_send_iq_samples_to_tcxomgr(void);
static void mdsp_toggle_iq_buf_index(void);

/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/

mdsp_intf_public_data_t mdsp_intf_public_data[NUM_GERAN_DATA_SPACES];
static mdsp_intf_private_data_t mdsp_intf_private_data[NUM_GERAN_DATA_SPACES];
mdsp_collect_iq_samples_cb_t mdsp_send_iqsamples_to_mcs_ptr = NULL;
static uint8 iq_buf_index = 0;
static uint8 iq_logging = 0;

uint8 tx_div_type2_enable[NUM_GERAN_DATA_SPACES] ;
int16  trafic_mdm_threshold [NUM_GERAN_DATA_SPACES] ,idle_mdm_threshold [NUM_GERAN_DATA_SPACES],gsm_trafi_sens [NUM_GERAN_DATA_SPACES],gsm_idle_sens[NUM_GERAN_DATA_SPACES];
int16  gsm_qsc_large_delta [NUM_GERAN_DATA_SPACES],gsm_qsc_small_delta [NUM_GERAN_DATA_SPACES] ,gsm_qsc_hysteriesis_time_l1 [NUM_GERAN_DATA_SPACES];
uint8  gsm_qsc_idle_pingpong_mitigation_thre [NUM_GERAN_DATA_SPACES], gsm_qsc_traffic_crisis_mode_thre [NUM_GERAN_DATA_SPACES], gsm_qsc_idle_suspension_time[NUM_GERAN_DATA_SPACES];

uint32 mdsp_get_fws_app_mask( gas_id_t gas_id );

boolean mdsp_get_Parallel_SCH_log_data(
          GfwPllelSchLogPckt *par_sch_log_data_p,
          gas_id_t            gas_id );
#ifdef FEATURE_GSM_RF_DEBUG_NV
boolean mdsp_rf_debug_NV_enable_g [NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE(FALSE) };
int16   mdsp_rf_debug_upper_power_limit_g[NUM_GERAN_DATA_SPACES];
int16   mdsp_rf_debug_lower_power_limit_g[NUM_GERAN_DATA_SPACES];
int8    mdsp_rf_debug_band_g[NUM_GERAN_DATA_SPACES];
int32   mdsp_rf_debug_arfcn_g[NUM_GERAN_DATA_SPACES];
boolean flag_for_fw_pwr_meas_crash [NUM_GERAN_DATA_SPACES][MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS+1]={ {INITIAL_VALUE( FALSE )} };
#endif 
/*===========================================================================

                                FUNCTIONS

===========================================================================*/

static void * mdsp_cmd_buf_ptr [NUM_GERAN_DATA_SPACES]= {INITIAL_VALUE( NULL )};
void * mdsp_getcmd_buf_address(gas_id_t gas_id)
{
  return(mdsp_cmd_buf_ptr[gas_id]);
}

/*===========================================================================

FUNCTION mdsp_init_public_data_space

DESCRIPTION
  This function initialises a public data space

===========================================================================*/
void mdsp_init_public_data_space(gas_id_t gas_id)
{
  mdsp_intf_public_data_t* data = get_mdsp_intf_public_dataspace_ptr( gas_id );

  /* BSIC to be used as part of every RACH transmission */
  data->mdsp_BSIC = 0x0;

  /* Tx rotator enable bit */
  data->mdsp_tx_rotator = FALSE;

  /* Counter to implement dsp_wait_for_frame_tick(). */
  data->mdsp_frame_tick_count = 0;

  data->mdsp_tx_burst_offset = (QS_PER_SLOT * 3 + FRAME_TICK_SETUP_MARGIN_QS);

#ifdef FEATURE_GERAN_RF_SELF_TEST 
  data->gsm_selftest_params.selftest_flag =FALSE;
#endif /*FEATURE_GERAN_RF_SELF_TEST*/

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
/* AEQ Ph2 8PSK ENABLE FLAG */
  data->mdsp_aeq_8psk = FALSE;
#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
  data->mdsp_aeq_8psk_dynamic = FALSE;
#endif /* FEATURE_GSM_DYNAMIC_AEQ_8PSK */
#endif
#ifndef FEATURE_GSM_RX_TX_SPLIT
  data->tx_config_cmd_pending = MDSP_TX_CONFIG_NO_CMD_PENDING;
#endif
#ifdef FEATURE_GSM_TDS
  data->external_control = FALSE;
#endif
  data->mdsp_async_imm_cmd_sent = FALSE;
}

/*===========================================================================

FUNCTION mdsp_init_private_data_space

DESCRIPTION
  This function initialises a private data space

===========================================================================*/
void mdsp_init_private_data_space(gas_id_t gas_id)
{
  mdsp_intf_private_data_t* data = get_private_dataspace_ptr( gas_id );

  /* Buffer indices incremented every frame */
  data->current_buffer      = 0;
  data->current_buffer_mod3 = 0;

  /* sequence number of current unprocessed command buffer */
  data->mdsp_current_seq_num = 0;

  /* Search Width the mdsp will use to search for a rx signal */
  data->mdsp_sync_rx_search_width  = MDSP_DEFAULT_RX_SEARCH_WIDTH;
  data->mdsp_async_rx_search_width = MDSP_DEFAULT_RX_SEARCH_WIDTH;

  data->mdsp_async_rx_data_wr_ptr  = 0;
  data->mdsp_async_rx_data_rd_ptr  = 0;

  data->mdsp_processing_pending_nonsbi_writes = FALSE;

#if defined(MDSP_gsmDsrSpurParamBuf)
  /* RF Spur Data Buffers for the mDSP */
  data->mdsp_rf_spur_buffer_offset[2] = {  0, MDSP_TOTAL_SPUR_WORDS };
#endif

  /* Frame Tick Setup Margin in quarter symbols */
  data->frame_tick_setup_margin = FRAME_TICK_SETUP_MARGIN_QS;

  /* Only set when all cmds written */
  data->mdsp_cmd_count    = 0;

  data->mdsp_cmd_offset = 0;
  /* Callback handler in NPL1 for mDSP halts. */
  data->mdsp_panicking = FALSE;

  /* AMPM Table is 256 Words for Polar, and 128 Words for Linear PA */
  data->mdsp_am_pm_tbl_size = MDSP_AM_PM_TABLE_ENTRIES;

#ifdef FEATURE_GSM_RX_TX_SPLIT
  data->mdsp_is_asynch_int_enabled = FALSE;
#else
#define MDSP_TX_CONFIG_NO_CMD_PENDING MDSP_TXLM_BUF_IDX_INVALID
  data->mdsp_is_asynch_int_enabled = MDSP_TX_CONFIG_NO_CMD_PENDING;
#endif
  data->mdsp_init_complete  = FALSE;

  /* SAIC enabled flag */
  data->mdsp_saic = FALSE;

#ifdef FEATURE_GSM_MDSP_ESAIC
  /* ESAIC ENABLE FLAG */
  data->mdsp_esaic = FALSE;
#endif

  /* AEQ ENABLE FLAG */
  data->mdsp_aeq = FALSE;

  /* Time offset till start of sync rx or sync tx burst */
  data->mdsp_sync_rx_time_offset = MDSP_DEFAULT_RX_TIME_OFFSET;
  data->mk_lastSeqNumRead = 0;

  data->mdsp_current_amam_buffer = 0;
  data->mdsp_current_ampm_buffer = 0;
}

/*===========================================================================

FUNCTION mdsp_update_nv_gfw_recovery

DESCRIPTION
  This function updates the gfw_recovery variable in the private dataspace
  This variable is used later on to decide whether recovery should be done

===========================================================================*/
void mdsp_update_nv_gfw_recovery(gas_id_t gas_id, boolean gl1_gfw_rec)
{
  mdsp_intf_private_data_t* data = get_private_dataspace_ptr( gas_id );

  data->mdsp_gfw_nv_recovery_restart = gl1_gfw_rec;
}

/*===========================================================================

FUNCTION get_mdsp_intf_public_dataspace_ptr

DESCRIPTION
  This function gets the public data space for a specified GAS ID

===========================================================================*/
mdsp_intf_public_data_t *get_mdsp_intf_public_dataspace_ptr(const gas_id_t gas_id)
{
  mdsp_intf_public_data_t *data_ptr;

  data_ptr =
      &(mdsp_intf_public_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_intf_private_data_t *get_private_dataspace_ptr(const gas_id_t gas_id)
{
  mdsp_intf_private_data_t *data_ptr;

  data_ptr =
      &(mdsp_intf_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION mdsp_current_buffer

DESCRIPTION
  This function gets the current buffer for a specified GAS ID

===========================================================================*/
uint8 mdsp_current_buffer(gas_id_t gas_id )
{
  uint8 result = get_private_dataspace_ptr( gas_id )->current_buffer;

  if (result > 1) /* (Always bound to 0-1 but belt and braces for static analysis) */
  {
    result = 0;
  }

  return result;
}

/*===========================================================================

FUNCTION mdsp_current_buffer_mod_3

DESCRIPTION
  This function gets the current buffer mod 3 for a specified GAS ID

===========================================================================*/
uint8 mdsp_current_buffer_mod3(gas_id_t gas_id )
{
  uint8 result = get_private_dataspace_ptr( gas_id )->current_buffer_mod3;

  if (result > 2) /* (Always bound to 0-2 but belt and braces for static analysis) */
  {
    result = 0;
  }

  return result;
}



/*===========================================================================

FUNCTION mdsp_setup_linear_pa

DESCRIPTION  This is called by the RF driver when GSM is activated. This
             sets up the Config2 initialisation to the mDSP.

===========================================================================*/
void mdsp_setup_linear_pa ( uint16 tx_cfg2_flag_val, sys_modem_as_id_e_type as_id)
{
  mdsp_intf_private_data_t* private_data_ptr;
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  private_data_ptr = get_private_dataspace_ptr( gas_id );

  /* Set to FALSE until RF driver activates. */
  private_data_ptr->mdsp_tx_pwr_ctrl_data.mdsp_linear_tx_gain_buff_updated = FALSE;

  /* The table size depends on PA Type. */
  if (tx_cfg2_flag_val == MDSP_LINEAR_PA)
  {
    /* Buffer addresses for the AM/AM tables; Linear has 128 Words. */
    private_data_ptr->mdsp_am_am_tbl_size = MDSP_AM_AM_TABLE_ENTRIES * 2;
    /* Buffer addresses for the AM/AM tables; Polar has 128 Words. */
    private_data_ptr->mdsp_am_pm_tbl_size = MDSP_AM_PM_TABLE_ENTRIES/2;
  }
  else /* POLAR Config. */
  {
    /* Buffer addresses for the AM/AM tables; Polar has 64 Words. */
    private_data_ptr->mdsp_am_am_tbl_size = MDSP_AM_AM_TABLE_ENTRIES;
    /* Buffer addresses for the AM/AM tables; Polar has 256 Words. */
    private_data_ptr->mdsp_am_pm_tbl_size = MDSP_AM_PM_TABLE_ENTRIES;
  }
}


/*===========================================================================

FUNCTION mdsp_nonsbi_write_schedulable

DESCRIPTION
   Returns if another nonsbi write may be scheduled this frame.  Checks
   that we are not too close to the frame boundary and that the current
   buffer is not full.

===========================================================================*/
/*static*/ boolean mdsp_nonsbi_write_schedulable (gas_id_t gas_id)
{
   uint16 qty;
   uint32 sym;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* If we are asleep, then can't write */
   if(!mdsp_awake( gas_id ))
   {
       MSG_GERAN_ERROR_1_G(GL1_SLEEP_HDR" mDSP NOT AWAKE fn=%d",GSTMR_GET_FN_GERAN(gas_id));
       return FALSE;
   }

   /* If we already have pending transactions and we are not currently
      processing them then we have to wait to maintain correct order */
   if (private_data_ptr->mdsp_pending_nonsbi_writes > 0 && !private_data_ptr->mdsp_processing_pending_nonsbi_writes)
   {
      return FALSE;
   }

   /* Read the current gstmr symbol count.  We do this first to try and
      avoid the race condition where the count has rolled over but the
      IRQ_STATUS register has not yet been updated - this can happen! */
   sym = gstmr_rd_symbol_count(gas_id);

   /* Check how many transactions are already scheduled this frame */
     qty = public_data_ptr->pMdspHostIfBaseStore->gfwNonSbiWriteBuffers[private_data_ptr->current_buffer].numberTransactions & ~MDSP_SEMAPHORE_BIT;

   /* Check if we can issue the nonsbi write, we cannot do so if:
      1. We are close enough to the frame tick such that it may
         fire before we are done writing the buffer and command.
      2. The GSM interrupt is already pending, which could happen if
         mdsp_nonsbi_write was called with interrupts locked.
      3. We are within the delayed window, which means that the mDSP has
         gotten the GSTMR, but the ARM has not.  This is a dangerous time
         since we are essentially out of sync.
      4. The current nonsbi transaction buffer is full.
      If any of these conditions are true we need to store the nonsbi write
      for the next frame */
   if (sym >= MDSP_MAX_SYMBOLS_FOR_NONSBI_WRITE ||
       gstmr_is_int_pending(gas_id) ||
       /* Force the Write to the start of the frame. */
       (gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)) > 1500) ||
       qty >= MDSP_MAX_NONSBI_REQUEST_TRANSACTIONS)
   {
      return FALSE;
   }
   return TRUE;
}



/*===========================================================================

FUNCTION mdsp_write_nonsbi

DESCRIPTION
  This function tells the mDSP to write data to a nonsbi register. Since the
  mdsp can take up to MAX_NONSBI_TRANSACTIONS as part of one command, this
  function will add to the nonsbi buffer.  It will issue the command to the
  mdsp if this is the first transaction in the nonsbi_buffer.

  The command will be delayed into the next tdma frame if we are too close
  to the end of the current frame or if MAX_NONSBI_TRANSACTIONS is reached.

  reg  - abstract register address to write to
  data - data to write - 32 bits

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void mdsp_nonsbi_write(uint32 reg, uint32 data , gas_id_t gas_id )
{
   uint16 qty;
   GfwCmdBufType cmd;
   GfwGrfcBuffer *pGrfcBuffer;


   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   if( !rex_is_in_irq_mode())
   {
     /* disabling the preemption here */
     REX_DISABLE_PREMPTION( );
     /* Lock interrupts since this may be called from task context */
     GDRV_ISR_SAVE_LOCK(gas_id);

     /* Check if we are actually able to schedule the nonsbi write this frame */
     if (!mdsp_nonsbi_write_schedulable( gas_id ))
     {
        if (private_data_ptr->mdsp_pending_nonsbi_writes >= MAX_PENDING_NONSBI_WRITES)
        {
            MSG_GERAN_ERROR_1_G("Pending NonSBI buffer full, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
        }
        else
        {
           private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr].reg = reg;
           private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr].data = data;

           /* Increment the write pointer count */
           if (++private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr >= MAX_PENDING_NONSBI_WRITES)
           {
              private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr = 0;
           }
           private_data_ptr->mdsp_pending_nonsbi_writes++;
        }
     }
     else
     {
        /* Prepare the transaction */
        /* Get the offset and currently scheduled transactions */
        pGrfcBuffer = (GfwGrfcBuffer*)&(public_data_ptr->pMdspHostIfBaseStore->gfwNonSbiWriteBuffers[private_data_ptr->current_buffer]);
        qty = pGrfcBuffer->numberTransactions & ~MDSP_SEMAPHORE_BIT;
        pGrfcBuffer->grfc[qty].data_h = data >> 16;
        pGrfcBuffer->grfc[qty].data_l = (data & 0x0000ffff);
        pGrfcBuffer->grfc[qty].reg = reg;

        qty++;

        pGrfcBuffer->numberTransactions = qty | MDSP_SEMAPHORE_BIT;

        /* If this was the first write into the buffer, send the command */
        if (qty == 1)
        {
           memset(&cmd,0,sizeof(GfwCmdBufType));
           cmd.nonSbiRegisterWriteCmd.cmd = GFW_NON_SBI_REGISTER_WRITE_CMD;
           cmd.nonSbiRegisterWriteCmd.grfcBuffIndex = private_data_ptr->current_buffer;

           /* issue the command to Q6*/
           mdsp_issue_qdsp6_command (&cmd, GFW_NON_SBI_REGISTER_WRITE_CMD , gas_id );
        }
        /* Log the write */
        mdsp_log_grfc_write(reg, data , gas_id );
     }

     GDRV_ISR_SAVE_UNLOCK(gas_id);

     /* Enable preemption here */
     REX_ENABLE_PREMPTION( );
   }
   else
   {
     /* Lock interrupts since this may be called from task context */
     GDRV_ISR_SAVE_LOCK(gas_id);

     /* Check if we are actually able to schedule the nonsbi write this frame */
     if (!mdsp_nonsbi_write_schedulable( gas_id ))
     {
        if (private_data_ptr->mdsp_pending_nonsbi_writes >= MAX_PENDING_NONSBI_WRITES)
        {
            MSG_GERAN_ERROR_1_G("Pending NonSBI buffer full, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
        }
        else
        {
           private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr].reg = reg;
           private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr].data = data;

           /* Increment the write pointer count */
           if (++private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr >= MAX_PENDING_NONSBI_WRITES)
           {
              private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr = 0;
           }
           private_data_ptr->mdsp_pending_nonsbi_writes++;
        }
     }
     else
     {
        /* Prepare the transaction */
        /* Get the offset and currently scheduled transactions */
        pGrfcBuffer = (GfwGrfcBuffer*)&(public_data_ptr->pMdspHostIfBaseStore->gfwNonSbiWriteBuffers[private_data_ptr->current_buffer]);
        qty = pGrfcBuffer->numberTransactions & ~MDSP_SEMAPHORE_BIT;
        pGrfcBuffer->grfc[qty].data_h = data >> 16;
        pGrfcBuffer->grfc[qty].data_l = (data & 0x0000ffff);
        pGrfcBuffer->grfc[qty].reg = reg;

        qty++;

        pGrfcBuffer->numberTransactions = qty | MDSP_SEMAPHORE_BIT;

        /* If this was the first write into the buffer, send the command */
        if (qty == 1)
        {
           memset(&cmd,0,sizeof(GfwCmdBufType));
           cmd.nonSbiRegisterWriteCmd.cmd = GFW_NON_SBI_REGISTER_WRITE_CMD;
           cmd.nonSbiRegisterWriteCmd.grfcBuffIndex = private_data_ptr->current_buffer;
           /* issue the command to Q6*/
           mdsp_issue_qdsp6_command (&cmd, GFW_NON_SBI_REGISTER_WRITE_CMD , gas_id );
        }

        /* Log the write */
        mdsp_log_grfc_write(reg, data , gas_id );
     }

     GDRV_ISR_SAVE_UNLOCK(gas_id);
   }
}



/*===========================================================================

FUNCTION mdsp_nonsbi_writes_pending

DESCRIPTION
   Checks if any nonsbi writes are pending for the next frame tick.

===========================================================================*/
boolean mdsp_nonsbi_writes_pending (gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_pending_nonsbi_writes > 0 ? TRUE : FALSE;
}



/*===========================================================================

FUNCTION mdsp_process_nonsbi_transactions

DESCRIPTION
   Called at the frame tick to issue any pending nonsbi transactions.

===========================================================================*/
/*static*/ void mdsp_process_nonsbi_transactions (gas_id_t gas_id)
{
   uint32 reg;
   uint32 data;
   uint16 cnt = 0;

   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Set flag informing scheduler we are processing the pending transactions */
   private_data_ptr->mdsp_processing_pending_nonsbi_writes = TRUE;

   /* Issue any pending schedulable nonsbi transactions */
   while (private_data_ptr->mdsp_pending_nonsbi_writes > 0 && mdsp_nonsbi_write_schedulable( gas_id ))
   {
      /* Send the nonsbi transaction */
      reg  = private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr].reg;
      data = private_data_ptr->mdsp_pending_nonsbi_write_buffer[private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr].data;
      mdsp_nonsbi_write(reg, data , gas_id );

      /* Increment the read pointer and decrement the count */
      if (++private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr >= MAX_PENDING_NONSBI_WRITES)
      {
         private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr = 0;
      }
      private_data_ptr->mdsp_pending_nonsbi_writes--;
      cnt++;
   }

   /* Clear flag */
   private_data_ptr->mdsp_processing_pending_nonsbi_writes = FALSE;

   /* Print a debug message if we issued any transactions */
   if (cnt > 0 || private_data_ptr->mdsp_pending_nonsbi_writes > 0)
   {
      MSG_GERAN_MED_3_G("Issued %d of %d pending NonSBI transactions, FN=%d",
              cnt, cnt + private_data_ptr->mdsp_pending_nonsbi_writes, GSTMR_GET_FN_GERAN(gas_id));
   }
}



/****************************************************************************
*****************************************************************************

                   GRFC FUNCTIONS

*****************************************************************************
*****************************************************************************/

/*===========================================================================

FUNCTION mdsp_grfc_tristate

DESCRIPTION
  This function is used set the direction (output/input) for the bits in the
  GRFC_DATA registers.  The direction register is MDSP_REG_grfc_dir_ctl.

  mask  - bit mask to be tri-stated.
  data - data to write

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_grfc_tristate
(
  uint32 mask,
  uint32 data
)
{
} /* end of mdsp_grfc_tristate() */




/*===========================================================================

FUNCTION mdsp_get_grfc_tristate

DESCRIPTION
  This function returns the current shadow register of the grfc tristate
  buffer.

DEPENDENCIES
  None

RETURN VALUE
  grfc tristate buffer shadow register.

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_get_grfc_tristate( void )
{
   return 0xFFFFFFFF;
}



/*===========================================================================

FUNCTION mdsp_grfc_out

DESCRIPTION
  This function is used to write to the GRFC_DATA register.

  mask  - bit mask to be written to
  data - data to write

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_grfc_out
(
  uint32 mask,
  uint32 data
)
{

} /* end of mdsp_grfc_out() */



/*===========================================================================

FUNCTION mdsp_get_grfc_out

DESCRIPTION
  This function returns the current shadow register of the grfc data
  buffer.

DEPENDENCIES
  None

RETURN VALUE
  grfc data buffer shadow register.

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_get_grfc_out( void )
{
   return 0xFFFFFFFF;
}


/*===========================================================================

FUNCTION mdsp_grfc_polarity

DESCRIPTION
  This function is used to write to the GRFC_POLARITY_CTL register.

  mask  - bit mask to be written to
  data - data to write

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_grfc_polarity
(
  uint32 mask,
  uint32 data
)
{
} /* end of mdsp_grfc_polarity() */


/*===========================================================================

FUNCTION mdsp_get_grfc_polarity

DESCRIPTION
  This function returns the current shadow register of the grfc polarity
  buffer.

DEPENDENCIES
  None

RETURN VALUE
  grfc polarity buffer shadow register.

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_get_grfc_polarity( void )
{
   return 0xFFFFFFFF;
}




/*===========================================================================

FUNCTION mdsp_grfc_omode

DESCRIPTION
  This function is used to write to the GRFC_OMODE_CTL register.

  mask  - bit mask to be written to
  data - data to write

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_grfc_omode
(
  uint32 mask,
  uint32 data
)
{
}



/*===========================================================================

FUNCTION mdsp_get_grfc_omode

DESCRIPTION
  This function returns the current shadow register of the grfc omode
  buffer.

DEPENDENCIES
  None

RETURN VALUE
  grfc omode buffer shadow register.

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_get_grfc_omode (void)
{
   return 0xFFFFFFFF;
}


/*===========================================================================

FUNCTION mdsp_grfc_in

DESCRIPTION
  This function retrieves the value of one or more GRFCs.

  mask - bitwise mask with bit 0 representing GRFC 0,
        a '1' in any bit position will read that GRFC as an input value.

DEPENDENCIES
  None

RETURN VALUE
  GRFC register with mask applied.

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_grfc_in (uint32 mask, uint16 waitms)
{
   return 0xFFFFFFFF;
}



/****************************************************************************
*****************************************************************************

                   ACQUISITION FUNCTIONS

*****************************************************************************
*****************************************************************************/


/*===========================================================================

FUNCTION mdsp_start_acquisition

DESCRIPTION
  This function tells the mDSP to start FCCH acquisition.

  offset - pointer to where to start the acquisition window
  decode - indicates if SCH decode should also be performed
  afc    - what type of acquisition afc to perform
  phase_increment - the phase increment (rotator value) to start with

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_start_acquisition(
  uint16              offset,
  gl1_hw_sch_decode_t sch_decode,
  mdsp_acq_afc_type   afc,
  boolean             part_tone_det,
  int32               phase_increment,
#ifdef FEATURE_GSM_COEX
  uint32              cxm_priority,
  uint32              cxm_desense_id,
  boolean             enhanced_fcch_detection,
#endif
  ARFCN_T             arfcn,
  boolean             disable_power_opt15,
  uint32              rf_seq_num,
#ifdef FEATURE_GSM_RX_DIVERSITY
   boolean            enable_RxDiversity,
#endif
gas_id_t gas_id
)
{
   GfwCmdBufType    cmd;
#ifdef FEATURE_GSM_RX_DIVERSITY 
   gl1_rxd_control_type rxd_ctl_flags;
#endif
   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.startAcqCmd.cmdType                   = GFW_START_ACQ_CMD;
   cmd.startAcqCmd.timeOffset                = mdsp_ftsm( gas_id ) + offset;
   cmd.startAcqCmd.rfBufIndex                = private_data_ptr->current_buffer_mod3;
   /* preserve current_buffer_mod3 for start_acq as the generated CCS buffer will also incldue the stop buffer, so FW
   will need to receive the index to the same buffer when sending the stop_acq */
   private_data_ptr->mdsp_stop_acq_buffer_mod3                 = private_data_ptr->current_buffer_mod3;
   cmd.startAcqCmd.startIndexMsw             = 0;
   cmd.startAcqCmd.startIndexLsw             = 0;

   /* Set for all modes except legacy FCCH */
   cmd.startAcqCmd.schDecodeFlag             = ( sch_decode != GL1_NO_SCH_DECODE );

   if(afc == MDSP_ACQ_AFC_ON_PDM)
   {
     cmd.startAcqCmd.fcchAfcFlag.rotatorFlag = 0;
     cmd.startAcqCmd.fcchAfcFlag.fcchAfcFlag = 1;
   }
   else if(afc == MDSP_ACQ_AFC_ON_ROTATOR)
   {
     cmd.startAcqCmd.fcchAfcFlag.rotatorFlag= 1;
     cmd.startAcqCmd.fcchAfcFlag.fcchAfcFlag= 1;
   }
   else
   {
     cmd.startAcqCmd.fcchAfcFlag.rotatorFlag= 0;
     cmd.startAcqCmd.fcchAfcFlag.fcchAfcFlag= 0;
   }
   cmd.startAcqCmd.fcchAfcFlag.xoFlag = 1;

   cmd.startAcqCmd.phaseIncrement            = phase_increment;

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
   cmd.startAcqCmd.enableAeqOnSch = public_data_ptr->mdsp_aeq_sch_enable;
#else
   cmd.startAcqCmd.enableAeqOnSch = FALSE;
#endif
   cmd.startAcqCmd.logIq =  (public_data_ptr->gsm_Iq_Log.acq_log_iq) |(iq_logging) ;

 /* current_buffer does not toggle every frame. It toggles only if there are any commands
  * to FW in that frame. So using static ping pong buffer index 'iq_buf_index'. This is
  * toggled every frame if the IQ logging is enabled. */

#ifdef FEATURE_GSM_COEX_FW_CXM
   cmd.startAcqCmd.cxmPriority  = cxm_priority;
   cmd.startAcqCmd.cxmDesenseId = cxm_desense_id;
#ifdef FEATURE_GSM_COEX_ENHANCED_FCCH
   cmd.startAcqCmd.doEnhancedFCCH = enhanced_fcch_detection;
   MSG_GERAN_LOW_3_G("CXM : ACQ coex_priority=%d coex_desense_id=%d enhanced_fcch=%d",
      cmd.startAcqCmd.cxmPriority, cmd.startAcqCmd.cxmDesenseId, cmd.startAcqCmd.doEnhancedFCCH);
#else
   cmd.startAcqCmd.doEnhancedFCCH = FALSE;
   MSG_GERAN_HIGH_2_G("CXM : ACQ coex_priority=%d coex_desense_id=%d", cmd.startAcqCmd.cxmPriority, cmd.startAcqCmd.cxmDesenseId);
#endif
#else
   cmd.startAcqCmd.cxmPriority  = 0;
   cmd.startAcqCmd.cxmDesenseId = 0;
   cmd.startAcqCmd.doEnhancedFCCH = FALSE;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
   cmd.startAcqCmd.doJdetReading = FALSE;
   if(gl1_hw_get_sawless_HLLL_support(arfcn.band,gas_id))
   {
      cmd.startAcqCmd.doJdetReading = TRUE;
   }
#endif /*GERAN_L1_HLLL_LNASTATE*/

   /* Parallel SCH enabled/disabled to firmware */
   cmd.startAcqCmd.bDoParallelSch = ( sch_decode == GL1_PARALLEL_SCH_DECODE ||
                                      sch_decode == X2G_PARALLEL_SCH_DECODE ||
                                      sch_decode == T2G_PARALLEL_SCH_DECODE );

   cmd.startAcqCmd.iqBufferIndex = iq_buf_index;

   cmd.startAcqCmd.enablePartToneDet = part_tone_det;

#ifdef FEATURE_GSM_COEX
   cmd.startAcqCmd.arfcn = arfcn.num;
#endif   

   cmd.startAcqCmd.rfSeqNr = rf_seq_num;
   cmd.startAcqCmd.bDisablePowerOpt15 = disable_power_opt15;


#ifdef FEATURE_GSM_RX_DIVERSITY 
   if(enable_RxDiversity)
   {
     rxd_ctl_flags                             = gl1_get_rxd_control_flags(gas_id);
     cmd.startAcqCmd.rxdControl.enableRxd      = (rxd_ctl_flags.enableRxd == 1)?1:0;	 
     cmd.startAcqCmd.rxdControl.enablePrxOnly  = (rxd_ctl_flags.enablePrx == 1)?1:0;
     cmd.startAcqCmd.rxdControl.enableDrxOnly  = (rxd_ctl_flags.enableDrx == 1)?1:0;
   }
   else
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   {
     cmd.startAcqCmd.rxdControl.enableRxd      = 0;	 
     cmd.startAcqCmd.rxdControl.enablePrxOnly  = 0;
     cmd.startAcqCmd.rxdControl.enableDrxOnly  = 0;
   }
   /* issue Command to QDSP6 FW */
   mdsp_issue_qdsp6_command( &cmd, cmd.startAcqCmd.cmdType, gas_id );
}

/*===========================================================================

FUNCTION mdsp_stop_acquisition

DESCRIPTION
  This function tells the mDSP to stop FCCH acquisition.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_stop_acquisition( uint16 offset , gas_id_t gas_id )
{
    GfwCmdBufType cmd;

    mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

    memset(&cmd,0,sizeof(GfwCmdBufType));
    cmd.stopAcqCmd.cmdType     = GFW_STOP_ACQ_CMD;

    cmd.stopAcqCmd.timeOffset  = mdsp_ftsm( gas_id ) + offset;
    cmd.stopAcqCmd.rfBufIndex      = private_data_ptr->mdsp_stop_acq_buffer_mod3;
    /* Issue the command to Q6 FW */
    mdsp_issue_qdsp6_command(&cmd,GFW_STOP_ACQ_CMD , gas_id );
}



/*===========================================================================

FUNCTION mdsp_get_FCCH_results

DESCRIPTION
  This function retrieves an FCCH data packet from the mDSP if available. The
  mDSP may hold up to MAX_FCCH_RES packets so this routine may be invoked
  again till no packets are available.

  data - pointer to store fcch results


DEPENDENCIES
  MDSP_mdspFcchWriteCounter, MDSP_hostFcchReadCounter must be reset by mDSP
  on start of acquisition command

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_get_FCCH_results ( GfwFcchResultStruct *data , gas_id_t gas_id )
{
uint16 num_unread;
boolean results_avail;

    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

    /* Get number of unread buffers          */
    /* This should always be <= MAX_FCCH_RES */
    /* The wr_counter should never reach 0xffff, L1 should timeout and switch chan */
    num_unread = public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.mdspFcchResultWrCtr - public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.mdspFcchResultRdCtr;

    if (num_unread != 0)
    {
      *data = *(&public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.fcchResults[public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.mdspFcchResultRdCtr % MAX_FCCH_RES]);
      public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.mdspFcchResultRdCtr++;

    gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, data->scheduleTimeError, gas_id);
 /* Needs implementing for Q6 */

        results_avail = TRUE;
    }
    else
    {
        results_avail = FALSE;
    }

    return results_avail;
}


boolean mdsp_x2g_get_FCCH_results ( GfwFcchResultStruct *data, uint16  seq_num, gas_id_t gas_id )
{
    uint16   i = 0;
    boolean  results_avail = FALSE;

    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
    mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

    /* Get number of unread buffers          */
    if (seq_num == private_data_ptr->mk_lastSeqNumRead)
    {
      MSG_GERAN_ERROR_1_G(" FCCH result seqNum check error: %d ",seq_num);
    }

    /* Use the seq_num to decide which buffer to pick up the results from and when the results are ready */
    while( i < MAX_FCCH_RES )
    {
      if( seq_num == public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.fcchResults[ i ].seqNum )
      {
        *data = *(&public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.fcchResults[ i ]);

        gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, data->scheduleTimeError, gas_id);

        results_avail     = TRUE;
        private_data_ptr->mk_lastSeqNumRead = public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.fcchResults[ i ].seqNum;
        public_data_ptr->pMdspHostResultsStore->gfwFcchResultBuffer.fcchResults[ i ].seqNum = 0;
        /* Mission accomplished, get out ! */
        break;
      }

      i++;
    } /* while( i < MAX_FCCH_RES ) */


    return results_avail;
}

/*===========================================================================

FUNCTION mdsp_reset_sch_results_ptr

DESCRIPTION
  Called at the start of acquisition, resets the SCH read and write pointers to be equal

===========================================================================*/
void mdsp_reset_sch_results_ptr( gas_id_t gas_id )
{
   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   /* If any unread buffers then reset the pointers */
   if (public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultWrCtr != public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr)
   {
      MSG_GERAN_HIGH_2_G( "Stale SCH results RdPtr %d, WrPtr %d",
                               public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr,
                               public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultWrCtr);

      public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr = 0;
      public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultWrCtr = 0;
   }
}

/*===========================================================================

FUNCTION mdsp_get_SCH_results

DESCRIPTION
  This function retrieves an SCH data packet from the mDSP if available. The
  mDSP may hold up to MAX_SCH_RES packets so this routine may be invoked
  again till no packets are available.

  data - pointer to store sch results


DEPENDENCIES
  MDSP_mdspSchWriteCounter, MDSP_hostSchReadCounter must be reset by mDSP
  on start of acquisition command

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_get_SCH_results( GfwSchResultStruct *sch_data_p, gas_id_t gas_id )
{
   uint16 num_unread;
   boolean results_avail;

   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   /* Get number of unread buffers         */
   /* This should always be <= MAX_SCH_RES */
   /* The wr_counter should never reach 0xffff, L1 should timeout and switch chan */
   num_unread = public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultWrCtr - public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr;

    if (num_unread > 0)
    {
        *sch_data_p = *(&public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.GfwSchResults[public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr % MAX_SCH_RES]);
        public_data_ptr->pMdspHostResultsStore->gfwSchResultBuffer.mdspSchResultRdCtr++;
        
        gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, sch_data_p->scheduleTimeError, gas_id);
 
        /* Needs to be implemented for Q6 */

        results_avail = TRUE;
    }
    else
    {
        results_avail = FALSE;
    }

    return results_avail;
}

/*===========================================================================

FUNCTION mdsp_get_Parallel_SCH_log_data

DESCRIPTION
  This function retrieves an Parallel SCH log data packet from
  the mDSP if available.

  par_sch_log_data_p - pointer to store parallel sch log results

DEPENDENCIES

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_get_Parallel_SCH_log_data(
          GfwPllelSchLogPckt *par_sch_log_data_p,
          gas_id_t            gas_id )
{
  boolean results_avail = FALSE;

  mdsp_intf_public_data_t *public_data_ptr =
    get_mdsp_intf_public_dataspace_ptr( gas_id );

  if ( par_sch_log_data_p )
  {
    *par_sch_log_data_p =
      *(&public_data_ptr->pMdspHostResultsStore->gfwParallelSchLogBuffer);

    results_avail = TRUE;
  }

  return results_avail;
}

/****************************************************************************
*****************************************************************************

                   TRANSMIT FUNCTIONS

*****************************************************************************
*****************************************************************************/

/*===========================================================================

FUNCTION mdsp_set_tx_burst_offset

DESCRIPTION
  These functions set and set the offset of the Tx burst from its true
  slot 3 offset.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_tx_burst_offset
(
  uint16 tx_burst_offset , gas_id_t gas_id
)
{
  get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_tx_burst_offset = FRAME_TICK_SETUP_MARGIN_QS
                         + (QS_PER_SLOT * 3)
                         - tx_burst_offset;
}

/*===========================================================================

FUNCTION mdsp_ftm_set_tx_burst_offset

DESCRIPTION
  These functions set and set the offset of the Tx burst from its true
  slot 3 offset.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_set_tx_burst_offset
(
  uint16 tx_burst_offset,
  sys_modem_as_id_e_type as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  mdsp_set_tx_burst_offset(tx_burst_offset, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Also Write Tx burst offset to 2nd Sub for DSDS mode as RF driver only has the
   * concept of one RF device
   */
  if(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY)
  {
    mdsp_set_tx_burst_offset(tx_burst_offset, GERAN_ACCESS_STRATUM_ID_2 );
  }

#ifdef FEATURE_TRIPLE_SIM
  /* Also Write Tx burst offset to 2nd/3rd Sub for TSTS mode as RF driver only has the
   * concept of one RF device
   */
  else if ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY )
  {
    mdsp_set_tx_burst_offset( tx_burst_offset, GERAN_ACCESS_STRATUM_ID_2 );

    mdsp_set_tx_burst_offset( tx_burst_offset, GERAN_ACCESS_STRATUM_ID_3 );
  }
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

/*===========================================================================

FUNCTION mdsp_get_tx_burst_offset

DESCRIPTION
  Gets the tx_burst_offset which RF has initially passed to L1 using
  mdsp_ftm_set_tx_burst_offset

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Will ERR_FATAL in case the value is greater than MDSP_MAX_TX_BURST_OFFSET value 
==============================================================================*/
uint16 mdsp_get_tx_burst_offset (gas_id_t gas_id )
{
  uint16  get_tx_offset;
    uint16  mdsp_burst_offset;

  mdsp_burst_offset = get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_tx_burst_offset;
	

  /*mdsp_tx_burst_offset should not be greater than 2500qs*/
  if (mdsp_burst_offset <= MDSP_MAX_TX_BURST_OFFSET)
  {
        get_tx_offset = FRAME_TICK_SETUP_MARGIN_QS + (QS_PER_SLOT * 3) - mdsp_burst_offset;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("ERR_FATAL: Invalid RF mdsp_tx_burst_offset:%d",mdsp_burst_offset);
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("ERR_FATAL: Invalid RF mdsp_tx_burst_offset:%d",mdsp_burst_offset,0,0);
  }

  return get_tx_offset;
}


#ifdef FEATURE_POLAR
/*===========================================================================

FUNCTION mdsp_cfg_pa_profile_polar

DESCRIPTION
  This function sets the static parameters of the PA profile for the POLAR
  RF.

===========================================================================*/
void mdsp_cfg_pa_profile_polar
(
   mdsp_pa_ramp_type *ramp_tbl
  , gas_id_t gas_id
)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Save the table pointers */
   private_data_ptr->mdsp_tx_pwr_ctrl_data.pa_ramp_table_polar = ramp_tbl;

   /* Flag them for pickup on the next transmit */
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_pa_ramp_table_polar = TRUE;
}

/*===========================================================================

FUNCTION mdsp_ftm_cfg_pa_profile_polar

DESCRIPTION
  This function sets the static parameters of the PA profile for the POLAR
  RF.

===========================================================================*/
void mdsp_ftm_cfg_pa_profile_polar
(
   mdsp_pa_ramp_type *ramp_tbl,
   sys_modem_as_id_e_type as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_cfg_pa_profile_polar(ramp_tbl, gas_id );
}
/*===========================================================================

FUNCTION mdsp_cfg_amam_ampm_tables

DESCRIPTION
  This functions configures the AM/AM and AM/PM tables for uplink
  power control.  It should be called once to set the pointer to the
  table location.

===========================================================================*/
void mdsp_cfg_amam_ampm_tables
(
   mdsp_amam_ampm_tables_type  *amam_ampm_tables , gas_id_t gas_id
)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Save the table pointers */
   private_data_ptr->mdsp_tx_pwr_ctrl_data.amam_ampm_tables = amam_ampm_tables;

   /* Pick up the new information */
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_am_table = TRUE;
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_pm_table = TRUE;
}

/*===========================================================================

FUNCTION mdsp_ftm_cfg_amam_ampm_tables

DESCRIPTION
  This functions configures the AM/AM and AM/PM tables for uplink
  power control.  It should be called once to set the pointer to the
  table location.

===========================================================================*/
void mdsp_ftm_cfg_amam_ampm_tables
(
   mdsp_amam_ampm_tables_type  *amam_ampm_tables,
   sys_modem_as_id_e_type      as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_cfg_amam_ampm_tables(amam_ampm_tables, gas_id );
}
/*===========================================================================

FUNCTION mdsp_update_amam_ampm_tables

DESCRIPTION
  This functions flags the AM/AM and AM/PM tables as having changed.

===========================================================================*/
void mdsp_update_amam_ampm_tables
(
   boolean  new_am_am_table,
   boolean  new_am_pm_table
   , gas_id_t gas_id
)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Be careful not to mark already stale data as valid */
   if (!private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_am_table)
   {
      private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_am_table = new_am_am_table;
   }
   if (!private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_pm_table)
   {
      private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_pm_table = new_am_pm_table;
   }
}

/*===========================================================================

FUNCTION mdsp_ftm_update_amam_ampm_tables

DESCRIPTION
  This functions flags the AM/AM and AM/PM tables as having changed.

===========================================================================*/
void mdsp_ftm_update_amam_ampm_tables
(
   boolean  new_am_am_table,
   boolean  new_am_pm_table,
   sys_modem_as_id_e_type as_id
)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_update_amam_ampm_tables(new_am_am_table,new_am_pm_table, gas_id );
}
#else

/*===========================================================================

FUNCTION mdsp_cfg_pa_profile

DESCRIPTION
   This function configures the linear PA table.

===========================================================================*/
void mdsp_cfg_pa_profile
(
  mdsp_pa_ramp_type *ramp_tbl
)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Save the pointer to the PA table */
   private_data_ptr->mdsp_tx_pwr_ctrl_data.pa_ramp_table_linear = ramp_tbl;
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_pa_ramp_table_linear = TRUE;
}

#endif



/*===========================================================================

FUNCTION mdsp_set_bsic

DESCRIPTION
  This function is used to set the BSIC that the mdsp driver will use for
  subsequent RACH transmit bursts.

  bsic - Base Station ID Code

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_bsic ( uint8 bsic , gas_id_t gas_id )
{
    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
    public_data_ptr->mdsp_BSIC = bsic;
}

/*===========================================================================

FUNCTION mdsp_enable_tx_rotator

DESCRIPTION
  This function enables or disables the transmit rotator.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_enable_tx_rotator (boolean enable , gas_id_t gas_id )
{
    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

    public_data_ptr->mdsp_tx_rotator = enable;
}

/****************************************************************************
*****************************************************************************

                   RECEIVE FUNCTIONS

*****************************************************************************
*****************************************************************************/

/*===========================================================================

FUNCTION mdsp_set_sync_rx_time_offset

DESCRIPTION
  This function sets the sync rx time offset.

===========================================================================*/
void mdsp_set_sync_rx_time_offset (uint16 offset , gas_id_t gas_id )
{
   get_private_dataspace_ptr( gas_id )->mdsp_sync_rx_time_offset = offset;
}

/*===========================================================================

FUNCTION mdsp_get_sync_rx_time_offset

DESCRIPTION
  This function gets the sync rx time offset.

===========================================================================*/
uint16 mdsp_get_sync_rx_time_offset (gas_id_t gas_id)
{
   return get_private_dataspace_ptr( gas_id )->mdsp_sync_rx_time_offset;
}

/*===========================================================================

FUNCTION mdsp_set_sync_rx_search_width, mdsp_set_async_rx_search_width

DESCRIPTION
  This function sets the search width to be used for subsequent sync_rx/async_rx
  commands.  The search width tells the mdsp to program the RF such that the
  samples on either side of the expected location are received, over which the
  DSP can search for the actual location of the receive burst.

  width - Symbol resolution is used. Must be an even number.

===========================================================================*/
void mdsp_set_sync_rx_search_width (uint16 width , gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   if (width % 2 == 1)
   {
      width++;
   }
   if (width > 98)
   {
      width = 98;
   }

   private_data_ptr->mdsp_sync_rx_search_width = width;   /* mdsp expects in symbols not QS */
}

uint16 mdsp_get_sync_rx_search_width (gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   return private_data_ptr->mdsp_sync_rx_search_width;
}


void mdsp_set_async_rx_search_width (uint16 width , gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   if (width % 2 == 1)
   {
      width--;
   }
   if (width > 98)
   {
      width = 98;
   }

   if (width != private_data_ptr->mdsp_async_rx_search_width)
   {
     MSG_GERAN_MED_2_G("Async rx Search width changed from %d to %d symbols",
                        private_data_ptr->mdsp_async_rx_search_width,width);  
   }

   private_data_ptr->mdsp_async_rx_search_width = width;   /* mdsp expects in symbols not QS */
}

uint16 mdsp_get_async_rx_search_width (gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   return private_data_ptr->mdsp_async_rx_search_width;
}



/*===========================================================================

FUNCTION mdsp_enable_saic

DESCRIPTION
  This function enables or disabled SAIC

===========================================================================*/
void mdsp_enable_saic(boolean enable , gas_id_t gas_id )
{
   get_private_dataspace_ptr( gas_id )->mdsp_saic = enable;
}

boolean mdsp_get_enable_saic(gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_saic;
}


#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION mdsp_enable_esaic

DESCRIPTION
  This function enables or disabled ESAIC

===========================================================================*/
void mdsp_enable_esaic (boolean enable , gas_id_t gas_id )
{
   get_private_dataspace_ptr( gas_id )->mdsp_esaic = enable;
}

boolean mdsp_get_enable_esaic (gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_esaic;
}

#endif /* FEATURE_GSM_MDSP_ESAIC */

/*===========================================================================

FUNCTION mdsp_enable_aeq

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
void mdsp_enable_aeq (boolean enable , gas_id_t gas_id )
{
   get_private_dataspace_ptr( gas_id )->mdsp_aeq = enable;
}

/*===========================================================================

FUNCTION mdsp_get_enable_aeq

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
boolean mdsp_get_enable_aeq(gas_id_t gas_id )
{
   return get_private_dataspace_ptr( gas_id )->mdsp_aeq;
}



#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
/*===========================================================================

FUNCTION mdsp_enable_aeq

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
void mdsp_enable_aeq_8psk (boolean enable , gas_id_t gas_id )
{
  get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_aeq_8psk = enable;
}

/*===========================================================================

FUNCTION mdsp_get_aeq_8psk_status

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
boolean mdsp_get_aeq_8psk_status (gas_id_t gas_id )
{
   return get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_aeq_8psk;
}

#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
/*===========================================================================

FUNCTION mdsp_enable_aeq

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
void mdsp_enable_aeq_8psk_dynamic (boolean enable , gas_id_t gas_id )
{
   get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_aeq_8psk_dynamic = enable;
}

/*===========================================================================

FUNCTION mdsp_get_aeq_8psk_status

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
boolean mdsp_get_aeq_8psk_dynamic_status (gas_id_t gas_id )
{
   return get_mdsp_intf_public_dataspace_ptr( gas_id )->mdsp_aeq_8psk_dynamic;
}

#endif  /* FEATURE_GSM_DYNAMIC_AEQ_8PSK */
#endif  /* FEATURE_GSM_MDSP_AEQ_8PSK */

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
/*===========================================================================

FUNCTION mdsp_enable_aeq

DESCRIPTION
  This function enables or disables AEQ

===========================================================================*/
void mdsp_enable_aeq_sch (boolean enable , gas_id_t gas_id )
{
  mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   /* Write to shared variable */
  public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.aeqEnableSchFlag = enable;
  mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id );

  public_data_ptr->mdsp_aeq_sch_enable = enable;

  MSG_GERAN_HIGH_1_G("AEQ SCH flag %d",enable);
}

#endif  /* FEATURE_GSM_MDSP_AEQ_SCH */



#ifdef FEATURE_GSM_AMR
/*===========================================================================

FUNCTION mdsp_dl_codec_set_configure_cmd

DESCRIPTION
  This function informs mDSP the four different Active Codec Set values to
  be used.  It is used for AMR only.

===========================================================================*/
void mdsp_dl_codec_set_configure_cmd(mdsp_amr_codec_mode_type *codec_mode , gas_id_t gas_id )
{
   GfwCmdBufType      cmd;

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.amrCodecCmd.cmd = GFW_CODEC_SET_CONFIGURE_CMD;
   cmd.amrCodecCmd.reserved = 0;
   cmd.amrCodecCmd.amrCm[0] = (uint16)codec_mode[0];
   cmd.amrCodecCmd.amrCm[1] = (uint16)codec_mode[1];
   cmd.amrCodecCmd.amrCm[2] = (uint16)codec_mode[2];
   cmd.amrCodecCmd.amrCm[3] = (uint16)codec_mode[3];

    mdsp_issue_qdsp6_command ( &cmd, GFW_CODEC_SET_CONFIGURE_CMD , gas_id );
}

#endif


/*===========================================================================

FUNCTION mdsp_async_rx_sch

DESCRIPTION
  This function tells the mDSP to start an SCH receive.

  time_offset - the offset of the burst from the serivng cell frame tick
                (in quarter symbols)
  phase_increment - the phase increment (rotator value) to use for this burst,
                    needed to handle ncell activity on differing band from
                    scell.  Ignored if the rx rotator is not enabled in the
                    channel filter general ctl.

DEPENDENCIES
  mdsp_async_rx_time_offset - should be set prior to calling this routine

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_x2g_async_rx_sch( uint16                   time_offset,
                            int32                    phase_increment,
                            uint8*                   sch_metrics_buffer_idx_ptr,
                            uint16*                  sch_metrics_seq_num_ptr,
                            uint8*                   sch_data_buffer_idx_ptr,
                            ARFCN_T                  arfcn,
                            uint32                   rf_seq_num,
                            gas_id_t                 gas_id
                          )
{
   GfwCmdBufType      cmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.schCmd.cmdType                     = GFW_SCH_CMD;
   cmd.schCmd.ctrlWord.chanId             = MDSP_SCH;
   cmd.schCmd.ctrlWord.usePhaseInc        = 1;
   cmd.schCmd.sw_tsc_word.decodeFlag      = 1;
   cmd.schCmd.sw_tsc_word.swidth          = private_data_ptr->mdsp_async_rx_search_width;

   cmd.schCmd.burstMetricsBufIndex        = private_data_ptr->current_buffer_mod3;
   *sch_metrics_buffer_idx_ptr            = private_data_ptr->current_buffer_mod3;

   cmd.schCmd.schDecodeDataBufIndex       = private_data_ptr->mdsp_async_rx_data_wr_ptr;   /* This is not a pointer type */

   *sch_data_buffer_idx_ptr                = cmd.schCmd.schDecodeDataBufIndex;
   cmd.schCmd.rfBufIndex                  = private_data_ptr->current_buffer_mod3;
   public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[cmd.schCmd.burstMetricsBufIndex].len = 0;
   public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[cmd.schCmd.schDecodeDataBufIndex].len = 0;

   cmd.schCmd.time_offset                 = private_data_ptr->mdsp_sync_rx_time_offset + time_offset;
   cmd.schCmd.phaseIncrement              = phase_increment;
   cmd.schCmd.logIq                       = public_data_ptr->gsm_Iq_Log.sch_log_iq;
   cmd.schCmd.iqBufferIndex               = private_data_ptr->current_buffer ;
#ifdef FEATURE_GSM_COEX
   cmd.schCmd.arfcn                       = arfcn.num;
#endif

#ifdef FEATURE_GSM_COEX_FW_CXM
   if(gas_id==GERAN_ACCESS_STRATUM_ID_1)
   {
     cmd.schCmd.cxmPriority  = 150;
     cmd.schCmd.cxmDesenseId = 0xFFFFFFF0;
   }
   else if(gas_id==GERAN_ACCESS_STRATUM_ID_2)
   {
     cmd.schCmd.cxmPriority  = 135;
     cmd.schCmd.cxmDesenseId = 0xFFFFFFF0;
   }
   else
   {
     cmd.schCmd.cxmPriority  = 115;
     cmd.schCmd.cxmDesenseId = 0xFFFFFFF0;
   }    

   MSG_GERAN_HIGH_2_G("CXM : SCH coex_priority=%d coex_desense_id=%d", cmd.schCmd.cxmPriority, cmd.schCmd.cxmDesenseId);
#else
   cmd.schCmd.cxmPriority  = 0;
   cmd.schCmd.cxmDesenseId = 0;
#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
   cmd.schCmd.enableAeqOnSch = public_data_ptr->mdsp_aeq_sch_enable;
#else
   cmd.schCmd.enableAeqOnSch = FALSE;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
   cmd.schCmd.doJdetReading = FALSE;
   if(gl1_hw_get_sawless_HLLL_support(arfcn.band,gas_id))
     {
     cmd.schCmd.doJdetReading = TRUE;
     }
#endif /*GERAN_L1_HLLL_LNASTATE*/

    cmd.schCmd.rfSeqNr = rf_seq_num;

   /* Issue command to Q6 FW */
   mdsp_issue_qdsp6_command ( &cmd,GFW_SCH_CMD , gas_id );

   /* Save the seq_num of this command to use later when decoding the data */
   private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_wr_ptr] =
     mdsp_get_current_seq_num( gas_id );

   *sch_metrics_seq_num_ptr = private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_wr_ptr];

   if (++private_data_ptr->mdsp_async_rx_data_wr_ptr >= MDSP_ASYNC_RX_DATA_FIFO_LEN)
   {
      private_data_ptr->mdsp_async_rx_data_wr_ptr = 0;
   }

   /* Save the metrics sequence number */
   private_data_ptr->mdsp_async_rx_metrics_seq_num_fifo[private_data_ptr->current_buffer_mod3] = mdsp_get_current_seq_num( gas_id );


}

void mdsp_async_rx_sch
(
   uint16 time_offset,
   int32  phase_increment,
   uint32 rf_seq_num,
#ifdef FEATURE_GSM_COEX
   uint32 cxm_priority,
   uint32 cxm_desense_id,
#endif
   ARFCN_T arfcn,
#ifdef FEATURE_GSM_RX_DIVERSITY
   boolean enable_RxDiversity,
#endif
   gas_id_t gas_id
)
{
   GfwCmdBufType      cmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY 
   gl1_rxd_control_type rxd_ctl_flags;
#endif

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.schCmd.cmdType                     = GFW_SCH_CMD;
   cmd.schCmd.ctrlWord.chanId             = MDSP_SCH;
   cmd.schCmd.ctrlWord.usePhaseInc        = 1;
   cmd.schCmd.sw_tsc_word.decodeFlag      = 1;
   cmd.schCmd.sw_tsc_word.swidth          = private_data_ptr->mdsp_async_rx_search_width;
   cmd.schCmd.burstMetricsBufIndex        = private_data_ptr->current_buffer_mod3;
   cmd.schCmd.schDecodeDataBufIndex       = private_data_ptr->mdsp_async_rx_data_wr_ptr;
   cmd.schCmd.rfBufIndex                  = private_data_ptr->current_buffer_mod3;
  public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[cmd.schCmd.burstMetricsBufIndex].len = 0;
  public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[cmd.schCmd.schDecodeDataBufIndex].len = 0;
   cmd.schCmd.time_offset                 = private_data_ptr->mdsp_sync_rx_time_offset + time_offset;
   cmd.schCmd.phaseIncrement              = phase_increment;
   cmd.schCmd.logIq                       = public_data_ptr->gsm_Iq_Log.sch_log_iq;
   cmd.schCmd.iqBufferIndex               = private_data_ptr->current_buffer;
#ifdef FEATURE_GSM_SENS_SUPPORT
   if(gl1_hw_get_sens_fr_ctrl())
   {
     cmd.schCmd.enableSens                = TRUE;
   }
   else
   {
     cmd.schCmd.enableSens                = FALSE;   
   }
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY 
   if(enable_RxDiversity)
   {
     rxd_ctl_flags                        = gl1_get_rxd_control_flags(gas_id);
     cmd.schCmd.rxdControl.enableRxd      = (rxd_ctl_flags.enableRxd == 1)?1:0;	 
     cmd.schCmd.rxdControl.enablePrxOnly  = (rxd_ctl_flags.enablePrx == 1)?1:0;
     cmd.schCmd.rxdControl.enableDrxOnly  = (rxd_ctl_flags.enableDrx == 1)?1:0;
   }
#endif /*FEATURE_GSM_RX_DIVERSITY*/


#ifdef FEATURE_GSM_COEX
   cmd.schCmd.arfcn                       = arfcn.num;
#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
   cmd.schCmd.enableAeqOnSch = public_data_ptr->mdsp_aeq_sch_enable;
#else
   cmd.schCmd.enableAeqOnSch = FALSE;
#endif

#ifdef FEATURE_GSM_COEX_FW_CXM
   cmd.schCmd.cxmPriority  = cxm_priority;
   cmd.schCmd.cxmDesenseId = cxm_desense_id;

   MSG_GERAN_LOW_2_G("CXM : SCH coex_priority=%d coex_desense_id=%d", cmd.schCmd.cxmPriority, cmd.schCmd.cxmDesenseId);
#else
   cmd.schCmd.cxmPriority  = 0;
   cmd.schCmd.cxmDesenseId = 0;
#endif

#ifdef GERAN_L1_HLLL_LNASTATE
   cmd.schCmd.doJdetReading = FALSE;
   if(gl1_hw_get_sawless_HLLL_support(arfcn.band,gas_id))
     {
     cmd.schCmd.doJdetReading = TRUE;
     }
#endif /*GERAN_L1_HLLL_LNASTATE*/

   cmd.schCmd.rfSeqNr = rf_seq_num;

   /* Issue command to Q6 FW */
   mdsp_issue_qdsp6_command ( &cmd,GFW_SCH_CMD , gas_id );

   /* Save the seq_num of this command to use later when decoding the data */
   private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_wr_ptr] =
     mdsp_get_current_seq_num( gas_id );
   if (++private_data_ptr->mdsp_async_rx_data_wr_ptr >= MDSP_ASYNC_RX_DATA_FIFO_LEN)
   {
      private_data_ptr->mdsp_async_rx_data_wr_ptr = 0;
   }

   /* Save the metrics sequence number */
   private_data_ptr->mdsp_async_rx_metrics_seq_num_fifo[private_data_ptr->current_buffer_mod3] = mdsp_get_current_seq_num( gas_id );
}


/*===========================================================================

FUNCTION mdsp_async_rx

DESCRIPTION
  This function tells the mDSP to start asynchronous reception of a burst.
  This would usually be used for reception from neighbor cells.

  chan        - which logical channel to transmit on
  TSC         - the training sequence expected in the burst
  decode      - indicates if this is the final burst in a sequence of bursts
  time_offset - the offset of the burst from the serivng cell frame tick
                (in quarter symbols)
  phase_increment - the phase increment (rotator value) to use for this burst,
                    needed to handle ncell activity on differing band from
                    scell.  Ignored if the rx rotator is not enabled in the
                    channel filter general ctl.
  cxm_priority - Priority for CXM
  cxm_desense_id - Desense ID for CXM
  NOTE: This function issues a _sync_ rx command to the mDSP.  This
        means the results are available sooner than they otherwise would be.

DEPENDENCIES
  mdsp_async_rx_time_offset - should be set prior to calling this routine

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_async_rx
(

   mdsp_async_params_type *params,
   mdsp_channel_type       chan,
   uint8                   TSC,
   boolean                 decode,
   boolean                 early_decoding,
   boolean                 sbd_enabled,
   boolean                 epd_allowed,
   uint8                   burst_num,
   uint16                  time_offset,
   int32                   phase_increment,
   mdsp_burst_mode         mdsp_burst_type,
   uint32                  rf_seq_num,
   uint16                  arfcn,
#ifdef FEATURE_GSM_COEX
   uint32                  cxm_priority,
   uint32                  cxm_desense_id,
#endif
   gas_id_t                gas_id
)
{

GfwCmdBufType             cmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memset(&cmd,0,sizeof(GfwCmdBufType));
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   if (mdsp_burst_type == RX_MODE_ASYNC)
   {
     cmd.rxCmd.cmd = GFW_ASYNC_RX_CMD;
     cmd.rxCmd.slotInfo[0].controlField.bSbDecNullPage = sbd_enabled;
     cmd.rxCmd.slotInfo[0].controlField.enableEPD      = epd_allowed;
   }
   else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
   {
     cmd.rxCmd.cmd = GFW_SYNC_RX_CMD;
   }
   cmd.rxCmd.controlField.tsc = TSC;
   cmd.rxCmd.controlField.uncWin = mdsp_get_sync_rx_search_width( gas_id );

   cmd.rxCmd.controlField.arfcn = arfcn;

   cmd.rxCmd.schedulerResultBufIndex     = private_data_ptr->current_buffer_mod3;
   cmd.rxCmd.metricsBufIndex             = private_data_ptr->current_buffer_mod3 + TRIPPLEBUF;  /* Add 3 as shares with sync metric Buffer */
   cmd.rxCmd.rfBufIndex                  = private_data_ptr->current_buffer_mod3;
   cmd.rxCmd.slotInfo[0].decodedDataBufferIndex = private_data_ptr->mdsp_async_rx_data_wr_ptr + MDSP_DTM_NUM_RX_DATA_BUFFERS;
   cmd.rxCmd.slotInfo[0].logIq           = FALSE;
   cmd.rxCmd.phaseIncrement              = phase_increment;
   cmd.rxCmd.numSlots                    = 1;

   if (ftm_get_mode() == FTM_MODE)
   {
      cmd.rxCmd.rxLeveldB = NO_IQ_SCALING_IN_FTM;
      cmd.rxCmd.rxLevelDrxdB = NO_IQ_SCALING_IN_FTM;
   }
   else 
   {
      cmd.rxCmd.rxLeveldB = params->rx_lev_dBm >> 4;
#ifdef FEATURE_GSM_RX_DIVERSITY
      cmd.rxCmd.rxLevelDrxdB = NO_IQ_SCALING_IN_FTM; // need support as a separate CR.
#else
      cmd.rxCmd.rxLevelDrxdB = NO_IQ_SCALING_IN_FTM;
#endif
   }

   /* Fill in the command */
   cmd.rxCmd.slotInfo[0].controlField.forceFacchCs1 = 0;
   cmd.rxCmd.slotInfo[0].controlField.lookForFacch = 0;
   /* GFW expects firstBurst to be set for the 1st burst sent over the air, not
      for the first burst GFW has had an opportunity to decode (if we miss one) */
   cmd.rxCmd.slotInfo[0].controlField.firstBurst = burst_num == 0 ? 1 : 0;
   cmd.rxCmd.slotInfo[0].controlField.aciDetect = 0;
   cmd.rxCmd.slotInfo[0].controlField.decodeFlag = decode;

   if ( gl1_is_early_decode_disabled( gas_id ) )
   {
     /* Never set early decode to TRUE for F/W if feature enabled */
     cmd.rxCmd.slotInfo[0].controlField.earlyDecode = 0;
   }
   else
   {
     cmd.rxCmd.slotInfo[0].controlField.earlyDecode = ( early_decoding && decode );
   }

   cmd.rxCmd.slotInfo[0].controlField.cmiPhase = 1;
    /*For desense channels frequency error is corrected by GFW using rotator*/
   if (params->DesenseArfcn != DESENSE_CHAN)
   {
      cmd.rxCmd.slotInfo[0].controlField.usePhaseInc = FALSE;
   }
   else
   {
      cmd.rxCmd.slotInfo[0].controlField.usePhaseInc = TRUE;
   }
   cmd.rxCmd.slotInfo[0].chanTsField.chanType = (GfwChanType)chan;
   cmd.rxCmd.slotInfo[0].chanTsField.tsNum = time_offset / QS_PER_SLOT;
   cmd.rxCmd.slotInfo[0].offset = private_data_ptr->mdsp_sync_rx_time_offset + time_offset;
   cmd.rxCmd.slotInfo[0].logNewIq  = public_data_ptr->gsm_Iq_Log.sync_rx_iq;
   cmd.rxCmd.iqBufferIndex = private_data_ptr->current_buffer;

   cmd.rxCmd.slotInfo[0].saicControl.spurChanFlag = params->DesenseArfcn;

   /* Enable SAIC for non-data channels */
   if (mdsp_dtm_use_saic(chan, params->saic , gas_id ))
   {
     cmd.rxCmd.slotInfo[0].saicControl.enableSaic = 1;
     cmd.rxCmd.slotInfo[0].saicControl.shortSaic = MDSP_SAIC_SHORT;
     cmd.rxCmd.slotInfo[0].saicControl.gainRangeFlag = params->rf_gain.range==1? 1 : 0;

#ifdef FEATURE_GSM_MDSP_ESAIC
          if(mdsp_dtm_use_esaic(chan,params->saic , gas_id ))
          {
            cmd.rxCmd.slotInfo[0].saicControl.enableESaic = 1;
          }
          else
          {
            cmd.rxCmd.slotInfo[0].saicControl.enableESaic = 0;
          }
          /* Does ESAIC IIR filter need disabling? */
          if( mdsp_dtm_disable_iir_filter(chan,params->idle) )
          {
            /* disable iir filter as we are scheduling PCH or BCCH */
            cmd.rxCmd.slotInfo[0].saicControl.suspendIir = 1;
          }else
          {
            cmd.rxCmd.slotInfo[0].saicControl.suspendIir = 0;
          }
#endif /* FEATURE_GSM_MDSP_ESAIC */

   }


   /* clear the RxDecodedDataBuffer completely prior to usage */
   memset((GfwDecodedDataStruct*)&(public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[cmd.rxCmd.slotInfo[0].decodedDataBufferIndex]), 0, sizeof(GfwDecodedDataStruct));

   /* Clear the semaphores of the buffers where data will be placed */
   if (decode)
   {
     public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[cmd.rxCmd.slotInfo[0].decodedDataBufferIndex].len = 0;
   }

  public_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers[cmd.rxCmd.metricsBufIndex].gfwBurstMetricsBuffers[0].len = 0;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  if ((mdsp_burst_type == RX_MODE_ASYNC) && ( burst_num != 0 ))
  {
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
    if((gl1_check_if_sub_in_two_phase_access(gas_id)) && (chan == MDSP_BCCH))
    {
      cmd.rxCmd.slotInfo[0].controlField.prioritySchedule  = 0;
    }
    else
#endif
     cmd.rxCmd.slotInfo[0].controlField.prioritySchedule  = 1;
  }
  else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
  {
     cmd.rxCmd.slotInfo[0].controlField.prioritySchedule  = 0;
  }
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
   MSG_GERAN_HIGH_3_G("CXM : Async Rx metricsBufIndex=%d decode index=%u prioritySchedule=%u ",
                       cmd.rxCmd.metricsBufIndex, cmd.rxCmd.slotInfo[0].decodedDataBufferIndex,cmd.rxCmd.slotInfo[0].controlField.prioritySchedule);
#else 
   MSG_GERAN_HIGH_2_G("CXM : Async Rx coex_priority=%d coex_desense_id=%u", cmd.rxCmd.slotInfo[0].cxmPriority, cmd.rxCmd.slotInfo[0].cxmDesenseId);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */

#ifdef FEATURE_GSM_COEX_FW_CXM
   cmd.rxCmd.slotInfo[0].cxmPriority  = cxm_priority;
   cmd.rxCmd.slotInfo[0].cxmDesenseId = cxm_desense_id;
#else
   cmd.rxCmd.slotInfo[0].cxmPriority  = 0;
   cmd.rxCmd.slotInfo[0].cxmDesenseId = 0;
#endif

   cmd.rxCmd.rfSeqNr = rf_seq_num;

   /* Send command down to Q6 Firmware */
   mdsp_issue_qdsp6_command ( &cmd ,GFW_SYNC_RX_CMD , gas_id );

   /* Save the seq_num of this command to use later when decoding the data */
   if (decode)
   {
      private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_wr_ptr] =
        mdsp_get_current_seq_num( gas_id );
      if (++private_data_ptr->mdsp_async_rx_data_wr_ptr >= MDSP_ASYNC_RX_DATA_FIFO_LEN)
      {
         private_data_ptr->mdsp_async_rx_data_wr_ptr = 0;
      }
   }

   /* Save the metrics sequence number */
   private_data_ptr->mdsp_async_rx_metrics_seq_num_fifo[private_data_ptr->current_buffer_mod3] = mdsp_get_current_seq_num( gas_id );
}

/*===========================================================================

FUNCTION mdsp_get_async_decoded_data

DESCRIPTION
  Retreive the data produced by an async receive (and decode).

  NOTE: Currently this function uses the same buffers as sync receives.

  This function may be called repeatedly to poll for results being
  available.  The parameter last_time is provided to support this
  fuctionality.  If this functions returns TRUE or last_time is TRUE
  then the next call to this function will expect that another block
  has been decoded (buffer pointers are updated).  Unless this
  function returns TRUE or last_time is TRUE this function will not
  move on to the next block. I.e. on the last attempt to get the
  data for a particular block last_time must be set to TRUE.


DEPENDENCIES
  An async receive should be intitated N frames before calling this function.
  The value of N is not well defined at this time.

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  The pointer to the results buffer in shared memory is updated if this
  function returns TRUE or if last_time is TRUE.

===========================================================================*/
boolean mdsp_get_async_decoded_data
(
  GfwDecodedDataStruct *hdr,
  uint16         *data,
  uint16         data_len,
  boolean        last_attempt
  , gas_id_t gas_id
)
{
   boolean results_avail = FALSE;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   uint8 local_async_rx_data_rd_ptr = private_data_ptr->mdsp_async_rx_data_rd_ptr + MDSP_DTM_NUM_RX_DATA_BUFFERS;

   /* Read the data from the buffer and see if the seq_num matches one of our requests */
   if (public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].len)
   {

      hdr->gfwDecodedCtrlWord1    = (public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].gfwDecodedCtrlWord1);
      hdr->gfwDecodedCtrlWord2    = (public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].gfwDecodedCtrlWord2);
      hdr->len           = public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].len;
      hdr->seqNum        = public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].seqNum;
      hdr->hardRxQual    = public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].hardRxQual;
      hdr->softRxQual    = public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].softRxQual;
      memscpy(data,
              data_len,
              (void *)public_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[local_async_rx_data_rd_ptr].data1,
              data_len);

      if (hdr->seqNum == private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_rd_ptr])
      {
         (void) mdsp_validate_decoded_data(hdr, FALSE , gas_id );
         results_avail = TRUE;
      }
      else
      {
         MSG_GERAN_ERROR_3_G("Async data: seq num mismatch %d!=%d, FN=%d",
                   hdr->seqNum,
                   private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_rd_ptr],
                   GSTMR_GET_FN_GERAN(gas_id));
      }
   }
   else if (last_attempt)
   {
      MSG_GERAN_ERROR_1_G("Async data not available, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Increment fifo pointers if necessary */
   if (results_avail || last_attempt)
   {
      if (++private_data_ptr->mdsp_async_rx_data_rd_ptr >= MDSP_ASYNC_RX_DATA_FIFO_LEN)
      {
         private_data_ptr->mdsp_async_rx_data_rd_ptr = 0;
      }
   }

   return results_avail;
}

/*===========================================================================

FUNCTION mdsp_get_ncell_sch_decoded_data

DESCRIPTION
  Retreive the data produced by an SCH receive (and decode).

  NOTE: Currently this function uses the same buffers as sync receives.

  This function may be called repeatedly to poll for results being
  available.  The parameter last_time is provided to support this
  fuctionality.  If this functions returns TRUE or last_time is TRUE
  then the next call to this function will expect that another block
  has been decoded (buffer pointers are updated).  Unless this
  function returns TRUE or last_time is TRUE this function will not
  move on to the next block. I.e. on the last attempt to get the
  data for a particular block last_time must be set to TRUE.


DEPENDENCIES
  An async receive should be intitated N frames before calling this function.
  The value of N is not well defined at this time.

RETURN VALUE
  TRUE if packet available, FALSE otherwise

SIDE EFFECTS
  The pointer to the results buffer in shared memory is updated if this
  function returns TRUE or if last_time is TRUE.

===========================================================================*/

boolean mdsp_x2g_get_ncell_sch_decoded_data
(
   GfwSchDecodeDataStruct*  hdr_ptr,
   uint16                   data[2],  /* MAX_SCH_OCTETS / 2 */
   uint16                   data_len,
   boolean                  last_attempt,
   uint8                    sch_data_buffer_idx,
   uint16                   sch_data_seq_num
   ,gas_id_t                gas_id
)
{
   boolean results_avail = FALSE;
   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

#ifdef FEATURE_GPRS_GBTA
  if ( (gl1_msg_get_multi_sim_sys_mode()==SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ) &&
       (IS_GBTA_ACTIVE())
     )
  {
    /*Clear the schedule bit indicating burst is blanked for RX Burst, blanking should be only happened when it is DSDS Mode and GBTA is active*/
   hdr_ptr->reason = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].reason;
  }
  else
  {
    /* based on GFW information, this flag is just clear in GFW Side when it did not be schduled in DSDS and GBTA. otherwise it is schduled by default.
     * However, this field shouldn't be considered if we are not in DSDS and GBTA is not active
     * Hence, explicitly set it to zero
     */
     hdr_ptr->reason = SCH_SCHEDULED_BIT;
	 MSG_GERAN_HIGH_1_G("GL1 hdr->reason status (1 << 16) %d ",hdr_ptr->reason);
  }
#endif /* FEATURE_GPRS_GBTA */

   if( public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].len > 0 )
   {
      /*Run Error Recovery algorithm even if one burst dropped because of RF script building delay*/
      if (public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].scheduleTimeError)
      {
        hdr_ptr->scheduleTimeError = TRUE;
      }
      else
      {
        hdr_ptr->scheduleTimeError = FALSE;
      }
      gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, hdr_ptr->scheduleTimeError, gas_id);

      hdr_ptr->control[0]    = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].control[0];
      hdr_ptr->control[1]    = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].control[1];
      hdr_ptr->len           = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].len;
      hdr_ptr->seq_num       = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].seq_num;
      hdr_ptr->rx_qual_hard  = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].rx_qual_hard;
      hdr_ptr->rx_qual_soft  = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].rx_qual_soft;
      data[0]                = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].data_1[0];
      data[1]                = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].data_1[1];


      if( hdr_ptr->seq_num == sch_data_seq_num )
      {
         results_avail = TRUE;
      }
      else
      {
         MSG_GERAN_ERROR_3_G("Async data: seq num mismatch %d!=%d, FN=%d",
                   hdr_ptr->seq_num,
                   sch_data_seq_num,
                   GSTMR_GET_FN_GERAN( gas_id ));
      }

   public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[sch_data_buffer_idx].len = 0;
   }
   else if (last_attempt)
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      MSG_GERAN_ERROR_2_G("Async data not available, FN=%d reason %d", GSTMR_GET_FN_GERAN( gas_id ),hdr_ptr->reason);
#else
      MSG_GERAN_ERROR_1_G("Async data not available, FN=%d", GSTMR_GET_FN_GERAN( gas_id ));
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   }

   return  results_avail;
}


boolean mdsp_get_ncell_sch_decoded_data
(
   GfwSchDecodeDataStruct *hdr,

   uint16         *data,
   uint16         data_len,
   boolean        last_attempt
  , gas_id_t gas_id
)
{
   boolean results_avail = FALSE;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   
#ifdef FEATURE_GPRS_GBTA
  if ( (gl1_msg_get_multi_sim_sys_mode()==SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ) &&
       (IS_GBTA_ACTIVE())
     )
  {
   hdr->reason = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].reason;
  }
  else
  {
    /* based on GFW information, this flag is just clear in GFW Side when it did not be schduled in DSDS and GBTA. otherwise it is schduled by default.
     * However, this field shouldn't be considered if we are not in DSDS and GBTA is not active
     * Hence, explicitly set it to zero
     */
     hdr->reason = SCH_SCHEDULED_BIT;
	 MSG_GERAN_HIGH_1_G("GL1 hdr->reason status (1 << 16) %d ",hdr->reason);
  }
#endif /* FEATURE_GPRS_GBTA */

   if (public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].len > 0)
   {
      /*Run Error Recovery algorithm even if one burst dropped because of RF script building delay*/
      if (public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].scheduleTimeError)
      {
        hdr->scheduleTimeError = TRUE;
      }
      else
      {
        hdr->scheduleTimeError = FALSE;
      }
      gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, hdr->scheduleTimeError, gas_id);

      hdr->control[0]    = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].control[0];
      hdr->control[1]    = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].control[1];
      hdr->len           = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].len;
      hdr->seq_num       = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].seq_num;
      hdr->rx_qual_hard  = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].rx_qual_hard;
      hdr->rx_qual_soft  = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].rx_qual_soft;
      data[0]            = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].data_1[0];
      data[1]            = public_data_ptr->pMdspHostResultsStore->gfwSchDecodedDataBuffer[private_data_ptr->mdsp_async_rx_data_rd_ptr].data_1[1];


      if (hdr->seq_num == private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_rd_ptr])
      {
         /* kishore: need to do this */
         //(void) mdsp_validate_decoded_data(hdr, FALSE , gas_id );
         results_avail = TRUE;
      }
      else
      {
         MSG_GERAN_ERROR_3_G("Async data: seq num mismatch %d!=%d, FN=%d",
                   hdr->seq_num,
                   private_data_ptr->mdsp_async_rx_data_seq_num_fifo[private_data_ptr->mdsp_async_rx_data_rd_ptr],
                   GSTMR_GET_FN_GERAN(gas_id));
      }

      /*For first SCH attempt, ignore data if metrics is not retreived from GFW. This is not applicable in INACTIVE mode*/
      if((l1_tskisr_blk->l1_state != L1_INACTIVE_MODE) &&(!(last_attempt)) && (!(gl1_hw_check_metrics_done(gas_id))))
      {
        MSG_GERAN_ERROR_2_G("Ignoring data as metrics not available FN=%d state %d", GSTMR_GET_FN_GERAN(gas_id),l1_tskisr_blk->l1_state);

        results_avail = FALSE;
      }
   }
   else if (last_attempt)
   {
      MSG_GERAN_ERROR_1_G("Async data not available, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
   }

   /* Increment fifo pointers if necessary */
   if (results_avail || last_attempt)
   {
      if (++private_data_ptr->mdsp_async_rx_data_rd_ptr >= MDSP_ASYNC_RX_DATA_FIFO_LEN)
      {
         private_data_ptr->mdsp_async_rx_data_rd_ptr = 0;
      }
   }

   return results_avail;
}

/*===========================================================================

FUNCTION mdsp_get_async_rx_metrics

DESCRIPTION
  This function retrieves the rx metrics that are returned by the mdsp as part of
  an async_rx request.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_x2g_get_async_sch_rx_metrics
 (
    GfwBurstMetricsStruct  *metrics,
    boolean                last_time,
   uint8                   metrics_buffer_idx,
   uint16                  seq_num
   ,gas_id_t              gas_id
 )
 {
   boolean metrics_available = FALSE;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   /* Sch flag is true means we are doing ncell sch and we need to extract the burst metrics from
         gfwSchRxBurstMetricsBuffer buffer. This is difference from Q4. In Q4 we use the gfwAsyncMetricBuffers
         buffers for both ncell sch and ccch. But in Q6 we use gfwSchRxBurstMetricsBuffer for ncell sch and
         gfwAsyncMetricBuffers for ccch*/
     if(public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].len > 0)
     {
       metrics->len           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].len;
       metrics->seqNum        = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].seq_num;
       metrics->pdch          = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].pdch;
       metrics->rssiMsw       = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].rssi_h;
       metrics->rssiLsw       = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].rssi_l;
       metrics->dcI           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].dc_offset_i;
       metrics->dcQ           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].dc_offset_q;
       metrics->freqOffset    = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].freq_offset;
       metrics->timingOffset  = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].timing_offset;
       metrics->snr           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].snr;
       metrics->aeqLogPacket  = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].aeqLogPacket;
#ifdef GERAN_L1_HLLL_LNASTATE 
       metrics->jdetReading   = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[metrics_buffer_idx].jdetReading;
#endif /*GERAN_L1_HLLL_LNASTATE*/

       if (metrics->seqNum != seq_num)
       {
         MSG_GERAN_ERROR_3_G("Async rx metrics: bad seq_num %d != %d, FN=%d",
                   metrics->seqNum, seq_num, GSTMR_GET_FN_GERAN( gas_id ));
       }
       else
       {
          metrics_available = TRUE;

          (void) mdsp_validate_rx_metrics(metrics, gas_id );
       }
     }

   return metrics_available;

}

boolean mdsp_get_async_rx_metrics
(
   GfwBurstMetricsStruct     *metrics,
   boolean last_attempt,
   boolean sch_flag
#ifdef FEATURE_GSM_SENS_SUPPORT
   , GfwGdetStruct *gfwGdetResults
#endif
   , gas_id_t gas_id
)
{
   boolean metrics_available = FALSE;
   uint16  index, seq_num;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Calculate the index in the circular buffer */
   index = private_data_ptr->current_buffer_mod3;
   if (!last_attempt) index = (private_data_ptr->current_buffer_mod3 + 3 - 2) % 3;

   /* Get the current expected Sequence number */
   seq_num = private_data_ptr->mdsp_async_rx_metrics_seq_num_fifo[index];

   if(sch_flag == TRUE)
   {
     /* Sch flag is true means we are doing ncell sch and we need to extract the burst metrics from
         gfwSchRxBurstMetricsBuffer buffer. This is difference from Q4. In Q4 we use the gfwAsyncMetricBuffers
         buffers for both ncell sch and ccch. But in Q6 we use gfwSchRxBurstMetricsBuffer for ncell sch and
         gfwAsyncMetricBuffers for ccch*/
     if(public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].len > 0)
     {
       metrics->len           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].len;
       metrics->seqNum        = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].seq_num;
       metrics->pdch          = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].pdch;
       metrics->rssiMsw       = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].rssi_h;
       metrics->rssiLsw       = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].rssi_l;
       metrics->dcI           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].dc_offset_i;
       metrics->dcQ           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].dc_offset_q;
       metrics->freqOffset    = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].freq_offset;
       metrics->timingOffset  = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].timing_offset;
       metrics->snr           = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].snr;
       metrics->aeqLogPacket  = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].aeqLogPacket;
#ifdef FEATURE_GSM_RX_DIVERSITY
       metrics->rxdLogPacket  = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].rxdLogPacket;
#endif
#ifdef GERAN_L1_HLLL_LNASTATE 
       metrics->jdetReading   = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].jdetReading;
#endif /*GERAN_L1_HLLL_LNASTATE*/
#ifdef FEATURE_GSM_SENS_SUPPORT
       if(gl1_hw_get_sens_fr_ctrl())
       {
         *gfwGdetResults = public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].gfwGdetResults;
       }
#endif

       if (metrics->seqNum!= seq_num)
       {
         MSG_GERAN_ERROR_3_G("Async rx metrics: bad seq_num %d != %d, FN=%d",
                   metrics->seqNum, seq_num, GSTMR_GET_FN_GERAN(gas_id));
       }
       else
       {
          metrics_available = TRUE;

          (void) mdsp_validate_rx_metrics(metrics , gas_id );

          /* Avoid stale results getting picked up later on so clear the length field */
          public_data_ptr->pMdspHostResultsStore->gfwSchRxBurstMetricsBuffer[index].len = 0;
       }
     }
   }
   else if(sch_flag == FALSE)
   {
     /* Sch flag is false true means we are doing ccch decoding and we need to extract the burst metrics from
         gfwAsyncMetricBuffers buffer. */
     /* If the semaphore is set then we can read the data */
     if (public_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers[index  + TRIPPLEBUF].gfwBurstMetricsBuffers[0].len > 0)
     {
        /* Read all the results */
        *metrics = * (&(public_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers[index + TRIPPLEBUF].gfwBurstMetricsBuffers[0]));

       /* The results are only valid if the seq num matches */
        if (metrics->seqNum!= seq_num)
        {
           MSG_GERAN_ERROR_3_G("Async rx metrics: bad seq_num %d != %d, FN=%d",
                     metrics->seqNum, seq_num, GSTMR_GET_FN_GERAN(gas_id));
        }
        else
        {
           metrics_available = TRUE;

           (void) mdsp_validate_rx_metrics(metrics , gas_id );
        }
     }
   }
   else if (last_attempt)
   {
      MSG_GERAN_ERROR_1_G("Async rx metrics not available, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
   }

   return metrics_available;

}

#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION mdsp_flush_esaic_iir_filter

DESCRIPTION
  Tells the mDSP to Clear it's IIR filter for use in applying the ESAIC on
  Downlink channels.

  Currently needs to be called For Downlink DTX and Handover scenarios

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_flush_esaic_iir_filter( boolean flush_esacch, boolean restore_ersacch , gas_id_t gas_id )
{
   GfwCmdBufType       cmd;

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.flushEsaicIIRFilter.cmd = GFW_FLUSH_ESAIC_IIR_FILTER_CMD;
   cmd.flushEsaicIIRFilter.flushEsacch = flush_esacch;
   cmd.flushEsaicIIRFilter.restoreErsacch =  restore_ersacch;

   mdsp_issue_qdsp6_command ( &cmd, GFW_FLUSH_ESAIC_IIR_FILTER_CMD , gas_id );
}

#endif

/*===========================================================================

FUNCTION mdsp_reset_async_rx_ptrs

DESCRIPTION
   Resets the pointers used for data and metrics buffers for async rx's.
   Used wehn aborting an async rx.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_reset_async_rx_ptrs(gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   private_data_ptr->mdsp_async_rx_data_wr_ptr    = 0;
   private_data_ptr->mdsp_async_rx_data_rd_ptr    = 0;
}



/*===========================================================================

FUNCTION mdsp_reset_rx_seq_nums

DESCRIPTION
  Resets the sequence number fifos for and async sync rx metrics and logging.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_reset_rx_seq_nums(gas_id_t gas_id )
{
   mdsp_dtm_reset_rx(gas_id);

   /* Async pointers */
   mdsp_reset_async_rx_ptrs( gas_id );
}



/****************************************************************************
*****************************************************************************

                   MISC FUNCTIONS

*****************************************************************************
*****************************************************************************/
#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION mdsp_set_tx_config_cmd_pending

DESCRIPTION
  This function indicates that a Tx config command is pending and is to be sent in the
  next GSTMR ISR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_tx_config_cmd_pending(uint32 txlm_buf_idx, gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr;
  GDRV_ISR_SAVE_LOCK(gas_id);
  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  public_data_ptr->tx_config_cmd_pending = txlm_buf_idx;

  GDRV_ISR_SAVE_UNLOCK(gas_id);
}
/*===========================================================================

FUNCTION mdsp_issue_tx_config_command_if_required

DESCRIPTION
  This function issues the Tx Config command if required

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_issue_tx_config_command_if_required(gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  if(public_data_ptr->tx_config_cmd_pending != MDSP_TX_CONFIG_NO_CMD_PENDING)
  {
    if (g1l_hw_is_mcpm_tx_on(gas_id) == TRUE)
    {
      /* Issue Tx Config command */
      mdsp_gsm_config_tx( public_data_ptr->tx_config_cmd_pending, gas_id );
      /* reset app_mode_cmd_pending_type variable */
      public_data_ptr->tx_config_cmd_pending = MDSP_TX_CONFIG_NO_CMD_PENDING;
    }
  }
}

/*===========================================================================

FUNCTION mdsp_get_tx_config_cmd_pending

DESCRIPTION
  This function retruns the status of Tx Config command if pending

DEPENDENCIES
  None

RETURN VALUE
 TRUE if Tx Config command is pending

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_get_tx_config_cmd_pending(gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  return (public_data_ptr->tx_config_cmd_pending != MDSP_TX_CONFIG_NO_CMD_PENDING);

}
#endif
/*===========================================================================

FUNCTION mdsp_issue_generic_config_command

DESCRIPTION
  This function ssues a generic configuration command to the firmware.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void mdsp_issue_generic_config_command(gas_id_t gas_id )
{
  static GfwGenericCmd      genericCmd[3];
  mdsp_intf_public_data_t*  public_data_ptr;

  gas_id = check_gas_id(gas_id);

  if ( !mdsp_awake( gas_id ) )
  {
    if ( gl1_asynch_int_dbg( gas_id ) )
    {
      MSG_GERAN_LOW_1_G(GL1_SLEEP_HDR" mDSP NOT AWAKE fn=%d",
                         GSTMR_GET_FN_GERAN( gas_id ) );
    }

    return;
  }

  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  if(public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated)
  {
    /* issue the command here */
    memset( &genericCmd[gas_id], 0, sizeof(GfwGenericCmd));

    genericCmd[gas_id].cmd                  = GFW_GENERIC_CONFIG_CMD;
    genericCmd[gas_id].rxSharedVarUpdated   = public_data_ptr->pendingGfwGenericCmd.rxSharedVarUpdated;
    genericCmd[gas_id].edgeSharedVarUpdated = public_data_ptr->pendingGfwGenericCmd.edgeSharedVarUpdated;

    /* reset the pending generic config buffer */
    public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.rxSharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.txSharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.edgeSharedVarUpdated = FALSE;

    mdsp_issue_qdsp6_command( (GfwCmdBufType *) &genericCmd[gas_id], GFW_GENERIC_CONFIG_CMD , gas_id );
  }
}

/*===========================================================================

FUNCTION mdsp_updated_shared_variable

DESCRIPTION
  This function populates the shared variables in the pending generic config buffer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_updated_shared_variable
(
  gfw_shared_variable_class_type shared_variable_class , gas_id_t gas_id
)
{
  mdsp_intf_public_data_t* public_data_ptr;

   /* Lock interrupts since this may be called from task context */
   GDRV_ISR_SAVE_LOCK(gas_id);

   public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   switch(shared_variable_class)
   {
     case GFW_RX_SHARED_VARIABLE:
        {
           public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = TRUE;
           public_data_ptr->pendingGfwGenericCmd.rxSharedVarUpdated = TRUE;
           break;
        }

     case GFW_TX_SHARED_VARIABLE:
        {
           public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = TRUE;
           public_data_ptr->pendingGfwGenericCmd.txSharedVarUpdated = TRUE;
           break;
        }
     case GFW_EDGE_SHARED_VARIABLE:
        {
           public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = TRUE;
           public_data_ptr->pendingGfwGenericCmd.edgeSharedVarUpdated = TRUE;
           break;
        }
     case GFW_SHARED_VARIABLE_FALSE:
        {
           public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = FALSE;
           break;
        }
     default:
        {
           MSG_GERAN_ERROR_1_G("GFW shared variable not supported, %d",(uint8)shared_variable_class);
           break;
        }
   }

   /* Unlock interrupts since this may be called from task context */
   GDRV_ISR_SAVE_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION mdsp_update_shared_variable

DESCRIPTION
  This function populates the shared variables in the GFW structure for the RF driver

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_update_shared_variable
(
  MdspSharedVariableType sharedVariableType,
  uint32 *pValue, /* this is may array or single value depending upon the shared variable type */
  gas_id_t gas_id
)
{
   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   switch(sharedVariableType)
   {
   case GFW_RX_ALPHA:
      {
          public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.useIdleModeAlpha =(boolean)*pValue;
          mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE , gas_id );
          break;
      }
   case GFW_DEBUG:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.gfwDebug =(uint32)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE , gas_id );
         break;
      }
   case GFW_VAMOS_SUPPORT:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.vamosSupport =(uint16)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE , gas_id );
         break;
      }
   case GFW_DIAG1:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.gfwDiag1 =(uint32)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE, gas_id);
         break;
      }   
   
   case GFW_DIAG2:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.gfwDiag2 =(uint32)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE, gas_id);
         break;
      }   
   
   case GFW_DIAG3:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.gfwDiag3 =(uint32)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE, gas_id);
         break;
      }   
   
   case GFW_DIAG4:
      {
         public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.gfwDiag4 =(uint32)*pValue;
         mdsp_updated_shared_variable(GFW_RX_SHARED_VARIABLE, gas_id);
         break;
      }   

   default:
      {
         MSG_GERAN_ERROR_1_G("MDSP RF Shared variable %d not supported", sharedVariableType);
      }
   }

}

/*===========================================================================

FUNCTION  mdsp_cmd_buffer_header

DESCRIPTION
  Add the num cmds and sequence number to the mdsp command buffer
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mdsp_cmd_buffer_header( gas_id_t gas_id )
{
   GfwHostCmdBufType      *hostCmd;
   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );


   /* Write the sequence number into the command buffer before sending */
   hostCmd = (GfwHostCmdBufType *)public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[private_data_ptr->current_buffer];
   hostCmd->cmdSeqNum = mdsp_get_current_seq_num(gas_id);

   /* Write num_cmds to command buffer */
   hostCmd->semaphoreAndNumCommands = private_data_ptr->mdsp_cmd_count;
   private_data_ptr->mdsp_cmd_count    = 0;
}

/*===========================================================================

FUNCTION  mdsp_issue_async_msg

DESCRIPTION
  if there are commands in the command buffer then this function issues
  the message to FW.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mdsp_issue_async_msg(GfwAsyncCmd cmdType , gas_id_t gas_id)
{
  GfwHostAsyncCmd gfwHostAsyncCmd;

  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );
  mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

  if( NULL == mdsp_intf_data_ptr->pMdspHostIfBaseStore  )
  {
    MSG_GERAN_ERROR_1_G(" mdsp_issue_async_msg pMdspHostIfBaseStore ox%p not ready ", mdsp_intf_data_ptr->pMdspHostIfBaseStore );
 
    return;
  }
  mdsp_pre_switch_command_buffers(gas_id);

  MSG_GERAN_LOW_3_G("ASYNC CMD: num_cmds %d, FN=%d, QS count=%d", private_data_ptr->mdsp_cmd_count, GSTMR_GET_FN_GERAN(gas_id),gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
  if(private_data_ptr->mdsp_cmd_count != 0)
  {
    gfwHostAsyncCmd.swFrameNo   = GSTMR_GET_FN_GERAN(gas_id);
    gfwHostAsyncCmd.hwFrameNo   = get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id));
    gfwHostAsyncCmd.cmdSeqNum   = mdsp_get_current_seq_num(gas_id);
    gfwHostAsyncCmd.hostCmdType = cmdType;
    gfwHostAsyncCmd.cmdBuffIdx  = private_data_ptr->current_buffer;


    /* Set sequence number and num_cmds */
    mdsp_cmd_buffer_header(gas_id);

    (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_ASYNC_CMD, &gfwHostAsyncCmd, NULL, gas_id);

    MSG_GERAN_LOW_1_G("sending msg router message to fw seq no is  %d",private_data_ptr->mdsp_current_seq_num);

    /* toggle command buffers now that FW have picked up previous commands*/
    mdsp_switch_command_buffers( geran_map_gas_id_to_nas_id(gas_id) );
  }
}

/*===========================================================================

FUNCTION  mdsp_wakeup_issue_async_msg

DESCRIPTION
  if there are commands in the command buffer then this function issues
  the message to FW. Specific for wake-up after sleep.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mdsp_wakeup_issue_async_msg(GfwAsyncCmd cmdType , gas_id_t gas_id, uint16 fn)
{
  GfwHostAsyncCmd gfwHostAsyncCmd;

  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );
  mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

  if( NULL == mdsp_intf_data_ptr->pMdspHostIfBaseStore  )
  {
    MSG_GERAN_ERROR_1_G(" mdsp_issue_async_msg pMdspHostIfBaseStore ox%p not ready ", mdsp_intf_data_ptr->pMdspHostIfBaseStore );
 
    return;
  }
  mdsp_pre_switch_command_buffers(gas_id);

  MSG_GERAN_HIGH_3_G("ASYNC CMD: VSTMR FN %d, SW FN=%d, QS count=%d", fn, GSTMR_GET_FN_GERAN(gas_id),gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
  if(private_data_ptr->mdsp_cmd_count != 0)
  {
    gfwHostAsyncCmd.swFrameNo   = GSTMR_GET_FN_GERAN(gas_id);
    gfwHostAsyncCmd.hwFrameNo   = fn;
    gfwHostAsyncCmd.cmdSeqNum   = mdsp_get_current_seq_num(gas_id);
    gfwHostAsyncCmd.hostCmdType = cmdType;
    gfwHostAsyncCmd.cmdBuffIdx  = private_data_ptr->current_buffer;


    /* Set sequence number and num_cmds */
    mdsp_cmd_buffer_header(gas_id);

    (void)mdsp_async_intf_send_blocking_cmd(GERAN_FW_ASYNC_CMD, &gfwHostAsyncCmd, NULL, gas_id);

    MSG_GERAN_LOW_1_G("sending msg router message to fw seq no is  %d",private_data_ptr->mdsp_current_seq_num);

    /* toggle command buffers now that FW have picked up previous commands*/
    mdsp_switch_command_buffers( geran_map_gas_id_to_nas_id(gas_id) );
  }
}

/*===========================================================================

FUNCTION mdsp_get_current_seq_num, mdsp_inc_current_seq_num

DESCRIPTION
  This function returns (increments) the sequence number used to differentiate
  command packets to the mdsp.  'current' is defined as the packet that the
  mdsp has yet to process.

===========================================================================*/
uint16 mdsp_get_current_seq_num (gas_id_t gas_id )
{
    mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

    return private_data_ptr->mdsp_current_seq_num;
}

uint16 mdsp_inc_current_seq_num (gas_id_t gas_id)
{
    mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

    if ( private_data_ptr->mdsp_current_seq_num >= 0xFFFF )
    {
      MSG_GERAN_ERROR_0_G( "Reset mdsp_seq_num" );

      private_data_ptr->mdsp_current_seq_num = 0;
    }

    return (++private_data_ptr->mdsp_current_seq_num);
}

/*===========================================================================

FUNCTION mdsp_issue_qdsp6_command

DESCRIPTION
  This function performs the writing of the command into mdsp space.
  Only one command is currently supported.

  cmd - pointer to command struct to issue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_issue_qdsp6_command (GfwCmdBufType *cmd,GfwMdspCmdType cmdId , gas_id_t gas_id)
{
   uint16                 len = 0;

   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );


   if(cmdId == GFW_START_ACQ_CMD)
   {
     len = sizeof(GfwStartAcqCmdBufStruct);
   }
   else if(cmdId == GFW_STOP_ACQ_CMD)
   {
     len = sizeof(GfwStopAcqCmdBufStruct);
   }
   else if(cmdId == GFW_GSM_TX_CMD)
   {
     len = sizeof(GfwGsmTxCmd);
   }
   else if(cmdId == GFW_SCH_CMD)
   {
     len = sizeof(GfwSchCmdBufStruct);
   }
   else if(cmdId == GFW_GPRS_PWR_MSR_CMD)
   {
     len = sizeof(GfwGsmPwrMsrCmd);
   }
   else if(cmdId == GFW_SYNC_RX_CMD)
   {
     len = sizeof(GfwGsmSyncRxCmd);
   }
   else if(cmdId == GFW_GENERIC_CONFIG_CMD)
   {
     len = sizeof(GfwGenericCmd);
   }
   else if(cmdId == GFW_LOOPBACK_CMD)
   {
     len = sizeof(GfwLoopBackCmd);
   }
   else if(cmdId == GFW_START_CONTINUOUS_TX_CMD)
   {
     len = sizeof(GfwStartTestTxCmd);
   }
   else if(cmdId == GFW_STOP_CONTINUOUS_TX_CMD)
   {
     len = sizeof(GfwStopTestTxCmd);
   }
   else if(cmdId == GFW_NON_SBI_REGISTER_WRITE_CMD)
   {
     len = sizeof(GfwNonSbiRegisterWriteCmd);
   }
   else if(cmdId == GFW_CIPHER_CONTROL_CMD)
   {
     len = sizeof(GfwCipherControlCmd);
   }
   else if(cmdId == GFW_CODEC_SET_CONFIGURE_CMD)
   {
     len = sizeof(GfwDlCodecSetConfigureCmd);
   }
   else if(cmdId == GFW_FLUSH_ESAIC_IIR_FILTER_CMD)
   {
     len = sizeof(GfwFlushEsaicIIRFilter);
   }
   else if(cmdId == GFW_CONFIGURE_IR_WINDOW_CMD)
   {
     len = sizeof(GfwConfigureIrWindowCmd);
   }
   else if(cmdId == GFW_TX_CAL_CMD)
   {
     len = sizeof(GfwTxCalCmd);
   }
   else if(cmdId == GFW_X2G_STARTUP_CMD)
   {
     len = sizeof(GfwX2GStartupCmd);
   }
   else if(cmdId == GFW_X2G_CLEANUP_CMD)
   {
     len = sizeof(GfwX2GCleanupCmd);
   }
   else if(cmdId == GFW_X2G_PRELOAD_CMD)
   {
     len = sizeof(GfwX2GPreLoadCmd);
   }
   else if(cmdId == GFW_X2G_PWR_MSR_CMD)
   {
     len = sizeof(GfwX2GPwrMsrCmd);
   }
   else if(cmdId == GFW_G2X_STARTUP_CMD)
   {
     len = sizeof(GfwG2XStartupCmd);
   }
   else if(cmdId == GFW_G2X_CLEANUP_CMD)
   {
     len = sizeof(GfwG2XCleanupCmd);
   }
   else if(cmdId == GFW_CONFIGURE_TX)
   {
     len = sizeof(GfwConfigureTxCmd);
   }
   else if(cmdId == GFW_AFC_UPDATE_CMD)
   {
     len = sizeof(GfwAfcUpdateCmd);
   }
   else if(cmdId == GFW_SET_APP_MODE_CMD)
   {
     len = sizeof(GfwSetAppModeCmd);
   }
#ifdef GERAN_L1_IP2CAL
   else if(cmdId == GFW_IP2_CAL_CMD)
   {
     len = sizeof(GfwIp2CalCmd);
   }
#endif /*GERAN_L1_IP2CAL*/

   /* Check length */
   if (len == 0)
   {
      MSG_GERAN_ERROR_2_G("Command length unknown: cmd=%d, FN=%d", cmd->cmdId, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }


   GDRV_ISR_SAVE_LOCK(gas_id);

   if (private_data_ptr->mdsp_cmd_count == 0)
   {
      /* Check for any errors */
      mdsp_check_for_errors(gas_id);
      /* Set offset to where commands will be written (skip cmd_cnt,seq_num) */
      private_data_ptr->mdsp_cmd_offset = 4;
   }

   /* Check for buffer overflows - should not happen, see comments in
      mdsp_cmd.h for the expected worst case buffer size */
   if (private_data_ptr->mdsp_cmd_offset + len >= MDSP_COMMAND_BUFFER_LENGTH)
   {
      MSG_5(MSG_SSID_DFLT, MSG_LEGACY_FATAL,
            "mDSP cmd buffer overflow by %d words: cmd=%d (len=%d), %d cmds, FN=%d",
            private_data_ptr->mdsp_cmd_offset + len - MDSP_COMMAND_BUFFER_LENGTH + 1, cmd->cmdId, len,
            private_data_ptr->mdsp_cmd_count, GSTMR_GET_FN_GERAN(gas_id));
      GDRV_ISR_SAVE_UNLOCK(gas_id);
      return;
   }

   /* Copy the cmd from ARM space to mdsp space */
   memscpy((void*)(public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[private_data_ptr->current_buffer] + private_data_ptr->mdsp_cmd_offset),
            (MDSP_COMMAND_BUFFER_LENGTH - private_data_ptr->mdsp_cmd_offset),
            cmd, 
            len);

   mdsp_cmd_buf_ptr[gas_id]=(void *)(public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[private_data_ptr->current_buffer] + private_data_ptr->mdsp_cmd_offset);

   private_data_ptr->mdsp_cmd_count+=1;

   /* Point to next location to store more commands */
   private_data_ptr->mdsp_cmd_offset += len;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
   /* Log the command */
   mdsp_log_qdsp6_cmds(cmd, len, private_data_ptr->mdsp_cmd_count , gas_id );

   GDRV_ISR_SAVE_UNLOCK(gas_id);

}

/*===========================================================================

FUNCTION mdsp_ftm_issue_qdsp6_command

DESCRIPTION
  API to aloow FTM mode to submit commands to GFW

===========================================================================*/

void mdsp_ftm_issue_qdsp6_command (GfwCmdBufType *cmd, GfwMdspCmdType  cmdId , sys_modem_as_id_e_type as_id )
{
  mdsp_issue_qdsp6_command(cmd,cmdId,geran_map_nas_id_to_gas_id(as_id));
}

/*===========================================================================

FUNCTION mdsp_clear_gap_cmds

DESCRIPTION
  Clear the command header and ccs semaphore for message that is not going
  to be sent.

===========================================================================*/
void mdsp_clear_gap_cmds(gas_id_t gas_id)
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );
  /* Reset count to 0 */
  private_data_ptr->mdsp_cmd_count = 0;
}


/*===========================================================================

FUNCTION mdsp_reset_command_buffers

DESCRIPTION
  This function resets the command buffer pointer and headers.

===========================================================================*/
void mdsp_reset_command_buffers (gas_id_t gas_id )
{
   GfwHostCmdBufType      *hostCmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   hostCmd = (GfwHostCmdBufType *)public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[0];
   hostCmd->semaphoreAndNumCommands = 0;
   hostCmd = (GfwHostCmdBufType *)public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[1];
   hostCmd->semaphoreAndNumCommands = 0;

   private_data_ptr->current_buffer = 0;
   private_data_ptr->current_buffer_mod3 = 0;

   private_data_ptr->mdsp_cmd_count    = 0;
}

/*===========================================================================

FUNCTION mdsp_switch_command_buffers

DESCRIPTION
  This function switches the command buffer in use such that new commands
  should now be placed into a new buffer.  This function should be called
  from the frame tick isr as that is when the mdsp will begin processing
  of the 'old' command buffer.

  It also performs some sanity check to ensure there are no dsp errors.

===========================================================================*/
void mdsp_switch_command_buffers (sys_modem_as_id_e_type as_id)
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  /* Switch command buffers */
  private_data_ptr->current_buffer = private_data_ptr->current_buffer ? 0 : 1;

  (void)mdsp_inc_current_seq_num(gas_id);
#ifdef FEATURE_GSM_COEX
  mdsp_process_cxm_logs(gas_id);
#endif
}

/*===========================================================================

FUNCTION mdsp_pre_switch_command_buffers

DESCRIPTION
  Before switching command buffers, post any pending commands into the buffer

===========================================================================*/
void mdsp_pre_switch_command_buffers (gas_id_t gas_id)
{
  /* Process any pending nonsbi transactions */
  mdsp_process_nonsbi_transactions(gas_id);
#ifndef FEATURE_GSM_RX_TX_SPLIT
  /* process any pending Tx config commands */
  mdsp_issue_tx_config_command_if_required(gas_id);
#endif
  /*For X2G operation only issue GCC via X2G Startup*/
  if ( mdsp_cm_active(gas_id) == FALSE || mdsp_commands_pending(gas_id )) 
  {
     mdsp_issue_generic_config_command(gas_id);
  }
}

/*===========================================================================

FUNCTION mdsp_inc_current_buffer_mod3

DESCRIPTION
  Increment current_buffer_mod3

===========================================================================*/
void mdsp_inc_current_buffer_mod3 (gas_id_t gas_id)
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  if (++private_data_ptr->current_buffer_mod3 > 2)
  {
    private_data_ptr->current_buffer_mod3 = 0;
  }

}

/*===========================================================================

FUNCTION mdsp_frame_tick_process

DESCRIPTION
  Tick GPRS and DTM FT processes and tick frame count

===========================================================================*/
void mdsp_frame_tick_process (gas_id_t gas_id)
{
  mdsp_intf_public_data_t* data = get_mdsp_intf_public_dataspace_ptr( gas_id );
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

 if (private_data_ptr->mdsp_init_complete == TRUE)
 {
  /* Let GPRS have the tick */
  mdsp_gprs_process_frame_tick(gas_id);

  /* Let DTM module have the tick */
  mdsp_dtm_process_frame_tick(gas_id);

  /* Increment frame count */
  data->mdsp_frame_tick_count++;

  if (data->mdsp_async_imm_cmd_sent == FALSE)
  {
     mdsp_inc_current_buffer_mod3 (gas_id);
  }
  data->mdsp_async_imm_cmd_sent = FALSE;
 }
}

/****************************************************************************
*****************************************************************************

                   PHONE TEST FUNCTIONS

*****************************************************************************
*****************************************************************************/

/*===========================================================================

FUNCTION mdsp_send_tx_cal_cmd

DESCRIPTION
  This function tells the mDSP to do the tx cal
  Explanation:
  This function is for KV calibration in FTM mode by sending
  ths sbi and grfc buffers to fw. This command also has the
  num of steps in the current frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_send_tx_cal_cmd ( mdsp_ftm_tx_cal_struct *tx_cal_buffer , gas_id_t gas_id )
{
/* This needs to be done once we get clarification from RF driver team */
    MSG_GERAN_ERROR_0_G("mdsp_send_tx_cal_cmd not yet supported for RF_TASK CCS IF");

}
/*===========================================================================

FUNCTION mdsp_start_tx_rnd_cont

DESCRIPTION
  This function tells the mDSP to transmit a tone every frame with random data.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_start_tx_rnd_cont ( uint8 gmsk_mod , gas_id_t gas_id )
{
   GfwCmdBufType      cmd;

   mdsp_intf_public_data_t*  public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.startTestTxCmd.cmd = GFW_START_CONTINUOUS_TX_CMD;
   cmd.startTestTxCmd.controlField.txDataModeType = GFW_TX_DATA_PSEUDO_RANDOM;

   cmd.startTestTxCmd.controlField.modulation = gmsk_mod;

   cmd.startTestTxCmd.offset = PHONET_DEFAULT_TX_TIME_OFFSET;

   cmd.startTestTxCmd.rfBufIndex       = private_data_ptr->current_buffer_mod3;
   private_data_ptr->mdsp_stop_cont_tx_buffer_mod3 = private_data_ptr->current_buffer_mod3;
   cmd.startTestTxCmd.dtmTxBufIndex = 0;

   /* Enable the rotator if requested */
   if (public_data_ptr->mdsp_tx_rotator)
   {
      cmd.startTestTxCmd.controlField.useRotator = 1;
   }
   else
   {
      cmd.startTestTxCmd.controlField.useRotator = 0;
   }

   cmd.startTestTxCmd.rfSeqNr = 0;
   mdsp_issue_qdsp6_command ( &cmd, GFW_START_CONTINUOUS_TX_CMD , gas_id );

}

/*===========================================================================

FUNCTION mdsp_ftm_start_tx_rnd_cont

DESCRIPTION
  This function tells the mDSP to transmit a tone every frame with random data.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_start_tx_rnd_cont ( uint8 gmsk_mod,sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_start_tx_rnd_cont ( gmsk_mod, gas_id );
}
/*===========================================================================

FUNCTION mdsp_start_tx_data_cont

DESCRIPTION
  This function tells the mDSP to transmit user defined data.
  Explanation:
  This function is for KV calibration in FTM mode by sending
  training data to reduce calibration time. It is similar to
  dynamic KV  calibration in online mode.  Instead of having
  G FW hard coding the training data as in online mode,  FTM
  code will pass training data  (user define)  to  FW during
  calibration.  Also,  we  don’t want G  FW to hard code the
  training  data in  continuous  TX.  This avoids disrupting
  RF calibration due to continuous TX being used in multiple
  target calibration.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_start_tx_data_cont ( uint16 *buffer, uint16 buf_len , gas_id_t gas_id )
{
   GfwCmdBufType      cmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memscpy((void*)&public_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[private_data_ptr->current_buffer],
            sizeof(GfwDtmTxStruct),
            buffer,
            (buf_len * 2));

   cmd.startTestTxCmd.cmd = GFW_START_CONTINUOUS_TX_CMD;
   cmd.startTestTxCmd.controlField.txDataModeType = GFW_TX_DATA_FROM_BUFFER;

   cmd.startTestTxCmd.controlField.modulation = 0;
   cmd.startTestTxCmd.offset = PHONET_DEFAULT_TX_TIME_OFFSET;
   cmd.startTestTxCmd.rfBufIndex       = private_data_ptr->current_buffer_mod3;
   private_data_ptr->mdsp_stop_cont_tx_buffer_mod3 = private_data_ptr->current_buffer_mod3;
   cmd.startTestTxCmd.dtmTxBufIndex = private_data_ptr->current_buffer;

   /* Enable the rotator if requested */
   if (public_data_ptr->mdsp_tx_rotator)
   {
     cmd.startTestTxCmd.controlField.useRotator = 1;
   }
   else
   {
     cmd.startTestTxCmd.controlField.useRotator = 0;
   }

   cmd.startTestTxCmd.rfSeqNr = 0;

   mdsp_issue_qdsp6_command ( &cmd, GFW_START_CONTINUOUS_TX_CMD , gas_id );

}
/*===========================================================================

FUNCTION mdsp_start_tx_data_cont

DESCRIPTION
  This function tells the mDSP to transmit user defined data.
  Explanation:
  This function is for KV calibration in FTM mode by sending
  training data to reduce calibration time. It is similar to
  dynamic KV  calibration in online mode.  Instead of having
  G FW hard coding the training data as in online mode,  FTM
  code will pass training data  (user define)  to  FW during
  calibration.  Also,  we  don’t want G  FW to hard code the
  training  data in  continuous  TX.  This avoids disrupting
  RF calibration due to continuous TX being used in multiple
  target calibration.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_start_tx_data_cont ( uint16 *buffer, uint16 buf_len,sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_start_tx_data_cont ( buffer, buf_len, gas_id  );
}


/*===========================================================================

FUNCTION mdsp_start_tx_tone_cont

DESCRIPTION
  This function tells the mDSP to transmit a tone every frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_start_tx_tone_cont (gas_id_t gas_id )
{
   GfwCmdBufType      cmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.startTestTxCmd.cmd = GFW_START_CONTINUOUS_TX_CMD;
   cmd.startTestTxCmd.controlField.txDataModeType = GFW_TX_DATA_TONE;

   cmd.startTestTxCmd.controlField.modulation = 0;
   cmd.startTestTxCmd.offset = PHONET_DEFAULT_TX_TIME_OFFSET;

   cmd.startTestTxCmd.rfBufIndex       = private_data_ptr->current_buffer_mod3;
   private_data_ptr->mdsp_stop_cont_tx_buffer_mod3 = private_data_ptr->current_buffer_mod3;
   cmd.startTestTxCmd.dtmTxBufIndex = 0;

   /* Enable the rotator if requested */
   if (public_data_ptr->mdsp_tx_rotator)
   {
     cmd.startTestTxCmd.controlField.useRotator = 1;
   }
   else
   {
     cmd.startTestTxCmd.controlField.useRotator = 0;
   }

   cmd.startTestTxCmd.rfSeqNr = 0;

   mdsp_issue_qdsp6_command ( &cmd , GFW_START_CONTINUOUS_TX_CMD , gas_id );

}

/*===========================================================================

FUNCTION mdsp_ftm_start_tx_tone_cont

DESCRIPTION
  This function tells the mDSP to transmit a tone every frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_start_tx_tone_cont ( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_start_tx_tone_cont ( gas_id );
}


/*===========================================================================

FUNCTION mdsp_stop_tx_tone_cont

DESCRIPTION
  This function tells the mDSP to stop transmit a tone every frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_stop_tx_tone_cont (gas_id_t gas_id )
{
    GfwCmdBufType      cmd;

    mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

    memset(&cmd,0,sizeof(GfwCmdBufType));
    cmd.stopTestTxCmd.cmd = GFW_STOP_CONTINUOUS_TX_CMD;
    cmd.stopTestTxCmd.offset = PHONET_DEFAULT_TX_TIME_OFFSET;
    cmd.stopTestTxCmd.rfBufIndex  = private_data_ptr->mdsp_stop_cont_tx_buffer_mod3;
    mdsp_issue_qdsp6_command ( &cmd, GFW_STOP_CONTINUOUS_TX_CMD , gas_id );

}

/*===========================================================================

FUNCTION mdsp_ftm_stop_tx_tone_cont

DESCRIPTION
  This function tells the mDSP to stop transmit a tone every frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_stop_tx_tone_cont ( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_stop_tx_tone_cont ( gas_id );
}
/*===========================================================================

FUNCTION mdsp_peek_one_port

DESCRIPTION
  This is the equivalent of an inport() command via the DSP.  Note that this
  function performs a single read.  For now, it can only be called once every
  frame tick.

  port - the port to read from.

DEPENDENCIES
  None

RETURN VALUE
  value read from the port

SIDE EFFECTS
  None
===========================================================================*/
//tjw TBD not called
uint32 mdsp_peek_one_port ( uint16 port, uint16 waitms , gas_id_t gas_id )
{
   MSG_GERAN_ERROR_0_G("GFW_PEEK_CMD not supported");
   return (0);
}

/****************************************************************************
*****************************************************************************

                   SBI/NON-SBI BUFFER FUNCTIONS

*****************************************************************************
*****************************************************************************/


/*===========================================================================

FUNCTION mdsp_burst_current_buffer

DESCRIPTION
  This function retrieves the current buffer index for the given burst
  type.

===========================================================================*/
uint8 mdsp_burst_current_buffer (mdsp_burst_type burst , gas_id_t gas_id)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   switch (burst)
   {
      /*  There are two buffers for ping pong. */
      case RX_BURST:
      case MON_BURST_3:
      case MON_BURST_7:
      case CM_MON_BURST_6:
      case CM_MON_BURST_7:
      case CM_MON_BURST_8:
#if defined  FEATURE_LTE
      case CM_MON_BURST_10:
#endif

      case TX_BURST:
      case MON_BURST_1:
      case MON_BURST_2:
      case STOP_ACQ_BURST:
         if (private_data_ptr->current_buffer_mod3 < 3) /* Static analysis tools are not convinced this is not always so */
         {
           return private_data_ptr->current_buffer_mod3;
         }
         /* else fall through to error return */

      /* Single buffer */
      case CM_CLEANUP_BURST:
         return 0;

#ifdef FEATURE_GSM_MDSP_GTOW
      case WCDMA_RF_ON_BURST:
      case WCDMA_RF_OFF_BURST:
         return 0;
#endif  /* FEATURE_GSM_MDSP_GTOW */


      default:
         MSG_GERAN_ERROR_1_G("Invalid mdsp_burst_type %d", burst);
         return 0;
   }
}

/*===========================================================================

FUNCTION mdsp_clear_nonsbi_static_buffer

DESCRIPTION
  Clears the static nonsbi buffer indicated by the argument by setting the
  transaction count for the buffer to 0.

===========================================================================*/
void mdsp_clear_nonsbi_static_buffer ( uint8 buffer , gas_id_t gas_id )
{
   /* Not supported on Nikel */
}


/*===========================================================================

FUNCTION mdsp_build_nonsbi_static_buffer

DESCRIPTION
  Adds a non-sbi transaction a non-sbi static buffer.   The transactions
  will be processed each time the static buffer is referenced by a dynamic
  buffer.  A reference to the static buffer is placed in a dynamic buffer
  by calling mdsp_use_nonsbi_static_buffer.

  reg  - register to write to
  data - data to write - 32 bits
  buffer - which static buffer to put the transaction into

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_build_nonsbi_static_buffer
(
   mdsp_nonsbi_ctl_unit **script_ptr,
   uint8 buffer
   , gas_id_t gas_id
)
{
   MSG_GERAN_ERROR_0_G("Static nonSBI buffer not supported");
}

/*===========================================================================

FUNCTION mdsp_use_nonsbi_static_buffer

DESCRIPTION
  Places a reference to a static buffer in the dynamic buffer for a burst.
  The non_sbi transactions processed for the burst will be the union of
  the transactions in the static buffer and the transactions in dynamic
  buffer.

  buffer - the static buffer to use
  burst - the burst type this static buffer should be used for

DEPENDENCIES
  Transactions should have been added to the static buffer by calling
  mdsp_build_nonsbi_static_buffer.

===========================================================================*/
void mdsp_use_nonsbi_static_buffer( uint8 buffer, mdsp_burst_type burst , gas_id_t gas_id )
{
   MSG_GERAN_ERROR_0_G("Static nonSBI buffer not supported");
}

/****************************************************************************
*****************************************************************************

                   INIT FUNCTIONS

*****************************************************************************
*****************************************************************************/

/*===========================================================================

FUNCTION mdsp_clear_cmd_len

DESCRIPTION
  This function set the command length for all commands to zero.

===========================================================================*/
void mdsp_clear_cmd_len (void)
{
}



/*===========================================================================

FUNCTION mdsp_set_cmd_len

DESCRIPTION
  This function sets the command length for the given command.

===========================================================================*/
void mdsp_set_cmd_len (uint16 cmd, uint16 len)
{
}


/*===========================================================================

FUNCTION mdsp_ftsm()

DESCRIPTION
  This function gets the mDSP frame-tick setup margin.

===========================================================================*/
uint16 mdsp_ftsm (gas_id_t gas_id)
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   return private_data_ptr->frame_tick_setup_margin;
}

/*===========================================================================

FUNCTION mdsp_get_ftsm()

DESCRIPTION
  This function gets the mDSP frame-tick setup margin for external APIs

===========================================================================*/
uint16 mdsp_get_ftsm (gas_id_t gas_id)
{
  return mdsp_ftsm(gas_id);
}

/*===========================================================================

FUNCTION mdsp_cm_set_ftsm()

DESCRIPTION
  Wrapper function sets the mDSP frame-tick setup margin.

===========================================================================*/

void mdsp_cm_set_ftsm (uint16 ftsm, sys_modem_as_id_e_type as_id)
{
    gas_id_t       gas_id =  geran_map_nas_id_to_gas_id(as_id); 
    mdsp_set_ftsm_geran(ftsm, gas_id);

}


/*===========================================================================

FUNCTION mdsp_set_ftsm()

DESCRIPTION
  This function sets the mDSP frame-tick setup margin.

===========================================================================*/
/* 569801 Deprecated api to be removed after ML1 changes */
void mdsp_set_ftsm(uint16 ftsm
#ifdef FEATURE_DUAL_SIM
                   , gas_id_t gas_id
#endif /*FEATURE_DUAL_SIM */                   
)
{
#ifndef FEATURE_DUAL_SIM
   gas_id_t gas_id =  GERAN_ACCESS_STRATUM_ID_1;
#endif /*FEATURE_DUAL_SIM */ 
   mdsp_set_ftsm_geran(ftsm, gas_id);
}

/*===========================================================================

FUNCTION mdsp_set_ftsm_geran()

DESCRIPTION
  This function sets the mDSP frame-tick setup margin.

===========================================================================*/
void mdsp_set_ftsm_geran (uint16 ftsm , gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   private_data_ptr->frame_tick_setup_margin = ftsm;
}



/*===========================================================================

FUNCTION mdsp_load_nv

DESCRIPTION
   This function loads any NV items required by the mDSP driver.  For now
   there is only one.

===========================================================================*/
void mdsp_load_nv
(
   rex_tcb_type *task_ptr,
   void (*task_wait_handler)(rex_sigs_type,gas_id_t),
   rex_sigs_type task_wait_sig
   , gas_id_t gas_id
)
{
   static boolean nv_read = FALSE; /* static_dual_spaced_ignore */
   nv_cmd_type    mdsp_nv_cmd;
   MSG_GERAN_LOW_0_G("mdsp_load_nv");

   if (nv_read) return;

   /* Prepare the NV read command buffer. */
   mdsp_nv_cmd.item       = NV_MDSP_MEM_DUMP_ENABLED_I;
   mdsp_nv_cmd.cmd        = NV_READ_F;
   mdsp_nv_cmd.data_ptr   = &mdsp_debug_nv_mem_dump_enabled(gas_id)->item;
   mdsp_nv_cmd.tcb_ptr    = task_ptr;
   mdsp_nv_cmd.sigs       = task_wait_sig;
   mdsp_nv_cmd.done_q_ptr = NULL;

   /* Read NV */
   nv_cmd(&mdsp_nv_cmd);
   task_wait_handler(task_wait_sig,gas_id);
   (void) rex_clr_sigs(task_ptr, task_wait_sig);

   /* Store the status */
   mdsp_debug_nv_mem_dump_enabled(gas_id)->status = mdsp_nv_cmd.status;

   nv_read = TRUE;
}

/*===========================================================================

FUNCTION mdsp_load_efs

DESCRIPTION
   This function loads any EFS items required by the mDSP driver.

===========================================================================*/
void mdsp_load_efs ( gas_id_t gas_id )
{
   mdsp_nv_recovery_restart_type *mdsp_nv_recovery_restart = mdsp_debug_nv_recovery_restart(gas_id);
   MSG_GERAN_LOW_0_G("mdsp_load_efs");

   if(geran_efs_read_primary(GERAN_EFS_COMM_RECOVERY_RESTART,
                &mdsp_nv_recovery_restart->recovery_restart_enabled,
                sizeof(mdsp_nv_recovery_restart->recovery_restart_enabled))
             == sizeof(mdsp_nv_recovery_restart->recovery_restart_enabled))
   {
     mdsp_nv_recovery_restart->valid = TRUE;

     if(mdsp_nv_recovery_restart->recovery_restart_enabled == FALSE)
     {
       MSG_GERAN_HIGH_0_G("GSM RECOVERY RESTART NV is written and recovery restart is not enabled in GERAN ");
     }
     else
     {
       MSG_GERAN_HIGH_0_G("GSM RECOVERY RESTART NV is written and recovery restart is enabled in GERAN ");
     }
   }
   else
   {
     mdsp_nv_recovery_restart->valid = FALSE;
     MSG_GERAN_HIGH_0_G("GSM RECOVERY RESTART NV is not written. So recovery restart is not enabled in GERAN");
   }
   antenna_swth_get_nv_item ();
   antenna_swth_get_nv_item_qsc();
#ifdef FEATURE_GSM_RF_DEBUG_NV
   MSG_GERAN_HIGH_0_G("GSM mdsp_rf_debug_efs_read");
   mdsp_rf_debug_efs_read ();
#endif /* FEATURE_GSM_RF_DEBUG_NV */

   if(geran_efs_read_primary(GERAN_EFS_SRCH_ALL_W_CELL,&nv_for_search_all_w_cell, sizeof(nv_for_search_all_w_cell)) <0)
   {
     nv_for_search_all_w_cell = 0;
   }
   
#if defined(FEATURE_QBTA)
   gl1_read_nv_qbta_support();
#endif
}

/*===========================================================================

FUNCTION mdsp_init_MdspHostIfBaseStore

DESCRIPTION
  This function initializes pMdspHostIfBaseStore pointer.
  Init the pMdspHostIfBaseStore as this store may be read without TRM lock
  before mdsp image is loaded.

DEPENDENCIES


===========================================================================*/
void mdsp_init_MdspHostIfBaseStore (gas_id_t gas_id)
{
  mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

#ifdef TEST_FRAMEWORK
  #error code not present
#else
  public_data_ptr->pMdspHostIfBaseStore=(GfwHostMdspInterfaceSegment*) GFW_SHARED_MEM_START;
#endif

  /* Align correctly based on gas_id */
  public_data_ptr->pMdspHostIfBaseStore +=
    GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
}

/*===========================================================================

FUNCTION mdsp_init

DESCRIPTION
  This function initializes data structures to enable communications with
  the mdsp.

DEPENDENCIES
  mdsp image must already be loaded.

===========================================================================*/
void mdsp_init (gas_id_t gas_id)
{
  uint16  mdsp_branch_ver = 0;
  uint16  mdsp_major_ver = 0;
  uint16  mdsp_minor_ver = 0;

  mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

#ifndef TEST_FRAMEWORK
    public_data_ptr->pMdspHostResultsStore = &(public_data_ptr->pMdspHostIfBaseStore->gfwResultBuffers);
#endif

  if(private_data_ptr->mdsp_gfw_nv_recovery_restart)
  {
    public_data_ptr->pMdspHostIfBaseStore->gfwActionOnCcsStatus.crashOnError  = TRUE;
  }
  else
  {
    public_data_ptr->pMdspHostIfBaseStore->gfwActionOnCcsStatus.crashOnError  = FALSE;
  }

   /* Reset pending nonsbi write pointers */
   private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr = private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr = 0;
   private_data_ptr->mdsp_pending_nonsbi_writes = 0;

   /* Need to rewrite the power control tables */
#ifdef FEATURE_POLAR
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_am_table          = TRUE;
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_am_pm_table          = TRUE;
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_pa_ramp_table_polar  = TRUE;
#else
   private_data_ptr->mdsp_tx_pwr_ctrl_data.new_pa_ramp_table_linear = TRUE;
#endif

   /* Reset command buffer */
   GDRV_ISR_SAVE_LOCK(gas_id);
   mdsp_reset_command_buffers( gas_id );
   GDRV_ISR_SAVE_UNLOCK(gas_id);
   /* Initialize the debug module */
   mdsp_debug_init( gas_id );

   /* gprs functions are in a separate file - perform init */
   mdsp_gprs_init( gas_id );

   /* Init DTM data structures */
   mdsp_dtm_init( gas_id );

   /* Set the frame tick setup margin and sync rx time offset */
   mdsp_set_ftsm_geran(FRAME_TICK_SETUP_MARGIN_QS , gas_id );
   mdsp_set_sync_rx_time_offset(MDSP_DEFAULT_RX_TIME_OFFSET , gas_id );

   /* Init sleep code */
   mdsp_sleep_init( gas_id );

   /* Set the channel filter coefficients */
   /* Needs to be coded for Q6 implementation */

   /* Start out not panicking */
   private_data_ptr->mdsp_panicking = FALSE;


   /* Always clear to avoid any hanging asynch int flag and register int handler */
   mdsp_asynch_pch_int_init( gas_id );

   /* Lock interrupts since this may be called from task context */
   GDRV_ISR_SAVE_LOCK(gas_id);

    /* reset the pending generic config buffer */
    public_data_ptr->pendingGfwGenericCmd.sharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.rxSharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.txSharedVarUpdated = FALSE;
    public_data_ptr->pendingGfwGenericCmd.edgeSharedVarUpdated = FALSE;

   /* Unlock interrupts since this may be called from task context */
   GDRV_ISR_SAVE_UNLOCK(gas_id);

  /* Needs to be coded - alternative to this for Q6  */
   mdsp_branch_ver = public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwVersion.number.branch;
   mdsp_major_ver = public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwVersion.number.major;
   mdsp_minor_ver = public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.gfwVersion.number.minor;

   /* Reduce F3 at wakeup in MSIM */
   MSG_GERAN_LOW_3_G("GSM mDSP ready (v%04x.%04x.%x)", mdsp_branch_ver,mdsp_major_ver, mdsp_minor_ver);
   MSG_GERAN_LOW_1_G( "GFW Interface version : %s", Gfw_Interface_Version );

   private_data_ptr->mdsp_init_complete = TRUE;

}

/*===========================================================================

FUNCTION mdsp_init_completed

DESCRIPTION
  Returns TRUE when the MDSP interface is initialised

DEPENDENCIES

===========================================================================*/
boolean mdsp_init_completed(gas_id_t gas_id)
{
  return get_private_dataspace_ptr( gas_id )->mdsp_init_complete;
}
/*===========================================================================

FUNCTION mdsp_set_init

DESCRIPTION
  sets mdsp_init_complete to TRUE

DEPENDENCIES

===========================================================================*/
void mdsp_set_init(gas_id_t gas_id)
{
  get_private_dataspace_ptr( gas_id )->mdsp_init_complete = TRUE;
  MSG_GERAN_HIGH_1_G("mdsp_init_complete is set to %d",mdsp_init_completed(gas_id));

}
/*===========================================================================
FUNCTION mdsp_disable

DESCRIPTION
  This function is called when the MDSP is disabled

DEPENDENCIES

===========================================================================*/
void mdsp_disable(gas_id_t gas_id )
{
   boolean mdsp_ready_to_disable;
   uint32 fws_status;

   if (mdsp_awake(gas_id))
   {
      mdsp_ready_to_disable = mdsp_ready_for_sleep(gas_id);
   }
   else
   {
      mdsp_ready_to_disable = TRUE;
   }

   if (mdsp_ready_to_disable == FALSE)
   {
      uint16 loop_count = 0;
      while((mdsp_ready_to_disable == FALSE) && (loop_count < MDSP_DISABLE_MAX_DELAY_COUNT))
      {
         loop_count++;
         gl1_hw_delay (MDSP_DISABLE_DELAY_LOOP, gas_id);
         mdsp_ready_to_disable = mdsp_ready_for_sleep(gas_id);
      }
      MSG_GERAN_HIGH_1_G("Wait %dus for FW to be ready for disable", (loop_count * MDSP_DISABLE_DELAY_LOOP));
      if (loop_count == MDSP_DISABLE_MAX_DELAY_COUNT)
      {
         MSG_GERAN_ERROR_0_G("Wait timeout, FW may not be ready for disable");
      }
   }

   /* Disable G firmware (blocking call). */
   fws_status = fws_app_disable( mdsp_get_fws_app_mask( gas_id ) );

   MSG_GERAN_HIGH_2_G("GERAN FWS APP Disable status %d [%d]",
                      fws_status,gl1_get_FN(gas_id));

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   gl1_set_gfw_app_mode_status( GFW_APP_MODE_IDLE, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   mdsp_awake_set(FALSE, gas_id);
   get_private_dataspace_ptr( gas_id )->mdsp_init_complete = FALSE;
   mdsp_sleep_reset_mdsp_intf( gas_id );
}

/*===========================================================================

FUNCTION mdsp_enable

DESCRIPTION
  This function is called when the MDSP is disabled

DEPENDENCIES

===========================================================================*/
void mdsp_enable(gas_id_t gas_id)
{
  /* Enable GFW APP as we are ready to enable GSM */
  uint32 fws_status = fws_app_enable( mdsp_get_fws_app_mask( gas_id ) );

  MSG_GERAN_HIGH_2_G( "GERAN FWS APP Enable status %d [%d]",
                      fws_status, gl1_get_FN(gas_id) );

  mdsp_awake_set(TRUE,gas_id);
}

#ifdef FEATURE_G2X_TUNEAWAY
/*===========================================================================

FUNCTION mdsp_init_G2X

DESCRIPTION
  

DEPENDENCIES

===========================================================================*/
void mdsp_init_G2X(gas_id_t gas_id)
{
      MSG_GERAN_HIGH_1_G( "g2w_tbf_abort_callback_received TRUE setting mdsp_init_complete %d",
                          get_private_dataspace_ptr( gas_id )->mdsp_init_complete);

     /* Enable GFW APP as we are ready to enable GSM */
     get_private_dataspace_ptr( gas_id )->mdsp_init_complete = TRUE;
}
#endif

/*===========================================================================

FUNCTION mdsp_get_fws_app_mask

DESCRIPTION
  This function return the appropriate firmware APP id for the given gas_id

DEPENDENCIES

===========================================================================*/
uint32 mdsp_get_fws_app_mask( gas_id_t gas_id )
{
  uint32 fws_app_mask = 0;

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      fws_app_mask = FW_APP_GERAN;
    break;

    case GERAN_ACCESS_STRATUM_ID_2:
      fws_app_mask = FW_APP_GERAN2;
    break;

#ifdef FEATURE_TRIPLE_SIM
    case GERAN_ACCESS_STRATUM_ID_3:
      fws_app_mask = FW_APP_GERAN3;
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
      MSG_GERAN_FATAL_0_G( "GAS ID is out of range" );
    break;
  }

  return ( fws_app_mask );
}

/*===========================================================================

FUNCTION mdsp_feature_check

DESCRIPTION
  This function checks that the MDSP features match the software build

DEPENDENCIES

===========================================================================*/
void mdsp_feature_check (gas_id_t gas_id )
{
   mdsp_enable_saic(TRUE , gas_id );

#ifdef FEATURE_GSM_MDSP_ESAIC
   mdsp_enable_esaic(TRUE , gas_id );
#endif

  mdsp_enable_aeq(TRUE , gas_id );

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
  mdsp_enable_aeq_8psk(TRUE , gas_id );
#ifdef FEATURE_GSM_DYNAMIC_AEQ_8PSK
  mdsp_enable_aeq_8psk_dynamic(TRUE , gas_id );
#endif /* FEATURE_GSM_DYNAMIC_AEQ_8PSK */
#endif

#ifdef FEATURE_GSM_MDSP_AEQ_SCH
  mdsp_enable_aeq_sch(TRUE , gas_id );
#endif

}



/*===========================================================================

FUNCTION mdsp_inact

DESCRIPTION
  This function cleans up and shuts-down the mDSP.

===========================================================================*/
void mdsp_inact (gas_id_t gas_id )
{
   /* Always clear to avoid any hanging asynch int flag and deregister int handler */
   mdsp_asynch_pch_int_inact( gas_id );

   /* Inactivate the sleep module */
   mdsp_sleep_inact();

   /* Inactive the GPRS module */
   mdsp_gprs_inact( gas_id );
}



/*===========================================================================

FUNCTION mdsp_set_loopback

DESCRIPTION
  This function is used to put the mdsp into various loopback modes, looping
  data it receives back onto the TX chain.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_set_loopback ( mdsp_loopback_type type , gas_id_t gas_id )
{
   GfwCmdBufType             cmd;

   memset(&cmd,0,sizeof(GfwCmdBufType));
   cmd.loopBackCmd.cmd = GFW_LOOPBACK_CMD;
   cmd.loopBackCmd.type = (uint16)type;

   mdsp_issue_qdsp6_command( &cmd, GFW_LOOPBACK_CMD , gas_id );

}

/*===========================================================================

FUNCTION mdsp_get_mdsp_frame_tick_count

DESCRIPTION
  This function returns the current value for mdsp_frame_tick_count.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint32 mdsp_get_mdsp_frame_tick_count(gas_id_t gas_id )
{
   mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

   return(public_data_ptr->mdsp_frame_tick_count);
}

/*===========================================================================

FUNCTION mdsp_wait_for_frame_tick_non_blocking

DESCRIPTION
  This function will not return until just after the next frame tick.
  It polls mdsp_frame_tick_count in a non-blocking fashion, meaning it will
  let other tasks run while it is waiting for the frame tick to happen.

DEPENDENCIES
  mdsp_switch_command_buffers() must be installed as a GSTMR interrupt
  handler or this function will not return at all.

PARAMETERS
  rex_tcb_type   *tcb_ptr  - Pointer to current TCB.
  rex_sigs_type   wait_sig - Signal set when wait timer expires.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_wait_for_frame_tick_non_blocking( rex_tcb_type  *tcb_ptr,
                                            rex_sigs_type  wait_sig
                                            , gas_id_t gas_id )
{
   volatile uint32 count;
   uint16 poll_count = 0;
   boolean frame_tick_running;
   rex_timer_type local_timer;
#ifdef FEATURE_GERAN_VP
  /* Timers not handled in Virtual platform so just return */
  return (TRUE);
#else
   /* Should be OK to call this each time this function is called.  Doesn't do
   ** much and will make sure tcb_ptr and wait_sig are set properly .*/
   rex_def_timer(&local_timer,tcb_ptr,wait_sig);

   /* Store the current frame tick number */
   count = mdsp_get_mdsp_frame_tick_count( gas_id );

   if ( rex_is_in_irq_mode() )
   {
      /*----------------------------------------------------------------------
         Since we are in interrupt context, counter will not increment
       ----------------------------------------------------------------------*/
      MSG_GERAN_FATAL_0_G( "Called from ISR context" );
   }

   (void)rex_clr_sigs( tcb_ptr, wait_sig );

   /* This loop waits for "mdsp_frame_tick_count" to increment.  It blocks and
   ** releases the CPU after each check instead of doing a busy wait.  That way
   ** other tasks are allowed to run while we wait. */
   while ((count == mdsp_get_mdsp_frame_tick_count( gas_id )) &&
          (poll_count < MDSP_FRAME_TICK_POLL_COUNT_MAX))
   {
      (void)rex_set_timer( &local_timer, MDSP_FRAME_TICK_POLL_TIMEOUT_MSEC );

      (void)rex_wait (wait_sig);

      (void)rex_clr_sigs( tcb_ptr, wait_sig );

      poll_count++;
   }

   /* Check to see if we waited too long for a frame tick to happen. */
   if (poll_count >= MDSP_FRAME_TICK_POLL_COUNT_MAX)
   {
      MSG_GERAN_HIGH_3_G("mdsp_frame_tick_count:%d stalled! irq_mode:%d ints_locked:%d",
                mdsp_get_mdsp_frame_tick_count( gas_id ), rex_is_in_irq_mode(), rex_ints_are_locked());
      frame_tick_running = FALSE;
   }
   else
   {
      MSG_GERAN_HIGH_2_G("Waiting for frame tick... frame:%d poll:%d",
               mdsp_get_mdsp_frame_tick_count( gas_id ), poll_count);
      frame_tick_running = TRUE;

   }

    rex_undef_timer(&local_timer);

   return (frame_tick_running);
#endif /* FEATURE_GERAN_VP */
}

/*===========================================================================

FUNCTION mdsp_wait_for_frame_tick

DESCRIPTION
  This function will not return until just after the next frame tick.

DEPENDENCIES
  mdsp_switch_command_buffers() must be installed as a GSTMR interrupt
  handler or this function will not return at all.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_wait_for_frame_tick(gas_id_t gas_id )
{
   uint32 count;

   /* Store the current frame tick number */
   count = mdsp_get_mdsp_frame_tick_count( gas_id );

   if ( rex_is_in_irq_mode() )
   {
      /*----------------------------------------------------------------------
         Since we are in interrupt context, counter will not increment
       ----------------------------------------------------------------------*/
      MSG_GERAN_FATAL_0_G( "Called from ISR context" );
   }


   MSG_GERAN_HIGH_0_G("Waiting for frame tick...");

   /* When the next frame tick occurs the number will be incremented.
    * Wait for that to happen */
   while (count == mdsp_get_mdsp_frame_tick_count( gas_id ))
     ;
}

/*===========================================================================

FUNCTION mdsp_ftm_wait_for_frame_tick

DESCRIPTION
  This function will not return until just after the next frame tick.

DEPENDENCIES
  mdsp_switch_command_buffers() must be installed as a GSTMR interrupt
  handler or this function will not return at all.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_ftm_wait_for_frame_tick (sys_modem_as_id_e_type as_id)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_wait_for_frame_tick( gas_id );
}

/*===========================================================================

FUNCTION mdsp_gsm_register_panic_cb

DESCRIPTION
  Function called by NPL1 to register a callback function called whenever a
  mDSP halt is received. The callback handler should be able propagate to
  upper layers the error message. This function should be called during
  initialization of the mdsp.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_gsm_register_panic_cb(mdsp_gsm_panic_cb_type cb , gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   private_data_ptr->mdsp_panic_cb = cb;
}

/*===========================================================================

FUNCTION mdsp_handle_panic

DESCRIPTION
  This function calls the NPL1 callback handler registered in an event of
  mdsp panic (mdsp halts).

DEPENDENCIES
  mdsp_gsm_register_panic_cb() should be called ahead of time.

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_handle_panic (gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* If we are already panicking, don't do it again */
   if (private_data_ptr->mdsp_panicking) return;
   private_data_ptr->mdsp_panicking = TRUE;

   if (private_data_ptr->mdsp_panic_cb != NULL)
   {
      private_data_ptr->mdsp_panic_cb(gas_id);
   }
}

/****************************************************************************
*****************************************************************************

                   GTOW FUNCTIONS

*****************************************************************************
*****************************************************************************/


#ifdef FEATURE_GSM_MDSP_GTOW
/*===========================================================================

FUNCTION mdsp_start_wcdma_rf_cmd

DESCRIPTION
  This function sets up and issues the start_wcdma_rf_cmd command.

===========================================================================*/
void mdsp_start_wcdma_rf_cmd( uint16 start_time, boolean  rf_tune2x, uint32 rxlm_irat_buf_idx , gas_id_t gas_id )
{
  mdsp_g2x_startup_cmd( start_time, rf_tune2x, rxlm_irat_buf_idx , FALSE, gas_id  );
}

/*===========================================================================

FUNCTION mdsp_prepare_wcdma_buffers

DESCRIPTION
  This checks clears the number of transactions and semaphore fields
  of the buffers used for RF on and RF off in WCDMA interRAT searches.
  This also checks the semaphore bit to see if the previous transactions
  were successful.

===========================================================================*/
void mdsp_prepare_wcdma_buffers(gas_id_t gas_id )
{
/* needs coding for Q6 */
}



/*===========================================================================

FUNCTION mdsp_rf_sample_ram_cmd

DESCRIPTION
  This function sets up and issues the rf_sample_ram_cmd command.

===========================================================================*/
void mdsp_rf_sample_ram_cmd (uint16 start_time, uint16 stop_time, boolean  rf_tune2x, uint32 rxlm_buf_idx, uint32 rxlm_irat_buf_idx , gas_id_t gas_id )
{
  mdsp_g2x_startup_cmd( start_time, rf_tune2x, rxlm_irat_buf_idx , FALSE, gas_id  );

  mdsp_g2x_cleanup_cmd( stop_time, rxlm_buf_idx, TRUE , FALSE, gas_id );
}

void  mdsp_g2x_startup_cmd( uint16   time_offset, boolean  rf_tune2x, uint32   rxlm_irat_buf_idx, boolean  immediate, gas_id_t gas_id )
{
  GfwCmdBufType  cmd;

  cmd.g2xStartCmd.cmd         = GFW_G2X_STARTUP_CMD;
  cmd.g2xStartCmd.time_offset = mdsp_ftsm( gas_id ) + time_offset;
  cmd.g2xStartCmd.rfTune2x    = rf_tune2x;

   cmd.g2xStartCmd.rfBufStartIndex = 0;
   cmd.g2xStartCmd.rxlmBufIndex = rxlm_irat_buf_idx;
   cmd.g2xStartCmd.doImmediateStartup = immediate;

  mdsp_issue_qdsp6_command ( &cmd, GFW_G2X_STARTUP_CMD , gas_id );
}

void  mdsp_g2x_cleanup_cmd( uint16  time_offset, uint32  rxlm_buf_idx,   boolean tuneback_to_gsm,   boolean immediate,  gas_id_t gas_id )
{
  GfwCmdBufType  cmd;

  cmd.g2xCleanupCmd.cmd         = GFW_G2X_CLEANUP_CMD;
  cmd.g2xCleanupCmd.time_offset = time_offset + mdsp_ftsm( gas_id );

  /* g2x cleanup uses buffer 1 */
   cmd.g2xStartCmd.rfBufStartIndex = G2X_CLEANUP_BURST_IDX;
   cmd.g2xCleanupCmd.rxlmBufIndex = rxlm_buf_idx;
   cmd.g2xCleanupCmd.rfTuneback = tuneback_to_gsm;
   cmd.g2xCleanupCmd.doImmediateCleanup = immediate;
   if(gl1_is_l1_state_ptm(gas_id) || gl1_is_l1_state_dtm(gas_id))
   {
     cmd.g2xCleanupCmd.sendReply = TRUE;
   }
   else
   {
     cmd.g2xCleanupCmd.sendReply = FALSE;
   }

  mdsp_issue_qdsp6_command ( &cmd, GFW_G2X_CLEANUP_CMD , gas_id );
}

#endif  /* FEATURE_GSM_MDSP_GTOW */

/*===========================================================================

FUNCTION mdsp_copy_xmsi_data

DESCRIPTION
  This function.

===========================================================================*/
void mdsp_copy_xmsi_data (boolean imsi_valid,  uint8 *imsi_ptr,
                          boolean tmsi_valid,  uint8 *tmsi_ptr,
                          boolean ptmsi_valid, uint8 *ptmsi_ptr
                          , gas_id_t gas_id )
{
    uint16 imsi[5] = {0};

    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

    if(tmsi_valid == TRUE)
    {
        uint16 tmsi[2];
        tmsi[0] = (tmsi_ptr[0]<<8)|tmsi_ptr[1];
        tmsi[1] = (tmsi_ptr[2]<<8)|tmsi_ptr[3];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdTmsi[0]= tmsi[0];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdTmsi[1]= tmsi[1];
        mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id );

        MSG_GERAN_LOW_2_G("SBD:write tmsi to mDSP [0]%x [1]%x",tmsi[0],tmsi[1]);
    }
    if(ptmsi_valid == TRUE)
    {
        uint16 ptmsi[2];
        ptmsi[0] = (ptmsi_ptr[0]<<8)|ptmsi_ptr[1];
        ptmsi[1] = (ptmsi_ptr[2]<<8)|ptmsi_ptr[3];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdPtmsi[0]= ptmsi[0];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdPtmsi[1]= ptmsi[1];
        mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id );

        MSG_GERAN_LOW_2_G("SBD:write ptmsi to mDSP [0]%x [1]%x",ptmsi[0],ptmsi[1]);
    }
    if(imsi_valid == TRUE)
    {
        /*#define GSDI_IMSI_LEN 9*/
        imsi[0] = ( (imsi_ptr[1]<<8)|(imsi_ptr[0]) );
        imsi[1] = ( (imsi_ptr[3]<<8)|(imsi_ptr[2]) );
        imsi[2] = ( (imsi_ptr[5]<<8)|(imsi_ptr[4]) );
        imsi[3] = ( (imsi_ptr[7]<<8)|(imsi_ptr[6]) );
        imsi[4] = (        (0x00<<8)|(imsi_ptr[8]) );
    }

    if( (tmsi_valid==TRUE) || (ptmsi_valid==TRUE) || (imsi_valid==TRUE) )
    {
        imsi[4] |= ( (imsi_valid<<15) | (tmsi_valid<<14) | (ptmsi_valid<<13) );
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdImsi[0]= imsi[0];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdImsi[1]= imsi[1];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdImsi[2]= imsi[2];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdImsi[3]= imsi[3];
        public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.edge.sbdImsi[4]= imsi[4];
        mdsp_updated_shared_variable(GFW_EDGE_SHARED_VARIABLE , gas_id );

        MSG_GERAN_LOW_3_G("SBD:write imsi to mDSP [0]%x [1]%x [4]%x",imsi[0],imsi[1],imsi[4]);
    }

}

/*===========================================================================

FUNCTION mdsp_sbd_read_pattern_used

DESCRIPTION
  This function.

===========================================================================*/
uint16 mdsp_sbd_read_pattern_used (gas_id_t gas_id )
{
    uint16 val;
    mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

    val = public_data_ptr->pMdspHostIfBaseStore->gfwReadVariablesStruct.nullPagePatternInUse;
    return val;
}

/*===========================================================================

FUNCTION mdsp_reset_pending_nonsbi_writes

DESCRIPTION
  This function resets any pending non SBI writes

===========================================================================*/
void mdsp_reset_pending_nonsbi_writes (gas_id_t gas_id )
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  private_data_ptr->mdsp_pending_nonsbi_write_rd_ptr = private_data_ptr->mdsp_pending_nonsbi_write_wr_ptr = 0;
  private_data_ptr->mdsp_pending_nonsbi_writes = 0;
}

/*===========================================================================

FUNCTION mdsp_asynch_pch_int_init

DESCRIPTION
  Registers the mdsp driver asynch int isr handler for init.

===========================================================================*/
void mdsp_asynch_pch_int_init( gas_id_t gas_id )
{
  if ( gl1_asynch_int_dbg( gas_id ) )
  {
    MSG_GERAN_LOW_0_G( "MDSP asynch pch int init" );
  }

  mdsp_set_asynch_int_enabled( FALSE, gas_id );
}

/*===========================================================================

FUNCTION mdsp_asynch_pch_int_inact

DESCRIPTION
  Clears the mdsp driver asynch int isr handler for inact.

===========================================================================*/
void mdsp_asynch_pch_int_inact( gas_id_t gas_id )
{
  if ( gl1_asynch_int_dbg( gas_id ) )
  {
    MSG_GERAN_LOW_0_G( "MDSP asynch pch int inact" );
  }

  mdsp_set_asynch_int_enabled( FALSE, gas_id );
}

/*===========================================================================

FUNCTION mdsp_set_asynch_int_enabled

DESCRIPTION
  Sets/clears the mdsp driver flag if the asynch int is enabled/disabled.

===========================================================================*/
void mdsp_set_asynch_int_enabled( boolean set_asynch_int , gas_id_t gas_id )
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  private_data_ptr->mdsp_is_asynch_int_enabled = set_asynch_int;
}

/*===========================================================================

FUNCTION mdsp_asynch_dsp_int_enabled

DESCRIPTION
  Tells the mdsp driver if the asynch int is enabled/disabled.

===========================================================================*/
boolean mdsp_asynch_dsp_int_enabled(gas_id_t gas_id )
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  return ( private_data_ptr->mdsp_is_asynch_int_enabled );
}

/*===========================================================================

FUNCTION mdsp_sleep_reset_mdsp_intf

DESCRIPTION
  This function initializes data structures to enable communications with
  the mdsp.

DEPENDENCIES


===========================================================================*/
void mdsp_sleep_reset_mdsp_intf( gas_id_t gas_id )
{
   uint8 i;

   mdsp_intf_private_data_t *private_data_ptr = get_private_dataspace_ptr( gas_id );

   uint8 saved_current_buffer      = private_data_ptr->current_buffer;
   uint8 saved_current_buffer_mod3 = private_data_ptr->current_buffer_mod3;

   /* Clear command buffers - double buffered */
   for ( i = 0; i < PINGPONG; i++ )
   {
     /* Buffer index increment for double buffered elements */
     private_data_ptr->current_buffer = i;

     mdsp_clear_cmd_buff( gas_id );
   }

   private_data_ptr->mdsp_cmd_count = 0;

   /*
    * Need to clear as any pending writes will be invalid and may be for
    * other G sub
    */
   mdsp_reset_pending_nonsbi_writes( gas_id );

   /* restore buffer indices to preserve buffer sequence history */
   private_data_ptr->current_buffer      = saved_current_buffer;
   private_data_ptr->current_buffer_mod3 = saved_current_buffer_mod3;
}

/*===========================================================================

FUNCTION mdsp_clear_cmd_buff

DESCRIPTION
  This function clears any pending command buffer entries when we execute the
  async pch in G+G DSDS mode.

DEPENDENCIES
  mdsp image must already be loaded.

===========================================================================*/
void mdsp_clear_cmd_buff(gas_id_t gas_id )
{

   /* Check nonsbi request buffer */
   uint16 num_cmd;
   GfwHostCmdBufType      *hostCmd;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   /* Clear any pending cmd buffer data */
   hostCmd = (GfwHostCmdBufType *)public_data_ptr->pMdspHostIfBaseStore->gfwHostCmdBuffer[private_data_ptr->current_buffer];
   num_cmd = hostCmd->semaphoreAndNumCommands;

   if (num_cmd & 0x7FFF)
   {
     MSG_GERAN_ERROR_3_G(" num cmd %d index %d seq num %d",
            num_cmd,private_data_ptr->current_buffer,hostCmd->cmdSeqNum);
   }
   hostCmd->semaphoreAndNumCommands = 0;
}

/*===========================================================================

FUNCTION mdsp_afc_update_cmd

DESCRIPTION
  This function sends the command to GFW to configure DCO (for gstmr clock)

===========================================================================*/
void mdsp_afc_update_cmd( int32 f_hz_q6 ,uint32 inv_f_dl_lo , gas_id_t gas_id )
{
  GfwCmdBufType   cmd;

  cmd.afcUpdateCmd.cmd  = GFW_AFC_UPDATE_CMD;
  cmd.afcUpdateCmd.inv_f_dl_lo = inv_f_dl_lo;
  cmd.afcUpdateCmd.f_hz_q6 = f_hz_q6;
  mdsp_issue_qdsp6_command ( &cmd , GFW_AFC_UPDATE_CMD , gas_id );

}

/*===========================================================================

FUNCTION mdsp_gsm_config_tx_channel_id

DESCRIPTION
  This function sends the command to GFW to configure the static Tx registers
  Channel id is passed as a parameter

===========================================================================*/
void mdsp_gsm_config_tx_channel_id(uint32 buf_idx, uint8 tx_channel_id, 
                                   gas_id_t gas_id)
{
  GfwCmdBufType   cmd;
  cmd.configureTxCmd.cmd  = GFW_CONFIGURE_TX;
  cmd.configureTxCmd.txlmBufIndex = buf_idx;
  cmd.configureTxCmd.channel_id0 = tx_channel_id;

  mdsp_issue_qdsp6_command ( &cmd , GFW_CONFIGURE_TX , gas_id );

  mdsp_set_mdsp_gsm_txlm_buf_idx(buf_idx , gas_id );

  MSG_GERAN_MED_2_G("Tx Config issued to FW, txlm %d, TX chain id %d", 
                    buf_idx, tx_channel_id);
}

boolean mdsp_commands_pending(gas_id_t gas_id )
{
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   if(private_data_ptr->mdsp_cmd_count )
   {
     return  TRUE;
   }
   else
   {
     return  FALSE;
   }
}


/*===========================================================================

FUNCTION mdsp_ftm_program_cal_amam_table

DESCRIPTION
  This function copies AM/PM tables into GFW shared interface during
  pre-distortion Calibration

===========================================================================*/
void mdsp_ftm_program_cal_amam_table(uint16* am_am_tbl, uint16* am_pm_tbl , sys_modem_as_id_e_type as_id )
{
  MSG_HIGH( "Function cal_amam_table not supported",0,0,0 ); 
}

/*===========================================================================

FUNCTION mdsp_set_tx_path_delay

DESCRIPTION
  This function set the dynamic tx path delay

===========================================================================*/
void mdsp_set_tx_path_delay (int16 delay_val , gas_id_t gas_id )
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  private_data_ptr->mdsp_tx_pwr_ctrl_data.tx_path_delay_val = delay_val;
  private_data_ptr->mdsp_tx_pwr_ctrl_data.tx_path_delay_updated = TRUE;
}
/*===========================================================================

FUNCTION mdsp_set_tx_smps_pdm_value

DESCRIPTION
  This function copies AM/PM tables into GFW shared interface during
  pre-distortion Calibration

===========================================================================*/
void mdsp_set_tx_smps_pdm_value (int16 pdm_value , gas_id_t gas_id )
{
  mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

  private_data_ptr->mdsp_tx_pwr_ctrl_data.tx_smps_pdm_value = pdm_value;
  private_data_ptr->mdsp_tx_pwr_ctrl_data.tx_smps_pdm_value_updated = TRUE;
}

/*===========================================================================

FUNCTION mdsp_xo_desense_arfcn

DESCRIPTION
  This function called by rf to update arfcn list rquired Dc spur removal

===========================================================================*/
void mdsp_xo_desense_arfcn_gas_id (mdsp_xo_desense_arfcn_struct *pointer , gas_id_t gas_id )  /*GERAN use (gl1_hw.c) */
{
  int i;
  mdsp_intf_public_data_t* public_data_ptr;

  public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

  for ( i = 0; i < MAX_NMBR_DESENSE_CHNL_PER_BAND ; i++)
  {
      public_data_ptr->desense_chanel.G850_chan_list[i]  = pointer-> G850_chan_list[i];
      public_data_ptr->desense_chanel.G900_chan_list[i]  = pointer-> G900_chan_list[i];
      public_data_ptr->desense_chanel.G1800_chan_list[i] = pointer-> G1800_chan_list[i];
      public_data_ptr->desense_chanel.G1900_chan_list[i] = pointer-> G1900_chan_list[i];
  }
}

/*===========================================================================

FUNCTION mdsp_rf_xo_desense_arfcn

DESCRIPTION
  This function called by rf to update arfcn list rquired Dc spur removal

===========================================================================*/

void mdsp_xo_desense_arfcn (mdsp_xo_desense_arfcn_struct *pointer, sys_modem_as_id_e_type as_id) /*API use*/
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_xo_desense_arfcn_gas_id(pointer, gas_id);
}

/*===========================================================================

FUNCTION mdsp_rf_xo_desense_arfcn

DESCRIPTION
  This function called by rf to update arfcn list rquired Dc spur removal

===========================================================================*/
void mdsp_rf_xo_desense_arfcn (mdsp_xo_desense_arfcn_struct *pointer , sys_modem_as_id_e_type as_id )
{
  mdsp_xo_desense_arfcn(pointer, as_id);
}
/*===========================================================================

FUNCTION mdsp_is_desense_arfcn

DESCRIPTION
  This function retuns true if the input arfcn rquired Dc spur removal

============================================================================*/
mdsp_rf_desense_chan_type mdsp_is_desense_arfcn( uint16 chanel_num , sys_band_T  band ,gas_id_t gas_id )
{
  int i;
  mdsp_rf_desense_chan_type chan_desense = NOT_DESENSE_CHAN;

  mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );
#ifdef FEATURE_GERAN_DRDSDS
  if (gl1_drdsds_enabled (gas_id) )
  {
   return DESENSE_CHAN;
  }
#endif /* FEATURE_GERAN_DRDSDS*/  
  switch (band) 
   {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
      if (124 == chanel_num)
      {
        chan_desense = DESENSE_CHAN_MINUS_1;
      }
      else
      {
         for ( i = 0; i < MAX_NMBR_DESENSE_CHNL_PER_BAND ; i++)
         {
            if ( public_data_ptr->desense_chanel.G900_chan_list[i] == (LAST_ELEMNT_IN_CHNL_LIST) )
            {
                 break;
            }
            else if ( public_data_ptr->desense_chanel.G900_chan_list[i] == chanel_num )
            {
                  chan_desense = DESENSE_CHAN;
                  break;
            }
            else if ( public_data_ptr->desense_chanel.G900_chan_list[i] == (chanel_num + 1) )
            {
                  chan_desense = DESENSE_CHAN_MINUS_1;
                  break;
            }
            else if ( public_data_ptr->desense_chanel.G900_chan_list[i] == (chanel_num - 1) )
            {
                  chan_desense = DESENSE_CHAN_PLUS_1;
                  break;
            }
            else if ((public_data_ptr->desense_chanel.G900_chan_list[i] == 0) &&(chanel_num == 1023))
            {
                  chan_desense = DESENSE_CHAN_MINUS_1;
                  break;
            }
            else if ((public_data_ptr->desense_chanel.G900_chan_list[i] == 1023) && (chanel_num == 0))
            {
                  chan_desense = DESENSE_CHAN_PLUS_1;
                  break;
             }
         }
      }
      break;

    case SYS_BAND_DCS_1800:
      for ( i = 0; i < MAX_NMBR_DESENSE_CHNL_PER_BAND ; i++)
      {
          if ( public_data_ptr->desense_chanel.G1800_chan_list[i] == (LAST_ELEMNT_IN_CHNL_LIST) )
          {
             break;
          }
          else if ( public_data_ptr->desense_chanel.G1800_chan_list[i] == chanel_num )
          {
              chan_desense = DESENSE_CHAN;
              break;
          }
          else if ( public_data_ptr->desense_chanel.G1800_chan_list[i] == (chanel_num + 1) )
          {
              chan_desense = DESENSE_CHAN_MINUS_1;
              break;
          }
          else if ( public_data_ptr->desense_chanel.G1800_chan_list[i] == (chanel_num - 1) )
          {
              chan_desense = DESENSE_CHAN_PLUS_1;
              break;
          }
      }
      break;

    case SYS_BAND_PCS_1900:
      for ( i = 0; i < MAX_NMBR_DESENSE_CHNL_PER_BAND ; i++)
      {
          if ( public_data_ptr->desense_chanel.G1900_chan_list[i] == (LAST_ELEMNT_IN_CHNL_LIST) )
          {
             break;
          }
          else if ( public_data_ptr->desense_chanel.G1900_chan_list[i] == chanel_num )
          {
              chan_desense = DESENSE_CHAN;
              break;
          }
          else if ( public_data_ptr->desense_chanel.G1900_chan_list[i] == (chanel_num + 1) )
          {
              chan_desense = DESENSE_CHAN_MINUS_1;
              break;
          }
          else if ( public_data_ptr->desense_chanel.G1900_chan_list[i] == (chanel_num - 1) )
          {
              chan_desense = DESENSE_CHAN_PLUS_1;
              break;
          }
      }
      break;

    case SYS_BAND_CELL_850:
      for ( i = 0; i < MAX_NMBR_DESENSE_CHNL_PER_BAND ; i++)
      {
          if ( public_data_ptr->desense_chanel.G850_chan_list[i] == (LAST_ELEMNT_IN_CHNL_LIST) )
          {
             break;
          }
          else if ( public_data_ptr->desense_chanel.G850_chan_list[i] == chanel_num )
          {
              chan_desense = DESENSE_CHAN;
              break;
          }
          else if ( public_data_ptr->desense_chanel.G850_chan_list[i] == (chanel_num + 1) )
          {
              chan_desense = DESENSE_CHAN_MINUS_1;
              break;
           }
           else if ( public_data_ptr->desense_chanel.G850_chan_list[i] == (chanel_num - 1) )
           {
              chan_desense = DESENSE_CHAN_PLUS_1;
              break;
           }
      }
      break;

    case  SYS_BAND_NONE:
    case  INVALID_BAND :
    default:
      break;
  }

#ifndef FEATURE_GSM_ADJACENT_SPUR_CHANNEL
  if ((chan_desense == DESENSE_CHAN_PLUS_1) || (chan_desense == DESENSE_CHAN_MINUS_1))
  {
      chan_desense = NOT_DESENSE_CHAN;
  }
#endif

  if (chan_desense != NOT_DESENSE_CHAN)
  {
      MSG_GERAN_HIGH_2_G("chan_desense:%d arfcn:%d", chan_desense, chanel_num );
  }

  return (chan_desense);
}
/*===========================================================================

FUNCTION mdsp_set_iq_log

DESCRIPTION
  This function is called by rf to set fw flag for iq samples

============================================================================*/

void mdsp_set_iq_log_gas_id (  boolean value , gas_id_t gas_id ) /*GERAN use (gl1_hw.c) */
{
  mdsp_intf_public_data_t* public_data_ptr;

  public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

  public_data_ptr->gsm_Iq_Log.acq_log_iq = value ;
  public_data_ptr->gsm_Iq_Log.sch_log_iq = value ;
  public_data_ptr->gsm_Iq_Log.gprs_pwr_mea_iq = value ;
  public_data_ptr->gsm_Iq_Log. x2g_pwr_meas_iq = value;
  public_data_ptr->gsm_Iq_Log. sync_rx_iq  = value;
}

/*===========================================================================

FUNCTION mdsp_rf_set_iq_log

DESCRIPTION
  This function is called by RF to set fw flag for iq samples in Multisim targets

============================================================================*/
void mdsp_rf_set_iq_log(boolean value,sys_modem_as_id_e_type as_id)
{
  mdsp_set_iq_log_gas_id(value, geran_map_nas_id_to_gas_id(as_id) );
}
/*===========================================================================

FUNCTION mdsp_send_iq_samples_to_rf

DESCRIPTION
  This function is called byGL1  to send iq samples to rf

============================================================================*/
void mdsp_send_iq_samples_to_rf( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

  fw_rf_common_intf_s *fw_rf_common_intf_ptr;
  static GfwIqLoggingMemoryRegion *pGfwIqLogBuffer; /* static_dual_spaced_ignore */

  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  fw_rf_common_intf_ptr = (fw_rf_common_intf_s *) FW_SMEM_COMMON_FW_RF_ADDR;
  pGfwIqLogBuffer= (GfwIqLoggingMemoryRegion*)&fw_rf_common_intf_ptr->rf_log_buffer;

  if( public_data_ptr->gsm_Iq_Log.acq_log_iq == TRUE )
  {
     MSG_GERAN_HIGH_1_G(" current_iq_sample buffer  = %d ",iq_buf_index);

     /* sending samples to rf  */
     rfm_log_iq_data((GfwIqSamplesBuffer*)&(pGfwIqLogBuffer->buffer[iq_buf_index][0]));
  }
}

/*===========================================================================

FUNCTION mdsp_rf_send_iq_samples_to_rf

DESCRIPTION
  This function is called by RF drivers  to retrieve IQ samples

============================================================================*/
void mdsp_rf_send_iq_samples_to_rf(sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  mdsp_send_iq_samples_to_rf(as_id);
  mdsp_send_iq_samples_to_tcxomgr();

  /* Toggle iq buffer index if IQ logging is enabled either
     *  for sending samples to RF or TCXO manager */
  if((public_data_ptr->gsm_Iq_Log.acq_log_iq )|(iq_logging))
  {
  mdsp_toggle_iq_buf_index();
  }
}

/*===========================================================================

FUNCTION mdsp_get_gfw_event_buffer

DESCRIPTION
  Returns pointer to GFW event buffer

DEPENDENCIES
  None

RETURN VALUE
  void *

SIDE EFFECTS
  None
===========================================================================*/
void* mdsp_get_gfw_event_buffer(mdsp_burst_mode burst_mode,uint8 burst_index, gas_id_t gas_id)
{
   void            *pGfwEvent = NULL;

   mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
   mdsp_intf_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id );

   if(private_data_ptr->current_buffer_mod3 <= MDSP_MAX_CURRENT_BUFFER_INDEX)
   {
     if(burst_mode == RX_MODE)
     {
       pGfwEvent = (void *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.burstEvent[private_data_ptr->current_buffer_mod3].rxEvent[GFW_SYNC_RX_EVENT_INDEX]);
     }
     else if(burst_mode == MON_MODE)
     {
       pGfwEvent = (void *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.burstEvent[private_data_ptr->current_buffer_mod3].monEvent[burst_index]);
     }
     else if(burst_mode == TX_MODE)
     {
       pGfwEvent = (void *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.burstEvent[private_data_ptr->current_buffer_mod3].txEvent);
     }
     else if(burst_mode == ACQ_MODE)
     {
       pGfwEvent = (void *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.burstEvent[private_data_ptr->current_buffer_mod3].rxEvent[GFW_SCH_OR_ACQ_RX_EVENT_INDEX]);
     }
     else if(burst_mode == RX_MODE_ASYNC)
     {
       pGfwEvent = (void *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.burstEvent[private_data_ptr->current_buffer_mod3].rxEvent[GFW_PCH_INDEX]);
     }
   }

   return pGfwEvent;
}
/*===========================================================================

FUNCTION mdsp_get_gfw_init_event_buffer

DESCRIPTION
  Returns pointer to GFW init event buffer

DEPENDENCIES
  None

RETURN VALUE
  void *

SIDE EFFECTS
  None
===========================================================================*/
GfwRfInitEvent* mdsp_get_gfw_init_event_buffer(gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
  GfwRfInitEvent  *pGfwRfInitEvent = (GfwRfInitEvent *)&public_data_ptr->pMdspHostIfBaseStore->rfEventTable.rfInitEvent;

  return (pGfwRfInitEvent);
}


#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION mdsp_get_gfw_init_event_buffer_DivRx

DESCRIPTION
  Returns pointer to GFW init event buffer for Diverity Rx

DEPENDENCIES
  None

RETURN VALUE
  void *

SIDE EFFECTS
  None
===========================================================================*/
GfwRfInitEvent* mdsp_get_gfw_init_event_buffer_DivRx(gas_id_t gas_id)	
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
  GfwRfInitEvent  *pGfwRfInitEvent = (GfwRfInitEvent *)&public_data_ptr->pMdspHostIfBaseStore->rfEventTable.rfInitEvent;

  return (pGfwRfInitEvent);
}
#endif /* FEATURE_GSM_RX_DIVERSITY */

/*===========================================================================

FUNCTION mdsp_get_gfw_tx_band_event_buffer

DESCRIPTION
  Returns pointer to GFW tx band event buffer

DEPENDENCIES
  None

RETURN VALUE
  void *

SIDE EFFECTS
  None
===========================================================================*/
volatile GfwRfSetTxBandEvent* mdsp_get_gfw_tx_band_event_buffer(gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
  GfwRfSetTxBandEvent *pGfwRfSetTxBandEvent = (GfwRfSetTxBandEvent *)&(public_data_ptr->pMdspHostIfBaseStore->rfEventTable.setTxBandEvent);

  return (pGfwRfSetTxBandEvent);
}

/*===========================================================================

FUNCTION mdsp_get_gfw_therm_read_event_buffer

DESCRIPTION
  Returns pointer to GFW thermistor read CCS event buffer

DEPENDENCIES
  None

RETURN VALUE
  void *

SIDE EFFECTS
  None
===========================================================================*/
GfwRfThermReadEvent* mdsp_get_gfw_therm_read_event_buffer(gas_id_t gas_id)
{
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  GfwRfThermReadEvent  *pGfwRfThermReadEvent = (GfwRfThermReadEvent *)&public_data_ptr->pMdspHostIfBaseStore->rfEventTable.thermReadEvent;

  return (pGfwRfThermReadEvent);
}

/*===========================================================================

FUNCTION mdsp_set_iq_log_enable_flag

DESCRIPTION
  Enables/disables iq_log_enable flag.

============================================================================*/
void mdsp_set_iq_log_flag( boolean value)
{
  iq_logging = value;
}

/*============================================================================

FUNCTION MDSP_REGISTER_IQ_SAMPLES_CALLBACK

DESCRIPTION
  Function to register for callbacks.

  Calling this API with a valid callback function pointer signifies
  start of IQ capture. And GL1 reports the captured IQ samples to TCXO manager
  at the end of every frame via the call back pointer.The GL1 falg which indicates
  that IQ capture has to be enabled in FW is set when FTM calls GL1 API to start
  acquisition.

  Calling this API with a NULL pointer signifies stopping of IQ capture. The GL1
  flag which indicates that IQ capture needs to be stopped in GFW is disabled when
  FTM calls GL1 API to stop acquisition.

  Please note that FTM calls GL1 APIs to start/stop acquisition before TCXO manager
  registers/deregisters itself with GL1 for IQ samples.

RETURN VALUE
  TRUE  - Success.
  FALSE - Failure

============================================================================*/
boolean mdsp_register_iq_samples_callback ( mdsp_collect_iq_samples_cb_t callback )
{
  if(callback != NULL)
   {
     mdsp_send_iqsamples_to_mcs_ptr = callback;
   }
  else
   {
     mdsp_send_iqsamples_to_mcs_ptr = NULL;
   }
  return (TRUE);
}

/*===========================================================================

FUNCTION mdsp_send_iq_samples_to_tcxomgr

DESCRIPTION
  This function is called by GL1  to send iq samples to tcxo manager

============================================================================*/
static void mdsp_send_iq_samples_to_tcxomgr( void )
{
  fw_rf_common_intf_s *fw_rf_common_intf_ptr;
  static GfwIqLoggingMemoryRegion *pGfwIqLogBuffer;
  fw_rf_common_intf_ptr = (fw_rf_common_intf_s *) FW_SMEM_COMMON_FW_RF_ADDR;
  pGfwIqLogBuffer= (GfwIqLoggingMemoryRegion*)&fw_rf_common_intf_ptr->rf_log_buffer;

  /*If TCXO manager has registered with a valid call back pointer, send IQ samples.*/
  if(mdsp_send_iqsamples_to_mcs_ptr != NULL )
  {
     if ( pGfwIqLogBuffer != NULL ) {
          MSG_HIGH (" current_iq_sample buffer = %d ",iq_buf_index,0,0);
          (*mdsp_send_iqsamples_to_mcs_ptr)((GfwIqSamplesBuffer*)&(pGfwIqLogBuffer->buffer[iq_buf_index][0]));
     }else{
           MSG_HIGH ("Not Sending iq_samples_to_tcxomg  mdsp_send_iqsamples_to_mcs_ptr = %d ,  pGfwIqLogBuffer = %d ", mdsp_send_iqsamples_to_mcs_ptr, pGfwIqLogBuffer , 0);
     }
   }
}

/*===========================================================================

FUNCTION mdsp_toggle_iq_buf_index

DESCRIPTION
  Toggles iq_buf_index every frame

============================================================================*/
static void mdsp_toggle_iq_buf_index( void )
{
  /*GFW logs the IQ samples into a pingpong buffer. GL1 toggles the iq buffer*/
  /*index in order to read the IQ samples from correct FW buffer.*/
  if(iq_buf_index == 0)
  {
    iq_buf_index = 1;
  }
  else
  {
    iq_buf_index = 0;
  }
}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION mdsp_process_cxm_logs

DESCRIPTION
  Logs the coexistence manager information returned by firmware

============================================================================*/
void mdsp_process_cxm_logs(gas_id_t gas_id)
{
  int i, j;
  uint32 valid_buffers[TRIPPLEBUF];
  uint32 num_valid_buffers = 0;

  volatile GfwCxmLogPacketBuff* cxm_log_buf_ptr;
  mdsp_intf_public_data_t* mdsp_intf_ptr = get_mdsp_intf_public_dataspace_ptr(gas_id); 

  /* Get pointer to the cxm logs buffers */
  cxm_log_buf_ptr = mdsp_intf_ptr->pMdspHostIfBaseStore->gfwResultBuffers.gfwCxmLogBuffer;

  /* Search for buffers with valid entries */
  for (i = 0; i < TRIPPLEBUF; i++)
  {
    if (cxm_log_buf_ptr[i].numEntries > 0)
    {
      valid_buffers[num_valid_buffers] = i;
      num_valid_buffers++;
    }
  }

  /* if no buffer contains valid entries, we're done */
  if (num_valid_buffers == 0)
    return;

  /* If more than one buffer contain valid entries, sort based on FN */
  /* sorting algorith based on bubble sort */
  if (num_valid_buffers > 1)
  {
    for (i = 0; i < num_valid_buffers - 1; i++)
    {
      for (j = 0; j < num_valid_buffers - 1; j++)
      {
        if (SUB_FN(cxm_log_buf_ptr[valid_buffers[j + 1]].gsmFrameNo, 
                   cxm_log_buf_ptr[valid_buffers[j]].gsmFrameNo) > FRAMES_PER_HYPERFRAME / 2)
        {
          int temp = valid_buffers[j];
          valid_buffers[j] = valid_buffers[j + 1];
          valid_buffers[j + 1] = temp;
        }
      }
    }
  }

  /* Log all the valid buffers */
  for (i = 0; i < num_valid_buffers; i++)
  {
#ifdef FEATURE_GSM_LOG_COEX
    l1_log_coex_params(&cxm_log_buf_ptr[valid_buffers[i]], gas_id);
#endif
    /* Clear number of entries of current buffer to indicate fw gl1 read the buffer */
    cxm_log_buf_ptr[valid_buffers[i]].numEntries = 0;
  }
}
#endif /* FEATURE_GSM_COEX */

void antenna_swth_get_nv_item_qsc ( )
{
#ifdef FEATURE_GSM_QSC_TX_DIV
   uint8 gasid;

   /*Initializing local vars holding EFS Data to 0*/
   uint8 idle_suspension_time_local        =0;
   uint8 traffic_crisis_mode_thre_local    =0;
   uint8 idle_ping_pong_mitigation_thre_local =0;
   int16 l_trafic_mdm_threshold            =0;
   int16 l_idle_mdm_threshold              =0;
   int16 l_gsm_trafi_sens                  =0;
   int16 l_gsm_idle_sens                   =0;
   int16 gsm_qsc_large_delta_local         =0;
   int16 gsm_qsc_small_delta_local         =0;
   int16 gsm_qsc_hysteriesis_time_l1_local =0;
   boolean tx_div_type2_enable_local       =FALSE;

   MSG_HIGH (" antenna_swth_get_nv_item_qsc  ",0,0,0);
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_T2_IDL_SUSP_TIME, &idle_suspension_time_local, sizeof(idle_suspension_time_local));
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_T2_TRFC_CRISIS_MODE_THRESH, &traffic_crisis_mode_thre_local, sizeof(traffic_crisis_mode_thre_local));
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_T2_IDL_PINGPONG_MIT_THRESH, &idle_ping_pong_mitigation_thre_local, sizeof(idle_ping_pong_mitigation_thre_local));
 
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_T2_EN, &tx_div_type2_enable_local, sizeof(tx_div_type2_enable_local)) ;
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_TRAFI_MDM_TH, &l_trafic_mdm_threshold, sizeof(l_trafic_mdm_threshold)) ;
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_IDLE_MDM_TH, &l_idle_mdm_threshold, sizeof(l_idle_mdm_threshold)) ;   
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_TRAFI_SENS, &l_gsm_trafi_sens, sizeof(l_gsm_trafi_sens)) ;   
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_IDLE_SENS, &l_gsm_idle_sens, sizeof(l_gsm_idle_sens)) ;   
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_QSC_LARGE_DELTA, &gsm_qsc_large_delta_local, sizeof(gsm_qsc_large_delta_local)) ;   
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_QSC_SMALL_DELTA, &gsm_qsc_small_delta_local, sizeof(gsm_qsc_small_delta_local)) ;   
   geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_QSC_HYST_TIME, &gsm_qsc_hysteriesis_time_l1_local, sizeof(gsm_qsc_hysteriesis_time_l1_local)) ;   

   
#ifdef FEATURE_DUAL_SIM
  for(gasid = 0; gasid < NUM_GERAN_DATA_SPACES; gasid++)
  {

    /*Default Values for Traffic Safe Condition*/
    if(l_trafic_mdm_threshold >= 0)
    {
      trafic_mdm_threshold[gasid] = 42;
    }
    else
    {
      trafic_mdm_threshold[gasid] = 111 + l_trafic_mdm_threshold;
    }
    
    /*Default Values for Idle Safe Condition*/
    if(l_idle_mdm_threshold >= 0)
    {
      idle_mdm_threshold[gasid] = -79;
    }
    else
    {
      idle_mdm_threshold[gasid] = l_idle_mdm_threshold;
    }
    
    /*Default Values for Traffic Switch Threshold*/
    if(l_gsm_trafi_sens >= 0)
    {
      gsm_trafi_sens[gasid] = 21;
    }
    else
    {
      gsm_trafi_sens[gasid] = 111+l_gsm_trafi_sens;
    }
    
    /*Default Values for Idle Switch Threshold*/
    if(l_gsm_idle_sens >= 0)
    {
      gsm_idle_sens[gasid] = -100;
    }
    else
    {
      gsm_idle_sens[gasid] = l_gsm_idle_sens;
    }
    trafic_mdm_threshold[gasid] = 111+l_trafic_mdm_threshold;
    idle_mdm_threshold[gasid] = l_idle_mdm_threshold;
    gsm_trafi_sens[gasid] = 111+l_gsm_trafi_sens;
    gsm_idle_sens [gasid] = l_gsm_idle_sens;
    tx_div_type2_enable[gasid] = tx_div_type2_enable_local;
    gsm_qsc_large_delta[gasid] = gsm_qsc_large_delta_local;
    gsm_qsc_small_delta[gasid] = gsm_qsc_small_delta_local;
    gsm_qsc_hysteriesis_time_l1[gasid] = gsm_qsc_hysteriesis_time_l1_local;
    MSG_HIGH (" tx_div_type2_enable = %d for gas_id = %d",tx_div_type2_enable[gasid], gasid, 0);
    MSG_GERAN_HIGH_3("mitigation_thre=%d, crisis_mod_thre=%d, suspension_time=%d", idle_ping_pong_mitigation_thre_local, 
																							traffic_crisis_mode_thre_local,
																							idle_suspension_time_local);
   gsm_qsc_idle_pingpong_mitigation_thre[gasid] = idle_ping_pong_mitigation_thre_local;
   gsm_qsc_traffic_crisis_mode_thre [gasid] = traffic_crisis_mode_thre_local;	
   gsm_qsc_idle_suspension_time[gasid] = idle_suspension_time_local;
	}

#else
  trafic_mdm_threshold[0] = 111+l_trafic_mdm_threshold;
  idle_mdm_threshold[0] = l_idle_mdm_threshold;
  gsm_trafi_sens[0] = 111+l_gsm_trafi_sens;
  gsm_idle_sens[0] = l_gsm_idle_sens;
  tx_div_type2_enable[0] = tx_div_type2_enable_local;
  gsm_qsc_large_delta[0] = gsm_qsc_large_delta_local;
  gsm_qsc_small_delta[0] = gsm_qsc_small_delta_local;
  gsm_qsc_hysteriesis_time_l1[0] = gsm_qsc_hysteriesis_time_l1_local;
  MSG_HIGH (" tx_div_type2_enable = %d ",tx_div_type2_enable[0] ,0,0);
#endif   

#endif /*FEATURE_GSM_QSC_TX_DIV*/
}

/************************************************************************************************** 
 
 FUNCTION mdsp_rf_debug_efs_read

 DESCRIPTION
    Read the rf debug nvs 
************************************************************************************************/


void mdsp_rf_debug_efs_read (void)
{
#ifdef FEATURE_GSM_RF_DEBUG_NV
  int16   Rf_debug_upper_power_limit;
  int16   Rf_debug_lower_power_limit;
  int8    Rf_debug_band;
  int32   Rf_debug_arfcn;
#ifdef FEATURE_DUAL_SIM  
  uint8   gasid ;
#endif   
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable", &mdsp_rf_debug_NV_enable_g[0], sizeof(mdsp_rf_debug_NV_enable_g[0])) ;
#ifdef FEATURE_DUAL_SIM   
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable_sub2", &mdsp_rf_debug_NV_enable_g[1], sizeof(mdsp_rf_debug_NV_enable_g[1])) ;
#ifdef FEATURE_TRIPLE_SIM  
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable_sub3", &mdsp_rf_debug_NV_enable_g[2], sizeof(mdsp_rf_debug_NV_enable_g[2])) ;
#endif    
#endif  
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_upper_power_limit", &Rf_debug_upper_power_limit, sizeof(Rf_debug_upper_power_limit)) ;
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_Lower_Limit", &Rf_debug_lower_power_limit, sizeof(Rf_debug_lower_power_limit)) ;
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_band", &Rf_debug_band, sizeof(Rf_debug_band)) ;
  geran_efs_read_primary("/nv/item_files/gsm/gl1/RF_Debug_arfcn", &Rf_debug_arfcn, sizeof(Rf_debug_arfcn)) ;    

#ifdef FEATURE_DUAL_SIM
	for(gasid = 0; gasid < NUM_GERAN_DATA_SPACES; gasid++)
	{

   mdsp_rf_debug_upper_power_limit_g[gasid] = Rf_debug_upper_power_limit;
   mdsp_rf_debug_lower_power_limit_g[gasid] = Rf_debug_lower_power_limit;
   mdsp_rf_debug_band_g [gasid]             = Rf_debug_band;
   mdsp_rf_debug_arfcn_g[gasid]             = Rf_debug_arfcn;

	}
#else

   mdsp_rf_debug_upper_power_limit_g[0] = Rf_debug_upper_power_limit;
   mdsp_rf_debug_lower_power_limit_g[0] = Rf_debug_lower_power_limit;
   mdsp_rf_debug_band_g [0]             = Rf_debug_band;
   mdsp_rf_debug_arfcn_g[0]             = Rf_debug_arfcn;
#endif 
#endif /* FEATURE_GSM_RF_DEBUG_NV */
}

#ifdef FEATURE_GSM_RF_DEBUG_NV
void set_flag_for_fw_pwr_meas_crash (uint16 index , boolean flag ,gas_id_t gas_id )
{
    if ( index <= MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS ) 
    {
         flag_for_fw_pwr_meas_crash[gas_id][index]= flag ;
    }
    else
    {
        MSG_GERAN_ERROR_1_G(" wrong  index %d ",index);
    }
}

boolean get_flag_for_fw_pwr_meas_crash (uint16 index  ,gas_id_t gas_id )
{
    if ( index <= MDSP_MAX_STANDALONE_RSSI_MEASUREMENTS) 
    {
         return flag_for_fw_pwr_meas_crash[gas_id][index] ;
    }
    else
    {
        MSG_GERAN_ERROR_1_G(" wrong  index %d ",index);
        return FALSE ;
    }
}
#endif /* FEATURE_GSM_RF_DEBUG_NV */

#ifdef FEATURE_QSH_DUMP
/*===========================================================================

FUNCTION mdsp_send_gfw_data_dump_req

DESCRIPTION
   This function populates dump address and invokes IPC to populate data.by GFW

===========================================================================*/
// Send GFW Data sump Req via IPC
void mdsp_send_gfw_data_dump_req(uint8 *dump_data_ptr1,uint8 *dump_data_ptr2,uint8 *gfw_result)
{
   mdsp_intf_public_data_t* public_data_ptr_1 = NULL;
#ifdef FEATURE_DUAL_SIM
   mdsp_intf_public_data_t* public_data_ptr_2 = NULL;
#endif /*   FEATURE_DUAL_SIM */ 

   public_data_ptr_1 = get_mdsp_intf_public_dataspace_ptr( GERAN_ACCESS_STRATUM_ID_1 );

#ifdef FEATURE_DUAL_SIM
   public_data_ptr_2 = get_mdsp_intf_public_dataspace_ptr( GERAN_ACCESS_STRATUM_ID_2 );
#endif /*   FEATURE_DUAL_SIM */ 

   if (public_data_ptr_1 != NULL)
   {
     if (public_data_ptr_1->pMdspHostIfBaseStore != NULL)
     {
       if (dump_data_ptr1)
       {
         public_data_ptr_1->pMdspHostIfBaseStore->gfwDumpCmd.gfwDumpAddress  = dump_data_ptr1;
         public_data_ptr_1->pMdspHostIfBaseStore->gfwDumpCmd.gfwDump         = TRUE;
         gfw_result[0] = TRUE;
       }
       else 
       {
         public_data_ptr_1->pMdspHostIfBaseStore->gfwDumpCmd.gfwDumpAddress  = NULL;
         public_data_ptr_1->pMdspHostIfBaseStore->gfwDumpCmd.gfwDump         = FALSE;
       }  
     }
   }
   
#ifdef FEATURE_DUAL_SIM
   if (public_data_ptr_2 != NULL)
   {
     if (public_data_ptr_2->pMdspHostIfBaseStore != NULL)
     {
       if (dump_data_ptr2)
       {
         public_data_ptr_2->pMdspHostIfBaseStore->gfwDumpCmd.gfwDumpAddress  = dump_data_ptr2;
         public_data_ptr_2->pMdspHostIfBaseStore->gfwDumpCmd.gfwDump         = TRUE;
         gfw_result[1] = TRUE;
       }
       else
       {
         public_data_ptr_2->pMdspHostIfBaseStore->gfwDumpCmd.gfwDumpAddress  = NULL;
         public_data_ptr_2->pMdspHostIfBaseStore->gfwDumpCmd.gfwDump         = FALSE;
       }
     }
   }

#endif /*   FEATURE_DUAL_SIM */ 

   /* Call IPC to get DAta from GFW */
   if ( (gfw_result[0] == TRUE)
#ifdef FEATURE_DUAL_SIM
        ||(gfw_result[1] == TRUE)
#endif /*   FEATURE_DUAL_SIM */
      )
   {
     /* Update the GFW shared memory with dump pointer information */
     fws_ipc_send(FWS_IPC_SW_FW__GERAN_CMD_GFW_DUMP); 
     MSG_HIGH ("gfw_data_dump_req IPC SEND to GFW ",0 ,0,0);
   }
   else
   {
     MSG_HIGH ("gfw_data_dump_req IPC NOT SEND as pointers are NULL ",0 ,0,0);
   }
}
#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_GERAN_RF_SELF_TEST 
/*===========================================================================

FUNCTION mdsp_set_selftest_params

DESCRIPTION
  Function to store selftest params into mdsp public 

===========================================================================*/
void mdsp_set_selftest_params
(   
    sys_modem_as_id_e_type as_id, 
    boolean enable, 
    uint32 buf_index,	
    fw_rf_capture_param_t *cap_param,	
    fw_rf_capture_config_t *cap_config)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );

  MSG_2( MSG_SSID_FTM, MSG_LEGACY_HIGH,
         "mdsp_set_selftest_params, enable = %d, buf_index = %d", 
  	     enable,
  	     buf_index
       );
  
  public_data_ptr->gsm_selftest_params.selftest_flag = enable;
  public_data_ptr->gsm_selftest_params.fbRxlBufIndex = buf_index;
  memscpy( &public_data_ptr->gsm_selftest_params.selfTestCmdParam, 
           sizeof(fw_rf_capture_param_t), cap_param, 
           sizeof(fw_rf_capture_param_t));

  memscpy( &public_data_ptr->gsm_selftest_params.selfTestconfig, 
           sizeof(fw_rf_capture_config_t), cap_config, 
           sizeof(fw_rf_capture_config_t));

  MSG_4( MSG_SSID_FTM, MSG_LEGACY_HIGH, 
         "public_data_ptr, enable = %d, buf_index = %d, seq_num = %d, num_samp = %d",
  	     public_data_ptr->gsm_selftest_params.selftest_flag,
  	     public_data_ptr->gsm_selftest_params.fbRxlBufIndex = buf_index,
  	     public_data_ptr->gsm_selftest_params.selfTestCmdParam.seq_num,
  	     public_data_ptr->gsm_selftest_params.selfTestconfig.num_samp
  	   );
    
}
/*===========================================================================

FUNCTION mdsp_get_selftest_params

DESCRIPTION
  Function to get selftest params into mdsp public 

===========================================================================*/
void mdsp_get_selftest_params
(   
     sys_modem_as_id_e_type as_id, 
     boolean *enable, 
     uint32 *buf_index,	
     fw_rf_capture_param_t **cap_param,	
     fw_rf_capture_config_t **cap_config)
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id); 
  mdsp_intf_public_data_t*  public_data_ptr  = get_mdsp_intf_public_dataspace_ptr( gas_id );
  
  *enable = public_data_ptr->gsm_selftest_params.selftest_flag;
  *buf_index = public_data_ptr->gsm_selftest_params.fbRxlBufIndex;
  *cap_param = &public_data_ptr->gsm_selftest_params.selfTestCmdParam;
  *cap_config = &public_data_ptr->gsm_selftest_params.selfTestconfig;
}
#endif /*FEATURE_GERAN_RF_SELF_TEST*/

/*===========================================================================

FUNCTION mdsp_get_shared_variable

DESCRIPTION
  This function returns the rx alpha state in the GFW structure. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE - Using idle (longer) rx alpha.
  FALSE - Using legacy rx alpha.

SIDE EFFECTS
  None
===========================================================================*/
boolean mdsp_get_rx_alpha_state (gas_id_t gas_id)
{
  mdsp_intf_public_data_t* public_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id );

  return public_data_ptr->pMdspHostIfBaseStore->gfwSharedVariables.rx.useIdleModeAlpha;
}


/* EOF */
